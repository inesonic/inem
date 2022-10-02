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
* This header defines the \ref M::Tuple class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_TUPLE_H
#define M_TUPLE_H

#include "m_common.h"
#include "model_intrinsic_types.h"
#include "m_list.h"
#include "m_intrinsics.h"
#include "m_intrinsic_types.h"
#include "model_tuple.h"

namespace M {
    class Range;
    class Set;
    class MatrixInteger;
    class MatrixReal;
    class MatrixComplex;

    /**
     * Tuple class.  This class extends the \ref Model::Tuple class to support direct access to the \ref M::Variant
     * objects contained within.  This class is intended to be used internally by the model while the \ref Model::Tuple
     * class is intended to be used by the model API.
     */
    class M_PUBLIC_API Tuple:public Model::Tuple {
        public:
            /**
             * Common base class for the \ref M::Tuple::ConstIterator and \ref M::Tuple::Iterator classes.
             */
            class M_PUBLIC_API IteratorBase {
                public:
                    ~IteratorBase();

                    /**
                     * Method you can use to obtain the current value pointed to by the iterator.  Note that the
                     * \ref M::Tuple::IteratorBase::constReference method may prove to be slightly faster.
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
                     * \return Returns true if this iterator is pointing to a valid entry in the tuple.  Returns false
                     *         if the iterator is not initialized or is pointing to an invalid location.
                     */
                    bool isValid() const;

                    /**
                     * Method you can use to determine if this iterator is pointing to an invalid location.
                     *
                     * \return Returns true if this iterator is pointing to an invalid location or is not initialized.
                     *         Returns false if this iterator is pointing to a valid location in the container.
                     */
                    bool isInvalid() const;

                    /**
                     * Subtraction operator.
                     *
                     * \param[in] other The iterator to subtract from this iterator to determine distance.
                     *
                     * \return Returns an iterator representing the requested position.
                     */
                    signed long long operator-(const IteratorBase& other) const;

                    /**
                     * Comparison operator
                     *
                     * \param[in] other The instance to compare against this instance.
                     *
                     * \return Returns true if the instance point to the same entry in the tuple.  Returns false if the
                     *         instances point to different entries.  Comparing entries in different tuples will lead
                     *         to indeterminate results.
                     */
                    bool operator==(const IteratorBase& other) const;

                    /**
                     * Comparison operator
                     *
                     * \param[in] other The instance to compare against this instance.
                     *
                     * \return Returns true if the instance point to different entries in the tuple.  Returns false if
                     *         the instances point to the same entry.  Comparing entries in different tuples will lead
                     *         to indeterminate results.
                     */
                    bool operator!=(const IteratorBase& other) const;

                    /**
                     * Assignment operator.
                     *
                     * \param[in] other Thei instance to assign to this instance.
                     */
                    IteratorBase& operator=(const IteratorBase& other);

                protected:
                    IteratorBase();

                    /**
                     * Constructor
                     *
                     * \param[in] newTuple The tuple to be traversed.
                     *
                     * \param[in] newIndex The new index into the tuple.
                     */
                    IteratorBase(const Tuple* newTuple, unsigned long newIndex);

                    /**
                     * Copy constructor
                     *
                     * \param[in] other The isntance to be copied.
                     */
                    IteratorBase(const IteratorBase& other);

                    /**
                     * Pointer to the tuple class this iterator is tracking.
                     */
                    const Tuple* currentTuple;

                    /**
                     * The current index into the tuple.
                     */
                    unsigned long currentIndex;
            };

            /**
             * Iterator you can use to traverse a tuple.
             */
            class M_PUBLIC_API ConstIterator:public IteratorBase {
                friend class Tuple;

                public:
                    ConstIterator();

                    /**
                     * Copy constructor
                     *
                     * \param other The instance to be copied.
                     */
                    ConstIterator(const ConstIterator& other);

                    ~ConstIterator();

                    /**
                     * Dereferencing operator.  Note that the Model::Tuple::Iterator::reference method may prove
                     * slightly faster than the dereferencing operator.
                     *
                     * \see Tuple::IteratorBase::value
                     * \see Tuple::IteratorBase::constReference
                     *
                     * \return Returns a reference to the current value pointed to by the iterator.
                     */
                    const Variant& operator*() const;

