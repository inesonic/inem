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
* This header defines functions used to load and save to/from files.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_FILE_FUNCTIONS_H
#define M_FILE_FUNCTIONS_H

#include "m_common.h"
#include "m_intrinsics.h"
#include "m_intrinsic_types.h"
#include "m_type_conversion.h"
#include "m_tuple.h"
#include "m_matrix_boolean.h"
#include "m_matrix_integer.h"
#include "m_matrix_real.h"
#include "m_matrix_complex.h"
#include "m_variant.h"

namespace M {
    /**
     * Function you can use to open a file for reading.
     *
     * \param[in] filename The name of the file to be opened.
     *
     * \param[in] binary   If true, the file will be opened as a binary file.  If false, the file will be opened as
     *                     a text file.
     *
     * \return Returns an integer value you can use to reference the file later.
     */
    M_PUBLIC_API Integer fileOpenRead(const Tuple& filename, Boolean binary = false);

    /**
     * Function you can use to open a file for readling and writing.  This version will truncate the file if it exists.
     *
     * \param[in] filename The name of the file to be opened.
     *
     * \param[in] binary   If true, the file will be opened as a binary file.  If false, the file will be opened as
     *                     a text file.
     *
     * \return Returns an integer value you can use to reference the file later.
     */
    M_PUBLIC_API Integer fileOpenWriteTruncate(const Tuple& filename, Boolean binary = false);

    /**
     * Function you can use to open a file for reading and writing.  This version will append to the file if it exists.
     *
     * \param[in] filename The name of the file to be opened.
     *
     * \param[in] binary   If true, the file will be opened as a binary file.  If false, the file will be opened as
     *                     a text file.
     *
     * \return Returns an integer value you can use to reference the file later.
     */
    M_PUBLIC_API Integer fileOpenWriteAppend(const Tuple& filename, Boolean binary = false);

    /**
     * Function you can use to determine if a file exists.
     *
     * \param[in] filename The name of the file to be tested.
     *
     * \return Returns true if the file exists.  Returns false if the file does not exist.
     */
    M_PUBLIC_API Boolean fileExists(const Tuple& filename);

    /**
     * Function you can use to close an already open file.
     *
     * \param[in] fileNumber The number of the file to be closed.
     *
     * \return Returns true on success, returns false if the file number is invalid or the file is already closed.
     */
    M_PUBLIC_API Boolean fileClose(Integer fileNumber);

    /**
     * Template function you can use to close an already open file.
     *
     * \param[in] fileNumber The number of the file to be closed.
     *
     * \return Returns true on success, returns false if the file number is invalid or the file is already closed.
     */
    template<typename FN> M_PUBLIC_TEMPLATE_FUNCTION Boolean fileClose(FN fileNumber) {
        Boolean result;

        Integer fn;
        if (toInteger(fn, fileNumber)) {
            result = fileClose(fn);
        } else {
            result = false;
        }

        return result;
    }

    /**
     * Function you can use to delete a file.  If the file is marked as open, then the file will be closed before being
     * deleted.
     *
     * \param[in] filename The name of the file to be deleted.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API Boolean fileDelete(const Tuple& filename);

    /**
     * Function you can use to delete a file.  If the file is marked as open, then the file will be closed before being
     * deleted.
     *
     * \param[in] fileNumber The number of the file to be deleted.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API Boolean fileDelete(Integer fileNumber);

    /**
     * Function you can use to delete a file.  If the file is marked as open, then the file will be closed before being
     * deleted.
     *
     * \param[in] fileNumber The number of the file to be deleted.
     *
     * \return Returns true on success, returns false on error.
     */
    template<typename FN> M_PUBLIC_TEMPLATE_FUNCTION Boolean fileDelete(FN fileNumber) {
        Integer fn;
        return toInteger(fn, fileNumber) ? fileDelete(fn) : false;
    }

