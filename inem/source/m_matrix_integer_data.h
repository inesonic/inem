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
* This header defines the private data base class for the \ref M::MatrixInteger class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_MATRIX_INTEGER_DATA_H
#define M_MATRIX_INTEGER_DATA_H

#include "m_api_types.h"
#include "m_intrinsic_types.h"
#include "m_reference_counter.h"
#include "m_matrix_dense_private.h"
#include "m_matrix_sparse_private.h"
#include "m_matrix_integer.h"

namespace M {
    /**
     * Base class for the data store for integer matrices.
     */
    class MatrixInteger::Data:public M::ReferenceCounter {
        public:
            virtual ~Data() = default;

            /**
             * Method you can call to destroy an instance of this class.
             *
             * \param[in] instance The instance to be destroyed.
             */
            static inline void destroy(MatrixInteger::Data* instance) {
                MatrixDensePrivate<Scalar>::destroy(instance);
            }

            /**
             * Method you can use to clone this matrix.
             *
             * \return Returns a newly created instance of this matrix.
             */
            virtual Data* clone() const = 0;

            /**
             * Method you can use to determine the array type.
             *
             * \return Returns the array type being represented by this instance.
             */
            virtual Matrix::MatrixType matrixType() const = 0;

            /**
             * Method you can use to determine the type of coefficient stored in this matrix.
             *
             * \return Returns the coefficient type stored in the matrix.
             */
            virtual ValueType coefficientType() const = 0;

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
            virtual bool toFile(const char* filename, Model::DataFileFormat fileFormat) const = 0;

            /**
             * Method you can use to determine number of matrix rows.
             *
             * \return Returns the current number of rows.
             */
            virtual unsigned long numberRows() const = 0;

            /**
             * Method you can use to determine the number of matrix columns.
             *
             * \return Returns the current number of matrix columns.
             */
            virtual unsigned long numberColumns() const = 0;

            /**
             * Method you can use to obtain the value at a given location.
             *
             * \param[in] rowIndex    The zero based row index we want the entry from.
             *
             * \param[in] columnIndex The zero based column index we want the entry from.
             *
             * \return Returns the value at the specified location.
             */
            virtual Scalar at(unsigned long rowIndex, unsigned long columnIndex) const = 0;

            /**
             * Method you can use to obtain the value at a given location.
             *
             * \param[in] rowIndex    The zero based row index we want the entry from.
             *
             * \param[in] columnIndex The zero based column index we want the entry from.
             *
             * \return Returns the value at the specified location.
             */
            virtual Scalar& at(unsigned long rowIndex, unsigned long columnIndex) = 0;

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined left to right.
             *
             * \param[in] other The matrix to combine to the right of this matrix.
             *
             * \return Returns a new matrix representing the combination of the two matrices.
             */
            virtual MatrixInteger::Data* combineLeftToRight(const MatrixDensePrivate<Scalar>& other) const = 0;

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined left to right.
             *
             * \param[in] other The matrix to combine to the right of this matrix.
             *
             * \return Returns a new matrix representing the combination of the two matrices.
             */
            virtual MatrixInteger::Data* combineLeftToRight(const MatrixSparsePrivate<Scalar>& other) const = 0;

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined top to bottom.
             *
             * \param[in] other The matrix to combine below this matrix.
             *
             * \return Returns a new matrix representing the combination of the two matrices.
             */
            virtual MatrixInteger::Data* combineTopToBottom(const MatrixDensePrivate<Scalar>& other) const = 0;

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined top to bottom.
             *
             * \param[in] other The matrix to combine below this matrix.
             *
             * \return Returns a new matrix representing the combination of the two matrices.
             */
            virtual MatrixInteger::Data* combineTopToBottom(const MatrixSparsePrivate<Scalar>& other) const = 0;

            /**
             * Method you can use to reverse the columns in this matrix.
             *
             * \return Returns a column-reversed version of this matrix.
             */
            virtual MatrixInteger::Data* columnReverse() const = 0;

            /**
             * Method you can use to reverse the rows in this matrix.
             *
             * \return Returns a row-reversed version of this matrix.
             */
            virtual MatrixInteger::Data* rowReverse() const = 0;

