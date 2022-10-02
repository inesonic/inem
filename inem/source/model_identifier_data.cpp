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
* This file implements the \ref IdentifierData class.
***********************************************************************************************************************/

#include "model_variant_private.h"
#include "model_identifier_data.h"
#include "model_identifier_data_private.h"

namespace Model {
    IdentifierData::IdentifierData():impl(nullptr) {}


    IdentifierData::IdentifierData(const IdentifierData& other) {
        if (other.impl != nullptr) {
            other.impl->addReference();
        }

        impl = other.impl;
    }


    IdentifierData::IdentifierData(IdentifierData&& other) {
        impl = other.impl;
        other.impl = nullptr;
    }


    IdentifierData::~IdentifierData() {
        if (impl != nullptr && impl->removeReference()) {
            delete impl;
        }
    }


    bool IdentifierData::isValid() const {
        return impl != nullptr && impl->valueType() != ValueType::NONE;
    }


    bool IdentifierData::isInvalid() const {
        return !isValid();
    }


    unsigned IdentifierData::identifierHandle() const {
        return impl != nullptr ? impl->identifierHandle() : invalidIdentifierHandle;
    }


    const char* IdentifierData::text1() const {
        return impl != nullptr ? impl->text1() : nullptr;
    }


    const char* IdentifierData::text2() const {
        return impl != nullptr ? impl->text2() : nullptr;
    }


    bool IdentifierData::isFunction() const {
        return impl != nullptr ? impl->isFunction() : false;
    }


    bool IdentifierData::isVariable() const {
        return impl != nullptr ? impl->isVariable() : false;
    }


    ValueType IdentifierData::valueType() const {
        return impl != nullptr ? impl->valueType() : ValueType::NONE;
    }


    Variant IdentifierData::value() const {
        return impl != nullptr ? Variant(new Variant::Private(impl->value())) : Variant();
    }


    bool IdentifierData::setValue(const Variant& newValue) {
        bool result;

        if (impl != nullptr) {
            result = impl->setValue(*newValue.impl);
        } else {
            result = false;
        }

        return result;
    }


    const void* IdentifierData::functionAddress() const {
        return impl != nullptr ? impl->functionAddress() : nullptr;
    }


    IdentifierData& IdentifierData::operator=(const IdentifierData& other) {
        Private* oldImplementation = impl;

        if (other.impl != nullptr) {
            other.impl->addReference();
        }

        impl = other.impl;

        if (oldImplementation->removeReference()) {
            delete oldImplementation;
        }

        return *this;
    }


    IdentifierData& IdentifierData::operator=(IdentifierData&& other) {
        impl = other.impl;
        other.impl = nullptr;

        return *this;
    }
}
