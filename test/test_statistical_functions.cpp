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
* This file implements tests of statistical math functions.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QObject>
#include <QVector>
#include <QtTest/QtTest>

#include <limits>
#include <complex>
#include <cmath>

#include <m_intrinsic_types.h>
#include <m_basic_functions.h>
#include <m_per_thread.h>
#include <m_matrix_integer.h>
#include <m_matrix_real.h>
#include <m_statistical_functions.h>

#include "test_statistical_functions.h"

TestStatisticalFunctions::TestStatisticalFunctions() {}


TestStatisticalFunctions::~TestStatisticalFunctions() {}


void TestStatisticalFunctions::testRandomInteger1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    QVector<double> d(numberPeriodicAutocorrelationPoints);

    for (unsigned i=0 ; i<numberPeriodicAutocorrelationPoints ; ++i) {
        M::Integer v = M::randomInteger64(pt);
        d[i] = (static_cast<long double>(v) / static_cast<long double>(0x8000000000000000ULL)) / 2.0;
    }

    QVector<double> pac = periodicAutocorrelation(d);

    QCOMPARE(averageCorrelation(pac) < 5.0E-7, true);
    QCOMPARE(maximumCorrelation(pac) < 0.06, true);
}


void TestStatisticalFunctions::testRandomIntegerMatrix1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    M::MatrixInteger m = M::randomInteger64(
        pt,
        M::Integer(100),
        M::Integer(numberPeriodicAutocorrelationPoints / 100)
    );

    QVector<double> d(numberPeriodicAutocorrelationPoints);
    unsigned long   index = 0;
    for (M::Integer ri=1 ; ri<=100 ; ++ri) {
        for (M::Integer ci=1 ; ci<=numberPeriodicAutocorrelationPoints/100 ; ++ci) {
            d[index++] = (static_cast<long double>(m(ri, ci)) / static_cast<long double>(0x8000000000000000ULL)) / 2.0;
        }
    }

    QVector<double> pac = periodicAutocorrelation(d);

    QCOMPARE(averageCorrelation(pac) < 5.0E-7, true);
    QCOMPARE(maximumCorrelation(pac) < 0.07, true);
}


void TestStatisticalFunctions::testRandomIntegerMatrix2() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    M::MatrixInteger m = M::randomInteger64(pt, M::Real(100), M::Real(numberPeriodicAutocorrelationPoints / 100));

    QVector<double> d(numberPeriodicAutocorrelationPoints);
    unsigned long   index = 0;
    for (M::Integer ri=1 ; ri<=100 ; ++ri) {
        for (M::Integer ci=1 ; ci<=numberPeriodicAutocorrelationPoints/100 ; ++ci) {
            d[index++] = (static_cast<long double>(m(ri, ci)) / static_cast<long double>(0x8000000000000000ULL)) / 2.0;
        }
    }

    QVector<double> pac = periodicAutocorrelation(d);

    QCOMPARE(averageCorrelation(pac) < 5.0E-7, true);
    QCOMPARE(maximumCorrelation(pac) < 0.07, true);
}


void TestStatisticalFunctions::testRandomInteger32() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    QVector<double> d(numberPeriodicAutocorrelationPoints);

    for (unsigned i=0 ; i<numberPeriodicAutocorrelationPoints ; ++i) {
        d[i] = static_cast<double>(M::randomInteger32(pt)) / static_cast<double>(0xFFFFFFFF) - 0.5;
    }

    QVector<double> pac = periodicAutocorrelation(d);

    QCOMPARE(averageCorrelation(pac) < 3.0E-5, true);
    QCOMPARE(maximumCorrelation(pac) < 0.07, true);
}


void TestStatisticalFunctions::testUniformDeviateInclusive1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    QVector<double> d(numberPeriodicAutocorrelationPoints);

    for (unsigned i=0 ; i<numberPeriodicAutocorrelationPoints ; ++i) {
        d[i] = M::uniformDeviateInclusive(pt) - 0.5;
    }

    QVector<double> pac = periodicAutocorrelation(d);

    QCOMPARE(averageCorrelation(pac) < 5.0E-6, true);
    QCOMPARE(maximumCorrelation(pac) < 0.07, true);
}


void TestStatisticalFunctions::testUniformDeviateInclusiveMatrix1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    M::MatrixReal m = M::uniformDeviateInclusive(
        pt,
        M::Integer(100),
        M::Integer(numberPeriodicAutocorrelationPoints / 100)
    );

    QVector<double> d(numberPeriodicAutocorrelationPoints);
    unsigned long   index = 0;
    for (M::Integer ri=1 ; ri<=100 ; ++ri) {
        for (M::Integer ci=1 ; ci<=numberPeriodicAutocorrelationPoints/100 ; ++ci) {
            d[index++] = m(ri, ci) - 0.5;
        }
    }

    QVector<double> pac = periodicAutocorrelation(d);

    QCOMPARE(averageCorrelation(pac) < 5.0E-6, true);
    QCOMPARE(maximumCorrelation(pac) < 0.07, true);
}


void TestStatisticalFunctions::testUniformDeviateInclusiveMatrix2() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    M::MatrixReal m = M::uniformDeviateInclusive(
        pt,
        M::Real(100),
        M::Complex(numberPeriodicAutocorrelationPoints / 100)
    );

    QVector<double> d(numberPeriodicAutocorrelationPoints);
    unsigned long   index = 0;
    for (M::Integer ri=1 ; ri<=100 ; ++ri) {
        for (M::Integer ci=1 ; ci<=numberPeriodicAutocorrelationPoints/100 ; ++ci) {
            d[index++] = m(ri, ci) - 0.5;
        }
    }

    QVector<double> pac = periodicAutocorrelation(d);

    QCOMPARE(averageCorrelation(pac) < 5.0E-6, true);
    QCOMPARE(maximumCorrelation(pac) < 0.07, true);
}


void TestStatisticalFunctions::testUniformDeviateExclusive1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    QVector<double> d(numberPeriodicAutocorrelationPoints);

    for (unsigned i=0 ; i<numberPeriodicAutocorrelationPoints ; ++i) {
        d[i] = M::uniformDeviateExclusive(pt) - 0.5;
    }

    QVector<double> pac = periodicAutocorrelation(d);

    QCOMPARE(averageCorrelation(pac) < 5.0E-6, true);
    QCOMPARE(maximumCorrelation(pac) < 0.07, true);
}


void TestStatisticalFunctions::testUniformDeviateExclusiveMatrix1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    M::MatrixReal m = M::uniformDeviateExclusive(
        pt,
        M::Integer(100),
        M::Integer(numberPeriodicAutocorrelationPoints / 100)
    );

    QVector<double> d(numberPeriodicAutocorrelationPoints);
    unsigned long   index = 0;
    for (M::Integer ri=1 ; ri<=100 ; ++ri) {
        for (M::Integer ci=1 ; ci<=numberPeriodicAutocorrelationPoints/100 ; ++ci) {
            d[index++] = m(ri, ci) - 0.5;
        }
    }

    QVector<double> pac = periodicAutocorrelation(d);

    QCOMPARE(averageCorrelation(pac) < 7.0E-6, true);
    QCOMPARE(maximumCorrelation(pac) < 0.07, true);
}


void TestStatisticalFunctions::testUniformDeviateExclusiveMatrix2() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    M::MatrixReal m = M::uniformDeviateExclusive(
        pt,
        M::Real(100),
        M::Complex(numberPeriodicAutocorrelationPoints / 100)
    );

    QVector<double> d(numberPeriodicAutocorrelationPoints);
    unsigned long   index = 0;
    for (M::Integer ri=1 ; ri<=100 ; ++ri) {
        for (M::Integer ci=1 ; ci<=numberPeriodicAutocorrelationPoints/100 ; ++ci) {
            d[index++] = m(ri, ci) - 0.5;
        }
    }

    QVector<double> pac = periodicAutocorrelation(d);

    QCOMPARE(averageCorrelation(pac) < 7.0E-6, true);
    QCOMPARE(maximumCorrelation(pac) < 0.07, true);
}


void TestStatisticalFunctions::testNormalDeviate1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    QVector<double> d(numberIterations);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        d[i] = M::normalDeviate(pt);
    }

    double          minimum     = -4.0;
    double          maximum     = +4.0;
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::normalPdf(x) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = h[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.005, true);
    QCOMPARE(stdDev < 0.2, true);
}


void TestStatisticalFunctions::testNormalDeviate2() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    QVector<double> d(numberIterations);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        d[i] = M::normalDeviate(pt, M::Integer(1), M::Complex(2.0));
    }

    double          minimum     = 1.0 - 2.0 * 4.0;
    double          maximum     = 1.0 + 2.0 * 4.0;
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::normalPdf(x, 1.0, 2.0) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = h[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.005, true);
    QCOMPARE(stdDev < 0.2, true);
}


void TestStatisticalFunctions::testNormalDeviateMatrix1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);
    M::MatrixReal m = M::normalDeviate(
        pt,
        matrixRows,
        matrixColumns,
        M::Real(1.0),
        M::Real(2.0)
    );

    unsigned long   i = 0;
    QVector<double> d(matrixRows * matrixColumns);
    for (M::Integer ri=1 ; ri<=matrixRows ; ++ri) {
        for (M::Integer ci=1 ; ci<=matrixColumns ; ++ci) {
            d[i++] = m(ri, ci);
        }
    }

    double          minimum     = 1.0 - 2.0 * 4.0;
    double          maximum     = 1.0 + 2.0 * 4.0;
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::normalPdf(x, 1.0, 2.0) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = h[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.005, true);
    QCOMPARE(stdDev < 0.2, true);
}


void TestStatisticalFunctions::testNormalDeviateMatrix2() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);
    M::MatrixReal m = M::normalDeviate(
        pt,
        M::Real(matrixRows),
        M::Integer(matrixColumns),
        M::Complex(1.0),
        M::Complex(2.0)
    );

    unsigned long   i = 0;
    QVector<double> d(matrixRows * matrixColumns);
    for (M::Integer ri=1 ; ri<=matrixRows ; ++ri) {
        for (M::Integer ci=1 ; ci<=matrixColumns ; ++ci) {
            d[i++] = m(ri, ci);
        }
    }

    double          minimum     = 1.0 - 2.0 * 4.0;
    double          maximum     = 1.0 + 2.0 * 4.0;
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::normalPdf(x, 1.0, 2.0) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = h[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(avg < 0.005, true);
    QCOMPARE(stdDev < 0.2, true);
}


void TestStatisticalFunctions::testNormalPdf1() {
    M::Real mean  = 1.0;
    M::Real sigma = 3.0;

    M::Real minimum = mean - 4.0 * sigma;
    M::Real maximum = mean + 4.0 * sigma;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::normalPdf(x, mean, sigma);
        M::Real t = (x - mean) / sigma;
        M::Real expected = (1.0 / (sigma * std::sqrt(2.0 * M::pi))) * std::exp(-0.5 * t * t);

        M::Real relativeError = (measured - expected) / expected;
        QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
    }
}


void TestStatisticalFunctions::testNormalPdf2() {
    M::Real mean  = 1.0;
    M::Real sigma = 3.0;

    M::Real minimum = mean - 4.0 * sigma;
    M::Real maximum = mean + 4.0 * sigma;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::normalPdf(M::Complex(x), M::Boolean(mean), sigma);
        M::Real t = (x - mean) / sigma;
        M::Real expected = (1.0 / (sigma * std::sqrt(2.0 * M::pi))) * std::exp(-0.5 * t * t);

        M::Real relativeError = (measured - expected) / expected;
        QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
    }
}


void TestStatisticalFunctions::testNormalCdf1() {
    M::Real mean  = 1.0;
    M::Real sigma = 3.0;

    M::Real minimum = mean - 4.0 * sigma;
    M::Real maximum = mean + 4.0 * sigma;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::normalCdf(x, mean, sigma);
        M::Real expected = 0.5 * (1.0 + M::erf((x - mean) / (sigma * std::sqrt(2.0))));

        M::Real relativeError = (measured - expected) / expected;
        QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
    }
}


void TestStatisticalFunctions::testNormalCdf2() {
    M::Real mean  = 1.0;
    M::Real sigma = 3.0;

    M::Real minimum = mean - 4.0 * sigma;
    M::Real maximum = mean + 4.0 * sigma;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::normalCdf(x, mean, sigma);
        M::Real expected = 0.5 * (1.0 + M::erf((x - mean) / (sigma * std::sqrt(2.0))));

        M::Real relativeError = (measured - expected) / expected;
        QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
    }
}


void TestStatisticalFunctions::testNormalQuantile1() {
    M::Real mean  = 1.0;
    M::Real sigma = 3.0;

    M::Real minimum = mean - 4.0 * sigma;
    M::Real maximum = mean + 4.0 * sigma;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    M::Real sumRelativeError     = 0;
    M::Real maximumRelativeError = 0;

    for (M::Real expected=minimum ; expected<=maximum ; expected+=stepSize) {
        M::Real p        = M::normalCdf(expected, mean, sigma);
        M::Real measured = M::normalQuantile(p, mean, sigma);

        M::Real relativeError = (measured - expected) / expected;

        if (relativeError > maximumRelativeError) {
            maximumRelativeError = relativeError;
        }

        sumRelativeError += relativeError;

        if (relativeError > 6.0E-12) {
            QCOMPARE(std::abs(relativeError) < 6.0E-12, true);
        }
    }

    M::Real averageRelativeError = sumRelativeError / numberIterations;
    qDebug() << "average relative error = " << averageRelativeError
             << ", maximum relative error = " << maximumRelativeError;
}


void TestStatisticalFunctions::testNormalQuantile2() {
    M::Real mean  = 1.0;
    M::Real sigma = 3.0;

    M::Real minimum = mean - 4.0 * sigma;
    M::Real maximum = mean + 4.0 * sigma;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    M::Real sumRelativeError     = 0;
    M::Real maximumRelativeError = 0;

    for (M::Real expected=minimum ; expected<=maximum ; expected+=stepSize) {
        M::Real p        = M::normalCdf(expected, mean, sigma);
        M::Real measured = M::normalQuantile(p, M::Complex(mean), M::Complex(sigma));

        M::Real relativeError = (measured - expected) / expected;

        if (relativeError > maximumRelativeError) {
            maximumRelativeError = relativeError;
        }

        sumRelativeError += relativeError;

        if (relativeError > 6.0E-12) {
            QCOMPARE(std::abs(relativeError) < 6.0E-12, true);
        }
    }

    M::Real averageRelativeError = sumRelativeError / numberIterations;
    qDebug() << "average relative error = " << averageRelativeError
             << ", maximum relative error = " << maximumRelativeError;
}


