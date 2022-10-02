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
* This header defines the \ref M::SetIterator class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_SET_ITERATOR_H
#define M_SET_ITERATOR_H

#include "m_common.h"
#include "model_intrinsic_types.h"
#include "m_variant.h"
#include "m_implicit_ordering.h"
#include "m_unordered_set_entry.h"
#include "m_ordered_set_entry.h"
#include "m_ordered_set_iterator.h"
#include "m_ordered_set.h"
#include "m_variant_ordered_set.h"
#include "model_set.h"

namespace M {
    class Variant;
    class Range;

    M_PUBLIC_TEMPLATE_CLASS(UnorderedSetEntry<Variant>)
    M_PUBLIC_TEMPLATE_CLASS(OrderedSetEntry<Variant>)
    M_PUBLIC_TEMPLATE_CLASS(OrderedSetIterator<Variant>)

    /**
     * Set iterator class.  You can use this class to iterate through an \ref M::Set class instance.
     */
    class M_PUBLIC_API SetIterator:public VariantOrderedSet::Iterator {
        public:
            SetIterator();

            /**
             * Constructor
             *
             * \param[in] other       The instance to be copied.
             *
             * \param[in] setInstance The set API instance associated with this iterator.
             */
            SetIterator(const VariantOrderedSet::Iterator& other, const Model::Set* setInstance = nullptr);

            /**
             * Copy constructor
             *
             * \param other The instance to be copied.
             */
            SetIterator(const SetIterator& other);

            virtual ~SetIterator();

            /**
             * Method you can use to set the API set instance to associate with this iterator.
             *
             * \param[in] setInstance The set API instance associated with this iterator.
             */
            void setSetApi(const Model::Set* setInstance);

            /**
             * Method you can use to obtain the API instance associated with this iterator.
             *
             * \return Returns a pointer to the set API instance associated with this iterator.
             */
            const Model::Set* setApi() const;

            /**
             * Copy assignment operator.
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this instance.
             */
            SetIterator& operator=(const SetIterator& other);

        private:
            /**
             * The Set API to associate with this iterator.
             */
            const Model::Set* currentSetApi;
    };
};

#endif
