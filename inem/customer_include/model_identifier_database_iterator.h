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
* This header defines the \ref Model::IdentifierDatabaseIterator class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_IDENTIFIER_DATABASE_ITERATOR_H
#define MODEL_IDENTIFIER_DATABASE_ITERATOR_H

#include "model_common.h"
#include "model_api_types.h"
#include "model_identifier_data.h"

namespace Model {
    class IdentifierDatabase;

    /**
     * Iterator you can use to traverse an identifier database.
     */
    class MODEL_PUBLIC_API IdentifierDatabaseIterator {
        friend class IdentifierDatabase;

        public:
            IdentifierDatabaseIterator();

            /**
             * Copy constructor
             *
             * \param other The instance to be copied.
             */
            IdentifierDatabaseIterator(const IdentifierDatabaseIterator& other);

            /**
             * Move constructor
             *
             * \param[in] other The instance to apply to this instance.
             */
            IdentifierDatabaseIterator(IdentifierDatabaseIterator&& other);

            ~IdentifierDatabaseIterator();

            /**
             * Method you can use to obtain the current value pointed to by the iterator.  Note that the
             * \ref Model::IdentifierDatabaseIterator::constReference method may prove to be slightly faster.
             *
             * \return Returns a reference to the current value.
             */
            IdentifierData value() const;

            /**
             * Method you can use to obtain a reference to the current value pointed to by the iterator.
             *
             * \return Returns a reference to the current value.
             */
            const IdentifierData& constReference() const;

            /**
             * Method you can use to obtain a pointer to the current value.
             *
             * \return Returns a pointer to the current value.
             */
            const IdentifierData* constPointer() const;

            /**
             * Convenience method you can use to obtain the handle for a current identifier.
             *
             * \return Returns the handle for the variable.
             */
            IdentifierHandle identifierHandle() const;

            /**
             * Convenience method you can use to obtain the UTF-8 encoded primary name of the identifier.  This value
             * will match the name of the identifier, less the subscript.
             *
             * \return Returns a NUL terminated string holding the UTF-8 encoded identifier name as seen in the
             *         document view of the program.
             */
            const char* text1() const;

            /**
             * Convenience method you can use to obtain the UTF-8 encoded subscript of the identifier.  This value will
             * match the subscript of the identifier.
             *
             * \return Returns a NUL terminated string holding the UTF-8 encoded variable subscript.  An empty string
             *         is returned if there is no subscript.
             */
            const char* text2() const;

            /**
             * Convenience method you can use to determine if this identifier points to a function.
             *
             * \return Returns true if this identifier points to a function.
             */
            bool isFunction() const;

            /**
             * Convenience method you can use to determine if this identifier points to an identifier.
             *
             * \return Returns true if this identifier points to a variable.
             */
            bool isVariable() const;

            /**
             * Convenience method you can use to determine the value type of the identifier pointed to by the iterator.
             *
             * \return Returns the value type of the identifier pointed to by the iterator.
             */
            ValueType valueType() const;

            /**
             * Method you can use to advance the iterator by one position.
             */
            void advance();

            /**
             * Method you can use to advance the iterator by a specified distance.
             *
             * \param[in] distance The distance to advance the iterator.
             */
            void advance(unsigned long distance);

            /**
             * Method you can use to determine if this iterator is pointing to a valid entry.
             *
             * \return Returns true if this iterator is pointing to a valid entry in the database.  Returns false if
             *         the iterator is not initialized or is pointing to an invalid location.
             */
            bool isValid() const;

            /**
             * Method you can use to determine if this iterator is pointing to an invalid location.
             *
             * \return Returns true if this iterator is pointing to an invalid location or is not initialized.  Returns
             *         false if this iterator is pointing to a valid location in the container.
             */
            bool isInvalid() const;

            /**
             * Dereferencing operator.
             *
             * \see Model::IdentifierDatabaseIterator::value
             * \see Model::IdentifierDatabaseIterator::constReference
             *
             * \return Returns a reference to the current value pointed to by the iterator.
             */
            const IdentifierData& operator*() const;

            /**
             * Indirection operator.
             *
             * \see Model::IdentifierDatabaseIterator::constReference
             * \see Model::IdentifierDatabaseIterator::constPointer
             *
             * \return Returns a pointer to the current value pointed to by the iterator.
             */
            const IdentifierData* operator->() const;

            /**
             * Pre-increment operator
             *
             * \return Returns a reference to this instance.
             */
            IdentifierDatabaseIterator& operator++();

            /**
             * Post-increment operator
             *
             * \return Returns an instance representing the state of this iterator prior to being advanced.
             */
            IdentifierDatabaseIterator operator++(int);

            /**
             * Add assignment operator.
             *
             * \param[in] distance The distance to advance the iterator.
             *
             * \return Returns a reference to this instance.
             */
            IdentifierDatabaseIterator& operator+=(unsigned long distance);

            /**
             * Copy assignment operator.
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this instance.
             */
            IdentifierDatabaseIterator& operator=(const IdentifierDatabaseIterator& other);

            /**
             * Move assignment operator.
             *
             * \param[in] other The instance to move-assign to this iterator.
             *
             * \return Returns a reference to this instance.
             */
            IdentifierDatabaseIterator& operator=(IdentifierDatabaseIterator&& other);

            /**
             * Comparison operator
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if the instance point to the same entry in the database.  Returns false if the
             *         iterator points to different entries.  Comparing entries in different databases will lead to
             *         indeterminate results.
             */
            bool operator==(const IdentifierDatabaseIterator& other) const;

            /**
             * Comparison operator
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if the instance point to different entries in the database.  Returns false if the
             *         instances point to the same entry.  Comparing entries in different databases will lead to
             *         indeterminate results.
             */
            bool operator!=(const IdentifierDatabaseIterator& other) const;

        private:
            class Private;

            /**
             * Private constructor
             *
             * \param[in] newImplementation The private implementation to assign to this iterator.
             */
            inline IdentifierDatabaseIterator(Private* newImplementation):impl(newImplementation) {}

            Private* impl;
    };
};

#endif
