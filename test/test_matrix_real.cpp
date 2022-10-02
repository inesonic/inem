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
* This file implements tests of the \ref Model::MatrixReal class.
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
#include <model_matrix_real.h>
#include <m_intrinsic_types.h>
#include <m_matrix_real.h>
#include <m_variant.h>
#include <m_range.h>
#include <m_set.h>
#include <m_tuple.h>
#include <m_matrix_integer.h>
#include <m_matrix_complex.h>

#include "test_matrix_base.h"
#include "test_matrix_real.h"

TestMatrixReal::TestMatrixReal() {}


TestMatrixReal::~TestMatrixReal() {}


void TestMatrixReal::testConstructorAndDestructors() {
    Model::MatrixReal m1;
    QCOMPARE(m1.numberRows(), 0);
    QCOMPARE(m1.numberColumns(), 0);

    Model::MatrixReal m2(3, 3);
    QCOMPARE(m2.numberRows(), 3);
    QCOMPARE(m2.numberColumns(), 3);

    QCOMPARE(m2(1,1), Model::Real(0));
    QCOMPARE(m2(1,2), Model::Real(0));
    QCOMPARE(m2(1,3), Model::Real(0));
    QCOMPARE(m2(2,1), Model::Real(0));
    QCOMPARE(m2(2,2), Model::Real(0));
    QCOMPARE(m2(2,3), Model::Real(0));
    QCOMPARE(m2(3,1), Model::Real(0));
    QCOMPARE(m2(3,2), Model::Real(0));
    QCOMPARE(m2(3,3), Model::Real(0));

    m2.update(1, 1, Model::Real(1));
    m2.update(1, 2, Model::Real(2));
    m2.update(1, 3, Model::Real(3));
    m2.update(2, 1, Model::Real(4));
    m2.update(2, 2, Model::Real(5));
    m2.update(2, 3, Model::Real(6));
    m2.update(3, 1, Model::Real(7));
    m2.update(3, 2, Model::Real(8));
    m2.update(3, 3, Model::Real(9));

    const Model::Real matrixData[] = {
        Model::Real(1), Model::Real(2),
        Model::Real(3), Model::Real(4)
    };

    Model::MatrixReal m3(2, 2, matrixData);
    QCOMPARE(m3.numberRows(), 2);
    QCOMPARE(m3.numberColumns(), 2);

    QCOMPARE(m3(1,1), Model::Real(1));
    QCOMPARE(m3(1,2), Model::Real(3));
    QCOMPARE(m3(2,1), Model::Real(2));
    QCOMPARE(m3(2,2), Model::Real(4));

    Model::MatrixReal m4(m2);
    QCOMPARE(m4.numberRows(), 3);
    QCOMPARE(m4.numberColumns(), 3);

    QCOMPARE(m4(1,1), Model::Real(1));
    QCOMPARE(m4(1,2), Model::Real(2));
    QCOMPARE(m4(1,3), Model::Real(3));
    QCOMPARE(m4(2,1), Model::Real(4));
    QCOMPARE(m4(2,2), Model::Real(5));
    QCOMPARE(m4(2,3), Model::Real(6));
    QCOMPARE(m4(3,1), Model::Real(7));
    QCOMPARE(m4(3,2), Model::Real(8));
    QCOMPARE(m4(3,3), Model::Real(9));

    Model::MatrixInteger i = Model::MatrixInteger::build(
        2, 2,
        Model::Integer(1), Model::Integer(2),
        Model::Integer(3), Model::Integer(4)
    );

    Model::MatrixReal rc(2 * i.transpose());
    QCOMPARE(
        rc,
        Model::MatrixReal::build(
            2, 2,
            Model::Real(2), Model::Real(6),
            Model::Real(4), Model::Real(8)
        )
    );

    Model::MatrixBoolean b = Model::MatrixBoolean::build(
        2, 2,
        true, false,
        true, true
    );

    Model::MatrixReal ib(b.transpose());
    QCOMPARE(
        ib,
        Model::MatrixReal::build(
            2, 2,
            Model::Real(1), Model::Real(1),
            Model::Real(0), Model::Real(1)
        )
    );
}


void TestMatrixReal::testCoefficientValueType() {
    Model::MatrixReal m;
    QCOMPARE(m.coefficientValueType(), Model::ValueType::REAL);
}


void TestMatrixReal::testNumberRowsColumns() {
    Model::MatrixReal m1(2, 3);

    QCOMPARE(m1.numberRows(), 2);
    QCOMPARE(m1.numberColumns(), 3);
    QCOMPARE(m1.numberCoefficients(), 6);
}


void TestMatrixReal::testValueMethods() {
    Model::MatrixReal m1;

    bool success = m1.setValue(1, 1, Model::Integer(1));
    QVERIFY(success);

    Model::Variant v11 = m1.value(1, 1);
    QCOMPARE(v11.valueType(), Model::ValueType::REAL);
    QCOMPARE(v11, Model::Variant(Model::Integer(1)));

    success = m1.setValue(1, 2, Model::Real(1.5));
    QVERIFY(success);

    v11 = m1.value(1, 1);
    Model::Variant v12 = m1.value(1, 2);

    QCOMPARE(v11.valueType(), Model::ValueType::REAL);
    QCOMPARE(v12.valueType(), Model::ValueType::REAL);

    QCOMPARE(v11, Model::Variant(Model::Integer(1)));
    QCOMPARE(v12, Model::Variant(Model::Real(1.5)));

    success = m1.setValue(2, 1, Model::Real(1.75));
    QVERIFY(success);

    v11 = m1.value(1, 1);
    v12 = m1.value(1, 2);
    Model::Variant v21 = m1.value(2, 1);

    QCOMPARE(v11.valueType(), Model::ValueType::REAL);
    QCOMPARE(v12.valueType(), Model::ValueType::REAL);
    QCOMPARE(v21.valueType(), Model::ValueType::REAL);

    QCOMPARE(v11, Model::Variant(Model::Integer(1)));
    QCOMPARE(v12, Model::Variant(Model::Real(1.5)));
    QCOMPARE(v21, Model::Variant(Model::Real(1.75)));

    success = m1.setValue(4, Model::Boolean(true));
    QVERIFY(!success);

    success = m1.setValue(4, Model::Real(2.75));
    QVERIFY(success);

    QCOMPARE(m1.numberRows(), 2);
    QCOMPARE(m1.numberColumns(), 2);

    v11 = m1.value(1, 1);
    v12 = m1.value(1, 2);
    v21 = m1.value(2, 1);
    Model::Variant v22 = m1.value(2, 2);

    QCOMPARE(v11.valueType(), Model::ValueType::REAL);
    QCOMPARE(v12.valueType(), Model::ValueType::REAL);
    QCOMPARE(v21.valueType(), Model::ValueType::REAL);
    QCOMPARE(v22.valueType(), Model::ValueType::REAL);

    QCOMPARE(v11, Model::Variant(Model::Integer(1)));
    QCOMPARE(v12, Model::Variant(Model::Real(1.5)));
    QCOMPARE(v21, Model::Variant(Model::Real(1.75)));
    QCOMPARE(v22, Model::Variant(Model::Real(2.75)));

    QCOMPARE(m1.value(1), Model::Variant(Model::Integer(1)));
    QCOMPARE(m1.value(2), Model::Variant(Model::Real(1.5)));
    QCOMPARE(m1.value(3), Model::Variant(Model::Real(1.75)));
    QCOMPARE(m1.value(4), Model::Variant(Model::Real(2.75)));
}


