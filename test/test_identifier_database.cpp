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
* This file implements tests of the \ref IdentifierDatabase class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QObject>
#include <QString>
#include <QSet>
#include <QtTest/QtTest>

#include <m_api_types.h>
#include <m_variant.h>
#include <model_intrinsic_types.h>
#include <model_set.h>
#include <model_tuple.h>
#include <m_set.h>
#include <m_tuple.h>
#include <model_identifier_data.h>
#include <model_variant.h>
#include <m_identifier_database.h>

#include "test_identifier_database.h"


TestIdentifierDatabase::TestIdentifierDatabase() {}


TestIdentifierDatabase::~TestIdentifierDatabase() {}


void TestIdentifierDatabase::testConstructorsAndDestructors() {
    M::IdentifierDatabase db1;
    QCOMPARE(db1.isEmpty(), true);
    QCOMPARE(db1.size(), 0U);

    Model::Integer fooBar;

    M::IdentifierDatabase db2;
    db2.insert(1, "foo", "bar", false, Model::ValueType::INTEGER, &fooBar);

    QCOMPARE(db2.isEmpty(), false);
    QCOMPARE(db2.size(), 1U);

    M::IdentifierDatabase db3(db2);

    QCOMPARE(db3.isEmpty(), false);
    QCOMPARE(db3.size(), 1U);
}


void TestIdentifierDatabase::testStatusAccessors() {
    // IdentifierDatabase::isEmpty and IdentifierDatabase::size tested in testConstructorsAndDestructors.
}


