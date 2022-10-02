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
* This header defines the \ref Model::MatrixInteger class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_MATRIX_INTEGER_H
#define MODEL_MATRIX_INTEGER_H

#include "model_common.h"
#include "model_api_types.h"
#include "model_intrinsic_types.h"
#include "model_range.h"
#include "model_tuple.h"
#include "model_matrix.h"

namespace M {
    class Variant;
}

namespace Model {
    class Variant;
    class MatrixBoolean;
    class MatrixInteger;
    class MatrixComplex;

    /**
     * Matrix class holding integer values.  This class uses copy-on-write semantics to reduce memory utilization as
     * well as lazy evaluation of expressions to reduce processing overhead.  Note that the class is intended primarily
     * for use as a storage class and provides greatly reduced functionality compared to \ref Model::MatrixComplex and
     * \ref Model::MatrixReal
     */
    class MODEL_PUBLIC_API MatrixInteger:public Matrix {
        friend class Variant;
        friend class M::Variant;
        friend class MatrixBoolean;
        friend class MatrixReal;
        friend class MatrixComplex;

        public:
            /**
             * The type contained in this matrix.
             */
            typedef Integer Scalar;

            MatrixInteger();

            /**
             * Constructor.  This constructor will assert if the number of rows or number of columns is 0 or negative.
             *
             * \param[in] numberRows    The desired number of matrix rows.  Entries will be initialized to 0.
             *
             * \param[in] numberColumns The desired number of matrix columns.  Entries will be initialized to 0.
             */
            MatrixInteger(Integer numberRows, Integer numberColumns);

            /**
             * Constructor.  This constructor will assert if the number of rows or number of columns is 0 or negative.
             *
             * \param[in] numberRows    The desired number of matrix rows.
             *
             * \param[in] numberColumns The desired number of matrix columns.
             *
             * \param[in] matrixData    Pointer to an array of value containing the matrix contents.  Data should be
             *                          in column major order and must have at least as many entries as there are
             *                          coefficients in the matrix.
             */
            MatrixInteger(Integer numberRows, Integer numberColumns, const Scalar* matrixData);

            /**
             * Constructor
             *
             * \param[in] other The instance to be copied to this instance.
             */
            MatrixInteger(const MatrixBoolean& other);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied to this instance.
             */
            MatrixInteger(const MatrixInteger& other);

            /**
             * Move constructor
             *
             * \param[in] other The instance to be moved to this instance.
             */
            MatrixInteger(MatrixInteger&& other);

            /**
             * Constructor
             *
             * \param[in] other The instance to copy to this instance.
             */
            MatrixInteger(const Variant& other);

            ~MatrixInteger() override;

            /**
             * Template method you can use to create a pre-initialized matrix.
             *
             * \param[in] numberRows    The number of rows required for the matrix.
             *
             * \param[in] numberColumns The number of columns required for the matrix.
             *
             * \param[in] coefficients  The coefficients stored in the matrix.  Values must be in column major order
             *                          and must have at least many entries as there are coefficients in the matrix.
             *
             * \return Returns a newly constructed matrix.
             */
            template<typename... Coefficients> static MODEL_PUBLIC_TEMPLATE_METHOD MatrixInteger build(
                    const Integer   numberRows,
                    const Integer   numberColumns,
                    Coefficients... coefficients
                ) {
                Scalar matrixData[] = { (coefficients)... };
                return MatrixInteger(numberRows, numberColumns, matrixData);
            }

            /**
             * Method you can use to determine the underlying coefficient type.
             *
             * \return Returns the value type of the underlying coefficient.
             */
            ValueType coefficientValueType() const override;

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
            bool toFile(const char* filename, DataFileFormat fileFormat) const;

            /**
             * Method you can use to load a new matrix from a file.  This method will throw an exception if an error
             * occurs and the associated exception is enabled.
             *
             * \param[in] filename The filename to save the file to.
             *
             * \return Returns a newly created matrix.  Returns an empty matrix on error.
             */
            static MatrixInteger fromFile(const char* filename);

            /**
             * Method you can use to determine number of matrix rows.
             *
             * \return Returns the current number of rows.
             */
            Index numberRows() const override;

            /**
             * Method you can use to determine the number of matrix columns.
             *
             * \return Returns the current number of matrix columns.
             */
            Index numberColumns() const override;

            /**
             * Method you can use to determine the current total number of matrix coefficients.
             *
             * \return Returns the size of the matrix, in coefficients.
             */
            Index numberCoefficients() const override;