            /**
             * Method you can use to obtain the raw data tracked by this matrix.
             *
             * \return Returns a void pointer to the raw data.
             */
            virtual const Integer* data() const = 0;

            /**
             * Method you can use to obtain the raw data tracked by this matrix.
             *
             * \return Returns a void pointer to the raw data.
             */
            virtual Integer* data() = 0;

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
            virtual Data* resizeTo(
                unsigned long newNumberRows,
                unsigned long newNumberColumns,
                bool          alwaysReallocate = false
            ) = 0;

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
            virtual bool resizeInPlaceTo(unsigned long newNumberRows, unsigned long newNumberColumns) = 0;

            /**
             * Method that calculates the Hadamard or Schur product of this matrix and another matrix.
             *
             * \param[in] other The matrix to multiply against this matrix.
             *
             * \return Returns the Hadamard product of the two matrices.
             */
            virtual Data* hadamard(const MatrixDensePrivate<Scalar>& other) const = 0;

            /**
             * Method that calculates the Hadamard or Schur product of this matrix and another matrix.
             *
             * \param[in] other The matrix to multiply against this matrix.
             *
             * \return Returns the Hadamard product of the two matrices.
             */
            virtual Data* hadamard(const MatrixSparsePrivate<Scalar>& other) const = 0;

            /**
             * Method that calculates the Kronecker product of this matrix and another matrix.
             *
             * \param[in] other            The matrix to multiply against this matrix.
             *
             * \param[in] otherTranspose   If true, the other matrix should be transposed before performing this
             *                             opration.
             *
             * \param[in] scalarMultiplier A scalar multiplier to be applied.
             *
             * \return Returns the Kronecker product of the two matrices.
             */
            virtual Data* kronecker(
                const MatrixDensePrivate<Scalar>& other,
                bool                              otherTranspose,
                Scalar                            scalarMultiplier
            ) const = 0;

            /**
             * Method that calculates the Kronecker product of this matrix and another matrix.
             *
             * \param[in] other            The matrix to multiply against this matrix.
             *
             * \param[in] otherTranspose   If true, the other matrix should be transposed before performing this
             *                             opration.
             *
             * \param[in] scalarMultiplier A scalar multiplier to be applied.
             *
             * \return Returns the Kronecker product of the two matrices.
             */
            virtual Data* kronecker(
                const MatrixSparsePrivate<Scalar>& other,
                bool                               otherTranspose,
                Scalar                             scalarMultiplier
            ) const = 0;

            /**
             * Method you can call to add this matrix to another matrix.
             *
             * \param[in] addend          The instance to be added to this instance.
             *
             * \param[in] augendTranspose Indicates that transpose needs to be performed on the augend (left value)
             *                            prior to performing the addition.  The augend (this data instance) will not
             *                            be modified.
             *
             * \param[in] addendTranspose Indicates that transpose needs to be performed on the added (right value)
             *                            prior to performing the addition.  The addend will not be modified.
             *
             * \param[in] augendScalar    A scalar value to be multiplied to the augend prior to the addition.
             *
             * \param[in] addendScalar    A scalar value to be multiplied to the addend prior to the addition.
             *
             * \return Returns a new matrix representing the sum of the two matrices.
             */
            virtual Data* add(
                const MatrixDensePrivate<Scalar>& addend,
                bool                              augendTranspose,
                bool                              addendTranspose,
                Integer                           augendScalar,
                Integer                           addendScalar
            ) const = 0;

            /**
             * Method you can call to add this matrix to another matrix.
             *
             * \param[in] addend          The instance to be added to this instance.
             *
             * \param[in] augendTranspose Indicates that transpose needs to be performed on the augend (left value)
             *                            prior to performing the addition.  The augend (this data instance) will not
             *                            be modified.
             *
             * \param[in] addendTranspose Indicates that transpose needs to be performed on the added (right value)
             *                            prior to performing the addition.  The addend will not be modified.
             *
             * \param[in] augendScalar    A scalar value to be multiplied to the augend prior to the addition.
             *
             * \param[in] addendScalar    A scalar value to be multiplied to the addend prior to the addition.
             *
             * \return Returns a new matrix representing the sum of the two matrices.
             */
            virtual Data* add(
                const MatrixSparsePrivate<Scalar>& addend,
                bool                               augendTranspose,
                bool                               addendTranspose,
                Integer                            augendScalar,
                Integer                            addendScalar
            ) const = 0;

