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
* This file implements the \ref Model::MatrixReal class.
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
#include "model_matrix_real.h"
#include "model_matrix_real_private.h"

namespace Model {
    MatrixReal::MatrixReal() {
        impl = new Private();
    }


    MatrixReal::MatrixReal(Integer numberRows, Integer numberColumns) {
        impl = new Private(numberRows, numberColumns);
    }


    MatrixReal::MatrixReal(Integer numberRows, Integer numberColumns, const MatrixReal::Scalar* matrixData) {
        impl = new Private(numberRows, numberColumns, matrixData);
    }


    MatrixReal::MatrixReal(const MatrixBoolean& other) {
        impl = new Private(*other.impl);
    }


    MatrixReal::MatrixReal(const MatrixInteger& other) {
        impl = new Private(*other.impl);
    }


    MatrixReal::MatrixReal(const MatrixReal& other) {
        impl = new Private(*other.impl);
    }


    MatrixReal::MatrixReal(MatrixReal&& other) {
        impl = other.impl;
        other.impl = nullptr;
    }


    MatrixReal::MatrixReal(const Variant& other) {
        bool ok;
        MatrixReal v = other.toMatrixReal(&ok);

        if (ok) {
            impl = new Private(*v.impl);
        } else {
            throw InvalidRuntimeConversion(other.valueType(), ValueType::MATRIX_REAL);
        }
    }


    MatrixReal::~MatrixReal() {
        if (impl != nullptr) {
            delete impl;
        }
    }


    ValueType MatrixReal::coefficientValueType() const {
        return ValueType::REAL;
    }


    bool MatrixReal::toFile(const char* filename, DataFileFormat fileFormat) const {
        return impl->toFile(filename, fileFormat);
    }


    MatrixReal MatrixReal::fromFile(const char* filename) {
        return MatrixReal(new Private(M::MatrixReal::fromFile(filename)));
    }


    MatrixReal::Index MatrixReal::numberRows() const {
        return impl->numberRows();
    }


    MatrixReal::Index MatrixReal::numberColumns() const {
        return impl->numberColumns();
    }


    MatrixReal::Index MatrixReal::numberCoefficients() const {
        return impl->numberCoefficients();
    }


    MatrixReal MatrixReal::identity(Index numberRows, Index numberColumns) {
        return MatrixReal(new Private(M::MatrixReal::identity(numberRows, numberColumns)));
    }


    MatrixReal MatrixReal::identity(Index numberRowColumns) {
        return MatrixReal(new Private(M::MatrixReal::identity(numberRowColumns)));
    }


    MatrixReal MatrixReal::ones(Index numberRows, Index numberColumns) {
        return MatrixReal(new Private(M::MatrixReal::ones(numberRows, numberColumns)));
    }


    MatrixReal MatrixReal::ones(Index numberRowColumns) {
        return MatrixReal(new Private(M::MatrixReal::ones(numberRowColumns)));
    }


    MatrixReal MatrixReal::diagonalEntries() const {
        return MatrixReal(new Private(impl->diagonalEntries()));
    }


    MatrixReal MatrixReal::diagonal() const {
        return MatrixReal(new Private(impl->diagonal()));
    }


    Variant MatrixReal::value(MatrixReal::Index row, MatrixReal::Index column) const {
        return impl->value(row, column);
    }


    Variant MatrixReal::value(MatrixReal::Index index) const {
        return impl->value(index);
    }


    bool MatrixReal::setValue(MatrixReal::Index row, MatrixReal::Index column, const Variant& value) {
        return impl->setValue(row, column, value);
    }


    bool MatrixReal::setValue(MatrixReal::Index index, const Variant& value) {
        return impl->setValue(index, value);
    }


    void MatrixReal::resize(MatrixReal::Index newNumberRows, MatrixReal::Index newNumberColumns) {
        impl->resize(newNumberRows, newNumberColumns);
    }


    MatrixReal::Scalar MatrixReal::at(MatrixReal::Index row, MatrixReal::Index column) const {
        return impl->at(row, column);
    }


    MatrixReal::Scalar MatrixReal::at(MatrixReal::Index index) const {
        return impl->at(index);
    }


    MatrixReal MatrixReal::at(const Range& rowRange, MatrixReal::Index column) const {
        return MatrixReal(new Private(impl->at(M::Range(rowRange), column)));
    }


