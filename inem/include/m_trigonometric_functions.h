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
* This header defines trigonometric functions used by many or most models.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_TRIGONOMETRIC_FUNCTIONS_H
#define M_TRIGONOMETRIC_FUNCTIONS_H

#include "m_common.h"
#include "m_intrinsic_types.h"
#include "m_variant.h"

namespace M {
    /**
     * Function that returns the sine of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the sine of the angle.
     */
    M_PUBLIC_API Real sine(Real angle);

    /**
     * Function that returns the sine of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the sine of the angle.
     */
    M_PUBLIC_API Complex sine(const Complex& angle);

    /**
     * Function that returns the sine of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the sine of the angle.
     */
    M_PUBLIC_API inline Real sine(Boolean angle) {
        return sine(Real(angle ? 1 : 0));
    }

    /**
     * Function that returns the sine of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the sine of the angle.
     */
    M_PUBLIC_API inline Real sine(Integer angle) {
        return sine(Real(angle));
    }

    /**
     * Function that returns the sine of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the sine of the angle.
     */
    M_PUBLIC_API inline Complex sine(const Variant& angle) {
        return sine(angle.toComplex());
    }

    /**
     * Function that returns the cosine of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the cosine of the angle.
     */
    M_PUBLIC_API Real cosine(Real angle);

    /**
     * Function that returns the cosine of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the cosine of the angle.
     */
    M_PUBLIC_API Complex cosine(const Complex& angle);

    /**
     * Function that returns the cosine of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the cosine of the angle.
     */
    M_PUBLIC_API inline Real cosine(Boolean angle) {
        return cosine(Real(angle ? 1 : 0));
    }

    /**
     * Function that returns the cosine of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the cosine of the angle.
     */
    M_PUBLIC_API inline Real cosine(Integer angle) {
        return cosine(Real(angle));
    }

    /**
     * Function that returns the cosine of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the cosine of the angle.
     */
    M_PUBLIC_API inline Complex cosine(const Variant& angle) {
        return cosine(angle.toComplex());
    }

    /**
     * Function that returns the tangent of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the tangent of the angle.
     */
    M_PUBLIC_API Real tangent(Real angle);

    /**
     * Function that returns the tangent of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the tangent of the angle.
     */
    M_PUBLIC_API Complex tangent(const Complex& angle);

    /**
     * Function that returns the tangent of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the tangent of the angle.
     */
    M_PUBLIC_API inline Real tangent(Boolean angle) {
        return tangent(Real(angle ? 1 : 0));
    }

    /**
     * Function that returns the tangent of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the tangent of the angle.
     */
    M_PUBLIC_API inline Real tangent(Integer angle) {
        return tangent(Real(angle));
    }

    /**
     * Function that returns the tangent of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the tangent of the angle.
     */
    M_PUBLIC_API inline Complex tangent(const Variant& angle) {
        return tangent(angle.toComplex());
    }

    /**
     * Function that returns the secant of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the secant of the angle.
     */
    M_PUBLIC_API Real secant(Real angle);

    /**
     * Function that returns the secant of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the secant of the angle.
     */
    M_PUBLIC_API Complex secant(const Complex& angle);

    /**
     * Function that returns the secant of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the secant of the angle.
     */
    M_PUBLIC_API inline Real secant(Boolean angle) {
        return secant(Real(angle ? 1 : 0));
    }

    /**
     * Function that returns the secant of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the secant of the angle.
     */
    M_PUBLIC_API inline Real secant(Integer angle) {
        return secant(Real(angle));
    }

    /**
     * Function that returns the secant of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the secant of the angle.
     */
    M_PUBLIC_API inline Complex secant(const Variant& angle) {
        return secant(angle.toComplex());
    }

    /**
     * Function that returns the cosecant of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the cosecant of the angle.
     */
    M_PUBLIC_API Real cosecant(Real angle);

    /**
     * Function that returns the cosecant of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the cosecant of the angle.
     */
    M_PUBLIC_API Complex cosecant(const Complex& angle);

    /**
     * Function that returns the cosecant of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the cosecant of the angle.
     */
    M_PUBLIC_API inline Real cosecant(Boolean angle) {
        return cosecant(Real(angle ? 1 : 0));
    }

    /**
     * Function that returns the cosecant of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the cosecant of the angle.
     */
    M_PUBLIC_API inline Real cosecant(Integer angle) {
        return cosecant(Real(angle));
    }

    /**
     * Function that returns the cosecant of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the cosecant of the angle.
     */
    M_PUBLIC_API inline Complex cosecant(const Variant& angle) {
        return cosecant(angle.toComplex());
    }

    /**
     * Function that returns the cotangent of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the cotangent of the angle.
     */
    M_PUBLIC_API Real cotangent(Real angle);

    /**
     * Function that returns the cotangent of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the cotangent of the angle.
     */
    M_PUBLIC_API Complex cotangent(const Complex& angle);

    /**
     * Function that returns the cotangent of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the cotangent of the angle.
     */
    M_PUBLIC_API inline Real cotangent(Boolean angle) {
        return cotangent(Real(angle ? 1 : 0));
    }

    /**
     * Function that returns the cotangent of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the cotangent of the angle.
     */
    M_PUBLIC_API inline Real cotangent(Integer angle) {
        return cotangent(Real(angle));
    }

    /**
     * Function that returns the cotangent of a real value.
     *
     * \param[in] angle The angle in radians.
     *
     * \return Returns the cotangent of the angle.
     */
    M_PUBLIC_API inline Complex cotangent(const Variant& angle) {
        return cotangent(angle.toComplex());
    }

