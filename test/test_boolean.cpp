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
* This file implements tests of the \ref Model::Boolean helper functions.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QObject>
#include <QtTest/QtTest>

#include <model_intrinsic_types.h>
#include <model_complex.h>
#include <m_variant.h>
#include <m_set.h>

#include "test_boolean.h"


TestBoolean::TestBoolean() {}


TestBoolean::~TestBoolean() {}


void TestBoolean::TestBooleanHelpers() {
    Model::Boolean b0(false);
    Model::Boolean b1(true);

    Model::Integer i0(0);
    Model::Integer i1(1);

    Model::Real    r0(0);
    Model::Real    r1(1);

    Model::Complex c0(0, 0);
    Model::Complex c1(1, 0);

    M::Set         s0;
    M::Set         s1;

    s1.insert(Model::Integer(1));

    QCOMPARE(b0 && b0, false);
    QCOMPARE(b0 && b1, false);
    QCOMPARE(b1 && b0, false);
    QCOMPARE(b1 && b1, true);

    QCOMPARE(b0 && i0, false);
    QCOMPARE(b0 && i1, false);
    QCOMPARE(b1 && i0, false);
    QCOMPARE(b1 && i1, true);

    QCOMPARE(b0 && r0, false);
    QCOMPARE(b0 && r1, false);
    QCOMPARE(b1 && r0, false);
    QCOMPARE(b1 && r1, true);

    QCOMPARE(b0 && c0, false);
    QCOMPARE(b0 && c1, false);
    QCOMPARE(b1 && c0, false);
    QCOMPARE(b1 && c1, true);

    QCOMPARE(b0 && s0, false);
    QCOMPARE(b0 && s1, false);
    QCOMPARE(b1 && s0, false);
    QCOMPARE(b1 && s1, true);

    QCOMPARE(b0 || b0, false);
    QCOMPARE(b0 || b1, true);
    QCOMPARE(b1 || b0, true);
    QCOMPARE(b1 || b1, true);

    QCOMPARE(b0 || i0, false);
    QCOMPARE(b0 || i1, true);
    QCOMPARE(b1 || i0, true);
    QCOMPARE(b1 || i1, true);

    QCOMPARE(b0 || r0, false);
    QCOMPARE(b0 || r1, true);
    QCOMPARE(b1 || r0, true);
    QCOMPARE(b1 || r1, true);

    QCOMPARE(b0 || c0, false);
    QCOMPARE(b0 || c1, true);
    QCOMPARE(b1 || c0, true);
    QCOMPARE(b1 || c1, true);

    QCOMPARE(b0 || s0, false);
    QCOMPARE(b0 || s1, true);
    QCOMPARE(b1 || s0, true);
    QCOMPARE(b1 || s1, true);
}
