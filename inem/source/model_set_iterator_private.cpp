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
* This file implements the \ref Model::SetIterator::Private class.
***********************************************************************************************************************/

#include <cmath>
#include <limits>

#include "model_set_iterator.h"
#include "model_variant.h"
#include "model_variant_private.h"
#include "m_variant.h"
#include "m_variant_ordered_set.h"
#include "model_set_iterator_private.h"

namespace Model {
    SetIterator::Private::Private() {}


    SetIterator::Private::Private(const Private& other):M::Set::Iterator(other) {
        currentValue = other.currentValue;
    }


    SetIterator::Private::Private(
            const M::Set::Iterator& other
        ):M::Set::Iterator(
            other
        ),currentValue(
            other.isValid() ? new Variant::Private(other.value()) : new Variant::Private()
        ) {}


    SetIterator::Private::Private(
            const M::VariantOrderedSet::Iterator& other,
            const Model::Set*                     setInstance
        ):M::Set::Iterator(
            other,
            setInstance
        ),currentValue(
            other.isValid() ? new Variant::Private(other.value()) : new Variant::Private()
        ) {}


    SetIterator::Private::~Private() {}


    void SetIterator::Private::advance() {
        M::Set::Iterator::advance();
        updateCurrentValue();
    }


    void SetIterator::Private::advance(unsigned long long distance) {
        M::Set::Iterator::advance(distance);
        updateCurrentValue();
    }


    const Variant& SetIterator::Private::publicValue() const {
        return currentValue;
    }


    const Variant* SetIterator::Private::publicPointer() const {
        return &currentValue;
    }


    void SetIterator::Private::updateCurrentValue() {
        if (isValid()) {
            currentValue = Variant(new Variant::Private(M::Set::Iterator::value()));
        } else {
            currentValue = Variant();
        }
    }
}
