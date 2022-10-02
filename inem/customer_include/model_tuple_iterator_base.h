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
* This header defines the \ref Model::TupleIteratorBase class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_TUPLE_ITERATOR_BASE_H
#define MODEL_TUPLE_ITERATOR_BASE_H

#include "model_common.h"

namespace Model {
    class MODEL_PUBLIC_API Variant;
    class MODEL_PUBLIC_API Tuple;

    /**
     * Common base class for the \ref Model::TupleConstIterator and \ref Model::TupleIterator classes.
     */
    class MODEL_PUBLIC_API TupleIteratorBase {
        friend class MODEL_PUBLIC_API Tuple;

        public:
            /**
             * Method you can use to obtain the current value pointed to by the iterator.  Note that the
             * \ref Model::TupleIteratorBase::constReference method may prove to be slightly faster.
             *
             * \return Returns a reference to the current value.
             */
            Variant value() const;

            /**
             * Method you can use to obtain a reference to the current value pointed to by the iterator.
             *
             * \return Returns a reference to the current value.
             */
            const Variant& constReference() const;

            /**
             * Method you can use to obtain a pointer to the current value.
             *
             * \return Returns a pointer to the current value.
             */
            const Variant* constPointer() const;

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
             * Method you can call to backup the iterator by one location.
             */
            void backup();

            /**
             * Method you can call to backup the iterator by a specified distance.
             *
             * \param[in] distance The distance to reverse the iterator.
             */
            void backup(signed long long distance);

            /**
             * Method you can use to determine if this iterator is pointing to a valid entry.
             *
             * \return Returns true if this iterator is pointing to a valid entry in the tuple.  Returns false if the
             *         iterator is not initialized or is pointing to an invalid location.
             */
            bool isValid() const;

            /**
             * Method you can use to determine if this iterator is pointing to an invalid location.
             *
             * \return Returns true if this iterator is pointing to an invalid location or is not initialized.  Returns
             *         false if this iterator is pointing to a valid location in the container.
             */
            bool isInvalid() const;

            /**
             * Dereferencing operator.  Note that the \ref constReference method may prove slightly faster than the
             * dereferencing operator.
             *
             * \see TupleIteratorBase::value
             * \see TupleIteratorBase::constReference
             *
             * \return Returns a reference to the current value pointed to by the iterator.
             */
            Variant operator*() const;

            /**
             * Indirection operator.
             *
             * \see TupleIteratorBase::constReference
             * \see TupleIteratorBase::constPointer
             *
             * \return Returns a pointer to the current value pointed to by the iterator.
             */
            const Variant* operator->() const;

            /**
             * Subtraction operator.
             *
             * \param[in] other The iterator to subtract from this iterator to determine distance.
             *
             * \return Returns an iterator representing the requested position.
             */
            signed long long operator-(const TupleIteratorBase& other) const;

            /**
             * Comparison operator
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if the instance point to the same entry in the tuple.  Returns false if the
             *         instances point to different entries.  Comparing entries in different tuples will lead to
             *         indeterminate results.
             */
            bool operator==(const TupleIteratorBase& other) const;

            /**
             * Comparison operator
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if the instance point to different entries in the tuple.  Returns false if the
             *         instances point to the same entry.  Comparing entries in different tuples will lead to
             *         indeterminate results.
             */
            bool operator!=(const TupleIteratorBase& other) const;

        protected:
            class Private;

            TupleIteratorBase();

            /**
             * Copy constructor
             *
             * \param other The instance to be copied.
             */
            TupleIteratorBase(const TupleIteratorBase& other);

            /**
             * Move constructor
             *
             * \param[in] other The instance to apply to this instance.
             */
            TupleIteratorBase(TupleIteratorBase&& other);

            ~TupleIteratorBase();

            /**
             * Private constructor
             *
             * \param[in] newImplementation The private implementation to assign to this iterator.
             */
            inline TupleIteratorBase(Private* newImplementation):impl(newImplementation) {}

            /**
             * Private implementation.
             */
            Private* impl;
    };
};

#endif
