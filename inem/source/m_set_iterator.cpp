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
* This file implements the \ref M::SetIterator class.
***********************************************************************************************************************/

#include <cmath>
#include <limits>

#include "model_intrinsic_types.h"
#include "model_variant.h"
#include "model_set.h"
#include "m_ordered_set.h"
#include "m_variant.h"
#include "m_set_iterator.h"

namespace M {
    SetIterator::SetIterator() {
        currentSetApi = nullptr;
    }


    SetIterator::SetIterator(
            const VariantOrderedSet::Iterator& other,
            const Model::Set*                  setInstance
        ):VariantOrderedSet::Iterator(
            other
        ),currentSetApi(
            setInstance
        ) {}


    SetIterator::SetIterator(
            const SetIterator& other
        ):VariantOrderedSet::Iterator(
            other
        ),currentSetApi(
            other.currentSetApi
        ) {}


    SetIterator::~SetIterator() {}


    void SetIterator::setSetApi(const Model::Set* setInstance) {
        currentSetApi = setInstance;
    }


    const Model::Set* SetIterator::setApi() const {
        return currentSetApi;
    }


    SetIterator& SetIterator::operator=(const SetIterator& other) {
        VariantOrderedSet::Iterator::operator=(other);
        currentSetApi = other.currentSetApi;

        return *this;
    }
}
