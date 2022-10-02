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
* This header defines support functions for the summation operator class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_SUMMATION_OPERATOR_H
#define M_SUMMATION_OPERATOR_H

#include "m_intrinsics.h"
#include "m_basic_functions.h"
#include "m_type_conversion_template.h"
#include "m_matrix_boolean.h"
#include "m_matrix_integer.h"
#include "m_matrix_real.h"
#include "m_matrix_complex.h"

namespace M {
    namespace Summation {
        /**
         * Value indicating the maximum number of allowed diverged values.
         */
        static constexpr unsigned maximumDivergentTerms = 3;

        /**
         * Value indicating the maximum number of converged terms before we believe we've converged.
         */
        static constexpr unsigned maximumConvergentTerms = 2;

        /**
         * Helper function that returns a one's value based on the type of a supplied parameter.
         *
         * \param[in] v The value to identify the type from.  The value is unused.
         */
        template<typename T> M_PUBLIC_TEMPLATE_FUNCTION T oneValue(const T& v) {
            (void) v;
            return T(1);
        }

        /**
         * Specialization of the oneValue template function for boolean matrices.
         *
         * \param[in] v The value to identify the type from.  The value is unused.
         */
        template<> M_PUBLIC_TEMPLATE_FUNCTION MatrixBoolean oneValue<MatrixBoolean>(const MatrixBoolean& v) {
            return MatrixBoolean::ones(v.numberRows(), v.numberColumns());
        }

        /**
         * Specialization of the oneValue template function for boolean matrices.
         *
         * \param[in] v The value to identify the type from.  The value is unused.
         */
        template<> M_PUBLIC_TEMPLATE_FUNCTION MatrixInteger oneValue<MatrixInteger>(const MatrixInteger& v) {
            return MatrixInteger::ones(v.numberRows(), v.numberColumns());
        }

        /**
         * Specialization of the oneValue template function for boolean matrices.
         *
         * \param[in] v The value to identify the type from.  The value is unused.
         */
        template<> M_PUBLIC_TEMPLATE_FUNCTION MatrixReal oneValue<MatrixReal>(const MatrixReal& v) {
            return MatrixReal::ones(v.numberRows(), v.numberColumns());
        }

        /**
         * Specialization of the oneValue template function for boolean matrices.
         *
         * \param[in] v The value to identify the type from.  The value is unused.
         */
        template<> M_PUBLIC_TEMPLATE_FUNCTION MatrixComplex oneValue<MatrixComplex>(const MatrixComplex& v) {
            return MatrixComplex::ones(v.numberRows(), v.numberColumns());
        }

        /**
         * Helper function that returns a zero value based on the type of a supplied parameter.
         *
         * \param[in] v The value to identify the type from.  The value is unused.
         */
        template<typename T> M_PUBLIC_TEMPLATE_FUNCTION T zeroValue(const T& v) {
            (void) v;
            return T(0);
        }

        /**
         * Specialization of the zeroValue template function for boolean matrices.
         *
         * \param[in] v The value to identify the type from.  The value is unused.
         */
        template<> M_PUBLIC_TEMPLATE_FUNCTION MatrixBoolean zeroValue<MatrixBoolean>(const MatrixBoolean& v) {
            return MatrixBoolean::zero(v.numberRows(), v.numberColumns());
        }

        /**
         * Specialization of the zeroValue template function for boolean matrices.
         *
         * \param[in] v The value to identify the type from.  The value is unused.
         */
        template<> M_PUBLIC_TEMPLATE_FUNCTION MatrixInteger zeroValue<MatrixInteger>(const MatrixInteger& v) {
            return MatrixInteger::zero(v.numberRows(), v.numberColumns());
        }

        /**
         * Specialization of the zeroValue template function for boolean matrices.
         *
         * \param[in] v The value to identify the type from.  The value is unused.
         */
        template<> M_PUBLIC_TEMPLATE_FUNCTION MatrixReal zeroValue<MatrixReal>(const MatrixReal& v) {
            return MatrixReal::zero(v.numberRows(), v.numberColumns());
        }

        /**
         * Specialization of the zeroValue template function for boolean matrices.
         *
         * \param[in] v The value to identify the type from.  The value is unused.
         */
        template<> M_PUBLIC_TEMPLATE_FUNCTION MatrixComplex zeroValue<MatrixComplex>(const MatrixComplex& v) {
            return MatrixComplex::zero(v.numberRows(), v.numberColumns());
        }

