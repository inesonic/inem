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
* This file implements tests of the \ref M::Tuple class.
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
#include <model_range.h>
#include <m_range.h>
#include <model_exceptions.h>
#include <model_variant.h>
#include <m_tuple.h>

#include "test_m_tuple.h"

TestMTuple::TestMTuple() {}


TestMTuple::~TestMTuple() {}


void TestMTuple::testConstructorsAndDestructors() {
    M::Tuple t1;
    t1.append(Model::Integer(1));

    QCOMPARE(t1.size(), 1U);

    M::Tuple t2(t1);
    QCOMPARE(t2.size(), 1U);
    QCOMPARE(t2.at(M::Integer(1)), M::Variant(Model::Integer(1)));
}


void TestMTuple::testAppendPrependMethods() {
    M::Tuple t1;

    t1.append(Model::Integer(1));
    t1.append(Model::Integer(2));
    t1.append(Model::Integer(3));
    t1.prepend(Model::Integer(4));
    t1.prepend(Model::Integer(5));
    t1.prepend(Model::Integer(6));

    QCOMPARE(t1.size(), 6U);
    QCOMPARE(t1[1], M::Variant(Model::Integer(6)));
    QCOMPARE(t1[2], M::Variant(Model::Integer(5)));
    QCOMPARE(t1[3], M::Variant(Model::Integer(4)));
    QCOMPARE(t1[4], M::Variant(Model::Integer(1)));
    QCOMPARE(t1[5], M::Variant(Model::Integer(2)));
    QCOMPARE(t1[6], M::Variant(Model::Integer(3)));

    // Check copy on write.
    M::Tuple t2 = t1;
    t2.append(Model::Integer(7));

    QCOMPARE(t1.size(), 6U);
    QCOMPARE(t2.size(), 7U);

    QCOMPARE(t1[1], M::Variant(Model::Integer(6)));
    QCOMPARE(t1[2], M::Variant(Model::Integer(5)));
    QCOMPARE(t1[3], M::Variant(Model::Integer(4)));
    QCOMPARE(t1[4], M::Variant(Model::Integer(1)));
    QCOMPARE(t1[5], M::Variant(Model::Integer(2)));
    QCOMPARE(t1[6], M::Variant(Model::Integer(3)));

    QCOMPARE(t2[1], M::Variant(Model::Integer(6)));
    QCOMPARE(t2[2], M::Variant(Model::Integer(5)));
    QCOMPARE(t2[3], M::Variant(Model::Integer(4)));
    QCOMPARE(t2[4], M::Variant(Model::Integer(1)));
    QCOMPARE(t2[5], M::Variant(Model::Integer(2)));
    QCOMPARE(t2[6], M::Variant(Model::Integer(3)));
    QCOMPARE(t2[7], M::Variant(Model::Integer(7)));
}


void TestMTuple::testRangeAppendPrependMethods() {
    M::Tuple t1;

    t1.append(M::Range(M::Integer(1), M::Integer(3), M::Integer(11)));

    QCOMPARE(t1.size(), 6U);
    QCOMPARE(t1[1], M::Variant(M::Integer(1)));
    QCOMPARE(t1[2], M::Variant(M::Integer(3)));
    QCOMPARE(t1[3], M::Variant(M::Integer(5)));
    QCOMPARE(t1[4], M::Variant(M::Integer(7)));
    QCOMPARE(t1[5], M::Variant(M::Integer(9)));
    QCOMPARE(t1[6], M::Variant(M::Integer(11)));

    t1.prepend(M::Range(M::Integer(2), M::Integer(4), M::Integer(8)));

    QCOMPARE(t1.size(), 10U);
    QCOMPARE(t1[1],  M::Variant(M::Integer(8)));
    QCOMPARE(t1[2],  M::Variant(M::Integer(6)));
    QCOMPARE(t1[3],  M::Variant(M::Integer(4)));
    QCOMPARE(t1[4],  M::Variant(M::Integer(2)));
    QCOMPARE(t1[5],  M::Variant(M::Integer(1)));
    QCOMPARE(t1[6],  M::Variant(M::Integer(3)));
    QCOMPARE(t1[7],  M::Variant(M::Integer(5)));
    QCOMPARE(t1[8],  M::Variant(M::Integer(7)));
    QCOMPARE(t1[9],  M::Variant(M::Integer(9)));
    QCOMPARE(t1[10], M::Variant(M::Integer(11)));
}


