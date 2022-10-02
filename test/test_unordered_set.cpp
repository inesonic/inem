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
* This file implements tests of the \ref M::UnorderedSet class.
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

#include <m_unordered_set.h>

#include "test_unordered_set.h"

/***********************************************************************************************************************
 * SetElement
 */

class SetElement {
    public:
        SetElement();

        SetElement(int value);

        SetElement(const SetElement& other);

        ~SetElement();

        void setValue(int newValue);

        int value() const;

        static void clear();

        static unsigned numberConstructorCalls();

        static unsigned numberDestructorCalls();

        SetElement& operator=(const SetElement&);

    private:
        static unsigned currentNumberConstructorCalls;
        static unsigned currentNumberDestructorCalls;

        int currentValue;
};

unsigned SetElement::currentNumberConstructorCalls;
unsigned SetElement::currentNumberDestructorCalls;

SetElement::SetElement() {
    ++currentNumberConstructorCalls;
}


SetElement::SetElement(int value) {
    currentValue = value;
}


SetElement::SetElement(const SetElement& other) {
    currentValue = other.currentValue;
}


SetElement::~SetElement() {
    ++currentNumberDestructorCalls;
}


void SetElement::setValue(int newValue) {
    currentValue = newValue;
}


int SetElement::value() const {
    return currentValue;
}


void SetElement::clear() {
    currentNumberConstructorCalls = 0;
    currentNumberDestructorCalls  = 0;
}


unsigned SetElement::numberConstructorCalls() {
    return currentNumberConstructorCalls;
}


unsigned SetElement::numberDestructorCalls() {
    return currentNumberDestructorCalls;
}


SetElement& SetElement::operator=(const SetElement& other) {
    currentValue = other.currentValue;
    return *this;
}

/***********************************************************************************************************************
 * TestUnorderedSet
 */

TestUnorderedSet::TestUnorderedSet() {}


TestUnorderedSet::~TestUnorderedSet() {}


void TestUnorderedSet::testCalculateNextPrime() {
    M::UnorderedSetBase::Index        v = 1;
    QList<M::UnorderedSetBase::Index> primes;

    v = M::UnorderedSetBase::calculateNextPrime(v);
    QCOMPARE(v, 2U);
    primes.append(v);

    while (v < 100000) {
        v = M::UnorderedSetBase::calculateNextPrime(v);

        for (M::UnorderedSetBase::Index prime : primes) {
            QVERIFY(v % prime != 0);
        }

        primes.append(v);
    }
}


void TestUnorderedSet::testClearMethod() {

}


void TestUnorderedSet::testReserveMethod() {

}


void TestUnorderedSet::testInsertMethod() {
    M::UnorderedSet<int> set;

    int initialValue = 1;
    QList<int> collisionValues;
    collisionValues << initialValue;

    M::UnorderedSetBase::Index
        target = static_cast<M::UnorderedSetBase::Index>(M::hashFunction(initialValue, set.seed()) % set.tableSize());

    int nextValue = initialValue;
    while (collisionValues.size() < 32) {
        ++nextValue;

        M::UnorderedSetBase::Index
            hash = static_cast<M::UnorderedSetBase::Index>(M::hashFunction(nextValue, set.seed()) % set.tableSize());

        if (hash == target) {
            collisionValues << nextValue;
        }
    }

    for (int value : collisionValues) {
        bool success = set.insert(value);
        QVERIFY(success);
    }

    QCOMPARE(set.size(), static_cast<unsigned>(collisionValues.size()));
}


void TestUnorderedSet::testRemoveMethod() {
    std::mt19937 rng(0x1234);
    std::uniform_int_distribution<> uniform(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    M::UnorderedSet<int> set;

    QMap<int, int> values;
    for (unsigned i=0 ; i<numberInsertedValues ; ++i) {
        int value;
        do {
            value = uniform(rng);
        } while (values.contains(value));

        values.insert(value, value);
        bool success = set.insert(value);
        QVERIFY(success);
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
    } while (!values.isEmpty());

    QVERIFY(set.isEmpty());
}


void TestUnorderedSet::testContainsMethod() {
    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<> uniform(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    M::UnorderedSet<int> set;

    QMap<int, int> values; // We use a map here because it's deterministic.
    for (unsigned i=0 ; i<numberInsertedValues ; ++i) {
        int value;
        do {
            value = uniform(rng);
        } while (values.contains(value));

        values.insert(value, value);
        bool success = set.insert(value);
        QVERIFY(success);
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


void TestUnorderedSet::testUniteWith() {
    M::UnorderedSet<int> s1;
    M::UnorderedSet<int> s2;

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
}


void TestUnorderedSet::testUnitedWith() {
    M::UnorderedSet<int> s1;
    M::UnorderedSet<int> s2;

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

    M::UnorderedSet<int> s3 = s1.unitedWith(s2);
    QSet<int> sv3 = sv1 | sv2;

    QCOMPARE(s3.size(), static_cast<unsigned>(sv3.size()));

    for (int v : sv3) {
        bool inSet = s3.contains(v);
        QVERIFY(inSet);
    }
}


void TestUnorderedSet::testIntersectWith() {
    M::UnorderedSet<int> s1;
    M::UnorderedSet<int> s2;

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
}


void TestUnorderedSet::testIntersectedWith() {
    M::UnorderedSet<int> s1;
    M::UnorderedSet<int> s2;

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

    M::UnorderedSet<int> s3 = s1.intersectedWith(s2);
    QSet<int> sv3 = sv1 & sv2;

    QCOMPARE(s3.size(), static_cast<unsigned>(sv3.size()));

    for (int v : sv3) {
        bool inSet = s3.contains(v);
        QVERIFY(inSet);
    }
}

void TestUnorderedSet::testDifference() {
    M::UnorderedSet<int> s1;
    M::UnorderedSet<int> s2;

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

    M::UnorderedSet<int> s3 = s1 - s2;
    QSet<int> sv3 = sv1 - sv2;

    QCOMPARE(s3.size(), static_cast<unsigned>(sv3.size()));

    for (int v : sv3) {
        bool inSet = s3.contains(v);
        QVERIFY(inSet);
    }
}


void TestUnorderedSet::testIterator() {
    M::UnorderedSet<int> s1;
    QSet<int> sv1;

    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<> uniform(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    for (unsigned i=0 ; i<numberInsertedValues ; ++i) {
        int v1 = uniform(rng);
        s1.insert(v1);
        sv1.insert(v1);
    }

    unsigned count = 0;
    for (M::UnorderedSet<int>::Iterator it=s1.begin(), end=s1.end() ; it!=end ; ++it) {
        int value = *it;
        QVERIFY(sv1.contains(value));
        ++count;
    }

    QCOMPARE(count, static_cast<unsigned>(sv1.size()));
}

