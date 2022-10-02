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
* This file implements customer visible exceptions thrown by the model classes.
***********************************************************************************************************************/

#include <exception>
#include <cstring>

#include "model_api_types.h"
#include "m_exceptions.h"
#include "model_exceptions.h"

#if (defined(_MSC_VER))

    #pragma warning(disable:4273)

#endif

/***********************************************************************************************************************
 * Global functions:
 */

namespace Model {
    void enableExceptionClass(ExceptionClass exceptionClass, bool nowEnabled) {
        switch (exceptionClass) {
            case ExceptionClass::FILE_EXCEPTIONS: {
                M::internalEnableFileErrorExceptions(nowEnabled);
                break;
            }

            case ExceptionClass::MALFORMED_STRING_EXCEPTIONS: {
                M::internalEnableMalformedStringExceptions(nowEnabled);
                break;
            }

            case ExceptionClass::TYPE_CONVERSION_EXCEPTIONS: {
                M::internalEnableTypeConversionExceptions(nowEnabled);
                break;
            }

            case ExceptionClass::INVALID_PARAMETER_VALUE_EXCEPTIONS: {
                M::internalEnableInvalidParameterValueExceptions(nowEnabled);
                break;
            }

            case ExceptionClass::NAN_VALUE_EXCEPTIONS: {
                M::internalEnableNaNExceptions(nowEnabled);
                break;
            }

            case ExceptionClass::INFINITY_VALUE_EXCEPTIONS: {
                M::internalEnableInfinityExceptions(nowEnabled);
                break;
            }

            case ExceptionClass::CAN_NOT_CONVERGE_EXCEPTIONS: {
                M::internalEnableCanNotConvergeExceptions(nowEnabled);
                break;
            }

            default: {
                throw InvalidNumericValue();
            }
        }
    }


    void disableExceptionClass(ExceptionClass exceptionClass, bool nowDisabled) {
        enableExceptionClass(exceptionClass, !nowDisabled);

    }
}

/***********************************************************************************************************************
 * Model::InesonicException
 */

namespace Model {
    static constexpr unsigned numberValueTypes = static_cast<unsigned>(Model::ValueType::NUMBER_TYPES);

    InesonicException::InesonicException() noexcept {}


    InesonicException::~InesonicException() noexcept {}


    const char* InesonicException::what() const noexcept {
        return "internal error";
    }


    InesonicException::ExceptionType InesonicException::exceptionType() const noexcept {
        return ExceptionType::INTERNAL_ERROR;
    }
}

/***********************************************************************************************************************
 * Model::FileError
 */

namespace Model {
    FileError::FileError(const char* filename, int errorNumber) noexcept {
        std::strncpy(currentFilename, filename, maximumFilenameLength - 1);
        currentFilename[maximumFilenameLength-1] = '\0';

        currentErrorNumber = errorNumber;
    }


    FileError::~FileError() noexcept {}


    const char* FileError::filename() const noexcept {
        return currentFilename;
    }


    int FileError::errorNumber() const noexcept {
        return currentErrorNumber;
    }
}

/***********************************************************************************************************************
 * Model::FileOpenError
 */

namespace Model {
    FileOpenError::FileOpenError(const char* filename, int errorNumber) noexcept:FileError(filename, errorNumber) {}


    FileOpenError::~FileOpenError() noexcept {}


    const char* FileOpenError::what() const noexcept {
        return "file open error";
    }


    InesonicException::ExceptionType FileOpenError::exceptionType() const noexcept {
        return ExceptionType::FILE_OPEN_ERROR;
    }
}

/***********************************************************************************************************************
 * Model::FileReadError
 */

namespace Model {
    FileReadError::FileReadError(const char* filename, int errorNumber) noexcept:FileError(filename, errorNumber) {}


    FileReadError::~FileReadError() noexcept {}


    const char* FileReadError::what() const noexcept {
        return "file read error";
    }


    InesonicException::ExceptionType FileReadError::exceptionType() const noexcept {
        return ExceptionType::FILE_READ_ERROR;
    }
}

/***********************************************************************************************************************
 * Model::FileWriteError
 */

namespace Model {
    FileWriteError::FileWriteError(const char* filename, int errorNumber) noexcept:FileError(filename, errorNumber) {}


