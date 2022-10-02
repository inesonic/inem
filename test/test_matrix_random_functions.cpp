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
* This file implements tests of the random functions in \ref M::MatrixInteger and \ref M::MatrixReal class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QObject>
#include <QList>
#include <QtTest/QtTest>

#include <limits>
#include <complex>
#include <cmath>
#include <random>
#include <algorithm>

#include <model_exceptions.h>
#include <m_basic_functions.h>
#include <m_per_thread.h>
#include <m_matrix_integer.h>
#include <m_matrix_real.h>

#include "test_matrix_random_functions.h"

TestMatrixRandomFunctions::TestMatrixRandomFunctions() {}


TestMatrixRandomFunctions::~TestMatrixRandomFunctions() {}


void TestMatrixRandomFunctions::testMatrixIntegerRandomInteger() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };
    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    M::MatrixInteger m = M::MatrixInteger::randomInteger64(pt, 500, 100);
    QVector<double>  d(50000);

    unsigned i = 0;
    for (M::Integer rowIndex=1 ; rowIndex<=500 ; ++rowIndex) {
        for (M::Integer columnIndex=1 ; columnIndex<=100 ; ++columnIndex) {
            d[i] = (1.0 * m(rowIndex, columnIndex)) / static_cast<double>(0x7FFFFFFFFFFFFFFFULL);
            ++i;
        }
    }

    QVector<double> pac     = periodicAutocorrelation(d);
    double          avg     = averageCorrelation(pac);
    double          maximum = maximumCorrelation(pac);

    qDebug() << "average correlation = " << avg << ", maximum correlation = " << maximum;

    QCOMPARE(std::abs(avg) < 5.0E-6, true);
    QCOMPARE(maximum < 0.2, true);
}


void TestMatrixRandomFunctions::testMatrixRealRandomInclusive() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };
    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    M::MatrixReal m = M::MatrixReal::randomInclusive(pt, 500, 100);
    QVector<double>  d(50000);

    unsigned i = 0;
    for (M::Integer rowIndex=1 ; rowIndex<=500 ; ++rowIndex) {
        for (M::Integer columnIndex=1 ; columnIndex<=100 ; ++columnIndex) {
            d[i] = m(rowIndex, columnIndex);
            ++i;
        }
    }

    double sum        = 0;
    double sumSquared = 0;

    QVector<double> h = histogram(d, 0, 1, 100);
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double error = h.at(i) - 0.01;
        sum        += error;
        sumSquared += error * error;
    }

    double avg    = sum / 100.0;
    double stddev = std::sqrt(sumSquared / 100.0);

    qDebug() << "average error = " << avg << ", standard deviation = " << stddev;

    QCOMPARE(std::abs(avg) < 1.0E-10, true);
    QCOMPARE(stddev < 0.02, true);
}


void TestMatrixRandomFunctions::testMatrixRealRandomInclusiveExclusive() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };
    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    M::MatrixReal m = M::MatrixReal::randomInclusiveExclusive(pt, 500, 100);
    QVector<double>  d(50000);

    unsigned i = 0;
    for (M::Integer rowIndex=1 ; rowIndex<=500 ; ++rowIndex) {
        for (M::Integer columnIndex=1 ; columnIndex<=100 ; ++columnIndex) {
            M::Real v = m(rowIndex, columnIndex);

            QCOMPARE(v != 1.0, true);

            d[i] = v;
            ++i;
        }
    }

    double sum        = 0;
    double sumSquared = 0;

    QVector<double> h = histogram(d, 0, 1, 100);
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double error = h.at(i) - 0.01;
        sum        += error;
        sumSquared += error * error;
    }

    double avg    = sum / 100.0;
    double stddev = std::sqrt(sumSquared / 100.0);

    qDebug() << "average error = " << avg << ", standard deviation = " << stddev;

    QCOMPARE(std::abs(avg) < 1.0E-10, true);
    QCOMPARE(stddev < 0.02, true);
}