void TestMTuple::testTakeFirstLastMethods() {
    M::Tuple t1;

    t1.append(Model::Integer(1));
    t1.append(Model::Integer(2));
    t1.append(Model::Integer(3));
    t1.append(Model::Integer(4));
    t1.append(Model::Integer(5));
    t1.append(Model::Integer(6));

    QCOMPARE(t1.takeLast(), M::Variant(Model::Integer(6)));
    QCOMPARE(t1.size(), 5U);

    QCOMPARE(t1.takeFirst(), M::Variant(Model::Integer(1)));
    QCOMPARE(t1.size(), 4U);

    QCOMPARE(t1[1], M::Variant(Model::Integer(2)));
    QCOMPARE(t1[2], M::Variant(Model::Integer(3)));
    QCOMPARE(t1[3], M::Variant(Model::Integer(4)));
    QCOMPARE(t1[4], M::Variant(Model::Integer(5)));

    M::Tuple t2 = t1;

    QCOMPARE(t2.takeLast(), M::Variant(Model::Integer(5)));
    QCOMPARE(t2.size(), 3U);

    QCOMPARE(t2.takeFirst(), M::Variant(Model::Integer(2)));
    QCOMPARE(t2.size(), 2U);

    QCOMPARE(t2[1], M::Variant(Model::Integer(3)));
    QCOMPARE(t2[2], M::Variant(Model::Integer(4)));

    QCOMPARE(t1[1], M::Variant(Model::Integer(2)));
    QCOMPARE(t1[2], M::Variant(Model::Integer(3)));
    QCOMPARE(t1[3], M::Variant(Model::Integer(4)));
    QCOMPARE(t1[4], M::Variant(Model::Integer(5)));
}


void TestMTuple::testPopMethods() {
    M::Tuple t1;

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

    QCOMPARE(t1[1], M::Variant(Model::Integer(2)));
    QCOMPARE(t1[2], M::Variant(Model::Integer(3)));
    QCOMPARE(t1[3], M::Variant(Model::Integer(4)));
    QCOMPARE(t1[4], M::Variant(Model::Integer(5)));

    M::Tuple t2 = t1;

    QCOMPARE(t2.popLast(), true);
    QCOMPARE(t2.size(), 3U);

    QCOMPARE(t2.popFirst(), true);
    QCOMPARE(t2.size(), 2U);

    QCOMPARE(t2[1], M::Variant(Model::Integer(3)));
    QCOMPARE(t2[2], M::Variant(Model::Integer(4)));

    QCOMPARE(t1[1], M::Variant(Model::Integer(2)));
    QCOMPARE(t1[2], M::Variant(Model::Integer(3)));
    QCOMPARE(t1[3], M::Variant(Model::Integer(4)));
    QCOMPARE(t1[4], M::Variant(Model::Integer(5)));

    M::Tuple t3;
    QCOMPARE(t3.popFirst(), false);
    QCOMPARE(t3.popLast(), false);
}


void TestMTuple::testStlPushPopMethods() {
    M::Tuple t;

    t.push_back(Model::Integer(4));
    t.push_back(Model::Integer(5));
    t.push_back(Model::Integer(6));
    t.push_front(Model::Integer(3));
    t.push_front(Model::Integer(2));
    t.push_front(Model::Integer(1));

    QCOMPARE(t.size(), 6U);
    QCOMPARE(t[1], M::Variant(Model::Integer(1)));
    QCOMPARE(t[2], M::Variant(Model::Integer(2)));
    QCOMPARE(t[3], M::Variant(Model::Integer(3)));
    QCOMPARE(t[4], M::Variant(Model::Integer(4)));
    QCOMPARE(t[5], M::Variant(Model::Integer(5)));
    QCOMPARE(t[6], M::Variant(Model::Integer(6)));

    t.pop_front();
    t.pop_back();

    QCOMPARE(t.size(), 4U);
    QCOMPARE(t[1], M::Variant(Model::Integer(2)));
    QCOMPARE(t[2], M::Variant(Model::Integer(3)));
    QCOMPARE(t[3], M::Variant(Model::Integer(4)));
    QCOMPARE(t[4], M::Variant(Model::Integer(5)));
}


