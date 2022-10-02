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
* This file implements tests of the \ref Model::Variant operators.
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

#include "test_variant_operators.h"

TestVariantOperators::TestVariantOperators() {}

TestVariantOperators::~TestVariantOperators() {}


void TestVariantOperators::testRelationalOperators() {
    M::Boolean boolean = true;
    M::Integer integer = 21;
    M::Real    real    = 20.0;
    M::Complex complex = M::Complex(19.0);

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);

    QCOMPARE(vBoolean == boolean, true);
    QCOMPARE(vBoolean == integer, false);
    QCOMPARE(vBoolean == real, false);
    QCOMPARE(vBoolean == complex, false);

    QCOMPARE(vBoolean != boolean, false);
    QCOMPARE(vBoolean != integer, true);
    QCOMPARE(vBoolean != real, true);
    QCOMPARE(vBoolean != complex, true);

    QCOMPARE(vBoolean < boolean, false);
    QCOMPARE(vBoolean < integer, true);
    QCOMPARE(vBoolean < real, true);
    QCOMPARE(vBoolean < complex, true);

    QCOMPARE(vBoolean > boolean, false);
    QCOMPARE(vBoolean > integer, false);
    QCOMPARE(vBoolean > real, false);
    QCOMPARE(vBoolean > complex, false);

    QCOMPARE(vBoolean <= boolean, true);
    QCOMPARE(vBoolean <= integer, true);
    QCOMPARE(vBoolean <= real, true);
    QCOMPARE(vBoolean <= complex, true);

    QCOMPARE(vBoolean >= boolean, true);
    QCOMPARE(vBoolean >= integer, false);
    QCOMPARE(vBoolean >= real, false);
    QCOMPARE(vBoolean >= complex, false);


    QCOMPARE(boolean == vBoolean, true);
    QCOMPARE(boolean == vInteger, false);
    QCOMPARE(boolean == vReal, false);
    QCOMPARE(boolean == vComplex, false);

    QCOMPARE(boolean != vBoolean, false);
    QCOMPARE(boolean != vInteger, true);
    QCOMPARE(boolean != vReal, true);
    QCOMPARE(boolean != vComplex, true);

    QCOMPARE(boolean < vBoolean, false);
    QCOMPARE(boolean < vInteger, true);
    QCOMPARE(boolean < vComplex, true);

    QCOMPARE(boolean > vBoolean, false);
    QCOMPARE(boolean > vInteger, false);
    QCOMPARE(boolean > vComplex, false);

    QCOMPARE(boolean <= vBoolean, true);
    QCOMPARE(boolean <= vInteger, true);
    QCOMPARE(boolean <= vComplex, true);

    QCOMPARE(boolean >= vBoolean, true);
    QCOMPARE(boolean >= vInteger, false);
    QCOMPARE(boolean >= vComplex, false);


    QCOMPARE(vInteger == boolean, false);
    QCOMPARE(vInteger == integer, true);
    QCOMPARE(vInteger == real, false);
    QCOMPARE(vInteger == complex, false);

    QCOMPARE(vInteger != boolean, true);
    QCOMPARE(vInteger != integer, false);
    QCOMPARE(vInteger != real, true);
    QCOMPARE(vInteger != complex, true);

    QCOMPARE(vInteger < boolean, false);
    QCOMPARE(vInteger < integer, false);
    QCOMPARE(vInteger < real, false);
    QCOMPARE(vInteger < complex, false);

    QCOMPARE(vInteger > boolean, true);
    QCOMPARE(vInteger > integer, false);
    QCOMPARE(vInteger > real, true);
    QCOMPARE(vInteger > complex, true);

    QCOMPARE(vInteger <= boolean, false);
    QCOMPARE(vInteger <= integer, true);
    QCOMPARE(vInteger <= real, false);
    QCOMPARE(vInteger <= complex, false);

    QCOMPARE(vInteger >= boolean, true);
    QCOMPARE(vInteger >= integer, true);
    QCOMPARE(vInteger >= real, true);
    QCOMPARE(vInteger >= complex, true);


    QCOMPARE(integer == vBoolean, false);
    QCOMPARE(integer == vInteger, true);
    QCOMPARE(integer == vReal, false);
    QCOMPARE(integer == vComplex, false);

    QCOMPARE(integer != vBoolean, true);
    QCOMPARE(integer != vInteger, false);
    QCOMPARE(integer != vReal, true);
    QCOMPARE(integer != vComplex, true);

    QCOMPARE(integer < vBoolean, false);
    QCOMPARE(integer < vInteger, false);
    QCOMPARE(integer < vReal, false);
    QCOMPARE(integer < vComplex, false);

    QCOMPARE(integer > vBoolean, true);
    QCOMPARE(integer > vInteger, false);
    QCOMPARE(integer > vReal, true);
    QCOMPARE(integer > vComplex, true);

    QCOMPARE(integer <= vBoolean, false);
    QCOMPARE(integer <= vInteger, true);
    QCOMPARE(integer <= vReal, false);
    QCOMPARE(integer <= vComplex, false);

    QCOMPARE(integer >= vBoolean, true);
    QCOMPARE(integer >= vInteger, true);
    QCOMPARE(integer >= vReal, true);
    QCOMPARE(integer >= vComplex, true);


    QCOMPARE(vReal == boolean, false);
    QCOMPARE(vReal == integer, false);
    QCOMPARE(vReal == real, true);
    QCOMPARE(vReal == complex, false);

    QCOMPARE(vReal != boolean, true);
    QCOMPARE(vReal != integer, true);
    QCOMPARE(vReal != real, false);
    QCOMPARE(vReal != complex, true);

    QCOMPARE(vReal < boolean, false);
    QCOMPARE(vReal < integer, true);
    QCOMPARE(vReal < real, false);
    QCOMPARE(vReal < complex, false);

    QCOMPARE(vReal > boolean, true);
    QCOMPARE(vReal > integer, false);
    QCOMPARE(vReal > real, false);
    QCOMPARE(vReal > complex, true);

    QCOMPARE(vReal <= boolean, false);
    QCOMPARE(vReal <= integer, true);
    QCOMPARE(vReal <= real, true);
    QCOMPARE(vReal <= complex, false);

    QCOMPARE(vReal >= boolean, true);
    QCOMPARE(vReal >= integer, false);
    QCOMPARE(vReal >= real, true);
    QCOMPARE(vReal >= complex, true);


    QCOMPARE(real == vBoolean, false);
    QCOMPARE(real == vInteger, false);
    QCOMPARE(real == vReal, true);
    QCOMPARE(real == vComplex, false);

    QCOMPARE(real != vBoolean, true);
    QCOMPARE(real != vInteger, true);
    QCOMPARE(real != vReal, false);
    QCOMPARE(real != vComplex, true);

    QCOMPARE(real < vBoolean, false);
    QCOMPARE(real < vInteger, true);
    QCOMPARE(real < vReal, false);
    QCOMPARE(real < vComplex, false);

    QCOMPARE(real > vBoolean, true);
    QCOMPARE(real > vInteger, false);
    QCOMPARE(real > vReal, false);
    QCOMPARE(real > vComplex, true);

    QCOMPARE(real <= vBoolean, false);
    QCOMPARE(real <= vInteger, true);
    QCOMPARE(real <= vReal, true);
    QCOMPARE(real <= vComplex, false);

    QCOMPARE(real >= vBoolean, true);
    QCOMPARE(real >= vInteger, false);
    QCOMPARE(real >= vReal, true);
    QCOMPARE(real >= vComplex, true);


    QCOMPARE(vComplex == boolean, false);
    QCOMPARE(vComplex == integer, false);
    QCOMPARE(vComplex == real, false);
    QCOMPARE(vComplex == complex, true);

    QCOMPARE(vComplex != boolean, true);
    QCOMPARE(vComplex != integer, true);
    QCOMPARE(vComplex != real, true);
    QCOMPARE(vComplex != complex, false);

    QCOMPARE(vComplex < boolean, false);
    QCOMPARE(vComplex < integer, true);
    QCOMPARE(vComplex < real, true);
    QCOMPARE(vComplex < complex, false);

    QCOMPARE(vComplex > boolean, true);
    QCOMPARE(vComplex > integer, false);
    QCOMPARE(vComplex > real, false);
    QCOMPARE(vComplex > complex, false);

    QCOMPARE(vComplex <= boolean, false);
    QCOMPARE(vComplex <= integer, true);
    QCOMPARE(vComplex <= real, true);
    QCOMPARE(vComplex <= complex, true);

    QCOMPARE(vComplex >= boolean, true);
    QCOMPARE(vComplex >= integer, false);
    QCOMPARE(vComplex >= real, false);
    QCOMPARE(vComplex >= complex, true);


    QCOMPARE(complex == vBoolean, false);
    QCOMPARE(complex == vInteger, false);
    QCOMPARE(complex == vReal, false);
    QCOMPARE(complex == vComplex, true);

    QCOMPARE(complex != vBoolean, true);
    QCOMPARE(complex != vInteger, true);
    QCOMPARE(complex != vReal, true);
    QCOMPARE(complex != vComplex, false);

    QCOMPARE(complex < vBoolean, false);
    QCOMPARE(complex < vInteger, true);
    QCOMPARE(complex < vReal, true);
    QCOMPARE(complex < vComplex, false);

    QCOMPARE(complex > vBoolean, true);
    QCOMPARE(complex > vInteger, false);
    QCOMPARE(complex > vReal, false);
    QCOMPARE(complex > vComplex, false);

    QCOMPARE(complex <= vBoolean, false);
    QCOMPARE(complex <= vInteger, true);
    QCOMPARE(complex <= vReal, true);
    QCOMPARE(complex <= vComplex, true);

    QCOMPARE(complex >= vBoolean, true);
    QCOMPARE(complex >= vInteger, false);
    QCOMPARE(complex >= vReal, false);
    QCOMPARE(complex >= vComplex, true);
}


