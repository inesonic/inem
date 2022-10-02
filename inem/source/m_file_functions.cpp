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
* This file implements functions used to load and save to/from files.
***********************************************************************************************************************/

#define _FILE_OFFSET_BITS (64) // Creates 64-bit seek on POSIX

#include <cassert>
#include <complex>
#include <cmath>
#include <limits>
#include <vector>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cinttypes>
#include <string>
#include <map>

#if (defined(_MSC_VER))

    #include <io.h>
    #define ACCESS(_path, _mode) _access((_path), (_mode))
    #define FSEEK(_f, _offset, _whence) _fseeki64((_f), (_offset), (_whence))

    #define F_OK (0)
    #define R_OK (4)
    #define W_OK (2)
    #define X_OK (0) // No equivalent on Windows

#else

    #include <unistd.h>
    #define ACCESS(_path, _mode) access((_path), (_mode))
    #define FSEEK(_f, _offset, _whence) fseeko((_f), (_offset), (_whence))

#endif

#include <boost/endian/conversion.hpp>

#include "m_common.h"
#include "m_intrinsics.h"
#include "m_intrinsic_types.h"
#include "m_type_conversion.h"
#include "m_exceptions.h"
#include "m_tuple.h"
#include "m_matrix_boolean.h"
#include "m_matrix_integer.h"
#include "m_matrix_real.h"
#include "m_matrix_complex.h"
#include "m_variant.h"
#include "m_file_functions.h"

namespace M {
    enum class AudioFileFormat {
        INVALID,
        WAV,
        FLAC,
        MP3
    };

    class FileData {
        public:
            enum class OpenMode {
                CLOSED,
                READ_ONLY,
                READ_WRITE
            };

            FileData() {}

            FileData(
                    const std::string& filename,
                    Integer            fileNumber,
                    Boolean            binaryFile = false,
                    OpenMode           openMode = OpenMode::CLOSED,
                    FILE*              fileHandle = nullptr
                ):currentFilename(
                    filename
                ),currentFileNumber(
                    fileNumber
                ),currentBinaryFile(
                    binaryFile
                ),currentOpenMode(
                    openMode
                ),currentFileHandle(
                    fileHandle
                ) {}

            FileData(
                    const FileData& other
                ):currentFilename(
                    other.currentFilename
                ),currentFileNumber(
                    other.currentFileNumber
                ),currentBinaryFile(
                    other.currentBinaryFile
                ),currentOpenMode(
                    other.currentOpenMode
                ),currentFileHandle(
                    other.currentFileHandle
                ) {}

            FileData(
                    FileData&& other
                ):currentFilename(
                    other.currentFilename
                ),currentFileNumber(
                    other.currentFileNumber
                ),currentBinaryFile(
                    other.currentBinaryFile
                ),currentOpenMode(
                    other.currentOpenMode
                ),currentFileHandle(
                    other.currentFileHandle
                ) {}

            ~FileData() {}

            void setFilename(const std::string& filename) {
                currentFilename = filename;
            }

            const std::string& filename() const {
                return currentFilename;
            }

            Integer fileNumber() const {
                return currentFileNumber;
            }

            void setBinaryFile(Boolean binaryFile) {
                currentBinaryFile = binaryFile;
            }

            Boolean binaryFile() const {
                return currentBinaryFile;
            }

            void setOpenMode(OpenMode openMode) {
                currentOpenMode = openMode;
            }

            OpenMode openMode() const {
                return currentOpenMode;
            }

            void setFileHandle(FILE* fileHandle) {
                currentFileHandle = fileHandle;
            }

            FILE* fileHandle() const {
                return currentFileHandle;
            }

        private:
            std::string currentFilename;
            Integer     currentFileNumber;
            Boolean     currentBinaryFile;
            OpenMode    currentOpenMode;
            FILE*       currentFileHandle;
    };

    static std::vector<FileData> files;

    // The getFileData function returns a pointer to a FileData instance or throws an exception if the supplied file
    // number is invalid.

    static FileData* getFileData(Integer fileNumber) {
        FileData* result;

        if (fileNumber <= 0 || static_cast<std::vector<FileData>::size_type>(fileNumber) > files.size()) {
            internalTriggerInvalidFileNumberError(fileNumber);
            result = nullptr;
        } else {
            result = &(files[fileNumber - 1]);
        }

        return result;
    }