        /**
         * Helper function that calculates an error value from two parameters.
         *
         * \param[in] last    The last calculated value.
         *
         * \param[in] current The current calculated value.
         *
         * \return Returns a value holding the relative error as a singular value.
         */
        M_PUBLIC_API Real errorValue(Boolean last, Boolean current);

        /**
         * Helper function that calculates an error value from two parameters.
         *
         * \param[in] last    The last calculated value.
         *
         * \param[in] current The current calculated value.
         *
         * \return Returns a value holding the relative error as a singular value.
         */
        M_PUBLIC_API Real errorValue(Integer last, Integer current);

        /**
         * Helper function that calculates an error value from two parameters.
         *
         * \param[in] last    The last calculated value.
         *
         * \param[in] current The current calculated value.
         *
         * \return Returns a value holding the relative error as a singular value.
         */
        M_PUBLIC_API Real errorValue(Real last, Real current);

        /**
         * Helper function that calculates an error value from two parameters.
         *
         * \param[in] last    The last calculated value.
         *
         * \param[in] current The current calculated value.
         *
         * \return Returns a value holding the relative error as a singular value.
         */
        M_PUBLIC_API Real errorValue(const Complex& last, const Complex& current);

        /**
         * Helper function that calculates an error value from two parameters.
         *
         * \param[in] last    The last calculated value.
         *
         * \param[in] current The current calculated value.
         *
         * \return Returns a value holding the relative error as a singular value.
         */
        M_PUBLIC_API Real errorValue(const MatrixBoolean& last, const MatrixBoolean& current);

        /**
         * Helper function that calculates an error value from two parameters.
         *
         *     \param[in] last    The last calculated value.
         *
         * \param[in] current The current calculated value.
         *
         * \return Returns a value holding the relative error as a singular value.
         */
        M_PUBLIC_API Real errorValue(const MatrixInteger& last, const MatrixInteger& current);

        /**
         * Helper function that calculates an error value from two parameters.
         *
         *     \param[in] last    The last calculated value.
         *
         * \param[in] current The current calculated value.
         *
         * \return Returns a value holding the relative error as a singular value.
         */
        M_PUBLIC_API Real errorValue(const MatrixReal& last, const MatrixReal& current);

        /**
         * Helper function that calculates an error value from two parameters.
         *
         *     \param[in] last    The last calculated value.
         *
         * \param[in] current The current calculated value.
         *
         * \return Returns a value holding the relative error as a singular value.
         */
        M_PUBLIC_API Real errorValue(const MatrixComplex& last, const MatrixComplex& current);

        /**
         * Helper function that returns a non-convergence failure value based on the type of a supplied parameter.
         *
         * \param[in] v The value to identify the type from.  The value is unused.
         */
        template<typename T> M_PUBLIC_TEMPLATE_FUNCTION T convergenceFailureValue(const T& v) {
            (void) v;
            return T(0);
        }

        /**
         * Specialization of the convergenceFailureValue template function for boolean matrices.
         *
         * \param[in] v The value to identify the type from.  The value is unused.
         */
        template<> M_PUBLIC_TEMPLATE_FUNCTION Real convergenceFailureValue<Real>(const Real& v) {
            (void) v;
            return M::NaN;
        }

        /**
         * Specialization of the convergenceFailureValue template function for boolean matrices.
         *
         * \param[in] v The value to identify the type from.  The value is unused.
         */
        template<> M_PUBLIC_TEMPLATE_FUNCTION Complex convergenceFailureValue<Complex>(const Complex& v) {
            (void) v;
            return Complex(M::NaN, M::NaN);
        }

        /**
         * Specialization of the convergenceFailureValue template function for boolean matrices.
         *
         * \param[in] v The value to identify the type from.  The value is unused.
         */
        template<> M_PUBLIC_TEMPLATE_FUNCTION
        MatrixBoolean convergenceFailureValue<MatrixBoolean>(const MatrixBoolean& v) {
            (void) v;
            return MatrixBoolean(0, 0);
        }

        /**
         * Specialization of the convergenceFailureValue template function for boolean matrices.
         *
         * \param[in] v The value to identify the type from.  The value is unused.
         */
        template<> M_PUBLIC_TEMPLATE_FUNCTION
        MatrixInteger convergenceFailureValue<MatrixInteger>(const MatrixInteger& v) {
            (void) v;
            return MatrixInteger(0, 0);
        }