void TestMatrixReal::testInternalAtMethods() {
    M::MatrixReal m(9, 9);
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
    QCOMPARE(m.at(i, rng), M::MatrixReal::build(1, 2, M::Integer(35), M::Integer(36)));
    QCOMPARE(m.at(i, set), M::MatrixReal::build(1, 2, M::Integer(32), M::Integer(34)));
    QCOMPARE(m.at(i, tuple), M::MatrixReal::build(1, 2, M::Integer(33), M::Integer(35)));
    QCOMPARE(m.at(i, mi), M::MatrixReal::build(1, 4, M::Integer(32), M::Integer(34), M::Integer(36), M::Integer(38)));
    QCOMPARE(m.at(i, mr), M::MatrixReal::build(1, 4, M::Integer(31), M::Integer(32), M::Integer(33), M::Integer(34)));
    QCOMPARE(m.at(i, mc), M::MatrixReal::build(1, 2, M::Integer(31), M::Integer(35)));

    QCOMPARE(m.at(r, i), M::Real(43));
    QCOMPARE(m.at(r, r), M::Real(44));
    QCOMPARE(m.at(r, c), M::Real(45));
    QCOMPARE(m.at(r, rng), M::MatrixReal::build(1, 2, M::Integer(45), M::Integer(46)));
    QCOMPARE(m.at(r, set), M::MatrixReal::build(1, 2, M::Integer(42), M::Integer(44)));
    QCOMPARE(m.at(r, tuple), M::MatrixReal::build(1, 2, M::Integer(43), M::Integer(45)));
    QCOMPARE(m.at(r, mi), M::MatrixReal::build(1, 4, M::Integer(42), M::Integer(44), M::Integer(46), M::Integer(48)));
    QCOMPARE(m.at(r, mr), M::MatrixReal::build(1, 4, M::Integer(41), M::Integer(42), M::Integer(43), M::Integer(44)));
    QCOMPARE(m.at(r, mc), M::MatrixReal::build(1, 2, M::Integer(41), M::Integer(45)));

    QCOMPARE(m.at(c, i), M::Real(53));
    QCOMPARE(m.at(c, r), M::Real(54));
    QCOMPARE(m.at(c, c), M::Real(55));
    QCOMPARE(m.at(c, rng), M::MatrixReal::build(1, 2, M::Integer(55), M::Integer(56)));
    QCOMPARE(m.at(c, set), M::MatrixReal::build(1, 2, M::Integer(52), M::Integer(54)));
    QCOMPARE(m.at(c, tuple), M::MatrixReal::build(1, 2, M::Integer(53), M::Integer(55)));
    QCOMPARE(m.at(c, mi), M::MatrixReal::build(1, 4, M::Integer(52), M::Integer(54), M::Integer(56), M::Integer(58)));
    QCOMPARE(m.at(c, mr), M::MatrixReal::build(1, 4, M::Integer(51), M::Integer(52), M::Integer(53), M::Integer(54)));
    QCOMPARE(m.at(c, mc), M::MatrixReal::build(1, 2, M::Integer(51), M::Integer(55)));

    QCOMPARE(m.at(rng, i), M::MatrixReal::build(2, 1, M::Real(53), M::Real(63)));
    QCOMPARE(m.at(rng, r), M::MatrixReal::build(2, 1, M::Real(54), M::Real(64)));
    QCOMPARE(m.at(rng, c), M::MatrixReal::build(2, 1, M::Real(55), M::Real(65)));
    QCOMPARE(
        m.at(rng, rng),
        M::MatrixReal::build(
            2, 2,
            M::Real(55), M::Real(65),
            M::Real(56), M::Real(66)
        )
    );
    QCOMPARE(
        m.at(rng, set),
        M::MatrixReal::build(
            2, 2,
            M::Real(52), M::Real(62),
            M::Real(54), M::Real(64)
        )
    );
    QCOMPARE(
        m.at(rng, tuple),
        M::MatrixReal::build(
            2, 2,
            M::Real(53), M::Real(63),
            M::Real(55), M::Real(65)
        )
    );
    QCOMPARE(
        m.at(rng, mi),
        M::MatrixReal::build(
            2, 4,
            M::Real(52), M::Real(62),
            M::Real(54), M::Real(64),
            M::Real(56), M::Real(66),
            M::Real(58), M::Real(68)
        )
    );
    QCOMPARE(
        m.at(rng, mr),
        M::MatrixReal::build(
            2, 4,
            M::Real(51), M::Real(61),
            M::Real(52), M::Real(62),
            M::Real(53), M::Real(63),
            M::Real(54), M::Real(64)
        )
    );
    QCOMPARE(
        m.at(rng, mc),
        M::MatrixReal::build(
            2, 2,
            M::Real(51), M::Real(61),
            M::Real(55), M::Real(65)
        )
    );

    QCOMPARE(m.at(set, i), M::MatrixReal::build(2, 1, M::Real(23), M::Real(43)));
    QCOMPARE(m.at(set, r), M::MatrixReal::build(2, 1, M::Real(24), M::Real(44)));
    QCOMPARE(m.at(set, c), M::MatrixReal::build(2, 1, M::Real(25), M::Real(45)));
    QCOMPARE(
        m.at(set, rng),
        M::MatrixReal::build(
            2, 2,
            M::Real(25), M::Real(45),
            M::Real(26), M::Real(46)
        )
    );
    QCOMPARE(
        m.at(set, set),
        M::MatrixReal::build(
            2, 2,
            M::Real(22), M::Real(42),
            M::Real(24), M::Real(44)
        )
    );
    QCOMPARE(
        m.at(set, tuple),
        M::MatrixReal::build(
            2, 2,
            M::Real(23), M::Real(43),
            M::Real(25), M::Real(45)
        )
    );
    QCOMPARE(
        m.at(set, mi),
        M::MatrixReal::build(
            2, 4,
            M::Real(22), M::Real(42),
            M::Real(24), M::Real(44),
            M::Real(26), M::Real(46),
            M::Real(28), M::Real(48)
        )
    );
    QCOMPARE(
        m.at(set, mr),
        M::MatrixReal::build(
            2, 4,
            M::Real(21), M::Real(41),
            M::Real(22), M::Real(42),
            M::Real(23), M::Real(43),
            M::Real(24), M::Real(44)
        )
    );
    QCOMPARE(
        m.at(set, mc),
        M::MatrixReal::build(
            2, 2,
            M::Real(21), M::Real(41),
            M::Real(25), M::Real(45)
        )
    );

    QCOMPARE(m.at(tuple, i), M::MatrixReal::build(2, 1, M::Real(33), M::Real(53)));
    QCOMPARE(m.at(tuple, r), M::MatrixReal::build(2, 1, M::Real(34), M::Real(54)));
    QCOMPARE(m.at(tuple, c), M::MatrixReal::build(2, 1, M::Real(35), M::Real(55)));
    QCOMPARE(
        m.at(tuple, rng),
        M::MatrixReal::build(
            2, 2,
            M::Real(35), M::Real(55),
            M::Real(36), M::Real(56)
        )
    );
    QCOMPARE(
        m.at(tuple, set),
        M::MatrixReal::build(
            2, 2,
            M::Real(32), M::Real(52),
            M::Real(34), M::Real(54)
        )
    );
    QCOMPARE(
        m.at(tuple, tuple),
        M::MatrixReal::build(
            2, 2,
            M::Real(33), M::Real(53),
            M::Real(35), M::Real(55)
        )
    );
    QCOMPARE(
        m.at(tuple, mi),
        M::MatrixReal::build(
            2, 4,
            M::Real(32), M::Real(52),
            M::Real(34), M::Real(54),
            M::Real(36), M::Real(56),
            M::Real(38), M::Real(58)
        )
    );
    QCOMPARE(
        m.at(tuple, mr),
        M::MatrixReal::build(
            2, 4,
            M::Real(31), M::Real(51),
            M::Real(32), M::Real(52),
            M::Real(33), M::Real(53),
            M::Real(34), M::Real(54)
        )
    );
    QCOMPARE(
        m.at(tuple, mc),
        M::MatrixReal::build(
            2, 2,
            M::Real(31), M::Real(51),
            M::Real(35), M::Real(55)
        )
    );

    QCOMPARE(m.at(mi, i), M::MatrixReal::build(4, 1, M::Real(23), M::Real(43), M::Real(63), M::Real(83)));
    QCOMPARE(m.at(mi, r), M::MatrixReal::build(4, 1, M::Real(24), M::Real(44), M::Real(64), M::Real(84)));
    QCOMPARE(m.at(mi, c), M::MatrixReal::build(4, 1, M::Real(25), M::Real(45), M::Real(65), M::Real(85)));
    QCOMPARE(
        m.at(mi, rng),
        M::MatrixReal::build(
            4, 2,
            M::Real(25), M::Real(45), M::Real(65), M::Real(85),
            M::Real(26), M::Real(46), M::Real(66), M::Real(86)
        )
    );
    QCOMPARE(
        m.at(mi, set),
        M::MatrixReal::build(
            4, 2,
            M::Real(22), M::Real(42), M::Real(62), M::Real(82),
            M::Real(24), M::Real(44), M::Real(64), M::Real(84)
        )
    );
    QCOMPARE(
        m.at(mi, tuple),
        M::MatrixReal::build(
            4, 2,
            M::Real(23), M::Real(43), M::Real(63), M::Real(83),
            M::Real(25), M::Real(45), M::Real(65), M::Real(85)
        )
    );
    QCOMPARE(
        m.at(mi, mi),
        M::MatrixReal::build(
            4, 4,
            M::Real(22), M::Real(42), M::Real(62), M::Real(82),
            M::Real(24), M::Real(44), M::Real(64), M::Real(84),
            M::Real(26), M::Real(46), M::Real(66), M::Real(86),
            M::Real(28), M::Real(48), M::Real(68), M::Real(88)
        )
    );
    QCOMPARE(
        m.at(mi, mr),
        M::MatrixReal::build(
            4, 4,
            M::Real(21), M::Real(41), M::Real(61), M::Real(81),
            M::Real(22), M::Real(42), M::Real(62), M::Real(82),
            M::Real(23), M::Real(43), M::Real(63), M::Real(83),
            M::Real(24), M::Real(44), M::Real(64), M::Real(84)
        )
    );
    QCOMPARE(
        m.at(mi, mc),
        M::MatrixReal::build(
            4, 2,
            M::Real(21), M::Real(41), M::Real(61), M::Real(81),
            M::Real(25), M::Real(45), M::Real(65), M::Real(85)
        )
    );

    QCOMPARE(m.at(mr, i), M::MatrixReal::build(4, 1, M::Real(13), M::Real(23), M::Real(33), M::Real(43)));
    QCOMPARE(m.at(mr, r), M::MatrixReal::build(4, 1, M::Real(14), M::Real(24), M::Real(34), M::Real(44)));
    QCOMPARE(m.at(mr, c), M::MatrixReal::build(4, 1, M::Real(15), M::Real(25), M::Real(35), M::Real(45)));
    QCOMPARE(
        m.at(mr, rng),
        M::MatrixReal::build(
            4, 2,
            M::Real(15), M::Real(25), M::Real(35), M::Real(45),
            M::Real(16), M::Real(26), M::Real(36), M::Real(46)
        )
    );
    QCOMPARE(
        m.at(mr, set),
        M::MatrixReal::build(
            4, 2,
            M::Real(12), M::Real(22), M::Real(32), M::Real(42),
            M::Real(14), M::Real(24), M::Real(34), M::Real(44)
        )
    );
    QCOMPARE(
        m.at(mr, tuple),
        M::MatrixReal::build(
            4, 2,
            M::Real(13), M::Real(23), M::Real(33), M::Real(43),
            M::Real(15), M::Real(25), M::Real(35), M::Real(45)
        )
    );
    QCOMPARE(
        m.at(mr, mi),
        M::MatrixReal::build(
            4, 4,
            M::Real(12), M::Real(22), M::Real(32), M::Real(42),
            M::Real(14), M::Real(24), M::Real(34), M::Real(44),
            M::Real(16), M::Real(26), M::Real(36), M::Real(46),
            M::Real(18), M::Real(28), M::Real(38), M::Real(48)
        )
    );
    QCOMPARE(
        m.at(mr, mr),
        M::MatrixReal::build(
            4, 4,
            M::Real(11), M::Real(21), M::Real(31), M::Real(41),
            M::Real(12), M::Real(22), M::Real(32), M::Real(42),
            M::Real(13), M::Real(23), M::Real(33), M::Real(43),
            M::Real(14), M::Real(24), M::Real(34), M::Real(44)
        )
    );
    QCOMPARE(
        m.at(mr, mc),
        M::MatrixReal::build(
            4, 2,
            M::Real(11), M::Real(21), M::Real(31), M::Real(41),
            M::Real(15), M::Real(25), M::Real(35), M::Real(45)
        )
    );

    QCOMPARE(m.at(mc, i), M::MatrixReal::build(2, 1, M::Real(13), M::Real(53)));
    QCOMPARE(m.at(mc, r), M::MatrixReal::build(2, 1, M::Real(14), M::Real(54)));
    QCOMPARE(m.at(mc, c), M::MatrixReal::build(2, 1, M::Real(15), M::Real(55)));
    QCOMPARE(
        m.at(mc, rng),
        M::MatrixReal::build(
            2, 2,
            M::Real(15), M::Real(55),
            M::Real(16), M::Real(56)
        )
    );
    QCOMPARE(
        m.at(mc, set),
        M::MatrixReal::build(
            2, 2,
            M::Real(12), M::Real(52),
            M::Real(14), M::Real(54)
        )
    );
    QCOMPARE(
        m.at(mc, tuple),
        M::MatrixReal::build(
            2, 2,
            M::Real(13), M::Real(53),
            M::Real(15), M::Real(55)
        )
    );
    QCOMPARE(
        m.at(mc, mi),
        M::MatrixReal::build(
            2, 4,
            M::Real(12), M::Real(52),
            M::Real(14), M::Real(54),
            M::Real(16), M::Real(56),
            M::Real(18), M::Real(58)
        )
    );
    QCOMPARE(
        m.at(mc, mr),
        M::MatrixReal::build(
            2, 4,
            M::Real(11), M::Real(51),
            M::Real(12), M::Real(52),
            M::Real(13), M::Real(53),
            M::Real(14), M::Real(54)
        )
    );
    QCOMPARE(
        m.at(mc, mc),
        M::MatrixReal::build(
            2, 2,
            M::Real(11), M::Real(51),
            M::Real(15), M::Real(55)
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


void TestMatrixReal::testResizeMethod() {
    Model::MatrixReal m(2, 2);
    QCOMPARE(m.numberRows(),    2);
    QCOMPARE(m.numberColumns(), 2);

    m.update(1,1, Model::Real(1));
    m.update(1,2, Model::Real(2));
    m.update(2,1, Model::Real(3));
    m.update(2,2, Model::Real(4));

    QCOMPARE(m(1,1), Model::Real(1));
    QCOMPARE(m(1,2), Model::Real(2));
    QCOMPARE(m(2,1), Model::Real(3));
    QCOMPARE(m(2,2), Model::Real(4));

    m.resize(3, 4);
    QCOMPARE(m.numberRows(),    3);
    QCOMPARE(m.numberColumns(), 4);

    QCOMPARE(m(1,1), Model::Real(1));
    QCOMPARE(m(1,2), Model::Real(2));
    QCOMPARE(m(1,3), Model::Real(0));
    QCOMPARE(m(1,4), Model::Real(0));
    QCOMPARE(m(2,1), Model::Real(3));
    QCOMPARE(m(2,2), Model::Real(4));
    QCOMPARE(m(3,3), Model::Real(0));
    QCOMPARE(m(3,4), Model::Real(0));
    QCOMPARE(m(3,1), Model::Real(0));
    QCOMPARE(m(3,2), Model::Real(0));
    QCOMPARE(m(3,3), Model::Real(0));
    QCOMPARE(m(3,4), Model::Real(0));
}


void TestMatrixReal::testCoefficientAccessorMethods() {
    Model::MatrixReal m1;
    QCOMPARE(m1.numberRows(), 0);
    QCOMPARE(m1.numberColumns(), 0);

    m1.update(1,1, Model::Real(1));
    QCOMPARE(m1.numberRows(), 1);
    QCOMPARE(m1.numberColumns(), 1);

    m1.update(1,2, Model::Real(2));
    QCOMPARE(m1.numberRows(), 1);
    QCOMPARE(m1.numberColumns(), 2);

    m1.update(2,1, Model::Real(4));
    QCOMPARE(m1.numberRows(), 2);
    QCOMPARE(m1.numberColumns(), 2);

    m1.update(2,2, Model::Real(5));
    QCOMPARE(m1.numberRows(), 2);
    QCOMPARE(m1.numberColumns(), 2);

    m1.update(1,3, Model::Real(3));
    QCOMPARE(m1.numberRows(), 2);
    QCOMPARE(m1.numberColumns(), 3);

    m1.update(2,3, Model::Real(6));
    QCOMPARE(m1.numberRows(), 2);
    QCOMPARE(m1.numberColumns(), 3);

    m1.update(3,1, Model::Real(7));
    QCOMPARE(m1.numberRows(), 3);
    QCOMPARE(m1.numberColumns(), 3);

    m1.update(3,2, Model::Real(8));
    m1.update(3,3, Model::Real(9));
    QCOMPARE(m1.numberRows(), 3);
    QCOMPARE(m1.numberColumns(), 3);

    QCOMPARE(m1(1,1), Model::Real(1));
    QCOMPARE(m1(1,2), Model::Real(2));
    QCOMPARE(m1(1,3), Model::Real(3));
    QCOMPARE(m1(2,1), Model::Real(4));
    QCOMPARE(m1(2,2), Model::Real(5));
    QCOMPARE(m1(2,3), Model::Real(6));
    QCOMPARE(m1(3,1), Model::Real(7));
    QCOMPARE(m1(3,2), Model::Real(8));
    QCOMPARE(m1(3,3), Model::Real(9));

    Model::MatrixReal m2;
    m2.update(1, 1);
    m2.update(2, 2);
    m2.update(3, 3);

    QCOMPARE(m2.numberRows(), 3);
    QCOMPARE(m2.numberColumns(), 1);

    QCOMPARE(m2(1,1), Model::Real(1));
    QCOMPARE(m2(2,1), Model::Real(2));
    QCOMPARE(m2(3,1), Model::Real(3));

    Model::MatrixReal m3;
    m3.update(1, 1, 1);
    m3.update(1, 2, 2);
    m3.update(3, 3);
    m3.update(4, 4);

    QCOMPARE(m3.numberRows(), 1);
    QCOMPARE(m3.numberColumns(), 4);

    QCOMPARE(m3(1,1), Model::Real(1));
    QCOMPARE(m3(1,2), Model::Real(2));
    QCOMPARE(m3(1,3), Model::Real(3));
    QCOMPARE(m3(1,4), Model::Real(4));
}


void TestMatrixReal::testSliceMethods() {
    Model::MatrixReal m = Model::MatrixReal::build(
        5,
        5,
        Model::Real( 1), Model::Real( 6), Model::Real(11), Model::Real(16), Model::Real(21),
        Model::Real( 2), Model::Real( 7), Model::Real(12), Model::Real(17), Model::Real(22),
        Model::Real( 3), Model::Real( 8), Model::Real(13), Model::Real(18), Model::Real(23),
        Model::Real( 4), Model::Real( 9), Model::Real(14), Model::Real(19), Model::Real(24),
        Model::Real( 5), Model::Real(10), Model::Real(15), Model::Real(20), Model::Real(25)
    );

    Model::MatrixReal m1 = m.at(Model::Range(Model::Integer(2),Model::Integer(4)), 2);

    QCOMPARE(m1.numberRows(), 3);
    QCOMPARE(m1.numberColumns(), 1);
    QCOMPARE(m1(1, 1), Model::Real(7));
    QCOMPARE(m1(2, 1), Model::Real(12));
    QCOMPARE(m1(3, 1), Model::Real(17));

    Model::MatrixReal m2 = m.at(3, Model::Range(Model::Integer(2),Model::Integer(3)));

    QCOMPARE(m2.numberRows(), 1);
    QCOMPARE(m2.numberColumns(), 2);
    QCOMPARE(m2(1, 1), Model::Real(12));
    QCOMPARE(m2(1, 2), Model::Real(13));

    Model::MatrixReal m3 = m.at(
        Model::Range(Model::Integer(1), Model::Integer(3), Model::Integer(5)),
        Model::Range(Model::Integer(2),Model::Integer(4))
    );

    QCOMPARE(m3.numberRows(), 3);
    QCOMPARE(m3.numberColumns(), 3);

    QCOMPARE(m3(1, 1), Model::Real(2));
    QCOMPARE(m3(1, 2), Model::Real(3));
    QCOMPARE(m3(1, 3), Model::Real(4));
    QCOMPARE(m3(2, 1), Model::Real(12));
    QCOMPARE(m3(2, 2), Model::Real(13));
    QCOMPARE(m3(2, 3), Model::Real(14));
    QCOMPARE(m3(3, 1), Model::Real(22));
    QCOMPARE(m3(3, 2), Model::Real(23));
    QCOMPARE(m3(3, 3), Model::Real(24));
}


void TestMatrixReal::testCombineMethods() {
    Model::MatrixReal m1 = Model::MatrixReal::build(
        3, 2,
        Model::Real(11), Model::Real(13), Model::Real(15),
        Model::Real(12), Model::Real(14), Model::Real(16)
    );

    Model::MatrixReal m2 = Model::MatrixReal::build(
        2, 3,
        Model::Real(21), Model::Real(24),
        Model::Real(22), Model::Real(25),
        Model::Real(23), Model::Real(26)
    );

    Model::MatrixReal m3 = m1.combineLeftToRight(m2);
    QCOMPARE(
        m3,
        Model::MatrixReal::build(
            3, 5,
            Model::Real(11), Model::Real(13), Model::Real(15),
            Model::Real(12), Model::Real(14), Model::Real(16),
            Model::Real(21), Model::Real(24), Model::Real( 0),
            Model::Real(22), Model::Real(25), Model::Real( 0),
            Model::Real(23), Model::Real(26), Model::Real( 0)
        )
    );

    Model::MatrixReal m4 = m2.combineLeftToRight(m1);
    QCOMPARE(
        m4,
        Model::MatrixReal::build(
            3, 5,
            Model::Real(21), Model::Real(24), Model::Real( 0),
            Model::Real(22), Model::Real(25), Model::Real( 0),
            Model::Real(23), Model::Real(26), Model::Real( 0),
            Model::Real(11), Model::Real(13), Model::Real(15),
            Model::Real(12), Model::Real(14), Model::Real(16)
        )
    );

    Model::MatrixReal m5 = m1.combineTopToBottom(m2);
    QCOMPARE(
        m5,
        Model::MatrixReal::build(
             5, 3,
             Model::Real(11), Model::Real(13), Model::Real(15), Model::Real(21), Model::Real(24),
             Model::Real(12), Model::Real(14), Model::Real(16), Model::Real(22), Model::Real(25),
             Model::Real( 0), Model::Real( 0), Model::Real( 0), Model::Real(23), Model::Real(26)
        )
    );

    Model::MatrixReal m6 = m2.combineTopToBottom(m1);
    QCOMPARE(
        m6,
        Model::MatrixReal::build(
             5, 3,
             Model::Real(21), Model::Real(24), Model::Real(11), Model::Real(13), Model::Real(15),
             Model::Real(22), Model::Real(25), Model::Real(12), Model::Real(14), Model::Real(16),
             Model::Real(23), Model::Real(26), Model::Real( 0), Model::Real( 0), Model::Real( 0)
        )
    );
}


void TestMatrixReal::testReverseMethods() {
    Model::MatrixReal m = Model::MatrixReal::build(
        4, 4,
        Model::Real(11), Model::Real(21), Model::Real(31), Model::Real(41),
        Model::Real(12), Model::Real(22), Model::Real(32), Model::Real(42),
        Model::Real(13), Model::Real(23), Model::Real(33), Model::Real(43),
        Model::Real(14), Model::Real(24), Model::Real(34), Model::Real(44)
    );

    Model::MatrixReal cr = m.columnReverse();
    QCOMPARE(
        cr,
        Model::MatrixReal::build(
            4, 4,
            Model::Real(14), Model::Real(24), Model::Real(34), Model::Real(44),
            Model::Real(13), Model::Real(23), Model::Real(33), Model::Real(43),
            Model::Real(12), Model::Real(22), Model::Real(32), Model::Real(42),
            Model::Real(11), Model::Real(21), Model::Real(31), Model::Real(41)
        )
    );

    Model::MatrixReal rr = m.rowReverse();
    QCOMPARE(
        rr,
        Model::MatrixReal::build(
            4, 4,
            Model::Real(41), Model::Real(31), Model::Real(21), Model::Real(11),
            Model::Real(42), Model::Real(32), Model::Real(22), Model::Real(12),
            Model::Real(43), Model::Real(33), Model::Real(23), Model::Real(13),
            Model::Real(44), Model::Real(34), Model::Real(24), Model::Real(14)
        )
    );
}


void TestMatrixReal::testIdentity() {
    Model::MatrixReal i1 = Model::MatrixReal::identity(10, 12);

    QCOMPARE(i1.numberRows(), 10);
    QCOMPARE(i1.numberColumns(), 12);

    for (unsigned row=1 ; row<=10 ; ++row) {
        for (unsigned column=1 ; column<=12 ; ++column) {
            Model::Real expected = (row == column) ? Model::Real(1) : Model::Real(0);
            Model::Real measured = i1(row, column);

            QCOMPARE(expected, measured);
        }
    }

    Model::MatrixReal i2 = Model::MatrixReal::identity(10);

    QCOMPARE(i2.numberRows(), 10);
    QCOMPARE(i2.numberColumns(), 10);

    for (unsigned row=1 ; row<=10 ; ++row) {
        for (unsigned column=1 ; column<=10 ; ++column) {
            Model::Real expected = (row == column) ? Model::Real(1) : Model::Real(0);
            Model::Real measured = i2(row, column);

            QCOMPARE(expected, measured);
        }
    }
}


void TestMatrixReal::testZero() {
    Model::MatrixReal i1 = Model::MatrixReal::zero(10, 12);

    QCOMPARE(i1.numberRows(), 10);
    QCOMPARE(i1.numberColumns(), 12);

    for (unsigned row=1 ; row<=10 ; ++row) {
        for (unsigned column=1 ; column<=12 ; ++column) {
            QCOMPARE(i1(row, column), Model::Real(0));
        }
    }

    Model::MatrixReal i2 = Model::MatrixReal::zero(10);

    QCOMPARE(i2.numberRows(), 10);
    QCOMPARE(i2.numberColumns(), 10);

    for (unsigned row=1 ; row<=10 ; ++row) {
        for (unsigned column=1 ; column<=10 ; ++column) {
            QCOMPARE(i2(row, column), Model::Real(0));
        }
    }
}


void TestMatrixReal::testOnes() {
    for (M::Integer numberRows=1 ; numberRows<=32 ; ++numberRows) {
        for (M::Integer numberColumns=1 ; numberColumns<=32 ; ++numberColumns) {
            Model::MatrixReal i1 = Model::MatrixReal::ones(numberRows, numberColumns);

            QCOMPARE(i1.numberRows(), numberRows);
            QCOMPARE(i1.numberColumns(), numberColumns);

            for (M::Integer row=1 ; row<=numberRows ; ++row) {
                for (M::Integer column=1 ; column<=numberColumns ; ++column) {
                    QCOMPARE(i1(row, column), 1.0);
                }
            }
        }

        Model::MatrixReal i2 = Model::MatrixReal::ones(numberRows);

        QCOMPARE(i2.numberRows(), numberRows);
        QCOMPARE(i2.numberColumns(), numberRows);

        for (unsigned row=1 ; row<=numberRows ; ++row) {
            for (unsigned column=1 ; column<=numberRows ; ++column) {
                QCOMPARE(i2(row, column), 1.0);
            }
        }
    }
}


void TestMatrixReal::testDiagnonalEntries() {
    Model::MatrixReal m = Model::MatrixReal::build(
        4, 4,
        Model::Real( 1), Model::Real( 5), Model::Real( 9), Model::Real(13),
        Model::Real( 2), Model::Real( 6), Model::Real(10), Model::Real(14),
        Model::Real( 3), Model::Real( 7), Model::Real(11), Model::Real(15),
        Model::Real( 4), Model::Real( 8), Model::Real(12), Model::Real(16)
    );

    Model::MatrixReal d = m.diagonalEntries();
    QCOMPARE(d.numberRows(), 4);
    QCOMPARE(d.numberColumns(), 1);

    QCOMPARE(d(1), Model::Real( 1));
    QCOMPARE(d(2), Model::Real( 6));
    QCOMPARE(d(3), Model::Real(11));
    QCOMPARE(d(4), Model::Real(16));
}


void TestMatrixReal::testDiagnonal() {
    Model::MatrixReal d = Model::MatrixReal::build(
        4, 1,
        Model::Real(1), Model::Real(2), Model::Real(3), Model::Real(4)
    );

    Model::MatrixReal m = d.diagonal();

    QCOMPARE(m.numberRows(), 4);
    QCOMPARE(m.numberColumns(), 4);

    for (unsigned row=1 ; row<=4 ; ++row) {
        for (unsigned column=1 ; column<=4 ; ++column) {
            Model::Real expected = (row == column) ? Model::Real(row) : Model::Real(0);
            Model::Real measured = m(row, column);
            QCOMPARE(measured, expected);
        }
    }
}


void TestMatrixReal::testMatrixTypeMethods() {
    Model::MatrixReal rectangular = Model::MatrixReal::build(
        3, 2,
        Model::Real(1), Model::Real(3), Model::Real(5),
        Model::Real(2), Model::Real(4), Model::Real(6)
    );

    Model::MatrixReal square = Model::MatrixReal::build(
        3, 3,
        Model::Real(1), Model::Real(4), Model::Real(7),
        Model::Real(2), Model::Real(5), Model::Real(8),
        Model::Real(3), Model::Real(6), Model::Real(9)
    );

    Model::MatrixReal symmetric = Model::MatrixReal::build(
        3, 3,
        Model::Real(1), Model::Real(2), Model::Real(3),
        Model::Real(2), Model::Real(4), Model::Real(5),
        Model::Real(3), Model::Real(5), Model::Real(6)
    );

    Model::MatrixReal skewSymmetric = Model::MatrixReal::build(
        3, 3,
        Model::Real(0), Model::Real(-1), Model::Real(-2),
        Model::Real(1), Model::Real( 0), Model::Real(-3),
        Model::Real(2), Model::Real( 3), Model::Real( 0)
    );

    // Matrix was shamelessly lifted from Wikipedia
    // https://en.wikipedia.org/wiki/Normal_matrix

    Model::MatrixReal normal = Model::MatrixReal::build(
        3, 3,
        Model::Real(1), Model::Real(0), Model::Real(1),
        Model::Real(1), Model::Real(1), Model::Real(0),
        Model::Real(0), Model::Real(1), Model::Real(1)
    );

    QCOMPARE(rectangular.isSquare(), false);
    QCOMPARE(rectangular.isSymmetric(), false);
    QCOMPARE(rectangular.isHermitian(), false);
    QCOMPARE(rectangular.isSkewSymmetric(), false);
    QCOMPARE(rectangular.isSkewHermitian(), false);
    QCOMPARE(rectangular.isNormal(), false);

    QCOMPARE(square.isSquare(), true);
    QCOMPARE(square.isSymmetric(), false);
    QCOMPARE(square.isHermitian(), false);
    QCOMPARE(square.isSkewSymmetric(), false);
    QCOMPARE(square.isSkewHermitian(), false);
    QCOMPARE(square.isNormal(), false);

    QCOMPARE(symmetric.isSquare(), true);
    QCOMPARE(symmetric.isSymmetric(), true);
    QCOMPARE(symmetric.isHermitian(), true);
    QCOMPARE(symmetric.isSkewSymmetric(), false);
    QCOMPARE(symmetric.isSkewHermitian(), false);
    QCOMPARE(symmetric.isNormal(), true); // All symmetric matrices are normal

    QCOMPARE(skewSymmetric.isSquare(), true);
    QCOMPARE(skewSymmetric.isSymmetric(), false);
    QCOMPARE(skewSymmetric.isHermitian(), false);
    QCOMPARE(skewSymmetric.isSkewSymmetric(), true);
    QCOMPARE(skewSymmetric.isSkewHermitian(), true);
    QCOMPARE(skewSymmetric.isNormal(), true); // All skew-symmetric matrices are normal

    QCOMPARE(normal.isSquare(), true);
    QCOMPARE(normal.isSymmetric(), false);
    QCOMPARE(normal.isHermitian(), false);
    QCOMPARE(normal.isSkewSymmetric(), false);
    QCOMPARE(normal.isSkewHermitian(), false);
    QCOMPARE(normal.isNormal(), true);
}


void TestMatrixReal::testDeterminant1() {
    // We noted a simple test produced an incorrect determinant value (+2 rather than -2).  This test exists to verify
    // that the issue is resolved.

    Model::MatrixReal matrix = Model::MatrixReal::build(
        2, 2,
        1.0, 3.0,
        2.0, 4.0
    );

    Model::Real d = matrix.determinant();
    QCOMPARE(d, -2.0);
}


void TestMatrixReal::testDeterminant2() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1, +1);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);
    Model::Real                                 maximumError = 1E-11;
    Model::Real                                 maximumRelativeError = 0;

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned numberRowColumns = dimensionDistribution(rng);
        Model::MatrixReal m(numberRowColumns, numberRowColumns);

        for (unsigned long column=1 ; column<=numberRowColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRowColumns ; ++row) {
                Model::Real v = coefficientDistribution(rng);
                m.update(row, column, v);
            }
        }

        Model::Real measured = m.determinant();

        // Calculate the determinate using the upper trangular matrix from an LU decomposition.  Note that this is sort
        // of cheating as we use the same underlying function to calculate the determinant; however, we do test that
        // function below based on our ability to reconstruct the input matrix so this test should, in fact, be
        // reasonable.

        Model::Tuple      pluResult   = m.plu();
        Model::MatrixReal p           = pluResult(1);
        Model::MatrixReal l           = pluResult(2);
        Model::MatrixReal u           = pluResult(3);
        Model::Boolean    notSingular = pluResult(4);

        if (notSingular) {
            unsigned long numberDiagnonals = std::min(u.numberRows(), u.numberColumns());
            Model::Real expected(1.0);
            for (unsigned index=1 ; index<=numberDiagnonals ; ++index) {
                expected *= u(index, index);
            }

            if (std::signbit(measured) != std::signbit(expected)) {
                // A bit of a cheat here; however, we check the signs in testDeterminant1.
                expected = -expected;
            }

            Model::Real difference    = expected - measured;
            Model::Real relativeError = std::abs(difference / expected);

            if (relativeError >= maximumError) {
                dumpMatrix(m);
                QCOMPARE(relativeError < maximumError, true);
            }

            if (relativeError > maximumRelativeError) {
                maximumRelativeError = relativeError;
            }
        }
    }

    qDebug() << "maximumRelativeError = " << maximumRelativeError;

    // Check the determinant of a singular matrix.
    Model::MatrixReal m1 = Model::MatrixReal::build(
        3, 3,
        Model::Real(2), Model::Real(2), Model::Real(2),
        Model::Real(2), Model::Real(2), Model::Real(2),
        Model::Real(2), Model::Real(2), Model::Real(2)
    );

    QCOMPARE(m1.determinant(), Model::Real(0));

    // Check for handling of non-square matricies.
    Model::MatrixReal m2(10, 11);

    bool caughtException = false;
    bool exceptionValid  = false;
    try {
        (void) m2.determinant(); // should throw an exception
    } catch (const Model::InvalidMatrixDimensions& e) {
        caughtException = true;
        exceptionValid  = (e.numberRows() == 10 && e.numberColumns() == 11);
    } catch (...) {
        caughtException = true;
        exceptionValid  = false;
    }

    QCOMPARE(caughtException, true);
    QCOMPARE(exceptionValid, true);
}


