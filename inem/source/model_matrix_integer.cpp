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
* This file implements the \ref Model::MatrixInteger class.
***********************************************************************************************************************/

#include <algorithm>
#include <cassert>

#include "model_api_types.h"
#include "model_exceptions.h"
#include "model_intrinsic_types.h"
#include "model_range.h"
#include "m_range.h"
#include "model_matrix.h"
#include "model_matrix_boolean.h"
#include "model_matrix_boolean_private.h"
#include "model_matrix_integer.h"
#include "model_matrix_integer_private.h"
#include "model_matrix_complex.h"
#include "model_matrix_complex_private.h"
#include "model_matrix_integer.h"
#include "model_matrix_integer_private.h"

namespace Model {
    MatrixInteger::MatrixInteger() {
        impl = new Private();
    }


    MatrixInteger::MatrixInteger(Integer numberRows, Integer numberColumns) {
        impl = new Private(numberRows, numberColumns);
    }


    MatrixInteger::MatrixInteger(Integer numberRows, Integer numberColumns, const MatrixInteger::Scalar* matrixData) {
        impl = new Private(numberRows, numberColumns, matrixData);
    }


    MatrixInteger::MatrixInteger(const MatrixBoolean& other) {
        impl = new Private(*other.impl);
    }


    MatrixInteger::MatrixInteger(const MatrixInteger& other) {
        impl = new Private(*other.impl);
    }


    MatrixInteger::MatrixInteger(MatrixInteger&& other) {
        impl = other.impl;
        other.impl = nullptr;
    }


    MatrixInteger::MatrixInteger(const Variant& other) {
        bool          ok;
        MatrixInteger v = other.toMatrixInteger(&ok);

        if (ok) {
            impl = new Private(*v.impl);
        } else {
            throw InvalidRuntimeConversion(other.valueType(), ValueType::MATRIX_INTEGER);
        }
    }


    MatrixInteger::~MatrixInteger() {
        if (impl != nullptr) {
            delete impl;
        }
    }


    ValueType MatrixInteger::coefficientValueType() const {
        return ValueType::INTEGER;
    }


    bool MatrixInteger::toFile(const char* filename, DataFileFormat fileFormat) const {
        return impl->toFile(filename, fileFormat);
    }


    MatrixInteger MatrixInteger::fromFile(const char* filename) {
        return MatrixInteger(new Private(M::MatrixInteger::fromFile(filename)));
    }


    MatrixInteger::Index MatrixInteger::numberRows() const {
        return impl->numberRows();
    }


    MatrixInteger::Index MatrixInteger::numberColumns() const {
        return impl->numberColumns();
    }


    MatrixInteger::Index MatrixInteger::numberCoefficients() const {
        return impl->numberCoefficients();
    }


    MatrixInteger MatrixInteger::identity(Index numberRows, Index numberColumns) {
        return MatrixInteger(new Private(M::MatrixInteger::identity(numberRows, numberColumns)));
    }


    MatrixInteger MatrixInteger::identity(Index numberRowColumns) {
        return MatrixInteger(new Private(M::MatrixInteger::identity(numberRowColumns)));
    }


    MatrixInteger MatrixInteger::ones(Index numberRows, Index numberColumns) {
        return MatrixInteger(new Private(M::MatrixInteger::ones(numberRows, numberColumns)));
    }


    MatrixInteger MatrixInteger::ones(Index numberRowColumns) {
        return MatrixInteger(new Private(M::MatrixInteger::ones(numberRowColumns)));
    }


    MatrixInteger MatrixInteger::diagonalEntries() const {
        return MatrixInteger(new Private(impl->diagonalEntries()));
    }


    MatrixInteger MatrixInteger::diagonal() const {
        return MatrixInteger(new Private(impl->diagonal()));
    }


    Variant MatrixInteger::value(MatrixInteger::Index row, MatrixInteger::Index column) const {
        return impl->value(row, column);
    }


    Variant MatrixInteger::value(MatrixInteger::Index index) const {
        return impl->value(index);
    }


    bool MatrixInteger::setValue(MatrixInteger::Index row, MatrixInteger::Index column, const Variant& value) {
        return impl->setValue(row, column, value);
    }


