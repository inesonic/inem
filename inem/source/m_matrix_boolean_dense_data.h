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
* This header defines the private implementation data store for boolean dense matrices.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_MATRIX_BOOLEAN_DENSE_DATA_H
#define MODEL_MATRIX_BOOLEAN_DENSE_DATA_H

#include <cstdint>

#include "model_api_types.h"
#include "model_intrinsic_types.h"
#include "m_matrix_complex.h"
#include "m_matrix_real.h"
#include "m_matrix_integer.h"
#include "m_matrix_boolean_data.h"
#include "m_matrix_boolean.h"
#include "m_matrix_private_base.h"

namespace M {
    /**
     * Private implementation class for \ref M::MatrixBoolean.  This class provides the data store and implementation
     * for real matricies using a dense data format.
     */
    class MatrixBoolean::DenseData:public MatrixBoolean::Data, public MatrixPrivateBase {
        friend class MatrixReal::DenseData;
        friend class MatrixComplex::DenseData;

        protected:
            /**
             * Constructor
             *
             * \param[in] newNumberRows               The new number of rows.
             *
             * \param[in] newNumberColumns            The new number of columns.
             *
             * \param[in] newColumnSizeInBits         The space assigned to an individual column, in bits.
             *
             * \param[in] newAllocationInStorageUnits The size of the allocated data structure, in storage units.
             *
             * \param[in] rawDataBaseAddress          The base address of the raw data stored in the matrix.
             */
            DenseData(
                unsigned long      newNumberRows,
                unsigned long      newNumberColumns,
                unsigned long long newColumnSizeInBits,
                unsigned long long newAllocationInStorageUnits,
                void*              rawDataBaseAddress
            );

            /**
             * Constructor
             *
             * \param[in] other                       The instance to be copied to this instance.
             *
             * \param[in] newColumnSizeInBits         The space assigned to an individual column, in bits.
             *
             * \param[in] newAllocationInStorageUnits The size of the allocated data structure, in storage units.
             *
             * \param[in] rawDataBaseAddress          The base address of the raw data stored in the matrix.
             */
            DenseData(
                const DenseData&   other,
                unsigned long long newColumnSizeInBits,
                unsigned long long newAllocationInStorageUnits,
                void*              rawDataBaseAddress
            );

            /**
             * Constructor
             *
             * \param[in] other                       The instance to be copied to this instance.
             *
             * \param[in] newNumberRows               The new number of rows.
             *
             * \param[in] newNumberColumns            The new number of columns.
             *
             * \param[in] newColumnSizeInBits         The space assigned to an individual column, in bits.
             *
             * \param[in] newAllocationInStorageUnits The size of the allocated data structure, in storage units.
             *
             * \param[in] rawDataBaseAddress          The base address of the raw data stored in the matrix.
             */
            DenseData(
                const DenseData&   other,
                unsigned long      newNumberRows,
                unsigned long      newNumberColumns,
                unsigned long long newColumnSizeInBits,
                unsigned long long newAllocationInStorageUnits,
                void*              rawDataBaseAddress
            );

            ~DenseData() override;

        public:
            /**
             * Method you can call to allocate an instance of this class.
             *
             * \param[in] newNumberRows    The new number of rows.
             *
             * \param[in] newNumberColumns The new number of columns.
             *
             * \return Returns a pointer to the newly created instance.
             */
            static DenseData* create(unsigned long newNumberRows, unsigned long newNumberColumns);

            /**
             * Method you can call to allocate an instance of this class and populate it with values from an array.
             *
             * \param[in] newNumberRows    The new number of rows.
             *
             * \param[in] newNumberColumns The new number of columns.
             *
             * \param[in] matrixData       The data to populate into the matrix.
             *
             * \return Returns a pointer to the newly created instance.
             */
            static DenseData* create(
                    unsigned long newNumberRows,
                    unsigned long newNumberColumns,
                    const Scalar* matrixData
                );

            /**
             * Method you can call to allocate an instance of this class and populate it with values from another
             * boolean matrix.
             *
             * \param[in] other The instance to populate this instance with.
             *
             * \return Returns a pointer to the newly created instance.
             */
            static DenseData* create(const DenseData& other);