void TestStatisticalFunctions::testGammaDeviate1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    QVector<double> d(numberIterations);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        d[i] = M::gammaDeviate(pt, M::Real(2.0), M::Real(1.0));
    }

    double          minimum     = 0;
    double          maximum     = +12.0;
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::gammaPdf(M::Real(x), M::Real(2.0), M::Real(1.0)) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = h[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.04, true);
    QCOMPARE(stdDev < 0.3, true);
}


void TestStatisticalFunctions::testGammaDeviate2() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    QVector<double> d(numberIterations);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        d[i] = M::gammaDeviate(pt, M::Complex(2.0), M::Integer(1.0));
    }

    double          minimum     = 0;
    double          maximum     = +12.0;
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::gammaPdf(M::Real(x), M::Real(2.0), M::Real(1.0)) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = h[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.04, true);
    QCOMPARE(stdDev < 0.3, true);
}


void TestStatisticalFunctions::testGammaDeviateMatrix1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);
    M::MatrixReal m = M::gammaDeviate(
        pt,
        matrixRows,
        matrixColumns,
        M::Real(2.0),
        M::Real(1.0)
    );

    unsigned long   i = 0;
    QVector<double> d(matrixRows * matrixColumns);
    for (M::Integer ri=1 ; ri<=matrixRows ; ++ri) {
        for (M::Integer ci=1 ; ci<=matrixColumns ; ++ci) {
            d[i++] = m(ri, ci);
        }
    }

    double          minimum     = 0.0;
    double          maximum     = +12.0;
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::gammaPdf(x, M::Real(2.0), M::Real(1.0)) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = h[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.05, true);
    QCOMPARE(stdDev < 0.2, true);
}


void TestStatisticalFunctions::testGammaDeviateMatrix2() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);
    M::MatrixReal m = M::gammaDeviate(
        pt,
        M::Complex(matrixRows),
        M::Real(matrixColumns),
        M::Complex(2.0),
        M::Boolean(1)
    );

    unsigned long   i = 0;
    QVector<double> d(matrixRows * matrixColumns);
    for (M::Integer ri=1 ; ri<=matrixRows ; ++ri) {
        for (M::Integer ci=1 ; ci<=matrixColumns ; ++ci) {
            d[i++] = m(ri, ci);
        }
    }

    double          minimum     = 0.0;
    double          maximum     = +12.0;
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::gammaPdf(x, M::Real(2.0), M::Real(1.0)) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = h[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.05, true);
    QCOMPARE(stdDev < 0.2, true);
}


void TestStatisticalFunctions::testGammaPdf1() {
    M::Real k = 1.0;
    M::Real s = 2.0;

    M::Real minimum = 0.0;
    M::Real maximum = 12.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::gammaPdf(x, k, s);
        M::Real expected = (1.0 / (M::gamma(k) * std::pow(s, k))) * std::pow(x, k - 1.0) * std::exp(-(x / s));

        M::Real relativeError = (measured - expected) / expected;
        QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
    }
}


void TestStatisticalFunctions::testGammaPdf2() {
    M::Real k = 1.0;
    M::Real s = 2.0;

    M::Real minimum = 0.0;
    M::Real maximum = 12.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::gammaPdf(x, M::Complex(k), M::Complex(s));
        M::Real expected = (1.0 / (M::gamma(k) * std::pow(s, k))) * std::pow(x, k - 1.0) * std::exp(-(x / s));

        M::Real relativeError = (measured - expected) / expected;
        QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
    }
}


void TestStatisticalFunctions::testGammaCdf1() {
    M::Real k = 1.0;
    M::Real s = 2.0;

    M::Real minimum = 0.0;
    M::Real maximum = 12.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::gammaCdf(x, k, s);
        M::Real expected = (1.0 / M::gamma(k)) * M::lowerGamma(k, x / s);

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;
            QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
        } else {
            QCOMPARE(measured < 1.0E-15, true);
        }
    }
}


void TestStatisticalFunctions::testGammaCdf2() {
    M::Real k = 1.0;
    M::Real s = 2.0;

    M::Real minimum = 0.0;
    M::Real maximum = 12.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::gammaCdf(x, M::Complex(k), s);
        M::Real expected = (1.0 / M::gamma(k)) * M::lowerGamma(k, x / s);

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;
            QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
        } else {
            QCOMPARE(measured < 1.0E-15, true);
        }
    }
}


void TestStatisticalFunctions::testGammaQuantile1() {
    M::Real k = 2.0;
    M::Real s = 1.0;

    M::Real minimum = 0.0;
    M::Real maximum = 12.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    M::Real sumRelativeError     = 0;
    M::Real maximumRelativeError = 0;

    for (M::Real expected=minimum ; expected<=maximum ; expected+=stepSize) {
        M::Real p        = M::gammaCdf(expected, k, s);
        M::Real measured = M::gammaQuantile(p, k, s);

        if (expected != 0.0) {
            M::Real relativeError = (measured - expected) / expected;

            if (relativeError > maximumRelativeError) {
                maximumRelativeError = relativeError;
            }

            sumRelativeError += relativeError;

            if (relativeError > 6.0E-12) {
                QCOMPARE(std::abs(relativeError) < 2.0E-13, true);
            }
        } else {
            QCOMPARE(std::abs(measured) < 5.0 * M::epsilon, true);
        }
    }

    M::Real averageRelativeError = sumRelativeError / numberIterations;
    qDebug() << "average relative error = " << averageRelativeError
             << ", maximum relative error = " << maximumRelativeError;
}


void TestStatisticalFunctions::testGammaQuantile2() {
    M::Real k = 2.0;
    M::Real s = 1.0;

    M::Real minimum = 0.0;
    M::Real maximum = 12.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    M::Real sumRelativeError     = 0;
    M::Real maximumRelativeError = 0;

    for (M::Real expected=minimum ; expected<=maximum ; expected+=stepSize) {
        M::Real p        = M::gammaCdf(expected, k, s);
        M::Real measured = M::gammaQuantile(p, M::Complex(k), s);

        if (expected != 0.0) {
            M::Real relativeError = (measured - expected) / expected;

            if (relativeError > maximumRelativeError) {
                maximumRelativeError = relativeError;
            }

            sumRelativeError += relativeError;

            if (relativeError > 6.0E-12) {
                QCOMPARE(std::abs(relativeError) < 2.0E-13, true);
            }
        } else {
            QCOMPARE(std::abs(measured) < 5.0 * M::epsilon, true);
        }
    }

    M::Real averageRelativeError = sumRelativeError / numberIterations;
    qDebug() << "average relative error = " << averageRelativeError
             << ", maximum relative error = " << maximumRelativeError;
}


void TestStatisticalFunctions::testWeibullDeviate1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    QVector<double> d(numberIterations);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        d[i] = M::weibullDeviate(pt, M::Real(2.0), M::Real(1.0), M::Real(1.0));
    }

    double          minimum     = 0;
    double          maximum     = +4;
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::weibullPdf(M::Real(x), M::Real(2.0), M::Real(1.0), M::Real(1.0)) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = pdf[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.01, true);
    QCOMPARE(stdDev < 0.2, true);
}


void TestStatisticalFunctions::testWeibullDeviate2() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    QVector<double> d(numberIterations);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        d[i] = M::weibullDeviate(pt, M::Complex(2.0), M::Integer(1.0), M::Boolean(1.0));
    }

    double          minimum     = 0;
    double          maximum     = +4;
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::weibullPdf(M::Real(x), M::Real(2.0), M::Real(1.0), M::Real(1.0)) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = pdf[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.01, true);
    QCOMPARE(stdDev < 0.2, true);
}


void TestStatisticalFunctions::testWeibullDeviateMatrix1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);
    M::MatrixReal m = M::weibullDeviate(
        pt,
        matrixRows,
        matrixColumns,
        M::Real(2.0),
        M::Real(1.0),
        M::Real(1.0)
    );

    unsigned long   i = 0;
    QVector<double> d(matrixRows * matrixColumns);
    for (M::Integer ri=1 ; ri<=matrixRows ; ++ri) {
        for (M::Integer ci=1 ; ci<=matrixColumns ; ++ci) {
            d[i++] = m(ri, ci);
        }
    }

    double          minimum     = 0.0;
    double          maximum     = +4.0;
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::weibullPdf(x, M::Real(2.0), M::Real(1.0), M::Real(1.0)) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = pdf[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.02, true);
    QCOMPARE(stdDev < 0.1, true);
}


void TestStatisticalFunctions::testWeibullDeviateMatrix2() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);
    M::MatrixReal m = M::weibullDeviate(
        pt,
        matrixRows,
        matrixColumns,
        M::Complex(2.0),
        M::Integer(1.0),
        M::Boolean(1.0)
    );

    unsigned long   i = 0;
    QVector<double> d(matrixRows * matrixColumns);
    for (M::Integer ri=1 ; ri<=matrixRows ; ++ri) {
        for (M::Integer ci=1 ; ci<=matrixColumns ; ++ci) {
            d[i++] = m(ri, ci);
        }
    }

    double          minimum     = 0.0;
    double          maximum     = +4.0;
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::weibullPdf(x, M::Real(2.0), M::Real(1.0), M::Real(1.0)) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = pdf[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.02, true);
    QCOMPARE(stdDev < 0.1, true);
}


void TestStatisticalFunctions::testWeibullPdf1() {
    M::Real shape = 3.0;
    M::Real scale = 2.0;
    M::Real delay = 1.0;

    M::Real minimum = 0.0;
    M::Real maximum = 5.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::weibullPdf(x, shape, scale, delay);
        M::Real expected =
              x < delay
            ? 0.0
            :   (shape / scale)
              * std::pow((x - delay) / scale, shape - 1.0)
              * std::exp(-std::pow((x - delay) / scale, shape));

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;
            QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
        } else {
            QCOMPARE(std::abs(measured) < 2.0 * M::epsilon, true);
        }
    }
}


void TestStatisticalFunctions::testWeibullPdf2() {
    M::Real shape = 3.0;
    M::Real scale = 2.0;
    M::Real delay = 1.0;

    M::Real minimum = 0.0;
    M::Real maximum = 5.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::weibullPdf(x, M::Complex(shape), scale, M::Complex(delay));
        M::Real expected =
              x < delay
            ? 0.0
            :   (shape / scale)
              * std::pow((x - delay) / scale, shape - 1.0)
              * std::exp(-std::pow((x - delay) / scale, shape));

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;
            QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
        } else {
            QCOMPARE(std::abs(measured) < 2.0 * M::epsilon, true);
        }
    }
}


void TestStatisticalFunctions::testWeibullCdf1() {
    M::Real shape = 3.0;
    M::Real scale = 2.0;
    M::Real delay = 1.0;

    M::Real minimum = 0.0;
    M::Real maximum = 5.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::weibullCdf(x, shape, scale, delay);
        M::Real expected =
              x < delay
            ? 0.0
            : 1.0 - std::exp(-std::pow((x - delay) / scale, shape));

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;
            QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
        } else {
            QCOMPARE(std::abs(measured) < 2.0 * M::epsilon, true);
        }
    }
}


void TestStatisticalFunctions::testWeibullCdf2() {
    M::Real shape = 3.0;
    M::Real scale = 2.0;
    M::Real delay = 1.0;

    M::Real minimum = 0.0;
    M::Real maximum = 5.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::weibullCdf(x, M::Complex(shape), scale, M::Complex(delay));
        M::Real expected =
              x < delay
            ? 0.0
            : 1.0 - std::exp(-std::pow((x - delay) / scale, shape));

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;
            QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
        } else {
            QCOMPARE(std::abs(measured) < 2.0 * M::epsilon, true);
        }
    }
}


void TestStatisticalFunctions::testWeibullQuantile1() {
    M::Real shape = 3.0;
    M::Real scale = 2.0;
    M::Real delay = 1.0;

    M::Real minimum = 0.0;
    M::Real maximum = 5.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    M::Real sumRelativeError     = 0;
    M::Real maximumRelativeError = 0;

    for (M::Real expected=minimum ; expected<=maximum ; expected+=stepSize) {
        M::Real p        = M::weibullCdf(expected, shape, scale, delay);
        M::Real measured = M::weibullQuantile(p, shape, scale, delay);

        if (p != 0.0) {
            M::Real relativeError = (measured - expected) / expected;

            if (relativeError > maximumRelativeError) {
                maximumRelativeError = relativeError;
            }

            sumRelativeError += relativeError;

            if (relativeError >= 6.0E-5) {
                // I am a bit surprized by the large relative error given how the Weibull CDF/Quantile functions
                // work.  I'm going to ignore the error for now but do believe that some optimization/tweaking should
                // be considered.
                QCOMPARE(std::abs(relativeError) < 6.0E-5, true);
            }
        } else {
            QCOMPARE(std::abs(measured) == delay, true);
        }
    }

    M::Real averageRelativeError = sumRelativeError / numberIterations;
    qDebug() << "average relative error = " << averageRelativeError
             << ", maximum relative error = " << maximumRelativeError;
}


void TestStatisticalFunctions::testWeibullQuantile2() {
    M::Real shape = 3.0;
    M::Real scale = 2.0;
    M::Real delay = 1.0;

    M::Real minimum = 0.0;
    M::Real maximum = 5.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    M::Real sumRelativeError     = 0;
    M::Real maximumRelativeError = 0;

    for (M::Real expected=minimum ; expected<=maximum ; expected+=stepSize) {
        M::Real p        = M::weibullCdf(expected, shape, scale, delay);
        M::Real measured = M::weibullQuantile(p, M::Complex(shape), scale, delay);

        if (p != 0.0) {
            M::Real relativeError = (measured - expected) / expected;

            if (relativeError > maximumRelativeError) {
                maximumRelativeError = relativeError;
            }

            sumRelativeError += relativeError;

            if (relativeError >= 6.0E-5) {
                // I am a bit surprized by the large relative error given how the Weibull CDF/Quantile functions
                // work.  I'm going to ignore the error for now but do believe that some optimization/tweaking should
                // be considered.
                QCOMPARE(std::abs(relativeError) < 6.0E-5, true);
            }
        } else {
            QCOMPARE(std::abs(measured) == delay, true);
        }
    }

    M::Real averageRelativeError = sumRelativeError / numberIterations;
    qDebug() << "average relative error = " << averageRelativeError
             << ", maximum relative error = " << maximumRelativeError;
}


