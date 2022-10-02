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
* This file implements provides tests for the set functions class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QObject>
#include <QList>
#include <QtTest/QtTest>

#include <limits>

#include <m_intrinsic_types.h>
#include <m_complex_operators.h>
#include <m_matrix_boolean.h>
#include <m_matrix_integer.h>
#include <m_matrix_real.h>
#include <m_matrix_complex.h>
#include <m_variant.h>
#include <m_variant_operators.h>
#include <m_set_functions.h>
#include <m_matrix_functions.h>
#include <m_basic_functions.h>
#include <m_variant_operators.h>
#include <m_variant_functions.h>

#include "test_variant_functions.h"

TestVariantFunctions::TestVariantFunctions() {}

TestVariantFunctions::~TestVariantFunctions() {}


void TestVariantFunctions::testAbs() {
    M::Boolean       boolean       = true;
    M::Integer       integer       = 2;
    M::Real          real          = 3.0;
    M::Complex       complex       = M::Complex(4.0, 5.0);
    M::Set           set           = M::Set::build(M::Integer(2), M::Integer(3), M::Integer(5));
    M::MatrixBoolean matrixBoolean = M::MatrixBoolean::build(2, 2, true, false, false, true);
    M::MatrixInteger matrixInteger = M::MatrixInteger::build(2, 2, 6, 7, 8, 9);
    M::MatrixReal    matrixReal    = M::MatrixReal::build(2, 2, 10.0, 11.0, 12.0, 13.0);
    M::MatrixComplex matrixComplex = M::MatrixComplex::build(
        2, 2,
        M::Complex(14, 15), M::Complex(16, 17),
        M::Complex(18, 19), M::Complex(20, 21)
    );

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);
    M::Variant vSet(set);
    M::Variant vMatrixBoolean(matrixBoolean);
    M::Variant vMatrixInteger(matrixInteger);
    M::Variant vMatrixReal(matrixReal);
    M::Variant vMatrixComplex(matrixComplex);

    QCOMPARE(M::abs(boolean), M::abs(vBoolean));
    QCOMPARE(M::abs(integer), M::abs(vInteger));
    QCOMPARE(M::abs(real), M::abs(vReal));
    QCOMPARE(M::abs(complex), M::abs(vComplex));
    QCOMPARE(M::abs(set), M::abs(vSet));
    QCOMPARE(M::abs(matrixBoolean), M::abs(vMatrixBoolean));
    QCOMPARE(M::abs(matrixInteger), M::abs(vMatrixInteger));
    QCOMPARE(M::abs(matrixReal), M::abs(vMatrixReal));
    QCOMPARE(M::abs(matrixComplex), M::abs(vMatrixComplex));
}


void TestVariantFunctions::testSize() {
    M::Tuple         tuple         = M::Tuple::build(M::Integer(1), M::Integer(2), M::Integer(4));
    M::Set           set           = M::Set::build(M::Integer(2), M::Integer(3), M::Integer(5));
    M::MatrixBoolean matrixBoolean = M::MatrixBoolean::build(2, 2, true, false, false, true);
    M::MatrixInteger matrixInteger = M::MatrixInteger::build(2, 2, 6, 7, 8, 9);
    M::MatrixReal    matrixReal    = M::MatrixReal::build(2, 2, 10.0, 11.0, 12.0, 13.0);
    M::MatrixComplex matrixComplex = M::MatrixComplex::build(
        2, 2,
        M::Complex(14, 15), M::Complex(16, 17),
        M::Complex(18, 19), M::Complex(20, 21)
    );

    M::Variant vTuple(tuple);
    M::Variant vSet(set);
    M::Variant vMatrixBoolean(matrixBoolean);
    M::Variant vMatrixInteger(matrixInteger);
    M::Variant vMatrixReal(matrixReal);
    M::Variant vMatrixComplex(matrixComplex);

    QCOMPARE(M::size(tuple), M::size(vTuple));
    QCOMPARE(M::size(set), M::size(vSet));
    QCOMPARE(M::size(matrixBoolean), M::size(vMatrixBoolean));
    QCOMPARE(M::size(matrixInteger), M::size(vMatrixInteger));
    QCOMPARE(M::size(matrixReal), M::size(vMatrixReal));
    QCOMPARE(M::size(matrixComplex), M::size(vMatrixComplex));
}