void TestMatrixReal::testTranspose() {
    Model::MatrixReal m = Model::MatrixReal::build(
        2, 3,
        Model::Real(1), Model::Real(4),
        Model::Real(2), Model::Real(5),
        Model::Real(3), Model::Real(6)
    );

    Model::MatrixReal t1 = m.transpose();
    QCOMPARE(t1.numberRows(), 3);
    QCOMPARE(t1.numberColumns(), 2);

    QCOMPARE(
        t1,
        Model::MatrixReal::build(
            3, 2,
            Model::Real(1), Model::Real(2), Model::Real(3),
            Model::Real(4), Model::Real(5), Model::Real(6)
        )
    );

    Model::MatrixReal t2 = m.transpose();
    Model::MatrixReal t3 = 2.0 * t2;
    QCOMPARE(
        t3,
        Model::MatrixReal::build(
            3, 2,
            Model::Real( 2), Model::Real( 4), Model::Real( 6),
            Model::Real( 8), Model::Real(10), Model::Real(12)
        )
    );
}


void TestMatrixReal::testComplexConjugate() {
    Model::MatrixReal m = Model::MatrixReal::build(
        2, 3,
        Model::Real(1), Model::Real(4),
        Model::Real(2), Model::Real(5),
        Model::Real(3), Model::Real(6)
    );

    Model::MatrixReal t1 = m.conj();
    QCOMPARE(t1.numberRows(), 2);
    QCOMPARE(t1.numberColumns(), 3);

    QCOMPARE(
        t1,
        Model::MatrixReal::build(
            2, 3,
            Model::Real(1), Model::Real(4),
            Model::Real(2), Model::Real(5),
            Model::Real(3), Model::Real(6)
        )
    );
}


void TestMatrixReal::testAdjoint() {
    Model::MatrixReal m = Model::MatrixReal::build(
        2, 3,
        Model::Real(1), Model::Real(4),
        Model::Real(2), Model::Real(5),
        Model::Real(3), Model::Real(6)
    );

    Model::MatrixReal t1 = m.adjoint();
    QCOMPARE(t1.numberRows(), 3);
    QCOMPARE(t1.numberColumns(), 2);

    QCOMPARE(
        t1,
        Model::MatrixReal::build(
            3, 2,
            Model::Real(1), Model::Real(2), Model::Real(3),
            Model::Real(4), Model::Real(5), Model::Real(6)
        )
    );

    Model::MatrixReal t2 = m.transpose();
    Model::MatrixReal t3 = 2.0 * t2;
    QCOMPARE(
        t3,
        Model::MatrixReal::build(
            3, 2,
            Model::Real( 2), Model::Real( 4), Model::Real( 6),
            Model::Real( 8), Model::Real(10), Model::Real(12)
        )
    );
}


void TestMatrixReal::testInverse() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E6, +1E6);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);
    Model::Real                                 maximumAllowedError  = 5E-9;
    Model::Real                                 maximumMeasuredError = 0;

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned numberRowColumns = dimensionDistribution(rng);
        Model::MatrixReal m(numberRowColumns, numberRowColumns);

        for (unsigned long column=1 ; column<=numberRowColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRowColumns ; ++row) {
                Model::Real v = coefficientDistribution(rng);
                m.update(row, column, v);
            }
        }

        bool isSingular = false;
        Model::MatrixReal inverse;
        try {
            inverse = m.inverse();
        } catch (Model::MatrixIsSingular&) {
            isSingular = true;
        }

        if (!isSingular) {
            QCOMPARE(inverse.numberRows(), numberRowColumns);
            QCOMPARE(inverse.numberColumns(), numberRowColumns);

            Model::MatrixReal measuredIdentityMatrix = m * inverse;

            for (unsigned row=1 ; row<=numberRowColumns ; ++row) {
                for (unsigned column=1 ; column<=numberRowColumns ; ++column) {
                    Model::Real expectedValue = row == column ? Model::Real(1) : Model::Real(0);
                    Model::Real measuredValue = measuredIdentityMatrix(row, column);

                    Model::Real error = std::abs(expectedValue - measuredValue);

                    if (error >= maximumAllowedError) {
                        QCOMPARE(error < maximumAllowedError, true);
                    }

                    if (error > maximumMeasuredError) {
                        maximumMeasuredError = error;
                    }
                }
            }
        }
    }
}