    /**
     * Function you can use to seek to a specific location in an open file.
     *
     * \param[in] fileNumber The number of the file in question.
     *
     * \param[in] offset     The zero based offset to seek to.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API Boolean fileSeek(Integer fileNumber, Integer offset);

    /**
     * Template function you can use to seek to a specific location in an open file.
     *
     * \param[in] fileNumber The number of the file in question.
     *
     * \param[in] offset     The zero based offset to seek to.
     *
     * \return Returns true on success, returns false on error.
     */
    template<typename FN, typename O> M_PUBLIC_TEMPLATE_FUNCTION Boolean fileSeek(FN fileNumber, O offset) {
        Boolean result;

        Integer fn;
        Integer o;
        if (toInteger(fn, fileNumber) && toInteger(o, offset)) {
            result = fileSeek(fn, o);
        } else {
            result = false;
        }

        return result;
    }

    /**
     * Function you can use to read an individual byte value.
     *
     * \param[in] fileNumber The number of the file to read from.
     *
     * \return Returns a tuple holding the read byte (as an integer value) and a boolean indicating success or
     *         failure.
     */
    M_PUBLIC_API Tuple fileReadByte(Integer fileNumber);

    /**
     * Template function you can use to read an individual byte value.
     *
     * \param[in] fileNumber The number of the file to read from.
     *
     * \return Returns a tuple holding the read byte (as an integer value) and a boolean indicating success or
     *         failure.
     */
    template<typename FN> M_PUBLIC_TEMPLATE_FUNCTION Tuple fileReadByte(FN fileNumber) {
        Integer fn;
        return toInteger(fn, fileNumber) ? fileReadByte(fn) : Tuple::build(Integer(0), Boolean(false));
    }

    /**
     * Function you can use to read an string.  This function behaves identically independently of whether the file was
     * opened as a text file or a binary file.
     *
     * \param[in] fileNumber The number of the file to read from.
     *
     * \param[in] length     The number of characters to be read.  A zero value indicates that the string should be
     *                       read until an end of line is reached.  Specifying a zero value will also cause the end of
     *                       line characters to be removed from the resulting string.  A negative value will cause the
     *                       entire file contents to be read, as is, into a single string buffer.
     *
     * \param[in] utf8       If true, the length will be based on the number of UTF-8 characters.  If false, the length
     *                       will be based on bytes read.
     *
     * \return Returns a tuple holding the string (as a tuple) and a boolean indicating success or failure.
     */
    M_PUBLIC_API Tuple fileReadString(Integer fileNumber, Integer length = 0, Boolean utf8 = true);

    /**
     * Template function you can use to read an string.  This function behaves identically independently of whether the
     * file was opened as a text file or a binary file.
     *
     * \param[in] fileNumber The number of the file to read from.
     *
     * \param[in] length     The number of characters to be read.  A zero value indicates that the string should be
     *                       read until an end of line is reached.  Specifying a zero value will also cause the end of
     *                       line characters to be removed from the resulting string.  A negative value will cause the
     *                       entire file contents to be read, as is, into a single string buffer.
     *
     * \param[in] utf8       If true, the length will be based on the number of UTF-8 characters.  If false, the length
     *                       will be based on bytes read.
     *
     * \return Returns a tuple holding the string (as a tuple) and a boolean indicating success or failure.
     */
    template<typename FN, typename L = Integer> M_PUBLIC_TEMPLATE_FUNCTION Tuple fileReadString(
            FN      fileNumber,
            L       length = L(0),
            Boolean utf8 = true
        ) {
        Integer fn;
        Integer l;

        return   toInteger(fn, fileNumber) && toInteger(l, length)
               ? fileReadString(fn, l, utf8)
               : Tuple::build(Tuple(), Boolean(false));
    }

    /**
     * Function you can use to read an individual integer value.
     *
     * \param[in] fileNumber  The number of the file to read from.
     *
     * \param[in] bytesToRead The number of bytes used to represent the integer.  A positive value will cause the
     *                        function to read the specified number of bytes, treating the value as a little-endian
     *                        integer value.  A negative value will cause the function to read the specified number of
     *                        bytes, treating the value as a big-endian value.  A value of zero will cause the function
     *                        to read the incoming stream as text, stopping at the first non-digit character (the non
     *                        digit character is not consumed).  You can prefix text integer values with a sign or with
     *                        '0x' or '0b'.
     *
     * \return Returns a tuple holding the integer value and a boolean indicating success or failure.
     */
    M_PUBLIC_API Tuple fileReadInteger(Integer fileNumber, Integer bytesToRead = 0);

