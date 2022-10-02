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
* This header defines basic functions used by many or most models.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_BASIC_FUNCTIONS_H
#define M_BASIC_FUNCTIONS_H

#include "m_common.h"
#include "m_type_conversion.h"
#include "m_exceptions.h"
#include "m_special_values.h"
#include "m_intrinsic_types.h"
#include "m_variant.h"

namespace M {
    class PerThread;

    /**
     * The default tolerance to apply to the Lambert W function.
     */
    M_PUBLIC_API extern const Real defaultLambertWEpsilon;

    /**
     * Function that is called when an invalid range is provided.
     */
    M_PUBLIC_API NO_RETURN void internalReportInvalidRange();

    /**
     * Template function that is called to validate range parameters at run-time.
     *
     * \param[in] startingValue The starting value for the range.
     *
     * \param[in] endingValue   The ending value for the range.
     */
    template<typename S, typename E> M_PUBLIC_TEMPLATE_FUNCTION void internalValidateRange(
            S startingValue,
            E endingValue
        ) {
        if (startingValue > endingValue) {
            internalReportInvalidRange();
        }
    }

    /**
     * Template function that is called to validate range parameters at run-time.
     *
     * \param[in] firstValue  The first range value.
     *
     * \param[in] secondValue The second range value.
     *
     * \param[in] endingValue The ending value for the range.
     */
    template<typename F, typename S, typename E> M_PUBLIC_TEMPLATE_FUNCTION void internalValidateRange(
            F firstValue,
            S secondValue,
            E endingValue
        ) {
        if (!(firstValue < secondValue && secondValue <= endingValue) &&
            !(firstValue > secondValue && secondValue >= endingValue)    ) {
            internalReportInvalidRange();
        }
    }

    /**
     * Function that determines the system elapsed time since the beginning of the Unix Epoch.
     *
     * \return Returns a 64-bit integer holding the system elapsed time in mSec.
     */
    M_PUBLIC_API Integer systemTime();

    /**
     * Function that determines the system elapsed time since the beginning of the Unix Epoch.
     *
     * \param[in] dummy A dummy parameter.  The value is ignored.
     *
     * \return Returns a 64-bit integer holding the system elapsed time in mSec.
     */
    template<typename T> M_PUBLIC_TEMPLATE_FUNCTION Integer systemTime(T dummy) {
        (void) dummy;
        return systemTime();
    }

    /**
     * Function that determines if a value is NaN.
     *
     * \param[in] value The value to be tested.
     *
     * \return Returns true if the value is NaN.  Returns false if the value is not NaN.
     */
    M_PUBLIC_API inline Boolean isNaN(Boolean value) {
        (void) value;
        return false;
    }

    /**
     * Function that determines if a value is NaN.
     *
     * \param[in] value The value to be tested.
     *
     * \return Returns true if the value is NaN.  Returns false if the value is not NaN.
     */
    M_PUBLIC_API inline Boolean isNaN(Integer value) {
        (void) value;
        return false;
    }

    /**
     * Function that determines if a value is NaN.
     *
     * \param[in] value The value to be tested.
     *
     * \return Returns true if the value is NaN.  Returns false if the value is not NaN.
     */
    M_PUBLIC_API Boolean isNaN(Real value);

    /**
     * Function that determines if a value is NaN.
     *
     * \param[in] value The value to be tested.
     *
     * \return Returns true if the value is NaN.  Returns false if the value is not NaN.
     */
    M_PUBLIC_API Boolean isNaN(const Complex& value);

    /**
     * Function that determines if a value is NaN.
     *
     * \param[in] value The value to be tested.
     *
     * \return Returns true if the value is NaN.  Returns false if the value is not NaN.
     */
    M_PUBLIC_API Boolean isNaN(const Variant& value);

    /**
     * Function that raises a value to a power.
     *
     * \param[in] base     The base value.
     *
     * \param[in] exponent The exponent to raise the base to.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Integer pow(Integer base, Integer exponent);

    /**
     * Function that raises a value to a power.
     *
     * \param[in] base     The base value.
     *
     * \param[in] exponent The exponent to raise the base to.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Real pow(Real base, Integer exponent);

    /**
     * Function that raises a value to a power.
     *
     * \param[in] base     The base value.
     *
     * \param[in] exponent The exponent to raise the base to.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex pow(Integer base, Real exponent);

    /**
     * Function that raises a value to a power.
     *
     * \param[in] base     The base value.
     *
     * \param[in] exponent The exponent to raise the base to.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex pow(Real base, Real exponent);

    /**
     * Function that raises a value to a power.
     *
     * \param[in] base     The base value.
     *
     * \param[in] exponent The exponent to raise the base to.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex pow(const Complex& base, Integer exponent);

    /**
     * Function that raises a value to a power.
     *
     * \param[in] base     The base value.
     *
     * \param[in] exponent The exponent to raise the base to.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex pow(Integer base, const Complex& exponent);

    /**
     * Function that raises a value to a power.
     *
     * \param[in] base     The base value.
     *
     * \param[in] exponent The exponent to raise the base to.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex pow(const Complex& base, Real exponent);

    /**
     * Function that raises a value to a power.
     *
     * \param[in] base     The base value.
     *
     * \param[in] exponent The exponent to raise the base to.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex pow(Real base, const Complex& exponent);

    /**
     * Function that raises a value to a power.
     *
     * \param[in] base     The base value.
     *
     * \param[in] exponent The exponent to raise the base to.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex pow(const Complex& base, const Complex& exponent);

    /**
     * Function that raises a value to a power.
     *
     * \param[in] base     The base value.
     *
     * \param[in] exponent The exponent to raise the base to.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex pow(const Variant& base, Integer exponent);

    /**
     * Function that raises a value to a power.
     *
     * \param[in] base     The base value.
     *
     * \param[in] exponent The exponent to raise the base to.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex pow(const Variant& base, Real exponent);

    /**
     * Function that raises a value to a power.
     *
     * \param[in] base     The base value.
     *
     * \param[in] exponent The exponent to raise the base to.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex pow(const Variant& base, const Complex& exponent);

    /**
     * Function that raises a value to a power.
     *
     * \param[in] base     The base value.
     *
     * \param[in] exponent The exponent to raise the base to.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex pow(Integer base, const Variant& exponent);

    /**
     * Function that raises a value to a power.
     *
     * \param[in] base     The base value.
     *
     * \param[in] exponent The exponent to raise the base to.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex pow(Real base, const Variant& exponent);

    /**
     * Function that raises a value to a power.
     *
     * \param[in] base     The base value.
     *
     * \param[in] exponent The exponent to raise the base to.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex pow(const Complex& base, const Variant& exponent);

    /**
     * Function that raises a value to a power.
     *
     * \param[in] base     The base value.
     *
     * \param[in] exponent The exponent to raise the base to.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex pow(const Variant& base, const Variant& exponent);

    /**
     * Function that raises a value to a power.
     *
     * \param[in] base     The base value.
     *
     * \param[in] exponent The exponent to raise the base to.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API inline Integer pow(Boolean base, Boolean exponent) {
        return pow(M::Integer(base ? 1 : 0), M::Integer(exponent ? 1 : 0));
    }

    /**
     * Function that raises a value to a power.
     *
     * \param[in] base     The base value.
     *
     * \param[in] exponent The exponent to raise the base to.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API inline Integer pow(Boolean base, Integer exponent) {
        return pow(M::Integer(base ? 1 : 0), exponent);
    }

    /**
     * Function that raises a value to a power.
     *
     * \param[in] base     The base value.
     *
     * \param[in] exponent The exponent to raise the base to.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API inline Complex pow(Boolean base, Real exponent) {
        return pow(M::Integer(base ? 1 : 0), exponent);
    }

    /**
     * Function that raises a value to a power.
     *
     * \param[in] base     The base value.
     *
     * \param[in] exponent The exponent to raise the base to.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API inline Complex pow(Boolean base, const Complex& exponent) {
        return pow(M::Integer(base ? 1 : 0), exponent);
    }

    /**
     * Function that raises a value to a power.
     *
     * \param[in] base     The base value.
     *
     * \param[in] exponent The exponent to raise the base to.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API inline Complex pow(Boolean base, const Variant& exponent) {
        return pow(M::Integer(base ? 1 : 0), exponent);
    }

    /**
     * Function that raises a value to a power.
     *
     * \param[in] base     The base value.
     *
     * \param[in] exponent The exponent to raise the base to.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API inline Integer pow(Integer base, Boolean exponent) {
        return pow(base, M::Integer(exponent ? 1 : 0));
    }

    /**
     * Function that raises a value to a power.
     *
     * \param[in] base     The base value.
     *
     * \param[in] exponent The exponent to raise the base to.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API inline Real pow(Real base, Boolean exponent) {
        return pow(base, M::Integer(exponent ? 1 : 0));
    }

    /**
     * Function that raises a value to a power.
     *
     * \param[in] base     The base value.
     *
     * \param[in] exponent The exponent to raise the base to.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API inline Complex pow(const Complex& base, Boolean exponent) {
        return pow(base, M::Integer(exponent ? 1 : 0));
    }

    /**
     * Function that raises a value to a power.
     *
     * \param[in] base     The base value.
     *
     * \param[in] exponent The exponent to raise the base to.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API inline Complex pow(const Variant& base, Boolean exponent) {
        return pow(base, M::Integer(exponent ? 1 : 0));
    }

    /**
     * Function that calculates the square root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex sqrt(Integer value);

    /**
     * Function that calculates the square root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex sqrt(Real value);

    /**
     * Function that calculates the square root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex sqrt(const Complex& value);

    /**
     * Function that calculates the square root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex sqrt(const Variant& value);

    /**
     * Function that calculates the square root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API inline Complex sqrt(Boolean value) {
        return sqrt(Integer(value ? 1 : 0));
    }

    /**
     * Function that calculates an arbitrary nth root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \param[in] root  The root to calculate the value at.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex nroot(Integer value, Integer root);

    /**
     * Function that calculates an arbitrary nth root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \param[in] root  The root to calculate the value at.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex nroot(Integer value, Real root);

    /**
     * Function that calculates an arbitrary nth root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \param[in] root  The root to calculate the value at.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex nroot(Integer value, const Complex& root);

    /**
     * Function that calculates an arbitrary nth root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \param[in] root  The root to calculate the value at.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex nroot(Real value, Integer root);

    /**
     * Function that calculates an arbitrary nth root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \param[in] root  The root to calculate the value at.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex nroot(Real value, Real root);

    /**
     * Function that calculates an arbitrary nth root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \param[in] root  The root to calculate the value at.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex nroot(Real value, const Complex& root);

    /**
     * Function that calculates an arbitrary nth root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \param[in] root  The root to calculate the value at.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex nroot(const Complex& value, Integer root);

    /**
     * Function that calculates an arbitrary nth root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \param[in] root  The root to calculate the value at.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex nroot(const Complex& value, Real root);

    /**
     * Function that calculates an arbitrary nth root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \param[in] root  The root to calculate the value at.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex nroot(const Complex& value, const Complex& root);

    /**
     * Function that calculates an arbitrary nth root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \param[in] root  The root to calculate the value at.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex nroot(const Variant& value, Integer root);

    /**
     * Function that calculates an arbitrary nth root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \param[in] root  The root to calculate the value at.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex nroot(const Variant& value, Real root);

    /**
     * Function that calculates an arbitrary nth root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \param[in] root  The root to calculate the value at.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex nroot(const Variant& value, const Complex& root);

    /**
     * Function that calculates an arbitrary nth root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \param[in] root  The root to calculate the value at.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex nroot(Integer value, const Variant& root);

    /**
     * Function that calculates an arbitrary nth root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \param[in] root  The root to calculate the value at.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex nroot(Real value, const Variant& root);

    /**
     * Function that calculates an arbitrary nth root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \param[in] root  The root to calculate the value at.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex nroot(const Complex& value, const Variant& root);

    /**
     * Function that calculates an arbitrary nth root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \param[in] root  The root to calculate the value at.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API Complex nroot(const Variant& value, const Variant& root);

    /**
     * Function that calculates an arbitrary nth root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \param[in] root  The root to calculate the value at.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API inline Complex nroot(Boolean value, Boolean root) {
        return nroot(M::Integer(value ? 1 : 0), M::Integer(root ? 1 : 0));
    }

    /**
     * Function that calculates an arbitrary nth root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \param[in] root  The root to calculate the value at.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API inline Complex nroot(Boolean value, Integer root) {
        return nroot(M::Integer(value ? 1 : 0), root);
    }

    /**
     * Function that calculates an arbitrary nth root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \param[in] root  The root to calculate the value at.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API inline Complex nroot(Boolean value, Real root) {
        return nroot(M::Integer(value ? 1 : 0), root);
    }

    /**
     * Function that calculates an arbitrary nth root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \param[in] root  The root to calculate the value at.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API inline Complex nroot(Boolean value, const Complex& root) {
        return nroot(M::Integer(value ? 1 : 0), root);
    }

    /**
     * Function that calculates an arbitrary nth root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \param[in] root  The root to calculate the value at.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API inline Complex nroot(Boolean value, const Variant& root) {
        return nroot(M::Integer(value ? 1 : 0), root);
    }

    /**
     * Function that calculates an arbitrary nth root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \param[in] root  The root to calculate the value at.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API inline Complex nroot(Integer value, Boolean root) {
        return nroot(value, M::Integer(root ? 1 : 0));
    }

    /**
     * Function that calculates an arbitrary nth root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \param[in] root  The root to calculate the value at.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API inline Complex nroot(Real value, Boolean root) {
        return nroot(value, M::Integer(root ? 1 : 0));
    }

    /**
     * Function that calculates an arbitrary nth root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \param[in] root  The root to calculate the value at.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API inline Complex nroot(const Complex& value, Boolean root) {
        return nroot(value, M::Integer(root ? 1 : 0));
    }

    /**
     * Function that calculates an arbitrary nth root of a value.
     *
     * \param[in] value The value to calculate the square root of.
     *
     * \param[in] root  The root to calculate the value at.
     *
     * \return Returns the base raised to the appropriate exponent.
     */
    M_PUBLIC_API inline Complex nroot(const Variant& value, Boolean root) {
        return nroot(value, M::Integer(root ? 1 : 0));
    }

    /**
     * Function that calculates the complex conjugate of a value.
     *
     * \param[in] value The value to calculate the complex conjugate of.
     *
     * \return Returns the complex conjugate.
     */
    M_PUBLIC_API inline Integer conj(Boolean value) {
        return value ? 1 : 0;
    }

