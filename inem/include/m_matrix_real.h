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
* This header defines the the \ref M::MatrixReal class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_MATRIX_REAL_H
#define M_MATRIX_REAL_H

#include "m_common.h"
#include "m_api_types.h"
#include "m_type_conversion.h"
#include "m_intrinsics.h"
#include "m_intrinsic_types.h"
#include "m_matrix_iterator.h"
#include "model_matrix_real.h"
#include "model_matrix.h"

namespace M {
    class Set;
    class Tuple;
    class MatrixBoolean;
    class MatrixInteger;
    class MatrixComplex;
    class Variant;
    class PerThread;

    /**
     * Matrix class holding real values.  This class uses copy-on-write semantics to reduce memory utilization as well
     * as lazy evaluation of expressions to reduce processing overhead.
     */
    class M_PUBLIC_API MatrixReal:public Model::Matrix {
        friend class MatrixBoolean;
        friend class MatrixInteger;
        friend class MatrixComplex;

        public:
            /**
             * The type contained in this matrix.
             */
            typedef Real Scalar;

            /**
             * The iterator used to traverse this matrix.
             */
            typedef MatrixIterator<MatrixReal> Iterator;

            /**
             * Iterator type.
             */
            typedef MatrixIterator<MatrixReal> ConstIterator;

            /**
             * Type used for STL compatibility.
             */
            typedef Iterator iterator;

            /**
             * Type used for STL compatibility.
             */
            typedef ConstIterator const_iterator;

            /**
             * Type used for STL compatibility.
             */
            typedef Scalar value_type;

            /**
             * The default tolerance (relative) for various tests.
             */
            static constexpr Real defaultRelativeTolerance = Model::MatrixReal::defaultRelativeTolerance;

            /**
             * Constructor
             *
             * \param[in] newNumberRows    The desired number of rows.
             *
             * \param[in] newNumberColumns The desired number of columns.
             */
            MatrixReal(Integer newNumberRows = 0, Integer newNumberColumns = 0);

            /**
             * Constructor.
             *
             * \param[in] numberRows    The desired number of matrix rows.
             *
             * \param[in] numberColumns The desired number of matrix columns.
             *
             * \param[in] matrixData    Pointer to an array of value containing the matrix contents.  Data should be
             *                          in column major order.
             */
            MatrixReal(Integer numberRows, Integer numberColumns, const Scalar* matrixData);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            MatrixReal(const MatrixBoolean& other);

            /**
             * Constructor
             *
             * \param[in] other The instance to be copied.
             */
            MatrixReal(const MatrixInteger& other);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            MatrixReal(const MatrixReal& other);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            MatrixReal(const MatrixComplex& other);

            /**
             * Constructor
             *
             * \param[in] other The instance to copy to this instance.
             */
            MatrixReal(const Variant& other);

