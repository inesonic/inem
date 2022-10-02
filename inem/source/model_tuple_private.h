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
* This header defines the \ref Model::Tuple::Private class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_TUPLE_PRIVATE_H
#define MODEL_TUPLE_PRIVATE_H

#include "m_intrinsics.h"
#include "m_variant.h"
#include "m_list.h"
#include "m_reference_counter.h"
#include "model_tuple.h"

namespace M {
    class Range;
}

namespace Model {
    class Range;

    /**
     * Private implementation of the \ref Model::Tuple class.
     */
    class Tuple::Private:public M::List<M::Variant>, public M::ReferenceCounter {
        public:
            Private();

            /**
             * Constructor
             *
             * \param[in] str The nul terminated string to convert to a tuple.  The string is expected to be UTF-8
             *                encoded.
             */
            Private(const char* str);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            Private(const Private& other);

            /**
             * Constructor
             *
             * \param[in] other The instance to be copied.
             */
            Private(const M::List<M::Variant>& other);

            /**
             * Move constructor
             *
             * \param[in] other The instance to be copied.
             */
            Private(Private&& other);

            virtual ~Private();

            /**
             * Method you can use to append a new value to the end of the tuple.
             *
             * \param[in] newValue The value to be appended to the tuple.
             */
            inline void append(const M::Variant& newValue) {
                M::List<M::Variant>::append(newValue);
            }

            /**
             * Method you can use to prepend a new value to the front of the tuple.
             *
             * \param[in] newValue The value to be prepended to the tuple.
             */
            inline void prepend(const M::Variant& newValue) {
                M::List<M::Variant>::prepend(newValue);
            }

            /**
             * Method you can use to append a new range to the end of the tuple.
             *
             * \param[in] range The range to be appended to the tuple.
             */
            void append(const Range& range);

            /**
             * Method you can use to prepend a new range to the end of the tuple.
             *
             * \param[in] range The range to be prepended to the tuple.
             */
            void prepend(const Range& range);

            /**
             * Method you can use to append a new range to the end of the tuple.
             *
             * \param[in] range The range to be appended to the tuple.
             */
            void append(const M::Range& range);

            /**
             * Method you can use to prepend a new range to the end of the tuple.
             *
             * \param[in] range The range to be prepended to the tuple.
             */
            void prepend(const M::Range& range);
    };
};

#endif
