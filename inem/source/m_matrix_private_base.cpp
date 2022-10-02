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
* This file implements the \ref Model::MatrixPrivateBase class.
***********************************************************************************************************************/

#include <new>
#include <cstring>
#include <cassert>
#include <cstdint>
#include <cassert>
#include <algorithm>

#include <mat_api.h>

#include "m_api_types.h"
#include "m_intrinsic_types.h"
#include "model_exceptions.h"
#include "m_matrix_private_base.h"

extern "C" typedef struct _MatApi MatApi;

namespace M {
    const MatApi* MatrixPrivateBase::currentMatrixApi = nullptr;

    void MatrixPrivateBase::setMatrixApi(const MatApi* matrixApi) {
        currentMatrixApi = matrixApi;
    }


    void* MatrixPrivateBase::allocateMemory(
            void*&             rawDataBaseAddress,
            unsigned long long newAllocationInBytes,
            unsigned           classInstanceSize
        ) {
        void*    basePointer;
        unsigned memoryAlignmentBytes = static_cast<unsigned>(currentMatrixApi->memoryAlignmentRequirementBytes);
        if (memoryAlignmentBytes > 1) {
            unsigned paddedClassSize;
            unsigned classInstanceResidue = classInstanceSize % memoryAlignmentBytes;
            if (classInstanceResidue > 0) {
                paddedClassSize = classInstanceSize + memoryAlignmentBytes - classInstanceResidue;
            } else {
                paddedClassSize = classInstanceSize;
            }

            unsigned long long totalBytesRequired = newAllocationInBytes + paddedClassSize;

            basePointer        = malloc(totalBytesRequired);
            rawDataBaseAddress = reinterpret_cast<std::uint8_t*>(basePointer) + paddedClassSize;
        } else {
            basePointer = rawDataBaseAddress = malloc(newAllocationInBytes);
        }

        return basePointer;
    }


    void MatrixPrivateBase::releaseMemory(void* ptr) {
        free(ptr);
    }


    void* MatrixPrivateBase::malloc(unsigned long long space) {
        void* p = (*currentMatrixApi->allocateMemory)(space, currentMatrixApi->memoryAlignmentRequirementBytes);

        if (p == nullptr) {
            throw Model::InsufficientMemory();
        }

        return p;
    }


    void MatrixPrivateBase::free(void* p) {
        (*currentMatrixApi->releaseMemory)(p);
    }
}
