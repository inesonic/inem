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
* This header defines the the \ref M::MatrixInteger class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_MATRIX_INTEGER_H
#define M_MATRIX_INTEGER_H

#include "m_common.h"
#include "m_api_types.h"
#include "m_type_conversion.h"
#include "m_intrinsics.h"
#include "m_intrinsic_types.h"
#include "m_type_conversion.h"
#include "m_matrix_iterator.h"
#include "model_matrix_integer.h"
#include "model_matrix.h"

namespace M {
    class PerThread;
    class MatrixBoolean;
    class MatrixInteger;
    class MatrixComplex;
    class Set;
    class Tuple;
    class Variant;

    /**
     * Matrix class holding integer values.  This class uses copy-on-write semantics to reduce memory utilization as
     * well as lazy evaluation of expressions to reduce processing overhead.  Note that the class is intended primarily
     * for use as a storage class and provides greatly reduced functionality compared to \ref Model::MatrixComplex and
     * \ref Model::MatrixReal
     */
    class M_PUBLIC_API MatrixInteger:public Model::Matrix {
        friend class MatrixBoolean;
        friend class MatrixReal;
        friend class MatrixComplex;

        public:
            /**
             * The type contained in this matrix.
             */
            typedef Integer Scalar;

            /**
             * The iterator used to traverse this matrix.
             */
            typedef MatrixIterator<MatrixInteger> Iterator;

            /**
             * Iterator type.
             */
            typedef MatrixIterator<MatrixInteger> ConstIterator;

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
             * Constructor
             *
             * \param[in] newNumberRows    The desired number of rows.
             *
             * \param[in] newNumberColumns The desired number of columns.
             */
            MatrixInteger(Integer newNumberRows = 0, Integer newNumberColumns = 0);

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
            MatrixInteger(Integer numberRows, Integer numberColumns, const Scalar* matrixData);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            MatrixInteger(const MatrixBoolean& other);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            MatrixInteger(const MatrixInteger& other);