    // The allocateFileData function identifies a FileData instance that can be used for future activity.

    static FileData* allocateFileData(const std::string& filename) {
        FileData* result;

        unsigned long numberInstances = static_cast<unsigned long>(files.size());
        unsigned long i               = 0;

        while (i < numberInstances && files.at(i).openMode() != FileData::OpenMode::CLOSED) {
            ++i;
        }

        if (i < numberInstances) {
            result = &(files[i]);
            result->setFilename(filename);
            result->setBinaryFile(false);
        } else {
            files.push_back(FileData(filename, numberInstances + 1));
            result = &(files.at(i));
        }

        return result;
    }


    // The releaseFileData method releases an existing file data instance.

    void releaseFileData(FileData* fileData) {
        fileData->setOpenMode(FileData::OpenMode::CLOSED);
        fileData->setFilename(std::string());
        fileData->setFileHandle(nullptr);
    }


    Integer fileOpenRead(const Tuple& filename, Boolean binary) {
        Integer result;
        char*   fn = filename.toString();

        if (fn == nullptr) {
            internalTriggerCanNotConvertToStringError();
            result = 0;
        } else {
            std::string f(fn);
            delete[] fn;

            FileData* fileData = allocateFileData(f);
            assert(fileData != nullptr);

            FILE* fileHandle = std::fopen(f.data(), binary ? "rb" : "r");

            if (fileHandle == nullptr) {
                internalTriggerFileOpenError(f.data(), errno);
                result = 0;
            } else {
                fileData->setFileHandle(fileHandle);
                fileData->setOpenMode(FileData::OpenMode::READ_ONLY);
                fileData->setBinaryFile(binary);

                result = fileData->fileNumber();
            }
        }

        return result;
    }


    Integer fileOpenWriteTruncate(const Tuple& filename, Boolean binary) {
        Integer result;
        char*   fn = filename.toString();

        if (fn == nullptr) {
            internalTriggerCanNotConvertToStringError();
            result = 0;
        } else {
            std::string f(fn);
            delete[] fn;

            FileData* fileData = allocateFileData(f);
            assert(fileData != nullptr);

            FILE* fileHandle = std::fopen(f.data(), binary ? "w+b" : "w+");

            if (fileHandle == nullptr) {
                internalTriggerFileOpenError(f.data(), errno);
                result = 0;
            } else {
                fileData->setFileHandle(fileHandle);
                fileData->setOpenMode(FileData::OpenMode::READ_WRITE);
                fileData->setBinaryFile(binary);

                result = fileData->fileNumber();
            }
        }

        return result;
    }


    Integer fileOpenWriteAppend(const Tuple& filename, Boolean binary) {
        Integer result;
        char*   fn = filename.toString();

        if (fn == nullptr) {
            internalTriggerCanNotConvertToStringError();
            result = 0;
        } else {
            std::string f(fn);
            delete[] fn;

            FileData* fileData = allocateFileData(f);
            assert(fileData != nullptr);

            FILE* fileHandle;
            if (ACCESS(f.data(), F_OK) == 0) {
                fileHandle = std::fopen(f.data(), binary ? "r+b" : "r+");
            } else {
                fileHandle = std::fopen(f.data(), binary ? "w+" : "w+b");
            }

            if (fileHandle == nullptr) {
                internalTriggerFileOpenError(f.data(), errno);
                result = 0;
            } else {
                if (FSEEK(fileHandle, 0, SEEK_END) != 0) {
                    internalTriggerFileSeekError(static_cast<unsigned long long>(-1), f.data(), errno);
                    result = 0;
                } else {
                    fileData->setFileHandle(fileHandle);
                    fileData->setOpenMode(FileData::OpenMode::READ_ONLY);
                    fileData->setBinaryFile(binary);

                    result = fileData->fileNumber();
                }
            }
        }

        return result;
    }


    Boolean fileExists(const Tuple& filename) {
        Boolean result;
        char*   fn = filename.toString();

        if (fn == nullptr) {
            internalTriggerCanNotConvertToStringError();
            result = false;
        } else {
            result = (ACCESS(fn, F_OK) == 0);
            delete[] fn;
        }

        return result;
    }


