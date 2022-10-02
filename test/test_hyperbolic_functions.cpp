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
* This file implements tests of hyperbolic math functions.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QObject>
#include <QList>
#include <QtTest/QtTest>

#include <limits>
#include <complex>
#include <cmath>
#include <random>
#include <cstdint> // Debug

#include <m_intrinsic_types.h>
#include <m_basic_functions.h>
#include <m_hyperbolic_functions.h>

#include "test_hyperbolic_functions.h"

const unsigned TestHyperbolicFunctions::numberTestIterations = 10000U;

typedef std::uint8_t ComplexArray[16];
typedef std::uint8_t RealArray[8];

TestHyperbolicFunctions::TestHyperbolicFunctions() {}


TestHyperbolicFunctions::~TestHyperbolicFunctions() {}


void TestHyperbolicFunctions::testHyperbolicSineFunctions() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> valueGenerator(-10.0, 10.0);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real                     realAngle(valueGenerator(rng));
        M::Complex                  complexAngle(valueGenerator(rng), valueGenerator(rng));
        std::complex<M::Complex::T> stdComplexAngle(complexAngle.real(), complexAngle.imag());

        M::Real    realMeasured    = M::hyperbolicSine(realAngle);
        M::Complex complexMeasured = M::hyperbolicSine(complexAngle);

        M::Real                     realExpected    = std::sinh(realAngle);
        std::complex<M::Complex::T> complexExpected = std::sinh(stdComplexAngle);

        M::Real realError    = M::abs(realMeasured - realExpected);
        M::Real complexError = M::abs(complexMeasured - M::Complex(complexExpected.real(), complexExpected.imag()));

        QCOMPARE(realError < 1.0E-15, true);
        QCOMPARE(complexError < 1.0E-15, true);
    }
}


void TestHyperbolicFunctions::testHyperbolicCosineFunctions() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> valueGenerator(-10.0, 10.0);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real                     realAngle(valueGenerator(rng));
        M::Complex                  complexAngle(valueGenerator(rng), valueGenerator(rng));
        std::complex<M::Complex::T> stdComplexAngle(complexAngle.real(), complexAngle.imag());

        M::Real    realMeasured    = M::hyperbolicCosine(realAngle);
        M::Complex complexMeasured = M::hyperbolicCosine(complexAngle);

        M::Real                     realExpected    = std::cosh(realAngle);
        std::complex<M::Complex::T> complexExpected = std::cosh(stdComplexAngle);

        M::Real realError    = M::abs(realMeasured - realExpected);
        M::Real complexError = M::abs(complexMeasured - M::Complex(complexExpected.real(), complexExpected.imag()));

        QCOMPARE(realError < 1.0E-15, true);
        QCOMPARE(complexError < 1.0E-15, true);
    }
}


void TestHyperbolicFunctions::testHyperbolicTangentFunctions() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> valueGenerator(-10.0, 10.0);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real                     realAngle(valueGenerator(rng));
        M::Complex                  complexAngle(valueGenerator(rng), valueGenerator(rng));
        std::complex<M::Complex::T> stdComplexAngle(complexAngle.real(), complexAngle.imag());

        M::Real    realMeasured    = M::hyperbolicTangent(realAngle);
        M::Complex complexMeasured = M::hyperbolicTangent(complexAngle);

        M::Real                     realExpected    = std::tanh(realAngle);
        std::complex<M::Complex::T> complexExpected = std::tanh(stdComplexAngle);

        M::Real realError    = M::abs(realMeasured - realExpected);
        M::Real complexError = M::abs(complexMeasured - M::Complex(complexExpected.real(), complexExpected.imag()));

        QCOMPARE(realError < 1.0E-15, true);
        QCOMPARE(complexError < 1.0E-15, true);
    }
}


