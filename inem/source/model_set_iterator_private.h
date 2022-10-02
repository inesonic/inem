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
* This header defines the \ref Model::SetIterator::Private class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_SET_ITERATOR_PRIVATE_H
#define MODEL_SET_ITERATOR_PRIVATE_H

#include "model_set_iterator.h"
#include "model_variant.h"
#include "m_variant.h"
#include "m_ordered_set.h"
#include "m_set_iterator.h"
#include "m_variant_ordered_set.h"
#include "m_set.h"

namespace Model {
    class Set;

    /**
     * Iterator you can use to traverse a set.
     */
    class SetIterator::Private:public M::SetIterator {
        public:
            Private();

            /**
             * Copy constructor
             *
             * \param other The instance to be copied.
             */
            Private(const Private& other);

            /**
             * Constructor
             *
             * \param other The instance to be copied.
             */
            Private(const M::SetIterator& other);

            /**
             * Constructor
             *
             * \param[in] other       The instance to be copied.
             *
             * \param[in] setInstance The set API instance associated with this iterator.
             */
            Private(const M::VariantOrderedSet::Iterator& other, const Model::Set* setInstance = nullptr);

            virtual ~Private();

            /**
             * Method you can use to advance the iterator by one position.
             */
            void advance();

            /**
             * Method you can use to advance the iterator by a specified distance.
             *
             * \param[in] distance The distance to advance the iterator.
             */
            void advance(unsigned long long distance);

            /**
             * Method you can use to set the current value.
             *
             * \param[in] newValue The new value to assign to the variant.
             */
            void setValue(const Variant& newValue);

            /**
             * Method you can use to obtain the current value.
             *
             * \return Returns a reference to the current value.
             */
            const Variant& publicValue() const;

            /**
             * Method you can use to obtain a pointer to the current value.
             *
             * \return Returns a pointer to to the current value.
             */
            const Variant* publicPointer() const;

        private:
            /**
             * Method that is called from other methods to update the current variant value.
             */
            void updateCurrentValue();

            /**
             * The current value being tracked by this variant.
             */
            Variant currentValue;
    };
};

#endif
