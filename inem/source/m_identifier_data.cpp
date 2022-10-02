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
* This file implements the \ref M::IdentifierData class.
***********************************************************************************************************************/

#include <cstdlib>
#include <cstring>

#include "m_api_types.h"
#include "m_identifier_data.h"

// Workaround for Microsoft's garbage tools.
#if (defined(_MSC_VER))

    inline char* copyString(const char* c) {
        return _strdup(c);
    }

#else

    inline char* copyString(const char* c) {
        return strdup(c);
    }

#endif

namespace M {
    IdentifierData::IdentifierData(
            const IdentifierData& other
        ):currentHandle(
            other.currentHandle
        ),currentText1(
            copyString(other.currentText1)
        ),currentText2(
            other.currentText2 != nullptr ? copyString(other.currentText2) : nullptr
        ),currentIsFunction(
            other.currentIsFunction
        ),currentValueType(
            other.currentValueType
        ),currentPointer(
            other.currentPointer
        ),deleteInDestructor(
            true
        ) {}


    IdentifierData::~IdentifierData() {
        if (deleteInDestructor) {
            std::free(const_cast<char*>(currentText1));
            std::free(const_cast<char*>(currentText2));
        }
    }


    IdentifierHandle IdentifierData::identifierHandle() const {
        return currentHandle;
    }


    const char* IdentifierData::text1() const {
        return currentText1;
    }


    const char* IdentifierData::text2() const {
        return currentText2;
    }


    bool IdentifierData::isFunction() const {
        return currentIsFunction;
    }


    bool IdentifierData::isVariable() const {
        return !currentIsFunction;
    }


    ValueType IdentifierData::valueType() const {
        return currentValueType;
    }


    void* IdentifierData::pointer() const {
        return currentPointer;
    }
}
