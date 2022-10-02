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
* This file implements tests of the \ref Model::MatrixInteger class.
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

#include <model_exceptions.h>
#include <model_tuple.h>
#include <m_basic_functions.h>
#include <model_matrix_boolean.h>
#include <model_matrix_integer.h>
#include <m_matrix_integer.h>
#include <m_variant.h>
#include <m_range.h>
#include <m_set.h>
#include <m_tuple.h>
#include <m_matrix_real.h>
#include <m_matrix_complex.h>

#include "test_matrix_base.h"
#include "test_matrix_integer.h"

TestMatrixInteger::TestMatrixInteger() {}


TestMatrixInteger::~TestMatrixInteger() {}


void TestMatrixInteger::testConstructorAndDestructors() {
    Model::MatrixInteger m1;
    QCOMPARE(m1.numberRows(), 0);
    QCOMPARE(m1.numberColumns(), 0);

    Model::MatrixInteger m2(3, 3);
    QCOMPARE(m2.numberRows(), 3);
    QCOMPARE(m2.numberColumns(), 3);

    QCOMPARE(m2(1,1), Model::Integer(0));
    QCOMPARE(m2(1,2), Model::Integer(0));
    QCOMPARE(m2(1,3), Model::Integer(0));
    QCOMPARE(m2(2,1), Model::Integer(0));
    QCOMPARE(m2(2,2), Model::Integer(0));
    QCOMPARE(m2(2,3), Model::Integer(0));
    QCOMPARE(m2(3,1), Model::Integer(0));
    QCOMPARE(m2(3,2), Model::Integer(0));
    QCOMPARE(m2(3,3), Model::Integer(0));

    m2.update(1, 1, Model::Integer(1));
    m2.update(1, 2, Model::Integer(2));
    m2.update(1, 3, Model::Integer(3));
    m2.update(2, 1, Model::Integer(4));
    m2.update(2, 2, Model::Integer(5));
    m2.update(2, 3, Model::Integer(6));
    m2.update(3, 1, Model::Integer(7));
    m2.update(3, 2, Model::Integer(8));
    m2.update(3, 3, Model::Integer(9));

    const Model::Integer matrixData[] = {
        Model::Integer(1), Model::Integer(2),
        Model::Integer(3), Model::Integer(4)
    };

    Model::MatrixInteger m3(2, 2, matrixData);
    QCOMPARE(m3.numberRows(), 2);
    QCOMPARE(m3.numberColumns(), 2);

    QCOMPARE(m3(1,1), Model::Integer(1));
    QCOMPARE(m3(1,2), Model::Integer(3));
    QCOMPARE(m3(2,1), Model::Integer(2));
    QCOMPARE(m3(2,2), Model::Integer(4));

    Model::MatrixInteger m4(m2);
    QCOMPARE(m4.numberRows(), 3);
    QCOMPARE(m4.numberColumns(), 3);

    QCOMPARE(m4(1,1), Model::Integer(1));
    QCOMPARE(m4(1,2), Model::Integer(2));
    QCOMPARE(m4(1,3), Model::Integer(3));
    QCOMPARE(m4(2,1), Model::Integer(4));
    QCOMPARE(m4(2,2), Model::Integer(5));
    QCOMPARE(m4(2,3), Model::Integer(6));
    QCOMPARE(m4(3,1), Model::Integer(7));
    QCOMPARE(m4(3,2), Model::Integer(8));
    QCOMPARE(m4(3,3), Model::Integer(9));

    Model::MatrixBoolean b = Model::MatrixBoolean::build(
        2, 2,
        true, false,
        true, true
    );

    Model::MatrixInteger ib(b.transpose());
    QCOMPARE(
        ib,
        Model::MatrixInteger::build(
            2, 2,
            Model::Integer(1), Model::Integer(1),
            Model::Integer(0), Model::Integer(1)
        )
    );
}


void TestMatrixInteger::testCoefficientValueType() {
    Model::MatrixInteger m;
    QCOMPARE(m.coefficientValueType(), Model::ValueType::INTEGER);
}


void TestMatrixInteger::testNumberRowsColumns() {
    Model::MatrixInteger m1(2, 3);

    QCOMPARE(m1.numberRows(), 2);
    QCOMPARE(m1.numberColumns(), 3);
    QCOMPARE(m1.numberCoefficients(), 6);
}


void TestMatrixInteger::testValueMethods() {
    Model::MatrixInteger m1;

    bool success = m1.setValue(1, 1, Model::Integer(1));
    QVERIFY(success);

    Model::Variant v11 = m1.value(1, 1);
    QCOMPARE(v11.valueType(), Model::ValueType::INTEGER);
    QCOMPARE(v11, Model::Variant(Model::Integer(1)));

    success = m1.setValue(1, 2, Model::Integer(1.5));
    QVERIFY(success);

    v11 = m1.value(1, 1);
    Model::Variant v12 = m1.value(1, 2);

    QCOMPARE(v11.valueType(), Model::ValueType::INTEGER);
    QCOMPARE(v12.valueType(), Model::ValueType::INTEGER);

    QCOMPARE(v11, Model::Variant(Model::Integer(1)));
    QCOMPARE(v12, Model::Variant(Model::Integer(1.5)));

    success = m1.setValue(2, 1, Model::Integer(1.75));
    QVERIFY(success);

    v11 = m1.value(1, 1);
    v12 = m1.value(1, 2);
    Model::Variant v21 = m1.value(2, 1);

    QCOMPARE(v11.valueType(), Model::ValueType::INTEGER);
    QCOMPARE(v12.valueType(), Model::ValueType::INTEGER);
    QCOMPARE(v21.valueType(), Model::ValueType::INTEGER);

    QCOMPARE(v11, Model::Variant(Model::Integer(1)));
    QCOMPARE(v12, Model::Variant(Model::Integer(1.5)));
    QCOMPARE(v21, Model::Variant(Model::Integer(1.75)));

    success = m1.setValue(4, Model::Boolean(true));
    QVERIFY(!success);

    success = m1.setValue(4, Model::Integer(2.75));
    QVERIFY(success);

    QCOMPARE(m1.numberRows(), 2);
    QCOMPARE(m1.numberColumns(), 2);

    v11 = m1.value(1, 1);
    v12 = m1.value(1, 2);
    v21 = m1.value(2, 1);
    Model::Variant v22 = m1.value(2, 2);

    QCOMPARE(v11.valueType(), Model::ValueType::INTEGER);
    QCOMPARE(v12.valueType(), Model::ValueType::INTEGER);
    QCOMPARE(v21.valueType(), Model::ValueType::INTEGER);
    QCOMPARE(v22.valueType(), Model::ValueType::INTEGER);

    QCOMPARE(v11, Model::Variant(Model::Integer(1)));
    QCOMPARE(v12, Model::Variant(Model::Integer(1.5)));
    QCOMPARE(v21, Model::Variant(Model::Integer(1.75)));
    QCOMPARE(v22, Model::Variant(Model::Integer(2.75)));

    QCOMPARE(m1.value(1), Model::Variant(Model::Integer(1)));
    QCOMPARE(m1.value(2), Model::Variant(Model::Integer(1.5)));
    QCOMPARE(m1.value(3), Model::Variant(Model::Integer(1.75)));
    QCOMPARE(m1.value(4), Model::Variant(Model::Integer(2.75)));
}


