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
* This file implements the \ref M::MatrixReal::DenseData class.
***********************************************************************************************************************/

#include <new>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <chrono> // Debug

#include <boost/algorithm/string.hpp>

#include "m_api_types.h"
#include "m_intrinsic_types.h"
#include "m_basic_functions.h"
#include "model_exceptions.h"
#include "model_range.h"
#include "m_per_thread.h"
#include "m_matrix_dense_private.h"
#include "m_matrix_sparse_private.h"
#include "m_matrix_complex.h"
#include "m_matrix_complex_data.h"
#include "m_matrix_complex_dense_data.h"
#include "m_matrix_integer.h"
#include "m_matrix_integer_data.h"
#include "m_matrix_integer_dense_data.h"
#include "m_matrix_boolean.h"
#include "m_matrix_boolean_data.h"
#include "m_matrix_boolean_dense_data.h"
#include "m_matrix_boolean_sparse_data.h"
#include "m_matrix_real.h"
#include "m_matrix_real_data.h"
#include "m_matrix_real_dense_data.h"

namespace M {
    long double MatrixReal::DenseData::currentSmallLimit = -1;
    long double MatrixReal::DenseData::currentBigLimit   = -1;

    MatrixReal::DenseData::DenseData(
            unsigned long      newNumberRows,
            unsigned long      newNumberColumns,
            unsigned long long newAllocationInBytes,
            void*              rawDataBaseAddress
        ):MatrixDensePrivate<MatrixReal::DenseData::Scalar>(
            newNumberRows,
            newNumberColumns,
            newAllocationInBytes,
            rawDataBaseAddress
        ) {
    }


    MatrixReal::DenseData::DenseData(
            const MatrixDensePrivate<MatrixReal::DenseData::Scalar>& other,
            unsigned long long                                       newAllocationInBytes,
            void*                                                    rawDataBaseAddress
        ):MatrixDensePrivate<MatrixReal::DenseData::Scalar>(
            other,
            newAllocationInBytes,
            rawDataBaseAddress
        ) {}


    MatrixReal::DenseData::DenseData(
            const MatrixDensePrivate<MatrixReal::DenseData::Scalar>& other,
            unsigned long                                            newNumberRows,
            unsigned long                                            newNumberColumns,
            unsigned long long                                       newAllocationInBytes,
            void*                                                    rawDataBaseAddress
        ):MatrixDensePrivate<MatrixReal::DenseData::Scalar>(
            other,
            newNumberRows,
            newNumberColumns,
            newAllocationInBytes,
            rawDataBaseAddress
        ) {}


    MatrixReal::DenseData::~DenseData() {}


    MatrixReal::DenseData* MatrixReal::DenseData::create(const MatrixComplex::DenseData& other) {
        unsigned long numberRows    = other.numberRows();
        unsigned long numberColumns = other.numberColumns();

        DenseData*         result                   = createUninitialized(numberRows, numberColumns);
        Scalar*            d                        = result->data();
        unsigned long long destColumnSizeInEntries  = result->columnSpacingInMemory();
        unsigned long long destResidueSizeInEntries = destColumnSizeInEntries - numberRows;

        const MatrixComplex::DenseData::Scalar* s                       = other.data();
        unsigned long long                      srcColumnSizeInEntries  = other.columnSpacingInMemory();
        unsigned long long                      srcResidueSizeInEntries = srcColumnSizeInEntries - numberRows;

        if (destResidueSizeInEntries != 0) {
            for (unsigned long long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                for (unsigned long long rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                    if (s->imag() != 0) {
                        internalTriggerTypeConversionError(M::ValueType::MATRIX_COMPLEX, M::ValueType::MATRIX_REAL);
                    }

                    *d = s->real();

                    ++s;
                    ++d;
                }

                std::memset(d, 0, sizeof(Scalar) * destResidueSizeInEntries);

                s += srcResidueSizeInEntries;
                d += destResidueSizeInEntries;
            }
        } else {
            for (unsigned long long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                for (unsigned long long rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                    if (s->imag() != 0) {
                        internalTriggerTypeConversionError(M::ValueType::MATRIX_COMPLEX, M::ValueType::MATRIX_REAL);
                    }

                    *d = s->real();

                    ++s;
                    ++d;
                }

                s += srcResidueSizeInEntries;
            }
        }

        return result;
    }


