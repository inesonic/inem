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

#include "model_intrinsic_types.h"
#include "model_complex.h"
#include "model_set.h"
#include "m_set.h"
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
#include "model_exceptions.h"
#include "model_set_private.h"
#include "model_variant_private.h"
#include "model_variant.h"

namespace Model {
    Variant::Variant():impl(new Private) {}


    Variant::Variant(Boolean value):impl(new Private(value)) {}


    Variant::Variant(Integer value):impl(new Private(value)) {}


    Variant::Variant(Real value):impl(new Private(value)) {}


    Variant::Variant(Complex value):impl(new Private(value)) {}


    Variant::Variant(const Set& value):impl(new Private(value)) {}


    Variant::Variant(const Tuple& value):impl(new Private(value)) {}


    Variant::Variant(const MatrixBoolean& value):impl(new Private(value)) {}


    Variant::Variant(MatrixBoolean&& value):impl(new Private(value)) {}


    Variant::Variant(const MatrixInteger& value):impl(new Private(value)) {}


    Variant::Variant(MatrixInteger&& value):impl(new Private(value)) {}


    Variant::Variant(const MatrixReal& value):impl(new Private(value)) {}


    Variant::Variant(MatrixReal&& value):impl(new Private(value)) {}


    Variant::Variant(const MatrixComplex& value):impl(new Private(value)) {}


    Variant::Variant(MatrixComplex&& value):impl(new Private(value)) {}


    Variant::Variant(const void* pointer, ValueType valueType):impl(new Private(pointer, valueType)) {}


    Variant::Variant(const Variant& other) {
        other.impl->addReference();
        impl = other.impl;
    }


    Variant::Variant(Variant&& other) {
        impl = other.impl;
        other.impl = nullptr;
    }


    Variant::~Variant() {
        if (impl != nullptr && impl->removeReference()) {
            delete impl;
        }
    }


    ValueType Variant::valueType() const {
        return impl->valueType();
    }


    bool Variant::canTranslateTo(ValueType desiredType) const {
        return impl->canTranslateTo(desiredType);
    }


    ValueType Variant::bestUpcast(ValueType type1, ValueType type2) {
        return M::Variant::bestUpcast(type1, type2);
    }


    ValueType Variant::bestUpcast(ValueType type1, const Variant& v2) {
        return M::Variant::bestUpcast(type1, v2.valueType());
    }


    ValueType Variant::bestUpcast(const Variant& v1, ValueType type2) {
        return M::Variant::bestUpcast(v1.valueType(), type2);
    }


    ValueType Variant::bestUpcast(const Variant& v1, const Variant& v2) {
        return M::Variant::bestUpcast(v1.valueType(), v2.valueType());
    }


    Boolean Variant::toBoolean(bool* ok) const {
        return impl->toBoolean(ok);
    }


    Integer Variant::toInteger(bool* ok) const {
        return impl->toInteger(ok);
    }


    Real Variant::toReal(bool* ok) const {
        return impl->toReal(ok);
    }


    Complex Variant::toComplex(bool* ok) const {
        return impl->toComplex(ok);
    }


    Set Variant::toSet(bool* ok) const {
        return impl->toSet(ok);
    }


    Tuple Variant::toTuple(bool* ok) const {
        return impl->toTuple(ok);
    }


    MatrixBoolean Variant::toMatrixBoolean(bool* ok) const {
        return MatrixBoolean(new MatrixBoolean::Private(impl->toMatrixBoolean(ok)));
    }


    MatrixInteger Variant::toMatrixInteger(bool* ok) const {
        return MatrixInteger(new MatrixInteger::Private(impl->toMatrixInteger(ok)));
    }


    MatrixReal Variant::toMatrixReal(bool* ok) const {
        return MatrixReal(new MatrixReal::Private(impl->toMatrixReal(ok)));
    }


    MatrixComplex Variant::toMatrixComplex(bool* ok) const {
        return MatrixComplex(new MatrixComplex::Private(impl->toMatrixComplex(ok)));
    }


    bool Variant::convert(void* pointer, ValueType valueType) const {
        return impl->convert(pointer, valueType);
    }


    Variant::operator Boolean() const {
        bool    ok;
        Integer result = impl->toBoolean(&ok);

        if (!ok) {
            throw InvalidRuntimeConversion(impl->valueType(), ValueType::BOOLEAN);
        }

        return result;
    }


    Variant::operator Integer() const {
        bool    ok;
        Integer result = impl->toInteger(&ok);

        if (!ok) {
            throw InvalidRuntimeConversion(impl->valueType(), ValueType::INTEGER);
        }

        return result;
    }


    Variant::operator Real() const {
        bool ok;
        Real result = impl->toReal(&ok);

        if (!ok) {
            throw InvalidRuntimeConversion(impl->valueType(), ValueType::REAL);
        }

        return result;
    }


    bool Variant::operator==(const Variant& other) const {
        return impl->operator==(*other.impl);
    }


    bool Variant::operator!=(const Variant& other) const {
        return impl->operator!=(*other.impl);
    }


    bool Variant::operator<(const Variant& other) const {
        return impl->operator<(*other.impl);
    }


    bool Variant::operator>(const Variant& other) const {
        return impl->operator>(*other.impl);
    }


    bool Variant::operator<=(const Variant& other) const {
        return impl->operator<=(*other.impl);
    }


    bool Variant::operator>=(const Variant& other) const {
        return impl->operator>=(*other.impl);
    }


    Variant& Variant::operator=(const Variant& other) {
        Private* oldImplementation = impl;

        other.impl->addReference();
        impl = other.impl;

        if (oldImplementation->removeReference()) {
            delete oldImplementation;
        }

        return *this;
    }


    Variant& Variant::operator=(Variant&& other) {
        impl = other.impl;
        other.impl = nullptr;

        return *this;
    }
}
