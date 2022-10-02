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
* This header defines the \ref Model::IdentifierDatabase class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_IDENTIFIER_DATABASE_H
#define MODEL_IDENTIFIER_DATABASE_H

#include "model_common.h"
#include "model_api_types.h"
#include "model_identifier_data.h"
#include "model_identifier_database_iterator.h"

namespace M {
    class IdentifierDatabase;
}

namespace Model {
    /**
     * Class you can use to identify variables and functions within your model or algorithm.  This class uses
     * copy-on-write semantics allowing you to rapidly assign one database to another within your own code.
     */
    class MODEL_PUBLIC_API IdentifierDatabase {
        friend class M::IdentifierDatabase;
        friend class IdentifierDatabaseIterator;

        public:
            /**
             * Type used to represent a constant iterator.
             */
            typedef IdentifierDatabaseIterator Iterator;

            /**
             * Type used to represent a constant iterator.
             */
            typedef IdentifierDatabaseIterator ConstIterator;

            /**
             * Type provided for STL compatibility.
             */
            typedef IdentifierDatabaseIterator iterator;

            /**
             * Type provided for STL compatibility.
             */
            typedef IdentifierDatabaseIterator const_iterator;

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

            ~IdentifierDatabase();

            /**
             * Method you can use to determine if the list is empty.
             *
             * \return Returns true if the list is empty.  Returns false if the list is not empty.
             */
            bool isEmpty() const;

            /**
             * Method you can use to determine the size of the list, in identifiers.
             *
             * \return Returns the current size of the list, in identifiers.
             */
            unsigned size() const;

            /**
             * Method you can use to obtain identifier data by identifier name.
             *
             * \param[in] text1 The UTF-8 encoded identifier's name.
             *
             * \param[in] text2 The UTF-8 encoded identifier's subscript.  You can either provide an empty string or
             *                  a null pointer if the desired identifier does not have a subscript.
             *
             * \return Returns the identifier data associated with the requested identifier name.  An invalid
             *         identifier data entry is returned if the name is invalid.
             */
            const IdentifierData& identifierDataByName(const char* text1, const char* text2 = nullptr) const;

            /**
             * Method you can use to obtain identifier data by identifier handle.
             *
             * \param[in] handle The handle to obtain the variable name of.
             *
             * \return Returns the identifier data associated with the specified handle.  An invalid identifier data
             *         instance is returned if the supplied handle is invalid.
             */
            const IdentifierData& identifierDataByHandle(IdentifierHandle handle) const;

            /**
             * Method you can use to obtain a constant iterator to the first entry in the database.
             *
             * \return Returns a constant iterator to the first entry in the database.
             */
            ConstIterator constBegin() const;

            /**
             * Method you can use to obtain a constant iterator just past the last entry in the database.
             *
             * \return Returns a constant iterator just past the last entry in the database.
             */
            ConstIterator constEnd() const;

            /**
             * Method you can use to obtain a constant iterator to the first entry in the database.  This version is
             * provided for STL compatibility.
             *
             * \return Returns a constant iterator to the first entry in the database.
             */
            const_iterator cbegin() const;

            /**
             * Method you can use to obtain a constant iterator just past the last entry in the database.  This version
             * is provided for STL compatibility.
             *
             * \return Returns a constant iterator just past the last entry in the database.
             */
            const_iterator cend() const;

            /**
             * Method you can use to obtain an iterator to the first entry in the database.
             *
             * \return Returns an iterator to the first entry in the database.
             */
            Iterator begin() const;

            /**
             * Method you can use to obtain an iterator just past the last entry in the database.
             *
             * \return Returns an iterator just past the last entry in the database.
             */
            Iterator end() const;

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

        private:
            class Private;

            /**
             * Private constructor
             *
             * \param[in] newImplementation The new private implementation to assign to this class.
             */
            inline IdentifierDatabase(Private* newImplementation):impl(newImplementation) {}

            Private* impl;
    };
}

#endif