            ~MatrixReal() override;

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
            template<typename... Coefficients> M_PUBLIC_TEMPLATE_METHOD static MatrixReal build(
                    const Integer   numberRows,
                    const Integer   numberColumns,
                    Coefficients... coefficients
                ) {
                Scalar matrixData[] = { inlineToReal(coefficients)... };
                return MatrixReal(numberRows, numberColumns, matrixData);
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
            bool toFile(const char* filename, Model::DataFileFormat fileFormat) const;

            /**
             * Method you can use to load a new matrix from a file.  This method will throw an exception if an error
             * occurs and the associated exception is enabled.
             *
             * \param[in] filename The filename to save the file to.
             *
             * \return Returns a newly created matrix.  Returns an empty matrix on error.
             */
            static MatrixReal fromFile(const char* filename);

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
             * Alternate method you can use to determine the current total number of matrix coefficients.
             *
             * \return Returns the size of the matrix, in coefficients.
             */
            inline Index size() const {
                return numberCoefficients();
            }

            /**
             * Method you can use to obtain an iterator that points to the first element of the matrix.
             *
             * \return Returns an iterator that points to the first element of the matrix.
             */
            inline Iterator begin() const {
                return Iterator(this, 1, 1);
            }

            /**
             * Method you can use to obtain an iterator that points past the last element of the matrix.
             *
             * \return returns an iterator that points just past the last element of the matrix.
             */
            inline Iterator end() const {
                return Iterator(this, numberRows(), numberColumns() + 1);
            }

            /**
             * Method you can use to obtain an iterator that points to the first element of the matrix.
             *
             * \return Returns an iterator that points to the first element of the matrix.
             */
            inline Iterator constBegin() const {
                return begin();
            }

            /**
             * Method you can use to obtain an iterator that points past the last element of the matrix.
             *
             * \return returns an iterator that points just past the last element of the matrix.
             */
            inline Iterator constEnd() const {
                return end();
            }

            /**
             * Method you can use to obtain an iterator that points to the first element of the matrix.  This version
             * is provided for limited STL compatibility.
             *
             * \return Returns an iterator that points to the first element of the matrix.
             */
            inline Iterator cbegin() const {
                return begin();
            }

            /**
             * Method you can use to obtain an iterator that points past the last element of the matrix.  This version
             * is provided for limited STL compatibility.
             *
             * \return returns an iterator that points just past the last element of the matrix.
             */
            inline Iterator cend() const {
                return end();
            }

            /**
             * Method you can use to obtain an identity matrix of a specified size.
             *
             * \param[in] numberRows    The number of rows in the matrix.
             *
             * \param[in] numberColumns The number of columns in the matrix.
             */
            static MatrixReal identity(Index numberRows, Index numberColumns);

            /**
             * Method you can use to obtain an identity matrix of a specified size.  This version will always create
             * a square matrix.
             *
             * \param[in] numberRowColumns The number of rows in the matrix.
             */
            static MatrixReal identity(Index numberRowColumns);

            /**
             * Method you can use to obtain a zero matrix of a specified size.
             *
             * \param[in] numberRows    The number of rows in the matrix.
             *
             * \param[in] numberColumns The number of columns in the matrix.
             */
            static inline MatrixReal zero(Index numberRows, Index numberColumns) {
                return MatrixReal(numberRows, numberColumns);
            }

            /**
             * Method you can use to obtain a zero matrix of a specified size.  This version will always create a
             * square matrix.
             *
             * \param[in] numberRowColumns The number of rows in the matrix.
             */
            static inline MatrixReal zero(Index numberRowColumns) {
                return MatrixReal(numberRowColumns, numberRowColumns);
            }

            /**
             * Method you can use to obtain a one's matrix of a specified size.
             *
             * \param[in] numberRows    The number of rows in the matrix.
             *
             * \param[in] numberColumns The number of columns in the matrix.
             */
            static MatrixReal ones(Index numberRows, Index numberColumns);

            /**
             * Method you can use to obtain a one's matrix of a specified size.  This version will always create a
             * square matrix.
             *
             * \param[in] numberRowColumns The number of rows in the matrix.
             */
            static MatrixReal ones(Index numberRowColumns);

            /**
             * Method you can use to obtain the diagnonal entries from this matrix.
             *
             * \return Returns a column matrix holding the diagnonal entries of the matrix.
             */
            MatrixReal diagonalEntries() const;

            /**
             * Method you can use construct a diagnonal matrix from a row or column matrix.
             *
             * \return Returns a newly constructed square matrix from the supplied diagnonal entries.
             */
            MatrixReal diagonal() const;

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
            Model::Variant value(Index row, Index column) const override;

            /**
             * Method you can use to access the value at a specified one based index.
             *
             * \param[in] index The one based index of the coefficient to be accessed.
             *
             * \return Returns a variant value holding the contents at the specified location.  An empty variant is
             *         returned if the location is invalid.
             */
            Model::Variant value(Index index) const override;

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
            bool setValue(Index row, Index column, const Model::Variant& value) override;

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
            bool setValue(Index index, const Model::Variant& value) override;

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
             * row or column is invalid.
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
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row    The one based row number.
             *
             * \param[in] column The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            Scalar at(Real row, Index column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row    The one based row number.
             *
             * \param[in] column The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            Scalar at(const Complex& row, Index column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row    The one based row number.
             *
             * \param[in] column The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            Scalar at(Index row, Real column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row    The one based row number.
             *
             * \param[in] column The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            Scalar at(Index row, const Complex& column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row    The one based row number.
             *
             * \param[in] column The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            Scalar at(Real row, Real column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row    The one based row number.
             *
             * \param[in] column The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            Scalar at(const Complex& row, Real column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row    The one based row number.
             *
             * \param[in] column The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            Scalar at(Real row, const Complex& column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row    The one based row number.
             *
             * \param[in] column The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            Scalar at(const Complex& row, const Complex& column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] index The one based index to the method.  On matrices, the index will go in row/column order.
             *
             * \return Returns the value at the specified location.
             */
            Scalar at(Real index) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] index The one based index to the method.  On matrices, the index will go in row/column order.
             *
             * \return Returns the value at the specified location.
             */
            Scalar at(const Complex& index) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowSet The one based row number set.
             *
             * \param[in] column The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Set& rowSet, Index column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowSet The one based row number set.
             *
             * \param[in] column The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Set& rowSet, Real column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowSet The one based row number set.
             *
             * \param[in] column The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Set& rowSet, const Complex& column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowSet      The one based row number set.
             *
             * \param[in] columnRange The one based column number range.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Set& rowSet, const Range& columnRange) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowSet    The one based row number.
             *
             * \param[in] columnSet The one based column number set.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Set& rowSet, const Set& columnSet) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowSet      The one based row number set.
             *
             * \param[in] columnTuple The one based column number tuple.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Set& rowSet, const Tuple& columnTuple) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowSet       The one based row number set.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Set& rowSet, const MatrixInteger& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowSet       The one based row number set.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Set& rowSet, const MatrixReal& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowSet          The one based row number set.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Set& rowSet, const MatrixComplex& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row       The one based row number.
             *
             * \param[in] columnSet The one based column number set.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(Index row, const Set& columnSet) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row       The one based row number.
             *
             * \param[in] columnSet The one based column number set.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(Real row, const Set& columnSet) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row       The one based row number.
             *
             * \param[in] columnSet The one based column number set.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Complex& row, const Set& columnSet) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowRange  The one based row number range.
             *
             * \param[in] columnSet The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Range& rowRange, const Set& columnSet) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowTuple  The one based row number tuple.
             *
             * \param[in] columnSet The one based column number set.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Tuple& rowTuple, const Set& columnSet) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix The one based row number matrix.
             *
             * \param[in] columnSet The one based column number set.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixInteger& rowMatrix, const Set& columnSet) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix The one based row number matrix.
             *
             * \param[in] columnSet The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixReal& rowMatrix, const Set& columnSet) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix The one based row number matrix.
             *
             * \param[in] columnSet The one based column number set.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixComplex& rowMatrix, const Set& columnSet) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] indexSet  The one based index number set.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Set& indexSet) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowTuple The one based row number tuple.
             *
             * \param[in] column   The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Tuple& rowTuple, Index column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowTuple The one based row number tuple.
             *
             * \param[in] column   The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Tuple& rowTuple, Real column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowTuple The one based row number tuple.
             *
             * \param[in] column   The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Tuple& rowTuple, const Complex& column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowTuple    The one based row number tuple.
             *
             * \param[in] columnRange The one based column number range.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Tuple& rowTuple, const Range& columnRange) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowTuple    The one based row number tuple.
             *
             * \param[in] columnTuple The one based column number tuple.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Tuple& rowTuple, const Tuple& columnTuple) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowTuple     The one based row number tuple.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Tuple& rowTuple, const MatrixInteger& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowTuple     The one based row number tuple.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Tuple& rowTuple, const MatrixReal& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowTuple The one based row number tuple.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Tuple& rowTuple, const MatrixComplex& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row         The one based row number.
             *
             * \param[in] columnTuple The one based column number tuple.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(Index row, const Tuple& columnTuple) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row    The one based row number.
             *
             * \param[in] columnTuple The one based column number tuple.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(Real row, const Tuple& columnTuple) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row    The one based row number.
             *
             * \param[in] columnTuple The one based column number tuple.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Complex& row, const Tuple& columnTuple) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowRange The one based row number range.
             *
             * \param[in] columnTuple The one based column number tuple.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Range& rowRange, const Tuple& columnTuple) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix The one based row number matrix.
             *
             * \param[in] columnTuple The one based column number tuple.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixInteger& rowMatrix, const Tuple& columnTuple) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix The one based row number matrix.
             *
             * \param[in] columnTuple The one based column number tuple.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixReal& rowMatrix, const Tuple& columnTuple) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix The one based row number matrix.
             *
             * \param[in] columnTuple The one based column number tuple.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixComplex& rowMatrix, const Tuple& columnTuple) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] indexTuple  The one based index number tuple.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Tuple& indexTuple) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix The one based row number matrix.
             *
             * \param[in] column    The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixInteger& rowMatrix, Index column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix The one based row number matrix.
             *
             * \param[in] column    The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixInteger& rowMatrix, Real column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix The one based row number matrix.
             *
             * \param[in] column    The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixInteger& rowMatrix, const Complex& column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix   The one based row number matrix.
             *
             * \param[in] columnRange The one based column number range.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixInteger& rowMatrix, const Range& columnRange) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix    The one based row number matrix.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixInteger& rowMatrix, const MatrixInteger& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix    The one based row number matrix.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixInteger& rowMatrix, const MatrixReal& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix    The one based row number matrix.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixInteger& rowMatrix, const MatrixComplex& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row          The one based row number.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(Index row, const MatrixInteger& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row          The one based row number.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(Real row, const MatrixInteger& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row         The one based row number.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Complex& row, const MatrixInteger& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowRange     The one based row number range.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Range& rowRange, const MatrixInteger& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix    The one based row number matrix.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixReal& rowMatrix, const MatrixInteger& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix    The one based row number matrix.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixComplex& rowMatrix, const MatrixInteger& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] indexMatrix The one based index number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixInteger& indexMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix The one based row number matrix.
             *
             * \param[in] column    The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixReal& rowMatrix, Index column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix The one based row number matrix.
             *
             * \param[in] column    The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixReal& rowMatrix, Real column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix The one based row number matrix.
             *
             * \param[in] column    The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixReal& rowMatrix, const Complex& column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix   The one based row number matrix.
             *
             * \param[in] columnRange The one based column number range.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixReal& rowMatrix, const Range& columnRange) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix    The one based row number matrix.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixReal& rowMatrix, const MatrixReal& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix    The one based row number matrix.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixReal& rowMatrix, const MatrixComplex& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row          The one based row number.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(Index row, const MatrixReal& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row          The one based row number.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(Real row, const MatrixReal& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row         The one based row number.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Complex& row, const MatrixReal& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowRange     The one based row number range.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Range& rowRange, const MatrixReal& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix    The one based row number matrix.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixComplex& rowMatrix, const MatrixReal& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] indexMatrix The one based index number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixReal& indexMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix The one based row number matrix.
             *
             * \param[in] column    The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixComplex& rowMatrix, Index column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix The one based row number matrix.
             *
             * \param[in] column    The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixComplex& rowMatrix, Real column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix The one based row number matrix.
             *
             * \param[in] column    The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixComplex& rowMatrix, const Complex& column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix   The one based row number matrix.
             *
             * \param[in] columnRange The one based column number range.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixComplex& rowMatrix, const Range& columnRange) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix    The one based row number matrix.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixComplex& rowMatrix, const MatrixComplex& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row          The one based row number.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(Index row, const MatrixComplex& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row          The one based row number.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(Real row, const MatrixComplex& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row         The one based row number.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Complex& row, const MatrixComplex& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowRange     The one based row number range.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Range& rowRange, const MatrixComplex& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] indexMatrix The one based index number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const MatrixComplex& indexMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowRange The one based row number range.
             *
             * \param[in] column   The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Range& rowRange, Index column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowRange The one based row number range.
             *
             * \param[in] column   The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Range& rowRange, Real column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowRange The one based row number range.
             *
             * \param[in] column   The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Range& rowRange, const Complex& column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowRange    The one based row number range.
             *
             * \param[in] columnRange The one based column number range.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Range& rowRange, const Range& columnRange) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row    The one based row number.
             *
             * \param[in] columnRange The one based column number range.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(Index row, const Range& columnRange) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row    The one based row number.
             *
             * \param[in] columnRange The one based column number range.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(Real row, const Range& columnRange) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row    The one based row number.
             *
             * \param[in] columnRange The one based column number range.
             *
             * \return Returns the value at the specified location.
             */
            MatrixReal at(const Complex& row, const Range& columnRange) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row    The one based row number.
             *
             * \param[in] column The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            Variant at(const Variant& row, Index column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row    The one based row number.
             *
             * \param[in] column The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            Variant at(const Variant& row, Real column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row    The one based row number.
             *
             * \param[in] column The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            Variant at(const Variant& row, const Complex& column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row         The one based row number.
             *
             * \param[in] columnRange The one based column number range.
             *
             * \return Returns the value at the specified location.
             */
            Variant at(const Variant& row, const Range& columnRange) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row       The one based row number.
             *
             * \param[in] columnSet The one based column number set.
             *
             * \return Returns the value at the specified location.
             */
            Variant at(const Variant& row, const Set& columnSet) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row         The one based row number.
             *
             * \param[in] columnTuple The one based column number tuple.
             *
             * \return Returns the value at the specified location.
             */
            Variant at(const Variant& row, const Tuple& columnTuple) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row          The one based row number.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            Variant at(const Variant& row, const MatrixInteger& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row          The one based row number.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            Variant at(const Variant& row, const MatrixReal& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row          The one based row number.
             *
             * \param[in] columnMatrix The one based column number matrix.
             *
             * \return Returns the value at the specified location.
             */
            Variant at(const Variant& row, const MatrixComplex& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row    The one based row number.
             *
             * \param[in] column The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            Variant at(Index row, const Variant& column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row    The one based row number.
             *
             * \param[in] column The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            Variant at(Real row, const Variant& column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row    The one based row number.
             *
             * \param[in] column The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            Variant at(const Complex& row, const Variant& column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowRange The one based row number range.
             *
             * \param[in] column   The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            Variant at(const Range& rowRange, const Variant& column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowSet The one based row number set.
             *
             * \param[in] column The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            Variant at(const Set& rowSet, const Variant& column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowTuple The one based row number tuple.
             *
             * \param[in] column   The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            Variant at(const Tuple& rowTuple, const Variant& column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix The one based row number matrix.
             *
             * \param[in] column    The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            Variant at(const MatrixInteger& rowMatrix, const Variant& column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix The one based row number matrix.
             *
             * \param[in] column    The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            Variant at(const MatrixReal& rowMatrix, const Variant& column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] rowMatrix The one based row number matrix.
             *
             * \param[in] column    The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            Variant at(const MatrixComplex& rowMatrix, const Variant& column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row    The one based row number.
             *
             * \param[in] column The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            Variant at(const Variant& row, const Variant& column) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] index  The one based index number.
             *
             * \return Returns the value at the specified location.
             */
            Variant at(const Variant& index) const;

            /**
             * Method you can use to obtain a pointer to a coefficient value.  This method will throw an exception if
             * the specified row or column is invalid.
             *
             * \param[in] row    The one based row number.
             *
             * \param[in] column The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            const Scalar* pointer(Index row, Index column) const;

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
            template<typename R, typename C, typename NV> M_PUBLIC_TEMPLATE_METHOD void update(
                    R  row,
                    C  column,
                    NV newValue
                ) {
                Integer r;
                Integer c;
                Real    nv;
                if (toInteger(r, row) && toInteger(c, column) && toReal(nv, newValue)) {
                    update(r, c, nv);
                }
            };

            /**
             * Method you can use to update a coefficient at a specified index.  This method will throw an exception
             * if the specified row or column is invalid and will resize the matrix, if needed, to include the
             * specified coefficient.
             *
             * \param[in] index    The one based index into the matrix.
             *
             * \param[in] newValue The new value at the specified location.
             */
            template<typename I, typename NV> M_PUBLIC_TEMPLATE_METHOD void update(
                    I  index,
                    NV newValue
                ) {
                Integer i;
                Real    nv;
                if (toInteger(i, index) && toReal(nv, newValue)) {
                    update(i, nv);
                }
            }

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined left to right.
             *
             * This method exists to simplify templatization of this operation.
             *
             * \return Returns a copy of this matrix.
             */
            inline const MatrixReal& combineLeftToRight() const {
                return *this;
            }

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined left to right.
             *
             * \param[in] other The matrix to combine to the right of this matrix.
             *
             * \return Returns a new matrix representing the combination of the two matrices.
             */
            MatrixReal combineLeftToRight(const MatrixReal& other) const;

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined top to bottom.
             *
             * This method exists to simplify templatization of this operation.
             *
             * \return Returns a copy of this matrix.
             */
            inline const MatrixReal& combineTopToBottom() const {
                return *this;
            }

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined top to bottom.
             *
             * \param[in] other The matrix to combine below this matrix.
             *
             * \return Returns a new matrix representing the combination of the two matrices.
             */
            MatrixReal combineTopToBottom(const MatrixReal& other) const;

            /**
             * Method you can use to reverse the columns in this matrix.
             *
             * \return Returns a column-reversed version of this matrix.
             */
            MatrixReal columnReverse() const;

            /**
             * Method you can use to reverse the rows in this matrix.
             *
             * \return Returns a row-reversed version of this matrix.
             */
            MatrixReal rowReverse() const;

            /**
             * Method you can use to determine if this matrix is square.  That is, whether the matrix has the same
             * number of rows as columns.
             *
             * \return Returns true if this matrix is square.  Returns false if this matrix is not square.
             */
            bool isSquare() const;

            /**
             * Method you can use to determine if this matrix is symmetric.  That is, whether the matrix is equal to
             * it's own transpose.
             *
             * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
             *                              the magnitude of the difference in values, divided by average of the two
             *                              values exceeds this threshold.
             *
             * \return Returns true if this matrix is symmetric.  Returns false if this matrix is not symmetric.
             */
            bool isSymmetric(Real relativeTolerance = defaultRelativeTolerance) const;

            /**
             * Method you can use to determine if this matrix is Hermitian.  That is, whether the matrix is equal to
             * it's own conjugate transpose.
             *
             * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
             *                              the magnitude of the difference in values, divided by average of the two
             *                              values exceeds this threshold.
             *
             * \return Returns true if this matrix is Hermitian.  Returns false if this matrix is not Hermitian.
             */
            inline bool isHermitian(Real relativeTolerance = defaultRelativeTolerance) const {
                return isSymmetric(relativeTolerance);
            }

            /**
             * Method you can use to determine if this matrix is skew symmetric.  That is, whether the matrix is equal
             * to the negative of its own transpose.
             *
             * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
             *                              the magnitude of the difference in values, divided by average of the two
             *                              values exceeds this threshold.
             *
             * \return Returns true if this matrix is skew symmetric.  Returns false if this matrix is not skew
             *         symmetric.
             */
            bool isSkewSymmetric(Real relativeTolerance = defaultRelativeTolerance) const;

            /**
             * Method you can use to determine if this matrix is skew Hermitian.  That is, whether the matrix is equal
             * to the negative of it's own conjugate transpose.
             *
             * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
             *                              the magnitude of the difference in values, divided by average of the two
             *                              values exceeds this threshold.
             *
             * \return Returns true if this matrix is shew Hermitian.  Returns false if this matrix is not skew
             *         Hermitian.
             */
            inline bool isSkewHermitian(Real relativeTolerance = defaultRelativeTolerance) const {
                return isSkewSymmetric(relativeTolerance);
            }

            /**
             * Method you can use to determine if this matrix is normal.  That is, the product of the matrix and its
             * conjugate transpose is equal to the product of the conjugate transpsoe of the matrix and the matrix.
             *
             * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
             *                              the magnitude of the difference in values, divided by average of the two
             *                              values exceeds this threshold.
             *
             * \return Returns true if the matrix is normal.  Returns false if the matrix is not normal.
             */
            bool isNormal(Real relativeTolerance = defaultRelativeTolerance) const;

            /**
             * Method you can use to determine the rank of the matrix.  This method operates using SVD decomposition of
             * the matrix.
             *
             * \param[in] epsilon An espilon value you can use as a threshold for a zero value.  The absolute value of
             *                    each diagonal value will be compared against this value.  Values equal to or less
             *                    than this value will be treated as zero.
             *
             * \return Returns the estimated matrix rank.
             */
            Integer rank(Real epsilon) const;

            /**
             * Method that returns the determinant of this matrix.
             *
             * \return Returns the complex conjugate of this value.
             */
            Scalar determinant() const;

            /**
             * Method that returns the transpose of this matrix.
             *
             * \return Returns the transpose of this matrix.
             */
            MatrixReal transpose() const;

            /**
             * Method that returns the complex conjugate of this matrix.
             *
             * \return Returns the complex conjugate of this value.
             */
            inline const MatrixReal& conj() const {
                return *this;
            }

            /**
             * Method that returns the adjoint of the matrix.
             *
             * \return Returns the adjoint of the matrix.
             */
            inline MatrixReal adjoint() const {
                return transpose();
            }

            /**
             * Method that returns the inverse of the matrix.
             *
             * \return Returns the inverse matrix.  Note that no pivot or singular indicator is provided.
             */
            MatrixReal inverse() const;

            /**
             * Method that perform LU decomposition with partial pivoting on this matrix.
             *
             * \return Returns a tuple containing the pivot matrix, the lower triangular matrix, the upper
             *         triangular matrix, and a flag indicating whether the matrix was not singular.
             */
            Tuple plu() const;

            /**
             * Method that perform singular value decomposition on this matrix.
             *
             * \return Returns a tuple containing the unitary matrix (U), the singular values matrix, (sigma), the
             *         conjugate transpose of the left and right singular vectors (V) of the matrix, and a boolean
             *         flag indicating success (or failure).
             */
            Tuple svd() const;

            /**
             * Method that performs QR decomposition of this matrix.
             *
             * \return Returns a tuple containing the Q unitary matrix, the R upper trianglular matrix representing,
             *         and a flag indicating success.
             */
            Tuple qr() const;

            /**
             * Method that performs LQ decomposition of this matrix.
             *
             * \return Returns a tuple containing the L lower triangular matrix, the Q unitary matrix, and a flag
             *         indicating success.
             */
            Tuple lq() const;

            /**
             * Method that performs Cholesky decomposition of this matrix.  The matrix must be Hermitian.
             *
             * \return Returns the lower triangular matrix representing the Cholesky decomposition of this matrix.
             *         An empty matrix will be returned on failure.
             */
            MatrixReal cholesky() const;

            /**
             * Method that performs Cholesky decomposition of this matrix, creating an upper triangular matrix.  The
             * matrix must be Hermitian.
             *
             * \return Returns the upper triangular matrix representing the Cholesky decomposition of this matrix.
             *         An empty matrix will be returned on failure.
             */
            MatrixReal upperCholesky() const;

            /**
             * Method that reduces this matrix to upper Hessenberg form.
             *
             * \return Returns a tuple containing the Q matrix and the upper Hessenberg matrix.
             */
            Tuple hessenberg() const;

            /**
             * Method that calculates the forward or type 2 discrete cosine transform (DCT) of this matrix.  A matrix
             * with a single dimension will be processed as a one-dimensional DCT.  A matrix with multiple rows and
             * columns will be processed as a two dimensional DCT.
             *
             * \return Returns a matrix representing the complex discrete cosine transform of the matrix.
             */
            MatrixReal dct() const;

            /**
             * Method that calculates the inverse or type 3 discrete cosine transform (DCT) of this matrix.  A matrix
             * with a single dimension will be processed as a one dimensional inverse DFT.  A matrix with multiple rows
             * and columns will be processed as a two dimensional inverse DCT.
             *
             * \return Returns a matrix representing the complex inverse discrete cosine transform of the matrix.
             */
            MatrixReal idct() const;

            /**
             * Method that calculates the Hilbert transform of an array (1 dimensional matrix).
             *
             * \return Returns the Hilbert transform of this matrix.
             */
            MatrixComplex hilbertTransform() const;

            /**
             * Method that calculates the condition number of this matrix.
             *
             * \return Returns the condition number of this matrix.
             */
            Real conditionNumber() const;

            /**
             * Method that calculates the entry-wise p-norm of this matrix.
             *
             * \param[in] p The power term to apply to the norm operation.
             *
             * \return Returns the p-norm of this matrix.
             */
            Real pNorm(Integer p) const;

            /**
             * Method that calculates the entry-wise Euclidean norm of this matrix.
             *
             * \return Returns the Euclidean norm of this matrix.
             */
            Real euclideanNorm() const;

            /**
             * Method that calculates the 1-norm of this matrix.
             *
             * \return Returns the 1-normnorm of this matrix.
             */
            Real oneNorm() const;

            /**
             * Method that calculates the infinity-norm of this matrix.
             *
             * \return Returns the infinity-norm of this matrix.
             */
            Real infinityNorm() const;

            /**
             * Method that calculates a set of row and column factors you can use to equilibrate a matrix.
             *
             * \return Returns a tuple containing two diagnonal matrices plus a flag indicating success.  The first
             *         matrix represents the row scale factors to apply to this matrix.  The second matrix represents
             *         the column scale factors to apply to this matrix.  For the tuple \f$ \left ( R, C \right ) \f$,
             *         you can fully equilibrate the matrix "M" using the relation \f$ R \times M \times C \f$.
             */
            Tuple equilibrate() const;

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
             * \param[in] y The row matrix holding the results of the product \f$ A \times X \f$.
             *
             * \return Returns the resulting values of 'X'.  An empty matrix will be returned if A is singular.
             */
            MatrixReal solve(const MatrixReal& y) const;

            /**
             * Method that solves and over/under determined system using QR or LQ factorization.  When provided an
             * under-determined matrix, this method will determine the minimum Euclidean-norm solution.
             *
             * For over-determined systems, this method will find the solution that exactly satisfies \f$ Ax = b \f$.
             * For under-determined system, this method will find the solution that identifies the value of \f$ x \f$
             * that minimizes \f$ \Vert\Vert A x - b \Vert\Vert _ 2 \f$.
             *
             * \param[in] b The row matrix holding the results of the product \f$ A \times X \f$.
             *
             * \return Returns the resulting values of 'x'.  An empty matrix will be returned if this matrix is not
             *         full rank.
             */
            MatrixReal leastSquares(const MatrixReal& b) const;

            /**
             * Method that returns the Hadamard or Schur product of this matrix and another matrix. This method will
             * assert if the matrices do not have identical dimensions.
             *
             * \param[in] other The matrix to multiply against kernel of the matrix.
             *
             * \return Returns the Hadamard product of the two matrices.
             */
            MatrixReal hadamard(const MatrixReal& other) const;

            /**
             * Method that returns the Kronecker product of this matrix and another matrix.
             *
             * \param[in] other The matrix to multiply against kernel of the matrix.
             *
             * \return Returns the Hadamard product of the two matrices.
             */
            MatrixReal kronecker(const MatrixReal& other) const;

            /**
             * Method you can use to determine the underlying memory layout for the matrix.
             *
             * \return Returns a value indicating the underlying memory layout for the matrix.
             */
            MatrixType matrixType() const override;

            /**
             * Method you can use to access the underlying raw matrix data.
             *
             * \return Returns a pointer to the underlying raw matrix data.
             */
            Scalar* data();

            /**
             * Method you can use to access the underlying raw matrix data.
             *
             * \return Returns a pointer to the underlying raw matrix data.
             */
            const Scalar* data() const;

            /**
             * Method you can use to obtain a matrix holding the floor of each value in the matrix.
             *
             * \return Returns a real matrix with each value equal to the floor of the matrix contents.
             */
            MatrixReal floor() const;

            /**
             * Method you can use to obtain a matrix holding the ceiling of each value in the matrix.
             *
             * \return Returns a real matrix with each value equal to the ceiling of the matrix contents.
             */
            MatrixReal ceil() const;

            /**
             * Method you can use to obtain a matrix holding the nearest integer to each value in the matrix.
             *
             * \return Returns a real matrix with each value equal to the nearest integer of each coefficeint of the
             *         matrix contents.
             */
            MatrixReal nint() const;

            /**
             * Method you can use to convert this matrix to an integer matrix.
             *
             * \return Returns this matrix converted to an integer matrix.  Values are truncated towards zero.
             */
            MatrixInteger truncateToInteger() const;

            /**
             * Method you can use to convert this matrix to an integer matrix.  Integer values represent the floor of
             * the supplied values.
             *
             * \return Returns this matrix converted to an integer matrix.  Values are truncated towards negative
             *         infinity.
             */
            MatrixInteger floorToInteger() const;

            /**
             * Method you can use to convert this matrix to an integer matrix.  Integer values represent the ceiling
             * of the supplied values.
             *
             * \return Returns this matrix converted to an integer matrix.  Values are truncated towards positive
             *         infinity.
             */
            MatrixInteger ceilToInteger() const;

            /**
             * Method you can use to convert this matrix to an integer matrix.  Integer values represent the nearest
             * integer of the supplied values.
             *
             * \return Returns this matrix converted to an integer matrix.  Values are truncated to the closest
             *         integer value.
             */
            MatrixInteger nintToInteger() const;

            /**
             * Method you can use to generate an arbitrary matrix containing uniform random values between 0 and 1,
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
            static MatrixReal randomInclusive(PerThread& pt, Integer numberRows, Integer numberColumns);

            /**
             * Method you can use to generate an arbitrary matrix containing uniform random values between [0, 1).
             *
             * \param[in] pt            The per-thread instance used for random number generation.
             *
             * \param[in] numberRows    The number of rows.
             *
             * \param[in] numberColumns The number of columns.
             *
             * \return Returns a matrix containing the requested distribution.
             */
            static MatrixReal randomInclusiveExclusive(PerThread& pt, Integer numberRows, Integer numberColumns);

            /**
             * Method you can use to generate an arbitrary matrix containing uniform random values between (0, 1].
             *
             * \param[in] pt            The per-thread instance used for random number generation.
             *
             * \param[in] numberRows    The number of rows.
             *
             * \param[in] numberColumns The number of columns.
             *
             * \return Returns a matrix containing the requested distribution.
             */
            static MatrixReal randomExclusiveInclusive(PerThread& pt, Integer numberRows, Integer numberColumns);

            /**
             * Method you can use to generate an arbitrary matrix containing uniform random values between (0, 1).
             *
             * \param[in] pt            The per-thread instance used for random number generation.
             *
             * \param[in] numberRows    The number of rows.
             *
             * \param[in] numberColumns The number of columns.
             *
             * \return Returns a matrix containing the requested distribution.
             */
            static MatrixReal randomExclusive(PerThread& pt, Integer numberRows, Integer numberColumns);

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
            static MatrixReal randomNormal(PerThread& pt, Integer numberRows, Integer numberColumns);

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
            static MatrixReal randomNormal(
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
            static MatrixReal randomWeibull(
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
            static MatrixReal randomExponential(PerThread& pt, Integer numberRows, Integer numberColumns, Real rate);

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
            static MatrixReal randomGamma(
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
            static MatrixReal randomRayleigh(PerThread& pt, Integer numberRows, Integer numberColumns, Real scale);

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
            static MatrixReal randomChiSquared(PerThread& pt, Integer numberRows, Integer numberColumns, Integer k);

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
            static MatrixReal randomLogNormal(PerThread& pt, Integer numberRows, Integer numberColumns);

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
            static MatrixReal randomLogNormal(
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
            static MatrixReal randomCauchyLorentz(
                PerThread& pt,
                Integer    numberRows,
                Integer    numberColumns,
                Real       location,
                Real       scale
            );

            /**
             * Assignment operator
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            MatrixReal& operator=(const MatrixReal& other);

            /**
             * Assignment operator
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            MatrixReal& operator=(const MatrixComplex& other);

            /**
             * Assignment operator
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            MatrixReal& operator=(const MatrixInteger& other);

            /**
             * Assignment operator
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            MatrixReal& operator=(const MatrixBoolean& other);

            /**
             * Assignment operator
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            MatrixReal& operator=(const Variant& other);

            /**
             * Addition assignment operator
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a reference to this instance.
             */
            MatrixReal& operator+=(const MatrixReal& v);

            /**
             * Subtraction assignment operator
             *
             * \param[in] v The value to subtract from this value.
             *
             * \return Returns a reference to this instance.
             */
            MatrixReal& operator-=(const MatrixReal& v);

            /**
             * Multiplication assignment operator
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a reference to this instance.
             */
            MatrixReal& operator*=(const MatrixReal& v);

            /**
             * Multiplication assignment operator
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a reference to this instance.
             */
            MatrixReal& operator*=(Boolean v);

            /**
             * Multiplication assignment operator
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a reference to this instance.
             */
            MatrixReal& operator*=(Integer v);

            /**
             * Multiplication assignment operator
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a reference to this instance.
             */
            MatrixReal& operator*=(Real v);

            /**
             * Division assignment operator
             *
             * \param[in] v The value divide each member by.
             *
             * \return Returns a reference to this instance.
             */
            inline MatrixReal& operator/=(Integer v) {
                (*this) *= (Real(1.0)/Real(v));
                return *this;
            }

            /**
             * Division assignment operator
             *
             * \param[in] v The value divide each member by.
             *
             * \return Returns a reference to this instance.
             */
            inline MatrixReal& operator/=(Real v) {
                (*this) *= (Real(1.0)/v);
                return *this;
            }

            /**
             * Addition operator.
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a matrix representing the sum.
             */
            MatrixReal operator+(const MatrixBoolean& v) const;

            /**
             * Addition operator.
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a matrix representing the sum.
             */
            MatrixReal operator+(const MatrixInteger& v) const;

            /**
             * Addition operator.
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a matrix representing the sum.
             */
            MatrixReal operator+(const MatrixReal& v) const;

            /**
             * Subtraction operator.
             *
             * \param[in] v The value to subtract from this value.
             *
             * \return Returns a matrix representing the difference.
             */
            MatrixReal operator-(const MatrixBoolean& v) const;

            /**
             * Subtraction operator.
             *
             * \param[in] v The value to subtract from this value.
             *
             * \return Returns a matrix representing the difference.
             */
            MatrixReal operator-(const MatrixInteger& v) const;

            /**
             * Subtraction operator.
             *
             * \param[in] v The value to subtract from this value.
             *
             * \return Returns a matrix representing the difference.
             */
            MatrixReal operator-(const MatrixReal& v) const;

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a matrix representing the product.
             */
            MatrixReal operator*(const MatrixBoolean& v) const;

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a matrix representing the product.
             */
            MatrixReal operator*(const MatrixInteger& v) const;

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a matrix representing the product.
             */
            MatrixReal operator*(const MatrixReal& v) const;

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a matrix representing the product.
             */
            MatrixReal operator*(Boolean v) const;

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a matrix representing the product.
             */
            MatrixReal operator*(Integer v) const;

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply with this value.
             *
             * \return Returns a matrix representing the product.
             */
            MatrixReal operator*(Real v) const;

            /**
             * Division operator.
             *
             * \param[in] v The value to divide this value by.
             *
             * \return Returns a matrix representing the quotient.
             */
            MatrixReal operator/(Boolean v) const;

            /**
             * Division operator.
             *
             * \param[in] v The value to divide this value by.
             *
             * \return Returns a matrix representing the quotient.
             */
            MatrixReal operator/(Integer v) const;

            /**
             * Division operator.
             *
             * \param[in] v The value to divide this value by.
             *
             * \return Returns a matrix representing the quotient.
             */
            MatrixReal operator/(Real v) const;

            /**
             * Unary plus operator.
             *
             * \return Returns a reference to this object.
             */
            inline const MatrixReal& operator+() const {
                return *this;
            }

            /**
             * Unary minus operator.
             *
             * \return Returns a reference to this object.
             */
            MatrixReal operator-() const;

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are equal.  Returns false if the values are not equal.
             */
            bool operator==(const MatrixReal& other) const;

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are not equal.  Returns false if the values are equal.
             */
            inline bool operator!=(const MatrixReal& other) const {
                return !operator==(other);
            }

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
             * \param[in] index The one based index to the method.  On matrices, the index will go in row/column order.
             *
             * \return Returns the value at the specified location.
             */
            inline Scalar operator()(Index index) const {
                return at(index);
            }

            /**
             * Array subscript operator.
             *
             * \param[in] rowIterable The iterable to use to create the submatrix.
             *
             * \param[in] column      The column to use to create the submatrix.
             *
             * \return Returns the desired submatrix.
             */
            template<typename RI> M_PUBLIC_TEMPLATE_METHOD MatrixReal operator()(RI rowIterable, Index column) const {
                return at(rowIterable, column);
            }

            /**
             * Array subscript operator.
             *
             * \param[in] row            The row to use to create the submatrix.
             *
             * \param[in] columnIterable The column iterable to use to create the submatrix.
             *
             * \return Returns the desired submatrix.
             */
            template<typename CI> M_PUBLIC_TEMPLATE_METHOD MatrixReal operator()(Index row, CI columnIterable) const {
                return at(row, columnIterable);
            }

            /**
             * Array subscript operator.
             *
             * \param[in] rowIterable    The row iterable to use to create the submatrix.
             *
             * \param[in] columnIterable The column iterable to use to create the submatrix.
             *
             * \return Returns the desired submatrix.
             */
            template<typename RI, typename CI> M_PUBLIC_TEMPLATE_METHOD MatrixReal operator()(
                    RI rowIterable,
                    CI columnIterable
                ) const {
                return at(rowIterable, columnIterable);
            }

            /**
             * Method you can call to determine the recommended ordering of this matrix relative to another matrix.
             * Ordering is based on:
             *
             *     * Whether the underlying data structures, lazy transforms, and scalar multipliers are the same.
             *
             *     * Number of matrix rows and columns.
             *
             *     * Value of each coefficient in the matrix.
             *
             * \param[in] other The other set to compare against.
             *
             * \return Returns a negative value if this matrix should precede the order matrix.  Returns 0 if the
             *         matrices are equal.  Returns a positive value if this matrix should follow the other matrix.
             */
            int relativeOrder(const MatrixReal& other) const;

        private:
            class Data;
            class SparseData;
            class DenseData;

            /**
             * Constructor
             *
             * \param[in] dataStore The data store backing this matrix.
             */
            MatrixReal(Data* dataStore);

            /**
             * Method that copies the contents of a boolean matrix into this object.
             *
             * \param[in] booleanMatrix The boolean matrix to be copied into this matrix.
             */
            void copyFrom(const MatrixBoolean& booleanMatrix);

            /**
             * Method that copies the contents of an integer matrix into this object.
             *
             * \param[in] integerMatrix The integer matrix to be copied into this matrix.
             */
            void copyFrom(const MatrixInteger& integerMatrix);

            /**
             * Method that copies the contents of a complex matrix into this object.
             *
             * \param[in] complexMatrix The complex matrix to be copied into this matrix.
             */
            void copyFrom(const MatrixComplex& complexMatrix);

            /**
             * Method that releases a data store, possibly deleting it.
             *
             * \param[in] dataStore The data store to be released.  Note that the data store is expected to be unlocked
             *                      prior to this call.
             */
            void releaseDataStore(Data* dataStore) const;

            /**
             * Method that assigns a new data store to this class.  This version will not increment the reference count
             * of the data store.
             *
             * \param[in] newDataStore The data store to be assigned to this class.
             *
             * \param[in] unlock       If true, the current data store needs to be unlocked.
             */
            void assignNewDataStore(Data* newDataStore, bool unlock) const;

            /**
             * Method that assigns an existing data store to this class.  This version will increment the reference
             * count of the data store.
             *
             * \param[in] newDataStore The data store to be assigned to this class.
             *
             * \param[in] unlock       If true, the current data store needs to be unlocked.
             */
            void assignExistingDataStore(Data* newDataStore, bool unlock) const;

            /**
             * Method that applies any pending lazy operations on this matrix.
             */
            void applyLazyTransformsAndScaling() const;

            /**
             * Instance that provides the underlying data store for the matrix.
             */
            mutable Data* currentData;

            /**
             * True if there is a pending transpose on the matrix.
             */
            mutable bool pendingTranspose;

            /**
             * Current scaler value.
             */
            mutable Scalar pendingScalarValue;
    };

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns a real matrix value representing the product.
     */
    M_PUBLIC_API MatrixReal operator*(Boolean a, const MatrixReal& b);

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns a real matrix value representing the product.
     */
    M_PUBLIC_API MatrixReal operator*(Integer a, const MatrixReal& b);

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns a real matrix value representing the product.
     */
    M_PUBLIC_API MatrixReal operator*(Real a, const MatrixBoolean& b);

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns a real matrix value representing the product.
     */
    M_PUBLIC_API MatrixReal operator*(Real a, const MatrixInteger& b);

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns a real matrix value representing the product.
     */
    M_PUBLIC_API MatrixReal operator*(Real a, const MatrixReal& b);

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns a real matrix value representing the product.
     */
    M_PUBLIC_API MatrixReal operator*(const MatrixBoolean& a, Real b);

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns a real matrix value representing the product.
     */
    M_PUBLIC_API MatrixReal operator*(const MatrixInteger& a, Real b);

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns a real matrix value representing the product.
     */
    M_PUBLIC_API MatrixReal operator*(const MatrixBoolean& a, const MatrixReal& b);

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns a real matrix value representing the product.
     */
    M_PUBLIC_API MatrixReal operator*(const MatrixInteger& a, const MatrixReal& b);

    /**
     * Scalar division operator.
     *
     * \param[in] a The dividend.
     *
     * \param[in] b The divisor.
     *
     * \return Returns a real matrix value representing the quotient.
     */
    M_PUBLIC_API MatrixReal operator/(const MatrixBoolean& a, Integer b);

    /**
     * Scalar division operator.
     *
     * \param[in] a The dividend.
     *
     * \param[in] b The divisor.
     *
     * \return Returns a real matrix value representing the quotient.
     */
    M_PUBLIC_API MatrixReal operator/(const MatrixBoolean& a, Real b);

    /**
     * Scalar division operator.
     *
     * \param[in] a The dividend.
     *
     * \param[in] b The divisor.
     *
     * \return Returns a real matrix value representing the quotient.
     */
    M_PUBLIC_API MatrixReal operator/(const MatrixInteger& a, Integer b);

    /**
     * Scalar division operator.
     *
     * \param[in] a The dividend.
     *
     * \param[in] b The divisor.
     *
     * \return Returns a real matrix value representing the quotient.
     */
    M_PUBLIC_API MatrixReal operator/(const MatrixInteger& a, Real b);

    /**
     * Matrix addition operator.
     *
     * \param[in] a The augend.
     *
     * \param[in] b The addend.
     *
     * \return Returns a real matrix value representing the sum.
     */
    M_PUBLIC_API MatrixReal operator+(const MatrixBoolean& a, const MatrixReal& b);

    /**
     * Matrix addition operator.
     *
     * \param[in] a The augend.
     *
     * \param[in] b The addend.
     *
     * \return Returns a real matrix value representing the sum.
     */
    M_PUBLIC_API MatrixReal operator+(const MatrixInteger& a, const MatrixReal& b);

    /**
     * Matrix subtraction operator.
     *
     * \param[in] a The minuend.
     *
     * \param[in] b The subtrahend.
     *
     * \return Returns a real matrix value representing the difference.
     */
    M_PUBLIC_API MatrixReal operator-(const MatrixBoolean& a, const MatrixReal& b);

    /**
     * Matrix subtraction operator.
     *
     * \param[in] a The minuend.
     *
     * \param[in] b The subtrahend.
     *
     * \return Returns a real matrix value representing the difference.
     */
    M_PUBLIC_API MatrixReal operator-(const MatrixInteger& a, const MatrixReal& b);

    /* Export template class specializations that we depend upon. */
    M_PUBLIC_TEMPLATE_CLASS(MatrixIterator<MatrixReal>)
}

#endif
