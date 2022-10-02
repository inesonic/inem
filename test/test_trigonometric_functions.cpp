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
* This file implements tests of trigonometric math functions.
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
#include <m_trigonometric_functions.h>

#include "test_trigonometric_functions.h"
const unsigned TestTrigonometricFunctions::numberTestIterations = 10000U;

const M::Real TestTrigonometricFunctions::pi = 4.0L * std::atan(1.0L);

typedef std::uint8_t ComplexArray[16];
typedef std::uint8_t RealArray[8];

TestTrigonometricFunctions::TestTrigonometricFunctions() {}


TestTrigonometricFunctions::~TestTrigonometricFunctions() {}


void TestTrigonometricFunctions::testSineFunctions() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> valueGenerator(-100.0, 100.0);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real                     realAngle(valueGenerator(rng));
        M::Complex                  complexAngle(valueGenerator(rng), valueGenerator(rng));
        std::complex<M::Complex::T> stdComplexAngle(complexAngle.real(), complexAngle.imag());

        M::Real    realMeasured    = M::sine(realAngle);
        M::Complex complexMeasured = M::sine(complexAngle);

        M::Real                     realExpected    = std::sin(realAngle);
        std::complex<M::Complex::T> complexExpected = std::sin(stdComplexAngle);

        M::Real realError    = M::abs(realMeasured - realExpected);
        M::Real complexError = M::abs(complexMeasured - M::Complex(complexExpected.real(), complexExpected.imag()));

        QCOMPARE(realError < 1.0E-15, true);
        QCOMPARE(complexError < 1.0E-15, true);
    }
}


void TestTrigonometricFunctions::testCosineFunctions() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> valueGenerator(-100.0, 100.0);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real                     realAngle(valueGenerator(rng));
        M::Complex                  complexAngle(valueGenerator(rng), valueGenerator(rng));
        std::complex<M::Complex::T> stdComplexAngle(complexAngle.real(), complexAngle.imag());

        M::Real    realMeasured    = M::cosine(realAngle);
        M::Complex complexMeasured = M::cosine(complexAngle);

        M::Real                     realExpected    = std::cos(realAngle);
        std::complex<M::Complex::T> complexExpected = std::cos(stdComplexAngle);

        M::Real realError    = M::abs(realMeasured - realExpected);
        M::Real complexError = M::abs(complexMeasured - M::Complex(complexExpected.real(), complexExpected.imag()));

        QCOMPARE(realError < 1.0E-15, true);
        QCOMPARE(complexError < 1.0E-15, true);
    }
}


void TestTrigonometricFunctions::testTangentFunctions() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> valueGenerator(-100.0, 100.0);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real                     realAngle(valueGenerator(rng));
        M::Complex                  complexAngle(valueGenerator(rng), valueGenerator(rng));
        std::complex<M::Complex::T> stdComplexAngle(complexAngle.real(), complexAngle.imag());

        M::Real    realMeasured    = M::tangent(realAngle);
        M::Complex complexMeasured = M::tangent(complexAngle);

        M::Real                     realExpected    = std::tan(realAngle);
        std::complex<M::Complex::T> complexExpected = std::tan(stdComplexAngle);

        M::Real realError    = M::abs(realMeasured - realExpected);
        M::Real complexError = M::abs(complexMeasured - M::Complex(complexExpected.real(), complexExpected.imag()));

        QCOMPARE(realError < 1.0E-15, true);
        QCOMPARE(complexError < 1.0E-15, true);
    }
}


