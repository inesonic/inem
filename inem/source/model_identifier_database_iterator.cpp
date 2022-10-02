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
* This file implements the \ref Model::IdentifierDatabaseIterator class.
***********************************************************************************************************************/

#include "model_variant.h"
#include "model_identifier_data.h"
#include "model_identifier_database.h"
#include "model_identifier_database_iterator.h"
#include "model_identifier_database_iterator_private.h"

namespace Model {
    IdentifierDatabaseIterator::IdentifierDatabaseIterator():impl(new Private) {}


    IdentifierDatabaseIterator::IdentifierDatabaseIterator(
            const IdentifierDatabaseIterator& other
        ):impl(
            new Private(*other.impl)
        ) {}


    IdentifierDatabaseIterator::IdentifierDatabaseIterator(IdentifierDatabaseIterator&& other) {
        impl = other.impl;
        other.impl = nullptr;
    }


    IdentifierDatabaseIterator::~IdentifierDatabaseIterator() {
        delete impl;
    }


    IdentifierData IdentifierDatabaseIterator::value() const {
        return (*impl)->second;
    }


    const IdentifierData& IdentifierDatabaseIterator::constReference() const {
        return (*impl)->second;
    }


    const IdentifierData* IdentifierDatabaseIterator::constPointer() const {
        return &(*impl)->second;
    }


    IdentifierHandle IdentifierDatabaseIterator::identifierHandle() const {
        return constReference().identifierHandle();
    }


    const char* IdentifierDatabaseIterator::text1() const {
        return constReference().text1();
    }


    const char* IdentifierDatabaseIterator::text2() const {
        return constReference().text2();
    }


    bool IdentifierDatabaseIterator::isFunction() const {
        return constReference().isFunction();
    }


    bool IdentifierDatabaseIterator::isVariable() const {
        return constReference().isVariable();
    }


    ValueType IdentifierDatabaseIterator::valueType() const {
        return constReference().valueType();
    }


    void IdentifierDatabaseIterator::advance() {
        ++(*impl);
    }


    void IdentifierDatabaseIterator::advance(unsigned long distance) {
        const Private cend(impl->container(), impl->container()->cend());
        while (*impl != cend && distance != 0) {
            ++(*impl);
            --distance;
        }
    }


    bool IdentifierDatabaseIterator::isValid() const {
        bool result;

        if (impl->container() != nullptr) {
            result = (*impl != Private(impl->container(), impl->container()->cend()));
        } else {
            result = false;
        }

        return result;
    }


    bool IdentifierDatabaseIterator::isInvalid() const {
        return !isValid();
    }


    const IdentifierData& IdentifierDatabaseIterator::operator*() const {
        return constReference();
    }


    const IdentifierData* IdentifierDatabaseIterator::operator->() const {
        return constPointer();
    }


    IdentifierDatabaseIterator& IdentifierDatabaseIterator::operator++() {
        advance();
        return *this;
    }


    IdentifierDatabaseIterator IdentifierDatabaseIterator::operator++(int) {
        IdentifierDatabaseIterator result = *this;
        advance();

        return result;
    }


    IdentifierDatabaseIterator& IdentifierDatabaseIterator::operator+=(unsigned long distance) {
        advance(distance);
        return *this;
    }


    IdentifierDatabaseIterator& IdentifierDatabaseIterator::operator=(const IdentifierDatabaseIterator& other) {
        Private* oldImplementation = impl;
        impl = new Private(*other.impl);

        delete oldImplementation;

        return *this;
    }


    IdentifierDatabaseIterator& IdentifierDatabaseIterator::operator=(IdentifierDatabaseIterator&& other) {
        impl = other.impl;
        other.impl = nullptr;

        return *this;

    }


    bool IdentifierDatabaseIterator::operator==(const IdentifierDatabaseIterator& other) const {
        return *impl == *other.impl;
    }


    bool IdentifierDatabaseIterator::operator!=(const IdentifierDatabaseIterator& other) const {
        return *impl != *other.impl;
    }
}
