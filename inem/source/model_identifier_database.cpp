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
* This file implements the \ref Model::IdentifierDatabase class.
***********************************************************************************************************************/

#include "model_identifier_data.h"
#include "model_identifier_database_iterator.h"
#include "model_identifier_database_iterator_private.h"
#include "model_identifier_database.h"
#include "model_identifier_database_private.h"

namespace Model {
    static const IdentifierData dummyIdentifierData;

    IdentifierDatabase::IdentifierDatabase():impl(new Private) {}


    IdentifierDatabase::IdentifierDatabase(const IdentifierDatabase& other) {
        other.impl->addReference();
        impl = other.impl;
    }


    IdentifierDatabase::IdentifierDatabase(IdentifierDatabase&& other) {
        impl = other.impl;
        other.impl = nullptr;
    }


    IdentifierDatabase::~IdentifierDatabase() {
        if (impl != nullptr && impl->removeReference()) {
            delete impl;
        }
    }


    bool IdentifierDatabase::isEmpty() const {
        return impl->isEmpty();
    }


    unsigned IdentifierDatabase::size() const {
        return impl->size();
    }


    const IdentifierData& IdentifierDatabase::identifierDataByName(const char* text1, const char* text2) const {
        return impl->identifierDataByName(text1, text2);
    }


    const IdentifierData& IdentifierDatabase::identifierDataByHandle(IdentifierHandle handle) const {
        return impl->identifierDataByHandle(handle);
    }


    IdentifierDatabase::ConstIterator IdentifierDatabase::constBegin() const {
        return begin();
    }


    IdentifierDatabase::ConstIterator IdentifierDatabase::constEnd() const {
        return end();
    }


    IdentifierDatabase::const_iterator IdentifierDatabase::cbegin() const {
        return begin();
    }


    IdentifierDatabase::const_iterator IdentifierDatabase::cend() const {
        return end();
    }


    IdentifierDatabase::Iterator IdentifierDatabase::begin() const {
        return Iterator(new Iterator::Private(impl->iteratorContainer(), impl->begin()));
    }


    IdentifierDatabase::Iterator IdentifierDatabase::end() const {
        return Iterator(new Iterator::Private(impl->iteratorContainer(), impl->end()));
    }


    IdentifierDatabase& IdentifierDatabase::operator=(const IdentifierDatabase& other) {
        Private* oldImplementation = impl;

        other.impl->addReference();
        impl = other.impl;

        if (oldImplementation != nullptr && oldImplementation->removeReference()) {
            delete oldImplementation;
        }

        return *this;
    }


    IdentifierDatabase& IdentifierDatabase::operator=(IdentifierDatabase&& other) {
        impl = other.impl;
        other.impl = nullptr;

        return *this;
    }
}
