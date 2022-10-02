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
* This file implements tests of the set functions.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QObject>
#include <QList>
#include <QtTest/QtTest>

#include <limits>

#include <m_intrinsic_types.h>
#include <m_set.h>
#include <m_tuple.h>
#include <m_matrix_boolean.h>
#include <m_matrix_integer.h>
#include <m_matrix_real.h>
#include <m_matrix_complex.h>
#include <m_variant.h>
#include <m_set_functions.h>

#include "test_set_functions.h"

TestSetFunctions::TestSetFunctions() {}


TestSetFunctions::~TestSetFunctions() {}


void TestSetFunctions::testAbs() {
    for (unsigned elementCount=0 ; elementCount < 100 ; ++elementCount) {
        M::Set set;

        for (unsigned i=0 ; i<elementCount ; ++i) {
            set.insert(static_cast<M::Integer>(i));
        }

        QCOMPARE(M::abs(set), M::Integer(elementCount));
    }
}


void TestSetFunctions::testBinaryUnionOf() {
    M::Set s1 = M::Set::build(M::Integer(1), M::Integer(2), M::Integer(3));
    M::Set s2 = M::Set::build(M::Integer(2), M::Integer(4));

    M::Set u  = M::unionOf(s1, s2);
    M::Set v = M::Set::build(M::Integer(1), M::Integer(2), M::Integer(3), M::Integer(4));
    QCOMPARE(u, v);
    QCOMPARE(M::unionOf(M::Variant(s1), s2), v);
    QCOMPARE(M::unionOf(s1, M::Variant(s2)), v);
    QCOMPARE(M::unionOf(M::Variant(s1), M::Variant(s2)), v);
}


void TestSetFunctions::testNaryUnionOf() {
    M::Set s1 = M::Set::build(M::Integer(1), M::Integer(2), M::Integer(3));
    M::Set s2 = M::Set::build(M::Integer(2), M::Integer(4));
    M::Set s3 = M::Set::build(M::Integer(4), M::Integer(6));

    M::Set u1 = M::unionOf(M::Set::build(s1, s2, s3));
    QCOMPARE(u1, M::Set::build(M::Integer(1), M::Integer(2), M::Integer(3), M::Integer(4), M::Integer(6)));

    M::Set u2 = M::unionOf(M::Tuple::build(s1, s2, s3));
    QCOMPARE(u2, M::Set::build(M::Integer(1), M::Integer(2), M::Integer(3), M::Integer(4), M::Integer(6)));

    M::Set u3 = M::unionOf(M::Variant(M::Set::build(s1, s2, s3)));
    QCOMPARE(u3, M::Set::build(M::Integer(1), M::Integer(2), M::Integer(3), M::Integer(4), M::Integer(6)));

    M::Set u4 = M::unionOf(M::Variant(M::Tuple::build(s1, s2, s3)));
    QCOMPARE(u4, M::Set::build(M::Integer(1), M::Integer(2), M::Integer(3), M::Integer(4), M::Integer(6)));
}


void TestSetFunctions::testBinaryIntersectionOf() {
    M::Set s1 = M::Set::build(M::Integer(1), M::Integer(2), M::Integer(3));
    M::Set s2 = M::Set::build(M::Integer(2), M::Integer(4));

    M::Set i = M::intersectionOf(s1, s2);
    QCOMPARE(i, M::Set::build(M::Integer(2)));

    QCOMPARE(M::intersectionOf(M::Variant(s1), s2), M::Set::build(M::Integer(2)));
    QCOMPARE(M::intersectionOf(s1, M::Variant(s2)), M::Set::build(M::Integer(2)));
    QCOMPARE(M::intersectionOf(M::Variant(s1), M::Variant(s2)), M::Set::build(M::Integer(2)));
}


void TestSetFunctions::testBinaryDisjointUnionOf() {
    M::Set s1 = M::Set::build(M::Integer(1), M::Integer(2));
    M::Set s2 = M::Set::build(M::Integer(2), M::Integer(4));

    M::Set set = M::disjointUnionOf(s1, s2);
    QCOMPARE(
        set,
        M::Set::build(
            M::Tuple::build(M::Integer(1), M::Integer(1)),
            M::Tuple::build(M::Integer(2), M::Integer(1)),
            M::Tuple::build(M::Integer(2), M::Integer(2)),
            M::Tuple::build(M::Integer(4), M::Integer(2))
        )
    );

    QCOMPARE(
        M::disjointUnionOf(M::Variant(s1), s2),
        M::Set::build(
            M::Tuple::build(M::Integer(1), M::Integer(1)),
            M::Tuple::build(M::Integer(2), M::Integer(1)),
            M::Tuple::build(M::Integer(2), M::Integer(2)),
            M::Tuple::build(M::Integer(4), M::Integer(2))
        )
    );
    QCOMPARE(
        M::disjointUnionOf(s1, M::Variant(s2)),
        M::Set::build(
            M::Tuple::build(M::Integer(1), M::Integer(1)),
            M::Tuple::build(M::Integer(2), M::Integer(1)),
            M::Tuple::build(M::Integer(2), M::Integer(2)),
            M::Tuple::build(M::Integer(4), M::Integer(2))
        )
    );
    QCOMPARE(
        M::disjointUnionOf(M::Variant(s1), M::Variant(s2)),
        M::Set::build(
            M::Tuple::build(M::Integer(1), M::Integer(1)),
            M::Tuple::build(M::Integer(2), M::Integer(1)),
            M::Tuple::build(M::Integer(2), M::Integer(2)),
            M::Tuple::build(M::Integer(4), M::Integer(2))
        )
    );
}

void TestSetFunctions::testNaryDisjointUnionOf() {
    M::Set s1 = M::Set::build(M::Integer(1), M::Integer(2));
    M::Set s2 = M::Set::build(M::Integer(2), M::Integer(4));
    M::Set s3 = M::Set::build(M::Integer(3), M::Integer(4));

    M::Set u1 = M::disjointUnionOf(M::Set::build(s1, s2, s3));
    QCOMPARE(
        u1,
        M::Set::build(
            M::Tuple::build(M::Integer(1), M::Integer(1)),
            M::Tuple::build(M::Integer(2), M::Integer(1)),
            M::Tuple::build(M::Integer(2), M::Integer(2)),
            M::Tuple::build(M::Integer(3), M::Integer(3)),
            M::Tuple::build(M::Integer(4), M::Integer(2)),
            M::Tuple::build(M::Integer(4), M::Integer(3))
        )
    );

    M::Set u2 = M::disjointUnionOf(M::Tuple::build(s1, s2, s3));
    QCOMPARE(
        u2,
        M::Set::build(
            M::Tuple::build(M::Integer(1), M::Integer(1)),
            M::Tuple::build(M::Integer(2), M::Integer(1)),
            M::Tuple::build(M::Integer(2), M::Integer(2)),
            M::Tuple::build(M::Integer(3), M::Integer(3)),
            M::Tuple::build(M::Integer(4), M::Integer(2)),
            M::Tuple::build(M::Integer(4), M::Integer(3))
        )
    );


    M::Set u3 = M::disjointUnionOf(M::Variant(M::Set::build(s1, s2, s3)));
    QCOMPARE(
        u3,
        M::Set::build(
            M::Tuple::build(M::Integer(1), M::Integer(1)),
            M::Tuple::build(M::Integer(2), M::Integer(1)),
            M::Tuple::build(M::Integer(2), M::Integer(2)),
            M::Tuple::build(M::Integer(3), M::Integer(3)),
            M::Tuple::build(M::Integer(4), M::Integer(2)),
            M::Tuple::build(M::Integer(4), M::Integer(3))
        )
    );

    M::Set u4 = M::disjointUnionOf(M::Variant(M::Tuple::build(s1, s2, s3)));
    QCOMPARE(
        u4,
        M::Set::build(
            M::Tuple::build(M::Integer(1), M::Integer(1)),
            M::Tuple::build(M::Integer(2), M::Integer(1)),
            M::Tuple::build(M::Integer(2), M::Integer(2)),
            M::Tuple::build(M::Integer(3), M::Integer(3)),
            M::Tuple::build(M::Integer(4), M::Integer(2)),
            M::Tuple::build(M::Integer(4), M::Integer(3))
        )
    );
}


