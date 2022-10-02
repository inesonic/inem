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
* This header defines customer visible exceptions thrown by model classes.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_EXCEPTIONS_H
#define MODEL_EXCEPTIONS_H

#include <exception>

#include "model_common.h"
#include "model_api_types.h"
#include "model_api.h"

#if (defined(_MSC_VER))

    #pragma warning(push)
    #pragma warning(disable: 4275)

#endif

namespace Model {
    /**
     * Function you can use to enable or disable specific classes of exceptions.
     *
     * \param[in] exceptionClass The class to be enabled.
     *
     * \param[in] nowEnabled     If true, the class will be enabled.  If false, the class will be disabled.
     */
    MODEL_PUBLIC_API void enableExceptionClass(ExceptionClass exceptionClass, bool nowEnabled = true);

    /**
     * Function you can use to disable or enable specific classes of exceptions.
     *
     * \param[in] exceptionClass The class to be enabled.
     *
     * \param[in] nowDisabled    If true, the class will be disabled.  If false, the class will be enabled.
     */
     MODEL_PUBLIC_API void disableExceptionClass(ExceptionClass exceptionClass, bool nowDisabled = true);

    /**
     * Pure virtual base class for all exceptions thrown by a customer model.
     */
    class MODEL_PUBLIC_API InesonicException:public std::exception {
        public:
            /**
             * Enumeration of exception codes.
             */
            typedef Model::AbortReason ExceptionType;

            InesonicException() noexcept;

            ~InesonicException() noexcept override;

            /**
             * Method that determines the name of the exception.
             *
             * \return Returns a textual description of the exception.
             */
            const char* what() const noexcept override;

            /**
             * Method that returns the exception type code.
             *
             * \return Returns the exception type code.
             */
            virtual ExceptionType exceptionType() const noexcept;
    };

    /**
     * Base class for file exceptions.
     */
    class MODEL_PUBLIC_API FileError:public InesonicException {
        public:
            /**
             * The maximum filename length.  Values longer than this will be truncated.
             */
            static constexpr unsigned maximumFilenameLength = 256;

            /**
             * Constructor
             *
             * \param[in] filename    The filename of the file that triggered the exception.
             *
             * \param[in] errorNumber An optional error number.
             */
            FileError(const char* filename, int errorNumber) noexcept;

            ~FileError() noexcept override;

            /**
             * Method that returns the filename of the file that triggered the exception.
             *
             * \return Returns the filename of the file that triggered the exception.
             */
            const char* filename() const noexcept;

            /**
             * Method that returns the error number associated with the error.
             *
             * \return Returns the ANSI-C error number associated with the error.
             */
            int errorNumber() const noexcept;

        private:
            /**
             * The filename.
             */
            char currentFilename[maximumFilenameLength];

            /**
             * The error number.
             */
            int currentErrorNumber;
    };

    /**
     * Class used to indicate a file open error.
     */
    class MODEL_PUBLIC_API FileOpenError:public FileError {
        public:
            /**
             * Constructor
             *
             * \param[in] filename    The filename of the file that triggered the exception.
             *
             * \param[in] errorNumber An optional error number.
             */
            FileOpenError(const char* filename, int errorNumber = 0) noexcept;

            ~FileOpenError() noexcept override;

            /**
             * Method that determines the name of the exception.
             *
             * \return Returns a textual description of the exception.
             */
            const char* what() const noexcept override;

            /**
             * Method that returns the exception type code.
             *
             * \return Returns the exception type code.
             */
            ExceptionType exceptionType() const noexcept override;
    };

    /**
     * Class used to indicate a file read error.
     */
    class MODEL_PUBLIC_API FileReadError:public FileError {
        public:
            /**
             * Constructor
             *
             * \param[in] filename    The filename of the file that triggered the exception.
             *
             * \param[in] errorNumber An optional error number.
             */
            FileReadError(const char* filename, int errorNumber = 0) noexcept;

            ~FileReadError() noexcept override;

            /**
             * Method that determines the name of the exception.
             *
             * \return Returns a textual description of the exception.
             */
            const char* what() const noexcept override;

            /**
             * Method that returns the exception type code.
             *
             * \return Returns the exception type code.
             */
            ExceptionType exceptionType() const noexcept override;
    };

