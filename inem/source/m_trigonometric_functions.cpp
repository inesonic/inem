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
* This file implements a collection of trignometric functions.
***********************************************************************************************************************/

#include <cassert>
#include <complex>
#include <cmath>
#include <limits>
#include <vector>

#include "model_exceptions.h"
#include "m_api_types.h"
#include "m_intrinsic_types.h"
#include "m_special_values.h"
#include "m_trigonometric_functions.h"

namespace M {
    static inline const std::complex<Complex::T>& toStdComplex(const Complex& value) {
        return reinterpret_cast<const std::complex<Complex::T>&>(value);
    }

    static inline const Complex& toComplex(const std::complex<Complex::T>& value) {
        return reinterpret_cast<const Complex&>(value);
    }


    Real sine(Real angle) {
        return std::sin(angle);
    }


    Complex sine(const Complex& angle) {
        return toComplex(std::sin(toStdComplex(angle)));
    }


    Real cosine(Real angle) {
        return std::cos(angle);
    }


    Complex cosine(const Complex& angle) {
        return toComplex(std::cos(toStdComplex(angle)));
    }


    Real tangent(Real angle) {
        return std::tan(angle);
    }


    Complex tangent(const Complex& angle) {
        return toComplex(std::tan(toStdComplex(angle)));
    }


    Real secant(Real angle) {
        return 1.0/std::cos(angle);
    }


    Complex secant(const Complex& angle) {
        return 1.0/cosine(angle);
    }


    Real cosecant(Real angle) {
        return 1.0/std::sin(angle);
    }


    Complex cosecant(const Complex& angle) {
        return 1.0/sine(angle);
    }


    Real cotangent(Real angle) {
        return 1.0/std::tan(angle);
    }


    Complex cotangent(const Complex& angle) {
        return 1.0/tangent(angle);
    }


    Real arcsine(Real value) {
        return std::asin(value);
    }


    Complex arcsine(const Complex& value) {
        return toComplex(std::asin(toStdComplex(value)));
    }


    Real arccosine(Real value) {
        return std::acos(value);
    }


    Complex arccosine(const Complex& value) {
        return toComplex(std::acos(toStdComplex(value)));
    }


    Real arctangent(Real value) {
        return std::atan(value);
    }


    Complex arctangent(const Complex& value) {
        return toComplex(std::atan(toStdComplex(value)));
    }


    Real arcsecant(Real value) {
        return std::acos(1.0/value);
    }


    Complex arcsecant(const Complex& value) {
        return toComplex(std::acos(toStdComplex(1.0/value)));
    }


    Real arccosecant(Real value) {
        return std::asin(1.0/value);
    }


    Complex arccosecant(const Complex& value) {
        return toComplex(std::asin(toStdComplex(1.0/value)));
    }


    Real arccotangent(Real value) {
        return std::atan(1.0/value);
    }


    Complex arccotangent(const Complex& value) {
        return toComplex(std::atan(toStdComplex(1.0/value)));
    }
}
