/*-*-c++-*-*************************************************************************************************************
* Copyright 2016 Inesonic, LLC.
* All rights reserved.
********************************************************************************************************************//**
* \file
*
* This file provides a table indicating the optimal type to use to compare two variants.
***********************************************************************************************************************/

#include "m_api_types.h"

static constexpr unsigned numberTypes = static_cast<unsigned>(M::ValueType::NUMBER_TYPES);

static M::ValueType bestCompareTypeArray[numberTypes][numberTypes] = {
                        // { NONE,               VARIANT,               BOOLEAN,               INTEGER,               REAL,                  COMPLEX,               SET                    TUPLE                  MATRIX_BOOLEAN,               MATRIX_INTEGER,               MATRIX_REAL,                  MATRIX_COMPLEX               },
    /* NONE */             { M::ValueType::NONE, M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,           M::ValueType::NONE,           M::ValueType::NONE,           M::ValueType::NONE           },
    /* VARIANT */          { M::ValueType::NONE, M::ValueType::VARIANT, M::ValueType::VARIANT, M::ValueType::VARIANT, M::ValueType::VARIANT, M::ValueType::VARIANT, M::ValueType::VARIANT, M::ValueType::VARIANT, M::ValueType::VARIANT,        M::ValueType::VARIANT,        M::ValueType::VARIANT,        M::ValueType::VARIANT        },
    /* BOOLEAN */          { M::ValueType::NONE, M::ValueType::VARIANT, M::ValueType::BOOLEAN, M::ValueType::INTEGER, M::ValueType::REAL,    M::ValueType::COMPLEX, M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,           M::ValueType::NONE,           M::ValueType::NONE,           M::ValueType::NONE           },
    /* INTEGER */          { M::ValueType::NONE, M::ValueType::VARIANT, M::ValueType::INTEGER, M::ValueType::INTEGER, M::ValueType::REAL,    M::ValueType::COMPLEX, M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,           M::ValueType::NONE,           M::ValueType::NONE,           M::ValueType::NONE           },
    /* REAL */             { M::ValueType::NONE, M::ValueType::VARIANT, M::ValueType::REAL,    M::ValueType::REAL,    M::ValueType::REAL,    M::ValueType::COMPLEX, M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,           M::ValueType::NONE,           M::ValueType::NONE,           M::ValueType::NONE           },
    /* COMPLEX */          { M::ValueType::NONE, M::ValueType::VARIANT, M::ValueType::COMPLEX, M::ValueType::COMPLEX, M::ValueType::COMPLEX, M::ValueType::COMPLEX, M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,           M::ValueType::NONE,           M::ValueType::NONE,           M::ValueType::NONE           },
    /* SET */              { M::ValueType::NONE, M::ValueType::VARIANT, M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::SET,     M::ValueType::NONE,    M::ValueType::NONE,           M::ValueType::NONE,           M::ValueType::NONE,           M::ValueType::NONE           },
    /* TUPLE */            { M::ValueType::NONE, M::ValueType::VARIANT, M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::TUPLE,   M::ValueType::NONE,           M::ValueType::NONE,           M::ValueType::NONE,           M::ValueType::NONE           },
    /* MATRIX_BOOLEAN */   { M::ValueType::NONE, M::ValueType::VARIANT, M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::MATRIX_BOOLEAN, M::ValueType::MATRIX_INTEGER, M::ValueType::MATRIX_REAL,    M::ValueType::MATRIX_COMPLEX },
    /* MATRIX_INTEGER */   { M::ValueType::NONE, M::ValueType::VARIANT, M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::MATRIX_INTEGER, M::ValueType::MATRIX_INTEGER, M::ValueType::MATRIX_REAL,    M::ValueType::MATRIX_COMPLEX },
    /* MATRIX_REAL */      { M::ValueType::NONE, M::ValueType::VARIANT, M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::MATRIX_REAL,    M::ValueType::MATRIX_REAL,    M::ValueType::MATRIX_REAL,    M::ValueType::MATRIX_COMPLEX },
    /* MATRIX_COMPLEX */   { M::ValueType::NONE, M::ValueType::VARIANT, M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::NONE,    M::ValueType::MATRIX_COMPLEX, M::ValueType::MATRIX_COMPLEX, M::ValueType::MATRIX_COMPLEX, M::ValueType::MATRIX_COMPLEX }
};