void TestSetFunctions::testBinaryCartesianProductOf() {
    M::Set s1 = M::Set::build(M::Integer(1), M::Integer(2));
    M::Set s2 = M::Set::build(M::Integer(2), M::Integer(4));

    QCOMPARE(
        M::cartesianProductOf(s1, s2),
        M::Set::build(
            M::Tuple::build(M::Integer(1), M::Integer(2)),
            M::Tuple::build(M::Integer(1), M::Integer(4)),
            M::Tuple::build(M::Integer(2), M::Integer(2)),
            M::Tuple::build(M::Integer(2), M::Integer(4))
        )
    );
    QCOMPARE(
        M::cartesianProductOf(M::Variant(s1), s2),
        M::Set::build(
            M::Tuple::build(M::Integer(1), M::Integer(2)),
            M::Tuple::build(M::Integer(1), M::Integer(4)),
            M::Tuple::build(M::Integer(2), M::Integer(2)),
            M::Tuple::build(M::Integer(2), M::Integer(4))
        )
    );
    QCOMPARE(
        M::cartesianProductOf(s1, M::Variant(s2)),
        M::Set::build(
            M::Tuple::build(M::Integer(1), M::Integer(2)),
            M::Tuple::build(M::Integer(1), M::Integer(4)),
            M::Tuple::build(M::Integer(2), M::Integer(2)),
            M::Tuple::build(M::Integer(2), M::Integer(4))
        )
    );
    QCOMPARE(
        M::cartesianProductOf(M::Variant(s1), M::Variant(s2)),
        M::Set::build(
            M::Tuple::build(M::Integer(1), M::Integer(2)),
            M::Tuple::build(M::Integer(1), M::Integer(4)),
            M::Tuple::build(M::Integer(2), M::Integer(2)),
            M::Tuple::build(M::Integer(2), M::Integer(4))
        )
    );
}


void TestSetFunctions::testRelativeComplementOf() {
    M::Set s1 = M::Set::build(M::Integer(1), M::Integer(2));
    M::Set s2 = M::Set::build(M::Integer(2), M::Integer(4));

    QCOMPARE(M::relativeComplementOf(s1, s2), M::Set::build(M::Integer(1)));
    QCOMPARE(M::relativeComplementOf(M::Variant(s1), s2), M::Set::build(M::Integer(1)));
    QCOMPARE(M::relativeComplementOf(s1, M::Variant(s2)), M::Set::build(M::Integer(1)));
    QCOMPARE(M::relativeComplementOf(M::Variant(s1), M::Variant(s2)), M::Set::build(M::Integer(1)));
}


void TestSetFunctions::testSymmetricDifferenceOf() {
    M::Set s1 = M::Set::build(M::Integer(1), M::Integer(2));
    M::Set s2 = M::Set::build(M::Integer(2), M::Integer(4));

    QCOMPARE(M::symmetricDifferenceOf(s1, s2), M::Set::build(M::Integer(1), M::Integer(4)));
    QCOMPARE(M::symmetricDifferenceOf(M::Variant(s1), s2), M::Set::build(M::Integer(1), M::Integer(4)));
    QCOMPARE(M::symmetricDifferenceOf(s1, M::Variant(s2)), M::Set::build(M::Integer(1), M::Integer(4)));
    QCOMPARE(M::symmetricDifferenceOf(M::Variant(s1), M::Variant(s2)), M::Set::build(M::Integer(1), M::Integer(4)));
}


void TestSetFunctions::testIsElementOfSet() {
    M::Set s = M::Set::build(M::Integer(1), M::Integer(2), M::Integer(4));
    QCOMPARE(M::isElementOfSet(M::Integer(2), s), true);
    QCOMPARE(M::isElementOfSet(M::Integer(3), s), false);

    M::Tuple t = M::Tuple::build(M::Integer(1));

    QCOMPARE(M::isElementOfSet(true, M::BooleanSet::instance), true);
    QCOMPARE(M::isElementOfSet(false, M::BooleanSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Integer(5), M::BooleanSet::instance), false);

    QCOMPARE(M::isElementOfSet(M::Integer(5), M::IntegerSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Real(5), M::IntegerSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Complex(5, 0), M::IntegerSet::instance), true);
    QCOMPARE(M::isElementOfSet(true, M::IntegerSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Real(5.5), M::IntegerSet::instance), false);
    QCOMPARE(M::isElementOfSet(M::Complex(5.5, 0), M::IntegerSet::instance), false);
    QCOMPARE(M::isElementOfSet(M::Complex(5, 1), M::IntegerSet::instance), false);
    QCOMPARE(M::isElementOfSet(s, M::IntegerSet::instance), false);
    QCOMPARE(M::isElementOfSet(t, M::IntegerSet::instance), false);

    QCOMPARE(M::isElementOfSet(M::Integer(5), M::RealSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Real(5), M::RealSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Real(5.5), M::RealSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Complex(5, 0), M::RealSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Complex(5.5, 0), M::RealSet::instance), true);
    QCOMPARE(M::isElementOfSet(true, M::RealSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Complex(5, 1), M::RealSet::instance), false);
    QCOMPARE(M::isElementOfSet(M::Complex(5.5, 1), M::RealSet::instance), false);
    QCOMPARE(M::isElementOfSet(s, M::RealSet::instance), false);
    QCOMPARE(M::isElementOfSet(t, M::RealSet::instance), false);

    QCOMPARE(M::isElementOfSet(M::Integer(5), M::ComplexSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Real(5), M::ComplexSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Real(5.5), M::ComplexSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Complex(5, 0), M::ComplexSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Complex(5.5, 0), M::ComplexSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Complex(5, 1), M::ComplexSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Complex(5.5, 1), M::ComplexSet::instance), true);
    QCOMPARE(M::isElementOfSet(true, M::ComplexSet::instance), true);
    QCOMPARE(M::isElementOfSet(s, M::ComplexSet::instance), false);
    QCOMPARE(M::isElementOfSet(t, M::ComplexSet::instance), false);


    QCOMPARE(M::isElementOfSet(M::Variant(true), M::BooleanSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Variant(false), M::BooleanSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Variant(M::Integer(5)), M::BooleanSet::instance), false);

    QCOMPARE(M::isElementOfSet(M::Variant(M::Integer(5)), M::IntegerSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Variant(M::Real(5)), M::IntegerSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Variant(M::Complex(5, 0)), M::IntegerSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Variant(true), M::IntegerSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Variant(M::Real(5.5)), M::IntegerSet::instance), false);
    QCOMPARE(M::isElementOfSet(M::Variant(M::Complex(5.5, 0)), M::IntegerSet::instance), false);
    QCOMPARE(M::isElementOfSet(M::Variant(M::Complex(5, 1)), M::IntegerSet::instance), false);

    QCOMPARE(M::isElementOfSet(M::Variant(M::Integer(5)), M::RealSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Variant(M::Real(5)), M::RealSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Variant(M::Real(5.5)), M::RealSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Variant(M::Complex(5, 0)), M::RealSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Variant(M::Complex(5.5, 0)), M::RealSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Variant(true), M::RealSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Variant(M::Complex(5, 1)), M::RealSet::instance), false);
    QCOMPARE(M::isElementOfSet(M::Variant(M::Complex(5.5, 1)), M::RealSet::instance), false);

    QCOMPARE(M::isElementOfSet(M::Variant(M::Integer(5)), M::ComplexSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Variant(M::Real(5)), M::ComplexSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Variant(M::Real(5.5)), M::ComplexSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Variant(M::Complex(5, 0)), M::ComplexSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Variant(M::Complex(5.5, 0)), M::ComplexSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Variant(M::Complex(5, 1)), M::ComplexSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Variant(M::Complex(5.5, 1)), M::ComplexSet::instance), true);
    QCOMPARE(M::isElementOfSet(M::Variant(true), M::ComplexSet::instance), true);
}


