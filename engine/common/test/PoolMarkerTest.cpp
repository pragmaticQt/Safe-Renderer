/******************************************************************************
**
**   File:        PoolMarkerTest.cpp
**   Description:
**
**   Copyright (C) 2017 Luxoft GmbH
**
**   This file is part of Luxoft Safe Renderer.
**
**   This Source Code Form is subject to the terms of the Mozilla Public
**   License, v. 2.0. If a copy of the MPL was not distributed with this
**   file, You can obtain one at https://mozilla.org/MPL/2.0/.
**
**   SPDX-License-Identifier: MPL-2.0
**
******************************************************************************/

#include <PoolMarker.h>

#include <gtest/gtest.h>
#include <cstddef>

using namespace lsr;

template <std::size_t N>
class TypeValue
{
public:
    enum
    {
        value = N
    };
};

/**
 * This test fixture is needed only for TYPED_TEST.
 */
template <class T>
class PoolMarkerTest : public testing::Test
{
public:
    PoolMarkerTest()
    {
    }

    virtual ~PoolMarkerTest()
    {}
};

typedef ::testing::Types <TypeValue<1>,
                          TypeValue<2>,
                          TypeValue<3>,
                          TypeValue<4> > TestCases;

TYPED_TEST_CASE(PoolMarkerTest, TestCases);

TYPED_TEST(PoolMarkerTest, Constructor)
{
    const U8 dataChar = 0xAA;
    U8 expectedData[TypeParam::value];
    memset(expectedData, dataChar, sizeof(expectedData));
    const impl::PoolMarker<TypeParam::value> marker(dataChar);

    EXPECT_TRUE(memcmp(expectedData, &marker, TypeParam::value) == 0);
}

TYPED_TEST(PoolMarkerTest, CopyConstructor)
{
    impl::PoolMarker<TypeParam::value> marker1(0xAA);
    const impl::PoolMarker<TypeParam::value> marker2(marker1);

    EXPECT_TRUE(memcmp(&marker1, &marker2, TypeParam::value) == 0);

    marker1 = marker1;
    EXPECT_TRUE(memcmp(&marker1, &marker2, TypeParam::value) == 0);
}

TYPED_TEST(PoolMarkerTest, Assignment)
{
    const impl::PoolMarker<TypeParam::value> marker1(0xAA);
    impl::PoolMarker<TypeParam::value> marker2(0xBB);
    marker2 = marker1;

    EXPECT_TRUE(memcmp(&marker1, &marker2, TypeParam::value) == 0);
}

TYPED_TEST(PoolMarkerTest, OperatorEqualReturnTrue)
{
    const impl::PoolMarker<TypeParam::value> marker1(0xAA);
    const impl::PoolMarker<TypeParam::value> marker2(0xAA);

    EXPECT_TRUE(marker1 == marker2);
}

TYPED_TEST(PoolMarkerTest, OperatorEqualReturnFalse)
{
    const impl::PoolMarker<TypeParam::value> marker1(0xAA);
    const impl::PoolMarker<TypeParam::value> marker2(0xBB);

    EXPECT_FALSE(marker1 == marker2);

    const impl::PoolMarker<TypeParam::value> marker3(0xAA);
    const impl::PoolMarker<TypeParam::value> marker4(0xAB);

    EXPECT_FALSE(marker3 == marker4);
}

TYPED_TEST(PoolMarkerTest, IsEqualToPatternReturnTrue)
{
    const U8 pattern = 0xAA;
    const impl::PoolMarker<TypeParam::value> marker(pattern);

    EXPECT_TRUE(impl::PoolMarker<TypeParam::value>::isEqualToPattern(marker, pattern));
}

TYPED_TEST(PoolMarkerTest, IsEqualToPatternReturnFalse)
{
    const U8 pattern = 0xAA;
    const impl::PoolMarker<TypeParam::value> marker(0xBB);

    EXPECT_FALSE(impl::PoolMarker<TypeParam::value>::isEqualToPattern(marker, pattern));
}

