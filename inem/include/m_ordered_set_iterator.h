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
* This header defines the \ref M::OrderedSetIterator class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_ORDERED_SET_ITERATOR_H
#define M_ORDERED_SET_ITERATOR_H

#include "m_intrinsics.h"
#include "m_list.h"
#include "m_unordered_set_base.h"
#include "m_ordered_set_compare_function.h"
#include "m_unordered_set.h"
#include "m_ordered_set_entry.h"

namespace M {
    template<typename T, typename E, int C(const T&, const T&)> class OrderedSet;

    /**
     * An ordered set template class iterator.  This class provides a forward iterator you can use to traverse
     * \ref OrderedSet instances.
     *
     * \param[in] T The type to be stored in the set.  T must be constructable, copy constructable, copy assignable,
     *              comparable against equality and less-than.  T must also be destructable.  The type must also have a
     *              supporting hash template function that calculates a hash on an instance of T and a seed value.
     *
     * \param[in] E The hash table entry instance.
     *
     * \param[in] C The entry comparison functor.
     */
    template<
        typename T,
        typename E = OrderedSetEntry<T>,
        int C(const T&, const T&) = OrderedSetCompareFunction<T>
    > class OrderedSetIterator {
        friend class OrderedSet<T, E, C>;

        public:
            /**
             * The index class.
             */
            typedef UnorderedSetBase::Index Index;

        private:
            /**
             * Constructor
             *
             * \param[in] set       The set that this iterator is attached to.
             *
             * \param[in] index     The index into the list.
             *
             * \param[in] goToFirst If true, the iterator should reset itself to the first entry in entry
             *                      order.  If false, the specified index will be used.
             */
            OrderedSetIterator(const OrderedSet<T, E, C>* set, UnorderedSetBase::Index index, bool goToFirst) {
                currentSet = nullptr;
                connectToSet(set);

                currentIndex = goToFirst ? findLowestUnderIndex(set->storage(), index) : index;
            }

        public:
            OrderedSetIterator() {
                currentSet   = nullptr;
                currentIndex = UnorderedSetBase::invalidIndex;
            }

            /**
             * Copy constructor.
             *
             * \param[in] other The instance to assign to this instance.
             */
            OrderedSetIterator(const OrderedSetIterator& other) {
                currentSet = nullptr;
                connectToSet(other.currentSet);
                currentIndex = other.currentIndex;
            }

            ~OrderedSetIterator() {
                if (currentSet != nullptr) {
                    currentSet->currentIterators.remove(this);
                }
            }

            /**
             * Method you can use to obtain the current value pointed to by the iterator.
             *
             * \return Returns a reference to the current value.
             */
            T value() const {
                return currentSet->storage().at(currentIndex).value();
            }

            /**
             * Method you can use to obtain a reference to the current value pointed to by the iterator.
             *
             * \return Returns a reference to the current value.
             */
            const T& constReference() const {
                return currentSet->storage().at(currentIndex).value();
            }

            /**
             * Method you can use to obtain a pointer to the current value.
             *
             * \return Returns a pointer to the current value.
             */
            const T* constPointer() const {
                return &currentSet->storage().at(currentIndex).value();
            }

            /**
             * Method you can use to advance the iterator by one position.
             */
            void advance() {
                if (currentIndex != UnorderedSetBase::invalidIndex && currentSet != nullptr) {
                    const List<E>& hashTable = currentSet->storage();
                    const E&       entry     = hashTable.at(currentIndex);

                    Index rightChildIndex = entry.rightChildIndex();
                    if (rightChildIndex != UnorderedSetBase::invalidIndex) {
                        currentIndex = findLowestUnderIndex(hashTable, rightChildIndex);
                    } else {
                        Index    parentIndex = entry.parentIndex();
                        Index    childIndex  = currentIndex;
                        const E* parent;

                        // NOTE: assignment in loop.
                        while (parentIndex != UnorderedSetBase::invalidIndex                          &&
                               childIndex == (parent = &hashTable.at(parentIndex))->rightChildIndex()    ) {
                            childIndex  = parentIndex;
                            parentIndex = parent->parentIndex();
                        }

                        currentIndex = parentIndex;
                    }
                }
            }

            /**
             * Method you can use to advance the iterator by one position by a specified distance.
             *
             * \param[in] distance The distance to advance the iterator.
             */
            void advance(unsigned long long distance) {
                while (isValid() && distance > 0) {
                    advance();
                    --distance;
                }
            }

            /**
             * Method you can use to determine if this iterator is pointing to a valid location.
             *
             * \return Returns true if this iterator is pointing to a valid location in th container.  Returns
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
            OrderedSetIterator& operator++() {
                advance();
                return *this;
            }

            /**
             * Post-increment operator.
             *
             * \return Returns a new iterator pointing to the old location.
             */
            OrderedSetIterator operator++(int) {
                OrderedSetIterator result = *this;
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
            OrderedSetIterator& operator+=(unsigned long long distance) {
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
            OrderedSetIterator& operator=(const OrderedSetIterator& other) {
                connectToSet(other.currentSet);
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
            bool operator==(const OrderedSetIterator& other) const {
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
            inline bool operator!=(const OrderedSetIterator& other) const {
                return currentIndex != other.currentIndex;
            }

        private:
            /**
             * Method that is called to connect to a new ordered set.
             *
             * \param[in] newOrderedSet The new set to connect to.
             */
            void connectToSet(const OrderedSet<T, E, C>* newOrderedSet) {
                if (currentSet != newOrderedSet) {
                    if (newOrderedSet != nullptr) {
                        newOrderedSet->currentIterators.insert(this);
                    }

                    if (currentSet != nullptr) {
                        currentSet->currentIterators.remove(this);
                    }

                    currentSet = newOrderedSet;
                }
            }

            /**
             * Method that is called to set the iterator to the lowest valued entry from a given location.
             *
             * \param[in] hashTable    The hash table holding the entries to traverse.
             *
             * \param[in] currentIndex The index to start the search from.
             *
             * \return Returns the index of the lowest valued entry.
             */
            Index findLowestUnderIndex(const List<E>& hashTable, Index currentIndex) {
                Index lowestIndex = currentIndex;
                if (lowestIndex != UnorderedSetBase::invalidIndex) {
                    Index nextIndex = currentIndex;
                    do {
                        const E& entry = hashTable.at(nextIndex);
                        lowestIndex = nextIndex;
                        nextIndex = entry.leftChildIndex();
                    } while (nextIndex != UnorderedSetBase::invalidIndex);
                }

                return lowestIndex;
            }

            /**
             * The set we are currently traversing.
             */
            const OrderedSet<T, E, C>* currentSet;

            /**
             * The current index into the ordered set.
             */
            UnorderedSetBase::Index currentIndex;

    };
};

#endif