void TestStatisticalFunctions::testExponentialDeviate1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    QVector<double> d(numberIterations);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        d[i] = M::exponentialDeviate(pt, M::Real(2.0));
    }

    double          minimum     = 0;
    double          maximum     = +4;
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::exponentialPdf(M::Real(x), M::Real(2.0)) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = pdf[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.01, true);
    QCOMPARE(stdDev < 0.25, true);
}


void TestStatisticalFunctions::testExponentialDeviate2() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    QVector<double> d(numberIterations);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        d[i] = M::exponentialDeviate(pt, M::Complex(2.0));
    }

    double          minimum     = 0;
    double          maximum     = +4;
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::exponentialPdf(M::Real(x), M::Real(2.0)) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = pdf[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.01, true);
    QCOMPARE(stdDev < 0.25, true);
}


void TestStatisticalFunctions::testExponentialDeviateMatrix1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);
    M::MatrixReal m = M::exponentialDeviate(pt, matrixRows, matrixColumns, M::Real(2.0));

    unsigned long   i = 0;
    QVector<double> d(matrixRows * matrixColumns);
    for (M::Integer ri=1 ; ri<=matrixRows ; ++ri) {
        for (M::Integer ci=1 ; ci<=matrixColumns ; ++ci) {
            d[i++] = m(ri, ci);
        }
    }

    double          minimum     = 0.0;
    double          maximum     = +4.0;
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::exponentialPdf(x, M::Real(2.0)) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = pdf[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.05, true);
    QCOMPARE(stdDev < 0.08, true);
}


void TestStatisticalFunctions::testExponentialDeviateMatrix2() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);
    M::MatrixReal m = M::exponentialDeviate(pt, matrixRows, matrixColumns, M::Integer(2.0));

    unsigned long   i = 0;
    QVector<double> d(matrixRows * matrixColumns);
    for (M::Integer ri=1 ; ri<=matrixRows ; ++ri) {
        for (M::Integer ci=1 ; ci<=matrixColumns ; ++ci) {
            d[i++] = m(ri, ci);
        }
    }

    double          minimum     = 0.0;
    double          maximum     = +4.0;
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::exponentialPdf(x, M::Real(2.0)) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = pdf[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.05, true);
    QCOMPARE(stdDev < 0.08, true);
}


void TestStatisticalFunctions::testExponentialPdf1() {
    M::Real rate = 2.0;

    M::Real minimum = 0.0;
    M::Real maximum = 5.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::exponentialPdf(x, rate);
        M::Real expected = rate * std::exp(-rate * x);

        M::Real relativeError = (measured - expected) / expected;
        QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
    }
}


void TestStatisticalFunctions::testExponentialPdf2() {
    M::Real rate = 2.0;

    M::Real minimum = 0.0;
    M::Real maximum = 5.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::exponentialPdf(x, M::Complex(rate));
        M::Real expected = rate * std::exp(-rate * x);

        M::Real relativeError = (measured - expected) / expected;
        QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
    }
}


void TestStatisticalFunctions::testExponentialCdf1() {
    M::Real rate = 2.0;

    M::Real minimum = 0.0;
    M::Real maximum = 5.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::exponentialCdf(x, rate);
        M::Real expected = 1.0 - std::exp(-rate * x);

        M::Real relativeError = (measured - expected) / expected;
        if (relativeError >= 2.0 * M::epsilon) {
            QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
        }
    }
}


void TestStatisticalFunctions::testExponentialCdf2() {
    M::Real rate = 2.0;

    M::Real minimum = 0.0;
    M::Real maximum = 5.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::exponentialCdf(x, M::Integer(rate));
        M::Real expected = 1.0 - std::exp(-rate * x);

        M::Real relativeError = (measured - expected) / expected;
        if (relativeError >= 2.0 * M::epsilon) {
            QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
        }
    }
}


void TestStatisticalFunctions::testExponentialQuantile1() {
    M::Real rate = 2.0;

    M::Real minimum = 0.0;
    M::Real maximum = 4.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    M::Real sumRelativeError     = 0;
    M::Real maximumRelativeError = 0;

    for (M::Real expected=minimum ; expected<=maximum ; expected+=stepSize) {
        M::Real p        = M::exponentialCdf(expected, rate);
        M::Real measured = M::exponentialQuantile(p, rate);

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;

            if (relativeError > maximumRelativeError) {
                maximumRelativeError = relativeError;
            }

            sumRelativeError += relativeError;

            QCOMPARE(std::abs(relativeError) < 5.0E-13, true);
        } else {
            QCOMPARE(std::abs(measured) < 2.0 * M::epsilon, true);
        }

    }

    M::Real averageRelativeError = sumRelativeError / numberIterations;
    qDebug() << "average relative error = " << averageRelativeError
             << ", maximum relative error = " << maximumRelativeError;
}


void TestStatisticalFunctions::testExponentialQuantile2() {
    M::Real rate = 2.0;

    M::Real minimum = 0.0;
    M::Real maximum = 4.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    M::Real sumRelativeError     = 0;
    M::Real maximumRelativeError = 0;

    for (M::Real expected=minimum ; expected<=maximum ; expected+=stepSize) {
        M::Real p        = M::exponentialCdf(expected, rate);
        M::Real measured = M::exponentialQuantile(M::Complex(p), M::Integer(rate));

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;

            if (relativeError > maximumRelativeError) {
                maximumRelativeError = relativeError;
            }

            sumRelativeError += relativeError;

            QCOMPARE(std::abs(relativeError) < 5.0E-13, true);
        } else {
            QCOMPARE(std::abs(measured) < 2.0 * M::epsilon, true);
        }

    }

    M::Real averageRelativeError = sumRelativeError / numberIterations;
    qDebug() << "average relative error = " << averageRelativeError
             << ", maximum relative error = " << maximumRelativeError;
}


void TestStatisticalFunctions::testRayleighDeviate1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    QVector<double> d(numberIterations);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        d[i] = M::rayleighDeviate(pt, M::Real(2.0));
    }

    double          minimum     = 0;
    double          maximum     = +6;
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::rayleighPdf(M::Real(x), M::Real(2.0)) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = pdf[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.01, true);
    QCOMPARE(stdDev < 0.1, true);
}


void TestStatisticalFunctions::testRayleighDeviate2() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    QVector<double> d(numberIterations);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        d[i] = M::rayleighDeviate(pt, M::Complex(2.0));
    }

    double          minimum     = 0;
    double          maximum     = +6;
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::rayleighPdf(M::Real(x), M::Real(2.0)) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = pdf[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.01, true);
    QCOMPARE(stdDev < 0.1, true);
}


void TestStatisticalFunctions::testRayleighDeviateMatrix1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);
    M::MatrixReal m = M::rayleighDeviate(pt, matrixRows, matrixColumns, M::Real(2.0));

    unsigned long   i = 0;
    QVector<double> d(matrixRows * matrixColumns);
    for (M::Integer ri=1 ; ri<=matrixRows ; ++ri) {
        for (M::Integer ci=1 ; ci<=matrixColumns ; ++ci) {
            d[i++] = m(ri, ci);
        }
    }

    double          minimum     = 0.0;
    double          maximum     = +6.0;
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::rayleighPdf(x, M::Real(2.0)) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = pdf[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.01, true);
    QCOMPARE(stdDev < 0.1, true);
}


void TestStatisticalFunctions::testRayleighDeviateMatrix2() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);
    M::MatrixReal m = M::rayleighDeviate(pt, matrixRows, M::Real(matrixColumns), M::Integer(2.0));

    unsigned long   i = 0;
    QVector<double> d(matrixRows * matrixColumns);
    for (M::Integer ri=1 ; ri<=matrixRows ; ++ri) {
        for (M::Integer ci=1 ; ci<=matrixColumns ; ++ci) {
            d[i++] = m(ri, ci);
        }
    }

    double          minimum     = 0.0;
    double          maximum     = +6.0;
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::rayleighPdf(x, M::Real(2.0)) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = pdf[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.01, true);
    QCOMPARE(stdDev < 0.1, true);
}


void TestStatisticalFunctions::testRayleighPdf1() {
    M::Real scale = 2.0;

    M::Real minimum = 0.0;
    M::Real maximum = 5.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured     = M::rayleighPdf(x, scale);
        M::Real scaleSquared = scale * scale;
        M::Real expected     = (x / scaleSquared) * std::exp(-(x * x / (2.0 * scaleSquared)));

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;
            if (std::abs(relativeError) >= 2.0 * M::epsilon || std::isnan(relativeError)) {
                QCOMPARE(std::isnan(relativeError), true);
                QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
            }
        } else {
            QCOMPARE(std::abs(measured) < 2.0 * M::epsilon, true);
        }
    }
}


void TestStatisticalFunctions::testRayleighPdf2() {
    M::Real scale = 2.0;

    M::Real minimum = 0.0;
    M::Real maximum = 5.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured     = M::rayleighPdf(M::Complex(x), scale);
        M::Real scaleSquared = scale * scale;
        M::Real expected     = (x / scaleSquared) * std::exp(-(x * x / (2.0 * scaleSquared)));

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;
            if (std::abs(relativeError) >= 2.0 * M::epsilon || std::isnan(relativeError)) {
                QCOMPARE(std::isnan(relativeError), true);
                QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
            }
        } else {
            QCOMPARE(std::abs(measured) < 2.0 * M::epsilon, true);
        }
    }
}


void TestStatisticalFunctions::testRayleighCdf1() {
    M::Real scale = 2.0;

    M::Real minimum = 0.0;
    M::Real maximum = 5.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::rayleighCdf(x, scale);
        M::Real expected = 1.0 - std::exp(-x * x / (2.0 * scale * scale));

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;
            if (std::abs(relativeError) >= 2.0 * M::epsilon || std::isnan(relativeError)) {
                QCOMPARE(std::isnan(relativeError), true);
                QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
            }
        } else {
            QCOMPARE(std::abs(measured) < 2.0 * M::epsilon, true);
        }
    }
}


void TestStatisticalFunctions::testRayleighCdf2() {
    M::Real scale = 2.0;

    M::Real minimum = 0.0;
    M::Real maximum = 5.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::rayleighCdf(M::Complex(x), scale);
        M::Real expected = 1.0 - std::exp(-x * x / (2.0 * scale * scale));

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;
            if (std::abs(relativeError) >= 2.0 * M::epsilon || std::isnan(relativeError)) {
                QCOMPARE(std::isnan(relativeError), true);
                QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
            }
        } else {
            QCOMPARE(std::abs(measured) < 2.0 * M::epsilon, true);
        }
    }
}


void TestStatisticalFunctions::testRayleighQuantile1() {
    M::Real rate = 2.0;

    M::Real minimum = 0.0;
    M::Real maximum = 5.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    M::Real sumRelativeError     = 0;
    M::Real maximumRelativeError = 0;

    for (M::Real expected=minimum ; expected<=maximum ; expected+=stepSize) {
        M::Real p        = M::rayleighCdf(expected, rate);
        M::Real measured = M::rayleighQuantile(p, rate);

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;

            if (relativeError > maximumRelativeError) {
                maximumRelativeError = relativeError;
            }

            sumRelativeError += relativeError;

            QCOMPARE(std::abs(relativeError) < 5.0E-8, true);
        } else {
            QCOMPARE(std::abs(measured) < 2.0 * M::epsilon, true);
        }

    }

    M::Real averageRelativeError = sumRelativeError / numberIterations;
    qDebug() << "average relative error = " << averageRelativeError
             << ", maximum relative error = " << maximumRelativeError;
}


void TestStatisticalFunctions::testRayleighQuantile2() {
    M::Real rate = 2.0;

    M::Real minimum = 0.0;
    M::Real maximum = 5.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    M::Real sumRelativeError     = 0;
    M::Real maximumRelativeError = 0;

    for (M::Real expected=minimum ; expected<=maximum ; expected+=stepSize) {
        M::Real p        = M::rayleighCdf(expected, rate);
        M::Real measured = M::rayleighQuantile(p, M::Complex(rate));

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;

            if (relativeError > maximumRelativeError) {
                maximumRelativeError = relativeError;
            }

            sumRelativeError += relativeError;

            QCOMPARE(std::abs(relativeError) < 5.0E-8, true);
        } else {
            QCOMPARE(std::abs(measured) < 2.0 * M::epsilon, true);
        }

    }

    M::Real averageRelativeError = sumRelativeError / numberIterations;
    qDebug() << "average relative error = " << averageRelativeError
             << ", maximum relative error = " << maximumRelativeError;
}


void TestStatisticalFunctions::testChiSquaredDeviate1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    QVector<double> d(numberIterations);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        d[i] = M::chiSquaredDeviate(pt, M::Integer(3));
    }

    double          minimum     = 0;
    double          maximum     = +6.0;
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::chiSquaredPdf(x, M::Integer(3.0)) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = pdf[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.01, true);
    QCOMPARE(stdDev < 0.05, true);
}


void TestStatisticalFunctions::testChiSquaredDeviate2() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    QVector<double> d(numberIterations);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        d[i] = M::chiSquaredDeviate(pt, M::Real(3.0));
    }

    double          minimum     = 0;
    double          maximum     = +6.0;
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::chiSquaredPdf(x, M::Integer(3.0)) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = pdf[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.01, true);
    QCOMPARE(stdDev < 0.05, true);
}


void TestStatisticalFunctions::testChiSquaredDeviateMatrix1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);
    M::MatrixReal m = M::chiSquaredDeviate(pt, matrixRows, matrixColumns, M::Integer(3));

    unsigned long   i = 0;
    QVector<double> d(matrixRows * matrixColumns);
    for (M::Integer ri=1 ; ri<=matrixRows ; ++ri) {
        for (M::Integer ci=1 ; ci<=matrixColumns ; ++ci) {
            d[i++] = m(ri, ci);
        }
    }

    double          minimum     = 0;
    double          maximum     = +6.0;
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::chiSquaredPdf(x, M::Integer(3)) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = h[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.01, true);
    QCOMPARE(stdDev < 0.05, true);
}


