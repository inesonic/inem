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
* This header provides tests for the \ref Model::MatrixReal class.
***********************************************************************************************************************/

#ifndef TEST_MATRIX_REAL_H
#define TEST_MATRIX_REAL_H

#include <QObject>
#include <QtTest/QtTest>

#include "test_matrix_base.h"

namespace Model {
    class MatrixReal;
}

class TestMatrixReal:public TestMatrixBase {
    Q_OBJECT

    public:
        TestMatrixReal();

        ~TestMatrixReal() override;

    private slots:
        void testConstructorAndDestructors();

        void testCoefficientValueType();

        void testNumberRowsColumns();

        void testValueMethods();

        void testInternalAtMethods();

        void testResizeMethod();

        void testCoefficientAccessorMethods();

        void testSliceMethods();

        void testCombineMethods();

        void testReverseMethods();

        void testIdentity();

        void testZero();

        void testOnes();

        void testDiagnonalEntries();

        void testDiagnonal();

        void testMatrixTypeMethods();

        void testDeterminant1();

        void testDeterminant2();

        void testTranspose();

        void testComplexConjugate();

        void testAdjoint();

        void testInverse();

        void testPLU();

        void testSVD();

        void testQR();

        void testLQ();

        void testCholesky();

        void testUpperCholesky();

        void testBunchKaufman();

        void testUpperBunchKaufman();

        void testHessenberg();

        void testDCT();

        void testHilbert();

        void testConditionNumber();

        void testNormMethods();

        void testRank();

        void testEquilibrate();

        void testSolve();

        void testLeastSquares();

        void testHadamardProduct();

        void testKroneckerProduct();

        void testRawDataAccess();

        void testFloorFunctions();

        void testCeilingFunctions();

        void testNearbyIntFunctions();

        void testTruncateToIntegerFunction();

        void testAssignmentOperators();

        void testAddAssignmentOperators();

        void testSubtractAssignmentOperators();

        void testMatrixMatrixMultiplyAssignmentOperator();

        void testMatrixScalarMultiplyAssignmentOperators();

        void testDivideAssignmentOperators();

        void testAdditionOperators();

        void testSubtractionOperators();

        void testMatrixMatrixMultiplicationOperator();

        void testMatrixScalarMultiplicationOperators();

        void testDivisionOperators();

        void testUnaryPlusMinusOperators();

        void testComparisonOperators();

        void testSubscriptingOperators();

        void testMultiplicationFunctions();

        void testIterator();

    private:
        static void dumpMatrix(const Model::MatrixReal& matrix);

        static const unsigned numberIterations = 10;
};

#endif
