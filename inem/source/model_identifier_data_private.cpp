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
* This file implements the \ref Model::IdentifierData::Private class.
***********************************************************************************************************************/

#include "model_api_types.h"
#include "m_reference_counter.h"
#include "m_identifier_data.h"
#include "m_variant.h"
#include "model_identifier_data.h"
#include "model_identifier_data_private.h"

namespace Model {
    IdentifierData::Private::Private(
            IdentifierHandle identifierHandle,
            const char*      text1,
            const char*      text2,
            bool             isFunction,
            ValueType        valueType,
            void*            pointer
        ):M::IdentifierData(
            identifierHandle,
            text1,
            text2,
            isFunction,
            valueType,
            pointer
        ) {}


    IdentifierData::Private::Private(const M::IdentifierData& identifierData):M::IdentifierData(identifierData) {}


    IdentifierData::Private::Private(const M::IdentifierData* identifierData):M::IdentifierData(*identifierData) {}


    IdentifierData::Private::Private(const Private& other):M::IdentifierData(other) {}


    IdentifierData::Private::~Private() {}


    IdentifierHandle IdentifierData::Private::identifierHandle() const {
        return M::IdentifierData::identifierHandle();
    }


    const char* IdentifierData::Private::text1() const {
        return M::IdentifierData::text1();
    }


    const char* IdentifierData::Private::text2() const {
        return M::IdentifierData::text2();
    }


    bool IdentifierData::Private::isFunction() const {
        return M::IdentifierData::isFunction();
    }


    bool IdentifierData::Private::isVariable() const {
        return M::IdentifierData::isVariable();
    }


    ValueType IdentifierData::Private::valueType() const {
        return M::IdentifierData::valueType();
    }


    M::Variant IdentifierData::Private::value() const {
        return M::Variant(M::IdentifierData::pointer(), M::IdentifierData::valueType());
    }


    bool IdentifierData::Private::setValue(const M::Variant& newValue) {
        bool success;

        if (newValue.valueType() == M::IdentifierData::valueType()) {
            success = newValue.convert(M::IdentifierData::pointer(), M::IdentifierData::valueType());
        } else {
            success = false;
        }

        return success;
    }


    const void* IdentifierData::Private::functionAddress() const {
        return M::IdentifierData::isFunction() ? M::IdentifierData::pointer() : nullptr;
    }
}
