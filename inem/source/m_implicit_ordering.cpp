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
* This header implements functions you can use enforce ordering between values of different types.
***********************************************************************************************************************/

#include <cstring>
#include <cmath>
#include <cassert>
#include <limits>
#include <complex>

#include "m_intrinsic_types.h"
#include "m_api_types.h"
#include "m_exceptions.h"
#include "m_intrinsic_types.h"
#include "m_basic_functions.h"
#include "m_set.h"
#include "m_set_iterator.h"
#include "m_tuple.h"
#include "m_matrix_boolean.h"
#include "m_matrix_integer.h"
#include "m_matrix_real.h"
#include "m_matrix_complex.h"
#include "m_variant.h"
#include "m_implicit_ordering.h"

namespace M {
    int implicitOrdering(Boolean a, Integer b) {
        Integer ai = a ? 1 : 0;
        return ai == b ? 0 : (ai < b ? -1 : +1);
    }


    int implicitOrdering(Boolean a, Real b) {
        Integer ai = a ? 1 : 0;
        return ai == b ? 0 : (ai < b ? -1 : +1);
    }


    int implicitOrdering(Boolean a, const Complex& b) {
        Integer ai = a ? 1 : 0;
        Real    mb = b.real() + b.imag();

        return ai == mb ? 0 : (ai < mb ? -1 : +1);
    }


    int implicitOrdering(Integer a, Boolean b) {
        Integer bi = b ? 1 : 0;
        return a == bi ? 0 : (a < bi ? -1 : +1);
    }


    int implicitOrdering(Integer a, Integer b) {
        return a == b ? 0 : (a < b ? -1 : +1);
    }


    int implicitOrdering(Integer a, Real b) {
        Real ar = Real(a);
        return ar == b ? 0 : (ar < b ? -1 : +1);
    }


    int implicitOrdering(Integer a, const Complex& b) {
        Real ar = Real(a);
        Real mb = b.real() + b.imag();

        return ar == mb ? 0 : (ar < mb ? -1 : +1);
    }


    int implicitOrdering(Real a, Boolean b) {
        Real br = b ? 1 : 0;
        return a == br ? 0 : (a < br ? -1 : +1);
    }


    int implicitOrdering(Real a, Integer b) {
        Real br = Real(b);
        return a == br ? 0 : (a < br ? -1 : +1);
    }


    int implicitOrdering(Real a, Real b) {
        return a == b ? 0 : (a < b ? -1 : +1);
    }


    int implicitOrdering(Real a, const Complex& b) {
        Real mb = b.real() + b.imag();
        return a == mb ? 0 : (a < mb ? -1 : +1);
    }


    int implicitOrdering(const Complex& a, Boolean b) {
        Real ma = a.real() + a.imag();
        Real br = b ? 1 : 0;
        return ma == br ? 0 : (ma < br ? -1 : +1);
    }


    int implicitOrdering(const Complex& a, Integer b) {
        Real ma = a.real() + a.imag();
        return ma == b ? 0 : (ma < b ? -1 : +1);
    }


    int implicitOrdering(const Complex& a, Real b) {
        Real ma = a.real() + a.imag();
        return ma == b ? 0 : (ma < b ? -1 : +1);
    }


    int implicitOrdering(const Complex& a, const Complex& b) {
        Real ma = a.real() + a.imag();
        Real mb = b.real() + b.imag();
        return ma == mb ? 0 : (ma < mb ? -1 : +1);
    }


    int implicitOrdering(const Set& a, const Set& b) {
        return a.relativeOrder(b);
    }

    int implicitOrdering(const Tuple& a, const Tuple& b) {
        return a.relativeOrder(b);
    }

    int implicitOrdering(const MatrixBoolean& a, const MatrixBoolean& b) {
        return a.relativeOrder(b);
    }

    int implicitOrdering(const MatrixInteger& a, const MatrixInteger& b) {
        return a.relativeOrder(b);
    }

    int implicitOrdering(const MatrixReal& a, const MatrixReal& b) {
        return a.relativeOrder(b);
    }

    int implicitOrdering(const MatrixComplex& a, const MatrixComplex& b) {
        return a.relativeOrder(b);
    }


