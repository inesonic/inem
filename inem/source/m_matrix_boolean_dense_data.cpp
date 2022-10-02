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
* This file implements the \ref M::MatrixBoolean::DenseData class.
***********************************************************************************************************************/

#include <new>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <vector>
#include <string>

#include "m_api_types.h"
#include "m_intrinsic_types.h"
#include "m_compiler_abstraction.h"
#include "model_exceptions.h"
#include "m_exceptions.h"
#include "m_matrix_integer.h"
#include "m_matrix_integer_dense_data.h"
#include "m_matrix_real.h"
#include "m_matrix_real_dense_data.h"
#include "m_matrix_complex.h"
#include "m_matrix_complex_dense_data.h"
#include "m_matrix_boolean.h"
#include "m_matrix_boolean_data.h"
#include "m_matrix_boolean_dense_data.h"

namespace M {
    std::uint8_t MatrixBoolean::DenseData::byteReverseTable[256] = {
        0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
        0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
        0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
        0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
        0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
        0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
        0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
        0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
        0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
        0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
        0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
        0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
        0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
        0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
        0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
        0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
    };

    MatrixBoolean::DenseData::DenseData(
            unsigned long      newNumberRows,
            unsigned long      newNumberColumns,
            unsigned long long newColumnSizeInBits,
            unsigned long long newAllocationInStorageUnits,
            void*              rawDataBaseAddress
        ):currentNumberRows(
            newNumberRows
        ),currentNumberColumns(
            newNumberColumns
        ),columnSizeInBits(
            newColumnSizeInBits
        ),currentAllocation(
            newAllocationInStorageUnits
        ),rawData(
            reinterpret_cast<StorageUnit*>(rawDataBaseAddress)
        ) {}


    MatrixBoolean::DenseData::DenseData(
            const MatrixBoolean::DenseData& other,
            unsigned long long              newColumnSizeInBits,
            unsigned long long              newAllocationInStorageUnits,
            void*                           rawDataBaseAddress
        ):currentNumberRows(
            other.currentNumberRows
        ),currentNumberColumns(
            other.currentNumberColumns
        ),columnSizeInBits(
            newColumnSizeInBits
        ),currentAllocation(
            newAllocationInStorageUnits
        ),rawData(
            reinterpret_cast<StorageUnit*>(rawDataBaseAddress)
        ) {}


    MatrixBoolean::DenseData::DenseData(
            const MatrixBoolean::DenseData& other,
            unsigned long                   newNumberRows,
            unsigned long                   newNumberColumns,
            unsigned long long              newColumnSizeInBits,
            unsigned long long              newAllocationInStorageUnits,
            void*                           rawDataBaseAddress
        ):currentNumberRows(
            newNumberRows
        ),currentNumberColumns(
            newNumberColumns
        ),columnSizeInBits(
            newColumnSizeInBits
        ),currentAllocation(
            newAllocationInStorageUnits
        ),rawData(
            reinterpret_cast<StorageUnit*>(rawDataBaseAddress)
        ) {
        unsigned long long numberBytesToCopy = newAllocationInStorageUnits * sizeof(StorageUnit);
        std::memcpy(rawDataBaseAddress, other.rawData, numberBytesToCopy);
    }


    MatrixBoolean::DenseData::~DenseData() {}


    MatrixBoolean::DenseData* MatrixBoolean::DenseData::create(
            unsigned long newNumberRows,
            unsigned long newNumberColumns
        ) {
        DenseData* result = createUninitialized(newNumberRows, newNumberColumns);
        result->zeroMemory();

        return result;
    }


    MatrixBoolean::DenseData* MatrixBoolean::DenseData::create(
            unsigned long                newNumberRows,
            unsigned long                newNumberColumns,
            const MatrixBoolean::Scalar* matrixData
        ) {
        DenseData* result = createUninitialized(newNumberRows, newNumberColumns);

        unsigned long long columnSizeInBits = result->columnSizeInBits;
        StorageUnit        workingUnit      = 0;
        StorageUnit        mask             = 1;

        const Scalar* s = matrixData;
        StorageUnit*  d = result->rawData;

        unsigned long long remainingBits         = columnSizeInBits - newNumberRows;
        unsigned           remainingBitsThisUnit = static_cast<unsigned>(remainingBits % storageUnitSizeInBits);
        unsigned           remainingStorageUnits = static_cast<unsigned>(remainingBits / storageUnitSizeInBits);

        for (unsigned columnIndex=0 ; columnIndex<newNumberColumns ; ++columnIndex) {
            for (unsigned long rowIndex=0 ; rowIndex<newNumberRows ; ++rowIndex) {
                Scalar v = *s;
                if (v) {
                    workingUnit |= mask;
                }

                ++s;

                mask <<= 1;
                if (mask == 0) {
                    *d = workingUnit;
                    ++d;

                    workingUnit = 0;
                    mask        = 1;
                }
            }

            if (columnSizeInBits > newNumberRows) {
                mask <<= remainingBitsThisUnit;
                if (mask == 0) {
                    *d = workingUnit;
                    ++d;

                    workingUnit = 0;
                    mask        = 1;
                }

                if (remainingStorageUnits > 0) {
                    assert(workingUnit == 0 && mask == 1);
                    memset(d, 0, remainingStorageUnits * sizeof(StorageUnit));
                    d += remainingStorageUnits;
                }
            }
        }

        if (workingUnit != 0 || mask != 1) {
            *d = workingUnit;
            ++d;
        }

        if (remainingStorageUnits > 0) {
            memset(d, 0, remainingStorageUnits * sizeof(StorageUnit));
        }

        return result;
    }


    MatrixBoolean::DenseData* MatrixBoolean::DenseData::create(const MatrixBoolean::DenseData& other) {
        DenseData* result = createUninitialized(other.currentNumberRows, other.currentNumberColumns);

        assert(result->currentAllocation == other.currentAllocation);
        assert(result->columnSizeInBits == other.columnSizeInBits);

        unsigned long long bytesToCopy = other.currentAllocation * sizeof(StorageUnit);
        std::memcpy(result->rawData, other.rawData, bytesToCopy);

        return result;
    }


    MatrixBoolean::DenseData* MatrixBoolean::DenseData::create(const MatrixInteger::DenseData& other) {
        unsigned long numberRows    = other.numberRows();
        unsigned long numberColumns = other.numberColumns();

        DenseData* result = create(numberRows, numberColumns);

        for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
            for (unsigned long rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                MatrixInteger::DenseData::Scalar v = other.at(rowIndex, columnIndex);
                if (v != MatrixInteger::DenseData::Scalar(0)) {
                    result->update(rowIndex, columnIndex, true);
                }
            }
        }

