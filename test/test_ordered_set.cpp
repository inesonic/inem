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
* This file implements tests of the \ref M::OrderedSet class.
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

#include <m_ordered_set.h>

#include "test_ordered_set.h"

TestOrderedSet::TestOrderedSet() {}


TestOrderedSet::~TestOrderedSet() {}


void TestOrderedSet::testClearMethod() {

}


void TestOrderedSet::testReserveMethod() {

}


void TestOrderedSet::testInsertMethod() {
    M::OrderedSet<M::Integer> set;

    int initialValue = 1;
    QList<int> collisionValues;
    collisionValues << initialValue;

    M::UnorderedSetBase::Index
        target = static_cast<M::UnorderedSetBase::Index>(M::hashFunction(initialValue, set.seed())) % set.tableSize();

    int nextValue = initialValue;
    while (collisionValues.size() < 32) {
        ++nextValue;

        M::UnorderedSetBase::Index
            hash = static_cast<M::UnorderedSetBase::Index>(M::hashFunction(nextValue, set.seed())) % set.tableSize();

        if (hash == target) {
            collisionValues << nextValue;
        }
    }

    for (int value : collisionValues) {
        bool success = set.insert(value);
        QVERIFY(success);

        unsigned minimumDepth;
        unsigned maximumDepth;

        set.calculateTreeDepths(minimumDepth, maximumDepth);
        QVERIFY(minimumDepth * 2 <= maximumDepth || minimumDepth <= maximumDepth + 1);
    }

    QCOMPARE(set.size(), static_cast<unsigned>(collisionValues.size()));
}