    /**
     * Function that returns the arcsine of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arc-sine of the value.
     */
    M_PUBLIC_API Real arcsine(Real value);

    /**
     * Function that returns the arcsine of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arc-sine of the value.
     */
    M_PUBLIC_API Complex arcsine(const Complex& value);

    /**
     * Function that returns the arcsine of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arc-sine of the value.
     */
    M_PUBLIC_API inline Real arcsine(Boolean value) {
        return arcsine(Real(value ? 1 : 0));
    }

    /**
     * Function that returns the arcsine of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arc-sine of the value.
     */
    M_PUBLIC_API inline Real arcsine(Integer value) {
        return arcsine(Real(value));
    }

    /**
     * Function that returns the arcsine of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arc-sine of the value.
     */
    M_PUBLIC_API inline Complex arcsine(const Variant& value) {
        return arcsine(value.toComplex());
    }

    /**
     * Function that returns the arccosine of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arc-cosine of the value.
     */
    M_PUBLIC_API Real arccosine(Real value);

    /**
     * Function that returns the arccosine of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arc-cosine of the value.
     */
    M_PUBLIC_API Complex arccosine(const Complex& value);

    /**
     * Function that returns the arccosine of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arc-cosine of the value.
     */
    M_PUBLIC_API inline Real arccosine(Boolean value) {
        return arccosine(Real(value ? 1 : 0));
    }

    /**
     * Function that returns the arccosine of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arc-cosine of the value.
     */
    M_PUBLIC_API inline Real arccosine(Integer value) {
        return arccosine(Real(value));
    }

    /**
     * Function that returns the arccosine of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arc-cosine of the value.
     */
    M_PUBLIC_API inline Complex arccosine(const Variant& value) {
        return arccosine(value.toComplex());
    }

    /**
     * Function that returns the arctangent of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arctangent of the value.
     */
    M_PUBLIC_API Real arctangent(Real value);

    /**
     * Function that returns the arctangent of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arctangent of the value.
     */
    M_PUBLIC_API Complex arctangent(const Complex& value);

    /**
     * Function that returns the arctangent of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arctangent of the value.
     */
    M_PUBLIC_API inline Real arctangent(Boolean value) {
        return arctangent(Real(value ? 1 : 0));
    }

    /**
     * Function that returns the arctangent of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arctangent of the value.
     */
    M_PUBLIC_API inline Real arctangent(Integer value) {
        return arctangent(Real(value));
    }

    /**
     * Function that returns the arctangent of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arctangent of the value.
     */
    M_PUBLIC_API inline Complex arctangent(const Variant& value) {
        return arctangent(value.toComplex());
    }

    /**
     * Function that returns the arcsecant of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arcsecant of the value.
     */
    M_PUBLIC_API Real arcsecant(Real value);

    /**
     * Function that returns the arcsecant of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arcsecant of the value.
     */
    M_PUBLIC_API Complex arcsecant(const Complex& value);

    /**
     * Function that returns the arcsecant of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arcsecant of the value.
     */
    M_PUBLIC_API inline Real arcsecant(Boolean value) {
        return arcsecant(Real(value ? 1 : 0));
    }

    /**
     * Function that returns the arcsecant of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arcsecant of the value.
     */
    M_PUBLIC_API inline Real arcsecant(Integer value) {
        return arcsecant(Real(value));
    }

    /**
     * Function that returns the arcsecant of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arcsecant of the value.
     */
    M_PUBLIC_API inline Complex arcsecant(const Variant& value) {
        return arcsecant(value.toComplex());
    }

    /**
     * Function that returns the arccosecant of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arccosecant of the value.
     */
    M_PUBLIC_API Real arccosecant(Real value);

    /**
     * Function that returns the arccosecant of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arccosecant of the value.
     */
    M_PUBLIC_API Complex arccosecant(const Complex& value);

    /**
     * Function that returns the arccosecant of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arccosecant of the value.
     */
    M_PUBLIC_API inline Real arccosecant(Boolean value) {
        return arccosecant(Real(value ? 1 : 0));
    }

    /**
     * Function that returns the arccosecant of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arccosecant of the value.
     */
    M_PUBLIC_API inline Real arccosecant(Integer value) {
        return arccosecant(Real(value));
    }

    /**
     * Function that returns the arccosecant of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arccosecant of the value.
     */
    M_PUBLIC_API inline Complex arccosecant(const Variant& value) {
        return arccosecant(value.toComplex());
    }

    /**
     * Function that returns the arccotangent of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arccotangent of the value.
     */
    M_PUBLIC_API Real arccotangent(Real value);

    /**
     * Function that returns the arccotangent of a real value.
     *
     * \param[in] value The value to determine the value from.
     *
     * \return returns the arccotangent of the value.
     */
    M_PUBLIC_API Complex arccotangent(const Complex& value);

    /**
     * Function that returns the arccotangent of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arccotangent of the value.
     */
    M_PUBLIC_API inline Real arccotangent(Boolean value) {
        return arccotangent(Real(value ? 1 : 0));
    }

    /**
     * Function that returns the arccotangent of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arccotangent of the value.
     */
    M_PUBLIC_API inline Real arccotangent(Integer value) {
        return arccotangent(Real(value));
    }

    /**
     * Function that returns the arccotangent of a real value.
     *
     * \param[in] value The value to determine the angle from.
     *
     * \return returns the arccotangent of the value.
     */
    M_PUBLIC_API inline Complex arccotangent(const Variant& value) {
        return arccotangent(value.toComplex());
    }
}

#endif
