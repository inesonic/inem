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
* This file is the main entry point for the inem unit tests.
***********************************************************************************************************************/

#include <QtTest/QtTest>

#include "m_api.h"

#include "test_reference_counter.h"
#include "test_boolean.h"
#include "test_complex.h"
#include "test_complex_operators.h"
#include "test_variant.h"
#include "test_variant_operators.h"
#include "test_variant_functions.h"
#include "test_list.h"
#include "test_unordered_set.h"
#include "test_ordered_set.h"
#include "test_model_range.h"
#include "test_m_range.h"
#include "test_model_tuple.h"
#include "test_m_tuple.h"
#include "test_set.h"
#include "test_matrix_boolean.h"
#include "test_matrix_integer.h"
#include "test_matrix_real.h"
#include "test_matrix_complex.h"
#include "test_implicit_ordering.h"
#include "test_matrix_random_functions.h"
#include "test_basic_functions.h"
#include "test_trigonometric_functions.h"
#include "test_hyperbolic_functions.h"
#include "test_set_functions.h"
#include "test_boolean_functions.h"
#include "test_statistical_functions.h"
#include "test_file_functions.h"
#include "test_summation_operator.h"
#include "test_per_thread.h"
#include "test_identifier_database.h"
#include "test_model_api.h"
#include "test_regressions.h"

#define TEST(_X) {                                                  \
    _X _x;                                                          \
    testStatus |= QTest::qExec(&_x, argumentCount, argumentValues); \
}

int main(int argumentCount, char** argumentValues) {
    int testStatus = 0;

    M::Api::setMatrixApi(&matApi);

//    TEST(TestReferenceCounter)
//    TEST(TestRegressions)
    TEST(TestSummationOperator)
//    TEST(TestVariantOperators)
//    TEST(TestBooleanFunctions)
//    TEST(TestSetFunctions)
//    TEST(TestVariantFunctions)
//    TEST(TestBoolean)
//    TEST(TestComplex)
//    TEST(TestComplexOperators)
//    TEST(TestVariant)
//    TEST(TestImplicitOrdering)
//    TEST(TestList)
//    TEST(TestUnorderedSet)
//    TEST(TestOrderedSet)
//    TEST(TestModelRange)
//    TEST(TestSet)
//    TEST(TestMTuple)
//    TEST(TestMRange)
//    TEST(TestModelTuple)
//    TEST(TestPerThread)
//    TEST(TestBasicFunctions)
//    TEST(TestTrigonometricFunctions)
//    TEST(TestHyperbolicFunctions)
//    TEST(TestStatisticalFunctions)
//    TEST(TestFileFunctions)
//    TEST(TestMatrixBoolean)
//    TEST(TestMatrixInteger)
//    TEST(TestMatrixReal)
//    TEST(TestMatrixComplex)
//    TEST(TestMatrixRandomFunctions)
//    TEST(TestSummationOperator)
//    TEST(TestIdentifierDatabase)
//    TEST(TestModelApi)

    return testStatus;
}
