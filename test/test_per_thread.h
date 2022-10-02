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
* This header provides tests for the \ref M::PerThread class.
***********************************************************************************************************************/

#ifndef TEST_PER_THREAD_H
#define TEST_PER_THREAD_H

#include <QObject>
#include <QVector>
#include <QtTest/QtTest>

#include <m_intrinsic_types.h>

class TestPerThread:public QObject {
    Q_OBJECT

    public:
        TestPerThread();

        ~TestPerThread() override;

    private slots:
        void testMT19937();

        void testMT216091();

        void testXorShiRo256Plus();

        void testXorShiRo256Stars();

        void testTRNG();

        void testMT19937Array();

        void testMT216091Array();

        void testXorShiRo256PlusArray();

        void testXorShiRo256StarsArray();

        void testTRNGArray();

        void testSpeed();

        void testAutoCorrelation();

        void testInclusive();

        void testExclusive();

        void testNormal();

        void testWeibull();

        void testExponential();

        void testGamma();

        void testRayleigh();

        void testChiSquared();

        void testPoisson();

        void testBinomial();

        void testLogNormal();

        void testGeometric();

        void testCauchyLorentz();

        void testThreadId();

        void testOperationHandle();

    private:
        QVector<double> histogram(
            const QVector<double>& values,
            double                 firstBucket,
            double                 lastBucket,
            unsigned               numberBuckets
        );

        static constexpr unsigned      numberIterations      = 6000; // 60000;
        static constexpr unsigned long numberSpeedIterations = 100000000;

        QVector<double> periodicAutocorrelation(const QVector<double>& input);
        double averageCorrelation(const QVector<double>& input);
        double maximumCorrelation(const QVector<double>& input);
};

#endif