void TestMatrixReal::testPLU() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E6, +1E6);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);
    Model::Real                                 maximumAllowedRelativeError = 5E-7;
    Model::Real                                 maximumRelativeError        = 0;
    Model::Real                                 maximumAllowedAbsoluteError = 1E-7;
    Model::Real                                 maximumAbsoluteError        = 0;

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned numberRowColumns = dimensionDistribution(rng);
        Model::MatrixReal m(numberRowColumns, numberRowColumns);

        for (unsigned long column=1 ; column<=numberRowColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRowColumns ; ++row) {
                if (dimensionDistribution(rng) < 100) {
                    m.update(row, column, 0); // Slip in a few zero values to force pivoting
                } else {
                    Model::Real v = coefficientDistribution(rng);
                    m.update(row, column, v);
                }
            }
        }

        Model::Tuple pluResult = m.plu();
        QCOMPARE(pluResult.size(), 4);

        Model::MatrixReal p           = pluResult(1);
        Model::MatrixReal l           = pluResult(2);
        Model::MatrixReal u           = pluResult(3);
        Model::Boolean    notSingular = pluResult(4);

        if (notSingular) {
            QCOMPARE(p.numberRows(), numberRowColumns);
            QCOMPARE(p.numberColumns(), numberRowColumns);

            QCOMPARE(l.numberRows(), numberRowColumns);
            QCOMPARE(l.numberColumns(), numberRowColumns);

            QCOMPARE(u.numberRows(), numberRowColumns);
            QCOMPARE(u.numberColumns(), numberRowColumns);

            Model::MatrixReal plu = p * l * u;

            QCOMPARE(plu.numberRows(), numberRowColumns);
            QCOMPARE(plu.numberColumns(), numberRowColumns);

            for (unsigned column=1 ; column<=numberRowColumns ; ++column) {
                for (unsigned row=1 ; row<=numberRowColumns ; ++row) {
                    Model::Real measured   = plu(row, column);
                    Model::Real expected   = m(row, column);
                    Model::Real difference = expected - measured;

                    if (expected != Model::Real(0)) {
                        Model::Real relativeError = std::abs(difference / expected);

                        if (relativeError >= maximumAllowedRelativeError) {
                            QCOMPARE(relativeError < maximumAllowedRelativeError, true);
                        }

                        if (relativeError > maximumRelativeError) {
                            maximumRelativeError = relativeError;
                        }
                    } else {
                        Model::Real absoluteError = std::abs(difference);
                        if (absoluteError >= maximumAllowedAbsoluteError) {
                            QCOMPARE(absoluteError < maximumAllowedAbsoluteError, true);
                        }

                        if (absoluteError > maximumAbsoluteError) {
                            maximumAbsoluteError = absoluteError;
                        }
                    }
                }
            }
        }
    }
}


void TestMatrixReal::testSVD() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E6, +1E6);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);
    Model::Real                                 maximumAllowedRelativeError = 1E-6;
    Model::Real                                 maximumRelativeError        = 0;
    Model::Real                                 maximumAllowedAbsoluteError = 1E-16;
    Model::Real                                 maximumAbsoluteError        = 0;

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned numberRows    = dimensionDistribution(rng);
        unsigned numberColumns = dimensionDistribution(rng);
        Model::MatrixReal m(numberRows, numberColumns);

        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                Model::Real v = coefficientDistribution(rng);
                m.update(row, column, v);
            }
        }

        Model::Tuple svdResult = m.svd();
        QCOMPARE(svdResult.size(), 4);

        Model::MatrixReal u       = svdResult(1);
        Model::MatrixReal s       = svdResult(2);
        Model::MatrixReal vh      = svdResult(3);
        Model::Boolean       success = svdResult(4);

        if (success) {
            QCOMPARE(u.numberRows(), numberRows);
            QCOMPARE(u.numberColumns(), numberRows);

            QCOMPARE(s.numberRows(), numberRows);
            QCOMPARE(s.numberColumns(), numberColumns);

            QCOMPARE(vh.numberRows(), numberColumns);
            QCOMPARE(vh.numberColumns(), numberColumns);

            Model::MatrixReal measuredMatrix = u * s * vh;

            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                for (unsigned long column=1 ; column<=numberColumns ; ++column) {
                    Model::Real expected = m(row, column);
                    Model::Real measured = measuredMatrix(row, column);

                    if (expected != Model::Real(0)) {
                        Model::Real difference    = expected - measured;
                        Model::Real relativeError = std::abs(difference / expected);

                        if (relativeError > maximumAllowedRelativeError) {
                            QCOMPARE(relativeError <= maximumAllowedRelativeError, true);
                        }

                        if (relativeError > maximumRelativeError) {
                            maximumRelativeError = relativeError;
                        }
                    } else {
                        Model::Real absoluteError = std::abs(measured);

                        if (absoluteError > maximumAllowedAbsoluteError) {
                            QCOMPARE(absoluteError <= maximumAllowedAbsoluteError, true);
                        }

                        if (absoluteError > maximumAbsoluteError) {
                            maximumAbsoluteError = absoluteError;
                        }
                    }
                }
            }
        }
    }
}


void TestMatrixReal::testQR() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E6, +1E6);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);
    Model::Real                                 maximumAllowedRelativeError = 1E-2;
    Model::Real                                 maximumRelativeError        = 0;
    Model::Real                                 maximumAllowedAbsoluteError = 1E-15;
    Model::Real                                 maximumAbsoluteError        = 0;
    unsigned                                    numberFailures              = 0;

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned long numberRows;
        unsigned long numberColumns;
        do {
            numberRows    = dimensionDistribution(rng);
            numberColumns = dimensionDistribution(rng);
        } while (numberRows < numberColumns);

        Model::MatrixReal m(numberRows, numberColumns);
        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                Model::Real v = coefficientDistribution(rng);
                m.update(row, column, v);
            }
        }

        Model::Tuple qrResult = m.qr();
        QCOMPARE(qrResult.size(), 3);

        Model::MatrixReal q       = qrResult(1);
        Model::MatrixReal r       = qrResult(2);
        Model::Boolean    success = qrResult(3);

        if (success) {
            QCOMPARE(static_cast<unsigned long>(q.numberRows()), numberRows);
            QCOMPARE(static_cast<unsigned long>(q.numberColumns()), numberRows);

            QCOMPARE(static_cast<unsigned long>(r.numberRows()), numberRows);
            QCOMPARE(static_cast<unsigned long>(r.numberColumns()), numberColumns);

            Model::MatrixReal measuredMatrix = q * r;
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                for (unsigned long column=1 ; column<=numberColumns ; ++column) {
                    Model::Real expected = m(row, column);
                    Model::Real measured = measuredMatrix(row, column);

                    if (expected != Model::Real(0)) {
                        Model::Real difference    = expected - measured;
                        Model::Real relativeError = std::abs(difference / expected);

                        if (relativeError > maximumAllowedRelativeError) {
                            QCOMPARE(relativeError <= maximumAllowedRelativeError, true);
                        }

                        if (relativeError > maximumRelativeError) {
                            maximumRelativeError = relativeError;
                        }
                    } else {
                        Model::Real absoluteError = std::abs(measured);

                        if (absoluteError > maximumAllowedAbsoluteError) {
                            QCOMPARE(absoluteError <= maximumAllowedAbsoluteError, true);
                        }

                        if (absoluteError > maximumAbsoluteError) {
                            maximumAbsoluteError = absoluteError;
                        }
                    }
                }
            }
        } else {
            ++numberFailures;
        }
    }

    Model::Real failureRate = (1.0 * numberFailures) / numberIterations;
    qDebug() << "maximum relative error = " << maximumRelativeError;
    qDebug() << "failure rate           = " << failureRate;
}


void TestMatrixReal::testLQ() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E6, +1E6);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);
    Model::Real                                 maximumAllowedRelativeError = 1E-2;
    Model::Real                                 maximumRelativeError        = 0;
    Model::Real                                 maximumAllowedAbsoluteError = 1E-15;
    Model::Real                                 maximumAbsoluteError        = 0;
    unsigned                                    numberFailures              = 0;

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned long numberRows;
        unsigned long numberColumns;
        do {
            numberRows    = dimensionDistribution(rng);
            numberColumns = dimensionDistribution(rng);
        } while (numberRows > numberColumns);

        Model::MatrixReal m(numberRows, numberColumns);
        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                Model::Real v = coefficientDistribution(rng);
                m.update(row, column, v);
            }
        }

        Model::Tuple lqResult = m.lq();
        QCOMPARE(lqResult.size(), 3);

        Model::MatrixReal l       = lqResult(1);
        Model::MatrixReal q       = lqResult(2);
        Model::Boolean       success = lqResult(3);

        if (success) {
            QCOMPARE(static_cast<unsigned long>(l.numberRows()), numberRows);
            QCOMPARE(static_cast<unsigned long>(l.numberColumns()), numberColumns);

            QCOMPARE(static_cast<unsigned long>(q.numberRows()), numberColumns);
            QCOMPARE(static_cast<unsigned long>(q.numberColumns()), numberColumns);

            Model::MatrixReal measuredMatrix = l * q;
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                for (unsigned long column=1 ; column<=numberColumns ; ++column) {
                    Model::Real expected = m(row, column);
                    Model::Real measured = measuredMatrix(row, column);

                    if (expected != Model::Real(0)) {
                        Model::Real difference    = expected - measured;
                        Model::Real relativeError = std::abs(difference / expected);

                        if (relativeError > maximumAllowedRelativeError) {
                            QCOMPARE(relativeError <= maximumAllowedRelativeError, true);
                        }

                        if (relativeError > maximumRelativeError) {
                            maximumRelativeError = relativeError;
                        }
                    } else {
                        Model::Real absoluteError = std::abs(measured);

                        if (absoluteError > maximumAllowedAbsoluteError) {
                            QCOMPARE(absoluteError <= maximumAllowedAbsoluteError, true);
                        }

                        if (absoluteError > maximumAbsoluteError) {
                            maximumAbsoluteError = absoluteError;
                        }
                    }
                }
            }
        } else {
            ++numberFailures;
        }
    }

    Model::Real failureRate = (1.0 * numberFailures) / numberIterations;
    qDebug() << "maximum relative error = " << maximumRelativeError;
    qDebug() << "failure rate           = " << failureRate;
}


void TestMatrixReal::testCholesky() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(0, +1);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);
    Model::Real                                 maximumAllowedRelativeError = 1E-14;
    Model::Real                                 maximumRelativeError        = 0;
    Model::Real                                 maximumAllowedAbsoluteError = 1E-15;
    Model::Real                                 maximumAbsoluteError        = 0;

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned long numberRowColumns = dimensionDistribution(rng);

        // Generator algorithm below shamelessly lifted from:
        //     https://math.stackexchange.com/question/357980/
        //         how-to-generate-random-symmetric-positive-definite-matrices-using-matlab
        //
        // As the article indicates, the resulting distribution is not truly uniform random but it should be more than
        // good enough for our purposes.

        Model::MatrixReal x(numberRowColumns, numberRowColumns);
        for (unsigned long column=1 ; column<=numberRowColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRowColumns ; ++row) {
                Model::Real v = coefficientDistribution(rng);
                x.update(row, column, v);
            }
        }

        Model::MatrixReal m = 0.5 * x * x.adjoint();
        for (unsigned index=1 ; index<=numberRowColumns ; ++index) {
            Model::Real v = m(index, index);
            m.update(index, index, v + 2.0 * numberRowColumns);
        }

        QCOMPARE(m.isHermitian(), true); // To validate the function above.

        Model::MatrixReal l = m.cholesky();

        QCOMPARE(static_cast<unsigned long>(l.numberRows()), numberRowColumns);
        QCOMPARE(static_cast<unsigned long>(l.numberColumns()), numberRowColumns);

        Model::MatrixReal measuredMatrix = l * l.adjoint();

        for (unsigned long row=1 ; row<=numberRowColumns ; ++row) {
            for (unsigned long column=1 ; column<=numberRowColumns ; ++column) {
                Model::Real expected = m(row, column);
                Model::Real measured = measuredMatrix(row, column);

                if (expected != Model::Real(0)) {
                    Model::Real difference    = expected - measured;
                    Model::Real relativeError = std::abs(difference / expected);

                    if (relativeError > maximumAllowedRelativeError) {
                        QCOMPARE(relativeError <= maximumAllowedRelativeError, true);
                    }

                    if (relativeError > maximumRelativeError) {
                        maximumRelativeError = relativeError;
                    }
                } else {
                    Model::Real absoluteError = std::abs(measured);

                    if (absoluteError > maximumAllowedAbsoluteError) {
                        QCOMPARE(absoluteError <= maximumAllowedAbsoluteError, true);
                    }

                    if (absoluteError > maximumAbsoluteError) {
                        maximumAbsoluteError = absoluteError;
                    }
                }
            }
        }
    }

    qDebug() << "maximum relative error = " << maximumRelativeError;
}


void TestMatrixReal::testUpperCholesky() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(0, +1);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);
    Model::Real                                 maximumAllowedRelativeError = 5E-15;
    Model::Real                                 maximumRelativeError        = 0;
    Model::Real                                 maximumAllowedAbsoluteError = 1E-15;
    Model::Real                                 maximumAbsoluteError        = 0;

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned long numberRowColumns = dimensionDistribution(rng);

        // Generator algorithm below shamelessly lifted from:
        //     https://math.stackexchange.com/question/357980/
        //         how-to-generate-random-symmetric-positive-definite-matrices-using-matlab
        //
        // As the article indicates, the resulting distribution is not truly uniform random but it should be more than
        // good enough for our purposes.

        Model::MatrixReal x(numberRowColumns, numberRowColumns);
        for (unsigned long column=1 ; column<=numberRowColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRowColumns ; ++row) {
                Model::Real v = coefficientDistribution(rng);
                x.update(row, column, v);
            }
        }

        Model::MatrixReal m = 0.5 * x * x.adjoint();
        for (unsigned index=1 ; index<=numberRowColumns ; ++index) {
            Model::Real v = m(index, index);
            m.update(index, index, v + 2.0 * numberRowColumns);
        }

        QCOMPARE(m.isHermitian(), true); // To validate the function above.

        Model::MatrixReal u = m.upperCholesky();

        QCOMPARE(static_cast<unsigned long>(u.numberRows()), numberRowColumns);
        QCOMPARE(static_cast<unsigned long>(u.numberColumns()), numberRowColumns);

        Model::MatrixReal measuredMatrix = u.adjoint() * u;

        for (unsigned long row=1 ; row<=numberRowColumns ; ++row) {
            for (unsigned long column=1 ; column<=numberRowColumns ; ++column) {
                Model::Real expected = m(row, column);
                Model::Real measured = measuredMatrix(row, column);

                if (expected != Model::Real(0)) {
                    Model::Real difference    = expected - measured;
                    Model::Real relativeError = std::abs(difference / expected);

                    if (relativeError > maximumAllowedRelativeError) {
                        QCOMPARE(relativeError <= maximumAllowedRelativeError, true);
                    }

                    if (relativeError > maximumRelativeError) {
                        maximumRelativeError = relativeError;
                    }
                } else {
                    Model::Real absoluteError = std::abs(measured);

                    if (absoluteError > maximumAllowedAbsoluteError) {
                        QCOMPARE(absoluteError <= maximumAllowedAbsoluteError, true);
                    }

                    if (absoluteError > maximumAbsoluteError) {
                        maximumAbsoluteError = absoluteError;
                    }
                }
            }
        }
    }

    qDebug() << "maximum relative error = " << maximumRelativeError;
}


void TestMatrixReal::testBunchKaufman() {
//    // FIXME: Add support for Bunch-Kaufman decomposition
}


void TestMatrixReal::testUpperBunchKaufman() {
//    // FIXME: Add support for Bunch-Kaufman decomposition
}


void TestMatrixReal::testHessenberg() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(0, +1);
    std::uniform_int_distribution<>             dimensionDistribution(3, 1000);
    Model::Real                                 maximumAllowedRelativeError = 1E-7;
    Model::Real                                 maximumRelativeError        = 0;
    Model::Real                                 maximumAllowedAbsoluteError = 1E-15;
    Model::Real                                 maximumAbsoluteError        = 0;

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned long numberRowColumns = dimensionDistribution(rng);

        Model::MatrixReal x(numberRowColumns, numberRowColumns);
        for (unsigned long column=1 ; column<=numberRowColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRowColumns ; ++row) {
                Model::Real v = coefficientDistribution(rng);
                x.update(row, column, v);
            }
        }

        Model::Tuple hessenbergResult = x.hessenberg();
        QCOMPARE(hessenbergResult.size() , 2);

        Model::MatrixReal q = hessenbergResult(1);
        Model::MatrixReal h = hessenbergResult(2);

        Model::MatrixReal measuredMatrix = q * h * q.adjoint();

        for (unsigned long row=1 ; row<=numberRowColumns ; ++row) {
            for (unsigned long column=1 ; column<=numberRowColumns ; ++column) {
                Model::Real expected = x(row, column);
                Model::Real measured = measuredMatrix(row, column);

                if (expected != Model::Real(0)) {
                    Model::Real difference    = expected - measured;
                    Model::Real relativeError = std::abs(difference / expected);

                    if (relativeError > maximumAllowedRelativeError) {
                        QCOMPARE(relativeError <= maximumAllowedRelativeError, true);
                    }

                    if (relativeError > maximumRelativeError) {
                        maximumRelativeError = relativeError;
                    }
                } else {
                    Model::Real absoluteError = std::abs(measured);

                   if (absoluteError > maximumAllowedAbsoluteError) {
                        QCOMPARE(absoluteError <= maximumAllowedAbsoluteError, true);
                    }

                    if (absoluteError > maximumAbsoluteError) {
                        maximumAbsoluteError = absoluteError;
                    }
                }
            }
        }
    }

    qDebug() << "maximum relative error = " << maximumRelativeError;
}