    /**
     * Function that calculates the complex conjugate of a value.
     *
     * \param[in] value The value to calculate the complex conjugate of.
     *
     * \return Returns the complex conjugate.
     */
    M_PUBLIC_API inline Integer conj(Integer value) {
        return value;
    }

    /**
     * Function that calculates the complex conjugate of a value.
     *
     * \param[in] value The value to calculate the complex conjugate of.
     *
     * \return Returns the complex conjugate.
     */
    M_PUBLIC_API inline Real conj(Real value) {
        return value;
    }

    /**
     * Function that calculates the complex conjugate of a value.
     *
     * \param[in] value The value to calculate the complex conjugate of.
     *
     * \return Returns the complex conjugate.
     */
    M_PUBLIC_API inline Complex conj(const Complex& value) {
        return value.conj();
    }

    /**
     * Function that calculates the absolute value of a value.
     *
     * \param[in] value The value to determine the absolute value of.
     *
     * \return Returns the absolute value.
     */
    M_PUBLIC_API inline Integer abs(Boolean value) {
        return value ? 1 : 0;
    }

    /**
     * Function that calculates the absolute value of a value.
     *
     * \param[in] value The value to determine the absolute value of.
     *
     * \return Returns the absolute value.
     */
    M_PUBLIC_API inline Integer abs(Integer value) {
        return value < 0 ? -value : value;
    }

    /**
     * Function that calculates the absolute value of a value.
     *
     * \param[in] value The value to calculate the absolute value of.
     *
     * \return Returns the absolute value.
     */
    M_PUBLIC_API inline Real abs(Real value) {
        return value < 0 ? -value : value;
    }

    /**
     * Function that calculates the magnitude of a complex value.
     *
     * \param[in] value The value to calculate the magnitude of.
     *
     * \return Returns the magnitude.
     */
    M_PUBLIC_API Real abs(const Complex& value);

    /**
     * Function that calculates the floor of a value.
     *
     * \param[in] value The value to calculate the floor of.
     *
     * \return Returns the floor of the value.
     */
    M_PUBLIC_API inline Integer floor(Integer value) {
        return value;
    }

    /**
     * Function that calculates the floor of a value.
     *
     * \param[in] value The value to calculate the floor of.
     *
     * \return Returns the floor of a value.
     */
    M_PUBLIC_API Real floor(Real value);

    /**
     * Function that calculates the floor of a value.
     *
     * \param[in] value The value to calculate the floor of.
     *
     * \return Returns the floor of a value.
     */
    M_PUBLIC_API Real floor(const Complex& value);

    /**
     * Function that calculates the floor of a value.
     *
     * \param[in] value The value to calculate the floor of.
     *
     * \return Returns the floor of a value.
     */
    M_PUBLIC_API Real floor(const Variant& value);

    /**
     * Function that calculates the floor of a value.
     *
     * \param[in] value The value to calculate the floor of.
     *
     * \return Returns the floor of a value.
     */
    M_PUBLIC_API inline Integer floor(Boolean value) {
        return value ? 1 : 0;
    }

    /**
     * Function that calculates the ceiling of a value.
     *
     * \param[in] value The value to calculate the ceiling of.
     *
     * \return Returns the ceiling of the value.
     */
    M_PUBLIC_API inline Integer ceil(Integer value) {
        return value;
    }

    /**
     * Function that calculates the ceiling of a value.
     *
     * \param[in] value The value to calculate the ceiling of.
     *
     * \return Returns the ceiling of a value.
     */
    M_PUBLIC_API Real ceil(Real value);

    /**
     * Function that calculates the ceiling of a value.
     *
     * \param[in] value The value to calculate the ceiling of.
     *
     * \return Returns the ceiling of a value.
     */
    M_PUBLIC_API Real ceil(const Complex& value);

    /**
     * Function that calculates the ceiling of a value.
     *
     * \param[in] value The value to calculate the ceiling of.
     *
     * \return Returns the ceiling of a value.
     */
    M_PUBLIC_API Real ceil(const Variant& value);

    /**
     * Function that calculates the floor of a value.
     *
     * \param[in] value The value to calculate the floor of.
     *
     * \return Returns the floor of a value.
     */
    M_PUBLIC_API inline Integer ceil(Boolean value) {
        return value ? 1 : 0;
    }

    /**
     * Function that calculates the nearest integer of a value.
     *
     * \param[in] value The value to calculate the nearest integer of.
     *
     * \return Returns the nearest integer of the value.
     */
    M_PUBLIC_API inline Integer nint(Integer value) {
        return value;
    }

    /**
     * Function that calculates the nearest integer of a value.  This method conforms with IEEE 754 using round to
     * nearest rules.  Values with a fractional part == 0.5 will round to the cloest even value.
     *
     * \param[in] value The value to calculate the nearest integer of.
     *
     * \return Returns the nearest integer of the value.
     */
    M_PUBLIC_API Real nint(Real value);

    /**
     * Function that calculates the nearest integer of a value.  This method conforms with IEEE 754 using round to
     * nearest rules.  Values with a fractional part == 0.5 will round to the cloest even value.
     *
     * \param[in] value The value to calculate the nearest integer of.
     *
     * \return Returns the nearest integer of the value.
     */
    M_PUBLIC_API Real nint(const Complex& value);

    /**
     * Function that calculates the nearest integer of a value.  This method conforms with IEEE 754 using round to
     * nearest rules.  Values with a fractional part == 0.5 will round to the cloest even value.
     *
     * \param[in] value The value to calculate the nearest integer of.
     *
     * \return Returns the nearest integer of the value.
     */
    M_PUBLIC_API Real nint(const Variant& value);

    /**
     * Function that calculates the nearest integer of a value.  This method conforms with IEEE 754 using round to
     * nearest rules.  Values with a fractional part == 0.5 will round to the cloest even value.
     *
     * \param[in] value The value to calculate the nearest integer of.
     *
     * \return Returns the nearest integer of the value.
     */
    M_PUBLIC_API inline Integer nint(Boolean value) {
        return value ? 1 : 0;
    }

    /**
     * Function that calculates the factorial of a value.  Given the relatively small number of available factorial
     * values, all factorials are calculated on the first call to this function using arbitrary precision integer
     * arithmetic and stored in a table for later use.
     *
     * \param[in] value The value to calculate the factorial of.
     *
     * \return Returns the factorial of the requested value.   This method will return NaN for non-integer values and
     *         negative values and infinity if the supplied value triggers an overflow.
     */
    M_PUBLIC_API Real factorial(Integer value);

    /**
     * Function that calculates the factorial of a value.  Given the relatively small number of available factorial
     * values, all factorials are calculated on the first call to this function using arbitrary precision integer
     * arithmetic and stored in a table for later use.
     *
     * \param[in] value The value to calculate the factorial of.
     *
     * \return Returns the factorial of the requested value.   This method will return NaN for non-integer values and
     *         negative values and infinity if the supplied value triggers an overflow.
     */
    M_PUBLIC_API Real factorial(Real value);

    /**
     * Function that calculates the factorial of a value.  Given the relatively small number of available factorial
     * values, all factorials are calculated on the first call to this function using arbitrary precision integer
     * arithmetic and stored in a table for later use.
     *
     * \param[in] value The value to calculate the factorial of.
     *
     * \return Returns the factorial of the requested value.   This method will return NaN for non-integer values and
     *         negative values and infinity if the supplied value triggers an overflow.
     */
    M_PUBLIC_API Real factorial(const Complex& value);

    /**
     * Function that calculates the factorial of a value.  Given the relatively small number of available factorial
     * values, all factorials are calculated on the first call to this function using arbitrary precision integer
     * arithmetic and stored in a table for later use.
     *
     * \param[in] value The value to calculate the factorial of.
     *
     * \return Returns the factorial of the requested value.   This method will return NaN for non-integer values and
     *         negative values and infinity if the supplied value triggers an overflow.
     */
    M_PUBLIC_API Real factorial(const Variant& value);

    /**
     * Function that calculates the Gamma function using either the factorial table or Lanczos approximation.
     *
     * \param[in] value The value to calculate the factorial of.
     *
     * \return Returns the gamma function of the requested value.
     */
    M_PUBLIC_API Real gamma(Integer value);

    /**
     * Function that calculates the Gamma function using Lanczos approximation.
     *
     * \param[in] value The value to calculate the factorial of.
     *
     * \return Returns the gamma function of the requested value.
     */
    M_PUBLIC_API Real gamma(Real value);

    /**
     * Function that calculates the Gamma function using Lanczos approximation.
     *
     * \param[in] value The value to calculate the factorial of.
     *
     * \return Returns the gamma function of the requested value.
     */
    M_PUBLIC_API Complex gamma(const Complex& value);

    /**
     * Function that calculates the Gamma function using Lanczos approximation.
     *
     * \param[in] value The value to calculate the factorial of.
     *
     * \return Returns the gamma function of the requested value.
     */
    M_PUBLIC_API Complex gamma(const Variant& value);

    /**
     * Function that calculates the Gamma function using Lanczos approximation.
     *
     * \param[in] value The value to calculate the factorial of.
     *
     * \return Returns the gamma function of the requested value.
     */
    M_PUBLIC_API inline Real gamma(Boolean value) {
        return gamma(M::Integer(value ? 1 : 0));
    }

    /**
     * Function that returns the real portion of a complex value.
     *
     * \param[in] v The value to return the real portion of.
     *
     * \return Returns the real portion of the complex value.
     */
    M_PUBLIC_API inline Real real(const Complex& v) {
        return v.real();
    }

    /**
     * Function that returns the imaginary portion of a complex value.
     *
     * \param[in] v The value to return the imaginary portion of.
     *
     * \return Returns the imaginary portion of the complex value.
     */
    M_PUBLIC_API inline Real imag(const Complex& v) {
        return v.imag();
    }

    /**
     * Function that returns the real portion of a real value.
     *
     * \param[in] v The value to return the real portion of.
     *
     * \return Returns the real portion of the real value.
     */
    M_PUBLIC_API inline Real real(Real v) {
        return v;
    }

    /**
     * Function that returns the imaginary portion of a real value.
     *
     * \param[in] v The value to return the imaginary portion of.
     *
     * \return Returns zero.
     */
    M_PUBLIC_API inline Real imag(Real v) {
        (void) v;
        return Real(0);
    }

    /**
     * Function that returns the real portion of an integer value.
     *
     * \param[in] v The value to return the real portion of.
     *
     * \return Returns the real portion of the integer value.
     */
    M_PUBLIC_API inline Integer real(Integer v) {
        return v;
    }

    /**
     * Function that returns the imaginary portion of an integer value.
     *
     * \param[in] v The value to return the imaginary portion of.
     *
     * \return Returns zero.
     */
    M_PUBLIC_API inline Integer imag(Integer v) {
        (void) v;
        return Integer(0);
    }

    /**
     * Function that returns the real portion of a boolean value.
     *
     * \param[in] v The value to return the real portion of.
     *
     * \return Returns the real portion of the boolean value.
     */
    M_PUBLIC_API inline Integer real(Boolean v) {
        return v ? 1 : 0;
    }

    /**
     * Function that returns the imaginary portion of a boolean value.
     *
     * \param[in] v The value to return the imaginary portion of.
     *
     * \return Returns false.
     */
    M_PUBLIC_API inline Integer imag(Boolean v) {
        (void) v;
        return Integer(0);
    }

    /**
     * Function that calculates the natural log of a value.
     *
     * \param[in] value The value to calculate the natural log of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API Complex ln(Integer value);

    /**
     * Function that calculates the natural log of a value.
     *
     * \param[in] value The value to calculate the natural log of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API Complex ln(Real value);

    /**
     * Function that calculates the natural log of a value.
     *
     * \param[in] value The value to calculate the natural log of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API Complex ln(const Complex& value);

    /**
     * Function that calculates the natural log of a value.
     *
     * \param[in] value The value to calculate the natural log of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API Complex ln(const Variant& value);

    /**
     * Function that calculates the natural log of a value.
     *
     * \param[in] value The value to calculate the natural log of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API inline Complex ln(Boolean value) {
        return ln(Integer(value ? 1 : 0));
    };

    /**
     * Function that calculates the log base 10 of a value.
     *
     * \param[in] base  The base to apply to the log.
     *
     * \param[in] value The value to calculate the natural log of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API Complex log(Integer base, Integer value);

    /**
     * Function that calculates the log base 10 a value.
     *
     * \param[in] base  The base to apply to the log.
     *
     * \param[in] value The value to calculate the natural log of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API Complex log(Integer base, Real value);

    /**
     * Function that calculates the log base 10 of a value.
     *
     * \param[in] base  The base to apply to the log.
     *
     * \param[in] value The value to calculate the natural log of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API Complex log(Integer base, const Complex& value);

    /**
     * Function that calculates the log base 10 of a value.
     *
     * \param[in] base  The base to apply to the log.
     *
     * \param[in] value The value to calculate the natural log of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API Complex log(Real base, Integer value);

    /**
     * Function that calculates the log base 10 a value.
     *
     * \param[in] base  The base to apply to the log.
     *
     * \param[in] value The value to calculate the natural log of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API Complex log(Real base, Real value);

    /**
     * Function that calculates the log base 10 of a value.
     *
     * \param[in] base  The base to apply to the log.
     *
     * \param[in] value The value to calculate the natural log of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API Complex log(Real base, const Complex& value);

    /**
     * Function that calculates the log base 10 of a value.
     *
     * \param[in] base  The base to apply to the log.
     *
     * \param[in] value The value to calculate the natural log of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API inline Complex log(const Complex& base, Integer value) {
        return ln(value) / ln(base);
    }

    /**
     * Function that calculates the log base 10 a value.
     *
     * \param[in] base  The base to apply to the log.
     *
     * \param[in] value The value to calculate the natural log of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API inline Complex log(const Complex& base, Real value) {
        return ln(value) / ln(base);
    }

    /**
     * Function that calculates the log base 10 of a value.
     *
     * \param[in] base  The base to apply to the log.
     *
     * \param[in] value The value to calculate the natural log of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API inline Complex log(const Complex& base, const Complex& value) {
        return ln(value) / ln(base);
    }

    /**
     * Function that calculates the log base 10 of a value.
     *
     * \param[in] base  The base to apply to the log.
     *
     * \param[in] value The value to calculate the natural log of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API Complex log(const Variant& base, Integer value);

    /**
     * Function that calculates the log base 10 of a value.
     *
     * \param[in] base  The base to apply to the log.
     *
     * \param[in] value The value to calculate the natural log of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API Complex log(const Variant& base, Real value);

    /**
     * Function that calculates the log base 10 of a value.
     *
     * \param[in] base  The base to apply to the log.
     *
     * \param[in] value The value to calculate the natural log of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API Complex log(const Variant& base, const Complex& value);

    /**
     * Function that calculates the log base 10 of a value.
     *
     * \param[in] base  The base to apply to the log.
     *
     * \param[in] value The value to calculate the natural log of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API Complex log(Integer base, const Variant& value);

    /**
     * Function that calculates the log base 10 of a value.
     *
     * \param[in] base  The base to apply to the log.
     *
     * \param[in] value The value to calculate the natural log of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API Complex log(Real base, const Variant& value);

    /**
     * Function that calculates the log base 10 of a value.
     *
     * \param[in] base  The base to apply to the log.
     *
     * \param[in] value The value to calculate the natural log of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API Complex log(const Complex& base, const Variant& value);

    /**
     * Function that calculates the log base 10 of a value.
     *
     * \param[in] base  The base to apply to the log.
     *
     * \param[in] value The value to calculate the natural log of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API Complex log(const Variant& base, const Variant& value);

    /**
     * Function that calculates the log base 10 of a value.
     *
     * \param[in] base  The base to apply to the log.
     *
     * \param[in] value The value to calculate the natural log of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API inline Complex log(Integer base, Boolean value) {
        return log(base, M::Integer(value ? 1 : 0));
    }

    /**
     * Function that calculates the log base 10 of a value.
     *
     * \param[in] base  The base to apply to the log.
     *
     * \param[in] value The value to calculate the natural log of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API inline Complex log(Real base, Boolean value) {
        return log(base, M::Integer(value ? 1 : 0));
    }

    /**
     * Function that calculates the log base 10 of a value.
     *
     * \param[in] base  The base to apply to the log.
     *
     * \param[in] value The value to calculate the natural log of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API inline Complex log(const Complex& base, Boolean value) {
        return log(base, M::Integer(value ? 1 : 0));
    }

    /**
     * Function that calculates the log base 10 of a value.
     *
     * \param[in] base  The base to apply to the log.
     *
     * \param[in] value The value to calculate the natural log of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API inline Complex log(const Variant& base, Boolean value) {
        return log(base, M::Integer(value ? 1 : 0));
    }

    /**
     * Function that calculates the natural log of the factorial of a value.
     *
     * \param[in] value The value to calculate the log factorial of.
     *
     * \return Returns the natural log of the factorial of a value.
     */
    M_PUBLIC_API Real lnFactorial(Integer value);

