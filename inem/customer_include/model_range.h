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
* This header defines the \ref Model::Range class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_RANGE_H
#define MODEL_RANGE_H

#include "model_common.h"
#include "model_intrinsic_types.h"
#include "model_range_iterator.h"

namespace M {
    class MODEL_PUBLIC_API Range;
}

namespace Model {
    class MODEL_PUBLIC_API Variant;
    class MODEL_PUBLIC_API RanageIterator;

    /**
     * Range class.  You can use this class to specify ranges of values, test to see if values are defined in the
     * specified range, and iterate over the range.
     */
    class MODEL_PUBLIC_API Range {
        friend class MODEL_PUBLIC_API RangeIterator;
        friend class MODEL_PUBLIC_API Variant;
        friend class MODEL_PUBLIC_API M::Range;

        public:
            /**
             * Type used to represent a constant iterator.
             */
            typedef RangeIterator ConstIterator;

            /**
             * Type used to represent the normal iterator.
             */
            typedef RangeIterator Iterator;

            /**
             * Type provided for STL compatibility.
             */
            typedef RangeIterator const_iterator;

            /**
             * Type provided for STL compatibility.
             */
            typedef RangeIterator iterator;

            Range();

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            Range(const Range& other);

            /**
             * Move constructor
             *
             * \param[in] other The instance to assign to this instance.
             */
            Range(Range&& other);

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
             * Method you can use to determine if the range is empty.
             *
             * \return Returns true if the range is empty.  Returns false if the range is not empty.
             */
            bool isEmpty() const;

            /**
             * Method you can use to determine the size of the range, in elements.
             *
             * \return Returns the current size of the range, in elements.
             */
            Model::Integer size() const;

            /**
             * Method that clears the range.
             */
            void clear();

            /**
             * Method you can use to obtain the first value in the range.
             *
             * \return Returns the first value in the range.
             */
            Variant first() const;

            /**
             * Method you can use to obtain the second value in the range.
             *
             * \return Returns the second value in the range.
             */
            Variant second() const;

            /**
             * Method you can use to obtain the last value in the range.
             *
             * \return Returns the last value in the range.
             */
            Variant last() const;

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

        private:
            class Private;

            /**
             * Private constructor
             *
             * \param[in] newImplementation The new private implementation to assign to this class.
             */
            inline Range(Private* newImplementation):impl(newImplementation) {}

            Private* impl;
    };
}

#endif