void TestHyperbolicFunctions::testHyperbolicSecantFunctions() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> valueGenerator(-10.0, 10.0);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real                     realAngle(valueGenerator(rng));
        M::Complex                  complexAngle(valueGenerator(rng), valueGenerator(rng));
        std::complex<M::Complex::T> stdComplexAngle(complexAngle.real(), complexAngle.imag());

        M::Real    realMeasured    = M::hyperbolicSecant(realAngle);
        M::Complex complexMeasured = M::hyperbolicSecant(complexAngle);

        M::Real                     realExpected    = 1.0/std::cosh(realAngle);
        std::complex<M::Complex::T> complexExpected = 1.0/std::cosh(stdComplexAngle);

        M::Real realError    = M::abs(realMeasured - realExpected);
        M::Real complexError = M::abs(complexMeasured - M::Complex(complexExpected.real(), complexExpected.imag()));

        QCOMPARE(realError < 1.0E-14, true);
        QCOMPARE(complexError < 1.0E-14, true);
    }
}


void TestHyperbolicFunctions::testHyperbolicCosecantFunctions() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> valueGenerator(-10.0, 10.0);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real                     realAngle(valueGenerator(rng));
        M::Complex                  complexAngle(valueGenerator(rng), valueGenerator(rng));
        std::complex<M::Complex::T> stdComplexAngle(complexAngle.real(), complexAngle.imag());

        M::Real    realMeasured    = M::hyperbolicCosecant(realAngle);
        M::Complex complexMeasured = M::hyperbolicCosecant(complexAngle);

        M::Real                     realExpected    = 1.0/std::sinh(realAngle);
        std::complex<M::Complex::T> complexExpected = 1.0/std::sinh(stdComplexAngle);

        M::Real realError    = M::abs(realMeasured - realExpected);
        M::Real complexError = M::abs(complexMeasured - M::Complex(complexExpected.real(), complexExpected.imag()));

        QCOMPARE(realError < 5.0E-14, true);
        QCOMPARE(complexError < 5.0E-14, true);
    }
}

void TestHyperbolicFunctions::testHyperbolicCotangentFunctions() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> valueGenerator(-10.0, 10.0);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real                     realAngle(valueGenerator(rng));
        M::Complex                  complexAngle(valueGenerator(rng), valueGenerator(rng));
        std::complex<M::Complex::T> stdComplexAngle(complexAngle.real(), complexAngle.imag());

        M::Real    realMeasured    = M::hyperbolicCotangent(realAngle);
        M::Complex complexMeasured = M::hyperbolicCotangent(complexAngle);

        M::Real                     realExpected    = 1.0/std::tanh(realAngle);
        std::complex<M::Complex::T> complexExpected = 1.0/std::tanh(stdComplexAngle);

        M::Real realError    = M::abs(realMeasured - realExpected);
        M::Real complexError = M::abs(complexMeasured - M::Complex(complexExpected.real(), complexExpected.imag()));

        QCOMPARE(realError < 1.0E-14, true);
        QCOMPARE(complexError < 1.0E-14, true);
   }
}


void TestHyperbolicFunctions::testHyperbolicArsineFunctions() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> angleGenerator(-1.0, +1.0);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real                     realAngle(angleGenerator(rng));
        M::Complex                  complexAngle(angleGenerator(rng), angleGenerator(rng));
        std::complex<M::Complex::T> stdComplexAngle(complexAngle.real(), complexAngle.imag());

        M::Real    realSine       = M::hyperbolicSine(realAngle);
        M::Complex complexSine    = M::hyperbolicSine(complexAngle);
        M::Real    realArcsine    = M::hyperbolicArsine(realSine);
        M::Complex complexArcsine = M::hyperbolicArsine(complexSine);

        M::Real realError    = M::abs(realArcsine - realAngle);
        M::Real complexError = M::abs(complexArcsine - complexAngle);

        QCOMPARE(realError < 5.0E-13, true);
        QCOMPARE(complexError < 5.0E-13, true);
    }
}


void TestHyperbolicFunctions::testHyperbolicArcosineFunctions() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> angleGenerator(0.0, +1.0);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real                     realAngle(angleGenerator(rng));
        M::Complex                  complexAngle(angleGenerator(rng), angleGenerator(rng));
        std::complex<M::Complex::T> stdComplexAngle(complexAngle.real(), complexAngle.imag());

        M::Real    realCosine       = M::hyperbolicCosine(realAngle);
        M::Complex complexCosine    = M::hyperbolicCosine(complexAngle);
        M::Real    realArccosine    = M::hyperbolicArcosine(realCosine);
        M::Complex complexArccosine = M::hyperbolicArcosine(complexCosine);

        M::Real realError    = M::abs(realArccosine - realAngle);
        M::Real complexError = M::abs(complexArccosine - complexAngle);

        QCOMPARE(realError < 5.0E-13, true);
        QCOMPARE(complexError < 5.0E-13, true);
    }
}


