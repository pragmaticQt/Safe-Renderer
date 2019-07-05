/******************************************************************************
**
**   File:        gil_sw_renderer.c
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

#include "gil_sw_renderer.h"
#include "gil_assert.h"

#include <string.h>

static void checkPreconditions(const GIL_SW_Surface* surf, const uint8_t bpp)
{
    GIL_ASSERT((bpp == 1u) || (bpp == 2u) || (bpp == 3u) || (bpp == 4u)); // we support only 1, 2, 3 and 4 bytes per pixel

    GIL_ASSERT(surf);
    //GIL_ASSERT(surf->x >= 0); // beginning could be negative?
    //GIL_ASSERT(surf->y >= 0);
    GIL_ASSERT(surf->w >= 0);
    GIL_ASSERT(surf->h >= 0);
    // GIL_ASSERT((surf->y+surf->h-1)*surf->alignment+(surf->x+surf->w)*bpp <= surf->bytes); // is rect maximum inside memory boundaries

    // check for correct start memory alignment
    GIL_ASSERT(((bpp == 2u) && (((uint32_t)surf->p & 1) == 0)) ||
               ((bpp == 3u || bpp == 4u) && (((uint32_t)surf->p & 3) == 0)));
    // check for correct alignment of columns
    GIL_ASSERT(((bpp == 2u) && (((uint32_t)surf->alignment & 1) == 0)) ||
               ((bpp == 3u) && (((uint32_t)surf->alignment & 1) == 0 || ((uint32_t)surf->alignment & 1) == 1 || ((uint32_t)surf->alignment & 3) == 0)) ||
               ((bpp == 4u) && (((uint32_t)surf->alignment & 3) == 0)));
}

// To do: Which functions are independent / and wich are dependent?

// --> We have a SW bitblitting library (platform independent)
// --> We have functions to get memory pointers - render targets (platform dependent)

// --> A Glue piece which combines windowing system with renderer
// --> Windows implementation which get Windows and memory pointer in RAM

uint8_t gil_helper_getbpp(GILFormat format)
{
    // check that format is supported
    uint8_t bpp = 0u;
    if (format < GIL_FORMAT_1_BPP)
        bpp = 1u;
    else if (format < GIL_FORMAT_2_BPP)
        bpp = 2u;
    else if (format < GIL_FORMAT_3_BPP)
        bpp = 3u;
    else if (format < GIL_FORMAT_4_BPP)
        bpp = 4u;

    return bpp;
}

void gil_helper_set_8_color(GIL_SW_Pointer dest, const GIL_SW_Color* color, GILFormat format)
{
    switch (format)
    {
    case GIL_FORMAT_A_8:
    {
        *dest = color->a;
        break;
    }
    // TODO: add implementation
    case GIL_FORMAT_P_8_ARGB_8888:
    case GIL_FORMAT_P_8_BGRA_8888:
    case GIL_FORMAT_P_8_RGBA_8888:
    case GIL_FORMAT_P_8_RGB_888:
    case GIL_FORMAT_P_8_RGB_565:
    default:
    {}
    }
}

void gil_helper_set_16_color(GIL_SW_Pointer dest, const GIL_SW_Color* color, GILFormat format)
{
    switch (format)
    {
    // TODO: Add implementation
    case GIL_FORMAT_RGB_565:
    case GIL_FORMAT_BGR_565:
    default:
    {}
    }
}

void gil_helper_set_24_color(GIL_SW_Pointer dest, const GIL_SW_Color* color, GILFormat format)
{
    switch (format)
    {
    case GIL_FORMAT_RGB_888:
    {
        *dest = color->r;
        *(dest+1) = color->g;
        *(dest+2) = color->b;
        break;
    }
    case GIL_FORMAT_BGR_888:
    {
        *dest = color->b;
        *(dest+1) = color->g;
        *(dest+2) = color->r;
        break;
    }
    default:
    {}
    }
}

void gil_helper_set_32_color(GIL_SW_Pointer dest, const GIL_SW_Color* color, GILFormat format)
{
    switch (format)
    {
    case GIL_FORMAT_ARGB_8888:
    {
        *dest = color->a;
        *(dest+1) = color->r;
        *(dest+2) = color->g;
        *(dest+3) = color->b;
        break;
    }
    case GIL_FORMAT_BGRA_8888:
    {
        *dest = color->b;
        *(dest+1) = color->g;
        *(dest+2) = color->r;
        *(dest+3) = color->a;
        break;
    }
    case GIL_FORMAT_RGBA_8888:
    {
        *dest = color->r;
        *(dest+1) = color->g;
        *(dest+2) = color->b;
        *(dest+3) = color->a;
        break;
    }
    default:
    {}
    }
}


// What interface needs the SW renderer
// SW renderer needs dest - memory pointer, x & y and alignment / offset to come to next column
// SW renderer needs src - memory pointer , x & y, width & height and alignment
// SW renderer can handle all format by simply copying content (no alpha blending in one render target support / bitblit over bitblit)

void gil_sw_bitblit_copy(GIL_SW_Surface *dest, GIL_SW_Surface *source)
{
    // check that format is supported
    const uint8_t bpp = gil_helper_getbpp(source->format);
    checkPreconditions(dest, bpp);
    checkPreconditions(source, bpp);

    const GIL_SW_Pointer psStart = source->p + source->y*source->alignment + source->x*bpp; // calculate start position of source memory pointer
    GIL_SW_Pointer ps = psStart;

    const GIL_SW_Pointer pdStart = dest->p + dest->y*dest->alignment + dest->x*bpp; // calculate start position of dest memory pointer
    GIL_SW_Pointer pd = pdStart;

    int32_t offsets = source->alignment - source->w*bpp; // calculate how many bytes need to be added to source when switching to the next column
    int32_t offsetd = dest->alignment - source->w*bpp; // attention: width of destination is only for informational purpose. No scaling applied. Currently ignored
    for (int32_t y = 0; y < source->h; ++y)
    {
        const GIL_SW_Pointer psBegin = psStart + y*source->alignment;
        const GIL_SW_Pointer psEnd = psBegin + source->bytes;
        const GIL_SW_Pointer pdBegin = pdStart + y*dest->alignment;
        const GIL_SW_Pointer pdEnd = pdBegin + dest->bytes + offsetd * dest->h;
        for (int32_t x = 0; x < source->w; ++x)
        {
            if ((ps >= psBegin) && (ps < psEnd) && (pd >= pdBegin) && (pd < pdEnd))
            {
                memcpy(pd, ps, bpp);
            }
            ps += bpp;
            pd += bpp;
        }
        ps += offsets;
        pd += offsetd;
    }
}

void gil_sw_set_color(GIL_SW_Surface* dest, const GIL_SW_Color* color)
{
    // check that format is supported
    const uint8_t bpp = gil_helper_getbpp(dest->format);
    checkPreconditions(dest, bpp);

    const GIL_SW_Pointer pdStart = dest->p + dest->y*dest->alignment + dest->x*bpp; // calculate start position of dest memory pointer
    GIL_SW_Pointer pd = pdStart;

    int32_t offsetd = dest->alignment - dest->w*bpp;

    for (int32_t y = 0; y < dest->h; ++y)
    {
        const GIL_SW_Pointer pdBegin = pdStart + y*dest->alignment;
        const GIL_SW_Pointer pdEnd = pdBegin + dest->bytes + offsetd * dest->h;
        for (int32_t x = 0; x < dest->w; ++x)
        {
            if ((pd >= pdBegin) && (pd < pdEnd))
            {
                switch (bpp)
                {
                case 1u:
                {
                    gil_helper_set_8_color(pd, color, dest->format);
                    break;
                }
                case 2u:
                {
                    gil_helper_set_16_color(pd, color, dest->format);
                    break;
                }
                case 3u:
                {
                    gil_helper_set_24_color(pd, color, dest->format);
                    break;
                }
                case 4u:
                {
                    gil_helper_set_32_color(pd, color, dest->format);
                    break;
                }
                }
            }
            pd += bpp;
        }
        pd += offsetd;
    }
}

uint32_t gil_sw_compare(const GIL_SW_Surface *dest, const GIL_SW_Surface *source)
{
    // check that format is supported
    const uint8_t bpp =gil_helper_getbpp(dest->format);
    checkPreconditions(dest, bpp);
    checkPreconditions(source, bpp);

    const GIL_SW_Pointer psStart = source->p + source->y*source->alignment + source->x*bpp; // calculate start position of source memory pointer
    GIL_SW_Pointer ps = psStart;

    const GIL_SW_Pointer pdStart = dest->p + dest->y*dest->alignment + dest->x*bpp; // calculate start position of dest memory pointer
    GIL_SW_Pointer pd = pdStart;

    int32_t offsets = source->alignment - source->w*bpp; // calculate how many bytes need to be added to source when switching to the next column
    int32_t offsetd = dest->alignment - source->w*bpp; // attention: width of destination is only for informational purpose. No scaling applied. Currently ignored
    uint32_t failures = 0;
    for (int32_t y = 0; y < source->h; ++y)
    {
        const GIL_SW_Pointer psBegin = psStart + y*source->alignment;
        const GIL_SW_Pointer psEnd = psBegin + source->bytes;
        const GIL_SW_Pointer pdBegin = pdStart + y*dest->alignment;
        const GIL_SW_Pointer pdEnd = pdBegin + dest->bytes + offsetd * dest->h;
        for (int32_t x = 0; x < source->w; ++x)
        {
            if ((ps >= psBegin) && (ps < psEnd) && (pd >= pdBegin) && (pd < pdEnd))
            {
                switch (bpp)
                {
                    case 1u:
                        if (*pd != *ps)
                        {
                            ++failures;
                        }
                        break;
                    case 2u:
                        if (*((uint16_t*)(pd)) != *((uint16_t*)(ps)))
                        {
                            ++failures;
                        }
                        break;
                    case 3u:
                        if ((*pd != *ps) || (*(pd+1) != *(ps+1)) || (*(pd+2) != *(ps+2)))
                        {
                            ++failures;
                        }
                        break;
                    case 4u:
                        if (*((uint32_t*)(pd)) != *((uint32_t*)(ps)))
                        {
                            ++failures;
                        }
                        break;
                }
            }
            else
            {
                ++failures;
            }
            ps += bpp;
            pd += bpp;
        }
        ps += offsets;
        pd += offsetd;
    }
    return failures;
}