    FileWriteError::~FileWriteError() noexcept {}


    const char* FileWriteError::what() const noexcept {
        return "file write error";
    }


    InesonicException::ExceptionType FileWriteError::exceptionType() const noexcept {
        return ExceptionType::FILE_WRITE_ERROR;
    }
}

/***********************************************************************************************************************
 * Model::FileSeekError
 */

namespace Model {
    FileSeekError::FileSeekError(
            unsigned long long seekOffset,
            const char*        filename,
            int                errorNumber
        ) noexcept:FileError(
            filename,
            errorNumber
        ),currentSeekOffset(
            seekOffset
        ) {}


    FileSeekError::~FileSeekError() noexcept {}


    const char* FileSeekError::what() const noexcept {
        return "file seek error";
    }


    InesonicException::ExceptionType FileSeekError::exceptionType() const noexcept {
        return ExceptionType::FILE_SEEK_ERROR;
    }


    unsigned long long FileSeekError::seekOffset() const noexcept {
        return currentSeekOffset;
    }
}

/***********************************************************************************************************************
 * Model::FileCloseError
 */

namespace Model {
    FileCloseError::FileCloseError(const char* filename, int errorNumber) noexcept:FileError(filename, errorNumber) {}


    FileCloseError::~FileCloseError() noexcept {}


    const char* FileCloseError::what() const noexcept {
        return "file close error";
    }


    InesonicException::ExceptionType FileCloseError::exceptionType() const noexcept {
        return ExceptionType::FILE_CLOSE_ERROR;
    }
}

/***********************************************************************************************************************
 * Model::InvalidFileNumber
 */

namespace Model {
    InvalidFileNumber::InvalidFileNumber(Integer fileNumber) noexcept {
        currentFileNumber = fileNumber;
    }


    InvalidFileNumber::~InvalidFileNumber() noexcept {}


    const char* InvalidFileNumber::what() const noexcept {
        return "invalid file number";
    }


    InesonicException::ExceptionType InvalidFileNumber::exceptionType() const noexcept {
        return ExceptionType::INVALID_FILE_NUMBER;
    }


    Integer InvalidFileNumber::fileNumber() const noexcept {
        return currentFileNumber;
    }
}

/***********************************************************************************************************************
 * Model::UnknownFileType
 */

namespace Model {
    UnknownFileType::UnknownFileType(const char* filename) noexcept:FileError(filename, 0) {}


    UnknownFileType::~UnknownFileType() noexcept {}


    const char* UnknownFileType::what() const noexcept {
        return "unknown file type";
    }


    InesonicException::ExceptionType UnknownFileType::exceptionType() const noexcept {
        return ExceptionType::UNKNOWN_FILE_TYPE;
    }
}

/***********************************************************************************************************************
 * Model::MalformedString
 */

namespace Model {
    MalformedString::MalformedString(const char* string, unsigned byteOffset) noexcept {
        currentByteOffset = byteOffset;
        std::strncpy(currentString, string, maximumStringLength);
        currentString[maximumStringLength] = '\0';
    }


    MalformedString::~MalformedString() noexcept {}


    const char* MalformedString::what() const noexcept {
        return "malformed string";
    }


    InesonicException::ExceptionType MalformedString::exceptionType() const noexcept {
        return ExceptionType::MALFORMED_STRING;
    }


    const char* MalformedString::str() const noexcept {
        return currentString;
    }


    unsigned MalformedString::byteOffset() const noexcept {
        return currentByteOffset;
    }
}

/***********************************************************************************************************************
 * Model::CanNotConvertToString
 */

namespace Model {
    CanNotConvertToString::CanNotConvertToString() noexcept {}


    CanNotConvertToString::~CanNotConvertToString() noexcept {}


    const char* CanNotConvertToString::what() const noexcept {
        return "can not convert to string";
    }


    InesonicException::ExceptionType CanNotConvertToString::exceptionType() const noexcept {
        return ExceptionType::CAN_NOT_CONVERT_TO_STRING;
    }
}

/***********************************************************************************************************************
 * Model::UserAbortRequested
 */

namespace Model {
    UserAbortRequested::UserAbortRequested() noexcept {}


