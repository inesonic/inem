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
* This file implements tests of the \ref Model::Tuple class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QObject>
#include <QList>
#include <QSet>
#include <QMap>
#include <QtTest/QtTest>

#include <QDebug>

#include <limits>
#include <random>

#include <model_intrinsic_types.h>
#include <m_variant.h>
#include <model_exceptions.h>
#include <model_variant.h>
#include <model_range.h>
#include <model_tuple.h>

#include "test_model_tuple.h"

TestModelTuple::TestModelTuple() {}


TestModelTuple::~TestModelTuple() {}


void TestModelTuple::testConstructorsAndDestructors() {
    Model::Tuple t1;
    t1.append(Model::Integer(1));

    QCOMPARE(t1.size(), 1U);

    Model::Tuple t2(t1);
    QCOMPARE(t2.size(), 1U);
    QCOMPARE(t2.at(1), Model::Variant(Model::Integer(1)));

    Model::Tuple t3 = createDefaultTuple(); // Note: Should invoke move constructor (but GCC optimizes it out).
    QCOMPARE(t3.size(), 2U);
    QCOMPARE(t3.at(1), Model::Variant(Model::Integer(1)));
    QCOMPARE(t3.at(2), Model::Variant(Model::Integer(2)));
}


void TestModelTuple::testAppendPrependMethods() {
    Model::Tuple t1;

    t1.append(Model::Integer(1));
    t1.append(Model::Integer(2));
    t1.append(Model::Integer(3));
    t1.prepend(Model::Integer(4));
    t1.prepend(Model::Integer(5));
    t1.prepend(Model::Integer(6));

    QCOMPARE(t1.size(), 6U);
    QCOMPARE(t1(1), Model::Variant(Model::Integer(6)));
    QCOMPARE(t1(2), Model::Variant(Model::Integer(5)));
    QCOMPARE(t1(3), Model::Variant(Model::Integer(4)));
    QCOMPARE(t1(4), Model::Variant(Model::Integer(1)));
    QCOMPARE(t1(5), Model::Variant(Model::Integer(2)));
    QCOMPARE(t1(6), Model::Variant(Model::Integer(3)));

    // Check copy on write.
    Model::Tuple t2 = t1;
    t2.append(Model::Integer(7));

    QCOMPARE(t1.size(), 6U);
    QCOMPARE(t2.size(), 7U);

    QCOMPARE(t1(1), Model::Variant(Model::Integer(6)));
    QCOMPARE(t1(2), Model::Variant(Model::Integer(5)));
    QCOMPARE(t1(3), Model::Variant(Model::Integer(4)));
    QCOMPARE(t1(4), Model::Variant(Model::Integer(1)));
    QCOMPARE(t1(5), Model::Variant(Model::Integer(2)));
    QCOMPARE(t1(6), Model::Variant(Model::Integer(3)));

    QCOMPARE(t2(1), Model::Variant(Model::Integer(6)));
    QCOMPARE(t2(2), Model::Variant(Model::Integer(5)));
    QCOMPARE(t2(3), Model::Variant(Model::Integer(4)));
    QCOMPARE(t2(4), Model::Variant(Model::Integer(1)));
    QCOMPARE(t2(5), Model::Variant(Model::Integer(2)));
    QCOMPARE(t2(6), Model::Variant(Model::Integer(3)));
    QCOMPARE(t2(7), Model::Variant(Model::Integer(7)));
}


void TestModelTuple::testRangeAppendPrependMethods() {
    Model::Tuple t1;

    t1.append(Model::Range(Model::Integer(1), Model::Integer(3), Model::Integer(11)));

    QCOMPARE(t1.size(), 6U);
    QCOMPARE(t1.at(1), Model::Variant(Model::Integer(1)));
    QCOMPARE(t1.at(2), Model::Variant(Model::Integer(3)));
    QCOMPARE(t1.at(3), Model::Variant(Model::Integer(5)));
    QCOMPARE(t1.at(4), Model::Variant(Model::Integer(7)));
    QCOMPARE(t1.at(5), Model::Variant(Model::Integer(9)));
    QCOMPARE(t1.at(6), Model::Variant(Model::Integer(11)));

    t1.prepend(Model::Range(Model::Integer(2), Model::Integer(4), Model::Integer(8)));

    QCOMPARE(t1.size(), 10U);
    QCOMPARE(t1.at(1),  Model::Variant(Model::Integer(8)));
    QCOMPARE(t1.at(2),  Model::Variant(Model::Integer(6)));
    QCOMPARE(t1.at(3),  Model::Variant(Model::Integer(4)));
    QCOMPARE(t1.at(4),  Model::Variant(Model::Integer(2)));
    QCOMPARE(t1.at(5),  Model::Variant(Model::Integer(1)));
    QCOMPARE(t1.at(6),  Model::Variant(Model::Integer(3)));
    QCOMPARE(t1.at(7),  Model::Variant(Model::Integer(5)));
    QCOMPARE(t1.at(8),  Model::Variant(Model::Integer(7)));
    QCOMPARE(t1.at(9),  Model::Variant(Model::Integer(9)));
    QCOMPARE(t1.at(10), Model::Variant(Model::Integer(11)));
}