void TestVariantOperators::testAddition() {
    M::Boolean       boolean       = true;
    M::Integer       integer       = 2;
    M::Real          real          = 3.0;
    M::Complex       complex       = M::Complex(4.0, 5.0);
    M::Tuple         tuple         = M::Tuple::build(M::Integer(1), M::Integer(2), M::Integer(4));
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
    M::Variant vTuple(tuple);
    M::Variant vMatrixBoolean(matrixBoolean);
    M::Variant vMatrixInteger(matrixInteger);
    M::Variant vMatrixReal(matrixReal);
    M::Variant vMatrixComplex(matrixComplex);

//    QCOMPARE(boolean + vBoolean, M::Integer(boolean + boolean));
    QCOMPARE(boolean + vInteger, boolean + integer);
    QCOMPARE(boolean + vReal,    boolean + real);
    QCOMPARE(boolean + vComplex, boolean + complex);

    QCOMPARE(integer + vBoolean, integer + boolean);
    QCOMPARE(integer + vInteger, integer + integer);
    QCOMPARE(integer + vReal, integer + real);
    QCOMPARE(integer + vComplex, integer + complex);

    QCOMPARE(real + vBoolean, real + boolean);
    QCOMPARE(real + vInteger, real + integer);
    QCOMPARE(real + vReal, real + real);
    QCOMPARE(real + vComplex, real + complex);

    QCOMPARE(complex + vBoolean, complex + boolean);
    QCOMPARE(complex + vInteger, complex + integer);
    QCOMPARE(complex + vReal, complex + real);
    QCOMPARE(complex + vComplex, complex + complex);

    QCOMPARE(matrixBoolean + vMatrixBoolean, matrixBoolean + matrixBoolean);
    QCOMPARE(matrixBoolean + vMatrixInteger, matrixBoolean + matrixInteger);
    QCOMPARE(matrixBoolean + vMatrixReal, matrixBoolean + matrixReal);
    QCOMPARE(matrixBoolean + vMatrixComplex, matrixBoolean + matrixComplex);

    QCOMPARE(matrixInteger + vMatrixBoolean, matrixInteger + matrixBoolean);
    QCOMPARE(matrixInteger + vMatrixInteger, matrixInteger + matrixInteger);
    QCOMPARE(matrixInteger + vMatrixReal, matrixInteger + matrixReal);
    QCOMPARE(matrixInteger + vMatrixComplex, matrixInteger + matrixComplex);

    QCOMPARE(matrixReal + vMatrixBoolean, matrixReal + matrixBoolean);
    QCOMPARE(matrixReal + vMatrixInteger, matrixReal + matrixInteger);
    QCOMPARE(matrixReal + vMatrixReal, matrixReal + matrixReal);
    QCOMPARE(matrixReal + vMatrixComplex, matrixReal + matrixComplex);

    QCOMPARE(matrixComplex + vMatrixBoolean, matrixComplex + matrixBoolean);
    QCOMPARE(matrixComplex + vMatrixInteger, matrixComplex + matrixInteger);
    QCOMPARE(matrixComplex + vMatrixReal, matrixComplex + matrixReal);
    QCOMPARE(matrixComplex + vMatrixComplex, matrixComplex + matrixComplex);


    QCOMPARE(vBoolean + boolean, M::Integer(boolean + boolean));
    QCOMPARE(vBoolean + integer, boolean + integer);
    QCOMPARE(vBoolean + real,    boolean + real);
    QCOMPARE(vBoolean + complex, boolean + complex);

    QCOMPARE(vInteger + boolean, integer + boolean);
    QCOMPARE(vInteger + integer, integer + integer);
    QCOMPARE(vInteger + real, integer + real);
    QCOMPARE(vInteger + complex, integer + complex);

    QCOMPARE(vReal + boolean, real + boolean);
    QCOMPARE(vReal + integer, real + integer);
    QCOMPARE(vReal + real, real + real);
    QCOMPARE(vReal + complex, real + complex);

    QCOMPARE(vComplex + boolean, complex + boolean);
    QCOMPARE(vComplex + integer, complex + integer);
    QCOMPARE(vComplex + real, complex + real);
    QCOMPARE(vComplex + complex, complex + complex);

    QCOMPARE(vMatrixBoolean + matrixBoolean, matrixBoolean + matrixBoolean);
    QCOMPARE(vMatrixBoolean + matrixInteger, matrixBoolean + matrixInteger);
    QCOMPARE(vMatrixBoolean + matrixReal, matrixBoolean + matrixReal);
    QCOMPARE(vMatrixBoolean + matrixComplex, matrixBoolean + matrixComplex);

    QCOMPARE(vMatrixInteger + matrixBoolean, matrixInteger + matrixBoolean);
    QCOMPARE(vMatrixInteger + matrixInteger, matrixInteger + matrixInteger);
    QCOMPARE(vMatrixInteger + matrixReal, matrixInteger + matrixReal);
    QCOMPARE(vMatrixInteger + matrixComplex, matrixInteger + matrixComplex);

    QCOMPARE(vMatrixReal + matrixBoolean, matrixReal + matrixBoolean);
    QCOMPARE(vMatrixReal + matrixInteger, matrixReal + matrixInteger);
    QCOMPARE(vMatrixReal + matrixReal, matrixReal + matrixReal);
    QCOMPARE(vMatrixReal + matrixComplex, matrixReal + matrixComplex);

    QCOMPARE(vMatrixComplex + matrixBoolean, matrixComplex + matrixBoolean);
    QCOMPARE(vMatrixComplex + matrixInteger, matrixComplex + matrixInteger);
    QCOMPARE(vMatrixComplex + matrixReal, matrixComplex + matrixReal);
    QCOMPARE(vMatrixComplex + matrixComplex, matrixComplex + matrixComplex);


    QCOMPARE(vBoolean + vBoolean, M::Integer(boolean + boolean));
    QCOMPARE(vBoolean + vInteger, boolean + integer);
    QCOMPARE(vBoolean + vReal,    boolean + real);
    QCOMPARE(vBoolean + vComplex, boolean + complex);

    QCOMPARE(vInteger + vBoolean, integer + boolean);
    QCOMPARE(vInteger + vInteger, integer + integer);
    QCOMPARE(vInteger + vReal, integer + real);
    QCOMPARE(vInteger + vComplex, integer + complex);

    QCOMPARE(vReal + vBoolean, real + boolean);
    QCOMPARE(vReal + vInteger, real + integer);
    QCOMPARE(vReal + vReal, real + real);
    QCOMPARE(vReal + vComplex, real + complex);

    QCOMPARE(vComplex + vBoolean, complex + boolean);
    QCOMPARE(vComplex + vInteger, complex + integer);
    QCOMPARE(vComplex + vReal, complex + real);
    QCOMPARE(vComplex + vComplex, complex + complex);

    QCOMPARE(vMatrixBoolean + vMatrixBoolean, matrixBoolean + matrixBoolean);
    QCOMPARE(vMatrixBoolean + vMatrixInteger, matrixBoolean + matrixInteger);
    QCOMPARE(vMatrixBoolean + vMatrixReal, matrixBoolean + matrixReal);
    QCOMPARE(vMatrixBoolean + vMatrixComplex, matrixBoolean + matrixComplex);

    QCOMPARE(vMatrixInteger + vMatrixBoolean, matrixInteger + matrixBoolean);
    QCOMPARE(vMatrixInteger + vMatrixInteger, matrixInteger + matrixInteger);
    QCOMPARE(vMatrixInteger + vMatrixReal, matrixInteger + matrixReal);
    QCOMPARE(vMatrixInteger + vMatrixComplex, matrixInteger + matrixComplex);

    QCOMPARE(vMatrixReal + vMatrixBoolean, matrixReal + matrixBoolean);
    QCOMPARE(vMatrixReal + vMatrixInteger, matrixReal + matrixInteger);
    QCOMPARE(vMatrixReal + vMatrixReal, matrixReal + matrixReal);
    QCOMPARE(vMatrixReal + vMatrixComplex, matrixReal + matrixComplex);

    QCOMPARE(vMatrixComplex + vMatrixBoolean, matrixComplex + matrixBoolean);
    QCOMPARE(vMatrixComplex + vMatrixInteger, matrixComplex + matrixInteger);
    QCOMPARE(vMatrixComplex + vMatrixReal, matrixComplex + matrixReal);
    QCOMPARE(vMatrixComplex + vMatrixComplex, matrixComplex + matrixComplex);
}


