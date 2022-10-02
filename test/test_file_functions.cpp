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
* This file implements tests of the file functions.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QObject>
#include <QList>
#include <QtTest/QtTest>
#include <QFile>
#include <QFileInfo>
#include <fstream>
#include <sstream>

#include <limits>

#include <m_intrinsic_types.h>
#include <model_exceptions.h>
#include <m_set.h>
#include <m_tuple.h>
#include <m_matrix_boolean.h>
#include <m_matrix_integer.h>
#include <m_matrix_real.h>
#include <m_matrix_complex.h>
#include <m_variant.h>
#include <m_file_functions.h>

#include "test_file_functions.h"

TestFileFunctions::TestFileFunctions() {}


TestFileFunctions::~TestFileFunctions() {}


void TestFileFunctions::testFileOpenRead() {
    buildTextFile("test.txt");

    M::Integer fid = M::fileOpenRead(M::Tuple("test.txt"), false);
    QCOMPARE(fid != 0, true);

    M::Tuple   r       = M::fileReadString(fid);
    M::Tuple   st      = r[1];
    M::Boolean success = r[2].toBoolean();

    QCOMPARE(success, true);

    char* s = st.toString();
    QCOMPARE(s != nullptr, true);

    QString str(s);
    delete[] s;

    QCOMPARE(str, QString("This is an example text file"));

    M::fileClose(fid);
}


void TestFileFunctions::testFileOpenWriteTruncate() {
    buildTextFile("test.txt");

    M::Integer fid = M::fileOpenWriteTruncate(M::Tuple("test.txt"), false);
    QCOMPARE(fid != 0, true);

    M::fileClose(fid);

    QFileInfo fileInformation("test.txt");
    QCOMPARE(fileInformation.exists(), true);
    QCOMPARE(fileInformation.size(), 0);
}


void TestFileFunctions::testFileOpenWriteAppend() {
    QFile("test.txt").remove();
    QCOMPARE(QFileInfo("test.txt").exists(), false);

    M::Integer fid = M::fileOpenWriteAppend(M::Tuple("test.txt"), false);
    QCOMPARE(fid != 0, true);

    M::fileClose(fid);

    QCOMPARE(QFileInfo("test.txt").exists(), true);
    QCOMPARE(QFileInfo("test.txt").size(), 0);

    buildTextFile("test.txt");
    unsigned long initialSize = static_cast<unsigned long>(QFileInfo("test.txt").size());

    fid = M::fileOpenWriteAppend(M::Tuple("test.txt"), false);
    QCOMPARE(fid != 0, true);

    M::Boolean success = M::fileWriteString(fid, M::Tuple("Some more text"));
    QCOMPARE(success, true);

    M::fileClose(fid);

    unsigned long finalSize = static_cast<unsigned long>(QFileInfo("test.txt").size());
    QCOMPARE(initialSize + 14, finalSize);
}


void TestFileFunctions::testFileExists() {
    QFile("test.txt").remove();

    QCOMPARE(QFileInfo("test.txt").exists(), false);
    QCOMPARE(M::fileExists(M::Tuple("test.txt")), false);

    buildTextFile("test.txt");

    QCOMPARE(QFileInfo("test.txt").exists(), true);
    QCOMPARE(M::fileExists(M::Tuple("test.txt")), true);
}


void TestFileFunctions::testFileClose() {
    // Tested elsewhere
}


void TestFileFunctions::testFileDelete1() {
    buildTextFile("test.txt");
    QCOMPARE(QFileInfo("test.txt").exists(), true);

    M::Boolean success = M::fileDelete(M::Tuple("test.txt"));
    QCOMPARE(success, true);

    QCOMPARE(QFileInfo("test.txt").exists(), false);

    success = M::fileDelete(M::Tuple("test.txt"));
    QCOMPARE(success, false);

    QCOMPARE(QFileInfo("test.txt").exists(), false);
}


void TestFileFunctions::testFileDelete2() {
    QFile("test.txt").remove();
    QCOMPARE(QFileInfo("test.txt").exists(), false);

    M::Integer fid = M::fileOpenWriteAppend(M::Tuple("test.txt"), false);
    QCOMPARE(fid != 0, true);

    M::Boolean success = M::fileDelete(fid);
    QCOMPARE(success, true);

    QCOMPARE(QFileInfo("test.txt").exists(), false);

    try {
        success = M::fileDelete(fid);
    } catch (Model::FileCloseError&) {
        success = false;
    }

    QCOMPARE(success, false);
}


