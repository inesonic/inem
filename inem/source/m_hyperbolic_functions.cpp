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
* This file implements a collection of hyperbolic functions.
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
#include "m_hyperbolic_functions.h"

namespace M {
    static inline const std::complex<Complex::T>& toStdComplex(const Complex& value) {
        return reinterpret_cast<const std::complex<Complex::T>&>(value);
    }

    static inline const Complex& toComplex(const std::complex<Complex::T>& value) {
        return reinterpret_cast<const Complex&>(value);
    }


    Real hyperbolicSine(Real value) {
        return std::sinh(value);
    }


    Complex hyperbolicSine(const Complex& value) {
        return toComplex(std::sinh(toStdComplex(value)));
    }


    Real hyperbolicCosine(Real value) {
        return std::cosh(value);
    }


    Complex hyperbolicCosine(const Complex& value) {
        return toComplex(std::cosh(toStdComplex(value)));
    }


    Real hyperbolicTangent(Real value) {
        return std::tanh(value);
    }


    Complex hyperbolicTangent(const Complex& value) {
        return toComplex(std::tanh(toStdComplex(value)));
    }


    Real hyperbolicSecant(Real value) {
        return 1.0/std::cosh(value);
    }


    Complex hyperbolicSecant(const Complex& value) {
        return 1.0/hyperbolicCosine(value);
    }


    Real hyperbolicCosecant(Real value) {
        return 1.0/std::sinh(value);
    }


    Complex hyperbolicCosecant(const Complex& value) {
        return 1.0/hyperbolicSine(value);
    }


    Real hyperbolicCotangent(Real value) {
        return 1.0/std::tanh(value);
    }


    Complex hyperbolicCotangent(const Complex& value) {
        return 1.0/hyperbolicTangent(value);
    }


    Real hyperbolicArsine(Real value) {
        return std::asinh(value);
    }


    Complex hyperbolicArsine(const Complex& value) {
        return toComplex(std::asinh(toStdComplex(value)));
    }


    Real hyperbolicArcosine(Real value) {
        return std::acosh(value);
    }


    Complex hyperbolicArcosine(const Complex& value) {
        return toComplex(std::acosh(toStdComplex(value)));
    }


    Real hyperbolicArtangent(Real value) {
        return std::atanh(value);
    }


    Complex hyperbolicArtangent(const Complex& value) {
        return toComplex(std::atanh(toStdComplex(value)));
    }


    Real hyperbolicArsecant(Real value) {
        return std::acosh(1.0/value);
    }


    Complex hyperbolicArsecant(const Complex& value) {
        return toComplex(std::acosh(toStdComplex(1.0/value)));
    }


    Real hyperbolicArcosecant(Real value) {
        return std::asinh(1.0/value);
    }


    Complex hyperbolicArcosecant(const Complex& value) {
        return toComplex(std::asinh(toStdComplex(1.0/value)));
    }


    Real hyperbolicArcotangent(Real value) {
        return std::atanh(1.0/value);
    }


    Complex hyperbolicArcotangent(const Complex& value) {
        return toComplex(std::atanh(toStdComplex(1.0/value)));
    }
}