void TestModelTuple::testTakeFirstLastMethods() {
    Model::Tuple t1;

    t1.append(Model::Integer(1));
    t1.append(Model::Integer(2));
    t1.append(Model::Integer(3));
    t1.append(Model::Integer(4));
    t1.append(Model::Integer(5));
    t1.append(Model::Integer(6));

    QCOMPARE(t1.takeLast(), Model::Variant(Model::Integer(6)));
    QCOMPARE(t1.size(), 5U);

    QCOMPARE(t1.takeFirst(), Model::Variant(Model::Integer(1)));
    QCOMPARE(t1.size(), 4U);

    QCOMPARE(t1(1), Model::Variant(Model::Integer(2)));
    QCOMPARE(t1(2), Model::Variant(Model::Integer(3)));
    QCOMPARE(t1(3), Model::Variant(Model::Integer(4)));
    QCOMPARE(t1(4), Model::Variant(Model::Integer(5)));

    Model::Tuple t2 = t1;

    QCOMPARE(t2.takeLast(), Model::Variant(Model::Integer(5)));
    QCOMPARE(t2.size(), 3U);

    QCOMPARE(t2.takeFirst(), Model::Variant(Model::Integer(2)));
    QCOMPARE(t2.size(), 2U);

    QCOMPARE(t2(1), Model::Variant(Model::Integer(3)));
    QCOMPARE(t2(2), Model::Variant(Model::Integer(4)));

    QCOMPARE(t1(1), Model::Variant(Model::Integer(2)));
    QCOMPARE(t1(2), Model::Variant(Model::Integer(3)));
    QCOMPARE(t1(3), Model::Variant(Model::Integer(4)));
    QCOMPARE(t1(4), Model::Variant(Model::Integer(5)));
}


void TestModelTuple::testPopMethods() {
    Model::Tuple t1;

    t1.append(Model::Integer(1));
    t1.append(Model::Integer(2));
    t1.append(Model::Integer(3));
    t1.append(Model::Integer(4));
    t1.append(Model::Integer(5));
    t1.append(Model::Integer(6));

    QCOMPARE(t1.popLast(), true);
    QCOMPARE(t1.size(), 5U);

    QCOMPARE(t1.popFirst(), true);
    QCOMPARE(t1.size(), 4U);

    QCOMPARE(t1(1), Model::Variant(Model::Integer(2)));
    QCOMPARE(t1(2), Model::Variant(Model::Integer(3)));
    QCOMPARE(t1(3), Model::Variant(Model::Integer(4)));
    QCOMPARE(t1(4), Model::Variant(Model::Integer(5)));

    Model::Tuple t2 = t1;

    QCOMPARE(t2.popLast(), true);
    QCOMPARE(t2.size(), 3U);

    QCOMPARE(t2.popFirst(), true);
    QCOMPARE(t2.size(), 2U);

    QCOMPARE(t2(1), Model::Variant(Model::Integer(3)));
    QCOMPARE(t2(2), Model::Variant(Model::Integer(4)));

    QCOMPARE(t1(1), Model::Variant(Model::Integer(2)));
    QCOMPARE(t1(2), Model::Variant(Model::Integer(3)));
    QCOMPARE(t1(3), Model::Variant(Model::Integer(4)));
    QCOMPARE(t1(4), Model::Variant(Model::Integer(5)));

    Model::Tuple t3;
    QCOMPARE(t3.popFirst(), false);
    QCOMPARE(t3.popLast(), false);
}


