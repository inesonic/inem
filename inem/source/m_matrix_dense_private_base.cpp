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
* This file implements the \ref Model::MatrixDensePrivateBase class.
***********************************************************************************************************************/

#include <new>
#include <cstring>
#include <cassert>
#include <cstdint>
#include <cassert>
#include <algorithm>

#include "m_api_types.h"
#include "m_intrinsic_types.h"
#include "model_exceptions.h"
#include "m_matrix_dense_private_base.h"

namespace M {
    MatrixDensePrivateBase::MatrixDensePrivateBase(
            unsigned long      newNumberRows,
            unsigned long      newNumberColumns,
            unsigned long long newAllocationInBytes,
            void*              rawDataBaseAddress,
            unsigned           coefficientSizeInBytes
        ):rawData(
            rawDataBaseAddress
        ),currentAllocatedSizeInBytes(
            newAllocationInBytes
        ),currentColumnSizeInBytes(
            columnSizeInBytes(newNumberRows, coefficientSizeInBytes)
        ),currentNumberRows(
            newNumberRows
        ),currentNumberColumns(
            newNumberColumns
        ),currentCoefficientSizeInBytes(
            coefficientSizeInBytes
        ) {
        assert(
               memoryColumnAlignmentRequirementBytes == 1
            || coefficientSizeInBytes == memoryColumnAlignmentRequirementBytes
            || (   coefficientSizeInBytes > memoryColumnAlignmentRequirementBytes
                && (currentCoefficientSizeInBytes % memoryColumnAlignmentRequirementBytes == 0)
               )
            || (    coefficientSizeInBytes < memoryColumnAlignmentRequirementBytes
                 && (memoryColumnAlignmentRequirementBytes % coefficientSizeInBytes == 0)
               )
        );
    }


    MatrixDensePrivateBase::MatrixDensePrivateBase(
            const MatrixDensePrivateBase& other,
            unsigned long long            newAllocationInBytes,
            void*                         rawDataBaseAddress
        ):rawData(
            rawDataBaseAddress
        ),currentAllocatedSizeInBytes(
            newAllocationInBytes
        ),currentColumnSizeInBytes(
            other.currentColumnSizeInBytes
        ),currentNumberRows(
            other.currentNumberRows
        ),currentNumberColumns(
            other.currentNumberColumns
        ),currentCoefficientSizeInBytes(
            other.currentCoefficientSizeInBytes
        ) {
        assert(
               memoryColumnAlignmentRequirementBytes == 1
            || currentCoefficientSizeInBytes == memoryColumnAlignmentRequirementBytes
            || (   currentCoefficientSizeInBytes > memoryColumnAlignmentRequirementBytes
                && (currentCoefficientSizeInBytes % memoryColumnAlignmentRequirementBytes == 0)
               )
            || (    currentCoefficientSizeInBytes < memoryColumnAlignmentRequirementBytes
                 && (memoryColumnAlignmentRequirementBytes % currentCoefficientSizeInBytes == 0)
               )
        );

        unsigned long long minimumRequiredMemoryAllocation = minimumRequiredAllocation(
            other.currentNumberRows,
            other.currentNumberColumns,
            other.currentCoefficientSizeInBytes
        );

        assert(newAllocationInBytes >= minimumRequiredMemoryAllocation);
        std::memcpy(rawData.byte, other.rawData.byte, minimumRequiredMemoryAllocation);
    }