    /**
     * Template function you can use to read an individual integer value.
     *
     * \param[in] fileNumber  The number of the file to read from.
     *
     * \param[in] bytesToRead The number of bytes to be read.
     *
     * \return Returns a tuple holding the integer value and a boolean indicating success or failure.
     */
    template<typename FN, typename BTR = Integer> M_PUBLIC_TEMPLATE_FUNCTION Tuple fileReadInteger(
            FN  fileNumber,
            BTR bytesToRead = BTR(0)
        ) {
        Integer fn;
        Integer btr;
        return   toInteger(fn, fileNumber) && toInteger(btr, bytesToRead)
               ? fileReadInteger(fn, btr)
               : Tuple::build(Integer(0), Boolean(false));
    }

    /**
     * Function you can use to read an individual real value.
     *
     * \param[in] fileNumber The number of the file to read from.
     *
     * \param[in] bytesToRead The number of bytes used to represent the real value.  A positive value of 4 will
     *                        indicate single precision little-endian floating point format.  A positive value of
     *                        8 will indicate double precision little-endian floating point format.  A value of -4
     *                        will indicate single precision big-endian floating point format.  A value of -8 will
     *                        indicate double precision big-endian floating point format.  A value of 0 indicates
     *                        traditional ASCII text format.  All other values will trigger an exception.
     *
     * \return Returns a tuple holding the real value and a boolean indicating success or failure.
     */
    M_PUBLIC_API Tuple fileReadReal(Integer fileNumber, Integer bytesToRead = 0);

    /**
     * Function you can use to read an individual real value.
     *
     * \param[in] fileNumber  The number of the file to read from.
     *
     * \param[in] bytesToRead The number of bytes used to represent the real value.  A positive value of 4 will
     *                        indicate single precision little-endian floating point format.  A positive value of
     *                        8 will indicate double precision little-endian floating point format.  A value of -4
     *                        will indicate single precision big-endian floating point format.  A value of -8 will
     *                        indicate double precision big-endian floating point format.  A value of 0 indicates
     *                        traditional ASCII text format.  All other values will trigger an exception.
     *
     * \return Returns a tuple holding the real value and a boolean indicating success or failure.
     */
    template<typename FN, typename BTR = Integer> M_PUBLIC_TEMPLATE_FUNCTION Tuple fileReadReal(
            FN  fileNumber,
            BTR bytesToRead = BTR(0)
        ) {
        Integer fn;
        Integer btr;
        return   toInteger(fn, fileNumber) && toInteger(btr, bytesToRead)
               ? fileReadReal(fn, btr)
               : Tuple::build(Real(0), Boolean(false));
    }

    /**
     * Function you can use to write an individual byte value.
     *
     * \param[in] fileNumber The number of the file to write to.
     *
     * \param[in] byteValue  The unsigned byte value to be written.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API Boolean fileWriteByte(Integer fileNumber, Integer byteValue);

    /**
     * Template function you can use to write an individual byte value.
     *
     * \param[in] fileNumber The number of the file to write to.
     *
     * \param[in] byteValue  The unsigned byte value to be written.
     *
     * \return Returns true on success, returns false on error.
     */
    template<typename FN, typename BV> M_PUBLIC_TEMPLATE_FUNCTION Boolean fileWriteByte(FN fileNumber, BV byteValue) {
        Boolean result;

        Integer fn;
        Integer bv;
        if (toInteger(fn, fileNumber) && toInteger(bv, byteValue)) {
            result = fileWriteByte(fn, bv);
        } else {
            result = false;
        }

        return result;
    }

