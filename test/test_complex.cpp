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
* This file implements tests of the \ref Model::Complex class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QObject>
#include <QList>
#include <QtTest/QtTest>

#include <limits>
#include <complex>
#include <cmath>
#include <random>

#include <model_complex.h>

#include "test_complex.h"

typedef Model::Complex::T T;
static constexpr T infinity = std::numeric_limits<T>::infinity();
static constexpr T NaN      = std::numeric_limits<T>::quiet_NaN();

struct LimitValues { T ar; T ai; T br; T bi; T rr; T ri; };
const LimitValues limitValues[] = {
    {   T(+0.0),   T(+0.0),   T(+0.0),   T(+0.0),       NaN,       NaN },
    {   T(+0.0),   T(+0.0),   T(+0.0),   T(-0.0),       NaN,       NaN },
    {   T(+0.0),   T(+0.0),   T(+0.0),   T(+1.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+0.0),   T(+0.0),   T(-1.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+0.0),   T(+0.0),  infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+0.0),   T(+0.0), -infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+0.0),   T(+0.0),       NaN,       NaN,       NaN },
    {   T(+0.0),   T(+0.0),   T(-0.0),   T(+0.0),       NaN,       NaN },
    {   T(+0.0),   T(+0.0),   T(-0.0),   T(-0.0),       NaN,       NaN },
    {   T(+0.0),   T(+0.0),   T(-0.0),   T(+1.0),   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(+0.0),   T(-0.0),   T(-1.0),   T(-0.0),   T(+0.0) },
    {   T(+0.0),   T(+0.0),   T(-0.0),  infinity,   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(+0.0),   T(-0.0), -infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+0.0),   T(-0.0),       NaN,       NaN,       NaN },
    {   T(+0.0),   T(+0.0),   T(+1.0),   T(+0.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+0.0),   T(+1.0),   T(-0.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+0.0),   T(+1.0),   T(+1.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+0.0),   T(+1.0),   T(-1.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+0.0),   T(+1.0),  infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+0.0),   T(+1.0), -infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+0.0),   T(+1.0),       NaN,       NaN,       NaN },
    {   T(+0.0),   T(+0.0),   T(-1.0),   T(+0.0),   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(+0.0),   T(-1.0),   T(-0.0),   T(-0.0),   T(+0.0) },
    {   T(+0.0),   T(+0.0),   T(-1.0),   T(+1.0),   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(+0.0),   T(-1.0),   T(-1.0),   T(-0.0),   T(+0.0) },
    {   T(+0.0),   T(+0.0),   T(-1.0),  infinity,   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(+0.0),   T(-1.0), -infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+0.0),   T(-1.0),       NaN,       NaN,       NaN },
    {   T(+0.0),   T(+0.0),  infinity,   T(+0.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+0.0),  infinity,   T(-0.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+0.0),  infinity,   T(+1.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+0.0),  infinity,   T(-1.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+0.0),  infinity,  infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+0.0),  infinity, -infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+0.0),  infinity,       NaN,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+0.0), -infinity,   T(+0.0),   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(+0.0), -infinity,   T(-0.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+0.0), -infinity,   T(+1.0),   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(+0.0), -infinity,   T(-1.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+0.0), -infinity,  infinity,   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(+0.0), -infinity, -infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+0.0), -infinity,       NaN,   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(+0.0),       NaN,   T(+0.0),       NaN,       NaN },
    {   T(+0.0),   T(+0.0),       NaN,   T(-0.0),       NaN,       NaN },
    {   T(+0.0),   T(+0.0),       NaN,   T(+1.0),       NaN,       NaN },
    {   T(+0.0),   T(+0.0),       NaN,   T(-1.0),       NaN,       NaN },
    {   T(+0.0),   T(+0.0),       NaN,  infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+0.0),       NaN, -infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+0.0),       NaN,       NaN,       NaN,       NaN },
    {   T(+0.0),   T(-0.0),   T(+0.0),   T(+0.0),       NaN,       NaN },
    {   T(+0.0),   T(-0.0),   T(+0.0),   T(-0.0),       NaN,       NaN },
    {   T(+0.0),   T(-0.0),   T(+0.0),   T(+1.0),   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(-0.0),   T(+0.0),   T(-1.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-0.0),   T(+0.0),  infinity,   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(-0.0),   T(+0.0), -infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-0.0),   T(+0.0),       NaN,       NaN,       NaN },
    {   T(+0.0),   T(-0.0),   T(-0.0),   T(+0.0),       NaN,       NaN },
    {   T(+0.0),   T(-0.0),   T(-0.0),   T(-0.0),       NaN,       NaN },
    {   T(+0.0),   T(-0.0),   T(-0.0),   T(+1.0),   T(-0.0),   T(+0.0) },
    {   T(+0.0),   T(-0.0),   T(-0.0),   T(-1.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-0.0),   T(-0.0),  infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-0.0),   T(-0.0), -infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-0.0),   T(-0.0),       NaN,       NaN,       NaN },
    {   T(+0.0),   T(-0.0),   T(+1.0),   T(+0.0),   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(-0.0),   T(+1.0),   T(-0.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-0.0),   T(+1.0),   T(+1.0),   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(-0.0),   T(+1.0),   T(-1.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-0.0),   T(+1.0),  infinity,   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(-0.0),   T(+1.0), -infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-0.0),   T(+1.0),       NaN,       NaN,       NaN },
    {   T(+0.0),   T(-0.0),   T(-1.0),   T(+0.0),   T(-0.0),   T(+0.0) },
    {   T(+0.0),   T(-0.0),   T(-1.0),   T(-0.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-0.0),   T(-1.0),   T(+1.0),   T(-0.0),   T(+0.0) },
    {   T(+0.0),   T(-0.0),   T(-1.0),   T(-1.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-0.0),   T(-1.0),  infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-0.0),   T(-1.0), -infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-0.0),   T(-1.0),       NaN,       NaN,       NaN },
    {   T(+0.0),   T(-0.0),  infinity,   T(+0.0),   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(-0.0),  infinity,   T(-0.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-0.0),  infinity,   T(+1.0),   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(-0.0),  infinity,   T(-1.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-0.0),  infinity,  infinity,   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(-0.0),  infinity, -infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-0.0),  infinity,       NaN,   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(-0.0), -infinity,   T(+0.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-0.0), -infinity,   T(-0.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-0.0), -infinity,   T(+1.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-0.0), -infinity,   T(-1.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-0.0), -infinity,  infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-0.0), -infinity, -infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-0.0), -infinity,       NaN,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-0.0),       NaN,   T(+0.0),       NaN,       NaN },
    {   T(+0.0),   T(-0.0),       NaN,   T(-0.0),       NaN,       NaN },
    {   T(+0.0),   T(-0.0),       NaN,   T(+1.0),       NaN,       NaN },
    {   T(+0.0),   T(-0.0),       NaN,   T(-1.0),       NaN,       NaN },
    {   T(+0.0),   T(-0.0),       NaN,  infinity,   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(-0.0),       NaN, -infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-0.0),       NaN,       NaN,       NaN,       NaN },
    {   T(+0.0),   T(+1.0),   T(+0.0),   T(+0.0),       NaN,  infinity },
    {   T(+0.0),   T(+1.0),   T(+0.0),   T(-0.0),       NaN,  infinity },
    {   T(+0.0),   T(+1.0),   T(+0.0),   T(+1.0),   T(+1.0),   T(+0.0) },
    {   T(+0.0),   T(+1.0),   T(+0.0),   T(-1.0),   T(-1.0),   T(+0.0) },
    {   T(+0.0),   T(+1.0),   T(+0.0),  infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+1.0),   T(+0.0), -infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+1.0),   T(+0.0),       NaN,       NaN,       NaN },
    {   T(+0.0),   T(+1.0),   T(-0.0),   T(+0.0),       NaN, -infinity },
    {   T(+0.0),   T(+1.0),   T(-0.0),   T(-0.0),       NaN, -infinity },
    {   T(+0.0),   T(+1.0),   T(-0.0),   T(+1.0),   T(+1.0),   T(-0.0) },
    {   T(+0.0),   T(+1.0),   T(-0.0),   T(-1.0),   T(-1.0),   T(+0.0) },
    {   T(+0.0),   T(+1.0),   T(-0.0),  infinity,   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(+1.0),   T(-0.0), -infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+1.0),   T(-0.0),       NaN,       NaN,       NaN },
    {   T(+0.0),   T(+1.0),   T(+1.0),   T(+0.0),   T(+0.0),   T(+1.0) },
    {   T(+0.0),   T(+1.0),   T(+1.0),   T(-0.0),   T(+0.0),   T(+1.0) },
    {   T(+0.0),   T(+1.0),   T(+1.0),   T(+1.0),    T(0.5),    T(0.5) },
    {   T(+0.0),   T(+1.0),   T(+1.0),   T(-1.0),   T(-0.5),    T(0.5) },
    {   T(+0.0),   T(+1.0),   T(+1.0),  infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+1.0),   T(+1.0), -infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+1.0),   T(+1.0),       NaN,       NaN,       NaN },
    {   T(+0.0),   T(+1.0),   T(-1.0),   T(+0.0),   T(+0.0),   T(-1.0) },
    {   T(+0.0),   T(+1.0),   T(-1.0),   T(-0.0),   T(-0.0),   T(-1.0) },
    {   T(+0.0),   T(+1.0),   T(-1.0),   T(+1.0),    T(0.5),   T(-0.5) },
    {   T(+0.0),   T(+1.0),   T(-1.0),   T(-1.0),   T(-0.5),   T(-0.5) },
    {   T(+0.0),   T(+1.0),   T(-1.0),  infinity,   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(+1.0),   T(-1.0), -infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+1.0),   T(-1.0),       NaN,       NaN,       NaN },
    {   T(+0.0),   T(+1.0),  infinity,   T(+0.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+1.0),  infinity,   T(-0.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+1.0),  infinity,   T(+1.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+1.0),  infinity,   T(-1.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+1.0),  infinity,  infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+1.0),  infinity, -infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+1.0),  infinity,       NaN,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+1.0), -infinity,   T(+0.0),   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(+1.0), -infinity,   T(-0.0),   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(+1.0), -infinity,   T(+1.0),   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(+1.0), -infinity,   T(-1.0),   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(+1.0), -infinity,  infinity,   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(+1.0), -infinity, -infinity,   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(+1.0), -infinity,       NaN,   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(+1.0),       NaN,   T(+0.0),       NaN,       NaN },
    {   T(+0.0),   T(+1.0),       NaN,   T(-0.0),       NaN,       NaN },
    {   T(+0.0),   T(+1.0),       NaN,   T(+1.0),       NaN,       NaN },
    {   T(+0.0),   T(+1.0),       NaN,   T(-1.0),       NaN,       NaN },
    {   T(+0.0),   T(+1.0),       NaN,  infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+1.0),       NaN, -infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(+1.0),       NaN,       NaN,       NaN,       NaN },
    {   T(+0.0),   T(-1.0),   T(+0.0),   T(+0.0),       NaN, -infinity },
    {   T(+0.0),   T(-1.0),   T(+0.0),   T(-0.0),       NaN, -infinity },
    {   T(+0.0),   T(-1.0),   T(+0.0),   T(+1.0),   T(-1.0),   T(-0.0) },
    {   T(+0.0),   T(-1.0),   T(+0.0),   T(-1.0),   T(+1.0),   T(+0.0) },
    {   T(+0.0),   T(-1.0),   T(+0.0),  infinity,   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(-1.0),   T(+0.0), -infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-1.0),   T(+0.0),       NaN,       NaN,       NaN },
    {   T(+0.0),   T(-1.0),   T(-0.0),   T(+0.0),       NaN,  infinity },
    {   T(+0.0),   T(-1.0),   T(-0.0),   T(-0.0),       NaN,  infinity },
    {   T(+0.0),   T(-1.0),   T(-0.0),   T(+1.0),   T(-1.0),   T(+0.0) },
    {   T(+0.0),   T(-1.0),   T(-0.0),   T(-1.0),   T(+1.0),   T(+0.0) },
    {   T(+0.0),   T(-1.0),   T(-0.0),  infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-1.0),   T(-0.0), -infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-1.0),   T(-0.0),       NaN,       NaN,       NaN },
    {   T(+0.0),   T(-1.0),   T(+1.0),   T(+0.0),   T(+0.0),   T(-1.0) },
    {   T(+0.0),   T(-1.0),   T(+1.0),   T(-0.0),   T(+0.0),   T(-1.0) },
    {   T(+0.0),   T(-1.0),   T(+1.0),   T(+1.0),   T(-0.5),   T(-0.5) },
    {   T(+0.0),   T(-1.0),   T(+1.0),   T(-1.0),    T(0.5),   T(-0.5) },
    {   T(+0.0),   T(-1.0),   T(+1.0),  infinity,   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(-1.0),   T(+1.0), -infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-1.0),   T(+1.0),       NaN,       NaN,       NaN },
    {   T(+0.0),   T(-1.0),   T(-1.0),   T(+0.0),   T(-0.0),   T(+1.0) },
    {   T(+0.0),   T(-1.0),   T(-1.0),   T(-0.0),   T(+0.0),   T(+1.0) },
    {   T(+0.0),   T(-1.0),   T(-1.0),   T(+1.0),   T(-0.5),    T(0.5) },
    {   T(+0.0),   T(-1.0),   T(-1.0),   T(-1.0),    T(0.5),    T(0.5) },
    {   T(+0.0),   T(-1.0),   T(-1.0),  infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-1.0),   T(-1.0), -infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-1.0),   T(-1.0),       NaN,       NaN,       NaN },
    {   T(+0.0),   T(-1.0),  infinity,   T(+0.0),   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(-1.0),  infinity,   T(-0.0),   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(-1.0),  infinity,   T(+1.0),   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(-1.0),  infinity,   T(-1.0),   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(-1.0),  infinity,  infinity,   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(-1.0),  infinity, -infinity,   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(-1.0),  infinity,       NaN,   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(-1.0), -infinity,   T(+0.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-1.0), -infinity,   T(-0.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-1.0), -infinity,   T(+1.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-1.0), -infinity,   T(-1.0),   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-1.0), -infinity,  infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-1.0), -infinity, -infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-1.0), -infinity,       NaN,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-1.0),       NaN,   T(+0.0),       NaN,       NaN },
    {   T(+0.0),   T(-1.0),       NaN,   T(-0.0),       NaN,       NaN },
    {   T(+0.0),   T(-1.0),       NaN,   T(+1.0),       NaN,       NaN },
    {   T(+0.0),   T(-1.0),       NaN,   T(-1.0),       NaN,       NaN },
    {   T(+0.0),   T(-1.0),       NaN,  infinity,   T(+0.0),   T(-0.0) },
    {   T(+0.0),   T(-1.0),       NaN, -infinity,   T(+0.0),   T(+0.0) },
    {   T(+0.0),   T(-1.0),       NaN,       NaN,       NaN,       NaN },
    {   T(+0.0),  infinity,   T(+0.0),   T(+0.0),       NaN,  infinity },
    {   T(+0.0),  infinity,   T(+0.0),   T(-0.0),       NaN,  infinity },
    {   T(+0.0),  infinity,   T(+0.0),   T(+1.0),  infinity,       NaN },
    {   T(+0.0),  infinity,   T(+0.0),   T(-1.0), -infinity,       NaN },
    {   T(+0.0),  infinity,   T(+0.0),  infinity,       NaN,       NaN },
    {   T(+0.0),  infinity,   T(+0.0), -infinity,       NaN,       NaN },
    {   T(+0.0),  infinity,   T(+0.0),       NaN,       NaN,       NaN },
    {   T(+0.0),  infinity,   T(-0.0),   T(+0.0),       NaN, -infinity },
    {   T(+0.0),  infinity,   T(-0.0),   T(-0.0),       NaN, -infinity },
    {   T(+0.0),  infinity,   T(-0.0),   T(+1.0),  infinity,       NaN },
    {   T(+0.0),  infinity,   T(-0.0),   T(-1.0), -infinity,       NaN },
    {   T(+0.0),  infinity,   T(-0.0),  infinity,       NaN,       NaN },
    {   T(+0.0),  infinity,   T(-0.0), -infinity,       NaN,       NaN },
    {   T(+0.0),  infinity,   T(-0.0),       NaN,       NaN,       NaN },
    {   T(+0.0),  infinity,   T(+1.0),   T(+0.0),       NaN,  infinity },
    {   T(+0.0),  infinity,   T(+1.0),   T(-0.0),       NaN,  infinity },
    {   T(+0.0),  infinity,   T(+1.0),   T(+1.0),  infinity,  infinity },
    {   T(+0.0),  infinity,   T(+1.0),   T(-1.0), -infinity,  infinity },
    {   T(+0.0),  infinity,   T(+1.0),  infinity,       NaN,       NaN },
    {   T(+0.0),  infinity,   T(+1.0), -infinity,       NaN,       NaN },
    {   T(+0.0),  infinity,   T(+1.0),       NaN,       NaN,       NaN },
    {   T(+0.0),  infinity,   T(-1.0),   T(+0.0),       NaN, -infinity },
    {   T(+0.0),  infinity,   T(-1.0),   T(-0.0),       NaN, -infinity },
    {   T(+0.0),  infinity,   T(-1.0),   T(+1.0),  infinity, -infinity },
    {   T(+0.0),  infinity,   T(-1.0),   T(-1.0), -infinity, -infinity },
    {   T(+0.0),  infinity,   T(-1.0),  infinity,       NaN,       NaN },
    {   T(+0.0),  infinity,   T(-1.0), -infinity,       NaN,       NaN },
    {   T(+0.0),  infinity,   T(-1.0),       NaN,       NaN,       NaN },
    {   T(+0.0),  infinity,  infinity,   T(+0.0),       NaN,       NaN },
    {   T(+0.0),  infinity,  infinity,   T(-0.0),       NaN,       NaN },
    {   T(+0.0),  infinity,  infinity,   T(+1.0),       NaN,       NaN },
    {   T(+0.0),  infinity,  infinity,   T(-1.0),       NaN,       NaN },
    {   T(+0.0),  infinity,  infinity,  infinity,       NaN,       NaN },
    {   T(+0.0),  infinity,  infinity, -infinity,       NaN,       NaN },
    {   T(+0.0),  infinity,  infinity,       NaN,       NaN,       NaN },
    {   T(+0.0),  infinity, -infinity,   T(+0.0),       NaN,       NaN },
    {   T(+0.0),  infinity, -infinity,   T(-0.0),       NaN,       NaN },
    {   T(+0.0),  infinity, -infinity,   T(+1.0),       NaN,       NaN },
    {   T(+0.0),  infinity, -infinity,   T(-1.0),       NaN,       NaN },
    {   T(+0.0),  infinity, -infinity,  infinity,       NaN,       NaN },
    {   T(+0.0),  infinity, -infinity, -infinity,       NaN,       NaN },
    {   T(+0.0),  infinity, -infinity,       NaN,       NaN,       NaN },
    {   T(+0.0),  infinity,       NaN,   T(+0.0),       NaN,       NaN },
    {   T(+0.0),  infinity,       NaN,   T(-0.0),       NaN,       NaN },
    {   T(+0.0),  infinity,       NaN,   T(+1.0),       NaN,       NaN },
    {   T(+0.0),  infinity,       NaN,   T(-1.0),       NaN,       NaN },
    {   T(+0.0),  infinity,       NaN,  infinity,       NaN,       NaN },
    {   T(+0.0),  infinity,       NaN, -infinity,       NaN,       NaN },
    {   T(+0.0),  infinity,       NaN,       NaN,       NaN,       NaN },
    {   T(+0.0), -infinity,   T(+0.0),   T(+0.0),       NaN, -infinity },
    {   T(+0.0), -infinity,   T(+0.0),   T(-0.0),       NaN, -infinity },
    {   T(+0.0), -infinity,   T(+0.0),   T(+1.0), -infinity,       NaN },
    {   T(+0.0), -infinity,   T(+0.0),   T(-1.0),  infinity,       NaN },
    {   T(+0.0), -infinity,   T(+0.0),  infinity,       NaN,       NaN },
    {   T(+0.0), -infinity,   T(+0.0), -infinity,       NaN,       NaN },
    {   T(+0.0), -infinity,   T(+0.0),       NaN,       NaN,       NaN },
    {   T(+0.0), -infinity,   T(-0.0),   T(+0.0),       NaN,  infinity },
    {   T(+0.0), -infinity,   T(-0.0),   T(-0.0),       NaN,  infinity },
    {   T(+0.0), -infinity,   T(-0.0),   T(+1.0), -infinity,       NaN },
    {   T(+0.0), -infinity,   T(-0.0),   T(-1.0),  infinity,       NaN },
    {   T(+0.0), -infinity,   T(-0.0),  infinity,       NaN,       NaN },
    {   T(+0.0), -infinity,   T(-0.0), -infinity,       NaN,       NaN },
    {   T(+0.0), -infinity,   T(-0.0),       NaN,       NaN,       NaN },
    {   T(+0.0), -infinity,   T(+1.0),   T(+0.0),       NaN, -infinity },
    {   T(+0.0), -infinity,   T(+1.0),   T(-0.0),       NaN, -infinity },
    {   T(+0.0), -infinity,   T(+1.0),   T(+1.0), -infinity, -infinity },
    {   T(+0.0), -infinity,   T(+1.0),   T(-1.0),  infinity, -infinity },
    {   T(+0.0), -infinity,   T(+1.0),  infinity,       NaN,       NaN },
    {   T(+0.0), -infinity,   T(+1.0), -infinity,       NaN,       NaN },
    {   T(+0.0), -infinity,   T(+1.0),       NaN,       NaN,       NaN },
    {   T(+0.0), -infinity,   T(-1.0),   T(+0.0),       NaN,  infinity },
    {   T(+0.0), -infinity,   T(-1.0),   T(-0.0),       NaN,  infinity },
    {   T(+0.0), -infinity,   T(-1.0),   T(+1.0), -infinity,  infinity },
    {   T(+0.0), -infinity,   T(-1.0),   T(-1.0),  infinity,  infinity },
    {   T(+0.0), -infinity,   T(-1.0),  infinity,       NaN,       NaN },
    {   T(+0.0), -infinity,   T(-1.0), -infinity,       NaN,       NaN },
    {   T(+0.0), -infinity,   T(-1.0),       NaN,       NaN,       NaN },
    {   T(+0.0), -infinity,  infinity,   T(+0.0),       NaN,       NaN },
    {   T(+0.0), -infinity,  infinity,   T(-0.0),       NaN,       NaN },
    {   T(+0.0), -infinity,  infinity,   T(+1.0),       NaN,       NaN },
    {   T(+0.0), -infinity,  infinity,   T(-1.0),       NaN,       NaN },
    {   T(+0.0), -infinity,  infinity,  infinity,       NaN,       NaN },
    {   T(+0.0), -infinity,  infinity, -infinity,       NaN,       NaN },
    {   T(+0.0), -infinity,  infinity,       NaN,       NaN,       NaN },
    {   T(+0.0), -infinity, -infinity,   T(+0.0),       NaN,       NaN },
    {   T(+0.0), -infinity, -infinity,   T(-0.0),       NaN,       NaN },
    {   T(+0.0), -infinity, -infinity,   T(+1.0),       NaN,       NaN },
    {   T(+0.0), -infinity, -infinity,   T(-1.0),       NaN,       NaN },
    {   T(+0.0), -infinity, -infinity,  infinity,       NaN,       NaN },
    {   T(+0.0), -infinity, -infinity, -infinity,       NaN,       NaN },
    {   T(+0.0), -infinity, -infinity,       NaN,       NaN,       NaN },
    {   T(+0.0), -infinity,       NaN,   T(+0.0),       NaN,       NaN },
    {   T(+0.0), -infinity,       NaN,   T(-0.0),       NaN,       NaN },
    {   T(+0.0), -infinity,       NaN,   T(+1.0),       NaN,       NaN },
    {   T(+0.0), -infinity,       NaN,   T(-1.0),       NaN,       NaN },
    {   T(+0.0), -infinity,       NaN,  infinity,       NaN,       NaN },
    {   T(+0.0), -infinity,       NaN, -infinity,       NaN,       NaN },
    {   T(+0.0), -infinity,       NaN,       NaN,       NaN,       NaN },
    {   T(+0.0),       NaN,   T(+0.0),   T(+0.0),       NaN,       NaN },
    {   T(+0.0),       NaN,   T(+0.0),   T(-0.0),       NaN,       NaN },
    {   T(+0.0),       NaN,   T(+0.0),   T(+1.0),       NaN,       NaN },
    {   T(+0.0),       NaN,   T(+0.0),   T(-1.0),       NaN,       NaN },
    {   T(+0.0),       NaN,   T(+0.0),  infinity,       NaN,       NaN },
    {   T(+0.0),       NaN,   T(+0.0), -infinity,       NaN,       NaN },
    {   T(+0.0),       NaN,   T(+0.0),       NaN,       NaN,       NaN },
    {   T(+0.0),       NaN,   T(-0.0),   T(+0.0),       NaN,       NaN },
    {   T(+0.0),       NaN,   T(-0.0),   T(-0.0),       NaN,       NaN },
    {   T(+0.0),       NaN,   T(-0.0),   T(+1.0),       NaN,       NaN },
    {   T(+0.0),       NaN,   T(-0.0),   T(-1.0),       NaN,       NaN },
    {   T(+0.0),       NaN,   T(-0.0),  infinity,       NaN,       NaN },
    {   T(+0.0),       NaN,   T(-0.0), -infinity,       NaN,       NaN },
    {   T(+0.0),       NaN,   T(-0.0),       NaN,       NaN,       NaN },
    {   T(+0.0),       NaN,   T(+1.0),   T(+0.0),       NaN,       NaN },
    {   T(+0.0),       NaN,   T(+1.0),   T(-0.0),       NaN,       NaN },
    {   T(+0.0),       NaN,   T(+1.0),   T(+1.0),       NaN,       NaN },
    {   T(+0.0),       NaN,   T(+1.0),   T(-1.0),       NaN,       NaN },
    {   T(+0.0),       NaN,   T(+1.0),  infinity,       NaN,       NaN },
    {   T(+0.0),       NaN,   T(+1.0), -infinity,       NaN,       NaN },
    {   T(+0.0),       NaN,   T(+1.0),       NaN,       NaN,       NaN },
    {   T(+0.0),       NaN,   T(-1.0),   T(+0.0),       NaN,       NaN },
    {   T(+0.0),       NaN,   T(-1.0),   T(-0.0),       NaN,       NaN },
    {   T(+0.0),       NaN,   T(-1.0),   T(+1.0),       NaN,       NaN },
    {   T(+0.0),       NaN,   T(-1.0),   T(-1.0),       NaN,       NaN },
    {   T(+0.0),       NaN,   T(-1.0),  infinity,       NaN,       NaN },
    {   T(+0.0),       NaN,   T(-1.0), -infinity,       NaN,       NaN },
    {   T(+0.0),       NaN,   T(-1.0),       NaN,       NaN,       NaN },
    {   T(+0.0),       NaN,  infinity,   T(+0.0),       NaN,       NaN },
    {   T(+0.0),       NaN,  infinity,   T(-0.0),       NaN,       NaN },
    {   T(+0.0),       NaN,  infinity,   T(+1.0),       NaN,       NaN },
    {   T(+0.0),       NaN,  infinity,   T(-1.0),       NaN,       NaN },
    {   T(+0.0),       NaN,  infinity,  infinity,       NaN,       NaN },
    {   T(+0.0),       NaN,  infinity, -infinity,       NaN,       NaN },
    {   T(+0.0),       NaN,  infinity,       NaN,       NaN,       NaN },
    {   T(+0.0),       NaN, -infinity,   T(+0.0),       NaN,       NaN },
    {   T(+0.0),       NaN, -infinity,   T(-0.0),       NaN,       NaN },
    {   T(+0.0),       NaN, -infinity,   T(+1.0),       NaN,       NaN },
    {   T(+0.0),       NaN, -infinity,   T(-1.0),       NaN,       NaN },
    {   T(+0.0),       NaN, -infinity,  infinity,       NaN,       NaN },
    {   T(+0.0),       NaN, -infinity, -infinity,       NaN,       NaN },
    {   T(+0.0),       NaN, -infinity,       NaN,       NaN,       NaN },
    {   T(+0.0),       NaN,       NaN,   T(+0.0),       NaN,       NaN },
    {   T(+0.0),       NaN,       NaN,   T(-0.0),       NaN,       NaN },
    {   T(+0.0),       NaN,       NaN,   T(+1.0),       NaN,       NaN },
    {   T(+0.0),       NaN,       NaN,   T(-1.0),       NaN,       NaN },
    {   T(+0.0),       NaN,       NaN,  infinity,       NaN,       NaN },
    {   T(+0.0),       NaN,       NaN, -infinity,       NaN,       NaN },
    {   T(+0.0),       NaN,       NaN,       NaN,       NaN,       NaN },
};