        /**
         * Specialization of the convergenceFailureValue template function for boolean matrices.
         *
         * \param[in] v The value to identify the type from.  The value is unused.
         */
        template<> M_PUBLIC_TEMPLATE_FUNCTION
        MatrixReal convergenceFailureValue<MatrixReal>(const MatrixReal& v) {
            (void) v;
            return MatrixReal(0, 0);
        }

        /**
         * Specialization of the convergenceFailureValue template function for boolean matrices.
         *
         * \param[in] v The value to identify the type from.  The value is unused.
         */
        template<> M_PUBLIC_TEMPLATE_FUNCTION
        MatrixComplex convergenceFailureValue<MatrixComplex>(const MatrixComplex& v) {
            (void) v;
            return MatrixComplex(0, 0);
        }

        /**
         * Function that determines if a value is infinite.  Provided to allow summations to work with non-basic
         * datatypes.
         *
         * \param[in] v The value to be tested.
         *
         * \return This version always returns false.
         */
        M_PUBLIC_API inline bool isNaN(Boolean v) {
            (void) v;
            return false;
        }

        /**
         * Function that determines if a value is infinite.  Provided to allow summations to work with non-basic
         * datatypes.
         *
         * \param[in] v The value to be tested.
         *
         * \return This version always returns false.
         */
        M_PUBLIC_API inline bool isNaN(Integer v) {
            (void) v;
            return false;
        }

        /**
         * Function that determines if a value is infinite.  Provided to allow summations to work with non-basic
         * datatypes.
         *
         * \param[in] v The value to be tested.
         *
         * \return This version returns true if the supplied value is infinite.  Returns false in all other cases.
         */
        M_PUBLIC_API inline bool isNaN(Real v) {
            return M::isNaN(v);
        }

        /**
         * Function that determines if a value is infinite.  Provided to allow summations to work with non-basic
         * datatypes.
         *
         * \param[in] v The value to be tested.
         *
         * \return This version returns true if the supplied value is infinite.  Returns false in all other cases.
         */
        M_PUBLIC_API inline bool isNaN(const Complex& v) {
            return M::isNaN(v.real()) && (v.imag() == 0 || M::isNaN(v.imag()));
        }

        /**
         * Function that determines if a value is infinite.  Provided to allow summations to work with non-basic
         * datatypes.
         *
         * \param[in] v The value to be tested.
         *
         * \return This version always returns false.
         */
        M_PUBLIC_API inline bool isNaN(const MatrixBoolean& v) {
            (void) v;
            return false;
        }

        /**
         * Function that determines if a value is infinite.  Provided to allow summations to work with non-basic
         * datatypes.
         *
         * \param[in] v The value to be tested.
         *
         * \return This version always returns false.
         */
        M_PUBLIC_API inline bool isNaN(const MatrixInteger& v) {
            (void) v;
            return false;
        }

        /**
         * Function that determines if a value is infinite.  Provided to allow summations to work with non-basic
         * datatypes.
         *
         * \param[in] v The value to be tested.
         *
         * \return This version always returns false.
         */
        M_PUBLIC_API inline bool isNaN(const MatrixReal& v) {
            (void) v;
            return false;
        }

        /**
         * Function that determines if a value is infinite.  Provided to allow summations to work with non-basic
         * datatypes.
         *
         * \param[in] v The value to be tested.
         *
         * \return This version always returns false.
         */
        M_PUBLIC_API inline bool isNaN(const MatrixComplex& v) {
            (void) v;
            return false;
        }

        /**
         * Function that determines if a value is less than zero.
         *
         * \param[in] v The value to be tested.
         *
         * \return This version always returns false.
         */
        M_PUBLIC_API inline bool isLessThanZero(Boolean v) {
            (void) v;
            return false;
        }

        /**
         * Function that determines if a value is less than zero.
         *
         * \param[in] v The value to be tested.
         *
         * \return This version always returns false.
         */
        M_PUBLIC_API inline bool isLessThanZero(Integer v) {
            return v < 0;
        }

        /**
         * Function that determines if a value is less than zero.
         *
         * \param[in] v The value to be tested.
         *
         * \return This version always returns false.
         */
        M_PUBLIC_API inline bool isLessThanZero(Real v) {
            return v < 0;
        }

        /**
         * Function that determines if a value is less than zero.
         *
         * \param[in] v The value to be tested.
         *
         * \return This version always returns false.
         */
        M_PUBLIC_API inline bool isLessThanZero(const Complex& v) {
            return v.real() < 0;
        }

