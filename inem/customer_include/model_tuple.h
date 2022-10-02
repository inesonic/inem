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
* This header defines the \ref Model::Tuple class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_TUPLE_H
#define MODEL_TUPLE_H

#include "model_common.h"
#include "model_intrinsic_types.h"
#include "model_tuple_iterator_base.h"
#include "model_tuple_const_iterator.h"
#include "model_tuple_iterator.h"

namespace M {
    class MODEL_PUBLIC_API Tuple;
}

namespace Model {
    class MODEL_PUBLIC_API Variant;
    class MODEL_PUBLIC_API TupleIteratorBase;
    class MODEL_PUBLIC_API Range;

    /**
     * Tuple class.  You can use this class to store ordered sets of variant types.
     */
    class MODEL_PUBLIC_API Tuple {
        friend class MODEL_PUBLIC_API TupleIteratorBase;
        friend class MODEL_PUBLIC_API Variant;
        friend class MODEL_PUBLIC_API M::Tuple;

        public:
            /**
             * Type used as the index into this object.
             */
            typedef Integer Index;

            /**
             * Type used to represent a constant iterator.
             */
            typedef TupleConstIterator ConstIterator;

            /**
             * Type used to represent the normal iterator.
             */
            typedef TupleIterator Iterator;

            /**
             * Type provided for STL compatibility.
             */
            typedef TupleConstIterator const_iterator;

            /**
             * Type provided for STL compatibility.
             */
            typedef TupleIterator iterator;

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
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            Tuple(const Tuple& other);

            /**
             * Move constructor
             *
             * \param[in] other The instance to assign to this instance.
             */
            Tuple(Tuple&& other);

            ~Tuple();

            /**
             * Template method you can use to build an empty tuple.  Exists to allow the variadic template build method
             * work in all cases.
             *
             * \return Returns the newly created set.
             */
            static Tuple build();

            /**
             * Template method you can use to build a new tuple with one or more elements.
             *
             * \param[in] elements The elements to be added to the tuple.
             *
             * \return Returns the newly created tuple.
             */
            template<typename... E> static MODEL_PUBLIC_TEMPLATE_METHOD Tuple build(E... elements) {
                Tuple result;
                result.append(elements...);

                return result;
            }

            /**
             * Method you can use to access the value at a specified one based index.  This method will throw an
             * exception if the specified index is invalid.
             *
             * \param[in] index The one based index of the coefficient to be accessed.
             *
             * \return Returns a variant value holding the contents at the specified location.  An empty variant will
             *         be returned if the index is invalid.
             */
            Variant value(Index index) const;

            /**
             * Method you can use to set a value by position.
             *
             * \param[in] index The one base index of the coefficient to be accessed.  New entries will be added if
             *                  the index is outside of the current length of the tuple.
             *
             * \param[in] value The value to assign to the specified location.
             *
             * \return This method returns true if the index is valid.  Returns false if the index is invalid.
             */
            bool setValue(Index index, const Variant& value);

            /**
             * Method you can use to determine if the Tuple is empty.
             *
             * \return Returns true if the tuple is empty.  Returns false if the tuple is not empty.
             */
            bool isEmpty() const;

            /**
             * Method you can use to determine the size of the tuple, in elements.
             *
             * \return Returns the current size of the tuple, in elements.
             */
            Index size() const;

            /**
             * Method that clears the tuple.
             */
            void clear();

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
            template<typename F, typename S, typename... A> MODEL_PUBLIC_TEMPLATE_METHOD void append(
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
             * Method you can use to prepend a new range to the end of the tuple.  The range will be prepended in
             * reverse order.  Inserting the range \f[ \left ( 1, 3, \ldots, 7 \right ) \f] will result in the
             * first four tuple elements being assigned as: 7, 5, 3, 1.
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
            template<typename F, typename S, typename... A> MODEL_PUBLIC_TEMPLATE_METHOD void prepend(
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
             * \param[in] range The range to be appended to the tuple.
             */
            void push_back(const Range& range);

            /**
             * Method you can use to prepend a new range to the front of the tuple.  Supplied for STL compatibility.
             * The range will be prepended in reverse order.  Inserting the range
             * \f[ \left ( 1, 3, \ldots, 7 \right ) \f] will result in the first four tuple elements being assigned
             * as: 7, 5, 3, 1.
             *
             * \param[in] range The range to be prepended to the tuple.
             */
            void push_front(const Range& range);

            /**
             * Method you can use to peek at the first element in the tuple.
             *
             * \return Returns the first element in the tuple.
             */
            Variant first() const;

            /**
             * Method you can use to peek at the last element in the tuple.
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
             * Method you can use to remove the first element from the tuple, returning the element.
             *
             * \return Returns true if the tuple was not empty.  Returns false if the tuple was empty.
             */
            bool popFirst();

            /**
             * Method you can use to remove the last element from the tuple, returning the element.
             *
             * \return Returns true if the tuple was not empty.  Returns false if the tuple was empty.
             */
            bool popLast();

            /**
             * Method you can use to remove the first element from the tuple, returning the element.  Supplied for
             * STL compatibility.
             */
            void pop_front();

            /**
             * Method you can use to remove the last element from the tuple, returning the element.  Supplied for
             * STL compatibility.
             */
            void pop_back();

            /**
             * Method you can use to access a member of the tuple by index.
             *
             * \param[in] index The one based index into the tuple.  This method will throw an exception if the index
             *                  is invalid.
             *
             * \return Returns the variant at the specified location.
             */
            Variant at(Index index) const;

            /**
             * Method you can use to update a member of the tuple, by index.
             *
             * \param[in] index    The one based index into the tuple.  If needed, the tuple will be resized to include
             *                     this index.  This method will throw an exception if the index is invalid.
             *
             * \param[in] newValue The new value to apply at the specified location.
             */
            void update(Index index, const Variant& newValue);

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
             * Method you can use to convert this tuple back into a UTF-8 compatible string.
             *
             * \return Returns a nul terminated character array holding the converted string.  A null pointer is
             *         returned if the tuple can-not be converted to a UTF-8 compatible string.  You are expected to
             *         take ownership of the string and delete it at end of scope.
             */
            char* toString() const;

            /**
             * Casting operator.
             *
             * \return Returns true if the tuple is not empty.  Returns false if the tuple is not empty.
             */
            explicit operator Boolean() const;

            /**
             * Array index operator.
             *
             * \param[in] index The one based index into the tuple.  This operator will throw an exception if the
             *                  index is invalid.
             *
             * \return Returns the value at the specified index.
             */
            Variant operator()(Index index) const;

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

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against this instance.
             *
             * \return Returns true if the tuples are equivalent.  Returns false if the tuples are not equivalent.
             */
            bool operator==(const Tuple& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns true if the tuples are not equivalent.  Returns false if the tuples are equivalent.
             */
            bool operator!=(const Tuple& other) const;

        private:
            class Private;

            /**
             * Private constructor
             *
             * \param[in] newImplementation The new private implementation to assign to this class.
             */
            inline Tuple(Private* newImplementation):impl(newImplementation) {}

            Private* impl;
    };
}

#endif