void TestSetFunctions::testIsNotElementOfSet() {
    M::Set s = M::Set::build(M::Integer(1), M::Integer(2), M::Integer(4));
    QCOMPARE(M::isNotElementOfSet(M::Integer(2), s), false);
    QCOMPARE(M::isNotElementOfSet(M::Integer(3), s), true);

    M::Tuple t = M::Tuple::build(M::Integer(1));

    QCOMPARE(M::isNotElementOfSet(true, M::BooleanSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(false, M::BooleanSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Integer(5), M::BooleanSet::instance), true);

    QCOMPARE(M::isNotElementOfSet(M::Integer(5), M::IntegerSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Real(5), M::IntegerSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Complex(5, 0), M::IntegerSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(true, M::IntegerSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Real(5.5), M::IntegerSet::instance), true);
    QCOMPARE(M::isNotElementOfSet(M::Complex(5.5, 0), M::IntegerSet::instance), true);
    QCOMPARE(M::isNotElementOfSet(M::Complex(5, 1), M::IntegerSet::instance), true);
    QCOMPARE(M::isNotElementOfSet(s, M::IntegerSet::instance), true);
    QCOMPARE(M::isNotElementOfSet(t, M::IntegerSet::instance), true);

    QCOMPARE(M::isNotElementOfSet(M::Integer(5), M::RealSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Real(5), M::RealSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Real(5.5), M::RealSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Complex(5, 0), M::RealSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Complex(5.5, 0), M::RealSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(true, M::RealSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Complex(5, 1), M::RealSet::instance), true);
    QCOMPARE(M::isNotElementOfSet(M::Complex(5.5, 1), M::RealSet::instance), true);
    QCOMPARE(M::isNotElementOfSet(s, M::RealSet::instance), true);
    QCOMPARE(M::isNotElementOfSet(t, M::RealSet::instance), true);

    QCOMPARE(M::isNotElementOfSet(M::Integer(5), M::ComplexSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Real(5), M::ComplexSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Real(5.5), M::ComplexSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Complex(5, 0), M::ComplexSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Complex(5.5, 0), M::ComplexSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Complex(5, 1), M::ComplexSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Complex(5.5, 1), M::ComplexSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(true, M::ComplexSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(s, M::ComplexSet::instance), true);
    QCOMPARE(M::isNotElementOfSet(t, M::ComplexSet::instance), true);


    QCOMPARE(M::isNotElementOfSet(M::Variant(true), M::BooleanSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Variant(false), M::BooleanSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Variant(M::Integer(5)), M::BooleanSet::instance), true);

    QCOMPARE(M::isNotElementOfSet(M::Variant(M::Integer(5)), M::IntegerSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Variant(M::Real(5)), M::IntegerSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Variant(M::Complex(5, 0)), M::IntegerSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Variant(true), M::IntegerSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Variant(M::Real(5.5)), M::IntegerSet::instance), true);
    QCOMPARE(M::isNotElementOfSet(M::Variant(M::Complex(5.5, 0)), M::IntegerSet::instance), true);
    QCOMPARE(M::isNotElementOfSet(M::Variant(M::Complex(5, 1)), M::IntegerSet::instance), true);

    QCOMPARE(M::isNotElementOfSet(M::Variant(M::Integer(5)), M::RealSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Variant(M::Real(5)), M::RealSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Variant(M::Real(5.5)), M::RealSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Variant(M::Complex(5, 0)), M::RealSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Variant(M::Complex(5.5, 0)), M::RealSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Variant(true), M::RealSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Variant(M::Complex(5, 1)), M::RealSet::instance), true);
    QCOMPARE(M::isNotElementOfSet(M::Variant(M::Complex(5.5, 1)), M::RealSet::instance), true);

    QCOMPARE(M::isNotElementOfSet(M::Variant(M::Integer(5)), M::ComplexSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Variant(M::Real(5)), M::ComplexSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Variant(M::Real(5.5)), M::ComplexSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Variant(M::Complex(5, 0)), M::ComplexSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Variant(M::Complex(5.5, 0)), M::ComplexSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Variant(M::Complex(5, 1)), M::ComplexSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Variant(M::Complex(5.5, 1)), M::ComplexSet::instance), false);
    QCOMPARE(M::isNotElementOfSet(M::Variant(true), M::ComplexSet::instance), false);

}


void TestSetFunctions::testIsSubsetOf() {
    M::Set s1 = M::Set::build(M::Integer(1), M::Integer(2), M::Integer(4));
    M::Set s2 = M::Set::build(M::Integer(2), M::Integer(4));
    M::Set s3 = M::Set::build(M::Integer(2), M::Integer(5));

    QCOMPARE(M::isSubsetOf(s2, s1), true);
    QCOMPARE(M::isSubsetOf(s1, s1), true);
    QCOMPARE(M::isSubsetOf(s3, s1), false);

    QCOMPARE(M::isSubsetOf(M::Set::build(true), M::BooleanSet::instance), true);
    QCOMPARE(M::isSubsetOf(M::Set::build(true, false), M::BooleanSet::instance), true);
    QCOMPARE(M::isSubsetOf(M::Set::build(M::Integer(2)), M::BooleanSet::instance), false);
    QCOMPARE(M::isSubsetOf(M::Set::build(true, M::Integer(2)), M::BooleanSet::instance), false);

    QCOMPARE(M::isSubsetOf(M::Set::build(M::Integer(1), M::Integer(5)), M::IntegerSet::instance), true);
    QCOMPARE(M::isSubsetOf(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5)), M::IntegerSet::instance), true);
    QCOMPARE(M::isSubsetOf(M::Set::build(M::Integer(1), M::Real(5.1), M::Complex(5)), M::IntegerSet::instance), false);
    QCOMPARE(M::isSubsetOf(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5.1)), M::IntegerSet::instance), false);
    QCOMPARE(M::isSubsetOf(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5,1)), M::IntegerSet::instance), false);
    QCOMPARE(M::isSubsetOf(M::Set::build(M::Integer(1), M::Real(5), false), M::IntegerSet::instance), false);

    QCOMPARE(M::isSubsetOf(M::Set::build(M::Integer(1), M::Integer(5)), M::RealSet::instance), true);
    QCOMPARE(M::isSubsetOf(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5)), M::RealSet::instance), true);
    QCOMPARE(M::isSubsetOf(M::Set::build(M::Integer(1), M::Real(5.1), M::Complex(5)), M::RealSet::instance), true);
    QCOMPARE(M::isSubsetOf(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5.1)), M::RealSet::instance), true);
    QCOMPARE(M::isSubsetOf(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5,1)), M::RealSet::instance), false);
    QCOMPARE(M::isSubsetOf(M::Set::build(M::Integer(1), M::Real(5), false), M::RealSet::instance), false);

    QCOMPARE(M::isSubsetOf(M::Set::build(M::Integer(1), M::Integer(5)), M::ComplexSet::instance), true);
    QCOMPARE(M::isSubsetOf(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5)), M::ComplexSet::instance), true);
    QCOMPARE(M::isSubsetOf(M::Set::build(M::Integer(1), M::Real(5.1), M::Complex(5)), M::ComplexSet::instance), true);
    QCOMPARE(M::isSubsetOf(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5.1)), M::ComplexSet::instance), true);
    QCOMPARE(M::isSubsetOf(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5,1)), M::ComplexSet::instance), true);
    QCOMPARE(M::isSubsetOf(M::Set::build(M::Integer(1), M::Real(5), false), M::ComplexSet::instance), false);

    QCOMPARE(M::isSubsetOf(M::BooleanSet::instance, M::BooleanSet::instance), true);
    QCOMPARE(M::isSubsetOf(M::BooleanSet::instance, M::IntegerSet::instance), false);
    QCOMPARE(M::isSubsetOf(M::BooleanSet::instance, M::RealSet::instance), false);
    QCOMPARE(M::isSubsetOf(M::BooleanSet::instance, M::ComplexSet::instance), false);

    QCOMPARE(M::isSubsetOf(M::IntegerSet::instance, M::BooleanSet::instance), false);
    QCOMPARE(M::isSubsetOf(M::IntegerSet::instance, M::IntegerSet::instance), true);
    QCOMPARE(M::isSubsetOf(M::IntegerSet::instance, M::RealSet::instance), true);
    QCOMPARE(M::isSubsetOf(M::IntegerSet::instance, M::ComplexSet::instance), true);

    QCOMPARE(M::isSubsetOf(M::RealSet::instance, M::BooleanSet::instance), false);
    QCOMPARE(M::isSubsetOf(M::RealSet::instance, M::IntegerSet::instance), false);
    QCOMPARE(M::isSubsetOf(M::RealSet::instance, M::RealSet::instance), true);
    QCOMPARE(M::isSubsetOf(M::RealSet::instance, M::ComplexSet::instance), true);

    QCOMPARE(M::isSubsetOf(M::ComplexSet::instance, M::BooleanSet::instance), false);
    QCOMPARE(M::isSubsetOf(M::ComplexSet::instance, M::IntegerSet::instance), false);
    QCOMPARE(M::isSubsetOf(M::ComplexSet::instance, M::RealSet::instance), false);
    QCOMPARE(M::isSubsetOf(M::ComplexSet::instance, M::ComplexSet::instance), true);

    QCOMPARE(M::isSubsetOf(M::BooleanSet::instance, M::Set::build(false)), false);
    QCOMPARE(M::isSubsetOf(M::BooleanSet::instance, M::Set::build(false, true)), true);
    QCOMPARE(M::isSubsetOf(M::IntegerSet::instance, s1), false);
    QCOMPARE(M::isSubsetOf(M::RealSet::instance, s1), false);
    QCOMPARE(M::isSubsetOf(M::ComplexSet::instance, s1), false);



    QCOMPARE(M::isSubsetOf(M::Variant(s2), s1), true);
    QCOMPARE(M::isSubsetOf(M::Variant(s1), s1), true);
    QCOMPARE(M::isSubsetOf(M::Variant(s3), s1), false);

    QCOMPARE(M::isSubsetOf(M::Variant(M::Set::build(true)), M::BooleanSet::instance), true);
    QCOMPARE(M::isSubsetOf(M::Variant(M::Set::build(true, false)), M::BooleanSet::instance), true);
    QCOMPARE(M::isSubsetOf(M::Variant(M::Set::build(M::Integer(2))), M::BooleanSet::instance), false);
    QCOMPARE(M::isSubsetOf(M::Variant(M::Set::build(true, M::Integer(2))), M::BooleanSet::instance), false);

    QCOMPARE(
        M::isSubsetOf(M::Variant(M::Set::build(M::Integer(1), M::Integer(5))), M::IntegerSet::instance),
        true
    );
    QCOMPARE(
        M::isSubsetOf(M::Variant(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5))), M::IntegerSet::instance),
        true
    );
    QCOMPARE(
        M::isSubsetOf(M::Variant(M::Set::build(M::Integer(1), M::Real(5.1), M::Complex(5))), M::IntegerSet::instance),
        false
    );
    QCOMPARE(
        M::isSubsetOf(M::Variant(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5.1))), M::IntegerSet::instance),
        false
    );
    QCOMPARE(
        M::isSubsetOf(M::Variant(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5,1))), M::IntegerSet::instance),
        false
    );
    QCOMPARE(
        M::isSubsetOf(M::Variant(M::Set::build(M::Integer(1), M::Real(5), false)), M::IntegerSet::instance),
        false
    );

    QCOMPARE(
        M::isSubsetOf(M::Variant(M::Set::build(M::Integer(1), M::Integer(5))), M::RealSet::instance),
        true
    );
    QCOMPARE(
        M::isSubsetOf(M::Variant(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5))), M::RealSet::instance),
        true
    );
    QCOMPARE(
        M::isSubsetOf(M::Variant(M::Set::build(M::Integer(1), M::Real(5.1), M::Complex(5))), M::RealSet::instance),
        true
    );
    QCOMPARE(
        M::isSubsetOf(M::Variant(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5.1))), M::RealSet::instance),
        true
    );
    QCOMPARE(
        M::isSubsetOf(M::Variant(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5,1))), M::RealSet::instance),
        false
    );
    QCOMPARE(
        M::isSubsetOf(M::Variant(M::Set::build(M::Integer(1), M::Real(5), false)), M::RealSet::instance),
        false
    );

    QCOMPARE(M::isSubsetOf(M::Variant(M::Set::build(M::Integer(1), M::Integer(5))), M::ComplexSet::instance), true);
    QCOMPARE(
        M::isSubsetOf(M::Variant(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5))), M::ComplexSet::instance),
        true
    );
    QCOMPARE(
        M::isSubsetOf(M::Variant(M::Set::build(M::Integer(1), M::Real(5.1), M::Complex(5))), M::ComplexSet::instance),
        true
    );
    QCOMPARE(
        M::isSubsetOf(M::Variant(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5.1))), M::ComplexSet::instance),
        true
    );
    QCOMPARE(
        M::isSubsetOf(M::Variant(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5,1))), M::ComplexSet::instance),
        true
    );
    QCOMPARE(
        M::isSubsetOf(M::Variant(M::Set::build(M::Integer(1), M::Real(5), false)), M::ComplexSet::instance),
        false
    );


    QCOMPARE(M::isSubsetOf(s2, M::Variant(s1)), true);
    QCOMPARE(M::isSubsetOf(s1, M::Variant(s1)), true);
    QCOMPARE(M::isSubsetOf(s3, M::Variant(s1)), false);


    QCOMPARE(M::isSubsetOf(M::Variant(s2), M::Variant(s1)), true);
    QCOMPARE(M::isSubsetOf(M::Variant(s1), M::Variant(s1)), true);
    QCOMPARE(M::isSubsetOf(M::Variant(s3), M::Variant(s1)), false);
}


