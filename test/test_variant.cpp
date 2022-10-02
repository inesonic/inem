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
* This file implements tests of the \ref Model::Variant class.  By extension, this class also tests the underlying
* \ref M::Variant class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QObject>
#include <QList>
#include <QtTest/QtTest>

#include <limits>

#include <model_intrinsic_types.h>
#include <model_complex.h>
#include <model_set.h>
#include <model_tuple.h>
#include <model_matrix_boolean.h>
#include <model_matrix_integer.h>
#include <model_matrix_real.h>
#include <model_matrix_complex.h>
#include <model_variant.h>

#include "test_variant.h"

TestVariant::TestVariant() {}

TestVariant::~TestVariant() {}


void TestVariant::testNoneToNone() {
    Model::Variant variant1;
    Model::Variant variant2(variant1);
    Model::Variant variant3(Model::Integer(1));

    QCOMPARE(variant1.valueType(), Model::ValueType::NONE);
    QCOMPARE(variant2.valueType(), Model::ValueType::NONE);
    QCOMPARE(variant3.valueType(), Model::ValueType::INTEGER);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::NONE), true);

    variant3 = variant1;
    QCOMPARE(variant3.valueType(), Model::ValueType::NONE);
}


void TestVariant::testNoneToBoolean() {
    Model::Variant variant1;
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::NONE);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::BOOLEAN), true);
    QCOMPARE(variant1.toBoolean(&ok), Model::Boolean(false));
    QCOMPARE(ok, true);
}


void TestVariant::testNoneToInteger() {
    Model::Variant variant1;
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::NONE);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::INTEGER), true);
    QCOMPARE(variant1.toInteger(&ok), Model::Integer(0));
    QCOMPARE(ok, true);
}


void TestVariant::testNoneToReal() {
    Model::Variant variant1;
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::NONE);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::REAL), true);
    QCOMPARE(variant1.toReal(&ok), Model::Real(0));
    QCOMPARE(ok, true);
}


void TestVariant::testNoneToComplex() {
    Model::Variant variant1;
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::NONE);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::COMPLEX), true);
    QCOMPARE(variant1.toComplex(&ok), Model::Complex(0, 0));
    QCOMPARE(ok, true);
}


void TestVariant::testNoneToSet() {
    Model::Variant variant1;
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::NONE);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::SET), true);
    QCOMPARE(variant1.toSet(&ok), Model::Set());
    QCOMPARE(ok, true);
}


void TestVariant::testNoneToTuple() {
    Model::Variant variant1;
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::NONE);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::TUPLE), true);
    QCOMPARE(variant1.toTuple(&ok), Model::Tuple());
    QCOMPARE(ok, true);
}


void TestVariant::testNoneToMatrixBoolean() {
    Model::Variant variant1;
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::NONE);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::MATRIX_BOOLEAN), true);
    QCOMPARE(variant1.toMatrixBoolean(&ok), Model::MatrixBoolean());
    QCOMPARE(ok, true);
}


void TestVariant::testNoneToMatrixInteger() {
    Model::Variant variant1;
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::NONE);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::MATRIX_INTEGER), true);
    QCOMPARE(variant1.toMatrixInteger(&ok), Model::MatrixInteger());
    QCOMPARE(ok, true);
}


void TestVariant::testNoneToMatrixReal() {
    Model::Variant variant1;
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::NONE);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::MATRIX_REAL), true);
    QCOMPARE(variant1.toMatrixReal(&ok), Model::MatrixReal());
    QCOMPARE(ok, true);
}


void TestVariant::testNoneToMatrixComplex() {
    Model::Variant variant1;
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::NONE);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::MATRIX_COMPLEX), true);
    QCOMPARE(variant1.toMatrixComplex(&ok), Model::MatrixComplex());
    QCOMPARE(ok, true);
}

// additional TestVariant::testNoneTo...

void TestVariant::testBooleanToNone() {
    Model::Variant variant1(Model::Boolean(true));

    QCOMPARE(variant1.valueType(), Model::ValueType::BOOLEAN);
    QCOMPARE(variant1.canTranslateTo(Model::ValueType::NONE), false);
}


void TestVariant::testBooleanToBoolean() {
    Model::Variant variant1(Model::Boolean(true));
    Model::Variant variant2(variant1);
    Model::Variant variant3;
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::BOOLEAN);
    QCOMPARE(variant2.valueType(), Model::ValueType::BOOLEAN);
    QCOMPARE(variant3.valueType(), Model::ValueType::NONE);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::BOOLEAN), true);
    QCOMPARE(variant1.toBoolean(&ok), Model::Boolean(true));
    QCOMPARE(ok, true);

    QCOMPARE(variant2.canTranslateTo(Model::ValueType::BOOLEAN), true);
    QCOMPARE(variant2.toBoolean(&ok), Model::Boolean(true));
    QCOMPARE(ok, true);

    variant3 = variant1;
    QCOMPARE(variant3.valueType(), Model::ValueType::BOOLEAN);
    QCOMPARE(variant3.canTranslateTo(Model::ValueType::BOOLEAN), true);
    QCOMPARE(variant3.toBoolean(&ok), Model::Boolean(true));
    QCOMPARE(ok, true);
}


void TestVariant::testBooleanToInteger() {
    Model::Variant variant1(Model::Boolean(true));
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::BOOLEAN);
    QCOMPARE(variant1.canTranslateTo(Model::ValueType::INTEGER), true);
    QCOMPARE(variant1.toInteger(&ok), Model::Integer(1));
    QCOMPARE(ok, true);
}


void TestVariant::testBooleanToReal() {
    Model::Variant variant1(Model::Boolean(true));
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::BOOLEAN);
    QCOMPARE(variant1.canTranslateTo(Model::ValueType::REAL), true);
    QCOMPARE(variant1.toReal(&ok), Model::Real(1));
    QCOMPARE(ok, true);
}


void TestVariant::testBooleanToComplex() {
    Model::Variant variant1(Model::Boolean(true));
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::BOOLEAN);
    QCOMPARE(variant1.canTranslateTo(Model::ValueType::COMPLEX), true);
    QCOMPARE(variant1.toComplex(&ok), Model::Complex(1, 0));
    QCOMPARE(ok, true);
}


void TestVariant::testBooleanToSet() {
    Model::Variant variant1(Model::Boolean(true));

    QCOMPARE(variant1.valueType(), Model::ValueType::BOOLEAN);
    QCOMPARE(variant1.canTranslateTo(Model::ValueType::SET), false);
}


void TestVariant::testBooleanToTuple() {
    Model::Variant variant1(Model::Boolean(true));

    QCOMPARE(variant1.valueType(), Model::ValueType::BOOLEAN);
    QCOMPARE(variant1.canTranslateTo(Model::ValueType::TUPLE), false);
}


void TestVariant::testBooleanToMatrixBoolean() {
    Model::Variant variant1(Model::Boolean(true));

    QCOMPARE(variant1.valueType(), Model::ValueType::BOOLEAN);
    QCOMPARE(variant1.canTranslateTo(Model::ValueType::MATRIX_BOOLEAN), false);
}


void TestVariant::testBooleanToMatrixInteger() {
    Model::Variant variant1(Model::Boolean(true));

    QCOMPARE(variant1.valueType(), Model::ValueType::BOOLEAN);
    QCOMPARE(variant1.canTranslateTo(Model::ValueType::MATRIX_INTEGER), false);
}


void TestVariant::testBooleanToMatrixReal() {
    Model::Variant variant1(Model::Boolean(true));

    QCOMPARE(variant1.valueType(), Model::ValueType::BOOLEAN);
    QCOMPARE(variant1.canTranslateTo(Model::ValueType::MATRIX_REAL), false);
}


void TestVariant::testBooleanToMatrixComplex() {
    Model::Variant variant1(Model::Boolean(true));

    QCOMPARE(variant1.valueType(), Model::ValueType::BOOLEAN);
    QCOMPARE(variant1.canTranslateTo(Model::ValueType::MATRIX_COMPLEX), false);
}

// additional TestVariant::testBooleanTo...

void TestVariant::testIntegerToNone() {
    Model::Variant variant1(Model::Integer(2));

    QCOMPARE(variant1.valueType(), Model::ValueType::INTEGER);
    QCOMPARE(variant1.canTranslateTo(Model::ValueType::NONE), false);
}


void TestVariant::testIntegerToBoolean() {
    Model::Variant variant1(Model::Integer(2));
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::INTEGER);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::BOOLEAN), true);
    QCOMPARE(variant1.toBoolean(&ok), Model::Boolean(true));
    QCOMPARE(ok, true);
}


void TestVariant::testIntegerToInteger() {
    Model::Variant variant1(Model::Integer(2));
    Model::Variant variant2(variant1);
    Model::Variant variant3;
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::INTEGER);
    QCOMPARE(variant2.valueType(), Model::ValueType::INTEGER);
    QCOMPARE(variant3.valueType(), Model::ValueType::NONE);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::INTEGER), true);
    QCOMPARE(variant1.toInteger(&ok), Model::Integer(2));
    QCOMPARE(ok, true);

    QCOMPARE(variant2.canTranslateTo(Model::ValueType::INTEGER), true);
    QCOMPARE(variant2.toInteger(&ok), Model::Integer(2));
    QCOMPARE(ok, true);

    variant3 = variant1;
    QCOMPARE(variant3.valueType(), Model::ValueType::INTEGER);
    QCOMPARE(variant3.canTranslateTo(Model::ValueType::INTEGER), true);
    QCOMPARE(variant3.toInteger(&ok), Model::Integer(2));
    QCOMPARE(ok, true);
}


void TestVariant::testIntegerToReal() {
    Model::Variant variant1(Model::Integer(2));
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::INTEGER);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::REAL), true);
    QCOMPARE(variant1.toReal(&ok), Model::Real(2));
    QCOMPARE(ok, true);
}


