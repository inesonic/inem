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
* This file implements tests of basic math functions.
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

#include <boost/math/special_functions/gamma.hpp>
#include <boost/math/special_functions/erf.hpp>

#include <m_intrinsic_types.h>
#include <m_per_thread.h>
#include <m_basic_functions.h>

#include "test_basic_functions.h"

const unsigned TestBasicFunctions::numberTestIterations             = 10000U;
const unsigned TestBasicFunctions::numberComplexGammaTestIterations = 100U;

const M::Real    TestBasicFunctions::epsilon      = std::numeric_limits<M::Real>::epsilon();
const M::Real    TestBasicFunctions::infinity     = std::numeric_limits<M::Real>::infinity();
const M::Real    TestBasicFunctions::NaN          = std::numeric_limits<M::Real>::quiet_NaN();
const M::Real    TestBasicFunctions::negativeZero = M::Real(-1.0)/TestBasicFunctions::infinity;
const M::Real    TestBasicFunctions::positiveZero = M::Real(+1.0)/TestBasicFunctions::infinity;
const M::Integer TestBasicFunctions::minInteger   = std::numeric_limits<M::Integer>::min();
const M::Integer TestBasicFunctions::maxInteger   = std::numeric_limits<M::Integer>::max();

typedef std::uint8_t ComplexArray[16];
typedef std::uint8_t RealArray[8];

static M::Complex testZeta(const M::Complex& s) {
    std::complex<double> sl(s.real(), s.imag());
    std::complex<double> one(1.0);
    std::complex<double> result     = 0;
    std::complex<double> lastResult = 1;
    M::Integer           i          = 1;
    double               relativeError;

    do {
        lastResult = result;
        result += one / std::pow(std::complex<double>(i), sl);
        relativeError = (std::abs(result - lastResult) / std::abs(result));
        ++i;
    } while (relativeError > 1.0E-12);

    return M::Complex(result.real(), result.imag());
}


static M::Real testZeta(M::Real s) {
    // Goal of this function is to provide a simple check.  I note that the results are only accurate to within an
    // approximate relative error of 1.3E-8, at least on MacOS, likely due to error accumulation.

    double     result     = 0;
    double     lastResult = 1;
    M::Integer i          = 1;
    double     relativeError;

    do {
        lastResult = result;
        result += std::exp(-static_cast<double>(s) * std::log(static_cast<double>(i)));
        relativeError = (std::abs(result - lastResult) / std::abs(result));
        ++i;
    } while (relativeError > 1.0E-12);

    return result;
}


#define THRESHOLD_COMPARE(_e, _m, _t)                             \
    {                                                             \
        auto __e = _e;                                            \
        auto __m = _m;                                            \
        auto __t = _t;                                            \
        M::Real _relativeError = M::abs(__e - __m) / M::abs(__e); \
        if (_relativeError > __t) {                               \
            qDebug() << "Expected: " << M::abs(__e)               \
                     << "  Measured: " << M::abs(__m)             \
                     << "  Relative Error" << _relativeError;     \
            QCOMPARE(_relativeError <= __t, true);                \
        }                                                         \
    }


TestBasicFunctions::TestBasicFunctions() {}


TestBasicFunctions::~TestBasicFunctions() {}


void TestBasicFunctions::testStdPowComplexRealValues() {
    std::mt19937                               rng(0x12345678);
    std::exponential_distribution<long double> realGenerator(0.01);
    std::uniform_int_distribution<int>         integerGenerator(-128, +128);

    std::vector<std::complex<long double>> bases;
    std::vector<long double>               exponents;
    std::vector<std::complex<long double>> results;

    // Build 32 random sets of values.
    unsigned i=0;
    do {
        std::complex<long double> base(
            integerGenerator(rng) > 0 ? realGenerator(rng) : -realGenerator(rng),
            integerGenerator(rng) > 0 ? realGenerator(rng) : -realGenerator(rng)
        );
        long double exponent = integerGenerator(rng) > 0 ? realGenerator(rng) : -realGenerator(rng);
        std::complex<long double> result = std::pow(base, exponent);

        if (!std::isnan(result.real()) && !std::isnan(result.imag())) {
            bases.push_back(base);
            exponents.push_back(exponent);
            results.push_back(result);

            ++i;
        }
    } while (i < 32);

    std::uniform_int_distribution<unsigned> indexGenerator(0, 31);

    for (i=0 ; i<10000 ; ++i) {
        unsigned index = indexGenerator(rng);

        std::complex<long double> base       = bases.at(index);
        long double               exponent   = exponents.at(index);
        std::complex<long double> expected   = results.at(index);

        std::complex<long double> calculated = std::pow(base, exponent);

        if (expected != calculated) {
            assert(false);
        }
    }
}


void TestBasicFunctions::testIntegerPowIntgerInteger() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> integerGenerator(-128, +128);

    M::Integer maximumValue = std::numeric_limits<M::Integer>::max();

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer base     = integerGenerator(rng);
        M::Integer exponent = integerGenerator(rng);

        M::Integer expected = std::pow(base, exponent);
        if (expected == std::numeric_limits<M::Integer>::min()) {
            if (base >= 0 || (exponent & 1) == 0) {
                expected = std::numeric_limits<M::Integer>::max();
            }
        }

        M::Integer actual = M::pow(base, exponent);

        QCOMPARE(actual, expected);
    }

    // check special cases
    QCOMPARE(M::pow(M::Integer(0), M::Integer(0)), M::Integer(1));
    QCOMPARE(M::pow(M::Integer(0), M::Integer(-1)), maximumValue);
}


void TestBasicFunctions::testRealPowRealInteger() {
    std::mt19937                              rng(0x12345678);
    std::exponential_distribution<M::Real>    realGenerator(1E-60);
    std::uniform_int_distribution<M::Integer> integerGenerator(-64, +64);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real    base     = integerGenerator(rng) < 0 ? -realGenerator(rng) : +realGenerator(rng);
        M::Integer exponent;

        // We short-circuit squares and cubes so there is a tiny error in those cases relative to the std::pow method.
        // For this reason, we don't test these cases.
        do {
            exponent = integerGenerator(rng);
        } while (exponent == 2 || exponent == 3);

        M::Real expected = std::pow(base, exponent);
        M::Real actual   = std::pow(base, exponent);

        QCOMPARE(actual, expected);

        if (base > 0) {
            QVERIFY(actual >= 0);
            QCOMPARE(std::signbit(actual), false);
        } else if (base < 0) {
            if ((exponent & 1) == 0) {
                QVERIFY(actual >= 0);
                QCOMPARE(std::signbit(actual), false);
            } else {
                QVERIFY(actual <= 0);
                QCOMPARE(std::signbit(actual), true);
            }
        }
    }

    // check special cases
    M::Real actual = M::pow(positiveZero, M::Integer(0));
    QCOMPARE(std::isnan(actual), true);

    actual = M::pow(negativeZero, M::Integer(0));
    QCOMPARE(std::isnan(actual), true);

    actual = M::pow(+infinity, M::Integer(0));
    QCOMPARE(std::isnan(actual), true);

    actual = M::pow(-infinity, M::Integer(0));
    QCOMPARE(std::isnan(actual), true);

    QCOMPARE(M::pow(+infinity, M::Integer(1)), +infinity);
    QCOMPARE(M::pow(-infinity, M::Integer(1)), -infinity);

    QCOMPARE(M::pow(+infinity, M::Integer(2)), +infinity);
    QCOMPARE(M::pow(-infinity, M::Integer(2)), +infinity);

    QCOMPARE(M::pow(+infinity, M::Integer(-1)), M::Real(0));
    QCOMPARE(M::pow(-infinity, M::Integer(-1)), M::Real(0));

    actual = M::pow(NaN, M::Integer(0));
    QCOMPARE(std::isnan(actual), true);

    actual = M::pow(NaN, M::Integer(1));
    QCOMPARE(std::isnan(actual), true);
}


void TestBasicFunctions::testComplexPowIntegerReal() {
    std::mt19937                              rng(0x12345678);
    std::exponential_distribution<M::Real>    realGenerator(0.01);
    std::uniform_int_distribution<M::Integer> integerGenerator(-128, +128);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer base     = integerGenerator(rng);
        M::Real    exponent;

        // We short-circuit square roots, squares and cubes so there is a tiny error in those cases relative to the
        // std::pow method.  For this reason, we don't test these cases
        do {
            exponent = integerGenerator(rng) < 0 ? -realGenerator(rng) : +realGenerator(rng);
        } while (exponent == 0.5 || exponent == 2 || exponent == 3);

        std::complex<M::Complex::T> complexBase(base, 0);

        M::Complex            expected = M::pow(base, exponent);
        std::complex<M::Real> actual   = std::pow(complexBase, exponent);

        // Ignore NaN and infinity cases for now as the standard library is broken on at least some platforms.  We'll
        // test these cases separately.
        if (!std::isnan(actual.real()) && !std::isnan(actual.imag()) &&
            !std::isinf(actual.real()) && !std::isinf(actual.imag())    ) {
            if (expected.real() != actual.real() || expected.imag() != actual.imag()) {
                QCOMPARE(expected.real(), actual.real());
                QCOMPARE(expected.imag(), actual.imag());
            }
        }
    }

    // check special cases
    M::Complex actual = M::pow(M::Integer(0), M::Real(0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Integer(0), +infinity);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Integer(0), -infinity);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    QCOMPARE(M::pow(M::Integer(1), +infinity), M::Complex(1, 0));
    QCOMPARE(M::pow(M::Integer(1), -infinity), M::Complex(1, 0));

    actual = M::pow(M::Integer(-1), +infinity);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Integer(-1), -infinity);

    actual = M::pow(M::Integer(-1), -infinity);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Integer(+1), NaN);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Integer(-1), NaN);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));
}


void TestBasicFunctions::testComplexPowRealReal() {
    std::mt19937                              rng(0x12345678);
    std::exponential_distribution<M::Real>    realGenerator(0.01);
    std::uniform_int_distribution<M::Integer> integerGenerator(-128, +128);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real base     = realGenerator(rng);
        M::Real exponent;

        // We short-circuit squares and cubes so there is a tiny error in those cases relative to the std::pow method.
        // For this reason, we don't test these cases
        do {
            exponent = integerGenerator(rng) < 0 ? -realGenerator(rng) : +realGenerator(rng);
        } while (exponent == 0.5 || exponent == 2 || exponent == 3);

        M::Complex            expected = M::pow(base, exponent);
        std::complex<M::Real> actual   = std::pow(std::complex<M::Real>(base), exponent);

        // Ignore NaN and infinity cases for now as the standard library is broken on at least some platforms.  We'll
        // test these cases separately.
        if (!std::isnan(actual.real()) && !std::isnan(actual.imag()) &&
            !std::isinf(actual.real()) && !std::isinf(actual.imag())    ) {
            QCOMPARE(expected.real(), actual.real());
            QCOMPARE(expected.imag(), actual.imag());
        }
    }

    // check special cases
    M::Complex actual = M::pow(M::Real(0), M::Real(0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Real(0), +infinity);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Real(0), -infinity);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(+infinity, M::Real(0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(-infinity, M::Real(0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    QCOMPARE(M::pow(M::Real(1), +infinity), M::Complex(1, 0));
    QCOMPARE(M::pow(M::Real(1), -infinity), M::Complex(1, 0));

    actual = M::pow(M::Real(-1), +infinity);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Real(-1), -infinity);

    actual = M::pow(M::Real(-1), -infinity);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    QCOMPARE(M::pow(+infinity, +infinity), M::Complex(+infinity, 0));

    actual = M::pow(-infinity, +infinity);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(+infinity, -infinity);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(-infinity, -infinity);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Real(-1), NaN);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(NaN, M::Real(+1));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(NaN, M::Real(-1));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(NaN, NaN);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));
}


