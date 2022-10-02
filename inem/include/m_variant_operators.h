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
* This header defines operators you can use with the \ref M::Variant class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_VARIANT_OPERATORS_H
#define M_VARIANT_OPERATORS_H

#include "m_common.h"
#include "m_api_types.h"
#include "m_matrix_complex.h"
#include "m_variant.h"
#include "m_intrinsic_types.h"

namespace M {
    class Set;
    class Tuple;
    class MatrixBoolean;
    class MatrixInteger;
    class MatrixReal;
}

namespace M {
    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a == b.
     */
    M_PUBLIC_API bool operator==(const Variant& a, Boolean b);

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a == b.
     */
    M_PUBLIC_API bool operator==(const Variant& a, Integer b);

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a == b.
     */
    M_PUBLIC_API bool operator==(const Variant& a, Real b);

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a == b.
     */
    M_PUBLIC_API bool operator==(const Variant& a, const Complex& b);

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a == b.
     */
    M_PUBLIC_API bool operator==(const Variant& a, const Set& b);

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a == b.
     */
    M_PUBLIC_API bool operator==(const Variant& a, const Tuple& b);

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a == b.
     */
    M_PUBLIC_API bool operator==(const Variant& a, const MatrixBoolean& b);

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a == b.
     */
    M_PUBLIC_API bool operator==(const Variant& a, const MatrixInteger& b);

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a == b.
     */
    M_PUBLIC_API bool operator==(const Variant& a, const MatrixReal& b);

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a == b.
     */
    M_PUBLIC_API bool operator==(const Variant& a, const MatrixComplex& b);

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a < b.
     */
    M_PUBLIC_API bool operator<(const Variant& a, Boolean b);

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a < b.
     */
    M_PUBLIC_API bool operator<(const Variant& a, Integer b);

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a < b.
     */
    M_PUBLIC_API bool operator<(const Variant& a, Real b);

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a < b.
     */
    M_PUBLIC_API bool operator<(const Variant& a, const Complex& b);

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a > b.
     */
    M_PUBLIC_API bool operator>(const Variant& a, Boolean b);

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a > b.
     */
    M_PUBLIC_API bool operator>(const Variant& a, Integer b);

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a > b.
     */
    M_PUBLIC_API bool operator>(const Variant& a, Real b);

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a > b.
     */
    M_PUBLIC_API bool operator>(const Variant& a, const Complex& b);

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a == b.
     */
    M_PUBLIC_API inline bool operator==(Boolean a, const Variant& b) {
        return operator==(b, a);
    }

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a == b.
     */
    M_PUBLIC_API inline bool operator==(Integer a, const Variant& b) {
        return operator==(b, a);
    }

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a == b.
     */
    M_PUBLIC_API inline bool operator==(Real a, const Variant& b) {
        return operator==(b, a);
    }

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a == b.
     */
    M_PUBLIC_API inline bool operator==(const Complex& a, const Variant& b) {
        return operator==(b, a);
    }

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a == b.
     */
    M_PUBLIC_API inline bool operator==(const Set& a, const Variant& b) {
        return operator==(b, a);
    }

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a == b.
     */
    M_PUBLIC_API inline bool operator==(const Tuple& a, const Variant& b) {
        return operator==(b, a);
    }

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a == b.
     */
    M_PUBLIC_API inline bool operator==(const MatrixBoolean& a, const Variant& b) {
        return operator==(b, a);
    }

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a == b.
     */
    M_PUBLIC_API inline bool operator==(const MatrixInteger& a, const Variant& b) {
        return operator==(b, a);
    }

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a == b.
     */
    M_PUBLIC_API inline bool operator==(const MatrixReal& a, const Variant& b) {
        return operator==(b, a);
    }

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a == b.
     */
    M_PUBLIC_API inline bool operator==(const MatrixComplex& a, const Variant& b) {
        return operator==(b, a);
    }

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a < b.
     */
    M_PUBLIC_API inline bool operator<(Boolean a, const Variant& b) {
        return operator>(b, a);
    }

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a < b.
     */
    M_PUBLIC_API inline bool operator<(Integer a, const Variant& b) {
        return operator>(b, a);
    }

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a < b.
     */
    M_PUBLIC_API inline bool operator<(Real a, const Variant& b) {
        return operator >(b, a);
    }

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a < b.
     */
    M_PUBLIC_API inline bool operator<(const Complex& a, const Variant& b) {
        return operator>(b, a);
    }

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a > b.
     */
    M_PUBLIC_API inline bool operator>(Boolean a, const Variant& b) {
        return operator<(b, a);
    }

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a > b.
     */
    M_PUBLIC_API inline bool operator>(const Integer a, const Variant& b) {
        return operator<(b, a);
    }

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a > b.
     */
    M_PUBLIC_API inline bool operator>(Real a, const Variant& b) {
        return operator<(b, a);
    }

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a > b.
     */
    M_PUBLIC_API inline bool operator>(const Complex& a, const Variant& b) {
        return operator<(b, a);
    }

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a != b.
     */
    template<typename T> M_PUBLIC_TEMPLATE_FUNCTION bool operator!=(const Variant& a, T b) {
        return !operator==(a, b);
    }

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a != b.
     */
    template<typename T> M_PUBLIC_TEMPLATE_FUNCTION bool operator!=(T a, const Variant& b) {
        return !operator==(b, a);
    }

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a != b.
     */
    template<typename T> M_PUBLIC_TEMPLATE_FUNCTION bool operator<=(const Variant& a, T b) {
        return !operator>(a, b);
    }

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a != b.
     */
    template<typename T> M_PUBLIC_TEMPLATE_FUNCTION bool operator<=(T a, const Variant& b) {
        return !operator>(a, b);
    }

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a != b.
     */
    template<typename T> M_PUBLIC_TEMPLATE_FUNCTION bool operator>=(const Variant& a, T b) {
        return !operator<(a, b);
    }

