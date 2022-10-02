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
* This file implements a collection of matrix functions.
***********************************************************************************************************************/

#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>

#include "m_api_types.h"
#include "model_exceptions.h"
#include "m_intrinsic_types.h"
#include "m_variant.h"
#include "m_matrix_boolean.h"
#include "m_matrix_integer.h"
#include "m_matrix_real.h"
#include "m_matrix_complex.h"
#include "m_matrix_functions.h"

namespace M {
    Integer numberRows(const Variant& m) {
        ValueType mType = m.valueType();
        switch (mType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return 0;
            }

            case ValueType::MATRIX_BOOLEAN: {
                return m.toMatrixBoolean().numberRows();
            }

            case ValueType::MATRIX_INTEGER: {
                return m.toMatrixInteger().numberRows();
            }

            case ValueType::MATRIX_REAL: {
                return m.toMatrixReal().numberRows();
            }

            case ValueType::MATRIX_COMPLEX: {
                return m.toMatrixComplex().numberRows();
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Integer numberColumns(const Variant& m) {
        ValueType mType = m.valueType();
        switch (mType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return 0;
            }

            case ValueType::MATRIX_BOOLEAN: {
                return m.toMatrixBoolean().numberColumns();
            }

            case ValueType::MATRIX_INTEGER: {
                return m.toMatrixInteger().numberColumns();
            }

            case ValueType::MATRIX_REAL: {
                return m.toMatrixReal().numberColumns();
            }

            case ValueType::MATRIX_COMPLEX: {
                return m.toMatrixComplex().numberColumns();
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Variant diagonalEntries(const Variant& m) {
        ValueType mType = m.valueType();
        switch (mType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN: {
                return Variant(m.toMatrixBoolean().diagonalEntries());
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(m.toMatrixInteger().diagonalEntries());
            }

            case ValueType::MATRIX_REAL: {
                return Variant(m.toMatrixReal().diagonalEntries());
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(m.toMatrixComplex().diagonalEntries());
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant diagonal(const Variant& m) {
        ValueType mType = m.valueType();
        switch (mType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN: {
                return Variant(m.toMatrixBoolean().diagonal());
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(m.toMatrixInteger().diagonal());
            }

            case ValueType::MATRIX_REAL: {
                return Variant(m.toMatrixReal().diagonal());
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(m.toMatrixComplex().diagonal());
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant matrixCombineLeftToRight(const Variant& m1, const MatrixBoolean& m2) {
        ValueType m1Type = m1.valueType();
        switch (m1Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN: {
                return Variant(matrixCombineLeftToRight(m1.toMatrixBoolean(), m2));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(matrixCombineLeftToRight(m1.toMatrixInteger(), m2));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(matrixCombineLeftToRight(m1.toMatrixReal(), m2));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(matrixCombineLeftToRight(m1.toMatrixComplex(), m2));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant matrixCombineLeftToRight(const Variant& m1, const MatrixInteger& m2) {
        ValueType m1Type = m1.valueType();
        switch (m1Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER: {
                return Variant(matrixCombineLeftToRight(m1.toMatrixInteger(), m2));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(matrixCombineLeftToRight(m1.toMatrixReal(), m2));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(matrixCombineLeftToRight(m1.toMatrixComplex(), m2));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant matrixCombineLeftToRight(const Variant& m1, const MatrixReal& m2) {
        ValueType m1Type = m1.valueType();
        switch (m1Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(matrixCombineLeftToRight(m1.toMatrixReal(), m2));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(matrixCombineLeftToRight(m1.toMatrixComplex(), m2));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    MatrixComplex matrixCombineLeftToRight(const Variant& m1, const MatrixComplex& m2) {
        ValueType m1Type = m1.valueType();
        switch (m1Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return MatrixComplex();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                return Variant(matrixCombineLeftToRight(m1.toMatrixComplex(), m2));
            }

            default: {
                assert(false);
                return MatrixComplex();
            }
        }
    }


    Variant matrixCombineLeftToRight(const MatrixBoolean& m1, const Variant& m2) {
        ValueType m2Type = m2.valueType();
        switch (m2Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN: {
                return Variant(matrixCombineLeftToRight(m1, m2.toMatrixBoolean()));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(matrixCombineLeftToRight(m1, m2.toMatrixInteger()));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(matrixCombineLeftToRight(m1, m2.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(matrixCombineLeftToRight(m1, m2.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant matrixCombineLeftToRight(const MatrixInteger& m1, const Variant& m2) {
        ValueType m2Type = m2.valueType();
        switch (m2Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER: {
                return Variant(matrixCombineLeftToRight(m1, m2.toMatrixInteger()));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(matrixCombineLeftToRight(m1, m2.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(matrixCombineLeftToRight(m1, m2.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant matrixCombineLeftToRight(const MatrixReal& m1, const Variant& m2) {
        ValueType m2Type = m2.valueType();
        switch (m2Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(matrixCombineLeftToRight(m1, m2.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(matrixCombineLeftToRight(m1, m2.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    MatrixComplex matrixCombineLeftToRight(const MatrixComplex& m1, const Variant& m2) {
        ValueType m2Type = m2.valueType();
        switch (m2Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return MatrixComplex();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                return Variant(matrixCombineLeftToRight(m1, m2.toMatrixComplex()));
            }

            default: {
                assert(false);
                return MatrixComplex();
            }
        }
    }


    Variant matrixCombineLeftToRight(const Variant& m1, const Variant& m2) {
        ValueType m1Type = m1.valueType();
        switch (m1Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN: {
                return Variant(matrixCombineLeftToRight(m1.toMatrixBoolean(), m2));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(matrixCombineLeftToRight(m1.toMatrixInteger(), m2));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(matrixCombineLeftToRight(m1.toMatrixReal(), m2));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(matrixCombineLeftToRight(m1.toMatrixComplex(), m2));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant matrixCombineTopToBottom(const Variant& m1, const MatrixBoolean& m2) {
        ValueType m1Type = m1.valueType();
        switch (m1Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN: {
                return Variant(matrixCombineTopToBottom(m1.toMatrixBoolean(), m2));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(matrixCombineTopToBottom(m1.toMatrixInteger(), m2));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(matrixCombineTopToBottom(m1.toMatrixReal(), m2));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(matrixCombineTopToBottom(m1.toMatrixComplex(), m2));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant matrixCombineTopToBottom(const Variant& m1, const MatrixInteger& m2) {
        ValueType m1Type = m1.valueType();
        switch (m1Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER: {
                return Variant(matrixCombineTopToBottom(m1.toMatrixInteger(), m2));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(matrixCombineTopToBottom(m1.toMatrixReal(), m2));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(matrixCombineTopToBottom(m1.toMatrixComplex(), m2));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant matrixCombineTopToBottom(const Variant& m1, const MatrixReal& m2) {
        ValueType m1Type = m1.valueType();
        switch (m1Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(matrixCombineTopToBottom(m1.toMatrixReal(), m2));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(matrixCombineTopToBottom(m1.toMatrixComplex(), m2));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    MatrixComplex matrixCombineTopToBottom(const Variant& m1, const MatrixComplex& m2) {
        ValueType m1Type = m1.valueType();
        switch (m1Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return MatrixComplex();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                return Variant(matrixCombineTopToBottom(m1.toMatrixComplex(), m2));
            }

            default: {
                assert(false);
                return MatrixComplex();
            }
        }
    }


    Variant matrixCombineTopToBottom(const MatrixBoolean& m1, const Variant& m2) {
        ValueType m2Type = m2.valueType();
        switch (m2Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN: {
                return Variant(matrixCombineTopToBottom(m1, m2.toMatrixBoolean()));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(matrixCombineTopToBottom(m1, m2.toMatrixInteger()));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(matrixCombineTopToBottom(m1, m2.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(matrixCombineTopToBottom(m1, m2.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant matrixCombineTopToBottom(const MatrixInteger& m1, const Variant& m2) {
        ValueType m2Type = m2.valueType();
        switch (m2Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER: {
                return Variant(matrixCombineTopToBottom(m1, m2.toMatrixInteger()));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(matrixCombineTopToBottom(m1, m2.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(matrixCombineTopToBottom(m1, m2.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant matrixCombineTopToBottom(const MatrixReal& m1, const Variant& m2) {
        ValueType m2Type = m2.valueType();
        switch (m2Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(matrixCombineTopToBottom(m1, m2.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(matrixCombineTopToBottom(m1, m2.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    MatrixComplex matrixCombineTopToBottom(const MatrixComplex& m1, const Variant& m2) {
        ValueType m2Type = m2.valueType();
        switch (m2Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return MatrixComplex();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                return Variant(matrixCombineTopToBottom(m1, m2.toMatrixComplex()));
            }

            default: {
                assert(false);
                return MatrixComplex();
            }
        }
    }


    Variant matrixCombineTopToBottom(const Variant& m1, const Variant& m2) {
        ValueType m1Type = m1.valueType();
        switch (m1Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN: {
                return Variant(matrixCombineTopToBottom(m1.toMatrixBoolean(), m2));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(matrixCombineTopToBottom(m1.toMatrixInteger(), m2));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(matrixCombineTopToBottom(m1.toMatrixReal(), m2));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(matrixCombineTopToBottom(m1.toMatrixComplex(), m2));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant hadamard(const Variant& m1, const MatrixBoolean& m2) {
        ValueType m1Type = m1.valueType();
        switch (m1Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN: {
                return Variant(hadamard(m1.toMatrixBoolean(), m2));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(hadamard(m1.toMatrixInteger(), m2));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(hadamard(m1.toMatrixReal(), m2));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(hadamard(m1.toMatrixComplex(), m2));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant hadamard(const Variant& m1, const MatrixInteger& m2) {
        ValueType m1Type = m1.valueType();
        switch (m1Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER: {
                return Variant(hadamard(m1.toMatrixInteger(), m2));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(hadamard(m1.toMatrixReal(), m2));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(hadamard(m1.toMatrixComplex(), m2));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant hadamard(const Variant& m1, const MatrixReal& m2) {
        ValueType m1Type = m1.valueType();
        switch (m1Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(hadamard(m1.toMatrixReal(), m2));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(hadamard(m1.toMatrixComplex(), m2));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    MatrixComplex hadamard(const Variant& m1, const MatrixComplex& m2) {
        ValueType m1Type = m1.valueType();
        switch (m1Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return MatrixComplex();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                return Variant(hadamard(m1.toMatrixComplex(), m2));
            }

            default: {
                assert(false);
                return MatrixComplex();
            }
        }
    }


    Variant hadamard(const MatrixBoolean& m1, const Variant& m2) {
        ValueType m2Type = m2.valueType();
        switch (m2Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN: {
                return Variant(hadamard(m1, m2.toMatrixBoolean()));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(hadamard(m1, m2.toMatrixInteger()));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(hadamard(m1, m2.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(hadamard(m1, m2.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant hadamard(const MatrixInteger& m1, const Variant& m2) {
        ValueType m2Type = m2.valueType();
        switch (m2Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER: {
                return Variant(hadamard(m1, m2.toMatrixInteger()));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(hadamard(m1, m2.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(hadamard(m1, m2.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant hadamard(const MatrixReal& m1, const Variant& m2) {
        ValueType m2Type = m2.valueType();
        switch (m2Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(hadamard(m1, m2.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(hadamard(m1, m2.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    MatrixComplex hadamard(const MatrixComplex& m1, const Variant& m2) {
        ValueType m2Type = m2.valueType();
        switch (m2Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return MatrixComplex();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                return Variant(hadamard(m1, m2.toMatrixComplex()));
            }

            default: {
                assert(false);
                return MatrixComplex();
            }
        }
    }


    Variant hadamard(const Variant& m1, const Variant& m2) {
        ValueType m1Type = m1.valueType();
        switch (m1Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN: {
                return Variant(hadamard(m1.toMatrixBoolean(), m2));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(hadamard(m1.toMatrixInteger(), m2));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(hadamard(m1.toMatrixReal(), m2));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(hadamard(m1.toMatrixComplex(), m2));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant kronecker(const Variant& m1, const MatrixBoolean& m2) {
        ValueType m1Type = m1.valueType();
        switch (m1Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN: {
                return Variant(kronecker(m1.toMatrixBoolean(), m2));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(kronecker(m1.toMatrixInteger(), m2));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(kronecker(m1.toMatrixReal(), m2));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(kronecker(m1.toMatrixComplex(), m2));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant kronecker(const Variant& m1, const MatrixInteger& m2) {
        ValueType m1Type = m1.valueType();
        switch (m1Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER: {
                return Variant(kronecker(m1.toMatrixInteger(), m2));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(kronecker(m1.toMatrixReal(), m2));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(kronecker(m1.toMatrixComplex(), m2));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant kronecker(const Variant& m1, const MatrixReal& m2) {
        ValueType m1Type = m1.valueType();
        switch (m1Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(kronecker(m1.toMatrixReal(), m2));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(kronecker(m1.toMatrixComplex(), m2));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    MatrixComplex kronecker(const Variant& m1, const MatrixComplex& m2) {
        ValueType m1Type = m1.valueType();
        switch (m1Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return MatrixComplex();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                return Variant(kronecker(m1.toMatrixComplex(), m2));
            }

            default: {
                assert(false);
                return MatrixComplex();
            }
        }
    }


    Variant kronecker(const MatrixBoolean& m1, const Variant& m2) {
        ValueType m2Type = m2.valueType();
        switch (m2Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN: {
                return Variant(kronecker(m1, m2.toMatrixBoolean()));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(kronecker(m1, m2.toMatrixInteger()));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(kronecker(m1, m2.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(kronecker(m1, m2.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant kronecker(const MatrixInteger& m1, const Variant& m2) {
        ValueType m2Type = m2.valueType();
        switch (m2Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER: {
                return Variant(kronecker(m1, m2.toMatrixInteger()));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(kronecker(m1, m2.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(kronecker(m1, m2.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant kronecker(const MatrixReal& m1, const Variant& m2) {
        ValueType m2Type = m2.valueType();
        switch (m2Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(kronecker(m1, m2.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(kronecker(m1, m2.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    MatrixComplex kronecker(const MatrixComplex& m1, const Variant& m2) {
        ValueType m2Type = m2.valueType();
        switch (m2Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return MatrixComplex();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                return Variant(kronecker(m1, m2.toMatrixComplex()));
            }

            default: {
                assert(false);
                return MatrixComplex();
            }
        }
    }


    Variant kronecker(const Variant& m1, const Variant& m2) {
        ValueType m1Type = m1.valueType();
        switch (m1Type) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN: {
                return Variant(kronecker(m1.toMatrixBoolean(), m2));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(kronecker(m1.toMatrixInteger(), m2));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(kronecker(m1.toMatrixReal(), m2));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(kronecker(m1.toMatrixComplex(), m2));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant transpose(const Variant& matrix) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN: {
                return Variant(transpose(matrix.toMatrixBoolean()));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(transpose(matrix.toMatrixInteger()));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(transpose(matrix.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(transpose(matrix.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant hermitian(const Variant& matrix) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN: {
                return Variant(hermitian(matrix.toMatrixBoolean()));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(hermitian(matrix.toMatrixInteger()));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(hermitian(matrix.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(hermitian(matrix.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant inverse(const Variant& matrix) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(inverse(matrix.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(inverse(matrix.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant columnReverse(const Variant& matrix) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN: {
                return Variant(columnReverse(matrix.toMatrixBoolean()));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(columnReverse(matrix.toMatrixInteger()));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(columnReverse(matrix.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(columnReverse(matrix.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant rowReverse(const Variant& matrix) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN: {
                return Variant(rowReverse(matrix.toMatrixBoolean()));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(rowReverse(matrix.toMatrixInteger()));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(rowReverse(matrix.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(rowReverse(matrix.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    bool isSquare(const Variant& matrix) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return false;
            }

            case ValueType::MATRIX_BOOLEAN: {
                return isSquare(matrix.toMatrixBoolean());
            }

            case ValueType::MATRIX_INTEGER: {
                return isSquare(matrix.toMatrixInteger());
            }

            case ValueType::MATRIX_REAL: {
                return isSquare(matrix.toMatrixReal());
            }

            case ValueType::MATRIX_COMPLEX: {
                return isSquare(matrix.toMatrixComplex());
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool isSymmetric(const Variant& matrix, Real relativeTolerance) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return false;
            }

            case ValueType::MATRIX_BOOLEAN: {
                return isSymmetric(matrix.toMatrixBoolean(), relativeTolerance);
            }

            case ValueType::MATRIX_INTEGER: {
                return isSymmetric(matrix.toMatrixInteger(), relativeTolerance);
            }

            case ValueType::MATRIX_REAL: {
                return isSymmetric(matrix.toMatrixReal(), relativeTolerance);
            }

            case ValueType::MATRIX_COMPLEX: {
                return isSymmetric(matrix.toMatrixComplex(), relativeTolerance);
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool isHermitian(const Variant& matrix, Real relativeTolerance) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return false;
            }

            case ValueType::MATRIX_BOOLEAN: {
                return isHermitian(matrix.toMatrixBoolean(), relativeTolerance);
            }

            case ValueType::MATRIX_INTEGER: {
                return isHermitian(matrix.toMatrixInteger(), relativeTolerance);
            }

            case ValueType::MATRIX_REAL: {
                return isHermitian(matrix.toMatrixReal(), relativeTolerance);
            }

            case ValueType::MATRIX_COMPLEX: {
                return isHermitian(matrix.toMatrixComplex(), relativeTolerance);
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool isSkewSymmetric(const Variant& matrix, Real relativeTolerance) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return false;
            }

            case ValueType::MATRIX_BOOLEAN: {
                return isSkewSymmetric(matrix.toMatrixBoolean(), relativeTolerance);
            }

            case ValueType::MATRIX_INTEGER: {
                return isSkewSymmetric(matrix.toMatrixInteger(), relativeTolerance);
            }

            case ValueType::MATRIX_REAL: {
                return isSkewSymmetric(matrix.toMatrixReal(), relativeTolerance);
            }

            case ValueType::MATRIX_COMPLEX: {
                return isSkewSymmetric(matrix.toMatrixComplex(), relativeTolerance);
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool isSkewHermitian(const Variant& matrix, Real relativeTolerance) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return false;
            }

            case ValueType::MATRIX_BOOLEAN: {
                return isSkewHermitian(matrix.toMatrixBoolean(), relativeTolerance);
            }

            case ValueType::MATRIX_INTEGER: {
                return isSkewHermitian(matrix.toMatrixInteger(), relativeTolerance);
            }

            case ValueType::MATRIX_REAL: {
                return isSkewHermitian(matrix.toMatrixReal(), relativeTolerance);
            }

            case ValueType::MATRIX_COMPLEX: {
                return isSkewHermitian(matrix.toMatrixComplex(), relativeTolerance);
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    bool isNormal(const Variant& matrix, Real relativeTolerance) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return false;
            }

            case ValueType::MATRIX_BOOLEAN: {
                return isNormal(matrix.toMatrixBoolean(), relativeTolerance);
            }

            case ValueType::MATRIX_INTEGER: {
                return isNormal(matrix.toMatrixInteger(), relativeTolerance);
            }

            case ValueType::MATRIX_REAL: {
                return isNormal(matrix.toMatrixReal(), relativeTolerance);
            }

            case ValueType::MATRIX_COMPLEX: {
                return isNormal(matrix.toMatrixComplex(), relativeTolerance);
            }

            default: {
                assert(false);
                return false;
            }
        }
    }


    Integer rank(const Variant& matrix, Real epsilon) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return -1;
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return rank(matrix.toMatrixReal(), epsilon);
            }

            case ValueType::MATRIX_COMPLEX: {
                return rank(matrix.toMatrixComplex(), epsilon);
            }

            default: {
                assert(false);
                return -1;
            }
        }
    }


    Complex determinant(const Variant& matrix, Real epsilon) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return -1;
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return determinant(matrix.toMatrixReal(), epsilon);
            }

            case ValueType::MATRIX_COMPLEX: {
                return determinant(matrix.toMatrixComplex(), epsilon);
            }

            default: {
                assert(false);
                return -1;
            }
        }
    }


    Tuple plu(const Variant& matrix) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Tuple();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return plu(matrix.toMatrixReal());
            }

            case ValueType::MATRIX_COMPLEX: {
                return plu(matrix.toMatrixComplex());
            }

            default: {
                assert(false);
                return Tuple();
            }
        }
    }


    Tuple svd(const Variant& matrix) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Tuple();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return svd(matrix.toMatrixReal());
            }

            case ValueType::MATRIX_COMPLEX: {
                return svd(matrix.toMatrixComplex());
            }

            default: {
                assert(false);
                return Tuple();
            }
        }
    }


    Tuple qr(const Variant& matrix) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Tuple();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return qr(matrix.toMatrixReal());
            }

            case ValueType::MATRIX_COMPLEX: {
                return qr(matrix.toMatrixComplex());
            }

            default: {
                assert(false);
                return Tuple();
            }
        }
    }


    Tuple lq(const Variant& matrix) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Tuple();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return lq(matrix.toMatrixReal());
            }

            case ValueType::MATRIX_COMPLEX: {
                return lq(matrix.toMatrixComplex());
            }

            default: {
                assert(false);
                return Tuple();
            }
        }
    }


    Variant cholesky(const Variant& matrix) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(cholesky(matrix.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(cholesky(matrix.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant upperCholesky(const Variant& matrix) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(upperCholesky(matrix.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(upperCholesky(matrix.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Tuple hessenberg(const Variant& matrix) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Tuple();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return hessenberg(matrix.toMatrixReal());
            }

            case ValueType::MATRIX_COMPLEX: {
                return hessenberg(matrix.toMatrixComplex());
            }

            default: {
                assert(false);
                return Tuple();
            }
        }
    }


    Tuple schur(const Variant& matrix) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Tuple();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                return schur(matrix.toMatrixComplex());
            }

            default: {
                assert(false);
                return Tuple();
            }
        }
    }


    Tuple eigenvectors(const Variant& matrix) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Tuple();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                return schur(matrix.toMatrixComplex());
            }

            default: {
                assert(false);
                return Tuple();
            }
        }
    }


    Real conditionNumber(const Variant& matrix) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return conditionNumber(matrix.toMatrixReal());
            }

            case ValueType::MATRIX_COMPLEX: {
                return conditionNumber(matrix.toMatrixComplex());
            }

            default: {
                assert(false);
                return std::numeric_limits<Real>::quiet_NaN();
            }
        }
    }


    Real pNorm(const MatrixBoolean& matrix, Real p) {
        Real result;

        Real wholePart;
        if (std::modf(p, &wholePart)) {
            result = MatrixReal(matrix).pNorm(static_cast<Integer>(p));
        } else {
            result = NaN;
        }

        return result;
    }


    Real pNorm(const MatrixInteger& matrix, Real p) {
        Real result;

        Real wholePart;
        if (std::modf(p, &wholePart)) {
            result = MatrixReal(matrix).pNorm(static_cast<Integer>(p));
        } else {
            result = NaN;
        }

        return result;
    }


    Real pNorm(const MatrixReal& matrix, Real p) {
        Real result;

        Real wholePart;
        if (std::modf(p, &wholePart)) {
            result = matrix.pNorm(static_cast<Integer>(p));
        } else {
            result = NaN;
        }

        return result;
    }


    Real pNorm(const MatrixComplex& matrix, Real p) {
        Real result;

        Real wholePart;
        if (std::modf(p, &wholePart)) {
            result = matrix.pNorm(static_cast<Integer>(p));
        } else {
            result = NaN;
        }

        return result;
    }


    Real pNorm(const MatrixBoolean& matrix, const Complex& p) {
        Real result;

        Real wholePart;
        if (p.imag() == 0 && std::modf(p.real(), &wholePart)) {
            result = MatrixReal(matrix).pNorm(static_cast<Integer>(p.real()));
        } else {
            result = NaN;
        }

        return result;
    }


    Real pNorm(const MatrixInteger& matrix, const Complex& p) {
        Real result;

        Real wholePart;
        if (p.imag() == 0 && std::modf(p.real(), &wholePart)) {
            result = MatrixReal(matrix).pNorm(static_cast<Integer>(p.real()));
        } else {
            result = NaN;
        }

        return result;
    }


    Real pNorm(const MatrixReal& matrix, const Complex& p) {
        Real result;

        Real wholePart;
        if (p.imag() == 0 && std::modf(p.real(), &wholePart)) {
            result = matrix.pNorm(static_cast<Integer>(p.real()));
        } else {
            result = NaN;
        }

        return result;
    }


    Real pNorm(const MatrixComplex& matrix, const Complex& p) {
        Real result;

        Real wholePart;
        if (p.imag() == 0 && std::modf(p.real(), &wholePart)) {
            result = matrix.pNorm(static_cast<Integer>(p.real()));
        } else {
            result = NaN;
        }

        return result;
    }



    Real pNorm(const Variant& matrix, Boolean p) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return pNorm(matrix.toMatrixReal(), Integer(p ? 1 : 0));
            }

            case ValueType::MATRIX_COMPLEX: {
                return pNorm(matrix.toMatrixComplex(), Integer(p ? 1 : 0));
            }

            default: {
                assert(false);
                return std::numeric_limits<Real>::quiet_NaN();
            }
        }
    }


    Real pNorm(const Variant& matrix, Integer p) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return pNorm(matrix.toMatrixReal(), p);
            }

            case ValueType::MATRIX_COMPLEX: {
                return pNorm(matrix.toMatrixComplex(), p);
            }

            default: {
                assert(false);
                return std::numeric_limits<Real>::quiet_NaN();
            }
        }
    }


    Real pNorm(const Variant& matrix, Real p) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return pNorm(matrix.toMatrixReal(), p);
            }

            case ValueType::MATRIX_COMPLEX: {
                return pNorm(matrix.toMatrixComplex(), p);
            }

            default: {
                assert(false);
                return std::numeric_limits<Real>::quiet_NaN();
            }
        }
    }


    Real pNorm(const Variant& matrix, const Complex& p) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return pNorm(matrix.toMatrixReal(), p);
            }

            case ValueType::MATRIX_COMPLEX: {
                return pNorm(matrix.toMatrixComplex(), p);
            }

            default: {
                assert(false);
                return std::numeric_limits<Real>::quiet_NaN();
            }
        }
    }


    Real pNorm(const MatrixBoolean& matrix, const Variant& p) {
        return pNorm(matrix, p.toInteger());
    }


    Real pNorm(const MatrixInteger& matrix, const Variant& p) {
        return pNorm(matrix, p.toInteger());
    }


    Real pNorm(const MatrixReal& matrix, const Variant& p) {
        return pNorm(matrix, p.toInteger());
    }


    Real pNorm(const MatrixComplex& matrix, const Variant& p) {
        return pNorm(matrix, p.toInteger());
    }


    Real pNorm(const Variant& matrix, const Variant& p) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return pNorm(matrix.toMatrixReal(), p.toInteger());
            }

            case ValueType::MATRIX_COMPLEX: {
                return pNorm(matrix.toMatrixComplex(), p.toInteger());
            }

            default: {
                assert(false);
                return std::numeric_limits<Real>::quiet_NaN();
            }
        }
    }


    Real euclideanNorm(const Variant& matrix) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return euclideanNorm(matrix.toMatrixReal());
            }

            case ValueType::MATRIX_COMPLEX: {
                return euclideanNorm(matrix.toMatrixComplex());
            }

            default: {
                assert(false);
                return std::numeric_limits<Real>::quiet_NaN();
            }
        }
    }


    Real oneNorm(const Variant& matrix) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return oneNorm(matrix.toMatrixReal());
            }

            case ValueType::MATRIX_COMPLEX: {
                return oneNorm(matrix.toMatrixComplex());
            }

            default: {
                assert(false);
                return std::numeric_limits<Real>::quiet_NaN();
            }
        }
    }


    Real infinityNorm(const Variant& matrix) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return infinityNorm(matrix.toMatrixReal());
            }

            case ValueType::MATRIX_COMPLEX: {
                return infinityNorm(matrix.toMatrixComplex());
            }

            default: {
                assert(false);
                return std::numeric_limits<Real>::quiet_NaN();
            }
        }
    }


    Tuple equilibrate(const Variant& matrix) {
        ValueType matrixType = matrix.valueType();
        switch (matrixType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Tuple();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return equilibrate(matrix.toMatrixReal());
            }

            case ValueType::MATRIX_COMPLEX: {
                return equilibrate(matrix.toMatrixComplex());
            }

            default: {
                assert(false);
                return Tuple();
            }
        }
    }


    Variant solve(const Variant& a, const MatrixBoolean& y) {
        ValueType aType = a.valueType();
        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(solve(a.toMatrixReal(), y));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(solve(a.toMatrixComplex(), y));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant solve(const Variant& a, const MatrixInteger& y) {
        ValueType aType = a.valueType();
        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(solve(a.toMatrixReal(), y));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(solve(a.toMatrixComplex(), y));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant solve(const Variant& a, const MatrixReal& y) {
        ValueType aType = a.valueType();
        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(solve(a.toMatrixReal(), y));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(solve(a.toMatrixComplex(), y));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    MatrixComplex solve(const Variant& a, const MatrixComplex& y) {
        ValueType aType = a.valueType();
        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return MatrixComplex();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                return solve(a.toMatrixComplex(), y);
            }

            default: {
                assert(false);
                return MatrixComplex();
            }
        }
    }


    Variant solve(const MatrixBoolean& a, const Variant& y) {
        ValueType yType = y.valueType();
        switch (yType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(solve(a, y.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(solve(a, y.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant solve(const MatrixInteger& a, const Variant& y) {
        ValueType yType = y.valueType();
        switch (yType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(solve(a, y.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(solve(a, y.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant solve(const MatrixReal& a, const Variant& y) {
        ValueType yType = y.valueType();
        switch (yType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(solve(a, y.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(solve(a, y.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    MatrixComplex solve(const MatrixComplex& a, const Variant& y) {
        ValueType yType = y.valueType();
        switch (yType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                return Variant(solve(a, y.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant solve(const Variant& a, const Variant& y) {
        ValueType aType = a.valueType();
        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(solve(a.toMatrixReal(), y));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(solve(a.toMatrixComplex(), y));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant leastSquares(const Variant& a, const MatrixBoolean& y) {
        ValueType aType = a.valueType();
        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(leastSquares(a.toMatrixReal(), y));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(leastSquares(a.toMatrixComplex(), y));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant leastSquares(const Variant& a, const MatrixInteger& y) {
        ValueType aType = a.valueType();
        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(leastSquares(a.toMatrixReal(), y));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(leastSquares(a.toMatrixComplex(), y));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant leastSquares(const Variant& a, const MatrixReal& y) {
        ValueType aType = a.valueType();
        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(leastSquares(a.toMatrixReal(), y));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(leastSquares(a.toMatrixComplex(), y));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    MatrixComplex leastSquares(const Variant& a, const MatrixComplex& y) {
        ValueType aType = a.valueType();
        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return MatrixComplex();
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                return leastSquares(a.toMatrixComplex(), y);
            }

            default: {
                assert(false);
                return MatrixComplex();
            }
        }
    }


    Variant leastSquares(const MatrixBoolean& a, const Variant& y) {
        ValueType yType = y.valueType();
        switch (yType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(leastSquares(a, y.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(leastSquares(a, y.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant leastSquares(const MatrixInteger& a, const Variant& y) {
        ValueType yType = y.valueType();
        switch (yType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(leastSquares(a, y.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(leastSquares(a, y.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant leastSquares(const MatrixReal& a, const Variant& y) {
        ValueType yType = y.valueType();
        switch (yType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(leastSquares(a, y.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(leastSquares(a, y.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    MatrixComplex leastSquares(const MatrixComplex& a, const Variant& y) {
        ValueType yType = y.valueType();
        switch (yType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                return Variant(leastSquares(a, y.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    Variant leastSquares(const Variant& a, const Variant& y) {
        ValueType aType = a.valueType();
        switch (aType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX:
            case ValueType::SET:
            case ValueType::TUPLE: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL: {
                return Variant(leastSquares(a.toMatrixReal(), y));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(leastSquares(a.toMatrixComplex(), y));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }
}
