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
* This header defines the \ref Model::Set::Private class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_SET_PRIVATE_H
#define MODEL_SET_PRIVATE_H

#include "m_intrinsics.h"
#include "m_variant.h"
#include "m_hash_functions.h"
#include "m_implicit_ordering.h"
#include "m_unordered_set.h"
#include "m_ordered_set.h"
#include "m_variant_ordered_set.h"
#include "m_reference_counter.h"
#include "model_set.h"

namespace Model {
    /**
     * Trivial private implementation of the \ref Model::Set class.
     */
    class Set::Private:public M::VariantOrderedSet, public M::ReferenceCounter {
        public:
            /**
             * Type used to hold reservation data/
             */
            typedef M::VariantOrderedSet::IteratorStateData IteratorStateData;

            Private();

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
            Private(const M::OrderedSet<M::Variant>& other);

            /**
             * Move constructor
             *
             * \param[in] other The instance to be copied.
             */
            Private(Private&& other);

            /**
             * Move constructor
             *
             * \param[in] other The instance to be copied.
             */
            Private(M::OrderedSet<M::Variant>&& other);

            virtual ~Private();

            /**
             * Method that calculates the cartesian product of this set and another set.
             *
             * \param[in] other The set to apply as the second parameter in the cartesian product.
             *
             * \return Returns a set containing the cartesian product of the two sets.
             */
            Private* cartesianProduct(const Private& other) const;

            /**
             * Method you can call to capture iterator state data.
             *
             * \return Returns an instance of \ref Model::Set::Private::IteratorStateData you can use to restore the
             *         iterator state.  The data is stored on the heap and must be deleted.
             */
            IteratorStateData* captureIteratorState();

            /**
             * Method you can call to restore iterator state.
             *
             * \param[in] iteratorStateData The iterator state data instance to use to restore the iterator state.
             *                              The state data instance will be deleted by this method.
             *
             * \param[in] setInstance       The \ref Model::Set instance we want to limit our iterators to.
             */
            void restoreIteratorState(IteratorStateData* iteratorStateData, Model::Set* setInstance);

            /**
             * Method you can call to reassign iterators to point to this instance.
             *
             * \param[in] old The instance to reassign iterators from.
             */
            void reassignIterators(Private* old);
    };
};

#endif