void TestMatrixReal::testDCT() {
    Model::MatrixReal t1di;
    for (Model::Integer r=1 ; r<=100000 ; ++r) {
        t1di.update(r, std::sin((2.0 * M::pi * 100.0 * (r-1.0))/100000.0));
    }

    Model::MatrixReal f1   = t1di.dct();
    Model::MatrixReal t1do = f1.idct();

    QCOMPARE(t1di.numberCoefficients(), t1do.numberCoefficients());
    QCOMPARE(t1di.numberRows(), t1do.numberRows());
    QCOMPARE(t1di.numberColumns(), t1do.numberColumns());

    Model::Integer numberRows          = t1do.numberRows();
    Model::Integer numberColumns       = t1do.numberColumns();
    double         sumSquareDifference = 0;

    for (Model::Integer r=1 ; r<=numberRows ; ++r) {
        for (Model::Integer c=1 ; c<=numberColumns ; ++c) {
            double d = t1di.at(r, c) - t1do.at(r, c);
            sumSquareDifference += d * d;
        }
    }

    double rms = std::sqrt(sumSquareDifference / (numberRows * numberColumns));
    qDebug() << rms;

    if (rms > 1E-15) {
        QCOMPARE(rms <= 1.0E-15, true);
    }

    Model::MatrixReal t2di(10000, 10000);
    for (Model::Integer r=1 ; r<=10000 ; ++r) {
        for (Model::Integer c=1 ; c<=10000 ; ++c) {
            t2di.update(
                r,
                c,
                  std::sin((2.0 * M::pi * 100.0 * (r-1.0))/10000.0)
                + std::sin((2.0 * M::pi * 120 * (r-1.0))/10000.0)
            );
        }
    }

    Model::MatrixReal f2   = t2di.dct();
    Model::MatrixReal t2do = f2.idct();

    QCOMPARE(t2di.numberCoefficients(), t2do.numberCoefficients());
    QCOMPARE(t2di.numberRows(), t2do.numberRows());
    QCOMPARE(t2di.numberColumns(), t2do.numberColumns());

    numberRows          = t2do.numberRows();
    numberColumns       = t2do.numberColumns();
    sumSquareDifference = 0;

    for (Model::Integer r=1 ; r<=numberRows ; ++r) {
        for (Model::Integer c=1 ; c<=numberColumns ; ++c) {
            double d = t2di.at(r, c) - t2do.at(r, c);
            sumSquareDifference += d * d;
        }
    }

    rms = std::sqrt(sumSquareDifference / (numberRows * numberColumns));
    qDebug() << rms;

    if (rms > 1E-15) {
        QCOMPARE(rms <= 1.0E-15, true);
    }
}


void TestMatrixReal::testHilbert() {
    Model::MatrixReal expected;
    for (Model::Integer r=1 ; r<=100000 ; ++r) {
        expected.update(
            r,
            std::sin((2.0 * M::pi * 100.0 * (r-1.0))/100000.0) + std::sin((3.0 * M::pi * 100.0 * (r-1))/100000.0)
        );
    }

    Model::MatrixComplex h = expected.hilbertTransform();

    QCOMPARE(expected.numberCoefficients(), h.numberCoefficients());
    QCOMPARE(expected.numberRows(), h.numberRows());
    QCOMPARE(expected.numberColumns(), h.numberColumns());

    Model::MatrixReal measured1 = h.real();
    Model::MatrixReal measured2 = h.imag().hilbertTransform().imag();
    Model::Real       s1        = 0;
    Model::Real       s2        = 0;
    for (Model::Integer r=1 ; r<=100000 ; ++r) {
        Model::Real m1 = measured1.at(r);
        Model::Real e  = expected.at(r);
        Model::Real m2 = measured2.at(r);

        Model::Real d1 = e - m1;
        Model::Real d2 = e - m2;
        s1 += d1 * d1;
        s2 += d2 * d2;
    }

    Model::Real rms1 = std::sqrt(s1 / 100000.0);
    Model::Real rms2 = std::sqrt(s2 / 100000.0);

    if (rms1 > 1.0E-15) {
        qDebug() << rms1;
        QCOMPARE(rms1 <= 1.0E15, true);
    }

    if (rms2 > 1.0E-15) {
        qDebug() << rms2;
        QCOMPARE(rms2 <= 1.0E15, true);
    }
}


void TestMatrixReal::testConditionNumber() {
    Model::MatrixReal m1 = Model::MatrixReal::build(
        3, 3,
        Model::Real(1), Model::Real(4), Model::Real(7),
        Model::Real(2), Model::Real(5), Model::Real(8),
        Model::Real(3), Model::Real(6), Model::Real(9)
    );

    Model::Real cn            = m1.conditionNumber();
    Model::Real expected      = 3.2E+17;
    Model::Real relativeError = abs(cn - expected) / expected;

    QCOMPARE(relativeError < 0.5, true);
}


void TestMatrixReal::testNormMethods() {
    Model::MatrixReal m1 = Model::MatrixReal::build(
        3, 3,
        Model::Real(1), Model::Real(4), Model::Real(7),
        Model::Real(2), Model::Real(5), Model::Real(8),
        Model::Real(3), Model::Real(6), Model::Real(9)
    );

    Model::Real pNorm1Measured = m1.pNorm(1);
    Model::Real pNorm1Expected = 45;
    Model::Real pNorm1Error    = std::abs(pNorm1Measured - pNorm1Expected);
    QCOMPARE(pNorm1Error < 1.0E-10, true);

    Model::Real pNorm2Measured = m1.pNorm(2);
    Model::Real pNorm2Expected = 16.881943016134134;
    Model::Real pNorm2Error    = std::abs(pNorm2Measured - pNorm2Expected);
    QCOMPARE(pNorm2Error < 1.0E-10, true);

    Model::Real pNorm3Measured = m1.pNorm(3);
    Model::Real pNorm3Expected = 12.651489979526238;
    Model::Real pNorm3Error    = std::abs(pNorm3Measured - pNorm3Expected);
    QCOMPARE(pNorm3Error < 1.0E-10, true);

    Model::Real euclideanNormMeasured = m1.euclideanNorm();
    Model::Real euclideanNormExpected = 16.881943016134134;
    Model::Real euclideanNormError    = std::abs(euclideanNormMeasured - euclideanNormExpected);
    QCOMPARE(euclideanNormError < 1.0E-10, true);

    Model::Real norm1Measured = m1.oneNorm();
    Model::Real norm1Expected = 18;
    Model::Real norm1Error    = std::abs(norm1Measured - norm1Expected);
    QCOMPARE(norm1Error < 1.0E-10, true);

    Model::Real normInfinityMeasured = m1.infinityNorm();
    Model::Real normInfinityExpected = 24;
    Model::Real normInfinityError    = std::abs(normInfinityMeasured - normInfinityExpected);
    QCOMPARE(normInfinityError < 1.0E-10, true);
}


void TestMatrixReal::testRank() {
    Model::MatrixReal m1 = Model::MatrixReal::build(
        3, 3,
        Model::Real( 1), Model::Real( 3), Model::Real( 5),
        Model::Real( 7), Model::Real(11), Model::Real(13),
        Model::Real(17), Model::Real(19), Model::Real(23)
    );

    Model::MatrixReal m2 = Model::MatrixReal::build(
        3, 3,
        Model::Real( 1), Model::Real( 3), Model::Real( 5),
        Model::Real( 7), Model::Real(11), Model::Real(13),
        Model::Real(14), Model::Real(22), Model::Real(26)
    );

    Model::MatrixReal m3 = Model::MatrixReal::build(
        3, 4,
        Model::Real( 1), Model::Real( 3), Model::Real( 5),
        Model::Real( 7), Model::Real(11), Model::Real(13),
        Model::Real( 2), Model::Real( 6), Model::Real(26),
        Model::Real(17), Model::Real(19), Model::Real(23)
    );

    Model::Integer rank1 = m1.rank(1.0E-10);
    Model::Integer rank2 = m2.rank(1.0E-10);
    Model::Integer rank3 = m3.rank(1.0E-10);

    QCOMPARE(rank1, 3);
    QCOMPARE(rank2, 2);
    QCOMPARE(rank3, 3);
}


void TestMatrixReal::testEquilibrate() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E6, +1E6);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);
    float                                       maximumDegregation = 1.10F;
    float                                       worstCaseDegredation = 0.0F;

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned long numberRowColumns = dimensionDistribution(rng);

        Model::MatrixReal m(numberRowColumns, numberRowColumns);
        for (unsigned long column=1 ; column<=numberRowColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRowColumns ; ++row) {
                Model::Real v = coefficientDistribution(rng);
                m.update(row, column, v);
            }
        }

        bool        canInvert = true;
        Model::Real initialConditionNumber;
        try {
            initialConditionNumber = m.conditionNumber();
        } catch(Model::MatrixIsSingular&) {
            canInvert = false;
        }

        if (canInvert) {
            Model::Tuple         t            = m.equilibrate();
            Model::MatrixReal rowScaler    = t(1);
            Model::MatrixReal columnScaler = t(2);
            Model::Boolean       successful   = t(3);

            QCOMPARE(successful, true);

            Model::MatrixReal rowScaledMatrix    = rowScaler * m;
            Model::MatrixReal columnScaledMatrix = m * columnScaler;
            Model::MatrixReal scaledMatrix       = rowScaledMatrix * columnScaler;

            Model::Real scaledConditionNumber = scaledMatrix.conditionNumber();
            if (scaledConditionNumber >= maximumDegregation * initialConditionNumber) {
                QCOMPARE(scaledConditionNumber < maximumDegregation * initialConditionNumber, true);
            }

            if (scaledConditionNumber > initialConditionNumber) {
                float degregation = scaledConditionNumber / initialConditionNumber;
                if (degregation > worstCaseDegredation) {
                    worstCaseDegredation = degregation;
                }
            }
        }
    }

    qDebug() << "worst case degredation = " << worstCaseDegredation;
}


void TestMatrixReal::testSolve() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E6, +1E6);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);
    std::uniform_int_distribution<>             rightHandSidesDistribution(1, 10);
    Model::Real                                 maximumAllowedRelativeError = 5E-4;
    Model::Real                                 maximumRelativeError        = 0;
    Model::Real                                 maximumAllowedAbsoluteError = 1E-10;
    Model::Real                                 maximumAbsoluteError        = 0;

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned long numberRowColumns     = dimensionDistribution(rng);
        unsigned      numberRightHandSides = rightHandSidesDistribution(rng);
        Model::MatrixReal a(numberRowColumns, numberRowColumns);
        Model::MatrixReal y(numberRowColumns, numberRightHandSides);

        for (unsigned long row=1 ; row<=numberRowColumns ; ++row) {
            for (unsigned long column=1 ; column<=numberRowColumns ; ++column) {
                Model::Real v;
                do {
                    v = coefficientDistribution(rng); // coefficientDistribution(rng));
                } while (M::abs(v) < 1);

                a.update(row, column, v);
            }

            for (unsigned long column=1 ; column<=numberRightHandSides ; ++column) {
                Model::Real v = coefficientDistribution(rng);
                y.update(row, column, v);
            }
        }

        Model::MatrixReal x = a.solve(y);

        QCOMPARE(static_cast<unsigned long>(x.numberRows()), numberRowColumns);
        QCOMPARE(static_cast<unsigned long>(x.numberColumns()), numberRightHandSides);

        Model::MatrixReal yMeasured = a * x;
        for (unsigned long column=1 ; column<numberRightHandSides ; ++column) {
            for (unsigned long row=1 ; row<numberRowColumns ; ++row) {
                Model::Real expected   = y(row, column);
                Model::Real measured   = yMeasured(row, column);
                Model::Real difference = expected - measured;

                if (expected != Model::Real(0)) {
                    Model::Real relativeError = M::abs(difference);
                    if (relativeError > maximumAllowedRelativeError) {
                        QCOMPARE(relativeError <= maximumAllowedRelativeError, true);
                    }

                    if (relativeError > maximumRelativeError) {
                        maximumRelativeError = relativeError;
                    }
                } else {
                    Model::Real absoluteError = M::abs(difference);

                    if (absoluteError > maximumAllowedAbsoluteError) {
                        QCOMPARE(absoluteError <= maximumAllowedAbsoluteError, true);
                    }

                    if (absoluteError > maximumAbsoluteError) {
                        maximumAbsoluteError = absoluteError;
                    }
                }
            }
        }
    }

    qDebug() << "maximum relative error = " << maximumRelativeError;
    qDebug() << "maximum absolute error = " << maximumAbsoluteError;
}


void TestMatrixReal::testLeastSquares() {
    std::mt19937                                rng(0x12345678);
    std::uniform_int_distribution<>             dimensionDistribution(1, 100);
    std::uniform_int_distribution<>             rightHandSidesDistribution(1, 10);
    std::uniform_real_distribution<>            bTermDistribution(-1.0E3, +1.0E3);
    Model::Real                                 maximumAllowedUnderdeterminedRelativeError = 1.0E-10;
    Model::Real                                 maximumUnderdeterminedRelativeError        = 0;
    Model::Real                                 maximumAllowedOverdeterminedRelativeError = 1.0;
    Model::Real                                 maximumOverdeterminedRelativeError        = 0;

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned long aNumberRows;
        unsigned long aNumberColumns;

        // Create over or under determined full rank matrix.
        do {
            aNumberRows    = dimensionDistribution(rng);
            aNumberColumns = dimensionDistribution(rng);
        } while (aNumberRows == aNumberColumns);

        // To make our matrix full rank, we populate the matrix with primes so that no value is divisible by another
        // value.  We first determine how many primes we need and then use that to build a list.
        unsigned long long numberRealTerms = aNumberRows * aNumberColumns;
        QList<unsigned long long> primes = calculatePrimes(numberRealTerms);

        std::uniform_int_distribution<unsigned long> indexDistribution(0, numberRealTerms - 1);
        for (unsigned i=0 ; i<numberRealTerms ; ++i) {
            unsigned long indexA = indexDistribution(rng);
            unsigned long indexB = indexDistribution(rng);

            if (indexA != indexB) {
                unsigned long long t = primes[indexA];
                primes[indexA] = primes[indexB];
                primes[indexB] = t;
            }
        }

        unsigned long numberRightHandSides = rightHandSidesDistribution(rng);

        Model::MatrixReal a(aNumberRows, aNumberColumns);
        Model::MatrixReal b(aNumberRows, numberRightHandSides);
        unsigned long        primeIndex = 0;
        for (unsigned long row=1 ; row<=aNumberRows ; ++row) {
            for (unsigned long column=1 ; column<=aNumberColumns ; ++column) {
                a.update(row, column, primes[primeIndex]);
                ++primeIndex;
            }

            for (unsigned long column=1 ; column<=numberRightHandSides ; ++column) {
                Model::Real v;
                do {
                    v = Model::Real(bTermDistribution(rng));
                } while (v == Model::Real(0));
                b.update(row, column, v);
            }
        }

        Model::MatrixReal x = a.leastSquares(b);
        QCOMPARE(static_cast<unsigned long>(x.numberRows()), aNumberColumns);
        QCOMPARE(static_cast<unsigned long>(x.numberColumns()), numberRightHandSides);

        Model::MatrixReal residuals = a * x - b;

        if (aNumberRows < aNumberColumns) {
            // Underdetermined case.

            for (unsigned long row=1 ; row<=aNumberRows ; ++row) {
                for (unsigned long column=1 ; column<=numberRightHandSides ; ++column) {
                    Model::Real magnitudeResidual = M::abs(residuals(row, column)) / M::abs(b(row, column));
                    if (magnitudeResidual > maximumAllowedUnderdeterminedRelativeError) {
                        QCOMPARE(magnitudeResidual <= maximumAllowedUnderdeterminedRelativeError, true);
                    }

                    if (magnitudeResidual > maximumUnderdeterminedRelativeError) {
                        maximumUnderdeterminedRelativeError = magnitudeResidual;
                    }
                }
            }
        } else {
            // Overdetermined case.

            Model::Real relativeNorm = residuals.euclideanNorm() / b.euclideanNorm();

            if (relativeNorm > maximumAllowedOverdeterminedRelativeError) {
                QCOMPARE(relativeNorm <= maximumAllowedOverdeterminedRelativeError, true);
            }

            if (relativeNorm > maximumOverdeterminedRelativeError) {
                maximumOverdeterminedRelativeError = relativeNorm;
            }
        }
    }

    qDebug() << "maximum underdetermined relative error = " << maximumUnderdeterminedRelativeError;
    qDebug() << "maximum overdetermined relative error = " << maximumOverdeterminedRelativeError;
}


