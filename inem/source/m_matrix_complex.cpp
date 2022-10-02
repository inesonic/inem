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
* This file implements the \ref M::MatrixComplex class.
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
#include "model_complex.h"
#include "model_tuple.h"
#include "model_matrix.h"
#include "m_basic_functions.h"
#include "m_matrix_helpers.h"
#include "m_matrix_boolean.h"
#include "m_matrix_integer.h"
#include "m_matrix_integer_data.h"
#include "m_matrix_integer_dense_data.h"
#include "m_matrix_integer_sparse_data.h"
#include "m_matrix_real.h"
#include "m_matrix_real_data.h"
#include "m_matrix_real_dense_data.h"
#include "m_matrix_complex.h"
#include "m_matrix_complex_data.h"
#include "m_matrix_complex_dense_data.h"
#include "m_matrix_complex_sparse_data.h"

namespace M {
    const MatrixComplex::Scalar MatrixComplex::zeroValue(0);

    MatrixComplex::MatrixComplex(Integer newNumberRows, Integer newNumberColumns) {
        if (newNumberRows < 0) {
            throw Model::InvalidRow(newNumberRows, 0);
        } else if (newNumberColumns < 0) {
            throw Model::InvalidColumn(newNumberColumns, 0);
        } else {
            currentData = DenseData::create(newNumberRows, newNumberColumns);
            pendingTransform   = LazyTransform::NONE;
            pendingScalarValue = Complex(1, 0);
        }
    }


    MatrixComplex::MatrixComplex(
            Integer                      newNumberRows,
            Integer                      newNumberColumns,
            const MatrixComplex::Scalar* matrixData
        ) {
        if (newNumberRows < 0) {
            throw Model::InvalidRow(newNumberRows, 0);
        } else if (newNumberColumns < 0) {
            throw Model::InvalidColumn(newNumberColumns, 0);
        } else {
            currentData = DenseData::create(newNumberRows, newNumberColumns, matrixData);
            pendingTransform   = LazyTransform::NONE;
            pendingScalarValue = Complex(1, 0);
        }
    }


    MatrixComplex::MatrixComplex(const MatrixBoolean& other) {
        copyFrom(other);
    }


    MatrixComplex::MatrixComplex(const MatrixInteger& other) {
        copyFrom(other);
    }


    MatrixComplex::MatrixComplex(const MatrixReal& other) {
        copyFrom(other);
    }


    MatrixComplex::MatrixComplex(const MatrixComplex& other) {
        other.currentData->addReference();
        currentData = other.currentData;

        pendingTransform   = other.pendingTransform;
        pendingScalarValue = other.pendingScalarValue;
    }


    MatrixComplex::MatrixComplex(const Variant& other) {
        bool          ok;
        MatrixComplex v = other.toMatrixComplex(&ok);

        if (ok) {
            v.currentData->addReference();
            currentData = v.currentData;

            pendingTransform   = v.pendingTransform;
            pendingScalarValue = v.pendingScalarValue;
        } else {
            throw Model::InvalidRuntimeConversion(other.valueType(), ValueType::MATRIX_COMPLEX);
        }
    }


    MatrixComplex::~MatrixComplex() {
        releaseDataStore(currentData);
    }


    ValueType MatrixComplex::coefficientValueType() const {
        return ValueType::COMPLEX;
    }


    bool MatrixComplex::toFile(const char* filename, Model::DataFileFormat fileFormat) const {
        return currentData->toFile(filename, fileFormat);
    }


    MatrixComplex MatrixComplex::fromFile(const char* filename) {
        return MatrixComplex(DenseData::fromFile(filename));
    }


    MatrixComplex::Index MatrixComplex::numberRows() const {
        Index result;

        if (pendingTransform != LazyTransform::ADJOINT && pendingTransform != LazyTransform::TRANSPOSE) {
            result = currentData->numberRows();
        } else {
            result = currentData->numberColumns();
        }

        return result;
    }


    MatrixComplex::Index MatrixComplex::numberColumns() const {
        Index result;

        if (pendingTransform != LazyTransform::ADJOINT && pendingTransform != LazyTransform::TRANSPOSE) {
            result = currentData->numberColumns();
        } else {
            result = currentData->numberRows();
        }

        return result;
    }


    MatrixComplex::Index MatrixComplex::numberCoefficients() const {
        return currentData->numberRows() * currentData->numberColumns();
    }


    Model::Variant MatrixComplex::value(MatrixComplex::Index row, MatrixComplex::Index column) const {
        Model::Variant result;

        if (row > 0 && static_cast<unsigned long>(row) <= currentData->numberRows()          &&
            column > 0 && static_cast<unsigned long>(column) <= currentData->numberColumns()    ) {
            result = currentData->at(row - 1, column - 1);
        }

        return result;
    }


    MatrixComplex MatrixComplex::identity(Index numberRows, Index numberColumns) {
        return MatrixComplex(DenseData::identity(numberRows, numberColumns));
    }


    MatrixComplex MatrixComplex::identity(Index numberRowColumns) {
        return MatrixComplex(DenseData::identity(numberRowColumns, numberRowColumns));
    }


    MatrixComplex MatrixComplex::ones(Index numberRows, Index numberColumns) {
        return MatrixComplex(DenseData::ones(numberRows, numberColumns));
    }


    MatrixComplex MatrixComplex::ones(Index numberRowColumns) {
        return MatrixComplex(DenseData::ones(numberRowColumns, numberRowColumns));
    }


    MatrixComplex MatrixComplex::diagonalEntries() const {
        unsigned long numberDiagnonalEntries = std::min(currentData->numberRows(), currentData->numberColumns());
        MatrixComplex result(numberDiagnonalEntries, 1);

        Scalar* data = result.currentData->data();

        for (unsigned index=0 ; index<numberDiagnonalEntries ; ++index) {
            Scalar v = currentData->at(index, index);
            data[index] = v;
        }

        return result;
    }


    MatrixComplex MatrixComplex::diagonal() const {
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

        return MatrixComplex(result);
    }