void TestSetFunctions::testIsProperSubsetOf() {
    M::Set s1 = M::Set::build(M::Integer(1), M::Integer(2), M::Integer(4));
    M::Set s2 = M::Set::build(M::Integer(2), M::Integer(4));
    M::Set s3 = M::Set::build(M::Integer(2), M::Integer(5));

    QCOMPARE(M::isProperSubsetOf(s2, s1), true);
    QCOMPARE(M::isProperSubsetOf(s1, s1), false);
    QCOMPARE(M::isProperSubsetOf(s3, s1), false);

    QCOMPARE(M::isProperSubsetOf(M::Set::build(true), M::BooleanSet::instance), true);
    QCOMPARE(M::isProperSubsetOf(M::Set::build(true, false), M::BooleanSet::instance), false);
    QCOMPARE(M::isProperSubsetOf(M::Set::build(M::Integer(2)), M::BooleanSet::instance), false);
    QCOMPARE(M::isProperSubsetOf(M::Set::build(true, M::Integer(2)), M::BooleanSet::instance), false);

    QCOMPARE(M::isProperSubsetOf(M::Set::build(M::Integer(1), M::Integer(5)), M::IntegerSet::instance), true);
    QCOMPARE(M::isProperSubsetOf(
        M::Set::build(M::Integer(1), M::Real(5), M::Complex(5)), M::IntegerSet::instance),
        true
    );
    QCOMPARE(M::isProperSubsetOf(
        M::Set::build(M::Integer(1), M::Real(5.1), M::Complex(5)), M::IntegerSet::instance),
        false
    );
    QCOMPARE(
        M::isProperSubsetOf(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5.1)), M::IntegerSet::instance),
        false
    );
    QCOMPARE(
        M::isProperSubsetOf(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5,1)), M::IntegerSet::instance),
        false
    );
    QCOMPARE(M::isProperSubsetOf(M::Set::build(M::Integer(1), M::Real(5), false), M::IntegerSet::instance), false);

    QCOMPARE(M::isProperSubsetOf(M::Set::build(M::Integer(1), M::Integer(5)), M::RealSet::instance), true);
    QCOMPARE(
        M::isProperSubsetOf(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5)), M::RealSet::instance),
        true
    );
    QCOMPARE(
        M::isProperSubsetOf(M::Set::build(M::Integer(1), M::Real(5.1), M::Complex(5)), M::RealSet::instance),
        true
    );
    QCOMPARE(
        M::isProperSubsetOf(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5.1)), M::RealSet::instance),
        true
    );
    QCOMPARE(
        M::isProperSubsetOf(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5,1)), M::RealSet::instance),
        false
    );
    QCOMPARE(M::isProperSubsetOf(M::Set::build(M::Integer(1), M::Real(5), false), M::RealSet::instance), false);

    QCOMPARE(M::isProperSubsetOf(M::Set::build(M::Integer(1), M::Integer(5)), M::ComplexSet::instance), true);
    QCOMPARE(M::isProperSubsetOf(
        M::Set::build(M::Integer(1), M::Real(5), M::Complex(5)), M::ComplexSet::instance),
        true
    );
    QCOMPARE(
        M::isProperSubsetOf(M::Set::build(M::Integer(1), M::Real(5.1), M::Complex(5)), M::ComplexSet::instance),
        true
    );
    QCOMPARE(
        M::isProperSubsetOf(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5.1)), M::ComplexSet::instance),
        true
    );
    QCOMPARE(
       M::isProperSubsetOf(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5,1)), M::ComplexSet::instance),
       true
    );
    QCOMPARE(
        M::isProperSubsetOf(M::Set::build(M::Integer(1), M::Real(5), false), M::ComplexSet::instance),
        false
    );

    QCOMPARE(M::isProperSubsetOf(M::BooleanSet::instance, M::BooleanSet::instance), false);
    QCOMPARE(M::isProperSubsetOf(M::BooleanSet::instance, M::IntegerSet::instance), true);
    QCOMPARE(M::isProperSubsetOf(M::BooleanSet::instance, M::RealSet::instance), true);
    QCOMPARE(M::isProperSubsetOf(M::BooleanSet::instance, M::ComplexSet::instance), true);

    QCOMPARE(M::isProperSubsetOf(M::IntegerSet::instance, M::BooleanSet::instance), false);
    QCOMPARE(M::isProperSubsetOf(M::IntegerSet::instance, M::IntegerSet::instance), false);
    QCOMPARE(M::isProperSubsetOf(M::IntegerSet::instance, M::RealSet::instance), true);
    QCOMPARE(M::isProperSubsetOf(M::IntegerSet::instance, M::ComplexSet::instance), true);

    QCOMPARE(M::isProperSubsetOf(M::RealSet::instance, M::BooleanSet::instance), false);
    QCOMPARE(M::isProperSubsetOf(M::RealSet::instance, M::IntegerSet::instance), false);
    QCOMPARE(M::isProperSubsetOf(M::RealSet::instance, M::RealSet::instance), false);
    QCOMPARE(M::isProperSubsetOf(M::RealSet::instance, M::ComplexSet::instance), true);

    QCOMPARE(M::isProperSubsetOf(M::ComplexSet::instance, M::BooleanSet::instance), false);
    QCOMPARE(M::isProperSubsetOf(M::ComplexSet::instance, M::IntegerSet::instance), false);
    QCOMPARE(M::isProperSubsetOf(M::ComplexSet::instance, M::RealSet::instance), false);
    QCOMPARE(M::isProperSubsetOf(M::ComplexSet::instance, M::ComplexSet::instance), false);

    QCOMPARE(M::isProperSubsetOf(M::BooleanSet::instance, M::Set::build(false)), false);
    QCOMPARE(M::isProperSubsetOf(M::BooleanSet::instance, M::Set::build(false, true)), false);
    QCOMPARE(M::isProperSubsetOf(M::IntegerSet::instance, s1), false);
    QCOMPARE(M::isProperSubsetOf(M::RealSet::instance, s1), false);
    QCOMPARE(M::isProperSubsetOf(M::ComplexSet::instance, s1), false);


    QCOMPARE(M::isProperSubsetOf(M::Variant(s2), s1), true);
    QCOMPARE(M::isProperSubsetOf(M::Variant(s1), s1), false);
    QCOMPARE(M::isProperSubsetOf(M::Variant(s3), s1), false);

    QCOMPARE(M::isProperSubsetOf(M::Variant(M::Set::build(true)), M::BooleanSet::instance), true);
    QCOMPARE(M::isProperSubsetOf(M::Variant(M::Set::build(true, false)), M::BooleanSet::instance), false);
    QCOMPARE(M::isProperSubsetOf(M::Variant(M::Set::build(M::Integer(2))), M::BooleanSet::instance), false);
    QCOMPARE(M::isProperSubsetOf(M::Variant(M::Set::build(true, M::Integer(2))), M::BooleanSet::instance), false);

    QCOMPARE(
        M::isProperSubsetOf(M::Variant(M::Set::build(M::Integer(1), M::Integer(5))), M::IntegerSet::instance),
        true
    );
    QCOMPARE(
        M::isProperSubsetOf(
            M::Variant(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5))),
            M::IntegerSet::instance
        ),
        true
    );
    QCOMPARE(
        M::isProperSubsetOf(
            M::Variant(M::Set::build(M::Integer(1), M::Real(5.1), M::Complex(5))),
            M::IntegerSet::instance
        ),
        false
    );
    QCOMPARE(
        M::isProperSubsetOf(
            M::Variant(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5.1))),
            M::IntegerSet::instance
        ),
        false
    );
    QCOMPARE(
        M::isProperSubsetOf(
            M::Variant(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5,1))),
            M::IntegerSet::instance
        ),
        false
    );
    QCOMPARE(
        M::isProperSubsetOf(
            M::Variant(M::Set::build(M::Integer(1), M::Real(5), false)),
            M::IntegerSet::instance
        ),
        false
    );

    QCOMPARE(M::isProperSubsetOf(M::Variant(M::Set::build(M::Integer(1), M::Integer(5))), M::RealSet::instance), true);
    QCOMPARE(
        M::isProperSubsetOf(
            M::Variant(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5))),
            M::RealSet::instance
        ),
        true
    );
    QCOMPARE(
        M::isProperSubsetOf(
            M::Variant(M::Set::build(M::Integer(1), M::Real(5.1), M::Complex(5))),
            M::RealSet::instance
        ),
        true
    );
    QCOMPARE(
        M::isProperSubsetOf(
            M::Variant(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5.1))),
            M::RealSet::instance
        ),
        true
    );
    QCOMPARE(
        M::isProperSubsetOf(
            M::Variant(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5,1))),
            M::RealSet::instance
        ),
        false
    );
    QCOMPARE(
        M::isProperSubsetOf(M::Variant(M::Set::build(M::Integer(1), M::Real(5), false)), M::RealSet::instance),
        false
    );

    QCOMPARE(
        M::isProperSubsetOf(M::Variant(M::Set::build(M::Integer(1), M::Integer(5))), M::ComplexSet::instance),
        true
    );
    QCOMPARE(M::isProperSubsetOf(
        M::Variant(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5))), M::ComplexSet::instance),
        true
    );
    QCOMPARE(
        M::isProperSubsetOf(
            M::Variant(M::Set::build(M::Integer(1), M::Real(5.1), M::Complex(5))),
            M::ComplexSet::instance
        ),
        true
    );
    QCOMPARE(
        M::isProperSubsetOf(
            M::Variant(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5.1))),
            M::ComplexSet::instance
        ),
        true
    );
    QCOMPARE(
       M::isProperSubsetOf(
           M::Variant(M::Set::build(M::Integer(1), M::Real(5), M::Complex(5,1))), M::ComplexSet::instance),
       true
    );
    QCOMPARE(
        M::isProperSubsetOf(M::Variant(M::Set::build(M::Integer(1), M::Real(5), false)), M::ComplexSet::instance),
        false
    );


    QCOMPARE(M::isProperSubsetOf(s2, M::Variant(s1)), true);
    QCOMPARE(M::isProperSubsetOf(s1, M::Variant(s1)), false);
    QCOMPARE(M::isProperSubsetOf(s3, M::Variant(s1)), false);


    QCOMPARE(M::isProperSubsetOf(M::Variant(s2), M::Variant(s1)), true);
    QCOMPARE(M::isProperSubsetOf(M::Variant(s1), M::Variant(s1)), false);
    QCOMPARE(M::isProperSubsetOf(M::Variant(s3), M::Variant(s1)), false);
}


