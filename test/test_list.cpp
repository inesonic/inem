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
* This file implements tests of the \ref M::List class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QObject>
#include <QList>
#include <QtTest/QtTest>

#include <m_list.h>

#include "test_list.h"

/***********************************************************************************************************************
 * ListElement
 */

class ListElement {
    public:
        ListElement();

        ListElement(int value);

        ListElement(const ListElement& other);

        ~ListElement();

        void setValue(int newValue);

        int value() const;

        static void clear();

        static unsigned numberConstructorCalls();

        static unsigned numberDestructorCalls();

        ListElement& operator=(const ListElement&);

    private:
        static unsigned currentNumberConstructorCalls;
        static unsigned currentNumberDestructorCalls;

        int currentValue;
};

unsigned ListElement::currentNumberConstructorCalls;
unsigned ListElement::currentNumberDestructorCalls;

ListElement::ListElement() {
    ++currentNumberConstructorCalls;
}


ListElement::ListElement(int value) {
    currentValue = value;
}


ListElement::ListElement(const ListElement& other) {
    currentValue = other.currentValue;
}


ListElement::~ListElement() {
    ++currentNumberDestructorCalls;
}


void ListElement::setValue(int newValue) {
    currentValue = newValue;
}


int ListElement::value() const {
    return currentValue;
}


void ListElement::clear() {
    currentNumberConstructorCalls = 0;
    currentNumberDestructorCalls  = 0;
}


unsigned ListElement::numberConstructorCalls() {
    return currentNumberConstructorCalls;
}


unsigned ListElement::numberDestructorCalls() {
    return currentNumberDestructorCalls;
}


ListElement& ListElement::operator=(const ListElement& other) {
    currentValue = other.currentValue;
    return *this;
}

/***********************************************************************************************************************
 * TestList
 */

TestList::TestList() {}


TestList::~TestList() {}


void TestList::testConstructorAndDestructors() {
    ListElement::clear();

    {
        M::List<ListElement> list1;
        QCOMPARE(list1.isEmpty(), true);
        QCOMPARE(list1.size(), 0U);
        QCOMPARE(list1.capacity(), 1U);
    }

    QCOMPARE(ListElement::numberConstructorCalls(), 1U);
    QCOMPARE(ListElement::numberDestructorCalls(), 1U);

    ListElement::clear();

    {
        M::List<ListElement> list2(1024);
        QCOMPARE(list2.isEmpty(), false);
        QCOMPARE(list2.size(), 1024U);
        QCOMPARE(list2.capacity(), 1024U);
    }

    QCOMPARE(ListElement::numberConstructorCalls(), 1024U);
    QCOMPARE(ListElement::numberDestructorCalls(), 1024U);

    ListElement::clear();

    {
        M::List<ListElement> list3(1024, ListElement(5));
        QCOMPARE(list3.isEmpty(), false);
        QCOMPARE(list3.size(), 1024U);
        QCOMPARE(list3.capacity(), 1024U);

        M::List<ListElement> list4 = list3;
        QCOMPARE(list4.isEmpty(), false);
        QCOMPARE(list4.size(), 1024U);
        QCOMPARE(list4.capacity(), 1024U);
    }

    QCOMPARE(ListElement::numberConstructorCalls(), 2 * 1024U);
    QCOMPARE(ListElement::numberDestructorCalls(), 2 * 1024U + 1);
}


void TestList::testStatusMethods() {
    M::List<int> list1;

    QCOMPARE(list1.isEmpty(), true);
    QCOMPARE(list1.isNotEmpty(), false);
    QCOMPARE(list1.size(), 0U);
    QCOMPARE(list1.capacity(), 1U);

    list1.append(5);

    QCOMPARE(list1.isEmpty(), false);
    QCOMPARE(list1.isNotEmpty(), true);
    QCOMPARE(list1.size(), 1U);
    QCOMPARE(list1.capacity(), 1U);

    list1.append(6);

    QCOMPARE(list1.isEmpty(), false);
    QCOMPARE(list1.isNotEmpty(), true);
    QCOMPARE(list1.size(), 2U);
    QCOMPARE(list1.capacity(), 2U);

    list1.append(7);

    QCOMPARE(list1.isEmpty(), false);
    QCOMPARE(list1.isNotEmpty(), true);
    QCOMPARE(list1.size(), 3U);
    QCOMPARE(list1.capacity(), 4U);
}


void TestList::testClearReserveResizeOptimizeMethods() {
    M::List<int> list;
    list.append(1);
    list.append(2);
    list.append(3);
    list.append(4);
    list.append(5);

    QCOMPARE(list.size(), 5U);
    QCOMPARE(list.capacity(), 8U);

    list.optimize();
    QCOMPARE(list.size(), 5U);
    QCOMPARE(list.capacity(), 5U);

    list.reserve(16);
    QCOMPARE(list.size(), 5U);
    QCOMPARE(list.capacity(), 16U);

    list.resize(25);
    QCOMPARE(list.size(), 25U);
    QCOMPARE(list.capacity(), 25U);
}