    /**
     * Function that calculates the natural log of the factorial of a value.
     *
     * \param[in] value The value to calculate the log factorial of.
     *
     * \return Returns the natural log of the factorial of a value.
     */
    M_PUBLIC_API Real lnFactorial(Real value);

    /**
     * Function that calculates the natural log of the factorial of a value.
     *
     * \param[in] value The value to calculate the log factorial of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API Real lnFactorial(const Complex& value);

    /**
     * Function that calculates the natural log of the factorial of a value.
     *
     * \param[in] value The value to calculate the log factorial of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API inline Real lnFactorial(const Variant& value) {
        return lnFactorial(value.toInteger());
    }

    /**
     * Function that calculates the natural log of the factorial of a value.
     *
     * \param[in] value The value to calculate the log factorial of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API inline Real lnFactorial(Boolean value) {
        return lnFactorial(M::Integer(value ? 1 : 0));
    }

    /**
     * Function that calculates the log base b of the factorial of a value.
     *
     * \param[in] base  The base to apply to the log.
     *
     * \param[in] value The value to calculate the log factorial of.
     *
     * \return Returns the log base b of the factorial of a value.
     */
    template<typename B, typename V> M_PUBLIC_TEMPLATE_FUNCTION Complex logFactorial(B base, V value) {
        return lnFactorial(value) / ln(base);
    }

    /**
     * Function that calculates the natural log of the gamma function of a value.
     *
     * \param[in] value The value to calculate the log gamma function of.
     *
     * \return Returns the natural log of the gamma function of a value.
     */
    M_PUBLIC_API Real lnGamma(Integer value);

    /**
     * Function that calculates the natural log of the gamma function of a value.
     *
     * \param[in] value The value to calculate the log gamma function of.
     *
     * \return Returns the natural log of the gamma function of a value.
     */
    M_PUBLIC_API Real lnGamma(Real value);

    /**
     * Function that calculates the natural log of the gamma function of a value.
     *
     * \param[in] value The value to calculate the log gamma function of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API Complex lnGamma(const Complex& value);

    /**
     * Function that calculates the natural log of the gamma function of a value.
     *
     * \param[in] value The value to calculate the log gamma function of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API Complex lnGamma(const Variant& value);

    /**
     * Function that calculates the natural log of the gamma function of a value.
     *
     * \param[in] value The value to calculate the log gamma function of.
     *
     * \return Returns the natural log of a value.
     */
    M_PUBLIC_API inline Complex lnGamma(Boolean value) {
        return lnGamma(Integer(value ? 1 : 0));
    }

    /**
     * Function that calculates the log base b of the gamma function of a value.
     *
     * \param[in] base  The base to apply to the log.
     *
     * \param[in] value The value to calculate the log gamma function of.
     *
     * \return Returns the log base b of the gamma function of a value.
     */
    template<typename B, typename V> M_PUBLIC_TEMPLATE_FUNCTION Complex logGamma(B base, V value) {
        return lnGamma(value) / ln(base);
    }

    /**
     * Function that calculates the binomial coefficient.  For small values of n and k, this method calculates the
     * binomial coefficient directly.  For large values, this method uses log factorial to calculate the binomial
     * coefficient.
     *
     * \param[in] n The number of values.
     *
     * \param[in] k The number to choose from.
     *
     * \return Returns the binomial coefficient of n/k.
     */
    M_PUBLIC_API Real binomial(Integer n, Integer k);

    /**
     * Function that calculates the binomial coefficient.  For small values of n and k, this method calculates the
     * binomial coefficient directly.  For large values, this method uses log factorial to calculate the binomial
     * coefficient.
     *
     * \param[in] n The number of values.
     *
     * \param[in] k The number to choose from.
     *
     * \return Returns the binomial coefficient of n/k.
     */
    M_PUBLIC_API Real binomial(Integer n, Real k);

    /**
     * Function that calculates the binomial coefficient.  For small values of n and k, this method calculates the
     * binomial coefficient directly.  For large values, this method uses log factorial to calculate the binomial
     * coefficient.
     *
     * \param[in] n The number of values.
     *
     * \param[in] k The number to choose from.
     *
     * \return Returns the binomial coefficient of n/k.
     */
    M_PUBLIC_API Real binomial(Real n, Integer k);

    /**
     * Function that calculates the binomial coefficient.  For small values of n and k, this method calculates the
     * binomial coefficient directly.  For large values, this method uses log factorial to calculate the binomial
     * coefficient.
     *
     * \param[in] n The number of values.
     *
     * \param[in] k The number to choose from.
     *
     * \return Returns the binomial coefficient of n/k.
     */
    M_PUBLIC_API Real binomial(Real n, Real k);

    /**
     * Function that calculates the binomial coefficient.  For small values of n and k, this method calculates the
     * binomial coefficient directly.  For large values, this method uses log factorial to calculate the binomial
     * coefficient.
     *
     * \param[in] n The number of values.
     *
     * \param[in] k The number to choose from.
     *
     * \return Returns the binomial coefficient of n/k.
     */
    M_PUBLIC_API Real binomial(Integer n, const Complex& k);

    /**
     * Function that calculates the binomial coefficient.  For small values of n and k, this method calculates the
     * binomial coefficient directly.  For large values, this method uses log factorial to calculate the binomial
     * coefficient.
     *
     * \param[in] n The number of values.
     *
     * \param[in] k The number to choose from.
     *
     * \return Returns the binomial coefficient of n/k.
     */
    M_PUBLIC_API Real binomial(Real n, const Complex& k);

    /**
     * Function that calculates the binomial coefficient.  For small values of n and k, this method calculates the
     * binomial coefficient directly.  For large values, this method uses log factorial to calculate the binomial
     * coefficient.
     *
     * \param[in] n The number of values.
     *
     * \param[in] k The number to choose from.
     *
     * \return Returns the binomial coefficient of n/k.
     */
    M_PUBLIC_API Real binomial(const Complex& n, Integer k);

    /**
     * Function that calculates the binomial coefficient.  For small values of n and k, this method calculates the
     * binomial coefficient directly.  For large values, this method uses log factorial to calculate the binomial
     * coefficient.
     *
     * \param[in] n The number of values.
     *
     * \param[in] k The number to choose from.
     *
     * \return Returns the binomial coefficient of n/k.
     */
    M_PUBLIC_API Real binomial(const Complex& n, Real k);

    /**
     * Function that calculates the binomial coefficient.  For small values of n and k, this method calculates the
     * binomial coefficient directly.  For large values, this method uses log factorial to calculate the binomial
     * coefficient.
     *
     * \param[in] n The number of values.
     *
     * \param[in] k The number to choose from.
     *
     * \return Returns the binomial coefficient of n/k.
     */
    M_PUBLIC_API Real binomial(const Complex& n, const Complex& k);

    /**
     * Function that calculates the binomial coefficient.  For small values of n and k, this method calculates the
     * binomial coefficient directly.  For large values, this method uses log factorial to calculate the binomial
     * coefficient.
     *
     * \param[in] n The number of values.
     *
     * \param[in] k The number to choose from.
     *
     * \return Returns the binomial coefficient of n/k.
     */
    M_PUBLIC_API Real binomial(const Variant& n, Integer k);

    /**
     * Function that calculates the binomial coefficient.  For small values of n and k, this method calculates the
     * binomial coefficient directly.  For large values, this method uses log factorial to calculate the binomial
     * coefficient.
     *
     * \param[in] n The number of values.
     *
     * \param[in] k The number to choose from.
     *
     * \return Returns the binomial coefficient of n/k.
     */
    M_PUBLIC_API Real binomial(const Variant& n, Real k);

    /**
     * Function that calculates the binomial coefficient.  For small values of n and k, this method calculates the
     * binomial coefficient directly.  For large values, this method uses log factorial to calculate the binomial
     * coefficient.
     *
     * \param[in] n The number of values.
     *
     * \param[in] k The number to choose from.
     *
     * \return Returns the binomial coefficient of n/k.
     */
    M_PUBLIC_API Real binomial(const Variant& n, const Complex& k);

    /**
     * Function that calculates the binomial coefficient.  For small values of n and k, this method calculates the
     * binomial coefficient directly.  For large values, this method uses log factorial to calculate the binomial
     * coefficient.
     *
     * \param[in] n The number of values.
     *
     * \param[in] k The number to choose from.
     *
     * \return Returns the binomial coefficient of n/k.
     */
    M_PUBLIC_API Real binomial(Integer n, const Variant& k);

    /**
     * Function that calculates the binomial coefficient.  For small values of n and k, this method calculates the
     * binomial coefficient directly.  For large values, this method uses log factorial to calculate the binomial
     * coefficient.
     *
     * \param[in] n The number of values.
     *
     * \param[in] k The number to choose from.
     *
     * \return Returns the binomial coefficient of n/k.
     */
    M_PUBLIC_API Real binomial(Real n, const Variant& k);

    /**
     * Function that calculates the binomial coefficient.  For small values of n and k, this method calculates the
     * binomial coefficient directly.  For large values, this method uses log factorial to calculate the binomial
     * coefficient.
     *
     * \param[in] n The number of values.
     *
     * \param[in] k The number to choose from.
     *
     * \return Returns the binomial coefficient of n/k.
     */
    M_PUBLIC_API Real binomial(const Complex& n, const Variant& k);

    /**
     * Function that calculates the binomial coefficient.  For small values of n and k, this method calculates the
     * binomial coefficient directly.  For large values, this method uses log factorial to calculate the binomial
     * coefficient.
     *
     * \param[in] n The number of values.
     *
     * \param[in] k The number to choose from.
     *
     * \return Returns the binomial coefficient of n/k.
     */
    M_PUBLIC_API Real binomial(const Variant& n, const Variant& k);

    /**
     * Function that calculates the binomial coefficient.  For small values of n and k, this method calculates the
     * binomial coefficient directly.  For large values, this method uses log factorial to calculate the binomial
     * coefficient.
     *
     * \param[in] n The number of values.
     *
     * \param[in] k The number to choose from.
     *
     * \return Returns the binomial coefficient of n/k.
     */
    M_PUBLIC_API inline Real binomial(Boolean n, Boolean k) {
        return binomial(Integer(n ? 1 : 0), Integer(k ? 1 : 0));
    }

    /**
     * Function that calculates the binomial coefficient.  For small values of n and k, this method calculates the
     * binomial coefficient directly.  For large values, this method uses log factorial to calculate the binomial
     * coefficient.
     *
     * \param[in] n The number of values.
     *
     * \param[in] k The number to choose from.
     *
     * \return Returns the binomial coefficient of n/k.
     */
    M_PUBLIC_API inline Real binomial(Boolean n, Integer k) {
        return binomial(Integer(n ? 1 : 0), k);
    }

    /**
     * Function that calculates the binomial coefficient.  For small values of n and k, this method calculates the
     * binomial coefficient directly.  For large values, this method uses log factorial to calculate the binomial
     * coefficient.
     *
     * \param[in] n The number of values.
     *
     * \param[in] k The number to choose from.
     *
     * \return Returns the binomial coefficient of n/k.
     */
    M_PUBLIC_API inline Real binomial(Boolean n, Real k) {
        return binomial(Integer(n ? 1 : 0), k);
    }

    /**
     * Function that calculates the binomial coefficient.  For small values of n and k, this method calculates the
     * binomial coefficient directly.  For large values, this method uses log factorial to calculate the binomial
     * coefficient.
     *
     * \param[in] n The number of values.
     *
     * \param[in] k The number to choose from.
     *
     * \return Returns the binomial coefficient of n/k.
     */
    M_PUBLIC_API inline Real binomial(Boolean n, const Complex& k) {
        return binomial(Integer(n ? 1 : 0), k);
    }

    /**
     * Function that calculates the binomial coefficient.  For small values of n and k, this method calculates the
     * binomial coefficient directly.  For large values, this method uses log factorial to calculate the binomial
     * coefficient.
     *
     * \param[in] n The number of values.
     *
     * \param[in] k The number to choose from.
     *
     * \return Returns the binomial coefficient of n/k.
     */
    M_PUBLIC_API inline Real binomial(Integer n, Boolean k) {
        return binomial(n, Integer(k ? 1 : 0));
    }