            /**
             * Method you can call to allocate an instance of this class and populate it with values from another
             * boolean matrix.
             *
             * \param[in] other The instance to populate this instance with.
             *
             * \return Returns a pointer to the newly created instance.
             */
            static DenseData* create(const MatrixInteger::DenseData& other);

            /**
             * Method you can call to allocate an instance of this class and populate it with values from another
             * boolean matrix.
             *
             * \param[in] other The instance to populate this instance with.
             *
             * \return Returns a pointer to the newly created instance.
             */
            static DenseData* create(const MatrixReal::DenseData& other);

            /**
             * Method you can call to allocate an instance of this class and populate it with values from another
             * boolean matrix.
             *
             * \param[in] other The instance to populate this instance with.
             *
             * \return Returns a pointer to the newly created instance.
             */
            static DenseData* create(const MatrixComplex::DenseData& other);

            /**
             * Method you can call to allocate an instance of this class without any memory initialization.
             *
             * \param[in] newNumberRows    The new number of rows.
             *
             * \param[in] newNumberColumns The new number of columns.
             *
             * \return Returns a pointer to the newly created instance.
             */
            static DenseData* createUninitialized(unsigned long newNumberRows, unsigned long newNumberColumns);

            /**
             * Method you can call to destroy an instance of this class.
             *
             * \param[in] instance The instance to be destroyed.
             */
            static void destroy(DenseData* instance);

            /**
             * Method you can use to clone this matrix.
             *
             * \return Returns a newly created instance of this matrix.
             */
            MatrixBoolean::Data* clone() const override {
                return create(*this);
            }

            /**
             * Method you can use to construct an identity matrix for this type.
             *
             * \param[in] numberRows    The number of rows in the matrix.
             *
             * \param[in] numberColumns The number of columns in the matrix.
             */
            static DenseData* identity(unsigned long numberRows, unsigned long numberColumns);

            /**
             * Method you can use to construct a ones matrix for this type.
             *
             * \param[in] numberRows    The number of rows in the matrix.
             *
             * \param[in] numberColumns The number of columns in the matrix.
             */
            static DenseData* ones(unsigned long numberRows, unsigned long numberColumns);

            /**
             * Method you can use to determine the array type.
             *
             * \return Returns the array type being represented by this instance.
             */
            Matrix::MatrixType matrixType() const override {
                return Matrix::MatrixType::DENSE;
            }

            /**
             * Method you can use to determine the type of coefficient stored in this matrix.
             *
             * \return Returns the coefficient type stored in the matrix.
             */
            ValueType coefficientType() const override {
                return ValueType::BOOLEAN;
            }

            /**
             * Method you can use to save this matrix to a file in binary format.  This method will throw an exception
             * if an error occurs and the associated exception is enabled.
             *
             * \param[in] filename   The filename to save the file to
             *
             * \param[in] fileFormat The desired file format.
             *
             * \return Returns true on success, returns false on error.
             */
            bool toFile(const char* filename, Model::DataFileFormat fileFormat) const override;

            /**
             * Method you can use to load a new matrix from a file.  This method will throw an exception if an error
             * occurs and the associated exception is enabled.
             *
             * \param[in] filename The filename to save the file to.
             *
             * \return Returns a newly created matrix.  Returns an empty matrix on error.
             */
            static DenseData* fromFile(const char* filename);

            /**
             * Method you can use to determine number of matrix rows.
             *
             * \return Returns the current number of rows.
             */
            unsigned long numberRows() const override {
                return currentNumberRows;
            }

            /**
             * Method you can use to determine the number of matrix columns.
             *
             * \return Returns the current number of matrix columns.
             */
            unsigned long numberColumns() const override {
                return currentNumberColumns;
            }

            /**
             * Method you can use to obtain the value at a given location.
             *
             * \param[in] rowIndex    The zero based row index we want the entry from.
             *
             * \param[in] columnIndex The zero based column index we want the entry from.
             *
             * \return Returns the value at the specified location.
             */
            Scalar at(unsigned long rowIndex, unsigned long columnIndex) const override;