void TestMatrixInteger::testInternalAtMethods() {
    M::MatrixInteger m(9, 9);
    for (M::Integer row=1 ; row<=9 ; ++row) {
        for (M::Integer col=1 ; col<=9 ; ++col) {
            m.update(row, col, 10.0 * row + col);
        }
    }

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

    QCOMPARE(m.at(i, i), M::Real(33));
    QCOMPARE(m.at(i, r), M::Real(34));
    QCOMPARE(m.at(i, c), M::Real(35));
    QCOMPARE(m.at(i, rng), M::MatrixInteger::build(1, 2, M::Integer(35), M::Integer(36)));
    QCOMPARE(m.at(i, set), M::MatrixInteger::build(1, 2, M::Integer(32), M::Integer(34)));
    QCOMPARE(m.at(i, tuple), M::MatrixInteger::build(1, 2, M::Integer(33), M::Integer(35)));
    QCOMPARE(
        m.at(i, mi),
        M::MatrixInteger::build(1, 4, M::Integer(32), M::Integer(34), M::Integer(36), M::Integer(38))
    );
    QCOMPARE(
        m.at(i, mr),
        M::MatrixInteger::build(1, 4, M::Integer(31), M::Integer(32), M::Integer(33), M::Integer(34))
    );
    QCOMPARE(m.at(i, mc), M::MatrixInteger::build(1, 2, M::Integer(31), M::Integer(35)));

    QCOMPARE(m.at(r, i), M::Real(43));
    QCOMPARE(m.at(r, r), M::Real(44));
    QCOMPARE(m.at(r, c), M::Real(45));
    QCOMPARE(m.at(r, rng), M::MatrixInteger::build(1, 2, M::Integer(45), M::Integer(46)));
    QCOMPARE(m.at(r, set), M::MatrixInteger::build(1, 2, M::Integer(42), M::Integer(44)));
    QCOMPARE(m.at(r, tuple), M::MatrixInteger::build(1, 2, M::Integer(43), M::Integer(45)));
    QCOMPARE(
        m.at(r, mi),
        M::MatrixInteger::build(1, 4, M::Integer(42), M::Integer(44), M::Integer(46), M::Integer(48))
    );
    QCOMPARE(
        m.at(r, mr),
        M::MatrixInteger::build(1, 4, M::Integer(41), M::Integer(42), M::Integer(43), M::Integer(44))
    );
    QCOMPARE(m.at(r, mc), M::MatrixInteger::build(1, 2, M::Integer(41), M::Integer(45)));

    QCOMPARE(m.at(c, i), M::Real(53));
    QCOMPARE(m.at(c, r), M::Real(54));
    QCOMPARE(m.at(c, c), M::Real(55));
    QCOMPARE(m.at(c, rng), M::MatrixInteger::build(1, 2, M::Integer(55), M::Integer(56)));
    QCOMPARE(m.at(c, set), M::MatrixInteger::build(1, 2, M::Integer(52), M::Integer(54)));
    QCOMPARE(m.at(c, tuple), M::MatrixInteger::build(1, 2, M::Integer(53), M::Integer(55)));
    QCOMPARE(
        m.at(c, mi),
        M::MatrixInteger::build(1, 4, M::Integer(52), M::Integer(54), M::Integer(56), M::Integer(58))
    );
    QCOMPARE(
        m.at(c, mr),
        M::MatrixInteger::build(1, 4, M::Integer(51), M::Integer(52), M::Integer(53), M::Integer(54))
    );
    QCOMPARE(m.at(c, mc), M::MatrixInteger::build(1, 2, M::Integer(51), M::Integer(55)));

    QCOMPARE(m.at(rng, i), M::MatrixInteger::build(2, 1, M::Real(53), M::Real(63)));
    QCOMPARE(m.at(rng, r), M::MatrixInteger::build(2, 1, M::Real(54), M::Real(64)));
    QCOMPARE(m.at(rng, c), M::MatrixInteger::build(2, 1, M::Real(55), M::Real(65)));
    QCOMPARE(
        m.at(rng, rng),
        M::MatrixInteger::build(
            2, 2,
            M::Real(55), M::Real(65),
            M::Real(56), M::Real(66)
        )
    );
    QCOMPARE(
        m.at(rng, set),
        M::MatrixInteger::build(
            2, 2,
            M::Real(52), M::Real(62),
            M::Real(54), M::Real(64)
        )
    );
    QCOMPARE(
        m.at(rng, tuple),
        M::MatrixInteger::build(
            2, 2,
            M::Real(53), M::Real(63),
            M::Real(55), M::Real(65)
        )
    );
    QCOMPARE(
        m.at(rng, mi),
        M::MatrixInteger::build(
            2, 4,
            M::Real(52), M::Real(62),
            M::Real(54), M::Real(64),
            M::Real(56), M::Real(66),
            M::Real(58), M::Real(68)
        )
    );
    QCOMPARE(
        m.at(rng, mr),
        M::MatrixInteger::build(
            2, 4,
            M::Real(51), M::Real(61),
            M::Real(52), M::Real(62),
            M::Real(53), M::Real(63),
            M::Real(54), M::Real(64)
        )
    );
    QCOMPARE(
        m.at(rng, mc),
        M::MatrixInteger::build(
            2, 2,
            M::Real(51), M::Real(61),
            M::Real(55), M::Real(65)
        )
    );

    QCOMPARE(m.at(set, i), M::MatrixInteger::build(2, 1, M::Real(23), M::Real(43)));
    QCOMPARE(m.at(set, r), M::MatrixInteger::build(2, 1, M::Real(24), M::Real(44)));
    QCOMPARE(m.at(set, c), M::MatrixInteger::build(2, 1, M::Real(25), M::Real(45)));
    QCOMPARE(
        m.at(set, rng),
        M::MatrixInteger::build(
            2, 2,
            M::Real(25), M::Real(45),
            M::Real(26), M::Real(46)
        )
    );
    QCOMPARE(
        m.at(set, set),
        M::MatrixInteger::build(
            2, 2,
            M::Real(22), M::Real(42),
            M::Real(24), M::Real(44)
        )
    );
    QCOMPARE(
        m.at(set, tuple),
        M::MatrixInteger::build(
            2, 2,
            M::Real(23), M::Real(43),
            M::Real(25), M::Real(45)
        )
    );
    QCOMPARE(
        m.at(set, mi),
        M::MatrixInteger::build(
            2, 4,
            M::Real(22), M::Real(42),
            M::Real(24), M::Real(44),
            M::Real(26), M::Real(46),
            M::Real(28), M::Real(48)
        )
    );
    QCOMPARE(
        m.at(set, mr),
        M::MatrixInteger::build(
            2, 4,
            M::Real(21), M::Real(41),
            M::Real(22), M::Real(42),
            M::Real(23), M::Real(43),
            M::Real(24), M::Real(44)
        )
    );
    QCOMPARE(
        m.at(set, mc),
        M::MatrixInteger::build(
            2, 2,
            M::Real(21), M::Real(41),
            M::Real(25), M::Real(45)
        )
    );

    QCOMPARE(m.at(tuple, i), M::MatrixInteger::build(2, 1, M::Real(33), M::Real(53)));
    QCOMPARE(m.at(tuple, r), M::MatrixInteger::build(2, 1, M::Real(34), M::Real(54)));
    QCOMPARE(m.at(tuple, c), M::MatrixInteger::build(2, 1, M::Real(35), M::Real(55)));
    QCOMPARE(
        m.at(tuple, rng),
        M::MatrixInteger::build(
            2, 2,
            M::Integer(35), M::Integer(55),
            M::Integer(36), M::Integer(56)
        )
    );
    QCOMPARE(
        m.at(tuple, set),
        M::MatrixInteger::build(
            2, 2,
            M::Integer(32), M::Integer(52),
            M::Integer(34), M::Integer(54)
        )
    );
    QCOMPARE(
        m.at(tuple, tuple),
        M::MatrixInteger::build(
            2, 2,
            M::Integer(33), M::Integer(53),
            M::Integer(35), M::Integer(55)
        )
    );
    QCOMPARE(
        m.at(tuple, mi),
        M::MatrixInteger::build(
            2, 4,
            M::Integer(32), M::Integer(52),
            M::Integer(34), M::Integer(54),
            M::Integer(36), M::Integer(56),
            M::Integer(38), M::Integer(58)
        )
    );
    QCOMPARE(
        m.at(tuple, mr),
        M::MatrixInteger::build(
            2, 4,
            M::Integer(31), M::Integer(51),
            M::Integer(32), M::Integer(52),
            M::Integer(33), M::Integer(53),
            M::Integer(34), M::Integer(54)
        )
    );
    QCOMPARE(
        m.at(tuple, mc),
        M::MatrixInteger::build(
            2, 2,
            M::Integer(31), M::Integer(51),
            M::Integer(35), M::Integer(55)
        )
    );

    QCOMPARE(
        m.at(mi, i),
        M::MatrixInteger::build(4, 1, M::Integer(23), M::Integer(43), M::Integer(63), M::Integer(83))
    );
    QCOMPARE(
        m.at(mi, r),
        M::MatrixInteger::build(4, 1, M::Integer(24), M::Integer(44), M::Integer(64), M::Integer(84))
    );
    QCOMPARE(
        m.at(mi, c),
        M::MatrixInteger::build(4, 1, M::Integer(25), M::Integer(45), M::Integer(65), M::Integer(85))
    );
    QCOMPARE(
        m.at(mi, rng),
        M::MatrixInteger::build(
            4, 2,
            M::Integer(25), M::Integer(45), M::Integer(65), M::Integer(85),
            M::Integer(26), M::Integer(46), M::Integer(66), M::Integer(86)
        )
    );
    QCOMPARE(
        m.at(mi, set),
        M::MatrixInteger::build(
            4, 2,
            M::Integer(22), M::Integer(42), M::Integer(62), M::Integer(82),
            M::Integer(24), M::Integer(44), M::Integer(64), M::Integer(84)
        )
    );
    QCOMPARE(
        m.at(mi, tuple),
        M::MatrixInteger::build(
            4, 2,
            M::Integer(23), M::Integer(43), M::Integer(63), M::Integer(83),
            M::Integer(25), M::Integer(45), M::Integer(65), M::Integer(85)
        )
    );
    QCOMPARE(
        m.at(mi, mi),
        M::MatrixInteger::build(
            4, 4,
            M::Integer(22), M::Integer(42), M::Integer(62), M::Integer(82),
            M::Integer(24), M::Integer(44), M::Integer(64), M::Integer(84),
            M::Integer(26), M::Integer(46), M::Integer(66), M::Integer(86),
            M::Integer(28), M::Integer(48), M::Integer(68), M::Integer(88)
        )
    );
    QCOMPARE(
        m.at(mi, mr),
        M::MatrixInteger::build(
            4, 4,
            M::Integer(21), M::Integer(41), M::Integer(61), M::Integer(81),
            M::Integer(22), M::Integer(42), M::Integer(62), M::Integer(82),
            M::Integer(23), M::Integer(43), M::Integer(63), M::Integer(83),
            M::Integer(24), M::Integer(44), M::Integer(64), M::Integer(84)
        )
    );
    QCOMPARE(
        m.at(mi, mc),
        M::MatrixInteger::build(
            4, 2,
            M::Integer(21), M::Integer(41), M::Integer(61), M::Integer(81),
            M::Integer(25), M::Integer(45), M::Integer(65), M::Integer(85)
        )
    );

    QCOMPARE(
        m.at(mr, i),
        M::MatrixInteger::build(4, 1, M::Integer(13), M::Integer(23), M::Integer(33), M::Integer(43))
    );
    QCOMPARE(
        m.at(mr, r),
        M::MatrixInteger::build(4, 1, M::Integer(14), M::Integer(24), M::Integer(34), M::Integer(44))
    );
    QCOMPARE(
        m.at(mr, c),
        M::MatrixInteger::build(4, 1, M::Integer(15), M::Integer(25), M::Integer(35), M::Integer(45))
    );
    QCOMPARE(
        m.at(mr, rng),
        M::MatrixInteger::build(
            4, 2,
            M::Integer(15), M::Integer(25), M::Integer(35), M::Integer(45),
            M::Integer(16), M::Integer(26), M::Integer(36), M::Integer(46)
        )
    );
    QCOMPARE(
        m.at(mr, set),
        M::MatrixInteger::build(
            4, 2,
            M::Integer(12), M::Integer(22), M::Integer(32), M::Integer(42),
            M::Integer(14), M::Integer(24), M::Integer(34), M::Integer(44)
        )
    );
    QCOMPARE(
        m.at(mr, tuple),
        M::MatrixInteger::build(
            4, 2,
            M::Integer(13), M::Integer(23), M::Integer(33), M::Integer(43),
            M::Integer(15), M::Integer(25), M::Integer(35), M::Integer(45)
        )
    );
    QCOMPARE(
        m.at(mr, mi),
        M::MatrixInteger::build(
            4, 4,
            M::Integer(12), M::Integer(22), M::Integer(32), M::Integer(42),
            M::Integer(14), M::Integer(24), M::Integer(34), M::Integer(44),
            M::Integer(16), M::Integer(26), M::Integer(36), M::Integer(46),
            M::Integer(18), M::Integer(28), M::Integer(38), M::Integer(48)
        )
    );
    QCOMPARE(
        m.at(mr, mr),
        M::MatrixInteger::build(
            4, 4,
            M::Integer(11), M::Integer(21), M::Integer(31), M::Integer(41),
            M::Integer(12), M::Integer(22), M::Integer(32), M::Integer(42),
            M::Integer(13), M::Integer(23), M::Integer(33), M::Integer(43),
            M::Integer(14), M::Integer(24), M::Integer(34), M::Integer(44)
        )
    );
    QCOMPARE(
        m.at(mr, mc),
        M::MatrixInteger::build(
            4, 2,
            M::Integer(11), M::Integer(21), M::Integer(31), M::Integer(41),
            M::Integer(15), M::Integer(25), M::Integer(35), M::Integer(45)
        )
    );

    QCOMPARE(m.at(mc, i), M::MatrixInteger::build(2, 1, M::Integer(13), M::Integer(53)));
    QCOMPARE(m.at(mc, r), M::MatrixInteger::build(2, 1, M::Integer(14), M::Integer(54)));
    QCOMPARE(m.at(mc, c), M::MatrixInteger::build(2, 1, M::Integer(15), M::Integer(55)));
    QCOMPARE(
        m.at(mc, rng),
        M::MatrixInteger::build(
            2, 2,
            M::Integer(15), M::Integer(55),
            M::Integer(16), M::Integer(56)
        )
    );
    QCOMPARE(
        m.at(mc, set),
        M::MatrixInteger::build(
            2, 2,
            M::Integer(12), M::Integer(52),
            M::Integer(14), M::Integer(54)
        )
    );
    QCOMPARE(
        m.at(mc, tuple),
        M::MatrixInteger::build(
            2, 2,
            M::Integer(13), M::Integer(53),
            M::Integer(15), M::Integer(55)
        )
    );
    QCOMPARE(
        m.at(mc, mi),
        M::MatrixInteger::build(
            2, 4,
            M::Integer(12), M::Integer(52),
            M::Integer(14), M::Integer(54),
            M::Integer(16), M::Integer(56),
            M::Integer(18), M::Integer(58)
        )
    );
    QCOMPARE(
        m.at(mc, mr),
        M::MatrixInteger::build(
            2, 4,
            M::Integer(11), M::Integer(51),
            M::Integer(12), M::Integer(52),
            M::Integer(13), M::Integer(53),
            M::Integer(14), M::Integer(54)
        )
    );
    QCOMPARE(
        m.at(mc, mc),
        M::MatrixInteger::build(
            2, 2,
            M::Integer(11), M::Integer(51),
            M::Integer(15), M::Integer(55)
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


void TestMatrixInteger::testResizeMethod() {
    Model::MatrixInteger m(2, 2);
    QCOMPARE(m.numberRows(),    2);
    QCOMPARE(m.numberColumns(), 2);

    m.update(1,1, Model::Integer(1));
    m.update(1,2, Model::Integer(2));
    m.update(2,1, Model::Integer(3));
    m.update(2,2, Model::Integer(4));

    QCOMPARE(m(1,1), Model::Integer(1));
    QCOMPARE(m(1,2), Model::Integer(2));
    QCOMPARE(m(2,1), Model::Integer(3));
    QCOMPARE(m(2,2), Model::Integer(4));

    m.resize(3, 4);
    QCOMPARE(m.numberRows(),    3);
    QCOMPARE(m.numberColumns(), 4);

    QCOMPARE(m(1,1), Model::Integer(1));
    QCOMPARE(m(1,2), Model::Integer(2));
    QCOMPARE(m(1,3), Model::Integer(0));
    QCOMPARE(m(1,4), Model::Integer(0));
    QCOMPARE(m(2,1), Model::Integer(3));
    QCOMPARE(m(2,2), Model::Integer(4));
    QCOMPARE(m(3,3), Model::Integer(0));
    QCOMPARE(m(3,4), Model::Integer(0));
    QCOMPARE(m(3,1), Model::Integer(0));
    QCOMPARE(m(3,2), Model::Integer(0));
    QCOMPARE(m(3,3), Model::Integer(0));
    QCOMPARE(m(3,4), Model::Integer(0));
}


void TestMatrixInteger::testCoefficientAccessorMethods() {
    Model::MatrixInteger m1;
    QCOMPARE(m1.numberRows(), 0);
    QCOMPARE(m1.numberColumns(), 0);

    m1.update(1,1, Model::Integer(1));
    QCOMPARE(m1.numberRows(), 1);
    QCOMPARE(m1.numberColumns(), 1);

    m1.update(1,2, Model::Integer(2));
    QCOMPARE(m1.numberRows(), 1);
    QCOMPARE(m1.numberColumns(), 2);

    m1.update(2,1, Model::Integer(4));
    QCOMPARE(m1.numberRows(), 2);
    QCOMPARE(m1.numberColumns(), 2);

    m1.update(2,2, Model::Integer(5));
    QCOMPARE(m1.numberRows(), 2);
    QCOMPARE(m1.numberColumns(), 2);

    m1.update(1,3, Model::Integer(3));
    QCOMPARE(m1.numberRows(), 2);
    QCOMPARE(m1.numberColumns(), 3);

    m1.update(2,3, Model::Integer(6));
    QCOMPARE(m1.numberRows(), 2);
    QCOMPARE(m1.numberColumns(), 3);

    m1.update(3,1, Model::Integer(7));
    QCOMPARE(m1.numberRows(), 3);
    QCOMPARE(m1.numberColumns(), 3);

    m1.update(3,2, Model::Integer(8));
    m1.update(3,3, Model::Integer(9));
    QCOMPARE(m1.numberRows(), 3);
    QCOMPARE(m1.numberColumns(), 3);

    QCOMPARE(m1(1,1), Model::Integer(1));
    QCOMPARE(m1(1,2), Model::Integer(2));
    QCOMPARE(m1(1,3), Model::Integer(3));
    QCOMPARE(m1(2,1), Model::Integer(4));
    QCOMPARE(m1(2,2), Model::Integer(5));
    QCOMPARE(m1(2,3), Model::Integer(6));
    QCOMPARE(m1(3,1), Model::Integer(7));
    QCOMPARE(m1(3,2), Model::Integer(8));
    QCOMPARE(m1(3,3), Model::Integer(9));

    Model::MatrixInteger m2;
    m2.update(1, 1);
    m2.update(2, 2);
    m2.update(3, 3);

    QCOMPARE(m2.numberRows(), 3);
    QCOMPARE(m2.numberColumns(), 1);

    QCOMPARE(m2(1,1), Model::Integer(1));
    QCOMPARE(m2(2,1), Model::Integer(2));
    QCOMPARE(m2(3,1), Model::Integer(3));

    Model::MatrixInteger m3;
    m3.update(1, 1, 1);
    m3.update(1, 2, 2);
    m3.update(3, 3);
    m3.update(4, 4);

    QCOMPARE(m3.numberRows(), 1);
    QCOMPARE(m3.numberColumns(), 4);

    QCOMPARE(m3(1,1), Model::Integer(1));
    QCOMPARE(m3(1,2), Model::Integer(2));
    QCOMPARE(m3(1,3), Model::Integer(3));
    QCOMPARE(m3(1,4), Model::Integer(4));
}


void TestMatrixInteger::testSliceMethods() {
    Model::MatrixInteger m = Model::MatrixInteger::build(
        5,
        5,
        Model::Integer( 1), Model::Integer( 6), Model::Integer(11), Model::Integer(16), Model::Integer(21),
        Model::Integer( 2), Model::Integer( 7), Model::Integer(12), Model::Integer(17), Model::Integer(22),
        Model::Integer( 3), Model::Integer( 8), Model::Integer(13), Model::Integer(18), Model::Integer(23),
        Model::Integer( 4), Model::Integer( 9), Model::Integer(14), Model::Integer(19), Model::Integer(24),
        Model::Integer( 5), Model::Integer(10), Model::Integer(15), Model::Integer(20), Model::Integer(25)
    );

    Model::MatrixInteger m1 = m.at(Model::Range(Model::Integer(2),Model::Integer(4)), 2);

    QCOMPARE(m1.numberRows(), 3);
    QCOMPARE(m1.numberColumns(), 1);
    QCOMPARE(m1(1, 1), Model::Integer(7));
    QCOMPARE(m1(2, 1), Model::Integer(12));
    QCOMPARE(m1(3, 1), Model::Integer(17));

    Model::MatrixInteger m2 = m.at(3, Model::Range(Model::Integer(2),Model::Integer(3)));

    QCOMPARE(m2.numberRows(), 1);
    QCOMPARE(m2.numberColumns(), 2);
    QCOMPARE(m2(1, 1), Model::Integer(12));
    QCOMPARE(m2(1, 2), Model::Integer(13));

    Model::MatrixInteger m3 = m.at(
        Model::Range(Model::Integer(1), Model::Integer(3), Model::Integer(5)),
        Model::Range(Model::Integer(2),Model::Integer(4))
    );

    QCOMPARE(m3.numberRows(), 3);
    QCOMPARE(m3.numberColumns(), 3);

    QCOMPARE(m3(1, 1), Model::Integer(2));
    QCOMPARE(m3(1, 2), Model::Integer(3));
    QCOMPARE(m3(1, 3), Model::Integer(4));
    QCOMPARE(m3(2, 1), Model::Integer(12));
    QCOMPARE(m3(2, 2), Model::Integer(13));
    QCOMPARE(m3(2, 3), Model::Integer(14));
    QCOMPARE(m3(3, 1), Model::Integer(22));
    QCOMPARE(m3(3, 2), Model::Integer(23));
    QCOMPARE(m3(3, 3), Model::Integer(24));
}


void TestMatrixInteger::testCombineMethods() {
    Model::MatrixInteger m1 = Model::MatrixInteger::build(
        3, 2,
        Model::Integer(11), Model::Integer(13), Model::Integer(15),
        Model::Integer(12), Model::Integer(14), Model::Integer(16)
    );

    Model::MatrixInteger m2 = Model::MatrixInteger::build(
        2, 3,
        Model::Integer(21), Model::Integer(24),
        Model::Integer(22), Model::Integer(25),
        Model::Integer(23), Model::Integer(26)
    );

    Model::MatrixInteger m3 = m1.combineLeftToRight(m2);
    QCOMPARE(
        m3,
        Model::MatrixInteger::build(
            3, 5,
            Model::Integer(11), Model::Integer(13), Model::Integer(15),
            Model::Integer(12), Model::Integer(14), Model::Integer(16),
            Model::Integer(21), Model::Integer(24), Model::Integer( 0),
            Model::Integer(22), Model::Integer(25), Model::Integer( 0),
            Model::Integer(23), Model::Integer(26), Model::Integer( 0)
        )
    );

    Model::MatrixInteger m4 = m2.combineLeftToRight(m1);
    QCOMPARE(
        m4,
        Model::MatrixInteger::build(
            3, 5,
            Model::Integer(21), Model::Integer(24), Model::Integer( 0),
            Model::Integer(22), Model::Integer(25), Model::Integer( 0),
            Model::Integer(23), Model::Integer(26), Model::Integer( 0),
            Model::Integer(11), Model::Integer(13), Model::Integer(15),
            Model::Integer(12), Model::Integer(14), Model::Integer(16)
        )
    );

    Model::MatrixInteger m5 = m1.combineTopToBottom(m2);
    QCOMPARE(
        m5,
        Model::MatrixInteger::build(
             5, 3,
             Model::Integer(11), Model::Integer(13), Model::Integer(15), Model::Integer(21), Model::Integer(24),
             Model::Integer(12), Model::Integer(14), Model::Integer(16), Model::Integer(22), Model::Integer(25),
             Model::Integer( 0), Model::Integer( 0), Model::Integer( 0), Model::Integer(23), Model::Integer(26)
        )
    );

    Model::MatrixInteger m6 = m2.combineTopToBottom(m1);
    QCOMPARE(
        m6,
        Model::MatrixInteger::build(
             5, 3,
             Model::Integer(21), Model::Integer(24), Model::Integer(11), Model::Integer(13), Model::Integer(15),
             Model::Integer(22), Model::Integer(25), Model::Integer(12), Model::Integer(14), Model::Integer(16),
             Model::Integer(23), Model::Integer(26), Model::Integer( 0), Model::Integer( 0), Model::Integer( 0)
        )
    );
}


void TestMatrixInteger::testReverseMethods() {
    Model::MatrixInteger m = Model::MatrixInteger::build(
        4, 4,
        Model::Integer(11), Model::Integer(21), Model::Integer(31), Model::Integer(41),
        Model::Integer(12), Model::Integer(22), Model::Integer(32), Model::Integer(42),
        Model::Integer(13), Model::Integer(23), Model::Integer(33), Model::Integer(43),
        Model::Integer(14), Model::Integer(24), Model::Integer(34), Model::Integer(44)
    );

    Model::MatrixInteger cr = m.columnReverse();
    QCOMPARE(
        cr,
        Model::MatrixInteger::build(
            4, 4,
            Model::Integer(14), Model::Integer(24), Model::Integer(34), Model::Integer(44),
            Model::Integer(13), Model::Integer(23), Model::Integer(33), Model::Integer(43),
            Model::Integer(12), Model::Integer(22), Model::Integer(32), Model::Integer(42),
            Model::Integer(11), Model::Integer(21), Model::Integer(31), Model::Integer(41)
        )
    );

    Model::MatrixInteger rr = m.rowReverse();
    QCOMPARE(
        rr,
        Model::MatrixInteger::build(
            4, 4,
            Model::Integer(41), Model::Integer(31), Model::Integer(21), Model::Integer(11),
            Model::Integer(42), Model::Integer(32), Model::Integer(22), Model::Integer(12),
            Model::Integer(43), Model::Integer(33), Model::Integer(23), Model::Integer(13),
            Model::Integer(44), Model::Integer(34), Model::Integer(24), Model::Integer(14)
        )
    );
}


void TestMatrixInteger::testIdentity() {
    Model::MatrixInteger i1 = Model::MatrixInteger::identity(10, 12);

    QCOMPARE(i1.numberRows(), 10);
    QCOMPARE(i1.numberColumns(), 12);

    for (unsigned row=1 ; row<=10 ; ++row) {
        for (unsigned column=1 ; column<=12 ; ++column) {
            Model::Integer expected = (row == column) ? Model::Integer(1) : Model::Integer(0);
            Model::Integer measured = i1(row, column);

            QCOMPARE(expected, measured);
        }
    }

    Model::MatrixInteger i2 = Model::MatrixInteger::identity(10);

    QCOMPARE(i2.numberRows(), 10);
    QCOMPARE(i2.numberColumns(), 10);

    for (unsigned row=1 ; row<=10 ; ++row) {
        for (unsigned column=1 ; column<=10 ; ++column) {
            Model::Integer expected = (row == column) ? Model::Integer(1) : Model::Integer(0);
            Model::Integer measured = i2(row, column);

            QCOMPARE(expected, measured);
        }
    }
}


void TestMatrixInteger::testZero() {
    Model::MatrixInteger i1 = Model::MatrixInteger::zero(10, 12);

    QCOMPARE(i1.numberRows(), 10);
    QCOMPARE(i1.numberColumns(), 12);

    for (unsigned row=1 ; row<=10 ; ++row) {
        for (unsigned column=1 ; column<=12 ; ++column) {
            QCOMPARE(i1(row, column), Model::Integer(0));
        }
    }

    Model::MatrixInteger i2 = Model::MatrixInteger::zero(10);

    QCOMPARE(i2.numberRows(), 10);
    QCOMPARE(i2.numberColumns(), 10);

    for (unsigned row=1 ; row<=10 ; ++row) {
        for (unsigned column=1 ; column<=10 ; ++column) {
            QCOMPARE(i2(row, column), Model::Integer(0));
        }
    }
}


void TestMatrixInteger::testOnes() {
    for (M::Integer numberRows=1 ; numberRows<=32 ; ++numberRows) {
        for (M::Integer numberColumns=1 ; numberColumns<=32 ; ++numberColumns) {
            Model::MatrixInteger i1 = Model::MatrixInteger::ones(numberRows, numberColumns);

            QCOMPARE(i1.numberRows(), numberRows);
            QCOMPARE(i1.numberColumns(), numberColumns);

            for (M::Integer row=1 ; row<=numberRows ; ++row) {
                for (M::Integer column=1 ; column<=numberColumns ; ++column) {
                    QCOMPARE(i1(row, column), 1);
                }
            }
        }

        Model::MatrixInteger i2 = Model::MatrixInteger::ones(numberRows);

        QCOMPARE(i2.numberRows(), numberRows);
        QCOMPARE(i2.numberColumns(), numberRows);

        for (unsigned row=1 ; row<=numberRows ; ++row) {
            for (unsigned column=1 ; column<=numberRows ; ++column) {
                QCOMPARE(i2(row, column), 1);
            }
        }
    }
}


void TestMatrixInteger::testDiagnonalEntries() {
    Model::MatrixInteger m = Model::MatrixInteger::build(
        4, 4,
        Model::Integer( 1), Model::Integer( 5), Model::Integer( 9), Model::Integer(13),
        Model::Integer( 2), Model::Integer( 6), Model::Integer(10), Model::Integer(14),
        Model::Integer( 3), Model::Integer( 7), Model::Integer(11), Model::Integer(15),
        Model::Integer( 4), Model::Integer( 8), Model::Integer(12), Model::Integer(16)
    );

    Model::MatrixInteger d = m.diagonalEntries();
    QCOMPARE(d.numberRows(), 4);
    QCOMPARE(d.numberColumns(), 1);

    QCOMPARE(d(1), Model::Integer( 1));
    QCOMPARE(d(2), Model::Integer( 6));
    QCOMPARE(d(3), Model::Integer(11));
    QCOMPARE(d(4), Model::Integer(16));
}


void TestMatrixInteger::testDiagnonal() {
    Model::MatrixInteger d = Model::MatrixInteger::build(
        4, 1,
        Model::Integer(1), Model::Integer(2), Model::Integer(3), Model::Integer(4)
    );

    Model::MatrixInteger m = d.diagonal();

    QCOMPARE(m.numberRows(), 4);
    QCOMPARE(m.numberColumns(), 4);

    for (unsigned row=1 ; row<=4 ; ++row) {
        for (unsigned column=1 ; column<=4 ; ++column) {
            Model::Integer expected = (row == column) ? Model::Integer(row) : Model::Integer(0);
            Model::Integer measured = m(row, column);
            QCOMPARE(measured, expected);
        }
    }
}


void TestMatrixInteger::testMatrixTypeMethods() {
    Model::MatrixInteger rectangular = Model::MatrixInteger::build(
        3, 2,
        Model::Integer(1), Model::Integer(3), Model::Integer(5),
        Model::Integer(2), Model::Integer(4), Model::Integer(6)
    );

    Model::MatrixInteger square = Model::MatrixInteger::build(
        3, 3,
        Model::Integer(1), Model::Integer(4), Model::Integer(7),
        Model::Integer(2), Model::Integer(5), Model::Integer(8),
        Model::Integer(3), Model::Integer(6), Model::Integer(9)
    );

    QCOMPARE(rectangular.isSquare(), false);
    QCOMPARE(square.isSquare(), true);
}


void TestMatrixInteger::testTranspose() {
    Model::MatrixInteger m = Model::MatrixInteger::build(
        2, 3,
        Model::Integer(1), Model::Integer(4),
        Model::Integer(2), Model::Integer(5),
        Model::Integer(3), Model::Integer(6)
    );

    Model::MatrixInteger t1 = m.transpose();
    QCOMPARE(t1.numberRows(), 3);
    QCOMPARE(t1.numberColumns(), 2);

    QCOMPARE(
        t1,
        Model::MatrixInteger::build(
            3, 2,
            Model::Integer(1), Model::Integer(2), Model::Integer(3),
            Model::Integer(4), Model::Integer(5), Model::Integer(6)
        )
    );

    Model::MatrixInteger t2 = m.transpose();
    Model::MatrixInteger t3 = 2 * t2;
    QCOMPARE(
        t3,
        Model::MatrixInteger::build(
            3, 2,
            Model::Integer( 2), Model::Integer( 4), Model::Integer( 6),
            Model::Integer( 8), Model::Integer(10), Model::Integer(12)
        )
    );
}


void TestMatrixInteger::testComplexConjugate() {
    Model::MatrixInteger m = Model::MatrixInteger::build(
        2, 3,
        Model::Integer(1), Model::Integer(4),
        Model::Integer(2), Model::Integer(5),
        Model::Integer(3), Model::Integer(6)
    );

    Model::MatrixInteger t1 = m.conj();
    QCOMPARE(t1.numberRows(), 2);
    QCOMPARE(t1.numberColumns(), 3);

    QCOMPARE(
        t1,
        Model::MatrixInteger::build(
            2, 3,
            Model::Integer(1), Model::Integer(4),
            Model::Integer(2), Model::Integer(5),
            Model::Integer(3), Model::Integer(6)
        )
    );
}


void TestMatrixInteger::testAdjoint() {
    Model::MatrixInteger m = Model::MatrixInteger::build(
        2, 3,
        Model::Integer(1), Model::Integer(4),
        Model::Integer(2), Model::Integer(5),
        Model::Integer(3), Model::Integer(6)
    );

    Model::MatrixInteger t1 = m.adjoint();
    QCOMPARE(t1.numberRows(), 3);
    QCOMPARE(t1.numberColumns(), 2);

    QCOMPARE(
        t1,
        Model::MatrixInteger::build(
            3, 2,
            Model::Integer(1), Model::Integer(2), Model::Integer(3),
            Model::Integer(4), Model::Integer(5), Model::Integer(6)
        )
    );

    Model::MatrixInteger t2 = m.transpose();
    Model::MatrixInteger t3 = 2 * t2;
    QCOMPARE(
        t3,
        Model::MatrixInteger::build(
            3, 2,
            Model::Integer( 2), Model::Integer( 4), Model::Integer( 6),
            Model::Integer( 8), Model::Integer(10), Model::Integer(12)
        )
    );
}


void TestMatrixInteger::testHadamardProduct() {
    std::mt19937                    rng(0x12345678);
    std::uniform_int_distribution<> coefficientDistribution(-1E6, +1E6);
    std::uniform_int_distribution<> dimensionDistribution(1, 1000);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned long numberRows    = dimensionDistribution(rng);
        unsigned long numberColumns = dimensionDistribution(rng);

        Model::MatrixInteger m1(numberRows, numberColumns);
        Model::MatrixInteger m2(numberRows, numberColumns);
        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                Model::Integer v1 = coefficientDistribution(rng);
                m1.update(row, column, v1);

                Model::Integer v2 = coefficientDistribution(rng);
                m2.update(row, column, v2);
            }
        }

        Model::MatrixInteger m = m1.hadamard(m2);

        QCOMPARE(static_cast<unsigned long>(m.numberRows()), numberRows);
        QCOMPARE(static_cast<unsigned long>(m.numberColumns()), numberColumns);

        for (unsigned long row=1 ; row<=numberRows ; ++row) {
            for (unsigned long column=1 ; column<=numberColumns ; ++column) {
                Model::Integer expected = m1(row, column) * m2(row, column);
                Model::Integer measured = m(row, column);

                QCOMPARE(measured, expected);
            }
        }
    }
}


