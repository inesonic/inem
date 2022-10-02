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

#ifndef M_MATRIX_DENSE_PRIVATE_BASE_H
#define M_MATRIX_DENSE_PRIVATE_BASE_H

#include <cstdint>

#include "m_api_types.h"
#include "m_intrinsic_types.h"
#include "m_compiler_abstraction.h"
#include "m_matrix_private_base.h"

namespace M {
    /**
     * Private base class used by the \ref MatrixDensePrivate template class.  This class provides generic support
     * for dense matrices.
     */
    class MatrixDensePrivateBase:public MatrixPrivateBase {
        public:
            /**
             * Constructor
             *
             * \param[in] newNumberRows          The new number of rows.
             *
             * \param[in] newNumberColumns       The new number of columns.
             *
             * \param[in] newAllocationInBytes   The size of the allocated data structure, in bytes.
             *
             * \param[in] rawDataBaseAddress     The base address of the raw data stored in the matrix.
             *
             * \param[in] coefficientSizeInBytes The coefficient size in bytes.
             */
            MatrixDensePrivateBase(
                unsigned long      newNumberRows,
                unsigned long      newNumberColumns,
                unsigned long long newAllocationInBytes,
                void*              rawDataBaseAddress,
                unsigned           coefficientSizeInBytes
            );

            /**
             * Constructor
             *
             * \param[in] other                The instance to be copied to this instance.
             *
             * \param[in] newAllocationInBytes The size of the allocated data structure, in bytes.
             *
             * \param[in] rawDataBaseAddress   The base address of the raw data stored in the matrix.
             */
            MatrixDensePrivateBase(
                const MatrixDensePrivateBase& other,
                unsigned long long            newAllocationInBytes,
                void*                         rawDataBaseAddress
            );

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
            MatrixDensePrivateBase(
                const MatrixDensePrivateBase& other,
                unsigned long                 newNumberRows,
                unsigned long                 newNumberColumns,
                unsigned long long            newAllocationInBytes,
                void*                         rawDataBaseAddress
            );

            ~MatrixDensePrivateBase();

            /**
             * Method you can use to determine number of matrix rows.
             *
             * \return Returns the current number of rows.
             */
            inline unsigned long numberRows() const {
                return currentNumberRows;
            }

            /**
             * Method you can use to determine the number of matrix columns.
             *
             * \return Returns the current number of matrix columns.
             */
            inline unsigned long numberColumns() const {
                return currentNumberColumns;
            }

            /**
             * Method you can use to determine the column spacing in memory.  This value will be equal to or greater
             * than the number of columns and represents the distance between columns in coefficients.
             *
             * \return Returns the column dimension.
             */
            unsigned long long columnSpacingInMemory() const;

            /**
             * Method you can use to quickly zero out the raw matrix data.
             */
            void zeroMatrix();

            /**
             * Method you can use to resize the matrix contents, in place.
             *
             * \param[in] newNumberRows    The new number of rows.
             *
             * \param[in] newNumberColumns The new number of columns.
             *
             * \return Returns true if the matrix can be resized in place, returns false if a new instance is required.
             */
            bool resizeInPlace(unsigned long newNumberRows, unsigned long newNumberColumns);

            /**
             * Method you can use tp determine if it's possible to resize to a given size, in place.
             *
             * \param[in] newNumberRows    The new number of rows.
             *
             * \param[in] newNumberColumns The new number of columns.
             *
             * \return Returns true if this matrix can be resized, in place, to the specified size.  Returns false if
             *         a new matrix must be created to be resized.
             */
            bool canResizeInPlace(unsigned long newNumberRows, unsigned long newNumberColumns) const;

            /**
             * Method you can use to obtain the value at a specified row/column.
             *
             * \param[in] rowIndex The zero based row index we want the entry from.
             *
             * \param[in] columnIndex The zero based column index we want the entry from.
             *
             * \return Returns a pointer to the location of the requested value.
             */
            inline const void* addressOf(unsigned long rowIndex, unsigned long columnIndex) const {
                unsigned long long columnSize = columnSizeInBytes(currentNumberRows, currentCoefficientSizeInBytes);
                return rawData.byte + (columnSize * columnIndex) + (currentCoefficientSizeInBytes * rowIndex);
            }

            /**
             * Method you can use to obtain the value at a specified row/column.
             *
             * \param[in] rowIndex The zero based row index we want the entry from.
             *
             * \param[in] columnIndex The zero based column index we want the entry from.
             *
             * \return Returns a pointer to the location of the requested value.
             */
            inline void* addressOf(unsigned long rowIndex, unsigned long columnIndex) {
                return (
                      rawData.byte
                    + (currentColumnSizeInBytes * columnIndex)
                    + (currentCoefficientSizeInBytes * rowIndex)
                );
            }