void TestModelTuple::testStlPushPopMethods() {
    Model::Tuple t;

    t.push_back(Model::Integer(4));
    t.push_back(Model::Integer(5));
    t.push_back(Model::Integer(6));
    t.push_front(Model::Integer(3));
    t.push_front(Model::Integer(2));
    t.push_front(Model::Integer(1));

    QCOMPARE(t.size(), 6U);
    QCOMPARE(t(1), Model::Variant(Model::Integer(1)));
    QCOMPARE(t(2), Model::Variant(Model::Integer(2)));
    QCOMPARE(t(3), Model::Variant(Model::Integer(3)));
    QCOMPARE(t(4), Model::Variant(Model::Integer(4)));
    QCOMPARE(t(5), Model::Variant(Model::Integer(5)));
    QCOMPARE(t(6), Model::Variant(Model::Integer(6)));

    t.pop_front();
    t.pop_back();

    QCOMPARE(t.size(), 4U);
    QCOMPARE(t(1), Model::Variant(Model::Integer(2)));
    QCOMPARE(t(2), Model::Variant(Model::Integer(3)));
    QCOMPARE(t(3), Model::Variant(Model::Integer(4)));
    QCOMPARE(t(4), Model::Variant(Model::Integer(5)));
}


void TestModelTuple::testAtArrayUpdateMethods() {
    Model::Tuple t1;

    t1.append(Model::Integer(1));
    t1.append(Model::Integer(2));
    t1.append(Model::Integer(3));
    t1.append(Model::Integer(4));
    t1.append(Model::Integer(5));
    t1.append(Model::Integer(6));

    // Array methods already tested.

    t1.update(1, Model::Integer(0));

    QCOMPARE(t1(1), Model::Variant(Model::Integer(0)));
    QCOMPARE(t1(2), Model::Variant(Model::Integer(2)));
    QCOMPARE(t1(3), Model::Variant(Model::Integer(3)));
    QCOMPARE(t1(4), Model::Variant(Model::Integer(4)));
    QCOMPARE(t1(5), Model::Variant(Model::Integer(5)));
    QCOMPARE(t1(6), Model::Variant(Model::Integer(6)));

    t1.update(8, Model::Integer(8));
    QCOMPARE(t1.size(), 8U);

    QCOMPARE(t1(1), Model::Variant(Model::Integer(0)));
    QCOMPARE(t1(2), Model::Variant(Model::Integer(2)));
    QCOMPARE(t1(3), Model::Variant(Model::Integer(3)));
    QCOMPARE(t1(4), Model::Variant(Model::Integer(4)));
    QCOMPARE(t1(5), Model::Variant(Model::Integer(5)));
    QCOMPARE(t1(6), Model::Variant(Model::Integer(6)));
    QCOMPARE(t1(7), Model::Variant());
    QCOMPARE(t1(8), Model::Variant(Model::Integer(8)));

    Model::Tuple t2 = t1;
    t2.update(7, Model::Integer(7));

    QCOMPARE(t2(1), Model::Variant(Model::Integer(0)));
    QCOMPARE(t2(2), Model::Variant(Model::Integer(2)));
    QCOMPARE(t2(3), Model::Variant(Model::Integer(3)));
    QCOMPARE(t2(4), Model::Variant(Model::Integer(4)));
    QCOMPARE(t2(5), Model::Variant(Model::Integer(5)));
    QCOMPARE(t2(6), Model::Variant(Model::Integer(6)));
    QCOMPARE(t2(7), Model::Variant(Model::Integer(7)));
    QCOMPARE(t2(8), Model::Variant(Model::Integer(8)));

    QCOMPARE(t1(1), Model::Variant(Model::Integer(0)));
    QCOMPARE(t1(2), Model::Variant(Model::Integer(2)));
    QCOMPARE(t1(3), Model::Variant(Model::Integer(3)));
    QCOMPARE(t1(4), Model::Variant(Model::Integer(4)));
    QCOMPARE(t1(5), Model::Variant(Model::Integer(5)));
    QCOMPARE(t1(6), Model::Variant(Model::Integer(6)));
    QCOMPARE(t1(7), Model::Variant());
    QCOMPARE(t1(8), Model::Variant(Model::Integer(8)));
}