    MatrixReal MatrixReal::at(MatrixReal::Index row, const Range& columnRange) const {
        return MatrixReal(new Private(impl->at(row, M::Range(columnRange))));
    }


    MatrixReal MatrixReal::at(const Range& rowRange, const Range& columnRange) const {
        return MatrixReal(new Private(impl->at(M::Range(rowRange), M::Range(columnRange))));
    }


    void MatrixReal::update(Matrix::Index row, Matrix::Index column, MatrixReal::Scalar newValue) {
        impl->update(row, column, newValue);
    }


    void MatrixReal::update(Matrix::Index index, MatrixReal::Scalar newValue) {
        impl->update(index, newValue);
    }


    MatrixReal MatrixReal::combineLeftToRight() const {
        return MatrixReal(new Private(*impl));
    }


    MatrixReal MatrixReal::combineLeftToRight(const MatrixBoolean& other) const {
        return MatrixReal(new Private(impl->combineLeftToRight(*other.impl)));
    }


    MatrixReal MatrixReal::combineLeftToRight(const MatrixInteger& other) const {
        return MatrixReal(new Private(impl->combineLeftToRight(*other.impl)));
    }


    MatrixReal MatrixReal::combineLeftToRight(const MatrixReal& other) const {
        return MatrixReal(new Private(impl->combineLeftToRight(*other.impl)));
    }


    MatrixReal MatrixReal::combineTopToBottom() const {
        return MatrixReal(new Private(*impl));
    }


    MatrixReal MatrixReal::combineTopToBottom(const MatrixBoolean& other) const {
        return MatrixReal(new Private(impl->combineTopToBottom(*other.impl)));
    }


    MatrixReal MatrixReal::combineTopToBottom(const MatrixInteger& other) const {
        return MatrixReal(new Private(impl->combineTopToBottom(*other.impl)));
    }


    MatrixReal MatrixReal::combineTopToBottom(const MatrixReal& other) const {
        return MatrixReal(new Private(impl->combineTopToBottom(*other.impl)));
    }


    MatrixReal MatrixReal::columnReverse() const {
        return MatrixReal(new Private(impl->columnReverse()));
    }


    MatrixReal MatrixReal::rowReverse() const {
        return MatrixReal(new Private(impl->rowReverse()));
    }


    bool MatrixReal::isSquare() const {
        return impl->isSquare();
    }


    bool MatrixReal::isSymmetric(Real relativeTolerance) const {
        return impl->isSymmetric(relativeTolerance);
    }


    bool MatrixReal::isHermitian(Real relativeTolerance) const {
        return impl->isHermitian(relativeTolerance);
    }


    bool MatrixReal::isSkewSymmetric(Real relativeTolerance) const {
        return impl->isSkewSymmetric(relativeTolerance);
    }


    bool MatrixReal::isSkewHermitian(Real relativeTolerance) const {
        return impl->isSkewHermitian(relativeTolerance);
    }


    bool MatrixReal::isNormal(Real relativeTolerance) const {
        return impl->isNormal(relativeTolerance);
    }


    Integer MatrixReal::rank(Real epsilon) const {
        return impl->rank(epsilon);
    }


    MatrixReal::Scalar MatrixReal::determinant() const {
        return impl->determinant();
    }


    MatrixReal MatrixReal::transpose() const {
        return MatrixReal(new Private(impl->transpose()));
    }


    MatrixReal MatrixReal::conj() const {
        return MatrixReal(new Private(impl->conj()));
    }


    MatrixReal MatrixReal::adjoint() const {
        return MatrixReal(new Private(impl->adjoint()));
    }


    MatrixReal MatrixReal::inverse() const {
        return MatrixReal(new Private(impl->inverse()));
    }


    Tuple MatrixReal::plu() const {
        return Tuple(impl->plu());
    }


    Tuple MatrixReal::svd() const {
        return Tuple(impl->svd());
    }


    Tuple MatrixReal::qr() const {
        return Tuple(impl->qr());
    }


    Tuple MatrixReal::lq() const {
        return Tuple(impl->lq());
    }


    MatrixReal MatrixReal::cholesky() const {
        return MatrixReal(new Private(impl->cholesky()));
    }


    MatrixReal MatrixReal::upperCholesky() const {
        return MatrixReal(new Private(impl->upperCholesky()));
    }


    Tuple MatrixReal::hessenberg() const {
        return Tuple(impl->hessenberg());
    }