    /**
     * Variant comparison operator.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if a != b.
     */
    template<typename T> M_PUBLIC_TEMPLATE_FUNCTION bool operator>=(T a, const Variant& b) {
        return !operator<(a, b);
    }

    /**
     * Variant addition operator.
     *
     * \param[in] a The augend.
     *
     * \param[in] b The addend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API Variant operator+(const Variant& a, Boolean b);

    /**
     * Variant addition operator.
     *
     * \param[in] a The augend.
     *
     * \param[in] b The addend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API Variant operator+(const Variant& a, Integer b);

    /**
     * Variant addition operator.
     *
     * \param[in] a The augend.
     *
     * \param[in] b The addend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API Variant operator+(const Variant& a, Real b);

    /**
     * Variant addition operator.
     *
     * \param[in] a The augend.
     *
     * \param[in] b The addend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API Complex operator+(const Variant& a, const Complex& b);

    /**
     * Variant addition operator.
     *
     * \param[in] a The augend.
     *
     * \param[in] b The addend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API Variant operator+(const Variant& a, const MatrixBoolean& b);

    /**
     * Variant addition operator.
     *
     * \param[in] a The augend.
     *
     * \param[in] b The addend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API Variant operator+(const Variant& a, const MatrixInteger& b);

    /**
     * Variant addition operator.
     *
     * \param[in] a The augend.
     *
     * \param[in] b The addend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API Variant operator+(const Variant& a, const MatrixReal& b);

    /**
     * Variant addition operator.
     *
     * \param[in] a The augend.
     *
     * \param[in] b The addend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API MatrixComplex operator+(const Variant& a, const MatrixComplex& b);

    /**
     * Variant addition operator.
     *
     * \param[in] a The augend.
     *
     * \param[in] b The addend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API inline Variant operator+(Boolean a, const Variant& b) {
        return operator+(b, a);
    }

    /**
     * Variant addition operator.
     *
     * \param[in] a The augend.
     *
     * \param[in] b The addend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API inline Variant operator+(Integer a, const Variant& b) {
        return operator+(b, a);
    }

    /**
     * Variant addition operator.
     *
     * \param[in] a The augend.
     *
     * \param[in] b The addend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API inline Variant operator+(Real a, const Variant& b) {
        return operator+(b, a);
    }

    /**
     * Variant addition operator.
     *
     * \param[in] a The augend.
     *
     * \param[in] b The addend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API inline Complex operator+(const Complex& a, const Variant& b) {
        return operator+(b, a);
    }

    /**
     * Variant addition operator.
     *
     * \param[in] a The augend.
     *
     * \param[in] b The addend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API inline Variant operator+(const MatrixBoolean& a, const Variant& b) {
        return operator+(b, a);
    }

    /**
     * Variant addition operator.
     *
     * \param[in] a The augend.
     *
     * \param[in] b The addend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API inline Variant operator+(const MatrixInteger& a, const Variant& b) {
        return operator+(b, a);
    }

    /**
     * Variant addition operator.
     *
     * \param[in] a The augend.
     *
     * \param[in] b The addend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API inline Variant operator+(const MatrixReal& a, const Variant& b) {
        return operator+(b, a);
    }

    /**
     * Variant addition operator.
     *
     * \param[in] a The augend.
     *
     * \param[in] b The addend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API inline MatrixComplex operator+(const MatrixComplex& a, const Variant& b) {
        return operator+(b, a);
    }

    /**
     * Variant addition operator.
     *
     * \param[in] a The augend.
     *
     * \param[in] b The addend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API Variant operator+(const Variant& a, const Variant& b);

    /**
     * Variant subtraction operator.
     *
     * \param[in] a The minuend.
     *
     * \param[in] b The subtrahend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API Variant operator-(const Variant& a, Boolean b);

    /**
     * Variant subtraction operator.
     *
     * \param[in] a The minuend.
     *
     * \param[in] b The subtrahend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API Variant operator-(const Variant& a, Integer b);

    /**
     * Variant subtraction operator.
     *
     * \param[in] a The minuend.
     *
     * \param[in] b The subtrahend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API Variant operator-(const Variant& a, Real b);

    /**
     * Variant subtraction operator.
     *
     * \param[in] a The minuend.
     *
     * \param[in] b The subtrahend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API Complex operator-(const Variant& a, const Complex& b);

    /**
     * Variant subtraction operator.
     *
     * \param[in] a The minuend.
     *
     * \param[in] b The subtrahend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API Variant operator-(const Variant& a, const MatrixBoolean& b);

    /**
     * Variant subtraction operator.
     *
     * \param[in] a The minuend.
     *
     * \param[in] b The subtrahend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API Variant operator-(const Variant& a, const MatrixInteger& b);

    /**
     * Variant subtraction operator.
     *
     * \param[in] a The minuend.
     *
     * \param[in] b The subtrahend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API Variant operator-(const Variant& a, const MatrixReal& b);

    /**
     * Variant subtraction operator.
     *
     * \param[in] a The minuend.
     *
     * \param[in] b The subtrahend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API MatrixComplex operator-(const Variant& a, const MatrixComplex& b);

    /**
     * Variant subtraction operator.
     *
     * \param[in] a The minuend.
     *
     * \param[in] b The subtrahend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API Variant operator-(Boolean a, const Variant& b);

    /**
     * Variant subtraction operator.
     *
     * \param[in] a The minuend.
     *
     * \param[in] b The subtrahend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API Variant operator-(Integer a, const Variant& b);

    /**
     * Variant subtraction operator.
     *
     * \param[in] a The minuend.
     *
     * \param[in] b The subtrahend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API Variant operator-(Real a, const Variant& b);

    /**
     * Variant subtraction operator.
     *
     * \param[in] a The minuend.
     *
     * \param[in] b The subtrahend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API Complex operator-(const Complex& a, const Variant& b);

    /**
     * Variant subtraction operator.
     *
     * \param[in] a The minuend.
     *
     * \param[in] b The subtrahend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API Variant operator-(const MatrixBoolean& a, const Variant& b);

    /**
     * Variant subtraction operator.
     *
     * \param[in] a The minuend.
     *
     * \param[in] b The subtrahend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API Variant operator-(const MatrixInteger& a, const Variant& b);

    /**
     * Variant subtraction operator.
     *
     * \param[in] a The minuend.
     *
     * \param[in] b The subtrahend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API Variant operator-(const MatrixReal& a, const Variant& b);

    /**
     * Variant subtraction operator.
     *
     * \param[in] a The minuend.
     *
     * \param[in] b The subtrahend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API MatrixComplex operator-(const MatrixComplex& a, const Variant& b);

    /**
     * Variant subtraction operator.
     *
     * \param[in] a The minuend.
     *
     * \param[in] b The subtrahend.
     *
     * \return Returns the sum.
     */
    M_PUBLIC_API Variant operator-(const Variant& a, const Variant& b);