    /**
     * Function that calculates the binomial coefficient.  For small values of n and k, this method calculates the
     * binomial coefficient directly.  For large values, this method uses log factorial to calculate the binomial
     * coefficient.
     *
     * \param[in] n The number of values.
     *
     * \param[in] k The number to choose from.
     *
     * \return Returns the binomial coefficient of n/k.
     */
    M_PUBLIC_API inline Real binomial(Real n, Boolean k) {
        return binomial(n, Integer(k ? 1 : 0));
    }

    /**
     * Function that calculates the binomial coefficient.  For small values of n and k, this method calculates the
     * binomial coefficient directly.  For large values, this method uses log factorial to calculate the binomial
     * coefficient.
     *
     * \param[in] n The number of values.
     *
     * \param[in] k The number to choose from.
     *
     * \return Returns the binomial coefficient of n/k.
     */
    M_PUBLIC_API inline Real binomial(const Complex& n, Boolean k) {
        return binomial(n, Integer(k ? 1 : 0));
    }

    /**
     * Function that calculates the binomial coefficient.  For small values of n and k, this method calculates the
     * binomial coefficient directly.  For large values, this method uses log factorial to calculate the binomial
     * coefficient.
     *
     * \param[in] n The number of values.
     *
     * \param[in] k The number to choose from.
     *
     * \return Returns the binomial coefficient of n/k.
     */
    M_PUBLIC_API inline Real binomial(const Variant& n, Boolean k) {
        return binomial(n, Integer(k ? 1 : 0));
    }

    /**
     * Function that calculates the log binomial coefficient directly.
     *
     * \param[in] n The number of values.
     *
     * \param[in] k The number to choose from.
     *
     * \return Returns the binomial coefficient of n/k.
     */
    template<typename N, typename K> M_PUBLIC_TEMPLATE_FUNCTION Real lnBinomial(N n, K k) {
        M::Integer ni = inlineToInteger(n);
        M::Integer ki = inlineToInteger(k);

        return lnFactorial(ni) - lnFactorial(ki) - lnFactorial(ni - ki);
    }

    /**
     * Function that calculates the log base b binomial coefficient directly.
     *
     * \param[in] b The base.
     *
     * \param[in] n The number of values.
     *
     * \param[in] k The number to choose from.
     *
     * \return Returns the binomial coefficient of n/k.
     */
    template<typename B, typename N, typename K> M_PUBLIC_TEMPLATE_FUNCTION Complex logBinomial(B b, N n, K k) {
        return lnBinomial(n, k) / ln(b);
    }

    /**
     * Function that calculates the Sterling number of first kind.
     *
     * \param[in] n The number of elements.
     *
     * \param[in] k The number of cycles.
     *
     * \return Returns the number of possible permutations of cycles across the elements.
     */
    M_PUBLIC_API Integer unsignedSterlingNumber1(Integer n, Integer k);

    /**
     * Function that calculates the unsigned Sterling number of first kind.
     *
     * \param[in] n The number of elements.
     *
     * \param[in] k The number of cycles.
     *
     * \return Returns the number of possible permutations of cycles across the elements.
     */
    M_PUBLIC_API Real unsignedSterlingNumber1(Integer n, Real k);

    /**
     * Function that calculates the unsigned Sterling number of first kind.
     *
     * \param[in] n The number of elements.
     *
     * \param[in] k The number of cycles.
     *
     * \return Returns the number of possible permutations of cycles across the elements.
     */
    M_PUBLIC_API Real unsignedSterlingNumber1(Integer n, const Complex& k);

    /**
     * Function that calculates the unsigned Sterling number of first kind.
     *
     * \param[in] n The number of elements.
     *
     * \param[in] k The number of cycles.
     *
     * \return Returns the number of possible permutations of cycles across the elements.
     */
    M_PUBLIC_API Real unsignedSterlingNumber1(Real n, Integer k);

    /**
     * Function that calculates the unsigned Sterling number of first kind.
     *
     * \param[in] n The number of elements.
     *
     * \param[in] k The number of cycles.
     *
     * \return Returns the number of possible permutations of cycles across the elements.
     */
    M_PUBLIC_API Real unsignedSterlingNumber1(Real n, Real k);

    /**
     * Function that calculates the unsigned Sterling number of first kind.
     *
     * \param[in] n The number of elements.
     *
     * \param[in] k The number of cycles.
     *
     * \return Returns the number of possible permutations of cycles across the elements.
     */
    M_PUBLIC_API Real unsignedSterlingNumber1(Real n, const Complex& k);

    /**
     * Function that calculates the unsigned Sterling number of first kind.
     *
     * \param[in] n The number of elements.
     *
     * \param[in] k The number of cycles.
     *
     * \return Returns the number of possible permutations of cycles across the elements.
     */
    M_PUBLIC_API Real unsignedSterlingNumber1(const Complex& n, Integer k);

    /**
     * Function that calculates the unsigned Sterling number of first kind.
     *
     * \param[in] n The number of elements.
     *
     * \param[in] k The number of cycles.
     *
     * \return Returns the number of possible permutations of cycles across the elements.
     */
    M_PUBLIC_API Real unsignedSterlingNumber1(const Complex& n, Real k);

    /**
     * Function that calculates the unsigned Sterling number of first kind.
     *
     * \param[in] n The number of elements.
     *
     * \param[in] k The number of cycles.
     *
     * \return Returns the number of possible permutations of cycles across the elements.
     */
    M_PUBLIC_API Real unsignedSterlingNumber1(const Complex& n, const Complex& k);

    /**
     * Function that calculates the unsigned Sterling number of first kind.
     *
     * \param[in] n The number of elements.
     *
     * \param[in] k The number of cycles.
     *
     * \return Returns the number of possible permutations of cycles across the elements.
     */
    M_PUBLIC_API inline Real unsignedSterlingNumber1(const Variant& n, Integer k) {
        return unsignedSterlingNumber1(n.toInteger(), k);
    }

    /**
     * Function that calculates the unsigned Sterling number of first kind.
     *
     * \param[in] n The number of elements.
     *
     * \param[in] k The number of cycles.
     *
     * \return Returns the number of possible permutations of cycles across the elements.
     */
    M_PUBLIC_API inline Real unsignedSterlingNumber1(const Variant& n, Real k) {
        return unsignedSterlingNumber1(n.toInteger(), k);
    }

    /**
     * Function that calculates the unsigned Sterling number of first kind.
     *
     * \param[in] n The number of elements.
     *
     * \param[in] k The number of cycles.
     *
     * \return Returns the number of possible permutations of cycles across the elements.
     */
    M_PUBLIC_API inline Real unsignedSterlingNumber1(const Variant& n, const Complex& k) {
        return unsignedSterlingNumber1(n.toInteger(), k);
    }

    /**
     * Function that calculates the unsigned Sterling number of first kind.
     *
     * \param[in] n The number of elements.
     *
     * \param[in] k The number of cycles.
     *
     * \return Returns the number of possible permutations of cycles across the elements.
     */
    M_PUBLIC_API inline Real unsignedSterlingNumber1(Integer n, const Variant& k) {
        return unsignedSterlingNumber1(n, k.toInteger());
    }

    /**
     * Function that calculates the unsigned Sterling number of first kind.
     *
     * \param[in] n The number of elements.
     *
     * \param[in] k The number of cycles.
     *
     * \return Returns the number of possible permutations of cycles across the elements.
     */
    M_PUBLIC_API inline Real unsignedSterlingNumber1(Real n, const Variant& k) {
        return unsignedSterlingNumber1(n, k.toInteger());
    }

    /**
     * Function that calculates the unsigned Sterling number of first kind.
     *
     * \param[in] n The number of elements.
     *
     * \param[in] k The number of cycles.
     *
     * \return Returns the number of possible permutations of cycles across the elements.
     */
    M_PUBLIC_API inline Real unsignedSterlingNumber1(const Complex& n, const Variant& k) {
        return unsignedSterlingNumber1(n, k.toInteger());
    }

    /**
     * Function that calculates the unsigned Sterling number of first kind.
     *
     * \param[in] n The number of elements.
     *
     * \param[in] k The number of cycles.
     *
     * \return Returns the number of possible permutations of cycles across the elements.
     */
    M_PUBLIC_API inline Real unsignedSterlingNumber1(const Variant& n, const Variant& k) {
        return unsignedSterlingNumber1(n, k.toInteger());
    }

    /**
     * Function that calculates the unsigned Sterling number of first kind.
     *
     * \param[in] n The number of elements.
     *
     * \param[in] k The number of cycles.
     *
     * \return Returns the number of possible permutations of cycles across the elements.
     */
    M_PUBLIC_API inline Real unsignedSterlingNumber1(Boolean n, Boolean k) {
        return unsignedSterlingNumber1(Integer(n ? 1 : 0), Integer(k ? 1 : 0));
    }

    /**
     * Function that calculates the unsigned Sterling number of first kind.
     *
     * \param[in] n The number of elements.
     *
     * \param[in] k The number of cycles.
     *
     * \return Returns the number of possible permutations of cycles across the elements.
     */
    M_PUBLIC_API inline Real unsignedSterlingNumber1(Boolean n, Integer k) {
        return unsignedSterlingNumber1(Integer(n ? 1 : 0), k);
    }

    /**
     * Function that calculates the unsigned Sterling number of first kind.
     *
     * \param[in] n The number of elements.
     *
     * \param[in] k The number of cycles.
     *
     * \return Returns the number of possible permutations of cycles across the elements.
     */
    M_PUBLIC_API inline Real unsignedSterlingNumber1(Boolean n, Real k) {
        return unsignedSterlingNumber1(Integer(n ? 1 : 0), k);
    }

    /**
     * Function that calculates the unsigned Sterling number of first kind.
     *
     * \param[in] n The number of elements.
     *
     * \param[in] k The number of cycles.
     *
     * \return Returns the number of possible permutations of cycles across the elements.
     */
    M_PUBLIC_API inline Real unsignedSterlingNumber1(Boolean n, const Complex& k) {
        return unsignedSterlingNumber1(Integer(n ? 1 : 0), k);
    }

    /**
     * Function that calculates the unsigned Sterling number of first kind.
     *
     * \param[in] n The number of elements.
     *
     * \param[in] k The number of cycles.
     *
     * \return Returns the number of possible permutations of cycles across the elements.
     */
    M_PUBLIC_API inline Real unsignedSterlingNumber1(Boolean n, const Variant& k) {
        return unsignedSterlingNumber1(Integer(n ? 1 : 0), k);
    }

    /**
     * Function that calculates the unsigned Sterling number of first kind.
     *
     * \param[in] n The number of elements.
     *
     * \param[in] k The number of cycles.
     *
     * \return Returns the number of possible permutations of cycles across the elements.
     */
    M_PUBLIC_API inline Real unsignedSterlingNumber1(Integer n, Boolean k) {
        return unsignedSterlingNumber1(n, Integer(k ? 1 : 0));
    }

    /**
     * Function that calculates the unsigned Sterling number of first kind.
     *
     * \param[in] n The number of elements.
     *
     * \param[in] k The number of cycles.
     *
     * \return Returns the number of possible permutations of cycles across the elements.
     */
    M_PUBLIC_API inline Real unsignedSterlingNumber1(Real n, Boolean k) {
        return unsignedSterlingNumber1(n, Integer(k ? 1 : 0));
    }

    /**
     * Function that calculates the unsigned Sterling number of first kind.
     *
     * \param[in] n The number of elements.
     *
     * \param[in] k The number of cycles.
     *
     * \return Returns the number of possible permutations of cycles across the elements.
     */
    M_PUBLIC_API inline Real unsignedSterlingNumber1(const Complex& n, Boolean k) {
        return unsignedSterlingNumber1(n, Integer(k ? 1 : 0));
    }

    /**
     * Function that calculates the unsigned Sterling number of first kind.
     *
     * \param[in] n The number of elements.
     *
     * \param[in] k The number of cycles.
     *
     * \return Returns the number of possible permutations of cycles across the elements.
     */
    M_PUBLIC_API inline Real unsignedSterlingNumber1(const Variant& n, Boolean k) {
        return unsignedSterlingNumber1(n, Integer(k ? 1 : 0));
    }

    /**
     * Function that calculates Sterling numbers of the second kind.
     *
     * \param[in] n The number of labelled objects.
     *
     * \param[in] k The number of non-empty unlabelled subsets.
     *
     * \return Returns the Sterling numbers of the second kind.
     */
    M_PUBLIC_API Real sterlingNumber2(Integer n, Integer k);

    /**
     * Function that calculates Sterling numbers of the second kind.
     *
     * \param[in] n The number of labelled objects.
     *
     * \param[in] k The number of non-empty unlabelled subsets.
     *
     * \return Returns the Sterling numbers of the second kind.
     */
    M_PUBLIC_API Real sterlingNumber2(Integer n, Real k);

    /**
     * Function that calculates Sterling numbers of the second kind.
     *
     * \param[in] n The number of labelled objects.
     *
     * \param[in] k The number of non-empty unlabelled subsets.
     *
     * \return Returns the Sterling numbers of the second kind.
     */
    M_PUBLIC_API Real sterlingNumber2(Integer n, const Complex& k);

    /**
     * Function that calculates Sterling numbers of the second kind.
     *
     * \param[in] n The number of labelled objects.
     *
     * \param[in] k The number of non-empty unlabelled subsets.
     *
     * \return Returns the Sterling numbers of the second kind.
     */
    M_PUBLIC_API Real sterlingNumber2(Real n, Integer k);

    /**
     * Function that calculates Sterling numbers of the second kind.
     *
     * \param[in] n The number of labelled objects.
     *
     * \param[in] k The number of non-empty unlabelled subsets.
     *
     * \return Returns the Sterling numbers of the second kind.
     */
    M_PUBLIC_API Real sterlingNumber2(Real n, Real k);

    /**
     * Function that calculates Sterling numbers of the second kind.
     *
     * \param[in] n The number of labelled objects.
     *
     * \param[in] k The number of non-empty unlabelled subsets.
     *
     * \return Returns the Sterling numbers of the second kind.
     */
    M_PUBLIC_API Real sterlingNumber2(Real n, const Complex& k);

    /**
     * Function that calculates Sterling numbers of the second kind.
     *
     * \param[in] n The number of labelled objects.
     *
     * \param[in] k The number of non-empty unlabelled subsets.
     *
     * \return Returns the Sterling numbers of the second kind.
     */
    M_PUBLIC_API Real sterlingNumber2(const Complex& n, Integer k);