void TestMatrixRandomFunctions::testMatrixRealRandomExclusiveInclusive() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };
    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    M::MatrixReal m = M::MatrixReal::randomExclusiveInclusive(pt, 500, 100);
    QVector<double>  d(50000);

    unsigned i = 0;
    for (M::Integer rowIndex=1 ; rowIndex<=500 ; ++rowIndex) {
        for (M::Integer columnIndex=1 ; columnIndex<=100 ; ++columnIndex) {
            M::Real v = m(rowIndex, columnIndex);

            QCOMPARE(v != 0.0, true);

            d[i] = v;
            ++i;
        }
    }

    double sum        = 0;
    double sumSquared = 0;

    QVector<double> h = histogram(d, 0, 1, 100);
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double error = h.at(i) - 0.01;
        sum        += error;
        sumSquared += error * error;
    }

    double avg    = sum / 100.0;
    double stddev = std::sqrt(sumSquared / 100.0);

    qDebug() << "average error = " << avg << ", standard deviation = " << stddev;

    QCOMPARE(std::abs(avg) < 1.0E-10, true);
    QCOMPARE(stddev < 0.02, true);
}


void TestMatrixRandomFunctions::testMatrixRealRandomExclusive() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };
    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    M::MatrixReal m = M::MatrixReal::randomExclusive(pt, 500, 100);
    QVector<double>  d(50000);

    unsigned i = 0;
    for (M::Integer rowIndex=1 ; rowIndex<=500 ; ++rowIndex) {
        for (M::Integer columnIndex=1 ; columnIndex<=100 ; ++columnIndex) {
            M::Real v = m(rowIndex, columnIndex);

            QCOMPARE(v != 0.0, true);
            QCOMPARE(v != 1.0, true);

            d[i] = v;
            ++i;
        }
    }

    double sum        = 0;
    double sumSquared = 0;

    QVector<double> h = histogram(d, 0, 1, 100);
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        double error = h.at(i) - 0.01;
        sum        += error;
        sumSquared += error * error;
    }

    double avg    = sum / 100.0;
    double stddev = std::sqrt(sumSquared / 100.0);

    qDebug() << "average error = " << avg << ", standard deviation = " << stddev;

    QCOMPARE(std::abs(avg) < 1.0E-10, true);
    QCOMPARE(stddev < 0.02, true);
}


void TestMatrixRandomFunctions::testMatrixRealRandomNormal1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };
    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    M::MatrixReal m = M::MatrixReal::randomNormal(pt, 1000, 1000);
    QVector<double>  d(1000000);

    unsigned i = 0;
    for (M::Integer rowIndex=1 ; rowIndex<=1000 ; ++rowIndex) {
        for (M::Integer columnIndex=1 ; columnIndex<=1000 ; ++columnIndex) {
            M::Real v = m(rowIndex, columnIndex);
            d[i] = v;
            ++i;
        }
    }

    QVector<double> h = histogram(d, -4.0, +4.0, 100);
    QVector<double> pdf(h.size());

    M::Real bucketWidth = 8.0 / 100.0;
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        M::Real x = bucketWidth * static_cast<M::Real>(i) - 4.0;
        M::Real p = (1.0 / (std::sqrt(2.0 * M::pi))) * std::exp(-0.5 * x * x);
        pdf[i] = p * bucketWidth;
    }

    double sum        = 0;
    double sumSquared = 0;

    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = h.at(i) != 0 ? (h.at(i) - pdf.at(i)) / pdf.at(i) : 0;
        sum        += error;
        sumSquared += error * error;
    }

    double avg    = sum / 100.0;
    double stddev = std::sqrt(sumSquared / 100.0);

    qDebug() << "average error = " << avg << ", standard deviation = " << stddev;

    QCOMPARE(std::abs(avg) < 0.005, true);
    QCOMPARE(stddev < 0.2, true);
}


