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
* This header provides tests for the random function in \ref M::MatrixInteger and \ref M::MatrixReal class.
***********************************************************************************************************************/

#ifndef TEST_MATRIX_RANDOM_FUNCTIONS_H
#define TEST_MATRIX_RANDOM_FUNCTIONS_H

#include <QObject>
#include <QVector>
#include <QtTest/QtTest>

#include "test_matrix_base.h"

namespace Model {
    class MatrixInteger;
}

class TestMatrixRandomFunctions:public TestMatrixBase {
    Q_OBJECT

    public:
        TestMatrixRandomFunctions();

        ~TestMatrixRandomFunctions() override;

    private slots:
        void testMatrixRealRandomInclusive();

        void testMatrixRealRandomInclusiveExclusive();

        void testMatrixRealRandomExclusiveInclusive();

        void testMatrixRealRandomExclusive();

        void testMatrixIntegerRandomInteger();

        void testMatrixRealRandomNormal1();

        void testMatrixRealRandomNormal2();

        void testMatrixRealRandomWeibull();

        void testMatrixRealRandomExponential();

        void testMatrixRealRandomGamma();

        void testMatrixRealRandomRayleight();

        void testMatrixRealRandomChiSquared();

        void testMatrixRealRandomLogNormal1();

        void testMatrixRealRandomLogNormal2();

        void testMatrixRealRandomCauchyLorentz();

        void testMatrixIntegerRandomPoisson();

        void testMatrixIntegerRandomBinomial();

        void testMatrixIntegerRandomGeometric();

    private:
        static const unsigned numberIterations = 10;

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
