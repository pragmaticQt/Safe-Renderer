/******************************************************************************
**
**   File:        DdhPageBuilder.cpp
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

#include "DdhPageBuilder.h"

namespace framehandlertests
{

DdhPageBuilder::DdhPageBuilder()
{}

void DdhPageBuilder::create(U32 panelCount, U16 panelId)
{
    m_factory.create(panelCount);
    for (U32 i = 0U; i < panelCount; ++i)
    {
        m_factory.addPanelId(panelId + i);
    }
}

} // namespace framehandlertests