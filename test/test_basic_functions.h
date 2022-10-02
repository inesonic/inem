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
* This header provides tests of the basic math functions.
***********************************************************************************************************************/

#ifndef TEST_BASIC_FUNCTIONS_H
#define TEST_BASIC_FUNCTIONS_H

#include <QObject>
#include <QtTest/QtTest>

#include <m_intrinsic_types.h>

class TestBasicFunctions:public QObject {
    Q_OBJECT

    public:
        TestBasicFunctions();

        ~TestBasicFunctions() override;

    private slots:
        void testZetaInteger();

        void testZetaReal();

        void testZetaComplex();

        void testZetaVariant();

        void testStdPowComplexRealValues();

        void testIntegerPowIntgerInteger();

        void testRealPowRealInteger();

        void testComplexPowIntegerReal();

        void testComplexPowRealReal();

        void testComplexPowComplexInteger();

        void testComplexPowIntegerComplex();

        void testComplexPowComplexReal();

        void testComplexPowRealComplex();

        void testComplexPowComplexComplex();

        void testComplexPowVariants();

        void testComplexSqrtInteger();

        void testComplexSqrtReal();

        void testComplexSqrtComplex();

        void testComplexSqrtVariant();

        void testComplexNRootIntegerInteger();

        void testComplexNRootIntegerReal();

        void testComplexNRootIntegerComplex();

        void testComplexNRootRealInteger();

        void testComplexNRootRealReal();

        void testComplexNRootRealComplex();

        void testComplexNRootComplexInteger();

        void testComplexNRootComplexReal();

        void testComplexNRootComplexComplex();

        void testComplexNRootVariants();

        void testIntegerConjInteger();

        void testRealConjReal();

        void testComplexConjComplex();

        void testIntegerAbsInteger();

        void testRealAbsReal();

        void testRealAbsComplex();

        void testIntegerFloorInteger();

        void testRealFloorReal();

        void testRealFloorComplex();

        void testIntegerCeilInteger();

        void testRealFloorVariants();

        void testRealCeilReal();

        void testRealCeilComplex();

        void testRealCeilVariants();

        void testIntegerNIntInteger();

        void testRealNIntReal();

        void testRealNIntComplex();

        void testRealNIntVariants();

        void testRealFactorialReal();

        void testRealFactorialVariants();

        void testLnInteger();

        void testLnReal();

        void testLnComplex();

        void testLnVariants();

        void testLogIntegerInteger();

        void testLogIntegerReal();

        void testLogIntegerComplex();

        void testLogRealInteger();

        void testLogRealReal();

        void testLogRealComplex();

        void testLogComplexInteger();

        void testLogComplexReal();

        void testLogComplexComplex();

        void testLogVariants();

        void testGammaInteger();

        void testGammaReal();

        void testGammaComplex();

        void testGammaVariants();

        void testLnFactorialInteger();

        void testLnFactorialReal();

        void testLnFactorialComplex();

        void testLnFactorialVariants();

        void testLogFactorialIntegerInteger();

        void testLogFactorialIntegerReal();

        void testLogFactorialIntegerComplex();

        void testLogFactorialRealInteger();

        void testLogFactorialRealReal();

        void testLogFactorialRealComplex();

        void testLogFactorialComplexInteger();

        void testLogFactorialComplexReal();

        void testLogFactorialComplexComplex();

        void testLogFactorialVariants();

        void testLnGammaInteger();

        void testLnGammaReal();

        void testLnGammaComplex();

        void testLnGammaVariants();

        void testLogGammaIntegerInteger();

        void testLogGammaIntegerReal();

        void testLogGammaIntegerComplex();

        void testLogGammaRealInteger();

        void testLogGammaRealReal();

        void testLogGammaRealComplex();

        void testLogGammaComplexInteger();

        void testLogGammaComplexReal();

        void testLogGammaComplexComplex();

        void testLogGammaVariants();

        void testBinomialIntegerInteger();

        void testBinomialIntegerReal();

        void testBinomialRealInteger();

        void testBinomialRealReal();

        void testBinomialIntegerComplex();

        void testBinomialRealComplex();

        void testBinomialComplexInteger();

        void testBinomialComplexReal();

        void testBinomialComplexComplex();

        void testBinomialVariants();

        void testLnBinomial();

        void testLnBinomialVariants();

        void testLogBinomial();

        void testLogBinomialVariants();