            /**
             * Method you can use to obtain an identity matrix of a specified size.
             *
             * \param[in] numberRows    The number of rows in the matrix.
             *
             * \param[in] numberColumns The number of columns in the matrix.
             */
            static MatrixInteger identity(Index numberRows, Index numberColumns);

            /**
             * Method you can use to obtain an identity matrix of a specified size.  This version will always create a
             * square matrix.
             *
             * \param[in] numberRowColumns The number of rows in the matrix.
             */
            static MatrixInteger identity(Index numberRowColumns);

            /**
             * Method you can use to obtain a zero matrix of a specified size.
             *
             * \param[in] numberRows    The number of rows in the matrix.
             *
             * \param[in] numberColumns The number of columns in the matrix.
             */
            static inline MatrixInteger zero(Index numberRows, Index numberColumns) {
                return MatrixInteger(numberRows, numberColumns);
            }

            /**
             * Method you can use to obtain a zero matrix of a specified size.  This version will always create a
             * square matrix.
             *
             * \param[in] numberRowColumns The number of rows in the matrix.
             */
            static inline MatrixInteger zero(Index numberRowColumns) {
                return MatrixInteger(numberRowColumns, numberRowColumns);
            }

            /**
             * Method you can use to obtain a one's matrix of a specified size.
             *
             * \param[in] numberRows    The number of rows in the matrix.
             *
             * \param[in] numberColumns The number of columns in the matrix.
             */
            static MatrixInteger ones(Index numberRows, Index numberColumns);

            /**
             * Method you can use to obtain a one's matrix of a specified size.  This version will always create a
             * square matrix.
             *
             * \param[in] numberRowColumns The number of rows in the matrix.
             */
            static MatrixInteger ones(Index numberRowColumns);

            /**
             * Method you can use to obtain the diagnonal entries from this matrix.
             *
             * \return Returns a column matrix holding the diagnonal entries of the matrix.
             */
            MatrixInteger diagonalEntries() const;

            /**
             * Method you can use construct a diagnonal matrix from a row or column matrix.
             *
             * \return Returns a newly constructed square matrix from the supplied diagnonal entries.
             */
            MatrixInteger diagonal() const;

            /**
             * Method you can use to access the value at a specified row/column address.
             *
             * \param[in] row    The one based row address.
             *
             * \param[in] column The one based column address.
             *
             * \return Returns a variant value holding the contents at the specified location.  An empty variant is
             *         returned if the location is invalid.
             */
            Variant value(Index row, Index column) const override;

            /**
             * Method you can use to access the value at a specified one based index.
             *
             * \param[in] index The one based index of the coefficient to be accessed.
             *
             * \return Returns a variant value holding the contents at the specified location.  An empty variant is
             *         returned if the location is invalid.
             */
            Variant value(Index index) const override;

            /**
             * Method you can use to set a value by position.  If needed, the matrix will be resized to contain the
             * provided coefficient.
             *
             * \param[in] row    The one based row address.
             *
             * \param[in] column The one based column address.
             *
             * \param[in] value  The value to assign to the specified location.
             *
             * \return Returns true on success, returns false if the variant is not compatible with the matrix type or
             *         the row/column address is invalid.
             */
            bool setValue(Index row, Index column, const Variant& value) override;

            /**
             * Method you can use to set a value by position.  If needed, the matrix will be resized to contain the
             * provided coefficient.  Now that, for empty matrices and matrices with a single row/column, matricies
             * will be resized as a row matrix.
             *
             * \param[in] index The one base index of the coefficient to be accessed.
             *
             * \param[in] value The value to assign to the specified location.
             *
             * \return Returns true on success, returns false if the variant is not compatible with the matrix type or
             *         the index is invalid.
             */
            bool setValue(Index index, const Variant& value) override;

            /**
             * Method you can use to resize the matrix.  Newly inserted values will be initialized to the default value
             * for the type and existing values will be preserved.
             *
             * Note that this is a potentially time-consuming operation.
             *
             * \param[in] newNumberRows    The new number of matrix rows.
             *
             * \param[in] newNumberColumns The new number of matrix columns.
             */
            void resize(Index newNumberRows, Index newNumberColumns) override;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row range or column is invalid.
             *
             * \param[in] row    The one based row number.
             *
             * \param[in] column The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            Scalar at(Index row, Index column) const;

