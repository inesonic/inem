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
* This header defines hyperbolic functions used by many or most models.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_HYPERBOLIC_FUNCTIONS_H
#define M_HYPERBOLIC_FUNCTIONS_H

#include "m_common.h"
#include "m_intrinsic_types.h"
#include "m_variant.h"

namespace M {
    /**
     * Function that returns the hyperbolic sine of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic sine of the angle.
     */
    M_PUBLIC_API Real hyperbolicSine(Real value);

    /**
     * Function that returns the hyperbolic sine of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic sine of the angle.
     */
    M_PUBLIC_API Complex hyperbolicSine(const Complex& value);

    /**
     * Function that returns the hyperbolic sine of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic sine of the angle.
     */
    M_PUBLIC_API inline Real hyperbolicSine(Boolean value) {
        return hyperbolicSine(Real(value ? 1 : 0));
    }

    /**
     * Function that returns the hyperbolic sine of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic sine of the angle.
     */
    M_PUBLIC_API inline Real hyperbolicSine(Integer value) {
        return hyperbolicSine(Real(value));
    }

    /**
     * Function that returns the hyperbolic sine of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic sine of the angle.
     */
    M_PUBLIC_API inline Complex hyperbolicSine(const Variant& value) {
        return hyperbolicSine(value.toComplex());
    }

    /**
     * Function that returns the hyperbolic cosine of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic cosine of the angle.
     */
    M_PUBLIC_API Real hyperbolicCosine(Real value);

    /**
     * Function that returns the hyperbolic cosine of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic cosine of the angle.
     */
    M_PUBLIC_API Complex hyperbolicCosine(const Complex& value);

    /**
     * Function that returns the hyperbolic cosine of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic cosine of the angle.
     */
    M_PUBLIC_API inline Real hyperbolicCosine(Boolean value) {
        return hyperbolicCosine(Real(value ? 1 : 0));
    }

    /**
     * Function that returns the hyperbolic cosine of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic cosine of the angle.
     */
    M_PUBLIC_API inline Real hyperbolicCosine(Integer value) {
        return hyperbolicCosine(Real(value));
    }

    /**
     * Function that returns the hyperbolic cosine of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic cosine of the angle.
     */
    M_PUBLIC_API inline Complex hyperbolicCosine(const Variant& value) {
        return hyperbolicCosine(value.toComplex());
    };

    /**
     * Function that returns the hyperbolic tangent of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic tangent of the angle.
     */
    M_PUBLIC_API Real hyperbolicTangent(Real value);

    /**
     * Function that returns the hyperbolic tangent of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic tangent of the angle.
     */
    M_PUBLIC_API Complex hyperbolicTangent(const Complex& value);

    /**
     * Function that returns the hyperbolic tangent of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic tangent of the angle.
     */
    M_PUBLIC_API inline Real hyperbolicTangent(Boolean value) {
        return hyperbolicTangent(Real(value ? 1 : 0));
    }

    /**
     * Function that returns the hyperbolic tangent of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic tangent of the angle.
     */
    M_PUBLIC_API inline Real hyperbolicTangent(Integer value) {
        return hyperbolicTangent(Real(value));
    }

    /**
     * Function that returns the hyperbolic tangent of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic tangent of the angle.
     */
    M_PUBLIC_API inline Complex hyperbolicTangent(const Variant& value) {
        return hyperbolicTangent(value.toComplex());
    }

    /**
     * Function that returns the hyperbolic secant of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic secant of the angle.
     */
    M_PUBLIC_API Real hyperbolicSecant(Real value);

    /**
     * Function that returns the hyperbolic secant of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic secant of the angle.
     */
    M_PUBLIC_API Complex hyperbolicSecant(const Complex& value);

    /**
     * Function that returns the hyperbolic secant of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic secant of the angle.
     */
    M_PUBLIC_API inline Real hyperbolicSecant(Boolean value) {
        return hyperbolicSecant(Real(value ? 1 : 0));
    }

