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
* This header defines inline functions used to perform variable assignments.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_ASSIGNMENT_FUNCTIONS_H
#define M_ASSIGNMENT_FUNCTIONS_H

#include "m_common.h"
#include "model_intrinsic_types.h"
#include "model_set.h"
#include "model_tuple.h"
#include "model_matrix_boolean.h"
#include "model_matrix_integer.h"
#include "model_matrix_real.h"
#include "model_matrix_complex.h"
#include "m_intrinsic_types.h"
#include "m_exceptions.h"
#include "m_set.h"
#include "m_tuple.h"
#include "m_matrix_boolean.h"
#include "m_matrix_integer.h"
#include "m_matrix_real.h"
#include "m_matrix_complex.h"
#include "m_variant.h"
#include "m_type_conversion.h"

namespace M {
    /**
     * Method that assigns a scalar type to a boolean value.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Boolean& l, Boolean r) {
        l = r;
    }

    /**
     * Method that assigns a scalar type to a boolean value.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Boolean& l, Integer r) {
        l = (r != 0);
    }

    /**
     * Method that assigns a scalar type to a boolean value.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Boolean& l, Real r) {
        l = (r != 0.0);
    }

    /**
     * Method that assigns a scalar type to a boolean value.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Boolean& l, const Complex& r) {
        l = (r.real() != 0.0 || r.imag() != 0.0);
    }

    /**
     * Method that assigns a variant to a boolean value.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Boolean& l, const Variant& r) {
        l = r.toBoolean();
    }

    /**
     * Method that assigns a scalar type to an integer value.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Integer& l, Boolean r) {
        l = Integer(r);
    }

    /**
     * Method that assigns a scalar type to an integer value.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Integer& l, Integer r) {
        l = r;
    }

    /**
     * Method that assigns a scalar type to an integer value.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Integer& l, Real r) {
        if (!toInteger(l, r)) {
            l = Integer(r);
        }
    }

    /**
     * Method that assigns a scalar type to an integer value.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Integer& l, const Complex& r) {
        if (!toInteger(l, r)) {
            l = Integer(r.real());
        }
    }

    /**
     * Method that assigns a variant to an integer value.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Integer& l, const Variant& r) {
        l = r.toInteger();
    }

    /**
     * Method that assigns a scalar type to a real value.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Real& l, Boolean r) {
        l = Real(r);
    }

    /**
     * Method that assigns a scalar type to a real value.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Real& l, Integer r) {
        l = Real(r);
    }

    /**
     * Method that assigns a scalar type to a real value.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Real& l, Real r) {
        l = r;
    }

    /**
     * Method that assigns a scalar type to a real value.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Real& l, const Complex& r) {
        if (!toReal(l, r)) {
            l = r.real();
        }
    }

    /**
     * Method that assigns a variant to a real value.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Real& l, const Variant& r) {
        l = r.toReal();
    }

    /**
     * Method that assigns a scalar type to a complex value.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Complex& l, Boolean r) {
        l = Complex(r);
    }

    /**
     * Method that assigns a scalar type to a complex value.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Complex& l, Integer r) {
        l = Complex(r);
    }

    /**
     * Method that assigns a scalar type to a complex value.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Complex& l, Real r) {
        l = Complex(r);
    }

    /**
     * Method that assigns a scalar type to a complex value.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Complex& l, const Complex& r) {
        l = r;
    }

    /**
     * Method that assigns a variant to a complex value.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Complex& l, const Variant& r) {
        l = r.toComplex();
    }

    /**
     * Method that assigns set values.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Set& l, const Set& r) {
        l = r;
    }

    /**
     * Method that assigns a variant to a set.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Set& l, const Variant& r) {
        l = r.toSet();
    }

    /**
     * Method that assigns tuple values.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Tuple& l, const Tuple& r) {
        l = r;
    }

    /**
     * Method that assigns a variant to a tuple.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Tuple& l, const Variant& r) {
        l = r.toTuple();
    }

    /**
     * Method that assigns a matrix value to a boolean matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixBoolean& l, const MatrixBoolean& r) {
        l = r;
    }

    /**
     * Method that assigns a matrix value to a boolean matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixBoolean& l, const MatrixInteger& r) {
        l = r;
    }

    /**
     * Method that assigns a matrix value to a boolean matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixBoolean& l, const MatrixReal& r) {
        l = r;
    }

    /**
     * Method that assigns a matrix value to a boolean matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixBoolean& l, const MatrixComplex& r) {
        l = r;
    }

    /**
     * Method that assigns a variant to a boolean matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixBoolean& l, const Variant& r) {
        l = r.toMatrixBoolean();
    }

    /**
     * Method that assigns a matrix value to an integer matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixInteger& l, const MatrixBoolean& r) {
        l = r;
    }

    /**
     * Method that assigns a matrix value to an integer matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixInteger& l, const MatrixInteger& r) {
        l = r;
    }

    /**
     * Method that assigns a matrix value to an integer matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixInteger& l, const MatrixReal& r) {
        l = r;
    }

    /**
     * Method that assigns a matrix value to an integer matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixInteger& l, const MatrixComplex& r) {
        l = r;
    }

    /**
     * Method that assigns a variant to an integer matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixInteger& l, const Variant& r) {
        l = r.toMatrixInteger();
    }

    /**
     * Method that assigns a matrix value to a real matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixReal& l, const MatrixBoolean& r) {
        l = r;
    }

    /**
     * Method that assigns a matrix value to a real matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixReal& l, const MatrixInteger& r) {
        l = r;
    }

    /**
     * Method that assigns a matrix value to a real matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixReal& l, const MatrixReal& r) {
        l = r;
    }

    /**
     * Method that assigns a matrix value to a real matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixReal& l, const MatrixComplex& r) {
        l = r;
    }

    /**
     * Method that assigns a variant to a real matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixReal& l, const Variant& r) {
        l = r.toMatrixReal();
    }

    /**
     * Method that assigns a matrix value to a complex matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixComplex& l, const MatrixBoolean& r) {
        l = r;
    }

    /**
     * Method that assigns a matrix value to a complex matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixComplex& l, const MatrixInteger& r) {
        l = r;
    }

    /**
     * Method that assigns a matrix value to a complex matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixComplex& l, const MatrixReal& r) {
        l = r;
    }

    /**
     * Method that assigns a matrix value to a complex matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixComplex& l, const MatrixComplex& r) {
        l = r;
    }

    /**
     * Method that assigns a variant to a complex matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixComplex& l, const Variant& r) {
        l = r.toMatrixComplex();
    }

    /**
     * Method that assigns a boolean to a variant.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Variant& l, Boolean r) {
        l = r;
    }

    /**
     * Method that assigns an integer to a variant.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Variant& l, Integer r) {
        l = r;
    }

    /**
     * Method that assigns a real to a variant.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Variant& l, Real r) {
        l = r;
    }

    /**
     * Method that assigns a complex to a variant.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Variant& l, const Complex& r) {
        l = r;
    }

    /**
     * Method that assigns a set to a variant.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Variant& l, const Set& r) {
        l = r;
    }

    /**
     * Method that assigns a tuple to a variant.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Variant& l, const Tuple& r) {
        l = r;
    }

    /**
     * Method that assigns a boolean matrix to a variant.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Variant& l, const MatrixBoolean& r) {
        l = r;
    }

    /**
     * Method that assigns an integer matrix to a variant.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Variant& l, const MatrixInteger& r) {
        l = r;
    }

    /**
     * Method that assigns a real matrix to a variant.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Variant& l, const MatrixReal& r) {
        l = r;
    }

    /**
     * Method that assigns a complex matrix to a variant.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Variant& l, const MatrixComplex& r) {
        l = r;
    }

    /**
     * Method that assigns a variant to a variant.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Variant& l, const Variant& r) {
        l = r;
    }

    /**
     * Method that assigns set values.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Set& l, const Model::Set& r) {
        l = M::Set(r);
    }

    /**
     * Method that assigns a variant to a set.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Set& l, const Model::Variant& r) {
        l = M::Set(r.toSet());
    }

    /**
     * Method that assigns tuple values.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Tuple& l, const Model::Tuple& r) {
        l = M::Tuple(r);
    }

    /**
     * Method that assigns a variant to a tuple.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Tuple& l, const Model::Variant& r) {
        l = M::Tuple(r.toTuple());
    }

    /**
     * Method that assigns a matrix value to a boolean matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixBoolean& l, const Model::MatrixBoolean& r) {
        l = r;
    }

    /**
     * Method that assigns a matrix value to a boolean matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixBoolean& l, const Model::MatrixInteger& r) {
        l = r;
    }

    /**
     * Method that assigns a matrix value to a boolean matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixBoolean& l, const Model::MatrixReal& r) {
        l = r;
    }

    /**
     * Method that assigns a matrix value to a boolean matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixBoolean& l, const Model::MatrixComplex& r) {
        l = r;
    }

    /**
     * Method that assigns a variant to a boolean matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixBoolean& l, const Model::Variant& r) {
        l = r.toMatrixBoolean();
    }

    /**
     * Method that assigns a matrix value to an integer matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixInteger& l, const Model::MatrixBoolean& r) {
        l = r;
    }

    /**
     * Method that assigns a matrix value to an integer matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixInteger& l, const Model::MatrixInteger& r) {
        l = r;
    }

    /**
     * Method that assigns a matrix value to an integer matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixInteger& l, const Model::MatrixReal& r) {
        l = r;
    }

    /**
     * Method that assigns a matrix value to an integer matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixInteger& l, const Model::MatrixComplex& r) {
        l = r;
    }

    /**
     * Method that assigns a variant to an integer matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixInteger& l, const Model::Variant& r) {
        l = r.toMatrixInteger();
    }

    /**
     * Method that assigns a matrix value to a real matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixReal& l, const Model::MatrixBoolean& r) {
        l = r;
    }

    /**
     * Method that assigns a matrix value to a real matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixReal& l, const Model::MatrixInteger& r) {
        l = r;
    }

    /**
     * Method that assigns a matrix value to a real matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixReal& l, const Model::MatrixReal& r) {
        l = r;
    }

    /**
     * Method that assigns a matrix value to a real matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixReal& l, const Model::MatrixComplex& r) {
        l = r;
    }

    /**
     * Method that assigns a variant to a real matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixReal& l, const Model::Variant& r) {
        l = r.toMatrixReal();
    }

    /**
     * Method that assigns a matrix value to a complex matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixComplex& l, const Model::MatrixBoolean& r) {
        l = r;
    }

    /**
     * Method that assigns a matrix value to a complex matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixComplex& l, const Model::MatrixInteger& r) {
        l = r;
    }

    /**
     * Method that assigns a matrix value to a complex matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixComplex& l, const Model::MatrixReal& r) {
        l = r;
    }

    /**
     * Method that assigns a matrix value to a complex matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixComplex& l, const Model::MatrixComplex& r) {
        l = r;
    }

    /**
     * Method that assigns a variant to a complex matrix.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(MatrixComplex& l, const Model::Variant& r) {
        l = r.toMatrixComplex();
    }

    /**
     * Method that assigns a set to a variant.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Variant& l, const Model::Set& r) {
        l = M::Set(r);
    }

    /**
     * Method that assigns a tuple to a variant.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Variant& l, const Model::Tuple& r) {
        l = M::Tuple(r);
    }

    /**
     * Method that assigns a boolean matrix to a variant.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Variant& l, const Model::MatrixBoolean& r) {
        l = r;
    }

    /**
     * Method that assigns an integer matrix to a variant.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Variant& l, const Model::MatrixInteger& r) {
        l = r;
    }

    /**
     * Method that assigns a real matrix to a variant.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Variant& l, const Model::MatrixReal& r) {
        l = r;
    }

    /**
     * Method that assigns a complex matrix to a variant.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Variant& l, const Model::MatrixComplex& r) {
        l = r;
    }

    /**
     * Method that assigns a variant to a variant.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    M_PUBLIC_API inline void assign(Variant& l, const Model::Variant& r) {
        l = M::Variant(r);
    }

    /**
     * Template method that assigns a new value.
     *
     * \param[out] l The L-value to receive the result.
     *
     * \param[in]  r The R-value holding the value to be assigned.
     */
    template<typename L, typename R> M_PUBLIC_TEMPLATE_FUNCTION void assign(L& l, const R& r) {
        l = r;
    }
}

#endif