void TestFileFunctions::testFileSeek() {}


void TestFileFunctions::testFileReadByte() {
    buildTextFile("test.txt");

    M::Integer fid = M::fileOpenRead(M::Tuple("test.txt"), false);
    QCOMPARE(fid != 0, true);

    M::Boolean  success;
    std::string contents;
    do {
        M::Tuple   r = M::fileReadByte(fid);
        M::Integer b = r[1].toInteger();
        success = r[2].toBoolean();

        if (success) {
            QCOMPARE(b > 0 && b <= 255, true);
            contents += char(b);
        }
    } while (success);

    M::fileClose(fid);

    std::stringstream ss;
    ss << "This is an example text file" << std::endl
       << "containing multiple lines." << std::endl
       << "\xC3\x97 \xD7\x91 \xEF\xAC\xA8 \xF0\x9D\x94\x84 \xE2\x84\xA8" << std::endl
       << "12 345 6789 103456 67801 0x12 0x1234 0b00010010 0b0001001000110100";

    std::string compare = ss.str();
    QCOMPARE(contents, compare);
}


void TestFileFunctions::testFileReadString() {
    buildTextFile("test.txt");

    M::Integer fid = M::fileOpenRead(M::Tuple("test.txt"), false);
    QCOMPARE(fid != 0, true);

    M::Tuple   l1      = M::fileReadString(fid);
    M::Tuple   st      = l1[1];
    M::Boolean success = l1[2].toBoolean();

    QCOMPARE(success, true);

    char* sc = st.toString();
    QCOMPARE(sc != nullptr, true);

    std::string s1(sc);
    delete[] sc;

    QCOMPARE(s1, "This is an example text file");

    M::Tuple l2 = M::fileReadString(fid, M::Integer(11), false);
    st          = l2[1];
    success     = l2[2].toBoolean();

    QCOMPARE(success, true);

    sc = st.toString();
    QCOMPARE(sc != nullptr, true);

    std::string s2(sc);
    delete[] sc;

    QCOMPARE(s2, "containing ");

    M::Tuple l3 = M::fileReadString(fid, M::Integer(-1));

    st          = l3[1];
    success     = l3[2].toBoolean();

    QCOMPARE(success, true);

    sc = st.toString();
    QCOMPARE(sc != nullptr, true);

    std::string s3(sc);
    delete[] sc;

    std::stringstream ss;
    ss << "multiple lines." << std::endl
       << "\xC3\x97 \xD7\x91 \xEF\xAC\xA8 \xF0\x9D\x94\x84 \xE2\x84\xA8" << std::endl
       << "12 345 6789 103456 67801 0x12 0x1234 0b00010010 0b0001001000110100";

    std::string compare = ss.str();
    QCOMPARE(s3, compare);

    M::fileClose(fid);
}


