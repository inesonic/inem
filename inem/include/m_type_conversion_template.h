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
* This header provides functions to provide compile-time casting of types.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_STATIC_UPCAST_H
#define M_STATIC_UPCAST_H

#include "m_common.h"
#include "m_api_types.h"
#include "m_intrinsic_types.h"
#include "m_type_conversion.h"
#include "m_set.h"
#include "m_tuple.h"
#include "m_matrix_boolean.h"
#include "m_matrix_integer.h"
#include "m_matrix_real.h"
#include "m_matrix_complex.h"
#include "m_variant.h"

namespace M {
    /**
     * Template function that performs a static upcast of value to a given type.
     *
     * \param[in] value The value to be cast and returned.
     *
     * \returns Returns value properly upcast.
     */
    template<typename Result> M_PUBLIC_TEMPLATE_FUNCTION Result inlineTo(const Boolean& value) {
        return Result(value ? 1 : 0);
    }

    /**
     * Template function that performs a static upcast of value to a given type.
     *
     * \param[in] value The value to be cast and returned.
     *
     * \returns Returns value properly upcast.
     */
    template<typename Result> M_PUBLIC_TEMPLATE_FUNCTION Result inlineTo(const Integer& value) {
        return static_cast<Result>(value);
    }

    /**
     * Template function that performs a static upcast of value to a given type.
     *
     * \param[in] value The value to be cast and returned.
     *
     * \returns Returns value properly upcast.
     */
    template<typename Result> M_PUBLIC_TEMPLATE_FUNCTION Result inlineTo(const Real& value) {
        return static_cast<Result>(value);
    }

    /**
     * Template function that performs a static upcast of value to a given type.
     *
     * \param[in] value The value to be cast and returned.
     *
     * \returns Returns value properly upcast.
     */
    template<typename Result> M_PUBLIC_TEMPLATE_FUNCTION Result inlineTo(const Complex& value) {
        return static_cast<Result>(value);
    }

    /**
     * Template function that performs a static upcast of value to a given type.
     *
     * \param[in] value The value to be cast and returned.
     *
     * \returns Returns value properly upcast.
     */
    template<typename Result> M_PUBLIC_TEMPLATE_FUNCTION Result inlineTo(const MatrixBoolean& value) {
        return Result(value);
    }

    /**
     * Template function that performs a static upcast of value to a given type.
     *
     * \param[in] value The value to be cast and returned.
     *
     * \returns Returns value properly upcast.
     */
    template<typename Result> M_PUBLIC_TEMPLATE_FUNCTION Result inlineTo(const MatrixInteger& value) {
        return Result(value);
    }

    /**
     * Template function that performs a static upcast of value to a given type.
     *
     * \param[in] value The value to be cast and returned.
     *
     * \returns Returns value properly upcast.
     */
    template<typename Result> M_PUBLIC_TEMPLATE_FUNCTION Result inlineTo(const MatrixReal& value) {
        return Result(value);
    }

    /**
     * Template function that performs a static upcast of value to a given type.
     *
     * \param[in] value The value to be cast and returned.
     *
     * \returns Returns value properly upcast.
     */
    template<typename Result> M_PUBLIC_TEMPLATE_FUNCTION Result inlineTo(const MatrixComplex& value) {
        return Result(value);
    }

    /**
     * Template function that performs a static upcast of value to a given type.
     *
     * \param[in] value The value to be cast and returned.
     *
     * \returns Returns value properly upcast.
     */
    template<> M_PUBLIC_TEMPLATE_FUNCTION Boolean inlineTo<Boolean>(const Boolean& value) {
        return inlineToBoolean(value);
    }

    /**
     * Template function that performs a static upcast of value to a given type.
     *
     * \param[in] value The value to be cast and returned.
     *
     * \returns Returns value properly upcast.
     */
    template<> M_PUBLIC_TEMPLATE_FUNCTION Boolean inlineTo<Boolean>(const Integer& value) {
        return inlineToBoolean(value);
    }

    /**
     * Template function that performs a static upcast of value to a given type.
     *
     * \param[in] value The value to be cast and returned.
     *
     * \returns Returns value properly upcast.
     */
    template<> M_PUBLIC_TEMPLATE_FUNCTION Boolean inlineTo<Boolean>(const Real& value) {
        return inlineToBoolean(value);
    }

