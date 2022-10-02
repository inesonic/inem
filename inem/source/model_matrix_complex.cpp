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
* This file implements the \ref Model::MatrixComplex class.
***********************************************************************************************************************/

#include <algorithm>
#include <cassert>

#include "model_api_types.h"
#include "model_exceptions.h"
#include "model_intrinsic_types.h"
#include "model_complex.h"
#include "model_range.h"
#include "m_range.h"
#include "model_matrix.h"
#include "model_matrix_boolean.h"
#include "model_matrix_boolean_private.h"
#include "model_matrix_integer.h"
#include "model_matrix_integer_private.h"
#include "model_matrix_real.h"
#include "model_matrix_real_private.h"
#include "model_matrix_complex.h"
#include "model_matrix_complex_private.h"

namespace Model {
    MatrixComplex::MatrixComplex() {
        impl = new Private();
    }


    MatrixComplex::MatrixComplex(Integer numberRows, Integer numberColumns) {
        impl = new Private(numberRows, numberColumns);
    }


    MatrixComplex::MatrixComplex(Integer numberRows, Integer numberColumns, const MatrixComplex::Scalar* matrixData) {
        impl = new Private(numberRows, numberColumns, matrixData);
    }


    MatrixComplex::MatrixComplex(const MatrixBoolean& other) {
        impl = new Private(*other.impl);
    }


    MatrixComplex::MatrixComplex(const MatrixInteger& other) {
        impl = new Private(*other.impl);
    }


    MatrixComplex::MatrixComplex(const MatrixReal& other) {
        impl = new Private(*other.impl);
    }


    MatrixComplex::MatrixComplex(const MatrixComplex& other) {
        impl = new Private(*other.impl);
    }


    MatrixComplex::MatrixComplex(MatrixComplex&& other) {
        impl = other.impl;
        other.impl = nullptr;
    }


    MatrixComplex::MatrixComplex(const Variant& other) {
        bool          ok;
        MatrixComplex v = other.toMatrixComplex(&ok);

        if (ok) {
            impl = new Private(*v.impl);
        } else {
            throw InvalidRuntimeConversion(other.valueType(), ValueType::MATRIX_COMPLEX);
        }
    }


    MatrixComplex::~MatrixComplex() {
        if (impl != nullptr) {
            delete impl;
        }
    }


    ValueType MatrixComplex::coefficientValueType() const {
        return ValueType::COMPLEX;
    }


    bool MatrixComplex::toFile(const char* filename, DataFileFormat fileFormat) const {
        return impl->toFile(filename, fileFormat);
    }


    MatrixComplex MatrixComplex::fromFile(const char* filename) {
        return MatrixComplex(new Private(M::MatrixComplex::fromFile(filename)));
    }


    MatrixComplex::Index MatrixComplex::numberRows() const {
        return impl->numberRows();
    }


    MatrixComplex::Index MatrixComplex::numberColumns() const {
        return impl->numberColumns();
    }


    MatrixComplex::Index MatrixComplex::numberCoefficients() const {
        return impl->numberCoefficients();
    }


    MatrixComplex MatrixComplex::identity(Index numberRows, Index numberColumns) {
        return MatrixComplex(new Private(M::MatrixComplex::identity(numberRows, numberColumns)));
    }


    MatrixComplex MatrixComplex::identity(Index numberRowColumns) {
        return MatrixComplex(new Private(M::MatrixComplex::identity(numberRowColumns)));
    }


    MatrixComplex MatrixComplex::ones(Index numberRows, Index numberColumns) {
        return MatrixComplex(new Private(M::MatrixComplex::ones(numberRows, numberColumns)));
    }


    MatrixComplex MatrixComplex::ones(Index numberRowColumns) {
        return MatrixComplex(new Private(M::MatrixComplex::ones(numberRowColumns)));
    }


    MatrixComplex MatrixComplex::diagonalEntries() const {
        return MatrixComplex(new Private(impl->diagonalEntries()));
    }


    MatrixComplex MatrixComplex::diagonal() const {
        return MatrixComplex(new Private(impl->diagonal()));
    }


    Variant MatrixComplex::value(MatrixComplex::Index row, MatrixComplex::Index column) const {
        return impl->value(row, column);
    }


