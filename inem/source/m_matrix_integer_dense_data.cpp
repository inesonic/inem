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
* This file implements the \ref M::MatrixInteger::DenseData class.
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

#include <boost/algorithm/string.hpp>

#include "m_api_types.h"
#include "m_intrinsic_types.h"
#include "m_basic_functions.h"
#include "model_exceptions.h"
#include "model_range.h"
#include "m_per_thread.h"
#include "m_compiler_abstraction.h"
#include "m_matrix_dense_private.h"
#include "m_matrix_sparse_private.h"
#include "m_matrix_boolean.h"
#include "m_matrix_boolean_dense_data.h"
#include "m_matrix_real.h"
#include "m_matrix_real_dense_data.h"
#include "m_matrix_complex.h"
#include "m_matrix_complex_dense_data.h"
#include "m_matrix_integer.h"
#include "m_matrix_integer_data.h"
#include "m_matrix_integer_dense_data.h"

namespace M {
    MatrixInteger::DenseData::DenseData(
            unsigned long      newNumberRows,
            unsigned long      newNumberColumns,
            unsigned long long newAllocationInBytes,
            void*              rawDataBaseAddress
        ):MatrixDensePrivate<MatrixInteger::DenseData::Scalar>(
            newNumberRows,
            newNumberColumns,
            newAllocationInBytes,
            rawDataBaseAddress
        ) {
    }


    MatrixInteger::DenseData::DenseData(
            const MatrixDensePrivate<MatrixInteger::DenseData::Scalar>& other,
            unsigned long long                                          newAllocationInBytes,
            void*                                                       rawDataBaseAddress
        ):MatrixDensePrivate<MatrixInteger::DenseData::Scalar>(
            other,
            newAllocationInBytes,
            rawDataBaseAddress
        ) {}


    MatrixInteger::DenseData::DenseData(
            const MatrixDensePrivate<MatrixInteger::DenseData::Scalar>& other,
            unsigned long                                               newNumberRows,
            unsigned long                                               newNumberColumns,
            unsigned long long                                          newAllocationInBytes,
            void*                                                       rawDataBaseAddress
        ):MatrixDensePrivate<MatrixInteger::DenseData::Scalar>(
            other,
            newNumberRows,
            newNumberColumns,
            newAllocationInBytes,
            rawDataBaseAddress
        ) {}


    MatrixInteger::DenseData::~DenseData() {}