void TestVariantOperators::testSubtraction() {
    M::Boolean       boolean       = true;
    M::Integer       integer       = 2;
    M::Real          real          = 3.0;
    M::Complex       complex       = M::Complex(4.0, 5.0);
    M::Tuple         tuple         = M::Tuple::build(M::Integer(1), M::Integer(2), M::Integer(4));
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
    M::Variant vTuple(tuple);
    M::Variant vMatrixBoolean(matrixBoolean);
    M::Variant vMatrixInteger(matrixInteger);
    M::Variant vMatrixReal(matrixReal);
    M::Variant vMatrixComplex(matrixComplex);

    QCOMPARE(boolean - vBoolean, M::Integer(boolean - boolean));
    QCOMPARE(boolean - vInteger, boolean - integer);
    QCOMPARE(boolean - vReal,    boolean - real);
    QCOMPARE(boolean - vComplex, boolean - complex);

    QCOMPARE(integer - vBoolean, integer - boolean);
    QCOMPARE(integer - vInteger, integer - integer);
    QCOMPARE(integer - vReal, integer - real);
    QCOMPARE(integer - vComplex, integer - complex);

    QCOMPARE(real - vBoolean, real - boolean);
    QCOMPARE(real - vInteger, real - integer);
    QCOMPARE(real - vReal, real - real);
    QCOMPARE(real - vComplex, real - complex);

    QCOMPARE(complex - vBoolean, complex - boolean);
    QCOMPARE(complex - vInteger, complex - integer);
    QCOMPARE(complex - vReal, complex - real);
    QCOMPARE(complex - vComplex, complex - complex);

    QCOMPARE(matrixBoolean - vMatrixBoolean, matrixBoolean - matrixBoolean);
    QCOMPARE(matrixBoolean - vMatrixInteger, matrixBoolean - matrixInteger);
    QCOMPARE(matrixBoolean - vMatrixReal, matrixBoolean - matrixReal);
    QCOMPARE(matrixBoolean - vMatrixComplex, matrixBoolean - matrixComplex);

    QCOMPARE(matrixInteger - vMatrixBoolean, matrixInteger - matrixBoolean);
    QCOMPARE(matrixInteger - vMatrixInteger, matrixInteger - matrixInteger);
    QCOMPARE(matrixInteger - vMatrixReal, matrixInteger - matrixReal);
    QCOMPARE(matrixInteger - vMatrixComplex, matrixInteger - matrixComplex);

    QCOMPARE(matrixReal - vMatrixBoolean, matrixReal - matrixBoolean);
    QCOMPARE(matrixReal - vMatrixInteger, matrixReal - matrixInteger);
    QCOMPARE(matrixReal - vMatrixReal, matrixReal - matrixReal);
    QCOMPARE(matrixReal - vMatrixComplex, matrixReal - matrixComplex);

    QCOMPARE(matrixComplex - vMatrixBoolean, matrixComplex - matrixBoolean);
    QCOMPARE(matrixComplex - vMatrixInteger, matrixComplex - matrixInteger);
    QCOMPARE(matrixComplex - vMatrixReal, matrixComplex - matrixReal);
    QCOMPARE(matrixComplex - vMatrixComplex, matrixComplex - matrixComplex);


    QCOMPARE(vBoolean - boolean, M::Integer(boolean - boolean));
    QCOMPARE(vBoolean - integer, boolean - integer);
    QCOMPARE(vBoolean - real,    boolean - real);
    QCOMPARE(vBoolean - complex, boolean - complex);

    QCOMPARE(vInteger - boolean, integer - boolean);
    QCOMPARE(vInteger - integer, integer - integer);
    QCOMPARE(vInteger - real, integer - real);
    QCOMPARE(vInteger - complex, integer - complex);

    QCOMPARE(vReal - boolean, real - boolean);
    QCOMPARE(vReal - integer, real - integer);
    QCOMPARE(vReal - real, real - real);
    QCOMPARE(vReal - complex, real - complex);

    QCOMPARE(vComplex - boolean, complex - boolean);
    QCOMPARE(vComplex - integer, complex - integer);
    QCOMPARE(vComplex - real, complex - real);
    QCOMPARE(vComplex - complex, complex - complex);

    QCOMPARE(vMatrixBoolean - matrixBoolean, matrixBoolean - matrixBoolean);
    QCOMPARE(vMatrixBoolean - matrixInteger, matrixBoolean - matrixInteger);
    QCOMPARE(vMatrixBoolean - matrixReal, matrixBoolean - matrixReal);
    QCOMPARE(vMatrixBoolean - matrixComplex, matrixBoolean - matrixComplex);

    QCOMPARE(vMatrixInteger - matrixBoolean, matrixInteger - matrixBoolean);
    QCOMPARE(vMatrixInteger - matrixInteger, matrixInteger - matrixInteger);
    QCOMPARE(vMatrixInteger - matrixReal, matrixInteger - matrixReal);
    QCOMPARE(vMatrixInteger - matrixComplex, matrixInteger - matrixComplex);

    QCOMPARE(vMatrixReal - matrixBoolean, matrixReal - matrixBoolean);
    QCOMPARE(vMatrixReal - matrixInteger, matrixReal - matrixInteger);
    QCOMPARE(vMatrixReal - matrixReal, matrixReal - matrixReal);
    QCOMPARE(vMatrixReal - matrixComplex, matrixReal - matrixComplex);

    QCOMPARE(vMatrixComplex - matrixBoolean, matrixComplex - matrixBoolean);
    QCOMPARE(vMatrixComplex - matrixInteger, matrixComplex - matrixInteger);
    QCOMPARE(vMatrixComplex - matrixReal, matrixComplex - matrixReal);
    QCOMPARE(vMatrixComplex - matrixComplex, matrixComplex - matrixComplex);


    QCOMPARE(vBoolean - vBoolean, M::Integer(boolean - boolean));
    QCOMPARE(vBoolean - vInteger, boolean - integer);
    QCOMPARE(vBoolean - vReal,    boolean - real);
    QCOMPARE(vBoolean - vComplex, boolean - complex);

    QCOMPARE(vInteger - vBoolean, integer - boolean);
    QCOMPARE(vInteger - vInteger, integer - integer);
    QCOMPARE(vInteger - vReal, integer - real);
    QCOMPARE(vInteger - vComplex, integer - complex);

    QCOMPARE(vReal - vBoolean, real - boolean);
    QCOMPARE(vReal - vInteger, real - integer);
    QCOMPARE(vReal - vReal, real - real);
    QCOMPARE(vReal - vComplex, real - complex);

    QCOMPARE(vComplex - vBoolean, complex - boolean);
    QCOMPARE(vComplex - vInteger, complex - integer);
    QCOMPARE(vComplex - vReal, complex - real);
    QCOMPARE(vComplex - vComplex, complex - complex);

    QCOMPARE(vMatrixBoolean - vMatrixBoolean, matrixBoolean - matrixBoolean);
    QCOMPARE(vMatrixBoolean - vMatrixInteger, matrixBoolean - matrixInteger);
    QCOMPARE(vMatrixBoolean - vMatrixReal, matrixBoolean - matrixReal);
    QCOMPARE(vMatrixBoolean - vMatrixComplex, matrixBoolean - matrixComplex);

    QCOMPARE(vMatrixInteger - vMatrixBoolean, matrixInteger - matrixBoolean);
    QCOMPARE(vMatrixInteger - vMatrixInteger, matrixInteger - matrixInteger);
    QCOMPARE(vMatrixInteger - vMatrixReal, matrixInteger - matrixReal);
    QCOMPARE(vMatrixInteger - vMatrixComplex, matrixInteger - matrixComplex);

    QCOMPARE(vMatrixReal - vMatrixBoolean, matrixReal - matrixBoolean);
    QCOMPARE(vMatrixReal - vMatrixInteger, matrixReal - matrixInteger);
    QCOMPARE(vMatrixReal - vMatrixReal, matrixReal - matrixReal);
    QCOMPARE(vMatrixReal - vMatrixComplex, matrixReal - matrixComplex);

    QCOMPARE(vMatrixComplex - vMatrixBoolean, matrixComplex - matrixBoolean);
    QCOMPARE(vMatrixComplex - vMatrixInteger, matrixComplex - matrixInteger);
    QCOMPARE(vMatrixComplex - vMatrixReal, matrixComplex - matrixReal);
    QCOMPARE(vMatrixComplex - vMatrixComplex, matrixComplex - matrixComplex);
}