    /**
     * Function that returns the hyperbolic secant of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic secant of the angle.
     */
    M_PUBLIC_API inline Real hyperbolicSecant(Integer value) {
        return hyperbolicSecant(Real(value));
    }

    /**
     * Function that returns the hyperbolic secant of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic secant of the angle.
     */
    M_PUBLIC_API inline Complex hyperbolicSecant(const Variant& value) {
        return hyperbolicSecant(value.toComplex());
    }

    /**
     * Function that returns the hyperbolic cosecant of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic cosecant of the angle.
     */
    M_PUBLIC_API Real hyperbolicCosecant(Real value);

    /**
     * Function that returns the hyperbolic cosecant of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic cosecant of the angle.
     */
    M_PUBLIC_API Complex hyperbolicCosecant(const Complex& value);

    /**
     * Function that returns the hyperbolic cosecant of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic cosecant of the angle.
     */
    M_PUBLIC_API inline Real hyperbolicCosecant(Boolean value) {
        return hyperbolicCosecant(Real(value ? 1 : 0));
    }

    /**
     * Function that returns the hyperbolic cosecant of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic cosecant of the angle.
     */
    M_PUBLIC_API inline Real hyperbolicCosecant(Integer value) {
        return hyperbolicCosecant(Real(value));
    }

    /**
     * Function that returns the hyperbolic cosecant of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic cosecant of the angle.
     */
    M_PUBLIC_API inline Complex hyperbolicCosecant(const Variant& value) {
        return hyperbolicCosecant(value.toComplex());
    }

    /**
     * Function that returns the hyperbolic cotangent of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic cotangent of the angle.
     */
    M_PUBLIC_API Real hyperbolicCotangent(Real value);

    /**
     * Function that returns the hyperbolic cotangent of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic cotangent of the angle.
     */
    M_PUBLIC_API Complex hyperbolicCotangent(const Complex& value);

    /**
     * Function that returns the hyperbolic cotangent of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic cotangent of the angle.
     */
    M_PUBLIC_API inline Real hyperbolicCotangent(Boolean value) {
        return hyperbolicCotangent(Real(value ? 1 : 0));
    }

    /**
     * Function that returns the hyperbolic cotangent of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic cotangent of the angle.
     */
    M_PUBLIC_API inline Real hyperbolicCotangent(Integer value) {
        return hyperbolicCotangent(Real(value));
    }

    /**
     * Function that returns the hyperbolic cotangent of a real value.
     *
     * \param[in] value The value to apply.
     *
     * \return Returns the hyperbolic cotangent of the angle.
     */
    M_PUBLIC_API inline Complex hyperbolicCotangent(const Variant& value) {
        return hyperbolicCotangent(value.toComplex());
    }

    /**
     * Function that returns the hyperbolic arsine of a real value.
     *
     * \param[in] value The value to determine the inverse of.
     *
     * \return returns the hyperbolic arsine of the value.
     */
    M_PUBLIC_API Real hyperbolicArsine(Real value);

    /**
     * Function that returns the hyperbolic arsine of a real value.
     *
     * \param[in] value The value to determine the inverse of.
     *
     * \return returns the hyperbolic arsine of the value.
     */
    M_PUBLIC_API Complex hyperbolicArsine(const Complex& value);

    /**
     * Function that returns the hyperbolic arsine of a real value.
     *
     * \param[in] value The value to determine the inverse of.
     *
     * \return returns the hyperbolic arsine of the value.
     */
    M_PUBLIC_API inline Real hyperbolicArsine(Boolean value) {
        return hyperbolicArsine(Real(value ? 1 : 0));
    }

    /**
     * Function that returns the hyperbolic arsine of a real value.
     *
     * \param[in] value The value to determine the inverse of.
     *
     * \return returns the hyperbolic arsine of the value.
     */
    M_PUBLIC_API inline Real hyperbolicArsine(Integer value) {
        return hyperbolicArsine(Real(value));
    }

    /**
     * Function that returns the hyperbolic arsine of a real value.
     *
     * \param[in] value The value to determine the inverse of.
     *
     * \return returns the hyperbolic arsine of the value.
     */
    M_PUBLIC_API inline Complex hyperbolicArsine(const Variant& value) {
        return hyperbolicArsine(value.toComplex());
    }

