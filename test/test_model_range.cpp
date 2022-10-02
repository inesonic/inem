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
* This file implements tests of the \ref Model::Range class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QObject>
#include <QList>
#include <QSet>
#include <QMap>
#include <QtTest/QtTest>

#include <QDebug>

#include <model_intrinsic_types.h>
#include <model_range.h>
#include <model_variant.h>

#include "test_model_range.h"

TestModelRange::TestModelRange() {}

TestModelRange::~TestModelRange() {}

void TestModelRange::testConstructorsAndDestructors() {
    Model::Range r1;

    QCOMPARE(r1.isEmpty(), true);

    Model::Range r2(Model::Integer(1), Model::Integer(5));

    QCOMPARE(r2.isEmpty(), false);
    QCOMPARE(r2.first(),  Model::Variant(Model::Integer(1)));
    QCOMPARE(r2.second(), Model::Variant(Model::Integer(2)));
    QCOMPARE(r2.last(),   Model::Variant(Model::Integer(5)));

    Model::Range r3(Model::Integer(1), Model::Integer(3), Model::Integer(7));

    QCOMPARE(r3.isEmpty(), false);
    QCOMPARE(r3.first(),  Model::Variant(Model::Integer(1)));
    QCOMPARE(r3.second(), Model::Variant(Model::Integer(3)));
    QCOMPARE(r3.last(),   Model::Variant(Model::Integer(7)));

    Model::Range r4 = r3;

    QCOMPARE(r4.isEmpty(), false);
    QCOMPARE(r4.first(),  Model::Variant(Model::Integer(1)));
    QCOMPARE(r4.second(), Model::Variant(Model::Integer(3)));
    QCOMPARE(r4.last(),   Model::Variant(Model::Integer(7)));
}


void TestModelRange::testEmptySizeMethods() {
    Model::Range r1;

    QCOMPARE(r1.isEmpty(), true);

    Model::Range r2(Model::Integer(1), Model::Integer(5));

    QCOMPARE(r2.isEmpty(), false);
    QCOMPARE(r2.size(), 5U);

    Model::Range r3(Model::Real(1.0), Model::Integer(3), Model::Integer(7));

    QCOMPARE(r3.isEmpty(), false);
    QCOMPARE(r3.size(), 4U);
}


void TestModelRange::testContainsMethod() {
    Model::Range r(Model::Integer(1), Model::Integer(3), Model::Integer(7));

    QCOMPARE(r.contains(Model::Integer(0)), false);
    QCOMPARE(r.contains(Model::Integer(1)), true);
    QCOMPARE(r.contains(Model::Integer(2)), false);
    QCOMPARE(r.contains(Model::Integer(3)), true);
    QCOMPARE(r.contains(Model::Integer(4)), false);
    QCOMPARE(r.contains(Model::Integer(5)), true);
    QCOMPARE(r.contains(Model::Integer(6)), false);
    QCOMPARE(r.contains(Model::Integer(7)), true);
    QCOMPARE(r.contains(Model::Integer(8)), false);
}


void TestModelRange::testIterator() {
    Model::Range r(Model::Integer(1), Model::Integer(3), Model::Integer(7));

    Model::Range::Iterator it  = r.begin();
    Model::Range::Iterator end = r.end();

    QVERIFY(it.isValid());
    QCOMPARE(it.value(), Model::Variant(Model::Integer(1)));
    it.advance();

    QVERIFY(it.isValid());
    QVERIFY(it != end);
    QCOMPARE(*it.constPointer(), Model::Variant(Model::Integer(3)));
    ++it;

    QVERIFY(it.isValid());
    QVERIFY(it != end);
    QCOMPARE(*it++, Model::Variant(Model::Integer(5)));

    QVERIFY(it.isValid());
    QVERIFY(it != end);
    QCOMPARE(*it, Model::Variant(Model::Integer(7)));

    ++it;
    QVERIFY(it.isInvalid());
    QVERIFY(it == end);
}