        /**
         * Function that determines if a value is less than zero.
         *
         * \param[in] v The value to be tested.
         *
         * \return This version always returns false.
         */
        M_PUBLIC_API inline bool isLessThanZero(const MatrixBoolean& v) {
            (void) v;
            return false;
        }

        /**
         * Function that determines if a value is less than zero.
         *
         * \param[in] v The value to be tested.
         *
         * \return This version always returns false.
         */
        M_PUBLIC_API inline bool isLessThanZero(const MatrixInteger& v) {
            (void) v;
            return false;
        }

        /**
         * Function that determines if a value is less than zero.
         *
         * \param[in] v The value to be tested.
         *
         * \return This version always returns false.
         */
        M_PUBLIC_API inline bool isLessThanZero(const MatrixReal& v) {
            (void) v;
            return false;
        }

        /**
         * Function that determines if a value is less than zero.
         *
         * \param[in] v The value to be tested.
         *
         * \return This version always returns false.
         */
        M_PUBLIC_API inline bool isLessThanZero(const MatrixComplex& v) {
            (void) v;
            return false;
        }

        /**
         * Function that determines if one value is less than or equal to another.
         *
         * \param[in] a The first value to be tested.
         *
         * \param[in] b The second value to be tested.
         *
         * \return Returns true if a <= b, returns false otherwise.
         */
        M_PUBLIC_API inline bool isLessThanOrEqual(Boolean a, Boolean b) {
            return static_cast<Integer>(a) <= static_cast<Integer>(b);
        }

        /**
         * Function that determines if one value is less than or equal to another.
         *
         * \param[in] a The first value to be tested.
         *
         * \param[in] b The second value to be tested.
         *
         * \return Returns true if a <= b, returns false otherwise.
         */
        M_PUBLIC_API inline bool isLessThanOrEqual(Integer a, Integer b) {
            return a <= b;
        }

        /**
         * Function that determines if one value is less than or equal to another.
         *
         * \param[in] a The first value to be tested.
         *
         * \param[in] b The second value to be tested.
         *
         * \return Returns true if a <= b, returns false otherwise.
         */
        M_PUBLIC_API inline bool isLessThanOrEqual(Real a, Real b) {
            return a <= b;
        }

        /**
         * Function that determines if one value is less than or equal to another.
         *
         * \param[in] a The first value to be tested.
         *
         * \param[in] b The second value to be tested.
         *
         * \return Returns true if a <= b, returns false otherwise.
         */
        M_PUBLIC_API inline bool isLessThanOrEqual(const Complex& a, const Complex& b) {
            return a.real() <= b.real();
        }

        /**
         * Function that determines if one value is less than or equal to another.
         *
         * \param[in] a The first value to be tested.
         *
         * \param[in] b The second value to be tested.
         *
         * \return Returns true if a <= b, returns false otherwise.
         */
        M_PUBLIC_API inline bool isLessThanOrEqual(
                const MatrixBoolean& a,
                const MatrixBoolean& b
            ) {
            (void) a;
            (void) b;

            return false;
        }

        /**
         * Function that determines if one value is less than or equal to another.
         *
         * \param[in] a The first value to be tested.
         *
         * \param[in] b The second value to be tested.
         *
         * \return Returns true if a <= b, returns false otherwise.
         */
        M_PUBLIC_API inline bool isLessThanOrEqual(
                const MatrixInteger& a,
                const MatrixInteger& b
            ) {
            (void) a;
            (void) b;

            return false;
        }

        /**
         * Function that determines if one value is less than or equal to another.
         *
         * \param[in] a The first value to be tested.
         *
         * \param[in] b The second value to be tested.
         *
         * \return Returns true if a <= b, returns false otherwise.
         */
        M_PUBLIC_API inline bool isLessThanOrEqual(
                const MatrixReal& a,
                const MatrixReal& b
            ) {
            (void) a;
            (void) b;

            return false;
        }

        /**
         * Function that determines if one value is less than or equal to another.
         *
         * \param[in] a The first value to be tested.
         *
         * \param[in] b The second value to be tested.
         *
         * \return Returns true if a <= b, returns false otherwise.
         */
        M_PUBLIC_API inline bool isLessThanOrEqual(
                const MatrixComplex& a,
                const MatrixComplex& b
            ) {
            (void) a;
            (void) b;

            return false;
        }