            /**
             * Cooy constructor
             *
             * \param[in] other The instance to be copied.
             */
            MatrixInteger(const MatrixReal& other);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            MatrixInteger(const MatrixComplex& other);

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
            template<typename... Coefficients> M_PUBLIC_TEMPLATE_METHOD static MatrixInteger build(
                    const Integer   numberRows,
                    const Integer   numberColumns,
                    Coefficients... coefficients
                ) {
                Scalar matrixData[] = { inlineToInteger(coefficients)... };
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
            bool toFile(const char* filename, Model::DataFileFormat fileFormat) const;

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
            static MatrixInteger identity(Index numberRows, Index numberColumns);

            /**
             * Method you can use to obtain an identity matrix of a specified size.  This version will always create
             * a square matrix.
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
            MatrixInteger at(const Set& rowSet, Index column) const;

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
            MatrixInteger at(const Set& rowSet, Real column) const;

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
            MatrixInteger at(const Set& rowSet, const Complex& column) const;

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
            MatrixInteger at(const Set& rowSet, const Range& columnRange) const;

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
            MatrixInteger at(const Set& rowSet, const Set& columnSet) const;

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
            MatrixInteger at(const Set& rowSet, const Tuple& columnTuple) const;

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
            MatrixInteger at(const Set& rowSet, const MatrixInteger& columnMatrix) const;

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
            MatrixInteger at(const Set& rowSet, const MatrixReal& columnMatrix) const;

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
            MatrixInteger at(const Set& rowSet, const MatrixComplex& columnMatrix) const;

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
            MatrixInteger at(Index row, const Set& columnSet) const;

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
            MatrixInteger at(Real row, const Set& columnSet) const;

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
            MatrixInteger at(const Complex& row, const Set& columnSet) const;

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
            MatrixInteger at(const Range& rowRange, const Set& columnSet) const;

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
            MatrixInteger at(const Tuple& rowTuple, const Set& columnSet) const;

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
            MatrixInteger at(const MatrixInteger& rowMatrix, const Set& columnSet) const;

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
            MatrixInteger at(const MatrixReal& rowMatrix, const Set& columnSet) const;

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
            MatrixInteger at(const MatrixComplex& rowMatrix, const Set& columnSet) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] indexSet  The one based index number set.
             *
             * \return Returns the value at the specified location.
             */
            MatrixInteger at(const Set& indexSet) const;

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
            MatrixInteger at(const Tuple& rowTuple, Index column) const;

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
            MatrixInteger at(const Tuple& rowTuple, Real column) const;

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
            MatrixInteger at(const Tuple& rowTuple, const Complex& column) const;

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
            MatrixInteger at(const Tuple& rowTuple, const Range& columnRange) const;

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
            MatrixInteger at(const Tuple& rowTuple, const Tuple& columnTuple) const;

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
            MatrixInteger at(const Tuple& rowTuple, const MatrixInteger& columnMatrix) const;

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
            MatrixInteger at(const Tuple& rowTuple, const MatrixReal& columnMatrix) const;

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
            MatrixInteger at(const Tuple& rowTuple, const MatrixComplex& columnMatrix) const;

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
            MatrixInteger at(Index row, const Tuple& columnTuple) const;

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
            MatrixInteger at(Real row, const Tuple& columnTuple) const;

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
            MatrixInteger at(const Complex& row, const Tuple& columnTuple) const;

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
            MatrixInteger at(const Range& rowRange, const Tuple& columnTuple) const;

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
            MatrixInteger at(const MatrixInteger& rowMatrix, const Tuple& columnTuple) const;

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
            MatrixInteger at(const MatrixReal& rowMatrix, const Tuple& columnTuple) const;

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
            MatrixInteger at(const MatrixComplex& rowMatrix, const Tuple& columnTuple) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] indexTuple  The one based index number tuple.
             *
             * \return Returns the value at the specified location.
             */
            MatrixInteger at(const Tuple& indexTuple) const;

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
            MatrixInteger at(const MatrixInteger& rowMatrix, Index column) const;

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
            MatrixInteger at(const MatrixInteger& rowMatrix, Real column) const;

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
            MatrixInteger at(const MatrixInteger& rowMatrix, const Complex& column) const;

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
            MatrixInteger at(const MatrixInteger& rowMatrix, const Range& columnRange) const;

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
            MatrixInteger at(const MatrixInteger& rowMatrix, const MatrixInteger& columnMatrix) const;

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
            MatrixInteger at(const MatrixInteger& rowMatrix, const MatrixReal& columnMatrix) const;

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
            MatrixInteger at(const MatrixInteger& rowMatrix, const MatrixComplex& columnMatrix) const;

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
            MatrixInteger at(Index row, const MatrixInteger& columnMatrix) const;

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
            MatrixInteger at(Real row, const MatrixInteger& columnMatrix) const;

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
            MatrixInteger at(const Complex& row, const MatrixInteger& columnMatrix) const;

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
            MatrixInteger at(const Range& rowRange, const MatrixInteger& columnMatrix) const;

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
            MatrixInteger at(const MatrixReal& rowMatrix, const MatrixInteger& columnMatrix) const;

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
            MatrixInteger at(const MatrixComplex& rowMatrix, const MatrixInteger& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] indexMatrix The one based index number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixInteger at(const MatrixInteger& indexMatrix) const;

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
            MatrixInteger at(const MatrixReal& rowMatrix, Index column) const;

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
            MatrixInteger at(const MatrixReal& rowMatrix, Real column) const;

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
            MatrixInteger at(const MatrixReal& rowMatrix, const Complex& column) const;

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
            MatrixInteger at(const MatrixReal& rowMatrix, const Range& columnRange) const;

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
            MatrixInteger at(const MatrixReal& rowMatrix, const MatrixReal& columnMatrix) const;

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
            MatrixInteger at(const MatrixReal& rowMatrix, const MatrixComplex& columnMatrix) const;

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
            MatrixInteger at(Index row, const MatrixReal& columnMatrix) const;

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
            MatrixInteger at(Real row, const MatrixReal& columnMatrix) const;

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
            MatrixInteger at(const Complex& row, const MatrixReal& columnMatrix) const;

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
            MatrixInteger at(const Range& rowRange, const MatrixReal& columnMatrix) const;

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
            MatrixInteger at(const MatrixComplex& rowMatrix, const MatrixReal& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] indexMatrix The one based index number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixInteger at(const MatrixReal& indexMatrix) const;

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
            MatrixInteger at(const MatrixComplex& rowMatrix, Index column) const;

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
            MatrixInteger at(const MatrixComplex& rowMatrix, Real column) const;

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
            MatrixInteger at(const MatrixComplex& rowMatrix, const Complex& column) const;

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
            MatrixInteger at(const MatrixComplex& rowMatrix, const Range& columnRange) const;

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
            MatrixInteger at(const MatrixComplex& rowMatrix, const MatrixComplex& columnMatrix) const;

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
            MatrixInteger at(Index row, const MatrixComplex& columnMatrix) const;

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
            MatrixInteger at(Real row, const MatrixComplex& columnMatrix) const;

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
            MatrixInteger at(const Complex& row, const MatrixComplex& columnMatrix) const;

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
            MatrixInteger at(const Range& rowRange, const MatrixComplex& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] indexMatrix The one based index number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixInteger at(const MatrixComplex& indexMatrix) const;

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
            MatrixInteger at(const Range& rowRange, Index column) const;

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
            MatrixInteger at(const Range& rowRange, Real column) const;

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
            MatrixInteger at(const Range& rowRange, const Complex& column) const;

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
            MatrixInteger at(const Range& rowRange, const Range& columnRange) const;

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
            MatrixInteger at(Index row, const Range& columnRange) const;

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
            MatrixInteger at(Real row, const Range& columnRange) const;

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
            MatrixInteger at(const Complex& row, const Range& columnRange) const;

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
                Integer nv;
                if (toInteger(r, row) && toInteger(c, column) && toInteger(nv, newValue)) {
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
                Integer nv;
                if (toInteger(i, index) && toInteger(nv, newValue)) {
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
            inline const MatrixInteger& combineLeftToRight() const {
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
            MatrixInteger combineLeftToRight(const MatrixInteger& other) const;

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined top to bottom.
             *
             * This method exists to simplify templatization of this operation.
             *
             * \return Returns a copy of this matrix.
             */
            inline const MatrixInteger& combineTopToBottom() const {
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
             * assert if the matrices do not have identical dimensions.
             *
             * \param[in] other The matrix to multiply against kernel of the matrix.
             *
             * \return Returns the Hadamard product of the two matrices.
             */
            MatrixInteger hadamard(const MatrixInteger& other) const;

            /**
             * Method that returns the Kronecker product of this matrix and another matrix.
             *
             * \param[in] other The matrix to multiply against kernel of the matrix.
             *
             * \return Returns the Hadamard product of the two matrices.
             */
            MatrixInteger kronecker(const MatrixInteger& other) const;

            /**
             * Method you can use to determine the underlying memory layout for the matrix.
             *
             * \return Returns a value indicating the underlying memory layout for the matrix.
             */
            MatrixType matrixType() const override;

            /**
             * Method you can use to access and update the underlying raw matrix data.
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
            static MatrixInteger randomInteger64(PerThread& pt, Integer numberRows, Integer numberColumns);

            /**
             * Method you can use to generate an arbitrary matrix containing random 32-bit unsigned integers.
             *
             * \param[in] pt            The per-thread instance used for random number generation.
             *
             * \param[in] numberRows    The number of rows.
             *
             * \param[in] numberColumns The number of columns.
             *
             * \return Returns a matrix containing the requested distribution.
             */
            static MatrixInteger randomInteger32(PerThread& pt, Integer numberRows, Integer numberColumns);

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
            static MatrixInteger randomPoisson(PerThread& pt, Integer numberRows, Integer numberColumns, Real rate);

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
            static MatrixInteger randomBinomial(
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
            static MatrixInteger randomGeometric(PerThread& pt, Integer numberRows, Integer numberColumns, Real p);

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
            MatrixInteger& operator=(const MatrixBoolean& other);

            /**
             * Assignment operator
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            MatrixInteger& operator=(const MatrixReal& other);

            /**
             * Assignment operator
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            MatrixInteger& operator=(const MatrixComplex& other);

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
            MatrixInteger& operator+=(const MatrixInteger& v);

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
             * \return Returns a reference to this object.
             */
            MatrixInteger operator-() const;

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
            inline bool operator!=(const MatrixInteger& other) const {
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
            template<typename RI> M_PUBLIC_TEMPLATE_METHOD MatrixInteger operator()(
                    RI    rowIterable,
                    Index column
                ) const {
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
            template<typename CI> M_PUBLIC_TEMPLATE_METHOD MatrixInteger operator()(
                    Index row,
                    CI    columnIterable
                ) const {
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
            template<typename RI, typename CI> M_PUBLIC_TEMPLATE_METHOD MatrixInteger operator()(
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
            int relativeOrder(const MatrixInteger& other) const;

        private:
            class Data;
            class SparseData;
            class DenseData;

            /**
             * Constructor
             *
             * \param[in] dataStore The data store backing this matrix.
             */
            MatrixInteger(Data* dataStore);

            /**
             * Method that copies the contents of a boolean matrix into this object.
             *
             * \param[in] booleanMatrix The boolean matrix to be copied into this matrix.
             */
            void copyFrom(const MatrixBoolean& booleanMatrix);

            /**
             * Method that copies the contents of a real matrix into this object.
             *
             * \param[in] realMatrix The real matrix to be copied into this matrix.
             */
            void copyFrom(const MatrixReal& realMatrix);

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
             * Current scaler multiplier value.
             */
            mutable Scalar pendingScalarMultiplierValue;
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
    M_PUBLIC_API MatrixInteger operator*(Boolean a, const MatrixInteger& b);

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns an integer matrix value representing the product.
     */
    M_PUBLIC_API MatrixInteger operator*(Integer a, const MatrixInteger& b);

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns an integer matrix value representing the product.
     */
    M_PUBLIC_API MatrixInteger operator*(Integer a, const MatrixBoolean& b);

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns an integer matrix value representing the product.
     */
    M_PUBLIC_API MatrixInteger operator*(const MatrixBoolean& a, const MatrixBoolean& b);

    /**
     * Scalar multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns an integer matrix value representing the product.
     */
    M_PUBLIC_API MatrixInteger operator*(const MatrixBoolean& a, const MatrixInteger& b);

    /**
     * Matrix addition operator.
     *
     * \param[in] a The augend.
     *
     * \param[in] b The addend.
     *
     * \return Returns a complex matrix value representing the sum.
     */
    M_PUBLIC_API MatrixInteger operator+(const MatrixBoolean& a, const MatrixBoolean& b);

    /**
     * Matrix addition operator.
     *
     * \param[in] a The augend.
     *
     * \param[in] b The addend.
     *
     * \return Returns a complex matrix value representing the sum.
     */
    M_PUBLIC_API MatrixInteger operator+(const MatrixBoolean& a, const MatrixInteger& b);

    /**
     * Matrix subtraction operator.
     *
     * \param[in] a The minuend.
     *
     * \param[in] b The subtrahend.
     *
     * \return Returns a complex matrix value representing the difference.
     */
    M_PUBLIC_API MatrixInteger operator-(const MatrixBoolean& a, const MatrixBoolean& b);

    /**
     * Matrix subtraction operator.
     *
     * \param[in] a The minuend.
     *
     * \param[in] b The subtrahend.
     *
     * \return Returns a complex matrix value representing the difference.
     */
    M_PUBLIC_API MatrixInteger operator-(const MatrixBoolean& a, const MatrixInteger& b);

    /**
     * Overloaded multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns a boolean matrix.
     */
    M_PUBLIC_API MatrixInteger operator*(Boolean a, const MatrixBoolean& b);

    /**
     * Overloaded multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns a boolean matrix.
     */
    M_PUBLIC_API MatrixInteger operator*(const MatrixBoolean& a, Boolean b);

    /**
     * Overloaded multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns a boolean matrix.
     */
    M_PUBLIC_API MatrixInteger operator*(const MatrixBoolean& a, const Integer b);

    /* Export template class specializations that we depend upon. */
    M_PUBLIC_TEMPLATE_CLASS(MatrixIterator<MatrixInteger>)
}

#endif
