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
* This file implements the \ref M::MatrixInteger class.
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
#include "m_matrix_boolean.h"
#include "m_matrix_boolean_data.h"
#include "m_matrix_boolean_dense_data.h"
#include "m_matrix_integer.h"
#include "m_matrix_complex.h"
#include "m_matrix_complex_data.h"
#include "m_matrix_complex_dense_data.h"
#include "m_matrix_real.h"
#include "m_matrix_real_data.h"
#include "m_matrix_real_dense_data.h"
#include "m_matrix_integer.h"
#include "m_matrix_integer_data.h"
#include "m_matrix_integer_dense_data.h"
#include "m_matrix_integer_sparse_data.h"

namespace M {
    MatrixInteger::MatrixInteger(Integer newNumberRows, Integer newNumberColumns) {
        if (newNumberRows < 0) {
            throw Model::InvalidRow(newNumberRows, 0);
        } else if (newNumberColumns < 0) {
            throw Model::InvalidColumn(newNumberColumns, 0);
        } else {
            currentData                  = DenseData::create(newNumberRows, newNumberColumns);
            pendingTranspose             = false;
            pendingScalarMultiplierValue = Scalar(1);
        }
    }


    MatrixInteger::MatrixInteger(
            Integer                      newNumberRows,
            Integer                      newNumberColumns,
            const MatrixInteger::Scalar* matrixData
        ) {
        if (newNumberRows < 0) {
            throw Model::InvalidRow(newNumberRows, 0);
        } else if (newNumberColumns < 0) {
            throw Model::InvalidColumn(newNumberColumns, 0);
        } else {
            currentData                  = DenseData::create(newNumberRows, newNumberColumns, matrixData);
            pendingTranspose             = false;
            pendingScalarMultiplierValue = Scalar(1);
        }
    }


    MatrixInteger::MatrixInteger(const MatrixBoolean& other) {
        copyFrom(other);
    }


    MatrixInteger::MatrixInteger(const MatrixInteger& other) {
        other.currentData->addReference();
        currentData = other.currentData;

        pendingTranspose             = other.pendingTranspose;
        pendingScalarMultiplierValue = other.pendingScalarMultiplierValue;
    }


    MatrixInteger::MatrixInteger(const MatrixReal& other) {
        copyFrom(other);
    }


    MatrixInteger::MatrixInteger(const MatrixComplex& other) {
        copyFrom(other);
    }


    MatrixInteger::MatrixInteger(const Variant& other) {
        bool          ok;
        MatrixInteger v = other.toMatrixInteger(&ok);

        if (ok) {
            v.currentData->addReference();
            currentData = v.currentData;

            pendingTranspose             = v.pendingTranspose;
            pendingScalarMultiplierValue = v.pendingScalarMultiplierValue;
        } else {
            throw Model::InvalidRuntimeConversion(other.valueType(), ValueType::MATRIX_INTEGER);
        }
    }


    MatrixInteger::~MatrixInteger() {
        releaseDataStore(currentData);
    }


    ValueType MatrixInteger::coefficientValueType() const {
        return ValueType::INTEGER;
    }


    bool MatrixInteger::toFile(const char* filename, Model::DataFileFormat fileFormat) const {
        return currentData->toFile(filename, fileFormat);
    }


    MatrixInteger MatrixInteger::fromFile(const char* filename) {
        return MatrixInteger(DenseData::fromFile(filename));
    }


    MatrixInteger::Index MatrixInteger::numberRows() const {
        return pendingTranspose ? currentData->numberColumns() : currentData->numberRows();
    }


    MatrixInteger::Index MatrixInteger::numberColumns() const {
        return pendingTranspose ? currentData->numberRows() : currentData->numberColumns();
    }


    MatrixInteger::Index MatrixInteger::numberCoefficients() const {
        return currentData->numberRows() * currentData->numberColumns();
    }


    Model::Variant MatrixInteger::value(MatrixInteger::Index row, MatrixInteger::Index column) const {
        Model::Variant result;

        if (row > 0 && static_cast<unsigned long>(row) <= currentData->numberRows()          &&
            column > 0 && static_cast<unsigned long>(column) <= currentData->numberColumns()    ) {
            result = currentData->at(row - 1, column - 1);
        }

        return result;
    }


    MatrixInteger MatrixInteger::identity(Index numberRows, Index numberColumns) {
        return MatrixInteger(DenseData::identity(numberRows, numberColumns));
    }


    MatrixInteger MatrixInteger::identity(Index numberRowColumns) {
        return MatrixInteger(DenseData::identity(numberRowColumns, numberRowColumns));
    }


    MatrixInteger MatrixInteger::ones(Index numberRows, Index numberColumns) {
        return MatrixInteger(DenseData::ones(numberRows, numberColumns));
    }


    MatrixInteger MatrixInteger::ones(Index numberRowColumns) {
        return MatrixInteger(DenseData::ones(numberRowColumns, numberRowColumns));
    }


    MatrixInteger MatrixInteger::diagonalEntries() const {
        unsigned long numberDiagnonalEntries = std::min(currentData->numberRows(), currentData->numberColumns());
        MatrixInteger result(numberDiagnonalEntries, 1);

        Scalar* data = result.currentData->data();

        for (unsigned index=0 ; index<numberDiagnonalEntries ; ++index) {
            Scalar v = currentData->at(index, index);
            data[index] = v;
        }

        return result;
    }


    MatrixInteger MatrixInteger::diagonal() const {
        unsigned long numberRows    = currentData->numberRows();
        unsigned long numberColumns = currentData->numberColumns();
        DenseData*    result        = nullptr;

        if ((numberRows != 1 && numberColumns != 1) || numberRows == 0 || numberColumns == 0) {
            throw Model::InvalidMatrixDimensions(numberRows, numberColumns);
        } else {
            unsigned long numberValues = std::max(numberRows, numberColumns);
            Scalar*       data         = currentData->data();

            result = DenseData::create(numberValues, numberValues);
            for (unsigned i=0 ; i<numberValues ; ++i) {
                Scalar  v = data[i];
                Scalar* d = &(result->at(i, i));
                *d = v;
            }
        }

        return MatrixInteger(result);
    }


