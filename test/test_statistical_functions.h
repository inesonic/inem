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
* This header provides tests of the statistical math functions.
***********************************************************************************************************************/

#ifndef TEST_STATISTICAL_FUNCTIONS_H
#define TEST_STATISTICAL_FUNCTIONS_H

#include <QObject>
#include <QtTest/QtTest>
#include <QVector>

#include "m_intrinsic_types.h"

class TestStatisticalFunctions:public QObject {
    Q_OBJECT

    public:
        TestStatisticalFunctions();

        ~TestStatisticalFunctions() override;

    private slots:
        void testSortSet();

        void testSortTuple();

        void testSortMatrixBoolean();

        void testSortMatrixInteger();

        void testSortMatrixReal();

        void testSortMatrixComplex();

        void testRandomInteger1();

        void testRandomIntegerMatrix1();

        void testRandomIntegerMatrix2();

        void testRandomInteger32();

        void testUniformDeviateInclusive1();

        void testUniformDeviateInclusiveMatrix1();

        void testUniformDeviateInclusiveMatrix2();

        void testUniformDeviateExclusive1();

        void testUniformDeviateExclusiveMatrix1();

        void testUniformDeviateExclusiveMatrix2();

        void testNormalDeviate1();

        void testNormalDeviate2();

        void testNormalDeviateMatrix1();

        void testNormalDeviateMatrix2();

        void testNormalPdf1();

        void testNormalPdf2();

        void testNormalCdf1();

        void testNormalCdf2();

        void testNormalQuantile1();

        void testNormalQuantile2();

        void testGammaDeviate1();

        void testGammaDeviate2();

        void testGammaDeviateMatrix1();

        void testGammaDeviateMatrix2();

        void testGammaPdf1();

        void testGammaPdf2();

        void testGammaCdf1();

        void testGammaCdf2();

        void testGammaQuantile1();

        void testGammaQuantile2();

        void testWeibullDeviate1();

        void testWeibullDeviate2();

        void testWeibullDeviateMatrix1();

        void testWeibullDeviateMatrix2();

        void testWeibullPdf1();

        void testWeibullPdf2();

        void testWeibullCdf1();

        void testWeibullCdf2();

        void testWeibullQuantile1();

        void testWeibullQuantile2();

        void testExponentialDeviate1();

        void testExponentialDeviate2();

        void testExponentialDeviateMatrix1();

        void testExponentialDeviateMatrix2();

        void testExponentialPdf1();

        void testExponentialPdf2();

        void testExponentialCdf1();

        void testExponentialCdf2();

        void testExponentialQuantile1();

        void testExponentialQuantile2();

        void testRayleighDeviate1();

        void testRayleighDeviate2();

        void testRayleighDeviateMatrix1();

        void testRayleighDeviateMatrix2();

        void testRayleighPdf1();

        void testRayleighPdf2();

        void testRayleighCdf1();

        void testRayleighCdf2();

        void testRayleighQuantile1();

        void testRayleighQuantile2();

        void testChiSquaredDeviate1();

        void testChiSquaredDeviate2();

        void testChiSquaredDeviateMatrix1();

        void testChiSquaredDeviateMatrix2();

        void testChiSquaredPdf1();

        void testChiSquaredPdf2();

        void testChiSquaredCdf1();

        void testChiSquaredCdf2();

        void testChiSquaredQuantile1();

        void testChiSquaredQuantile2();

        void testPoissonDeviate1();

        void testPoissonDeviate2();

        void testPoissonDeviateMatrix1();

        void testPoissonDeviateMatrix2();

        void testPoissonPmf1();

        void testPoissonPmf2();

        void testPoissonCdf1();

        void testPoissonCdf2();

        void testBinomialDeviate1();

        void testBinomialDeviate2();

        void testBinomialDeviateMatrix1();

        void testBinomialDeviateMatrix2();

        void testBinomialPmf1();

        void testBinomialPmf2();

        void testBinomialCdf1();

        void testBinomialCdf2();

        void testLogNormalDeviate1();

        void testLogNormalDeviate2();

        void testLogNormalDeviateMatrix1();

        void testLogNormalDeviateMatrix2();

        void testLogNormalPdf1();

        void testLogNormalPdf2();

        void testLogNormalCdf1();

        void testLogNormalCdf2();

        void testLogNormalQuantile1();

        void testLogNormalQuantile2();

        void testGeometricDeviate1();

        void testGeometricDeviate2();

        void testGeometricDeviateMatrix1();

        void testGeometricDeviateMatrix2();

        void testGeometricPmf1();

        void testGeometricPmf2();

        void testGeometricCdf1();

        void testGeometricCdf2();

        void testCauchyLorentzDeviate1();

        void testCauchyLorentzDeviate2();

        void testCauchyLorentzDeviateMatrix1();

        void testCauchyLorentzDeviateMatrix2();

        void testCauchyLorentzPdf1();

        void testCauchyLorentzPdf2();

        void testCauchyLorentzCdf1();

        void testCauchyLorentzCdf2();

        void testCauchyLorentzQuantile1();

        void testCauchyLorentzQuantile2();

        void testCount();

        void testSum();

        void testAverage();

        void testMinimum();

        void testMaximum();

        void testVariance();

        void testStdDev();

        void testSampleStdDev();

        void testMedian();

        void testMode();

        void testSampleSkew();

        void testExcessKurtosis();

        void testHistogram();

    private:
        static constexpr unsigned      numberPeriodicAutocorrelationPoints = 10000;
        static constexpr unsigned      numberIterations                    = 100000;
        static constexpr M::Integer    matrixRows                          = 1000;
        static constexpr M::Integer    matrixColumns                       = 1000;
        static constexpr unsigned long numberSpeedIterations               = 100000000;

        QVector<double> histogram(
            const QVector<double>& values,
            double                 firstBucket,
            double                 lastBucket,
            unsigned               numberBuckets
        );

        QVector<double> periodicAutocorrelation(const QVector<double>& input);
        double averageCorrelation(const QVector<double>& input);
        double maximumCorrelation(const QVector<double>& input);
};

#endif