void TestVariant::testIntegerToComplex() {
    Model::Variant variant1(Model::Integer(2));
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::INTEGER);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::COMPLEX), true);
    QCOMPARE(variant1.toComplex(&ok), Model::Complex(2, 0));
    QCOMPARE(ok, true);
}


void TestVariant::testIntegerToSet() {
    Model::Variant variant1(Model::Integer(2));
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::INTEGER);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::SET), false);
    QCOMPARE(variant1.toSet(&ok), Model::Set());
    QCOMPARE(ok, false);
}


void TestVariant::testIntegerToTuple() {
    Model::Variant variant1(Model::Integer(2));
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::INTEGER);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::TUPLE), false);
    QCOMPARE(variant1.toTuple(&ok), Model::Tuple());
    QCOMPARE(ok, false);
}


void TestVariant::testIntegerToMatrixBoolean() {
    Model::Variant variant1(Model::Integer(2));
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::INTEGER);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::MATRIX_BOOLEAN), false);
    QCOMPARE(variant1.toMatrixBoolean(&ok), Model::MatrixBoolean());
    QCOMPARE(ok, false);
}


void TestVariant::testIntegerToMatrixInteger() {
    Model::Variant variant1(Model::Integer(2));
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::INTEGER);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::MATRIX_INTEGER), false);
    QCOMPARE(variant1.toMatrixInteger(&ok), Model::MatrixInteger());
    QCOMPARE(ok, false);
}


void TestVariant::testIntegerToMatrixReal() {
    Model::Variant variant1(Model::Integer(2));
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::INTEGER);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::MATRIX_REAL), false);
    QCOMPARE(variant1.toMatrixReal(&ok), Model::MatrixReal());
    QCOMPARE(ok, false);
}


void TestVariant::testIntegerToMatrixComplex() {
    Model::Variant variant1(Model::Integer(2));
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::INTEGER);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::MATRIX_COMPLEX), false);
    QCOMPARE(variant1.toMatrixComplex(&ok), Model::MatrixComplex());
    QCOMPARE(ok, false);
}

// additional TestVariant::testIntegerTo...

void TestVariant::testRealToNone() {
    Model::Variant variant1(Model::Real(2));

    QCOMPARE(variant1.valueType(), Model::ValueType::REAL);
    QCOMPARE(variant1.canTranslateTo(Model::ValueType::NONE), false);
}


void TestVariant::testRealToBoolean() {
    Model::Variant variant1(Model::Real(2));
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::REAL);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::BOOLEAN), true);
    QCOMPARE(variant1.toBoolean(&ok), Model::Boolean(true));
    QCOMPARE(ok, true);
}


void TestVariant::testRealToInteger() {
    Model::Variant variant1(Model::Real(2));
    Model::Variant variant2(Model::Real(2.5));
    Model::Variant variant3(2.0 * Model::Real(std::numeric_limits<Model::Integer>::max()));
    Model::Variant variant4(2.0 * Model::Real(std::numeric_limits<Model::Integer>::min()));
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::REAL);
    QCOMPARE(variant2.valueType(), Model::ValueType::REAL);
    QCOMPARE(variant3.valueType(), Model::ValueType::REAL);
    QCOMPARE(variant4.valueType(), Model::ValueType::REAL);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::INTEGER), true);
    QCOMPARE(variant1.toInteger(&ok), Model::Integer(2));
    QCOMPARE(ok, true);

    QCOMPARE(variant2.canTranslateTo(Model::ValueType::INTEGER), false);
    QCOMPARE(variant2.toInteger(&ok), Model::Integer(0));
    QCOMPARE(ok, false);

    QCOMPARE(variant3.canTranslateTo(Model::ValueType::INTEGER), false);
    QCOMPARE(variant3.toInteger(&ok), Model::Integer(0));
    QCOMPARE(ok, false);

    QCOMPARE(variant4.canTranslateTo(Model::ValueType::INTEGER), false);
    QCOMPARE(variant4.toInteger(&ok), Model::Integer(0));
    QCOMPARE(ok, false);
}


void TestVariant::testRealToReal() {
    Model::Variant variant1(Model::Real(2));
    Model::Variant variant2(Model::Real(2.5));
    Model::Variant variant3(variant2);
    Model::Variant variant4;
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::REAL);
    QCOMPARE(variant2.valueType(), Model::ValueType::REAL);
    QCOMPARE(variant3.valueType(), Model::ValueType::REAL);
    QCOMPARE(variant4.valueType(), Model::ValueType::NONE);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::REAL), true);
    QCOMPARE(variant1.toReal(&ok), Model::Real(2));
    QCOMPARE(ok, true);

    QCOMPARE(variant2.canTranslateTo(Model::ValueType::REAL), true);
    QCOMPARE(variant2.toReal(&ok), Model::Real(2.5));
    QCOMPARE(ok, true);

    QCOMPARE(variant3.canTranslateTo(Model::ValueType::REAL), true);
    QCOMPARE(variant3.toReal(&ok), Model::Real(2.5));
    QCOMPARE(ok, true);

    variant4 = variant3;

    QCOMPARE(variant4.valueType(), Model::ValueType::REAL);
    QCOMPARE(variant4.canTranslateTo(Model::ValueType::REAL), true);
    QCOMPARE(variant4.toReal(&ok), Model::Real(2.5));
    QCOMPARE(ok, true);
}


void TestVariant::testRealToComplex() {
    Model::Variant variant1(Model::Real(2));
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::REAL);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::COMPLEX), true);
    QCOMPARE(variant1.toComplex(&ok), Model::Complex(2, 0));
    QCOMPARE(ok, true);
}


void TestVariant::testRealToSet() {
    Model::Variant variant1(Model::Real(2));
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::REAL);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::SET), false);
    QCOMPARE(variant1.toSet(&ok), Model::Set());
    QCOMPARE(ok, false);
}


void TestVariant::testRealToTuple() {
    Model::Variant variant1(Model::Real(2));
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::REAL);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::TUPLE), false);
    QCOMPARE(variant1.toTuple(&ok), Model::Tuple());
    QCOMPARE(ok, false);
}


void TestVariant::testRealToMatrixBoolean() {
    Model::Variant variant1(Model::Real(2));
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::REAL);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::MATRIX_BOOLEAN), false);
    QCOMPARE(variant1.toMatrixBoolean(&ok), Model::MatrixBoolean());
    QCOMPARE(ok, false);
}


void TestVariant::testRealToMatrixInteger() {
    Model::Variant variant1(Model::Real(2));
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::REAL);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::MATRIX_INTEGER), false);
    QCOMPARE(variant1.toMatrixInteger(&ok), Model::MatrixInteger());
    QCOMPARE(ok, false);
}


void TestVariant::testRealToMatrixReal() {
    Model::Variant variant1(Model::Real(2));
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::REAL);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::MATRIX_REAL), false);
    QCOMPARE(variant1.toMatrixReal(&ok), Model::MatrixReal());
    QCOMPARE(ok, false);
}


void TestVariant::testRealToMatrixComplex() {
    Model::Variant variant1(Model::Real(2));
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::REAL);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::MATRIX_COMPLEX), false);
    QCOMPARE(variant1.toMatrixComplex(&ok), Model::MatrixComplex());
    QCOMPARE(ok, false);
}

// additional TestVariant::testRealTo...

