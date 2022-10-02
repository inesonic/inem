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
* This header defines the \ref Model::Matrix class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_MATRIX_H
#define MODEL_MATRIX_H

#include "model_common.h"
#include "model_api_types.h"
#include "model_intrinsic_types.h"
#include "model_complex.h"
#include "model_range.h"
#include "model_variant.h"

namespace Model {
    class MatrixBoolean;
    class MatrixInteger;
    class MatrixReal;
    class MatrixComplex;

    /**
     * Pure virtual base class for all matrix types.  This class provides a common API you can use to access values
     * from any matrix class in a consistent manner.
     *
     * Note that you should always instantiate and use one of \ref Model::MatrixBoolean, \ref Model::MatrixInteger,
     * \ref Model::MatrixReal, or \ref Model::MatrixComplex.
     */
    class MODEL_PUBLIC_API Matrix {
        public:
            /**
             * Enumeration of available matrix types.
             */
            enum class MatrixType {
                /**
                 * Indicates a dense array in column-major format.
                 */
                DENSE,

                /**
                 * Indicates a sparse array.  Note that, at this time, there is no support for sparse matrices.
                 */
                SPARSE
            };

            /**
             * The type used as the index into the matrix.
             */
            typedef Integer Index;

            virtual ~Matrix() = default;

            /**
             * Pure virtual method you can use to determine the underlying coefficient type.
             *
             * \return Returns the value type of the underlying coefficient.
             */
            virtual ValueType coefficientValueType() const = 0;

            /**
             * Pure virtual method you can use to determine number of matrix rows.
             *
             * \return Returns the current number of rows.
             */
            virtual Index numberRows() const = 0;

            /**
             * Pure virtual method you can use to determine the number of matrix columns.
             *
             * \return Returns the current number of matrix columns.
             */
            virtual Index numberColumns() const = 0;

            /**
             * Pure virtual method you can use to determine the current total number of matrix coefficients.
             *
             * \return Returns the size of the matrix, in coefficients.
             */
            virtual Index numberCoefficients() const = 0;

            /**
             * Pure virtual method you can use to determine the underlying memory layout for the matrix.
             *
             * \return Returns a value indicating the underlying memory layout for the matrix.
             */
            virtual MatrixType matrixType() const = 0;

            /**
             * Pure virtual method you can use to access the value at a specified row/column address.  This method will
             * throw an exception if the specified row or column is invalid.
             *
             * \param[in] row    The one based row address.
             *
             * \param[in] column The one based column address.
             *
             * \return Returns a variant value holding the contents at the specified location.
             */
            virtual Variant value(Index row, Index column) const = 0;

            /**
             * Pure virtual method you can use to access the value at a specified one based index.  This method will
             * throw an exception if the specified index is invalid.
             *
             * \param[in] index The one based index of the coefficient to be accessed.
             *
             * \return Returns a variant value holding the contents at the specified location.
             */
            virtual Variant value(Index index) const = 0;

            /**
             * Pure virtual method you can use to set a value by position.  If needed, the matrix will be resized to
             * contain the provided coefficient.
             *
             * Note that resizing the matrix is a potentially time-consuming operation.
             *
             * \param[in] row    The one based row address.
             *
             * \param[in] column The one based column address.
             *
             * \param[in] value  The value to assign to the specified location.
             *
             * \return Returns true on success, returns false if the variant is not compatible with the matrix type or
             *         the row/column address is invalid.
             */
            virtual bool setValue(Index row, Index column, const Variant& value) = 0;

            /**
             * Pure virtual method you can use to set a value by position.
             *
             * \param[in] index The one base index of the coefficient to be accessed.
             *
             * \param[in] value The value to assign to the specified location.
             *
             * \return Returns true on success, returns false if the variant is not compatible with the matrix type or
             *         the index is invalid.
             */
            virtual bool setValue(Index index, const Variant& value) = 0;

            /**
             * Pure virtual method you can use to resize the matrix.  Newly inserted values will be initialized to the
             * default value for the type and existing values will be preserved.
             *
             * Note that this is a potentially time-consuming operation.
             *
             * \param[in] newNumberRows    The new number of matrix rows.
             *
             * \param[in] newNumberColumns The new number of matrix columns.
             */
            virtual void resize(Index newNumberRows, Index newNumberColumns) = 0;
    };
}

#endif