void TestModelTuple::testConstIterator() {
    Model::Tuple t;

    t.append(Model::Integer(1));
    t.append(Model::Integer(2));
    t.append(Model::Integer(3));
    t.append(Model::Integer(4));
    t.append(Model::Integer(5));
    t.append(Model::Integer(6));

    Model::Tuple::ConstIterator it  = t.constBegin();
    Model::Tuple::ConstIterator end = t.constEnd();

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(*it++, Model::Variant(Model::Integer(1)));

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(it.constReference(), Model::Variant(Model::Integer(2)));
    it.advance();

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(*it.constPointer(), Model::Variant(Model::Integer(3)));
    it.advance(2);

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(it->toInteger(), Model::Integer(5));
    it.backup();

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(*it, Model::Variant(Model::Integer(4)));
    it.backup(2);
    it.advance(4);

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(*it, Model::Variant(Model::Integer(6)));
    --it;

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(*it, Model::Variant(Model::Integer(5)));
    ++it;

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(*it, Model::Variant(Model::Integer(6)));
    ++it;

    QCOMPARE(it.isValid(), false);
    QCOMPARE(it.isInvalid(), true);
    QCOMPARE(it == end, true);
    QCOMPARE(it != end, false);
    QCOMPARE(*it, Model::Variant());
}


void TestModelTuple::testIterator() {
    Model::Tuple t;

    t.append(Model::Integer(1));
    t.append(Model::Integer(2));
    t.append(Model::Integer(3));
    t.append(Model::Integer(4));
    t.append(Model::Integer(5));
    t.append(Model::Integer(6));

    Model::Tuple::Iterator it  = t.begin();
    Model::Tuple::Iterator end = t.end();

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(*it++, Model::Variant(Model::Integer(1)));
    (it-1).setValue(Model::Variant(Model::Integer(11)));

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(it.constReference(), Model::Variant(Model::Integer(2)));
    it.setValue(Model::Variant(Model::Integer(12)));
    it.advance();

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(*it.constPointer(), Model::Variant(Model::Integer(3)));
    it.setValue(Model::Variant(Model::Integer(13)));
    it.advance(2);

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(it->toInteger(), Model::Integer(5));
    it.backup();
    (it+1).setValue(Model::Variant(Model::Integer(15)));

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(*it, Model::Variant(Model::Integer(4)));
    it.setValue(Model::Variant(Model::Integer(14)));
    it.backup(2);
    it.advance(4);

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(*it, Model::Variant(Model::Integer(6)));
    --it;
    (it+1).setValue(Model::Variant(Model::Integer(16)));

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(*it, Model::Variant(Model::Integer(15)));
    ++it;

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(*it, Model::Variant(Model::Integer(16)));
    ++it;

    QCOMPARE(it.isValid(), false);
    QCOMPARE(it.isInvalid(), true);
    QCOMPARE(it == end, true);
    QCOMPARE(it != end, false);
    QCOMPARE(*it, Model::Variant());

    QCOMPARE(t(1), Model::Variant(Model::Integer(11)));
    QCOMPARE(t(2), Model::Variant(Model::Integer(12)));
    QCOMPARE(t(3), Model::Variant(Model::Integer(13)));
    QCOMPARE(t(4), Model::Variant(Model::Integer(14)));
    QCOMPARE(t(5), Model::Variant(Model::Integer(15)));
    QCOMPARE(t(6), Model::Variant(Model::Integer(16)));
}


