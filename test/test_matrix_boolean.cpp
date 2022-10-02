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
* This file implements tests of the \ref Model::MatrixBoolean class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QObject>
#include <QList>
#include <QtTest/QtTest>

#include <limits>
#include <complex>
#include <cmath>
#include <random>
#include <algorithm>
#include <cstdint>

#include <model_exceptions.h>
#include <model_tuple.h>
#include <model_variant.h>
#include <m_basic_functions.h>
#include <model_matrix_integer.h>
#include <m_matrix_boolean.h>
#include <m_variant.h>
#include <m_range.h>
#include <m_set.h>
#include <m_tuple.h>
#include <m_matrix_real.h>
#include <m_matrix_complex.h>
#include <model_matrix_boolean.h>

#include "test_matrix_base.h"
#include "test_matrix_boolean.h"

TestMatrixBoolean::TestMatrixBoolean() {}


TestMatrixBoolean::~TestMatrixBoolean() {}


void TestMatrixBoolean::testConstructorAndDestructors() {
    Model::MatrixBoolean m1;
    QCOMPARE(m1.numberRows(), 0);
    QCOMPARE(m1.numberColumns(), 0);

    Model::MatrixBoolean m2(3, 3);
    QCOMPARE(m2.numberRows(), 3);
    QCOMPARE(m2.numberColumns(), 3);

    QCOMPARE(m2(1,1), false);
    QCOMPARE(m2(1,2), false);
    QCOMPARE(m2(1,3), false);
    QCOMPARE(m2(2,1), false);
    QCOMPARE(m2(2,2), false);
    QCOMPARE(m2(2,3), false);
    QCOMPARE(m2(3,1), false);
    QCOMPARE(m2(3,2), false);
    QCOMPARE(m2(3,3), false);

    m2.update(1, 1, true);
    m2.update(1, 2, false);
    m2.update(1, 3, false);
    m2.update(2, 1, false);
    m2.update(2, 2, true);
    m2.update(2, 3, false);
    m2.update(3, 1, true);
    m2.update(3, 2, false);
    m2.update(3, 3, true);

    const Model::Boolean matrixData[] = {
        true,  false,
        false, true
    };

    Model::MatrixBoolean m3(2, 2, matrixData);
    QCOMPARE(m3.numberRows(), 2);
    QCOMPARE(m3.numberColumns(), 2);

    QCOMPARE(m3(1,1), true);
    QCOMPARE(m3(1,2), false);
    QCOMPARE(m3(2,1), false);
    QCOMPARE(m3(2,2), true);

    Model::MatrixBoolean m4(m2);
    QCOMPARE(m4.numberRows(), 3);
    QCOMPARE(m4.numberColumns(), 3);

    QCOMPARE(m4(1,1), true);
    QCOMPARE(m4(1,2), false);
    QCOMPARE(m4(1,3), false);
    QCOMPARE(m4(2,1), false);
    QCOMPARE(m4(2,2), true);
    QCOMPARE(m4(2,3), false);
    QCOMPARE(m4(3,1), true);
    QCOMPARE(m4(3,2), false);
    QCOMPARE(m4(3,3), true);

    std::mt19937                    rng(0x12345678);
    std::uniform_int_distribution<> coefficientDistribution(-1E6, +1E6);
    std::uniform_int_distribution<> dimensionDistribution(1, 1000);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned long numberRows         = dimensionDistribution(rng);
        unsigned long numberColumns      = dimensionDistribution(rng);
        unsigned long numberCoefficients = numberRows * numberColumns;

        Model::Boolean* booleanData = new Model::Boolean[numberCoefficients];
        Model::Boolean* b           = booleanData;

        for (unsigned long columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
            for (unsigned long rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                Model::Boolean v = (coefficientDistribution(rng) < 0);
                *b = v;
                ++b;
            }
        }

        Model::MatrixBoolean matrix(numberRows, numberColumns, booleanData);

        b = booleanData;
        for (unsigned long columnIndex=1 ; columnIndex<=numberColumns ; ++columnIndex) {
            for (unsigned long rowIndex=1 ; rowIndex<=numberRows ; ++rowIndex) {
                Model::Boolean expected = *b;
                QCOMPARE(matrix(rowIndex, columnIndex), expected);

                ++b;
            }
        }
    }
}


void TestMatrixBoolean::testCoefficientValueType() {
    Model::MatrixBoolean m;
    QCOMPARE(m.coefficientValueType(), Model::ValueType::BOOLEAN);
}


void TestMatrixBoolean::testNumberRowsColumns() {
    Model::MatrixBoolean m1(2, 3);

    QCOMPARE(m1.numberRows(), 2);
    QCOMPARE(m1.numberColumns(), 3);
    QCOMPARE(m1.numberCoefficients(), 6);
}


void TestMatrixBoolean::testValueMethods() {
    Model::MatrixBoolean m1;

    bool success = m1.setValue(1, 1, true);
    QVERIFY(success);

    Model::Variant v11 = m1.value(1, 1);
    QCOMPARE(v11.valueType(), Model::ValueType::BOOLEAN);
    QCOMPARE(v11, Model::Variant(Model::Boolean(true)));

    success = m1.setValue(1, 2, false);
    QVERIFY(success);

    v11 = m1.value(1, 1);
    Model::Variant v12 = m1.value(1, 2);

    QCOMPARE(v11.valueType(), Model::ValueType::BOOLEAN);
    QCOMPARE(v12.valueType(), Model::ValueType::BOOLEAN);

    QCOMPARE(v11, Model::Variant(Model::Boolean(true)));
    QCOMPARE(v12, Model::Variant(Model::Boolean(false)));

    success = m1.setValue(2, 1, false);
    QVERIFY(success);

    v11 = m1.value(1, 1);
    v12 = m1.value(1, 2);
    Model::Variant v21 = m1.value(2, 1);

    QCOMPARE(v11.valueType(), Model::ValueType::BOOLEAN);
    QCOMPARE(v12.valueType(), Model::ValueType::BOOLEAN);
    QCOMPARE(v21.valueType(), Model::ValueType::BOOLEAN);

    QCOMPARE(v11, Model::Variant(Model::Boolean(true)));
    QCOMPARE(v12, Model::Variant(Model::Boolean(false)));
    QCOMPARE(v21, Model::Variant(Model::Boolean(false)));

    success = m1.setValue(4, true);
    QVERIFY(success);

    QCOMPARE(m1.numberRows(), 2);
    QCOMPARE(m1.numberColumns(), 2);

    v11 = m1.value(1, 1);
    v12 = m1.value(1, 2);
    v21 = m1.value(2, 1);
    Model::Variant v22 = m1.value(2, 2);

    QCOMPARE(v11.valueType(), Model::ValueType::BOOLEAN);
    QCOMPARE(v12.valueType(), Model::ValueType::BOOLEAN);
    QCOMPARE(v21.valueType(), Model::ValueType::BOOLEAN);
    QCOMPARE(v22.valueType(), Model::ValueType::BOOLEAN);

    QCOMPARE(v11, Model::Variant(Model::Boolean(true)));
    QCOMPARE(v12, Model::Variant(Model::Boolean(false)));
    QCOMPARE(v21, Model::Variant(Model::Boolean(false)));
    QCOMPARE(v22, Model::Variant(Model::Boolean(true)));

    QCOMPARE(m1.value(1), Model::Variant(Model::Boolean(true)));
    QCOMPARE(m1.value(2), Model::Variant(Model::Boolean(false)));
    QCOMPARE(m1.value(3), Model::Variant(Model::Boolean(false)));
    QCOMPARE(m1.value(4), Model::Variant(Model::Boolean(true)));
}