void TestStatisticalFunctions::testChiSquaredDeviateMatrix2() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);
    M::MatrixReal m = M::chiSquaredDeviate(pt, M::Complex(matrixRows), matrixColumns, M::Real(3));

    unsigned long   i = 0;
    QVector<double> d(matrixRows * matrixColumns);
    for (M::Integer ri=1 ; ri<=matrixRows ; ++ri) {
        for (M::Integer ci=1 ; ci<=matrixColumns ; ++ci) {
            d[i++] = m(ri, ci);
        }
    }

    double          minimum     = 0;
    double          maximum     = +6.0;
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::chiSquaredPdf(x, M::Integer(3)) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = h[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.01, true);
    QCOMPARE(stdDev < 0.05, true);
}


void TestStatisticalFunctions::testChiSquaredPdf1() {
    M::Integer k = 3.0;

    M::Real minimum = 0.0;
    M::Real maximum = 8.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::chiSquaredPdf(x, k);
        M::Real expected = (
              (1.0 / (std::pow(2.0, k / 2.0) * M::gamma(k / 2.0)))
            * std::pow(x, k / 2.0 - 1.0)
            * std::exp(-x / 2.0)
        );

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;
            QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
        } else {
            QCOMPARE(std::abs(measured) < 2.0 * M::epsilon, true);
        }
    }
}


void TestStatisticalFunctions::testChiSquaredPdf2() {
    M::Integer k = 3.0;

    M::Real minimum = 0.0;
    M::Real maximum = 8.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::chiSquaredPdf(x, M::Complex(k));
        M::Real expected = (
              (1.0 / (std::pow(2.0, k / 2.0) * M::gamma(k / 2.0)))
            * std::pow(x, k / 2.0 - 1.0)
            * std::exp(-x / 2.0)
        );

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;
            QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
        } else {
            QCOMPARE(std::abs(measured) < 2.0 * M::epsilon, true);
        }
    }
}


void TestStatisticalFunctions::testChiSquaredCdf1() {
    M::Integer k = 3.0;

    M::Real minimum = 0.0;
    M::Real maximum = 8.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::chiSquaredCdf(x, k);
        M::Real expected = (1.0 / M::gamma(k / 2.0)) * M::lowerGamma(k / 2.0, x / 2.0);

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;
            QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
        } else {
            QCOMPARE(std::abs(measured) < 2.0 * M::epsilon, true);
        }
    }
}


void TestStatisticalFunctions::testChiSquaredCdf2() {
    M::Integer k = 3.0;

    M::Real minimum = 0.0;
    M::Real maximum = 8.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::chiSquaredCdf(M::Complex(x), M::Real(k));
        M::Real expected = (1.0 / M::gamma(k / 2.0)) * M::lowerGamma(k / 2.0, x / 2.0);

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;
            QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
        } else {
            QCOMPARE(std::abs(measured) < 2.0 * M::epsilon, true);
        }
    }
}


void TestStatisticalFunctions::testChiSquaredQuantile1() {
    M::Integer k = 3.0;

    M::Real minimum = 0.0;
    M::Real maximum = 8.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    M::Real sumRelativeError     = 0;
    M::Real maximumRelativeError = 0;

    for (M::Real expected=minimum ; expected<=maximum ; expected+=stepSize) {
        M::Real p        = M::chiSquaredCdf(expected, k);
        M::Real measured = M::chiSquaredQuantile(p, k);

        if (expected != 0.0) {
            M::Real relativeError = (measured - expected) / expected;

            if (std::abs(relativeError) > maximumRelativeError) {
                maximumRelativeError = std::abs(relativeError);
            }

            sumRelativeError += relativeError;

            QCOMPARE(std::isnan(relativeError), false);
            QCOMPARE(std::abs(relativeError) < 2.0E-10, true);
        } else {
            QCOMPARE(std::abs(measured) < 10.0 * M::epsilon, true);
        }
    }

    M::Real averageRelativeError = sumRelativeError / numberIterations;
    qDebug() << "average relative error = " << averageRelativeError
             << ", maximum relative error = " << maximumRelativeError;
}


void TestStatisticalFunctions::testChiSquaredQuantile2() {
    M::Integer k = 3.0;

    M::Real minimum = 0.0;
    M::Real maximum = 8.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    M::Real sumRelativeError     = 0;
    M::Real maximumRelativeError = 0;

    for (M::Real expected=minimum ; expected<=maximum ; expected+=stepSize) {
        M::Real p        = M::chiSquaredCdf(expected, k);
        M::Real measured = M::chiSquaredQuantile(M::Complex(p), M::Real(k));

        if (expected != 0.0) {
            M::Real relativeError = (measured - expected) / expected;

            if (std::abs(relativeError) > maximumRelativeError) {
                maximumRelativeError = std::abs(relativeError);
            }

            sumRelativeError += relativeError;

            QCOMPARE(std::isnan(relativeError), false);
            QCOMPARE(std::abs(relativeError) < 2.0E-10, true);
        } else {
            QCOMPARE(std::abs(measured) < 10.0 * M::epsilon, true);
        }
    }

    M::Real averageRelativeError = sumRelativeError / numberIterations;
    qDebug() << "average relative error = " << averageRelativeError
             << ", maximum relative error = " << maximumRelativeError;
}


void TestStatisticalFunctions::testPoissonDeviate1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    QVector<M::Integer> d(numberIterations);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        d[i] = M::poissonDeviate(pt, M::Real(10.0));
    }

    QVector<double> h;
    for (unsigned i=0 ; i<numberIterations ; ++i) {
        M::Integer k = d[i];
        QCOMPARE(k >= 0, true);

        while (k >= h.size()) {
            h.append(0);
        }

        ++h[k];
    }

    for (M::Integer k=0 ; k<h.size() ; ++k) {
        h[k] /= static_cast<M::Real>(numberIterations);
    }

    QVector<double> pmf(h.size());
    for (M::Integer k=0 ; k<static_cast<unsigned>(pmf.size()) ; ++k) {
        pmf[k] = M::poissonPmf(k, M::Real(10.0));
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (M::Integer k=0 ; k<h.size() ; ++k) {
        double error = pmf[k] == 0 ? 0 : (h[k] - pmf[k]) / pmf[k];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.08, true);
    QCOMPARE(stdDev < 0.5, true);
}


void TestStatisticalFunctions::testPoissonDeviate2() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    QVector<M::Integer> d(numberIterations);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        d[i] = M::poissonDeviate(pt, M::Integer(10));
    }

    QVector<double> h;
    for (unsigned i=0 ; i<numberIterations ; ++i) {
        M::Integer k = d[i];
        QCOMPARE(k >= 0, true);

        while (k >= h.size()) {
            h.append(0);
        }

        ++h[k];
    }

    for (M::Integer k=0 ; k<h.size() ; ++k) {
        h[k] /= static_cast<M::Real>(numberIterations);
    }

    QVector<double> pmf(h.size());
    for (M::Integer k=0 ; k<static_cast<unsigned>(pmf.size()) ; ++k) {
        pmf[k] = M::poissonPmf(k, M::Real(10.0));
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (M::Integer k=0 ; k<h.size() ; ++k) {
        double error = pmf[k] == 0 ? 0 : (h[k] - pmf[k]) / pmf[k];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.08, true);
    QCOMPARE(stdDev < 0.5, true);
}


void TestStatisticalFunctions::testPoissonDeviateMatrix1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    M::MatrixInteger m = M::poissonDeviate(pt, matrixRows, matrixColumns, M::Real(10.0));

    unsigned long       i = 0;
    QVector<M::Integer> d(matrixRows * matrixColumns);
    for (M::Integer ri=1 ; ri<=matrixRows ; ++ri) {
        for (M::Integer ci=1 ; ci<=matrixColumns ; ++ci) {
            d[i++] = m(ri, ci);
        }
    }

    QVector<double> h;
    for (unsigned i=0 ; i<static_cast<unsigned>(d.size()) ; ++i) {
        M::Integer k = d[i];
        QCOMPARE(k >= 0, true);

        while (k >= h.size()) {
            h.append(0);
        }

        ++h[k];
    }

    for (M::Integer k=0 ; k<h.size() ; ++k) {
        h[k] /= static_cast<M::Real>(d.size());
    }

    QVector<double> pmf(h.size());
    for (M::Integer k=0 ; k<static_cast<unsigned>(pmf.size()) ; ++k) {
        pmf[k] = M::poissonPmf(k, M::Real(10.0));
    }

    double   sumError            = 0;
    double   sumSquaredError     = 0;
    unsigned numberIncludedTerms = 0;
    for (M::Integer k=0 ; k<h.size() ; ++k) {
        if (pmf[k] > 0.0001 && h[k] > 0.0001) { // Ignore extremes due to quantitization
            double error = pmf[k] == 0 ? 0 : (h[k] - pmf[k]) / pmf[k];
            sumError        += error;
            sumSquaredError += error * error;

            ++numberIncludedTerms;
        }
    }

    double avg    = sumError / static_cast<double>(numberIncludedTerms);
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(numberIncludedTerms)) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.01, true);
    QCOMPARE(stdDev < 0.03, true);
}


void TestStatisticalFunctions::testPoissonDeviateMatrix2() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    M::MatrixInteger m = M::poissonDeviate(pt, M::Real(matrixRows), matrixColumns, M::Integer(10));

    unsigned long       i = 0;
    QVector<M::Integer> d(matrixRows * matrixColumns);
    for (M::Integer ri=1 ; ri<=matrixRows ; ++ri) {
        for (M::Integer ci=1 ; ci<=matrixColumns ; ++ci) {
            d[i++] = m(ri, ci);
        }
    }

    QVector<double> h;
    for (unsigned i=0 ; i<static_cast<unsigned>(d.size()) ; ++i) {
        M::Integer k = d[i];
        QCOMPARE(k >= 0, true);

        while (k >= h.size()) {
            h.append(0);
        }

        ++h[k];
    }

    for (M::Integer k=0 ; k<h.size() ; ++k) {
        h[k] /= static_cast<M::Real>(d.size());
    }

    QVector<double> pmf(h.size());
    for (M::Integer k=0 ; k<static_cast<unsigned>(pmf.size()) ; ++k) {
        pmf[k] = M::poissonPmf(k, M::Real(10.0));
    }

    double   sumError            = 0;
    double   sumSquaredError     = 0;
    unsigned numberIncludedTerms = 0;
    for (M::Integer k=0 ; k<h.size() ; ++k) {
        if (pmf[k] > 0.0001 && h[k] > 0.0001) { // Ignore extremes due to quantitization
            double error = pmf[k] == 0 ? 0 : (h[k] - pmf[k]) / pmf[k];
            sumError        += error;
            sumSquaredError += error * error;

            ++numberIncludedTerms;
        }
    }

    double avg    = sumError / static_cast<double>(numberIncludedTerms);
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(numberIncludedTerms)) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.01, true);
    QCOMPARE(stdDev < 0.03, true);
}


void TestStatisticalFunctions::testPoissonPmf1() {
    M::Real rate = 10.0;

    M::Integer minimum = 0;
    M::Integer maximum = 30;
    for (M::Integer k=minimum ; k<=maximum ; ++k) {
        M::Real measured = M::poissonPmf(k, rate);
        M::Real expected = std::pow(rate, k) * std::exp(-rate) / M::factorial(k);

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;
            if (std::isnan(relativeError) || relativeError >= 2.0E-14) {
                QCOMPARE(std::isnan(relativeError), true);
                QCOMPARE(std::abs(relativeError) < 2.0E-14, true);
            }
        } else {
            QCOMPARE(std::abs(measured) < 2.0 * M::epsilon, true);
        }
    }
}


void TestStatisticalFunctions::testPoissonPmf2() {
    M::Real rate = 10.0;

    M::Integer minimum = 0;
    M::Integer maximum = 30;
    for (M::Integer k=minimum ; k<=maximum ; ++k) {
        M::Real measured = M::poissonPmf(k, M::Complex(rate));
        M::Real expected = std::pow(rate, k) * std::exp(-rate) / M::factorial(k);

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;
            if (std::isnan(relativeError) || relativeError >= 2.0E-14) {
                QCOMPARE(std::isnan(relativeError), true);
                QCOMPARE(std::abs(relativeError) < 2.0E-14, true);
            }
        } else {
            QCOMPARE(std::abs(measured) < 2.0 * M::epsilon, true);
        }
    }
}


void TestStatisticalFunctions::testPoissonCdf1() {
    M::Real rate = 10.0;

    M::Integer minimum = 0;
    M::Integer maximum = 30;
    for (M::Integer k=minimum ; k<=maximum ; ++k) {
        M::Real measured = M::poissonCdf(k, rate);
        M::Real expected = M::upperGamma(k + 1, rate) / M::factorial(k);

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;

            if (relativeError >= 2.0E-14 || std::isnan(relativeError)) {
                QCOMPARE(std::isnan(relativeError), true);
                QCOMPARE(std::abs(relativeError) < 2.0E-14, true);
            }
        } else {
            QCOMPARE(std::abs(measured) < 2.0 * M::epsilon, true);
        }
    }
}


void TestStatisticalFunctions::testPoissonCdf2() {
    M::Real rate = 10.0;

    M::Integer minimum = 0;
    M::Integer maximum = 30;
    for (M::Integer k=minimum ; k<=maximum ; ++k) {
        M::Real measured = M::poissonCdf(M::Complex(k), M::Complex(rate));
        M::Real expected = M::upperGamma(k + 1, rate) / M::factorial(k);

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;

            if (relativeError >= 2.0E-14 || std::isnan(relativeError)) {
                QCOMPARE(std::isnan(relativeError), true);
                QCOMPARE(std::abs(relativeError) < 2.0E-14, true);
            }
        } else {
            QCOMPARE(std::abs(measured) < 2.0 * M::epsilon, true);
        }
    }
}