    /**
     * Function that calculates Sterling numbers of the second kind.
     *
     * \param[in] n The number of labelled objects.
     *
     * \param[in] k The number of non-empty unlabelled subsets.
     *
     * \return Returns the Sterling numbers of the second kind.
     */
    M_PUBLIC_API Real sterlingNumber2(const Complex& n, Real k);

    /**
     * Function that calculates Sterling numbers of the second kind.
     *
     * \param[in] n The number of labelled objects.
     *
     * \param[in] k The number of non-empty unlabelled subsets.
     *
     * \return Returns the Sterling numbers of the second kind.
     */
    M_PUBLIC_API Real sterlingNumber2(const Complex& n, const Complex& k);

    /**
     * Function that calculates Sterling numbers of the second kind.
     *
     * \param[in] n The number of labelled objects.
     *
     * \param[in] k The number of non-empty unlabelled subsets.
     *
     * \return Returns the Sterling numbers of the second kind.
     */
    M_PUBLIC_API inline Real sterlingNumber2(const Variant& n, Integer k) {
        return sterlingNumber2(n.toInteger(), k);
    }

    /**
     * Function that calculates Sterling numbers of the second kind.
     *
     * \param[in] n The number of labelled objects.
     *
     * \param[in] k The number of non-empty unlabelled subsets.
     *
     * \return Returns the Sterling numbers of the second kind.
     */
    M_PUBLIC_API inline Real sterlingNumber2(const Variant& n, Real k) {
        return sterlingNumber2(n.toInteger(), k);
    }

    /**
     * Function that calculates Sterling numbers of the second kind.
     *
     * \param[in] n The number of labelled objects.
     *
     * \param[in] k The number of non-empty unlabelled subsets.
     *
     * \return Returns the Sterling numbers of the second kind.
     */
    M_PUBLIC_API inline Real sterlingNumber2(const Variant& n, const Complex& k) {
        return sterlingNumber2(n.toInteger(), k);
    }

    /**
     * Function that calculates Sterling numbers of the second kind.
     *
     * \param[in] n The number of labelled objects.
     *
     * \param[in] k The number of non-empty unlabelled subsets.
     *
     * \return Returns the Sterling numbers of the second kind.
     */
    M_PUBLIC_API inline Real sterlingNumber2(Integer n, const Variant& k) {
        return sterlingNumber2(n, k.toInteger());
    }

    /**
     * Function that calculates Sterling numbers of the second kind.
     *
     * \param[in] n The number of labelled objects.
     *
     * \param[in] k The number of non-empty unlabelled subsets.
     *
     * \return Returns the Sterling numbers of the second kind.
     */
    M_PUBLIC_API inline Real sterlingNumber2(Real n, const Variant& k) {
        return sterlingNumber2(n, k.toInteger());
    }

    /**
     * Function that calculates Sterling numbers of the second kind.
     *
     * \param[in] n The number of labelled objects.
     *
     * \param[in] k The number of non-empty unlabelled subsets.
     *
     * \return Returns the Sterling numbers of the second kind.
     */
    M_PUBLIC_API inline Real sterlingNumber2(const Complex& n, const Variant& k) {
        return sterlingNumber2(n, k.toInteger());
    }

    /**
     * Function that calculates Sterling numbers of the second kind.
     *
     * \param[in] n The number of labelled objects.
     *
     * \param[in] k The number of non-empty unlabelled subsets.
     *
     * \return Returns the Sterling numbers of the second kind.
     */
    M_PUBLIC_API inline Real sterlingNumber2(const Variant& n, const Variant& k) {
        return sterlingNumber2(n.toInteger(), k.toInteger());
    }

    /**
     * Function that calculates Sterling numbers of the second kind.
     *
     * \param[in] n The number of labelled objects.
     *
     * \param[in] k The number of non-empty unlabelled subsets.
     *
     * \return Returns the Sterling numbers of the second kind.
     */
    M_PUBLIC_API inline Real sterlingNumber2(Boolean n, Boolean k) {
        return sterlingNumber2(Integer(n ? 1 : 0), Integer(k ? 1 : 0));
    }

    /**
     * Function that calculates Sterling numbers of the second kind.
     *
     * \param[in] n The number of labelled objects.
     *
     * \param[in] k The number of non-empty unlabelled subsets.
     *
     * \return Returns the Sterling numbers of the second kind.
     */
    M_PUBLIC_API inline Real sterlingNumber2(Boolean n, Integer k) {
        return sterlingNumber2(Integer(n ? 1 : 0), k);
    }

    /**
     * Function that calculates Sterling numbers of the second kind.
     *
     * \param[in] n The number of labelled objects.
     *
     * \param[in] k The number of non-empty unlabelled subsets.
     *
     * \return Returns the Sterling numbers of the second kind.
     */
    M_PUBLIC_API inline Real sterlingNumber2(Boolean n, Real k) {
        return sterlingNumber2(Integer(n ? 1 : 0), k);
    }

    /**
     * Function that calculates Sterling numbers of the second kind.
     *
     * \param[in] n The number of labelled objects.
     *
     * \param[in] k The number of non-empty unlabelled subsets.
     *
     * \return Returns the Sterling numbers of the second kind.
     */
    M_PUBLIC_API inline Real sterlingNumber2(Boolean n, const Complex& k) {
        return sterlingNumber2(Integer(n ? 1 : 0), k);
    }

    /**
     * Function that calculates Sterling numbers of the second kind.
     *
     * \param[in] n The number of labelled objects.
     *
     * \param[in] k The number of non-empty unlabelled subsets.
     *
     * \return Returns the Sterling numbers of the second kind.
     */
    M_PUBLIC_API inline Real sterlingNumber2(Boolean n, const Variant& k) {
        return sterlingNumber2(Integer(n ? 1 : 0), k);
    }

    /**
     * Function that calculates Sterling numbers of the second kind.
     *
     * \param[in] n The number of labelled objects.
     *
     * \param[in] k The number of non-empty unlabelled subsets.
     *
     * \return Returns the Sterling numbers of the second kind.
     */
    M_PUBLIC_API inline Real sterlingNumber2(Integer n, Boolean k) {
        return sterlingNumber2(n, Integer(k ? 1 : 0));
    }

    /**
     * Function that calculates Sterling numbers of the second kind.
     *
     * \param[in] n The number of labelled objects.
     *
     * \param[in] k The number of non-empty unlabelled subsets.
     *
     * \return Returns the Sterling numbers of the second kind.
     */
    M_PUBLIC_API inline Real sterlingNumber2(Real n, Boolean k) {
        return sterlingNumber2(n, Integer(k ? 1 : 0));
    }

    /**
     * Function that calculates Sterling numbers of the second kind.
     *
     * \param[in] n The number of labelled objects.
     *
     * \param[in] k The number of non-empty unlabelled subsets.
     *
     * \return Returns the Sterling numbers of the second kind.
     */
    M_PUBLIC_API inline Real sterlingNumber2(const Complex& n, Boolean k) {
        return sterlingNumber2(n, Integer(k ? 1 : 0));
    }

    /**
     * Function that calculates Sterling numbers of the second kind.
     *
     * \param[in] n The number of labelled objects.
     *
     * \param[in] k The number of non-empty unlabelled subsets.
     *
     * \return Returns the Sterling numbers of the second kind.
     */
    M_PUBLIC_API inline Real sterlingNumber2(const Variant& n, Boolean k) {
        return sterlingNumber2(n, Integer(k ? 1 : 0));
    }

    /**
     * Function that calculates the lower gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the lower gamma integral.
     *
     * \return Returns the lower gamma of s and z.
     */
    M_PUBLIC_API Real lowerGamma(Integer s, Integer z);

    /**
     * Function that calculates the lower gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the lower gamma integral.
     *
     * \return Returns the lower gamma of s and z.
     */
    M_PUBLIC_API Real lowerGamma(Integer s, Real z);

    /**
     * Function that calculates the lower gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the lower gamma integral.
     *
     * \return Returns the lower gamma of s and z.
     */
    M_PUBLIC_API Complex lowerGamma(Integer s, const Complex& z);

    /**
     * Function that calculates the lower gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the lower gamma integral.
     *
     * \return Returns the lower gamma of s and z.
     */
    M_PUBLIC_API Real lowerGamma(Real s, Integer z);

    /**
     * Function that calculates the lower gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the lower gamma integral.
     *
     * \return Returns the lower gamma of s and z.
     */
    M_PUBLIC_API Real lowerGamma(Real s, Real z);

    /**
     * Function that calculates the lower gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the lower gamma integral.
     *
     * \return Returns the lower gamma of s and z.
     */
    M_PUBLIC_API Complex lowerGamma(Real s, const Complex& z);

    /**
     * Function that calculates the lower gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the lower gamma integral.
     *
     * \return Returns the lower gamma of s and z.
     */
    M_PUBLIC_API Complex lowerGamma(const Complex& s, Integer z);

    /**
     * Function that calculates the lower gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the lower gamma integral.
     *
     * \return Returns the lower gamma of s and z.
     */
    M_PUBLIC_API Complex lowerGamma(const Complex& s, Real z);

    /**
     * Function that calculates the lower gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the lower gamma integral.
     *
     * \return Returns the lower gamma of s and z.
     */
    M_PUBLIC_API Complex lowerGamma(const Complex& s, const Complex& z);

    /**
     * Function that calculates the lower gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the lower gamma integral.
     *
     * \return Returns the lower gamma of s and z.
     */
    M_PUBLIC_API Complex lowerGamma(const Variant& s, Integer z);

    /**
     * Function that calculates the lower gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the lower gamma integral.
     *
     * \return Returns the lower gamma of s and z.
     */
    M_PUBLIC_API Complex lowerGamma(const Variant& s, Real z);

    /**
     * Function that calculates the lower gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the lower gamma integral.
     *
     * \return Returns the lower gamma of s and z.
     */
    M_PUBLIC_API Complex lowerGamma(const Variant& s, const Complex& z);

    /**
     * Function that calculates the lower gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the lower gamma integral.
     *
     * \return Returns the lower gamma of s and z.
     */
    M_PUBLIC_API Complex lowerGamma(Integer s, const Variant& z);

    /**
     * Function that calculates the lower gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the lower gamma integral.
     *
     * \return Returns the lower gamma of s and z.
     */
    M_PUBLIC_API Complex lowerGamma(Real s, const Variant& z);

    /**
     * Function that calculates the lower gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the lower gamma integral.
     *
     * \return Returns the lower gamma of s and z.
     */
    M_PUBLIC_API Complex lowerGamma(const Complex& s, const Variant& z);

    /**
     * Function that calculates the lower gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the lower gamma integral.
     *
     * \return Returns the lower gamma of s and z.
     */
    M_PUBLIC_API Complex lowerGamma(const Variant& s, const Variant& z);

    /**
     * Function that calculates the lower gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the lower gamma integral.
     *
     * \return Returns the lower gamma of s and z.
     */
    M_PUBLIC_API inline Complex lowerGamma(Boolean s, Boolean z) {
        return lowerGamma(Integer(s ? 1 : 0), Integer(z ? 1 : 0));
    }

    /**
     * Function that calculates the lower gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the lower gamma integral.
     *
     * \return Returns the lower gamma of s and z.
     */
    M_PUBLIC_API inline Complex lowerGamma(Boolean s, Integer z) {
        return lowerGamma(Integer(s ? 1 : 0), z);
    }

    /**
     * Function that calculates the lower gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the lower gamma integral.
     *
     * \return Returns the lower gamma of s and z.
     */
    M_PUBLIC_API inline Complex lowerGamma(Boolean s, Real z) {
        return lowerGamma(Integer(s ? 1 : 0), z);
    }

    /**
     * Function that calculates the lower gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the lower gamma integral.
     *
     * \return Returns the lower gamma of s and z.
     */
    M_PUBLIC_API inline Complex lowerGamma(Boolean s, const Complex& z) {
        return lowerGamma(Integer(s ? 1 : 0), z);
    }

    /**
     * Function that calculates the lower gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the lower gamma integral.
     *
     * \return Returns the lower gamma of s and z.
     */
    M_PUBLIC_API inline Complex lowerGamma(Boolean s, const Variant& z) {
        return lowerGamma(Integer(s ? 1 : 0), z);
    }

    /**
     * Function that calculates the lower gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the lower gamma integral.
     *
     * \return Returns the lower gamma of s and z.
     */
    M_PUBLIC_API inline Complex lowerGamma(Integer s, Boolean z) {
        return lowerGamma(s, Integer(z ? 1 : 0));
    }

    /**
     * Function that calculates the lower gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the lower gamma integral.
     *
     * \return Returns the lower gamma of s and z.
     */
    M_PUBLIC_API inline Complex lowerGamma(Real s, Boolean z) {
        return lowerGamma(s, Integer(z ? 1 : 0));
    }

    /**
     * Function that calculates the lower gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the lower gamma integral.
     *
     * \return Returns the lower gamma of s and z.
     */
    M_PUBLIC_API inline Complex lowerGamma(const Complex& s, Boolean z) {
        return lowerGamma(s, Integer(z ? 1 : 0));
    }

    /**
     * Function that calculates the lower gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the lower gamma integral.
     *
     * \return Returns the lower gamma of s and z.
     */
    M_PUBLIC_API inline Complex lowerGamma(const Variant& s, Boolean z) {
        return lowerGamma(s, Integer(z ? 1 : 0));
    }

