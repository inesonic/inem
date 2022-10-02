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
* This file implements the \ref M::MatrixComplex::DenseData class.
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

#include <boost/algorithm/string.hpp>

#include "m_api_types.h"
#include "m_intrinsic_types.h"
#include "m_basic_functions.h"
#include "model_exceptions.h"
#include "model_range.h"
#include "m_matrix_dense_private.h"
#include "m_matrix_sparse_private.h"
#include "m_matrix_real.h"
#include "m_matrix_real_dense_data.h"
#include "m_matrix_integer.h"
#include "m_matrix_integer_dense_data.h"
#include "m_matrix_boolean.h"
//#include "m_matrix_boolean_dense_data.h"
#include "m_matrix_complex.h"
#include "m_matrix_complex_data.h"
#include "m_matrix_complex_dense_data.h"

namespace M {
    long double MatrixComplex::DenseData::currentSmallLimit = -1;
    long double MatrixComplex::DenseData::currentBigLimit   = -1;

    MatrixComplex::DenseData::DenseData(
            unsigned long      newNumberRows,
            unsigned long      newNumberColumns,
            unsigned long long newAllocationInBytes,
            void*              rawDataBaseAddress
        ):MatrixDensePrivate<MatrixComplex::DenseData::Scalar>(
            newNumberRows,
            newNumberColumns,
            newAllocationInBytes,
            rawDataBaseAddress
        ) {
    }


    MatrixComplex::DenseData::DenseData(
            const MatrixDensePrivate<MatrixComplex::DenseData::Scalar>& other,
            unsigned long long                                          newAllocationInBytes,
            void*                                                       rawDataBaseAddress
        ):MatrixDensePrivate<MatrixComplex::DenseData::Scalar>(
            other,
            newAllocationInBytes,
            rawDataBaseAddress
        ) {}


    MatrixComplex::DenseData::DenseData(
            const MatrixDensePrivate<MatrixComplex::DenseData::Scalar>& other,
            unsigned long                                               newNumberRows,
            unsigned long                                               newNumberColumns,
            unsigned long long                                          newAllocationInBytes,
            void*                                                       rawDataBaseAddress
        ):MatrixDensePrivate<MatrixComplex::DenseData::Scalar>(
            other,
            newNumberRows,
            newNumberColumns,
            newAllocationInBytes,
            rawDataBaseAddress
        ) {}


    MatrixComplex::DenseData::~DenseData() {}


    MatrixComplex::DenseData* MatrixComplex::DenseData::create(const MatrixReal::DenseData& other) {
        unsigned long long numberRows    = other.numberRows();
        unsigned long long numberColumns = other.numberColumns();

        const MatrixReal::DenseData::Scalar* s                         = other.data();
        unsigned long long                   srcColumnSpacingInEntries = other.columnSpacingInMemory();
        unsigned long long                   srcResidueInEntries       = srcColumnSpacingInEntries - numberRows;

        DenseData*         result                    = createUninitialized(numberRows, numberColumns);
        Scalar*            d                         = result->data();
        unsigned long long dstColumnSpacingInEntries = result->columnSpacingInMemory();
        unsigned long long dstResidueInEntries       = dstColumnSpacingInEntries - numberRows;

        if (dstResidueInEntries != 0) {
            for (unsigned long long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                for (unsigned long long rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                    MatrixReal::DenseData::Scalar v = *s;
                    *d = Complex(v, 0);

                    ++s;
                    ++d;
                }

                memset(reinterpret_cast<void*>(d), 0, sizeof(Scalar) * dstResidueInEntries);

                s += srcResidueInEntries;
                d += dstResidueInEntries;
            }
        } else {
            for (unsigned long long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                for (unsigned long long rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                    MatrixReal::DenseData::Scalar v = *s;
                    *d = Complex(v, 0);

                    ++s;
                    ++d;
                }

                s += srcResidueInEntries;
            }
        }

        return result;
    }


