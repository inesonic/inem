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
* This file implements tests of the \ref M::Set class.
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
#include <m_intrinsic_types.h>
#include <m_variant.h>
#include <model_variant.h>
#include <model_range.h>
#include <m_range.h>
#include <m_set.h>
#include <m_set_iterator.h>

#include "test_set.h"

TestSet::TestSet() {}


TestSet::~TestSet() {}


void TestSet::testInsertMethod() {
    M::Set               set;
    QSet<Model::Integer> sv;

    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<> uniform(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    for (unsigned i=0 ; i<numberInsertedValues ; ++i) {
        Model::Integer value;
        do {
            value = uniform(rng);
        } while (sv.contains(value));

        set.insert(M::Variant(value));
        sv.insert(value);
    }

    QCOMPARE(static_cast<unsigned>(sv.size()), set.size());

    for (Model::Integer value : sv) {
        QCOMPARE(set.contains(M::Variant(value)), true);
    }
}


void TestSet::testInsertRangeMethod() {
    M::Set set1;

    bool allValuesUnique = set1.insert(M::Range(M::Real(1.0), M::Real(3.0), M::Real(7.0)));
    QVERIFY(allValuesUnique);

    QCOMPARE(set1.size(), 4);
    QCOMPARE(set1.contains(M::Real(1.0)), true);
    QCOMPARE(set1.contains(M::Real(3.0)), true);
    QCOMPARE(set1.contains(M::Real(5.0)), true);
    QCOMPARE(set1.contains(M::Real(7.0)), true);

    allValuesUnique = set1.insert(M::Range(M::Real(5.0), M::Real(9.0)));
    QCOMPARE(allValuesUnique, false);

    QCOMPARE(set1.size(), 7);
    QCOMPARE(set1.contains(M::Real(1.0)), true);
    QCOMPARE(set1.contains(M::Real(3.0)), true);
    QCOMPARE(set1.contains(M::Real(5.0)), true);
    QCOMPARE(set1.contains(M::Real(6.0)), true);
    QCOMPARE(set1.contains(M::Real(7.0)), true);
    QCOMPARE(set1.contains(M::Real(8.0)), true);
    QCOMPARE(set1.contains(M::Real(9.0)), true);

    Model::Set set2;

    allValuesUnique = set2.insert(Model::Range(M::Real(1.0), M::Real(3.0), M::Real(7.0)));
    QVERIFY(allValuesUnique);

    QCOMPARE(set2.size(), 4);
    QCOMPARE(set2.contains(M::Real(1.0)), true);
    QCOMPARE(set2.contains(M::Real(3.0)), true);
    QCOMPARE(set2.contains(M::Real(5.0)), true);
    QCOMPARE(set2.contains(M::Real(7.0)), true);

    allValuesUnique = set2.insert(M::Range(M::Real(5.0), M::Real(9.0)));
    QCOMPARE(allValuesUnique, false);

    QCOMPARE(set2.size(), 7);
    QCOMPARE(set2.contains(M::Real(1.0)), true);
    QCOMPARE(set2.contains(M::Real(3.0)), true);
    QCOMPARE(set2.contains(M::Real(5.0)), true);
    QCOMPARE(set2.contains(M::Real(6.0)), true);
    QCOMPARE(set2.contains(M::Real(7.0)), true);
    QCOMPARE(set2.contains(M::Real(8.0)), true);
    QCOMPARE(set2.contains(M::Real(9.0)), true);
}


void TestSet::testRemoveMethod() {
    std::mt19937                    rng(0x1234);
    std::uniform_int_distribution<> uniform(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    M::Set set;

    QMap<Model::Integer, Model::Integer> values;
    for (unsigned i=0 ; i<numberInsertedValues ; ++i) {
        Model::Integer value;
        do {
            value = uniform(rng);
        } while (values.contains(value));

        values.insert(value, value);
        bool success = set.insert(value);
        QVERIFY(success);
    }

    do {
        Model::Integer value;
        bool           knownValue = (uniform(rng) >= 0);
        if (knownValue) {
            value = *values.begin();
        } else {
            value      = uniform(rng);
            knownValue = values.contains(value);
        }

        bool success = set.remove(value);
        QCOMPARE(success, knownValue);

        if (knownValue) {
            values.remove(value);
        }

        QCOMPARE(set.size(), values.size());
    } while (!values.isEmpty());

    QVERIFY(set.isEmpty());
}


void TestSet::testContainsMethod() {
    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<> uniform(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    M::Set set;

    QMap<Model::Integer, Model::Integer> values; // We use a map here because it's deterministic.
    for (unsigned i=0 ; i<numberInsertedValues ; ++i) {
        Model::Integer value;
        do {
            value = uniform(rng);
        } while (values.contains(value));

        values.insert(value, value);
        bool success = set.insert(value);
        QVERIFY(success);
    }

    unsigned i=0;
    for (  QMap<Model::Integer, Model::Integer>::const_iterator it = values.constBegin(),end=values.constEnd()
         ; it!=end
         ; ++it
        ) {
        Model::Integer value;
        do {
            value = uniform(rng);
        } while (values.contains(value));

        bool inSet = set.contains(value);
        QVERIFY(!inSet);

        value = it.key();

        inSet = set.contains(value);
        if (!inSet) {
            qDebug() << i;
            qDebug() << value;
        }

        QVERIFY(inSet);
        ++i;
    }
}


void TestSet::testUniteWith() {
    M::Set s1;
    M::Set s2;

    QSet<Model::Integer> sv1;
    QSet<Model::Integer> sv2;

    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<> uniform(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    for (unsigned i=0 ; i<numberInsertedValues ; ++i) {
        Model::Integer v1 = uniform(rng);
        Model::Integer v2;
        if (uniform(rng) > 0) {
            v2 = v1;
        } else {
            v2 = uniform(rng);
        }

        s1.insert(v1);
        s2.insert(v2);

        sv1.insert(v1);
        sv2.insert(v2);
    }

    s1.uniteWith(s2);
    sv1.unite(sv2);

    QCOMPARE(s1.size(), static_cast<unsigned>(sv1.size()));

    for (Model::Integer v : sv1) {
        bool inSet = s1.contains(v);
        QVERIFY(inSet);
    }
}


void TestSet::testUnitedWith() {
    M::Set s1;
    M::Set s2;

    QSet<Model::Integer> sv1;
    QSet<Model::Integer> sv2;

    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<> uniform(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    for (unsigned i=0 ; i<numberInsertedValues ; ++i) {
        Model::Integer v1 = uniform(rng);
        Model::Integer v2;
        if (uniform(rng) > 0) {
            v2 = v1;
        } else {
            v2 = uniform(rng);
        }

        s1.insert(v1);
        s2.insert(v2);

        sv1.insert(v1);
        sv2.insert(v2);
    }

    M::Set               s3  = s1.unitedWith(s2);
    QSet<Model::Integer> sv3 = sv1 | sv2;

    QCOMPARE(s3.size(), static_cast<unsigned>(sv3.size()));

    for (Model::Integer v : sv3) {
        bool inSet = s3.contains(v);
        QVERIFY(inSet);
    }
}


void TestSet::testIntersectWith() {
    M::Set s1;
    M::Set s2;

    QSet<Model::Integer> sv1;
    QSet<Model::Integer> sv2;

    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<> uniform(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    for (unsigned i=0 ; i<numberInsertedValues ; ++i) {
        Model::Integer v1 = uniform(rng);
        Model::Integer v2;
        if (uniform(rng) > 0) {
            v2 = v1;
        } else {
            v2 = uniform(rng);
        }

        s1.insert(v1);
        s2.insert(v2);

        sv1.insert(v1);
        sv2.insert(v2);
    }

    s1.intersectWith(s2);
    sv1.intersect(sv2);

    QCOMPARE(s1.size(), static_cast<unsigned>(sv1.size()));

    for (Model::Integer v : sv1) {
        bool inSet = s1.contains(v);
        QVERIFY(inSet);
    }
}


void TestSet::testIntersectedWith() {
    M::Set s1;
    M::Set s2;

    QSet<Model::Integer> sv1;
    QSet<Model::Integer> sv2;

    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<> uniform(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    for (unsigned i=0 ; i<numberInsertedValues ; ++i) {
        Model::Integer v1 = uniform(rng);
        Model::Integer v2;
        if (uniform(rng) > 0) {
            v2 = v1;
        } else {
            v2 = uniform(rng);
        }

        s1.insert(v1);
        s2.insert(v2);

        sv1.insert(v1);
        sv2.insert(v2);
    }

    M::Set               s3  = s1.intersectedWith(s2);
    QSet<Model::Integer> sv3 = sv1 & sv2;

    QCOMPARE(s3.size(), static_cast<unsigned>(sv3.size()));

    for (Model::Integer v : sv3) {
        bool inSet = s3.contains(v);
        QVERIFY(inSet);
    }
}


void TestSet::testDifference() {
    M::Set s1;
    M::Set s2;

    QSet<Model::Integer> sv1;
    QSet<Model::Integer> sv2;

    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<> uniform(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    for (unsigned i=0 ; i<numberInsertedValues ; ++i) {
        Model::Integer v1 = uniform(rng);
        Model::Integer v2;
        if (uniform(rng) > 0) {
            v2 = v1;
        } else {
            v2 = uniform(rng);
        }

        s1.insert(v1);
        s2.insert(v2);

        sv1.insert(v1);
        sv2.insert(v2);
    }

    M::Set               s3  = s1 - s2;
    QSet<Model::Integer> sv3 = sv1 - sv2;

    QCOMPARE(s3.size(), static_cast<unsigned>(sv3.size()));

    for (Model::Integer v : sv3) {
        bool inSet = s3.contains(v);
        QVERIFY(inSet);
    }
}


void TestSet::testModelSetIteratorBasicUsage() {
    Model::Set           s1;
    QSet<Model::Integer> sv1;

    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<> uniform(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    for (unsigned i=0 ; i<numberInsertedValues ; ++i) {
        Model::Integer v1 = uniform(rng);
        s1.insert(v1);
        sv1.insert(v1);
    }

    unsigned       count         = 0;
    Model::Integer previousValue = 0;
    for (Model::Set::Iterator it=s1.begin(), end=s1.end() ; it!=end ; ++it) {
        Model::Integer value = *it;
        QVERIFY(sv1.contains(value));

        if (count != 0) {
            QVERIFY(value > previousValue);
        }

        previousValue = value;
        ++count;
    }

    QCOMPARE(count, static_cast<unsigned>(sv1.size()));
}


void TestSet::testFind() {
    M::Set               s1;
    Model::Set           s2;
    QSet<Model::Integer> sv1;

    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<> uniform(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    for (unsigned i=0 ; i<numberFuzzIterations; ++i) {
        M::Integer v1 = uniform(rng);

        s1.insert(v1);
        s2.insert(v1);
        sv1.insert(v1);
    }

    for (QSet<M::Integer>::const_iterator it=sv1.constBegin(),end=sv1.constEnd() ; it!=end ; ++it) {
        M::Variant v(*it);
        M::Set::ConstIterator ci = s1.find(v);
        QCOMPARE(ci != s1.constEnd(), true);
        QCOMPARE(*ci, v);
    }

    for (unsigned i=0 ; i<numberFuzzIterations ; ++i) {
        M::Integer v1;
        do {
            v1 = uniform(rng);
        } while (sv1.contains(v1));

        M::Variant v(v1);
        QCOMPARE(s1.find(v), s1.constEnd());
    }

    for (QSet<M::Integer>::const_iterator it=sv1.constBegin(),end=sv1.constEnd() ; it!=end ; ++it) {
        Model::Variant v(*it);
        Model::Set::ConstIterator ci = s2.find(v);
        QCOMPARE(ci != s2.constEnd(), true);
        QCOMPARE(*ci, v);
    }

    for (unsigned i=0 ; i<numberFuzzIterations ; ++i) {
        Model::Integer v1;
        do {
            v1 = uniform(rng);
        } while (sv1.contains(v1));

        Model::Variant v(v1);
        QCOMPARE(s2.find(v), s2.constEnd());
    }
}


void TestSet::testModelSetIteratorDuringModification() {
    Model::Set           s1;
    QSet<Model::Integer> sv1;

    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<> uniform(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    for (unsigned i=0 ; i<numberFuzzIterations ; ++i) {
        Model::Integer v1 = uniform(rng);
        s1.insert(v1);
        sv1.insert(v1);
    }

    Model::Integer previousValue = 0;
    bool           firstPass     = true;
    for (Model::Set::Iterator it=s1.begin(), end=s1.end() ; it!=end ; ++it) {
        Model::Integer value = *it;
        QVERIFY(sv1.contains(value));

        if (!firstPass) {
            QVERIFY(value > previousValue);
        } else {
            firstPass = false;
        }

        previousValue = value;

        Model::Integer newValue = uniform(rng);
        s1.insert(newValue);
        sv1.insert(newValue);

        Model::Integer valueRepeat = it->toInteger();
        QCOMPARE(value, valueRepeat);
    }
}


void TestSet::testModelSetIteratorDuringAssignment() {
    Model::Set s1;
    Model::Set s2;

    for (unsigned i=1 ; i<10 ; ++i) {
        s1.insert(Model::Integer(9 * i));
        s2.insert(Model::Integer(15 * i));
    }

    Model::Set::Iterator it = s1.find(Model::Integer(36));

    QCOMPARE(*it, Model::Variant(Model::Integer(36)));
    ++it;
    QCOMPARE(*it, Model::Variant(Model::Integer(45)));

    s1 = s2;

    QCOMPARE(*it, Model::Variant(Model::Integer(45)));
    ++it;
    QCOMPARE(*it, Model::Variant(Model::Integer(60)));
}


void TestSet::testSetOfSets() {
    Model::Set container;
    Model::Set s1; // One value
    Model::Set s2; // Two values
    Model::Set s3; // Three values
    Model::Set s4; // empty set
    Model::Set s5; // Three values, same as s3
    Model::Set s6; // Three values, same as s3, populated independently.
    Model::Set s7; // Three values, different from s3.

    s1.insert(Model::Integer(1));

    s2.insert(Model::Integer(1));
    s2.insert(Model::Integer(2));

    s3.insert(Model::Integer(1));
    s3.insert(Model::Integer(2));
    s3.insert(Model::Integer(3));

    s5 = s3; // Note will have same impl as s3

    s6.insert(Model::Integer(1));
    s6.insert(Model::Integer(2));
    s6.insert(Model::Integer(3));

    s7.insert(Model::Integer(1));
    s7.insert(Model::Integer(2));
    s7.insert(Model::Integer(4));

    container.insert(Model::Integer(1));

    container.insert(s1);
    container.insert(s2);
    container.insert(s3);
    container.insert(s4);
    container.insert(s5);
    container.insert(s6);
    container.insert(s7);

    QCOMPARE(container.size(), 6);

    Model::Set::Iterator it = container.begin();

    Model::Variant v1 = *it++;
    Model::Variant v2 = *it++;
    Model::Variant v3 = *it++;
    Model::Variant v4 = *it++;
    Model::Variant v5 = *it++;
    Model::Variant v6 = *it++;

    QCOMPARE(it.isValid(), false);
    QCOMPARE(it, container.end());

    QCOMPARE(v1.valueType(), Model::ValueType::INTEGER);
    QCOMPARE(v1.toInteger(), 1);

    QCOMPARE(v2.valueType(), Model::ValueType::SET);
    QCOMPARE(v3.valueType(), Model::ValueType::SET);
    QCOMPARE(v4.valueType(), Model::ValueType::SET);
    QCOMPARE(v5.valueType(), Model::ValueType::SET);
    QCOMPARE(v6.valueType(), Model::ValueType::SET);

    QCOMPARE(v2.toSet(), s4);
    QCOMPARE(v3.toSet(), s1);
    QCOMPARE(v4.toSet(), s2);
    QCOMPARE(v5.toSet(), s6);
    QCOMPARE(v6.toSet(), s7);
}
