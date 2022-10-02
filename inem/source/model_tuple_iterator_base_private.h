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
* This header defines the \ref Model::TupleIteratorBase::Private class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_TUPLE_ITERATOR_BASE_PRIVATE_H
#define MODEL_TUPLE_ITERATOR_BASE_PRIVATE_H

#include "m_variant.h"
#include "model_tuple.h"
#include "model_tuple_iterator_base.h"

namespace Model {
    class Variant;
    class Tuple;

    /**
     * Private implementation of the \ref Model::TupleIteratorBase class.
     */
    class TupleIteratorBase::Private {
        friend class Tuple;

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
             * \param[in] tupleImplementation The instance to be copied.
             *
             * \param[in] index               The index used to position the iterator.
             */
            Private(const Tuple* tupleImplementation, unsigned long long index = 0);

            ~Private();

            /**
             * Method you can use to determine if this iterator is pointing to a valid entry.
             *
             * \return Returns true if the iterator is pointing to a valid entry.  Returns false if the iterator is
             *         not pointing to a valid entry.
             */
            bool isValid() const;

            /**
             * Method you can use to advance the iterator by one position.
             */
            void advance();

            /**
             * Method you can use to advance the iterator by a specified distance.
             *
             * \param[in] distance The distance to advance the iterator.
             */
            void advance(signed long long distance);

            /**
             * Method you can use to backup the iterator by one position.
             */
            void backup();

            /**
             * Method you can use to advance the iterator by a specified distance.
             *
             * \param[in] distance The distance to advance the iterator.
             */
            void backup(signed long long distance);

            /**
             * Method you can use to calculate the distance between this iterator and another iterator.
             *
             * \param[in] other The other iterator to compare against.
             *
             * \return Returns the distance between the iterators.
             */
            signed long long distance(const Private& other) const;

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

            /**
             * Comparison operator
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if the instance point to the same entry in the set.  Returns false if the instances
             *         point to different entries.  Comparing entries in different sets will lead to indeterminate
             *         results.
             */
            bool operator==(const Private& other) const;

            /**
             * Comparison operator
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if the instance point to different entries in the set.  Returns false if the
             *         instances point to the same entry.  Comparing entries in different sets will lead to
             *         indeterminate results.
             */
            bool operator!=(const Private& other) const;

        private:
            /**
             * Method that is called from other methods to update the current variant value.
             */
            void updateCurrentValue();

            /**
             * Pointer to the tuple associated with this iterator.
             */
            const Tuple* currentTuple;

            /**
             * Current index into the tuple.
             */
            unsigned long long currentIndex;

            /**
             * The current value being tracked by this variant.
             */
            Variant currentValue;
    };
};

#endif
