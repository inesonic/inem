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
* This file implements the \ref M::IdentifierDatabase class.
***********************************************************************************************************************/

#include "m_api_types.h"
#include "m_identifier_data.h"
#include "model_identifier_data.h"
#include "model_identifier_data_private.h"
#include "model_identifier_database.h"
#include "model_identifier_database_private.h"
#include "m_identifier_database.h"

namespace M {
    IdentifierDatabase::IdentifierDatabase() {}


    IdentifierDatabase::IdentifierDatabase(const IdentifierDatabase& other):Model::IdentifierDatabase(other) {}


    IdentifierDatabase::IdentifierDatabase(IdentifierDatabase&& other):Model::IdentifierDatabase(other) {}


    IdentifierDatabase::IdentifierDatabase(const Model::IdentifierDatabase& other):Model::IdentifierDatabase(other) {}


    IdentifierDatabase::IdentifierDatabase(Model::IdentifierDatabase&& other):Model::IdentifierDatabase(other) {}


    IdentifierDatabase::~IdentifierDatabase() {}


    bool IdentifierDatabase::insert(const Model::IdentifierData& newEntry) {
        return impl->insert(newEntry);
    }


    bool IdentifierDatabase::insert(const IdentifierData& newEntry) {
        return insert(Model::IdentifierData(new Model::IdentifierData::Private(newEntry)));
    }


    bool IdentifierDatabase::insert(
            IdentifierHandle identifierHandle,
            const char*      text1,
            const char*      text2,
            bool             isFunction,
            ValueType        valueType,
            void*            pointer
        ) {
        return insert(IdentifierData(identifierHandle, text1, text2, isFunction, valueType, pointer));
    }


    IdentifierDatabase& IdentifierDatabase::operator=(const IdentifierDatabase& other) {
        Model::IdentifierDatabase::operator=(other);
        return *this;
    }


    IdentifierDatabase& IdentifierDatabase::operator=(IdentifierDatabase&& other) {
        Model::IdentifierDatabase::operator=(other);
        return *this;
    }
}