void TestMatrixInteger::testKroneckerProduct() {
    std::mt19937                    rng(0x12345678);
    std::uniform_int_distribution<> coefficientDistribution(-1E6, +1E6);
    std::uniform_int_distribution<> dimensionDistribution(1, 50);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned long multiplierNumberRows      = dimensionDistribution(rng);
        unsigned long multiplierNumberColumns   = dimensionDistribution(rng);
        unsigned long multiplicandNumberRows    = dimensionDistribution(rng);
        unsigned long multiplicandNumberColumns = dimensionDistribution(rng);

        Model::MatrixInteger m1(multiplierNumberRows, multiplierNumberColumns);
        Model::MatrixInteger m2(multiplicandNumberRows, multiplicandNumberColumns);

        for (unsigned long column=1 ; column<=multiplierNumberColumns ; ++column) {
            for (unsigned long row=1 ; row<=multiplierNumberRows ; ++row) {
                Model::Integer v1 = coefficientDistribution(rng);
                m1.update(row, column, v1);
            }
        }

        for (unsigned long column=1 ; column<=multiplicandNumberColumns ; ++column) {
            for (unsigned long row=1 ; row<=multiplicandNumberRows ; ++row) {
                Model::Integer v2 = coefficientDistribution(rng);
                m2.update(row, column, v2);
            }
        }

        Model::MatrixInteger m = m1.kronecker(m2);

        QCOMPARE(static_cast<unsigned long>(m.numberRows()), multiplierNumberRows * multiplicandNumberRows);
        QCOMPARE(static_cast<unsigned long>(m.numberColumns()), multiplierNumberColumns * multiplicandNumberColumns);

        for (unsigned long multiplierRow=1 ; multiplierRow<=multiplierNumberRows ; ++multiplierRow) {
            unsigned long productRowOffset = (multiplierRow - 1) * multiplicandNumberRows;

            for (unsigned long multiplierColumn=1 ; multiplierColumn<=multiplierNumberColumns ; ++multiplierColumn) {
                unsigned long  productColumnOffset = (multiplierColumn - 1) * multiplicandNumberColumns;
                Model::Integer multiplier          = m1(multiplierRow, multiplierColumn);

                for (unsigned long multiplicandRow=1 ; multiplicandRow<=multiplicandNumberRows ; ++multiplicandRow) {
                    unsigned long productRow = productRowOffset + multiplicandRow;

                    for (  unsigned long multiplicandColumn = 1
                         ; multiplicandColumn <= multiplicandNumberColumns
                         ; ++multiplicandColumn
                        ) {
                        unsigned long productColumn = productColumnOffset + multiplicandColumn;

                        Model::Integer multiplicand = m2(multiplicandRow, multiplicandColumn);
                        Model::Integer expected     = multiplier * multiplicand;
                        Model::Integer measured     = m(productRow, productColumn);

                        QCOMPARE(measured, expected);
                    }
                }
            }
        }
    }
}


