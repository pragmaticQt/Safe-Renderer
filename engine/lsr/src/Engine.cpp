/******************************************************************************
**
**   File:        Engine.cpp
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

#include "Engine.h"
#include "OdiTypes.h"
#include <pil.h>

namespace lsr
{

Engine::Engine(const Database& db, IMsgDispatcher& msgDispatcher)
: m_msgDispatcher(msgDispatcher)
, m_db(db)
, m_display()
, m_dataHandler(db)
, m_frameHandler(m_db, m_dataHandler, m_display)
, m_error(db.getError())
{
    if (LSR_NO_ERROR == m_error)
    {
        m_error = m_msgDispatcher.registerMsgReceiver(&m_dataHandler, MessageTypes::REGISTRATION);
    }
    if (LSR_NO_ERROR == m_error)
    {
        m_error = m_msgDispatcher.registerMsgReceiver(&m_dataHandler, MessageTypes::ODI);
    }
    if (LSR_NO_ERROR == m_error)
    {
        const bool success = m_frameHandler.start();
        if (!success)
        {
            m_error = m_frameHandler.getError();;
        }
    }
}

bool Engine::render()
{
    m_error = m_msgDispatcher.handleIncomingData(0);
    const U32 monotonicTime = pilGetMonotonicTime();
    m_frameHandler.update(monotonicTime);
    return m_frameHandler.render();
}

bool Engine::verify()
{
    return m_frameHandler.verify();
}

bool Engine::setData(FUClassId fuClassId,
                     DataId dataId,
                     const Number& value,
                     DataStatus status)
{
    return m_dataHandler.setData(fuClassId, dataId, value, status);
}

LSRError Engine::getError()
{
    // TODO: ask each component for errors
    LSRError error = m_error;
    if (LSR_NO_ERROR == error)
    {
        // database errors are permanent
        error = m_db.getError();
        if (LSR_NO_ERROR == error)
        {
            error = m_frameHandler.getError();
        }
    }
    else
    {
        m_error = LSR_NO_ERROR;
    }
    return error;
}

} // namespace lsr