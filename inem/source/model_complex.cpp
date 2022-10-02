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
* This file implements the \ref Model::Complex class and core math functions.
***********************************************************************************************************************/

#include <cmath>
#include <limits>

#include "model_intrinsic_types.h"
#include "model_exceptions.h"
#include "model_variant.h"
#include "m_variant.h"
#include "model_complex.h"

namespace Model {
    static const Complex::T infinity = std::numeric_limits<Complex::T>::infinity();


    Complex::Complex(const Variant& other) {
        bool ok;
        Complex v = other.toComplex(&ok);

        if (ok) {
            this->r = v.r;
            this->i = v.i;
        } else {
            throw InvalidRuntimeConversion(other.valueType(), ValueType::COMPLEX);
        }
    }


    Complex::Complex(const M::Variant& other) {
        bool ok;
        Complex v = other.toComplex(&ok);

        if (ok) {
            this->r = v.r;
            this->i = v.i;
        } else {
            throw InvalidRuntimeConversion(other.valueType(), ValueType::COMPLEX);
        }
    }


    Complex& Complex::operator=(const Variant& other) {
        bool ok;
        Complex v = other.toComplex(&ok);

        if (ok) {
            this->r = v.r;
            this->i = v.i;
        } else {
            throw InvalidRuntimeConversion(other.valueType(), ValueType::COMPLEX);
        }

        return *this;
    }


    Complex& Complex::operator=(const M::Variant& other) {
        bool ok;
        Complex v = other.toComplex(&ok);

        if (ok) {
            this->r = v.r;
            this->i = v.i;
        } else {
            throw InvalidRuntimeConversion(other.valueType(), ValueType::COMPLEX);
        }

        return *this;
    }


    Complex& Complex::operator*=(const Complex& v) {
        T nr = (r * v.r) - (i * v.i);
        i    = (i * v.r) + (r * v.i);
        r = nr;

        return *this;
    }


    Complex Complex::operator*(const Complex& v) const {
        return Complex((r * v.r) - (i * v.i), (i * v.r) + (r * v.i));
    }


    Complex Complex::operator/(const Complex& v) const {
        static_assert(std::numeric_limits<T>::has_infinity,  "Complex::T type does not support infinity");
        static_assert(std::numeric_limits<T>::has_quiet_NaN, "Complex::T type does not support quiet NaN");

        // Note that some of this was inspired by reviewing the CLANG C++ standard library implementation.

        T dividendReal      = r;
        T dividendImaginary = i;
        T divisorReal       = v.r;
        T divisorImaginary  = v.i;

        // Keep all numbers close to 1.0 based on the maximum value in the denominator.
        // TODO: Is it faster to just use ilogb here and avoid the casts below ?
        T   radixBase     = std::logb(std::fmax(std::fabs(divisorReal), std::fabs(divisorImaginary)));
        int radixExponent = static_cast<int>(radixBase);

        T denominator;
        T quotientReal;
        T quotientImaginary;

        if (std::isfinite(radixBase) && radixExponent != 0) {
            divisorReal       = std::scalbn(divisorReal, -radixExponent);
            divisorImaginary  = std::scalbn(divisorImaginary, -radixExponent);

            T numeratorRealUnscaled      = dividendReal * divisorReal + dividendImaginary * divisorImaginary;
            T numeratorImaginaryUnscaled = dividendImaginary * divisorReal - dividendReal * divisorImaginary;

            denominator       = divisorReal * divisorReal + divisorImaginary * divisorImaginary;
            quotientReal      = std::scalbn(numeratorRealUnscaled / denominator, -radixExponent);
            quotientImaginary = std::scalbn(numeratorImaginaryUnscaled / denominator, -radixExponent);
        } else {
            T numeratorRealUnscaled      = dividendReal * divisorReal + dividendImaginary * divisorImaginary;
            T numeratorImaginaryUnscaled = dividendImaginary * divisorReal - dividendReal * divisorImaginary;

            denominator       = divisorReal * divisorReal + divisorImaginary * divisorImaginary;
            quotientReal      = numeratorRealUnscaled / denominator;
            quotientImaginary = numeratorImaginaryUnscaled / denominator;
        }

        if (std::isnan(quotientReal) && std::isnan(quotientImaginary)) {
            if (denominator == 0 && (!std::isnan(dividendReal) || !std::isnan(dividendImaginary))) {
                quotientReal      = copysign(infinity, divisorReal) * dividendReal;
                quotientImaginary = copysign(infinity, divisorReal) * dividendImaginary;
            } else if (!std::isinf(denominator) && (std::isinf(dividendReal) || std::isinf(dividendImaginary))) {
                dividendReal      = copysign(std::isinf(dividendReal) ? T(1) : T(0), dividendReal);
                dividendImaginary = copysign(std::isinf(dividendImaginary) ? T(1) : T(0), dividendImaginary);

                quotientReal      = infinity * (dividendReal * divisorReal + dividendImaginary * divisorImaginary);
                quotientImaginary = infinity * (dividendImaginary * divisorReal - dividendReal * divisorImaginary);
            } else if (std::isinf(radixBase)            &&
                       radixBase > T(0)                 &&
                       std::isfinite(dividendReal)      &&
                       std::isfinite(dividendImaginary)    ) {
                // Radix is infinitely large and we have a finite dividend.  Result will be 0 (signed).
                divisorReal      = copysign(std::isinf(divisorReal) ? T(1) : T(0), divisorReal);
                divisorImaginary = copysign(std::isinf(divisorImaginary) ? T(1) : T(0), divisorImaginary);

                quotientReal      = T(0) * (dividendReal * divisorReal + divisorImaginary * divisorImaginary);
                quotientImaginary = T(0) * (dividendImaginary * divisorReal - dividendReal * divisorImaginary);
            }
        }

        return Complex(quotientReal, quotientImaginary);
    }


    Complex::operator Boolean() const {
        return r != Real(0) || i != Real(0);
    }
}