void TestStatisticalFunctions::testBinomialDeviate1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    QVector<M::Integer> d(numberIterations);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        d[i] = M::binomialDeviate(pt, M::Integer(30), M::Real(0.7));
    }

    QVector<double> h(30 + 1);
    for (unsigned i=0 ; i<numberIterations ; ++i) {
        M::Integer k = d[i];
        QCOMPARE(k >= 0 && k <= 30, true);

        ++h[k];
    }

    for (M::Integer k=0 ; k<h.size() ; ++k) {
        h[k] /= static_cast<M::Real>(numberIterations);
    }

    QVector<double> pmf(h.size());
    for (M::Integer k=0 ; k<static_cast<unsigned>(pmf.size()) ; ++k) {
        pmf[k] = M::binomialPmf(k, M::Integer(30), M::Real(0.7));
    }

    double   sumError             = 0;
    double   sumSquaredError      = 0;
    unsigned numberIncludedValues = 0;
    for (M::Integer k=0 ; k<h.size() ; ++k) {
        if (std::abs(pmf[k]) > 0.001) {
            double error = pmf[k] == 0 ? 0 : (h[k] - pmf[k]) / pmf[k];
            sumError        += error;
            sumSquaredError += error * error;

            ++numberIncludedValues;
        }
    }

    double avg    = sumError / static_cast<double>(numberIncludedValues);
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(numberIncludedValues)) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.20, true);
    QCOMPARE(stdDev < 0.75, true);
}


void TestStatisticalFunctions::testBinomialDeviate2() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    QVector<M::Integer> d(numberIterations);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        d[i] = M::binomialDeviate(pt, M::Real(30), M::Complex(0.7));
    }

    QVector<double> h(30 + 1);
    for (unsigned i=0 ; i<numberIterations ; ++i) {
        M::Integer k = d[i];
        QCOMPARE(k >= 0 && k <= 30, true);

        ++h[k];
    }

    for (M::Integer k=0 ; k<h.size() ; ++k) {
        h[k] /= static_cast<M::Real>(numberIterations);
    }

    QVector<double> pmf(h.size());
    for (M::Integer k=0 ; k<static_cast<unsigned>(pmf.size()) ; ++k) {
        pmf[k] = M::binomialPmf(k, M::Integer(30), M::Real(0.7));
    }

    double   sumError             = 0;
    double   sumSquaredError      = 0;
    unsigned numberIncludedValues = 0;
    for (M::Integer k=0 ; k<h.size() ; ++k) {
        if (std::abs(pmf[k]) > 0.001) {
            double error = pmf[k] == 0 ? 0 : (h[k] - pmf[k]) / pmf[k];
            sumError        += error;
            sumSquaredError += error * error;

            ++numberIncludedValues;
        }
    }

    double avg    = sumError / static_cast<double>(numberIncludedValues);
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(numberIncludedValues)) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.20, true);
    QCOMPARE(stdDev < 0.75, true);
}


void TestStatisticalFunctions::testBinomialDeviateMatrix1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    M::MatrixInteger m = M::binomialDeviate(pt, matrixRows, matrixColumns, M::Integer(30), M::Real(0.7));

    unsigned long       i = 0;
    QVector<M::Integer> d(matrixRows * matrixColumns);
    for (M::Integer ri=1 ; ri<=matrixRows ; ++ri) {
        for (M::Integer ci=1 ; ci<=matrixColumns ; ++ci) {
            d[i++] = m(ri, ci);
        }
    }

    QVector<double> h(30 + 1);
    for (unsigned long i=0 ; i<static_cast<unsigned long>(d.size()) ; ++i) {
        M::Integer k = d[i];
        QCOMPARE(k >= 0 && k <= 30, true);

        ++h[k];
    }

    for (M::Integer k=0 ; k<h.size() ; ++k) {
        h[k] /= static_cast<M::Real>(d.size());
    }

    QVector<double> pmf(h.size());
    for (M::Integer k=0 ; k<static_cast<unsigned>(pmf.size()) ; ++k) {
        pmf[k] = M::binomialPmf(k, M::Integer(30), M::Real(0.7));
    }

    double   sumError             = 0;
    double   sumSquaredError      = 0;
    unsigned numberIncludedValues = 0;
    for (M::Integer k=0 ; k<h.size() ; ++k) {
        if (std::abs(pmf[k]) > 0.001) {
            double error = pmf[k] == 0 ? 0 : (h[k] - pmf[k]) / pmf[k];
            sumError        += error;
            sumSquaredError += error * error;

            ++numberIncludedValues;
        }
    }

    double avg    = sumError / static_cast<double>(numberIncludedValues);
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(numberIncludedValues)) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.20, true);
    QCOMPARE(stdDev < 0.75, true);
}


void TestStatisticalFunctions::testBinomialDeviateMatrix2() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    M::MatrixInteger m = M::binomialDeviate(
        pt,
        M::Complex(matrixRows),
        M::Real(matrixColumns),
        M::Real(30), M::Complex(0.7)
    );

    unsigned long       i = 0;
    QVector<M::Integer> d(matrixRows * matrixColumns);
    for (M::Integer ri=1 ; ri<=matrixRows ; ++ri) {
        for (M::Integer ci=1 ; ci<=matrixColumns ; ++ci) {
            d[i++] = m(ri, ci);
        }
    }

    QVector<double> h(30 + 1);
    for (unsigned long i=0 ; i<static_cast<unsigned long>(d.size()) ; ++i) {
        M::Integer k = d[i];
        QCOMPARE(k >= 0 && k <= 30, true);

        ++h[k];
    }

    for (M::Integer k=0 ; k<h.size() ; ++k) {
        h[k] /= static_cast<M::Real>(d.size());
    }

    QVector<double> pmf(h.size());
    for (M::Integer k=0 ; k<static_cast<unsigned>(pmf.size()) ; ++k) {
        pmf[k] = M::binomialPmf(k, M::Integer(30), M::Real(0.7));
    }

    double   sumError             = 0;
    double   sumSquaredError      = 0;
    unsigned numberIncludedValues = 0;
    for (M::Integer k=0 ; k<h.size() ; ++k) {
        if (std::abs(pmf[k]) > 0.001) {
            double error = pmf[k] == 0 ? 0 : (h[k] - pmf[k]) / pmf[k];
            sumError        += error;
            sumSquaredError += error * error;

            ++numberIncludedValues;
        }
    }

    double avg    = sumError / static_cast<double>(numberIncludedValues);
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(numberIncludedValues)) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.20, true);
    QCOMPARE(stdDev < 0.75, true);
}


void TestStatisticalFunctions::testBinomialPmf1() {
    M::Integer n = 30;
    M::Real    p = 0.7;

    for (M::Integer k=0 ; k<=n ; ++k) {
        M::Real measured = M::binomialPmf(k, n, p);
        M::Real expected = M::binomial(n, k) * std::pow(p, k) * std::pow(1.0 - p, n - k);

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;

            if (std::abs(relativeError) >= 5.0E-14 || std::isnan(relativeError)) {
                QCOMPARE(std::isnan(relativeError), true);
                QCOMPARE(std::abs(relativeError) < 5.0E-14, true);
            }
        } else {
            QCOMPARE(std::abs(measured) < 2.0 * M::epsilon, true);
        }
    }
}


void TestStatisticalFunctions::testBinomialPmf2() {
    M::Integer n = 30;
    M::Real    p = 0.7;

    for (M::Integer k=0 ; k<=n ; ++k) {
        M::Real measured = M::binomialPmf(M::Real(k), M::Complex(n), p);
        M::Real expected = M::binomial(n, k) * std::pow(p, k) * std::pow(1.0 - p, n - k);

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;

            if (std::abs(relativeError) >= 5.0E-14 || std::isnan(relativeError)) {
                QCOMPARE(std::isnan(relativeError), true);
                QCOMPARE(std::abs(relativeError) < 5.0E-14, true);
            }
        } else {
            QCOMPARE(std::abs(measured) < 2.0 * M::epsilon, true);
        }
    }
}


void TestStatisticalFunctions::testBinomialCdf1() {
    M::Integer n = 30;
    M::Real    p = 0.7;

    for (M::Integer k=0 ; k<=n ; ++k) {
        M::Real measured = M::binomialCdf(k, n, p);

        M::Real expected = 0;
        for (unsigned i=0 ; i<=k ; ++i) {
            expected += M::binomialPmf(M::Integer(i), n, p);
        }

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;

            if (std::abs(relativeError) >= 5.0E-14 || std::isnan(relativeError)) {
                QCOMPARE(std::isnan(relativeError), true);
                QCOMPARE(std::abs(relativeError) < 5.0E-14, true);
            }
        } else {
            QCOMPARE(std::abs(measured) < 2.0 * M::epsilon, true);
        }
    }
}


void TestStatisticalFunctions::testBinomialCdf2() {
    M::Integer n = 30;
    M::Real    p = 0.7;

    for (M::Integer k=0 ; k<=n ; ++k) {
        M::Real measured = M::binomialCdf(M::Real(k), M::Complex(n), M::Complex(p));

        M::Real expected = 0;
        for (unsigned i=0 ; i<=k ; ++i) {
            expected += M::binomialPmf(M::Integer(i), n, p);
        }

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;

            if (std::abs(relativeError) >= 5.0E-14 || std::isnan(relativeError)) {
                QCOMPARE(std::isnan(relativeError), true);
                QCOMPARE(std::abs(relativeError) < 5.0E-14, true);
            }
        } else {
            QCOMPARE(std::abs(measured) < 2.0 * M::epsilon, true);
        }
    }
}


void TestStatisticalFunctions::testLogNormalDeviate1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    QVector<double> d(numberIterations);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        d[i] = M::logNormalDeviate(pt);
    }

    double          minimum     = std::exp(-3.0);
    double          maximum     = std::exp(+3.0);
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::logNormalPdf(x) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = h[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.05, true);
    QCOMPARE(stdDev < 0.25, true);
}


void TestStatisticalFunctions::testLogNormalDeviate2() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    QVector<double> d(numberIterations);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        M::Real v = M::logNormalDeviate(pt, M::Complex(1.0), M::Complex(1.5));
        QCOMPARE(std::isnan(v), false);
        d[i] = v;
    }

    double          minimum     = std::exp(1.0 - 2.0 * 1.5);
    double          maximum     = std::exp(1.0 + 2.0 * 1.5);
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::logNormalPdf(x, M::Real(1.0), M::Real(1.5)) * bucketWidth;

        if (std::isnan(p)) {
            QCOMPARE(std::isnan(p), false);
        }

        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = pdf[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.05, true);
    QCOMPARE(stdDev < 0.25, true);
}


void TestStatisticalFunctions::testLogNormalDeviateMatrix1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);
    M::MatrixReal m = M::logNormalDeviate(
        pt,
        matrixRows,
        matrixColumns,
        M::Real(1.0),
        M::Real(2.0)
    );

    unsigned long   i = 0;
    QVector<double> d(matrixRows * matrixColumns);
    for (M::Integer ri=1 ; ri<=matrixRows ; ++ri) {
        for (M::Integer ci=1 ; ci<=matrixColumns ; ++ci) {
            d[i++] = m(ri, ci);
        }
    }

    double          minimum     = std::exp(1.0 - 2.0 * 2.0);
    double          maximum     = std::exp(1.0 + 2.0 * 2.0);
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::logNormalPdf(x, 1.0, 2.0) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = h[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.05, true);
    QCOMPARE(stdDev < 0.1, true);
}


void TestStatisticalFunctions::testLogNormalDeviateMatrix2() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);
    M::MatrixReal m = M::logNormalDeviate(
        pt,
        matrixRows,
        matrixColumns,
        M::Complex(1.0),
        M::Integer(2.0)
    );

    unsigned long   i = 0;
    QVector<double> d(matrixRows * matrixColumns);
    for (M::Integer ri=1 ; ri<=matrixRows ; ++ri) {
        for (M::Integer ci=1 ; ci<=matrixColumns ; ++ci) {
            d[i++] = m(ri, ci);
        }
    }

    double          minimum     = std::exp(1.0 - 2.0 * 2.0);
    double          maximum     = std::exp(1.0 + 2.0 * 2.0);
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::logNormalPdf(x, 1.0, 2.0) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = h[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.05, true);
    QCOMPARE(stdDev < 0.1, true);
}


void TestStatisticalFunctions::testLogNormalPdf1() {
    M::Real mean  = 1.0;
    M::Real sigma = 3.0;

    M::Real minimum  = std::exp(mean - 4.0 * sigma);
    M::Real maximum  = std::exp(mean + 4.0 * sigma);
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::logNormalPdf(x, mean, sigma);
        M::Real t = (std::log(x) - mean) / sigma;
        M::Real expected = (1.0 / (x * sigma * std::sqrt(2.0 * M::pi))) * std::exp(-0.5 * t * t);

        M::Real relativeError = (measured - expected) / expected;
        QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
    }
}


void TestStatisticalFunctions::testLogNormalPdf2() {
    M::Real mean  = 1.0;
    M::Real sigma = 3.0;

    M::Real minimum  = std::exp(mean - 4.0 * sigma);
    M::Real maximum  = std::exp(mean + 4.0 * sigma);
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::logNormalPdf(x, M::Integer(mean), M::Complex(sigma));
        M::Real t = (std::log(x) - mean) / sigma;
        M::Real expected = (1.0 / (x * sigma * std::sqrt(2.0 * M::pi))) * std::exp(-0.5 * t * t);

        M::Real relativeError = (measured - expected) / expected;
        QCOMPARE(std::abs(relativeError) < 2.0E-15, true);
    }
}


void TestStatisticalFunctions::testLogNormalCdf1() {
    M::Real mean  = 1.0;
    M::Real sigma = 3.0;

    M::Real minimum = std::exp(mean - 1.0 * sigma);
    M::Real maximum = std::exp(mean + 2.0 * sigma);
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::logNormalCdf(x, mean, sigma);
        M::Real expected = 0.5 + 0.5 * M::erf((std::log(x) - mean) / (sigma * std::sqrt(2.0)));

        M::Real relativeError = (measured - expected) / expected;
        QCOMPARE(std::abs(relativeError) < 2.0E-15, true);
    }
}


void TestStatisticalFunctions::testLogNormalCdf2() {
    M::Real mean  = 1.0;
    M::Real sigma = 3.0;

    M::Real minimum = std::exp(mean - 1.0 * sigma);
    M::Real maximum = std::exp(mean + 2.0 * sigma);
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::logNormalCdf(x, M::Complex(mean), M::Integer(sigma));
        M::Real expected = 0.5 + 0.5 * M::erf((std::log(x) - mean) / (sigma * std::sqrt(2.0)));

        M::Real relativeError = (measured - expected) / expected;
        QCOMPARE(std::abs(relativeError) < 2.0E-15, true);
    }
}


