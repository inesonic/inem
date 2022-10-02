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
* This header defines the private implementation data store for complex dense matrices.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_MATRIX_COMPLEX_DENSE_DATA_H
#define MODEL_MATRIX_COMPLEX_DENSE_DATA_H

#include <mat_api.h>

#include "model_api_types.h"
#include "model_intrinsic_types.h"
#include "model_complex.h"
#include "m_matrix_sparse_private.h"
#include "m_matrix_dense_private.h"
#include "m_matrix_real.h"
#include "m_matrix_real_dense_data.h"
#include "m_matrix_integer.h"
#include "m_matrix_integer_dense_data.h"
#include "m_matrix_boolean.h"
#include "m_matrix_boolean_dense_data.h"
#include "m_matrix_complex_data.h"
#include "m_matrix_complex.h"

namespace M {
    /**
     * Private implementation class for \ref M::MatrixComplex.  This class provides the data store and implementation
     * for complex matricies using a dense data format.
     */
    class MatrixComplex::DenseData:public MatrixComplex::Data, public MatrixDensePrivate<Complex> {
        friend class MatrixDensePrivate<Scalar>;

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
             * Method that translates real matrix data to this type.
             *
             * \param[in] other The matrix data to be translated.
             *
             * \return Returns the translated matrix.
             */
            static DenseData* create(const MatrixReal::DenseData& other);

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
            MatrixComplex::Data* clone() const override {
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
                return ValueType::COMPLEX;
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
             * Method you can use to load a new matrix from a file. This method will throw an exception if an error
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
            const Scalar& at(unsigned long rowIndex, unsigned long columnIndex) const override {
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
            MatrixComplex::Data* combineLeftToRight(const MatrixDensePrivate<Scalar>& other) const override;

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined left to right.
             *
             * \param[in] other The matrix to combine to the right of this matrix.
             *
             * \return Returns a new matrix representing the combination of the two matrices.
             */
            MatrixComplex::Data* combineLeftToRight(const MatrixSparsePrivate<Scalar>& other) const override;

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined top to bottom.
             *
             * \param[in] other The matrix to combine below this matrix.
             *
             * \return Returns a new matrix representing the combination of the two matrices.
             */
            MatrixComplex::Data* combineTopToBottom(const MatrixDensePrivate<Scalar>& other) const override;

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined top to bottom.
             *
             * \param[in] other The matrix to combine below this matrix.
             *
             * \return Returns a new matrix representing the combination of the two matrices.
             */
            MatrixComplex::Data* combineTopToBottom(const MatrixSparsePrivate<Scalar>& other) const override;

            /**
             * Method you can use to reverse the columns in this matrix.
             *
             * \return Returns a column-reversed version of this matrix.
             */
            MatrixComplex::Data* columnReverse() const override;

            /**
             * Method you can use to reverse the rows in this matrix.
             *
             * \return Returns a row-reversed version of this matrix.
             */
            MatrixComplex::Data* rowReverse() const override;

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
            MatrixComplex::Data* resizeTo(
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
             * Method that returns the determinant of this matrix.
             *
             * \return Returns the complex conjugate of this value.
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
             * Method that performs a Schur decomposition of this matrix.
             *
             * \param[out] qData The Q unitary matrix.  You are expected to take ownership of this matrix.
             *
             * \param[out] uData The Schur decomposition of this matrix.  You are expected to take ownership of this
             *                   matrix.
             *
             * \param[out] wData A column matrix containing the computed eigenvalues.  Values correspond to the
             *                   diagnonal of the returned Schur matrix.  You are expected to take ownership of this
             *                   matrix.
             *
             * \return Returns true on success, returns false if one or more eigenvalues could not be determined.
             */
            bool schur(Data*& qData, Data*& uData, Data*& wData) const override;

            /**
             * Method that calculates a complete set of eigenvectors and eigenvalues for this matrix.  The method also
             * calculates and returns the Schur decomposition of this matrix.
             *
             * \param[in]  rightEigenvectors If true, this routine will compute the right eigenvectors of the matrix.
             *                               If false, this routine will compute the left eigenvectors of the matrix.
             *
             * \param[out] eigenvalueMatrix  Pointer to the created row matrix holding the computed eigenvalues.  You
             *                               are expected to take ownership of the matrix.
             *
             * \param[out] qMatrix           Pointer to the created Q unitary matrix.  You are expected to take
             *                               ownership of the matrix.
             *
             * \param[out] uMatrix           Pointer to the Schur decomposition of this matrix.  You are expected to
             *                               take ownership of the matrix.
             *
             * \param[out] eigenvectors      Pointer to an array to contain the eigenvectors associated with each
             *                               eigenvalue.  You are expected to take ownership of both the array and the
             *                               pointers to the matrices it contains.  The list will be terminated with a
             *                               null pointer.
             *
             * \return Returns true on success, return false on error.
             */
            bool eigenvectors(
                bool    rightEigenvectors,
                Data*&  eigenvalueMatrix,
                Data*&  qMatrix,
                Data*&  uMatrix,
                Data**& eigenvectors
            ) const override;

            /**
             * Method that calculates the forward discrete Fourier transform (DFT) of this matrix.  A matrix with a
             * single dimension will be processed as a one-dimensional DFT.  A matrix with multiple rows and columns
             * will be processed as a two dimensional DFT.
             *
             * \return Returns a matrix representing the complex discrete Fourier transform of the matrix.
             */
            Data* dft() const override;

            /**
             * Method that calculates the backwards or inverse discrete Fourier transform (DFT) of this matrix.  A
             * matrix with a single dimension will be processed as a one dimensional backward DFT.  A matrix with
             * multiple rows and columns will be processed as a two dimensional backward DFT.
             *
             * \return Returns a matrix representing the complex inverse discrete Fourier transform of the matrix.
             */
            Data* idft() const override;

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
            Data* solve(const MatrixComplex::DenseData* y) const override;

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
            Data* solve(const MatrixComplex::SparseData* y) const override;

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
            Data* leastSquares(const MatrixComplex::DenseData* b) const override;

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
            Data* leastSquares(const MatrixComplex::SparseData* b) const override;

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
             * \param[in] otherTransform   The transform to be applied to the other matrix.
             *
             * \param[in] scalarMultiplier A scalar multiplier to be applied.
             *
             * \return Returns the Kronecker product of the two matrices.
             */
            Data* kronecker(
                const MatrixDensePrivate<Scalar>& other,
                LazyTransform                      otherTransform,
                const Scalar&                      scalarMultiplier
            ) const override;

            /**
             * Method that calculates the Kronecker product of this matrix and another matrix.
             *
             * \param[in] other            The matrix to multiply against this matrix.
             *
             * \param[in] otherTransform   The transform to be applied to the other matrix.
             *
             * \param[in] scalarMultiplier A scalar multiplier to be applied.
             *
             * \return Returns the Kronecker product of the two matrices.
             */
            Data* kronecker(
                const MatrixSparsePrivate<Scalar>& other,
                LazyTransform                      otherTransform,
                const Scalar&                      scalarMultiplier
            ) const override;

            /**
             * Method you can call to add this matrix to another matrix.
             *
             * \param[in] addend          The instance to be added to this instance.
             *
             * \param[in] augendTransform Indicates the transformation to be performed on the augend (left value)
             *                            prior to performing the addition.  The augend (this data instance) will not
             *                            be modified.  Note that all lazy transformations are supported.
             *
             * \param[in] addendTransform Indicates the transformation to be performed on the added (right value) prior
             *                            to performing the addition.  The addend will not be modified.  Note that all
             *                            lazy transforms are supported.
             *
             * \param[in] augendScalar    A scalar value to be multiplied to the augend prior to the addition.
             *
             * \param[in] addendScalar    A scalar value to be multiplied to the addend prior to the addition.
             *
             * \return Returns a new matrix representing the sum of the two matrices.
             */
            Data* add(
                const MatrixDensePrivate<Scalar>& addend,
                LazyTransform                      augendTransform,
                LazyTransform                      addendTransform,
                const Complex&                     augendScalar,
                const Complex&                     addendScalar
            ) const override;

            /**
             * Method you can call to add this matrix to another matrix.
             *
             * \param[in] addend          The instance to be added to this instance.
             *
             * \param[in] augendTransform Indicates the transformation to be performed on the augend (left value)
             *                            prior to performing the addition.  The augend (this data instance) will not
             *                            be modified.  Note that all lazy transformations are supported.
             *
             * \param[in] addendTransform Indicates the transformation to be performed on the added (right value) prior
             *                            to performing the addition.  The addend will not be modified.  Note that all
             *                            lazy transforms are supported.
             *
             * \param[in] augendScalar    A scalar value to be multiplied to the augend prior to the addition.
             *
             * \param[in] addendScalar    A scalar value to be multiplied to the addend prior to the addition.
             *
             * \return Returns a new matrix representing the sum of the two matrices.
             */
            Data* add(
                const MatrixSparsePrivate<Scalar>& addend,
                LazyTransform                      augendTransform,
                LazyTransform                      addendTransform,
                const Complex&                     augendScalar,
                const Complex&                     addendScalar
            ) const override;

            /**
             * Method you can call to multiply this matrix to another matrix.
             *
             * \param[in] multiplicand          The instance to be multiplied to this instance.
             *
             * \param[in] multiplierTransform   Indicates the transformation to be formed on the multiplier during the
             *                                  multiplication. The multiplier will not be modified during the
             *                                  operation.  Note \ref M::MatrixComplex::Data::LazyTransform::CONJUGATE
             *                                  is not supported by this method and will trigger an assert.
             *
             * \param[in] multiplicandTransform Indicates the transformation to be formed on the multiplicand during
             *                                  the multiplication. The multiplicand will not be modified during the
             *                                  operation.  Note \ref M::MatrixComplex::Data::LazyTransform::CONJUGATE
             *                                  is not supported by this method and will trigger an assert.
             *
             * \param[in] scalerMultiplier      A scaler multiplier to be applied to the multiplication operation.
             *
             * \return Returns a new matrix representing the product of the two matrices.
             */
            Data* multiply(
                const MatrixDensePrivate<Scalar>& multiplicand,
                LazyTransform                      multiplierTransform,
                LazyTransform                      multiplicandTransform,
                const Complex&                     scalerMultiplier
            ) const override;

            /**
             * Method you can call to multiply this matrix to another matrix.
             *
             * \param[in] multiplicand          The instance to be multiplied to this instance.
             *
             * \param[in] multiplierTransform   Indicates the transformation to be formed on the multiplier during the
             *                                  multiplication. The multiplier will not be modified during the
             *                                  operation.  Note \ref M::MatrixComplex::Data::LazyTransform::CONJUGATE
             *                                  is not supported by this method and will trigger an assert.
             *
             * \param[in] multiplicandTransform Indicates the transformation to be formed on the multiplicand during
             *                                  the multiplication. The multiplicand will not be modified during the
             *                                  operation.  Note \ref M::MatrixComplex::Data::LazyTransform::CONJUGATE
             *                                  is not supported by this method and will trigger an assert.
             *
             * \param[in] scalerMultiplier      A scaler multiplier to be applied to the multiplication operation.
             *
             * \return Returns a new matrix representing the product of the two matrices.
             */
            Data* multiply(
                const MatrixSparsePrivate<Scalar>& multiplicand,
                LazyTransform                      multiplierTransform,
                LazyTransform                      multiplicandTransform,
                const Complex&                     scalerMultiplier
            ) const override;

            /**
             * Method you can use to obtain the real portion of this matrix data.
             *
             * \return Returns the real portion of this matrix data.
             */
            MatrixReal::Data* real() const override;

            /**
             * Method you can use to obtain the imaginary portion of this matrix data.
             *
             * \return Returns the imaginary portion of this matrix data.
             */
            MatrixReal::Data* imag() const override;

            /**
             * Method that compares this matrix against another matrix.  This version supports complex types.
             *
             * \param[in] other The matrix to be compared against.
             *
             * \return Returns true if the matrices are equal.  Returns false if the matrices are not equal.
             */
            bool isEqualTo(const MatrixDensePrivate<Scalar>& other) const override;

            /**
             * Method that compares this matrix against another matrix.  This version supports complex types.
             *
             * \param[in] other The matrix to be compared against.
             *
             * \return Returns true if the matrices are equal.  Returns false if the matrices are not equal.
             */
            bool isEqualTo(const MatrixSparsePrivate<Scalar>& other) const override;

            /**
             * Method that applies a requested transform and scaling.
             *
             * \param[in] transform   The desired transform.
             *
             * \param[in] scaleFactor The desired scale factor.
             *
             * \return Returns a pointer to the newly transformed and scaled instance.
             */
            Data* applyTransformAndScaling(LazyTransform transform, const Scalar& scaleFactor) const override;

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
            int relativeOrder(const MatrixComplex::Data& other) const override;

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
             * Method that is used by the \ref M::MatrixComplex::DenseData::real and
             * \ref M::MatrixComplex::DenseData::imag methods to split this matrix.
             *
             * \param[in] offset The offset to the real or imaginary entry.  A value of 0 indicates a real entry.  A
             *                   value of 1 indicates the imaginary entry.
             *
             * \return Returns a pointer to the newly minted \ref M::MatrixReal::DenseData instance holding the
             *         requested data.
             */
            MatrixReal::DenseData* splitIntoParts(unsigned offset) const;

            /**
             * Method that converts an matrix pivot array into a row pivot matrix.
             *
             * \param[in] matPivotOperationsByRow Any array holding the pivot operations by row.
             *
             * \param[in] numberRows              The number of matrix rows.  The returned matrix will be a square
             *                                    matrix with this many rows and columns.
             *
             * \param[in] matPivotArraySize       The size of the pivot array, in entries.
             *
             * \return Returns a \ref M::MatrixComplex::DenseData instance holding the pivot matrix.
             */
            static DenseData* generatePivotMatrix(
                const MatInteger* matPivotOperationsByRow,
                unsigned long     numberRows,
                unsigned long     matPivotArraySize
            );

            /**
             * Helper method that calculates a Schur and Q unitary matrix as part of the Eigenvalue decomposition.
             *
             * \param[out] sData      A matrix created to hold the Schur matrix.  You are expected to take ownership of
             *                        this matrix.
             *
             * \param[out] qzData     A matrix created to hold the QZ unitary matrix terms.  You are expected to take
             *                        ownership of this matrix.
             *
             * \param[out] scaleTerms Scale terms used when balancing this input matrix.  You will need to supply the
             *                        scale terms to the LAPACKE_zgebak function.  You are expected to take ownership
             *                        of this dynamically allocated array.
             *
             * \param[out] wData      A column matrix that is created to contain the calculated eigenvalues.  You are
             *                        expected to take ownership of this matrix.
             *
             * \param[out] ilo        Location to hold the balancing low index.
             *
             * \param[out] ihi        Location to hold the balancing high index.
             *
             * \return Returns true on success, returns false on error.
             */
            bool calculateSchur(
                DenseData*& sData,
                DenseData*& qzData,
                double*&    scaleTerms,
                DenseData*& wData,
                MatInteger& ilo,
                MatInteger& ihi
            ) const;

            /**
             * Helper function used to calculate both DFT and inverse DFTs of this this matrix.
             *
             * \param[in] forward If true, we want to calculate a forward DFT.  If false, we want to calculate a
             *                    backward or inverse DFT.
             *
             * \return Returns a pointer to the newly created matrix data instance.
             */
            DenseData* calculateDft(bool forward) const;

            /**
             * Method that multiplies each row against a unique scale factor.
             *
             * \param[in] sourceMatrix      The source matrix to be used.
             *
             * \param[in] rowScaleFactors   An array holding the row scale factors as real/imaginary pairs.
             *
             * \return Returns a newly created, row-scaled matrix.
             */
            static DenseData* multiplyRows(const DenseData& sourceMatrix, const double* rowScaleFactors);

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