void TestMatrixBoolean::testInternalAtMethods() {
    M::MatrixBoolean m(9, 9);
    for (M::Integer row=1 ; row<=9 ; ++row) {
        for (M::Integer col=1 ; col<=9 ; ++col) {
            m.update(row, col, boolify(10 * row + col));
        }
    }

    // T F T T F T F F T
    // T T F F T T F T F
    // T T F F T F T T F
    // T T F T F F T F T
    // F T F F T T F F T
    // T T F T F F T F T
    // F F T T F T F F T
    // T T F T F F T T F
    // T F T T F F T T F

    M::Integer       i = 3;
    M::Real          r = 4;
    M::Complex       c(5);
    M::Range         rng(M::Integer(5), M::Integer(6));
    M::Set           set   = M::Set::build(M::Integer(2), M::Integer(4));
    M::Tuple         tuple = M::Tuple::build(M::Integer(3), M::Integer(5));
    M::MatrixInteger mi = M::MatrixInteger::build(2, 2, M::Integer(2), M::Integer(6), M::Integer(4), M::Integer(8));
    M::MatrixReal    mr = M::MatrixReal::build(2, 2, M::Real(1), M::Real(3), M::Real(2), M::Real(4));
    M::MatrixComplex mc = M::MatrixComplex::build(1, 2, M::Complex(1), M::Complex(5));

    M::Variant  vi(i);
    M::Variant  vr(r);
    M::Variant  vc(c);
    M::Variant  vset(set);
    M::Variant  vtuple(tuple);
    M::Variant  vmi(mi);
    M::Variant  vmr(mr);
    M::Variant  vmc(mc);

    QCOMPARE(m.at(i, i), boolify(33));
    QCOMPARE(m.at(i, r), boolify(34));
    QCOMPARE(m.at(i, c), boolify(35));
    QCOMPARE(m.at(i, rng), M::MatrixBoolean::build(1, 2, boolify(35), boolify(36)));
    QCOMPARE(m.at(i, set), M::MatrixBoolean::build(1, 2, boolify(32), boolify(34)));
    QCOMPARE(m.at(i, tuple), M::MatrixBoolean::build(1, 2, boolify(33), boolify(35)));
    QCOMPARE(
        m.at(i, mi),
        M::MatrixBoolean::build(1, 4, boolify(32), boolify(34), boolify(36), boolify(38))
    );
    QCOMPARE(
        m.at(i, mr),
        M::MatrixBoolean::build(1, 4, boolify(31), boolify(32), boolify(33), boolify(34))
    );
    QCOMPARE(m.at(i, mc), M::MatrixBoolean::build(1, 2, boolify(31), boolify(35)));

    QCOMPARE(m.at(r, i), boolify(43));
    QCOMPARE(m.at(r, r), boolify(44));
    QCOMPARE(m.at(r, c), boolify(45));
    QCOMPARE(m.at(r, rng), M::MatrixBoolean::build(1, 2, boolify(45), boolify(46)));
    QCOMPARE(m.at(r, set), M::MatrixBoolean::build(1, 2, boolify(42), boolify(44)));
    QCOMPARE(m.at(r, tuple), M::MatrixBoolean::build(1, 2, boolify(43), boolify(45)));
    QCOMPARE(
        m.at(r, mi),
        M::MatrixBoolean::build(1, 4, boolify(42), boolify(44), boolify(46), boolify(48))
    );
    QCOMPARE(
        m.at(r, mr),
        M::MatrixBoolean::build(1, 4, boolify(41), boolify(42), boolify(43), boolify(44))
    );
    QCOMPARE(m.at(r, mc), M::MatrixBoolean::build(1, 2, boolify(41), boolify(45)));

    QCOMPARE(m.at(c, i), boolify(53));
    QCOMPARE(m.at(c, r), boolify(54));
    QCOMPARE(m.at(c, c), boolify(55));
    QCOMPARE(m.at(c, rng), M::MatrixBoolean::build(1, 2, boolify(55), boolify(56)));
    QCOMPARE(m.at(c, set), M::MatrixBoolean::build(1, 2, boolify(52), boolify(54)));
    QCOMPARE(m.at(c, tuple), M::MatrixBoolean::build(1, 2, boolify(53), boolify(55)));
    QCOMPARE(
        m.at(c, mi),
        M::MatrixBoolean::build(1, 4, boolify(52), boolify(54), boolify(56), boolify(58))
    );
    QCOMPARE(
        m.at(c, mr),
        M::MatrixBoolean::build(1, 4, boolify(51), boolify(52), boolify(53), boolify(54))
    );
    QCOMPARE(m.at(c, mc), M::MatrixBoolean::build(1, 2, boolify(51), boolify(55)));

    QCOMPARE(m.at(rng, i), M::MatrixBoolean::build(2, 1, boolify(53), boolify(63)));
    QCOMPARE(m.at(rng, r), M::MatrixBoolean::build(2, 1, boolify(54), boolify(64)));
    QCOMPARE(m.at(rng, c), M::MatrixBoolean::build(2, 1, boolify(55), boolify(65)));
    QCOMPARE(
        m.at(rng, rng),
        M::MatrixBoolean::build(
            2, 2,
            boolify(55), boolify(65),
            boolify(56), boolify(66)
        )
    );
    QCOMPARE(
        m.at(rng, set),
        M::MatrixBoolean::build(
            2, 2,
            boolify(52), boolify(62),
            boolify(54), boolify(64)
        )
    );
    QCOMPARE(
        m.at(rng, tuple),
        M::MatrixBoolean::build(
            2, 2,
            boolify(53), boolify(63),
            boolify(55), boolify(65)
        )
    );
    QCOMPARE(
        m.at(rng, mi),
        M::MatrixBoolean::build(
            2, 4,
            boolify(52), boolify(62),
            boolify(54), boolify(64),
            boolify(56), boolify(66),
            boolify(58), boolify(68)
        )
    );
    QCOMPARE(
        m.at(rng, mr),
        M::MatrixBoolean::build(
            2, 4,
            boolify(51), boolify(61),
            boolify(52), boolify(62),
            boolify(53), boolify(63),
            boolify(54), boolify(64)
        )
    );
    QCOMPARE(
        m.at(rng, mc),
        M::MatrixBoolean::build(
            2, 2,
            boolify(51), boolify(61),
            boolify(55), boolify(65)
        )
    );

    QCOMPARE(m.at(set, i), M::MatrixBoolean::build(2, 1, boolify(23), boolify(43)));
    QCOMPARE(m.at(set, r), M::MatrixBoolean::build(2, 1, boolify(24), boolify(44)));
    QCOMPARE(m.at(set, c), M::MatrixBoolean::build(2, 1, boolify(25), boolify(45)));
    QCOMPARE(
        m.at(set, rng),
        M::MatrixBoolean::build(
            2, 2,
            boolify(25), boolify(45),
            boolify(26), boolify(46)
        )
    );
    QCOMPARE(
        m.at(set, set),
        M::MatrixBoolean::build(
            2, 2,
            boolify(22), boolify(42),
            boolify(24), boolify(44)
        )
    );
    QCOMPARE(
        m.at(set, tuple),
        M::MatrixBoolean::build(
            2, 2,
            boolify(23), boolify(43),
            boolify(25), boolify(45)
        )
    );
    QCOMPARE(
        m.at(set, mi),
        M::MatrixBoolean::build(
            2, 4,
            boolify(22), boolify(42),
            boolify(24), boolify(44),
            boolify(26), boolify(46),
            boolify(28), boolify(48)
        )
    );
    QCOMPARE(
        m.at(set, mr),
        M::MatrixBoolean::build(
            2, 4,
            boolify(21), boolify(41),
            boolify(22), boolify(42),
            boolify(23), boolify(43),
            boolify(24), boolify(44)
        )
    );
    QCOMPARE(
        m.at(set, mc),
        M::MatrixBoolean::build(
            2, 2,
            boolify(21), boolify(41),
            boolify(25), boolify(45)
        )
    );

    QCOMPARE(m.at(tuple, i), M::MatrixBoolean::build(2, 1, boolify(33), boolify(53)));
    QCOMPARE(m.at(tuple, r), M::MatrixBoolean::build(2, 1, boolify(34), boolify(54)));
    QCOMPARE(m.at(tuple, c), M::MatrixBoolean::build(2, 1, boolify(35), boolify(55)));
    QCOMPARE(
        m.at(tuple, rng),
        M::MatrixBoolean::build(
            2, 2,
            boolify(35), boolify(55),
            boolify(36), boolify(56)
        )
    );
    QCOMPARE(
        m.at(tuple, set),
        M::MatrixBoolean::build(
            2, 2,
            boolify(32), boolify(52),
            boolify(34), boolify(54)
        )
    );
    QCOMPARE(
        m.at(tuple, tuple),
        M::MatrixBoolean::build(
            2, 2,
            boolify(33), boolify(53),
            boolify(35), boolify(55)
        )
    );
    QCOMPARE(
        m.at(tuple, mi),
        M::MatrixBoolean::build(
            2, 4,
            boolify(32), boolify(52),
            boolify(34), boolify(54),
            boolify(36), boolify(56),
            boolify(38), boolify(58)
        )
    );
    QCOMPARE(
        m.at(tuple, mr),
        M::MatrixBoolean::build(
            2, 4,
            boolify(31), boolify(51),
            boolify(32), boolify(52),
            boolify(33), boolify(53),
            boolify(34), boolify(54)
        )
    );
    QCOMPARE(
        m.at(tuple, mc),
        M::MatrixBoolean::build(
            2, 2,
            boolify(31), boolify(51),
            boolify(35), boolify(55)
        )
    );

    QCOMPARE(m.at(mi, i), M::MatrixBoolean::build(4, 1, boolify(23), boolify(43), boolify(63), boolify(83)));
    QCOMPARE(m.at(mi, r), M::MatrixBoolean::build(4, 1, boolify(24), boolify(44), boolify(64), boolify(84)));
    QCOMPARE(m.at(mi, c), M::MatrixBoolean::build(4, 1, boolify(25), boolify(45), boolify(65), boolify(85)));
    QCOMPARE(
        m.at(mi, rng),
        M::MatrixBoolean::build(
            4, 2,
            boolify(25), boolify(45), boolify(65), boolify(85),
            boolify(26), boolify(46), boolify(66), boolify(86)
        )
    );
    QCOMPARE(
        m.at(mi, set),
        M::MatrixBoolean::build(
            4, 2,
            boolify(22), boolify(42), boolify(62), boolify(82),
            boolify(24), boolify(44), boolify(64), boolify(84)
        )
    );
    QCOMPARE(
        m.at(mi, tuple),
        M::MatrixBoolean::build(
            4, 2,
            boolify(23), boolify(43), boolify(63), boolify(83),
            boolify(25), boolify(45), boolify(65), boolify(85)
        )
    );
    QCOMPARE(
        m.at(mi, mi),
        M::MatrixBoolean::build(
            4, 4,
            boolify(22), boolify(42), boolify(62), boolify(82),
            boolify(24), boolify(44), boolify(64), boolify(84),
            boolify(26), boolify(46), boolify(66), boolify(86),
            boolify(28), boolify(48), boolify(68), boolify(88)
        )
    );
    QCOMPARE(
        m.at(mi, mr),
        M::MatrixBoolean::build(
            4, 4,
            boolify(21), boolify(41), boolify(61), boolify(81),
            boolify(22), boolify(42), boolify(62), boolify(82),
            boolify(23), boolify(43), boolify(63), boolify(83),
            boolify(24), boolify(44), boolify(64), boolify(84)
        )
    );
    QCOMPARE(
        m.at(mi, mc),
        M::MatrixBoolean::build(
            4, 2,
            boolify(21), boolify(41), boolify(61), boolify(81),
            boolify(25), boolify(45), boolify(65), boolify(85)
        )
    );

    QCOMPARE(m.at(mr, i), M::MatrixBoolean::build(4, 1, boolify(13), boolify(23), boolify(33), boolify(43)));
    QCOMPARE(m.at(mr, r), M::MatrixBoolean::build(4, 1, boolify(14), boolify(24), boolify(34), boolify(44)));
    QCOMPARE(m.at(mr, c), M::MatrixBoolean::build(4, 1, boolify(15), boolify(25), boolify(35), boolify(45)));
    QCOMPARE(
        m.at(mr, rng),
        M::MatrixBoolean::build(
            4, 2,
            boolify(15), boolify(25), boolify(35), boolify(45),
            boolify(16), boolify(26), boolify(36), boolify(46)
        )
    );
    QCOMPARE(
        m.at(mr, set),
        M::MatrixBoolean::build(
            4, 2,
            boolify(12), boolify(22), boolify(32), boolify(42),
            boolify(14), boolify(24), boolify(34), boolify(44)
        )
    );
    QCOMPARE(
        m.at(mr, tuple),
        M::MatrixBoolean::build(
            4, 2,
            boolify(13), boolify(23), boolify(33), boolify(43),
            boolify(15), boolify(25), boolify(35), boolify(45)
        )
    );
    QCOMPARE(
        m.at(mr, mi),
        M::MatrixBoolean::build(
            4, 4,
            boolify(12), boolify(22), boolify(32), boolify(42),
            boolify(14), boolify(24), boolify(34), boolify(44),
            boolify(16), boolify(26), boolify(36), boolify(46),
            boolify(18), boolify(28), boolify(38), boolify(48)
        )
    );
    QCOMPARE(
        m.at(mr, mr),
        M::MatrixBoolean::build(
            4, 4,
            boolify(11), boolify(21), boolify(31), boolify(41),
            boolify(12), boolify(22), boolify(32), boolify(42),
            boolify(13), boolify(23), boolify(33), boolify(43),
            boolify(14), boolify(24), boolify(34), boolify(44)
        )
    );
    QCOMPARE(
        m.at(mr, mc),
        M::MatrixBoolean::build(
            4, 2,
            boolify(11), boolify(21), boolify(31), boolify(41),
            boolify(15), boolify(25), boolify(35), boolify(45)
        )
    );

    QCOMPARE(m.at(mc, i), M::MatrixBoolean::build(2, 1, boolify(13), boolify(53)));
    QCOMPARE(m.at(mc, r), M::MatrixBoolean::build(2, 1, boolify(14), boolify(54)));
    QCOMPARE(m.at(mc, c), M::MatrixBoolean::build(2, 1, boolify(15), boolify(55)));
    QCOMPARE(
        m.at(mc, rng),
        M::MatrixBoolean::build(
            2, 2,
            boolify(15), boolify(55),
            boolify(16), boolify(56)
        )
    );
    QCOMPARE(
        m.at(mc, set),
        M::MatrixBoolean::build(
            2, 2,
            boolify(12), boolify(52),
            boolify(14), boolify(54)
        )
    );
    QCOMPARE(
        m.at(mc, tuple),
        M::MatrixBoolean::build(
            2, 2,
            boolify(13), boolify(53),
            boolify(15), boolify(55)
        )
    );
    QCOMPARE(
        m.at(mc, mi),
        M::MatrixBoolean::build(
            2, 4,
            boolify(12), boolify(52),
            boolify(14), boolify(54),
            boolify(16), boolify(56),
            boolify(18), boolify(58)
        )
    );
    QCOMPARE(
        m.at(mc, mr),
        M::MatrixBoolean::build(
            2, 4,
            boolify(11), boolify(51),
            boolify(12), boolify(52),
            boolify(13), boolify(53),
            boolify(14), boolify(54)
        )
    );
    QCOMPARE(
        m.at(mc, mc),
        M::MatrixBoolean::build(
            2, 2,
            boolify(11), boolify(51),
            boolify(15), boolify(55)
        )
    );


    QCOMPARE(m.at(vi, i), m.at(i, i));
    QCOMPARE(m.at(vi, r), m.at(i, r));
    QCOMPARE(m.at(vi, c), m.at(i, c));
    QCOMPARE(m.at(vi, set), m.at(i, set));
    QCOMPARE(m.at(vi, tuple), m.at(i, tuple));
    QCOMPARE(m.at(vi, mi), m.at(i, mi));
    QCOMPARE(m.at(vi, mr), m.at(i, mr));
    QCOMPARE(m.at(vi, mc), m.at(i, mc));


    QCOMPARE(m.at(i, vi), m.at(i, i));
    QCOMPARE(m.at(r, vi), m.at(r, i));
    QCOMPARE(m.at(c, vi), m.at(c, i));
    QCOMPARE(m.at(set, vi), m.at(set, i));
    QCOMPARE(m.at(tuple, vi), m.at(tuple, i));
    QCOMPARE(m.at(mi, vi), m.at(mi, i));
    QCOMPARE(m.at(mr, vi), m.at(mr, i));
    QCOMPARE(m.at(mc, vi), m.at(mc, i));


    QCOMPARE(m.at(vi, vi), m.at(i, i));
    QCOMPARE(m.at(vi, vr), m.at(i, r));
    QCOMPARE(m.at(vi, vc), m.at(i, c));
    QCOMPARE(m.at(vi, vset), m.at(i, set));
    QCOMPARE(m.at(vi, vtuple), m.at(i, tuple));
    QCOMPARE(m.at(vi, vmi), m.at(i, mi));
    QCOMPARE(m.at(vi, vmr), m.at(i, mr));
    QCOMPARE(m.at(vi, vmc), m.at(i, mc));

    QCOMPARE(m.at(vr, vi), m.at(r, i));
    QCOMPARE(m.at(vr, vr), m.at(r, r));
    QCOMPARE(m.at(vr, vc), m.at(r, c));
    QCOMPARE(m.at(vr, vset), m.at(r, set));
    QCOMPARE(m.at(vr, vtuple), m.at(r, tuple));
    QCOMPARE(m.at(vr, vmi), m.at(r, mi));
    QCOMPARE(m.at(vr, vmr), m.at(r, mr));
    QCOMPARE(m.at(vr, vmc), m.at(r, mc));

    QCOMPARE(m.at(vc, vi), m.at(c, i));
    QCOMPARE(m.at(vc, vr), m.at(c, r));
    QCOMPARE(m.at(vc, vc), m.at(c, c));
    QCOMPARE(m.at(vc, vset), m.at(c, set));
    QCOMPARE(m.at(vc, vtuple), m.at(c, tuple));
    QCOMPARE(m.at(vc, vmi), m.at(c, mi));
    QCOMPARE(m.at(vc, vmr), m.at(c, mr));
    QCOMPARE(m.at(vc, vmc), m.at(c, mc));

    QCOMPARE(m.at(vset, vi), m.at(set, i));
    QCOMPARE(m.at(vset, vr), m.at(set, r));
    QCOMPARE(m.at(vset, vc), m.at(set, c));
    QCOMPARE(m.at(vset, vset), m.at(set, set));
    QCOMPARE(m.at(vset, vtuple), m.at(set, tuple));
    QCOMPARE(m.at(vset, vmi), m.at(set, mi));
    QCOMPARE(m.at(vset, vmr), m.at(set, mr));
    QCOMPARE(m.at(vset, vmc), m.at(set, mc));

    QCOMPARE(m.at(vtuple, vi), m.at(tuple, i));
    QCOMPARE(m.at(vtuple, vr), m.at(tuple, r));
    QCOMPARE(m.at(vtuple, vc), m.at(tuple, c));
    QCOMPARE(m.at(vtuple, vset), m.at(tuple, set));
    QCOMPARE(m.at(vtuple, vtuple), m.at(tuple, tuple));
    QCOMPARE(m.at(vtuple, vmi), m.at(tuple, mi));
    QCOMPARE(m.at(vtuple, vmr), m.at(tuple, mr));
    QCOMPARE(m.at(vtuple, vmc), m.at(tuple, mc));

    QCOMPARE(m.at(vmi, vi), m.at(mi, i));
    QCOMPARE(m.at(vmi, vr), m.at(mi, r));
    QCOMPARE(m.at(vmi, vc), m.at(mi, c));
    QCOMPARE(m.at(vmi, vset), m.at(mi, set));
    QCOMPARE(m.at(vmi, vtuple), m.at(mi, tuple));
    QCOMPARE(m.at(vmi, vmi), m.at(mi, mi));
    QCOMPARE(m.at(vmi, vmr), m.at(mi, mr));
    QCOMPARE(m.at(vmi, vmc), m.at(mi, mc));

    QCOMPARE(m.at(vmr, vi), m.at(mr, i));
    QCOMPARE(m.at(vmr, vr), m.at(mr, r));
    QCOMPARE(m.at(vmr, vc), m.at(mr, c));
    QCOMPARE(m.at(vmr, vset), m.at(mr, set));
    QCOMPARE(m.at(vmr, vtuple), m.at(mr, tuple));
    QCOMPARE(m.at(vmr, vmi), m.at(mr, mi));
    QCOMPARE(m.at(vmr, vmr), m.at(mr, mr));
    QCOMPARE(m.at(vmr, vmc), m.at(mr, mc));

    QCOMPARE(m.at(vmc, vi), m.at(mc, i));
    QCOMPARE(m.at(vmc, vr), m.at(mc, r));
    QCOMPARE(m.at(vmc, vc), m.at(mc, c));
    QCOMPARE(m.at(vmc, vset), m.at(mc, set));
    QCOMPARE(m.at(vmc, vtuple), m.at(mc, tuple));
    QCOMPARE(m.at(vmc, vmi), m.at(mc, mi));
    QCOMPARE(m.at(vmc, vmr), m.at(mc, mr));
    QCOMPARE(m.at(vmc, vmc), m.at(mc, mc));
}


