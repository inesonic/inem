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
* This header defines the \ref Model::TupleConstIterator class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_TUPLE_CONST_ITERATOR_H
#define MODEL_TUPLE_CONST_ITERATOR_H

#include "model_common.h"
#include "model_tuple_iterator_base.h"

namespace Model {
    class Variant;
    class Tuple;

    /**
     * Iterator you can use to traverse a tuple.
     */
    class MODEL_PUBLIC_API TupleConstIterator:public TupleIteratorBase {
        friend class Tuple;

        public:
            TupleConstIterator();

            /**
             * Copy constructor
             *
             * \param other The instance to be copied.
             */
            TupleConstIterator(const TupleConstIterator& other);

            /**
             * Move constructor
             *
             * \param[in] other The instance to apply to this instance.
             */
            TupleConstIterator(TupleConstIterator&& other);

            ~TupleConstIterator();

            /**
             * Pre-increment operator
             *
             * \return Returns a reference to this instance.
             */
            TupleConstIterator& operator++();

            /**
             * Post-increment operator
             *
             * \return Returns an instance representing the state of this iterator prior to being advanced.
             */
            TupleConstIterator operator++(int);

            /**
             * Pre-decrement operator
             *
             * \return Returns a reference to this instance.
             */
            TupleConstIterator& operator--();

            /**
             * Post-decrement operator
             *
             * \return Returns an instance representing the state of this iterator prior to being backed up.
             */
            TupleConstIterator operator--(int);

            /**
             * Add assignment operator.
             *
             * \param[in] distance The distance to advance the iterator.
             *
             * \return Returns a reference to this instance.
             */
            TupleConstIterator& operator+=(signed long long distance);

            /**
             * Subtract assignment operator.
             *
             * \param[in] distance The distance to advance the iterator.
             *
             * \return Returns a reference to this instance.
             */
            TupleConstIterator& operator-=(signed long long distance);

            /**
             * Addition operator.
             *
             * \param[in] distance The distance to add to this iterator to calculated the result.
             *
             * \return Returns an iterator representing the requested position.
             */
            TupleConstIterator operator+(signed long long distance) const;

            /**
             * Subtraction operator.
             *
             * \param[in] distance The distance to apply to this iterator to calculate the result.
             *
             * \return Returns an iterator representing the requested position.
             */
            TupleConstIterator operator-(signed long long distance) const;

            /**
             * Copy assignment operator.
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this instance.
             */
            TupleConstIterator& operator=(const TupleConstIterator& other);

            /**
             * Move assignment operator.
             *
             * \param[in] other The instance to move-assign to this iterator.
             *
             * \return Returns a reference to this instance.
             */
            TupleConstIterator& operator=(TupleConstIterator&& other);

        private:
            /**
             * Private constructor
             *
             * \param[in] newImplementation The private implementation to assign to this iterator.
             */
            inline TupleConstIterator(
                    TupleIteratorBase::Private* newImplementation
                ):TupleIteratorBase(
                    newImplementation
                ) {}
    };
};

#endif