    Variant MatrixComplex::value(MatrixComplex::Index index) const {
        return impl->value(index);
    }


    bool MatrixComplex::setValue(MatrixComplex::Index row, MatrixComplex::Index column, const Variant& value) {
        return impl->setValue(row, column, value);
    }


    bool MatrixComplex::setValue(MatrixComplex::Index index, const Variant& value) {
        return impl->setValue(index, value);
    }


    void MatrixComplex::resize(MatrixComplex::Index newNumberRows, MatrixComplex::Index newNumberColumns) {
        impl->resize(newNumberRows, newNumberColumns);
    }


    const MatrixComplex::Scalar& MatrixComplex::at(MatrixComplex::Index row, MatrixComplex::Index column) const {
        return impl->at(row, column);
    }


    const MatrixComplex::Scalar& MatrixComplex::at(MatrixComplex::Index index) const {
        return impl->at(index);
    }


    MatrixComplex MatrixComplex::at(const Range& rowRange, MatrixComplex::Index column) const {
        return MatrixComplex(new Private(impl->at(M::Range(rowRange), column)));
    }


    MatrixComplex MatrixComplex::at(MatrixComplex::Index row, const Range& columnRange) const {
        return MatrixComplex(new Private(impl->at(row, M::Range(columnRange))));
    }


    MatrixComplex MatrixComplex::at(const Range& rowRange, const Range& columnRange) const {
        return MatrixComplex(new Private(impl->at(rowRange, columnRange)));
    }


    void MatrixComplex::update(Matrix::Index row, Matrix::Index column, const Scalar& newValue) {
        impl->update(row, column, newValue);
    }


    void MatrixComplex::update(Matrix::Index index, const Scalar& newValue) {
        impl->update(index, newValue);
    }


    MatrixComplex MatrixComplex::combineLeftToRight() const {
        return MatrixComplex(new Private(*impl));
    }


    MatrixComplex MatrixComplex::combineLeftToRight(const MatrixBoolean& other) const {
        return MatrixComplex(new Private(impl->combineLeftToRight(*other.impl)));
    }


    MatrixComplex MatrixComplex::combineLeftToRight(const MatrixInteger& other) const {
        return MatrixComplex(new Private(impl->combineLeftToRight(*other.impl)));
    }


    MatrixComplex MatrixComplex::combineLeftToRight(const MatrixReal& other) const {
        return MatrixComplex(new Private(impl->combineLeftToRight(*other.impl)));
    }


    MatrixComplex MatrixComplex::combineLeftToRight(const MatrixComplex& other) const {
        return MatrixComplex(new Private(impl->combineLeftToRight(*other.impl)));
    }


    MatrixComplex MatrixComplex::combineTopToBottom() const {
        return MatrixComplex(new Private(*impl));
    }


    MatrixComplex MatrixComplex::combineTopToBottom(const MatrixBoolean& other) const {
        return MatrixComplex(new Private(impl->combineTopToBottom(*other.impl)));
    }


    MatrixComplex MatrixComplex::combineTopToBottom(const MatrixInteger& other) const {
        return MatrixComplex(new Private(impl->combineTopToBottom(*other.impl)));
    }


    MatrixComplex MatrixComplex::combineTopToBottom(const MatrixReal& other) const {
        return MatrixComplex(new Private(impl->combineTopToBottom(*other.impl)));
    }


    MatrixComplex MatrixComplex::combineTopToBottom(const MatrixComplex& other) const {
        return MatrixComplex(new Private(impl->combineTopToBottom(*other.impl)));
    }


    MatrixComplex MatrixComplex::columnReverse() const {
        return MatrixComplex(new Private(impl->columnReverse()));
    }


    MatrixComplex MatrixComplex::rowReverse() const {
        return MatrixComplex(new Private(impl->rowReverse()));
    }


    bool MatrixComplex::isSquare() const {
        return impl->isSquare();
    }


    bool MatrixComplex::isSymmetric(Real relativeTolerance) const {
        return impl->isSymmetric(relativeTolerance);
    }


    bool MatrixComplex::isHermitian(Real relativeTolerance) const {
        return impl->isHermitian(relativeTolerance);
    }