void TestMatrixRandomFunctions::testMatrixRealRandomNormal2() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };
    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    for (unsigned trial=0 ; trial<10 ; ++trial) {
        M::Real mean  = pt.randomInclusive() * 2.0 - 1.0;
        M::Real sigma = pt.randomExclusiveInclusive() * 2.0;
        M::MatrixReal m = M::MatrixReal::randomNormal(pt, 1000, 1000, mean, sigma);

        QVector<double>  d(1000000);

        unsigned i = 0;
        for (M::Integer rowIndex=1 ; rowIndex<=1000 ; ++rowIndex) {
            for (M::Integer columnIndex=1 ; columnIndex<=1000 ; ++columnIndex) {
                M::Real v = m(rowIndex, columnIndex);
                d[i] = v;
                ++i;
            }
        }

        QVector<double> h = histogram(d, mean - 4.0 * sigma, mean + 4.0 * sigma, 100);
        QVector<double> pdf(h.size());

        M::Real bucketWidth = (8.0 * sigma) / 100.0;
        for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
            M::Real x = bucketWidth * static_cast<M::Real>(i) + mean - 4.0 * sigma;
            M::Real t = (x - mean) / sigma;
            M::Real p = (1.0 / (sigma * std::sqrt(2.0 * M::pi))) * std::exp(-0.5 * t * t);
            pdf[i] = p * bucketWidth;
        }

        double sum        = 0;
        double sumSquared = 0;

        for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
            double error = h.at(i) != 0 ? (h.at(i) - pdf.at(i)) / pdf.at(i) : 0;
            sum        += error;
            sumSquared += error * error;
        }

        double avg    = sum / 100.0;
        double stddev = std::sqrt(sumSquared / 100.0);

        qDebug() << "average error = " << avg << ", standard deviation = " << stddev;

        QCOMPARE(std::abs(avg) < 0.02, true);
        QCOMPARE(stddev < 0.2, true);
    }
}


void TestMatrixRandomFunctions::testMatrixRealRandomWeibull() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };
    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    for (unsigned trial=0 ; trial<10 ; ++trial) {
        M::Real scale = pt.randomInclusive() * 1.8 + 0.2;
        M::Real shape = pt.randomInclusive() * 1.8 + 0.2;
        M::Real delay = pt.randomInclusive();
        M::MatrixReal m = M::MatrixReal::randomWeibull(pt, 1000, 1000, scale, shape, delay);

        QVector<double> d(1000000);

        unsigned i = 0;
        for (M::Integer rowIndex=1 ; rowIndex<=1000 ; ++rowIndex) {
            for (M::Integer columnIndex=1 ; columnIndex<=1000 ; ++columnIndex) {
                M::Real v = m(rowIndex, columnIndex);
                d[i] = v;
                ++i;
            }
        }

        double minimum = delay;
        double maximum = delay + 4.0 * scale / shape;
        QVector<double> h = histogram(d, minimum, maximum, 100);
        QVector<double> pdf(h.size());

        M::Real bucketWidth = (maximum - minimum) / 100.0;
        for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
            M::Real x = bucketWidth * static_cast<M::Real>(i) + minimum;

            M::Real p;
            if (x < delay) {
                p = 0;
            } else {
                M::Real xd = x - delay;
                p = (shape / scale) * std::pow(xd / scale, shape - 1.0) * std::exp(-std::pow(xd / scale, shape));
            }

            pdf[i] = p * bucketWidth;
        }

        double sum        = 0;
        double sumSquared = 0;

        for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
            double error = h.at(i) != 0 ? (h.at(i) - pdf.at(i)) / pdf.at(i) : 0;
            sum        += error;
            sumSquared += error * error;
        }

        double avg    = sum / 100.0;
        double stddev = std::sqrt(sumSquared / 100.0);

        qDebug() << "average error = " << avg << ", standard deviation = " << stddev;

        QCOMPARE(std::abs(avg) < 0.03, true);
        QCOMPARE(stddev < 0.06, true);
    }
}


