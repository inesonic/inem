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
* This file implements tests of the implicit ordering functions.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QObject>
#include <QList>
#include <QtTest/QtTest>

#include <limits>

#include <m_intrinsic_types.h>
#include <m_matrix_boolean.h>
#include <m_matrix_integer.h>
#include <m_matrix_real.h>
#include <m_matrix_complex.h>
#include <m_variant.h>
#include <m_implicit_ordering.h>

#include "test_implicit_ordering.h"

TestImplicitOrdering::TestImplicitOrdering() {}

TestImplicitOrdering::~TestImplicitOrdering() {}


void TestImplicitOrdering::testImplicitOrdering() {
    M::Boolean       boolean1       = false;
    M::Integer       integer1       = 25;
    M::Real          real1          = 24.0;
    M::Complex       complex1       = M::Complex(23.0);
    M::Complex       complex2       = M::Complex(23.0, 22.0);
    M::Set           set1           = M::Set::build(M::Integer(21), M::Integer(20));
    M::Tuple         tuple1         = M::Tuple::build(M::Integer(19), M::Integer(18));
    M::MatrixBoolean matrixBoolean1 = M::MatrixBoolean::build(2, 2, true, false, false, true);
    M::MatrixInteger matrixInteger1 = M::MatrixInteger::build(2, 2, 17, 16, 15, 14);
    M::MatrixReal    matrixReal1    = M::MatrixReal::build(2, 2, 13.0, 12.0, 11.0, 10.0);
    M::MatrixComplex matrixComplex1 = M::MatrixComplex::build(
        2, 2,
        M::Complex(9, 8), M::Complex(7, 6),
        M::Complex(5, 4), M::Complex(3, 2)
    );

    M::Boolean       boolean2       = true;
    M::Integer       integer2       = 125;
    M::Real          real2          = 124.0;
    M::Complex       complex3       = M::Complex(123.0);
    M::Complex       complex4       = M::Complex(123.0, 122.0);
    M::Set           set2           = M::Set::build(M::Integer(121), M::Integer(120));
    M::Tuple         tuple2         = M::Tuple::build(M::Integer(119), M::Integer(118));
    M::MatrixBoolean matrixBoolean2 = M::MatrixBoolean::build(2, 2, true, true, true, true);
    M::MatrixInteger matrixInteger2 = M::MatrixInteger::build(2, 2, 117, 116, 115, 114);
    M::MatrixReal    matrixReal2    = M::MatrixReal::build(2, 2, 113.0, 112.0, 111.0, 110.0);
    M::MatrixComplex matrixComplex2 = M::MatrixComplex::build(
        2, 2,
        M::Complex(19, 18), M::Complex(17, 16),
        M::Complex(15, 14), M::Complex(13, 12)
    );

    M::Variant vBoolean1(boolean1);
    M::Variant vInteger1(integer1);
    M::Variant vReal1(real1);
    M::Variant vComplex1(complex1);
    M::Variant vComplex2(complex2);
    M::Variant vSet1(set1);
    M::Variant vTuple1(tuple1);
    M::Variant vMatrixBoolean1(matrixBoolean1);
    M::Variant vMatrixInteger1(matrixInteger1);
    M::Variant vMatrixReal1(matrixReal1);
    M::Variant vMatrixComplex1(matrixComplex1);

    M::Variant vBoolean2(boolean2);
    M::Variant vInteger2(integer2);
    M::Variant vReal2(real2);
    M::Variant vComplex3(complex3);
    M::Variant vComplex4(complex4);
    M::Variant vSet2(set2);
    M::Variant vTuple2(tuple2);
    M::Variant vMatrixBoolean2(matrixBoolean2);
    M::Variant vMatrixInteger2(matrixInteger2);
    M::Variant vMatrixReal2(matrixReal2);
    M::Variant vMatrixComplex2(matrixComplex2);

    QList<M::Variant> expectedOrdering;
    expectedOrdering << vBoolean1
                     << vBoolean2
                     << vComplex1
                     << vReal1
                     << vInteger1
                     << vComplex2
                     << vComplex3
                     << vComplex4
                     << vSet1
                     << vSet2
                     << vTuple1
                     << vTuple2
                     << vMatrixBoolean1
                     << vMatrixBoolean2
                     << vMatrixInteger1
                     << vMatrixInteger2
                     << vMatrixReal1
                     << vMatrixReal2
                     << vMatrixComplex1
                     << vMatrixComplex2;

    unsigned numberVariants = static_cast<unsigned>(expectedOrdering.size());
    for (unsigned i=1 ; i<numberVariants ; ++i) {
        const M::Variant& a = expectedOrdering.at(i - 1);
        const M::Variant& b = expectedOrdering.at(i);

        QCOMPARE(M::implicitOrdering(a, b) < 0,  true);
        QCOMPARE(M::implicitOrdering(a, a) == 0, true);
        QCOMPARE(M::implicitOrdering(b, b) == 0, true);
        QCOMPARE(M::implicitOrdering(b, a) > 0,  true);
    }
}
