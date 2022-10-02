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

#include "model_exceptions.h"
#include "m_type_conversion.h"
#include "m_exceptions.h"
#include "m_console.h"

namespace M {
    static bool currentEnableFileErrorExceptions             = true;
    static bool currentEnableMalformedStringException        = true;
    static bool currentEnableTypeConversionExceptions        = true;
    static bool currentEnableInvalidParameterValueExceptions = true;
    static bool currentEnableNaNExceptions                   = true;
    static bool currentEnableInfinityExceptions              = true;
    static bool currentEnableCanNotConvergeExceptions        = true;

    void internalEnableFileErrorExceptions(bool enableExceptions) {
        currentEnableFileErrorExceptions = enableExceptions;
    }


    void internalEnableMalformedStringExceptions(bool enableExceptions) {
        currentEnableMalformedStringException = enableExceptions;
    }


    void internalEnableTypeConversionExceptions(bool enableExceptions) {
        currentEnableTypeConversionExceptions = enableExceptions;
    }


    void internalEnableInvalidParameterValueExceptions(bool enableExceptions) {
        currentEnableInvalidParameterValueExceptions = enableExceptions;
    }


    void internalEnableNaNExceptions(bool enableExceptions) {
        currentEnableNaNExceptions = enableExceptions;
    }


    void internalEnableInfinityExceptions(bool enableExceptions) {
        currentEnableInfinityExceptions = enableExceptions;
    }


    void internalEnableCanNotConvergeExceptions(bool enableExceptions) {
        currentEnableCanNotConvergeExceptions = enableExceptions;
    }


    void internalTriggerFileOpenError(const char* filename, int errorNumber) {
        if (currentEnableFileErrorExceptions) {
            throw Model::FileOpenError(filename, errorNumber);
        }
    }


    void internalTriggerFileReadError(const char* filename, int errorNumber) {
        if (currentEnableFileErrorExceptions) {
            throw Model::FileReadError(filename, errorNumber);
        }
    }


    void internalTriggerFileWriteError(const char* filename, int errorNumber) {
        if (currentEnableFileErrorExceptions) {
            throw Model::FileWriteError(filename, errorNumber);
        }
    }


    void internalTriggerFileSeekError(unsigned long long seekOffset, const char* filename, int errorNumber) {
        if (currentEnableFileErrorExceptions) {
            throw Model::FileSeekError(seekOffset, filename, errorNumber);
        }
    }


    void internalTriggerFileCloseError(const char* filename, int errorNumber) {
        if (currentEnableFileErrorExceptions) {
            throw Model::FileCloseError(filename, errorNumber);
        }
    }


    void internalTriggerInvalidFileNumberError(Integer fileNumber) {
        if (currentEnableFileErrorExceptions) {
            throw Model::InvalidFileNumber(fileNumber);
        }
    }


    void internalTriggerUnknownFileTypeError(const char* filename) {
        if (currentEnableFileErrorExceptions) {
            throw Model::UnknownFileType(filename);
        }
    }


    void internalTriggerMalformedStringError(const char* string, unsigned byteOffset) {
        if (currentEnableMalformedStringException) {
            throw Model::MalformedString(string, byteOffset);
        }
    }


    void internalTriggerCanNotConvertToStringError() {
        if (currentEnableMalformedStringException) {
            throw Model::CanNotConvertToString();
        }
    }


    void internalTriggerTypeConversionError(ValueType fromType, ValueType toType) {
        if (currentEnableTypeConversionExceptions) {
            throw Model::InvalidRuntimeConversion(fromType, toType);
        }
    }


    void internalTriggerInvalidParameterValueError() {
        if (currentEnableInvalidParameterValueExceptions) {
            throw Model::InvalidNumericValue();
        }
    }


    void internalTriggerNaNError() {
        if (currentEnableNaNExceptions) {
            throw Model::ResultIsNaN();
        }
    }


    void internalTriggerInfinityError() {
        if (currentEnableInfinityExceptions) {
            throw Model::ResultIsInfinite();
        }
    }


    void internalTriggerCanNotConvergeError() {
        if (currentEnableCanNotConvergeExceptions) {
            throw Model::CanNotConverge();
        }
    }
}
