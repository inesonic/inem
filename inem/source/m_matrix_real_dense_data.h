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
* This header defines the private implementation data store for real dense matrices.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_MATRIX_REAL_DENSE_DATA_H
#define MODEL_MATRIX_REAL_DENSE_DATA_H

#include <mat_api.h>

#include "model_api_types.h"
#include "model_intrinsic_types.h"
#include "m_matrix_sparse_private.h"
#include "m_matrix_dense_private.h"
#include "m_matrix_integer.h"
#include "m_matrix_integer_data.h"
#include "m_matrix_integer_dense_data.h"
#include "m_matrix_boolean.h"
//#include "m_matrix_boolean_data.h"
//#include "m_matrix_boolean_dense_data.h"
#include "m_matrix_complex.h"
#include "m_matrix_real_data.h"
#include "m_matrix_real.h"

namespace M {
    /**
     * Private implementation class for \ref M::MatrixReal.  This class provides the data store and implementation
     * for real matricies using a dense data format.
     */
    class MatrixReal::DenseData:public MatrixReal::Data, public MatrixDensePrivate<Real> {
        friend class MatrixDensePrivate<Scalar>;
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
             * Method that translates complex matrix data to this type.
             *
             * \param[in] other The matrix data to be translated.
             *
             * \return Returns the translated matrix.
             */
            static DenseData* create(const MatrixComplex::DenseData& other);

            /**
             * Method that translates integer matrix data to this type.
             *
             * \param[in] other The matrix data to be translated.
             *
             * \return Returns the translated matrix.
             */
            static DenseData* create(const MatrixInteger::DenseData& other);