    UserAbortRequested::~UserAbortRequested() noexcept {}


    const char* UserAbortRequested::what() const noexcept {
        return "user abort";
    }


    InesonicException::ExceptionType UserAbortRequested::exceptionType() const noexcept {
        return ExceptionType::USER_REQUEST;
    }
}

/***********************************************************************************************************************
 * Model::InsufficientMemory
 */

namespace Model {
    InsufficientMemory::InsufficientMemory() noexcept {}


    InsufficientMemory::~InsufficientMemory() noexcept {}


    const char* InsufficientMemory::what() const noexcept {
        return "insufficient memory";
    }


    InesonicException::ExceptionType InsufficientMemory::exceptionType() const noexcept {
        return ExceptionType::INSUFFICIENT_MEMORY;
    }
}

/***********************************************************************************************************************
 * Model::InvalidRuntimeConversion
 */

namespace Model {
    static const char* invalidConversionString[numberValueTypes][numberValueTypes] = {
        /* sourceValueType == NONE */ {
            "can not convert nothing to nothing",        /* destinationValueType == NONE */
            "can not convert nothing to integer",        /* destinationValueType == INTEGER */
            "can not convert nothing to real",           /* destinationValueType == REAL */
            "can not convert nothing to complex",        /* destinationValueType == COMPLEX */
            "can not convert nothing to set",            /* destinationValueType == SET */
            "can not convert nothing to tuple",          /* destinationValueType == TUPLE */
            "can not convert nothing to boolean matrix", /* destinationValueType == MATRIX_BOOLEAN */
            "can not convert nothing to integer matrix", /* destinationValueType == MATRIX_INTEGER */
            "can not convert nothing to real matrix",    /* destinationValueType == MATRIX_REAL */
            "can not convert nothing to complex matrix"  /* destinationValueType == MATRIX_COMPLEX */
        },
        /* sourceValueType == INTEGER */ {
            "can not convert integer to nothing",        /* destinationValueType == NONE */
            "can not convert integer to integer",        /* destinationValueType == INTEGER */
            "can not convert integer to real",           /* destinationValueType == REAL */
            "can not convert integer to complex",        /* destinationValueType == COMPLEX */
            "can not convert integer to set",            /* destinationValueType == SET */
            "can not convert integer to tuple",          /* destinationValueType == TUPLE */
            "can not convert integer to boolean matrix", /* destinationValueType == MATRIX_BOOLEAN */
            "can not convert integer to integer matrix", /* destinationValueType == MATRIX_INTEGER */
            "can not convert integer to real matrix",    /* destinationValueType == MATRIX_REAL */
            "can not convert integer to complex matrix"  /* destinationValueType == MATRIX_COMPLEX */
        },
        /* sourceValueType == REAL */ {
            "can not convert real to nothing",        /* destinationValueType == NONE */
            "can not convert real to integer",        /* destinationValueType == INTEGER */
            "can not convert real to real",           /* destinationValueType == REAL */
            "can not convert real to complex",        /* destinationValueType == COMPLEX */
            "can not convert real to set",            /* destinationValueType == SET */
            "can not convert real to tuple",          /* destinationValueType == TUPLE */
            "can not convert real to boolean matrix", /* destinationValueType == MATRIX_BOOLEAN */
            "can not convert real to integer matrix", /* destinationValueType == MATRIX_INTEGER */
            "can not convert real to real matrix",    /* destinationValueType == MATRIX_REAL */
            "can not convert real to complex matrix"  /* destinationValueType == MATRIX_COMPLEX */
        },
        /* sourceValueType == COMPLEX */ {
            "can not convert complex to nothing",        /* destinationValueType == NONE */
            "can not convert complex to integer",        /* destinationValueType == INTEGER */
            "can not convert complex to real",           /* destinationValueType == REAL */
            "can not convert complex to complex",        /* destinationValueType == COMPLEX */
            "can not convert complex to set",            /* destinationValueType == SET */
            "can not convert complex to tuple",          /* destinationValueType == TUPLE */
            "can not convert complex to boolean matrix", /* destinationValueType == MATRIX_BOOLEAN */
            "can not convert complex to integer matrix", /* destinationValueType == MATRIX_INTEGER */
            "can not convert complex to real matrix",    /* destinationValueType == MATRIX_REAL */
            "can not convert complex to complex matrix"  /* destinationValueType == MATRIX_COMPLEX */
        },
        /* sourceValueType == SET */ {
            "can not convert set to nothing",        /* destinationValueType == NONE */
            "can not convert set to integer",        /* destinationValueType == INTEGER */
            "can not convert set to real",           /* destinationValueType == REAL */
            "can not convert set to complex",        /* destinationValueType == COMPLEX */
            "can not convert set to set",            /* destinationValueType == SET */
            "can not convert set to tuple",          /* destinationValueType == TUPLE */
            "can not convert set to boolean matrix", /* destinationValueType == MATRIX_BOOLEAN */
            "can not convert set to integer matrix", /* destinationValueType == MATRIX_INTEGER */
            "can not convert set to real matrix",    /* destinationValueType == MATRIX_REAL */
            "can not convert set to complex matrix"  /* destinationValueType == MATRIX_COMPLEX */
        },
        /* sourceValueType == TUPLE */ {
            "can not convert tuple to nothing",        /* destinationValueType == NONE */
            "can not convert tuple to integer",        /* destinationValueType == INTEGER */
            "can not convert tuple to real",           /* destinationValueType == REAL */
            "can not convert tuple to complex",        /* destinationValueType == COMPLEX */
            "can not convert tuple to set",            /* destinationValueType == SET */
            "can not convert tuple to tuple",          /* destinationValueType == TUPLE */
            "can not convert tuple to boolean matrix", /* destinationValueType == MATRIX_BOOLEAN */
            "can not convert tuple to integer matrix", /* destinationValueType == MATRIX_INTEGER */
            "can not convert tuple to real matrix",    /* destinationValueType == MATRIX_REAL */
            "can not convert tuple to complex matrix"  /* destinationValueType == MATRIX_COMPLEX */
        },
        /* sourceValueType == MATRIX_BOOLEAN */ {
            "can not convert boolean matrix to nothing",        /* destinationValueType == NONE */
            "can not convert boolean matrix to integer",        /* destinationValueType == INTEGER */
            "can not convert boolean matrix to real",           /* destinationValueType == REAL */
            "can not convert boolean matrix to complex",        /* destinationValueType == COMPLEX */
            "can not convert boolean matrix to set",            /* destinationValueType == SET */
            "can not convert boolean matrix to tuple",          /* destinationValueType == TUPLE */
            "can not convert boolean matrix to boolean matrix", /* destinationValueType == MATRIX_BOOLEAN */
            "can not convert boolean matrix to integer matrix", /* destinationValueType == MATRIX_INTEGER */
            "can not convert boolean matrix to real matrix",    /* destinationValueType == MATRIX_REAL */
            "can not convert boolean matrix to complex matrix"  /* destinationValueType == MATRIX_COMPLEX */
        },
        /* sourceValueType == MATRIX_INTEGER */ {
            "can not convert integer matrix to nothing",        /* destinationValueType == NONE */
            "can not convert integer matrix to integer",        /* destinationValueType == INTEGER */
            "can not convert integer matrix to real",           /* destinationValueType == REAL */
            "can not convert integer matrix to complex",        /* destinationValueType == COMPLEX */
            "can not convert integer matrix to set",            /* destinationValueType == SET */
            "can not convert integer matrix to tuple",          /* destinationValueType == TUPLE */
            "can not convert integer matrix to boolean matrix", /* destinationValueType == MATRIX_BOOLEAN */
            "can not convert integer matrix to integer matrix", /* destinationValueType == MATRIX_INTEGER */
            "can not convert integer matrix to real matrix",    /* destinationValueType == MATRIX_REAL */
            "can not convert integer matrix to complex matrix"  /* destinationValueType == MATRIX_COMPLEX */
        },
        /* sourceValueType == MATRIX_REAL */ {
            "can not convert real matrix to nothing",        /* destinationValueType == NONE */
            "can not convert real matrix to integer",        /* destinationValueType == INTEGER */
            "can not convert real matrix to real",           /* destinationValueType == REAL */
            "can not convert real matrix to complex",        /* destinationValueType == COMPLEX */
            "can not convert real matrix to set",            /* destinationValueType == SET */
            "can not convert real matrix to tuple",          /* destinationValueType == TUPLE */
            "can not convert real matrix to boolean matrix", /* destinationValueType == MATRIX_BOOLEAN */
            "can not convert real matrix to integer matrix", /* destinationValueType == MATRIX_INTEGER */
            "can not convert real matrix to real matrix",    /* destinationValueType == MATRIX_REAL */
            "can not convert real matrix to complex matrix"  /* destinationValueType == MATRIX_COMPLEX */
        },
        /* sourceValueType == MATRIX_COMPLEX */ {
            "can not convert complex matrix to nothing",        /* destinationValueType == NONE */
            "can not convert complex matrix to integer",        /* destinationValueType == INTEGER */
            "can not convert complex matrix to real",           /* destinationValueType == REAL */
            "can not convert complex matrix to complex",        /* destinationValueType == COMPLEX */
            "can not convert complex matrix to set",            /* destinationValueType == SET */
            "can not convert complex matrix to tuple",          /* destinationValueType == TUPLE */
            "can not convert complex matrix to boolean matrix", /* destinationValueType == MATRIX_BOOLEAN */
            "can not convert complex matrix to integer matrix", /* destinationValueType == MATRIX_INTEGER */
            "can not convert complex matrix to real matrix",    /* destinationValueType == MATRIX_REAL */
            "can not convert complex matrix to complex matrix"  /* destinationValueType == MATRIX_COMPLEX */
        }
    };