void TestMatrixRandomFunctions::testMatrixRealRandomExponential() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };
    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    for (unsigned trial=0 ; trial<10 ; ++trial) {
        M::Real rate = pt.randomInclusive() * 1.8 + 0.2;
        M::MatrixReal m = M::MatrixReal::randomExponential(pt, 1000, 1000, rate);

        QVector<double> d(1000000);

        unsigned i = 0;
        for (M::Integer rowIndex=1 ; rowIndex<=1000 ; ++rowIndex) {
            for (M::Integer columnIndex=1 ; columnIndex<=1000 ; ++columnIndex) {
                M::Real v = m(rowIndex, columnIndex);
                d[i] = v;
                ++i;
            }
        }

        double minimum = 0;
        double maximum = 4.0 / rate;
        QVector<double> h = histogram(d, minimum, maximum, 100);
        QVector<double> pdf(h.size());

        M::Real bucketWidth = (maximum - minimum) / 100.0;
        for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
            M::Real x = bucketWidth * static_cast<M::Real>(i) + minimum;
            M::Real p = rate * std::exp(-rate * x);
            pdf[i] = p * bucketWidth;
        }

        double sum        = 0;
        double sumSquared = 0;

        for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
            double error = h.at(i) != 0 ? (h.at(i) - pdf.at(i)) / pdf.at(i) : 0;
            sum        += error;
            sumSquared += error * error;
        }

        double avg    = sum / 100.0;
        double stddev = std::sqrt(sumSquared / 100.0);

        qDebug() << "average error = " << avg << ", standard deviation = " << stddev;

        QCOMPARE(std::abs(avg) < 0.025, true);
        QCOMPARE(stddev < 0.03, true);
    }
}


void TestMatrixRandomFunctions::testMatrixRealRandomGamma() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };
    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    for (unsigned trial=0 ; trial<10 ; ++trial) {
        M::Real k = pt.randomInclusive() * 4.5 + 0.5;
        M::Real s = pt.randomInclusive() * 1.5 + 0.5;
        M::MatrixReal m = M::MatrixReal::randomGamma(pt, 10000, 3000, k, s);

        QVector<double> d(30000000);

        unsigned i = 0;
        for (M::Integer rowIndex=1 ; rowIndex<=10000 ; ++rowIndex) {
            for (M::Integer columnIndex=1 ; columnIndex<=3000 ; ++columnIndex) {
                M::Real v = m(rowIndex, columnIndex);
                d[i] = v;
                ++i;
            }
        }

        double minimum = 0;
        double maximum = 3.0 * s;
        QVector<double> h = histogram(d, minimum, maximum, 100);
        QVector<double> pdf(h.size());

        M::Real bucketWidth = (maximum - minimum) / 100.0;
        for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
            M::Real x = bucketWidth * static_cast<M::Real>(i) + minimum;
            M::Real p = (1.0 / (M::gamma(k) * std::pow(s, k))) * std::pow(x, k - 1) * std::exp(- x / s);
            pdf[i] = p * bucketWidth;
        }

        double sum        = 0;
        double sumSquared = 0;

        for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
            double error = h.at(i) != 0 ? (h.at(i) - pdf.at(i)) / pdf.at(i) : 0;
            sum        += error;
            sumSquared += error * error;
        }

        double avg    = sum / 100.0;
        double stddev = std::sqrt(sumSquared / 100.0);

        qDebug() << "average error = " << avg << ", standard deviation = " << stddev;

        QCOMPARE(std::abs(avg) < 0.12, true);
        QCOMPARE(stddev < 0.6, true);
    }
}