void TestOrderedSet::testRemoveMethod() {
    std::mt19937 rng(0x1234);
    std::uniform_int_distribution<> uniform(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    M::OrderedSet<M::Integer> set;

    QMap<int, int> values;
    for (unsigned i=0 ; i<numberInsertedValues ; ++i) {
        int value;
        do {
            value = uniform(rng);
        } while (values.contains(value));

        values.insert(value, value);
        bool success = set.insert(value);
        QVERIFY(success);

        unsigned minimumDepth;
        unsigned maximumDepth;

        set.calculateTreeDepths(minimumDepth, maximumDepth);
        QVERIFY(minimumDepth * 2 <= maximumDepth || minimumDepth <= maximumDepth + 1);
    }

    do {
        int  value;
        bool knownValue = (uniform(rng) >= 0);
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

        QCOMPARE(set.size(), static_cast<unsigned long>(values.size()));

        unsigned minimumDepth;
        unsigned maximumDepth;

        set.calculateTreeDepths(minimumDepth, maximumDepth);
        QVERIFY(minimumDepth * 2 <= maximumDepth || minimumDepth <= maximumDepth + 1);
    } while (!values.isEmpty());

    QVERIFY(set.isEmpty());
}


void TestOrderedSet::testContainsMethod() {
    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<> uniform(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    M::OrderedSet<M::Integer> set;

    QMap<int, int> values; // We use a map here because it's deterministic.
    for (unsigned i=0 ; i<numberInsertedValues ; ++i) {
        int value;
        do {
            value = uniform(rng);
        } while (values.contains(value));

        values.insert(value, value);
        bool success = set.insert(value);
        QVERIFY(success);

        unsigned minimumDepth;
        unsigned maximumDepth;

        set.calculateTreeDepths(minimumDepth, maximumDepth);
        QVERIFY(minimumDepth * 2 <= maximumDepth || minimumDepth <= maximumDepth + 1);
    }

    unsigned i=0;
    for (  QMap<int, int>::const_iterator it=values.constBegin(),end=values.constEnd() ; it!=end ; ++it) {
        int value;
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


void TestOrderedSet::testUniteWith() {
    M::OrderedSet<M::Integer> s1;
    M::OrderedSet<M::Integer> s2;

    QSet<int> sv1;
    QSet<int> sv2;

    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<> uniform(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    for (unsigned i=0 ; i<numberInsertedValues ; ++i) {
        int v1 = uniform(rng);
        int v2;
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

    for (int v : sv1) {
        bool inSet = s1.contains(v);
        QVERIFY(inSet);
    }

    unsigned minimumDepth;
    unsigned maximumDepth;

    s1.calculateTreeDepths(minimumDepth, maximumDepth);
    QVERIFY(minimumDepth * 2 <= maximumDepth || minimumDepth <= maximumDepth + 1);
}


void TestOrderedSet::testUnitedWith() {
    M::OrderedSet<M::Integer> s1;
    M::OrderedSet<M::Integer> s2;

    QSet<int> sv1;
    QSet<int> sv2;

    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<> uniform(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    for (unsigned i=0 ; i<numberInsertedValues ; ++i) {
        int v1 = uniform(rng);
        int v2;
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

    M::OrderedSet<M::Integer> s3 = s1.unitedWith(s2);
    QSet<int> sv3 = sv1 | sv2;

    QCOMPARE(s3.size(), static_cast<unsigned>(sv3.size()));

    for (int v : sv3) {
        bool inSet = s3.contains(v);
        QVERIFY(inSet);
    }

    unsigned minimumDepth;
    unsigned maximumDepth;

    s3.calculateTreeDepths(minimumDepth, maximumDepth);
    QVERIFY(minimumDepth * 2 <= maximumDepth || minimumDepth <= maximumDepth + 1);
}


void TestOrderedSet::testIntersectWith() {
    M::OrderedSet<M::Integer> s1;
    M::OrderedSet<M::Integer> s2;

    QSet<int> sv1;
    QSet<int> sv2;

    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<> uniform(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    for (unsigned i=0 ; i<numberInsertedValues ; ++i) {
        int v1 = uniform(rng);
        int v2;
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

    for (int v : sv1) {
        bool inSet = s1.contains(v);
        QVERIFY(inSet);
    }

    unsigned minimumDepth;
    unsigned maximumDepth;

    s1.calculateTreeDepths(minimumDepth, maximumDepth);
    QVERIFY(minimumDepth * 2 <= maximumDepth || minimumDepth <= maximumDepth + 1);
}


void TestOrderedSet::testIntersectedWith() {
    M::OrderedSet<M::Integer> s1;
    M::OrderedSet<M::Integer> s2;

    QSet<int> sv1;
    QSet<int> sv2;

    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<> uniform(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    for (unsigned i=0 ; i<numberInsertedValues ; ++i) {
        int v1 = uniform(rng);
        int v2;
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

    M::OrderedSet<M::Integer> s3 = s1.intersectedWith(s2);
    QSet<int> sv3 = sv1 & sv2;

    QCOMPARE(s3.size(), static_cast<unsigned>(sv3.size()));

    for (int v : sv3) {
        bool inSet = s3.contains(v);
        QVERIFY(inSet);
    }

    unsigned minimumDepth;
    unsigned maximumDepth;

    s3.calculateTreeDepths(minimumDepth, maximumDepth);
    QVERIFY(minimumDepth * 2 <= maximumDepth || minimumDepth <= maximumDepth + 1);
}


void TestOrderedSet::testDifference() {
    M::OrderedSet<M::Integer> s1;
    M::OrderedSet<M::Integer> s2;

    QSet<int> sv1;
    QSet<int> sv2;

    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<> uniform(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    for (unsigned i=0 ; i<numberInsertedValues ; ++i) {
        int v1 = uniform(rng);
        int v2;
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

    M::OrderedSet<M::Integer> s3 = s1 - s2;
    QSet<int> sv3 = sv1 - sv2;

    QCOMPARE(s3.size(), static_cast<unsigned>(sv3.size()));

    for (int v : sv3) {
        bool inSet = s3.contains(v);
        QVERIFY(inSet);
    }

    unsigned minimumDepth;
    unsigned maximumDepth;

    s3.calculateTreeDepths(minimumDepth, maximumDepth);
    QVERIFY(minimumDepth * 2 <= maximumDepth || minimumDepth <= maximumDepth + 1);
}


void TestOrderedSet::testTreeDepthConstraint() {
    M::OrderedSet<M::Integer> s;
    QSet<int> sv;

    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<> uniform(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    for (unsigned index=0 ; index<numberFuzzIterations ; ++index) {
        int value;

        if (uniform(rng) > 0 && uniform(rng) > 0 && !sv.isEmpty()) {
            // Ocassionally remove values
            int value = *sv.begin();

            s.remove(value);
            sv.remove(value);
        } else {
            do {
                value = uniform(rng);
            } while (sv.contains(value));

            s.insert(value);
            sv.insert(value);
        }

        unsigned minimumDepth;
        unsigned maximumDepth;

        s.calculateTreeDepths(minimumDepth, maximumDepth);
        QVERIFY(minimumDepth * 2 <= maximumDepth || minimumDepth <= maximumDepth + 1);
    }
}


void TestOrderedSet::testIteratorBasicUsage() {
    M::OrderedSet<M::Integer> s1;
    QSet<int> sv1;

    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<> uniform(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    for (unsigned i=0 ; i<numberInsertedValues ; ++i) {
        int v1 = uniform(rng);
        s1.insert(v1);
        sv1.insert(v1);
    }

    unsigned count         = 0;
    int      previousValue = 0;
    for (M::OrderedSet<M::Integer>::Iterator it=s1.begin(), end=s1.end() ; it!=end ; ++it) {
        int value = *it;
        QVERIFY(sv1.contains(value));

        if (count != 0) {
            QVERIFY(value > previousValue);
        }

        previousValue = value;
        ++count;
    }

    QCOMPARE(count, static_cast<unsigned>(sv1.size()));
}


void TestOrderedSet::testIteratorDuringModification() {
    M::OrderedSet<M::Integer> s1;
    QSet<int> sv1;

    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<> uniform(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    for (unsigned i=0 ; i<numberFuzzIterations ; ++i) {
        int v1 = uniform(rng);
        s1.insert(v1);
        sv1.insert(v1);
    }

    int      previousValue = 0;
    bool     firstPass     = true;
    for (M::OrderedSet<M::Integer>::Iterator it=s1.begin(), end=s1.end() ; it!=end ; ++it) {
        int value = *it;
        QVERIFY(sv1.contains(value));

        if (!firstPass) {
            QVERIFY(value > previousValue);
        } else {
            firstPass = false;
        }

        previousValue = value;

        int newValue = uniform(rng);
        s1.insert(newValue);
        sv1.insert(newValue);

        int valueRepeat = *it;
        QCOMPARE(value, valueRepeat);
    }
}


