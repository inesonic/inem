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
* This header defines the \ref M::ListIterator class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_LIST_CONST_ITERATOR_H
#define M_LIST_CONST_ITERATOR_H

namespace M {
    template<typename T> class List;
    template<typename T> class ListIterator;

    /**
     * The \ref List constant bidirectional iterator class.
     *
     * \param[in] T The type to be stored in the list.  T must be constructable, copy constructable, copy assignable,
     *              and destructable.
     */
    template<typename T> class ListConstIterator {
        friend class ListIterator<T>;
        friend class List<T>;

        public:
            /**
             * Type used to represent an index into the list.
             */
            typedef unsigned long Index;

        private:
            /**
             * Constructor
             *
             * \param[in] list  The list that this iterator is attached to.
             *
             * \param[in] index The index into the list.
             */
            ListConstIterator(const List<T>* list, Index index) {
                currentList  = list;
                currentIndex = index;
            }

        public:
            ListConstIterator() {
                currentList  = nullptr;
                currentIndex = static_cast<Index>(-1);
            }

            /**
             * Copy constructor.
             *
             * \param[in] other The instance to assign to this instance.
             */
            ListConstIterator(const ListConstIterator& other) {
                currentList  = other.currentList;
                currentIndex = other.currentIndex;
            }

            /**
             * Copy constructor.
             *
             * \param[in] other The instance to assign to this instance.
             */
            ListConstIterator(const ListIterator<T>& other) {
                currentList  = other.currentList;
                currentIndex = other.currentIndex;
            }

            ~ListConstIterator() {}

            /**
             * Method you can use to obtain the current value pointed to by the iterator.
             *
             * \return Returns a reference to the current value.
             */
            T value() const {
                return currentList->currentData[currentIndex];
            }

            /**
             * Method you can use to obtain a reference to the current value pointed to by the iterator.
             *
             * \return Returns a reference to the current value.
             */
            const T& constReference() const {
                return currentList->currentData[currentIndex];
            }

            /**
             * Method you can use to obtain a pointer to the current value.
             *
             * \return Returns a pointer to the current value.
             */
            const T* constPointer() const {
                return currentList->currentData+currentIndex;
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
             * Method you can use to advance the iterator by one position.
             */
            void advance() {
                if (currentIndex < currentList->currentSize) {
                    ++currentIndex;
                }
            }

            /**
             * Method you can use to advance the iterator by a specified distance.
             *
             * \param[in] distance The distance to advance the iterator.
             */
            void advance(signed long distance) {
                currentIndex += distance;
                if (currentIndex > currentList->currentSize) {
                    currentIndex = currentList->currentSize;
                }
            }

            /**
             * Method you can call to backup the iterator by one location.
             */
            void backup() {
                if (currentIndex > 0) {
                    --currentIndex;
                } else {
                    currentIndex = currentList->currentSize;
                }
            }

            /**
             * Method you can call to backup the iterator by a specified distance.  The default implementation works by
             * repeatedly calling \ref advance(signed long) with a negative distance.
             *
             * \param[in] distance The distance to reverse the iterator.
             */
            void backup(signed long distance) {
                advance(-distance);
            }

            /**
             * Method you can use to determine if this iterator is pointing to a valid location.
             *
             * \return Returns true if this iterator is pointing to a valid location in th container.  Returns false if
             *         the iterator is not initialized or is pointing to an invalid location.
             */
            bool isValid() const {
                return currentList != nullptr && currentIndex < currentList->currentSize;
            }

            /**
             * Method you can use to determine if this iterator is pointing to an invalid location.
             *
             * \return Returns true if this iterator is not initialized or is pointing to an invalid location in the
             *         container.   Returns false if the iterator is pointint to a valid location.
             */
            bool isInvalid() const {
                return !isValid();
            }

            /**
             * Pre-increment operator.
             *
             * \return Returns a reference to this iterator.
             */
            ListConstIterator& operator++() {
                advance();
                return *this;
            }

            /**
             * Post-increment operator.
             *
             * \return Returns a new iterator pointing to the old location.
             */
            ListConstIterator operator++(int) {
                ListConstIterator result = *this;
                advance();

                return result;
            }

            /**
             * Pre-decrement operator.
             *
             * \return Returns a reference to this iterator.
             */
            ListConstIterator& operator--() {
                backup();
                return *this;
            }

            /**
             * Post-decrement operator.
             *
             * \return Returns a new iterator pointing to the old location.
             */
            ListConstIterator operator--(int) {
                ListConstIterator result = *this;
                backup();

                return result;
            }

            /**
             * Addition assignment operator.
             *
             * \param[in] offset The offset to apply to the list.
             *
             * \return Returns a reference to this iterator.
             */
            ListConstIterator& operator+=(long offset) {
                advance(offset);
                return *this;
            }

            /**
             * Subtraction assignment operator.
             *
             * \param[in] offset The offset to apply to the list.
             *
             * \return Returns a reference to this iterator.
             */
            ListConstIterator& operator-=(long offset) {
                backup(offset);
                return *this;
            }

            /**
             * Addition operator.
             *
             * \param[in] offset The offset to apply to the list;
             *
             * \return Returns a new, adjusted iterator.
             */
            ListConstIterator operator+(long offset) const {
                ListConstIterator result = *this;
                result.advance(offset);

                return result;
            }

            /**
             * Subtraction operator.
             *
             * \param[in] offset The offset to apply to the list;
             *
             * \return Returns a new, adjusted iterator.
             */
            ListConstIterator operator-(long offset) const {
                ListConstIterator result = *this;
                result.backup(offset);

                return result;
            }

            /**
             * Addition operator.
             *
             * \param[in] other The other iterator to adjust for.
             *
             * \return Returns a new, adjusted iterator.
             */
            long operator+(const ListConstIterator& other) const {
                return currentIndex + other.currentIndex;
            }

            /**
             * Subtraction operator.
             *
             * \param[in] other The other iterator to adjust for.
             *
             * \return Returns a new, adjusted iterator.
             */
            long operator-(const ListConstIterator& other) const {
                return currentIndex - other.currentIndex;
            }

            /**
             * Addition operator.
             *
             * \param[in] other The other iterator to adjust for.
             *
             * \return Returns a new, adjusted iterator.
             */
            long operator+(const ListIterator<T>& other) const {
                return currentIndex + other.currentIndex;
            }

            /**
             * Subtraction operator.
             *
             * \param[in] other The other iterator to adjust for.
             *
             * \return Returns a new, adjusted iterator.
             */
            long operator-(const ListIterator<T>& other) const {
                return currentIndex - other.currentIndex;
            }

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this iterator.
             */
            ListConstIterator& operator=(const ListConstIterator<T>& other) {
                currentList  = other.currentList;
                currentIndex = other.currentIndex;

                return *this;
            }

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this iterator.
             */
            ListConstIterator& operator=(const ListIterator<T>& other) {
                currentList  = other.currentList;
                currentIndex = other.currentIndex;

                return *this;
            }

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the iterators point to the same element.  Returns false if the iterators point
             *         to different elements.
             */
            bool operator==(const ListConstIterator& other) const {
                return other.currentIndex == currentIndex;
            }

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the iterators point to the same element.  Returns false if the iterators point
             *         to different elements.
             */
            inline bool operator!=(const ListConstIterator& other) const {
                return other.currentIndex != currentIndex;
            }

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the iterators point to the same element.  Returns false if the iterators point
             *         to different elements.
             */
            bool operator==(const ListIterator<T>& other) const {
                return other.currentIndex == currentIndex;
            }

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the iterators point to the same element.  Returns false if the iterators point
             *         to different elements.
             */
            inline bool operator!=(const ListIterator<T>& other) const {
                return other.currentIndex != currentIndex;
            }

        private:
            /**
             * The list being traversed by this iterator.
             */
            const List<T>* currentList;

            /**
             * The current index into the list.
             */
            Index currentIndex;
    };
};

#endif