void TestMatrixReal::testHadamardProduct() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E6, +1E6);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);
    Model::Real                                 maximumAllowedRelativeError = 1E-15;
    Model::Real                                 maximumRelativeError        = 0;
    Model::Real                                 maximumAllowedAbsoluteError = 1E-15;
    Model::Real                                 maximumAbsoluteError        = 0;

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned long numberRows    = dimensionDistribution(rng);
        unsigned long numberColumns = dimensionDistribution(rng);

        Model::MatrixReal m1(numberRows, numberColumns);
        Model::MatrixReal m2(numberRows, numberColumns);
        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                Model::Real v1 = coefficientDistribution(rng);
                m1.update(row, column, v1);

                Model::Real v2 = coefficientDistribution(rng);
                m2.update(row, column, v2);
            }
        }

        Model::MatrixReal m = m1.hadamard(m2);

        QCOMPARE(static_cast<unsigned long>(m.numberRows()), numberRows);
        QCOMPARE(static_cast<unsigned long>(m.numberColumns()), numberColumns);

        for (unsigned long row=1 ; row<=numberRows ; ++row) {
            for (unsigned long column=1 ; column<=numberColumns ; ++column) {
                Model::Real expected = m1(row, column) * m2(row, column);
                Model::Real measured = m(row, column);

                if (expected != Model::Real(0)) {
                    Model::Real difference    = expected - measured;
                    Model::Real relativeError = std::abs(difference / expected);

                    if (relativeError > maximumAllowedRelativeError) {
                        QCOMPARE(relativeError < maximumAllowedRelativeError, true);
                    }

                    if (relativeError > maximumRelativeError) {
                        maximumRelativeError = relativeError;
                    }
                } else {
                    Model::Real absoluteError = std::abs(measured);

                    if (absoluteError > maximumAllowedAbsoluteError) {
                        QCOMPARE(absoluteError < maximumAllowedAbsoluteError, true);
                    }

                    if (absoluteError > maximumAbsoluteError) {
                        maximumAbsoluteError = absoluteError;
                    }
                }
            }
        }
    }

    qDebug() << "maximum relative error = " << maximumRelativeError;
}


void TestMatrixReal::testKroneckerProduct() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E6, +1E6);
    std::uniform_int_distribution<>             dimensionDistribution(1, 50);
    Model::Real                                 maximumAllowedRelativeError = 1E-15;
    Model::Real                                 maximumRelativeError        = 0;
    Model::Real                                 maximumAllowedAbsoluteError = 1E-15;
    Model::Real                                 maximumAbsoluteError        = 0;

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned long multiplierNumberRows      = dimensionDistribution(rng);
        unsigned long multiplierNumberColumns   = dimensionDistribution(rng);
        unsigned long multiplicandNumberRows    = dimensionDistribution(rng);
        unsigned long multiplicandNumberColumns = dimensionDistribution(rng);

        Model::MatrixReal m1(multiplierNumberRows, multiplierNumberColumns);
        Model::MatrixReal m2(multiplicandNumberRows, multiplicandNumberColumns);

        for (unsigned long column=1 ; column<=multiplierNumberColumns ; ++column) {
            for (unsigned long row=1 ; row<=multiplierNumberRows ; ++row) {
                Model::Real v1 = coefficientDistribution(rng);
                m1.update(row, column, v1);
            }
        }

        for (unsigned long column=1 ; column<=multiplicandNumberColumns ; ++column) {
            for (unsigned long row=1 ; row<=multiplicandNumberRows ; ++row) {
                Model::Real v2 = coefficientDistribution(rng);
                m2.update(row, column, v2);
            }
        }

        Model::MatrixReal m = m1.kronecker(m2);

        QCOMPARE(static_cast<unsigned long>(m.numberRows()), multiplierNumberRows * multiplicandNumberRows);
        QCOMPARE(static_cast<unsigned long>(m.numberColumns()), multiplierNumberColumns * multiplicandNumberColumns);

        for (unsigned long multiplierRow=1 ; multiplierRow<=multiplierNumberRows ; ++multiplierRow) {
            unsigned long productRowOffset = (multiplierRow - 1) * multiplicandNumberRows;

            for (unsigned long multiplierColumn=1 ; multiplierColumn<=multiplierNumberColumns ; ++multiplierColumn) {
                unsigned long  productColumnOffset = (multiplierColumn - 1) * multiplicandNumberColumns;
                Model::Real multiplier          = m1(multiplierRow, multiplierColumn);

                for (unsigned long multiplicandRow=1 ; multiplicandRow<=multiplicandNumberRows ; ++multiplicandRow) {
                    unsigned long productRow = productRowOffset + multiplicandRow;

                    for (  unsigned long multiplicandColumn = 1
                         ; multiplicandColumn <= multiplicandNumberColumns
                         ; ++multiplicandColumn
                        ) {
                        unsigned long productColumn = productColumnOffset + multiplicandColumn;

                        Model::Real multiplicand = m2(multiplicandRow, multiplicandColumn);
                        Model::Real expected     = multiplier * multiplicand;
                        Model::Real measured     = m(productRow, productColumn);

                        if (expected != Model::Real(0)) {
                            Model::Real difference    = expected - measured;
                            Model::Real relativeError = std::abs(difference / expected);

                            if (relativeError > maximumAllowedRelativeError) {
                                QCOMPARE(relativeError < maximumAllowedRelativeError, true);
                            }

                            if (relativeError > maximumRelativeError) {
                                maximumRelativeError = relativeError;
                            }
                        } else {
                            Model::Real absoluteError = std::abs(measured);

                            if (absoluteError > maximumAllowedAbsoluteError) {
                                QCOMPARE(absoluteError < maximumAllowedAbsoluteError, true);
                            }

                            if (absoluteError > maximumAbsoluteError) {
                                maximumAbsoluteError = absoluteError;
                            }
                        }
                    }
                }
            }
        }
    }

    qDebug() << "maximum relative error = " << maximumRelativeError;
}


void TestMatrixReal::testRawDataAccess() {
    Model::MatrixReal m = Model::MatrixReal::build(
        2, 3,
        Model::Real(1), Model::Real(4),
        Model::Real(2), Model::Real(5),
        Model::Real(3), Model::Real(6)
    );

    const Model::Real* rawData = m.data();

    QCOMPARE(rawData[ 0], Model::Real(1));
    QCOMPARE(rawData[ 1], Model::Real(4));
    QCOMPARE(rawData[ 2], Model::Real(0));
    QCOMPARE(rawData[ 3], Model::Real(0));
    QCOMPARE(rawData[ 4], Model::Real(0));
    QCOMPARE(rawData[ 5], Model::Real(0));
    QCOMPARE(rawData[ 6], Model::Real(0));
    QCOMPARE(rawData[ 7], Model::Real(0));

    QCOMPARE(rawData[ 8], Model::Real(2));
    QCOMPARE(rawData[ 9], Model::Real(5));
    QCOMPARE(rawData[10], Model::Real(0));
    QCOMPARE(rawData[11], Model::Real(0));
    QCOMPARE(rawData[12], Model::Real(0));
    QCOMPARE(rawData[13], Model::Real(0));
    QCOMPARE(rawData[14], Model::Real(0));
    QCOMPARE(rawData[15], Model::Real(0));

    QCOMPARE(rawData[16], Model::Real(3));
    QCOMPARE(rawData[17], Model::Real(6));
    QCOMPARE(rawData[18], Model::Real(0));
    QCOMPARE(rawData[19], Model::Real(0));
    QCOMPARE(rawData[20], Model::Real(0));
    QCOMPARE(rawData[21], Model::Real(0));
    QCOMPARE(rawData[22], Model::Real(0));
    QCOMPARE(rawData[23], Model::Real(0));
}


void TestMatrixReal::testFloorFunctions() {
    Model::MatrixReal m = Model::MatrixReal::build(
        2, 3,
        Model::Real( 1   ), Model::Real( 4.5 ),
        Model::Real(-2.1 ), Model::Real( 5.5 ),
        Model::Real( 3.49), Model::Real( 6.51)
    );

    Model::MatrixReal r = m.transpose().floor();

    QCOMPARE(
        r,
        Model::MatrixReal::build(
            3, 2,
            Model::Real( 1), Model::Real(-3), Model::Real( 3),
            Model::Real( 4), Model::Real( 5), Model::Real( 6)
        )
    );
}

void TestMatrixReal::testCeilingFunctions() {
    Model::MatrixReal m = Model::MatrixReal::build(
        2, 3,
        Model::Real( 1   ), Model::Real( 4.5 ),
        Model::Real(-2.1 ), Model::Real( 5.5 ),
        Model::Real( 3.49), Model::Real( 6.51)
    );

    Model::MatrixReal r = m.transpose().ceil();

    QCOMPARE(
        r,
        Model::MatrixReal::build(
            3, 2,
            Model::Real( 1), Model::Real(-2), Model::Real( 4),
            Model::Real( 5), Model::Real( 6), Model::Real( 7)
        )
    );
}


void TestMatrixReal::testNearbyIntFunctions() {
    Model::MatrixReal m = Model::MatrixReal::build(
        2, 3,
        Model::Real( 1   ), Model::Real( 4.5 ),
        Model::Real(-2.1 ), Model::Real( 5.5 ),
        Model::Real( 3.49), Model::Real( 6.51)
    );

    Model::MatrixReal r = m.transpose().nint();

    QCOMPARE(
        r,
        Model::MatrixReal::build(
            3, 2,
            Model::Real( 1), Model::Real(-2), Model::Real( 3),
            Model::Real( 4), Model::Real( 6), Model::Real( 7)
        )
    );
}


void TestMatrixReal::testTruncateToIntegerFunction() {
    Model::MatrixReal m = Model::MatrixReal::build(
        2, 3,
        Model::Real( 1   ), Model::Real( 4.5 ),
        Model::Real(-2.1 ), Model::Real( 5.5 ),
        Model::Real( 3.49), Model::Real( 6.51)
    );

    Model::MatrixInteger t = m.transpose().truncateToInteger();

    QCOMPARE(
        t,
        Model::MatrixInteger::build(
            3, 2,
            Model::Integer( 1), Model::Integer(-2), Model::Integer( 3),
            Model::Integer( 4), Model::Integer( 5), Model::Integer( 6)
        )
    );

    Model::MatrixInteger f = m.transpose().floorToInteger();

    QCOMPARE(
        f,
        Model::MatrixInteger::build(
            3, 2,
            Model::Integer( 1), Model::Integer(-3), Model::Integer( 3),
            Model::Integer( 4), Model::Integer( 5), Model::Integer( 6)
        )
    );

    Model::MatrixInteger c = m.transpose().ceilToInteger();

    QCOMPARE(
        c,
        Model::MatrixInteger::build(
            3, 2,
            Model::Integer( 1), Model::Integer(-2), Model::Integer( 4),
            Model::Integer( 5), Model::Integer( 6), Model::Integer( 7)
        )
    );

    Model::MatrixInteger n = m.transpose().nintToInteger();

    QCOMPARE(
        n,
        Model::MatrixInteger::build(
            3, 2,
            Model::Integer( 1), Model::Integer(-2), Model::Integer( 3),
            Model::Integer( 4), Model::Integer( 6), Model::Integer( 7)
        )
    );
}


void TestMatrixReal::testAssignmentOperators() {
    Model::MatrixReal m1(2, 3);
    QCOMPARE(m1.numberRows(),    2);
    QCOMPARE(m1.numberColumns(), 3);

    m1.update(1, 1, Model::Real(1));
    m1.update(1, 2, Model::Real(2));
    m1.update(1, 3, Model::Real(3));
    m1.update(2, 1, Model::Real(4));
    m1.update(2, 2, Model::Real(5));
    m1.update(2, 3, Model::Real(6));

    Model::MatrixReal m2(1, 2);
    m2.update(1, 1, Model::Real(7));
    m2.update(1, 2, Model::Real(8));

    QCOMPARE(m1.numberRows(),    2);
    QCOMPARE(m1.numberColumns(), 3);

    QCOMPARE(m1(1,1), Model::Real(1));
    QCOMPARE(m1(1,2), Model::Real(2));
    QCOMPARE(m1(1,3), Model::Real(3));
    QCOMPARE(m1(2,1), Model::Real(4));
    QCOMPARE(m1(2,2), Model::Real(5));
    QCOMPARE(m1(2,3), Model::Real(6));

    QCOMPARE(m2.numberRows(),    1);
    QCOMPARE(m2.numberColumns(), 2);
    QCOMPARE(m2(1,1), Model::Real(7));
    QCOMPARE(m2(1,2), Model::Real(8));

    m2 = m1;

    QCOMPARE(m2.numberRows(),    2);
    QCOMPARE(m2.numberColumns(), 3);

    QCOMPARE(m2(1,1), Model::Real(1));
    QCOMPARE(m2(1,2), Model::Real(2));
    QCOMPARE(m2(1,3), Model::Real(3));
    QCOMPARE(m2(2,1), Model::Real(4));
    QCOMPARE(m2(2,2), Model::Real(5));
    QCOMPARE(m2(2,3), Model::Real(6));

    m1.update(1, 1, Model::Real(10));
    m1.update(1, 2, Model::Real(11));
    m1.update(1, 3, Model::Real(12));
    m1.update(2, 1, Model::Real(13));
    m1.update(2, 2, Model::Real(14));
    m1.update(2, 3, Model::Real(15));

    QCOMPARE(m2(1,1), Model::Real(1));
    QCOMPARE(m2(1,2), Model::Real(2));
    QCOMPARE(m2(1,3), Model::Real(3));
    QCOMPARE(m2(2,1), Model::Real(4));
    QCOMPARE(m2(2,2), Model::Real(5));
    QCOMPARE(m2(2,3), Model::Real(6));
}


