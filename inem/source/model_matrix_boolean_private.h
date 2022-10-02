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
* This header defines the private implementation base class of the \ref Model::MatrixBoolean class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_MATRIX_BOOLEAN_PRIVATE_H
#define MODEL_MATRIX_BOOLEAN_PRIVATE_H

#include "model_matrix_boolean.h"
#include "m_matrix_boolean.h"

namespace Model {
    /**
     * Private implementation base class of the \ref Model::MatrixBoolean class.
     */
    class MatrixBoolean::Private:public M::MatrixBoolean {
        public:
            /**
             * Constructor
             *
             * \param[in] newNumberRows    The desired number of rows.
             *
             * \param[in] newNumberColumns The desired number of columns.
             */
            inline Private(
                unsigned long newNumberRows = 0,
                unsigned long newNumberColumns = 0
            ):M::MatrixBoolean(
                newNumberRows,
                newNumberColumns
            ) {}

            /**
             * Constructor
             *
             * \param[in] newNumberRows    The desired number of rows.
             *
             * \param[in] newNumberColumns The desired number of columns.
             *
             * \param[in] matrixData       The matrix data to use to initialize this matrix.
             */
            inline Private(
                unsigned long                newNumberRows,
                unsigned long                newNumberColumns,
                const MatrixBoolean::Scalar* matrixData
            ):M::MatrixBoolean(
                newNumberRows,
                newNumberColumns,
                matrixData
            ) {}

            /**
             * Constructor
             *
             * \param[in] other The instance to be copied to this instance.
             */
            inline Private(const M::MatrixBoolean& other):M::MatrixBoolean(other) {}

            /**
             * Constructor
             *
             * \param[in] other The instance to be copied to this instance.
             */
            inline Private(const M::MatrixInteger& other):M::MatrixBoolean(other) {}

            /**
             * Constructor
             *
             * \param[in] other The instance to be copied to this instance.
             */
            inline Private(const M::MatrixReal& other):M::MatrixBoolean(other) {}

            /**
             * Constructor
             *
             * \param[in] other The instance to be copied to this instance.
             */
            inline Private(const M::MatrixComplex& other):M::MatrixBoolean(other) {}

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            inline Private(const Private& other):M::MatrixBoolean(other) {}

            ~Private() override {}

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns an instance to this object.
             */
            inline Private operator=(const Private& other) {
                M::MatrixBoolean::operator=(other);
                return *this;
            }
    };
}

#endif