    InvalidRuntimeConversion::InvalidRuntimeConversion(
            ValueType sourceValueType,
            ValueType destinationValueType
        ) noexcept
         :currentSourceValueType(
            sourceValueType
        ),currentDestinationValueType (
            destinationValueType
        ) {}


    InvalidRuntimeConversion::~InvalidRuntimeConversion() noexcept {}


    const char* InvalidRuntimeConversion::what() const noexcept {
        return invalidConversionString[static_cast<unsigned>(currentSourceValueType)]
                                      [static_cast<unsigned>(currentDestinationValueType)];
    };


    InesonicException::ExceptionType InvalidRuntimeConversion::exceptionType() const noexcept {
        return ExceptionType::INVALID_RUNTIME_CONVERSION;
    };


    ValueType InvalidRuntimeConversion::sourceValueType() const noexcept {
        return currentSourceValueType;
    }


    ValueType InvalidRuntimeConversion::destinationValueType() const noexcept {
        return currentDestinationValueType;
    }
}

/***********************************************************************************************************************
 * Model::InvalidRangeParameter7
 */

namespace Model {
    static constexpr unsigned numberRangePositions = 3U;

    static const char* invalidRangeParameterStrings[numberRangePositions][numberValueTypes] = {
        /* rangePosition == FIRST */ {
            "can not use nothing as first parameter of range",        /* valueType == NONE */
            "can not use integer as first parameter of range",        /* valueType == INTEGER */
            "can not use real as first parameter of range",           /* valueType == REAL */
            "can not use complex as first parameter of range",        /* valueType == COMPLEX */
            "can not use set as first parameter of range",            /* valueType == SET */
            "can not use tuple as first parameter of range",          /* valueType == TUPLE */
            "can not use boolean matrix as first parameter of range", /* valueType == MATRIX_BOOLEAN */
            "can not use integer matrix as first parameter of range", /* valueType == MATRIX_INTEGER */
            "can not use real matrix as first parameter of range",    /* valueType == MATRIX_REAL */
            "can not use complex matrix as first parameter of range"  /* valueType == MATRIX_COMPLEX */
        },
        /* rangePosition == SECOND */ {
            "can not use nothing as second parameter of range",        /* valueType == NONE */
            "can not use integer as second parameter of range",        /* valueType == INTEGER */
            "can not use real as second parameter of range",           /* valueType == REAL */
            "can not use complex as second parameter of range",        /* valueType == COMPLEX */
            "can not use set as second parameter of range",            /* valueType == SET */
            "can not use tuple as second parameter of range",          /* valueType == TUPLE */
            "can not use boolean matrix as second parameter of range", /* valueType == MATRIX_BOOLEAN */
            "can not use integer matrix as second parameter of range", /* valueType == MATRIX_INTEGER */
            "can not use real matrix as second parameter of range",    /* valueType == MATRIX_REAL */
            "can not use complex matrix as second parameter of range"  /* valueType == MATRIX_COMPLEX */
        },
        /* rangePosition == LAST */ {
            "can not use nothing as last parameter of range",        /* valueType == NONE */
            "can not use integer as last parameter of range",        /* valueType == INTEGER */
            "can not use real as last parameter of range",           /* valueType == REAL */
            "can not use complex as last parameter of range",        /* valueType == COMPLEX */
            "can not use set as last parameter of range",            /* valueType == SET */
            "can not use tuple as last parameter of range",          /* valueType == TUPLE */
            "can not use boolean matrix as last parameter of range", /* valueType == MATRIX_BOOLEAN */
            "can not use integer matrix as last parameter of range", /* valueType == MATRIX_INTEGER */
            "can not use real matrix as last parameter of range",    /* valueType == MATRIX_REAL */
            "can not use complex matrix as last parameter of range"  /* valueType == MATRIX_COMPLEX */
        }
    };

