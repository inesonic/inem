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
* This header defines the \ref Model::Set class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_SET_H
#define MODEL_SET_H

#include "model_common.h"
#include "model_intrinsic_types.h"
#include "model_set_iterator.h"

namespace M {
    class Set;
}

namespace Model {
    class Variant;
    class Range;

    /**
     * Set class.  You can use this class to manage an ordered set of variant values.  To reduce memory footprint, the
     * \ref Model::Set class uses copy-on-write semantics.
     */
    class MODEL_PUBLIC_API Set {
        friend class Variant;
        friend class M::Set;

        public:
            /**
             * Type used to represent a constant iterator.
             */
            typedef SetIterator ConstIterator;

            /**
             * Type used to represent the normal iterator.
             */
            typedef SetIterator Iterator;

            /**
             * Type provided for STL compatibility.
             */
            typedef SetIterator const_iterator;

            /**
             * Type provided for STL compatibility.
             */
            typedef SetIterator iterator;

            Set();

            /**
             * Constructor
             *
             * \param[in] other The instance to be copied from.
             */
            Set(const Variant& other);

            /**
             * Copy constructor.
             *
             * \param[in] other The instance to be copied.
             */
            Set(const Set& other);

            /**
             * Move constructor.
             *
             * \param[in] other The instance to assign to this instance.
             */
            Set(Set&& other);

            ~Set();

            /**
             * Template method you can use to build an empty set.  This method exists to make the variadic template
             * build method work in all cases.
             *
             * \return Returns the newly created set.
             */
            static inline Set build() {
                return Set();
            }

            /**
             * Template method you can use to build a new set with one or more elements.
             *
             * \param[in] elements The elements to be added to the set.
             *
             * \return Returns the newly created set.
             */
            template<typename... E> MODEL_PUBLIC_TEMPLATE_METHOD static Set build(E... elements) {
                Set result;
                result.insert(elements...);

                return result;
            }

            /**
             * Method you can use to determine if the set is empty.
             *
             * \return Returns true if the set is empty.  Returns false if the set is not empty.
             */
            bool isEmpty() const;

            /**
             * Method you can use to determine the current size of the set, in elements.
             *
             * \return Returns the current size, in elements.
             */
            Model::Integer size() const;

            /**
             * Method that clears the set destroying all elements.
             */
            void clear();

            /**
             * Method you can use to insert no elements.  This method exists to make the variadic template insert
             * method work in all cases.
             *
             * \return Returns true.
             */
            inline bool insert() {
                return true;
            }

            /**
             * Method you can use to insert an element into the set.
             *
             * \param[in] value The value to be inserted.
             *
             * \return Returns true if the value has been inserted into the set.  Returns false if the value already
             *         exists in the set and was not inserted.
             */
            bool insert(const Variant& value);

            /**
             * Method you can use to insert a range of values in the set.  Note that this inserts the values
             * represented by the range, not a distinct set defined by the range.
             *
             * \param[in] range The range to be inserted into the set.
             *
             * \return Returns true if all inserted values were new to the set.  Returns false if one or more values
             *         in the range already existed in the set.  Note that the entire range will be inserted even if
             *         some values already reside in the set.
             */
            bool insert(const Range& range);

            /**
             * Template method you can use to insert multiple elements to the set.
             *
             * \param[in] first      The first element to be added to the set.
             *
             * \param[in] second     The second element to be added to the set.
             *
             * \param[in] additional The additional elements to be added to the set.
             *
             * \return Returns true if all elements were inserted.  Returns false if one or more elements were
             *         duplicates.
             */
            template<typename F, typename S, typename... A> MODEL_PUBLIC_TEMPLATE_METHOD bool insert(
                    F    first,
                    S    second,
                    A... additional
                ) {
                bool success = insert(first);
                success = insert(second) && success;
                success = insert(additional...) && success;

                return success;
            }

            /**
             * Method you can use to remove no elements.  This method exists to make the variadic template remove
             * method work in all cases.
             *
             * \return Returns true.
             */
            inline bool remove() {
                return true;
            }

            /**
             * Method you can use to remove an element from the set.
             *
             * \param[in] value The value to be removed.
             *
             * \return Returns true if the value was removed.  Returns false if the value did not exist in the set.
             */
            bool remove(const Variant& value);

            /**
             * Template method you can use to remove multiple elements from the set.
             *
             * \param[in] first      The first element to be remove from the set.
             *
             * \param[in] second     The second element to be removed from the set.
             *
             * \param[in] additional The additional elements to be removed from the set.
             *
             * \return Returns true if all elements were removed.  Returns false if one or more elements were not found
             *         in the set.
             */
            template<typename F, typename S, typename... A> MODEL_PUBLIC_TEMPLATE_METHOD bool remove(
                    F    first,
                    S    second,
                    A... additional
                ) {
                bool success = remove(first);
                success = remove(second) && success;
                success = remove(additional...) && success;

                return success;
            }