    Boolean fileClose(Integer fileNumber) {
        Boolean result;

        FileData* fileData = getFileData(fileNumber);
        if (fileData != nullptr) {
            if (fileData->openMode() != FileData::OpenMode::CLOSED) {
                if (fclose(fileData->fileHandle()) != 0) {
                    internalTriggerFileCloseError(fileData->filename().data(), errno);
                    result = false;
                } else {
                    releaseFileData(fileData);
                    result = true;
                }
            } else {
                result = false;
            }
        } else {
            result = false;
        }

        return result;
    }


    Boolean fileDelete(const Tuple& filename) {
        Boolean result;
        char*   fn = filename.toString();

        if (fn == nullptr) {
            internalTriggerCanNotConvertToStringError();
            result = false;
        } else {
            result = (std::remove(fn) == 0);
            delete[] fn;
        }

        return result;
    }


    Boolean fileDelete(Integer fileNumber) {
        Boolean result = true;

        FileData* fileData = getFileData(fileNumber);
        if (fileData != nullptr) {
            std::string filename = fileData->filename();

            if (fileData->openMode() != FileData::OpenMode::CLOSED) {
                if (fclose(fileData->fileHandle()) != 0) {
                    internalTriggerFileCloseError(fileData->filename().data(), errno);
                    result = false;
                } else {
                    releaseFileData(fileData);
                }

                if (result) {
                    result = (std::remove(filename.data()) == 0);
                }
            } else {
                internalTriggerFileCloseError(filename.data(), 0);
                result = false;
            }
        } else {
            result = false;
        }

        return result;
    }


    Boolean fileSeek(Integer fileNumber, Integer offset) {
        Boolean result;

        FileData* fileData = getFileData(fileNumber);
        if (fileData != nullptr) {
            if (FSEEK(fileData->fileHandle(), offset, SEEK_SET) != 0) {
                internalTriggerFileSeekError(offset, fileData->filename().data(), errno);
                result = false;
            } else {
                result = true;
            }
        } else {
            result = false;
        }

        return result;
    }


    Tuple fileReadByte(Integer fileNumber) {
        Boolean success;
        Integer value;

        FileData* fileData = getFileData(fileNumber);
        if (fileData != nullptr) {
            std::uint8_t buffer[1];
            std::size_t  bytesRead = std::fread(buffer, 1, 1, fileData->fileHandle());

            if (bytesRead != 1) {
                int errorCode = std::ferror(fileData->fileHandle());
                if (errorCode != 0) {
                    internalTriggerFileReadError(fileData->filename().data(), errorCode);
                }

                success = false;
            } else {
                success = true;
            }

            value = static_cast<Integer>(buffer[0]);
        } else {
            value   = static_cast<Integer>(-1);
            success = false;
        }

        return Tuple::build(value, success);
    }


    Tuple fileReadString(Integer fileNumber, Integer length, Boolean utf8) {
        Boolean     success = true;
        std::string buffer;

        FileData* fileData = getFileData(fileNumber);
        if (fileData != nullptr) {
            if (length <= 0) {
                int  c;
                bool atEOL = false;
                bool atEOF = false;
                do {
                    c = std::fgetc(fileData->fileHandle());
                    if (c == EOF) {
                        int errorCode = std::ferror(fileData->fileHandle());
                        if (errorCode != 0) {
                            internalTriggerFileReadError(fileData->filename().data(), errorCode);
                            success = false;
                        } else {
                            atEOF = true;
                        }
                    } else {
                        if (length == 0 && (c == '\x0C' || c == '\x0A')) {
                            atEOL = true;
                        } else {
                            buffer.push_back(static_cast<char>(c));
                        }
                    }
                } while (success && !atEOL && !atEOF);

                if (atEOL) {
                    do {
                        c = std::fgetc(fileData->fileHandle());
                        if (c == EOF) {
                            int errorCode = std::ferror(fileData->fileHandle());
                            if (errorCode != 0) {
                                internalTriggerFileReadError(fileData->filename().data(), errorCode);
                                success = false;
                            } else {
                                atEOF = true;
                            }
                        }
                    } while (success && !atEOF && (c == '\x0C' || c == '\x0A'));

                    if (success && !atEOF) {
                        if (std::ungetc(c, fileData->fileHandle()) == EOF) {
                            int errorCode = std::ferror(fileData->fileHandle());
                            if (errorCode != 0) {
                                internalTriggerFileReadError(fileData->filename().data(), errorCode);
                                success = false;
                            }
                        }
                    }
                }
            } else /* if (length > 0) */ {
                unsigned      additionalToReadThisCharacter = 0;
                unsigned long remaining                     = length;
                bool          atEOF                         = false;

                do {
                    int c = std::fgetc(fileData->fileHandle());
                    if (c == EOF) {
                        int errorCode = std::ferror(fileData->fileHandle());
                        if (errorCode != 0) {
                            internalTriggerFileReadError(fileData->filename().data(), errorCode);
                            success = false;
                        } else {
                            atEOF = true;
                        }
                    } else {
                        if (!utf8 || additionalToReadThisCharacter == 0) {
                            if ((c & 0xE0) == 0xC0) {
                                additionalToReadThisCharacter = 1;
                            } else if ((c & 0xF0) == 0xE0) {
                                additionalToReadThisCharacter = 2;
                            } else if ((c & 0xF8) == 0xF0) {
                                additionalToReadThisCharacter = 3;
                            }

                            --remaining;
                        } else {
                            --additionalToReadThisCharacter;
                        }

                        buffer.push_back(static_cast<char>(c));
                    }
                } while (success && !atEOF && remaining > 0);
            }
        } else {
            success = false;
        }

        Tuple tupleBuffer;
        if (utf8) {
            tupleBuffer = Tuple(buffer.data());
        } else {
            for (std::string::iterator it=buffer.begin(),end=buffer.end() ; it!=end ; ++it) {
                tupleBuffer.append(Integer(*it));
            }
        }

        return Tuple::build(tupleBuffer, success);
    }