    /**
     * Class used to indicate a file write error.
     */
    class MODEL_PUBLIC_API FileWriteError:public FileError {
        public:
            /**
             * Constructor
             *
             * \param[in] filename    The filename of the file that triggered the exception.
             *
             * \param[in] errorNumber An optional error number.
             */
            FileWriteError(const char* filename, int errorNumber = 0) noexcept;

            ~FileWriteError() noexcept override;

            /**
             * Method that determines the name of the exception.
             *
             * \return Returns a textual description of the exception.
             */
            const char* what() const noexcept override;

            /**
             * Method that returns the exception type code.
             *
             * \return Returns the exception type code.
             */
            ExceptionType exceptionType() const noexcept override;
    };

    /**
     * Class used to indicate a file seek error.
     */
    class MODEL_PUBLIC_API FileSeekError:public FileError {
        public:
            /**
             * Constructor
             *
             * \param[in] seekOffset  The offset that failed the seek.
             *
             * \param[in] errorNumber An optional error number.
             *
             * \param[in] filename    The filename of the file that triggered the exception.
             */
            FileSeekError(unsigned long long seekOffset, const char* filename, int errorNumber = 0) noexcept;

            ~FileSeekError() noexcept override;

            /**
             * Method that determines the name of the exception.
             *
             * \return Returns a textual description of the exception.
             */
            const char* what() const noexcept override;

            /**
             * Method that returns the exception type code.
             *
             * \return Returns the exception type code.
             */
            ExceptionType exceptionType() const noexcept override;

            /**
             * Method that returns the failed seek location.
             *
             * \return Returns the seek offset.
             */
            unsigned long long seekOffset() const noexcept;

        private:
            /**
             * The current seek offset.
             */
            unsigned long long currentSeekOffset;
    };

    /**
     * Class used to indicate a file close error.
     */
    class MODEL_PUBLIC_API FileCloseError:public FileError {
        public:
            /**
             * Constructor
             *
             * \param[in] filename    The filename of the file that triggered the exception.
             *
             * \param[in] errorNumber An optional error number.
             */
            FileCloseError(const char* filename, int errorNumber = 0) noexcept;

            ~FileCloseError() noexcept override;

            /**
             * Method that determines the name of the exception.
             *
             * \return Returns a textual description of the exception.
             */
            const char* what() const noexcept override;

            /**
             * Method that returns the exception type code.
             *
             * \return Returns the exception type code.
             */
            ExceptionType exceptionType() const noexcept override;
    };

    /**
     * Class used to indicate an invalid file number.
     */
    class MODEL_PUBLIC_API InvalidFileNumber:public InesonicException {
        public:
            /**
             * Constructor
             *
             * \param[in] fileNumber  The file number that triggered the exception.
             */
            InvalidFileNumber(Integer fileNumber) noexcept;

            ~InvalidFileNumber() noexcept override;

            /**
             * Method that determines the name of the exception.
             *
             * \return Returns a textual description of the exception.
             */
            const char* what() const noexcept override;

            /**
             * Method that returns the exception type code.
             *
             * \return Returns the exception type code.
             */
            ExceptionType exceptionType() const noexcept override;

            /**
             * Method that provides the reported invalid file number.
             *
             * \return Returns the invalid file number.
             */
            Integer fileNumber() const noexcept;

        private:
            /**
             * The reported file number.
             */
            Integer currentFileNumber;
    };

    /**
     * Class used to indicate an unknown file type.
     */
    class MODEL_PUBLIC_API UnknownFileType:public FileError {
        public:
            /**
             * Constructor
             *
             * \param[in] filename The filename of the file that triggered the exception.
             */
            UnknownFileType(const char* filename) noexcept;

            ~UnknownFileType() noexcept override;

            /**
             * Method that determines the name of the exception.
             *
             * \return Returns a textual description of the exception.
             */
            const char* what() const noexcept override;

            /**
             * Method that returns the exception type code.
             *
             * \return Returns the exception type code.
             */
            ExceptionType exceptionType() const noexcept override;
    };

    /**
     * Class used to indicate a malformed string.
     */
    class MODEL_PUBLIC_API MalformedString:public InesonicException {
        public:
            /**
             * Constructor.
             *
             * \param[in] string     The string to be copied.  Only the first 128 characters of the string will be
             *                       preserved.
             *
             * \param[in] byteOffset The byte offset into the provided string.
             */
            MalformedString(const char* string, unsigned byteOffset) noexcept;

            ~MalformedString() noexcept override;