        return result;
    }


    MatrixBoolean::DenseData* MatrixBoolean::DenseData::create(const MatrixReal::DenseData& other) {
        unsigned long numberRows    = other.numberRows();
        unsigned long numberColumns = other.numberColumns();

        DenseData* result = create(numberRows, numberColumns);

        for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
            for (unsigned long rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                MatrixReal::DenseData::Scalar v = other.at(rowIndex, columnIndex);
                if (v != MatrixReal::DenseData::Scalar(0)) {
                    result->update(rowIndex, columnIndex, true);
                }
            }
        }

        return result;
    }


    MatrixBoolean::DenseData* MatrixBoolean::DenseData::create(const MatrixComplex::DenseData& other) {
        unsigned long numberRows    = other.numberRows();
        unsigned long numberColumns = other.numberColumns();

        DenseData* result = create(numberRows, numberColumns);

        for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
            for (unsigned long rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                MatrixComplex::DenseData::Scalar v = other.at(rowIndex, columnIndex);
                if (v != MatrixComplex::DenseData::Scalar(0)) {
                    result->update(rowIndex, columnIndex, true);
                }
            }
        }

        return result;
    }


    MatrixBoolean::DenseData* MatrixBoolean::DenseData::createUninitialized(
            unsigned long newNumberRows,
            unsigned long newNumberColumns
        ) {
        unsigned long long columnSizeInBits   = recommendedColumnSize(newNumberRows);
        unsigned long long numberBits         = columnSizeInBits * std::max(1UL, newNumberColumns);
        unsigned long long numberStorageUnits = (numberBits + storageUnitSizeInBits - 1) / storageUnitSizeInBits;
        unsigned long long numberBytes        = numberStorageUnits * sizeof(StorageUnit);

        void* storage;
        void* p = allocateMemory(storage, numberBytes, sizeof(DenseData));

        return new(p) DenseData(newNumberRows, newNumberColumns, columnSizeInBits, numberStorageUnits, storage);
    }


    void MatrixBoolean::DenseData::destroy(MatrixBoolean::DenseData* instance) {
        instance->~DenseData();
        releaseMemory(instance);
    }


    MatrixBoolean::DenseData* MatrixBoolean::DenseData::identity(
            unsigned long numberRows,
            unsigned long numberColumns
        ) {
        DenseData* result = create(numberRows, numberColumns);

        unsigned long numberDiagnonalEntries = std::min(numberRows, numberColumns);
        for (unsigned long index=0 ; index<numberDiagnonalEntries ; ++index) {
            result->update(index, index, true);
        }

        return result;
    }


    MatrixBoolean::DenseData* MatrixBoolean::DenseData::ones(unsigned long numberRows, unsigned long numberColumns) {
        static constexpr StorageUnit storageUnit1 = static_cast<StorageUnit>(1);

        DenseData*         result                = createUninitialized(numberRows, numberColumns);
        StorageUnit*       d                     = result->rawData;
        unsigned long long allocatedStorageUnits = result->currentAllocation;

        if (numberRows == 1 || numberColumns == 1) {
            unsigned long long totalEntries          = numberRows * numberColumns;
            unsigned long long fullStorageUnits      = totalEntries / storageUnitSizeInBits;
            unsigned           residueEntries        = totalEntries % storageUnitSizeInBits;
            unsigned long long residueStorageUnits   =   residueEntries == 0
                                                       ? allocatedStorageUnits - fullStorageUnits
                                                       : allocatedStorageUnits - fullStorageUnits - 1;

            if (fullStorageUnits > 0) {
                std::memset(d, 0xFF, fullStorageUnits * sizeof(StorageUnit));
                d += fullStorageUnits;
            }

            if (residueEntries > 0) {
                *d = (storageUnit1 << residueEntries) - 1;
                ++d;
            }

            if (residueStorageUnits > 0) {
                std::memset(d, 0x00, residueStorageUnits * sizeof(StorageUnit));
            }
        } else {
            unsigned long long columnSizeInBits = recommendedColumnSize(numberRows);
            if (numberRows == columnSizeInBits) {
                unsigned long long bits         = columnSizeInBits * numberColumns;
                unsigned long long storageUnits = (bits + storageUnitSizeInBits - 1) / storageUnitSizeInBits;
                unsigned long long bytes        = storageUnits * sizeof(StorageUnit);

                std::memset(result->rawData, 0xFF, bytes);
            } else if (columnSizeInBits <= storageUnitSizeInBits) {
                StorageUnit mask = (static_cast<StorageUnit>(1) << numberRows) - 1;
                unsigned    filledBits = columnSizeInBits;
                while (filledBits < storageUnitSizeInBits) {
                    mask = mask | (mask << filledBits);
                    filledBits <<= 1;
                }

                unsigned long long numberColumnBits       = columnSizeInBits * numberColumns;
                unsigned long long numberFullStorageUnits = numberColumnBits / storageUnitSizeInBits;
                unsigned           columnsPerStorageUnit  = storageUnitSizeInBits / columnSizeInBits;
                unsigned long long allocatedColumns       = allocatedStorageUnits * columnsPerStorageUnit;
                unsigned           residueColumns         = allocatedColumns - numberColumns;

                for (unsigned long long i=0 ; i<numberFullStorageUnits ; ++i) {
                    *d = mask;
                    ++d;
                }

                if (residueColumns > 0) {
                    StorageUnit residueMask = (
                          (storageUnit1 << (storageUnitSizeInBits - residueColumns * columnSizeInBits))
                        - 1
                    );
                    *d = residueMask & mask;
                }
            } else {
                unsigned long long fullStorageUnitsPerColumn = numberRows / storageUnitSizeInBits;
                unsigned long long residueRowsPerColumn      = numberRows % storageUnitSizeInBits;
                StorageUnit        residueMask               = (storageUnit1 << residueRowsPerColumn) - 1;

                assert(fullStorageUnitsPerColumn > 0);
                if (fullStorageUnitsPerColumn == 1) {
                    StorageUnit fullStorageUnit = static_cast<StorageUnit>(-1);
                    for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                        *d = fullStorageUnit;
                        ++d;
                        *d = residueMask;
                        ++d;
                    }
                } else {
                    unsigned long long bytesPerFullStorageUnit = fullStorageUnitsPerColumn * sizeof(StorageUnit);
                    for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                        std::memset(d, 0xFF, bytesPerFullStorageUnit);
                        d += fullStorageUnitsPerColumn;
                        *d = residueMask;
                        ++d;
                    }
                }
            }
        }

        return result;
    }


    bool MatrixBoolean::DenseData::toFile(const char* filename, Model::DataFileFormat fileFormat) const {
        bool success = true;

        switch (fileFormat) {
            case Model::DataFileFormat::CSV: {
                std::ofstream f(filename);

                if (!f) {
                    internalTriggerFileOpenError(filename);
                    success = false;
                }

                unsigned long rowIndex = 0;
                while (success && rowIndex < currentNumberRows) {
                    unsigned long columnIndex = 0;
                    while (success && columnIndex < currentNumberColumns) {
                        if (columnIndex > 0) {
                            f << "\t" << static_cast<unsigned>(at(rowIndex, columnIndex));
                        } else {
                            f << static_cast<unsigned>(at(rowIndex, columnIndex));
                        }

                        if (!f) {
                            internalTriggerFileWriteError(filename);
                            success = false;
                        }

                        ++columnIndex;
                    }

                    if (success) {
                        f << std::endl;
                        if (!f) {
                            internalTriggerFileWriteError(filename);
                            success = false;
                        }
                    }

                    ++rowIndex;
                }

                if (success) {
                    f.close();
                    if (!f) {
                        internalTriggerFileCloseError(filename);
                        success = false;
                    }
                }

                break;
            }

            case Model::DataFileFormat::BINARY: {
                std::ofstream f(filename, std::ios::binary);
                if (!f) {
                    internalTriggerFileOpenError(filename);
                    success = false;
                }

                unsigned      i      = 6;
                std::uint8_t* buffer = new std::uint8_t[4096];

                std::memcpy(buffer, "INEBIN", 6);
                buffer[i++] = 0;
                buffer[i++] = 'B';

                buffer[i++] = static_cast<std::uint8_t>(currentNumberRows      );
                buffer[i++] = static_cast<std::uint8_t>(currentNumberRows >>  8);
                buffer[i++] = static_cast<std::uint8_t>(currentNumberRows >> 16);
                buffer[i++] = static_cast<std::uint8_t>(currentNumberRows >> 24);

                buffer[i++] = static_cast<std::uint8_t>(currentNumberColumns      );
                buffer[i++] = static_cast<std::uint8_t>(currentNumberColumns >>  8);
                buffer[i++] = static_cast<std::uint8_t>(currentNumberColumns >> 16);
                buffer[i++] = static_cast<std::uint8_t>(currentNumberColumns >> 24);

                std::uint8_t  mask     = 1;
                unsigned long rowIndex = 0;
                std::uint8_t  v        = 0;
                while (success && rowIndex<currentNumberRows) {
                    unsigned long columnIndex = 0;
                    while (success && columnIndex<currentNumberColumns) {
                        if (at(rowIndex, columnIndex)) {
                            v |= mask;
                        }

                        mask <<= 1;
                        if (mask == 0) {
                            buffer[i++] = v;
                            v = 0;

                            mask = 1;

                            if (i == 4096) {
                                f.write(reinterpret_cast<const char*>(buffer), 4096);
                                if (!f) {
                                    delete[] buffer;
                                    buffer = nullptr;

                                    internalTriggerFileWriteError(filename);
                                    success = false;
                                }

                                i = 0;
                            }
                        }

                        ++columnIndex;
                    }

                    ++rowIndex;
                }

                if (success) {
                    if (mask != 1 || i != 0) {
                        if (mask != 1) {
                            buffer[i++] = v;
                        }

                        f.write(reinterpret_cast<const char*>(buffer), i);
                        if (!f) {
                            delete[] buffer;
                            buffer = nullptr;

                            internalTriggerFileWriteError(filename);
                            success = false;
                        }
                    }
                }

                if (buffer != nullptr) {
                    delete[] buffer;
                }

                if (success) {
                    f.close();
                    if (!f) {
                        internalTriggerFileCloseError(filename);
                        success = false;
                    }
                }

                break;
            }

            default: {
                internalTriggerInvalidParameterValueError();
                success = false;

                break;
            }
        }

        return success;
    }


    MatrixBoolean::DenseData* MatrixBoolean::DenseData::fromFile(const char* filename) {
        bool                  success    = true;
        DenseData*            result     = nullptr;
        Model::DataFileFormat fileFormat = Model::DataFileFormat::CSV;
        std::ifstream         f(filename, std::ios::binary);

        if (f) {
            char buffer[8];
            f.read(buffer, 8);

            if (f) {
                if (strncmp("INEBIN", buffer, 6) == 0) {
                    fileFormat = Model::DataFileFormat::BINARY;
                } else {
                    fileFormat = Model::DataFileFormat::CSV;
                }
            } else {
                internalTriggerFileReadError(filename);
                success = false;
            }

            if (success) {
                f.close();
                if (!f) {
                    internalTriggerFileCloseError(filename);
                    success = false;
                }
            }
        } else {
            internalTriggerFileOpenError(filename);
            success = false;
        }

        if (success) {
            switch (fileFormat) {
                case Model::DataFileFormat::CSV: {
                    f.open(filename);
                    if (!f) {
                        internalTriggerFileOpenError(filename);
                        success = false;
                    }

                    std::vector<std::vector<bool>> matrixData;
                    unsigned long                  numberColumns = 0;

                    while (success && f.peek() != EOF) { // IMHO, C++ streams are broken when testing against EOF.
                        std::vector<bool> rowArray;

                        std::string line;
                        std::getline(f, line);
                        if (!f) {
                            internalTriggerFileReadError(filename);
                            success = false;
                        } else {
                            std::string value;
                            for (std::string::const_iterator it=line.cbegin(),end=line.cend() ; it!=end ; ++it) {
                                char c = *it;
                                if (c == '\t' || c == ' ' || c == ',' || c == ';' || c == '|' || c == ':') {
                                    rowArray.push_back(value == "0" ? false : true);
                                    value.clear();
                                } else {
                                    value += c;
                                }
                            }

                            if (value.length() > 0) {
                                rowArray.push_back(value == "0" ? false : true);
                            }

                            matrixData.push_back(rowArray);
                            unsigned long rowLength = static_cast<unsigned long>(rowArray.size());
                            if (rowLength > numberColumns) {
                                numberColumns = rowLength;
                            }
                        }
                    }

                    if (success) {
                        f.close();
                        if (!f) {
                            internalTriggerFileCloseError(filename);
                            success = false;
                        }
                    }

                    if (success) {
                        unsigned long numberRows = static_cast<unsigned long>(matrixData.size());
                        result = create(numberRows, numberColumns);

                        for (unsigned long rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                            const std::vector<bool>& rowArray = matrixData.at(rowIndex);
                            unsigned long numberColumns = static_cast<unsigned long>(rowArray.size());
                            for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                                result->update(rowIndex, columnIndex, rowArray.at(columnIndex));
                            }
                        }
                    }

                    break;
                }

                case Model::DataFileFormat::BINARY: {
                    f.open(filename);
                    if (!f) {
                        internalTriggerFileOpenError(filename);
                        success = false;
                    } else {
                        std::uint8_t header[8 + 4 + 4];
                        f.read(reinterpret_cast<char*>(header), 8 + 4 + 4);
                        if (!f) {
                            internalTriggerFileReadError(filename);
                            success = false;
                        } else {
                            if (header[7] != 'B') {
                                internalTriggerFileReadError(filename, 1);
                                success = false;
                            } else {
                                unsigned long numberRows = (
                                      (static_cast<unsigned long>(header[ 8])      )
                                    | (static_cast<unsigned long>(header[ 9]) <<  8)
                                    | (static_cast<unsigned long>(header[10]) << 16)
                                    | (static_cast<unsigned long>(header[11]) << 24)
                                );

                                unsigned long numberColumns = (
                                      (static_cast<unsigned long>(header[12])      )
                                    | (static_cast<unsigned long>(header[13]) <<  8)
                                    | (static_cast<unsigned long>(header[14]) << 16)
                                    | (static_cast<unsigned long>(header[15]) << 24)
                                );

                                unsigned long numberCoefficients = numberRows * numberColumns;
                                unsigned long bufferSize         = (numberCoefficients + 7) / 8;
                                std::uint8_t* buffer             = new std::uint8_t[bufferSize];

                                f.read(reinterpret_cast<char*>(buffer), bufferSize);
                                if (!f) {
                                    delete[] buffer;
                                    buffer = nullptr;

                                    internalTriggerFileReadError(filename);
                                    success = false;
                                }

                                result = DenseData::create(numberRows, numberColumns);

                                std::vector<std::vector<bool>> matrixData;

                                unsigned long i        = 0;
                                std::uint8_t  mask     = 1;
                                unsigned long rowIndex = 0;
                                while (success && rowIndex < numberRows) {
                                    unsigned long columnIndex = 0;
                                    while (success && columnIndex < numberColumns) {
                                        bool isSet = (buffer[i] & mask);
                                        result->update(rowIndex, columnIndex, isSet);

                                        mask <<= 1;
                                        if (mask == 0) {
                                            mask = 1;
                                            ++i;
                                        }

                                        ++columnIndex;
                                    }

                                    ++rowIndex;
                                }

                                if (buffer != nullptr) {
                                    delete[] buffer;
                                }
                            }
                        }
                    }

                    break;
                }

                default: {
                    internalTriggerInvalidParameterValueError();
                    success = false;
                    break;
                }
            }
        }

        if (!success) {
            if (result != nullptr) {
                destroy(result);
            }

            result = DenseData::create(0, 0);
        }

        return result;
    }


    MatrixBoolean::Scalar MatrixBoolean::DenseData::at(unsigned long rowIndex, unsigned long columnIndex) const {
        unsigned long long bitOffset         = rowIndex + columnIndex * columnSizeInBits;
        unsigned long long storageUnitIndex  = bitOffset / storageUnitSizeInBits;
        unsigned           storageUnitOffset = bitOffset % storageUnitSizeInBits;
        StorageUnit        mask              = StorageUnit(1) << storageUnitOffset;

        return (rawData[storageUnitIndex] & mask) != 0;
    }


    void MatrixBoolean::DenseData::update(unsigned long rowIndex, unsigned long columnIndex, Scalar newValue) {
        unsigned long long bitOffset         = rowIndex + columnIndex * columnSizeInBits;
        unsigned long long storageUnitIndex  = bitOffset / storageUnitSizeInBits;
        unsigned           storageUnitOffset = bitOffset % storageUnitSizeInBits;
        StorageUnit        mask              = StorageUnit(1) << storageUnitOffset;

        if (newValue == true) {
            rawData[storageUnitIndex] |= mask;
        } else {
            rawData[storageUnitIndex] &= ~mask;
        }
    }


    MatrixBoolean::Data* MatrixBoolean::DenseData::combineLeftToRight(const MatrixBoolean::DenseData& other) const {
        DenseData* result;

        unsigned long otherNumberRows     = other.currentNumberRows;
        unsigned long otherNumberColumns  = other.currentNumberColumns;

        if (otherNumberRows != 0 && otherNumberColumns != 0) {
            unsigned long resultNumberRows    = std::max(currentNumberRows, otherNumberRows);
            unsigned long resultNumberColumns = currentNumberColumns + otherNumberColumns;

            result = resizeOutOfPlace(resultNumberRows, resultNumberColumns);

            unsigned long resultColumnSizeInBits = result->columnSizeInBits;
            unsigned long otherColumnSizeInBits  = other.columnSizeInBits;

            if (resultColumnSizeInBits >= storageUnitSizeInBits) {
                // Result is aligned to storage units.

                assert((resultColumnSizeInBits % storageUnitSizeInBits) == 0);

                if (otherColumnSizeInBits >= storageUnitSizeInBits) {
                    // right is aligned to storage units

                    assert((otherColumnSizeInBits % storageUnitSizeInBits) == 0);

                    unsigned long resultColumnSizeInStorageUnits = resultColumnSizeInBits / storageUnitSizeInBits;
                    unsigned long otherColumnSizeInStorageUnits  = otherColumnSizeInBits / storageUnitSizeInBits;
                    unsigned long otherColumnSizeInBytes         = otherColumnSizeInStorageUnits * sizeof(StorageUnit);

                    const StorageUnit* s = other.rawData;
                    StorageUnit*       d = result->rawData + resultColumnSizeInStorageUnits * currentNumberColumns;

                    for (unsigned long columnIndex=0 ; columnIndex<otherNumberColumns ; ++columnIndex) {
                        std::memcpy(d, s, otherColumnSizeInBytes);
                        s += otherColumnSizeInStorageUnits;
                        d += resultColumnSizeInStorageUnits;
                    }
                } else {
                    // right is bit packed within one or more storage units.  Right side columns never cross storage
                    // units.

                    assert((storageUnitSizeInBits % otherColumnSizeInBits) == 0);

                    unsigned long resultColumnSizeInStorageUnits = resultColumnSizeInBits / storageUnitSizeInBits;
                    StorageUnit*  d = result->rawData + resultColumnSizeInStorageUnits * currentNumberColumns;

                    StorageUnit sourceMask = (StorageUnit(1) << otherNumberRows) - 1;

                    for (unsigned long columnIndex=0 ; columnIndex<otherNumberColumns ; ++columnIndex) {
                        unsigned long long matrixOffset = other.columnSizeInBits * columnIndex;
                        unsigned long      storageUnit  = matrixOffset / storageUnitSizeInBits;
                        unsigned           bitOffset    = static_cast<unsigned>(matrixOffset % storageUnitSizeInBits);

                        StorageUnit sourceData = (other.rawData[storageUnit] >> bitOffset) & sourceMask;
                        *d = sourceData;

                        d += resultColumnSizeInStorageUnits;
                    }
                }
            } else {
                // Result, left, and right are *not* storage unit aligned.

                assert((storageUnitSizeInBits % otherColumnSizeInBits) == 0);
                assert((storageUnitSizeInBits % resultColumnSizeInBits) == 0);

                unsigned long long matrixOffset = currentNumberColumns * resultColumnSizeInBits;
                unsigned long      storageUnit  = static_cast<unsigned long>(matrixOffset / storageUnitSizeInBits);
                unsigned           bitOffset    = static_cast<unsigned>(matrixOffset % storageUnitSizeInBits);
                StorageUnit*       d            = result->rawData + storageUnit;
                StorageUnit        resultData   = *d;

                const StorageUnit* s               = other.rawData;
                StorageUnit        sourceData      = *s;
                unsigned           sourceBitOffset = 0;

                for (unsigned long columnIndex=0 ; columnIndex<otherNumberColumns ; ++columnIndex) {
                    StorageUnit columnData = readAndAdjustSourcePointer(
                        sourceBitOffset,
                        s,
                        sourceData,
                        static_cast<unsigned>(otherColumnSizeInBits)
                    );

                    writeAndAdjustDestinationPointer(
                        bitOffset,
                        d,
                        resultData,
                        static_cast<unsigned>(resultColumnSizeInBits),
                        columnData
                    );
                }

                if (bitOffset != 0) {
                    *d = resultData;
                }
            }
        } else {
            result = POLYMORPHIC_CAST<DenseData*>(clone());
        }

        return result;
    }


    MatrixBoolean::Data* MatrixBoolean::DenseData::combineLeftToRight(const MatrixBoolean::SparseData& other) const {
        (void) other; // FIXME
        return nullptr; // FIXME
    }


    MatrixBoolean::Data* MatrixBoolean::DenseData::combineTopToBottom(const MatrixBoolean::DenseData& other) const {
        DenseData* result;

        unsigned long otherNumberRows     = other.currentNumberRows;
        unsigned long otherNumberColumns  = other.currentNumberColumns;

        if (otherNumberRows != 0 && otherNumberColumns != 0) {
            unsigned long resultNumberRows    = currentNumberRows + otherNumberRows;
            unsigned long resultNumberColumns = std::max(currentNumberColumns, otherNumberColumns);

            result = resizeOutOfPlace(resultNumberRows, resultNumberColumns);

            unsigned long resultColumnSizeInBits = result->columnSizeInBits;
            unsigned long otherColumnSizeInBits  = other.columnSizeInBits;

            for (unsigned long columnIndex=0 ; columnIndex<otherNumberColumns ; ++columnIndex) {
                unsigned long long sourceColumnStart = otherColumnSizeInBits * columnIndex;
                unsigned long long resultColumnStart = resultColumnSizeInBits * columnIndex + currentNumberRows;

                unsigned long sourceUnitOffset = static_cast<unsigned long>(sourceColumnStart / storageUnitSizeInBits);
                unsigned      sourceBitOffset  = static_cast<unsigned>(sourceColumnStart % storageUnitSizeInBits);
                unsigned long resultUnitOffset = static_cast<unsigned long>(resultColumnStart / storageUnitSizeInBits);
                unsigned      resultBitOffset  = static_cast<unsigned>(resultColumnStart % storageUnitSizeInBits);
                unsigned      nextUnitShift   = storageUnitSizeInBits - resultBitOffset;

                StorageUnit* s = other.rawData + sourceUnitOffset;
                StorageUnit* d = result->rawData + resultUnitOffset;

                StorageUnit  resultData = *d;

                assert(otherColumnSizeInBits < 128 || sourceBitOffset == 0);

                unsigned long otherBitsRemaining = otherNumberRows;
                while (otherBitsRemaining >= storageUnitSizeInBits) {
                    StorageUnit sourceData = *s++;
                    resultData |= sourceData << resultBitOffset;
                    *d = resultData;

                    ++d;
                    if (resultBitOffset == 0) {
                        resultData = 0;
                    } else {
                        resultData = sourceData >> nextUnitShift;
                    }

                    otherBitsRemaining -= storageUnitSizeInBits;
                }

                if (otherBitsRemaining > 0) {
                    StorageUnit remainingRowsMask = (StorageUnit(1) << otherBitsRemaining) - 1;
                    StorageUnit sourceData        = ((*s) >> sourceBitOffset) & remainingRowsMask;

                    resultData |= sourceData << resultBitOffset;
                    if (otherBitsRemaining > nextUnitShift) {
                        *d = resultData;

                        ++d;
                        resultData = (*d) | (sourceData >> nextUnitShift);
                    }

                    *d = resultData;
                } else {
                    if (resultBitOffset != 0) {
                        *d = resultData;
                    }
                }
            }
        } else {
            result = POLYMORPHIC_CAST<DenseData*>(clone());
        }

        return result;
    }


    MatrixBoolean::Data* MatrixBoolean::DenseData::combineTopToBottom(const MatrixBoolean::SparseData& other) const {
        (void) other; // FIXME
        return nullptr; // FIXME
    }


    MatrixBoolean::Data* MatrixBoolean::DenseData::columnReverse() const {
        DenseData* result;

        if (currentNumberColumns == 0 || currentNumberRows == 0) {
            result = createUninitialized(currentNumberRows, currentNumberColumns);
        } else if (currentNumberColumns == 1) {
            result = POLYMORPHIC_CAST<DenseData*>(clone());
        } else {
            result = createUninitialized(currentNumberRows, currentNumberColumns);

            if (columnSizeInBits >= storageUnitSizeInBits) {
                // Columns are storage unit aligned so we can simply memcpy them into position in reverse order.

                assert(columnSizeInBits == result->columnSizeInBits);

                unsigned long columnSizeInUnits = static_cast<unsigned long>(columnSizeInBits / storageUnitSizeInBits);
                unsigned long columnSizeInBytes = columnSizeInUnits * sizeof(StorageUnit);

                StorageUnit* s = rawData + (currentNumberColumns - 1) * columnSizeInUnits;
                StorageUnit* d = result->rawData;

                for (unsigned columnIndex=0 ; columnIndex<currentNumberColumns ; ++columnIndex) {
                    std::memcpy(d, s, columnSizeInBytes);
                    d += columnSizeInUnits;
                    s -= columnSizeInUnits;
                }
            } else {
                // Columns are bit packed so we much unpack them and re-insert them.

                StorageUnit*  d                      = result->rawData;
                StorageUnit   resultData             = 0;
                unsigned      resultBitOffset        = 0;
                unsigned      resultColumnSizeInBits = static_cast<unsigned>(result->columnSizeInBits);

                unsigned long columnIndex = currentNumberColumns;
                StorageUnit   columnMask  = (StorageUnit(1) << columnSizeInBits) - 1;
                do {
                    --columnIndex;

                    unsigned long long srcBitIndex  = columnIndex * columnSizeInBits;
                    unsigned long      srcUnitIndex = static_cast<unsigned long>(srcBitIndex / storageUnitSizeInBits);
                    unsigned           srcBitOffset = static_cast<unsigned>(srcBitIndex % storageUnitSizeInBits);
                    StorageUnit        columnData   = (rawData[srcUnitIndex] >> srcBitOffset) & columnMask;

                    writeAndAdjustDestinationPointer(
                        resultBitOffset,
                        d,
                        resultData,
                        resultColumnSizeInBits,
                        columnData
                    );
                } while (columnIndex > 0);

                if (resultBitOffset != 0) {
                    *d = resultData;
                }
            }
        }

        return result;
    }


    MatrixBoolean::Data* MatrixBoolean::DenseData::rowReverse() const {
        DenseData* result;

        if (currentNumberColumns == 0 || currentNumberRows == 0) {
            result = createUninitialized(currentNumberRows, currentNumberColumns);
        } else if (currentNumberRows == 1) {
            result = POLYMORPHIC_CAST<DenseData*>(clone());
        } else {
            result = createUninitialized(currentNumberRows, currentNumberColumns);

            unsigned shiftAmount = columnSizeInBits - currentNumberRows;

            if (columnSizeInBits >= storageUnitSizeInBits) {
                // Columns are at least a storage unit in size.

                assert(columnSizeInBits == result->columnSizeInBits);

                unsigned long columnSizeInUnits = static_cast<unsigned long>(columnSizeInBits / storageUnitSizeInBits);
                StorageUnit*  d                 = result->rawData;

                for (unsigned long columnIndex=0 ; columnIndex<currentNumberColumns ; ++columnIndex) {
                    unsigned long      unitIndex = (columnIndex + 1) * columnSizeInUnits - 1;
                    const StorageUnit* s         = rawData + unitIndex;

                    if (shiftAmount == 0) {
                        for (unsigned long i=0 ; i<columnSizeInUnits ; ++i) {
                            *d++ = reverse(s--);
                        }
                    } else {
                        StorageUnit destinationData    = reverse(s--) >> shiftAmount;
                        unsigned    residueShiftAmount = storageUnitSizeInBits - shiftAmount;

                        for (unsigned long i=1 ; i<columnSizeInUnits ; ++i) {
                            StorageUnit su = reverse(s--);
                            destinationData |= su << residueShiftAmount;

                            *d++ = destinationData;

                            destinationData = su >> shiftAmount;
                        }

                        *d++ = destinationData;
                    }
                }
            } else {
                // Columns are less than a storage unit in size.

                unsigned           sourceBitShiftValue      = 0;
                const StorageUnit* s                        = rawData;
                StorageUnit        sourceData               = *s;
                unsigned           destinationBitShiftValue = 0;
                StorageUnit*       d                        = result->rawData;
                StorageUnit        destinationData          = 0;

                for (unsigned columnIndex=0 ; columnIndex<currentNumberColumns ; ++columnIndex) {
                    StorageUnit columnData = readAndAdjustSourcePointer(
                        sourceBitShiftValue,
                        s,
                        sourceData,
                        columnSizeInBits
                    );

                    writeAndAdjustDestinationPointer(
                        destinationBitShiftValue,
                        d,
                        destinationData,
                        columnSizeInBits,
                        reverse(columnData, currentNumberRows)
                    );
                }

                if (destinationBitShiftValue != 0) {
                    *d = destinationData;
                }
            }
        }

        return result;
    }


    MatrixBoolean::Data* MatrixBoolean::DenseData::resizeTo(
            unsigned long newNumberRows,
            unsigned long newNumberColumns,
            bool          alwaysReallocate
        ) {
        DenseData* result;

        if (alwaysReallocate || !resizeInPlaceTo(newNumberRows, newNumberColumns)) {
            result = resizeOutOfPlace(newNumberRows, newNumberColumns);
        } else {
            result = this;
        }

        return result;
    }


    bool MatrixBoolean::DenseData::resizeInPlaceTo(unsigned long newNumberRows, unsigned long newNumberColumns) {
        bool success;

        if (newNumberRows == currentNumberRows && newNumberColumns == currentNumberColumns) {
            // Matrix size isn't changing.
            success = true;
        } else {
            unsigned long long newColumnSizeInBits   = recommendedColumnSize(newNumberRows);

            if (newNumberRows == 0 || newNumberColumns == 0) {
                // Matrix is now zero size.
                zeroMemory();

                currentNumberRows    = newNumberRows;
                currentNumberColumns = newNumberColumns;
                columnSizeInBits     = newColumnSizeInBits;

                success = true;
            } else {
                // Matrix size is changing and is not zero size.

                unsigned long long newNumberBits         = newColumnSizeInBits * std::max(1UL, newNumberColumns);
                unsigned long long newNumberStorageUnits = (
                      (newNumberBits + storageUnitSizeInBits - 1)
                    / storageUnitSizeInBits
                );

                if (currentAllocation >= newNumberStorageUnits) {
                    if (currentNumberRows == 0 || currentNumberColumns == 0) {
                        // Matrix is already zero size.

                        currentNumberRows    = newNumberRows;
                        currentNumberColumns = newNumberColumns;
                        columnSizeInBits     = newColumnSizeInBits;

                        success = true;
                    } else {
                        unsigned long numberCopiedColumns = std::min(currentNumberColumns, newNumberColumns);

                        if (newColumnSizeInBits == columnSizeInBits) {
                            // No movement of data is is needed but we might need to zero out stuff.

                            if (newNumberRows < currentNumberRows) {
                                for (unsigned columnIndex=0 ; columnIndex<numberCopiedColumns ; ++columnIndex) {
                                    unsigned long long columnBitOffset         = columnSizeInBits * columnIndex;
                                    unsigned long long columnStartingBitOffset = columnBitOffset + newNumberRows;
                                    unsigned long      currentLastRow          = currentNumberRows - 1;
                                    unsigned long long columnEndingBitOffset   = columnBitOffset + currentLastRow;

                                    zeroBits(columnStartingBitOffset, columnEndingBitOffset);
                                }
                            }

                            if (newNumberColumns < currentNumberColumns) {
                                unsigned long long currentEndingBitOffset = currentNumberColumns * columnSizeInBits;
                                unsigned long long newEndingBitOffset     = newNumberColumns * columnSizeInBits;

                                zeroBits(newEndingBitOffset, currentEndingBitOffset);
                            }

                            currentNumberRows    = newNumberRows;
                            currentNumberColumns = newNumberColumns;

                            success = true;
                        } else {
                            // Movement of data is needed, it's likely faster to build a new data instance than to move
                            // everything around (write v/s read-modify-writes).

                            success= false;
                        }
                    }
                } else {
                    // New matrix physically can't fit.
                    success = false;
                }
            }
        }

        return success;
    }


    bool MatrixBoolean::DenseData::isEqualTo(const MatrixBoolean::DenseData& other) const {
        bool isEqual;

        if (currentNumberRows != other.currentNumberRows || currentNumberColumns != other.currentNumberColumns) {
            isEqual = false;
        } else {
            assert(columnSizeInBits == other.columnSizeInBits);

            unsigned long long bitsToCompare         = columnSizeInBits * currentNumberColumns;
            unsigned long long storageUnitsToCompare = bitsToCompare / storageUnitSizeInBits;

            if (memcmp(rawData, other.rawData, storageUnitsToCompare * sizeof(StorageUnit)) == 0) {
                unsigned residueToCompare = static_cast<unsigned>(bitsToCompare % storageUnitSizeInBits);
                if (residueToCompare == 0) {
                    isEqual = true;
                } else {
                    StorageUnit mask = (StorageUnit(1) << residueToCompare) - 1;
                    StorageUnit r1   = rawData[storageUnitsToCompare] & mask;
                    StorageUnit r2   = other.rawData[storageUnitsToCompare] & mask;

                    isEqual = (r1 == r2);
                }
            } else {
                isEqual = false;
            }
        }

        return isEqual;
    }


    bool MatrixBoolean::DenseData::isEqualTo(const MatrixBoolean::SparseData& other) const {
        (void) other; // FIXME
        return false; // FIXME
    }


    MatrixBoolean::Data* MatrixBoolean::DenseData::applyTransform() const {
        DenseData* result = createUninitialized(currentNumberColumns, currentNumberRows);

        StorageUnit* d               = result->rawData;
        StorageUnit  destinationData = 0;
        StorageUnit  writeMask       = 1;
        unsigned     residueBits     = static_cast<unsigned>(result->columnSizeInBits - currentNumberColumns);

        for (unsigned long columnIndex=0 ; columnIndex<currentNumberRows ; ++columnIndex) {
            for (unsigned long rowIndex=0 ; rowIndex<currentNumberColumns ; ++rowIndex) {
                Scalar b = at(columnIndex, rowIndex);

                if (b == true) {
                    destinationData |= writeMask;
                }

                writeMask <<= 1;
                if (writeMask == 0) {
                    *d++ = destinationData;
                    writeMask       = 1;
                    destinationData = 0;
                }
            }

            writeMask <<= residueBits;
            if (writeMask == 0) {
                *d++ = destinationData;
                writeMask       = 1;
                destinationData = 0;
            }
        }

        if (writeMask != 1) {
            *d++ = destinationData;
        }

        StorageUnit* endOfData = result->rawData + result->currentAllocation;
        if (d != endOfData) {
            unsigned long long bytesToFill = (endOfData - d) * sizeof(StorageUnit);
            std::memset(d, 0, bytesToFill);
        }

        return result;
    }


    int MatrixBoolean::DenseData::relativeOrder(const MatrixBoolean::Data& other) const {
        int result;

        if (other.matrixType() == MatrixType::DENSE) {
            const DenseData& otherData = POLYMORPHIC_CAST<const DenseData&>(other);

            if (currentNumberRows == otherData.currentNumberRows) {
                if (currentNumberColumns == otherData.currentNumberColumns) {
                    unsigned long long numberUnits = std::min(currentAllocation, otherData.currentAllocation);
                    unsigned long long numberBytes = numberUnits * sizeof(StorageUnit);

                    result = std::memcmp(rawData, otherData.rawData, numberBytes);
                } else {
                    result = currentNumberColumns - otherData.currentNumberColumns;
                }
            } else {
                result = currentNumberRows - otherData.currentNumberRows;
            }
        } else {
            result = static_cast<int>(matrixType()) - static_cast<int>(other.matrixType());
        }

        return result;
    }


    unsigned long long MatrixBoolean::DenseData::recommendedColumnSize(unsigned long numberRows) {
        unsigned long long result;

        if (numberRows <= 1) {
            result = 1;
        } else if (numberRows < storageUnitSizeInBits) {
            unsigned long msbPosition;

            #if (defined(_MSC_VER))

                if (sizeof(unsigned long) == 4) {
                    (void) _BitScanReverse(&msbPosition, numberRows);
                } else {
                    assert(sizeof(unsigned long) == 8);
                    (void) _BitScanReverse64(&msbPosition, numberRows);
                }

                result = static_cast<unsigned long long>(1) << (msbPosition + 1);

            #elif (defined(__APPLE__) || defined(__linux__))

                msbPosition = 8 * sizeof(unsigned long) - __builtin_clzl(numberRows);
                result = static_cast<unsigned long long>(1) << msbPosition;

            #else

                #error Unknown platform

            #endif
        } else {
            unsigned long storageUnitsPerColumn = (numberRows + (storageUnitSizeInBits - 1)) / storageUnitSizeInBits;
            result = storageUnitSizeInBits * storageUnitsPerColumn;
        }

        assert(result >= numberRows);

        return result;
    }


    MatrixBoolean::DenseData* MatrixBoolean::DenseData::resizeOutOfPlace(
            unsigned long newNumberRows,
            unsigned long newNumberColumns
        ) const {
        DenseData*         result                 = createUninitialized(newNumberRows, newNumberColumns);
        StorageUnit*       resultData             = result->rawData;
        unsigned long long resultColumnSizeInBits = result->columnSizeInBits;
        unsigned long      copiedColumns          = std::min(currentNumberColumns, newNumberColumns);

        const StorageUnit* s = rawData;
        StorageUnit*       d = resultData;

        if (resultColumnSizeInBits >= storageUnitSizeInBits) {
            assert((resultColumnSizeInBits % storageUnitSizeInBits) == 0);

            unsigned long resultColumnSizeInStorageUnits = resultColumnSizeInBits / storageUnitSizeInBits;

            if (columnSizeInBits >= storageUnitSizeInBits) {
                // Both source & destination are storage unit aligned

                assert((columnSizeInBits % storageUnitSizeInBits) == 0);

                unsigned long sourceColumnSizeInStorageUnits = columnSizeInBits / storageUnitSizeInBits;
                unsigned long copiedStorageUnits             = std::min(
                    resultColumnSizeInStorageUnits,
                    sourceColumnSizeInStorageUnits
                );
                unsigned long copiedBytes                    = copiedStorageUnits * sizeof(StorageUnit);

                if (resultColumnSizeInStorageUnits > copiedStorageUnits) {
                    unsigned long residueInStorageUnits = resultColumnSizeInStorageUnits - copiedStorageUnits;
                    unsigned long residueInBytes        = residueInStorageUnits * sizeof(StorageUnit);
                    for (unsigned long columnIndex=0 ; columnIndex<copiedColumns ; ++columnIndex) {
                        std::memcpy(d, s, copiedBytes);
                        std::memset(d + copiedStorageUnits, 0, residueInBytes);

                        s += sourceColumnSizeInStorageUnits;
                        d += resultColumnSizeInStorageUnits;
                    }
                } else {
                    if (resultColumnSizeInBits > newNumberRows) {
                        unsigned      residueLength   = static_cast<unsigned>(resultColumnSizeInBits - newNumberRows);
                        StorageUnit   lastEntryMask   = static_cast<StorageUnit>(-1) >> residueLength;
                        unsigned long lastEntryOffset = resultColumnSizeInStorageUnits - 1;

                        for (unsigned long columnIndex=0 ; columnIndex<copiedColumns ; ++columnIndex) {
                            std::memcpy(d, s, copiedBytes);

                            if (residueLength > 0) {
                                *(d + lastEntryOffset) &= lastEntryMask;
                            }

                            s += sourceColumnSizeInStorageUnits;
                            d += resultColumnSizeInStorageUnits;
                        }
                    } else {
                        for (unsigned long columnIndex=0 ; columnIndex<copiedColumns ; ++columnIndex) {
                            std::memcpy(d, s, copiedBytes);
                            s += sourceColumnSizeInStorageUnits;
                            d += resultColumnSizeInStorageUnits;
                        }
                    }
                }
            } else {
                // destination is storage unit aligned, source is not storage unit aligned.

                assert((storageUnitSizeInBits % columnSizeInBits) == 0);
                assert(newNumberRows > currentNumberRows);

                StorageUnit sourceData     = *s;
                unsigned    sourceBitShift = 0;

                if (resultColumnSizeInStorageUnits > 1) {
                    unsigned long bytesToClearEachColumn = (
                          (resultColumnSizeInStorageUnits - 1)
                        * storageUnitSizeInBits
                        / 8
                    );

                    for (unsigned long columnIndex=0 ; columnIndex<copiedColumns ; ++columnIndex) {
                        StorageUnit v = readAndAdjustSourcePointer(
                            sourceBitShift,
                            s,
                            sourceData,
                            static_cast<unsigned>(columnSizeInBits)
                        );

                        *d = v;
                        memset(d + 1, 0, bytesToClearEachColumn);

                        d += resultColumnSizeInStorageUnits;
                    }
                } else {
                    for (unsigned long columnIndex=0 ; columnIndex<copiedColumns ; ++columnIndex) {
                        StorageUnit v = readAndAdjustSourcePointer(
                            sourceBitShift,
                            s,
                            sourceData,
                            static_cast<unsigned>(columnSizeInBits)
                        );

                        *d = v;
                        ++d;
                    }
                }
            }

            if (newNumberColumns > copiedColumns) {
                unsigned long long columnSizeInBytes = resultColumnSizeInBits / 8;
                for (unsigned long columnIndex=copiedColumns ; columnIndex<newNumberColumns ; ++columnIndex) {
                    std::memset(d, 0, columnSizeInBytes);
                    d += resultColumnSizeInStorageUnits;
                }
            }
        } else {
            assert((storageUnitSizeInBits % resultColumnSizeInBits) == 0);

            if (columnSizeInBits >= storageUnitSizeInBits) {
                // source is storage unit aligned, destination is not storage unit aligned.

                assert((columnSizeInBits % storageUnitSizeInBits) == 0);
                assert(newNumberRows < currentNumberRows);

                StorageUnit   resultData                     = 0;
                unsigned      resultBitShift                 = 0;
                StorageUnit   resultMask                     = (StorageUnit(1) << newNumberRows) - 1;
                unsigned long sourceColumnSizeInStorageUnits = columnSizeInBits / storageUnitSizeInBits;

                for (unsigned long columnIndex=0 ; columnIndex<copiedColumns ; ++columnIndex) {
                    StorageUnit v = (*s) & resultMask;
                    s += sourceColumnSizeInStorageUnits;

                    writeAndAdjustDestinationPointer(
                        resultBitShift,
                        d,
                        resultData,
                        static_cast<unsigned>(resultColumnSizeInBits),
                        v
                    );
                }

                if (resultBitShift != 0) {
                    *d = resultData;
                    ++d;
                }
            } else {
                // Neither source nor destination are storage unit aligned.

                StorageUnit resultData     = 0;
                StorageUnit sourceData     = *s;
                unsigned    sourceBitShift = 0;
                unsigned    resultBitShift = 0;
                StorageUnit resultMask     = (StorageUnit(1) << newNumberRows) - 1;

                for (unsigned long columnIndex=0 ; columnIndex<copiedColumns ; ++columnIndex) {
                    StorageUnit v = readAndAdjustSourcePointer(
                        sourceBitShift,
                        s,
                        sourceData,
                        static_cast<unsigned>(columnSizeInBits)
                    );

                    writeAndAdjustDestinationPointer(
                        resultBitShift,
                        d,
                        resultData,
                        static_cast<unsigned>(resultColumnSizeInBits),
                        v & resultMask
                    );
                }

                if (resultBitShift != 0) {
                    *d = resultData;
                    ++d;
                }
            }

            StorageUnit* endingPointer = resultData + result->currentAllocation;
            unsigned     numberResidueStorageUnits = static_cast<unsigned>(endingPointer - d);
            if (numberResidueStorageUnits > 0) {
                std::memset(d, 0, numberResidueStorageUnits * sizeof(StorageUnit));
            }
        }

        return result;
    }


    void MatrixBoolean::DenseData::zeroMemory() {
        std::memset(rawData, 0, currentAllocation * sizeof(StorageUnit));
    }


    void MatrixBoolean::DenseData::zeroBits(unsigned long long startingBitOffset, unsigned long long endingBitOffset) {
        unsigned long long startingStorageUnit = startingBitOffset / storageUnitSizeInBits;
        unsigned           startingOffset      = static_cast<unsigned>(startingBitOffset % storageUnitSizeInBits);

        unsigned long long endingStorageUnit   = (endingBitOffset - 1) / storageUnitSizeInBits;
        unsigned           endingOffset        = static_cast<unsigned>((endingBitOffset - 1) % storageUnitSizeInBits);

        if (startingStorageUnit == endingStorageUnit) {
            StorageUnit startingMask = (static_cast<StorageUnit>(1) << startingOffset) - 1;
            StorageUnit endingMask   = static_cast<StorageUnit>(-1) << (endingOffset + 1);

            rawData[startingStorageUnit] &= (startingMask | endingMask);
        } else {
            if (startingOffset > 0) {
                StorageUnit startingMask = (static_cast<StorageUnit>(1) << startingOffset) - 1;
                rawData[startingStorageUnit] &= startingMask;

                ++startingStorageUnit;
            }

            if (endingOffset == (storageUnitSizeInBits - 1)) {
                unsigned long storageUnitsToClear = endingStorageUnit - startingStorageUnit + 1;
                memset(rawData + startingStorageUnit, 0, storageUnitsToClear * sizeof(Scalar));
            } else {
                if (endingStorageUnit != startingStorageUnit) {
                    unsigned long storageUnitsToClear = endingStorageUnit - startingStorageUnit + 1;
                    memset(rawData + startingStorageUnit, 0, storageUnitsToClear * sizeof(Scalar));
                }

                StorageUnit endingMask = static_cast<StorageUnit>(-1) << (endingOffset + 1);
                rawData[endingStorageUnit] &= endingMask;
            }
        }
    }


    MatrixBoolean::DenseData::StorageUnit MatrixBoolean::DenseData::readAndAdjustSourcePointer(
            unsigned&                                     currentBitShiftValue,
            const MatrixBoolean::DenseData::StorageUnit*& currentDataPointer,
            MatrixBoolean::DenseData::StorageUnit&        currentData,
            unsigned                                      shiftAmount
        ) {
        StorageUnit mask   = (StorageUnit(1) << shiftAmount) - 1;
        StorageUnit result = (currentData >> currentBitShiftValue) & mask;

        currentBitShiftValue += shiftAmount;
        if (currentBitShiftValue >= storageUnitSizeInBits) {
            assert(currentBitShiftValue == storageUnitSizeInBits);

            ++currentDataPointer;
            currentData = *currentDataPointer;

            currentBitShiftValue = 0;
        }

        return result;
    }


    void MatrixBoolean::DenseData::writeAndAdjustDestinationPointer(
            unsigned&                               currentBitShiftValue,
            MatrixBoolean::DenseData::StorageUnit*& currentDataPointer,
            MatrixBoolean::DenseData::StorageUnit&  currentData,
            unsigned                                shiftAmount,
            MatrixBoolean::DenseData::StorageUnit   columnValue
        ) {
        currentData |= (columnValue << currentBitShiftValue);

        currentBitShiftValue += shiftAmount;
        if (currentBitShiftValue >= storageUnitSizeInBits) {
            assert(currentBitShiftValue == storageUnitSizeInBits);

            *currentDataPointer = currentData;
            ++currentDataPointer;

            currentData          = 0;
            currentBitShiftValue = 0;
        }
    }


    MatrixBoolean::DenseData::StorageUnit MatrixBoolean::DenseData::reverse(
            const MatrixBoolean::DenseData::StorageUnit* v
        ) {
        StorageUnitBytes out;

        out.bytes[7] = byteReverseTable[reinterpret_cast<const StorageUnitBytes*>(v)->bytes[0]];
        out.bytes[6] = byteReverseTable[reinterpret_cast<const StorageUnitBytes*>(v)->bytes[1]];
        out.bytes[5] = byteReverseTable[reinterpret_cast<const StorageUnitBytes*>(v)->bytes[2]];
        out.bytes[4] = byteReverseTable[reinterpret_cast<const StorageUnitBytes*>(v)->bytes[3]];
        out.bytes[3] = byteReverseTable[reinterpret_cast<const StorageUnitBytes*>(v)->bytes[4]];
        out.bytes[2] = byteReverseTable[reinterpret_cast<const StorageUnitBytes*>(v)->bytes[5]];
        out.bytes[1] = byteReverseTable[reinterpret_cast<const StorageUnitBytes*>(v)->bytes[6]];
        out.bytes[0] = byteReverseTable[reinterpret_cast<const StorageUnitBytes*>(v)->bytes[7]];

        return out.storageUnit;
    }


    MatrixBoolean::DenseData::StorageUnit MatrixBoolean::DenseData::reverse(
            MatrixBoolean::DenseData::StorageUnit v,
            unsigned                              size
        ) {
        StorageUnit result;

        if (size <= 8) {
            result = byteReverseTable[static_cast<std::uint8_t>(v)] >> (8 - size);
        } else if (size <= 16) {
            StorageUnitBytes in;
            in.storageUnit = v;

            StorageUnitBytes out;
            out.bytes[0] = byteReverseTable[in.bytes[1]];
            out.bytes[1] = byteReverseTable[in.bytes[0]];

            result = out.storageUnit >> (16 - size);
        } else if (size <= 24) {
            StorageUnitBytes in;
            in.storageUnit = v;

            StorageUnitBytes out;
            out.bytes[0] = byteReverseTable[in.bytes[2]];
            out.bytes[1] = byteReverseTable[in.bytes[1]];
            out.bytes[2] = byteReverseTable[in.bytes[0]];

            result = out.storageUnit >> (24 - size);
        } else if (size <= 32) {
            StorageUnitBytes in;
            in.storageUnit = v;

            StorageUnitBytes out;
            out.bytes[0] = byteReverseTable[in.bytes[3]];
            out.bytes[1] = byteReverseTable[in.bytes[2]];
            out.bytes[2] = byteReverseTable[in.bytes[1]];
            out.bytes[3] = byteReverseTable[in.bytes[0]];

            result = out.storageUnit >> (32 - size);
        } else if (size <= 40) {
            StorageUnitBytes in;
            in.storageUnit = v;

            StorageUnitBytes out;
            out.bytes[0] = byteReverseTable[in.bytes[4]];
            out.bytes[1] = byteReverseTable[in.bytes[3]];
            out.bytes[2] = byteReverseTable[in.bytes[2]];
            out.bytes[3] = byteReverseTable[in.bytes[1]];
            out.bytes[4] = byteReverseTable[in.bytes[0]];

            result = out.storageUnit >> (40 - size);
        } else if (size <= 48) {
            StorageUnitBytes in;
            in.storageUnit = v;

            StorageUnitBytes out;
            out.bytes[0] = byteReverseTable[in.bytes[5]];
            out.bytes[1] = byteReverseTable[in.bytes[4]];
            out.bytes[2] = byteReverseTable[in.bytes[3]];
            out.bytes[3] = byteReverseTable[in.bytes[2]];
            out.bytes[4] = byteReverseTable[in.bytes[1]];
            out.bytes[5] = byteReverseTable[in.bytes[0]];

            result = out.storageUnit >> (48 - size);
        } else if (size <= 56) {
            StorageUnitBytes in;
            in.storageUnit = v;

            StorageUnitBytes out;
            out.bytes[0] = byteReverseTable[in.bytes[6]];
            out.bytes[1] = byteReverseTable[in.bytes[5]];
            out.bytes[2] = byteReverseTable[in.bytes[4]];
            out.bytes[3] = byteReverseTable[in.bytes[3]];
            out.bytes[4] = byteReverseTable[in.bytes[2]];
            out.bytes[5] = byteReverseTable[in.bytes[1]];
            out.bytes[6] = byteReverseTable[in.bytes[0]];

            result = out.storageUnit >> (56 - size);
        } else {
            StorageUnitBytes in;
            in.storageUnit = v;

            StorageUnitBytes out;
            out.bytes[0] = byteReverseTable[in.bytes[7]];
            out.bytes[1] = byteReverseTable[in.bytes[6]];
            out.bytes[2] = byteReverseTable[in.bytes[5]];
            out.bytes[3] = byteReverseTable[in.bytes[4]];
            out.bytes[4] = byteReverseTable[in.bytes[3]];
            out.bytes[5] = byteReverseTable[in.bytes[2]];
            out.bytes[6] = byteReverseTable[in.bytes[1]];
            out.bytes[7] = byteReverseTable[in.bytes[0]];

            result = out.storageUnit >> (64 - size);
        }

        return result;
    }
}