    /**
     * Function you can use to write an string.
     *
     * \param[in] fileNumber     The number of the file to write to.
     *
     * \param[in] string         The string to be written (as a tuple).
     *
     * \param[in] includeNewline If true, the written data should include an ending newline.  If false, no new-line
     *                           character should be included.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API Boolean fileWriteString(Integer fileNumber, const Tuple& string, Boolean includeNewline = false);

    /**
     * Function you can use to write an string.
     *
     * \param[in] fileNumber     The number of the file to write to.
     *
     * \param[in] string         The string to be written (as a tuple).
     *
     * \param[in] includeNewline If true, the written data should include an ending newline.  If false, no new-line
     *                           character should be included.
     *
     * \return Returns true on success, returns false on error.
     */
    template<typename FN> M_PUBLIC_TEMPLATE_FUNCTION Boolean fileWriteString(
            FN           fileNumber,
            const Tuple& string,
            Boolean      includeNewline = false
        ) {
        Boolean result;

        Integer fn;
        if (toInteger(fn, fileNumber)) {
            result = fileWriteString(fn, string, includeNewline);
        } else {
            result = false;
        }

        return result;
    }

    /**
     * Function you can use to write an individual integer value.
     *
     * \param[in] fileNumber   The number of the file to write to.
     *
     * \param[in] integerValue The integer value to be written.
     *
     * \param[in] bytesToWrite The number of bytes used to represent the integer.  A positive value will cause the
     *                         function to write the specified number of bytes, treating the value as a little-endian
     *                         integer value.  A negative value will cause the function to write the specified number
     *                         of bytes, treating the value as a big-endian value.  A value of zero will cause the
     *                         function to write the stream as text.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API Boolean fileWriteInteger(Integer fileNumber, Integer integerValue, Integer bytesToWrite = 0);

    /**
     * Template function you can use to write an individual integer value.
     *
     * \param[in] fileNumber   The number of the file to write to.
     *
     * \param[in] integerValue The integer value to be written.
     *
     * \param[in] bytesToWrite The number of bytes used to represent the integer.  A positive value will cause the
     *                         function to write the specified number of bytes, treating the value as a little-endian
     *                         integer value.  A negative value will cause the function to write the specified number
     *                         of bytes, treating the value as a big-endian value.  A value of zero will cause the
     *                         function to write the stream as text.
     *
     * \return Returns true on success, returns false on error.
     */
    template<typename FN, typename IV, typename BTW = Integer> M_PUBLIC_TEMPLATE_FUNCTION Boolean fileWriteInteger(
            FN  fileNumber,
            IV  integerValue,
            BTW bytesToWrite = BTW(0)
        ) {
        Boolean result;

        Integer fn;
        Integer iv;
        Integer btw;
        if (toInteger(fn, fileNumber) && toInteger(iv, integerValue) && toInteger(btw, bytesToWrite)) {
            result = fileWriteInteger(fn, iv, btw);
        } else {
            result = false;
        }

        return result;
    }

    /**
     * Function you can use to write an individual real value.
     *
     * \param[in] fileNumber   The number of the file to write to.
     *
     * \param[in] realValue    The real value to be written.
     *
     * \param[in] bytesToWrite The number of bytes used to represent the real value.  A positive value of 4 will
     *                         indicate single precision little-endian floating point format.  A positive value of
     *                         8 will indicate double precision little-endian floating point format.  A value of -4
     *                         will indicate single precision big-endian floating point format.  A value of -8 will
     *                         indicate double precision big-endian floating point format.  A value of 0 indicates
     *                         traditional ASCII text format.  All other values will trigger an exception.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API Boolean fileWriteReal(Integer fileNumber, Real realValue, Integer bytesToWrite = 0);

    /**
     * Function you can use to write an individual real value.
     *
     * \param[in] fileNumber   The number of the file to write to.
     *
     * \param[in] realValue    The real value to be written.
     *
     * \param[in] bytesToWrite The number of bytes to be written.
     *
     * \return Returns true on success, returns false on error.
     */
    template<typename FN, typename RV, typename BTW = Integer> M_PUBLIC_TEMPLATE_FUNCTION Boolean fileWriteReal(
            FN  fileNumber,
            RV  realValue,
            BTW bytesToWrite = BTW(0)
        ) {
        Boolean result;

        Integer fn;
        Real    rv;
        Integer btw;

        if (toInteger(fn, fileNumber) && toReal(rv, realValue) && toInteger(btw, bytesToWrite)) {
            result = fileWriteReal(fn, rv, btw);
        } else {
            result = false;
        }

        return result;
    }