void TestStatisticalFunctions::testLogNormalQuantile1() {
    M::Real mean  = 1.0;
    M::Real sigma = 3.0;

    M::Real minimum = std::exp(mean - 4.0 * sigma);
    M::Real maximum = std::exp(mean + 4.0 * sigma);
    M::Real stepSize = (maximum - minimum) / numberIterations;

    M::Real sumRelativeError     = 0;
    M::Real maximumRelativeError = 0;

    for (M::Real expected=minimum ; expected<=maximum ; expected+=stepSize) {
        M::Real p        = M::logNormalCdf(expected, mean, sigma);
        M::Real measured = M::logNormalQuantile(p, mean, sigma);

        M::Real relativeError = (measured - expected) / expected;

        if (relativeError > maximumRelativeError) {
            maximumRelativeError = relativeError;
        }

        sumRelativeError += relativeError;

        if (relativeError > 2.0E-12) {
            QCOMPARE(std::abs(relativeError) < 2.0E-12, true);
        }
    }

    M::Real averageRelativeError = sumRelativeError / numberIterations;
    qDebug() << "average relative error = " << averageRelativeError
             << ", maximum relative error = " << maximumRelativeError;
}


void TestStatisticalFunctions::testLogNormalQuantile2() {
    M::Real mean  = 1.0;
    M::Real sigma = 3.0;

    M::Real minimum = std::exp(mean - 4.0 * sigma);
    M::Real maximum = std::exp(mean + 4.0 * sigma);
    M::Real stepSize = (maximum - minimum) / numberIterations;

    M::Real sumRelativeError     = 0;
    M::Real maximumRelativeError = 0;

    for (M::Real expected=minimum ; expected<=maximum ; expected+=stepSize) {
        M::Real p        = M::logNormalCdf(expected, M::Complex(mean), M::Integer(sigma));
        M::Real measured = M::logNormalQuantile(p, mean, sigma);

        M::Real relativeError = (measured - expected) / expected;

        if (relativeError > maximumRelativeError) {
            maximumRelativeError = relativeError;
        }

        sumRelativeError += relativeError;

        if (relativeError > 2.0E-12) {
            QCOMPARE(std::abs(relativeError) < 2.0E-12, true);
        }
    }

    M::Real averageRelativeError = sumRelativeError / numberIterations;
    qDebug() << "average relative error = " << averageRelativeError
             << ", maximum relative error = " << maximumRelativeError;
}


void TestStatisticalFunctions::testGeometricDeviate1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    QVector<M::Integer> d(numberIterations);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        d[i] = M::geometricDeviate(pt, M::Real(0.3));
    }

    QVector<double> h;
    for (unsigned i=0 ; i<numberIterations ; ++i) {
        M::Integer k = d[i];
        QCOMPARE(k >= 0, true);

        while (k >= h.size()) {
            h.append(0);
        }

        ++h[k];
    }

    for (M::Integer k=0 ; k<h.size() ; ++k) {
        h[k] /= static_cast<M::Real>(numberIterations);
    }

    QVector<double> pmf(h.size());
    for (M::Integer k=0 ; k<static_cast<unsigned>(pmf.size()) ; ++k) {
        pmf[k] = M::geometricPmf(k, M::Real(0.3));
    }

    double   sumError            = 0;
    double   sumSquaredError     = 0;
    unsigned numberIncludedTerms = 0;
    for (M::Integer k=0 ; k<h.size() ; ++k) {
        if (pmf[k] >= 0.0001) {
            double error = (h[k] - pmf[k]) / pmf[k];
            sumError        += error;
            sumSquaredError += error * error;

            ++numberIncludedTerms;
        }
    }

    double avg    = sumError / static_cast<double>(numberIncludedTerms);
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(numberIncludedTerms)) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.03, true);
    QCOMPARE(stdDev < 0.20, true);
}


void TestStatisticalFunctions::testGeometricDeviate2() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    QVector<M::Integer> d(numberIterations);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        d[i] = M::geometricDeviate(pt, M::Complex(0.3));
    }

    QVector<double> h;
    for (unsigned i=0 ; i<numberIterations ; ++i) {
        M::Integer k = d[i];
        QCOMPARE(k >= 0, true);

        while (k >= h.size()) {
            h.append(0);
        }

        ++h[k];
    }

    for (M::Integer k=0 ; k<h.size() ; ++k) {
        h[k] /= static_cast<M::Real>(numberIterations);
    }

    QVector<double> pmf(h.size());
    for (M::Integer k=0 ; k<static_cast<unsigned>(pmf.size()) ; ++k) {
        pmf[k] = M::geometricPmf(k, M::Real(0.3));
    }

    double   sumError            = 0;
    double   sumSquaredError     = 0;
    unsigned numberIncludedTerms = 0;
    for (M::Integer k=0 ; k<h.size() ; ++k) {
        if (pmf[k] >= 0.0001) {
            double error = (h[k] - pmf[k]) / pmf[k];
            sumError        += error;
            sumSquaredError += error * error;

            ++numberIncludedTerms;
        }
    }

    double avg    = sumError / static_cast<double>(numberIncludedTerms);
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(numberIncludedTerms)) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.03, true);
    QCOMPARE(stdDev < 0.20, true);
}


void TestStatisticalFunctions::testGeometricDeviateMatrix1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    M::MatrixInteger m = M::geometricDeviate(pt, matrixRows, matrixColumns, M::Real(0.3));

    unsigned long       i = 0;
    QVector<M::Integer> d(matrixRows * matrixColumns);
    for (M::Integer ri=1 ; ri<=matrixRows ; ++ri) {
        for (M::Integer ci=1 ; ci<=matrixColumns ; ++ci) {
            d[i++] = m(ri, ci);
        }
    }

    QVector<double> h;
    for (unsigned i=0 ; i<static_cast<unsigned>(d.size()) ; ++i) {
        M::Integer k = d[i];
        QCOMPARE(k >= 0, true);

        while (k >= h.size()) {
            h.append(0);
        }

        ++h[k];
    }

    for (M::Integer k=0 ; k<h.size() ; ++k) {
        h[k] /= static_cast<M::Real>(d.size());
    }

    QVector<double> pmf(h.size());
    for (M::Integer k=0 ; k<static_cast<unsigned>(pmf.size()) ; ++k) {
        pmf[k] = M::geometricPmf(k, M::Real(0.3));
    }

    double   sumError            = 0;
    double   sumSquaredError     = 0;
    unsigned numberIncludedTerms = 0;
    for (M::Integer k=0 ; k<h.size() ; ++k) {
        if (pmf[k] >= 0.0001) {
            double error = (h[k] - pmf[k]) / pmf[k];
            sumError        += error;
            sumSquaredError += error * error;

            ++numberIncludedTerms;
        }
    }

    double avg    = sumError / static_cast<double>(numberIncludedTerms);
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(numberIncludedTerms)) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.01, true);
    QCOMPARE(stdDev < 0.05, true);
}


void TestStatisticalFunctions::testGeometricDeviateMatrix2() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    M::MatrixInteger m = M::geometricDeviate(pt, matrixRows, matrixColumns, M::Complex(0.3));

    unsigned long       i = 0;
    QVector<M::Integer> d(matrixRows * matrixColumns);
    for (M::Integer ri=1 ; ri<=matrixRows ; ++ri) {
        for (M::Integer ci=1 ; ci<=matrixColumns ; ++ci) {
            d[i++] = m(ri, ci);
        }
    }

    QVector<double> h;
    for (unsigned i=0 ; i<static_cast<unsigned>(d.size()) ; ++i) {
        M::Integer k = d[i];
        QCOMPARE(k >= 0, true);

        while (k >= h.size()) {
            h.append(0);
        }

        ++h[k];
    }

    for (M::Integer k=0 ; k<h.size() ; ++k) {
        h[k] /= static_cast<M::Real>(d.size());
    }

    QVector<double> pmf(h.size());
    for (M::Integer k=0 ; k<static_cast<unsigned>(pmf.size()) ; ++k) {
        pmf[k] = M::geometricPmf(k, M::Real(0.3));
    }

    double   sumError            = 0;
    double   sumSquaredError     = 0;
    unsigned numberIncludedTerms = 0;
    for (M::Integer k=0 ; k<h.size() ; ++k) {
        if (pmf[k] >= 0.0001) {
            double error = (h[k] - pmf[k]) / pmf[k];
            sumError        += error;
            sumSquaredError += error * error;

            ++numberIncludedTerms;
        }
    }

    double avg    = sumError / static_cast<double>(numberIncludedTerms);
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(numberIncludedTerms)) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.01, true);
    QCOMPARE(stdDev < 0.05, true);
}


void TestStatisticalFunctions::testGeometricPmf1() {
    M::Real p = 0.3;

    M::Integer minimum = 1;
    M::Integer maximum = 30;
    for (M::Integer k=minimum ; k<=maximum ; ++k) {
        M::Real measured = M::geometricPmf(k, p);
        M::Real expected = std::pow(1.0 - p, k - 1) * p;

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;
            if (std::isnan(relativeError) || relativeError >= 2.0E-14) {
                QCOMPARE(std::isnan(relativeError), true);
                QCOMPARE(std::abs(relativeError) < 2.0E-14, true);
            }
        } else {
            QCOMPARE(std::abs(measured) < 2.0 * M::epsilon, true);
        }
    }
}


void TestStatisticalFunctions::testGeometricPmf2() {
    M::Real p = 0.3;

    M::Integer minimum = 1;
    M::Integer maximum = 30;
    for (M::Integer k=minimum ; k<=maximum ; ++k) {
        M::Real measured = M::geometricPmf(M::Real(k), M::Complex(p));
        M::Real expected = std::pow(1.0 - p, k - 1) * p;

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;
            if (std::isnan(relativeError) || relativeError >= 2.0E-14) {
                QCOMPARE(std::isnan(relativeError), true);
                QCOMPARE(std::abs(relativeError) < 2.0E-14, true);
            }
        } else {
            QCOMPARE(std::abs(measured) < 2.0 * M::epsilon, true);
        }
    }
}


void TestStatisticalFunctions::testGeometricCdf1() {
    M::Real p = 0.3;

    M::Integer minimum = 1;
    M::Integer maximum = 30;
    for (M::Integer k=minimum ; k<=maximum ; ++k) {
        M::Real measured = M::geometricCdf(k, p);
        M::Real expected = 1.0 - std::pow(1.0 - p, k);

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;
            if (std::isnan(relativeError) || relativeError >= 2.0E-14) {
                QCOMPARE(std::isnan(relativeError), true);
                QCOMPARE(std::abs(relativeError) < 2.0E-14, true);
            }
        } else {
            QCOMPARE(std::abs(measured) < 2.0 * M::epsilon, true);
        }
    }
}


void TestStatisticalFunctions::testGeometricCdf2() {
    M::Real p = 0.3;

    M::Integer minimum = 1;
    M::Integer maximum = 30;
    for (M::Integer k=minimum ; k<=maximum ; ++k) {
        M::Real measured = M::geometricCdf(M::Real(k), M::Complex(p));
        M::Real expected = 1.0 - std::pow(1.0 - p, k);

        if (expected != 0) {
            M::Real relativeError = (measured - expected) / expected;
            if (std::isnan(relativeError) || relativeError >= 2.0E-14) {
                QCOMPARE(std::isnan(relativeError), true);
                QCOMPARE(std::abs(relativeError) < 2.0E-14, true);
            }
        } else {
            QCOMPARE(std::abs(measured) < 2.0 * M::epsilon, true);
        }
    }
}


void TestStatisticalFunctions::testCauchyLorentzDeviate1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    QVector<double> d(numberIterations);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        d[i] = M::cauchyLorentzDeviate(pt, M::Real(1.0), M::Real(0.5));
    }

    double          minimum     = -1.0;
    double          maximum     = +3.0;
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::cauchyLorentzPdf(x, M::Real(1.0), M::Real(0.5)) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = pdf[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.01, true);
    QCOMPARE(stdDev < 0.1, true);
}


void TestStatisticalFunctions::testCauchyLorentzDeviate2() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    QVector<double> d(numberIterations);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        d[i] = M::cauchyLorentzDeviate(pt, M::Integer(1.0), M::Complex(0.5));
    }

    double          minimum     = -1.0;
    double          maximum     = +3.0;
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::cauchyLorentzPdf(x, M::Real(1.0), M::Real(0.5)) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = pdf[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.01, true);
    QCOMPARE(stdDev < 0.1, true);
}


void TestStatisticalFunctions::testCauchyLorentzDeviateMatrix1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);
    M::MatrixReal m = M::cauchyLorentzDeviate(
        pt,
        matrixRows,
        matrixColumns,
        M::Real(1.0),
        M::Real(0.5)
    );

    unsigned long   i = 0;
    QVector<double> d(matrixRows * matrixColumns);
    for (M::Integer ri=1 ; ri<=matrixRows ; ++ri) {
        for (M::Integer ci=1 ; ci<=matrixColumns ; ++ci) {
            d[i++] = m(ri, ci);
        }
    }

    double          minimum     = -1.0;
    double          maximum     = +3.0;
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::cauchyLorentzPdf(x, 1.0, 0.5) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = h[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.01, true);
    QCOMPARE(stdDev < 0.05, true);
}


void TestStatisticalFunctions::testCauchyLorentzDeviateMatrix2() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };

    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);
    M::MatrixReal m = M::cauchyLorentzDeviate(
        pt,
        matrixRows,
        matrixColumns,
        M::Integer(1.0),
        M::Complex(0.5)
    );

    unsigned long   i = 0;
    QVector<double> d(matrixRows * matrixColumns);
    for (M::Integer ri=1 ; ri<=matrixRows ; ++ri) {
        for (M::Integer ci=1 ; ci<=matrixColumns ; ++ci) {
            d[i++] = m(ri, ci);
        }
    }

    double          minimum     = -1.0;
    double          maximum     = +3.0;
    double          bucketWidth = (maximum - minimum) / 100.0;
    QVector<double> h           = histogram(d, minimum, maximum, 100);

    QVector<double> pdf(h.size());
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double x = bucketWidth * i + minimum;
        double p = M::cauchyLorentzPdf(x, 1.0, 0.5) * bucketWidth;
        pdf[i] = p;
    }

    double sumError        = 0;
    double sumSquaredError = 0;
    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = h[i] == 0 ? 0 : (h[i] - pdf[i]) / pdf[i];
        sumError        += error;
        sumSquaredError += error * error;
    }

    double avg    = sumError / static_cast<double>(h.size());
    double stdDev = std::sqrt((sumSquaredError / static_cast<double>(h.size())) - avg * avg);

    QCOMPARE(std::abs(avg) < 0.01, true);
    QCOMPARE(stdDev < 0.05, true);
}