void TestFileFunctions::testFileReadIntegerText() {
    std::ofstream f("test.txt");
    Q_ASSERT(f);

    f << "123456789 +1234567890%-123456789-987654321/0xABCDEFG0x1234567890abcdefg0b111111101101110010111010";
    f.close();

    M::Integer fid = M::fileOpenRead(M::Tuple("test.txt"), false);
    QCOMPARE(fid != 0, true);

    M::Tuple   i1      = M::fileReadInteger(fid);
    M::Integer i       = i1[1].toInteger();
    M::Boolean success = i1[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(i, +123456789L);

    M::Tuple   c1 = M::fileReadByte(fid);
    M::Integer c  = c1[1].toInteger();
    success       = c1[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(c, ' ');

    M::Tuple i2 = M::fileReadInteger(fid);
    i           = i2[1].toInteger();
    success     = i2[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(i, +1234567890L);

    M::Tuple c2 = M::fileReadByte(fid);
    c           = c2[1].toInteger();
    success     = c2[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(c, '%');

    M::Tuple i3 = M::fileReadInteger(fid);
    i           = i3[1].toInteger();
    success     = i3[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(i, -123456789L);

    M::Tuple i4 = M::fileReadInteger(fid);
    i           = i4[1].toInteger();
    success     = i4[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(i, -987654321L);

    M::Tuple c3 = M::fileReadByte(fid);
    c           = c3[1].toInteger();
    success     = c3[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(c, '/');

    M::Tuple i5 = M::fileReadInteger(fid);
    i           = i5[1].toInteger();
    success     = i5[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(i, 0xABCDEFL);

    M::Tuple c4 = M::fileReadByte(fid);
    c           = c4[1].toInteger();
    success     = c4[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(c, 'G');

    M::Tuple i6 = M::fileReadInteger(fid);
    i           = i6[1].toInteger();
    success     = i6[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(i, 0x1234567890abcdefL);

    M::Tuple c5 = M::fileReadByte(fid);
    c           = c5[1].toInteger();
    success     = c5[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(c, 'g');

    M::Tuple i7 = M::fileReadInteger(fid);
    i           = i7[1].toInteger();
    success     = i7[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(i, 0xFEDCBAL);

    M::fileClose(fid);
}


void TestFileFunctions::testFileReadIntegerBinary() {
    std::ofstream f("test.bin", std::fstream::binary | std::fstream::trunc);
    Q_ASSERT(f);

    unsigned char buffer[256];
    for (unsigned i=0; i<256 ; ++i) {
        buffer[i] = i + 1;
    }

    f.write(reinterpret_cast<const char*>(buffer), 256);
    f.close();

    M::Integer fid = M::fileOpenRead(M::Tuple("test.bin"), true);
    QCOMPARE(fid != 0, true);

    unsigned char c = 1;
    for (unsigned i=1 ; i<=8 ; ++i) {
        union {
            M::Integer    integer;
            unsigned char bytes[sizeof(M::Integer)];
        } remap;

        M::Tuple   le      = M::fileReadInteger(fid, M::Integer(i));
        M::Integer r       = le[1].toInteger();
        M::Boolean success = le[2].toBoolean();

        QCOMPARE(success, true);

        remap.integer = r;

        for (unsigned j=0 ; j<i ; ++j) {
            QCOMPARE(remap.bytes[j], c + j);
        }

        c += i;

        M::Tuple be = M::fileReadInteger(fid, -M::Integer(i));
        r           = be[1].toInteger();
        success     = be[2].toBoolean();

        QCOMPARE(success, true);

        remap.integer = r;

        for (unsigned j=0 ; j<i ; ++j) {
            QCOMPARE(remap.bytes[j], c + i - j - 1);
        }

        c += i;
    }

    M::fileClose(fid);
}


void TestFileFunctions::testFileReadRealText() {
    std::ofstream f("test.txt");
    Q_ASSERT(f);

    f << "123 +123%-123+1.23-1.23+1.234E3-1.234E3+1.234E-3-1.234E-3";
    f.close();

    M::Integer fid = M::fileOpenRead(M::Tuple("test.txt"), false);
    QCOMPARE(fid != 0, true);

    M::Tuple   r1      = M::fileReadReal(fid);
    M::Real    r       = r1[1].toReal();
    M::Boolean success = r1[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(r, 123);

    M::Tuple   c1 = M::fileReadByte(fid);
    M::Integer c  = c1[1].toInteger();
    success       = c1[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(c, ' ');

    M::Tuple r2 = M::fileReadReal(fid);
    r           = r2[1].toReal();
    success     = r2[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(r, 123);

    M::Tuple c2 = M::fileReadByte(fid);
    c           = c2[1].toInteger();
    success     = c2[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(c, '%');

    M::Tuple r3 = M::fileReadReal(fid);
    r           = r3[1].toReal();
    success     = r3[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(r, -123);

    M::Tuple r4 = M::fileReadReal(fid);
    r           = r4[1].toReal();
    success     = r4[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(r, 1.23);

    M::Tuple r5 = M::fileReadReal(fid);
    r           = r5[1].toReal();
    success     = r5[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(r, -1.23);

    M::Tuple r6 = M::fileReadReal(fid);
    r           = r6[1].toReal();
    success     = r6[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(r, 1.234E3);

    M::Tuple r7 = M::fileReadReal(fid);
    r           = r7[1].toReal();
    success     = r7[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(r, -1.234E3);

    M::Tuple r8 = M::fileReadReal(fid);
    r           = r8[1].toReal();
    success     = r8[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(r, 1.234E-3);

    M::Tuple r9 = M::fileReadReal(fid);
    r           = r9[1].toReal();
    success     = r9[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(r, -1.234E-3);

    M::fileClose(fid);
}


void TestFileFunctions::testFileReadRealBinary() {
    std::ofstream f("test.bin", std::fstream::binary | std::fstream::trunc);
    Q_ASSERT(f);

    struct {
        union {
            std::uint8_t buffer[4];
            float        r;
        } lef;

        union {
            std::uint8_t buffer[4];
            float        r;
        } bef;

        union {
            std::uint8_t buffer[8];
            double       r;
        } led;

        union {
            std::uint8_t buffer[8];
            double       r;
        } bed;
    } buffer;

    buffer.lef.r = 1.23456F;
    buffer.led.r = 4.56789;

    buffer.bef.buffer[0] = buffer.lef.buffer[3];
    buffer.bef.buffer[1] = buffer.lef.buffer[2];
    buffer.bef.buffer[2] = buffer.lef.buffer[1];
    buffer.bef.buffer[3] = buffer.lef.buffer[0];

    buffer.bed.buffer[0] = buffer.led.buffer[7];
    buffer.bed.buffer[1] = buffer.led.buffer[6];
    buffer.bed.buffer[2] = buffer.led.buffer[5];
    buffer.bed.buffer[3] = buffer.led.buffer[4];
    buffer.bed.buffer[4] = buffer.led.buffer[3];
    buffer.bed.buffer[5] = buffer.led.buffer[2];
    buffer.bed.buffer[6] = buffer.led.buffer[1];
    buffer.bed.buffer[7] = buffer.led.buffer[0];

    f.write(reinterpret_cast<const char*>(&buffer), sizeof(buffer));
    f.close();

    M::Integer fid = M::fileOpenRead(M::Tuple("test.bin"), true);
    QCOMPARE(fid != 0, true);

    M::Tuple   r1      = M::fileReadReal(fid, M::Integer(+4));
    M::Real    r       = r1[1].toReal();
    M::Boolean success = r1[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(r, buffer.lef.r);

    M::Tuple r2 = M::fileReadReal(fid, M::Complex(-4));
    r           = r2[1].toReal();
    success     = r2[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(r, buffer.lef.r);

    M::Tuple r3 = M::fileReadReal(fid, M::Real(+8));
    r           = r3[1].toReal();
    success     = r3[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(r, buffer.led.r);

    M::Tuple r4 = M::fileReadReal(fid, M::Integer(-8));
    r           = r4[1].toReal();
    success     = r4[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(r, buffer.led.r);
}


void TestFileFunctions::testFileWriteByte() {
    M::Integer fid = M::fileOpenWriteTruncate(M::Tuple("test.bin"), true);
    QCOMPARE(fid != 0, true);

    for (M::Integer i=1 ; i<=255 ; ++i) {
        M::Boolean success = M::fileWriteByte(fid, i);
        QCOMPARE(success, true);
    }

    for (M::Real j=0.0 ; j<=255.0 ; ++j) {
        M::Boolean success = M::fileWriteByte(M::Complex(fid), j);
        QCOMPARE(success, true);
    }

    M::Boolean success = M::fileClose(fid);
    QCOMPARE(success, true);

    std::ifstream f("test.bin", std::ios::binary);

    std::uint8_t buffer[255 + 256];
    f.read(reinterpret_cast<char*>(buffer), 255 + 256);

    f.close();

    for (int k=0 ; k<(255 + 256) ; ++k) {
        QCOMPARE(buffer[k], std::uint8_t(k + 1));
    }
}


void TestFileFunctions::testFileWriteString() {
    M::Integer fid = M::fileOpenWriteTruncate(M::Tuple("test.txt"));
    QCOMPARE(fid != 0, true);

    M::Boolean success = M::fileWriteString(
        fid,
        M::Tuple("In the Northern Ocean there is a fish who's name is K\xC5\xABn --\n")
    );
    QCOMPARE(success, true);

    success = M::fileWriteString(fid, M::Tuple("I do not know how many l\xC7\x90 in size."), true);
    QCOMPARE(success, true);

    success = M::fileWriteString(fid, M::Tuple("It changes into a bird who's name is P\xC3\xA9ng."), true);
    QCOMPARE(success, true);

    success = M::fileClose(fid);
    QCOMPARE(success, true);

    std::ifstream f("test.txt");

    std::string line1;
    success = !!std::getline(f, line1);
    QCOMPARE(success, true);

    std::string line2;
    success = !!std::getline(f, line2);
    QCOMPARE(success, true);

    std::string line3;
    success = !!std::getline(f, line3);
    QCOMPARE(success, true);

    QCOMPARE(line1, "In the Northern Ocean there is a fish who's name is K\xC5\xABn --");
    QCOMPARE(line2, "I do not know how many l\xC7\x90 in size.");
    QCOMPARE(line3, "It changes into a bird who's name is P\xC3\xA9ng.");
}


void TestFileFunctions::testFileWriteIntegerText() {
    M::Integer fid = M::fileOpenWriteTruncate(M::Tuple("test.txt"));
    QCOMPARE(fid != 0, true);

    std::uint64_t s = 1;
    for (unsigned i=0 ; i<10 ; ++i) {
        s = s * 2862933555777941757 + 303700493; // Thansk to https://nuclear.linl.gov/CNP/rng/rngman/node4.html
        M::Boolean success = M::fileWriteInteger(fid, M::Integer(s));
        QCOMPARE(success, true);

        success = M::fileWriteString(fid, M::Tuple("\n"));
        QCOMPARE(success, true);
    }

    M::Boolean success = M::fileClose(fid);
    QCOMPARE(success, true);

    s = 1;
    std::ifstream f("test.txt");
    for (unsigned i=0 ; i<10 ; ++i) {
        std::uint64_t in;
        f >> in;
        QCOMPARE(!!f, true);

        s = s * 2862933555777941757 + 303700493; // Thansk to https://nuclear.linl.gov/CNP/rng/rngman/node4.html
        QCOMPARE(in, s);
    }

    f.close();
}


void TestFileFunctions::testFileWriteIntegerBinary() {
    M::Integer fid = M::fileOpenWriteTruncate(M::Tuple("test.bin"), true);
    QCOMPARE(fid != 0, true);

    std::uint64_t s = 1234567890123456LL;
    for (M::Integer l=-8 ; l<=+8 ; ++l) {
        if (l != 0) {
            s = s * 2862933555777941757 + 303700493; // Thansk to https://nuclear.linl.gov/CNP/rng/rngman/node4.html

            std::uint64_t mask    = (std::uint64_t(1) << (8 * std::abs(l))) - 1;
            std::uint64_t st      = s & mask;
            M::Boolean    success = M::fileWriteInteger(fid, M::Integer(st), l);
            QCOMPARE(success, true);
        }
    }

    M::Boolean success = M::fileClose(fid);
    QCOMPARE(success, true);

    s = 1234567890123456LL;

    fid = M::fileOpenRead(M::Tuple("test.bin"), true);
    QCOMPARE(fid != 0, true);

    for (M::Integer l=-8 ; l<=+8 ; ++l) {
        if (l != 0) {
            s = s * 2862933555777941757 + 303700493; // Thansk to https://nuclear.linl.gov/CNP/rng/rngman/node4.html

            std::uint64_t mask = (std::uint64_t(1) << (8 * std::abs(l))) - 1;
            std::uint64_t st   = s & mask;
            M::Tuple      r    = M::fileReadInteger(fid, l);

            M::Integer in = r[1].toInteger();
            success       = r[2].toBoolean();

            std::uint64_t inu = static_cast<uint64_t>(in) & mask;

            QCOMPARE(success, true);
            QCOMPARE(inu, st);
        }
    }

    M::fileClose(fid);
}


void TestFileFunctions::testFileWriteRealText() {
    M::Integer fid = M::fileOpenWriteTruncate(M::Tuple("test.txt"));
    QCOMPARE(fid != 0, true);

    std::uint64_t s = 9876543210987654LL;
    for (unsigned i=0 ; i<10 ; ++i) {
        s = s * 2862933555777941757 + 303700493; // Thansk to https://nuclear.linl.gov/CNP/rng/rngman/node4.html

        M::Real    r       = ((1.0 * s) / 0x7FFFFFFFFFFFFFFFULL) - 1.0;
        M::Boolean success = M::fileWriteReal(fid, r);
        QCOMPARE(success, true);

        success = M::fileWriteString(fid, "\n");
        QCOMPARE(success, true);
    }

    M::Boolean success = M::fileClose(fid);
    QCOMPARE(success, true);

    s = 9876543210987654LL;
    std::ifstream f("test.txt");
    for (unsigned i=0 ; i<10 ; ++i) {
        s = s * 2862933555777941757 + 303700493; // Thansk to https://nuclear.linl.gov/CNP/rng/rngman/node4.html
        M::Real expected = ((1.0 * s) / 0x7FFFFFFFFFFFFFFFULL) - 1.0;

        M::Real in;
        f >> in;
        QCOMPARE(!!f, true);

        M::Real relativeError = std::abs(in - expected)/expected;
        QCOMPARE(relativeError < 1.0E-15, true);
    }

    f.close();
}


void TestFileFunctions::testFileWriteReadBinary() {
    M::Integer fid = M::fileOpenWriteTruncate(M::Tuple("test.bin"));
    QCOMPARE(fid != 0, true);

    double r1 = 1.23456789;
    float  r2 = 2.34567890F;
    float  r3 = 3.45678901F;
    double r4 = 4.56789012;

    M::Boolean success = M::fileWriteReal(fid, r1, M::Real(-8));
    QCOMPARE(success, true);

    success = M::fileWriteReal(fid, r2, M::Integer(-4));
    QCOMPARE(success, true);

    success = M::fileWriteReal(M::Real(fid), r3, M::Integer(+4));
    QCOMPARE(success, true);

    success = M::fileWriteReal(fid, r4, M::Integer(+8));
    QCOMPARE(success, true);

    success = M::fileClose(fid);
    QCOMPARE(success, true);

    fid = M::fileOpenRead(M::Tuple("test.bin"));

    M::Tuple t1 = M::fileReadReal(fid, M::Integer(-8));
    M::Real  rr1 = t1[1].toReal();
    success      = t1[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(rr1, r1);

    M::Tuple t2 = M::fileReadReal(M::Real(fid), M::Integer(-4));
    M::Real  rr2 = t2[1].toReal();
    success      = t2[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(float(rr2), r2);

    M::Tuple t3 = M::fileReadReal(fid, M::Real(+4));
    M::Real  rr3 = t3[1].toReal();
    success      = t3[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(float(rr3), r3);

    M::Tuple t4 = M::fileReadReal(fid, M::Integer(+8));
    M::Real  rr4 = t4[1].toReal();
    success      = t4[2].toBoolean();

    QCOMPARE(success, true);
    QCOMPARE(rr4, r4);

    M::fileClose(fid);
}


void TestFileFunctions::testLoadBooleanMatrixCSV() {
    M::MatrixBoolean m1 = M::MatrixBoolean::build(
        4, 5,
         true, false, false, false,
        false,  true, false, false,
        false, false,  true, false,
        false, false, false,  true,
        false,  true, false, false
    );

    std::ofstream csvFile("matrix.csv");
    for (M::Integer row=1 ; row<=m1.numberRows() ; ++row) {
        for (M::Integer column=1 ; column<=m1.numberColumns() ; ++column) {
            if (column == 1) {
                csvFile << (m1.at(row, column) ? (row + column - 1) : 0);
            } else {
                csvFile << "\t" << (m1.at(row, column) ? (row + column - 1) : 0);
            }
        }

        csvFile << std::endl;
    }

    csvFile.close();

    M::MatrixBoolean m2 = M::loadBooleanMatrix(M::Tuple("matrix.csv"));
    QCOMPARE(m1, m2);
}


void TestFileFunctions::testSaveBooleanMatrixCSV() {
    M::MatrixBoolean m1 = M::MatrixBoolean::build(
        4, 5,
         true, false, false, false,
        false,  true, false, false,
        false, false,  true, false,
        false, false, false,  true,
        false,  true, false, false
    );

    M::Boolean success = M::saveBooleanMatrix(m1, M::Tuple("matrix.csv"));
    QCOMPARE(success, true);

    M::MatrixBoolean m2 = M::loadBooleanMatrix(M::Tuple("matrix.csv"));
    QCOMPARE(m1, m2);
}


void TestFileFunctions::testLoadSaveBooleanMatrixBinary() {
    M::MatrixBoolean m1 = M::MatrixBoolean::build(
        4, 5,
         true, false, false,  true,
        false,  true, false, false,
        false, false,  true, false,
        false, false, false,  true,
         true,  true, false, false
    );

    M::Boolean success = M::saveBooleanMatrix(m1, M::Tuple("matrix.bin"), true);
    QCOMPARE(success, true);

    M::MatrixBoolean m2 = M::loadBooleanMatrix(M::Tuple("matrix.bin"));
    QCOMPARE(m1, m2);
}


void TestFileFunctions::testLoadIntegerMatrixCSV() {
    M::MatrixInteger m1 = M::MatrixInteger::build(
        4, 5,
         1,  2,  3,  4,
         5,  6,  7,  8,
         9, 10, 11, 12,
        13, 14, 15, 16,
        17, 18, 19, 20
    );

    std::ofstream csvFile("matrix.csv");
    for (M::Integer row=1 ; row<=m1.numberRows() ; ++row) {
        for (M::Integer column=1 ; column<=m1.numberColumns() ; ++column) {
            if (column == 1) {
                csvFile << m1.at(row, column);
            } else {
                csvFile << "\t" << m1.at(row, column);
            }
        }

        csvFile << std::endl;
    }

    csvFile.close();

    M::MatrixInteger m2 = M::loadIntegerMatrix(M::Tuple("matrix.csv"));
    QCOMPARE(m1, m2);
}


void TestFileFunctions::testSaveIntegerMatrixCSV() {
    M::MatrixInteger m1 = M::MatrixInteger::build(
        4, 5,
        20, 19, 18, 17,
        16, 15, 14, 13,
        12, 11, 10,  9,
         8,  7,  6,  5,
         4,  3,  2,  1
    );

    M::Boolean success = M::saveIntegerMatrix(m1, M::Tuple("matrix.csv"));
    QCOMPARE(success, true);

    M::MatrixInteger m2 = M::loadIntegerMatrix(M::Tuple("matrix.csv"));
    QCOMPARE(m1, m2);
}


void TestFileFunctions::testLoadSaveIntegerMatrixBinary() {
    M::MatrixInteger m1 = M::MatrixInteger::build(
        4, 5,
        20, 19, 18, 17,
        16, 15, 14, 13,
        12, 11, 10,  9,
         8,  7,  6,  5,
         4,  3,  2,  1
    );

    M::Boolean success = M::saveIntegerMatrix(m1, M::Tuple("matrix.bin"), true);
    QCOMPARE(success, true);

    M::MatrixInteger m2 = M::loadIntegerMatrix(M::Tuple("matrix.bin"));
    QCOMPARE(m1, m2);
}


void TestFileFunctions::testLoadRealMatrixCSV() {
    M::MatrixReal m1 = M::MatrixReal::build(
        4, 5,
         1.2,  2.3,  3.4,  4.5,
         5.6,  6.7,  7.8,  8.9,
         9.0, 10.1, 11.2, 12.3,
        13.4, 14.5, 15.6, 16.7,
        17.8, 18.9, 19.0, 20.1
    );

    std::ofstream csvFile("matrix.csv");
    for (M::Integer row=1 ; row<=m1.numberRows() ; ++row) {
        for (M::Integer column=1 ; column<=m1.numberColumns() ; ++column) {
            if (column == 1) {
                csvFile << m1.at(row, column);
            } else {
                csvFile << "\t" << m1.at(row, column);
            }
        }

        csvFile << std::endl;
    }

    csvFile.close();

    M::MatrixReal m2 = M::loadRealMatrix(M::Tuple("matrix.csv"));
    QCOMPARE(m1, m2);
}


void TestFileFunctions::testSaveRealMatrixCSV() {
    M::MatrixReal m1 = M::MatrixReal::build(
        4, 5,
         1.9,  2.8,  3.7,  4.6,
         5.5,  6.4,  7.3,  8.2,
         9.1, 10.0, 11.9, 12.8,
        13.7, 14.6, 15.5, 16.4,
        17.3, 18.2, 19.1, 20.0
    );

    M::Boolean success = M::saveRealMatrix(m1, M::Tuple("matrix.csv"));
    QCOMPARE(success, true);

    M::MatrixReal m2 = M::loadRealMatrix(M::Tuple("matrix.csv"));
    QCOMPARE(m1, m2);
}


void TestFileFunctions::testLoadSaveRealMatrixBinary() {
    M::MatrixReal m1 = M::MatrixReal::build(
        4, 5,
         1.9,  2.8,  3.7,  4.6,
         5.5,  6.4,  7.3,  8.2,
         9.1, 10.0, 11.9, 12.8,
        13.7, 14.6, 15.5, 16.4,
        17.3, 18.2, 19.1, 20.0
    );

    M::Boolean success = M::saveRealMatrix(m1, M::Tuple("matrix.bin"), true);
    QCOMPARE(success, true);

    M::MatrixReal m2 = M::loadRealMatrix(M::Tuple("matrix.bin"));
    QCOMPARE(m1, m2);
}


void TestFileFunctions::testLoadComplexMatrixCSV() {
    M::MatrixComplex m1 = M::MatrixComplex::build(
        4, 5,
        M::Complex(1.2, 3.4), M::Complex(5.6, 7.8), M::Complex(9.0, 1.2), M::Complex(1.0, 9.8),
        M::Complex(7.6, 5.4), M::Complex(3.2, 1.0), M::Complex(0.1, 2.3), M::Complex(4.5, 6.7),
        M::Complex(8.9, 9.8), M::Complex(7.6, 5.4), M::Complex(3.2, 1.0), M::Complex(1.2, 3.4),
        M::Complex(5.6, 7.8), M::Complex(9.0, 9.8), M::Complex(7.6, 5.4), M::Complex(3.2, 1.0),
        M::Complex(1.2, 3.4), M::Complex(5.6, 7.8), M::Complex(9.0, 1.2), M::Complex(1.0, 9.8)
    );

    std::ofstream csvFile("matrix.csv");
    for (M::Integer row=1 ; row<=m1.numberRows() ; ++row) {
        for (M::Integer column=1 ; column<=m1.numberColumns() ; ++column) {
            if (column == 1) {
                csvFile << m1.at(row, column).real() << "\t" << m1.at(row, column).imag();
            } else {
                csvFile << "\t" << m1.at(row, column).real() << "\t" << m1.at(row, column).imag();
            }
        }

        csvFile << std::endl;
    }

    csvFile.close();

    M::MatrixComplex m2 = M::loadComplexMatrix(M::Tuple("matrix.csv"));
    QCOMPARE(m1, m2);
}


void TestFileFunctions::testSaveComplexMatrixCSV() {
    M::MatrixComplex m1 = M::MatrixComplex::build(
        4, 5,
        M::Complex(1.2, 3.4), M::Complex(5.6, 7.8), M::Complex(9.0, 1.2), M::Complex(1.0, 9.8),
        M::Complex(5.6, 7.8), M::Complex(9.0, 9.8), M::Complex(7.6, 5.4), M::Complex(3.2, 1.0),
        M::Complex(8.9, 9.8), M::Complex(7.6, 5.4), M::Complex(3.2, 1.0), M::Complex(1.2, 3.4),
        M::Complex(7.6, 5.4), M::Complex(3.2, 1.0), M::Complex(0.1, 2.3), M::Complex(4.5, 6.7),
        M::Complex(1.2, 3.4), M::Complex(5.6, 7.8), M::Complex(9.0, 1.2), M::Complex(1.0, 9.8)
    );

    M::Boolean success = M::saveComplexMatrix(m1, M::Tuple("matrix.csv"));
    QCOMPARE(success, true);

    M::MatrixComplex m2 = M::loadComplexMatrix(M::Tuple("matrix.csv"));
    QCOMPARE(m1, m2);
}


void TestFileFunctions::testLoadSaveComplexMatrixBinary() {
    M::MatrixComplex m1 = M::MatrixComplex::build(
        4, 5,
        M::Complex(1.2, 3.4), M::Complex(5.6, 7.8), M::Complex(9.0, 1.2), M::Complex(1.0, 9.8),
        M::Complex(5.6, 7.8), M::Complex(9.0, 9.8), M::Complex(7.6, 5.4), M::Complex(3.2, 1.0),
        M::Complex(8.9, 9.8), M::Complex(7.6, 5.4), M::Complex(3.2, 1.0), M::Complex(1.2, 3.4),
        M::Complex(7.6, 5.4), M::Complex(3.2, 1.0), M::Complex(0.1, 2.3), M::Complex(4.5, 6.7),
        M::Complex(1.2, 3.4), M::Complex(5.6, 7.8), M::Complex(9.0, 1.2), M::Complex(1.0, 9.8)
    );

    M::Boolean success = M::saveComplexMatrix(m1, M::Tuple("matrix.bin"), true);
    QCOMPARE(success, true);

    M::MatrixComplex m2 = M::loadComplexMatrix(M::Tuple("matrix.bin"));
    QCOMPARE(m1, m2);
}



void TestFileFunctions::testLoadWavFile() {}


void TestFileFunctions::buildTextFile(const char* filename) {
    std::ofstream f(filename);
    Q_ASSERT(f);

    f << "This is an example text file" << std::endl
      << "containing multiple lines." << std::endl
      << "\xC3\x97 \xD7\x91 \xEF\xAC\xA8 \xF0\x9D\x94\x84 \xE2\x84\xA8" << std::endl
      << "12 345 6789 103456 67801 0x12 0x1234 0b00010010 0b0001001000110100";

    f.close();
}