void TestMTuple::testAtArrayUpdateMethods() {
    M::Tuple t1;

    t1.append(Model::Integer(1));
    t1.append(Model::Integer(2));
    t1.append(Model::Integer(3));
    t1.append(Model::Integer(4));
    t1.append(Model::Integer(5));
    t1.append(Model::Integer(6));

    // Array methods already tested.

    t1.update(1, Model::Integer(0));

    QCOMPARE(t1[1], M::Variant(Model::Integer(0)));
    QCOMPARE(t1[2], M::Variant(Model::Integer(2)));
    QCOMPARE(t1[3], M::Variant(Model::Integer(3)));
    QCOMPARE(t1[4], M::Variant(Model::Integer(4)));
    QCOMPARE(t1[5], M::Variant(Model::Integer(5)));
    QCOMPARE(t1[6], M::Variant(Model::Integer(6)));

    t1.update(8, Model::Integer(8));
    QCOMPARE(t1.size(), 8U);

    QCOMPARE(t1[1], M::Variant(Model::Integer(0)));
    QCOMPARE(t1[2], M::Variant(Model::Integer(2)));
    QCOMPARE(t1[3], M::Variant(Model::Integer(3)));
    QCOMPARE(t1[4], M::Variant(Model::Integer(4)));
    QCOMPARE(t1[5], M::Variant(Model::Integer(5)));
    QCOMPARE(t1[6], M::Variant(Model::Integer(6)));
    QCOMPARE(t1[7], M::Variant());
    QCOMPARE(t1[8], M::Variant(Model::Integer(8)));

    M::Tuple t2 = t1;
    t2.update(7, Model::Integer(7));

    QCOMPARE(t2[1], M::Variant(Model::Integer(0)));
    QCOMPARE(t2[2], M::Variant(Model::Integer(2)));
    QCOMPARE(t2[3], M::Variant(Model::Integer(3)));
    QCOMPARE(t2[4], M::Variant(Model::Integer(4)));
    QCOMPARE(t2[5], M::Variant(Model::Integer(5)));
    QCOMPARE(t2[6], M::Variant(Model::Integer(6)));
    QCOMPARE(t2[7], M::Variant(Model::Integer(7)));
    QCOMPARE(t2[8], M::Variant(Model::Integer(8)));

    QCOMPARE(t1[1], M::Variant(Model::Integer(0)));
    QCOMPARE(t1[2], M::Variant(Model::Integer(2)));
    QCOMPARE(t1[3], M::Variant(Model::Integer(3)));
    QCOMPARE(t1[4], M::Variant(Model::Integer(4)));
    QCOMPARE(t1[5], M::Variant(Model::Integer(5)));
    QCOMPARE(t1[6], M::Variant(Model::Integer(6)));
    QCOMPARE(t1[7], M::Variant());
    QCOMPARE(t1[8], M::Variant(Model::Integer(8)));
}


void TestMTuple::testAtMethods() {
    M::Tuple t = M::Tuple::build(
        M::Integer(11), M::Integer(12), M::Integer(13), M::Integer(14), M::Integer(15),
        M::Integer(16), M::Integer(17), M::Integer(18), M::Integer(19), M::Integer(20)
    );

    M::Integer       i = 3;
    M::Real          r = 4;
    M::Complex       c(5);
    M::Range         rng(M::Integer(5), M::Integer(6));
    M::Set           set   = M::Set::build(M::Integer(2), M::Integer(4));
    M::Tuple         tuple = M::Tuple::build(M::Integer(3), M::Integer(5));
    M::MatrixInteger mi = M::MatrixInteger::build(2, 2, M::Integer(2), M::Integer(6), M::Integer(4), M::Integer(8));
    M::MatrixReal    mr = M::MatrixReal::build(2, 2, M::Real(1), M::Real(3), M::Real(2), M::Real(4));
    M::MatrixComplex mc = M::MatrixComplex::build(1, 2, M::Complex(1), M::Complex(5));

    M::Variant  vi(i);
    M::Variant  vr(r);
    M::Variant  vc(c);
    M::Variant  vset(set);
    M::Variant  vtuple(tuple);
    M::Variant  vmi(mi);
    M::Variant  vmr(mr);
    M::Variant  vmc(mc);

    QCOMPARE(t.at(i), M::Variant(M::Integer(13)));
    QCOMPARE(t.at(r), M::Variant(M::Integer(14)));
    QCOMPARE(t.at(c), M::Variant(M::Integer(15)));
    QCOMPARE(t.at(rng), M::Tuple::build(M::Integer(15), M::Integer(16)));
    QCOMPARE(t.at(set), M::Tuple::build(M::Integer(12), M::Integer(14)));
    QCOMPARE(t.at(tuple), M::Tuple::build(M::Integer(13), M::Integer(15)));
    QCOMPARE(t.at(mi), M::Tuple::build(M::Integer(12), M::Integer(14), M::Integer(16), M::Integer(18)));
    QCOMPARE(t.at(mr), M::Tuple::build(M::Integer(11), M::Integer(12), M::Integer(13), M::Integer(14)));
    QCOMPARE(t.at(mc), M::Tuple::build(M::Integer(11), M::Integer(15)));


    QCOMPARE(t.at(vi), t.at(i));
    QCOMPARE(t.at(vr), t.at(r));
    QCOMPARE(t.at(vc), t.at(c));
    QCOMPARE(t.at(vset), t.at(set));
    QCOMPARE(t.at(vtuple), t.at(tuple));
    QCOMPARE(t.at(vmi), t.at(mi));
    QCOMPARE(t.at(vmr), t.at(mr));
    QCOMPARE(t.at(vmc), t.at(mc));
}