    /**
     * Function that returns the hyperbolic arosine of a real value.
     *
     * \param[in] value The value to determine the inverse of.
     *
     * \return returns the hyperbolic arcosine of the value.
     */
    M_PUBLIC_API Real hyperbolicArcosine(Real value);

    /**
     * Function that returns the hyperbolic arosine of a real value.
     *
     * \param[in] value The value to determine the inverse of.
     *
     * \return returns the hyperbolic arcosine of the value.
     */
    M_PUBLIC_API Complex hyperbolicArcosine(const Complex& value);

    /**
     * Function that returns the hyperbolic arosine of a real value.
     *
     * \param[in] value The value to determine the inverse of.
     *
     * \return returns the hyperbolic arcosine of the value.
     */
    M_PUBLIC_API inline Real hyperbolicArcosine(Boolean value) {
        return hyperbolicArcosine(Real(value ? 1 : 0));
    }

    /**
     * Function that returns the hyperbolic arosine of a real value.
     *
     * \param[in] value The value to determine the inverse of.
     *
     * \return returns the hyperbolic arcosine of the value.
     */
    M_PUBLIC_API inline Real hyperbolicArcosine(Integer value) {
        return hyperbolicArcosine(Real(value));
    }

    /**
     * Function that returns the hyperbolic arosine of a real value.
     *
     * \param[in] value The value to determine the inverse of.
     *
     * \return returns the hyperbolic arcosine of the value.
     */
    M_PUBLIC_API inline Complex hyperbolicArcosine(const Variant& value) {
        return hyperbolicArcosine(value.toComplex());
    }

    /**
     * Function that returns the hyperbolic artangent of a real value.
     *
     * \param[in] value The value to determine the inverse of.
     *
     * \return returns the hyperbolic artangent of the value.
     */
    M_PUBLIC_API Real hyperbolicArtangent(Real value);

    /**
     * Function that returns the hyperbolic artangent of a real value.
     *
     * \param[in] value The value to determine the inverse of.
     *
     * \return returns the hyperbolic artangent of the value.
     */
    M_PUBLIC_API Complex hyperbolicArtangent(const Complex& value);

    /**
     * Function that returns the hyperbolic artangent of a real value.
     *
     * \param[in] value The value to determine the inverse of.
     *
     * \return returns the hyperbolic artangent of the value.
     */
    M_PUBLIC_API inline Real hyperbolicArtangent(Boolean value) {
        return hyperbolicArtangent(Real(value ? 1 : 0));
    }

    /**
     * Function that returns the hyperbolic artangent of a real value.
     *
     * \param[in] value The value to determine the inverse of.
     *
     * \return returns the hyperbolic artangent of the value.
     */
    M_PUBLIC_API inline Real hyperbolicArtangent(Integer value) {
        return hyperbolicArtangent(Real(value));
    }

    /**
     * Function that returns the hyperbolic artangent of a real value.
     *
     * \param[in] value The value to determine the inverse of.
     *
     * \return returns the hyperbolic artangent of the value.
     */
    M_PUBLIC_API inline Complex hyperbolicArtangent(const Variant& value) {
        return hyperbolicArtangent(value.toComplex());
    }

    /**
     * Function that returns the hyperbolic arsecant of a real value.
     *
     * \param[in] value The value to determine the inverse of.
     *
     * \return returns the hyperbolic arsecant of the value.
     */
    M_PUBLIC_API Real hyperbolicArsecant(Real value);

    /**
     * Function that returns the hyperbolic arsecant of a real value.
     *
     * \param[in] value The value to determine the inverse of.
     *
     * \return returns the hyperbolic arsecant of the value.
     */
    M_PUBLIC_API Complex hyperbolicArsecant(const Complex& value);

    /**
     * Function that returns the hyperbolic arsecant of a real value.
     *
     * \param[in] value The value to determine the inverse of.
     *
     * \return returns the hyperbolic arsecant of the value.
     */
    M_PUBLIC_API inline Real hyperbolicArsecant(Boolean value) {
        return hyperbolicArsecant(Real(value ? 1 : 0));
    }

