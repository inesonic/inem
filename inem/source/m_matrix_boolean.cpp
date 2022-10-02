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
* This file implements the \ref M::MatrixBoolean class.
***********************************************************************************************************************/

#include <algorithm>
#include <cassert>

#include "m_api_types.h"
#include "model_exceptions.h"
#include "m_intrinsic_types.h"
#include "m_range.h"
#include "m_set.h"
#include "m_set_iterator.h"
#include "m_tuple.h"
#include "m_variant.h"
#include "model_variant.h"
#include "model_tuple.h"
#include "model_matrix.h"
#include "m_basic_functions.h"
#include "m_matrix_helpers.h"
#include "m_matrix_integer.h"
#include "m_matrix_integer_data.h"
#include "m_matrix_integer_dense_data.h"
#include "m_matrix_integer_sparse_data.h"
#include "m_matrix_real.h"
#include "m_matrix_real_data.h"
#include "m_matrix_real_dense_data.h"
#include "m_matrix_real_sparse_data.h"
#include "m_matrix_complex.h"
#include "m_matrix_complex_data.h"
#include "m_matrix_complex_dense_data.h"
#include "m_matrix_complex_sparse_data.h"
#include "m_matrix_boolean.h"
#include "m_matrix_boolean_data.h"
#include "m_matrix_boolean_dense_data.h"
#include "m_matrix_boolean_sparse_data.h"

/***********************************************************************************************************************
 * M::MatrixBoolean
 */

namespace M {
    MatrixBoolean::Iterator::Iterator() {
        currentMatrix = nullptr;
        currentValue  = false;
        currentRow    = 0;
        currentColumn = 0;
    }


    MatrixBoolean::Iterator::Iterator(const MatrixBoolean* matrix, unsigned long row, unsigned long column) {
        currentMatrix = matrix;
        currentRow    = row;
        currentColumn = column;

        if (static_cast<M::Integer>(row) < matrix->numberRows()       &&
            static_cast<M::Integer>(column) < matrix->numberColumns()    ) {
            currentValue = matrix->at(
                static_cast<M::Integer>(row),
                static_cast<M::Integer>(column)
            );
        } else {
            currentValue = false;
        }
    }


    MatrixBoolean::Iterator::Iterator(const Iterator& other) {
        currentMatrix = other.currentMatrix;
        currentValue  = other.currentValue;
        currentRow    = other.currentRow;
        currentColumn = other.currentColumn;
    }


    MatrixBoolean::Iterator::~Iterator() {}


    void MatrixBoolean::Iterator::advance() {
        unsigned long numberColumns = currentMatrix->numberColumns();
        ++currentColumn;
        if (currentColumn > numberColumns) {
            unsigned long numberRows = currentMatrix->numberRows();
            if (currentRow < numberRows) {
                ++currentRow;
                currentColumn = 1;

                currentValue = currentMatrix->at(
                    static_cast<M::Integer>(currentRow),
                    static_cast<M::Integer>(currentColumn)
                );
            } else {
                currentRow    = numberRows;
                currentColumn = numberColumns + 1;
                currentValue  = false;
            }
        } else {
            currentValue = currentMatrix->at(
                static_cast<M::Integer>(currentRow),
                static_cast<M::Integer>(currentColumn)
            );
        }
    }


    void MatrixBoolean::Iterator::advance(unsigned long long distance) {
        unsigned long      numberColumns      = currentMatrix->numberColumns();
        unsigned long      numberRows         = currentMatrix->numberRows();
        unsigned long long numberCoefficients = numberRows * numberColumns;
        unsigned long long currentIndex       = (currentRow - 1) * numberColumns + currentColumn - 1;
        unsigned long long newIndex           = currentIndex + distance;

        if (newIndex < numberCoefficients) {
            currentRow    = 1 + (newIndex / numberColumns);
            currentColumn = 1 + (newIndex % numberColumns);

            currentValue = currentMatrix->at(
                static_cast<M::Integer>(currentRow),
                static_cast<M::Integer>(currentColumn)
            );
        } else {
            currentRow    = numberRows;
            currentColumn = numberColumns + 1;
            currentValue  = false;
        }
    }


    bool MatrixBoolean::Iterator::isValid() const {
        return (
               currentMatrix != nullptr
            && currentRow <= static_cast<unsigned long>(currentMatrix->numberRows())
            && currentColumn <= static_cast<unsigned long>(currentMatrix->numberColumns())
        );
    }


    MatrixBoolean::Iterator& MatrixBoolean::Iterator::operator=(const MatrixBoolean::Iterator& other) {
        currentMatrix = other.currentMatrix;
        currentValue  = other.currentValue;
        currentRow    = other.currentRow;
        currentColumn = other.currentColumn;

        return *this;
    }
}

/***********************************************************************************************************************
 * M::MatrixBoolean::Iterator
 */

namespace M {
    MatrixBoolean::MatrixBoolean(Integer newNumberRows, Integer newNumberColumns) {
        if (newNumberRows < 0) {
            throw Model::InvalidRow(newNumberRows, 0);
        } else if (newNumberColumns < 0) {
            throw Model::InvalidColumn(newNumberColumns, 0);
        } else {
            currentData      = DenseData::create(newNumberRows, newNumberColumns);
            pendingTranspose = false;
        }
    }


    MatrixBoolean::MatrixBoolean(
            Integer                      newNumberRows,
            Integer                      newNumberColumns,
            const MatrixBoolean::Scalar* matrixData
        ) {
        if (newNumberRows < 0) {
            throw Model::InvalidRow(newNumberRows, 0);
        } else if (newNumberColumns < 0) {
            throw Model::InvalidColumn(newNumberColumns, 0);
        } else {
            currentData      = DenseData::create(newNumberRows, newNumberColumns, matrixData);
            pendingTranspose = false;
        }
    }


    MatrixBoolean::MatrixBoolean(const MatrixBoolean& other) {
        other.currentData->addReference();

        currentData      = other.currentData;
        pendingTranspose = other.pendingTranspose;
    }


    MatrixBoolean::MatrixBoolean(const MatrixInteger& other) {
        if (other.currentData->matrixType() == MatrixType::DENSE) {
            other.applyLazyTransformsAndScaling();

            currentData      = DenseData::create(*POLYMORPHIC_CAST<MatrixInteger::DenseData*>(other.currentData));
            pendingTranspose = false;
        } else {
            assert(other.currentData->matrixType() == MatrixType::SPARSE);
            assert(false); // FIXME
        }
    }