TestComplex::TestComplex() {}


TestComplex::~TestComplex() {}


void TestComplex::testConstructorAndDestructors() {
    Model::Complex c1;

    QCOMPARE(c1.real(), Model::Complex::T(0));
    QCOMPARE(c1.imag(), Model::Complex::T(0));

    Model::Complex c2(1, 2);

    QCOMPARE(c2.real(), Model::Complex::T(1));
    QCOMPARE(c2.imag(), Model::Complex::T(2));

    Model::Complex c3(3);

    QCOMPARE(c3.real(), Model::Complex::T(3));
    QCOMPARE(c3.imag(), Model::Complex::T(0));

    Model::Complex c4(c2);

    QCOMPARE(c4.real(), Model::Complex::T(1));
    QCOMPARE(c4.imag(), Model::Complex::T(2));

    Model::Complex c5(c3);

    QCOMPARE(c5.real(), Model::Complex::T(3));
    QCOMPARE(c5.imag(), Model::Complex::T(0));
}


void TestComplex::testAssignmentOperators() {
    // Note that we stick with values that will map to powers of 2 to avoid false failures due to rounding errors.

    Model::Complex c1;
    Model::Complex c2(1, 2);
    Model::Complex c3(3);

    QCOMPARE(c1.real(), Model::Complex::T(0));
    QCOMPARE(c1.imag(), Model::Complex::T(0));

    QCOMPARE(c2.real(), Model::Complex::T(1));
    QCOMPARE(c2.imag(), Model::Complex::T(2));

    QCOMPARE(c3.real(), Model::Complex::T(3));
    QCOMPARE(c3.imag(), Model::Complex::T(0));

    c1 = c2;

    QCOMPARE(c1.real(), Model::Complex::T(1));
    QCOMPARE(c1.imag(), Model::Complex::T(2));

    c1 = 5;

    QCOMPARE(c1.real(), Model::Complex::T(5));
    QCOMPARE(c1.imag(), Model::Complex::T(0));

    c1 = c3;

    QCOMPARE(c1.real(), Model::Complex::T(3));
    QCOMPARE(c1.imag(), Model::Complex::T(0));

    c1 += c2;

    QCOMPARE(c1.real(), Model::Complex::T(4));
    QCOMPARE(c1.imag(), Model::Complex::T(2));

    c1 += 3;

    QCOMPARE(c1.real(), Model::Complex::T(7));
    QCOMPARE(c1.imag(), Model::Complex::T(2));

    c1 -= c2;

    QCOMPARE(c1.real(), Model::Complex::T(6));
    QCOMPARE(c1.imag(), Model::Complex::T(0));

    c1 -= 2;

    QCOMPARE(c1.real(), Model::Complex::T(4));
    QCOMPARE(c1.imag(), Model::Complex::T(0));

    c1 += Model::Complex(0, 2);

    QCOMPARE(c1.real(), Model::Complex::T(4));
    QCOMPARE(c1.imag(), Model::Complex::T(2));

    c1 *= Model::Complex(2, 3);

    QCOMPARE(c1.real(), Model::Complex::T(2));
    QCOMPARE(c1.imag(), Model::Complex::T(16));

    c1 *= 3;

    QCOMPARE(c1.real(), Model::Complex::T(6));
    QCOMPARE(c1.imag(), Model::Complex::T(48));

    c1 /= Model::Complex(2, 2);

    QCOMPARE(c1.real(), Model::Complex::T(13.5));
    QCOMPARE(c1.imag(), Model::Complex::T(10.5));

    c1 /= 2;

    QCOMPARE(c1.real(), Model::Complex::T(6.75));
    QCOMPARE(c1.imag(), Model::Complex::T(5.25));
}


