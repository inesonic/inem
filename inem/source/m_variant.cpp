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
* This file implements the \ref Model::Variant class.
***********************************************************************************************************************/

#include <cstring>
#include <cmath>
#include <cassert>
#include <limits>
#include <complex>

#include "m_intrinsics.h"
#include "model_intrinsic_types.h"
#include "model_complex.h"
#include "model_exceptions.h"
#include "m_set.h"
#include "m_tuple.h"
#include "m_matrix_boolean.h"
#include "model_matrix_boolean.h"
#include "model_matrix_boolean_private.h"
#include "m_matrix_integer.h"
#include "model_matrix_integer.h"
#include "model_matrix_integer_private.h"
#include "m_matrix_real.h"
#include "model_matrix_real.h"
#include "model_matrix_real_private.h"
#include "m_matrix_complex.h"
#include "model_matrix_complex.h"
#include "model_matrix_complex_private.h"
#include "model_variant.h"
#include "model_variant_private.h"
#include "m_variant.h"

#include "m_variant_upcast_table.inc"

namespace M {
    const Set   Variant::dummySet;
    const Tuple Variant::dummyTuple;

    Variant::Data::Data() {
        std::memset(reinterpret_cast<UI8*>(this), 0, sizeof(Data));
    }


    Variant::Data::~Data() {}


    Variant::Variant() {
        currentValueType = ValueType::NONE;
    }


    Variant::Variant(Model::Boolean value) {
        data.boolean     = value;
        currentValueType = ValueType::BOOLEAN;
    }


    Variant::Variant(Model::Integer value) {
        data.integer     = value;
        currentValueType = ValueType::INTEGER;
    }


    Variant::Variant(Model::Real value) {
        data.real        = value;
        currentValueType = ValueType::REAL;
    }


    Variant::Variant(Model::Complex value) {
        data.complex     = value;
        currentValueType = ValueType::COMPLEX;
    }


    Variant::Variant(const Model::Set& value) {
        (void) new(&data.set) Set(value);
        currentValueType = ValueType::SET;
    }


    Variant::Variant(const M::Set& value) {
        (void) new(&data.set) Set(value);
        currentValueType = ValueType::SET;
    }


    Variant::Variant(const Model::Tuple& value) {
        (void) new(&data.tuple) Tuple(value);
        currentValueType = ValueType::TUPLE;
    }


    Variant::Variant(const M::Tuple& value) {
        (void) new(&data.tuple) Tuple(value);
        currentValueType = ValueType::TUPLE;
    }


    Variant::Variant(const Model::MatrixBoolean& value) {
        (void) new(&data.matrixBoolean) Model::MatrixBoolean(value);
        currentValueType = ValueType::MATRIX_BOOLEAN;
    }


    Variant::Variant(Model::MatrixBoolean&& value) {
        (void) new(&data.matrixBoolean) Model::MatrixBoolean(value);
        currentValueType = ValueType::MATRIX_BOOLEAN;
    }


    Variant::Variant(const M::MatrixBoolean& value) {
        (void) new(&data.matrixBoolean) Model::MatrixBoolean(new Model::MatrixBoolean::Private(value));
        currentValueType = ValueType::MATRIX_BOOLEAN;
    }


    Variant::Variant(const Model::MatrixInteger& value) {
        (void) new(&data.matrixInteger) Model::MatrixInteger(value);
        currentValueType = ValueType::MATRIX_INTEGER;
    }


    Variant::Variant(Model::MatrixInteger&& value) {
        (void) new(&data.matrixInteger) Model::MatrixInteger(value);
        currentValueType = ValueType::MATRIX_INTEGER;
    }


    Variant::Variant(const M::MatrixInteger& value) {
        (void) new(&data.matrixInteger) Model::MatrixInteger(new Model::MatrixInteger::Private(value));
        currentValueType = ValueType::MATRIX_INTEGER;
    }


    Variant::Variant(const Model::MatrixReal& value) {
        (void) new(&data.matrixReal) Model::MatrixReal(value);
        currentValueType = ValueType::MATRIX_REAL;
    }


    Variant::Variant(Model::MatrixReal&& value) {
        (void) new(&data.matrixReal) Model::MatrixReal(value);
        currentValueType = ValueType::MATRIX_REAL;
    }


    Variant::Variant(const M::MatrixReal& value) {
        (void) new(&data.matrixReal) Model::MatrixReal(new Model::MatrixReal::Private(value));
        currentValueType = ValueType::MATRIX_REAL;
    }


    Variant::Variant(const Model::MatrixComplex& value) {
        (void) new(&data.matrixComplex) Model::MatrixComplex(value);
        currentValueType = ValueType::MATRIX_COMPLEX;
    }


    Variant::Variant(Model::MatrixComplex&& value) {
        (void) new(&data.matrixComplex) Model::MatrixComplex(value);
        currentValueType = ValueType::MATRIX_COMPLEX;
    }


    Variant::Variant(const M::MatrixComplex& value) {
        (void) new(&data.matrixComplex) Model::MatrixComplex(new Model::MatrixComplex::Private(value));
        currentValueType = ValueType::MATRIX_COMPLEX;
    }


    Variant::Variant(const Model::Variant& value) {
        copy(value.impl);
    }


