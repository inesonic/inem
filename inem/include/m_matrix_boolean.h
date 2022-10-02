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
* This header defines the the \ref M::MatrixBoolean class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_MATRIX_BOOLEAN_H
#define M_MATRIX_BOOLEAN_H

#include "m_common.h"
#include "m_api_types.h"
#include "m_intrinsics.h"
#include "m_type_conversion.h"
#include "m_intrinsic_types.h"
#include "m_matrix_iterator.h"
#include "model_matrix_integer.h"
#include "model_matrix.h"

namespace M {
    class Set;
    class Tuple;
    class MatrixInteger;
    class MatrixReal;
    class MatrixComplex;
    class Variant;

    /**
     * Matrix class holding boolean values.  This class uses copy-on-write semantics to reduce memory utilization and
     * to reduce processing overhead.  Note that the class is intended for use as a storage class and does not provide
     * traditional matrix operations such as addition, multiplication, etc.  If you need a matrix class with support
     * for matrix mathematics, consider using \ref M::MatrixInteger, \ref M::MatrixReal, or \ref M::MatrixComplex.
     */
    class M_PUBLIC_API MatrixBoolean:public Model::Matrix {
        friend class MatrixInteger;
        friend class MatrixReal;
        friend class MatrixComplex;

        public:
            /**
             * The type contained in this matrix.
             */
            typedef Boolean Scalar;

            /**
             * Matrix iterator class.  You can use this class as a generic base class for matrix iterators.
             *
             * \param[in] T The matrix type we are iterator over.
             *
             * \param[in] S The scalar the matrix supports.
             */
            class M_PUBLIC_API Iterator {
                public:
                    Iterator();

                    /**
                     * Constructor
                     *
                     * \param[in] matrix A pointer to the matrix class we are iterator over.
                     *
                     * \param[in] row    The one based row index to set this iterator to.
                     *
                     * \param[in] column The one based column index to set this iterator to.
                     */
                    Iterator(const MatrixBoolean* matrix, unsigned long row, unsigned long column);

                    /**
                     * Copy constructor
                     *
                     * \param[in] other The iterator to assign to this iterator.
                     */
                    Iterator(const Iterator& other);

                    ~Iterator();

                    /**
                     * Method you can use to obtain the current value pointed to by the iterator.
                     *
                     * \return Returns a reference to the current value.
                     */
                    inline Scalar value() const {
                        return currentValue;
                    }

                    /**
                     * Method you can use to obtain a reference to the current value pointed to by the iterator.
                     *
                     * \return Returns a reference to the current value.
                     */
                    inline const Scalar& constReference() const {
                        return currentValue;
                    }

                    /**
                     * Method you can use to obtain a pointer to the current value.
                     *
                     * \return Returns a pointer to the current value.
                     */
                    const Scalar* constPointer() const {
                        return &currentValue;
                    }

                    /**
                     * Method you can use to advance the iterator by one position.
                     */
                    void advance();

                    /**
                     * Method you can use to advance the iterator by one position by a specified distance.
                     *
                     * \param[in] distance The distance to advance the iterator.
                     */
                    void advance(unsigned long long distance);

                    /**
                     * Method you can use to determine if this iterator is pointing to a valid location.
                     *
                     * \return Returns true if this iterator is pointing to a valid location in the container.  Returns
                     *         false if the iterator is not initialized or is pointing to an invalid location.
                     */
                    bool isValid() const;

                    /**
                     * Method you can use to determine if this iterator is pointing to an invalid location.
                     *
                     * \return Returns true if this iterator is not initialized or is pointing to an invalid location
                     *         in the container.   Returns false if the iterator is pointint to a valid location.
                     */
                    inline bool isInvalid() const {
                        return !isValid();
                    }

                    /**
                     * Deferencing operator
                     *
                     * \return Returns a reference to the object.
                     */
                    inline Scalar operator*() const {
                        return value();
                    }

                    /**
                     * Pre-increment operator.
                     *
                     * \return Returns a reference to this iterator.
                     */
                    inline Iterator& operator++() {
                        advance();
                        return *this;
                    }