            /**
             * Method you can use to obtain a value, by index.  This method will throw an exception if the specified
             * index is invalid.
             *
             * \param[in] index The one based index to the method.  On matrices, the index will go in row/column order.
             *
             * \return Returns the value at the specified location.
             */
            Scalar at(Index index) const;

            /**
             * Method you can use to obtain a submatrix of this matrix, by range values.  This method will throw an
             * exception if the specified row range or column is invalid.
             *
             * \param[in] rowRange The range to use to create the submatrix.
             *
             * \param[in] column   The column to use to create the submatrix.
             *
             * \return Returns the desired submatrix.
             */
            MatrixInteger at(const Range& rowRange, Index column) const;

            /**
             * Method you can use to obtain a submatrix of this matrix, by range values.  This method will throw an
             * exception if the specified row range or column is invalid.
             *
             * \param[in] row         The row to use to create the submatrix.
             *
             * \param[in] columnRange The column range to use to create the submatrix.
             *
             * \return Returns the desired submatrix.
             */
            MatrixInteger at(Index row, const Range& columnRange) const;

            /**
             * Method you can use to obtain a submatrix of this matrix, by range values.  This method will throw an
             * exception if the specified row range or column is invalid.
             *
             * \param[in] rowRange    The row range to use to create the submatrix.
             *
             * \param[in] columnRange The column range to use to create the submatrix.
             *
             * \return Returns the desired submatrix.
             */
            MatrixInteger at(const Range& rowRange, const Range& columnRange) const;

            /**
             * Method you can use to update a coefficient at a specified row and column.  This method will throw an
             * exception if the specified row or column is invalid and will resize the matrix, if needed, to include
             * the specified coefficient.
             *
             * \param[in] row      The one based row number.
             *
             * \param[in] column   The one based column number.
             *
             * \param[in] newValue The new value at the specified location.
             */
            void update(Index row, Index column, Scalar newValue);

            /**
             * Method you can use to update a coefficient at a specified index.  This method will throw an exception
             * if the specified row or column is invalid and will resize the matrix, if needed, to include the
             * specified coefficient.
             *
             * \param[in] index    The one based index into the matrix.
             *
             * \param[in] newValue The new value at the specified location.
             */
            void update(Index index, Scalar newValue);

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined left to right.
             *
             * This method exists to simplify templatization of this operation.
             *
             * \return Returns a copy of this matrix.
             */
            MatrixInteger combineLeftToRight() const;

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined left to right.
             *
             * \param[in] other The matrix to combine to the right of this matrix.
             *
             * \return Returns a new matrix representing the combination of the two matrices.
             */
            MatrixInteger combineLeftToRight(const MatrixBoolean& other) const;

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined left to right.
             *
             * \param[in] other The matrix to combine to the right of this matrix.
             *
             * \return Returns a new matrix representing the combination of the two matrices.
             */
            MatrixInteger combineLeftToRight(const MatrixInteger& other) const;

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined top to bottom.
             *
             * This method exists to simplify templatization of this operation.
             *
             * \return Returns a copy of this matrix.
             */
            MatrixInteger combineTopToBottom() const;

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined top to bottom.
             *
             * \param[in] other The matrix to combine below this matrix.
             *
             * \return Returns a new matrix representing the combination of the two matrices.
             */
            MatrixInteger combineTopToBottom(const MatrixBoolean& other) const;

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined top to bottom.
             *
             * \param[in] other The matrix to combine below this matrix.
             *
             * \return Returns a new matrix representing the combination of the two matrices.
             */
            MatrixInteger combineTopToBottom(const MatrixInteger& other) const;

            /**
             * Method you can use to reverse the columns in this matrix.
             *
             * \return Returns a column-reversed version of this matrix.
             */
            MatrixInteger columnReverse() const;

            /**
             * Method you can use to reverse the rows in this matrix.
             *
             * \return Returns a row-reversed version of this matrix.
             */
            MatrixInteger rowReverse() const;

            /**
             * Method you can use to determine if this matrix is square.  That is, whether the matrix has the same
             * number of rows as columns.
             *
             * \return Returns true if this matrix is square.  Returns false if this matrix is not square.
             */
            bool isSquare() const;

            /**
             * Method that returns the transpose of this matrix.
             *
             * \return Returns the transpose of this matrix.
             */
            MatrixInteger transpose() const;

            /**
             * Method that returns the complex conjugate of this matrix.
             *
             * \return Returns the complex conjugate of this value.
             */
            MatrixInteger conj() const;