void TestSetFunctions::testIsSupersetOf() {
    // Function simily refers back to M::isSubsetOf so we do only minimal testing here.

    M::Set s1 = M::Set::build(M::Integer(1), M::Integer(2), M::Integer(4));
    M::Set s2 = M::Set::build(M::Integer(2), M::Integer(4));
    M::Set s3 = M::Set::build(M::Integer(2), M::Integer(5));

    QCOMPARE(M::isSupersetOf(s1, s2), true);
    QCOMPARE(M::isSupersetOf(s1, s1), true);
    QCOMPARE(M::isSupersetOf(s1, s3), false);

    QCOMPARE(M::isSupersetOf(M::Variant(s1), s2), true);
    QCOMPARE(M::isSupersetOf(M::Variant(s1), s1), true);
    QCOMPARE(M::isSupersetOf(M::Variant(s1), s3), false);

    QCOMPARE(M::isSupersetOf(M::Variant(s1), M::Variant(s2)), true);
    QCOMPARE(M::isSupersetOf(M::Variant(s1), M::Variant(s1)), true);
    QCOMPARE(M::isSupersetOf(M::Variant(s1), M::Variant(s3)), false);

    QCOMPARE(M::isSupersetOf(s1, M::Variant(s2)), true);
    QCOMPARE(M::isSupersetOf(s1, M::Variant(s1)), true);
    QCOMPARE(M::isSupersetOf(s1, M::Variant(s3)), false);
}


void TestSetFunctions::testIsProperSupersetOf() {
    // Function simily refers back to M::isProperSubsetOf so we do only minimal testing here.

    M::Set s1 = M::Set::build(M::Integer(1), M::Integer(2), M::Integer(4));
    M::Set s2 = M::Set::build(M::Integer(2), M::Integer(4));
    M::Set s3 = M::Set::build(M::Integer(2), M::Integer(5));

    QCOMPARE(M::isProperSupersetOf(s1, s2), true);
    QCOMPARE(M::isProperSupersetOf(s1, s1), false);
    QCOMPARE(M::isProperSupersetOf(s1, s3), false);

    QCOMPARE(M::isProperSupersetOf(s1, M::Variant(s2)), true);
    QCOMPARE(M::isProperSupersetOf(s1, M::Variant(s1)), false);
    QCOMPARE(M::isProperSupersetOf(s1, M::Variant(s3)), false);

    QCOMPARE(M::isProperSupersetOf(s1, M::Variant(s2)), true);
    QCOMPARE(M::isProperSupersetOf(s1, M::Variant(s1)), false);
    QCOMPARE(M::isProperSupersetOf(s1, M::Variant(s3)), false);

    QCOMPARE(M::isProperSupersetOf(M::Variant(s1), M::Variant(s2)), true);
    QCOMPARE(M::isProperSupersetOf(M::Variant(s1), M::Variant(s1)), false);
    QCOMPARE(M::isProperSupersetOf(M::Variant(s1), M::Variant(s3)), false);
}


void TestSetFunctions::testIsNotSubsetOf() {
    // Function simily refers back to M::isSubsetOf so we do only minimal testing here.

    M::Set s1 = M::Set::build(M::Integer(1), M::Integer(2), M::Integer(4));
    M::Set s2 = M::Set::build(M::Integer(2), M::Integer(4));
    M::Set s3 = M::Set::build(M::Integer(2), M::Integer(5));

    QCOMPARE(M::isNotSubsetOf(s2, s1), false);
    QCOMPARE(M::isNotSubsetOf(s1, s1), false);
    QCOMPARE(M::isNotSubsetOf(s3, s1), true);

    QCOMPARE(M::isNotSubsetOf(M::Variant(s2), s1), false);
    QCOMPARE(M::isNotSubsetOf(M::Variant(s1), s1), false);
    QCOMPARE(M::isNotSubsetOf(M::Variant(s3), s1), true);

    QCOMPARE(M::isNotSubsetOf(s2, M::Variant(s1)), false);
    QCOMPARE(M::isNotSubsetOf(s1, M::Variant(s1)), false);
    QCOMPARE(M::isNotSubsetOf(s3, M::Variant(s1)), true);

    QCOMPARE(M::isNotSubsetOf(M::Variant(s2), M::Variant(s1)), false);
    QCOMPARE(M::isNotSubsetOf(M::Variant(s1), M::Variant(s1)), false);
    QCOMPARE(M::isNotSubsetOf(M::Variant(s3), M::Variant(s1)), true);
}


