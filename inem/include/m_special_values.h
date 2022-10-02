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
* This header defines special values available to the customer model.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_SPECIAL_VALUES_H
#define M_SPECIAL_VALUES_H

#include "m_common.h"
#include "m_intrinsic_types.h"
#include "m_set.h"

namespace M {
    /**
     * Pi
     */
    M_PUBLIC_API extern const Real pi;

    /**
     * Euler's number
     */
    M_PUBLIC_API extern const Real e;

    /**
     * Epsilon
     */
    M_PUBLIC_API extern const Real epsilon;

    /**
     * Infinity.
     */
    M_PUBLIC_API extern const Real infinity;

    /**
     * NaN
     */
    M_PUBLIC_API extern const Real NaN;

    /**
     * Null set.
     */
    M_PUBLIC_API extern const Set nullSet;
}

#endif
