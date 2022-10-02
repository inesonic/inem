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
* This header provides functions to assist with type conversions.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_TYPE_CONVERSION_H
#define M_TYPE_CONVERSION_H

#include "m_common.h"
#include "m_api_types.h"
#include "m_intrinsic_types.h"
#include "m_exceptions.h"

namespace M {
    class Variant;
    class Set;
    class Tuple;
    class MatrixBoolean;
    class MatrixInteger;
    class MatrixReal;
    class MatrixComplex;

    /**
     * Method that converts a type to an integer.
     *
     * \param[in]  inputValue The input data type.
     *
     * \param[out] result     The converted value.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API inline bool toInteger(Real& result, Boolean inputValue) {
        result = static_cast<Integer>(inputValue);
        return true;
    }

    /**
     * Method that converts a type to an integer.
     *
     * \param[in]  inputValue The input data type.
     *
     * \param[out] result     The converted value.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API inline bool toInteger(Integer& result, Integer inputValue) {
        result = inputValue;
        return true;
    }

    /**
     * Method that converts a type to an integer.
     *
     * \param[in]  inputValue The input data type.
     *
     * \param[out] result     The converted value.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API bool toInteger(Integer& result, Real inputValue);

    /**
     * Method that converts a type to an integer.
     *
     * \param[in]  inputValue The input data type.
     *
     * \param[out] result     The converted value.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API bool toInteger(Integer& result, const Complex& inputValue);

    /**
     * Method that converts a type to an integer.
     *
     * \param[in]  inputValue The input data type.
     *
     * \param[out] result     The converted value.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API bool toInteger(Integer& result, const Variant& inputValue);

    /**
     * Method that converts a type to an integer.
     *
     * \param[in]  inputValue The input data type.
     *
     * \param[out] result     The converted value.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API inline bool toReal(Real& result, Boolean inputValue) {
        result = static_cast<Real>(inputValue);
        return true;
    }

    /**
     * Method that converts a type to an integer.
     *
     * \param[in]  inputValue The input data type.
     *
     * \param[out] result     The converted value.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API inline bool toReal(Real& result, Integer inputValue) {
        result = static_cast<Real>(inputValue);
        return true;
    }

    /**
     * Method that converts a type to an integer.
     *
     * \param[in]  inputValue The input data type.
     *
     * \param[out] result     The converted value.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API inline bool toReal(Real& result, Real inputValue) {
        result = inputValue;
        return true;
    }

    /**
     * Method that converts a type to an integer.
     *
     * \param[in]  inputValue The input data type.
     *
     * \param[out] result     The converted value.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API bool toReal(Real& result, const Complex& inputValue);

    /**
     * Method that converts a type to an integer.
     *
     * \param[in]  inputValue The input data type.
     *
     * \param[out] result     The converted value.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API bool toReal(Real& result, const Variant& inputValue);

    /**
     * Method that converts a type to an integer.
     *
     * \param[in]  inputValue The input data type.
     *
     * \param[out] result     The converted value.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API inline bool toComplex(Complex& result, Boolean inputValue) {
        result = inputValue;
        return true;
    }

    /**
     * Method that converts a type to an integer.
     *
     * \param[in]  inputValue The input data type.
     *
     * \param[out] result     The converted value.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API inline bool toComplex(Complex& result, Integer inputValue) {
        result = inputValue;
        return true;
    }

    /**
     * Method that converts a type to an integer.
     *
     * \param[in]  inputValue The input data type.
     *
     * \param[out] result     The converted value.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API inline bool toComplex(Complex& result, Real inputValue) {
        result = inputValue;
        return true;
    }

    /**
     * Method that converts a type to an integer.
     *
     * \param[in]  inputValue The input data type.
     *
     * \param[out] result     The converted value.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API inline bool toComplex(Complex& result, const Complex& inputValue) {
        result = inputValue;
        return true;
    }

    /**
     * Method that converts a type to an integer.
     *
     * \param[in]  inputValue The input data type.
     *
     * \param[out] result     The converted value.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API bool toComplex(Complex& result, const Variant& inputValue);

    /**
     * Method that identifies the type of a variable.
     *
     * \param[in] value The variable to be identified.
     *
     * \return Returns the value type of the variable.
     */
    M_PUBLIC_API inline ValueType valueTypeOf(Boolean value) {
        (void) value;
        return ValueType::BOOLEAN;
    }

    /**
     * Method that identifies the type of a variable.
     *
     * \param[in] value The variable to be identified.
     *
     * \return Returns the value type of the variable.
     */
    M_PUBLIC_API inline ValueType valueTypeOf(Integer value) {
        (void) value;
        return ValueType::INTEGER;
    }

    /**
     * Method that identifies the type of a variable.
     *
     * \param[in] value The variable to be identified.
     *
     * \return Returns the value type of the variable.
     */
    M_PUBLIC_API inline ValueType valueTypeOf(Real value) {
        (void) value;
        return ValueType::REAL;
    }