void TestMatrixBoolean::testResizeMethod() {
    std::mt19937                    rng(0x12345678);
    std::uniform_int_distribution<> coefficientDistribution(-1E6, +1E6);
    std::uniform_int_distribution<> dimensionDistribution(1, 1000);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned long initialNumberRows         = dimensionDistribution(rng);
        unsigned long initialNumberColumns      = dimensionDistribution(rng);
        unsigned long initialNumberCoefficients = initialNumberRows * initialNumberColumns;

        bool* values = new bool[initialNumberCoefficients];
        bool* v      = values;

        // Note: the approach used to build the matirx will trigger a large number of internal resize operations so
        //       just building the matrix without pre-sizing it will test the resize functions.

        Model::MatrixBoolean initial;
        for (unsigned long column=1 ; column<=initialNumberColumns ; ++column) {
            for (unsigned long row=1 ; row<=initialNumberRows ; ++row) {
                bool c = (coefficientDistribution(rng) > 0);
                *v++ = c;

                initial.update(row, column, c);

//                bool* vt = values;
//                for (unsigned long testColumn=1 ; testColumn<=column ; ++testColumn) {
//                    unsigned long testFinalRow = testColumn < column ? initialNumberRows : row;
//                    for (unsigned long testRow=1 ; testRow<=testFinalRow ; ++testRow) {
//                        bool expected = *vt++;
//                        bool measured = initial(testRow, testColumn);

//                        if (expected != measured) {
//                            QCOMPARE(expected, measured);
//                        }
//                    }
//                }
            }
        }

        unsigned long newNumberRows    = dimensionDistribution(rng);
        unsigned long newNumberColumns = dimensionDistribution(rng);

        v = values;
        for (unsigned long column=1 ; column<=initialNumberColumns ; ++column) {
            for (unsigned long row=1 ; row<=initialNumberRows ; ++row) {
                bool expected = *v++;
                bool measured;
                measured = initial(row, column);

                if (expected != measured) {
                    QCOMPARE(expected, measured);
                }
            }
        }

        delete[] values;

        Model::MatrixBoolean resized = initial;

        resized.resize(newNumberRows, newNumberColumns);

        unsigned long comparedRows = std::min(newNumberRows, initialNumberRows);
        unsigned long comparedColumns = std::min(newNumberColumns, initialNumberColumns);

        for (unsigned long column=1 ; column<=comparedColumns ; ++column) {
            for (unsigned long row=1 ; row<=comparedRows ; ++row) {
                bool expected = initial(row, column);
                bool measured = resized(row, column);

                if (expected != measured) {
                    QCOMPARE(expected, measured);
                }
            }
        }

        if (comparedColumns < newNumberColumns) {
            for (unsigned long column=comparedColumns+1 ; column<=newNumberColumns ; ++column) {
                for (unsigned row=1 ; row<=newNumberRows ; ++row) {
                    bool measured = resized(row, column);
                    if (measured != false) {
                        QCOMPARE(false, measured);
                    }
                }
            }
        }

        if (comparedRows < newNumberRows) {
            for (unsigned long column=1 ; column<=comparedColumns ; ++column) {
                for (unsigned long row=comparedRows+1 ; row<=newNumberRows ; ++row) {
                    bool measured = resized(row, column);
                    if (measured != false) {
                        QCOMPARE(false, measured);
                    }
                }
            }
        }
    }
}