void TestMatrixRandomFunctions::testMatrixRealRandomRayleight() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };
    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    for (unsigned trial=0 ; trial<10 ; ++trial) {
        M::Real scale = pt.randomInclusive() * 1.5 + 0.5;
        M::MatrixReal m = M::MatrixReal::randomRayleigh(pt, 1000, 1000, scale);

        QVector<double> d(1000000);

        unsigned i = 0;
        for (M::Integer rowIndex=1 ; rowIndex<=1000 ; ++rowIndex) {
            for (M::Integer columnIndex=1 ; columnIndex<=1000 ; ++columnIndex) {
                M::Real v = m(rowIndex, columnIndex);
                d[i] = v;
                ++i;
            }
        }

        double minimum = 0;
        double maximum = 2.0 * scale;
        QVector<double> h = histogram(d, minimum, maximum, 100);
        QVector<double> pdf(h.size());

        M::Real bucketWidth = (maximum - minimum) / 100.0;
        for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
            M::Real x = bucketWidth * static_cast<M::Real>(i) + minimum;
            M::Real p = (x / (scale * scale)) * std::exp(-x * x / (2 * scale * scale));
            pdf[i] = p * bucketWidth;
        }

        double sum        = 0;
        double sumSquared = 0;

        for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
            double error = h.at(i) != 0 ? (h.at(i) - pdf.at(i)) / pdf.at(i) : 0;
            sum        += error;
            sumSquared += error * error;
        }

        double avg    = sum / 100.0;
        double stddev = std::sqrt(sumSquared / 100.0);

        qDebug() << "average error = " << avg << ", standard deviation = " << stddev;

        QCOMPARE(std::abs(avg) < 0.03, true);
        QCOMPARE(stddev < 0.1, true);
    }
}


void TestMatrixRandomFunctions::testMatrixRealRandomChiSquared() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };
    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    for (unsigned trial=0 ; trial<10 ; ++trial) {
        M::Real k = pt.randomInclusive() * 5.5 + 0.5;
        M::MatrixReal m = M::MatrixReal::randomChiSquared(pt, 10000, 10000, k);

        QVector<double> d(100000000);

        unsigned i = 0;
        for (M::Integer rowIndex=1 ; rowIndex<=10000 ; ++rowIndex) {
            for (M::Integer columnIndex=1 ; columnIndex<=10000 ; ++columnIndex) {
                M::Real v = m(rowIndex, columnIndex);
                d[i] = v;
                ++i;
            }
        }

        double minimum = 0;
        double maximum = 6.0 * k;
        QVector<double> h = histogram(d, minimum, maximum, 100);
        QVector<double> pdf(h.size());

        M::Real bucketWidth = (maximum - minimum) / 100.0;
        M::Real c = (1.0 / (std::pow(2.0, k /2.0) * M::gamma(k / 2.0)));
        for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
            M::Real x = bucketWidth * static_cast<M::Real>(i) + minimum;
            M::Real p = c * std::pow(x, (k / 2.0) - 1) * std::exp(-x / 2.0);
            pdf[i] = p * bucketWidth;
        }

        double sum        = 0;
        double sumSquared = 0;

        for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
            double error = h.at(i) != 0 ? (h.at(i) - pdf.at(i)) / pdf.at(i) : 0;
            sum        += error;
            sumSquared += error * error;
        }

        double avg    = sum / 100.0;
        double stddev = std::sqrt(sumSquared / 100.0);

        qDebug() << "average error = " << avg << ", standard deviation = " << stddev;

        QCOMPARE(std::abs(avg) < 0.5, true);
        QCOMPARE(stddev < 0.6, true);
    }
}


void TestMatrixRandomFunctions::testMatrixRealRandomLogNormal1() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };
    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    M::MatrixReal m = M::MatrixReal::randomLogNormal(pt, 1000, 1000);
    QVector<double>  d(1000000);

    unsigned i = 0;
    for (M::Integer rowIndex=1 ; rowIndex<=1000 ; ++rowIndex) {
        for (M::Integer columnIndex=1 ; columnIndex<=1000 ; ++columnIndex) {
            M::Real v = m(rowIndex, columnIndex);
            d[i] = v;
            ++i;
        }
    }

    double minimum = std::exp(-5.0);
    double maximum = std::exp(5.0);
    QVector<double> h = histogram(d, minimum, maximum, 100);
    QVector<double> pdf(h.size());

    M::Real bucketWidth = (maximum - minimum) / 100.0;
    for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
        M::Real x = bucketWidth * static_cast<M::Real>(i) + minimum;
        M::Real t = std::log(x);
        M::Real p = (1.0 / (x * std::sqrt(2.0 * M::pi))) * std::exp(-0.5 * t * t);
        pdf[i] = p * bucketWidth;
    }

    double sum        = 0;
    double sumSquared = 0;

    for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
        double error = h.at(i) != 0 ? (h.at(i) - pdf.at(i)) / pdf.at(i) : 0;
        sum        += error;
        sumSquared += error * error;
    }

    double avg    = sum / 100.0;
    double stddev = std::sqrt(sumSquared / 100.0);

    qDebug() << "average error = " << avg << ", standard deviation = " << stddev;

    QCOMPARE(std::abs(avg) < 0.1, true);
    QCOMPARE(stddev < 0.8, true);
}