    MatrixDensePrivateBase::MatrixDensePrivateBase(
            const MatrixDensePrivateBase& other,
            unsigned long                 newNumberRows,
            unsigned long                 newNumberColumns,
            unsigned long long            newAllocationInBytes,
            void*                         rawDataBaseAddress
        ):rawData(
            rawDataBaseAddress
        ),currentAllocatedSizeInBytes(
            newAllocationInBytes
        ),currentColumnSizeInBytes(
            columnSizeInBytes(newNumberRows, other.currentCoefficientSizeInBytes)
        ),currentNumberRows(
            newNumberRows
        ),currentNumberColumns(
            newNumberColumns
        ),currentCoefficientSizeInBytes(
            other.currentCoefficientSizeInBytes
        ) {
        assert(
               memoryColumnAlignmentRequirementBytes == 1
            || currentCoefficientSizeInBytes == memoryColumnAlignmentRequirementBytes
            || (   currentCoefficientSizeInBytes > memoryColumnAlignmentRequirementBytes
                && (currentCoefficientSizeInBytes % memoryColumnAlignmentRequirementBytes == 0)
               )
            || (    currentCoefficientSizeInBytes < memoryColumnAlignmentRequirementBytes
                 && (memoryColumnAlignmentRequirementBytes % currentCoefficientSizeInBytes == 0)
               )
        );

        assert(currentColumnSizeInBytes * newNumberColumns <= newAllocationInBytes);

        unsigned long       numberRelocatedColumns = std::min(other.currentNumberColumns, newNumberColumns);
        const std::uint8_t* source                 = other.rawData.byte;
        std::uint8_t*       destination            = rawData.byte;

        if (other.currentColumnSizeInBytes < currentColumnSizeInBytes) {
            unsigned long long bytesToZero = currentColumnSizeInBytes - other.currentColumnSizeInBytes;

            for (unsigned long columnIndex=0 ; columnIndex<numberRelocatedColumns ; ++columnIndex) {
                std::memcpy(destination, source, other.currentColumnSizeInBytes);
                std::memset(destination + other.currentColumnSizeInBytes, 0, bytesToZero);

                source      += other.currentColumnSizeInBytes;
                destination += currentColumnSizeInBytes;
            }
        } else {
            for (unsigned long columnIndex=0 ; columnIndex<numberRelocatedColumns ; ++columnIndex) {
                std::memcpy(destination, source, currentColumnSizeInBytes);

                source      += other.currentColumnSizeInBytes;
                destination += currentColumnSizeInBytes;
            }
        }

        std::uint8_t*      endOfStructure = rawData.byte + newAllocationInBytes;
        unsigned long long bytesToZero    = endOfStructure - destination;

        if (bytesToZero > 0) {
            std::memset(destination, 0, bytesToZero);
        }
    }


    MatrixDensePrivateBase::~MatrixDensePrivateBase() {}


    unsigned long long MatrixDensePrivateBase::columnSpacingInMemory() const {
        unsigned long long columnSize = columnSizeInBytes(currentNumberRows, currentCoefficientSizeInBytes);
        return columnSize / currentCoefficientSizeInBytes;
    }


    void MatrixDensePrivateBase::zeroMatrix() {
        unsigned long long matrixSizeInBytes = minimumRequiredAllocation(
            currentNumberRows,
            currentNumberColumns,
            currentCoefficientSizeInBytes
        );
        std::memset(rawData.byte, 0, matrixSizeInBytes);
    }