    Variant::Variant(const void* pointer, ValueType valueType) {
        currentValueType = valueType;

        switch (valueType) {
            case ValueType::NONE: {
                break;
            }

            case ValueType::VARIANT: {
                copy(*reinterpret_cast<const Variant*>(pointer));
                break;
            }

            case ValueType::BOOLEAN: {
                data.boolean = *reinterpret_cast<const Boolean*>(pointer);
                break;
            }

            case ValueType::INTEGER: {
                data.integer = *reinterpret_cast<const Integer*>(pointer);
                break;
            }

            case ValueType::REAL: {
                data.real = *reinterpret_cast<const Real*>(pointer);
                break;
            }

            case ValueType::COMPLEX: {
                data.complex = *reinterpret_cast<const Complex*>(pointer);
                break;
            }

            case ValueType::SET: {
                (void) new(&data.set) Set(*reinterpret_cast<const Set*>(pointer));
                break;
            }

            case ValueType::TUPLE: {
                (void) new(&data.tuple) Tuple(*reinterpret_cast<const Tuple*>(pointer));
                break;
            }

            case ValueType::MATRIX_BOOLEAN: {
                const MatrixBoolean* matrix = reinterpret_cast<const MatrixBoolean*>(pointer);
                (void) new(&data.matrixBoolean) Model::MatrixBoolean(new Model::MatrixBoolean::Private(*matrix));
                break;
            }

            case ValueType::MATRIX_INTEGER: {
                const MatrixInteger* matrix = reinterpret_cast<const MatrixInteger*>(pointer);
                (void) new(&data.matrixInteger) Model::MatrixInteger(new Model::MatrixInteger::Private(*matrix));
                break;
            }

            case ValueType::MATRIX_REAL: {
                const MatrixReal* matrix = reinterpret_cast<const MatrixReal*>(pointer);
                (void) new(&data.matrixReal) Model::MatrixReal(new Model::MatrixReal::Private(*matrix));
                break;
            }

            case ValueType::MATRIX_COMPLEX: {
                const MatrixComplex* matrix = reinterpret_cast<const MatrixComplex*>(pointer);
                (void) new(&data.matrixComplex) Model::MatrixComplex(new Model::MatrixComplex::Private(*matrix));
                break;
            }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                break;
            }

            default: {
                assert(false);
                break;
            }
        }
    }



    Variant::Variant(const Variant& other) {
        copy(other);
    }


    Variant::~Variant() {
        releaseMemory();
    }


    ValueType Variant::valueType() const {
        return currentValueType;
    }


    bool Variant::canTranslateTo(ValueType desiredType) const {
        bool result = false;

        switch (currentValueType) {
            case ValueType::NONE: {
                result = true;
                break;
            }

            case ValueType::BOOLEAN: {
                switch (desiredType) {
                    case ValueType::NONE:           { result = false;  break; }
                    case ValueType::BOOLEAN:        { result = true;   break; }
                    case ValueType::INTEGER:        { result = true;   break; }
                    case ValueType::REAL:           { result = true;   break; }
                    case ValueType::COMPLEX:        { result = true;   break; }
                    case ValueType::SET:            { result = false;  break; }
                    case ValueType::TUPLE:          { result = false;  break; }
                    case ValueType::MATRIX_BOOLEAN: { result = false;  break; }
                    case ValueType::MATRIX_INTEGER: { result = false;  break; }
                    case ValueType::MATRIX_REAL:    { result = false;  break; }
                    case ValueType::MATRIX_COMPLEX: { result = false;  break; }

                    case ValueType::NUMBER_TYPES: {
                        assert(false);
                        break;
                    }

                    default: {
                        assert(false);
                        break;
                    }
                }

                break;
            }

            case ValueType::INTEGER: {
                switch (desiredType) {
                    case ValueType::NONE:           { result = false;  break; }
                    case ValueType::BOOLEAN:        { result = true;   break; }
                    case ValueType::INTEGER:        { result = true;   break; }
                    case ValueType::REAL:           { result = true;   break; }
                    case ValueType::COMPLEX:        { result = true;   break; }
                    case ValueType::SET:            { result = false;  break; }
                    case ValueType::TUPLE:          { result = false;  break; }
                    case ValueType::MATRIX_BOOLEAN: { result = false;  break; }
                    case ValueType::MATRIX_INTEGER: { result = false;  break; }
                    case ValueType::MATRIX_REAL:    { result = false;  break; }
                    case ValueType::MATRIX_COMPLEX: { result = false;  break; }

                    case ValueType::NUMBER_TYPES: {
                        assert(false);
                        break;
                    }

                    default: {
                        assert(false);
                        break;
                    }
                }

                break;
            }

            case ValueType::REAL: {
                switch (desiredType) {
                    case ValueType::NONE: {
                        result = false;
                        break;
                    }

                    case ValueType::BOOLEAN: {
                        result = true;
                        break;
                    }

                    case ValueType::INTEGER: {
                        Model::Real whole;
                        Model::Real fractional = std::modf(data.real, &whole);
                        result = (fractional == Model::Real(0)                            &&
                                  data.real <= std::numeric_limits<Model::Integer>::max() &&
                                  data.real >= std::numeric_limits<Model::Integer>::min()    );
                        break;
                    }

                    case ValueType::REAL:    {
                        result = true;
                        break;
                    }

                    case ValueType::COMPLEX: {
                        result = true;
                        break;
                    }

                    case ValueType::SET: {
                        result = false;
                        break;
                    }

                    case ValueType::TUPLE: {
                        result = false;
                        break;
                    }

                    case ValueType::MATRIX_BOOLEAN: {
                        result = false;
                        break;
                    }

                    case ValueType::MATRIX_INTEGER: {
                        result = false;
                        break;
                    }

                    case ValueType::MATRIX_REAL: {
                        result = false;
                        break;
                    }

                    case ValueType::MATRIX_COMPLEX: {
                        result = false;
                        break;
                    }

                    case ValueType::NUMBER_TYPES: {
                        assert(false);
                        break;
                    }

                    default: {
                        assert(false);
                        break;
                    }
                }

                break;

            }

            case ValueType::COMPLEX: {
                switch (desiredType) {
                    case ValueType::NONE: {
                        result = false;
                        break;
                    }

                    case ValueType::BOOLEAN: {
                        result = true;
                        break;
                    }

                    case ValueType::INTEGER: {
                        if (data.complex.imag() == Model::Real(0)) {
                            Model::Real whole;
                            Model::Real fractional = std::modf(data.complex.real(), &whole);
                            result = (fractional == Model::Real(0)                                      &&
                                      data.complex.real() <= std::numeric_limits<Model::Integer>::max() &&
                                      data.complex.real() >= std::numeric_limits<Model::Integer>::min()    );
                        } else {
                            result = false;
                        }

                        break;
                    }

                    case ValueType::REAL: {
                        result = (data.complex.imag() == Model::Real(0));
                        break;
                    }

                    case ValueType::COMPLEX: {
                        result = true;
                        break;
                    }

                    case ValueType::SET: {
                        result = false;
                        break;
                    }

                    case ValueType::TUPLE: {
                        result = false;
                        break;
                    }

                    case ValueType::MATRIX_BOOLEAN: {
                        result = false;
                        break;
                    }

                    case ValueType::MATRIX_INTEGER: {
                        result = false;
                        break;
                    }

                    case ValueType::MATRIX_REAL: {
                        result = false;
                        break;
                    }

                    case ValueType::MATRIX_COMPLEX: {
                        result = false;
                        break;
                    }

                    case ValueType::NUMBER_TYPES: {
                        assert(false);
                        break;
                    }

                    default: {
                        assert(false);
                        break;
                    }
                }

                break;
            }

            case ValueType::SET: {
                switch (desiredType) {
                    case ValueType::NONE:           { result = false;  break; }
                    case ValueType::BOOLEAN:        { result = true;   break; }
                    case ValueType::INTEGER:        { result = false;  break; }
                    case ValueType::REAL:           { result = false;  break; }
                    case ValueType::COMPLEX:        { result = false;  break; }
                    case ValueType::SET:            { result = true;   break; }
                    case ValueType::TUPLE:          { result = false;  break; }
                    case ValueType::MATRIX_BOOLEAN: { result = false;  break; }
                    case ValueType::MATRIX_INTEGER: { result = false;  break; }
                    case ValueType::MATRIX_REAL:    { result = false;  break; }
                    case ValueType::MATRIX_COMPLEX: { result = false;  break; }

                    case ValueType::NUMBER_TYPES: {
                        assert(false);
                        break;
                    }

                    default: {
                        assert(false);
                        break;
                    }
                }

                break;
            }

            case ValueType::TUPLE: {
                switch (desiredType) {
                    case ValueType::NONE:           { result = false;  break; }
                    case ValueType::BOOLEAN:        { result = true;   break; }
                    case ValueType::INTEGER:        { result = false;  break; }
                    case ValueType::REAL:           { result = false;  break; }
                    case ValueType::COMPLEX:        { result = false;  break; }
                    case ValueType::SET:            { result = false;  break; }
                    case ValueType::TUPLE:          { result = true;   break; }
                    case ValueType::MATRIX_BOOLEAN: { result = false;  break; }
                    case ValueType::MATRIX_INTEGER: { result = false;  break; }
                    case ValueType::MATRIX_REAL:    { result = false;  break; }
                    case ValueType::MATRIX_COMPLEX: { result = false;  break; }

                    case ValueType::NUMBER_TYPES: {
                        assert(false);
                        break;
                    }

                    default: {
                        assert(false);
                        break;
                    }
                }

                break;
            }

            case ValueType::MATRIX_BOOLEAN: {
                switch (desiredType) {
                    case ValueType::NONE:           { result = false;  break; }
                    case ValueType::BOOLEAN:        { result = false;  break; }
                    case ValueType::INTEGER:        { result = false;  break; }
                    case ValueType::REAL:           { result = false;  break; }
                    case ValueType::COMPLEX:        { result = false;  break; }
                    case ValueType::SET:            { result = false;  break; }
                    case ValueType::TUPLE:          { result = false;  break; }
                    case ValueType::MATRIX_BOOLEAN: { result = true;   break; }
                    case ValueType::MATRIX_INTEGER: { result = true;   break; }
                    case ValueType::MATRIX_REAL:    { result = true;   break; }
                    case ValueType::MATRIX_COMPLEX: { result = true;   break; }

                    case ValueType::NUMBER_TYPES: {
                        assert(false);
                        break;
                    }

                    default: {
                        assert(false);
                        break;
                    }
                }

                break;
            }

            case ValueType::MATRIX_INTEGER: {
                switch (desiredType) {
                    case ValueType::NONE:           { result = false;  break; }
                    case ValueType::BOOLEAN:        { result = false;  break; }
                    case ValueType::INTEGER:        { result = false;  break; }
                    case ValueType::REAL:           { result = false;  break; }
                    case ValueType::COMPLEX:        { result = false;  break; }
                    case ValueType::SET:            { result = false;  break; }
                    case ValueType::TUPLE:          { result = false;  break; }
                    case ValueType::MATRIX_BOOLEAN: { result = true;   break; }
                    case ValueType::MATRIX_INTEGER: { result = true;   break; }
                    case ValueType::MATRIX_REAL:    { result = true;   break; }
                    case ValueType::MATRIX_COMPLEX: { result = true;   break; }

                    case ValueType::NUMBER_TYPES: {
                        assert(false);
                        break;
                    }

                    default: {
                        assert(false);
                        break;
                    }
                }

                break;
            }

            case ValueType::MATRIX_REAL: {
                switch (desiredType) {
                    case ValueType::NONE:           { result = false;  break; }
                    case ValueType::BOOLEAN:        { result = false;  break; }
                    case ValueType::INTEGER:        { result = false;  break; }
                    case ValueType::REAL:           { result = false;  break; }
                    case ValueType::COMPLEX:        { result = false;  break; }
                    case ValueType::SET:            { result = false;  break; }
                    case ValueType::TUPLE:          { result = false;  break; }
                    case ValueType::MATRIX_BOOLEAN: { result = true;   break; }
                    case ValueType::MATRIX_INTEGER: { result = false;  break; }
                    case ValueType::MATRIX_REAL:    { result = true;   break; }
                    case ValueType::MATRIX_COMPLEX: { result = true;   break; }

                    case ValueType::NUMBER_TYPES: {
                        assert(false);
                        break;
                    }

                    default: {
                        assert(false);
                        break;
                    }
                }

                break;
            }

            case ValueType::MATRIX_COMPLEX: {
                switch (desiredType) {
                    case ValueType::NONE:           { result = false;  break; }
                    case ValueType::BOOLEAN:        { result = false;  break; }
                    case ValueType::INTEGER:        { result = false;  break; }
                    case ValueType::REAL:           { result = false;  break; }
                    case ValueType::COMPLEX:        { result = false;  break; }
                    case ValueType::SET:            { result = false;  break; }
                    case ValueType::TUPLE:          { result = false;  break; }
                    case ValueType::MATRIX_BOOLEAN: { result = true;   break; }
                    case ValueType::MATRIX_INTEGER: { result = false;  break; }
                    case ValueType::MATRIX_REAL:    { result = false;  break; }
                    case ValueType::MATRIX_COMPLEX: { result = true;   break; }

                    case ValueType::NUMBER_TYPES: {
                        assert(false);
                        break;
                    }

                    default: {
                        assert(false);
                        break;
                    }
                }

                break;
            }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        return result;
    }


    Boolean Variant::toBoolean(bool* ok) const {
        Model::Boolean result = Model::Boolean(false);
        bool           isOk   = false;

        switch (currentValueType) {
            case ValueType::NONE: {
                isOk = true;
                break;
            }

            case ValueType::BOOLEAN: {
                isOk   = true;
                result = data.boolean;
                break;
            }

            case ValueType::INTEGER: {
                isOk   = true;
                result = (data.integer != Model::Integer(0));
                break;
            }

            case ValueType::REAL: {
                isOk  = true;
                result = (data.real != Model::Real(0));
                break;
            }

            case ValueType::COMPLEX: {
                isOk   = true;
                result = (data.complex != Model::Complex(0, 0));
                break;
            }

            case ValueType::SET: {
                isOk   = true;
                result = !data.set.isEmpty();
                break;
            }

            case ValueType::TUPLE: {
                isOk   = true;
                result = !data.tuple.isEmpty();
                break;
            }

            case ValueType::MATRIX_BOOLEAN: {
                isOk   = false;
                break;
            }

            case ValueType::MATRIX_INTEGER: {
                isOk   = false;
                break;
            }

            case ValueType::MATRIX_REAL: {
                isOk   = false;
                break;
            }

            case ValueType::MATRIX_COMPLEX: {
                isOk   = false;
                break;
            }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        if (ok != nullptr) {
            *ok = isOk;
        }

        return result;
    }


    Integer Variant::toInteger(bool* ok) const {
        Model::Integer result = Model::Integer(0);
        bool           isOk   = false;

        switch (currentValueType) {
            case ValueType::NONE: {
                isOk = true;
                break;
            }

            case ValueType::BOOLEAN: {
                isOk   = true;
                result = data.boolean ? Model::Integer(1) : Model::Integer(0);
                break;
            }

            case ValueType::INTEGER: {
                isOk   = true;
                result = data.integer;

                break;
            }

            case ValueType::REAL: {
                Model::Real whole;
                Model::Real fractional = std::modf(data.real, &whole);
                isOk = (fractional == Model::Real(0)                            &&
                        data.real <= std::numeric_limits<Model::Integer>::max() &&
                        data.real >= std::numeric_limits<Model::Integer>::min()    );

                if (isOk) {
                    result = static_cast<Model::Integer>(data.real);
                }

                break;
            }

            case ValueType::COMPLEX: {
                if (data.complex.imag() == Model::Real(0)) {
                    Model::Real whole;
                    Model::Real fractional = std::modf(data.complex.real(), &whole);
                    isOk = (fractional == Model::Real(0)                                      &&
                            data.complex.real() <= std::numeric_limits<Model::Integer>::max() &&
                            data.complex.real() >= std::numeric_limits<Model::Integer>::min()    );

                    if (isOk) {
                        result = static_cast<Model::Integer>(data.complex.real());
                    }
                }

                break;
            }

            case ValueType::SET: {
                isOk = false;
                break;
            }

            case ValueType::TUPLE: {
                isOk = false;
                break;
            }

            case ValueType::MATRIX_BOOLEAN: {
                isOk   = false;
                break;
            }

            case ValueType::MATRIX_INTEGER: {
                isOk   = false;
                break;
            }

            case ValueType::MATRIX_REAL: {
                isOk   = false;
                break;
            }

            case ValueType::MATRIX_COMPLEX: {
                isOk   = false;
                break;
            }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        if (ok != nullptr) {
            *ok = isOk;
        }

        return result;
    }


    Real Variant::toReal(bool* ok) const {
        Model::Real result = Model::Real(0);
        bool        isOk   = false;

        switch (currentValueType) {
            case ValueType::NONE: {
                isOk = true;
                break;
            }

            case ValueType::BOOLEAN: {
                isOk   = true;
                result = data.boolean ? Model::Real(1) : Model::Real(0);
                break;
            }

            case ValueType::INTEGER: {
                isOk   = true;
                result = Model::Real(data.integer);

                break;
            }

            case ValueType::REAL: {
                isOk   = true;
                result = data.real;

                break;
            }

            case ValueType::COMPLEX: {
                if (data.complex.imag() == Model::Real(0)) {
                    isOk   = true;
                    result = data.complex.real();
                }

                break;
            }

            case ValueType::SET: {
                isOk = false;
                break;
            }

            case ValueType::TUPLE: {
                isOk = false;
                break;
            }

            case ValueType::MATRIX_BOOLEAN: {
                isOk   = false;
                break;
            }

            case ValueType::MATRIX_INTEGER: {
                isOk   = false;
                break;
            }

            case ValueType::MATRIX_REAL: {
                isOk   = false;
                break;
            }

            case ValueType::MATRIX_COMPLEX: {
                isOk   = false;
                break;
            }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        if (ok != nullptr) {
            *ok = isOk;
        }

        return result;
    }


    Complex Variant::toComplex(bool* ok) const {
        Model::Complex result;
        bool           isOk = false;

        switch (currentValueType) {
            case ValueType::NONE: {
                isOk = true;
                break;
            }

            case ValueType::BOOLEAN: {
                isOk   = true;
                result = data.boolean ? Model::Complex(1, 0) : Model::Complex(0, 0);
                break;
            }

            case ValueType::INTEGER: {
                isOk   = true;
                result = Model::Complex(Model::Real(data.integer));

                break;
            }

            case ValueType::REAL: {
                isOk   = true;
                result = Model::Complex(data.real);

                break;
            }

            case ValueType::COMPLEX: {
                isOk = true;
                result = data.complex;

                break;
            }

            case ValueType::SET: {
                isOk = false;
                break;
            }

            case ValueType::TUPLE: {
                isOk = false;
                break;
            }

            case ValueType::MATRIX_BOOLEAN: {
                isOk   = false;
                break;
            }

            case ValueType::MATRIX_INTEGER: {
                isOk   = false;
                break;
            }

            case ValueType::MATRIX_REAL: {
                isOk   = false;
                break;
            }

            case ValueType::MATRIX_COMPLEX: {
                isOk   = false;
                break;
            }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        if (ok != nullptr) {
            *ok = isOk;
        }

        return result;
    }


    const Set& Variant::toSet(bool* ok) const {
        const Set* result = &dummySet;
        bool       isOk   = false;

        switch (currentValueType) {
            case ValueType::NONE: {
                isOk = true;
                break;
            }

            case ValueType::BOOLEAN: {
                isOk = false;
                break;
            }

            case ValueType::INTEGER: {
                isOk = false;
                break;
            }

            case ValueType::REAL: {
                isOk = false;
                break;
            }

            case ValueType::COMPLEX: {
                isOk = false;
                break;
            }

            case ValueType::SET: {
                isOk = true;
                result = &data.set;
                break;
            }

            case ValueType::TUPLE: {
                isOk = false;
                break;
            }

            case ValueType::MATRIX_BOOLEAN: {
                isOk   = false;
                break;
            }

            case ValueType::MATRIX_INTEGER: {
                isOk   = false;
                break;
            }

            case ValueType::MATRIX_REAL: {
                isOk   = false;
                break;
            }

            case ValueType::MATRIX_COMPLEX: {
                isOk   = false;
                break;
            }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        if (ok != nullptr) {
            *ok = isOk;
        }

        return *result;
    }


    const Tuple& Variant::toTuple(bool* ok) const {
        const Tuple* result = &dummyTuple;
        bool         isOk   = false;

        switch (currentValueType) {
            case ValueType::NONE: {
                isOk = true;
                break;
            }

            case ValueType::BOOLEAN: {
                isOk = false;
                break;
            }

            case ValueType::INTEGER: {
                isOk = false;
                break;
            }

            case ValueType::REAL: {
                isOk = false;
                break;
            }

            case ValueType::COMPLEX: {
                isOk = false;
                break;
            }

            case ValueType::SET: {
                isOk = false;
                break;
            }

            case ValueType::TUPLE: {
                isOk = true;
                result = &data.tuple;
                break;
            }

            case ValueType::MATRIX_BOOLEAN: {
                isOk   = false;
                break;
            }

            case ValueType::MATRIX_INTEGER: {
                isOk   = false;
                break;
            }

            case ValueType::MATRIX_REAL: {
                isOk   = false;
                break;
            }

            case ValueType::MATRIX_COMPLEX: {
                isOk   = false;
                break;
            }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        if (ok != nullptr) {
            *ok = isOk;
        }

        return *result;
    }


    MatrixBoolean Variant::toMatrixBoolean(bool* ok) const {
        MatrixBoolean result;
        bool          isOk   = false;

        switch (currentValueType) {
            case ValueType::NONE: {
                isOk = true;
                break;
            }

            case ValueType::BOOLEAN: {
                isOk = false;
                break;
            }

            case ValueType::INTEGER: {
                isOk = false;
                break;
            }

            case ValueType::REAL: {
                isOk = false;
                break;
            }

            case ValueType::COMPLEX: {
                isOk = false;
                break;
            }

            case ValueType::SET: {
                isOk = false;
                break;
            }

            case ValueType::TUPLE: {
                isOk = false;
                break;
            }

            case ValueType::MATRIX_BOOLEAN: {
                isOk   = true;
                result = *data.matrixBoolean.impl;
                break;
            }

            case ValueType::MATRIX_INTEGER: {
                isOk   = true;
                result = MatrixBoolean(*data.matrixInteger.impl);
                break;
            }

            case ValueType::MATRIX_REAL: {
                isOk   = true;
                result = MatrixBoolean(*data.matrixReal.impl);
                break;
            }

            case ValueType::MATRIX_COMPLEX: {
                isOk   = true;
                result = MatrixBoolean(*data.matrixComplex.impl);
                break;
            }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        if (ok != nullptr) {
            *ok = isOk;
        }

        return result;
    }


    MatrixInteger Variant::toMatrixInteger(bool* ok) const {
        MatrixInteger result;
        bool          isOk   = false;

        switch (currentValueType) {
            case ValueType::NONE: {
                isOk = true;
                break;
            }

            case ValueType::BOOLEAN: {
                isOk = false;
                break;
            }

            case ValueType::INTEGER: {
                isOk = false;
                break;
            }

            case ValueType::REAL: {
                isOk = false;
                break;
            }

            case ValueType::COMPLEX: {
                isOk = false;
                break;
            }

            case ValueType::SET: {
                isOk = false;
                break;
            }

            case ValueType::TUPLE: {
                isOk = false;
                break;
            }

            case ValueType::MATRIX_BOOLEAN: {
                isOk   = true;
                result = MatrixInteger(*data.matrixBoolean.impl);
                break;
            }

            case ValueType::MATRIX_INTEGER: {
                isOk   = true;
                result = *data.matrixInteger.impl;
                break;
            }

            case ValueType::MATRIX_REAL: {
                isOk = false;
                break;
            }

            case ValueType::MATRIX_COMPLEX: {
                isOk = false;
                break;
            }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        if (ok != nullptr) {
            *ok = isOk;
        }

        return result;
    }


    MatrixReal Variant::toMatrixReal(bool* ok) const {
        MatrixReal result;
        bool       isOk   = false;

        switch (currentValueType) {
            case ValueType::NONE: {
                isOk = true;
                break;
            }

            case ValueType::BOOLEAN: {
                isOk = false;
                break;
            }

            case ValueType::INTEGER: {
                isOk = false;
                break;
            }

            case ValueType::REAL: {
                isOk = false;
                break;
            }

            case ValueType::COMPLEX: {
                isOk = false;
                break;
            }

            case ValueType::SET: {
                isOk = false;
                break;
            }

            case ValueType::TUPLE: {
                isOk = false;
                break;
            }

            case ValueType::MATRIX_BOOLEAN: {
                isOk   = true;
                result = MatrixReal(*data.matrixBoolean.impl);
                break;
            }

            case ValueType::MATRIX_INTEGER: {
                isOk   = true;
                result = MatrixReal(*data.matrixInteger.impl);
                break;
            }

            case ValueType::MATRIX_REAL: {
                isOk   = true;
                result = *data.matrixReal.impl;
                break;
            }

            case ValueType::MATRIX_COMPLEX: {
                isOk = false;
                break;
            }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        if (ok != nullptr) {
            *ok = isOk;
        }

        return result;
    }


    MatrixComplex Variant::toMatrixComplex(bool* ok) const {
        MatrixComplex result;
        bool          isOk   = false;

        switch (currentValueType) {
            case ValueType::NONE: {
                isOk = true;
                break;
            }

            case ValueType::BOOLEAN: {
                isOk = false;
                break;
            }

            case ValueType::INTEGER: {
                isOk = false;
                break;
            }

            case ValueType::REAL: {
                isOk = false;
                break;
            }

            case ValueType::COMPLEX: {
                isOk = false;
                break;
            }

            case ValueType::SET: {
                isOk = false;
                break;
            }

            case ValueType::TUPLE: {
                isOk = false;
                break;
            }

            case ValueType::MATRIX_BOOLEAN: {
                isOk   = true;
                result = MatrixComplex(*data.matrixBoolean.impl);
                break;
            }

            case ValueType::MATRIX_INTEGER: {
                isOk   = true;
                result = MatrixComplex(*data.matrixInteger.impl);
                break;
            }

            case ValueType::MATRIX_REAL: {
                isOk   = true;
                result = MatrixComplex(*data.matrixReal.impl);
                break;
            }

            case ValueType::MATRIX_COMPLEX: {
                isOk   = true;
                result = *data.matrixComplex.impl;
                break;
            }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        if (ok != nullptr) {
            *ok = isOk;
        }

        return result;
    }


    bool Variant::convert(void* pointer, ValueType valueType) const {
        bool success = false;

        switch (valueType) {
            case ValueType::NONE: {
                success = canTranslateTo(valueType);
                break;
            }

            case ValueType::BOOLEAN: {
                *reinterpret_cast<Boolean*>(pointer) = toBoolean(&success);
                break;
            }

            case ValueType::INTEGER: {
                *reinterpret_cast<Integer*>(pointer) = toInteger(&success);
                break;
            }

            case ValueType::REAL: {
                *reinterpret_cast<Real*>(pointer) = toReal(&success);
                break;
            }

            case ValueType::COMPLEX: {
                *reinterpret_cast<Complex*>(pointer) = toComplex(&success);
                break;
            }

            case ValueType::SET: {
                *reinterpret_cast<Set*>(pointer) = toSet(&success);
                break;
            }

            case ValueType::TUPLE: {
                *reinterpret_cast<Tuple*>(pointer) = toTuple(&success);
                break;
            }

            case ValueType::MATRIX_BOOLEAN: {
                *reinterpret_cast<MatrixBoolean*>(pointer) = toMatrixBoolean(&success);
                break;
            }

            case ValueType::MATRIX_INTEGER: {
                *reinterpret_cast<MatrixInteger*>(pointer) = toMatrixInteger(&success);
                break;
            }

            case ValueType::MATRIX_REAL: {
                *reinterpret_cast<MatrixReal*>(pointer) = toMatrixReal(&success);
                break;
            }

            case ValueType::MATRIX_COMPLEX: {
                *reinterpret_cast<MatrixComplex*>(pointer) = toMatrixComplex(&success);
                break;
            }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        return success;
    }


    ValueType Variant::bestUpcast(ValueType type1, ValueType type2) {
        return bestCompareTypeArray[static_cast<unsigned>(type1)][static_cast<unsigned>(type2)];
    }


    ValueType Variant::bestUpcast(ValueType type1, const Variant& v2) {
        return bestUpcast(type1, v2.currentValueType);
    }


    ValueType Variant::bestUpcast(const Variant& v1, ValueType type2) {
        return bestUpcast(v1.currentValueType, type2);
    }


    ValueType Variant::bestUpcast(const Variant& v1, const Variant& v2) {
        return bestUpcast(v1.currentValueType, v2.currentValueType);
    }


    void Variant::update(Model::Matrix::Index row, Model::Matrix::Index column, Variant newValue) {
        switch (currentValueType) {
            case ValueType::NONE:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                throw Model::TypeDoesNotSupportSubscripts(currentValueType);
            }

            case ValueType::MATRIX_BOOLEAN: {
                bool ok;
                data.matrixBoolean.update(row, column, newValue.toBoolean(&ok));

                if (!ok) {
                    throw Model::InvalidRuntimeConversion(newValue.valueType(), ValueType::BOOLEAN);
                }

                break;
            }

            case ValueType::MATRIX_INTEGER: {
                bool ok;
                data.matrixInteger.update(row, column, newValue.toInteger(&ok));

                if (!ok) {
                    throw Model::InvalidRuntimeConversion(newValue.valueType(), ValueType::INTEGER);
                }

                break;
            }

            case ValueType::MATRIX_REAL: {
                bool ok;
                data.matrixReal.update(row, column, newValue.toReal(&ok));

                if (!ok) {
                    throw Model::InvalidRuntimeConversion(newValue.valueType(), ValueType::REAL);
                }

                break;
            }

            case ValueType::MATRIX_COMPLEX: {
                bool ok;
                data.matrixComplex.update(row, column, newValue.toComplex(&ok));

                if (!ok) {
                    throw Model::InvalidRuntimeConversion(newValue.valueType(), ValueType::COMPLEX);
                }

                break;
            }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                break;
            }

            default: {
                assert(false);
                break;
            }
        }
    }


    void Variant::update(Model::Matrix::Index index, Variant newValue) {
        switch (currentValueType) {
            case ValueType::NONE:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET: {
                throw Model::TypeDoesNotSupportSubscripts(currentValueType);
            }

            case ValueType::TUPLE: {
                data.tuple.update(index, newValue);
                break;
            }

            case ValueType::MATRIX_BOOLEAN: {
                bool ok;
                data.matrixBoolean.update(index, newValue.toBoolean(&ok));

                if (!ok) {
                    throw Model::InvalidRuntimeConversion(newValue.valueType(), ValueType::BOOLEAN);
                }

                break;
            }

            case ValueType::MATRIX_INTEGER: {
                bool ok;
                data.matrixInteger.update(index, newValue.toInteger(&ok));

                if (!ok) {
                    throw Model::InvalidRuntimeConversion(newValue.valueType(), ValueType::INTEGER);
                }

                break;
            }

            case ValueType::MATRIX_REAL: {
                bool ok;
                data.matrixReal.update(index, newValue.toReal(&ok));

                if (!ok) {
                    throw Model::InvalidRuntimeConversion(newValue.valueType(), ValueType::REAL);
                }

                break;
            }

            case ValueType::MATRIX_COMPLEX: {
                bool ok;
                data.matrixComplex.update(index, newValue.toComplex(&ok));

                if (!ok) {
                    throw Model::InvalidRuntimeConversion(newValue.valueType(), ValueType::COMPLEX);
                }

                break;
            }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                break;
            }

            default: {
                assert(false);
                break;
            }
        }
    }


    bool Variant::operator==(const Variant& other) const {
        bool result = false;

        switch (bestUpcast(currentValueType, other.currentValueType)) {
            case ValueType::NONE: {
                result = (currentValueType == ValueType::NONE && other.currentValueType == ValueType::NONE);
                break;
            }

            case ValueType::BOOLEAN: {
                Model::Boolean v1 = toBoolean();
                Model::Boolean v2 = other.toBoolean();

                result = (v1 == v2);
                break;
            }

            case ValueType::INTEGER: {
                Model::Integer v1 = toInteger();
                Model::Integer v2 = other.toInteger();

                result = (v1 == v2);
                break;
            }

            case ValueType::REAL: {
                Model::Real v1 = toReal();
                Model::Real v2 = other.toReal();

                result = (v1 == v2);
                break;
            }

            case ValueType::COMPLEX: {
                Model::Complex v1 = toComplex();
                Model::Complex v2 = other.toComplex();

                result = (v1 == v2);
                break;
            }

            case ValueType::SET: {
                M::Set v1 = toSet();
                M::Set v2 = other.toSet();

                result = (v1 == v2);
                break;
            }

            case ValueType::TUPLE: {
                M::Tuple v1 = toTuple();
                M::Tuple v2 = other.toTuple();

                result = (v1 == v2);
                break;
            }

            case ValueType::MATRIX_BOOLEAN: {
                if (currentValueType == other.currentValueType) {
                    M::MatrixBoolean v1 = toMatrixBoolean();
                    M::MatrixBoolean v2 = other.toMatrixBoolean();

                    result = (v1 == v2);
                } else {
                    result = false;
                }

                break;
            }

            case ValueType::MATRIX_INTEGER: {
                if (currentValueType == other.currentValueType) {
                    M::MatrixInteger v1 = toMatrixInteger();
                    M::MatrixInteger v2 = other.toMatrixInteger();

                    result = (v1 == v2);
                } else {
                    result = false;
                }

                break;
            }

            case ValueType::MATRIX_REAL: {
                if (currentValueType == other.currentValueType) {
                    M::MatrixReal v1 = toMatrixReal();
                    M::MatrixReal v2 = other.toMatrixReal();

                    result = (v1 == v2);
                } else {
                    result = false;
                }

                break;
            }

            case ValueType::MATRIX_COMPLEX: {
                if (currentValueType == other.currentValueType) {
                    M::MatrixComplex v1 = toMatrixComplex();
                    M::MatrixComplex v2 = other.toMatrixComplex();

                    result = (v1 == v2);
                } else {
                    result = false;
                }

                break;
            }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        return result;
    }


    bool Variant::operator!=(const Variant& other) const {
        return !operator==(other);
    }


    bool Variant::operator<(const Variant& other) const {
        bool result = false;

        switch (bestUpcast(currentValueType, other.currentValueType)) {
            case ValueType::NONE: {
                result = static_cast<unsigned>(currentValueType) < static_cast<unsigned>(other.currentValueType);
                break;
            }

            case ValueType::BOOLEAN: {
                Model::Boolean v1 = toBoolean();
                Model::Boolean v2 = other.toBoolean();

                result = (v1 ? 1 : 0) < (v2 ? 1 : 0);
                break;
            }

            case ValueType::INTEGER: {
                Model::Integer v1 = toInteger();
                Model::Integer v2 = other.toInteger();

                result = (v1 < v2);
                break;
            }

            case ValueType::REAL: {
                Model::Real v1 = toReal();
                Model::Real v2 = other.toReal();

                result = (v1 < v2);
                break;
            }

            case ValueType::COMPLEX: {
                Model::Complex v1 = toComplex();
                Model::Complex v2 = other.toComplex();

                if (v1.imag() == 0 && v2.imag() == 0) {
                    result = v1.real() < v2.real();
                } else {
                    internalTriggerInvalidParameterValueError();
                    result = false;
                }

                break;
            }

            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerInvalidParameterValueError();
                result = false;
                break;
            }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        return result;
    }


    bool Variant::operator>(const Variant& other) const {
        bool result = false;

        switch (bestUpcast(currentValueType, other.currentValueType)) {
            case ValueType::NONE: {
                result = static_cast<unsigned>(currentValueType) > static_cast<unsigned>(other.currentValueType);
                break;
            }

            case ValueType::BOOLEAN: {
                Model::Boolean v1 = toBoolean();
                Model::Boolean v2 = other.toBoolean();

                result = (static_cast<int>(v1) > static_cast<int>(v2));
                break;
            }

            case ValueType::INTEGER: {
                Model::Integer v1 = toInteger();
                Model::Integer v2 = other.toInteger();

                result = (v1 > v2);
                break;
            }

            case ValueType::REAL: {
                Model::Real v1 = toReal();
                Model::Real v2 = other.toReal();

                result = (v1 > v2);
                break;
            }

            case ValueType::COMPLEX: {
                Model::Complex v1 = toComplex();
                Model::Complex v2 = other.toComplex();

                if (v1.imag() == 0 && v2.imag() == 0) {
                    result = v1.real() > v2.real();
                } else {
                    internalTriggerInvalidParameterValueError();
                    result = false;
                }

                break;
            }

            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerInvalidParameterValueError();
                result = false;
                break;
            }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        return result;
    }


    bool Variant::operator<=(const Variant& other) const {
        return !operator>(other);
    }


    bool Variant::operator>=(const Variant& other) const {
        return !operator<(other);
    }


    Variant& Variant::operator=(Model::Boolean value) {
        releaseMemory();

        data.boolean     = value;
        currentValueType = ValueType::BOOLEAN;

        return *this;
    }


    Variant& Variant::operator=(Model::Integer value) {
        releaseMemory();

        data.integer     = value;
        currentValueType = ValueType::INTEGER;

        return *this;
    }


    Variant& Variant::operator=(Model::Real value) {
        releaseMemory();

        data.real = value;
        currentValueType = ValueType::REAL;

        return *this;
    }


    Variant& Variant::operator=(Model::Complex value) {
        releaseMemory();

        data.complex     = value;
        currentValueType = ValueType::COMPLEX;

        return *this;
    }


    Variant& Variant::operator=(const Set& value) {
        releaseMemory();

        (void) new(&data.set) Set(value);
        currentValueType = ValueType::SET;

        return *this;
    }


    Variant& Variant::operator=(const Tuple& value) {
        releaseMemory();

        (void) new(&data.tuple) Tuple(value);
        currentValueType = ValueType::TUPLE;

        return *this;
    }


    Variant& Variant::operator=(const MatrixBoolean& value) {
        releaseMemory();

        (void) new(&data.matrixBoolean) Model::MatrixBoolean(new Model::MatrixBoolean::Private(value));
        currentValueType = ValueType::MATRIX_BOOLEAN;

        return *this;
    }


    Variant& Variant::operator=(const MatrixInteger& value) {
        releaseMemory();

        (void) new(&data.matrixInteger) Model::MatrixInteger(new Model::MatrixInteger::Private(value));
        currentValueType = ValueType::MATRIX_INTEGER;

        return *this;
    }


    Variant& Variant::operator=(const MatrixReal& value) {
        releaseMemory();

        (void) new(&data.matrixReal) Model::MatrixReal(new Model::MatrixReal::Private(value));
        currentValueType = ValueType::MATRIX_REAL;

        return *this;
    }


    Variant& Variant::operator=(const MatrixComplex& value) {
        releaseMemory();

        (void) new(&data.matrixComplex) Model::MatrixComplex(new Model::MatrixComplex::Private(value));
        currentValueType = ValueType::MATRIX_COMPLEX;

        return *this;
    }


    Variant& Variant::operator=(const Variant& other) {
        releaseMemory();
        copy(other);

        return *this;
    }


    void Variant::releaseMemory() {
        switch (currentValueType) {
            case ValueType::NONE:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                // No cleanup needed for POD and invalid types.
                break;
            }

            case ValueType::SET: {
                data.set.~Set();
                break;
            }

            case ValueType::TUPLE: {
                data.tuple.~Tuple();
                break;
            }

            case ValueType::MATRIX_BOOLEAN: {
                data.matrixBoolean.~MatrixBoolean();
                break;
            }

            case ValueType::MATRIX_INTEGER: {
                data.matrixInteger.~MatrixInteger();
                break;
            }

            case ValueType::MATRIX_REAL: {
                data.matrixReal.~MatrixReal();
                break;
            }

            case ValueType::MATRIX_COMPLEX: {
                data.matrixComplex.~MatrixComplex();
                break;
            }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                break;
            }

            default: {
                assert(false);
                break;
            }
        }
    }


    void Variant::copy(const Variant& other) {
        switch (other.currentValueType) {
            case ValueType::NONE: {
                std::memset(reinterpret_cast<UI8*>(this), 0, sizeof(Data));
                break;
            }

            case ValueType::BOOLEAN: {
                data.boolean = other.data.boolean;
                break;
            }

            case ValueType::INTEGER: {
                data.integer = other.data.integer;
                break;
            }

            case ValueType::REAL: {
                data.real = other.data.real;
                break;
            }

            case ValueType::COMPLEX: {
                data.complex = other.data.complex;
                break;
            }

            case ValueType::SET: {
                (void) new(&data.set) Set(other.data.set);
                break;
            }

            case ValueType::TUPLE: {
                (void) new(&data.tuple) Tuple(other.data.tuple);
                break;
            }

            case ValueType::MATRIX_BOOLEAN: {
                (void) new(&data.matrixBoolean) Model::MatrixBoolean(other.data.matrixBoolean);
                break;
            }

            case ValueType::MATRIX_INTEGER: {
                (void) new(&data.matrixInteger) Model::MatrixInteger(other.data.matrixInteger);
                break;
            }

            case ValueType::MATRIX_REAL: {
                (void) new(&data.matrixReal) Model::MatrixReal(other.data.matrixReal);
                break;
            }

            case ValueType::MATRIX_COMPLEX: {
                (void) new(&data.matrixComplex) Model::MatrixComplex(other.data.matrixComplex);
                break;
            }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        currentValueType = other.currentValueType;
    }
}
