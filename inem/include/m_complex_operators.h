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

/* .. sphinx-project inem */

#ifndef M_COMPLEX_OPERATORS_H
#define M_COMPLEX_OPERATORS_H

#include "m_common.h"
#include "model_complex.h"
#include "m_intrinsic_types.h"
#include "m_exceptions.h"

namespace M {
    /**
     * Function that checks if a complex value is essentially real.
     *
     * \param[in] value The value to be tested.
     *
     * \return Returns true if the value should be considered real.  Returns false if the value is not real.
     */
    M_PUBLIC_API bool internalCheckIfReal(const Complex& value);
}

/**
 * Complex less-than operator.  This operator exists to allow comparisons of complex values when holding only
 * real data.
 *
 * \param[in] a The first value to compare.
 *
 * \param[in] b The second value to compare.
 *
 * \return Returns true if the condition tests true, otherwise this operator returns false.
 */
M_PUBLIC_API bool operator<(const M::Complex& a, M::Boolean b);

/**
 * Complex greater-than operator.  This operator exists to allow comparisons of complex values when holding only
 * real data.
 *
 * \param[in] a The first value to compare.
 *
 * \param[in] b The second value to compare.
 *
 * \return Returns true if the condition tests true, otherwise this operator returns false.
 */
M_PUBLIC_API bool operator>(const M::Complex& a, M::Boolean b);

/**
 * Complex less-than operator.  This operator exists to allow comparisons of complex values when holding only
 * real data.
 *
 * \param[in] a The first value to compare.
 *
 * \param[in] b The second value to compare.
 *
 * \return Returns true if the condition tests true, otherwise this operator returns false.
 */
M_PUBLIC_API bool operator<(const M::Complex& a, M::Integer b);

/**
 * Complex greater-than operator.  This operator exists to allow comparisons of complex values when holding only
 * real data.
 *
 * \param[in] a The first value to compare.
 *
 * \param[in] b The second value to compare.
 *
 * \return Returns true if the condition tests true, otherwise this operator returns false.
 */
M_PUBLIC_API bool operator>(const M::Complex& a, M::Integer b);

/**
 * Complex less-than operator.  This operator exists to allow comparisons of complex values when holding only
 * real data.
 *
 * \param[in] a The first value to compare.
 *
 * \param[in] b The second value to compare.
 *
 * \return Returns true if the condition tests true, otherwise this operator returns false.
 */
M_PUBLIC_API bool operator<(const M::Complex& a, M::Real b);

/**
 * Complex greater-than operator.  This operator exists to allow comparisons of complex values when holding only
 * real data.
 *
 * \param[in] a The first value to compare.
 *
 * \param[in] b The second value to compare.
 *
 * \return Returns true if the condition tests true, otherwise this operator returns false.
 */
M_PUBLIC_API bool operator>(const M::Complex& a, M::Real b);

/**
 * Complex less-than operator.  This operator exists to allow comparisons of complex values when holding only
 * real data.
 *
 * \param[in] a The first value to compare.
 *
 * \param[in] b The second value to compare.
 *
 * \return Returns true if the condition tests true, otherwise this operator returns false.
 */
M_PUBLIC_API bool operator<(M::Boolean a, const M::Complex& b);

/**
 * Complex greater-than operator.  This operator exists to allow comparisons of complex values when holding only
 * real data.
 *
 * \param[in] a The first value to compare.
 *
 * \param[in] b The second value to compare.
 *
 * \return Returns true if the condition tests true, otherwise this operator returns false.
 */
M_PUBLIC_API bool operator>(M::Boolean a, const M::Complex& b);

/**
 * Complex less-than operator.  This operator exists to allow comparisons of complex values when holding only
 * real data.
 *
 * \param[in] a The first value to compare.
 *
 * \param[in] b The second value to compare.
 *
 * \return Returns true if the condition tests true, otherwise this operator returns false.
 */
M_PUBLIC_API bool operator<(M::Integer a, const M::Complex& b);

/**
 * Complex greater-than operator.  This operator exists to allow comparisons of complex values when holding only
 * real data.
 *
 * \param[in] a The first value to compare.
 *
 * \param[in] b The second value to compare.
 *
 * \return Returns true if the condition tests true, otherwise this operator returns false.
 */
