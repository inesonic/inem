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
* This header defines the \ref M::Set class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_SET_H
#define M_SET_H

#include "m_common.h"
#include "model_intrinsic_types.h"
#include "model_variant.h"
#include "m_ordered_set.h"
#include "model_set.h"

namespace M {
    class Variant;
    class Range;
    class SetIterator;

    /**
     * Set class.  This class extends the \ref Model::Set class to support direct access to the \ref M::Variant
     * objects.  This class is intended to be used internally by the model while the \ref Model::Set class is intended
     * to be used by the model API.
     */
    class M_PUBLIC_API Set:public Model::Set {
        public:
            /**
             * The type tracked by this set.
             */
            typedef Variant ValueType;

            /**
             * Iterator you can use to traverse a set.
             */
            typedef SetIterator Iterator;

            /**
             * Type used to represent a constant iterator.
             */
            typedef Iterator ConstIterator;

            /**
             * Type provided for STL compatibility.
             */
            typedef Iterator const_iterator;

            /**
             * Type provided for STL compatibility.
             */
            typedef Iterator iterator;

            /**
             * Type provided for STL compatibility;
             */
            typedef ValueType value_type;

            Set();

            /**
             * Constructor
             *
             * \param[in] other The instance to be copied from.
             */
            Set(const Variant& other);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            Set(const Model::Set& other);

            /**
             * Move constructor
             *
             * \param[in] other The instance to be copied.
             */
            Set(Model::Set&& other);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            Set(const Set& other);

            /**
             * Move constructor
             *
             * \param[in] other The instance to be copied.
             */
            Set(Set&& other);

            ~Set();

            /**
             * Template method you can use to build an empty set.
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
            template<typename... E> M_PUBLIC_TEMPLATE_METHOD static Set build(E... elements) {
                Set result;
                result.insert(elements...);

                return result;
            }

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
            template<typename F, typename S, typename... A> M_PUBLIC_TEMPLATE_METHOD bool insert(
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
            template<typename F, typename S, typename... A> M_PUBLIC_TEMPLATE_METHOD bool remove(
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
             * Method you can call to determine the recommended ordering of this set relative to another set.  Ordering
             * is based on:
             *
             *     * Whether the underlying implementations are the same
             *
             *     * Number of members in the set with smaller sets preceding larger sets.
             *
             *     * Ordering of each member of the set.
             *
             * \param[in] other The other set to compare against.
             *
             * \return Returns a negative value if this set should precede the order set.  Returns 0 if the sets are
             *         equal.  Returns a positive value if this set should follow the other set.
             */
            int relativeOrder(const Set& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            bool operator==(const Set& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            inline bool operator!=(const Set& other) const {
                return !operator==(other);
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
    };
};

#endif