                    /**
                     * Post-increment operator.
                     *
                     * \return Returns a new iterator pointing to the old location.
                     */
                    inline Iterator operator++(int) {
                        Iterator result = *this;
                        advance();

                        return result;
                    }

                    /**
                     * Addition assignment operator.
                     *
                     * \param[in] distance The distance to adjust this iterator.
                     *
                     * \return Returns a reference to this iterator.
                     */
                    inline Iterator& operator+=(unsigned long long distance) {
                        advance(distance);
                        return *this;
                    }

                    /**
                     * Comparison operator.
                     *
                     * \param[in] other The instance to be compared against.
                     *
                     * \return Returns true if the iterators point to the same element.  Returns false if the
                     *         iterators point to different elements.
                     */
                    inline bool operator==(const Iterator& other) const {
                        return currentRow == other.currentRow && currentColumn == other.currentColumn;
                    }

                    /**
                     * Comparison operator.
                     *
                     * \param[in] other The instance to be compared against.
                     *
                     * \return Returns true if the iterators point to the same element.  Returns false if the
                     *         iterators point to different elements.
                     */
                    inline bool operator!=(const Iterator& other) const {
                        return currentRow != other.currentRow || currentColumn != other.currentColumn;
                    }

                    /**
                     * Copy assignment operator.
                     *
                     * \param[in] other The instance to assign to this instance.
                     *
                     * \return Returns a reference to this instance.
                     */
                    Iterator& operator=(const Iterator& other);

                private:
                    /**
                     * The matrix this iterator is operating over.
                     */
                    const MatrixBoolean* currentMatrix;

                    /**
                     * The underlying value.
                     */
                    M::Boolean currentValue;

                    /**
                     * The current iterator row.
                     */
                    unsigned long currentRow;

                    /**
                     * The current iterator column.
                     */
                    unsigned long currentColumn;
            };

            /**
             * Type used for STL compatibility.
             */
            typedef Iterator iterator;

            /**
             * Iterator type.
             */
            typedef Iterator ConstIterator;

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
            MatrixBoolean(Integer newNumberRows = 0, Integer newNumberColumns = 0);

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
            MatrixBoolean(Integer numberRows, Integer numberColumns, const Scalar* matrixData);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            MatrixBoolean(const MatrixBoolean& other);

            /**
             * Constructor
             *
             * \param[in] other The instance to be copied.
             */
            MatrixBoolean(const MatrixInteger& other);

            /**
             * Constructor
             *
             * \param[in] other The instance to be copied.
             */
            MatrixBoolean(const MatrixReal& other);

            /**
             * Constructor
             *
             * \param[in] other The instance to be copied.
             */
            MatrixBoolean(const MatrixComplex& other);

            /**
             * Constructor
             *
             * \param[in] other The instance to copy to this instance.
             */
            MatrixBoolean(const Variant& other);