            /**
             * Method you can call to determine the recommended ordering of this matrix data relative to another
             * matrix data instance.  Ordering is based on:
             *
             *     * Number of matrix rows and columns.
             *
             *     * Value of each byte in the matrix.
             *
             * \param[in] other The other set to compare against.
             *
             * \return Returns a negative value if this matrix should precede the order matrix.  Returns 0 if the
             *         matrices are equal.  Returns a positive value if this matrix should follow the other matrix.
             */
            int relativeOrder(const MatrixDensePrivateBase& other) const;

        protected:
            /**
             * Method you can use to obtain the raw data tracked by this matrix.
             *
             * \return Returns a void pointer to the raw data.
             */
            inline const void* voidData() const {
                return rawData.byte;
            }

            /**
             * Method you can use to obtain the raw data tracked by this matrix.
             *
             * \return Returns a void pointer to the raw data.
             */
            inline void* voidData() {
                return rawData.byte;
            }

            /**
             * Method you can use to quickly zero out the entire matrix allocated memory.
             */
            void zeroMatrixMemory();

            /**
             * Method you can use to clear entries, turning this matrix into an upper triangular matrix.
             */
            void toUpperTriangular();

            /**
             * Method you can use to clear entries, turning this matrix into a lower triangular matrix.
             */
            void toLowerTriangular();

            /**
             * Method you can call to populate the matrix from an array of values.
             *
             * \param[in] matrixData The matrix data used to populate the matrix.
             */
            void populate(const void* matrixData);

            /**
             * Method that calculates the memory footprint for a single column, in bytes.  Note that this method will
             * automatically assumed that columns should be packed if the number of rows per column is 1.
             *
             * \param[in] numberRows             The number of rows in the matrix.
             *
             * \param[in] coefficientSizeInBytes The coefficient size in bytes.
             *
             * \return Returns the space required for a single column.  This value represents the number of
             *         coefficients per column times the space required per coefficient, padded up to the next column
             *         boundary.
             */
            static unsigned long long columnSizeInBytes(
                unsigned long numberRows,
                unsigned      coefficientSizeInBytes
            );

            /**
             * Method you can call to calculate the minimum space needed for this structure, in bytes.  This method
             * look at just the raw data for the array and ignores additional overhead that might be required.
             *
             * \param[in] newNumberRows          The new number of rows.
             *
             * \param[in] newNumberColumns       The new number of columns.
             *
             * \param[in] coefficientSizeInBytes The coefficient size in bytes.
             */
            static unsigned long long minimumRequiredAllocation(
                unsigned long newNumberRows,
                unsigned long newNumberColumns,
                unsigned      coefficientSizeInBytes
            );

            /**
             * Method that calculates the recommended memory allocation based on the size of the matrix.
             *
             * \param[in] newNumberRows          The new number of rows.
             *
             * \param[in] newNumberColumns       The new number of columns.
             *
             * \param[in] coefficientSizeInBytes The size of a single matrix coefficient, in bytes.
             *
             * \return Returns the recommended allocation for the data structure, in bytes.
             */
            static unsigned long long recommendedAllocation(
                unsigned long newNumberRows,
                unsigned long newNumberColumns,
                unsigned      coefficientSizeInBytes
            );

        private:
            /**
             * Memory alignment requirement.  This alignment requirement should be enforced for each column of the
             * array.
             */
            static constexpr unsigned memoryColumnAlignmentRequirementBytes = 64;

            /**
             * The base pointer for the raw data stored in the matrix.  We define as a union of several pointer types
             * to simplify debugging.
             */
            union RawData {
                RawData(void* p) { byte = reinterpret_cast<std::uint8_t*>(p); }

                std::uint8_t* byte;
                Integer*      integer;
                Real*         real;
                Complex*      complex;
            } rawData;

            /**
             * The current total allocated size, in bytes.
             */
            unsigned long long currentAllocatedSizeInBytes;

            /**
             * The current column size in bytes.
             */
            unsigned long long currentColumnSizeInBytes;

            /**
             * The current number of rows.
             */
            unsigned long currentNumberRows;

            /**
             * The current number of columns
             */
            unsigned long currentNumberColumns;

            /**
             * The size of a single coefficient, in bytes.
             */
            unsigned currentCoefficientSizeInBytes;
    };
}

#endif