    int implicitOrdering(const Variant& a, Boolean b) {
        switch (a.valueType()) {
            case M::ValueType::NONE:
            case M::ValueType::VARIANT: {
                assert(false);
                return 0;
            }

            case M::ValueType::BOOLEAN:        { return implicitOrdering(a.toBoolean(), b); }
            case M::ValueType::INTEGER:        { return implicitOrdering(a.toInteger(), b); }
            case M::ValueType::REAL:           { return implicitOrdering(a.toReal(), b); }
            case M::ValueType::COMPLEX:        { return implicitOrdering(a.toComplex(), b); }
            case M::ValueType::SET:            { return +1; }
            case M::ValueType::TUPLE:          { return +1; }
            case M::ValueType::MATRIX_BOOLEAN: { return +1; }
            case M::ValueType::MATRIX_INTEGER: { return +1; }
            case M::ValueType::MATRIX_REAL:    { return +1; }
            case M::ValueType::MATRIX_COMPLEX: { return +1; }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    int implicitOrdering(const Variant& a, Integer b) {
        switch (a.valueType()) {
            case M::ValueType::NONE:
            case M::ValueType::VARIANT: {
                assert(false);
                return 0;
            }

            case M::ValueType::BOOLEAN:        { return implicitOrdering(a.toBoolean(), b); }
            case M::ValueType::INTEGER:        { return implicitOrdering(a.toInteger(), b); }
            case M::ValueType::REAL:           { return implicitOrdering(a.toReal(), b); }
            case M::ValueType::COMPLEX:        { return implicitOrdering(a.toComplex(), b); }
            case M::ValueType::SET:            { return +1; }
            case M::ValueType::TUPLE:          { return +1; }
            case M::ValueType::MATRIX_BOOLEAN: { return +1; }
            case M::ValueType::MATRIX_INTEGER: { return +1; }
            case M::ValueType::MATRIX_REAL:    { return +1; }
            case M::ValueType::MATRIX_COMPLEX: { return +1; }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    int implicitOrdering(const Variant& a, Real b) {
        switch (a.valueType()) {
            case M::ValueType::NONE:
            case M::ValueType::VARIANT: {
                assert(false);
                return 0;
            }

            case M::ValueType::BOOLEAN:        { return implicitOrdering(a.toBoolean(), b); }
            case M::ValueType::INTEGER:        { return implicitOrdering(a.toInteger(), b); }
            case M::ValueType::REAL:           { return implicitOrdering(a.toReal(), b); }
            case M::ValueType::COMPLEX:        { return implicitOrdering(a.toComplex(), b); }
            case M::ValueType::SET:            { return +1; }
            case M::ValueType::TUPLE:          { return +1; }
            case M::ValueType::MATRIX_BOOLEAN: { return +1; }
            case M::ValueType::MATRIX_INTEGER: { return +1; }
            case M::ValueType::MATRIX_REAL:    { return +1; }
            case M::ValueType::MATRIX_COMPLEX: { return +1; }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    int implicitOrdering(const Variant& a, const Complex& b) {
        switch (a.valueType()) {
            case M::ValueType::NONE:
            case M::ValueType::VARIANT: {
                assert(false);
                return 0;
            }

            case M::ValueType::BOOLEAN:        { return implicitOrdering(a.toBoolean(), b); }
            case M::ValueType::INTEGER:        { return implicitOrdering(a.toInteger(), b); }
            case M::ValueType::REAL:           { return implicitOrdering(a.toReal(), b); }
            case M::ValueType::COMPLEX:        { return implicitOrdering(a.toComplex(), b); }
            case M::ValueType::SET:            { return +1; }
            case M::ValueType::TUPLE:          { return +1; }
            case M::ValueType::MATRIX_BOOLEAN: { return +1; }
            case M::ValueType::MATRIX_INTEGER: { return +1; }
            case M::ValueType::MATRIX_REAL:    { return +1; }
            case M::ValueType::MATRIX_COMPLEX: { return +1; }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    int implicitOrdering(const Variant& a, const Set& b) {
        switch (a.valueType()) {
            case M::ValueType::NONE:
            case M::ValueType::VARIANT: {
                assert(false);
                return 0;
            }

            case M::ValueType::BOOLEAN:        { return -1; }
            case M::ValueType::INTEGER:        { return -1; }
            case M::ValueType::REAL:           { return -1; }
            case M::ValueType::COMPLEX:        { return -1; }
            case M::ValueType::SET:            { return implicitOrdering(a.toSet(), b); }
            case M::ValueType::TUPLE:          { return +1; }
            case M::ValueType::MATRIX_BOOLEAN: { return +1; }
            case M::ValueType::MATRIX_INTEGER: { return +1; }
            case M::ValueType::MATRIX_REAL:    { return +1; }
            case M::ValueType::MATRIX_COMPLEX: { return +1; }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    int implicitOrdering(const Variant& a, const Tuple& b) {
        switch (a.valueType()) {
            case M::ValueType::NONE:
            case M::ValueType::VARIANT: {
                assert(false);
                return 0;
            }

            case M::ValueType::BOOLEAN:        { return -1; }
            case M::ValueType::INTEGER:        { return -1; }
            case M::ValueType::REAL:           { return -1; }
            case M::ValueType::COMPLEX:        { return -1; }
            case M::ValueType::SET:            { return -1; }
            case M::ValueType::TUPLE:          { return implicitOrdering(a.toTuple(), b); }
            case M::ValueType::MATRIX_BOOLEAN: { return +1; }
            case M::ValueType::MATRIX_INTEGER: { return +1; }
            case M::ValueType::MATRIX_REAL:    { return +1; }
            case M::ValueType::MATRIX_COMPLEX: { return +1; }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    int implicitOrdering(const Variant& a, const MatrixBoolean& b) {
        switch (a.valueType()) {
            case M::ValueType::NONE:
            case M::ValueType::VARIANT: {
                assert(false);
                return 0;
            }

            case M::ValueType::BOOLEAN:        { return -1; }
            case M::ValueType::INTEGER:        { return -1; }
            case M::ValueType::REAL:           { return -1; }
            case M::ValueType::COMPLEX:        { return -1; }
            case M::ValueType::SET:            { return -1; }
            case M::ValueType::TUPLE:          { return -1; }
            case M::ValueType::MATRIX_BOOLEAN: { return implicitOrdering(a.toMatrixBoolean(), b); }
            case M::ValueType::MATRIX_INTEGER: { return +1; }
            case M::ValueType::MATRIX_REAL:    { return +1; }
            case M::ValueType::MATRIX_COMPLEX: { return +1; }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    int implicitOrdering(const Variant& a, const MatrixInteger& b) {
        switch (a.valueType()) {
            case M::ValueType::NONE:
            case M::ValueType::VARIANT: {
                assert(false);
                return 0;
            }

            case M::ValueType::BOOLEAN:        { return -1; }
            case M::ValueType::INTEGER:        { return -1; }
            case M::ValueType::REAL:           { return -1; }
            case M::ValueType::COMPLEX:        { return -1; }
            case M::ValueType::SET:            { return -1; }
            case M::ValueType::TUPLE:          { return -1; }
            case M::ValueType::MATRIX_BOOLEAN: { return -1; }
            case M::ValueType::MATRIX_INTEGER: { return implicitOrdering(a.toMatrixInteger(), b); }
            case M::ValueType::MATRIX_REAL:    { return +1; }
            case M::ValueType::MATRIX_COMPLEX: { return +1; }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    int implicitOrdering(const Variant& a, const MatrixReal& b) {
        switch (a.valueType()) {
            case M::ValueType::NONE:
            case M::ValueType::VARIANT: {
                assert(false);
                return 0;
            }

            case M::ValueType::BOOLEAN:        { return -1; }
            case M::ValueType::INTEGER:        { return -1; }
            case M::ValueType::REAL:           { return -1; }
            case M::ValueType::COMPLEX:        { return -1; }
            case M::ValueType::SET:            { return -1; }
            case M::ValueType::TUPLE:          { return -1; }
            case M::ValueType::MATRIX_BOOLEAN: { return -1; }
            case M::ValueType::MATRIX_INTEGER: { return -1; }
            case M::ValueType::MATRIX_REAL:    { return implicitOrdering(a.toMatrixReal(), b); }
            case M::ValueType::MATRIX_COMPLEX: { return +1; }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    int implicitOrdering(const Variant& a, const MatrixComplex& b) {
        switch (a.valueType()) {
            case M::ValueType::NONE:
            case M::ValueType::VARIANT: {
                assert(false);
                return 0;
            }

            case M::ValueType::BOOLEAN:        { return -1; }
            case M::ValueType::INTEGER:        { return -1; }
            case M::ValueType::REAL:           { return -1; }
            case M::ValueType::COMPLEX:        { return -1; }
            case M::ValueType::SET:            { return -1; }
            case M::ValueType::TUPLE:          { return -1; }
            case M::ValueType::MATRIX_BOOLEAN: { return -1; }
            case M::ValueType::MATRIX_INTEGER: { return -1; }
            case M::ValueType::MATRIX_REAL:    { return -1; }
            case M::ValueType::MATRIX_COMPLEX: { return implicitOrdering(a.toMatrixComplex(), b); }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    int implicitOrdering(Boolean a, const Variant& b) {
        return -implicitOrdering(b, a);
    }


    int implicitOrdering(Integer a, const Variant& b) {
        return -implicitOrdering(b, a);
    }


    int implicitOrdering(Real a, const Variant& b) {
        return -implicitOrdering(b, a);
    }


    int implicitOrdering(const Complex& a, const Variant& b) {
        return -implicitOrdering(b, a);
    }


    int implicitOrdering(const Set& a, const Variant& b) {
        return -implicitOrdering(b, a);
    }


    int implicitOrdering(const Tuple& a, const Variant& b) {
        return -implicitOrdering(b, a);
    }


    int implicitOrdering(const MatrixBoolean& a, const Variant& b) {
        return -implicitOrdering(b, a);
    }


    int implicitOrdering(const MatrixInteger& a, const Variant& b) {
        return -implicitOrdering(b, a);
    }


    int implicitOrdering(const MatrixReal& a, const Variant& b) {
        return -implicitOrdering(b, a);
    }


    int implicitOrdering(const MatrixComplex& a, const Variant& b) {
        return -implicitOrdering(b, a);
    }


    int implicitOrdering(const Variant& a, const Variant& b) {
        ValueType bValueType = b.valueType();
        switch (bValueType) {
            case M::ValueType::NONE:
            case M::ValueType::VARIANT: {
                assert(false);
                return 0;
            }

            case M::ValueType::BOOLEAN: {
                return implicitOrdering(a, b.toBoolean());
            }

            case M::ValueType::INTEGER: {
                return implicitOrdering(a, b.toInteger());
            }

            case M::ValueType::REAL: {
                return implicitOrdering(a, b.toReal());
            }

            case M::ValueType::COMPLEX: {
                return implicitOrdering(a, b.toComplex());
            }

            case M::ValueType::SET:
            case M::ValueType::TUPLE:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                ValueType aValueType = a.valueType();

                if (aValueType == bValueType) {
                    switch (bValueType) {
                        case M::ValueType::NONE:
                        case M::ValueType::VARIANT:
                        case M::ValueType::BOOLEAN:
                        case M::ValueType::INTEGER:
                        case M::ValueType::REAL:
                        case M::ValueType::COMPLEX: {
                            assert(false);
                            return 0;
                        }

                        case M::ValueType::SET: {
                            return implicitOrdering(a.toSet(), b.toSet());
                        }

                        case M::ValueType::TUPLE: {
                            return implicitOrdering(a.toTuple(), b.toTuple());
                        }

                        case M::ValueType::MATRIX_BOOLEAN: {
                            return implicitOrdering(a.toMatrixBoolean(), b.toMatrixBoolean());
                        }

                        case M::ValueType::MATRIX_INTEGER: {
                            return implicitOrdering(a.toMatrixInteger(), b.toMatrixInteger());
                        }

                        case M::ValueType::MATRIX_REAL: {
                            return implicitOrdering(a.toMatrixReal(), b.toMatrixReal());
                        }

                        case M::ValueType::MATRIX_COMPLEX: {
                            return implicitOrdering(a.toMatrixComplex(), b.toMatrixComplex());
                        }

                        default: {
                            assert(false);
                            return 0;
                        }
                    }
                } else if (aValueType < bValueType) {
                    return -1;
                } else {
                    return +1;
                }
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }
}
