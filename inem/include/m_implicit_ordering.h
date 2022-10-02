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
* This header defines functions you can use enforce ordering between values of different types.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_IMPLICIT_ORDERING_H
#define M_IMPLICIT_ORDERING_H

#include "m_common.h"
#include "m_api_types.h"
#include "m_intrinsic_types.h"
#include "m_type_conversion.h"
#include "m_exceptions.h"

namespace M {
    class Set;
    class Tuple;
    class MatrixBoolean;
    class MatrixInteger;
    class MatrixReal;
    class MatrixComplex;

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    inline M_PUBLIC_API int implicitOrdering(Boolean a, Boolean b) {
        return (a ? 1 : 0) - (b ? 1 : 0);
    }

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(Boolean a, Integer b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(Boolean a, Real b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(Boolean a, const Complex& b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(Integer a, Boolean b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(Integer a, Integer b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(Integer a, Real b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(Integer a, const Complex& b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(Real a, Boolean b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(Real a, Integer b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(Real a, Real b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(Real a, const Complex& b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(const Complex& a, Boolean b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(const Complex& a, Integer b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(const Complex& a, Real b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(const Complex& a, const Complex& b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(const Set& a, const Set& b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(const Tuple& a, const Tuple& b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(const MatrixBoolean& a, const MatrixBoolean& b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(const MatrixInteger& a, const MatrixInteger& b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(const MatrixReal& a, const MatrixReal& b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(const MatrixComplex& a, const MatrixComplex& b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(const Variant& a, Boolean b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(const Variant& a, Integer b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(const Variant& a, Real b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(const Variant& a, const Complex& b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(const Variant& a, const Set& b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(const Variant& a, const Tuple& b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(const Variant& a, const MatrixBoolean& b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(const Variant& a, const MatrixInteger& b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(const Variant& a, const MatrixReal& b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(const Variant& a, const MatrixComplex& b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(Boolean a, const Variant& b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(Integer a, const Variant& b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(Real a, const Variant& b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(const Complex& a, const Variant& b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(const Set& a, const Variant& b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(const Tuple& a, const Variant& b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(const MatrixBoolean& a, const Variant& b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(const MatrixInteger& a, const Variant& b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(const MatrixReal& a, const Variant& b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(const MatrixComplex& a, const Variant& b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    M_PUBLIC_API int implicitOrdering(const Variant& a, const Variant& b);

    /**
     * Function that determines the ordering of two arbitrary values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns a negative value if a should precede b, returns 0 if the values are equivalent, returns a
     *         positive positive value if a should follow b.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION int implicitOrdering(T1 a, T2 b) {
        if (valueTypeOf(a) == valueTypeOf(b)) {
            internalTriggerInvalidParameterValueError();
        }

        return static_cast<int>(valueTypeOf(a)) - static_cast<int>(valueTypeOf(b));
    }
}

#endif