    /**
     * Function that calculates the upper gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API Real upperGamma(Integer s, Integer z);

    /**
     * Function that calculates the upper gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API Real upperGamma(Integer s, Real z);

    /**
     * Function that calculates the upper gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API Complex upperGamma(Integer s, const Complex& z);

    /**
     * Function that calculates the upper gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API Real upperGamma(Real s, Integer z);

    /**
     * Function that calculates the upper gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API Real upperGamma(Real s, Real z);

    /**
     * Function that calculates the upper gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API Complex upperGamma(Real s, const Complex& z);

    /**
     * Function that calculates the upper gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API Complex upperGamma(const Complex& s, Integer z);

    /**
     * Function that calculates the upper gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API Complex upperGamma(const Complex& s, Real z);

    /**
     * Function that calculates the upper gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API Complex upperGamma(const Complex& s, const Complex& z);

    /**
     * Function that calculates the upper gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API Complex upperGamma(const Variant& s, Integer z);

    /**
     * Function that calculates the upper gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API Complex upperGamma(const Variant& s, Real z);

    /**
     * Function that calculates the upper gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API Complex upperGamma(const Variant& s, const Complex& z);

    /**
     * Function that calculates the upper gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API Complex upperGamma(Integer s, const Variant& z);

    /**
     * Function that calculates the upper gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API Complex upperGamma(Real s, const Variant& z);

    /**
     * Function that calculates the upper gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API Complex upperGamma(const Complex& s, const Variant& z);

    /**
     * Function that calculates the upper gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API Complex upperGamma(const Variant& s, const Variant& z);

    /**
     * Function that calculates the upper gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API inline Complex upperGamma(Boolean s, Boolean z) {
        return upperGamma(Integer(s ? 1 : 0), Integer(z ? 1 : 0));
    }

    /**
     * Function that calculates the upper gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API inline Complex upperGamma(Boolean s, Integer z) {
        return upperGamma(Integer(s ? 1 : 0), z);
    }

    /**
     * Function that calculates the upper gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API inline Complex upperGamma(Boolean s, Real z) {
        return upperGamma(Integer(s ? 1 : 0), z);
    }

    /**
     * Function that calculates the upper gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API inline Complex upperGamma(Boolean s, const Complex& z) {
        return upperGamma(Integer(s ? 1 : 0), z);
    }

    /**
     * Function that calculates the upper gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API inline Complex upperGamma(Boolean s, const Variant& z) {
        return upperGamma(Integer(s ? 1 : 0), z);
    }

    /**
     * Function that calculates the upper gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API inline Complex upperGamma(Integer s, Boolean z) {
        return upperGamma(s, Integer(z ? 1 : 0));
    }

    /**
     * Function that calculates the upper gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API inline Complex upperGamma(Real s, Boolean z) {
        return upperGamma(s, Integer(z ? 1 : 0));
    }

    /**
     * Function that calculates the upper gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API inline Complex upperGamma(const Complex& s, Boolean z) {
        return upperGamma(s, Integer(z ? 1 : 0));
    }

    /**
     * Function that calculates the upper gamma function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API inline Complex upperGamma(const Variant& s, Boolean z) {
        return upperGamma(s, Integer(z ? 1 : 0));
    }

    /**
     * Function that calculates the upper gamma function (alternate form).
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API Real gamma(Integer s, Integer z);

    /**
     * Function that calculates the upper gamma function (alternate form).
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API Real gamma(Integer s, Real z);

    /**
     * Function that calculates the upper gamma function (alternate form).
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API Complex gamma(Integer s, const Complex& z);

    /**
     * Function that calculates the upper gamma function (alternate form).
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API Real gamma(Real s, Integer z);

    /**
     * Function that calculates the upper gamma function (alternate form).
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API Real gamma(Real s, Real z);

    /**
     * Function that calculates the upper gamma function (alternate form).
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API Complex gamma(Real s, const Complex& z);

    /**
     * Function that calculates the upper gamma function (alternate form).
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API Complex gamma(const Complex& s, Integer z);

    /**
     * Function that calculates the upper gamma function (alternate form).
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API Complex gamma(const Complex& s, Real z);

    /**
     * Function that calculates the upper gamma function (alternate form).
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API Complex gamma(const Complex& s, const Complex& z);

    /**
     * Function that calculates the upper gamma function (alternate form).
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API inline Complex gamma(const Variant& s, Integer z) {
        return upperGamma(s, z);
    }

    /**
     * Function that calculates the upper gamma function (alternate form).
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API inline Complex gamma(const Variant& s, Real z) {
        return upperGamma(s, z);
    }

    /**
     * Function that calculates the upper gamma function (alternate form).
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API inline Complex gamma(const Variant& s, const Complex& z) {
        return upperGamma(s, z);
    }

    /**
     * Function that calculates the upper gamma function (alternate form).
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API inline Complex gamma(Integer s, const Variant& z) {
        return upperGamma(s, z);
    }

    /**
     * Function that calculates the upper gamma function (alternate form).
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API inline Complex gamma(Real s, const Variant& z) {
        return upperGamma(s, z);
    }

    /**
     * Function that calculates the upper gamma function (alternate form).
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API inline Complex gamma(const Complex& s, const Variant& z) {
        return upperGamma(s, z);
    }

    /**
     * Function that calculates the upper gamma function (alternate form).
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API inline Complex gamma(const Variant& s, const Variant& z) {
        return upperGamma(s, z);
    }

    /**
     * Function that calculates the upper gamma function (alternate form).
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API inline Complex gamma(Boolean s, Boolean z) {
        return gamma(M::Integer(s ? 1 : 0), M::Integer(z ? 1 : 0));
    }

    /**
     * Function that calculates the upper gamma function (alternate form).
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API inline Complex gamma(Boolean s, Integer z) {
        return gamma(M::Integer(s ? 1 : 0), z);
    }

    /**
     * Function that calculates the upper gamma function (alternate form).
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API inline Complex gamma(Boolean s, Real z) {
        return gamma(M::Integer(s ? 1 : 0), z);
    }

    /**
     * Function that calculates the upper gamma function (alternate form).
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API inline Complex gamma(Boolean s, const Complex& z) {
        return gamma(M::Integer(s ? 1 : 0), z);
    }

    /**
     * Function that calculates the upper gamma function (alternate form).
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API inline Complex gamma(Boolean s, const Variant& z) {
        return gamma(M::Integer(s ? 1 : 0), z);
    }

    /**
     * Function that calculates the upper gamma function (alternate form).
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API inline Complex gamma(Integer s, Boolean z) {
        return gamma(s, M::Integer(z ? 1 : 0));
    }

    /**
     * Function that calculates the upper gamma function (alternate form).
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API inline Complex gamma(Real s, Boolean z) {
        return gamma(s, M::Integer(z ? 1 : 0));
    }

    /**
     * Function that calculates the upper gamma function (alternate form).
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API inline Complex gamma(const Complex& s, Boolean z) {
        return gamma(s, M::Integer(z ? 1 : 0));
    }

    /**
     * Function that calculates the upper gamma function (alternate form).
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the upper gamma of s and z.
     */
    M_PUBLIC_API inline Complex gamma(const Variant& s, Boolean z) {
        return gamma(s, M::Integer(z ? 1 : 0));
    }

    /**
     * Function that calculates the regularized gamma (normalized lower gamma) function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the regularized gamma integral.
     *
     * \return Returns the regularized gamma of s and z.
     */
    M_PUBLIC_API Real normalizedLowerGamma(Integer s, Integer z);

    /**
     * Function that calculates the regularized gamma (normalized lower gamma) function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the regularized gamma integral.
     *
     * \return Returns the regularized gamma of s and z.
     */
    M_PUBLIC_API Real normalizedLowerGamma(Integer s, Real z);

    /**
     * Function that calculates the regularized gamma (normalized lower gamma) function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the regularized gamma integral.
     *
     * \return Returns the regularized gamma of s and z.
     */
    M_PUBLIC_API Complex normalizedLowerGamma(Integer s, const Complex& z);

    /**
     * Function that calculates the regularized gamma (normalized lower gamma) function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the regularized gamma integral.
     *
     * \return Returns the regularized gamma of s and z.
     */
    M_PUBLIC_API Real normalizedLowerGamma(Real s, Integer z);

    /**
     * Function that calculates the regularized gamma (normalized lower gamma) function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the regularized gamma integral.
     *
     * \return Returns the regularized gamma of s and z.
     */
    M_PUBLIC_API Real normalizedLowerGamma(Real s, Real z);

    /**
     * Function that calculates the regularized gamma (normalized lower gamma) function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the regularized gamma integral.
     *
     * \return Returns the regularized gamma of s and z.
     */
    M_PUBLIC_API Complex normalizedLowerGamma(Real s, const Complex& z);

    /**
     * Function that calculates the regularized gamma (normalized lower gamma) function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the regularized gamma integral.
     *
     * \return Returns the regularized gamma of s and z.
     */
    M_PUBLIC_API Complex normalizedLowerGamma(const Complex& s, Integer z);

    /**
     * Function that calculates the regularized gamma (normalized lower gamma) function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the regularized gamma integral.
     *
     * \return Returns the regularized gamma of s and z.
     */
    M_PUBLIC_API Complex normalizedLowerGamma(const Complex& s, Real z);

    /**
     * Function that calculates the regularized gamma (normalized lower gamma) function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the regularized gamma integral.
     *
     * \return Returns the regularized gamma of s and z.
     */
    M_PUBLIC_API Complex normalizedLowerGamma(const Complex& s, const Complex& z);

    /**
     * Function that calculates the regularized gamma (normalized lower gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized gamma of s and z.
     */
    M_PUBLIC_API Complex normalizedLowerGamma(const Variant& s, Integer z);

    /**
     * Function that calculates the regularized gamma (normalized lower gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized gamma of s and z.
     */
    M_PUBLIC_API Complex normalizedLowerGamma(const Variant& s, Real z);

    /**
     * Function that calculates the regularized gamma (normalized lower gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized gamma of s and z.
     */
    M_PUBLIC_API Complex normalizedLowerGamma(const Variant& s, const Complex& z);

    /**
     * Function that calculates the regularized gamma (normalized lower gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized gamma of s and z.
     */
    M_PUBLIC_API Complex normalizedLowerGamma(Integer s, const Variant& z);

    /**
     * Function that calculates the regularized gamma (normalized lower gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized gamma of s and z.
     */
    M_PUBLIC_API Complex normalizedLowerGamma(Real s, const Variant& z);

    /**
     * Function that calculates the regularized gamma (normalized lower gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized gamma of s and z.
     */
    M_PUBLIC_API Complex normalizedLowerGamma(const Complex& s, const Variant& z);

    /**
     * Function that calculates the regularized gamma (normalized lower gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized gamma of s and z.
     */
    M_PUBLIC_API Complex normalizedLowerGamma(const Variant& s, const Variant& z);

    /**
     * Function that calculates the regularized gamma (normalized lower gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized gamma of s and z.
     */
    M_PUBLIC_API inline Complex normalizedLowerGamma(Boolean s, Boolean z) {
        return normalizedLowerGamma(Integer(s ? 1 : 0), Integer(z ? 1 : 0));
    }

    /**
     * Function that calculates the regularized gamma (normalized lower gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized gamma of s and z.
     */
    M_PUBLIC_API inline Complex normalizedLowerGamma(Boolean s, Integer z) {
        return normalizedLowerGamma(Integer(s ? 1 : 0), z);
    }

    /**
     * Function that calculates the regularized gamma (normalized lower gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized gamma of s and z.
     */
    M_PUBLIC_API inline Complex normalizedLowerGamma(Boolean s, Real z) {
        return normalizedLowerGamma(Integer(s ? 1 : 0), z);
    }

    /**
     * Function that calculates the regularized gamma (normalized lower gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized gamma of s and z.
     */
    M_PUBLIC_API inline Complex normalizedLowerGamma(Boolean s, const Complex& z) {
        return normalizedLowerGamma(Integer(s ? 1 : 0), z);
    }

    /**
     * Function that calculates the regularized gamma (normalized lower gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized gamma of s and z.
     */
    M_PUBLIC_API inline Complex normalizedLowerGamma(Boolean s, const Variant& z) {
        return normalizedLowerGamma(Integer(s ? 1 : 0), z);
    }

    /**
     * Function that calculates the regularized gamma (normalized lower gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized gamma of s and z.
     */
    M_PUBLIC_API inline Complex normalizedLowerGamma(Integer s, Boolean z) {
        return normalizedLowerGamma(s, Integer(z ? 1 : 0));
    }

    /**
     * Function that calculates the regularized gamma (normalized lower gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized gamma of s and z.
     */
    M_PUBLIC_API inline Complex normalizedLowerGamma(Real s, Boolean z) {
        return normalizedLowerGamma(s, Integer(z ? 1 : 0));
    }

    /**
     * Function that calculates the regularized gamma (normalized lower gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized gamma of s and z.
     */
    M_PUBLIC_API inline Complex normalizedLowerGamma(const Complex& s, Boolean z) {
        return normalizedLowerGamma(s, Integer(z ? 1 : 0));
    }

    /**
     * Function that calculates the regularized gamma (normalized lower gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized gamma of s and z.
     */
    M_PUBLIC_API inline Complex normalizedLowerGamma(const Variant& s, Boolean z) {
        return normalizedLowerGamma(s, Integer(z ? 1 : 0));
    }

    /**
     * Function that calculates the regularized complementary gamma (normalized upper gamma) function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the regularized complementary gamma integral.
     *
     * \return Returns the regularized complementary gamma of s and z.
     */
    M_PUBLIC_API Real normalizedUpperGamma(Integer s, Integer z);

    /**
     * Function that calculates the regularized complementary gamma (normalized upper gamma) function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the regularized complementary gamma integral.
     *
     * \return Returns the regularized complementary gamma of s and z.
     */
    M_PUBLIC_API Real normalizedUpperGamma(Integer s, Real z);

    /**
     * Function that calculates the regularized complementary gamma (normalized upper gamma) function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the regularized complementary gamma integral.
     *
     * \return Returns the regularized complementary gamma of s and z.
     */
    M_PUBLIC_API Complex normalizedUpperGamma(Integer s, const Complex& z);

    /**
     * Function that calculates the regularized complementary gamma (normalized upper gamma) function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the regularized complementary gamma integral.
     *
     * \return Returns the regularized complementary gamma of s and z.
     */
    M_PUBLIC_API Real normalizedUpperGamma(Real s, Integer z);

    /**
     * Function that calculates the regularized complementary gamma (normalized upper gamma) function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the regularized complementary gamma integral.
     *
     * \return Returns the regularized complementary gamma of s and z.
     */
    M_PUBLIC_API Real normalizedUpperGamma(Real s, Real z);

    /**
     * Function that calculates the regularized complementary gamma (normalized upper gamma) function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the regularized complementary gamma integral.
     *
     * \return Returns the regularized complementary gamma of s and z.
     */
    M_PUBLIC_API Complex normalizedUpperGamma(Real s, const Complex& z);

    /**
     * Function that calculates the regularized complementary gamma (normalized upper gamma) function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the regularized complementary gamma integral.
     *
     * \return Returns the regularized complementary gamma of s and z.
     */
    M_PUBLIC_API Complex normalizedUpperGamma(const Complex& s, Integer z);

    /**
     * Function that calculates the regularized complementary gamma (normalized upper gamma) function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the regularized complementary gamma integral.
     *
     * \return Returns the regularized complementary gamma of s and z.
     */
    M_PUBLIC_API Complex normalizedUpperGamma(const Complex& s, Real z);