void TestMatrixBoolean::testCoefficientAccessorMethods() {
    Model::MatrixBoolean m1;
    QCOMPARE(m1.numberRows(), 0);
    QCOMPARE(m1.numberColumns(), 0);

    m1.update(1,1, Model::Boolean(true));
    QCOMPARE(m1.numberRows(), 1);
    QCOMPARE(m1.numberColumns(), 1);

    m1.update(1,2, Model::Boolean(false));
    QCOMPARE(m1.numberRows(), 1);
    QCOMPARE(m1.numberColumns(), 2);

    m1.update(2,1, Model::Boolean(false));
    QCOMPARE(m1.numberRows(), 2);
    QCOMPARE(m1.numberColumns(), 2);

    m1.update(2,2, Model::Boolean(true));
    QCOMPARE(m1.numberRows(), 2);
    QCOMPARE(m1.numberColumns(), 2);

    m1.update(1,3, Model::Boolean(false));
    QCOMPARE(m1.numberRows(), 2);
    QCOMPARE(m1.numberColumns(), 3);

    m1.update(2,3, Model::Boolean(false));
    QCOMPARE(m1.numberRows(), 2);
    QCOMPARE(m1.numberColumns(), 3);

    m1.update(3,1, Model::Boolean(false));
    QCOMPARE(m1.numberRows(), 3);
    QCOMPARE(m1.numberColumns(), 3);

    m1.update(3,2, Model::Boolean(false));
    m1.update(3,3, Model::Boolean(true));
    QCOMPARE(m1.numberRows(), 3);
    QCOMPARE(m1.numberColumns(), 3);

    QCOMPARE(m1(1,1), Model::Boolean(true));
    QCOMPARE(m1(1,2), Model::Boolean(false));
    QCOMPARE(m1(1,3), Model::Boolean(false));
    QCOMPARE(m1(2,1), Model::Boolean(false));
    QCOMPARE(m1(2,2), Model::Boolean(true));
    QCOMPARE(m1(2,3), Model::Boolean(false));
    QCOMPARE(m1(3,1), Model::Boolean(false));
    QCOMPARE(m1(3,2), Model::Boolean(false));
    QCOMPARE(m1(3,3), Model::Boolean(true));

    Model::MatrixBoolean m2;
    m2.update(1, true);
    m2.update(2, false);
    m2.update(3, true);

    QCOMPARE(m2.numberRows(), 3);
    QCOMPARE(m2.numberColumns(), 1);

    QCOMPARE(m2(1,1), Model::Boolean(true));
    QCOMPARE(m2(2,1), Model::Boolean(false));
    QCOMPARE(m2(3,1), Model::Boolean(true));

    Model::MatrixBoolean m3;
    m3.update(1, 1, true);
    m3.update(1, 2, false);
    m3.update(3, false);
    m3.update(4, true);

    QCOMPARE(m3.numberRows(), 1);
    QCOMPARE(m3.numberColumns(), 4);

    QCOMPARE(m3(1,1), Model::Boolean(true));
    QCOMPARE(m3(1,2), Model::Boolean(false));
    QCOMPARE(m3(1,3), Model::Boolean(false));
    QCOMPARE(m3(1,4), Model::Boolean(true));
}