            /**
             * Method that returns the adjoint or conjugate transpose of the matrix.
             *
             * \return Returns the adjoint of the matrix.
             */
            MatrixInteger adjoint() const;

            /**
             * Method that returns the Hadamard or Schur product of this matrix and another matrix. This method will
             * throw an exception if the matrices do not have identical dimensions.
             *
             * \param[in] other The matrix to multiply against this matrix.
             *
             * \return Returns the Hadamard product of the two matrices.
             */
            MatrixInteger hadamard(const MatrixInteger& other) const;

            /**
             * Method that returns the Kronecker product of this matrix and another matrix.
             *
             * \param[in] other The matrix to multiply against this matrix.
             *
             * \return Returns the Kronecker of the two matrices.
             */
            MatrixInteger kronecker(const MatrixInteger& other) const;

            /**
             * Method you can use to determine the underlying memory layout for the matrix.
             *
             * \return Returns a value indicating the underlying memory layout for the matrix.
             */
            MatrixType matrixType() const override;

            /**
             * Method you can use to obtain a pointer to the raw data.
             *
             * \return Returns a pointer to the internal raw data.
             */
            const Scalar* data() const;

            /**
             * Assignment operator
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            MatrixInteger& operator=(const MatrixBoolean& other);

            /**
             * Assignment operator
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            MatrixInteger& operator=(const MatrixInteger& other);

            /**
             * Assignment operator
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            MatrixInteger& operator=(const Variant& other);

            /**
             * Addition assignment operator
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a reference to this instance.
             */
            MatrixInteger& operator+=(const MatrixBoolean& v);

            /**
             * Addition assignment operator
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a reference to this instance.
             */
            MatrixInteger& operator+=(const MatrixInteger& v);

            /**
             * Subtraction assignment operator
             *
             * \param[in] v The value to subtract from this value.
             *
             * \return Returns a reference to this instance.
             */
            MatrixInteger& operator-=(const MatrixBoolean& v);

            /**
             * Subtraction assignment operator
             *
             * \param[in] v The value to subtract from this value.
             *
             * \return Returns a reference to this instance.
             */
            MatrixInteger& operator-=(const MatrixInteger& v);

            /**
             * Multiplication assignment operator
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a reference to this instance.
             */
            MatrixInteger& operator*=(const MatrixBoolean& v);

            /**
             * Multiplication assignment operator
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a reference to this instance.
             */
            MatrixInteger& operator*=(const MatrixInteger& v);

            /**
             * Multiplication assignment operator
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a reference to this instance.
             */
            MatrixInteger& operator*=(Boolean v);

            /**
             * Multiplication assignment operator
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a reference to this instance.
             */
            MatrixInteger& operator*=(Integer v);

            /**
             * Multiplication assignment operator
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a reference to this instance.
             */
            inline MatrixInteger& operator*=(signed char v) {
                return operator*=(Integer(v));
            }

            /**
             * Multiplication assignment operator
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a reference to this instance.
             */
            inline MatrixInteger& operator*=(unsigned char v) {
                return operator*=(Integer(v));
            }

            /**
             * Multiplication assignment operator
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a reference to this instance.
             */
            inline MatrixInteger& operator*=(signed short v) {
                return operator*=(Integer(v));
            }

            /**
             * Multiplication assignment operator
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a reference to this instance.
             */
            inline MatrixInteger& operator*=(unsigned short v) {
                return operator*=(Integer(v));
            }

            /**
             * Multiplication assignment operator
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a reference to this instance.
             */
            inline MatrixInteger& operator*=(signed v) {
                return operator*=(Integer(v));
            }

            /**
             * Multiplication assignment operator
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a reference to this instance.
             */
            inline MatrixInteger& operator*=(unsigned v) {
                return operator*=(Integer(v));
            }

            /**
             * Multiplication assignment operator
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a reference to this instance.
             */
            inline MatrixInteger& operator*=(signed long v) {
                return operator*=(Integer(v));
            }

            /**
             * Multiplication assignment operator
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a reference to this instance.
             */
            inline MatrixInteger& operator*=(unsigned long v) {
                return operator*=(Integer(v));
            }

            /**
             * Addition operator.
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a matrix representing the sum.
             */
            MatrixInteger operator+(const MatrixBoolean& v) const;

            /**
             * Addition operator.
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a matrix representing the sum.
             */
            MatrixInteger operator+(const MatrixInteger& v) const;

