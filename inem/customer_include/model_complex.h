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
* This header defines the \ref Model::Complex type.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_COMPLEX_H
#define MODEL_COMPLEX_H

#include "model_common.h"
#include "model_intrinsic_types.h"

namespace M {
    class MODEL_PUBLIC_API Variant;
}

namespace Model {
    class MODEL_PUBLIC_API Variant;

    /**
     * Complex data-type class.  Class is designed to be memory footprint compatible with the C++
     * std::complex<Model::Real> class as well as the C99 _Complex Model::Real type.
     */
    class MODEL_PUBLIC_API Complex {
        public:
            /**
             * Fundamental datatype.
             */
            typedef Real T;

        private:
            friend MODEL_PUBLIC_API Complex operator+(Complex::T a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator+(Boolean a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator+(Integer a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator+(unsigned char a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator+(signed char a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator+(unsigned short a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator+(signed short a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator+(unsigned a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator+(signed a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator+(unsigned long a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator+(signed long a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator+(unsigned long long a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator-(Complex::T a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator-(Boolean a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator-(Integer a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator-(unsigned char a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator-(signed char a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator-(unsigned short a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator-(signed short a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator-(unsigned a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator-(signed a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator-(unsigned long a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator-(signed long a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator-(unsigned long long a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator*(Complex::T a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator*(Boolean a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator*(Integer a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator*(unsigned char a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator*(signed char a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator*(unsigned short a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator*(signed short a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator*(unsigned a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator*(signed a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator*(unsigned long a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator*(signed long a, const Complex& b);
            friend MODEL_PUBLIC_API Complex operator*(unsigned long long a, const Complex& b);
            friend MODEL_PUBLIC_API bool operator==(Complex::T a, const Complex& b);
            friend MODEL_PUBLIC_API bool operator!=(Complex::T a, const Complex& b);

            /**
             * Real value.
             */
            T r;

            /**
             * Imaginary value.
             */
            T i;

        public:
            constexpr inline Complex():r(0), i(0) {}

            /**
             * Constructor
             *
             * \param[in] real The real portion of the value.
             */
            constexpr inline Complex(float real):r(real), i(0) {}

            /**
             * Constructor
             *
             * \param[in] real The real portion of the value.
             */
            constexpr inline Complex(double real):r(real), i(0) {}

            /**
             * Constructor
             *
             * \param[in] real The real portion of the value.
             */
            constexpr inline Complex(long double real):r(real), i(0) {}

            /**
             * Constructor
             *
             * \param[in] real The real portion of the value.
             */
            constexpr inline Complex(signed char real):r(real), i(0) {}

            /**
             * Constructor
             *
             * \param[in] real The real portion of the value.
             */
            constexpr inline Complex(short real):r(real), i(0) {}

            /**
             * Constructor
             *
             * \param[in] real The real portion of the value.
             */
            constexpr inline Complex(int real):r(real), i(0) {}

            /**
             * Constructor
             *
             * \param[in] real The real portion of the value.
             */
            constexpr inline Complex(long real):r(real), i(0) {}

            /**
             * Constructor
             *
             * \param[in] real The real portion of the value.
             */
            constexpr inline Complex(long long real):r(static_cast<T>(real)), i(0) {}

            /**
             * Constructor
             *
             * \param[in] real The real portion of the value.
             */
            constexpr inline Complex(unsigned char real):r(real), i(0) {}

            /**
             * Constructor
             *
             * \param[in] real The real portion of the value.
             */
            constexpr inline Complex(unsigned short real):r(real), i(0) {}

            /**
             * Constructor
             *
             * \param[in] real The real portion of the value.
             */
            constexpr inline Complex(unsigned real):r(real), i(0) {}

            /**
             * Constructor
             *
             * \param[in] real The real portion of the value.
             */
            constexpr inline Complex(unsigned long real):r(real), i(0) {}

            /**
             * Constructor
             *
             * \param[in] real The real portion of the value.
             */
            constexpr inline Complex(unsigned long long real):r(static_cast<T>(real)), i(0) {}

            /**
             * Constructor
             *
             * \param[in] other The value to conver to a complex type.
             */
            Complex(const M::Variant& other);

            /**
             * Constructor
             *
             * \param[in] other The value to conver to a complex type.
             */
            Complex(const Variant& other);

            /**
             * Copy constructor
             *
             * \param[in] other The value to be copied.
             */
            constexpr inline Complex(const Complex& other):r(other.r), i(other.i) {}

            /**
             * Templated constructor
             *
             * \param[in] p1 The first parameter.
             *
             * \param[in] p2 The second parameter.
             */
            template<typename P1, typename P2> constexpr inline Complex(P1 p1, P2 p2):r(p1), i(p2) {}

            ~Complex() = default;

            /**
             * Method that returns the real portion of the value.
             *
             * \return Returns the real portion of the complex value.
             */
            inline T real() const {
                return r;
            }

            /**
             * Method that returns the imaginary portion of the value.
             *
             * \return Returns the imaginary portion of the complex value.
             */
            inline T imag() const {
                return i;
            }

            /**
             * Method that returns the complex conjugate of this value.
             *
             * \return Returns the complex conjugate of this value.
             */
            inline Complex conj() const {
                return Complex(r,-i);
            }

            /**
             * Assignment operator
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            inline Complex& operator=(const Complex& other) {
                r = other.r;
                i = other.i;
                return *this;
            }

            /**
             * Assignment operator
             *
             * \param[in] other The value to be copied.
             *
             * \return Returns a reference to this instance.
             */
            inline Complex& operator=(float other) {
                r = other;
                i = 0;
                return *this;
            }

            /**
             * Assignment operator
             *
             * \param[in] other The value to be copied.
             *
             * \return Returns a reference to this instance.
             */
            inline Complex& operator=(double other) {
                r = other;
                i = 0;
                return *this;
            }

            /**
             * Assignment operator
             *
             * \param[in] other The value to be copied.
             *
             * \return Returns a reference to this instance.
             */
            inline Complex& operator=(long double other) {
                r = other;
                i = 0;
                return *this;
            }

            /**
             * Assignment operator
             *
             * \param[in] other The value to be copied.
             *
             * \return Returns a reference to this instance.
             */
            inline Complex& operator=(unsigned char other) {
                r = other;
                i = 0;
                return *this;
            }

            /**
             * Assignment operator
             *
             * \param[in] other The value to be copied.
             *
             * \return Returns a reference to this instance.
             */
            inline Complex& operator=(signed char other) {
                r = other;
                i = 0;
                return *this;
            }

            /**
             * Assignment operator
             *
             * \param[in] other The value to be copied.
             *
             * \return Returns a reference to this instance.
             */
            inline Complex& operator=(unsigned short other) {
                r = other;
                i = 0;
                return *this;
            }

            /**
             * Assignment operator
             *
             * \param[in] other The value to be copied.
             *
             * \return Returns a reference to this instance.
             */
            inline Complex& operator=(signed short other) {
                r = other;
                i = 0;
                return *this;
            }

            /**
             * Assignment operator
             *
             * \param[in] other The value to be copied.
             *
             * \return Returns a reference to this instance.
             */
            inline Complex& operator=(unsigned int other) {
                r = other;
                i = 0;
                return *this;
            }

            /**
             * Assignment operator
             *
             * \param[in] other The value to be copied.
             *
             * \return Returns a reference to this instance.
             */
            inline Complex& operator=(signed int other) {
                r = other;
                i = 0;
                return *this;
            }

            /**
             * Assignment operator
             *
             * \param[in] other The value to be copied.
             *
             * \return Returns a reference to this instance.
             */
            inline Complex& operator=(unsigned long other) {
                r = other;
                i = 0;
                return *this;
            }

            /**
             * Assignment operator
             *
             * \param[in] other The value to be copied.
             *
             * \return Returns a reference to this instance.
             */
            inline Complex& operator=(signed long other) {
                r = other;
                i = 0;
                return *this;
            }

            /**
             * Assignment operator
             *
             * \param[in] other The value to be copied.
             *
             * \return Returns a reference to this instance.
             */
            inline Complex& operator=(unsigned long long other) {
                r = static_cast<T>(other);
                i = 0;
                return *this;
            }

            /**
             * Assignment operator
             *
             * \param[in] other The value to be copied.
             *
             * \return Returns a reference to this instance.
             */
            inline Complex& operator=(signed long long other) {
                r = static_cast<T>(other);
                i = 0;
                return *this;
            }

            /**
             * Assignment operator
             *
             * \param[in] other The value to be copied.
             *
             * \return Returns a reference to this instance.
             */
            Complex& operator=(const Variant& other);

            /**
             * Assignment operator
             *
             * \param[in] other The value to be copied.
             *
             * \return Returns a reference to this instance.
             */
            Complex& operator=(const M::Variant& other);

            /**
             * Addition assignment operator
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a reference to this instance.
             */
            inline Complex& operator+=(const Complex& v) {
                r += v.r;
                i += v.i;
                return *this;
            }

            /**
             * Addition assignment operator
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a reference to this instance.
             */
            inline Complex& operator+=(T v) {
                r += v;
                return *this;
            }

            /**
             * Subtraction assignment operator
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a reference to this instance.
             */
            inline Complex& operator-=(const Complex& v) {
                r -= v.r;
                i -= v.i;
                return *this;
            }

            /**
             * Subtraction assignment operator
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a reference to this instance.
             */
            inline Complex& operator-=(T v) {
                r -= v;
                return *this;
            }

            /**
             * Multiplication assignment operator
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a reference to this instance.
             */
            Complex& operator*=(const Complex& v);

            /**
             * Multiplication assignment operator
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a reference to this instance.
             */
            inline Complex& operator*=(T v) {
                r *= v;
                i *= v;
                return *this;
            }

            /**
             * Division assignment operator
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a reference to this instance.
             */
            inline Complex& operator/=(const Complex& v) {
                *this = *this / v;
                return *this;
            }

            /**
             * Division assignment operator
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a reference to this instance.
             */
            inline Complex& operator/=(T v) {
                r /= v;
                i /= v;
                return *this;
            }

            /**
             * Addition operator.
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a complex value representing the sum.
             */
            inline Complex operator+(const Complex& v) const {
                return Complex(r + v.r, i + v.i);
            }

            /**
             * Addition operator.
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a complex value representing the sum.
             */
            inline Complex operator+(T v) const {
                return Complex(r + v, i);
            }

            /**
             * Addition operator.
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a complex value representing the sum.
             */
            inline Complex operator+(Boolean v) const {
                return Complex(r + (v ? 1 : 0), i);
            }

            /**
             * Addition operator.
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a complex value representing the sum.
             */
            inline Complex operator+(Integer v) const {
                return Complex(r + v, i);
            }

            /**
             * Addition operator.
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a complex value representing the sum.
             */
            inline Complex operator+(unsigned char v) const {
                return Complex(r + v, i);
            }

            /**
             * Addition operator.
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a complex value representing the sum.
             */
            inline Complex operator+(signed char v) const {
                return Complex(r + v, i);
            }

            /**
             * Addition operator.
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a complex value representing the sum.
             */
            inline Complex operator+(unsigned short v) const {
                return Complex(r + v, i);
            }

            /**
             * Addition operator.
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a complex value representing the sum.
             */
            inline Complex operator+(signed short v) const {
                return Complex(r + v, i);
            }

            /**
             * Addition operator.
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a complex value representing the sum.
             */
            inline Complex operator+(unsigned v) const {
                return Complex(r + v, i);
            }

            /**
             * Addition operator.
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a complex value representing the sum.
             */
            inline Complex operator+(signed v) const {
                return Complex(r + v, i);
            }

            /**
             * Addition operator.
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a complex value representing the sum.
             */
            inline Complex operator+(unsigned long v) const {
                return Complex(r + v, i);
            }

            /**
             * Addition operator.
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a complex value representing the sum.
             */
            inline Complex operator+(signed long v) const {
                return Complex(r + v, i);
            }

            /**
             * Addition operator.
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a complex value representing the sum.
             */
            inline Complex operator+(unsigned long long v) const {
                return Complex(r + v, i);
            }

            /**
             * Subtraction operator.
             *
             * \param[in] v The value to subtract from this value.
             *
             * \return Returns a complex value representing the difference.
             */
            inline Complex operator-(const Complex& v) const {
                return Complex(r - v.r, i - v.i);
            }

            /**
             * Subtraction operator.
             *
             * \param[in] v The value to subtract from this value.
             *
             * \return Returns a complex value representing the difference.
             */
            inline Complex operator-(T v) const {
                return Complex(r - v, i);
            }

            /**
             * Subtraction operator.
             *
             * \param[in] v The value to subtract from this value.
             *
             * \return Returns a complex value representing the difference.
             */
            inline Complex operator-(Boolean v) const {
                return Complex(r - (v ? 1 : 0), i);
            }

            /**
             * Subtraction operator.
             *
             * \param[in] v The value to subtract from this value.
             *
             * \return Returns a complex value representing the difference.
             */
            inline Complex operator-(Integer v) const {
                return Complex(r - v, i);
            }

            /**
             * Subtraction operator.
             *
             * \param[in] v The value to subtract from this value.
             *
             * \return Returns a complex value representing the difference.
             */
            inline Complex operator-(unsigned char v) const {
                return Complex(r - v, i);
            }

            /**
             * Subtraction operator.
             *
             * \param[in] v The value to subtract from this value.
             *
             * \return Returns a complex value representing the difference.
             */
            inline Complex operator-(signed char v) const {
                return Complex(r - v, i);
            }

            /**
             * Subtraction operator.
             *
             * \param[in] v The value to subtract from this value.
             *
             * \return Returns a complex value representing the difference.
             */
            inline Complex operator-(unsigned short v) const {
                return Complex(r - v, i);
            }

            /**
             * Subtraction operator.
             *
             * \param[in] v The value to subtract from this value.
             *
             * \return Returns a complex value representing the difference.
             */
            inline Complex operator-(signed short v) const {
                return Complex(r - v, i);
            }

            /**
             * Subtraction operator.
             *
             * \param[in] v The value to subtract from this value.
             *
             * \return Returns a complex value representing the difference.
             */
            inline Complex operator-(unsigned v) const {
                return Complex(r - v, i);
            }

            /**
             * Subtraction operator.
             *
             * \param[in] v The value to subtract from this value.
             *
             * \return Returns a complex value representing the difference.
             */
            inline Complex operator-(signed v) const {
                return Complex(r - v, i);
            }

            /**
             * Subtraction operator.
             *
             * \param[in] v The value to subtract from this value.
             *
             * \return Returns a complex value representing the difference.
             */
            inline Complex operator-(unsigned long v) const {
                return Complex(r - v, i);
            }

            /**
             * Subtraction operator.
             *
             * \param[in] v The value to subtract from this value.
             *
             * \return Returns a complex value representing the difference.
             */
            inline Complex operator-(signed long v) const {
                return Complex(r - v, i);
            }

            /**
             * Subtraction operator.
             *
             * \param[in] v The value to subtract from this value.
             *
             * \return Returns a complex value representing the difference.
             */
            inline Complex operator-(unsigned long long v) const {
                return Complex(r - v, i);
            }

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply against to this value.
             *
             * \return Returns a complex value representing the product.
             */
            Complex operator*(const Complex& v) const;

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply against this value.
             *
             * \return Returns a complex value representing the product.
             */
            inline Complex operator*(T v) const {
                return Complex(r*v, i*v);
            }

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply against this value.
             *
             * \return Returns a complex value representing the product.
             */
            inline Complex operator*(Boolean v) const {
                T vr = v ? 1 : 0;
                return Complex(r*vr, i*vr);
            }

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply against this value.
             *
             * \return Returns a complex value representing the product.
             */
            inline Complex operator*(Integer v) const {
                return Complex(r*v, i*v);
            }

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply against this value.
             *
             * \return Returns a complex value representing the product.
             */
            inline Complex operator*(unsigned char v) const {
                return Complex(r*v, i*v);
            }

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply against this value.
             *
             * \return Returns a complex value representing the product.
             */
            inline Complex operator*(signed char v) const {
                return Complex(r*v, i*v);
            }

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply against this value.
             *
             * \return Returns a complex value representing the product.
             */
            inline Complex operator*(unsigned short v) const {
                return Complex(r*v, i*v);
            }

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply against this value.
             *
             * \return Returns a complex value representing the product.
             */
            inline Complex operator*(signed short v) const {
                return Complex(r*v, i*v);
            }

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply against this value.
             *
             * \return Returns a complex value representing the product.
             */
            inline Complex operator*(unsigned v) const {
                return Complex(r*v, i*v);
            }

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply against this value.
             *
             * \return Returns a complex value representing the product.
             */
            inline Complex operator*(signed v) const {
                return Complex(r*v, i*v);
            }

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply against this value.
             *
             * \return Returns a complex value representing the product.
             */
            inline Complex operator*(unsigned long v) const {
                return Complex(r*v, i*v);
            }

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply against this value.
             *
             * \return Returns a complex value representing the product.
             */
            inline Complex operator*(signed long v) const {
                return Complex(r*v, i*v);
            }

            /**
             * Multiplication operator.
             *
             * \param[in] v The value to multiply against this value.
             *
             * \return Returns a complex value representing the product.
             */
            inline Complex operator*(unsigned long long v) const {
                return Complex(r*v, i*v);
            }

            /**
             * Division operator.
             *
             * \param[in] v The value to divide from this value.
             *
             * \return Returns a complex value representing the quotient.
             */
            Complex operator/(const Complex& v) const;

            /**
             * Division operator.
             *
             * \param[in] v The value to add to this value.
             *
             * \return Returns a complex value representing the quotient.
             */
            inline Complex operator/(T v) const {
                return Complex(r/v, i/v);
            }

            /**
             * Division operator.
             *
             * \param[in] v The value to divide from this value.
             *
             * \return Returns a complex value representing the quotient.
             */
            inline Complex operator/(Boolean v) const {
                T vr = v ? 1 : 0;
                return Complex(r/vr, i/vr);
            }

            /**
             * Division operator.
             *
             * \param[in] v The value to divide from this value.
             *
             * \return Returns a complex value representing the quotient.
             */
            inline Complex operator/(Integer v) const {
                return Complex(r/v, i/v);
            }

            /**
             * Division operator.
             *
             * \param[in] v The value to divide from this value.
             *
             * \return Returns a complex value representing the quotient.
             */
            inline Complex operator/(unsigned char v) const {
                return Complex(r/v, i/v);
            }

            /**
             * Division operator.
             *
             * \param[in] v The value to divide from this value.
             *
             * \return Returns a complex value representing the quotient.
             */
            inline Complex operator/(signed char v) const {
                return Complex(r/v, i/v);
            }

            /**
             * Division operator.
             *
             * \param[in] v The value to divide from this value.
             *
             * \return Returns a complex value representing the quotient.
             */
            inline Complex operator/(unsigned short v) const {
                return Complex(r/v, i/v);
            }

            /**
             * Division operator.
             *
             * \param[in] v The value to divide from this value.
             *
             * \return Returns a complex value representing the quotient.
             */
            inline Complex operator/(signed short v) const {
                return Complex(r/v, i/v);
            }

            /**
             * Division operator.
             *
             * \param[in] v The value to divide from this value.
             *
             * \return Returns a complex value representing the quotient.
             */
            inline Complex operator/(unsigned v) const {
                return Complex(r/v, i/v);
            }

            /**
             * Division operator.
             *
             * \param[in] v The value to divide from this value.
             *
             * \return Returns a complex value representing the quotient.
             */
            inline Complex operator/(signed v) const {
                return Complex(r/v, i/v);
            }

            /**
             * Division operator.
             *
             * \param[in] v The value to divide from this value.
             *
             * \return Returns a complex value representing the quotient.
             */
            inline Complex operator/(unsigned long v) const {
                return Complex(r/v, i/v);
            }

            /**
             * Division operator.
             *
             * \param[in] v The value to divide from this value.
             *
             * \return Returns a complex value representing the quotient.
             */
            inline Complex operator/(signed long v) const {
                return Complex(r/v, i/v);
            }

            /**
             * Division operator.
             *
             * \param[in] v The value to divide from this value.
             *
             * \return Returns a complex value representing the quotient.
             */
            inline Complex operator/(unsigned long long v) const {
                return Complex(r/v, i/v);
            }

            /**
             * Unary plus operator.
             *
             * \return Returns a constant reference to this instance.
             */
            inline const Complex& operator+() const {
                return *this;
            }

            /**
             * Unary minus operator
             *
             * \return Returns a new complex instance representing the negative of this instance.
             */
            inline Complex operator-() const {
                return Complex(-r, -i);
            }

            /**
             * Pre-increment operator
             *
             * \return Returns a reference to this instance.
             */
            inline Complex& operator++() {
                ++r;
                return *this;
            }

            /**
             * Post-increment operator
             *
             * \return Returns an unmodified copy of this instance.
             */
            inline Complex operator++(int) {
                Complex result = *this;
                ++r;

                return result;
            }

            /**
             * Pre-decrement operator
             *
             * \return Returns a reference to this instance.
             */
            inline Complex& operator--() {
                --r;
                return *this;
            }

            /**
             * Post-decrement operator
             *
             * \return Returns an unmodified copy of this instance.
             */
            inline Complex operator--(int) {
                Complex result = *this;
                --r;

                return result;
            }

            /**
             * Casting operator.
             *
             * \return Returns true if the complex value is non-zero.  Returns false if the complex value is zero.
             */
            explicit operator Model::Boolean() const;

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are equal.  Returns false if the values are not equal.
             */
            inline bool operator==(const Complex& other) const {
                return other.r == r && other.i == i;
            }

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are equal.  Returns false if the values are not equal.
             */
            inline bool operator==(T other) const {
                return other == r && i == 0;
            }

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are equal.  Returns false if the values are not equal.
             */
            inline bool operator==(Boolean other) const {
                return (other ? 1 : 0) == r && i == 0;
            }

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are equal.  Returns false if the values are not equal.
             */
            inline bool operator==(Integer other) const {
                return other == r && i == 0;
            }

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are equal.  Returns false if the values are not equal.
             */
            inline bool operator==(unsigned char other) const {
                return other == r && i == 0;
            }

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are equal.  Returns false if the values are not equal.
             */
            inline bool operator==(signed char other) const {
                return other == r && i == 0;
            }

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are equal.  Returns false if the values are not equal.
             */
            inline bool operator==(unsigned short other) const {
                return other == r && i == 0;
            }

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are equal.  Returns false if the values are not equal.
             */
            inline bool operator==(signed short other) const {
                return other == r && i == 0;
            }

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are equal.  Returns false if the values are not equal.
             */
            inline bool operator==(unsigned other) const {
                return other == r && i == 0;
            }

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are equal.  Returns false if the values are not equal.
             */
            inline bool operator==(signed other) const {
                return other == r && i == 0;
            }

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are equal.  Returns false if the values are not equal.
             */
            inline bool operator==(unsigned long other) const {
                return other == r && i == 0;
            }

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are equal.  Returns false if the values are not equal.
             */
            inline bool operator==(signed long other) const {
                return other == r && i == 0;
            }

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are equal.  Returns false if the values are not equal.
             */
            inline bool operator==(unsigned long long other) const {
                return other == r && i == 0;
            }

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are not equal.  Returns false if the values are equal.
             */
            inline bool operator!=(const Complex& other) const {
                return other.r != r || other.i != i;
            }

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are not equal.  Returns false if the values are equal.
             */
            inline bool operator!=(T other) const {
                return other != r || i != 0;
            }

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are not equal.  Returns false if the values are equal.
             */
            inline bool operator!=(Boolean other) const {
                return (other ? 1 : 0) != r || i != 0;
            }

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are not equal.  Returns false if the values are equal.
             */
            inline bool operator!=(Integer other) const {
                return other != r || i != 0;
            }

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are not equal.  Returns false if the values are equal.
             */
            inline bool operator!=(unsigned char other) const {
                return other != r || i != 0;
            }

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are not equal.  Returns false if the values are equal.
             */
            inline bool operator!=(signed char other) const {
                return other != r || i != 0;
            }

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are not equal.  Returns false if the values are equal.
             */
            inline bool operator!=(unsigned short other) const {
                return other != r || i != 0;
            }

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are not equal.  Returns false if the values are equal.
             */
            inline bool operator!=(signed short other) const {
                return other != r || i != 0;
            }

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are not equal.  Returns false if the values are equal.
             */
            inline bool operator!=(unsigned other) const {
                return other != r || i != 0;
            }

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are not equal.  Returns false if the values are equal.
             */
            inline bool operator!=(signed other) const {
                return other != r || i != 0;
            }

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are not equal.  Returns false if the values are equal.
             */
            inline bool operator!=(unsigned long other) const {
                return other != r || i != 0;
            }

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are not equal.  Returns false if the values are equal.
             */
            inline bool operator!=(signed long other) const {
                return other != r || i != 0;
            }

            /**
             * Comparison operator
             *
             * \param[in] other The value to compare against this value.
             *
             * \return Returns true if the values are not equal.  Returns false if the values are equal.
             */
            inline bool operator!=(unsigned long long other) const {
                return other != r || i != 0;
            }
    };

    /**
     * Addition operator.
     *
     * \param[in] a The first value to add.
     *
     * \param[in] b The second value to add.
     *
     * \return Returns a complex value representing the sum.
     */
    MODEL_PUBLIC_API inline Complex operator+(Complex::T a, const Complex& b) {
        return Complex(a + b.r, b.i);
    }

    /**
     * Addition operator.
     *
     * \param[in] a The first value to add.
     *
     * \param[in] b The second value to add.
     *
     * \return Returns a complex value representing the sum.
     */
    MODEL_PUBLIC_API inline Complex operator+(Boolean a, const Complex& b) {
        Complex::T ar = a ? 1 : 0;
        return Complex(ar + b.r, b.i);
    }

    /**
     * Addition operator.
     *
     * \param[in] a The first value to add.
     *
     * \param[in] b The second value to add.
     *
     * \return Returns a complex value representing the sum.
     */
    MODEL_PUBLIC_API inline Complex operator+(Integer a, const Complex& b) {
        return Complex(a + b.r, b.i);
    }

    /**
     * Addition operator.
     *
     * \param[in] a The first value to add.
     *
     * \param[in] b The second value to add.
     *
     * \return Returns a complex value representing the sum.
     */
    MODEL_PUBLIC_API inline Complex operator+(unsigned char a, const Complex& b) {
        return Complex(a + b.r, b.i);
    }

    /**
     * Addition operator.
     *
     * \param[in] a The first value to add.
     *
     * \param[in] b The second value to add.
     *
     * \return Returns a complex value representing the sum.
     */
    MODEL_PUBLIC_API inline Complex operator+(signed char a, const Complex& b) {
        return Complex(a + b.r, b.i);
    }

    /**
     * Addition operator.
     *
     * \param[in] a The first value to add.
     *
     * \param[in] b The second value to add.
     *
     * \return Returns a complex value representing the sum.
     */
    MODEL_PUBLIC_API inline Complex operator+(unsigned short a, const Complex& b) {
        return Complex(a + b.r, b.i);
    }

    /**
     * Addition operator.
     *
     * \param[in] a The first value to add.
     *
     * \param[in] b The second value to add.
     *
     * \return Returns a complex value representing the sum.
     */
    MODEL_PUBLIC_API inline Complex operator+(signed short a, const Complex& b) {
        return Complex(a + b.r, b.i);
    }

    /**
     * Addition operator.
     *
     * \param[in] a The first value to add.
     *
     * \param[in] b The second value to add.
     *
     * \return Returns a complex value representing the sum.
     */
    MODEL_PUBLIC_API inline Complex operator+(unsigned a, const Complex& b) {
        return Complex(a + b.r, b.i);
    }

    /**
     * Addition operator.
     *
     * \param[in] a The first value to add.
     *
     * \param[in] b The second value to add.
     *
     * \return Returns a complex value representing the sum.
     */
    MODEL_PUBLIC_API inline Complex operator+(signed a, const Complex& b) {
        return Complex(a + b.r, b.i);
    }

    /**
     * Addition operator.
     *
     * \param[in] a The first value to add.
     *
     * \param[in] b The second value to add.
     *
     * \return Returns a complex value representing the sum.
     */
    MODEL_PUBLIC_API inline Complex operator+(unsigned long a, const Complex& b) {
        return Complex(a + b.r, b.i);
    }

    /**
     * Addition operator.
     *
     * \param[in] a The first value to add.
     *
     * \param[in] b The second value to add.
     *
     * \return Returns a complex value representing the sum.
     */
    MODEL_PUBLIC_API inline Complex operator+(signed long a, const Complex& b) {
        return Complex(a + b.r, b.i);
    }

    /**
     * Addition operator.
     *
     * \param[in] a The first value to add.
     *
     * \param[in] b The second value to add.
     *
     * \return Returns a complex value representing the sum.
     */
    MODEL_PUBLIC_API inline Complex operator+(unsigned long long a, const Complex& b) {
        return Complex(a + b.r, b.i);
    }

    /**
     * Subtraction operator.
     *
     * \param[in] a The first value to subtract.
     *
     * \param[in] b The second value to subtract.
     *
     * \return Returns a complex value representing the difference.
     */
    MODEL_PUBLIC_API inline Complex operator-(Complex::T a, const Complex& b) {
        return Complex(a - b.r, -b.i);
    }

    /**
     * Subtraction operator.
     *
     * \param[in] a The first value to subtract.
     *
     * \param[in] b The second value to subtract.
     *
     * \return Returns a complex value representing the difference.
     */
    MODEL_PUBLIC_API inline Complex operator-(Boolean a, const Complex& b) {
        Complex::T ar = a ? 1 : 0;
        return Complex(ar - b.r, -b.i);
    }

    /**
     * Subtraction operator.
     *
     * \param[in] a The first value to subtract.
     *
     * \param[in] b The second value to subtract.
     *
     * \return Returns a complex value representing the difference.
     */
    MODEL_PUBLIC_API inline Complex operator-(Integer a, const Complex& b) {
        return Complex(a - b.r, -b.i);
    }

    /**
     * Subtraction operator.
     *
     * \param[in] a The first value to subtract.
     *
     * \param[in] b The second value to subtract.
     *
     * \return Returns a complex value representing the difference.
     */
    MODEL_PUBLIC_API inline Complex operator-(unsigned char a, const Complex& b) {
        return Complex(a - b.r, -b.i);
    }

    /**
     * Subtraction operator.
     *
     * \param[in] a The first value to subtract.
     *
     * \param[in] b The second value to subtract.
     *
     * \return Returns a complex value representing the difference.
     */
    MODEL_PUBLIC_API inline Complex operator-(signed char a, const Complex& b) {
        return Complex(a - b.r, -b.i);
    }

    /**
     * Subtraction operator.
     *
     * \param[in] a The first value to subtract.
     *
     * \param[in] b The second value to subtract.
     *
     * \return Returns a complex value representing the difference.
     */
    MODEL_PUBLIC_API inline Complex operator-(unsigned short a, const Complex& b) {
        return Complex(a - b.r, -b.i);
    }

    /**
     * Subtraction operator.
     *
     * \param[in] a The first value to subtract.
     *
     * \param[in] b The second value to subtract.
     *
     * \return Returns a complex value representing the difference.
     */
    MODEL_PUBLIC_API inline Complex operator-(signed short a, const Complex& b) {
        return Complex(a - b.r, -b.i);
    }

    /**
     * Subtraction operator.
     *
     * \param[in] a The first value to subtract.
     *
     * \param[in] b The second value to subtract.
     *
     * \return Returns a complex value representing the difference.
     */
    MODEL_PUBLIC_API inline Complex operator-(unsigned a, const Complex& b) {
        return Complex(a - b.r, -b.i);
    }

    /**
     * Subtraction operator.
     *
     * \param[in] a The first value to subtract.
     *
     * \param[in] b The second value to subtract.
     *
     * \return Returns a complex value representing the difference.
     */
    MODEL_PUBLIC_API inline Complex operator-(signed a, const Complex& b) {
        return Complex(a - b.r, -b.i);
    }

    /**
     * Subtraction operator.
     *
     * \param[in] a The first value to subtract.
     *
     * \param[in] b The second value to subtract.
     *
     * \return Returns a complex value representing the difference.
     */
    MODEL_PUBLIC_API inline Complex operator-(unsigned long a, const Complex& b) {
        return Complex(a - b.r, -b.i);
    }

    /**
     * Subtraction operator.
     *
     * \param[in] a The first value to subtract.
     *
     * \param[in] b The second value to subtract.
     *
     * \return Returns a complex value representing the difference.
     */
    MODEL_PUBLIC_API inline Complex operator-(signed long a, const Complex& b) {
        return Complex(a - b.r, -b.i);
    }

    /**
     * Subtraction operator.
     *
     * \param[in] a The first value to subtract.
     *
     * \param[in] b The second value to subtract.
     *
     * \return Returns a complex value representing the difference.
     */
    MODEL_PUBLIC_API inline Complex operator-(unsigned long long a, const Complex& b) {
        return Complex(a - b.r, -b.i);
    }

    /**
     * Multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns a complex value representing the product.
     */
    MODEL_PUBLIC_API inline Complex operator*(Complex::T a, const Complex& b) {
        return Complex(a * b.r, a * b.i);
    }

    /**
     * Multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns a complex value representing the product.
     */
    MODEL_PUBLIC_API inline Complex operator*(Boolean a, const Complex& b) {
        return a ? b : Complex(0);
    }

    /**
     * Multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns a complex value representing the product.
     */
    MODEL_PUBLIC_API inline Complex operator*(Integer a, const Complex& b) {
        return Complex(a * b.r, a * b.i);
    }

    /**
     * Multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns a complex value representing the product.
     */
    MODEL_PUBLIC_API inline Complex operator*(unsigned char a, const Complex& b) {
        return Complex(a * b.r, a * b.i);
    }

    /**
     * Multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns a complex value representing the product.
     */
    MODEL_PUBLIC_API inline Complex operator*(signed char a, const Complex& b) {
        return Complex(a * b.r, a * b.i);
    }

    /**
     * Multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns a complex value representing the product.
     */
    MODEL_PUBLIC_API inline Complex operator*(unsigned short a, const Complex& b) {
        return Complex(a * b.r, a * b.i);
    }

    /**
     * Multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns a complex value representing the product.
     */
    MODEL_PUBLIC_API inline Complex operator*(signed short a, const Complex& b) {
        return Complex(a * b.r, a * b.i);
    }

    /**
     * Multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns a complex value representing the product.
     */
    MODEL_PUBLIC_API inline Complex operator*(unsigned a, const Complex& b) {
        return Complex(a * b.r, a * b.i);
    }

    /**
     * Multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns a complex value representing the product.
     */
    MODEL_PUBLIC_API inline Complex operator*(signed a, const Complex& b) {
        return Complex(a * b.r, a * b.i);
    }

    /**
     * Multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns a complex value representing the product.
     */
    MODEL_PUBLIC_API inline Complex operator*(unsigned long a, const Complex& b) {
        return Complex(a * b.r, a * b.i);
    }

    /**
     * Multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns a complex value representing the product.
     */
    MODEL_PUBLIC_API inline Complex operator*(signed long a, const Complex& b) {
        return Complex(a * b.r, a * b.i);
    }

    /**
     * Multiplication operator.
     *
     * \param[in] a The first value to multiply.
     *
     * \param[in] b The second value to multiply.
     *
     * \return Returns a complex value representing the product.
     */
    MODEL_PUBLIC_API inline Complex operator*(unsigned long long a, const Complex& b) {
        return Complex(a * b.r, a * b.i);
    }

    /**
     * Division operator.
     *
     * \param[in] a The first value to divide.
     *
     * \param[in] b The second value to divide.
     *
     * \return Returns a complex value representing the quotient.
     */
    MODEL_PUBLIC_API inline Complex operator/(Complex::T a, const Complex& b) {
        return Complex(a) / b;
    }

    /**
     * Division operator.
     *
     * \param[in] a The first value to divide.
     *
     * \param[in] b The second value to divide.
     *
     * \return Returns a complex value representing the quotient.
     */
    MODEL_PUBLIC_API inline Complex operator/(Boolean a, const Complex& b) {
        return a ? Complex(1) / b : Complex(0);
    }

    /**
     * Division operator.
     *
     * \param[in] a The first value to divide.
     *
     * \param[in] b The second value to divide.
     *
     * \return Returns a complex value representing the quotient.
     */
    MODEL_PUBLIC_API inline Complex operator/(Integer a, const Complex& b) {
        return Complex(a) / b;
    }

    /**
     * Division operator.
     *
     * \param[in] a The first value to divide.
     *
     * \param[in] b The second value to divide.
     *
     * \return Returns a complex value representing the quotient.
     */
    MODEL_PUBLIC_API inline Complex operator/(unsigned char a, const Complex& b) {
        return Complex(a) / b;
    }

    /**
     * Division operator.
     *
     * \param[in] a The first value to divide.
     *
     * \param[in] b The second value to divide.
     *
     * \return Returns a complex value representing the quotient.
     */
    MODEL_PUBLIC_API inline Complex operator/(signed char a, const Complex& b) {
        return Complex(a) / b;
    }

    /**
     * Division operator.
     *
     * \param[in] a The first value to divide.
     *
     * \param[in] b The second value to divide.
     *
     * \return Returns a complex value representing the quotient.
     */
    MODEL_PUBLIC_API inline Complex operator/(unsigned short a, const Complex& b) {
        return Complex(a) / b;
    }

    /**
     * Division operator.
     *
     * \param[in] a The first value to divide.
     *
     * \param[in] b The second value to divide.
     *
     * \return Returns a complex value representing the quotient.
     */
    MODEL_PUBLIC_API inline Complex operator/(signed short a, const Complex& b) {
        return Complex(a) / b;
    }

    /**
     * Division operator.
     *
     * \param[in] a The first value to divide.
     *
     * \param[in] b The second value to divide.
     *
     * \return Returns a complex value representing the quotient.
     */
    MODEL_PUBLIC_API inline Complex operator/(unsigned a, const Complex& b) {
        return Complex(a) / b;
    }

    /**
     * Division operator.
     *
     * \param[in] a The first value to divide.
     *
     * \param[in] b The second value to divide.
     *
     * \return Returns a complex value representing the quotient.
     */
    MODEL_PUBLIC_API inline Complex operator/(signed a, const Complex& b) {
        return Complex(a) / b;
    }

    /**
     * Division operator.
     *
     * \param[in] a The first value to divide.
     *
     * \param[in] b The second value to divide.
     *
     * \return Returns a complex value representing the quotient.
     */
    MODEL_PUBLIC_API inline Complex operator/(unsigned long a, const Complex& b) {
        return Complex(a) / b;
    }

    /**
     * Division operator.
     *
     * \param[in] a The first value to divide.
     *
     * \param[in] b The second value to divide.
     *
     * \return Returns a complex value representing the quotient.
     */
    MODEL_PUBLIC_API inline Complex operator/(signed long a, const Complex& b) {
        return Complex(a) / b;
    }

    /**
     * Division operator.
     *
     * \param[in] a The first value to divide.
     *
     * \param[in] b The second value to divide.
     *
     * \return Returns a complex value representing the quotient.
     */
    MODEL_PUBLIC_API inline Complex operator/(unsigned long long a, const Complex& b) {
        return Complex(a) / b;
    }

    /**
     * Comparison operator
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if the values are equal.  Returns false if the values are not equal.
     */
    MODEL_PUBLIC_API inline bool operator==(Complex::T a, const Complex& b) {
        return a == b.r && b.i == 0;
    }

    /**
     * Comparison operator
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if the values are equal.  Returns false if the values are not equal.
     */
    MODEL_PUBLIC_API inline bool operator==(Boolean a, const Complex& b) {
        return b.operator==(a);
    }

    /**
     * Comparison operator
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if the values are equal.  Returns false if the values are not equal.
     */
    MODEL_PUBLIC_API inline bool operator==(Integer a, const Complex& b) {
        return b.operator==(a);
    }

    /**
     * Comparison operator
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if the values are equal.  Returns false if the values are not equal.
     */
    MODEL_PUBLIC_API inline bool operator==(unsigned char a, const Complex& b) {
        return b.operator==(a);
    }

    /**
     * Comparison operator
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if the values are equal.  Returns false if the values are not equal.
     */
    MODEL_PUBLIC_API inline bool operator==(signed char a, const Complex& b) {
        return b.operator==(a);
    }

    /**
     * Comparison operator
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if the values are equal.  Returns false if the values are not equal.
     */
    MODEL_PUBLIC_API inline bool operator==(unsigned short a, const Complex& b) {
        return b.operator==(a);
    }

    /**
     * Comparison operator
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if the values are equal.  Returns false if the values are not equal.
     */
    MODEL_PUBLIC_API inline bool operator==(signed short a, const Complex& b) {
        return b.operator==(a);
    }

    /**
     * Comparison operator
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if the values are equal.  Returns false if the values are not equal.
     */
    MODEL_PUBLIC_API inline bool operator==(unsigned a, const Complex& b) {
        return b.operator==(a);
    }

    /**
     * Comparison operator
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if the values are equal.  Returns false if the values are not equal.
     */
    MODEL_PUBLIC_API inline bool operator==(signed a, const Complex& b) {
        return b.operator==(a);
    }

    /**
     * Comparison operator
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if the values are equal.  Returns false if the values are not equal.
     */
    MODEL_PUBLIC_API inline bool operator==(unsigned long a, const Complex& b) {
        return b.operator==(a);
    }

    /**
     * Comparison operator
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if the values are equal.  Returns false if the values are not equal.
     */
    MODEL_PUBLIC_API inline bool operator==(signed long a, const Complex& b) {
        return b.operator==(a);
    }

    /**
     * Comparison operator
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if the values are equal.  Returns false if the values are not equal.
     */
    MODEL_PUBLIC_API inline bool operator==(unsigned long long a, const Complex& b) {
        return b.operator==(a);
    }

    /**
     * Comparison operator
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if the values are not equal.  Returns false if the values are equal.
     */
    MODEL_PUBLIC_API inline bool operator!=(Complex::T a, const Complex& b) {
        return a != b.r || b.i != 0;
    }

    /**
     * Comparison operator
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if the values are not equal.  Returns false if the values are equal.
     */
    MODEL_PUBLIC_API inline bool operator!=(Boolean a, const Complex& b) {
        return b.operator!=(a);
    }

    /**
     * Comparison operator
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if the values are not equal.  Returns false if the values are equal.
     */
    MODEL_PUBLIC_API inline bool operator!=(Integer a, const Complex& b) {
        return b.operator!=(a);
    }

    /**
     * Comparison operator
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if the values are not equal.  Returns false if the values are equal.
     */
    MODEL_PUBLIC_API inline bool operator!=(unsigned char a, const Complex& b) {
        return b.operator!=(a);
    }

    /**
     * Comparison operator
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if the values are not equal.  Returns false if the values are equal.
     */
    MODEL_PUBLIC_API inline bool operator!=(signed char a, const Complex& b) {
        return b.operator!=(a);
    }

    /**
     * Comparison operator
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if the values are not equal.  Returns false if the values are equal.
     */
    MODEL_PUBLIC_API inline bool operator!=(unsigned short a, const Complex& b) {
        return b.operator!=(a);
    }

    /**
     * Comparison operator
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if the values are not equal.  Returns false if the values are equal.
     */
    MODEL_PUBLIC_API inline bool operator!=(signed short a, const Complex& b) {
        return b.operator!=(a);
    }

    /**
     * Comparison operator
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if the values are not equal.  Returns false if the values are equal.
     */
    MODEL_PUBLIC_API inline bool operator!=(unsigned a, const Complex& b) {
        return b.operator!=(a);
    }

    /**
     * Comparison operator
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if the values are not equal.  Returns false if the values are equal.
     */
    MODEL_PUBLIC_API inline bool operator!=(signed a, const Complex& b) {
        return b.operator!=(a);
    }

    /**
     * Comparison operator
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if the values are not equal.  Returns false if the values are equal.
     */
    MODEL_PUBLIC_API inline bool operator!=(unsigned long a, const Complex& b) {
        return b.operator!=(a);
    }

    /**
     * Comparison operator
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if the values are not equal.  Returns false if the values are equal.
     */
    MODEL_PUBLIC_API inline bool operator!=(signed long a, const Complex& b) {
        return b.operator!=(a);
    }

    /**
     * Comparison operator
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns true if the values are not equal.  Returns false if the values are equal.
     */
    MODEL_PUBLIC_API inline bool operator!=(unsigned long long a, const Complex& b) {
        return b.operator!=(a);
    }
}

#endif