        /**
         * Function that determines if a value is infinite.
         *
         * \param[in] v The value to be tested.
         *
         * \return This version always returns false.
         */
        M_PUBLIC_API inline bool isInfinite(Boolean v) {
            (void) v;
            return false;
        }

        /**
         * Function that determines if a value is infinite.
         *
         * \param[in] v The value to be tested.
         *
         * \return This version always returns false.
         */
        M_PUBLIC_API inline bool isInfinite(Integer v) {
            (void) v;
            return false;
        }

        /**
         * Function that determines if a value is infinite.
         *
         * \param[in] v The value to be tested.
         *
         * \return This version returns true if the supplied value is infinite.  Returns false otherwise.
         */
        M_PUBLIC_API bool isInfinite(Real v);

        /**
         * Function that determines if a value is infinite.
         *
         * \param[in] v The value to be tested.
         *
         * \return This version returns true if the supplied value is infinite.  Returns false otherwise.
         */
        M_PUBLIC_API bool isInfinite(const Complex& v);

        /**
         * Function that determines if a value is infinite.
         *
         * \param[in] v The value to be tested.
         *
         * \return This version always returns false.
         */
        M_PUBLIC_API inline bool isInfinite(const MatrixBoolean& v) {
            (void) v;
            return false;
        }

        /**
         * Function that determines if a value is infinite.
         *
         * \param[in] v The value to be tested.
         *
         * \return This version always returns false.
         */
        M_PUBLIC_API inline bool isInfinite(const MatrixInteger& v) {
            (void) v;
            return false;
        }

        /**
         * Function that determines if a value is infinite.
         *
         * \param[in] v The value to be tested.
         *
         * \return This version always returns false.
         */
        M_PUBLIC_API inline bool isInfinite(const MatrixReal& v) {
            (void) v;
            return false;
        }

        /**
         * Function that determines if a value is infinite.
         *
         * \param[in] v The value to be tested.
         *
         * \return This version always returns false.
         */
        M_PUBLIC_API inline bool isInfinite(const MatrixComplex& v) {
            (void) v;
            return false;
        }

        /**
         * Pure virtual template base class for our function definition.
         */
        template<typename Result, typename Index> class Function {
            public:
                /**
                 * The function result type.
                 */
                typedef Result ResultType;

                /**
                 * The function index type.
                 */
                typedef Index IndexType;

                /**
                 * The function functor.
                 *
                 * \param[in] m The index for the function.
                 *
                 * \return Returns the result of the summation function.
                 */
                virtual Result operator()(const Index& m) = 0;
        };

        /**
         * Pure virtual template base class for our Shank transform engine.
         */
        template<typename Result> class Base {
            public:
                /**
                 * Function that returns the computed value.
                 *
                 * \param[in] m The summation index.
                 *
                 * \return Returns the computed value.
                 */
                virtual Result value() = 0;

                /**
                 * Function that indicates if we had a divide by zero during our computation of the summation (but not
                 * the computation of the underlying summation operation).
                 *
                 * \return Returns true if we had a divide by zero.
                 */
                virtual bool hadDivideByZero() const = 0;
        };

        /**
         * Series functor class.
         */
        template<typename Result, typename Index> class BasicSummation:public Base<Result> {
            public:
                /**
                 * Constructor for the shank transform input class.
                 *
                 * \param[in] function      The function to process.
                 *
                 * \param[in] startingIndex The starting index for the series.  Value should always be positive.
                 *
                 * \param[in] indexSign     The sign for the starting index.  Value should be positive or negative 1.
                 */
                BasicSummation(
                        Function<Result, Index>& function,
                        const Index&             startingIndex,
                        const Index&             indexSign
                    ):fn(
                        function
                    ),m(
                        startingIndex
                    ),mSign(
                        indexSign
                    ) {
                    mOne       = oneValue(startingIndex);
                    currentSum = zeroValue(currentSum);
                }

                ~BasicSummation() {}

                /**
                 * Function that returns the current index value.
                 *
                 * \return Returns the current index value.
                 */
                inline const Index& index() const {
                    return m;
                }

                /**
                 * Function that sets the index for the summation input.
                 *
                 * \param[in] newIndex The new index value.
                 */
                void setIndex(const Index& newIndex) {
                    m = newIndex;
                }

                /**
                 * Function that returns the computed value.
                 *
                 * \return Returns the computed value.
                 */
                virtual Result value() final {
                    currentSum += fn(m * mSign);
                    m          += mOne;

                    return currentSum;
                }

                /**
                 * Function that indicates if we had a divide by zero during our computation of the summation (but not
                 * the computation of the underlying summation operation).
                 *
                 * \return Returns true if we had a divide by zero.  This version always returns false.
                 */
                bool hadDivideByZero() const final {
                    return false;
                }

            private:
                /**
                 * Function we're performing our summation over.
                 */
                Function<Result, Index>& fn;

                /**
                 * The current running summation index.
                 */
                Index m;

                /**
                 * The sign of the summation.
                 */
                Index mSign;

                /**
                 * Value to use as the increment.
                 */
                Index mOne;

                /**
                 * The current summation value.
                 */
                Result currentSum;
        };