void TestVariant::testComplexToNone() {
    Model::Variant variant1(Model::Complex(2,0));

    QCOMPARE(variant1.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(variant1.canTranslateTo(Model::ValueType::NONE), false);
}


void TestVariant::testComplexToBoolean() {
    Model::Variant variant(Model::Complex(2, 1));
    bool           ok;

    QCOMPARE(variant.valueType(), Model::ValueType::COMPLEX);

    QCOMPARE(variant.canTranslateTo(Model::ValueType::BOOLEAN), true);
    QCOMPARE(variant.toBoolean(&ok), Model::Boolean(true));
    QCOMPARE(ok, true);
}


void TestVariant::testComplexToInteger() {
    Model::Variant variant1(Model::Complex(2));
    Model::Variant variant2(Model::Complex(2.5));
    Model::Variant variant3(2.0 * Model::Complex(std::numeric_limits<Model::Integer>::max()));
    Model::Variant variant4(2.0 * Model::Complex(std::numeric_limits<Model::Integer>::min()));
    Model::Variant variant5(Model::Complex(2, 1));
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(variant2.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(variant3.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(variant4.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(variant5.valueType(), Model::ValueType::COMPLEX);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::INTEGER), true);
    QCOMPARE(variant1.toInteger(&ok), Model::Integer(2));
    QCOMPARE(ok, true);

    QCOMPARE(variant2.canTranslateTo(Model::ValueType::INTEGER), false);
    QCOMPARE(variant2.toInteger(&ok), Model::Integer(0));
    QCOMPARE(ok, false);

    QCOMPARE(variant3.canTranslateTo(Model::ValueType::INTEGER), false);
    QCOMPARE(variant3.toInteger(&ok), Model::Integer(0));
    QCOMPARE(ok, false);

    QCOMPARE(variant4.canTranslateTo(Model::ValueType::INTEGER), false);
    QCOMPARE(variant4.toInteger(&ok), Model::Integer(0));
    QCOMPARE(ok, false);

    QCOMPARE(variant5.canTranslateTo(Model::ValueType::INTEGER), false);
    QCOMPARE(variant5.toInteger(&ok), Model::Integer(0));
    QCOMPARE(ok, false);
}


void TestVariant::testComplexToReal() {
    Model::Variant variant1(Model::Complex(2));
    Model::Variant variant2(Model::Complex(2.5));
    Model::Variant variant3(Model::Complex(2, 1));
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(variant2.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(variant3.valueType(), Model::ValueType::COMPLEX);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::REAL), true);
    QCOMPARE(variant1.toReal(&ok), Model::Real(2));
    QCOMPARE(ok, true);

    QCOMPARE(variant2.canTranslateTo(Model::ValueType::REAL), true);
    QCOMPARE(variant2.toReal(&ok), Model::Real(2.5));
    QCOMPARE(ok, true);

    QCOMPARE(variant3.canTranslateTo(Model::ValueType::REAL), false);
    QCOMPARE(variant3.toReal(&ok), Model::Real(0));
    QCOMPARE(ok, false);
}


void TestVariant::testComplexToComplex() {
    Model::Variant variant1(Model::Complex(2));
    Model::Variant variant2(Model::Complex(2.5, 1.0));
    Model::Variant variant3(variant2);
    Model::Variant variant4;
    bool           ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(variant2.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(variant3.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(variant4.valueType(), Model::ValueType::NONE);

    QCOMPARE(variant1.canTranslateTo(Model::ValueType::COMPLEX), true);
    QCOMPARE(variant1.toComplex(&ok), Model::Complex(2));
    QCOMPARE(ok, true);

    QCOMPARE(variant2.canTranslateTo(Model::ValueType::COMPLEX), true);
    QCOMPARE(variant2.toComplex(&ok), Model::Complex(2.5, 1.0));
    QCOMPARE(ok, true);

    QCOMPARE(variant3.canTranslateTo(Model::ValueType::COMPLEX), true);
    QCOMPARE(variant3.toComplex(&ok), Model::Complex(2.5, 1.0));
    QCOMPARE(ok, true);

    variant4 = variant3;

    QCOMPARE(variant4.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(variant4.canTranslateTo(Model::ValueType::COMPLEX), true);
    QCOMPARE(variant4.toComplex(&ok), Model::Complex(2.5, 1.0));
    QCOMPARE(ok, true);
}


void TestVariant::testComplexToSet() {
    Model::Variant variant(Model::Complex(2, 1));
    bool           ok;

    QCOMPARE(variant.valueType(), Model::ValueType::COMPLEX);

    QCOMPARE(variant.canTranslateTo(Model::ValueType::SET), false);
    QCOMPARE(variant.toSet(&ok), Model::Set());
    QCOMPARE(ok, false);
}


void TestVariant::testComplexToTuple() {
    Model::Variant variant(Model::Complex(2, 1));
    bool           ok;

    QCOMPARE(variant.valueType(), Model::ValueType::COMPLEX);

    QCOMPARE(variant.canTranslateTo(Model::ValueType::TUPLE), false);
    QCOMPARE(variant.toTuple(&ok), Model::Tuple());
    QCOMPARE(ok, false);
}


void TestVariant::testComplexToMatrixBoolean() {
    Model::Variant variant(Model::Complex(2, 1));
    bool           ok;

    QCOMPARE(variant.valueType(), Model::ValueType::COMPLEX);

    QCOMPARE(variant.canTranslateTo(Model::ValueType::MATRIX_BOOLEAN), false);
    QCOMPARE(variant.toMatrixBoolean(&ok), Model::MatrixBoolean());
    QCOMPARE(ok, false);
}


void TestVariant::testComplexToMatrixInteger() {
    Model::Variant variant(Model::Complex(2, 1));
    bool           ok;

    QCOMPARE(variant.valueType(), Model::ValueType::COMPLEX);

    QCOMPARE(variant.canTranslateTo(Model::ValueType::MATRIX_INTEGER), false);
    QCOMPARE(variant.toMatrixInteger(&ok), Model::MatrixInteger());
    QCOMPARE(ok, false);
}


void TestVariant::testComplexToMatrixReal() {
    Model::Variant variant(Model::Complex(2, 1));
    bool           ok;

    QCOMPARE(variant.valueType(), Model::ValueType::COMPLEX);

    QCOMPARE(variant.canTranslateTo(Model::ValueType::MATRIX_REAL), false);
    QCOMPARE(variant.toMatrixReal(&ok), Model::MatrixReal());
    QCOMPARE(ok, false);
}


void TestVariant::testComplexToMatrixComplex() {
    Model::Variant variant(Model::Complex(2, 1));
    bool           ok;

    QCOMPARE(variant.valueType(), Model::ValueType::COMPLEX);

    QCOMPARE(variant.canTranslateTo(Model::ValueType::MATRIX_COMPLEX), false);
    QCOMPARE(variant.toMatrixComplex(&ok), Model::MatrixComplex());
    QCOMPARE(ok, false);
}

// additional TestVariant::testComplexTo...

void TestVariant::testSetToNone() {
    Model::Set set;
    set.insert(Model::Integer(1));
    set.insert(Model::Integer(2));
    set.insert(Model::Integer(3));

    Model::Variant variant(set);

    QCOMPARE(variant.valueType(), Model::ValueType::SET);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::NONE), false);
}


void TestVariant::testSetToBoolean() {
    Model::Set set;
    set.insert(Model::Integer(1));
    set.insert(Model::Integer(2));
    set.insert(Model::Integer(3));

    Model::Variant variant(set);

    QCOMPARE(variant.valueType(), Model::ValueType::SET);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::BOOLEAN), true);
}


void TestVariant::testSetToInteger() {
    Model::Set set;
    set.insert(Model::Integer(1));
    set.insert(Model::Integer(2));
    set.insert(Model::Integer(3));

    Model::Variant variant(set);

    QCOMPARE(variant.valueType(), Model::ValueType::SET);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::INTEGER), false);
}


void TestVariant::testSetToReal() {
    Model::Set set;
    set.insert(Model::Integer(1));
    set.insert(Model::Integer(2));
    set.insert(Model::Integer(3));

    Model::Variant variant(set);

    QCOMPARE(variant.valueType(), Model::ValueType::SET);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::REAL), false);
}


void TestVariant::testSetToComplex() {
    Model::Set set;
    set.insert(Model::Integer(1));
    set.insert(Model::Integer(2));
    set.insert(Model::Integer(3));

    Model::Variant variant(set);

    QCOMPARE(variant.valueType(), Model::ValueType::SET);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::COMPLEX), false);
}


void TestVariant::testSetToSet() {
    Model::Set set;
    set.insert(Model::Integer(1));
    set.insert(Model::Integer(2));
    set.insert(Model::Integer(3));

    Model::Variant variant(set);

    QCOMPARE(variant.valueType(), Model::ValueType::SET);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::SET), true);

    bool       ok;
    Model::Set set2 = variant.toSet(&ok);

    QCOMPARE(ok, true);
    QCOMPARE(set, set2);
}


void TestVariant::testSetToTuple() {
    Model::Set set;
    set.insert(Model::Integer(1));
    set.insert(Model::Integer(2));
    set.insert(Model::Integer(3));

    Model::Variant variant(set);

    QCOMPARE(variant.valueType(), Model::ValueType::SET);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::TUPLE), false);
}


void TestVariant::testSetToMatrixBoolean() {
    Model::Set set;
    set.insert(Model::Integer(1));
    set.insert(Model::Integer(2));
    set.insert(Model::Integer(3));

    Model::Variant variant(set);

    QCOMPARE(variant.valueType(), Model::ValueType::SET);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::MATRIX_BOOLEAN), false);
}


void TestVariant::testSetToMatrixInteger() {
    Model::Set set;
    set.insert(Model::Integer(1));
    set.insert(Model::Integer(2));
    set.insert(Model::Integer(3));

    Model::Variant variant(set);

    QCOMPARE(variant.valueType(), Model::ValueType::SET);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::MATRIX_INTEGER), false);
}


void TestVariant::testSetToMatrixReal() {
    Model::Set set;
    set.insert(Model::Integer(1));
    set.insert(Model::Integer(2));
    set.insert(Model::Integer(3));

    Model::Variant variant(set);

    QCOMPARE(variant.valueType(), Model::ValueType::SET);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::MATRIX_REAL), false);
}


void TestVariant::testSetToMatrixComplex() {
    Model::Set set;
    set.insert(Model::Integer(1));
    set.insert(Model::Integer(2));
    set.insert(Model::Integer(3));

    Model::Variant variant(set);

    QCOMPARE(variant.valueType(), Model::ValueType::SET);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::MATRIX_COMPLEX), false);
}

// additional TestVariant::testSetTo...

void TestVariant::testTupleToNone() {
    Model::Tuple tuple;
    tuple.append(Model::Integer(1));
    tuple.append(Model::Integer(2));
    tuple.append(Model::Integer(3));

    Model::Variant variant(tuple);

    QCOMPARE(variant.valueType(), Model::ValueType::TUPLE);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::NONE), false);
}


void TestVariant::testTupleToBoolean() {
    Model::Tuple tuple1;
    tuple1.append(Model::Integer(1));
    tuple1.append(Model::Integer(2));
    tuple1.append(Model::Integer(3));

    Model::Tuple tuple2;

    Model::Variant variant1(tuple1);
    Model::Variant variant2(tuple2);

    bool ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::TUPLE);
    QCOMPARE(variant1.canTranslateTo(Model::ValueType::BOOLEAN), true);

    QCOMPARE(variant1.toBoolean(&ok), true);
    QCOMPARE(ok, true);

    QCOMPARE(variant2.toBoolean(&ok), false);
    QCOMPARE(ok, true);
}


void TestVariant::testTupleToInteger() {
    Model::Tuple tuple;
    tuple.append(Model::Integer(1));
    tuple.append(Model::Integer(2));
    tuple.append(Model::Integer(3));

    Model::Variant variant(tuple);

    QCOMPARE(variant.valueType(), Model::ValueType::TUPLE);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::INTEGER), false);
}