void TestSetFunctions::testIsNotProperSubsetOf() {
    // Function simily refers back to M::isProperSubsetOf so we do only minimal testing here.

    M::Set s1 = M::Set::build(M::Integer(1), M::Integer(2), M::Integer(4));
    M::Set s2 = M::Set::build(M::Integer(2), M::Integer(4));
    M::Set s3 = M::Set::build(M::Integer(2), M::Integer(5));

    QCOMPARE(M::isNotProperSubsetOf(s2, s1), false);
    QCOMPARE(M::isNotProperSubsetOf(s1, s1), true);
    QCOMPARE(M::isNotProperSubsetOf(s3, s1), true);

    QCOMPARE(M::isNotProperSubsetOf(s2, M::Variant(s1)), false);
    QCOMPARE(M::isNotProperSubsetOf(s1, M::Variant(s1)), true);
    QCOMPARE(M::isNotProperSubsetOf(s3, M::Variant(s1)), true);

    QCOMPARE(M::isNotProperSubsetOf(M::Variant(s2), s1), false);
    QCOMPARE(M::isNotProperSubsetOf(M::Variant(s1), s1), true);
    QCOMPARE(M::isNotProperSubsetOf(M::Variant(s3), s1), true);

    QCOMPARE(M::isNotProperSubsetOf(M::Variant(s2), M::Variant(s1)), false);
    QCOMPARE(M::isNotProperSubsetOf(M::Variant(s1), M::Variant(s1)), true);
    QCOMPARE(M::isNotProperSubsetOf(M::Variant(s3), M::Variant(s1)), true);
}


void TestSetFunctions::testIsNotSupersetOf() {
    // Function simily refers back to M::isSubsetOf so we do only minimal testing here.

    M::Set s1 = M::Set::build(M::Integer(1), M::Integer(2), M::Integer(4));
    M::Set s2 = M::Set::build(M::Integer(2), M::Integer(4));
    M::Set s3 = M::Set::build(M::Integer(2), M::Integer(5));

    QCOMPARE(M::isNotSupersetOf(s1, s2), false);
    QCOMPARE(M::isNotSupersetOf(s1, s1), false);
    QCOMPARE(M::isNotSupersetOf(s1, s3), true);

    QCOMPARE(M::isNotSupersetOf(s1, M::Variant(s2)), false);
    QCOMPARE(M::isNotSupersetOf(s1, M::Variant(s1)), false);
    QCOMPARE(M::isNotSupersetOf(s1, M::Variant(s3)), true);

    QCOMPARE(M::isNotSupersetOf(M::Variant(s1), s2), false);
    QCOMPARE(M::isNotSupersetOf(M::Variant(s1), s1), false);
    QCOMPARE(M::isNotSupersetOf(M::Variant(s1), s3), true);

    QCOMPARE(M::isNotSupersetOf(M::Variant(s1), M::Variant(s2)), false);
    QCOMPARE(M::isNotSupersetOf(M::Variant(s1), M::Variant(s1)), false);
    QCOMPARE(M::isNotSupersetOf(M::Variant(s1), M::Variant(s3)), true);
}


void TestSetFunctions::testIsNotProperSupersetOf() {
    // Function simily refers back to M::isProperSubsetOf so we do only minimal testing here.

    M::Set s1 = M::Set::build(M::Integer(1), M::Integer(2), M::Integer(4));
    M::Set s2 = M::Set::build(M::Integer(2), M::Integer(4));
    M::Set s3 = M::Set::build(M::Integer(2), M::Integer(5));

    QCOMPARE(M::isNotProperSupersetOf(s1, s2), false);
    QCOMPARE(M::isNotProperSupersetOf(s1, s1), true);
    QCOMPARE(M::isNotProperSupersetOf(s1, s3), true);

    QCOMPARE(M::isNotProperSupersetOf(s1, M::Variant(s2)), false);
    QCOMPARE(M::isNotProperSupersetOf(s1, M::Variant(s1)), true);
    QCOMPARE(M::isNotProperSupersetOf(s1, M::Variant(s3)), true);

    QCOMPARE(M::isNotProperSupersetOf(M::Variant(s1), s2), false);
    QCOMPARE(M::isNotProperSupersetOf(M::Variant(s1), s1), true);
    QCOMPARE(M::isNotProperSupersetOf(M::Variant(s1), s3), true);

    QCOMPARE(M::isNotProperSupersetOf(M::Variant(s1), M::Variant(s2)), false);
    QCOMPARE(M::isNotProperSupersetOf(M::Variant(s1), M::Variant(s1)), true);
    QCOMPARE(M::isNotProperSupersetOf(M::Variant(s1), M::Variant(s3)), true);
}


void TestSetFunctions::testAlphabetSetTuple() {
    M::Set s1;
    QCOMPARE(M::alphabet(s1), M::Set());

    M::Set s2 = M::Set::build(M::Integer(1), M::Integer(3), M::Complex(3,5));
    QCOMPARE(M::alphabet(s2), M::Set::build(M::Integer(3), M::Integer(1), M::Complex(3,5)));

    M::Tuple t1("The rain in Spain stays mainly on the plain.");
    M::Set   s3 = alphabet(t1);

    M::Set   compared;
    unsigned long l = static_cast<unsigned long>(t1.size());
    for (unsigned long i=1 ; i<=l ; ++i) {
        compared.insert(t1.at(M::Integer(i)));
    }

    QCOMPARE(s3, compared);
}


void TestSetFunctions::testAlphabetMatrix() {
    M::MatrixBoolean mb1;
    QCOMPARE(M::alphabet(mb1), M::Set());

    M::MatrixBoolean mb2 = M::MatrixBoolean::build(1, 1, true);
    QCOMPARE(M::alphabet(mb2), M::Set::build(M::Boolean(true)));

    M::MatrixBoolean mb3 = M::MatrixBoolean::build(1, 2, true, false);
    QCOMPARE(M::alphabet(mb3), M::Set::build(M::Boolean(true), M::Boolean(false)));

    M::MatrixInteger mi1;
    QCOMPARE(M::alphabet(mi1), M::Set());

    M::MatrixInteger mi2 = M::MatrixInteger::build(
        2, 2,
        M::Integer(1), M::Integer(3),
        M::Integer(3), M::Integer(5)
    );

    QCOMPARE(M::alphabet(mi2), M::Set::build(M::Integer(1), M::Integer(3), M::Integer(5)));

    M::MatrixReal mr1;
    QCOMPARE(M::alphabet(mr1), M::Set());

    M::MatrixReal mr2 = M::MatrixReal::build(
        2, 2,
        M::Real(1), M::Real(3),
        M::Real(3), M::Real(5)
    );

    QCOMPARE(M::alphabet(mr2), M::Set::build(M::Real(1), M::Real(3), M::Real(5)));

    M::MatrixComplex mc1;
    QCOMPARE(M::alphabet(mc1), M::Set());

    M::MatrixComplex mc2 = M::MatrixComplex::build(
        2, 2,
        M::Complex(1),    M::Complex(3, 1),
        M::Complex(3, 1), M::Complex(3, 3)
    );

    QCOMPARE(M::alphabet(mc2), M::Set::build(M::Complex(1), M::Complex(3, 1), M::Complex(3, 3)));
}


