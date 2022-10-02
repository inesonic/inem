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
* This header defines various types required by the model API.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_API_TYPES_H
#define MODEL_API_TYPES_H

namespace Model {
    /**
     * Type used to represent an operation handle.
     */
    typedef unsigned long long OperationHandle;

    /**
     * Type used to represent an identifier handle.
     */
    typedef unsigned IdentifierHandle;

    /**
     * Value used to represent an invalid operation handle.
     */
    static constexpr OperationHandle invalidOperationHandle = static_cast<OperationHandle>(-1);

    /**
     * Value used to represent an invalid identifier handle.
     */
    static constexpr IdentifierHandle invalidIdentifierHandle = static_cast<IdentifierHandle>(-1);

    /**
     * Enumeration of supported base types.
     */
    enum class ValueType : unsigned char {
        /**
         * Indicates no underlying value type.
         */
        NONE = 0,

        /**
         * Indicates a run-time variant type.  Note that this value will never be returned by methods such as
         * \ref Model::IdentifierData::valueType and \ref Model::Variant::valueType that are expected to always resulve
         * to a distinct type.
         *
         * Run-time variants are only used in cases where the compiler can not identify the actual variable type.
         */
        VARIANT = 1,

        /**
         * Indicates a boolean value.
         */
        BOOLEAN = 2,

        /**
         * Indicates an integer value.
         */
        INTEGER = 3,

        /**
         * Indicates a real value.
         */
        REAL = 4,

        /**
         * Indicates a complex value.
         */
        COMPLEX = 5,

        /**
         * Indicates a set.
         */
        SET = 6,

        /**
         * Indicates a tuple.
         */
        TUPLE = 7,

        /**
         * Indicates a matrix of boolean values.
         */
        MATRIX_BOOLEAN = 8,

        /**
         * Indicates a matrix of integer values.
         */
        MATRIX_INTEGER = 9,

        /**
         * Indicatea a matrix of real values.
         */
        MATRIX_REAL = 10,

        /**
         * Indicate a matrix of complex values.
         */
        MATRIX_COMPLEX = 11,

        /**
         * Enumerated value used to indicate the number of known types.  This value must be placed last in the
         * enumeration.
         */
        NUMBER_TYPES = 12
    };

    /**
     * Enumeration of model operating states.
     */
    enum class State : unsigned char {
        /**
         * Indicates the model is not running.
         */
        STOPPED = 0,

        /**
         * Indicates the model is not running and successfully aborted.
         */
        ABORTED = 1,

        /**
         * Indicates the model is actively running.
         */
        ACTIVE = 2,

        /**
         * Indicates the model is paused on user request.
         */
        PAUSED_ON_USER_REQUEST = 3,

        /**
         * Indicates the model is paused at an operation.
         */
        PAUSED_AT_OPERATION = 4,

        /**
         * Indicates the model is paused just prior to writing a variable.
         */
        PAUSED_ON_VARIABLE_UPDATE = 5,

        /**
         * Indicates the model is in the process of aborting.
         */
        ABORTING = 6,

        /**
         * Value indicating the number of state codes.
         */
        NUMBER_STATES = 7
    };

    /**
     * Enumeration of abort reason codes.
     */
    enum class AbortReason : unsigned char {
        /**
         * Indicates no abort reason.
         */
        NONE = 0,

        /**
         * Indicates the model was aborted on user request.
         */
        USER_REQUEST = 1,

        /**
         * Indicates that there is insufficient memory available.
         */
        INSUFFICIENT_MEMORY = 2,

        /**
         * Indicates an unknown system exception.
         */
        SYSTEM = 3,

        /**
         * Indicates an undefined model internal error.
         */
        INTERNAL_ERROR = 4,

        /**
         * Indicates a memory allocation error.
         */
        MEMORY_ALLOCATION_ERROR = 5,

        /**
         * Indicates a file open error.
         */
        FILE_OPEN_ERROR = 6,

        /**
         * Indicates a file read error.
         */
        FILE_READ_ERROR = 7,

        /**
         * Indicates a file write error.
         */
        FILE_WRITE_ERROR = 8,

        /**
         * Indicates an error during a seek.
         */
        FILE_SEEK_ERROR = 9,

