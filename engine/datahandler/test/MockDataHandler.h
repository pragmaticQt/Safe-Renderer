#ifndef LUXOFTSAFERENDERER_MOCKDATAHANDLER_H
#define LUXOFTSAFERENDERER_MOCKDATAHANDLER_H

/******************************************************************************
**
**   File:        MockDataHandler.h
**   Description:
**
**   Copyright (C) 2017 Luxoft GmbH
**
**   This file is part of Luxoft Safe Renderer.
**
**   Luxoft Safe Renderer is free software: you can redistribute it and/or
**   modify it under the terms of the GNU Lesser General Public
**   License as published by the Free Software Foundation.
**
**   Safe Render is distributed in the hope that it will be useful,
**   but WITHOUT ANY WARRANTY; without even the implied warranty of
**   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
**   Lesser General Public License for more details.
**
**   You should have received a copy of the GNU Lesser General Public
**   License along with Safe Render.  If not, see
**   <http://www.gnu.org/licenses/>.
**
**   SPDX-License-Identifier: LGPL-3.0
**
******************************************************************************/


#include <IDataHandler.h>
#include <Number.h>
#include <DynamicDataType.h>

#include <gmock/gmock.h>
#include <map>

struct DataEntrance
{
    FUClassId fuID;
    DataId dataID;

    DataEntrance()
    : fuID(0U), dataID(0U)
    {}

    DataEntrance(FUClassId fu, DataId data)
    : fuID(fu), dataID(data)
    {}

    friend bool operator<(const DataEntrance& lhs, const DataEntrance& rhs);
    friend bool operator==(const DataEntrance& lhs, const DataEntrance& rhs);
};

inline bool operator<(const DataEntrance& lhs, const DataEntrance& rhs)
{
    return (lhs.fuID < rhs.fuID || lhs.dataID < rhs.dataID);
}

inline bool operator==(const DataEntrance& lhs, const DataEntrance& rhs)
{
    return (lhs.fuID == rhs.fuID && lhs.dataID == rhs.dataID);
}

struct DataValue
{
    lsr::DataStatus status;
    lsr::Number number;

    DataValue()
    : status(lsr::DataStatus::INCONSISTENT)
    {}

    DataValue(lsr::DataStatus st, lsr::Number num)
    : status(st), number(num)
    {}

    friend bool operator<(const DataValue& lhs, const DataValue& rhs);
    friend bool operator==(const DataValue& lhs, const DataValue& rhs);
};

inline bool operator<(const DataValue& lhs, const DataValue& rhs)
{
    return (lhs.status.getValue() < rhs.status.getValue() ||
            lhs.number < rhs.number);
}

inline bool operator==(const DataValue& lhs, const DataValue& rhs)
{
    return (lhs.status.getValue() == rhs.status.getValue() &&
            lhs.number == rhs.number);
}

class MockDataHandler: public lsr::IDataHandler
{
public:
    MockDataHandler()
    {}

    MockDataHandler(lsr::Number number)
    {
        setNumber(number);
    }

    MOCK_METHOD3(subscribeData, bool (FUClassId, DataId, lsr::IDataHandler::IListener*));
    MOCK_METHOD3(subscribeIndication, bool (FUClassId, IndicationId, lsr::IDataHandler::IListener*));
    MOCK_METHOD3(unsubscribeData, void (FUClassId, DataId, lsr::IDataHandler::IListener*));
    MOCK_METHOD3(unsubscribeIndication, void (FUClassId, IndicationId, lsr::IDataHandler::IListener*));
    MOCK_METHOD4(setData, bool(FUClassId, DataId, const lsr::Number&, lsr::DataStatus));

    void setNumber(const lsr::Number& number)
    {
        m_commonData.number = number;
        m_commonData.status = lsr::DataStatus::VALID;
    }

    void setNumber(const lsr::Number& number,
                   FUClassId fuClassId,
                   DataId dataId,
                   lsr::DataStatus status)
    {
        DataEntrance de(fuClassId, dataId);
        DataValue dv(status, number);
        m_data[de] = dv;
    }

    void setOutDatedNumber(const lsr::Number& number)
    {
        m_commonData.number = number;
        m_commonData.status = lsr::DataStatus::NOT_AVAILABLE;
    }

    void setInvalidNumber(const lsr::Number& number)
    {
        m_commonData.number = number;
        m_commonData.status = lsr::DataStatus::INVALID;
    }

    virtual lsr::DataStatus getNumber(FUClassId fuClassId,
                                      DataId dataId,
                                      lsr::Number &value) const P_OVERRIDE
    {
        lsr::DataStatus status;

        m_lastFuId = fuClassId;
        m_lastDataId = dataId;

        DataEntrance de(fuClassId, dataId);

        std::map<DataEntrance, DataValue>::const_iterator it = m_data.find(de);

        if (it != m_data.end())
        {
            value = it->second.number;
            status = it->second.status;
        }
        else
        {
            value = m_commonData.number;
            status = m_commonData.status;
        }

        return status;
    }

    virtual lsr::DataStatus getIndication(FUClassId fuClassId,
                                          IndicationId indicationId,
                                          bool& value) const P_OVERRIDE
    {
        m_lastFuId = fuClassId;
        m_lastIndicationId = indicationId;
        value = true;

        return lsr::DataStatus::VALID;
    }

    FUClassId getLastFuId() const
    {
        return m_lastFuId;
    }

    DataId getLastDataId() const
    {
        return m_lastDataId;
    }

    IndicationId getLastIndicationId() const
    {
        return m_lastIndicationId;
    }

private:
    mutable FUClassId m_lastFuId;
    mutable DataId m_lastDataId;
    mutable IndicationId m_lastIndicationId;

    std::map<DataEntrance, DataValue> m_data;
    DataValue m_commonData;
};


#endif // LUXOFTSAFERENDERER_MOCKDATAHANDLER_H