void TestVariantOperators::testMultiplication() {
    M::Boolean       boolean       = true;
    M::Integer       integer       = 2;
    M::Real          real          = 3.0;
    M::Complex       complex       = M::Complex(4.0, 5.0);
    M::Tuple         tuple         = M::Tuple::build(M::Integer(1), M::Integer(2), M::Integer(4));
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
    M::Variant vTuple(tuple);
    M::Variant vMatrixBoolean(matrixBoolean);
    M::Variant vMatrixInteger(matrixInteger);
    M::Variant vMatrixReal(matrixReal);
    M::Variant vMatrixComplex(matrixComplex);

    QCOMPARE(boolean * vBoolean, M::Integer(boolean * boolean));
    QCOMPARE(boolean * vInteger, boolean * integer);
    QCOMPARE(boolean * vReal,    boolean * real);
    QCOMPARE(boolean * vComplex, boolean * complex);
    QCOMPARE(boolean * vMatrixBoolean, boolean * matrixBoolean);
    QCOMPARE(boolean * vMatrixInteger, boolean * matrixInteger);
    QCOMPARE(boolean * vMatrixReal, boolean * matrixReal);
    QCOMPARE(boolean * vMatrixComplex, boolean * matrixComplex);

    QCOMPARE(integer * vBoolean, integer * boolean);
    QCOMPARE(integer * vInteger, integer * integer);
    QCOMPARE(integer * vReal, integer * real);
    QCOMPARE(integer * vComplex, integer * complex);
    QCOMPARE(integer * vMatrixBoolean, integer * matrixBoolean);
    QCOMPARE(integer * vMatrixInteger, integer * matrixInteger);
    QCOMPARE(integer * vMatrixReal, integer * matrixReal);
    QCOMPARE(integer * vMatrixComplex, integer * matrixComplex);

    QCOMPARE(real * vBoolean, real * boolean);
    QCOMPARE(real * vInteger, real * integer);
    QCOMPARE(real * vReal, real * real);
    QCOMPARE(real * vComplex, real * complex);
    QCOMPARE(real * vMatrixBoolean, real * matrixBoolean);
    QCOMPARE(real * vMatrixInteger, real * matrixInteger);
    QCOMPARE(real * vMatrixReal, real * matrixReal);
    QCOMPARE(real * vMatrixComplex, real * matrixComplex);

    QCOMPARE(complex * vBoolean, complex * boolean);
    QCOMPARE(complex * vInteger, complex * integer);
    QCOMPARE(complex * vReal, complex * real);
    QCOMPARE(complex * vComplex, complex * complex);
    QCOMPARE(complex * vMatrixBoolean, complex * matrixBoolean);
    QCOMPARE(complex * vMatrixInteger, complex * matrixInteger);
    QCOMPARE(complex * vMatrixReal, complex * matrixReal);
    QCOMPARE(complex * vMatrixComplex, complex * matrixComplex);

    QCOMPARE(tuple * vTuple, tuple * tuple);

    QCOMPARE(matrixBoolean * vBoolean, matrixBoolean * boolean);
    QCOMPARE(matrixBoolean * vInteger, matrixBoolean * integer);
    QCOMPARE(matrixBoolean * vReal, matrixBoolean * real);
    QCOMPARE(matrixBoolean * vComplex, matrixBoolean * complex);
    QCOMPARE(matrixBoolean * vMatrixBoolean, matrixBoolean * matrixBoolean);
    QCOMPARE(matrixBoolean * vMatrixInteger, matrixBoolean * matrixInteger);
    QCOMPARE(matrixBoolean * vMatrixReal, matrixBoolean * matrixReal);
    QCOMPARE(matrixBoolean * vMatrixComplex, matrixBoolean * matrixComplex);

    QCOMPARE(matrixInteger * vBoolean, matrixInteger * boolean);
    QCOMPARE(matrixInteger * vInteger, matrixInteger * integer);
    QCOMPARE(matrixInteger * vReal, matrixInteger * real);
    QCOMPARE(matrixInteger * vComplex, matrixInteger * complex);
    QCOMPARE(matrixInteger * vMatrixBoolean, matrixInteger * matrixBoolean);
    QCOMPARE(matrixInteger * vMatrixInteger, matrixInteger * matrixInteger);
    QCOMPARE(matrixInteger * vMatrixReal, matrixInteger * matrixReal);
    QCOMPARE(matrixInteger * vMatrixComplex, matrixInteger * matrixComplex);

    QCOMPARE(matrixReal * vBoolean, matrixReal * boolean);
    QCOMPARE(matrixReal * vInteger, matrixReal * integer);
    QCOMPARE(matrixReal * vReal, matrixReal * real);
    QCOMPARE(matrixReal * vComplex, matrixReal * complex);
    QCOMPARE(matrixReal * vMatrixBoolean, matrixReal * matrixBoolean);
    QCOMPARE(matrixReal * vMatrixInteger, matrixReal * matrixInteger);
    QCOMPARE(matrixReal * vMatrixReal, matrixReal * matrixReal);
    QCOMPARE(matrixReal * vMatrixComplex, matrixReal * matrixComplex);

    QCOMPARE(matrixComplex * vBoolean, matrixComplex * boolean);
    QCOMPARE(matrixComplex * vInteger, matrixComplex * integer);
    QCOMPARE(matrixComplex * vReal, matrixComplex * real);
    QCOMPARE(matrixComplex * vComplex, matrixComplex * complex);
    QCOMPARE(matrixComplex * vMatrixBoolean, matrixComplex * matrixBoolean);
    QCOMPARE(matrixComplex * vMatrixInteger, matrixComplex * matrixInteger);
    QCOMPARE(matrixComplex * vMatrixReal, matrixComplex * matrixReal);
    QCOMPARE(matrixComplex * vMatrixComplex, matrixComplex * matrixComplex);


    QCOMPARE(vBoolean * boolean, M::Integer(boolean * boolean));
    QCOMPARE(vBoolean * integer, boolean * integer);
    QCOMPARE(vBoolean * real,    boolean * real);
    QCOMPARE(vBoolean * complex, boolean * complex);
    QCOMPARE(vBoolean * matrixBoolean, boolean * matrixBoolean);
    QCOMPARE(vBoolean * matrixInteger, boolean * matrixInteger);
    QCOMPARE(vBoolean * matrixReal, boolean * matrixReal);
    QCOMPARE(vBoolean * matrixComplex, boolean * matrixComplex);

    QCOMPARE(vInteger * boolean, integer * boolean);
    QCOMPARE(vInteger * integer, integer * integer);
    QCOMPARE(vInteger * real, integer * real);
    QCOMPARE(vInteger * complex, integer * complex);
    QCOMPARE(vInteger * matrixBoolean, integer * matrixBoolean);
    QCOMPARE(vInteger * matrixInteger, integer * matrixInteger);
    QCOMPARE(vInteger * matrixReal, integer * matrixReal);
    QCOMPARE(vInteger * matrixComplex, integer * matrixComplex);

    QCOMPARE(vReal * boolean, real * boolean);
    QCOMPARE(vReal * integer, real * integer);
    QCOMPARE(vReal * real, real * real);
    QCOMPARE(vReal * complex, real * complex);
    QCOMPARE(vReal * matrixBoolean, real * matrixBoolean);
    QCOMPARE(vReal * matrixInteger, real * matrixInteger);
    QCOMPARE(vReal * matrixReal, real * matrixReal);
    QCOMPARE(vReal * matrixComplex, real * matrixComplex);

    QCOMPARE(vComplex * boolean, complex * boolean);
    QCOMPARE(vComplex * integer, complex * integer);
    QCOMPARE(vComplex * real, complex * real);
    QCOMPARE(vComplex * complex, complex * complex);
    QCOMPARE(vComplex * matrixBoolean, complex * matrixBoolean);
    QCOMPARE(vComplex * matrixInteger, complex * matrixInteger);
    QCOMPARE(vComplex * matrixReal, complex * matrixReal);
    QCOMPARE(vComplex * matrixComplex, complex * matrixComplex);

    QCOMPARE(vTuple * vTuple, tuple * tuple);

    QCOMPARE(vMatrixBoolean * boolean, matrixBoolean * boolean);
    QCOMPARE(vMatrixBoolean * integer, matrixBoolean * integer);
    QCOMPARE(vMatrixBoolean * real, matrixBoolean * real);
    QCOMPARE(vMatrixBoolean * complex, matrixBoolean * complex);
    QCOMPARE(vMatrixBoolean * matrixBoolean, matrixBoolean * matrixBoolean);
    QCOMPARE(vMatrixBoolean * matrixInteger, matrixBoolean * matrixInteger);
    QCOMPARE(vMatrixBoolean * matrixReal, matrixBoolean * matrixReal);
    QCOMPARE(vMatrixBoolean * matrixComplex, matrixBoolean * matrixComplex);

    QCOMPARE(vMatrixInteger * boolean, matrixInteger * boolean);
    QCOMPARE(vMatrixInteger * integer, matrixInteger * integer);
    QCOMPARE(vMatrixInteger * real, matrixInteger * real);
    QCOMPARE(vMatrixInteger * complex, matrixInteger * complex);
    QCOMPARE(vMatrixInteger * matrixBoolean, matrixInteger * matrixBoolean);
    QCOMPARE(vMatrixInteger * matrixInteger, matrixInteger * matrixInteger);
    QCOMPARE(vMatrixInteger * matrixReal, matrixInteger * matrixReal);
    QCOMPARE(vMatrixInteger * matrixComplex, matrixInteger * matrixComplex);

    QCOMPARE(vMatrixReal * boolean, matrixReal * boolean);
    QCOMPARE(vMatrixReal * integer, matrixReal * integer);
    QCOMPARE(vMatrixReal * real, matrixReal * real);
    QCOMPARE(vMatrixReal * complex, matrixReal * complex);
    QCOMPARE(vMatrixReal * matrixBoolean, matrixReal * matrixBoolean);
    QCOMPARE(vMatrixReal * matrixInteger, matrixReal * matrixInteger);
    QCOMPARE(vMatrixReal * matrixReal, matrixReal * matrixReal);
    QCOMPARE(vMatrixReal * matrixComplex, matrixReal * matrixComplex);

    QCOMPARE(vMatrixComplex * boolean, matrixComplex * boolean);
    QCOMPARE(vMatrixComplex * integer, matrixComplex * integer);
    QCOMPARE(vMatrixComplex * real, matrixComplex * real);
    QCOMPARE(vMatrixComplex * complex, matrixComplex * complex);
    QCOMPARE(vMatrixComplex * matrixBoolean, matrixComplex * matrixBoolean);
    QCOMPARE(vMatrixComplex * matrixInteger, matrixComplex * matrixInteger);
    QCOMPARE(vMatrixComplex * matrixReal, matrixComplex * matrixReal);
    QCOMPARE(vMatrixComplex * matrixComplex, matrixComplex * matrixComplex);


    QCOMPARE(vBoolean * vBoolean, M::Integer(boolean * boolean));
    QCOMPARE(vBoolean * vInteger, boolean * integer);
    QCOMPARE(vBoolean * vReal,    boolean * real);
    QCOMPARE(vBoolean * vComplex, boolean * complex);
    QCOMPARE(vBoolean * vMatrixBoolean, boolean * matrixBoolean);
    QCOMPARE(vBoolean * vMatrixInteger, boolean * matrixInteger);
    QCOMPARE(vBoolean * vMatrixReal, boolean * matrixReal);
    QCOMPARE(vBoolean * vMatrixComplex, boolean * matrixComplex);

    QCOMPARE(vInteger * vBoolean, integer * boolean);
    QCOMPARE(vInteger * vInteger, integer * integer);
    QCOMPARE(vInteger * vReal, integer * real);
    QCOMPARE(vInteger * vComplex, integer * complex);
    QCOMPARE(vInteger * vMatrixBoolean, integer * matrixBoolean);
    QCOMPARE(vInteger * vMatrixInteger, integer * matrixInteger);
    QCOMPARE(vInteger * vMatrixReal, integer * matrixReal);
    QCOMPARE(vInteger * vMatrixComplex, integer * matrixComplex);

    QCOMPARE(vReal * vBoolean, real * boolean);
    QCOMPARE(vReal * vInteger, real * integer);
    QCOMPARE(vReal * vReal, real * real);
    QCOMPARE(vReal * vComplex, real * complex);
    QCOMPARE(vReal * vMatrixBoolean, real * matrixBoolean);
    QCOMPARE(vReal * vMatrixInteger, real * matrixInteger);
    QCOMPARE(vReal * vMatrixReal, real * matrixReal);
    QCOMPARE(vReal * vMatrixComplex, real * matrixComplex);

    QCOMPARE(vComplex * vBoolean, complex * boolean);
    QCOMPARE(vComplex * vInteger, complex * integer);
    QCOMPARE(vComplex * vReal, complex * real);
    QCOMPARE(vComplex * vComplex, complex * complex);
    QCOMPARE(vComplex * vMatrixBoolean, complex * matrixBoolean);
    QCOMPARE(vComplex * vMatrixInteger, complex * matrixInteger);
    QCOMPARE(vComplex * vMatrixReal, complex * matrixReal);
    QCOMPARE(vComplex * vMatrixComplex, complex * matrixComplex);

    QCOMPARE(vTuple * vTuple, tuple * tuple);

    QCOMPARE(vMatrixBoolean * vBoolean, matrixBoolean * boolean);
    QCOMPARE(vMatrixBoolean * vInteger, matrixBoolean * integer);
    QCOMPARE(vMatrixBoolean * vReal, matrixBoolean * real);
    QCOMPARE(vMatrixBoolean * vComplex, matrixBoolean * complex);
    QCOMPARE(vMatrixBoolean * vMatrixBoolean, matrixBoolean * matrixBoolean);
    QCOMPARE(vMatrixBoolean * vMatrixInteger, matrixBoolean * matrixInteger);
    QCOMPARE(vMatrixBoolean * vMatrixReal, matrixBoolean * matrixReal);
    QCOMPARE(vMatrixBoolean * vMatrixComplex, matrixBoolean * matrixComplex);

    QCOMPARE(vMatrixInteger * vBoolean, matrixInteger * boolean);
    QCOMPARE(vMatrixInteger * vInteger, matrixInteger * integer);
    QCOMPARE(vMatrixInteger * vReal, matrixInteger * real);
    QCOMPARE(vMatrixInteger * vComplex, matrixInteger * complex);
    QCOMPARE(vMatrixInteger * vMatrixBoolean, matrixInteger * matrixBoolean);
    QCOMPARE(vMatrixInteger * vMatrixInteger, matrixInteger * matrixInteger);
    QCOMPARE(vMatrixInteger * vMatrixReal, matrixInteger * matrixReal);
    QCOMPARE(vMatrixInteger * vMatrixComplex, matrixInteger * matrixComplex);

    QCOMPARE(vMatrixReal * vBoolean, matrixReal * boolean);
    QCOMPARE(vMatrixReal * vInteger, matrixReal * integer);
    QCOMPARE(vMatrixReal * vReal, matrixReal * real);
    QCOMPARE(vMatrixReal * vComplex, matrixReal * complex);
    QCOMPARE(vMatrixReal * vMatrixBoolean, matrixReal * matrixBoolean);
    QCOMPARE(vMatrixReal * vMatrixInteger, matrixReal * matrixInteger);
    QCOMPARE(vMatrixReal * vMatrixReal, matrixReal * matrixReal);
    QCOMPARE(vMatrixReal * vMatrixComplex, matrixReal * matrixComplex);

    QCOMPARE(vMatrixComplex * vBoolean, matrixComplex * boolean);
    QCOMPARE(vMatrixComplex * vInteger, matrixComplex * integer);
    QCOMPARE(vMatrixComplex * vReal, matrixComplex * real);
    QCOMPARE(vMatrixComplex * vComplex, matrixComplex * complex);
    QCOMPARE(vMatrixComplex * vMatrixBoolean, matrixComplex * matrixBoolean);
    QCOMPARE(vMatrixComplex * vMatrixInteger, matrixComplex * matrixInteger);
    QCOMPARE(vMatrixComplex * vMatrixReal, matrixComplex * matrixReal);
    QCOMPARE(vMatrixComplex * vMatrixComplex, matrixComplex * matrixComplex);
}