void TestStatisticalFunctions::testCauchyLorentzPdf1() {
    M::Real location = 1.0;
    M::Real scale    = 0.5;

    M::Real minimum = -1.0;
    M::Real maximum = +3.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::cauchyLorentzPdf(x, location, scale);
        M::Real t = (x - location) / scale;
        M::Real expected = 1.0 / (M::pi * scale * (1.0 + t * t));

        M::Real relativeError = (measured - expected) / expected;
        QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
    }
}


void TestStatisticalFunctions::testCauchyLorentzPdf2() {
    M::Real location = 1.0;
    M::Real scale    = 0.5;

    M::Real minimum = -1.0;
    M::Real maximum = +3.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::cauchyLorentzPdf(M::Complex(x), M::Complex(location), scale);
        M::Real t = (x - location) / scale;
        M::Real expected = 1.0 / (M::pi * scale * (1.0 + t * t));

        M::Real relativeError = (measured - expected) / expected;
        QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
    }
}


void TestStatisticalFunctions::testCauchyLorentzCdf1() {
    M::Real location = 1.0;
    M::Real scale    = 0.5;

    M::Real minimum = -1.0;
    M::Real maximum = +3.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::cauchyLorentzCdf(x, location, scale);
        M::Real expected = (1.0 / M::pi) * std::atan((x - location) / scale) + 0.5;

        M::Real relativeError = (measured - expected) / expected;
        QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
    }
}


void TestStatisticalFunctions::testCauchyLorentzCdf2() {
    M::Real location = 1.0;
    M::Real scale    = 0.5;

    M::Real minimum = -1.0;
    M::Real maximum = +3.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    for (M::Real x=minimum ; x<=maximum ; x+=stepSize) {
        M::Real measured = M::cauchyLorentzCdf(x, M::Complex(location), scale);
        M::Real expected = (1.0 / M::pi) * std::atan((x - location) / scale) + 0.5;

        M::Real relativeError = (measured - expected) / expected;
        QCOMPARE(std::abs(relativeError) < 2.0 * M::epsilon, true);
    }
}


void TestStatisticalFunctions::testCauchyLorentzQuantile1() {
    M::Real location = 1.0;
    M::Real scale    = 0.5;

    M::Real minimum = -1.0;
    M::Real maximum = +3.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    M::Real sumRelativeError     = 0;
    M::Real maximumRelativeError = 0;

    for (M::Real expected=minimum ; expected<=maximum ; expected+=stepSize) {
        M::Real p        = M::cauchyLorentzCdf(expected, location, scale);
        M::Real measured = M::cauchyLorentzQuantile(p, location, scale);

        M::Real relativeError = (measured - expected) / expected;

        if (relativeError > maximumRelativeError) {
            maximumRelativeError = relativeError;
        }

        sumRelativeError += relativeError;

        if (relativeError > 1.0E-3) {
            QCOMPARE(std::abs(relativeError) < 1.0E-3, true);
        }
    }

    M::Real averageRelativeError = sumRelativeError / numberIterations;
    qDebug() << "average relative error = " << averageRelativeError
             << ", maximum relative error = " << maximumRelativeError;
}


void TestStatisticalFunctions::testCauchyLorentzQuantile2() {
    M::Real location = 1.0;
    M::Real scale    = 0.5;

    M::Real minimum = -1.0;
    M::Real maximum = +3.0;
    M::Real stepSize = (maximum - minimum) / numberIterations;

    M::Real sumRelativeError     = 0;
    M::Real maximumRelativeError = 0;

    for (M::Real expected=minimum ; expected<=maximum ; expected+=stepSize) {
        M::Real p        = M::cauchyLorentzCdf(expected, location, scale);
        M::Real measured = M::cauchyLorentzQuantile(p, M::Complex(location), scale);

        M::Real relativeError = (measured - expected) / expected;

        if (relativeError > maximumRelativeError) {
            maximumRelativeError = relativeError;
        }

        sumRelativeError += relativeError;

        if (relativeError > 1.0E-3) {
            QCOMPARE(std::abs(relativeError) < 1.0E-3, true);
        }
    }

    M::Real averageRelativeError = sumRelativeError / numberIterations;
    qDebug() << "average relative error = " << averageRelativeError
             << ", maximum relative error = " << maximumRelativeError;
}


void TestStatisticalFunctions::testCount() {
    M::MatrixReal matrix = M::MatrixReal::build(
        3, 3,
        1.0, 2.0, 3.0,
        4.0, 5.0, 6.0,
        7.0, 8.0, 9.0
    );

    M::Set set1;
    set1.insert(matrix);
    set1.insert(M::Real(1.0));

    M::Set set2;
    set2.insert(set1);
    set2.insert(M::Integer(3));

    M::Tuple tuple;
    tuple.append(M::Complex(2.0, 1.0));
    tuple.append(M::Boolean(true));

    //                             1              11    2      9 = 23
    M::Integer c = M::count(M::Complex(4.0, 5.0), set2, tuple, matrix);
    QCOMPARE(c, 23);
}


void TestStatisticalFunctions::testSum() {
    M::MatrixReal matrix = M::MatrixReal::build(
        3, 3,
        1.0, 2.0, 3.0,
        4.0, 5.0, 6.0,
        7.0, 8.0, 9.0
    );

    M::Set set1;
    set1.insert(matrix);
    set1.insert(M::Real(1.0));

    M::Set set2;
    set2.insert(set1);
    set2.insert(M::Integer(3));

    M::Tuple tuple;
    tuple.append(M::Complex(2.0, 1.0));
    tuple.append(M::Boolean(true));

    M::Complex c = M::sum(M::Complex(4.0, 5.0), set2, tuple, matrix);
    QCOMPARE(c, M::Complex(101.0, 6.0));
}


void TestStatisticalFunctions::testAverage() {
    M::MatrixReal matrix = M::MatrixReal::build(
        3, 3,
        1.0, 2.0, 3.0,
        4.0, 5.0, 6.0,
        7.0, 8.0, 9.0
    );

    M::Complex c = M::avg(M::Complex(1.0, 2.0), matrix);
    QCOMPARE(c, M::Complex(4.6, 0.2));
}


void TestStatisticalFunctions::testMinimum() {
    M::MatrixReal matrix = M::MatrixReal::build(
        3, 3,
         1.0, -2.0,  3.0,
        -4.0,  5.0, -6.0,
         7.0, -8.0,  9.0
    );

    M::Set set;
    M::Real c = M::min(set, M::Complex(1.0, 0.0), matrix);
    QCOMPARE(c, -8.0);
}


void TestStatisticalFunctions::testMaximum() {
    M::MatrixReal matrix = M::MatrixReal::build(
        3, 3,
         1.0, -2.0,  3.0,
        -4.0,  5.0, -6.0,
         7.0, -8.0,  9.0
    );

    M::Set set;
    M::Real c = M::max(set, M::Complex(1.0, 0.0), matrix);
    QCOMPARE(c, 9.0);
}


void TestStatisticalFunctions::testVariance() {
    M::MatrixReal matrix = M::MatrixReal::build(
        3, 3,
        1.0, 2.0, 3.0,
        4.0, 5.0, 6.0,
        7.0, 8.0, 9.0
    );

    M::Set set1;
    M::Set set2;

    set2.insert(M::Real(10));
    set2.insert(M::Real(11));
    set1.insert(set2);

    M::Set set3;

    M::Real c = M::variance(set3, set2, matrix, M::Real(12), M::Integer(13));
    QCOMPARE(c, 14);
}


void TestStatisticalFunctions::testStdDev() {
    M::MatrixReal matrix = M::MatrixReal::build(
        3, 3,
        1.0, 2.0, 3.0,
        4.0, 5.0, 6.0,
        7.0, 8.0, 9.0
    );

    M::Set set1;
    M::Set set2;

    set2.insert(M::Real(10));
    set2.insert(M::Real(11));
    set1.insert(set2);

    M::Set set3;

    M::Real c = M::stdDev(set3, set2, matrix, M::Real(12), M::Integer(13));
    QCOMPARE(M::abs(c - M::sqrt(14.0)) < 2.0 * M::epsilon, true);
}


void TestStatisticalFunctions::testSampleStdDev() {
    M::MatrixReal matrix = M::MatrixReal::build(
        3, 3,
        1.0, 2.0, 3.0,
        4.0, 5.0, 6.0,
        7.0, 8.0, 9.0
    );

    M::Set set1;
    M::Set set2;

    set2.insert(M::Real(10));
    set2.insert(M::Real(11));
    set1.insert(set2);

    M::Set set3;

    M::Real c = M::sampleStdDev(set3, set2, matrix, M::Real(12), M::Integer(13));
    QCOMPARE(M::abs(c - 3.89444048185) < 1.0E-10, true);
}


void TestStatisticalFunctions::testMedian() {
    M::MatrixReal matrix = M::MatrixReal::build(
        3, 3,
         1.0,  1.0,  2.0,
         2.0,  3.0,  4.0,
        10.0, 11.0, 12.0
    );

    M::Set set1;
    M::Set set2;

    set2.insert(M::Real(13));
    set2.insert(M::Real(14));
    set1.insert(set2);

    M::Set set3;

    M::Real c = M::median(set3, set2, matrix, M::Real(15), M::Integer(16));
    QCOMPARE(c, 10.0);

    c = M::median(set3, set2, matrix, M::Real(15));
    QCOMPARE(c, 7.0);
}


void TestStatisticalFunctions::testMode() {
    M::MatrixReal matrix = M::MatrixReal::build(
        3, 3,
         1.0,  1.0,  2.0,
         2.0,  3.0,  4.0,
        10.0,  3.0, 12.0
    );

    M::Set set1;
    M::Set set2;

    set2.insert(M::Real(13));
    set2.insert(M::Real(3));
    set1.insert(set2);

    M::Set set3;

    M::Complex c = M::mode(set3, set2, matrix, M::Real(15), M::Integer(16));
    QCOMPARE(c, M::Complex(3.0));
}


void TestStatisticalFunctions::testSampleSkew() {
    M::MatrixReal matrix = M::MatrixReal::build(
        3, 3,
         1.0,  2.0,  2.0,
         3.0,  3.0,  3.0,
         4.0,  4.0,  4.0
    );

    M::Set set1;
    M::Set set2;

    set2.insert(M::Real(4));
    set2.insert(M::Real(5));
    set1.insert(set2);

    M::Set set3;

    M::Real c = M::sampleSkew(set3, set2, matrix, M::Real(6), M::Integer(7));
    QCOMPARE(std::abs(c - 0.35161696804) < 1.0E-9, true);
}


void TestStatisticalFunctions::testExcessKurtosis() {
    M::MatrixReal matrix = M::MatrixReal::build(
        3, 3,
         1.0,  2.0,  3.0,
         4.0,  5.0,  6.0,
         7.0,  8.0,  9.0
    );

    M::Set set1;
    M::Set set2;

    set2.insert(M::Real(10));
    set2.insert(M::Real(11));
    set1.insert(set2);

    M::Set set3;

    M::Real c = M::excessKurtosis(set3, set2, matrix, M::Real(12), M::Integer(13));
    QCOMPARE(std::abs(c - (-1.2142857143)) < 1.0E-9, true);
}


void TestStatisticalFunctions::testHistogram() {
    M::MatrixReal matrix = M::MatrixReal::build(
        3, 15,
         1.0,  2.0,  3.0,
         4.0,  5.0,  6.0,
         7.0,  8.0,  9.0,
         2.5,  3.3,  3.6,
         4.2,  4.4,  5.1,
         5.2,  6.1,  6.2,
         7.1,  7.2,  8.1,
         8.2,  9.1,  9.2,
         4.6,  5.3,  5.4,
         6.3,  6.4,  7.3,
         7.4,  8.3,  8.4,
         9.3,  9.4,  6.5,
         7.5,  7.6,  8.5,
         8.6,  9.5,  9.6,
         8.7,  9.7,  9.8
    );

    M::Set set1;
    M::Set set2;

    set2.insert(M::Real(-2.0));
    set2.insert(M::Real(11.0));
    set1.insert(set2);

    M::Set set3;

    M::Tuple t = M::histogram(
        M::Integer(1),
        M::Real(10.0),
        M::Complex(9.0),
        matrix,
        set1,
        M::Real(-3),
        M::Integer(12)
    );

    M::MatrixInteger counts        = t.at(M::Integer(1));
    M::MatrixReal    pdf           = t.at(M::Integer(2));
    M::MatrixReal    bucketCenters = t.at(M::Integer(3));
    M::Integer       numberBelow   = t.at(M::Integer(4)).toInteger();
    M::Integer       numberAbove   = t.at(M::Integer(5)).toInteger();
    M::Real          lowerBounds   = t.at(M::Integer(6)).toReal();
    M::Real          upperBounds   = t.at(M::Integer(7)).toReal();

    QCOMPARE(counts.numberRows(), 9);
    QCOMPARE(counts.numberColumns(), 1);
    QCOMPARE(pdf.numberRows(), 9);
    QCOMPARE(pdf.numberColumns(), 1);
    QCOMPARE(bucketCenters.numberRows(), 9);
    QCOMPARE(bucketCenters.numberColumns(), 1);

    for (M::Integer ri=1 ; ri<=9 ; ++ri) {
        M::Integer count = counts.at(ri, M::Integer(1));
        M::Real    p     = pdf.at(ri, M::Integer(1));
        M::Real    bc    = bucketCenters.at(ri, M::Integer(1));

        QCOMPARE(count, M::Integer(ri));
        QCOMPARE(std::abs(p - (M::Real(ri) / M::Real(45))) < 4.0 * M::epsilon, true);
        QCOMPARE(std::abs(bc - (1.0 * ri + 0.5)) < 4.0 * M::epsilon, true);
    }

    QCOMPARE(numberBelow, 2);
    QCOMPARE(numberAbove, 2);
    QCOMPARE(lowerBounds, M::Real(1.0));
    QCOMPARE(upperBounds, M::Real(10.0));
}






