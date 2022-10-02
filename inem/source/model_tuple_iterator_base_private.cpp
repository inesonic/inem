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
* This file implements the \ref Model::TupleIteratorBase::Private class.
***********************************************************************************************************************/

#include <cmath>
#include <limits>

#include "model_intrinsic_types.h"
#include "model_variant.h"
#include "model_variant_private.h"
#include "model_tuple.h"
#include "model_tuple_private.h"
#include "model_tuple_iterator_base.h"
#include "model_tuple_iterator_base_private.h"

namespace Model {
    TupleIteratorBase::Private::Private() {
        currentTuple = nullptr;
        currentIndex = 0;
    }


    TupleIteratorBase::Private::Private(
            const Tuple*       rangeImplementation,
            unsigned long long index
        ):currentTuple(
            rangeImplementation
        ),currentIndex(
            index
        ) {
        if (index < static_cast<unsigned long long>(rangeImplementation->size())) {
            updateCurrentValue();
        } else {
            currentValue = Variant();
        }
    }


    TupleIteratorBase::Private::Private(
            const Private& other
        ):currentTuple(
            other.currentTuple
        ),currentIndex(
            other.currentIndex
        ),currentValue(
            other.currentValue
        ) {}


    TupleIteratorBase::Private::~Private() {}


    bool TupleIteratorBase::Private::isValid() const {
        return currentTuple != nullptr && static_cast<unsigned long long>(currentTuple->impl->size()) > currentIndex;
    }


    void TupleIteratorBase::Private::advance() {
        unsigned long long currentSize = static_cast<unsigned long long>(currentTuple->impl->size());
        if (currentIndex < currentSize) {
            ++currentIndex;

            if (currentIndex < currentSize) {
                updateCurrentValue();
            } else {
                currentValue = Model::Variant();
            }
        }
    }


    void TupleIteratorBase::Private::advance(signed long long distance) {
        unsigned long long currentSize = static_cast<unsigned long long>(currentTuple->impl->size());
        if (distance >= 0) {
            if (currentIndex + distance < currentSize) {
                currentIndex += distance;
            } else {
                currentIndex = currentSize;
            }
        } else {
            if (currentIndex >= static_cast<unsigned long long>(-distance)) {
                currentIndex += distance;
            } else {
                currentIndex = currentSize;
            }
        }

        if (currentIndex < currentSize) {
            updateCurrentValue();
        } else {
            currentValue = Model::Variant();
        }
    }


    void TupleIteratorBase::Private::backup() {
        unsigned long long currentSize = static_cast<unsigned long long>(currentTuple->impl->size());

        if (currentIndex > 0) {
            --currentIndex;
        } else {
            currentIndex = currentSize;
        }

        if (currentIndex < currentSize) {
            updateCurrentValue();
        } else {
            currentValue = Model::Variant();
        }
    }


    void TupleIteratorBase::Private::backup(signed long long distance) {
        advance(-distance);
    }


    signed long long TupleIteratorBase::Private::distance(const TupleIteratorBase::Private& other) const {
        return currentIndex - other.currentIndex;
    }


    void TupleIteratorBase::Private::setValue(const Variant& newValue) {
        const_cast<Tuple*>(currentTuple)->update(currentIndex + 1, newValue);
    }


    const Variant& TupleIteratorBase::Private::publicValue() const {
        return currentValue;
    }


    const Variant* TupleIteratorBase::Private::publicPointer() const {
        return &currentValue;
    }


    bool TupleIteratorBase::Private::operator==(const TupleIteratorBase::Private& other) const {
        return currentIndex == other.currentIndex;
    }


    bool TupleIteratorBase::Private::operator!=(const TupleIteratorBase::Private& other) const {
        return currentIndex != other.currentIndex;
    }


    void TupleIteratorBase::Private::updateCurrentValue() {
        currentValue = Model::Variant(new Model::Variant::Private(currentTuple->impl->at(currentIndex)));
    }
}