void TestSetFunctions::testToTupleString() {
    QCOMPARE(M::toTupleString(M::Boolean(false)),                  M::Tuple("false"));
    QCOMPARE(M::toTupleString(M::Boolean(true)),                   M::Tuple("true"));
    QCOMPARE(M::toTupleString(M::Boolean(false),  M::Tuple("%d")), M::Tuple("0"));
    QCOMPARE(M::toTupleString(M::Boolean(true),   M::Tuple("%d")), M::Tuple("1"));

    QCOMPARE(M::toTupleString(M::Integer(5)),                     M::Tuple("5"));
    QCOMPARE(M::toTupleString(M::Integer(-524)),                  M::Tuple("-524"));
    QCOMPARE(M::toTupleString(M::Integer(5), M::Tuple("0x%08X")), M::Tuple("0x00000005"));
    QCOMPARE(M::toTupleString(M::Integer(523), M::Tuple("%x")),   M::Tuple("20b"));

    QCOMPARE(M::toTupleString(M::Real(5.24)),                     M::Tuple("5.24"));
    QCOMPARE(M::toTupleString(M::Real(-4.321)),                   M::Tuple("-4.321"));
    QCOMPARE(M::toTupleString(M::Real(5), M::Tuple("%e")),        M::Tuple("5.000000e+00"));
    QCOMPARE(M::toTupleString(M::Real(5.24), M::Tuple("%10.5f")), M::Tuple("   5.24000"));

    QCOMPARE(M::toTupleString(M::Complex(5.24)),                     M::Tuple("5.24"));
    QCOMPARE(M::toTupleString(M::Complex(-4.321)),                   M::Tuple("-4.321"));
    QCOMPARE(M::toTupleString(M::Complex(5), M::Tuple("%e")),        M::Tuple("5.000000e+00"));
    QCOMPARE(M::toTupleString(M::Complex(5.24), M::Tuple("%10.5f")), M::Tuple("   5.24000"));

    QCOMPARE(M::toTupleString(M::Variant(M::Boolean(false))),                  M::Tuple("false"));
    QCOMPARE(M::toTupleString(M::Variant(M::Boolean(true))),                   M::Tuple("true"));
    QCOMPARE(M::toTupleString(M::Variant(M::Boolean(false)),  M::Tuple("%d")), M::Tuple("0"));
    QCOMPARE(M::toTupleString(M::Variant(M::Boolean(true)),   M::Tuple("%d")), M::Tuple("1"));

    QCOMPARE(M::toTupleString(M::Variant(M::Integer(5))),                     M::Tuple("5"));
    QCOMPARE(M::toTupleString(M::Variant(M::Integer(-524))),                  M::Tuple("-524"));
    QCOMPARE(M::toTupleString(M::Variant(M::Integer(5)), M::Tuple("0x%08X")), M::Tuple("0x00000005"));
    QCOMPARE(M::toTupleString(M::Variant(M::Integer(523)), M::Tuple("%x")),   M::Tuple("20b"));

    QCOMPARE(M::toTupleString(M::Variant(M::Real(5.24))),                     M::Tuple("5.24"));
    QCOMPARE(M::toTupleString(M::Variant(M::Real(-4.321))),                   M::Tuple("-4.321"));
    QCOMPARE(M::toTupleString(M::Variant(M::Real(5)), M::Tuple("%e")),        M::Tuple("5.000000e+00"));
    QCOMPARE(M::toTupleString(M::Variant(M::Real(5.24)), M::Tuple("%10.5f")), M::Tuple("   5.24000"));


    QCOMPARE(M::toTupleString(M::Boolean(false)),                              M::Tuple("false"));
    QCOMPARE(M::toTupleString(M::Boolean(true)),                               M::Tuple("true"));
    QCOMPARE(M::toTupleString(M::Boolean(false),  M::Variant(M::Tuple("%d"))), M::Tuple("0"));
    QCOMPARE(M::toTupleString(M::Boolean(true),   M::Variant(M::Tuple("%d"))), M::Tuple("1"));

    QCOMPARE(M::toTupleString(M::Integer(5)),                                 M::Tuple("5"));
    QCOMPARE(M::toTupleString(M::Integer(-524)),                              M::Tuple("-524"));
    QCOMPARE(M::toTupleString(M::Integer(5), M::Variant(M::Tuple("0x%08X"))), M::Tuple("0x00000005"));
    QCOMPARE(M::toTupleString(M::Integer(523), M::Variant(M::Tuple("%x"))),   M::Tuple("20b"));

    QCOMPARE(M::toTupleString(M::Real(5.24)),                                 M::Tuple("5.24"));
    QCOMPARE(M::toTupleString(M::Real(-4.321)),                               M::Tuple("-4.321"));
    QCOMPARE(M::toTupleString(M::Real(5), M::Variant(M::Tuple("%e"))),        M::Tuple("5.000000e+00"));
    QCOMPARE(M::toTupleString(M::Real(5.24), M::Variant(M::Tuple("%10.5f"))), M::Tuple("   5.24000"));

    QCOMPARE(M::toTupleString(M::Complex(5.24)),                                 M::Tuple("5.24"));
    QCOMPARE(M::toTupleString(M::Complex(-4.321)),                               M::Tuple("-4.321"));
    QCOMPARE(M::toTupleString(M::Complex(5), M::Variant(M::Tuple("%e"))),        M::Tuple("5.000000e+00"));
    QCOMPARE(M::toTupleString(M::Complex(5.24), M::Variant(M::Tuple("%10.5f"))), M::Tuple("   5.24000"));

    QCOMPARE(M::toTupleString(M::Variant(M::Boolean(false))),                              M::Tuple("false"));
    QCOMPARE(M::toTupleString(M::Variant(M::Boolean(true))),                               M::Tuple("true"));
    QCOMPARE(M::toTupleString(M::Variant(M::Boolean(false)),  M::Variant(M::Tuple("%d"))), M::Tuple("0"));
    QCOMPARE(M::toTupleString(M::Variant(M::Boolean(true)),   M::Variant(M::Tuple("%d"))), M::Tuple("1"));

    QCOMPARE(M::toTupleString(M::Variant(M::Integer(5))),                                 M::Tuple("5"));
    QCOMPARE(M::toTupleString(M::Variant(M::Integer(-524))),                              M::Tuple("-524"));
    QCOMPARE(M::toTupleString(M::Variant(M::Integer(5)), M::Variant(M::Tuple("0x%08X"))), M::Tuple("0x00000005"));
    QCOMPARE(M::toTupleString(M::Variant(M::Integer(523)), M::Variant(M::Tuple("%x"))),   M::Tuple("20b"));

    QCOMPARE(M::toTupleString(M::Variant(M::Real(5.24))),                                 M::Tuple("5.24"));
    QCOMPARE(M::toTupleString(M::Variant(M::Real(-4.321))),                               M::Tuple("-4.321"));
    QCOMPARE(M::toTupleString(M::Variant(M::Real(5)), M::Variant(M::Tuple("%e"))),        M::Tuple("5.000000e+00"));
    QCOMPARE(M::toTupleString(M::Variant(M::Real(5.24)), M::Variant(M::Tuple("%10.5f"))), M::Tuple("   5.24000"));
}


void TestSetFunctions::testFind() {
    //           123456789012345678901234567
    M::Tuple m1("papaya banana papaya orange");
    M::Tuple m2("apple");

    QCOMPARE(M::find(m1, M::Tuple("pa")), M::Integer(1));
    QCOMPARE(M::find(m1, M::Tuple("ga")), M::Integer(0));
    QCOMPARE(M::find(m1, M::Tuple("pap")), M::Integer(1));
    QCOMPARE(M::find(m1, M::Tuple("pap"), M::Integer(2)), M::Integer(15));

    QCOMPARE(M::find(m2, M::Tuple("apple")),  M::Integer(1));
    QCOMPARE(M::find(m2, M::Tuple("apples")), M::Integer(0));


    QCOMPARE(M::find(M::Variant(m1), M::Tuple("pa")), M::Integer(1));
    QCOMPARE(M::find(M::Variant(m1), M::Tuple("ga")), M::Integer(0));
    QCOMPARE(M::find(M::Variant(m1), M::Tuple("pap")), M::Integer(1));
    QCOMPARE(M::find(M::Variant(m1), M::Tuple("pap"), M::Integer(2)), M::Integer(15));

    QCOMPARE(M::find(M::Variant(m2), M::Tuple("apple")),  M::Integer(1));
    QCOMPARE(M::find(M::Variant(m2), M::Tuple("apples")), M::Integer(0));


    QCOMPARE(M::find(m1, M::Variant(M::Tuple("pa"))), M::Integer(1));
    QCOMPARE(M::find(m1, M::Variant(M::Tuple("ga"))), M::Integer(0));
    QCOMPARE(M::find(m1, M::Variant(M::Tuple("pap"))), M::Integer(1));
    QCOMPARE(M::find(m1, M::Variant(M::Tuple("pap")), M::Integer(2)), M::Integer(15));

    QCOMPARE(M::find(m2, M::Variant(M::Tuple("apple"))),  M::Integer(1));
    QCOMPARE(M::find(m2, M::Variant(M::Tuple("apples"))), M::Integer(0));


    QCOMPARE(M::find(M::Variant(m1), M::Variant(M::Tuple("pa"))), M::Integer(1));
    QCOMPARE(M::find(M::Variant(m1), M::Variant(M::Tuple("ga"))), M::Integer(0));
    QCOMPARE(M::find(M::Variant(m1), M::Variant(M::Tuple("pap"))), M::Integer(1));
    QCOMPARE(M::find(M::Variant(m1), M::Variant(M::Tuple("pap")), M::Integer(2)), M::Integer(15));

    QCOMPARE(M::find(M::Variant(m2), M::Variant(M::Tuple("apple"))),  M::Integer(1));
    QCOMPARE(M::find(M::Variant(m2), M::Variant(M::Tuple("apples"))), M::Integer(0));
}