void TestMatrixInteger::testRawDataAccess() {
    Model::MatrixInteger m = Model::MatrixInteger::build(
        2, 3,
        Model::Integer(1), Model::Integer(4),
        Model::Integer(2), Model::Integer(5),
        Model::Integer(3), Model::Integer(6)
    );

    const Model::Integer* rawData = m.data();

    QCOMPARE(rawData[ 0], Model::Integer(1));
    QCOMPARE(rawData[ 1], Model::Integer(4));
    QCOMPARE(rawData[ 2], Model::Integer(0));
    QCOMPARE(rawData[ 3], Model::Integer(0));
    QCOMPARE(rawData[ 4], Model::Integer(0));
    QCOMPARE(rawData[ 5], Model::Integer(0));
    QCOMPARE(rawData[ 6], Model::Integer(0));
    QCOMPARE(rawData[ 7], Model::Integer(0));

    QCOMPARE(rawData[ 8], Model::Integer(2));
    QCOMPARE(rawData[ 9], Model::Integer(5));
    QCOMPARE(rawData[10], Model::Integer(0));
    QCOMPARE(rawData[11], Model::Integer(0));
    QCOMPARE(rawData[12], Model::Integer(0));
    QCOMPARE(rawData[13], Model::Integer(0));
    QCOMPARE(rawData[14], Model::Integer(0));
    QCOMPARE(rawData[15], Model::Integer(0));

    QCOMPARE(rawData[16], Model::Integer(3));
    QCOMPARE(rawData[17], Model::Integer(6));
    QCOMPARE(rawData[18], Model::Integer(0));
    QCOMPARE(rawData[19], Model::Integer(0));
    QCOMPARE(rawData[20], Model::Integer(0));
    QCOMPARE(rawData[21], Model::Integer(0));
    QCOMPARE(rawData[22], Model::Integer(0));
    QCOMPARE(rawData[23], Model::Integer(0));
}