void TestIdentifierDatabase::testInsertAndQueryMethods() {
    Model::Boolean bi = true;
    Model::Integer ii = 1234L;
    Model::Real    ri = 1.234;
    Model::Complex ci(1.25, 5.75);
    M::Set         si = M::Set::build(Model::Integer(1), Model::Integer(2));
    M::Tuple       ti = M::Tuple::build(si);

    M::IdentifierDatabase db1;
    bool success = db1.insert(0, "b", "bi", false, Model::ValueType::BOOLEAN, &bi);
    QCOMPARE(success, true);
    QCOMPARE(db1.size(), 1U);

    success = db1.insert(0, "x", "xi", false, Model::ValueType::INTEGER, &ii); // Duplicate handle.
    QCOMPARE(success, false);
    QCOMPARE(db1.size(), 1U);

    success = db1.insert(1, "b", "bi", false, Model::ValueType::INTEGER, &ii); // Duplicate name.
    QCOMPARE(success, false);
    QCOMPARE(db1.size(), 1U);

    success = db1.insert(1, "i", "ii", false, Model::ValueType::INTEGER, &ii);
    QCOMPARE(success, true);
    QCOMPARE(db1.size(), 2U);

    success = db1.insert(2, "r", "ri", false, Model::ValueType::REAL, &ri);
    QCOMPARE(success, true);
    QCOMPARE(db1.size(), 3U);

    success = db1.insert(3, "c", "ci", false, Model::ValueType::COMPLEX, &ci);
    QCOMPARE(success, true);
    QCOMPARE(db1.size(), 4U);

    success = db1.insert(4, "s", "si", false, Model::ValueType::SET, &si);
    QCOMPARE(success, true);
    QCOMPARE(db1.size(), 5U);

    success = db1.insert(5, "t", "ti", false, Model::ValueType::TUPLE, &ti);
    QCOMPARE(success, true);
    QCOMPARE(db1.size(), 6U);

    Model::IdentifierDatabase db2 = db1;

    Model::IdentifierData xid = db2.identifierDataByName("x", "xi");
    Model::IdentifierData yid = db2.identifierDataByHandle(64);

    QCOMPARE(xid.isValid(), false);
    QCOMPARE(xid.isInvalid(), true);
    QCOMPARE(yid.isValid(), false);
    QCOMPARE(yid.isInvalid(), true);

    Model::IdentifierData bid = db2.identifierDataByName("b", "bi");
    Model::IdentifierData iid = db2.identifierDataByHandle(1);
    Model::IdentifierData rid = db2.identifierDataByName("r", "ri");
    Model::IdentifierData cid = db2.identifierDataByHandle(3);
    Model::IdentifierData sid = db2.identifierDataByName("s", "si");
    Model::IdentifierData tid = db2.identifierDataByHandle(5);

    QCOMPARE(bid.identifierHandle(), 0U);
    QCOMPARE(iid.identifierHandle(), 1U);
    QCOMPARE(rid.identifierHandle(), 2U);
    QCOMPARE(cid.identifierHandle(), 3U);
    QCOMPARE(sid.identifierHandle(), 4U);
    QCOMPARE(tid.identifierHandle(), 5U);

    QCOMPARE(bid.valueType(), Model::ValueType::BOOLEAN);
    QCOMPARE(iid.valueType(), Model::ValueType::INTEGER);
    QCOMPARE(rid.valueType(), Model::ValueType::REAL);
    QCOMPARE(cid.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(sid.valueType(), Model::ValueType::SET);
    QCOMPARE(tid.valueType(), Model::ValueType::TUPLE);

    QCOMPARE(QString(bid.text1()), QString("b"));
    QCOMPARE(QString(iid.text1()), QString("i"));
    QCOMPARE(QString(rid.text1()), QString("r"));
    QCOMPARE(QString(cid.text1()), QString("c"));
    QCOMPARE(QString(sid.text1()), QString("s"));
    QCOMPARE(QString(tid.text1()), QString("t"));

    QCOMPARE(QString(bid.text2()), QString("bi"));
    QCOMPARE(QString(iid.text2()), QString("ii"));
    QCOMPARE(QString(rid.text2()), QString("ri"));
    QCOMPARE(QString(cid.text2()), QString("ci"));
    QCOMPARE(QString(sid.text2()), QString("si"));
    QCOMPARE(QString(tid.text2()), QString("ti"));

    Model::Boolean b = bid.value();
    Model::Integer i = iid.value();
    Model::Real    r = rid.value();
    Model::Complex c = cid.value();
    Model::Set     s = sid.value();
    Model::Tuple   t = tid.value();

    QCOMPARE(b, bi);
    QCOMPARE(i, ii);
    QCOMPARE(r, ri);
    QCOMPARE(c, ci);
    QCOMPARE(s, si);
    QCOMPARE(t, ti);

    bid.setValue(Model::Boolean(false));
    iid.setValue(Model::Integer(5678));
    rid.setValue(Model::Real(3.21));
    cid.setValue(Model::Complex(4.75, 3.25));
    sid.setValue(M::Set::build(Model::Integer(5), Model::Integer(6), Model::Integer(7)));
    tid.setValue(M::Tuple::build(Model::Integer(8), Model::Integer(9)));

    QCOMPARE(bi, false);
    QCOMPARE(ii, Model::Integer(5678));
    QCOMPARE(ri, Model::Real(3.21));
    QCOMPARE(ci, Model::Complex(4.75, 3.25));
    QCOMPARE(si, M::Set::build(Model::Integer(5), Model::Integer(6), Model::Integer(7)));
    QCOMPARE(ti, M::Tuple::build(Model::Integer(8), Model::Integer(9)));
}


void TestIdentifierDatabase::testAssignmentOperator() {
    M::IdentifierDatabase db1;
    QCOMPARE(db1.isEmpty(), true);
    QCOMPARE(db1.size(), 0U);

    Model::Integer fooBar;

    M::IdentifierDatabase db2;
    db2.insert(1, "foo", "bar", false, Model::ValueType::INTEGER, &fooBar);

    QCOMPARE(db2.isEmpty(), false);
    QCOMPARE(db2.size(), 1U);

    db1 = db2;

    QCOMPARE(db2.isEmpty(), false);
    QCOMPARE(db2.size(), 1U);

    Model::IdentifierData id = db2.identifierDataByHandle(1);
    QCOMPARE(id.isValid(), true);
    QCOMPARE(id.isInvalid(), false);
    QCOMPARE(id.identifierHandle(), 1U);
    QCOMPARE(QString(id.text1()), QString("foo"));
    QCOMPARE(QString(id.text2()), QString("bar"));
}


void TestIdentifierDatabase::testIterator() {
    Model::Boolean bi = true;
    Model::Integer ii = 1234L;
    Model::Real    ri = 1.234;
    Model::Complex ci(1.25, 5.75);
    M::Set         si = M::Set::build(Model::Integer(1), Model::Integer(2));
    M::Tuple       ti = M::Tuple::build(si);

    M::IdentifierDatabase db1;
    bool success = db1.insert(0, "b", "bi", false, Model::ValueType::BOOLEAN, &bi);
    QCOMPARE(success, true);
    QCOMPARE(db1.size(), 1U);

    success = db1.insert(0, "x", "xi", false, Model::ValueType::INTEGER, &ii); // Duplicate handle.
    QCOMPARE(success, false);
    QCOMPARE(db1.size(), 1U);

    success = db1.insert(1, "b", "bi", false, Model::ValueType::INTEGER, &ii); // Duplicate name.
    QCOMPARE(success, false);
    QCOMPARE(db1.size(), 1U);

    success = db1.insert(1, "i", "ii", false, Model::ValueType::INTEGER, &ii);
    QCOMPARE(success, true);
    QCOMPARE(db1.size(), 2U);

    success = db1.insert(2, "r", "ri", false, Model::ValueType::REAL, &ri);
    QCOMPARE(success, true);
    QCOMPARE(db1.size(), 3U);

    success = db1.insert(3, "c", "ci", false, Model::ValueType::COMPLEX, &ci);
    QCOMPARE(success, true);
    QCOMPARE(db1.size(), 4U);

    success = db1.insert(4, "s", "si", false, Model::ValueType::SET, &si);
    QCOMPARE(success, true);
    QCOMPARE(db1.size(), 5U);

    success = db1.insert(5, "t", "ti", false, Model::ValueType::TUPLE, &ti);
    QCOMPARE(success, true);
    QCOMPARE(db1.size(), 6U);

    Model::IdentifierDatabase db2 = db1;

    Model::IdentifierDatabase::Iterator it  = db2.begin();
    Model::IdentifierDatabase::Iterator end = db2.end();

    QSet<Model::IdentifierHandle> foundHandles;

    while (it != end) {
        QCOMPARE(it.isValid(), true);
        QCOMPARE(it.isInvalid(), false);

        QCOMPARE(foundHandles.contains(it.identifierHandle()), false);
        foundHandles.insert(it.identifierHandle());

        switch (it.identifierHandle()) {
            case 0: {
                Model::IdentifierData identifierData = it.value();

                QCOMPARE(identifierData.identifierHandle(), 0U);
                QCOMPARE(QString(identifierData.text1()), QString("b"));
                QCOMPARE(QString(identifierData.text2()), QString("bi"));
                QCOMPARE(identifierData.valueType(), Model::ValueType::BOOLEAN);

                ++it;
                break;
            }

            case 1: {
                const Model::IdentifierData& identifierData = it.constReference();

                QCOMPARE(identifierData.identifierHandle(), 1U);
                QCOMPARE(QString(identifierData.text1()), QString("i"));
                QCOMPARE(QString(identifierData.text2()), QString("ii"));
                QCOMPARE(identifierData.valueType(), Model::ValueType::INTEGER);

                it++;
                break;
            }

            case 2: {
                const Model::IdentifierData* identifierData = it.constPointer();

                QCOMPARE(identifierData->identifierHandle(), 2U);
                QCOMPARE(QString(identifierData->text1()), QString("r"));
                QCOMPARE(QString(identifierData->text2()), QString("ri"));
                QCOMPARE(identifierData->valueType(), Model::ValueType::REAL);

                it.advance();
                break;
            }

            case 3: {
                QCOMPARE(it->identifierHandle(), 3U);
                QCOMPARE(QString(it->text1()), QString("c"));
                QCOMPARE(QString(it->text2()), QString("ci"));
                QCOMPARE(it->valueType(), Model::ValueType::COMPLEX);

                it.advance(1);
                break;
            }

            case 4: {
                QCOMPARE((*it).identifierHandle(), 4U);
                QCOMPARE(QString((*it).text1()), QString("s"));
                QCOMPARE(QString((*it).text2()), QString("si"));
                QCOMPARE((*it).valueType(), Model::ValueType::SET);

                it.advance(1);
                break;
            }

            case 5: {
                QCOMPARE(it.identifierHandle(), 5U);
                QCOMPARE(QString(it.text1()), QString("t"));
                QCOMPARE(QString(it.text2()), QString("ti"));
                QCOMPARE(it.valueType(), Model::ValueType::TUPLE);

                it.advance(1);
                break;
            }

            default: {
                QVERIFY(false);
                break;
            }
        }
    }

    QCOMPARE(it.isValid(), false);
    QCOMPARE(it.isInvalid(), true);
    QCOMPARE(it == end, true);
    QCOMPARE(it != end, false);

    QCOMPARE(foundHandles.contains(0), true);
    QCOMPARE(foundHandles.contains(1), true);
    QCOMPARE(foundHandles.contains(2), true);
    QCOMPARE(foundHandles.contains(3), true);
    QCOMPARE(foundHandles.contains(4), true);
    QCOMPARE(foundHandles.contains(5), true);
    QCOMPARE(foundHandles.size(), 6);
}