            ~MatrixBoolean() override;

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
            template<typename... Coefficients> M_PUBLIC_TEMPLATE_METHOD static MatrixBoolean build(
                    const Integer   numberRows,
                    const Integer   numberColumns,
                    Coefficients... coefficients
                ) {
                Scalar matrixData[] = { inlineToBoolean(coefficients)... };
                return MatrixBoolean(numberRows, numberColumns, matrixData);
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
            static MatrixBoolean fromFile(const char* filename);

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
            static MatrixBoolean identity(Index numberRows, Index numberColumns);

            /**
             * Method you can use to obtain an identity matrix of a specified size.  This version will always create
             * a square matrix.
             *
             * \param[in] numberRowColumns The number of rows in the matrix.
             */
            static MatrixBoolean identity(Index numberRowColumns);

            /**
             * Method you can use to obtain a zero matrix of a specified size.
             *
             * \param[in] numberRows    The number of rows in the matrix.
             *
             * \param[in] numberColumns The number of columns in the matrix.
             */
            static inline MatrixBoolean zero(Index numberRows, Index numberColumns) {
                return MatrixBoolean(numberRows, numberColumns);
            }

            /**
             * Method you can use to obtain a zero matrix of a specified size.  This version will always create a
             * square matrix.
             *
             * \param[in] numberRowColumns The number of rows in the matrix.
             */
            static inline MatrixBoolean zero(Index numberRowColumns) {
                return MatrixBoolean(numberRowColumns, numberRowColumns);
            }

            /**
             * Method you can use to obtain a one's matrix of a specified size.
             *
             * \param[in] numberRows    The number of rows in the matrix.
             *
             * \param[in] numberColumns The number of columns in the matrix.
             */
            static MatrixBoolean ones(Index numberRows, Index numberColumns);

            /**
             * Method you can use to obtain a one's matrix of a specified size.  This version will always create a
             * square matrix.
             *
             * \param[in] numberRowColumns The number of rows in the matrix.
             */
            static MatrixBoolean ones(Index numberRowColumns);

            /**
             * Method you can use to obtain the diagnonal entries from this matrix.
             *
             * \return Returns a column matrix holding the diagnonal entries of the matrix.
             */
            MatrixBoolean diagonalEntries() const;

            /**
             * Method you can use construct a diagnonal matrix from a row or column matrix.
             *
             * \return Returns a newly constructed square matrix from the supplied diagnonal entries.
             */
            MatrixBoolean diagonal() const;

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
            MatrixBoolean at(const Set& rowSet, Index column) const;

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
            MatrixBoolean at(const Set& rowSet, Real column) const;

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
            MatrixBoolean at(const Set& rowSet, const Complex& column) const;

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
            MatrixBoolean at(const Set& rowSet, const Range& columnRange) const;

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
            MatrixBoolean at(const Set& rowSet, const Set& columnSet) const;

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
            MatrixBoolean at(const Set& rowSet, const Tuple& columnTuple) const;

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
            MatrixBoolean at(const Set& rowSet, const MatrixInteger& columnMatrix) const;

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
            MatrixBoolean at(const Set& rowSet, const MatrixReal& columnMatrix) const;

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
            MatrixBoolean at(const Set& rowSet, const MatrixComplex& columnMatrix) const;

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
            MatrixBoolean at(Index row, const Set& columnSet) const;

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
            MatrixBoolean at(Real row, const Set& columnSet) const;

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
            MatrixBoolean at(const Complex& row, const Set& columnSet) const;

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
            MatrixBoolean at(const Range& rowRange, const Set& columnSet) const;

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
            MatrixBoolean at(const Tuple& rowTuple, const Set& columnSet) const;

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
            MatrixBoolean at(const MatrixInteger& rowMatrix, const Set& columnSet) const;

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
            MatrixBoolean at(const MatrixReal& rowMatrix, const Set& columnSet) const;

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
            MatrixBoolean at(const MatrixComplex& rowMatrix, const Set& columnSet) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] indexSet  The one based index number set.
             *
             * \return Returns the value at the specified location.
             */
            MatrixBoolean at(const Set& indexSet) const;

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
            MatrixBoolean at(const Tuple& rowTuple, Index column) const;

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
            MatrixBoolean at(const Tuple& rowTuple, Real column) const;

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
            MatrixBoolean at(const Tuple& rowTuple, const Complex& column) const;

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
            MatrixBoolean at(const Tuple& rowTuple, const Range& columnRange) const;

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
            MatrixBoolean at(const Tuple& rowTuple, const Tuple& columnTuple) const;

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
            MatrixBoolean at(const Tuple& rowTuple, const MatrixInteger& columnMatrix) const;

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
            MatrixBoolean at(const Tuple& rowTuple, const MatrixReal& columnMatrix) const;

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
            MatrixBoolean at(const Tuple& rowTuple, const MatrixComplex& columnMatrix) const;

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
            MatrixBoolean at(Index row, const Tuple& columnTuple) const;

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
            MatrixBoolean at(Real row, const Tuple& columnTuple) const;

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
            MatrixBoolean at(const Complex& row, const Tuple& columnTuple) const;

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
            MatrixBoolean at(const Range& rowRange, const Tuple& columnTuple) const;

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
            MatrixBoolean at(const MatrixInteger& rowMatrix, const Tuple& columnTuple) const;

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
            MatrixBoolean at(const MatrixReal& rowMatrix, const Tuple& columnTuple) const;

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
            MatrixBoolean at(const MatrixComplex& rowMatrix, const Tuple& columnTuple) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] indexTuple  The one based index number tuple.
             *
             * \return Returns the value at the specified location.
             */
            MatrixBoolean at(const Tuple& indexTuple) const;

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
            MatrixBoolean at(const MatrixInteger& rowMatrix, Index column) const;

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
            MatrixBoolean at(const MatrixInteger& rowMatrix, Real column) const;

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
            MatrixBoolean at(const MatrixInteger& rowMatrix, const Complex& column) const;

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
            MatrixBoolean at(const MatrixInteger& rowMatrix, const Range& columnRange) const;

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
            MatrixBoolean at(const MatrixInteger& rowMatrix, const MatrixInteger& columnMatrix) const;

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
            MatrixBoolean at(const MatrixInteger& rowMatrix, const MatrixReal& columnMatrix) const;

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
            MatrixBoolean at(const MatrixInteger& rowMatrix, const MatrixComplex& columnMatrix) const;

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
            MatrixBoolean at(Index row, const MatrixInteger& columnMatrix) const;

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
            MatrixBoolean at(Real row, const MatrixInteger& columnMatrix) const;

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
            MatrixBoolean at(const Complex& row, const MatrixInteger& columnMatrix) const;

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
            MatrixBoolean at(const Range& rowRange, const MatrixInteger& columnMatrix) const;

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
            MatrixBoolean at(const MatrixReal& rowMatrix, const MatrixInteger& columnMatrix) const;

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
            MatrixBoolean at(const MatrixComplex& rowMatrix, const MatrixInteger& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] indexMatrix The one based index number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixBoolean at(const MatrixInteger& indexMatrix) const;

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
            MatrixBoolean at(const MatrixReal& rowMatrix, Index column) const;

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
            MatrixBoolean at(const MatrixReal& rowMatrix, Real column) const;

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
            MatrixBoolean at(const MatrixReal& rowMatrix, const Complex& column) const;

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
            MatrixBoolean at(const MatrixReal& rowMatrix, const Range& columnRange) const;

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
            MatrixBoolean at(const MatrixReal& rowMatrix, const MatrixReal& columnMatrix) const;

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
            MatrixBoolean at(const MatrixReal& rowMatrix, const MatrixComplex& columnMatrix) const;

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
            MatrixBoolean at(Index row, const MatrixReal& columnMatrix) const;

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
            MatrixBoolean at(Real row, const MatrixReal& columnMatrix) const;

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
            MatrixBoolean at(const Complex& row, const MatrixReal& columnMatrix) const;

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
            MatrixBoolean at(const Range& rowRange, const MatrixReal& columnMatrix) const;

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
            MatrixBoolean at(const MatrixComplex& rowMatrix, const MatrixReal& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] indexMatrix The one based index number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixBoolean at(const MatrixReal& indexMatrix) const;

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
            MatrixBoolean at(const MatrixComplex& rowMatrix, Index column) const;

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
            MatrixBoolean at(const MatrixComplex& rowMatrix, Real column) const;

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
            MatrixBoolean at(const MatrixComplex& rowMatrix, const Complex& column) const;

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
            MatrixBoolean at(const MatrixComplex& rowMatrix, const Range& columnRange) const;

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
            MatrixBoolean at(const MatrixComplex& rowMatrix, const MatrixComplex& columnMatrix) const;

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
            MatrixBoolean at(Index row, const MatrixComplex& columnMatrix) const;

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
            MatrixBoolean at(Real row, const MatrixComplex& columnMatrix) const;

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
            MatrixBoolean at(const Complex& row, const MatrixComplex& columnMatrix) const;

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
            MatrixBoolean at(const Range& rowRange, const MatrixComplex& columnMatrix) const;

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] indexMatrix The one based index number.
             *
             * \return Returns the value at the specified location.
             */
            MatrixBoolean at(const MatrixComplex& indexMatrix) const;

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
            MatrixBoolean at(const Range& rowRange, Index column) const;

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
            MatrixBoolean at(const Range& rowRange, Real column) const;

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
            MatrixBoolean at(const Range& rowRange, const Complex& column) const;

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
            MatrixBoolean at(const Range& rowRange, const Range& columnRange) const;

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
            MatrixBoolean at(Index row, const Range& columnRange) const;

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
            MatrixBoolean at(Real row, const Range& columnRange) const;

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
            MatrixBoolean at(const Complex& row, const Range& columnRange) const;

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
                if (toInteger(r, row) && toInteger(c, column)) {
                    update(r, c, Boolean(newValue != NV(0)));
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
                if (toInteger(i, index)) {
                    update(i, Boolean(newValue != NV(0)));
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
            inline const MatrixBoolean& combineLeftToRight() const {
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
            MatrixBoolean combineLeftToRight(const MatrixBoolean& other) const;

            /**
             * Method you can use to create a matrix that represents the combination of two other matrices.  The
             * matrices are combined top to bottom.
             *
             * This method exists to simplify templatization of this operation.
             *
             * \return Returns a copy of this matrix.
             */
            inline const MatrixBoolean& combineTopToBottom() const {
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
            MatrixBoolean combineTopToBottom(const MatrixBoolean& other) const;

            /**
             * Method you can use to reverse the columns in this matrix.
             *
             * \return Returns a column-reversed version of this matrix.
             */
            MatrixBoolean columnReverse() const;

            /**
             * Method you can use to reverse the rows in this matrix.
             *
             * \return Returns a row-reversed version of this matrix.
             */
            MatrixBoolean rowReverse() const;

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
            MatrixBoolean transpose() const;

            /**
             * Method that returns the complex conjugate of this matrix.
             *
             * \return Returns the complex conjugate of this value.
             */
            MatrixBoolean conj() const;

            /**
             * Method that returns the adjoint or conjugate transpose of the matrix.
             *
             * \return Returns the adjoint of the matrix.
             */
            MatrixBoolean adjoint() const;

            /**
             * Method you can use to determine the underlying memory layout for the matrix.
             *
             * \return Returns a value indicating the underlying memory layout for the matrix.
             */
            MatrixType matrixType() const override;

            /**
             * Assignment operator
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            MatrixBoolean& operator=(const MatrixBoolean& other);

            /**
             * Assignment operator
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            MatrixBoolean& operator=(const MatrixInteger& other);

            /**
             * Assignment operator
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            MatrixBoolean& operator=(const MatrixReal& other);

            /**
             * Assignment operator
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            MatrixBoolean& operator=(const MatrixComplex& other);

            /**
             * Assignment operator
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            MatrixBoolean& operator=(const Variant& other);

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
             * \return Returns true if the values are equal.  Returns false if the values are not equal.
             */
            bool operator==(const MatrixReal& other) const;

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are equal.  Returns false if the values are not equal.
             */
            bool operator==(const MatrixComplex& other) const;

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are not equal.  Returns false if the values are equal.
             */
            inline bool operator!=(const MatrixBoolean& other) const {
                return !operator==(other);
            }

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
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are not equal.  Returns false if the values are equal.
             */
            inline bool operator!=(const MatrixComplex& other) const {
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
            template<typename RI> M_PUBLIC_TEMPLATE_METHOD MatrixBoolean operator()(
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
            template<typename CI> M_PUBLIC_TEMPLATE_METHOD MatrixBoolean operator()(
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
            template<typename RI, typename CI> M_PUBLIC_TEMPLATE_METHOD MatrixBoolean operator()(
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
            int relativeOrder(const MatrixBoolean& other) const;

        private:
            class Data;
            class SparseData;
            class DenseData;

            /**
             * Constructor
             *
             * \param[in] dataStore The data store backing this matrix.
             */
            MatrixBoolean(Data* dataStore);

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
            void applyTransform() const;

            /**
             * Instance that provides the underlying data store for the matrix.
             */
            mutable Data* currentData;

            /**
             * True if there is a pending transpose on the matrix.
             */
            mutable bool pendingTranspose;
    };
}

#endif