    /**
     * Function you can use to load a boolean matrix from a file.  This method will automatically determine if the file
     * is in CSV format or binary format.
     *
     * \param[in] filename The name of the file holding the boolean matrix.
     *
     * \return Returns a boolean matrix.  This method will throw an exception or returns an empty matrix if an error
     *         is detected.
     */
    M_PUBLIC_API MatrixBoolean loadBooleanMatrix(const Tuple& filename);

    /**
     * Function you can use to save a boolean matrix to a file.
     *
     * \param[in] matrix   The matrix to be stored.
     *
     * \param[in] filename The name of the file holding the boolean matrix.
     *
     * \param[in] binary   If true, the matrix will be stored in binary format.  If false, the matrix will be stored
     *                     as a CSV file.
     *
     * \return Returns true on success.  Returns false on error.
     */
    M_PUBLIC_API Boolean saveBooleanMatrix(const MatrixBoolean& matrix, const Tuple& filename, Boolean binary = false);

    /**
     * Function you can use to load  an integer matrix from a file.  This method will automatically determine if the file
     * is in CSV format or binary format.
     *
     * \param[in] filename The name of the file holding the integer matrix.
     *
     * \return Returns  an integer matrix.  This method will throw an exception or returns an empty matrix if an error
     *         is detected.
     */
    M_PUBLIC_API MatrixInteger loadIntegerMatrix(const Tuple& filename);

    /**
     * Function you can use to save  an integer matrix to a file.
     *
     * \param[in] matrix   The matrix to be stored.
     *
     * \param[in] filename The name of the file holding the integer matrix.
     *
     * \param[in] binary   If true, the matrix will be stored in binary format.  If false, the matrix will be stored
     *                     as a CSV file.
     *
     * \return Returns true on success.  REturns false on error.
     */
    M_PUBLIC_API Boolean saveIntegerMatrix(const MatrixInteger& matrix, const Tuple& filename, Boolean binary = false);

    /**
     * Function you can use to load a real matrix from a file.  This method will automatically determine if the file
     * is in CSV format or binary format.
     *
     * \param[in] filename The name of the file holding the real matrix.
     *
     * \return Returns a real matrix.  This method will throw an exception or returns an empty matrix if an error
     *         is detected.
     */
    M_PUBLIC_API MatrixReal loadRealMatrix(const Tuple& filename);

    /**
     * Function you can use to save a real matrix to a file.
     *
     * \param[in] matrix   The matrix to be stored.
     *
     * \param[in] filename The name of the file holding the real matrix.
     *
     * \param[in] binary   If true, the matrix will be stored in binary format.  If false, the matrix will be stored
     *                     as a CSV file.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API Boolean saveRealMatrix(const MatrixReal& matrix, const Tuple& filename, Boolean binary = false);

    /**
     * Function you can use to load a complex matrix from a file.  This method will automatically determine if the file
     * is in CSV format or binary format.
     *
     * \param[in] filename The name of the file holding the complex matrix.
     *
     * \return Returns a complex matrix.  This method will throw an exception or returns an empty matrix if an error
     *         is detected.
     */
    M_PUBLIC_API MatrixComplex loadComplexMatrix(const Tuple& filename);

    /**
     * Function you can use to save a complex matrix to a file.
     *
     * \param[in] matrix   The matrix to be stored.
     *
     * \param[in] filename The name of the file holding the complex matrix.
     *
     * \param[in] binary   If true, the matrix will be stored in binary format.  If false, the matrix will be stored
     *                     as a CSV file.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API Boolean saveComplexMatrix(const MatrixComplex& matrix, const Tuple& filename, Boolean binary = false);
}

#endif
