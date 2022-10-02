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
* This header defines the private data base class for the \ref M::MatrixReal class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_MATRIX_REAL_DATA_H
#define M_MATRIX_REAL_DATA_H

#include "m_api_types.h"
#include "m_intrinsic_types.h"
#include "m_reference_counter.h"
#include "m_matrix_dense_private.h"
#include "m_matrix_sparse_private.h"
#include "m_matrix_integer_data.h"
#include "m_matrix_complex_data.h"
#include "m_matrix_real.h"

namespace M {
    /**
     * Base class for the data store for real matrices.
     */
    class MatrixReal::Data:public M::ReferenceCounter {
        public:
            virtual ~Data() = default;

            /**
             * Method you can call to destroy an instance of this class.
             *
             * \param[in] instance The instance to be destroyed.
             */
            static inline void destroy(MatrixReal::Data* instance) {
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
            virtual MatrixReal::Data* combineLeftToRight(const MatrixDensePrivate<Scalar>& other) const = 0;

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined left to right.
             *
             * \param[in] other The matrix to combine to the right of this matrix.
             *
             * \return Returns a new matrix representing the combination of the two matrices.
             */
            virtual MatrixReal::Data* combineLeftToRight(const MatrixSparsePrivate<Scalar>& other) const = 0;

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined top to bottom.
             *
             * \param[in] other The matrix to combine below this matrix.
             *
             * \return Returns a new matrix representing the combination of the two matrices.
             */
            virtual MatrixReal::Data* combineTopToBottom(const MatrixDensePrivate<Scalar>& other) const = 0;

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined top to bottom.
             *
             * \param[in] other The matrix to combine below this matrix.
             *
             * \return Returns a new matrix representing the combination of the two matrices.
             */
            virtual MatrixReal::Data* combineTopToBottom(const MatrixSparsePrivate<Scalar>& other) const = 0;

            /**
             * Method you can use to reverse the columns in this matrix.
             *
             * \return Returns a column-reversed version of this matrix.
             */
            virtual MatrixReal::Data* columnReverse() const = 0;

            /**
             * Method you can use to reverse the rows in this matrix.
             *
             * \return Returns a row-reversed version of this matrix.
             */
            virtual MatrixReal::Data* rowReverse() const = 0;

            /**
             * Method you can use to obtain the raw data tracked by this matrix.
             *
             * \return Returns a void pointer to the raw data.
             */
            virtual const Real* data() const = 0;

            /**
             * Method you can use to obtain the raw data tracked by this matrix.
             *
             * \return Returns a void pointer to the raw data.
             */
            virtual Real* data() = 0;

            /**
             * Method you can use to obtain a matrix holding the floor of each value in the matrix.
             *
             * \return Returns a real matrix with each value equal to the floor of the matrix contents.
             */
            virtual Data* floor() const = 0;

            /**
             * Method you can use to obtain a matrix holding the ceiling of each value in the matrix.
             *
             * \return Returns a real matrix with each value equal to the ceiling of the matrix contents.
             */
            virtual Data* ceil() const = 0;

            /**
             * Method you can use to obtain a matrix holding the nearest integer to each value in the matrix.
             *
             * \return Returns a real matrix with each value equal to the nearest integer of each coefficeint of the
             *         matrix contents.
             */
            virtual Data* nint() const = 0;

            /**
             * Method you can use to convert this matrix to an integer matrix.
             *
             * \return Returns this matrix converted to an integer matrix.  Values are truncated towards zero.
             */
            virtual MatrixInteger::Data* truncateToInteger() const = 0;

            /**
             * Method you can use to convert this matrix to an integer matrix.  Integer values represent the floor of
             * the supplied values.
             *
             * \return Returns this matrix converted to an integer matrix.  Values are truncated towards negative
             *         infinity.
             */
            virtual MatrixInteger::Data* floorToInteger() const = 0;

            /**
             * Method you can use to convert this matrix to an integer matrix.  Integer values represent the ceiling
             * of the supplied values.
             *
             * \return Returns this matrix converted to an integer matrix.  Values are truncated towards positive
             *         infinity.
             */
            virtual MatrixInteger::Data* ceilToInteger() const = 0;

            /**
             * Method you can use to convert this matrix to an integer matrix.  Integer values represent the nearest
             * integer of the supplied values.
             *
             * \return Returns this matrix converted to an integer matrix.  Values are truncated to the closest
             *         integer value.
             */
            virtual MatrixInteger::Data* nintToInteger() const = 0;

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
             * Method that returns the determinant of this matrix.
             *
             * \return Returns the determinant of this value.
             */
            virtual Scalar determinant() const = 0;

            /**
             * Method that returns the inverse of the matrix.
             *
             * \return Returns the inverse matrix.  Returns an empty matrix if inverse could not be determined.
             */
            virtual Data* inverse() const = 0;

            /**
             * Method that performs PLU decomposition of this matrix.
             *
             * \param[out] p The pivot matrix.  You are expected to take ownership of this matrix.
             *
             * \param[out] l The resulting lower triangular matrix.  You are expected to take ownership of this matrix.
             *
             * \param[out] u The resulting upper triangular matrix.  You are expected to take ownership of this matrix.
             *
             * \return Returns true on success, returns false if the resulting upper matrix is singular.
             */
            virtual bool plu(Data*& p, Data*& l, Data*& u) const = 0;

            /**
             * Method that performs singular value decomposition of this matrix.
             *
             * \param[out] u  The unitary matrix.  You are expected to take ownership of this matrix.
             *
             * \param[out] s  The singular value matrix.  You are expected to take ownership of this matrix.
             *
             * \param[out] vh The left and right singular vector matrix.  You are expected to take ownership of this
             *                matrix.
             *
             * \return Returns true on success, returns false if the resulting upper matrix is singular.
             */
            virtual bool svd(Data*& u, Data*& s, Data*& vh) const = 0;

            /**
             * Method that performs QR decomposition of this matrix.
             *
             * \param[out] q The unitary matrix.  You are expected to take ownership of this matrix.
             *
             * \param[out] r The upper triangular matrix.  You are expected to take ownership of this matrix.
             *
             * \return Returns true on success, false if the decomposition could not be performed.
             */
            virtual bool qr(Data*& q, Data*& r) const = 0;

            /**
             * Method that performs Cholesky decomposition of this matrix.  The matrix must be Hermitian.
             *
             * \return Returns the lower triangular matrix representing the Cholesky decomposition of this matrix.
             *         An empty matrix will be returned on failure.
             */
            virtual Data* cholesky() const = 0;

            /**
             * Method that performs Cholesky decomposition of this matrix, creating an upper triangular matrix.  The
             * matrix must be Hermitian.
             *
             * \return Returns the upper triangular matrix representing the Cholesky decomposition of this matrix.
             *         An empty matrix will be returned on failure.
             */
            virtual Data* upperCholesky() const = 0;

            /**
             * Method that reduces this matrix to upper Hessenberg form.
             *
             * \param[out] qData The Q unitary matrix.  You are expected to take ownership of this matrix.
             *
             * \param[out] hData The Hessenberg matrix.  You are expected to take ownership of this matrix.
             */
            virtual void hessenberg(Data*& qData, Data*& hData) const = 0;

            /**
             * Method that calculates the forward or type 2 discrete cosine transform (DCT) of this matrix.  A matrix
             * with a single dimension will be processed as a one-dimensional DCT.  A matrix with multiple rows and
             * columns will be processed as a two dimensional DCT.
             *
             * \return Returns a matrix representing the complex discrete cosine transform of the matrix.
             */
            virtual Data* dct() const = 0;

            /**
             * Method that calculates the inverse or type 3 discrete cosine transform (DCT) of this matrix.  A matrix
             * with a single dimension will be processed as a one dimensional inverse DFT.  A matrix with multiple rows
             * and columns will be processed as a two dimensional inverse DCT.
             *
             * \return Returns a matrix representing the complex inverse discrete cosine transform of the matrix.
             */
            virtual Data* idct() const = 0;

            /**
             * Method that calculates the Hilbert transform of an array (1 dimensional matrix).
             *
             * \return Returns the Hilbert transform of this matrix.
             */
            virtual MatrixComplex::Data* hilbertTransform() const = 0;

            /**
             * Method that calculates the entry-wise p-norm of this matrix.
             *
             * \param[in] p The power term to apply to the norm operation.
             *
             * \return Returns the p-norm of this matrix.
             */
            virtual Real pNorm(Integer p) const = 0;

            /**
             * Method that calculates the 1-norm of this matrix.
             *
             * \return Returns the 1-normnorm of this matrix.
             */
            virtual Real oneNorm() const = 0;

            /**
             * Method that calculates the infinity-norm of this matrix.
             *
             * \return Returns the infinity-norm of this matrix.
             */
            virtual Real infinityNorm() const = 0;

            /**
             * Method that calculates a set of row and column factors you can use to equilibrate a matrix.
             *
             * \param[out] row    The row scaling matrix.  You are expected to take ownership of this matrix.
             *
             * \param[out] column The column scaling matrix.  You are expected to take ownership of this matrix.
             *
             * \return Returns true on success.  Returns false if one or more row of the matrix is exactly 0.
             */
            virtual bool equilibrate(Data*& row, Data*& column) const = 0;

            /**
             * Method that solves the system of linear equations specified by this matrix.  For a system of linear
             * equations defined by:
             *
             * \f[
             *     A \times X = Y
             * \f]
             *
             * Where 'A' is represented by this matrix and Y is a row vector defining the results, this method will
             * return approximate values for 'X'.
             *
             * \param[in] y The row matrix holding the results of the product \f[ A \times X \f].
             *
             * \return Returns the resulting values of 'X'.  An empty matrix will be returned if A is singular.
             */
            virtual Data* solve(const MatrixReal::DenseData* y) const = 0;

            /**
             * Method that solves the system of linear equations specified by this matrix.  For a system of linear
             * equations defined by:
             *
             * \f[
             *     A \times X = Y
             * \f]
             *
             * Where 'A' is represented by this matrix and Y is a row vector defining the results, this method will
             * return approximate values for 'X'.
             *
             * \param[in] y The row matrix holding the results of the product \f[ A \times X \f].
             *
             * \return Returns the resulting values of 'X'.  An empty matrix will be returned if A is singular.
             */
            virtual Data* solve(const MatrixReal::SparseData* y) const = 0;

            /**
             * Method that solves and over/under determined system using QR or LQ factorization.  When provided an
             * under-determined matrix, this method will determine the minimum Euclidean-norm solution.
             *
             * For over-determined systems, this method will find the solution that exactly satisfies \f[ Ax = b \f].  For
             * under-determined system, this method will find the solution that identifies the value of \f[ x \f] that
             * minimizes \f[ \Vert\Vert A x - b \Vert\Vert _ 2 \f].
             *
             * \param[in] b The row matrix holding the results of the product \f[ A \times X \f].
             *
             * \return Returns the resulting values of 'x'.  An empty matrix will be returned on error.
             */
            virtual Data* leastSquares(const MatrixReal::DenseData* b) const = 0;

            /**
             * Method that solves and over/under determined system using QR or LQ factorization.  When provided an
             * under-determined matrix, this method will determine the minimum Euclidean-norm solution.
             *
             * For over-determined systems, this method will find the solution that exactly satisfies \f[ Ax = b \f].
             * For under-determined system, this method will find the solution that identifies the value of \f[ x \f]
             * that minimizes \f[ \Vert\Vert A x - b \Vert\Vert _ 2 \f].
             *
             * \param[in] b The row matrix holding the results of the product \f[ A \times X \f].
             *
             * \return Returns the resulting values of 'x'.  An empty matrix will be returned on error.
             */
            virtual Data* leastSquares(const MatrixReal::SparseData* b) const = 0;

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
                Real                              augendScalar,
                Real                              addendScalar
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
                Real                               augendScalar,
                Real                               addendScalar
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
                Real                              scalerMultiplier
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
                Real                               scalerMultiplier
            ) const = 0;

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
            virtual int relativeOrder(const MatrixReal::Data& other) const = 0;
    };
}

#endif