void TestBasicFunctions::testComplexPowComplexInteger() {
    std::mt19937                              rng(0x12345678);
    std::exponential_distribution<M::Real>    realGenerator(0.01);
    std::uniform_int_distribution<M::Integer> integerGenerator(-128, +128);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex base(
            integerGenerator(rng) > 0 ? realGenerator(rng) : -realGenerator(rng),
            integerGenerator(rng) > 0 ? realGenerator(rng) : -realGenerator(rng)
        );
        M::Integer exponent;

        // We short-circuit squares and cubes so there is a tiny error in those cases relative to the std::pow method.
        // For this reason, we don't test these cases
        do {
            exponent = integerGenerator(rng);
        } while (exponent == 0.5 || exponent == 2 || exponent == 3);

        std::complex<M::Complex::T> stdBase(base.real(), base.imag());
        std::complex<M::Complex::T> stdExponent(exponent, 0);

        const ComplexArray* sba = reinterpret_cast<const ComplexArray*>(&stdBase);
        const ComplexArray* sea = reinterpret_cast<const ComplexArray*>(&stdExponent);

        std::complex<M::Complex::T> actual(std::pow(stdBase, stdExponent));

        M::Complex expected = M::pow(base, exponent);

        // Ignore NaN and infinity cases for now as the standard library is broken on at least some platforms.  We'll
        // test these cases separately.
        if (!std::isnan(actual.real()) && !std::isnan(actual.imag()) &&
            !std::isinf(actual.real()) && !std::isinf(actual.imag())    ) {
            // TODO: We appear to get a tiny random error between runs, even when the same underlying M::pow method is
            //       being used (note we use the complex version of std::pow above in both the test and our
            //       implementation.  For now we ignore this error even though it is somewhat concerning.

            M::Real error         = M::abs(expected - M::Complex(actual.real(), actual.imag()));
            M::Real relativeError = error / std::max(std::abs(expected.real()), std::abs(expected.imag()));

            if (relativeError >= 2 * epsilon) {
                qDebug() << relativeError << " " << epsilon;
                QCOMPARE(relativeError < 2 * epsilon, true);
            }
        }

        (void) sba;
        (void) sea;
    }

    // check special cases
    M::Complex actual = M::pow(M::Complex(0, 0), M::Integer(0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Complex(+infinity, 0), M::Integer(0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Complex(-infinity, 0), M::Integer(0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Complex(0, +infinity), M::Integer(0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(0, -infinity), M::Integer(0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, +infinity), M::Integer(0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, -infinity), M::Integer(0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, +infinity), M::Integer(0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, -infinity), M::Integer(0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    QCOMPARE(M::pow(M::Complex(+infinity, 0), M::Integer(1)), M::Complex(+infinity, 0));
    QCOMPARE(M::pow(M::Complex(-infinity, 0), M::Integer(1)), M::Complex(-infinity, 0));
    QCOMPARE(M::pow(M::Complex(0, +infinity), M::Integer(1)), M::Complex(0, +infinity));
    QCOMPARE(M::pow(M::Complex(0, -infinity), M::Integer(1)), M::Complex(0, -infinity));
    QCOMPARE(M::pow(M::Complex(+infinity, +infinity), M::Integer(1)), M::Complex(+infinity, +infinity));
    QCOMPARE(M::pow(M::Complex(+infinity, -infinity), M::Integer(1)), M::Complex(+infinity, -infinity));
    QCOMPARE(M::pow(M::Complex(-infinity, +infinity), M::Integer(1)), M::Complex(-infinity, +infinity));
    QCOMPARE(M::pow(M::Complex(-infinity, -infinity), M::Integer(1)), M::Complex(-infinity, -infinity));

    QCOMPARE(M::pow(M::Complex(+infinity, 0), M::Integer(-1)), M::Complex(0, 0));
    QCOMPARE(M::pow(M::Complex(-infinity, 0), M::Integer(-1)), M::Complex(0, 0));
    QCOMPARE(M::pow(M::Complex(0, +infinity), M::Integer(-1)), M::Complex(0, 0));
    QCOMPARE(M::pow(M::Complex(0, -infinity), M::Integer(-1)), M::Complex(0, 0));

    // The four scenarios tested below are technically wrong but the complexity required to determine the actual
    // correct result is surprisingly involved.  For now the function just throws up it's hands and reports NaN as
    // that is the most likely result.
    //
    // TODO: Update the tests if/when the M::pow function's behaviors are addressed.

    actual = M::pow(M::Complex(+infinity, +infinity), M::Integer(-1));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, -infinity), M::Integer(-1));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, +infinity), M::Integer(-1));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, -infinity), M::Integer(-1));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(NaN, 0), M::Integer(0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Complex(0, NaN), M::Integer(0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(NaN, NaN), M::Integer(0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);
}


void TestBasicFunctions::testComplexPowIntegerComplex() {
    std::mt19937                              rng(0x12345678);
    std::exponential_distribution<M::Real>    realGenerator(0.01);
    std::uniform_int_distribution<M::Integer> integerGenerator(-128, +128);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer base     = integerGenerator(rng);
        M::Complex exponent(
            integerGenerator(rng) < 0 ? -realGenerator(rng) : +realGenerator(rng),
            integerGenerator(rng) < 0 ? -realGenerator(rng) : +realGenerator(rng)
        );

        M::Complex            expected = M::pow(base, exponent);
        std::complex<M::Real> actual   = std::pow(
            std::complex<M::Real>(base),
            std::complex<M::Real>(exponent.real(), exponent.imag())
        );

        // Ignore NaN and infinity cases for now as the standard library is broken on at least some platforms.  We'll
        // test these cases separately.
        if (!std::isnan(actual.real()) && !std::isnan(actual.imag()) &&
            !std::isinf(actual.real()) && !std::isinf(actual.imag())    ) {
            if (expected.real() != actual.real() || expected.imag() != actual.imag()) {
                QCOMPARE(expected.real(), actual.real());
                QCOMPARE(expected.imag(), actual.imag());
            }
        }
    }

    // check special cases
    M::Complex actual = M::pow(M::Integer(0), M::Complex(0, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Integer(0), M::Complex(+infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Integer(0), M::Complex(-infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Integer(0), M::Complex(0, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Integer(0), M::Complex(0, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Integer(0), M::Complex(+infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Integer(0), M::Complex(+infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Integer(0), M::Complex(-infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Integer(0), M::Complex(-infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);



    QCOMPARE(M::pow(M::Integer(1), M::Complex(+infinity, 0)), M::Complex(1, 0));
    QCOMPARE(M::pow(M::Integer(1), M::Complex(-infinity, 0)), M::Complex(1, 0));

    actual = M::pow(M::Integer(1), M::Complex(0, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Integer(1), M::Complex(0, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Integer(1), M::Complex(+infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Integer(1), M::Complex(+infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Integer(1), M::Complex(-infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Integer(1), M::Complex(-infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);


    actual = M::pow(M::Integer(-1), M::Complex(+infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Integer(-1), M::Complex(-infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Integer(-1), M::Complex(0, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Integer(-1), M::Complex(0, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Integer(-1), M::Complex(+infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Integer(-1), M::Complex(+infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Integer(-1), M::Complex(-infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Integer(-1), M::Complex(-infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);


    actual = M::pow(M::Integer(+1), M::Complex(NaN, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Integer(+1), M::Complex(0, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Integer(+1), M::Complex(NaN, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Integer(0), M::Complex(NaN, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Integer(0), M::Complex(0, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Integer(0), M::Complex(NaN, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Integer(-1), M::Complex(NaN, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Integer(-1), M::Complex(0, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Integer(-1), M::Complex(NaN, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);
}


void TestBasicFunctions::testComplexPowComplexReal() {
    std::mt19937                              rng(0x12345678);
    std::exponential_distribution<M::Real>    realGenerator(0.01);
    std::uniform_int_distribution<M::Integer> integerGenerator(-128, +128);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex base(
            integerGenerator(rng) > 0 ? realGenerator(rng) : -realGenerator(rng),
            integerGenerator(rng) > 0 ? realGenerator(rng) : -realGenerator(rng)
        );
        M::Real exponent = integerGenerator(rng) > 0 ? realGenerator(rng) : -realGenerator(rng);

        M::Complex            expected = M::pow(base, exponent);
        std::complex<M::Real> actual   = std::pow(
            std::complex<M::Real>(base.real(), base.imag()),
            std::complex<M::Real>(exponent, 0)
        );

        // Ignore NaN and infinity cases for now as the standard library is broken on at least some platforms.  We'll
        // test these cases separately.
        if (!std::isnan(actual.real()) && !std::isnan(actual.imag()) &&
            !std::isinf(actual.real()) && !std::isinf(actual.imag())    ) {
            // TODO: We appear to get a tiny random error between runs, even when the same underlying M::pow method is
            //       being used (note we use the complex version of std::pow above in both the test and our
            //       implementation.  For now we ignore this error even though it is somewhat concerning.

            M::Real error         = M::abs(expected - M::Complex(actual.real(), actual.imag()));
            M::Real relativeError = error / std::max(std::abs(expected.real()), std::abs(expected.imag()));

            if (relativeError >= 2 * epsilon) {
                qDebug() << relativeError << " " << epsilon;
                QCOMPARE(relativeError < 2 * epsilon, true);
            }
        }
    }

    // check special cases
    M::Complex actual = M::pow(M::Complex(0, 0), M::Real(0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Complex(+infinity, 0), M::Real(0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Complex(-infinity, 0), M::Real(0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Complex(0, +infinity), M::Real(0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(0, -infinity), M::Real(0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, +infinity), M::Real(0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, -infinity), M::Real(0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, +infinity), M::Real(0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, -infinity), M::Real(0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);


    actual = M::pow(M::Complex(0, 0), +infinity);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Complex(+infinity, 0), +infinity);
    QCOMPARE(actual, M::Complex(+infinity, 0));

    actual = M::pow(M::Complex(-infinity, 0), +infinity);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Complex(0, +infinity), +infinity);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(0, -infinity), +infinity);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, +infinity), +infinity);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, -infinity), +infinity);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, +infinity), +infinity);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, -infinity), +infinity);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);


    actual = M::pow(M::Complex(0, 0), -infinity);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Complex(+infinity, 0), -infinity);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Complex(-infinity, 0), -infinity);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Complex(0, +infinity), -infinity);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(0, -infinity), -infinity);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, +infinity), -infinity);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, -infinity), -infinity);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, +infinity), -infinity);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, -infinity), -infinity);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);


    QCOMPARE(M::pow(M::Complex(+infinity, 0), M::Real(1)), M::Complex(+infinity, 0));
    QCOMPARE(M::pow(M::Complex(-infinity, 0), M::Real(1)), M::Complex(-infinity, 0));
    QCOMPARE(M::pow(M::Complex(0, +infinity), M::Real(1)), M::Complex(0, +infinity));
    QCOMPARE(M::pow(M::Complex(0, -infinity), M::Real(1)), M::Complex(0, -infinity));
    QCOMPARE(M::pow(M::Complex(+infinity, +infinity), M::Real(1)), M::Complex(+infinity, +infinity));
    QCOMPARE(M::pow(M::Complex(+infinity, -infinity), M::Real(1)), M::Complex(+infinity, -infinity));
    QCOMPARE(M::pow(M::Complex(-infinity, +infinity), M::Real(1)), M::Complex(-infinity, +infinity));
    QCOMPARE(M::pow(M::Complex(-infinity, -infinity), M::Real(1)), M::Complex(-infinity, -infinity));

    QCOMPARE(M::pow(M::Complex(+infinity, 0), M::Real(-1)), M::Complex(0, 0));
    QCOMPARE(M::pow(M::Complex(-infinity, 0), M::Real(-1)), M::Complex(0, 0));
    QCOMPARE(M::pow(M::Complex(0, +infinity), M::Real(-1)), M::Complex(0, 0));
    QCOMPARE(M::pow(M::Complex(0, -infinity), M::Real(-1)), M::Complex(0, 0));

    // The four scenarios tested below are technically wrong but the complexity required to determine the actual
    // correct result is surprisingly involved.  For now the function just throws up it's hands and reports NaN as
    // that is the most likely result.
    //
    // TODO: Update the tests if/when the M::pow function's behaviors are addressed.

    actual = M::pow(M::Complex(+infinity, +infinity), M::Real(-1));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, -infinity), M::Real(-1));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, +infinity), M::Real(-1));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, -infinity), M::Real(-1));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(NaN, 0), M::Real(0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Complex(0, NaN), M::Real(0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(NaN, NaN), M::Real(0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);
}


void TestBasicFunctions::testComplexPowRealComplex() {
    std::mt19937                              rng(0x12345678);
    std::exponential_distribution<M::Real>    realGenerator(0.01);
    std::uniform_int_distribution<M::Integer> integerGenerator(-128, +128);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real    base     = integerGenerator(rng) < 0 ? -realGenerator(rng) : +realGenerator(rng);
        M::Complex exponent(
            integerGenerator(rng) < 0 ? -realGenerator(rng) : +realGenerator(rng),
            integerGenerator(rng) < 0 ? -realGenerator(rng) : +realGenerator(rng)
        );

        M::Complex            expected = M::pow(base, exponent);
        std::complex<M::Real> actual   = std::pow(
            std::complex<M::Real>(base),
            std::complex<M::Real>(exponent.real(), exponent.imag())
        );

        // Ignore NaN and infinity cases for now as the standard library is broken on at least some platforms.  We'll
        // test these cases separately.
        if (!std::isnan(actual.real()) && !std::isnan(actual.imag()) &&
            !std::isinf(actual.real()) && !std::isinf(actual.imag())    ) {
            QCOMPARE(expected.real(), actual.real());
            QCOMPARE(expected.imag(), actual.imag());
        }
    }

    // check special cases
    M::Complex actual = M::pow(M::Real(0), M::Complex(0, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Real(0), M::Complex(+infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Real(0), M::Complex(-infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Real(0), M::Complex(0, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Real(0), M::Complex(0, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Real(0), M::Complex(+infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Real(0), M::Complex(+infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Real(0), M::Complex(-infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Real(0), M::Complex(-infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);


    actual = M::pow(+infinity, M::Complex(0, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(+infinity, M::Complex(+infinity, 0));
    QCOMPARE(actual, M::Complex(+infinity, 0));

    actual = M::pow(+infinity, M::Complex(-infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(+infinity, M::Complex(0, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(+infinity, M::Complex(0, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(+infinity, M::Complex(+infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(+infinity, M::Complex(+infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(+infinity, M::Complex(-infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(+infinity, M::Complex(-infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);


    actual = M::pow(-infinity, M::Complex(0, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(-infinity, M::Complex(+infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(-infinity, M::Complex(-infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(-infinity, M::Complex(0, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(-infinity, M::Complex(0, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(-infinity, M::Complex(+infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(-infinity, M::Complex(+infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(-infinity, M::Complex(-infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(-infinity, M::Complex(-infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);


    QCOMPARE(M::pow(M::Real(1), M::Complex(+infinity, 0)), M::Complex(1, 0));
    QCOMPARE(M::pow(M::Real(1), M::Complex(-infinity, 0)), M::Complex(1, 0));

    actual = M::pow(M::Real(1), M::Complex(0, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Real(1), M::Complex(0, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Real(1), M::Complex(+infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Real(1), M::Complex(+infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Real(1), M::Complex(-infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Real(1), M::Complex(-infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);


    actual = M::pow(M::Real(-1), M::Complex(+infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Real(-1), M::Complex(-infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Real(-1), M::Complex(0, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Real(-1), M::Complex(0, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Real(-1), M::Complex(+infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Real(-1), M::Complex(+infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Real(-1), M::Complex(-infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Real(-1), M::Complex(-infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);


    actual = M::pow(M::Real(+1), M::Complex(NaN, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Real(+1), M::Complex(0, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Real(+1), M::Complex(NaN, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Real(0), M::Complex(NaN, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Real(0), M::Complex(0, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Real(0), M::Complex(NaN, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Real(-1), M::Complex(NaN, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Real(-1), M::Complex(0, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Real(-1), M::Complex(NaN, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(NaN, M::Complex(NaN, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(NaN, M::Complex(0, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(NaN, M::Complex(NaN, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);
}


void TestBasicFunctions::testComplexPowComplexComplex() {
    std::mt19937                              rng(0x12345678);
    std::exponential_distribution<M::Real>    realGenerator(0.01);
    std::uniform_int_distribution<M::Integer> integerGenerator(-128, +128);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex base(
            integerGenerator(rng) < 0 ? -realGenerator(rng) : +realGenerator(rng),
            integerGenerator(rng) < 0 ? -realGenerator(rng) : +realGenerator(rng)
        );
        M::Complex exponent(
            integerGenerator(rng) < 0 ? -realGenerator(rng) : +realGenerator(rng),
            integerGenerator(rng) < 0 ? -realGenerator(rng) : +realGenerator(rng)
        );

        M::Complex            expected = M::pow(base, exponent);
        std::complex<M::Real> actual   = std::pow(
            std::complex<M::Real>(base.real(), base.imag()),
            std::complex<M::Real>(exponent.real(), exponent.imag())
        );

        // Ignore NaN and infinity cases for now as the standard library is broken on at least some platforms.  We'll
        // test these cases separately.
        if (!std::isnan(actual.real()) && !std::isnan(actual.imag()) &&
            !std::isinf(actual.real()) && !std::isinf(actual.imag())    ) {
            QCOMPARE(expected.real(), actual.real());
            QCOMPARE(expected.imag(), actual.imag());
        }
    }

    // check special cases
    M::Complex actual = M::pow(M::Complex(0, 0), M::Complex(0, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Complex(0, 0), M::Complex(+infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Complex(0, 0), M::Complex(-infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Complex(0, 0), M::Complex(0, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(0, 0), M::Complex(0, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(0, 0), M::Complex(+infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(0, 0), M::Complex(+infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(0, 0), M::Complex(-infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(0, 0), M::Complex(-infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);


    actual = M::pow(M::Complex(+infinity, 0), M::Complex(0, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Complex(+infinity, 0), M::Complex(+infinity, 0));
    QCOMPARE(actual, M::Complex(+infinity, 0));

    actual = M::pow(M::Complex(+infinity, 0), M::Complex(-infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Complex(+infinity, 0), M::Complex(0, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, 0), M::Complex(0, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, 0), M::Complex(+infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, 0), M::Complex(+infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, 0), M::Complex(-infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, 0), M::Complex(-infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);


    actual = M::pow(M::Complex(-infinity, 0), M::Complex(0, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Complex(-infinity, 0), M::Complex(+infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Complex(-infinity, 0), M::Complex(-infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Complex(-infinity, 0), M::Complex(0, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, 0), M::Complex(0, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, 0), M::Complex(+infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, 0), M::Complex(+infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, 0), M::Complex(-infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, 0), M::Complex(-infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    // Some of the scenarios below are not correct but they do describe how the M::pow function currently behaves
    // There is a TODO to fix this behavior so eventually these tests will need to be updated.

    actual = M::pow(M::Complex(+infinity, +infinity), M::Complex(0, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, +infinity), M::Complex(+infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, +infinity), M::Complex(-infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, +infinity), M::Complex(0, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, +infinity), M::Complex(0, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, +infinity), M::Complex(+infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, +infinity), M::Complex(+infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, +infinity), M::Complex(-infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, +infinity), M::Complex(-infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    // Some of the scenarios below are not correct but they do describe how the M::pow function currently behaves
    // There is a TODO to fix this behavior so eventually these tests will need to be updated.

    actual = M::pow(M::Complex(-infinity, +infinity), M::Complex(0, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, +infinity), M::Complex(+infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, +infinity), M::Complex(-infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, +infinity), M::Complex(0, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, +infinity), M::Complex(0, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, +infinity), M::Complex(+infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, +infinity), M::Complex(+infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, +infinity), M::Complex(-infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, +infinity), M::Complex(-infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    // Some of the scenarios below are not correct but they do describe how the M::pow function currently behaves
    // There is a TODO to fix this behavior so eventually these tests will need to be updated.

    actual = M::pow(M::Complex(+infinity, -infinity), M::Complex(0, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, -infinity), M::Complex(+infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, -infinity), M::Complex(-infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, -infinity), M::Complex(0, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, -infinity), M::Complex(0, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, -infinity), M::Complex(+infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, -infinity), M::Complex(+infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, -infinity), M::Complex(-infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+infinity, -infinity), M::Complex(-infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    // Some of the scenarios below are not correct but they do describe how the M::pow function currently behaves
    // There is a TODO to fix this behavior so eventually these tests will need to be updated.

    actual = M::pow(M::Complex(-infinity, -infinity), M::Complex(0, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, -infinity), M::Complex(+infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, -infinity), M::Complex(-infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, -infinity), M::Complex(0, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, -infinity), M::Complex(0, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, -infinity), M::Complex(+infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, -infinity), M::Complex(+infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, -infinity), M::Complex(-infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-infinity, -infinity), M::Complex(-infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);


    QCOMPARE(M::pow(M::Complex(1, 0), M::Complex(+infinity, 0)), M::Complex(1, 0));
    QCOMPARE(M::pow(M::Complex(1, 0), M::Complex(-infinity, 0)), M::Complex(1, 0));

    actual = M::pow(M::Complex(1, 0), M::Complex(0, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(1, 0), M::Complex(0, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(1, 0), M::Complex(+infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(1, 0), M::Complex(+infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(1, 0), M::Complex(-infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(1, 0), M::Complex(-infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);


    actual = M::pow(M::Complex(1, 1), M::Complex(+infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(1, 1), M::Complex(0, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(1, 1), M::Complex(0, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(1, 1), M::Complex(+infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(1, 1), M::Complex(+infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(1, 1), M::Complex(-infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(1, 1), M::Complex(-infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(1, -1), M::Complex(+infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(1, -1), M::Complex(0, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(1, -1), M::Complex(0, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(1, -1), M::Complex(+infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(1, -1), M::Complex(+infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(1, -1), M::Complex(-infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(1, -1), M::Complex(-infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);


    actual = M::pow(M::Complex(-1, 0), M::Complex(+infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Complex(-1, 0), M::Complex(-infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Complex(-1, 0), M::Complex(0, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-1, 0), M::Complex(0, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-1, 0), M::Complex(+infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-1, 0), M::Complex(+infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-1, 0), M::Complex(-infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-1, 0), M::Complex(-infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);


    actual = M::pow(M::Complex(-1, +1), M::Complex(+infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-1, +1), M::Complex(-infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-1, +1), M::Complex(0, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-1, +1), M::Complex(0, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-1, +1), M::Complex(+infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-1, +1), M::Complex(+infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-1, +1), M::Complex(-infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-1, +1), M::Complex(-infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);


    actual = M::pow(M::Complex(-1, -1), M::Complex(+infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-1, -1), M::Complex(-infinity, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-1, -1), M::Complex(0, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-1, -1), M::Complex(0, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-1, -1), M::Complex(+infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-1, -1), M::Complex(+infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-1, -1), M::Complex(-infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-1, -1), M::Complex(-infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);


    actual = M::pow(M::Complex(+1, 0), M::Complex(NaN, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Complex(+1, 0), M::Complex(0, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+1, 0), M::Complex(NaN, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(0, 0), M::Complex(NaN, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Complex(0, 0), M::Complex(0, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(0, 0), M::Complex(NaN, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-1, 0), M::Complex(NaN, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Complex(-1, 0), M::Complex(0, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-1, 0), M::Complex(NaN, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);


    actual = M::pow(M::Complex(+1, +1), M::Complex(NaN, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+1, +1), M::Complex(0, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+1, +1), M::Complex(NaN, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(0, +1), M::Complex(NaN, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(0, +1), M::Complex(0, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(0, +1), M::Complex(NaN, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-1, +1), M::Complex(NaN, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-1, +1), M::Complex(0, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-1, +1), M::Complex(NaN, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);


    actual = M::pow(M::Complex(+1, -1), M::Complex(NaN, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+1, -1), M::Complex(0, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(+1, -1), M::Complex(NaN, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(0, -1), M::Complex(NaN, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(0, -1), M::Complex(0, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(0, -1), M::Complex(NaN, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-1, -1), M::Complex(NaN, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-1, -1), M::Complex(0, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(-1, -1), M::Complex(NaN, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);


    actual = M::pow(M::Complex(NaN, 0), M::Complex(0, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Complex(NaN, 0), M::Complex(NaN, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::pow(M::Complex(NaN, 0), M::Complex(0, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(NaN, 0), M::Complex(NaN, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);


    actual = M::pow(M::Complex(0, NaN), M::Complex(0, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(0, NaN), M::Complex(NaN, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(0, NaN), M::Complex(0, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(0, NaN), M::Complex(NaN, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);


    actual = M::pow(M::Complex(NaN, NaN), M::Complex(0, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(NaN, NaN), M::Complex(NaN, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(NaN, NaN), M::Complex(0, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::pow(M::Complex(NaN, NaN), M::Complex(NaN, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);
}


void TestBasicFunctions::testComplexPowVariants() {
    M::Boolean boolean = true;
    M::Integer integer = 2;
    M::Real    real    = 3.0;
    M::Complex complex = M::Complex(4.0, 5.0);

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);

    QCOMPARE(M::pow(vBoolean, boolean), M::pow(boolean, boolean));
    QCOMPARE(M::pow(vBoolean, integer), M::pow(boolean, integer));
    QCOMPARE(M::pow(vBoolean, real), M::pow(boolean, real));
    QCOMPARE(M::pow(vBoolean, complex), M::pow(boolean, complex));

    QCOMPARE(M::pow(vInteger, boolean), M::pow(integer, boolean));
    QCOMPARE(M::pow(vInteger, integer), M::pow(integer, integer));
    QCOMPARE(M::pow(vInteger, real), M::pow(integer, real));
    QCOMPARE(M::pow(vInteger, complex), M::pow(integer, complex));

    QCOMPARE(M::pow(vReal, boolean), M::pow(real, boolean));
    QCOMPARE(M::pow(vReal, integer), M::pow(real, integer));
    QCOMPARE(M::pow(vReal, real), M::pow(real, real));
    QCOMPARE(M::pow(vReal, complex), M::pow(real, complex));

    QCOMPARE(M::pow(vComplex, boolean), M::pow(complex, boolean));
    QCOMPARE(M::pow(vComplex, integer), M::pow(complex, integer));
    QCOMPARE(M::pow(vComplex, real), M::pow(complex, real));
    QCOMPARE(M::pow(vComplex, complex), M::pow(complex, complex));

    QCOMPARE(M::pow(boolean, vBoolean), M::pow(boolean, boolean));
    QCOMPARE(M::pow(boolean, vInteger), M::pow(boolean, integer));
    QCOMPARE(M::pow(boolean, vReal), M::pow(boolean, real));
    QCOMPARE(M::pow(boolean, vComplex), M::pow(boolean, complex));

    QCOMPARE(M::pow(integer, vBoolean), M::pow(integer, boolean));
    QCOMPARE(M::pow(integer, vInteger), M::pow(integer, integer));
    QCOMPARE(M::pow(integer, vReal), M::pow(integer, real));
    QCOMPARE(M::pow(integer, vComplex), M::pow(integer, complex));

    QCOMPARE(M::pow(real, vBoolean), M::pow(real, boolean));
    QCOMPARE(M::pow(real, vInteger), M::pow(real, integer));
    QCOMPARE(M::pow(real, vReal), M::pow(real, real));
    QCOMPARE(M::pow(real, vComplex), M::pow(real, complex));

    QCOMPARE(M::pow(complex, vBoolean), M::pow(complex, boolean));
    QCOMPARE(M::pow(complex, vInteger), M::pow(complex, integer));
    QCOMPARE(M::pow(complex, vReal), M::pow(complex, real));
    QCOMPARE(M::pow(complex, vComplex), M::pow(complex, complex));

    QCOMPARE(M::pow(vBoolean, vBoolean), M::pow(boolean, boolean));
    QCOMPARE(M::pow(vBoolean, vInteger), M::pow(boolean, integer));
    QCOMPARE(M::pow(vBoolean, vReal), M::pow(boolean, real));
    QCOMPARE(M::pow(vBoolean, vComplex), M::pow(boolean, complex));

    QCOMPARE(M::pow(vInteger, vBoolean), M::pow(integer, boolean));
    QCOMPARE(M::pow(vInteger, vInteger), M::pow(integer, integer));
    QCOMPARE(M::pow(vInteger, vReal), M::pow(integer, real));
    QCOMPARE(M::pow(vInteger, vComplex), M::pow(integer, complex));

    QCOMPARE(M::pow(vReal, vBoolean), M::pow(real, boolean));
    QCOMPARE(M::pow(vReal, vInteger), M::pow(real, integer));
    QCOMPARE(M::pow(vReal, vReal), M::pow(real, real));
    QCOMPARE(M::pow(vReal, vComplex), M::pow(real, complex));

    QCOMPARE(M::pow(vComplex, vBoolean), M::pow(complex, boolean));
    QCOMPARE(M::pow(vComplex, vInteger), M::pow(complex, integer));
    QCOMPARE(M::pow(vComplex, vReal), M::pow(complex, real));
    QCOMPARE(M::pow(vComplex, vComplex), M::pow(complex, complex));
}


void TestBasicFunctions::testComplexSqrtInteger() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> integerGenerator(
        std::numeric_limits<M::Integer>::min(),
        std::numeric_limits<M::Integer>::max()
    );

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer value = integerGenerator(rng);

        std::complex<M::Complex::T> stdResult = std::sqrt(std::complex<M::Complex::T>(value, 0));
        M::Complex                  result    = M::sqrt(value);

        QCOMPARE(result.real(), stdResult.real());
        QCOMPARE(result.imag(), stdResult.imag());

        M::Real squaredResult = (result * result).real();
        M::Real error         = M::abs(squaredResult - M::Real(value));
        M::Real relativeError = value == 0 ? error : M::abs(error / M::Real(value));

        QCOMPARE(relativeError < 16 * epsilon, true);
    }
}


void TestBasicFunctions::testComplexSqrtReal() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> realGenerator(
        std::numeric_limits<M::Real>::lowest() / 2.0,
        std::numeric_limits<M::Real>::max() / 2.0
    );

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real value = realGenerator(rng);

        std::complex<M::Complex::T> stdResult = std::sqrt(std::complex<M::Complex::T>(value, 0));
        M::Complex                  result    = M::sqrt(value);

        QCOMPARE(result.real(), stdResult.real());
        QCOMPARE(result.imag(), stdResult.imag());

        M::Real squaredResult = (result * result).real();
        M::Real error         = M::abs(squaredResult - M::Real(value));
        M::Real relativeError = value == 0 ? error : M::abs(error / M::Real(value));

        QCOMPARE(relativeError < 2 * epsilon, true);
    }

    // Check edge cases
    QCOMPARE(M::sqrt(+infinity), M::Complex(+infinity, 0));
    QCOMPARE(M::sqrt(-infinity), M::Complex(0, +infinity));
    QCOMPARE(M::sqrt(M::Real(0)), M::Complex(0, 0));

    M::Complex actual = M::sqrt(NaN);
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));
}


void TestBasicFunctions::testComplexSqrtComplex() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> realGenerator(
        std::numeric_limits<M::Real>::lowest() / 2.0,
        std::numeric_limits<M::Real>::max() / 2.0
    );

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real rv = realGenerator(rng);
        M::Real iv = realGenerator(rng);
        M::Complex value(rv, iv);

        std::complex<M::Complex::T> stdResult = std::sqrt(std::complex<M::Complex::T>(rv, iv));
        M::Complex                  result    = M::sqrt(value);

        QCOMPARE(result.real(), stdResult.real());
        QCOMPARE(result.imag(), stdResult.imag());

        M::Complex squaredResult = result * result;
        M::Real    error         = M::abs(squaredResult - value);
        M::Real    relativeError = value == 0 ? error : (error / M::abs(value));

        if (relativeError > (3*epsilon)) {
            qDebug() << relativeError << " " << epsilon;
            QCOMPARE(relativeError <= (3 * epsilon), true);
        }
    }

    // Check edge cases
    QCOMPARE(M::sqrt(M::Complex(+infinity, 0)), M::Complex(+infinity, 0));
    QCOMPARE(M::sqrt(M::Complex(-infinity, 0)), M::Complex(0, +infinity));
    QCOMPARE(M::sqrt(M::Complex(0, +infinity)), M::Complex(+infinity, +infinity));
    QCOMPARE(M::sqrt(M::Complex(0, -infinity)), M::Complex(+infinity, -infinity));

    M::Complex actual =M::sqrt(M::Complex(+infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual =M::sqrt(M::Complex(+infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual =M::sqrt(M::Complex(-infinity, +infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual =M::sqrt(M::Complex(-infinity, -infinity));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    QCOMPARE(M::sqrt(M::Real(0)), M::Complex(0, 0));

    actual = M::sqrt(M::Complex(NaN, 0));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(actual.imag(), M::Real(0));

    actual = M::sqrt(M::Complex(0, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);

    actual = M::sqrt(M::Complex(NaN, NaN));
    QCOMPARE(std::isnan(actual.real()), true);
    QCOMPARE(std::isnan(actual.imag()), true);
}


void TestBasicFunctions::testComplexSqrtVariant() {
    M::Boolean boolean = true;
    M::Integer integer = 2;
    M::Real    real    = 3.0;
    M::Complex complex = M::Complex(4.0, 5.0);

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);

    QCOMPARE(M::sqrt(vBoolean), M::sqrt(boolean));
    QCOMPARE(M::sqrt(vInteger), M::sqrt(integer));
    QCOMPARE(M::sqrt(vReal), M::sqrt(real));
    QCOMPARE(M::sqrt(vComplex), M::sqrt(complex));
}


void TestBasicFunctions::testComplexNRootIntegerInteger() {
    // The M::nroot function uses the M::pow function so we do more trivial testing, at least more now.

    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> baseGenerator(
        std::numeric_limits<M::Integer>::min(),
        std::numeric_limits<M::Integer>::max()
    );
    std::uniform_int_distribution<M::Integer> rootGenerator(1, 16);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer base = baseGenerator(rng);
        M::Integer root;

        // The nroot/pow function short circuit a root of 2, using a square root function. Our result therefore
        // differs from the std::pow approach below.
        do {
            root = rootGenerator(rng);
        } while (root == 2);

        std::complex<M::Complex::T> expected = std::pow(std::complex<M::Complex::T>(base, 0), 1.0 / M::Real(root));
        M::Complex                  actual = M::nroot(base, root);

        if (actual.real() != expected.real() || actual.imag() != expected.imag()) {
            QCOMPARE(actual.real(), expected.real());
            QCOMPARE(actual.imag(), expected.imag());
        }
    }
}


void TestBasicFunctions::testComplexNRootIntegerReal() {
    // The M::nroot function uses the M::pow function so we do more trivial testing, at least more now.

    std::mt19937                            rng(0x12345678);
    std::uniform_int_distribution<M::Integer> baseGenerator(
        std::numeric_limits<M::Integer>::min(),
        std::numeric_limits<M::Integer>::max()
    );
    std::uniform_real_distribution<M::Real> realGenerator(0, 64.0);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer base = baseGenerator(rng);
        M::Real    root;

        // The nroot/pow function short circuit a root of 2, using a square root function. Our result therefore
        // differs from the std::pow approach below.
        do {
            root = realGenerator(rng);
        } while (root == M::Real(2));

        std::complex<M::Complex::T> expected = std::pow(std::complex<M::Complex::T>(base, 0), 1.0 / M::Real(root));
        M::Complex                  actual = M::nroot(base, root);

        if (actual.real() != expected.real() || actual.imag() != expected.imag()) {
            QCOMPARE(actual.real(), expected.real());
            QCOMPARE(actual.imag(), expected.imag());
        }
    }
}


void TestBasicFunctions::testComplexNRootIntegerComplex() {
    // The M::nroot function uses the M::pow function so we do more trivial testing, at least more now.

    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> realGenerator(0, 64.0);
    std::uniform_int_distribution<M::Integer> baseGenerator(
        std::numeric_limits<M::Integer>::min(),
        std::numeric_limits<M::Integer>::max()
    );

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer base = baseGenerator(rng);
        M::Complex root;

        // The nroot/pow function short circuit a root of 2, using a square root function. Our result therefore
        // differs from the std::pow approach below.
        do {
            root = M::Complex(realGenerator(rng), realGenerator(rng));
        } while (root == M::Complex(2,0) || root == M::Complex(0, 0));

        M::Complex::T d = root.real() * root.real() + root.imag() * root.imag();
        std::complex<M::Complex::T> resiprocalRoot(root.real() / d, -root.imag() / d);
        std::complex<M::Complex::T> expected = std::pow(std::complex<M::Complex::T>(base, 0), resiprocalRoot);

        M::Complex actual = M::nroot(base, root);

        M::Complex::T error         = M::abs(actual - M::Complex(expected.real(), expected.imag()));
        M::Complex::T relativeError = error / M::abs(actual);

        QCOMPARE(relativeError <= 1 * epsilon, true);
    }
}


void TestBasicFunctions::testComplexNRootRealInteger() {
    // The M::nroot function uses the M::pow function so we do more trivial testing, at least more now.

    std::mt19937                              rng(0x12345678);
    std::uniform_real_distribution<M::Real> baseGenerator(
        std::numeric_limits<M::Real>::lowest() / 2.0,
        std::numeric_limits<M::Real>::max() / 2.0
    );
    std::uniform_int_distribution<M::Integer> rootGenerator(1, 16);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real    base = baseGenerator(rng);
        M::Integer root;

        // The nroot/pow function short circuit a root of 2, using a square root function. Our result therefore
        // differs from the std::pow approach below.
        do {
            root = rootGenerator(rng);
        } while (root == 2);

        std::complex<M::Complex::T> expected = std::pow(std::complex<M::Complex::T>(base, 0), 1.0 / M::Real(root));
        M::Complex                  actual   = M::nroot(base, root);

        if (actual.real() != expected.real() || actual.imag() != expected.imag()) {
            QCOMPARE(actual.real(), expected.real());
            QCOMPARE(actual.imag(), expected.imag());
        }
    }
}


void TestBasicFunctions::testComplexNRootRealReal() {
    // The M::nroot function uses the M::pow function so we do more trivial testing, at least more now.

    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> baseGenerator(
        std::numeric_limits<M::Real>::lowest() / 2.0,
        std::numeric_limits<M::Real>::max() / 2.0
    );
    std::uniform_real_distribution<M::Real> rootGenerator(0, 64.0);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real base = baseGenerator(rng);
        M::Real root;

        // The nroot/pow function short circuit a root of 2, using a square root function. Our result therefore
        // differs from the std::pow approach below.
        do {
            root = rootGenerator(rng);
        } while (root == M::Real(2) || root == M::Real(0));

        std::complex<M::Complex::T> expected = std::pow(std::complex<M::Complex::T>(base, 0), 1.0 / M::Real(root));
        M::Complex                  actual   = M::nroot(base, root);

        if (actual.real() != expected.real() || actual.imag() != expected.imag()) {
            QCOMPARE(actual.real(), expected.real());
            QCOMPARE(actual.imag(), expected.imag());
        }
    }
}


void TestBasicFunctions::testComplexNRootRealComplex() {
    // The M::nroot function uses the M::pow function so we do more trivial testing, at least more now.

    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> baseGenerator(
        std::numeric_limits<M::Real>::lowest() / 2.0,
        std::numeric_limits<M::Real>::max() / 2.0
    );
    std::uniform_real_distribution<M::Real> rootGenerator(0, 64.0);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real    base = baseGenerator(rng);
        M::Complex root;

        // The nroot/pow function short circuit a root of 2, using a square root function. Our result therefore
        // differs from the std::pow approach below.
        do {
            root = M::Complex(rootGenerator(rng), rootGenerator(rng));
        } while (root == M::Complex(2, 0) || root == M::Complex(0, 0));

        M::Complex::T d = root.real() * root.real() + root.imag() * root.imag();
        std::complex<M::Complex::T> resiprocalRoot(root.real() / d, -root.imag() / d);
        std::complex<M::Complex::T> expected = std::pow(std::complex<M::Complex::T>(base, 0), resiprocalRoot);
        M::Complex                  actual   = M::nroot(base, root);

        if (actual.real() != expected.real() || actual.imag() != expected.imag()) {
            QCOMPARE(actual.real(), expected.real());
            QCOMPARE(actual.imag(), expected.imag());
        }
    }
}


void TestBasicFunctions::testComplexNRootComplexInteger() {
    // The M::nroot function uses the M::pow function so we do more trivial testing, at least more now.

    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> baseGenerator(
        std::numeric_limits<M::Real>::lowest() / 2.0,
        std::numeric_limits<M::Real>::max() / 2.0
    );
    std::uniform_int_distribution<M::Integer> rootGenerator(1, 16);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex base(baseGenerator(rng), baseGenerator(rng));
        M::Integer root;

        // The nroot/pow function short circuit a root of 2, using a square root function. Our result therefore
        // differs from the std::pow approach below.
        do {
            root = rootGenerator(rng);
        } while (root == M::Real(2));

        std::complex<M::Complex::T> expected = std::pow(
            std::complex<M::Complex::T>(base.real(), base.imag()),
            1.0 / M::Real(root)
        );
        M::Complex actual   = M::nroot(base, root);

        if (actual.real() != expected.real() || actual.imag() != expected.imag()) {
            QCOMPARE(actual.real(), expected.real());
            QCOMPARE(actual.imag(), expected.imag());
        }
    }
}


void TestBasicFunctions::testComplexNRootComplexReal() {
    // The M::nroot function uses the M::pow function so we do more trivial testing, at least more now.

    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> baseGenerator(
        std::numeric_limits<M::Real>::lowest() / 2.0,
        std::numeric_limits<M::Real>::max() / 2.0
    );
    std::uniform_real_distribution<M::Real> rootGenerator(0, 64.0);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex base(baseGenerator(rng), baseGenerator(rng));
        M::Real    root;

        // The nroot/pow function short circuit a root of 2, using a square root function. Our result therefore
        // differs from the std::pow approach below.
        do {
            root = rootGenerator(rng);
        } while (root == M::Real(2) || root == M::Real(0));

        std::complex<M::Complex::T> expected = std::pow(
            std::complex<M::Complex::T>(base.real(), base.imag()),
            1.0 / root
        );
        M::Complex actual = M::nroot(base, root);

        if (actual.real() != expected.real() || actual.imag() != expected.imag()) {
            QCOMPARE(actual.real(), expected.real());
            QCOMPARE(actual.imag(), expected.imag());
        }
    }
}


void TestBasicFunctions::testComplexNRootComplexComplex() {
    // The M::nroot function uses the M::pow function so we do more trivial testing, at least more now.

    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> baseGenerator(
        std::numeric_limits<M::Real>::lowest() / 2.0,
        std::numeric_limits<M::Real>::max() / 2.0
    );
    std::uniform_real_distribution<M::Real> rootGenerator(0, 64.0);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex base(baseGenerator(rng), baseGenerator(rng));
        M::Complex root;

        // The nroot/pow function short circuit a root of 2, using a square root function. Our result therefore
        // differs from the std::pow approach below.
        do {
            root = M::Complex(rootGenerator(rng), rootGenerator(rng));
        } while (root == M::Complex(2, 0) || root == M::Complex(0, 0));

        M::Complex::T d = root.real() * root.real() + root.imag() * root.imag();
        std::complex<M::Complex::T> resiprocalRoot(root.real() / d, -root.imag() / d);
        std::complex<M::Complex::T> expected = std::pow(
            std::complex<M::Complex::T>(base.real(), base.imag()),
            resiprocalRoot
        );
        M::Complex actual = M::nroot(base, root);

        if (actual.real() != expected.real() || actual.imag() != expected.imag()) {
            QCOMPARE(actual.real(), expected.real());
            QCOMPARE(actual.imag(), expected.imag());
        }
    }
}


void TestBasicFunctions::testComplexNRootVariants() {
    M::Boolean boolean = true;
    M::Integer integer = 2;
    M::Real    real    = 3.0;
    M::Complex complex = M::Complex(4.0, 5.0);

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);

    QCOMPARE(M::nroot(vBoolean, boolean), M::nroot(boolean, boolean));
    QCOMPARE(M::nroot(vBoolean, integer), M::nroot(boolean, integer));
    QCOMPARE(M::nroot(vBoolean, real), M::nroot(boolean, real));
    QCOMPARE(M::nroot(vBoolean, complex), M::nroot(boolean, complex));

    QCOMPARE(M::nroot(vInteger, boolean), M::nroot(integer, boolean));
    QCOMPARE(M::nroot(vInteger, integer), M::nroot(integer, integer));
    QCOMPARE(M::nroot(vInteger, real), M::nroot(integer, real));
    QCOMPARE(M::nroot(vInteger, complex), M::nroot(integer, complex));

    QCOMPARE(M::nroot(vReal, boolean), M::nroot(real, boolean));
    QCOMPARE(M::nroot(vReal, integer), M::nroot(real, integer));
    QCOMPARE(M::nroot(vReal, real), M::nroot(real, real));
    QCOMPARE(M::nroot(vReal, complex), M::nroot(real, complex));

    QCOMPARE(M::nroot(vComplex, boolean), M::nroot(complex, boolean));
    QCOMPARE(M::nroot(vComplex, integer), M::nroot(complex, integer));
    QCOMPARE(M::nroot(vComplex, real), M::nroot(complex, real));
    QCOMPARE(M::nroot(vComplex, complex), M::nroot(complex, complex));

    QCOMPARE(M::nroot(boolean, vBoolean), M::nroot(boolean, boolean));
    QCOMPARE(M::nroot(boolean, vInteger), M::nroot(boolean, integer));
    QCOMPARE(M::nroot(boolean, vReal), M::nroot(boolean, real));
    QCOMPARE(M::nroot(boolean, vComplex), M::nroot(boolean, complex));

    QCOMPARE(M::nroot(integer, vBoolean), M::nroot(integer, boolean));
    QCOMPARE(M::nroot(integer, vInteger), M::nroot(integer, integer));
    QCOMPARE(M::nroot(integer, vReal), M::nroot(integer, real));
    QCOMPARE(M::nroot(integer, vComplex), M::nroot(integer, complex));

    QCOMPARE(M::nroot(real, vBoolean), M::nroot(real, boolean));
    QCOMPARE(M::nroot(real, vInteger), M::nroot(real, integer));
    QCOMPARE(M::nroot(real, vReal), M::nroot(real, real));
    QCOMPARE(M::nroot(real, vComplex), M::nroot(real, complex));

    QCOMPARE(M::nroot(complex, vBoolean), M::nroot(complex, boolean));
    QCOMPARE(M::nroot(complex, vInteger), M::nroot(complex, integer));
    QCOMPARE(M::nroot(complex, vReal), M::nroot(complex, real));
    QCOMPARE(M::nroot(complex, vComplex), M::nroot(complex, complex));

    QCOMPARE(M::nroot(vBoolean, vBoolean), M::nroot(boolean, boolean));
    QCOMPARE(M::nroot(vBoolean, vInteger), M::nroot(boolean, integer));
    QCOMPARE(M::nroot(vBoolean, vReal), M::nroot(boolean, real));
    QCOMPARE(M::nroot(vBoolean, vComplex), M::nroot(boolean, complex));

    QCOMPARE(M::nroot(vInteger, vBoolean), M::nroot(integer, boolean));
    QCOMPARE(M::nroot(vInteger, vInteger), M::nroot(integer, integer));
    QCOMPARE(M::nroot(vInteger, vReal), M::nroot(integer, real));
    QCOMPARE(M::nroot(vInteger, vComplex), M::nroot(integer, complex));

    QCOMPARE(M::nroot(vReal, vBoolean), M::nroot(real, boolean));
    QCOMPARE(M::nroot(vReal, vInteger), M::nroot(real, integer));
    QCOMPARE(M::nroot(vReal, vReal), M::nroot(real, real));
    QCOMPARE(M::nroot(vReal, vComplex), M::nroot(real, complex));

    QCOMPARE(M::nroot(vComplex, vBoolean), M::nroot(complex, boolean));
    QCOMPARE(M::nroot(vComplex, vInteger), M::nroot(complex, integer));
    QCOMPARE(M::nroot(vComplex, vReal), M::nroot(complex, real));
    QCOMPARE(M::nroot(vComplex, vComplex), M::nroot(complex, complex));
}


void TestBasicFunctions::testIntegerConjInteger() {
    QCOMPARE(M::conj(M::Integer(-1)), M::Integer(-1));
    QCOMPARE(M::conj(M::Integer(0)), M::Integer(0));
    QCOMPARE(M::conj(M::Integer(1)), M::Integer(1));
}


void TestBasicFunctions::testRealConjReal() {
    QCOMPARE(M::conj(M::Real(-1)), M::Real(-1));
    QCOMPARE(M::conj(M::Real(0)), M::Real(0));
    QCOMPARE(M::conj(M::Real(1)), M::Real(1));

    // Check special cases.
    QCOMPARE(M::conj(+infinity), +infinity);
    QCOMPARE(M::conj(-infinity), -infinity);

    M::Real result = M::conj(NaN);
    QCOMPARE(std::isnan(result), true);
}


void TestBasicFunctions::testComplexConjComplex() {
    QCOMPARE(M::conj(M::Complex(-1, 0)), M::Complex(-1, 0));
    QCOMPARE(M::conj(M::Complex(0, 0)), M::Complex(0, 0));
    QCOMPARE(M::conj(M::Complex(1, 0)), M::Complex(1, 0));

    QCOMPARE(M::conj(M::Complex(-1, -1)), M::Complex(-1, +1));
    QCOMPARE(M::conj(M::Complex(0, -1)), M::Complex(0, +1));
    QCOMPARE(M::conj(M::Complex(1, -1)), M::Complex(1, +1));

    QCOMPARE(M::conj(M::Complex(-1, +1)), M::Complex(-1, -1));
    QCOMPARE(M::conj(M::Complex(0, +1)), M::Complex(0, -1));
    QCOMPARE(M::conj(M::Complex(1, +1)), M::Complex(1, -1));

    // Check special cases.
    QCOMPARE(M::conj(M::Complex(+infinity, 0)), M::Complex(+infinity, 0));
    QCOMPARE(M::conj(M::Complex(-infinity, 0)), M::Complex(-infinity, 0));
    QCOMPARE(M::conj(M::Complex(0, +infinity)), M::Complex(0, -infinity));
    QCOMPARE(M::conj(M::Complex(0, -infinity)), M::Complex(0, +infinity));
    QCOMPARE(M::conj(M::Complex(+infinity, +infinity)), M::Complex(+infinity, -infinity));
    QCOMPARE(M::conj(M::Complex(+infinity, -infinity)), M::Complex(+infinity, +infinity));
    QCOMPARE(M::conj(M::Complex(-infinity, +infinity)), M::Complex(-infinity, -infinity));
    QCOMPARE(M::conj(M::Complex(-infinity, -infinity)), M::Complex(-infinity, +infinity));

    M::Complex result = M::conj(M::Complex(NaN, 0));
    QCOMPARE(std::isnan(result.real()), true);
    QCOMPARE(result.imag(), 0);

    result = M::conj(M::Complex(0, NaN));
    QCOMPARE(result.real(), 0);
    QCOMPARE(std::isnan(result.imag()), true);

    result = M::conj(M::Complex(NaN, NaN));
    QCOMPARE(std::isnan(result.real()), true);
    QCOMPARE(std::isnan(result.imag()), true);
}


void TestBasicFunctions::testIntegerAbsInteger() {
    QCOMPARE(M::abs(M::Integer(5)), M::Integer(5));
    QCOMPARE(M::abs(M::Integer(0)), M::Integer(0));
    QCOMPARE(M::abs(M::Integer(-6)), M::Integer(6));
}


void TestBasicFunctions::testRealAbsReal() {
    QCOMPARE(M::abs(M::Real(5)), M::Real(5));
    QCOMPARE(M::abs(M::Real(0)), M::Real(0));
    QCOMPARE(M::abs(M::Real(-6)), M::Real(6));

    // Check special cases.
    QCOMPARE(M::abs(+infinity), +infinity);
    QCOMPARE(M::abs(-infinity), +infinity);

    M::Real result = M::abs(NaN);
    QCOMPARE(std::isnan(result), true);
}


void TestBasicFunctions::testRealAbsComplex() {
    QCOMPARE(M::abs(M::Complex(3, -4)), M::Real(5));
    QCOMPARE(M::abs(M::Complex(6, 8)), M::Real(10));

    // Check special cases.
    QCOMPARE(M::abs(M::Complex(+infinity, 0)), +infinity);
    QCOMPARE(M::abs(M::Complex(-infinity, 0)), +infinity);
    QCOMPARE(M::abs(M::Complex(0, +infinity)), +infinity);
    QCOMPARE(M::abs(M::Complex(0, -infinity)), +infinity);
    QCOMPARE(M::abs(M::Complex(+infinity, +infinity)), +infinity);
    QCOMPARE(M::abs(M::Complex(+infinity, -infinity)), +infinity);
    QCOMPARE(M::abs(M::Complex(-infinity, +infinity)), +infinity);
    QCOMPARE(M::abs(M::Complex(-infinity, -infinity)), +infinity);

    M::Real result = M::abs(M::Complex(NaN, 0));
    QCOMPARE(std::isnan(result), true);

    result = M::abs(M::Complex(0, NaN));
    QCOMPARE(std::isnan(result), true);

    result = M::abs(M::Complex(NaN, NaN));
    QCOMPARE(std::isnan(result), true);
}


void TestBasicFunctions::testIntegerFloorInteger() {
    QCOMPARE(M::floor(M::Integer(4)), M::Integer(4));
    QCOMPARE(M::floor(M::Integer(0)), M::Integer(0));
    QCOMPARE(M::floor(M::Integer(-4)), M::Integer(-4));
}


void TestBasicFunctions::testRealFloorReal() {
    QCOMPARE(M::floor(M::Real(4)), M::Real(4));
    QCOMPARE(M::floor(M::Real(4.5)), M::Real(4));
    QCOMPARE(M::floor(M::Real(0)), M::Real(0));
    QCOMPARE(M::floor(M::Real(-4)), M::Real(-4));
    QCOMPARE(M::floor(M::Real(-4.5)), M::Real(-5));

    // Check special cases.
    QCOMPARE(M::floor(+infinity), +infinity);
    QCOMPARE(M::floor(-infinity), -infinity);

    M::Real result = M::floor(NaN);
    QCOMPARE(std::isnan(result), true);
}


void TestBasicFunctions::testRealFloorComplex() {
    QCOMPARE(M::floor(M::Complex(4)), M::Real(4));
    QCOMPARE(M::floor(M::Complex(4.5)), M::Real(4));
    QCOMPARE(M::floor(M::Complex(0)), M::Real(0));
    QCOMPARE(M::floor(M::Complex(-4)), M::Real(-4));
    QCOMPARE(M::floor(M::Complex(-4.5)), M::Real(-5));
}


void TestBasicFunctions::testRealFloorVariants() {
    M::Boolean boolean = true;
    M::Integer integer = 2;
    M::Real    real    = 3.4;
    M::Complex complex = M::Complex(4.4);

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);

    QCOMPARE(M::floor(vBoolean), M::floor(boolean));
    QCOMPARE(M::floor(vInteger), M::floor(integer));
    QCOMPARE(M::floor(vReal), M::floor(real));
    QCOMPARE(M::floor(vComplex), M::floor(complex));
}


void TestBasicFunctions::testIntegerCeilInteger() {
    QCOMPARE(M::ceil(M::Integer(4)), M::Integer(4));
    QCOMPARE(M::ceil(M::Integer(0)), M::Integer(0));
    QCOMPARE(M::ceil(M::Integer(-4)), M::Integer(-4));
}


void TestBasicFunctions::testRealCeilReal() {
    QCOMPARE(M::ceil(M::Real(4)), M::Real(4));
    QCOMPARE(M::ceil(M::Real(4.5)), M::Real(5));
    QCOMPARE(M::ceil(M::Real(0)), M::Real(0));
    QCOMPARE(M::ceil(M::Real(-4)), M::Real(-4));
    QCOMPARE(M::ceil(M::Real(-4.5)), M::Real(-4));

    // Check special cases.
    QCOMPARE(M::ceil(+infinity), +infinity);
    QCOMPARE(M::ceil(-infinity), -infinity);

    M::Real result = M::ceil(NaN);
    QCOMPARE(std::isnan(result), true);
}


void TestBasicFunctions::testRealCeilComplex() {
    QCOMPARE(M::ceil(M::Complex(4)), M::Real(4));
    QCOMPARE(M::ceil(M::Complex(4.5)), M::Real(5));
    QCOMPARE(M::ceil(M::Complex(0)), M::Real(0));
    QCOMPARE(M::ceil(M::Complex(-4)), M::Real(-4));
    QCOMPARE(M::ceil(M::Complex(-4.5)), M::Real(-4));
}


void TestBasicFunctions::testRealCeilVariants() {
    M::Boolean boolean = true;
    M::Integer integer = 2;
    M::Real    real    = 3.0;
    M::Complex complex = M::Complex(4.4);

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);

    QCOMPARE(M::ceil(vBoolean), M::ceil(boolean));
    QCOMPARE(M::ceil(vInteger), M::ceil(integer));
    QCOMPARE(M::ceil(vReal), M::ceil(real));
    QCOMPARE(M::ceil(vComplex), M::ceil(complex));
}


void TestBasicFunctions::testIntegerNIntInteger() {
    QCOMPARE(M::nint(M::Integer(4)), M::Integer(4));
    QCOMPARE(M::nint(M::Integer(0)), M::Integer(0));
    QCOMPARE(M::nint(M::Integer(-4)), M::Integer(-4));
}


void TestBasicFunctions::testRealNIntReal() {
    QCOMPARE(M::nint(M::Real(4)), M::Real(4));
    QCOMPARE(M::nint(M::Real(4.2)), M::Real(4));
    QCOMPARE(M::nint(M::Real(4.5 - 4 * epsilon)), M::Real(4));
    QCOMPARE(M::nint(M::Real(4.5)), M::Real(4));
    QCOMPARE(M::nint(M::Real(4.5 + 4 * epsilon)), M::Real(5));
    QCOMPARE(M::nint(M::Real(4.8)), M::Real(5));

    QCOMPARE(M::nint(M::Real(5)), M::Real(5));
    QCOMPARE(M::nint(M::Real(5.2)), M::Real(5));
    QCOMPARE(M::nint(M::Real(5.5- 5 * epsilon)), M::Real(5));
    QCOMPARE(M::nint(M::Real(5.5)), M::Real(6));
    QCOMPARE(M::nint(M::Real(5.5 + 5 * epsilon)), M::Real(6));
    QCOMPARE(M::nint(M::Real(5.8)), M::Real(6));

    QCOMPARE(M::nint(M::Real(0)), M::Real(0));

    QCOMPARE(M::nint(M::Real(-4)), M::Real(-4));
    QCOMPARE(M::nint(M::Real(-4.2)), M::Real(-4));
    QCOMPARE(M::nint(M::Real(-4.5 + 4 * epsilon)), M::Real(-4));
    QCOMPARE(M::nint(M::Real(-4.5)), M::Real(-4));
    QCOMPARE(M::nint(M::Real(-4.5 - 4 * epsilon)), M::Real(-5));
    QCOMPARE(M::nint(M::Real(-4.8)), M::Real(-5));

    QCOMPARE(M::nint(M::Real(-5)), M::Real(-5));
    QCOMPARE(M::nint(M::Real(-5.2)), M::Real(-5));
    QCOMPARE(M::nint(M::Real(-5.5 + 5 * epsilon)), M::Real(-5));
    QCOMPARE(M::nint(M::Real(-5.5)), M::Real(-6));
    QCOMPARE(M::nint(M::Real(-5.5 - 5 * epsilon)), M::Real(-6));
    QCOMPARE(M::nint(M::Real(-5.8)), M::Real(-6));

    // Check special cases.
    QCOMPARE(M::nint(+infinity), +infinity);
    QCOMPARE(M::nint(-infinity), -infinity);

    M::Real result = M::nint(NaN);
    QCOMPARE(std::isnan(result), true);
}


void TestBasicFunctions::testRealNIntComplex() {
    QCOMPARE(M::nint(M::Complex(4)), M::Real(4));
    QCOMPARE(M::nint(M::Complex(4.2)), M::Real(4));
    QCOMPARE(M::nint(M::Complex(4.5 - 4 * epsilon)), M::Real(4));
    QCOMPARE(M::nint(M::Complex(4.5)), M::Real(4));
    QCOMPARE(M::nint(M::Complex(4.5 + 4 * epsilon)), M::Real(5));
    QCOMPARE(M::nint(M::Complex(4.8)), M::Real(5));

    QCOMPARE(M::nint(M::Complex(5)), M::Real(5));
    QCOMPARE(M::nint(M::Complex(5.2)), M::Real(5));
    QCOMPARE(M::nint(M::Complex(5.5- 5 * epsilon)), M::Real(5));
    QCOMPARE(M::nint(M::Complex(5.5)), M::Real(6));
    QCOMPARE(M::nint(M::Complex(5.5 + 5 * epsilon)), M::Real(6));
    QCOMPARE(M::nint(M::Complex(5.8)), M::Real(6));

    QCOMPARE(M::nint(M::Complex(0)), M::Real(0));

    QCOMPARE(M::nint(M::Complex(-4)), M::Real(-4));
    QCOMPARE(M::nint(M::Complex(-4.2)), M::Real(-4));
    QCOMPARE(M::nint(M::Complex(-4.5 + 4 * epsilon)), M::Real(-4));
    QCOMPARE(M::nint(M::Complex(-4.5)), M::Real(-4));
    QCOMPARE(M::nint(M::Complex(-4.5 - 4 * epsilon)), M::Real(-5));
    QCOMPARE(M::nint(M::Complex(-4.8)), M::Real(-5));

    QCOMPARE(M::nint(M::Complex(-5)), M::Real(-5));
    QCOMPARE(M::nint(M::Complex(-5.2)), M::Real(-5));
    QCOMPARE(M::nint(M::Complex(-5.5 + 5 * epsilon)), M::Real(-5));
    QCOMPARE(M::nint(M::Complex(-5.5)), M::Real(-6));
    QCOMPARE(M::nint(M::Complex(-5.5 - 5 * epsilon)), M::Real(-6));
    QCOMPARE(M::nint(M::Complex(-5.8)), M::Real(-6));
}


void TestBasicFunctions::testRealNIntVariants() {
    M::Boolean boolean = true;
    M::Integer integer = 2;
    M::Real    real    = 3.0;
    M::Complex complex = M::Complex(4.4);

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);

    QCOMPARE(M::nint(vBoolean), M::nint(boolean));
    QCOMPARE(M::nint(vInteger), M::nint(integer));
    QCOMPARE(M::nint(vReal), M::nint(real));
    QCOMPARE(M::nint(vComplex), M::nint(complex));
}


void TestBasicFunctions::testRealFactorialReal() {
    M::Real expected = M::Real(1);
    M::Real i        = M::Real(0);

    do {
        M::Real actual = M::factorial(i);
        QCOMPARE(actual, expected);

        ++i;
        expected *= i;
    } while (!std::isinf(expected));

    // Check special cases

    M::Real actual = M::factorial(-1.0);
    QCOMPARE(std::isnan(actual), true);

    actual = M::factorial(1.5);
    QCOMPARE(std::isnan(actual), true);
}


void TestBasicFunctions::testRealFactorialVariants() {
    M::Real expected = M::Real(1);
    M::Real i        = M::Real(0);

    do {
        M::Real actual = M::factorial(M::Variant(i));
        QCOMPARE(actual, expected);

        ++i;
        expected *= i;
    } while (!std::isinf(expected));
}


void TestBasicFunctions::testLnInteger() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> valueGenerator(
        std::numeric_limits<M::Integer>::lowest(),
        std::numeric_limits<M::Integer>::max()
    );

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer value    = valueGenerator(rng);
        M::Complex measured = M::ln(value);

        std::complex<M::Complex::T> expected      = std::log(std::complex<M::Complex::T>(value));
        M::Complex                  error         = measured - M::Complex(expected.real(), expected.imag());
        M::Real                     relativeError = M::abs(error) / std::abs(expected);

        if (relativeError >= 1.0E-15) {
            QCOMPARE(relativeError < 1.0E-15, true);
        }
    }
}


void TestBasicFunctions::testLnReal() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> valueGenerator(-1.0E12, 1.0E-12);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real    value    = valueGenerator(rng);
        M::Complex measured = M::ln(value);

        std::complex<M::Complex::T> expected      = std::log(std::complex<M::Complex::T>(value));
        M::Complex                  error         = measured - M::Complex(expected.real(), expected.imag());
        M::Real                     relativeError = M::abs(error) / std::abs(expected);

        if (relativeError >= 1.0E-15) {
            QCOMPARE(relativeError < 1.0E-15, true);
        }
    }
}


void TestBasicFunctions::testLnComplex() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> valueGenerator(-1.0E12, 1.0E-12);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex value(valueGenerator(rng), valueGenerator(rng));
        M::Complex measured = M::ln(value);

        std::complex<M::Complex::T> expected      = std::log(std::complex<M::Complex::T>(value.real(), value.imag()));
        M::Complex                  error         = measured - M::Complex(expected.real(), expected.imag());
        M::Real                     relativeError = M::abs(error) / std::abs(expected);

        if (relativeError >= 1.0E-15) {
            QCOMPARE(relativeError < 1.0E-15, true);
        }
    }
}


void TestBasicFunctions::testLnVariants() {
    M::Boolean boolean = true;
    M::Integer integer = 2;
    M::Real    real    = 3.0;
    M::Complex complex = M::Complex(4.4, 5.0);

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);

    QCOMPARE(M::ln(vBoolean), M::ln(boolean));
    QCOMPARE(M::ln(vInteger), M::ln(integer));
    QCOMPARE(M::ln(vReal), M::ln(real));
    QCOMPARE(M::ln(vComplex), M::ln(complex));
}


void TestBasicFunctions::testLogIntegerInteger() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> valueGenerator(
        std::numeric_limits<M::Integer>::lowest(),
        std::numeric_limits<M::Integer>::max()
    );

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer value    = valueGenerator(rng);
        M::Integer base     = valueGenerator(rng);
        M::Complex measured = M::log(base, value);

        std::complex<M::Complex::T> expected      = std::log(std::complex<M::Complex::T>(value)) / std::log(base);
        M::Complex                  error         = measured - M::Complex(expected.real(), expected.imag());
        M::Real                     relativeError = M::abs(error) / std::abs(expected);

        if (relativeError >= 1.0E-15) {
            QCOMPARE(relativeError < 1.0E-15, true);
        }
    }
}


void TestBasicFunctions::testLogIntegerReal() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> baseGenerator(
        std::numeric_limits<M::Integer>::lowest(),
        std::numeric_limits<M::Integer>::max()
    );
    std::uniform_real_distribution<M::Real>   valueGenerator(-1.0E12, 1.0E-12);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer base     = baseGenerator(rng);
        M::Real    value    = valueGenerator(rng);
        M::Complex measured = M::log(base, value);

        std::complex<M::Complex::T> expected      = std::log(std::complex<M::Complex::T>(value)) / std::log(base);
        M::Complex                  error         = measured - M::Complex(expected.real(), expected.imag());
        M::Real                     relativeError = M::abs(error) / std::abs(expected);

        if (relativeError >= 1.0E-15) {
            QCOMPARE(relativeError < 1.0E-15, true);
        }
    }
}


void TestBasicFunctions::testLogIntegerComplex() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> baseGenerator(
        std::numeric_limits<M::Integer>::lowest(),
        std::numeric_limits<M::Integer>::max()
    );
    std::uniform_real_distribution<M::Real>   valueGenerator(-1.0E12, 1.0E-12);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer base = baseGenerator(rng);
        M::Complex value(valueGenerator(rng), valueGenerator(rng));
        M::Complex measured = M::log(base, value);

        std::complex<M::Complex::T> expected = (
              std::log(std::complex<M::Complex::T>(value.real(), value.imag()))
            / std::log(base)
        );

        M::Complex error         = measured - M::Complex(expected.real(), expected.imag());
        M::Real    relativeError = M::abs(error) / std::abs(expected);

        if (relativeError >= 1.0E-15) {
            QCOMPARE(relativeError < 1.0E-15, true);
        }
    }
}


void TestBasicFunctions::testLogRealInteger() {
    std::mt19937                              rng(0x12345678);
    std::uniform_real_distribution<M::Real>   baseGenerator(-1.0E12, 1.0E-12);
    std::uniform_int_distribution<M::Integer> valueGenerator(
        std::numeric_limits<M::Integer>::lowest(),
        std::numeric_limits<M::Integer>::max()
    );

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer value    = valueGenerator(rng);
        M::Real    base     = baseGenerator(rng);
        M::Complex measured = M::log(base, value);

        std::complex<M::Complex::T> expected      = std::log(std::complex<M::Complex::T>(value)) / std::log(base);
        M::Complex                  error         = measured - M::Complex(expected.real(), expected.imag());
        M::Real                     relativeError = M::abs(error) / std::abs(expected);

        if (relativeError >= 1.0E-15) {
            QCOMPARE(relativeError < 1.0E-15, true);
        }
    }
}


void TestBasicFunctions::testLogRealReal() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> baseGenerator(-1.0E12, 1.0E-12);
    std::uniform_real_distribution<M::Real> valueGenerator(-1.0E12, 1.0E+12);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real    value    = valueGenerator(rng);
        M::Real    base     = baseGenerator(rng);
        M::Complex measured = M::log(base, value);

        std::complex<M::Complex::T> expected      = std::log(std::complex<M::Complex::T>(value)) / std::log(base);
        M::Complex                  error         = measured - M::Complex(expected.real(), expected.imag());
        M::Real                     relativeError = M::abs(error) / std::abs(expected);

        if (relativeError >= 1.0E-15) {
            QCOMPARE(relativeError < 1.0E-15, true);
        }
    }
}


void TestBasicFunctions::testLogRealComplex() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> baseGenerator(-1.0E12, 1.0E-12);
    std::uniform_real_distribution<M::Real> valueGenerator(-1.0E12, 1.0E+12);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex value(valueGenerator(rng), valueGenerator(rng));
        M::Real    base     = baseGenerator(rng);
        M::Complex measured = M::log(base, value);

        std::complex<M::Complex::T> expected = (
              std::log(std::complex<M::Complex::T>(value.real(), value.imag()))
            / std::log(base)
        );

        M::Complex error         = measured - M::Complex(expected.real(), expected.imag());
        M::Real    relativeError = M::abs(error) / std::abs(expected);

        if (relativeError >= 1.0E-15) {
            QCOMPARE(relativeError < 1.0E-15, true);
        }
    }
}


void TestBasicFunctions::testLogComplexInteger() {
    std::mt19937                              rng(0x12345678);
    std::uniform_real_distribution<M::Real>   baseGenerator(-1.0E12, 1.0E-12);
    std::uniform_int_distribution<M::Integer> valueGenerator(
        std::numeric_limits<M::Integer>::lowest(),
        std::numeric_limits<M::Integer>::max()
    );

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex base(baseGenerator(rng), baseGenerator(rng));
        M::Integer value    = valueGenerator(rng);
        M::Complex measured = M::log(base, value);

        std::complex<M::Complex::T> expected = (
              std::log(std::complex<M::Complex::T>(value))
            / std::log(std::complex<M::Complex::T>(base.real(), base.imag()))
        );

        M::Complex error         = measured - M::Complex(expected.real(), expected.imag());
        M::Real    relativeError = M::abs(error) / std::abs(expected);

        if (relativeError >= 1.0E-15) {
            QCOMPARE(relativeError < 1.0E-15, true);
        }
    }
}


void TestBasicFunctions::testLogComplexReal() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> baseGenerator(-1.0E12, 1.0E-12);
    std::uniform_real_distribution<M::Real> valueGenerator(-1.0E12, 1.0E12);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex base(baseGenerator(rng), baseGenerator(rng));
        M::Real    value    = valueGenerator(rng);
        M::Complex measured = M::log(base, value);

        std::complex<M::Complex::T> expected = (
              std::log(std::complex<M::Complex::T>(value))
            / std::log(std::complex<M::Complex::T>(base.real(), base.imag()))
        );

        M::Complex error         = measured - M::Complex(expected.real(), expected.imag());
        M::Real    relativeError = M::abs(error) / std::abs(expected);

        if (relativeError >= 1.0E-15) {
            QCOMPARE(relativeError < 1.0E-15, true);
        }
    }
}


void TestBasicFunctions::testLogComplexComplex() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> baseGenerator(-1.0E12, 1.0E-12);
    std::uniform_real_distribution<M::Real> valueGenerator(-1.0E12, 1.0E12);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex base(baseGenerator(rng), baseGenerator(rng));
        M::Complex value(valueGenerator(rng), valueGenerator(rng));
        M::Complex measured = M::log(base, value);

        std::complex<M::Complex::T> expected = (
              std::log(std::complex<M::Complex::T>(value.real(), value.imag()))
            / std::log(std::complex<M::Complex::T>(base.real(), base.imag()))
        );

        M::Complex error         = measured - M::Complex(expected.real(), expected.imag());
        M::Real    relativeError = M::abs(error) / std::abs(expected);

        if (relativeError >= 1.0E-15) {
            QCOMPARE(relativeError < 1.0E-15, true);
        }
    }
}


void TestBasicFunctions::testLogVariants() {
    M::Boolean boolean = true;
    M::Integer integer = 2;
    M::Real    real    = 3.0;
    M::Complex complex = M::Complex(4.0, 5.0);

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);

    QCOMPARE(M::log(vInteger, boolean), M::log(integer, boolean));
    QCOMPARE(M::log(vInteger, integer), M::log(integer, integer));
    QCOMPARE(M::log(vInteger, real), M::log(integer, real));
    QCOMPARE(M::log(vInteger, complex), M::log(integer, complex));

    QCOMPARE(M::log(vReal, boolean), M::log(real, boolean));
    QCOMPARE(M::log(vReal, integer), M::log(real, integer));
    QCOMPARE(M::log(vReal, real), M::log(real, real));
    QCOMPARE(M::log(vReal, complex), M::log(real, complex));

    QCOMPARE(M::log(vComplex, boolean), M::log(complex, boolean));
    QCOMPARE(M::log(vComplex, integer), M::log(complex, integer));
    QCOMPARE(M::log(vComplex, real), M::log(complex, real));
    QCOMPARE(M::log(vComplex, complex), M::log(complex, complex));

    QCOMPARE(M::log(integer, vBoolean), M::log(integer, boolean));
    QCOMPARE(M::log(integer, vInteger), M::log(integer, integer));
    QCOMPARE(M::log(integer, vReal), M::log(integer, real));
    QCOMPARE(M::log(integer, vComplex), M::log(integer, complex));

    QCOMPARE(M::log(real, vBoolean), M::log(real, boolean));
    QCOMPARE(M::log(real, vInteger), M::log(real, integer));
    QCOMPARE(M::log(real, vReal), M::log(real, real));
    QCOMPARE(M::log(real, vComplex), M::log(real, complex));

    QCOMPARE(M::log(complex, vBoolean), M::log(complex, boolean));
    QCOMPARE(M::log(complex, vInteger), M::log(complex, integer));
    QCOMPARE(M::log(complex, vReal), M::log(complex, real));
    QCOMPARE(M::log(complex, vComplex), M::log(complex, complex));

    QCOMPARE(M::log(vInteger, vBoolean), M::log(integer, boolean));
    QCOMPARE(M::log(vInteger, vInteger), M::log(integer, integer));
    QCOMPARE(M::log(vInteger, vReal), M::log(integer, real));
    QCOMPARE(M::log(vInteger, vComplex), M::log(integer, complex));

    QCOMPARE(M::log(vReal, vBoolean), M::log(real, boolean));
    QCOMPARE(M::log(vReal, vInteger), M::log(real, integer));
    QCOMPARE(M::log(vReal, vReal), M::log(real, real));
    QCOMPARE(M::log(vReal, vComplex), M::log(real, complex));

    QCOMPARE(M::log(vComplex, vBoolean), M::log(complex, boolean));
    QCOMPARE(M::log(vComplex, vInteger), M::log(complex, integer));
    QCOMPARE(M::log(vComplex, vReal), M::log(complex, real));
    QCOMPARE(M::log(vComplex, vComplex), M::log(complex, complex));
}


void TestBasicFunctions::testGammaInteger() {}


void TestBasicFunctions::testGammaReal() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> valueGenerator(-1.0E2, 1.0E2);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real value         = valueGenerator(rng);
        M::Real measured      = M::gamma(value);
        M::Real expected      = boost::math::tgamma(value);
        M::Real relativeError = std::abs(measured - expected) / std::abs(expected);

        if (relativeError >= 2.0E-11) {
            QCOMPARE(relativeError < 2.0E-11, true);
        }
    }
}


void TestBasicFunctions::testGammaComplex() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> valueGenerator(-1.0E2, 1.0E2);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real    value         = valueGenerator(rng);
        M::Complex measured      = M::gamma(M::Complex(value));
        M::Real    expected      = boost::math::tgamma(value);

        QCOMPARE(measured.imag(), 0.0); // Must be closed under real values.

        M::Real    relativeError = std::abs(measured.real() - expected) / std::abs(expected);
        if (relativeError >= 2.0E-11) {
            QCOMPARE(relativeError < 2.0E-11, true);
        }
    }
    M::Real maximumRelativeError = 0.0;

    for (unsigned i=0 ; i<numberComplexGammaTestIterations ; ++i) {
        M::Complex value;

        // Euler's definition doesn't work for non-positive integers so we avoid those in this test.
        do {
            value = M::Complex(valueGenerator(rng), valueGenerator(rng));
        } while (value.imag() == 0 && value.real() <= 0);

        M::Complex measured = M::gamma(value);

        // Estimate the value by Euler's definition as an infinite product.
        M::Complex expected = 1.0 / value;
        for (unsigned n=1 ; n<1000000 ; ++n) {
            M::Complex eulerTerm = M::pow(1.0 + (1.0 / n), value) / (1.0 + (value / n));
            expected *= eulerTerm;
        }

        if (M::abs(measured) > 0.01) {
            M::Real relativeError = M::abs(measured - expected) / M::abs(expected);

            if (relativeError >= 0.05) {
                QCOMPARE(relativeError < 0.05, true);
            }

            if (relativeError > maximumRelativeError) {
                maximumRelativeError = relativeError;
            }
        } else {
            M::Real absoluteError = M::abs(measured - expected);

            if (absoluteError >= 5E-5) {
                QCOMPARE(absoluteError < 5.0E-5, true);
            }
        }
    }
}


void TestBasicFunctions::testGammaVariants() {
    M::Boolean boolean = true;
    M::Integer integer = 2;
    M::Real    real    = 3.0;
    M::Complex complex = M::Complex(4.0, 5.0);

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);

    QCOMPARE(M::gamma(vBoolean), M::gamma(boolean));
    QCOMPARE(M::gamma(vInteger), M::gamma(integer));
    QCOMPARE(M::gamma(vReal), M::gamma(real));
    QCOMPARE(M::gamma(vComplex), M::gamma(complex));

    QCOMPARE(M::gamma(vInteger, boolean), M::gamma(integer, boolean));
    QCOMPARE(M::gamma(vInteger, integer), M::gamma(integer, integer));
    QCOMPARE(M::gamma(vInteger, real), M::gamma(integer, real));
    QCOMPARE(M::gamma(vInteger, complex), M::gamma(integer, complex));

    QCOMPARE(M::gamma(vReal, boolean), M::gamma(real, boolean));
    QCOMPARE(M::gamma(vReal, integer), M::gamma(real, integer));
    QCOMPARE(M::gamma(vReal, real), M::gamma(real, real));
    QCOMPARE(M::gamma(vReal, complex), M::gamma(real, complex));

    QCOMPARE(M::gamma(vComplex, boolean), M::gamma(complex, boolean));
    QCOMPARE(M::gamma(vComplex, integer), M::gamma(complex, integer));
    QCOMPARE(M::gamma(vComplex, real), M::gamma(complex, real));
    QCOMPARE(M::gamma(vComplex, complex), M::gamma(complex, complex));

    QCOMPARE(M::gamma(integer, vBoolean), M::gamma(integer, boolean));
    QCOMPARE(M::gamma(integer, vInteger), M::gamma(integer, integer));
    QCOMPARE(M::gamma(integer, vReal), M::gamma(integer, real));
    QCOMPARE(M::gamma(integer, vComplex), M::gamma(integer, complex));

    QCOMPARE(M::gamma(real, vBoolean), M::gamma(real, boolean));
    QCOMPARE(M::gamma(real, vInteger), M::gamma(real, integer));
    QCOMPARE(M::gamma(real, vReal), M::gamma(real, real));
    QCOMPARE(M::gamma(real, vComplex), M::gamma(real, complex));

    QCOMPARE(M::gamma(complex, vBoolean), M::gamma(complex, boolean));
    QCOMPARE(M::gamma(complex, vInteger), M::gamma(complex, integer));
    QCOMPARE(M::gamma(complex, vReal), M::gamma(complex, real));
    QCOMPARE(M::gamma(complex, vComplex), M::gamma(complex, complex));

    QCOMPARE(M::gamma(vInteger, vBoolean), M::gamma(integer, boolean));
    QCOMPARE(M::gamma(vInteger, vInteger), M::gamma(integer, integer));
    QCOMPARE(M::gamma(vInteger, vReal), M::gamma(integer, real));
    QCOMPARE(M::gamma(vInteger, vComplex), M::gamma(integer, complex));

    QCOMPARE(M::gamma(vReal, vBoolean), M::gamma(real, boolean));
    QCOMPARE(M::gamma(vReal, vInteger), M::gamma(real, integer));
    QCOMPARE(M::gamma(vReal, vReal), M::gamma(real, real));
    QCOMPARE(M::gamma(vReal, vComplex), M::gamma(real, complex));

    QCOMPARE(M::gamma(vComplex, vBoolean), M::gamma(complex, boolean));
    QCOMPARE(M::gamma(vComplex, vInteger), M::gamma(complex, integer));
    QCOMPARE(M::gamma(vComplex, vReal), M::gamma(complex, real));
    QCOMPARE(M::gamma(vComplex, vComplex), M::gamma(complex, complex));
}


void TestBasicFunctions::testLnFactorialInteger() {
    unsigned                                  maximumValue = 10000000;
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> valueGenerator(0, maximumValue);

    QList<M::Real> logFactorialValues;
    logFactorialValues << 0 << 0;

    M::Real sum = 0;
    for (unsigned i=2 ; i<=maximumValue ; ++i) {
        sum += std::log(static_cast<M::Real>(i));
        logFactorialValues << sum;
    }

    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer value    = valueGenerator(rng);
        M::Real    measured = M::lnFactorial(value);

        M::Real expected      = logFactorialValues.at(value);
        M::Real error         = measured - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        QCOMPARE(relativeError < 1.0E-13, true);

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }
    }

    QCOMPARE(std::isnan(M::lnFactorial(static_cast<M::Integer>(-1))), true);
}


void TestBasicFunctions::testLnFactorialReal() {
    unsigned                                  maximumValue = 10000000;
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> valueGenerator(0, maximumValue);

    QList<M::Real> logFactorialValues;
    logFactorialValues << 0 << 0;

    M::Real sum = 0;
    for (unsigned i=2 ; i<=maximumValue ; ++i) {
        sum += std::log(static_cast<M::Real>(i));
        logFactorialValues << sum;
    }

    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer value    = valueGenerator(rng);
        M::Real    measured = M::lnFactorial(static_cast<M::Real>(value));

        M::Real expected      = logFactorialValues.at(value);
        M::Real error         = measured - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        QCOMPARE(relativeError < 1.0E-13, true);

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }
    }

    QCOMPARE(std::isnan(M::lnFactorial(-1.0)), true);
    QCOMPARE(std::isnan(M::lnFactorial(1.5)), true);
}


void TestBasicFunctions::testLnFactorialComplex() {
    unsigned                                  maximumValue = 10000000;
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> valueGenerator(0, maximumValue);

    QList<M::Real> logFactorialValues;
    logFactorialValues << 0 << 0;

    M::Real sum = 0;
    for (unsigned i=2 ; i<=maximumValue ; ++i) {
        sum += std::log(static_cast<M::Real>(i));
        logFactorialValues << sum;
    }

    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer value    = valueGenerator(rng);
        M::Real    measured = M::lnFactorial(M::Complex(value));

        M::Real expected      = logFactorialValues.at(value);
        M::Real error         = measured - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        QCOMPARE(relativeError < 1.0E-13, true);

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }
    }

    QCOMPARE(std::isnan(M::lnFactorial(M::Complex(-1.0))), true);
    QCOMPARE(std::isnan(M::lnFactorial(M::Complex(1.5))), true);
    QCOMPARE(std::isnan(M::lnFactorial(M::Complex(1.0, 1.0))), true);
}


void TestBasicFunctions::testLnFactorialVariants() {
    M::Boolean boolean = true;
    M::Integer integer = 2;
    M::Real    real    = 3.0;
    M::Complex complex = M::Complex(4.0, 5.0);

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);

    QCOMPARE(M::gamma(vBoolean), M::gamma(boolean));
    QCOMPARE(M::gamma(vInteger), M::gamma(integer));
    QCOMPARE(M::gamma(vReal), M::gamma(real));
    QCOMPARE(M::gamma(vComplex), M::gamma(complex));

}


void TestBasicFunctions::testLogFactorialIntegerInteger() {
    unsigned                                  maximumValue = 10000000;
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> valueGenerator(0, maximumValue);
    std::uniform_int_distribution<M::Integer> baseGenerator(
        std::numeric_limits<M::Integer>::lowest(),
        std::numeric_limits<M::Integer>::max()
    );

    QList<M::Real> logFactorialValues;
    logFactorialValues << 0 << 0;

    M::Real sum = 0;
    for (unsigned i=2 ; i<=maximumValue ; ++i) {
        sum += std::log(static_cast<M::Real>(i));
        logFactorialValues << sum;
    }

    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer base     = baseGenerator(rng);
        M::Integer value    = valueGenerator(rng);
        M::Complex measured = M::logFactorial(base, value);

        M::Complex expected      = logFactorialValues.at(value) / M::ln(base);
        M::Complex error         = measured - expected;
        M::Real    relativeError = M::abs(error) / M::abs(expected);

        QCOMPARE(relativeError < 1.0E-13, true);

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }
    }
}


void TestBasicFunctions::testLogFactorialIntegerReal() {
    unsigned                                  maximumValue = 10000000;
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> valueGenerator(0, maximumValue);
    std::uniform_int_distribution<M::Integer> baseGenerator(
        std::numeric_limits<M::Integer>::lowest(),
        std::numeric_limits<M::Integer>::max()
    );

    QList<M::Real> logFactorialValues;
    logFactorialValues << 0 << 0;

    M::Real sum = 0;
    for (unsigned i=2 ; i<=maximumValue ; ++i) {
        sum += std::log(static_cast<M::Real>(i));
        logFactorialValues << sum;
    }

    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer base     = baseGenerator(rng);
        M::Integer value    = valueGenerator(rng);
        M::Complex measured = M::logFactorial(base, static_cast<M::Real>(value));

        M::Complex expected      = logFactorialValues.at(value) / M::ln(base);
        M::Complex error         = measured - expected;
        M::Real    relativeError = M::abs(error) / M::abs(expected);

        QCOMPARE(relativeError < 1.0E-13, true);

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }
    }
}


void TestBasicFunctions::testLogFactorialIntegerComplex() {
    unsigned                                  maximumValue = 10000000;
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> valueGenerator(0, maximumValue);
    std::uniform_int_distribution<M::Integer> baseGenerator(
        std::numeric_limits<M::Integer>::lowest(),
        std::numeric_limits<M::Integer>::max()
    );

    QList<M::Real> logFactorialValues;
    logFactorialValues << 0 << 0;

    M::Real sum = 0;
    for (unsigned i=2 ; i<=maximumValue ; ++i) {
        sum += std::log(static_cast<M::Real>(i));
        logFactorialValues << sum;
    }

    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer base     = baseGenerator(rng);
        M::Integer value    = valueGenerator(rng);
        M::Complex measured = M::logFactorial(base, M::Complex(value));

        M::Complex expected      = logFactorialValues.at(value) / M::ln(base);
        M::Complex error         = measured - expected;
        M::Real    relativeError = M::abs(error) / M::abs(expected);

        QCOMPARE(relativeError < 1.0E-13, true);

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }
    }
}


void TestBasicFunctions::testLogFactorialRealInteger() {
    unsigned                                  maximumValue = 10000000;
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> valueGenerator(0, maximumValue);
    std::uniform_real_distribution<M::Real>   baseGenerator(-1.0E12, +1.0E12);

    QList<M::Real> logFactorialValues;
    logFactorialValues << 0 << 0;

    M::Real sum = 0;
    for (unsigned i=2 ; i<=maximumValue ; ++i) {
        sum += std::log(static_cast<M::Real>(i));
        logFactorialValues << sum;
    }

    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real    base     = baseGenerator(rng);
        M::Integer value    = valueGenerator(rng);
        M::Complex measured = M::logFactorial(base, value);

        M::Complex expected      = logFactorialValues.at(value) / M::ln(base);
        M::Complex error         = measured - expected;
        M::Real    relativeError = M::abs(error) / M::abs(expected);

        QCOMPARE(relativeError < 1.0E-13, true);

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }
    }
}


void TestBasicFunctions::testLogFactorialRealReal() {
    unsigned                                  maximumValue = 10000000;
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> valueGenerator(0, maximumValue);
    std::uniform_real_distribution<M::Real>   baseGenerator(-1.0E12, +1.0E12);

    QList<M::Real> logFactorialValues;
    logFactorialValues << 0 << 0;

    M::Real sum = 0;
    for (unsigned i=2 ; i<=maximumValue ; ++i) {
        sum += std::log(static_cast<M::Real>(i));
        logFactorialValues << sum;
    }

    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real    base     = baseGenerator(rng);
        M::Integer value    = valueGenerator(rng);
        M::Complex measured = M::logFactorial(base, static_cast<M::Real>(value));

        M::Complex expected      = logFactorialValues.at(value) / M::ln(base);
        M::Complex error         = measured - expected;
        M::Real    relativeError = M::abs(error) / M::abs(expected);

        QCOMPARE(relativeError < 1.0E-13, true);

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }
    }
}


void TestBasicFunctions::testLogFactorialRealComplex() {
    unsigned                                  maximumValue = 10000000;
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> valueGenerator(0, maximumValue);
    std::uniform_real_distribution<M::Real>   baseGenerator(-1.0E12, +1.0E12);

    QList<M::Real> logFactorialValues;
    logFactorialValues << 0 << 0;

    M::Real sum = 0;
    for (unsigned i=2 ; i<=maximumValue ; ++i) {
        sum += std::log(static_cast<M::Real>(i));
        logFactorialValues << sum;
    }

    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real    base     = baseGenerator(rng);
        M::Integer value    = valueGenerator(rng);
        M::Complex measured = M::logFactorial(base, M::Complex(value));

        M::Complex expected      = logFactorialValues.at(value) / M::ln(base);
        M::Complex error         = measured - expected;
        M::Real    relativeError = M::abs(error) / M::abs(expected);

        QCOMPARE(relativeError < 1.0E-13, true);

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }
    }
}


void TestBasicFunctions::testLogFactorialComplexInteger() {
    unsigned                                  maximumValue = 10000000;
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> valueGenerator(0, maximumValue);
    std::uniform_real_distribution<M::Real>   baseGenerator(-1.0E12, +1.0E12);

    QList<M::Real> logFactorialValues;
    logFactorialValues << 0 << 0;

    M::Real sum = 0;
    for (unsigned i=2 ; i<=maximumValue ; ++i) {
        sum += std::log(static_cast<M::Real>(i));
        logFactorialValues << sum;
    }

    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex base(baseGenerator(rng), baseGenerator(rng));
        M::Integer value    = valueGenerator(rng);
        M::Complex measured = M::logFactorial(base, value);

        M::Complex expected      = logFactorialValues.at(value) / M::ln(base);
        M::Complex error         = measured - expected;
        M::Real    relativeError = M::abs(error) / M::abs(expected);

        QCOMPARE(relativeError < 1.0E-13, true);

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }
    }
}


void TestBasicFunctions::testLogFactorialComplexReal() {
    unsigned                                  maximumValue = 10000000;
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> valueGenerator(0, maximumValue);
    std::uniform_real_distribution<M::Real>   baseGenerator(-1.0E12, +1.0E12);

    QList<M::Real> logFactorialValues;
    logFactorialValues << 0 << 0;

    M::Real sum = 0;
    for (unsigned i=2 ; i<=maximumValue ; ++i) {
        sum += std::log(static_cast<M::Real>(i));
        logFactorialValues << sum;
    }

    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex base(baseGenerator(rng), baseGenerator(rng));
        M::Integer value    = valueGenerator(rng);
        M::Complex measured = M::logFactorial(base, static_cast<M::Real>(value));

        M::Complex expected      = logFactorialValues.at(value) / M::ln(base);
        M::Complex error         = measured - expected;
        M::Real    relativeError = M::abs(error) / M::abs(expected);

        QCOMPARE(relativeError < 1.0E-13, true);

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }
    }
}


void TestBasicFunctions::testLogFactorialComplexComplex() {
    unsigned                                  maximumValue = 10000000;
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> valueGenerator(0, maximumValue);
    std::uniform_real_distribution<M::Real>   baseGenerator(-1.0E12, +1.0E12);

    QList<M::Real> logFactorialValues;
    logFactorialValues << 0 << 0;

    M::Real sum = 0;
    for (unsigned i=2 ; i<=maximumValue ; ++i) {
        sum += std::log(static_cast<M::Real>(i));
        logFactorialValues << sum;
    }

    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex base(baseGenerator(rng), baseGenerator(rng));
        M::Integer value    = valueGenerator(rng);
        M::Complex measured = M::logFactorial(base, M::Complex(value));

        M::Complex expected      = logFactorialValues.at(value) / M::ln(base);
        M::Complex error         = measured - expected;
        M::Real    relativeError = M::abs(error) / M::abs(expected);

        QCOMPARE(relativeError < 1.0E-13, true);

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }
    }
}


void TestBasicFunctions::testLogFactorialVariants() {
    M::Boolean boolean = true;
    M::Integer integer = 2;
    M::Real    real    = 3.0;
    M::Complex complex = M::Complex(4.0, 5.0);

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);

    QCOMPARE(M::logFactorial(vInteger, boolean), M::logFactorial(integer, boolean));
    QCOMPARE(M::logFactorial(vInteger, integer), M::logFactorial(integer, integer));
    QCOMPARE(M::logFactorial(vInteger, real), M::logFactorial(integer, real));
    QCOMPARE(M::logFactorial(vInteger, complex), M::logFactorial(integer, complex));

    QCOMPARE(M::logFactorial(vReal, boolean), M::logFactorial(real, boolean));
    QCOMPARE(M::logFactorial(vReal, integer), M::logFactorial(real, integer));
    QCOMPARE(M::logFactorial(vReal, real), M::logFactorial(real, real));
    QCOMPARE(M::logFactorial(vReal, complex), M::logFactorial(real, complex));

    QCOMPARE(M::logFactorial(vComplex, boolean), M::logFactorial(complex, boolean));
    QCOMPARE(M::logFactorial(vComplex, integer), M::logFactorial(complex, integer));
    QCOMPARE(M::logFactorial(vComplex, real), M::logFactorial(complex, real));
    QCOMPARE(M::logFactorial(vComplex, complex), M::logFactorial(complex, complex));

    QCOMPARE(M::logFactorial(integer, vBoolean), M::logFactorial(integer, boolean));
    QCOMPARE(M::logFactorial(integer, vInteger), M::logFactorial(integer, integer));
    QCOMPARE(M::logFactorial(integer, vReal), M::logFactorial(integer, real));
    QCOMPARE(M::logFactorial(integer, vComplex), M::logFactorial(integer, complex));

    QCOMPARE(M::logFactorial(real, vBoolean), M::logFactorial(real, boolean));
    QCOMPARE(M::logFactorial(real, vInteger), M::logFactorial(real, integer));
    QCOMPARE(M::logFactorial(real, vReal), M::logFactorial(real, real));
    QCOMPARE(M::logFactorial(real, vComplex), M::logFactorial(real, complex));

    QCOMPARE(M::logFactorial(complex, vBoolean), M::logFactorial(complex, boolean));
    QCOMPARE(M::logFactorial(complex, vInteger), M::logFactorial(complex, integer));
    QCOMPARE(M::logFactorial(complex, vReal), M::logFactorial(complex, real));
    QCOMPARE(M::logFactorial(complex, vComplex), M::logFactorial(complex, complex));

    QCOMPARE(M::logFactorial(vInteger, vBoolean), M::logFactorial(integer, boolean));
    QCOMPARE(M::logFactorial(vInteger, vInteger), M::logFactorial(integer, integer));
    QCOMPARE(M::logFactorial(vInteger, vReal), M::logFactorial(integer, real));
    QCOMPARE(M::logFactorial(vInteger, vComplex), M::logFactorial(integer, complex));

    QCOMPARE(M::logFactorial(vReal, vBoolean), M::logFactorial(real, boolean));
    QCOMPARE(M::logFactorial(vReal, vInteger), M::logFactorial(real, integer));
    QCOMPARE(M::logFactorial(vReal, vReal), M::logFactorial(real, real));
    QCOMPARE(M::logFactorial(vReal, vComplex), M::logFactorial(real, complex));

    QCOMPARE(M::logFactorial(vComplex, vBoolean), M::logFactorial(complex, boolean));
    QCOMPARE(M::logFactorial(vComplex, vInteger), M::logFactorial(complex, integer));
    QCOMPARE(M::logFactorial(vComplex, vReal), M::logFactorial(complex, real));
    QCOMPARE(M::logFactorial(vComplex, vComplex), M::logFactorial(complex, complex));
}


void TestBasicFunctions::testLnGammaInteger() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> valueGenerator(-10000, 10000);

    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer value;
        do {
            value = valueGenerator(rng);
        } while (value == 0);

        M::Real measured = M::lnGamma(value);
        M::Real expected = M::lnGamma(static_cast<M::Real>(value));
        M::Real error    = measured - expected;

        if (measured > 0.01) {
            M::Real relativeError = M::abs(error) / M::abs(expected);

            if (relativeError >= 1.0E-14) {
                QCOMPARE(error < 1.0E-14 || relativeError < 1.0E-14, true);
            }

            if (relativeError > maximumError) {
                maximumError = relativeError;
            }
        } else {
            if (error >= 1.0E-15) {
                QCOMPARE(error < 1.0E-15, true);
            }
        }
    }
}


void TestBasicFunctions::testLnGammaReal() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> valueGenerator(-10000, 10000);

    M::Real maximumError = 0;
    M::Real sumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real value;
        // log gamma should be NaN for values less than 0 who's whole part is even.  We must trap that here so that the
        // results can converge.

        signed wholePart;
        do {
            value     = valueGenerator(rng);
            wholePart = static_cast<signed>(value);
        } while (value <= 0 && ((-wholePart) & 1) == 0);

        M::Real measured      = M::lnGamma(value);
        M::Real expected      = boost::math::lgamma(value); // This calculates log of magnitude gamma.
        M::Real error         = measured - expected;
        M::Real relativeError = M::abs(error) / M::abs(expected);

        if (relativeError > 2.0E-12) {
            QCOMPARE(relativeError < 2.0E-12, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    (void) averageError;

    // Check NaN cases.
    M::Real result = M::lnGamma(-0.5);
    QCOMPARE(std::isnan(result), true);

    result = M::lnGamma(-1000.5);
    QCOMPARE(std::isnan(result), true);
}


void TestBasicFunctions::testLnGammaComplex() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> realValueGenerator(-10000, 10000);
    std::uniform_real_distribution<M::Real> complexValueGenerator(-100, 100);

    M::Real maximumError = 0;
    M::Real sumError     = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real value;
        // log gamma should be NaN for values less than 0 who's whole part is even.  We must trap that here so that the
        // results can converge.

        signed wholePart;
        do {
            value     = realValueGenerator(rng);
            wholePart = static_cast<signed>(value);
        } while (value <= 0 && ((-wholePart) & 1) == 0);

        M::Complex measured      = M::lnGamma(M::Complex(value));
        M::Real    expected      = boost::math::lgamma(value); // This calculates log of magnitude gamma.
        M::Real    error         = measured.real() - expected;
        M::Real    relativeError = M::abs(error) / M::abs(expected);

        if (relativeError > 2.0E-12) {
            QCOMPARE(relativeError < 2.0E-12, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        if (!std::isnan(relativeError)) {
            sumError += relativeError;
        }
    }

    M::Real averageError = sumError / numberTestIterations;
    (void) averageError;

    M::Real maximumRelativeError = 0;

    for (unsigned i=0 ; i<numberComplexGammaTestIterations ; ++i) {
        M::Complex value;

        // Euler's definition doesn't work for non-positive integers so we avoid those in this test.
        do {
            value = M::Complex(complexValueGenerator(rng), complexValueGenerator(rng));
        } while (value.imag() == 0 && value.real() <= 0);

        M::Complex measured = M::lnGamma(value);

        // Estimate the value by Euler's definition as an infinite product.
        M::Complex expected = 1.0 / value;
        for (unsigned n=1 ; n<1000000 ; ++n) {
            M::Complex eulerTerm = M::pow(1.0 + (1.0 / n), value) / (1.0 + (value / n));
            expected *= eulerTerm;
        }

        M::Complex lnExpected = M::ln(expected);

        if (M::abs(measured) > 0.01) {
            M::Real relativeError = M::abs(measured - lnExpected) / M::abs(lnExpected);

            if (relativeError >= 0.05) {
                QCOMPARE(relativeError < 0.05, true);
            }

            if (relativeError > maximumRelativeError) {
                maximumRelativeError = relativeError;
            }
        } else {
            M::Real absoluteError = M::abs(measured - lnExpected);

            if (absoluteError >= 5E-5) {
                QCOMPARE(absoluteError < 5.0E-5, true);
            }
        }
    }

    // Check NaN cases.
    M::Real result = M::lnGamma(-0.5);
    QCOMPARE(std::isnan(result), true);

    result = M::lnGamma(-1000.5);
    QCOMPARE(std::isnan(result), true);
}


void TestBasicFunctions::testLnGammaVariants() {
    M::Boolean boolean = true;
    M::Integer integer = 2;
    M::Real    real    = 3.0;
    M::Complex complex = M::Complex(4.0, 5.0);

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);

    QCOMPARE(M::lnGamma(vBoolean), M::lnGamma(boolean));
    QCOMPARE(M::lnGamma(vInteger), M::lnGamma(integer));
    QCOMPARE(M::lnGamma(vReal), M::lnGamma(real));
    QCOMPARE(M::lnGamma(vComplex), M::lnGamma(complex));
}


void TestBasicFunctions::testLogGammaIntegerInteger() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> baseGenerator(1, 20);
    std::uniform_int_distribution<M::Integer> valueGenerator(1, 10000);

    M::Real maximumError = 0;
    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer base  = baseGenerator(rng);
        M::Integer value = valueGenerator(rng);

        M::Complex measured      = M::logGamma(base, value);
        M::Complex expected      = boost::math::lgamma(static_cast<double>(value)) / M::ln(M::Complex(base));
        M::Complex error         = measured - expected;
        M::Real    relativeError = M::abs(error) / M::abs(expected);

        if (relativeError > 2.0E-12) {
            QCOMPARE(relativeError < 2.0E-12, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }
    }
}


void TestBasicFunctions::testLogGammaIntegerReal() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> baseGenerator(1, 20);
    std::uniform_real_distribution<M::Real>   valueGenerator(-10000, 10000);

    M::Real maximumError = 0;
    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer base  = baseGenerator(rng);

        M::Real value;
        // log gamma should be NaN for values less than 0 who's whole part is even.  We must trap that here so that the
        // results can converge.

        signed wholePart;
        do {
            value     = valueGenerator(rng);
            wholePart = static_cast<signed>(value);
        } while (value <= 0 && ((-wholePart) & 1) == 0);

        M::Complex measured      = M::logGamma(base, value);
        M::Complex expected      = boost::math::lgamma(static_cast<double>(value)) / M::ln(M::Complex(base));
        M::Complex error         = measured - expected;
        M::Real    relativeError = M::abs(error) / M::abs(expected);

        if (relativeError > 2.0E-12) {
            QCOMPARE(relativeError < 2.0E-12, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }
    }
}


void TestBasicFunctions::testLogGammaIntegerComplex() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> baseGenerator(1, 20);
    std::uniform_real_distribution<M::Real>   valueGenerator(-10000, 10000);

    M::Real maximumError = 0;
    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer base  = baseGenerator(rng);

        M::Real value;
        // log gamma should be NaN for values less than 0 who's whole part is even.  We must trap that here so that the
        // results can converge.

        signed wholePart;
        do {
            value     = valueGenerator(rng);
            wholePart = static_cast<signed>(value);
        } while (value <= 0 && ((-wholePart) & 1) == 0);

        M::Complex measured      = M::logGamma(base, M::Complex(value));
        M::Complex expected      = boost::math::lgamma(static_cast<double>(value)) / M::ln(M::Complex(base));
        M::Complex error         = measured.real() - expected;
        M::Real    relativeError = M::abs(error) / M::abs(expected);

        if (relativeError > 2.0E-12) {
            QCOMPARE(relativeError < 2.0E-12, true);
        }

        if (std::abs(measured.imag()) >= std::abs(1.0E-15 * measured.real())) {
            QCOMPARE(std::abs(measured.imag()) < std::abs(1.0E-15 * measured.real()), true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }
    }
}


void TestBasicFunctions::testLogGammaRealInteger() {
    std::mt19937                              rng(0x12345678);
    std::uniform_real_distribution<M::Real>   baseGenerator(1, 20);
    std::uniform_int_distribution<M::Integer> valueGenerator(1, 10000);

    M::Real maximumError = 0;
    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real    base  = baseGenerator(rng);
        M::Integer value = valueGenerator(rng);

        M::Complex measured      = M::logGamma(base, value);
        M::Complex expected      = boost::math::lgamma(static_cast<double>(value)) / M::ln(M::Complex(base));
        M::Complex error         = measured - expected;
        M::Real    relativeError = M::abs(error) / M::abs(expected);

        if (relativeError > 2.0E-12) {
            QCOMPARE(relativeError < 2.0E-12, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }
    }
}


void TestBasicFunctions::testLogGammaRealReal() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> baseGenerator(1, 20);
    std::uniform_real_distribution<M::Real> valueGenerator(-10000, 10000);

    M::Real maximumError = 0;
    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real base  = baseGenerator(rng);

        M::Real value;
        // log gamma should be NaN for values less than 0 who's whole part is even.  We must trap that here so that the
        // results can converge.

        signed wholePart;
        do {
            value     = valueGenerator(rng);
            wholePart = static_cast<signed>(value);
        } while (value <= 0 && ((-wholePart) & 1) == 0);

        M::Complex measured      = M::logGamma(base, value);
        M::Complex expected      = boost::math::lgamma(static_cast<double>(value)) / M::ln(M::Complex(base));
        M::Complex error         = measured - expected;
        M::Real    relativeError = M::abs(error) / M::abs(expected);

        if (relativeError > 5.0E-12) {
            QCOMPARE(relativeError < 5.0E-12, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }
    }
}


void TestBasicFunctions::testLogGammaRealComplex() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> baseGenerator(1, 20);
    std::uniform_real_distribution<M::Real> valueGenerator(-10000, 10000);

    M::Real maximumError = 0;
    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real base = baseGenerator(rng);

        M::Real value;
        // log gamma should be NaN for values less than 0 who's whole part is even.  We must trap that here so that the
        // results can converge.

        signed wholePart;
        do {
            value     = valueGenerator(rng);
            wholePart = static_cast<signed>(value);
        } while (value <= 0 && ((-wholePart) & 1) == 0);

        M::Complex measured      = M::logGamma(base, M::Complex(value));
        M::Complex expected      = boost::math::lgamma(static_cast<double>(value)) / M::ln(M::Complex(base));
        M::Complex error         = measured.real() - expected;
        M::Real    relativeError = M::abs(error) / M::abs(expected);

        if (relativeError > 5.0E-12) {
            QCOMPARE(relativeError < 5.0E-12, true);
        }

        if (std::abs(measured.imag()) >= std::abs(1.0E-15 * measured.real())) {
            QCOMPARE(std::abs(measured.imag()) < std::abs(1.0E-15 * measured.real()), true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }
    }
}


void TestBasicFunctions::testLogGammaComplexInteger() {
    std::mt19937                              rng(0x12345678);
    std::uniform_real_distribution<M::Real>   baseGenerator(-20, 20);
    std::uniform_int_distribution<M::Integer> valueGenerator(1, 10000);

    M::Real maximumError = 0;
    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex base(baseGenerator(rng), baseGenerator(rng));
        M::Integer value = valueGenerator(rng);

        M::Complex measured      = M::logGamma(base, value);
        M::Complex expected      = boost::math::lgamma(static_cast<double>(value)) / M::ln(M::Complex(base));
        M::Complex error         = measured - expected;
        M::Real    relativeError = M::abs(error) / M::abs(expected);

        if (relativeError > 2.0E-12) {
            QCOMPARE(relativeError < 2.0E-12, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }
    }
}


void TestBasicFunctions::testLogGammaComplexReal() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> baseGenerator(1, 20);
    std::uniform_real_distribution<M::Real> valueGenerator(-10000, 10000);

    M::Real maximumError = 0;
    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex base(baseGenerator(rng), baseGenerator(rng));

        M::Real value;
        // log gamma should be NaN for values less than 0 who's whole part is even.  We must trap that here so that the
        // results can converge.

        signed wholePart;
        do {
            value     = valueGenerator(rng);
            wholePart = static_cast<signed>(value);
        } while (value <= 0 && ((-wholePart) & 1) == 0);

        M::Complex measured      = M::logGamma(base, value);
        M::Complex expected      = boost::math::lgamma(static_cast<double>(value)) / M::ln(base);
        M::Complex error         = measured - expected;
        M::Real    relativeError = M::abs(error) / M::abs(expected);

        if (relativeError > 5.0E-12) {
            QCOMPARE(relativeError < 5.0E-12, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }
    }
}


void TestBasicFunctions::testLogGammaComplexComplex() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> baseGenerator(1, 20);
    std::uniform_real_distribution<M::Real> valueGenerator(-10000, 10000);

    M::Real maximumError = 0;
    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex base(baseGenerator(rng), baseGenerator(rng));

        M::Real value;
        // log gamma should be NaN for values less than 0 who's whole part is even.  We must trap that here so that the
        // results can converge.

        signed wholePart;
        do {
            value     = valueGenerator(rng);
            wholePart = static_cast<signed>(value);
        } while (value <= 0 && ((-wholePart) & 1) == 0);

        M::Complex measured      = M::logGamma(base, M::Complex(value));
        M::Complex expected      = boost::math::lgamma(static_cast<double>(value)) / M::ln(base);
        M::Complex error         = measured - expected;
        M::Real    relativeError = M::abs(error) / M::abs(expected);

        if (relativeError > 5.0E-12) {
            QCOMPARE(relativeError < 5.0E-12, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }
    }
}


void TestBasicFunctions::testLogGammaVariants() {
    M::Boolean boolean = true;
    M::Integer integer = 2;
    M::Real    real    = 3.0;
    M::Complex complex = M::Complex(4.0, 5.0);

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);

    QCOMPARE(M::logGamma(vInteger, boolean), M::logGamma(integer, boolean));
    QCOMPARE(M::logGamma(vInteger, integer), M::logGamma(integer, integer));
    QCOMPARE(M::logGamma(vInteger, real), M::logGamma(integer, real));
    QCOMPARE(M::logGamma(vInteger, complex), M::logGamma(integer, complex));

    QCOMPARE(M::logGamma(vReal, boolean), M::logGamma(real, boolean));
    QCOMPARE(M::logGamma(vReal, integer), M::logGamma(real, integer));
    QCOMPARE(M::logGamma(vReal, real), M::logGamma(real, real));
    QCOMPARE(M::logGamma(vReal, complex), M::logGamma(real, complex));

    QCOMPARE(M::logGamma(vComplex, boolean), M::logGamma(complex, boolean));
    QCOMPARE(M::logGamma(vComplex, integer), M::logGamma(complex, integer));
    QCOMPARE(M::logGamma(vComplex, real), M::logGamma(complex, real));
    QCOMPARE(M::logGamma(vComplex, complex), M::logGamma(complex, complex));

    QCOMPARE(M::logGamma(integer, vBoolean), M::logGamma(integer, boolean));
    QCOMPARE(M::logGamma(integer, vInteger), M::logGamma(integer, integer));
    QCOMPARE(M::logGamma(integer, vReal), M::logGamma(integer, real));
    QCOMPARE(M::logGamma(integer, vComplex), M::logGamma(integer, complex));

    QCOMPARE(M::logGamma(real, vBoolean), M::logGamma(real, boolean));
    QCOMPARE(M::logGamma(real, vInteger), M::logGamma(real, integer));
    QCOMPARE(M::logGamma(real, vReal), M::logGamma(real, real));
    QCOMPARE(M::logGamma(real, vComplex), M::logGamma(real, complex));

    QCOMPARE(M::logGamma(complex, vBoolean), M::logGamma(complex, boolean));
    QCOMPARE(M::logGamma(complex, vInteger), M::logGamma(complex, integer));
    QCOMPARE(M::logGamma(complex, vReal), M::logGamma(complex, real));
    QCOMPARE(M::logGamma(complex, vComplex), M::logGamma(complex, complex));

    QCOMPARE(M::logGamma(vInteger, vBoolean), M::logGamma(integer, boolean));
    QCOMPARE(M::logGamma(vInteger, vInteger), M::logGamma(integer, integer));
    QCOMPARE(M::logGamma(vInteger, vReal), M::logGamma(integer, real));
    QCOMPARE(M::logGamma(vInteger, vComplex), M::logGamma(integer, complex));

    QCOMPARE(M::logGamma(vReal, vBoolean), M::logGamma(real, boolean));
    QCOMPARE(M::logGamma(vReal, vInteger), M::logGamma(real, integer));
    QCOMPARE(M::logGamma(vReal, vReal), M::logGamma(real, real));
    QCOMPARE(M::logGamma(vReal, vComplex), M::logGamma(real, complex));

    QCOMPARE(M::logGamma(vComplex, vBoolean), M::logGamma(complex, boolean));
    QCOMPARE(M::logGamma(vComplex, vInteger), M::logGamma(complex, integer));
    QCOMPARE(M::logGamma(vComplex, vReal), M::logGamma(complex, real));
    QCOMPARE(M::logGamma(vComplex, vComplex), M::logGamma(complex, complex));
}


void TestBasicFunctions::testBinomialIntegerInteger() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> nGenerator(1, 1000);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer n = nGenerator(rng);

        std::uniform_int_distribution<M::Integer> kGenerator(1, n);
        M::Integer k = kGenerator(rng);

        M::Real measured      = M::binomial(n, k);
        M::Real expected      = std::exp(M::lnFactorial(n) - M::lnFactorial(n - k) - M::lnFactorial(k));
        M::Real error         = measured - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }
    }

    QCOMPARE(std::isnan(M::binomial(M::Integer(1), M::Integer(2))), true);
}


void TestBasicFunctions::testBinomialIntegerReal() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> nGenerator(1, 1000);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer n = nGenerator(rng);

        std::uniform_int_distribution<M::Integer> kGenerator(1, n);
        M::Integer k = kGenerator(rng);

        M::Real measured      = M::binomial(n, static_cast<M::Real>(k));
        M::Real expected      = std::exp(M::lnFactorial(n) - M::lnFactorial(n - k) - M::lnFactorial(k));
        M::Real error         = measured - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }
    }

    QCOMPARE(std::isnan(M::binomial(M::Integer(1), M::Real(2))), true);
    QCOMPARE(std::isnan(M::binomial(M::Integer(3), M::Real(1.5))), true);
}


void TestBasicFunctions::testBinomialRealInteger() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> nGenerator(1, 1000);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer n = nGenerator(rng);

        std::uniform_int_distribution<M::Integer> kGenerator(1, n);
        M::Integer k = kGenerator(rng);

        M::Real measured      = M::binomial(static_cast<M::Real>(n), k);
        M::Real expected      = std::exp(M::lnFactorial(n) - M::lnFactorial(n - k) - M::lnFactorial(k));
        M::Real error         = measured - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }
    }

    QCOMPARE(std::isnan(M::binomial(M::Real(1), M::Integer(2))), true);
    QCOMPARE(std::isnan(M::binomial(M::Real(3.5), M::Integer(2))), true);
}


void TestBasicFunctions::testBinomialRealReal() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> nGenerator(1, 1000);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer n = nGenerator(rng);

        std::uniform_int_distribution<M::Integer> kGenerator(1, n);
        M::Integer k = kGenerator(rng);

        M::Real measured      = M::binomial(static_cast<M::Real>(n), static_cast<M::Real>(k));
        M::Real expected      = std::exp(M::lnFactorial(n) - M::lnFactorial(n - k) - M::lnFactorial(k));
        M::Real error         = measured - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }
    }

    QCOMPARE(std::isnan(M::binomial(M::Real(1), M::Real(2))), true);
    QCOMPARE(std::isnan(M::binomial(M::Real(3.5), M::Real(2))), true);
}


void TestBasicFunctions::testBinomialIntegerComplex() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> nGenerator(1, 1000);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer n = nGenerator(rng);

        std::uniform_int_distribution<M::Integer> kGenerator(1, n);
        M::Integer k = kGenerator(rng);

        M::Real measured      = M::binomial(n, M::Complex(k));
        M::Real expected      = std::exp(M::lnFactorial(n) - M::lnFactorial(n - k) - M::lnFactorial(k));
        M::Real error         = measured - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }
    }

    QCOMPARE(std::isnan(M::binomial(M::Integer(1), M::Complex(2))), true);
    QCOMPARE(std::isnan(M::binomial(M::Integer(3), M::Complex(1.5))), true);
    QCOMPARE(std::isnan(M::binomial(M::Integer(3), M::Complex(2, 1))), true);
}


void TestBasicFunctions::testBinomialRealComplex() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> nGenerator(1, 1000);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer n = nGenerator(rng);

        std::uniform_int_distribution<M::Integer> kGenerator(1, n);
        M::Integer k = kGenerator(rng);

        M::Real measured      = M::binomial(static_cast<M::Real>(n), M::Complex(k));
        M::Real expected      = std::exp(M::lnFactorial(n) - M::lnFactorial(n - k) - M::lnFactorial(k));
        M::Real error         = measured - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }
    }

    QCOMPARE(std::isnan(M::binomial(M::Real(1), M::Complex(2))), true);
    QCOMPARE(std::isnan(M::binomial(M::Real(3), M::Complex(1.5))), true);
    QCOMPARE(std::isnan(M::binomial(M::Real(3), M::Complex(2, 1))), true);
    QCOMPARE(std::isnan(M::binomial(M::Real(3.5), M::Complex(2))), true);
}


void TestBasicFunctions::testBinomialComplexInteger() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> nGenerator(1, 1000);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer n = nGenerator(rng);

        std::uniform_int_distribution<M::Integer> kGenerator(1, n);
        M::Integer k = kGenerator(rng);

        M::Real measured      = M::binomial(M::Complex(n), k);
        M::Real expected      = std::exp(M::lnFactorial(n) - M::lnFactorial(n - k) - M::lnFactorial(k));
        M::Real error         = measured - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }
    }

    QCOMPARE(std::isnan(M::binomial(M::Complex(1), M::Integer(2))), true);
    QCOMPARE(std::isnan(M::binomial(M::Complex(3.5), M::Integer(2))), true);
    QCOMPARE(std::isnan(M::binomial(M::Complex(3,2), M::Integer(2))), true);
}


void TestBasicFunctions::testBinomialComplexReal() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> nGenerator(1, 1000);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer n = nGenerator(rng);

        std::uniform_int_distribution<M::Integer> kGenerator(1, n);
        M::Integer k = kGenerator(rng);

        M::Real measured      = M::binomial(M::Complex(n), M::Real(k));
        M::Real expected      = std::exp(M::lnFactorial(n) - M::lnFactorial(n - k) - M::lnFactorial(k));
        M::Real error         = measured - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }
    }

    QCOMPARE(std::isnan(M::binomial(M::Complex(1), M::Real(2))), true);
    QCOMPARE(std::isnan(M::binomial(M::Complex(3.5), M::Real(2))), true);
    QCOMPARE(std::isnan(M::binomial(M::Complex(3,2), M::Real(2))), true);
    QCOMPARE(std::isnan(M::binomial(M::Complex(3), M::Real(1.5))), true);
}


void TestBasicFunctions::testBinomialComplexComplex() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> nGenerator(1, 1000);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer n = nGenerator(rng);

        std::uniform_int_distribution<M::Integer> kGenerator(1, n);
        M::Integer k = kGenerator(rng);

        M::Real measured      = M::binomial(M::Complex(n), M::Complex(k));
        M::Real expected      = std::exp(M::lnFactorial(n) - M::lnFactorial(n - k) - M::lnFactorial(k));
        M::Real error         = measured - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }
    }

    QCOMPARE(std::isnan(M::binomial(M::Complex(1), M::Complex(2))), true);
    QCOMPARE(std::isnan(M::binomial(M::Complex(3.5), M::Complex(2))), true);
    QCOMPARE(std::isnan(M::binomial(M::Complex(3,2), M::Complex(2))), true);
    QCOMPARE(std::isnan(M::binomial(M::Complex(3), M::Complex(1.5))), true);
    QCOMPARE(std::isnan(M::binomial(M::Complex(3), M::Complex(2,1))), true);
}


void TestBasicFunctions::testBinomialVariants() {
    M::Boolean boolean = true;
    M::Integer integer = 2;
    M::Real    real    = 3.0;
    M::Complex complex = M::Complex(4.0);

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);

    QCOMPARE(M::binomial(vInteger, boolean), M::binomial(integer, boolean));
    QCOMPARE(M::binomial(vInteger, integer), M::binomial(integer, integer));
    QCOMPARE(M::binomial(vInteger, real), M::binomial(integer, real));
    QCOMPARE(M::binomial(vInteger, complex), M::binomial(integer, complex));

    QCOMPARE(M::binomial(vReal, boolean), M::binomial(real, boolean));
    QCOMPARE(M::binomial(vReal, integer), M::binomial(real, integer));
    QCOMPARE(M::binomial(vReal, real), M::binomial(real, real));
    QCOMPARE(M::binomial(vReal, complex), M::binomial(real, complex));

    QCOMPARE(M::binomial(vComplex, boolean), M::binomial(complex, boolean));
    QCOMPARE(M::binomial(vComplex, integer), M::binomial(complex, integer));
    QCOMPARE(M::binomial(vComplex, real), M::binomial(complex, real));
    QCOMPARE(M::binomial(vComplex, complex), M::binomial(complex, complex));

    QCOMPARE(M::binomial(integer, vBoolean), M::binomial(integer, boolean));
    QCOMPARE(M::binomial(integer, vInteger), M::binomial(integer, integer));
    QCOMPARE(M::binomial(integer, vReal), M::binomial(integer, real));
    QCOMPARE(M::binomial(integer, vComplex), M::binomial(integer, complex));

    QCOMPARE(M::binomial(real, vBoolean), M::binomial(real, boolean));
    QCOMPARE(M::binomial(real, vInteger), M::binomial(real, integer));
    QCOMPARE(M::binomial(real, vReal), M::binomial(real, real));
    QCOMPARE(M::binomial(real, vComplex), M::binomial(real, complex));

    QCOMPARE(M::binomial(complex, vBoolean), M::binomial(complex, boolean));
    QCOMPARE(M::binomial(complex, vInteger), M::binomial(complex, integer));
    QCOMPARE(M::binomial(complex, vReal), M::binomial(complex, real));
    QCOMPARE(M::binomial(complex, vComplex), M::binomial(complex, complex));

    QCOMPARE(M::binomial(vInteger, vBoolean), M::binomial(integer, boolean));
    QCOMPARE(M::binomial(vInteger, vInteger), M::binomial(integer, integer));
    QCOMPARE(M::binomial(vInteger, vReal), M::binomial(integer, real));
    QCOMPARE(M::binomial(vInteger, vComplex), M::binomial(integer, complex));

    QCOMPARE(M::binomial(vReal, vBoolean), M::binomial(real, boolean));
    QCOMPARE(M::binomial(vReal, vInteger), M::binomial(real, integer));
    QCOMPARE(M::binomial(vReal, vReal), M::binomial(real, real));
    QCOMPARE(M::binomial(vReal, vComplex), M::binomial(real, complex));

    QCOMPARE(M::binomial(vComplex, vBoolean), M::binomial(complex, boolean));
    QCOMPARE(M::binomial(vComplex, vInteger), M::binomial(complex, integer));
    QCOMPARE(M::binomial(vComplex, vReal), M::binomial(complex, real));
    QCOMPARE(M::binomial(vComplex, vComplex), M::binomial(complex, complex));
}


void TestBasicFunctions::testLnBinomial() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> nGenerator(1, 1000);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer n = nGenerator(rng);

        std::uniform_int_distribution<M::Integer> kGenerator(1, n);
        M::Integer k = kGenerator(rng);

        M::Real measured      = M::lnBinomial(n, k);
        M::Real expected      = M::lnFactorial(n) - M::lnFactorial(n - k) - M::lnFactorial(k);
        M::Real error         = measured - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        if (relativeError >= 2.0E-14) {
            QCOMPARE(relativeError < 2.0E-14, true);
        }
    }
}


void TestBasicFunctions::testLnBinomialVariants() {
    M::Boolean boolean = true;
    M::Integer integer = 2;
    M::Real    real    = 3.0;
    M::Complex complex = M::Complex(4.0);

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);

    QCOMPARE(M::lnBinomial(vInteger, boolean), M::lnBinomial(integer, boolean));
    QCOMPARE(M::lnBinomial(vInteger, integer), M::lnBinomial(integer, integer));
    QCOMPARE(M::lnBinomial(vInteger, real), M::lnBinomial(integer, real));
    QCOMPARE(M::lnBinomial(vInteger, complex), M::lnBinomial(integer, complex));

    QCOMPARE(M::lnBinomial(vReal, boolean), M::lnBinomial(real, boolean));
    QCOMPARE(M::lnBinomial(vReal, integer), M::lnBinomial(real, integer));
    QCOMPARE(M::lnBinomial(vReal, real), M::lnBinomial(real, real));
    QCOMPARE(M::lnBinomial(vReal, complex), M::lnBinomial(real, complex));

    QCOMPARE(M::lnBinomial(vComplex, boolean), M::lnBinomial(complex, boolean));
    QCOMPARE(M::lnBinomial(vComplex, integer), M::lnBinomial(complex, integer));
    QCOMPARE(M::lnBinomial(vComplex, real), M::lnBinomial(complex, real));
    QCOMPARE(M::lnBinomial(vComplex, complex), M::lnBinomial(complex, complex));

    QCOMPARE(M::lnBinomial(integer, vBoolean), M::lnBinomial(integer, boolean));
    QCOMPARE(M::lnBinomial(integer, vInteger), M::lnBinomial(integer, integer));
    QCOMPARE(M::lnBinomial(integer, vReal), M::lnBinomial(integer, real));
    QCOMPARE(M::lnBinomial(integer, vComplex), M::lnBinomial(integer, complex));

    QCOMPARE(M::lnBinomial(real, vBoolean), M::lnBinomial(real, boolean));
    QCOMPARE(M::lnBinomial(real, vInteger), M::lnBinomial(real, integer));
    QCOMPARE(M::lnBinomial(real, vReal), M::lnBinomial(real, real));
    QCOMPARE(M::lnBinomial(real, vComplex), M::lnBinomial(real, complex));

    QCOMPARE(M::lnBinomial(complex, vBoolean), M::lnBinomial(complex, boolean));
    QCOMPARE(M::lnBinomial(complex, vInteger), M::lnBinomial(complex, integer));
    QCOMPARE(M::lnBinomial(complex, vReal), M::lnBinomial(complex, real));
    QCOMPARE(M::lnBinomial(complex, vComplex), M::lnBinomial(complex, complex));

    QCOMPARE(M::lnBinomial(vInteger, vBoolean), M::lnBinomial(integer, boolean));
    QCOMPARE(M::lnBinomial(vInteger, vInteger), M::lnBinomial(integer, integer));
    QCOMPARE(M::lnBinomial(vInteger, vReal), M::lnBinomial(integer, real));
    QCOMPARE(M::lnBinomial(vInteger, vComplex), M::lnBinomial(integer, complex));

    QCOMPARE(M::lnBinomial(vReal, vBoolean), M::lnBinomial(real, boolean));
    QCOMPARE(M::lnBinomial(vReal, vInteger), M::lnBinomial(real, integer));
    QCOMPARE(M::lnBinomial(vReal, vReal), M::lnBinomial(real, real));
    QCOMPARE(M::lnBinomial(vReal, vComplex), M::lnBinomial(real, complex));

    QCOMPARE(M::lnBinomial(vComplex, vBoolean), M::lnBinomial(complex, boolean));
    QCOMPARE(M::lnBinomial(vComplex, vInteger), M::lnBinomial(complex, integer));
    QCOMPARE(M::lnBinomial(vComplex, vReal), M::lnBinomial(complex, real));
    QCOMPARE(M::lnBinomial(vComplex, vComplex), M::lnBinomial(complex, complex));
}


void TestBasicFunctions::testLogBinomial() {
    std::mt19937                              rng(0x12345678);
    std::uniform_real_distribution<M::Real>   baseGenerator(1, 20);
    std::uniform_int_distribution<M::Integer> nGenerator(1, 1000);

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real    b = baseGenerator(rng);
        M::Integer n = nGenerator(rng);

        std::uniform_int_distribution<M::Integer> kGenerator(1, n);
        M::Integer k = kGenerator(rng);

        M::Complex measured      = M::logBinomial(b, n, k);
        M::Complex expected      = (M::lnFactorial(n) - M::lnFactorial(n - k) - M::lnFactorial(k)) / M::ln(b);
        M::Complex error         = measured - expected;
        M::Real    relativeError = M::abs(error) / M::abs(expected);

        if (relativeError >= 2.0E-14) {
            QCOMPARE(relativeError < 2.0E-14, true);
        }
    }
}


void TestBasicFunctions::testLogBinomialVariants() {
    M::Boolean boolean = true;
    M::Integer integer = 2;
    M::Real    real    = 3.0;
    M::Complex complex = M::Complex(4.0);

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);

    QCOMPARE(M::logBinomial(integer, vInteger, boolean), M::logBinomial(integer, integer, boolean));
    QCOMPARE(M::logBinomial(integer, vInteger, integer), M::logBinomial(integer, integer, integer));
    QCOMPARE(M::logBinomial(integer, vInteger, real), M::logBinomial(integer, integer, real));
    QCOMPARE(M::logBinomial(integer, vInteger, complex), M::logBinomial(integer, integer, complex));

    QCOMPARE(M::logBinomial(integer, vReal, boolean), M::logBinomial(integer, real, boolean));
    QCOMPARE(M::logBinomial(integer, vReal, integer), M::logBinomial(integer, real, integer));
    QCOMPARE(M::logBinomial(integer, vReal, real), M::logBinomial(integer, real, real));
    QCOMPARE(M::logBinomial(integer, vReal, complex), M::logBinomial(integer, real, complex));

    QCOMPARE(M::logBinomial(integer, vComplex, boolean), M::logBinomial(integer, complex, boolean));
    QCOMPARE(M::logBinomial(integer, vComplex, integer), M::logBinomial(integer, complex, integer));
    QCOMPARE(M::logBinomial(integer, vComplex, real), M::logBinomial(integer, complex, real));
    QCOMPARE(M::logBinomial(integer, vComplex, complex), M::logBinomial(integer, complex, complex));

    QCOMPARE(M::logBinomial(integer, integer, vBoolean), M::logBinomial(integer, integer, boolean));
    QCOMPARE(M::logBinomial(integer, integer, vInteger), M::logBinomial(integer, integer, integer));
    QCOMPARE(M::logBinomial(integer, integer, vReal), M::logBinomial(integer, integer, real));
    QCOMPARE(M::logBinomial(integer, integer, vComplex), M::logBinomial(integer, integer, complex));

    QCOMPARE(M::logBinomial(integer, real, vBoolean), M::logBinomial(integer, real, boolean));
    QCOMPARE(M::logBinomial(integer, real, vInteger), M::logBinomial(integer, real, integer));
    QCOMPARE(M::logBinomial(integer, real, vReal), M::logBinomial(integer, real, real));
    QCOMPARE(M::logBinomial(integer, real, vComplex), M::logBinomial(integer, real, complex));

    QCOMPARE(M::logBinomial(integer, complex, vBoolean), M::logBinomial(integer, complex, boolean));
    QCOMPARE(M::logBinomial(integer, complex, vInteger), M::logBinomial(integer, complex, integer));
    QCOMPARE(M::logBinomial(integer, complex, vReal), M::logBinomial(integer, complex, real));
    QCOMPARE(M::logBinomial(integer, complex, vComplex), M::logBinomial(integer, complex, complex));

    QCOMPARE(M::logBinomial(integer, vInteger, vBoolean), M::logBinomial(integer, integer, boolean));
    QCOMPARE(M::logBinomial(integer, vInteger, vInteger), M::logBinomial(integer, integer, integer));
    QCOMPARE(M::logBinomial(integer, vInteger, vReal), M::logBinomial(integer, integer, real));
    QCOMPARE(M::logBinomial(integer, vInteger, vComplex), M::logBinomial(integer, integer, complex));

    QCOMPARE(M::logBinomial(integer, vReal, vBoolean), M::logBinomial(integer, real, boolean));
    QCOMPARE(M::logBinomial(integer, vReal, vInteger), M::logBinomial(integer, real, integer));
    QCOMPARE(M::logBinomial(integer, vReal, vReal), M::logBinomial(integer, real, real));
    QCOMPARE(M::logBinomial(integer, vReal, vComplex), M::logBinomial(integer, real, complex));

    QCOMPARE(M::logBinomial(integer, vComplex, vBoolean), M::logBinomial(integer, complex, boolean));
    QCOMPARE(M::logBinomial(integer, vComplex, vInteger), M::logBinomial(integer, complex, integer));
    QCOMPARE(M::logBinomial(integer, vComplex, vReal), M::logBinomial(integer, complex, real));
    QCOMPARE(M::logBinomial(integer, vComplex, vComplex), M::logBinomial(integer, complex, complex));




    QCOMPARE(M::logBinomial(vInteger, vInteger, boolean), M::logBinomial(integer, integer, boolean));
    QCOMPARE(M::logBinomial(vInteger, vInteger, integer), M::logBinomial(integer, integer, integer));
    QCOMPARE(M::logBinomial(vInteger, vInteger, real), M::logBinomial(integer, integer, real));
    QCOMPARE(M::logBinomial(vInteger, vInteger, complex), M::logBinomial(integer, integer, complex));

    QCOMPARE(M::logBinomial(vInteger, vReal, boolean), M::logBinomial(integer, real, boolean));
    QCOMPARE(M::logBinomial(vInteger, vReal, integer), M::logBinomial(integer, real, integer));
    QCOMPARE(M::logBinomial(vInteger, vReal, real), M::logBinomial(integer, real, real));
    QCOMPARE(M::logBinomial(vInteger, vReal, complex), M::logBinomial(integer, real, complex));

    QCOMPARE(M::logBinomial(vInteger, vComplex, boolean), M::logBinomial(integer, complex, boolean));
    QCOMPARE(M::logBinomial(vInteger, vComplex, integer), M::logBinomial(integer, complex, integer));
    QCOMPARE(M::logBinomial(vInteger, vComplex, real), M::logBinomial(integer, complex, real));
    QCOMPARE(M::logBinomial(vInteger, vComplex, complex), M::logBinomial(integer, complex, complex));

    QCOMPARE(M::logBinomial(vInteger, integer, vBoolean), M::logBinomial(integer, integer, boolean));
    QCOMPARE(M::logBinomial(vInteger, integer, vInteger), M::logBinomial(integer, integer, integer));
    QCOMPARE(M::logBinomial(vInteger, integer, vReal), M::logBinomial(integer, integer, real));
    QCOMPARE(M::logBinomial(vInteger, integer, vComplex), M::logBinomial(integer, integer, complex));

    QCOMPARE(M::logBinomial(vInteger, real, vBoolean), M::logBinomial(integer, real, boolean));
    QCOMPARE(M::logBinomial(vInteger, real, vInteger), M::logBinomial(integer, real, integer));
    QCOMPARE(M::logBinomial(vInteger, real, vReal), M::logBinomial(integer, real, real));
    QCOMPARE(M::logBinomial(vInteger, real, vComplex), M::logBinomial(integer, real, complex));

    QCOMPARE(M::logBinomial(vInteger, complex, vBoolean), M::logBinomial(integer, complex, boolean));
    QCOMPARE(M::logBinomial(vInteger, complex, vInteger), M::logBinomial(integer, complex, integer));
    QCOMPARE(M::logBinomial(vInteger, complex, vReal), M::logBinomial(integer, complex, real));
    QCOMPARE(M::logBinomial(vInteger, complex, vComplex), M::logBinomial(integer, complex, complex));

    QCOMPARE(M::logBinomial(vInteger, vInteger, vBoolean), M::logBinomial(integer, integer, boolean));
    QCOMPARE(M::logBinomial(vInteger, vInteger, vInteger), M::logBinomial(integer, integer, integer));
    QCOMPARE(M::logBinomial(vInteger, vInteger, vReal), M::logBinomial(integer, integer, real));
    QCOMPARE(M::logBinomial(vInteger, vInteger, vComplex), M::logBinomial(integer, integer, complex));

    QCOMPARE(M::logBinomial(vInteger, vReal, vBoolean), M::logBinomial(integer, real, boolean));
    QCOMPARE(M::logBinomial(vInteger, vReal, vInteger), M::logBinomial(integer, real, integer));
    QCOMPARE(M::logBinomial(vInteger, vReal, vReal), M::logBinomial(integer, real, real));
    QCOMPARE(M::logBinomial(vInteger, vReal, vComplex), M::logBinomial(integer, real, complex));

    QCOMPARE(M::logBinomial(vInteger, vComplex, vBoolean), M::logBinomial(integer, complex, boolean));
    QCOMPARE(M::logBinomial(vInteger, vComplex, vInteger), M::logBinomial(integer, complex, integer));
    QCOMPARE(M::logBinomial(vInteger, vComplex, vReal), M::logBinomial(integer, complex, real));
    QCOMPARE(M::logBinomial(vInteger, vComplex, vComplex), M::logBinomial(integer, complex, complex));
}


void TestBasicFunctions::testUnsignedSterlingNumber1IntegerInteger() {
    QCOMPARE(M::unsignedSterlingNumber1(M::Integer(4), M::Integer(2)), 11);
    QCOMPARE(M::unsignedSterlingNumber1(M::Integer(3), M::Integer(2)), 3);
    QCOMPARE(M::unsignedSterlingNumber1(M::Integer(3), M::Integer(2)), 3);
}


void TestBasicFunctions::testUnsignedSterlingNumber1IntegerReal() {
    QCOMPARE(M::unsignedSterlingNumber1(M::Integer(4), M::Real(2)), M::Real(11));
    QCOMPARE(M::unsignedSterlingNumber1(M::Integer(3), M::Real(2)), M::Real(3));
    QCOMPARE(M::unsignedSterlingNumber1(M::Integer(3), M::Real(2)), M::Real(3));

    QCOMPARE(std::isnan(M::unsignedSterlingNumber1(M::Integer(3), M::Real(2.5))), true);
}


void TestBasicFunctions::testUnsignedSterlingNumber1IntegerComplex() {
    QCOMPARE(M::unsignedSterlingNumber1(M::Integer(4), M::Complex(2)), M::Real(11));
    QCOMPARE(M::unsignedSterlingNumber1(M::Integer(3), M::Complex(2)), M::Real(3));
    QCOMPARE(M::unsignedSterlingNumber1(M::Integer(3), M::Complex(2)), M::Real(3));

    QCOMPARE(std::isnan(M::unsignedSterlingNumber1(M::Integer(3), M::Complex(2.5))), true);
    QCOMPARE(std::isnan(M::unsignedSterlingNumber1(M::Integer(3), M::Complex(2, 1))), true);
}


void TestBasicFunctions::testUnsignedSterlingNumber1RealInteger() {
    QCOMPARE(M::unsignedSterlingNumber1(M::Real(4), M::Integer(2)), M::Real(11));
    QCOMPARE(M::unsignedSterlingNumber1(M::Real(3), M::Integer(2)), M::Real(3));
    QCOMPARE(M::unsignedSterlingNumber1(M::Real(3), M::Integer(2)), M::Real(3));

    QCOMPARE(std::isnan(M::unsignedSterlingNumber1(M::Real(3.5), M::Integer(2))), true);
}


void TestBasicFunctions::testUnsignedSterlingNumber1RealReal() {
    QCOMPARE(M::unsignedSterlingNumber1(M::Real(4), M::Real(2)), M::Real(11));
    QCOMPARE(M::unsignedSterlingNumber1(M::Real(3), M::Real(2)), M::Real(3));
    QCOMPARE(M::unsignedSterlingNumber1(M::Real(3), M::Real(2)), M::Real(3));

    QCOMPARE(std::isnan(M::unsignedSterlingNumber1(M::Real(3), M::Real(2.5))), true);
    QCOMPARE(std::isnan(M::unsignedSterlingNumber1(M::Real(3.5), M::Real(2))), true);
    QCOMPARE(std::isnan(M::unsignedSterlingNumber1(M::Real(3.5), M::Real(2.1))), true);
}


void TestBasicFunctions::testUnsignedSterlingNumber1RealComplex() {
    QCOMPARE(M::unsignedSterlingNumber1(M::Real(4), M::Complex(2)), M::Real(11));
    QCOMPARE(M::unsignedSterlingNumber1(M::Real(3), M::Complex(2)), M::Real(3));
    QCOMPARE(M::unsignedSterlingNumber1(M::Real(3), M::Complex(2)), M::Real(3));

    QCOMPARE(std::isnan(M::unsignedSterlingNumber1(M::Real(3), M::Complex(2.5))), true);
    QCOMPARE(std::isnan(M::unsignedSterlingNumber1(M::Real(3.5), M::Complex(2))), true);
    QCOMPARE(std::isnan(M::unsignedSterlingNumber1(M::Real(3.5), M::Complex(2.1))), true);
    QCOMPARE(std::isnan(M::unsignedSterlingNumber1(M::Real(3), M::Complex(2, 1))), true);
}


void TestBasicFunctions::testUnsignedSterlingNumber1ComplexInteger() {
    QCOMPARE(M::unsignedSterlingNumber1(M::Complex(4), M::Integer(2)), M::Real(11));
    QCOMPARE(M::unsignedSterlingNumber1(M::Complex(3), M::Integer(2)), M::Real(3));
    QCOMPARE(M::unsignedSterlingNumber1(M::Complex(3), M::Integer(2)), M::Real(3));

    QCOMPARE(std::isnan(M::unsignedSterlingNumber1(M::Complex(3.5), M::Integer(2))), true);
    QCOMPARE(std::isnan(M::unsignedSterlingNumber1(M::Complex(3, 5), M::Integer(2))), true);
}


void TestBasicFunctions::testUnsignedSterlingNumber1ComplexReal() {
    QCOMPARE(M::unsignedSterlingNumber1(M::Complex(4), M::Real(2)), M::Real(11));
    QCOMPARE(M::unsignedSterlingNumber1(M::Complex(3), M::Real(2)), M::Real(3));
    QCOMPARE(M::unsignedSterlingNumber1(M::Complex(3), M::Real(2)), M::Real(3));

    QCOMPARE(std::isnan(M::unsignedSterlingNumber1(M::Complex(3), M::Real(2.5))), true);
    QCOMPARE(std::isnan(M::unsignedSterlingNumber1(M::Complex(3.5), M::Real(2))), true);
    QCOMPARE(std::isnan(M::unsignedSterlingNumber1(M::Complex(3.5), M::Real(2.1))), true);
    QCOMPARE(std::isnan(M::unsignedSterlingNumber1(M::Complex(4, 1), M::Real(2))), true);
}


void TestBasicFunctions::testUnsignedSterlingNumber1ComplexComplex() {
    QCOMPARE(M::unsignedSterlingNumber1(M::Complex(4), M::Complex(2)), M::Real(11));
    QCOMPARE(M::unsignedSterlingNumber1(M::Complex(3), M::Complex(2)), M::Real(3));
    QCOMPARE(M::unsignedSterlingNumber1(M::Complex(3), M::Complex(2)), M::Real(3));

    QCOMPARE(std::isnan(M::unsignedSterlingNumber1(M::Complex(3), M::Complex(2.5))), true);
    QCOMPARE(std::isnan(M::unsignedSterlingNumber1(M::Complex(3.5), M::Complex(2))), true);
    QCOMPARE(std::isnan(M::unsignedSterlingNumber1(M::Complex(3.5), M::Complex(2.1))), true);
    QCOMPARE(std::isnan(M::unsignedSterlingNumber1(M::Complex(4), M::Complex(2, 1))), true);
}


void TestBasicFunctions::testUnsignedSterlingNumber1Variant() {
    M::Boolean boolean = true;
    M::Integer integer = 2;
    M::Real    real    = 3.0;
    M::Complex complex = M::Complex(4.0);

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);

    QCOMPARE(M::unsignedSterlingNumber1(vInteger, boolean), M::unsignedSterlingNumber1(integer, boolean));
    QCOMPARE(M::unsignedSterlingNumber1(vInteger, integer), M::unsignedSterlingNumber1(integer, integer));
    QCOMPARE(M::unsignedSterlingNumber1(vInteger, real), M::unsignedSterlingNumber1(integer, real));
    QCOMPARE(M::unsignedSterlingNumber1(vInteger, complex), M::unsignedSterlingNumber1(integer, complex));

    QCOMPARE(M::unsignedSterlingNumber1(vReal, boolean), M::unsignedSterlingNumber1(real, boolean));
    QCOMPARE(M::unsignedSterlingNumber1(vReal, integer), M::unsignedSterlingNumber1(real, integer));
    QCOMPARE(M::unsignedSterlingNumber1(vReal, real), M::unsignedSterlingNumber1(real, real));
    QCOMPARE(M::unsignedSterlingNumber1(vReal, complex), M::unsignedSterlingNumber1(real, complex));

    QCOMPARE(M::unsignedSterlingNumber1(vComplex, boolean), M::unsignedSterlingNumber1(complex, boolean));
    QCOMPARE(M::unsignedSterlingNumber1(vComplex, integer), M::unsignedSterlingNumber1(complex, integer));
    QCOMPARE(M::unsignedSterlingNumber1(vComplex, real), M::unsignedSterlingNumber1(complex, real));
    QCOMPARE(M::unsignedSterlingNumber1(vComplex, complex), M::unsignedSterlingNumber1(complex, complex));

    QCOMPARE(M::unsignedSterlingNumber1(integer, vBoolean), M::unsignedSterlingNumber1(integer, boolean));
    QCOMPARE(M::unsignedSterlingNumber1(integer, vInteger), M::unsignedSterlingNumber1(integer, integer));
    QCOMPARE(M::unsignedSterlingNumber1(integer, vReal), M::unsignedSterlingNumber1(integer, real));
    QCOMPARE(M::unsignedSterlingNumber1(integer, vComplex), M::unsignedSterlingNumber1(integer, complex));

    QCOMPARE(M::unsignedSterlingNumber1(real, vBoolean), M::unsignedSterlingNumber1(real, boolean));
    QCOMPARE(M::unsignedSterlingNumber1(real, vInteger), M::unsignedSterlingNumber1(real, integer));
    QCOMPARE(M::unsignedSterlingNumber1(real, vReal), M::unsignedSterlingNumber1(real, real));
    QCOMPARE(M::unsignedSterlingNumber1(real, vComplex), M::unsignedSterlingNumber1(real, complex));

    QCOMPARE(M::unsignedSterlingNumber1(complex, vBoolean), M::unsignedSterlingNumber1(complex, boolean));
    QCOMPARE(M::unsignedSterlingNumber1(complex, vInteger), M::unsignedSterlingNumber1(complex, integer));
    QCOMPARE(M::unsignedSterlingNumber1(complex, vReal), M::unsignedSterlingNumber1(complex, real));
    QCOMPARE(M::unsignedSterlingNumber1(complex, vComplex), M::unsignedSterlingNumber1(complex, complex));

    QCOMPARE(M::unsignedSterlingNumber1(vInteger, vBoolean), M::unsignedSterlingNumber1(integer, boolean));
    QCOMPARE(M::unsignedSterlingNumber1(vInteger, vInteger), M::unsignedSterlingNumber1(integer, integer));
    QCOMPARE(M::unsignedSterlingNumber1(vInteger, vReal), M::unsignedSterlingNumber1(integer, real));
    QCOMPARE(M::unsignedSterlingNumber1(vInteger, vComplex), M::unsignedSterlingNumber1(integer, complex));

    QCOMPARE(M::unsignedSterlingNumber1(vReal, vBoolean), M::unsignedSterlingNumber1(real, boolean));
    QCOMPARE(M::unsignedSterlingNumber1(vReal, vInteger), M::unsignedSterlingNumber1(real, integer));
    QCOMPARE(M::unsignedSterlingNumber1(vReal, vReal), M::unsignedSterlingNumber1(real, real));
    QCOMPARE(M::unsignedSterlingNumber1(vReal, vComplex), M::unsignedSterlingNumber1(real, complex));

    QCOMPARE(M::unsignedSterlingNumber1(vComplex, vBoolean), M::unsignedSterlingNumber1(complex, boolean));
    QCOMPARE(M::unsignedSterlingNumber1(vComplex, vInteger), M::unsignedSterlingNumber1(complex, integer));
    QCOMPARE(M::unsignedSterlingNumber1(vComplex, vReal), M::unsignedSterlingNumber1(complex, real));
    QCOMPARE(M::unsignedSterlingNumber1(vComplex, vComplex), M::unsignedSterlingNumber1(complex, complex));
}


void TestBasicFunctions::testSterlingNumber2IntegerInteger() {
    QCOMPARE(M::sterlingNumber2(M::Integer(4), M::Integer(2)), 7);
    QCOMPARE(M::sterlingNumber2(M::Integer(5), M::Integer(2)), 15);
    QCOMPARE(M::sterlingNumber2(M::Integer(7), M::Integer(3)), 301);
    QCOMPARE(M::sterlingNumber2(M::Integer(8), M::Integer(4)), 1701);
}


void TestBasicFunctions::testSterlingNumber2IntegerReal() {
    QCOMPARE(M::sterlingNumber2(M::Integer(4), M::Real(2)), 7);
    QCOMPARE(M::sterlingNumber2(M::Integer(5), M::Real(2)), 15);
    QCOMPARE(M::sterlingNumber2(M::Integer(7), M::Real(3)), 301);
    QCOMPARE(M::sterlingNumber2(M::Integer(8), M::Real(4)), 1701);

    QCOMPARE(std::isnan(M::sterlingNumber2(M::Integer(8), M::Real(4.5))), true);
}


void TestBasicFunctions::testSterlingNumber2IntegerComplex() {
    QCOMPARE(M::sterlingNumber2(M::Integer(4), M::Complex(2)), 7);
    QCOMPARE(M::sterlingNumber2(M::Integer(5), M::Complex(2)), 15);
    QCOMPARE(M::sterlingNumber2(M::Integer(7), M::Complex(3)), 301);
    QCOMPARE(M::sterlingNumber2(M::Integer(8), M::Complex(4)), 1701);

    QCOMPARE(std::isnan(M::sterlingNumber2(M::Integer(8), M::Complex(4.5))), true);
    QCOMPARE(std::isnan(M::sterlingNumber2(M::Integer(8), M::Complex(4, 5))), true);
}


void TestBasicFunctions::testSterlingNumber2RealInteger() {
    QCOMPARE(M::sterlingNumber2(M::Real(4), M::Integer(2)), 7);
    QCOMPARE(M::sterlingNumber2(M::Real(5), M::Integer(2)), 15);
    QCOMPARE(M::sterlingNumber2(M::Real(7), M::Integer(3)), 301);
    QCOMPARE(M::sterlingNumber2(M::Real(8), M::Integer(4)), 1701);

    QCOMPARE(std::isnan(M::sterlingNumber2(M::Real(8.5), M::Integer(4))), true);
}


void TestBasicFunctions::testSterlingNumber2RealReal() {
    QCOMPARE(M::sterlingNumber2(M::Real(4), M::Real(2)), 7);
    QCOMPARE(M::sterlingNumber2(M::Real(5), M::Real(2)), 15);
    QCOMPARE(M::sterlingNumber2(M::Real(7), M::Real(3)), 301);
    QCOMPARE(M::sterlingNumber2(M::Real(8), M::Real(4)), 1701);

    QCOMPARE(std::isnan(M::sterlingNumber2(M::Real(8), M::Real(4.5))), true);
    QCOMPARE(std::isnan(M::sterlingNumber2(M::Real(8.5), M::Real(4))), true);
}


void TestBasicFunctions::testSterlingNumber2RealComplex() {
    QCOMPARE(M::sterlingNumber2(M::Real(4), M::Complex(2)), 7);
    QCOMPARE(M::sterlingNumber2(M::Real(5), M::Complex(2)), 15);
    QCOMPARE(M::sterlingNumber2(M::Real(7), M::Complex(3)), 301);
    QCOMPARE(M::sterlingNumber2(M::Real(8), M::Complex(4)), 1701);

    QCOMPARE(std::isnan(M::sterlingNumber2(M::Real(8), M::Complex(4.5))), true);
    QCOMPARE(std::isnan(M::sterlingNumber2(M::Real(8), M::Complex(4, 5))), true);
    QCOMPARE(std::isnan(M::sterlingNumber2(M::Real(8.5), M::Complex(4))), true);
}


void TestBasicFunctions::testSterlingNumber2ComplexInteger() {
    QCOMPARE(M::sterlingNumber2(M::Complex(4), M::Integer(2)), 7);
    QCOMPARE(M::sterlingNumber2(M::Complex(5), M::Integer(2)), 15);
    QCOMPARE(M::sterlingNumber2(M::Complex(7), M::Integer(3)), 301);
    QCOMPARE(M::sterlingNumber2(M::Complex(8), M::Integer(4)), 1701);

    QCOMPARE(std::isnan(M::sterlingNumber2(M::Complex(8.5), M::Integer(4))), true);
    QCOMPARE(std::isnan(M::sterlingNumber2(M::Complex(8, 5), M::Integer(4))), true);
}


void TestBasicFunctions::testSterlingNumber2ComplexReal() {
    QCOMPARE(M::sterlingNumber2(M::Complex(4), M::Real(2)), 7);
    QCOMPARE(M::sterlingNumber2(M::Complex(5), M::Real(2)), 15);
    QCOMPARE(M::sterlingNumber2(M::Complex(7), M::Real(3)), 301);
    QCOMPARE(M::sterlingNumber2(M::Complex(8), M::Real(4)), 1701);

    QCOMPARE(std::isnan(M::sterlingNumber2(M::Complex(8), M::Real(4.5))), true);
    QCOMPARE(std::isnan(M::sterlingNumber2(M::Complex(8.5), M::Real(4))), true);
    QCOMPARE(std::isnan(M::sterlingNumber2(M::Complex(8, 5), M::Real(4))), true);
}


void TestBasicFunctions::testSterlingNumber2ComplexComplex() {
    QCOMPARE(M::sterlingNumber2(M::Complex(4), M::Complex(2)), 7);
    QCOMPARE(M::sterlingNumber2(M::Complex(5), M::Complex(2)), 15);
    QCOMPARE(M::sterlingNumber2(M::Complex(7), M::Complex(3)), 301);
    QCOMPARE(M::sterlingNumber2(M::Complex(8), M::Complex(4)), 1701);

    QCOMPARE(std::isnan(M::sterlingNumber2(M::Complex(8), M::Complex(4.5))), true);
    QCOMPARE(std::isnan(M::sterlingNumber2(M::Complex(8), M::Complex(4, 5))), true);
    QCOMPARE(std::isnan(M::sterlingNumber2(M::Complex(8.5), M::Complex(4))), true);
    QCOMPARE(std::isnan(M::sterlingNumber2(M::Complex(8, 5), M::Complex(4))), true);
}


void TestBasicFunctions::testSterlingNumber2Variant() {
    M::Boolean boolean = true;
    M::Integer integer = 2;
    M::Real    real    = 3.0;
    M::Complex complex = M::Complex(4.0);

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);

    QCOMPARE(M::sterlingNumber2(vInteger, boolean), M::sterlingNumber2(integer, boolean));
    QCOMPARE(M::sterlingNumber2(vInteger, integer), M::sterlingNumber2(integer, integer));
    QCOMPARE(M::sterlingNumber2(vInteger, real), M::sterlingNumber2(integer, real));
    QCOMPARE(M::sterlingNumber2(vInteger, complex), M::sterlingNumber2(integer, complex));

    QCOMPARE(M::sterlingNumber2(vReal, boolean), M::sterlingNumber2(real, boolean));
    QCOMPARE(M::sterlingNumber2(vReal, integer), M::sterlingNumber2(real, integer));
    QCOMPARE(M::sterlingNumber2(vReal, real), M::sterlingNumber2(real, real));
    QCOMPARE(M::sterlingNumber2(vReal, complex), M::sterlingNumber2(real, complex));

    QCOMPARE(M::sterlingNumber2(vComplex, boolean), M::sterlingNumber2(complex, boolean));
    QCOMPARE(M::sterlingNumber2(vComplex, integer), M::sterlingNumber2(complex, integer));
    QCOMPARE(M::sterlingNumber2(vComplex, real), M::sterlingNumber2(complex, real));
    QCOMPARE(M::sterlingNumber2(vComplex, complex), M::sterlingNumber2(complex, complex));

    QCOMPARE(M::sterlingNumber2(integer, vBoolean), M::sterlingNumber2(integer, boolean));
    QCOMPARE(M::sterlingNumber2(integer, vInteger), M::sterlingNumber2(integer, integer));
    QCOMPARE(M::sterlingNumber2(integer, vReal), M::sterlingNumber2(integer, real));
    QCOMPARE(M::sterlingNumber2(integer, vComplex), M::sterlingNumber2(integer, complex));

    QCOMPARE(M::sterlingNumber2(real, vBoolean), M::sterlingNumber2(real, boolean));
    QCOMPARE(M::sterlingNumber2(real, vInteger), M::sterlingNumber2(real, integer));
    QCOMPARE(M::sterlingNumber2(real, vReal), M::sterlingNumber2(real, real));
    QCOMPARE(M::sterlingNumber2(real, vComplex), M::sterlingNumber2(real, complex));

    QCOMPARE(M::sterlingNumber2(complex, vBoolean), M::sterlingNumber2(complex, boolean));
    QCOMPARE(M::sterlingNumber2(complex, vInteger), M::sterlingNumber2(complex, integer));
    QCOMPARE(M::sterlingNumber2(complex, vReal), M::sterlingNumber2(complex, real));
    QCOMPARE(M::sterlingNumber2(complex, vComplex), M::sterlingNumber2(complex, complex));

    QCOMPARE(M::sterlingNumber2(vInteger, vBoolean), M::sterlingNumber2(integer, boolean));
    QCOMPARE(M::sterlingNumber2(vInteger, vInteger), M::sterlingNumber2(integer, integer));
    QCOMPARE(M::sterlingNumber2(vInteger, vReal), M::sterlingNumber2(integer, real));
    QCOMPARE(M::sterlingNumber2(vInteger, vComplex), M::sterlingNumber2(integer, complex));

    QCOMPARE(M::sterlingNumber2(vReal, vBoolean), M::sterlingNumber2(real, boolean));
    QCOMPARE(M::sterlingNumber2(vReal, vInteger), M::sterlingNumber2(real, integer));
    QCOMPARE(M::sterlingNumber2(vReal, vReal), M::sterlingNumber2(real, real));
    QCOMPARE(M::sterlingNumber2(vReal, vComplex), M::sterlingNumber2(real, complex));

    QCOMPARE(M::sterlingNumber2(vComplex, vBoolean), M::sterlingNumber2(complex, boolean));
    QCOMPARE(M::sterlingNumber2(vComplex, vInteger), M::sterlingNumber2(complex, integer));
    QCOMPARE(M::sterlingNumber2(vComplex, vReal), M::sterlingNumber2(complex, real));
    QCOMPARE(M::sterlingNumber2(vComplex, vComplex), M::sterlingNumber2(complex, complex));
}


void TestBasicFunctions::testLowerGammaIntegerInteger() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> sGenerator(1, 100);
    std::uniform_int_distribution<M::Integer> zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer s = sGenerator(rng);
        M::Integer z = zGenerator(rng);

        M::Real measured = M::lowerGamma(s, z);
        M::Real expected = boost::math::tgamma_lower(static_cast<M::Real>(s), static_cast<M::Real>(z));

        M::Real error = measured - expected;
        if (expected == 0) {
            if (error >= 1.0E-12) {
                QCOMPARE(error < 1.0E-12, true);
            }
        } else {
            M::Real relativeError = std::abs(error) / std::abs(expected);

            if (relativeError >= 1.0E-13) {
                QCOMPARE(relativeError < 1.0E-13, true);
            }

            if (relativeError > maximumError) {
                maximumError = relativeError;
            }

            if (!std::isnan(relativeError)) {
                sumError += relativeError;
            } else {
                QCOMPARE(std::isnan(relativeError), false);
            }
        }
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "lowerGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testLowerGammaIntegerReal() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> sGenerator(1, 100);
    std::uniform_real_distribution<M::Real>   zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer s = sGenerator(rng);
        M::Real    z = zGenerator(rng);

        M::Real measured = M::lowerGamma(s, z);
        M::Real expected = boost::math::tgamma_lower(static_cast<M::Real>(s), static_cast<M::Real>(z));

        M::Real error         = measured - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        if (relativeError >= 2.0E-13) {
            QCOMPARE(relativeError < 2.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "lowerGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testLowerGammaIntegerComplex() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> sGenerator(1, 100);
    std::uniform_real_distribution<M::Real>   zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer s = sGenerator(rng);
        M::Complex z(zGenerator(rng), 0);

        M::Complex measured = M::lowerGamma(s, z);
        M::Real    expected = boost::math::tgamma_lower(static_cast<M::Real>(s), z.real());

        M::Real error         = measured.real() - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        QCOMPARE(measured.imag(), 0);
        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "lowerGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testLowerGammaRealInteger() {
    std::mt19937                              rng(0x12345678);
    std::uniform_real_distribution<M::Real>   sGenerator(0, 100);
    std::uniform_int_distribution<M::Integer> zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real    s = sGenerator(rng);
        M::Integer z = zGenerator(rng);

        M::Real measured = M::lowerGamma(s, z);
        M::Real expected = boost::math::tgamma_lower(s, static_cast<M::Real>(z));

        M::Real error         = measured - expected;
        if (expected == 0) {
            if (error >= 1.0E-12) {
                QCOMPARE(error < 1.0E-12, true);
            }
        } else {
            M::Real relativeError = std::abs(error) / std::abs(expected);

            if (relativeError >= 2.0E-13) {
                QCOMPARE(relativeError < 2.0E-13, true);
            }

            if (relativeError > maximumError) {
                maximumError = relativeError;
            }

            QCOMPARE(!std::isnan(relativeError), true);
            sumError += relativeError;
        }
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "lowerGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testLowerGammaRealReal() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> valueGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real s = valueGenerator(rng);
        M::Real z = valueGenerator(rng);

        M::Real measured = M::lowerGamma(s, z);
        M::Real expected = boost::math::tgamma_lower(s, z);

        M::Real error         = measured - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        if (relativeError >= 2.0E-13) {
            QCOMPARE(relativeError < 2.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "lowerGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testLowerGammaRealComplex() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> sGenerator(0, 100);
    std::uniform_real_distribution<M::Real> zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real    s = sGenerator(rng);
        M::Complex z(zGenerator(rng), 0);

        M::Complex measured = M::lowerGamma(s, z);
        M::Real    expected = boost::math::tgamma_lower(s, z.real());

        M::Real error         = measured.real() - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        QCOMPARE(measured.imag(), 0);
        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "lowerGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testLowerGammaComplexInteger() {
    std::mt19937                              rng(0x12345678);
    std::uniform_real_distribution<M::Real>   sGenerator(0, 100);
    std::uniform_int_distribution<M::Integer> zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex s(sGenerator(rng), 0);
        M::Integer z = zGenerator(rng);

        M::Complex measured = M::lowerGamma(s, z);
        M::Real    expected = boost::math::tgamma_lower(s.real(), static_cast<M::Real>(z));

        M::Real error         = measured.real() - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        QCOMPARE(measured.imag(), 0);
        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "lowerGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testLowerGammaComplexReal() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> sGenerator(0, 100);
    std::uniform_real_distribution<M::Real> zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex s(sGenerator(rng), 0);
        M::Real    z = zGenerator(rng);

        M::Complex measured = M::lowerGamma(s, z);
        M::Real    expected = boost::math::tgamma_lower(s.real(), z);

        M::Real error         = measured.real() - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        QCOMPARE(measured.imag(), 0);
        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "lowerGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testLowerGammaComplexComplex() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> sGenerator(0, 100);
    std::uniform_real_distribution<M::Real> zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex s(sGenerator(rng), 0);
        M::Complex z(zGenerator(rng), 0);

        M::Complex measured = M::lowerGamma(s, z);
        M::Real    expected = boost::math::tgamma_lower(s.real(), z.real());

        M::Real error         = measured.real() - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        QCOMPARE(measured.imag(), 0);
        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "lowerGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testLowerGammaVariants() {
    M::Boolean boolean = true;
    M::Integer integer = 2;
    M::Real    real    = 3.0;
    M::Complex complex = M::Complex(4.0);

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);

    QCOMPARE(M::lowerGamma(vInteger, boolean), M::lowerGamma(integer, boolean));
    QCOMPARE(M::lowerGamma(vInteger, integer), M::lowerGamma(integer, integer));
    QCOMPARE(M::lowerGamma(vInteger, real), M::lowerGamma(integer, real));
    QCOMPARE(M::lowerGamma(vInteger, complex), M::lowerGamma(integer, complex));

    QCOMPARE(M::lowerGamma(vReal, boolean), M::lowerGamma(real, boolean));
    QCOMPARE(M::lowerGamma(vReal, integer), M::lowerGamma(real, integer));
    QCOMPARE(M::lowerGamma(vReal, real), M::lowerGamma(real, real));
    QCOMPARE(M::lowerGamma(vReal, complex), M::lowerGamma(real, complex));

    QCOMPARE(M::lowerGamma(vComplex, boolean), M::lowerGamma(complex, boolean));
    QCOMPARE(M::lowerGamma(vComplex, integer), M::lowerGamma(complex, integer));
    QCOMPARE(M::lowerGamma(vComplex, real), M::lowerGamma(complex, real));
    QCOMPARE(M::lowerGamma(vComplex, complex), M::lowerGamma(complex, complex));

    QCOMPARE(M::lowerGamma(integer, vBoolean), M::lowerGamma(integer, boolean));
    QCOMPARE(M::lowerGamma(integer, vInteger), M::lowerGamma(integer, integer));
    QCOMPARE(M::lowerGamma(integer, vReal), M::lowerGamma(integer, real));
    QCOMPARE(M::lowerGamma(integer, vComplex), M::lowerGamma(integer, complex));

    QCOMPARE(M::lowerGamma(real, vBoolean), M::lowerGamma(real, boolean));
    QCOMPARE(M::lowerGamma(real, vInteger), M::lowerGamma(real, integer));
    QCOMPARE(M::lowerGamma(real, vReal), M::lowerGamma(real, real));
    QCOMPARE(M::lowerGamma(real, vComplex), M::lowerGamma(real, complex));

    QCOMPARE(M::lowerGamma(complex, vBoolean), M::lowerGamma(complex, boolean));
    QCOMPARE(M::lowerGamma(complex, vInteger), M::lowerGamma(complex, integer));
    QCOMPARE(M::lowerGamma(complex, vReal), M::lowerGamma(complex, real));
    QCOMPARE(M::lowerGamma(complex, vComplex), M::lowerGamma(complex, complex));

    QCOMPARE(M::lowerGamma(vInteger, vBoolean), M::lowerGamma(integer, boolean));
    QCOMPARE(M::lowerGamma(vInteger, vInteger), M::lowerGamma(integer, integer));
    QCOMPARE(M::lowerGamma(vInteger, vReal), M::lowerGamma(integer, real));
    QCOMPARE(M::lowerGamma(vInteger, vComplex), M::lowerGamma(integer, complex));

    QCOMPARE(M::lowerGamma(vReal, vBoolean), M::lowerGamma(real, boolean));
    QCOMPARE(M::lowerGamma(vReal, vInteger), M::lowerGamma(real, integer));
    QCOMPARE(M::lowerGamma(vReal, vReal), M::lowerGamma(real, real));
    QCOMPARE(M::lowerGamma(vReal, vComplex), M::lowerGamma(real, complex));

    QCOMPARE(M::lowerGamma(vComplex, vBoolean), M::lowerGamma(complex, boolean));
    QCOMPARE(M::lowerGamma(vComplex, vInteger), M::lowerGamma(complex, integer));
    QCOMPARE(M::lowerGamma(vComplex, vReal), M::lowerGamma(complex, real));
    QCOMPARE(M::lowerGamma(vComplex, vComplex), M::lowerGamma(complex, complex));
}


void TestBasicFunctions::testUpperGammaIntegerInteger() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> sGenerator(1, 100);
    std::uniform_int_distribution<M::Integer> zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer s = sGenerator(rng);
        M::Integer z = zGenerator(rng);

        M::Real measured = M::upperGamma(s, z);
        M::Real expected = boost::math::tgamma(static_cast<M::Real>(s), static_cast<M::Real>(z));

        M::Real error = measured - expected;
        if (expected == 0) {
            if (error >= 1.0E-12) {
                QCOMPARE(error < 1.0E-12, true);
            }
        } else {
            M::Real relativeError = std::abs(error) / std::abs(expected);

            if (relativeError >= 2.0E-13) {
                QCOMPARE(relativeError < 2.0E-13, true);
            }

            if (relativeError > maximumError) {
                maximumError = relativeError;
            }

            if (!std::isnan(relativeError)) {
                sumError += relativeError;
            } else {
                QCOMPARE(std::isnan(relativeError), false);
            }
        }
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "upperGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testUpperGammaIntegerReal() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> sGenerator(1, 100);
    std::uniform_real_distribution<M::Real>   zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer s = sGenerator(rng);
        M::Real    z = zGenerator(rng);

        M::Real measured = M::upperGamma(s, z);
        M::Real expected = boost::math::tgamma(static_cast<M::Real>(s), static_cast<M::Real>(z));

        M::Real error         = measured - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        if (relativeError >= 2.0E-13) {
            QCOMPARE(relativeError < 2.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "upperGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testUpperGammaIntegerComplex() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> sGenerator(1, 100);
    std::uniform_real_distribution<M::Real>   zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer s = sGenerator(rng);
        M::Complex z(zGenerator(rng), 0);

        M::Complex measured = M::upperGamma(s, z);
        M::Real    expected = boost::math::tgamma(static_cast<M::Real>(s), z.real());

        M::Real error         = measured.real() - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        QCOMPARE(measured.imag(), 0);
        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "upperGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testUpperGammaRealInteger() {
    std::mt19937                              rng(0x12345678);
    std::uniform_real_distribution<M::Real>   sGenerator(0, 100);
    std::uniform_int_distribution<M::Integer> zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real    s = sGenerator(rng);
        M::Integer z = zGenerator(rng);

        M::Real measured = M::upperGamma(s, z);
        M::Real expected = boost::math::tgamma(s, static_cast<M::Real>(z));

        M::Real error         = measured - expected;
        if (expected == 0) {
            if (error >= 1.0E-12) {
                QCOMPARE(error < 1.0E-12, true);
            }
        } else {
            M::Real relativeError = std::abs(error) / std::abs(expected);

            if (relativeError >= 2.0E-13) {
                QCOMPARE(relativeError < 2.0E-13, true);
            }

            if (relativeError > maximumError) {
                maximumError = relativeError;
            }

            QCOMPARE(!std::isnan(relativeError), true);
            sumError += relativeError;
        }
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "upperGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testUpperGammaRealReal() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> valueGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real s = valueGenerator(rng);
        M::Real z = valueGenerator(rng);

        M::Real measured = M::upperGamma(s, z);
        M::Real expected = boost::math::tgamma(s, z);

        M::Real error         = measured - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "upperGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testUpperGammaRealComplex() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> sGenerator(0, 100);
    std::uniform_real_distribution<M::Real> zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real    s = sGenerator(rng);
        M::Complex z(zGenerator(rng), 0);

        M::Complex measured = M::upperGamma(s, z);
        M::Real    expected = boost::math::tgamma(s, z.real());

        M::Real error         = measured.real() - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        QCOMPARE(measured.imag(), 0);
        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "upperGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testUpperGammaComplexInteger() {
    std::mt19937                              rng(0x12345678);
    std::uniform_real_distribution<M::Real>   sGenerator(0, 100);
    std::uniform_int_distribution<M::Integer> zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex s(sGenerator(rng), 0);
        M::Integer z = zGenerator(rng);

        M::Complex measured = M::upperGamma(s, z);
        M::Real    expected = boost::math::tgamma(s.real(), static_cast<M::Real>(z));

        M::Real error         = measured.real() - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        QCOMPARE(measured.imag(), 0);
        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "upperGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testUpperGammaComplexReal() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> sGenerator(0, 100);
    std::uniform_real_distribution<M::Real> zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex s(sGenerator(rng), 0);
        M::Real    z = zGenerator(rng);

        M::Complex measured = M::upperGamma(s, z);
        M::Real    expected = boost::math::tgamma(s.real(), z);

        M::Real error         = measured.real() - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        QCOMPARE(measured.imag(), 0);
        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "upperGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testUpperGammaComplexComplex() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> sGenerator(0, 100);
    std::uniform_real_distribution<M::Real> zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex s(sGenerator(rng), 0);
        M::Complex z(zGenerator(rng), 0);

        M::Complex measured = M::upperGamma(s, z);
        M::Real    expected = boost::math::tgamma(s.real(), z.real());

        M::Real error         = measured.real() - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        QCOMPARE(measured.imag(), 0);
        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "upperGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testUpperGammaVariants() {
    M::Boolean boolean = true;
    M::Integer integer = 2;
    M::Real    real    = 3.0;
    M::Complex complex = M::Complex(4.0);

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);

    QCOMPARE(M::upperGamma(vInteger, boolean), M::upperGamma(integer, boolean));
    QCOMPARE(M::upperGamma(vInteger, integer), M::upperGamma(integer, integer));
    QCOMPARE(M::upperGamma(vInteger, real), M::upperGamma(integer, real));
    QCOMPARE(M::upperGamma(vInteger, complex), M::upperGamma(integer, complex));

    QCOMPARE(M::upperGamma(vReal, boolean), M::upperGamma(real, boolean));
    QCOMPARE(M::upperGamma(vReal, integer), M::upperGamma(real, integer));
    QCOMPARE(M::upperGamma(vReal, real), M::upperGamma(real, real));
    QCOMPARE(M::upperGamma(vReal, complex), M::upperGamma(real, complex));

    QCOMPARE(M::upperGamma(vComplex, boolean), M::upperGamma(complex, boolean));
    QCOMPARE(M::upperGamma(vComplex, integer), M::upperGamma(complex, integer));
    QCOMPARE(M::upperGamma(vComplex, real), M::upperGamma(complex, real));
    QCOMPARE(M::upperGamma(vComplex, complex), M::upperGamma(complex, complex));

    QCOMPARE(M::upperGamma(integer, vBoolean), M::upperGamma(integer, boolean));
    QCOMPARE(M::upperGamma(integer, vInteger), M::upperGamma(integer, integer));
    QCOMPARE(M::upperGamma(integer, vReal), M::upperGamma(integer, real));
    QCOMPARE(M::upperGamma(integer, vComplex), M::upperGamma(integer, complex));

    QCOMPARE(M::upperGamma(real, vBoolean), M::upperGamma(real, boolean));
    QCOMPARE(M::upperGamma(real, vInteger), M::upperGamma(real, integer));
    QCOMPARE(M::upperGamma(real, vReal), M::upperGamma(real, real));
    QCOMPARE(M::upperGamma(real, vComplex), M::upperGamma(real, complex));

    QCOMPARE(M::upperGamma(complex, vBoolean), M::upperGamma(complex, boolean));
    QCOMPARE(M::upperGamma(complex, vInteger), M::upperGamma(complex, integer));
    QCOMPARE(M::upperGamma(complex, vReal), M::upperGamma(complex, real));
    QCOMPARE(M::upperGamma(complex, vComplex), M::upperGamma(complex, complex));

    QCOMPARE(M::upperGamma(vInteger, vBoolean), M::upperGamma(integer, boolean));
    QCOMPARE(M::upperGamma(vInteger, vInteger), M::upperGamma(integer, integer));
    QCOMPARE(M::upperGamma(vInteger, vReal), M::upperGamma(integer, real));
    QCOMPARE(M::upperGamma(vInteger, vComplex), M::upperGamma(integer, complex));

    QCOMPARE(M::upperGamma(vReal, vBoolean), M::upperGamma(real, boolean));
    QCOMPARE(M::upperGamma(vReal, vInteger), M::upperGamma(real, integer));
    QCOMPARE(M::upperGamma(vReal, vReal), M::upperGamma(real, real));
    QCOMPARE(M::upperGamma(vReal, vComplex), M::upperGamma(real, complex));

    QCOMPARE(M::upperGamma(vComplex, vBoolean), M::upperGamma(complex, boolean));
    QCOMPARE(M::upperGamma(vComplex, vInteger), M::upperGamma(complex, integer));
    QCOMPARE(M::upperGamma(vComplex, vReal), M::upperGamma(complex, real));
    QCOMPARE(M::upperGamma(vComplex, vComplex), M::upperGamma(complex, complex));
}


void TestBasicFunctions::testUpperLowerGammaComplexValues() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> sGenerator(0, 100);
    std::uniform_real_distribution<M::Real> zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex s(sGenerator(rng), 0);
        M::Complex z(zGenerator(rng), 0);

        M::Complex measuredLower = M::lowerGamma(s, z);
        M::Complex measuredUpper = M::upperGamma(s, z);
        M::Complex measured      = measuredLower + measuredUpper;
        M::Complex expected      = M::gamma(s);

        M::Real error         = M::abs(measured - expected);
        M::Real relativeError = error / M::abs(expected);

        QCOMPARE(!std::isnan(relativeError), true);

        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "complex value test: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testNormalizedLowerGammaIntegerInteger() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> sGenerator(1, 100);
    std::uniform_int_distribution<M::Integer> zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer s = sGenerator(rng);
        M::Integer z = zGenerator(rng);

        M::Real measured = M::normalizedLowerGamma(s, z);
        M::Real expected = boost::math::gamma_p(static_cast<M::Real>(s), static_cast<M::Real>(z));

        M::Real error = measured - expected;
        if (expected == 0) {
            if (error >= 1.0E-12) {
                QCOMPARE(error < 1.0E-12, true);
            }
        } else {
            M::Real relativeError = std::abs(error) / std::abs(expected);

            if (relativeError >= 2.0E-13) {
                QCOMPARE(relativeError < 2.0E-13, true);
            }

            if (relativeError > maximumError) {
                maximumError = relativeError;
            }

            if (!std::isnan(relativeError)) {
                sumError += relativeError;
            } else {
                QCOMPARE(std::isnan(relativeError), false);
            }
        }
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "upperGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testNormalizedLowerGammaIntegerReal() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> sGenerator(1, 100);
    std::uniform_real_distribution<M::Real>   zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer s = sGenerator(rng);
        M::Real    z = zGenerator(rng);

        M::Real measured = M::normalizedLowerGamma(s, z);
        M::Real expected = boost::math::gamma_p(static_cast<M::Real>(s), static_cast<M::Real>(z));

        M::Real error         = measured - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        if (relativeError >= 2.0E-13) {
            QCOMPARE(relativeError < 2.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "upperGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testNormalizedLowerGammaIntegerComplex() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> sGenerator(1, 100);
    std::uniform_real_distribution<M::Real>   zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer s = sGenerator(rng);
        M::Complex z(zGenerator(rng), 0);

        M::Complex measured = M::normalizedLowerGamma(s, z);
        M::Real    expected = boost::math::gamma_p(static_cast<M::Real>(s), z.real());

        M::Real error         = measured.real() - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        QCOMPARE(measured.imag(), 0);
        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "upperGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testNormalizedLowerGammaRealInteger() {
    std::mt19937                              rng(0x12345678);
    std::uniform_real_distribution<M::Real>   sGenerator(0, 100);
    std::uniform_int_distribution<M::Integer> zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real    s = sGenerator(rng);
        M::Integer z = zGenerator(rng);

        M::Real measured = M::normalizedLowerGamma(s, z);
        M::Real expected = boost::math::gamma_p(s, static_cast<M::Real>(z));

        M::Real error         = measured - expected;
        if (expected == 0) {
            if (error >= 1.0E-12) {
                QCOMPARE(error < 1.0E-12, true);
            }
        } else {
            M::Real relativeError = std::abs(error) / std::abs(expected);

            if (relativeError >= 2.0E-13) {
                QCOMPARE(relativeError < 2.0E-13, true);
            }

            if (relativeError > maximumError) {
                maximumError = relativeError;
            }

            QCOMPARE(!std::isnan(relativeError), true);
            sumError += relativeError;
        }
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "upperGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testNormalizedLowerGammaRealReal() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> valueGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real s = valueGenerator(rng);
        M::Real z = valueGenerator(rng);

        M::Real measured = M::normalizedLowerGamma(s, z);
        M::Real expected = boost::math::gamma_p(s, z);

        M::Real error         = measured - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        if (relativeError >= 2.0E-13) {
            QCOMPARE(relativeError < 2.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "upperGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testNormalizedLowerGammaRealComplex() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> sGenerator(0, 100);
    std::uniform_real_distribution<M::Real> zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real    s = sGenerator(rng);
        M::Complex z(zGenerator(rng), 0);

        M::Complex measured = M::normalizedLowerGamma(s, z);
        M::Real    expected = boost::math::gamma_p(s, z.real());

        M::Real error         = measured.real() - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        QCOMPARE(measured.imag(), 0);
        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "upperGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testNormalizedLowerGammaComplexInteger() {
    std::mt19937                              rng(0x12345678);
    std::uniform_real_distribution<M::Real>   sGenerator(0, 100);
    std::uniform_int_distribution<M::Integer> zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex s(sGenerator(rng), 0);
        M::Integer z = zGenerator(rng);

        M::Complex measured = M::normalizedLowerGamma(s, z);
        M::Real    expected = boost::math::gamma_p(s.real(), static_cast<M::Real>(z));

        M::Real error         = measured.real() - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        QCOMPARE(measured.imag(), 0);
        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "upperGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testNormalizedLowerGammaComplexReal() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> sGenerator(0, 100);
    std::uniform_real_distribution<M::Real> zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex s(sGenerator(rng), 0);
        M::Real    z = zGenerator(rng);

        M::Complex measured = M::normalizedLowerGamma(s, z);
        M::Real    expected = boost::math::gamma_p(s.real(), z);

        M::Real error         = measured.real() - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        QCOMPARE(measured.imag(), 0);
        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "upperGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testNormalizedLowerGammaComplexComplex() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> sGenerator(0, 100);
    std::uniform_real_distribution<M::Real> zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex s(sGenerator(rng), 0);
        M::Complex z(zGenerator(rng), 0);

        M::Complex measured = M::normalizedLowerGamma(s, z);
        M::Real    expected = boost::math::gamma_p(s.real(), z.real());

        M::Real error         = measured.real() - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        QCOMPARE(measured.imag(), 0);
        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "upperGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testNormalizedLowerGammaVariants() {
    M::Boolean boolean = true;
    M::Integer integer = 2;
    M::Real    real    = 3.0;
    M::Complex complex = M::Complex(4.0);

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);

    QCOMPARE(M::normalizedLowerGamma(vInteger, boolean), M::normalizedLowerGamma(integer, boolean));
    QCOMPARE(M::normalizedLowerGamma(vInteger, integer), M::normalizedLowerGamma(integer, integer));
    QCOMPARE(M::normalizedLowerGamma(vInteger, real), M::normalizedLowerGamma(integer, real));
    QCOMPARE(M::normalizedLowerGamma(vInteger, complex), M::normalizedLowerGamma(integer, complex));

    QCOMPARE(M::normalizedLowerGamma(vReal, boolean), M::normalizedLowerGamma(real, boolean));
    QCOMPARE(M::normalizedLowerGamma(vReal, integer), M::normalizedLowerGamma(real, integer));
    QCOMPARE(M::normalizedLowerGamma(vReal, real), M::normalizedLowerGamma(real, real));
    QCOMPARE(M::normalizedLowerGamma(vReal, complex), M::normalizedLowerGamma(real, complex));

    QCOMPARE(M::normalizedLowerGamma(vComplex, boolean), M::normalizedLowerGamma(complex, boolean));
    QCOMPARE(M::normalizedLowerGamma(vComplex, integer), M::normalizedLowerGamma(complex, integer));
    QCOMPARE(M::normalizedLowerGamma(vComplex, real), M::normalizedLowerGamma(complex, real));
    QCOMPARE(M::normalizedLowerGamma(vComplex, complex), M::normalizedLowerGamma(complex, complex));

    QCOMPARE(M::normalizedLowerGamma(integer, vBoolean), M::normalizedLowerGamma(integer, boolean));
    QCOMPARE(M::normalizedLowerGamma(integer, vInteger), M::normalizedLowerGamma(integer, integer));
    QCOMPARE(M::normalizedLowerGamma(integer, vReal), M::normalizedLowerGamma(integer, real));
    QCOMPARE(M::normalizedLowerGamma(integer, vComplex), M::normalizedLowerGamma(integer, complex));

    QCOMPARE(M::normalizedLowerGamma(real, vBoolean), M::normalizedLowerGamma(real, boolean));
    QCOMPARE(M::normalizedLowerGamma(real, vInteger), M::normalizedLowerGamma(real, integer));
    QCOMPARE(M::normalizedLowerGamma(real, vReal), M::normalizedLowerGamma(real, real));
    QCOMPARE(M::normalizedLowerGamma(real, vComplex), M::normalizedLowerGamma(real, complex));

    QCOMPARE(M::normalizedLowerGamma(complex, vBoolean), M::normalizedLowerGamma(complex, boolean));
    QCOMPARE(M::normalizedLowerGamma(complex, vInteger), M::normalizedLowerGamma(complex, integer));
    QCOMPARE(M::normalizedLowerGamma(complex, vReal), M::normalizedLowerGamma(complex, real));
    QCOMPARE(M::normalizedLowerGamma(complex, vComplex), M::normalizedLowerGamma(complex, complex));

    QCOMPARE(M::normalizedLowerGamma(vInteger, vBoolean), M::normalizedLowerGamma(integer, boolean));
    QCOMPARE(M::normalizedLowerGamma(vInteger, vInteger), M::normalizedLowerGamma(integer, integer));
    QCOMPARE(M::normalizedLowerGamma(vInteger, vReal), M::normalizedLowerGamma(integer, real));
    QCOMPARE(M::normalizedLowerGamma(vInteger, vComplex), M::normalizedLowerGamma(integer, complex));

    QCOMPARE(M::normalizedLowerGamma(vReal, vBoolean), M::normalizedLowerGamma(real, boolean));
    QCOMPARE(M::normalizedLowerGamma(vReal, vInteger), M::normalizedLowerGamma(real, integer));
    QCOMPARE(M::normalizedLowerGamma(vReal, vReal), M::normalizedLowerGamma(real, real));
    QCOMPARE(M::normalizedLowerGamma(vReal, vComplex), M::normalizedLowerGamma(real, complex));

    QCOMPARE(M::normalizedLowerGamma(vComplex, vBoolean), M::normalizedLowerGamma(complex, boolean));
    QCOMPARE(M::normalizedLowerGamma(vComplex, vInteger), M::normalizedLowerGamma(complex, integer));
    QCOMPARE(M::normalizedLowerGamma(vComplex, vReal), M::normalizedLowerGamma(complex, real));
    QCOMPARE(M::normalizedLowerGamma(vComplex, vComplex), M::normalizedLowerGamma(complex, complex));
}


void TestBasicFunctions::testNormalizedUpperGammaIntegerInteger() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> sGenerator(1, 100);
    std::uniform_int_distribution<M::Integer> zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer s = sGenerator(rng);
        M::Integer z = zGenerator(rng);

        M::Real measured = M::normalizedUpperGamma(s, z);
        M::Real expected = boost::math::gamma_q(static_cast<M::Real>(s), static_cast<M::Real>(z));

        M::Real error = measured - expected;
        if (expected == 0) {
            if (error >= 1.0E-12) {
                QCOMPARE(error < 1.0E-12, true);
            }
        } else {
            M::Real relativeError = std::abs(error) / std::abs(expected);

            if (relativeError >= 2.0E-13) {
                QCOMPARE(relativeError < 2.0E-13, true);
            }

            if (relativeError > maximumError) {
                maximumError = relativeError;
            }

            if (!std::isnan(relativeError)) {
                sumError += relativeError;
            } else {
                QCOMPARE(std::isnan(relativeError), false);
            }
        }
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "upperGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testNormalizedUpperGammaIntegerReal() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> sGenerator(1, 100);
    std::uniform_real_distribution<M::Real>   zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer s = sGenerator(rng);
        M::Real    z = zGenerator(rng);

        M::Real measured = M::normalizedUpperGamma(s, z);
        M::Real expected = boost::math::gamma_q(static_cast<M::Real>(s), static_cast<M::Real>(z));

        M::Real error         = measured - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        if (relativeError >= 2.0E-13) {
            QCOMPARE(relativeError < 2.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "upperGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testNormalizedUpperGammaIntegerComplex() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> sGenerator(1, 100);
    std::uniform_real_distribution<M::Real>   zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer s = sGenerator(rng);
        M::Complex z(zGenerator(rng), 0);

        M::Complex measured = M::normalizedUpperGamma(s, z);
        M::Real    expected = boost::math::gamma_q(static_cast<M::Real>(s), z.real());

        M::Real error         = measured.real() - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        QCOMPARE(measured.imag(), 0);
        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "upperGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testNormalizedUpperGammaRealInteger() {
    std::mt19937                              rng(0x12345678);
    std::uniform_real_distribution<M::Real>   sGenerator(0, 100);
    std::uniform_int_distribution<M::Integer> zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real    s = sGenerator(rng);
        M::Integer z = zGenerator(rng);

        M::Real measured = M::normalizedUpperGamma(s, z);
        M::Real expected = boost::math::gamma_q(s, static_cast<M::Real>(z));

        M::Real error         = measured - expected;
        if (expected == 0) {
            if (error >= 1.0E-12) {
                QCOMPARE(error < 1.0E-12, true);
            }
        } else {
            M::Real relativeError = std::abs(error) / std::abs(expected);

            if (relativeError >= 2.0E-13) {
                QCOMPARE(relativeError < 2.0E-13, true);
            }

            if (relativeError > maximumError) {
                maximumError = relativeError;
            }

            QCOMPARE(!std::isnan(relativeError), true);
            sumError += relativeError;
        }
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "upperGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testNormalizedUpperGammaRealReal() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> valueGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real s = valueGenerator(rng);
        M::Real z = valueGenerator(rng);

        M::Real measured = M::normalizedUpperGamma(s, z);
        M::Real expected = boost::math::gamma_q(s, z);

        M::Real error         = measured - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        if (relativeError >= 2.0E-13) {
            QCOMPARE(relativeError < 2.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "upperGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testNormalizedUpperGammaRealComplex() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> sGenerator(0, 100);
    std::uniform_real_distribution<M::Real> zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real    s = sGenerator(rng);
        M::Complex z(zGenerator(rng), 0);

        M::Complex measured = M::normalizedUpperGamma(s, z);
        M::Real    expected = boost::math::gamma_q(s, z.real());

        M::Real error         = measured.real() - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        QCOMPARE(measured.imag(), 0);
        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "upperGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testNormalizedUpperGammaComplexInteger() {
    std::mt19937                              rng(0x12345678);
    std::uniform_real_distribution<M::Real>   sGenerator(0, 100);
    std::uniform_int_distribution<M::Integer> zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex s(sGenerator(rng), 0);
        M::Integer z = zGenerator(rng);

        M::Complex measured = M::normalizedUpperGamma(s, z);
        M::Real    expected = boost::math::gamma_q(s.real(), static_cast<M::Real>(z));

        M::Real error         = measured.real() - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        QCOMPARE(measured.imag(), 0);
        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "upperGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testNormalizedUpperGammaComplexReal() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> sGenerator(0, 100);
    std::uniform_real_distribution<M::Real> zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex s(sGenerator(rng), 0);
        M::Real    z = zGenerator(rng);

        M::Complex measured = M::normalizedUpperGamma(s, z);
        M::Real    expected = boost::math::gamma_q(s.real(), z);

        M::Real error         = measured.real() - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        QCOMPARE(measured.imag(), 0);
        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "upperGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testNormalizedUpperGammaComplexComplex() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> sGenerator(0, 100);
    std::uniform_real_distribution<M::Real> zGenerator(0, 100);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex s(sGenerator(rng), 0);
        M::Complex z(zGenerator(rng), 0);

        M::Complex measured = M::normalizedUpperGamma(s, z);
        M::Real    expected = boost::math::gamma_q(s.real(), z.real());

        M::Real error         = measured.real() - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        QCOMPARE(measured.imag(), 0);
        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "upperGamma: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testNormalizedUpperGammaVariants() {
    M::Boolean boolean = true;
    M::Integer integer = 2;
    M::Real    real    = 3.0;
    M::Complex complex = M::Complex(4.0);

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);

    QCOMPARE(M::normalizedUpperGamma(vInteger, boolean), M::normalizedUpperGamma(integer, boolean));
    QCOMPARE(M::normalizedUpperGamma(vInteger, integer), M::normalizedUpperGamma(integer, integer));
    QCOMPARE(M::normalizedUpperGamma(vInteger, real), M::normalizedUpperGamma(integer, real));
    QCOMPARE(M::normalizedUpperGamma(vInteger, complex), M::normalizedUpperGamma(integer, complex));

    QCOMPARE(M::normalizedUpperGamma(vReal, boolean), M::normalizedUpperGamma(real, boolean));
    QCOMPARE(M::normalizedUpperGamma(vReal, integer), M::normalizedUpperGamma(real, integer));
    QCOMPARE(M::normalizedUpperGamma(vReal, real), M::normalizedUpperGamma(real, real));
    QCOMPARE(M::normalizedUpperGamma(vReal, complex), M::normalizedUpperGamma(real, complex));

    QCOMPARE(M::normalizedUpperGamma(vComplex, boolean), M::normalizedUpperGamma(complex, boolean));
    QCOMPARE(M::normalizedUpperGamma(vComplex, integer), M::normalizedUpperGamma(complex, integer));
    QCOMPARE(M::normalizedUpperGamma(vComplex, real), M::normalizedUpperGamma(complex, real));
    QCOMPARE(M::normalizedUpperGamma(vComplex, complex), M::normalizedUpperGamma(complex, complex));

    QCOMPARE(M::normalizedUpperGamma(integer, vBoolean), M::normalizedUpperGamma(integer, boolean));
    QCOMPARE(M::normalizedUpperGamma(integer, vInteger), M::normalizedUpperGamma(integer, integer));
    QCOMPARE(M::normalizedUpperGamma(integer, vReal), M::normalizedUpperGamma(integer, real));
    QCOMPARE(M::normalizedUpperGamma(integer, vComplex), M::normalizedUpperGamma(integer, complex));

    QCOMPARE(M::normalizedUpperGamma(real, vBoolean), M::normalizedUpperGamma(real, boolean));
    QCOMPARE(M::normalizedUpperGamma(real, vInteger), M::normalizedUpperGamma(real, integer));
    QCOMPARE(M::normalizedUpperGamma(real, vReal), M::normalizedUpperGamma(real, real));
    QCOMPARE(M::normalizedUpperGamma(real, vComplex), M::normalizedUpperGamma(real, complex));

    QCOMPARE(M::normalizedUpperGamma(complex, vBoolean), M::normalizedUpperGamma(complex, boolean));
    QCOMPARE(M::normalizedUpperGamma(complex, vInteger), M::normalizedUpperGamma(complex, integer));
    QCOMPARE(M::normalizedUpperGamma(complex, vReal), M::normalizedUpperGamma(complex, real));
    QCOMPARE(M::normalizedUpperGamma(complex, vComplex), M::normalizedUpperGamma(complex, complex));

    QCOMPARE(M::normalizedUpperGamma(vInteger, vBoolean), M::normalizedUpperGamma(integer, boolean));
    QCOMPARE(M::normalizedUpperGamma(vInteger, vInteger), M::normalizedUpperGamma(integer, integer));
    QCOMPARE(M::normalizedUpperGamma(vInteger, vReal), M::normalizedUpperGamma(integer, real));
    QCOMPARE(M::normalizedUpperGamma(vInteger, vComplex), M::normalizedUpperGamma(integer, complex));

    QCOMPARE(M::normalizedUpperGamma(vReal, vBoolean), M::normalizedUpperGamma(real, boolean));
    QCOMPARE(M::normalizedUpperGamma(vReal, vInteger), M::normalizedUpperGamma(real, integer));
    QCOMPARE(M::normalizedUpperGamma(vReal, vReal), M::normalizedUpperGamma(real, real));
    QCOMPARE(M::normalizedUpperGamma(vReal, vComplex), M::normalizedUpperGamma(real, complex));

    QCOMPARE(M::normalizedUpperGamma(vComplex, vBoolean), M::normalizedUpperGamma(complex, boolean));
    QCOMPARE(M::normalizedUpperGamma(vComplex, vInteger), M::normalizedUpperGamma(complex, integer));
    QCOMPARE(M::normalizedUpperGamma(vComplex, vReal), M::normalizedUpperGamma(complex, real));
    QCOMPARE(M::normalizedUpperGamma(vComplex, vComplex), M::normalizedUpperGamma(complex, complex));
}


void TestBasicFunctions::testInverseLowerGammaRealReal() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> sGenerator(0, 10);
    std::uniform_real_distribution<M::Real> zGenerator(0, 10);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real s  = sGenerator(rng);
        M::Real z  = zGenerator(rng);
        M::Real lg = M::lowerGamma(s, z);

        M::Real measured      = M::inverseLowerGamma(s, lg);
        M::Real error         = measured - z;
        M::Real relativeError = std::abs(error) / z;

        QCOMPARE(std::isnan(measured), false);

        if (z > 0.2) {
            if (relativeError >= 5.0E-10) {
                QCOMPARE(relativeError < 5.0E-10, true);
            }

            if (relativeError > maximumError) {
                maximumError = relativeError;
            }

            sumError += relativeError;
        } else if ( z > 0.1) {
            if (relativeError >= 1.0E-5) {
                QCOMPARE(relativeError < 1.0E-5, true);
            }
        } else {
            if (relativeError > 1.1) {
                QCOMPARE(relativeError < 1.1, true);
            }
        }
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "inverseLowerGamma: average error = " << averageError << ", maximum error = " << maximumError;

    QCOMPARE(
        M::inverseLowerGamma(M::Variant(M::Real(2)), M::Variant(M::Real(1))),
        M::inverseLowerGamma(M::Real(2.0), M::Real(1.0))
    );
}


void TestBasicFunctions::testBetaIntegerInteger() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> xGenerator(1, 500);
    std::uniform_int_distribution<M::Integer> yGenerator(1, 500);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer x = xGenerator(rng);
        M::Integer y = yGenerator(rng);

        M::Real measured = M::beta(x, y);
        M::Real expected = std::exp(M::lnGamma(M::Real(x)) + M::lnGamma(M::Real(y)) - M::lnGamma(M::Real(x + y)));

        M::Real error         = measured - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "beta: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testBetaIntegerReal() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> xGenerator(1, 500);
    std::uniform_real_distribution<M::Real>   yGenerator(1, 500);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer x = xGenerator(rng);
        M::Real    y = yGenerator(rng);

        M::Real measured = M::beta(x, y);
        M::Real expected = std::exp(M::lnGamma(M::Real(x)) + M::lnGamma(y) - M::lnGamma(M::Real(x) + y));

        M::Real error         = measured - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "beta: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testBetaIntegerComplex() {
    std::mt19937                              rng(0x12345678);
    std::uniform_int_distribution<M::Integer> xGenerator(1, 500);
    std::uniform_real_distribution<M::Real>   yGenerator(1, 500);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer x = xGenerator(rng);
        M::Complex y(yGenerator(rng));

        M::Complex           measured    = M::beta(x, y);
        M::Complex           exponent    = M::lnGamma(x) + M::lnGamma(y) - M::lnGamma(x + y);
        std::complex<double> stdExpected = std::exp(std::complex<double>(exponent.real(), exponent.imag()));
        M::Complex           expected(stdExpected.real(), stdExpected.imag());

        M::Complex error         = measured - expected;
        M::Real    relativeError = M::abs(error) / M::abs(expected);

        if (relativeError >= 2.0E-12) {
            QCOMPARE(relativeError < 2.0E-12, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "beta: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testBetaRealInteger() {
    std::mt19937                              rng(0x12345678);
    std::uniform_real_distribution<M::Real>   xGenerator(0, 500);
    std::uniform_int_distribution<M::Integer> yGenerator(1, 500);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real    x = xGenerator(rng);
        M::Integer y = yGenerator(rng);

        M::Real measured = M::beta(x, y);
        M::Real expected = std::exp(M::lnGamma(x) + M::lnGamma(M::Real(y)) - M::lnGamma(x + M::Real(y)));

        M::Real error         = measured - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "beta: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testBetaRealReal() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> xGenerator(0, 500);
    std::uniform_real_distribution<M::Real> yGenerator(0, 500);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real x = xGenerator(rng);
        M::Real y = yGenerator(rng);

        M::Real measured = M::beta(x, y);
        M::Real expected = std::exp(M::lnGamma(x) + M::lnGamma(y) - M::lnGamma(x + y));

        M::Real error         = measured - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        if (relativeError >= 5.0E-13) {
            QCOMPARE(relativeError < 5.0E-13, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "beta: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testBetaRealComplex() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> xGenerator(0, 500);
    std::uniform_real_distribution<M::Real> yGenerator(0, 500);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real x = xGenerator(rng);
        M::Complex y(yGenerator(rng));

        M::Complex           measured    = M::beta(x, y);
        M::Complex           exponent    = M::lnGamma(x) + M::lnGamma(y) - M::lnGamma(x + y);
        std::complex<double> stdExpected = std::exp(std::complex<double>(exponent.real(), exponent.imag()));
        M::Complex           expected(stdExpected.real(), stdExpected.imag());

        M::Complex error         = measured - expected;
        M::Real    relativeError = M::abs(error) / M::abs(expected);

        if (relativeError >= 2.0E-12) {
            QCOMPARE(relativeError < 2.0E-12, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "beta: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testBetaComplexInteger() {
    std::mt19937                              rng(0x12345678);
    std::uniform_real_distribution<M::Real>   xGenerator(0, 500);
    std::uniform_int_distribution<M::Integer> yGenerator(1, 500);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex x(xGenerator(rng), 0);
        M::Integer y = yGenerator(rng);

        M::Complex           measured    = M::beta(x, y);
        M::Complex           exponent    = M::lnGamma(x) + M::lnGamma(y) - M::lnGamma(x + y);
        std::complex<double> stdExpected = std::exp(std::complex<double>(exponent.real(), exponent.imag()));
        M::Complex           expected(stdExpected.real(), stdExpected.imag());

        M::Complex error         = measured - expected;
        M::Real    relativeError = M::abs(error) / M::abs(expected);

        if (relativeError >= 2.0E-12) {
            QCOMPARE(relativeError < 2.0E-12, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "beta: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testBetaComplexReal() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> xGenerator(0, 500);
    std::uniform_real_distribution<M::Real> yGenerator(0, 500);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex x(xGenerator(rng));
        M::Real    y = yGenerator(rng);

        M::Complex           measured    = M::beta(x, y);
        M::Complex           exponent    = M::lnGamma(x) + M::lnGamma(y) - M::lnGamma(x + y);
        std::complex<double> stdExpected = std::exp(std::complex<double>(exponent.real(), exponent.imag()));
        M::Complex           expected(stdExpected.real(), stdExpected.imag());

        M::Complex error         = measured - expected;
        M::Real    relativeError = M::abs(error) / M::abs(expected);

        if (relativeError >= 2.0E-12) {
            QCOMPARE(relativeError < 2.0E-12, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "beta: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testBetaComplexComplex() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> xGenerator(0, 500);
    std::uniform_real_distribution<M::Real> yGenerator(0, 500);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex x(xGenerator(rng));
        M::Complex y(yGenerator(rng));

        M::Complex           measured    = M::beta(x, y);
        M::Complex           exponent    = M::lnGamma(x) + M::lnGamma(y) - M::lnGamma(x + y);
        std::complex<double> stdExpected = std::exp(std::complex<double>(exponent.real(), exponent.imag()));
        M::Complex           expected(stdExpected.real(), stdExpected.imag());

        M::Complex error         = measured - expected;
        M::Real    relativeError = M::abs(error) / M::abs(expected);

        if (relativeError >= 2.0E-12) {
            QCOMPARE(relativeError < 2.0E-12, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "beta: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testBetaVariants() {
    M::Boolean boolean = true;
    M::Integer integer = 2;
    M::Real    real    = 3.0;
    M::Complex complex = M::Complex(4.0);

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);

    QCOMPARE(M::beta(vInteger, boolean), M::beta(integer, boolean));
    QCOMPARE(M::beta(vInteger, integer), M::beta(integer, integer));
    QCOMPARE(M::beta(vInteger, real), M::beta(integer, real));
    QCOMPARE(M::beta(vInteger, complex), M::beta(integer, complex));

    QCOMPARE(M::beta(vReal, boolean), M::beta(real, boolean));
    QCOMPARE(M::beta(vReal, integer), M::beta(real, integer));
    QCOMPARE(M::beta(vReal, real), M::beta(real, real));
    QCOMPARE(M::beta(vReal, complex), M::beta(real, complex));

    QCOMPARE(M::beta(vComplex, boolean), M::beta(complex, boolean));
    QCOMPARE(M::beta(vComplex, integer), M::beta(complex, integer));
    QCOMPARE(M::beta(vComplex, real), M::beta(complex, real));
    QCOMPARE(M::beta(vComplex, complex), M::beta(complex, complex));

    QCOMPARE(M::beta(integer, vBoolean), M::beta(integer, boolean));
    QCOMPARE(M::beta(integer, vInteger), M::beta(integer, integer));
    QCOMPARE(M::beta(integer, vReal), M::beta(integer, real));
    QCOMPARE(M::beta(integer, vComplex), M::beta(integer, complex));

    QCOMPARE(M::beta(real, vBoolean), M::beta(real, boolean));
    QCOMPARE(M::beta(real, vInteger), M::beta(real, integer));
    QCOMPARE(M::beta(real, vReal), M::beta(real, real));
    QCOMPARE(M::beta(real, vComplex), M::beta(real, complex));

    QCOMPARE(M::beta(complex, vBoolean), M::beta(complex, boolean));
    QCOMPARE(M::beta(complex, vInteger), M::beta(complex, integer));
    QCOMPARE(M::beta(complex, vReal), M::beta(complex, real));
    QCOMPARE(M::beta(complex, vComplex), M::beta(complex, complex));

    QCOMPARE(M::beta(vInteger, vBoolean), M::beta(integer, boolean));
    QCOMPARE(M::beta(vInteger, vInteger), M::beta(integer, integer));
    QCOMPARE(M::beta(vInteger, vReal), M::beta(integer, real));
    QCOMPARE(M::beta(vInteger, vComplex), M::beta(integer, complex));

    QCOMPARE(M::beta(vReal, vBoolean), M::beta(real, boolean));
    QCOMPARE(M::beta(vReal, vInteger), M::beta(real, integer));
    QCOMPARE(M::beta(vReal, vReal), M::beta(real, real));
    QCOMPARE(M::beta(vReal, vComplex), M::beta(real, complex));

    QCOMPARE(M::beta(vComplex, vBoolean), M::beta(complex, boolean));
    QCOMPARE(M::beta(vComplex, vInteger), M::beta(complex, integer));
    QCOMPARE(M::beta(vComplex, vReal), M::beta(complex, real));
    QCOMPARE(M::beta(vComplex, vComplex), M::beta(complex, complex));
}


void TestBasicFunctions::testZetaInteger() {
    M::PerThread::RngSeed rngSeed = { 0, 0, 0, 0};
    M::PerThread pt(0, M::PerThread::RngType::XORSHIRO256_STARS, rngSeed, nullptr, nullptr);

    THRESHOLD_COMPARE(M::pi * M::pi / 6.0, M::reimannZeta(pt, M::Integer(2)), 2 * M::epsilon);
}


void TestBasicFunctions::testZetaReal() {
    M::PerThread::RngSeed rngSeed = { 0, 0, 0, 0};
    M::PerThread pt(0, M::PerThread::RngType::XORSHIRO256_STARS, rngSeed, nullptr, nullptr);

    THRESHOLD_COMPARE(M::pi * M::pi / 6.0, M::reimannZeta(pt, M::Real(2)), 2 * M::epsilon);
    THRESHOLD_COMPARE(1.2020569031595942853, M::reimannZeta(pt, M::Real(3)), 2 * M::epsilon);
    THRESHOLD_COMPARE(M::pi * M::pi * M::pi * M::pi / 90, M::reimannZeta(pt, M::Real(4)), 2 * M::epsilon);
    THRESHOLD_COMPARE(1.0369277551433699263, M::reimannZeta(pt, M::Real(5)), 2 * M::epsilon);

    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<long double> realGenerator(2.0, 50.0);

    for (unsigned i=0 ; i<100 ; ++i) {
        M::Real s             = realGenerator(rng);
        M::Real expected      = testZeta(s);
        M::Real measured      = M::reimannZeta(pt, s);
        M::Real relativeError = M::abs(expected - measured) / M::abs(expected);

        if (relativeError > 4.E-6) {
            qDebug() << "s = " << s
                     << " expected = " << expected
                     << " measured = " << measured
                     << " relativeError = " << relativeError;

            QCOMPARE(relativeError <= 4.0E-6, true);
        }
    }
}


void TestBasicFunctions::testZetaComplex() {
    M::PerThread::RngSeed rngSeed = { 0, 0, 0, 0};
    M::PerThread pt(0, M::PerThread::RngType::XORSHIRO256_STARS, rngSeed, nullptr, nullptr);

    // We first test by comparing against our real version over very slightly complex values.

    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<long double> realGenerator1(1.01, 50.0);
    std::uniform_real_distribution<long double> realGenerator2(0.01, 0.99);
    std::uniform_real_distribution<long double> realGenerator3(-4.0, -0.01);
    std::uniform_real_distribution<long double> imagGenerator(-2.0 * M::pi, +2.0 * M::pi);

    for (unsigned i=0 ; i<10000 ; ++i) {
        M::Real    s             = realGenerator1(rng);
        M::Real    expected      = M::reimannZeta(pt, s);
        M::Complex measured      = M::reimannZeta(pt, M::Complex(s, std::numeric_limits<M::Complex::T>::epsilon()));
        M::Real    relativeError = M::abs(expected - measured) / M::abs(expected);
        M::Real    errorLimit    = s <= 2.0 ? std::pow(10.0, -std::log10(s - 1) - 14.0) : 1.0E-14;

        if (relativeError > errorLimit) {
            qDebug() << "s = " << s
                     << " expected = " << expected
                     << " measured = (" << measured.real() << ","<< measured.imag() << ")"
                     << " relativeError = " << relativeError
                     << " errorLimit = " << errorLimit;

            QCOMPARE(relativeError <= errorLimit, true);
        }
    }

    for (unsigned i=0 ; i<10000 ; ++i) {
        M::Real    s             = realGenerator2(rng);
        M::Real    expected      = M::reimannZeta(pt, s);
        M::Complex measured      = M::reimannZeta(pt, M::Complex(s, std::numeric_limits<M::Complex::T>::epsilon()));
        M::Real    relativeError = M::abs(expected - measured) / M::abs(expected);
        M::Real    errorLimit    = 1.0E-13; // We have a bit more error very close to 1.0

        if (relativeError > errorLimit) {
            qDebug() << "s = " << s
                     << " expected = " << expected
                     << " measured = (" << measured.real() << ","<< measured.imag() << ")"
                     << " relativeError = " << relativeError
                     << " errorLimit = " << errorLimit;

            QCOMPARE(relativeError <= errorLimit, true);
        }
    }

    for (unsigned i=0 ; i<10000 ; ++i) {
        M::Real    s             = realGenerator3(rng);
        M::Real    expected      = M::reimannZeta(pt, s);
        M::Complex measured      = M::reimannZeta(pt, M::Complex(s, std::numeric_limits<M::Complex::T>::epsilon()));
        M::Real    relativeError = M::abs(expected - measured) / M::abs(expected);
        M::Real    errorLimit    = 1.0E-11;

        if (relativeError > errorLimit) {
            qDebug() << "s = " << s
                     << " expected = " << expected
                     << " measured = (" << measured.real() << ","<< measured.imag() << ")"
                     << " relativeError = " << relativeError
                     << " errorLimit = " << errorLimit;

            QCOMPARE(relativeError <= errorLimit, true);
        }
    }

    // Next we check our solution against our complex test version.  Note that the complex test version takes a lot of
    // iterations so it's slow and there tends to be error accumulation which means we need wider error bars.

    for (unsigned i=0 ; i<100 ; ++i) {
        M::Complex s             = M::Complex(realGenerator1(rng), imagGenerator(rng));
        M::Complex expected      = testZeta(s);
        M::Complex measured      = M::reimannZeta(pt, s);
        M::Real    relativeError = M::abs(expected - measured) / M::abs(expected);
        M::Real    errorLimit    = 2.0E-5;

        if (relativeError > errorLimit) {
            qDebug() << "s = (" << s.real() << "," << s.imag() << ")"
                     << " expected = (" << expected.real() << "," << expected.imag() << ")"
                     << " measured = (" << measured.real() << ","<< measured.imag() << ")"
                     << " relativeError = " << relativeError
                     << " errorLimit = " << errorLimit;

            QCOMPARE(relativeError <= errorLimit, true);
        }
    }
}


void TestBasicFunctions::testZetaVariant() {
    M::PerThread::RngSeed rngSeed = { 0, 0, 0, 0};
    M::PerThread pt(0, M::PerThread::RngType::XORSHIRO256_STARS, rngSeed, nullptr, nullptr);

    M::Integer i  = 2;
    M::Real    r  = 3;
    M::Complex c  = M::Complex(4, 1);
    M::Variant vi(i);
    M::Variant vr(r);
    M::Variant vc(c);

    QCOMPARE(M::reimannZeta(pt, i), M::reimannZeta(pt, vi));
    QCOMPARE(M::reimannZeta(pt, r), M::reimannZeta(pt, vr));
    QCOMPARE(M::reimannZeta(pt, c), M::reimannZeta(pt, vc));
}


void TestBasicFunctions::testErfInteger() {
    M::Real measured = M::erf(M::Integer(1));
    M::Real expected = boost::math::erf(1.0);

    M::Real relativeError = std::abs(measured - expected) / std::abs(expected);
    QCOMPARE(relativeError < 1.0E-15, true);
}


void TestBasicFunctions::testErfReal() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> xGenerator(-5, 5);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real x = xGenerator(rng);

        M::Real measured = M::erf(x);
        M::Real expected = boost::math::erf(x);

        M::Real error         = measured - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        if (relativeError >= 1.0E-15) {
            QCOMPARE(relativeError < 1.0E-15, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "beta: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testErfComplex() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> xGenerator(-5, 5);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex x(xGenerator(rng));

        M::Complex measured = M::erf(x);
        M::Real    expected = boost::math::erf(x.real());

        QCOMPARE(measured.imag(), 0);
        M::Real error         = measured.real() - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        if (relativeError >= 1.0E-15) {
            QCOMPARE(relativeError < 1.0E-15, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "beta: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testErfVariant() {
    M::Boolean boolean = true;
    M::Integer integer = 2;
    M::Real    real    = 3.0;
    M::Complex complex = M::Complex(4.0);

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);

    QCOMPARE(M::erf(vBoolean), M::erf(boolean));
    QCOMPARE(M::erf(vInteger), M::erf(integer));
    QCOMPARE(M::erf(vReal), M::erf(real));
    QCOMPARE(M::erf(vComplex), M::erf(complex));
}


void TestBasicFunctions::testErfcInteger() {
    M::Real measured = M::erfc(M::Integer(1));
    M::Real expected = boost::math::erfc(1.0);

    M::Real relativeError = std::abs(measured - expected) / std::abs(expected);
    QCOMPARE(relativeError < 1.0E-15, true);
}


void TestBasicFunctions::testErfcReal() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> xGenerator(-5, 5);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real x = xGenerator(rng);

        M::Real measured = M::erfc(x);
        M::Real expected = boost::math::erfc(x);

        M::Real error         = measured - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        if (relativeError >= 5.0E-15) {
            QCOMPARE(relativeError < 5.0E-15, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "beta: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testErfcComplex() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> xGenerator(-5, 5);

    M::Real sumError     = 0;
    M::Real maximumError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Complex x(xGenerator(rng));

        M::Complex measured = M::erfc(x);
        M::Real    expected = boost::math::erfc(x.real());

        QCOMPARE(measured.imag(), 0);

        M::Real error         = measured.real() - expected;
        M::Real relativeError = std::abs(error) / std::abs(expected);

        if (relativeError >= 5.0E-15) {
            QCOMPARE(relativeError < 5.0E-15, true);
        }

        if (relativeError > maximumError) {
            maximumError = relativeError;
        }

        sumError += relativeError;
    }

    M::Real averageError = sumError / numberTestIterations;
    qDebug() << "beta: average error = " << averageError << ", maximum error = " << maximumError;
}


void TestBasicFunctions::testErfcVariant() {
    M::Boolean boolean = true;
    M::Integer integer = 2;
    M::Real    real    = 3.0;
    M::Complex complex = M::Complex(4.0);

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);

    QCOMPARE(M::erfc(vBoolean), M::erfc(boolean));
    QCOMPARE(M::erfc(vInteger), M::erfc(integer));
    QCOMPARE(M::erfc(vReal), M::erfc(real));
    QCOMPARE(M::erfc(vComplex), M::erfc(complex));
}


void TestBasicFunctions::testErfInvReal() {
    std::mt19937                            rng(0x12345678);
    std::uniform_real_distribution<M::Real> yGenerator(-1.0, 1.0);

    M::Real maximumRelativeError = 0;
    M::Real maximumAbsoluteError = 0;

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Real expected;
        do {
            expected = yGenerator(rng);
        } while (expected == -1.0 || expected == 1.0);

        M::Real x        = M::erf(expected);
        M::Real measured = M::erfInv(x);

        M::Real error = measured - expected;
        if (expected != 0) {
            M::Real relativeError = std::abs(error / expected);

            if (relativeError > maximumRelativeError) {
                maximumRelativeError = relativeError;
            }

            QCOMPARE(relativeError < 2.0E-15, true);
        } else {
            error = std::abs(error);
            if (error > maximumAbsoluteError) {
                maximumAbsoluteError = error;
            }

            QCOMPARE(error < 2.0E-15, true);
        }
    }
}


void TestBasicFunctions::testErfInvVariants() {
    M::Boolean boolean = true;
    M::Integer integer = 2;
    M::Real    real    = 3.0;
    M::Complex complex = M::Complex(4.0);

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);

    QCOMPARE(M::erfInv(vBoolean), M::erfInv(boolean));
    QCOMPARE(M::erfInv(vInteger), M::erfInv(integer));
    QCOMPARE(M::erfInv(vReal), M::erfInv(real));
    QCOMPARE(M::erfInv(vComplex), M::erfInv(complex));
}


void TestBasicFunctions::testLambertWIntegerInteger() {
    M::Real maximumAllowedRelativeError = 1.0E-15;
    M::Real maximumAllowedAbsoluteError = 1.0E-15;

    M::Complex measured = M::lambertW(M::Integer(0), M::Integer(0));
    M::Complex expected = M::Complex(0, 0);
    M::Real absoluteError = M::abs(measured - expected);
    QCOMPARE(absoluteError < maximumAllowedAbsoluteError, true);

    measured = M::lambertW(M::Integer(0), M::Integer(1));
    expected = M::Complex( 0.56714329040978384, 0);
    M::Real relativeError = M::abs(measured - expected) / M::abs(expected);
    QCOMPARE(relativeError < maximumAllowedRelativeError, true);
}


void TestBasicFunctions::testLambertWIntegerReal() {
    M::Real maximumAllowedRelativeError = 1.0E-15;
    M::Real maximumAllowedAbsoluteError = 1.0E-15;

    M::Complex measured = M::lambertW(M::Integer(0), M::Real(-M::pi/2.0));
    M::Complex expected = M::Complex(0, M::pi/2.0);
    M::Real relativeError = M::abs(measured - expected) / M::abs(expected);
    QCOMPARE(relativeError < maximumAllowedRelativeError, true);

    measured = M::lambertW(M::Integer(0), M::Real(-1.0/M::e));
    expected = M::Complex(-1, 0);
    relativeError = M::abs(measured - expected) / M::abs(expected);
    QCOMPARE(relativeError < 1.0E-7, true); // Increased error near -1.0/e should be expected.

    measured = M::lambertW(M::Integer(0), M::Real(2.0 * M::ln(2.0).real()));
    expected = M::Complex(0.693147180559945286, 0);
    relativeError = M::abs(measured - expected) / M::abs(expected);
    QCOMPARE(relativeError < maximumAllowedRelativeError, true);

    measured = M::lambertW(M::Integer(0), M::Real(0.0));
    expected = M::Complex(0, 0);
    M::Real absoluteError = M::abs(measured - expected);
    QCOMPARE(absoluteError < maximumAllowedAbsoluteError, true);

    measured = M::lambertW(M::Integer(0), M::Real(1.0));
    expected = M::Complex( 0.56714329040978384, 0);
    relativeError = M::abs(measured - expected) / M::abs(expected);
    QCOMPARE(relativeError < maximumAllowedRelativeError, true);

    measured = M::lambertW(M::Integer(-1), M::Real(-1.0 / (2.0 * M::e)));
    expected = M::Complex(-2.67834699001666054, 0);
    relativeError = M::abs(measured - expected) / M::abs(expected);
    QCOMPARE(relativeError < maximumAllowedRelativeError, true);

    measured = M::lambertW(M::Integer(-1), M::Real(1.0 / (2.0 * M::e)));
    expected = M::Complex(-3.34746556346020974, -4.01774723801199141);
    relativeError = M::abs(measured - expected) / M::abs(expected);
    QCOMPARE(relativeError < maximumAllowedRelativeError, true);
}


void TestBasicFunctions::testLambertWIntegerComplex() {
    std::mt19937                              rng(0x12345678);
    std::uniform_real_distribution<M::Real>   wGenerator(-5, 5);
    std::uniform_int_distribution<M::Integer> kGenerator(-5, 5);

    M::Real sumError                         = 0;
    M::Real maximumRelativeErrorNearBoundary = 0;
    M::Real maximumRelativeError             = 0;
    M::Real maximumAbsoluteError             = 0;

    // For tests below, we test backwards, derive w from a value of z and then reverse the Lambert W operator to
    // derive a measued "z" term.  We then compare those terms.

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer k = kGenerator(rng);
        M::Complex expected(wGenerator(rng), 0.0);

        M::Complex w        = M::lambertW(k, expected);
        M::Complex measured = w * M::pow(M::e, w);

        M::Real error = M::abs(expected - measured);

        if (expected.real() == 0 && expected.imag() == 0) {
            if (error > maximumAbsoluteError) {
                maximumAbsoluteError = error;
            }
        } else {
            M::Real relativeError = error / M::abs(expected);

            if (M::abs(expected.real() - (-1.0/M::e)) < 0.1) {
                QCOMPARE(relativeError < 1.0E-7, true);

                if (relativeError > maximumRelativeErrorNearBoundary) {
                    maximumRelativeErrorNearBoundary = relativeError;
                }
            } else {
                QCOMPARE(relativeError < 1.0E-14, true);

                if (relativeError > maximumRelativeError) {
                    maximumRelativeError = relativeError;
                }
            }

            sumError += relativeError;
        }
    }

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer k = kGenerator(rng);
        M::Complex expected(wGenerator(rng), kGenerator(rng));

        M::Complex w        = M::lambertW(k, expected);
        M::Complex measured = w * M::pow(M::e, w);

        M::Real error = M::abs(expected - measured);

        if (expected.real() == 0 && expected.imag() == 0) {
            if (error > maximumAbsoluteError) {
                maximumAbsoluteError = error;
            }
        } else {
            M::Real relativeError = error / M::abs(expected);

            if (M::abs(expected.real() - (-1.0/M::e)) < 0.1) {
                QCOMPARE(relativeError < 1.0E-7, true);

                if (relativeError > maximumRelativeErrorNearBoundary) {
                    maximumRelativeErrorNearBoundary = relativeError;
                }
            } else {
                QCOMPARE(relativeError < 1.0E-14, true);

                if (relativeError > maximumRelativeError) {
                    maximumRelativeError = relativeError;
                }
            }

            sumError += relativeError;
        }
    }

    M::Real averageError = sumError / (2.0 * numberTestIterations);
    qDebug() << "lambert W: average error = " << averageError
             << ", maximum absolute error = " << maximumAbsoluteError
             << ", maximum relative error = " << maximumRelativeError
             << ", maximum relative error near boundary = " << maximumRelativeErrorNearBoundary;
}


void TestBasicFunctions::testLambertWRealInteger() {
    M::Real maximumAllowedRelativeError = 1.0E-15;
    M::Real maximumAllowedAbsoluteError = 1.0E-15;

    M::Complex measured = M::lambertW(M::Real(0), M::Integer(0));
    M::Complex expected = M::Complex(0, 0);
    M::Real absoluteError = M::abs(measured - expected);
    QCOMPARE(absoluteError < maximumAllowedAbsoluteError, true);

    measured = M::lambertW(M::Real(0), M::Integer(1));
    expected = M::Complex( 0.56714329040978384, 0);
    M::Real relativeError = M::abs(measured - expected) / M::abs(expected);
    QCOMPARE(relativeError < maximumAllowedRelativeError, true);

    measured = M::lambertW(M::Real(1.5), M::Integer(0));
    QCOMPARE(std::isnan(measured.real()) && std::isnan(measured.imag()), true);
}


void TestBasicFunctions::testLambertWRealReal() {
    M::Real maximumAllowedRelativeError = 1.0E-15;
    M::Real maximumAllowedAbsoluteError = 1.0E-15;

    M::Complex measured = M::lambertW(M::Real(0), M::Real(-M::pi/2.0));
    M::Complex expected = M::Complex(0, M::pi/2.0);
    M::Real relativeError = M::abs(measured - expected) / M::abs(expected);
    QCOMPARE(relativeError < maximumAllowedRelativeError, true);

    measured = M::lambertW(M::Real(0), M::Real(-1.0/M::e));
    expected = M::Complex(-1, 0);
    relativeError = M::abs(measured - expected) / M::abs(expected);
    QCOMPARE(relativeError < 1.0E-7, true); // Increased error near -1.0/e should be expected.

    measured = M::lambertW(M::Real(0), M::Real(2.0 * M::ln(2.0).real()));
    expected = M::Complex(0.693147180559945286, 0);
    relativeError = M::abs(measured - expected) / M::abs(expected);
    QCOMPARE(relativeError < maximumAllowedRelativeError, true);

    measured = M::lambertW(M::Real(0), M::Real(0.0));
    expected = M::Complex(0, 0);
    M::Real absoluteError = M::abs(measured - expected);
    QCOMPARE(absoluteError < maximumAllowedAbsoluteError, true);

    measured = M::lambertW(M::Real(0), M::Real(1.0));
    expected = M::Complex( 0.56714329040978384, 0);
    relativeError = M::abs(measured - expected) / M::abs(expected);
    QCOMPARE(relativeError < maximumAllowedRelativeError, true);

    measured = M::lambertW(M::Real(-1), M::Real(-1.0 / (2.0 * M::e)));
    expected = M::Complex(-2.67834699001666054, 0);
    relativeError = M::abs(measured - expected) / M::abs(expected);
    QCOMPARE(relativeError < maximumAllowedRelativeError, true);

    measured = M::lambertW(M::Real(-1), M::Real(1.0 / (2.0 * M::e)));
    expected = M::Complex(-3.34746556346020974, -4.01774723801199141);
    relativeError = M::abs(measured - expected) / M::abs(expected);
    QCOMPARE(relativeError < maximumAllowedRelativeError, true);

    measured = M::lambertW(M::Real(1.5), M::Real(0));
    QCOMPARE(std::isnan(measured.real()) && std::isnan(measured.imag()), true);
}


void TestBasicFunctions::testLambertWRealComplex() {
    std::mt19937                              rng(0x12345678);
    std::uniform_real_distribution<M::Real>   wGenerator(-5, 5);
    std::uniform_int_distribution<M::Integer> kGenerator(-5, 5);

    M::Real sumError                         = 0;
    M::Real maximumRelativeErrorNearBoundary = 0;
    M::Real maximumRelativeError             = 0;
    M::Real maximumAbsoluteError             = 0;

    // For tests below, we test backwards, derive w from a value of z and then reverse the Lambert W operator to
    // derive a measued "z" term.  We then compare those terms.

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer k = kGenerator(rng);
        M::Complex expected(wGenerator(rng), 0.0);

        M::Complex w        = M::lambertW(M::Real(k), expected);
        M::Complex measured = w * M::pow(M::e, w);

        M::Real error = M::abs(expected - measured);

        if (expected.real() == 0 && expected.imag() == 0) {
            if (error > maximumAbsoluteError) {
                maximumAbsoluteError = error;
            }
        } else {
            M::Real relativeError = error / M::abs(expected);

            if (M::abs(expected.real() - (-1.0/M::e)) < 0.1) {
                QCOMPARE(relativeError < 1.0E-7, true);

                if (relativeError > maximumRelativeErrorNearBoundary) {
                    maximumRelativeErrorNearBoundary = relativeError;
                }
            } else {
                QCOMPARE(relativeError < 1.0E-14, true);

                if (relativeError > maximumRelativeError) {
                    maximumRelativeError = relativeError;
                }
            }

            sumError += relativeError;
        }
    }

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer k = kGenerator(rng);
        M::Complex expected(wGenerator(rng), kGenerator(rng));

        M::Complex w        = M::lambertW(M::Real(k), expected);
        M::Complex measured = w * M::pow(M::e, w);

        M::Real error = M::abs(expected - measured);

        if (expected.real() == 0 && expected.imag() == 0) {
            if (error > maximumAbsoluteError) {
                maximumAbsoluteError = error;
            }
        } else {
            M::Real relativeError = error / M::abs(expected);

            if (M::abs(expected.real() - (-1.0/M::e)) < 0.1) {
                QCOMPARE(relativeError < 1.0E-7, true);

                if (relativeError > maximumRelativeErrorNearBoundary) {
                    maximumRelativeErrorNearBoundary = relativeError;
                }
            } else {
                QCOMPARE(relativeError < 1.0E-14, true);

                if (relativeError > maximumRelativeError) {
                    maximumRelativeError = relativeError;
                }
            }

            sumError += relativeError;
        }
    }

    M::Complex measured = M::lambertW(M::Real(1.5), M::Complex(0));
    QCOMPARE(std::isnan(measured.real()) && std::isnan(measured.imag()), true);

    M::Real averageError = sumError / (2.0 * numberTestIterations);
    qDebug() << "lambert W: average error = " << averageError
             << ", maximum absolute error = " << maximumAbsoluteError
             << ", maximum relative error = " << maximumRelativeError
             << ", maximum relative error near boundary = " << maximumRelativeErrorNearBoundary;
}


void TestBasicFunctions::testLambertWComplexInteger() {
    M::Real maximumAllowedRelativeError = 1.0E-15;
    M::Real maximumAllowedAbsoluteError = 1.0E-15;

    M::Complex measured = M::lambertW(M::Real(0), M::Integer(0));
    M::Complex expected = M::Complex(0, 0);
    M::Real absoluteError = M::abs(measured - expected);
    QCOMPARE(absoluteError < maximumAllowedAbsoluteError, true);

    measured = M::lambertW(M::Complex(0), M::Integer(1));
    expected = M::Complex( 0.56714329040978384, 0);
    M::Real relativeError = M::abs(measured - expected) / M::abs(expected);
    QCOMPARE(relativeError < maximumAllowedRelativeError, true);

    measured = M::lambertW(M::Complex(1.5), M::Integer(0));
    QCOMPARE(std::isnan(measured.real()) && std::isnan(measured.imag()), true);

    measured = M::lambertW(M::Complex(0, 1), M::Integer(0));
    QCOMPARE(std::isnan(measured.real()) && std::isnan(measured.imag()), true);
}


void TestBasicFunctions::testLambertWComplexReal() {
    M::Real maximumAllowedRelativeError = 1.0E-15;
    M::Real maximumAllowedAbsoluteError = 1.0E-15;

    M::Complex measured = M::lambertW(M::Complex(0), M::Real(-M::pi/2.0));
    M::Complex expected = M::Complex(0, M::pi/2.0);
    M::Real relativeError = M::abs(measured - expected) / M::abs(expected);
    QCOMPARE(relativeError < maximumAllowedRelativeError, true);

    measured = M::lambertW(M::Complex(0), M::Real(-1.0/M::e));
    expected = M::Complex(-1, 0);
    relativeError = M::abs(measured - expected) / M::abs(expected);
    QCOMPARE(relativeError < 1.0E-7, true); // Increased error near -1.0/e should be expected.

    measured = M::lambertW(M::Complex(0), M::Real(2.0 * M::ln(2.0).real()));
    expected = M::Complex(0.693147180559945286, 0);
    relativeError = M::abs(measured - expected) / M::abs(expected);
    QCOMPARE(relativeError < maximumAllowedRelativeError, true);

    measured = M::lambertW(M::Complex(0), M::Real(0.0));
    expected = M::Complex(0, 0);
    M::Real absoluteError = M::abs(measured - expected);
    QCOMPARE(absoluteError < maximumAllowedAbsoluteError, true);

    measured = M::lambertW(M::Complex(0), M::Real(1.0));
    expected = M::Complex( 0.56714329040978384, 0);
    relativeError = M::abs(measured - expected) / M::abs(expected);
    QCOMPARE(relativeError < maximumAllowedRelativeError, true);

    measured = M::lambertW(M::Complex(-1), M::Real(-1.0 / (2.0 * M::e)));
    expected = M::Complex(-2.67834699001666054, 0);
    relativeError = M::abs(measured - expected) / M::abs(expected);
    QCOMPARE(relativeError < maximumAllowedRelativeError, true);

    measured = M::lambertW(M::Complex(-1), M::Real(1.0 / (2.0 * M::e)));
    expected = M::Complex(-3.34746556346020974, -4.01774723801199141);
    relativeError = M::abs(measured - expected) / M::abs(expected);
    QCOMPARE(relativeError < maximumAllowedRelativeError, true);

    measured = M::lambertW(M::Complex(1.5), M::Real(0));
    QCOMPARE(std::isnan(measured.real()) && std::isnan(measured.imag()), true);

    measured = M::lambertW(M::Complex(0, 1), M::Real(0));
    QCOMPARE(std::isnan(measured.real()) && std::isnan(measured.imag()), true);
}


void TestBasicFunctions::testLambertWComplexComplex() {
    std::mt19937                              rng(0x12345678);
    std::uniform_real_distribution<M::Real>   wGenerator(-5, 5);
    std::uniform_int_distribution<M::Integer> kGenerator(-5, 5);

    M::Real sumError                         = 0;
    M::Real maximumRelativeErrorNearBoundary = 0;
    M::Real maximumRelativeError             = 0;
    M::Real maximumAbsoluteError             = 0;

    // For tests below, we test backwards, derive w from a value of z and then reverse the Lambert W operator to
    // derive a measued "z" term.  We then compare those terms.

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer k = kGenerator(rng);
        M::Complex expected(wGenerator(rng), 0.0);

        M::Complex w        = M::lambertW(M::Complex(k), expected);
        M::Complex measured = w * M::pow(M::e, w);

        M::Real error = M::abs(expected - measured);

        if (expected.real() == 0 && expected.imag() == 0) {
            if (error > maximumAbsoluteError) {
                maximumAbsoluteError = error;
            }
        } else {
            M::Real relativeError = error / M::abs(expected);

            if (M::abs(expected.real() - (-1.0/M::e)) < 0.1) {
                QCOMPARE(relativeError < 1.0E-7, true);

                if (relativeError > maximumRelativeErrorNearBoundary) {
                    maximumRelativeErrorNearBoundary = relativeError;
                }
            } else {
                QCOMPARE(relativeError < 1.0E-14, true);

                if (relativeError > maximumRelativeError) {
                    maximumRelativeError = relativeError;
                }
            }

            sumError += relativeError;
        }
    }

    for (unsigned i=0 ; i<numberTestIterations ; ++i) {
        M::Integer k = kGenerator(rng);
        M::Complex expected(wGenerator(rng), kGenerator(rng));

        M::Complex w        = M::lambertW(M::Complex(k), expected);
        M::Complex measured = w * M::pow(M::e, w);

        M::Real error = M::abs(expected - measured);

        if (expected.real() == 0 && expected.imag() == 0) {
            if (error > maximumAbsoluteError) {
                maximumAbsoluteError = error;
            }
        } else {
            M::Real relativeError = error / M::abs(expected);

            if (M::abs(expected.real() - (-1.0/M::e)) < 0.1) {
                QCOMPARE(relativeError < 1.0E-7, true);

                if (relativeError > maximumRelativeErrorNearBoundary) {
                    maximumRelativeErrorNearBoundary = relativeError;
                }
            } else {
                QCOMPARE(relativeError < 1.0E-14, true);

                if (relativeError > maximumRelativeError) {
                    maximumRelativeError = relativeError;
                }
            }

            sumError += relativeError;
        }
    }

    M::Complex measured = M::lambertW(M::Complex(1.5), M::Real(0));
    QCOMPARE(std::isnan(measured.real()) && std::isnan(measured.imag()), true);

    measured = M::lambertW(M::Complex(0, 1), M::Real(0));
    QCOMPARE(std::isnan(measured.real()) && std::isnan(measured.imag()), true);

    M::Real averageError = sumError / (2.0 * numberTestIterations);
    qDebug() << "lambert W: average error = " << averageError
             << ", maximum absolute error = " << maximumAbsoluteError
             << ", maximum relative error = " << maximumRelativeError
             << ", maximum relative error near boundary = " << maximumRelativeErrorNearBoundary;
}


void TestBasicFunctions::testLambertWVariants() {
    M::Boolean boolean = true;
    M::Integer integer = 2;
    M::Real    real    = 3.0;
    M::Complex complex = M::Complex(4.0);

    M::Variant vBoolean(boolean);
    M::Variant vInteger(integer);
    M::Variant vReal(real);
    M::Variant vComplex(complex);

    QCOMPARE(M::lambertW(vInteger, boolean), M::lambertW(integer, boolean));
    QCOMPARE(M::lambertW(vInteger, integer), M::lambertW(integer, integer));
    QCOMPARE(M::lambertW(vInteger, real), M::lambertW(integer, real));
    QCOMPARE(M::lambertW(vInteger, complex), M::lambertW(integer, complex));

    QCOMPARE(M::lambertW(vReal, boolean), M::lambertW(real, boolean));
    QCOMPARE(M::lambertW(vReal, integer), M::lambertW(real, integer));
    QCOMPARE(M::lambertW(vReal, real), M::lambertW(real, real));
    QCOMPARE(M::lambertW(vReal, complex), M::lambertW(real, complex));

    QCOMPARE(M::lambertW(vComplex, boolean), M::lambertW(complex, boolean));
    QCOMPARE(M::lambertW(vComplex, integer), M::lambertW(complex, integer));
    QCOMPARE(M::lambertW(vComplex, real), M::lambertW(complex, real));
    QCOMPARE(M::lambertW(vComplex, complex), M::lambertW(complex, complex));

    QCOMPARE(M::lambertW(integer, vBoolean), M::lambertW(integer, boolean));
    QCOMPARE(M::lambertW(integer, vInteger), M::lambertW(integer, integer));
    QCOMPARE(M::lambertW(integer, vReal), M::lambertW(integer, real));
    QCOMPARE(M::lambertW(integer, vComplex), M::lambertW(integer, complex));

    QCOMPARE(M::lambertW(real, vBoolean), M::lambertW(real, boolean));
    QCOMPARE(M::lambertW(real, vInteger), M::lambertW(real, integer));
    QCOMPARE(M::lambertW(real, vReal), M::lambertW(real, real));
    QCOMPARE(M::lambertW(real, vComplex), M::lambertW(real, complex));

    QCOMPARE(M::lambertW(complex, vBoolean), M::lambertW(complex, boolean));
    QCOMPARE(M::lambertW(complex, vInteger), M::lambertW(complex, integer));
    QCOMPARE(M::lambertW(complex, vReal), M::lambertW(complex, real));
    QCOMPARE(M::lambertW(complex, vComplex), M::lambertW(complex, complex));

    QCOMPARE(M::lambertW(vInteger, vBoolean), M::lambertW(integer, boolean));
    QCOMPARE(M::lambertW(vInteger, vInteger), M::lambertW(integer, integer));
    QCOMPARE(M::lambertW(vInteger, vReal), M::lambertW(integer, real));
    QCOMPARE(M::lambertW(vInteger, vComplex), M::lambertW(integer, complex));

    QCOMPARE(M::lambertW(vReal, vBoolean), M::lambertW(real, boolean));
    QCOMPARE(M::lambertW(vReal, vInteger), M::lambertW(real, integer));
    QCOMPARE(M::lambertW(vReal, vReal), M::lambertW(real, real));
    QCOMPARE(M::lambertW(vReal, vComplex), M::lambertW(real, complex));

    QCOMPARE(M::lambertW(vComplex, vBoolean), M::lambertW(complex, boolean));
    QCOMPARE(M::lambertW(vComplex, vInteger), M::lambertW(complex, integer));
    QCOMPARE(M::lambertW(vComplex, vReal), M::lambertW(complex, real));
    QCOMPARE(M::lambertW(vComplex, vComplex), M::lambertW(complex, complex));
}
