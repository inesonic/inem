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
* This file implements the \ref Model::RangeIterator class.
***********************************************************************************************************************/

#include <cmath>
#include <limits>

#include "model_intrinsic_types.h"
#include "model_variant.h"
#include "model_variant_private.h"
#include "model_set.h"
#include "model_range_iterator.h"
#include "model_range_iterator_private.h"

namespace Model {
    RangeIterator::RangeIterator():impl(new Private) {}


    RangeIterator::RangeIterator(const RangeIterator& other):impl(new Private(*other.impl)) {}


    RangeIterator::RangeIterator(RangeIterator&& other) {
        impl = other.impl;
        other.impl = nullptr;
    }


    RangeIterator::~RangeIterator() {
        delete impl;
    }


    Variant RangeIterator::value() const {
        return impl->publicValue();
    }


    const Variant& RangeIterator::constReference() const {
        return impl->publicValue();
    }


    const Variant* RangeIterator::constPointer() const {
        return impl->publicPointer();
    }


    void RangeIterator::advance() {
        impl->advance();
    }


    void RangeIterator::advance(signed long long distance) {
        impl->advance(distance);
    }


    void RangeIterator::backup() {
        impl->backup();
    }


    void RangeIterator::backup(signed long long distance) {
        impl->backup(distance);
    }


    bool RangeIterator::isValid() const {
        return impl->isValid();
    }


    bool RangeIterator::isInvalid() const {
        return !impl->isValid();
    }


    Variant RangeIterator::operator*() const {
        return impl->publicValue();
    }


    const Variant* RangeIterator::operator->() const {
        return impl->publicPointer();
    }


    RangeIterator& RangeIterator::operator++() {
        impl->advance();
        return *this;
    }


    RangeIterator RangeIterator::operator++(int) {
        RangeIterator result = *this;
        impl->advance();

        return result;
    }


    RangeIterator& RangeIterator::operator--() {
        impl->backup();
        return *this;
    }


    RangeIterator RangeIterator::operator--(int) {
        RangeIterator result = *this;
        impl->backup();

        return result;
    }


    RangeIterator& RangeIterator::operator+=(signed long long distance) {
        impl->advance(distance);
        return *this;
    }


    RangeIterator& RangeIterator::operator-=(signed long long distance) {
        impl->backup(distance);
        return *this;
    }


    RangeIterator RangeIterator::operator+(signed long long distance) const {
        RangeIterator result = *this;
        result.impl->advance(distance);

        return result;
    }


    RangeIterator RangeIterator::operator-(signed long long distance) const {
        RangeIterator result = *this;
        result.impl->backup(distance);

        return result;
    }


    signed long long RangeIterator::operator-(const RangeIterator& other) const {
        return impl->distance(*other.impl);
    }


    RangeIterator& RangeIterator::operator=(const RangeIterator& other) {
        Private* oldImplementation = impl;
        impl = new Private(*other.impl);

        delete oldImplementation;

        return *this;
    }


    RangeIterator& RangeIterator::operator=(RangeIterator&& other) {
        impl = other.impl;
        other.impl = nullptr;

        return *this;
    }


    bool RangeIterator::operator==(const RangeIterator& other) const {
        return *impl == *other.impl;
    }


    bool RangeIterator::operator!=(const RangeIterator& other) const {
        return *impl != *other.impl;
    }
}
