/*-*-c++-*-*************************************************************************************************************
* Copyright 2016 - 2022 Inesonic, LLC.
* 
* This file is licensed under two licenses.
*
* Inesonic Commercial License, Version 1:
*   All rights reserved.  Inesonic, LLC retains all rights to this software, including the right to relicense the
*   software in source or binary formats under different terms.  Unauthorized use under the terms of this license is
*   strictly prohibited.
*
* GNU Public License, Version 2:
*   This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
*   License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later
*   version.
*   
*   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
*   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
*   details.
*   
*   You should have received a copy of the GNU General Public License along with this program; if not, write to the Free
*   Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
********************************************************************************************************************//**
* \file
*
* This file implements tests of the \ref M::Complex operators.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QObject>
#include <QList>
#include <QtTest/QtTest>

#include <limits>
#include <m_intrinsic_types.h>
#include <m_complex_operators.h>

#include "test_complex_operators.h"

TestComplexOperators::TestComplexOperators() {}


TestComplexOperators::~TestComplexOperators() {}


void TestComplexOperators::testComparisonOperators() {
    M::Complex a = 5;
    M::Complex b = 6;

    QCOMPARE((a < b), true);
    QCOMPARE((b < a), false);
    QCOMPARE((a <= b), true);
    QCOMPARE((b <= a), false);
    QCOMPARE((a < a), false);
    QCOMPARE((a <= a), true);

    QCOMPARE((a < static_cast<M::Integer>(6)), true);
    QCOMPARE((static_cast<M::Integer>(6) < a), false);
    QCOMPARE((a <= static_cast<M::Integer>(6)), true);
    QCOMPARE((static_cast<M::Integer>(6) <= a), false);

    QCOMPARE((a < static_cast<M::Real>(6)), true);
    QCOMPARE((static_cast<M::Real>(6) < a), false);
    QCOMPARE((a <= static_cast<M::Real>(6)), true);
    QCOMPARE((static_cast<M::Real>(6) <= a), false);

    QCOMPARE((a > b), false);
    QCOMPARE((b > a), true);
    QCOMPARE((a >= b), false);
    QCOMPARE((b >= a), true);
    QCOMPARE((a > a), false);
    QCOMPARE((a >= a), true);

    QCOMPARE((a > static_cast<M::Integer>(6)), false);
    QCOMPARE((static_cast<M::Integer>(6) > a), true);
    QCOMPARE((a >= static_cast<M::Integer>(6)), false);
    QCOMPARE((static_cast<M::Integer>(6) >= a), true);

    QCOMPARE((a > static_cast<M::Real>(6)), false);
    QCOMPARE((static_cast<M::Real>(6) > a), true);
    QCOMPARE((a >= static_cast<M::Real>(6)), false);
    QCOMPARE((static_cast<M::Real>(6) >= a), true);
}