            /**
             * Method that translates boolean matrix data to this type.
             *
             * \param[in] other The matrix data to be translated.
             *
             * \return Returns the translated matrix.
             */
            static DenseData* create(const MatrixBoolean::DenseData& other);

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
             * Method that translates a matrix of a different type to this type.
             *
             * \param[in] other The matrix data to be copied.
             *
             * \return Returns true on success, returns false
             */
            static DenseData* create(const MatrixDensePrivate<Real>& other);

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
            MatrixReal::Data* clone() const override {
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
                return ValueType::REAL;
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
            MatrixReal::Data* combineLeftToRight(const MatrixDensePrivate<Scalar>& other) const override;

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined left to right.
             *
             * \param[in] other The matrix to combine to the right of this matrix.
             *
             * \return Returns a new matrix representing the combination of the two matrices.
             */
            MatrixReal::Data* combineLeftToRight(const MatrixSparsePrivate<Scalar>& other) const override;

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined top to bottom.
             *
             * \param[in] other The matrix to combine below this matrix.
             *
             * \return Returns a new matrix representing the combination of the two matrices.
             */
            MatrixReal::Data* combineTopToBottom(const MatrixDensePrivate<Scalar>& other) const override;

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined top to bottom.
             *
             * \param[in] other The matrix to combine below this matrix.
             *
             * \return Returns a new matrix representing the combination of the two matrices.
             */
            MatrixReal::Data* combineTopToBottom(const MatrixSparsePrivate<Scalar>& other) const override;

            /**
             * Method you can use to reverse the columns in this matrix.
             *
             * \return Returns a column-reversed version of this matrix.
             */
            MatrixReal::Data* columnReverse() const override;

            /**
             * Method you can use to reverse the rows in this matrix.
             *
             * \return Returns a row-reversed version of this matrix.
             */
            MatrixReal::Data* rowReverse() const override;

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
            MatrixReal::Data* resizeTo(
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
             * Method you can use to obtain a matrix holding the floor of each value in the matrix.
             *
             * \return Returns a real matrix with each value equal to the floor of the matrix contents.
             */
            Data* floor() const override;

            /**
             * Method you can use to obtain a matrix holding the ceiling of each value in the matrix.
             *
             * \return Returns a real matrix with each value equal to the ceiling of the matrix contents.
             */
            Data* ceil() const override;

            /**
             * Method you can use to obtain a matrix holding the nearest integer to each value in the matrix.
             *
             * \return Returns a real matrix with each value equal to the nearest integer of each coefficeint of the
             *         matrix contents.
             */
            Data* nint() const override;

            /**
             * Method you can use to convert this matrix to an integer matrix.
             *
             * \return Returns this matrix converted to an integer matrix.  Values are truncated towards zero.
             */
            MatrixInteger::Data* truncateToInteger() const override;

            /**
             * Method you can use to convert this matrix to an integer matrix.  Integer values represent the floor of
             * the supplied values.
             *
             * \return Returns this matrix converted to an integer matrix.  Values are truncated towards negative
             *         infinity.
             */
            MatrixInteger::Data* floorToInteger() const override;

            /**
             * Method you can use to convert this matrix to an integer matrix.  Integer values represent the ceiling
             * of the supplied values.
             *
             * \return Returns this matrix converted to an integer matrix.  Values are truncated towards positive
             *         infinity.
             */
            MatrixInteger::Data* ceilToInteger() const override;

            /**
             * Method you can use to convert this matrix to an integer matrix.  Integer values represent the nearest
             * integer of the supplied values.
             *
             * \return Returns this matrix converted to an integer matrix.  Values are truncated to the closest
             *         integer value.
             */
            MatrixInteger::Data* nintToInteger() const override;

            /**
             * Method that returns the determinant of this matrix.
             *
             * \return Returns the determinant of this value.
             */
            Scalar determinant() const override;

            /**
             * Method that returns the inverse of the matrix.
             *
             * \return Returns the inverse matrix.  Returns an empty matrix if inverse could not be determined.
             */
            Data* inverse() const override;

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
            bool plu(Data*& p, Data*& l, Data*& u) const override;

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
            bool svd(Data*& u, Data*& s, Data*& vh) const override;

            /**
             * Method that performs QR decomposition of this matrix.
             *
             * \param[out] q The unitary matrix.  You are expected to take ownership of this matrix.
             *
             * \param[out] r The upper triangular matrix.  You are expected to take ownership of this matrix.
             *
             * \return Returns true on success, false if the decomposition could not be performed.
             */
            bool qr(Data*& q, Data*& r) const override;

            /**
             * Method that performs Cholesky decomposition of this matrix.  The matrix must be Hermitian.
             *
             * \return Returns the lower triangular matrix representing the Cholesky decomposition of this matrix.
             *         An empty matrix will be returned on failure.
             */
            Data* cholesky() const override;

            /**
             * Method that performs Cholesky decomposition of this matrix, creating an upper triangular matrix.  The
             * matrix must be Hermitian.
             *
             * \return Returns the upper triangular matrix representing the Cholesky decomposition of this matrix.
             *         An empty matrix will be returned on failure.
             */
            Data* upperCholesky() const override;

            /**
             * Method that reduces this matrix to upper Hessenberg form.
             *
             * \param[out] qData The Q unitary matrix.  You are expected to take ownership of this matrix.
             *
             * \param[out] hData The Hessenberg matrix.  You are expected to take ownership of this matrix.
             *
             * \return Returns true on success, returns false on error.
             */
            void hessenberg(Data*& qData, Data*& hData) const override;

            /**
             * Method that calculates the forward or type 2 discrete cosine transform (DCT) of this matrix.  A matrix
             * with a single dimension will be processed as a one-dimensional DCT.  A matrix with multiple rows and
             * columns will be processed as a two dimensional DCT.
             *
             * \return Returns a matrix representing the complex discrete cosine transform of the matrix.
             */
            Data* dct() const override;

            /**
             * Method that calculates the inverse or type 3 discrete cosine transform (DCT) of this matrix.  A matrix
             * with a single dimension will be processed as a one dimensional inverse DFT.  A matrix with multiple rows
             * and columns will be processed as a two dimensional inverse DCT.
             *
             * \return Returns a matrix representing the complex inverse discrete cosine transform of the matrix.
             */
            Data* idct() const override;

            /**
             * Method that calculates the Hilbert transform of an array (1 dimensional matrix).
             *
             * \return Returns the Hilbert transform of this matrix.
             */
            MatrixComplex::Data* hilbertTransform() const override;

            /**
             * Method that calculates the entry-wise p-norm of this matrix.
             *
             * \param[in] p The power term to apply to the norm operation.
             *
             * \return Returns the p-norm of this matrix.
             */
            Real pNorm(Integer p) const override;

            /**
             * Method that calculates the 1-norm of this matrix.
             *
             * \return Returns the 1-normnorm of this matrix.
             */
            Real oneNorm() const override;

            /**
             * Method that calculates the infinity-norm of this matrix.
             *
             * \return Returns the infinity-norm of this matrix.
             */
            Real infinityNorm() const override;

            /**
             * Method that calculates a set of row and column factors you can use to equilibrate a matrix.
             *
             * \param[out] row    The row scaling matrix.  You are expected to take ownership of this matrix.
             *
             * \param[out] column The column scaling matrix.  You are expected to take ownership of this matrix.
             *
             * \return Returns true on success.  Returns false if one or more row of the matrix is exactly 0.
             */
            bool equilibrate(Data*& row, Data*& column) const override;

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
            Data* solve(const MatrixReal::DenseData* y) const override;

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
            Data* solve(const MatrixReal::SparseData* y) const override;

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
            Data* leastSquares(const MatrixReal::DenseData* b) const override;

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
            Data* leastSquares(const MatrixReal::SparseData* b) const override;

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
             * Method you can use to generate an arbitrary matrix containing uniform real values between 0 and 1,
             * inclusive.
             *
             * \param[in] pt            The per-thread instance used for random number generation.
             *
             * \param[in] numberRows    The number of rows.
             *
             * \param[in] numberColumns The number of columns.
             *
             * \return Returns a matrix containing the requested distribution.
             */
            static DenseData* randomInclusive(PerThread& pt, Integer numberRows, Integer numberColumns);

            /**
             * Method you can use to generate an arbitrary matrix containing uniform real values between [0, 1).
             *
             * \param[in] pt            The per-thread instance used for random number generation.
             *
             * \param[in] numberRows    The number of rows.
             *
             * \param[in] numberColumns The number of columns.
             *
             * \return Returns a matrix containing the requested distribution.
             */
            static DenseData* randomInclusiveExclusive(PerThread& pt, Integer numberRows, Integer numberColumns);

            /**
             * Method you can use to generate an arbitrary matrix containing uniform real values between (0, 1].
             *
             * \param[in] pt            The per-thread instance used for random number generation.
             *
             * \param[in] numberRows    The number of rows.
             *
             * \param[in] numberColumns The number of columns.
             *
             * \return Returns a matrix containing the requested distribution.
             */
            static DenseData* randomExclusiveInclusive(PerThread& pt, Integer numberRows, Integer numberColumns);

            /**
             * Method you can use to generate an arbitrary matrix containing uniform real values between 0 and 1,
             * exclusive.
             *
             * \param[in] pt            The per-thread instance used for random number generation.
             *
             * \param[in] numberRows    The number of rows.
             *
             * \param[in] numberColumns The number of columns.
             *
             * \return Returns a matrix containing the requested distribution.
             */
            static DenseData* randomExclusive(PerThread& pt, Integer numberRows, Integer numberColumns);

            /**
             * Method you can use to generate an arbitrary matrix containing values from a normal distribution.
             *
             * \param[in] pt            The per-thread instance used for random number generation.
             *
             * \param[in] numberRows    The number of rows.
             *
             * \param[in] numberColumns The number of columns.
             *
             * \return Returns a matrix containing the requested distribution.
             */
            static DenseData* randomNormal(PerThread& pt, Integer numberRows, Integer numberColumns);

            /**
             * Method you can use to generate an arbitrary matrix containing values from a normal distribution.
             *
             * \param[in] pt            The per-thread instance used for random number generation.
             *
             * \param[in] numberRows    The number of rows.
             *
             * \param[in] numberColumns The number of columns.
             *
             * \param[in] mean          The desired mean value.
             *
             * \param[in] sigma         The desired sigma.
             *
             * \return Returns a matrix containing the requested distribution.
             */
            static DenseData* randomNormal(
                PerThread& pt,
                Integer    numberRows,
                Integer    numberColumns,
                Real       mean,
                Real       sigma
            );

            /**
             * Method you can use to generate an arbitrary matrix containing values from a Weibull distribution.
             *
             * \param[in] pt            The per-thread instance used for random number generation.
             *
             * \param[in] numberRows    The number of rows.
             *
             * \param[in] numberColumns The number of columns.
             *
             * \param[in] scale         The scale term.
             *
             * \param[in] shape         The shape term.
             *
             * \param[in] delay         The delay term.
             *
             * \return Returns a matrix containing the requested distribution.
             */
            static DenseData* randomWeibull(
                PerThread& pt,
                Integer    numberRows,
                Integer    numberColumns,
                Real       scale,
                Real       shape,
                Real       delay
            );

            /**
             * Method you can use to generate a matrix containing values from an exponential distribution.
             *
             * \param[in] pt            The per-thread instance used for random number generation.
             *
             * \param[in] numberRows    The number of rows.
             *
             * \param[in] numberColumns The number of columns.
             *
             * \param[in] rate          The rate term.
             *
             * \return Returns a matrix containing the requested distribution.
             */
            static DenseData* randomExponential(PerThread& pt, Integer numberRows, Integer numberColumns, Real rate);

            /**
             * Method you can use to generate a matrix containing values from a gamma distribution.
             *
             * \param[in] pt            The per-thread instance used for random number generation.
             *
             * \param[in] numberRows    The number of rows.
             *
             * \param[in] numberColumns The number of columns.
             *
             * \param[in] k             The shape parameter.
             *
             * \param[in] s             The scale parameter.
             *
             * \return Returns a matrix containing the requested distribution.
             */
            static DenseData* randomGamma(
                PerThread& pt,
                Integer    numberRows,
                Integer    numberColumns,
                Real       k,
                Real       s
            );

            /**
             * Method you can use to generate a matrix containing values from a Rayleigh distribution.
             *
             * \param[in] pt            The per-thread instance used for random number generation.
             *
             * \param[in] numberRows    The number of rows.
             *
             * \param[in] numberColumns The number of columns.
             *
             * \param[in] scale         The scale parameter for the distribution.
             *
             * \return Returns a matrix containing the requested distribution.
             */
            static DenseData* randomRayleigh(PerThread& pt, Integer numberRows, Integer numberColumns, Real scale);

            /**
             * Method you can use to generate a matrix containing values from a Chi-sqared distribution.
             *
             * \param[in] pt            The per-thread instance used for random number generation.
             *
             * \param[in] numberRows    The number of rows.
             *
             * \param[in] numberColumns The number of columns.
             *
             * \param[in] k             The distribution degrees of freedom.
             *
             * \return Returns a matrix containing the requested distribution.
             */
            static DenseData* randomChiSquared(PerThread& pt, Integer numberRows, Integer numberColumns, Integer k);

            /**
             * Method you can use to generate a matrix containing values from a log normal distribution with zero mean
             * and unit sigma.
             *
             * \param[in] pt            The per-thread instance used for random number generation.
             *
             * \param[in] numberRows    The number of rows.
             *
             * \param[in] numberColumns The number of columns.
             *
             * \return Returns a matrix containing the requested distribution.
             */
            static DenseData* randomLogNormal(PerThread& pt, Integer numberRows, Integer numberColumns);

            /**
             * Method you can use to generate a matrix containing values from a log normal distribution.
             *
             * \param[in] pt            The per-thread instance used for random number generation.
             *
             * \param[in] numberRows    The number of rows.
             *
             * \param[in] numberColumns The number of columns.
             *
             * \param[in] mean          The mean value.
             *
             * \param[in] sigma         The standard deviation.
             *
             * \return Returns a matrix containing the requested distribution.
             */
            static DenseData* randomLogNormal(
                PerThread& pt,
                Integer    numberRows,
                Integer    numberColumns,
                Real       mean,
                Real       sigma
            );

            /**
             * Method you can use to obtain random values in a Cauchy-Lorentz distribution.
             *
             * \param[in] pt            The per-thread instance used for random number generation.
             *
             * \param[in] numberRows    The number of rows.
             *
             * \param[in] numberColumns The number of columns.
             *
             * \param[in] location      The location parameter.
             *
             * \param[in] scale         The scale parameter.
             *
             * \return Returns a matrix containing the requested distribution.
             */
            static DenseData* randomCauchyLorentz(
                PerThread& pt,
                Integer    numberRows,
                Integer    numberColumns,
                Real       location,
                Real       scale
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
             * Method that compares this matrix against another matrix.  This version supports real types.
             *
             * \param[in] other The matrix to be compared against.
             *
             * \return Returns true if the matrices are equal.  Returns false if the matrices are not equal.
             */
            bool isEqualTo(const MatrixDensePrivate<Scalar>& other) const override;

            /**
             * Method that compares this matrix against another matrix.  This version supports real types.
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
            int relativeOrder(const MatrixReal::Data& other) const override;

        private:
            /**
             * Method you can use to obtain the small limit before equilibration is recommeded.
             *
             * \return Returns the small limit for equilibration.
             */
            static long double smallLimit();

            /**
             * Method you can use to obtain the big limit before equilibration is recommeded.
             *
             * \return Returns the big limit for equilibration.
             */
            static long double bigLimit();

            /**
             * Method that converts a matrix pivot array into a row pivot matrix.
             *
             * \param[in] matPivotOperationsByRow Any array holding the pivot operations by row.
             *
             * \param[in] numberRows              The number of matrix rows.  The returned matrix will be a square
             *                                    matrix with this many rows and columns.
             *
             * \param[in] matPivotArraySize       The size of the pivot array, in entries.
             *
             * \return Returns a \ref M::MatrixReal::DenseData instance holding the pivot matrix.
             */
            static DenseData* generatePivotMatrix(
                const MatInteger* matPivotOperationsByRow,
                unsigned long     numberRows,
                unsigned long     matPivotArraySize
            );

            /**
             * Method that converts a vector to a vector of integer values.
             *
             * \param[in] numberEntries The number of entries in the vector.
             *
             * \param[in] padding       Number of entries to pad to zero in the destination.
             *
             * \param[in] source        Pointer to the first entry in the source vector.
             *
             * \param[in] destination   Pointer where the first entry in the destination should be stored.
             */
            static void toInteger(
                unsigned long          numberEntries,
                unsigned               padding,
                const Scalar*          source,
                MatrixInteger::Scalar* destination
            );

            /**
             * Method that generates an matrix of normal random values with mean of 0 and sigma of 1.0.  Residues and
             * tails in the matrix are not cleared to 0.
             *
             * \param[in] pt            The per-thread instance used for random number generation.
             *
             * \param[in] numberRows    The number of rows.
             *
             * \param[in] numberColumns The number of columns.
             *
             * \return Returns a matrix containing the calculated normal values.
             */
            static DenseData* randomNormalEngine(PerThread& pt, Integer numberRows, Integer numberColumns);

            /**
             * Method that multiplies each row against a unique scale factor.
             *
             * \param[in] sourceMatrix      The source matrix to be used.
             *
             * \param[in] rowScaleFactors   An array holding the row scale factors.
             *
             * \return Returns a newly created, row-scaled matrix.
             */
            static DenseData* multiplyRows(const DenseData& sourceMatrix, const double* rowScaleFactors);

            /**
             * Method that performs a DCT or inverse DCT.
             *
             * \param[in] dctEngine Pointer to the DCT engine.
             *
             * \return Returns the resulting DCT or inverse DCT.
             */
            Data* dctHelper(const MatRealDct dctEngine) const;

            /**
             * Method that transposes the supplied input data and then performs a DCT transform on each column.
             *
             * \param[in] input     The input data.
             *
             * \param[in] dctEngine Pointer to the DCT engine.
             *
             * \return Returns the resulting transposed and transformed data.
             */
            static DenseData* dctHelper2(const DenseData* input, const MatRealDct dctEngine);

            /**
             * The "small" limit value.  A negative value indicates that the small limit needs to be calculated.
             */
            static long double currentSmallLimit;

            /**
             * The "big" limit value.  A negative value indicates that the big limit needs to be calculated.
             */
            static long double currentBigLimit;
    };
}

#endif
