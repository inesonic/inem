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
* This file implements the \ref Model::TupleIterator class.
***********************************************************************************************************************/

#include <cmath>
#include <limits>

#include "model_intrinsic_types.h"
#include "model_variant.h"
#include "model_variant_private.h"
#include "model_set.h"
#include "model_tuple_iterator_base.h"
#include "model_tuple_iterator_base_private.h"
#include "model_tuple_iterator.h"

namespace Model {
    TupleIterator::TupleIterator() {}


    TupleIterator::TupleIterator(const TupleIterator& other):TupleIteratorBase(other) {}


    TupleIterator::TupleIterator(TupleIterator&& other):TupleIteratorBase(other) {}


    TupleIterator::~TupleIterator() {}


    void TupleIterator::setValue(const Variant& newValue) {
        impl->setValue(newValue);
    }


    TupleIterator& TupleIterator::operator++() {
        impl->advance();
        return *this;
    }


    TupleIterator TupleIterator::operator++(int) {
        TupleIterator result = *this;
        impl->advance();

        return result;
    }


    TupleIterator& TupleIterator::operator--() {
        impl->backup();
        return *this;
    }


    TupleIterator TupleIterator::operator--(int) {
        TupleIterator result = *this;
        impl->backup();

        return result;
    }


    TupleIterator& TupleIterator::operator+=(signed long long distance) {
        impl->advance(distance);
        return *this;
    }


    TupleIterator& TupleIterator::operator-=(signed long long distance) {
        impl->backup(distance);
        return *this;
    }


    TupleIterator TupleIterator::operator+(signed long long distance) const {
        TupleIterator result = *this;
        result.impl->advance(distance);

        return result;
    }


    TupleIterator TupleIterator::operator-(signed long long distance) const {
        TupleIterator result = *this;
        result.impl->backup(distance);

        return result;
    }


    TupleIterator& TupleIterator::operator=(const TupleIterator& other) {
        Private* oldImplementation = impl;
        impl = new Private(*other.impl);

        delete oldImplementation;

        return *this;
    }


    TupleIterator& TupleIterator::operator=(TupleIterator&& other) {
        impl = other.impl;
        other.impl = nullptr;

        return *this;
    }
}
