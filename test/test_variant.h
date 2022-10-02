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
* This header provides tests for the \ref Model::Variant class.  By extension, this class also tests the underlying
* \ref M::Variant class.
***********************************************************************************************************************/

#ifndef TEST_VARIANT_H
#define TEST_VARIANT_H

#include <QObject>
#include <QtTest/QtTest>

class TestVariant:public QObject {
    Q_OBJECT

    public:
        TestVariant();

        ~TestVariant() override;

    private slots:
        void testNoneToNone();

        void testNoneToBoolean();

        void testNoneToInteger();

        void testNoneToReal();

        void testNoneToComplex();

        void testNoneToSet();

        void testNoneToTuple();

        void testNoneToMatrixBoolean();

        void testNoneToMatrixInteger();

        void testNoneToMatrixReal();

        void testNoneToMatrixComplex();

        // additional testNoneTo...

        void testBooleanToNone();

        void testBooleanToBoolean();

        void testBooleanToInteger();

        void testBooleanToReal();

        void testBooleanToComplex();

        void testBooleanToSet();

        void testBooleanToTuple();

        void testBooleanToMatrixBoolean();

        void testBooleanToMatrixInteger();

        void testBooleanToMatrixReal();

        void testBooleanToMatrixComplex();

        // additional testBooleanTo...

        void testIntegerToNone();

        void testIntegerToBoolean();

        void testIntegerToInteger();

        void testIntegerToReal();

        void testIntegerToComplex();

        void testIntegerToSet();

        void testIntegerToTuple();

        void testIntegerToMatrixBoolean();

        void testIntegerToMatrixInteger();

        void testIntegerToMatrixReal();

        void testIntegerToMatrixComplex();

        // additional testIntegerTo...

        void testRealToNone();

        void testRealToBoolean();

        void testRealToInteger();

        void testRealToReal();

        void testRealToComplex();

        void testRealToSet();

        void testRealToTuple();

        void testRealToMatrixBoolean();

        void testRealToMatrixInteger();

        void testRealToMatrixReal();

        void testRealToMatrixComplex();

        // additional testRealTo...

        void testComplexToNone();

        void testComplexToBoolean();

        void testComplexToInteger();

        void testComplexToReal();

        void testComplexToComplex();

        void testComplexToSet();

        void testComplexToTuple();

        void testComplexToMatrixBoolean();

        void testComplexToMatrixInteger();

        void testComplexToMatrixReal();

        void testComplexToMatrixComplex();

        // additional testComplexTo...

        void testSetToNone();

        void testSetToBoolean();

        void testSetToInteger();

        void testSetToReal();

        void testSetToComplex();

        void testSetToSet();

        void testSetToTuple();

        void testSetToMatrixBoolean();

        void testSetToMatrixInteger();

        void testSetToMatrixReal();

        void testSetToMatrixComplex();

        // additional testSetTo...

        void testTupleToNone();

        void testTupleToBoolean();

        void testTupleToInteger();

        void testTupleToReal();

        void testTupleToComplex();

        void testTupleToSet();

        void testTupleToTuple();

        void testTupleToMatrixBoolean();

        void testTupleToMatrixInteger();

        void testTupleToMatrixReal();

        void testTupleToMatrixComplex();

        // additional testTupleTo...

        void testMatrixBooleanToNone();

        void testMatrixBooleanToBoolean();

        void testMatrixBooleanToInteger();

        void testMatrixBooleanToReal();

        void testMatrixBooleanToComplex();

        void testMatrixBooleanToSet();

        void testMatrixBooleanToTuple();

        void testMatrixBooleanToMatrixBoolean();

        void testMatrixBooleanToMatrixInteger();

        void testMatrixBooleanToMatrixReal();

        void testMatrixBooleanToMatrixComplex();

        // additional testMatrixBooleanTo...

        void testMatrixIntegerToNone();

        void testMatrixIntegerToBoolean();

        void testMatrixIntegerToInteger();

        void testMatrixIntegerToReal();

        void testMatrixIntegerToComplex();

        void testMatrixIntegerToSet();

        void testMatrixIntegerToTuple();

        void testMatrixIntegerToMatrixBoolean();

        void testMatrixIntegerToMatrixInteger();

        void testMatrixIntegerToMatrixReal();

        void testMatrixIntegerToMatrixComplex();

        // additional testMatrixIntegerTo...

        void testMatrixRealToNone();

        void testMatrixRealToBoolean();

        void testMatrixRealToInteger();

        void testMatrixRealToReal();

        void testMatrixRealToComplex();

        void testMatrixRealToSet();

        void testMatrixRealToTuple();

        void testMatrixRealToMatrixBoolean();

        void testMatrixRealToMatrixInteger();

        void testMatrixRealToMatrixReal();

        void testMatrixRealToMatrixComplex();

        // additional testMatrixRealTo...

        void testMatrixComplexToNone();

        void testMatrixComplexToBoolean();

        void testMatrixComplexToInteger();

        void testMatrixComplexToReal();

        void testMatrixComplexToComplex();

        void testMatrixComplexToSet();

        void testMatrixComplexToTuple();

        void testMatrixComplexToMatrixBoolean();

        void testMatrixComplexToMatrixInteger();

        void testMatrixComplexToMatrixReal();

        void testMatrixComplexToMatrixComplex();

        // additional testMatrixComplexTo...

        void testComparisonTypesAgainstNone();

        void testComparisonTypesAgainstBoolean();

        void testComparisonTypesAgainstInteger();

        void testComparisonTypesAgainstReal();

        void testComparisonTypesAgainstComplex();

        void testConvertFunctions();
};

#endif