void TestMatrixReal::testAddAssignmentOperators() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E10, +1E10);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);
    Model::Real                                 maximumAllowedRelativeError = 1E-9;
    Model::Real                                 maximumRelativeError        = 0;
    Model::Real                                 maximumAllowedAbsoluteError = 1E-15;
    Model::Real                                 maximumAbsoluteError        = 0;

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned numberRows    = dimensionDistribution(rng);
        unsigned numberColumns = dimensionDistribution(rng);

        Model::MatrixReal expected1(numberRows, numberColumns);
        Model::MatrixReal expected2(numberColumns, numberRows);

        Model::MatrixReal augend(numberRows, numberColumns);
        Model::MatrixReal addend(numberRows, numberColumns);

        for (unsigned long columnIndex=1 ; columnIndex<=numberColumns ; ++columnIndex) {
            for (unsigned long rowIndex=1 ; rowIndex<=numberRows ; ++rowIndex) {
                Model::Real a1 = coefficientDistribution(rng);
                Model::Real a2 = coefficientDistribution(rng);

                augend.update(rowIndex, columnIndex, a1);
                addend.update(rowIndex, columnIndex, a2);

                expected1.update(rowIndex, columnIndex, a1 + a2);
                expected2.update(columnIndex, rowIndex, 2 * a1 + 3 * a2);
            }
        }

        Model::MatrixReal measured1 = augend;
        measured1 += addend;

        Model::MatrixReal measured2 = 2 * augend.transpose();
        measured2 += 3 * addend.transpose();

        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                Model::Real e1 = expected1(row, column);
                Model::Real m1 = measured1(row, column);
                Model::Real e2 = expected2(column, row);
                Model::Real m2 = measured2(column, row);

                if (e1 != Model::Real(0)) {
                    Model::Real difference    = e1 - m1;
                    Model::Real relativeError = std::abs(difference / e1);

                    if (relativeError > maximumAllowedRelativeError) {
                        QCOMPARE(relativeError < maximumAllowedRelativeError, true);
                    }

                    if (relativeError > maximumRelativeError) {
                        maximumRelativeError = relativeError;
                    }
                } else {
                    Model::Real absoluteError = std::abs(m1);

                    if (absoluteError > maximumAllowedAbsoluteError) {
                        QCOMPARE(absoluteError < maximumAllowedAbsoluteError, true);
                    }

                    if (absoluteError > maximumAbsoluteError) {
                        maximumAbsoluteError = absoluteError;
                    }
                }

                if (e2 != Model::Real(0)) {
                    Model::Real difference    = e2 - m2;
                    Model::Real relativeError = std::abs(difference / e2);

                    if (relativeError > maximumAllowedRelativeError) {
                        QCOMPARE(relativeError < maximumAllowedRelativeError, true);
                    }

                    if (relativeError > maximumRelativeError) {
                        maximumRelativeError = relativeError;
                    }
                } else {
                    Model::Real absoluteError = std::abs(m2);

                    if (absoluteError > maximumAllowedAbsoluteError) {
                        QCOMPARE(absoluteError < maximumAllowedAbsoluteError, true);
                    }

                    if (absoluteError > maximumAbsoluteError) {
                        maximumAbsoluteError = absoluteError;
                    }
                }
            }
        }
    }

    qDebug() << "maximum relative error = " << maximumRelativeError;

    Model::MatrixReal m1(10, 11);
    Model::MatrixReal m2(12, 13);

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


void TestMatrixReal::testSubtractAssignmentOperators() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E10, +1E10);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);
    Model::Real                                 maximumAllowedRelativeError = 1E-9;
    Model::Real                                 maximumRelativeError        = 0;
    Model::Real                                 maximumAllowedAbsoluteError = 1E-15;
    Model::Real                                 maximumAbsoluteError        = 0;

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned numberRows    = dimensionDistribution(rng);
        unsigned numberColumns = dimensionDistribution(rng);

        Model::MatrixReal expected1(numberRows, numberColumns);
        Model::MatrixReal expected2(numberColumns, numberRows);

        Model::MatrixReal augend(numberRows, numberColumns);
        Model::MatrixReal addend(numberRows, numberColumns);

        for (unsigned long columnIndex=1 ; columnIndex<=numberColumns ; ++columnIndex) {
            for (unsigned long rowIndex=1 ; rowIndex<=numberRows ; ++rowIndex) {
                Model::Real a1 = coefficientDistribution(rng);
                Model::Real a2 = coefficientDistribution(rng);

                augend.update(rowIndex, columnIndex, a1);
                addend.update(rowIndex, columnIndex, a2);

                expected1.update(rowIndex, columnIndex, a1 - a2);
                expected2.update(columnIndex, rowIndex, 2 * a1 - 3 * a2);
            }
        }

        Model::MatrixReal measured1 = augend;
        measured1 -= addend;

        Model::MatrixReal measured2 = 2 * augend.transpose();
        measured2 -= 3 * addend.transpose();

        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                Model::Real e1 = expected1(row, column);
                Model::Real m1 = measured1(row, column);
                Model::Real e2 = expected2(column, row);
                Model::Real m2 = measured2(column, row);

                if (e1 != Model::Real(0)) {
                    Model::Real difference    = e1 - m1;
                    Model::Real relativeError = std::abs(difference / e1);

                    if (relativeError > maximumAllowedRelativeError) {
                        QCOMPARE(relativeError < maximumAllowedRelativeError, true);
                    }

                    if (relativeError > maximumRelativeError) {
                        maximumRelativeError = relativeError;
                    }
                } else {
                    Model::Real absoluteError = std::abs(m1);

                    if (absoluteError > maximumAllowedAbsoluteError) {
                        QCOMPARE(absoluteError < maximumAllowedAbsoluteError, true);
                    }

                    if (absoluteError > maximumAbsoluteError) {
                        maximumAbsoluteError = absoluteError;
                    }
                }

                if (e2 != Model::Real(0)) {
                    Model::Real difference    = e2 - m2;
                    Model::Real relativeError = std::abs(difference / e2);

                    if (relativeError > maximumAllowedRelativeError) {
                        QCOMPARE(relativeError < maximumAllowedRelativeError, true);
                    }

                    if (relativeError > maximumRelativeError) {
                        maximumRelativeError = relativeError;
                    }
                } else {
                    Model::Real absoluteError = std::abs(m2);

                    if (absoluteError > maximumAllowedAbsoluteError) {
                        QCOMPARE(absoluteError < maximumAllowedAbsoluteError, true);
                    }

                    if (absoluteError > maximumAbsoluteError) {
                        maximumAbsoluteError = absoluteError;
                    }
                }
            }
        }
    }

    qDebug() << "maximum relative error = " << maximumRelativeError;

    Model::MatrixReal m1(10, 11);
    Model::MatrixReal m2(12, 13);

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


void TestMatrixReal::testMatrixMatrixMultiplyAssignmentOperator() {
    // For now we just to a cursory test.  Since the underlying engine is the same, we do a more thorough test below.

    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E10, +1E10);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);
    Model::Real                                 maximumError = 1E-9;

    unsigned multiplierRows      = dimensionDistribution(rng);
    unsigned multiplierColumns   = dimensionDistribution(rng);
    unsigned multiplicandRows    = multiplierColumns;
    unsigned multiplicandColumns = dimensionDistribution(rng);

    Model::MatrixReal multiplier(multiplierRows, multiplierColumns);
    for (unsigned row=1 ; row<=multiplierRows ; ++row) {
        for (unsigned column=1 ; column<multiplierColumns ; ++column) {
            Model::Real c = coefficientDistribution(rng);
            multiplier.update(row, column, c);
        }
    }

    Model::MatrixReal multiplicand(multiplicandRows, multiplicandColumns);
    for (unsigned row=1 ; row<=multiplicandRows ; ++row) {
        for (unsigned column=1 ; column<multiplicandColumns ; ++column) {
            Model::Real c = coefficientDistribution(rng);
            multiplicand.update(row, column, c);
        }
    }

    Model::MatrixReal measuredMatrix = multiplier;
    measuredMatrix *= multiplicand;

    QCOMPARE(measuredMatrix.numberRows(), multiplierRows);
    QCOMPARE(measuredMatrix.numberColumns(), multiplicandColumns);

    for (unsigned row=1 ; row<=multiplierRows ; ++row) {
        for (unsigned column=1 ; column<=multiplicandColumns ; ++column) {
            Model::Real expected(0);
            for (unsigned multiplierColumn=1 ; multiplierColumn<=multiplierColumns ; ++multiplierColumn) {
                expected += multiplier(row, multiplierColumn) * multiplicand(multiplierColumn, column);
            }

            Model::Real measured      = measuredMatrix(row, column);
            Model::Real difference    = expected - measured;
            Model::Real relativeError = std::abs(difference / expected);

            if (relativeError > maximumError) {
                QCOMPARE(relativeError < maximumError, true);
            }
        }
    }

    Model::MatrixReal m1(10, 11);
    Model::MatrixReal m2(12, 13);

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


void TestMatrixReal::testMatrixScalarMultiplyAssignmentOperators() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E10, +1E10);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);
    Model::Real                                 maximumError = 5.0E-16;

    unsigned multiplierRows    = dimensionDistribution(rng);
    unsigned multiplierColumns = dimensionDistribution(rng);

    Model::MatrixReal multiplier(multiplierRows, multiplierColumns);
    for (unsigned row=1 ; row<=multiplierRows ; ++row) {
        for (unsigned column=1 ; column<multiplierColumns ; ++column) {
            Model::Real c = coefficientDistribution(rng);
            multiplier.update(row, column, c);
        }
    }

    Model::MatrixReal booleanTrueMeasured = multiplier;
    booleanTrueMeasured  *= Model::Boolean(true);

    Model::MatrixReal booleanFalseMeasured = multiplier;
    booleanFalseMeasured *= Model::Boolean(false);

    Model::Integer       integerScalar   = dimensionDistribution(rng);
    Model::MatrixReal integerMeasured = multiplier;
    integerMeasured *= integerScalar;

    Model::Real          realScalar   = coefficientDistribution(rng);
    Model::MatrixReal realMeasured = multiplier;
    realMeasured *= realScalar;

    QCOMPARE(booleanTrueMeasured.numberRows(), multiplierRows);
    QCOMPARE(booleanTrueMeasured.numberColumns(), multiplierColumns);

    QCOMPARE(booleanFalseMeasured.numberRows(), multiplierRows);
    QCOMPARE(booleanFalseMeasured.numberColumns(), multiplierColumns);

    QCOMPARE(integerMeasured.numberRows(), multiplierRows);
    QCOMPARE(integerMeasured.numberColumns(), multiplierColumns);

    QCOMPARE(realMeasured.numberRows(), multiplierRows);
    QCOMPARE(realMeasured.numberColumns(), multiplierColumns);

    Model::Real maximumRelativeError = 0;
    for (unsigned row=1 ; row<=multiplierRows ; ++row) {
        for (unsigned column=1 ; column<=multiplierColumns ; ++column) {
            QCOMPARE(booleanTrueMeasured(row, column), multiplier(row, column));
            QCOMPARE(booleanFalseMeasured(row, column), Model::Real(0));

            Model::Real integerExpected   = multiplier(row, column) * integerScalar;
            Model::Real integerDifference = integerMeasured(row, column) - integerExpected;
            Model::Real relativeError     = std::abs(integerDifference / integerExpected);

            if (relativeError > maximumError) {
                QCOMPARE(relativeError <= maximumError, true);
            }

            if (relativeError > maximumRelativeError) {
                maximumRelativeError = relativeError;
            }

            Model::Real realExpected   = multiplier(row, column) * realScalar;
            Model::Real realDifference = realMeasured(row, column) - realExpected;
            relativeError = std::abs(realDifference / realExpected);

            if (relativeError > maximumError) {
                QCOMPARE(relativeError <= maximumError, true);
            }

            if (relativeError > maximumRelativeError) {
                maximumRelativeError = relativeError;
            }
        }
    }

    qDebug() << "maximum relative error = " << maximumRelativeError;
}


void TestMatrixReal::testDivideAssignmentOperators() {
    // Divide operators are functionally identical to the multiply operators so we only do limited testing.

    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E10, +1E10);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);

    unsigned dividendRows    = dimensionDistribution(rng);
    unsigned dividendColumns = dimensionDistribution(rng);

    Model::MatrixReal dividend(dividendRows, dividendColumns);
    for (unsigned row=1 ; row<=dividendRows ; ++row) {
        for (unsigned column=1 ; column<dividendColumns ; ++column) {
            Model::Real c = coefficientDistribution(rng);
            dividend.update(row, column, c);
        }
    }

    Model::Integer       integerScalar   = dimensionDistribution(rng);
    Model::MatrixReal integerMeasured = dividend;
    integerMeasured /= integerScalar;

    Model::Real          realScalar   = coefficientDistribution(rng);
    Model::MatrixReal realMeasured = dividend;
    realMeasured /= realScalar;

    QCOMPARE(integerMeasured.numberRows(), dividendRows);
    QCOMPARE(integerMeasured.numberColumns(), dividendColumns);

    QCOMPARE(realMeasured.numberRows(), dividendRows);
    QCOMPARE(realMeasured.numberColumns(), dividendColumns);

    Model::Real maximumError = 0;
    for (unsigned row=1 ; row<=dividendRows ; ++row) {
        for (unsigned column=1 ; column<=dividendColumns ; ++column) {
            Model::Real integerExpected      = dividend(row, column) / integerScalar;
            Model::Real integerDifference    = integerMeasured(row, column) - integerExpected;
            Model::Real integerRelativeError = std::abs(integerDifference / integerExpected);

            if (integerRelativeError >= 1.0E-11) {
                QCOMPARE(integerRelativeError < 1.0E-15, true);
            }

            Model::Real realExpected = dividend(row, column) / realScalar;
            Model::Real realDifference    = realMeasured(row, column) - realExpected;
            Model::Real realRelativeError = std::abs(realDifference / realExpected);

            if (realRelativeError >= 1.0E-11) {
                QCOMPARE(realRelativeError < 1.0E-15, true);
            }


            maximumError = std::max(maximumError, std::max(integerRelativeError, realRelativeError));
        }
    }

    qDebug() << "maximum error = " << maximumError;
}


void TestMatrixReal::testAdditionOperators() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E10, +1E10);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);
    Model::Real                                 maximumAllowedRelativeError = 1E-9;
    Model::Real                                 maximumRelativeError        = 0;
    Model::Real                                 maximumAllowedAbsoluteError = 1E-15;
    Model::Real                                 maximumAbsoluteError        = 0;

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned numberRows      = dimensionDistribution(rng);
        unsigned numberColumns   = dimensionDistribution(rng);

        Model::MatrixReal augend(numberRows, numberColumns);
        Model::MatrixReal addend(numberRows, numberColumns);

        Model::MatrixReal expected1(numberRows, numberColumns);
        Model::MatrixReal expected2(numberColumns, numberRows);

        for (unsigned long columnIndex=1 ; columnIndex<=numberColumns ; ++columnIndex) {
            for (unsigned long rowIndex=1 ; rowIndex<=numberRows ; ++rowIndex) {
                Model::Real a1 = coefficientDistribution(rng);
                Model::Real a2 = coefficientDistribution(rng);

                augend.update(rowIndex, columnIndex, a1);
                addend.update(rowIndex, columnIndex, a2);

                expected1.update(rowIndex, columnIndex, a1 + a2);
                expected2.update(columnIndex, rowIndex, 2 * a1 + 3 * a2);
            }
        }

        Model::MatrixReal measured1 = augend + addend;
        Model::MatrixReal measured2 = 2 * augend.transpose() + 3 * addend.transpose();

        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                Model::Real e1 = expected1(row, column);
                Model::Real m1 = measured1(row, column);
                Model::Real e2 = expected2(column, row);
                Model::Real m2 = measured2(column, row);

                if (e1 != Model::Real(0)) {
                    Model::Real difference    = e1 - m1;
                    Model::Real relativeError = std::abs(difference / e1);

                    if (relativeError > maximumAllowedRelativeError) {
                        QCOMPARE(relativeError < maximumAllowedRelativeError, true);
                    }

                    if (relativeError > maximumRelativeError) {
                        maximumRelativeError = relativeError;
                    }
                } else {
                    Model::Real absoluteError = std::abs(m1);

                    if (absoluteError > maximumAllowedAbsoluteError) {
                        QCOMPARE(absoluteError < maximumAllowedAbsoluteError, true);
                    }

                    if (absoluteError > maximumAbsoluteError) {
                        maximumAbsoluteError = absoluteError;
                    }
                }

                if (e2 != Model::Real(0)) {
                    Model::Real difference    = e2 - m2;
                    Model::Real relativeError = std::abs(difference / e2);

                    if (relativeError > maximumAllowedRelativeError) {
                        QCOMPARE(relativeError < maximumAllowedRelativeError, true);
                    }

                    if (relativeError > maximumRelativeError) {
                        maximumRelativeError = relativeError;
                    }
                } else {
                    Model::Real absoluteError = std::abs(m2);

                    if (absoluteError > maximumAllowedAbsoluteError) {
                        QCOMPARE(absoluteError < maximumAllowedAbsoluteError, true);
                    }

                    if (absoluteError > maximumAbsoluteError) {
                        maximumAbsoluteError = absoluteError;
                    }
                }
            }
        }
    }

    qDebug() << "maximum relative error = " << maximumRelativeError;

    Model::MatrixReal m1(10, 11);
    Model::MatrixReal m2(12, 13);
    Model::MatrixReal s;

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


