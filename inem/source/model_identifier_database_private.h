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
* This header defines the \ref Model::IdentifierDatabase::Private class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_IDENTIFIER_DATABASE_PRIVATE_H
#define MODEL_IDENTIFIER_DATABASE_PRIVATE_H

#include <unordered_map>
#include <string>

#include "model_api_types.h"
#include "model_identifier_data.h"
#include "model_identifier_database_iterator.h"
#include "model_identifier_database.h"
#include "m_reference_counter.h"

namespace Model {
    /**
     * Private implementation of the \ref Model::IdentifierDatabase class.
     */
    class IdentifierDatabase::Private:public M::ReferenceCounter {
        public:
            /**
             * Type used to represent a constant iterator.
             */
            typedef std::unordered_map<IdentifierHandle, IdentifierData> Container;

            /**
             * Type used to represent a constant iterator.
             */
            typedef Container::const_iterator Iterator;

            Private();

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            Private(const Private& other);

            ~Private();

            /**
             * Method you can use to determine if the list is empty.
             *
             * \return Returns true if the list is empty.  Returns false if the list is not empty.
             */
            bool isEmpty() const;

            /**
             * Method you can use to determine the size of the list, in elements.
             *
             * \return Returns the current size of the list, in elements.
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
             * Method you can use to insert a new entry into the identifier database.
             *
             * \param[in] newEntry The new entry to be added to the database.
             *
             * \return Returns true on success, returns false on error.
             */
            bool insert(const IdentifierData& newEntry);

            /**
             * Method you can use to obtain an iterator to the first entry in the range.
             *
             * \return Returns an iterator to the first entry in the range.
             */
            Iterator begin() const;

            /**
             * Method you can use to obtain an iterator just past the last entry in the range.
             *
             * \return Returns an iterator just past the last entry in the range.
             */
            Iterator end() const;

            /**
             * Method you can use to obtain a pointer to the iterator container.
             *
             * \return Returns a pointer to the iterator container.
             */
            const Container* iteratorContainer() const;

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to be assigned to this instance.
             *
             * \return Returns a reference to this instance.
             */
            Private& operator=(const Private& other);

        private:
            /**
             * A dummy identifier data instance used for return by reference.
             */
            static const IdentifierData dummyIdentifierData;

            /**
             * Trivial class that stores a pair of text entries.
             */
            class VariableName {
                public:
                    VariableName();

                    /**
                     * Constructor
                     *
                     * \param[in] text1 The first text value.
                     *
                     * \param[in] text2 The second text value.
                     */
                    VariableName(const char* text1, const char* text2);

                    /**
                     * Constructor
                     *
                     * \param[in] text1 The first text value.
                     *
                     * \param[in] text2 The second text value.
                     */
                    VariableName(const std::string& text1, const std::string& text2);

                    /**
                     * Copy constructor.
                     *
                     * \param[in] other The instance to be copied.
                     */
                    VariableName(const VariableName& other);

                    /**
                     * Method that returns the first text value.
                     *
                     * \return Returns a string holding the first text value.
                     */
                    std::string text1() const;

                    /**
                     * Method that returns the second text value.
                     *
                     * \return Returns the second text value.
                     */
                    std::string text2() const;

                    /**
                     * Comparison operator.
                     *
                     * \param[in] other The value to compare against.
                     *
                     * \return Returns true if the values are equal.  Returns false if the values are not equal.
                     */
                    bool operator==(const VariableName& other) const;

                    /**
                     * Comparison operator.
                     *
                     * \param[in] other The value to compare against.
                     *
                     * \return Returns true if the values are not.  Returns false if the values are equal.
                     */
                    bool operator!=(const VariableName& other) const;

                    /**
                     * Comparison operator.
                     *
                     * \param[in] other The value to compare against.
                     *
                     * \return Returns true if this value should precede the other value.  Returns false if this value
                     *         should follow the other value.
                     */
                    bool operator<(const VariableName& other) const;

                    /**
                     * Comparison operator.
                     *
                     * \param[in] other The value to compare against.
                     *
                     * \return Returns true if this value should follow the other value.  Returns false if this value
                     *         should precede the other value.
                     */
                    bool operator>(const VariableName& other) const;

                    /**
                     * Comparison operator.
                     *
                     * \param[in] other The value to compare against.
                     *
                     * \return Returns true if this value should precede the other value or is equal to the other
                     *         value.  Returns false if this value should follow the other value.
                     */
                    bool operator<=(const VariableName& other) const;

                    /**
                     * Comparison operator.
                     *
                     * \param[in] other The value to compare against.
                     *
                     * \return Returns true if this value should follow the other value or is equal to the other value.
                     *         Returns false if this value should precede the other value.
                     */
                    bool operator>=(const VariableName& other) const;

                    /**
                     * Assignment operator
                     *
                     * \param[in] value The value to be assigned to this instance.
                     *
                     * \return Returns a reference to this instance.
                     */
                    VariableName& operator=(const VariableName& value);

                private:
                    /**
                     * The first text string.
                     */
                    std::string currentText1;

                    /**
                     * The second text string.
                     */
                    std::string currentText2;
            };

            /**
             * Trivial class used to hash the VariableName instances.
             */
            class VariableNameHasher {
                public:
                    /**
                     * Functor used to hash the VariableName values.
                     *
                     * \param[in] value A reference to the VariableName instance to be hashed.
                     *
                     * \return Returns a hash calculated from the variable name.
                     */
                    std::size_t operator()(const VariableName& value) const;

                private:
                    static std::hash<std::string> stringHasher;
            };

            /**
             * Hash table providing identifier data instances by handle.
             */
            std::unordered_map<IdentifierHandle, IdentifierData> currentIdentifierDataByHandle;

            /**
             * Hash table providing identifier data instances by variable name.
             */
            std::unordered_map<VariableName, IdentifierData, VariableNameHasher> currentIdentifierDataByName;
    };
}

#endif