void TestStatisticalFunctions::testSortSet() {
    M::Set set = M::Set::build(
        M::Boolean(1),
        M::Set::build(M::Real(1), M::Complex(2, 1)),
        M::Real(9),
        M::Real(7),
        M::Real(8),
        M::Real(5),
        M::Real(6),
        M::Real(3),
        M::Real(2),
        M::Real(4),
        M::Real(1)
    );

    M::Tuple t1 = M::sort(set);

    QCOMPARE(t1.size(), 11);
    QCOMPARE(t1.at(M::Integer( 1)), M::Variant(M::Boolean(1)));
    QCOMPARE(t1.at(M::Integer( 2)), M::Variant(M::Real(1)));
    QCOMPARE(t1.at(M::Integer( 3)), M::Variant(M::Real(2)));
    QCOMPARE(t1.at(M::Integer( 4)), M::Variant(M::Real(3)));
    QCOMPARE(t1.at(M::Integer( 5)), M::Variant(M::Real(4)));
    QCOMPARE(t1.at(M::Integer( 6)), M::Variant(M::Real(5)));
    QCOMPARE(t1.at(M::Integer( 7)), M::Variant(M::Real(6)));
    QCOMPARE(t1.at(M::Integer( 8)), M::Variant(M::Real(7)));
    QCOMPARE(t1.at(M::Integer( 9)), M::Variant(M::Real(8)));
    QCOMPARE(t1.at(M::Integer(10)), M::Variant(M::Real(9)));
    QCOMPARE(t1.at(M::Integer(11)), M::Variant(M::Set::build(M::Real(1), M::Complex(2, 1))));

    M::Tuple t2 = M::sortDescending(set);

    QCOMPARE(t2.size(), 11);
    QCOMPARE(t2.at(M::Integer( 1)), M::Variant(M::Set::build(M::Real(1), M::Complex(2, 1))));
    QCOMPARE(t2.at(M::Integer( 2)), M::Variant(M::Real(9)));
    QCOMPARE(t2.at(M::Integer( 3)), M::Variant(M::Real(8)));
    QCOMPARE(t2.at(M::Integer( 4)), M::Variant(M::Real(7)));
    QCOMPARE(t2.at(M::Integer( 5)), M::Variant(M::Real(6)));
    QCOMPARE(t2.at(M::Integer( 6)), M::Variant(M::Real(5)));
    QCOMPARE(t2.at(M::Integer( 7)), M::Variant(M::Real(4)));
    QCOMPARE(t2.at(M::Integer( 8)), M::Variant(M::Real(3)));
    QCOMPARE(t2.at(M::Integer( 9)), M::Variant(M::Real(2)));
    QCOMPARE(t2.at(M::Integer(10)), M::Variant(M::Real(1)));
    QCOMPARE(t2.at(M::Integer(11)), M::Variant(M::Boolean(1)));
}


void TestStatisticalFunctions::testSortTuple() {
    M::Tuple tuple = M::Tuple::build(
        M::Boolean(1),
        M::Set::build(M::Real(1), M::Complex(2, 1)),
        M::Real(9),
        M::Real(7),
        M::Real(8),
        M::Real(5),
        M::Real(6),
        M::Real(3),
        M::Real(2),
        M::Real(4),
        M::Real(1)
    );

    M::Tuple t1 = M::sort(tuple);

    QCOMPARE(t1.size(), 11);
    QCOMPARE(t1.at(M::Integer( 1)), M::Variant(M::Boolean(1)));
    QCOMPARE(t1.at(M::Integer( 2)), M::Variant(M::Real(1)));
    QCOMPARE(t1.at(M::Integer( 3)), M::Variant(M::Real(2)));
    QCOMPARE(t1.at(M::Integer( 4)), M::Variant(M::Real(3)));
    QCOMPARE(t1.at(M::Integer( 5)), M::Variant(M::Real(4)));
    QCOMPARE(t1.at(M::Integer( 6)), M::Variant(M::Real(5)));
    QCOMPARE(t1.at(M::Integer( 7)), M::Variant(M::Real(6)));
    QCOMPARE(t1.at(M::Integer( 8)), M::Variant(M::Real(7)));
    QCOMPARE(t1.at(M::Integer( 9)), M::Variant(M::Real(8)));
    QCOMPARE(t1.at(M::Integer(10)), M::Variant(M::Real(9)));
    QCOMPARE(t1.at(M::Integer(11)), M::Variant(M::Set::build(M::Real(1), M::Complex(2, 1))));

    M::Tuple t2 = M::sortDescending(tuple);

    QCOMPARE(t2.size(), 11);
    QCOMPARE(t2.at(M::Integer( 1)), M::Variant(M::Set::build(M::Real(1), M::Complex(2, 1))));
    QCOMPARE(t2.at(M::Integer( 2)), M::Variant(M::Real(9)));
    QCOMPARE(t2.at(M::Integer( 3)), M::Variant(M::Real(8)));
    QCOMPARE(t2.at(M::Integer( 4)), M::Variant(M::Real(7)));
    QCOMPARE(t2.at(M::Integer( 5)), M::Variant(M::Real(6)));
    QCOMPARE(t2.at(M::Integer( 6)), M::Variant(M::Real(5)));
    QCOMPARE(t2.at(M::Integer( 7)), M::Variant(M::Real(4)));
    QCOMPARE(t2.at(M::Integer( 8)), M::Variant(M::Real(3)));
    QCOMPARE(t2.at(M::Integer( 9)), M::Variant(M::Real(2)));
    QCOMPARE(t2.at(M::Integer(10)), M::Variant(M::Real(1)));
    QCOMPARE(t2.at(M::Integer(11)), M::Variant(M::Boolean(1)));
}


void TestStatisticalFunctions::testSortMatrixBoolean() {
    M::MatrixBoolean matrix = M::MatrixBoolean::build(
        4, 5,
         true, false, false, false,
        false,  true, false, false,
         true, false,  true, false,
        false,  true, false,  true,
         true, false, false, false
    );

    M::MatrixBoolean m1 = M::sort(matrix);

    QCOMPARE(m1.numberRows(), 4 * 5);
    QCOMPARE(m1.numberColumns(), 1);

    QCOMPARE(
        m1,
        M::MatrixBoolean::build(
            4 * 5, 1,
            false, false, false, false,
            false, false, false, false,
            false, false, false, false,
            false,  true,  true,  true,
             true,  true,  true,  true
        )
    );

    M::MatrixBoolean m2 = M::sortDescending(matrix);

    QCOMPARE(m2.numberRows(), 4 * 5);
    QCOMPARE(m2.numberColumns(), 1);

    QCOMPARE(
        m2,
        M::MatrixBoolean::build(
            4 * 5, 1,
             true,  true,  true,  true,
             true,  true,  true, false,
            false, false, false, false,
            false, false, false, false,
            false, false, false, false
        )
    );
}


void TestStatisticalFunctions::testSortMatrixInteger() {
    M::MatrixInteger matrix = M::MatrixInteger::build(
        4, 5,
          1,  3,  2,  4,
          6,  7,  5,  8,
         11,  9, 10, 12,
         13, 15, 14, 16,
          0, 19, 17, 18
    );

    M::MatrixInteger m1 = M::sort(matrix);

    QCOMPARE(m1.numberRows(), 4 * 5);
    QCOMPARE(m1.numberColumns(), 1);

    QCOMPARE(
        m1,
        M::MatrixInteger::build(
            4 * 5, 1,
             0,  1,  2,  3,
             4,  5,  6,  7,
             8,  9, 10, 11,
            12, 13, 14, 15,
            16, 17, 18, 19
        )
    );

    M::MatrixInteger m2 = M::sortDescending(matrix);

    QCOMPARE(m2.numberRows(), 4 * 5);
    QCOMPARE(m2.numberColumns(), 1);

    QCOMPARE(
        m2,
        M::MatrixInteger::build(
            4 * 5, 1,
            19, 18, 17, 16,
            15, 14, 13, 12,
            11, 10,  9,  8,
             7,  6,  5,  4,
             3,  2,  1,  0
        )
    );
}


void TestStatisticalFunctions::testSortMatrixReal() {
    M::MatrixReal matrix = M::MatrixReal::build(
        4, 5,
          1.0,  3.0,  2.0,  4.0,
          6.0,  7.0,  5.0,  8.0,
         11.0,  9.0, 10.0, 12.0,
         13.0, 15.0, 14.0, 16.0,
          0.0, 19.0, 17.0, 18.0
    );

    M::MatrixReal m1 = M::sort(matrix);

    QCOMPARE(m1.numberRows(), 4 * 5);
    QCOMPARE(m1.numberColumns(), 1);

    QCOMPARE(
        m1,
        M::MatrixReal::build(
            4 * 5, 1,
             0.0,  1.0,  2.0,  3.0,
             4.0,  5.0,  6.0,  7.0,
             8.0,  9.0, 10.0, 11.0,
            12.0, 13.0, 14.0, 15.0,
            16.0, 17.0, 18.0, 19.0
        )
    );

    M::MatrixReal m2 = M::sortDescending(matrix);

    QCOMPARE(m2.numberRows(), 4 * 5);
    QCOMPARE(m2.numberColumns(), 1);

    QCOMPARE(
        m2,
        M::MatrixReal::build(
            4 * 5, 1,
            19.0, 18.0, 17.0, 16.0,
            15.0, 14.0, 13.0, 12.0,
            11.0, 10.0,  9.0,  8.0,
             7.0,  6.0,  5.0,  4.0,
             3.0,  2.0,  1.0,  0.0
        )
    );
}


void TestStatisticalFunctions::testSortMatrixComplex() {
    M::MatrixComplex matrix = M::MatrixComplex::build(
        4 * 5, 1,
          1.0,  3.0,  2.0,  4.0,
          6.0,  7.0,  5.0,  8.0,
         11.0,  9.0, 10.0, 12.0,
         13.0, 15.0, 14.0, 16.0,
          0.0, 19.0, 17.0, 18.0
    );

    M::MatrixComplex m1 = M::sort(matrix);

    QCOMPARE(m1.numberRows(), 4 * 5);
    QCOMPARE(m1.numberColumns(), 1);

    for (M::Integer ri=1 ; ri<=4 ; ++ri) {
        QString s;
        for (M::Integer ci=1 ; ci<=5 ; ++ci) {
            M::Complex v = m1.at(ri, ci);
            QString vs = tr("%1 + %2i").arg(v.real()).arg(v.imag());
            s += QString("%1").arg(vs, 20);
        }
        qDebug() << s;
    }

    QCOMPARE(
        m1,
        M::MatrixComplex::build(
            4 * 5, 1,
             0.0,  1.0,  2.0,  3.0,
             4.0,  5.0,  6.0,  7.0,
             8.0,  9.0, 10.0, 11.0,
            12.0, 13.0, 14.0, 15.0,
            16.0, 17.0, 18.0, 19.0
        )
    );

    M::MatrixComplex m2 = M::sortDescending(matrix);

    QCOMPARE(m2.numberRows(), 4 * 5);
    QCOMPARE(m2.numberColumns(), 1);

    for (M::Integer ri=1 ; ri<=4 ; ++ri) {
        QString s;
        for (M::Integer ci=1 ; ci<=5 ; ++ci) {
            M::Complex v = m2.at(ri, ci);
            QString vs = tr("%1 + %2i").arg(v.real()).arg(v.imag());
            s += QString("%1").arg(vs, 20);
        }
        qDebug() << s;
    }

    QCOMPARE(
        m2,
        M::MatrixComplex::build(
            4 * 5, 1,
            19.0, 18.0, 17.0, 16.0,
            15.0, 14.0, 13.0, 12.0,
            11.0, 10.0,  9.0,  8.0,
             7.0,  6.0,  5.0,  4.0,
             3.0,  2.0,  1.0,  0.0
        )
    );
}


QVector<double> TestStatisticalFunctions::histogram(
        const QVector<double>& values,
        double                 firstBucket,
        double                 lastBucket,
        unsigned               numberBuckets
    ) {
    QVector<double> result(numberBuckets);

    double   bucketWidth  = (lastBucket - firstBucket) / static_cast<double>(numberBuckets);
    unsigned numberValues = static_cast<unsigned>(values.size());

    for (unsigned i=0 ; i<numberValues ; ++i) {
        double   v      = values.at(i);
        if (v < firstBucket) {
            ++result[0];
        } else if (v >= lastBucket) {
            ++result[numberBuckets - 1];
        } else {
            unsigned bucket = static_cast<unsigned>((v - firstBucket) / bucketWidth);
            ++result[bucket];
        }
    }

    for (unsigned i=0 ; i<numberBuckets ; ++i) {
        result[i] /= 1.0 * numberValues;
    }

    return result;
}


QVector<double> TestStatisticalFunctions::periodicAutocorrelation(const QVector<double>& input) {
    unsigned        length = input.length();
    QVector<double> result(length, 0.0);

    for (unsigned long i=0 ; i<length ; ++i) {
        double x = 0;

        for (unsigned long j=0 ; j<length ; ++j) {
            unsigned long k = (j-i) % length;
            x += input[j]*input[k];
        }

        result[i] = x / length;
    }

    return result;
}


double TestStatisticalFunctions::averageCorrelation(const QVector<double>& input) {
    unsigned long inputLength    = static_cast<unsigned>(input.size());
    long double   sumCorrelation = 0;

    for (unsigned long i=1 ; i<inputLength ; ++i) {
        sumCorrelation += input.at(i);
    }

    return sumCorrelation / static_cast<double>(inputLength);
}


double TestStatisticalFunctions::maximumCorrelation(const QVector<double>& input) {
    unsigned long inputLength = static_cast<unsigned>(input.size());
    long double   maximum     = 0;

    for (unsigned long i=1 ; i<inputLength ; ++i) {
        double v = std::abs(input.at(i));
        if (v > maximum) {
            maximum = v;
        }
    }

    return maximum;
}