    bool MatrixComplex::isSkewSymmetric(Real relativeTolerance) const {
        return impl->isSkewSymmetric(relativeTolerance);
    }


    bool MatrixComplex::isSkewHermitian(Real relativeTolerance) const {
        return impl->isSkewHermitian(relativeTolerance);
    }


    bool MatrixComplex::isNormal(Real relativeTolerance) const {
        return impl->isNormal(relativeTolerance);
    }


    Integer MatrixComplex::rank(Real epsilon) const {
        return impl->rank(epsilon);
    }


    MatrixComplex::Scalar MatrixComplex::determinant() const {
        return impl->determinant();
    }


    MatrixComplex MatrixComplex::transpose() const {
        return MatrixComplex(new Private(impl->transpose()));
    }


    MatrixComplex MatrixComplex::conj() const {
        return MatrixComplex(new Private(impl->conj()));
    }


    MatrixComplex MatrixComplex::adjoint() const {
        return MatrixComplex(new Private(impl->adjoint()));
    }


    MatrixComplex MatrixComplex::inverse() const {
        return MatrixComplex(new Private(impl->inverse()));
    }


    Tuple MatrixComplex::plu() const {
        return Tuple(impl->plu());
    }


    Tuple MatrixComplex::svd() const {
        return Tuple(impl->svd());
    }


    Tuple MatrixComplex::qr() const {
        return Tuple(impl->qr());
    }


    Tuple MatrixComplex::lq() const {
        return Tuple(impl->lq());
    }


    MatrixComplex MatrixComplex::cholesky() const {
        return MatrixComplex(new Private(impl->cholesky()));
    }


    MatrixComplex MatrixComplex::upperCholesky() const {
        return MatrixComplex(new Private(impl->upperCholesky()));
    }


    Tuple MatrixComplex::hessenberg() const {
        return Tuple(impl->hessenberg());
    }


    Tuple MatrixComplex::schur() const {
        return Tuple(impl->schur());
    }


    Tuple MatrixComplex::eigenvectors() const {
        return Tuple(impl->eigenvectors());
    }


    MatrixComplex MatrixComplex::dft() const {
        return MatrixComplex(new Private(impl->dft()));
    }


    MatrixComplex MatrixComplex::idft() const {
        return MatrixComplex(new Private(impl->idft()));
    }


    Real MatrixComplex::conditionNumber() const {
        return impl->conditionNumber();
    }


    Real MatrixComplex::pNorm(Integer p) const {
        return impl->pNorm(p);
    }


    Real MatrixComplex::euclideanNorm() const {
        return impl->euclideanNorm();
    }


    Real MatrixComplex::oneNorm() const {
        return impl->oneNorm();
    }


    Real MatrixComplex::infinityNorm() const {
        return impl->infinityNorm();
    }


    Tuple MatrixComplex::equilibrate() const {
        return Tuple(impl->equilibrate());
    }


    MatrixComplex MatrixComplex::solve(const MatrixComplex& y) const {
        return MatrixComplex(new Private(impl->solve(*y.impl)));
    }


    MatrixComplex MatrixComplex::leastSquares(const MatrixComplex& b) const {
        return MatrixComplex(new Private(impl->leastSquares(*b.impl)));
    }


    MatrixComplex MatrixComplex::hadamard(const MatrixComplex& other) const {
        return MatrixComplex(new Private(impl->hadamard(*other.impl)));
    }


    MatrixComplex MatrixComplex::kronecker(const MatrixComplex& other) const {
        return MatrixComplex(new Private(impl->kronecker(*other.impl)));
    }


    MatrixComplex::MatrixType MatrixComplex::matrixType() const {
        return impl->matrixType();
    }


    const MatrixComplex::Scalar* MatrixComplex::data() const {
        return impl->data();
    }


    MatrixReal MatrixComplex::real() const {
        return MatrixReal(new MatrixReal::Private(impl->real()));
    }


    MatrixReal MatrixComplex::imag() const {
        return MatrixReal(new MatrixReal::Private(impl->imag()));
    }


    MatrixComplex& MatrixComplex::operator=(const MatrixBoolean& other) {
        (*impl) = (*other.impl);
        return *this;
    }


    MatrixComplex& MatrixComplex::operator=(const MatrixInteger& other) {
        (*impl) = (*other.impl);
        return *this;
    }