void TestSetFunctions::testSplit() {
    M::Tuple tuple("papaya  banana papaya \torange  guava passion fruit");

    M::Tuple s1 = M::split(tuple, M::Tuple("  "));
    QCOMPARE(s1.size(), 3);
    QCOMPARE(s1.at(M::Integer(1)), M::Tuple("papaya"));
    QCOMPARE(s1.at(M::Integer(2)), M::Tuple("banana papaya \torange"));
    QCOMPARE(s1.at(M::Integer(3)), M::Tuple("guava passion fruit"));

    M::Tuple s2 = M::split(tuple, M::Tuple("  "), true);
    QCOMPARE(s2.size(), 3);
    QCOMPARE(s2.at(M::Integer(1)), M::Tuple("papaya  "));
    QCOMPARE(s2.at(M::Integer(2)), M::Tuple("banana papaya \torange  "));
    QCOMPARE(s2.at(M::Integer(3)), M::Tuple("guava passion fruit"));

    M::Tuple s3 = M::split(tuple, M::Tuple(" "));
    QCOMPARE(s3.size(), 9);
    QCOMPARE(s3.at(M::Integer(1)), M::Tuple("papaya"));
    QCOMPARE(s3.at(M::Integer(2)), M::Tuple(""));
    QCOMPARE(s3.at(M::Integer(3)), M::Tuple("banana"));
    QCOMPARE(s3.at(M::Integer(4)), M::Tuple("papaya"));
    QCOMPARE(s3.at(M::Integer(5)), M::Tuple("\torange"));
    QCOMPARE(s3.at(M::Integer(6)), M::Tuple(""));
    QCOMPARE(s3.at(M::Integer(7)), M::Tuple("guava"));
    QCOMPARE(s3.at(M::Integer(8)), M::Tuple("passion"));
    QCOMPARE(s3.at(M::Integer(9)), M::Tuple("fruit"));

    M::Tuple s4 = M::split(tuple, M::Tuple(" "), false, true);
    QCOMPARE(s4.size(), 7);
    QCOMPARE(s4.at(M::Integer(1)), M::Tuple("papaya"));
    QCOMPARE(s4.at(M::Integer(2)), M::Tuple("banana"));
    QCOMPARE(s4.at(M::Integer(3)), M::Tuple("papaya"));
    QCOMPARE(s4.at(M::Integer(4)), M::Tuple("\torange"));
    QCOMPARE(s4.at(M::Integer(5)), M::Tuple("guava"));
    QCOMPARE(s4.at(M::Integer(6)), M::Tuple("passion"));
    QCOMPARE(s4.at(M::Integer(7)), M::Tuple("fruit"));

    M::Tuple s5 = M::split(tuple, M::Set::build(M::Integer(' '), M::Integer('\t')));
    QCOMPARE(s5.size(), 10);
    QCOMPARE(s5.at(M::Integer(1)), M::Tuple("papaya"));
    QCOMPARE(s5.at(M::Integer(2)), M::Tuple(""));
    QCOMPARE(s5.at(M::Integer(3)), M::Tuple("banana"));
    QCOMPARE(s5.at(M::Integer(4)), M::Tuple("papaya"));
    QCOMPARE(s5.at(M::Integer(5)), M::Tuple(""));
    QCOMPARE(s5.at(M::Integer(6)), M::Tuple("orange"));
    QCOMPARE(s5.at(M::Integer(7)), M::Tuple(""));
    QCOMPARE(s5.at(M::Integer(8)), M::Tuple("guava"));
    QCOMPARE(s5.at(M::Integer(9)), M::Tuple("passion"));
    QCOMPARE(s5.at(M::Integer(10)), M::Tuple("fruit"));

    M::Tuple s6 = M::split(tuple, M::Set::build(M::Integer(' '), M::Integer('\t')), true);
    QCOMPARE(s6.size(), 10);
    QCOMPARE(s6.at(M::Integer(1)), M::Tuple("papaya "));
    QCOMPARE(s6.at(M::Integer(2)), M::Tuple(" "));
    QCOMPARE(s6.at(M::Integer(3)), M::Tuple("banana "));
    QCOMPARE(s6.at(M::Integer(4)), M::Tuple("papaya "));
    QCOMPARE(s6.at(M::Integer(5)), M::Tuple("\t"));
    QCOMPARE(s6.at(M::Integer(6)), M::Tuple("orange "));
    QCOMPARE(s6.at(M::Integer(7)), M::Tuple(" "));
    QCOMPARE(s6.at(M::Integer(8)), M::Tuple("guava "));
    QCOMPARE(s6.at(M::Integer(9)), M::Tuple("passion "));
    QCOMPARE(s6.at(M::Integer(10)), M::Tuple("fruit"));

    M::Tuple s7 = M::split(tuple, M::Set::build(M::Integer(' '), M::Integer('\t')), false, true);
    QCOMPARE(s7.size(), 7);
    QCOMPARE(s7.at(M::Integer(1)), M::Tuple("papaya"));
    QCOMPARE(s7.at(M::Integer(2)), M::Tuple("banana"));
    QCOMPARE(s7.at(M::Integer(3)), M::Tuple("papaya"));
    QCOMPARE(s7.at(M::Integer(4)), M::Tuple("orange"));
    QCOMPARE(s7.at(M::Integer(5)), M::Tuple("guava"));
    QCOMPARE(s7.at(M::Integer(6)), M::Tuple("passion"));
    QCOMPARE(s7.at(M::Integer(7)), M::Tuple("fruit"));




    M::Tuple s8 = M::split(M::Variant(tuple), M::Tuple("  "));
    QCOMPARE(s8.size(), 3);
    QCOMPARE(s8.at(M::Integer(1)), M::Tuple("papaya"));
    QCOMPARE(s8.at(M::Integer(2)), M::Tuple("banana papaya \torange"));
    QCOMPARE(s8.at(M::Integer(3)), M::Tuple("guava passion fruit"));

    M::Tuple s9 = M::split(M::Variant(tuple), M::Tuple("  "), true);
    QCOMPARE(s9.size(), 3);
    QCOMPARE(s9.at(M::Integer(1)), M::Tuple("papaya  "));
    QCOMPARE(s9.at(M::Integer(2)), M::Tuple("banana papaya \torange  "));
    QCOMPARE(s9.at(M::Integer(3)), M::Tuple("guava passion fruit"));

    M::Tuple s10 = M::split(tuple, M::Variant(M::Tuple(" ")));
    QCOMPARE(s10.size(), 9);
    QCOMPARE(s10.at(M::Integer(1)), M::Tuple("papaya"));
    QCOMPARE(s10.at(M::Integer(2)), M::Tuple(""));
    QCOMPARE(s10.at(M::Integer(3)), M::Tuple("banana"));
    QCOMPARE(s10.at(M::Integer(4)), M::Tuple("papaya"));
    QCOMPARE(s10.at(M::Integer(5)), M::Tuple("\torange"));
    QCOMPARE(s10.at(M::Integer(6)), M::Tuple(""));
    QCOMPARE(s10.at(M::Integer(7)), M::Tuple("guava"));
    QCOMPARE(s10.at(M::Integer(8)), M::Tuple("passion"));
    QCOMPARE(s10.at(M::Integer(9)), M::Tuple("fruit"));

    M::Tuple s11 = M::split(tuple, M::Tuple(" "), false, true);
    QCOMPARE(s11.size(), 7);
    QCOMPARE(s11.at(M::Integer(1)), M::Tuple("papaya"));
    QCOMPARE(s11.at(M::Integer(2)), M::Tuple("banana"));
    QCOMPARE(s11.at(M::Integer(3)), M::Tuple("papaya"));
    QCOMPARE(s11.at(M::Integer(4)), M::Tuple("\torange"));
    QCOMPARE(s11.at(M::Integer(5)), M::Tuple("guava"));
    QCOMPARE(s11.at(M::Integer(6)), M::Tuple("passion"));
    QCOMPARE(s11.at(M::Integer(7)), M::Tuple("fruit"));

    M::Tuple s12 = M::split(tuple, M::Variant(M::Set::build(M::Integer(' '), M::Integer('\t'))));
    QCOMPARE(s12.size(), 10);
    QCOMPARE(s12.at(M::Integer(1)), M::Tuple("papaya"));
    QCOMPARE(s12.at(M::Integer(2)), M::Tuple(""));
    QCOMPARE(s12.at(M::Integer(3)), M::Tuple("banana"));
    QCOMPARE(s12.at(M::Integer(4)), M::Tuple("papaya"));
    QCOMPARE(s12.at(M::Integer(5)), M::Tuple(""));
    QCOMPARE(s12.at(M::Integer(6)), M::Tuple("orange"));
    QCOMPARE(s12.at(M::Integer(7)), M::Tuple(""));
    QCOMPARE(s12.at(M::Integer(8)), M::Tuple("guava"));
    QCOMPARE(s12.at(M::Integer(9)), M::Tuple("passion"));
    QCOMPARE(s12.at(M::Integer(10)), M::Tuple("fruit"));

    M::Tuple s13 = M::split(
        M::Variant(tuple),
        M::Variant(M::Set::build(M::Integer(' '), M::Integer('\t'))),
        M::Integer(1)
    );
    QCOMPARE(s13.size(), 10);
    QCOMPARE(s13.at(M::Integer(1)), M::Tuple("papaya "));
    QCOMPARE(s13.at(M::Integer(2)), M::Tuple(" "));
    QCOMPARE(s13.at(M::Integer(3)), M::Tuple("banana "));
    QCOMPARE(s13.at(M::Integer(4)), M::Tuple("papaya "));
    QCOMPARE(s13.at(M::Integer(5)), M::Tuple("\t"));
    QCOMPARE(s13.at(M::Integer(6)), M::Tuple("orange "));
    QCOMPARE(s13.at(M::Integer(7)), M::Tuple(" "));
    QCOMPARE(s13.at(M::Integer(8)), M::Tuple("guava "));
    QCOMPARE(s13.at(M::Integer(9)), M::Tuple("passion "));
    QCOMPARE(s13.at(M::Integer(10)), M::Tuple("fruit"));

    M::Tuple s14 = M::split(tuple, M::Variant(M::Set::build(M::Integer(' '), M::Integer('\t'))), M::Real(false), true);
    QCOMPARE(s14.size(), 7);
    QCOMPARE(s14.at(M::Integer(1)), M::Tuple("papaya"));
    QCOMPARE(s14.at(M::Integer(2)), M::Tuple("banana"));
    QCOMPARE(s14.at(M::Integer(3)), M::Tuple("papaya"));
    QCOMPARE(s14.at(M::Integer(4)), M::Tuple("orange"));
    QCOMPARE(s14.at(M::Integer(5)), M::Tuple("guava"));
    QCOMPARE(s14.at(M::Integer(6)), M::Tuple("passion"));
    QCOMPARE(s14.at(M::Integer(7)), M::Tuple("fruit"));
}