    /**
     * Variant multiplication operator.
     *
     * \param[in] a The multiplier.
     *
     * \param[in] b The multiplicand.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator*(const Variant& a, Boolean b);

    /**
     * Variant multiplication operator.
     *
     * \param[in] a The multiplier.
     *
     * \param[in] b The multiplicand.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator*(const Variant& a, Integer b);

    /**
     * Variant multiplication operator.
     *
     * \param[in] a The multiplier.
     *
     * \param[in] b The multiplicand.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator*(const Variant& a, Real b);

    /**
     * Variant multiplication operator.
     *
     * \param[in] a The multiplier.
     *
     * \param[in] b The multiplicand.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator*(const Variant& a, const Complex& b);

    /**
     * Variant multiplication operator.
     *
     * \param[in] a The multiplier.
     *
     * \param[in] b The multiplicand.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Tuple operator*(const Variant& a, const Tuple& b);

    /**
     * Variant multiplication operator.
     *
     * \param[in] a The multiplier.
     *
     * \param[in] b The multiplicand.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator*(const Variant& a, const MatrixBoolean& b);

    /**
     * Variant multiplication operator.
     *
     * \param[in] a The multiplier.
     *
     * \param[in] b The multiplicand.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator*(const Variant& a, const MatrixInteger& b);

    /**
     * Variant multiplication operator.
     *
     * \param[in] a The multiplier.
     *
     * \param[in] b The multiplicand.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator*(const Variant& a, const MatrixReal& b);

    /**
     * Variant multiplication operator.
     *
     * \param[in] a The multiplier.
     *
     * \param[in] b The multiplicand.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API MatrixComplex operator*(const Variant& a, const MatrixComplex& b);

    /**
     * Variant multiplication operator.
     *
     * \param[in] a The multiplier.
     *
     * \param[in] b The multiplicand.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator*(Boolean a, const Variant& b);

    /**
     * Variant multiplication operator.
     *
     * \param[in] a The multiplier.
     *
     * \param[in] b The multiplicand.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator*(Integer a, const Variant& b);

    /**
     * Variant multiplication operator.
     *
     * \param[in] a The multiplier.
     *
     * \param[in] b The multiplicand.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator*(Real a, const Variant& b);

    /**
     * Variant multiplication operator.
     *
     * \param[in] a The multiplier.
     *
     * \param[in] b The multiplicand.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator*(const Complex& a, const Variant& b);

    /**
     * Variant multiplication operator.
     *
     * \param[in] a The multiplier.
     *
     * \param[in] b The multiplicand.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Tuple operator*(const Tuple& a, const Variant& b);

    /**
     * Variant multiplication operator.
     *
     * \param[in] a The multiplier.
     *
     * \param[in] b The multiplicand.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator*(const MatrixBoolean& a, const Variant& b);

    /**
     * Variant multiplication operator.
     *
     * \param[in] a The multiplier.
     *
     * \param[in] b The multiplicand.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator*(const MatrixInteger& a, const Variant& b);

    /**
     * Variant multiplication operator.
     *
     * \param[in] a The multiplier.
     *
     * \param[in] b The multiplicand.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator*(const MatrixReal& a, const Variant& b);

    /**
     * Variant multiplication operator.
     *
     * \param[in] a The multiplier.
     *
     * \param[in] b The multiplicand.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API MatrixComplex operator*(const MatrixComplex& a, const Variant& b);

    /**
     * Variant multiplication operator.
     *
     * \param[in] a The multiplier.
     *
     * \param[in] b The multiplicand.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator*(const Variant& a, const Variant& b);

    /**
     * Variant division operator.
     *
     * \param[in] a The dividend.
     *
     * \param[in] b The divisor.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator/(const Variant& a, Boolean b);

    /**
     * Variant division operator.
     *
     * \param[in] a The dividend.
     *
     * \param[in] b The divisor.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator/(const Variant& a, Integer b);

    /**
     * Variant division operator.
     *
     * \param[in] a The dividend.
     *
     * \param[in] b The divisor.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator/(const Variant& a, Real b);

    /**
     * Variant division operator.
     *
     * \param[in] a The dividend.
     *
     * \param[in] b The divisor.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator/(const Variant& a, const Complex& b);

    /**
     * Variant division operator.
     *
     * \param[in] a The dividend.
     *
     * \param[in] b The divisor.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Tuple operator/(const Variant& a, const Tuple& b);

    /**
     * Variant division operator.
     *
     * \param[in] a The dividend.
     *
     * \param[in] b The divisor.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator/(Boolean a, const Variant& b);

    /**
     * Variant division operator.
     *
     * \param[in] a The dividend.
     *
     * \param[in] b The divisor.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator/(Integer a, const Variant& b);

    /**
     * Variant division operator.
     *
     * \param[in] a The dividend.
     *
     * \param[in] b The divisor.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator/(Real a, const Variant& b);

    /**
     * Variant division operator.
     *
     * \param[in] a The dividend.
     *
     * \param[in] b The divisor.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator/(const Complex& a, const Variant& b);

    /**
     * Variant division operator.
     *
     * \param[in] a The dividend.
     *
     * \param[in] b The divisor.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Tuple operator/(const Tuple& a, const Variant& b);

    /**
     * Variant division operator.
     *
     * \param[in] a The dividend.
     *
     * \param[in] b The divisor.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator/(const MatrixBoolean& a, const Variant& b);

    /**
     * Variant division operator.
     *
     * \param[in] a The dividend.
     *
     * \param[in] b The divisor.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator/(const MatrixInteger& a, const Variant& b);

    /**
     * Variant division operator.
     *
     * \param[in] a The dividend.
     *
     * \param[in] b The divisor.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator/(const MatrixReal& a, const Variant& b);

    /**
     * Variant division operator.
     *
     * \param[in] a The dividend.
     *
     * \param[in] b The divisor.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API MatrixComplex operator/(const MatrixComplex& a, const Variant& b);

    /**
     * Variant division operator.
     *
     * \param[in] a The dividend.
     *
     * \param[in] b The divisor.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator/(const Variant& a, const Variant& b);

    /**
     * Variant unary + operator.
     *
     * \param[in] value The value of interest.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator+(const Variant& value);

    /**
     * Variant unary - operator.
     *
     * \param[in] value The value of interest.
     *
     * \return Returns the result from the calculation.
     */
    M_PUBLIC_API Variant operator-(const Variant& value);