    InvalidRangeParameter::InvalidRangeParameter(
            InvalidRangeParameter::RangePosition rangePosition,
            ValueType                            valueType
        ) noexcept
         :currentRangePosition(
            rangePosition
        ),currentValueType (
            valueType
        ) {}


    InvalidRangeParameter::~InvalidRangeParameter() noexcept {}


    const char* InvalidRangeParameter::what() const noexcept {
        return invalidRangeParameterStrings[static_cast<unsigned>(currentRangePosition)]
                                           [static_cast<unsigned>(currentValueType)];
    };


    InesonicException::ExceptionType InvalidRangeParameter::exceptionType() const noexcept {
        return ExceptionType::INVALID_RANGE_PARAMETER;
    };


    InvalidRangeParameter::RangePosition InvalidRangeParameter::rangePosition() const noexcept {
        return currentRangePosition;
    }


    ValueType InvalidRangeParameter::valueType() const noexcept {
        return currentValueType;
    }
}

/***********************************************************************************************************************
 * Model::InvalidContainerContents
 */

namespace Model {
    static const char* invalidContainerContentsString[numberValueTypes] = {
        "found unexpected container contents of nothing",             /* valueType == NONE */
        "found unexpected container contents of integer type",        /* valueType == INTEGER */
        "found unexpected container contents of real type",           /* valueType == REAL */
        "found unexpected container contents of complex type",        /* valueType == COMPLEX */
        "found unexpected container contents of set type",            /* valueType == SET */
        "found unexpected container contents of tuple type",          /* valueType == TUPLE */
        "found unexpected container contents of boolean matrix type", /* valueType == MATRIX_BOOLEAN */
        "found unexpected container contents of integer matrix type", /* valueType == MATRIX_INTEGER */
        "found unexpected container contents of real matrix type",    /* valueType == MATRIX_REAL */
        "found unexpected container contents of complex matrix type"  /* valueType == MATRIX_COMPLEX */
    };