    MatrixBoolean::MatrixBoolean(const MatrixReal& other) {
        if (other.currentData->matrixType() == MatrixType::DENSE) {
            other.applyLazyTransformsAndScaling();

            currentData      = DenseData::create(*POLYMORPHIC_CAST<MatrixReal::DenseData*>(other.currentData));
            pendingTranspose = false;
        } else {
            assert(other.currentData->matrixType() == MatrixType::SPARSE);
            assert(false); // FIXME
        }
    }


    MatrixBoolean::MatrixBoolean(const MatrixComplex& other) {
        if (other.currentData->matrixType() == MatrixType::DENSE) {
            other.applyLazyTransformsAndScaling();

            currentData      = DenseData::create(*POLYMORPHIC_CAST<MatrixComplex::DenseData*>(other.currentData));
            pendingTranspose = false;
        } else {
            assert(other.currentData->matrixType() == MatrixType::SPARSE);
            assert(false); // FIXME
        }
    }


    MatrixBoolean::MatrixBoolean(const Variant& other) {
        bool          ok;
        MatrixBoolean v = other.toMatrixBoolean(&ok);

        if (ok) {
            v.currentData->addReference();
            currentData = v.currentData;

            pendingTranspose = v.pendingTranspose;
        } else {
            throw Model::InvalidRuntimeConversion(other.valueType(), ValueType::MATRIX_BOOLEAN);
        }
    }


    MatrixBoolean::~MatrixBoolean() {
        releaseDataStore(currentData);
    }


    ValueType MatrixBoolean::coefficientValueType() const {
        return ValueType::INTEGER;
    }


    bool MatrixBoolean::toFile(const char* filename, Model::DataFileFormat fileFormat) const {
        return currentData->toFile(filename, fileFormat);
    }


    MatrixBoolean MatrixBoolean::fromFile(const char* filename) {
        return MatrixBoolean(DenseData::fromFile(filename));
    }


    MatrixBoolean::Index MatrixBoolean::numberRows() const {
        return pendingTranspose ? currentData->numberColumns() : currentData->numberRows();
    }


    MatrixBoolean::Index MatrixBoolean::numberColumns() const {
        return pendingTranspose ? currentData->numberRows() : currentData->numberColumns();
    }


    MatrixBoolean::Index MatrixBoolean::numberCoefficients() const {
        return currentData->numberRows() * currentData->numberColumns();
    }


    MatrixBoolean MatrixBoolean::identity(Index numberRows, Index numberColumns) {
        return MatrixBoolean(DenseData::identity(numberRows, numberColumns));
    }


    MatrixBoolean MatrixBoolean::identity(Index numberRowColumns) {
        return MatrixBoolean(DenseData::identity(numberRowColumns, numberRowColumns));
    }


    MatrixBoolean MatrixBoolean::ones(Index numberRows, Index numberColumns) {
        return MatrixBoolean(DenseData::ones(numberRows, numberColumns));
    }


    MatrixBoolean MatrixBoolean::ones(Index numberRowColumns) {
        return MatrixBoolean(DenseData::ones(numberRowColumns, numberRowColumns));
    }


    MatrixBoolean MatrixBoolean::diagonalEntries() const {
        unsigned long numberDiagnonalEntries = std::min(currentData->numberRows(), currentData->numberColumns());
        MatrixBoolean result(numberDiagnonalEntries, 1);

        for (unsigned index=0 ; index<numberDiagnonalEntries ; ++index) {
            Scalar v = currentData->at(index, index);
            result.currentData->update(index, 0, v);
        }

        return result;
    }


    MatrixBoolean MatrixBoolean::diagonal() const {
        unsigned long numberRows    = currentData->numberRows();
        unsigned long numberColumns = currentData->numberColumns();
        DenseData*    result        = nullptr;

        if ((numberRows != 1 && numberColumns != 1) || numberRows == 0 || numberColumns == 0) {
            throw Model::InvalidMatrixDimensions(numberRows, numberColumns);
        } else {
            if (numberRows > 1) {
                result = DenseData::create(numberRows, numberRows);
                for (unsigned i=0 ; i<numberRows ; ++i) {
                    Scalar  v = currentData->at(i, 0);
                    result->update(i, i, v);
                }
            } else {
                result = DenseData::create(numberColumns, numberColumns);
                for (unsigned i=0 ; i<numberColumns ; ++i) {
                    Scalar  v = currentData->at(0, i);
                    result->update(i, i, v);
                }
            }
        }

        return MatrixBoolean(result);
    }


    Model::Variant MatrixBoolean::value(MatrixBoolean::Index row, MatrixBoolean::Index column) const {
        Model::Variant result;

        if (row > 0 && static_cast<unsigned long>(row) <= currentData->numberRows()          &&
            column > 0 && static_cast<unsigned long>(column) <= currentData->numberColumns()    ) {
            result = currentData->at(row - 1, column - 1);
        }

        return result;
    }


    Model::Variant MatrixBoolean::value(Matrix::Index index) const {
        Model::Variant result;

        unsigned long      numberRows         = currentData->numberRows();
        unsigned long      numberColumns      = currentData->numberColumns();
        unsigned long long numberCoefficients = numberRows * numberColumns;

        if (index > 0 || static_cast<unsigned long long>(index) <= numberCoefficients) {
            unsigned long long i           = static_cast<unsigned long long>(index - 1);
            unsigned long      rowIndex    = i / numberColumns;
            unsigned long      columnIndex = i % numberColumns;

            result = currentData->at(rowIndex, columnIndex);
        }

        return result;
    }


    bool MatrixBoolean::setValue(MatrixBoolean::Index row, MatrixBoolean::Index column, const Model::Variant& value) {
        bool success;

        if (row > 0 && column > 0) {
            Boolean realValue = value.toBoolean(&success);

            if (success) {
                update(row, column, realValue);
            }
        } else {
            success = false;
        }

        return success;
    }


    bool MatrixBoolean::setValue(MatrixBoolean::Index index, const Model::Variant& value) {
        bool success;

        if (index > 0) {
            Boolean realValue = value.toBoolean(&success);

            if (success) {
                update(index, realValue);
            }
        } else {
            success = false;
        }

        return success;
    }


    void MatrixBoolean::resize(MatrixBoolean::Index newNumberRows, MatrixBoolean::Index newNumberColumns) {
        applyTransform();

        currentData->lock();
        if (currentData->referenceCount() > 1) {
            Data* newData = currentData->resizeTo(newNumberRows, newNumberColumns, true);
            assignNewDataStore(newData, true);
        } else {
            Data* newDataStore = currentData->resizeTo(newNumberRows, newNumberColumns, false);
            if (newDataStore != currentData) {
                assignNewDataStore(newDataStore, true);
            } else {
                currentData->unlock();
            }
        }
    }