void TestModelTuple::testStringSupport() {
    //               12345678901234567890123456
    Model::Tuple t1("Here is an example string.");

    QCOMPARE(t1.size(), 26);
    char* s = t1.toString();
    QCOMPARE(QString::fromUtf8(s), QString("Here is an example string."));
    delete s;

    // times aleph tav fraktur-A fraktur-Z
    Model::Tuple t2("\xC3\x97 \xD7\x91 \xEF\xAC\xA8 \xF0\x9D\x94\x84 \xE2\x84\xA8");
    QCOMPARE(t2.size(), 9);
    QCOMPARE(t2.at(1), Model::Variant(Model::Integer(0xD7)));     // 0xC3 0x97             times symbol
    QCOMPARE(t2.at(2), Model::Variant(Model::Integer(0x20)));     //                       space
    QCOMPARE(t2.at(3), Model::Variant(Model::Integer(0x05D1)));   // 0xD7 0x91             bet
    QCOMPARE(t2.at(4), Model::Variant(Model::Integer(0x20)));     //                       space
    QCOMPARE(t2.at(5), Model::Variant(Model::Integer(0xFB28)));   // 0xEF 0xAC 0xA8        tav
    QCOMPARE(t2.at(6), Model::Variant(Model::Integer(0x20)));     //                       space
    QCOMPARE(t2.at(7), Model::Variant(Model::Integer(0x1D504L))); // 0xF0 0x9D 0x94 0x84   Fraktur A
    QCOMPARE(t2.at(8), Model::Variant(Model::Integer(0x20)));     //                       space
    QCOMPARE(t2.at(9), Model::Variant(Model::Integer(0x2128)));   // 0xE2 0x84 0xA8        Fraktur Z

    s = t2.toString();
    QCOMPARE(QString::fromUtf8(s), QString::fromUtf8("\xC3\x97 \xD7\x91 \xEF\xAC\xA8 \xF0\x9D\x94\x84 \xE2\x84\xA8"));
    delete s;

    // Badly terminated UTF-8 sequence
    bool threwException = false;
    try {
        Model::Tuple t3("\xC3\x97 \xD7");
    } catch (Model::MalformedString& e) {
        threwException = (e.byteOffset() == 4);
    }

    QCOMPARE(threwException, true);

    // Improper UTF-8 code.
    threwException = false;
    try {
        Model::Tuple t3("\xC3\x97 \xD7\xD1 ");
    } catch (Model::MalformedString& e) {
        threwException = (e.byteOffset() == 4);
    }

    QCOMPARE(threwException, true);
}


void TestModelTuple::testMultiplicationOperator() {
    Model::Tuple t1("Here is an ");
    Model::Tuple t2("example string.");

    Model::Tuple t3 = t1 * t2;

    char* s = t3.toString();
    QCOMPARE(s != nullptr, true);
    QCOMPARE(QString::fromUtf8(s), QString("Here is an example string."));
    delete s;

    t1 *= t2;

    s = t1.toString();
    QCOMPARE(s != nullptr, true);
    QCOMPARE(QString::fromUtf8(s), QString("Here is an example string."));
    delete s;
}


void TestModelTuple::testDivisionOperator() {
    Model::Tuple t1("Here is an example string.");
    Model::Tuple t2("Here is ");
    Model::Tuple t3(" string.");

    Model::Tuple t4 = t1 / t2;
    Model::Tuple t5 = t1 / t3;

    QCOMPARE(t4, Model::Tuple("an example string."));
    QCOMPARE(t5, Model::Tuple("Here is an example"));

    t1 /= t2;
    QCOMPARE(t1, Model::Tuple("an example string."));

    t1 /= t3;
    QCOMPARE(t1, Model::Tuple("an example"));
}


void TestModelTuple::testAssignmentOperator() {
    Model::Tuple t1;
    Model::Tuple t2;

    t1.append(Model::Integer(1));
    t1.append(Model::Integer(2));
    t1.append(Model::Integer(3));
    t1.append(Model::Integer(4));
    t1.append(Model::Integer(5));
    t1.append(Model::Integer(6));

    t2 = t1;

    t2.append(Model::Integer(1));
    t2.append(Model::Integer(2));
    t2.append(Model::Integer(3));
    t2.append(Model::Integer(4));
    t2.append(Model::Integer(5));
    t2.append(Model::Integer(6));
}


void TestModelTuple::testComparisonOperator() {
    Model::Tuple t1 = []{ Model::Tuple t; t.append(Model::Integer(1)); t.append(Model::Integer(2)); return t; }();
    Model::Tuple t2 = []{ Model::Tuple t; t.append(Model::Integer(1)); t.append(Model::Integer(2)); return t; }();
    Model::Tuple t3 = []{ Model::Tuple t; t.append(Model::Integer(2)); t.append(Model::Integer(3)); return t; }();
    Model::Tuple t4 = []{ Model::Tuple t; t.append(Model::Integer(1)); return t; }();

    QCOMPARE(t1 == t1, true);
    QCOMPARE(t1 != t1, false);

    QCOMPARE(t1 == t2, true);
    QCOMPARE(t1 != t2, false);

    QCOMPARE(t1 == t3, false);
    QCOMPARE(t1 != t3, true);

    QCOMPARE(t1 == t4, false);
    QCOMPARE(t1 != t4, true);
}


Model::Tuple TestModelTuple::createDefaultTuple() {
    Model::Tuple t;
    t.append(Model::Integer(1));
    t.append(Model::Integer(2));
    return t;
}