            /**
             * Subtraction operator.
             *
             * \param[in] v The value to subtract from this value.
             *
             * \return Returns a matrix representing the difference.
             */
            MatrixInteger operator-(const MatrixBoolean& v) const;

            /**
             * Subtraction operator.
             *
             * \param[in] v The value to subtract from this value.
             *
             * \return Returns a matrix representing the difference.
             */
            MatrixInteger operator-(const MatrixInteger& v) const;

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a matrix representing the product.
             */
            MatrixInteger operator*(const MatrixBoolean& v) const;

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a matrix representing the product.
             */
            MatrixInteger operator*(const MatrixInteger& v) const;

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a matrix representing the product.
             */
            MatrixInteger operator*(Boolean v) const;

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a matrix representing the product.
             */
            MatrixInteger operator*(Integer v) const;

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a matrix representing the product.
             */
            inline MatrixInteger operator*(signed char v) const {
                return operator*(Integer(v));
            }

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a matrix representing the product.
             */
            inline MatrixInteger operator*(unsigned char v) const {
                return operator*(Integer(v));
            }

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a matrix representing the product.
             */
            inline MatrixInteger operator*(signed short v) const {
                return operator*(Integer(v));
            }

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a matrix representing the product.
             */
            inline MatrixInteger operator*(unsigned short v) const {
                return operator*(Integer(v));
            }

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a matrix representing the product.
             */
            inline MatrixInteger operator*(signed v) const {
                return operator*(Integer(v));
            }

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a matrix representing the product.
             */
            inline MatrixInteger operator*(unsigned v) const {
                return operator*(Integer(v));
            }

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a matrix representing the product.
             */
            inline MatrixInteger operator*(signed long v) const {
                return operator*(Integer(v));
            }

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a matrix representing the product.
             */
            inline MatrixInteger operator*(unsigned long v) const {
                return operator*(Integer(v));
            }

            /**
             * Unary plus operator.
             *
             * \return Returns a reference to this object.
             */
            inline const MatrixInteger& operator+() const {
                return *this;
            }

            /**
             * Unary minus operator.
             *
             * \return Returns this matrix multiplied by -1.
             */
            MatrixInteger operator-() const;

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are equal.  Returns false if the values are not equal.
             */
            bool operator==(const MatrixBoolean& other) const;

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are equal.  Returns false if the values are not equal.
             */
            bool operator==(const MatrixInteger& other) const;

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are not equal.  Returns false if the values are equal.
             */
            bool operator!=(const MatrixBoolean& other) const;

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are not equal.  Returns false if the values are equal.
             */
            bool operator!=(const MatrixInteger& other) const;

            /**
             * Array subscript operator.
             *
             * \param[in] row    The one based row number.
             *
             * \param[in] column The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            inline Scalar operator()(Index row, Index column) const {
                return at(row, column);
            }

            /**
             * Array subscript operator.
             *
             * \param[in] index The one based index to the method.  On matrices, the index will go in reading order.
             *
             * \return Returns the value at the specified location.
             */
            inline Scalar operator()(Index index) const {
                return at(index);
            }

            /**
             * Array subscript operator.
             *
             * \param[in] rowRange The range to use to create the submatrix.
             *
             * \param[in] column   The column to use to create the submatrix.
             *
             * \return Returns the desired submatrix.
             */
            inline MatrixInteger operator()(const Range& rowRange, Index column) const {
                return at(rowRange, column);
            }

            /**
             * Array subscript operator.
             *
             * \param[in] row         The row to use to create the submatrix.
             *
             * \param[in] columnRange The column range to use to create the submatrix.
             *
             * \return Returns the desired submatrix.
             */
            inline MatrixInteger operator()(Index row, const Range& columnRange) const {
                return at(row, columnRange);
            }

            /**
             * Array subscript operator.
             *
             * \param[in] rowRange    The row range to use to create the submatrix.
             *
             * \param[in] columnRange The column range to use to create the submatrix.
             *
             * \return Returns the desired submatrix.
             */
            inline MatrixInteger operator()(const Range& rowRange, const Range& columnRange) const {
                return at(rowRange, columnRange);
            }

        private:
            class Private;

            /**
             * Private constructor
             *
             * \param[in] newImplementation Pointer to the new implementation instance.
             */
            inline MatrixInteger(Private* newImplementation):impl(newImplementation) {}