void TestComplex::testMathOperators() {
    Model::Complex c1;
    Model::Complex c2(1, 2);
    Model::Complex c3(3, 5);

    c1 = c2 + c3;

    QCOMPARE(c1.real(), Model::Complex::T(4));
    QCOMPARE(c1.imag(), Model::Complex::T(7));

    c1 = c2 + 4;

    QCOMPARE(c1.real(), Model::Complex::T(5));
    QCOMPARE(c1.imag(), Model::Complex::T(2));

    c1 = 4 + c2;

    QCOMPARE(c1.real(), Model::Complex::T(5));
    QCOMPARE(c1.imag(), Model::Complex::T(2));

    c1 = c3 - c2;

    QCOMPARE(c1.real(), Model::Complex::T(2));
    QCOMPARE(c1.imag(), Model::Complex::T(3));

    c1 = c3 - 2;

    QCOMPARE(c1.real(), Model::Complex::T(1));
    QCOMPARE(c1.imag(), Model::Complex::T(5));

    c1 = 3 - c2;

    QCOMPARE(c1.real(), Model::Complex::T(2));
    QCOMPARE(c1.imag(), Model::Complex::T(-2));

    c1 = c2 * c3;

    QCOMPARE(c1.real(), Model::Complex::T(-7));
    QCOMPARE(c1.imag(), Model::Complex::T(11));

    c1 = c2 * 2;

    QCOMPARE(c1.real(), Model::Complex::T(2));
    QCOMPARE(c1.imag(), Model::Complex::T(4));

    c1 = 3 * c2;

    QCOMPARE(c1.real(), Model::Complex::T(3));
    QCOMPARE(c1.imag(), Model::Complex::T(6));

    c1 = c3 / c2;

    Model::Complex::T errorLimit = 1000.0 * std::numeric_limits<Model::Complex::T>::epsilon();
    Model::Complex::T realError  = abs(c1.real() - Model::Complex::T(2.6));
    Model::Complex::T imagError  = abs(c1.imag() - Model::Complex::T(-0.2));

    QVERIFY(realError < errorLimit);
    QVERIFY(imagError < errorLimit);

    c1 = c3 / 2;

    QCOMPARE(c1.real(), Model::Complex::T(1.5));
    QCOMPARE(c1.imag(), Model::Complex::T(2.5));

    c1 = 2 / c2;

    realError  = abs(c1.real() - Model::Complex::T( 0.4));
    imagError  = abs(c1.imag() - Model::Complex::T(-0.8));

    QVERIFY(realError < errorLimit);
    QVERIFY(imagError < errorLimit);
}