void TestMatrixRandomFunctions::testMatrixRealRandomLogNormal2() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };
    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    for (unsigned trial=0 ; trial<10 ; ++trial) {
        M::Real mean  = pt.randomInclusive() * 2.0 - 1.0;
        M::Real sigma = pt.randomExclusiveInclusive() * 2.0;
        M::MatrixReal m = M::MatrixReal::randomLogNormal(pt, 1000, 1000, mean, sigma);

        QVector<double>  d(1000000);

        unsigned i = 0;
        for (M::Integer rowIndex=1 ; rowIndex<=1000 ; ++rowIndex) {
            for (M::Integer columnIndex=1 ; columnIndex<=1000 ; ++columnIndex) {
                M::Real v = m(rowIndex, columnIndex);
                d[i] = v;
                ++i;
            }
        }

        double minimum = std::exp(mean - 5.0 * sigma);
        double maximum = std::exp(mean + 5.0 * sigma);
        QVector<double> h = histogram(d, minimum, maximum, 100);
        QVector<double> pdf(h.size());

        M::Real bucketWidth = (maximum - minimum) / 100.0;
        for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
            M::Real x = bucketWidth * static_cast<M::Real>(i) + minimum;
            M::Real t = (std::log(x) - mean) / sigma;
            M::Real p = (1.0 / (x * sigma * std::sqrt(2.0 * M::pi))) * std::exp(-0.5 * t * t);
            pdf[i] = p * bucketWidth;
        }

        double sum        = 0;
        double sumSquared = 0;

        for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
            double error = h.at(i) != 0 ? (h.at(i) - pdf.at(i)) / pdf.at(i) : 0;
            sum        += error;
            sumSquared += error * error;
        }

        double avg    = sum / 100.0;
        double stddev = std::sqrt(sumSquared / 100.0);

        qDebug() << "average error = " << avg << ", standard deviation = " << stddev;

        QCOMPARE(std::abs(avg) < 0.4, true);
        QCOMPARE(stddev < 2, true); // This value is quite large.  Shape seems roughly correct.  Needs investigation.
    }
}


void TestMatrixRandomFunctions::testMatrixRealRandomCauchyLorentz() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };
    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    for (unsigned trial=0 ; trial<10 ; ++trial) {
        M::Real location = pt.randomInclusive() * 2.0 - 1.0;
        M::Real scale    = pt.randomInclusive() * 1.5 + 0.5;
        M::MatrixReal m = M::MatrixReal::randomCauchyLorentz(pt, 1000, 1000, location, scale);

        QVector<double> d(1000000);

        unsigned i = 0;
        for (M::Integer rowIndex=1 ; rowIndex<=1000 ; ++rowIndex) {
            for (M::Integer columnIndex=1 ; columnIndex<=1000 ; ++columnIndex) {
                M::Real v = m(rowIndex, columnIndex);
                d[i] = v;
                ++i;
            }
        }

        double minimum = location - 6.0 * scale;
        double maximum = location + 6.0 * scale;
        QVector<double> h = histogram(d, minimum, maximum, 100);
        QVector<double> pdf(h.size());

        M::Real bucketWidth = (maximum - minimum) / 100.0;
        for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
            M::Real x = bucketWidth * static_cast<M::Real>(i) + minimum;
            M::Real t = (x - location) / scale;
            M::Real p = 1.0 / (M::pi * scale * (1.0 + t * t));
            pdf[i] = p * bucketWidth;
        }

        double sum        = 0;
        double sumSquared = 0;

        for (unsigned i=1 ; i<static_cast<unsigned>(h.size() - 1) ; ++i) {
            double error = h.at(i) != 0 ? (h.at(i) - pdf.at(i)) / pdf.at(i) : 0;
            sum        += error;
            sumSquared += error * error;
        }

        double avg    = sum / 100.0;
        double stddev = std::sqrt(sumSquared / 100.0);

        qDebug() << "average error = " << avg << ", standard deviation = " << stddev;

        QCOMPARE(std::abs(avg) < 0.005, true);
        QCOMPARE(stddev < 0.05, true);
    }
}


