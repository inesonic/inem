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
* This file provides space for specific special values.
***********************************************************************************************************************/

#include <limits>
#include <cmath>

#include "m_intrinsic_types.h"
#include "m_set.h"
#include "m_special_values.h"

namespace M {
    const Real pi       = 4.0L * std::atan(1.0L);
    const Real e        = std::exp(1.0L);
    const Real epsilon  = std::numeric_limits<Real>::epsilon();
    const Real infinity = std::numeric_limits<Real>::infinity();
    const Real NaN      = std::numeric_limits<Real>::quiet_NaN();
    const Set  nullSet;
}