    bool MatrixInteger::setValue(MatrixInteger::Index index, const Variant& value) {
        return impl->setValue(index, value);
    }


    void MatrixInteger::resize(MatrixInteger::Index newNumberRows, MatrixInteger::Index newNumberColumns) {
        impl->resize(newNumberRows, newNumberColumns);
    }


    MatrixInteger::Scalar MatrixInteger::at(MatrixInteger::Index row, MatrixInteger::Index column) const {
        return impl->at(row, column);
    }


    MatrixInteger::Scalar MatrixInteger::at(MatrixInteger::Index index) const {
        return impl->at(index);
    }


    MatrixInteger MatrixInteger::at(const Range& rowRange, MatrixInteger::Index column) const {
        return MatrixInteger(new Private(impl->at(M::Range(rowRange), column)));
    }


    MatrixInteger MatrixInteger::at(MatrixInteger::Index row, const Range& columnRange) const {
        return MatrixInteger(new Private(impl->at(row, M::Range(columnRange))));
    }


    MatrixInteger MatrixInteger::at(const Range& rowRange, const Range& columnRange) const {
        return MatrixInteger(new Private(impl->at(rowRange, M::Range(columnRange))));
    }


    void MatrixInteger::update(Matrix::Index row, Matrix::Index column, MatrixInteger::Scalar newValue) {
        impl->update(row, column, newValue);
    }


    void MatrixInteger::update(Matrix::Index index, MatrixInteger::Scalar newValue) {
        impl->update(index, newValue);
    }


    MatrixInteger MatrixInteger::combineLeftToRight() const {
        return MatrixInteger(new Private(*impl));
    }


    MatrixInteger MatrixInteger::combineLeftToRight(const MatrixBoolean& other) const {
        return MatrixInteger(new Private(impl->combineLeftToRight(*other.impl)));
    }


    MatrixInteger MatrixInteger::combineLeftToRight(const MatrixInteger& other) const {
        return MatrixInteger(new Private(impl->combineLeftToRight(*other.impl)));
    }


    MatrixInteger MatrixInteger::combineTopToBottom() const {
        return MatrixInteger(new Private(*impl));
    }


    MatrixInteger MatrixInteger::combineTopToBottom(const MatrixBoolean& other) const {
        return MatrixInteger(new Private(impl->combineTopToBottom(*other.impl)));
    }


    MatrixInteger MatrixInteger::combineTopToBottom(const MatrixInteger& other) const {
        return MatrixInteger(new Private(impl->combineTopToBottom(*other.impl)));
    }


    MatrixInteger MatrixInteger::columnReverse() const {
        return MatrixInteger(new Private(impl->columnReverse()));
    }


    MatrixInteger MatrixInteger::rowReverse() const {
        return MatrixInteger(new Private(impl->rowReverse()));
    }


    bool MatrixInteger::isSquare() const {
        return impl->isSquare();
    }


    MatrixInteger MatrixInteger::transpose() const {
        return MatrixInteger(new Private(impl->transpose()));
    }


    MatrixInteger MatrixInteger::conj() const {
        return MatrixInteger(new Private(impl->conj()));
    }


    MatrixInteger MatrixInteger::adjoint() const {
        return MatrixInteger(new Private(impl->adjoint()));
    }


    MatrixInteger MatrixInteger::hadamard(const MatrixInteger& other) const {
        return MatrixInteger(new Private(impl->hadamard(*other.impl)));
    }


    MatrixInteger MatrixInteger::kronecker(const MatrixInteger& other) const {
        return MatrixInteger(new Private(impl->kronecker(*other.impl)));
    }


    MatrixInteger::MatrixType MatrixInteger::matrixType() const {
        return impl->matrixType();
    }


    const MatrixInteger::Scalar* MatrixInteger::data() const {
        return impl->data();
    }


    MatrixInteger& MatrixInteger::operator=(const MatrixBoolean& other) {
        (*impl) = (*other.impl);
        return *this;
    }