            /**
             * Method that determines the name of the exception.
             *
             * \return Returns a textual description of the exception.
             */
            const char* what() const noexcept override;

            /**
             * Method that returns the exception type code.
             *
             * \return Returns the exception type code.
             */
            ExceptionType exceptionType() const noexcept override;

            /**
             * Method that provides the string, up-to 128 bytes in length.
             *
             * \return Returns the first 128 bytes of the malformed string.
             */
            const char* str() const noexcept;

            /**
             * Method that provides the byte offset into the string.
             *
             * \return Returns the zero based byte offset into the string.
             */
            unsigned byteOffset() const noexcept;

        private:
            /**
             * The maximum supported string length.
             */
            static constexpr unsigned maximumStringLength = 128;

            /**
             * The byte offset into the string.
             */
            unsigned currentByteOffset;

            /**
             * The first part of the the malformed string.
             */
            char currentString[maximumStringLength + 1];
    };

    /**
     * Class used to indicate that a data structure can-not be converted to a string.
     */
    class MODEL_PUBLIC_API CanNotConvertToString:public InesonicException {
        public:
            /**
             * Constructor.
             */
            CanNotConvertToString() noexcept;

            ~CanNotConvertToString() noexcept override;

            /**
             * Method that determines the name of the exception.
             *
             * \return Returns a textual description of the exception.
             */
            const char* what() const noexcept override;

            /**
             * Method that returns the exception type code.
             *
             * \return Returns the exception type code.
             */
            ExceptionType exceptionType() const noexcept override;
    };

    /**
     * Exception thrown when the user requests an abort.
     */
    class MODEL_PUBLIC_API UserAbortRequested:public InesonicException {
        public:
            UserAbortRequested() noexcept;

            ~UserAbortRequested() noexcept override;

            /**
             * Method that determines the name of the exception.
             *
             * \return Returns a textual description of the exception.
             */
            const char* what() const noexcept override;

            /**
             * Method that returns the exception type code.
             *
             * \return Returns the exception type code.
             */
            ExceptionType exceptionType() const noexcept override;
    };

    /**
     * Exception thrown when memory could not be allocated within a user's model.
     */
    class MODEL_PUBLIC_API InsufficientMemory:public InesonicException {
        public:
            InsufficientMemory() noexcept;

            ~InsufficientMemory() noexcept override;

            /**
             * Method that determines the name of the exception.
             *
             * \return Returns a textual description of the exception.
             */
            const char* what() const noexcept override;

            /**
             * Method that returns the exception type code.
             *
             * \return Returns the exception type code.
             */
            ExceptionType exceptionType() const noexcept override;
    };

    /**
     * Exception thrown when an invalid run-time type conversion is requested.
     */
    class MODEL_PUBLIC_API InvalidRuntimeConversion:public InesonicException {
        public:
            /**
             * Constructor
             *
             * \param[in] sourceValueType      The value type of the source for the conversion.
             *
             * \param[in] destinationValueType The value type of the destination for the conversion.
             */
            InvalidRuntimeConversion(
                ValueType sourceValueType,
                ValueType destinationValueType
            ) noexcept;

            ~InvalidRuntimeConversion() noexcept override;

            /**
             * Method that determines the name of the exception.
             *
             * \return Returns a textual description of the exception.
             */
            const char* what() const noexcept override;

            /**
             * Method that returns the exception type code.
             *
             * \return Returns the exception type code.
             */
            ExceptionType exceptionType() const noexcept override;

            /**
             * Method that returns the source value type.
             *
             * \return Returns the source value type.
             */
            ValueType sourceValueType() const noexcept;

            /**
             * Method that returns the destination value type.
             *
             * \return Returns the destination value type.
             */
            ValueType destinationValueType() const noexcept;

        private:
            /**
             * The reported source value type.
             */
            ValueType currentSourceValueType;

            /**
             * The reported destination value type.
             */
            ValueType currentDestinationValueType;
    };

    /**
     * Exception thrown when an invalid range parameter is used.
     */
    class MODEL_PUBLIC_API InvalidRangeParameter:public InesonicException {
        public:
            /**
             * Enumeration of range positions.
             */
            enum class RangePosition {
                /**
                 * Indicates the range position was not determined.
                 */
                NOT_DETERMINED,

                /**
                 * Indicates the first range position.
                 */
                FIRST,

                /**
                 * Indicates the second range position.
                 */
                SECOND,

                /**
                 * Indicates the last range position.
                 */
                LAST
            };

