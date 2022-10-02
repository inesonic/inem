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
* This header defines operators you can use with the \ref Variant class.
***********************************************************************************************************************/

#include <cstring>
#include <cmath>
#include <cassert>
#include <limits>
#include <complex>

#include "m_intrinsic_types.h"
#include "m_api_types.h"
#include "m_exceptions.h"
#include "m_intrinsic_types.h"
#include "m_set.h"
#include "m_tuple.h"
#include "m_matrix_boolean.h"
#include "m_matrix_integer.h"
#include "m_matrix_real.h"
#include "m_matrix_complex.h"
#include "m_variant.h"
#include "m_variant_operators.h"

namespace M {
    bool operator==(const Variant& a, Boolean b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::BOOLEAN);
                return false;
            }

            case ValueType::BOOLEAN: {
                return a.toBoolean() == (b ? true : false);
            }

            case ValueType::INTEGER: {
                return a.toInteger() == (b ? 1 : 0);
            }

            case ValueType::REAL: {
                return a.toReal() == (b ? 1 : 0);
            }

            case ValueType::COMPLEX: {
                return a.toComplex() == Complex(b ? 1 : 0);
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool operator==(const Variant& a, Integer b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::INTEGER);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return a.toInteger() == b;
            }

            case ValueType::REAL: {
                return a.toReal() == Real(b);
            }

            case ValueType::COMPLEX: {
                return a.toComplex() == Complex(b);
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool operator==(const Variant& a, Real b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::REAL);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL: {
                return a.toReal() == b;
            }

            case ValueType::COMPLEX: {
                return a.toComplex() == Complex(b);
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool operator==(const Variant& a, const Complex& b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::COMPLEX);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return a.toComplex() == b;
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool operator==(const Variant& a, const Set& b) {
        return a.toSet() == b;
    }


    bool operator==(const Variant& a, const Tuple& b) {
        return a.toTuple() == b;
    }


    bool operator==(const Variant& a, const MatrixBoolean& b) {
        return a.toMatrixBoolean() == b;
    }


    bool operator==(const Variant& a, const MatrixInteger& b) {
        return a.toMatrixInteger() == b;
    }


    bool operator==(const Variant& a, const MatrixReal& b) {
        return a.toMatrixReal() == b;
    }


    bool operator==(const Variant& a, const MatrixComplex& b) {
        return a.toMatrixComplex() == b;
    }


    bool operator<(const Variant& a, Boolean b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::BOOLEAN);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return a.toInteger() < (b ? 1 : 0);
            }

            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return a.toReal() < (b ? 1 : 0);
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool operator<(const Variant& a, Integer b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::INTEGER);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return a.toInteger() < b;
            }

            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return a.toReal() < Real(b);
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool operator<(const Variant& a, Real b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::REAL);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return a.toReal() < b;
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool operator<(const Variant& a, const Complex& b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::REAL);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                if (b.imag() == 0) {
                    return a.toReal() < b.real();
                } else {
                    internalTriggerTypeConversionError(ValueType::COMPLEX, ValueType::REAL);
                    return false;
                }
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool operator>(const Variant& a, Boolean b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::BOOLEAN);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return a.toInteger() > (b ? 1 : 0);
            }

            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return a.toReal() > (b ? 1 : 0);
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool operator>(const Variant& a, Integer b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::INTEGER);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return a.toInteger() > b;
            }

            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return a.toReal() > Real(b);
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool operator>(const Variant& a, Real b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::REAL);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return a.toReal() > b;
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool operator>(const Variant& a, const Complex& b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::REAL);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                if (b.imag() == 0) {
                    return a.toReal() > b.real();
                } else {
                    internalTriggerTypeConversionError(ValueType::COMPLEX, ValueType::REAL);
                    return false;
                }
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    Variant operator+(const Variant& a, Boolean b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::BOOLEAN);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return Variant(a.toInteger() + static_cast<Integer>(b ? 1 : 0));
            }

            case ValueType::REAL: {
                return Variant(a.toReal() + static_cast<Real>(b ? 1 : 0));
            }

            case ValueType::COMPLEX: {
                return Variant(a.toComplex() + Complex(b ? 1 : 0));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator+(const Variant& a, Integer b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::INTEGER);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return Variant(a.toInteger() + b);
            }

            case ValueType::REAL: {
                return Variant(a.toReal() + static_cast<Real>(b));
            }

            case ValueType::COMPLEX: {
                return Variant(a.toComplex() + Complex(b));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator+(const Variant& a, Real b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::REAL);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL: {
                return Variant(a.toReal() + b);
            }

            case ValueType::COMPLEX: {
                return Variant(a.toComplex() + Complex(b));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Complex operator+(const Variant& a, const Complex& b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::COMPLEX);
                return Complex(std::numeric_limits<Real>::quiet_NaN(), std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return a.toComplex() + b;
            }

            default: {
                assert(false);
                return Complex(std::numeric_limits<Real>::quiet_NaN(), std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator+(const Variant& a, const MatrixBoolean& b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::MATRIX_BOOLEAN);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER: {
                return Variant(a.toMatrixInteger() + MatrixInteger(b));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(a.toMatrixReal() + MatrixReal(b));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(a.toMatrixComplex() + MatrixComplex(b));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator+(const Variant& a, const MatrixInteger& b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::MATRIX_INTEGER);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER: {
                return Variant(a.toMatrixInteger() + b);
            }

            case ValueType::MATRIX_REAL: {
                return Variant(a.toMatrixReal() + MatrixReal(b));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(a.toMatrixComplex() + MatrixComplex(b));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator+(const Variant& a, const MatrixReal& b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::MATRIX_REAL);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(a.toMatrixReal() + b);
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(a.toMatrixComplex() + MatrixComplex(b));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    MatrixComplex operator+(const Variant& a, const MatrixComplex& b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::MATRIX_COMPLEX);
                return MatrixComplex();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                return a.toMatrixComplex() + b;
            }

            default: {
                assert(false);
                return MatrixComplex();
            }
        }
    }


    Variant operator+(const Variant& a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerTypeConversionError(a.valueType(), bType);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return operator+(a, b.toInteger());
            }

            case ValueType::REAL: {
                return operator+(a, b.toReal());
            }

            case ValueType::COMPLEX: {
                return Variant(operator+(a, b.toComplex()));
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER: {
                return operator+(a, b.toMatrixInteger());
            }

            case ValueType::MATRIX_REAL: {
                return operator+(a, b.toMatrixReal());
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(operator+(a, b.toMatrixComplex()));
            }

            default: {
                assert(false);
                break;
            }
        }

        return Variant(std::numeric_limits<Real>::quiet_NaN());
    }


    Variant operator-(const Variant& a, Boolean b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::INTEGER);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return Variant(a.toInteger() - static_cast<Integer>(b ? 1 : 0));
            }

            case ValueType::REAL: {
                return Variant(a.toReal() - static_cast<Real>(b ? 1 : 0));
            }

            case ValueType::COMPLEX: {
                return Variant(a.toComplex() - Complex(b ? 1 : 0));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator-(const Variant& a, Integer b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::INTEGER);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return Variant(a.toInteger() - b);
            }

            case ValueType::REAL: {
                return Variant(a.toReal() - static_cast<Real>(b));
            }

            case ValueType::COMPLEX: {
                return Variant(a.toComplex() - Complex(b));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator-(const Variant& a, Real b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::REAL);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL: {
                return Variant(a.toReal() - b);
            }

            case ValueType::COMPLEX: {
                return Variant(a.toComplex() - Complex(b));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Complex operator-(const Variant& a, const Complex& b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::COMPLEX);
                return Complex(std::numeric_limits<Real>::quiet_NaN(), std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return a.toComplex() - b;
            }

            default: {
                assert(false);
                return Complex(std::numeric_limits<Real>::quiet_NaN(), std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator-(const Variant& a, const MatrixBoolean& b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::MATRIX_BOOLEAN);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER: {
                return Variant(a.toMatrixInteger() - MatrixInteger(b));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(a.toMatrixReal() - MatrixReal(b));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(a.toMatrixComplex() - MatrixComplex(b));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator-(const Variant& a, const MatrixInteger& b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::MATRIX_INTEGER);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER: {
                return Variant(a.toMatrixInteger() - b);
            }

            case ValueType::MATRIX_REAL: {
                return Variant(a.toMatrixReal() - MatrixReal(b));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(a.toMatrixComplex() - MatrixComplex(b));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator-(const Variant& a, const MatrixReal& b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::MATRIX_REAL);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(a.toMatrixReal() - b);
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(a.toMatrixComplex() - MatrixComplex(b));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    MatrixComplex operator-(const Variant& a, const MatrixComplex& b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::MATRIX_COMPLEX);
                return MatrixComplex();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                return a.toMatrixComplex() - b;
            }

            default: {
                assert(false);
                return MatrixComplex();
            }
        }
    }


    Variant operator-(Boolean a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(ValueType::BOOLEAN, bType);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return Variant(static_cast<Integer>(a ? 1 : 0) - b.toInteger());
            }

            case ValueType::REAL: {
                return Variant(static_cast<Real>(a ? 1 : 0) - b.toReal());
            }

            case ValueType::COMPLEX: {
                return Variant(Complex(a ? 1 : 0) - b.toComplex());
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator-(Integer a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(ValueType::INTEGER, bType);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return Variant(a - b.toInteger());
            }

            case ValueType::REAL: {
                return Variant(static_cast<Real>(a) - b.toReal());
            }

            case ValueType::COMPLEX: {
                return Variant(Complex(a) - b.toComplex());
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator-(Real a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(ValueType::REAL, bType);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL: {
                return Variant(a - b.toReal());
            }

            case ValueType::COMPLEX: {
                return Variant(Complex(a) - b.toComplex());
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Complex operator-(const Complex& a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(ValueType::COMPLEX, bType);
                return Complex(std::numeric_limits<Real>::quiet_NaN(), std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return a - b.toComplex();
            }

            default: {
                assert(false);
                return Complex(std::numeric_limits<Real>::quiet_NaN(), std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator-(const MatrixBoolean& a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                internalTriggerTypeConversionError(ValueType::MATRIX_BOOLEAN, bType);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER: {
                return Variant(MatrixInteger(a) - b.toMatrixInteger());
            }

            case ValueType::MATRIX_REAL: {
                return Variant(MatrixReal(a) - b.toMatrixReal());
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(MatrixComplex(a) - b.toMatrixComplex());
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator-(const MatrixInteger& a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                internalTriggerTypeConversionError(ValueType::MATRIX_INTEGER, bType);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER: {
                return Variant(a - b.toMatrixInteger());
            }

            case ValueType::MATRIX_REAL: {
                return Variant(MatrixReal(a) - b.toMatrixReal());
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(MatrixComplex(a) - b.toMatrixComplex());
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator-(const MatrixReal& a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                internalTriggerTypeConversionError(ValueType::MATRIX_REAL, bType);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(a - b.toMatrixReal());
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(MatrixComplex(a) - b.toMatrixComplex());
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    MatrixComplex operator-(const MatrixComplex& a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                internalTriggerTypeConversionError(ValueType::MATRIX_COMPLEX, bType);
                return MatrixComplex();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                return a - b.toMatrixComplex();
            }

            default: {
                assert(false);
                return MatrixComplex();
            }
        }
    }


    Variant operator-(const Variant& a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerTypeConversionError(a.valueType(), bType);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return operator-(a, b.toInteger());
            }

            case ValueType::REAL: {
                return operator-(a, b.toReal());
            }

            case ValueType::COMPLEX: {
                return Variant(operator-(a, b.toComplex()));
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER: {
                return operator-(a, b.toMatrixInteger());
            }

            case ValueType::MATRIX_REAL: {
                return operator-(a, b.toMatrixReal());
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(operator-(a, b.toMatrixComplex()));
            }

            default: {
                assert(false);
                break;
            }
        }

        return Variant(std::numeric_limits<Real>::quiet_NaN());
    }


    Variant operator*(const Variant& a, Boolean b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerTypeConversionError(aType, ValueType::INTEGER);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return Variant(b ? a.toInteger() : Integer(0));
            }

            case ValueType::REAL: {
                return Variant(b ? a.toReal() : Real(0));
            }

            case ValueType::COMPLEX: {
                return Variant(b ? a.toComplex() : Complex(0));
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER: {
                return Variant(a.toMatrixInteger() * static_cast<Integer>(b ? 1 : 0));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(a.toMatrixReal() * static_cast<Real>(b ? 1 : 0));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(a.toMatrixComplex() * Complex(b ? 1 : 0));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator*(const Variant& a, Integer b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerTypeConversionError(aType, ValueType::INTEGER);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return Variant(a.toInteger() * b);
            }

            case ValueType::REAL: {
                return Variant(a.toReal() * Real(b));
            }

            case ValueType::COMPLEX: {
                return Variant(a.toComplex() * Complex(b));
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER: {
                return Variant(a.toMatrixInteger() * b);
            }

            case ValueType::MATRIX_REAL: {
                return Variant(a.toMatrixReal() * Real(b));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(a.toMatrixComplex() * Complex(b));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator*(const Variant& a, Real b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerTypeConversionError(aType, ValueType::REAL);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL: {
                return Variant(a.toReal() * b);
            }

            case ValueType::COMPLEX: {
                return Variant(a.toComplex() * Complex(b));
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(a.toMatrixReal() * b);
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(a.toMatrixComplex() * Complex(b));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator*(const Variant& a, const Complex& b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerTypeConversionError(aType, ValueType::COMPLEX);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return Variant(a.toComplex() * b);
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                return Variant(a.toMatrixComplex() * b);
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Tuple operator*(const Variant& a, const Tuple& b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::TUPLE);
                return Tuple();
            }

            case ValueType::TUPLE: {
                return a.toTuple() * b;
            }

            default: {
                assert(false);
                return Tuple();
            }
        }
    }


    Variant operator*(const Variant& a, const MatrixBoolean& b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerTypeConversionError(aType, ValueType::INTEGER);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return Variant(a.toInteger() * MatrixInteger(b));
            }

            case ValueType::REAL: {
                return Variant(a.toReal() * MatrixReal(b));
            }

            case ValueType::COMPLEX: {
                return Variant(a.toComplex() * MatrixComplex(b));
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER: {
                return Variant(a.toMatrixInteger() * MatrixInteger(b));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(a.toMatrixReal() * MatrixReal(b));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(a.toMatrixComplex() * MatrixComplex(b));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator*(const Variant& a, const MatrixInteger& b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerTypeConversionError(aType, ValueType::INTEGER);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return Variant(a.toInteger() * b);
            }

            case ValueType::REAL: {
                return Variant(a.toReal() * MatrixReal(b));
            }

            case ValueType::COMPLEX: {
                return Variant(a.toComplex() * MatrixComplex(b));
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER: {
                return Variant(a.toMatrixInteger() * b);
            }

            case ValueType::MATRIX_REAL: {
                return Variant(a.toMatrixReal() * MatrixReal(b));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(a.toMatrixComplex() * MatrixComplex(b));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator*(const Variant& a, const MatrixReal& b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerTypeConversionError(aType, ValueType::INTEGER);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL: {
                return Variant(a.toReal() * b);
            }

            case ValueType::COMPLEX: {
                return Variant(a.toComplex() * MatrixComplex(b));
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(a.toMatrixReal() * MatrixReal(b));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(a.toMatrixComplex() * MatrixComplex(b));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    MatrixComplex operator*(const Variant& a, const MatrixComplex& b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerTypeConversionError(aType, ValueType::INTEGER);
                return MatrixComplex();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return a.toComplex() * b;
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                return a.toMatrixComplex() * b;
            }

            default: {
                assert(false);
                return MatrixComplex();
            }
        }
    }


    Variant operator*(Boolean a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerTypeConversionError(ValueType::INTEGER, bType);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return Variant(a ? b.toInteger() : Integer(0));
            }

            case ValueType::REAL: {
                return Variant(a ? b.toReal() : Real(0));
            }

            case ValueType::COMPLEX: {
                return Variant(a ? b.toComplex() : Complex(0));
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER: {
                return Variant(static_cast<Integer>(a ? 1 : 0) * b.toMatrixInteger());
            }

            case ValueType::MATRIX_REAL: {
                return Variant(static_cast<Real>(a ? 1 : 0) * b.toMatrixReal());
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(Complex(a ? 1 : 0) * b.toMatrixComplex());
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator*(Integer a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerTypeConversionError(ValueType::INTEGER, bType);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return Variant(a * b.toInteger());
            }

            case ValueType::REAL: {
                return Variant(a * b.toReal());
            }

            case ValueType::COMPLEX: {
                return Variant(Real(a) * b.toComplex());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER: {
                return Variant(a * b.toMatrixInteger());
            }

            case ValueType::MATRIX_REAL: {
                return Variant(a * b.toMatrixReal());
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(a * b.toMatrixComplex());
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator*(Real a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerTypeConversionError(ValueType::REAL, bType);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL: {
                return Variant(a * b.toReal());
            }

            case ValueType::COMPLEX: {
                return Variant(Real(a) * b.toComplex());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(a * b.toMatrixReal());
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(a * b.toMatrixComplex());
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator*(const Complex& a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerTypeConversionError(ValueType::COMPLEX, bType);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return Variant(a * b.toComplex());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                return Variant(a * b.toMatrixComplex());
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Tuple operator*(const Tuple& a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(ValueType::TUPLE, bType);
                return Tuple();
            }

            case ValueType::TUPLE: {
                return a * b.toTuple();
            }

            default: {
                assert(false);
                return Tuple();
            }
        }
    }


    Variant operator*(const MatrixBoolean& a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerTypeConversionError(ValueType::MATRIX_INTEGER, bType);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return Variant(MatrixInteger(a) * b.toInteger());
            }

            case ValueType::REAL: {
                return Variant(MatrixReal(a) * b.toReal());
            }

            case ValueType::COMPLEX: {
                return Variant(MatrixComplex(a) * b.toComplex());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER: {
                return Variant(MatrixInteger(a) * b.toMatrixInteger());
            }

            case ValueType::MATRIX_REAL: {
                return Variant(MatrixReal(a) * b.toMatrixReal());
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(MatrixComplex(a) * b.toMatrixComplex());
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator*(const MatrixInteger& a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerTypeConversionError(ValueType::MATRIX_INTEGER, bType);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return Variant(a * b.toInteger());
            }

            case ValueType::REAL: {
                return Variant(MatrixReal(a) * b.toReal());
            }

            case ValueType::COMPLEX: {
                return Variant(a * b.toComplex());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER: {
                return Variant(a * b.toMatrixInteger());
            }

            case ValueType::MATRIX_REAL: {
                return Variant(a * b.toMatrixReal());
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(a * b.toMatrixComplex());
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator*(const MatrixReal& a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerTypeConversionError(ValueType::MATRIX_REAL, bType);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL: {
                return Variant(a * b.toReal());
            }

            case ValueType::COMPLEX: {
                return Variant(a * b.toComplex());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(a * b.toMatrixReal());
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(a * b.toMatrixComplex());
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    MatrixComplex operator*(const MatrixComplex& a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerTypeConversionError(ValueType::INTEGER, bType);
                return MatrixComplex();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return a * b.toComplex();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                return a * b.toMatrixComplex();
            }

            default: {
                assert(false);
                return MatrixComplex();
            }
        }
    }


    Variant operator*(const Variant& a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET: {
                internalTriggerTypeConversionError(a.valueType(), bType);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN: {
                return operator*(a, b.toInteger());
            }

            case ValueType::INTEGER: {
                return operator*(a, b.toInteger());
            }

            case ValueType::REAL: {
                return operator*(a, b.toReal());
            }

            case ValueType::COMPLEX: {
                return operator*(a, b.toComplex());
            }

            case ValueType::TUPLE: {
                return Variant(operator*(a, b.toTuple()));
            }

            case ValueType::MATRIX_BOOLEAN: {
                return operator*(a, b.toMatrixInteger());
            }

            case ValueType::MATRIX_INTEGER: {
                return operator*(a, b.toMatrixInteger());
            }

            case ValueType::MATRIX_REAL: {
                return operator*(a, b.toMatrixReal());
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(operator*(a, b.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator/(const Variant& a, Boolean b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerTypeConversionError(aType, ValueType::INTEGER);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                Integer bi = b ? 1 : 0;
                return Variant(a.toInteger() /  bi);
            }

            case ValueType::REAL: {
                return Variant(a.toReal() / Real(b ? 1 : 0));
            }

            case ValueType::COMPLEX: {
                return Variant(a.toComplex() / Real(b ? 1 : 0));
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER: {
                return Variant(a.toMatrixInteger() / Integer(b ? 1 : 0));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(a.toMatrixReal() / Real(b ? 1 : 0));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(a.toMatrixComplex() / Real(b ? 1 : 0));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator/(const Variant& a, Integer b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerTypeConversionError(aType, ValueType::INTEGER);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return Variant(a.toInteger() /  b);
            }

            case ValueType::REAL: {
                return Variant(a.toReal() / b);
            }

            case ValueType::COMPLEX: {
                return Variant(a.toComplex() / Real(b));
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER: {
                return Variant(a.toMatrixInteger() / b);
            }

            case ValueType::MATRIX_REAL: {
                return Variant(a.toMatrixReal() / Real(b));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(a.toMatrixComplex() / Real(b));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator/(const Variant& a, Real b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerTypeConversionError(aType, ValueType::REAL);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL: {
                return Variant(a.toReal() / b);
            }

            case ValueType::COMPLEX: {
                return Variant(a.toComplex() / b);
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(a.toMatrixReal() / b);
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(a.toMatrixComplex() / b);
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator/(const Variant& a, const Complex& b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerTypeConversionError(aType, ValueType::REAL);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return Variant(a.toComplex() / b);
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                return Variant(a.toMatrixComplex() / b);
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Tuple operator/(const Variant& a, const Tuple& b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::TUPLE);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::TUPLE: {
                return Variant(a.toTuple().operator/(b));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator/(Boolean a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(ValueType::INTEGER, bType);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                Integer ai = a ? 1 : 0;
                return Variant(ai / b.toInteger());
            }

            case ValueType::REAL: {
                Integer ai = a ? 1 : 0;
                return Variant(ai / b.toReal());
            }

            case ValueType::COMPLEX: {
                Integer ai = a ? 1 : 0;
                return Variant(ai / b.toComplex());
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator/(Integer a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(ValueType::INTEGER, bType);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return Variant(a / b.toInteger());
            }

            case ValueType::REAL: {
                return Variant(a / b.toReal());
            }

            case ValueType::COMPLEX: {
                return Variant(a / b.toComplex());
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator/(Real a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(ValueType::REAL, bType);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL: {
                return Variant(a / b.toReal());
            }

            case ValueType::COMPLEX: {
                return Variant(a / b.toComplex());
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator/(const Complex& a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(ValueType::COMPLEX, bType);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return Variant(a / b.toComplex());
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Tuple operator/(const Tuple& a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(ValueType::TUPLE, bType);
                return Tuple();
            }

            case ValueType::TUPLE: {
                return a / b.toTuple();
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator/(const MatrixBoolean& a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(ValueType::MATRIX_BOOLEAN, bType);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return Variant(a / b.toInteger());
            }

            case ValueType::REAL: {
                return Variant(a / b.toReal());
            }

            case ValueType::COMPLEX: {
                return Variant(a / b.toComplex());
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator/(const MatrixInteger& a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(ValueType::MATRIX_BOOLEAN, bType);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return Variant(a / b.toInteger());
            }

            case ValueType::REAL: {
                return Variant(a / b.toReal());
            }

            case ValueType::COMPLEX: {
                return Variant(a / b.toComplex());
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator/(const MatrixReal& a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(ValueType::MATRIX_BOOLEAN, bType);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL: {
                return Variant(a / b.toReal());
            }

            case ValueType::COMPLEX: {
                return Variant(a / b.toComplex());
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    MatrixComplex operator/(const MatrixComplex& a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(ValueType::MATRIX_BOOLEAN, bType);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return Variant(a / b.toComplex());
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator/(const Variant& a, const Variant& b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET: {
                internalTriggerTypeConversionError(aType, b.valueType());
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return a.toInteger() / b;
            }

            case ValueType::REAL: {
                return a.toReal() / b;
            }

            case ValueType::COMPLEX: {
                return a.toComplex() / b;
            }

            case ValueType::TUPLE: {
                if (b.valueType() == ValueType::TUPLE) {
                    return Variant(a.toTuple().operator/(b.toTuple()));
                } else {
                    internalTriggerTypeConversionError(aType, b.valueType());
                    return Variant(Tuple());
                }
            }

            case ValueType::MATRIX_BOOLEAN: {
                return a.toMatrixBoolean() / b;
            }

            case ValueType::MATRIX_INTEGER: {
                return a.toMatrixInteger() / b;
            }

            case ValueType::MATRIX_REAL: {
                return a.toMatrixReal() / b;
            }

            case ValueType::MATRIX_COMPLEX: {
                return a.toMatrixComplex() / b;
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator+(const Variant& value) {
        ValueType valueType = value.valueType();

        switch (valueType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerTypeConversionError(valueType, valueType);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN: {
                return Variant(value.toInteger());
            }

            case ValueType::MATRIX_BOOLEAN: {
                return Variant(value.toMatrixInteger());
            }

            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                return value;
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant operator-(const Variant& value) {
        ValueType valueType = value.valueType();

        switch (valueType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerTypeConversionError(valueType, valueType);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return Variant(-value.toInteger());
            }

            case ValueType::REAL: {
                return Variant(-value.toReal());
            }

            case ValueType::COMPLEX: {
                return Variant(-value.toComplex());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER: {
                return Variant(-value.toMatrixInteger());
            }

            case ValueType::MATRIX_REAL: {
                return Variant(-value.toMatrixReal());
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(-value.toMatrixComplex());
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    bool operator&&(const Variant& a, Boolean b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::BOOLEAN);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return a.toBoolean() && b;
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool operator&&(const Variant& a, Integer b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::BOOLEAN);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return a.toBoolean() && (b != 0);
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool operator&&(const Variant& a, Real b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::BOOLEAN);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return a.toBoolean() && (b != 0);
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool operator&&(const Variant& a, const Complex& b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::BOOLEAN);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return a.toBoolean() && (b != 0);
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool operator&&(Boolean a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(bType, ValueType::BOOLEAN);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return a && b.toBoolean();
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool operator&&(Integer a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(bType, ValueType::BOOLEAN);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return (a != 0) && b.toBoolean();
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool operator&&(Real a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(bType, ValueType::BOOLEAN);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return (a != 0) && b.toBoolean();
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool operator&&(const Complex& a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(bType, ValueType::BOOLEAN);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return (a != 0) && b.toBoolean();
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool operator&&(const Variant& a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(bType, ValueType::BOOLEAN);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return operator&&(a, b.toBoolean());
            }

            default: {
                assert(false);
                return false;
            }
        }
    }

    bool operator||(const Variant& a, Boolean b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::BOOLEAN);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return a.toBoolean() || b;
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool operator||(const Variant& a, Integer b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::BOOLEAN);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return a.toBoolean() || (b != 0);
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool operator||(const Variant& a, Real b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::BOOLEAN);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return a.toBoolean() || (b != 0);
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool operator||(const Variant& a, const Complex& b) {
        ValueType aType = a.valueType();

        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(aType, ValueType::BOOLEAN);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return a.toBoolean() || (b != 0);
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool operator||(Boolean a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(bType, ValueType::BOOLEAN);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return a || b.toBoolean();
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool operator||(Integer a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(bType, ValueType::BOOLEAN);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return (a != 0) || b.toBoolean();
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool operator||(Real a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(bType, ValueType::BOOLEAN);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return (a != 0) || b.toBoolean();
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool operator||(const Complex& a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(bType, ValueType::BOOLEAN);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return (a != 0) || b.toBoolean();
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool operator||(const Variant& a, const Variant& b) {
        ValueType bType = b.valueType();

        switch (bType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(bType, ValueType::BOOLEAN);
                return false;
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return operator||(a, b.toBoolean());
            }

            default: {
                assert(false);
                return false;
            }
        }
    }
}