                    /**
                     * Indirection operator.
                     *
                     * \see Tuple::IteratorBase::constReference
                     * \see Tuple::IteratorBase::constPointer
                     *
                     * \return Returns a pointer to the current value pointed to by the iterator.
                     */
                    const Variant* operator->() const;

                    /**
                     * Pre-increment operator
                     *
                     * \return Returns a reference to this instance.
                     */
                    ConstIterator& operator++();

                    /**
                     * Post-increment operator
                     *
                     * \return Returns an instance representing the state of this iterator prior to being advanced.
                     */
                    ConstIterator operator++(int);

                    /**
                     * Pre-decrement operator
                     *
                     * \return Returns a reference to this instance.
                     */
                    ConstIterator& operator--();

                    /**
                     * Post-decrement operator
                     *
                     * \return Returns an instance representing the state of this iterator prior to being backed up.
                     */
                    ConstIterator operator--(int);

                    /**
                     * Add assignment operator.
                     *
                     * \param[in] distance The distance to advance the iterator.
                     *
                     * \return Returns a reference to this instance.
                     */
                    ConstIterator& operator+=(signed long long distance);

                    /**
                     * Subtract assignment operator.
                     *
                     * \param[in] distance The distance to advance the iterator.
                     *
                     * \return Returns a reference to this instance.
                     */
                    ConstIterator& operator-=(signed long long distance);

                    /**
                     * Addition operator.
                     *
                     * \param[in] distance The distance to add to this iterator to calculated the result.
                     *
                     * \return Returns an iterator representing the requested position.
                     */
                    ConstIterator operator+(signed long long distance) const;

                    /**
                     * Subtraction operator.
                     *
                     * \param[in] distance The distance to apply to this iterator to calculate the result.
                     *
                     * \return Returns an iterator representing the requested position.
                     */
                    ConstIterator operator-(signed long long distance) const;

                    /**
                     * Copy assignment operator.
                     *
                     * \param[in] other The instance to assign to this instance.
                     *
                     * \return Returns a reference to this instance.
                     */
                    ConstIterator& operator=(const ConstIterator& other);

                private:
                    /**
                     * Private constructor
                     *
                     * \param[in] newTuple The tuple we're iterating across.
                     *
                     * \param[in] newIndex The starting index into the tuple.
                     */
                    ConstIterator(const Tuple* newTuple, unsigned long newIndex);
            };

            /**
             * Iterator you can use to traverse a tuple.
             */
            class M_PUBLIC_API Iterator:public IteratorBase {
                friend class Tuple;

                public:
                    Iterator();

                    /**
                     * Copy constructor
                     *
                     * \param other The instance to be copied.
                     */
                    Iterator(const Iterator& other);

                    ~Iterator();

                    /**
                     * Method you can use to obtain a reference to the current value pointed to by the iterator.
                     *
                     * \return Returns a reference to the current value.
                     */
                    Variant& reference() const;

                    /**
                     * Method you can use to obtain a pointer to the current value.
                     *
                     * \return Returns a pointer to the current value.
                     */
                    Variant* pointer() const;

                    /**
                     * Dereferencing operator.
                     *
                     * \see M::Tuple::IteratorBase::value
                     * \see M::Tuple::Iterator::reference
                     *
                     * \return Returns a reference to the current value pointed to by the iterator.
                     */
                    Variant& operator*() const;

                    /**
                     * Indirection operator.
                     *
                     * \see M::Tuple::Iterator::reference
                     * \see M::Tuple::Iterator::pointer
                     *
                     * \return Returns a pointer to the current value pointed to by the iterator.
                     */
                    Variant* operator->() const;

                    /**
                     * Pre-increment operator
                     *
                     * \return Returns a reference to this instance.
                     */
                    Iterator& operator++();

                    /**
                     * Post-increment operator
                     *
                     * \return Returns an instance representing the state of this iterator prior to being advanced.
                     */
                    Iterator operator++(int);

                    /**
                     * Pre-decrement operator
                     *
                     * \return Returns a reference to this instance.
                     */
                    Iterator& operator--();

                    /**
                     * Post-decrement operator
                     *
                     * \return Returns an instance representing the state of this iterator prior to being backed up.
                     */
                    Iterator operator--(int);

                    /**
                     * Add assignment operator.
                     *
                     * \param[in] distance The distance to advance the iterator.
                     *
                     * \return Returns a reference to this instance.
                     */
                    Iterator& operator+=(signed long long distance);