            /**
             * Constructor
             *
             * \param[in] rangePosition The range position that threw the error.
             *
             * \param[in] valueType     The value type of the source for the conversion.
             */
            InvalidRangeParameter(
                RangePosition rangePosition = RangePosition::NOT_DETERMINED,
                ValueType     valueType = ValueType::NONE
            ) noexcept;

            ~InvalidRangeParameter() noexcept override;

            /**
             * Method that determines the name of the exception.
             *
             * \return Returns a textual description of the exception.
             */
            const char* what() const noexcept override;

            /**
             * Method that returns the exception type code.
             *
             * \return Returns the exception type code.
             */
            ExceptionType exceptionType() const noexcept override;

            /**
             * Method that returns the position of the range error.
             *
             * \return Returns the position of the range error.
             */
            RangePosition rangePosition() const noexcept;

            /**
             * Method that returns the source value type.
             *
             * \return Returns the source value type.
             */
            ValueType valueType() const noexcept;

        private:
            /**
             * The reported range position.
             */
            RangePosition currentRangePosition;

            /**
             * The reported destination value type.
             */
            ValueType currentValueType;
    };

    /**
     * Exception thrown when a container holds contents of an invalid type.
     */
    class MODEL_PUBLIC_API InvalidContainerContents:public InesonicException {
        public:
            /**
             * Constructor
             *
             * \param[in] valueType The data type that was encountered.
             */
            InvalidContainerContents(ValueType valueType) noexcept;

            ~InvalidContainerContents() noexcept override;

            /**
             * Method that determines the name of the exception.
             *
             * \return Returns a textual description of the exception.
             */
            const char* what() const noexcept override;

            /**
             * Method that returns the exception type code.
             *
             * \return Returns the exception type code.
             */
            ExceptionType exceptionType() const noexcept override;

            /**
             * Method that returns the source value type.
             *
             * \return Returns the source value type.
             */
            ValueType valueType() const noexcept;

        private:
            /**
             * The reported destination value type.
             */
            ValueType currentValueType;
    };

    /**
     * Exception thrown when an invalid row is specified for a matrix.
     */
    class MODEL_PUBLIC_API InvalidRow:public InesonicException {
        public:
            /**
             * Constructor
             *
             * \param[in] rowIndex   The row index that was specified.
             *
             * \param[in] numberRows The number of rows.
             */
            InvalidRow(Integer rowIndex, Integer numberRows) noexcept;

            ~InvalidRow() noexcept override;

            /**
             * Method that determines the name of the exception.
             *
             * \return Returns a textual description of the exception.
             */
            const char* what() const noexcept override;

            /**
             * Method that returns the exception type code.
             *
             * \return Returns the exception type code.
             */
            ExceptionType exceptionType() const noexcept override;

            /**
             * Method you can use to determine the row index that triggered the exception.
             *
             * \return Returns the row index value that triggered the exception.
             */
            Integer rowIndex() const noexcept;

            /**
             * Method you can use to determine the reported number of rows.
             *
             * \return Returns the reported number of rows.
             */
            Integer numberRows() const noexcept;

        private:
            /**
             * The reported row index.
             */
            Integer currentRowIndex;

            /**
             * The number of available rows.
             */
            Integer currentNumberRows;
    };

    /**
     * Exception thrown when an invalid column is specified for a matrix.
     */
    class MODEL_PUBLIC_API InvalidColumn:public InesonicException {
        public:
            /**
             * Constructor
             *
             * \param[in] columnIndex   The column index that was specified.
             *
             * \param[in] numberColumns The number of rows.
             */
            InvalidColumn(Integer columnIndex, Integer numberColumns) noexcept;

            ~InvalidColumn() noexcept override;

            /**
             * Method that determines the name of the exception.
             *
             * \return Returns a textual description of the exception.
             */
            const char* what() const noexcept override;

            /**
             * Method that returns the exception type code.
             *
             * \return Returns the exception type code.
             */
            ExceptionType exceptionType() const noexcept override;

            /**
             * Method you can use to determine the column index that triggered the exception.
             *
             * \return Returns the column index value that triggered the exception.
             */
            Integer columnIndex() const noexcept;

            /**
             * Method you can use to determine the reported number of columns.
             *
             * \return Returns the reported number of columns.
             */
            Integer numberColumns() const noexcept;

        private:
            /**
             * The reported column index.
             */
            Integer currentColumnIndex;