void TestMatrixBoolean::testSliceMethods() {
    Model::MatrixBoolean m = Model::MatrixBoolean::build(
        5, 5,
         true,  true,  true,  false, true,
         true,  true, false,   true, false,
         true, false, false,   true, true,
         true, false,  true,  false, false,
         true,  true,  true,  false, true
    );

    Model::MatrixBoolean m1 = m.at(Model::Range(Model::Integer(2),Model::Integer(4)), 2);

    QCOMPARE(m1.numberRows(), 3);
    QCOMPARE(m1.numberColumns(), 1);
    QCOMPARE(m1(1, 1), true);
    QCOMPARE(m1(2, 1), false);
    QCOMPARE(m1(3, 1), true);

    Model::MatrixBoolean m2 = m.at(3, Model::Range(Model::Integer(1),Model::Integer(3)));

    QCOMPARE(m2.numberRows(), 1);
    QCOMPARE(m2.numberColumns(), 3);
    QCOMPARE(m2(1, 1), true);
    QCOMPARE(m2(1, 2), false);
    QCOMPARE(m2(1, 3), false);

    Model::MatrixBoolean m3 = m.at(
        Model::Range(Model::Integer(1), Model::Integer(3), Model::Integer(5)),
        Model::Range(Model::Integer(2),Model::Integer(4))
    );

    QCOMPARE(m3.numberRows(), 3);
    QCOMPARE(m3.numberColumns(), 3);
    QCOMPARE(
        m3,
        Model::MatrixBoolean::build(
            3, 3,
            true, false, false,
            true, false, true,
            true, true,  false
        )
    );
}