void TestVariant::testTupleToReal() {
    Model::Tuple tuple;
    tuple.append(Model::Integer(1));
    tuple.append(Model::Integer(2));
    tuple.append(Model::Integer(3));

    Model::Variant variant(tuple);

    QCOMPARE(variant.valueType(), Model::ValueType::TUPLE);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::REAL), false);
}


void TestVariant::testTupleToComplex() {
    Model::Tuple tuple;
    tuple.append(Model::Integer(1));
    tuple.append(Model::Integer(2));
    tuple.append(Model::Integer(3));

    Model::Variant variant(tuple);

    QCOMPARE(variant.valueType(), Model::ValueType::TUPLE);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::COMPLEX), false);
}


void TestVariant::testTupleToSet() {
    Model::Tuple tuple;
    tuple.append(Model::Integer(1));
    tuple.append(Model::Integer(2));
    tuple.append(Model::Integer(3));

    Model::Variant variant(tuple);

    QCOMPARE(variant.valueType(), Model::ValueType::TUPLE);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::SET), false);
}


void TestVariant::testTupleToTuple() {
    Model::Tuple tuple1;
    tuple1.append(Model::Integer(1));
    tuple1.append(Model::Integer(2));
    tuple1.append(Model::Integer(3));

    Model::Tuple tuple2;

    Model::Variant variant1(tuple1);
    Model::Variant variant2(tuple2);

    bool ok;

    QCOMPARE(variant1.valueType(), Model::ValueType::TUPLE);
    QCOMPARE(variant1.canTranslateTo(Model::ValueType::TUPLE), true);

    QCOMPARE(variant1.toTuple(&ok), tuple1);
    QCOMPARE(ok, true);

    QCOMPARE(variant2.toTuple(&ok), tuple2);
    QCOMPARE(ok, true);
}


void TestVariant::testTupleToMatrixBoolean() {
    Model::Tuple tuple;
    tuple.append(Model::Integer(1));
    tuple.append(Model::Integer(2));
    tuple.append(Model::Integer(3));

    Model::Variant variant(tuple);

    QCOMPARE(variant.valueType(), Model::ValueType::TUPLE);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::MATRIX_BOOLEAN), false);
}


void TestVariant::testTupleToMatrixInteger() {
    Model::Tuple tuple;
    tuple.append(Model::Integer(1));
    tuple.append(Model::Integer(2));
    tuple.append(Model::Integer(3));

    Model::Variant variant(tuple);

    QCOMPARE(variant.valueType(), Model::ValueType::TUPLE);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::MATRIX_INTEGER), false);
}


void TestVariant::testTupleToMatrixReal() {
    Model::Tuple tuple;
    tuple.append(Model::Integer(1));
    tuple.append(Model::Integer(2));
    tuple.append(Model::Integer(3));

    Model::Variant variant(tuple);

    QCOMPARE(variant.valueType(), Model::ValueType::TUPLE);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::MATRIX_REAL), false);
}


void TestVariant::testTupleToMatrixComplex() {
    Model::Tuple tuple;
    tuple.append(Model::Integer(1));
    tuple.append(Model::Integer(2));
    tuple.append(Model::Integer(3));

    Model::Variant variant(tuple);

    QCOMPARE(variant.valueType(), Model::ValueType::TUPLE);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::MATRIX_COMPLEX), false);
}

// additional TestVariant::testTupleTo...

void TestVariant::testMatrixBooleanToNone() {
    Model::MatrixBoolean matrix = Model::MatrixBoolean::build(
        2, 2,
        true,  false,
        false, true
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_BOOLEAN);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::NONE), false);
}


void TestVariant::testMatrixBooleanToBoolean() {
    Model::MatrixBoolean matrix = Model::MatrixBoolean::build(
        2, 2,
        true,  false,
        false, true
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_BOOLEAN);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::BOOLEAN), false);

    bool success;
    QCOMPARE(variant.toBoolean(&success), Model::Boolean(false));
    QCOMPARE(success, false);
}


void TestVariant::testMatrixBooleanToInteger() {
    Model::MatrixBoolean matrix = Model::MatrixBoolean::build(
        2, 2,
        true,  false,
        false, true
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_BOOLEAN);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::INTEGER), false);

    bool success;
    QCOMPARE(variant.toInteger(&success), Model::Integer(0));
    QCOMPARE(success, false);
}


void TestVariant::testMatrixBooleanToReal() {
    Model::MatrixBoolean matrix = Model::MatrixBoolean::build(
        2, 2,
        true,  false,
        false, true
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_BOOLEAN);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::REAL), false);

    bool success;
    QCOMPARE(variant.toReal(&success), Model::Real(0));
    QCOMPARE(success, false);
}


void TestVariant::testMatrixBooleanToComplex() {
    Model::MatrixBoolean matrix = Model::MatrixBoolean::build(
        2, 2,
        true,  false,
        false, true
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_BOOLEAN);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::COMPLEX), false);

    bool success;
    QCOMPARE(variant.toComplex(&success), Model::Complex(0));
    QCOMPARE(success, false);
}


void TestVariant::testMatrixBooleanToSet() {
    Model::MatrixBoolean matrix = Model::MatrixBoolean::build(
        2, 2,
        true,  false,
        false, true
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_BOOLEAN);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::SET), false);
}


void TestVariant::testMatrixBooleanToTuple() {
    Model::MatrixBoolean matrix = Model::MatrixBoolean::build(
        2, 2,
        true,  false,
        false, true
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_BOOLEAN);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::TUPLE), false);
}


void TestVariant::testMatrixBooleanToMatrixBoolean() {
    Model::MatrixBoolean matrix = Model::MatrixBoolean::build(
        2, 2,
        true,  false,
        false, true
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_BOOLEAN);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::MATRIX_BOOLEAN), true);

    Model::MatrixBoolean expected = Model::MatrixBoolean::build(
        2, 2,
        true,  false,
        false, true
    );

    bool success;
    QCOMPARE(variant.toMatrixBoolean(&success), expected);
    QCOMPARE(success, true);
}


void TestVariant::testMatrixBooleanToMatrixInteger() {
    Model::MatrixBoolean matrix = Model::MatrixBoolean::build(
        2, 2,
        true,  false,
        false, true
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_BOOLEAN);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::MATRIX_INTEGER), true);

    Model::MatrixInteger expected = Model::MatrixInteger::build(
        2, 2,
        Model::Integer(1), Model::Integer(0),
        Model::Integer(0), Model::Integer(1)
    );

    bool success;
    QCOMPARE(variant.toMatrixInteger(&success), expected);
    QCOMPARE(success, true);
}


void TestVariant::testMatrixBooleanToMatrixReal() {
    Model::MatrixBoolean matrix = Model::MatrixBoolean::build(
        2, 2,
        true,  false,
        false, true
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_BOOLEAN);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::MATRIX_INTEGER), true);

    Model::MatrixReal expected = Model::MatrixReal::build(
        2, 2,
        Model::Real(1), Model::Real(0),
        Model::Real(0), Model::Real(1)
    );

    bool success;
    QCOMPARE(variant.toMatrixReal(&success), expected);
    QCOMPARE(success, true);
}


void TestVariant::testMatrixBooleanToMatrixComplex() {
    Model::MatrixBoolean matrix = Model::MatrixBoolean::build(
        2, 2,
        true,  false,
        false, true
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_BOOLEAN);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::MATRIX_INTEGER), true);

    Model::MatrixComplex expected = Model::MatrixComplex::build(
        2, 2,
        Model::Complex(1), Model::Complex(0),
        Model::Complex(0), Model::Complex(1)
    );

    bool success;
    QCOMPARE(variant.toMatrixComplex(&success), expected);
    QCOMPARE(success, true);
}

// additional testMatrixBooleanTo...

void TestVariant::testMatrixIntegerToNone() {
    Model::MatrixInteger matrix = Model::MatrixInteger::build(
        2, 2,
        Model::Integer(1), Model::Integer(0),
        Model::Integer(0), Model::Integer(1)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_INTEGER);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::NONE), false);
}


void TestVariant::testMatrixIntegerToBoolean() {
    Model::MatrixInteger matrix = Model::MatrixInteger::build(
        2, 2,
        Model::Integer(1), Model::Integer(0),
        Model::Integer(0), Model::Integer(1)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_INTEGER);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::BOOLEAN), false);

    bool success;
    QCOMPARE(variant.toBoolean(&success), Model::Boolean(false));
    QCOMPARE(success, false);
}


void TestVariant::testMatrixIntegerToInteger() {
    Model::MatrixInteger matrix = Model::MatrixInteger::build(
        2, 2,
        Model::Integer(1), Model::Integer(0),
        Model::Integer(0), Model::Integer(1)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_INTEGER);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::INTEGER), false);

    bool success;
    QCOMPARE(variant.toInteger(&success), Model::Integer(0));
    QCOMPARE(success, false);
}


void TestVariant::testMatrixIntegerToReal() {
    Model::MatrixInteger matrix = Model::MatrixInteger::build(
        2, 2,
        Model::Integer(1), Model::Integer(0),
        Model::Integer(0), Model::Integer(1)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_INTEGER);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::REAL), false);

    bool success;
    QCOMPARE(variant.toReal(&success), Model::Real(0));
    QCOMPARE(success, false);
}


void TestVariant::testMatrixIntegerToComplex() {
    Model::MatrixInteger matrix = Model::MatrixInteger::build(
        2, 2,
        Model::Integer(1), Model::Integer(0),
        Model::Integer(0), Model::Integer(1)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_INTEGER);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::COMPLEX), false);

    bool success;
    QCOMPARE(variant.toComplex(&success), Model::Complex(0));
    QCOMPARE(success, false);
}


void TestVariant::testMatrixIntegerToSet() {
    Model::MatrixInteger matrix = Model::MatrixInteger::build(
        2, 2,
        Model::Integer(1), Model::Integer(0),
        Model::Integer(0), Model::Integer(1)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_INTEGER);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::SET), false);
}