    Model::Variant MatrixComplex::value(Matrix::Index index) const {
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


    bool MatrixComplex::setValue(MatrixComplex::Index row, MatrixComplex::Index column, const Model::Variant& value) {
        bool success;

        if (row > 0 && column > 0 && value.valueType() != ValueType::BOOLEAN) {
            Complex complexValue = value.toComplex(&success);

            if (success) {
                update(row, column, complexValue);
            }
        } else {
            success = false;
        }

        return success;
    }


    bool MatrixComplex::setValue(MatrixComplex::Index index, const Model::Variant& value) {
        bool success;

        if (index > 0 && value.valueType() != ValueType::BOOLEAN) {
            Complex complexValue = value.toComplex(&success);

            if (success) {
                update(index, complexValue);
            }
        } else {
            success = false;
        }

        return success;
    }


    void MatrixComplex::resize(MatrixComplex::Index newNumberRows, MatrixComplex::Index newNumberColumns) {
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


    const MatrixComplex::Scalar& MatrixComplex::at(
            MatrixComplex::Index row,
            MatrixComplex::Index column
        ) const {
        return *pointer(row, column);
    }


    const MatrixComplex::Scalar& MatrixComplex::at(MatrixComplex::Index index) const {
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


    MatrixComplex::Scalar MatrixComplex::at(Real row, MatrixComplex::Index column) const {
        Index r;
        return toInteger(r, row) ? at(r, column) : Scalar(0);
    }


    MatrixComplex::Scalar MatrixComplex::at(const Complex& row, MatrixComplex::Index column) const {
        Index r;
        return toInteger(r, row) ? at(r, column) : Scalar(0);
    }


    MatrixComplex::Scalar MatrixComplex::at(MatrixComplex::Index row, Real column) const {
        Index c;
        return toInteger(c, column) ? at(row, c) : Scalar(0);
    }


    MatrixComplex::Scalar MatrixComplex::at(MatrixComplex::Index row, const Complex& column) const {
        Index c;
        return toInteger(c, column) ? at(row, c) : Scalar(0);
    }


    MatrixComplex::Scalar MatrixComplex::at(Real row, Real column) const {
        Index r;
        Index c;
        return (toInteger(r, row) && toInteger(c, column)) ? at(r, c) : Scalar(0);
    }


    MatrixComplex::Scalar MatrixComplex::at(const Complex& row, Real column) const {
        Index r;
        Index c;
        return (toInteger(r, row) && toInteger(c, column)) ? at(r, c) : Scalar(0);
    }


    MatrixComplex::Scalar MatrixComplex::at(Real row, const Complex& column) const {
        Index r;
        Index c;
        return (toInteger(r, row) && toInteger(c, column)) ? at(r, c) : Scalar(0);
    }


    MatrixComplex::Scalar MatrixComplex::at(const Complex& row, const Complex& column) const {
        Index r;
        Index c;
        return (toInteger(r, row) && toInteger(c, column)) ? at(r, c) : Scalar(0);
    }


    MatrixComplex::Scalar MatrixComplex::at(Real index) const {
        Index i;
        return toInteger(i, index) ? at(i) : Scalar(0);
    }


    MatrixComplex::Scalar MatrixComplex::at(const Complex& index) const {
        Index i;
        return toInteger(i, index) ? at(i) : Scalar(0);
    }


    MatrixComplex MatrixComplex::at(const Set& rowSet, MatrixComplex::Index column) const {
        return atICHelper(*this, rowSet, column);
    }


    MatrixComplex MatrixComplex::at(const Set& rowSet, Real column) const {
        return at(rowSet, inlineToInteger(column));
    }


    MatrixComplex MatrixComplex::at(const Set& rowSet, const Complex& column) const {
        return at(rowSet, inlineToInteger(column));
    }


    MatrixComplex MatrixComplex::at(const Set& rowSet, const Range& column) const {
        return atIIHelper(*this, rowSet, column);
    }


    MatrixComplex MatrixComplex::at(const Set& rowSet, const Set& columnSet) const {
        return atIIHelper(*this, rowSet, columnSet);
    }


    MatrixComplex MatrixComplex::at(const Set& rowSet, const Tuple& columnTuple) const {
        return atIIHelper(*this, rowSet, columnTuple);
    }


    MatrixComplex MatrixComplex::at(const Set& rowSet, const MatrixInteger& columnMatrix) const {
        return atIMHelper(*this, rowSet, columnMatrix);
    }


    MatrixComplex MatrixComplex::at(const Set& rowSet, const MatrixReal& columnMatrix) const {
        return atIMHelper(*this, rowSet, columnMatrix);
    }


    MatrixComplex MatrixComplex::at(const Set& rowSet, const MatrixComplex& columnMatrix) const {
        return atIMHelper(*this, rowSet, columnMatrix);
    }


    MatrixComplex MatrixComplex::at(MatrixComplex::Index row, const Set& columnSet) const {
        return atCIHelper(*this, row, columnSet);
    }


    MatrixComplex MatrixComplex::at(Real row, const Set& columnSet) const {
        return at(inlineToInteger(row), columnSet);
    }


    MatrixComplex MatrixComplex::at(const Complex& row, const Set& columnSet) const {
        return at(inlineToInteger(row), columnSet);
    }


    MatrixComplex MatrixComplex::at(const Range& rowRange, const Set& columnSet) const {
        return atIIHelper(*this, rowRange, columnSet);
    }


    MatrixComplex MatrixComplex::at(const Tuple& rowTuple, const Set& columnSet) const {
        return atIIHelper(*this, rowTuple, columnSet);
    }


    MatrixComplex MatrixComplex::at(const MatrixInteger& rowMatrix, const Set& columnSet) const {
        return atMIHelper(*this, rowMatrix, columnSet);
    }


    MatrixComplex MatrixComplex::at(const MatrixReal& rowMatrix, const Set& columnSet) const {
        return atMIHelper(*this, rowMatrix, columnSet);
    }


    MatrixComplex MatrixComplex::at(const MatrixComplex& rowMatrix, const Set& columnSet) const {
        return atMIHelper(*this, rowMatrix, columnSet);
    }


    MatrixComplex MatrixComplex::at(const Set& indexSet) const {
        return atIHelper(*this, indexSet);
    }


    MatrixComplex MatrixComplex::at(const Tuple& rowTuple, MatrixComplex::Index column) const {
        return atICHelper(*this, rowTuple, column);
    }


    MatrixComplex MatrixComplex::at(const Tuple& rowTuple, Real column) const {
        return at(rowTuple, inlineToInteger(column));
    }


    MatrixComplex MatrixComplex::at(const Tuple& rowTuple, const Complex& column) const {
        return at(rowTuple, inlineToInteger(column));
    }


    MatrixComplex MatrixComplex::at(const Tuple& rowTuple, const Range& columnRange) const {
        return atIIHelper(*this, rowTuple, columnRange);
    }


    MatrixComplex MatrixComplex::at(const Tuple& rowTuple, const Tuple& columnTuple) const {
        return atIIHelper(*this, rowTuple, columnTuple);
    }


    MatrixComplex MatrixComplex::at(const Tuple& rowTuple, const MatrixInteger& columnMatrix) const {
        return atIMHelper(*this, rowTuple, columnMatrix);
    }


    MatrixComplex MatrixComplex::at(const Tuple& rowTuple, const MatrixReal& columnMatrix) const {
        return atIMHelper(*this, rowTuple, columnMatrix);
    }


    MatrixComplex MatrixComplex::at(const Tuple& rowTuple, const MatrixComplex& columnMatrix) const {
        return atIMHelper(*this, rowTuple, columnMatrix);
    }


    MatrixComplex MatrixComplex::at(MatrixComplex::Index row, const Tuple& columnTuple) const {
        return atCIHelper(*this, row, columnTuple);
    }


    MatrixComplex MatrixComplex::at(Real row, const Tuple& columnTuple) const {
        return at(inlineToInteger(row), columnTuple);
    }


    MatrixComplex MatrixComplex::at(const Complex& row, const Tuple& columnTuple) const {
        return at(inlineToInteger(row), columnTuple);
    }


    MatrixComplex MatrixComplex::at(const Range& rowRange, const Tuple& columnTuple) const {
        return atIIHelper(*this, rowRange, columnTuple);
    }


    MatrixComplex MatrixComplex::at(const MatrixInteger& rowMatrix, const Tuple& columnTuple) const {
        return atMIHelper(*this, rowMatrix, columnTuple);
    }


    MatrixComplex MatrixComplex::at(const MatrixReal& rowMatrix, const Tuple& columnTuple) const {
        return atMIHelper(*this, rowMatrix, columnTuple);
    }


    MatrixComplex MatrixComplex::at(const MatrixComplex& rowMatrix, const Tuple& columnTuple) const {
        return atMIHelper(*this, rowMatrix, columnTuple);
    }


    MatrixComplex MatrixComplex::at(const Tuple& indexTuple) const {
        return atIHelper(*this, indexTuple);
    }


    MatrixComplex MatrixComplex::at(const MatrixInteger& rowMatrix, MatrixComplex::Index column) const {
        return atMCHelper(*this, rowMatrix, column);
    }


    MatrixComplex MatrixComplex::at(const MatrixInteger& rowMatrix, Real column) const {
        return at(rowMatrix, inlineToInteger(column));
    }


    MatrixComplex MatrixComplex::at(const MatrixInteger& rowMatrix, const Complex& column) const {
        return at(rowMatrix, inlineToInteger(column));
    }


    MatrixComplex MatrixComplex::at(const MatrixInteger& rowMatrix, const Range& columnRange) const {
        return atMIHelper(*this, rowMatrix, columnRange);
    }


    MatrixComplex MatrixComplex::at(const MatrixInteger& rowMatrix, const MatrixInteger& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixComplex MatrixComplex::at(const MatrixInteger& rowMatrix, const MatrixReal& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixComplex MatrixComplex::at(const MatrixInteger& rowMatrix, const MatrixComplex& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixComplex MatrixComplex::at(MatrixComplex::Index row, const MatrixInteger& columnMatrix) const {
        return atCMHelper(*this, row, columnMatrix);
    }


    MatrixComplex MatrixComplex::at(Real row, const MatrixInteger& columnMatrix) const {
        return at(inlineToInteger(row), columnMatrix);
    }


    MatrixComplex MatrixComplex::at(const Complex& row, const MatrixInteger& columnMatrix) const {
        return at(inlineToInteger(row), columnMatrix);
    }


    MatrixComplex MatrixComplex::at(const Range& rowRange, const MatrixInteger& columnMatrix) const {
        return atIMHelper(*this, rowRange, columnMatrix);
    }


    MatrixComplex MatrixComplex::at(const MatrixReal& rowMatrix, const MatrixInteger& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixComplex MatrixComplex::at(const MatrixComplex& rowMatrix, const MatrixInteger& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixComplex MatrixComplex::at(const MatrixInteger& indexMatrix) const {
        return atMHelper(*this, indexMatrix);
    }


    MatrixComplex MatrixComplex::at(const MatrixReal& rowMatrix, MatrixComplex::Index column) const {
        return atMCHelper(*this, rowMatrix, column);
    }


    MatrixComplex MatrixComplex::at(const MatrixReal& rowMatrix, Real column) const {
        return at(rowMatrix, inlineToInteger(column));
    }


    MatrixComplex MatrixComplex::at(const MatrixReal& rowMatrix, const Complex& column) const {
        return at(rowMatrix, inlineToInteger(column));
    }


    MatrixComplex MatrixComplex::at(const MatrixReal& rowMatrix, const Range& columnRange) const {
        return atMIHelper(*this, rowMatrix, columnRange);
    }


    MatrixComplex MatrixComplex::at(const MatrixReal& rowMatrix, const MatrixReal& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixComplex MatrixComplex::at(const MatrixReal& rowMatrix, const MatrixComplex& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixComplex MatrixComplex::at(MatrixComplex::Index row, const MatrixReal& columnMatrix) const {
        return atCMHelper(*this, row, columnMatrix);
    }


    MatrixComplex MatrixComplex::at(Real row, const MatrixReal& columnMatrix) const {
        return at(inlineToInteger(row), columnMatrix);
    }


    MatrixComplex MatrixComplex::at(const Complex& row, const MatrixReal& columnMatrix) const {
        return at(inlineToInteger(row), columnMatrix);
    }


    MatrixComplex MatrixComplex::at(const Range& rowRange, const MatrixReal& columnMatrix) const {
        return atIMHelper(*this, rowRange, columnMatrix);
    }


    MatrixComplex MatrixComplex::at(const MatrixComplex& rowMatrix, const MatrixReal& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixComplex MatrixComplex::at(const MatrixReal& indexMatrix) const {
        return atMHelper(*this, indexMatrix);
    }


    MatrixComplex MatrixComplex::at(const MatrixComplex& rowMatrix, MatrixComplex::Index column) const {
        return atMCHelper(*this, rowMatrix, column);
    }


    MatrixComplex MatrixComplex::at(const MatrixComplex& rowMatrix, Real column) const {
        return at(rowMatrix, inlineToInteger(column));
    }


    MatrixComplex MatrixComplex::at(const MatrixComplex& rowMatrix, const Complex& column) const {
        return at(rowMatrix, inlineToInteger(column));
    }


    MatrixComplex MatrixComplex::at(const MatrixComplex& rowMatrix, const Range& columnRange) const {
        return atMIHelper(*this, rowMatrix, columnRange);
    }


    MatrixComplex MatrixComplex::at(const MatrixComplex& rowMatrix, const MatrixComplex& columnMatrix) const {
        return atMMHelper(*this, rowMatrix, columnMatrix);
    }


    MatrixComplex MatrixComplex::at(MatrixComplex::Index row, const MatrixComplex& columnMatrix) const {
        return atCMHelper(*this, row, columnMatrix);
    }


    MatrixComplex MatrixComplex::at(Real row, const MatrixComplex& columnMatrix) const {
        return at(inlineToInteger(row), columnMatrix);
    }


    MatrixComplex MatrixComplex::at(const Complex& row, const MatrixComplex& columnMatrix) const {
        return at(inlineToInteger(row), columnMatrix);
    }


    MatrixComplex MatrixComplex::at(const Range& rowRange, const MatrixComplex& columnMatrix) const {
        return atIMHelper(*this, rowRange, columnMatrix);
    }


    MatrixComplex MatrixComplex::at(const MatrixComplex& indexMatrix) const {
        return atMHelper(*this, indexMatrix);
    }


    MatrixComplex MatrixComplex::at(const Range& rowRange, MatrixComplex::Index column) const {
        return atICHelper(*this, rowRange, column);
    }


    MatrixComplex MatrixComplex::at(const Range& rowRange, Real column) const {
        return at(rowRange, inlineToInteger(column));
    }


    MatrixComplex MatrixComplex::at(const Range& rowRange, const Complex& column) const {
        return at(rowRange, inlineToInteger(column));
    }


    MatrixComplex MatrixComplex::at(const Range& rowRange, const Range& columnRange) const {
        return atIIHelper(*this, rowRange, columnRange);
    }


    MatrixComplex MatrixComplex::at(MatrixComplex::Index row, const Range& columnRange) const {
        return atCIHelper(*this, row, columnRange);
    }


    MatrixComplex MatrixComplex::at(Real row, const Range& columnRange) const {
        return at(inlineToInteger(row), columnRange);
    }


    MatrixComplex MatrixComplex::at(const Complex& row, const Range& columnRange) const {
        return at(inlineToInteger(row), columnRange);
    }


    Variant MatrixComplex::at(const Variant& row, MatrixComplex::Index column) const {
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


    Variant MatrixComplex::at(const Variant& row, Real column) const {
        return at(row, inlineToInteger(column));
    }


    Variant MatrixComplex::at(const Variant& row, const Complex& column) const {
        return at(row, inlineToInteger(column));
    }


    Variant MatrixComplex::at(const Variant& row, const Range& columnRange) const {
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


    Variant MatrixComplex::at(const Variant& row, const Set& columnSet) const {
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


    Variant MatrixComplex::at(const Variant& row, const Tuple& columnTuple) const {
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


    Variant MatrixComplex::at(const Variant& row, const MatrixInteger& columnMatrix) const {
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


    Variant MatrixComplex::at(const Variant& row, const MatrixReal& columnMatrix) const {
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


    Variant MatrixComplex::at(const Variant& row, const MatrixComplex& columnMatrix) const {
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


    Variant MatrixComplex::at(MatrixComplex::Index row, const Variant& column) const {
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


    Variant MatrixComplex::at(Real row, const Variant& column) const {
        return at(inlineToInteger(row), column);
    }


    Variant MatrixComplex::at(const Complex& row, const Variant& column) const {
        return at(inlineToInteger(row), column);
    }


    Variant MatrixComplex::at(const Range& rowRange, const Variant& column) const {
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


    Variant MatrixComplex::at(const Set& rowSet, const Variant& column) const {
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


    Variant MatrixComplex::at(const Tuple& rowTuple, const Variant& column) const {
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


    Variant MatrixComplex::at(const MatrixInteger& rowMatrix, const Variant& column) const {
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


    Variant MatrixComplex::at(const MatrixReal& rowMatrix, const Variant& column) const {
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


    Variant MatrixComplex::at(const MatrixComplex& rowMatrix, const Variant& column) const {
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


    Variant MatrixComplex::at(const Variant& row, const Variant& column) const {
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


    Variant MatrixComplex::at(const Variant& index) const {
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


    const MatrixComplex::Scalar* MatrixComplex::pointer(MatrixComplex::Index row, MatrixComplex::Index column) const {
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


    void MatrixComplex::update(Matrix::Index row, Matrix::Index column, const Scalar& newValue) {
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


    void MatrixComplex::update(Matrix::Index index, const Scalar& newValue) {
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


    MatrixComplex MatrixComplex::combineLeftToRight(const MatrixComplex& other) const {
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

        return MatrixComplex(resultData);
    }


    MatrixComplex MatrixComplex::combineTopToBottom(const MatrixComplex& other) const {
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

        return MatrixComplex(resultData);
    }


    MatrixComplex MatrixComplex::columnReverse() const {
        applyLazyTransformsAndScaling();
        return MatrixComplex(currentData->columnReverse());
    }


    MatrixComplex MatrixComplex::rowReverse() const {
        applyLazyTransformsAndScaling();
        return MatrixComplex(currentData->rowReverse());
    }


    bool MatrixComplex::isSquare() const {
        return currentData->numberRows() == currentData->numberColumns();
    }


    bool MatrixComplex::isSymmetric(Real relativeTolerance) const {
        bool symmetric = true;

        if (isSquare()) {
            unsigned long numberRowColumns = currentData->numberRows();
            unsigned long rowIndex         = 0;

            while (symmetric && rowIndex < numberRowColumns) {
                unsigned long columnIndex = 0;

                while (symmetric && columnIndex < numberRowColumns) {
                    const Scalar& v1 = currentData->at(rowIndex, columnIndex);
                    const Scalar& v2 = currentData->at(columnIndex, rowIndex);

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


    bool MatrixComplex::isHermitian(Real relativeTolerance) const {
        bool hermitian = true;

        if (isSquare()) {
            unsigned long numberRowColumns = currentData->numberRows();
            unsigned long rowIndex         = 0;

            while (hermitian && rowIndex < numberRowColumns) {
                unsigned long columnIndex = 0;

                while (hermitian && columnIndex < numberRowColumns) {
                    const Scalar& v1 = currentData->at(rowIndex, columnIndex);
                    const Scalar& v2 = currentData->at(columnIndex, rowIndex).conj();

                    if (v1 != v2) {
                        Real average = abs(v1 + v2) / 2.0;
                        Real error   = abs(v1 - v2);
                        hermitian = error <= relativeTolerance * average;
                    }

                    ++columnIndex;
                }

                ++rowIndex;
            }
        } else {
            hermitian = false;
        }

        return hermitian;
    }


    bool MatrixComplex::isSkewSymmetric(Real relativeTolerance) const {
        bool skewSymmetric = true;

        if (isSquare()) {
            unsigned long numberRowColumns = currentData->numberRows();
            unsigned long rowIndex         = 0;

            while (skewSymmetric && rowIndex < numberRowColumns) {
                unsigned long columnIndex = 0;

                while (skewSymmetric && columnIndex < numberRowColumns) {
                    const Scalar& v1 =  currentData->at(rowIndex, columnIndex);
                    const Scalar& v2 = -currentData->at(columnIndex, rowIndex);

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


    bool MatrixComplex::isSkewHermitian(Real relativeTolerance) const {
        bool skewHermitian = true;

        if (isSquare()) {
            unsigned long numberRowColumns = currentData->numberRows();
            unsigned long rowIndex         = 0;

            while (skewHermitian && rowIndex < numberRowColumns) {
                unsigned long columnIndex = 0;

                while (skewHermitian && columnIndex < numberRowColumns) {
                    const Scalar& v1 =  currentData->at(rowIndex, columnIndex);
                    const Scalar& v2 = -currentData->at(columnIndex, rowIndex).conj();

                    if (v1 != v2) {
                        Real average = abs(v1 + v2) / 2.0;
                        Real error   = abs(v1 - v2);
                        skewHermitian = error <= relativeTolerance * average;
                    }

                    ++columnIndex;
                }

                ++rowIndex;
            }
        } else {
            skewHermitian = false;
        }

        return skewHermitian;
    }


    bool MatrixComplex::isNormal(Real relativeTolerance) const {
        bool normal = true;

        if (isSquare()) {
            MatrixComplex ah = this->adjoint();

            MatrixComplex p1 = ah * (*this);
            MatrixComplex p2 = (*this) * ah;

            if (p1 != p2) {
                if (p1.numberRows() == p2.numberRows() && p1.numberColumns() == p2.numberColumns()) {
                    normal = true;
                    unsigned long column        = 1;
                    unsigned long numberRows    = p1.numberRows();
                    unsigned long numberColumns = p1.numberColumns();
                    while (normal && column <= numberColumns) {
                        unsigned long row = 1;
                        while (normal && row <= numberRows) {
                            Complex v1 = p1.at(static_cast<Integer>(row), static_cast<Integer>(column));
                            Complex v2 = p2.at(static_cast<Integer>(row), static_cast<Integer>(column));

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


    Integer MatrixComplex::rank(Real epsilon) const {
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
            const Scalar& v = sData->at(index, index);
            if (M::abs(v) > epsilon) {
                ++result;
            }
        }

        Data::destroy(sData);

        return result;
    }


    MatrixComplex::Scalar MatrixComplex::determinant() const {
        applyLazyTransformsAndScaling();
        return currentData->determinant();
    }


    MatrixComplex MatrixComplex::transpose() const {
        MatrixComplex result(*this);

        switch (pendingTransform) {
            case LazyTransform::NONE: {
                result.pendingTransform = LazyTransform::TRANSPOSE;
                break;
            }

            case LazyTransform::TRANSPOSE: {
                result.pendingTransform = LazyTransform::NONE;
                break;
            }

            case LazyTransform::ADJOINT: {
                result.pendingTransform = LazyTransform::CONJUGATE;
                break;
            }

            case LazyTransform::CONJUGATE: {
                result.pendingTransform = LazyTransform::ADJOINT;
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        return result;
    }


    MatrixComplex MatrixComplex::conj() const {
        MatrixComplex result(*this);

        switch (pendingTransform) {
            case LazyTransform::NONE: {
                result.pendingTransform = LazyTransform::CONJUGATE;
                break;
            }

            case LazyTransform::TRANSPOSE: {
                result.pendingTransform = LazyTransform::ADJOINT;
                break;
            }

            case LazyTransform::ADJOINT: {
                result.pendingTransform = LazyTransform::TRANSPOSE;
                break;
            }

            case LazyTransform::CONJUGATE: {
                result.pendingTransform = LazyTransform::NONE;
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        return result;
    }


    MatrixComplex MatrixComplex::adjoint() const {
        MatrixComplex result(*this);

        switch (pendingTransform) {
            case LazyTransform::NONE: {
                result.pendingTransform = LazyTransform::ADJOINT;
                break;
            }

            case LazyTransform::TRANSPOSE: {
                result.pendingTransform = LazyTransform::CONJUGATE;
                break;
            }

            case LazyTransform::ADJOINT: {
                result.pendingTransform = LazyTransform::NONE;
                break;
            }

            case LazyTransform::CONJUGATE: {
                result.pendingTransform = LazyTransform::TRANSPOSE;
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        return result;
    }


    MatrixComplex MatrixComplex::inverse() const {
        applyLazyTransformsAndScaling();
        return MatrixComplex(currentData->inverse());
    }


    Tuple MatrixComplex::plu() const {
        Tuple result;

        applyLazyTransformsAndScaling();

        Data* pData;
        Data* lData;
        Data* uData;

        bool notSingular = currentData->plu(pData, lData, uData);

        result.append(MatrixComplex(pData));
        result.append(MatrixComplex(lData));
        result.append(MatrixComplex(uData));
        result.append(Boolean(notSingular));

        return result;
    }


    Tuple MatrixComplex::svd() const {
        Tuple result;

        applyLazyTransformsAndScaling();

        Data* uData;
        Data* sData;
        Data* vData;

        bool success = currentData->svd(uData, sData, vData);

        result.append(MatrixComplex(uData));
        result.append(MatrixComplex(sData));
        result.append(MatrixComplex(vData));
        result.append(Model::Boolean(success));

        return result;
    }


    Tuple MatrixComplex::qr() const {
        Tuple result;

        applyLazyTransformsAndScaling();

        Data* qData;
        Data* rData;

        bool success = currentData->qr(qData, rData);

        result.append(MatrixComplex(qData));
        result.append(MatrixComplex(rData));
        result.append(Boolean(success));

        return result;
    }


    Tuple MatrixComplex::lq() const {
        Tuple result;

        Data* qData;
        Data* rData;
        bool  success;

        {
            MatrixComplex ah = this->adjoint();
            ah.applyLazyTransformsAndScaling();
            success = ah.currentData->qr(qData, rData);
        }

        MatrixComplex l(rData);
        MatrixComplex q(qData);

        l.pendingTransform = LazyTransform::ADJOINT;
        q.pendingTransform = LazyTransform::ADJOINT;

        result.append(l);
        result.append(q);
        result.append(Boolean(success));

        return result;
    }


    MatrixComplex MatrixComplex::cholesky() const {
        applyLazyTransformsAndScaling();
        return MatrixComplex(currentData->cholesky());
    }


    MatrixComplex MatrixComplex::upperCholesky() const {
        applyLazyTransformsAndScaling();
        return MatrixComplex(currentData->upperCholesky());
    }


    Tuple MatrixComplex::hessenberg() const {
        Tuple result;

        applyLazyTransformsAndScaling();

        Data* qData;
        Data* hData;

        currentData->hessenberg(qData, hData);

        result.append(MatrixComplex(qData));
        result.append(MatrixComplex(hData));

        return result;
    }


    Tuple MatrixComplex::schur() const {
        Tuple result;

        applyLazyTransformsAndScaling();

        Data* qData;
        Data* uData;
        Data* wData;
        bool  success;

        success = currentData->schur(qData, uData, wData);

        result.append(MatrixComplex(qData));
        result.append(MatrixComplex(uData));
        result.append(MatrixComplex(wData));
        result.append(Boolean(success));

        return result;
    }


    Tuple MatrixComplex::eigenvectors() const {
        return eigenvectors(true);
    }


    MatrixComplex MatrixComplex::dft() const {
        applyLazyTransformsAndScaling();
        return MatrixComplex(currentData->dft());
    }


    MatrixComplex MatrixComplex::idft() const {
        applyLazyTransformsAndScaling();
        return MatrixComplex(currentData->idft());
    }


    Real MatrixComplex::conditionNumber() const {
        applyLazyTransformsAndScaling();

        Data* inverse     = currentData->inverse();
        Real  inverseNorm = inverse->pNorm(2.0);
        Data::destroy(inverse);

        Real norm = currentData->pNorm(2.0);

        return inverseNorm * norm;
    }


    Real MatrixComplex::pNorm(Integer p) const {
        applyLazyTransformsAndScaling();
        return currentData->pNorm(p);
    }


    Real MatrixComplex::euclideanNorm() const {
        return pNorm(2.0);
    }


    Real MatrixComplex::oneNorm() const {
        applyLazyTransformsAndScaling();
        return currentData->oneNorm();
    }


    Real MatrixComplex::infinityNorm() const {
        applyLazyTransformsAndScaling();
        return currentData->infinityNorm();
    }


    Tuple MatrixComplex::equilibrate() const {
        Tuple result;

        applyLazyTransformsAndScaling();

        Data* row;
        Data* column;

        bool  success = currentData->equilibrate(row, column);

        result.append(MatrixComplex(row));
        result.append(MatrixComplex(column));
        result.append(Boolean(success));

        return result;
    }


    MatrixComplex MatrixComplex::solve(const MatrixComplex& y) const {
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

        return MatrixComplex(result);
    }


    MatrixComplex MatrixComplex::leastSquares(const MatrixComplex& b) const {
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

        return MatrixComplex(result);
    }


    MatrixComplex MatrixComplex::hadamard(const MatrixComplex& other) const {
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

        return MatrixComplex(result);
    }


    MatrixComplex MatrixComplex::kronecker(const MatrixComplex& other) const {
        applyLazyTransformsAndScaling();

        Data* result = nullptr;
        if (other.currentData->matrixType() == MatrixType::DENSE) {
            result = currentData->kronecker(
                *POLYMORPHIC_CAST<const DenseData*>(other.currentData),
                other.pendingTransform,
                other.pendingScalarValue
            );
        } else {
            assert(other.currentData->matrixType() == MatrixType::SPARSE);
            assert(false); // FIXME
//            result = currentData->hadamard(*POLYMORPHIC_CAST<const SparseData*>(other.currentData));
        }

        return MatrixComplex(result);
    }


    MatrixComplex::MatrixType MatrixComplex::matrixType() const {
        return currentData->matrixType();
    }


    MatrixComplex::Scalar* MatrixComplex::data() {
        applyLazyTransformsAndScaling();
        return currentData->data();
    }


    const MatrixComplex::Scalar* MatrixComplex::data() const {
        applyLazyTransformsAndScaling();
        return currentData->data();
    }


    MatrixReal MatrixComplex::real() const {
        if (pendingScalarValue.real() != Complex::T(0) && pendingScalarValue.imag() != Complex::T(0)) {
            applyLazyTransformsAndScaling();
        }

        MatrixReal::Data* realData;
        Real              realScalarValue = 0;
        bool              realTransposed  = false;

        if (pendingScalarValue.imag() != Complex::T(0)) {
            realData = currentData->imag();

            switch (pendingTransform) {
                case LazyTransform::NONE: {
                    realScalarValue = -pendingScalarValue.imag();
                    realTransposed  = false;

                    break;
                }

                case LazyTransform::TRANSPOSE: {
                    realScalarValue = -pendingScalarValue.imag();
                    realTransposed  = true;

                    break;
                }

                case LazyTransform::CONJUGATE: {
                    realScalarValue = +pendingScalarValue.imag();
                    realTransposed  = false;

                    break;
                }

                case LazyTransform::ADJOINT: {
                    realScalarValue = +pendingScalarValue.imag();
                    realTransposed  = true;

                    break;
                }

                default: {
                    assert(false);
                    break;
                }
            }
        } else {
            realData        = currentData->real();
            realScalarValue = pendingScalarValue.real();
            realTransposed  = (
                   pendingTransform == LazyTransform::TRANSPOSE
                || pendingTransform == LazyTransform::ADJOINT
            );
        }

        MatrixReal result(realData);
        result.pendingScalarValue = realScalarValue;
        result.pendingTranspose   = realTransposed;

        return result;
    }


    MatrixReal MatrixComplex::imag() const {
        if (pendingScalarValue.real() != Complex::T(0) && pendingScalarValue.imag() != Complex::T(0)) {
            applyLazyTransformsAndScaling();
        }

        MatrixReal::Data* imaginaryData;
        Real              imaginaryScalarValue = 0;
        bool              imaginaryTransposed  = false;

        if (pendingScalarValue.imag() != Complex::T(0)) {
            imaginaryData        = currentData->real();
            imaginaryScalarValue = pendingScalarValue.imag();
            imaginaryTransposed  = (
                   pendingTransform == LazyTransform::TRANSPOSE
                || pendingTransform == LazyTransform::ADJOINT
            );
        } else {
            imaginaryData = currentData->imag();

            switch (pendingTransform) {
                case LazyTransform::NONE: {
                    imaginaryScalarValue = +pendingScalarValue.real();
                    imaginaryTransposed  = false;

                    break;
                }

                case LazyTransform::TRANSPOSE: {
                    imaginaryScalarValue = +pendingScalarValue.real();
                    imaginaryTransposed  = true;

                    break;
                }

                case LazyTransform::CONJUGATE: {
                    imaginaryScalarValue = -pendingScalarValue.real();
                    imaginaryTransposed  = false;

                    break;
                }

                case LazyTransform::ADJOINT: {
                    imaginaryScalarValue = -pendingScalarValue.real();
                    imaginaryTransposed  = true;

                    break;
                }

                default: {
                    assert(false);
                    break;
                }
            }
        }

        MatrixReal result(imaginaryData);
        result.pendingScalarValue = imaginaryScalarValue;
        result.pendingTranspose   = imaginaryTransposed;

        return result;
    }


    MatrixComplex& MatrixComplex::operator=(const MatrixComplex& other) {
        assignExistingDataStore(other.currentData, false);

        pendingTransform   = other.pendingTransform;
        pendingScalarValue = other.pendingScalarValue;

        return *this;
    }


    MatrixComplex& MatrixComplex::operator=(const MatrixReal& other) {
        releaseDataStore(currentData);
        copyFrom(other);

        return *this;
    }


    MatrixComplex& MatrixComplex::operator=(const MatrixInteger& other) {
        releaseDataStore(currentData);
        copyFrom(other);

        return *this;
    }


    MatrixComplex& MatrixComplex::operator=(const MatrixBoolean& other) {
        releaseDataStore(currentData);
        copyFrom(other);

        return *this;
    }


    MatrixComplex& MatrixComplex::operator=(const Variant& other) {
        bool          ok;
        MatrixComplex v = other.toMatrixComplex(&ok);

        if (ok) {
            assignExistingDataStore(v.currentData, false);

            pendingTransform   = v.pendingTransform;
            pendingScalarValue = v.pendingScalarValue;
        } else {
            throw Model::InvalidRuntimeConversion(other.valueType(), ValueType::MATRIX_COMPLEX);
        }

        return *this;
    }


    MatrixComplex& MatrixComplex::operator+=(const MatrixComplex& v) {
        currentData->lock();
        v.currentData->lock();

        Data* newDataStore = nullptr;
        if (v.currentData->matrixType() == MatrixType::DENSE) {
            newDataStore = currentData->add(
                *POLYMORPHIC_CAST<const DenseData*>(v.currentData),
                pendingTransform,
                v.pendingTransform,
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

        pendingTransform = LazyTransform::NONE;
        pendingScalarValue = Complex(1, 0);

        return *this;
    }


    MatrixComplex& MatrixComplex::operator-=(const MatrixComplex& v) {
        currentData->lock();
        v.currentData->lock();

        Data* newDataStore = nullptr;
        if (v.currentData->matrixType() == MatrixType::DENSE) {
            newDataStore = currentData->add(
                *POLYMORPHIC_CAST<const DenseData*>(v.currentData),
                pendingTransform,
                v.pendingTransform,
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

        pendingTransform = LazyTransform::NONE;
        pendingScalarValue = Complex(1, 0);

        return *this;
    }


    MatrixComplex& MatrixComplex::operator*=(const MatrixComplex& v) {
        Data* newDataStore;

        if (pendingTransform == LazyTransform::CONJUGATE) {
            applyLazyTransformsAndScaling();
        }

        if (v.pendingTransform == LazyTransform::CONJUGATE) {
            v.applyLazyTransformsAndScaling();
        }

        Complex scalarMultiplier = pendingScalarValue * v.pendingScalarValue;
        Data*   multiplicand     = v.currentData;

        currentData->lock();

        multiplicand->lock();
        if (multiplicand->matrixType() == MatrixType::DENSE) {
            newDataStore = currentData->multiply(
                static_cast<const DenseData&>(*multiplicand),
                pendingTransform,
                v.pendingTransform,
                scalarMultiplier
            );
        } else {
            assert(multiplicand->matrixType() == MatrixType::SPARSE);

            newDataStore = currentData->multiply(
                static_cast<const SparseData&>(*multiplicand),
                pendingTransform,
                v.pendingTransform,
                scalarMultiplier
            );
        }
        multiplicand->unlock();
        assignNewDataStore(newDataStore, true);

        pendingTransform   = LazyTransform::NONE;
        pendingScalarValue = Complex(1, 0);

        return *this;
    }


    MatrixComplex& MatrixComplex::operator*=(Boolean v) {
        if (v == false) {
            pendingScalarValue = Complex(0, 0);
        }

        return *this;
    }


    MatrixComplex& MatrixComplex::operator*=(Integer v) {
        pendingScalarValue *= Complex(v);
        return *this;
    }


    MatrixComplex& MatrixComplex::operator*=(Real v) {
        pendingScalarValue *= Complex(v);
        return *this;
    }


    MatrixComplex& MatrixComplex::operator*=(const Complex& v) {
        pendingScalarValue *= v;
        return *this;
    }


    MatrixComplex MatrixComplex::operator+(const MatrixBoolean& v) const {
        return operator+(MatrixComplex(v));
    }


    MatrixComplex MatrixComplex::operator+(const MatrixInteger& v) const {
        return operator+(MatrixComplex(v));
    }


    MatrixComplex MatrixComplex::operator+(const MatrixReal& v) const {
        return operator+(MatrixComplex(v));
    }


    MatrixComplex MatrixComplex::operator+(const MatrixComplex& v) const {
        currentData->lock();
        v.currentData->lock();

        Data* newDataStore = nullptr;
        if (v.currentData->matrixType() == MatrixType::DENSE) {
            newDataStore = currentData->add(
                *POLYMORPHIC_CAST<const DenseData*>(v.currentData),
                pendingTransform,
                v.pendingTransform,
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

        return MatrixComplex(newDataStore);
    }


    MatrixComplex MatrixComplex::operator-(const MatrixBoolean& v) const {
        return operator-(MatrixComplex(v));
    }


    MatrixComplex MatrixComplex::operator-(const MatrixInteger& v) const {
        return operator-(MatrixComplex(v));
    }


    MatrixComplex MatrixComplex::operator-(const MatrixReal& v) const {
        return operator-(MatrixComplex(v));
    }


    MatrixComplex MatrixComplex::operator-(const MatrixComplex& v) const {
        currentData->lock();
        v.currentData->lock();

        Data* newDataStore = nullptr;
        if (v.currentData->matrixType() == MatrixType::DENSE) {
            newDataStore = currentData->add(
                *POLYMORPHIC_CAST<const DenseData*>(v.currentData),
                pendingTransform,
                v.pendingTransform,
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

        return MatrixComplex(newDataStore);
    }


    MatrixComplex MatrixComplex::operator*(const MatrixBoolean& v) const {
        return operator*(MatrixComplex(v));
    }


    MatrixComplex MatrixComplex::operator*(const MatrixInteger& v) const {
        return operator*(MatrixComplex(v));
    }


    MatrixComplex MatrixComplex::operator*(const MatrixReal& v) const {
        return operator*(MatrixComplex(v));
    }


    MatrixComplex MatrixComplex::operator*(const MatrixComplex& v) const {
        Data* result;

        if (pendingTransform == LazyTransform::CONJUGATE) {
            applyLazyTransformsAndScaling();
        }

        if (v.pendingTransform == LazyTransform::CONJUGATE) {
            v.applyLazyTransformsAndScaling();
        }

        Complex scalarMultiplier = pendingScalarValue * v.pendingScalarValue;
        Data*   multiplicand     = v.currentData;

        if (multiplicand->matrixType() == MatrixType::DENSE) {
            Complex scalarMultiplier = pendingScalarValue * v.pendingScalarValue;
            result = currentData->multiply(
                static_cast<const DenseData&>(*multiplicand),
                pendingTransform,
                v.pendingTransform,
                scalarMultiplier
            );
        } else {
            assert(multiplicand->matrixType() == MatrixType::SPARSE);

            result = currentData->multiply(
                static_cast<const SparseData&>(*multiplicand),
                pendingTransform,
                v.pendingTransform,
                scalarMultiplier
            );
        }

        return MatrixComplex(result);
    }


    MatrixComplex MatrixComplex::operator*(Boolean v) const {
        MatrixComplex result(*this);

        if (v == false) {
            result.pendingScalarValue = Complex(0, 0);
        }

        return result;
    }


    MatrixComplex MatrixComplex::operator*(Integer v) const {
        MatrixComplex result(*this);
        result.pendingScalarValue *= Complex(v);

        return result;
    }


    MatrixComplex MatrixComplex::operator*(Real v) const {
        MatrixComplex result(*this);
        result.pendingScalarValue *= Complex(v);

        return result;
    }

    MatrixComplex MatrixComplex::operator*(const Complex& v) const {
        MatrixComplex result(*this);
        result.pendingScalarValue *= v;

        return result;
    }


    MatrixComplex MatrixComplex::operator/(Integer v) const {
        MatrixComplex result(*this);
        result.pendingScalarValue /= Complex(v);

        return result;
    }


    MatrixComplex MatrixComplex::operator/(Real v) const {
        MatrixComplex result(*this);
        result.pendingScalarValue /= Complex(v);

        return result;
    }


    MatrixComplex MatrixComplex::operator/(const Complex& v) const {
        MatrixComplex result(*this);
        result.pendingScalarValue /= v;

        return result;
    }


    MatrixComplex MatrixComplex::operator-() const {
        MatrixComplex result(*this);
        result.pendingScalarValue *= Complex(-1);

        return result;
    }


    bool MatrixComplex::operator==(const MatrixComplex& other) const {
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


    int MatrixComplex::relativeOrder(const MatrixComplex& other) const {
        int result;

        if (currentData == other.currentData) {
            if (pendingTransform == other.pendingTransform) {
                if (pendingScalarValue.real() < other.pendingScalarValue.real()) {
                    result = -1;
                } else if (pendingScalarValue.real() > other.pendingScalarValue.real()) {
                    result = +1;
                } else /* if (pendingScalarValue.real() == other.pendingScalarValue.real()) */ {
                    if (pendingScalarValue.imag() < other.pendingScalarValue.imag()) {
                        result = -1;
                    } else if (pendingScalarValue.imag() > other.pendingScalarValue.imag()) {
                        result = +1;
                    } else /* if (pendingScalarValue.imag() == other.pendingScalarValue.imag()) */ {
                        result = 0;
                    }
                }
            } else {
                result = static_cast<int>(pendingTransform) - static_cast<int>(other.pendingTransform);
            }
        } else {
            applyLazyTransformsAndScaling();
            other.applyLazyTransformsAndScaling();

            result = currentData->relativeOrder(*other.currentData);
        }

        return result;
    }


    MatrixComplex::MatrixComplex(
            Data* dataStore
        ):currentData(
            dataStore
        ),pendingTransform(
            LazyTransform::NONE
        ),pendingScalarValue(
            1.0
        ) {}


    void MatrixComplex::copyFrom(const MatrixBoolean& booleanMatrix) {
        MatrixBoolean::Data* otherData = booleanMatrix.currentData;

        if (otherData->matrixType() == MatrixType::DENSE) {
            currentData = DenseData::create(*POLYMORPHIC_CAST<MatrixBoolean::DenseData*>(otherData));
        } else {
            assert(otherData->matrixType() == MatrixType::SPARSE);
            assert(false); // FIXME
        }

        pendingTransform   = booleanMatrix.pendingTranspose ? LazyTransform::TRANSPOSE : LazyTransform::NONE;
        pendingScalarValue = Scalar(1);
    }


    void MatrixComplex::copyFrom(const MatrixInteger& integerMatrix) {
        if (integerMatrix.currentData->matrixType() == MatrixType::DENSE) {
            const MatrixInteger::DenseData*
                data = POLYMORPHIC_CAST<const MatrixInteger::DenseData*>(integerMatrix.currentData);

            currentData = DenseData::create(*data);
            pendingTransform   = integerMatrix.pendingTranspose ? LazyTransform::TRANSPOSE : LazyTransform::NONE;
            pendingScalarValue = Complex(integerMatrix.pendingScalarMultiplierValue);
        } else {
            assert(integerMatrix.currentData->matrixType() == MatrixType::SPARSE);
            assert(false); // FIXME
        }
    }


    void MatrixComplex::copyFrom(const MatrixReal& realMatrix) {
        if (realMatrix.currentData->matrixType() == MatrixType::DENSE) {
            const MatrixReal::DenseData* data = POLYMORPHIC_CAST<const MatrixReal::DenseData*>(realMatrix.currentData);
            currentData = DenseData::create(*data);

            pendingTransform   = realMatrix.pendingTranspose ? LazyTransform::TRANSPOSE : LazyTransform::NONE;
            pendingScalarValue = realMatrix.pendingScalarValue;
        } else {
            assert(realMatrix.currentData->matrixType() == MatrixType::SPARSE);
            assert(false); // FIXME
        }
    }


    void MatrixComplex::releaseDataStore(MatrixComplex::Data* dataStore) const {
        if (dataStore->removeReference()) {
            Data::destroy(dataStore);
        }
    }


    void MatrixComplex::assignNewDataStore(MatrixComplex::Data* newDataStore, bool unlock) const {
        Data* oldData = currentData;
        currentData = newDataStore;

        if (unlock) {
            oldData->unlock();
        }

        releaseDataStore(oldData);
    }


    void MatrixComplex::assignExistingDataStore(MatrixComplex::Data* newDataStore, bool unlock) const {
        newDataStore->addReference();
        assignNewDataStore(newDataStore, unlock);
    }


    void MatrixComplex::applyLazyTransformsAndScaling() const {
        if (pendingScalarValue != Complex(1, 0) || pendingTransform != LazyTransform::NONE) {
            currentData->lock();
            Data* newDataStore = currentData->applyTransformAndScaling(pendingTransform, pendingScalarValue);
            assignNewDataStore(newDataStore, true);

            pendingTransform = LazyTransform::NONE;
            pendingScalarValue = Complex(1, 0);
        }
    }


    Tuple MatrixComplex::eigenvectors(bool right) const {
        Tuple result;

        applyLazyTransformsAndScaling();

        Data*  eigenvalueMatrix;
        Data*  qMatrix;
        Data*  uMatrix;
        Data** eigenvectorMatrices;

        currentData->eigenvectors(right, eigenvalueMatrix, qMatrix, uMatrix, eigenvectorMatrices);

        result.append(MatrixComplex(eigenvalueMatrix));
        result.append(MatrixComplex(qMatrix));
        result.append(MatrixComplex(uMatrix));

        Data** ev = eigenvectorMatrices;
        while (*ev != nullptr) {
            result.append(MatrixComplex(*ev));
            ++ev;
        }

        delete[] eigenvectorMatrices;

        return result;
    }


    MatrixComplex operator*(Boolean a, const MatrixComplex& b) {
        return b * a;
    }


    MatrixComplex operator*(Integer a, const MatrixComplex& b) {
        return b * a;
    }


    MatrixComplex operator*(Real a, const MatrixComplex& b) {
        return b * a;
    }


    MatrixComplex operator*(const Complex& a, const MatrixComplex& b) {
        return b * a;
    }


    MatrixComplex operator*(const Complex& a, const MatrixBoolean& b) {
        return MatrixComplex(b) * a;
    }


    MatrixComplex operator*(const Complex& a, const MatrixInteger& b) {
        return MatrixComplex(b) * a;
    }


    MatrixComplex operator*(const Complex& a, const MatrixReal& b) {
        return MatrixComplex(b) * a;
    }


    MatrixComplex operator*(const MatrixBoolean& a, const Complex& b) {
        return MatrixComplex(a) * b;
    }


    MatrixComplex operator*(const MatrixInteger& a, const Complex& b) {
        return MatrixComplex(a) * b;
    }


    MatrixComplex operator*(const MatrixReal& a, const Complex& b) {
        return MatrixComplex(a) * b;
    }


    MatrixComplex operator*(const MatrixBoolean& a, const MatrixComplex& b) {
        return MatrixComplex(a) * b;
    }


    MatrixComplex operator*(const MatrixInteger& a, const MatrixComplex& b) {
        return MatrixComplex(a) * b;
    }


    MatrixComplex operator*(const MatrixReal& a, const MatrixComplex& b) {
        return MatrixComplex(a) * b;
    }


    MatrixComplex operator/(const MatrixBoolean& a, const Complex& b) {
        return MatrixComplex(a) / b;
    }


    MatrixComplex operator/(const MatrixInteger& a, const Complex& b) {
        return MatrixComplex(a) / b;
    }


    MatrixComplex operator/(const MatrixReal& a, const Complex& b) {
        return MatrixComplex(a) / b;
    }


    MatrixComplex operator+(const MatrixBoolean& a, const MatrixComplex& b) {
        return MatrixComplex(a) + b;
    }


    MatrixComplex operator+(const MatrixInteger& a, const MatrixComplex& b) {
        return MatrixComplex(a) + b;
    }


    MatrixComplex operator+(const MatrixReal& a, const MatrixComplex& b) {
        return MatrixComplex(a) + b;
    }


    MatrixComplex operator-(const MatrixBoolean& a, const MatrixComplex& b) {
        return MatrixComplex(a) - b;
    }


    MatrixComplex operator-(const MatrixInteger& a, const MatrixComplex& b) {
        return MatrixComplex(a) - b;
    }


    MatrixComplex operator-(const MatrixReal& a, const MatrixComplex& b) {
        return MatrixComplex(a) - b;
    }
}