void TestMatrixBoolean::testCombineMethods() {
    std::mt19937                    rng(0x12345678);
    std::uniform_int_distribution<> coefficientDistribution(-1E6, +1E6);
    std::uniform_int_distribution<> dimensionDistribution(1, 1000);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned long matrix1NumberRows         = dimensionDistribution(rng);
        unsigned long matrix1NumberColumns      = dimensionDistribution(rng);

        Model::MatrixBoolean m1(matrix1NumberRows, matrix1NumberColumns);
        for (unsigned long column=1 ; column<=matrix1NumberColumns ; ++column) {
            for (unsigned long row=1 ; row<=matrix1NumberRows ; ++row) {
                bool c = (coefficientDistribution(rng) > 0);
                m1.update(row, column, c);
            }
        }

        unsigned long matrix2NumberRows         = dimensionDistribution(rng);
        unsigned long matrix2NumberColumns      = dimensionDistribution(rng);

        Model::MatrixBoolean m2(matrix2NumberRows, matrix2NumberColumns);
        for (unsigned long column=1 ; column<=matrix2NumberColumns ; ++column) {
            for (unsigned long row=1 ; row<=matrix2NumberRows ; ++row) {
                bool c = (coefficientDistribution(rng) > 0);
                m2.update(row, column, c);
            }
        }

        Model::MatrixBoolean m = m1.combineLeftToRight(m2);
        QCOMPARE(static_cast<unsigned long>(m.numberRows()), std::max(matrix1NumberRows, matrix2NumberRows));
        QCOMPARE(static_cast<unsigned long>(m.numberColumns()), matrix1NumberColumns + matrix2NumberColumns);

        for (unsigned long column=1 ; column<=matrix1NumberColumns ; ++column) {
            for (unsigned long row=1 ; row<=matrix1NumberRows ; ++row) {
                Model::Boolean measured = m(row, column);
                Model::Boolean expected = m1(row, column);

                if (measured != expected) {
                    QCOMPARE(measured, expected);
                }
            }
        }

        if (matrix1NumberRows < matrix2NumberRows) {
            for (unsigned long column=1 ; column<=matrix1NumberColumns ; ++column) {
                for (unsigned long row=matrix1NumberRows+1 ; row<=matrix2NumberRows ; ++row) {
                    Model::Boolean measured = m(row, column);
                    if (measured != false) {
                        QCOMPARE(measured, false);
                    }
                }
            }
        }

        for (unsigned long column=1 ; column<=matrix2NumberColumns ; ++column) {
            for (unsigned long row=1 ; row<=matrix2NumberRows ; ++row) {
                Model::Boolean measured = m(row, column+matrix1NumberColumns);
                Model::Boolean expected = m2(row, column);

                if (measured != expected) {
                    QCOMPARE(measured, expected);
                }
            }
        }

        if (matrix2NumberRows < matrix1NumberRows) {
            unsigned long measuredNumberColumns = m.numberColumns();
            for (unsigned long column=matrix1NumberColumns + 1 ; column<=measuredNumberColumns ; ++column) {
                for (unsigned long row=matrix2NumberRows+1 ; row<=matrix1NumberRows ; ++row) {
                    Model::Boolean measured = m(row, column);
                    if (measured != false) {
                        QCOMPARE(measured, false);
                    }
                }
            }
        }

        m = m1.combineTopToBottom(m2);
        QCOMPARE(static_cast<unsigned long>(m.numberRows()), matrix1NumberRows + matrix2NumberRows);
        QCOMPARE(static_cast<unsigned long>(m.numberColumns()), std::max(matrix1NumberColumns, matrix2NumberColumns));

        for (unsigned long column=1 ; column<=matrix1NumberColumns ; ++column) {
            for (unsigned long row=1 ; row<=matrix1NumberRows ; ++row) {
                Model::Boolean measured = m(row, column);
                Model::Boolean expected = m1(row, column);

                if (measured != expected) {
                    QCOMPARE(measured, expected);
                }
            }
        }

        if (matrix1NumberColumns < matrix2NumberColumns) {
            for (unsigned long column=matrix1NumberColumns+1 ; column<=matrix2NumberColumns ; ++column) {
                for (unsigned long row=1 ; row<=matrix1NumberRows ; ++row) {
                    Model::Boolean measured = m(row, column);
                    if (measured != false) {
                        QCOMPARE(measured, false);
                    }
                }
            }
        }

        for (unsigned long column=1 ; column<=matrix2NumberColumns ; ++column) {
            for (unsigned long row=1 ; row<=matrix2NumberRows ; ++row) {
                Model::Boolean measured = m(row+matrix1NumberRows, column);
                Model::Boolean expected = m2(row, column);

                if (measured != expected) {
                    QCOMPARE(measured, expected);
                }
            }
        }

        if (matrix2NumberColumns < matrix1NumberColumns) {
            unsigned long measuredNumberRows = m.numberRows();
            for (unsigned long column=matrix2NumberColumns+1 ; column<=matrix1NumberColumns ; ++column) {
                for (unsigned long row=matrix1NumberRows+1 ; row<=measuredNumberRows ; ++row) {
                    Model::Boolean measured = m(row, column);
                    if (measured != false) {
                        QCOMPARE(measured, false);
                    }
                }
            }
        }
    }
}


void TestMatrixBoolean::testReverseMethods() {
    std::mt19937                    rng(0x12345678);
    std::uniform_int_distribution<> coefficientDistribution(-1E6, +1E6);
    std::uniform_int_distribution<> dimensionDistribution(1, 1000);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned long numberRows         = dimensionDistribution(rng);
        unsigned long numberColumns      = dimensionDistribution(rng);

        Model::MatrixBoolean m(numberRows, numberColumns);
        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                bool c = (coefficientDistribution(rng) > 0);
                m.update(row, column, c);
            }
        }

        Model::MatrixBoolean cr = m.columnReverse();

        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            unsigned long reverseColumn = numberColumns - column + 1;
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                Model::Boolean measured = cr(row, reverseColumn);
                Model::Boolean expected = m(row, column);

                if (measured != expected) {
                    QCOMPARE(measured, expected);
                }
            }
        }

        Model::MatrixBoolean rr = m.rowReverse();

        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                Model::Boolean measured = rr(numberRows - row + 1, column);
                Model::Boolean expected = m(row, column);

                if (measured != expected) {
                    QCOMPARE(measured, expected);
                }
            }
        }
    }
}


void TestMatrixBoolean::testIdentity() {
    Model::MatrixBoolean i1 = Model::MatrixBoolean::identity(10, 12);

    QCOMPARE(i1.numberRows(), 10);
    QCOMPARE(i1.numberColumns(), 12);

    for (unsigned row=1 ; row<=10 ; ++row) {
        for (unsigned column=1 ; column<=12 ; ++column) {
            Model::Boolean expected = (row == column) ? true : false;
            Model::Boolean measured = i1(row, column);

            QCOMPARE(expected, measured);
        }
    }

    Model::MatrixBoolean i2 = Model::MatrixBoolean::identity(10);

    QCOMPARE(i2.numberRows(), 10);
    QCOMPARE(i2.numberColumns(), 10);

    for (unsigned row=1 ; row<=10 ; ++row) {
        for (unsigned column=1 ; column<=10 ; ++column) {
            Model::Boolean expected = (row == column) ? true : false;
            Model::Boolean measured = i2(row, column);
            QCOMPARE(expected, measured);
        }
    }
}