M_PUBLIC_API bool operator>(M::Integer a, const M::Complex& b);

/**
 * Complex less-than operator.  This operator exists to allow comparisons of complex values when holding only
 * real data.
 *
 * \param[in] a The first value to compare.
 *
 * \param[in] b The second value to compare.
 *
 * \return Returns true if the condition tests true, otherwise this operator returns false.
 */
M_PUBLIC_API bool operator<(M::Real a, const M::Complex& b);

/**
 * Complex greater-than operator.  This operator exists to allow comparisons of complex values when holding only
 * real data.
 *
 * \param[in] a The first value to compare.
 *
 * \param[in] b The second value to compare.
 *
 * \return Returns true if the condition tests true, otherwise this operator returns false.
 */
M_PUBLIC_API bool operator>(M::Real a, const M::Complex& b);

/**
 * Complex less-than operator.  This operator exists to allow comparisons of complex values when holding only
 * real data.
 *
 * \param[in] a The first value to compare.
 *
 * \param[in] b The second value to compare.
 *
 * \return Returns true if the condition tests true, otherwise this operator returns false.
 */
M_PUBLIC_API bool operator<(const M::Complex& a, const M::Complex& b);

/**
 * Complex greater-than operator.  This operator exists to allow comparisons of complex values when holding only
 * real data.
 *
 * \param[in] a The first value to compare.
 *
 * \param[in] b The second value to compare.
 *
 * \return Returns true if the condition tests true, otherwise this operator returns false.
 */
M_PUBLIC_API bool operator>(const M::Complex& a, const M::Complex& b);

/**
 * Complex less-than-or-equal-to operator.  This operator exists to allow comparisons of complex values when
 * holding only real data.
 *
 * \param[in] a The first value to compare.
 *
 * \param[in] b The second value to compare.
 *
 * \return Returns true if the condition tests true, otherwise this operator returns false.
 */
inline bool operator<=(const M::Complex& a, const M::Complex& b) {
    return !operator>(a, b);
}

/**
 * Complex greater-than-or-equal-to operator.  This operator exists to allow comparisons of complex values when
 * holding only real data.
 *
 * \param[in] a The first value to compare.
 *
 * \param[in] b The second value to compare.
 *
 * \return Returns true if the condition tests true, otherwise this operator returns false.
 */
inline bool operator>=(const M::Complex& a, const M::Complex& b) {
    return !operator<(a, b);
}

/**
 * Complex less-than-or operator.  This operator exists to allow comparisons of complex values when holding only
 * real data.
 *
 * \param[in] a The first value to compare.
 *
 * \param[in] b The second value to compare.
 *
 * \return Returns true if the condition tests true, otherwise this operator returns false.
 */
M_PUBLIC_API inline bool operator<=(const M::Complex& a, M::Boolean b) {
    return !operator>(a, b);
}

/**
 * Complex less-than-or operator.  This operator exists to allow comparisons of complex values when holding only
 * real data.
 *
 * \param[in] a The first value to compare.
 *
 * \param[in] b The second value to compare.
 *
 * \return Returns true if the condition tests true, otherwise this operator returns false.
 */
M_PUBLIC_API inline bool operator<=(const M::Complex& a, M::Integer b) {
    return !operator>(a, b);
}

/**
 * Complex less-than-or operator.  This operator exists to allow comparisons of complex values when holding only
 * real data.
 *
 * \param[in] a The first value to compare.
 *
 * \param[in] b The second value to compare.
 *
 * \return Returns true if the condition tests true, otherwise this operator returns false.
 */
M_PUBLIC_API inline bool operator<=(const M::Complex& a, M::Real b) {
    return !operator>(a, b);
}

/**
 * Complex greater-than-or operator.  This operator exists to allow comparisons of complex values when holding only
 * real data.
 *
 * \param[in] a The first value to compare.
 *
 * \param[in] b The second value to compare.
 *
 * \return Returns true if the condition tests true, otherwise this operator returns false.
 */
M_PUBLIC_API inline bool operator>=(const M::Complex& a, M::Boolean b) {
    return !operator<(a, b);
}

/**
 * Complex greater-than-or operator.  This operator exists to allow comparisons of complex values when holding only
 * real data.
 *
 * \param[in] a The first value to compare.
 *
 * \param[in] b The second value to compare.
 *
 * \return Returns true if the condition tests true, otherwise this operator returns false.
 */
