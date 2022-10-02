/*-*-c++-*-*************************************************************************************************************
* Copyright 2022 Inesonic, LLC.
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
* This file implements tests of the \ref M::summation function and associated infrastructure.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QObject>
#include <QtTest/QtTest>

#include <complex>

#include <m_intrinsic_types.h>
#include <m_matrix_boolean.h>
#include <m_matrix_integer.h>
#include <m_matrix_real.h>
#include <m_matrix_complex.h>
#include <m_summation_operator.h>

#include "test_summation_operator.h"

TestSummationOperator::TestSummationOperator() {}


TestSummationOperator::~TestSummationOperator() {}


void TestSummationOperator::testOneValueFunctions() {
    M::Boolean       booleanOne       = M::Summation::oneValue(M::Boolean(0));
    M::Integer       integerOne       = M::Summation::oneValue(M::Integer(0));
    M::Real          realOne          = M::Summation::oneValue(M::Real(0));
    M::Complex       complexOne       = M::Summation::oneValue(M::Complex(0));

    M::MatrixBoolean booleanMatrixOne = M::Summation::oneValue(
        M::MatrixBoolean::build(
            2, 3,
            false, false,
            true, false,
            false, true
        )
    );
    M::MatrixInteger integerMatrixOne = M::Summation::oneValue(
        M::MatrixInteger::build(
            2, 3,
            1, 2,
            3, 4,
            5, 6
        )
    );
    M::MatrixReal realMatrixOne = M::Summation::oneValue(
        M::MatrixReal::build(
            2, 3,
            1, 2,
            3, 4,
            5, 6
        )
    );
    M::MatrixComplex complexMatrixOne = M::Summation::oneValue(
        M::MatrixComplex::build(
            2, 3,
            complexOne, complexOne,
            complexOne, complexOne,
            complexOne, complexOne
        )
    );

    QCOMPARE(booleanOne, true);
    QCOMPARE(integerOne, 1);
    QCOMPARE(realOne, 1.0);
    QCOMPARE(complexOne, M::Complex(1.0, 0.0));

    QCOMPARE(
        booleanMatrixOne,
        M::MatrixBoolean::build(
            2, 3,
            true, true,
            true, true,
            true, true
        )
    );
    QCOMPARE(
        integerMatrixOne,
        M::MatrixInteger::build(
            2, 3,
            1, 1,
            1, 1,
            1, 1
        )
    );
    QCOMPARE(
        realMatrixOne,
        M::MatrixReal::build(
            2, 3,
            1.0, 1.0,
            1.0, 1.0,
            1.0, 1.0
        )
    );
    QCOMPARE(
        complexMatrixOne,
        M::MatrixComplex::build(
            2, 3,
            complexOne, complexOne,
            complexOne, complexOne,
            complexOne, complexOne
        )
    );
}


void TestSummationOperator::testZeroValueFunctions() {
    M::Boolean       booleanZero       = M::Summation::zeroValue(M::Boolean(0));
    M::Integer       integerZero       = M::Summation::zeroValue(M::Integer(0));
    M::Real          realZero          = M::Summation::zeroValue(M::Real(0));
    M::Complex       complexZero       = M::Summation::zeroValue(M::Complex(0));

    M::MatrixBoolean booleanMatrixZero = M::Summation::zeroValue(
        M::MatrixBoolean::build(
            2, 3,
            false, false,
            true, false,
            false, true
        )
    );
    M::MatrixInteger integerMatrixZero = M::Summation::zeroValue(
        M::MatrixInteger::build(
            2, 3,
            1, 2,
            3, 4,
            5, 6
        )
    );
    M::MatrixReal realMatrixZero = M::Summation::zeroValue(
        M::MatrixReal::build(
            2, 3,
            1, 2,
            3, 4,
            5, 6
        )
    );
    M::MatrixComplex complexMatrixZero = M::Summation::zeroValue(
        M::MatrixComplex::build(
            2, 3,
            complexZero, complexZero,
            complexZero, complexZero,
            complexZero, complexZero
        )
    );

    QCOMPARE(booleanZero, false);
    QCOMPARE(integerZero, 0);
    QCOMPARE(realZero, 0.0);
    QCOMPARE(complexZero, M::Complex(0.0, 0.0));

    QCOMPARE(
        booleanMatrixZero,
        M::MatrixBoolean::build(
            2, 3,
            false, false,
            false, false,
            false, false
        )
    );
    QCOMPARE(
        integerMatrixZero,
        M::MatrixInteger::build(
            2, 3,
            0, 0,
            0, 0,
            0, 0
        )
    );
    QCOMPARE(
        realMatrixZero,
        M::MatrixReal::build(
            2, 3,
            0.0, 0.0,
            0.0, 0.0,
            0.0, 0.0
        )
    );
    QCOMPARE(
        complexMatrixZero,
        M::MatrixComplex::build(
            2, 3,
            complexZero, complexZero,
            complexZero, complexZero,
            complexZero, complexZero
        )
    );
}


void TestSummationOperator::testErrorValueFunctions() {
    M::Real booleanError = M::Summation::errorValue(M::Boolean(3), M::Boolean(2));
    M::Real integerError = M::Summation::errorValue(M::Integer(3), M::Integer(2));
    M::Real realError    = M::Summation::errorValue(M::Real(3), M::Real(2));
    M::Real complexError = M::Summation::errorValue(M::Complex(3, 3), M::Complex(2, 3));

    M::Real booleanMatrixError = M::Summation::errorValue(
        M::MatrixBoolean::build(
            3, 3,
            false, true,  true,
            false, false, false,
            true,  true,  true
        ),
        M::MatrixBoolean::build(
            3, 3,
            true,  true, true,
            true,  true, true,
            true,  true, true
        )
    );
    M::Real integerMatrixError = M::Summation::errorValue(
        M::MatrixInteger::build(
            3, 3,
            1, 2, 3,
            4, 5, 6,
            7, 8, 9
        ),
        M::MatrixInteger::build(
            3, 3,
             2,  4,  6,
             8, 10, 12,
            14, 16, 18
        )
    );
    M::Real realMatrixError = M::Summation::errorValue(
        M::MatrixReal::build(
            3, 3,
            1.0, 2.0, 3.0,
            4.0, 5.0, 6.0,
            7.0, 8.0, 9.0
        ),
        M::MatrixReal::build(
            3, 3,
             2.0,  4.0,  6.0,
             8.0, 10.0, 12.0,
            14.0, 16.0, 18.0
        )
    );
    M::Real complexMatrixError = M::Summation::errorValue(
        M::MatrixReal::build(
            3, 3,
            M::Complex(1.0), M::Complex( 2.0), M::Complex( 3.0),
            M::Complex(4.0), M::Complex( 5.0), M::Complex( 6.0),
            M::Complex(7.0), M::Complex( 8.0), M::Complex( 9.0)
        ),
        M::MatrixReal::build(
            3, 3,
            M::Complex( 2.0), M::Complex(  4.0), M::Complex(  6.0),
            M::Complex( 8.0), M::Complex( 10.0), M::Complex( 12.0),
            M::Complex(14.0), M::Complex( 16.0), M::Complex( 18.0)
        )
    );

    M::Real complexExpected = (
          std::abs(std::complex<double>(3, 3) - std::complex<double>(2, 3))
        / std::abs(std::complex<double>(2, 3))
    );
    M::Real matrixExpected = std::abs(std::sqrt(285.0) - std::sqrt(1140.0)) / std::abs(std::sqrt(1140.0));

    QCOMPARE(booleanError, 0.0);
    QCOMPARE(integerError, 0.5);
    QCOMPARE(realError, 0.5);
    QCOMPARE((std::abs(complexError - complexExpected) / complexExpected) < 1.0E-15, true);
    QCOMPARE((std::abs(booleanMatrixError - (2.0/3.0)) / (2.0/3.0)) < 1.0E-15, true);
    QCOMPARE((std::abs(integerMatrixError - matrixExpected) / matrixExpected) < 1.0E-15, true);
    QCOMPARE((std::abs(realMatrixError - matrixExpected) / matrixExpected) < 1.0E-15, true);
    QCOMPARE((std::abs(complexMatrixError - matrixExpected) / matrixExpected) < 1.0E-15, true);
}


void TestSummationOperator::testConvergenceFailureValueFunctions() {
    M::Boolean       booleanConvergenceError       = M::Summation::convergenceFailureValue(M::Boolean(0));
    M::Integer       integerConvergenceError       = M::Summation::convergenceFailureValue(M::Integer(0));
    M::Real          realConvergenceError          = M::Summation::convergenceFailureValue(M::Real(0));
    M::Complex       complexConvergenceError       = M::Summation::convergenceFailureValue(M::Complex(0));

    M::MatrixBoolean booleanMatrixConvergenceError = M::Summation::convergenceFailureValue(
        M::MatrixBoolean::build(
            2, 3,
            false, false,
            true, false,
            false, true
        )
    );
    M::MatrixInteger integerMatrixConvergenceError = M::Summation::convergenceFailureValue(
        M::MatrixInteger::build(
            2, 3,
            1, 2,
            3, 4,
            5, 6
        )
    );
    M::MatrixReal realMatrixConvergenceError = M::Summation::convergenceFailureValue(
        M::MatrixReal::build(
            2, 3,
            1, 2,
            3, 4,
            5, 6
        )
    );
    M::MatrixComplex complexMatrixConvergenceError = M::Summation::convergenceFailureValue(
        M::MatrixComplex::build(
            2, 3,
            complexConvergenceError, complexConvergenceError,
            complexConvergenceError, complexConvergenceError,
            complexConvergenceError, complexConvergenceError
        )
    );

    QCOMPARE(booleanConvergenceError, false);
    QCOMPARE(integerConvergenceError, 0);
    QCOMPARE(M::isNaN(realConvergenceError), true);
    QCOMPARE(M::isNaN(complexConvergenceError.real()) && M::isNaN(complexConvergenceError.imag()), true);

    QCOMPARE(booleanMatrixConvergenceError, M::MatrixBoolean(0, 0));
    QCOMPARE(integerMatrixConvergenceError, M::MatrixInteger(0, 0));
    QCOMPARE(realMatrixConvergenceError, M::MatrixReal(0, 0));
    QCOMPARE(complexMatrixConvergenceError, M::MatrixComplex(0, 0));
}


void TestSummationOperator::testFiniteSummation1() {
    class Function:public M::Summation::Function<M::Real, M::Integer> {
        public:
            M::Real operator()(const M::Integer& m) final {
                M::Real mr = static_cast<M::Real>(m);
                return 4.0 * std::pow(-1.0, mr) / (2 * mr + 1);
            }
    } fn;

    M::Real result = M::Summation::finiteSummation<M::Real, M::Integer>(fn, 0, 100000);

    QCOMPARE(std::abs(result - M::pi) / M::pi < 1.0E-3, true);
}


void TestSummationOperator::testFiniteSummation2() {
    class Function:public M::Summation::Function<M::Real, M::Integer> {
        public:
            M::Real operator()(const M::Integer& /* m */) final {
                return 1;
            }
    } fn;

    M::Real result = M::Summation::finiteSummation<M::Real, M::Integer>(fn, 1, 100);
    QCOMPARE(result, 100.0);
}


