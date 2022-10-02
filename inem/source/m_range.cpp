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
* This file implements the \ref M::Range class.
***********************************************************************************************************************/

#include <cmath>
#include <limits>

#include "model_intrinsic_types.h"
#include "m_api_types.h"
#include "m_variant.h"
#include "model_exceptions.h"
#include "model_range.h"
#include "model_range_private.h"
#include "model_range.h"
#include "m_range.h"

/***********************************************************************************************************************
 * M::Range::Iterator
 */

namespace M {
    Range::Iterator::Iterator() {
        currentRange = nullptr;
        currentIndex = static_cast<unsigned long long>(-1);
    }


    Range::Iterator::Iterator(const Range::Iterator& other) {
        currentRange = other.currentRange;
        currentIndex = other.currentIndex;
        currentValue = other.currentValue;
    }


    Range::Iterator::~Iterator() {}


    Variant Range::Iterator::value() const {
        return currentValue;
    }


    const Variant& Range::Iterator::constReference() const {
        return currentValue;
    }


    const Variant* Range::Iterator::constPointer() const {
        return &currentValue;
    }


    void Range::Iterator::advance() {
        if (static_cast<unsigned long long>(currentRange->impl->size()) > currentIndex) {
            ++currentIndex;
        }

        updateCurrentValue();
    }


    void Range::Iterator::advance(signed long long distance) {
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


    void Range::Iterator::backup() {
        if (currentIndex > 0) {
            --currentIndex;
        } else {
            currentIndex = static_cast<unsigned long long>(currentRange->impl->size());
        }

        updateCurrentValue();
    }


    void Range::Iterator::backup(signed long long distance) {
        advance(-distance);
    }


    bool Range::Iterator::isValid() const {
        return currentRange != nullptr && static_cast<unsigned long long>(currentRange->impl->size()) > currentIndex;
    }


    bool Range::Iterator::isInvalid() const {
        return !isValid();
    }


    Variant Range::Iterator::operator*() const {
        return currentValue;
    }


    const Variant* Range::Iterator::operator->() const {
        return &currentValue;
    }


    Range::Iterator& Range::Iterator::operator++() {
        advance();
        return *this;
    }


    Range::Iterator Range::Iterator::operator++(int) {
        Range::Iterator result = *this;
        advance();

        return result;
    }


    Range::Iterator& Range::Iterator::operator--() {
        backup();
        return *this;
    }


    Range::Iterator Range::Iterator::operator--(int) {
        Range::Iterator result = *this;
        backup();

        return result;
    }


    Range::Iterator& Range::Iterator::operator+=(signed long long distance) {
        advance(distance);
        return *this;
    }


    Range::Iterator& Range::Iterator::operator-=(signed long long distance) {
        backup(distance);
        return *this;
    }


    Range::Iterator Range::Iterator::operator+(signed long long distance) const {
        Range::Iterator result = *this;
        result.advance(distance);

        return result;
    }


    Range::Iterator Range::Iterator::operator-(signed long long distance) const {
        Range::Iterator result = *this;
        result.backup(distance);

        return result;
    }


    signed long long Range::Iterator::operator-(const Range::Iterator& other) const {
        return currentIndex - other.currentIndex;
    }


    Range::Iterator& Range::Iterator::operator=(const Range::Iterator& other) {
        currentRange = other.currentRange;
        currentIndex = other.currentIndex;
        currentValue = other.currentValue;

        return *this;
    }


    bool Range::Iterator::operator==(const Range::Iterator& other) const {
        return currentIndex == other.currentIndex;
    }


    bool Range::Iterator::operator!=(const Range::Iterator& other) const {
        return currentIndex != other.currentIndex;
    }


    void Range::Iterator::updateCurrentValue() {
        currentValue = currentRange->impl->valueAtIndex(currentIndex);
    }


    Range::Iterator::Iterator(
            const Range*       newRange,
            unsigned long long newIndex,
            const Variant&     newValue
        ):currentRange(
            newRange
        ),currentIndex(
            newIndex
        ),currentValue(
            newValue
        ) {}
}

/***********************************************************************************************************************
 * M::Range
 */

namespace M {
    Range::Range() {}