void TestVariant::testMatrixIntegerToTuple() {
    Model::MatrixInteger matrix = Model::MatrixInteger::build(
        2, 2,
        true,  false,
        false, true
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_INTEGER);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::TUPLE), false);
}


void TestVariant::testMatrixIntegerToMatrixBoolean() {
    Model::MatrixInteger matrix = Model::MatrixInteger::build(
        2, 2,
        Model::Integer(1), Model::Integer(0),
        Model::Integer(0), Model::Integer(1)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_INTEGER);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::MATRIX_BOOLEAN), true);

    Model::MatrixBoolean expected = Model::MatrixBoolean::build(
        2, 2,
        true,  false,
        false, true
    );

    bool success;
    QCOMPARE(variant.toMatrixBoolean(&success), expected);
    QCOMPARE(success, true);
}


void TestVariant::testMatrixIntegerToMatrixInteger() {
    Model::MatrixInteger matrix = Model::MatrixInteger::build(
        2, 2,
        Model::Integer(1), Model::Integer(0),
        Model::Integer(0), Model::Integer(1)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_INTEGER);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::MATRIX_INTEGER), true);

    Model::MatrixInteger expected = Model::MatrixInteger::build(
        2, 2,
        Model::Integer(1), Model::Integer(0),
        Model::Integer(0), Model::Integer(1)
    );

    bool success;
    QCOMPARE(variant.toMatrixInteger(&success), expected);
    QCOMPARE(success, true);
}


void TestVariant::testMatrixIntegerToMatrixReal() {
    Model::MatrixInteger matrix = Model::MatrixInteger::build(
        2, 2,
        Model::Integer(1), Model::Integer(0),
        Model::Integer(0), Model::Integer(1)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_INTEGER);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::MATRIX_REAL), true);

    Model::MatrixReal expected = Model::MatrixReal::build(
        2, 2,
        Model::Real(1), Model::Real(0),
        Model::Real(0), Model::Real(1)
    );

    bool success;
    QCOMPARE(variant.toMatrixReal(&success), expected);
    QCOMPARE(success, true);
}


void TestVariant::testMatrixIntegerToMatrixComplex() {
    Model::MatrixInteger matrix = Model::MatrixInteger::build(
        2, 2,
        Model::Integer(1), Model::Integer(0),
        Model::Integer(0), Model::Integer(1)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_INTEGER);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::MATRIX_REAL), true);

    Model::MatrixComplex expected = Model::MatrixComplex::build(
        2, 2,
        Model::Complex(1), Model::Complex(0),
        Model::Complex(0), Model::Complex(1)
    );

    bool success;
    QCOMPARE(variant.toMatrixComplex(&success), expected);
    QCOMPARE(success, true);
}

// additional testMatrixIntegerTo...

void TestVariant::testMatrixRealToNone() {
    Model::MatrixReal matrix = Model::MatrixReal::build(
        2, 2,
        Model::Real(1), Model::Real(0),
        Model::Real(0), Model::Real(1)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_REAL);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::NONE), false);
}


void TestVariant::testMatrixRealToBoolean() {
    Model::MatrixReal matrix = Model::MatrixReal::build(
        2, 2,
        Model::Real(1), Model::Real(0),
        Model::Real(0), Model::Real(1)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_REAL);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::BOOLEAN), false);

    bool success;
    QCOMPARE(variant.toBoolean(&success), Model::Boolean(false));
    QCOMPARE(success, false);
}


void TestVariant::testMatrixRealToInteger() {
    Model::MatrixReal matrix = Model::MatrixReal::build(
        2, 2,
        Model::Real(1), Model::Real(0),
        Model::Real(0), Model::Real(1)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_REAL);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::INTEGER), false);

    bool success;
    QCOMPARE(variant.toInteger(&success), Model::Integer(0));
    QCOMPARE(success, false);
}


void TestVariant::testMatrixRealToReal() {
    Model::MatrixReal matrix = Model::MatrixReal::build(
        2, 2,
        Model::Real(1), Model::Real(0),
        Model::Real(0), Model::Real(1)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_REAL);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::REAL), false);

    bool success;
    QCOMPARE(variant.toReal(&success), Model::Real(0));
    QCOMPARE(success, false);
}


void TestVariant::testMatrixRealToComplex() {
    Model::MatrixReal matrix = Model::MatrixReal::build(
        2, 2,
        Model::Real(1), Model::Real(0),
        Model::Real(0), Model::Real(1)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_REAL);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::COMPLEX), false);

    bool success;
    QCOMPARE(variant.toComplex(&success), Model::Complex(0));
    QCOMPARE(success, false);
}


void TestVariant::testMatrixRealToSet() {
    Model::MatrixReal matrix = Model::MatrixReal::build(
        2, 2,
        Model::Real(1), Model::Real(0),
        Model::Real(0), Model::Real(1)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_REAL);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::SET), false);
}


void TestVariant::testMatrixRealToTuple() {
    Model::MatrixReal matrix = Model::MatrixReal::build(
        2, 2,
        Model::Real(1), Model::Real(0),
        Model::Real(0), Model::Real(1)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_REAL);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::TUPLE), false);
}


void TestVariant::testMatrixRealToMatrixBoolean() {
    Model::MatrixReal matrix = Model::MatrixReal::build(
        2, 2,
        Model::Real(1), Model::Real(0),
        Model::Real(0), Model::Real(1)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_REAL);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::MATRIX_BOOLEAN), true);

    Model::MatrixBoolean expected = Model::MatrixBoolean::build(
        2, 2,
        true,  false,
        false, true
    );

    bool success;
    QCOMPARE(variant.toMatrixBoolean(&success), expected);
    QCOMPARE(success, true);
}


void TestVariant::testMatrixRealToMatrixInteger() {
    Model::MatrixReal matrix = Model::MatrixReal::build(
        2, 2,
        Model::Real(1), Model::Real(0),
        Model::Real(0), Model::Real(1)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_REAL);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::MATRIX_INTEGER), false);

    bool success;
    QCOMPARE(variant.toMatrixInteger(&success), Model::MatrixInteger());
    QCOMPARE(success, false);
}


void TestVariant::testMatrixRealToMatrixReal() {
    Model::MatrixReal matrix = Model::MatrixReal::build(
        2, 2,
        Model::Real(1), Model::Real(0),
        Model::Real(0), Model::Real(1)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_REAL);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::MATRIX_REAL), true);

    Model::MatrixReal expected = Model::MatrixReal::build(
        2, 2,
        Model::Real(1), Model::Real(0),
        Model::Real(0), Model::Real(1)
    );

    bool success;
    QCOMPARE(variant.toMatrixReal(&success), expected);
    QCOMPARE(success, true);
}


void TestVariant::testMatrixRealToMatrixComplex() {
    Model::MatrixReal matrix = Model::MatrixReal::build(
        2, 2,
        Model::Real(1), Model::Real(0),
        Model::Real(0), Model::Real(1)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_REAL);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::MATRIX_COMPLEX), true);

    Model::MatrixComplex expected = Model::MatrixComplex::build(
        2, 2,
        Model::Complex(1), Model::Complex(0),
        Model::Complex(0), Model::Complex(1)
    );

    bool success;
    QCOMPARE(variant.toMatrixComplex(&success), expected);
    QCOMPARE(success, true);
}

// additional testMatrixRealTo...

void TestVariant::testMatrixComplexToNone() {
    Model::MatrixComplex matrix = Model::MatrixComplex::build(
        2, 2,
        Model::Complex(1, 1), Model::Complex(2, 1),
        Model::Complex(1, 2), Model::Complex(2, 2)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_COMPLEX);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::NONE), false);
}


void TestVariant::testMatrixComplexToBoolean() {
    Model::MatrixComplex matrix = Model::MatrixComplex::build(
        2, 2,
        Model::Complex(1, 1), Model::Complex(2, 1),
        Model::Complex(1, 2), Model::Complex(2, 2)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_COMPLEX);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::BOOLEAN), false);

    bool success;
    QCOMPARE(variant.toBoolean(&success), Model::Boolean(false));
    QCOMPARE(success, false);
}


void TestVariant::testMatrixComplexToInteger() {
    Model::MatrixComplex matrix = Model::MatrixComplex::build(
        2, 2,
        Model::Complex(1, 1), Model::Complex(2, 1),
        Model::Complex(1, 2), Model::Complex(2, 2)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_COMPLEX);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::INTEGER), false);

    bool success;
    QCOMPARE(variant.toInteger(&success), Model::Integer(0));
    QCOMPARE(success, false);
}


void TestVariant::testMatrixComplexToReal() {
    Model::MatrixComplex matrix = Model::MatrixComplex::build(
        2, 2,
        Model::Complex(1, 1), Model::Complex(2, 1),
        Model::Complex(1, 2), Model::Complex(2, 2)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_COMPLEX);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::REAL), false);

    bool success;
    QCOMPARE(variant.toReal(&success), Model::Real(0));
    QCOMPARE(success, false);
}


void TestVariant::testMatrixComplexToComplex() {
    Model::MatrixComplex matrix = Model::MatrixComplex::build(
        2, 2,
        Model::Complex(1, 1), Model::Complex(2, 1),
        Model::Complex(1, 2), Model::Complex(2, 2)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_COMPLEX);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::COMPLEX), false);

    bool success;
    QCOMPARE(variant.toComplex(&success), Model::Complex(0));
    QCOMPARE(success, false);
}


void TestVariant::testMatrixComplexToSet() {
    Model::MatrixComplex matrix = Model::MatrixComplex::build(
        2, 2,
        Model::Complex(1, 1), Model::Complex(2, 1),
        Model::Complex(1, 2), Model::Complex(2, 2)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_COMPLEX);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::SET), false);
}


