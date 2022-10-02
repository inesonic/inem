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
* This header defines the \ref M::IdentifierDatabase class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_IDENTIFIER_DATABASE_H
#define M_IDENTIFIER_DATABASE_H

#include "m_common.h"
#include "m_api_types.h"
#include "model_identifier_database.h"
#include "m_identifier_data.h"

namespace M {
    /**
     * Internal identifier database class.  This class extends the \ref Model::IdentifierDatabase to provide support
     * for insertion of new identifier data entries.
     */
    class M_PUBLIC_API IdentifierDatabase:public Model::IdentifierDatabase {
        public:
            IdentifierDatabase();

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            IdentifierDatabase(const IdentifierDatabase& other);

            /**
             * Move constructor
             *
             * \param[in] other The instance to be copied.
             */
            IdentifierDatabase(IdentifierDatabase&& other);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            IdentifierDatabase(const Model::IdentifierDatabase& other);

            /**
             * Move constructor
             *
             * \param[in] other The instance to be copied.
             */
            IdentifierDatabase(Model::IdentifierDatabase&& other);

            ~IdentifierDatabase();

            /**
             * Method you can use to insert a new entry into the identifier database.
             *
             * \param[in] newEntry The new entry to be added to the database.
             *
             * \return Returns true on success, returns false on error.
             */
            bool insert(const Model::IdentifierData& newEntry);

            /**
             * Method you can use to insert a new entry into the identifier database.
             *
             * \param[in] newEntry The new entry to be added to the database.
             *
             * \return Returns true on success, returns false on error.
             */
            bool insert(const IdentifierData& newEntry);

            /**
             * Convenience method you can use to insert a new entry into the identifier database.
             *
             * \param[in] identifierHandle The handle used to reference this variable.
             *
             * \param[in] text1            The variable's name within the UI.
             *
             * \param[in] text2            The variable's subscript.
             *
             * \param[in] isFunction       Value indicating true if this identifier references a function.
             *
             * \param[in] valueType        The variable's data-type name.
             *
             * \param[in] pointer          Pointer to the underlying instance.
             *
             * \return Returns true on success, returns false on error.
             */
            bool insert(
                IdentifierHandle identifierHandle,
                const char*      text1,
                const char*      text2,
                bool             isFunction,
                ValueType        valueType,
                void*            pointer
            );

            /**
             * Copy assignment operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            IdentifierDatabase& operator=(const IdentifierDatabase& other);

            /**
             * Move assignment operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            IdentifierDatabase& operator=(IdentifierDatabase&& other);
    };
}

#endif