    Tuple fileReadInteger(Integer fileNumber, Integer bytesToRead) {
        Boolean success = true;
        Integer result  = 0;

        FileData* fileData = getFileData(fileNumber);
        if (fileData != nullptr) {
            if (bytesToRead == 0) {
                std::string buffer;
                int         c;
                bool        atEOF      = false;
                bool        endReached = false;
                unsigned    base       = 10;
                do {
                    c = std::fgetc(fileData->fileHandle());
                    if (c == EOF) {
                        int errorCode = std::ferror(fileData->fileHandle());
                        if (errorCode != 0) {
                            internalTriggerFileReadError(fileData->filename().data(), errorCode);
                            success = false;
                        } else {
                            atEOF = true;
                        }
                    } else {
                        unsigned long currentLength = static_cast<unsigned long>(buffer.size());
                        if (currentLength == 0) {
                            if ((c >= '0' && c <= '9') || c == '+' || c == '-') {
                                buffer.push_back(char(c));
                            } else {
                                endReached = true;
                            }
                        } else if (currentLength == 1) {
                            if (c >= '0' && c <= '9') {
                                buffer.push_back(char(c));
                            } else if (buffer.at(0) == '0') {
                                if (c == 'x') {
                                    buffer.push_back(char(c));
                                    base = 16;
                                } else if (c == 'b') {
                                    buffer.push_back(char(c));
                                    base = 2;
                                } else {
                                    endReached = true;
                                }
                            } else {
                                endReached = true;
                            }
                        } else /* if (currentLength > 1) */ {
                            if (base == 2 && (c  == '0' || c == '1')) {
                                buffer.push_back(char(c));
                            } else if (base == 10 && (c >= '0' && c <= '9')) {
                                buffer.push_back(char(c));
                            } else if (base == 16                   &&
                                       ((c >= '0' && c <= '9') ||
                                        (c >= 'A' && c <= 'F') ||
                                        (c >= 'a' && c <= 'f')    )    ) {
                                buffer.push_back(char(c));
                            } else {
                                endReached = true;
                            }
                        }
                    }
                } while (success && !atEOF && !endReached);

                if (endReached) {
                    if (std::ungetc(c, fileData->fileHandle()) == EOF) {
                        int errorCode = std::ferror(fileData->fileHandle());
                        if (errorCode != 0) {
                            internalTriggerFileReadError(fileData->filename().data(), errorCode);
                            success = false;
                        }
                    }
                }

                if (success) {
                    if (buffer.empty() || buffer == "+" || buffer == "-" || buffer == "0x" || buffer == "0b") {
                        success = false;
                    } else {
                        unsigned offset   = 0;
                        bool     negative = false;
                        unsigned base     = 10;

                        if (buffer.at(0) == '+') {
                            offset = 1;
                        } else if (buffer.at(0) == '-') {
                            offset   = 1;
                            negative = true;
                        } else if (buffer.at(0) == '0' && buffer.size() > 1) {
                            if (buffer.at(1) == 'x') {
                                offset = 2;
                                base   = 16;
                            } else if (buffer.at(1) == 'b') {
                                offset = 2;
                                base   = 2;
                            }
                        }

                        result = std::stoll(buffer.substr(offset), nullptr, base);
                        if (negative) {
                            result = -result;
                        }
                    }
                }
            } else {
                if (bytesToRead < -8) {
                    bytesToRead = -8;
                } else if (bytesToRead > 8) {
                    bytesToRead = 8;
                }

                unsigned btr = static_cast<unsigned>(bytesToRead > 0 ? +bytesToRead : -bytesToRead);

                unsigned char buffer[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
                std::size_t   bytesRead = fread(buffer, 1, btr, fileData->fileHandle());

                if (static_cast<unsigned>(bytesRead) != btr) {
                    internalTriggerFileReadError(fileData->filename().data(), std::ferror(fileData->fileHandle()));
                    success = false;
                } else {
                    switch (bytesToRead) {
                        case -8: { result = boost::endian::load_big_s64(buffer);       break; }
                        case -7: { result = boost::endian::load_big_s56(buffer);       break; }
                        case -6: { result = boost::endian::load_big_s48(buffer);       break; }
                        case -5: { result = boost::endian::load_big_s40(buffer);       break; }
                        case -4: { result = boost::endian::load_big_s32(buffer);       break; }
                        case -3: { result = boost::endian::load_big_s24(buffer);       break; }
                        case -2: { result = boost::endian::load_big_s16(buffer);       break; }
                        case -1: { result = static_cast<Integer>(buffer[0]);           break; }
                        case  0: { assert(false);                                      break; }
                        case +1: { result = static_cast<Integer>(buffer[0]);           break; }
                        case +2: { result = boost::endian::load_little_s16(buffer);    break; }
                        case +3: { result = boost::endian::load_little_s24(buffer);    break; }
                        case +4: { result = boost::endian::load_little_s32(buffer);    break; }
                        case +5: { result = boost::endian::load_little_s40(buffer);    break; }
                        case +6: { result = boost::endian::load_little_s48(buffer);    break; }
                        case +7: { result = boost::endian::load_little_s56(buffer);    break; }
                        case +8: { result = boost::endian::load_little_s64(buffer);    break; }
                        default: { assert(false);                                      break; }
                    }
                }
            }
        } else {
            success = false;
        }

        return Tuple::build(result, success);
    }


    Tuple fileReadReal(Integer fileNumber, Integer bytesToRead) {
        Boolean success = true;
        Real    result  = 0;

        FileData* fileData = getFileData(fileNumber);
        if (fileData != nullptr) {
            if (bytesToRead == 0) {
                std::string buffer;
                int         c;
                bool        atEOF       = false;
                bool        endReached  = false;
                bool        sawDP       = false;
                bool        sawExponent = false;
                do {
                    c = std::fgetc(fileData->fileHandle());
                    if (c == EOF) {
                        int errorCode = std::ferror(fileData->fileHandle());
                        if (errorCode != 0) {
                            internalTriggerFileReadError(fileData->filename().data(), errorCode);
                            success = false;
                        } else {
                            atEOF = true;
                        }
                    } else {
                        if (c >= '0' && c <= '9') {
                            buffer.push_back(char(c));
                        } else if (c == '.') {
                            if (!sawDP && !sawExponent) {
                                buffer.push_back(char(c));
                                sawDP = true;
                            } else {
                                endReached = true;
                            }
                        } else if (c == '+' || c == '-') {
                            if (buffer.empty() || buffer.back() == 'e' || buffer.back() == 'E') {
                                buffer.push_back(char(c));
                            } else {
                                endReached = true;
                            }
                        } else if (c == 'e' || c == 'E') {
                            if (!sawExponent                                 &&
                                !buffer.empty()                              &&
                                buffer.back() != '+'                         &&
                                buffer.back() != '-'                         &&
                                (buffer.length() > 1 || buffer.at(0) != '.')    ) {
                                buffer.push_back(char(c));
                                sawExponent = true;
                            } else {
                                endReached = true;
                            }
                        } else {
                            endReached = true;
                        }
                    }
                } while (success && !atEOF && !endReached);

                if (endReached) {
                    if (std::ungetc(c, fileData->fileHandle()) == EOF) {
                        int errorCode = std::ferror(fileData->fileHandle());
                        if (errorCode != 0) {
                            internalTriggerFileReadError(fileData->filename().data(), errorCode);
                            success = false;
                        }
                    }
                }

                if (success) {
                    if (buffer.empty()       ||
                        buffer == "+"        ||
                        buffer == "-"        ||
                        buffer == "."        ||
                        buffer.back() == 'e' ||
                        buffer.back() == 'E'    ) {
                        success = false;
                    } else {
                        result = std::strtod(buffer.data(), nullptr);
                    }
                }
            } else {
                unsigned btr = static_cast<unsigned>(bytesToRead > 0 ? +bytesToRead : -bytesToRead);

                unsigned char buffer[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
                std::size_t   bytesRead = fread(buffer, 1, btr, fileData->fileHandle());

                if (static_cast<unsigned>(bytesRead) != btr) {
                    internalTriggerFileReadError(fileData->filename().data(), std::ferror(fileData->fileHandle()));
                    success = false;
                } else {
                    if (bytesToRead == -8) {
                        union {
                            std::uint64_t i;
                            double        r;
                        } v;
                        v.i = boost::endian::load_big_u64(buffer);
                        result = v.r;
                    } else if (bytesToRead == -4) {
                        union {
                            std::uint32_t i;
                            float         r;
                        } v;
                        v.i = boost::endian::load_big_u32(buffer);
                        result = v.r;
                    } else if (bytesToRead == +4) {
                        result = *(reinterpret_cast<float*>(buffer));
                    } else if (bytesToRead == +8) {
                        result = *(reinterpret_cast<double*>(buffer));
                    } else {
                        internalTriggerInvalidParameterValueError();
                        success = false;
                    }
                }
            }
        } else {
            success = false;
        }

        return Tuple::build(result, success);
    }


    Boolean fileWriteByte(Integer fileNumber, Integer byteValue) {
        Boolean success = true;

        if (byteValue >= 0 && byteValue <= 255) {
            FileData* fileData = getFileData(fileNumber);
            if (fileData != nullptr) {
                std::uint8_t byte   = static_cast<std::uint8_t>(byteValue);
                std::size_t  result = std::fwrite(&byte, 1, 1, fileData->fileHandle());

                if (result != 1) {
                    int errorCode = std::ferror(fileData->fileHandle());
                    internalTriggerFileWriteError(fileData->filename().data(), errorCode);
                    success = false;
                }
            } else {
                success = false;
            }
        } else {
            internalTriggerInvalidParameterValueError();
            success = false;
        }

        return success;
    }


    Boolean fileWriteString(Integer fileNumber, const Tuple& string, Boolean includeNewline) {
        Boolean success = true;

        char* c = string.toString();
        if (c != nullptr) {
            std::string str(c);
            delete[] c;

            FileData* fileData = getFileData(fileNumber);
            if (fileData != nullptr) {
                if (includeNewline) {
                    str += "\n";
                }

                unsigned long length = static_cast<unsigned long>(str.length());
                std::size_t   result = fwrite(str.data(), 1, length, fileData->fileHandle());

                if (result != length) {
                    int errorCode = std::ferror(fileData->fileHandle());
                    internalTriggerFileWriteError(fileData->filename().data(), errorCode);
                    success = false;
                }
            } else {
                success = false;
            }
        } else {
            internalTriggerCanNotConvertToStringError();
            success = false;
        }

        return success;
    }


    Boolean fileWriteInteger(Integer fileNumber, Integer integerValue, Integer bytesToWrite) {
        Boolean success = true;

        FileData* fileData = getFileData(fileNumber);
        if (fileData != nullptr) {
            if (bytesToWrite == 0) {
                std::stringstream ss;
                ss << integerValue;
                std::string str = ss.str();

                unsigned long length = static_cast<unsigned long>(str.length());
                std::size_t   result = fwrite(str.data(), 1, length, fileData->fileHandle());

                if (result != length) {
                    int errorCode = std::ferror(fileData->fileHandle());
                    internalTriggerFileWriteError(fileData->filename().data(), errorCode);
                    success = false;
                }
            } else {
                if (bytesToWrite < -8) {
                    bytesToWrite = -8;
                } else if (bytesToWrite > +8) {
                    bytesToWrite = +8;
                }

                unsigned btw = static_cast<unsigned>(bytesToWrite < 0 ? -bytesToWrite : +bytesToWrite);

                unsigned char buffer[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
                switch (bytesToWrite) {
                    case -8: {
                        boost::endian::store_big_s64(buffer, integerValue);
                        break;
                    }

                    case -7: {
                        boost::endian::store_big_s56(buffer, integerValue);
                        break;
                    }

                    case -6: {
                        boost::endian::store_big_s48(buffer, integerValue);
                        break;
                    }

                    case -5: {
                        boost::endian::store_big_s40(buffer, integerValue);
                        break;
                    }

                    case -4: {
                        boost::endian::store_big_s32(buffer, static_cast<std::int32_t>(integerValue));
                        break;
                    }

                    case -3: {
                        boost::endian::store_big_s24(buffer, static_cast<std::int32_t>(integerValue));
                        break;
                    }

                    case -2: {
                        boost::endian::store_big_s16(buffer, integerValue);
                        break;
                    }

                    case -1: {
                        buffer[0] = static_cast<unsigned char>(integerValue);
                        break;
                    }

                    case  0: {
                        assert(false);
                        break;
                    }

                    case +1: {
                        buffer[0] = static_cast<unsigned char>(integerValue);
                        break;
                    }

                    case +2: {
                        boost::endian::store_little_s16(buffer, integerValue);
                        break;
                    }

                    case +3: {
                        boost::endian::store_little_s24(buffer, static_cast<std::int32_t>(integerValue));
                        break;
                    }

                    case +4: {
                        boost::endian::store_little_s32(buffer, static_cast<std::int32_t>(integerValue));
                        break;
                    }

                    case +5: {
                        boost::endian::store_little_s40(buffer, integerValue);
                        break;
                    }

                    case +6: {
                        boost::endian::store_little_s48(buffer, integerValue);
                        break;
                    }

                    case +7: {
                        boost::endian::store_little_s56(buffer, integerValue);
                        break;
                    }

                    case +8: {
                        boost::endian::store_little_s64(buffer, integerValue);
                        break;
                    }

                    default: {
                        assert(false);
                        break;
                    }
                }

                std::size_t result = fwrite(buffer, 1, btw, fileData->fileHandle());

                if (result != static_cast<std::size_t>(btw)) {
                    int errorCode = std::ferror(fileData->fileHandle());
                    internalTriggerFileWriteError(fileData->filename().data(), errorCode);
                    success = false;
                }
            }
        } else {
            success = false;
        }

        return success;
    }


    Boolean fileWriteReal(Integer fileNumber, Real realValue, Integer bytesToWrite) {
        Boolean success = true;

        FileData* fileData = getFileData(fileNumber);
        if (fileData != nullptr) {
            if (bytesToWrite == 0) {
                char buffer[30];
                sprintf(buffer, "%.17lg", realValue);
                std::string str(buffer);

                if (str.find('.') != std::string::npos &&
                    str.find('e') != std::string::npos &&
                    str.find('E') != std::string::npos    ) {
                    while (str.length() > 1 && str.back() == '0') {
                        str.pop_back();
                    }
                }

                std::size_t result = fwrite(str.data(), 1, str.length(), fileData->fileHandle());

                if (result != static_cast<std::size_t>(str.length())) {
                    int errorCode = std::ferror(fileData->fileHandle());
                    internalTriggerFileWriteError(fileData->filename().data(), errorCode);
                    success = false;
                }
            } else {
                unsigned char buffer[8];
                unsigned      btw = 0;

                if (bytesToWrite == -8) {
                    btw = 8;

                    union {
                        std::uint64_t i;
                        double        r;
                    } u;

                    u.r = realValue;
                    boost::endian::store_big_u64(buffer, u.i);
                } else if (bytesToWrite == -4) {
                    btw = 4;

                    union {
                        std::uint32_t i;
                        float         r;
                    } u;

                    u.r = realValue;
                    boost::endian::store_big_u32(buffer, u.i);
                } else if (bytesToWrite == +4) {
                    btw = 4;

                    union {
                        std::uint32_t i;
                        float         r;
                    } u;

                    u.r = realValue;
                    boost::endian::store_little_u32(buffer, u.i);
                } else if (bytesToWrite == +8) {
                    btw = 8;

                    union {
                        std::uint64_t i;
                        double        r;
                    } u;

                    u.r = realValue;
                    boost::endian::store_little_u64(buffer, u.i);
                } else {
                    internalTriggerInvalidParameterValueError();
                    success = false;
                }

                if (success) {
                    std::size_t result = fwrite(buffer, 1, btw, fileData->fileHandle());

                    if (result != static_cast<std::size_t>(btw)) {
                        int errorCode = std::ferror(fileData->fileHandle());
                        internalTriggerFileWriteError(fileData->filename().data(), errorCode);
                        success = false;
                    }
                }
            }
        } else {
            success = false;
        }

        return success;
    }


    MatrixBoolean loadBooleanMatrix(const Tuple& filename) {
        MatrixBoolean result;
        char*         fn = filename.toString();

        if (fn == nullptr) {
            internalTriggerCanNotConvertToStringError();
        } else {
            std::string f(fn);
            delete[] fn;

            result = MatrixBoolean::fromFile(f.data());
        }

        return result;
    }


    Boolean saveBooleanMatrix(const MatrixBoolean& matrix, const Tuple& filename, Boolean binary) {
        Boolean success;

        char* fn = filename.toString();
        if (fn == nullptr) {
            internalTriggerCanNotConvertToStringError();
            success = false;
        } else {
            std::string f(fn);
            delete[] fn;

            success = matrix.toFile(f.data(), binary ? Model::DataFileFormat::BINARY : Model::DataFileFormat::CSV);
        }

        return success;
    }


    MatrixInteger loadIntegerMatrix(const Tuple& filename) {
        MatrixInteger result;
        char*         fn = filename.toString();

        if (fn == nullptr) {
            internalTriggerCanNotConvertToStringError();
        } else {
            std::string f(fn);
            delete[] fn;

            result = MatrixInteger::fromFile(f.data());
        }

        return result;
    }


    Boolean saveIntegerMatrix(const MatrixInteger& matrix, const Tuple& filename, Boolean binary) {
        Boolean success;

        char* fn = filename.toString();
        if (fn == nullptr) {
            internalTriggerCanNotConvertToStringError();
            success = false;
        } else {
            std::string f(fn);
            delete[] fn;

            success = matrix.toFile(f.data(), binary ? Model::DataFileFormat::BINARY : Model::DataFileFormat::CSV);
        }

        return success;
    }


    MatrixReal loadRealMatrix(const Tuple& filename) {
        MatrixReal result;
        char*      fn = filename.toString();

        if (fn == nullptr) {
            internalTriggerCanNotConvertToStringError();
        } else {
            std::string f(fn);
            delete[] fn;

            result = MatrixReal::fromFile(f.data());
        }

        return result;
    }


    Boolean saveRealMatrix(const MatrixReal& matrix, const Tuple& filename, Boolean binary) {
        Boolean success;

        char* fn = filename.toString();
        if (fn == nullptr) {
            internalTriggerCanNotConvertToStringError();
            success = false;
        } else {
            std::string f(fn);
            delete[] fn;

            success = matrix.toFile(f.data(), binary ? Model::DataFileFormat::BINARY : Model::DataFileFormat::CSV);
        }

        return success;
    }


    MatrixComplex loadComplexMatrix(const Tuple& filename) {
        MatrixComplex result;
        char*      fn = filename.toString();

        if (fn == nullptr) {
            internalTriggerCanNotConvertToStringError();
        } else {
            std::string f(fn);
            delete[] fn;

            result = MatrixComplex::fromFile(f.data());
        }

        return result;
    }


    Boolean saveComplexMatrix(const MatrixComplex& matrix, const Tuple& filename, Boolean binary) {
        Boolean success;

        char* fn = filename.toString();
        if (fn == nullptr) {
            internalTriggerCanNotConvertToStringError();
            success = false;
        } else {
            std::string f(fn);
            delete[] fn;

            success = matrix.toFile(f.data(), binary ? Model::DataFileFormat::BINARY : Model::DataFileFormat::CSV);
        }

        return success;
    }
}