    /**
     * Function that calculates the regularized complementary gamma (normalized upper gamma) function.
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the regularized complementary gamma integral.
     *
     * \return Returns the regularized complementary gamma of s and z.
     */
    M_PUBLIC_API Complex normalizedUpperGamma(const Complex& s, const Complex& z);

    /**
     * Function that calculates the regularized complementary gamma (normalized upper gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized complementary gamma of s and z.
     */
    M_PUBLIC_API Complex normalizedUpperGamma(const Variant& s, Integer z);

    /**
     * Function that calculates the regularized complementary gamma (normalized upper gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized complementary gamma of s and z.
     */
    M_PUBLIC_API Complex normalizedUpperGamma(const Variant& s, Real z);

    /**
     * Function that calculates the regularized complementary gamma (normalized upper gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized complementary gamma of s and z.
     */
    M_PUBLIC_API Complex normalizedUpperGamma(const Variant& s, const Complex& z);

    /**
     * Function that calculates the regularized complementary gamma (normalized upper gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized complementary gamma of s and z.
     */
    M_PUBLIC_API Complex normalizedUpperGamma(Integer s, const Variant& z);

    /**
     * Function that calculates the regularized complementary gamma (normalized upper gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized complementary gamma of s and z.
     */
    M_PUBLIC_API Complex normalizedUpperGamma(Real s, const Variant& z);

    /**
     * Function that calculates the regularized complementary gamma (normalized upper gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized complementary gamma of s and z.
     */
    M_PUBLIC_API Complex normalizedUpperGamma(const Complex& s, const Variant& z);

    /**
     * Function that calculates the regularized complementary gamma (normalized upper gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized complementary gamma of s and z.
     */
    M_PUBLIC_API Complex normalizedUpperGamma(const Variant& s, const Variant& z);

    /**
     * Function that calculates the complementary gamma (normalized upper gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized complementary gamma of s and z.
     */
    M_PUBLIC_API inline Complex normalizedUpperGamma(Boolean s, Boolean z) {
        return normalizedUpperGamma(Integer(s ? 1 : 0), Integer(z ? 1 : 0));
    }

    /**
     * Function that calculates the complementary gamma (normalized upper gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized complementary gamma of s and z.
     */
    M_PUBLIC_API inline Complex normalizedUpperGamma(Boolean s, Integer z) {
        return normalizedUpperGamma(Integer(s ? 1 : 0), z);
    }

    /**
     * Function that calculates the complementary gamma (normalized upper gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized complementary gamma of s and z.
     */
    M_PUBLIC_API inline Complex normalizedUpperGamma(Boolean s, Real z) {
        return normalizedUpperGamma(Integer(s ? 1 : 0), z);
    }

    /**
     * Function that calculates the complementary gamma (normalized upper gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized complementary gamma of s and z.
     */
    M_PUBLIC_API inline Complex normalizedUpperGamma(Boolean s, const Complex& z) {
        return normalizedUpperGamma(Integer(s ? 1 : 0), z);
    }

    /**
     * Function that calculates the complementary gamma (normalized upper gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized complementary gamma of s and z.
     */
    M_PUBLIC_API inline Complex normalizedUpperGamma(Boolean s, const Variant& z) {
        return normalizedUpperGamma(Integer(s ? 1 : 0), z);
    }

    /**
     * Function that calculates the complementary gamma (normalized upper gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized complementary gamma of s and z.
     */
    M_PUBLIC_API inline Complex normalizedUpperGamma(Integer s, Boolean z) {
        return normalizedUpperGamma(s, Integer(z ? 1 : 0));
    }

    /**
     * Function that calculates the complementary gamma (normalized upper gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized complementary gamma of s and z.
     */
    M_PUBLIC_API inline Complex normalizedUpperGamma(Real s, Boolean z) {
        return normalizedUpperGamma(s, Integer(z ? 1 : 0));
    }

    /**
     * Function that calculates the complementary gamma (normalized upper gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized complementary gamma of s and z.
     */
    M_PUBLIC_API inline Complex normalizedUpperGamma(const Complex& s, Boolean z) {
        return normalizedUpperGamma(s, Integer(z ? 1 : 0));
    }

    /**
     * Function that calculates the complementary gamma (normalized upper gamma) function..
     *
     * \param[in] s The exponent term.
     *
     * \param[in] z The ending range of the upper gamma integral.
     *
     * \return Returns the regularized complementary gamma of s and z.
     */
    M_PUBLIC_API inline Complex normalizedUpperGamma(const Variant& s, Boolean z) {
        return normalizedUpperGamma(s, Integer(z ? 1 : 0));
    }

    /**
     * Function that calculates the inverse lower gamma.
     *
     * \param[in] s The upper bounds for the integral.
     *
     * \param[in] y The value to determine the "z" term for.
     *
     * \return Returns the inverse lower gamma of s and y.
     */
    M_PUBLIC_API Real inverseLowerGamma(Real s, Real y);

    /**
     * Function that calculates the inverse lower gamma.
     *
     * \param[in] s The upper bounds for the integral.
     *
     * \param[in] y The value to determine the "z" term for.
     *
     * \return Returns the inverse lower gamma of s and y.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION Real inverseLowerGamma(T1 s, T2 y) {
        Real result;

        Real lS;
        Real lY;
        if (toReal(lS, s) && toReal(lY, y)) {
            result = inverseLowerGamma(lS, lY);
        } else {
            result = NaN;
        }

        return result;
    }

    /**
     * Function that calculates the beta function.
     *
     * \param[in] x The first beta function parameter.
     *
     * \param[in] y The second beta function parameter.
     *
     * \return Returns the beta function of x and y.
     */
    M_PUBLIC_API Real beta(Integer x, Integer y);

    /**
     * Function that calculates the beta function.
     *
     * \param[in] x The first beta function parameter.
     *
     * \param[in] y The second beta function parameter.
     *
     * \return Returns the beta function of x and y.
     */
    M_PUBLIC_API Real beta(Integer x, Real y);

    /**
     * Function that calculates the beta function.
     *
     * \param[in] x The first beta function parameter.
     *
     * \param[in] y The second beta function parameter.
     *
     * \return Returns the beta function of x and y.
     */
    M_PUBLIC_API Complex beta(Integer x, const Complex& y);

    /**
     * Function that calculates the beta function.
     *
     * \param[in] x The first beta function parameter.
     *
     * \param[in] y The second beta function parameter.
     *
     * \return Returns the beta function of x and y.
     */
    M_PUBLIC_API Real beta(Real x, Integer y);

    /**
     * Function that calculates the beta function.
     *
     * \param[in] x The first beta function parameter.
     *
     * \param[in] y The second beta function parameter.
     *
     * \return Returns the beta function of x and y.
     */
    M_PUBLIC_API Real beta(Real x, Real y);

    /**
     * Function that calculates the beta function.
     *
     * \param[in] x The first beta function parameter.
     *
     * \param[in] y The second beta function parameter.
     *
     * \return Returns the beta function of x and y.
     */
    M_PUBLIC_API Complex beta(Real x, const Complex& y);

    /**
     * Function that calculates the beta function.
     *
     * \param[in] x The first beta function parameter.
     *
     * \param[in] y The second beta function parameter.
     *
     * \return Returns the beta function of x and y.
     */
    M_PUBLIC_API Complex beta(const Complex& x, Integer y);

    /**
     * Function that calculates the beta function.
     *
     * \param[in] x The first beta function parameter.
     *
     * \param[in] y The second beta function parameter.
     *
     * \return Returns the beta function of x and y.
     */
    M_PUBLIC_API Complex beta(const Complex& x, Real y);

    /**
     * Function that calculates the beta function.
     *
     * \param[in] x The first beta function parameter.
     *
     * \param[in] y The second beta function parameter.
     *
     * \return Returns the beta function of x and y.
     */
    M_PUBLIC_API Complex beta(const Complex& x, const Complex& y);

    /**
     * Function that calculates the beta function.
     *
     * \param[in] x The first beta function parameter.
     *
     * \param[in] y The second beta function parameter.
     *
     * \return Returns the beta function of x and y.
     */
    M_PUBLIC_API Complex beta(const Variant& x, Integer y);

    /**
     * Function that calculates the beta function.
     *
     * \param[in] x The first beta function parameter.
     *
     * \param[in] y The second beta function parameter.
     *
     * \return Returns the beta function of x and y.
     */
    M_PUBLIC_API Complex beta(const Variant& x, Real y);

    /**
     * Function that calculates the beta function.
     *
     * \param[in] x The first beta function parameter.
     *
     * \param[in] y The second beta function parameter.
     *
     * \return Returns the beta function of x and y.
     */
    M_PUBLIC_API Complex beta(const Variant& x, const Complex& y);

    /**
     * Function that calculates the beta function.
     *
     * \param[in] x The first beta function parameter.
     *
     * \param[in] y The second beta function parameter.
     *
     * \return Returns the beta function of x and y.
     */
    M_PUBLIC_API Complex beta(Integer x, const Variant& y);

    /**
     * Function that calculates the beta function.
     *
     * \param[in] x The first beta function parameter.
     *
     * \param[in] y The second beta function parameter.
     *
     * \return Returns the beta function of x and y.
     */
    M_PUBLIC_API Complex beta(Real x, const Variant& y);

    /**
     * Function that calculates the beta function.
     *
     * \param[in] x The first beta function parameter.
     *
     * \param[in] y The second beta function parameter.
     *
     * \return Returns the beta function of x and y.
     */
    M_PUBLIC_API Complex beta(const Complex& x, const Variant& y);

    /**
     * Function that calculates the beta function.
     *
     * \param[in] x The first beta function parameter.
     *
     * \param[in] y The second beta function parameter.
     *
     * \return Returns the beta function of x and y.
     */
    M_PUBLIC_API Complex beta(const Variant& x, const Variant& y);

    /**
     * Function that calculates the beta function.
     *
     * \param[in] x The first beta function parameter.
     *
     * \param[in] y The second beta function parameter.
     *
     * \return Returns the beta function of x and y.
     */
    M_PUBLIC_API inline Complex beta(Boolean x, Boolean y) {
        return beta(Integer(x ? 1 : 0), Integer(y ? 1 : 0));
    }

    /**
     * Function that calculates the beta function.
     *
     * \param[in] x The first beta function parameter.
     *
     * \param[in] y The second beta function parameter.
     *
     * \return Returns the beta function of x and y.
     */
    M_PUBLIC_API inline Complex beta(Boolean x, Integer y) {
        return beta(Integer(x ? 1 : 0), y);
    }

    /**
     * Function that calculates the beta function.
     *
     * \param[in] x The first beta function parameter.
     *
     * \param[in] y The second beta function parameter.
     *
     * \return Returns the beta function of x and y.
     */
    M_PUBLIC_API inline Complex beta(Boolean x, Real y) {
        return beta(Integer(x ? 1 : 0), y);
    }

    /**
     * Function that calculates the beta function.
     *
     * \param[in] x The first beta function parameter.
     *
     * \param[in] y The second beta function parameter.
     *
     * \return Returns the beta function of x and y.
     */
    M_PUBLIC_API inline Complex beta(Boolean x, const Complex& y) {
        return beta(Integer(x ? 1 : 0), y);
    }

    /**
     * Function that calculates the beta function.
     *
     * \param[in] x The first beta function parameter.
     *
     * \param[in] y The second beta function parameter.
     *
     * \return Returns the beta function of x and y.
     */
    M_PUBLIC_API inline Complex beta(Boolean x, const Variant& y) {
        return beta(Integer(x ? 1 : 0), y);
    }

    /**
     * Function that calculates the beta function.
     *
     * \param[in] x The first beta function parameter.
     *
     * \param[in] y The second beta function parameter.
     *
     * \return Returns the beta function of x and y.
     */
    M_PUBLIC_API inline Complex beta(Integer x, Boolean y) {
        return beta(x, Integer(y ? 1 : 0));
    }

    /**
     * Function that calculates the beta function.
     *
     * \param[in] x The first beta function parameter.
     *
     * \param[in] y The second beta function parameter.
     *
     * \return Returns the beta function of x and y.
     */
    M_PUBLIC_API inline Complex beta(Real x, Boolean y) {
        return beta(x, Integer(y ? 1 : 0));
    }

    /**
     * Function that calculates the beta function.
     *
     * \param[in] x The first beta function parameter.
     *
     * \param[in] y The second beta function parameter.
     *
     * \return Returns the beta function of x and y.
     */
    M_PUBLIC_API inline Complex beta(const Complex& x, Boolean y) {
        return beta(x, Integer(y ? 1 : 0));
    }

    /**
     * Function that calculates the beta function.
     *
     * \param[in] x The first beta function parameter.
     *
     * \param[in] y The second beta function parameter.
     *
     * \return Returns the beta function of x and y.
     */
    M_PUBLIC_API inline Complex beta(const Variant& x, Boolean y) {
        return beta(x, Integer(y ? 1 : 0));
    }

    /**
     * Function that calculates the Reimann zeta function.
     *
     * \param[in] pt The per-thread instance used to generate the random deviate.
     *
     * \param[in] s  The value to calculate the Reimann zeta function over.
     *
     * \return Returns the result of the Reimann zeta function.
     */
    M_PUBLIC_API Real reimannZeta(PerThread& pt, Integer s);

    /**
     * Function that calculates the Reimann zeta function.
     *
     * \param[in] pt The per-thread instance used to generate the random deviate.
     *
     * \param[in] s  The value to calculate the Reimann zeta function over.
     *
     * \return Returns the result of the Reimann zeta function.
     */
    M_PUBLIC_API Real reimannZeta(PerThread& pt, Real s);

    /**
     * Function that calculates the Reimann zeta function.
     *
     * \param[in] pt The per-thread instance used to generate the random deviate.
     *
     * \param[in] s  The value to calculate the Reimann zeta function over.
     *
     * \return Returns the result of the Reimann zeta function.
     */
    M_PUBLIC_API Complex reimannZeta(PerThread& pt, const Complex& s);

    /**
     * Function that calculates the Reimann zeta function.
     *
     * \param[in] pt The per-thread instance used to generate the random deviate.
     *
     * \param[in] s  The value to calculate the Reimann zeta function over.
     *
     * \return Returns the result of the Reimann zeta function.
     */
    M_PUBLIC_API Complex reimannZeta(PerThread& pt, const Variant& s);

    /**
     * Function that calculates the error function.
     *
     * \param[in] x The value to calculate the error function from.
     *
     * \return Returns the error function of x.
     */
    M_PUBLIC_API Real erf(Integer x);