void TestHyperbolicFunctions::testHyperbolicArtangentFunctions() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> angleGenerator(0.0, 1.0);


    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real                     realAngle(angleGenerator(rng));
        M::Complex                  complexAngle(angleGenerator(rng), angleGenerator(rng));
        std::complex<M::Complex::T> stdComplexAngle(complexAngle.real(), complexAngle.imag());

        M::Real    realTangent       = M::hyperbolicTangent(realAngle);
        M::Complex complexTangent    = M::hyperbolicTangent(complexAngle);
        M::Real    realArctangent    = M::hyperbolicArtangent(realTangent);
        M::Complex complexArctangent = M::hyperbolicArtangent(complexTangent);

        M::Real realError    = M::abs(realArctangent - realAngle);
        M::Real complexError = M::abs(complexArctangent - complexAngle);

        QCOMPARE(realError < 1.0E-14, true);
        QCOMPARE(complexError < 1.0E-14, true);
    }
}


void TestHyperbolicFunctions::testHyperbolicArsecantFunctions() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> angleGenerator(0.0, +1.0);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real                     realAngle(angleGenerator(rng));
        M::Complex                  complexAngle(angleGenerator(rng), angleGenerator(rng));
        std::complex<M::Complex::T> stdComplexAngle(complexAngle.real(), complexAngle.imag());

        M::Real    realSecant       = M::hyperbolicSecant(realAngle);
        M::Complex complexSecant    = M::hyperbolicSecant(complexAngle);
        M::Real    realArcsecant    = M::hyperbolicArsecant(realSecant);
        M::Complex complexArcsecant = M::hyperbolicArsecant(complexSecant);

        M::Real realError    = M::abs(realArcsecant - realAngle);
        M::Real complexError = M::abs(complexArcsecant - complexAngle);

        QCOMPARE(realError < 5.0E-13, true);
        QCOMPARE(complexError < 5.0E-13, true);
    }
}


void TestHyperbolicFunctions::testHyperbolicArcosecantFunctions() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> angleGenerator(-1.0, +1.0);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real                     realAngle(angleGenerator(rng));
        M::Complex                  complexAngle(angleGenerator(rng), angleGenerator(rng));
        std::complex<M::Complex::T> stdComplexAngle(complexAngle.real(), complexAngle.imag());

        M::Real    realCosecant       = M::hyperbolicCosecant(realAngle);
        M::Complex complexCosecant    = M::hyperbolicCosecant(complexAngle);
        M::Real    realArccosecant    = M::hyperbolicArcosecant(realCosecant);
        M::Complex complexArccosecant = M::hyperbolicArcosecant(complexCosecant);

        M::Real realError    = M::abs(realArccosecant - realAngle);
        M::Real complexError = M::abs(complexArccosecant - complexAngle);

        QCOMPARE(realError < 5.0E-13, true);
        QCOMPARE(complexError < 5.0E-13, true);
    }
}


void TestHyperbolicFunctions::testHyperbolicArcotangentFunctions() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> angleGenerator(-1.0, +1.0);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real                     realAngle(angleGenerator(rng));
        M::Complex                  complexAngle(angleGenerator(rng), angleGenerator(rng));
        std::complex<M::Complex::T> stdComplexAngle(complexAngle.real(), complexAngle.imag());

        M::Real    realCotangent       = M::hyperbolicCotangent(realAngle);
        M::Complex complexCotangent    = M::hyperbolicCotangent(complexAngle);
        M::Real    realArccotangent    = M::hyperbolicArcotangent(realCotangent);
        M::Complex complexArccotangent = M::hyperbolicArcotangent(complexCotangent);

        M::Real realError    = M::abs(realArccotangent - realAngle);
        M::Real complexError = M::abs(complexArccotangent - complexAngle);

        QCOMPARE(realError < 5.0E-13, true);
        QCOMPARE(complexError < 5.0E-13, true);
    }
}