void TestMatrixInteger::testAssignmentOperators() {
    Model::MatrixInteger m1(2, 3);
    QCOMPARE(m1.numberRows(),    2);
    QCOMPARE(m1.numberColumns(), 3);

    m1.update(1, 1, Model::Integer(1));
    m1.update(1, 2, Model::Integer(2));
    m1.update(1, 3, Model::Integer(3));
    m1.update(2, 1, Model::Integer(4));
    m1.update(2, 2, Model::Integer(5));
    m1.update(2, 3, Model::Integer(6));

    Model::MatrixInteger m2(1, 2);
    m2.update(1, 1, Model::Integer(7));
    m2.update(1, 2, Model::Integer(8));

    QCOMPARE(m1.numberRows(),    2);
    QCOMPARE(m1.numberColumns(), 3);

    QCOMPARE(m1(1,1), Model::Integer(1));
    QCOMPARE(m1(1,2), Model::Integer(2));
    QCOMPARE(m1(1,3), Model::Integer(3));
    QCOMPARE(m1(2,1), Model::Integer(4));
    QCOMPARE(m1(2,2), Model::Integer(5));
    QCOMPARE(m1(2,3), Model::Integer(6));

    QCOMPARE(m2.numberRows(),    1);
    QCOMPARE(m2.numberColumns(), 2);
    QCOMPARE(m2(1,1), Model::Integer(7));
    QCOMPARE(m2(1,2), Model::Integer(8));

    m2 = m1;

    QCOMPARE(m2.numberRows(),    2);
    QCOMPARE(m2.numberColumns(), 3);

    QCOMPARE(m2(1,1), Model::Integer(1));
    QCOMPARE(m2(1,2), Model::Integer(2));
    QCOMPARE(m2(1,3), Model::Integer(3));
    QCOMPARE(m2(2,1), Model::Integer(4));
    QCOMPARE(m2(2,2), Model::Integer(5));
    QCOMPARE(m2(2,3), Model::Integer(6));

    m1.update(1, 1, Model::Integer(10));
    m1.update(1, 2, Model::Integer(11));
    m1.update(1, 3, Model::Integer(12));
    m1.update(2, 1, Model::Integer(13));
    m1.update(2, 2, Model::Integer(14));
    m1.update(2, 3, Model::Integer(15));

    QCOMPARE(m2(1,1), Model::Integer(1));
    QCOMPARE(m2(1,2), Model::Integer(2));
    QCOMPARE(m2(1,3), Model::Integer(3));
    QCOMPARE(m2(2,1), Model::Integer(4));
    QCOMPARE(m2(2,2), Model::Integer(5));
    QCOMPARE(m2(2,3), Model::Integer(6));
}


