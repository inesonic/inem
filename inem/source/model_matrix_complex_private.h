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
* This header defines the private implementation base class of the \ref Model::MatrixComplex class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_MATRIX_COMPLEX_PRIVATE_H
#define MODEL_MATRIX_COMPLEX_PRIVATE_H

#include "model_matrix_complex.h"
#include "m_matrix_complex.h"

namespace M {
    class MatrixBoolean;
    class MatrixInteger;
    class MatrixReal;
}

namespace Model {
    /**
     * Private implementation base class of the \ref Model::MatrixComplex class.
     */
    class MatrixComplex::Private:public M::MatrixComplex {
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
            ):M::MatrixComplex(
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
                const MatrixComplex::Scalar* matrixData
            ):M::MatrixComplex(
                newNumberRows,
                newNumberColumns,
                matrixData
            ) {}

            /**
             * Constructor
             *
             * \param[in] other The instance to be copied to this instance.
             */
            inline Private(const M::MatrixBoolean& other):M::MatrixComplex(other) {}

            /**
             * Constructor
             *
             * \param[in] other The instance to be copied to this instance.
             */
            inline Private(const M::MatrixInteger& other):M::MatrixComplex(other) {}

            /**
             * Constructor
             *
             * \param[in] other The instance to be copied to this instance.
             */
            inline Private(const M::MatrixReal& other):M::MatrixComplex(other) {}

            /**
             * Constructor
             *
             * \param[in] other The instance to be copied to this instance.
             */
            inline Private(const M::MatrixComplex& other):M::MatrixComplex(other) {}

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            inline Private(const Private& other):M::MatrixComplex(other) {}

            ~Private() override {}

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns an instance to this object.
             */
            inline Private operator=(const Private& other) {
                M::MatrixComplex::operator=(other);
                return *this;
            }
    };
}

#endif