    InvalidContainerContents::InvalidContainerContents(
            ValueType valueType
        ) noexcept
         :currentValueType (
            valueType
        ) {}


    InvalidContainerContents::~InvalidContainerContents() noexcept {}


    const char* InvalidContainerContents::what() const noexcept {
        return invalidContainerContentsString[static_cast<unsigned>(currentValueType)];
    };


    InesonicException::ExceptionType InvalidContainerContents::exceptionType() const noexcept {
        return ExceptionType::INVALID_CONTAINER_CONTENTS;
    };


    ValueType InvalidContainerContents::valueType() const noexcept {
        return currentValueType;
    }
}

/***********************************************************************************************************************
 * Model::InvalidRow
 */

namespace Model {
    InvalidRow::InvalidRow(Integer rowIndex, Integer numberRows) noexcept {
        currentRowIndex   = rowIndex;
        currentNumberRows = numberRows;
    }


    InvalidRow::~InvalidRow() noexcept {}


    const char* InvalidRow::what() const noexcept {
        return "invalid row index";
    }


    InvalidRow::ExceptionType InvalidRow::exceptionType() const noexcept {
        return ExceptionType::INVALID_ROW_INDEX;
    }


    Integer InvalidRow::rowIndex() const noexcept {
        return currentRowIndex;
    }