    /**
     * Variant logical AND operator.
     *
     * \param[in] a The first value to apply.
     *
     * \param[in] b The second value to apply.
     *
     * \return Returns true if both values are true..
     */
    M_PUBLIC_API bool operator&&(const Variant& a, Boolean b);

    /**
     * Variant logical AND operator.
     *
     * \param[in] a The first value to apply.
     *
     * \param[in] b The second value to apply.
     *
     * \return Returns true if both values are true..
     */
    M_PUBLIC_API bool operator&&(const Variant& a, Integer b);

    /**
     * Variant logical AND operator.
     *
     * \param[in] a The first value to apply.
     *
     * \param[in] b The second value to apply.
     *
     * \return Returns true if both values are true..
     */
    M_PUBLIC_API bool operator&&(const Variant& a, Real b);

    /**
     * Variant logical AND operator.
     *
     * \param[in] a The first value to apply.
     *
     * \param[in] b The second value to apply.
     *
     * \return Returns true if both values are true..
     */
    M_PUBLIC_API bool operator&&(const Variant& a, const Complex& b);

    /**
     * Variant logical AND operator.
     *
     * \param[in] a The first value to apply.
     *
     * \param[in] b The second value to apply.
     *
     * \return Returns true if both values are true..
     */
    M_PUBLIC_API bool operator&&(Boolean a, const Variant& b);

