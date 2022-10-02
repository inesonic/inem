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
* This header defines the private implementation of the \ref M::MatrixDensePrivate class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_MATRIX_DENSE_PRIVATE_H
#define M_MATRIX_DENSE_PRIVATE_H

#include <new>

#include "m_matrix_dense_private_base.h"

namespace M {
    /**
     * Private implementation of the \ref M::MatrixDensePrivate template class.  This class provides generic support
     * for dense matrices.
     *
     * You should use this class primary as an input parameter to method that operate on dense matrices.
     *
     * \param[in] C The scalar type to be contained in this matrix.
     */
    template<typename C> class MatrixDensePrivate:public MatrixDensePrivateBase {
        protected:
            /**
             * Constructor
             *
             * \param[in] newNumberRows        The new number of rows.
             *
             * \param[in] newNumberColumns     The new number of columns.
             *
             * \param[in] newAllocationInBytes The size of the allocated data structure, in bytes.
             *
             * \param[in] rawDataBaseAddress   The base address of the raw data stored in the matrix.
             */
            MatrixDensePrivate(
                    unsigned long      newNumberRows,
                    unsigned long      newNumberColumns,
                    unsigned long long newAllocationInBytes,
                    void*              rawDataBaseAddress
                ):MatrixDensePrivateBase(
                    newNumberRows,
                    newNumberColumns,
                    newAllocationInBytes,
                    rawDataBaseAddress,
                    sizeof(C)
                ) {}

            /**
             * Constructor
             *
             * \param[in] other                  The instance to be copied to this instance.
             *
             * \param[in] newAllocationInEntries The size of the allocated data structure, in entries.
             *
             * \param[in] rawDataBaseAddress     The base address of the raw data stored in the matrix.
             */
            MatrixDensePrivate(
                    const MatrixDensePrivate<C>& other,
                    unsigned long long           newAllocationInEntries,
                    void*                        rawDataBaseAddress
                ):MatrixDensePrivateBase(
                    other,
                    newAllocationInEntries,
                    rawDataBaseAddress
                ) {}

            /**
             * Constructor
             *
             * \param[in] other                The instance to be copied to this instance.
             *
             * \param[in] newNumberRows        The new number of rows.
             *
             * \param[in] newNumberColumns     The new number of columns.
             *
             * \param[in] newAllocationInBytes The size of the allocated data structure, in bytes.
             *
             * \param[in] rawDataBaseAddress   The base address of the raw data stored in the matrix.
             */
            MatrixDensePrivate(
                    const MatrixDensePrivate<C>& other,
                    unsigned long                newNumberRows,
                    unsigned long                newNumberColumns,
                    unsigned long long           newAllocationInBytes,
                    void*                        rawDataBaseAddress
                ):MatrixDensePrivateBase(
                    other,
                    newNumberRows,
                    newNumberColumns,
                    newAllocationInBytes,
                    rawDataBaseAddress
                ) {}

        public:
            /**
             * Type used to represent the scalar value used as the coefficients of this matrix.
             */
            typedef C Scalar;

            MatrixDensePrivate(const MatrixDensePrivate<C>& other) = delete;

            /**
             * Method you can use to obtain a reference to the data at a specific location.
             *
             * \param[in] rowIndex The zero based row index we want the entry from.
             *
             * \param[in] columnIndex The zero based column index we want the entry from.
             *
             * \return Returns a pointer to the location of the requested value.
             */
            const C& at(unsigned long rowIndex, unsigned long columnIndex) const {
                return *reinterpret_cast<const C*>(addressOf(rowIndex, columnIndex));
            }

            /**
             * Method you can use to obtain a reference to the data at a specific location.
             *
             * \param[in] rowIndex The zero based row index we want the entry from.
             *
             * \param[in] columnIndex The zero based column index we want the entry from.
             *
             * \return Returns a pointer to the location of the requested value.
             */
            C& at(unsigned long rowIndex, unsigned long columnIndex) {
                return *reinterpret_cast<const C*>(addressOf(rowIndex, columnIndex));
            }

            /**
             * Method you can use to obtain the raw data tracked by this matrix.
             *
             * \return Returns a void pointer to the raw data.
             */
            inline const C* data() const {
                return reinterpret_cast<const C*>(voidData());
            }

            /**
             * Method you can use to obtain the raw data tracked by this matrix.
             *
             * \return Returns a void pointer to the raw data.
             */
            inline C* data() {
                return reinterpret_cast<C*>(voidData());
            }

            /**
             * Method you can use to destroy a previously allocated instance derived from this class.
             *
             * \param[in] instance The instance to be destroyed.
             */
            template<typename D> static void destroy(D* instance) {
                instance->~D();
                releaseMemory(instance);
            }

        protected:
            /**
             * Method you can call to allocate an instance derived from this template class.
             *
             * \param[in] newNumberRows    The new number of rows.
             *
             * \param[in] newNumberColumns The new number of columns.
             */
            template<typename D> static D* createUninitialized(
                    unsigned long newNumberRows,
                    unsigned long newNumberColumns
                ) {
                unsigned long long newAllocation = recommendedAllocation(
                    newNumberRows,
                    newNumberColumns,
                    sizeof(Scalar)
                );

                void* rawDataBaseAddress;
                void* p = allocateMemory(rawDataBaseAddress, newAllocation, sizeof(D));

                return new(p) D(newNumberRows, newNumberColumns, newAllocation, rawDataBaseAddress);
            }

            /**
             * Method you can call to allocate an instance derived from this template class.
             *
             * \param[in] newNumberRows    The new number of rows.
             *
             * \param[in] newNumberColumns The new number of columns.
             */
            template<typename D> static D* create(
                    unsigned long newNumberRows,
                    unsigned long newNumberColumns
                ) {
                unsigned long long newAllocation = recommendedAllocation(
                    newNumberRows,
                    newNumberColumns,
                    sizeof(Scalar)
                );

                void* rawDataBaseAddress;
                void* p = allocateMemory(rawDataBaseAddress, newAllocation, sizeof(D));

                D* result = new(p) D(newNumberRows, newNumberColumns, newAllocation, rawDataBaseAddress);
                result->zeroMatrixMemory();

                return result;
            }

            /**
             * Method you can call to allocate an instance derived from this template class and populate its contents
             * from an array.
             *
             * \param[in] newNumberRows    The new number of rows.
             *
             * \param[in] newNumberColumns The new number of columns.
             *
             * \param[in] matrixData       The matrix data to populate the matrix with.
             */
            template<typename D> static D* create(
                    unsigned long newNumberRows,
                    unsigned long newNumberColumns,
                    const C*      matrixData
                ) {
                unsigned long long newAllocation = recommendedAllocation(
                    newNumberRows,
                    newNumberColumns,
                    sizeof(Scalar)
                );

                void* rawDataBaseAddress;
                void* p = allocateMemory(rawDataBaseAddress, newAllocation, sizeof(D));

                D* result = new(p) D(newNumberRows, newNumberColumns, newAllocation, rawDataBaseAddress);
                result->populate(matrixData);

                return result;
            }

            /**
             * Method you can call to allocate an instance derived from this template class.
             *
             * \param[in] other The instance to make a copy of.
             */
            template<typename D> static D* create(const D& other) {
                unsigned long long newAllocation = recommendedAllocation(
                    other.numberRows(),
                    other.numberColumns(),
                    sizeof(Scalar)
                );

                void* rawDataBaseAddress;
                void* p = allocateMemory(rawDataBaseAddress, newAllocation, sizeof(D));

                D* result = new(p) D(other, newAllocation, rawDataBaseAddress);
                return result;
            }

            /**
             * Method you can call to allocate a new instance with a new row/column size.  The row column values must
             * be greater than or equal to the class being copied.
             *
             * \param[in] other                  The instance to be copied to this instance.
             *
             * \param[in] newNumberRows          The new number of rows.
             *
             * \param[in] newNumberColumns       The new number of columns.
             */
            template<typename D> static D* create(
                    const D&      other,
                    unsigned long newNumberRows,
                    unsigned long newNumberColumns
                ) {
                unsigned long long newAllocation = recommendedAllocation(
                    newNumberRows,
                    newNumberColumns,
                    sizeof(Scalar)
                );

                void* rawDataBaseAddress;
                void* p = allocateMemory(rawDataBaseAddress, newAllocation, sizeof(D));

                D* result = new(p) D(other, newNumberRows, newNumberColumns, newAllocation, rawDataBaseAddress);
                return result;
            }
    };
}

#endif