            /**
             * The numbefr of available columns.
             */
            Integer currentNumberColumns;
    };

    /**
     * Exception thrown when an invalid element index is specified for a matrix or tuple.
     */
    class MODEL_PUBLIC_API InvalidIndex:public InesonicException {
        public:
            /**
             * Constructor
             *
             * \param[in] newIndex      The index that was specified.
             *
             * \param[in] numberEntries The total number of entries in the matrix or tuple.
             */
            InvalidIndex(Integer newIndex, Integer numberEntries) noexcept;

            ~InvalidIndex() noexcept override;

            /**
             * Method that determines the name of the exception.
             *
             * \return Returns a textual description of the exception.
             */
            const char* what() const noexcept override;

            /**
             * Method that returns the exception type code.
             *
             * \return Returns the exception type code.
             */
            ExceptionType exceptionType() const noexcept override;

            /**
             * Method you can use to determine the index that triggered the exception.
             *
             * \return Returns the index value that triggered the exception.
             */
            Integer index() const noexcept;

            /**
             * Method you can use to determine the reported number of matrix/tuple entries.
             *
             * \return Returns the reported number of matrix or tuple entries.
             */
            Integer numberEntries() const noexcept;

        private:
            /**
             * The reported index.
             */
            Integer currentIndex;

            /**
             * The number of available entries.
             */
            Integer currentNumberEntries;
    };

    /**
     * Exception thrown when a multiplication is attempted with matricies of incompatible size.
     */
    class MODEL_PUBLIC_API IncompatibleMatrixDimensions:public InesonicException {
        public:
            /**
             * Constructor
             *
             * \param[in] multiplierRows      The number of rows in the multiplier (left term).
             *
             * \param[in] multiplierColumns   The number of columns in the multiplier (left term).
             *
             * \param[in] multiplicandRows    The number of rows in the multiplicand (right term).
             *
             * \param[in] multiplicandColumns The number of terms in the multiplicand (right term).
             */
            IncompatibleMatrixDimensions(
                Integer multiplierRows,
                Integer multiplierColumns,
                Integer multiplicandRows,
                Integer multiplicandColumns
            ) noexcept;

            ~IncompatibleMatrixDimensions() noexcept override;

            /**
             * Method that determines the name of the exception.
             *
             * \return Returns a textual description of the exception.
             */
            const char* what() const noexcept override;

            /**
             * Method that returns the exception type code.
             *
             * \return Returns the exception type code.
             */
            ExceptionType exceptionType() const noexcept override;

            /**
             * Method you can use to determine the number of multiplier rows.
             *
             * \return Returns the number of multiplier rows.
             */
            Integer multiplierRows() const noexcept;

            /**
             * Method you can use to determine the number of multiplier columns.
             *
             * \return Returns the number of multiplier columns.
             */
            Integer multiplierColumns() const noexcept;

            /**
             * Method you can use to determine the number of multiplicand rows.
             *
             * \return Returns the number of multiplicand rows.
             */
            Integer multiplicandRows() const noexcept;

            /**
             * Method you can use to determine the number of multiplicand columns.
             *
             * \return Returns the number of multiplicand columns.
             */
            Integer multiplicandColumns() const noexcept;

        private:
            /**
             * The current number of multiplier rows.
             */
            Integer currentMultiplierRows;

            /**
             * The current number of multiplier columns.
             */
            Integer currentMultiplierColumns;

            /**
             * The current number of multiplicand rows.
             */
            Integer currentMultiplicandRows;

            /**
             * The current number of multiplicand columns.
             */
            Integer currentMultiplicandColumns;
    };

    /**
     * Exception thrown when a matrix has dimensions that are incompatible with a requested operation.  Examples
     * include such things as attempting to calculating the inverse or determinant of a matrix that is not square.
     */
    class MODEL_PUBLIC_API InvalidMatrixDimensions:public InesonicException {
        public:
            /**
             * Constructor
             *
             * \param[in] numberRows    The number of matrix rows.
             *
             * \param[in] numberColumns The number of matrix columns.
             */
            InvalidMatrixDimensions(Integer numberRows, Integer numberColumns) noexcept;

            ~InvalidMatrixDimensions() noexcept override;

            /**
             * Method that determines the name of the exception.
             *
             * \return Returns a textual description of the exception.
             */
            const char* what() const noexcept override;