    MatrixInteger::DenseData* MatrixInteger::DenseData::create(const MatrixBoolean::DenseData& other) {
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


    MatrixInteger::DenseData* MatrixInteger::DenseData::create(const MatrixReal::DenseData& other) {
        unsigned long numberRows    = other.numberRows();
        unsigned long numberColumns = other.numberColumns();

        DenseData*         result                    = createUninitialized(numberRows, numberColumns);
        Scalar*            d                         = result->data();
        unsigned long long destColumnSpacing         = result->columnSpacingInMemory();
        unsigned long long destResidueEntriesToClear = destColumnSpacing - numberRows;

        const MatrixReal::Scalar* s                 = other.data();
        unsigned long long        srcColumnSpacing  = other.columnSpacingInMemory();
        unsigned long long        srcResidueEntries = srcColumnSpacing - numberRows;

        for (unsigned long long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
            for (unsigned long long rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                MatrixReal::Scalar v = *s;

                MatrixReal::Scalar wholePart;
                if (std::modf(v, &wholePart) != 0.0) {
                    internalTriggerTypeConversionError(M::ValueType::MATRIX_REAL, M::ValueType::MATRIX_INTEGER);
                }

                *d = static_cast<Scalar>(v);

                ++s;
                ++d;
            }

            s += srcResidueEntries;

            if (destResidueEntriesToClear != 0) {
                std::memset(d, 0, sizeof(Scalar) * destResidueEntriesToClear);
                d += destResidueEntriesToClear;
            }
        }

        return result;
    }


    MatrixInteger::DenseData* MatrixInteger::DenseData::create(const MatrixComplex::DenseData& other) {
        unsigned long numberRows    = other.numberRows();
        unsigned long numberColumns = other.numberColumns();

        DenseData*         result                    = createUninitialized(numberRows, numberColumns);
        Scalar*            d                         = result->data();
        unsigned long long destColumnSpacing         = result->columnSpacingInMemory();
        unsigned long long destResidueEntriesToClear = destColumnSpacing - numberRows;

        const MatrixComplex::Scalar* s                 = other.data();
        unsigned long long           srcColumnSpacing  = other.columnSpacingInMemory();
        unsigned long long           srcResidueEntries = srcColumnSpacing - numberRows;

        if (destResidueEntriesToClear != 0) {
            for (unsigned long long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                for (unsigned long long rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                    MatrixComplex::Scalar v = *s;

                    MatrixComplex::Scalar::T wholePart = MatrixComplex::Scalar::T(0);
                    if (v.imag() != 0 || std::modf(v.real(), &wholePart) != 0.0) {
                        internalTriggerTypeConversionError(M::ValueType::MATRIX_COMPLEX, M::ValueType::MATRIX_INTEGER);
                    }

                    *d = static_cast<Scalar>(wholePart);

                    ++s;
                    ++d;
                }

                std::memset(d, 0, sizeof(Scalar) * destResidueEntriesToClear);

                s += srcResidueEntries;
                d += destResidueEntriesToClear;
            }
        } else {
            for (unsigned long long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                for (unsigned long long rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                    MatrixComplex::Scalar v = *s;

                    MatrixComplex::Scalar::T wholePart = MatrixComplex::Scalar::T(0);
                    if (v.imag() != 0 || std::modf(v.real(), &wholePart) != 0.0) {
                        internalTriggerTypeConversionError(M::ValueType::MATRIX_COMPLEX, M::ValueType::MATRIX_INTEGER);
                    }

                    *d = static_cast<Scalar>(wholePart);

                    ++s;
                    ++d;
                }

                s += srcResidueEntries;
            }
        }

        return result;
    }


    MatrixInteger::DenseData* MatrixInteger::DenseData::identity(Index numberRows, Index numberColumns) {
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


    MatrixInteger::DenseData* MatrixInteger::DenseData::ones(unsigned long numberRows, unsigned long numberColumns) {
        DenseData* result = createUninitialized(numberRows, numberColumns);

        unsigned long long columnSpacing = result->columnSpacingInMemory();
        if (columnSpacing == numberRows) {
            matrixApi()->integerFill(
                numberRows * numberColumns,
                1,
                reinterpret_cast<MatInteger*>(result->data())
            );
        } else {
            unsigned long long residuePerColumn     = columnSpacing - numberRows;
            unsigned           residueLengthInBytes = residuePerColumn * sizeof(Scalar);
            Scalar*            d                    = result->data();

            for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                matrixApi()->integerFill(numberRows, 1, reinterpret_cast<MatInteger*>(d));
                d += numberRows;
                std::memset(d, 0, residueLengthInBytes);
                d += residuePerColumn;
            }
        }

        return result;
    }


    bool MatrixInteger::DenseData::toFile(const char* filename, Model::DataFileFormat fileFormat) const {
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
                            f << "\t" << at(rowIndex, columnIndex);
                        } else {
                            f << at(rowIndex, columnIndex);
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
                buffer[i++] = 'Z';

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
                    while (columnIndex < currentNumberColumns) {
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

                if (i != 0) {
                    f.write(reinterpret_cast<const char*>(buffer), i);
                    if (!f) {
                        delete[] buffer;
                        buffer = nullptr;

                        internalTriggerFileWriteError(filename);
                        success = false;
                    }
                }

                if (success) {
                    f.close();
                    if (!f) {
                        internalTriggerFileCloseError(filename);
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
            }
        }

        return success;
    }


    MatrixInteger::DenseData* MatrixInteger::DenseData::fromFile(const char* filename) {
        bool                  success    = true; // Note: For consistency, we set success = false in all cases.
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

                    std::vector<std::vector<Scalar>> matrixData;
                    unsigned long                    numberColumns = 0;

                    while (success && f.peek() != EOF) { // IMHO, C++ stream are broken when testing against EOF.
                        std::vector<Scalar> rowArray;

                        std::string line;
                        std::getline(f, line);
                        if (!f) {
                            internalTriggerFileReadError(filename);
                            success= false;
                        }

                        std::string                 value;
                        std::string::const_iterator it  = line.cbegin();
                        std::string::const_iterator end = line.cend();

                        while (success && it != end) {
                            char c = *it;
                            if (c == '\t' || c == ' ' || c == ',' || c == ';' || c == '|' || c == ':') {
                                bool    ok;
                                Scalar rv = convertString(value, &ok);

                                if (!ok) {
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
                            Scalar rv = convertString(value, &success);

                            if (!success) {
                                internalTriggerFileReadError(filename);
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
                            if (header[7] != 'Z') {
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
                                unsigned long long bufferSize         = 8* numberCoefficients;
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
            result  = DenseData::create(0, 0);
        }

        return result;
    }


    MatrixInteger::Data* MatrixInteger::DenseData::combineLeftToRight(
            const MatrixDensePrivate<MatrixInteger::DenseData::Scalar>& other
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


    MatrixInteger::Data* MatrixInteger::DenseData::combineLeftToRight(
            const MatrixSparsePrivate<MatrixInteger::DenseData::Scalar>& other
        ) const {
        (void) other; // FIXME
        return nullptr; // FIXME
    }


    MatrixInteger::Data* MatrixInteger::DenseData::combineTopToBottom(
            const MatrixDensePrivate<MatrixInteger::DenseData::Scalar>& other
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


    MatrixInteger::Data* MatrixInteger::DenseData::combineTopToBottom(
            const MatrixSparsePrivate<MatrixInteger::DenseData::Scalar>& other
        ) const {
        (void) other; // FIXME
        return nullptr; // FIXME
    }


    MatrixInteger::Data* MatrixInteger::DenseData::columnReverse() const {
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


    MatrixInteger::Data* MatrixInteger::DenseData::rowReverse() const {
        unsigned long      numberRows        = DenseData::numberRows();
        unsigned long      numberColumns     = DenseData::numberColumns();
        DenseData*         result            = createUninitialized(numberRows, numberColumns);
        unsigned long long columnSize        = columnSpacingInMemory();

        for (unsigned long sourceRowIndex=0 ; sourceRowIndex<numberRows ; ++sourceRowIndex) {
            unsigned long  destinationRowIndex = numberRows - sourceRowIndex - 1;
            const Integer* source              = reinterpret_cast<const Integer*>(addressOf(sourceRowIndex, 0));
            Integer*       destination         = reinterpret_cast<Integer*>(result->addressOf(destinationRowIndex, 0));

            // FIXME: vectorize, if possible.
            for (unsigned i=0 ; i<numberColumns ; ++i) {
                *destination = *source;
                source      += columnSize;
                destination += columnSize;
            }
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


    MatrixInteger::Data* MatrixInteger::DenseData::resizeTo(
            unsigned long newNumberRows,
            unsigned long newNumberColumns,
            bool          alwaysReallocate
        ) {
        MatrixInteger::Data* result = this;
        if (alwaysReallocate || !resizeInPlace(newNumberRows, newNumberColumns)) {
            result = MatrixDensePrivate<MatrixInteger::DenseData::Scalar>::create<DenseData>(
                *this,
                newNumberRows,
                newNumberColumns
            );
        }

        return result;
    }


    bool MatrixInteger::DenseData::resizeInPlaceTo(unsigned long newNumberRows, unsigned long newNumberColumns) {
        return MatrixDensePrivate<MatrixInteger::DenseData::Scalar>::resizeInPlace(newNumberRows, newNumberColumns);
    }


    MatrixInteger::Data* MatrixInteger::DenseData::hadamard(
            const MatrixDensePrivate<MatrixInteger::DenseData::Scalar>& other
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

                    vectorDotProduct(
                        columnSizeInEntries,
                        1,
                        a,
                        1,
                        b,
                        1,
                        y,
                        1
                    );

                    std::memset(reinterpret_cast<std::uint8_t*>(y) + columnDataSizeInBytes, 0, residueSizeInBytes);
                }
            } else {
                for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                    const Scalar* a = reinterpret_cast<const Scalar*>(addressOf(0, columnIndex));
                    const Scalar* b = reinterpret_cast<const Scalar*>(other.addressOf(0, columnIndex));
                    Scalar*       y = reinterpret_cast<Scalar*>(result->addressOf(0, columnIndex));

                    vectorDotProduct(
                        columnSizeInEntries,
                        1,
                        a,
                        1,
                        b,
                        1,
                        y,
                        1
                    );
                }
            }
        }

        return result;
    }


    MatrixInteger::Data* MatrixInteger::DenseData::hadamard(
            const MatrixSparsePrivate<MatrixInteger::DenseData::Scalar>& other
        ) const {
        (void) other; // FIXME
        return nullptr; // FIXME
    }


    MatrixInteger::Data* MatrixInteger::DenseData::kronecker(
            const MatrixDensePrivate<MatrixInteger::DenseData::Scalar>& other,
            bool                                                        otherTranspose,
            MatrixInteger::DenseData::Scalar                            scalarMultiplier
        ) const {
        unsigned long multiplierNumberRows        = numberRows();
        unsigned long multiplierNumberColumns     = numberColumns();
        unsigned long multiplicandInNumberRows    = other.numberRows();
        unsigned long multiplicandInNumberColumns = other.numberColumns();

        unsigned long multiplicandOutNumberRows;
        unsigned long multiplicandOutNumberColumns;

        if (otherTranspose) {
            multiplicandOutNumberRows    = multiplicandInNumberColumns;
            multiplicandOutNumberColumns = multiplicandInNumberRows;
        } else {
            multiplicandOutNumberRows    = multiplicandInNumberRows;
            multiplicandOutNumberColumns = multiplicandInNumberColumns;
        }

        unsigned long productNumberRows    = multiplierNumberRows * multiplicandOutNumberRows;
        unsigned long productNumberColumns = multiplierNumberColumns * multiplicandOutNumberColumns;

        DenseData*    result           = createUninitialized(productNumberRows, productNumberColumns);
        const Scalar* multiplicandData = other.data();

        unsigned long multiplicandColumnSizeInEntries = other.columnSpacingInMemory();
        unsigned long productColumnSizeInEntries      = result->columnSpacingInMemory();

        for (unsigned long multiplierColumn=0 ; multiplierColumn<multiplierNumberColumns ; ++multiplierColumn) {
            unsigned long productLeftColumn = multiplierColumn * multiplicandOutNumberColumns;
            for (unsigned long multiplierRow=0 ; multiplierRow<multiplierNumberRows ; ++multiplierRow) {
                unsigned long  productTopRow         = multiplierRow * multiplicandOutNumberRows;
                Scalar         multiplierCoefficient = at(multiplierRow, multiplierColumn) * scalarMultiplier;

                Scalar* resultData = reinterpret_cast<Scalar*>(result->addressOf(productTopRow, productLeftColumn));

                matrixScalarProduct(
                    multiplicandOutNumberRows,
                    multiplicandOutNumberColumns,
                    otherTranspose,
                    multiplierCoefficient,
                    multiplicandData,
                    multiplicandColumnSizeInEntries,
                    resultData,
                    productColumnSizeInEntries
                );
            }
        }

        unsigned residueSizeInEntries = static_cast<unsigned>(productColumnSizeInEntries - productNumberRows);
        if (residueSizeInEntries > 0) {
            unsigned long long residueSizeInBytes = residueSizeInEntries * sizeof(Scalar);
            for (unsigned long columnIndex=0 ; columnIndex<productNumberColumns ; ++columnIndex) {
                std::uint8_t* zeroBase = static_cast<std::uint8_t*>(result->addressOf(productNumberRows, columnIndex));
                std::memset(zeroBase, 0, residueSizeInBytes);
            }
        }

        return result;
    }


    MatrixInteger::Data* MatrixInteger::DenseData::kronecker(
            const MatrixSparsePrivate<MatrixInteger::DenseData::Scalar>& other,
            bool                                                         otherTranspose,
            MatrixInteger::DenseData::Scalar                             scalarMultiplier
        ) const {
        (void) other; // FIXME
        (void) otherTranspose; // FIXME
        (void) scalarMultiplier; // FIXME
        return nullptr; // FIXME
    }


    MatrixInteger::DenseData* MatrixInteger::DenseData::randomInteger64(
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
                Scalar* residueBase = reinterpret_cast<Scalar*>(result->addressOf(numberRows, columnIndex));
                memset(residueBase, 0, residueEntriesToClear * sizeof(Scalar));
            }
        }

        return result;
    }


    MatrixInteger::DenseData* MatrixInteger::DenseData::randomInteger32(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns
        ) {
        DenseData*         result                   = createUninitialized(numberRows, numberColumns);
        Scalar*            d                        = result->data();
        unsigned long long columnSpacing            = result->columnSpacingInMemory();
        unsigned long long matrixSizeInCoefficients = columnSpacing * numberColumns;
        unsigned long long residueEntriesToClear    = columnSpacing - numberRows;
        unsigned long long numberRandomValues       = matrixSizeInCoefficients * sizeof(Scalar) / 8;
        unsigned long long numberRandom64Bit        = (numberRandomValues + 1) / 2;

        pt.fillArray(d, numberRandom64Bit);
        Scalar* d1 = d;
        Scalar* d2 = d + numberRandom64Bit;
        unsigned long long numberToAdd = matrixSizeInCoefficients - numberRandom64Bit;
        for (unsigned i=0 ; i<numberToAdd ; ++i) {
            Scalar v = d1[i];
            d2[i] = (v >> 32) & 0x00000000FFFFFFFFULL;
            d1[i] = v & 0x00000000FFFFFFFFULL;
        }

        Scalar v = d[numberRandom64Bit - 1];
        if ((v & 0xFFFFFFFF00000000ULL) != 0) {
            d[numberRandom64Bit - 1] = v & 0x00000000FFFFFFFFULL;
        }

        if (residueEntriesToClear > 0) {
            for (Integer columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                Scalar* residueBase = reinterpret_cast<Scalar*>(result->addressOf(numberRows, columnIndex));
                memset(residueBase, 0, residueEntriesToClear * sizeof(Scalar));
            }
        }

        return result;
    }


    MatrixInteger::DenseData* MatrixInteger::DenseData::randomPoisson(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns,
            Real       rate
        ) {
        DenseData*         result                   = createUninitialized(numberRows, numberColumns);
        Scalar*            d                        = result->data();
        unsigned long long columnSpacing            = result->columnSpacingInMemory();
        unsigned long long matrixSizeInCoefficients = columnSpacing * numberColumns;
        unsigned long long residueEntriesToClear    = columnSpacing - numberRows;

        if (residueEntriesToClear > 0) {
            Scalar* p = d;
            for (Integer columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                for (Integer rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                    *p = pt.randomPoisson(rate);
                    ++p;
                }
            }

            std::memset(p, 0, sizeof(Scalar) * residueEntriesToClear);
            p += residueEntriesToClear;
        } else {
            Scalar* p   = d;
            Scalar* end = d + matrixSizeInCoefficients;
            while (p != end) {
                *p = pt.randomPoisson(rate);
                ++p;
            }
        }

        return result;
    }


    MatrixInteger::DenseData* MatrixInteger::DenseData::randomBinomial(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns,
            Integer    n,
            Real       p
        ) {
        DenseData*         result                   = createUninitialized(numberRows, numberColumns);
        Scalar*            d                        = result->data();
        unsigned long long columnSpacing            = result->columnSpacingInMemory();
        unsigned long long matrixSizeInCoefficients = columnSpacing * numberColumns;
        unsigned long long residueEntriesToClear    = columnSpacing - numberRows;

        if (residueEntriesToClear > 0) {
            Scalar* ptr = d;
            for (Integer columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                for (Integer rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                    *ptr = pt.randomBinomial(n, p);
                    ++ptr;
                }
            }

            std::memset(ptr, 0, sizeof(Scalar) * residueEntriesToClear);
            ptr += residueEntriesToClear;
        } else {
            Scalar* ptr = d;
            Scalar* end = d + matrixSizeInCoefficients;
            while (ptr != end) {
                *ptr = pt.randomBinomial(n, p);
                ++ptr;
            }
        }

        return result;
    }


    MatrixInteger::DenseData* MatrixInteger::DenseData::randomGeometric(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns,
            Real       p
        ) {
        MatrixReal::DenseData* working = MatrixReal::DenseData::randomExclusiveInclusive(
            pt,
            numberRows,
            numberColumns
        );

        MatrixReal::Scalar* workingData                     = working->data();
        unsigned long long  workingColumnSpacing            = working->columnSpacingInMemory();
        unsigned long long  workingResidueEntriesToClear    = workingColumnSpacing - numberRows;
        unsigned long long  workingMatrixSizeInCoefficients = (
              workingColumnSpacing * numberColumns
            - workingResidueEntriesToClear
        );

        if (workingResidueEntriesToClear == 0) {
            matrixApi()->doubleLog(workingMatrixSizeInCoefficients, workingData, workingData);
        } else {
            MatrixReal::Scalar* columnBase = workingData;
            for (Integer columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                matrixApi()->doubleLog(numberRows, columnBase, columnBase);
                columnBase += workingColumnSpacing;
            }
        }

        matrixApi()->doubleVectorScalarMultiplyAdd(
            workingMatrixSizeInCoefficients,
            workingData,
            1.0 / std::log(1.0 - p),
            1.0,
            workingData
        );

        DenseData* result = createUninitialized(numberRows, numberColumns);
        Scalar*    data   = result->data();

        assert(result->columnSpacingInMemory() == workingColumnSpacing);

        Scalar*             d   = data;
        MatrixReal::Scalar* s   = workingData;
        Scalar*             end = data + workingMatrixSizeInCoefficients;

        while (d != end) {
            *d = static_cast<Integer>(*s);
            ++d;
            ++s;
        }

        MatrixReal::DenseData::destroy(working);

        return result;
    }


    MatrixInteger::Data* MatrixInteger::DenseData::add(
            const MatrixDensePrivate<MatrixInteger::DenseData::Scalar>& addend,
            bool                                                        augendTranspose,
            bool                                                        addendTranspose,
            MatrixInteger::DenseData::Scalar                            augendScalar,
            MatrixInteger::DenseData::Scalar                            addendScalar
        ) const {
        DenseData* result = nullptr;

        unsigned long      augendNumberRows    = numberRows();
        unsigned long      augendNumberColumns = numberColumns();
        unsigned long long augendColumnSpacing = columnSpacingInMemory();

        unsigned long      augendTransformNumberRows;
        unsigned long      augendTransformNumberColumns;
        unsigned long long augendTermIncrement;
        unsigned long long augendColumnIncrement;

        if (augendTranspose) {
            augendTransformNumberRows    = augendNumberColumns;
            augendTransformNumberColumns = augendNumberRows;
            augendTermIncrement          = augendColumnSpacing;
            augendColumnIncrement        = 1;
        } else {
            augendTransformNumberRows    = augendNumberRows;
            augendTransformNumberColumns = augendNumberColumns;
            augendTermIncrement          = 1;
            augendColumnIncrement        = augendColumnSpacing;
        }

        unsigned long      addendNumberRows    = addend.numberRows();
        unsigned long      addendNumberColumns = addend.numberColumns();
        unsigned long long addendColumnSpacing = addend.columnSpacingInMemory();

        unsigned long      addendTransformNumberRows;
        unsigned long      addendTransformNumberColumns;
        unsigned long long addendTermIncrement;
        unsigned long long addendColumnIncrement;

        if (addendTranspose) {
            addendTransformNumberRows    = addendNumberColumns;
            addendTransformNumberColumns = addendNumberRows;
            addendTermIncrement          = addendColumnSpacing;
            addendColumnIncrement        = 1;
        } else {
            addendTransformNumberRows    = addendNumberRows;
            addendTransformNumberColumns = addendNumberColumns;
            addendTermIncrement          = 1;
            addendColumnIncrement        = addendColumnSpacing;
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
            const Scalar*      addendData               = addend.data();
            Scalar*            destinationData          = result->data();
            unsigned long long destinationColumnSpacing = result->columnSpacingInMemory();

            for (unsigned columnIndex=0 ; columnIndex<augendTransformNumberColumns ; ++columnIndex) {
                vectorAdd(
                    augendTransformNumberRows,
                    augendData,
                    augendTermIncrement,
                    augendScalar,
                    addendData,
                    addendTermIncrement,
                    addendScalar,
                    destinationData,
                    1
                );

                augendData      += augendColumnIncrement;
                addendData      += addendColumnIncrement;
                destinationData += destinationColumnSpacing;
            }

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


    MatrixInteger::Data* MatrixInteger::DenseData::add(
            const MatrixSparsePrivate<MatrixInteger::DenseData::Scalar>& addend,
            bool                                                         augendtTranspose,
            bool                                                         addendTranspose,
            MatrixInteger::DenseData::Scalar                             augendScalar,
            MatrixInteger::DenseData::Scalar                             addendScalar
        ) const {
        (void) addend; // FIXME
        (void) augendtTranspose; // FIXME
        (void) addendTranspose; // FIXME
        (void) augendScalar; // FIXME
        (void) addendScalar; // FIXME
        return nullptr; // FIXME
    }


    MatrixInteger::Data* MatrixInteger::DenseData::multiply(
            const MatrixDensePrivate<MatrixInteger::DenseData::Scalar>& multiplicand,
            bool                                                        multiplierTranspose,
            bool                                                        multiplicandTranspose,
            MatrixInteger::DenseData::Scalar                            scalarMultiplier
        ) const {
        DenseData* result = nullptr;

        unsigned long multiplierRows      = numberRows();
        unsigned long multiplierColumns   = numberColumns();
        unsigned long multiplicandRows    = multiplicand.numberRows();
        unsigned long multiplicandColumns = multiplicand.numberColumns();

        if (multiplierRows != 0 && multiplierColumns != 0 && multiplicandRows != 0 && multiplicandColumns != 0) {
            unsigned long long multiplierColumnSpacingInMemory   = columnSpacingInMemory();

            unsigned long      multiplierOutputRows;
            unsigned long      multiplierOutputColumns;
            unsigned long long multiplierRowIncrement;
            unsigned long long multiplierColumnIncrement;

            if (multiplierTranspose) {
                multiplierOutputRows      = multiplierColumns;
                multiplierOutputColumns   = multiplierRows;
                multiplierRowIncrement    = multiplierColumnSpacingInMemory;
                multiplierColumnIncrement = 1;
            } else {
                multiplierOutputRows      = multiplierRows;
                multiplierOutputColumns   = multiplierColumns;
                multiplierRowIncrement    = 1;
                multiplierColumnIncrement = multiplierColumnSpacingInMemory;
            }

            unsigned long long multiplicandColumnSpacingInMemory = multiplicand.columnSpacingInMemory();

            unsigned long      multiplicandOutputRows;
            unsigned long      multiplicandOutputColumns;
            unsigned long long multiplicandRowIncrement;
            unsigned long long multiplicandColumnIncrement;

            if (multiplicandTranspose) {
                multiplicandOutputRows      = multiplicandColumns;
                multiplicandOutputColumns   = multiplicandRows;
                multiplicandRowIncrement    = multiplicandColumnSpacingInMemory;
                multiplicandColumnIncrement = 1;
            } else {
                multiplicandOutputRows      = multiplicandRows;
                multiplicandOutputColumns   = multiplicandColumns;
                multiplicandRowIncrement    = 1;
                multiplicandColumnIncrement = multiplicandColumnSpacingInMemory;
            }

            if (multiplierOutputColumns != multiplicandOutputRows) {
                throw Model::IncompatibleMatrixDimensions(
                    multiplierOutputRows,
                    multiplierOutputColumns,
                    multiplicandOutputRows,
                    multiplicandOutputColumns
                );
            } else {
                result = DenseData::create(multiplierOutputRows, multiplicandOutputColumns);

                const Scalar* multiplierBase   = data();
                const Scalar* multiplicandBase = multiplicand.data();

                // TODO: Consider Strassen method
                // TODO: Consider using SIMD instructions for inner loop.
                const Scalar*      multiplicandData = multiplicandBase;
                Scalar*            y                = result->data();
                unsigned long long resultColumnSize = result->columnSpacingInMemory();
                unsigned           residue          = static_cast<unsigned>(resultColumnSize - multiplierOutputRows);
                unsigned           residueInBytes   = residue * sizeof(Scalar);

                for (unsigned columnIndex=0 ; columnIndex<multiplicandOutputColumns ; ++columnIndex) {
                    const Scalar* multiplierData = multiplierBase;
                    for (unsigned rowIndex=0 ; rowIndex<multiplierOutputRows ; ++rowIndex) {
                        Integer       sum = 0;
                        const Scalar* a   = multiplierData;
                        const Scalar* b   = multiplicandData;

                        for (unsigned termIndex=0 ; termIndex<multiplierOutputColumns ; ++termIndex) {
                            const Scalar va = *a;
                            const Scalar vb = *b;
                            sum += va * vb * scalarMultiplier;
                            a += multiplierColumnIncrement;
                            b += multiplicandRowIncrement;
                        }

                        (*y) = sum;
                        ++y;

                        multiplierData += multiplierRowIncrement;
                    }

                    if (residue > 0) {
                        std::memset(y, 0, residueInBytes);
                        y += residue;
                    }

                    multiplicandData += multiplicandColumnIncrement;
                }
            }
        } else {
            result = DenseData::create(0, 0);
        }

        return result;
    }


    MatrixInteger::Data* MatrixInteger::DenseData::multiply(
            const MatrixSparsePrivate<MatrixInteger::DenseData::Scalar>& multiplicand,
            bool                                                         multiplierTranspose,
            bool                                                         multiplicandTranspose,
            MatrixInteger::DenseData::Scalar                             scalerMultiplier
        ) const {
        (void) multiplicand; // FIXME
        (void) multiplierTranspose; // FIXME
        (void) multiplicandTranspose; // FIXME
        (void) scalerMultiplier; // FIXME
        return nullptr; // FIXME
    }


    bool MatrixInteger::DenseData::isEqualTo(const MatrixDensePrivate<MatrixInteger::DenseData::Scalar>& other) const {
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


    MatrixInteger::Data* MatrixInteger::DenseData::divide(
            bool    transpose,
            Integer scalarMultiplier,
            Integer scalarDivisor
        ) const {
        unsigned long      numberOutputRows;
        unsigned long      numberOutputColumns;
        unsigned long long inputColumnIncrement;
        unsigned long long inputRowIncrement;

        if (transpose) {
            numberOutputRows     = numberColumns();
            numberOutputColumns  = numberRows();
            inputColumnIncrement = 1;
            inputRowIncrement    = columnSpacingInMemory();
        } else {
            numberOutputRows     = numberRows();
            numberOutputColumns  = numberColumns();
            inputColumnIncrement = columnSpacingInMemory();
            inputRowIncrement    = 1;
        }

        DenseData*         result        = DenseData::createUninitialized(numberOutputRows, numberOutputColumns);
        unsigned           residue       = static_cast<unsigned>(result->columnSpacingInMemory() - numberOutputRows);
        Scalar*            y             = result->data();
        unsigned long long columnSpacing = result->columnSpacingInMemory();
        const Scalar*      a             = data();

        if (residue > 0) {
            unsigned residueInBytes = residue * sizeof(Scalar);
            for (unsigned long outputColumnIndex=0 ; outputColumnIndex<numberOutputColumns ; ++outputColumnIndex) {
                vectorScalarQuotient(numberOutputRows, scalarMultiplier, scalarDivisor, a, inputRowIncrement, y, 1);

                std::memset(y + numberOutputRows, 0, residueInBytes);

                a += inputColumnIncrement;
                y += columnSpacing;
            }
        } else {
            for (unsigned long outputColumnIndex=0 ; outputColumnIndex<numberOutputColumns ; ++outputColumnIndex) {
                vectorScalarQuotient(numberOutputRows, scalarMultiplier, scalarDivisor, a, inputRowIncrement, y, 1);

                a += inputColumnIncrement;
                y += columnSpacing;
            }
        }

        return result;
    }


    bool MatrixInteger::DenseData::isEqualTo(const MatrixSparsePrivate<MatrixInteger::DenseData::Scalar>& other) const {
        (void) other; // FIXME
        assert(false); // FIXME
        return false;
    }


    MatrixInteger::Data* MatrixInteger::DenseData::applyTransformAndScaling(
            bool                                    transpose,
            MatrixInteger::DenseData::Scalar        scaleFactor
        ) const {
        unsigned long inNumberRows    = numberRows();
        unsigned long inNumberColumns = numberColumns();

        unsigned long long sourceColumnSpacing = columnSpacingInMemory();

        unsigned long      outNumberRows;
        unsigned long      outNumberColumns;
        unsigned long long sourceRowIncrement;
        unsigned long long sourceColumnIncrement;

        if (transpose) {
            outNumberRows         = inNumberColumns;
            outNumberColumns      = inNumberRows;
            sourceRowIncrement    = sourceColumnSpacing;
            sourceColumnIncrement = 1;
        } else {
            outNumberRows         = inNumberRows;
            outNumberColumns      = inNumberColumns;
            sourceRowIncrement    = 1;
            sourceColumnIncrement = sourceColumnSpacing;
        }

        DenseData*         result                   = createUninitialized(outNumberRows, outNumberColumns);
        const Scalar*      sourceData               = data();
        Scalar*            destinationData          = result->data();
        unsigned long long destinationColumnSpacing = result->columnSpacingInMemory();

        unsigned long long residueRows = destinationColumnSpacing - outNumberRows;
        if (residueRows > 0) {
            unsigned residueSizeInBytes = static_cast<unsigned>(sizeof(Scalar) * residueRows);

            for (unsigned long columnIndex=0; columnIndex<outNumberColumns ; ++columnIndex) {
                vectorScalarProduct(outNumberRows, scaleFactor, sourceData, sourceRowIncrement, destinationData, 1);
                std::memset(destinationData + outNumberRows, 0, residueSizeInBytes);

                sourceData      += sourceColumnIncrement;
                destinationData += destinationColumnSpacing;
            }
        } else {
            for (unsigned long columnIndex=0; columnIndex<outNumberColumns ; ++columnIndex) {
                vectorScalarProduct(outNumberRows, scaleFactor, sourceData, sourceRowIncrement, destinationData, 1);
                sourceData      += sourceColumnIncrement;
                destinationData += destinationColumnSpacing;
            }
        }

        return result;
    }


    int MatrixInteger::DenseData::relativeOrder(const MatrixInteger::Data& other) const {
        int result;

        if (other.matrixType() == MatrixType::DENSE) {
            const DenseData& denseOther = POLYMORPHIC_CAST<const DenseData&>(other);
            result = MatrixDensePrivateBase::relativeOrder(denseOther);
        } else {
            result = static_cast<int>(matrixType()) - static_cast<int>(other.matrixType());
        }

        return result;
    }


    void MatrixInteger::DenseData::vectorAdd(
            unsigned long long       numberTerms,
            const Scalar*            augendBase,
            const unsigned long long augendIncrement,
            const Scalar             augendScaleFactor,
            const Scalar*            addendBase,
            const unsigned long long addendIncrement,
            const Scalar             addendScaleFactor,
            Scalar*                  resultBase,
            const unsigned long long resultIncrement
        ) {
        const Scalar* a = augendBase;
        const Scalar* b = addendBase;
        Scalar*       y = resultBase;

        for (unsigned long long i=0 ; i<numberTerms ; ++i) {
            *y = augendScaleFactor * (*a) + addendScaleFactor * (*b);
            a += augendIncrement;
            b += addendIncrement;
            y += resultIncrement;
        }
    }


    void MatrixInteger::DenseData::vectorDotProduct(
            unsigned long long       numberTerms,
            const Scalar             scaleFactor,
            const Scalar*            multiplierBase,
            const unsigned long long multiplierIncrement,
            const Scalar*            multiplicandBase,
            const unsigned long long multiplicandIncrement,
            Scalar*                  resultBase,
            const unsigned long long resultIncrement
        ) {
        const Scalar* a = multiplierBase;
        const Scalar* b = multiplicandBase;
        Scalar*       y = resultBase;

        for (unsigned long long i=0 ; i<numberTerms ; ++i) {
            *y = scaleFactor * (*a) * (*b);
            a += multiplierIncrement;
            b += multiplicandIncrement;
            y += resultIncrement;
        }
    }


    void MatrixInteger::DenseData::vectorScalarProduct(
            unsigned long long       numberTerms,
            const Scalar             scaleFactor,
            const Scalar*            base,
            const unsigned long long increment,
            Scalar*                  resultBase,
            const unsigned long long resultIncrement
        ) {
        const Scalar* a = base;
        Scalar*       y = resultBase;

        for (unsigned long long i=0 ; i<numberTerms ; ++i) {
            *y = scaleFactor * (*a);
            a += increment;
            y += resultIncrement;
        }
    }


    void MatrixInteger::DenseData::vectorScalarQuotient(
            unsigned long long       numberTerms,
            const Scalar             scalarMultiplier,
            const Scalar             scalarDivisor,
            const Scalar*            base,
            const unsigned long long increment,
            Scalar*                  resultBase,
            const unsigned long long resultIncrement
        ) {
        const Scalar* a = base;
        Scalar*       y = resultBase;

        for (unsigned long long i=0 ; i<numberTerms ; ++i) {
            *y = (scalarMultiplier * (*a)) / scalarDivisor;
            a += increment;
            y += resultIncrement;
        }
    }


    void MatrixInteger::DenseData::matrixScalarProduct(
            unsigned long            numberOutputRows,
            unsigned long            numberOutputColumns,
            bool                     transpose,
            const Scalar             scaleFactor,
            const Scalar*            inputBase,
            const unsigned long long inputColumnSpacingInEntries,
            Scalar*                  resultBase,
            const unsigned long long resultColumnSpacingInEntries
        ) {
        const Scalar* a = inputBase;
        Scalar*       y = resultBase;

        if (transpose) {
            for (unsigned long columnIndex=0 ; columnIndex<numberOutputColumns ; ++columnIndex) {
                vectorScalarProduct(
                    numberOutputRows,
                    scaleFactor,
                    a,
                    inputColumnSpacingInEntries,
                    y,
                    1
                );

                a += 1;
                y += resultColumnSpacingInEntries;
            }
        } else {
            for (unsigned long columnIndex=0 ; columnIndex<numberOutputColumns ; ++columnIndex) {
                vectorScalarProduct(
                    numberOutputRows,
                    scaleFactor,
                    a,
                    1,
                    y,
                    1
                );

                a += inputColumnSpacingInEntries;
                y += resultColumnSpacingInEntries;
            }
        }
    }


    MatrixInteger::Scalar MatrixInteger::DenseData::convertString(const std::string& str, bool* ok) {
        std::string t = str;
        boost::algorithm::trim(t);
        boost::algorithm::to_lower(t);

        unsigned radix = 10;
        if        (t.rfind("0b") == 0 || t.rfind("'b") == 0) {
            radix = 2;
            t     = t.substr(2);
        } else if (t.rfind("0x") == 0 || t.rfind("'h") == 0) {
            radix = 16;
            t     = t.substr(2);
        } else {
            radix = 10;
            t     = str;
        }

        std::size_t index;
        Scalar      result = std::stoll(t.c_str(), &index, radix);

        if (ok != nullptr) {
            *ok = (index == str.length());
        }

        return result;
    }
}
