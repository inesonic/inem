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
* This header defines the private implementation data store for complex sparse matrices.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_MATRIX_COMPLEX_SPARSE_DATA_H
#define MODEL_MATRIX_COMPLEX_SPARSE_DATA_H

#include "model_api_types.h"
#include "model_intrinsic_types.h"
#include "model_complex.h"
#include "m_matrix_sparse_private.h"
#include "m_matrix_dense_private.h"
#include "m_matrix_complex_data.h"
#include "m_matrix_complex.h"

namespace M {
    /**
     * Private implementation class for \ref M::MatrixComplex.  This class provides the data store and implementation
     * for complex matricies using a sparse data format.
     */
    class MatrixComplex::SparseData:public MatrixComplex::Data, public MatrixSparsePrivate<Complex> {
        friend class MatrixSparsePrivate<Complex>;

        protected:
            /**
             * Constructor
             *
             * \param[in] newNumberRows        The new number of rows.
             *
             * \param[in] newNumberColumns     The new number of columns.
             *
             * \param[in] newAllocationInBytes The size of the allocated data structure, in bytes.
             *
             * \param[in] rawDataBaseAddress   The base address of the raw data stored in the matrix.
             */
            SparseData(
                unsigned long      newNumberRows,
                unsigned long      newNumberColumns,
                unsigned long long newAllocationInBytes,
                void*              rawDataBaseAddress
            );

            /**
             * Constructor
             *
             * \param[in] other                The instance to be copied to this instance.
             *
             * \param[in] newAllocationInBytes The size of the allocated data structure, in bytes.
             *
             * \param[in] rawDataBaseAddress   The base address of the raw data stored in the matrix.
             */
            SparseData(
                const MatrixDensePrivate<Complex>& other,
                unsigned long long                 newAllocationInBytes,
                void*                              rawDataBaseAddress
            );

            /**
             * Constructor
             *
             * \param[in] other                The instance to be copied to this instance.
             *
             * \param[in] newNumberRows        The new number of rows.
             *
             * \param[in] newNumberColumns     The new number of columns.
             *
             * \param[in] newAllocationInBytes The size of the allocated data structure, entries.
             *
             * \param[in] rawDataBaseAddress   The base address of the raw data stored in the matrix.
             */
            SparseData(
                const MatrixDensePrivate<Complex>& other,
                unsigned long                      newNumberRows,
                unsigned long                      newNumberColumns,
                unsigned long long                 newAllocationInBytes,
                void*                              rawDataBaseAddress
            );

            ~SparseData() override;

        public:
            /**
             * Method you can call to allocate an instance of this class.
             *
             * \param[in] newNumberRows    The new number of rows.
             *
             * \param[in] newNumberColumns The new number of columns.
             */
            static inline SparseData* create(
                    unsigned long newNumberRows,
                    unsigned long newNumberColumns
                ) {
                (void) newNumberRows;    // FIXME
                (void) newNumberColumns; // FIXME

                return nullptr; // FIXME
            }

            /**
             * Method you can call to destroy an instance of this class.
             *
             * \param[in] instance The instance to be destroyed.
             */
            static inline void destroy(SparseData* instance) {
                (void) instance; // FIXME
            }

            /**
             * Method you can use to clone this matrix.
             *
             * \return Returns a newly created instance of this matrix.
             */
            MatrixComplex::Data* clone() const override {
                return nullptr; // FIXME
            }

            /**
             * Method you can use to determine the array type.
             *
             * \return Returns the array type being represented by this instance.
             */
            Matrix::MatrixType matrixType() const override {
                return Matrix::MatrixType::SPARSE;
            }

            /**
             * Method you can use to determine the type of coefficient stored in this matrix.
             *
             * \return Returns the coefficient type stored in the matrix.
             */
            ValueType coefficientType() const override {
                return ValueType::COMPLEX;
            }
    };
}

#endif