void TestVariant::testMatrixComplexToTuple() {
    Model::MatrixComplex matrix = Model::MatrixComplex::build(
        2, 2,
        Model::Complex(1, 1), Model::Complex(2, 1),
        Model::Complex(1, 2), Model::Complex(2, 2)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_COMPLEX);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::TUPLE), false);
}


void TestVariant::testMatrixComplexToMatrixBoolean() {
    Model::MatrixComplex matrix = Model::MatrixComplex::build(
        2, 2,
        Model::Complex(1, 1), Model::Complex(2, 1),
        Model::Complex(1, 2), Model::Complex(2, 2)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_COMPLEX);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::MATRIX_BOOLEAN), true);
}


void TestVariant::testMatrixComplexToMatrixInteger() {
    Model::MatrixComplex matrix = Model::MatrixComplex::build(
        2, 2,
        Model::Complex(1, 1), Model::Complex(2, 1),
        Model::Complex(1, 2), Model::Complex(2, 2)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_COMPLEX);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::MATRIX_INTEGER), false);
}


void TestVariant::testMatrixComplexToMatrixReal() {
    Model::MatrixComplex matrix = Model::MatrixComplex::build(
        2, 2,
        Model::Complex(1, 1), Model::Complex(2, 1),
        Model::Complex(1, 2), Model::Complex(2, 2)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_COMPLEX);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::MATRIX_REAL), false);
}


void TestVariant::testMatrixComplexToMatrixComplex() {
    Model::MatrixComplex matrix = Model::MatrixComplex::build(
        2, 2,
        Model::Complex(1, 1), Model::Complex(2, 1),
        Model::Complex(1, 2), Model::Complex(2, 2)
    );

    Model::Variant variant(matrix);

    QCOMPARE(variant.valueType(), Model::ValueType::MATRIX_COMPLEX);
    QCOMPARE(variant.canTranslateTo(Model::ValueType::MATRIX_COMPLEX), true);

    Model::MatrixComplex expected = Model::MatrixComplex::build(
        2, 2,
        Model::Complex(1, 1), Model::Complex(2, 1),
        Model::Complex(1, 2), Model::Complex(2, 2)
    );

    bool success;
    QCOMPARE(variant.toMatrixComplex(&success), expected);
    QCOMPARE(success, true);
}

// additional testMatrixComplexTo...

void TestVariant::testComparisonTypesAgainstNone() {
    Model::Set           set;
    Model::Tuple         tuple;
    Model::MatrixBoolean matrixBoolean;
    Model::MatrixInteger matrixInteger;
    Model::MatrixReal    matrixReal;
    Model::MatrixComplex matrixComplex;

    Model::Variant vNone;
    Model::Variant vBoolean(Model::Boolean(true));
    Model::Variant vInteger(Model::Integer(1));
    Model::Variant vReal(Model::Real(1));
    Model::Variant vComplex(Model::Complex(1, 1));
    Model::Variant vSet(set);
    Model::Variant vTuple(tuple);
    Model::Variant vMatrixBoolean(matrixBoolean);
    Model::Variant vMatrixInteger(matrixInteger);
    Model::Variant vMatrixReal(matrixReal);
    Model::Variant vMatrixComplex(matrixComplex);

    QCOMPARE(vNone == vNone, true);
    QCOMPARE(vNone != vNone, false);
    QCOMPARE(vNone <  vNone, false);
    QCOMPARE(vNone >  vNone, false);
    QCOMPARE(vNone <= vNone, true);
    QCOMPARE(vNone >= vNone, true);

    QCOMPARE(vNone == vBoolean, false);
    QCOMPARE(vNone != vBoolean, true);
    QCOMPARE(vNone <  vBoolean, true);
    QCOMPARE(vNone >  vBoolean, false);
    QCOMPARE(vNone <= vBoolean, true);
    QCOMPARE(vNone >= vBoolean, false);

    QCOMPARE(vNone == vInteger, false);
    QCOMPARE(vNone != vInteger, true);
    QCOMPARE(vNone <  vInteger, true);
    QCOMPARE(vNone >  vInteger, false);
    QCOMPARE(vNone <= vInteger, true);
    QCOMPARE(vNone >= vInteger, false);

    QCOMPARE(vNone == vReal, false);
    QCOMPARE(vNone != vReal, true);
    QCOMPARE(vNone <  vReal, true);
    QCOMPARE(vNone >  vReal, false);
    QCOMPARE(vNone <= vReal, true);
    QCOMPARE(vNone >= vReal, false);

    QCOMPARE(vNone == vComplex, false);
    QCOMPARE(vNone != vComplex, true);
    QCOMPARE(vNone <  vComplex, true);
    QCOMPARE(vNone >  vComplex, false);
    QCOMPARE(vNone <= vComplex, true);
    QCOMPARE(vNone >= vComplex, false);

    QCOMPARE(vNone == vSet, false);
    QCOMPARE(vNone != vSet, true);
    QCOMPARE(vNone <  vSet, true);
    QCOMPARE(vNone >  vSet, false);
    QCOMPARE(vNone <= vSet, true);
    QCOMPARE(vNone >= vSet, false);

    QCOMPARE(vNone == vTuple, false);
    QCOMPARE(vNone != vTuple, true);
    QCOMPARE(vNone <  vTuple, true);
    QCOMPARE(vNone >  vTuple, false);
    QCOMPARE(vNone <= vTuple, true);
    QCOMPARE(vNone >= vTuple, false);

    QCOMPARE(vNone == vMatrixBoolean, false);
    QCOMPARE(vNone != vMatrixBoolean, true);
    QCOMPARE(vNone <  vMatrixBoolean, true);
    QCOMPARE(vNone >  vMatrixBoolean, false);
    QCOMPARE(vNone <= vMatrixBoolean, true);
    QCOMPARE(vNone >= vMatrixBoolean, false);

    QCOMPARE(vNone == vMatrixInteger, false);
    QCOMPARE(vNone != vMatrixInteger, true);
    QCOMPARE(vNone <  vMatrixInteger, true);
    QCOMPARE(vNone >  vMatrixInteger, false);
    QCOMPARE(vNone <= vMatrixInteger, true);
    QCOMPARE(vNone >= vMatrixInteger, false);

    QCOMPARE(vNone == vMatrixReal, false);
    QCOMPARE(vNone != vMatrixReal, true);
    QCOMPARE(vNone <  vMatrixReal, true);
    QCOMPARE(vNone >  vMatrixReal, false);
    QCOMPARE(vNone <= vMatrixReal, true);
    QCOMPARE(vNone >= vMatrixReal, false);

    QCOMPARE(vNone == vMatrixComplex, false);
    QCOMPARE(vNone != vMatrixComplex, true);
    QCOMPARE(vNone <  vMatrixComplex, true);
    QCOMPARE(vNone >  vMatrixComplex, false);
    QCOMPARE(vNone <= vMatrixComplex, true);
    QCOMPARE(vNone >= vMatrixComplex, false);
}


void TestVariant::testComparisonTypesAgainstBoolean() {
    Model::Set           set;
    Model::Tuple         tuple;
    Model::MatrixBoolean matrixBoolean;
    Model::MatrixInteger matrixInteger;
    Model::MatrixReal    matrixReal;
    Model::MatrixComplex matrixComplex;

    Model::Variant vNone;
    Model::Variant vBoolean0(Model::Boolean(false));
    Model::Variant vInteger1(Model::Integer(1));
    Model::Variant vReal1(Model::Real(1));
    Model::Variant vComplex10(Model::Complex(1,0));
    Model::Variant vSet(set);
    Model::Variant vTuple(tuple);
    Model::Variant vMatrixBoolean(matrixBoolean);
    Model::Variant vMatrixInteger(matrixInteger);
    Model::Variant vMatrixReal(matrixReal);
    Model::Variant vMatrixComplex(matrixComplex);

    Model::Variant vInteger2(Model::Integer(2));
    Model::Variant vBoolean1(Model::Boolean(true));
    Model::Variant vReal05(Model::Real(0.5));
    Model::Variant vComplex005(Model::Complex(0, 0.5));

    QCOMPARE(vBoolean0 == vNone, false);
    QCOMPARE(vBoolean0 != vNone, true);
    QCOMPARE(vBoolean0 <  vNone, false);
    QCOMPARE(vBoolean0 >  vNone, true);
    QCOMPARE(vBoolean0 <= vNone, false);
    QCOMPARE(vBoolean0 >= vNone, true);

    QCOMPARE(vBoolean0 == vBoolean0, true);
    QCOMPARE(vBoolean0 != vBoolean0, false);
    QCOMPARE(vBoolean0 <  vBoolean0, false);
    QCOMPARE(vBoolean0 >  vBoolean0, false);
    QCOMPARE(vBoolean0 <= vBoolean0, true);
    QCOMPARE(vBoolean0 >= vBoolean0, true);

    QCOMPARE(vBoolean0 == vBoolean1, false);
    QCOMPARE(vBoolean0 != vBoolean1, true);
    QCOMPARE(vBoolean0 <  vBoolean1, true);
    QCOMPARE(vBoolean0 >  vBoolean1, false);
    QCOMPARE(vBoolean0 <= vBoolean1, true);
    QCOMPARE(vBoolean0 >= vBoolean1, false);

    QCOMPARE(vBoolean0 == vInteger1, false);
    QCOMPARE(vBoolean0 != vInteger1, true);
    QCOMPARE(vBoolean0 <  vInteger1, true);
    QCOMPARE(vBoolean0 >  vInteger1, false);
    QCOMPARE(vBoolean0 <= vInteger1, true);
    QCOMPARE(vBoolean0 >= vInteger1, false);

    QCOMPARE(vBoolean0 == vInteger2, false);
    QCOMPARE(vBoolean0 != vInteger2, true);
    QCOMPARE(vBoolean0 <  vInteger2, true);
    QCOMPARE(vBoolean0 >  vInteger2, false);
    QCOMPARE(vBoolean0 <= vInteger2, true);
    QCOMPARE(vBoolean0 >= vInteger2, false);

    QCOMPARE(vBoolean1 == vInteger1, true);
    QCOMPARE(vBoolean1 != vInteger1, false);
    QCOMPARE(vBoolean1 <  vInteger1, false);
    QCOMPARE(vBoolean1 >  vInteger1, false);
    QCOMPARE(vBoolean1 <= vInteger1, true);
    QCOMPARE(vBoolean1 >= vInteger1, true);

    QCOMPARE(vBoolean0 == vReal1, false);
    QCOMPARE(vBoolean0 != vReal1, true);
    QCOMPARE(vBoolean0 <  vReal1, true);
    QCOMPARE(vBoolean0 >  vReal1, false);
    QCOMPARE(vBoolean0 <= vReal1, true);
    QCOMPARE(vBoolean0 >= vReal1, false);

    QCOMPARE(vBoolean0 == vReal05, false);
    QCOMPARE(vBoolean0 != vReal05, true);
    QCOMPARE(vBoolean0 <  vReal05, true);
    QCOMPARE(vBoolean0 >  vReal05, false);
    QCOMPARE(vBoolean0 <= vReal05, true);
    QCOMPARE(vBoolean0 >= vReal05, false);

    QCOMPARE(vBoolean0 == vComplex10, false);
    QCOMPARE(vBoolean0 != vComplex10, true);
    QCOMPARE(vBoolean0 <  vComplex10, true);
    QCOMPARE(vBoolean0 >  vComplex10, false);
    QCOMPARE(vBoolean0 <= vComplex10, true);
    QCOMPARE(vBoolean0 >= vComplex10, false);

    QCOMPARE(vBoolean0 == vComplex005, false);
    QCOMPARE(vBoolean0 != vComplex005, true);
}