    MatrixReal MatrixReal::dct() const {
        return MatrixReal(new Private(impl->dct()));
    }


    MatrixReal MatrixReal::idct() const {
        return MatrixReal(new Private(impl->idct()));
    }


    MatrixComplex MatrixReal::hilbertTransform() const {
        return MatrixComplex(new MatrixComplex::Private(impl->hilbertTransform()));
    }


    Real MatrixReal::conditionNumber() const {
        return impl->conditionNumber();
    }


    Real MatrixReal::pNorm(Integer p) const {
        return impl->pNorm(p);
    }


    Real MatrixReal::euclideanNorm() const {
        return impl->euclideanNorm();
    }


    Real MatrixReal::oneNorm() const {
        return impl->oneNorm();
    }


    Real MatrixReal::infinityNorm() const {
        return impl->infinityNorm();
    }


    Tuple MatrixReal::equilibrate() const {
        return Tuple(impl->equilibrate());
    }


    MatrixReal MatrixReal::solve(const MatrixReal& y) const {
        return MatrixReal(new Private(impl->solve(*y.impl)));
    }


    MatrixReal MatrixReal::leastSquares(const MatrixReal& b) const {
        return MatrixReal(new Private(impl->leastSquares(*b.impl)));
    }


    MatrixReal MatrixReal::hadamard(const MatrixReal& other) const {
        return MatrixReal(new Private(impl->hadamard(*other.impl)));
    }


    MatrixReal MatrixReal::kronecker(const MatrixReal& other) const {
        return MatrixReal(new Private(impl->kronecker(*other.impl)));
    }


    MatrixReal::MatrixType MatrixReal::matrixType() const {
        return impl->matrixType();
    }


    const MatrixReal::Scalar* MatrixReal::data() const {
        return impl->data();
    }


    MatrixReal MatrixReal::floor() const {
        return MatrixReal(new Private(impl->floor()));
    }


    MatrixReal MatrixReal::ceil() const {
        return MatrixReal(new Private(impl->ceil()));
    }


    MatrixReal MatrixReal::nint() const {
        return MatrixReal(new Private(impl->nint()));
    }


    MatrixInteger MatrixReal::truncateToInteger() const {
        return MatrixInteger(new MatrixInteger::Private(impl->truncateToInteger()));
    }


    MatrixInteger MatrixReal::floorToInteger() const {
        return MatrixInteger(new MatrixInteger::Private(impl->floorToInteger()));
    }


    MatrixInteger MatrixReal::ceilToInteger() const {
        return MatrixInteger(new MatrixInteger::Private(impl->ceilToInteger()));
    }


    MatrixInteger MatrixReal::nintToInteger() const {
        return MatrixInteger(new MatrixInteger::Private(impl->nintToInteger()));
    }


    MatrixReal& MatrixReal::operator=(const MatrixBoolean& other) {
        (*impl) = (*other.impl);
        return *this;
    }


    MatrixReal& MatrixReal::operator=(const MatrixInteger& other) {
        (*impl) = (*other.impl);
        return *this;
    }


    MatrixReal& MatrixReal::operator=(const MatrixReal& other) {
        (*impl) = (*other.impl);
        return *this;
    }


    MatrixReal& MatrixReal::operator=(const Variant& other) {
        bool      ok;
        MatrixReal v = other.toMatrixReal(&ok);

        if (ok) {
            (*impl) = (*v.impl);
        } else {
            throw InvalidRuntimeConversion(other.valueType(), ValueType::MATRIX_REAL);
        }

        return *this;
    }


    MatrixReal& MatrixReal::operator+=(const MatrixBoolean& v) {
        (*impl) += (*v.impl);
        return *this;
    }


    MatrixReal& MatrixReal::operator+=(const MatrixInteger& v) {
        (*impl) += (*v.impl);
        return *this;
    }


    MatrixReal& MatrixReal::operator+=(const MatrixReal& v) {
        (*impl) += (*v.impl);
        return *this;
    }


    MatrixReal& MatrixReal::operator-=(const MatrixBoolean& v) {
        (*impl) -= (*v.impl);
        return *this;
    }


    MatrixReal& MatrixReal::operator-=(const MatrixInteger& v) {
        (*impl) -= (*v.impl);
        return *this;
    }


    MatrixReal& MatrixReal::operator-=(const MatrixReal& v) {
        (*impl) -= (*v.impl);
        return *this;
    }