                    /**
                     * Subtract assignment operator.
                     *
                     * \param[in] distance The distance to advance the iterator.
                     *
                     * \return Returns a reference to this instance.
                     */
                    Iterator& operator-=(signed long long distance);

                    /**
                     * Addition operator.
                     *
                     * \param[in] distance The distance to add to this iterator to calculated the result.
                     *
                     * \return Returns an iterator representing the requested position.
                     */
                    Iterator operator+(signed long long distance) const;

                    /**
                     * Subtraction operator.
                     *
                     * \param[in] distance The distance to apply to this iterator to calculate the result.
                     *
                     * \return Returns an iterator representing the requested position.
                     */
                    Iterator operator-(signed long long distance) const;

                    /**
                     * Copy assignment operator.
                     *
                     * \param[in] other The instance to assign to this instance.
                     *
                     * \return Returns a reference to this instance.
                     */
                    Iterator& operator=(const Iterator& other);

                    /**
                     * Move assignment operator.
                     *
                     * \param[in] other The instance to move-assign to this iterator.
                     *
                     * \return Returns a reference to this instance.
                     */
                    Iterator& operator=(Iterator&& other);

                private:
                    /**
                     * Private constructor
                     *
                     * \param[in] newTuple The tuple we're iterating across.
                     *
                     * \param[in] newIndex The starting index into the tuple.
                     */
                    Iterator(Tuple* newTuple, unsigned long newIndex);
            };

            /**
             * Type provided for STL compatibility.
             */
            typedef ConstIterator const_iterator;

            /**
             * Type provided for STL compatibility.
             */
            typedef Iterator iterator;

            Tuple();

            /**
             * Constructor
             *
             * \param[in] other The instance to be copied to this instance.
             */
            Tuple(const Variant& other);

            /**
             * Constructor.  Creates a new tuple from a nul terminated string.  Note that this constructor will throw
             * an exception if the supplied string is not properly UTF-8 encoded.
             *
             * \param[in] str The nul string to be converted to a tuple.  The string is expected to be UTF-8 encoded.
             */
            Tuple(const char* str);

            /**
             * Constructor
             *
             * \param[in] other The instance to be copied.
             */
            Tuple(const Model::Tuple& other);

            /**
             * Move constructor
             *
             * \param[in] other The instance to assign to this instance.
             */
            Tuple(Model::Tuple&& other);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            Tuple(const M::Tuple& other);

            /**
             * Move constructor
             *
             * \param[in] other The instance to assign to this instance.
             */
            Tuple(M::Tuple&& other);

            ~Tuple();

            /**
             * Template method you can use to build an empty tuple.
             *
             * \return Returns the newly created set.
             */
            static inline Tuple build() {
                return Tuple();
            }

            /**
             * Template method you can use to build a new tuple with one or more elements.
             *
             * \param[in] elements The elements to be added to the tuple.
             *
             * \return Returns the newly created tuple.
             */
            template<typename... E> static M_PUBLIC_TEMPLATE_METHOD Tuple build(E... elements) {
                Tuple result;
                result.append(elements...);

                return result;
            }

            /**
             * Method you can use to append no elements.  This method exists to make the variadic template append
             * method work in all cases.
             *
             * \return Returns true.
             */
            inline void append() {}

            /**
             * Method you can use to append a new value to the end of the tuple.
             *
             * \param[in] newValue The value to be appended to the tuple.
             */
            void append(const Variant& newValue);

            /**
             * Method you can use to append a new range to the end of the tuple.
             *
             * \param[in] range The range to be appended to the tuple.
             */
            void append(const Range& range);

            /**
             * Method you can use to append multiple elements to the tuple.
             *
             * \param[in] first      The first element to be added to the tuple.
             *
             * \param[in] second     The second element to be added to the tuple.
             *
             * \param[in] additional The additional elements to be added to the tuple.
             */
            template<typename F, typename S, typename... A> M_PUBLIC_TEMPLATE_METHOD void append(
                    F    first,
                    S    second,
                    A... additional
                ) {
                append(first);
                append(second);
                append(additional...);
            }

            /**
             * Method you can use to prepend no elements.  This method exists to make the variadic template prepend
             * method work in all cases.
             *
             * \return Returns true.
             */
            inline void prepend() {}

            /**
             * Method you can use to prepend a new value to the front of the tuple.
             *
             * \param[in] newValue The value to be prepended to the tuple.
             */
            void prepend(const Variant& newValue);