    /**
     * Variant logical AND operator.
     *
     * \param[in] a The first value to apply.
     *
     * \param[in] b The second value to apply.
     *
     * \return Returns true if both values are true..
     */
    M_PUBLIC_API bool operator&&(Integer a, const Variant& b);

    /**
     * Variant logical AND operator.
     *
     * \param[in] a The first value to apply.
     *
     * \param[in] b The second value to apply.
     *
     * \return Returns true if both values are true..
     */
    M_PUBLIC_API bool operator&&(Real a, const Variant& b);

    /**
     * Variant logical AND operator.
     *
     * \param[in] a The first value to apply.
     *
     * \param[in] b The second value to apply.
     *
     * \return Returns true if both values are true..
     */
    M_PUBLIC_API bool operator&&(const Complex& a, const Variant& b);

    /**
     * Variant logical AND operator.
     *
     * \param[in] a The first value to apply.
     *
     * \param[in] b The second value to apply.
     *
     * \return Returns true if both values are true..
     */
    M_PUBLIC_API bool operator&&(const Variant& a, const Variant& b);

    /**
     * Variant logical OR operator.
     *
     * \param[in] a The first value to apply.
     *
     * \param[in] b The second value to apply.
     *
     * \return Returns true if either value is true..
     */
    M_PUBLIC_API bool operator||(const Variant& a, Boolean b);