void TestMTuple::testConstIterator() {
    M::Tuple t;

    t.append(Model::Integer(1));
    t.append(Model::Integer(2));
    t.append(Model::Integer(3));
    t.append(Model::Integer(4));
    t.append(Model::Integer(5));
    t.append(Model::Integer(6));

    M::Tuple::ConstIterator it  = t.constBegin();
    M::Tuple::ConstIterator end = t.constEnd();

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(*it++, M::Variant(Model::Integer(1)));

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(it.constReference(), M::Variant(Model::Integer(2)));
    it.advance();

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(*it.constPointer(), M::Variant(Model::Integer(3)));
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
    QCOMPARE(*it, M::Variant(Model::Integer(4)));
    it.backup(2);
    it.advance(4);

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(*it, M::Variant(Model::Integer(6)));
    --it;

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(*it, M::Variant(Model::Integer(5)));
    ++it;

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(*it, M::Variant(Model::Integer(6)));
    ++it;

    QCOMPARE(it.isValid(), false);
    QCOMPARE(it.isInvalid(), true);
    QCOMPARE(it == end, true);
    QCOMPARE(it != end, false);
}


void TestMTuple::testIterator() {
    M::Tuple t;

    t.append(Model::Integer(1));
    t.append(Model::Integer(2));
    t.append(Model::Integer(3));
    t.append(Model::Integer(4));
    t.append(Model::Integer(5));
    t.append(Model::Integer(6));

    M::Tuple::Iterator it  = t.begin();
    M::Tuple::Iterator end = t.end();

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(*it++, M::Variant(Model::Integer(1)));
    *(it-1) = Model::Integer(11);

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(it.constReference(), M::Variant(Model::Integer(2)));
    it.reference() = Model::Integer(12);
    it.advance();

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(*it.constPointer(), M::Variant(Model::Integer(3)));
    *it.pointer() = Model::Integer(13);
    it.advance(2);

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(it->toInteger(), Model::Integer(5));
    it.backup();
    *(it+1) = Model::Integer(15);

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(*it, M::Variant(Model::Integer(4)));
    *it = Model::Integer(14);
    it.backup(2);
    it.advance(4);

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(*it, M::Variant(Model::Integer(6)));
    --it;
    *(it+1) = Model::Integer(16);

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(*it, M::Variant(Model::Integer(15)));
    ++it;

    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(it == end, false);
    QCOMPARE(it != end, true);
    QCOMPARE(*it, M::Variant(Model::Integer(16)));
    ++it;

    QCOMPARE(it.isValid(), false);
    QCOMPARE(it.isInvalid(), true);
    QCOMPARE(it == end, true);
    QCOMPARE(it != end, false);

    QCOMPARE(t[1], M::Variant(Model::Integer(11)));
    QCOMPARE(t[2], M::Variant(Model::Integer(12)));
    QCOMPARE(t[3], M::Variant(Model::Integer(13)));
    QCOMPARE(t[4], M::Variant(Model::Integer(14)));
    QCOMPARE(t[5], M::Variant(Model::Integer(15)));
    QCOMPARE(t[6], M::Variant(Model::Integer(16)));
}