void TestTrigonometricFunctions::testSecantFunctions() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> valueGenerator(-100.0, 100.0);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real                     realAngle(valueGenerator(rng));
        M::Complex                  complexAngle(valueGenerator(rng), valueGenerator(rng));
        std::complex<M::Complex::T> stdComplexAngle(complexAngle.real(), complexAngle.imag());

        M::Real    realMeasured    = M::secant(realAngle);
        M::Complex complexMeasured = M::secant(complexAngle);

        M::Real                     realExpected    = 1.0/std::cos(realAngle);
        std::complex<M::Complex::T> complexExpected = 1.0/std::cos(stdComplexAngle);

        M::Real realError    = M::abs(realMeasured - realExpected);
        M::Real complexError = M::abs(complexMeasured - M::Complex(complexExpected.real(), complexExpected.imag()));

        QCOMPARE(realError < 1.0E-15, true);
        QCOMPARE(complexError < 1.0E-15, true);
    }
}


void TestTrigonometricFunctions::testCosecantFunctions() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> valueGenerator(-100.0, 100.0);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real                     realAngle(valueGenerator(rng));
        M::Complex                  complexAngle(valueGenerator(rng), valueGenerator(rng));
        std::complex<M::Complex::T> stdComplexAngle(complexAngle.real(), complexAngle.imag());

        M::Real    realMeasured    = M::cosecant(realAngle);
        M::Complex complexMeasured = M::cosecant(complexAngle);

        M::Real                     realExpected    = 1.0/std::sin(realAngle);
        std::complex<M::Complex::T> complexExpected = 1.0/std::sin(stdComplexAngle);

        M::Real realError    = M::abs(realMeasured - realExpected);
        M::Real complexError = M::abs(complexMeasured - M::Complex(complexExpected.real(), complexExpected.imag()));

        QCOMPARE(realError < 1.0E-15, true);
        QCOMPARE(complexError < 1.0E-15, true);
    }
}

void TestTrigonometricFunctions::testCotangentFunctions() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> valueGenerator(-100.0, 100.0);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real                     realAngle(valueGenerator(rng));
        M::Complex                  complexAngle(valueGenerator(rng), valueGenerator(rng));
        std::complex<M::Complex::T> stdComplexAngle(complexAngle.real(), complexAngle.imag());

        M::Real    realMeasured    = M::cotangent(realAngle);
        M::Complex complexMeasured = M::cotangent(complexAngle);

        M::Real                     realExpected    = 1.0/std::tan(realAngle);
        std::complex<M::Complex::T> complexExpected = 1.0/std::tan(stdComplexAngle);

        M::Real realError    = M::abs(realMeasured - realExpected);
        M::Real complexError = M::abs(complexMeasured - M::Complex(complexExpected.real(), complexExpected.imag()));

        QCOMPARE(realError < 1.0E-15, true);
        QCOMPARE(complexError < 1.0E-15, true);
    }
}


void TestTrigonometricFunctions::testArcsineFunctions() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> angleGenerator(-pi / 2.0, +pi / 2.0);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real                     realAngle(angleGenerator(rng));
        M::Complex                  complexAngle(angleGenerator(rng), angleGenerator(rng));
        std::complex<M::Complex::T> stdComplexAngle(complexAngle.real(), complexAngle.imag());

        M::Real    realSine       = M::sine(realAngle);
        M::Complex complexSine    = M::sine(complexAngle);
        M::Real    realArcsine    = M::arcsine(realSine);
        M::Complex complexArcsine = M::arcsine(complexSine);

        M::Real realError    = M::abs(realArcsine - realAngle);
        M::Real complexError = M::abs(complexArcsine - complexAngle);

        QCOMPARE(realError < 5.0E-13, true);
        QCOMPARE(complexError < 5.0E-13, true);
    }
}


void TestTrigonometricFunctions::testArccosineFunctions() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> angleGenerator(0.0, +pi / 2.0);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real                     realAngle(angleGenerator(rng));
        M::Complex                  complexAngle(angleGenerator(rng), angleGenerator(rng));
        std::complex<M::Complex::T> stdComplexAngle(complexAngle.real(), complexAngle.imag());

        M::Real    realCosine       = M::cosine(realAngle);
        M::Complex complexCosine    = M::cosine(complexAngle);
        M::Real    realArccosine    = M::arccosine(realCosine);
        M::Complex complexArccosine = M::arccosine(complexCosine);

        M::Real realError    = M::abs(realArccosine - realAngle);
        M::Real complexError = M::abs(complexArccosine - complexAngle);

        QCOMPARE(realError < 5.0E-13, true);
        QCOMPARE(complexError < 5.0E-13, true);
    }
}