void TestVariantFunctions::testAlphabet() {
    M::Tuple         tuple         = M::Tuple::build(M::Integer(1), M::Integer(2), M::Integer(4));
    M::Set           set           = M::Set::build(M::Integer(2), M::Integer(3), M::Integer(5));
    M::MatrixBoolean matrixBoolean = M::MatrixBoolean::build(2, 2, true, false, false, true);
    M::MatrixInteger matrixInteger = M::MatrixInteger::build(2, 2, 6, 7, 8, 9);
    M::MatrixReal    matrixReal    = M::MatrixReal::build(2, 2, 10.0, 11.0, 12.0, 13.0);
    M::MatrixComplex matrixComplex = M::MatrixComplex::build(
        2, 2,
        M::Complex(14, 15), M::Complex(16, 17),
        M::Complex(18, 19), M::Complex(20, 21)
    );

    M::Variant vTuple(tuple);
    M::Variant vSet(set);
    M::Variant vMatrixBoolean(matrixBoolean);
    M::Variant vMatrixInteger(matrixInteger);
    M::Variant vMatrixReal(matrixReal);
    M::Variant vMatrixComplex(matrixComplex);

    QCOMPARE(M::alphabet(tuple), M::alphabet(vTuple));
    QCOMPARE(M::alphabet(set), M::alphabet(vSet));
    QCOMPARE(M::alphabet(matrixBoolean), M::alphabet(vMatrixBoolean));
    QCOMPARE(M::alphabet(matrixInteger), M::alphabet(vMatrixInteger));
    QCOMPARE(M::alphabet(matrixReal), M::alphabet(vMatrixReal));
    QCOMPARE(M::alphabet(matrixComplex), M::alphabet(vMatrixComplex));
}


void TestVariantFunctions::testConj() {
    M::Boolean       boolean       = true;
    M::Integer       integer       = 2;
    M::Real          real          = 3.0;
    M::Complex       complex       = M::Complex(4.0, 5.0);
    M::MatrixBoolean matrixBoolean = M::MatrixBoolean::build(2, 2, true, false, false, true);
    M::MatrixInteger matrixInteger = M::MatrixInteger::build(2, 2, 6, 7, 8, 9);
    M::MatrixReal    matrixReal    = M::MatrixReal::build(2, 2, 10.0, 11.0, 12.0, 13.0);
    M::MatrixComplex matrixComplex = M::MatrixComplex::build(
        2, 2,
        M::Complex(14, 15), M::Complex(16, 17),
        M::Complex(18, 19), M::Complex(20, 21)
    );

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);
    M::Variant vMatrixBoolean(matrixBoolean);
    M::Variant vMatrixInteger(matrixInteger);
    M::Variant vMatrixReal(matrixReal);
    M::Variant vMatrixComplex(matrixComplex);

    QCOMPARE(M::conj(boolean), M::conj(vBoolean));
    QCOMPARE(M::conj(integer), M::conj(vInteger));
    QCOMPARE(M::conj(real), M::conj(vReal));
    QCOMPARE(M::conj(complex), M::conj(vComplex));
    QCOMPARE(M::conj(matrixBoolean), M::conj(vMatrixBoolean));
    QCOMPARE(M::conj(matrixInteger), M::conj(vMatrixInteger));
    QCOMPARE(M::conj(matrixReal), M::conj(vMatrixReal));
    QCOMPARE(M::conj(matrixComplex), M::conj(vMatrixComplex));
}
