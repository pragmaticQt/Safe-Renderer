/******************************************************************************
**
**   File:        Canvas.cpp
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

#include "Canvas.h"
#include "DisplayManager.h"
#include "gil.h"
#include "Area.h"
#include "Color.h"
#include "Assertion.h"

namespace lsr
{

Canvas::Canvas(DisplayManager& dsp, const U16 width, const U16 height)
: m_dsp(dsp)
, m_width(width)
, m_height(height)
{
}

void Canvas::clear(const Color& color)
{
    const GILContext ctx = m_dsp.getContext();
    const U8 r = color.getRed();
    const U8 g = color.getGreen();
    const U8 b = color.getBlue();
    const U8 a = color.getAlpha();
    static_cast<void>(gilSetColor(ctx, r, g, b, a));  // ignore return value
    gilClear(ctx);
}

void Canvas::drawBitmap(const StaticBitmap& bitmap, const Area& area)
{
    Texture* const t = m_dsp.loadTexture(bitmap);
    ASSERT(t != NULL);
    const GILContext ctx = m_dsp.getContext();
    t->bind(ctx);
    // output coordinates
    const I32 x1 = area.getLeftFP();
    const I32 y1 = area.getTopFP();
    const I32 x2 = area.getRightFP();
    const I32 y2 = area.getBottomFP();
    // TODO: get texture coordinates
    const I32 u1 = 0;
    const I32 v1 = 0;
    const U32 widthFP = (static_cast<U32>(t->getWidth()) - 1U) << 4U;
    const U32 heightFP = (static_cast<U32>(t->getHeight()) - 1U) << 4U;
    const I32 u2 = static_cast<I32>(widthFP);
    const I32 v2 = static_cast<I32>(heightFP);
    gilDrawQuad(ctx, x1, y1, u1, v1, x2, y2, u2, v2);
}

bool Canvas::verify(const StaticBitmap& bitmap, const Area& area)
{
    bool verified = false;
    Texture* const t = m_dsp.loadTexture(bitmap);
    if (NULL != t)
    {
        const GILContext ctx = m_dsp.getContext();
        t->bind(ctx);
        // output coordinates
        const I32 x1 = area.getLeftFP();
        const I32 y1 = area.getTopFP();
        const I32 x2 = area.getRightFP();
        const I32 y2 = area.getBottomFP();
        // TODO: get texture coordinates
        const I32 u1 = 0;
        const I32 v1 = 0;
        const U32 widthFP = (static_cast<U32>(t->getWidth()) - 1U) << 4U;
        const U32 heightFP = (static_cast<U32>(t->getHeight()) - 1U) << 4U;
        const I32 u2 = static_cast<I32>(widthFP);
        const I32 v2 = static_cast<I32>(heightFP);
        const GILBoolean ret = gilVerify(ctx, x1, y1, u1, v1, x2, y2, u2, v2);
        verified = (ret == GIL_TRUE);
    }
    return verified;
}

} // namespace lsr