    bool MatrixDensePrivateBase::resizeInPlace(unsigned long newNumberRows, unsigned long newNumberColumns) {
        bool success;

        unsigned long long newColumnSizeInBytes = columnSizeInBytes(newNumberRows, currentCoefficientSizeInBytes);
        if (newNumberRows > 0 && newNumberColumns > 0) {
            unsigned long long minumumAllocatedSize = newColumnSizeInBytes * newNumberColumns;

            if (currentAllocatedSizeInBytes >= minumumAllocatedSize) {
                unsigned long numberRelocatedColumns = std::min(currentNumberColumns, newNumberColumns);

                if (numberRelocatedColumns > 0) {
                    if (newColumnSizeInBytes > currentColumnSizeInBytes) {
                        // columns are growing - work from the back forwards

                        unsigned long lastRelocatedColumn = numberRelocatedColumns - 1;
                        std::uint8_t* source      = rawData.byte + currentColumnSizeInBytes * lastRelocatedColumn;
                        std::uint8_t* destination = rawData.byte + newColumnSizeInBytes * lastRelocatedColumn;

                        unsigned long long bytesToClearEachColumn = newColumnSizeInBytes - currentColumnSizeInBytes;

                        while (source >= rawData.byte) {
                            if (source != destination) {
                                std::memcpy(destination, source, currentColumnSizeInBytes);
                            }

                            std::memset(destination + currentColumnSizeInBytes, 0, bytesToClearEachColumn);

                            source      -= currentColumnSizeInBytes;
                            destination -= newColumnSizeInBytes;
                        }
                    } else if (newColumnSizeInBytes < currentColumnSizeInBytes) {
                        // columns are shrinking - work from column 1 forwards

                        std::uint8_t* source      = rawData.byte + currentColumnSizeInBytes;
                        std::uint8_t* destination = rawData.byte + newColumnSizeInBytes;

                        unsigned long long usedColumnBytes       = newNumberRows * currentCoefficientSizeInBytes;
                        unsigned long long residueBytesPerColumn = newColumnSizeInBytes - usedColumnBytes;

                        if (residueBytesPerColumn > 0) {
                            std::memset(rawData.byte + usedColumnBytes, 0, residueBytesPerColumn);

                            for (unsigned long columnIndex=1 ; columnIndex<numberRelocatedColumns ; ++columnIndex) {
                                std::memcpy(destination, source, newColumnSizeInBytes);
                                std::memset(destination + usedColumnBytes, 0, residueBytesPerColumn);

                                source      += currentColumnSizeInBytes;
                                destination += newColumnSizeInBytes;
                            }
                        } else {
                            for (unsigned long columnIndex=1 ; columnIndex<numberRelocatedColumns ; ++columnIndex) {
                                std::memcpy(destination, source, newColumnSizeInBytes);
                                source      += currentColumnSizeInBytes;
                                destination += newColumnSizeInBytes;
                            }
                        }
                    } else {
                        assert(newColumnSizeInBytes == currentColumnSizeInBytes);

                        // column lengths are not changing but we may have a small reduction in rows that don't
                        // require us to move anything but may require that we clear out a few bytes in each column.

                        unsigned long long usedColumnBytes       = newNumberRows * currentCoefficientSizeInBytes;
                        unsigned long long residueBytesPerColumn = newColumnSizeInBytes - usedColumnBytes;

                        if (residueBytesPerColumn > 0) {
                            std::uint8_t* destination = rawData.byte + usedColumnBytes;
                            for (unsigned long columnIndex=0 ; columnIndex<numberRelocatedColumns ; ++columnIndex) {
                                std::memset(destination, 0, residueBytesPerColumn);
                                destination += newColumnSizeInBytes;
                            }
                        }
                    }
                }

                std::uint8_t* newTail = rawData.byte + newColumnSizeInBytes * numberRelocatedColumns;
                std::uint8_t* oldTail = rawData.byte + currentColumnSizeInBytes * currentNumberColumns;
                if (oldTail > newTail) {
                    std::memset(newTail, 0, oldTail - newTail);
                }

                currentColumnSizeInBytes = newColumnSizeInBytes;
                currentNumberRows        = newNumberRows;
                currentNumberColumns     = newNumberColumns;

                success = true;
            } else {
                success = false;
            }
        } else {
            zeroMatrix();

            currentColumnSizeInBytes = newColumnSizeInBytes;
            currentNumberRows        = newNumberRows;
            currentNumberColumns     = newNumberColumns;

            success = true;
        }

        return success;
    }


    bool MatrixDensePrivateBase::canResizeInPlace(unsigned long newNumberRows, unsigned long newNumberColumns) const {
        unsigned long long requiredAllocationInBytes = minimumRequiredAllocation(
            newNumberRows,
            newNumberColumns,
            currentCoefficientSizeInBytes
        );

        return (requiredAllocationInBytes <= currentAllocatedSizeInBytes);
    }


    int MatrixDensePrivateBase::relativeOrder(const MatrixDensePrivateBase& other) const {
        int result;

        if (currentNumberRows < other.currentNumberRows) {
            result = -1;
        } else if (currentNumberRows > other.currentNumberRows) {
            result = +1;
        } else /* if (currentNumberRows == other.currentNumberRows) */ {
            if (currentNumberColumns < other.currentNumberColumns) {
                result = -1;
            } else if (currentNumberColumns > other.currentNumberColumns) {
                result = +1;
            } else /* if (currentNumberColumns == other.currentNumberColumns) */ {
                assert(currentCoefficientSizeInBytes == other.currentCoefficientSizeInBytes);

                unsigned long      columnIndex           = 0;
                unsigned long long columnSizeInBytes     = currentCoefficientSizeInBytes * currentNumberRows;
                unsigned long long columnSpacingInMemory = MatrixDensePrivateBase::columnSpacingInMemory();

                result = 0;
                while (result == 0 && columnIndex < currentNumberColumns) {
                    unsigned long long columnByteOffset = columnSpacingInMemory * columnIndex;
                    result = memcmp(
                        rawData.byte + columnByteOffset,
                        other.rawData.byte + columnByteOffset,
                        columnSizeInBytes
                    );

                    ++columnIndex;
                }
            }
        }

        return result;
    }


    void MatrixDensePrivateBase::zeroMatrixMemory() {
        std::memset(rawData.byte, 0, currentAllocatedSizeInBytes);
    }