    /**
     * Method that identifies the type of a variable.
     *
     * \param[in] value The variable to be identified.
     *
     * \return Returns the value type of the variable.
     */
    M_PUBLIC_API inline ValueType valueTypeOf(const M::Complex& value) {
        (void) value;
        return ValueType::COMPLEX;
    }

    /**
     * Method that identifies the type of a variable.
     *
     * \param[in] value The variable to be identified.
     *
     * \return Returns the value type of the variable.
     */
    M_PUBLIC_API inline ValueType valueTypeOf(const M::Set& value) {
        (void) value;
        return ValueType::SET;
    }

    /**
     * Method that identifies the type of a variable.
     *
     * \param[in] value The variable to be identified.
     *
     * \return Returns the value type of the variable.
     */
    M_PUBLIC_API inline ValueType valueTypeOf(const M::Tuple& value) {
        (void) value;
        return ValueType::TUPLE;
    }

    /**
     * Method that identifies the type of a variable.
     *
     * \param[in] value The variable to be identified.
     *
     * \return Returns the value type of the variable.
     */
    M_PUBLIC_API inline ValueType valueTypeOf(const M::MatrixBoolean& value) {
        (void) value;
        return ValueType::MATRIX_BOOLEAN;
    }

    /**
     * Method that identifies the type of a variable.
     *
     * \param[in] value The variable to be identified.
     *
     * \return Returns the value type of the variable.
     */
    M_PUBLIC_API inline ValueType valueTypeOf(const M::MatrixInteger& value) {
        (void) value;
        return ValueType::MATRIX_INTEGER;
    }

    /**
     * Method that identifies the type of a variable.
     *
     * \param[in] value The variable to be identified.
     *
     * \return Returns the value type of the variable.
     */
    M_PUBLIC_API inline ValueType valueTypeOf(const M::MatrixReal& value) {
        (void) value;
        return ValueType::MATRIX_REAL;
    }

    /**
     * Method that identifies the type of a variable.
     *
     * \param[in] value The variable to be identified.
     *
     * \return Returns the value type of the variable.
     */
    M_PUBLIC_API inline ValueType valueTypeOf(const M::MatrixComplex& value) {
        (void) value;
        return ValueType::MATRIX_COMPLEX;
    }

    /**
     * Method that identifies the type of a variable.
     *
     * \param[in] value The variable to be identified.
     *
     * \return Returns the value type of the variable.
     */
    M_PUBLIC_API ValueType valueTypeOf(const Variant& value);

    /**
     * Method that performs in-line conversion to boolean values with an exception thrown if the conversion is not
     * possible.
     *
     * \param[in] inputValue The input data type
     *
     * \return Returns the value converted to a boolean value.
     */
    M_PUBLIC_API inline Boolean inlineToBoolean(Boolean inputValue) {
        return inputValue;
    }

    /**
     * Method that performs in-line conversion to boolean values with an exception thrown if the conversion is not
     * possible.
     *
     * \param[in] inputValue The input data type
     *
     * \return Returns the value converted to a boolean value.
     */
    M_PUBLIC_API inline Boolean inlineToBoolean(Integer inputValue) {
        return inputValue != 0 ? true : false;
    }

    /**
     * Method that performs in-line conversion to boolean values with an exception thrown if the conversion is not
     * possible.
     *
     * \param[in] inputValue The input data type
     *
     * \return Returns the value converted to a boolean value.
     */
    M_PUBLIC_API inline Boolean inlineToBoolean(Real inputValue) {
        return inputValue != 0 ? true : false;
    }

    /**
     * Method that performs in-line conversion to boolean values with an exception thrown if the conversion is not
     * possible.
     *
     * \param[in] inputValue The input data type
     *
     * \return Returns the value converted to a boolean value.
     */
    M_PUBLIC_API inline Boolean inlineToBoolean(const Complex& inputValue) {
        return inputValue.real() != 0 || inputValue.imag() != 0 ? true : false;
    }

    /**
     * Method that performs in-line conversion to boolean values with an exception thrown if the conversion is not
     * possible.
     *
     * \param[in] inputValue The input data type
     *
     * \return Returns the value converted to a boolean value.
     */
    M_PUBLIC_API Boolean inlineToBoolean(const Variant& inputValue);

    /**
     * Method that performs in-line conversion to integer values with an exception thrown if the conversion is not
     * possible.
     *
     * \param[in] inputValue The input data type
     *
     * \return Returns the value converted to an integer value.
     */
    M_PUBLIC_API inline Integer inlineToInteger(Boolean inputValue) {
        return inputValue ? Integer(1) : Integer(0);
    }

    /**
     * Method that performs in-line conversion to integer values with an exception thrown if the conversion is not
     * possible.
     *
     * \param[in] inputValue The input data type
     *
     * \return Returns the value converted to an integer value.
     */
    M_PUBLIC_API inline Integer inlineToInteger(int inputValue) {
        return inputValue;
    }

    /**
     * Method that performs in-line conversion to integer values with an exception thrown if the conversion is not
     * possible.
     *
     * \param[in] inputValue The input data type
     *
     * \return Returns the value converted to an integer value.
     */
    M_PUBLIC_API inline Integer inlineToInteger(Integer inputValue) {
        return inputValue;
    }