    Range::Range(const Model::Range& other):Model::Range(other) {}


    Range::Range(Model::Range&& other):Model::Range(other) {}


    Range::Range(const M::Range& other):Model::Range(other) {}


    Range::Range(M::Range&& other):Model::Range(other) {}


    Range::Range(const Variant& first, const Variant& last) {
        M::ValueType             valueType = M::Variant::bestUpcast(first, last);
        Model::RangePrivateBase* pimpl     = nullptr;

        switch (valueType) {
            case M::ValueType::NONE:
            case M::ValueType::BOOLEAN:
            case M::ValueType::COMPLEX:
            case M::ValueType::SET: {
                pimpl = new Model::RangePrivateBase;

                Model::InvalidRangeParameter::RangePosition rangePosition;
                if (valueType == first.valueType()) {
                    rangePosition = Model::InvalidRangeParameter::RangePosition::FIRST;
                } else {
                    rangePosition = Model::InvalidRangeParameter::RangePosition::LAST;
                }

                throw Model::InvalidRangeParameter(rangePosition, valueType);
            }

            case M::ValueType::INTEGER: {
                pimpl = new Model::RangePrivate<Model::Integer>(first.toInteger(), last.toInteger());
                break;
            }

            case M::ValueType::REAL: {
                pimpl = new Model::RangePrivate<Model::Real>(first.toReal(), last.toReal());
                break;
            }

            case M::ValueType::NUMBER_TYPES: {
                assert(false);
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        impl = static_cast<Model::Range::Private*>(pimpl);
    }


    Range::Range(const Variant& first, const Variant& second, const Variant& last) {
        M::ValueType             valueType = M::Variant::bestUpcast(first, second, last);
        Model::RangePrivateBase* pimpl     = nullptr;

        switch (valueType) {
            case M::ValueType::NONE:
            case M::ValueType::BOOLEAN:
            case M::ValueType::COMPLEX:
            case M::ValueType::SET: {
                pimpl = new Model::RangePrivateBase;

                Model::InvalidRangeParameter::RangePosition rangePosition;
                if (valueType == first.valueType()) {
                    rangePosition = Model::InvalidRangeParameter::RangePosition::FIRST;
                } else if (valueType == second.valueType()) {
                    rangePosition = Model::InvalidRangeParameter::RangePosition::SECOND;
                } else {
                    rangePosition = Model::InvalidRangeParameter::RangePosition::LAST;
                }

                throw Model::InvalidRangeParameter(rangePosition, valueType);
            }

            case M::ValueType::INTEGER: {
                pimpl = new Model::RangePrivate<Model::Integer>(
                    first.toInteger(),
                    second.toInteger(),
                    last.toInteger()
                );

                break;
            }

            case M::ValueType::REAL: {
                pimpl = new Model::RangePrivate<Model::Real>(
                    first.toReal(),
                    second.toReal(),
                    last.toReal());
                break;
            }

            case M::ValueType::NUMBER_TYPES: {
                assert(false);
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        impl = static_cast<Range::Private*>(pimpl);
    }


    Range::~Range() {}


    const Variant& Range::first() const {
        return static_cast<Model::RangePrivateBase*>(impl)->first();
    }


    const Variant& Range::second() const {
        return static_cast<Model::RangePrivateBase*>(impl)->second();
    }


    const Variant& Range::last() const {
        return static_cast<Model::RangePrivateBase*>(impl)->last();
    }


    bool Range::contains(const Variant& value) const {
        return static_cast<Model::RangePrivateBase*>(impl)->contains(value);
    }


    Range::ConstIterator Range::constBegin() const {
        return ConstIterator(this, 0, static_cast<Model::RangePrivateBase*>(impl)->first());
    }


    Range::ConstIterator Range::constEnd() const {
        return ConstIterator(
            this,
            static_cast<unsigned long long>(static_cast<Model::RangePrivateBase*>(impl)->size()),
            Variant()
        );
    }


    Range::const_iterator Range::cbegin() const {
        return constBegin();
    }


    Range::const_iterator Range::cend() const {
        return constEnd();
    }


    Range::Iterator Range::begin() {
        return constBegin();
    }


    Range::Iterator Range::end() {
        return constEnd();
    }
}