void TestMatrixInteger::testAddAssignmentOperators() {
    std::mt19937                    rng(0x12345678);
    std::uniform_int_distribution<> coefficientDistribution(-1024 * 1024, +1024 * 1024);
    std::uniform_int_distribution<> dimensionDistribution(1, 1000);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned numberRows    = dimensionDistribution(rng);
        unsigned numberColumns = dimensionDistribution(rng);

        Model::MatrixInteger expected1(numberRows, numberColumns);
        Model::MatrixInteger expected2(numberColumns, numberRows);

        Model::MatrixInteger augend(numberRows, numberColumns);
        Model::MatrixInteger addend(numberRows, numberColumns);

        for (unsigned long columnIndex=1 ; columnIndex<=numberColumns ; ++columnIndex) {
            for (unsigned long rowIndex=1 ; rowIndex<=numberRows ; ++rowIndex) {
                Model::Integer a1 = coefficientDistribution(rng);
                Model::Integer a2 = coefficientDistribution(rng);

                augend.update(rowIndex, columnIndex, a1);
                addend.update(rowIndex, columnIndex, a2);

                expected1.update(rowIndex, columnIndex, a1 + a2);
                expected2.update(columnIndex, rowIndex, 2 * a1 + 3 * a2);
            }
        }

        Model::MatrixInteger measured1 = augend;
        measured1 += addend;

        Model::MatrixInteger measured2 = 2 * augend.transpose();
        measured2 += 3 * addend.transpose();

        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                Model::Integer e1 = expected1(row, column);
                Model::Integer m1 = measured1(row, column);

                QCOMPARE(m1, e1);

                Model::Integer e2 = expected2(column, row);
                Model::Integer m2 = measured2(column, row);

                QCOMPARE(m2, e2);
            }
        }
    }

    Model::MatrixInteger m1(10, 11);
    Model::MatrixInteger m2(12, 13);

    bool caughtException = false;
    bool exceptionValid  = false;
    try {
        m1 += m2; // should throw an exception.
    } catch (const Model::IncompatibleMatrixDimensions& e) {
        caughtException = true;

        exceptionValid  = (
               e.multiplierRows() == 10
            && e.multiplierColumns() == 11
            && e.multiplicandRows() == 12
            && e.multiplicandColumns() == 13
        );
    } catch (...) {
        caughtException = true;
        exceptionValid  = false;
    }

    QCOMPARE(caughtException, true);
    QCOMPARE(exceptionValid, true);
}


void TestMatrixInteger::testSubtractAssignmentOperators() {
    std::mt19937                    rng(0x12345678);
    std::uniform_int_distribution<> coefficientDistribution(-1024 * 1024, +1024 * 1024);
    std::uniform_int_distribution<> dimensionDistribution(1, 1000);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned numberRows    = dimensionDistribution(rng);
        unsigned numberColumns = dimensionDistribution(rng);

        Model::MatrixInteger expected1(numberRows, numberColumns);
        Model::MatrixInteger expected2(numberColumns, numberRows);

        Model::MatrixInteger augend(numberRows, numberColumns);
        Model::MatrixInteger addend(numberRows, numberColumns);

        for (unsigned long columnIndex=1 ; columnIndex<=numberColumns ; ++columnIndex) {
            for (unsigned long rowIndex=1 ; rowIndex<=numberRows ; ++rowIndex) {
                Model::Integer a1 = coefficientDistribution(rng);
                Model::Integer a2 = coefficientDistribution(rng);

                augend.update(rowIndex, columnIndex, a1);
                addend.update(rowIndex, columnIndex, a2);

                expected1.update(rowIndex, columnIndex, a1 - a2);
                expected2.update(columnIndex, rowIndex, 2 * a1 - 3 * a2);
            }
        }

        Model::MatrixInteger measured1 = augend;
        measured1 -= addend;

        Model::MatrixInteger measured2 = 2 * augend.transpose();
        measured2 -= 3 * addend.transpose();

        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                Model::Integer e1 = expected1(row, column);
                Model::Integer m1 = measured1(row, column);

                QCOMPARE(m1, e1);

                Model::Integer e2 = expected2(column, row);
                Model::Integer m2 = measured2(column, row);

                QCOMPARE(m2, e2);
            }
        }
    }

    Model::MatrixInteger m1(10, 11);
    Model::MatrixInteger m2(12, 13);

    bool caughtException = false;
    bool exceptionValid  = false;
    try {
        m1 -= m2; // should throw an exception.
    } catch (const Model::IncompatibleMatrixDimensions& e) {
        caughtException = true;

        exceptionValid  = (
               e.multiplierRows() == 10
            && e.multiplierColumns() == 11
            && e.multiplicandRows() == 12
            && e.multiplicandColumns() == 13
        );
    } catch (...) {
        caughtException = true;
        exceptionValid  = false;
    }

    QCOMPARE(caughtException, true);
    QCOMPARE(exceptionValid, true);
}


void TestMatrixInteger::testMatrixMatrixMultiplyAssignmentOperator() {
    // For now we just to a cursory test.  Since the underlying engine is the same, we do a more thorough test below.

    std::mt19937                    rng(0x12345678);
    std::uniform_int_distribution<> coefficientDistribution(-1024 * 1024, +1024 * 1024);
    std::uniform_int_distribution<> dimensionDistribution(1, 1000);

    unsigned multiplierRows      = dimensionDistribution(rng);
    unsigned multiplierColumns   = dimensionDistribution(rng);
    unsigned multiplicandRows    = multiplierColumns;
    unsigned multiplicandColumns = dimensionDistribution(rng);

    Model::MatrixInteger multiplier(multiplierRows, multiplierColumns);
    for (unsigned row=1 ; row<=multiplierRows ; ++row) {
        for (unsigned column=1 ; column<multiplierColumns ; ++column) {
            Model::Integer c = coefficientDistribution(rng);
            multiplier.update(row, column, c);
        }
    }

    Model::MatrixInteger multiplicand(multiplicandRows, multiplicandColumns);
    for (unsigned row=1 ; row<=multiplicandRows ; ++row) {
        for (unsigned column=1 ; column<multiplicandColumns ; ++column) {
            Model::Integer c = coefficientDistribution(rng);
            multiplicand.update(row, column, c);
        }
    }

    Model::MatrixInteger measuredMatrix = multiplier;
    measuredMatrix *= multiplicand;

    QCOMPARE(measuredMatrix.numberRows(), multiplierRows);
    QCOMPARE(measuredMatrix.numberColumns(), multiplicandColumns);

    for (unsigned row=1 ; row<=multiplierRows ; ++row) {
        for (unsigned column=1 ; column<=multiplicandColumns ; ++column) {
            Model::Integer expected(0);
            for (unsigned multiplierColumn=1 ; multiplierColumn<=multiplierColumns ; ++multiplierColumn) {
                expected += multiplier(row, multiplierColumn) * multiplicand(multiplierColumn, column);
            }

            Model::Real measured = measuredMatrix(row, column);

            if (measured != expected) {
                QCOMPARE(measured, expected);
            }
        }
    }

    Model::MatrixInteger m1(10, 11);
    Model::MatrixInteger m2(12, 13);

    bool caughtException = false;
    bool exceptionValid  = false;
    try {
        m1 *= m2; // should throw an exception.
    } catch (const Model::IncompatibleMatrixDimensions& e) {
        caughtException = true;

        exceptionValid  = (
               e.multiplierRows() == 10
            && e.multiplierColumns() == 11
            && e.multiplicandRows() == 12
            && e.multiplicandColumns() == 13
        );
    } catch (...) {
        caughtException = true;
        exceptionValid  = false;
    }

    QCOMPARE(caughtException, true);
    QCOMPARE(exceptionValid, true);
}


