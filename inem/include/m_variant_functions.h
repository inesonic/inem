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
* This header defines functions that operate on variant types.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_VARIANT_FUNCTIONS_H
#define M_VARIANT_FUNCTIONS_H

#include "m_common.h"
#include "m_intrinsic_types.h"

namespace M {
    class Set;
    class Tuple;
    class MatrixBoolean;
    class MatrixInteger;
    class MatrixReal;
    class Variant;
}

namespace M {
    /**
     * Function that calculates different values based on the supplied parameter type aligning with the absolute value
     * operator.
     *
     * \param[in] value THe value of interest.
     *
     * \return Returns the set cardinality.
     */
    M_PUBLIC_API Variant abs(const Variant& value);

    /**
     * Function that calculates the size of a container object.
     *
     * \param[in] tuple The container to calculate the size of.
     *
     * \return Returns the size of the container, in entries.
     */
    M_PUBLIC_API Integer size(const Variant& tuple);

    /**
     * Function that calculates the alphabet of a set.
     *
     * \param[in] set The set to calculate the alphabet of.
     *
     * \return Returns a set representing the alphabet of the supplied set.
     */
    M_PUBLIC_API Set alphabet(const Variant& set);

    /**
     * Function that calculates the complex conjugate of a value.
     *
     * \param[in] value The value to calculate the complex conjugate of.
     *
     * \return Returns the complex conjugate.
     */
    M_PUBLIC_API Variant conj(const Variant& value);

    /**
     * Function that calculates the real portion of a value.
     *
     * \param[in] value The value to calculate the real portion of.
     *
     * \return Returns the real portion of the value.
     */
    M_PUBLIC_API Variant real(const Variant& value);

    /**
     * Function that calculates the imaginary portion of a value.
     *
     * \param[in] value The value to calculate the imaginary portion of.
     *
     * \return Returns the imaginary portion of the value.
     */
    M_PUBLIC_API Variant imag(const Variant& value);
}

#endif
