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
* This file implements a collection of functions used to manipulate sets.
***********************************************************************************************************************/

#include <cassert>
#include <complex>
#include <cmath>
#include <limits>
#include <vector>
#include <cstdio>
#include <cstring>
#include <cinttypes>

#include "m_api_types.h"
#include "m_intrinsic_types.h"
#include "model_exceptions.h"
#include "m_set_functions.h"
#include "m_basic_functions.h"
#include "m_matrix_functions.h"
#include "m_variant_functions.h"

namespace M {
    Variant abs(const Variant& value) {
        ValueType valueType = value.valueType();
        switch (valueType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE: {
                M::internalTriggerTypeConversionError(valueType, ValueType::TUPLE);
                return M::Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return Variant(abs(value.toInteger()));
            }

            case ValueType::REAL: {
                return Variant(abs(value.toReal()));
            }

            case M::ValueType::COMPLEX: {
                return Variant(abs(value.toComplex()));
            }

            case M::ValueType::SET:  {
                return Variant(abs(value.toSet()));
            }

            case M::ValueType::MATRIX_BOOLEAN: {
                return Variant(abs(value.toMatrixBoolean()));
            }

            case M::ValueType::MATRIX_INTEGER: {
                return Variant(abs(value.toMatrixInteger()));
            }

            case M::ValueType::MATRIX_REAL: {
                return Variant(abs(value.toMatrixReal()));
            }

            case M::ValueType::MATRIX_COMPLEX: {
                return Variant(abs(value.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant();
            }
        }
    }


    Integer size(const Variant& value) {
        ValueType valueType = value.valueType();
        switch (valueType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case M::ValueType::COMPLEX: {
                M::internalTriggerTypeConversionError(valueType, ValueType::TUPLE);
                return 0;
            }

            case M::ValueType::TUPLE: {
                return size(value.toTuple());
            }

            case M::ValueType::SET:  {
                return size(value.toSet());
            }

            case M::ValueType::MATRIX_BOOLEAN: {
                return size(value.toMatrixBoolean());
            }

            case M::ValueType::MATRIX_INTEGER: {
                return size(value.toMatrixInteger());
            }

            case M::ValueType::MATRIX_REAL: {
                return size(value.toMatrixReal());
            }

            case M::ValueType::MATRIX_COMPLEX: {
                return size(value.toMatrixComplex());
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Set alphabet(const Variant& value) {
        ValueType valueType = value.valueType();
        switch (valueType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case M::ValueType::COMPLEX: {
                M::internalTriggerTypeConversionError(valueType, ValueType::SET);
                return Set();
            }

            case M::ValueType::TUPLE: {
                return alphabet(value.toTuple());
            }

            case M::ValueType::SET:  {
                return alphabet(value.toSet());
            }

            case M::ValueType::MATRIX_BOOLEAN: {
                return alphabet(value.toMatrixBoolean());
            }

            case M::ValueType::MATRIX_INTEGER: {
                return alphabet(value.toMatrixInteger());
            }

            case M::ValueType::MATRIX_REAL: {
                return alphabet(value.toMatrixReal());
            }

            case M::ValueType::MATRIX_COMPLEX: {
                return alphabet(value.toMatrixComplex());
            }

            default: {
                assert(false);
                return Set();
            }
        }
    }


    Variant conj(const Variant& value) {
        ValueType valueType = value.valueType();
        switch (valueType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:  {
                M::internalTriggerTypeConversionError(valueType, ValueType::SET);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return Variant(conj(value.toInteger()));
            }

            case ValueType::REAL: {
                return Variant(conj(value.toReal()));
            }

            case M::ValueType::COMPLEX: {
                return Variant(conj(value.toComplex()));
            }

            case M::ValueType::MATRIX_BOOLEAN: {
                return Variant(conj(value.toMatrixBoolean()));
            }

            case M::ValueType::MATRIX_INTEGER: {
                return Variant(conj(value.toMatrixInteger()));
            }

            case M::ValueType::MATRIX_REAL: {
                return Variant(conj(value.toMatrixReal()));
            }

            case M::ValueType::MATRIX_COMPLEX: {
                return Variant(conj(value.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant real(const Variant& value) {
        ValueType valueType = value.valueType();
        switch (valueType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:  {
                M::internalTriggerTypeConversionError(valueType, ValueType::SET);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return Variant(real(value.toInteger()));
            }

            case ValueType::REAL: {
                return Variant(real(value.toReal()));
            }

            case M::ValueType::COMPLEX: {
                return Variant(real(value.toComplex()));
            }

            case M::ValueType::MATRIX_BOOLEAN: {
                return Variant(real(value.toMatrixBoolean()));
            }

            case M::ValueType::MATRIX_INTEGER: {
                return Variant(real(value.toMatrixInteger()));
            }

            case M::ValueType::MATRIX_REAL: {
                return Variant(real(value.toMatrixReal()));
            }

            case M::ValueType::MATRIX_COMPLEX: {
                return Variant(real(value.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant imag(const Variant& value) {
        ValueType valueType = value.valueType();
        switch (valueType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:  {
                M::internalTriggerTypeConversionError(valueType, ValueType::SET);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return Variant(imag(value.toInteger()));
            }

            case ValueType::REAL: {
                return Variant(imag(value.toReal()));
            }

            case M::ValueType::COMPLEX: {
                return Variant(imag(value.toComplex()));
            }

            case M::ValueType::MATRIX_BOOLEAN: {
                return Variant(imag(value.toMatrixBoolean()));
            }

            case M::ValueType::MATRIX_INTEGER: {
                return Variant(imag(value.toMatrixInteger()));
            }

            case M::ValueType::MATRIX_REAL: {
                return Variant(imag(value.toMatrixReal()));
            }

            case M::ValueType::MATRIX_COMPLEX: {
                return Variant(imag(value.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }
}