void TestMatrixInteger::testMatrixScalarMultiplyAssignmentOperators() {
    std::mt19937                    rng(0x12345678);
    std::uniform_int_distribution<> coefficientDistribution(-1024 * 1024, +1024 * 1024);
    std::uniform_int_distribution<> dimensionDistribution(1, 1000);

    unsigned multiplierRows    = dimensionDistribution(rng);
    unsigned multiplierColumns = dimensionDistribution(rng);

    Model::MatrixInteger multiplier(multiplierRows, multiplierColumns);
    for (unsigned row=1 ; row<=multiplierRows ; ++row) {
        for (unsigned column=1 ; column<multiplierColumns ; ++column) {
            Model::Integer c = coefficientDistribution(rng);
            multiplier.update(row, column, c);
        }
    }

    Model::MatrixInteger booleanTrueMeasured = multiplier;
    booleanTrueMeasured  *= Model::Boolean(true);

    Model::MatrixInteger booleanFalseMeasured = multiplier;
    booleanFalseMeasured *= Model::Boolean(false);

    Model::Integer       integerScalar   = dimensionDistribution(rng);
    Model::MatrixInteger integerMeasured = multiplier;
    integerMeasured *= integerScalar;

    QCOMPARE(booleanTrueMeasured.numberRows(), multiplierRows);
    QCOMPARE(booleanTrueMeasured.numberColumns(), multiplierColumns);

    QCOMPARE(booleanFalseMeasured.numberRows(), multiplierRows);
    QCOMPARE(booleanFalseMeasured.numberColumns(), multiplierColumns);

    QCOMPARE(integerMeasured.numberRows(), multiplierRows);
    QCOMPARE(integerMeasured.numberColumns(), multiplierColumns);

    for (unsigned row=1 ; row<=multiplierRows ; ++row) {
        for (unsigned column=1 ; column<=multiplierColumns ; ++column) {
            QCOMPARE(booleanTrueMeasured(row, column), multiplier(row, column));
            QCOMPARE(booleanFalseMeasured(row, column), Model::Integer(0));

            Model::Integer expected = multiplier(row, column) * integerScalar;
            Model::Integer measured = integerMeasured(row, column);

            QCOMPARE(measured, expected);
        }
    }
}


void TestMatrixInteger::testAdditionOperators() {
    std::mt19937                    rng(0x12345678);
    std::uniform_int_distribution<> coefficientDistribution(-1024 * 1024, +1024 * 1024);
    std::uniform_int_distribution<> dimensionDistribution(1, 1000);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned numberRows      = dimensionDistribution(rng);
        unsigned numberColumns   = dimensionDistribution(rng);

        Model::MatrixInteger augend(numberRows, numberColumns);
        Model::MatrixInteger addend(numberRows, numberColumns);

        Model::MatrixInteger expected1(numberRows, numberColumns);
        Model::MatrixInteger expected2(numberColumns, numberRows);

        for (unsigned long columnIndex=1 ; columnIndex<=numberColumns ; ++columnIndex) {
            for (unsigned long rowIndex=1 ; rowIndex<=numberRows ; ++rowIndex) {
                Model::Integer a1 = coefficientDistribution(rng);
                Model::Integer a2 = coefficientDistribution(rng);

                augend.update(rowIndex, columnIndex, a1);
                addend.update(rowIndex, columnIndex, a2);

                expected1.update(rowIndex, columnIndex, a1 + a2);
                expected2.update(columnIndex, rowIndex, 2 * a1 + 3 * a2);
            }
        }

        Model::MatrixInteger measured1 = augend + addend;
        Model::MatrixInteger measured2 = 2 * augend.transpose() + 3 * addend.transpose();

        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                Model::Integer e1 = expected1(row, column);
                Model::Integer m1 = measured1(row, column);

                QCOMPARE(m1, e1);

                Model::Integer e2 = expected2(column, row);
                Model::Integer m2 = measured2(column, row);

                QCOMPARE(m2, e2);
            }
        }
    }

    Model::MatrixInteger m1(10, 11);
    Model::MatrixInteger m2(12, 13);
    Model::MatrixInteger s;

    bool caughtException = false;
    bool exceptionValid  = false;
    try {
        s = m1 + m2; // should throw an exception.
    } catch (const Model::IncompatibleMatrixDimensions& e) {
        caughtException = true;

        exceptionValid  = (
               e.multiplierRows() == 10
            && e.multiplierColumns() == 11
            && e.multiplicandRows() == 12
            && e.multiplicandColumns() == 13
        );
    } catch (...) {
        caughtException = true;
        exceptionValid  = false;
    }

    QCOMPARE(caughtException, true);
    QCOMPARE(exceptionValid, true);
}


void TestMatrixInteger::testSubtractionOperators() {
    std::mt19937                    rng(0x12345678);
    std::uniform_int_distribution<> coefficientDistribution(-1024 * 1024, +1024 * 1024);
    std::uniform_int_distribution<> dimensionDistribution(1, 1000);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned numberRows      = dimensionDistribution(rng);
        unsigned numberColumns   = dimensionDistribution(rng);

        Model::MatrixInteger augend(numberRows, numberColumns);
        Model::MatrixInteger addend(numberRows, numberColumns);

        Model::MatrixInteger expected1(numberRows, numberColumns);
        Model::MatrixInteger expected2(numberColumns, numberRows);

        for (unsigned long columnIndex=1 ; columnIndex<=numberColumns ; ++columnIndex) {
            for (unsigned long rowIndex=1 ; rowIndex<=numberRows ; ++rowIndex) {
                Model::Integer a1 = coefficientDistribution(rng);
                Model::Integer a2 = coefficientDistribution(rng);

                augend.update(rowIndex, columnIndex, a1);
                addend.update(rowIndex, columnIndex, a2);

                expected1.update(rowIndex, columnIndex, a1 - a2);
                expected2.update(columnIndex, rowIndex, 2 * a1 - 3 * a2);
            }
        }

        Model::MatrixInteger measured1 = augend - addend;
        Model::MatrixInteger measured2 = 2 * augend.transpose() - 3 * addend.transpose();

        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                Model::Integer e1 = expected1(row, column);
                Model::Integer m1 = measured1(row, column);

                QCOMPARE(m1, e1);

                Model::Integer e2 = expected2(column, row);
                Model::Integer m2 = measured2(column, row);

                QCOMPARE(m2, e2);
            }
        }
    }

    Model::MatrixInteger m1(10, 11);
    Model::MatrixInteger m2(12, 13);
    Model::MatrixInteger s;

    bool caughtException = false;
    bool exceptionValid  = false;
    try {
        s = m1 - m2; // should throw an exception.
    } catch (const Model::IncompatibleMatrixDimensions& e) {
        caughtException = true;

        exceptionValid  = (
               e.multiplierRows() == 10
            && e.multiplierColumns() == 11
            && e.multiplicandRows() == 12
            && e.multiplicandColumns() == 13
        );
    } catch (...) {
        caughtException = true;
        exceptionValid  = false;
    }

    QCOMPARE(caughtException, true);
    QCOMPARE(exceptionValid, true);
}