        /**
         * Shank transform engine.  This template class performs a Shank transform on an input to improve convergence
         * of series.
         */
        template<typename Result, typename Index> class ShankTransform:public Base<Result> {
            public:
                /**
                 * Constructor for the shank transform input class.
                 *
                 * \param[in] input         The input summation engine.
                 *
                 * \param[in] startingIndex The starting index for the series.  Value should always be positive.
                 *
                 * \param[in] indexSign     The sign for the starting index.  Value should be positive or negative 1.
                 */
                ShankTransform(Base<Result>& input):currentInput(input) {
                     amm1 = zeroValue(am);
                     am   = amm1;
                     amp1 = currentInput.value();

                     divideByZero = false;
                }

                ~ShankTransform() {}

                /**
                 * Function that indicates if we had a divide by zero during our computation of the summation (but not
                 * the computation of the underlying summation operation).
                 *
                 * \return Returns true if we had a divide by zero.
                 */
                bool hadDivideByZero() const final {
                    return divideByZero;
                }

                /**
                 * Function that returns the computed value.
                 *
                 * \return Returns the computed value.
                 */
                Result value() final {
                    Result s = 0;

                    amm1 = am;
                    am   = amp1;
                    amp1 = currentInput.value();

                    if (currentInput.hadDivideByZero()) {
                        divideByZero = true;
                    } else {
                        Result n = amp1 - am;
                        Result d = (amp1 - am) - (am - amm1);

                        if (d == 0) {
                            divideByZero = true;
                        } else {
                            s = amp1 - (n * n) / d;
                        }
                    }

                    return s;
                }

            private:
                /**
                 * The input function.
                 */
                Base<Result>& currentInput;

                /**
                 * Intermediate value at A_{m-1}
                 */
                Result amm1;

                /**
                 * Intermediate value at A_m
                 */
                Result am;

                /**
                 * Intermediate value at A_{m+1}
                 */
                Result amp1;

                /**
                 * Flag indicating that we've had a divide by zero so we can't use this Shank transform.
                 */
                bool divideByZero;
        };

        /**
         * Template function that solves a finite series.
         *
         * \param[in] function      The function to calculate the series from.
         *
         * \param[in] startingIndex The starting value for the summation.
         *
         * \param[in] endingIndex   The value to end the summation with.
         *
         * \return Returns the result.
         */
        template<typename Result, typename Index> Result finiteSummation(
                Function<Result, Index>& function,
                const Index&             startingIndex,
                const Index&             endingIndex
            ) {
            BasicSummation<Result, Index> b(function, startingIndex, oneValue(startingIndex));
            Result                        r;
            while (b.index() <= endingIndex) {
                r = b.value();
            }

            return r;
        }