    /**
     * Function that calculates the error function.
     *
     * \param[in] x The value to calculate the error function from.
     *
     * \return Returns the error function of x.
     */
    M_PUBLIC_API Real erf(Real x);

    /**
     * Function that calculates the error function.
     *
     * \param[in] x The value to calculate the error function from.
     *
     * \return Returns the error function of x.
     */
    M_PUBLIC_API Complex erf(const Complex& x);

    /**
     * Function that calculates the error function.
     *
     * \param[in] x The value to calculate the error function from.
     *
     * \return Returns the error function of x.
     */
    M_PUBLIC_API inline Complex erf(Boolean x) {
        return erf(Integer(x ? 1 : 0));
    }

    /**
     * Function that calculates the error function.
     *
     * \param[in] x The value to calculate the error function from.
     *
     * \return Returns the error function of x.
     */
    M_PUBLIC_API inline Complex erf(const Variant& x) {
        return erf(x.toComplex());
    }

    /**
     * Function that calculates the complementary error function.
     *
     * \param[in] x The value to calculate the complementary error function from.
     *
     * \return Returns the error function of x.
     */
    M_PUBLIC_API Real erfc(Integer x);

    /**
     * Function that calculates the complementary error function.
     *
     * \param[in] x The value to calculate the complementary error function from.
     *
     * \return Returns the error function of x.
     */
    M_PUBLIC_API Real erfc(Real x);

    /**
     * Function that calculates the complementary error function.
     *
     * \param[in] x The value to calculate the complementary error function from.
     *
     * \return Returns the error function of x.
     */
    M_PUBLIC_API Complex erfc(const Complex& x);

    /**
     * Function that calculates the complementary error function.
     *
     * \param[in] x The value to calculate the complementary error function from.
     *
     * \return Returns the error function of x.
     */
    M_PUBLIC_API inline Complex erfc(const Variant& x) {
        return erfc(x.toComplex());
    }

    /**
     * Function that calculates the error function.
     *
     * \param[in] x The value to calculate the error function from.
     *
     * \return Returns the error function of x.
     */
    M_PUBLIC_API inline Complex erfc(Boolean x) {
        return erfc(Integer(x ? 1 : 0));
    }

    /**
     * Function that calculates the inverse error function.
     *
     * \param[in] x The value to calculate the inverse error function of.
     *
     * \return Returns the inverse error function of x.
     */
    M_PUBLIC_API Real erfInv(Real x);

    /**
     * Wrapper function for the inverse error function.
     *
     * \param[in] x The value to calculate the inverse error function of.
     *
     * \return Returns the inverse error function of x.
     */
    template<typename T1> M_PUBLIC_TEMPLATE_FUNCTION Real erfInv(T1 x) {
        Real result;

        Real lX;
        if (toReal(lX, x)) {
            result = erfInv(lX);
        } else {
            result = NaN;
        }

        return result;
    }

    /**
     * Function that calculates Lambert's W function.
     *
     * \param[in] k         The branch
     *
     * \param[in] z         The value to calculate Lambert's W from.
     *
     * \param[in] epsilon The maximum allowed relative error between terms during Holdholder method.
     *
     * \return Returns the result.
     */
    M_PUBLIC_API Complex lambertW(Integer k, Integer z, Real epsilon = defaultLambertWEpsilon);

    /**
     * Function that calculates Lambert's W function.
     *
     * \param[in] k         The branch
     *
     * \param[in] z         The value to calculate Lambert's W from.
     *
     * \param[in] epsilon The maximum allowed relative error between terms during Holdholder method.
     *
     * \return Returns the result.
     */
    M_PUBLIC_API Complex lambertW(Integer k, Real z, Real epsilon = defaultLambertWEpsilon);

    /**
     * Function that calculates Lambert's W function.
     *
     * \param[in] k         The branch
     *
     * \param[in] z         The value to calculate Lambert's W from.
     *
     * \param[in] epsilon The maximum allowed relative error between terms during Holdholder method.
     *
     * \return Returns the result.
     */
    M_PUBLIC_API Complex lambertW(Integer k, const Complex& z, Real epsilon = defaultLambertWEpsilon);

    /**
     * Function that calculates Lambert's W function.
     *
     * \param[in] k         The branch
     *
     * \param[in] z         The value to calculate Lambert's W from.
     *
     * \param[in] epsilon The maximum allowed relative error between terms during Holdholder method.
     *
     * \return Returns the result.
     */
    M_PUBLIC_API Complex lambertW(Real k, Integer z, Real epsilon = defaultLambertWEpsilon);

    /**
     * Function that calculates Lambert's W function.
     *
     * \param[in] k         The branch
     *
     * \param[in] z         The value to calculate Lambert's W from.
     *
     * \param[in] epsilon The maximum allowed relative error between terms during Holdholder method.
     *
     * \return Returns the result.
     */
    M_PUBLIC_API Complex lambertW(Real k, Real z, Real epsilon = defaultLambertWEpsilon);

    /**
     * Function that calculates Lambert's W function.
     *
     * \param[in] k         The branch
     *
     * \param[in] z         The value to calculate Lambert's W from.
     *
     * \param[in] epsilon The maximum allowed relative error between terms during Holdholder method.
     *
     * \return Returns the result.
     */
    M_PUBLIC_API Complex lambertW(Real k, const Complex& z, Real epsilon = defaultLambertWEpsilon);

    /**
     * Function that calculates Lambert's W function.
     *
     * \param[in] k         The branch
     *
     * \param[in] z         The value to calculate Lambert's W from.
     *
     * \param[in] epsilon The maximum allowed relative error between terms during Holdholder method.
     *
     * \return Returns the result.
     */
    M_PUBLIC_API Complex lambertW(const Complex& k, Integer z, Real epsilon = defaultLambertWEpsilon);

    /**
     * Function that calculates Lambert's W function.
     *
     * \param[in] k         The branch
     *
     * \param[in] z         The value to calculate Lambert's W from.
     *
     * \param[in] epsilon The maximum allowed relative error between terms during Holdholder method.
     *
     * \return Returns the result.
     */
    M_PUBLIC_API Complex lambertW(const Complex& k, Real z, Real epsilon = defaultLambertWEpsilon);

    /**
     * Function that calculates Lambert's W function.
     *
     * \param[in] k         The branch
     *
     * \param[in] z         The value to calculate Lambert's W from.
     *
     * \param[in] epsilon The maximum allowed relative error between terms during Holdholder method.
     *
     * \return Returns the result.
     */
    M_PUBLIC_API Complex lambertW(const Complex& k, const Complex& z, Real epsilon = defaultLambertWEpsilon);

    /**
     * Function that calculates Lambert's W function.
     *
     * \param[in] k         The branch
     *
     * \param[in] z         The value to calculate Lambert's W from.
     *
     * \param[in] epsilon The maximum allowed relative error between terms during Holdholder method.
     *
     * \return Returns the result.
     */
    M_PUBLIC_API inline Complex lambertW(const Variant& k, Integer z, Real epsilon = defaultLambertWEpsilon) {
        return lambertW(k.toInteger(), z, epsilon);
    }

    /**
     * Function that calculates Lambert's W function.
     *
     * \param[in] k         The branch
     *
     * \param[in] z         The value to calculate Lambert's W from.
     *
     * \param[in] epsilon The maximum allowed relative error between terms during Holdholder method.
     *
     * \return Returns the result.
     */
    M_PUBLIC_API inline Complex lambertW(const Variant& k, Real z, Real epsilon = defaultLambertWEpsilon) {
        return lambertW(k.toInteger(), z, epsilon);
    }

    /**
     * Function that calculates Lambert's W function.
     *
     * \param[in] k         The branch
     *
     * \param[in] z         The value to calculate Lambert's W from.
     *
     * \param[in] epsilon The maximum allowed relative error between terms during Holdholder method.
     *
     * \return Returns the result.
     */
    M_PUBLIC_API inline Complex lambertW(const Variant& k, const Complex& z, Real epsilon = defaultLambertWEpsilon) {
        return lambertW(k.toInteger(), z, epsilon);
    }

    /**
     * Function that calculates Lambert's W function.
     *
     * \param[in] k         The branch
     *
     * \param[in] z         The value to calculate Lambert's W from.
     *
     * \param[in] epsilon The maximum allowed relative error between terms during Holdholder method.
     *
     * \return Returns the result.
     */
    M_PUBLIC_API inline Complex lambertW(Integer k, const Variant& z, Real epsilon = defaultLambertWEpsilon) {
        return lambertW(k, z.toComplex(), epsilon);
    }

    /**
     * Function that calculates Lambert's W function.
     *
     * \param[in] k         The branch
     *
     * \param[in] z         The value to calculate Lambert's W from.
     *
     * \param[in] epsilon The maximum allowed relative error between terms during Holdholder method.
     *
     * \return Returns the result.
     */
    M_PUBLIC_API inline Complex lambertW(Real k, const Variant& z, Real epsilon = defaultLambertWEpsilon) {
        return lambertW(k, z.toComplex(), epsilon);
    }

    /**
     * Function that calculates Lambert's W function.
     *
     * \param[in] k         The branch
     *
     * \param[in] z         The value to calculate Lambert's W from.
     *
     * \param[in] epsilon The maximum allowed relative error between terms during Holdholder method.
     *
     * \return Returns the result.
     */
    M_PUBLIC_API inline Complex lambertW(const Complex& k, const Variant& z, Real epsilon = defaultLambertWEpsilon) {
        return lambertW(k, z.toComplex(), epsilon);
    }

    /**
     * Function that calculates Lambert's W function.
     *
     * \param[in] k         The branch
     *
     * \param[in] z         The value to calculate Lambert's W from.
     *
     * \param[in] epsilon The maximum allowed relative error between terms during Holdholder method.
     *
     * \return Returns the result.
     */
    M_PUBLIC_API inline Complex lambertW(const Variant& k, const Variant& z, Real epsilon = defaultLambertWEpsilon) {
        return lambertW(k.toInteger(), z.toComplex(), epsilon);
    }

    /**
     * Function that calculates Lambert's W function.
     *
     * \param[in] k         The branch
     *
     * \param[in] z         The value to calculate Lambert's W from.
     *
     * \param[in] epsilon The maximum allowed relative error between terms during Holdholder method.
     *
     * \return Returns the result.
     */
    M_PUBLIC_API inline Complex lambertW(Boolean k, Boolean z, Real epsilon = defaultLambertWEpsilon) {
        return lambertW(Integer(k ? 1 : 0), Integer(z ? 1 : 0), epsilon);
    }

    /**
     * Function that calculates Lambert's W function.
     *
     * \param[in] k         The branch
     *
     * \param[in] z         The value to calculate Lambert's W from.
     *
     * \param[in] epsilon The maximum allowed relative error between terms during Holdholder method.
     *
     * \return Returns the result.
     */
    M_PUBLIC_API inline Complex lambertW(Boolean k, Integer z, Real epsilon = defaultLambertWEpsilon) {
        return lambertW(Integer(k ? 1 : 0), z, epsilon);
    }

    /**
     * Function that calculates Lambert's W function.
     *
     * \param[in] k         The branch
     *
     * \param[in] z         The value to calculate Lambert's W from.
     *
     * \param[in] epsilon The maximum allowed relative error between terms during Holdholder method.
     *
     * \return Returns the result.
     */
    M_PUBLIC_API inline Complex lambertW(Boolean k, Real z, Real epsilon = defaultLambertWEpsilon) {
        return lambertW(Integer(k ? 1 : 0), z, epsilon);
    }

    /**
     * Function that calculates Lambert's W function.
     *
     * \param[in] k         The branch
     *
     * \param[in] z         The value to calculate Lambert's W from.
     *
     * \param[in] epsilon The maximum allowed relative error between terms during Holdholder method.
     *
     * \return Returns the result.
     */
    M_PUBLIC_API inline Complex lambertW(Boolean k, const Complex& z, Real epsilon = defaultLambertWEpsilon) {
        return lambertW(Integer(k ? 1 : 0), z, epsilon);
    }

    /**
     * Function that calculates Lambert's W function.
     *
     * \param[in] k         The branch
     *
     * \param[in] z         The value to calculate Lambert's W from.
     *
     * \param[in] epsilon The maximum allowed relative error between terms during Holdholder method.
     *
     * \return Returns the result.
     */
    M_PUBLIC_API inline Complex lambertW(Boolean k, const Variant& z, Real epsilon = defaultLambertWEpsilon) {
        return lambertW(Integer(k ? 1 : 0), z, epsilon);
    }

    /**
     * Function that calculates Lambert's W function.
     *
     * \param[in] k         The branch
     *
     * \param[in] z         The value to calculate Lambert's W from.
     *
     * \param[in] epsilon The maximum allowed relative error between terms during Holdholder method.
     *
     * \return Returns the result.
     */
    M_PUBLIC_API inline Complex lambertW(Integer k, Boolean z, Real epsilon = defaultLambertWEpsilon) {
        return lambertW(k, Integer(z ? 1 : 0), epsilon);
    }

    /**
     * Function that calculates Lambert's W function.
     *
     * \param[in] k         The branch
     *
     * \param[in] z         The value to calculate Lambert's W from.
     *
     * \param[in] epsilon The maximum allowed relative error between terms during Holdholder method.
     *
     * \return Returns the result.
     */
    M_PUBLIC_API inline Complex lambertW(Real k, Boolean z, Real epsilon = defaultLambertWEpsilon) {
        return lambertW(k, Integer(z ? 1 : 0), epsilon);
    }

    /**
     * Function that calculates Lambert's W function.
     *
     * \param[in] k         The branch
     *
     * \param[in] z         The value to calculate Lambert's W from.
     *
     * \param[in] epsilon The maximum allowed relative error between terms during Holdholder method.
     *
     * \return Returns the result.
     */
    M_PUBLIC_API inline Complex lambertW(const Complex& k, Boolean z, Real epsilon = defaultLambertWEpsilon) {
        return lambertW(k, Integer(z ? 1 : 0), epsilon);
    }

    /**
     * Function that calculates Lambert's W function.
     *
     * \param[in] k         The branch
     *
     * \param[in] z         The value to calculate Lambert's W from.
     *
     * \param[in] epsilon The maximum allowed relative error between terms during Holdholder method.
     *
     * \return Returns the result.
     */
    M_PUBLIC_API inline Complex lambertW(const Variant& k, Boolean z, Real epsilon = defaultLambertWEpsilon) {
        return lambertW(k, Integer(z ? 1 : 0), epsilon);
    }
}

#endif
