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
* This header defines the private implementation of the \ref M::MatrixPrivateBase class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_MATRIX_PRIVATE_BASE_H
#define M_MATRIX_PRIVATE_BASE_H

#include <cstdint>

#include "m_api_types.h"
#include "m_intrinsic_types.h"
#include "m_compiler_abstraction.h"
#include "model_matrix.h"

extern "C" typedef struct _MatApi MatApi;

namespace M {
    /**
     * Private base class used by various matrix storage classes.  The class provides a handful of methods used to
     * assist in memory management.
     */
    class MatrixPrivateBase {
        public:
            /**
             * Method you can use to specify the matrix library to be used.  You should call this before any other
             * method or class.
             *
             * Note that this settings is global.
             *
             * \param[in] matrixApi The matrix API library to be used.
             */
            static void setMatrixApi(const MatApi* matrixApi);

        protected:
            /**
             * Method you can use to obtain the matrix library API.
             *
             * \return Returns the matrix library API.
             */
            static inline const MatApi* matrixApi() {
                return currentMatrixApi;
            }

            /**
             * Method you should call to allocate memory for this structure and it's associated data.
             *
             * \param[out] rawDataBaseAddress        The base address for the raw data.
             *
             * \param[in]  requiredAllocationInBytes The allocated space for the new raw data memory, in bytes.
             *
             * \param[in]  classInstanceSize         The size of the class instance, in bytes.  The allocation engine
             *                                       will assume that the classAdditional overhead to be applied.
             */
            static void* allocateMemory(
                void*&             rawDataBaseAddress,
                unsigned long long requiredAllocationInBytes,
                unsigned           classInstanceSize
            );

            /**
             * Method you should call to deallocate memory previously allocated with
             * \ref M::MatrixPrivateBase::allocateMemory.
             *
             * \param[in] ptr The pointer to the location to be free'd.
             */
            static void releaseMemory(void* ptr);

        private:
            /**
             * The current matrix API library.
             */
            static const MatApi* currentMatrixApi;

            /**
             * Method that is called to perform memory allocation.  You should use this method for matrix structures
             * rather than C++ new/delete or stdio malloc/free so that guarantees can be made regarding memory
             * alignment.
             *
             * This memory will throw an exception if memory could not be allocated for the array.
             *
             * \param[in] space The total required space to be malloc'd.
             *
             * \return Returns a pointer to the requested memory that is properly aligned.
             */
            static void* malloc(unsigned long long space);

            /**
             * Method you should call to deallocate previously allocated memory.
             *
             * \param[in] p The pointer to the memory region to be deallocated.
             */
            static void free(void* p);
    };
}

#endif