            Private* impl;
    };

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns an integer matrix value representing the product.
     */
    MODEL_PUBLIC_API MatrixInteger operator*(Boolean a, const MatrixInteger& b);

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns an integer matrix value representing the product.
     */
    MODEL_PUBLIC_API MatrixInteger operator*(Integer a, const MatrixInteger& b);

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns an integer matrix value representing the product.
     */
    MODEL_PUBLIC_API inline MatrixInteger operator*(signed char a, const MatrixInteger& b) {
        return b * Integer(a);
    }

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns an integer matrix value representing the product.
     */
    MODEL_PUBLIC_API inline MatrixInteger operator*(unsigned char a, const MatrixInteger& b) {
        return b * Integer(a);
    }

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns an integer matrix value representing the product.
     */
    MODEL_PUBLIC_API inline MatrixInteger operator*(signed short a, const MatrixInteger& b) {
        return b * Integer(a);
    }

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns an integer matrix value representing the product.
     */
    MODEL_PUBLIC_API inline MatrixInteger operator*(unsigned short a, const MatrixInteger& b) {
        return b * Integer(a);
    }

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns an integer matrix value representing the product.
     */
    MODEL_PUBLIC_API inline MatrixInteger operator*(signed a, const MatrixInteger& b) {
        return b * Integer(a);
    }

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns an integer matrix value representing the product.
     */
    MODEL_PUBLIC_API inline MatrixInteger operator*(unsigned a, const MatrixInteger& b) {
        return b * Integer(a);
    }

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns an integer matrix value representing the product.
     */
    MODEL_PUBLIC_API inline MatrixInteger operator*(signed long a, const MatrixInteger& b) {
        return b * Integer(a);
    }

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns an integer matrix value representing the product.
     */
    MODEL_PUBLIC_API inline MatrixInteger operator*(unsigned long a, const MatrixInteger& b) {
        return b * Integer(a);
    }

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns an integer matrix value representing the product.
     */
    MODEL_PUBLIC_API MatrixInteger operator*(Integer a, const MatrixBoolean& b);

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns an integer matrix value representing the product.
     */
    MODEL_PUBLIC_API inline MatrixInteger operator*(signed char a, const MatrixBoolean& b) {
        return MatrixInteger(b) * Integer(a);
    }

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns an integer matrix value representing the product.
     */
    MODEL_PUBLIC_API inline MatrixInteger operator*(unsigned char a, const MatrixBoolean& b) {
        return MatrixInteger(b) * Integer(a);
    }

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns an integer matrix value representing the product.
     */
    MODEL_PUBLIC_API inline MatrixInteger operator*(signed short a, const MatrixBoolean& b) {
        return MatrixInteger(b) * Integer(a);
    }

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns an integer matrix value representing the product.
     */
    MODEL_PUBLIC_API inline MatrixInteger operator*(unsigned short a, const MatrixBoolean& b) {
        return MatrixInteger(b) * Integer(a);
    }

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns an integer matrix value representing the product.
     */
    MODEL_PUBLIC_API inline MatrixInteger operator*(signed a, const MatrixBoolean& b) {
        return MatrixInteger(b) * Integer(a);
    }

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns an integer matrix value representing the product.
     */
    MODEL_PUBLIC_API inline MatrixInteger operator*(unsigned a, const MatrixBoolean& b) {
        return MatrixInteger(b) * Integer(a);
    }

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns an integer matrix value representing the product.
     */
    MODEL_PUBLIC_API inline MatrixInteger operator*(signed long a, const MatrixBoolean& b) {
        return MatrixInteger(b) * Integer(a);
    }

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns an integer matrix value representing the product.
     */
    MODEL_PUBLIC_API inline MatrixInteger operator*(unsigned long a, const MatrixBoolean& b) {
        return MatrixInteger(b) * Integer(a);
    }

    /**
     * Matrix addition operator.
     *
     * \param[in] a The augend.
     *
     * \param[in] b The addend.
     *
     * \return Returns a complex matrix value representing the sum.
     */
    MODEL_PUBLIC_API MatrixInteger operator+(const MatrixBoolean& a, const MatrixInteger& b);

    /**
     * Matrix subtraction operator.
     *
     * \param[in] a The minuend.
     *
     * \param[in] b The subtrahend.
     *
     * \return Returns a complex matrix value representing the difference.
     */
    MODEL_PUBLIC_API MatrixInteger operator-(const MatrixBoolean& a, const MatrixInteger& b);
}

#endif