            /**
             * Method you can call to multiply this matrix to another matrix.
             *
             * \param[in] multiplicand          The instance to be multiplied to this instance.
             *
             * \param[in] multiplierTranspose   Indicates that a transpose needs to be performed on the multiplier
             *                                  during the multiplication. The multiplier will not be modified during
             *                                  the operation.
             *
             * \param[in] multiplicandTranspose Indicates that a transpose needs to be performed on the multiplicand
             *                                  during the multiplication. The multiplicand will not be modified during
             *                                  the operation.
             *
             * \param[in] scalerMultiplier      A scaler multiplier to be applied to the multiplication operation.
             *
             * \return Returns a new matrix representing the product of the two matrices.
             */
            virtual Data* multiply(
                const MatrixDensePrivate<Scalar>& multiplicand,
                bool                              multiplierTranspose,
                bool                              multiplicandTranspose,
                Integer                           scalerMultiplier
            ) const = 0;

            /**
             * Method you can call to multiply this matrix to another matrix.
             *
             * \param[in] multiplicand          The instance to be multiplied to this instance.
             *
             * \param[in] multiplierTranspose   Indicates that a transpose needs to be performed on the multiplier
             *                                  during the multiplication. The multiplier will not be modified during
             *                                  the operation.
             *
             * \param[in] multiplicandTranspose Indicates that a transpose needs to be performed on the multiplicand
             *                                  during the multiplication. The multiplicand will not be modified during
             *                                  the operation.
             *
             * \param[in] scalerMultiplier      A scaler multiplier to be applied to the multiplication operation.
             *
             * \return Returns a new matrix representing the product of the two matrices.
             */
            virtual Data* multiply(
                const MatrixSparsePrivate<Scalar>& multiplicand,
                bool                               multiplierTranspose,
                bool                               multiplicandTranspose,
                Integer                            scalerMultiplier
            ) const = 0;

            /**
             * Method you can call to divide this matrix by a scalar value.
             *
             * \param[in] transpose        Indicates that a transpose needs to be performed on the multiplier during
             *                             the multiplication. The multiplier will not be modified during the
             *                             operation.
             *
             * \param[in] scalarMultiplier A scaler multiplier to be applied to the multiplication operation.  The
             *                             scalar is applied before the divide operation.
             *
             * \param[in] scalarDivisor    The scalar divisor.  This will be applied after the multiplier.
             *
             * \return Returns a new matrix representing the quotient.
             */
            virtual Data* divide(bool transpose, Integer scalarMultiplier, Integer scalarDivisor) const = 0;

            /**
             * Method that compares this matrix against another matrix.
             *
             * \param[in] other The matrix to be compared against.
             *
             * \return Returns true if the matrices are equal.  Returns false if the matrices are not equal.
             */
            virtual bool isEqualTo(const MatrixDensePrivate<Scalar>& other) const = 0;

            /**
             * Method that compares this matrix against another matrix.
             *
             * \param[in] other The matrix to be compared against.
             *
             * \return Returns true if the matrices are equal.  Returns false if the matrices are not equal.
             */
            virtual bool isEqualTo(const MatrixSparsePrivate<Scalar>& other) const = 0;

            /**
             * Method that applies a requested transform and scaling.
             *
             * \param[in] transpose   If true, the matrix will be transposed.
             *
             * \param[in] scaleFactor The desired scale factor.
             *
             * \return Returns a pointer to the newly transformed and scaled instance.
             */
            virtual Data* applyTransformAndScaling(bool transpose, Scalar scaleFactor) const = 0;

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
            virtual int relativeOrder(const MatrixInteger::Data& other) const = 0;
    };
}

#endif