M_PUBLIC_API inline bool operator>=(const M::Complex& a, M::Integer b) {
    return !operator<(a, b);
}

/**
 * Complex greater-than-or operator.  This operator exists to allow comparisons of complex values when holding only
 * real data.
 *
 * \param[in] a The first value to compare.
 *
 * \param[in] b The second value to compare.
 *
 * \return Returns true if the condition tests true, otherwise this operator returns false.
 */
M_PUBLIC_API inline bool operator>=(const M::Complex& a, M::Real b) {
    return !operator<(a, b);
}

/**
 * Complex less-than-or operator.  This operator exists to allow comparisons of complex values when holding only
 * real data.
 *
 * \param[in] a The first value to compare.
 *
 * \param[in] b The second value to compare.
 *
 * \return Returns true if the condition tests true, otherwise this operator returns false.
 */
M_PUBLIC_API inline bool operator<=(M::Boolean a, const M::Complex& b) {
    return !operator>(a, b);
}

/**
 * Complex less-than-or operator.  This operator exists to allow comparisons of complex values when holding only
 * real data.
 *
 * \param[in] a The first value to compare.
 *
 * \param[in] b The second value to compare.
 *
 * \return Returns true if the condition tests true, otherwise this operator returns false.
 */
M_PUBLIC_API inline bool operator<=(M::Integer a, const M::Complex& b) {
    return !operator>(a, b);
}

/**
 * Complex less-than-or operator.  This operator exists to allow comparisons of complex values when holding only
 * real data.
 *
 * \param[in] a The first value to compare.
 *
 * \param[in] b The second value to compare.
 *
 * \return Returns true if the condition tests true, otherwise this operator returns false.
 */
M_PUBLIC_API inline bool operator<=(M::Real a, const M::Complex& b) {
    return !operator>(a, b);
}

/**
 * Complex greater-than-or operator.  This operator exists to allow comparisons of complex values when holding only
 * real data.
 *
 * \param[in] a The first value to compare.
 *
 * \param[in] b The second value to compare.
 *
 * \return Returns true if the condition tests true, otherwise this operator returns false.
 */
M_PUBLIC_API inline bool operator>=(M::Boolean a, const M::Complex& b) {
    return !operator<(a, b);
}

/**
 * Complex greater-than-or operator.  This operator exists to allow comparisons of complex values when holding only
 * real data.
 *
 * \param[in] a The first value to compare.
 *
 * \param[in] b The second value to compare.
 *
 * \return Returns true if the condition tests true, otherwise this operator returns false.
 */
M_PUBLIC_API inline bool operator>=(M::Integer a, const M::Complex& b) {
    return !operator<(a, b);
}

/**
 * Complex greater-than-or operator.  This operator exists to allow comparisons of complex values when holding only
 * real data.
 *
 * \param[in] a The first value to compare.
 *
 * \param[in] b The second value to compare.
 *
 * \return Returns true if the condition tests true, otherwise this operator returns false.
 */
M_PUBLIC_API inline bool operator>=(M::Real a, const M::Complex& b) {
    return !operator<(a, b);
}

/**
 * Complex logical AND operator.
 *
 * \param[in] a The first value to AND.
 *
 * \param[in] b The second value to AND.
 *
 * \return Returns true if both a and b evaluate to true.  Returns false in all other cases.
 */
M_PUBLIC_API inline bool operator&&(const M::Complex& a, M::Boolean b) {
    return (a != 0) && b;
}

/**
 * Complex logical AND operator.
 *
 * \param[in] a The first value to AND.
 *
 * \param[in] b The second value to AND.
 *
 * \return Returns true if both a and b evaluate to true.  Returns false in all other cases.
 */
M_PUBLIC_API inline bool operator&&(const M::Complex& a, M::Integer b) {
    return (a != 0) && (b != 0);
}

/**
 * Complex logical AND operator.
 *
 * \param[in] a The first value to AND.
 *
 * \param[in] b The second value to AND.
 *
 * \return Returns true if both a and b evaluate to true.  Returns false in all other cases.
 */
M_PUBLIC_API inline bool operator&&(const M::Complex& a, M::Real b) {
    return (a != 0) && (b != 0);
}

