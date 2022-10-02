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
* This header implements support functions for the summation operator class.
***********************************************************************************************************************/

#include <cassert>
#include <complex>
#include <cmath>
#include <limits>
#include <vector>
#include <cassert>
#include <cstdint>
#include <chrono>

#include "m_intrinsics.h"
#include "m_basic_functions.h"
#include "m_matrix_boolean.h"
#include "m_matrix_integer.h"
#include "m_matrix_real.h"
#include "m_matrix_complex.h"
#include "m_summation_operator.h"

namespace M {
    namespace Summation {
        Real errorValue(Boolean last, Boolean current) {
            return last == current ? 0 : 1;
        }


        Real errorValue(Integer last, Integer current) {
            Real l = static_cast<Real>(last);
            Real c = static_cast<Real>(current);

            return c != Real(0) ? M::abs((l - c) / c) : M::abs(l - c);
        }


        Real errorValue(Real last, Real current) {
            return current != 0.0 ? M::abs((last - current) / current) : M::abs(last - current);
        }


        Real errorValue(const Complex& last, const Complex& current) {
            return current != Complex(0) ? M::abs((last - current) / current) : M::abs(last - current);
        }


        Real errorValue(const MatrixBoolean& last, const MatrixBoolean& current) {
            MatrixReal lastAsReal    = M::MatrixReal(last);
            MatrixReal currentAsReal = M::MatrixReal(current);

            Real normDifference = (lastAsReal - currentAsReal).euclideanNorm();
            Real normCurrent    = currentAsReal.euclideanNorm();

            return normCurrent != 0 ? normDifference / normCurrent : normDifference;
        }


        Real errorValue(const MatrixInteger& last, const MatrixInteger& current) {
            MatrixReal lastAsReal    = M::MatrixReal(last);
            MatrixReal currentAsReal = M::MatrixReal(current);

            Real normDifference = (lastAsReal - currentAsReal).euclideanNorm();
            Real normCurrent    = currentAsReal.euclideanNorm();

            return normCurrent != 0 ? normDifference / normCurrent : normDifference;
        }


        Real errorValue(const MatrixReal& last, const MatrixReal& current) {
            Real normDifference = (last - current).euclideanNorm();
            Real normCurrent    = current.euclideanNorm();

            return normCurrent != 0 ? normDifference / normCurrent : normDifference;
        }


        Real errorValue(const MatrixComplex& last, const MatrixComplex& current) {
            Real normDifference = M::abs((last - current).euclideanNorm());
            Real normCurrent    = M::abs(current.euclideanNorm());

            return normCurrent != 0 ? normDifference / normCurrent : normDifference;
        }


        bool isInfinite(Real v) {
            return std::isinf(v);
        }


        bool isInfinite(const Complex& v) {
            return std::isinf(v.real()) || std::isinf(v.imag());
        }
    }
}