    MatrixComplex::DenseData* MatrixComplex::DenseData::create(const MatrixInteger::DenseData& other) {
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

                std::memset(reinterpret_cast<void*>(d), 0, destinationResidueInBytes);

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


    MatrixComplex::DenseData* MatrixComplex::DenseData::create(const MatrixBoolean::DenseData& other) {
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

                std::memset(reinterpret_cast<void*>(d), 0, residueBytesToClear);
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


    bool MatrixComplex::DenseData::toFile(const char* filename, Model::DataFileFormat fileFormat) const {
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
                        Scalar value = at(rowIndex, columnIndex);
                        if (columnIndex > 0) {
                            f << "\t" << std::setprecision(17) << value.real()
                              << "\t" << std::setprecision(17) << value.imag();
                        } else {
                            f         << std::setprecision(17) << value.real()
                              << "\t" << std::setprecision(17) << value.imag();
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
                buffer[i++] = 'C';

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
                        Scalar value = at(rowIndex, columnIndex);

                        *reinterpret_cast<Scalar::T*>(buffer + i) = value.real();
                        i += sizeof(Scalar::T);

                        *reinterpret_cast<Scalar::T*>(buffer + i) = value.imag();
                        i += sizeof(Scalar::T);

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


    MatrixComplex::DenseData* MatrixComplex::DenseData::fromFile(const char* filename) {
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

                    std::vector<std::vector<Scalar::T>> matrixData;
                    unsigned long                       numberColumns = 0;

                    while (success && f.peek() != EOF) { // IMHO, C++ stream are broken when testing against EOF.
                        std::vector<Scalar::T> rowArray;

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
                                size_t    index;
                                Scalar::T rv = std::stod(value, &index);

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
                            size_t    index;
                            Scalar::T rv = std::stod(value, &index);

                            if (index != value.length()) {
                                internalTriggerFileReadError(filename);
                                success = false;
                            } else {
                                rowArray.push_back(rv);
                            }
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
                        result = create(numberRows, numberColumns / 2);

                        for (unsigned long rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                            const std::vector<Scalar::T>& rowArray      = matrixData.at(rowIndex);
                            unsigned long                 numberEntries = static_cast<unsigned long>(rowArray.size());

                            if ((numberEntries % 2) != 0) {
                                DenseData::destroy(result);
                                throw Model::FileReadError(filename);
                            }

                            unsigned long numberColumns = static_cast<unsigned long>(numberEntries / 2);
                            for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                                Scalar value(rowArray.at(2 * columnIndex + 0), rowArray.at(2 * columnIndex + 1));
                                result->at(rowIndex, columnIndex) = value;
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
                            if (header[7] != 'C') {
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
                                unsigned long long bufferSize         = 16 * numberCoefficients;
                                std::uint8_t*      buffer             = new std::uint8_t[bufferSize];

                                f.read(reinterpret_cast<char*>(buffer), bufferSize);
                                if (!f) {
                                    delete[] buffer;

                                    internalTriggerFileReadError(filename);
                                    success = false;
                                } else {
                                    result = DenseData::createUninitialized(numberRows, numberColumns);

                                    unsigned long long i = 0;
                                    for (unsigned long rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                                        for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                                            Scalar::T real = *reinterpret_cast<Scalar::T*>(buffer + i);
                                            i += sizeof(Scalar::T);

                                            Scalar::T imag = *reinterpret_cast<Scalar::T*>(buffer + i);
                                            i += sizeof(Scalar::T);

                                            result->at(rowIndex, columnIndex) = Scalar(real, imag);
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


    MatrixComplex::DenseData* MatrixComplex::DenseData::identity(Index numberRows, Index numberColumns) {
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


    MatrixComplex::DenseData* MatrixComplex::DenseData::ones(unsigned long numberRows, unsigned long numberColumns) {
        DenseData* result = createUninitialized(numberRows, numberColumns);

        MatComplex c;
        c.i = 0;
        c.r = 1.0;

        unsigned long long columnSpacing = result->columnSpacingInMemory();
        if (columnSpacing == numberRows) {
            matrixApi()->complexFill(
                numberRows * numberColumns,
                c,
                reinterpret_cast<MatComplex*>(result->data())
            );
        } else {
            unsigned long long residuePerColumn     = columnSpacing - numberRows;
            unsigned           residueLengthInBytes = residuePerColumn * sizeof(Scalar);
            Scalar*            d                    = result->data();

            for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                matrixApi()->complexFill(numberRows, c, reinterpret_cast<MatComplex*>(d));
                d += numberRows;
                std::memset(reinterpret_cast<void*>(d), 0, residueLengthInBytes);
                d += residuePerColumn;
            }
        }

        return result;
    }


    MatrixComplex::Data* MatrixComplex::DenseData::combineLeftToRight(
            const MatrixDensePrivate<MatrixComplex::DenseData::Scalar>& other
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


    MatrixComplex::Data* MatrixComplex::DenseData::combineLeftToRight(
            const MatrixSparsePrivate<MatrixComplex::DenseData::Scalar>& other
        ) const {
        (void) other; // FIXME
        return nullptr; // FIXME
    }


    MatrixComplex::Data* MatrixComplex::DenseData::combineTopToBottom(
            const MatrixDensePrivate<MatrixComplex::DenseData::Scalar>& other
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


    MatrixComplex::Data* MatrixComplex::DenseData::combineTopToBottom(
            const MatrixSparsePrivate<MatrixComplex::DenseData::Scalar>& other
        ) const {
        (void) other; // FIXME
        return nullptr; // FIXME
    }


    MatrixComplex::Data* MatrixComplex::DenseData::columnReverse() const {
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


    MatrixComplex::Data* MatrixComplex::DenseData::rowReverse() const {
        unsigned long      numberRows        = DenseData::numberRows();
        unsigned long      numberColumns     = DenseData::numberColumns();
        DenseData*         result            = createUninitialized(numberRows, numberColumns);
        unsigned long long columnSize        = columnSpacingInMemory();

        for (unsigned long sourceRowIndex=0 ; sourceRowIndex<numberRows ; ++sourceRowIndex) {
            unsigned long destinationRowIndex = numberRows - sourceRowIndex - 1;
            const void*   source              = addressOf(sourceRowIndex, 0);
            void*         destination         = result->addressOf(destinationRowIndex, 0);

            matrixApi()->blasComplexCopy(
                numberColumns,
                reinterpret_cast<const MatComplex*>(source),
                columnSize,
                reinterpret_cast<MatComplex*>(destination),
                columnSize
            );
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


    MatrixComplex::Data* MatrixComplex::DenseData::resizeTo(
            unsigned long newNumberRows,
            unsigned long newNumberColumns,
            bool          alwaysReallocate
        ) {
        MatrixComplex::Data* result = this;
        if (alwaysReallocate || !resizeInPlace(newNumberRows, newNumberColumns)) {
            result = MatrixDensePrivate<MatrixComplex::DenseData::Scalar>::create<DenseData>(
                *this,
                newNumberRows,
                newNumberColumns
            );
        }

        return result;
    }


    bool MatrixComplex::DenseData::resizeInPlaceTo(unsigned long newNumberRows, unsigned long newNumberColumns) {
        return MatrixDensePrivate<MatrixComplex::DenseData::Scalar>::resizeInPlace(newNumberRows, newNumberColumns);
    }


    Complex MatrixComplex::DenseData::determinant() const {
        Scalar result(1, 0);

        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        if (numberRows != numberColumns || numberRows == 0) {
            throw Model::InvalidMatrixDimensions(numberRows, numberColumns);
        } else {
            DenseData*    luMatrix       = static_cast<DenseData*>(clone());
            unsigned long pivotArraySize = std::max(1UL, std::min(numberRows, numberColumns));
            MatInteger*   pivotArray     = new MatInteger[pivotArraySize];

            MatInteger info = matrixApi()->lapackComplexPlu(
                MatMatrixMode::COLUMN_MAJOR,
                numberRows,
                numberColumns,
                reinterpret_cast<MatComplex*>(luMatrix->data()),
                luMatrix->columnSpacingInMemory(),
                pivotArray
            );

            assert(info >= 0);

            if (info > 0) {
                result = Scalar(0);
            } else {
                // TODO: consider using SIMD for larger matrices to calculate the product of the diagonal.
                unsigned long diagnonalSize = std::min(numberRows, numberColumns);
                for (unsigned rowColumnIndex=0 ; rowColumnIndex<diagnonalSize ; ++rowColumnIndex) {
                    Scalar value = luMatrix->at(rowColumnIndex, rowColumnIndex);

                    assert(!std::isnan(value.real()));
                    assert(!std::isnan(value.imag()));
                    assert(!std::isinf(value.real()));
                    assert(!std::isinf(value.imag()));

                    result *= value;
                }

                // The pivot array is not a permutation vector, rather it represents the pivoting steps that were
                // performed.  For this reason, the pivot array is not a symmetrical structure.

                float signMultiplier = +1.0;
                for (unsigned long i=0 ; i<pivotArraySize ; ++i) {
                    if (pivotArray[i] != static_cast<MatInteger>(i+1)) {
                        signMultiplier *= -1.0;
                    }
                }

                result *= signMultiplier;
            }

            delete[] pivotArray;
            destroy(luMatrix);
        }

        return result;
    }


    MatrixComplex::Data* MatrixComplex::DenseData::inverse() const {
        Data* result = nullptr;

        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        if (numberColumns == 0 || numberRows == 0 || numberRows != numberColumns) {
            throw Model::InvalidMatrixDimensions(numberRows, numberColumns);
        } else {
            DenseData*         di                    = static_cast<DenseData*>(clone());
            MatInteger*        pivotOperationsByRow  = new MatInteger[numberRows];
            unsigned long long columnSpacingInMemory = di->columnSpacingInMemory();
            MatComplex*        data                  = reinterpret_cast<MatComplex*>(di->data());

            MatInteger info = matrixApi()->lapackComplexPlu(
                MatMatrixMode::COLUMN_MAJOR,
                numberRows,
                numberColumns,
                data,
                columnSpacingInMemory,
                pivotOperationsByRow
            );

            if (info == 0) {
                info = matrixApi()->lapackComplexLuInverse(
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


    bool MatrixComplex::DenseData::plu(
            MatrixComplex::Data*& p,
            MatrixComplex::Data*& l,
            MatrixComplex::Data*& u
        ) const {
        bool notSingular = false;

        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        if (numberColumns  == 0 || numberRows == 0) {
            p = nullptr;
            l = nullptr;
            u = nullptr;

            throw Model::InvalidMatrixDimensions(numberRows, numberColumns);
        } else {
            DenseData*  du                             = static_cast<DenseData*>(clone());
            unsigned long maximumNumberPivotOperations = std::max(1UL, std::min(numberRows, numberColumns));
            MatInteger*   pivotOperationsByRow         = new MatInteger[maximumNumberPivotOperations];

            MatInteger info = matrixApi()->lapackComplexPlu(
                MatMatrixMode::COLUMN_MAJOR,
                numberRows,
                numberColumns,
                reinterpret_cast<MatComplex*>(du->data()),
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


    bool MatrixComplex::DenseData::svd(
            MatrixComplex::Data*& u,
            MatrixComplex::Data*& s,
            MatrixComplex::Data*& vh
        ) const {
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

            MatInteger info = matrixApi()->lapackComplexSvd(
                MatMatrixMode::COLUMN_MAJOR,
                numberRows,
                numberColumns,
                reinterpret_cast<MatComplex*>(a->data()),
                a->columnSpacingInMemory(),
                singularValues,
                reinterpret_cast<MatComplex*>(du->data()),
                du->columnSpacingInMemory(),
                reinterpret_cast<MatComplex*>(dvh->data()),
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


    bool MatrixComplex::DenseData::qr(MatrixComplex::Data*& q, MatrixComplex::Data*& r) const {
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
                dq = MatrixDensePrivate<MatrixComplex::DenseData::Scalar>::create<DenseData>(
                    *this,
                    numberRows,
                    numberRows
                );
            } else {
                dq = static_cast<DenseData*>(clone());
            }
            unsigned long long numberQReflectors = numberColumns;
            MatComplex*        tau               = new MatComplex[numberQReflectors];

            MatInteger info = matrixApi()->lapackComplexQrFactorization(
                MatMatrixMode::COLUMN_MAJOR,
                numberRows,
                numberColumns,
                reinterpret_cast<MatComplex*>(dq->data()),
                dq->columnSpacingInMemory(),
                tau
            );

            if (info == 0) {
                if (numberRows > numberColumns) {
                    dr = MatrixDensePrivate<Scalar>::create<DenseData>(*dq, numberRows, numberColumns);
                } else {
                    dr = static_cast<DenseData*>(dq->clone());
                }

                info = matrixApi()->lapackComplexGenerateQFromQrMatrix(
                    MatMatrixMode::COLUMN_MAJOR,
                    numberRows,
                    numberRows,
                    numberQReflectors,
                    reinterpret_cast<MatComplex*>(dq->data()),
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


    MatrixComplex::Data* MatrixComplex::DenseData::cholesky() const {
        Data* result;

        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        if (numberColumns  == 0 || numberRows == 0 || numberRows != numberColumns) {
            result  = nullptr;
            throw Model::InvalidMatrixDimensions(numberRows, numberColumns);
        } else {
            DenseData* dl   = static_cast<DenseData*>(clone());
            MatInteger info = matrixApi()->lapackComplexCholesky(
                MatMatrixMode::COLUMN_MAJOR,
                MatCholeskyType::LOWER,
                numberRows,
                reinterpret_cast<MatComplex*>(dl->data()),
                dl->columnSpacingInMemory()
            );

            if (info == 0) {
                dl->toLowerTriangular();
            } else {
                destroy(dl);
                dl = MatrixDensePrivate<Scalar>::create<DenseData>(0, 0);
            }

            result = dl;
        }

        return result;
    }


    MatrixComplex::Data* MatrixComplex::DenseData::upperCholesky() const {
        Data* result;

        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        if (numberColumns  == 0 || numberRows == 0 || numberRows != numberColumns) {
            result = nullptr;
            throw Model::InvalidMatrixDimensions(numberRows, numberColumns);
        } else {
            DenseData* du   = static_cast<DenseData*>(clone());
            MatInteger info = matrixApi()->lapackComplexCholesky(
                MatMatrixMode::COLUMN_MAJOR,
                MatCholeskyType::UPPER,
                numberRows,
                reinterpret_cast<MatComplex*>(du->data()),
                du->columnSpacingInMemory()
            );

            if (info == 0) {
                du->toUpperTriangular();
            } else {
                destroy(du);
                du = MatrixDensePrivate<Scalar>::create<DenseData>(0, 0);
            }

            result = du;
        }

        return result;
    }


    void MatrixComplex::DenseData::hessenberg(MatrixComplex::Data*& qData, MatrixComplex::Data*& hData) const {
        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        if (numberColumns  == 0 || numberRows == 0 || numberRows != numberColumns) {
            qData = nullptr;
            hData = nullptr;
            throw Model::InvalidMatrixDimensions(numberRows, numberColumns);
        } else {
            DenseData*    h       = POLYMORPHIC_CAST<DenseData*>(clone());
            unsigned long tauSize = std::max(1UL, numberRows - 1);
            MatComplex*   tau     = new MatComplex[tauSize];

            MatInteger info = matrixApi()->lapackComplexUpperHessenberg(
                MatMatrixMode::COLUMN_MAJOR,
                numberRows,
                1,
                numberRows,
                reinterpret_cast<MatComplex*>(h->data()),
                h->columnSpacingInMemory(),
                tau
            );

            assert(info == 0);

            DenseData* q = POLYMORPHIC_CAST<DenseData*>(h->clone());

            info = matrixApi()->lapackComplexUpperHessenbergQMatrix(
                MatMatrixMode::COLUMN_MAJOR,
                numberRows,
                1,
                numberRows,
                reinterpret_cast<MatComplex*>(q->data()),
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


    bool MatrixComplex::DenseData::schur(
            MatrixComplex::Data*& qData,
            MatrixComplex::Data*& uData,
            MatrixComplex::Data*& wData
        ) const {
        bool          result        = false;

        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        if (numberColumns  == 0 || numberRows == 0 || numberRows != numberColumns) {
            qData = nullptr;
            uData = nullptr;
            wData = nullptr;

            throw Model::InvalidMatrixDimensions(numberRows, numberColumns);
        } else {
            DenseData* s;
            DenseData* qz;
            double*    scaleTerms;
            DenseData* w;
            MatInteger ilo;
            MatInteger ihi;

            result = calculateSchur(s, qz, scaleTerms, w, ilo, ihi);

            if (result) {
                // Rebalance/unbalance Schur matrix to get back to a usable form.

                MatInteger info = matrixApi()->lapackComplexEigenUnbalance(
                    MatMatrixMode::COLUMN_MAJOR,
                    MatEigenBalanceJob::PERMUTED,
                    MatSide::RIGHT,
                    numberRows,
                    numberColumns,
                    ilo,
                    ihi,
                    scaleTerms,
                    reinterpret_cast<MatComplex*>(s->data()),
                    s->columnSpacingInMemory()
                );

                delete[] scaleTerms;

                (void) info;
                assert(info == 0);

                result = true;
            } else {
                delete[] scaleTerms;

                DenseData::destroy(s);
                DenseData::destroy(qz);
                DenseData::destroy(w);

                s  = DenseData::create(0, 0);
                qz = DenseData::create(0, 0);
                w  = DenseData::create(0, 0);

                result = false;
            }

            qData = qz;
            uData = s;
            wData = w;
        }

        return result;
    }


    bool MatrixComplex::DenseData::eigenvectors(
            bool                   rightEigenvectors,
            MatrixComplex::Data*&  eigenvalueMatrix,
            MatrixComplex::Data*&  qMatrix,
            MatrixComplex::Data*&  uMatrix,
            MatrixComplex::Data**& eigenvectors
        ) const {
        bool          result        = false;

        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        if (numberColumns  == 0 || numberRows == 0 || numberRows != numberColumns) {
            eigenvalueMatrix = nullptr;
            qMatrix          = nullptr;
            uMatrix          = nullptr;
            eigenvectors     = nullptr;

            throw Model::InvalidMatrixDimensions(numberRows, numberColumns);
        } else {
            DenseData* s;
            DenseData* qz;
            double*    scaleTerms;
            DenseData* w;
            MatInteger ilo;
            MatInteger ihi;

            result = calculateSchur(s, qz, scaleTerms, w, ilo, ihi);

            if (result) {
                // Calculate our complete set of eigenvectors

                MatInteger numberEigenvectors = numberColumns;
                DenseData* evs                = POLYMORPHIC_CAST<DenseData*>(qz->clone());

                MatInteger info = matrixApi()->lapackComplexEigenvectors(
                    MatMatrixMode::COLUMN_MAJOR,
                    rightEigenvectors ? MatSide::RIGHT : MatSide::LEFT,
                    MatEigenMode::ALL_FOR_SIDE_WITH_BACKTRANSFORM,
                    nullptr, // select,
                    numberRows,
                    reinterpret_cast<MatComplex*>(s->data()),
                    s->columnSpacingInMemory(),
                    reinterpret_cast<MatComplex*>(evs->data()),
                    evs->columnSpacingInMemory(),
                    reinterpret_cast<MatComplex*>(evs->data()),
                    evs->columnSpacingInMemory(),
                    numberColumns,
                    &numberEigenvectors
                );

                assert(info == 0);

                // Now convert the eigenvectors to individual column matrices.

                eigenvectors = new Data*[numberEigenvectors + 1];
                unsigned long long columnSize = MatrixDensePrivateBase::columnSizeInBytes(numberRows, sizeof(Scalar));
                for (unsigned columnIndex=0 ; columnIndex<numberEigenvectors ; ++columnIndex) {
                    DenseData* ev = DenseData::createUninitialized(numberRows, 1);
                    void*      d  = ev->data();
                    void*      s  = evs->addressOf(0, columnIndex);

                    std::memcpy(d, s, columnSize);

                    eigenvectors[columnIndex] = ev;
                }

                DenseData::destroy(evs);
                eigenvectors[numberEigenvectors] = nullptr;

                // Rebalance/unbalance Schur matrix to get back to a usable form.

                info = matrixApi()->lapackComplexEigenUnbalance(
                    MatMatrixMode::COLUMN_MAJOR,
                    MatEigenBalanceJob::PERMUTED,
                    rightEigenvectors ? MatSide::RIGHT : MatSide::LEFT,
                    numberRows,
                    numberColumns,
                    ilo,
                    ihi,
                    scaleTerms,
                    reinterpret_cast<MatComplex*>(s->data()),
                    s->columnSpacingInMemory()
                );

                delete[] scaleTerms;

                assert(info == 0);

                result = true;
            } else {
                delete[] scaleTerms;

                DenseData::destroy(s);
                DenseData::destroy(qz);
                DenseData::destroy(w);

                s            = DenseData::create(0, 0);
                qz           = DenseData::create(0, 0);
                w            = DenseData::create(0, 0);
                eigenvectors = new Data*[1];

                result = false;
            }

            eigenvalueMatrix = w;
            qMatrix          = qz;
            uMatrix          = s;
        }

        return result;
    }


    MatrixComplex::Data* MatrixComplex::DenseData::dft() const {
        return calculateDft(true);
    }


    MatrixComplex::Data* MatrixComplex::DenseData::idft() const {
        return calculateDft(false);
    }


    Real MatrixComplex::DenseData::pNorm(Integer p) const {
        Real          result;

        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        if (p > 0) {
            if (p == 2) {
                Real squaredResult = 0;
                for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                    const Scalar* v = reinterpret_cast<const Scalar*>(addressOf(0, columnIndex));
                    for (unsigned long rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                        squaredResult += ((*v) * v->conj()).real();
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


    Real MatrixComplex::DenseData::oneNorm() const {
        unsigned long numberRows    = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        Real columnMaximum = 0;
        for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
            Real columnSum = 0;
            const Scalar* v = reinterpret_cast<const Scalar*>(addressOf(0, columnIndex));
            for (unsigned long rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                columnSum += abs(*v);
                ++v;
            }

            if (columnSum > columnMaximum) {
                columnMaximum = columnSum;
            }
        }

        return columnMaximum;
    }


    Real MatrixComplex::DenseData::infinityNorm() const {
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



    bool MatrixComplex::DenseData::equilibrate(MatrixComplex::Data*& row, MatrixComplex::Data*& column) const {
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

            MatInteger info = matrixApi()->lapackComplexEquilibrate(
                MatMatrixMode::COLUMN_MAJOR,
                numberRows,
                numberColumns,
                reinterpret_cast<const MatComplex*>(data()),
                columnSpacingInMemory(),
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


    MatrixComplex::Data* MatrixComplex::DenseData::solve(const MatrixComplex::DenseData* y) const {
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

            MatInteger info = matrixApi()->lapackComplexEquilibratePowerOf2(
                MatMatrixMode::COLUMN_MAJOR,
                aNumberRows,
                aNumberColumns,
                reinterpret_cast<const MatComplex*>(data()),
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
                MatInteger info = matrixApi()->lapackComplexSolve(
                    MatMatrixMode::COLUMN_MAJOR,
                    aNumberRows,
                    yNumberColumns,
                    reinterpret_cast<MatComplex*>(a->data()),
                    a->columnSpacingInMemory(),
                    pivotOperationArray,
                    reinterpret_cast<const MatComplex*>(yValues->data()),
                    yValues->columnSpacingInMemory(),
                    reinterpret_cast<MatComplex*>(x->data()),
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


    MatrixComplex::Data* MatrixComplex::DenseData::solve(const MatrixComplex::SparseData* y) const {
        (void) y; // FIXME
        return nullptr; // FIXME
    }


    MatrixComplex::Data* MatrixComplex::DenseData::leastSquares(const MatrixComplex::DenseData* b) const {
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
            unsigned long xNumberRows  = std::max(aNumberRows, aNumberColumns);
            DenseData*    a            = POLYMORPHIC_CAST<DenseData*>(clone());
            DenseData*    x            = POLYMORPHIC_CAST<DenseData*>(
                const_cast<DenseData*>(POLYMORPHIC_CAST<const DenseData*>(b))
                    ->resizeTo(xNumberRows, bNumberColumns, true)
            );

            MatInteger info = matrixApi()->lapackComplexLeastSquaresSolve(
                MatMatrixMode::COLUMN_MAJOR,
                MatOperation::NO_OPERATION,
                aNumberRows,
                aNumberColumns,
                bNumberColumns,
                reinterpret_cast<MatComplex*>(a->data()),
                a->columnSpacingInMemory(),
                reinterpret_cast<MatComplex*>(x->data()),
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


    MatrixComplex::Data* MatrixComplex::DenseData::leastSquares(const MatrixComplex::SparseData* b) const {
        (void) b; // FIXME
        return nullptr; // FIXME
    }


    MatrixComplex::Data* MatrixComplex::DenseData::hadamard(
            const MatrixDensePrivate<MatrixComplex::DenseData::Scalar>& other
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

                    matrixApi()->complexDotProduct(
                        columnSizeInEntries,
                        reinterpret_cast<const MatComplex*>(a),
                        reinterpret_cast<const MatComplex*>(b),
                        reinterpret_cast<MatComplex*>(y)
                    );

                    std::memset(reinterpret_cast<std::uint8_t*>(y) + columnDataSizeInBytes, 0, residueSizeInBytes);
                }
            } else {
                for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                    const Scalar* a = reinterpret_cast<const Scalar*>(addressOf(0, columnIndex));
                    const Scalar* b = reinterpret_cast<const Scalar*>(other.addressOf(0, columnIndex));
                    Scalar*       y = reinterpret_cast<Scalar*>(result->addressOf(0, columnIndex));

                    matrixApi()->complexDotProduct(
                        columnSizeInEntries,
                        reinterpret_cast<const MatComplex*>(a),
                        reinterpret_cast<const MatComplex*>(b),
                        reinterpret_cast<MatComplex*>(y)
                    );
                }
            }
        }

        return result;
    }


    MatrixComplex::Data* MatrixComplex::DenseData::hadamard(
            const MatrixSparsePrivate<MatrixComplex::DenseData::Scalar>& other
        ) const {
        (void) other; // FIXME
        return nullptr; // FIXME
    }


    MatrixComplex::Data* MatrixComplex::DenseData::kronecker(
            const MatrixDensePrivate<MatrixComplex::DenseData::Scalar>& other,
            MatrixComplex::DenseData::LazyTransform                     otherTransform,
            const MatrixComplex::DenseData::Scalar&                     scalarMultiplier
        ) const {
        unsigned long multiplierNumberRows        = numberRows();
        unsigned long multiplierNumberColumns     = numberColumns();
        unsigned long multiplicandInNumberRows    = other.numberRows();
        unsigned long multiplicandInNumberColumns = other.numberColumns();

        MatOperation  multiplicandTransformType;
        unsigned long multiplicandOutNumberRows;
        unsigned long multiplicandOutNumberColumns;

        switch (otherTransform) {
            case LazyTransform::NONE: {
                multiplicandTransformType    = MatOperation::NO_OPERATION;
                multiplicandOutNumberRows    = multiplicandInNumberRows;
                multiplicandOutNumberColumns = multiplicandInNumberColumns;

                break;
            }

            case LazyTransform::TRANSPOSE: {
                multiplicandTransformType    = MatOperation::TRANSPOSE;
                multiplicandOutNumberRows    = multiplicandInNumberColumns;
                multiplicandOutNumberColumns = multiplicandInNumberRows;

                break;
            }

            case LazyTransform::CONJUGATE: {
                multiplicandTransformType    = MatOperation::CONJUGATE;
                multiplicandOutNumberRows    = multiplicandInNumberRows;
                multiplicandOutNumberColumns = multiplicandInNumberColumns;

                break;
            }

            case LazyTransform::ADJOINT: {
                multiplicandTransformType    = MatOperation::CONJUGATE_TRANSPOSE;
                multiplicandOutNumberRows    = multiplicandInNumberColumns;
                multiplicandOutNumberColumns = multiplicandInNumberRows;

                break;
            }

            default: {
                assert(false);
                break;
            }
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
                unsigned long         productTopRow         = multiplierRow * multiplicandOutNumberRows;
                const Model::Complex& multiplierCoefficient = at(multiplierRow, multiplierColumn) * scalarMultiplier;

                Scalar* resultData = reinterpret_cast<Scalar*>(result->addressOf(productTopRow, productLeftColumn));

                matrixApi()->complexScaleCopy(
                    MatMatrixMode::COLUMN_MAJOR,
                    multiplicandTransformType,
                    multiplicandInNumberRows,
                    multiplicandInNumberColumns,
                    reinterpret_cast<const MatComplex*>(&multiplierCoefficient),
                    reinterpret_cast<const MatComplex*>(multiplicandData),
                    multipicandColumnSizeInEntries,
                    reinterpret_cast<MatComplex*>(resultData),
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


    MatrixComplex::Data* MatrixComplex::DenseData::kronecker(
            const MatrixSparsePrivate<MatrixComplex::DenseData::Scalar>& other,
            MatrixComplex::DenseData::LazyTransform                      otherTransform,
            const MatrixComplex::DenseData::Scalar&                      scalarMultiplier
        ) const {
        (void) other; // FIXME
        (void) otherTransform; // FIXME
        (void) scalarMultiplier; // FIXME
        return nullptr; // FIXME
    }


    MatrixComplex::Data* MatrixComplex::DenseData::add(
            const MatrixDensePrivate<MatrixComplex::DenseData::Scalar>& addend,
            LazyTransform                                               augendTransform,
            LazyTransform                                               addendTransform,
            const Complex&                                              augendScalar,
            const Complex&                                              addendScalar
        ) const {
        DenseData* result = nullptr;

        unsigned long augendNumberRows    = numberRows();
        unsigned long augendNumberColumns = numberColumns();

        MatOperation  augendTransformType;
        unsigned long augendTransformNumberRows;
        unsigned long augendTransformNumberColumns;

        switch (augendTransform) {
            case LazyTransform::NONE: {
                augendTransformType          = MatOperation::NO_OPERATION;
                augendTransformNumberRows    = augendNumberRows;
                augendTransformNumberColumns = augendNumberColumns;

                break;
            }

            case LazyTransform::TRANSPOSE: {
                augendTransformType          = MatOperation::TRANSPOSE;
                augendTransformNumberRows    = augendNumberColumns;
                augendTransformNumberColumns = augendNumberRows;

                break;
            }

            case LazyTransform::CONJUGATE: {
                augendTransformType          = MatOperation::CONJUGATE;
                augendTransformNumberRows    = augendNumberRows;
                augendTransformNumberColumns = augendNumberColumns;

                break;
            }

            case LazyTransform::ADJOINT: {
                augendTransformType          = MatOperation::CONJUGATE_TRANSPOSE;
                augendTransformNumberRows    = augendNumberColumns;
                augendTransformNumberColumns = augendNumberRows;

                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        unsigned long addendNumberRows    = addend.numberRows();
        unsigned long addendNumberColumns = addend.numberColumns();

        MatOperation  addendTransformType;
        unsigned long addendTransformNumberRows;
        unsigned long addendTransformNumberColumns;

        switch (addendTransform) {
            case LazyTransform::NONE: {
                addendTransformType          = MatOperation::NO_OPERATION;
                addendTransformNumberRows    = addendNumberRows;
                addendTransformNumberColumns = addendNumberColumns;

                break;
            }

            case LazyTransform::TRANSPOSE: {
                addendTransformType          = MatOperation::TRANSPOSE;
                addendTransformNumberRows    = addendNumberColumns;
                addendTransformNumberColumns = addendNumberRows;

                break;
            }

            case LazyTransform::CONJUGATE: {
                addendTransformType          = MatOperation::CONJUGATE;
                addendTransformNumberRows    = addendNumberRows;
                addendTransformNumberColumns = addendNumberColumns;

                break;
            }

            case LazyTransform::ADJOINT: {
                addendTransformType          = MatOperation::CONJUGATE_TRANSPOSE;
                addendTransformNumberRows    = addendNumberColumns;
                addendTransformNumberColumns = addendNumberRows;

                break;
            }

            default: {
                assert(false);
                break;
            }
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

            matrixApi()->complexScaleAdd(
                MatMatrixMode::COLUMN_MAJOR,
                augendTransformType,
                addendTransformType,
                augendTransformNumberRows,
                augendTransformNumberColumns,
                reinterpret_cast<const MatComplex*>(&augendScalar),
                reinterpret_cast<const MatComplex*>(augendData),
                augendColumnSpacing,
                reinterpret_cast<const MatComplex*>(&addendScalar),
                reinterpret_cast<const MatComplex*>(addendData),
                addendColumnSpacing,
                reinterpret_cast<MatComplex*>(destinationData),
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


    MatrixComplex::Data* MatrixComplex::DenseData::add(
            const MatrixSparsePrivate<MatrixComplex::DenseData::Scalar>& addend,
            LazyTransform                                                augendtTransform,
            LazyTransform                                                addendTransform,
            const Complex&                                               augendScalar,
            const Complex&                                               addendScalar
        ) const {
        (void) addend; // FIXME
        (void) augendtTransform; // FIXME
        (void) addendTransform; // FIXME
        (void) augendScalar; // FIXME
        (void) addendScalar; // FIXME
        return nullptr; // FIXME
    }


    MatrixComplex::Data* MatrixComplex::DenseData::multiply(
            const MatrixDensePrivate<MatrixComplex::DenseData::Scalar>& multiplicand,
            MatrixComplex::DenseData::LazyTransform                     multiplierTransform,
            MatrixComplex::DenseData::LazyTransform                     multiplicandTransform,
            const Complex&                                              scalerMultiplier
        ) const {
        assert(multiplierTransform   != LazyTransform::CONJUGATE);
        assert(multiplicandTransform != LazyTransform::CONJUGATE);

        DenseData* result = nullptr;

        unsigned long multiplierRows      = numberRows();
        unsigned long multiplierColumns   = numberColumns();
        unsigned long multiplicandRows    = multiplicand.numberRows();
        unsigned long multiplicandColumns = multiplicand.numberColumns();

        if (multiplierRows != 0 && multiplierColumns != 0 && multiplicandRows != 0 && multiplicandColumns != 0) {
            MatOperation  multiplierTransformOp;
            unsigned long multiplierOpRows;
            unsigned long multiplierOpColumns;
            if (multiplierTransform == LazyTransform::TRANSPOSE) {
                multiplierOpRows      = multiplierColumns;
                multiplierOpColumns   = multiplierRows;
                multiplierTransformOp = MatOperation::TRANSPOSE;
            } else if (multiplierTransform == LazyTransform::ADJOINT) {
                multiplierOpRows      = multiplierColumns;
                multiplierOpColumns   = multiplierRows;
                multiplierTransformOp = MatOperation::CONJUGATE_TRANSPOSE;
            } else {
                multiplierOpRows      = multiplierRows;
                multiplierOpColumns   = multiplierColumns;
                multiplierTransformOp = MatOperation::NO_OPERATION;
            }

            MatOperation  multiplicandTransformOp;
            unsigned long multiplicandOpRows;
            unsigned long multiplicandOpColumns;
            if (multiplicandTransform == LazyTransform::TRANSPOSE) {
                multiplicandOpRows      = multiplicandColumns;
                multiplicandOpColumns   = multiplicandRows;
                multiplicandTransformOp = MatOperation::TRANSPOSE;
            } else if (multiplicandTransform == LazyTransform::ADJOINT) {
                multiplicandOpRows      = multiplicandColumns;
                multiplicandOpColumns   = multiplicandRows;
                multiplicandTransformOp = MatOperation::CONJUGATE_TRANSPOSE;
            } else {
                multiplicandOpRows      = multiplicandRows;
                multiplicandOpColumns   = multiplicandColumns;
                multiplicandTransformOp = MatOperation::NO_OPERATION;
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

                matrixApi()->blasComplexMultiplyAdd(
                    MatMatrixMode::COLUMN_MAJOR,
                    multiplierTransformOp,
                    multiplicandTransformOp,
                    multiplierOpRows,
                    multiplicandOpColumns,
                    multiplierOpColumns,
                    reinterpret_cast<const MatComplex*>(&scalerMultiplier),
                    reinterpret_cast<const MatComplex*>(multiplierData),
                    multiplierColumnSpacingInMemory,
                    reinterpret_cast<const MatComplex*>(multiplicandData),
                    multiplicandColumnSpacingInMemory,
                    reinterpret_cast<const MatComplex*>(&beta),
                    reinterpret_cast<MatComplex*>(resultData),
                    resultColumnSpacingInMemory
                );
            }
        } else {
            result = DenseData::create(0, 0);
        }

        return result;
    }


    MatrixComplex::Data* MatrixComplex::DenseData::multiply(
            const MatrixSparsePrivate<MatrixComplex::DenseData::Scalar>& multiplicand,
            MatrixComplex::DenseData::LazyTransform                      multiplierTransform,
            MatrixComplex::DenseData::LazyTransform                      multiplicandTransform,
            const Complex&                                               scalerMultiplier
        ) const {
        (void) multiplicand; // FIXME
        (void) multiplierTransform; // FIXME
        (void) multiplicandTransform; // FIXME
        (void) scalerMultiplier; // FIXME
        return nullptr; // FIXME
    }


    MatrixReal::Data* MatrixComplex::DenseData::real() const {
        return splitIntoParts(0);
    }


    MatrixReal::Data* MatrixComplex::DenseData::imag() const {
        return splitIntoParts(1);
    }


    bool MatrixComplex::DenseData::isEqualTo(const MatrixDensePrivate<MatrixComplex::DenseData::Scalar>& other) const {
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


    bool MatrixComplex::DenseData::isEqualTo(
            const MatrixSparsePrivate<MatrixComplex::DenseData::Scalar>& other
        ) const {
        (void) other; // FIXME
        assert(false); // FIXME
        return false;
    }


    MatrixComplex::Data* MatrixComplex::DenseData::applyTransformAndScaling(
            MatrixComplex::DenseData::LazyTransform transform,
            const Scalar&                           scaleFactor
        ) const {
        unsigned long inNumberRows    = numberRows();
        unsigned long inNumberColumns = numberColumns();

        MatOperation  transformType;
        unsigned long outNumberRows;
        unsigned long outNumberColumns;

        switch (transform) {
            case LazyTransform::NONE: {
                transformType    = MatOperation::NO_OPERATION;
                outNumberRows    = inNumberRows;
                outNumberColumns = inNumberColumns;

                break;
            }

            case LazyTransform::TRANSPOSE: {
                transformType    = MatOperation::TRANSPOSE;
                outNumberRows    = inNumberColumns;
                outNumberColumns = inNumberRows;

                break;
            }

            case LazyTransform::CONJUGATE: {
                transformType    = MatOperation::CONJUGATE;
                outNumberRows    = inNumberRows;
                outNumberColumns = inNumberColumns;

                break;
            }

            case LazyTransform::ADJOINT: {
                transformType    = MatOperation::CONJUGATE_TRANSPOSE;
                outNumberRows    = inNumberColumns;
                outNumberColumns = inNumberRows;

                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        DenseData*         result                   = createUninitialized(outNumberRows, outNumberColumns);
        const Scalar*      sourceData               = data();
        unsigned long long sourceColumnSpacing      = columnSpacingInMemory();
        Scalar*            destinationData          = result->data();
        unsigned long long destinationColumnSpacing = result->columnSpacingInMemory();

        matrixApi()->complexScaleCopy(
            MatMatrixMode::COLUMN_MAJOR,
            transformType,
            inNumberRows,
            inNumberColumns,
            reinterpret_cast<const MatComplex*>(&scaleFactor),
            reinterpret_cast<const MatComplex*>(sourceData),
            sourceColumnSpacing,
            reinterpret_cast<MatComplex*>(destinationData),
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


    int MatrixComplex::DenseData::relativeOrder(const MatrixComplex::Data& other) const {
        int result;

        if (other.matrixType() == MatrixType::DENSE) {
            const DenseData& denseOther = POLYMORPHIC_CAST<const DenseData&>(other);
            result = MatrixDensePrivateBase::relativeOrder(denseOther);
        } else {
            result = static_cast<int>(matrixType()) - static_cast<int>(other.matrixType());
        }

        return result;
    }


    long double MatrixComplex::DenseData::smallLimit() {
        if (currentSmallLimit < 0) {
            currentSmallLimit = matrixApi()->lapackDoubleMachineParameter(MatMachineParameter::SAFE_MINIMUM);
        }

        return currentSmallLimit;
    }


    long double MatrixComplex::DenseData::bigLimit() {
        if (currentBigLimit < 0) {
            currentBigLimit = 1.0L / smallLimit();
        }

        return currentBigLimit;
    }


    MatrixReal::DenseData* MatrixComplex::DenseData::splitIntoParts(unsigned offset) const {
        unsigned long numberRows    = MatrixComplex::DenseData::numberRows();
        unsigned long numberColumns = MatrixComplex::DenseData::numberColumns();

        MatrixReal::DenseData* result = MatrixReal::DenseData::createUninitialized(numberRows, numberColumns);

        unsigned long long destinationColumnSpacing = result->columnSpacingInMemory();
        unsigned long      residueInEntries         = static_cast<unsigned long>(destinationColumnSpacing - numberRows);

        if (residueInEntries > 0) {
            for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                const double* s = reinterpret_cast<const double*>(addressOf(0, columnIndex)) + offset;
                double*       d = reinterpret_cast<double*>(result->addressOf(0, columnIndex));

                matrixApi()->doublePack(numberRows, s, 2, d);
                memset(d + numberRows, 0, residueInEntries * sizeof(Scalar));
            }
        } else {
            for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                const double* s = reinterpret_cast<const double*>(addressOf(0, columnIndex)) + offset;
                double*       d = reinterpret_cast<double*>(result->addressOf(0, columnIndex));

                matrixApi()->doublePack(numberRows, s, 2, d);
            }
        }

        return result;
    }


    MatrixComplex::DenseData* MatrixComplex::DenseData::generatePivotMatrix(
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


    bool MatrixComplex::DenseData::calculateSchur(
            DenseData*& sData,
            DenseData*& qzData,
            double*&    scaleTerms,
            DenseData*& wData,
            MatInteger& ilo,
            MatInteger& ihi
        ) const {
        unsigned long numberRows = DenseData::numberRows();
        unsigned long tauSize    = std::max(1UL, numberRows - 1);
        MatComplex*   tau        = new MatComplex[tauSize];

        sData      = POLYMORPHIC_CAST<DenseData*>(clone());
        scaleTerms = new double[numberRows];
        wData      = DenseData::create(numberRows, 1);

        // Balance matrix terms to reduce errors.

        MatInteger info = matrixApi()->lapackComplexEigenBalance(
            MatMatrixMode::COLUMN_MAJOR,
            MatEigenBalanceJob::PERMUTED,
            numberRows,
            reinterpret_cast<MatComplex*>(sData->data()),
            sData->columnSpacingInMemory(),
            &ilo,
            &ihi,
            scaleTerms
        );

        assert(info == 0);

        // Calculate Hessenberg matrix form

        info = matrixApi()->lapackComplexUpperHessenberg(
            MatMatrixMode::COLUMN_MAJOR,
            numberRows,
            ilo,
            ihi,
            reinterpret_cast<MatComplex*>(sData->data()),
            sData->columnSpacingInMemory(),
            tau
        );

        assert(info == 0);

        // Generate unitary matrix that is associated with the Hessenberg matrix.

        qzData = POLYMORPHIC_CAST<DenseData*>(sData->clone());
        info = matrixApi()->lapackComplexUpperHessenbergQMatrix(
            MatMatrixMode::COLUMN_MAJOR,
            numberRows,
            ilo,
            ihi,
            reinterpret_cast<MatComplex*>(qzData->data()),
            qzData->columnSpacingInMemory(),
            tau
        );

        delete[] tau;

        assert(info == 0);

        // Compute Schur matrix with Eigenvalues

        info = matrixApi()->lapackComplexSchur(
            MatMatrixMode::COLUMN_MAJOR,
            MatSchurMode::SCHUR_WITH_Q_MATRIX,
            numberRows,
            ilo,
            ihi,
            reinterpret_cast<MatComplex*>(sData->data()),
            sData->columnSpacingInMemory(),
            reinterpret_cast<MatComplex*>(wData->data()),
            reinterpret_cast<MatComplex*>(qzData->data()),
            qzData->columnSpacingInMemory()
        );

        return info == 0;
    }


    MatrixComplex::DenseData* MatrixComplex::DenseData::calculateDft(bool forward) const {
        DenseData*    result;
        unsigned long numberRows = DenseData::numberRows();
        unsigned long numberColumns = DenseData::numberColumns();

        if (numberRows > 0 && numberColumns > 0) {
            result = DenseData::createUninitialized(numberRows, numberColumns);

            MatInteger status;
            if (forward) {
                status = matrixApi()->complexDft(
                    MatMatrixMode::COLUMN_MAJOR,
                    numberRows,
                    numberColumns,
                    reinterpret_cast<const MatComplex*>(data()),
                    columnSpacingInMemory(),
                    reinterpret_cast<MatComplex*>(result->data()),
                    result->columnSpacingInMemory()
                );
            } else {
                status = matrixApi()->complexInverseDft(
                    MatMatrixMode::COLUMN_MAJOR,
                    numberRows,
                    numberColumns,
                    reinterpret_cast<const MatComplex*>(data()),
                    columnSpacingInMemory(),
                    reinterpret_cast<MatComplex*>(result->data()),
                    result->columnSpacingInMemory()
                );
            }

            assert(status == 0); // Means DFT failed.
        } else {
            result = DenseData::create(0, 0);
        }

        return result;
    }


    MatrixComplex::DenseData* MatrixComplex::DenseData::multiplyRows(
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

                matrixApi()->complexDotProduct(
                    numberRows,
                    reinterpret_cast<const MatComplex*>(source),
                    reinterpret_cast<const MatComplex*>(rowScaleFactors),
                    reinterpret_cast<MatComplex*>(destination)
                );

                std::memset(destination + columnSizeBytes, 0, residueSizeBytes);
            }
        } else {
            for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                matrixApi()->complexDotProduct(
                    numberRows,
                    reinterpret_cast<const MatComplex*>(sourceMatrix.addressOf(0, columnIndex)),
                    reinterpret_cast<const MatComplex*>(rowScaleFactors),
                    reinterpret_cast<MatComplex*>(result->addressOf(0, columnIndex))
                );
            }
        }

        return result;
    }
}