    MatrixBoolean::Scalar MatrixBoolean::at(
            MatrixBoolean::Index row,
            MatrixBoolean::Index column
        ) const {
        applyTransform();

        Scalar result = Scalar(0);

        unsigned long numberRows = currentData->numberRows();
        if (row < 1 || static_cast<unsigned long>(row) > numberRows) {
            throw Model::InvalidRow(row, numberRows);
        } else {
            unsigned long numberColumns = currentData->numberColumns();
            if (column < 1 || static_cast<unsigned long>(column) > numberColumns) {
                throw Model::InvalidColumn(column, numberColumns);
            } else {
                result = currentData->at(row - 1, column - 1);
            }
        }

        return result;
    }


    MatrixBoolean::Scalar MatrixBoolean::at(MatrixBoolean::Index index) const {
        applyTransform();

        Scalar result = false;

        unsigned long      numberRows         = currentData->numberRows();
        unsigned long      numberColumns      = currentData->numberColumns();
        unsigned long long numberCoefficients = numberRows * numberColumns;

        if (index < 1 && static_cast<unsigned long long>(index) > numberCoefficients) {
            throw Model::InvalidIndex(index, numberCoefficients);
        } else {
            unsigned long long i           = static_cast<unsigned long long>(index - 1);
            unsigned long      rowIndex    = i / numberColumns;
            unsigned long      columnIndex = i % numberColumns;

            result = currentData->at(rowIndex, columnIndex);
        }

        return result;
    }


















    MatrixBoolean::Scalar MatrixBoolean::at(Real row, MatrixBoolean::Index column) const {
        Index r;
        return toInteger(r, row) ? at(r, column) : Scalar(0);
    }


    MatrixBoolean::Scalar MatrixBoolean::at(const Complex& row, MatrixBoolean::Index column) const {
        Index r;
        return toInteger(r, row) ? at(r, column) : Scalar(0);
    }


    MatrixBoolean::Scalar MatrixBoolean::at(MatrixBoolean::Index row, Real column) const {
        Index c;
        return toInteger(c, column) ? at(row, c) : Scalar(0);
    }


    MatrixBoolean::Scalar MatrixBoolean::at(MatrixBoolean::Index row, const Complex& column) const {
        Index c;
        return toInteger(c, column) ? at(row, c) : Scalar(0);
    }


    MatrixBoolean::Scalar MatrixBoolean::at(Real row, Real column) const {
        Index r;
        Index c;
        return (toInteger(r, row) && toInteger(c, column)) ? at(r, c) : Scalar(0);
    }


    MatrixBoolean::Scalar MatrixBoolean::at(const Complex& row, Real column) const {
        Index r;
        Index c;
        return (toInteger(r, row) && toInteger(c, column)) ? at(r, c) : Scalar(0);
    }


    MatrixBoolean::Scalar MatrixBoolean::at(Real row, const Complex& column) const {
        Index r;
        Index c;
        return (toInteger(r, row) && toInteger(c, column)) ? at(r, c) : Scalar(0);
    }


    MatrixBoolean::Scalar MatrixBoolean::at(const Complex& row, const Complex& column) const {
        Index r;
        Index c;
        return (toInteger(r, row) && toInteger(c, column)) ? at(r, c) : Scalar(0);
    }


    MatrixBoolean::Scalar MatrixBoolean::at(Real index) const {
        Index i;
        return toInteger(i, index) ? at(i) : Scalar(0);
    }


    MatrixBoolean::Scalar MatrixBoolean::at(const Complex& index) const {
        Index i;
        return toInteger(i, index) ? at(i) : Scalar(0);
    }


    MatrixBoolean MatrixBoolean::at(const Set& rowSet, MatrixBoolean::Index column) const {
        return atICHelper(*this, rowSet, column);
    }


    MatrixBoolean MatrixBoolean::at(const Set& rowSet, Real column) const {
        return at(rowSet, inlineToInteger(column));
    }


    MatrixBoolean MatrixBoolean::at(const Set& rowSet, const Complex& column) const {
        return at(rowSet, inlineToInteger(column));
    }


    MatrixBoolean MatrixBoolean::at(const Set& rowSet, const Range& column) const {
        return atIIHelper(*this, rowSet, column);
    }


    MatrixBoolean MatrixBoolean::at(const Set& rowSet, const Set& columnSet) const {
        return atIIHelper(*this, rowSet, columnSet);
    }


    MatrixBoolean MatrixBoolean::at(const Set& rowSet, const Tuple& columnTuple) const {
        return atIIHelper(*this, rowSet, columnTuple);
    }


    MatrixBoolean MatrixBoolean::at(const Set& rowSet, const MatrixInteger& columnMatrix) const {
        return atIMHelper(*this, rowSet, columnMatrix);
    }


    MatrixBoolean MatrixBoolean::at(const Set& rowSet, const MatrixReal& columnMatrix) const {
        return atIMHelper(*this, rowSet, columnMatrix);
    }


    MatrixBoolean MatrixBoolean::at(const Set& rowSet, const MatrixComplex& columnMatrix) const {
        return atIMHelper(*this, rowSet, columnMatrix);
    }


    MatrixBoolean MatrixBoolean::at(MatrixBoolean::Index row, const Set& columnSet) const {
        return atCIHelper(*this, row, columnSet);
    }


    MatrixBoolean MatrixBoolean::at(Real row, const Set& columnSet) const {
        return at(inlineToInteger(row), columnSet);
    }


    MatrixBoolean MatrixBoolean::at(const Complex& row, const Set& columnSet) const {
        return at(inlineToInteger(row), columnSet);
    }


    MatrixBoolean MatrixBoolean::at(const Range& rowRange, const Set& columnSet) const {
        return atIIHelper(*this, rowRange, columnSet);
    }


    MatrixBoolean MatrixBoolean::at(const Tuple& rowTuple, const Set& columnSet) const {
        return atIIHelper(*this, rowTuple, columnSet);
    }


    MatrixBoolean MatrixBoolean::at(const MatrixInteger& rowMatrix, const Set& columnSet) const {
        return atMIHelper(*this, rowMatrix, columnSet);
    }


    MatrixBoolean MatrixBoolean::at(const MatrixReal& rowMatrix, const Set& columnSet) const {
        return atMIHelper(*this, rowMatrix, columnSet);
    }