    /**
     * Function that returns the hyperbolic arsecant of a real value.
     *
     * \param[in] value The value to determine the inverse of.
     *
     * \return returns the hyperbolic arsecant of the value.
     */
    M_PUBLIC_API inline Real hyperbolicArsecant(Integer value) {
        return hyperbolicArsecant(Real(value));
    }

    /**
     * Function that returns the hyperbolic arsecant of a real value.
     *
     * \param[in] value The value to determine the inverse of.
     *
     * \return returns the hyperbolic arsecant of the value.
     */
    M_PUBLIC_API inline Complex hyperbolicArsecant(const Variant& value) {
        return hyperbolicArsecant(value.toComplex());
    }

    /**
     * Function that returns the hyperbolic arosecant of a real value.
     *
     * \param[in] value The value to determine the inverse of.
     *
     * \return returns the hyperbolic arcosecant of the value.
     */
    M_PUBLIC_API Real hyperbolicArcosecant(Real value);

    /**
     * Function that returns the hyperbolic arosecant of a real value.
     *
     * \param[in] value The value to determine the inverse of.
     *
     * \return returns the hyperbolic arcosecant of the value.
     */
    M_PUBLIC_API Complex hyperbolicArcosecant(const Complex& value);

    /**
     * Function that returns the hyperbolic arosecant of a real value.
     *
     * \param[in] value The value to determine the inverse of.
     *
     * \return returns the hyperbolic arcosecant of the value.
     */
    M_PUBLIC_API inline Real hyperbolicArcosecant(Boolean value) {
        return hyperbolicArcosecant(Real(value ? 1 : 0));
    }

    /**
     * Function that returns the hyperbolic arosecant of a real value.
     *
     * \param[in] value The value to determine the inverse of.
     *
     * \return returns the hyperbolic arcosecant of the value.
     */
    M_PUBLIC_API inline Real hyperbolicArcosecant(Integer value) {
        return hyperbolicArcosecant(Real(value));
    }

    /**
     * Function that returns the hyperbolic arosecant of a real value.
     *
     * \param[in] value The value to determine the inverse of.
     *
     * \return returns the hyperbolic arcosecant of the value.
     */
    M_PUBLIC_API inline Complex hyperbolicArcosecant(const Variant& value) {
        return hyperbolicArcosecant(value.toComplex());
    }

    /**
     * Function that returns the hyperbolic arotangent of a real value.
     *
     * \param[in] value The value to determine the inverse of.
     *
     * \return returns the hyperbolic arcotangent of the value.
     */
    M_PUBLIC_API Real hyperbolicArcotangent(Real value);

    /**
     * Function that returns the hyperbolic arotangent of a real value.
     *
     * \param[in] value The value to determine the value from.
     *
     * \return returns the hyperbolic arcotangent of the value.
     */
    M_PUBLIC_API Complex hyperbolicArcotangent(const Complex& value);

    /**
     * Function that returns the hyperbolic arotangent of a real value.
     *
     * \param[in] value The value to determine the inverse of.
     *
     * \return returns the hyperbolic arcotangent of the value.
     */
    M_PUBLIC_API inline Real hyperbolicArcotangent(Boolean value) {
        return hyperbolicArcotangent(Real(value ? 1 : 0));
    }

    /**
     * Function that returns the hyperbolic arotangent of a real value.
     *
     * \param[in] value The value to determine the inverse of.
     *
     * \return returns the hyperbolic arcotangent of the value.
     */
    M_PUBLIC_API inline Real hyperbolicArcotangent(Integer value) {
        return hyperbolicArcotangent(Real(value));
    }

    /**
     * Function that returns the hyperbolic arotangent of a real value.
     *
     * \param[in] value The value to determine the value from.
     *
     * \return returns the hyperbolic arcotangent of the value.
     */
    M_PUBLIC_API inline Complex hyperbolicArcotangent(const Variant& value) {
        return hyperbolicArcotangent(value.toComplex());
    }
}

#endif