            /**
             * Method you can use to obtain the value at a given location.
             *
             * \param[in] rowIndex    The zero based row index we want the entry from.
             *
             * \param[in] columnIndex The zero based column index we want the entry from.
             *
             * \param[in] newValue    The new value to assign to the specified location.
             *
             * \return Returns the value at the specified location.
             */
            void update(unsigned long rowIndex, unsigned long columnIndex, Scalar newValue) override;

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined left to right.
             *
             * \param[in] other The matrix to combine to the right of this matrix.
             *
             * \return Returns a new matrix representing the combination of the two matrices.
             */
            MatrixBoolean::Data* combineLeftToRight(const MatrixBoolean::DenseData& other) const override;

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined left to right.
             *
             * \param[in] other The matrix to combine to the right of this matrix.
             *
             * \return Returns a new matrix representing the combination of the two matrices.
             */
            MatrixBoolean::Data* combineLeftToRight(const MatrixBoolean::SparseData& other) const override;

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined top to bottom.
             *
             * \param[in] other The matrix to combine below this matrix.
             *
             * \return Returns a new matrix representing the combination of the two matrices.
             */
            MatrixBoolean::Data* combineTopToBottom(const MatrixBoolean::DenseData& other) const override;

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined top to bottom.
             *
             * \param[in] other The matrix to combine below this matrix.
             *
             * \return Returns a new matrix representing the combination of the two matrices.
             */
            MatrixBoolean::Data* combineTopToBottom(const MatrixBoolean::SparseData& other) const override;

            /**
             * Method you can use to reverse the columns in this matrix.
             *
             * \return Returns a column-reversed version of this matrix.
             */
            MatrixBoolean::Data* columnReverse() const override;

            /**
             * Method you can use to reverse the rows in this matrix.
             *
             * \return Returns a row-reversed version of this matrix.
             */
            MatrixBoolean::Data* rowReverse() const override;

            /**
             * Method you can call to resize this matrix to a specific dimension.
             *
             * \param[in] newNumberRows    The new number of rows for the array.
             *
             * \param[in] newNumberColumns The new number of columns for the array.
             *
             * \param[in] alwaysReallocate if true, then this method will always create a new array instance.  If
             *                             false, then this method may try to preserve the existing array depending on
             *                             the magnitude of the size change.
             *
             * \return Returns either a pointer to this object or a newly initialized instance if the object size
             *         could not support the matrix.  Note that reducing the array size may also cause a new instance
             *         to be allocated.
             */
            MatrixBoolean::Data* resizeTo(
                    unsigned long newNumberRows,
                    unsigned long newNumberColumns,
                    bool          alwaysReallocate = false
                ) override;

            /**
             * Method you can call attempt a resize-in-place.
             *
             * \param[in] newNumberRows    The new number of rows for the array.
             *
             * \param[in] newNumberColumns The new number of columns for the array.
             *
             * \return Returns true on success, returns false if the matrix could not be adjusted in-place to the new
             *         size.
             */
            bool resizeInPlaceTo(unsigned long newNumberRows, unsigned long newNumberColumns) override;

            /**
             * Method that compares this matrix against another matrix.  This version supports boolean types.
             *
             * \param[in] other The matrix to be compared against.
             *
             * \return Returns true if the matrices are equal.  Returns false if the matrices are not equal.
             */
            bool isEqualTo(const MatrixBoolean::DenseData& other) const override;

            /**
             * Method that compares this matrix against another matrix.  This version supports boolean types.
             *
             * \param[in] other The matrix to be compared against.
             *
             * \return Returns true if the matrices are equal.  Returns false if the matrices are not equal.
             */
            bool isEqualTo(const MatrixBoolean::SparseData& other) const override;

            /**
             * Method that applies a requested transform and scaling.
             *
             * \return Returns a pointer to the newly transformed and scaled instance.
             */
            Data* applyTransform() const override;

            /**
             * Method you can call to determine the recommended ordering of this matrix data relative to another
             * matrix data instance.  Ordering is based on:
             *
             *     * The type of the underlying data.
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
            int relativeOrder(const MatrixBoolean::Data& other) const override;

        private:
            /**
             * Type that represents the fundamental storage unit for this matrix type.
             */
            typedef std::uint64_t StorageUnit;

            /**
             * Table used to perform fast byte reversals.
             */
            static std::uint8_t byteReverseTable[256];

            /**
             * Type used to map storage units to an array of bytes.
             */
            union StorageUnitBytes {
                StorageUnit  storageUnit;
                std::uint8_t bytes[sizeof(StorageUnit)];
            };