    MatrixInteger& MatrixInteger::operator=(const MatrixInteger& other) {
        (*impl) = (*other.impl);
        return *this;
    }


    MatrixInteger& MatrixInteger::operator=(const Variant& other) {
        bool          ok;
        MatrixInteger v = other.toMatrixInteger(&ok);

        if (ok) {
            (*impl) = (*v.impl);
        } else {
            throw InvalidRuntimeConversion(other.valueType(), ValueType::MATRIX_INTEGER);
        }

        return *this;
    }


    MatrixInteger& MatrixInteger::operator+=(const MatrixBoolean& v) {
        (*impl) += (*v.impl);
        return *this;
    }


    MatrixInteger& MatrixInteger::operator+=(const MatrixInteger& v) {
        (*impl) += (*v.impl);
        return *this;
    }


    MatrixInteger& MatrixInteger::operator-=(const MatrixBoolean& v) {
        (*impl) -= (*v.impl);
        return *this;
    }


    MatrixInteger& MatrixInteger::operator-=(const MatrixInteger& v) {
        (*impl) -= (*v.impl);
        return *this;
    }


    MatrixInteger& MatrixInteger::operator*=(const MatrixBoolean& v) {
        (*impl) *= (*v.impl);
        return *this;
    }


    MatrixInteger& MatrixInteger::operator*=(const MatrixInteger& v) {
        (*impl) *= (*v.impl);
        return *this;
    }


    MatrixInteger& MatrixInteger::operator*=(Boolean v) {
        (*impl) *= v;
        return *this;
    }


    MatrixInteger& MatrixInteger::operator*=(Integer v) {
        (*impl) *= v;
        return *this;
    }


    MatrixInteger MatrixInteger::operator+(const MatrixBoolean& v) const {
        return MatrixInteger(new Private((*impl) + (*v.impl)));
    }


    MatrixInteger MatrixInteger::operator+(const MatrixInteger& v) const {
        return MatrixInteger(new Private((*impl) + (*v.impl)));
    }


    MatrixInteger MatrixInteger::operator-(const MatrixBoolean& v) const {
        return MatrixInteger(new Private((*impl) - (*v.impl)));
    }


    MatrixInteger MatrixInteger::operator-(const MatrixInteger& v) const {
        return MatrixInteger(new Private((*impl) - (*v.impl)));
    }


    MatrixInteger MatrixInteger::operator*(const MatrixBoolean& v) const {
        return MatrixInteger(new Private((*impl) * (*v.impl)));
    }


    MatrixInteger MatrixInteger::operator*(const MatrixInteger& v) const {
        return MatrixInteger(new Private((*impl) * (*v.impl)));
    }


    MatrixInteger MatrixInteger::operator*(Boolean v) const {
        return MatrixInteger(new Private((*impl) * v));
    }


    MatrixInteger MatrixInteger::operator*(Integer v) const {
        return MatrixInteger(new Private((*impl) * v));
    }


    MatrixInteger MatrixInteger::operator-() const {
        return MatrixInteger(new Private(-(*impl)));
    }


    bool MatrixInteger::operator==(const MatrixBoolean& other) const {
        return (*impl) == (*other.impl);
    }


    bool MatrixInteger::operator==(const MatrixInteger& other) const {
        return (*impl) == (*other.impl);
    }


    bool MatrixInteger::operator!=(const MatrixBoolean& other) const {
        return (*impl) != (*other.impl);
    }


    bool MatrixInteger::operator!=(const MatrixInteger& other) const {
        return (*impl) != (*other.impl);
    }


    MatrixInteger operator*(Boolean a, const MatrixInteger& b) {
        return b * a;
    }


    MatrixInteger operator*(Integer a, const MatrixInteger& b) {
        return b * a;
    }


    MatrixInteger operator*(Integer a, const MatrixBoolean& b) {
        return MatrixInteger(b) * a;
    }


    MatrixInteger operator+(const MatrixBoolean& a, const MatrixInteger& b) {
        return MatrixInteger(a) + b;
    }


    MatrixInteger operator-(const MatrixBoolean& a, const MatrixInteger& b) {
        return MatrixInteger(a) - b;
    }
}