void TestTrigonometricFunctions::testArctangentFunctions() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> angleGenerator(0.0, +pi / 2.0);


    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real                     realAngle(angleGenerator(rng));
        M::Complex                  complexAngle(angleGenerator(rng), angleGenerator(rng));
        std::complex<M::Complex::T> stdComplexAngle(complexAngle.real(), complexAngle.imag());

        M::Real    realTangent       = M::tangent(realAngle);
        M::Complex complexTangent    = M::tangent(complexAngle);
        M::Real    realArctangent    = M::arctangent(realTangent);
        M::Complex complexArctangent = M::arctangent(complexTangent);

        M::Real realError    = M::abs(realArctangent - realAngle);
        M::Real complexError = M::abs(complexArctangent - complexAngle);

        QCOMPARE(realError < 1.0E-14, true);
        QCOMPARE(complexError < 1.0E-14, true);
    }
}


void TestTrigonometricFunctions::testArcsecantFunctions() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> angleGenerator(0.0, +pi / 2.0);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real                     realAngle(angleGenerator(rng));
        M::Complex                  complexAngle(angleGenerator(rng), angleGenerator(rng));
        std::complex<M::Complex::T> stdComplexAngle(complexAngle.real(), complexAngle.imag());

        M::Real    realSecant       = M::secant(realAngle);
        M::Complex complexSecant    = M::secant(complexAngle);
        M::Real    realArcsecant    = M::arcsecant(realSecant);
        M::Complex complexArcsecant = M::arcsecant(complexSecant);

        M::Real realError    = M::abs(realArcsecant - realAngle);
        M::Real complexError = M::abs(complexArcsecant - complexAngle);

        QCOMPARE(realError < 5.0E-13, true);
        QCOMPARE(complexError < 5.0E-13, true);
    }
}


void TestTrigonometricFunctions::testArccosecantFunctions() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> angleGenerator(0.0, +pi / 2.0);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real                     realAngle(angleGenerator(rng));
        M::Complex                  complexAngle(angleGenerator(rng), angleGenerator(rng));
        std::complex<M::Complex::T> stdComplexAngle(complexAngle.real(), complexAngle.imag());

        M::Real    realCosecant       = M::cosecant(realAngle);
        M::Complex complexCosecant    = M::cosecant(complexAngle);
        M::Real    realArccosecant    = M::arccosecant(realCosecant);
        M::Complex complexArccosecant = M::arccosecant(complexCosecant);

        M::Real realError    = M::abs(realArccosecant - realAngle);
        M::Real complexError = M::abs(complexArccosecant - complexAngle);

        QCOMPARE(realError < 5.0E-13, true);
        QCOMPARE(complexError < 5.0E-13, true);
    }
}


void TestTrigonometricFunctions::testArccotangentFunctions() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> angleGenerator(0.0, +pi / 2.0);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real                     realAngle(angleGenerator(rng));
        M::Complex                  complexAngle(angleGenerator(rng), angleGenerator(rng));
        std::complex<M::Complex::T> stdComplexAngle(complexAngle.real(), complexAngle.imag());

        M::Real    realCotangent       = M::cotangent(realAngle);
        M::Complex complexCotangent    = M::cotangent(complexAngle);
        M::Real    realArccotangent    = M::arccotangent(realCotangent);
        M::Complex complexArccotangent = M::arccotangent(complexCotangent);

        M::Real realError    = M::abs(realArccotangent - realAngle);
        M::Real complexError = M::abs(complexArccotangent - complexAngle);

        QCOMPARE(realError < 5.0E-13, true);
        QCOMPARE(complexError < 5.0E-13, true);
    }
}