    /**
     * Method that performs in-line conversion to integer values with an exception thrown if the conversion is not
     * possible.
     *
     * \param[in] inputValue The input data type
     *
     * \return Returns the value converted to an integer value.
     */
    M_PUBLIC_API inline Integer inlineToInteger(Real inputValue) {
        Integer result;
        if (!toInteger(result, inputValue)) {
            internalTriggerTypeConversionError(ValueType::REAL, ValueType::INTEGER);
        }

        return result;
    }

    /**
     * Method that performs in-line conversion to integer values with an exception thrown if the conversion is not
     * possible.
     *
     * \param[in] inputValue The input data type
     *
     * \return Returns the value converted to an integer value.
     */
    M_PUBLIC_API inline Integer inlineToInteger(const Complex& inputValue) {
        Integer result;
        if (!toInteger(result, inputValue)) {
            internalTriggerTypeConversionError(ValueType::COMPLEX, ValueType::INTEGER);
        }

        return result;
    }

    /**
     * Method that performs in-line conversion to integer values with an exception thrown if the conversion is not
     * possible.
     *
     * \param[in] inputValue The input data type
     *
     * \return Returns the value converted to an integer value.
     */
    M_PUBLIC_API Integer inlineToInteger(const Variant& inputValue);

    /**
     * Method that performs in-line conversion to real values with an exception thrown if the conversion is not
     * possible.
     *
     * \param[in] inputValue The input data type
     *
     * \return Returns the value converted to a real value.
     */
    M_PUBLIC_API inline Real inlineToReal(Boolean inputValue) {
        return inputValue ? Real(1) : Real(0);
    }

    /**
     * Method that performs in-line conversion to real values with an exception thrown if the conversion is not
     * possible.
     *
     * \param[in] inputValue The input data type
     *
     * \return Returns the value converted to a real value.
     */
    M_PUBLIC_API inline Real inlineToReal(Integer inputValue) {
        return static_cast<Real>(inputValue);
    }

    /**
     * Method that performs in-line conversion to real values with an exception thrown if the conversion is not
     * possible.
     *
     * \param[in] inputValue The input data type
     *
     * \return Returns the value converted to a real value.
     */
    M_PUBLIC_API inline Real inlineToReal(int inputValue) {
        return static_cast<Real>(inputValue);
    }

    /**
     * Method that performs in-line conversion to real values with an exception thrown if the conversion is not
     * possible.
     *
     * \param[in] inputValue The input data type
     *
     * \return Returns the value converted to a real value.
     */
    M_PUBLIC_API inline Real inlineToReal(Real inputValue) {
        return inputValue;
    }

    /**
     * Method that performs in-line conversion to real values with an exception thrown if the conversion is not
     * possible.
     *
     * \param[in] inputValue The input data type
     *
     * \return Returns the value converted to a real value.
     */
    M_PUBLIC_API inline Real inlineToReal(const Complex& inputValue) {
        Real result;
        if (!toReal(result, inputValue)) {
            internalTriggerTypeConversionError(ValueType::COMPLEX, ValueType::REAL);
        }

        return result;
    }

    /**
     * Method that performs in-line conversion to real values with an exception thrown if the conversion is not
     * possible.
     *
     * \param[in] inputValue The input data type
     *
     * \return Returns the value converted to a real value.
     */
    M_PUBLIC_API Real inlineToReal(const Variant& inputValue);

    /**
     * Method that performs in-line conversion to complex values with an exception thrown if the conversion is not
     * possible.
     *
     * \param[in] inputValue The input data type
     *
     * \return Returns the value converted to a complex value.
     */
    M_PUBLIC_API inline Complex inlineToComplex(Boolean inputValue) {
        return inputValue ? Complex(1) : Complex(0);
    }

    /**
     * Method that performs in-line conversion to complex values with an exception thrown if the conversion is not
     * possible.
     *
     * \param[in] inputValue The input data type
     *
     * \return Returns the value converted to a complex value.
     */
    M_PUBLIC_API inline Complex inlineToComplex(Integer inputValue) {
        return Complex(inputValue);
    }

    /**
     * Method that performs in-line conversion to complex values with an exception thrown if the conversion is not
     * possible.
     *
     * \param[in] inputValue The input data type
     *
     * \return Returns the value converted to a complex value.
     */
    M_PUBLIC_API inline Complex inlineToComplex(Real inputValue) {
        return Complex(inputValue);
    }

    /**
     * Method that performs in-line conversion to complex values with an exception thrown if the conversion is not
     * possible.
     *
     * \param[in] inputValue The input data type
     *
     * \return Returns the value converted to a complex value.
     */
    M_PUBLIC_API inline const Complex& inlineToComplex(const Complex& inputValue) {
        return inputValue;
    }

    /**
     * Method that performs in-line conversion to complex values with an exception thrown if the conversion is not
     * possible.
     *
     * \param[in] inputValue The input data type
     *
     * \return Returns the value converted to a complex value.
     */
    M_PUBLIC_API Complex inlineToComplex(const Variant& inputValue);
}

#endif


