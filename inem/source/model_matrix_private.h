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
* This header defines the private implementation of the \ref Model::MatrixPrivate class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_MATRIX_PRIVATE_H
#define MODEL_MATRIX_PRIVATE_H

#include <cstdint>

#include "model_api_types.h"
#include "model_intrinsic_types.h"
#include "model_matrix.h"
#include "m_compiler_abstraction.h"
#include "m_reference_counter.h"

namespace Model {
    /**
     * Private implementation base class for all Matrix private classes.
     */
    class MatrixPrivate:public M::ReferenceCounter {
        public:
            virtual ~MatrixPrivate() = default;

            /**
             * Method you can use to determine the array type.
             *
             * \return Returns the array type being represented by this instance.
             */
            virtual Matrix::MatrixType matrixType() const = 0;

            /**
             * Method you can use to determine the type of coefficient stored in this matrix.
             *
             * \return Returns the coefficient type stored in the matrix.
             */
            virtual ValueType coefficientType() const = 0;

            /**
             * Method you can use to determine number of matrix rows.
             *
             * \return Returns the current number of rows.
             */
            virtual unsigned long numberRows() const = 0;

            /**
             * Method you can use to determine the number of matrix columns.
             *
             * \return Returns the current number of matrix columns.
             */
            virtual unsigned long numberColumns() const = 0;
    };
}

#endif
