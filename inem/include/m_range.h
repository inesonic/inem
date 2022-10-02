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
* This header defines the \ref M::Range class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_RANGE_H
#define M_RANGE_H

#include "m_common.h"
#include "m_variant.h"
#include "model_range.h"

namespace M {
    /**
     * Internally used version of the \ref Model::Range class.  You can use this class to specify ranges of values,
     * test to see if values are defined in the specified range, and iterate over the range.
     */
    class M_PUBLIC_API Range:public Model::Range {
        public:
            /**
             * Iterator you can use to traverse a range.
             */
            class M_PUBLIC_API Iterator {
                friend class Range;

                public:
                    Iterator();

                    /**
                     * Copy constructor
                     *
                     * \param other The instance to be copied.
                     */
                    Iterator(const Iterator& other);

                    virtual ~Iterator();

                    /**
                     * Method you can use to obtain the current value pointed to by the iterator.  Note that the
                     * Model::Iterator::reference method may prove to be slightly faster.
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
                    virtual void backup();

                    /**
                     * Method you can call to backup the iterator by a specified distance.
                     *
                     * \param[in] distance The distance to reverse the iterator.
                     */
                    void backup(signed long long distance);

                    /**
                     * Method you can use to determine if this iterator is pointing to a valid entry.
                     *
                     * \return Returns true if this iterator is pointing to a valid entry in the range.  Returns false
                     *         if the iterator is not initialized or is pointing to an invalid location.
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
                     * Dereferencing operator.  Note that the Model::Range::reference method may prove slightly faster
                     * than the dereferencing operator.
                     *
                     * \see Range::Iterator::value
                     * \see Range::Iterator::constReference
                     *
                     * \return Returns a reference to the current value pointed to by the iterator.
                     */
                    Variant operator*() const;

                    /**
                     * Indirection operator.
                     *
                     * \see Range::Iterator::constReference
                     * \see Range::Iterator::constPointer
                     *
                     * \return Returns a pointer to the current value pointed to by the iterator.
                     */
                    const Variant* operator->() const;

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
                     * Subtraction operator.
                     *
                     * \param[in] other The iterator to subtract from this iterator to determine distance.
                     *
                     * \return Returns an iterator representing the requested position.
                     */
                    signed long long operator-(const Iterator& other) const;

                    /**
                     * Copy assignment operator.
                     *
                     * \param[in] other The instance to assign to this instance.
                     *
                     * \return Returns a reference to this instance.
                     */
                    Iterator& operator=(const Iterator& other);

                    /**
                     * Comparison operator
                     *
                     * \param[in] other The instance to compare against this instance.
                     *
                     * \return Returns true if the instance point to the same entry in the range.  Returns false if the
                     *         instances point to different entries.  Comparing entries in different ranges will lead
                     *         to indeterminate results.
                     */
                    bool operator==(const Iterator& other) const;

                    /**
                     * Comparison operator
                     *
                     * \param[in] other The instance to compare against this instance.
                     *
                     * \return Returns true if the instance point to different entries in the range.  Returns false if
                     *         the instances point to the same entry.  Comparing entries in different ranges will lead
                     *         to indeterminate results.
                     */
                    bool operator!=(const Iterator& other) const;

                private:
                    /**
                     * Method that is called from other methods to update the current variant value.
                     */
                    void updateCurrentValue();

                    /**
                     * Private constructor
                     *
                     * \param[in] newRange The new range associated with this iterator.
                     *
                     * \param[in] newIndex The new starting index for the iterator.
                     *
                     * \param[in] newValue The new initial value for the iterator.
                     */
                    Iterator(const Range* newRange, unsigned long long newIndex, const Variant& newValue);

                    /**
                     * Pointer to the range associated with this iterator.
                     */
                    const Range* currentRange;

                    /**
                     * Current index into the range.
                     */
                    unsigned long long currentIndex;

                    /**
                     * The current value being tracked by this variant.
                     */
                    Variant currentValue;
            };

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

            Range();

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            Range(const Model::Range& other);

            /**
             * Move constructor
             *
             * \param[in] other The instance to assign to this instance.
             */
            Range(Model::Range&& other);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            Range(const M::Range& other);

            /**
             * Move constructor
             *
             * \param[in] other The instance to assign to this instance.
             */
            Range(M::Range&& other);

            /**
             * Constructor
             *
             * \param[in] first The first value in the range.
             *
             * \param[in] last  The last value in the range.
             *
             * \throws Model::InvalidRangeParameter
             */
            Range(const Variant& first, const Variant& last);

            /**
             * Constructor
             *
             * \param[in] first  The first value in the range.
             *
             * \param[in] second The second value in the range.
             *
             * \param[in] last   The last value in the range.
             *
             * \throws Model::InvalidRangeParameter
             */
            Range(const Variant& first, const Variant& second, const Variant& last);

            ~Range();

            /**
             * Method you can use to obtain the first value in the range.
             *
             * \return Returns the first value in the range.
             */
            const Variant& first() const;

            /**
             * Method you can use to obtain the second value in the range.
             *
             * \return Returns the second value in the range.
             */
            const Variant& second() const;

            /**
             * Method you can use to obtain the last value in the range.
             *
             * \return Returns the last value in the range.
             */
            const Variant& last() const;

            /**
             * Method you can use to determine if a value is contained in the range.
             *
             * \param[in] value The value to check for.
             *
             * \return Returns true if the value exists in the range.  Returns false if the value does not exist in the
             *         range.  Note that only values that will be returned by iterating over the range will be
             *         considered as in the range.
             */
            bool contains(const Variant& value) const;

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
        };
}

#endif