void TestVariant::testComparisonTypesAgainstInteger() {
    Model::Set           set;
    Model::Tuple         tuple;
    Model::MatrixBoolean matrixBoolean;
    Model::MatrixInteger matrixInteger;
    Model::MatrixReal    matrixReal;
    Model::MatrixComplex matrixComplex;

    Model::Variant vNone;
    Model::Variant vBoolean0(Model::Boolean(false));
    Model::Variant vInteger1(Model::Integer(1));
    Model::Variant vReal1(Model::Real(1));
    Model::Variant vComplex10(Model::Complex(1,0));
    Model::Variant vSet(set);
    Model::Variant vTuple(tuple);
    Model::Variant vMatrixBoolean(matrixBoolean);
    Model::Variant vMatrixInteger(matrixInteger);
    Model::Variant vMatrixReal(matrixReal);
    Model::Variant vMatrixComplex(matrixComplex);

    Model::Variant vInteger2(Model::Integer(2));
    Model::Variant vBoolean1(Model::Boolean(true));
    Model::Variant vReal05(Model::Real(0.5));
    Model::Variant vReal2(Model::Real(2));
    Model::Variant vComplex005(Model::Complex(0, 0.5));
    Model::Variant vComplex33(Model::Complex(3, 3));

    QCOMPARE(vInteger1 == vNone, false);
    QCOMPARE(vInteger1 != vNone, true);
    QCOMPARE(vInteger1 <  vNone, false);
    QCOMPARE(vInteger1 >  vNone, true);
    QCOMPARE(vInteger1 <= vNone, false);
    QCOMPARE(vInteger1 >= vNone, true);

    QCOMPARE(vInteger1 == vBoolean0, false);
    QCOMPARE(vInteger1 != vBoolean0, true);
    QCOMPARE(vInteger1 <  vBoolean0, false);
    QCOMPARE(vInteger1 >  vBoolean0, true);
    QCOMPARE(vInteger1 <= vBoolean0, false);
    QCOMPARE(vInteger1 >= vBoolean0, true);

    QCOMPARE(vInteger1 == vBoolean1, true);
    QCOMPARE(vInteger1 != vBoolean1, false);
    QCOMPARE(vInteger1 <  vBoolean1, false);
    QCOMPARE(vInteger1 >  vBoolean1, false);
    QCOMPARE(vInteger1 <= vBoolean1, true);
    QCOMPARE(vInteger1 >= vBoolean1, true);

    QCOMPARE(vInteger1 == vInteger1, true);
    QCOMPARE(vInteger1 != vInteger1, false);
    QCOMPARE(vInteger1 <  vInteger1, false);
    QCOMPARE(vInteger1 >  vInteger1, false);
    QCOMPARE(vInteger1 <= vInteger1, true);
    QCOMPARE(vInteger1 >= vInteger1, true);

    QCOMPARE(vInteger1 == vInteger2, false);
    QCOMPARE(vInteger1 != vInteger2, true);
    QCOMPARE(vInteger1 <  vInteger2, true);
    QCOMPARE(vInteger1 >  vInteger2, false);
    QCOMPARE(vInteger1 <= vInteger2, true);
    QCOMPARE(vInteger1 >= vInteger2, false);

    QCOMPARE(vInteger2 == vInteger1, false);
    QCOMPARE(vInteger2 != vInteger1, true);
    QCOMPARE(vInteger2 <  vInteger1, false);
    QCOMPARE(vInteger2 >  vInteger1, true);
    QCOMPARE(vInteger2 <= vInteger1, false);
    QCOMPARE(vInteger2 >= vInteger1, true);

    QCOMPARE(vInteger1 == vReal1, true);
    QCOMPARE(vInteger1 != vReal1, false);
    QCOMPARE(vInteger1 <  vReal1, false);
    QCOMPARE(vInteger1 >  vReal1, false);
    QCOMPARE(vInteger1 <= vReal1, true);
    QCOMPARE(vInteger1 >= vReal1, true);

    QCOMPARE(vInteger1 == vReal05, false);
    QCOMPARE(vInteger1 != vReal05, true);
    QCOMPARE(vInteger1 <  vReal05, false);
    QCOMPARE(vInteger1 >  vReal05, true);
    QCOMPARE(vInteger1 <= vReal05, false);
    QCOMPARE(vInteger1 >= vReal05, true);

    QCOMPARE(vInteger1 == vReal2, false);
    QCOMPARE(vInteger1 != vReal2, true);
    QCOMPARE(vInteger1 <  vReal2, true);
    QCOMPARE(vInteger1 >  vReal2, false);
    QCOMPARE(vInteger1 <= vReal2, true);
    QCOMPARE(vInteger1 >= vReal2, false);

    QCOMPARE(vInteger1 == vComplex10, true);
    QCOMPARE(vInteger1 != vComplex10, false);
    QCOMPARE(vInteger1 <  vComplex10, false);
    QCOMPARE(vInteger1 >  vComplex10, false);
    QCOMPARE(vInteger1 <= vComplex10, true);
    QCOMPARE(vInteger1 >= vComplex10, true);

    QCOMPARE(vInteger1 == vComplex33, false);
    QCOMPARE(vInteger1 != vComplex33, true);
}


void TestVariant::testComparisonTypesAgainstReal() {
    Model::Set           set;
    Model::Tuple         tuple;
    Model::MatrixBoolean matrixBoolean;
    Model::MatrixInteger matrixInteger;
    Model::MatrixReal    matrixReal;
    Model::MatrixComplex matrixComplex;

    Model::Variant vNone;
    Model::Variant vBoolean0(Model::Boolean(false));
    Model::Variant vInteger1(Model::Integer(1));
    Model::Variant vReal1(Model::Real(1));
    Model::Variant vComplex10(Model::Complex(1,0));
    Model::Variant vSet(set);
    Model::Variant vTuple(tuple);
    Model::Variant vMatrixBoolean(matrixBoolean);
    Model::Variant vMatrixInteger(matrixInteger);
    Model::Variant vMatrixReal(matrixReal);
    Model::Variant vMatrixComplex(matrixComplex);

    Model::Variant vBoolean1(Model::Boolean(true));
    Model::Variant vInteger2(Model::Integer(2));
    Model::Variant vReal05(Model::Real(0.5));
    Model::Variant vReal2(Model::Real(2));
    Model::Variant vComplex005(Model::Complex(0, 0.5));
    Model::Variant vComplex33(Model::Complex(3, 3));

    QCOMPARE(vReal1 == vNone, false);
    QCOMPARE(vReal1 != vNone, true);
    QCOMPARE(vReal1 <  vNone, false);
    QCOMPARE(vReal1 >  vNone, true);
    QCOMPARE(vReal1 <= vNone, false);
    QCOMPARE(vReal1 >= vNone, true);

    QCOMPARE(vReal1 == vBoolean0, false);
    QCOMPARE(vReal1 != vBoolean0, true);
    QCOMPARE(vReal1 <  vBoolean0, false);
    QCOMPARE(vReal1 >  vBoolean0, true);
    QCOMPARE(vReal1 <= vBoolean0, false);
    QCOMPARE(vReal1 >= vBoolean0, true);

    QCOMPARE(vReal1 == vBoolean1, true);
    QCOMPARE(vReal1 != vBoolean1, false);
    QCOMPARE(vReal1 <  vBoolean1, false);
    QCOMPARE(vReal1 >  vBoolean1, false);
    QCOMPARE(vReal1 <= vBoolean1, true);
    QCOMPARE(vReal1 >= vBoolean1, true);

    QCOMPARE(vReal1 == vInteger1, true);
    QCOMPARE(vReal1 != vInteger1, false);
    QCOMPARE(vReal1 <  vInteger1, false);
    QCOMPARE(vReal1 >  vInteger1, false);
    QCOMPARE(vReal1 <= vInteger1, true);
    QCOMPARE(vReal1 >= vInteger1, true);

    QCOMPARE(vReal1 == vInteger2, false);
    QCOMPARE(vReal1 != vInteger2, true);
    QCOMPARE(vReal1 <  vInteger2, true);
    QCOMPARE(vReal1 >  vInteger2, false);
    QCOMPARE(vReal1 <= vInteger2, true);
    QCOMPARE(vReal1 >= vInteger2, false);

    QCOMPARE(vReal1 == vReal1, true);
    QCOMPARE(vReal1 != vReal1, false);
    QCOMPARE(vReal1 <  vReal1, false);
    QCOMPARE(vReal1 >  vReal1, false);
    QCOMPARE(vReal1 <= vReal1, true);
    QCOMPARE(vReal1 >= vReal1, true);

    QCOMPARE(vReal1 == vReal05, false);
    QCOMPARE(vReal1 != vReal05, true);
    QCOMPARE(vReal1 <  vReal05, false);
    QCOMPARE(vReal1 >  vReal05, true);
    QCOMPARE(vReal1 <= vReal05, false);
    QCOMPARE(vReal1 >= vReal05, true);

    QCOMPARE(vReal1 == vReal2, false);
    QCOMPARE(vReal1 != vReal2, true);
    QCOMPARE(vReal1 <  vReal2, true);
    QCOMPARE(vReal1 >  vReal2, false);
    QCOMPARE(vReal1 <= vReal2, true);
    QCOMPARE(vReal1 >= vReal2, false);

    QCOMPARE(vReal2 == vReal1, false);
    QCOMPARE(vReal2 != vReal1, true);
    QCOMPARE(vReal2 <  vReal1, false);
    QCOMPARE(vReal2 >  vReal1, true);
    QCOMPARE(vReal2 <= vReal1, false);
    QCOMPARE(vReal2 >= vReal1, true);

    QCOMPARE(vReal1 == vComplex10, true);
    QCOMPARE(vReal1 != vComplex10, false);
    QCOMPARE(vReal1 <  vComplex10, false);
    QCOMPARE(vReal1 >  vComplex10, false);
    QCOMPARE(vReal1 <= vComplex10, true);
    QCOMPARE(vReal1 >= vComplex10, true);

    QCOMPARE(vReal1 == vComplex33, false);
    QCOMPARE(vReal1 != vComplex33, true);
}


