/******************************************************************************
**
**   File:        GilSampleTest.cpp
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

#include <gtest/gtest.h>

#ifdef WIN32
#include <Windows.h>
#endif

#include "gil.h"

const unsigned char image2x2[] =
{
//    R     G     B     A
    0x00, 0x00, 0xff, 0xff, //cyan
    0xff, 0x00, 0xff, 0xff, //magenta
    0xff, 0xff, 0x00, 0xff, //yellow
    0xff, 0x00, 0x00, 0xff  //red
};

static const unsigned char image_data_0026indicator_oil[] =
{
    0x00, 0xff, 0xff, 0xff, 0x10, 0xff, 0xaa, 0x00, 0x80, 0xff, 0xaa, 0x00, 0xd0, 0xff, 0xaa, 0x00, 0x90, 0xff, 0xaa, 0x00, 0x30, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xd0, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xf0, 0xff, 0xaa, 0x00, 0x20, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff, 0x60, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xd0, 0xff, 0xaa, 0x00, 0x70, 0xff, 0xaa, 0x00, 0x20, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x40, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0x80, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0x90, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0xc0, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xf0, 0xff, 0xaa, 0x00, 0x20, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x30, 0xff, 0xaa, 0x00, 0x70, 0xff, 0xaa, 0x00, 0xb0, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xf0, 0xff, 0xaa, 0x00, 0x30, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff,
    0x40, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xd0, 0xff, 0xaa, 0x00, 0x30, 0xff, 0xaa, 0x00, 0x90, 0xff, 0xaa, 0x00, 0xe0, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x20, 0xff, 0xaa, 0x00, 0x60, 0xff, 0xaa, 0x00, 0xa0, 0xff, 0xaa, 0x00, 0xf0, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xf0, 0xff, 0xaa, 0x00, 0x30, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff,
    0xa0, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xe0, 0xff, 0xaa, 0x00, 0x80, 0xff, 0xaa, 0x00, 0x30, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x40, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xd0, 0xff, 0xaa, 0x00, 0x10, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x10, 0xff, 0xaa, 0x00, 0x50, 0xff, 0xaa, 0x00, 0x90, 0xff, 0xaa, 0x00, 0xd0, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xf0, 0xff, 0xaa, 0x00, 0xc0, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xf0, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff,
    0x90, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xe0, 0xff, 0xaa, 0x00, 0x90, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xd0, 0xff, 0xaa, 0x00, 0x80, 0xff, 0xaa, 0x00, 0xc0, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xd0, 0xff, 0xaa, 0x00, 0x30, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x60, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xe0, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff, 0x50, 0xff, 0xaa, 0x00, 0xa0, 0xff, 0xaa, 0x00, 0xf0, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xa0, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xe0, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xd0, 0xff, 0xaa, 0x00, 0x10, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x20, 0xff, 0xaa, 0x00, 0x20, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x30, 0xff, 0xaa, 0x00, 0x90, 0xff, 0xaa, 0x00, 0xe0, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xa0, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xf0, 0xff, 0xaa, 0x00, 0xb0, 0xff, 0xaa, 0x00, 0x60, 0xff, 0xaa, 0x00, 0x20, 0xff, 0xaa, 0x00, 0x70, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xf0, 0xff, 0xaa, 0x00, 0x10, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xa0, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x30, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x40, 0xff, 0xaa, 0x00, 0x30, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x60, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xf0, 0xff, 0xaa, 0x00, 0x30, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xaa, 0x00, 0x30, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x30, 0xff, 0xaa, 0x00, 0xf0, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0x40, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x70, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xa0, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x20, 0xff, 0xaa, 0x00, 0xf0, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0x60, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x10, 0xff, 0xaa, 0x00, 0xf0, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0x30, 0xff, 0xaa, 0x00,
    0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x10, 0xff, 0xaa, 0x00, 0xd0, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0x80, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x80, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xb0, 0xff, 0xaa, 0x00,
    0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xa0, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xc0, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xc0, 0xff, 0xaa, 0x00,
    0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xc0, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x90, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xff, 0xff, 0xaa, 0x00, 0xb0, 0xff, 0xaa, 0x00,
    0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x30, 0xff, 0xaa, 0x00, 0x40, 0xff, 0xaa, 0x00, 0x40, 0xff, 0xaa, 0x00, 0x40, 0xff, 0xaa, 0x00, 0x40, 0xff, 0xaa, 0x00, 0x40, 0xff, 0xaa, 0x00, 0x40, 0xff, 0xaa, 0x00, 0x40, 0xff, 0xaa, 0x00, 0x40, 0xff, 0xaa, 0x00, 0x40, 0xff, 0xaa, 0x00, 0x40, 0xff, 0xaa, 0x00, 0x40, 0xff, 0xaa, 0x00, 0x40, 0xff, 0xaa, 0x00, 0x40, 0xff, 0xaa, 0x00, 0x40, 0xff, 0xaa, 0x00, 0x40, 0xff, 0xaa, 0x00, 0x20, 0xff, 0xaa, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x10, 0xff, 0xaa, 0x00, 0xb0, 0xff, 0xaa, 0x00, 0xf0, 0xff, 0xaa, 0x00, 0xb0, 0xff, 0xaa, 0x00, 0x10, 0xff, 0xaa, 0x00
};

TEST(gilSample, drawBitmap)
{
    gilInit(NULL);
    GILSurface window = gilCreateWindow(0, 0, 0, 800, 480);
    EXPECT_TRUE(window != NULL);
    GILContext context = gilCreateContext();
    EXPECT_TRUE(context != NULL);
    EXPECT_EQ(GIL_TRUE, gilSetSurface(context, window));
    EXPECT_EQ(GIL_TRUE, gilSetColor(context, 0xff, 0x00, 0xff, 0xff));
    gilDrawArea(context, 10<<4, 20<<4, 400<<4, 240<<4);
    GILTexture texture = gilCreateTexture(context);
    gilBindTexture(context, texture);
    EXPECT_EQ(GIL_TRUE, gilLoadTexture(texture, 41, 15, GIL_FORMAT_BGRA_8888, GIL_FALSE, image_data_0026indicator_oil));
    int x1 = 20;
    int x2 = x1 + 41 -1;
    int y1 = 30;
    int y2 = y1 + 15 - 1;
    gilDrawQuad(context, x1<<4, y1<<4, 0, 0, x2 << 4, y2 << 4, 40 << 4, 14 << 4);
    gilDrawQuad(context, (x1*3) << 4, (y1*3) << 4, 0, 0, x2 << 4, y2 << 4, 40 << 4, 14 << 4);
    EXPECT_EQ(GIL_TRUE, gilSwapBuffers(window));
    EXPECT_EQ(GIL_NO_ERROR, gilGetError(context));
    gilHandleWindowEvents(context);
    // verify previous render commands
    EXPECT_EQ(GIL_TRUE, gilVerify(context, x1 << 4, y1 << 4, 0, 0, x2 << 4, y2 << 4, 40 << 4, 14 << 4));
    EXPECT_EQ(GIL_TRUE, gilVerify(context, (x1*3) << 4, (y1*3) << 4, 0, 0, x2 << 4, y2 << 4, 40 << 4, 14 << 4));
    // verify at wrong position
    EXPECT_EQ(GIL_FALSE, gilVerify(context, 0, 0, 0, 0, 40 << 4, 14 << 4, 40 << 4, 14 << 4));
    // TODO: image verification
}