void TestMatrixRandomFunctions::testMatrixIntegerRandomPoisson() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };
    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    for (unsigned trial=0 ; trial<10 ; ++trial) {
        M::Real rate = pt.randomInclusive() * 3.75 + 0.25;
        M::MatrixInteger m = M::MatrixInteger::randomPoisson(pt, 1000, 1000, rate);

        QVector<M::Integer> d(1000000);

        unsigned i = 0;
        for (M::Integer rowIndex=1 ; rowIndex<=1000 ; ++rowIndex) {
            for (M::Integer columnIndex=1 ; columnIndex<=1000 ; ++columnIndex) {
                M::Real v = m(rowIndex, columnIndex);
                d[i] = v;
                ++i;
            }
        }

        QVector<M::Real> h;
        for (unsigned long i=0 ; i<static_cast<unsigned long>(d.size()) ; ++i) {
            M::Integer v = d[i];
            QCOMPARE(v >= 0, true);

            while (v >= h.size()) {
                h.append(0);
            }

            ++h[v];
        }

        for (unsigned long i=0 ; i<static_cast<unsigned long>(h.size()) ; ++i) {
            h[i] /= static_cast<M::Real>(d.size());
        }

        QVector<M::Real> pmf;
        for (unsigned long i=0 ; i<static_cast<unsigned long>(h.size()) ; ++i) {
            M::Real p = std::exp(i * std::log(rate) - rate - M::lnFactorial(M::Integer(i)));
            pmf.append(p);
        }

        double sum        = 0;
        double sumSquared = 0;

        for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
            double error = h.at(i) != 0 ? (h.at(i) - pmf.at(i)) / pmf.at(i) : 0;
            sum        += error;
            sumSquared += error * error;
        }

        double avg    = sum / 100.0;
        double stddev = std::sqrt(sumSquared / 100.0);

        qDebug() << "average error = " << avg << ", standard deviation = " << stddev;

        QCOMPARE(std::abs(avg) < 0.06, true);
        QCOMPARE(stddev < 0.6, true);
    }
}


void TestMatrixRandomFunctions::testMatrixIntegerRandomBinomial() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };
    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    for (unsigned trial=0 ; trial<10 ; ++trial) {
        M::Integer n = (std::abs(pt.randomInteger()) % 16 + 1);
        M::Real    p = pt.randomExclusive();
        M::MatrixInteger m = M::MatrixInteger::randomBinomial(pt, 1000, 1000, n, p);

        QVector<M::Integer> d(1000000);

        unsigned i = 0;
        for (M::Integer rowIndex=1 ; rowIndex<=1000 ; ++rowIndex) {
            for (M::Integer columnIndex=1 ; columnIndex<=1000 ; ++columnIndex) {
                M::Real v = m(rowIndex, columnIndex);
                d[i] = v;
                ++i;
            }
        }

        QVector<M::Real> h;
        for (unsigned long i=0 ; i<static_cast<unsigned long>(d.size()) ; ++i) {
            M::Integer v = d[i];
            QCOMPARE(v >= 0, true);

            while (v >= h.size()) {
                h.append(0);
            }

            ++h[v];
        }

        for (unsigned long i=0 ; i<static_cast<unsigned long>(h.size()) ; ++i) {
            h[i] /= static_cast<M::Real>(d.size());
        }

        QVector<M::Real> pmf;
        for (unsigned long i=0 ; i<static_cast<unsigned long>(h.size()) ; ++i) {
            M::Real pv = M::binomial(M::Integer(n), M::Integer(i)) * std::pow(p, i) * std::pow(1.0 - p, n - i);
            pmf.append(pv);
        }

        double sum        = 0;
        double sumSquared = 0;

        for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
            double error = h.at(i) != 0 ? (h.at(i) - pmf.at(i)) / pmf.at(i) : 0;
            sum        += error;
            sumSquared += error * error;
        }

        double avg    = sum / 100.0;
        double stddev = std::sqrt(sumSquared / 100.0);

        qDebug() << "average error = " << avg << ", standard deviation = " << stddev;

        QCOMPARE(std::abs(avg) < 0.01, true);
        QCOMPARE(stddev < 0.08, true);
    }
}


