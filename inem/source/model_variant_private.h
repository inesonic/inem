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
* This header defines the \ref Model::Variant::Private class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_VARIANT_PRIVATE_H
#define MODEL_VARIANT_PRIVATE_H

#include "model_intrinsic_types.h"
#include "model_complex.h"
#include "m_reference_counter.h"
#include "m_variant.h"
#include "model_variant.h"

namespace Model {
    class Set;
    class Tuple;
    class MatrixBoolean;
    class MatrixInteger;
    class MatrixReal;
    class MatrixComplex;

    /**
     * Private implementation of the \ref Model::Variant class.  This class acts as a very thin shim on the
     * \ref M::Variant class.
     */
    class Variant::Private:public M::Variant, public M::ReferenceCounter {
        public:
            inline Private() {}

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            inline Private(Boolean value):M::Variant(value) {}

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            inline Private(Integer value):M::Variant(value) {}

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            inline Private(Real value):M::Variant(value) {}

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            inline Private(Complex value):M::Variant(value) {}

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            inline Private(const Model::Set& value):M::Variant(value) {}

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            inline Private(const Model::Tuple& value):M::Variant(value) {}

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            inline Private(const Model::MatrixBoolean& value):M::Variant(value) {}

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            inline Private(Model::MatrixBoolean&& value):M::Variant(value) {}

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            inline Private(const Model::MatrixInteger& value):M::Variant(value) {}

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            inline Private(Model::MatrixInteger&& value):M::Variant(value) {}

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            inline Private(const Model::MatrixReal& value):M::Variant(value) {}

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            inline Private(Model::MatrixReal&& value):M::Variant(value) {}

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            inline Private(const Model::MatrixComplex& value):M::Variant(value) {}

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            inline Private(Model::MatrixComplex&& value):M::Variant(value) {}

            /**
             * Constructor
             *
             * \param[in] pointer   Pointer to a location containing a value of a specified type.
             *
             * \param[in] valueType The type of the value at the location.
             */
            inline Private(const void* pointer, ValueType valueType):M::Variant(pointer, valueType) {}

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            inline Private(const M::Variant& other):M::Variant(other) {}

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            inline Private(const Private& other):M::Variant(other) {}

            inline ~Private() {}
    };
}

#endif