            /**
             * Method you can use to determine if a value is contained in the set.
             *
             * \param[in] value The value to check for.
             *
             * \return Returns true if the value exists in the set.  Returns false if the value does not exist in the
             *         set.
             */
            bool contains(const Variant& value) const;

            /**
             * Method that unites this set with another set.
             *
             * \param[in] other The set to unite with this set.
             */
            void uniteWith(const Set& other);

            /**
             * Method that unites this set with another set, returning the union.  Note that this method may be faster
             * than the \ref Model::Set::uniteWith method.
             *
             * \param[in] other The set to unite with this set.
             *
             * \return Returns the union of the two sets.
             */
            Set unitedWith(const Set& other) const;

            /**
             * Method that intersects this set with another set.
             *
             * \param[in] other The set to intersect with this set.
             */
            void intersectWith(const Set& other);

            /**
             * Method that calculates the intersection of this set with another set.
             *
             * \param[in] other The set to unite with this set.
             *
             * \return Returns the union of the two sets.
             */
            Set intersectedWith(const Set& other) const;

            /**
             * Method that subtracts another set from this set.
             *
             * \param[in] other The set to intersect with this set.
             */
            void subtract(const Set& other);

            /**
             * Method that calculates the difference of this set and other set.  The returned set will contain the
             * elements in this set that are not found in the other set.
             *
             * \param[in] other The set to subtract from this set.
             */
            Set difference(const Set& other) const;

            /**
             * Method that calculates the cartesian product of this set and another set.
             *
             * \param[in] other The set to apply as the second parameter in the cartesian product.
             *
             * \return Returns a set containing the cartesian product of the two sets.
             */
            Set cartesianProduct(const Set& other) const;

            /**
             * Method you can use to locate a value in the set.
             *
             * \param[in] value The value to be located.
             *
             * \return Returns an iterator to the desired value.  An iterator to constEnd is returned if the requested
             *         value does not exist in the set.
             */
            ConstIterator find(const Variant& value) const;

            /**
             * Method you can use to locate a value in the set.
             *
             * \param[in] value The value to be located.
             *
             * \return Returns an iterator to the desired value.  An iterator to constEnd is returned if the requested
             *         value does not exist in the set.
             */
            Iterator find(const Variant& value);

            /**
             * Method you can use to obtain a constant iterator to the first entry in the set.
             *
             * \return Returns a constant iterator to the first entry in the set.
             */
            ConstIterator constBegin() const;

            /**
             * Method you can use to obtain a constant iterator just past the last entry in the set.
             *
             * \return Returns a constant iterator just past the last entry in the set.
             */
            ConstIterator constEnd() const;

            /**
             * Method you can use to obtain a constant iterator to the first entry in the set.  This version is
             * provided for STL compatibility.
             *
             * \return Returns a constant iterator to the first entry in the set.
             */
            const_iterator cbegin() const;

            /**
             * Method you can use to obtain a constant iterator just past the last entry in the set.
             *
             * \return Returns a constant iterator just past the last entry in the set.
             */
            const_iterator cend() const;

            /**
             * Method you can use to obtain an iterator to the first entry in the set.
             *
             * \return Returns an iterator to the first entry in the set.
             */
            Iterator begin();

            /**
             * Method you can use to obtain an iterator just past the last entry in the set.
             *
             * \return Returns an iterator just past the last entry in the set.
             */
            Iterator end();

            /**
             * Casting operator.
             *
             * \return Returns true if the set is not empty.  Returns false if the set is empty.
             */
            explicit operator Boolean() const;

            /**
             * Subtraction operator
             *
             * \param[in] other The instance to subtract from this instance.
             *
             * \return Returns a set containing the difference of the two sets.
             */
            inline Set operator-(const Set& other) {
                return difference(other);
            }

            /**
             * Cartesian product operator.
             *
             * \param[in] other The set to use as the second parameter of the cartesian product.
             *
             * \return Returns a set containing the cartesian product of the two sets.  Each element in the set will
             *         be a \ref Model::Tuple containing a pair of values.
             */
            inline Set operator*(const Set& other) const {
                return cartesianProduct(other);
            }

            /**
             * Copy assignment operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            Set& operator=(const Set& other);

            /**
             * Move assignment operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            Set& operator=(Set&& other);

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            Set& operator=(const Variant& other);

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against this instance.
             *
             * \return Returns true if the sets are equivalent.  Returns false if the sets are not equivalent.
             */
            bool operator==(const Set& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns true if the sets are not equivalent.  Returns false if the sets are equivalent.
             */
            bool operator!=(const Set& other) const;

        private:
            class Private;

            /**
             * Private constructor
             *
             * \param[in] newImplementation The new private implementation to assign to this class.
             */
            inline Set(Private* newImplementation):impl(newImplementation) {}

            Private* impl;
    };
};

#endif