void TestMatrixRandomFunctions::testMatrixIntegerRandomGeometric() {
    M::PerThread::RngSeed rngSeed = {
        0x123456789ABCDEF0ULL,
        0x132457689BACDFE0ULL,
        0x10FEDCBA98765432ULL,
        0x76543210FEDCBA98ULL
    };
    M::PerThread pt(1, M::PerThread::RngType::MT19937, rngSeed, Q_NULLPTR);

    for (unsigned trial=0 ; trial<10 ; ++trial) {
        M::Real          p = pt.randomExclusive();
        M::MatrixInteger m = M::MatrixInteger::randomGeometric(pt, 3000, 3000, p);

        QVector<M::Integer> d(9000000);

        unsigned i = 0;
        for (M::Integer rowIndex=1 ; rowIndex<=3000 ; ++rowIndex) {
            for (M::Integer columnIndex=1 ; columnIndex<=3000 ; ++columnIndex) {
                M::Real v = m(rowIndex, columnIndex);
                d[i] = v;
                ++i;
            }
        }

        QVector<M::Real> h;
        for (unsigned long i=0 ; i<static_cast<unsigned long>(d.size()) ; ++i) {
            M::Integer v = d[i];
            QCOMPARE(v >= 0, true);

            while (v >= h.size()) {
                h.append(0);
            }

            ++h[v];
        }

        for (unsigned long i=0 ; i<static_cast<unsigned long>(h.size()) ; ++i) {
            h[i] /= static_cast<M::Real>(d.size());
        }

        QVector<M::Real> pmf;
        pmf.append(0);
        for (unsigned long i=1 ; i<static_cast<unsigned long>(h.size()) ; ++i) {
            M::Real pv = std::pow(1.0 - p, i - 1) * p;
            pmf.append(pv);
        }

        double sum        = 0;
        double sumSquared = 0;

        for (unsigned i=0 ; i<static_cast<unsigned>(h.size()) ; ++i) {
            double error = h.at(i) != 0 ? (h.at(i) - pmf.at(i)) / pmf.at(i) : 0;
            sum        += error;
            sumSquared += error * error;
        }

        double avg    = sum / 100.0;
        double stddev = std::sqrt(sumSquared / 100.0);

        qDebug() << "average error = " << avg << ", standard deviation = " << stddev;

        QCOMPARE(std::abs(avg) < 0.25, true);
        QCOMPARE(stddev < 1.1, true);
    }
}


QVector<double> TestMatrixRandomFunctions::histogram(
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


QVector<double> TestMatrixRandomFunctions::periodicAutocorrelation(const QVector<double>& input) {
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


double TestMatrixRandomFunctions::averageCorrelation(const QVector<double>& input) {
    unsigned long inputLength    = static_cast<unsigned>(input.size());
    long double   sumCorrelation = 0;

    for (unsigned long i=1 ; i<inputLength ; ++i) {
        sumCorrelation += input.at(i);
    }

    return sumCorrelation / static_cast<double>(inputLength);
}


double TestMatrixRandomFunctions::maximumCorrelation(const QVector<double>& input) {
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