void TestComplex::testComparisonOperators() {
    Model::Complex c1;
    Model::Complex c2(1, 2);
    Model::Complex c3(1, 0);
    Model::Complex c4(0, 1);
    Model::Complex c5(3, 5);

    QCOMPARE(c1 == c1, true);
    QCOMPARE(c1 != c1, false);

    QCOMPARE(c1 == c2, false);
    QCOMPARE(c1 != c2, true);

    QCOMPARE(c1 == c3, false);
    QCOMPARE(c1 != c3, true);

    QCOMPARE(c1 == c4, false);
    QCOMPARE(c1 != c4, true);

    QCOMPARE(c1 == c5, false);
    QCOMPARE(c1 != c5, true);

    QCOMPARE(c2 == c2, true);
    QCOMPARE(c2 != c2, false);

    QCOMPARE(c2 == c3, false);
    QCOMPARE(c2 != c3, true);

    QCOMPARE(c2 == c4, false);
    QCOMPARE(c2 != c4, true);

    QCOMPARE(c2 == c5, false);
    QCOMPARE(c2 != c5, true);

    QCOMPARE(c3 == c3, true);
    QCOMPARE(c3 != c3, false);

    QCOMPARE(c3 == c4, false);
    QCOMPARE(c3 != c4, true);

    QCOMPARE(c3 == c5, false);
    QCOMPARE(c3 != c5, true);

    QCOMPARE(c4 == c4, true);
    QCOMPARE(c4 != c4, false);

    QCOMPARE(c4 == c5, false);
    QCOMPARE(c4 != c5, true);

    QCOMPARE(c5 == c5, true);
    QCOMPARE(c5 != c5, false);
}