    /**
     * Variant logical OR operator.
     *
     * \param[in] a The first value to apply.
     *
     * \param[in] b The second value to apply.
     *
     * \return Returns true if either value is true..
     */
    M_PUBLIC_API bool operator||(const Variant& a, Integer b);

    /**
     * Variant logical OR operator.
     *
     * \param[in] a The first value to apply.
     *
     * \param[in] b The second value to apply.
     *
     * \return Returns true if either value is true..
     */
    M_PUBLIC_API bool operator||(const Variant& a, Real b);

    /**
     * Variant logical OR operator.
     *
     * \param[in] a The first value to apply.
     *
     * \param[in] b The second value to apply.
     *
     * \return Returns true if either value is true..
     */
    M_PUBLIC_API bool operator||(const Variant& a, const Complex& b);

    /**
     * Variant logical OR operator.
     *
     * \param[in] a The first value to apply.
     *
     * \param[in] b The second value to apply.
     *
     * \return Returns true if either value is true..
     */
    M_PUBLIC_API bool operator||(Boolean a, const Variant& b);

    /**
     * Variant logical OR operator.
     *
     * \param[in] a The first value to apply.
     *
     * \param[in] b The second value to apply.
     *
     * \return Returns true if either value is true..
     */
    M_PUBLIC_API bool operator||(Integer a, const Variant& b);

    /**
     * Variant logical OR operator.
     *
     * \param[in] a The first value to apply.
     *
     * \param[in] b The second value to apply.
     *
     * \return Returns true if either value is true..
     */
    M_PUBLIC_API bool operator||(Real a, const Variant& b);

    /**
     * Variant logical OR operator.
     *
     * \param[in] a The first value to apply.
     *
     * \param[in] b The second value to apply.
     *
     * \return Returns true if either value is true..
     */
    M_PUBLIC_API bool operator||(const Complex& a, const Variant& b);

    /**
     * Variant logical OR operator.
     *
     * \param[in] a The first value to apply.
     *
     * \param[in] b The second value to apply.
     *
     * \return Returns true if either value is true..
     */
    M_PUBLIC_API bool operator||(const Variant& a, const Variant& b);

    /**
     * Variant logical not operator.
     *
     * \param[in] value The value to calculate the logical "not" of.
     *
     * \return Returns true if value resolves to false. Returns false if value resolves to true.
     */
    M_PUBLIC_API inline bool operator!(const Variant& value) {
        return !value.toBoolean();
    }
}

#endif