/**
 * Complex logical AND operator.
 *
 * \param[in] a The first value to AND.
 *
 * \param[in] b The second value to AND.
 *
 * \return Returns true if both a and b evaluate to true.  Returns false in all other cases.
 */
M_PUBLIC_API inline bool operator&&(M::Boolean a, const M::Complex& b) {
    return a && (b != 0);
}

/**
 * Complex logical AND operator.
 *
 * \param[in] a The first value to AND.
 *
 * \param[in] b The second value to AND.
 *
 * \return Returns true if both a and b evaluate to true.  Returns false in all other cases.
 */
M_PUBLIC_API inline bool operator&&(M::Integer a, const M::Complex& b) {
    return (a != 0) && (b != 0);
}

/**
 * Complex logical AND operator.
 *
 * \param[in] a The first value to AND.
 *
 * \param[in] b The second value to AND.
 *
 * \return Returns true if both a and b evaluate to true.  Returns false in all other cases.
 */
M_PUBLIC_API inline bool operator&&(M::Real a, const M::Complex& b) {
    return (a != 0) && (b != 0);
}

/**
 * Complex logical AND operator.
 *
 * \param[in] a The first value to AND.
 *
 * \param[in] b The second value to AND.
 *
 * \return Returns true if both a and b evaluate to true.  Returns false in all other cases.
 */
M_PUBLIC_API inline bool operator&&(const M::Complex& a, const M::Complex& b) {
    return (a != 0) && (b != 0);
}

/**
 * Complex logical OR operator.
 *
 * \param[in] a The first value to OR.
 *
 * \param[in] b The second value to OR.
 *
 * \return Returns true if either a or b are true.  Returns false if both a and b are false.
 */
M_PUBLIC_API inline bool operator||(const M::Complex& a, M::Boolean b) {
    return (a != 0) || b;
}

/**
 * Complex logical OR operator.
 *
 * \param[in] a The first value to OR.
 *
 * \param[in] b The second value to OR.
 *
 * \return Returns true if either a or b are true.  Returns false if both a and b are false.
 */
M_PUBLIC_API inline bool operator||(const M::Complex& a, M::Integer b) {
    return (a != 0) || (b != 0);
}

/**
 * Complex logical OR operator.
 *
 * \param[in] a The first value to OR.
 *
 * \param[in] b The second value to OR.
 *
 * \return Returns true if either a or b are true.  Returns false if both a and b are false.
 */
M_PUBLIC_API inline bool operator||(const M::Complex& a, M::Real b) {
    return (a != 0) || (b != 0);
}

/**
 * Complex logical OR operator.
 *
 * \param[in] a The first value to OR.
 *
 * \param[in] b The second value to OR.
 *
 * \return Returns true if either a or b are true.  Returns false if both a and b are false.
 */
M_PUBLIC_API inline bool operator||(M::Boolean a, const M::Complex& b) {
    return a || (b != 0);
}

/**
 * Complex logical OR operator.
 *
 * \param[in] a The first value to OR.
 *
 * \param[in] b The second value to OR.
 *
 * \return Returns true if either a or b are true.  Returns false if both a and b are false.
 */
M_PUBLIC_API inline bool operator||(M::Integer a, const M::Complex& b) {
    return (a != 0) || (b != 0);
}

/**
 * Complex logical OR operator.
 *
 * \param[in] a The first value to OR.
 *
 * \param[in] b The second value to OR.
 *
 * \return Returns true if either a or b are true.  Returns false if both a and b are false.
 */
M_PUBLIC_API inline bool operator||(M::Real a, const M::Complex& b) {
    return (a != 0) || (b != 0);
}

/**
 * Complex logical OR operator.
 *
 * \param[in] a The first value to OR.
 *
 * \param[in] b The second value to OR.
 *
 * \return Returns true if either a or b are true.  Returns false if both a and b are false.
 */
M_PUBLIC_API inline bool operator||(const M::Complex& a, const M::Complex& b) {
    return (a != 0) || (b != 0);
}

/**
 * Complex logical not operator.
 *
 * \param[in] value The value to calculate the logical "not" of.
 *
 * \return Returns true if the value resolves to false.  Returns false if the value resolves to true.
 */
M_PUBLIC_API inline bool operator!(const M::Complex& value) {
    return value == 0;
}

#endif
