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
* This file implements the \ref Model::Range class.
***********************************************************************************************************************/

#include <cmath>
#include <limits>

#include "model_api_types.h"
#include "model_intrinsic_types.h"
#include "model_variant.h"
#include "model_variant_private.h"
#include "model_exceptions.h"
#include "model_range_iterator.h"
#include "model_range_iterator_private.h"
#include "model_range.h"
#include "model_range_private.h"

namespace Model {
    Range::Range():impl(static_cast<Private*>(new RangePrivateBase)) {}


    Range::Range(const Range& other):impl(static_cast<Private*>(other.impl->clone())) {}


    Range::Range(Range&& other) {
        impl       = other.impl;
        other.impl = nullptr;
    }


    Range::Range(const Variant& first, const Variant& last) {
        M::ValueType      valueType = Variant::bestUpcast(first, last);
        RangePrivateBase* pimpl     = nullptr;

        switch (valueType) {
            case M::ValueType::NONE:
            case M::ValueType::BOOLEAN:
            case M::ValueType::COMPLEX:
            case M::ValueType::SET: {
                pimpl = new RangePrivateBase;

                InvalidRangeParameter::RangePosition rangePosition;
                if (valueType == first.valueType()) {
                    rangePosition = InvalidRangeParameter::RangePosition::FIRST;
                } else {
                    rangePosition = InvalidRangeParameter::RangePosition::LAST;
                }

                throw InvalidRangeParameter(rangePosition, valueType);
            }

            case M::ValueType::INTEGER: {
                pimpl = new RangePrivate<Integer>(first.impl->toInteger(), last.impl->toInteger());
                break;
            }

            case M::ValueType::REAL: {
                pimpl = new RangePrivate<Real>(first.impl->toReal(), last.impl->toReal());
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


    Range::Range(const Variant& first, const Variant& second, const Variant& last) {
        M::ValueType      valueType = Model::Variant::bestUpcast(first, second, last);
        RangePrivateBase* pimpl     = nullptr;

        switch (valueType) {
            case M::ValueType::NONE:
            case M::ValueType::BOOLEAN:
            case M::ValueType::COMPLEX:
            case M::ValueType::SET: {
                pimpl = new RangePrivateBase;

                InvalidRangeParameter::RangePosition rangePosition;
                if (valueType == first.valueType()) {
                    rangePosition = InvalidRangeParameter::RangePosition::FIRST;
                } else if (valueType == second.valueType()) {
                    rangePosition = InvalidRangeParameter::RangePosition::SECOND;
                } else {
                    rangePosition = InvalidRangeParameter::RangePosition::LAST;
                }

                throw InvalidRangeParameter(rangePosition, valueType);
            }

            case M::ValueType::INTEGER: {
                pimpl = new RangePrivate<Integer>(
                    first.impl->toInteger(),
                    second.impl->toInteger(),
                    last.impl->toInteger()
                );

                break;
            }

            case M::ValueType::REAL: {
                pimpl = new RangePrivate<Real>(
                    first.impl->toReal(),
                    second.impl->toReal(),
                    last.impl->toReal()
                );

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


    Range::~Range() {
        delete impl;
    }


    bool Range::isEmpty() const {
        return static_cast<RangePrivateBase*>(impl)->isEmpty();
    }


    Model::Integer Range::size() const {
        return static_cast<RangePrivateBase*>(impl)->size();
    }


    void Range::clear() {
        delete impl;
        impl = static_cast<Private*>(new RangePrivateBase);
    }


    Variant Range::first() const {
        return Variant(new Variant::Private(static_cast<RangePrivateBase*>(impl)->first()));
    }


    Variant Range::second() const {
        return Variant(new Variant::Private(static_cast<RangePrivateBase*>(impl)->second()));
    }


    Variant Range::last() const {
        return Variant(new Variant::Private(static_cast<RangePrivateBase*>(impl)->last()));
    }


    bool Range::contains(const Variant& value) const {
        return static_cast<RangePrivateBase*>(impl)->contains(*value.impl);
    }


    Range::ConstIterator Range::constBegin() const {
        return ConstIterator(new RangeIterator::Private(this));
    }


    Range::ConstIterator Range::constEnd() const {
        return ConstIterator(new RangeIterator::Private(this, static_cast<unsigned long long>(impl->size())));
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