    MatrixComplex& MatrixComplex::operator=(const MatrixReal& other) {
        (*impl) = (*other.impl);
        return *this;
    }


    MatrixComplex& MatrixComplex::operator=(const MatrixComplex& other) {
        (*impl) = (*other.impl);
        return *this;
    }


    MatrixComplex& MatrixComplex::operator=(const Variant& other) {
        bool          ok;
        MatrixComplex v = other.toMatrixComplex(&ok);

        if (ok) {
            (*impl) = (*v.impl);
        } else {
            throw InvalidRuntimeConversion(other.valueType(), ValueType::MATRIX_COMPLEX);
        }

        return *this;
    }


    MatrixComplex& MatrixComplex::operator+=(const MatrixBoolean& v) {
        (*impl) += (*v.impl);
        return *this;
    }


    MatrixComplex& MatrixComplex::operator+=(const MatrixInteger& v) {
        (*impl) += (*v.impl);
        return *this;
    }


    MatrixComplex& MatrixComplex::operator+=(const MatrixReal& v) {
        (*impl) += (*v.impl);
        return *this;
    }


    MatrixComplex& MatrixComplex::operator+=(const MatrixComplex& v) {
        (*impl) += (*v.impl);
        return *this;
    }


    MatrixComplex& MatrixComplex::operator-=(const MatrixBoolean& v) {
        (*impl) -= (*v.impl);
        return *this;
    }


    MatrixComplex& MatrixComplex::operator-=(const MatrixInteger& v) {
        (*impl) -= (*v.impl);
        return *this;
    }


    MatrixComplex& MatrixComplex::operator-=(const MatrixReal& v) {
        (*impl) -= (*v.impl);
        return *this;
    }


    MatrixComplex& MatrixComplex::operator-=(const MatrixComplex& v) {
        (*impl) -= (*v.impl);
        return *this;
    }


    MatrixComplex& MatrixComplex::operator*=(const MatrixBoolean& v) {
        (*impl) *= (*v.impl);
        return *this;
    }


    MatrixComplex& MatrixComplex::operator*=(const MatrixInteger& v) {
        (*impl) *= (*v.impl);
        return *this;
    }


    MatrixComplex& MatrixComplex::operator*=(const MatrixReal& v) {
        (*impl) *= (*v.impl);
        return *this;
    }


    MatrixComplex& MatrixComplex::operator*=(const MatrixComplex& v) {
        (*impl) *= (*v.impl);
        return *this;
    }


    MatrixComplex& MatrixComplex::operator*=(Boolean v) {
        (*impl) *= v;
        return *this;
    }


    MatrixComplex& MatrixComplex::operator*=(Integer v) {
        (*impl) *= v;
        return *this;
    }


    MatrixComplex& MatrixComplex::operator*=(Real v) {
        (*impl) *= v;
        return *this;
    }


    MatrixComplex& MatrixComplex::operator*=(const Complex& v) {
        (*impl) *= v;
        return *this;
    }


    MatrixComplex& MatrixComplex::operator/=(Integer v) {
        (*impl) /= v;
        return *this;
    }


    MatrixComplex& MatrixComplex::operator/=(Real v) {
        (*impl) /= v;
        return *this;
    }


    MatrixComplex& MatrixComplex::operator/=(const Complex& v) {
        (*impl) /= v;
        return *this;
    }


    MatrixComplex MatrixComplex::operator+(const MatrixBoolean& v) const {
        return MatrixComplex(new Private((*impl) + (*v.impl)));
    }


    MatrixComplex MatrixComplex::operator+(const MatrixInteger& v) const {
        return MatrixComplex(new Private((*impl) + (*v.impl)));
    }


    MatrixComplex MatrixComplex::operator+(const MatrixReal& v) const {
        return MatrixComplex(new Private((*impl) + (*v.impl)));
    }


    MatrixComplex MatrixComplex::operator+(const MatrixComplex& v) const {
        return MatrixComplex(new Private((*impl) + (*v.impl)));
    }


    MatrixComplex MatrixComplex::operator-(const MatrixBoolean& v) const {
        return MatrixComplex(new Private((*impl) - (*v.impl)));
    }