    Model::Variant MatrixInteger::value(Matrix::Index index) const {
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


    bool MatrixInteger::setValue(MatrixInteger::Index row, MatrixInteger::Index column, const Model::Variant& value) {
        bool success;

        if (row > 0 && column > 0 && value.valueType() != ValueType::BOOLEAN) {
            Integer realValue = value.toInteger(&success);

            if (success) {
                update(row, column, realValue);
            }
        } else {
            success = false;
        }

        return success;
    }


    bool MatrixInteger::setValue(MatrixInteger::Index index, const Model::Variant& value) {
        bool success;

        if (index > 0 && value.valueType() != ValueType::BOOLEAN) {
            Integer realValue = value.toInteger(&success);

            if (success) {
                update(index, realValue);
            }
        } else {
            success = false;
        }

        return success;
    }


    void MatrixInteger::resize(MatrixInteger::Index newNumberRows, MatrixInteger::Index newNumberColumns) {
        applyLazyTransformsAndScaling();

        currentData->lock();
        if (currentData->referenceCount() > 1) {
            Data* oldDataStore = currentData;
            currentData = currentData->resizeTo(newNumberRows, newNumberColumns, true);

            oldDataStore->unlock();
            releaseDataStore(oldDataStore);
        } else {
            Data* newDataStore = currentData->resizeTo(newNumberRows, newNumberColumns, false);
            if (newDataStore != currentData) {
                assignNewDataStore(newDataStore, true);
            } else {
                currentData->unlock();
            }
        }
    }


    MatrixInteger::Scalar MatrixInteger::at(
            MatrixInteger::Index row,
            MatrixInteger::Index column
        ) const {
        applyLazyTransformsAndScaling();

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


    MatrixInteger::Scalar MatrixInteger::at(MatrixInteger::Index index) const {
        applyLazyTransformsAndScaling();

        const Scalar* result = nullptr;

        unsigned long      numberRows         = currentData->numberRows();
        unsigned long      numberColumns      = currentData->numberColumns();
        unsigned long long numberCoefficients = numberRows * numberColumns;

        if (index < 1 && static_cast<unsigned long long>(index) > numberCoefficients) {
            throw Model::InvalidIndex(index, numberCoefficients);
        } else {
            unsigned long long i           = static_cast<unsigned long long>(index - 1);
            unsigned long      rowIndex    = i / numberColumns;
            unsigned long      columnIndex = i % numberColumns;

            result = &(currentData->at(rowIndex, columnIndex));
        }

        return *result;
    }

    MatrixInteger::Scalar MatrixInteger::at(Real row, MatrixInteger::Index column) const {
        Index r;
        return toInteger(r, row) ? at(r, column) : Scalar(0);
    }


    MatrixInteger::Scalar MatrixInteger::at(const Complex& row, MatrixInteger::Index column) const {
        Index r;
        return toInteger(r, row) ? at(r, column) : Scalar(0);
    }


    MatrixInteger::Scalar MatrixInteger::at(MatrixInteger::Index row, Real column) const {
        Index c;
        return toInteger(c, column) ? at(row, c) : Scalar(0);
    }


    MatrixInteger::Scalar MatrixInteger::at(MatrixInteger::Index row, const Complex& column) const {
        Index c;
        return toInteger(c, column) ? at(row, c) : Scalar(0);
    }


    MatrixInteger::Scalar MatrixInteger::at(Real row, Real column) const {
        Index r;
        Index c;
        return (toInteger(r, row) && toInteger(c, column)) ? at(r, c) : Scalar(0);
    }


    MatrixInteger::Scalar MatrixInteger::at(const Complex& row, Real column) const {
        Index r;
        Index c;
        return (toInteger(r, row) && toInteger(c, column)) ? at(r, c) : Scalar(0);
    }


    MatrixInteger::Scalar MatrixInteger::at(Real row, const Complex& column) const {
        Index r;
        Index c;
        return (toInteger(r, row) && toInteger(c, column)) ? at(r, c) : Scalar(0);
    }


    MatrixInteger::Scalar MatrixInteger::at(const Complex& row, const Complex& column) const {
        Index r;
        Index c;
        return (toInteger(r, row) && toInteger(c, column)) ? at(r, c) : Scalar(0);
    }


    MatrixInteger::Scalar MatrixInteger::at(Real index) const {
        Index i;
        return toInteger(i, index) ? at(i) : Scalar(0);
    }


    MatrixInteger::Scalar MatrixInteger::at(const Complex& index) const {
        Index i;
        return toInteger(i, index) ? at(i) : Scalar(0);
    }


    MatrixInteger MatrixInteger::at(const Set& rowSet, MatrixInteger::Index column) const {
        return atICHelper(*this, rowSet, column);
    }


    MatrixInteger MatrixInteger::at(const Set& rowSet, Real column) const {
        return at(rowSet, inlineToInteger(column));
    }


    MatrixInteger MatrixInteger::at(const Set& rowSet, const Complex& column) const {
        return at(rowSet, inlineToInteger(column));
    }


    MatrixInteger MatrixInteger::at(const Set& rowSet, const Range& column) const {
        return atIIHelper(*this, rowSet, column);
    }


    MatrixInteger MatrixInteger::at(const Set& rowSet, const Set& columnSet) const {
        return atIIHelper(*this, rowSet, columnSet);
    }


    MatrixInteger MatrixInteger::at(const Set& rowSet, const Tuple& columnTuple) const {
        return atIIHelper(*this, rowSet, columnTuple);
    }


    MatrixInteger MatrixInteger::at(const Set& rowSet, const MatrixInteger& columnMatrix) const {
        return atIMHelper(*this, rowSet, columnMatrix);
    }


    MatrixInteger MatrixInteger::at(const Set& rowSet, const MatrixReal& columnMatrix) const {
        return atIMHelper(*this, rowSet, columnMatrix);
    }


    MatrixInteger MatrixInteger::at(const Set& rowSet, const MatrixComplex& columnMatrix) const {
        return atIMHelper(*this, rowSet, columnMatrix);
    }


    MatrixInteger MatrixInteger::at(MatrixInteger::Index row, const Set& columnSet) const {
        return atCIHelper(*this, row, columnSet);
    }


    MatrixInteger MatrixInteger::at(Real row, const Set& columnSet) const {
        return at(inlineToInteger(row), columnSet);
    }


    MatrixInteger MatrixInteger::at(const Complex& row, const Set& columnSet) const {
        return at(inlineToInteger(row), columnSet);
    }


    MatrixInteger MatrixInteger::at(const Range& rowRange, const Set& columnSet) const {
        return atIIHelper(*this, rowRange, columnSet);
    }


    MatrixInteger MatrixInteger::at(const Tuple& rowTuple, const Set& columnSet) const {
        return atIIHelper(*this, rowTuple, columnSet);
    }


    MatrixInteger MatrixInteger::at(const MatrixInteger& rowMatrix, const Set& columnSet) const {
        return atMIHelper(*this, rowMatrix, columnSet);
    }


    MatrixInteger MatrixInteger::at(const MatrixReal& rowMatrix, const Set& columnSet) const {
        return atMIHelper(*this, rowMatrix, columnSet);
    }


    MatrixInteger MatrixInteger::at(const MatrixComplex& rowMatrix, const Set& columnSet) const {
        return atMIHelper(*this, rowMatrix, columnSet);
    }


    MatrixInteger MatrixInteger::at(const Set& indexSet) const {
        return atIHelper(*this, indexSet);
    }


    MatrixInteger MatrixInteger::at(const Tuple& rowTuple, MatrixInteger::Index column) const {
        return atICHelper(*this, rowTuple, column);
    }


    MatrixInteger MatrixInteger::at(const Tuple& rowTuple, Real column) const {
        return at(rowTuple, inlineToInteger(column));
    }


    MatrixInteger MatrixInteger::at(const Tuple& rowTuple, const Complex& column) const {
        return at(rowTuple, inlineToInteger(column));
    }


    MatrixInteger MatrixInteger::at(const Tuple& rowTuple, const Range& columnRange) const {
        return atIIHelper(*this, rowTuple, columnRange);
    }


    MatrixInteger MatrixInteger::at(const Tuple& rowTuple, const Tuple& columnTuple) const {
        return atIIHelper(*this, rowTuple, columnTuple);
    }


    MatrixInteger MatrixInteger::at(const Tuple& rowTuple, const MatrixInteger& columnMatrix) const {
        return atIMHelper(*this, rowTuple, columnMatrix);
    }


    MatrixInteger MatrixInteger::at(const Tuple& rowTuple, const MatrixReal& columnMatrix) const {
        return atIMHelper(*this, rowTuple, columnMatrix);
    }


    MatrixInteger MatrixInteger::at(const Tuple& rowTuple, const MatrixComplex& columnMatrix) const {
        return atIMHelper(*this, rowTuple, columnMatrix);
    }


    MatrixInteger MatrixInteger::at(MatrixInteger::Index row, const Tuple& columnTuple) const {
        return atCIHelper(*this, row, columnTuple);
    }


    MatrixInteger MatrixInteger::at(Real row, const Tuple& columnTuple) const {
        return at(inlineToInteger(row), columnTuple);
    }


    MatrixInteger MatrixInteger::at(const Complex& row, const Tuple& columnTuple) const {
        return at(inlineToInteger(row), columnTuple);
    }


    MatrixInteger MatrixInteger::at(const Range& rowRange, const Tuple& columnTuple) const {
        return atIIHelper(*this, rowRange, columnTuple);
    }


    MatrixInteger MatrixInteger::at(const MatrixInteger& rowMatrix, const Tuple& columnTuple) const {
        return atMIHelper(*this, rowMatrix, columnTuple);
    }


    MatrixInteger MatrixInteger::at(const MatrixReal& rowMatrix, const Tuple& columnTuple) const {
        return atMIHelper(*this, rowMatrix, columnTuple);
    }


    MatrixInteger MatrixInteger::at(const MatrixComplex& rowMatrix, const Tuple& columnTuple) const {
        return atMIHelper(*this, rowMatrix, columnTuple);
    }


    MatrixInteger MatrixInteger::at(const Tuple& indexTuple) const {
        return atIHelper(*this, indexTuple);
    }


    MatrixInteger MatrixInteger::at(const MatrixInteger& rowMatrix, MatrixInteger::Index column) const {
        return atMCHelper(*this, rowMatrix, column);
    }


    MatrixInteger MatrixInteger::at(const MatrixInteger& rowMatrix, Real column) const {
        return at(rowMatrix, inlineToInteger(column));
    }


    MatrixInteger MatrixInteger::at(const MatrixInteger& rowMatrix, const Complex& column) const {
        return at(rowMatrix, inlineToInteger(column));
    }


    MatrixInteger MatrixInteger::at(const MatrixInteger& rowMatrix, const Range& columnRange) const {
        return atMIHelper(*this, rowMatrix, columnRange);
    }


    MatrixInteger MatrixInteger::at(const MatrixInteger& rowMatrix, const MatrixInteger& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixInteger MatrixInteger::at(const MatrixInteger& rowMatrix, const MatrixReal& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixInteger MatrixInteger::at(const MatrixInteger& rowMatrix, const MatrixComplex& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixInteger MatrixInteger::at(MatrixInteger::Index row, const MatrixInteger& columnMatrix) const {
        return atCMHelper(*this, row, columnMatrix);
    }


    MatrixInteger MatrixInteger::at(Real row, const MatrixInteger& columnMatrix) const {
        return at(inlineToInteger(row), columnMatrix);
    }


    MatrixInteger MatrixInteger::at(const Complex& row, const MatrixInteger& columnMatrix) const {
        return at(inlineToInteger(row), columnMatrix);
    }


    MatrixInteger MatrixInteger::at(const Range& rowRange, const MatrixInteger& columnMatrix) const {
        return atIMHelper(*this, rowRange, columnMatrix);
    }


    MatrixInteger MatrixInteger::at(const MatrixReal& rowMatrix, const MatrixInteger& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixInteger MatrixInteger::at(const MatrixComplex& rowMatrix, const MatrixInteger& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixInteger MatrixInteger::at(const MatrixInteger& indexMatrix) const {
        return atMHelper(*this, indexMatrix);
    }


    MatrixInteger MatrixInteger::at(const MatrixReal& rowMatrix, MatrixInteger::Index column) const {
        return atMCHelper(*this, rowMatrix, column);
    }


    MatrixInteger MatrixInteger::at(const MatrixReal& rowMatrix, Real column) const {
        return at(rowMatrix, inlineToInteger(column));
    }


    MatrixInteger MatrixInteger::at(const MatrixReal& rowMatrix, const Complex& column) const {
        return at(rowMatrix, inlineToInteger(column));
    }


    MatrixInteger MatrixInteger::at(const MatrixReal& rowMatrix, const Range& columnRange) const {
        return atMIHelper(*this, rowMatrix, columnRange);
    }


    MatrixInteger MatrixInteger::at(const MatrixReal& rowMatrix, const MatrixReal& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixInteger MatrixInteger::at(const MatrixReal& rowMatrix, const MatrixComplex& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixInteger MatrixInteger::at(MatrixInteger::Index row, const MatrixReal& columnMatrix) const {
        return atCMHelper(*this, row, columnMatrix);
    }


    MatrixInteger MatrixInteger::at(Real row, const MatrixReal& columnMatrix) const {
        return at(inlineToInteger(row), columnMatrix);
    }


    MatrixInteger MatrixInteger::at(const Complex& row, const MatrixReal& columnMatrix) const {
        return at(inlineToInteger(row), columnMatrix);
    }


    MatrixInteger MatrixInteger::at(const Range& rowRange, const MatrixReal& columnMatrix) const {
        return atIMHelper(*this, rowRange, columnMatrix);
    }


    MatrixInteger MatrixInteger::at(const MatrixComplex& rowMatrix, const MatrixReal& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixInteger MatrixInteger::at(const MatrixReal& indexMatrix) const {
        return atMHelper(*this, indexMatrix);
    }


    MatrixInteger MatrixInteger::at(const MatrixComplex& rowMatrix, MatrixInteger::Index column) const {
        return atMCHelper(*this, rowMatrix, column);
    }


    MatrixInteger MatrixInteger::at(const MatrixComplex& rowMatrix, Real column) const {
        return at(rowMatrix, inlineToInteger(column));
    }


    MatrixInteger MatrixInteger::at(const MatrixComplex& rowMatrix, const Complex& column) const {
        return at(rowMatrix, inlineToInteger(column));
    }


    MatrixInteger MatrixInteger::at(const MatrixComplex& rowMatrix, const Range& columnRange) const {
        return atMIHelper(*this, rowMatrix, columnRange);
    }


    MatrixInteger MatrixInteger::at(const MatrixComplex& rowMatrix, const MatrixComplex& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixInteger MatrixInteger::at(MatrixInteger::Index row, const MatrixComplex& columnMatrix) const {
        return atCMHelper(*this, row, columnMatrix);
    }


    MatrixInteger MatrixInteger::at(Real row, const MatrixComplex& columnMatrix) const {
        return at(inlineToInteger(row), columnMatrix);
    }


    MatrixInteger MatrixInteger::at(const Complex& row, const MatrixComplex& columnMatrix) const {
        return at(inlineToInteger(row), columnMatrix);
    }


    MatrixInteger MatrixInteger::at(const Range& rowRange, const MatrixComplex& columnMatrix) const {
        return atIMHelper(*this, rowRange, columnMatrix);
    }


    MatrixInteger MatrixInteger::at(const MatrixComplex& indexMatrix) const {
        return atMHelper(*this, indexMatrix);
    }


    MatrixInteger MatrixInteger::at(const Range& rowRange, MatrixInteger::Index column) const {
        return atICHelper(*this, rowRange, column);
    }


    MatrixInteger MatrixInteger::at(const Range& rowRange, Real column) const {
        return at(rowRange, inlineToInteger(column));
    }


    MatrixInteger MatrixInteger::at(const Range& rowRange, const Complex& column) const {
        return at(rowRange, inlineToInteger(column));
    }


    MatrixInteger MatrixInteger::at(const Range& rowRange, const Range& columnRange) const {
        return atIIHelper(*this, rowRange, columnRange);
    }


    MatrixInteger MatrixInteger::at(MatrixInteger::Index row, const Range& columnRange) const {
        return atCIHelper(*this, row, columnRange);
    }


    MatrixInteger MatrixInteger::at(Real row, const Range& columnRange) const {
        return at(inlineToInteger(row), columnRange);
    }


    MatrixInteger MatrixInteger::at(const Complex& row, const Range& columnRange) const {
        return at(inlineToInteger(row), columnRange);
    }


    Variant MatrixInteger::at(const Variant& row, MatrixInteger::Index column) const {
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


    Variant MatrixInteger::at(const Variant& row, Real column) const {
        return at(row, inlineToInteger(column));
    }


    Variant MatrixInteger::at(const Variant& row, const Complex& column) const {
        return at(row, inlineToInteger(column));
    }


    Variant MatrixInteger::at(const Variant& row, const Range& columnRange) const {
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


    Variant MatrixInteger::at(const Variant& row, const Set& columnSet) const {
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


    Variant MatrixInteger::at(const Variant& row, const Tuple& columnTuple) const {
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


    Variant MatrixInteger::at(const Variant& row, const MatrixInteger& columnMatrix) const {
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


    Variant MatrixInteger::at(const Variant& row, const MatrixReal& columnMatrix) const {
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


    Variant MatrixInteger::at(const Variant& row, const MatrixComplex& columnMatrix) const {
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


    Variant MatrixInteger::at(MatrixInteger::Index row, const Variant& column) const {
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


    Variant MatrixInteger::at(Real row, const Variant& column) const {
        return at(inlineToInteger(row), column);
    }


    Variant MatrixInteger::at(const Complex& row, const Variant& column) const {
        return at(inlineToInteger(row), column);
    }


    Variant MatrixInteger::at(const Range& rowRange, const Variant& column) const {
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


    Variant MatrixInteger::at(const Set& rowSet, const Variant& column) const {
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


    Variant MatrixInteger::at(const Tuple& rowTuple, const Variant& column) const {
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


    Variant MatrixInteger::at(const MatrixInteger& rowMatrix, const Variant& column) const {
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


    Variant MatrixInteger::at(const MatrixReal& rowMatrix, const Variant& column) const {
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


    Variant MatrixInteger::at(const MatrixComplex& rowMatrix, const Variant& column) const {
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


    Variant MatrixInteger::at(const Variant& row, const Variant& column) const {
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


    Variant MatrixInteger::at(const Variant& index) const {
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


    const MatrixInteger::Scalar* MatrixInteger::pointer(MatrixInteger::Index row, MatrixInteger::Index column) const {
        applyLazyTransformsAndScaling();

        const Scalar* result = nullptr;

        unsigned long numberRows = currentData->numberRows();
        if (row < 1 || static_cast<unsigned long>(row) > numberRows) {
            throw Model::InvalidRow(row, numberRows);
        } else {
            unsigned long numberColumns = currentData->numberColumns();
            if (column < 1 || static_cast<unsigned long>(column) > numberColumns) {
                throw Model::InvalidColumn(column, numberColumns);
            } else {
                result = &(currentData->at(row - 1, column - 1));
            }
        }

        return result;
    }


    void MatrixInteger::update(Matrix::Index row, Matrix::Index column, MatrixInteger::Scalar newValue) {
        applyLazyTransformsAndScaling();

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

                currentData->at(row - 1, column - 1) = newValue;
            } else {
                if (static_cast<unsigned long>(row) > numberRows       ||
                    static_cast<unsigned long>(column) > numberColumns    ) {
                    unsigned long newNumberRows    = std::max(numberRows, static_cast<unsigned long>(row));
                    unsigned long newNumberColumns = std::max(numberColumns, static_cast<unsigned long>(column));

                    Data* newDataStore = currentData->resizeTo(newNumberRows, newNumberColumns);
                    if (newDataStore != currentData) {
                        assignNewDataStore(newDataStore, true);
                        currentData->at(row - 1, column - 1) = newValue;
                    } else {
                        currentData->at(row - 1, column - 1) = newValue;
                        currentData->unlock();
                    }
                } else {
                    currentData->at(row - 1, column - 1) = newValue;
                    currentData->unlock();
                }
            }
        }
    }


    void MatrixInteger::update(Matrix::Index index, MatrixInteger::Scalar newValue) {
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


    MatrixInteger MatrixInteger::combineLeftToRight(const MatrixInteger& other) const {
        applyLazyTransformsAndScaling();
        other.applyLazyTransformsAndScaling();

        Data* resultData;
        if (other.currentData->matrixType() == Matrix::MatrixType::DENSE) {
            resultData = currentData->combineLeftToRight(*POLYMORPHIC_CAST<const DenseData*>(other.currentData));
        } else {
            assert(other.currentData->matrixType() == Matrix::MatrixType::SPARSE);
            assert(false); // FIXME
            resultData = nullptr;
//            resultData = currentData->combineLeftToRight(*POLYMORPHIC_CAST<const SparseData*>(other.currentData));
        }

        return MatrixInteger(resultData);
    }


    MatrixInteger MatrixInteger::combineTopToBottom(const MatrixInteger& other) const {
        applyLazyTransformsAndScaling();
        other.applyLazyTransformsAndScaling();

        Data* resultData;
        if (other.currentData->matrixType() == Matrix::MatrixType::DENSE) {
            resultData = currentData->combineTopToBottom(*POLYMORPHIC_CAST<const DenseData*>(other.currentData));
        } else {
            assert(other.currentData->matrixType() == Matrix::MatrixType::SPARSE);
            assert(false); // FIXME
            resultData = nullptr;
//            resultData = currentData->combineTopToBottom(*POLYMORPHIC_CAST<const SparseData*>(other.currentData));
        }

        return MatrixInteger(resultData);
    }


    MatrixInteger MatrixInteger::columnReverse() const {
        applyLazyTransformsAndScaling();
        return MatrixInteger(currentData->columnReverse());
    }


    MatrixInteger MatrixInteger::rowReverse() const {
        applyLazyTransformsAndScaling();
        return MatrixInteger(currentData->rowReverse());
    }


    bool MatrixInteger::isSquare() const {
        return currentData->numberRows() == currentData->numberColumns();
    }


    MatrixInteger MatrixInteger::transpose() const {
        MatrixInteger result(*this);
        result.pendingTranspose = !pendingTranspose;
        return result;
    }


    MatrixInteger MatrixInteger::conj() const {
        return MatrixInteger(*this);
    }


    MatrixInteger MatrixInteger::adjoint() const {
        return transpose();
    }


    MatrixInteger MatrixInteger::hadamard(const MatrixInteger& other) const {
        applyLazyTransformsAndScaling();
        other.applyLazyTransformsAndScaling();

        Data* result = nullptr;
        if (other.currentData->matrixType() == MatrixType::DENSE) {
            result = currentData->hadamard(*POLYMORPHIC_CAST<const DenseData*>(other.currentData));
        } else {
            assert(other.currentData->matrixType() == MatrixType::SPARSE);
            assert(false); // FIXME
//            result = currentData->hadamard(*POLYMORPHIC_CAST<const SparseData*>(other.currentData));
        }

        return MatrixInteger(result);
    }


    MatrixInteger MatrixInteger::kronecker(const MatrixInteger& other) const {
        applyLazyTransformsAndScaling();

        Data* result = nullptr;
        if (other.currentData->matrixType() == MatrixType::DENSE) {
            result = currentData->kronecker(
                *POLYMORPHIC_CAST<const DenseData*>(other.currentData),
                other.pendingTranspose,
                other.pendingScalarMultiplierValue
            );
        } else {
            assert(other.currentData->matrixType() == MatrixType::SPARSE);
            assert(false); // FIXME
//            result = currentData->hadamard(*POLYMORPHIC_CAST<const SparseData*>(other.currentData));
        }

        return MatrixInteger(result);
    }


    MatrixInteger::MatrixType MatrixInteger::matrixType() const {
        return currentData->matrixType();
    }


    MatrixInteger::Scalar* MatrixInteger::data() {
        applyLazyTransformsAndScaling();
        return currentData->data();
    }


    const MatrixInteger::Scalar* MatrixInteger::data() const {
        applyLazyTransformsAndScaling();
        return currentData->data();
    }


    MatrixInteger MatrixInteger::randomInteger64(PerThread& pt, Integer numberRows, Integer numberColumns) {
        return MatrixInteger(DenseData::randomInteger64(pt, numberRows, numberColumns));
    }


    MatrixInteger MatrixInteger::randomInteger32(PerThread& pt, Integer numberRows, Integer numberColumns) {
        return MatrixInteger(DenseData::randomInteger32(pt, numberRows, numberColumns));
    }


    MatrixInteger MatrixInteger::randomPoisson(PerThread& pt, Integer numberRows, Integer numberColumns, Real rate) {
        if (rate <= 0) {
            throw Model::InvalidNumericValue();
        }

        return MatrixInteger(DenseData::randomPoisson(pt, numberRows, numberColumns, rate));
    }


    MatrixInteger MatrixInteger::randomBinomial(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns,
            Integer    n,
            Real       p
        ) {
        if (n <= 0 || p < 0.0 || p > 1.0) {
            throw Model::InvalidNumericValue();
        }

        return MatrixInteger(DenseData::randomBinomial(pt, numberRows, numberColumns, n, p));
    }


    MatrixInteger MatrixInteger::randomGeometric(PerThread& pt, Integer numberRows, Integer numberColumns, Real p) {
        if (p < 0.0 || p > 1.0) {
            throw Model::InvalidNumericValue();
        }

        return MatrixInteger(DenseData::randomGeometric(pt, numberRows, numberColumns, p));
    }


    MatrixInteger& MatrixInteger::operator=(const MatrixInteger& other) {
        assignExistingDataStore(other.currentData, false);

        pendingTranspose             = other.pendingTranspose;
        pendingScalarMultiplierValue = other.pendingScalarMultiplierValue;

        return *this;
    }


    MatrixInteger& MatrixInteger::operator=(const MatrixBoolean& other) {
        releaseDataStore(currentData);
        copyFrom(other);

        return *this;
    }


    MatrixInteger& MatrixInteger::operator=(const MatrixReal& other) {
        releaseDataStore(currentData);
        copyFrom(other);

        return *this;
    }


    MatrixInteger& MatrixInteger::operator=(const MatrixComplex& other) {
        releaseDataStore(currentData);
        copyFrom(other);

        return *this;
    }


    MatrixInteger& MatrixInteger::operator=(const Variant& other) {
        bool          ok;
        MatrixInteger v = other.toMatrixInteger(&ok);

        if (ok) {
            assignExistingDataStore(v.currentData, false);

            pendingTranspose             = v.pendingTranspose;
            pendingScalarMultiplierValue = v.pendingScalarMultiplierValue;
        } else {
            throw Model::InvalidRuntimeConversion(other.valueType(), ValueType::MATRIX_INTEGER);
        }

        return *this;
    }


    MatrixInteger& MatrixInteger::operator+=(const MatrixInteger& v) {
        currentData->lock();
        v.currentData->lock();

        Data* newDataStore = nullptr;
        if (v.currentData->matrixType() == MatrixType::DENSE) {
            newDataStore = currentData->add(
                *POLYMORPHIC_CAST<const DenseData*>(v.currentData),
                pendingTranspose,
                v.pendingTranspose,
                pendingScalarMultiplierValue,
                v.pendingScalarMultiplierValue
            );
        } else {
            assert(v.currentData->matrixType() == MatrixType::SPARSE);
            assert(false);
//            newDataStore = currentData->add(
//                *POLYMORPHIC_CAST<const SparseData*>(v.currentData),
//                pendingTransform,
//                v.pendingTransform,
//                pendingScalarMultiplierValue,
//                v.pendingScalarMultiplierValue
//            );
        }

        v.currentData->unlock();
        assignNewDataStore(newDataStore, true);

        pendingTranspose             = false;
        pendingScalarMultiplierValue = Scalar(1);

        return *this;
    }


    MatrixInteger& MatrixInteger::operator-=(const MatrixInteger& v) {
        currentData->lock();
        v.currentData->lock();

        Data* newDataStore = nullptr;
        if (v.currentData->matrixType() == MatrixType::DENSE) {
            newDataStore = currentData->add(
                *POLYMORPHIC_CAST<const DenseData*>(v.currentData),
                pendingTranspose,
                v.pendingTranspose,
                pendingScalarMultiplierValue,
                -v.pendingScalarMultiplierValue
            );
        } else {
            assert(v.currentData->matrixType() == MatrixType::SPARSE);
            assert(false);
//            newDataStore = currentData->add(
//                *POLYMORPHIC_CAST<const SparseData*>(v.currentData),
//                pendingTransform,
//                v.pendingTransform,
//                pendingScalarMultiplierValue,
//                -v.pendingScalarMultiplierValue
//            );
        }

        v.currentData->unlock();
        assignNewDataStore(newDataStore, true);

        pendingTranspose             = false;
        pendingScalarMultiplierValue = Scalar(1);

        return *this;
    }


    MatrixInteger& MatrixInteger::operator*=(const MatrixInteger& v) {
        Data* newDataStore;

        Real  scalarMultiplier = pendingScalarMultiplierValue * v.pendingScalarMultiplierValue;
        Data* multiplicand     = v.currentData;

        currentData->lock();

        multiplicand->lock();
        if (multiplicand->matrixType() == MatrixType::DENSE) {
            newDataStore = currentData->multiply(
                static_cast<const DenseData&>(*multiplicand),
                pendingTranspose,
                v.pendingTranspose,
                scalarMultiplier
            );
        } else {
            assert(multiplicand->matrixType() == MatrixType::SPARSE);

            newDataStore = currentData->multiply(
                static_cast<const SparseData&>(*multiplicand),
                pendingTranspose,
                v.pendingTranspose,
                scalarMultiplier
            );
        }
        multiplicand->unlock();
        assignNewDataStore(newDataStore, true);

        pendingTranspose             = false;
        pendingScalarMultiplierValue = Scalar(1);

        return *this;
    }


    MatrixInteger& MatrixInteger::operator*=(Boolean v) {
        if (v == false) {
            pendingScalarMultiplierValue = Scalar(0);
        }

        return *this;
    }


    MatrixInteger& MatrixInteger::operator*=(Integer v) {
        pendingScalarMultiplierValue *= Scalar(v);
        return *this;
    }


    MatrixInteger MatrixInteger::operator+(const MatrixBoolean& v) const {
        return operator+(MatrixInteger(v));
    }


    MatrixInteger MatrixInteger::operator+(const MatrixInteger& v) const {
        currentData->lock();
        v.currentData->lock();

        Data* newDataStore = nullptr;
        if (v.currentData->matrixType() == MatrixType::DENSE) {
            newDataStore = currentData->add(
                *POLYMORPHIC_CAST<const DenseData*>(v.currentData),
                pendingTranspose,
                v.pendingTranspose,
                pendingScalarMultiplierValue,
                v.pendingScalarMultiplierValue
            );
        } else {
            assert(v.currentData->matrixType() == MatrixType::SPARSE);
            assert(false); // FIXME
//            newDataStore = currentData->add(
//                *POLYMORPHIC_CAST<const SparseData*>(v.currentData),
//                pendingTransform,
//                v.pendingTransform,
//                pendingScalarMultiplierValue,
//                v.pendingScalarMultiplierValue
//            );
        }

        v.currentData->unlock();
        currentData->unlock();

        return MatrixInteger(newDataStore);
    }


    MatrixInteger MatrixInteger::operator-(const MatrixBoolean& v) const {
        return operator-(MatrixInteger(v));
    }


    MatrixInteger MatrixInteger::operator-(const MatrixInteger& v) const {
        currentData->lock();
        v.currentData->lock();

        Data* newDataStore = nullptr;
        if (v.currentData->matrixType() == MatrixType::DENSE) {
            newDataStore = currentData->add(
                *POLYMORPHIC_CAST<const DenseData*>(v.currentData),
                pendingTranspose,
                v.pendingTranspose,
                pendingScalarMultiplierValue,
                -v.pendingScalarMultiplierValue
            );
        } else {
            assert(v.currentData->matrixType() == MatrixType::SPARSE);
            assert(false);
//            newDataStore = currentData->add(
//                *POLYMORPHIC_CAST<const SparseData*>(v.currentData),
//                pendingTransform,
//                v.pendingTransform,
//                pendingScalarMultiplierValue,
//                -v.pendingScalarMultiplierValue
//            );
        }

        v.currentData->unlock();
        currentData->unlock();

        return MatrixInteger(newDataStore);
    }


    MatrixInteger MatrixInteger::operator*(const MatrixBoolean& v) const {
        return operator*(MatrixInteger(v));
    }


    MatrixInteger MatrixInteger::operator*(const MatrixInteger& v) const {
        Data* result;

        Real  scalarMultiplier = pendingScalarMultiplierValue * v.pendingScalarMultiplierValue;
        Data* multiplicand     = v.currentData;

        if (multiplicand->matrixType() == MatrixType::DENSE) {
            result = currentData->multiply(
                static_cast<const DenseData&>(*multiplicand),
                pendingTranspose,
                v.pendingTranspose,
                scalarMultiplier
            );
        } else {
            assert(multiplicand->matrixType() == MatrixType::SPARSE);

            result = currentData->multiply(
                static_cast<const SparseData&>(*multiplicand),
                pendingTranspose,
                v.pendingTranspose,
                scalarMultiplier
            );
        }

        return MatrixInteger(result);
    }


    MatrixInteger MatrixInteger::operator*(Boolean v) const {
        MatrixInteger result(*this);

        if (v == false) {
            result.pendingScalarMultiplierValue = Scalar(0);
        }

        return result;
    }


    MatrixInteger MatrixInteger::operator*(Integer v) const {
        MatrixInteger result(*this);
        result.pendingScalarMultiplierValue *= Scalar(v);

        return result;
    }


    MatrixInteger MatrixInteger::operator-() const {
        MatrixInteger result(*this);
        result.pendingScalarMultiplierValue *= Scalar(-1);

        return result;
    }


    bool MatrixInteger::operator==(const MatrixInteger& other) const {
        bool result;

        if (other.currentData->matrixType() == Matrix::MatrixType::DENSE) {
            applyLazyTransformsAndScaling();
            other.applyLazyTransformsAndScaling();

            if (currentData == other.currentData) {
                result = true;
            } else {
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


    int MatrixInteger::relativeOrder(const MatrixInteger& other) const {
        int result;

        if (currentData == other.currentData) {
            if (pendingTranspose == other.pendingTranspose) {
                if (pendingScalarMultiplierValue < other.pendingScalarMultiplierValue) {
                    result = -1;
                } else if (pendingScalarMultiplierValue > other.pendingScalarMultiplierValue) {
                    result = +1;
                } else /* if (pendingScalarMultiplierValue.real() == other.pendingScalarMultiplierValue.real()) */ {
                    result = 0;
                }
            } else {
                result = static_cast<int>(pendingTranspose) - static_cast<int>(other.pendingTranspose);
            }
        } else {
            applyLazyTransformsAndScaling();
            other.applyLazyTransformsAndScaling();

            result = currentData->relativeOrder(*other.currentData);
        }

        return result;
    }


    MatrixInteger::MatrixInteger(
            Data* dataStore
        ):currentData(
            dataStore
        ),pendingTranspose(
            false
        ),pendingScalarMultiplierValue(
            1
        ) {}


    void MatrixInteger::copyFrom(const MatrixBoolean& booleanMatrix) {
        MatrixBoolean::Data* otherData = booleanMatrix.currentData;

        if (otherData->matrixType() == MatrixType::DENSE) {
            currentData = DenseData::create(*POLYMORPHIC_CAST<MatrixBoolean::DenseData*>(otherData));
        } else {
            assert(otherData->matrixType() == MatrixType::SPARSE);
            assert(false); // FIXME
        }

        pendingTranspose             = booleanMatrix.pendingTranspose;
        pendingScalarMultiplierValue = Scalar(1);
    }


    void MatrixInteger::copyFrom(const MatrixReal& realMatrix) {
        realMatrix.applyLazyTransformsAndScaling();

        MatrixReal::Data* otherData = realMatrix.currentData;

        if (otherData->matrixType() == MatrixType::DENSE) {
            currentData = DenseData::create(*POLYMORPHIC_CAST<MatrixReal::DenseData*>(otherData));
        } else {
            assert(otherData->matrixType() == MatrixType::SPARSE);
            assert(false); // FIXME
        }

        pendingTranspose             = false;
        pendingScalarMultiplierValue = Scalar(1);
    }


    void MatrixInteger::copyFrom(const MatrixComplex& complexMatrix) {
        complexMatrix.applyLazyTransformsAndScaling();

        MatrixComplex::Data* otherData = complexMatrix.currentData;

        if (otherData->matrixType() == MatrixType::DENSE) {
            currentData = DenseData::create(*POLYMORPHIC_CAST<MatrixComplex::DenseData*>(otherData));
        } else {
            assert(otherData->matrixType() == MatrixType::SPARSE);
            assert(false); // FIXME
        }

        pendingTranspose             = false;
        pendingScalarMultiplierValue = Scalar(1);
    }


    void MatrixInteger::releaseDataStore(MatrixInteger::Data* dataStore) const {
        if (dataStore->removeReference()) {
            Data::destroy(dataStore);
        }
    }


    void MatrixInteger::assignNewDataStore(MatrixInteger::Data* newDataStore, bool unlock) const {
        Data* oldData = currentData;
        currentData = newDataStore;

        if (unlock) {
            oldData->unlock();
        }

        releaseDataStore(oldData);
    }


    void MatrixInteger::assignExistingDataStore(MatrixInteger::Data* newDataStore, bool unlock) const {
        newDataStore->addReference();
        assignNewDataStore(newDataStore, unlock);
    }


    void MatrixInteger::applyLazyTransformsAndScaling() const {
        if (pendingScalarMultiplierValue != Scalar(1) || pendingTranspose) {
            currentData->lock();
            Data* newDataStore = currentData->applyTransformAndScaling(pendingTranspose, pendingScalarMultiplierValue);
            assignNewDataStore(newDataStore, true);

            pendingTranspose             = false;
            pendingScalarMultiplierValue = Scalar(1);
        }
    }


    MatrixInteger operator*(Boolean a, const MatrixInteger& b) {
        return b * Integer(a);
    }


    MatrixInteger operator*(Integer a, const MatrixInteger& b) {
        return b * a;
    }


    MatrixInteger operator*(Integer a, const MatrixBoolean& b) {
        return MatrixInteger(b) * a;
    }


    MatrixInteger operator*(const MatrixBoolean& a, const MatrixBoolean& b) {
        return MatrixInteger(a) * MatrixInteger(b);
    }


    MatrixInteger operator*(const MatrixBoolean& a, const MatrixInteger& b) {
        return MatrixInteger(a) * b;
    }


    MatrixInteger operator+(const MatrixBoolean& a, const MatrixBoolean& b) {
        return MatrixInteger(a) + MatrixInteger(b);
    }


    MatrixInteger operator+(const MatrixBoolean& a, const MatrixInteger& b) {
        return MatrixInteger(a) + b;
    }


    MatrixInteger operator-(const MatrixBoolean& a, const MatrixBoolean& b) {
        return MatrixInteger(a) - MatrixInteger(b);
    }


    MatrixInteger operator-(const MatrixBoolean& a, const MatrixInteger& b) {
        return MatrixInteger(a) - b;
    }


    MatrixInteger operator*(Boolean a, const MatrixBoolean& b) {
        return Integer(a) * MatrixInteger(b);
    }

    MatrixInteger operator*(const MatrixBoolean& a, Boolean b) {
        return MatrixInteger(a) * Integer(b);
    }

    MatrixInteger operator*(const MatrixBoolean& a, const Integer b) {
        return MatrixInteger(a) * b;
    }

}
