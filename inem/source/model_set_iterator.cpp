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
* This file implements the \ref Model::SetIterator class.
***********************************************************************************************************************/

#include <cmath>
#include <limits>

#include "model_intrinsic_types.h"
#include "model_variant.h"
#include "model_variant_private.h"
#include "model_set.h"
#include "model_set_iterator.h"
#include "model_set_iterator_private.h"

namespace Model {
    SetIterator::SetIterator():impl(new Private) {}


    SetIterator::SetIterator(const SetIterator& other):impl(new Private(*other.impl)) {}


    SetIterator::SetIterator(SetIterator&& other) {
        impl       = other.impl;
        other.impl = nullptr;
    }


    SetIterator::~SetIterator() {
        delete impl;
    }


    Variant SetIterator::value() const {
        return impl->publicValue();
    }


    const Variant& SetIterator::constReference() const {
        return impl->publicValue();
    }


    const Variant* SetIterator::constPointer() const {
        return impl->publicPointer();
    }


    void SetIterator::advance() {
        impl->advance();
    }


    void SetIterator::advance(unsigned long distance) {
        impl->advance(distance);
    }


    bool SetIterator::isValid() const {
        return impl->isValid();
    }


    bool SetIterator::isInvalid() const {
        return impl->isInvalid();
    }


    Variant SetIterator::operator*() const {
        return impl->publicValue();
    }


    const Variant* SetIterator::operator->() const {
        return impl->publicPointer();
    }


    SetIterator& SetIterator::operator++() {
        advance();
        return *this;
    }


    SetIterator SetIterator::operator++(int) {
        SetIterator result = *this;
        advance();

        return result;
    }


    SetIterator& SetIterator::operator+=(unsigned long distance) {
        advance(distance);
        return *this;
    }


    SetIterator& SetIterator::operator=(const SetIterator& other) {
        Private* oldImplementation = impl;
        impl = new Private(*other.impl);

        delete oldImplementation;

        return *this;
    }


    SetIterator& SetIterator::operator=(SetIterator&& other) {
        impl = other.impl;
        other.impl = nullptr;

        return *this;
    }


    bool SetIterator::operator==(const SetIterator& other) const {
        return *impl == *other.impl;
    }


    bool SetIterator::operator!=(const SetIterator& other) const {
        return *impl != *other.impl;
    }
}
