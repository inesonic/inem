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
* This header defines several additional "operators" needed to support the complex type.
***********************************************************************************************************************/

#include <cmath>

#include "model_complex.h"
#include "m_intrinsic_types.h"
#include "m_exceptions.h"
#include "m_complex_operators.h"

namespace M {
    bool internalCheckIfReal(const Complex& value) {
        return value.imag() == 0;
    }
}


bool operator<(const M::Complex& a, M::Boolean b) {
    bool result;

    if (M::internalCheckIfReal(a)) {
        result = a.real() < static_cast<M::Complex::T>(b);
    } else {
        M::internalTriggerTypeConversionError(M::ValueType::COMPLEX, M::ValueType::REAL);
        result = false;
    }

    return result;
}


bool operator>(const M::Complex& a, M::Boolean b) {
    bool result;

    if (M::internalCheckIfReal(a)) {
        result = a.real() > static_cast<M::Complex::T>(b);
    } else {
        M::internalTriggerTypeConversionError(M::ValueType::COMPLEX, M::ValueType::REAL);
        result = false;
    }

    return result;
}


bool operator<(const M::Complex& a, M::Integer b) {
    bool result;

    if (M::internalCheckIfReal(a)) {
        result = a.real() < static_cast<M::Complex::T>(b);
    } else {
        M::internalTriggerTypeConversionError(M::ValueType::COMPLEX, M::ValueType::REAL);
        result = false;
    }

    return result;
}


bool operator>(const M::Complex& a, M::Integer b) {
    bool result;

    if (M::internalCheckIfReal(a)) {
        result = a.real() > static_cast<M::Complex::T>(b);
    } else {
        M::internalTriggerTypeConversionError(M::ValueType::COMPLEX, M::ValueType::REAL);
        result = false;
    }

    return result;
}


bool operator<(const M::Complex& a, M::Real b) {
    bool result;

    if (M::internalCheckIfReal(a)) {
        result = a.real() < static_cast<M::Complex::T>(b);
    } else {
        M::internalTriggerTypeConversionError(M::ValueType::COMPLEX, M::ValueType::REAL);
        result = false;
    }

    return result;
}


bool operator>(const M::Complex& a, M::Real b) {
    bool result;

    if (M::internalCheckIfReal(a)) {
        result = a.real() > static_cast<M::Complex::T>(b);
    } else {
        M::internalTriggerTypeConversionError(M::ValueType::COMPLEX, M::ValueType::REAL);
        result = false;
    }

    return result;
}


bool operator<(M::Boolean a, const M::Complex& b) {
    bool result;

    if (M::internalCheckIfReal(b)) {
        result = (a ? 1 : 0) < b.real();
    } else {
        M::internalTriggerTypeConversionError(M::ValueType::COMPLEX, M::ValueType::REAL);
        result = false;
    }

    return result;
}


bool operator>(M::Boolean a, const M::Complex& b) {
    bool result;

    if (M::internalCheckIfReal(b)) {
        result = (a ? 1 : 0) > b.real();
    } else {
        M::internalTriggerTypeConversionError(M::ValueType::COMPLEX, M::ValueType::REAL);
        result = false;
    }

    return result;
}


bool operator<(M::Integer a, const M::Complex& b) {
    bool result;

    if (M::internalCheckIfReal(b)) {
        result = M::Complex::T(a) < b.real();
    } else {
        M::internalTriggerTypeConversionError(M::ValueType::COMPLEX, M::ValueType::REAL);
        result = false;
    }

    return result;
}


bool operator>(M::Integer a, const M::Complex& b) {
    bool result;

    if (M::internalCheckIfReal(b)) {
        result = M::Complex::T(a) > b.real();
    } else {
        M::internalTriggerTypeConversionError(M::ValueType::COMPLEX, M::ValueType::REAL);
        result = false;
    }

    return result;
}


bool operator<(M::Real a, const M::Complex& b) {
    bool result;

    if (M::internalCheckIfReal(b)) {
        result = M::Complex::T(a) < b.real();
    } else {
        M::internalTriggerTypeConversionError(M::ValueType::COMPLEX, M::ValueType::REAL);
        result = false;
    }

    return result;
}


bool operator>(M::Real a, const M::Complex& b) {
    bool result;

    if (M::internalCheckIfReal(b)) {
        result = M::Complex::T(a) > b.real();
    } else {
        M::internalTriggerTypeConversionError(M::ValueType::COMPLEX, M::ValueType::REAL);
        result = false;
    }

    return result;
}


bool operator<(const M::Complex& a, const M::Complex& b) {
    bool result;

    if (a.imag() != 0 || b.imag() != 0) {
        M::internalTriggerTypeConversionError(M::ValueType::COMPLEX, M::ValueType::REAL);
        result = false;
    } else {
        result = a.real() < b.real();
    }

    return result;
}


bool operator>(const M::Complex& a, const M::Complex& b) {
    bool result;

    if (M::internalCheckIfReal(a) && M::internalCheckIfReal(b)) {
        result = a.real() > b.real();
    } else {
        M::internalTriggerTypeConversionError(M::ValueType::COMPLEX, M::ValueType::REAL);
        result = false;
    }

    return result;
}