void TestVariantOperators::testDivision() {
    M::Boolean       boolean       = true;
    M::Integer       integer       = 2;
    M::Real          real          = 3.0;
    M::Complex       complex       = M::Complex(4.0, 5.0);
    M::Tuple         tuple         = M::Tuple::build(M::Integer(1), M::Integer(2), M::Integer(4));
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
    M::Variant vTuple(tuple);
    M::Variant vMatrixBoolean(matrixBoolean);
    M::Variant vMatrixInteger(matrixInteger);
    M::Variant vMatrixReal(matrixReal);
    M::Variant vMatrixComplex(matrixComplex);

//    QCOMPARE(boolean / vBoolean, M::Integer(boolean / boolean));
//    QCOMPARE(boolean / vInteger, boolean / integer);
//    QCOMPARE(boolean / vReal,    boolean / real);
    QCOMPARE(boolean / vComplex, boolean / complex);

//    QCOMPARE(integer / vBoolean, integer / boolean);
    QCOMPARE(integer / vInteger, integer / integer);
    QCOMPARE(integer / vReal, integer / real);
    QCOMPARE(integer / vComplex, integer / complex);

//    QCOMPARE(real / vBoolean, real / boolean);
    QCOMPARE(real / vInteger, real / integer);
    QCOMPARE(real / vReal, real / real);
    QCOMPARE(real / vComplex, real / complex);

//    QCOMPARE(complex / vBoolean, complex / boolean);
    QCOMPARE(complex / vInteger, complex / integer);
    QCOMPARE(complex / vReal, complex / real);
    QCOMPARE(complex / vComplex, complex / complex);

    QCOMPARE(tuple / vTuple, tuple / tuple);

//    QCOMPARE(matrixBoolean / vBoolean, matrixBoolean / boolean);
    QCOMPARE(matrixBoolean / vInteger, matrixBoolean / integer);
    QCOMPARE(matrixBoolean / vReal, matrixBoolean / real);
    QCOMPARE(matrixBoolean / vComplex, matrixBoolean / complex);

//    QCOMPARE(matrixInteger / vBoolean, matrixInteger / boolean);
    QCOMPARE(matrixInteger / vInteger, matrixInteger / integer);
    QCOMPARE(matrixInteger / vReal, matrixInteger / real);
    QCOMPARE(matrixInteger / vComplex, matrixInteger / complex);

//    QCOMPARE(matrixReal / vBoolean, matrixReal / boolean);
    QCOMPARE(matrixReal / vInteger, matrixReal / integer);
    QCOMPARE(matrixReal / vReal, matrixReal / real);
    QCOMPARE(matrixReal / vComplex, matrixReal / complex);

//    QCOMPARE(matrixComplex / vBoolean, matrixComplex / boolean);
    QCOMPARE(matrixComplex / vInteger, matrixComplex / integer);
    QCOMPARE(matrixComplex / vReal, matrixComplex / real);
    QCOMPARE(matrixComplex / vComplex, matrixComplex / complex);


//    QCOMPARE(vBoolean / boolean, M::Integer(boolean / boolean));
    QCOMPARE(vBoolean / integer, M::Integer(boolean) / integer);
    QCOMPARE(vBoolean / real,    M::Integer(boolean) / real);
    QCOMPARE(vBoolean / complex, M::Integer(boolean) / complex);

//    QCOMPARE(vInteger / boolean, integer / boolean);
    QCOMPARE(vInteger / integer, integer / integer);
    QCOMPARE(vInteger / real, integer / real);
    QCOMPARE(vInteger / complex, integer / complex);

//    QCOMPARE(vReal / boolean, real / boolean);
    QCOMPARE(vReal / integer, real / integer);
    QCOMPARE(vReal / real, real / real);
    QCOMPARE(vReal / complex, real / complex);

//    QCOMPARE(vComplex / boolean, complex / boolean);
    QCOMPARE(vComplex / integer, complex / integer);
    QCOMPARE(vComplex / real, complex / real);
    QCOMPARE(vComplex / complex, complex / complex);

    QCOMPARE(vTuple / vTuple, tuple / tuple);

//    QCOMPARE(vMatrixBoolean / boolean, matrixBoolean / boolean);
    QCOMPARE(vMatrixBoolean / integer, matrixBoolean / integer);
    QCOMPARE(vMatrixBoolean / real, matrixBoolean / real);
    QCOMPARE(vMatrixBoolean / complex, matrixBoolean / complex);

//    QCOMPARE(vMatrixInteger / boolean, matrixInteger / boolean);
    QCOMPARE(vMatrixInteger / integer, matrixInteger / integer);
    QCOMPARE(vMatrixInteger / real, matrixInteger / real);
    QCOMPARE(vMatrixInteger / complex, matrixInteger / complex);

//    QCOMPARE(vMatrixReal / boolean, matrixReal / boolean);
    QCOMPARE(vMatrixReal / integer, matrixReal / integer);
    QCOMPARE(vMatrixReal / real, matrixReal / real);
    QCOMPARE(vMatrixReal / complex, matrixReal / complex);

//    QCOMPARE(vMatrixComplex / boolean, matrixComplex / boolean);
    QCOMPARE(vMatrixComplex / integer, matrixComplex / integer);
    QCOMPARE(vMatrixComplex / real, matrixComplex / real);
    QCOMPARE(vMatrixComplex / complex, matrixComplex / complex);


//    QCOMPARE(vBoolean / vBoolean, M::Integer(boolean / boolean));
    QCOMPARE(vBoolean / vInteger, M::Integer(boolean) / integer);
    QCOMPARE(vBoolean / vReal,    M::Integer(boolean) / real);
    QCOMPARE(vBoolean / vComplex, boolean / complex);

//    QCOMPARE(vInteger / vBoolean, integer / boolean);
    QCOMPARE(vInteger / vInteger, integer / integer);
    QCOMPARE(vInteger / vReal, integer / real);
    QCOMPARE(vInteger / vComplex, integer / complex);

//    QCOMPARE(vReal / vBoolean, real / boolean);
    QCOMPARE(vReal / vInteger, real / integer);
    QCOMPARE(vReal / vReal, real / real);
    QCOMPARE(vReal / vComplex, real / complex);

//    QCOMPARE(vComplex / vBoolean, complex / boolean);
    QCOMPARE(vComplex / vInteger, complex / integer);
    QCOMPARE(vComplex / vReal, complex / real);
    QCOMPARE(vComplex / vComplex, complex / complex);

    QCOMPARE(vTuple / vTuple, tuple / tuple);

//    QCOMPARE(vMatrixBoolean / vBoolean, matrixBoolean / boolean);
    QCOMPARE(vMatrixBoolean / vInteger, matrixBoolean / integer);
    QCOMPARE(vMatrixBoolean / vReal, matrixBoolean / real);
    QCOMPARE(vMatrixBoolean / vComplex, matrixBoolean / complex);

//    QCOMPARE(vMatrixInteger / vBoolean, matrixInteger / boolean);
    QCOMPARE(vMatrixInteger / vInteger, matrixInteger / integer);
    QCOMPARE(vMatrixInteger / vReal, matrixInteger / real);
    QCOMPARE(vMatrixInteger / vComplex, matrixInteger / complex);

//    QCOMPARE(vMatrixReal / vBoolean, matrixReal / boolean);
    QCOMPARE(vMatrixReal / vInteger, matrixReal / integer);
    QCOMPARE(vMatrixReal / vReal, matrixReal / real);
    QCOMPARE(vMatrixReal / vComplex, matrixReal / complex);

//    QCOMPARE(vMatrixComplex / vBoolean, matrixComplex / boolean);
    QCOMPARE(vMatrixComplex / vInteger, matrixComplex / integer);
    QCOMPARE(vMatrixComplex / vReal, matrixComplex / real);
    QCOMPARE(vMatrixComplex / vComplex, matrixComplex / complex);
}


void TestVariantOperators::testUnaryPlusMinus() {
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

    QCOMPARE(+vBoolean, M::Integer(1));
    QCOMPARE(+vInteger, integer);
    QCOMPARE(+vReal, real);
    QCOMPARE(+vComplex, complex);
    QCOMPARE(+vMatrixBoolean, M::MatrixInteger::build(2, 2, 1, 0, 0, 1));
    QCOMPARE(+vMatrixInteger, matrixInteger);
    QCOMPARE(+vMatrixReal, matrixReal);
    QCOMPARE(+vMatrixComplex, matrixComplex);

    QCOMPARE(-vBoolean, M::Integer(-1));
    QCOMPARE(-vInteger, -integer);
    QCOMPARE(-vReal, -real);
    QCOMPARE(-vComplex, -complex);
    QCOMPARE(-vMatrixBoolean, M::MatrixInteger::build(2, 2, -1, 0, 0, -1));
    QCOMPARE(-vMatrixInteger, -matrixInteger);
    QCOMPARE(-vMatrixReal, -matrixReal);
    QCOMPARE(-vMatrixComplex, -matrixComplex);
}