void TestMatrixBoolean::testZero() {
    Model::MatrixBoolean i1 = Model::MatrixBoolean::zero(10, 12);

    QCOMPARE(i1.numberRows(), 10);
    QCOMPARE(i1.numberColumns(), 12);

    for (unsigned row=1 ; row<=10 ; ++row) {
        for (unsigned column=1 ; column<=12 ; ++column) {
            QCOMPARE(i1(row, column), false);
        }
    }

    Model::MatrixBoolean i2 = Model::MatrixBoolean::zero(10);

    QCOMPARE(i2.numberRows(), 10);
    QCOMPARE(i2.numberColumns(), 10);

    for (unsigned row=1 ; row<=10 ; ++row) {
        for (unsigned column=1 ; column<=10 ; ++column) {
            QCOMPARE(i2(row, column), false);
        }
    }
}


void TestMatrixBoolean::testOnes() {
    QList<unsigned> increment;
    unsigned        incrementPowerOf2  = 1;
    unsigned        transitionPowerOf2 = 16;
    unsigned        i                  = 0;

    while (i <= 1024) {
        unsigned nextTransitionStart = transitionPowerOf2 - incrementPowerOf2;
        if (i == nextTransitionStart) {
            increment.append(std::max(1U, incrementPowerOf2 - 1));
            ++i;
            while (i <= transitionPowerOf2) {
                increment.append(1);
                ++i;
            }

            transitionPowerOf2 <<= 1;
            incrementPowerOf2 <<= 1;

            increment.append(incrementPowerOf2 - 1);
            ++i;
        } else {
            increment.append(incrementPowerOf2);
            ++i;
        }
    }

    for (M::Integer numberRows=1 ; numberRows<=512 ; numberRows += increment.at(numberRows)) {
        for (M::Integer numberColumns=1 ; numberColumns<=512 ; numberColumns += increment.at(numberColumns)) {
            Model::MatrixBoolean i1 = Model::MatrixBoolean::ones(numberRows, numberColumns);

            QCOMPARE(i1.numberRows(), numberRows);
            QCOMPARE(i1.numberColumns(), numberColumns);

            for (M::Integer row=1 ; row<=numberRows ; ++row) {
                for (M::Integer column=1 ; column<=numberColumns ; ++column) {
                    QCOMPARE(i1(row, column), true);
                }
            }
        }

        Model::MatrixBoolean i2 = Model::MatrixBoolean::ones(numberRows);

        QCOMPARE(i2.numberRows(), numberRows);
        QCOMPARE(i2.numberColumns(), numberRows);

        for (unsigned row=1 ; row<=numberRows ; ++row) {
            for (unsigned column=1 ; column<=numberRows ; ++column) {
                QCOMPARE(i2(row, column), true);
            }
        }
    }
}


void TestMatrixBoolean::testDiagnonalEntries() {
    Model::MatrixBoolean m = Model::MatrixBoolean::build(
        4, 4,
        true,  false, true,  false,
        false, false, true,  true,
        true,  true,  true,  false,
        true,  false, false, false
    );

    Model::MatrixBoolean d = m.diagonalEntries();
    QCOMPARE(d.numberRows(), 4);
    QCOMPARE(d.numberColumns(), 1);

    QCOMPARE(d(1), true);
    QCOMPARE(d(2), false);
    QCOMPARE(d(3), true);
    QCOMPARE(d(4), false);
}


void TestMatrixBoolean::testDiagnonal() {
    Model::MatrixBoolean d = Model::MatrixBoolean::build(
        4, 1,
        true, false, true, true
    );

    Model::MatrixBoolean m = d.diagonal();

    QCOMPARE(m.numberRows(), 4);
    QCOMPARE(m.numberColumns(), 4);

    for (unsigned row=1 ; row<=4 ; ++row) {
        for (unsigned column=1 ; column<=4 ; ++column) {
            Model::Integer expected = (row == column) ? d(row) : false;
            Model::Integer measured = m(row, column);
            QCOMPARE(measured, expected);
        }
    }
}


void TestMatrixBoolean::testMatrixTypeMethods() {
    Model::MatrixBoolean rectangular = Model::MatrixBoolean::build(
        3, 2,
        true,  false, true,
        false, true,  false
    );

    Model::MatrixBoolean square = Model::MatrixBoolean::build(
        3, 3,
        true,  false, true,
        false, true,  false,
        true,  false, false
    );

    QCOMPARE(rectangular.isSquare(), false);
    QCOMPARE(square.isSquare(), true);
}


void TestMatrixBoolean::testTranspose() {
    std::mt19937                    rng(0x12345678);
    std::uniform_int_distribution<> coefficientDistribution(-1E6, +1E6);
    std::uniform_int_distribution<> dimensionDistribution(1, 1000);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned long numberRows         = dimensionDistribution(rng);
        unsigned long numberColumns      = dimensionDistribution(rng);

        Model::MatrixBoolean m(numberRows, numberColumns);
        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                bool c = (coefficientDistribution(rng) > 0);
                m.update(row, column, c);
            }
        }

        Model::MatrixBoolean t = m.transpose();
        QCOMPARE(static_cast<unsigned long>(t.numberRows()), numberColumns);
        QCOMPARE(static_cast<unsigned long>(t.numberColumns()), numberRows);

        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                Model::Boolean measured = t(column, row);
                Model::Boolean expected = m(row, column);

                if (measured != expected) {
                    QCOMPARE(measured, expected);
                }
            }
        }
    }
}


void TestMatrixBoolean::testComplexConjugate() {
    std::mt19937                    rng(0x12345678);
    std::uniform_int_distribution<> coefficientDistribution(-1E6, +1E6);
    std::uniform_int_distribution<> dimensionDistribution(1, 1000);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned long numberRows         = dimensionDistribution(rng);
        unsigned long numberColumns      = dimensionDistribution(rng);

        Model::MatrixBoolean m(numberRows, numberColumns);
        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                bool c = (coefficientDistribution(rng) > 0);
                m.update(row, column, c);
            }
        }

        Model::MatrixBoolean c = m.conj();
        QCOMPARE(static_cast<unsigned long>(c.numberRows()), numberRows);
        QCOMPARE(static_cast<unsigned long>(c.numberColumns()), numberColumns);

        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                Model::Boolean measured = c(row, column);
                Model::Boolean expected = m(row, column);

                if (measured != expected) {
                    QCOMPARE(measured, expected);
                }
            }
        }
    }
}


void TestMatrixBoolean::testAdjoint() {
    std::mt19937                    rng(0x12345678);
    std::uniform_int_distribution<> coefficientDistribution(-1E6, +1E6);
    std::uniform_int_distribution<> dimensionDistribution(1, 1000);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned long numberRows         = dimensionDistribution(rng);
        unsigned long numberColumns      = dimensionDistribution(rng);

        Model::MatrixBoolean m(numberRows, numberColumns);
        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                bool c = (coefficientDistribution(rng) > 0);
                m.update(row, column, c);
            }
        }

        Model::MatrixBoolean t = m.adjoint();
        QCOMPARE(static_cast<unsigned long>(t.numberRows()), numberColumns);
        QCOMPARE(static_cast<unsigned long>(t.numberColumns()), numberRows);

        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                Model::Boolean measured = t(column, row);
                Model::Boolean expected = m(row, column);

                if (measured != expected) {
                    QCOMPARE(measured, expected);
                }
            }
        }
    }
}