    MatrixComplex MatrixComplex::operator-(const MatrixInteger& v) const {
        return MatrixComplex(new Private((*impl) - (*v.impl)));
    }


    MatrixComplex MatrixComplex::operator-(const MatrixReal& v) const {
        return MatrixComplex(new Private((*impl) - (*v.impl)));
    }


    MatrixComplex MatrixComplex::operator-(const MatrixComplex& v) const {
        return MatrixComplex(new Private((*impl) - (*v.impl)));
    }


    MatrixComplex MatrixComplex::operator*(const MatrixBoolean& v) const {
        return MatrixComplex(new Private((*impl) * (*v.impl)));
    }


    MatrixComplex MatrixComplex::operator*(const MatrixInteger& v) const {
        return MatrixComplex(new Private((*impl) * (*v.impl)));
    }


    MatrixComplex MatrixComplex::operator*(const MatrixReal& v) const {
        return MatrixComplex(new Private((*impl) * (*v.impl)));
    }


    MatrixComplex MatrixComplex::operator*(const MatrixComplex& v) const {
        return MatrixComplex(new Private((*impl) * (*v.impl)));
    }


    MatrixComplex MatrixComplex::operator*(Boolean v) const {
        return MatrixComplex(new Private((*impl) * v));
    }


    MatrixComplex MatrixComplex::operator*(Integer v) const {
        return MatrixComplex(new Private((*impl) * v));
    }


    MatrixComplex MatrixComplex::operator*(Real v) const {
        return MatrixComplex(new Private((*impl) * v));
    }


    MatrixComplex MatrixComplex::operator*(const Complex& v) const {
        return MatrixComplex(new Private((*impl) * v));
    }


    MatrixComplex MatrixComplex::operator/(Integer v) const {
        return MatrixComplex(new Private((*impl) / v));
    }


    MatrixComplex MatrixComplex::operator/(Real v) const {
        return MatrixComplex(new Private((*impl) / v));
    }


    MatrixComplex MatrixComplex::operator/(const Complex& v) const {
        return MatrixComplex(new Private((*impl) / v));
    }


    MatrixComplex MatrixComplex::operator-() const {
        return MatrixComplex(new Private(-(*impl)));
    }


    bool MatrixComplex::operator==(const MatrixBoolean& other) const {
        return (*impl) == (*other.impl);
    }


    bool MatrixComplex::operator==(const MatrixInteger& other) const {
        return (*impl) == (*other.impl);
    }


    bool MatrixComplex::operator==(const MatrixReal& other) const {
        return (*impl) == (*other.impl);
    }


    bool MatrixComplex::operator==(const MatrixComplex& other) const {
        return (*impl) == (*other.impl);
    }


    bool MatrixComplex::operator!=(const MatrixBoolean& other) const {
        return (*impl) != (*other.impl);
    }


    bool MatrixComplex::operator!=(const MatrixInteger& other) const {
        return (*impl) != (*other.impl);
    }


    bool MatrixComplex::operator!=(const MatrixReal& other) const {
        return (*impl) != (*other.impl);
    }


    bool MatrixComplex::operator!=(const MatrixComplex& other) const {
        return (*impl) != (*other.impl);
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


    MODEL_PUBLIC_API MatrixComplex operator+(const MatrixBoolean& a, const MatrixComplex& b) {
        return MatrixComplex(a) + b;
    }


    MODEL_PUBLIC_API MatrixComplex operator+(const MatrixInteger& a, const MatrixComplex& b) {
        return MatrixComplex(a) + b;
    }


    MODEL_PUBLIC_API MatrixComplex operator+(const MatrixReal& a, const MatrixComplex& b) {
        return MatrixComplex(a) + b;
    }


    MODEL_PUBLIC_API MatrixComplex operator-(const MatrixBoolean& a, const MatrixComplex& b) {
        return MatrixComplex(a) - b;
    }


    MODEL_PUBLIC_API MatrixComplex operator-(const MatrixInteger& a, const MatrixComplex& b) {
        return MatrixComplex(a) - b;
    }


    MODEL_PUBLIC_API MatrixComplex operator-(const MatrixReal& a, const MatrixComplex& b) {
        return MatrixComplex(a) - b;
    }
}