    MatrixBoolean MatrixBoolean::at(const MatrixComplex& rowMatrix, const Set& columnSet) const {
        return atMIHelper(*this, rowMatrix, columnSet);
    }


    MatrixBoolean MatrixBoolean::at(const Set& indexSet) const {
        return atIHelper(*this, indexSet);
    }


    MatrixBoolean MatrixBoolean::at(const Tuple& rowTuple, MatrixBoolean::Index column) const {
        return atICHelper(*this, rowTuple, column);
    }


    MatrixBoolean MatrixBoolean::at(const Tuple& rowTuple, Real column) const {
        return at(rowTuple, inlineToInteger(column));
    }


    MatrixBoolean MatrixBoolean::at(const Tuple& rowTuple, const Complex& column) const {
        return at(rowTuple, inlineToInteger(column));
    }


    MatrixBoolean MatrixBoolean::at(const Tuple& rowTuple, const Range& columnRange) const {
        return atIIHelper(*this, rowTuple, columnRange);
    }


    MatrixBoolean MatrixBoolean::at(const Tuple& rowTuple, const Tuple& columnTuple) const {
        return atIIHelper(*this, rowTuple, columnTuple);
    }


    MatrixBoolean MatrixBoolean::at(const Tuple& rowTuple, const MatrixInteger& columnMatrix) const {
        return atIMHelper(*this, rowTuple, columnMatrix);
    }


    MatrixBoolean MatrixBoolean::at(const Tuple& rowTuple, const MatrixReal& columnMatrix) const {
        return atIMHelper(*this, rowTuple, columnMatrix);
    }


    MatrixBoolean MatrixBoolean::at(const Tuple& rowTuple, const MatrixComplex& columnMatrix) const {
        return atIMHelper(*this, rowTuple, columnMatrix);
    }


    MatrixBoolean MatrixBoolean::at(MatrixBoolean::Index row, const Tuple& columnTuple) const {
        return atCIHelper(*this, row, columnTuple);
    }


    MatrixBoolean MatrixBoolean::at(Real row, const Tuple& columnTuple) const {
        return at(inlineToInteger(row), columnTuple);
    }


    MatrixBoolean MatrixBoolean::at(const Complex& row, const Tuple& columnTuple) const {
        return at(inlineToInteger(row), columnTuple);
    }


    MatrixBoolean MatrixBoolean::at(const Range& rowRange, const Tuple& columnTuple) const {
        return atIIHelper(*this, rowRange, columnTuple);
    }


    MatrixBoolean MatrixBoolean::at(const MatrixInteger& rowMatrix, const Tuple& columnTuple) const {
        return atMIHelper(*this, rowMatrix, columnTuple);
    }


    MatrixBoolean MatrixBoolean::at(const MatrixReal& rowMatrix, const Tuple& columnTuple) const {
        return atMIHelper(*this, rowMatrix, columnTuple);
    }


    MatrixBoolean MatrixBoolean::at(const MatrixComplex& rowMatrix, const Tuple& columnTuple) const {
        return atMIHelper(*this, rowMatrix, columnTuple);
    }


    MatrixBoolean MatrixBoolean::at(const Tuple& indexTuple) const {
        return atIHelper(*this, indexTuple);
    }


    MatrixBoolean MatrixBoolean::at(const MatrixInteger& rowMatrix, MatrixBoolean::Index column) const {
        return atMCHelper(*this, rowMatrix, column);
    }


    MatrixBoolean MatrixBoolean::at(const MatrixInteger& rowMatrix, Real column) const {
        return at(rowMatrix, inlineToInteger(column));
    }


    MatrixBoolean MatrixBoolean::at(const MatrixInteger& rowMatrix, const Complex& column) const {
        return at(rowMatrix, inlineToInteger(column));
    }


    MatrixBoolean MatrixBoolean::at(const MatrixInteger& rowMatrix, const Range& columnRange) const {
        return atMIHelper(*this, rowMatrix, columnRange);
    }