void TestComplex::testLimits() {
    // The divide operator has to deal with a number of corner cases involving infinity and NaN.  We test those cases
    // here.  Note that we do not compare against std::complex<Model::Complex::T> because each platform has a slightly
    // different implementation.  The MacOS implementation aligns with our implementation and was used to perform the
    // intial round of testing.  The GCC implementation does not always produce correct results in the corner cases we
    // test here.

    unsigned numberValues = sizeof(limitValues) / sizeof(LimitValues);
    for (unsigned index=0 ; index<numberValues ; ++index) {
        const LimitValues* entry = limitValues + index;

        Model::Complex::T      ar = entry->ar;
        Model::Complex::T      ai = entry->ai;

        Model::Complex::T      br = entry->br;
        Model::Complex::T      bi = entry->bi;

        Model::Complex::T      rr = entry->rr;
        Model::Complex::T      ri = entry->ri;

        Model::Complex         a(ar, ai);
        Model::Complex         b(br, bi);
        Model::Complex         measured = a / b;

        if (std::isnan(measured.real())) {
            QVERIFY(std::isnan(rr));
        } else {
            QCOMPARE(measured.real(), rr);
        }

        if (std::isnan(measured.imag())) {
            QVERIFY(std::isnan(ri));
        } else {
            QCOMPARE(measured.imag(), ri);
        }
    }
}