    MatrixReal& MatrixReal::operator*=(const MatrixBoolean& v) {
        (*impl) *= (*v.impl);
        return *this;
    }


    MatrixReal& MatrixReal::operator*=(const MatrixInteger& v) {
        (*impl) *= (*v.impl);
        return *this;
    }


    MatrixReal& MatrixReal::operator*=(const MatrixReal& v) {
        (*impl) *= (*v.impl);
        return *this;
    }


    MatrixReal& MatrixReal::operator*=(Boolean v) {
        (*impl) *= v;
        return *this;
    }


    MatrixReal& MatrixReal::operator*=(Integer v) {
        (*impl) *= v;
        return *this;
    }


    MatrixReal& MatrixReal::operator*=(Real v) {
        (*impl) *= v;
        return *this;
    }


    MatrixReal& MatrixReal::operator/=(Integer v) {
        (*impl) /= v;
        return *this;
    }


    MatrixReal& MatrixReal::operator/=(Real v) {
        (*impl) /= v;
        return *this;
    }


    MatrixReal MatrixReal::operator+(const MatrixBoolean& v) const {
        return MatrixReal(new Private((*impl) + (*v.impl)));
    }


    MatrixReal MatrixReal::operator+(const MatrixInteger& v) const {
        return MatrixReal(new Private((*impl) + (*v.impl)));
    }


    MatrixReal MatrixReal::operator+(const MatrixReal& v) const {
        return MatrixReal(new Private((*impl) + (*v.impl)));
    }


    MatrixReal MatrixReal::operator-(const MatrixBoolean& v) const {
        return MatrixReal(new Private((*impl) - (*v.impl)));
    }


    MatrixReal MatrixReal::operator-(const MatrixInteger& v) const {
        return MatrixReal(new Private((*impl) - (*v.impl)));
    }


    MatrixReal MatrixReal::operator-(const MatrixReal& v) const {
        return MatrixReal(new Private((*impl) - (*v.impl)));
    }


    MatrixReal MatrixReal::operator*(const MatrixBoolean& v) const {
        return MatrixReal(new Private((*impl) * (*v.impl)));
    }


    MatrixReal MatrixReal::operator*(const MatrixInteger& v) const {
        return MatrixReal(new Private((*impl) * (*v.impl)));
    }


    MatrixReal MatrixReal::operator*(const MatrixReal& v) const {
        return MatrixReal(new Private((*impl) * (*v.impl)));
    }


    MatrixReal MatrixReal::operator*(Boolean v) const {
        return MatrixReal(new Private((*impl) * v));
    }


    MatrixReal MatrixReal::operator*(Integer v) const {
        return MatrixReal(new Private((*impl) * v));
    }


    MatrixReal MatrixReal::operator*(Real v) const {
        return MatrixReal(new Private((*impl) * v));
    }


    MatrixReal MatrixReal::operator/(Integer v) const {
        return MatrixReal(new Private((*impl) / v));
    }


    MatrixReal MatrixReal::operator/(Real v) const {
        return MatrixReal(new Private((*impl) / v));
    }


    MatrixReal MatrixReal::operator-() const {
        return MatrixReal(new Private(-(*impl)));
    }


    bool MatrixReal::operator==(const MatrixBoolean& other) const {
        return (*impl) == (*other.impl);
    }


    bool MatrixReal::operator==(const MatrixInteger& other) const {
        return (*impl) == (*other.impl);
    }


    bool MatrixReal::operator==(const MatrixReal& other) const {
        return (*impl) == (*other.impl);
    }


    bool MatrixReal::operator!=(const MatrixBoolean& other) const {
        return (*impl) != (*other.impl);
    }


    bool MatrixReal::operator!=(const MatrixInteger& other) const {
        return (*impl) != (*other.impl);
    }


    bool MatrixReal::operator!=(const MatrixReal& other) const {
        return (*impl) != (*other.impl);
    }


    MatrixReal operator*(Boolean a, const MatrixReal& b) {
        return b * a;
    }


    MatrixReal operator*(Integer a, const MatrixReal& b) {
        return b * a;
    }


    MatrixReal operator*(Real a, const MatrixReal& b) {
        return b * a;
    }


    MatrixReal operator*(const MatrixBoolean& a, const MatrixReal& b) {
        return MatrixReal(a) * b;
    }


    MatrixReal operator*(const MatrixInteger& a, const MatrixReal& b) {
        return MatrixReal(a) * b;
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
