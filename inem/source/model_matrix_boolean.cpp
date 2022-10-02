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
* This file implements the \ref Model::MatrixBoolean class.
***********************************************************************************************************************/

#include <algorithm>
#include <cassert>

#include "model_api_types.h"
#include "model_exceptions.h"
#include "model_intrinsic_types.h"
#include "model_range.h"
#include "m_range.h"
#include "model_matrix.h"
#include "model_matrix_integer.h"
#include "model_matrix_integer_private.h"
#include "model_matrix_real.h"
#include "model_matrix_real_private.h"
#include "model_matrix_complex.h"
#include "model_matrix_complex_private.h"
#include "model_matrix_boolean.h"
#include "model_matrix_boolean_private.h"

namespace Model {
    MatrixBoolean::MatrixBoolean() {
        impl = new Private();
    }


    MatrixBoolean::MatrixBoolean(Integer numberRows, Integer numberColumns) {
        impl = new Private(numberRows, numberColumns);
    }


    MatrixBoolean::MatrixBoolean(Integer numberRows, Integer numberColumns, const MatrixBoolean::Scalar* matrixData) {
        impl = new Private(numberRows, numberColumns, matrixData);
    }


    MatrixBoolean::MatrixBoolean(const MatrixBoolean& other) {
        impl = new Private(*other.impl);
    }


    MatrixBoolean::MatrixBoolean(const MatrixInteger& other) {
        impl = new Private(*other.impl);
    }


    MatrixBoolean::MatrixBoolean(const MatrixReal& other) {
        impl = new Private(*other.impl);
    }


    MatrixBoolean::MatrixBoolean(const MatrixComplex& other) {
        impl = new Private(*other.impl);
    }


    MatrixBoolean::MatrixBoolean(MatrixBoolean&& other) {
        impl = other.impl;
        other.impl = nullptr;
    }


    MatrixBoolean::MatrixBoolean(const Variant& other) {
        bool          ok;
        MatrixBoolean v = other.toMatrixBoolean(&ok);

        if (ok) {
            impl = new Private(*v.impl);
        } else {
            throw InvalidRuntimeConversion(other.valueType(), ValueType::MATRIX_BOOLEAN);
        }
    }


    MatrixBoolean::~MatrixBoolean() {
        if (impl != nullptr) {
            delete impl;
        }
    }


    ValueType MatrixBoolean::coefficientValueType() const {
        return ValueType::BOOLEAN;
    }


    bool MatrixBoolean::toFile(const char* filename, DataFileFormat fileFormat) const {
        return impl->toFile(filename, fileFormat);
    }


    MatrixBoolean MatrixBoolean::fromFile(const char* filename) {
        return MatrixBoolean(new Private(M::MatrixBoolean::fromFile(filename)));
    }


    MatrixBoolean::Index MatrixBoolean::numberRows() const {
        return impl->numberRows();
    }


    MatrixBoolean::Index MatrixBoolean::numberColumns() const {
        return impl->numberColumns();
    }


    MatrixBoolean::Index MatrixBoolean::numberCoefficients() const {
        return impl->numberCoefficients();
    }


    MatrixBoolean MatrixBoolean::identity(Index numberRows, Index numberColumns) {
        return MatrixBoolean(new Private(M::MatrixBoolean::identity(numberRows, numberColumns)));
    }


    MatrixBoolean MatrixBoolean::identity(Index numberRowColumns) {
        return MatrixBoolean(new Private(M::MatrixBoolean::identity(numberRowColumns)));
    }


    MatrixBoolean MatrixBoolean::ones(Index numberRows, Index numberColumns) {
        return MatrixBoolean(new Private(M::MatrixBoolean::ones(numberRows, numberColumns)));
    }


    MatrixBoolean MatrixBoolean::ones(Index numberRowColumns) {
        return MatrixBoolean(new Private(M::MatrixBoolean::ones(numberRowColumns)));
    }


    MatrixBoolean MatrixBoolean::diagonalEntries() const {
        return MatrixBoolean(new Private(impl->diagonalEntries()));
    }


    MatrixBoolean MatrixBoolean::diagonal() const {
        return MatrixBoolean(new Private(impl->diagonal()));
    }


    Variant MatrixBoolean::value(MatrixBoolean::Index row, MatrixBoolean::Index column) const {
        return impl->value(row, column);
    }


    Variant MatrixBoolean::value(MatrixBoolean::Index index) const {
        return impl->value(index);
    }


    bool MatrixBoolean::setValue(MatrixBoolean::Index row, MatrixBoolean::Index column, const Variant& value) {
        return impl->setValue(row, column, value);
    }


    bool MatrixBoolean::setValue(MatrixBoolean::Index index, const Variant& value) {
        return impl->setValue(index, value);
    }


    void MatrixBoolean::resize(MatrixBoolean::Index newNumberRows, MatrixBoolean::Index newNumberColumns) {
        impl->resize(newNumberRows, newNumberColumns);
    }


