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
* This header defines the \ref M::List class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_LIST_H
#define M_LIST_H

#include "m_list_const_iterator.h"
#include "m_list_iterator.h"

namespace M {
    /**
     * List template class.  This class provides support for a trivial type-safe array of elements similar to that
     * provided by stl::vector.
     *
     * \param[in] T The type to be stored in the list.  T must be constructable, copy constructable, copy assignable,
     *              and destructable.
     */
    template<typename T> class List {
        friend class ListIterator<T>;
        friend class ListConstIterator<T>;

        public:
            /**
             * Type used to represent an index into the list.
             */
            typedef unsigned long Index;

            /**
             * Iterator that can be used to traverse the list.
             */
            typedef ListIterator<T> Iterator;

            /**
             * Constant iterator that can be used to traverse the list.
             */
            typedef ListConstIterator<T> ConstIterator;

            /**
             * Type representing the type managed by this list.
             */
            typedef T ValueType;

            /**
             * Type provided for STL compatibility.
             */
            typedef Iterator iterator;

            /**
             * Type provided for STL compatibility.
             */
            typedef ConstIterator const_iterator;

            /**
             * Type provided for STL compatibility.
             */
            typedef ValueType value_type;

            List() {
                currentData           = new T[1]();
                currentSize           = 0;
                currentAllocationSize = 1;
            }

            /**
             * Constructor
             *
             * \param[in] size The required size of the list, in elements.  Elements will be default constructed.
             */
            List(Index size) {
                currentData           = new T[size]();
                currentSize           = size;
                currentAllocationSize = size;
            }

            /**
             * Constructor
             *
             * \param[in] size  The required size of the list, in elements.
             *
             * \param[in] value The copy constructed value.
             */
            List(Index size, const T& value) {
                currentData = new T[size]();
                for (unsigned i=0 ; i<size ; ++i) {
                    currentData[i] = value;
                }

                currentSize           = size;
                currentAllocationSize = size;
            }

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            List(const List<T>& other) {
                currentData           = createPool(other.currentData, other.currentSize, other.currentAllocationSize);
                currentSize           = other.currentSize;
                currentAllocationSize = other.currentAllocationSize;
            }

            /**
             * Copy constructor (move semantics)
             *
             * \param[in] other The instance to be copied.
             */
            List(List<T>&& other) {
                currentData           = other.currentData;
                currentSize           = other.currentSize;
                currentAllocationSize = other.currentAllocationSize;

                other.currentData = nullptr;
            }

            ~List() {
                delete[] currentData;
            }

            /**
             * Method you can use to determine if the list is empty.
             *
             * \return Returns true if the list is empty.  Returns false if the list is not empty.
             */
            bool isEmpty() const {
                return currentSize == 0;
            }

            /**
             * Method you can use to determine if the list is *not* empty.
             *
             * \return Returns true if the list is not empty.  Returns false if the list is empty.
             */
            bool isNotEmpty() const {
                return !isEmpty();
            }

            /**
             * Method you can use to determine the current size of the list, in elements.
             *
             * \return Returns the current size, in elements.
             */
            Index size() const {
                return currentSize;
            }

            /**
             * Method you can use to determine the current capacity of the list, in elements.
             *
             * \return Returns the current capacity, in elements.
             */
            Index capacity() const {
                return currentAllocationSize;
            }

            /**
             * Method that clears the list, destroying all elements.
             */
            void clear() {
                delete[] currentData;

                currentData           = new T[1]();
                currentSize           = 0;
                currentAllocationSize = 1;
            }

            /**
             * Method you can use to pre-allocate a specified amount of space for the list.
             *
             * \param[in] newReservedSpace The new space to reserve, in elements.  If the new reservation is less than
             *                             the current size of the list, the list will be shrunk to match the reserved
             *                             space.
             */
            void reserve(Index newReservedSpace) {
                if (newReservedSpace < currentSize) {
                    currentSize = newReservedSpace;
                }

                T* newData = new T[newReservedSpace]();
                for (unsigned i=0 ; i<currentSize ; ++i) {
                    newData[i] = currentData[i];
                }

                delete[] currentData;
                currentData = newData;

                currentAllocationSize = newReservedSpace;
            }

            /**
             * Method you can use to resize the list to a requested size.
             *
             * \param[in] newSize The new size for the list, in elements.
             */
            void resize(Index newSize) {
                if (newSize > currentSize) {
                    T* newData = createPool(currentData, currentSize, newSize);

                    delete[] currentData;
                    currentData = newData;
                    currentAllocationSize = newSize;
                }

                currentSize = newSize;
            }

            /**
             * Method you can use to shrink the list to perfectly fit the contained data.
             */
            void optimize() {
                T* newData = createPool(currentData, currentSize, currentSize);
                delete[] currentData;

                currentData           = newData;
                currentAllocationSize = currentSize;
            }

            /**
             * Method you can use to swap this list's contents with another list.
             *
             * \param[in] other The list to swap contents against this list.
             */
            void swap(List<T>& other) {
                T*    temporaryData           = other.currentData;
                Index temporaryAllocationSize = other.currentAllocationSize;
                Index temporarySize           = other.currentSize;

                other.currentData           = currentData;
                other.currentAllocationSize = currentAllocationSize;
                other.currentSize           = currentSize;

                currentData           = temporaryData;
                currentAllocationSize = temporaryAllocationSize;
                currentSize           = temporarySize;
            }

            /**
             * Method you can use to obtain an entry from the list.
             *
             * \param[in] index The zero based index into the list.
             *
             * \return Returns a reference to the requested instance.
             */
            const T& at(Index index) const {
                return currentData[index];
            }

            /**
             * Method you can use to obtain an entry from the list.
             *
             * \param[in] index The zero based index into the list.
             *
             * \return Returns a reference to the requested instance.
             */
            T& at(Index index) {
                return currentData[index];
            }

            /**
             * Method you can use to snoop the first element of the list.
             *
             * \return Returns a reference to the first element in the list.
             */
            const T& first() const {
                return currentData[0];
            }

            /**
             * Method you can use to snoop the first element of the list.
             *
             * \return Returns a reference to the first element in the list.
             */
            T& first() {
                return currentData[0];
            }

            /**
             * Method you can use to snoop the last element of the list.
             *
             * \return Returns a reference to the last element in the list.
             */
            const T& last() const {
                return currentData[currentSize == 0 ? 0 : currentSize - 1];
            }

            /**
             * Method you can use to snoop the last element of the list.
             *
             * \return Returns a reference to the last element in the list.
             */
            T& last() {
                return currentData[currentSize == 0 ? 0 : currentSize - 1];
            }

            /**
             * Method you can use to pop the last element of the list, reducing the size of the list by 1.
             *
             * \return Returns a copy of the removed element.  A default constructed value will be returned if the list
             *         is empty.
             */
            T takeLast() {
                T result;

                if (currentSize > 0) {
                    --currentSize;
                    result = currentData[currentSize];
                } else {
                    result = T();
                }

                return result;
            }

            /**
             * Method you can use to pop the last element of the list, reducing the size of the list by 1.  Method
             * provided for STL compatibility.
             *
             * \return Returns a copy of the removed element.  A default constructed value will be returned if the list
             *         is empty.
             */
            T pop_back() {
                return takeLast();
            }

            /**
             * Method you can use to pop the first element of the list, reducing the size of the list by 1.
             *
             * \return Returns a copy of the removed element.  A default constructed value will be returned if the list
             *         is empty.
             */
            T takeFirst() {
                T result;

                if (currentSize > 0) {
                    result = currentData[0];
                    for (unsigned index=0 ; index<currentSize ; ++index) {
                        currentData[index] = currentData[index+1];
                    }

                    --currentSize;
                } else {
                    result = T();
                }

                return result;
            }

            /**
             * Method you can use to pop the first element of the list, reducing the size of the list by 1.  Method
             * provided for STL compatibility.
             *
             * \return Returns a copy of the removed element.  A default constructed value will be returned if the list
             *         is empty.
             */
            T pop_front() {
                return takeLast();
            }

            /**
             * Method you can use to push an entry onto the end of the array.
             *
             * \param[in] value The value to be appended to the end of the array.
             */
            void append(const T& value) {
                if (currentSize >= currentAllocationSize) {
                    currentAllocationSize *= 2;
                    T* newData = createPool(currentData, currentSize, currentAllocationSize);

                    delete[] currentData;
                    currentData = newData;
                }

                currentData[currentSize] = value;
                ++currentSize;
            }

            /**
             * Method you can use to push an entry onto the front of the array.
             *
             * \param[in] value The value to be appended to the end of the array.
             */
            void prepend(const T& value) {
                if (currentSize >= currentAllocationSize) {
                    currentAllocationSize *= 2;
                    T* newData = createPool(currentData, currentSize, currentAllocationSize);

                    delete[] currentData;
                    currentData = newData;
                }

                for (unsigned long index=currentSize ; index>0 ; --index) {
                    currentData[index] = currentData[index-1];
                }

                currentData[0] = value;
                ++currentSize;
            }

            /**
             * Method you can use to push an entry onto the end of the array.  This method is provided for STL
             * compatibility.
             *
             * \param[in] value The value to be appended to the end of the array.
             */
            void push_back(const T& value) {
                append(value);
            }

            /**
             * Method you can use to obtain an iterator to the first entry in the list.
             *
             * \return Returns an iterator to the first entry in the list.
             */
            Iterator begin() {
                return Iterator(this, 0);
            }

            /**
             * Method you can use to obtain an iterator just past the last entry in the list.
             *
             * \return Returns an iterator just past the last entry in the list.
             */
            Iterator end() {
                return Iterator(this, currentSize);
            }

            /**
             * Method you can use to obtain an iterator to the first entry in the list.
             *
             * \return Returns an iterator to the first entry in the list.
             */
            ConstIterator constBegin() const {
                return ConstIterator(this, 0);
            }

            /**
             * Method you can use to obtain an iterator just past the last entry in the list.
             *
             * \return Returns an iterator just past the last entry in the list.
             */
            ConstIterator constEnd() const {
                return ConstIterator(this, currentSize);
            }

            /**
             * Array operator.
             *
             * \param[in] index The zero based index of the desired entry.
             *
             * \return Returns a reference to the requested entry.
             */
            T& operator[](Index index) {
                return currentData[index];
            }

            /**
             * Array operator.
             *
             * \param[in] index The zero based index of the desired entry.
             *
             * \return Returns a reference to the requested entry.
             */
            const T& operator[](Index index) const {
                return currentData[index];
            }

            /**
             * Copy assignment operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            List<T>& operator=(const List<T>& other) {
                delete[] currentData;

                currentData           = createPool(other.currentData, other.currentSize, other.currentAllocationSize);
                currentSize           = other.currentSize;
                currentAllocationSize = other.currentAllocationSize;

                return *this;
            }

            /**
             * Move assignment operator.
             *
             * \param[in] other The instance to be assigned to this instance.
             *
             * \return Returns a reference to this instance.
             */
            List<T>& operator=(List<T>&& other) {
                delete[] currentData;

                currentData           = other.currentData;
                currentSize           = other.currentSize;
                currentAllocationSize = other.currentAllocationSize;

                other.currentData = nullptr;

                return *this;
            }

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared to this instance..
             *
             * \return Returns true if the lists are the same size and all elements are equal.  Returns false if the
             *         lists are a different size or one or more elements are not equal.
             */
            bool operator==(const List<T>& other) const {
                bool isEqual = true;

                if (currentSize == other.currentSize) {
                    unsigned long index = 0;
                    while (isEqual && index<currentSize) {
                        isEqual = currentData[index] == other.currentData[index];
                        ++index;
                    }
                } else {
                    isEqual = false;
                }

                return isEqual;
            }

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared to this instance..
             *
             * \return Returns true if the lists are of a different size or if any elements are not equal.  Returns
             *         false if both lists are the same size and all elements are equal.
             */
            bool operator!=(const List<T>& other) const {
                return !operator==(other);
            }

        private:
            /**
             * Method used to create a new data pool with a new size.
             *
             * \param[in] currentPool       The pool to be copied over.
             *
             * \param[in] currentSize       The number of elements in the current pool.
             *
             * \param[in] newAllocationSize The new allocated size for the pool, in elements.
             *
             * \return Returns a pointer to the newly created pool.
             */
            static T* createPool(const T* currentPool, Index currentSize, Index newAllocationSize) {
                T* result = new T[newAllocationSize]();

                if (currentPool != nullptr) {
                    Index count = currentSize <= newAllocationSize ? currentSize : newAllocationSize;
                    for (Index i=0 ; i<count ; ++i) {
                        result[i] = currentPool[i];
                    }
                }

                return result;
            }

            /**
             * Pointer to the underlying list data.
             */
            T* currentData;

            /**
             * The current allocated size, in entries.
             */
            Index currentAllocationSize;

            /**
             * The current list size, in entries.
             */
            Index currentSize;
    };
};

#endif