        /**
         * Indicates an error during file close.
         */
        FILE_CLOSE_ERROR = 10,

        /**
         * Indicates an invalid file number.
         */
        INVALID_FILE_NUMBER = 11,

        /**
         * Indicates an unrecognized file type.
         */
        UNKNOWN_FILE_TYPE = 12,

        /**
         * Indicates a string was not properly encoded.
         */
        MALFORMED_STRING = 13,

        /**
         * Indicates a tuple can-not be converted into a string.
         */
        CAN_NOT_CONVERT_TO_STRING = 14,

        /**
         * Indicates an invalid run-time type conversion.
         */
        INVALID_RUNTIME_CONVERSION = 15,

        /**
         * Indicates type used as a parameter for a range is invalid.
         */
        INVALID_RANGE_PARAMETER = 16,

        /**
         * Indicates a container class holds contents of an invalid type.
         */
        INVALID_CONTAINER_CONTENTS = 17,

        /**
         * Indicates an invalid row was specified for a matrix.
         */
        INVALID_ROW_INDEX = 18,

        /**
         * Indicates an invalid column index was specified for a matrix.
         */
        INVALID_COLUMN_INDEX = 19,

        /**
         * Indicates an invalid index was specified for a matrix or tuple.
         */
        INVALID_INDEX = 20,

        /**
         * Indicates matrix dimensions were incompatible for the given operation.  This exception is mostly associated
         * with matrix multiplication operations.
         */
        INCOMPATIBLE_MATRIX_DIMENSIONS = 21,

        /**
         * Indicates matrix dimensions were invalid for the given operation.  This exception is mostly associated
         * with such things as calculation of the determinant or inverse of a matrix.
         */
        INVALID_MATRIX_DIMENSIONS = 22,

        /**
         * Indicates an attempt to invert a singular matrix.
         */
        MATRIX_IS_SINGULAR = 23,

        /**
         * Indicates attempt to access a subscript of a scalar type.
         */
        TYPE_DOES_NOT_SUPPORT_SUBSCRIPTS = 24,

        /**
         * Indicates an invalid value.  This value is generally tied to probabilities and other values with an expected
         * limited range.
         */
        INVALID_NUMERIC_VALUE = 25,

        /**
         * Indicates an NaN result.
         */
        RESULT_IS_NAN = 26,

        /**
         * Indicates an infinite result.
         */
        RESULT_IS_INFINITE = 27,

        /**
         * Indicates a function can not converge on a value.
         */
        CAN_NOT_CONVERGE = 28,

        /**
         * Value indicating the number of abort reason codes.
         */
        NUMBER_ABORT_REASONS = 29
    };

    /**
     * Enumeration of supported exception classes.
     */
    enum class ExceptionClass : unsigned char {
        /**
         * Exceptions triggered by file read/write operations.
         */
        FILE_EXCEPTIONS = 0,

        /**
         * Exceptions triggered by a malformed string.
         */
        MALFORMED_STRING_EXCEPTIONS = 1,

        /**
         * Exceptions triggered by run-time type conversion issues.
         */
        TYPE_CONVERSION_EXCEPTIONS = 2,

        /**
         * Exceptions triggered by an invalid function parameter value.
         */
        INVALID_PARAMETER_VALUE_EXCEPTIONS = 3,

        /**
         * Exceptions triggered by an unexpected NaN value.
         */
        NAN_VALUE_EXCEPTIONS = 4,

        /**
         * Exceptions triggered by an unexpected infinity value.
         */
        INFINITY_VALUE_EXCEPTIONS = 5,

        /**
         * Exceptions triggered when a function can-not converge on a value.
         */
        CAN_NOT_CONVERGE_EXCEPTIONS = 6
    };

    /**
     * Enumeration of supported file formats.
     */
    enum class DataFileFormat : unsigned char {
        /**
         * Value indicating an invalid/undefined file format.
         */
        INVALID = 0,

        /**
         * Value indicating a CSV file format.
         */
        CSV = 1,

        /**
         * Value indicating a binary file format.
         */
        BINARY = 2
    };

    /**
     * Enumeration of supported I/O devices.
     */
    enum class Device : unsigned char {
        /**
         * Indicates an audio device.
         */
        AUDIO = 0,
    };
}

#endif
