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
* This file implements the \ref Model::TupleIteratorBase class.
***********************************************************************************************************************/

#include <cmath>
#include <limits>

#include "model_intrinsic_types.h"
#include "model_variant.h"
#include "model_variant_private.h"
#include "model_set.h"
#include "model_tuple_iterator_base.h"
#include "model_tuple_iterator_base_private.h"

namespace Model {
    Variant TupleIteratorBase::value() const {
        return impl->publicValue();
    }


    const Variant& TupleIteratorBase::constReference() const {
        return impl->publicValue();
    }


    const Variant* TupleIteratorBase::constPointer() const {
        return impl->publicPointer();
    }


    void TupleIteratorBase::advance() {
        impl->advance();
    }


    void TupleIteratorBase::advance(signed long long distance) {
        impl->advance(distance);
    }


    void TupleIteratorBase::backup() {
        impl->backup();
    }


    void TupleIteratorBase::backup(signed long long distance) {
        impl->backup(distance);
    }


    bool TupleIteratorBase::isValid() const {
        return impl->isValid();
    }


    bool TupleIteratorBase::isInvalid() const {
        return !impl->isValid();
    }


    Variant TupleIteratorBase::operator*() const {
        return impl->publicValue();
    }


    const Variant* TupleIteratorBase::operator->() const {
        return impl->publicPointer();
    }


    signed long long TupleIteratorBase::operator-(const TupleIteratorBase& other) const {
        return impl->distance(*other.impl);
    }


    bool TupleIteratorBase::operator==(const TupleIteratorBase& other) const {
        return *impl == *other.impl;
    }


    bool TupleIteratorBase::operator!=(const TupleIteratorBase& other) const {
        return *impl != *other.impl;
    }


    TupleIteratorBase::TupleIteratorBase():impl(new Private) {}


    TupleIteratorBase::TupleIteratorBase(const TupleIteratorBase& other):impl(new Private(*other.impl)) {}


    TupleIteratorBase::TupleIteratorBase(TupleIteratorBase&& other) {
        impl = other.impl;
        other.impl = nullptr;
    }


    TupleIteratorBase::~TupleIteratorBase() {
        delete impl;
    }
}