    MatrixBoolean::Scalar MatrixBoolean::at(MatrixBoolean::Index row, MatrixBoolean::Index column) const {
        return impl->at(row, column);
    }


    MatrixBoolean::Scalar MatrixBoolean::at(MatrixBoolean::Index index) const {
        return impl->at(index);
    }


    MatrixBoolean MatrixBoolean::at(const Range& rowRange, MatrixBoolean::Index column) const {
        return MatrixBoolean(new Private(impl->at(M::Range(rowRange), column)));
    }


    MatrixBoolean MatrixBoolean::at(MatrixBoolean::Index row, const Range& columnRange) const {
        return MatrixBoolean(new Private(impl->at(row, M::Range(columnRange))));
    }


    MatrixBoolean MatrixBoolean::at(const Range& rowRange, const Range& columnRange) const {
        return MatrixBoolean(new Private(impl->at(M::Range(rowRange), M::Range(columnRange))));
    }


    void MatrixBoolean::update(Matrix::Index row, Matrix::Index column, MatrixBoolean::Scalar newValue) {
        impl->update(row, column, newValue);
    }


    void MatrixBoolean::update(Matrix::Index index, MatrixBoolean::Scalar newValue) {
        impl->update(index, newValue);
    }


    MatrixBoolean MatrixBoolean::combineLeftToRight() const {
        return MatrixBoolean(new Private(*impl));
    }


    MatrixBoolean MatrixBoolean::combineLeftToRight(const MatrixBoolean& other) const {
        return MatrixBoolean(new Private(impl->combineLeftToRight(*other.impl)));
    }


    MatrixBoolean MatrixBoolean::combineTopToBottom() const {
        return MatrixBoolean(new Private(*impl));
    }


    MatrixBoolean MatrixBoolean::combineTopToBottom(const MatrixBoolean& other) const {
        return MatrixBoolean(new Private(impl->combineTopToBottom(*other.impl)));
    }


    MatrixBoolean MatrixBoolean::columnReverse() const {
        return MatrixBoolean(new Private(impl->columnReverse()));
    }


    MatrixBoolean MatrixBoolean::rowReverse() const {
        return MatrixBoolean(new Private(impl->rowReverse()));
    }


    bool MatrixBoolean::isSquare() const {
        return impl->isSquare();
    }


    MatrixBoolean MatrixBoolean::transpose() const {
        return MatrixBoolean(new Private(impl->transpose()));
    }


    MatrixBoolean MatrixBoolean::conj() const {
        return MatrixBoolean(new Private(impl->conj()));
    }


    MatrixBoolean MatrixBoolean::adjoint() const {
        return MatrixBoolean(new Private(impl->adjoint()));
    }


    MatrixBoolean::MatrixType MatrixBoolean::matrixType() const {
        return impl->matrixType();
    }


    MatrixBoolean& MatrixBoolean::operator=(const MatrixBoolean& other) {
        (*impl) = (*other.impl);
        return *this;
    }


    MatrixBoolean& MatrixBoolean::operator=(const MatrixInteger& other) {
        (*impl) = (*other.impl);
        return *this;
    }


    MatrixBoolean& MatrixBoolean::operator=(const MatrixReal& other) {
        (*impl) = (*other.impl);
        return *this;
    }


    MatrixBoolean& MatrixBoolean::operator=(const MatrixComplex& other) {
        (*impl) = (*other.impl);
        return *this;
    }


    MatrixBoolean& MatrixBoolean::operator=(const Variant& other) {
        bool          ok;
        MatrixBoolean v = other.toMatrixBoolean(&ok);

        if (ok) {
            (*impl) = (*v.impl);
        } else {
            throw InvalidRuntimeConversion(other.valueType(), ValueType::MATRIX_BOOLEAN);
        }

        return *this;
    }


    bool MatrixBoolean::operator==(const MatrixBoolean& other) const {
        return (*impl) == (*other.impl);
    }


    bool MatrixBoolean::operator==(const MatrixInteger& other) const {
        return (*impl) == (*other.impl);
    }


    bool MatrixBoolean::operator==(const MatrixReal& other) const {
        return (*impl) == (*other.impl);
    }


    bool MatrixBoolean::operator==(const MatrixComplex& other) const {
        return (*impl) == (*other.impl);
    }


    bool MatrixBoolean::operator!=(const MatrixBoolean& other) const {
        return (*impl) != (*other.impl);
    }


    bool MatrixBoolean::operator!=(const MatrixInteger& other) const {
        return (*impl) != (*other.impl);
    }


    bool MatrixBoolean::operator!=(const MatrixReal& other) const {
        return (*impl) != (*other.impl);
    }


    bool MatrixBoolean::operator!=(const MatrixComplex& other) const {
        return (*impl) != (*other.impl);
    }
}
