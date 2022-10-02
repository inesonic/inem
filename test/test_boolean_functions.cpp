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
* This file implements tests of the boolean functions.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QObject>
#include <QList>
#include <QtTest/QtTest>

#include <limits>

#include <m_intrinsic_types.h>
#include <m_set.h>
#include <m_tuple.h>
#include <m_variant.h>
#include <m_boolean_functions.h>

#include "test_boolean_functions.h"

TestBooleanFunctions::TestBooleanFunctions() {}


TestBooleanFunctions::~TestBooleanFunctions() {}


void TestBooleanFunctions::testLogicalExclusiveOr() {
    QCOMPARE(M::logicalExclusiveOr(false, false), false);
    QCOMPARE(M::logicalExclusiveOr(false, true),  true);
    QCOMPARE(M::logicalExclusiveOr(true, false),  true);
    QCOMPARE(M::logicalExclusiveOr(true, true),   false);

    QCOMPARE(M::logicalExclusiveOr(false, M::Integer(0)), false);
    QCOMPARE(M::logicalExclusiveOr(false, M::Integer(1)), true);
    QCOMPARE(M::logicalExclusiveOr(true, M::Integer(0)),  true);
    QCOMPARE(M::logicalExclusiveOr(true, M::Integer(1)),  false);

    QCOMPARE(M::logicalExclusiveOr(false, M::Real(0)), false);
    QCOMPARE(M::logicalExclusiveOr(false, M::Real(1)), true);
    QCOMPARE(M::logicalExclusiveOr(true, M::Real(0)),  true);
    QCOMPARE(M::logicalExclusiveOr(true, M::Real(1)),  false);

    QCOMPARE(M::logicalExclusiveOr(false, M::Complex(0)), false);
    QCOMPARE(M::logicalExclusiveOr(false, M::Complex(1)), true);
    QCOMPARE(M::logicalExclusiveOr(true, M::Complex(0)),  true);
    QCOMPARE(M::logicalExclusiveOr(true, M::Complex(1)),  false);

    QCOMPARE(M::logicalExclusiveOr(false, M::Variant(M::Complex(0))), false);
    QCOMPARE(M::logicalExclusiveOr(false, M::Variant(M::Complex(1))), true);
    QCOMPARE(M::logicalExclusiveOr(true, M::Variant(M::Complex(0))),  true);
    QCOMPARE(M::logicalExclusiveOr(true, M::Variant(M::Complex(1))),  false);


    QCOMPARE(M::logicalExclusiveOr(M::Integer(0), false), false);
    QCOMPARE(M::logicalExclusiveOr(M::Integer(0), true),  true);
    QCOMPARE(M::logicalExclusiveOr(M::Integer(1), false),  true);
    QCOMPARE(M::logicalExclusiveOr(M::Integer(1), true),   false);

    QCOMPARE(M::logicalExclusiveOr(M::Integer(0), M::Integer(0)), false);
    QCOMPARE(M::logicalExclusiveOr(M::Integer(0), M::Integer(1)), true);
    QCOMPARE(M::logicalExclusiveOr(M::Integer(1), M::Integer(0)),  true);
    QCOMPARE(M::logicalExclusiveOr(M::Integer(1), M::Integer(1)),  false);

    QCOMPARE(M::logicalExclusiveOr(M::Integer(0), M::Real(0)), false);
    QCOMPARE(M::logicalExclusiveOr(M::Integer(0), M::Real(1)), true);
    QCOMPARE(M::logicalExclusiveOr(M::Integer(1), M::Real(0)),  true);
    QCOMPARE(M::logicalExclusiveOr(M::Integer(1), M::Real(1)),  false);

    QCOMPARE(M::logicalExclusiveOr(M::Integer(0), M::Complex(0)), false);
    QCOMPARE(M::logicalExclusiveOr(M::Integer(0), M::Complex(1)), true);
    QCOMPARE(M::logicalExclusiveOr(M::Integer(1), M::Complex(0)),  true);
    QCOMPARE(M::logicalExclusiveOr(M::Integer(1), M::Complex(1)),  false);

    QCOMPARE(M::logicalExclusiveOr(M::Integer(0), M::Variant(M::Complex(0))), false);
    QCOMPARE(M::logicalExclusiveOr(M::Integer(0), M::Variant(M::Complex(1))), true);
    QCOMPARE(M::logicalExclusiveOr(M::Integer(1), M::Variant(M::Complex(0))),  true);
    QCOMPARE(M::logicalExclusiveOr(M::Integer(1), M::Variant(M::Complex(1))),  false);


    QCOMPARE(M::logicalExclusiveOr(M::Real(0), false), false);
    QCOMPARE(M::logicalExclusiveOr(M::Real(0), true),  true);
    QCOMPARE(M::logicalExclusiveOr(M::Real(1), false),  true);
    QCOMPARE(M::logicalExclusiveOr(M::Real(1), true),   false);

    QCOMPARE(M::logicalExclusiveOr(M::Real(0), M::Integer(0)), false);
    QCOMPARE(M::logicalExclusiveOr(M::Real(0), M::Integer(1)), true);
    QCOMPARE(M::logicalExclusiveOr(M::Real(1), M::Integer(0)),  true);
    QCOMPARE(M::logicalExclusiveOr(M::Real(1), M::Integer(1)),  false);

    QCOMPARE(M::logicalExclusiveOr(M::Real(0), M::Real(0)), false);
    QCOMPARE(M::logicalExclusiveOr(M::Real(0), M::Real(1)), true);
    QCOMPARE(M::logicalExclusiveOr(M::Real(1), M::Real(0)),  true);
    QCOMPARE(M::logicalExclusiveOr(M::Real(1), M::Real(1)),  false);

    QCOMPARE(M::logicalExclusiveOr(M::Real(0), M::Complex(0)), false);
    QCOMPARE(M::logicalExclusiveOr(M::Real(0), M::Complex(1)), true);
    QCOMPARE(M::logicalExclusiveOr(M::Real(1), M::Complex(0)),  true);
    QCOMPARE(M::logicalExclusiveOr(M::Real(1), M::Complex(1)),  false);

    QCOMPARE(M::logicalExclusiveOr(M::Real(0), M::Variant(M::Complex(0))), false);
    QCOMPARE(M::logicalExclusiveOr(M::Real(0), M::Variant(M::Complex(1))), true);
    QCOMPARE(M::logicalExclusiveOr(M::Real(1), M::Variant(M::Complex(0))),  true);
    QCOMPARE(M::logicalExclusiveOr(M::Real(1), M::Variant(M::Complex(1))),  false);


    QCOMPARE(M::logicalExclusiveOr(M::Complex(0), false), false);
    QCOMPARE(M::logicalExclusiveOr(M::Complex(0), true),  true);
    QCOMPARE(M::logicalExclusiveOr(M::Complex(1), false),  true);
    QCOMPARE(M::logicalExclusiveOr(M::Complex(1), true),   false);

    QCOMPARE(M::logicalExclusiveOr(M::Complex(0), M::Integer(0)), false);
    QCOMPARE(M::logicalExclusiveOr(M::Complex(0), M::Integer(1)), true);
    QCOMPARE(M::logicalExclusiveOr(M::Complex(1), M::Integer(0)),  true);
    QCOMPARE(M::logicalExclusiveOr(M::Complex(1), M::Integer(1)),  false);

    QCOMPARE(M::logicalExclusiveOr(M::Complex(0), M::Real(0)), false);
    QCOMPARE(M::logicalExclusiveOr(M::Complex(0), M::Real(1)), true);
    QCOMPARE(M::logicalExclusiveOr(M::Complex(1), M::Real(0)),  true);
    QCOMPARE(M::logicalExclusiveOr(M::Complex(1), M::Real(1)),  false);

    QCOMPARE(M::logicalExclusiveOr(M::Complex(0), M::Complex(0)), false);
    QCOMPARE(M::logicalExclusiveOr(M::Complex(0), M::Complex(1)), true);
    QCOMPARE(M::logicalExclusiveOr(M::Complex(1), M::Complex(0)),  true);
    QCOMPARE(M::logicalExclusiveOr(M::Complex(1), M::Complex(1)),  false);

    QCOMPARE(M::logicalExclusiveOr(M::Complex(0), M::Variant(M::Complex(0))), false);
    QCOMPARE(M::logicalExclusiveOr(M::Complex(0), M::Variant(M::Complex(1))), true);
    QCOMPARE(M::logicalExclusiveOr(M::Complex(1), M::Variant(M::Complex(0))),  true);
    QCOMPARE(M::logicalExclusiveOr(M::Complex(1), M::Variant(M::Complex(1))),  false);


    QCOMPARE(M::logicalExclusiveOr(M::Variant(M::Integer(0)), false), false);
    QCOMPARE(M::logicalExclusiveOr(M::Variant(M::Integer(0)), true),  true);
    QCOMPARE(M::logicalExclusiveOr(M::Variant(M::Integer(1)), false),  true);
    QCOMPARE(M::logicalExclusiveOr(M::Variant(M::Integer(1)), true),   false);

    QCOMPARE(M::logicalExclusiveOr(M::Variant(M::Integer(0)), M::Integer(0)), false);
    QCOMPARE(M::logicalExclusiveOr(M::Variant(M::Integer(0)), M::Integer(1)), true);
    QCOMPARE(M::logicalExclusiveOr(M::Variant(M::Integer(1)), M::Integer(0)),  true);
    QCOMPARE(M::logicalExclusiveOr(M::Variant(M::Integer(1)), M::Integer(1)),  false);

    QCOMPARE(M::logicalExclusiveOr(M::Variant(M::Integer(0)), M::Real(0)), false);
    QCOMPARE(M::logicalExclusiveOr(M::Variant(M::Integer(0)), M::Real(1)), true);
    QCOMPARE(M::logicalExclusiveOr(M::Variant(M::Integer(1)), M::Real(0)),  true);
    QCOMPARE(M::logicalExclusiveOr(M::Variant(M::Integer(1)), M::Real(1)),  false);

    QCOMPARE(M::logicalExclusiveOr(M::Variant(M::Integer(0)), M::Complex(0)), false);
    QCOMPARE(M::logicalExclusiveOr(M::Variant(M::Integer(0)), M::Complex(1)), true);
    QCOMPARE(M::logicalExclusiveOr(M::Variant(M::Integer(1)), M::Complex(0)),  true);
    QCOMPARE(M::logicalExclusiveOr(M::Variant(M::Integer(1)), M::Complex(1)),  false);

    QCOMPARE(M::logicalExclusiveOr(M::Variant(M::Integer(0)), M::Variant(M::Integer(0))), false);
    QCOMPARE(M::logicalExclusiveOr(M::Variant(M::Integer(0)), M::Variant(M::Complex(1))), true);
    QCOMPARE(M::logicalExclusiveOr(M::Variant(M::Integer(1)), M::Variant(M::Complex(0))),  true);
    QCOMPARE(M::logicalExclusiveOr(M::Variant(M::Integer(1)), M::Variant(M::Complex(1))),  false);
}