void TestMatrixBoolean::testComparisonOperators() {
    Model::MatrixBoolean m1 = Model::MatrixBoolean::build(
        3, 3,
        true,  false, false,
        false, true,  false,
        false, true,  true
    );

    Model::MatrixBoolean m2 = Model::MatrixBoolean::build(
        2, 3,
        true,  false,
        false, true,
        false, true
    );

    Model::MatrixBoolean m3 = Model::MatrixBoolean::build(
        3, 2,
        true,  false, false,
        false, true,  false
    );

    Model::MatrixBoolean m4 = Model::MatrixBoolean::build( // same as m1
        3, 3,
        true,  false, false,
        false, true,  false,
        false, true,  true
    );

    Model::MatrixBoolean m5 = Model::MatrixBoolean::build( // same as m1
        3, 3,
        true,  false, false,
        false, true,  false,
        false, false, true
    );

    QCOMPARE(m1 == m2, false);
    QCOMPARE(m1 == m3, false);
    QCOMPARE(m1 == m1, true);
    QCOMPARE(m1 == m4, true);
    QCOMPARE(m1 == m5, false);

    QCOMPARE(m1 != m2, true);
    QCOMPARE(m1 != m3, true);
    QCOMPARE(m1 != m1, false);
    QCOMPARE(m1 != m4, false);
    QCOMPARE(m1 != m5, true);

    Model::MatrixInteger m6 = Model::MatrixInteger::build(
        3, 3,
        1, 0, 0,
        0, 2, 0,
        0, 3, 4
    );

    Model::MatrixInteger m7 = Model::MatrixInteger::build(
        3, 2,
        1, 0,
        0, 2,
        0, 3
    );

    Model::MatrixInteger m8 = Model::MatrixInteger::build(
        3, 3,
        1, 0, 0,
        0, 2, 0,
        0, 3, 5
    );

    Model::MatrixInteger m9 = Model::MatrixInteger::build(
        3, 3,
        1, 0, 0,
        0, 2, 0,
        0, 0, 4
    );

    QCOMPARE(m1 == m6, true);
    QCOMPARE(m1 == m7, false);
    QCOMPARE(m1 == m8, true);
    QCOMPARE(m1 == m9, false);

    Model::MatrixReal m10 = Model::MatrixReal::build(
        3, 3,
        Model::Real(1), Model::Real(0), Model::Real(0),
        Model::Real(0), Model::Real(2), Model::Real(0),
        Model::Real(0), Model::Real(3), Model::Real(4)
    );

    Model::MatrixReal m11 = Model::MatrixReal::build(
        3, 2,
        Model::Real(1), Model::Real(0),
        Model::Real(0), Model::Real(2),
        Model::Real(0), Model::Real(3)
    );

    Model::MatrixReal m12 = Model::MatrixReal::build(
        3, 3,
        Model::Real(1), Model::Real(0), Model::Real(0),
        Model::Real(0), Model::Real(2), Model::Real(0),
        Model::Real(0), Model::Real(3), Model::Real(5)
    );

    Model::MatrixReal m13 = Model::MatrixReal::build(
        3, 3,
        Model::Real(1), Model::Real(0), Model::Real(0),
        Model::Real(0), Model::Real(2), Model::Real(0),
        Model::Real(0), Model::Real(0), Model::Real(4)
    );

    QCOMPARE(m1 == m10, true);
    QCOMPARE(m1 == m11, false);
    QCOMPARE(m1 == m12, true);
    QCOMPARE(m1 == m13, false);

    Model::MatrixComplex m14 = Model::MatrixComplex::build(
        3, 3,
        Model::Complex(1), Model::Complex(0), Model::Complex(0),
        Model::Complex(0), Model::Complex(2), Model::Complex(0),
        Model::Complex(0), Model::Complex(3), Model::Complex(4)
    );

    Model::MatrixComplex m15 = Model::MatrixComplex::build(
        3, 2,
        Model::Complex(1), Model::Complex(0),
        Model::Complex(0), Model::Complex(2),
        Model::Complex(0), Model::Complex(3)
    );

    Model::MatrixComplex m16 = Model::MatrixComplex::build(
        3, 3,
        Model::Complex(1), Model::Complex(0), Model::Complex(0),
        Model::Complex(0), Model::Complex(2), Model::Complex(0),
        Model::Complex(0), Model::Complex(3), Model::Complex(5)
    );

    Model::MatrixComplex m17 = Model::MatrixComplex::build(
        3, 3,
        Model::Complex(1), Model::Complex(0), Model::Complex(0),
        Model::Complex(0), Model::Complex(2), Model::Complex(0),
        Model::Complex(0), Model::Complex(0), Model::Complex(4)
    );

    QCOMPARE(m1 == m14, true);
    QCOMPARE(m1 == m15, false);
    QCOMPARE(m1 == m16, true);
    QCOMPARE(m1 == m17, false);
}


void TestMatrixBoolean::testSubscriptingOperators() {
//    // Tested by other methods.
}


void TestMatrixBoolean::testIterator() {
    M::MatrixBoolean m = M::MatrixBoolean::build(
        3, 3,
        true,  false, false,
        false, true,  false,
        true,  false, true
    );

    M::MatrixBoolean::Iterator it = m.begin();
    QCOMPARE(it != m.end(), true);
    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);

    QCOMPARE(it.value(), true);
    QCOMPARE(it.constReference(), true);
    QCOMPARE(*(it.constPointer()), true);
    QCOMPARE(*it, true);

    ++it;
    QCOMPARE(it != m.end(), true);
    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(*it, false);

    M::MatrixBoolean::Iterator it2 = it++;
    QCOMPARE(*it2, false);

    QCOMPARE(it != m.end(), true);
    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(*it, true);

    it.advance(2);

    QCOMPARE(it != m.end(), true);
    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(*it, true);

    it.advance(2);

    QCOMPARE(it != m.end(), true);
    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(*it, false);

    it.advance(2);

    QCOMPARE(it != m.end(), true);
    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(*it, true);

    ++it;

    QCOMPARE(it == m.end(), true);
    QCOMPARE(it.isValid(), false);
    QCOMPARE(it.isInvalid(), true);
}


bool TestMatrixBoolean::boolify(unsigned value) {
    std::uint32_t x = value;

    x -= ((x >> 1) & 0x55555555);
    x = (((x >> 2) & 0x33333333) + (x & 0x33333333));
    x = (((x >> 4) + x) & 0x0F0F0F0F);
    x += (x >> 8);
    x += (x >> 16);

    return bool(x & 0x0000001);
}


void TestMatrixBoolean::dumpMatrix(const Model::MatrixBoolean& matrix) {
    unsigned long numberRows = matrix.numberRows();
    unsigned long numberColumns = matrix.numberColumns();

    for (unsigned long row=1 ; row<=numberRows ; ++row) {
        QString t;
        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            Model::Boolean v = matrix(row, column);
            t += v == true ? QString("T ") : QString("F ");
        }

        qDebug() << t;
    }
}