    Integer InvalidRow::numberRows() const noexcept {
        return currentNumberRows;
    }
}

/***********************************************************************************************************************
 * Model::InvalidColumn
 */

namespace Model {
    InvalidColumn::InvalidColumn(Integer columnIndex, Integer numberColumns) noexcept {
        currentColumnIndex   = columnIndex;
        currentNumberColumns = numberColumns;
    }


    InvalidColumn::~InvalidColumn() noexcept {}


    const char* InvalidColumn::what() const noexcept {
        return "invalid column index";
    }


    InvalidColumn::ExceptionType InvalidColumn::exceptionType() const noexcept {
        return ExceptionType::INVALID_COLUMN_INDEX;
    }


    Integer InvalidColumn::columnIndex() const noexcept {
        return currentColumnIndex;
    }


    Integer InvalidColumn::numberColumns() const noexcept {
        return currentNumberColumns;
    }
}

/***********************************************************************************************************************
 * Model::InvalidIndex
 */

namespace Model {
    InvalidIndex::InvalidIndex(Integer newIndex, Integer numberEntries) noexcept {
        currentIndex         = newIndex;
        currentNumberEntries = numberEntries;
    }


    InvalidIndex::~InvalidIndex() noexcept {}


    const char* InvalidIndex::what() const noexcept {
        return "invalid index";
    }


    InvalidIndex::ExceptionType InvalidIndex::exceptionType() const noexcept {
        return ExceptionType::INVALID_INDEX;
    }


    Integer InvalidIndex::index() const noexcept {
        return currentIndex;
    }


    Integer InvalidIndex::numberEntries() const noexcept {
        return currentNumberEntries;
    }
}

/***********************************************************************************************************************
 * Model::IncompatibleMatrixDimensions
 */

namespace Model {
    IncompatibleMatrixDimensions::IncompatibleMatrixDimensions(
            Integer multiplierRows,
            Integer multiplierColumns,
            Integer multiplicandRows,
            Integer multiplicandColumns
        ) noexcept {
        currentMultiplierRows      = multiplierRows;
        currentMultiplierColumns   = multiplierColumns;
        currentMultiplicandRows    = multiplicandRows;
        currentMultiplicandColumns = multiplicandColumns;
    }


    IncompatibleMatrixDimensions::~IncompatibleMatrixDimensions() noexcept {}


    const char* IncompatibleMatrixDimensions::what() const noexcept {
        return "incompatible matrix dimensions";
    }


    IncompatibleMatrixDimensions::ExceptionType IncompatibleMatrixDimensions::exceptionType() const noexcept {
        return ExceptionType::INCOMPATIBLE_MATRIX_DIMENSIONS;
    }


    Integer IncompatibleMatrixDimensions::multiplierRows() const noexcept {
        return currentMultiplierRows;
    }


    Integer IncompatibleMatrixDimensions::multiplierColumns() const noexcept {
        return currentMultiplierColumns;
    }


    Integer IncompatibleMatrixDimensions::multiplicandRows() const noexcept {
        return currentMultiplicandRows;
    }


    Integer IncompatibleMatrixDimensions::multiplicandColumns() const noexcept {
        return currentMultiplicandColumns;
    }
}

/***********************************************************************************************************************
 * Model::InvalidMatrixDimensions
 */

namespace Model {
    InvalidMatrixDimensions::InvalidMatrixDimensions(Integer numberRows, Integer numberColumns) noexcept {
        currentNumberRows      = numberRows;
        currentNumberColumns   = numberColumns;
    }


    InvalidMatrixDimensions::~InvalidMatrixDimensions() noexcept {}


    const char* InvalidMatrixDimensions::what() const noexcept {
        return "invalid matrix dimensions";
    }


    InvalidMatrixDimensions::ExceptionType InvalidMatrixDimensions::exceptionType() const noexcept {
        return ExceptionType::INVALID_MATRIX_DIMENSIONS;
    }


    Integer InvalidMatrixDimensions::numberRows() const noexcept {
        return currentNumberRows;
    }


    Integer InvalidMatrixDimensions::numberColumns() const noexcept {
        return currentNumberColumns;
    }
}