        void testUnsignedSterlingNumber1IntegerInteger();

        void testUnsignedSterlingNumber1IntegerReal();

        void testUnsignedSterlingNumber1IntegerComplex();

        void testUnsignedSterlingNumber1RealInteger();

        void testUnsignedSterlingNumber1RealReal();

        void testUnsignedSterlingNumber1RealComplex();

        void testUnsignedSterlingNumber1ComplexInteger();

        void testUnsignedSterlingNumber1ComplexReal();

        void testUnsignedSterlingNumber1ComplexComplex();

        void testUnsignedSterlingNumber1Variant();

        void testSterlingNumber2IntegerInteger();

        void testSterlingNumber2IntegerReal();

        void testSterlingNumber2IntegerComplex();

        void testSterlingNumber2RealInteger();

        void testSterlingNumber2RealReal();

        void testSterlingNumber2RealComplex();

        void testSterlingNumber2ComplexInteger();

        void testSterlingNumber2ComplexReal();

        void testSterlingNumber2ComplexComplex();

        void testSterlingNumber2Variant();

        void testLowerGammaIntegerInteger();

        void testLowerGammaIntegerReal();

        void testLowerGammaIntegerComplex();

        void testLowerGammaRealInteger();

        void testLowerGammaRealReal();

        void testLowerGammaRealComplex();

        void testLowerGammaComplexInteger();

        void testLowerGammaComplexReal();

        void testLowerGammaComplexComplex();

        void testLowerGammaVariants();

        void testUpperGammaIntegerInteger();

        void testUpperGammaIntegerReal();

        void testUpperGammaIntegerComplex();

        void testUpperGammaRealInteger();

        void testUpperGammaRealReal();

        void testUpperGammaRealComplex();

        void testUpperGammaComplexInteger();

        void testUpperGammaComplexReal();

        void testUpperGammaComplexComplex();

        void testUpperGammaVariants();

        void testUpperLowerGammaComplexValues();

        void testNormalizedLowerGammaIntegerInteger();

        void testNormalizedLowerGammaIntegerReal();

        void testNormalizedLowerGammaIntegerComplex();

        void testNormalizedLowerGammaRealInteger();

        void testNormalizedLowerGammaRealReal();

        void testNormalizedLowerGammaRealComplex();

        void testNormalizedLowerGammaComplexInteger();

        void testNormalizedLowerGammaComplexReal();

        void testNormalizedLowerGammaComplexComplex();

        void testNormalizedLowerGammaVariants();

        void testNormalizedUpperGammaIntegerInteger();

        void testNormalizedUpperGammaIntegerReal();

        void testNormalizedUpperGammaIntegerComplex();

        void testNormalizedUpperGammaRealInteger();

        void testNormalizedUpperGammaRealReal();

        void testNormalizedUpperGammaRealComplex();

        void testNormalizedUpperGammaComplexInteger();

        void testNormalizedUpperGammaComplexReal();

        void testNormalizedUpperGammaComplexComplex();

        void testNormalizedUpperGammaVariants();

        void testInverseLowerGammaRealReal();

        void testBetaIntegerInteger();

        void testBetaIntegerReal();

        void testBetaIntegerComplex();

        void testBetaRealInteger();

        void testBetaRealReal();

        void testBetaRealComplex();

        void testBetaComplexInteger();

        void testBetaComplexReal();

        void testBetaComplexComplex();

        void testBetaVariants();

        void testErfInteger();

        void testErfReal();

        void testErfComplex();

        void testErfVariant();

        void testErfcInteger();

        void testErfcReal();

        void testErfcComplex();

        void testErfcVariant();

        void testErfInvReal();

        void testErfInvVariants();

        void testLambertWIntegerInteger();

        void testLambertWIntegerReal();

        void testLambertWIntegerComplex();

        void testLambertWRealInteger();

        void testLambertWRealReal();

        void testLambertWRealComplex();

        void testLambertWComplexInteger();

        void testLambertWComplexReal();

        void testLambertWComplexComplex();

        void testLambertWVariants();


    private:
        static const unsigned numberTestIterations;
        static const unsigned numberComplexGammaTestIterations;

        static const M::Real epsilon;
        static const M::Real infinity;
        static const M::Real NaN;
        static const M::Real negativeZero;
        static const M::Real positiveZero;

        static const M::Integer minInteger;
        static const M::Integer maxInteger;
};

#endif
