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
* This file implements the \ref M::MatrixReal class.
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
#include "m_matrix_boolean_sparse_data.h"
#include "m_matrix_integer.h"
#include "m_matrix_integer_data.h"
#include "m_matrix_integer_dense_data.h"
#include "m_matrix_integer_sparse_data.h"
#include "m_matrix_complex.h"
#include "m_matrix_complex_data.h"
#include "m_matrix_complex_dense_data.h"
#include "m_matrix_complex_sparse_data.h"
#include "m_matrix_real.h"
#include "m_matrix_real_data.h"
#include "m_matrix_real_dense_data.h"
#include "m_matrix_real_sparse_data.h"

namespace M {
    MatrixReal::MatrixReal(Integer newNumberRows, Integer newNumberColumns) {
        if (newNumberRows < 0) {
            throw Model::InvalidRow(newNumberRows, 0);
        } else if (newNumberColumns < 0) {
            throw Model::InvalidColumn(newNumberColumns, 0);
        } else {
            currentData = DenseData::create(newNumberRows, newNumberColumns);
            pendingTranspose   = false;
            pendingScalarValue = Scalar(1);
        }
    }


    MatrixReal::MatrixReal(Integer newNumberRows, Integer newNumberColumns, const MatrixReal::Scalar* matrixData) {
        if (newNumberRows < 0) {
            throw Model::InvalidRow(newNumberRows, 0);
        } else if (newNumberColumns < 0) {
            throw Model::InvalidColumn(newNumberColumns, 0);
        } else {
            currentData = DenseData::create(newNumberRows, newNumberColumns, matrixData);
            pendingTranspose   = false;
            pendingScalarValue = Scalar(1);
        }
    }


    MatrixReal::MatrixReal(const MatrixBoolean& other) {
        copyFrom(other);
    }


    MatrixReal::MatrixReal(const MatrixInteger& other) {
        copyFrom(other);
    }


    MatrixReal::MatrixReal(const MatrixReal& other) {
        other.currentData->addReference();
        currentData = other.currentData;

        pendingTranspose   = other.pendingTranspose;
        pendingScalarValue = other.pendingScalarValue;
    }


    MatrixReal::MatrixReal(const MatrixComplex& other) {
        copyFrom(other);
    }


    MatrixReal::MatrixReal(const Variant& other) {
        bool       ok;
        MatrixReal v = other.toMatrixReal(&ok);

        if (ok) {
            v.currentData->addReference();
            currentData = v.currentData;

            pendingTranspose   = v.pendingTranspose;
            pendingScalarValue = v.pendingScalarValue;
        } else {
            throw Model::InvalidRuntimeConversion(other.valueType(), ValueType::MATRIX_REAL);
        }
    }


    MatrixReal::~MatrixReal() {
        releaseDataStore(currentData);
    }


    ValueType MatrixReal::coefficientValueType() const {
        return ValueType::REAL;
    }


    bool MatrixReal::toFile(const char* filename, Model::DataFileFormat fileFormat) const {
        return currentData->toFile(filename, fileFormat);
    }


    MatrixReal MatrixReal::fromFile(const char* filename) {
        return MatrixReal(DenseData::fromFile(filename));
    }


    MatrixReal::Index MatrixReal::numberRows() const {
        return pendingTranspose ? currentData->numberColumns() : currentData->numberRows();
    }


    MatrixReal::Index MatrixReal::numberColumns() const {
        return pendingTranspose ? currentData->numberRows() : currentData->numberColumns();
    }


    MatrixReal::Index MatrixReal::numberCoefficients() const {
        return currentData->numberRows() * currentData->numberColumns();
    }


    Model::Variant MatrixReal::value(MatrixReal::Index row, MatrixReal::Index column) const {
        Model::Variant result;

        if (row > 0 && static_cast<unsigned long>(row) <= currentData->numberRows()          &&
            column > 0 && static_cast<unsigned long>(column) <= currentData->numberColumns()    ) {
            result = currentData->at(row - 1, column - 1);
        }

        return result;
    }


    MatrixReal MatrixReal::identity(Index numberRows, Index numberColumns) {
        return MatrixReal(DenseData::identity(numberRows, numberColumns));
    }


    MatrixReal MatrixReal::identity(Index numberRowColumns) {
        return MatrixReal(DenseData::identity(numberRowColumns, numberRowColumns));
    }


    MatrixReal MatrixReal::ones(Index numberRows, Index numberColumns) {
        return MatrixReal(DenseData::ones(numberRows, numberColumns));
    }


    MatrixReal MatrixReal::ones(Index numberRowColumns) {
        return MatrixReal(DenseData::ones(numberRowColumns, numberRowColumns));
    }


    MatrixReal MatrixReal::diagonalEntries() const {
        unsigned long numberDiagnonalEntries = std::min(currentData->numberRows(), currentData->numberColumns());
        MatrixReal result(numberDiagnonalEntries, 1);

        Scalar* data = result.currentData->data();

        for (unsigned index=0 ; index<numberDiagnonalEntries ; ++index) {
            Scalar v = currentData->at(index, index);
            data[index] = v;
        }

        return result;
    }


    MatrixReal MatrixReal::diagonal() const {
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

        return MatrixReal(result);
    }