void TestMatrixReal::testSubtractionOperators() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E10, +1E10);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);
    Model::Real                                 maximumAllowedRelativeError = 1E-9;
    Model::Real                                 maximumRelativeError        = 0;
    Model::Real                                 maximumAllowedAbsoluteError = 1E-15;
    Model::Real                                 maximumAbsoluteError        = 0;

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned numberRows      = dimensionDistribution(rng);
        unsigned numberColumns   = dimensionDistribution(rng);

        Model::MatrixReal augend(numberRows, numberColumns);
        Model::MatrixReal addend(numberRows, numberColumns);

        Model::MatrixReal expected1(numberRows, numberColumns);
        Model::MatrixReal expected2(numberColumns, numberRows);

        for (unsigned long columnIndex=1 ; columnIndex<=numberColumns ; ++columnIndex) {
            for (unsigned long rowIndex=1 ; rowIndex<=numberRows ; ++rowIndex) {
                Model::Real a1 = coefficientDistribution(rng);
                Model::Real a2 = coefficientDistribution(rng);

                augend.update(rowIndex, columnIndex, a1);
                addend.update(rowIndex, columnIndex, a2);

                expected1.update(rowIndex, columnIndex, a1 - a2);
                expected2.update(columnIndex, rowIndex, 2 * a1 - 3 * a2);
            }
        }

        Model::MatrixReal measured1 = augend - addend;
        Model::MatrixReal measured2 = 2 * augend.transpose() - 3 * addend.transpose();

        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                Model::Real e1 = expected1(row, column);
                Model::Real m1 = measured1(row, column);
                Model::Real e2 = expected2(column, row);
                Model::Real m2 = measured2(column, row);

                if (e1 != Model::Real(0)) {
                    Model::Real difference    = e1 - m1;
                    Model::Real relativeError = std::abs(difference / e1);

                    if (relativeError > maximumAllowedRelativeError) {
                        QCOMPARE(relativeError < maximumAllowedRelativeError, true);
                    }

                    if (relativeError > maximumRelativeError) {
                        maximumRelativeError = relativeError;
                    }
                } else {
                    Model::Real absoluteError = std::abs(m1);

                    if (absoluteError > maximumAllowedAbsoluteError) {
                        QCOMPARE(absoluteError < maximumAllowedAbsoluteError, true);
                    }

                    if (absoluteError > maximumAbsoluteError) {
                        maximumAbsoluteError = absoluteError;
                    }
                }

                if (e2 != Model::Real(0)) {
                    Model::Real difference    = e2 - m2;
                    Model::Real relativeError = std::abs(difference / e2);

                    if (relativeError > maximumAllowedRelativeError) {
                        QCOMPARE(relativeError < maximumAllowedRelativeError, true);
                    }

                    if (relativeError > maximumRelativeError) {
                        maximumRelativeError = relativeError;
                    }
                } else {
                    Model::Real absoluteError = std::abs(m2);

                    if (absoluteError > maximumAllowedAbsoluteError) {
                        QCOMPARE(absoluteError < maximumAllowedAbsoluteError, true);
                    }

                    if (absoluteError > maximumAbsoluteError) {
                        maximumAbsoluteError = absoluteError;
                    }
                }
            }
        }
    }

    qDebug() << "maximum relative error = " << maximumRelativeError;

    Model::MatrixReal m1(10, 11);
    Model::MatrixReal m2(12, 13);
    Model::MatrixReal s;

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


void TestMatrixReal::testMatrixMatrixMultiplicationOperator() {
    std::mt19937 rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E10, +1E10);

    // We test three scenarios, the larger the matrix, the fewer times we must iterate to keep the test time
    // reasonable.
    unsigned mode4Iterations = std::max(1U, TestMatrixReal::numberIterations / 1000);
    unsigned mode3Iterations = std::max(1U, TestMatrixReal::numberIterations / 200);
    unsigned mode2Iterations = std::max(1U, TestMatrixReal::numberIterations / 100);
    unsigned mode1Iterations = std::max(1U, TestMatrixReal::numberIterations /  10);
    unsigned mode0Iterations = (
          TestMatrixReal::numberIterations
        - mode1Iterations
        - mode2Iterations
        - mode3Iterations
        - mode4Iterations
    );

    for (unsigned mode=0 ; mode<4 ; ++mode) {
        unsigned    maximumDimension = 0;
        unsigned    numberIterations = 0;
        Model::Real maximumError     = 0;

        switch (mode) {
            case 0: {
                maximumDimension = 100;
                numberIterations = mode0Iterations;
                maximumError     = 1.0E-10;

                break;
            }

            case 1: {
                maximumDimension = 200;
                numberIterations = mode1Iterations;
                maximumError     = 4.0E-10;

                break;
            }

            case 2: {
                maximumDimension  = 500;
                numberIterations  = mode2Iterations;
                maximumError      = 1.0E-9;

                break;
            }


            case 3: {
                maximumDimension  = 1000;
                numberIterations  = mode2Iterations;
                maximumError      = 5.0E-9;

                break;
            }

            case 4: {
                maximumDimension = 10000;
                numberIterations = mode4Iterations;
                maximumError      = 1.0E-8;

                break;
            }

            default: {
                Q_ASSERT(false);
                break;
            }
        }

        std::uniform_int_distribution<> dimensionDistribution(1, maximumDimension);
        Model::Real                     maximumMeasuredError = 0;

        for (unsigned i=0 ; i<numberIterations ; ++i) {
            unsigned multiplierRows      = dimensionDistribution(rng);
            unsigned multiplierColumns   = dimensionDistribution(rng);
            unsigned multiplicandRows    = multiplierColumns;
            unsigned multiplicandColumns = dimensionDistribution(rng);

            Model::MatrixReal multiplier(multiplierRows, multiplierColumns);
            for (unsigned row=1 ; row<=multiplierRows ; ++row) {
                for (unsigned column=1 ; column<multiplierColumns ; ++column) {
                    Model::Real c = coefficientDistribution(rng);
                    multiplier.update(row, column, c);
                }
            }

            Model::MatrixReal multiplicand(multiplicandRows, multiplicandColumns);
            for (unsigned row=1 ; row<=multiplicandRows ; ++row) {
                for (unsigned column=1 ; column<multiplicandColumns ; ++column) {
                    Model::Real c = coefficientDistribution(rng);
                    multiplicand.update(row, column, c);
                }
            }

            Model::MatrixReal measuredMatrix = multiplier * multiplicand;

            QCOMPARE(measuredMatrix.numberRows(), multiplierRows);
            QCOMPARE(measuredMatrix.numberColumns(), multiplicandColumns);

            for (unsigned row=1 ; row<=multiplierRows ; ++row) {
                for (unsigned column=1 ; column<=multiplicandColumns ; ++column) {
                    Model::Real expected(0);
                    for (unsigned multiplierColumn=1 ; multiplierColumn<=multiplierColumns ; ++multiplierColumn) {
                        expected += multiplier(row, multiplierColumn) * multiplicand(multiplierColumn, column);
                    }

                    Model::Real measured      = measuredMatrix(row, column);
                    Model::Real error         = expected - measured;
                    Model::Real relativeError = std::abs(error / expected);

                    if (relativeError > maximumError) {
                        QCOMPARE(relativeError < maximumError, true);
                    }

                    if (relativeError > maximumMeasuredError) {
                        maximumMeasuredError = relativeError;
                    }
                }
            }
        }

        qDebug() << "Matrix size " << maximumDimension << " x " << maximumDimension
                 << ": maximum measured error = " << maximumMeasuredError
                 << " (over " << numberIterations << " iterations)";
    }

    Model::MatrixReal m1(10, 11);
    Model::MatrixReal m2(12, 13);
    Model::MatrixReal p;

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


void TestMatrixReal::testMatrixScalarMultiplicationOperators() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E10, +1E10);
    std::uniform_int_distribution<>             dimensionDistribution(1, 100);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned multiplierRows    = dimensionDistribution(rng);
        unsigned multiplierColumns = dimensionDistribution(rng);

        Model::MatrixReal multiplier(multiplierRows, multiplierColumns);
        for (unsigned row=1 ; row<=multiplierRows ; ++row) {
            for (unsigned column=1 ; column<multiplierColumns ; ++column) {
                Model::Real c = coefficientDistribution(rng);
                multiplier.update(row, column, c);
            }
        }

        Model::Boolean booleanScalar = (dimensionDistribution(rng) % 2) != 0;
        Model::Integer integerScalar = dimensionDistribution(rng);
        Model::Real    realScalar    = coefficientDistribution(rng);

        bool tm = (dimensionDistribution(rng) % 2) != 0;

        Model::MatrixReal booleanMeasured  = tm ? multiplier * booleanScalar : booleanScalar * multiplier;
        Model::MatrixReal integerMeasured  = tm ? multiplier * integerScalar : integerScalar * multiplier;
        Model::MatrixReal realMeasured     = tm ? multiplier * realScalar    : realScalar * multiplier;

        QCOMPARE(booleanMeasured.numberRows(), multiplierRows);
        QCOMPARE(booleanMeasured.numberColumns(), multiplierColumns);

        QCOMPARE(integerMeasured.numberRows(), multiplierRows);
        QCOMPARE(integerMeasured.numberColumns(), multiplierColumns);

        QCOMPARE(realMeasured.numberRows(), multiplierRows);
        QCOMPARE(realMeasured.numberColumns(), multiplierColumns);

        for (unsigned row=1 ; row<=multiplierRows ; ++row) {
            for (unsigned column=1 ; column<=multiplierColumns ; ++column) {
                QCOMPARE(booleanMeasured(row, column), booleanScalar ? multiplier(row, column) : Model::Real(0));

                Model::Real integerExpected = multiplier(row, column) * integerScalar;
                QCOMPARE(integerMeasured(row, column), integerExpected);

                Model::Real realExpected = multiplier(row, column) * realScalar;
                QCOMPARE(realMeasured(row, column), realExpected);
            }
        }
    }
}


void TestMatrixReal::testDivisionOperators() {
    // Divide operators are functionally identical to the multiply operators so we only do limited testing.

    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E10, +1E10);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);

    unsigned dividendRows    = dimensionDistribution(rng);
    unsigned dividendColumns = dimensionDistribution(rng);

    Model::MatrixReal dividend(dividendRows, dividendColumns);
    for (unsigned row=1 ; row<=dividendRows ; ++row) {
        for (unsigned column=1 ; column<dividendColumns ; ++column) {
            Model::Real c = coefficientDistribution(rng);
            dividend.update(row, column, c);
        }
    }

    Model::Integer       integerScalar   = dimensionDistribution(rng);
    Model::MatrixReal integerMeasured = dividend / integerScalar;

    Model::Real          realScalar   = coefficientDistribution(rng);
    Model::MatrixReal realMeasured = dividend / realScalar;

    QCOMPARE(integerMeasured.numberRows(), dividendRows);
    QCOMPARE(integerMeasured.numberColumns(), dividendColumns);

    QCOMPARE(realMeasured.numberRows(), dividendRows);
    QCOMPARE(realMeasured.numberColumns(), dividendColumns);

    Model::Real maximumError = 0;
    for (unsigned row=1 ; row<=dividendRows ; ++row) {
        for (unsigned column=1 ; column<=dividendColumns ; ++column) {
            Model::Real integerExpected      = dividend(row, column) / integerScalar;
            Model::Real integerDifference    = integerMeasured(row, column) - integerExpected;
            Model::Real integerRelativeError = std::abs(integerDifference / integerExpected);

            if (integerRelativeError >= 1.0E-11) {
                QCOMPARE(integerRelativeError < 1.0E-15, true);
            }

            Model::Real realExpected      = dividend(row, column) / realScalar;
            Model::Real realDifference    = realMeasured(row, column) - realExpected;
            Model::Real realRelativeError = std::abs(realDifference / realExpected);

            if (realRelativeError >= 1.0E-11) {
                QCOMPARE(realRelativeError < 1.0E-15, true);
            }

            maximumError = std::max(maximumError, std::max(integerRelativeError, realRelativeError));
        }
    }

    qDebug() << "maximum error = " << maximumError;
}


void TestMatrixReal::testUnaryPlusMinusOperators() {
    Model::MatrixReal e = Model::MatrixReal::build(
        3, 3,
        Model::Real( 1), Model::Real(-4), Model::Real( 7),
        Model::Real(-2), Model::Real( 5), Model::Real(-8),
        Model::Real( 3), Model::Real(-6), Model::Real( 9)
    );

    Model::MatrixReal p = +e;
    Model::MatrixReal m = -e;

    for (unsigned row=1 ; row<=3 ; ++row) {
        for (unsigned column=1 ; column<=3 ; ++column) {
            QCOMPARE(p(row, column), e(row, column));
            QCOMPARE(m(row, column), -e(row, column));
        }
    }
}


void TestMatrixReal::testComparisonOperators() {
    Model::MatrixReal m1 = Model::MatrixReal::build(
        3, 3,
        Model::Real( 1), Model::Real(-4), Model::Real( 7),
        Model::Real(-2), Model::Real( 5), Model::Real(-8),
        Model::Real( 3), Model::Real(-6), Model::Real( 9)
    );

    Model::MatrixReal m2 = Model::MatrixReal::build(
        2, 3,
        Model::Real(10), Model::Real(13),
        Model::Real(11), Model::Real(14),
        Model::Real(12), Model::Real(15)
    );

    Model::MatrixReal m3 = Model::MatrixReal::build(
        3, 2,
        Model::Real(10), Model::Real(12), Model::Real(14),
        Model::Real(11), Model::Real(13), Model::Real(15)
    );

    Model::MatrixReal m4 = Model::MatrixReal::build( // same as m1
        3, 3,
        Model::Real( 1), Model::Real(-4), Model::Real( 7),
        Model::Real(-2), Model::Real( 5), Model::Real(-8),
        Model::Real( 3), Model::Real(-6), Model::Real( 9)
    );

    Model::MatrixReal m5 = Model::MatrixReal::build( // one value different from m1
        3, 3,
        Model::Real( 1), Model::Real(-4), Model::Real( 7),
        Model::Real(-2), Model::Real(15), Model::Real(-8),
        Model::Real( 3), Model::Real(-6), Model::Real( 9)
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


void TestMatrixReal::testSubscriptingOperators() {
//    // Tested by other methods.
}


void TestMatrixReal::testMultiplicationFunctions() {
//    // Tested by TestMatrixReal::testMatrixScalarMultiplicationOperators
}


void TestMatrixReal::testIterator() {
    M::MatrixReal m = M::MatrixReal::build(
        3, 3,
        Model::Real( 1), Model::Real( 7), Model::Real(13),
        Model::Real( 3), Model::Real( 9), Model::Real(15),
        Model::Real( 5), Model::Real(11), Model::Real(17)
    );

    M::MatrixReal::Iterator it = m.begin();
    QCOMPARE(it != m.end(), true);
    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);

    QCOMPARE(it.value(), Model::Real(1));
    QCOMPARE(it.constReference(), Model::Real(1));
    QCOMPARE(*(it.constPointer()), Model::Real(1));
    QCOMPARE(*it, Model::Real(1));

    ++it;
    QCOMPARE(it != m.end(), true);
    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(*it, Model::Real(3));

    M::MatrixReal::Iterator it2 = it++;
    QCOMPARE(*it2, Model::Real(3));

    QCOMPARE(it != m.end(), true);
    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(*it, Model::Real(5));

    it.advance(2);

    QCOMPARE(it != m.end(), true);
    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(*it, Model::Real(9));

    it.advance(2);

    QCOMPARE(it != m.end(), true);
    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(*it, Model::Real(13));

    it.advance(2);

    QCOMPARE(it != m.end(), true);
    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(*it, Model::Real(17));

    ++it;

    QCOMPARE(it == m.end(), true);
    QCOMPARE(it.isValid(), false);
    QCOMPARE(it.isInvalid(), true);
}


void TestMatrixReal::dumpMatrix(const Model::MatrixReal& matrix) {
    unsigned long numberRows = matrix.numberRows();
    unsigned long numberColumns = matrix.numberColumns();

    for (unsigned long row=1 ; row<=numberRows ; ++row) {
        QString t;
        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            if (!t.isEmpty()) {
                t +="  |  ";
            }

            Model::Real v = matrix(row, column);
            QString     s =QString("%1").arg(v, 8, 'g').trimmed();
            t += QString("%1").arg(s, 20);
        }

        qDebug() << t;
    }
}