void TestVariant::testComparisonTypesAgainstComplex() {
    Model::Set           set;
    Model::Tuple         tuple;
    Model::MatrixBoolean matrixBoolean;
    Model::MatrixInteger matrixInteger;
    Model::MatrixReal    matrixReal;
    Model::MatrixComplex matrixComplex;

    Model::Variant vNone;
    Model::Variant vBoolean0(Model::Boolean(false));
    Model::Variant vInteger1(Model::Integer(1));
    Model::Variant vReal1(Model::Real(1));
    Model::Variant vComplex10(Model::Complex(1,0));
    Model::Variant vSet(set);
    Model::Variant vTuple(tuple);
    Model::Variant vMatrixBoolean(matrixBoolean);
    Model::Variant vMatrixInteger(matrixInteger);
    Model::Variant vMatrixReal(matrixReal);
    Model::Variant vMatrixComplex(matrixComplex);

    Model::Variant vBoolean1(Model::Boolean(true));
    Model::Variant vInteger2(Model::Integer(2));
    Model::Variant vReal05(Model::Real(0.5));
    Model::Variant vReal2(Model::Real(2));
    Model::Variant vComplex005(Model::Complex(0, 0.5));
    Model::Variant vComplex33(Model::Complex(3, 3));

    QCOMPARE(vComplex10 == vNone, false);
    QCOMPARE(vComplex10 != vNone, true);
    QCOMPARE(vComplex10 <  vNone, false);
    QCOMPARE(vComplex10 >  vNone, true);
    QCOMPARE(vComplex10 <= vNone, false);
    QCOMPARE(vComplex10 >= vNone, true);

    QCOMPARE(vComplex10 == vBoolean0, false);
    QCOMPARE(vComplex10 != vBoolean0, true);
    QCOMPARE(vComplex10 <  vBoolean0, false);
    QCOMPARE(vComplex10 >  vBoolean0, true);
    QCOMPARE(vComplex10 <= vBoolean0, false);
    QCOMPARE(vComplex10 >= vBoolean0, true);

    QCOMPARE(vComplex10 == vBoolean1, true);
    QCOMPARE(vComplex10 != vBoolean1, false);
    QCOMPARE(vComplex10 <  vBoolean1, false);
    QCOMPARE(vComplex10 >  vBoolean1, false);
    QCOMPARE(vComplex10 <= vBoolean1, true);
    QCOMPARE(vComplex10 >= vBoolean1, true);

    QCOMPARE(vComplex10 == vInteger1, true);
    QCOMPARE(vComplex10 != vInteger1, false);
    QCOMPARE(vComplex10 <  vInteger1, false);
    QCOMPARE(vComplex10 >  vInteger1, false);
    QCOMPARE(vComplex10 <= vInteger1, true);
    QCOMPARE(vComplex10 >= vInteger1, true);

    QCOMPARE(vComplex10 == vInteger2, false);
    QCOMPARE(vComplex10 != vInteger2, true);
    QCOMPARE(vComplex10 <  vInteger2, true);
    QCOMPARE(vComplex10 >  vInteger2, false);
    QCOMPARE(vComplex10 <= vInteger2, true);
    QCOMPARE(vComplex10 >= vInteger2, false);

    QCOMPARE(vComplex10 == vReal1, true);
    QCOMPARE(vComplex10 != vReal1, false);
    QCOMPARE(vComplex10 <  vReal1, false);
    QCOMPARE(vComplex10 >  vReal1, false);
    QCOMPARE(vComplex10 <= vReal1, true);
    QCOMPARE(vComplex10 >= vReal1, true);

    QCOMPARE(vComplex10 == vReal05, false);
    QCOMPARE(vComplex10 != vReal05, true);
    QCOMPARE(vComplex10 <  vReal05, false);
    QCOMPARE(vComplex10 >  vReal05, true);
    QCOMPARE(vComplex10 <= vReal05, false);
    QCOMPARE(vComplex10 >= vReal05, true);

    QCOMPARE(vComplex10 == vReal2, false);
    QCOMPARE(vComplex10 != vReal2, true);
    QCOMPARE(vComplex10 <  vReal2, true);
    QCOMPARE(vComplex10 >  vReal2, false);
    QCOMPARE(vComplex10 <= vReal2, true);
    QCOMPARE(vComplex10 >= vReal2, false);

    QCOMPARE(vComplex10 == vComplex10, true);
    QCOMPARE(vComplex10 != vComplex10, false);
    QCOMPARE(vComplex10 <  vComplex10, false);
    QCOMPARE(vComplex10 >  vComplex10, false);
    QCOMPARE(vComplex10 <= vComplex10, true);
    QCOMPARE(vComplex10 >= vComplex10, true);

    QCOMPARE(vComplex10 == vComplex005, false);
    QCOMPARE(vComplex10 != vComplex005, true);

    QCOMPARE(vComplex10 == vComplex33, false);
    QCOMPARE(vComplex10 != vComplex33, true);

    QCOMPARE(vComplex33 == vComplex33, true);
    QCOMPARE(vComplex33 != vComplex33, false);
}


void TestVariant::testConvertFunctions() {
    Model::Set set;
    set.insert(Model::Integer(1));
    set.insert(Model::Integer(2));

    Model::Tuple tuple;
    tuple.append(Model::Integer(1));
    tuple.append(Model::Integer(2));

    Model::MatrixBoolean matrixBoolean = Model::MatrixBoolean::build(2, 1, true, false);
    Model::MatrixInteger matrixInteger = Model::MatrixInteger::build(2, 1, Model::Integer(1), Model::Integer(2));
    Model::MatrixReal    matrixReal    = Model::MatrixReal::build(2, 1, Model::Real(1), Model::Real(2));
    Model::MatrixComplex matrixComplex = Model::MatrixComplex::build(2, 1, Model::Complex(1,1), Model::Complex(2,1));

    Model::Variant vInteger(Model::Integer(1));
    Model::Variant vBoolean0(Model::Boolean(false));
    Model::Variant vBoolean1(Model::Boolean(true));
    Model::Variant vReal(Model::Real(2));
    Model::Variant vComplex(Model::Complex(3,3));
    Model::Variant vSet(set);
    Model::Variant vTuple(tuple);
    Model::Variant vMatrixBoolean(matrixBoolean);
    Model::Variant vMatrixInteger(matrixInteger);
    Model::Variant vMatrixReal(matrixReal);
    Model::Variant vMatrixComplex(matrixComplex);

    Model::Boolean       b0 = vBoolean0;
    Model::Boolean       b1 = vBoolean1;
    Model::Integer       i  = vInteger;
    Model::Real          r  = vReal;
    Model::Complex       c  = vComplex;
    Model::Set           s  = vSet;
    Model::Tuple         t  = vTuple;
    Model::MatrixBoolean mb = vMatrixBoolean;
    Model::MatrixInteger mi = vMatrixInteger;
    Model::MatrixReal    mr = vMatrixReal;
    Model::MatrixComplex mc = vMatrixComplex;

    QCOMPARE(b0, Model::Boolean(false));
    QCOMPARE(b1, Model::Boolean(true));
    QCOMPARE(i,  Model::Integer(1));
    QCOMPARE(r,  Model::Real(2.0));
    QCOMPARE(c,  Model::Complex(3, 3));
    QCOMPARE(s,  set);
    QCOMPARE(t,  tuple);
    QCOMPARE(mb, matrixBoolean);
    QCOMPARE(mi, matrixInteger);
    QCOMPARE(mr, matrixReal);
    QCOMPARE(mc, matrixComplex);
}
