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
* This header provides functions to conditionally trigger exceptions.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_EXCEPTIONS_H
#define M_EXCEPTIONS_H

#include "m_common.h"
#include "m_api_types.h"
#include "m_intrinsic_types.h"

namespace M {
    /**
     * Method you can use to request an exception on a file error.
     *
     * \param[in] enableExceptions If true, exceptions will be enabled.  If false, exceptions will be disabled.
     */
    M_PUBLIC_API void internalEnableFileErrorExceptions(bool enableExceptions);

    /**
     * Method you can use to request an exception when a malformed string is encountered.
     *
     * \param[in] enableExceptions If true, exceptions will be enabled.  If false, exceptions will be disabled.
     */
    M_PUBLIC_API void internalEnableMalformedStringExceptions(bool enableExceptions);

    /**
     * Method you can use to request an exception on a type conversion error.
     *
     * \param[in] enableExceptions If true, exceptions will be enabled.  If false, exceptions will be disabled.
     */
    M_PUBLIC_API void internalEnableTypeConversionExceptions(bool enableExceptions);

    /**
     * Method you can use to request an exception on an invalid parameter value.
     *
     * \param[in] enableExceptions If true, exceptions will be enabled.  If false, exceptions will be disabled.
     */
    M_PUBLIC_API void internalEnableInvalidParameterValueExceptions(bool enableExceptions);

    /**
     * Method you can use to request an exception when a result is NaN.
     *
     * \param[in] enableExceptions If true, exceptions will be enabled.  If false, exceptions will be disabled.
     */
    M_PUBLIC_API void internalEnableNaNExceptions(bool enableExceptions);

    /**
     * Method you can use to request an exception when a result is infinite.
     *
     * \param[in] enableExceptions If true, exceptions will be enabled.  If false, exceptions will be disabled.
     */
    M_PUBLIC_API void internalEnableInfinityExceptions(bool enableExceptions);

    /**
     * Method you can use to request an exception when a result is non-convergent.
     *
     * \param[in] enableExceptions If true, exceptions will be enabled.  If false, exceptions will be disabled.
     */
    M_PUBLIC_API void internalEnableCanNotConvergeExceptions(bool enableExceptions);

    /**
     * Method that triggers an exception on a file open error.
     *
     * \param[in] filename    The filename of the file that triggered the exception.
     *
     * \param[in] errorNumber An optional error number.
     */
    M_PUBLIC_API void internalTriggerFileOpenError(const char* filename, int errorNumber = 0);

    /**
     * Method that triggers an exception on a file read error.
     *
     * \param[in] filename    The filename of the file that triggered the exception.
     *
     * \param[in] errorNumber An optional error number.
     */
    M_PUBLIC_API void internalTriggerFileReadError(const char* filename, int errorNumber = 0);

    /**
     * Method that triggers an exception on a file write error.
     *
     * \param[in] filename    The filename of the file that triggered the exception.
     *
     * \param[in] errorNumber An optional error number.
     */
    M_PUBLIC_API void internalTriggerFileWriteError(const char* filename, int errorNumber = 0);

    /**
     * Method that triggers an exception on a file seek error.
     *
     * \param[in] seekOffset  The offset into the file that triggered this exception.
     *
     * \param[in] filename    The filename of the file that triggered the exception.
     *
     * \param[in] errorNumber An optional error number.
     */
    M_PUBLIC_API void internalTriggerFileSeekError(
        unsigned long long seekOffset,
        const char*        filename,
        int                errorNumber = 0
    );

    /**
     * Method that triggers an exception on a file close error.
     *
     * \param[in] filename    The filename of the file that triggered the exception.
     *
     * \param[in] errorNumber An optional error number.
     */
    M_PUBLIC_API void internalTriggerFileCloseError(const char* filename, int errorNumber = 0);

    /**
     * Method that triggers an exception on an invalid file number.
     *
     * \param[in] fileNumber The file number that triggered the exception.
     */
    M_PUBLIC_API void internalTriggerInvalidFileNumberError(Integer fileNumber);

    /**
     * Method that triggers an exception on an unknown file type.
     *
     * \param[in] filename The filename of the file that triggered the exception.
     */
    M_PUBLIC_API void internalTriggerUnknownFileTypeError(const char* filename);

    /**
     * Method that triggers an exception on a malformed string.
     *
     * \param[in] string     The string that triggered the exception.  Only the first 128 characters of the string will
     *                       be preserved.
     *
     * \param[in] byteOffset The byte offset into the provided string.
     */
    M_PUBLIC_API void internalTriggerMalformedStringError(const char* string, unsigned byteOffset);

    /**
     * Method that triggers an exception if a data type can not be converted to a string.
     */
    M_PUBLIC_API void internalTriggerCanNotConvertToStringError();

    /**
     * Method that triggers an exception on a type conversion error.
     *
     * \param[in] fromType The type we are converting from.
     *
     * \param[in] toType   The type we are converting to.
     */
    M_PUBLIC_API void internalTriggerTypeConversionError(ValueType fromType, ValueType toType);

    /**
     * Method that triggers an exception on an invalid parameter value.
     */
    M_PUBLIC_API void internalTriggerInvalidParameterValueError();

    /**
     * Method that triggers an exception when a result is NaN.
     */
    M_PUBLIC_API void internalTriggerNaNError();

    /**
     * Method that triggers an exception when a result is infinite.
     */
    M_PUBLIC_API void internalTriggerInfinityError();

    /**
     * Method that triggers an exception when a value can-not converge.
     */
    M_PUBLIC_API void internalTriggerCanNotConvergeError();
}

#endif


