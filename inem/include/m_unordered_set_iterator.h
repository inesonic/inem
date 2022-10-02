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
* This header defines the \ref M::UnorderedSetIterator class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_UNORDERED_SET_ITERATOR_H
#define M_UNORDERED_SET_ITERATOR_H

#include "m_intrinsics.h"
#include "m_unordered_set_base.h"
#include "m_unordered_set_entry.h"

namespace M {
    template<typename T, typename E> class UnorderedSet;

    /**
     * Iterator used to traverse an unordered set.
     *
     * \param[in] T The type to be stored in the set.  T must be constructable, copy constructable, copy assignable,
     *              comparable against equality, and destructable.  The type must also have a supporting hash template
     *              function that calculates a hash on an instance of T and a seed value.
     *
     * \param[in] E The hash table entry instance.
     */
    template<typename T, typename E = UnorderedSetEntry<T>> class UnorderedSetIterator {
        friend class UnorderedSet<T, E>;
        private:
                /**
                 * Constructor
                 *
                 * \param[in] set           The set that this iterator is attached to.
                 *
                 * \param[in] startingIndex The index into the list.
                 */
                UnorderedSetIterator(const UnorderedSet<T, E>* set, UnorderedSetBase::Index startingIndex) {
                    currentSet   = set;
                    currentIndex = startingIndex;
                }

            public:
                UnorderedSetIterator() {
                    currentSet   = nullptr;
                    currentIndex = 0;
                }

                /**
                 * Copy constructor.
                 *
                 * \param[in] other The instance to assign to this instance.
                 */
                UnorderedSetIterator(const UnorderedSetIterator& other) {
                    currentSet   = other.currentSet;
                    currentIndex = other.currentIndex;
                }

                ~UnorderedSetIterator() {}

                /**
                 * Method you can use to obtain the current value pointed to by the iterator.
                 *
                 * \return Returns a reference to the current value.
                 */
                T value() const {
                    return currentSet->hashTable.at(currentIndex).value();
                }

                /**
                 * Method you can use to obtain a reference to the current value pointed to by the iterator.
                 *
                 * \return Returns a reference to the current value.
                 */
                const T& constReference() const {
                    return currentSet->hashTable.at(currentIndex).value();
                }

                /**
                 * Method you can use to obtain a pointer to the current value.
                 *
                 * \return Returns a pointer to the current value.
                 */
                const T* constPointer() const {
                    return &currentSet->hashTable.at(currentIndex).value();
                }

                /**
                 * Method you can use to advance the iterator by one position.
                 */
                void advance() {
                    UnorderedSetBase::Index tableSize = currentSet->hashTable.size();
                    do {
                        ++currentIndex;
                    } while (currentIndex < tableSize && currentSet->hashTable.at(currentIndex).isUnoccupied());
                }

                /**
                 * Method you can use to advance the iterator by one position by a specified distance.
                 *
                 * \param[in] distance The distance to advance the iterator.
                 */
                void advance(unsigned long long distance) {
                    while (isValid() && distance > 0) {
                        advance();
                    }
                }

                /**
                 * Method you can use to determine if this iterator is pointing to a valid location.
                 *
                 * \return Returns true if this iterator is pointing to a valid location in the container.  Returns
                 *         false if the iterator is not initialized or is pointing to an invalid location.
                 */
                bool isValid() const {
                    return currentSet != nullptr && currentIndex != UnorderedSetBase::invalidIndex;
                }

                /**
                 * Method you can use to determine if this iterator is pointing to an invalid location.
                 *
                 * \return Returns true if this iterator is not initialized or is pointing to an invalid location
                 *         in the container.   Returns false if the iterator is pointint to a valid location.
                 */
                bool isInvalid() const {
                    return !isValid();
                }

                /**
                 * Deferencing operator
                 *
                 * \return Returns a reference to the object.
                 */
                inline T operator*() const {
                    return constReference();
                }

                /**
                 * Indirection operator
                 *
                 * \return Returns a pointer to the object.
                 */
                inline const T* operator->() const {
                    return constPointer();
                }

                /**
                 * Pre-increment operator.
                 *
                 * \return Returns a reference to this iterator.
                 */
                UnorderedSetIterator& operator++() {
                    advance();
                    return *this;
                }

                /**
                 * Post-increment operator.
                 *
                 * \return Returns a new iterator pointing to the old location.
                 */
                UnorderedSetIterator operator++(int) {
                    UnorderedSetIterator result = *this;
                    advance();

                    return result;
                }

                /**
                 * Addition assignment operator.
                 *
                 * \param[in] distance The distance to adjust this iterator.
                 *
                 * \return Returns a reference to this iterator.
                 */
                UnorderedSetIterator& operator+=(unsigned long long distance) {
                    advance(distance);
                    return *this;
                }

                /**
                 * Assignment operator.
                 *
                 * \param[in] other The instance to assign to this instance.
                 *
                 * \return Returns a reference to this iterator.
                 */
                UnorderedSetIterator& operator=(const UnorderedSetIterator& other) {
                    currentSet   = other.currentSet;
                    currentIndex = other.currentIndex;

                    return *this;
                }

                /**
                 * Comparison operator.
                 *
                 * \param[in] other The instance to be compared against.
                 *
                 * \return Returns true if the iterators point to the same element.  Returns false if the
                 *         iterators point to different elements.
                 */
                bool operator==(const UnorderedSetIterator& other) const {
                    return currentIndex == other.currentIndex;
                }

                /**
                 * Comparison operator.
                 *
                 * \param[in] other The instance to be compared against.
                 *
                 * \return Returns true if the iterators point to the same element.  Returns false if the
                 *         iterators point to different elements.
                 */
                inline bool operator!=(const UnorderedSetIterator& other) const {
                    return currentIndex != other.currentIndex;
                }

            private:
                /**
                 * The set this iterator is traversing.
                 */
                const UnorderedSet<T, E>* currentSet;

                /**
                 * The current index into the set.
                 */
                UnorderedSetBase::Index currentIndex;
    };
};

#endif