    MatrixReal::DenseData* MatrixReal::DenseData::create(const MatrixInteger::DenseData& other) {
        unsigned long numberRows    = other.numberRows();
        unsigned long numberColumns = other.numberColumns();

        DenseData*    result                         = createUninitialized(numberRows, numberColumns);
        unsigned long destinationColumnSizeInEntries = result->columnSpacingInMemory();
        unsigned long sourceColumnSizeInEntries      = other.columnSpacingInMemory();
        unsigned      sourceResidueInEntries         = static_cast<unsigned>(sourceColumnSizeInEntries - numberRows);

        const MatrixInteger::Scalar* s = other.data();
        Scalar*                      d = result->data();

        if (destinationColumnSizeInEntries != numberRows) {
            unsigned destinationResidueInEntries = static_cast<unsigned>(destinationColumnSizeInEntries - numberRows);
            unsigned destinationResidueInBytes   = destinationResidueInEntries * sizeof(Scalar);

            for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                for (unsigned rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                    Scalar v = Scalar(*s);
                    *d = v;

                    ++s;
                    ++d;
                }

                std::memset(d, 0, destinationResidueInBytes);

                d += destinationResidueInEntries;
                s += sourceResidueInEntries;
            }
        } else {
            for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                for (unsigned rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                    Scalar v = Scalar(*s);
                    *d = v;

                    ++s;
                    ++d;
                }

                s += sourceResidueInEntries;
            }
        }

        return result;
    }


    MatrixReal::DenseData* MatrixReal::DenseData::create(const MatrixBoolean::DenseData& other) {
        unsigned long numberRows    = other.numberRows();
        unsigned long numberColumns = other.numberColumns();

        DenseData*         result                = createUninitialized(numberRows, numberColumns);
        Scalar*            d                     = result->data();
        unsigned long long columnSpacing         = result->columnSpacingInMemory();
        unsigned long long residueEntriesToClear = columnSpacing - numberRows;

        if (residueEntriesToClear > 0) {
            unsigned long long residueBytesToClear = residueEntriesToClear * sizeof(Scalar);
            for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                for (unsigned long rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                    MatrixBoolean::Scalar sourceValue      = other.at(rowIndex, columnIndex);
                    Scalar                destinationValue = sourceValue ? Scalar(1) : Scalar(0);

                    *d = destinationValue;
                    ++d;
                }

                std::memset(d, 0, residueBytesToClear);
                d += residueEntriesToClear;
            }
        } else {
            for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                for (unsigned long rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                    MatrixBoolean::Scalar sourceValue      = other.at(rowIndex, columnIndex);
                    Scalar                destinationValue = sourceValue ? Scalar(1) : Scalar(0);
                    *d = destinationValue;
                    ++d;
                }
            }
        }

        return result;
    }


    MatrixReal::DenseData* MatrixReal::DenseData::identity(Index numberRows, Index numberColumns) {
        DenseData* data = POLYMORPHIC_CAST<DenseData*>(create(numberRows, numberColumns));

        unsigned long      numberDiagnonals = std::min(data->numberRows(), data->numberColumns());
        unsigned long long columnSize       = data->columnSpacingInMemory();
        Scalar*            p                = data->data();

        for (unsigned i=0 ; i<numberDiagnonals ; ++i) {
            *p = Scalar(1);
            p += columnSize + 1;
        }

        return data;
    }


    MatrixReal::DenseData* MatrixReal::DenseData::ones(unsigned long numberRows, unsigned long numberColumns) {
        DenseData* result = createUninitialized(numberRows, numberColumns);

        unsigned long long columnSpacing = result->columnSpacingInMemory();
        if (columnSpacing == numberRows) {
            matrixApi()->doubleFill(numberRows * numberColumns, 1.0, result->data());
        } else {
            unsigned long long residuePerColumn     = columnSpacing - numberRows;
            unsigned           residueLengthInBytes = residuePerColumn * sizeof(Scalar);
            Scalar*            d                    = result->data();

            for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                matrixApi()->doubleFill(numberRows, 1, d);
                d += numberRows;
                std::memset(d, 0, residueLengthInBytes);
                d += residuePerColumn;
            }
        }

        return result;
    }


    bool MatrixReal::DenseData::toFile(const char* filename, Model::DataFileFormat fileFormat) const {
        bool          success              = true;
        unsigned long currentNumberRows    = static_cast<unsigned long>(numberRows());
        unsigned long currentNumberColumns = static_cast<unsigned long>(numberColumns());

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
                            f << "\t" << std::setprecision(17) << at(rowIndex, columnIndex);
                        } else {
                            f << std::setprecision(17) << at(rowIndex, columnIndex);
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
                        internalTriggerFileWriteError(filename);
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
                } else {
                    unsigned      i      = 6;
                    std::uint8_t* buffer = new std::uint8_t[4096];

                    std::memcpy(buffer, "INEBIN", 6);
                    buffer[i++] = 0;
                    buffer[i++] = 'R';

                    buffer[i++] = static_cast<std::uint8_t>(currentNumberRows      );
                    buffer[i++] = static_cast<std::uint8_t>(currentNumberRows >>  8);
                    buffer[i++] = static_cast<std::uint8_t>(currentNumberRows >> 16);
                    buffer[i++] = static_cast<std::uint8_t>(currentNumberRows >> 24);

                    buffer[i++] = static_cast<std::uint8_t>(currentNumberColumns      );
                    buffer[i++] = static_cast<std::uint8_t>(currentNumberColumns >>  8);
                    buffer[i++] = static_cast<std::uint8_t>(currentNumberColumns >> 16);
                    buffer[i++] = static_cast<std::uint8_t>(currentNumberColumns >> 24);

                    unsigned long rowIndex = 0;
                    while (success && rowIndex < currentNumberRows) {
                        unsigned long columnIndex = 0;
                        while (success && columnIndex < currentNumberColumns) {
                            *reinterpret_cast<Scalar*>(buffer + i) = at(rowIndex, columnIndex);
                            i += sizeof(Scalar);

                            if (i >= 4096) {
                                f.write(reinterpret_cast<const char*>(buffer), 4096);
                                if (!f) {
                                    delete[] buffer;
                                    buffer = nullptr;

                                    internalTriggerFileWriteError(filename);
                                    success = false;
                                }

                                i = 0;
                            }

                            ++columnIndex;
                        }

                        ++rowIndex;
                    }

                    if (success && i != 0) {
                        f.write(reinterpret_cast<const char*>(buffer), i);
                        if (!f) {
                            delete[] buffer;
                            buffer = nullptr;

                            internalTriggerFileWriteError(filename);
                            success = false;
                        }
                    }

                    if (buffer != nullptr) {
                        delete[] buffer;
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


    MatrixReal::DenseData* MatrixReal::DenseData::fromFile(const char* filename) {
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

            f.close();
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

                    std::vector<std::vector<Scalar>> matrixData;
                    unsigned long                    numberColumns = 0;

                    while (success && f.peek() != EOF) { // IMHO, C++ stream are broken when testing against EOF.
                        std::vector<Scalar> rowArray;

                        std::string line;
                        std::getline(f, line);
                        if (!f) {
                            internalTriggerFileReadError(filename);
                            success = false;
                        }

                        std::string                 value;
                        std::string::const_iterator it  = line.cbegin();
                        std::string::const_iterator end = line.cend();

                        while (success && it != end) {
                            char c = *it;
                            if (c == '\t' || c == ' ' || c == ',' || c == ';' || c == '|' || c == ':') {
                                size_t index;
                                Scalar rv = std::stod(value, &index);

                                if (index != value.length()) {
                                    internalTriggerFileReadError(filename);
                                    success = false;
                                } else {
                                    rowArray.push_back(rv);
                                    value.clear();
                                }
                            } else {
                                value += c;
                            }

                            ++it;
                        }

                        if (success && value.length() > 0) {
                            size_t index;
                            Scalar rv = std::stod(value, &index);

                            if (index != value.length()) {
                                internalTriggerFileReadError(filename);
                                success = false;
                            }

                            rowArray.push_back(rv);
                        }

                        if (success) {
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
                            const std::vector<Scalar>& rowArray = matrixData.at(rowIndex);
                            unsigned long numberColumns = static_cast<unsigned long>(rowArray.size());
                            for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                                result->at(rowIndex, columnIndex) = rowArray.at(columnIndex);
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
                            if (header[7] != 'R') {
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

                                unsigned long long numberCoefficients = numberRows * numberColumns;
                                unsigned long long bufferSize         = 8 * numberCoefficients;
                                std::uint8_t*      buffer             = new std::uint8_t[bufferSize];

                                f.read(reinterpret_cast<char*>(buffer), bufferSize);
                                if (!f) {
                                    delete[] buffer;
                                    buffer = nullptr;

                                    internalTriggerFileReadError(filename);
                                    success = false;
                                } else {
                                    result = DenseData::createUninitialized(numberRows, numberColumns);

                                    unsigned long long i = 0;
                                    for (unsigned long rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                                        for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                                            Scalar value = *reinterpret_cast<Scalar*>(buffer + i);
                                            i += sizeof(Scalar);
                                            result->at(rowIndex, columnIndex) = value;
                                        }
                                    }

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

        if (result == nullptr) {
            result = DenseData::create(0, 0);
        }

        return result;
    }


    MatrixReal::Data* MatrixReal::DenseData::combineLeftToRight(
            const MatrixDensePrivate<MatrixReal::DenseData::Scalar>& other
        ) const {
        unsigned long thisNumberRows      = numberRows();
        unsigned long thisNumberColumns   = numberColumns();
        unsigned long otherNumberRows     = other.numberRows();
        unsigned long otherNumberColumns  = other.numberColumns();
        unsigned long resultNumberRows    = std::max(thisNumberRows, otherNumberRows);
        unsigned long resultNumberColumns = thisNumberColumns + otherNumberColumns;

        DenseData* resultData = createUninitialized(resultNumberRows, resultNumberColumns);

        unsigned long long thisColumnSizeInBytes      = thisNumberRows * sizeof(Scalar);
        unsigned long long otherColumnSizeInBytes     = otherNumberRows * sizeof(Scalar);
        unsigned long long resultColumnSpacingInBytes = columnSizeInBytes(resultNumberRows, sizeof(Scalar));
        unsigned long long leftResultResidueInBytes   = resultColumnSpacingInBytes - thisColumnSizeInBytes;
        unsigned long long rightResultResidueInBytes  = resultColumnSpacingInBytes - otherColumnSizeInBytes;

        if (leftResultResidueInBytes > 0) {
            for (unsigned long sourceColumnIndex=0 ; sourceColumnIndex<thisNumberColumns ; ++sourceColumnIndex) {
                const std::uint8_t* s = reinterpret_cast<const std::uint8_t*>(addressOf(0, sourceColumnIndex));
                std::uint8_t*       d = reinterpret_cast<std::uint8_t*>(resultData->addressOf(0, sourceColumnIndex));

                std::memcpy(d, s, thisColumnSizeInBytes);
                std::memset(d + thisColumnSizeInBytes, 0, leftResultResidueInBytes);
            }
        } else {
            for (unsigned long sourceColumnIndex=0 ; sourceColumnIndex<thisNumberColumns ; ++sourceColumnIndex) {
                const std::uint8_t* s = reinterpret_cast<const std::uint8_t*>(addressOf(0, sourceColumnIndex));
                std::uint8_t*       d = reinterpret_cast<std::uint8_t*>(resultData->addressOf(0, sourceColumnIndex));

                std::memcpy(d, s, thisColumnSizeInBytes);
            }
        }

        unsigned long resultColumnIndex = thisNumberColumns;

        if (rightResultResidueInBytes > 0) {
            for (unsigned long sourceColumnIndex=0 ; sourceColumnIndex<otherNumberColumns ; ++sourceColumnIndex) {
                const std::uint8_t* s = reinterpret_cast<const std::uint8_t*>(other.addressOf(0, sourceColumnIndex));
                std::uint8_t*       d = reinterpret_cast<std::uint8_t*>(resultData->addressOf(0, resultColumnIndex));

                std::memcpy(d, s, otherColumnSizeInBytes);
                std::memset(d + otherColumnSizeInBytes, 0, rightResultResidueInBytes);

                ++resultColumnIndex;
            }
        } else {
            for (unsigned long sourceColumnIndex=0 ; sourceColumnIndex<otherNumberColumns ; ++sourceColumnIndex) {
                const std::uint8_t* s = reinterpret_cast<const std::uint8_t*>(other.addressOf(0, sourceColumnIndex));
                std::uint8_t*       d = reinterpret_cast<std::uint8_t*>(resultData->addressOf(0, resultColumnIndex));

                std::memcpy(d, s, otherColumnSizeInBytes);
                ++resultColumnIndex;
            }
        }

        return resultData;
    }


    MatrixReal::Data* MatrixReal::DenseData::combineLeftToRight(
            const MatrixSparsePrivate<MatrixReal::DenseData::Scalar>& other
        ) const {
        (void) other; // FIXME
        return nullptr; // FIXME
    }


    MatrixReal::Data* MatrixReal::DenseData::combineTopToBottom(
            const MatrixDensePrivate<MatrixReal::DenseData::Scalar>& other
        ) const {
        unsigned long thisNumberRows      = numberRows();
        unsigned long thisNumberColumns   = numberColumns();
        unsigned long otherNumberRows     = other.numberRows();
        unsigned long otherNumberColumns  = other.numberColumns();
        unsigned long resultNumberRows    = thisNumberRows + otherNumberRows;
        unsigned long resultNumberColumns = std::max(thisNumberColumns, otherNumberColumns);

        unsigned long long thisColumnSizeInBytes      = thisNumberRows * sizeof(Scalar);
        unsigned long long otherColumnSizeInBytes     = otherNumberRows * sizeof(Scalar);
        unsigned long long resultColumnSizeInBytes    = thisColumnSizeInBytes + otherColumnSizeInBytes;
        unsigned long long resultColumnSpacingInBytes = columnSizeInBytes(resultNumberRows, sizeof(Scalar));

        DenseData* resultData = createUninitialized(resultNumberRows, resultNumberColumns);

        unsigned sharedColumnsResidueInBytes = static_cast<unsigned>(
            resultColumnSpacingInBytes - resultColumnSizeInBytes
        );

        unsigned long sharedNumberColumns = std::min(thisNumberColumns, otherNumberColumns);

        if (sharedColumnsResidueInBytes > 0) {
            for (unsigned long column=0 ; column<sharedNumberColumns ; ++column) {
                const std::uint8_t* thisColumn   = reinterpret_cast<const std::uint8_t*>(addressOf(0, column));
                const std::uint8_t* otherColumn  = reinterpret_cast<const std::uint8_t*>(other.addressOf(0, column));
                std::uint8_t*       resultColumn = reinterpret_cast<std::uint8_t*>(resultData->addressOf(0, column));

                std::memcpy(resultColumn, thisColumn, thisColumnSizeInBytes);
                std::memcpy(resultColumn + thisColumnSizeInBytes, otherColumn, otherColumnSizeInBytes);
                std::memset(resultColumn + resultColumnSizeInBytes, 0, sharedColumnsResidueInBytes);
            }
        } else {
            for (unsigned long column=0 ; column<sharedNumberColumns ; ++column) {
                const std::uint8_t* thisColumn   = reinterpret_cast<const std::uint8_t*>(addressOf(0, column));
                const std::uint8_t* otherColumn  = reinterpret_cast<const std::uint8_t*>(other.addressOf(0, column));
                std::uint8_t*       resultColumn = reinterpret_cast<std::uint8_t*>(resultData->addressOf(0, column));

                std::memcpy(resultColumn, thisColumn, thisColumnSizeInBytes);
                std::memcpy(resultColumn + thisColumnSizeInBytes, otherColumn, otherColumnSizeInBytes);
            }
        }

        if (thisNumberColumns > otherNumberColumns) {
            unsigned long long residueInBytes = resultColumnSpacingInBytes - thisColumnSizeInBytes;

            for (unsigned long column=sharedNumberColumns ; column<resultNumberColumns ; ++column) {
                const std::uint8_t* thisColumn   = reinterpret_cast<const std::uint8_t*>(addressOf(0, column));
                std::uint8_t*       resultColumn = reinterpret_cast<std::uint8_t*>(resultData->addressOf(0, column));

                std::memcpy(resultColumn, thisColumn, thisColumnSizeInBytes);
                std::memset(resultColumn + thisColumnSizeInBytes, 0, residueInBytes);
            }
        } else if (thisNumberColumns < otherNumberColumns) {
            if (sharedColumnsResidueInBytes > 0) {
                for (unsigned long column=sharedNumberColumns ; column<resultNumberColumns ; ++column) {
                    const std::uint8_t* o  = reinterpret_cast<const std::uint8_t*>(other.addressOf(0, column));
                    std::uint8_t*       r = reinterpret_cast<std::uint8_t*>(resultData->addressOf(0, column));

                    std::memset(r, 0, thisColumnSizeInBytes);
                    std::memcpy(r + thisColumnSizeInBytes, o, otherColumnSizeInBytes);
                    std::memset(r + resultColumnSizeInBytes, 0, sharedColumnsResidueInBytes);
                }
            } else {
                for (unsigned long column=sharedNumberColumns ; column<resultNumberColumns ; ++column) {
                    const std::uint8_t* o  = reinterpret_cast<const std::uint8_t*>(other.addressOf(0, column));
                    std::uint8_t*       r = reinterpret_cast<std::uint8_t*>(resultData->addressOf(0, column));

                    std::memset(r, 0, thisColumnSizeInBytes);
                    std::memcpy(r + thisColumnSizeInBytes, o, otherColumnSizeInBytes);
                }
            }
        }

        return resultData;
    }


    MatrixReal::Data* MatrixReal::DenseData::combineTopToBottom(
            const MatrixSparsePrivate<MatrixReal::DenseData::Scalar>& other
        ) const {
        (void) other; // FIXME
        return nullptr; // FIXME
    }


    MatrixReal::Data* MatrixReal::DenseData::columnReverse() const {
        unsigned long      numberRows        = DenseData::numberRows();
        unsigned long      numberColumns     = DenseData::numberColumns();
        DenseData*         result            = createUninitialized(numberRows, numberColumns);
        unsigned long long columnSizeInBytes = DenseData::columnSizeInBytes(numberRows, sizeof(Scalar));

        for (unsigned sourceColumnIndex=0 ; sourceColumnIndex<numberColumns ; ++sourceColumnIndex) {
            unsigned long destinationColumnIndex = numberColumns - sourceColumnIndex - 1;
            const void*   source                 = addressOf(0, sourceColumnIndex);
            void*         destination            = result->addressOf(0, destinationColumnIndex);
            std::memcpy(destination, source, columnSizeInBytes);
        }

        return result;
    }


    MatrixReal::Data* MatrixReal::DenseData::rowReverse() const {
        unsigned long      numberRows        = DenseData::numberRows();
        unsigned long      numberColumns     = DenseData::numberColumns();
        DenseData*         result            = createUninitialized(numberRows, numberColumns);
        unsigned long long columnSize        = columnSpacingInMemory();

        for (unsigned long sourceRowIndex=0 ; sourceRowIndex<numberRows ; ++sourceRowIndex) {
            unsigned long destinationRowIndex = numberRows - sourceRowIndex - 1;
            const double* source              = reinterpret_cast<const double*>(addressOf(sourceRowIndex, 0));
            double*       destination         = reinterpret_cast<double*>(result->addressOf(destinationRowIndex, 0));

            matrixApi()->blasDoubleCopy(numberColumns, source, columnSize, destination, columnSize);
        }

        if (columnSize > numberRows) {
            unsigned long residueInBytes = (columnSize - numberRows) * sizeof(Scalar);
            for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                void* residueBase = result->addressOf(numberRows, columnIndex);
                std::memset(residueBase, 0, residueInBytes);
            }
        }

        return result;
    }


    MatrixReal::Data* MatrixReal::DenseData::resizeTo(
            unsigned long newNumberRows,
            unsigned long newNumberColumns,
            bool          alwaysReallocate
        ) {
        MatrixReal::Data* result = this;
        if (alwaysReallocate || !resizeInPlace(newNumberRows, newNumberColumns)) {
            result = MatrixDensePrivate<MatrixReal::DenseData::Scalar>::create<DenseData>(
                *this,
                newNumberRows,
                newNumberColumns
            );
        }

        return result;
    }


    bool MatrixReal::DenseData::resizeInPlaceTo(unsigned long newNumberRows, unsigned long newNumberColumns) {
        return MatrixDensePrivate<MatrixReal::DenseData::Scalar>::resizeInPlace(newNumberRows, newNumberColumns);
    }


    MatrixReal::Data* MatrixReal::DenseData::floor() const {
        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        DenseData*         result                         = createUninitialized(numberRows, numberColumns);
        unsigned long long sourceColumnSizeInEntries      = columnSpacingInMemory();
        unsigned long long destinationColumnSizeInEntries = result->columnSpacingInMemory();

        const double* s = data();
        double*       d = result->data();

        if (destinationColumnSizeInEntries > numberRows) {
            unsigned residueInEntries = static_cast<unsigned>(destinationColumnSizeInEntries - numberRows);
            unsigned residueInBytes   = residueInEntries * sizeof(Scalar);

            for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                matrixApi()->doubleFloor(numberRows, s, d);
                std::memset(d + numberRows, 0, residueInBytes);

                s += sourceColumnSizeInEntries;
                d += destinationColumnSizeInEntries;
            }
        } else {
            for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                matrixApi()->doubleFloor(numberRows, s, d);
                s += sourceColumnSizeInEntries;
                d += destinationColumnSizeInEntries;
            }
        }

        return result;
    }


    MatrixReal::Data* MatrixReal::DenseData::ceil() const {
        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        DenseData*         result                         = createUninitialized(numberRows, numberColumns);
        unsigned long long sourceColumnSizeInEntries      = columnSpacingInMemory();
        unsigned long long destinationColumnSizeInEntries = result->columnSpacingInMemory();

        const double* s = data();
        double*       d = result->data();

        if (destinationColumnSizeInEntries > numberRows) {
            unsigned residueInEntries = static_cast<unsigned>(destinationColumnSizeInEntries - numberRows);
            unsigned residueInBytes   = residueInEntries * sizeof(Scalar);

            for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                matrixApi()->doubleCeiling(numberRows, s, d);
                std::memset(d + numberRows, 0, residueInBytes);

                s += sourceColumnSizeInEntries;
                d += destinationColumnSizeInEntries;
            }
        } else {
            for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                matrixApi()->doubleCeiling(numberRows, s, d);
                s += sourceColumnSizeInEntries;
                d += destinationColumnSizeInEntries;
            }
        }

        return result;
    }


    MatrixReal::Data* MatrixReal::DenseData::nint() const {
        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        DenseData*         result                         = createUninitialized(numberRows, numberColumns);
        unsigned long long sourceColumnSizeInEntries      = columnSpacingInMemory();
        unsigned long long destinationColumnSizeInEntries = result->columnSpacingInMemory();

        const double* s = data();
        double*       d = result->data();

        if (destinationColumnSizeInEntries > numberRows) {
            unsigned residueInEntries = static_cast<unsigned>(destinationColumnSizeInEntries - numberRows);
            unsigned residueInBytes   = residueInEntries * sizeof(Scalar);

            for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                matrixApi()->doubleNearestInteger(numberRows, s, d);
                std::memset(d + numberRows, 0, residueInBytes);

                s += sourceColumnSizeInEntries;
                d += destinationColumnSizeInEntries;
            }
        } else {
            for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                matrixApi()->doubleNearestInteger(numberRows, s, d);
                s += sourceColumnSizeInEntries;
                d += destinationColumnSizeInEntries;
            }
        }

        return result;
    }


    MatrixInteger::Data* MatrixReal::DenseData::truncateToInteger() const {
        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        MatrixInteger::DenseData* result = MatrixInteger::DenseData::createUninitialized(numberRows, numberColumns);

        unsigned long long destinationColumnSizeInEntries = result->columnSpacingInMemory();
        unsigned long      sourceColumnSizeInEntries      = columnSpacingInMemory();

        unsigned residueInEntries = static_cast<unsigned>(destinationColumnSizeInEntries - numberRows);

        const double*          s = data();
        MatrixInteger::Scalar* d = result->data();

        for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
            toInteger(numberRows, residueInEntries, s, d);

            s += sourceColumnSizeInEntries;
            d += destinationColumnSizeInEntries;
        }

        return result;
    }


    MatrixInteger::Data* MatrixReal::DenseData::floorToInteger() const {
        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        MatrixInteger::DenseData* result = MatrixInteger::DenseData::createUninitialized(numberRows, numberColumns);

        unsigned long long sourceColumnSizeInEntries      = columnSpacingInMemory();
        unsigned long long destinationColumnSizeInEntries = result->columnSpacingInMemory();

        unsigned residueInEntries = static_cast<unsigned>(destinationColumnSizeInEntries - numberRows);

        Scalar*                buffer = new Scalar[numberRows];
        const double*          s      = data();
        MatrixInteger::Scalar* d      = result->data();

        for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
            matrixApi()->doubleFloor(numberRows, s, buffer);
            toInteger(numberRows, residueInEntries, buffer, d);

            s += sourceColumnSizeInEntries;
            d += destinationColumnSizeInEntries;
        }

        delete[] buffer;

        return result;
    }


    MatrixInteger::Data* MatrixReal::DenseData::ceilToInteger() const {
        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        MatrixInteger::DenseData* result = MatrixInteger::DenseData::createUninitialized(numberRows, numberColumns);

        unsigned long long sourceColumnSizeInEntries      = columnSpacingInMemory();
        unsigned long long destinationColumnSizeInEntries = result->columnSpacingInMemory();

        unsigned residueInEntries = static_cast<unsigned>(destinationColumnSizeInEntries - numberRows);

        Scalar*                buffer = new Scalar[numberRows];
        const double*          s      = data();
        MatrixInteger::Scalar* d      = result->data();

        for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
            matrixApi()->doubleCeiling(numberRows, s, buffer);
            toInteger(numberRows, residueInEntries, buffer, d);

            s += sourceColumnSizeInEntries;
            d += destinationColumnSizeInEntries;
        }

        delete[] buffer;

        return result;
    }


    MatrixInteger::Data* MatrixReal::DenseData::nintToInteger() const {
        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        MatrixInteger::DenseData* result = MatrixInteger::DenseData::createUninitialized(numberRows, numberColumns);

        unsigned long long sourceColumnSizeInEntries      = columnSpacingInMemory();
        unsigned long long destinationColumnSizeInEntries = result->columnSpacingInMemory();

        unsigned residueInEntries = static_cast<unsigned>(destinationColumnSizeInEntries - numberRows);

        Scalar*                buffer = new Scalar[numberRows];
        const double*          s      = data();
        MatrixInteger::Scalar* d      = result->data();

        for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
            matrixApi()->doubleNearestInteger(numberRows, s, buffer);
            toInteger(numberRows, residueInEntries, buffer, d);

            s += sourceColumnSizeInEntries;
            d += destinationColumnSizeInEntries;
        }

        delete[] buffer;

        return result;
    }


    MatrixReal::Scalar MatrixReal::DenseData::determinant() const {
        Scalar result(1);

        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        if (numberRows != numberColumns || numberRows == 0) {
            throw Model::InvalidMatrixDimensions(numberRows, numberColumns);
        } else {
            DenseData*    luMatrix       = static_cast<DenseData*>(clone());
            unsigned long pivotArraySize = std::max(1UL, std::min(numberRows, numberColumns));
            MatInteger*   pivotArray     = new MatInteger[pivotArraySize];

            MatInteger info = matrixApi()->lapackDoublePlu(
                MatMatrixMode::COLUMN_MAJOR,
                numberRows,
                numberColumns,
                luMatrix->data(),
                luMatrix->columnSpacingInMemory(),
                static_cast<MatInteger*>(pivotArray)
            );

            assert(info >= 0);

            if (info > 0) {
                result = Scalar(0);
            } else {
                // TODO: consider using SIMD for larger matrices to calculate the product of the diagonal.
                unsigned long diagnonalSize = std::min(numberRows, numberColumns);
                for (unsigned rowColumnIndex=0 ; rowColumnIndex<diagnonalSize ; ++rowColumnIndex) {
                    Scalar value = luMatrix->at(rowColumnIndex, rowColumnIndex);

                    assert(!std::isnan(value));
                    assert(!std::isinf(value));

                    result *= value;
                }

                // The pivot array is not a permutation vector, rather it represents the pivoting steps that were
                // performed.  For this reason, the pivot array is not a symmetrical structure.

                for (unsigned long i=0 ; i<pivotArraySize ; ++i) {
                    if (pivotArray[i] != static_cast<MatInteger>(i+1)) {
                        result *= -1.0;
                    }
                }
            }

            delete[] pivotArray;
            destroy(luMatrix);
        }

        return result;
    }


    MatrixReal::Data* MatrixReal::DenseData::inverse() const {
        Data* result = nullptr;

        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        if (numberColumns == 0 || numberRows == 0 || numberRows != numberColumns) {
            throw Model::InvalidMatrixDimensions(numberRows, numberColumns);
        } else {
            DenseData*         di                    = static_cast<DenseData*>(clone());
            MatInteger*        pivotOperationsByRow  = new MatInteger[numberRows];
            unsigned long long columnSpacingInMemory = di->columnSpacingInMemory();
            double*            data                  = reinterpret_cast<double*>(di->data());

            MatInteger info = matrixApi()->lapackDoublePlu(
                MatMatrixMode::COLUMN_MAJOR,
                numberRows,
                numberColumns,
                data,
                columnSpacingInMemory,
                pivotOperationsByRow
            );

            if (info == 0) {
                info = matrixApi()->lapackDoubleLuInverse(
                    MatMatrixMode::COLUMN_MAJOR,
                    numberRows,
                    data,
                    columnSpacingInMemory,
                    pivotOperationsByRow
                );
            }

            if (info != 0) {
                destroy(di);
                throw Model::MatrixIsSingular();
            } else {
                result = di;
            }
        }

        return result;
    }


    bool MatrixReal::DenseData::plu(MatrixReal::Data*& p, MatrixReal::Data*& l, MatrixReal::Data*& u) const {
        bool notSingular = false;

        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        if (numberColumns  == 0 || numberRows == 0) {
            p = nullptr;
            l = nullptr;
            u = nullptr;

            throw Model::InvalidMatrixDimensions(numberRows, numberColumns);
        } else {
            DenseData*    du                           = static_cast<DenseData*>(clone());
            unsigned long maximumNumberPivotOperations = std::max(1UL, std::min(numberRows, numberColumns));
            MatInteger*   pivotOperationsByRow         = new MatInteger[maximumNumberPivotOperations];

            MatInteger info = matrixApi()->lapackDoublePlu(
                MatMatrixMode::COLUMN_MAJOR,
                numberRows,
                numberColumns,
                du->data(),
                du->columnSpacingInMemory(),
                pivotOperationsByRow
            );

            notSingular = (info == 0);

            DenseData* dl = static_cast<DenseData*>(du->clone());

            // Fix-up l and u matrices.
            dl->toLowerTriangular();
            du->toUpperTriangular();

            unsigned long numberIndexValues = std::min(numberRows, numberColumns);
            for (unsigned long index=0 ; index<numberIndexValues ; ++index) {
                dl->at(index, index) = Scalar(1);
            }

            DenseData* dp = generatePivotMatrix(pivotOperationsByRow, numberRows, maximumNumberPivotOperations);
            delete[] pivotOperationsByRow;

            l = dl;
            u = du;
            p = dp;
        }

        return notSingular;
    }


    bool MatrixReal::DenseData::svd(MatrixReal::Data*& u, MatrixReal::Data*& s, MatrixReal::Data*& vh) const {
        bool          success       = false;
        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        if (numberColumns  == 0 || numberRows == 0) {
            u  = nullptr;
            s  = nullptr;
            vh = nullptr;

            throw Model::InvalidMatrixDimensions(numberRows, numberColumns);
        } else {
            DenseData*         a                          = static_cast<DenseData*>(clone());
            unsigned long long numberSingularValues       = std::min(numberRows, numberColumns);
            double*            singularValues             = new double[numberSingularValues];
            double*            unconvergedSuperDiagnonals = new double[numberSingularValues];

            DenseData* du  = create(numberRows, numberRows);
            DenseData* dvh = create(numberColumns, numberColumns);

            MatInteger info = matrixApi()->lapackDoubleSvd(
                MatMatrixMode::COLUMN_MAJOR,
                numberRows,
                numberColumns,
                reinterpret_cast<double*>(a->data()),
                a->columnSpacingInMemory(),
                singularValues,
                reinterpret_cast<double*>(du->data()),
                du->columnSpacingInMemory(),
                reinterpret_cast<double*>(dvh->data()),
                dvh->columnSpacingInMemory(),
                unconvergedSuperDiagnonals
            );

            destroy(a);
            assert(info >= 0);

            delete[] unconvergedSuperDiagnonals;
            u  = du;
            s  = create(numberRows, numberColumns);
            vh = dvh;
            if (info == 0) {
                for (unsigned long index=0 ; index<numberSingularValues ; ++index) {
                    s->at(index, index) = singularValues[index];
                }

                success = true;
            } else {
                success = false;
            }

            delete[] singularValues;
        }

        return success;
    }


    bool MatrixReal::DenseData::qr(MatrixReal::Data*& q, MatrixReal::Data*& r) const {
        bool          success;

        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        if (numberColumns  == 0 || numberRows == 0 || numberRows < numberColumns) {
            q  = nullptr;
            r  = nullptr;

            throw Model::InvalidMatrixDimensions(numberRows, numberColumns);
        } else {
            DenseData* dq;
            DenseData* dr;

            if (numberRows > numberColumns) {
                dq = MatrixDensePrivate<MatrixReal::DenseData::Scalar>::create<DenseData>(
                    *this,
                    numberRows,
                    numberRows
                );
            } else {
                dq = static_cast<DenseData*>(clone());
            }
            unsigned long long numberQReflectors = numberColumns;
            double*            tau               = new double[numberQReflectors];

            MatInteger info = matrixApi()->lapackDoubleQrFactorization(
                MatMatrixMode::COLUMN_MAJOR,
                numberRows,
                numberColumns,
                dq->data(),
                dq->columnSpacingInMemory(),
                tau
            );

            if (info == 0) {
                if (numberRows > numberColumns) {
                    dr = MatrixDensePrivate<Scalar>::create<DenseData>(*dq, numberRows, numberColumns);
                } else {
                    dr = static_cast<DenseData*>(dq->clone());
                }

                info = matrixApi()->lapackDoubleGenerateQFromQrMatrix(
                    MatMatrixMode::COLUMN_MAJOR,
                    numberRows,
                    numberRows,
                    numberQReflectors,
                    dq->data(),
                    dq->columnSpacingInMemory(),
                    tau
                );

                delete[] tau;

                if (info == 0) {
                    dr->toUpperTriangular();

                    // Fix-up size of Q matrix if not numberRows, numberRows
                    if (dq->numberColumns() != numberRows) {
                        bool resizeSuccessful = dq->resizeInPlaceTo(numberRows, numberRows);
                        (void) resizeSuccessful;
                        assert(resizeSuccessful);
                    }

                    success = true;
                } else {
                    destroy(dq);
                    destroy(dr);

                    dq = MatrixDensePrivate<Scalar>::create<DenseData>(numberRows, numberRows);
                    dr = MatrixDensePrivate<Scalar>::create<DenseData>(numberRows, numberColumns);

                    success = false;
                }
            } else {
                delete[] tau;
                destroy(dq);

                dq = MatrixDensePrivate<Scalar>::create<DenseData>(numberRows, numberRows);
                dr = MatrixDensePrivate<Scalar>::create<DenseData>(numberRows, numberColumns);

                success = false;
            }

            q = dq;
            r = dr;
        }

        return success;
    }


    MatrixReal::Data* MatrixReal::DenseData::cholesky() const {
        Data* result;

        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        if (numberColumns  == 0 || numberRows == 0 || numberRows != numberColumns) {
            result  = nullptr;
            throw Model::InvalidMatrixDimensions(numberRows, numberColumns);
        } else {
            DenseData* dl   = static_cast<DenseData*>(clone());
            MatInteger info = matrixApi()->lapackDoubleCholesky(
                MatMatrixMode::COLUMN_MAJOR,
                MatCholeskyType::LOWER,
                numberRows,
                reinterpret_cast<double*>(dl->data()),
                dl->columnSpacingInMemory()
            );

            if (info == 0) {
                dl->toLowerTriangular();
            } else {
                destroy(dl);
                internalTriggerInvalidParameterValueError();
                dl = MatrixDensePrivate<Scalar>::create<DenseData>(0, 0);
            }

            result = dl;
        }

        return result;
    }


    MatrixReal::Data* MatrixReal::DenseData::upperCholesky() const {
        Data* result;

        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        if (numberColumns  == 0 || numberRows == 0 || numberRows != numberColumns) {
            result = nullptr;
            throw Model::InvalidMatrixDimensions(numberRows, numberColumns);
        } else {
            DenseData* du   = static_cast<DenseData*>(clone());
            MatInteger info = matrixApi()->lapackDoubleCholesky(
                MatMatrixMode::COLUMN_MAJOR,
                MatCholeskyType::UPPER,
                numberRows,
                reinterpret_cast<double*>(du->data()),
                du->columnSpacingInMemory()
            );

            if (info == 0) {
                du->toUpperTriangular();
            } else {
                destroy(du);
                internalTriggerInvalidParameterValueError();
                du = MatrixDensePrivate<Scalar>::create<DenseData>(0, 0);
            }

            result = du;
        }

        return result;
    }


    void MatrixReal::DenseData::hessenberg(MatrixReal::Data*& qData, MatrixReal::Data*& hData) const {
        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        if (numberColumns  == 0 || numberRows == 0 || numberRows != numberColumns) {
            qData = nullptr;
            hData = nullptr;
            throw Model::InvalidMatrixDimensions(numberRows, numberColumns);
        } else {
            DenseData*    h       = POLYMORPHIC_CAST<DenseData*>(clone());
            unsigned long tauSize = std::max(1UL, numberRows - 1);
            double*       tau     = new double[tauSize];

            MatInteger info = matrixApi()->lapackDoubleUpperHessenberg(
                MatMatrixMode::COLUMN_MAJOR,
                numberRows,
                reinterpret_cast<double*>(h->data()),
                h->columnSpacingInMemory(),
                tau
            );

            assert(info == 0);

            DenseData* q = POLYMORPHIC_CAST<DenseData*>(h->clone());

            info = matrixApi()->lapackDoubleUpperHessenbergQMatrix(
                MatMatrixMode::COLUMN_MAJOR,
                numberRows,
                reinterpret_cast<double*>(q->data()),
                q->columnSpacingInMemory(),
                tau
            );

            assert(info == 0);

            delete[] tau;

            // Zero out the lower portion of the Hessenberg matrix (contains the elementary reflectors of Q).
            if (numberColumns > 2) {
                unsigned long lastZeroColumn = numberColumns-3;
                for (unsigned column=0 ; column<=lastZeroColumn ; ++column) {
                    std::uint8_t* zeroStart = reinterpret_cast<std::uint8_t*>(h->addressOf(column + 2, column));
                    std::uint8_t* zeroEnd   = reinterpret_cast<std::uint8_t*>(h->addressOf(numberRows, column));

                    std::memset(zeroStart, 0, zeroEnd - zeroStart);
                }
            }

            qData = q;
            hData = h;
        }
    }


    MatrixReal::Data* MatrixReal::DenseData::dct() const {
        return POLYMORPHIC_CAST<Data*>(dctHelper(matrixApi()->realDctType2));
    }


    MatrixReal::Data* MatrixReal::DenseData::idct() const {
        return POLYMORPHIC_CAST<Data*>(dctHelper(matrixApi()->realDctType3));
    }


    MatrixComplex::Data* MatrixReal::DenseData::hilbertTransform() const {
        MatrixComplex::DenseData* result;

        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        if (numberRows == 1 || numberColumns == 1) {
            result = MatrixComplex::DenseData::createUninitialized(numberRows, numberColumns);

            const Scalar*          inputData  = data();
            MatrixComplex::Scalar* outputData = result->data();

            MatInteger status = matrixApi()->complexHilbertTransform(
                numberRows * numberColumns,
                inputData,
                reinterpret_cast<MatComplex*>(outputData)
            );

            if (status != 0) {
                throw Model::InsufficientMemory();
            }
        } else {
            throw Model::InvalidMatrixDimensions(numberRows, numberColumns);
        }

        return result;
    }


    Real MatrixReal::DenseData::pNorm(Integer p) const {
        Real          result;

        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        if (p > 0) {
            if (p == 2) {
                Real squaredResult = 0;
                for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                    const Scalar* v = reinterpret_cast<const Scalar*>(addressOf(0, columnIndex));
                    for (unsigned long rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                        Scalar value = *v;
                        squaredResult += value * value;
                        ++v;
                    }
                }

                result = std::sqrt(squaredResult);
            } else {
                Real sum = 0;
                for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                    const Scalar* v = reinterpret_cast<const Scalar*>(addressOf(0, columnIndex));
                    for (unsigned long rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                        sum += pow(M::abs(*v), p);
                        ++v;
                    }
                }

                result = nroot(sum, p).real();
            }
        } else {
            result = NaN;
        }

        return result;
    }


    Real MatrixReal::DenseData::oneNorm() const {
        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        Real columnMaximum = 0;
        for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
            Real columnSum = 0;
            const Scalar* v = reinterpret_cast<const Scalar*>(addressOf(0, columnIndex));
            for (unsigned long rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                columnSum += std::abs(*v);
                ++v;
            }

            if (columnSum > columnMaximum) {
                columnMaximum = columnSum;
            }
        }

        return columnMaximum;
    }


    Real MatrixReal::DenseData::infinityNorm() const {
        unsigned long numberRows                    = DenseData::numberRows();
        unsigned long numberColumns                 = DenseData::numberColumns();
        unsigned long long columnSizeInCoefficients = columnSpacingInMemory();

        Real rowMaximum = 0;
        for (unsigned long rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
            Real rowSum = 0;
            const Scalar* v = reinterpret_cast<const Scalar*>(addressOf(rowIndex, 0));
            for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                rowSum += abs(*v);
                v += columnSizeInCoefficients;
            }

            if (rowSum > rowMaximum) {
                rowMaximum = rowSum;
            }
        }

        return rowMaximum;
    }



    bool MatrixReal::DenseData::equilibrate(MatrixReal::Data*& row, MatrixReal::Data*& column) const {
        bool          success       = false;
        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        if (numberColumns  == 0 || numberRows == 0 || numberRows != numberColumns) {
            row    = nullptr;
            column = nullptr;

            throw Model::InvalidMatrixDimensions(numberRows, numberColumns);
        } else {
            double* rowScaleFactors    = new double[numberRows];
            double* columnScaleFactors = new double[numberColumns];
            double  rowRatio;
            double  columnRatio;
            double  aMax;

            MatInteger info = matrixApi()->lapackDoubleEquilibrate(
                MatMatrixMode::COLUMN_MAJOR,
                static_cast<MatInteger>(numberRows),
                static_cast<MatInteger>(numberColumns),
                reinterpret_cast<const double*>(data()),
                static_cast<MatInteger>(columnSpacingInMemory()),
                rowScaleFactors,
                columnScaleFactors,
                &rowRatio,
                &columnRatio,
                &aMax
            );

            assert(info >= 0);

            DenseData* dRow = DenseData::create(numberRows, numberRows);
            DenseData* dColumn = DenseData::create(numberColumns, numberColumns);

            if (info == 0) {
                for (unsigned long rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                    dRow->at(rowIndex, rowIndex) = rowScaleFactors[rowIndex];
                }

                for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                    dColumn->at(columnIndex, columnIndex) = columnScaleFactors[columnIndex];
                }

                delete[] rowScaleFactors;
                delete[] columnScaleFactors;

                success = true;
            } else {
                success = false;
            }

            row    = dRow;
            column = dColumn;
        }

        return success;
    }


    MatrixReal::Data* MatrixReal::DenseData::solve(const MatrixReal::DenseData* y) const {
        Data*         result         = nullptr;

        unsigned long aNumberRows    = DenseData::numberRows();
        unsigned long aNumberColumns = DenseData::numberColumns();
        unsigned long yNumberRows    = y->numberRows();
        unsigned long yNumberColumns = y->numberColumns();

        if (aNumberColumns == 0 || aNumberRows == 0 || aNumberRows != aNumberColumns) {
            throw Model::InvalidMatrixDimensions(aNumberRows, aNumberColumns);
        } else if (yNumberColumns == 0 || yNumberRows == 0) {
            throw Model::InvalidMatrixDimensions(yNumberRows, yNumberColumns);
        } else if (aNumberRows != yNumberRows) {
            throw Model::IncompatibleMatrixDimensions(aNumberRows, aNumberColumns, yNumberRows, yNumberColumns);
        } else {
            double* rowScaleFactors    = new double[aNumberRows];
            double* columnScaleFactors = new double[aNumberColumns];
            double  rowRatio;
            double  columnRatio;
            double  aMax;

            MatInteger info = matrixApi()->lapackDoubleEquilibratePowerOf2(
                MatMatrixMode::COLUMN_MAJOR,
                aNumberRows,
                aNumberColumns,
                reinterpret_cast<const double*>(data()),
                columnSpacingInMemory(),
                rowScaleFactors,
                columnScaleFactors,
                &rowRatio,
                &columnRatio,
                &aMax
            );

            assert(info >= 0);

            if (info == 0) {
                DenseData*       a;
                DenseData*       yScaled;
                const DenseData* yValues;

                if (rowRatio < 0.1 || aMax > bigLimit() || aMax < smallLimit()) {
                    a       = multiplyRows(*this, rowScaleFactors);
                    yScaled = multiplyRows(*y, rowScaleFactors);
                    yValues = yScaled;
                } else {
                    a       = POLYMORPHIC_CAST<DenseData*>(clone());
                    yScaled = nullptr;
                    yValues = y;
                }

                MatInteger* pivotOperationArray = new MatInteger[aNumberRows];
                DenseData*  x                   = createUninitialized(aNumberRows, yNumberColumns);
                MatInteger  iterationDetails;
                MatInteger info = matrixApi()->lapackDoubleSolve(
                    MatMatrixMode::COLUMN_MAJOR,
                    aNumberRows,
                    yNumberColumns,
                    reinterpret_cast<double*>(a->data()),
                    a->columnSpacingInMemory(),
                    pivotOperationArray,
                    reinterpret_cast<const double*>(yValues->data()),
                    yValues->columnSpacingInMemory(),
                    reinterpret_cast<double*>(x->data()),
                    x->columnSpacingInMemory(),
                    &iterationDetails
                );

                delete[] pivotOperationArray;

                if (info == 0) {
                    result = x;
                } else {
                    DenseData::destroy(x);
                    result = create(0, 0);
                }

                if (yScaled != nullptr) {
                    DenseData::destroy(yScaled);
                }
            } else {
                result = create(0, 0);
            }

            delete[] rowScaleFactors;
            delete[] columnScaleFactors;
        }

        return result;
    }


    MatrixReal::Data* MatrixReal::DenseData::solve(const MatrixReal::SparseData* y) const {
        (void) y; // FIXME
        return nullptr; // FIXME
    }


    MatrixReal::Data* MatrixReal::DenseData::leastSquares(const MatrixReal::DenseData* b) const {
        Data*         result         = nullptr;

        unsigned long aNumberRows    = DenseData::numberRows();
        unsigned long aNumberColumns = DenseData::numberColumns();
        unsigned long bNumberRows    = b->numberRows();
        unsigned long bNumberColumns = b->numberColumns();

        if (aNumberColumns == 0 || aNumberRows == 0) {
            throw Model::InvalidMatrixDimensions(aNumberRows, aNumberColumns);
        } else if (bNumberColumns == 0 || bNumberRows == 0) {
            throw Model::InvalidMatrixDimensions(bNumberRows, bNumberColumns);
        } else if (bNumberRows != aNumberRows) {
            throw Model::IncompatibleMatrixDimensions(aNumberRows, aNumberColumns, bNumberRows, bNumberColumns);
        } else {
            unsigned long xNumberRows = std::max(aNumberRows, aNumberColumns);
            DenseData*    a           = POLYMORPHIC_CAST<DenseData*>(clone());
            DenseData*    x           = POLYMORPHIC_CAST<DenseData*>(
                const_cast<DenseData*>(POLYMORPHIC_CAST<const DenseData*>(b))
                    ->resizeTo(xNumberRows, bNumberColumns, true)
            );

            MatInteger info = matrixApi()->lapackDoubleLeastSquaresSolve(
                MatMatrixMode::COLUMN_MAJOR,
                MatOperation::NO_OPERATION,
                aNumberRows,
                aNumberColumns,
                bNumberColumns,
                reinterpret_cast<double*>(a->data()),
                a->columnSpacingInMemory(),
                reinterpret_cast<double*>(x->data()),
                x->columnSpacingInMemory()
            );

            assert(info >= 0);

            DenseData::destroy(a);

            if (info == 0) {
                if (x->numberRows() != aNumberColumns) {
                    result = x->resizeTo(aNumberColumns, bNumberColumns);
                    if (result != x) {
                        DenseData::destroy(x);
                    }
                } else {
                    result = x;
                }
            } else {
                DenseData::destroy(x);
                result = create(0, 0);
            }
        }

        return result;
    }


    MatrixReal::Data* MatrixReal::DenseData::leastSquares(const MatrixReal::SparseData* b) const {
        (void) b; // FIXME
        return nullptr; // FIXME
    }


    MatrixReal::Data* MatrixReal::DenseData::hadamard(
            const MatrixDensePrivate<MatrixReal::DenseData::Scalar>& other
        ) const {
        DenseData*    result        = nullptr;

        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        if (other.numberRows() != numberRows || other.numberColumns() != numberColumns) {
            throw Model::IncompatibleMatrixDimensions(
                numberRows,
                numberColumns,
                other.numberRows(),
                other.numberColumns()
            );
        } else {
            unsigned long long columnSizeInEntries = columnSpacingInMemory();

            result = createUninitialized(numberRows, numberColumns);
            unsigned long long columnSizeInBytes     = result->columnSizeInBytes(numberRows, sizeof(Scalar));
            unsigned long long columnDataSizeInBytes = numberRows * sizeof(Scalar);
            unsigned long long residueSizeInBytes    = columnSizeInBytes - columnDataSizeInBytes;

            if (residueSizeInBytes > 0) {
                for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                    const Scalar* a = reinterpret_cast<const Scalar*>(addressOf(0, columnIndex));
                    const Scalar* b = reinterpret_cast<const Scalar*>(other.addressOf(0, columnIndex));
                    Scalar*       y = reinterpret_cast<Scalar*>(result->addressOf(0, columnIndex));

                    matrixApi()->doubleDotProduct(columnSizeInEntries, a, b, y);
                    std::memset(reinterpret_cast<std::uint8_t*>(y) + columnDataSizeInBytes, 0, residueSizeInBytes);
                }
            } else {
                for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                    const Scalar* a = reinterpret_cast<const Scalar*>(addressOf(0, columnIndex));
                    const Scalar* b = reinterpret_cast<const Scalar*>(other.addressOf(0, columnIndex));
                    Scalar*       y = reinterpret_cast<Scalar*>(result->addressOf(0, columnIndex));

                    matrixApi()->doubleDotProduct(columnSizeInEntries, a, b, y);
                }
            }
        }

        return result;
    }


    MatrixReal::Data* MatrixReal::DenseData::hadamard(
            const MatrixSparsePrivate<MatrixReal::DenseData::Scalar>& other
        ) const {
        (void) other; // FIXME
        return nullptr; // FIXME
    }


    MatrixReal::Data* MatrixReal::DenseData::kronecker(
            const MatrixDensePrivate<MatrixReal::DenseData::Scalar>& other,
            bool                                                     otherTranspose,
            MatrixReal::DenseData::Scalar                            scalarMultiplier
        ) const {
        unsigned long multiplierNumberRows        = numberRows();
        unsigned long multiplierNumberColumns     = numberColumns();
        unsigned long multiplicandInNumberRows    = other.numberRows();
        unsigned long multiplicandInNumberColumns = other.numberColumns();

        MatOperation  multiplicandTransformType;
        unsigned long multiplicandOutNumberRows;
        unsigned long multiplicandOutNumberColumns;

        if (otherTranspose) {
            multiplicandTransformType    = MatOperation::TRANSPOSE;
            multiplicandOutNumberRows    = multiplicandInNumberColumns;
            multiplicandOutNumberColumns = multiplicandInNumberRows;
        } else {
            multiplicandTransformType    = MatOperation::NO_OPERATION;
            multiplicandOutNumberRows    = multiplicandInNumberRows;
            multiplicandOutNumberColumns = multiplicandInNumberColumns;
        }

        unsigned long productNumberRows    = multiplierNumberRows * multiplicandOutNumberRows;
        unsigned long productNumberColumns = multiplierNumberColumns * multiplicandOutNumberColumns;

        DenseData*    result           = createUninitialized(productNumberRows, productNumberColumns);
        const Scalar* multiplicandData = other.data();

        unsigned long long multipicandColumnSizeInEntries = other.columnSpacingInMemory();
        unsigned long long productColumnSizeInEntries     = result->columnSpacingInMemory();

        for (unsigned long multiplierColumn=0 ; multiplierColumn<multiplierNumberColumns ; ++multiplierColumn) {
            unsigned long productLeftColumn = multiplierColumn * multiplicandOutNumberColumns;
            for (unsigned long multiplierRow=0 ; multiplierRow<multiplierNumberRows ; ++multiplierRow) {
                unsigned long  productTopRow         = multiplierRow * multiplicandOutNumberRows;
                Scalar         multiplierCoefficient = at(multiplierRow, multiplierColumn) * scalarMultiplier;

                Scalar* resultData = reinterpret_cast<Scalar*>(result->addressOf(productTopRow, productLeftColumn));

                matrixApi()->doubleScaleCopy(
                    MatMatrixMode::COLUMN_MAJOR,
                    multiplicandTransformType,
                    multiplicandInNumberRows,
                    multiplicandInNumberColumns,
                    multiplierCoefficient,
                    multiplicandData,
                    multipicandColumnSizeInEntries,
                    resultData,
                    productColumnSizeInEntries
                );
            }
        }

        unsigned long residueSizeInEntries = productColumnSizeInEntries - productNumberRows;
        if (residueSizeInEntries > 0) {
            unsigned long long residueSizeInBytes = residueSizeInEntries * sizeof(Scalar);
            for (unsigned long columnIndex=0 ; columnIndex<productNumberColumns ; ++columnIndex) {
                std::uint8_t* zeroBase = static_cast<std::uint8_t*>(result->addressOf(productNumberRows, columnIndex));
                std::memset(zeroBase, 0, residueSizeInBytes);
            }
        }

        return result;
    }


    MatrixReal::Data* MatrixReal::DenseData::kronecker(
            const MatrixSparsePrivate<MatrixReal::DenseData::Scalar>& other,
            bool                                                      otherTranspose,
            MatrixReal::DenseData::Scalar                             scalarMultiplier
        ) const {
        (void) other; // FIXME
        (void) otherTranspose; // FIXME
        (void) scalarMultiplier; // FIXME
        return nullptr; // FIXME
    }


    static_assert(sizeof(std::uint64_t) == sizeof(MatrixReal::Scalar), "Double is not 64-bits wide.");

    MatrixReal::DenseData* MatrixReal::DenseData::randomInclusive(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns
        ) {
        DenseData*         result                   = createUninitialized(numberRows, numberColumns);
        Scalar*            d                        = result->data();
        unsigned long long columnSpacing            = result->columnSpacingInMemory();
        unsigned long long matrixSizeInCoefficients = columnSpacing * numberColumns;
        unsigned long long residueEntriesToClear    = columnSpacing - numberRows;

        pt.fillArray(d, matrixSizeInCoefficients * sizeof(Scalar) / 8);

        if (residueEntriesToClear > 0) {
            for (Integer columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                matrixApi()->integerToFloatInclusive(numberRows, d);
                d += numberRows;
                memset(d, 0, residueEntriesToClear * sizeof(Scalar));
                d += residueEntriesToClear;
            }
        } else {
            matrixApi()->integerToFloatInclusive(numberRows * numberColumns, d);
        }

        return result;
    }


    MatrixReal::DenseData* MatrixReal::DenseData::randomInclusiveExclusive(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns
        ) {
        DenseData*         result                   = createUninitialized(numberRows, numberColumns);
        Scalar*            d                        = result->data();
        unsigned long long columnSpacing            = result->columnSpacingInMemory();
        unsigned long long matrixSizeInCoefficients = columnSpacing * numberColumns;
        unsigned long long residueEntriesToClear    = columnSpacing - numberRows;

        pt.fillArray(d, matrixSizeInCoefficients * sizeof(Scalar) / 8);

        if (residueEntriesToClear > 0) {
            for (Integer columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                matrixApi()->integerToFloatInclusiveExclusive(numberRows, d);
                d += numberRows;
                memset(d, 0, residueEntriesToClear * sizeof(Scalar));
                d += residueEntriesToClear;
            }
        } else {
            matrixApi()->integerToFloatInclusiveExclusive(numberRows * numberColumns, d);
        }

        return result;
    }


    MatrixReal::DenseData* MatrixReal::DenseData::randomExclusiveInclusive(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns
        ) {
        DenseData*         result                   = createUninitialized(numberRows, numberColumns);
        Scalar*            d                        = result->data();
        unsigned long long columnSpacing            = result->columnSpacingInMemory();
        unsigned long long matrixSizeInCoefficients = columnSpacing * numberColumns;
        unsigned long long residueEntriesToClear    = columnSpacing - numberRows;

        pt.fillArray(d, matrixSizeInCoefficients * sizeof(Scalar) / 8);

        if (residueEntriesToClear > 0) {
            for (Integer columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                matrixApi()->integerToFloatExclusiveInclusive(numberRows, d);
                d += numberRows;
                memset(d, 0, residueEntriesToClear * sizeof(Scalar));
                d += residueEntriesToClear;
            }
        } else {
            matrixApi()->integerToFloatExclusiveInclusive(numberRows * numberColumns, d);
        }

        return result;
    }


    MatrixReal::DenseData* MatrixReal::DenseData::randomExclusive(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns
        ) {
        DenseData*         result                   = createUninitialized(numberRows, numberColumns);
        Scalar*            d                        = result->data();
        unsigned long long columnSpacing            = result->columnSpacingInMemory();
        unsigned long long matrixSizeInCoefficients = columnSpacing * numberColumns;
        unsigned long long residueEntriesToClear    = columnSpacing - numberRows;

        pt.fillArray(d, matrixSizeInCoefficients * sizeof(Scalar) / 8);

        if (residueEntriesToClear > 0) {
            for (Integer columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                matrixApi()->integerToFloatExclusive(numberRows, d);
                d += numberRows;
                memset(d, 0, residueEntriesToClear * sizeof(Scalar));
                d += residueEntriesToClear;
            }
        } else {
            matrixApi()->integerToFloatExclusive(numberRows * numberColumns, d);
        }

        return result;
    }


    MatrixReal::DenseData* MatrixReal::DenseData::randomNormal(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns
        ) {
        DenseData*         result                = randomNormalEngine(pt, numberRows, numberColumns);
        Scalar*            data                  = result->data();
        unsigned long long columnSpacing         = result->columnSpacingInMemory();
        unsigned long long residueEntriesToClear = columnSpacing - numberRows;

        if (residueEntriesToClear > 0) {
            Scalar* columnStart = data + numberRows;
            for (Integer columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                std::memset(columnStart, 0, sizeof(Scalar) * residueEntriesToClear);
                columnStart += columnSpacing;
            }
        }

        return result;
    }


    MatrixReal::DenseData* MatrixReal::DenseData::randomNormal(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns,
            Real       mean,
            Real       sigma
        ) {
        DenseData*         result                   = randomNormalEngine(pt, numberRows, numberColumns);
        Scalar*            data                     = result->data();
        unsigned long long columnSpacing            = result->columnSpacingInMemory();
        unsigned long long residueEntriesToClear    = columnSpacing - numberRows;
        unsigned long long matrixSizeInCoefficients = columnSpacing * numberColumns - residueEntriesToClear;

        matrixApi()->doubleVectorScalarMultiplyAdd(matrixSizeInCoefficients, data, sigma, mean, data);

        if (residueEntriesToClear > 0) {
            Scalar* columnStart = data + numberRows;
            for (Integer columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                std::memset(columnStart, 0, sizeof(Scalar) * residueEntriesToClear);
                columnStart += columnSpacing;
            }
        }

        return result;
    }


    MatrixReal::DenseData* MatrixReal::DenseData::randomWeibull(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns,
            Real       scale,
            Real       shape,
            Real       delay
        ) {
        DenseData*         result                   = randomExclusiveInclusive(pt, numberRows, numberColumns);
        Scalar*            data                     = result->data();
        unsigned long long columnSpacing            = result->columnSpacingInMemory();
        unsigned long long residueEntriesToClear    = columnSpacing - numberRows;
        unsigned long long matrixSizeInCoefficients = columnSpacing * numberColumns - residueEntriesToClear;

        if (residueEntriesToClear == 0) {
            matrixApi()->doubleLog(matrixSizeInCoefficients, data, data);
        } else {
            Scalar* columnBase = data;
            for (unsigned long long ci=0 ; ci<static_cast<unsigned long long>(numberColumns) ; ++ci) {
                matrixApi()->doubleLog(numberRows, columnBase, columnBase);
                columnBase += columnSpacing;
            }
        }

        Scalar* p            = data;
        Scalar* end          = data + matrixSizeInCoefficients;
        Scalar  oneOverShape = 1.0 / shape;
        while (p != end) {
            *p = std::pow(-(*p), oneOverShape);
            ++p;
        }

        if (delay == 0 || residueEntriesToClear == 0) {
            matrixApi()->doubleVectorScalarMultiplyAdd(
                matrixSizeInCoefficients,
                data,
                scale,
                delay,
                data
            );
        } else {
            Scalar* columnBase = data;
            for (unsigned long long ci=0 ; ci<static_cast<unsigned long long>(numberColumns) ; ++ci) {
                matrixApi()->doubleVectorScalarMultiplyAdd(numberRows, columnBase, scale, delay, columnBase);
                columnBase += columnSpacing;
            }
        }

        return result;
    }


    MatrixReal::DenseData* MatrixReal::DenseData::randomExponential(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns,
            Real       rate
        ) {
        DenseData*         result                   = randomExclusiveInclusive(pt, numberRows, numberColumns);
        Scalar*            data                     = result->data();
        unsigned long long columnSpacing            = result->columnSpacingInMemory();
        unsigned long long residueEntriesToClear    = columnSpacing - numberRows;
        unsigned long long matrixSizeInCoefficients = columnSpacing * numberColumns - residueEntriesToClear;

        if (residueEntriesToClear == 0) {
            matrixApi()->doubleLog(matrixSizeInCoefficients, data, data);
        } else {
            Scalar* columnBase = data;
            for (unsigned long long ci=0 ; ci<static_cast<unsigned long long>(numberColumns) ; ++ci) {
                matrixApi()->doubleLog(numberRows, columnBase, columnBase);
                columnBase += columnSpacing;
            }
        }

        matrixApi()->doubleVectorScalarMultiplyAdd(
            matrixSizeInCoefficients,
            data,
            -1.0 / rate,
            0,
            data
        );

        return result;
    }


    MatrixReal::DenseData* MatrixReal::DenseData::randomGamma(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns,
            Real       k,
            Real       s
        ) {
        DenseData*         result                   = createUninitialized(numberRows, numberColumns);
        Scalar*            data                     = result->data();
        unsigned long long columnSpacing            = result->columnSpacingInMemory();
        unsigned long long residueEntriesToClear    = columnSpacing - numberRows;
        unsigned long long matrixSizeInCoefficients = columnSpacing * numberColumns - residueEntriesToClear;

        if (residueEntriesToClear > 0) {
            Scalar* p = data;
            for (unsigned long long ci=0 ; ci<static_cast<unsigned long long>(numberColumns) ; ++ci) {
                Scalar* columnEnd = p + numberRows;
                while (p != columnEnd) {
                    *p = pt.randomGamma(k, s);
                    ++p;
                }

                std::memset(p, 0, residueEntriesToClear * sizeof(Scalar));
                p += residueEntriesToClear;
            }
        } else {
            Scalar* p   = data;
            Scalar* end = data + matrixSizeInCoefficients;
            while (p != end) {
                *p = pt.randomGamma(k, s);
                ++p;
            }
        }

        return result;
    }


    MatrixReal::DenseData* MatrixReal::DenseData::randomRayleigh(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns,
            Real       scale
        ) {
        DenseData*         result                   = randomExclusive(pt, numberRows, numberColumns);
        Scalar*            data                     = result->data();
        unsigned long long columnSpacing            = result->columnSpacingInMemory();
        unsigned long long residueEntriesToClear    = columnSpacing - numberRows;
        unsigned long long matrixSizeInCoefficients = columnSpacing * numberColumns - residueEntriesToClear;

        if (residueEntriesToClear == 0) {
            matrixApi()->doubleLog(matrixSizeInCoefficients, data, data);
        } else {
            Scalar* columnBase = data;
            for (unsigned long long ci=0 ; ci<static_cast<unsigned long long>(numberColumns) ; ++ci) {
                matrixApi()->doubleLog(numberRows, columnBase, columnBase);
                columnBase += columnSpacing;
            }
        }

        matrixApi()->doubleVectorScalarMultiplyAdd(matrixSizeInCoefficients, data, -2.0, 0, data);
        matrixApi()->doubleSquareRoot(matrixSizeInCoefficients, data, data);
        matrixApi()->doubleVectorScalarMultiplyAdd(matrixSizeInCoefficients, data, scale, 0, data);

        return result;
    }


    MatrixReal::DenseData* MatrixReal::DenseData::randomChiSquared(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns,
            Integer    k
        ) {
        return randomGamma(pt, numberRows, numberColumns, k / 2.0, 2.0);
    }


    MatrixReal::DenseData* MatrixReal::DenseData::randomLogNormal(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns
        ) {
        DenseData*         result                   = randomNormal(pt, numberRows, numberColumns);
        Scalar*            data                     = result->data();
        unsigned long long columnSpacing            = result->columnSpacingInMemory();
        unsigned long long residueEntriesToClear    = columnSpacing - numberRows;
        unsigned long long matrixSizeInCoefficients = columnSpacing * numberColumns - residueEntriesToClear;

        if (residueEntriesToClear == 0) {
            matrixApi()->doubleExponential(matrixSizeInCoefficients, data, data);
        } else {
            Scalar* columnBase = data;
            for (unsigned long long ci=0 ; ci<static_cast<unsigned long long>(numberColumns) ; ++ci) {
                matrixApi()->doubleExponential(numberRows, columnBase, columnBase);
                columnBase += columnSpacing;
            }
        }

        return result;
    }


    MatrixReal::DenseData* MatrixReal::DenseData::randomLogNormal(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns,
            Real       mean,
            Real       sigma
        ) {
        DenseData*         result                   = randomNormal(pt, numberRows, numberColumns, mean, sigma);
        Scalar*            data                     = result->data();
        unsigned long long columnSpacing            = result->columnSpacingInMemory();
        unsigned long long residueEntriesToClear    = columnSpacing - numberRows;
        unsigned long long matrixSizeInCoefficients = columnSpacing * numberColumns - residueEntriesToClear;

        if (residueEntriesToClear == 0) {
            matrixApi()->doubleExponential(matrixSizeInCoefficients, data, data);
        } else {
            Scalar* columnBase = data;
            for (unsigned long long ci=0 ; ci<static_cast<unsigned long long>(numberColumns) ; ++ci) {
                matrixApi()->doubleExponential(numberRows, columnBase, columnBase);
                columnBase += columnSpacing;
            }
        }

        return result;
    }


    MatrixReal::DenseData* MatrixReal::DenseData::randomCauchyLorentz(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns,
            Real       location,
            Real       scale
        ) {
        DenseData*         result                   = randomExclusive(pt, numberRows, numberColumns);
        Scalar*            data                     = result->data();
        unsigned long long columnSpacing            = result->columnSpacingInMemory();
        unsigned long long residueEntriesToClear    = columnSpacing - numberRows;
        unsigned long long matrixSizeInCoefficients = columnSpacing * numberColumns - residueEntriesToClear;

        if (residueEntriesToClear == 0) {
            matrixApi()->doubleVectorScalarMultiplyAdd(matrixSizeInCoefficients, data, M::pi, -M::pi / 2.0, data);
        } else {
            Scalar* columnBase = data;
            for (unsigned long long ci=0 ; ci<static_cast<unsigned long long>(numberColumns) ; ++ci) {
                matrixApi()->doubleVectorScalarMultiplyAdd(
                    numberRows,
                    columnBase,
                    M::pi,
                    -M::pi / 2.0,
                    columnBase
                );

                columnBase += columnSpacing;
            }
        }

        matrixApi()->doubleTangent(matrixSizeInCoefficients, data, data);
        matrixApi()->doubleVectorScalarMultiplyAdd(matrixSizeInCoefficients, data, scale, location, data);

        return result;
    }


    MatrixReal::Data* MatrixReal::DenseData::add(
            const MatrixDensePrivate<MatrixReal::DenseData::Scalar>& addend,
            bool                                                     augendTranspose,
            bool                                                     addendTranspose,
            MatrixReal::DenseData::Scalar                            augendScalar,
            MatrixReal::DenseData::Scalar                            addendScalar
        ) const {
        DenseData* result = nullptr;

        unsigned long augendNumberRows    = numberRows();
        unsigned long augendNumberColumns = numberColumns();

        MatOperation  augendTransformType;
        unsigned long augendTransformNumberRows;
        unsigned long augendTransformNumberColumns;

        if (augendTranspose) {
            augendTransformType          = MatOperation::TRANSPOSE;
            augendTransformNumberRows    = augendNumberColumns;
            augendTransformNumberColumns = augendNumberRows;
        } else {
            augendTransformType          = MatOperation::NO_OPERATION;
            augendTransformNumberRows    = augendNumberRows;
            augendTransformNumberColumns = augendNumberColumns;
        }

        unsigned long addendNumberRows    = addend.numberRows();
        unsigned long addendNumberColumns = addend.numberColumns();

        MatOperation  addendTransformType;
        unsigned long addendTransformNumberRows;
        unsigned long addendTransformNumberColumns;

        if (addendTranspose) {
            addendTransformType          = MatOperation::TRANSPOSE;
            addendTransformNumberRows    = addendNumberColumns;
            addendTransformNumberColumns = addendNumberRows;
        } else {
            addendTransformType          = MatOperation::NO_OPERATION;
            addendTransformNumberRows    = addendNumberRows;
            addendTransformNumberColumns = addendNumberColumns;
        }

        if (augendTransformNumberRows != addendTransformNumberRows       ||
            augendTransformNumberColumns != addendTransformNumberColumns    ) {
            throw Model::IncompatibleMatrixDimensions(
                augendTransformNumberRows,
                augendTransformNumberColumns,
                addendTransformNumberRows,
                addendTransformNumberColumns
            );
        } else {
            result = createUninitialized(augendTransformNumberRows, augendTransformNumberColumns);

            const Scalar*      augendData               = data();
            unsigned long long augendColumnSpacing      = columnSpacingInMemory();
            const Scalar*      addendData               = addend.data();
            unsigned long long addendColumnSpacing      = addend.columnSpacingInMemory();
            Scalar*            destinationData          = result->data();
            unsigned long long destinationColumnSpacing = result->columnSpacingInMemory();

            matrixApi()->doubleScaleAdd(
                MatMatrixMode::COLUMN_MAJOR,
                augendTransformType,
                addendTransformType,
                augendTransformNumberRows,
                augendTransformNumberColumns,
                augendScalar,
                augendData,
                augendColumnSpacing,
                addendScalar,
                addendData,
                addendColumnSpacing,
                destinationData,
                destinationColumnSpacing
            );

            unsigned long long residueRows = destinationColumnSpacing - augendTransformNumberRows;
            if (residueRows > 0) {
                unsigned residueSizeInBytes = static_cast<unsigned>(sizeof(Scalar) * residueRows);
                for (unsigned long columnIndex=0 ; columnIndex<augendTransformNumberColumns ; ++columnIndex) {
                    void* residueBaseAddress = result->addressOf(augendTransformNumberRows, columnIndex);
                    std::memset(residueBaseAddress, 0, residueSizeInBytes);
                }
            }
        }

        return result;
    }


    MatrixReal::Data* MatrixReal::DenseData::add(
            const MatrixSparsePrivate<MatrixReal::DenseData::Scalar>& addend,
            bool                                                      augendtTranspose,
            bool                                                      addendTranspose,
            MatrixReal::DenseData::Scalar                             augendScalar,
            MatrixReal::DenseData::Scalar                             addendScalar
        ) const {
        (void) addend; // FIXME
        (void) augendtTranspose; // FIXME
        (void) addendTranspose; // FIXME
        (void) augendScalar; // FIXME
        (void) addendScalar; // FIXME
        return nullptr; // FIXME
    }


    MatrixReal::Data* MatrixReal::DenseData::multiply(
            const MatrixDensePrivate<MatrixReal::DenseData::Scalar>& multiplicand,
            bool                                                     multiplierTranspose,
            bool                                                     multiplicandTranspose,
            MatrixReal::DenseData::Scalar                            scalerMultiplier
        ) const {
        DenseData* result = nullptr;

        unsigned long multiplierRows      = numberRows();
        unsigned long multiplierColumns   = numberColumns();
        unsigned long multiplicandRows    = multiplicand.numberRows();
        unsigned long multiplicandColumns = multiplicand.numberColumns();

        if (multiplierRows != 0 && multiplierColumns != 0 && multiplicandRows != 0 && multiplicandColumns != 0) {
            unsigned long multiplierOpRows;
            unsigned long multiplierOpColumns;
            if (multiplierTranspose) {
                multiplierOpRows    = multiplierColumns;
                multiplierOpColumns = multiplierRows;
            } else {
                multiplierOpRows    = multiplierRows;
                multiplierOpColumns = multiplierColumns;
            }

            unsigned long multiplicandOpRows;
            unsigned long multiplicandOpColumns;
            if (multiplicandTranspose) {
                multiplicandOpRows    = multiplicandColumns;
                multiplicandOpColumns = multiplicandRows;
            } else {
                multiplicandOpRows    = multiplicandRows;
                multiplicandOpColumns = multiplicandColumns;
            }

            if (multiplierOpColumns != multiplicandOpRows) {
                throw Model::IncompatibleMatrixDimensions(
                    multiplierOpRows,
                    multiplierOpColumns,
                    multiplicandOpRows,
                    multiplicandOpColumns
                );
            } else {
                result = DenseData::create(multiplierOpRows, multiplicandOpColumns);

                const Scalar*      multiplierData                    = data();
                unsigned long long multiplierColumnSpacingInMemory   = columnSpacingInMemory();
                const Scalar*      multiplicandData                  = multiplicand.data();
                unsigned long long multiplicandColumnSpacingInMemory = multiplicand.columnSpacingInMemory();
                Scalar*            resultData                        = result->data();
                unsigned long long resultColumnSpacingInMemory       = result->columnSpacingInMemory();
                Scalar             beta(0);

                matrixApi()->blasDoubleMultiplyAdd(
                    MatMatrixMode::COLUMN_MAJOR,
                    multiplierTranspose ? MatOperation::TRANSPOSE : MatOperation::NO_OPERATION,
                    multiplicandTranspose ? MatOperation::TRANSPOSE : MatOperation::NO_OPERATION,
                    multiplierOpRows,
                    multiplicandOpColumns,
                    multiplierOpColumns,
                    scalerMultiplier,
                    multiplierData,
                    multiplierColumnSpacingInMemory,
                    multiplicandData,
                    multiplicandColumnSpacingInMemory,
                    beta,
                    resultData,
                    resultColumnSpacingInMemory
                );
            }
        } else {
            result = DenseData::create(0, 0);
        }

        return result;
    }


    MatrixReal::Data* MatrixReal::DenseData::multiply(
            const MatrixSparsePrivate<MatrixReal::DenseData::Scalar>& multiplicand,
            bool                                                      multiplierTranspose,
            bool                                                      multiplicandTranspose,
            MatrixReal::DenseData::Scalar                             scalerMultiplier
        ) const {
        (void) multiplicand; // FIXME
        (void) multiplierTranspose; // FIXME
        (void) multiplicandTranspose; // FIXME
        (void) scalerMultiplier; // FIXME
        return nullptr; // FIXME
    }


    bool MatrixReal::DenseData::isEqualTo(const MatrixDensePrivate<MatrixReal::DenseData::Scalar>& other) const {
        bool matches;

        unsigned long thisNumberRows  = numberRows();
        unsigned long otherNumberRows = other.numberRows();
        if (thisNumberRows == otherNumberRows) {
            unsigned long thisNumberColumns  = numberColumns();
            unsigned long otherNumberColumns = other.numberColumns();
            if (thisNumberColumns == otherNumberColumns) {
                unsigned long long columnSizeInBytes = sizeof(Scalar) * thisNumberRows;
                matches = true;

                unsigned long columnIndex = 0;
                while (matches && columnIndex < thisNumberColumns) {
                    const void* thisColumnBase  = addressOf(0, columnIndex);
                    const void* otherColumnBase = other.addressOf(0, columnIndex);

                    if (std::memcmp(thisColumnBase, otherColumnBase, columnSizeInBytes) != 0) {
                        matches = false;
                    }

                    ++columnIndex;
                }
            } else {
                matches = false;
            }
        } else {
            matches = false;
        }

        return matches;
    }


    bool MatrixReal::DenseData::isEqualTo(const MatrixSparsePrivate<MatrixReal::DenseData::Scalar>& other) const {
        (void) other; // FIXME
        assert(false); // FIXME
        return false;
    }


    MatrixReal::Data* MatrixReal::DenseData::applyTransformAndScaling(
            bool                                 transpose,
            MatrixReal::DenseData::Scalar        scaleFactor
        ) const {
        unsigned long inNumberRows    = numberRows();
        unsigned long inNumberColumns = numberColumns();

        MatOperation  transformType;
        unsigned long outNumberRows;
        unsigned long outNumberColumns;

        if (transpose) {
            transformType    = MatOperation::TRANSPOSE;
            outNumberRows    = inNumberColumns;
            outNumberColumns = inNumberRows;
        } else {
            transformType    = MatOperation::NO_OPERATION;
            outNumberRows    = inNumberRows;
            outNumberColumns = inNumberColumns;
        }

        DenseData*         result                   = createUninitialized(outNumberRows, outNumberColumns);
        const Scalar*      sourceData               = data();
        unsigned long long sourceColumnSpacing      = columnSpacingInMemory();
        Scalar*            destinationData          = result->data();
        unsigned long long destinationColumnSpacing = result->columnSpacingInMemory();

        matrixApi()->doubleScaleCopy(
            MatMatrixMode::COLUMN_MAJOR,
            transformType,
            inNumberRows,
            inNumberColumns,
            scaleFactor,
            sourceData,
            sourceColumnSpacing,
            destinationData,
            destinationColumnSpacing
        );

        unsigned long long residueRows = destinationColumnSpacing - outNumberRows;
        if (residueRows > 0) {
            unsigned residueSizeInBytes = static_cast<unsigned>(sizeof(Scalar) * residueRows);
            for (unsigned long columnIndex=0 ; columnIndex<outNumberColumns ; ++columnIndex) {
                void* residueBaseAddress = result->addressOf(outNumberRows, columnIndex);
                std::memset(residueBaseAddress, 0, residueSizeInBytes);
            }
        }

        return result;
    }


    int MatrixReal::DenseData::relativeOrder(const MatrixReal::Data& other) const {
        int result;

        if (other.matrixType() == MatrixType::DENSE) {
            const DenseData& denseOther = POLYMORPHIC_CAST<const DenseData&>(other);
            result = MatrixDensePrivateBase::relativeOrder(denseOther);
        } else {
            result = static_cast<int>(matrixType()) - static_cast<int>(other.matrixType());
        }

        return result;
    }


    long double MatrixReal::DenseData::smallLimit() {
        if (currentSmallLimit < 0) {
            currentSmallLimit = matrixApi()->lapackDoubleMachineParameter(MatMachineParameter::SAFE_MINIMUM);
        }

        return currentSmallLimit;
    }


    long double MatrixReal::DenseData::bigLimit() {
        if (currentBigLimit < 0) {
            currentBigLimit = 1.0L / smallLimit();
        }

        return currentBigLimit;
    }


    MatrixReal::DenseData* MatrixReal::DenseData::generatePivotMatrix(
            const MatInteger* matPivotOperationsByRow,
            unsigned long     numberRows,
            unsigned long     matPivotArraySize
        ) {
        // The pivotOperationsByRow array contains an ordered list of pivot operations by row.  Row numbers are one
        // based.
        //
        // We need to turn this list of operations into a pivot matrix.  To do this, we first create an array of
        // rows representing the results of all the row swap operations.  We then convert that array into a
        // pivot matrix.

        unsigned long* pivotArray = new unsigned long[numberRows];
        for (unsigned long rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
            pivotArray[rowIndex] = rowIndex;
        }

        for (unsigned long rowIndexA=0 ; rowIndexA<matPivotArraySize ; ++rowIndexA) {
            unsigned long rowIndexB = matPivotOperationsByRow[rowIndexA] - 1;
            if (rowIndexA != rowIndexB) {
                unsigned long rowA = pivotArray[rowIndexA];
                unsigned long rowB = pivotArray[rowIndexB];
                pivotArray[rowIndexA] = rowB;
                pivotArray[rowIndexB] = rowA;
            }
        }

        DenseData* pivotMatrix = DenseData::create(numberRows, numberRows);

        for (unsigned initialRowIndex=0 ; initialRowIndex<numberRows ; ++initialRowIndex) {
            unsigned long newRowIndex = pivotArray[initialRowIndex];
            *reinterpret_cast<Scalar*>(pivotMatrix->addressOf(newRowIndex, initialRowIndex)) = Scalar(1);
        }

        delete[] pivotArray;

        return pivotMatrix;
    }


    void MatrixReal::DenseData::toInteger(
            unsigned long          numberEntries,
            unsigned               padding,
            const Scalar*          source,
            MatrixInteger::Scalar* destination
        ) {
        // TODO: Consider faster approaches for this method.

        for (unsigned i=0 ; i<numberEntries ; ++i) {
            MatrixInteger::Scalar v = source[i];
            destination[i] = v;
        }

        if (padding > 0) {
            std::memset(destination + numberEntries, 0, padding * sizeof(Scalar));
        }
    }


    MatrixReal::DenseData* MatrixReal::DenseData::randomNormalEngine(PerThread &pt, Integer numberRows, Integer numberColumns) {
        // This method uses the Marsaglia Polar Method to calculate two terms simultaneously.  We fill the matrix with
        // random 64-bit integer values and then convert the values to values in a normal distribution.  If we run out
        // of space, we repeat starting at the first unused location.
        //
        // To keep things Residue
        // locations are also calculated and then used to populate remaining entries.  If needed, we will fill any
        // remaining entries using the M::PerThread::randomNormal method.

        union ValueUnion {
            Scalar        f;
            std::uint64_t i;
        };

        DenseData*         result                   = createUninitialized(numberRows, numberColumns);
        ValueUnion*        data                     = reinterpret_cast<ValueUnion*>(result->data());
        unsigned long long columnSpacing            = result->columnSpacingInMemory();
        unsigned long long matrixSizeInCoefficients = columnSpacing * numberColumns;
        unsigned long long residueEntriesToClear    = columnSpacing - numberRows;
        unsigned long long firstTailEntry           = matrixSizeInCoefficients - residueEntriesToClear;
        unsigned long long d                        = 0;

        while (d < firstTailEntry) {
            unsigned long long s                  = d;
            unsigned long long coefficientsNeeded = matrixSizeInCoefficients - d;

            pt.fillArray(data + d, coefficientsNeeded * sizeof(Scalar) / 8);

            while (s < matrixSizeInCoefficients && d < firstTailEntry) {
                Real x1;
                Real x2;
                Real w;

                do {
                    std::uint64_t vi1 = data[s++].i;
                    std::uint64_t vi2 = data[s++].i;

                    x1 = 2.0 * (static_cast<double>(vi1) / static_cast<double>(0xFFFFFFFFFFFFFFFFULL)) - 1.0;
                    x2 = 2.0 * (static_cast<double>(vi2) / static_cast<double>(0xFFFFFFFFFFFFFFFFULL)) - 1.0;

                    w = x1 * x1 + x2 * x2;
                } while (s < matrixSizeInCoefficients && (w == 0.0 || w >= 1.0));

                if (w > 0 && w< 1.0) {
                    w = std::sqrt(-2.0 * std::log(w) / w);
                    data[d + 0].f = x1 * w;
                    data[d + 1].f = x2 * w;

                    d += 2;
                }
            }
        }

        return result;
    }


    MatrixReal::DenseData* MatrixReal::DenseData::multiplyRows(
            const DenseData& sourceMatrix,
            const double*    rowScaleFactors
        ) {
        unsigned long numberRows    = sourceMatrix.numberRows();
        unsigned long numberColumns = sourceMatrix.numberColumns();

        DenseData* result = createUninitialized(numberRows, numberColumns);

        unsigned long long columnSizeBytes    = numberRows * sizeof(Scalar);
        unsigned long long columnSpacingBytes = sourceMatrix.columnSpacingInMemory() * sizeof(Scalar);
        unsigned           residueSizeBytes   = static_cast<unsigned>(columnSpacingBytes - columnSizeBytes);

        if (residueSizeBytes > 0) {
            for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                const Scalar* source      = reinterpret_cast<const Scalar*>(sourceMatrix.addressOf(0, columnIndex));
                std::uint8_t* destination = reinterpret_cast<std::uint8_t*>(result->addressOf(0, columnIndex));

                matrixApi()->doubleDotProduct(
                    numberRows,
                    source,
                    rowScaleFactors,
                    reinterpret_cast<double*>(destination)
                );
                std::memset(destination + columnSizeBytes, 0, residueSizeBytes);
            }
        } else {
            for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                matrixApi()->doubleDotProduct(
                    numberRows,
                    reinterpret_cast<const double*>(sourceMatrix.addressOf(0, columnIndex)),
                    rowScaleFactors,
                    reinterpret_cast<double*>(result->addressOf(0, columnIndex))
                );
            }
        }

        return result;
    }


    MatrixReal::Data* MatrixReal::DenseData::dctHelper(const MatRealDct dctEngine) const {
        MatrixReal::Data* result = nullptr;

        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        if (numberRows == 1 || numberColumns == 1) {
            // A 1D DCT -- life is simple.
            unsigned long long numberCoefficients = numberRows * numberColumns;

            DenseData* resultData = createUninitialized(numberRows, numberColumns);
            MatInteger status = (*dctEngine)(numberCoefficients, data(), resultData->data());

            if (status != 0) {
                throw Model::InsufficientMemory();
            }

            unsigned long long numberAllocatedCoefficients = resultData->columnSpacingInMemory() * numberColumns;
            if (numberAllocatedCoefficients > numberCoefficients) {
                unsigned long long residueSize = (numberAllocatedCoefficients - numberCoefficients) * sizeof(Scalar);
                memset(resultData->data() + numberCoefficients, 0, residueSize);
            }

            result = resultData;
        } else {
            // A 2D DCT -- First do 1D DCT by rows, then 1D DCT by columns.

            DenseData* dctByRows = dctHelper2(this, dctEngine);
            DenseData* dctByRowsByColumns = dctHelper2(dctByRows, dctEngine);

            DenseData::destroy(dctByRows);

            result = dctByRowsByColumns;
        }

        return result;
    }


    MatrixReal::DenseData* MatrixReal::DenseData::dctHelper2(
            const MatrixReal::DenseData* input,
            const MatRealDct             dctEngine
        ) {
        unsigned long numberRows    = input->numberRows();
        unsigned long numberColumns = input->numberColumns();

        DenseData* transposedInput = createUninitialized(numberColumns, numberRows);
        matrixApi()->doubleScaleCopy(
            MatMatrixMode::COLUMN_MAJOR,
            MatOperation::TRANSPOSE,
            numberRows,
            numberColumns,
            1.0,
            input->data(),
            input->columnSpacingInMemory(),
            transposedInput->data(),
            transposedInput->columnSpacingInMemory()
        );

        DenseData*         result        = createUninitialized(numberColumns, numberRows);
        const Scalar*      sourceData    = transposedInput->data();
        Scalar*            resultData    = result->data();
        unsigned long long columnSpacing = result->columnSpacingInMemory();

        if (columnSpacing > numberColumns) {
            unsigned residue = columnSpacing - numberColumns;
            for (unsigned rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                MatInteger status = (*dctEngine)(numberColumns, sourceData, resultData);

                if (status != 0) {
                    throw Model::InsufficientMemory();
                }

                resultData += numberColumns;
                std::memset(resultData, 0, residue * sizeof(Scalar));

                sourceData += columnSpacing;
            }
        } else {
            for (unsigned rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                (*dctEngine)(numberColumns, sourceData, resultData);
                sourceData += numberColumns;
                resultData += numberColumns;
            }
        }

        DenseData::destroy(transposedInput);
        return result;
    }
}