void TestVariantOperators::testLogicalAndOperator() {
    M::Boolean boolean1       = true;
    M::Integer integer1       = 2;
    M::Real    real1          = 3.0;
    M::Complex complex1       = M::Complex(4.0, 5.0);

    M::Boolean boolean0       = false;
    M::Integer integer0       = 0;
    M::Real    real0          = 0;
    M::Complex complex0       = M::Complex(0);

    M::Variant vBoolean1(boolean1);
    M::Variant vInteger1(integer1);
    M::Variant vReal1(real1);
    M::Variant vComplex1(complex1);

    M::Variant vBoolean0(boolean0);
    M::Variant vInteger0(integer0);
    M::Variant vReal0(real0);
    M::Variant vComplex0(complex0);

    QCOMPARE(boolean1 && vBoolean1, boolean1 && boolean1);
    QCOMPARE(boolean1 && vInteger1, boolean1 && integer1);
    QCOMPARE(boolean1 && vReal1,    boolean1 && real1);
    QCOMPARE(boolean1 && vComplex1, boolean1 && complex1);

    QCOMPARE(integer1 && vBoolean1, integer1 && boolean1);
    QCOMPARE(integer1 && vInteger1, integer1 && integer1);
    QCOMPARE(integer1 && vReal1, integer1 && real1);
    QCOMPARE(integer1 && vComplex1, integer1 && complex1);

    QCOMPARE(real1 && vBoolean1, real1 && boolean1);
    QCOMPARE(real1 && vInteger1, real1 && integer1);
    QCOMPARE(real1 && vReal1, real1 && real1);
    QCOMPARE(real1 && vComplex1, real1 && complex1);

    QCOMPARE(complex1 && vBoolean1, complex1 && boolean1);
    QCOMPARE(complex1 && vInteger1, complex1 && integer1);
    QCOMPARE(complex1 && vReal1, complex1 && real1);
    QCOMPARE(complex1 && vComplex1, complex1 && complex1);


    QCOMPARE(vBoolean1 && boolean1, boolean1 && boolean1);
    QCOMPARE(vBoolean1 && integer1, boolean1 && integer1);
    QCOMPARE(vBoolean1 && real1,    boolean1 && real1);
    QCOMPARE(vBoolean1 && complex1, boolean1 && complex1);

    QCOMPARE(vInteger1 && boolean1, integer1 && boolean1);
    QCOMPARE(vInteger1 && integer1, integer1 && integer1);
    QCOMPARE(vInteger1 && real1, integer1 && real1);
    QCOMPARE(vInteger1 && complex1, integer1 && complex1);

    QCOMPARE(vReal1 && boolean1, real1 && boolean1);
    QCOMPARE(vReal1 && integer1, real1 && integer1);
    QCOMPARE(vReal1 && real1, real1 && real1);
    QCOMPARE(vReal1 && complex1, real1 && complex1);

    QCOMPARE(vComplex1 && boolean1, complex1 && boolean1);
    QCOMPARE(vComplex1 && integer1, complex1 && integer1);
    QCOMPARE(vComplex1 && real1, complex1 && real1);
    QCOMPARE(vComplex1 && complex1, complex1 && complex1);


    QCOMPARE(vBoolean1 && vBoolean1, boolean1 && boolean1);
    QCOMPARE(vBoolean1 && vInteger1, boolean1 && integer1);
    QCOMPARE(vBoolean1 && vReal1,    boolean1 && real1);
    QCOMPARE(vBoolean1 && vComplex1, boolean1 && complex1);

    QCOMPARE(vInteger1 && vBoolean1, integer1 && boolean1);
    QCOMPARE(vInteger1 && vInteger1, integer1 && integer1);
    QCOMPARE(vInteger1 && vReal1, integer1 && real1);
    QCOMPARE(vInteger1 && vComplex1, integer1 && complex1);

    QCOMPARE(vReal1 && vBoolean1, real1 && boolean1);
    QCOMPARE(vReal1 && vInteger1, real1 && integer1);
    QCOMPARE(vReal1 && vReal1, real1 && real1);
    QCOMPARE(vReal1 && vComplex1, real1 && complex1);

    QCOMPARE(vComplex1 && vBoolean1, complex1 && boolean1);
    QCOMPARE(vComplex1 && vInteger1, complex1 && integer1);
    QCOMPARE(vComplex1 && vReal1, complex1 && real1);
    QCOMPARE(vComplex1 && vComplex1, complex1 && complex1);


    QCOMPARE(boolean0 && vBoolean1, boolean0 && boolean1);
    QCOMPARE(boolean0 && vInteger1, boolean0 && integer1);
    QCOMPARE(boolean0 && vReal1,    boolean0 && real1);
    QCOMPARE(boolean0 && vComplex1, boolean0 && complex1);

    QCOMPARE(integer0 && vBoolean1, integer0 && boolean1);
    QCOMPARE(integer0 && vInteger1, integer0 && integer1);
    QCOMPARE(integer0 && vReal1, integer0 && real1);
    QCOMPARE(integer0 && vComplex1, integer0 && complex1);

    QCOMPARE(real0 && vBoolean1, real0 && boolean1);
    QCOMPARE(real0 && vInteger1, real0 && integer1);
    QCOMPARE(real0 && vReal1, real0 && real1);
    QCOMPARE(real0 && vComplex1, real0 && complex1);

    QCOMPARE(complex0 && vBoolean1, complex0 && boolean1);
    QCOMPARE(complex0 && vInteger1, complex0 && integer1);
    QCOMPARE(complex0 && vReal1, complex0 && real1);
    QCOMPARE(complex0 && vComplex1, complex0 && complex1);


    QCOMPARE(vBoolean0 && boolean1, boolean0 && boolean1);
    QCOMPARE(vBoolean0 && integer1, boolean0 && integer1);
    QCOMPARE(vBoolean0 && real1,    boolean0 && real1);
    QCOMPARE(vBoolean0 && complex1, boolean0 && complex1);

    QCOMPARE(vInteger0 && boolean1, integer0 && boolean1);
    QCOMPARE(vInteger0 && integer1, integer0 && integer1);
    QCOMPARE(vInteger0 && real1, integer0 && real1);
    QCOMPARE(vInteger0 && complex1, integer0 && complex1);

    QCOMPARE(vReal0 && boolean1, real0 && boolean1);
    QCOMPARE(vReal0 && integer1, real0 && integer1);
    QCOMPARE(vReal0 && real1, real0 && real1);
    QCOMPARE(vReal0 && complex1, real0 && complex1);

    QCOMPARE(vComplex0 && boolean1, complex0 && boolean1);
    QCOMPARE(vComplex0 && integer1, complex0 && integer1);
    QCOMPARE(vComplex0 && real1, complex0 && real1);
    QCOMPARE(vComplex0 && complex1, complex0 && complex1);


    QCOMPARE(vBoolean0 && vBoolean1, boolean0 && boolean1);
    QCOMPARE(vBoolean0 && vInteger1, boolean0 && integer1);
    QCOMPARE(vBoolean0 && vReal1,    boolean0 && real1);
    QCOMPARE(vBoolean0 && vComplex1, boolean0 && complex1);

    QCOMPARE(vInteger0 && vBoolean1, integer0 && boolean1);
    QCOMPARE(vInteger0 && vInteger1, integer0 && integer1);
    QCOMPARE(vInteger0 && vReal1, integer0 && real1);
    QCOMPARE(vInteger0 && vComplex1, integer0 && complex1);

    QCOMPARE(vReal0 && vBoolean1, real0 && boolean1);
    QCOMPARE(vReal0 && vInteger1, real0 && integer1);
    QCOMPARE(vReal0 && vReal1, real0 && real1);
    QCOMPARE(vReal0 && vComplex1, real0 && complex1);

    QCOMPARE(vComplex0 && vBoolean1, complex0 && boolean1);
    QCOMPARE(vComplex0 && vInteger1, complex0 && integer1);
    QCOMPARE(vComplex0 && vReal1, complex0 && real1);
    QCOMPARE(vComplex0 && vComplex1, complex0 && complex1);


    QCOMPARE(boolean1 && vBoolean0, boolean1 && boolean0);
    QCOMPARE(boolean1 && vInteger0, boolean1 && integer0);
    QCOMPARE(boolean1 && vReal0,    boolean1 && real0);
    QCOMPARE(boolean1 && vComplex0, boolean1 && complex0);

    QCOMPARE(integer1 && vBoolean0, integer1 && boolean0);
    QCOMPARE(integer1 && vInteger0, integer1 && integer0);
    QCOMPARE(integer1 && vReal0, integer1 && real0);
    QCOMPARE(integer1 && vComplex0, integer1 && complex0);

    QCOMPARE(real1 && vBoolean0, real1 && boolean0);
    QCOMPARE(real1 && vInteger0, real1 && integer0);
    QCOMPARE(real1 && vReal0, real1 && real0);
    QCOMPARE(real1 && vComplex0, real1 && complex0);

    QCOMPARE(complex1 && vBoolean0, complex1 && boolean0);
    QCOMPARE(complex1 && vInteger0, complex1 && integer0);
    QCOMPARE(complex1 && vReal0, complex1 && real0);
    QCOMPARE(complex1 && vComplex0, complex1 && complex0);


    QCOMPARE(vBoolean1 && boolean0, boolean1 && boolean0);
    QCOMPARE(vBoolean1 && integer0, boolean1 && integer0);
    QCOMPARE(vBoolean1 && real0,    boolean1 && real0);
    QCOMPARE(vBoolean1 && complex0, boolean1 && complex0);

    QCOMPARE(vInteger1 && boolean0, integer1 && boolean0);
    QCOMPARE(vInteger1 && integer0, integer1 && integer0);
    QCOMPARE(vInteger1 && real0, integer1 && real0);
    QCOMPARE(vInteger1 && complex0, integer1 && complex0);

    QCOMPARE(vReal1 && boolean0, real1 && boolean0);
    QCOMPARE(vReal1 && integer0, real1 && integer0);
    QCOMPARE(vReal1 && real0, real1 && real0);
    QCOMPARE(vReal1 && complex0, real1 && complex0);

    QCOMPARE(vComplex1 && boolean0, complex1 && boolean0);
    QCOMPARE(vComplex1 && integer0, complex1 && integer0);
    QCOMPARE(vComplex1 && real0, complex1 && real0);
    QCOMPARE(vComplex1 && complex0, complex1 && complex0);


    QCOMPARE(vBoolean1 && vBoolean0, boolean1 && boolean0);
    QCOMPARE(vBoolean1 && vInteger0, boolean1 && integer0);
    QCOMPARE(vBoolean1 && vReal0,    boolean1 && real0);
    QCOMPARE(vBoolean1 && vComplex0, boolean1 && complex0);

    QCOMPARE(vInteger1 && vBoolean0, integer1 && boolean0);
    QCOMPARE(vInteger1 && vInteger0, integer1 && integer0);
    QCOMPARE(vInteger1 && vReal0, integer1 && real0);
    QCOMPARE(vInteger1 && vComplex0, integer1 && complex0);

    QCOMPARE(vReal1 && vBoolean0, real1 && boolean0);
    QCOMPARE(vReal1 && vInteger0, real1 && integer0);
    QCOMPARE(vReal1 && vReal0, real1 && real0);
    QCOMPARE(vReal1 && vComplex0, real1 && complex0);

    QCOMPARE(vComplex1 && vBoolean0, complex1 && boolean0);
    QCOMPARE(vComplex1 && vInteger0, complex1 && integer0);
    QCOMPARE(vComplex1 && vReal0, complex1 && real0);
    QCOMPARE(vComplex1 && vComplex0, complex1 && complex0);


    QCOMPARE(boolean0 && vBoolean0, boolean0 && boolean0);
    QCOMPARE(boolean0 && vInteger0, boolean0 && integer0);
    QCOMPARE(boolean0 && vReal0,    boolean0 && real0);
    QCOMPARE(boolean0 && vComplex0, boolean0 && complex0);

    QCOMPARE(integer0 && vBoolean0, integer0 && boolean0);
    QCOMPARE(integer0 && vInteger0, integer0 && integer0);
    QCOMPARE(integer0 && vReal0, integer0 && real0);
    QCOMPARE(integer0 && vComplex0, integer0 && complex0);

    QCOMPARE(real0 && vBoolean0, real0 && boolean0);
    QCOMPARE(real0 && vInteger0, real0 && integer0);
    QCOMPARE(real0 && vReal0, real0 && real0);
    QCOMPARE(real0 && vComplex0, real0 && complex0);

    QCOMPARE(complex0 && vBoolean0, complex0 && boolean0);
    QCOMPARE(complex0 && vInteger0, complex0 && integer0);
    QCOMPARE(complex0 && vReal0, complex0 && real0);
    QCOMPARE(complex0 && vComplex0, complex0 && complex0);


    QCOMPARE(vBoolean0 && boolean0, boolean0 && boolean0);
    QCOMPARE(vBoolean0 && integer0, boolean0 && integer0);
    QCOMPARE(vBoolean0 && real0,    boolean0 && real0);
    QCOMPARE(vBoolean0 && complex0, boolean0 && complex0);

    QCOMPARE(vInteger0 && boolean0, integer0 && boolean0);
    QCOMPARE(vInteger0 && integer0, integer0 && integer0);
    QCOMPARE(vInteger0 && real0, integer0 && real0);
    QCOMPARE(vInteger0 && complex0, integer0 && complex0);

    QCOMPARE(vReal0 && boolean0, real0 && boolean0);
    QCOMPARE(vReal0 && integer0, real0 && integer0);
    QCOMPARE(vReal0 && real0, real0 && real0);
    QCOMPARE(vReal0 && complex0, real0 && complex0);

    QCOMPARE(vComplex0 && boolean0, complex0 && boolean0);
    QCOMPARE(vComplex0 && integer0, complex0 && integer0);
    QCOMPARE(vComplex0 && real0, complex0 && real0);
    QCOMPARE(vComplex0 && complex0, complex0 && complex0);


    QCOMPARE(vBoolean0 && vBoolean0, boolean0 && boolean0);
    QCOMPARE(vBoolean0 && vInteger0, boolean0 && integer0);
    QCOMPARE(vBoolean0 && vReal0,    boolean0 && real0);
    QCOMPARE(vBoolean0 && vComplex0, boolean0 && complex0);

    QCOMPARE(vInteger0 && vBoolean0, integer0 && boolean0);
    QCOMPARE(vInteger0 && vInteger0, integer0 && integer0);
    QCOMPARE(vInteger0 && vReal0, integer0 && real0);
    QCOMPARE(vInteger0 && vComplex0, integer0 && complex0);

    QCOMPARE(vReal0 && vBoolean0, real0 && boolean0);
    QCOMPARE(vReal0 && vInteger0, real0 && integer0);
    QCOMPARE(vReal0 && vReal0, real0 && real0);
    QCOMPARE(vReal0 && vComplex0, real0 && complex0);

    QCOMPARE(vComplex0 && vBoolean0, complex0 && boolean0);
    QCOMPARE(vComplex0 && vInteger0, complex0 && integer0);
    QCOMPARE(vComplex0 && vReal0, complex0 && real0);
    QCOMPARE(vComplex0 && vComplex0, complex0 && complex0);
}


