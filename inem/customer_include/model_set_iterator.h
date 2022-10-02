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
* This header defines the \ref Model::SetIterator class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_SET_ITERATOR_H
#define MODEL_SET_ITERATOR_H

#include "model_common.h"

namespace Model {
    class Variant;
    class Set;

    /**
     * Iterator you can use to traverse a set.
     */
    class MODEL_PUBLIC_API SetIterator {
        friend class Set;

        public:
            SetIterator();

            /**
             * Copy constructor
             *
             * \param other The instance to be copied.
             */
            SetIterator(const SetIterator& other);

            /**
             * Move constructor
             *
             * \param[in] other The instance to apply to this instance.
             */
            SetIterator(SetIterator&& other);

            ~SetIterator();

            /**
             * Method you can use to obtain the current value pointed to by the iterator.  Note that the
             * \ref Model::SetIterator::constReference method may prove to be slightly faster.
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
            void advance(unsigned long distance);

            /**
             * Method you can use to determine if this iterator is pointing to a valid entry.
             *
             * \return Returns true if this iterator is pointing to a valid entry in the set.  Returns false if the
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
             * Dereferencing operator.  Note that the \ref SetIterator::constReference method may prove slightly faster
             * than the dereferencing operator.
             *
             * \see SetIterator::value
             * \see SetIterator::constReference
             *
             * \return Returns a reference to the current value pointed to by the iterator.
             */
            Variant operator*() const;

            /**
             * Indirection operator.
             *
             * \see Model::SetIterator::constReference
             * \see Model::SetIterator::constPointer
             *
             * \return Returns a pointer to the current value pointed to by the iterator.
             */
            const Variant* operator->() const;

            /**
             * Pre-increment operator
             *
             * \return Returns a reference to this instance.
             */
            SetIterator& operator++();

            /**
             * Post-increment operator
             *
             * \return Returns an instance representing the state of this iterator prior to being advanced.
             */
            SetIterator operator++(int);

            /**
             * Add assignment operator.
             *
             * \param[in] distance The distance to advance the iterator.
             *
             * \return Returns a reference to this instance.
             */
            SetIterator& operator+=(unsigned long distance);

            /**
             * Copy assignment operator.
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this instance.
             */
            SetIterator& operator=(const SetIterator& other);

            /**
             * Move assignment operator.
             *
             * \param[in] other The instance to move-assign to this iterator.
             *
             * \return Returns a reference to this instance.
             */
            SetIterator& operator=(SetIterator&& other);

            /**
             * Comparison operator
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if the instance point to the same entry in the set.  Returns false if the instances
             *         point to different entries.  Comparing entries in different sets will lead to indeterminate
             *         results.
             */
            bool operator==(const SetIterator& other) const;

            /**
             * Comparison operator
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if the instance point to different entries in the set.  Returns false if the
             *         instances point to the same entry.  Comparing entries in different sets will lead to
             *         indeterminate results.
             */
            bool operator!=(const SetIterator& other) const;

        private:
            class Private;

            /**
             * Private constructor
             *
             * \param[in] newImplementation The private implementation to assign to this iterator.
             */
            inline SetIterator(Private* newImplementation):impl(newImplementation) {};

            Private* impl;
    };
};

#endif
