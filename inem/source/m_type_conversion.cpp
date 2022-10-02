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
* This file implements functions to assist with type conversions.
***********************************************************************************************************************/

#include <cmath>
#include <limits>

#include "model_exceptions.h"
#include "m_api_types.h"
#include "m_exceptions.h"
#include "m_variant.h"
#include "m_type_conversion.h"

namespace M {
    bool toInteger(Integer& result, Real inputValue) {
        Real wholePart;
        if (std::modf(inputValue, &wholePart) != 0) {
            internalTriggerTypeConversionError(ValueType::REAL, ValueType::INTEGER);
            return false;
        } else {
            result = static_cast<Integer>(wholePart);
            return true;
        }
    }


    bool toInteger(Integer& result, const Complex& inputValue) {
        Real wholePart;
        if (inputValue.imag() != 0 || std::modf(inputValue.real(), &wholePart) != 0) {
            internalTriggerTypeConversionError(ValueType::COMPLEX, ValueType::INTEGER);
            return false;
        } else {
            result = static_cast<Integer>(wholePart);
            return true;
        }
    }


    bool toInteger(Integer& result, const Variant& inputValue) {
        bool success;
        result = inputValue.toInteger(&success);

        if (!success) {
            internalTriggerTypeConversionError(inputValue.valueType(), ValueType::INTEGER);
        }

        return success;
    }


    bool toReal(Real& result, const Complex& inputValue) {
        if (inputValue.imag() != 0) {
            internalTriggerTypeConversionError(ValueType::COMPLEX, ValueType::REAL);
            return false;
        } else {
            result = inputValue.real();
            return true;
        }
    }


    bool toReal(Real& result, const Variant& inputValue) {
        bool success;
        result = inputValue.toReal(&success);

        if (!success) {
            internalTriggerTypeConversionError(inputValue.valueType(), ValueType::REAL);
        }

        return success;
    }


    bool toComplex(Complex& result, const Variant& inputValue) {
        bool success;
        result = inputValue.toComplex(&success);

        if (!success) {
            internalTriggerTypeConversionError(inputValue.valueType(), ValueType::COMPLEX);
        }

        return success;
    }


    ValueType valueTypeOf(const M::Variant& value) {
        return value.valueType();
    }


    M_PUBLIC_API Boolean inlineToBoolean(const Variant& inputValue) {
        bool    ok;
        Boolean result = inputValue.toBoolean(&ok);

        if (!ok) {
            internalTriggerTypeConversionError(inputValue.valueType(), ValueType::BOOLEAN);
            result = false;
        }

        return result;
    }


    M_PUBLIC_API Integer inlineToInteger(const Variant& inputValue) {
        bool    ok;
        Integer result = inputValue.toInteger(&ok);

        if (!ok) {
            internalTriggerTypeConversionError(inputValue.valueType(), ValueType::INTEGER);
            result = 0;
        }

        return result;
    }


    M_PUBLIC_API Real inlineToReal(const Variant& inputValue) {
        bool ok;
        Real result = inputValue.toReal(&ok);

        if (!ok) {
            internalTriggerTypeConversionError(inputValue.valueType(), ValueType::REAL);
            result = std::numeric_limits<Real>::quiet_NaN();
        }

        return result;
    }


    M_PUBLIC_API Complex inlineToComplex(const Variant& inputValue) {
        bool    ok;
        Complex result = inputValue.toReal(&ok);

        if (!ok) {
            internalTriggerTypeConversionError(inputValue.valueType(), ValueType::COMPLEX);
            result = Complex(std::numeric_limits<Real>::quiet_NaN(), std::numeric_limits<Real>::quiet_NaN());
        }

        return result;
    }
}
