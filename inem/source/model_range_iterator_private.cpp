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
* This file implements the \ref Model::RangeIterator::Private class.
***********************************************************************************************************************/

#include <cmath>
#include <limits>

#include "model_intrinsic_types.h"
#include "model_variant.h"
#include "model_variant_private.h"
#include "model_range.h"
#include "model_range_private.h"
#include "model_range_iterator.h"
#include "model_range_iterator_private.h"

namespace Model {
    RangeIterator::Private::Private() {
        currentRange = nullptr;
        currentIndex = 0;
    }


    RangeIterator::Private::Private(
            const Range*       rangeImplementation,
            unsigned long long index
        ):currentRange(
            rangeImplementation
        ),currentIndex(
            index
        ) {
        updateCurrentValue();
    }


    RangeIterator::Private::Private(
            const Private& other
        ):currentRange(
            other.currentRange
        ),currentIndex(
            other.currentIndex
        ),currentValue(
            other.currentValue
        ) {}


    RangeIterator::Private::~Private() {}


    bool RangeIterator::Private::isValid() const {
        return currentRange != nullptr && static_cast<unsigned long long>(currentRange->impl->size()) > currentIndex;
    }


    void RangeIterator::Private::advance() {
        if (static_cast<unsigned long long>(currentRange->impl->size()) > currentIndex) {
            ++currentIndex;
        }

        updateCurrentValue();
    }


    void RangeIterator::Private::advance(signed long long distance) {
        unsigned long long currentSize = static_cast<unsigned long long>(currentRange->impl->size());
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

        updateCurrentValue();
    }


    void RangeIterator::Private::backup() {
        if (currentIndex > 0) {
            --currentIndex;
        } else {
            currentIndex = static_cast<unsigned long long>(currentRange->impl->size());
        }

        updateCurrentValue();
    }


    void RangeIterator::Private::backup(signed long long distance) {
        advance(-distance);
    }


    signed long long RangeIterator::Private::distance(const RangeIterator::Private& other) const {
        return currentIndex - other.currentIndex;
    }


    const Variant& RangeIterator::Private::publicValue() const {
        return currentValue;
    }


    const Variant* RangeIterator::Private::publicPointer() const {
        return &currentValue;
    }


    bool RangeIterator::Private::operator==(const RangeIterator::Private& other) const {
        return currentIndex == other.currentIndex;
    }


    bool RangeIterator::Private::operator!=(const RangeIterator::Private& other) const {
        return currentIndex != other.currentIndex;
    }


    void RangeIterator::Private::updateCurrentValue() {
        currentValue = Model::Variant(new Model::Variant::Private(currentRange->impl->valueAtIndex(currentIndex)));
    }
}