void TestComplex::testRandomDivide() {
    std::mt19937                                      rng;
    std::uniform_int_distribution<>                   exponentGenerator(-16, +16);
    std::uniform_real_distribution<Model::Complex::T> mantissaGenerator(-1.0, +1.0);

    Model::Complex::T errorLimit = 10.0 * std::numeric_limits<Model::Complex::T>::epsilon();

    for (unsigned i=0 ; i<1000000 ; ++i) {
        int           are = exponentGenerator(rng);
        Model::Complex::T arm = mantissaGenerator(rng);

        int           aie = exponentGenerator(rng);
        Model::Complex::T aim = mantissaGenerator(rng);

        int           bre = exponentGenerator(rng);
        Model::Complex::T brm = mantissaGenerator(rng);

        int           bie = exponentGenerator(rng);
        Model::Complex::T bim = mantissaGenerator(rng);

        Model::Complex::T ar = arm * std::pow(10.0, are);
        Model::Complex::T ai = aim * std::pow(10.0, aie);
        Model::Complex::T br = brm * std::pow(10.0, bre);
        Model::Complex::T bi = bim * std::pow(10.0, bie);

        if (br != Model::Complex::T(0) && bi != Model::Complex::T(0)) {
            Model::Complex a(ar, ai);
            Model::Complex b(br, bi);

            Model::Complex measured = a / b;

            Model::Complex::T denominator        = br * br + bi * bi;
            Model::Complex::T numeratorReal      = ar * br + ai * bi;
            Model::Complex::T numeratorImaginary = ai * br - ar * bi;
            Model::Complex::T expectedReal       = numeratorReal / denominator;
            Model::Complex::T expectedImaginary  = numeratorImaginary / denominator;

            Model::Complex::T errorReal      = std::abs(measured.real() - expectedReal);
            Model::Complex::T errorImaginary = std::abs(measured.imag() - expectedImaginary);

            QVERIFY(errorReal < errorLimit);
            QVERIFY(errorImaginary < errorLimit);
        }
    }
}


void TestComplex::testCastability() {
    Model::Complex a(2.0, 3.0);
    const std::complex<Model::Complex::T>* b = reinterpret_cast<std::complex<Model::Complex::T>*>(&a);

    QCOMPARE(a.real(), b->real());
    QCOMPARE(a.imag(), b->imag());
}