            /**
             * Method that returns the exception type code.
             *
             * \return Returns the exception type code.
             */
            ExceptionType exceptionType() const noexcept override;

            /**
             * Method you can use to determine the number of matrix rows.
             *
             * \return Returns the number of matrix rows.
             */
            Integer numberRows() const noexcept;

            /**
             * Method you can use to determine the number of matrix columns.
             *
             * \return Returns the number of matrix columns.
             */
            Integer numberColumns() const noexcept;

        private:
            /**
             * The current number of rows.
             */
            Integer currentNumberRows;

            /**
             * The current number of columns.
             */
            Integer currentNumberColumns;
    };

    /**
     * Exception thrown when a singular matrix is inverted.
     */
    class MODEL_PUBLIC_API MatrixIsSingular:public InesonicException {
        public:
            MatrixIsSingular() noexcept;

            ~MatrixIsSingular() noexcept override;

            /**
             * Method that determines the name of the exception.
             *
             * \return Returns a textual description of the exception.
             */
            const char* what() const noexcept override;

            /**
             * Method that returns the exception type code.
             *
             * \return Returns the exception type code.
             */
            ExceptionType exceptionType() const noexcept override;
    };

    /**
     * Exception thrown when a subscripting operator is applied to a variant holding a scalar type.
     */
    class MODEL_PUBLIC_API TypeDoesNotSupportSubscripts:public InesonicException {
        public:
            /**
             * Constructor
             *
             * \param[in] valueType The data type that was encountered.
             */
            TypeDoesNotSupportSubscripts(ValueType valueType) noexcept;

            ~TypeDoesNotSupportSubscripts() noexcept override;

            /**
             * Method that determines the name of the exception.
             *
             * \return Returns a textual description of the exception.
             */
            const char* what() const noexcept override;

            /**
             * Method that returns the exception type code.
             *
             * \return Returns the exception type code.
             */
            ExceptionType exceptionType() const noexcept override;

            /**
             * Method that returns the value type held by the variant.
             *
             * \return Returns the variant value type.
             */
            ValueType valueType() const noexcept;

        private:
            /**
             * The reported value type.
             */
            ValueType currentValueType;
    };

    /**
     * Exception thrown when an invalid numeric value is encountered.
     */
    class MODEL_PUBLIC_API InvalidNumericValue:public InesonicException {
        public:
            InvalidNumericValue() noexcept;

            ~InvalidNumericValue() noexcept override;

            /**
             * Method that determines the name of the exception.
             *
             * \return Returns a textual description of the exception.
             */
            const char* what() const noexcept override;

            /**
             * Method that returns the exception type code.
             *
             * \return Returns the exception type code.
             */
            ExceptionType exceptionType() const noexcept override;
    };

    /**
     * Exception thrown when a result is NaN.
     */
    class MODEL_PUBLIC_API ResultIsNaN:public InesonicException {
        public:
            ResultIsNaN() noexcept;

            ~ResultIsNaN() noexcept override;

            /**
             * Method that determines the name of the exception.
             *
             * \return Returns a textual description of the exception.
             */
            const char* what() const noexcept override;

            /**
             * Method that returns the exception type code.
             *
             * \return Returns the exception type code.
             */
            ExceptionType exceptionType() const noexcept override;
    };

    /**
     * Exception thrown when a result is infinite.
     */
    class MODEL_PUBLIC_API ResultIsInfinite:public InesonicException {
        public:
            ResultIsInfinite() noexcept;

            ~ResultIsInfinite() noexcept override;

            /**
             * Method that determines the name of the exception.
             *
             * \return Returns a textual description of the exception.
             */
            const char* what() const noexcept override;

            /**
             * Method that returns the exception type code.
             *
             * \return Returns the exception type code.
             */
            ExceptionType exceptionType() const noexcept override;
    };

    /**
     * Exception thrown when a function can-not converge to a value.
     */
    class MODEL_PUBLIC_API CanNotConverge:public InesonicException {
        public:
            CanNotConverge() noexcept;

            ~CanNotConverge() noexcept override;

            /**
             * Method that determines the name of the exception.
             *
             * \return Returns a textual description of the exception.
             */
            const char* what() const noexcept override;

            /**
             * Method that returns the exception type code.
             *
             * \return Returns the exception type code.
             */
            ExceptionType exceptionType() const noexcept override;
    };
}

#if (defined(_MSC_VER))

    #pragma warning(pop)

#endif

#endif