            /**
             * Method that calculates a recommended column size, in bits.
             *
             * \param[in] numberRows The required number of rows.
             *
             * \return Recommends a recommended column size, in bits.
             */
            static unsigned long long recommendedColumnSize(unsigned long numberRows);

            /**
             * Method that performs an out-of-place resize.
             *
             * \param[in] newNumberRows    The new number of rows for the array.
             *
             * \param[in] newNumberColumns The new number of columns for the array.
             *
             * \return Returns a new \ref M::MatrixBoolean::DenseData instance holding the resized version of this
             *         object.
             */
            DenseData* resizeOutOfPlace(unsigned long newNumberRows, unsigned long newNumberColumns) const;

            /**
             * Method that zeros the entire memory contents for this matrix.
             */
            void zeroMemory();

            /**
             * Method that zeros a range of bits.
             *
             * \param[in] startingBitOffset The starting bit offset, inclusive.
             *
             * \param[in] endingBitOffset   The ending bit offset, inclusive.
             */
            void zeroBits(unsigned long long startingBitOffset, unsigned long long endingBitOffset);

            /**
             * Method you can use to read non-aligned column data while updating a bit offset and storage unit pointer.
             *
             * \param[in,out] currentBitShiftValue The current bit shift value.  The value will be adjusted by the
             *                                     provided shift amount.  If the value equals or exceeds the storage
             *                                     unit size, the value will be reset and the pointer and current data
             *                                     values will be updated.
             *
             * \param[in,out] currentDataPointer   The current pointer to the data.  This should point to the location
             *                                     being operated on.  The pointer will be advanced if the shift value
             *                                     rolls over.
             *
             * \param[in,out] currentData          The current data value.  This value will be updated if the shift
             *                                     value rolls over.
             *
             * \param[in]     shiftAmount          The amount to adjust the shift value by.
             *
             * \return Returns the bits at the specified location.
             */
            static StorageUnit readAndAdjustSourcePointer(
                unsigned&           currentBitShiftValue,
                const StorageUnit*& currentDataPointer,
                StorageUnit&        currentData,
                unsigned            shiftAmount
            );

            /**
             * Method you can use to update a bit offset and storage unit pointer during a resize operation.
             *
             * \param[in,out] currentBitShiftValue The current bit shift value.  The value will be adjusted by the
             *                                     provided shift amount.  If the value equals or exceeds the storage
             *                                     unit size, the value will be reset and the pointer and current data
             *                                     values will be updated.
             *
             * \param[in,out] currentDataPointer   The current pointer to the data.  This should point to the location
             *                                     being operated on.  The pointer will be advanced if the shift value
             *                                     rolls over.
             *
             * \param[in,out] currentData          The current data value.  This value will be updated if the shift
             *                                     value rolls over.
             *
             * \param[in]     shiftAmount          The amount to adjust the shift value by.
             *
             * \param[in]     columnValue          The column value to be OR'd into the currentData value.
             *
             * \return Returns the bits at the specified location.
             */
            static void writeAndAdjustDestinationPointer(
                unsigned&     currentBitShiftValue,
                StorageUnit*& currentDataPointer,
                StorageUnit&  currentData,
                unsigned      shiftAmount,
                StorageUnit   columnValue
            );

            /**
             * Method that performs a bit reversal of a storage unit.
             *
             * \param[in] v The value to be reversed.
             *
             * \return Returns the bit reversed storage unit.
             */
            static StorageUnit reverse(const StorageUnit* v);

            /**
             * Method that performs a bit reversal of an arbitrary length bit field.
             *
             * \param[in] v    The value to be reversed.
             *
             * \param[in] size The bit field size, in bits.
             *
             * \return Returns the bit reversed storage unit.
             */
            static StorageUnit reverse(StorageUnit v, unsigned size);

            /**
             * The size of the storage unit, in bits.
             */
            static constexpr unsigned storageUnitSizeInBits = sizeof(StorageUnit) * 8;

            /**
             * The current number of rows in the matrix.
             */
            unsigned long currentNumberRows;

            /**
             * The current number of columns in the matrix.
             */
            unsigned long currentNumberColumns;

            /**
             * The current column storage size, in bits.
             */
            unsigned long long columnSizeInBits;

            /**
             * The total allocation size, in storage units.
             */
            unsigned long long currentAllocation;

            /**
             * Pointer to the raw data storage.
             */
            StorageUnit* rawData;
    };
}

#endif