void TestSummationOperator::testOneSidedInfiniteSummation() {
    class Function:public M::Summation::Function<M::Real, M::Integer> {
        public:
            M::Real operator()(const M::Integer& m) final {
                M::Real mr = static_cast<M::Real>(m);
                return 4.0 * std::pow(-1.0, mr) / (2 * mr + 1);
            }
    } fn;

    M::Real result        = M::Summation::oneSidedInfiniteSummation<M::Real, M::Integer>(fn, 0, +1, 1.0E-15);
    M::Real relativeError = std::abs(result - M::pi) / M::pi;

    QCOMPARE(relativeError < 1.0E-13, true);
}


void TestSummationOperator::testSummationFunction() {
    class Function1:public M::Summation::Function<M::Real, M::Real> {
        public:
            M::Real operator()(const M::Real& m) final {
                M::Real mr = static_cast<M::Real>(m);
                return 4.0 * std::pow(-1.0, mr) / (2 * mr + 1);
            }
    } fn1;

    class Function2:public M::Summation::Function<M::Real, M::Real> {
        public:
            M::Real operator()(const M::Real& m) final {
                return std::pow(0.5, m);
            }
    } fn2;

    class Function3:public M::Summation::Function<M::Real, M::Real> {
        public:
            M::Real operator()(const M::Real& m) final {
                return std::pow(0.5, -m);
            }
    } fn3;

    M::Real result = M::summation(fn1, M::Integer(0), M::infinity);
    M::Real relativeError = std::abs(result - M::pi) / M::pi;

    QCOMPARE(relativeError < 1.0E-13, true);

    result = M::summation(fn1, M::infinity, M::Complex(0));
    relativeError = std::abs(result - M::pi) / M::pi;

    QCOMPARE(relativeError < 1.0E-13, true);

    result = M::summation(fn2, M::Integer(0), M::infinity); // sanity check before moving forward
    relativeError = std::abs(result - 2.0) / 2.0;

    QCOMPARE(relativeError < 1.0E-13, true);

    result = M::summation(fn2, M::Integer(-2), M::infinity);
    relativeError = std::abs(result - 8.0) / 8.0;

    QCOMPARE(relativeError < 1.0E-13, true);

    result = M::summation(fn2, M::infinity, M::Integer(-2));
    relativeError = std::abs(result - 8.0) / 8.0;

    QCOMPARE(relativeError < 1.0E-13, true);

    result = M::summation(fn3, M::Integer(0), -M::infinity); // sanity check before moving forward
    relativeError = std::abs(result - 2.0) / 2.0;

    QCOMPARE(relativeError < 1.0E-13, true);

    result = M::summation(fn3, M::Integer(2), -M::infinity);
    relativeError = std::abs(result - 8.0) / 8.0;

    QCOMPARE(relativeError < 1.0E-13, true);

    result = M::summation(fn3, -M::infinity, M::Integer(2));
    relativeError = std::abs(result - 8.0) / 8.0;

    QCOMPARE(relativeError < 1.0E-13, true);

    result = M::summation(fn3, -M::infinity, M::Integer(-1));
    relativeError = std::abs(result - 1.0) / 1.0;

    QCOMPARE(relativeError < 1.0E-13, true);
}