void TestMTuple::testStringSupport() {
    //          12345678901234567890123456
    M::Tuple t1("Here is an example string.");

    QCOMPARE(t1.size(), 26);
    char* s = t1.toString();
    QCOMPARE(QString::fromUtf8(s), QString("Here is an example string."));
    delete s;

    // times aleph tav fraktur-A fraktur-Z
    M::Tuple t2("\xC3\x97 \xD7\x91 \xEF\xAC\xA8 \xF0\x9D\x94\x84 \xE2\x84\xA8");
    QCOMPARE(t2.size(), 9);
    QCOMPARE(t2.at(M::Integer(1)), M::Variant(Model::Integer(0xD7)));     // 0xC3 0x97             times symbol
    QCOMPARE(t2.at(M::Integer(2)), M::Variant(Model::Integer(0x20)));     //                       space
    QCOMPARE(t2.at(M::Integer(3)), M::Variant(Model::Integer(0x05D1)));   // 0xD7 0x91             bet
    QCOMPARE(t2.at(M::Integer(4)), M::Variant(Model::Integer(0x20)));     //                       space
    QCOMPARE(t2.at(M::Integer(5)), M::Variant(Model::Integer(0xFB28)));   // 0xEF 0xAC 0xA8        tav
    QCOMPARE(t2.at(M::Integer(6)), M::Variant(Model::Integer(0x20)));     //                       space
    QCOMPARE(t2.at(M::Integer(7)), M::Variant(Model::Integer(0x1D504L))); // 0xF0 0x9D 0x94 0x84   Fraktur A
    QCOMPARE(t2.at(M::Integer(8)), M::Variant(Model::Integer(0x20)));     //                       space
    QCOMPARE(t2.at(M::Integer(9)), M::Variant(Model::Integer(0x2128)));   // 0xE2 0x84 0xA8        Fraktur Z

    s = t2.toString();
    QCOMPARE(QString::fromUtf8(s), QString::fromUtf8("\xC3\x97 \xD7\x91 \xEF\xAC\xA8 \xF0\x9D\x94\x84 \xE2\x84\xA8"));
    delete s;

    // Badly terminated UTF-8 sequence
    bool threwException = false;
    try {
        M::Tuple t3("\xC3\x97 \xD7");
    } catch (Model::MalformedString& e) {
        threwException = (e.byteOffset() == 4);
    }

    QCOMPARE(threwException, true);

    // Improper UTF-8 code.
    threwException = false;
    try {
        M::Tuple t3("\xC3\x97 \xD7\xD1 ");
    } catch (Model::MalformedString& e) {
        threwException = (e.byteOffset() == 4);
    }

    QCOMPARE(threwException, true);
}


void TestMTuple::testMultiplicationOperator() {
    M::Tuple t1("Here is an ");
    M::Tuple t2("example string.");

    M::Tuple t3 = t1 * t2;

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


void TestMTuple::testDivisionOperator() {
    M::Tuple t1("Here is an example string.");
    M::Tuple t2("Here is ");
    M::Tuple t3(" string.");

    M::Tuple t4 = t1 / t2;
    M::Tuple t5 = t1 / t3;

    QCOMPARE(t4, M::Tuple("an example string."));
    QCOMPARE(t5, M::Tuple("Here is an example"));

    t1 /= t2;
    QCOMPARE(t1, M::Tuple("an example string."));

    t1 /= t3;
    QCOMPARE(t1, M::Tuple("an example"));
}


void TestMTuple::testAssignmentOperator() {
    M::Tuple t1;
    M::Tuple t2;

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


void TestMTuple::testComparisonOperator() {
    M::Tuple t1 = []{ M::Tuple t; t.append(Model::Integer(1)); t.append(Model::Integer(2)); return t; }();
    M::Tuple t2 = []{ M::Tuple t; t.append(Model::Integer(1)); t.append(Model::Integer(2)); return t; }();
    M::Tuple t3 = []{ M::Tuple t; t.append(Model::Integer(2)); t.append(Model::Integer(3)); return t; }();
    M::Tuple t4 = []{ M::Tuple t; t.append(Model::Integer(1)); return t; }();

    QCOMPARE(t1 == t1, true);
    QCOMPARE(t1 != t1, false);

    QCOMPARE(t1 == t2, true);
    QCOMPARE(t1 != t2, false);

    QCOMPARE(t1 == t3, false);
    QCOMPARE(t1 != t3, true);

    QCOMPARE(t1 == t4, false);
    QCOMPARE(t1 != t4, true);
}


M::Tuple TestMTuple::createDefaultTuple() {
    M::Tuple t;
    t.append(Model::Integer(1));
    t.append(Model::Integer(2));
    return t;
}
