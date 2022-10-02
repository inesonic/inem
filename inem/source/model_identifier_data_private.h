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
* This header defines the \ref Model::IdentifierData::Private class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_IDENTIFIER_DATA_PRIVATE_H
#define MODEL_IDENTIFIER_DATA_PRIVATE_H

#include "model_api_types.h"
#include "m_reference_counter.h"
#include "m_identifier_data.h"
#include "m_variant.h"
#include "model_identifier_data.h"

namespace Model {
    /**
     * Private implementation of the \ref Model::IdentifierData class.
     */
    class IdentifierData::Private:public M::ReferenceCounter,private M::IdentifierData {
        public:
            /**
             * Constructor
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
             */
            Private(
                IdentifierHandle identifierHandle,
                const char*      text1,
                const char*      text2,
                bool             isFunction,
                ValueType        valueType,
                void*            pointer
            );

            /**
             * Constructor
             *
             * \param[in] identifierData The identifier data instance to copy to this instance.
             */
            Private(const M::IdentifierData& identifierData);

            /**
             * Constructor
             *
             * \param[in] identifierData The identifier data instance to copy to this instance.
             */
            Private(const M::IdentifierData* identifierData);

            /**
             * Copy constructor
             */
            Private(const Private& other);

            ~Private();

            /**
             * Method you can use to obtain the handle for the identifier.
             *
             * \return Returns the handle for the variable.
             */
            IdentifierHandle identifierHandle() const;

            /**
             * Method you can use to obtain the UTF-8 encoded identifier name as presented to the user within the
             * program.
             *
             * \return Returns a NUL terminated string holding the UTF-8 encoded identifier name as seen in the
             *         document view of the program.
             */
            const char* text1() const;

            /**
             * Method you can use to obtain the UTF-8 encoded identifier subscript text as presented to the user within
             * the program.
             *
             * \return Returns a NUL terminated string holding the UTF-8 encoded identifier subscript.
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
             * \return Returns the type of the identifier.
             */
            ValueType valueType() const;

            /**
             * Method you can use to obtain a variant holding the current value of the variable.
             *
             * \return Returns a variant holding the current value of the variable.
             */
            M::Variant value() const;

            /**
             * Method you can use to update the current value of the variable.
             *
             * \param[in] newValue The new value of the variable.
             *
             * \return Returns true on success, returns false if the variant is not compatible with the variable.
             */
            bool setValue(const M::Variant& newValue);

            /**
             * Method you can use to obtain the address of a function.
             *
             * \return Returns the address of a function associated with this identifier.
             */
            const void* functionAddress() const;
    };
}

#endif