void TestVariantOperators::testLogicalOrOperator() {
    M::Boolean boolean1       = true;
    M::Integer integer1       = 2;
    M::Real    real1          = 3.0;
    M::Complex complex1       = M::Complex(4.0, 5.0);

    M::Boolean boolean0       = false;
    M::Integer integer0       = 0;
    M::Real    real0          = 0;
    M::Complex complex0       = M::Complex(0);

    M::Variant vBoolean1(boolean1);
    M::Variant vInteger1(integer1);
    M::Variant vReal1(real1);
    M::Variant vComplex1(complex1);

    M::Variant vBoolean0(boolean0);
    M::Variant vInteger0(integer0);
    M::Variant vReal0(real0);
    M::Variant vComplex0(complex0);

    QCOMPARE(boolean1 || vBoolean1, boolean1 || boolean1);
    QCOMPARE(boolean1 || vInteger1, boolean1 || integer1);
    QCOMPARE(boolean1 || vReal1,    boolean1 || real1);
    QCOMPARE(boolean1 || vComplex1, boolean1 || complex1);

    QCOMPARE(integer1 || vBoolean1, integer1 || boolean1);
    QCOMPARE(integer1 || vInteger1, integer1 || integer1);
    QCOMPARE(integer1 || vReal1, integer1 || real1);
    QCOMPARE(integer1 || vComplex1, integer1 || complex1);

    QCOMPARE(real1 || vBoolean1, real1 || boolean1);
    QCOMPARE(real1 || vInteger1, real1 || integer1);
    QCOMPARE(real1 || vReal1, real1 || real1);
    QCOMPARE(real1 || vComplex1, real1 || complex1);

    QCOMPARE(complex1 || vBoolean1, complex1 || boolean1);
    QCOMPARE(complex1 || vInteger1, complex1 || integer1);
    QCOMPARE(complex1 || vReal1, complex1 || real1);
    QCOMPARE(complex1 || vComplex1, complex1 || complex1);


    QCOMPARE(vBoolean1 || boolean1, boolean1 || boolean1);
    QCOMPARE(vBoolean1 || integer1, boolean1 || integer1);
    QCOMPARE(vBoolean1 || real1,    boolean1 || real1);
    QCOMPARE(vBoolean1 || complex1, boolean1 || complex1);

    QCOMPARE(vInteger1 || boolean1, integer1 || boolean1);
    QCOMPARE(vInteger1 || integer1, integer1 || integer1);
    QCOMPARE(vInteger1 || real1, integer1 || real1);
    QCOMPARE(vInteger1 || complex1, integer1 || complex1);

    QCOMPARE(vReal1 || boolean1, real1 || boolean1);
    QCOMPARE(vReal1 || integer1, real1 || integer1);
    QCOMPARE(vReal1 || real1, real1 || real1);
    QCOMPARE(vReal1 || complex1, real1 || complex1);

    QCOMPARE(vComplex1 || boolean1, complex1 || boolean1);
    QCOMPARE(vComplex1 || integer1, complex1 || integer1);
    QCOMPARE(vComplex1 || real1, complex1 || real1);
    QCOMPARE(vComplex1 || complex1, complex1 || complex1);


    QCOMPARE(vBoolean1 || vBoolean1, boolean1 || boolean1);
    QCOMPARE(vBoolean1 || vInteger1, boolean1 || integer1);
    QCOMPARE(vBoolean1 || vReal1,    boolean1 || real1);
    QCOMPARE(vBoolean1 || vComplex1, boolean1 || complex1);

    QCOMPARE(vInteger1 || vBoolean1, integer1 || boolean1);
    QCOMPARE(vInteger1 || vInteger1, integer1 || integer1);
    QCOMPARE(vInteger1 || vReal1, integer1 || real1);
    QCOMPARE(vInteger1 || vComplex1, integer1 || complex1);

    QCOMPARE(vReal1 || vBoolean1, real1 || boolean1);
    QCOMPARE(vReal1 || vInteger1, real1 || integer1);
    QCOMPARE(vReal1 || vReal1, real1 || real1);
    QCOMPARE(vReal1 || vComplex1, real1 || complex1);

    QCOMPARE(vComplex1 || vBoolean1, complex1 || boolean1);
    QCOMPARE(vComplex1 || vInteger1, complex1 || integer1);
    QCOMPARE(vComplex1 || vReal1, complex1 || real1);
    QCOMPARE(vComplex1 || vComplex1, complex1 || complex1);


    QCOMPARE(boolean0 || vBoolean1, boolean0 || boolean1);
    QCOMPARE(boolean0 || vInteger1, boolean0 || integer1);
    QCOMPARE(boolean0 || vReal1,    boolean0 || real1);
    QCOMPARE(boolean0 || vComplex1, boolean0 || complex1);

    QCOMPARE(integer0 || vBoolean1, integer0 || boolean1);
    QCOMPARE(integer0 || vInteger1, integer0 || integer1);
    QCOMPARE(integer0 || vReal1, integer0 || real1);
    QCOMPARE(integer0 || vComplex1, integer0 || complex1);

    QCOMPARE(real0 || vBoolean1, real0 || boolean1);
    QCOMPARE(real0 || vInteger1, real0 || integer1);
    QCOMPARE(real0 || vReal1, real0 || real1);
    QCOMPARE(real0 || vComplex1, real0 || complex1);

    QCOMPARE(complex0 || vBoolean1, complex0 || boolean1);
    QCOMPARE(complex0 || vInteger1, complex0 || integer1);
    QCOMPARE(complex0 || vReal1, complex0 || real1);
    QCOMPARE(complex0 || vComplex1, complex0 || complex1);


    QCOMPARE(vBoolean0 || boolean1, boolean0 || boolean1);
    QCOMPARE(vBoolean0 || integer1, boolean0 || integer1);
    QCOMPARE(vBoolean0 || real1,    boolean0 || real1);
    QCOMPARE(vBoolean0 || complex1, boolean0 || complex1);

    QCOMPARE(vInteger0 || boolean1, integer0 || boolean1);
    QCOMPARE(vInteger0 || integer1, integer0 || integer1);
    QCOMPARE(vInteger0 || real1, integer0 || real1);
    QCOMPARE(vInteger0 || complex1, integer0 || complex1);

    QCOMPARE(vReal0 || boolean1, real0 || boolean1);
    QCOMPARE(vReal0 || integer1, real0 || integer1);
    QCOMPARE(vReal0 || real1, real0 || real1);
    QCOMPARE(vReal0 || complex1, real0 || complex1);

    QCOMPARE(vComplex0 || boolean1, complex0 || boolean1);
    QCOMPARE(vComplex0 || integer1, complex0 || integer1);
    QCOMPARE(vComplex0 || real1, complex0 || real1);
    QCOMPARE(vComplex0 || complex1, complex0 || complex1);


    QCOMPARE(vBoolean0 || vBoolean1, boolean0 || boolean1);
    QCOMPARE(vBoolean0 || vInteger1, boolean0 || integer1);
    QCOMPARE(vBoolean0 || vReal1,    boolean0 || real1);
    QCOMPARE(vBoolean0 || vComplex1, boolean0 || complex1);

    QCOMPARE(vInteger0 || vBoolean1, integer0 || boolean1);
    QCOMPARE(vInteger0 || vInteger1, integer0 || integer1);
    QCOMPARE(vInteger0 || vReal1, integer0 || real1);
    QCOMPARE(vInteger0 || vComplex1, integer0 || complex1);

    QCOMPARE(vReal0 || vBoolean1, real0 || boolean1);
    QCOMPARE(vReal0 || vInteger1, real0 || integer1);
    QCOMPARE(vReal0 || vReal1, real0 || real1);
    QCOMPARE(vReal0 || vComplex1, real0 || complex1);

    QCOMPARE(vComplex0 || vBoolean1, complex0 || boolean1);
    QCOMPARE(vComplex0 || vInteger1, complex0 || integer1);
    QCOMPARE(vComplex0 || vReal1, complex0 || real1);
    QCOMPARE(vComplex0 || vComplex1, complex0 || complex1);


    QCOMPARE(boolean1 || vBoolean0, boolean1 || boolean0);
    QCOMPARE(boolean1 || vInteger0, boolean1 || integer0);
    QCOMPARE(boolean1 || vReal0,    boolean1 || real0);
    QCOMPARE(boolean1 || vComplex0, boolean1 || complex0);

    QCOMPARE(integer1 || vBoolean0, integer1 || boolean0);
    QCOMPARE(integer1 || vInteger0, integer1 || integer0);
    QCOMPARE(integer1 || vReal0, integer1 || real0);
    QCOMPARE(integer1 || vComplex0, integer1 || complex0);

    QCOMPARE(real1 || vBoolean0, real1 || boolean0);
    QCOMPARE(real1 || vInteger0, real1 || integer0);
    QCOMPARE(real1 || vReal0, real1 || real0);
    QCOMPARE(real1 || vComplex0, real1 || complex0);

    QCOMPARE(complex1 || vBoolean0, complex1 || boolean0);
    QCOMPARE(complex1 || vInteger0, complex1 || integer0);
    QCOMPARE(complex1 || vReal0, complex1 || real0);
    QCOMPARE(complex1 || vComplex0, complex1 || complex0);


    QCOMPARE(vBoolean1 || boolean0, boolean1 || boolean0);
    QCOMPARE(vBoolean1 || integer0, boolean1 || integer0);
    QCOMPARE(vBoolean1 || real0,    boolean1 || real0);
    QCOMPARE(vBoolean1 || complex0, boolean1 || complex0);

    QCOMPARE(vInteger1 || boolean0, integer1 || boolean0);
    QCOMPARE(vInteger1 || integer0, integer1 || integer0);
    QCOMPARE(vInteger1 || real0, integer1 || real0);
    QCOMPARE(vInteger1 || complex0, integer1 || complex0);

    QCOMPARE(vReal1 || boolean0, real1 || boolean0);
    QCOMPARE(vReal1 || integer0, real1 || integer0);
    QCOMPARE(vReal1 || real0, real1 || real0);
    QCOMPARE(vReal1 || complex0, real1 || complex0);

    QCOMPARE(vComplex1 || boolean0, complex1 || boolean0);
    QCOMPARE(vComplex1 || integer0, complex1 || integer0);
    QCOMPARE(vComplex1 || real0, complex1 || real0);
    QCOMPARE(vComplex1 || complex0, complex1 || complex0);


    QCOMPARE(vBoolean1 || vBoolean0, boolean1 || boolean0);
    QCOMPARE(vBoolean1 || vInteger0, boolean1 || integer0);
    QCOMPARE(vBoolean1 || vReal0,    boolean1 || real0);
    QCOMPARE(vBoolean1 || vComplex0, boolean1 || complex0);

    QCOMPARE(vInteger1 || vBoolean0, integer1 || boolean0);
    QCOMPARE(vInteger1 || vInteger0, integer1 || integer0);
    QCOMPARE(vInteger1 || vReal0, integer1 || real0);
    QCOMPARE(vInteger1 || vComplex0, integer1 || complex0);

    QCOMPARE(vReal1 || vBoolean0, real1 || boolean0);
    QCOMPARE(vReal1 || vInteger0, real1 || integer0);
    QCOMPARE(vReal1 || vReal0, real1 || real0);
    QCOMPARE(vReal1 || vComplex0, real1 || complex0);

    QCOMPARE(vComplex1 || vBoolean0, complex1 || boolean0);
    QCOMPARE(vComplex1 || vInteger0, complex1 || integer0);
    QCOMPARE(vComplex1 || vReal0, complex1 || real0);
    QCOMPARE(vComplex1 || vComplex0, complex1 || complex0);


    QCOMPARE(boolean0 || vBoolean0, boolean0 || boolean0);
    QCOMPARE(boolean0 || vInteger0, boolean0 || integer0);
    QCOMPARE(boolean0 || vReal0,    boolean0 || real0);
    QCOMPARE(boolean0 || vComplex0, boolean0 || complex0);

    QCOMPARE(integer0 || vBoolean0, integer0 || boolean0);
    QCOMPARE(integer0 || vInteger0, integer0 || integer0);
    QCOMPARE(integer0 || vReal0, integer0 || real0);
    QCOMPARE(integer0 || vComplex0, integer0 || complex0);

    QCOMPARE(real0 || vBoolean0, real0 || boolean0);
    QCOMPARE(real0 || vInteger0, real0 || integer0);
    QCOMPARE(real0 || vReal0, real0 || real0);
    QCOMPARE(real0 || vComplex0, real0 || complex0);

    QCOMPARE(complex0 || vBoolean0, complex0 || boolean0);
    QCOMPARE(complex0 || vInteger0, complex0 || integer0);
    QCOMPARE(complex0 || vReal0, complex0 || real0);
    QCOMPARE(complex0 || vComplex0, complex0 || complex0);


    QCOMPARE(vBoolean0 || boolean0, boolean0 || boolean0);
    QCOMPARE(vBoolean0 || integer0, boolean0 || integer0);
    QCOMPARE(vBoolean0 || real0,    boolean0 || real0);
    QCOMPARE(vBoolean0 || complex0, boolean0 || complex0);

    QCOMPARE(vInteger0 || boolean0, integer0 || boolean0);
    QCOMPARE(vInteger0 || integer0, integer0 || integer0);
    QCOMPARE(vInteger0 || real0, integer0 || real0);
    QCOMPARE(vInteger0 || complex0, integer0 || complex0);

    QCOMPARE(vReal0 || boolean0, real0 || boolean0);
    QCOMPARE(vReal0 || integer0, real0 || integer0);
    QCOMPARE(vReal0 || real0, real0 || real0);
    QCOMPARE(vReal0 || complex0, real0 || complex0);

    QCOMPARE(vComplex0 || boolean0, complex0 || boolean0);
    QCOMPARE(vComplex0 || integer0, complex0 || integer0);
    QCOMPARE(vComplex0 || real0, complex0 || real0);
    QCOMPARE(vComplex0 || complex0, complex0 || complex0);


    QCOMPARE(vBoolean0 || vBoolean0, boolean0 || boolean0);
    QCOMPARE(vBoolean0 || vInteger0, boolean0 || integer0);
    QCOMPARE(vBoolean0 || vReal0,    boolean0 || real0);
    QCOMPARE(vBoolean0 || vComplex0, boolean0 || complex0);

    QCOMPARE(vInteger0 || vBoolean0, integer0 || boolean0);
    QCOMPARE(vInteger0 || vInteger0, integer0 || integer0);
    QCOMPARE(vInteger0 || vReal0, integer0 || real0);
    QCOMPARE(vInteger0 || vComplex0, integer0 || complex0);

    QCOMPARE(vReal0 || vBoolean0, real0 || boolean0);
    QCOMPARE(vReal0 || vInteger0, real0 || integer0);
    QCOMPARE(vReal0 || vReal0, real0 || real0);
    QCOMPARE(vReal0 || vComplex0, real0 || complex0);

    QCOMPARE(vComplex0 || vBoolean0, complex0 || boolean0);
    QCOMPARE(vComplex0 || vInteger0, complex0 || integer0);
    QCOMPARE(vComplex0 || vReal0, complex0 || real0);
    QCOMPARE(vComplex0 || vComplex0, complex0 || complex0);
}


void TestVariantOperators::testLogicalNotOperator() {
    M::Boolean boolean1 = true;
    M::Integer integer1 = 2;
    M::Real    real1    = 3.0;
    M::Complex complex1 = M::Complex(4.0, 5.0);

    M::Boolean boolean0 = false;
    M::Integer integer0 = 0;
    M::Real    real0    = 0;
    M::Complex complex0 = M::Complex(0);

    M::Variant vBoolean1(boolean1);
    M::Variant vInteger1(integer1);
    M::Variant vReal1(real1);
    M::Variant vComplex1(complex1);

    M::Variant vBoolean0(boolean0);
    M::Variant vInteger0(integer0);
    M::Variant vReal0(real0);
    M::Variant vComplex0(complex0);

    QCOMPARE(!vBoolean1, !boolean1);
    QCOMPARE(!vInteger1, !integer1);
    QCOMPARE(!vReal1,    !real1);
    QCOMPARE(!vComplex1, !complex1);

    QCOMPARE(!vBoolean0, !boolean0);
    QCOMPARE(!vInteger0, !integer0);
    QCOMPARE(!vReal0,    !real0);
    QCOMPARE(!vComplex0, !complex0);
}