            /**
             * Method you can use to prepend a new range to the front of the tuple.
             *
             * \param[in] range The range to be prepended to the tuple.
             */
            void prepend(const Range& range);

            /**
             * Method you can use to prepend multiple elements to the tuple.
             *
             * \param[in] first      The first element to be added to the tuple.
             *
             * \param[in] second     The second element to be added to the tuple.
             *
             * \param[in] additional The additional elements to be added to the tuple.
             */
            template<typename F, typename S, typename... A> M_PUBLIC_TEMPLATE_METHOD void prepend(
                    F    first,
                    S    second,
                    A... additional
                ) {
                prepend(first);
                prepend(second);
                prepend(additional...);
            }

            /**
             * Method you can use to append a new value to the end of the tuple.  Supplied for STL compatibility.
             *
             * \param[in] newValue The value to be appended to the tuple.
             */
            void push_back(const Variant& newValue);

            /**
             * Method you can use to prepend a new value to the front of the tuple.  Supplied for STL compatibility.
             *
             * \param[in] newValue The value to be prepended to the tuple.
             */
            void push_front(const Variant& newValue);

            /**
             * Method you can use to append a new range to the end of the tuple.  Supplied for STL compatibility.
             *
             * \param[in] range The value to be appended to the tuple.
             */
            void push_back(const Range& range);

            /**
             * Method you can use to prepend a new range to the front of the tuple.  Supplied for STL compatibility.
             *
             * \param[in] range The range to be prepended to the tuple.
             */
            void push_front(const Range& range);

            /**
             * Method you can use to snoop the first element in the tuple.
             *
             * \return Returns the first element in the tuple.
             */
            Variant first() const;

            /**
             * Method you can use to snoop the last element in the tuple.
             *
             * \return Returns the last element in the tuple.
             */
            Variant last() const;

            /**
             * Method you can use to remove the first element from the tuple, returning the element.
             *
             * \return Returns the removed first element of the tuple.
             */
            Variant takeFirst();

            /**
             * Method you can use to remove the last element from the tuple, returning the element.
             *
             * \return Returns the removed last element of the tuple.
             */
            Variant takeLast();

            /**
             * Method you can use to access a member of the tuple by index.
             *
             * \param[in] index The index into the tuple.
             *
             * \return Returns the variant at the specified location.
             */
            const Variant& at(Integer index) const;

            /**
             * Method you can use to access a member of the tuple by index.
             *
             * \param[in] index The index into the tuple.
             *
             * \return Returns the variant at the specified location.
             */
            const Variant& at(Real index) const;

            /**
             * Method you can use to access a member of the tuple by index.
             *
             * \param[in] index The index into the tuple.
             *
             * \return Returns the variant at the specified location.
             */
            const Variant& at(const Complex& index) const;

            /**
             * Method you can use to access a member of the tuple by index.
             *
             * \param[in] index The index into the tuple.
             *
             * \return Returns a tuple holding the sub-tuple.
             */
            Tuple at(const Range& index) const;

            /**
             * Method you can use to access a member of the tuple by index.
             *
             * \param[in] index The index into the tuple.
             *
             * \return Returns a tuple holding the sub-tuple.
             */
            Tuple at(const Set& index) const;

            /**
             * Method you can use to access a member of the tuple by index.
             *
             * \param[in] index The index into the tuple.
             *
             * \return Returns a tuple holding the sub-tuple.
             */
            Tuple at(const Tuple& index) const;

            /**
             * Method you can use to access a member of the tuple by index.
             *
             * \param[in] index The index into the tuple.
             *
             * \return Returns a tuple holding the sub-tuple.
             */
            Tuple at(const MatrixInteger& index) const;

            /**
             * Method you can use to access a member of the tuple by index.
             *
             * \param[in] index The index into the tuple.
             *
             * \return Returns a tuple holding the sub-tuple.
             */
            Tuple at(const MatrixReal& index) const;

            /**
             * Method you can use to access a member of the tuple by index.
             *
             * \param[in] index The index into the tuple.
             *
             * \return Returns a tuple holding the sub-tuple.
             */
            Tuple at(const MatrixComplex& index) const;

            /**
             * Method you can use to access a member of the tuple by index.
             *
             * \param[in] index The index into the tuple.
             *
             * \return Returns the variant at the specified location.
             */
            Variant at(const Variant& index) const;

