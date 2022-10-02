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
* This header defines the \ref M::IdentifierData class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_IDENTIFIER_DATA_H
#define M_IDENTIFIER_DATA_H

#include "m_common.h"
#include "m_api_types.h"

namespace M {
    /**
     * Lightweight class used to store data about an identifier.
     */
    class M_PUBLIC_API IdentifierData {
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
            inline constexpr IdentifierData(
                    IdentifierHandle identifierHandle,
                    const char*      text1,
                    const char*      text2,
                    bool             isFunction,
                    ValueType        valueType,
                    void*            pointer
                ):currentHandle(
                    identifierHandle
                ),currentText1(
                    text1
                ),currentText2(
                    text2
                ),currentIsFunction(
                    isFunction
                ),currentValueType(
                    valueType
                ),currentPointer(
                    pointer
                ),deleteInDestructor(
                    false
                ) {}

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            IdentifierData(const IdentifierData& other);

            ~IdentifierData();

            /**
             * Method you can use to obtain the handle for a variable.
             *
             * \return Returns the handle for the variable.
             */
            IdentifierHandle identifierHandle() const;

            /**
             * Method you can use to obtain the UTF-8 encoded variable name as presented to the user within the
             * program.
             *
             * \return Returns a NUL terminated string holding the UTF-8 encoded variable name as seen in the document
             *         view of the program.
             */
            const char* text1() const;

            /**
             * Method you can use to obtain the UTF-8 encoded variable subscript text as presented to the user within
             * the program.
             *
             * \return Returns a NUL terminated string holding the UTF-8 encoded variable subscript.
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
             * Method you can use to determine the current address of the underlying data instance.
             *
             * \return Returns a pointer to the underlying data instance.
             */
            void* pointer() const;

        private:
            /**
             * The variable handle.
             */
            IdentifierHandle currentHandle;

            /**
             * The variable's main text name (UTF-8 format)
             */
            const char* currentText1;

            /**
             * The variable's subscript.
             */
            const char* currentText2;

            /**
             * Flag indicating that this identifier is a function.
             */
            bool currentIsFunction;

            /**
             * The variable's data-type.
             */
            ValueType currentValueType;

            /**
             * Pointer to the underlying identifier instance.
             */
            void* currentPointer;

            /**
             * Flag indicating if the provided values should be deleted at end of scope.
             */
            bool deleteInDestructor;
    };
}

#endif