void TestBooleanFunctions::testBiconditional() {
    QCOMPARE(M::logicalBiconditional(false, false), true);
    QCOMPARE(M::logicalBiconditional(false, true),  false);
    QCOMPARE(M::logicalBiconditional(true, false),  false);
    QCOMPARE(M::logicalBiconditional(true, true),   true);

    QCOMPARE(M::logicalBiconditional(false, M::Integer(0)), true);
    QCOMPARE(M::logicalBiconditional(false, M::Integer(1)), false);
    QCOMPARE(M::logicalBiconditional(true, M::Integer(0)),  false);
    QCOMPARE(M::logicalBiconditional(true, M::Integer(1)),  true);

    QCOMPARE(M::logicalBiconditional(false, M::Real(0)), true);
    QCOMPARE(M::logicalBiconditional(false, M::Real(1)), false);
    QCOMPARE(M::logicalBiconditional(true, M::Real(0)),  false);
    QCOMPARE(M::logicalBiconditional(true, M::Real(1)),  true);

    QCOMPARE(M::logicalBiconditional(false, M::Complex(0)), true);
    QCOMPARE(M::logicalBiconditional(false, M::Complex(1)), false);
    QCOMPARE(M::logicalBiconditional(true, M::Complex(0)),  false);
    QCOMPARE(M::logicalBiconditional(true, M::Complex(1)),  true);

    QCOMPARE(M::logicalBiconditional(false, M::Variant(M::Complex(0))), true);
    QCOMPARE(M::logicalBiconditional(false, M::Variant(M::Complex(1))), false);
    QCOMPARE(M::logicalBiconditional(true, M::Variant(M::Complex(0))),  false);
    QCOMPARE(M::logicalBiconditional(true, M::Variant(M::Complex(1))),  true);


    QCOMPARE(M::logicalBiconditional(M::Integer(0), false), true);
    QCOMPARE(M::logicalBiconditional(M::Integer(0), true),  false);
    QCOMPARE(M::logicalBiconditional(M::Integer(1), false),  false);
    QCOMPARE(M::logicalBiconditional(M::Integer(1), true),   true);

    QCOMPARE(M::logicalBiconditional(M::Integer(0), M::Integer(0)), true);
    QCOMPARE(M::logicalBiconditional(M::Integer(0), M::Integer(1)), false);
    QCOMPARE(M::logicalBiconditional(M::Integer(1), M::Integer(0)),  false);
    QCOMPARE(M::logicalBiconditional(M::Integer(1), M::Integer(1)),  true);

    QCOMPARE(M::logicalBiconditional(M::Integer(0), M::Real(0)), true);
    QCOMPARE(M::logicalBiconditional(M::Integer(0), M::Real(1)), false);
    QCOMPARE(M::logicalBiconditional(M::Integer(1), M::Real(0)),  false);
    QCOMPARE(M::logicalBiconditional(M::Integer(1), M::Real(1)),  true);

    QCOMPARE(M::logicalBiconditional(M::Integer(0), M::Complex(0)), true);
    QCOMPARE(M::logicalBiconditional(M::Integer(0), M::Complex(1)), false);
    QCOMPARE(M::logicalBiconditional(M::Integer(1), M::Complex(0)),  false);
    QCOMPARE(M::logicalBiconditional(M::Integer(1), M::Complex(1)),  true);

    QCOMPARE(M::logicalBiconditional(M::Integer(0), M::Variant(M::Complex(0))), true);
    QCOMPARE(M::logicalBiconditional(M::Integer(0), M::Variant(M::Complex(1))), false);
    QCOMPARE(M::logicalBiconditional(M::Integer(1), M::Variant(M::Complex(0))),  false);
    QCOMPARE(M::logicalBiconditional(M::Integer(1), M::Variant(M::Complex(1))),  true);


    QCOMPARE(M::logicalBiconditional(M::Real(0), false), true);
    QCOMPARE(M::logicalBiconditional(M::Real(0), true),  false);
    QCOMPARE(M::logicalBiconditional(M::Real(1), false),  false);
    QCOMPARE(M::logicalBiconditional(M::Real(1), true),   true);

    QCOMPARE(M::logicalBiconditional(M::Real(0), M::Integer(0)), true);
    QCOMPARE(M::logicalBiconditional(M::Real(0), M::Integer(1)), false);
    QCOMPARE(M::logicalBiconditional(M::Real(1), M::Integer(0)),  false);
    QCOMPARE(M::logicalBiconditional(M::Real(1), M::Integer(1)),  true);

    QCOMPARE(M::logicalBiconditional(M::Real(0), M::Real(0)), true);
    QCOMPARE(M::logicalBiconditional(M::Real(0), M::Real(1)), false);
    QCOMPARE(M::logicalBiconditional(M::Real(1), M::Real(0)),  false);
    QCOMPARE(M::logicalBiconditional(M::Real(1), M::Real(1)),  true);

    QCOMPARE(M::logicalBiconditional(M::Real(0), M::Complex(0)), true);
    QCOMPARE(M::logicalBiconditional(M::Real(0), M::Complex(1)), false);
    QCOMPARE(M::logicalBiconditional(M::Real(1), M::Complex(0)),  false);
    QCOMPARE(M::logicalBiconditional(M::Real(1), M::Complex(1)),  true);

    QCOMPARE(M::logicalBiconditional(M::Real(0), M::Variant(M::Complex(0))), true);
    QCOMPARE(M::logicalBiconditional(M::Real(0), M::Variant(M::Complex(1))), false);
    QCOMPARE(M::logicalBiconditional(M::Real(1), M::Variant(M::Complex(0))),  false);
    QCOMPARE(M::logicalBiconditional(M::Real(1), M::Variant(M::Complex(1))),  true);


    QCOMPARE(M::logicalBiconditional(M::Complex(0), false), true);
    QCOMPARE(M::logicalBiconditional(M::Complex(0), true),  false);
    QCOMPARE(M::logicalBiconditional(M::Complex(1), false),  false);
    QCOMPARE(M::logicalBiconditional(M::Complex(1), true),   true);

    QCOMPARE(M::logicalBiconditional(M::Complex(0), M::Integer(0)), true);
    QCOMPARE(M::logicalBiconditional(M::Complex(0), M::Integer(1)), false);
    QCOMPARE(M::logicalBiconditional(M::Complex(1), M::Integer(0)),  false);
    QCOMPARE(M::logicalBiconditional(M::Complex(1), M::Integer(1)),  true);

    QCOMPARE(M::logicalBiconditional(M::Complex(0), M::Real(0)), true);
    QCOMPARE(M::logicalBiconditional(M::Complex(0), M::Real(1)), false);
    QCOMPARE(M::logicalBiconditional(M::Complex(1), M::Real(0)),  false);
    QCOMPARE(M::logicalBiconditional(M::Complex(1), M::Real(1)),  true);

    QCOMPARE(M::logicalBiconditional(M::Complex(0), M::Complex(0)), true);
    QCOMPARE(M::logicalBiconditional(M::Complex(0), M::Complex(1)), false);
    QCOMPARE(M::logicalBiconditional(M::Complex(1), M::Complex(0)),  false);
    QCOMPARE(M::logicalBiconditional(M::Complex(1), M::Complex(1)),  true);

    QCOMPARE(M::logicalBiconditional(M::Complex(0), M::Variant(M::Complex(0))), true);
    QCOMPARE(M::logicalBiconditional(M::Complex(0), M::Variant(M::Complex(1))), false);
    QCOMPARE(M::logicalBiconditional(M::Complex(1), M::Variant(M::Complex(0))),  false);
    QCOMPARE(M::logicalBiconditional(M::Complex(1), M::Variant(M::Complex(1))),  true);


    QCOMPARE(M::logicalBiconditional(M::Variant(M::Integer(0)), false), true);
    QCOMPARE(M::logicalBiconditional(M::Variant(M::Integer(0)), true),  false);
    QCOMPARE(M::logicalBiconditional(M::Variant(M::Integer(1)), false),  false);
    QCOMPARE(M::logicalBiconditional(M::Variant(M::Integer(1)), true),   true);

    QCOMPARE(M::logicalBiconditional(M::Variant(M::Integer(0)), M::Integer(0)), true);
    QCOMPARE(M::logicalBiconditional(M::Variant(M::Integer(0)), M::Integer(1)), false);
    QCOMPARE(M::logicalBiconditional(M::Variant(M::Integer(1)), M::Integer(0)),  false);
    QCOMPARE(M::logicalBiconditional(M::Variant(M::Integer(1)), M::Integer(1)),  true);

    QCOMPARE(M::logicalBiconditional(M::Variant(M::Integer(0)), M::Real(0)), true);
    QCOMPARE(M::logicalBiconditional(M::Variant(M::Integer(0)), M::Real(1)), false);
    QCOMPARE(M::logicalBiconditional(M::Variant(M::Integer(1)), M::Real(0)),  false);
    QCOMPARE(M::logicalBiconditional(M::Variant(M::Integer(1)), M::Real(1)),  true);

    QCOMPARE(M::logicalBiconditional(M::Variant(M::Integer(0)), M::Complex(0)), true);
    QCOMPARE(M::logicalBiconditional(M::Variant(M::Integer(0)), M::Complex(1)), false);
    QCOMPARE(M::logicalBiconditional(M::Variant(M::Integer(1)), M::Complex(0)),  false);
    QCOMPARE(M::logicalBiconditional(M::Variant(M::Integer(1)), M::Complex(1)),  true);

    QCOMPARE(M::logicalBiconditional(M::Variant(M::Integer(0)), M::Variant(M::Integer(0))), true);
    QCOMPARE(M::logicalBiconditional(M::Variant(M::Integer(0)), M::Variant(M::Complex(1))), false);
    QCOMPARE(M::logicalBiconditional(M::Variant(M::Integer(1)), M::Variant(M::Complex(0))),  false);
    QCOMPARE(M::logicalBiconditional(M::Variant(M::Integer(1)), M::Variant(M::Complex(1))),  true);
}