    MatrixBoolean MatrixBoolean::at(const MatrixInteger& rowMatrix, const MatrixInteger& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixBoolean MatrixBoolean::at(const MatrixInteger& rowMatrix, const MatrixReal& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixBoolean MatrixBoolean::at(const MatrixInteger& rowMatrix, const MatrixComplex& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixBoolean MatrixBoolean::at(MatrixBoolean::Index row, const MatrixInteger& columnMatrix) const {
        return atCMHelper(*this, row, columnMatrix);
    }


    MatrixBoolean MatrixBoolean::at(Real row, const MatrixInteger& columnMatrix) const {
        return at(inlineToInteger(row), columnMatrix);
    }


    MatrixBoolean MatrixBoolean::at(const Complex& row, const MatrixInteger& columnMatrix) const {
        return at(inlineToInteger(row), columnMatrix);
    }


    MatrixBoolean MatrixBoolean::at(const Range& rowRange, const MatrixInteger& columnMatrix) const {
        return atIMHelper(*this, rowRange, columnMatrix);
    }


    MatrixBoolean MatrixBoolean::at(const MatrixReal& rowMatrix, const MatrixInteger& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixBoolean MatrixBoolean::at(const MatrixComplex& rowMatrix, const MatrixInteger& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixBoolean MatrixBoolean::at(const MatrixInteger& indexMatrix) const {
        return atMHelper(*this, indexMatrix);
    }


    MatrixBoolean MatrixBoolean::at(const MatrixReal& rowMatrix, MatrixBoolean::Index column) const {
        return atMCHelper(*this, rowMatrix, column);
    }


    MatrixBoolean MatrixBoolean::at(const MatrixReal& rowMatrix, Real column) const {
        return at(rowMatrix, inlineToInteger(column));
    }


    MatrixBoolean MatrixBoolean::at(const MatrixReal& rowMatrix, const Complex& column) const {
        return at(rowMatrix, inlineToInteger(column));
    }


    MatrixBoolean MatrixBoolean::at(const MatrixReal& rowMatrix, const Range& columnRange) const {
        return atMIHelper(*this, rowMatrix, columnRange);
    }


    MatrixBoolean MatrixBoolean::at(const MatrixReal& rowMatrix, const MatrixReal& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixBoolean MatrixBoolean::at(const MatrixReal& rowMatrix, const MatrixComplex& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixBoolean MatrixBoolean::at(MatrixBoolean::Index row, const MatrixReal& columnMatrix) const {
        return atCMHelper(*this, row, columnMatrix);
    }


    MatrixBoolean MatrixBoolean::at(Real row, const MatrixReal& columnMatrix) const {
        return at(inlineToInteger(row), columnMatrix);
    }


    MatrixBoolean MatrixBoolean::at(const Complex& row, const MatrixReal& columnMatrix) const {
        return at(inlineToInteger(row), columnMatrix);
    }


    MatrixBoolean MatrixBoolean::at(const Range& rowRange, const MatrixReal& columnMatrix) const {
        return atIMHelper(*this, rowRange, columnMatrix);
    }


    MatrixBoolean MatrixBoolean::at(const MatrixComplex& rowMatrix, const MatrixReal& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixBoolean MatrixBoolean::at(const MatrixReal& indexMatrix) const {
        return atMHelper(*this, indexMatrix);
    }


    MatrixBoolean MatrixBoolean::at(const MatrixComplex& rowMatrix, MatrixBoolean::Index column) const {
        return atMCHelper(*this, rowMatrix, column);
    }


    MatrixBoolean MatrixBoolean::at(const MatrixComplex& rowMatrix, Real column) const {
        return at(rowMatrix, inlineToInteger(column));
    }


    MatrixBoolean MatrixBoolean::at(const MatrixComplex& rowMatrix, const Complex& column) const {
        return at(rowMatrix, inlineToInteger(column));
    }


    MatrixBoolean MatrixBoolean::at(const MatrixComplex& rowMatrix, const Range& columnRange) const {
        return atMIHelper(*this, rowMatrix, columnRange);
    }


    MatrixBoolean MatrixBoolean::at(const MatrixComplex& rowMatrix, const MatrixComplex& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixBoolean MatrixBoolean::at(MatrixBoolean::Index row, const MatrixComplex& columnMatrix) const {
        return atCMHelper(*this, row, columnMatrix);
    }


    MatrixBoolean MatrixBoolean::at(Real row, const MatrixComplex& columnMatrix) const {
        return at(inlineToInteger(row), columnMatrix);
    }


    MatrixBoolean MatrixBoolean::at(const Complex& row, const MatrixComplex& columnMatrix) const {
        return at(inlineToInteger(row), columnMatrix);
    }


    MatrixBoolean MatrixBoolean::at(const Range& rowRange, const MatrixComplex& columnMatrix) const {
        return atIMHelper(*this, rowRange, columnMatrix);
    }


    MatrixBoolean MatrixBoolean::at(const MatrixComplex& indexMatrix) const {
        return atMHelper(*this, indexMatrix);
    }


    MatrixBoolean MatrixBoolean::at(const Range& rowRange, MatrixBoolean::Index column) const {
        return atICHelper(*this, rowRange, column);
    }


    MatrixBoolean MatrixBoolean::at(const Range& rowRange, Real column) const {
        return at(rowRange, inlineToInteger(column));
    }


    MatrixBoolean MatrixBoolean::at(const Range& rowRange, const Complex& column) const {
        return at(rowRange, inlineToInteger(column));
    }


    MatrixBoolean MatrixBoolean::at(const Range& rowRange, const Range& columnRange) const {
        return atIIHelper(*this, rowRange, columnRange);
    }


    MatrixBoolean MatrixBoolean::at(MatrixBoolean::Index row, const Range& columnRange) const {
        return atCIHelper(*this, row, columnRange);
    }


    MatrixBoolean MatrixBoolean::at(Real row, const Range& columnRange) const {
        return at(inlineToInteger(row), columnRange);
    }


    MatrixBoolean MatrixBoolean::at(const Complex& row, const Range& columnRange) const {
        return at(inlineToInteger(row), columnRange);
    }


    Variant MatrixBoolean::at(const Variant& row, MatrixBoolean::Index column) const {
        ValueType rowType = row.valueType();
        switch (rowType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::MATRIX_BOOLEAN: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return Variant(at(row.toInteger(), column));
            }

            case ValueType::SET: {
                return Variant(at(row.toSet(), column));
            }

            case ValueType::TUPLE: {
                return Variant(at(row.toTuple(), column));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(at(row.toMatrixInteger(), column));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(at(row.toMatrixReal(), column));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(at(row.toMatrixComplex(), column));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant MatrixBoolean::at(const Variant& row, Real column) const {
        return at(row, inlineToInteger(column));
    }


    Variant MatrixBoolean::at(const Variant& row, const Complex& column) const {
        return at(row, inlineToInteger(column));
    }


    Variant MatrixBoolean::at(const Variant& row, const Range& columnRange) const {
        ValueType rowType = row.valueType();
        switch (rowType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::MATRIX_BOOLEAN: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return Variant(at(row.toInteger(), columnRange));
            }

            case ValueType::SET: {
                return Variant(at(row.toSet(), columnRange));
            }

            case ValueType::TUPLE: {
                return Variant(at(row.toTuple(), columnRange));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(at(row.toMatrixInteger(), columnRange));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(at(row.toMatrixReal(), columnRange));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(at(row.toMatrixComplex(), columnRange));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant MatrixBoolean::at(const Variant& row, const Set& columnSet) const {
        ValueType rowType = row.valueType();
        switch (rowType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::MATRIX_BOOLEAN: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return Variant(at(row.toInteger(), columnSet));
            }

            case ValueType::SET: {
                return Variant(at(row.toSet(), columnSet));
            }

            case ValueType::TUPLE: {
                return Variant(at(row.toTuple(), columnSet));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(at(row.toMatrixInteger(), columnSet));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(at(row.toMatrixReal(), columnSet));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(at(row.toMatrixComplex(), columnSet));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant MatrixBoolean::at(const Variant& row, const Tuple& columnTuple) const {
        ValueType rowType = row.valueType();
        switch (rowType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::MATRIX_BOOLEAN: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return Variant(at(row.toInteger(), columnTuple));
            }

            case ValueType::SET: {
                return Variant(at(row.toSet(), columnTuple));
            }

            case ValueType::TUPLE: {
                return Variant(at(row.toTuple(), columnTuple));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(at(row.toMatrixInteger(), columnTuple));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(at(row.toMatrixReal(), columnTuple));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(at(row.toMatrixComplex(), columnTuple));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant MatrixBoolean::at(const Variant& row, const MatrixInteger& columnMatrix) const {
        ValueType rowType = row.valueType();
        switch (rowType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::MATRIX_BOOLEAN: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return Variant(at(row.toInteger(), columnMatrix));
            }

            case ValueType::SET: {
                return Variant(at(row.toSet(), columnMatrix));
            }

            case ValueType::TUPLE: {
                return Variant(at(row.toTuple(), columnMatrix));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(at(row.toMatrixInteger(), columnMatrix));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(at(row.toMatrixReal(), columnMatrix));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(at(row.toMatrixComplex(), columnMatrix));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant MatrixBoolean::at(const Variant& row, const MatrixReal& columnMatrix) const {
        ValueType rowType = row.valueType();
        switch (rowType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::MATRIX_BOOLEAN: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return Variant(at(row.toInteger(), columnMatrix));
            }

            case ValueType::SET: {
                return Variant(at(row.toSet(), columnMatrix));
            }

            case ValueType::TUPLE: {
                return Variant(at(row.toTuple(), columnMatrix));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(at(row.toMatrixInteger(), columnMatrix));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(at(row.toMatrixReal(), columnMatrix));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(at(row.toMatrixComplex(), columnMatrix));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant MatrixBoolean::at(const Variant& row, const MatrixComplex& columnMatrix) const {
        ValueType rowType = row.valueType();
        switch (rowType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::MATRIX_BOOLEAN: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return Variant(at(row.toInteger(), columnMatrix));
            }

            case ValueType::SET: {
                return Variant(at(row.toSet(), columnMatrix));
            }

            case ValueType::TUPLE: {
                return Variant(at(row.toTuple(), columnMatrix));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(at(row.toMatrixInteger(), columnMatrix));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(at(row.toMatrixReal(), columnMatrix));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(at(row.toMatrixComplex(), columnMatrix));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant MatrixBoolean::at(MatrixBoolean::Index row, const Variant& column) const {
        ValueType columnType = column.valueType();
        switch (columnType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::MATRIX_BOOLEAN: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return Variant(at(row, column.toInteger()));
            }

            case ValueType::SET: {
                return Variant(at(row, column.toSet()));
            }

            case ValueType::TUPLE: {
                return Variant(at(row, column.toTuple()));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(at(row, column.toMatrixInteger()));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(at(row, column.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(at(row, column.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant MatrixBoolean::at(Real row, const Variant& column) const {
        return at(inlineToInteger(row), column);
    }


    Variant MatrixBoolean::at(const Complex& row, const Variant& column) const {
        return at(inlineToInteger(row), column);
    }


    Variant MatrixBoolean::at(const Range& rowRange, const Variant& column) const {
        ValueType columnType = column.valueType();
        switch (columnType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::MATRIX_BOOLEAN: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return Variant(at(rowRange, column.toInteger()));
            }

            case ValueType::SET: {
                return Variant(at(rowRange, column.toSet()));
            }

            case ValueType::TUPLE: {
                return Variant(at(rowRange, column.toTuple()));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(at(rowRange, column.toMatrixInteger()));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(at(rowRange, column.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(at(rowRange, column.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant MatrixBoolean::at(const Set& rowSet, const Variant& column) const {
        ValueType columnType = column.valueType();
        switch (columnType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::MATRIX_BOOLEAN: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return Variant(at(rowSet, column.toInteger()));
            }

            case ValueType::SET: {
                return Variant(at(rowSet, column.toSet()));
            }

            case ValueType::TUPLE: {
                return Variant(at(rowSet, column.toTuple()));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(at(rowSet, column.toMatrixInteger()));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(at(rowSet, column.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(at(rowSet, column.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant MatrixBoolean::at(const Tuple& rowTuple, const Variant& column) const {
        ValueType columnType = column.valueType();
        switch (columnType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::MATRIX_BOOLEAN: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return Variant(at(rowTuple, column.toInteger()));
            }

            case ValueType::SET: {
                return Variant(at(rowTuple, column.toSet()));
            }

            case ValueType::TUPLE: {
                return Variant(at(rowTuple, column.toTuple()));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(at(rowTuple, column.toMatrixInteger()));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(at(rowTuple, column.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(at(rowTuple, column.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant MatrixBoolean::at(const MatrixInteger& rowMatrix, const Variant& column) const {
        ValueType columnType = column.valueType();
        switch (columnType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::MATRIX_BOOLEAN: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return Variant(at(rowMatrix, column.toInteger()));
            }

            case ValueType::SET: {
                return Variant(at(rowMatrix, column.toSet()));
            }

            case ValueType::TUPLE: {
                return Variant(at(rowMatrix, column.toTuple()));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(at(rowMatrix, column.toMatrixInteger()));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(at(rowMatrix, column.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(at(rowMatrix, column.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant MatrixBoolean::at(const MatrixReal& rowMatrix, const Variant& column) const {
        ValueType columnType = column.valueType();
        switch (columnType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::MATRIX_BOOLEAN: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return Variant(at(rowMatrix, column.toInteger()));
            }

            case ValueType::SET: {
                return Variant(at(rowMatrix, column.toSet()));
            }

            case ValueType::TUPLE: {
                return Variant(at(rowMatrix, column.toTuple()));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(at(rowMatrix, column.toMatrixInteger()));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(at(rowMatrix, column.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(at(rowMatrix, column.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant MatrixBoolean::at(const MatrixComplex& rowMatrix, const Variant& column) const {
        ValueType columnType = column.valueType();
        switch (columnType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::MATRIX_BOOLEAN: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return Variant(at(rowMatrix, column.toInteger()));
            }

            case ValueType::SET: {
                return Variant(at(rowMatrix, column.toSet()));
            }

            case ValueType::TUPLE: {
                return Variant(at(rowMatrix, column.toTuple()));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(at(rowMatrix, column.toMatrixInteger()));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(at(rowMatrix, column.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(at(rowMatrix, column.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant MatrixBoolean::at(const Variant& row, const Variant& column) const {
        ValueType rowType = row.valueType();
        switch (rowType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::MATRIX_BOOLEAN: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return Variant(at(row.toInteger(), column));
            }

            case ValueType::SET: {
                return Variant(at(row.toSet(), column));
            }

            case ValueType::TUPLE: {
                return Variant(at(row.toTuple(), column));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(at(row.toMatrixInteger(), column));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(at(row.toMatrixReal(), column));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(at(row.toMatrixComplex(), column));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant MatrixBoolean::at(const Variant& index) const {
        ValueType indexType = index.valueType();
        switch (indexType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::MATRIX_BOOLEAN: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return Variant(at(index.toInteger()));
            }

            case ValueType::SET: {
                return Variant(at(index.toSet()));
            }

            case ValueType::TUPLE: {
                return Variant(at(index.toTuple()));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(at(index.toMatrixInteger()));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(at(index.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(at(index.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }














    void MatrixBoolean::update(Matrix::Index row, Matrix::Index column, MatrixBoolean::Scalar newValue) {
        applyTransform();

        if (row < 1) {
            throw Model::InvalidRow(row, currentData->numberRows());
        } else if (column < 1) {
            throw Model::InvalidColumn(column, currentData->numberColumns());
        } else {
            unsigned long numberRows    = currentData->numberRows();
            unsigned long numberColumns = currentData->numberColumns();

            currentData->lock();
            if (currentData->referenceCount() > 1) {
                Data* oldDataStore = currentData;

                if (static_cast<unsigned long>(row) > numberRows       ||
                    static_cast<unsigned long>(column) > numberColumns    ) {
                    unsigned long newNumberRows    = std::max(numberRows, static_cast<unsigned long>(row));
                    unsigned long newNumberColumns = std::max(numberColumns, static_cast<unsigned long>(column));

                    currentData = currentData->resizeTo(newNumberRows, newNumberColumns, true);
                } else {
                    currentData = oldDataStore->clone();
                }

                oldDataStore->unlock();
                releaseDataStore(oldDataStore);

                currentData->update(row - 1, column - 1, newValue);
            } else {
                if (static_cast<unsigned long>(row) > numberRows       ||
                    static_cast<unsigned long>(column) > numberColumns    ) {
                    unsigned long newNumberRows    = std::max(numberRows, static_cast<unsigned long>(row));
                    unsigned long newNumberColumns = std::max(numberColumns, static_cast<unsigned long>(column));

                    Data* newDataStore = currentData->resizeTo(newNumberRows, newNumberColumns);
                    if (newDataStore != currentData) {
                        assignNewDataStore(newDataStore, true);
                        currentData->update(row - 1, column - 1, newValue);
                    } else {
                        currentData->update(row - 1, column - 1, newValue);
                        currentData->unlock();
                    }
                } else {
                    currentData->update(row - 1, column - 1, newValue);
                    currentData->unlock();
                }
            }
        }
    }


    void MatrixBoolean::update(Matrix::Index index, MatrixBoolean::Scalar newValue) {
        unsigned long      numberRows         = currentData->numberRows();
        unsigned long      numberColumns      = currentData->numberColumns();
        unsigned long long numberCoefficients = numberRows * numberColumns;

        if (index < 1) {
            throw Model::InvalidIndex(index, numberCoefficients);
        } else {
            Index rowIndex;
            Index columnIndex;

            unsigned long long i = static_cast<unsigned long long>(index - 1);

            if (static_cast<unsigned long long>(index) > numberCoefficients) {
                if (numberColumns > 1) {
                    if (numberRows == 1) {
                        rowIndex    = 0;
                        columnIndex = index - 1;
                    } else {
                        rowIndex    = i / numberColumns;
                        columnIndex = i % numberColumns;
                    }
                } else {
                    rowIndex    = i;
                    columnIndex = 0;
                }
            } else {
                rowIndex    = i / numberColumns;
                columnIndex = i % numberColumns;
            }

            update(rowIndex + 1, columnIndex + 1, newValue);
        }
    }


    MatrixBoolean MatrixBoolean::combineLeftToRight(const MatrixBoolean& other) const {
        applyTransform();
        other.applyTransform();

        Data* resultData;
        if (other.currentData->matrixType() == Matrix::MatrixType::DENSE) {
            resultData = currentData->combineLeftToRight(*POLYMORPHIC_CAST<const DenseData*>(other.currentData));
        } else {
            assert(other.currentData->matrixType() == Matrix::MatrixType::SPARSE);
            assert(false); // FIXME
            resultData = nullptr;
//            resultData = currentData->combineLeftToRight(*POLYMORPHIC_CAST<const SparseData*>(other.currentData));
        }

        return MatrixBoolean(resultData);
    }


    MatrixBoolean MatrixBoolean::combineTopToBottom(const MatrixBoolean& other) const {
        applyTransform();
        other.applyTransform();

        Data* resultData;
        if (other.currentData->matrixType() == Matrix::MatrixType::DENSE) {
            resultData = currentData->combineTopToBottom(*POLYMORPHIC_CAST<const DenseData*>(other.currentData));
        } else {
            assert(other.currentData->matrixType() == Matrix::MatrixType::SPARSE);
            assert(false); // FIXME
            resultData = nullptr;
//            resultData = currentData->combineTopToBottom(*POLYMORPHIC_CAST<const SparseData*>(other.currentData));
        }

        return MatrixBoolean(resultData);
    }


    MatrixBoolean MatrixBoolean::columnReverse() const {
        applyTransform();
        return MatrixBoolean(currentData->columnReverse());
    }


    MatrixBoolean MatrixBoolean::rowReverse() const {
        applyTransform();
        return MatrixBoolean(currentData->rowReverse());
    }


    bool MatrixBoolean::isSquare() const {
        return currentData->numberRows() == currentData->numberColumns();
    }


    MatrixBoolean MatrixBoolean::transpose() const {
        MatrixBoolean result(*this);
        result.pendingTranspose = !pendingTranspose;
        return result;
    }


    MatrixBoolean MatrixBoolean::conj() const {
        return MatrixBoolean(*this);
    }


    MatrixBoolean MatrixBoolean::adjoint() const {
        return transpose();
    }


    MatrixBoolean::MatrixType MatrixBoolean::matrixType() const {
        return currentData->matrixType();
    }


    MatrixBoolean& MatrixBoolean::operator=(const MatrixBoolean& other) {
        assignExistingDataStore(other.currentData, false);
        pendingTranspose = other.pendingTranspose;

        return *this;
    }


    MatrixBoolean& MatrixBoolean::operator=(const MatrixInteger& other) {
        if (other.currentData->matrixType() == MatrixType::DENSE) {
            other.applyLazyTransformsAndScaling();

            releaseDataStore(currentData);
            currentData = DenseData::create(*POLYMORPHIC_CAST<MatrixInteger::DenseData*>(other.currentData));
            pendingTranspose = false;
        } else {
            assert(other.currentData->matrixType() == MatrixType::SPARSE);
            assert(false); // FIXME
        }

        return *this;
    }


    MatrixBoolean& MatrixBoolean::operator=(const MatrixReal& other) {
        if (other.currentData->matrixType() == MatrixType::DENSE) {
            other.applyLazyTransformsAndScaling();

            releaseDataStore(currentData);
            currentData = DenseData::create(*POLYMORPHIC_CAST<MatrixReal::DenseData*>(other.currentData));
            pendingTranspose = false;
        } else {
            assert(other.currentData->matrixType() == MatrixType::SPARSE);
            assert(false); // FIXME
        }

        return *this;
    }


    MatrixBoolean& MatrixBoolean::operator=(const MatrixComplex& other) {
        if (other.currentData->matrixType() == MatrixType::DENSE) {
            other.applyLazyTransformsAndScaling();

            releaseDataStore(currentData);
            currentData = DenseData::create(*POLYMORPHIC_CAST<MatrixComplex::DenseData*>(other.currentData));
            pendingTranspose = false;
        } else {
            assert(other.currentData->matrixType() == MatrixType::SPARSE);
            assert(false); // FIXME
        }

        return *this;
    }


    MatrixBoolean& MatrixBoolean::operator=(const Variant& other) {
        bool          ok;
        MatrixBoolean v = other.toMatrixBoolean(&ok);

        if (ok) {
            assignExistingDataStore(v.currentData, false);
            pendingTranspose   = v.pendingTranspose;
        } else {
            throw Model::InvalidRuntimeConversion(other.valueType(), ValueType::MATRIX_BOOLEAN);
        }

        return *this;
    }


    bool MatrixBoolean::operator==(const MatrixBoolean& other) const {
        bool result;

        if (other.currentData->matrixType() == Matrix::MatrixType::DENSE) {
            if (currentData == other.currentData && pendingTranspose == other.pendingTranspose) {
                result = true;
            } else {
                if (pendingTranspose != other.pendingTranspose) {
                    applyTransform();
                    other.applyTransform();
                }

                result = currentData->isEqualTo(*POLYMORPHIC_CAST<const DenseData*>(other.currentData));
            }
        } else {
            assert(other.currentData->matrixType() == Matrix::MatrixType::SPARSE);
            assert(false); // FIXME
            result = false;
//            result = currentData->isEqualTo(*POLYMORPHIC_CAST<const SparseData*>(other.currentData));
        }

        return result;
    }


    bool MatrixBoolean::operator==(const MatrixInteger& other) const {
        bool result;

        unsigned long numberRows    = MatrixBoolean::numberRows();
        unsigned long numberColumns = MatrixBoolean::numberColumns();

        if (other.numberRows() == static_cast<M::Integer>(numberRows)       &&
            other.numberColumns() == static_cast<M::Integer>(numberColumns)    ) {
            applyTransform();
            other.applyLazyTransformsAndScaling();

            unsigned long columnIndex = 0;
            result = true;
            while (result && columnIndex<numberColumns) {
                unsigned long rowIndex = 0;
                while (result && rowIndex<numberRows) {
                    bool v = (other.currentData->at(rowIndex, columnIndex) != MatrixInteger::Scalar(0));
                    result = (currentData->at(rowIndex, columnIndex) == v);

                    ++rowIndex;
                }

                ++columnIndex;
            }
        } else {
            result = false;
        }

        return result;
    }


    bool MatrixBoolean::operator==(const MatrixReal& other) const {
        bool result;

        unsigned long numberRows    = MatrixBoolean::numberRows();
        unsigned long numberColumns = MatrixBoolean::numberColumns();

        if (other.numberRows() == static_cast<M::Integer>(numberRows)       &&
            other.numberColumns() == static_cast<M::Integer>(numberColumns)    ) {
            applyTransform();
            other.applyLazyTransformsAndScaling();

            unsigned long columnIndex = 0;
            result = true;
            while (result && columnIndex<numberColumns) {
                unsigned long rowIndex = 0;
                while (result && rowIndex<numberRows) {
                    bool v = (other.currentData->at(rowIndex, columnIndex) != MatrixReal::Scalar(0));
                    result = (currentData->at(rowIndex, columnIndex) == v);

                    ++rowIndex;
                }

                ++columnIndex;
            }
        } else {
            result = false;
        }

        return result;
    }


    bool MatrixBoolean::operator==(const MatrixComplex& other) const {
        bool result;

        unsigned long numberRows    = MatrixBoolean::numberRows();
        unsigned long numberColumns = MatrixBoolean::numberColumns();

        if (other.numberRows() == static_cast<M::Integer>(numberRows)       &&
            other.numberColumns() == static_cast<M::Integer>(numberColumns)    ) {
            applyTransform();
            other.applyLazyTransformsAndScaling();

            unsigned long columnIndex = 0;
            result = true;
            while (result && columnIndex<numberColumns) {
                unsigned long rowIndex = 0;
                while (result && rowIndex<numberRows) {
                    bool v = (other.currentData->at(rowIndex, columnIndex) != MatrixComplex::Scalar(0));
                    result = (currentData->at(rowIndex, columnIndex) == v);

                    ++rowIndex;
                }

                ++columnIndex;
            }
        } else {
            result = false;
        }

        return result;
    }


    int MatrixBoolean::relativeOrder(const MatrixBoolean& other) const {
        int result;

        if (currentData == other.currentData) {
            if (pendingTranspose == other.pendingTranspose) {
                result = 0;
            } else {
                result = static_cast<int>(pendingTranspose) - static_cast<int>(other.pendingTranspose);
            }
        } else {
            applyTransform();
            other.applyTransform();

            result = currentData->relativeOrder(*other.currentData);
        }

        return result;
    }


    MatrixBoolean::MatrixBoolean(
            Data* dataStore
        ):currentData(
            dataStore
        ),pendingTranspose(
            false
        ) {}


    void MatrixBoolean::releaseDataStore(MatrixBoolean::Data* dataStore) const {
        if (dataStore->removeReference()) {
            if (dataStore->matrixType() == MatrixType::DENSE) {
                DenseData::destroy(POLYMORPHIC_CAST<DenseData*>(dataStore));
            } else {
                assert(dataStore->matrixType() == MatrixType::SPARSE);
                assert(false); // FIXME
            }
        }
    }


    void MatrixBoolean::assignNewDataStore(MatrixBoolean::Data* newDataStore, bool unlock) const {
        Data* oldData = currentData;
        currentData = newDataStore;

        if (unlock) {
            oldData->unlock();
        }

        releaseDataStore(oldData);
    }


    void MatrixBoolean::assignExistingDataStore(MatrixBoolean::Data* newDataStore, bool unlock) const {
        newDataStore->addReference();
        assignNewDataStore(newDataStore, unlock);
    }


    void MatrixBoolean::applyTransform() const {
        if (pendingTranspose) {
            currentData->lock();
            Data* newDataStore = currentData->applyTransform();
            assignNewDataStore(newDataStore, true);

            pendingTranspose = false;
        }
    }
}