void TestList::testAccessorMethods() {
    M::List<int> list;
    list.append(1);
    list.append(2);
    list.append(3);
    list.append(4);
    list.append(5);

    QCOMPARE(list.at(0), 1);
    QCOMPARE(list.at(1), 2);
    QCOMPARE(list.at(2), 3);
    QCOMPARE(list.at(3), 4);
    QCOMPARE(list.at(4), 5);

    QCOMPARE(list[0], 1);
    QCOMPARE(list[1], 2);
    QCOMPARE(list[2], 3);
    QCOMPARE(list[3], 4);
    QCOMPARE(list[4], 5);

    QCOMPARE(list.first(), 1);
    QCOMPARE(list.last(), 5);

    list[0] = 6;
    list[1] = 5;
    list[2] = 4;
    list[3] = 3;
    list[4] = 2;

    QCOMPARE(list.at(0), 6);
    QCOMPARE(list.at(1), 5);
    QCOMPARE(list.at(2), 4);
    QCOMPARE(list.at(3), 3);
    QCOMPARE(list.at(4), 2);
}


void TestList::testAppendTakeMethods() {
    M::List<int> list;
    list.append(1);
    list.append(2);
    list.append(3);
    list.append(4);
    list.append(5);

    QCOMPARE(list.takeLast(), 5);
    QCOMPARE(list.takeLast(), 4);
    QCOMPARE(list.takeLast(), 3);
    QCOMPARE(list.takeLast(), 2);
    QCOMPARE(list.takeLast(), 1);

    QCOMPARE(list.isEmpty(), true);
}


void TestList::testAssignmentOperators() {
    M::List<int> list1;
    list1.append(1);
    list1.append(2);
    list1.append(3);
    list1.append(4);
    list1.append(5);

    M::List<int> list2;
    list2 = list1;

    QCOMPARE(list2.at(0), 1);
    QCOMPARE(list2.at(1), 2);
    QCOMPARE(list2.at(2), 3);
    QCOMPARE(list2.at(3), 4);
    QCOMPARE(list2.at(4), 5);

    list1[0] = 6;
    list1[1] = 5;
    list1[2] = 4;
    list1[3] = 3;
    list1[4] = 2;

    QCOMPARE(list2.at(0), 1);
    QCOMPARE(list2.at(1), 2);
    QCOMPARE(list2.at(2), 3);
    QCOMPARE(list2.at(3), 4);
    QCOMPARE(list2.at(4), 5);
}


void TestList::testConstIterator() {
    M::List<int> list1;
    list1.append(1);
    list1.append(2);
    list1.append(3);
    list1.append(4);
    list1.append(5);

    M::List<int>::ConstIterator iterator = list1.constBegin();

    QCOMPARE(*iterator, 1);
    ++iterator;
    QCOMPARE(iterator != list1.constEnd(), true);

    QCOMPARE(*iterator++, 2);
    QCOMPARE(iterator != list1.constEnd(), true);

    QCOMPARE(*iterator++, 3);
    QCOMPARE(iterator != list1.constEnd(), true);

    QCOMPARE(*iterator++, 4);
    QCOMPARE(iterator != list1.constEnd(), true);

    QCOMPARE(*iterator++, 5);
    QCOMPARE(iterator == list1.constEnd(), true);

    --iterator;
    QCOMPARE(*iterator--, 5);
    QCOMPARE(*iterator--, 4);
    QCOMPARE(*iterator--, 3);
    QCOMPARE(*iterator--, 2);
    QCOMPARE(*iterator,   1);
}


void TestList::testIterator() {
    M::List<int> list1;
    list1.append(1);
    list1.append(2);
    list1.append(3);
    list1.append(4);
    list1.append(5);

    M::List<int>::Iterator iterator = list1.begin();

    QCOMPARE(*iterator, 1);
    ++iterator;
    QCOMPARE(iterator != list1.end(), true);

    QCOMPARE(*iterator++, 2);
    QCOMPARE(iterator != list1.end(), true);

    QCOMPARE(*iterator++, 3);
    QCOMPARE(iterator != list1.end(), true);

    QCOMPARE(*iterator++, 4);
    QCOMPARE(iterator != list1.end(), true);

    QCOMPARE(*iterator++, 5);
    QCOMPARE(iterator == list1.end(), true);

    --iterator;
    QCOMPARE(*iterator--, 5);
    QCOMPARE(*iterator--, 4);
    QCOMPARE(*iterator--, 3);
    QCOMPARE(*iterator--, 2);
    QCOMPARE(*iterator,   1);
}
