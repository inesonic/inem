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
* This file implements the \ref Model::IdentifierDatabase::Private class.
***********************************************************************************************************************/

#include <unordered_map>
#include <utility>
#include <string>
#include <cstring>

#include "model_identifier_data.h"
#include "model_identifier_database_iterator.h"
#include "model_identifier_database_iterator_private.h"
#include "model_identifier_database.h"
#include "model_identifier_database_private.h"

/***********************************************************************************************************************
 * Model::IdentifierDatabase::Private::VariableName
 */

namespace Model {
    IdentifierDatabase::Private::VariableName::VariableName() {}


    IdentifierDatabase::Private::VariableName::VariableName(
            const char* text1,
            const char* text2
        ):currentText1(
            text1 != nullptr ? text1 : ""
        ),currentText2(
            text2 != nullptr ? text2 : ""
        ) {}


    IdentifierDatabase::Private::VariableName::VariableName(
            const std::string& text1,
            const std::string& text2
        ):currentText1(
            text1
        ),currentText2(
            text2
        ) {}


    IdentifierDatabase::Private::VariableName::VariableName(
            const VariableName& other
        ):currentText1(
            other.currentText1
        ),currentText2(
            other.currentText2
        ) {}


    std::string IdentifierDatabase::Private::VariableName::text1() const {
        return currentText1;
    }


    std::string IdentifierDatabase::Private::VariableName::text2() const {
        return currentText2;
    }


    bool IdentifierDatabase::Private::VariableName::operator==(const VariableName& other) const {
        return currentText1 == other.currentText1 && currentText2 == other.currentText2;
    }


    bool IdentifierDatabase::Private::VariableName::operator!=(const VariableName& other) const {
        return currentText1 != other.currentText1 || currentText2 != other.currentText2;
    }


    bool IdentifierDatabase::Private::VariableName::operator<(const VariableName& other) const {
        int compared = strcmp(currentText1.data(), other.currentText2.data());
        if (compared == 0) {
            compared = strcmp(currentText2.data(), other.currentText2.data());
        }

        return compared < 0;
    }


    bool IdentifierDatabase::Private::VariableName::operator>(const VariableName& other) const {
        int compared = strcmp(currentText1.data(), other.currentText2.data());
        if (compared == 0) {
            compared = strcmp(currentText2.data(), other.currentText2.data());
        }

        return compared > 0;
    }


    bool IdentifierDatabase::Private::VariableName::operator<=(const VariableName& other) const {
        int compared = strcmp(currentText1.data(), other.currentText2.data());
        if (compared == 0) {
            compared = strcmp(currentText2.data(), other.currentText2.data());
        }

        return compared <= 0;
    }


    bool IdentifierDatabase::Private::VariableName::operator>=(const VariableName& other) const {
        int compared = strcmp(currentText1.data(), other.currentText2.data());
        if (compared == 0) {
            compared = strcmp(currentText2.data(), other.currentText2.data());
        }

        return compared >= 0;
    }


    IdentifierDatabase::Private::VariableName& IdentifierDatabase::Private::VariableName::operator=(
            const VariableName& value
        ) {
        currentText1 = value.currentText1;
        currentText2 = value.currentText2;

        return *this;
    }
}

/***********************************************************************************************************************
 * Model::IdentifierDatabase::Private::VariableNameHasher
 */

namespace Model {
    std::hash<std::string> IdentifierDatabase::Private::VariableNameHasher::stringHasher;

    std::size_t IdentifierDatabase::Private::VariableNameHasher::operator ()(
            const IdentifierDatabase::Private::VariableName& value
        )  const {
        return stringHasher(value.text1()) ^ stringHasher(value.text2());
    }
}

/***********************************************************************************************************************
 * Model::IdentifierDatabase::Private
 */

namespace Model {
    const IdentifierData IdentifierDatabase::Private::dummyIdentifierData;

    IdentifierDatabase::Private::Private() {}


    IdentifierDatabase::Private::Private(const IdentifierDatabase::Private& other) {
        currentIdentifierDataByHandle = other.currentIdentifierDataByHandle;
        currentIdentifierDataByName   = other.currentIdentifierDataByName;
    }


    IdentifierDatabase::Private::~Private() {}


    bool IdentifierDatabase::Private::isEmpty() const {
        return currentIdentifierDataByHandle.empty();
    }


    unsigned IdentifierDatabase::Private::size() const {
        return static_cast<unsigned>(currentIdentifierDataByHandle.size());
    }


    const IdentifierData& IdentifierDatabase::Private::identifierDataByName(
            const char* text1,
            const char* text2
        ) const {
        std::unordered_map<VariableName, IdentifierData, VariableNameHasher>::const_iterator
            it = currentIdentifierDataByName.find(VariableName(text1, text2));

        return it != currentIdentifierDataByName.cend() ? it->second : dummyIdentifierData;
    }


    const IdentifierData& IdentifierDatabase::Private::identifierDataByHandle(IdentifierHandle handle) const {
        std::unordered_map<IdentifierHandle, IdentifierData>::const_iterator
            it = currentIdentifierDataByHandle.find(handle);

        return it != currentIdentifierDataByHandle.end() ? it->second : dummyIdentifierData;
    }


    bool IdentifierDatabase::Private::insert(const IdentifierData& newEntry) {
        bool             result = false;
        IdentifierHandle handle = newEntry.identifierHandle();

        if (currentIdentifierDataByHandle.find(handle) == currentIdentifierDataByHandle.cend()) {
            VariableName name(newEntry.text1(), newEntry.text2());
            if (currentIdentifierDataByName.find(name) == currentIdentifierDataByName.cend()) {
                currentIdentifierDataByHandle.insert(std::pair<IdentifierHandle, IdentifierData>(handle, newEntry));
                currentIdentifierDataByName.insert(std::pair<VariableName, IdentifierData>(name, newEntry));

                result = true;
            }
        }

        return result;
    }


    IdentifierDatabase::Private::Iterator IdentifierDatabase::Private::begin() const {
        return currentIdentifierDataByHandle.cbegin();
    }


    IdentifierDatabase::Private::Iterator IdentifierDatabase::Private::end() const {
        return currentIdentifierDataByHandle.cend();
    }


    const IdentifierDatabase::Private::Container* IdentifierDatabase::Private::iteratorContainer() const {
        return &currentIdentifierDataByHandle;
    }


    IdentifierDatabase::Private& IdentifierDatabase::Private::operator=(const IdentifierDatabase::Private& other) {
        currentIdentifierDataByHandle = other.currentIdentifierDataByHandle;
        currentIdentifierDataByName   = other.currentIdentifierDataByName;

        return *this;
    }
}