/***********************************************************************************************************************
 * Model::MatrixIsSingular
 */

namespace Model {
    MatrixIsSingular::MatrixIsSingular() noexcept {}


    MatrixIsSingular::~MatrixIsSingular() noexcept {}


    const char* MatrixIsSingular::what() const noexcept {
        return "matrix is singular";
    }


    MatrixIsSingular::ExceptionType MatrixIsSingular::exceptionType() const noexcept {
        return ExceptionType::MATRIX_IS_SINGULAR;
    }
}

/***********************************************************************************************************************
 * Model::TypeDoesNotSupportSubscripts
 */

namespace Model {
    static const char* typeDoesNotSupportSubscriptStrings[numberValueTypes] = {
        "type nothing does not support subscripts",        /* valueType == NONE */
        "type integer does not support subscripts",        /* valueType == INTEGER */
        "type real does not support subscripts",           /* valueType == REAL */
        "type complex does not support subscripts",        /* valueType == COMPLEX */
        "type set does not support subscripts",            /* valueType == SET */
        "type tuple does not support subscripts",          /* valueType == TUPLE */
        "type boolean matrix does not support subscripts", /* valueType == MATRIX_BOOLEAN */
        "type integer matrix does not support subscripts", /* valueType == MATRIX_INTEGER */
        "type real matrix does not support subscripts",    /* valueType == MATRIX_REAL */
        "type complex matrix does not support subscripts"  /* valueType == MATRIX_COMPLEX */
    };

    TypeDoesNotSupportSubscripts::TypeDoesNotSupportSubscripts(ValueType valueType) noexcept {
        currentValueType = valueType;
    }


    TypeDoesNotSupportSubscripts::~TypeDoesNotSupportSubscripts() noexcept {}


    const char* TypeDoesNotSupportSubscripts::what() const noexcept {
        return typeDoesNotSupportSubscriptStrings[static_cast<unsigned char>(currentValueType)];
    }


    TypeDoesNotSupportSubscripts::ExceptionType TypeDoesNotSupportSubscripts::exceptionType() const noexcept {
        return ExceptionType::MATRIX_IS_SINGULAR;
    }


    ValueType TypeDoesNotSupportSubscripts::valueType() const noexcept {
        return currentValueType;
    }
}

/***********************************************************************************************************************
 * Model::InvalidNumericValue
 */

namespace Model {
    InvalidNumericValue::InvalidNumericValue() noexcept {}


    InvalidNumericValue::~InvalidNumericValue() noexcept {}


    const char* InvalidNumericValue::what() const noexcept {
        return "invalid numeric value";
    }


    InvalidNumericValue::ExceptionType InvalidNumericValue::exceptionType() const noexcept {
        return ExceptionType::INVALID_NUMERIC_VALUE;
    }
}

/***********************************************************************************************************************
 * Model::ResultIsNaN
 */

namespace Model {
    ResultIsNaN::ResultIsNaN() noexcept {}


    ResultIsNaN::~ResultIsNaN() noexcept {}


    const char* ResultIsNaN::what() const noexcept {
        return "result is nan";
    }


    ResultIsNaN::ExceptionType ResultIsNaN::exceptionType() const noexcept {
        return ExceptionType::RESULT_IS_NAN;
    }
}

/***********************************************************************************************************************
 * Model::ResultIsInfinite
 */

namespace Model {
    ResultIsInfinite::ResultIsInfinite() noexcept {}


    ResultIsInfinite::~ResultIsInfinite() noexcept {}


    const char* ResultIsInfinite::what() const noexcept {
        return "result is infinite";
    }


    ResultIsInfinite::ExceptionType ResultIsInfinite::exceptionType() const noexcept {
        return ExceptionType::RESULT_IS_INFINITE;
    }
}

/***********************************************************************************************************************
 * Model::CanNotConverge
 */

namespace Model {
    CanNotConverge::CanNotConverge() noexcept {}


    CanNotConverge::~CanNotConverge() noexcept {}


    const char* CanNotConverge::what() const noexcept {
        return "can-not converge";
    }


    CanNotConverge::ExceptionType CanNotConverge::exceptionType() const noexcept {
        return ExceptionType::CAN_NOT_CONVERGE;
    }
}