    Model::Variant MatrixReal::value(Matrix::Index index) const {
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


    bool MatrixReal::setValue(MatrixReal::Index row, MatrixReal::Index column, const Model::Variant& value) {
        bool success;

        if (row > 0 && column > 0 && value.valueType() != ValueType::BOOLEAN) {
            Real realValue = value.toReal(&success);

            if (success) {
                update(row, column, realValue);
            }
        } else {
            success = false;
        }

        return success;
    }


    bool MatrixReal::setValue(MatrixReal::Index index, const Model::Variant& value) {
        bool success;

        if (index > 0 && value.valueType() != ValueType::BOOLEAN) {
            Real realValue = value.toReal(&success);

            if (success) {
                update(index, realValue);
            }
        } else {
            success = false;
        }

        return success;
    }


    void MatrixReal::resize(MatrixReal::Index newNumberRows, MatrixReal::Index newNumberColumns) {
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


    MatrixReal::Scalar MatrixReal::at(
            MatrixReal::Index row,
            MatrixReal::Index column
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


    MatrixReal::Scalar MatrixReal::at(MatrixReal::Index index) const {
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


    MatrixReal::Scalar MatrixReal::at(Real row, MatrixReal::Index column) const {
        Index r;
        return toInteger(r, row) ? at(r, column) : Scalar(0);
    }


    MatrixReal::Scalar MatrixReal::at(const Complex& row, MatrixReal::Index column) const {
        Index r;
        return toInteger(r, row) ? at(r, column) : Scalar(0);
    }


    MatrixReal::Scalar MatrixReal::at(MatrixReal::Index row, Real column) const {
        Index c;
        return toInteger(c, column) ? at(row, c) : Scalar(0);
    }


    MatrixReal::Scalar MatrixReal::at(MatrixReal::Index row, const Complex& column) const {
        Index c;
        return toInteger(c, column) ? at(row, c) : Scalar(0);
    }


    MatrixReal::Scalar MatrixReal::at(Real row, Real column) const {
        Index r;
        Index c;
        return (toInteger(r, row) && toInteger(c, column)) ? at(r, c) : Scalar(0);
    }


    MatrixReal::Scalar MatrixReal::at(const Complex& row, Real column) const {
        Index r;
        Index c;
        return (toInteger(r, row) && toInteger(c, column)) ? at(r, c) : Scalar(0);
    }


    MatrixReal::Scalar MatrixReal::at(Real row, const Complex& column) const {
        Index r;
        Index c;
        return (toInteger(r, row) && toInteger(c, column)) ? at(r, c) : Scalar(0);
    }


    MatrixReal::Scalar MatrixReal::at(const Complex& row, const Complex& column) const {
        Index r;
        Index c;
        return (toInteger(r, row) && toInteger(c, column)) ? at(r, c) : Scalar(0);
    }


    MatrixReal::Scalar MatrixReal::at(Real index) const {
        Index i;
        return toInteger(i, index) ? at(i) : Scalar(0);
    }


    MatrixReal::Scalar MatrixReal::at(const Complex& index) const {
        Index i;
        return toInteger(i, index) ? at(i) : Scalar(0);
    }


    MatrixReal MatrixReal::at(const Set& rowSet, MatrixReal::Index column) const {
        return atICHelper(*this, rowSet, column);
    }


    MatrixReal MatrixReal::at(const Set& rowSet, Real column) const {
        return at(rowSet, inlineToInteger(column));
    }


    MatrixReal MatrixReal::at(const Set& rowSet, const Complex& column) const {
        return at(rowSet, inlineToInteger(column));
    }


    MatrixReal MatrixReal::at(const Set& rowSet, const Range& column) const {
        return atIIHelper(*this, rowSet, column);
    }


    MatrixReal MatrixReal::at(const Set& rowSet, const Set& columnSet) const {
        return atIIHelper(*this, rowSet, columnSet);
    }


    MatrixReal MatrixReal::at(const Set& rowSet, const Tuple& columnTuple) const {
        return atIIHelper(*this, rowSet, columnTuple);
    }


    MatrixReal MatrixReal::at(const Set& rowSet, const MatrixInteger& columnMatrix) const {
        return atIMHelper(*this, rowSet, columnMatrix);
    }


    MatrixReal MatrixReal::at(const Set& rowSet, const MatrixReal& columnMatrix) const {
        return atIMHelper(*this, rowSet, columnMatrix);
    }


    MatrixReal MatrixReal::at(const Set& rowSet, const MatrixComplex& columnMatrix) const {
        return atIMHelper(*this, rowSet, columnMatrix);
    }


    MatrixReal MatrixReal::at(MatrixReal::Index row, const Set& columnSet) const {
        return atCIHelper(*this, row, columnSet);
    }


    MatrixReal MatrixReal::at(Real row, const Set& columnSet) const {
        return at(inlineToInteger(row), columnSet);
    }


    MatrixReal MatrixReal::at(const Complex& row, const Set& columnSet) const {
        return at(inlineToInteger(row), columnSet);
    }


    MatrixReal MatrixReal::at(const Range& rowRange, const Set& columnSet) const {
        return atIIHelper(*this, rowRange, columnSet);
    }


    MatrixReal MatrixReal::at(const Tuple& rowTuple, const Set& columnSet) const {
        return atIIHelper(*this, rowTuple, columnSet);
    }


    MatrixReal MatrixReal::at(const MatrixInteger& rowMatrix, const Set& columnSet) const {
        return atMIHelper(*this, rowMatrix, columnSet);
    }


    MatrixReal MatrixReal::at(const MatrixReal& rowMatrix, const Set& columnSet) const {
        return atMIHelper(*this, rowMatrix, columnSet);
    }


    MatrixReal MatrixReal::at(const MatrixComplex& rowMatrix, const Set& columnSet) const {
        return atMIHelper(*this, rowMatrix, columnSet);
    }


    MatrixReal MatrixReal::at(const Set& indexSet) const {
        return atIHelper(*this, indexSet);
    }


    MatrixReal MatrixReal::at(const Tuple& rowTuple, MatrixReal::Index column) const {
        return atICHelper(*this, rowTuple, column);
    }


    MatrixReal MatrixReal::at(const Tuple& rowTuple, Real column) const {
        return at(rowTuple, inlineToInteger(column));
    }


    MatrixReal MatrixReal::at(const Tuple& rowTuple, const Complex& column) const {
        return at(rowTuple, inlineToInteger(column));
    }


    MatrixReal MatrixReal::at(const Tuple& rowTuple, const Range& columnRange) const {
        return atIIHelper(*this, rowTuple, columnRange);
    }


    MatrixReal MatrixReal::at(const Tuple& rowTuple, const Tuple& columnTuple) const {
        return atIIHelper(*this, rowTuple, columnTuple);
    }


    MatrixReal MatrixReal::at(const Tuple& rowTuple, const MatrixInteger& columnMatrix) const {
        return atIMHelper(*this, rowTuple, columnMatrix);
    }


    MatrixReal MatrixReal::at(const Tuple& rowTuple, const MatrixReal& columnMatrix) const {
        return atIMHelper(*this, rowTuple, columnMatrix);
    }


    MatrixReal MatrixReal::at(const Tuple& rowTuple, const MatrixComplex& columnMatrix) const {
        return atIMHelper(*this, rowTuple, columnMatrix);
    }


    MatrixReal MatrixReal::at(MatrixReal::Index row, const Tuple& columnTuple) const {
        return atCIHelper(*this, row, columnTuple);
    }


    MatrixReal MatrixReal::at(Real row, const Tuple& columnTuple) const {
        return at(inlineToInteger(row), columnTuple);
    }


    MatrixReal MatrixReal::at(const Complex& row, const Tuple& columnTuple) const {
        return at(inlineToInteger(row), columnTuple);
    }


    MatrixReal MatrixReal::at(const Range& rowRange, const Tuple& columnTuple) const {
        return atIIHelper(*this, rowRange, columnTuple);
    }


    MatrixReal MatrixReal::at(const MatrixInteger& rowMatrix, const Tuple& columnTuple) const {
        return atMIHelper(*this, rowMatrix, columnTuple);
    }


    MatrixReal MatrixReal::at(const MatrixReal& rowMatrix, const Tuple& columnTuple) const {
        return atMIHelper(*this, rowMatrix, columnTuple);
    }


    MatrixReal MatrixReal::at(const MatrixComplex& rowMatrix, const Tuple& columnTuple) const {
        return atMIHelper(*this, rowMatrix, columnTuple);
    }


    MatrixReal MatrixReal::at(const Tuple& indexTuple) const {
        return atIHelper(*this, indexTuple);
    }


    MatrixReal MatrixReal::at(const MatrixInteger& rowMatrix, MatrixReal::Index column) const {
        return atMCHelper(*this, rowMatrix, column);
    }


    MatrixReal MatrixReal::at(const MatrixInteger& rowMatrix, Real column) const {
        return at(rowMatrix, inlineToInteger(column));
    }


    MatrixReal MatrixReal::at(const MatrixInteger& rowMatrix, const Complex& column) const {
        return at(rowMatrix, inlineToInteger(column));
    }


    MatrixReal MatrixReal::at(const MatrixInteger& rowMatrix, const Range& columnRange) const {
        return atMIHelper(*this, rowMatrix, columnRange);
    }


    MatrixReal MatrixReal::at(const MatrixInteger& rowMatrix, const MatrixInteger& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixReal MatrixReal::at(const MatrixInteger& rowMatrix, const MatrixReal& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixReal MatrixReal::at(const MatrixInteger& rowMatrix, const MatrixComplex& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixReal MatrixReal::at(MatrixReal::Index row, const MatrixInteger& columnMatrix) const {
        return atCMHelper(*this, row, columnMatrix);
    }


    MatrixReal MatrixReal::at(Real row, const MatrixInteger& columnMatrix) const {
        return at(inlineToInteger(row), columnMatrix);
    }


    MatrixReal MatrixReal::at(const Complex& row, const MatrixInteger& columnMatrix) const {
        return at(inlineToInteger(row), columnMatrix);
    }


    MatrixReal MatrixReal::at(const Range& rowRange, const MatrixInteger& columnMatrix) const {
        return atIMHelper(*this, rowRange, columnMatrix);
    }


    MatrixReal MatrixReal::at(const MatrixReal& rowMatrix, const MatrixInteger& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixReal MatrixReal::at(const MatrixComplex& rowMatrix, const MatrixInteger& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixReal MatrixReal::at(const MatrixInteger& indexMatrix) const {
        return atMHelper(*this, indexMatrix);
    }


    MatrixReal MatrixReal::at(const MatrixReal& rowMatrix, MatrixReal::Index column) const {
        return atMCHelper(*this, rowMatrix, column);
    }


    MatrixReal MatrixReal::at(const MatrixReal& rowMatrix, Real column) const {
        return at(rowMatrix, inlineToInteger(column));
    }


    MatrixReal MatrixReal::at(const MatrixReal& rowMatrix, const Complex& column) const {
        return at(rowMatrix, inlineToInteger(column));
    }


    MatrixReal MatrixReal::at(const MatrixReal& rowMatrix, const Range& columnRange) const {
        return atMIHelper(*this, rowMatrix, columnRange);
    }


    MatrixReal MatrixReal::at(const MatrixReal& rowMatrix, const MatrixReal& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixReal MatrixReal::at(const MatrixReal& rowMatrix, const MatrixComplex& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixReal MatrixReal::at(MatrixReal::Index row, const MatrixReal& columnMatrix) const {
        return atCMHelper(*this, row, columnMatrix);
    }


    MatrixReal MatrixReal::at(Real row, const MatrixReal& columnMatrix) const {
        return at(inlineToInteger(row), columnMatrix);
    }


    MatrixReal MatrixReal::at(const Complex& row, const MatrixReal& columnMatrix) const {
        return at(inlineToInteger(row), columnMatrix);
    }


    MatrixReal MatrixReal::at(const Range& rowRange, const MatrixReal& columnMatrix) const {
        return atIMHelper(*this, rowRange, columnMatrix);
    }


    MatrixReal MatrixReal::at(const MatrixComplex& rowMatrix, const MatrixReal& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixReal MatrixReal::at(const MatrixReal& indexMatrix) const {
        return atMHelper(*this, indexMatrix);
    }


    MatrixReal MatrixReal::at(const MatrixComplex& rowMatrix, MatrixReal::Index column) const {
        return atMCHelper(*this, rowMatrix, column);
    }


    MatrixReal MatrixReal::at(const MatrixComplex& rowMatrix, Real column) const {
        return at(rowMatrix, inlineToInteger(column));
    }


    MatrixReal MatrixReal::at(const MatrixComplex& rowMatrix, const Complex& column) const {
        return at(rowMatrix, inlineToInteger(column));
    }


    MatrixReal MatrixReal::at(const MatrixComplex& rowMatrix, const Range& columnRange) const {
        return atMIHelper(*this, rowMatrix, columnRange);
    }


    MatrixReal MatrixReal::at(const MatrixComplex& rowMatrix, const MatrixComplex& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixReal MatrixReal::at(MatrixReal::Index row, const MatrixComplex& columnMatrix) const {
        return atCMHelper(*this, row, columnMatrix);
    }


    MatrixReal MatrixReal::at(Real row, const MatrixComplex& columnMatrix) const {
        return at(inlineToInteger(row), columnMatrix);
    }


    MatrixReal MatrixReal::at(const Complex& row, const MatrixComplex& columnMatrix) const {
        return at(inlineToInteger(row), columnMatrix);
    }


    MatrixReal MatrixReal::at(const Range& rowRange, const MatrixComplex& columnMatrix) const {
        return atIMHelper(*this, rowRange, columnMatrix);
    }


    MatrixReal MatrixReal::at(const MatrixComplex& indexMatrix) const {
        return atMHelper(*this, indexMatrix);
    }


    MatrixReal MatrixReal::at(const Range& rowRange, MatrixReal::Index column) const {
        return atICHelper(*this, rowRange, column);
    }


    MatrixReal MatrixReal::at(const Range& rowRange, Real column) const {
        return at(rowRange, inlineToInteger(column));
    }


    MatrixReal MatrixReal::at(const Range& rowRange, const Complex& column) const {
        return at(rowRange, inlineToInteger(column));
    }


    MatrixReal MatrixReal::at(const Range& rowRange, const Range& columnRange) const {
        return atIIHelper(*this, rowRange, columnRange);
    }


    MatrixReal MatrixReal::at(MatrixReal::Index row, const Range& columnRange) const {
        return atCIHelper(*this, row, columnRange);
    }


    MatrixReal MatrixReal::at(Real row, const Range& columnRange) const {
        return at(inlineToInteger(row), columnRange);
    }


    MatrixReal MatrixReal::at(const Complex& row, const Range& columnRange) const {
        return at(inlineToInteger(row), columnRange);
    }


    Variant MatrixReal::at(const Variant& row, MatrixReal::Index column) const {
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


    Variant MatrixReal::at(const Variant& row, Real column) const {
        return at(row, inlineToInteger(column));
    }


    Variant MatrixReal::at(const Variant& row, const Complex& column) const {
        return at(row, inlineToInteger(column));
    }


    Variant MatrixReal::at(const Variant& row, const Range& columnRange) const {
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


    Variant MatrixReal::at(const Variant& row, const Set& columnSet) const {
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


    Variant MatrixReal::at(const Variant& row, const Tuple& columnTuple) const {
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


    Variant MatrixReal::at(const Variant& row, const MatrixInteger& columnMatrix) const {
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


    Variant MatrixReal::at(const Variant& row, const MatrixReal& columnMatrix) const {
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


    Variant MatrixReal::at(const Variant& row, const MatrixComplex& columnMatrix) const {
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


    Variant MatrixReal::at(MatrixReal::Index row, const Variant& column) const {
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


    Variant MatrixReal::at(Real row, const Variant& column) const {
        return at(inlineToInteger(row), column);
    }


    Variant MatrixReal::at(const Complex& row, const Variant& column) const {
        return at(inlineToInteger(row), column);
    }


    Variant MatrixReal::at(const Range& rowRange, const Variant& column) const {
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


    Variant MatrixReal::at(const Set& rowSet, const Variant& column) const {
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


    Variant MatrixReal::at(const Tuple& rowTuple, const Variant& column) const {
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


    Variant MatrixReal::at(const MatrixInteger& rowMatrix, const Variant& column) const {
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


    Variant MatrixReal::at(const MatrixReal& rowMatrix, const Variant& column) const {
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


    Variant MatrixReal::at(const MatrixComplex& rowMatrix, const Variant& column) const {
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


    Variant MatrixReal::at(const Variant& row, const Variant& column) const {
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


    Variant MatrixReal::at(const Variant& index) const {
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


    const MatrixReal::Scalar* MatrixReal::pointer(MatrixReal::Index row, MatrixReal::Index column) const {
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


    void MatrixReal::update(Matrix::Index row, Matrix::Index column, MatrixReal::Scalar newValue) {
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


    void MatrixReal::update(Matrix::Index index, MatrixReal::Scalar newValue) {
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


    MatrixReal MatrixReal::combineLeftToRight(const MatrixReal& other) const {
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

        return MatrixReal(resultData);
    }


    MatrixReal MatrixReal::combineTopToBottom(const MatrixReal& other) const {
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

        return MatrixReal(resultData);
    }


    MatrixReal MatrixReal::columnReverse() const {
        applyLazyTransformsAndScaling();
        return MatrixReal(currentData->columnReverse());
    }


    MatrixReal MatrixReal::rowReverse() const {
        applyLazyTransformsAndScaling();
        return MatrixReal(currentData->rowReverse());
    }


    bool MatrixReal::isSquare() const {
        return currentData->numberRows() == currentData->numberColumns();
    }


    bool MatrixReal::isSymmetric(Real relativeTolerance) const {
        bool symmetric = true;

        if (isSquare()) {
            unsigned long numberRowColumns = currentData->numberRows();
            unsigned long rowIndex         = 0;

            while (symmetric && rowIndex < numberRowColumns) {
                unsigned long columnIndex = 0;

                while (symmetric && columnIndex < numberRowColumns) {
                    Scalar v1 = currentData->at(rowIndex, columnIndex);
                    Scalar v2 = currentData->at(columnIndex, rowIndex);

                    if (v1 != v2) {
                        Real average = abs(v1 + v2) / 2.0;
                        Real error   = abs(v1 - v2);
                        symmetric = error <= relativeTolerance * average;
                    }

                    ++columnIndex;
                }

                ++rowIndex;
            }
        } else {
            symmetric = false;
        }

        return symmetric;
    }


    bool MatrixReal::isSkewSymmetric(Real relativeTolerance) const {
        bool skewSymmetric = true;

        if (isSquare()) {
            unsigned long numberRowColumns = currentData->numberRows();
            unsigned long rowIndex         = 0;

            while (skewSymmetric && rowIndex < numberRowColumns) {
                unsigned long columnIndex = 0;

                while (skewSymmetric && columnIndex < numberRowColumns) {
                    Scalar v1 =  currentData->at(rowIndex, columnIndex);
                    Scalar v2 = -currentData->at(columnIndex, rowIndex);

                    if (v1 != v2) {
                        Real average = abs(v1 + v2) / 2.0;
                        Real error   = abs(v1 - v2);
                        skewSymmetric = error <= relativeTolerance * average;
                    }

                    ++columnIndex;
                }

                ++rowIndex;
            }
        } else {
            skewSymmetric = false;
        }

        return skewSymmetric;
    }


    bool MatrixReal::isNormal(Real relativeTolerance) const {
        bool normal = true;

        if (isSquare()) {
            MatrixReal ah = this->adjoint();

            MatrixReal p1 = ah * (*this);
            MatrixReal p2 = (*this) * ah;

            if (p1 != p2) {
                if (p1.numberRows() == p2.numberRows() && p1.numberColumns() == p2.numberColumns()) {
                    normal = true;
                    unsigned long column        = 1;
                    unsigned long numberRows    = p1.numberRows();
                    unsigned long numberColumns = p1.numberColumns();
                    while (normal && column <= numberColumns) {
                        unsigned long row = 1;
                        while (normal && row <= numberRows) {
                            Real v1 = p1.at(static_cast<Integer>(row), static_cast<Integer>(column));
                            Real v2 = p2.at(static_cast<Integer>(row), static_cast<Integer>(column));

                            if (v1 != v2) {
                                Real average = abs(v1 + v2) / 2.0;
                                Real error   = abs(v1 - v2);
                                normal = error <= relativeTolerance * average;
                            }

                            ++row;
                        }

                        ++column;
                    }
                }
            }
        } else {
            normal = false;
        }

        return normal;
    }


    Integer MatrixReal::rank(Real epsilon) const {
        applyLazyTransformsAndScaling();

        Data* uData;
        Data* sData;
        Data* vData;

        currentData->svd(uData, sData, vData);

        Data::destroy(uData);
        Data::destroy(vData);

        unsigned long diagnonalLength = std::min(currentData->numberRows(), currentData->numberColumns());
        Integer       result          = 0;

        for (unsigned index=0 ; index<diagnonalLength ; ++index) {
            Scalar v = sData->at(index, index);
            if (M::abs(v) > epsilon) {
                ++result;
            }
        }

        Data::destroy(sData);

        return result;
    }


    MatrixReal::Scalar MatrixReal::determinant() const {
        applyLazyTransformsAndScaling();
        return currentData->determinant();
    }


    MatrixReal MatrixReal::transpose() const {
        MatrixReal result(*this);
        result.pendingTranspose = !pendingTranspose;
        return result;
    }


    MatrixReal MatrixReal::inverse() const {
        applyLazyTransformsAndScaling();
        return MatrixReal(currentData->inverse());
    }


    Tuple MatrixReal::plu() const {
        Tuple result;

        applyLazyTransformsAndScaling();

        Data* pData;
        Data* lData;
        Data* uData;

        bool notSingular = currentData->plu(pData, lData, uData);

        result.append(MatrixReal(pData));
        result.append(MatrixReal(lData));
        result.append(MatrixReal(uData));
        result.append(Boolean(notSingular));

        return result;
    }


    Tuple MatrixReal::svd() const {
        Tuple result;

        applyLazyTransformsAndScaling();

        Data* uData;
        Data* sData;
        Data* vData;

        bool success = currentData->svd(uData, sData, vData);

        result.append(MatrixReal(uData));
        result.append(MatrixReal(sData));
        result.append(MatrixReal(vData));
        result.append(Model::Boolean(success));

        return result;
    }


    Tuple MatrixReal::qr() const {
        Tuple result;

        applyLazyTransformsAndScaling();

        Data* qData;
        Data* rData;

        bool success = currentData->qr(qData, rData);

        result.append(MatrixReal(qData));
        result.append(MatrixReal(rData));
        result.append(Boolean(success));

        return result;
    }


    Tuple MatrixReal::lq() const {
        Tuple result;

        Data* qData;
        Data* rData;
        bool  success;

        {
            MatrixReal ah = this->transpose();
            ah.applyLazyTransformsAndScaling();
            success = ah.currentData->qr(qData, rData);
        }

        MatrixReal l(rData);
        MatrixReal q(qData);

        l.pendingTranspose = true;
        q.pendingTranspose = true;

        result.append(l);
        result.append(q);
        result.append(Boolean(success));

        return result;
    }


    MatrixReal MatrixReal::cholesky() const {
        applyLazyTransformsAndScaling();
        return MatrixReal(currentData->cholesky());
    }


    MatrixReal MatrixReal::upperCholesky() const {
        applyLazyTransformsAndScaling();
        return MatrixReal(currentData->upperCholesky());
    }


    Tuple MatrixReal::hessenberg() const {
        Tuple result;

        applyLazyTransformsAndScaling();

        Data* qData;
        Data* hData;

        currentData->hessenberg(qData, hData);

        result.append(MatrixReal(qData));
        result.append(MatrixReal(hData));

        return result;
    }


    MatrixReal MatrixReal::dct() const {
        applyLazyTransformsAndScaling();
        return MatrixReal(currentData->dct());
    }


    MatrixReal MatrixReal::idct() const {
        applyLazyTransformsAndScaling();
        return MatrixReal(currentData->idct());
    }


    MatrixComplex MatrixReal::hilbertTransform() const {
        applyLazyTransformsAndScaling();
        return MatrixComplex(currentData->hilbertTransform());
    }


    Real MatrixReal::conditionNumber() const {
        applyLazyTransformsAndScaling();

        Data* inverse     = currentData->inverse();
        Real  inverseNorm = inverse->pNorm(2.0);
        Data::destroy(inverse);

        Real norm = currentData->pNorm(2.0);

        return inverseNorm * norm;
    }


    Real MatrixReal::pNorm(Integer p) const {
        applyLazyTransformsAndScaling();
        return currentData->pNorm(p);
    }


    Real MatrixReal::euclideanNorm() const {
        return pNorm(2.0);
    }


    Real MatrixReal::oneNorm() const {
        applyLazyTransformsAndScaling();
        return currentData->oneNorm();
    }


    Real MatrixReal::infinityNorm() const {
        applyLazyTransformsAndScaling();
        return currentData->infinityNorm();
    }


    Tuple MatrixReal::equilibrate() const {
        Tuple result;

        applyLazyTransformsAndScaling();

        Data* row;
        Data* column;

        bool  success = currentData->equilibrate(row, column);

        result.append(MatrixReal(row));
        result.append(MatrixReal(column));
        result.append(Boolean(success));

        return result;
    }


    MatrixReal MatrixReal::solve(const MatrixReal& y) const {
        Data* result = nullptr;

        applyLazyTransformsAndScaling();
        y.applyLazyTransformsAndScaling();

        if (y.currentData->matrixType() == MatrixType::DENSE) {
            result = currentData->solve(POLYMORPHIC_CAST<const DenseData*>(y.currentData));
        } else {
            assert(y.currentData->matrixType() == MatrixType::SPARSE);
            assert(false); // FIXME
//            result = currentData->solve(POLYMORPHIC_CAST<const SparseData*>(y.currentData));
        }

        return MatrixReal(result);
    }


    MatrixReal MatrixReal::leastSquares(const MatrixReal& b) const {
        Data* result = nullptr;

        applyLazyTransformsAndScaling();
        b.applyLazyTransformsAndScaling();

        if (b.currentData->matrixType() == MatrixType::DENSE) {
            result = currentData->leastSquares(POLYMORPHIC_CAST<const DenseData*>(b.currentData));
        } else {
            assert(b.currentData->matrixType() == MatrixType::SPARSE);
            assert(false); // FIXME
//            result = currentData->leastSquares(POLYMORPHIC_CAST<const SparseData*>(b.currentData));
        }

        return MatrixReal(result);
    }


    MatrixReal MatrixReal::hadamard(const MatrixReal& other) const {
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

        return MatrixReal(result);
    }


    MatrixReal MatrixReal::kronecker(const MatrixReal& other) const {
        applyLazyTransformsAndScaling();

        Data* result = nullptr;
        if (other.currentData->matrixType() == MatrixType::DENSE) {
            result = currentData->kronecker(
                *POLYMORPHIC_CAST<const DenseData*>(other.currentData),
                other.pendingTranspose,
                other.pendingScalarValue
            );
        } else {
            assert(other.currentData->matrixType() == MatrixType::SPARSE);
            assert(false); // FIXME
//            result = currentData->hadamard(*POLYMORPHIC_CAST<const SparseData*>(other.currentData));
        }

        return MatrixReal(result);
    }


    MatrixReal::MatrixType MatrixReal::matrixType() const {
        return currentData->matrixType();
    }


    MatrixReal::Scalar* MatrixReal::data() {
        applyLazyTransformsAndScaling();
        return currentData->data();
    }


    const MatrixReal::Scalar* MatrixReal::data() const {
        applyLazyTransformsAndScaling();
        return currentData->data();
    }


    MatrixReal MatrixReal::floor() const {
        MatrixReal result(currentData->floor());

        result.pendingTranspose   = pendingTranspose;
        result.pendingScalarValue = pendingScalarValue;

        return result;
    }


    MatrixReal MatrixReal::ceil() const {
        MatrixReal result(currentData->ceil());

        result.pendingTranspose   = pendingTranspose;
        result.pendingScalarValue = pendingScalarValue;

        return result;
    }


    MatrixReal MatrixReal::nint() const {
        MatrixReal result(currentData->nint());

        result.pendingTranspose   = pendingTranspose;
        result.pendingScalarValue = pendingScalarValue;

        return result;
    }


    MatrixInteger MatrixReal::truncateToInteger() const {
        applyLazyTransformsAndScaling();
        return MatrixInteger(currentData->truncateToInteger());
    }


    MatrixInteger MatrixReal::floorToInteger() const {
        applyLazyTransformsAndScaling();
        return MatrixInteger(currentData->floorToInteger());
    }


    MatrixInteger MatrixReal::ceilToInteger() const {
        applyLazyTransformsAndScaling();
        return MatrixInteger(currentData->ceilToInteger());
    }


    MatrixInteger MatrixReal::nintToInteger() const {
        applyLazyTransformsAndScaling();
        return MatrixInteger(currentData->nintToInteger());
    }


    MatrixReal MatrixReal::randomInclusive(PerThread& pt, Integer numberRows, Integer numberColumns) {
        return MatrixReal(DenseData::randomInclusive(pt, numberRows, numberColumns));
    }


    MatrixReal MatrixReal::randomInclusiveExclusive(PerThread& pt, Integer numberRows, Integer numberColumns) {
        return MatrixReal(DenseData::randomInclusiveExclusive(pt, numberRows, numberColumns));
    }


    MatrixReal MatrixReal::randomExclusiveInclusive(PerThread& pt, Integer numberRows, Integer numberColumns) {
        return MatrixReal(DenseData::randomExclusiveInclusive(pt, numberRows, numberColumns));
    }


    MatrixReal MatrixReal::randomExclusive(PerThread& pt, Integer numberRows, Integer numberColumns) {
        return MatrixReal(DenseData::randomExclusive(pt, numberRows, numberColumns));
    }


    MatrixReal MatrixReal::randomNormal(PerThread& pt, Integer numberRows, Integer numberColumns) {
        return MatrixReal(DenseData::randomNormal(pt, numberRows, numberColumns));
    }


    MatrixReal MatrixReal::randomNormal(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns,
            Real       mean,
            Real       sigma
        ) {
        return MatrixReal(DenseData::randomNormal(pt, numberRows, numberColumns, mean, sigma));
    }


    MatrixReal MatrixReal::randomWeibull(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns,
            Real       scale,
            Real       shape,
            Real       delay
        ) {
        if (scale <= 0.0 || shape <= 0.0) {
            throw Model::InvalidNumericValue();
        }

        return MatrixReal(DenseData::randomWeibull(pt, numberRows, numberColumns, scale, shape, delay));
    }


    MatrixReal MatrixReal::randomExponential(PerThread& pt, Integer numberRows, Integer numberColumns, Real rate) {
        if (rate <= 0.0) {
            throw Model::InvalidNumericValue();
        }

        return MatrixReal(DenseData::randomExponential(pt, numberRows, numberColumns, rate));
    }


    MatrixReal MatrixReal::randomGamma(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns,
            Real       k,
            Real       s
        ) {
        if (k <= 0 || s <= 0) {
            throw Model::InvalidNumericValue();
        }

        return MatrixReal(DenseData::randomGamma(pt, numberRows, numberColumns, k, s));
    }


    MatrixReal MatrixReal::randomRayleigh(PerThread& pt, Integer numberRows, Integer numberColumns, Real scale) {
        if (scale <= 0) {
            throw Model::InvalidNumericValue();
        }

        return MatrixReal(DenseData::randomRayleigh(pt, numberRows, numberColumns, scale));
    }


    MatrixReal MatrixReal::randomChiSquared(PerThread& pt, Integer numberRows, Integer numberColumns, Integer k) {
        if (k <= 0) {
            throw Model::InvalidNumericValue();
        }

        return MatrixReal(DenseData::randomChiSquared(pt, numberRows, numberColumns, k));
    }


    MatrixReal MatrixReal::randomLogNormal(PerThread& pt, Integer numberRows, Integer numberColumns) {
        return MatrixReal(DenseData::randomLogNormal(pt, numberRows, numberColumns));
    }


    MatrixReal MatrixReal::randomLogNormal(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns,
            Real       mean,
            Real       sigma
        ) {
        return MatrixReal(DenseData::randomLogNormal(pt, numberRows, numberColumns, mean, sigma));
    }


    MatrixReal MatrixReal::randomCauchyLorentz(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns,
            Real       location,
            Real       scale
        ) {
        if (scale <= 0) {
            throw Model::InvalidNumericValue();
        }

        return MatrixReal(DenseData::randomCauchyLorentz(pt, numberRows, numberColumns, location, scale));
    }


    MatrixReal& MatrixReal::operator=(const MatrixReal& other) {
        assignExistingDataStore(other.currentData, false);

        pendingTranspose   = other.pendingTranspose;
        pendingScalarValue = other.pendingScalarValue;

        return *this;
    }


    MatrixReal& MatrixReal::operator=(const MatrixComplex& other) {
        releaseDataStore(currentData);
        copyFrom(other);

        return *this;
    }


    MatrixReal& MatrixReal::operator=(const MatrixInteger& other) {
        releaseDataStore(currentData);
        copyFrom(other);

        return *this;
    }


    MatrixReal& MatrixReal::operator=(const MatrixBoolean& other) {
        releaseDataStore(currentData);
        copyFrom(other);

        return *this;
    }


    MatrixReal& MatrixReal::operator=(const Variant& other) {
        bool       ok;
        MatrixReal v = other.toMatrixReal(&ok);

        if (ok) {
            assignExistingDataStore(v.currentData, false);

            pendingTranspose   = v.pendingTranspose;
            pendingScalarValue = v.pendingScalarValue;
        } else {
            throw Model::InvalidRuntimeConversion(other.valueType(), ValueType::MATRIX_REAL);
        }

        return *this;
    }


    MatrixReal& MatrixReal::operator+=(const MatrixReal& v) {
        currentData->lock();
        v.currentData->lock();

        Data* newDataStore = nullptr;
        if (v.currentData->matrixType() == MatrixType::DENSE) {
            newDataStore = currentData->add(
                *POLYMORPHIC_CAST<const DenseData*>(v.currentData),
                pendingTranspose,
                v.pendingTranspose,
                pendingScalarValue,
                v.pendingScalarValue
            );
        } else {
            assert(v.currentData->matrixType() == MatrixType::SPARSE);
            assert(false);
//            newDataStore = currentData->add(
//                *POLYMORPHIC_CAST<const SparseData*>(v.currentData),
//                pendingTransform,
//                v.pendingTransform,
//                pendingScalarValue,
//                v.pendingScalarValue
//            );
        }

        v.currentData->unlock();
        assignNewDataStore(newDataStore, true);

        pendingTranspose   = false;
        pendingScalarValue = Scalar(1);

        return *this;
    }


    MatrixReal& MatrixReal::operator-=(const MatrixReal& v) {
        currentData->lock();
        v.currentData->lock();

        Data* newDataStore = nullptr;
        if (v.currentData->matrixType() == MatrixType::DENSE) {
            newDataStore = currentData->add(
                *POLYMORPHIC_CAST<const DenseData*>(v.currentData),
                pendingTranspose,
                v.pendingTranspose,
                pendingScalarValue,
                -v.pendingScalarValue
            );
        } else {
            assert(v.currentData->matrixType() == MatrixType::SPARSE);
            assert(false);
//            newDataStore = currentData->add(
//                *POLYMORPHIC_CAST<const SparseData*>(v.currentData),
//                pendingTransform,
//                v.pendingTransform,
//                pendingScalarValue,
//                -v.pendingScalarValue
//            );
        }

        v.currentData->unlock();
        assignNewDataStore(newDataStore, true);

        pendingTranspose   = false;
        pendingScalarValue = Scalar(1);

        return *this;
    }


    MatrixReal& MatrixReal::operator*=(const MatrixReal& v) {
        Data* newDataStore;

        Real  scalarMultiplier = pendingScalarValue * v.pendingScalarValue;
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

        pendingTranspose   = false;
        pendingScalarValue = Scalar(1);

        return *this;
    }


    MatrixReal& MatrixReal::operator*=(Boolean v) {
        if (v == false) {
            pendingScalarValue = Scalar(0);
        }

        return *this;
    }


    MatrixReal& MatrixReal::operator*=(Integer v) {
        pendingScalarValue *= Scalar(v);
        return *this;
    }


    MatrixReal& MatrixReal::operator*=(Real v) {
        pendingScalarValue *= Scalar(v);
        return *this;
    }


    MatrixReal MatrixReal::operator+(const MatrixBoolean& v) const {
        return operator+(MatrixReal(v));
    }


    MatrixReal MatrixReal::operator+(const MatrixInteger& v) const {
        return operator+(MatrixReal(v));
    }


    MatrixReal MatrixReal::operator+(const MatrixReal& v) const {
        currentData->lock();
        v.currentData->lock();

        Data* newDataStore = nullptr;
        if (v.currentData->matrixType() == MatrixType::DENSE) {
            newDataStore = currentData->add(
                *POLYMORPHIC_CAST<const DenseData*>(v.currentData),
                pendingTranspose,
                v.pendingTranspose,
                pendingScalarValue,
                v.pendingScalarValue
            );
        } else {
            assert(v.currentData->matrixType() == MatrixType::SPARSE);
            assert(false); // FIXME
//            newDataStore = currentData->add(
//                *POLYMORPHIC_CAST<const SparseData*>(v.currentData),
//                pendingTransform,
//                v.pendingTransform,
//                pendingScalarValue,
//                v.pendingScalarValue
//            );
        }

        v.currentData->unlock();
        currentData->unlock();

        return MatrixReal(newDataStore);
    }


    MatrixReal MatrixReal::operator-(const MatrixBoolean& v) const {
        return operator-(MatrixReal(v));
    }


    MatrixReal MatrixReal::operator-(const MatrixInteger& v) const {
        return operator-(MatrixReal(v));
    }


    MatrixReal MatrixReal::operator-(const MatrixReal& v) const {
        currentData->lock();
        v.currentData->lock();

        Data* newDataStore = nullptr;
        if (v.currentData->matrixType() == MatrixType::DENSE) {
            newDataStore = currentData->add(
                *POLYMORPHIC_CAST<const DenseData*>(v.currentData),
                pendingTranspose,
                v.pendingTranspose,
                pendingScalarValue,
                -v.pendingScalarValue
            );
        } else {
            assert(v.currentData->matrixType() == MatrixType::SPARSE);
            assert(false);
//            newDataStore = currentData->add(
//                *POLYMORPHIC_CAST<const SparseData*>(v.currentData),
//                pendingTransform,
//                v.pendingTransform,
//                pendingScalarValue,
//                -v.pendingScalarValue
//            );
        }

        v.currentData->unlock();
        currentData->unlock();

        return MatrixReal(newDataStore);
    }


    MatrixReal MatrixReal::operator*(const MatrixBoolean& v) const {
        return operator*(MatrixReal(v));
    }


    MatrixReal MatrixReal::operator*(const MatrixInteger& v) const {
        return operator*(MatrixReal(v));
    }


    MatrixReal MatrixReal::operator*(const MatrixReal& v) const {
        Data* result;

        Real  scalarMultiplier = pendingScalarValue * v.pendingScalarValue;
        Data* multiplicand     = v.currentData;

        if (multiplicand->matrixType() == MatrixType::DENSE) {
            Scalar scalarMultiplier = pendingScalarValue * v.pendingScalarValue;
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

        return MatrixReal(result);
    }


    MatrixReal MatrixReal::operator*(Boolean v) const {
        MatrixReal result(*this);

        if (v == false) {
            result.pendingScalarValue = Scalar(0);
        }

        return result;
    }


    MatrixReal MatrixReal::operator*(Integer v) const {
        MatrixReal result(*this);
        result.pendingScalarValue *= Scalar(v);

        return result;
    }


    MatrixReal MatrixReal::operator*(Real v) const {
        MatrixReal result(*this);
        result.pendingScalarValue *= Scalar(v);

        return result;
    }


    MatrixReal MatrixReal::operator/(Boolean v) const {
        MatrixReal result(*this);
        result.pendingScalarValue /= Scalar(v);

        return result;
    }


    MatrixReal MatrixReal::operator/(Integer v) const {
        MatrixReal result(*this);
        result.pendingScalarValue /= Scalar(v);

        return result;
    }


    MatrixReal MatrixReal::operator/(Real v) const {
        MatrixReal result(*this);
        result.pendingScalarValue /= Scalar(v);

        return result;
    }


    MatrixReal MatrixReal::operator-() const {
        MatrixReal result(*this);
        result.pendingScalarValue *= Scalar(-1);

        return result;
    }


    bool MatrixReal::operator==(const MatrixReal& other) const {
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


    int MatrixReal::relativeOrder(const MatrixReal& other) const {
        int result;

        if (currentData == other.currentData) {
            if (pendingTranspose == other.pendingTranspose) {
                if (pendingScalarValue < other.pendingScalarValue) {
                    result = -1;
                } else if (pendingScalarValue > other.pendingScalarValue) {
                    result = +1;
                } else /* if (pendingScalarValue.real() == other.pendingScalarValue.real()) */ {
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


    MatrixReal::MatrixReal(
            Data* dataStore
        ):currentData(
            dataStore
        ),pendingTranspose(
            false
        ),pendingScalarValue(
            1.0
        ) {}


    void MatrixReal::copyFrom(const MatrixBoolean& booleanMatrix) {
        MatrixBoolean::Data* otherData = booleanMatrix.currentData;

        if (otherData->matrixType() == MatrixType::DENSE) {
            currentData = DenseData::create(*POLYMORPHIC_CAST<MatrixBoolean::DenseData*>(otherData));
        } else {
            assert(otherData->matrixType() == MatrixType::SPARSE);
            assert(false); // FIXME
        }

        pendingTranspose   = booleanMatrix.pendingTranspose;
        pendingScalarValue = Scalar(1);
    }


    void MatrixReal::copyFrom(const MatrixInteger& integerMatrix) {
        if (integerMatrix.currentData->matrixType() == MatrixType::DENSE) {
            const MatrixInteger::DenseData*
                data = POLYMORPHIC_CAST<const MatrixInteger::DenseData*>(integerMatrix.currentData);

            currentData        = DenseData::create(*data);
            pendingTranspose   = integerMatrix.pendingTranspose;
            pendingScalarValue = Real(integerMatrix.pendingScalarMultiplierValue);
        } else {
            assert(integerMatrix.currentData->matrixType() == MatrixType::SPARSE);
            assert(false); // FIXME
        }
    }


    void MatrixReal::copyFrom(const MatrixComplex& complexMatrix) {
        complexMatrix.applyLazyTransformsAndScaling();

        MatrixComplex::Data* otherData = complexMatrix.currentData;

        if (otherData->matrixType() == MatrixType::DENSE) {
            currentData = DenseData::create(*POLYMORPHIC_CAST<MatrixComplex::DenseData*>(otherData));
        } else {
            assert(otherData->matrixType() == MatrixType::SPARSE);
            assert(false); // FIXME
        }

        pendingTranspose   = false;
        pendingScalarValue = Scalar(1);
    }


    void MatrixReal::releaseDataStore(MatrixReal::Data* dataStore) const {
        if (dataStore->removeReference()) {
            Data::destroy(dataStore);
        }
    }


    void MatrixReal::assignNewDataStore(MatrixReal::Data* newDataStore, bool unlock) const {
        Data* oldData = currentData;
        currentData = newDataStore;

        if (unlock) {
            oldData->unlock();
        }

        releaseDataStore(oldData);
    }


    void MatrixReal::assignExistingDataStore(MatrixReal::Data* newDataStore, bool unlock) const {
        newDataStore->addReference();
        assignNewDataStore(newDataStore, unlock);
    }


    void MatrixReal::applyLazyTransformsAndScaling() const {
        if (pendingScalarValue != Scalar(1) || pendingTranspose) {
            currentData->lock();
            Data* newDataStore = currentData->applyTransformAndScaling(pendingTranspose, pendingScalarValue);
            assignNewDataStore(newDataStore, true);

            pendingTranspose   = false;
            pendingScalarValue = Scalar(1);
        }
    }


    MatrixReal operator*(Boolean a, const MatrixReal& b) {
        return b * Integer(a);
    }


    MatrixReal operator*(Integer a, const MatrixReal& b) {
        return b * a;
    }


    MatrixReal operator*(Real a, const MatrixBoolean& b) {
        return MatrixReal(b) * a;
    }


    MatrixReal operator*(Real a, const MatrixInteger& b) {
        return MatrixReal(b) * a;
    }


    MatrixReal operator*(Real a, const MatrixReal& b) {
        return b * a;
    }


    MatrixReal operator*(const MatrixBoolean& a, Real b) {
        return MatrixReal(a) * b;
    }


    MatrixReal operator*(const MatrixInteger& a, Real b) {
        return MatrixReal(a) * b;
    }


    MatrixReal operator*(const MatrixBoolean& a, const MatrixReal& b) {
        return MatrixReal(a) * b;
    }


    MatrixReal operator*(const MatrixInteger& a, const MatrixReal& b) {
        return MatrixReal(a) * b;
    }


    MatrixReal operator/(const MatrixBoolean& a, Integer b) {
        return MatrixReal(a) / Real(b);
    }


    MatrixReal operator/(const MatrixBoolean& a, Real b) {
        return MatrixReal(a) / b;
    }


    MatrixReal operator/(const MatrixInteger& a, Integer b) {
        return MatrixReal(a) / Real(b);
    }


    MatrixReal operator/(const MatrixInteger& a, Real b) {
        return MatrixReal(a) / b;
    }


    MatrixComplex operator/(const MatrixComplex& a, Boolean b) {
        M::Integer bi = b ? 1 : 0;
        return a / bi;
    }


    MatrixReal operator+(const MatrixBoolean& a, const MatrixReal& b) {
        return MatrixReal(a) + b;
    }


    MatrixReal operator+(const MatrixInteger& a, const MatrixReal& b) {
        return MatrixReal(a) + b;
    }


    MatrixReal operator-(const MatrixBoolean& a, const MatrixReal& b) {
        return MatrixReal(a) - b;
    }


    MatrixReal operator-(const MatrixInteger& a, const MatrixReal& b) {
        return MatrixReal(a) - b;
    }
}
