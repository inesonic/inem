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
* This header defines the private implementation data store for integer dense matrices.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_MATRIX_INTEGER_DENSE_DATA_H
#define MODEL_MATRIX_INTEGER_DENSE_DATA_H

#include <string>

#include "model_api_types.h"
#include "model_intrinsic_types.h"
#include "m_matrix_sparse_private.h"
#include "m_matrix_dense_private.h"
#include "m_matrix_complex.h"
#include "m_matrix_real.h"
#include "m_matrix_boolean.h"
#include "m_matrix_integer_data.h"
#include "m_matrix_integer.h"

namespace M {
    /**
     * Private implementation class for \ref M::MatrixInteger.  This class provides the data store and implementation
     * for real matricies using a dense data format.
     */
    class MatrixInteger::DenseData:public MatrixInteger::Data, public MatrixDensePrivate<Integer> {
        friend class MatrixDensePrivate<Scalar>;
        friend class MatrixReal::DenseData;
        friend class MatrixComplex::DenseData;

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
            DenseData(
                unsigned long      newNumberRows,
                unsigned long      newNumberColumns,
                unsigned long long newAllocationInBytes,
                void*              rawDataBaseAddress
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
            DenseData(
                const MatrixDensePrivate<Scalar>& other,
                unsigned long long                newAllocationInBytes,
                void*                             rawDataBaseAddress
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
             * \param[in] newAllocationInBytes The size of the allocated data structure, entries.
             *
             * \param[in] rawDataBaseAddress   The base address of the raw data stored in the matrix.
             */
            DenseData(
                const MatrixDensePrivate<Scalar>& other,
                unsigned long                     newNumberRows,
                unsigned long                     newNumberColumns,
                unsigned long long                newAllocationInBytes,
                void*                             rawDataBaseAddress
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
            static inline DenseData* create(
                    unsigned long newNumberRows,
                    unsigned long newNumberColumns
                ) {
                return MatrixDensePrivate<Scalar>::create<DenseData>(newNumberRows, newNumberColumns);
            }

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
            static inline DenseData* create(
                    unsigned long newNumberRows,
                    unsigned long newNumberColumns,
                    const Scalar* matrixData
                ) {
                return MatrixDensePrivate<Scalar>::create<DenseData>(newNumberRows, newNumberColumns, matrixData);
            }

            /**
             * Method that translates a matrix of a different type to this type.
             *
             * \param[in] other The matrix data to be translated.
             *
             * \return Returns the translated matrix.  A null pointer will be returned if the matrix could not be
             *         translated.
             */
            static DenseData* create(const MatrixBoolean::DenseData& other);

            /**
             * Method that translates a matrix of a different type to this type.
             *
             * \param[in] other The matrix data to be translated.
             *
             * \return Returns the translated matrix.  A null pointer will be returned if the matrix could not be
             *         translated.
             */
            static DenseData* create(const MatrixReal::DenseData& other);

            /**
             * Method that translates a matrix of a different type to this type.
             *
             * \param[in] other The matrix data to be translated.
             *
             * \return Returns the translated matrix.  A null pointer will be returned if the matrix could not be
             *         translated.
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
            static inline DenseData* createUninitialized(
                    unsigned long newNumberRows,
                    unsigned long newNumberColumns
                ) {
                return MatrixDensePrivate<Scalar>::createUninitialized<DenseData>(newNumberRows, newNumberColumns);
            }

            /**
             * Method that translates a matrix of a different type to this type.
             *
             * \param[in] other The matrix data to be copied.
             *
             * \return Returns true on success, returns false
             */
            static DenseData* create(const MatrixDensePrivate<Integer>& other);

            /**
             * Method you can call to destroy an instance of this class.
             *
             * \param[in] instance The instance to be destroyed.
             */
            static inline void destroy(DenseData* instance) {
                MatrixDensePrivate<Scalar>::destroy(instance);
            }

            /**
             * Method you can use to clone this matrix.
             *
             * \return Returns a newly created instance of this matrix.
             */
            MatrixInteger::Data* clone() const override {
                return MatrixDensePrivate<Scalar>::create(*this);
            }

            /**
             * Method you can use to construct an identity matrix for this type.
             *
             * \param[in] numberRows    The number of rows in the matrix.
             *
             * \param[in] numberColumns The number of columns in the matrix.
             */
            static DenseData* identity(Index numberRows, Index numberColumns);

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
                return ValueType::INTEGER;
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
                return MatrixDensePrivate<Scalar>::numberRows();
            }

            /**
             * Method you can use to determine the number of matrix columns.
             *
             * \return Returns the current number of matrix columns.
             */
            unsigned long numberColumns() const override {
                return MatrixDensePrivate<Scalar>::numberColumns();
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
            Scalar at(unsigned long rowIndex, unsigned long columnIndex) const override {
                return *reinterpret_cast<const Scalar*>(addressOf(rowIndex, columnIndex));
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
            Scalar& at(unsigned long rowIndex, unsigned long columnIndex) override {
                return *reinterpret_cast<Scalar*>(addressOf(rowIndex, columnIndex));
            }

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined left to right.
             *
             * \param[in] other The matrix to combine to the right of this matrix.
             *
             * \return Returns a new matrix representing the combination of the two matrices.
             */
            MatrixInteger::Data* combineLeftToRight(const MatrixDensePrivate<Scalar>& other) const override;

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined left to right.
             *
             * \param[in] other The matrix to combine to the right of this matrix.
             *
             * \return Returns a new matrix representing the combination of the two matrices.
             */
            MatrixInteger::Data* combineLeftToRight(const MatrixSparsePrivate<Scalar>& other) const override;

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined top to bottom.
             *
             * \param[in] other The matrix to combine below this matrix.
             *
             * \return Returns a new matrix representing the combination of the two matrices.
             */
            MatrixInteger::Data* combineTopToBottom(const MatrixDensePrivate<Scalar>& other) const override;

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined top to bottom.
             *
             * \param[in] other The matrix to combine below this matrix.
             *
             * \return Returns a new matrix representing the combination of the two matrices.
             */
            MatrixInteger::Data* combineTopToBottom(const MatrixSparsePrivate<Scalar>& other) const override;

            /**
             * Method you can use to reverse the columns in this matrix.
             *
             * \return Returns a column-reversed version of this matrix.
             */
            MatrixInteger::Data* columnReverse() const override;

            /**
             * Method you can use to reverse the rows in this matrix.
             *
             * \return Returns a row-reversed version of this matrix.
             */
            MatrixInteger::Data* rowReverse() const override;

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
            MatrixInteger::Data* resizeTo(
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
             * Method you can use to obtain the raw data tracked by this matrix.
             *
             * \return Returns a void pointer to the raw data.
             */
            const Scalar* data() const override {
                return MatrixDensePrivate<Scalar>::data();
            }

            /**
             * Method you can use to obtain the raw data tracked by this matrix.
             *
             * \return Returns a void pointer to the raw data.
             */
            Scalar* data() override {
                return MatrixDensePrivate<Scalar>::data();
            }

            /**
             * Method that calculates the Hadamard or Schur product of this matrix and another matrix.
             *
             * \param[in] other The matrix to multiply against this matrix.
             *
             * \return Returns the Hadamard product of the two matrices.
             */
            Data* hadamard(const MatrixDensePrivate<Scalar>& other) const override;

            /**
             * Method that calculates the Hadamard or Schur product of this matrix and another matrix.
             *
             * \param[in] other The matrix to multiply against this matrix.
             *
             * \return Returns the Hadamard product of the two matrices.
             */
            Data* hadamard(const MatrixSparsePrivate<Scalar>& other) const override;

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
            Data* kronecker(
                const MatrixDensePrivate<Scalar>& other,
                bool                              otherTranspose,
                Scalar                            scalarMultiplier
            ) const override;

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
            Data* kronecker(
                const MatrixSparsePrivate<Scalar>& other,
                bool                               otherTranspose,
                Scalar                             scalarMultiplier
            ) const override;

            /**
             * Method you can use to generate an arbitrary matrix containing random integers.
             *
             * \param[in] pt            The per-thread instance used for random number generation.
             *
             * \param[in] numberRows    The number of rows.
             *
             * \param[in] numberColumns The number of columns.
             *
             * \return Returns a matrix containing the requested distribution.
             */
            static DenseData* randomInteger64(PerThread& pt, Integer numberRows, Integer numberColumns);

            /**
             * Method you can use to generate an arbitrary matrix containing 32-bit unsigned random integers.
             *
             * \param[in] pt            The per-thread instance used for random number generation.
             *
             * \param[in] numberRows    The number of rows.
             *
             * \param[in] numberColumns The number of columns.
             *
             * \return Returns a matrix containing the requested distribution.
             */
            static DenseData* randomInteger32(PerThread& pt, Integer numberRows, Integer numberColumns);

            /**
             * Method you can use to generate an arbitrary matrix containing random values from a Poisson distribution.
             *
             * \param[in] pt            The per-thread instance used for random number generation.
             *
             * \param[in] numberRows    The number of rows.
             *
             * \param[in] numberColumns The number of columns.
             *
             * \param[in] rate          The distribution rate parameter.
             *
             * \return Returns a matrix containing the requested distribution.
             */
            static MatrixInteger::DenseData* randomPoisson(
                PerThread& pt,
                Integer    numberRows,
                Integer    numberColumns,
                Real       rate
            );

            /**
             * Method you can use to generate an arbitrary matrix containing random values from a binomial
             * distribution.
             *
             * \param[in] pt            The per-thread instance used for random number generation.
             *
             * \param[in] numberRows    The number of rows.
             *
             * \param[in] numberColumns The number of columns.
             *
             * \param[in] n             The number of trials.
             *
             * \param[in] p             The success probability for each trial.
             *
             * \return Returns a matrix containing the requested distribution.
             */
            static MatrixInteger::DenseData* randomBinomial(
                PerThread& pt,
                Integer    numberRows,
                Integer    numberColumns,
                Integer    n,
                Real       p
            );

            /**
             * Method you can use to generate an arbitrary matrix containing random values from a geometric
             * distribution.
             *
             * \param[in] pt            The per-thread instance used for random number generation.
             *
             * \param[in] numberRows    The number of rows.
             *
             * \param[in] numberColumns The number of columns.
             *
             * \param[in] p             The success probability.
             *
             * \return Returns a matrix containing the requested distribution.
             */
            static MatrixInteger::DenseData* randomGeometric(
                PerThread& pt,
                Integer    numberRows,
                Integer    numberColumns,
                Real       p
            );

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
            Data* add(
                const MatrixDensePrivate<Scalar>& addend,
                bool                              augendTranspose,
                bool                              addendTranspose,
                Scalar                            augendScalar,
                Scalar                            addendScalar
            ) const override;

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
            Data* add(
                const MatrixSparsePrivate<Scalar>& addend,
                bool                               augendTranspose,
                bool                               addendTranspose,
                Scalar                             augendScalar,
                Scalar                             addendScalar
            ) const override;

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
            Data* multiply(
                const MatrixDensePrivate<Scalar>& multiplicand,
                bool                              multiplierTranspose,
                bool                              multiplicandTranspose,
                Scalar                            scalerMultiplier
            ) const override;

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
            Data* multiply(
                const MatrixSparsePrivate<Scalar>& multiplicand,
                bool                               multiplierTranspose,
                bool                               multiplicandTranspose,
                Scalar                             scalerMultiplier
            ) const override;

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
            Data* divide(bool transpose, Integer scalarMultiplier, Integer scalarDivisor) const override;

            /**
             * Method that compares this matrix against another matrix.  This version supports integer types.
             *
             * \param[in] other The matrix to be compared against.
             *
             * \return Returns true if the matrices are equal.  Returns false if the matrices are not equal.
             */
            bool isEqualTo(const MatrixDensePrivate<Scalar>& other) const override;

            /**
             * Method that compares this matrix against another matrix.  This version supports integer types.
             *
             * \param[in] other The matrix to be compared against.
             *
             * \return Returns true if the matrices are equal.  Returns false if the matrices are not equal.
             */
            bool isEqualTo(const MatrixSparsePrivate<Scalar>& other) const override;

            /**
             * Method that applies a requested transform and scaling.
             *
             * \param[in] transpose   If true, the matrix will be transposed.
             *
             * \param[in] scaleFactor The desired scale factor.
             *
             * \return Returns a pointer to the newly transformed and scaled instance.
             */
            Data* applyTransformAndScaling(bool transpose, Scalar scaleFactor) const override;

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
            int relativeOrder(const MatrixInteger::Data& other) const override;

        private:
            /**
             * Method that adds two vectors with scaling.  This method performs the operation:
             *
             * \f[
             *     result _ i = \alpha \times a _ i + \beta \times b _ i
             * \f]
             *
             * \param[in] numberTerms       The number of terms to be added together.
             *
             * \param[in] augendBase        The base address of the augend to be added (a).
             *
             * \param[in] augendIncrement   The increment to apply to the augend entries.
             *
             * \param[in] augendScaleFactor A scale factor to apply to the augend before addition.
             *
             * \param[in] addendBase        The base address of the addend to be added.
             *
             * \param[in] addendIncrement   The increment to apply to the addend entries.
             *
             * \param[in] addendScaleFactor A scale factor to apply to the addend before addition.
             *
             * \param[in] resultBase        The base address of the result location.
             *
             * \param[in] resultIncrement   The increment to apply to the result location.
             */
            static void vectorAdd(
                unsigned long long       numberTerms,
                const Scalar*            augendBase,
                const unsigned long long augendIncrement,
                const Scalar             augendScaleFactor,
                const Scalar*            addendBase,
                const unsigned long long addendIncrement,
                const Scalar             addendScaleFactor,
                Scalar*                  resultBase,
                const unsigned long long resultIncrement
            );

            /**
             * Method that performs a dot product of two vectors, with scaling.  This method performs the operation:
             *
             * \f[
             *     result _ i = \alpha \times a _ i \times b _ i
             * \f]
             *
             * \param[in] numberTerms           The number of terms to be multiplied together.
             *
             * \param[in] scaleFactor           The scale factor to apply.
             *
             * \param[in] multiplierBase        The base address of the augend to be added (a).
             *
             * \param[in] multiplierIncrement   The increment to apply to the augend entries.
             *
             * \param[in] multiplicandBase      The base address of the addend to be added.
             *
             * \param[in] multiplicandIncrement The increment to apply to the addend entries.
             *
             * \param[in] resultBase            The base address of the result location.
             *
             * \param[in] resultIncrement       The increment to apply to the result location.
             */
            static void vectorDotProduct(
                unsigned long long       numberTerms,
                const Scalar             scaleFactor,
                const Scalar*            multiplierBase,
                const unsigned long long multiplierIncrement,
                const Scalar*            multiplicandBase,
                const unsigned long long multiplicandIncrement,
                Scalar*                  resultBase,
                const unsigned long long resultIncrement
            );

            /**
             * Method that performs a vector/scalar product.  This method performs the operation:
             *
             * \f[
             *     result _ i = \alpha \times a _ i
             * \f]
             *
             * \param[in] numberTerms     The number of terms to be multiplied.
             *
             * \param[in] scaleFactor     The scale factor to apply.
             *
             * \param[in] base            The base address of the augend to be added (a).
             *
             * \param[in] increment       The increment to apply to the augend entries.
             *
             * \param[in] resultBase      The base address of the result location.
             *
             * \param[in] resultIncrement The increment to apply to the result location.
             */
            static void vectorScalarProduct(
                unsigned long long       numberTerms,
                const Scalar             scaleFactor,
                const Scalar*            base,
                const unsigned long long increment,
                Scalar*                  resultBase,
                const unsigned long long resultIncrement
            );

            /**
             * Method that performs a vector/scalar multiply and divide.  This method performs the operation:
             *
             * \f[
             *     result _ i = \frac{\alpha \times a _ i}{\beta}
             * \f]
             *
             * \param[in] numberTerms      The number of terms to be multiplied.
             *
             * \param[in] scalarMultiplier The scalar multiplier factor to apply.
             *
             * \param[in] scalarDivisor    The scalar divisor to apply.
             *
             * \param[in] base             The base address of the augend to be added (a).
             *
             * \param[in] increment        The increment to apply to the augend entries.
             *
             * \param[in] resultBase       The base address of the result location.
             *
             * \param[in] resultIncrement  The increment to apply to the result location.
             */
            static void vectorScalarQuotient(
                unsigned long long       numberTerms,
                const Scalar             scalarMultiplier,
                const Scalar             scalarDivisor,
                const Scalar*            base,
                const unsigned long long increment,
                Scalar*                  resultBase,
                const unsigned long long resultIncrement
            );

            /**
             * Method that performs a matrix/scalar product.  This method performs the operation:
             *
             * If no transposition is requested, this method performs the operation:
             *
             *     \f[
             *         result _ { i,j } = \alpha \times a _ { i, j }
             *     \f]
             *
             * If transposition is requested, this method will transpose during the copy operation, performing:
             *
             *     \f[
             *         result _ { i,j } = \alpha \times a _ { j, i }
             *     \f]
             *
             * \param[in] numberOutputRows             The number of rows in the output matrix.
             *
             * \param[in] numberOutputColumns          The number of columns in the output matrix.
             *
             * \param[in] transpose                    If true, the input matrix should be transposed.  If false, the
             *                                         input matrix should not be transposed.
             *
             * \param[in] scaleFactor                  The scale factor to apply.
             *
             * \param[in] inputBase                    The base address of the input matrix.
             *
             * \param[in] inputColumnSpacingInEntries  The spacing between columns, in entries.
             *
             * \param[in] resultBase                   The base address of the result location.
             *
             * \param[in] resultColumnSpacingInEntries The spacing between columns in the result, in entries.
             */
            static void matrixScalarProduct(
                unsigned long            numberOutputRows,
                unsigned long            numberOutputColumns,
                bool                     transpose,
                const Scalar             scaleFactor,
                const Scalar*            inputBase,
                const unsigned long long inputColumnSpacingInEntries,
                Scalar*                  resultBase,
                const unsigned long long resultColumnSpacingInEntries
            );

            /**
             * Method that converts a string to a numerical value.
             *
             * \param[in]  str The string to be converted.
             *
             * \param[out] ok  A pointer to a boolean value.  If not null, the boolean value will hold true on
             *                 success, false on error.
             *
             * \return Returns the numerical value.
             */
            static Scalar convertString(const std::string& str, bool* ok = nullptr);
    };
}

#endif