void TestMatrixInteger::testMatrixMatrixMultiplicationOperator() {
    std::mt19937 rng(0x12345678);
    std::uniform_int_distribution<> coefficientDistribution(-1024 * 1024, +1024 * 1024);

    // We test three scenarios, the larger the matrix, the fewer times we must iterate to keep the test time
    // reasonable.
    unsigned mode4Iterations = std::max(1U, TestMatrixInteger::numberIterations / 1000);
    unsigned mode3Iterations = std::max(1U, TestMatrixInteger::numberIterations / 200);
    unsigned mode2Iterations = std::max(1U, TestMatrixInteger::numberIterations / 100);
    unsigned mode1Iterations = std::max(1U, TestMatrixInteger::numberIterations /  10);
    unsigned mode0Iterations = (
          TestMatrixInteger::numberIterations
        - mode1Iterations
        - mode2Iterations
        - mode3Iterations
        - mode4Iterations
    );

    for (unsigned mode=0 ; mode<4 ; ++mode) {
        unsigned    maximumDimension = 0;
        unsigned    numberIterations = 0;

        switch (mode) {
            case 0: {
                maximumDimension = 100;
                numberIterations = mode0Iterations;

                break;
            }

            case 1: {
                maximumDimension = 200;
                numberIterations = mode1Iterations;

                break;
            }

            case 2: {
                maximumDimension  = 500;
                numberIterations  = mode2Iterations;

                break;
            }


            case 3: {
                maximumDimension  = 1000;
                numberIterations  = mode2Iterations;

                break;
            }

            case 4: {
                maximumDimension = 10000;
                numberIterations = mode4Iterations;

                break;
            }

            default: {
                Q_ASSERT(false);
                break;
            }
        }

        std::uniform_int_distribution<> dimensionDistribution(1, maximumDimension);

        for (unsigned i=0 ; i<numberIterations ; ++i) {
            unsigned multiplierRows      = dimensionDistribution(rng);
            unsigned multiplierColumns   = dimensionDistribution(rng);
            unsigned multiplicandRows    = multiplierColumns;
            unsigned multiplicandColumns = dimensionDistribution(rng);

            Model::MatrixInteger multiplier(multiplierRows, multiplierColumns);
            for (unsigned row=1 ; row<=multiplierRows ; ++row) {
                for (unsigned column=1 ; column<multiplierColumns ; ++column) {
                    Model::Integer c = coefficientDistribution(rng);
                    multiplier.update(row, column, c);
                }
            }

            Model::MatrixInteger multiplicand(multiplicandRows, multiplicandColumns);
            for (unsigned row=1 ; row<=multiplicandRows ; ++row) {
                for (unsigned column=1 ; column<multiplicandColumns ; ++column) {
                    Model::Integer c = coefficientDistribution(rng);
                    multiplicand.update(row, column, c);
                }
            }

            Model::MatrixInteger measuredMatrix = multiplier * multiplicand;

            QCOMPARE(measuredMatrix.numberRows(), multiplierRows);
            QCOMPARE(measuredMatrix.numberColumns(), multiplicandColumns);

            for (unsigned row=1 ; row<=multiplierRows ; ++row) {
                for (unsigned column=1 ; column<=multiplicandColumns ; ++column) {
                    Model::Integer expected(0);
                    for (unsigned multiplierColumn=1 ; multiplierColumn<=multiplierColumns ; ++multiplierColumn) {
                        expected += multiplier(row, multiplierColumn) * multiplicand(multiplierColumn, column);
                    }

                    Model::Integer measured   = measuredMatrix(row, column);

                    QCOMPARE(measured, expected);
                }
            }
        }
    }

    Model::MatrixInteger m1(10, 11);
    Model::MatrixInteger m2(12, 13);
    Model::MatrixInteger p;

    bool caughtException = false;
    bool exceptionValid  = false;
    try {
        p = m1 * m2; // should throw an exception.
    } catch (const Model::IncompatibleMatrixDimensions& e) {
        caughtException = true;

        exceptionValid  = (
               e.multiplierRows() == 10
            && e.multiplierColumns() == 11
            && e.multiplicandRows() == 12
            && e.multiplicandColumns() == 13
        );
    } catch (...) {
        caughtException = true;
        exceptionValid  = false;
    }

    QCOMPARE(caughtException, true);
    QCOMPARE(exceptionValid, true);
}


void TestMatrixInteger::testMatrixScalarMultiplicationOperators() {
    std::mt19937                    rng(0x12345678);
    std::uniform_int_distribution<> coefficientDistribution(-1024 * 1024, +1024 * 1024);
    std::uniform_int_distribution<> dimensionDistribution(1, 100);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned multiplierRows    = dimensionDistribution(rng);
        unsigned multiplierColumns = dimensionDistribution(rng);

        Model::MatrixInteger multiplier(multiplierRows, multiplierColumns);
        for (unsigned row=1 ; row<=multiplierRows ; ++row) {
            for (unsigned column=1 ; column<multiplierColumns ; ++column) {
                Model::Integer c = coefficientDistribution(rng);
                multiplier.update(row, column, c);
            }
        }

        Model::Boolean booleanScalar = (dimensionDistribution(rng) % 2) != 0;
        Model::Integer integerScalar = dimensionDistribution(rng);

        bool tm = (dimensionDistribution(rng) % 2) != 0;

        Model::MatrixInteger booleanMeasured  = tm ? multiplier * booleanScalar : booleanScalar * multiplier;
        Model::MatrixInteger integerMeasured  = tm ? multiplier * integerScalar : integerScalar * multiplier;

        QCOMPARE(booleanMeasured.numberRows(), multiplierRows);
        QCOMPARE(booleanMeasured.numberColumns(), multiplierColumns);

        QCOMPARE(integerMeasured.numberRows(), multiplierRows);
        QCOMPARE(integerMeasured.numberColumns(), multiplierColumns);

        for (unsigned row=1 ; row<=multiplierRows ; ++row) {
            for (unsigned column=1 ; column<=multiplierColumns ; ++column) {
                QCOMPARE(booleanMeasured(row, column), booleanScalar ? multiplier(row, column) : Model::Integer(0));

                Model::Integer integerExpected = multiplier(row, column) * integerScalar;
                QCOMPARE(integerMeasured(row, column), integerExpected);
            }
        }
    }
}


void TestMatrixInteger::testUnaryPlusMinusOperators() {
    Model::MatrixInteger e = Model::MatrixInteger::build(
        3, 3,
        Model::Integer( 1), Model::Integer(-4), Model::Integer( 7),
        Model::Integer(-2), Model::Integer( 5), Model::Integer(-8),
        Model::Integer( 3), Model::Integer(-6), Model::Integer( 9)
    );

    Model::MatrixInteger p = +e;
    Model::MatrixInteger m = -e;

    for (unsigned row=1 ; row<=3 ; ++row) {
        for (unsigned column=1 ; column<=3 ; ++column) {
            QCOMPARE(p(row, column), e(row, column));
            QCOMPARE(m(row, column), -e(row, column));
        }
    }
}


void TestMatrixInteger::testComparisonOperators() {
    Model::MatrixInteger m1 = Model::MatrixInteger::build(
        3, 3,
        Model::Integer( 1), Model::Integer(-4), Model::Integer( 7),
        Model::Integer(-2), Model::Integer( 5), Model::Integer(-8),
        Model::Integer( 3), Model::Integer(-6), Model::Integer( 9)
    );

    Model::MatrixInteger m2 = Model::MatrixInteger::build(
        2, 3,
        Model::Integer(10), Model::Integer(13),
        Model::Integer(11), Model::Integer(14),
        Model::Integer(12), Model::Integer(15)
    );

    Model::MatrixInteger m3 = Model::MatrixInteger::build(
        3, 2,
        Model::Integer(10), Model::Integer(12), Model::Integer(14),
        Model::Integer(11), Model::Integer(13), Model::Integer(15)
    );

    Model::MatrixInteger m4 = Model::MatrixInteger::build( // same as m1
        3, 3,
        Model::Integer( 1), Model::Integer(-4), Model::Integer( 7),
        Model::Integer(-2), Model::Integer( 5), Model::Integer(-8),
        Model::Integer( 3), Model::Integer(-6), Model::Integer( 9)
    );

    Model::MatrixInteger m5 = Model::MatrixInteger::build( // one value different from m1
        3, 3,
        Model::Integer( 1), Model::Integer(-4), Model::Integer( 7),
        Model::Integer(-2), Model::Integer(15), Model::Integer(-8),
        Model::Integer( 3), Model::Integer(-6), Model::Integer( 9)
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
}


void TestMatrixInteger::testSubscriptingOperators() {
//    // Tested by other methods.
}


void TestMatrixInteger::testMultiplicationFunctions() {
//    // Tested by TestMatrixInteger::testMatrixScalarMultiplicationOperators
}


void TestMatrixInteger::testIterator() {
    M::MatrixInteger m = M::MatrixInteger::build(
        3, 3,
        Model::Integer( 1), Model::Integer( 7), Model::Integer(13),
        Model::Integer( 3), Model::Integer( 9), Model::Integer(15),
        Model::Integer( 5), Model::Integer(11), Model::Integer(17)
    );

    M::MatrixInteger::Iterator it = m.begin();
    QCOMPARE(it != m.end(), true);
    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);

    QCOMPARE(it.value(), Model::Integer(1));
    QCOMPARE(it.constReference(), Model::Integer(1));
    QCOMPARE(*(it.constPointer()), Model::Integer(1));
    QCOMPARE(*it, Model::Integer(1));

    ++it;
    QCOMPARE(it != m.end(), true);
    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(*it, Model::Integer(3));

    M::MatrixInteger::Iterator it2 = it++;
    QCOMPARE(*it2, Model::Integer(3));

    QCOMPARE(it != m.end(), true);
    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(*it, Model::Integer(5));

    it.advance(2);

    QCOMPARE(it != m.end(), true);
    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(*it, Model::Integer(9));

    it.advance(2);

    QCOMPARE(it != m.end(), true);
    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(*it, Model::Integer(13));

    it.advance(2);

    QCOMPARE(it != m.end(), true);
    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(*it, Model::Integer(17));

    ++it;

    QCOMPARE(it == m.end(), true);
    QCOMPARE(it.isValid(), false);
    QCOMPARE(it.isInvalid(), true);
}


void TestMatrixInteger::dumpMatrix(const Model::MatrixInteger& matrix) {
    unsigned long numberRows = matrix.numberRows();
    unsigned long numberColumns = matrix.numberColumns();

    for (unsigned long row=1 ; row<=numberRows ; ++row) {
        QString t;
        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            if (!t.isEmpty()) {
                t +="  |  ";
            }

            Model::Integer v = matrix(row, column);
            QString     s =QString("%1").arg(v, 8).trimmed();
            t += QString("%1").arg(s, 20);
        }

        qDebug() << t;
    }
}