    /**
     * Template function that performs a static upcast of value to a given type.
     *
     * \param[in] value The value to be cast and returned.
     *
     * \returns Returns value properly upcast.
     */
    template<> M_PUBLIC_TEMPLATE_FUNCTION Boolean inlineTo<Boolean>(const Complex& value) {
        return inlineToBoolean(value);
    }

    /**
     * Template function that performs a static upcast of value to a given type.
     *
     * \param[in] value The value to be cast and returned.
     *
     * \returns Returns value properly upcast.
     */
    template<> M_PUBLIC_TEMPLATE_FUNCTION Integer inlineTo<Integer>(const Boolean& value) {
        return inlineToInteger(value);
    }

    /**
     * Template function that performs a static upcast of value to a given type.
     *
     * \param[in] value The value to be cast and returned.
     *
     * \returns Returns value properly upcast.
     */
    template<> M_PUBLIC_TEMPLATE_FUNCTION Integer inlineTo<Integer>(const Integer& value) {
        return inlineToInteger(value);
    }

    /**
     * Template function that performs a static upcast of value to a given type.
     *
     * \param[in] value The value to be cast and returned.
     *
     * \returns Returns value properly upcast.
     */
    template<> M_PUBLIC_TEMPLATE_FUNCTION Integer inlineTo<Integer>(const Real& value) {
        return inlineToInteger(value);
    }

    /**
     * Template function that performs a static upcast of value to a given type.
     *
     * \param[in] value The value to be cast and returned.
     *
     * \returns Returns value properly upcast.
     */
    template<> M_PUBLIC_TEMPLATE_FUNCTION Integer inlineTo<Integer>(const Complex& value) {
        return inlineToInteger(value);
    }

    /**
     * Template function that performs a static upcast of value to a given type.
     *
     * \param[in] value The value to be cast and returned.
     *
     * \returns Returns value properly upcast.
     */
    template<> M_PUBLIC_TEMPLATE_FUNCTION Real inlineTo<Real>(const Boolean& value) {
        return inlineToReal(value);
    }

    /**
     * Template function that performs a static upcast of value to a given type.
     *
     * \param[in] value The value to be cast and returned.
     *
     * \returns Returns value properly upcast.
     */
    template<> M_PUBLIC_TEMPLATE_FUNCTION Real inlineTo<Real>(const Integer& value) {
        return inlineToReal(value);
    }

    /**
     * Template function that performs a static upcast of value to a given type.
     *
     * \param[in] value The value to be cast and returned.
     *
     * \returns Returns value properly upcast.
     */
    template<> M_PUBLIC_TEMPLATE_FUNCTION Real inlineTo<Real>(const Real& value) {
        return inlineToReal(value);
    }

    /**
     * Template function that performs a static upcast of value to a given type.
     *
     * \param[in] value The value to be cast and returned.
     *
     * \returns Returns value properly upcast.
     */
    template<> M_PUBLIC_TEMPLATE_FUNCTION Real inlineTo<Real>(const Complex& value) {
        return inlineToReal(value);
    }

    /**
     * Template function that performs a static upcast of value to a given type.
     *
     * \param[in] value The value to be cast and returned.
     *
     * \returns Returns value properly upcast.
     */
    template<> M_PUBLIC_TEMPLATE_FUNCTION Complex inlineTo<Complex>(const Boolean& value) {
        return inlineToComplex(value);
    }

    /**
     * Template function that performs a static upcast of value to a given type.
     *
     * \param[in] value The value to be cast and returned.
     *
     * \returns Returns value properly upcast.
     */
    template<> M_PUBLIC_TEMPLATE_FUNCTION Complex inlineTo<Complex>(const Integer& value) {
        return inlineToComplex(value);
    }

    /**
     * Template function that performs a static upcast of value to a given type.
     *
     * \param[in] value The value to be cast and returned.
     *
     * \returns Returns value properly upcast.
     */
    template<> M_PUBLIC_TEMPLATE_FUNCTION Complex inlineTo<Complex>(const Real& value) {
        return inlineToComplex(value);
    }

    /**
     * Template function that performs a static upcast of value to a given type.
     *
     * \param[in] value The value to be cast and returned.
     *
     * \returns Returns value properly upcast.
     */
    template<> M_PUBLIC_TEMPLATE_FUNCTION Complex inlineTo<Complex>(const Complex& value) {
        return inlineToComplex(value);
    }
}

#endif