            /**
             * Method you can use to update a member of the tuple, by index.
             *
             * \param[in] index    The index into the tuple.  If needed, the tuple will be resized to include this
             *                     index.
             *
             * \param[in] newValue The new value to apply at the specified location.
             */
            void update(unsigned long index, const Variant& newValue);

            /**
             * Method you can use to obtain a constant iterator to the first entry in the range.
             *
             * \return Returns a constant iterator to the first entry in the range.
             */
            ConstIterator constBegin() const;

            /**
             * Method you can use to obtain a constant iterator just past the last entry in the range.
             *
             * \return Returns a constant iterator just past the last entry in the range.
             */
            ConstIterator constEnd() const;

            /**
             * Method you can use to obtain a constant iterator to the first entry in the range.  This version is
             * provided for STL compatibility.
             *
             * \return Returns a constant iterator to the first entry in the range.
             */
            const_iterator cbegin() const;

            /**
             * Method you can use to obtain a constant iterator just past the last entry in the range.  This version is
             * provided for STL compatibility.
             *
             * \return Returns a constant iterator just past the last entry in the range.
             */
            const_iterator cend() const;

            /**
             * Method you can use to obtain an iterator to the first entry in the range.
             *
             * \return Returns an iterator to the first entry in the range.
             */
            Iterator begin();

            /**
             * Method you can use to obtain an iterator just past the last entry in the range.
             *
             * \return Returns an iterator just past the last entry in the range.
             */
            Iterator end();

            /**
             * Method you can call to determine the recommended ordering of this tuple relative to another tuple.
             * Ordering is based on:
             *
             *     * Whether the underlying implementations are the same
             *
             *     * Number of members in the tuple with a smaller number of elements precedes the tuple with a greater
             *       number of members.
             *
             *     * Ordering of each element of the tuple.
             *
             * \param[in] other The other set to compare against.
             *
             * \return Returns a negative value if this tuple should precede the other tuple.  Returns 0 if the
             *         tuples are equal.  Returns a positive value if this tuple should follow the other tuple.
             */
            int relativeOrder(const Tuple& other) const;

            /**
             * Array index operator.
             *
             * \param[in] index The index into the tuple.
             *
             * \return Returns the value at the specified index.
             */
            const Variant& operator[](unsigned long index) const;

            /**
             * Array index operator.  Note that this operator performs an internal copy-on-write operation if there are
             * multiple references to this instance.  This operator may also resize the tuple.
             *
             * \param[in] index The index into the tuple.
             *
             * \return Returns the value at the specified index.
             */
            Variant& operator[](unsigned long index);

            /**
             * Multiplication operator.  The multiplication operator concatenates this tuple against another tuple.
             *
             * \param[in] other The instance to concatenate against this instance.
             *
             * \return Returns a new tuple containing a concatenation of the two tuples.
             */
            Tuple operator*(const Tuple& other) const;

            /**
             * Multiplication operator.  The multiplication operator concatenates this tuple against another tuple.
             *
             * \param[in] other The instance to concatenate against this instance.
             *
             * \return Returns a a reference to this tuple.
             */
            Tuple& operator*=(const Tuple& other);

            /**
             * Division operator.  The division operator performs right cancellation of elements from this tuple.  This
             * operator represents the string obelus operator.
             *
             * \param[in] other The instance to right cancel from this tuple.
             *
             * \return Returns a new tuple containing a right cancelled version of this tuple.
             */
            Tuple operator/(const Tuple& other) const;

            /**
             * Division operator.  The division operator performs right cancellation of elements from this tuple.  This
             * operator represents the string obelus operator.
             *
             * \param[in] other The instance to right cancel from this tuple.
             *
             * \return Returns a a reference to this tuple.
             */
            Tuple& operator/=(const Tuple& other);

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this instance.
             *
             * \return Returns true if the tuples are the same.  Returns false if the tuples are different.
             */
            bool operator==(const Tuple& other) const;

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this instance.
             *
             * \return Returns true if the tuples are the same.  Returns false if the tuples are different.
             */
            inline bool operator!=(const Tuple& other) const {
                return !operator==(other);
            }

            /**
             * Copy assignment operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            Tuple& operator=(const Tuple& other);

            /**
             * Move assignment operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            Tuple& operator=(Tuple&& other);

            /**
             * Move assignment operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            Tuple& operator=(const Variant& other);

        private:
            /**
             * Dummy variant used as a default return value.
             */
            static const Variant dummyVariant;
    };
}

#endif