        /**
         * Template function that solves a one-sided infinite series using a Shank transform.
         *
         * \param[in] function         The function to calculate the series from.
         *
         * \param[in] startingIndex    The starting value for the summation.
         *
         * \param[in] indexSign        The sign for the index.  Value should be positive or negative 1.
         *
         * \param[in] convergenceLimit The limit required for convergence.
         *
         * \return Returns the result.  This function will throw an exception or return NaN if the supplied function
         *         does not converge.
         */
        template<typename Result, typename Index> Result oneSidedInfiniteSummation(
                Function<Result, Index>& function,
                const Index&             startingIndex,
                const Index&             indexSign,
                M::Real                  convergenceLimit = M::epsilon
            ) {
            BasicSummation<Result, Index> b(function, startingIndex, indexSign);
            ShankTransform<Result, Index> s1(b);
            ShankTransform<Result, Index> s2(s1);
            ShankTransform<Result, Index> s3(s2);
            Base<Result>*                 entries[4] = { &b, &s1, &s2, &s3 };

            Result                        lastResult       = s2.value();
            Result                        result           = s2.value();
            M::Real                       sumError         = errorValue(lastResult, result);
            unsigned long                 numberIterations = 1; // We consider above one iterations.
            unsigned                      numberConverged  = 0;
            unsigned                      numberDiverged   = 0;

            unsigned                      useValueForConvergence = 3;
            do {
                while (entries[useValueForConvergence]->hadDivideByZero()) {
                    numberConverged = 0;
                    numberDiverged  = 0;
                    --useValueForConvergence;
                }

                lastResult = result;
                result     = entries[useValueForConvergence]->value();

                M::Real error        = errorValue(lastResult, result);
                M::Real averageError = sumError / numberIterations;

                sumError += error;
                ++numberIterations;

                if (error >= averageError) {
                    ++numberDiverged;
                } else {
                    numberDiverged = 0;
                }

                if (error <= convergenceLimit) {
                    ++numberConverged;
                } else {
                    numberConverged = 0;
                }
            } while (numberDiverged < maximumDivergentTerms && numberConverged < maximumConvergentTerms);

            if (numberDiverged > maximumDivergentTerms) {
                M::internalTriggerCanNotConvergeError();
                // trigger convergence error here.
                result = convergenceFailureValue(result);
            }

            return result;
        }
    };

    /**
     * Template function that solves an arbitrary summation.
     *
     * \param[in] function         The function to calculate the series from.
     *
     * \param[in] startingIndex    The starting index for the summation.  Value can be infinite and negative.
     *
     * \param[in] endingIndex      The ending index for the summation.  Value can be infinite and negative and less
     *                             than the lower bound.
     *
     * \param[in] convergenceLimit The limit required for convergence.
     *
     * \return Returns the result.  This function will throw an exception or return NaN if the supplied function
     *         does not converge.
     */
    template<
            typename Function,
            typename StartingIndex,
            typename EndingIndex,
            typename Result = typename Function::ResultType
        > Result summation(
            Function&     function,
            StartingIndex startingIndex,
            EndingIndex   endingIndex,
            M::Real       convergenceLimit = M::epsilon
        ) {
        typedef typename Function::IndexType Index;
        Result r;

        Index  s         = inlineTo<Index>(startingIndex);
        Index  e         = inlineTo<Index>(endingIndex);
        auto   oneValue  = Summation::oneValue(s);
        auto   zeroValue = Summation::zeroValue(s);

        if (Summation::isLessThanOrEqual(e, s)) {
            auto t = s;
            s = e;
            e = t;
        }

        if (Summation::isInfinite(s)) {
            if (Summation::isLessThanZero(s)) {
                if (Summation::isInfinite(e)) {
                    if (Summation::isLessThanZero(e)) {
                        M::internalTriggerInvalidParameterValueError();
                    } else {
                        r = (
                              Summation::oneSidedInfiniteSummation(function, oneValue, -oneValue, convergenceLimit)
                            + Summation::oneSidedInfiniteSummation(function, zeroValue, oneValue, convergenceLimit)
                        );
                    }
                } else if (Summation::isLessThanZero(e)) {
                    r = Summation::oneSidedInfiniteSummation(function, -e, -oneValue, convergenceLimit);
                } else /* if (!Summation::isInfinite(e) && !Summation::isLessThanZero(e)) */ {
                    r = (
                          Summation::oneSidedInfiniteSummation(function, oneValue, -oneValue, convergenceLimit)
                        + Summation::finiteSummation(function, zeroValue, e)
                    );
                }
            } else {
                M::internalTriggerInvalidParameterValueError();
            }
        } else if (Summation::isInfinite(e)) {
            if (Summation::isLessThanZero(e)) {
                M::internalTriggerInvalidParameterValueError();
            } else if (Summation::isLessThanZero(s)) {
                r = (
                      Summation::finiteSummation(function, s, -oneValue)
                    + Summation::oneSidedInfiniteSummation(function, zeroValue, oneValue, convergenceLimit)
                );
            } else /* if (!Summation::isInfinite(s) || !Summation::isLessThanZero(s)) */ {
                r = Summation::oneSidedInfiniteSummation(function, s, oneValue, convergenceLimit);
            }
        } else {
            r = Summation::finiteSummation(function, s, e);
        }

        return r;
    }
};

#endif
