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
* This header defines the \ref Model::IdentifierData class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_IDENTIFIER_DATA_H
#define MODEL_IDENTIFIER_DATA_H

#include "model_common.h"
#include "model_api_types.h"
#include "model_variant.h"

namespace M {
    class MODEL_PUBLIC_API IdentifierDatabase;
}

namespace Model {
    /**
     * Class that is used to report information on identifiers within the model.  You can use this class to query the
     * various identifiers and access identifiers within a model.
     *
     * Note that this class uses copy-on-write semantics allowing you to quickly assign one instance of this class to
     * another instance.
     */
    class MODEL_PUBLIC_API IdentifierData {
        friend class MODEL_PUBLIC_API M::IdentifierDatabase;

        public:
            IdentifierData();

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            IdentifierData(const IdentifierData& other);

            /**
             * Move constructor
             *
             * \param[in] other The instance to move to this instance.
             */
            IdentifierData(IdentifierData&& other);

            ~IdentifierData();

            /**
             * Method you can use to determine if this identifier is valid.
             *
             * \return Returns true if the identifier instance is valid.  Returns false if the identifier instance is
             *         invalid.
             */
            bool isValid() const;

            /**
             * Method you can use to determine if this identifier is invalid.
             *
             * \return Returns true if the identifier instance is invalid.  Returns false if the identifier instance is
             *         valid.
             */
            bool isInvalid() const;

            /**
             * Method you can use to obtain the identifier handle for this identifier.
             *
             * \return Returns the handle for the identifier.
             */
            unsigned identifierHandle() const;

            /**
             * Method you can use to obtain the UTF-8 encoded identifier name as presented to the user within the
             * program.
             *
             * \return Returns a NUL terminated string holding the UTF-8 encoded identifier name as seen in the
             *         document view of the program.  A null pointer is returned if the identifier data is invalid.
             */
            const char* text1() const;

            /**
             * Method you can use to obtain the UTF-8 encoded identifier subscript text as presented to the user within
             * the program.
             *
             * \return Returns a NUL terminated string holding the UTF-8 encoded identifier subscript.  A null pointer
             *         is returned if the identifier data is invalid.
             */
            const char* text2() const;

            /**
             * Method you can use to determine if this identifier points to a function.
             *
             * \return Returns true if this identifier points to a function.
             */
            bool isFunction() const;

            /**
             * Method you can use to determine if this identifier points to an identifier.
             *
             * \return Returns true if this identifier points to a variable.
             */
            bool isVariable() const;

            /**
             * Method you can use to obtain the type of the identifier.
             *
             * \return Returns the type of the identifier.  A value of ValueType::NONE is returned if the identifier
             *         data is invalid.
             */
            ValueType valueType() const;

            /**
             * Method you can use to obtain a variant holding the current value of the variable.
             *
             * \return Returns a variant holding the current value of the variable.
             */
            Variant value() const;

            /**
             * Method you can use to update the current value of the variable.
             *
             * \param[in] newValue The new value of the variable.
             *
             * \return Returns true on success, returns false if the variant is not compatible with the variable.
             */
            bool setValue(const Variant& newValue);

            /**
             * Method you can use to obtain the address of a function.
             *
             * \return Returns the address of a function associated with this identifier.
             */
            const void* functionAddress() const;

            /**
             * Copy assignment operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            IdentifierData& operator=(const IdentifierData& other);

            /**
             * Move assignment operator.
             *
             * \param[in] other The instance to move to this instance.
             *
             * \return Returns a reference to this instance.
             */
            IdentifierData& operator=(IdentifierData&& other);

        private:
            class Private;

            /**
             * Private constructor
             *
             * \param[in] newImplementation Pointer to the new implementation to assign to this
             *                              \ref Model::IdentifierData instance.
             */
            inline IdentifierData(Private* newImplementation):impl(newImplementation) {}

            Private* impl;
    };
}

#endif