    void MatrixDensePrivateBase::toUpperTriangular() {
        if (currentNumberColumns > 1 && currentNumberRows > 1) {
            unsigned long      lastColumn     = std::min(currentNumberColumns - 1, currentNumberRows - 1);
            unsigned long long rowSizeInBytes = currentCoefficientSizeInBytes * currentNumberRows;
            for (unsigned columnIndex=0 ; columnIndex<lastColumn ; ++columnIndex) {
                std::uint8_t*      columnBase  = rawData.byte + currentColumnSizeInBytes * columnIndex;
                unsigned long long bytesToKeep = (columnIndex + 1) * currentCoefficientSizeInBytes;
                std::uint8_t*      zeroStart   = columnBase  + bytesToKeep;
                unsigned long long bytesToZero = rowSizeInBytes - bytesToKeep;

                std::memset(zeroStart, 0, bytesToZero);
            }
        }
    }


    void MatrixDensePrivateBase::toLowerTriangular() {
        if (currentNumberColumns > 1 && currentNumberRows > 1) {
            unsigned long lastColumn = std::min(currentNumberColumns - 1, currentNumberRows - 1);
            for (unsigned columnIndex=1 ; columnIndex<=lastColumn ; ++columnIndex) {
                std::uint8_t*      zeroStart   = rawData.byte + currentColumnSizeInBytes * columnIndex;
                unsigned long long bytesToZero = columnIndex * currentCoefficientSizeInBytes;

                std::memset(zeroStart, 0, bytesToZero);
            }
        }
    }


    unsigned long long MatrixDensePrivateBase::columnSizeInBytes(
            unsigned long numberRows,
            unsigned      coefficientSizeInBytes
        ) {
        unsigned long long columnSize = numberRows * coefficientSizeInBytes;

        if (numberRows > 1) {
            unsigned residue = columnSize % memoryColumnAlignmentRequirementBytes;
            if (residue != 0) {
                columnSize += memoryColumnAlignmentRequirementBytes - residue;
            }
        }

        return columnSize;
    }


    void MatrixDensePrivateBase::populate(const void* matrixData) {
        unsigned long long sourceColumnSize = currentNumberRows * currentCoefficientSizeInBytes;
        unsigned long long bytesToZero      = currentColumnSizeInBytes - sourceColumnSize;

        const std::uint8_t* source      = reinterpret_cast<const std::uint8_t*>(matrixData);
        std::uint8_t*       destination = rawData.byte;

        if (bytesToZero > 0) {
            for (unsigned long long columnIndex=0 ; columnIndex<currentNumberColumns ; ++columnIndex) {
                std::memcpy(destination, source, sourceColumnSize);
                std::memset(destination + sourceColumnSize, 0, bytesToZero);

                source      += sourceColumnSize;
                destination += currentColumnSizeInBytes;
            }
        } else {
            for (unsigned long long columnIndex=0 ; columnIndex<currentNumberColumns ; ++columnIndex) {
                std::memcpy(destination, source, sourceColumnSize);

                source      += sourceColumnSize;
                destination += currentColumnSizeInBytes;
            }
        }
    }


    unsigned long long MatrixDensePrivateBase::minimumRequiredAllocation(
            unsigned long newNumberRows,
            unsigned long newNumberColumns,
            unsigned      coefficientSizeInBytes
        ) {
        unsigned long long requiredColumnSizeInBytes = columnSizeInBytes(newNumberRows, coefficientSizeInBytes);
        return requiredColumnSizeInBytes * newNumberColumns;
    }


    unsigned long long MatrixDensePrivateBase::recommendedAllocation(
            unsigned long newNumberRows,
            unsigned long newNumberColumns,
            unsigned      coefficientSizeInBytes
        ) {
        unsigned long long requiredAllocation = minimumRequiredAllocation(
            newNumberRows,
            newNumberColumns,
            coefficientSizeInBytes
        );

        if (requiredAllocation <= 8 * 1024 * 1024) {
            requiredAllocation *= 2;
        } else if (requiredAllocation <= 64 * 1024 * 1024) {
            requiredAllocation *= 1.5;
        } else if (requiredAllocation <= 512 * 1024 * 1024) {
            requiredAllocation *= 1.25;
        } else {
            requiredAllocation *= 1.125;
        }

        if (requiredAllocation < memoryColumnAlignmentRequirementBytes) {
            requiredAllocation = memoryColumnAlignmentRequirementBytes;
        }

        return requiredAllocation;
    }
}
