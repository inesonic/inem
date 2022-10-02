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
* This header defines the \ref M::MatrixIterator class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_MATRIX_ITERATOR_H
#define M_MATRIX_ITERATOR_H

#include "m_intrinsic_types.h"

namespace M {
    class Variant;
    class Range;

    /**
     * Matrix iterator class.  You can use this class as a generic base class for matrix iterators.
     *
     * \param[in] T The matrix type we are iterator over.
     *
     * \param[in] S The scalar the matrix supports.
     */
    template<typename T, typename S = typename T::Scalar> class MatrixIterator {
        public:
            /**
             * The scalar type returned by this iterator.
             */
            typedef S Scalar;

            MatrixIterator() {
                currentMatrix = nullptr;
                currentRow    = 0;
                currentColumn = 0;
            }

            /**
             * Constructor
             *
             * \param[in] matrix A pointer to the matrix class we are iterator over.
             *
             * \param[in] row    The one based row index to set this iterator to.
             *
             * \param[in] column The one based column index to set this iterator to.
             */
            MatrixIterator(const T* matrix, unsigned long row, unsigned long column) {
                currentMatrix = matrix;
                currentRow    = row;
                currentColumn = column;
            }

            /**
             * Copy constructor
             *
             * \param[in] other The iterator to assign to this iterator.
             */
            MatrixIterator(const MatrixIterator<T,S>& other) {
                currentMatrix = other.currentMatrix;
                currentRow    = other.currentRow;
                currentColumn = other.currentColumn;
            }

            ~MatrixIterator() {}

            /**
             * Method you can use to obtain the current value pointed to by the iterator.
             *
             * \return Returns a reference to the current value.
             */
            Scalar value() const {
                return currentMatrix->at(static_cast<M::Integer>(currentRow), static_cast<M::Integer>(currentColumn));
            }

            /**
             * Method you can use to obtain a reference to the current value pointed to by the iterator.
             *
             * \return Returns a reference to the current value.
             */
            const Scalar& constReference() const {
                return *(currentMatrix->pointer(
                    static_cast<M::Integer>(currentRow),
                    static_cast<M::Integer>(currentColumn)
                ));
            }

            /**
             * Method you can use to obtain a pointer to the current value.
             *
             * \return Returns a pointer to the current value.
             */
            const Scalar* constPointer() const {
                return currentMatrix->pointer(
                    static_cast<M::Integer>(currentRow),
                    static_cast<M::Integer>(currentColumn)
                );
            }

            /**
             * Method you can use to advance the iterator by one position.
             */
            void advance() {
                unsigned long numberColumns = currentMatrix->numberColumns();
                ++currentColumn;
                if (currentColumn > numberColumns) {
                    unsigned long numberRows = currentMatrix->numberRows();
                    if (currentRow < numberRows) {
                        ++currentRow;
                        currentColumn = 1;
                    } else {
                        currentRow    = numberRows;
                        currentColumn = numberColumns + 1;
                    }
                }
            }

            /**
             * Method you can use to advance the iterator by one position by a specified distance.
             *
             * \param[in] distance The distance to advance the iterator.
             */
            void advance(unsigned long long distance) {
                unsigned long      numberColumns      = currentMatrix->numberColumns();
                unsigned long      numberRows         = currentMatrix->numberRows();
                unsigned long long numberCoefficients = numberRows * numberColumns;
                unsigned long long currentIndex       = (currentRow - 1) * numberColumns + currentColumn - 1;
                unsigned long long newIndex           = currentIndex + distance;

                if (newIndex < numberCoefficients) {
                    currentRow    = 1 + (newIndex / numberColumns);
                    currentColumn = 1 + (newIndex % numberColumns);
                } else {
                    currentRow    = numberRows;
                    currentColumn = numberColumns + 1;
                }
            }

            /**
             * Method you can use to determine if this iterator is pointing to a valid location.
             *
             * \return Returns true if this iterator is pointing to a valid location in the container.  Returns
             *         false if the iterator is not initialized or is pointing to an invalid location.
             */
            bool isValid() const {
                return (
                       currentMatrix != nullptr
                    && currentRow <= static_cast<unsigned long>(currentMatrix->numberRows())
                    && currentColumn <= static_cast<unsigned long>(currentMatrix->numberColumns())
                );
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
            inline Scalar operator*() const {
                return constReference();
            }

            /**
             * Indirection operator
             *
             * \return Returns a pointer to the object.
             */
            inline const Scalar* operator->() const {
                return constPointer();
            }

            /**
             * Pre-increment operator.
             *
             * \return Returns a reference to this iterator.
             */
            MatrixIterator& operator++() {
                advance();
                return *this;
            }

            /**
             * Post-increment operator.
             *
             * \return Returns a new iterator pointing to the old location.
             */
            MatrixIterator operator++(int) {
                MatrixIterator result = *this;
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
            MatrixIterator& operator+=(unsigned long long distance) {
                advance(distance);
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
            bool operator==(const MatrixIterator<T,S>& other) const {
                return currentRow == other.currentRow && currentColumn == other.currentColumn;
            }

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the iterators point to the same element.  Returns false if the
             *         iterators point to different elements.
             */
            bool operator!=(const MatrixIterator<T,S>& other) const {
                return currentRow != other.currentRow || currentColumn != other.currentColumn;
            }

            /**
             * Copy assignment operator.
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this instance.
             */
            MatrixIterator<T,S>& operator=(const MatrixIterator<T,S>& other) {
                currentMatrix = other.currentMatrix;
                currentRow    = other.currentRow;
                currentColumn = other.currentColumn;

                return *this;
            }

        private:
            /**
             * The matrix this iterator is operating over.
             */
            const T* currentMatrix;

            /**
             * The current iterator row.
             */
            unsigned long currentRow;

            /**
             * The current iterator column.
             */
            unsigned long currentColumn;
    };
};

#endif
