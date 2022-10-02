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
* This file implements tests of the \ref Model::MatrixComplex class.
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

#include <model_complex.h>
#include <model_exceptions.h>
#include <model_tuple.h>
#include <m_basic_functions.h>
#include <model_matrix_boolean.h>
#include <model_matrix_integer.h>
#include <model_matrix_real.h>
#include <model_matrix_complex.h>
#include <m_matrix_complex.h>
#include <m_variant.h>
#include <m_range.h>
#include <m_set.h>
#include <m_tuple.h>

#include "test_matrix_base.h"
#include "test_matrix_complex.h"

TestMatrixComplex::TestMatrixComplex() {}


TestMatrixComplex::~TestMatrixComplex() {}


void TestMatrixComplex::testConstructorAndDestructors() {
    Model::MatrixComplex m1;
    QCOMPARE(m1.numberRows(), 0);
    QCOMPARE(m1.numberColumns(), 0);

    Model::MatrixComplex m2(3, 3);
    QCOMPARE(m2.numberRows(), 3);
    QCOMPARE(m2.numberColumns(), 3);

    QCOMPARE(m2(1,1), Model::Complex(0));
    QCOMPARE(m2(1,2), Model::Complex(0));
    QCOMPARE(m2(1,3), Model::Complex(0));
    QCOMPARE(m2(2,1), Model::Complex(0));
    QCOMPARE(m2(2,2), Model::Complex(0));
    QCOMPARE(m2(2,3), Model::Complex(0));
    QCOMPARE(m2(3,1), Model::Complex(0));
    QCOMPARE(m2(3,2), Model::Complex(0));
    QCOMPARE(m2(3,3), Model::Complex(0));

    m2.update(1, 1, Model::Complex(1,1));
    m2.update(1, 2, Model::Complex(1,2));
    m2.update(1, 3, Model::Complex(1,3));
    m2.update(2, 1, Model::Complex(2,1));
    m2.update(2, 2, Model::Complex(2,2));
    m2.update(2, 3, Model::Complex(2,3));
    m2.update(3, 1, Model::Complex(3,1));
    m2.update(3, 2, Model::Complex(3,2));
    m2.update(3, 3, Model::Complex(3,3));

    const Model::Complex matrixData[] = {
        Model::Complex(1, 1), Model::Complex(2, 1),
        Model::Complex(1, 2), Model::Complex(2, 2)
    };

    Model::MatrixComplex m3(2, 2, matrixData);
    QCOMPARE(m3.numberRows(), 2);
    QCOMPARE(m3.numberColumns(), 2);

    QCOMPARE(m3(1,1), Model::Complex(1, 1));
    QCOMPARE(m3(1,2), Model::Complex(1, 2));
    QCOMPARE(m3(2,1), Model::Complex(2, 1));
    QCOMPARE(m3(2,2), Model::Complex(2, 2));

    Model::MatrixComplex m4(m2);
    QCOMPARE(m4.numberRows(), 3);
    QCOMPARE(m4.numberColumns(), 3);

    QCOMPARE(m4(1,1), Model::Complex(1,1));
    QCOMPARE(m4(1,2), Model::Complex(1,2));
    QCOMPARE(m4(1,3), Model::Complex(1,3));
    QCOMPARE(m4(2,1), Model::Complex(2,1));
    QCOMPARE(m4(2,2), Model::Complex(2,2));
    QCOMPARE(m4(2,3), Model::Complex(2,3));
    QCOMPARE(m4(3,1), Model::Complex(3,1));
    QCOMPARE(m4(3,2), Model::Complex(3,2));
    QCOMPARE(m4(3,3), Model::Complex(3,3));

    Model::MatrixReal r = Model::MatrixReal::build(
        2, 2,
        Model::Real(1), Model::Real(3),
        Model::Real(2), Model::Real(4)
    );

    Model::MatrixComplex rc(2.0 * r.transpose());
    QCOMPARE(
        rc,
        Model::MatrixComplex::build(
            2, 2,
            Model::Complex(2, 0), Model::Complex(4, 0),
            Model::Complex(6, 0), Model::Complex(8, 0)
        )
    );

    Model::MatrixInteger i = Model::MatrixInteger::build(
        2, 2,
        Model::Integer(1), Model::Integer(2),
        Model::Integer(3), Model::Integer(4)
    );

    Model::MatrixComplex ic(2 * i.transpose());
    QCOMPARE(
        ic,
        Model::MatrixComplex::build(
            2, 2,
            Model::Complex(2, 0), Model::Complex(6, 0),
            Model::Complex(4, 0), Model::Complex(8, 0)
        )
    );

    Model::MatrixBoolean b = Model::MatrixBoolean::build(
        2, 2,
        true, false,
        true, true
    );

    Model::MatrixComplex bc(b.transpose());
    QCOMPARE(
        bc,
        Model::MatrixComplex::build(
            2, 2,
            Model::Complex(1), Model::Complex(1),
            Model::Complex(0), Model::Complex(1)
        )
    );
}


void TestMatrixComplex::testCoefficientValueType() {
    Model::MatrixComplex m;
    QCOMPARE(m.coefficientValueType(), Model::ValueType::COMPLEX);
}


void TestMatrixComplex::testNumberRowsColumns() {
    Model::MatrixComplex m1(2, 3);

    QCOMPARE(m1.numberRows(), 2);
    QCOMPARE(m1.numberColumns(), 3);
    QCOMPARE(m1.numberCoefficients(), 6);
}


void TestMatrixComplex::testValueMethods() {
    Model::MatrixComplex m1;

    bool success = m1.setValue(1, 1, Model::Integer(1));
    QVERIFY(success);

    Model::Variant v11 = m1.value(1, 1);
    QCOMPARE(v11.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(v11, Model::Variant(Model::Integer(1)));

    success = m1.setValue(1, 2, Model::Real(1.5));
    QVERIFY(success);

    v11 = m1.value(1, 1);
    Model::Variant v12 = m1.value(1, 2);

    QCOMPARE(v11.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(v12.valueType(), Model::ValueType::COMPLEX);

    QCOMPARE(v11, Model::Variant(Model::Integer(1)));
    QCOMPARE(v12, Model::Variant(Model::Real(1.5)));

    success = m1.setValue(2, 1, Model::Complex(1.5, 2.5));
    QVERIFY(success);

    v11 = m1.value(1, 1);
    v12 = m1.value(1, 2);
    Model::Variant v21 = m1.value(2, 1);

    QCOMPARE(v11.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(v12.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(v21.valueType(), Model::ValueType::COMPLEX);

    QCOMPARE(v11, Model::Variant(Model::Integer(1)));
    QCOMPARE(v12, Model::Variant(Model::Real(1.5)));
    QCOMPARE(v21, Model::Variant(Model::Complex(1.5, 2.5)));

    success = m1.setValue(4, Model::Boolean(true));
    QVERIFY(!success);

    success = m1.setValue(4, Model::Complex(2.5, 3.5));
    QVERIFY(success);

    QCOMPARE(m1.numberRows(), 2);
    QCOMPARE(m1.numberColumns(), 2);

    v11 = m1.value(1, 1);
    v12 = m1.value(1, 2);
    v21 = m1.value(2, 1);
    Model::Variant v22 = m1.value(2, 2);

    QCOMPARE(v11.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(v12.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(v21.valueType(), Model::ValueType::COMPLEX);
    QCOMPARE(v22.valueType(), Model::ValueType::COMPLEX);

    QCOMPARE(v11, Model::Variant(Model::Integer(1)));
    QCOMPARE(v12, Model::Variant(Model::Real(1.5)));
    QCOMPARE(v21, Model::Variant(Model::Complex(1.5, 2.5)));
    QCOMPARE(v22, Model::Variant(Model::Complex(2.5, 3.5)));

    QCOMPARE(m1.value(1), Model::Variant(Model::Integer(1)));
    QCOMPARE(m1.value(2), Model::Variant(Model::Real(1.5)));
    QCOMPARE(m1.value(3), Model::Variant(Model::Complex(1.5, 2.5)));
    QCOMPARE(m1.value(4), Model::Variant(Model::Complex(2.5, 3.5)));
}


void TestMatrixComplex::testInternalAtMethods() {
    M::MatrixComplex m(9, 9);
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
    QCOMPARE(m.at(i, rng), M::MatrixComplex::build(1, 2, M::Integer(35), M::Integer(36)));
    QCOMPARE(m.at(i, set), M::MatrixComplex::build(1, 2, M::Integer(32), M::Integer(34)));
    QCOMPARE(m.at(i, tuple), M::MatrixComplex::build(1, 2, M::Integer(33), M::Integer(35)));
    QCOMPARE(
        m.at(i, mi),
        M::MatrixComplex::build(1, 4, M::Integer(32), M::Integer(34), M::Integer(36), M::Integer(38))
    );
    QCOMPARE(
        m.at(i, mr),
        M::MatrixComplex::build(1, 4, M::Integer(31), M::Integer(32), M::Integer(33), M::Integer(34))
    );
    QCOMPARE(m.at(i, mc), M::MatrixComplex::build(1, 2, M::Integer(31), M::Integer(35)));

    QCOMPARE(m.at(r, i), M::Real(43));
    QCOMPARE(m.at(r, r), M::Real(44));
    QCOMPARE(m.at(r, c), M::Real(45));
    QCOMPARE(m.at(r, rng), M::MatrixComplex::build(1, 2, M::Integer(45), M::Integer(46)));
    QCOMPARE(m.at(r, set), M::MatrixComplex::build(1, 2, M::Integer(42), M::Integer(44)));
    QCOMPARE(m.at(r, tuple), M::MatrixComplex::build(1, 2, M::Integer(43), M::Integer(45)));
    QCOMPARE(
        m.at(r, mi),
        M::MatrixComplex::build(1, 4, M::Integer(42), M::Integer(44), M::Integer(46), M::Integer(48))
    );
    QCOMPARE(
        m.at(r, mr),
        M::MatrixComplex::build(1, 4, M::Integer(41), M::Integer(42), M::Integer(43), M::Integer(44))
    );
    QCOMPARE(m.at(r, mc), M::MatrixComplex::build(1, 2, M::Integer(41), M::Integer(45)));

    QCOMPARE(m.at(c, i), M::Real(53));
    QCOMPARE(m.at(c, r), M::Real(54));
    QCOMPARE(m.at(c, c), M::Real(55));
    QCOMPARE(m.at(c, rng), M::MatrixComplex::build(1, 2, M::Integer(55), M::Integer(56)));
    QCOMPARE(m.at(c, set), M::MatrixComplex::build(1, 2, M::Integer(52), M::Integer(54)));
    QCOMPARE(m.at(c, tuple), M::MatrixComplex::build(1, 2, M::Integer(53), M::Integer(55)));
    QCOMPARE(
        m.at(c, mi),
        M::MatrixComplex::build(1, 4, M::Integer(52), M::Integer(54), M::Integer(56), M::Integer(58))
    );
    QCOMPARE(
        m.at(c, mr),
        M::MatrixComplex::build(1, 4, M::Integer(51), M::Integer(52), M::Integer(53), M::Integer(54))
    );
    QCOMPARE(m.at(c, mc), M::MatrixComplex::build(1, 2, M::Integer(51), M::Integer(55)));

    QCOMPARE(m.at(rng, i), M::MatrixComplex::build(2, 1, M::Real(53), M::Real(63)));
    QCOMPARE(m.at(rng, r), M::MatrixComplex::build(2, 1, M::Real(54), M::Real(64)));
    QCOMPARE(m.at(rng, c), M::MatrixComplex::build(2, 1, M::Real(55), M::Real(65)));
    QCOMPARE(
        m.at(rng, rng),
        M::MatrixComplex::build(
            2, 2,
            M::Real(55), M::Real(65),
            M::Real(56), M::Real(66)
        )
    );
    QCOMPARE(
        m.at(rng, set),
        M::MatrixComplex::build(
            2, 2,
            M::Real(52), M::Real(62),
            M::Real(54), M::Real(64)
        )
    );
    QCOMPARE(
        m.at(rng, tuple),
        M::MatrixComplex::build(
            2, 2,
            M::Real(53), M::Real(63),
            M::Real(55), M::Real(65)
        )
    );
    QCOMPARE(
        m.at(rng, mi),
        M::MatrixComplex::build(
            2, 4,
            M::Real(52), M::Real(62),
            M::Real(54), M::Real(64),
            M::Real(56), M::Real(66),
            M::Real(58), M::Real(68)
        )
    );
    QCOMPARE(
        m.at(rng, mr),
        M::MatrixComplex::build(
            2, 4,
            M::Real(51), M::Real(61),
            M::Real(52), M::Real(62),
            M::Real(53), M::Real(63),
            M::Real(54), M::Real(64)
        )
    );
    QCOMPARE(
        m.at(rng, mc),
        M::MatrixComplex::build(
            2, 2,
            M::Real(51), M::Real(61),
            M::Real(55), M::Real(65)
        )
    );

    QCOMPARE(m.at(set, i), M::MatrixComplex::build(2, 1, M::Real(23), M::Real(43)));
    QCOMPARE(m.at(set, r), M::MatrixComplex::build(2, 1, M::Real(24), M::Real(44)));
    QCOMPARE(m.at(set, c), M::MatrixComplex::build(2, 1, M::Real(25), M::Real(45)));
    QCOMPARE(
        m.at(set, rng),
        M::MatrixComplex::build(
            2, 2,
            M::Real(25), M::Real(45),
            M::Real(26), M::Real(46)
        )
    );
    QCOMPARE(
        m.at(set, set),
        M::MatrixComplex::build(
            2, 2,
            M::Real(22), M::Real(42),
            M::Real(24), M::Real(44)
        )
    );
    QCOMPARE(
        m.at(set, tuple),
        M::MatrixComplex::build(
            2, 2,
            M::Real(23), M::Real(43),
            M::Real(25), M::Real(45)
        )
    );
    QCOMPARE(
        m.at(set, mi),
        M::MatrixComplex::build(
            2, 4,
            M::Real(22), M::Real(42),
            M::Real(24), M::Real(44),
            M::Real(26), M::Real(46),
            M::Real(28), M::Real(48)
        )
    );
    QCOMPARE(
        m.at(set, mr),
        M::MatrixComplex::build(
            2, 4,
            M::Real(21), M::Real(41),
            M::Real(22), M::Real(42),
            M::Real(23), M::Real(43),
            M::Real(24), M::Real(44)
        )
    );
    QCOMPARE(
        m.at(set, mc),
        M::MatrixComplex::build(
            2, 2,
            M::Real(21), M::Real(41),
            M::Real(25), M::Real(45)
        )
    );

    QCOMPARE(m.at(tuple, i), M::MatrixComplex::build(2, 1, M::Real(33), M::Real(53)));
    QCOMPARE(m.at(tuple, r), M::MatrixComplex::build(2, 1, M::Real(34), M::Real(54)));
    QCOMPARE(m.at(tuple, c), M::MatrixComplex::build(2, 1, M::Real(35), M::Real(55)));
    QCOMPARE(
        m.at(tuple, rng),
        M::MatrixComplex::build(
            2, 2,
            M::Real(35), M::Real(55),
            M::Real(36), M::Real(56)
        )
    );
    QCOMPARE(
        m.at(tuple, set),
        M::MatrixComplex::build(
            2, 2,
            M::Real(32), M::Real(52),
            M::Real(34), M::Real(54)
        )
    );
    QCOMPARE(
        m.at(tuple, tuple),
        M::MatrixComplex::build(
            2, 2,
            M::Real(33), M::Real(53),
            M::Real(35), M::Real(55)
        )
    );
    QCOMPARE(
        m.at(tuple, mi),
        M::MatrixComplex::build(
            2, 4,
            M::Real(32), M::Real(52),
            M::Real(34), M::Real(54),
            M::Real(36), M::Real(56),
            M::Real(38), M::Real(58)
        )
    );
    QCOMPARE(
        m.at(tuple, mr),
        M::MatrixComplex::build(
            2, 4,
            M::Real(31), M::Real(51),
            M::Real(32), M::Real(52),
            M::Real(33), M::Real(53),
            M::Real(34), M::Real(54)
        )
    );
    QCOMPARE(
        m.at(tuple, mc),
        M::MatrixComplex::build(
            2, 2,
            M::Real(31), M::Real(51),
            M::Real(35), M::Real(55)
        )
    );

    QCOMPARE(m.at(mi, i), M::MatrixComplex::build(4, 1, M::Real(23), M::Real(43), M::Real(63), M::Real(83)));
    QCOMPARE(m.at(mi, r), M::MatrixComplex::build(4, 1, M::Real(24), M::Real(44), M::Real(64), M::Real(84)));
    QCOMPARE(m.at(mi, c), M::MatrixComplex::build(4, 1, M::Real(25), M::Real(45), M::Real(65), M::Real(85)));
    QCOMPARE(
        m.at(mi, rng),
        M::MatrixComplex::build(
            4, 2,
            M::Real(25), M::Real(45), M::Real(65), M::Real(85),
            M::Real(26), M::Real(46), M::Real(66), M::Real(86)
        )
    );
    QCOMPARE(
        m.at(mi, set),
        M::MatrixComplex::build(
            4, 2,
            M::Real(22), M::Real(42), M::Real(62), M::Real(82),
            M::Real(24), M::Real(44), M::Real(64), M::Real(84)
        )
    );
    QCOMPARE(
        m.at(mi, tuple),
        M::MatrixComplex::build(
            4, 2,
            M::Real(23), M::Real(43), M::Real(63), M::Real(83),
            M::Real(25), M::Real(45), M::Real(65), M::Real(85)
        )
    );
    QCOMPARE(
        m.at(mi, mi),
        M::MatrixComplex::build(
            4, 4,
            M::Real(22), M::Real(42), M::Real(62), M::Real(82),
            M::Real(24), M::Real(44), M::Real(64), M::Real(84),
            M::Real(26), M::Real(46), M::Real(66), M::Real(86),
            M::Real(28), M::Real(48), M::Real(68), M::Real(88)
        )
    );
    QCOMPARE(
        m.at(mi, mr),
        M::MatrixComplex::build(
            4, 4,
            M::Real(21), M::Real(41), M::Real(61), M::Real(81),
            M::Real(22), M::Real(42), M::Real(62), M::Real(82),
            M::Real(23), M::Real(43), M::Real(63), M::Real(83),
            M::Real(24), M::Real(44), M::Real(64), M::Real(84)
        )
    );
    QCOMPARE(
        m.at(mi, mc),
        M::MatrixComplex::build(
            4, 2,
            M::Real(21), M::Real(41), M::Real(61), M::Real(81),
            M::Real(25), M::Real(45), M::Real(65), M::Real(85)
        )
    );

    QCOMPARE(m.at(mr, i), M::MatrixComplex::build(4, 1, M::Real(13), M::Real(23), M::Real(33), M::Real(43)));
    QCOMPARE(m.at(mr, r), M::MatrixComplex::build(4, 1, M::Real(14), M::Real(24), M::Real(34), M::Real(44)));
    QCOMPARE(m.at(mr, c), M::MatrixComplex::build(4, 1, M::Real(15), M::Real(25), M::Real(35), M::Real(45)));
    QCOMPARE(
        m.at(mr, rng),
        M::MatrixComplex::build(
            4, 2,
            M::Real(15), M::Real(25), M::Real(35), M::Real(45),
            M::Real(16), M::Real(26), M::Real(36), M::Real(46)
        )
    );
    QCOMPARE(
        m.at(mr, set),
        M::MatrixComplex::build(
            4, 2,
            M::Real(12), M::Real(22), M::Real(32), M::Real(42),
            M::Real(14), M::Real(24), M::Real(34), M::Real(44)
        )
    );
    QCOMPARE(
        m.at(mr, tuple),
        M::MatrixComplex::build(
            4, 2,
            M::Real(13), M::Real(23), M::Real(33), M::Real(43),
            M::Real(15), M::Real(25), M::Real(35), M::Real(45)
        )
    );
    QCOMPARE(
        m.at(mr, mi),
        M::MatrixComplex::build(
            4, 4,
            M::Real(12), M::Real(22), M::Real(32), M::Real(42),
            M::Real(14), M::Real(24), M::Real(34), M::Real(44),
            M::Real(16), M::Real(26), M::Real(36), M::Real(46),
            M::Real(18), M::Real(28), M::Real(38), M::Real(48)
        )
    );
    QCOMPARE(
        m.at(mr, mr),
        M::MatrixComplex::build(
            4, 4,
            M::Real(11), M::Real(21), M::Real(31), M::Real(41),
            M::Real(12), M::Real(22), M::Real(32), M::Real(42),
            M::Real(13), M::Real(23), M::Real(33), M::Real(43),
            M::Real(14), M::Real(24), M::Real(34), M::Real(44)
        )
    );
    QCOMPARE(
        m.at(mr, mc),
        M::MatrixComplex::build(
            4, 2,
            M::Real(11), M::Real(21), M::Real(31), M::Real(41),
            M::Real(15), M::Real(25), M::Real(35), M::Real(45)
        )
    );

    QCOMPARE(m.at(mc, i), M::MatrixComplex::build(2, 1, M::Real(13), M::Real(53)));
    QCOMPARE(m.at(mc, r), M::MatrixComplex::build(2, 1, M::Real(14), M::Real(54)));
    QCOMPARE(m.at(mc, c), M::MatrixComplex::build(2, 1, M::Real(15), M::Real(55)));
    QCOMPARE(
        m.at(mc, rng),
        M::MatrixComplex::build(
            2, 2,
            M::Real(15), M::Real(55),
            M::Real(16), M::Real(56)
        )
    );
    QCOMPARE(
        m.at(mc, set),
        M::MatrixComplex::build(
            2, 2,
            M::Real(12), M::Real(52),
            M::Real(14), M::Real(54)
        )
    );
    QCOMPARE(
        m.at(mc, tuple),
        M::MatrixComplex::build(
            2, 2,
            M::Real(13), M::Real(53),
            M::Real(15), M::Real(55)
        )
    );
    QCOMPARE(
        m.at(mc, mi),
        M::MatrixComplex::build(
            2, 4,
            M::Real(12), M::Real(52),
            M::Real(14), M::Real(54),
            M::Real(16), M::Real(56),
            M::Real(18), M::Real(58)
        )
    );
    QCOMPARE(
        m.at(mc, mr),
        M::MatrixComplex::build(
            2, 4,
            M::Real(11), M::Real(51),
            M::Real(12), M::Real(52),
            M::Real(13), M::Real(53),
            M::Real(14), M::Real(54)
        )
    );
    QCOMPARE(
        m.at(mc, mc),
        M::MatrixComplex::build(
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


void TestMatrixComplex::testResizeMethod() {
    Model::MatrixComplex m(2, 2);
    QCOMPARE(m.numberRows(),    2);
    QCOMPARE(m.numberColumns(), 2);

    m.update(1,1, Model::Complex(1,1));
    m.update(1,2, Model::Complex(1,2));
    m.update(2,1, Model::Complex(2,1));
    m.update(2,2, Model::Complex(2,2));

    QCOMPARE(m(1,1), Model::Complex(1,1));
    QCOMPARE(m(1,2), Model::Complex(1,2));
    QCOMPARE(m(2,1), Model::Complex(2,1));
    QCOMPARE(m(2,2), Model::Complex(2,2));

    m.resize(3, 4);
    QCOMPARE(m.numberRows(),    3);
    QCOMPARE(m.numberColumns(), 4);

    QCOMPARE(m(1,1), Model::Complex(1,1));
    QCOMPARE(m(1,2), Model::Complex(1,2));
    QCOMPARE(m(1,3), Model::Complex(0,0));
    QCOMPARE(m(1,4), Model::Complex(0,0));
    QCOMPARE(m(2,1), Model::Complex(2,1));
    QCOMPARE(m(2,2), Model::Complex(2,2));
    QCOMPARE(m(3,3), Model::Complex(0,0));
    QCOMPARE(m(3,4), Model::Complex(0,0));
    QCOMPARE(m(3,1), Model::Complex(0,0));
    QCOMPARE(m(3,2), Model::Complex(0,0));
    QCOMPARE(m(3,3), Model::Complex(0,0));
    QCOMPARE(m(3,4), Model::Complex(0,0));
}


void TestMatrixComplex::testCoefficientAccessorMethods() {
    Model::MatrixComplex m1;
    QCOMPARE(m1.numberRows(), 0);
    QCOMPARE(m1.numberColumns(), 0);

    m1.update(1,1, Model::Complex(1,1));
    QCOMPARE(m1.numberRows(), 1);
    QCOMPARE(m1.numberColumns(), 1);

    m1.update(1,2, Model::Complex(1,2));
    QCOMPARE(m1.numberRows(), 1);
    QCOMPARE(m1.numberColumns(), 2);

    m1.update(2,1, Model::Complex(2,1));
    QCOMPARE(m1.numberRows(), 2);
    QCOMPARE(m1.numberColumns(), 2);

    m1.update(2,2, Model::Complex(2,2));
    QCOMPARE(m1.numberRows(), 2);
    QCOMPARE(m1.numberColumns(), 2);

    m1.update(1,3, Model::Complex(1,3));
    QCOMPARE(m1.numberRows(), 2);
    QCOMPARE(m1.numberColumns(), 3);

    m1.update(2,3, Model::Complex(2,3));
    QCOMPARE(m1.numberRows(), 2);
    QCOMPARE(m1.numberColumns(), 3);

    m1.update(3,1, Model::Complex(3,1));
    QCOMPARE(m1.numberRows(), 3);
    QCOMPARE(m1.numberColumns(), 3);

    m1.update(3,2, Model::Complex(3,2));
    m1.update(3,3, Model::Complex(3,3));
    QCOMPARE(m1.numberRows(), 3);
    QCOMPARE(m1.numberColumns(), 3);

    QCOMPARE(m1(1,1), Model::Complex(1,1));
    QCOMPARE(m1(1,2), Model::Complex(1,2));
    QCOMPARE(m1(1,3), Model::Complex(1,3));
    QCOMPARE(m1(2,1), Model::Complex(2,1));
    QCOMPARE(m1(2,2), Model::Complex(2,2));
    QCOMPARE(m1(2,3), Model::Complex(2,3));
    QCOMPARE(m1(3,1), Model::Complex(3,1));
    QCOMPARE(m1(3,2), Model::Complex(3,2));
    QCOMPARE(m1(3,3), Model::Complex(3,3));

    Model::MatrixComplex m2;
    m2.update(1, 1);
    m2.update(2, 2);
    m2.update(3, 3);

    QCOMPARE(m2.numberRows(), 3);
    QCOMPARE(m2.numberColumns(), 1);

    QCOMPARE(m2(1,1), Model::Complex(1, 0));
    QCOMPARE(m2(2,1), Model::Complex(2, 0));
    QCOMPARE(m2(3,1), Model::Complex(3, 0));

    Model::MatrixComplex m3;
    m3.update(1, 1, 1);
    m3.update(1, 2, 2);
    m3.update(3, 3);
    m3.update(4, 4);

    QCOMPARE(m3.numberRows(), 1);
    QCOMPARE(m3.numberColumns(), 4);

    QCOMPARE(m3(1,1), Model::Complex(1, 0));
    QCOMPARE(m3(1,2), Model::Complex(2, 0));
    QCOMPARE(m3(1,3), Model::Complex(3, 0));
    QCOMPARE(m3(1,4), Model::Complex(4, 0));
}


void TestMatrixComplex::testSliceMethods() {
    Model::MatrixComplex m = Model::MatrixComplex::build(
        5,
        5,
        Model::Complex(1, 1), Model::Complex(2, 1), Model::Complex(3, 1), Model::Complex(4, 1), Model::Complex(5, 1),
        Model::Complex(1, 2), Model::Complex(2, 2), Model::Complex(3, 2), Model::Complex(4, 2), Model::Complex(5, 2),
        Model::Complex(1, 3), Model::Complex(2, 3), Model::Complex(3, 3), Model::Complex(4, 3), Model::Complex(5, 3),
        Model::Complex(1, 4), Model::Complex(2, 4), Model::Complex(3, 4), Model::Complex(4, 4), Model::Complex(5, 4),
        Model::Complex(1, 5), Model::Complex(2, 5), Model::Complex(3, 5), Model::Complex(4, 5), Model::Complex(5, 5)
    );

    Model::MatrixComplex m1 = m.at(Model::Range(Model::Integer(2),Model::Integer(4)), 2);

    QCOMPARE(m1.numberRows(), 3);
    QCOMPARE(m1.numberColumns(), 1);
    QCOMPARE(m1(1, 1), Model::Complex(2, 2));
    QCOMPARE(m1(2, 1), Model::Complex(3, 2));
    QCOMPARE(m1(3, 1), Model::Complex(4, 2));

    Model::MatrixComplex m2 = m.at(3, Model::Range(Model::Integer(2),Model::Integer(3)));

    QCOMPARE(m2.numberRows(), 1);
    QCOMPARE(m2.numberColumns(), 2);
    QCOMPARE(m2(1, 1), Model::Complex(3, 2));
    QCOMPARE(m2(1, 2), Model::Complex(3, 3));

    Model::MatrixComplex m3 = m.at(
        Model::Range(Model::Integer(1), Model::Integer(3), Model::Integer(5)),
        Model::Range(Model::Integer(2),Model::Integer(4))
    );

    QCOMPARE(m3.numberRows(), 3);
    QCOMPARE(m3.numberColumns(), 3);

    QCOMPARE(m3(1, 1), Model::Complex(1, 2));
    QCOMPARE(m3(1, 2), Model::Complex(1, 3));
    QCOMPARE(m3(1, 3), Model::Complex(1, 4));
    QCOMPARE(m3(2, 1), Model::Complex(3, 2));
    QCOMPARE(m3(2, 2), Model::Complex(3, 3));
    QCOMPARE(m3(2, 3), Model::Complex(3, 4));
    QCOMPARE(m3(3, 1), Model::Complex(5, 2));
    QCOMPARE(m3(3, 2), Model::Complex(5, 3));
    QCOMPARE(m3(3, 3), Model::Complex(5, 4));
}


void TestMatrixComplex::testCombineMethods() {
    Model::MatrixComplex m1 = Model::MatrixComplex::build(
        3, 2,
        Model::Complex(11, 11), Model::Complex(12, 11), Model::Complex(13, 11),
        Model::Complex(11, 12), Model::Complex(12, 12), Model::Complex(13, 12)
    );

    Model::MatrixComplex m2 = Model::MatrixComplex::build(
        2, 3,
        Model::Complex(21, 21), Model::Complex(22, 21),
        Model::Complex(21, 22), Model::Complex(22, 22),
        Model::Complex(21, 23), Model::Complex(22, 23)
    );

    Model::MatrixComplex m3 = m1.combineLeftToRight(m2);
    QCOMPARE(
        m3,
        Model::MatrixComplex::build(
            3, 5,
            Model::Complex(11, 11), Model::Complex(12, 11), Model::Complex(13, 11),
            Model::Complex(11, 12), Model::Complex(12, 12), Model::Complex(13, 12),
            Model::Complex(21, 21), Model::Complex(22, 21), Model::Complex(0, 0),
            Model::Complex(21, 22), Model::Complex(22, 22), Model::Complex(0, 0),
            Model::Complex(21, 23), Model::Complex(22, 23), Model::Complex(0, 0)
        )
    );

    Model::MatrixComplex m4 = m2.combineLeftToRight(m1);
    QCOMPARE(
        m4,
        Model::MatrixComplex::build(
            3, 5,
            Model::Complex(21, 21), Model::Complex(22, 21), Model::Complex(0, 0),
            Model::Complex(21, 22), Model::Complex(22, 22), Model::Complex(0, 0),
            Model::Complex(21, 23), Model::Complex(22, 23), Model::Complex(0, 0),
            Model::Complex(11, 11), Model::Complex(12, 11), Model::Complex(13, 11),
            Model::Complex(11, 12), Model::Complex(12, 12), Model::Complex(13, 12)
        )
    );

    Model::MatrixComplex m5 = m1.combineTopToBottom(m2);
    QCOMPARE(
        m5,
        Model::MatrixComplex::build(
             5, 3,
             Model::Complex(11, 11), Model::Complex(12, 11), Model::Complex(13, 11),
                                                                        Model::Complex(21, 21), Model::Complex(22, 21),
             Model::Complex(11, 12), Model::Complex(12, 12), Model::Complex(13, 12),
                                                                        Model::Complex(21, 22), Model::Complex(22, 22),
             Model::Complex( 0,  0), Model::Complex( 0,  0), Model::Complex( 0,  0),
                                                                        Model::Complex(21, 23), Model::Complex(22, 23)
        )
    );

    Model::MatrixComplex m6 = m2.combineTopToBottom(m1);
    QCOMPARE(
        m6,
        Model::MatrixComplex::build(
             5, 3,
             Model::Complex(21, 21), Model::Complex(22, 21),
                                                Model::Complex(11, 11), Model::Complex(12, 11), Model::Complex(13, 11),
             Model::Complex(21, 22), Model::Complex(22, 22),
                                                Model::Complex(11, 12), Model::Complex(12, 12), Model::Complex(13, 12),
             Model::Complex(21, 23), Model::Complex(22, 23),
                                                Model::Complex( 0,  0), Model::Complex( 0,  0), Model::Complex( 0,  0)
        )
    );
}


void TestMatrixComplex::testReverseMethods() {
    Model::MatrixComplex m = Model::MatrixComplex::build(
        4, 4,
        Model::Complex(1, 1), Model::Complex(2, 1), Model::Complex(3, 1), Model::Complex(4, 1),
        Model::Complex(1, 2), Model::Complex(2, 2), Model::Complex(3, 2), Model::Complex(4, 2),
        Model::Complex(1, 3), Model::Complex(2, 3), Model::Complex(3, 3), Model::Complex(4, 3),
        Model::Complex(1, 4), Model::Complex(2, 4), Model::Complex(3, 4), Model::Complex(4, 4)
    );

    Model::MatrixComplex cr = m.columnReverse();
    QCOMPARE(
        cr,
        Model::MatrixComplex::build(
            4, 4,
            Model::Complex(1, 4), Model::Complex(2, 4), Model::Complex(3, 4), Model::Complex(4, 4),
            Model::Complex(1, 3), Model::Complex(2, 3), Model::Complex(3, 3), Model::Complex(4, 3),
            Model::Complex(1, 2), Model::Complex(2, 2), Model::Complex(3, 2), Model::Complex(4, 2),
            Model::Complex(1, 1), Model::Complex(2, 1), Model::Complex(3, 1), Model::Complex(4, 1)
        )
    );

    Model::MatrixComplex rr = m.rowReverse();
    QCOMPARE(
        rr,
        Model::MatrixComplex::build(
            4, 4,
            Model::Complex(4, 1), Model::Complex(3, 1), Model::Complex(2, 1), Model::Complex(1, 1),
            Model::Complex(4, 2), Model::Complex(3, 2), Model::Complex(2, 2), Model::Complex(1, 2),
            Model::Complex(4, 3), Model::Complex(3, 3), Model::Complex(2, 3), Model::Complex(1, 3),
            Model::Complex(4, 4), Model::Complex(3, 4), Model::Complex(2, 4), Model::Complex(1, 4)
        )
    );
}


void TestMatrixComplex::testIdentity() {
    Model::MatrixComplex i1 = Model::MatrixComplex::identity(10, 12);

    QCOMPARE(i1.numberRows(), 10);
    QCOMPARE(i1.numberColumns(), 12);

    for (unsigned row=1 ; row<=10 ; ++row) {
        for (unsigned column=1 ; column<=12 ; ++column) {
            Model::Complex expected = (row == column) ? Model::Complex(1) : Model::Complex(0);
            Model::Complex measured = i1(row, column);

            QCOMPARE(expected, measured);
        }
    }

    Model::MatrixComplex i2 = Model::MatrixComplex::identity(10);

    QCOMPARE(i2.numberRows(), 10);
    QCOMPARE(i2.numberColumns(), 10);

    for (unsigned row=1 ; row<=10 ; ++row) {
        for (unsigned column=1 ; column<=10 ; ++column) {
            Model::Complex expected = (row == column) ? Model::Complex(1) : Model::Complex(0);
            Model::Complex measured = i2(row, column);

            QCOMPARE(expected, measured);
        }
    }
}


void TestMatrixComplex::testZero() {
    Model::MatrixComplex i1 = Model::MatrixComplex::zero(10, 12);

    QCOMPARE(i1.numberRows(), 10);
    QCOMPARE(i1.numberColumns(), 12);

    for (unsigned row=1 ; row<=10 ; ++row) {
        for (unsigned column=1 ; column<=12 ; ++column) {
            QCOMPARE(i1(row, column), Model::Complex(0));
        }
    }

    Model::MatrixComplex i2 = Model::MatrixComplex::zero(10);

    QCOMPARE(i2.numberRows(), 10);
    QCOMPARE(i2.numberColumns(), 10);

    for (unsigned row=1 ; row<=10 ; ++row) {
        for (unsigned column=1 ; column<=10 ; ++column) {
            QCOMPARE(i2(row, column), Model::Complex(0));
        }
    }
}


void TestMatrixComplex::testOnes() {
    Model::Complex one(1.0);

    for (M::Integer numberRows=1 ; numberRows<=32 ; ++numberRows) {
        for (M::Integer numberColumns=1 ; numberColumns<=32 ; ++numberColumns) {
            Model::MatrixComplex i1 = Model::MatrixComplex::ones(numberRows, numberColumns);

            QCOMPARE(i1.numberRows(), numberRows);
            QCOMPARE(i1.numberColumns(), numberColumns);

            for (M::Integer row=1 ; row<=numberRows ; ++row) {
                for (M::Integer column=1 ; column<=numberColumns ; ++column) {
                    QCOMPARE(i1(row, column), one);
                }
            }
        }

        Model::MatrixComplex i2 = Model::MatrixComplex::ones(numberRows);

        QCOMPARE(i2.numberRows(), numberRows);
        QCOMPARE(i2.numberColumns(), numberRows);

        for (unsigned row=1 ; row<=numberRows ; ++row) {
            for (unsigned column=1 ; column<=numberRows ; ++column) {
                QCOMPARE(i2(row, column), one);
            }
        }
    }
}


void TestMatrixComplex::testDiagnonalEntries() {
    Model::MatrixComplex m = Model::MatrixComplex::build(
        4, 4,
        Model::Complex( 0,  -0), Model::Complex( 1,  -1), Model::Complex( 2,  -2), Model::Complex( 3,  -3),
        Model::Complex( 4,  -4), Model::Complex( 5,  -5), Model::Complex( 6,  -6), Model::Complex( 7,  -7),
        Model::Complex( 8,  -8), Model::Complex( 9,  -9), Model::Complex(10, -10), Model::Complex(11, -11),
        Model::Complex(12, -12), Model::Complex(13, -13), Model::Complex(14, -14), Model::Complex(15, -15)
    );

    Model::MatrixComplex d = m.diagonalEntries();
    QCOMPARE(d.numberRows(), 4);
    QCOMPARE(d.numberColumns(), 1);

    QCOMPARE(d(1), Model::Complex( 0,  -0));
    QCOMPARE(d(2), Model::Complex( 5,  -5));
    QCOMPARE(d(3), Model::Complex(10, -10));
    QCOMPARE(d(4), Model::Complex(15, -15));
}


void TestMatrixComplex::testDiagnonal() {
    Model::MatrixComplex d = Model::MatrixComplex::build(
        4, 1,
        Model::Complex(1, -1), Model::Complex(2, -2), Model::Complex(3, -3), Model::Complex(4, -4)
    );

    Model::MatrixComplex m = d.diagonal();

    QCOMPARE(m.numberRows(), 4);
    QCOMPARE(m.numberColumns(), 4);

    for (unsigned row=1 ; row<=4 ; ++row) {
        for (unsigned column=1 ; column<=4 ; ++column) {
            Model::Complex expected;
            if (row == column) {
                expected = Model::Complex(static_cast<Model::Real>(row), -static_cast<Model::Real>(row));
            } else {
                expected = Model::Complex(0);
            }

            Model::Complex measured = m(row, column);
            QCOMPARE(measured, expected);
        }
    }
}


void TestMatrixComplex::testMatrixTypeMethods() {
    Model::MatrixComplex rectangular = Model::MatrixComplex::build(
        3, 2,
        Model::Complex(1, 1), Model::Complex(2, 1), Model::Complex(3, 1),
        Model::Complex(1, 2), Model::Complex(2, 2), Model::Complex(3, 2)
    );

    Model::MatrixComplex square = Model::MatrixComplex::build(
        3, 3,
        Model::Complex(1, 1), Model::Complex(12, 11), Model::Complex(13, 11),
        Model::Complex(1, 2), Model::Complex(2, 2),   Model::Complex(13, 12),
        Model::Complex(1, 3), Model::Complex(2, 3),   Model::Complex(3, 3)
    );

    Model::MatrixComplex symmetric = Model::MatrixComplex::build(
        3, 3,
        Model::Complex(1, 1), Model::Complex(2, 1), Model::Complex(3, 1),
        Model::Complex(2, 1), Model::Complex(2, 2), Model::Complex(3, 2),
        Model::Complex(3, 1), Model::Complex(3, 2), Model::Complex(3, 3)
    );

    Model::MatrixComplex hermitian = Model::MatrixComplex::build(
        3, 3,
        Model::Complex(1,  0), Model::Complex(2,  1), Model::Complex(3, 1),
        Model::Complex(2, -1), Model::Complex(2,  0), Model::Complex(3, 2),
        Model::Complex(3, -1), Model::Complex(3, -2), Model::Complex(3, 0)
    );

    Model::MatrixComplex skewSymmetric = Model::MatrixComplex::build(
        3, 3,
        Model::Complex( 0,  0), Model::Complex( 2,  1), Model::Complex(3, 1),
        Model::Complex(-2, -1), Model::Complex( 0,  0), Model::Complex(3, 2),
        Model::Complex(-3, -1), Model::Complex(-3, -2), Model::Complex(0, 0)
    );

    Model::MatrixComplex skewHermitian = Model::MatrixComplex::build(
        3, 3,
        Model::Complex( 0, 0), Model::Complex( 2, 1), Model::Complex(3, 1),
        Model::Complex(-2, 1), Model::Complex( 0, 0), Model::Complex(3, 2),
        Model::Complex(-3, 1), Model::Complex(-3, 2), Model::Complex(0, 0)
    );

    // Matrix was shamelessly lifted from Wikipedia
    // https://en.wikipedia.org/wiki/Normal_matrix

    Model::MatrixComplex normal = Model::MatrixComplex::build(
        3, 3,
        Model::Complex(1, 0), Model::Complex(0, 0), Model::Complex(1, 0),
        Model::Complex(1, 0), Model::Complex(1, 0), Model::Complex(0, 0),
        Model::Complex(0, 0), Model::Complex(1, 0), Model::Complex(1, 0)
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
    QCOMPARE(symmetric.isHermitian(), false);
    QCOMPARE(symmetric.isSkewSymmetric(), false);
    QCOMPARE(symmetric.isSkewHermitian(), false);
    QCOMPARE(symmetric.isNormal(), false);

    QCOMPARE(hermitian.isSquare(), true);
    QCOMPARE(hermitian.isSymmetric(), false);
    QCOMPARE(hermitian.isHermitian(), true);
    QCOMPARE(hermitian.isSkewSymmetric(), false);
    QCOMPARE(hermitian.isSkewHermitian(), false);
    QCOMPARE(hermitian.isNormal(), true);

    QCOMPARE(skewSymmetric.isSquare(), true);
    QCOMPARE(skewSymmetric.isSymmetric(), false);
    QCOMPARE(skewSymmetric.isHermitian(), false);
    QCOMPARE(skewSymmetric.isSkewSymmetric(), true);
    QCOMPARE(skewSymmetric.isSkewHermitian(), false);
    QCOMPARE(skewSymmetric.isNormal(), false);

    QCOMPARE(skewHermitian.isSquare(), true);
    QCOMPARE(skewHermitian.isSymmetric(), false);
    QCOMPARE(skewHermitian.isHermitian(), false);
    QCOMPARE(skewHermitian.isSkewSymmetric(), false);
    QCOMPARE(skewHermitian.isSkewHermitian(), true);
    QCOMPARE(skewHermitian.isNormal(), true);

    QCOMPARE(normal.isSquare(), true);
    QCOMPARE(normal.isSymmetric(), false);
    QCOMPARE(normal.isHermitian(), false);
    QCOMPARE(normal.isSkewSymmetric(), false);
    QCOMPARE(normal.isSkewHermitian(), false);
    QCOMPARE(normal.isNormal(), true);
}


void TestMatrixComplex::testDeterminant1() {
    // We noted a simple test produced an incorrect determinant value (+2 rather than -2).  This test exists to verify
    // that the issue is resolved.

    Model::MatrixComplex matrix = Model::MatrixComplex::build(
        2, 2,
        1, 3,
        2, 4
    );

    Model::Complex d = matrix.determinant();
    QCOMPARE(d, Model::Complex(-2.0));

}


void TestMatrixComplex::testDeterminant2() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1, +1);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);
    Model::Real                                 maximumError = 1E-11;
    Model::Real                                 maximumRelativeError = 0;

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned numberRowColumns = dimensionDistribution(rng);
        Model::MatrixComplex m(numberRowColumns, numberRowColumns);

        for (unsigned long column=1 ; column<=numberRowColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRowColumns ; ++row) {
                Model::Complex v(coefficientDistribution(rng), coefficientDistribution(rng));
                m.update(row, column, v);
            }
        }

        Model::Complex measured = m.determinant();

        // Calculate the determinate using the upper trangular matrix from an LU decomposition.  Note that this is sort
        // of cheating as we use the same underlying function to calculate the determinant; however, we do test that
        // function below based on our ability to reconstruct the input matrix so this test should, in fact, be
        // reasonable.

        Model::Tuple         pluResult   = m.plu();
        Model::MatrixComplex p           = pluResult(1);
        Model::MatrixComplex l           = pluResult(2);
        Model::MatrixComplex u           = pluResult(3);
        Model::Boolean       notSingular = pluResult(4);

        if (notSingular) {
            unsigned long numberDiagnonals = std::min(u.numberRows(), u.numberColumns());
            Model::Complex expected(1.0);
            for (unsigned index=1 ; index<=numberDiagnonals ; ++index) {
                expected *= u(index, index);
            }

            if (std::signbit(measured.real()) != std::signbit(expected.real())) {
                // A bit of a cheat here; however, we check the signs in testDeterminant1.
                expected = -expected;
            }

            Model::Complex difference    = expected - measured;
            Model::Real    relativeError = std::sqrt(
                (difference * difference.conj()).real() / (expected * expected.conj()).real()
            );

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
    Model::MatrixComplex m1 = Model::MatrixComplex::build(
        3, 3,
        Model::Complex(2, 2), Model::Complex(2, 2), Model::Complex(2, 2),
        Model::Complex(2, 2), Model::Complex(2, 2), Model::Complex(2, 2),
        Model::Complex(2, 2), Model::Complex(2, 2), Model::Complex(2, 2)
    );

    QCOMPARE(m1.determinant(), Model::Complex(0));

    // Check for handling of non-square matricies.
    Model::MatrixComplex m2(10, 11);

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


void TestMatrixComplex::testTranspose() {
    Model::MatrixComplex m = Model::MatrixComplex::build(
        2, 3,
        Model::Complex(1, 1), Model::Complex(2, 1),
        Model::Complex(1, 2), Model::Complex(2, 2),
        Model::Complex(1, 3), Model::Complex(2, 3)
    );

    Model::MatrixComplex t1 = m.transpose();
    QCOMPARE(t1.numberRows(), 3);
    QCOMPARE(t1.numberColumns(), 2);

    QCOMPARE(
        t1,
        Model::MatrixComplex::build(
            3, 2,
            Model::Complex(1, 1), Model::Complex(1, 2), Model::Complex(1, 3),
            Model::Complex(2, 1), Model::Complex(2, 2), Model::Complex(2, 3)
        )
    );

    Model::MatrixComplex t2 = m.transpose();
    Model::MatrixComplex t3 = 2.0 * t2;
    QCOMPARE(
        t3,
        Model::MatrixComplex::build(
            3, 2,
            Model::Complex(2, 2), Model::Complex(2, 4), Model::Complex(2, 6),
            Model::Complex(4, 2), Model::Complex(4, 4), Model::Complex(4, 6)
        )
    );
}


void TestMatrixComplex::testComplexConjugate() {
    Model::MatrixComplex m = Model::MatrixComplex::build(
        2, 3,
        Model::Complex(1, 1), Model::Complex(2, 1),
        Model::Complex(1, 2), Model::Complex(2, 2),
        Model::Complex(1, 3), Model::Complex(2, 3)
    );

    Model::MatrixComplex t1 = m.conj();
    QCOMPARE(t1.numberRows(), 2);
    QCOMPARE(t1.numberColumns(), 3);

    QCOMPARE(
        t1,
        Model::MatrixComplex::build(
            2, 3,
            Model::Complex(1, -1), Model::Complex(2, -1),
            Model::Complex(1, -2), Model::Complex(2, -2),
            Model::Complex(1, -3), Model::Complex(2, -3)
        )
    );

    Model::MatrixComplex t2 = m.conj();
    Model::MatrixComplex t3 = 2.0 * t2;
    QCOMPARE(
        t3,
        Model::MatrixComplex::build(
            2, 3,
            Model::Complex(2, -2), Model::Complex(4, -2),
            Model::Complex(2, -4), Model::Complex(4, -4),
            Model::Complex(2, -6), Model::Complex(4, -6)
        )
    );
}


void TestMatrixComplex::testAdjoint() {
    Model::MatrixComplex m = Model::MatrixComplex::build(
        2, 3,
        Model::Complex(1, 1), Model::Complex(2, 1),
        Model::Complex(1, 2), Model::Complex(2, 2),
        Model::Complex(1, 3), Model::Complex(2, 3)
    );

    Model::MatrixComplex t1 = m.adjoint();
    QCOMPARE(t1.numberRows(), 3);
    QCOMPARE(t1.numberColumns(), 2);

    QCOMPARE(
        t1,
        Model::MatrixComplex::build(
            3, 2,
            Model::Complex(1, -1), Model::Complex(1, -2), Model::Complex(1, -3),
            Model::Complex(2, -1), Model::Complex(2, -2), Model::Complex(2, -3)
        )
    );

    Model::MatrixComplex t2 = m.adjoint();
    Model::MatrixComplex t3 = 2.0 * t2;
    QCOMPARE(
        t3,
        Model::MatrixComplex::build(
            3, 2,
            Model::Complex(2, -2), Model::Complex(2, -4), Model::Complex(2, -6),
            Model::Complex(4, -2), Model::Complex(4, -4), Model::Complex(4, -6)
        )
    );
}


void TestMatrixComplex::testInverse() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E6, +1E6);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);
    Model::Real                                 maximumAllowedRealError       = 1E-11;
    Model::Real                                 maximumAllowedImaginaryError  = 1E-11;
    Model::Real                                 maximumMeasuredRealError      = 0;
    Model::Real                                 maximumMeasuredImaginaryError = 0;

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned numberRowColumns = dimensionDistribution(rng);
        Model::MatrixComplex m(numberRowColumns, numberRowColumns);

        for (unsigned long column=1 ; column<=numberRowColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRowColumns ; ++row) {
                Model::Complex v(coefficientDistribution(rng), coefficientDistribution(rng));
                m.update(row, column, v);
            }
        }

        bool isSingular = false;
        Model::MatrixComplex inverse;
        try {
            inverse = m.inverse();
        } catch (Model::MatrixIsSingular&) {
            isSingular = true;
        }

        if (!isSingular) {
            QCOMPARE(inverse.numberRows(), numberRowColumns);
            QCOMPARE(inverse.numberColumns(), numberRowColumns);

            Model::MatrixComplex measuredIdentityMatrix = m * inverse;

            for (unsigned row=1 ; row<=numberRowColumns ; ++row) {
                for (unsigned column=1 ; column<=numberRowColumns ; ++column) {
                    Model::Complex expectedValue = row == column ? Model::Complex(1) : Model::Complex(0);
                    Model::Complex measuredValue = measuredIdentityMatrix(row, column);

                    Model::Real realError      = std::abs(expectedValue.real() - measuredValue.real());
                    Model::Real imaginaryError = std::abs(expectedValue.imag() - measuredValue.imag());

                    if (realError >= maximumAllowedRealError) {
                        QCOMPARE(realError < maximumAllowedRealError, true);
                    }

                    if (realError > maximumMeasuredRealError) {
                        maximumMeasuredRealError = realError;
                    }

                    if (imaginaryError >= maximumAllowedImaginaryError) {
                        QCOMPARE(imaginaryError < maximumAllowedImaginaryError, true);
                    }

                    if (imaginaryError > maximumMeasuredImaginaryError) {
                        maximumMeasuredImaginaryError = imaginaryError;
                    }
                }
            }
        }
    }
}


void TestMatrixComplex::testPLU() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E6, +1E6);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);
    Model::Real                                 maximumAllowedRelativeError = 5E-10;
    Model::Real                                 maximumRelativeError        = 0;
    Model::Real                                 maximumAllowedAbsoluteError = 1E-6;
    Model::Real                                 maximumAbsoluteError        = 0;

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned numberRowColumns = dimensionDistribution(rng);
        Model::MatrixComplex m(numberRowColumns, numberRowColumns);

        for (unsigned long column=1 ; column<=numberRowColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRowColumns ; ++row) {
                if (dimensionDistribution(rng) < 100) {
                    m.update(row, column, 0); // Slip in a few zero values to force pivoting
                } else {
                    Model::Complex v(coefficientDistribution(rng), coefficientDistribution(rng));
                    m.update(row, column, v);
                }
            }
        }

        Model::Tuple pluResult = m.plu();
        QCOMPARE(pluResult.size(), 4);

        Model::MatrixComplex p           = pluResult(1);
        Model::MatrixComplex l           = pluResult(2);
        Model::MatrixComplex u           = pluResult(3);
        Model::Boolean       notSingular = pluResult(4);

        if (notSingular) {
            QCOMPARE(p.numberRows(), numberRowColumns);
            QCOMPARE(p.numberColumns(), numberRowColumns);

            QCOMPARE(l.numberRows(), numberRowColumns);
            QCOMPARE(l.numberColumns(), numberRowColumns);

            QCOMPARE(u.numberRows(), numberRowColumns);
            QCOMPARE(u.numberColumns(), numberRowColumns);

            Model::MatrixComplex plu = p * l * u;

            QCOMPARE(plu.numberRows(), numberRowColumns);
            QCOMPARE(plu.numberColumns(), numberRowColumns);

            for (unsigned column=1 ; column<=numberRowColumns ; ++column) {
                for (unsigned row=1 ; row<=numberRowColumns ; ++row) {
                    Model::Complex measured      = plu(row, column);
                    Model::Complex expected      = m(row, column);
                    Model::Complex difference    = expected - measured;
                    Model::Real    relativeError;

                    if (expected != Model::Complex(0)) {
                        relativeError = std::sqrt(
                            (difference * difference.conj()).real() / (expected * expected.conj()).real()
                        );

                        if (relativeError >= maximumAllowedRelativeError) {
                            QCOMPARE(relativeError < maximumAllowedRelativeError, true);
                        }

                        if (relativeError > maximumRelativeError) {
                            maximumRelativeError = relativeError;
                        }
                    } else {
                        Model::Real absoluteError = std::sqrt((difference * difference.conj()).real());
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


void TestMatrixComplex::testSVD() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E6, +1E6);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);
    Model::Real                                 maximumAllowedRelativeError = 1E-8;
    Model::Real                                 maximumRelativeError        = 0;
    Model::Real                                 maximumAllowedAbsoluteError = 1E-16;
    Model::Real                                 maximumAbsoluteError        = 0;

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned numberRows    = dimensionDistribution(rng);
        unsigned numberColumns = dimensionDistribution(rng);
        Model::MatrixComplex m(numberRows, numberColumns);

        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                Model::Complex v(coefficientDistribution(rng), coefficientDistribution(rng));
                m.update(row, column, v);
            }
        }

        Model::Tuple svdResult = m.svd();
        QCOMPARE(svdResult.size(), 4);

        Model::MatrixComplex u       = svdResult(1);
        Model::MatrixComplex s       = svdResult(2);
        Model::MatrixComplex vh      = svdResult(3);
        Model::Boolean       success = svdResult(4);

        if (success) {
            QCOMPARE(u.numberRows(), numberRows);
            QCOMPARE(u.numberColumns(), numberRows);

            QCOMPARE(s.numberRows(), numberRows);
            QCOMPARE(s.numberColumns(), numberColumns);

            QCOMPARE(vh.numberRows(), numberColumns);
            QCOMPARE(vh.numberColumns(), numberColumns);

            Model::MatrixComplex measuredMatrix = u * s * vh;

            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                for (unsigned long column=1 ; column<=numberColumns ; ++column) {
                    Model::Complex expected = m(row, column);
                    Model::Complex measured = measuredMatrix(row, column);

                    if (expected != Model::Complex(0)) {
                        Model::Complex difference = expected - measured;
                        Model::Real relativeError = std::sqrt(
                            (difference * difference.conj()).real() / (expected * expected.conj()).real()
                        );

                        if (relativeError > maximumAllowedRelativeError) {
                            QCOMPARE(relativeError <= maximumAllowedRelativeError, true);
                        }

                        if (relativeError > maximumRelativeError) {
                            maximumRelativeError = relativeError;
                        }
                    } else {
                        Model::Real absoluteError = std::sqrt((measured * measured.conj()).real());

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


void TestMatrixComplex::testQR() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E6, +1E6);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);
    Model::Real                                 maximumAllowedRelativeError = 5E-5;
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

        Model::MatrixComplex m(numberRows, numberColumns);
        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                Model::Complex v(coefficientDistribution(rng), coefficientDistribution(rng));
                m.update(row, column, v);
            }
        }

        Model::Tuple qrResult = m.qr();
        QCOMPARE(qrResult.size(), 3);

        Model::MatrixComplex q       = qrResult(1);
        Model::MatrixComplex r       = qrResult(2);
        Model::Boolean       success = qrResult(3);

        if (success) {
            QCOMPARE(static_cast<unsigned long>(q.numberRows()), numberRows);
            QCOMPARE(static_cast<unsigned long>(q.numberColumns()), numberRows);

            QCOMPARE(static_cast<unsigned long>(r.numberRows()), numberRows);
            QCOMPARE(static_cast<unsigned long>(r.numberColumns()), numberColumns);

            Model::MatrixComplex measuredMatrix = q * r;
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                for (unsigned long column=1 ; column<=numberColumns ; ++column) {
                    Model::Complex expected = m(row, column);
                    Model::Complex measured = measuredMatrix(row, column);

                    if (expected != Model::Complex(0)) {
                        Model::Complex difference    = expected - measured;
                        Model::Real    relativeError = std::sqrt(
                            (difference * difference.conj()).real() / (expected * expected.conj()).real()
                        );

                        if (relativeError > maximumAllowedRelativeError) {
                            QCOMPARE(relativeError <= maximumAllowedRelativeError, true);
                        }

                        if (relativeError > maximumRelativeError) {
                            maximumRelativeError = relativeError;
                        }
                    } else {
                        Model::Real absoluteError = std::sqrt((measured * measured.conj()).real());

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


void TestMatrixComplex::testLQ() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E6, +1E6);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);
    Model::Real                                 maximumAllowedRelativeError = 5E-5;
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

        Model::MatrixComplex m(numberRows, numberColumns);
        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                Model::Complex v(coefficientDistribution(rng), coefficientDistribution(rng));
                m.update(row, column, v);
            }
        }

        Model::Tuple lqResult = m.lq();
        QCOMPARE(lqResult.size(), 3);

        Model::MatrixComplex l       = lqResult(1);
        Model::MatrixComplex q       = lqResult(2);
        Model::Boolean       success = lqResult(3);

        if (success) {
            QCOMPARE(static_cast<unsigned long>(l.numberRows()), numberRows);
            QCOMPARE(static_cast<unsigned long>(l.numberColumns()), numberColumns);

            QCOMPARE(static_cast<unsigned long>(q.numberRows()), numberColumns);
            QCOMPARE(static_cast<unsigned long>(q.numberColumns()), numberColumns);

            Model::MatrixComplex measuredMatrix = l * q;
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                for (unsigned long column=1 ; column<=numberColumns ; ++column) {
                    Model::Complex expected = m(row, column);
                    Model::Complex measured = measuredMatrix(row, column);

                    if (expected != Model::Complex(0)) {
                        Model::Complex difference    = expected - measured;
                        Model::Real    relativeError = std::sqrt(
                            (difference * difference.conj()).real() / (expected * expected.conj()).real()
                        );

                        if (relativeError > maximumAllowedRelativeError) {
                            QCOMPARE(relativeError <= maximumAllowedRelativeError, true);
                        }

                        if (relativeError > maximumRelativeError) {
                            maximumRelativeError = relativeError;
                        }
                    } else {
                        Model::Real absoluteError = std::sqrt((measured * measured.conj()).real());

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


void TestMatrixComplex::testCholesky() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(0, +1);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);
    Model::Real                                 maximumAllowedRelativeError = 5E-5;
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

        Model::MatrixComplex x(numberRowColumns, numberRowColumns);
        for (unsigned long column=1 ; column<=numberRowColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRowColumns ; ++row) {
                Model::Complex v(coefficientDistribution(rng), 0);
                x.update(row, column, v);
            }
        }

        Model::MatrixComplex m = 0.5 * x * x.adjoint();
        for (unsigned index=1 ; index<=numberRowColumns ; ++index) {
            Model::Complex v = m(index, index);
            m.update(index, index, v + 2.0 * numberRowColumns);
        }

        QCOMPARE(m.isHermitian(), true); // To validate the function above.

        Model::MatrixComplex l = m.cholesky();

        QCOMPARE(static_cast<unsigned long>(l.numberRows()), numberRowColumns);
        QCOMPARE(static_cast<unsigned long>(l.numberColumns()), numberRowColumns);

        Model::MatrixComplex measuredMatrix = l * l.adjoint();

        for (unsigned long row=1 ; row<=numberRowColumns ; ++row) {
            for (unsigned long column=1 ; column<=numberRowColumns ; ++column) {
                Model::Complex expected = m(row, column);
                Model::Complex measured = measuredMatrix(row, column);

                if (expected != Model::Complex(0)) {
                    Model::Complex difference    = expected - measured;
                    Model::Real    relativeError = std::sqrt(
                        (difference * difference.conj()).real() / (expected * expected.conj()).real()
                    );

                    if (relativeError > maximumAllowedRelativeError) {
                        QCOMPARE(relativeError <= maximumAllowedRelativeError, true);
                    }

                    if (relativeError > maximumRelativeError) {
                        maximumRelativeError = relativeError;
                    }
                } else {
                    Model::Real absoluteError = std::sqrt((measured * measured.conj()).real());

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


void TestMatrixComplex::testUpperCholesky() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(0, +1);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);
    Model::Real                                 maximumAllowedRelativeError = 5E-5;
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

        Model::MatrixComplex x(numberRowColumns, numberRowColumns);
        for (unsigned long column=1 ; column<=numberRowColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRowColumns ; ++row) {
                Model::Complex v(coefficientDistribution(rng), 0);
                x.update(row, column, v);
            }
        }

        Model::MatrixComplex m = 0.5 * x * x.adjoint();
        for (unsigned index=1 ; index<=numberRowColumns ; ++index) {
            Model::Complex v = m(index, index);
            m.update(index, index, v + 2.0 * numberRowColumns);
        }

        QCOMPARE(m.isHermitian(), true); // To validate the function above.

        Model::MatrixComplex u = m.upperCholesky();

        QCOMPARE(static_cast<unsigned long>(u.numberRows()), numberRowColumns);
        QCOMPARE(static_cast<unsigned long>(u.numberColumns()), numberRowColumns);

        Model::MatrixComplex measuredMatrix = u.adjoint() * u;

        for (unsigned long row=1 ; row<=numberRowColumns ; ++row) {
            for (unsigned long column=1 ; column<=numberRowColumns ; ++column) {
                Model::Complex expected = m(row, column);
                Model::Complex measured = measuredMatrix(row, column);

                if (expected != Model::Complex(0)) {
                    Model::Complex difference    = expected - measured;
                    Model::Real    relativeError = std::sqrt(
                        (difference * difference.conj()).real() / (expected * expected.conj()).real()
                    );

                    if (relativeError > maximumAllowedRelativeError) {
                        QCOMPARE(relativeError <= maximumAllowedRelativeError, true);
                    }

                    if (relativeError > maximumRelativeError) {
                        maximumRelativeError = relativeError;
                    }
                } else {
                    Model::Real absoluteError = std::sqrt((measured * measured.conj()).real());

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


void TestMatrixComplex::testBunchKaufman() {
    // FIXME: Add support for Bunch-Kaufman decomposition
}


void TestMatrixComplex::testUpperBunchKaufman() {
    // FIXME: Add support for Bunch-Kaufman decomposition
}


void TestMatrixComplex::testHessenberg() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(0, +1);
    std::uniform_int_distribution<>             dimensionDistribution(3, 1000);
    Model::Real                                 maximumAllowedRelativeError = 1E-10;
    Model::Real                                 maximumRelativeError        = 0;
    Model::Real                                 maximumAllowedAbsoluteError = 1E-15;
    Model::Real                                 maximumAbsoluteError        = 0;

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned long numberRowColumns = dimensionDistribution(rng);

        Model::MatrixComplex x(numberRowColumns, numberRowColumns);
        for (unsigned long column=1 ; column<=numberRowColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRowColumns ; ++row) {
                Model::Complex v(coefficientDistribution(rng), coefficientDistribution(rng));
                x.update(row, column, v);
            }
        }

        Model::Tuple hessenbergResult = x.hessenberg();
        QCOMPARE(hessenbergResult.size() , 2);

        Model::MatrixComplex q = hessenbergResult(1);
        Model::MatrixComplex h = hessenbergResult(2);

        Model::MatrixComplex measuredMatrix = q * h * q.adjoint();

        for (unsigned long row=1 ; row<=numberRowColumns ; ++row) {
            for (unsigned long column=1 ; column<=numberRowColumns ; ++column) {
                Model::Complex expected = x(row, column);
                Model::Complex measured = measuredMatrix(row, column);

                if (expected != Model::Complex(0)) {
                    Model::Complex difference    = expected - measured;
                    Model::Real    relativeError = std::sqrt(
                        (difference * difference.conj()).real() / (expected * expected.conj()).real()
                    );

                    if (relativeError > maximumAllowedRelativeError) {
                        QCOMPARE(relativeError <= maximumAllowedRelativeError, true);
                    }

                    if (relativeError > maximumRelativeError) {
                        maximumRelativeError = relativeError;
                    }
                } else {
                    Model::Real absoluteError = std::sqrt((measured * measured.conj()).real());

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


void TestMatrixComplex::testSchur() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(0, +1);
    std::uniform_int_distribution<>             dimensionDistribution(3, 1000);
    Model::Real                                 maximumAllowedRelativeError = 1E-8;
    Model::Real                                 maximumRelativeError        = 0;
    Model::Real                                 maximumAllowedAbsoluteError = 1E-15;
    Model::Real                                 maximumAbsoluteError        = 0;
    unsigned                                    numberFailures              = 0;

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned long numberRowColumns = dimensionDistribution(rng);

        Model::MatrixComplex x(numberRowColumns, numberRowColumns);
        for (unsigned long column=1 ; column<=numberRowColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRowColumns ; ++row) {
                Model::Complex v(coefficientDistribution(rng), coefficientDistribution(rng));
                x.update(row, column, v);
            }
        }

        Model::Tuple schurResult = x.schur();
        QCOMPARE(schurResult.size() , 4);

        Model::MatrixComplex q       = schurResult(1);
        Model::MatrixComplex u       = schurResult(2);
        Model::MatrixComplex w       = schurResult(3);
        Model::Boolean       success = schurResult(4);

        if (success) {
            Model::MatrixComplex qi             = q.inverse();
            Model::MatrixComplex measuredMatrix = q * u * qi;

            for (unsigned long row=1 ; row<=numberRowColumns ; ++row) {
                for (unsigned long column=1 ; column<=numberRowColumns ; ++column) {
                    Model::Complex expected = x(row, column);
                    Model::Complex measured = measuredMatrix(row, column);

                    if (expected != Model::Complex(0)) {
                        Model::Complex difference    = expected - measured;
                        Model::Real    relativeError = std::sqrt(
                            (difference * difference.conj()).real() / (expected * expected.conj()).real()
                        );

                        if (relativeError > maximumAllowedRelativeError) {
                            QCOMPARE(relativeError <= maximumAllowedRelativeError, true);
                        }

                        if (relativeError > maximumRelativeError) {
                            maximumRelativeError = relativeError;
                        }
                    } else {
                        Model::Real absoluteError = std::sqrt((measured * measured.conj()).real());

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

    QCOMPARE(numberFailures == 0, true); // Failures should be extremely rare.

    qDebug() << "number failures        = " << numberFailures;
    qDebug() << "maximum relative error = " << maximumRelativeError;
}


void TestMatrixComplex::testEigenvectors() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(0, +1);
    std::uniform_int_distribution<>             dimensionDistribution(3, 1000);
    Model::Real                                 maximumAllowedRelativeError = 1E-9;
    Model::Real                                 maximumRelativeError        = 0;
    Model::Real                                 maximumAllowedAbsoluteError = 1E-9;
    Model::Real                                 maximumAbsoluteError        = 0;
    unsigned                                    numberFailures              = 0;

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned long numberRowColumns = dimensionDistribution(rng);

        Model::MatrixComplex a(numberRowColumns, numberRowColumns);
        for (unsigned long column=1 ; column<=numberRowColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRowColumns ; ++row) {
                Model::Complex v(coefficientDistribution(rng), coefficientDistribution(rng));
                a.update(row, column, v);
            }
        }

        Model::Tuple eigenvectorResult = a.eigenvectors();
        QCOMPARE(eigenvectorResult.size() >= 3, true);

        Model::MatrixComplex eigenvalues = eigenvectorResult(1);
        Model::MatrixComplex q           = eigenvectorResult(2);
        Model::MatrixComplex u           = eigenvectorResult(3);

        unsigned long numberEigenvectors = eigenvalues.numberRows();
        QCOMPARE(static_cast<unsigned long>(eigenvectorResult.size()), numberEigenvectors + 3);

        for (unsigned evi=1 ; evi<=numberEigenvectors ; ++evi) {
            Model::MatrixComplex eigenvector = eigenvectorResult(evi + 3);
            Model::Complex       eigenvalue  = eigenvalues(evi);

            Model::MatrixComplex p1 = a * eigenvector;
            Model::MatrixComplex p2 = eigenvalue * eigenvector;

            QCOMPARE(p1.numberRows(),    p2.numberRows());
            QCOMPARE(p1.numberColumns(), p2.numberColumns());

            unsigned long numberRows    = p1.numberRows();
            unsigned long numberColumns = p1.numberColumns();

            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                for (unsigned long column=1 ; column<=numberColumns ; ++column) {
                    Model::Complex v1 = p1(row, column);
                    Model::Complex v2 = p2(row, column);

                    if (v1 != Model::Complex(0)) {
                        Model::Complex difference    = v1 - v2;
                        Model::Real    relativeError = std::sqrt(
                            (difference * difference.conj()).real() / (v1 * v1.conj()).real()
                        );

                        if (relativeError > maximumAllowedRelativeError) {
                            QCOMPARE(relativeError <= maximumAllowedRelativeError, true);
                        }

                        if (relativeError > maximumRelativeError) {
                            maximumRelativeError = relativeError;
                        }
                    } else {
                        Model::Real absoluteError = std::sqrt((v2 * v2.conj()).real());

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

    qDebug() << "number failures        = " << numberFailures;
    qDebug() << "maximum relative error = " << maximumRelativeError;
}


void TestMatrixComplex::testDft() {
    std::mt19937                    rng(0x12345678);
    std::uniform_int_distribution<> numberFrequenciesDistribution(1, 8);
    std::uniform_int_distribution<> frequencyDistribution(1, 100);
    std::uniform_int_distribution<> dimensionDistribution(3, 1000);
    Model::Real                     maximumAllowedRelativeError = 1E-13;
    Model::Real                     maximumRelativeError        = 0;
    Model::Real                     maximumAllowedAbsoluteError = 1E-9;
    Model::Real                     maximumAllowedDCError       = 1.0E-13;
    Model::Real                     maximumAbsoluteError        = 0;

    unsigned long numberRows    = 50;
    unsigned long numberColumns = 50;
    Model::Real   pi            = 4.0*std::atan(Model::Real(1));

    Model::MatrixComplex t(numberRows, numberColumns);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        QList<Model::Real> realFrequencies;
        QList<Model::Real> imaginaryFrequencies;

        unsigned numberRealFrequencies      = numberFrequenciesDistribution(rng);
        unsigned numberImaginaryFrequencies = numberFrequenciesDistribution(rng);

        for (unsigned f=0 ; f<numberRealFrequencies ; ++f) {
            realFrequencies.append(1); // frequencyDistribution(rng));
        }

        for (unsigned f=0 ; f<numberImaginaryFrequencies ; ++f) {
            imaginaryFrequencies.append(1); // frequencyDistribution(rng));
        }

        for (unsigned row=1 ; row<=numberRows ; ++row) {
            Model::Real r = 0;

            for (unsigned f=0 ; f<numberRealFrequencies ; ++f) {
                Model::Real frequency = realFrequencies.at(f);
                r += std::sin(frequency * 2 * pi * row / numberRows);
            }

            for (unsigned long column=1 ; column<=numberColumns ; ++column) {
                Model::Real i = 0;

                for (unsigned f=0 ; f<numberImaginaryFrequencies ; ++f) {
                    Model::Real frequency = imaginaryFrequencies.at(f);
                    i += std::sin(frequency * 2 * pi * column / numberColumns);
                }

                Model::Complex v(r, i);
                t.update(row, column, v);
            }
        }

        Model::MatrixComplex f = t.dft();
        QCOMPARE(static_cast<unsigned long>(f.numberRows()), numberRows);
        QCOMPARE(static_cast<unsigned long>(f.numberColumns()), numberColumns);

        // Question: Do we need to explicitly check frequency domain representation or is testing the forward and
        //           reverse transform sufficient.  I've verified the frequency domain representation matches the time
        //           domain representation for several simple cases.

        Model::MatrixComplex t2 = f.idft();
        QCOMPARE(static_cast<unsigned long>(t2.numberRows()), numberRows);
        QCOMPARE(static_cast<unsigned long>(t2.numberColumns()), numberColumns);

        for (unsigned long row=1 ; row<numberRows ; ++row) {
            for (unsigned long column=1 ; column<numberColumns ; ++column) {
                Model::Complex expected = t(row, column);
                Model::Complex measured = t2(row, column);

                if (row == numberRows / 2 && column == numberColumns / 2) {
                    Model::Complex difference  = expected - measured;
                    Model::Real    error       = std::sqrt((difference * difference.conj()).real());
                    if (error > maximumAllowedDCError) {
                        QCOMPARE(error <= maximumAllowedDCError, true);
                    }
                } else {
                    if (expected != Model::Complex(0)) {
                        Model::Complex difference    = expected - measured;
                        Model::Real    relativeError = std::sqrt(
                            (difference * difference.conj()).real() / (expected * expected.conj()).real()
                        );

                        if (relativeError > maximumAllowedRelativeError) {
                            QCOMPARE(relativeError <= maximumAllowedRelativeError, true);
                        }

                        if (relativeError > maximumRelativeError) {
                            maximumRelativeError = relativeError;
                        }
                    } else {
                        Model::Real absoluteError = std::sqrt((measured * measured.conj()).real());

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

    qDebug() << "maximum relative error = " << maximumRelativeError;
}


void TestMatrixComplex::testConditionNumber() {
    Model::MatrixComplex m1 = Model::MatrixComplex::build(
        3, 3,
        Model::Complex(1, 1), Model::Complex(2, 1), Model::Complex(3, 1),
        Model::Complex(1, 2), Model::Complex(2, 2), Model::Complex(3, 2),
        Model::Complex(1, 3), Model::Complex(2, 3), Model::Complex(3, 3)
    );

    Model::Real cn            = m1.conditionNumber();
    Model::Real expected      = 4.377999209165166E+16;
    Model::Real relativeError = abs(cn - expected) / expected;

    QCOMPARE(relativeError < 0.5, true);
}


void TestMatrixComplex::testNormMethods() {
    Model::MatrixComplex m1 = Model::MatrixComplex::build(
        3, 3,
        Model::Complex(1, 1), Model::Complex(2, 1), Model::Complex(3, 1),
        Model::Complex(1, 2), Model::Complex(2, 2), Model::Complex(3, 2),
        Model::Complex(1, 3), Model::Complex(2, 3), Model::Complex(3, 3)
    );

    Model::Real pNorm1Measured = m1.pNorm(1);
    Model::Real pNorm1Expected = 26.493075200502883;
    Model::Real pNorm1Error    = std::abs(pNorm1Measured - pNorm1Expected);
    QCOMPARE(pNorm1Error < 1.0E-10, true);

    Model::Real pNorm2Measured = m1.pNorm(2);
    Model::Real pNorm2Expected = 9.16515138991168;
    Model::Real pNorm2Error    = std::abs(pNorm2Measured - pNorm2Expected);
    QCOMPARE(pNorm2Error < 1.0E-10, true);

    Model::Real pNorm3Measured = m1.pNorm(3);
    Model::Real pNorm3Expected = 6.551262834663196;
    Model::Real pNorm3Error    = std::abs(pNorm3Measured - pNorm3Expected);
    QCOMPARE(pNorm3Error < 1.0E-10, true);

    Model::Real euclideanNormMeasured = m1.euclideanNorm();
    Model::Real euclideanNormExpected = 9.16515138991168;
    Model::Real euclideanNormError    = std::abs(euclideanNormMeasured - euclideanNormExpected);
    QCOMPARE(euclideanNormError < 1.0E-10, true);

    Model::Real norm1Measured = m1.oneNorm();
    Model::Real norm1Expected = 11.010469622751653;
    Model::Real norm1Error    = std::abs(norm1Measured - norm1Expected);
    QCOMPARE(norm1Error < 1.0E-10, true);

    Model::Real normInfinityMeasured = m1.infinityNorm();
    Model::Real normInfinityExpected = 11.010469622751653;
    Model::Real normInfinityError    = std::abs(normInfinityMeasured - normInfinityExpected);
    QCOMPARE(normInfinityError < 1.0E-10, true);
}


void TestMatrixComplex::testRank() {
    Model::MatrixComplex m1 = Model::MatrixComplex::build(
        3, 3,
        Model::Complex(1, 3),   Model::Complex(5, 7),   Model::Complex(11, 13),
        Model::Complex(17, 19), Model::Complex(23, 29), Model::Complex(31, 37),
        Model::Complex(41, 43), Model::Complex(47, 53), Model::Complex(59, 61)
    );

    Model::MatrixComplex m2 = Model::MatrixComplex::build(
        3, 3,
        Model::Complex(1, 1), Model::Complex(2, 1), Model::Complex(3, 1),
        Model::Complex(1, 2), Model::Complex(2, 2), Model::Complex(3, 2),
        Model::Complex(0, 0), Model::Complex(0, 0), Model::Complex(0, 0)
    );

    Model::MatrixComplex m3 = Model::MatrixComplex::build(
        3, 4,
        Model::Complex(1, 3),   Model::Complex(5, 7), Model::Complex(11, 13),
        Model::Complex(17, 19), Model::Complex(23, 29), Model::Complex(31, 37),
        Model::Complex(41, 43), Model::Complex(47, 53), Model::Complex(59, 61),
        Model::Complex(67, 71), Model::Complex(73, 79), Model::Complex(83, 89)
    );


    Model::MatrixComplex m4 = Model::MatrixComplex::build(
        4, 3,
        Model::Complex(1, 3),   Model::Complex(5, 7),   Model::Complex(11, 13), Model::Complex(17, 19),
        Model::Complex(23, 29), Model::Complex(31, 37), Model::Complex(41, 43), Model::Complex(47, 53),
        Model::Complex(59, 61), Model::Complex(67, 71), Model::Complex(73, 79), Model::Complex(83, 89)
    );

    Model::Integer rank1 = m1.rank(1.0E-10);
    Model::Integer rank2 = m2.rank(1.0E-10);
    Model::Integer rank3 = m3.rank(1.0E-10);
    Model::Integer rank4 = m4.rank(1.0E-10);

    QCOMPARE(rank1, 3);
    QCOMPARE(rank2, 2);
    QCOMPARE(rank3, 3);
    QCOMPARE(rank4, 3);
}


void TestMatrixComplex::testEquilibrate() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E6, +1E6);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);
    float                                       maximumDegregation = 1.10F;
    float                                       worstCaseDegredation = 0.0F;

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned long numberRowColumns = dimensionDistribution(rng);

        Model::MatrixComplex m(numberRowColumns, numberRowColumns);
        for (unsigned long column=1 ; column<=numberRowColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRowColumns ; ++row) {
                Model::Complex v(coefficientDistribution(rng), coefficientDistribution(rng));
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
            Model::MatrixComplex rowScaler    = t(1);
            Model::MatrixComplex columnScaler = t(2);
            Model::Boolean       successful   = t(3);

            QCOMPARE(successful, true);

            Model::MatrixComplex rowScaledMatrix    = rowScaler * m;
            Model::MatrixComplex columnScaledMatrix = m * columnScaler;
            Model::MatrixComplex scaledMatrix       = rowScaledMatrix * columnScaler;

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


void TestMatrixComplex::testSolve() {
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
        Model::MatrixComplex a(numberRowColumns, numberRowColumns);
        Model::MatrixComplex y(numberRowColumns, numberRightHandSides);

        for (unsigned long row=1 ; row<=numberRowColumns ; ++row) {
            for (unsigned long column=1 ; column<=numberRowColumns ; ++column) {
                Model::Complex v;
                do {
                    v = Model::Complex(coefficientDistribution(rng), 0); // coefficientDistribution(rng));
                } while (M::abs(v) < 1);

                a.update(row, column, v);
            }

            for (unsigned long column=1 ; column<=numberRightHandSides ; ++column) {
                Model::Complex v(coefficientDistribution(rng), coefficientDistribution(rng));
                y.update(row, column, v);
            }
        }

        Model::MatrixComplex x = a.solve(y);

        QCOMPARE(static_cast<unsigned long>(x.numberRows()), numberRowColumns);
        QCOMPARE(static_cast<unsigned long>(x.numberColumns()), numberRightHandSides);

        Model::MatrixComplex yMeasured = a * x;
        for (unsigned long column=1 ; column<numberRightHandSides ; ++column) {
            for (unsigned long row=1 ; row<numberRowColumns ; ++row) {
                Model::Complex expected   = y(row, column);
                Model::Complex measured   = yMeasured(row, column);
                Model::Complex difference = expected - measured;

                if (expected != Model::Complex(0)) {
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


void TestMatrixComplex::testLeastSquares() {
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
        unsigned long long numberRealTerms = 2 * aNumberRows * aNumberColumns;
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

        Model::MatrixComplex a(aNumberRows, aNumberColumns);
        Model::MatrixComplex b(aNumberRows, numberRightHandSides);
        unsigned long        primeIndex = 0;
        for (unsigned long row=1 ; row<=aNumberRows ; ++row) {
            for (unsigned long column=1 ; column<=aNumberColumns ; ++column) {
                a.update(row, column, Model::Complex(primes[primeIndex], primes[primeIndex+1]));
                primeIndex += 2;
            }

            for (unsigned long column=1 ; column<=numberRightHandSides ; ++column) {
                Model::Complex v;
                do {
                    v = Model::Complex(bTermDistribution(rng), bTermDistribution(rng));
                } while (v == Model::Complex(0));
                b.update(row, column, v);
            }
        }

        Model::MatrixComplex x = a.leastSquares(b);
        QCOMPARE(static_cast<unsigned long>(x.numberRows()), aNumberColumns);
        QCOMPARE(static_cast<unsigned long>(x.numberColumns()), numberRightHandSides);

        Model::MatrixComplex residuals = a * x - b;

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


void TestMatrixComplex::testHadamardProduct() {
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

        Model::MatrixComplex m1(numberRows, numberColumns);
        Model::MatrixComplex m2(numberRows, numberColumns);
        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            for (unsigned long row=1 ; row<=numberRows ; ++row) {
                Model::Complex v1(coefficientDistribution(rng), coefficientDistribution(rng));
                m1.update(row, column, v1);

                Model::Complex v2(coefficientDistribution(rng), coefficientDistribution(rng));
                m2.update(row, column, v2);
            }
        }

        Model::MatrixComplex m = m1.hadamard(m2);

        QCOMPARE(static_cast<unsigned long>(m.numberRows()), numberRows);
        QCOMPARE(static_cast<unsigned long>(m.numberColumns()), numberColumns);

        for (unsigned long row=1 ; row<=numberRows ; ++row) {
            for (unsigned long column=1 ; column<=numberColumns ; ++column) {
                Model::Complex expected = m1(row, column) * m2(row, column);
                Model::Complex measured = m(row, column);

                if (expected != Model::Complex(0)) {
                    Model::Complex difference    = expected - measured;
                    Model::Real    relativeError = std::sqrt(
                        (difference * difference.conj()).real() / (expected * expected.conj()).real()
                    );

                    if (relativeError > maximumAllowedRelativeError) {
                        QCOMPARE(relativeError < maximumAllowedRelativeError, true);
                    }

                    if (relativeError > maximumRelativeError) {
                        maximumRelativeError = relativeError;
                    }
                } else {
                    Model::Real absoluteError = std::sqrt((measured * measured.conj()).real());

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


void TestMatrixComplex::testKroneckerProduct() {
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

        Model::MatrixComplex m1(multiplierNumberRows, multiplierNumberColumns);
        Model::MatrixComplex m2(multiplicandNumberRows, multiplicandNumberColumns);

        for (unsigned long column=1 ; column<=multiplierNumberColumns ; ++column) {
            for (unsigned long row=1 ; row<=multiplierNumberRows ; ++row) {
                Model::Complex v1(coefficientDistribution(rng), coefficientDistribution(rng));
                m1.update(row, column, v1);
            }
        }

        for (unsigned long column=1 ; column<=multiplicandNumberColumns ; ++column) {
            for (unsigned long row=1 ; row<=multiplicandNumberRows ; ++row) {
                Model::Complex v2(coefficientDistribution(rng), coefficientDistribution(rng));
                m2.update(row, column, v2);
            }
        }

        Model::MatrixComplex m = m1.kronecker(m2);

        QCOMPARE(static_cast<unsigned long>(m.numberRows()), multiplierNumberRows * multiplicandNumberRows);
        QCOMPARE(static_cast<unsigned long>(m.numberColumns()), multiplierNumberColumns * multiplicandNumberColumns);

        for (unsigned long multiplierRow=1 ; multiplierRow<=multiplierNumberRows ; ++multiplierRow) {
            unsigned long productRowOffset = (multiplierRow - 1) * multiplicandNumberRows;

            for (unsigned long multiplierColumn=1 ; multiplierColumn<=multiplierNumberColumns ; ++multiplierColumn) {
                unsigned long  productColumnOffset = (multiplierColumn - 1) * multiplicandNumberColumns;
                Model::Complex multiplier          = m1(multiplierRow, multiplierColumn);

                for (unsigned long multiplicandRow=1 ; multiplicandRow<=multiplicandNumberRows ; ++multiplicandRow) {
                    unsigned long productRow = productRowOffset + multiplicandRow;

                    for (  unsigned long multiplicandColumn = 1
                         ; multiplicandColumn <= multiplicandNumberColumns
                         ; ++multiplicandColumn
                        ) {
                        unsigned long productColumn = productColumnOffset + multiplicandColumn;

                        Model::Complex multiplicand = m2(multiplicandRow, multiplicandColumn);
                        Model::Complex expected     = multiplier * multiplicand;
                        Model::Complex measured     = m(productRow, productColumn);

                        if (expected != Model::Complex(0)) {
                            Model::Complex difference    = expected - measured;
                            Model::Real    relativeError = std::sqrt(
                                (difference * difference.conj()).real() / (expected * expected.conj()).real()
                            );

                            if (relativeError > maximumAllowedRelativeError) {
                                QCOMPARE(relativeError < maximumAllowedRelativeError, true);
                            }

                            if (relativeError > maximumRelativeError) {
                                maximumRelativeError = relativeError;
                            }
                        } else {
                            Model::Real absoluteError = std::sqrt((measured * measured.conj()).real());

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


void TestMatrixComplex::testRawDataAccess() {
    Model::MatrixComplex m = Model::MatrixComplex::build(
        2, 3,
        Model::Complex(1, 1), Model::Complex(2, 1),
        Model::Complex(1, 2), Model::Complex(2, 2),
        Model::Complex(1, 3), Model::Complex(2, 3)
    );

    const Model::Complex* rawData = m.data();

    QCOMPARE(rawData[ 0], Model::Complex(1, 1));
    QCOMPARE(rawData[ 1], Model::Complex(2, 1));
    QCOMPARE(rawData[ 2], Model::Complex(0, 0));
    QCOMPARE(rawData[ 3], Model::Complex(0, 0));

    QCOMPARE(rawData[ 4], Model::Complex(1, 2));
    QCOMPARE(rawData[ 5], Model::Complex(2, 2));
    QCOMPARE(rawData[ 6], Model::Complex(0, 0));
    QCOMPARE(rawData[ 7], Model::Complex(0, 0));

    QCOMPARE(rawData[ 8], Model::Complex(1, 3));
    QCOMPARE(rawData[ 9], Model::Complex(2, 3));
    QCOMPARE(rawData[10], Model::Complex(0, 0));
    QCOMPARE(rawData[11], Model::Complex(0, 0));
}


void TestMatrixComplex::testAssignmentOperators() {
    Model::MatrixComplex m1(2, 3);
    QCOMPARE(m1.numberRows(),    2);
    QCOMPARE(m1.numberColumns(), 3);

    m1.update(1, 1, Model::Complex(-1,-2));
    m1.update(1, 2, Model::Complex(3,4));
    m1.update(1, 3, Model::Complex(5,6));
    m1.update(2, 1, Model::Complex(7,8));
    m1.update(2, 2, Model::Complex(-9,-10));
    m1.update(2, 3, Model::Complex(11,12));

    Model::MatrixComplex m2(1, 2);
    m2.update(1, 1, Model::Complex(10, 11));
    m2.update(1, 2, Model::Complex(12, 13));

    QCOMPARE(m1.numberRows(),    2);
    QCOMPARE(m1.numberColumns(), 3);

    QCOMPARE(m1(1,1), Model::Complex(-1,-2));
    QCOMPARE(m1(1,2), Model::Complex(3,4));
    QCOMPARE(m1(1,3), Model::Complex(5,6));
    QCOMPARE(m1(2,1), Model::Complex(7,8));
    QCOMPARE(m1(2,2), Model::Complex(-9,-10));
    QCOMPARE(m1(2,3), Model::Complex(11,12));

    QCOMPARE(m2.numberRows(),    1);
    QCOMPARE(m2.numberColumns(), 2);
    QCOMPARE(m2(1,1), Model::Complex(10, 11));
    QCOMPARE(m2(1,2), Model::Complex(12, 13));

    m2 = m1;

    QCOMPARE(m2.numberRows(),    2);
    QCOMPARE(m2.numberColumns(), 3);

    QCOMPARE(m2(1,1), Model::Complex(-1,-2));
    QCOMPARE(m2(1,2), Model::Complex(3,4));
    QCOMPARE(m2(1,3), Model::Complex(5,6));
    QCOMPARE(m2(2,1), Model::Complex(7,8));
    QCOMPARE(m2(2,2), Model::Complex(-9,-10));
    QCOMPARE(m2(2,3), Model::Complex(11,12));

    m1.update(1, 1, Model::Complex(12,11));
    m1.update(1, 2, Model::Complex(10, 9));
    m1.update(1, 3, Model::Complex( 8, 7));
    m1.update(2, 1, Model::Complex( 6, 5));
    m1.update(2, 2, Model::Complex( 4, 3));
    m1.update(2, 3, Model::Complex( 2, 1));

    QCOMPARE(m2(1,1), Model::Complex(-1,-2));
    QCOMPARE(m2(1,2), Model::Complex(3,4));
    QCOMPARE(m2(1,3), Model::Complex(5,6));
    QCOMPARE(m2(2,1), Model::Complex(7,8));
    QCOMPARE(m2(2,2), Model::Complex(-9,-10));
    QCOMPARE(m2(2,3), Model::Complex(11,12));
}


void TestMatrixComplex::testAddAssignmentOperators() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E10, +1E10);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned numberRows      = dimensionDistribution(rng);
        unsigned numberColumns   = dimensionDistribution(rng);

        Model::MatrixComplex expected1(numberRows, numberColumns);
        Model::MatrixComplex augend1(numberRows, numberColumns);
        Model::MatrixComplex addend1(numberRows, numberColumns);

        Model::MatrixComplex expected2(numberRows, numberColumns);
        Model::MatrixComplex augend2(numberRows, numberColumns);
        Model::MatrixComplex addend2(numberRows, numberColumns);

        Model::MatrixComplex expected3(numberColumns, numberRows);
        Model::MatrixComplex augend3(numberRows, numberColumns);
        Model::MatrixComplex addend3(numberRows, numberColumns);

        for (unsigned long columnIndex=1 ; columnIndex<=numberColumns ; ++columnIndex) {
            for (unsigned long rowIndex=1 ; rowIndex<=numberRows ; ++rowIndex) {
                Model::Complex a1 = Model::Complex(coefficientDistribution(rng), coefficientDistribution(rng));
                Model::Complex a2 = Model::Complex(coefficientDistribution(rng), coefficientDistribution(rng));

                augend1.update(rowIndex, columnIndex, a1);
                addend1.update(rowIndex, columnIndex, a2);
                expected1.update(rowIndex, columnIndex, a1 + a2);

                augend2.update(rowIndex, columnIndex, a1);
                addend2.update(rowIndex, columnIndex, a2);
                expected2.update(rowIndex, columnIndex, 2 * a1 + 3 * a2.conj());

                augend3.update(rowIndex, columnIndex, a1);
                addend3.update(rowIndex, columnIndex, a2);
                expected3.update(columnIndex, rowIndex, 2 * a1 + 3 * a2.conj());
            }
        }

        Model::MatrixComplex measured = augend1;
        measured += addend1;
        QCOMPARE(measured, expected1);

        measured = 2 * augend2;
        measured += 3 * addend2.conj();
        QCOMPARE(measured, expected2);

        measured = 2 * augend3.transpose();
        measured += 3 * addend3.adjoint();
        QCOMPARE(measured, expected3);
    }

    Model::MatrixComplex m1(10, 11);
    Model::MatrixComplex m2(12, 13);

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


void TestMatrixComplex::testSubtractAssignmentOperators() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E10, +1E10);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned numberRows      = dimensionDistribution(rng);
        unsigned numberColumns   = dimensionDistribution(rng);

        Model::MatrixComplex expected1(numberRows, numberColumns);
        Model::MatrixComplex augend1(numberRows, numberColumns);
        Model::MatrixComplex addend1(numberRows, numberColumns);

        Model::MatrixComplex expected2(numberRows, numberColumns);
        Model::MatrixComplex augend2(numberRows, numberColumns);
        Model::MatrixComplex addend2(numberRows, numberColumns);

        Model::MatrixComplex expected3(numberColumns, numberRows);
        Model::MatrixComplex augend3(numberRows, numberColumns);
        Model::MatrixComplex addend3(numberRows, numberColumns);

        for (unsigned long columnIndex=1 ; columnIndex<=numberColumns ; ++columnIndex) {
            for (unsigned long rowIndex=1 ; rowIndex<=numberRows ; ++rowIndex) {
                Model::Complex a1 = Model::Complex(coefficientDistribution(rng), coefficientDistribution(rng));
                Model::Complex a2 = Model::Complex(coefficientDistribution(rng), coefficientDistribution(rng));

                augend1.update(rowIndex, columnIndex, a1);
                addend1.update(rowIndex, columnIndex, a2);
                expected1.update(rowIndex, columnIndex, a1 - a2);

                augend2.update(rowIndex, columnIndex, a1);
                addend2.update(rowIndex, columnIndex, a2);
                expected2.update(rowIndex, columnIndex, 2 * a1 - 3 * a2.conj());

                augend3.update(rowIndex, columnIndex, a1);
                addend3.update(rowIndex, columnIndex, a2);
                expected3.update(columnIndex, rowIndex, 2 * a1 - 3 * a2.conj());
            }
        }

        Model::MatrixComplex measured = augend1;
        measured -= addend1;
        QCOMPARE(measured, expected1);

        measured = 2 * augend2;
        measured -= 3 * addend2.conj();
        QCOMPARE(measured, expected2);

        measured = 2 * augend3.transpose();
        measured -= 3 * addend3.adjoint();
        QCOMPARE(measured, expected3);
    }

    Model::MatrixComplex m1(10, 11);
    Model::MatrixComplex m2(12, 13);

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


void TestMatrixComplex::testMatrixMatrixMultiplyAssignmentOperator() {
    // For now we just to a cursory test.  Since the underlying engine is the same, we do a more thorough test below.

    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E10, +1E10);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);
    Model::Real                                 maximumError = 1E-11;

    unsigned multiplierRows      = dimensionDistribution(rng);
    unsigned multiplierColumns   = dimensionDistribution(rng);
    unsigned multiplicandRows    = multiplierColumns;
    unsigned multiplicandColumns = dimensionDistribution(rng);

    Model::MatrixComplex multiplier(multiplierRows, multiplierColumns);
    for (unsigned row=1 ; row<=multiplierRows ; ++row) {
        for (unsigned column=1 ; column<multiplierColumns ; ++column) {
            Model::Complex c(coefficientDistribution(rng), coefficientDistribution(rng));
            multiplier.update(row, column, c);
        }
    }

    Model::MatrixComplex multiplicand(multiplicandRows, multiplicandColumns);
    for (unsigned row=1 ; row<=multiplicandRows ; ++row) {
        for (unsigned column=1 ; column<multiplicandColumns ; ++column) {
            Model::Complex c(coefficientDistribution(rng), coefficientDistribution(rng));
            multiplicand.update(row, column, c);
        }
    }

    Model::MatrixComplex measuredMatrix = multiplier;
    measuredMatrix *= multiplicand;

    QCOMPARE(measuredMatrix.numberRows(), multiplierRows);
    QCOMPARE(measuredMatrix.numberColumns(), multiplicandColumns);

    for (unsigned row=1 ; row<=multiplierRows ; ++row) {
        for (unsigned column=1 ; column<=multiplicandColumns ; ++column) {
            Model::Complex expected(0);
            for (unsigned multiplierColumn=1 ; multiplierColumn<=multiplierColumns ; ++multiplierColumn) {
                expected += multiplier(row, multiplierColumn) * multiplicand(multiplierColumn, column);
            }

            Model::Complex measured = measuredMatrix(row, column);
            Model::Complex error    = expected - measured;

            Model::Real relativeError = sqrt((error * error.conj()).real() / (expected * expected).real());

            if (relativeError > maximumError) {
                QCOMPARE(relativeError < maximumError, true);
            }
        }
    }

    Model::MatrixComplex m1(10, 11);
    Model::MatrixComplex m2(12, 13);

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


void TestMatrixComplex::testMatrixScalarMultiplyAssignmentOperators() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E10, +1E10);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);
    Model::Real                                 maximumError = 5.0E-16;

    unsigned multiplierRows    = dimensionDistribution(rng);
    unsigned multiplierColumns = dimensionDistribution(rng);

    Model::MatrixComplex multiplier(multiplierRows, multiplierColumns);
    for (unsigned row=1 ; row<=multiplierRows ; ++row) {
        for (unsigned column=1 ; column<multiplierColumns ; ++column) {
            Model::Complex c(coefficientDistribution(rng), coefficientDistribution(rng));
            multiplier.update(row, column, c);
        }
    }

    Model::MatrixComplex booleanTrueMeasured = multiplier;
    booleanTrueMeasured  *= Model::Boolean(true);

    Model::MatrixComplex booleanFalseMeasured = multiplier;
    booleanFalseMeasured *= Model::Boolean(false);

    Model::Integer       integerScalar   = dimensionDistribution(rng);
    Model::MatrixComplex integerMeasured = multiplier;
    integerMeasured *= integerScalar;

    Model::Real          realScalar   = coefficientDistribution(rng);
    Model::MatrixComplex realMeasured = multiplier;
    realMeasured *= realScalar;

    Model::Complex       complexScalar(coefficientDistribution(rng), coefficientDistribution(rng));
    Model::MatrixComplex complexMeasured = multiplier;
    complexMeasured *= complexScalar;

    QCOMPARE(booleanTrueMeasured.numberRows(), multiplierRows);
    QCOMPARE(booleanTrueMeasured.numberColumns(), multiplierColumns);

    QCOMPARE(booleanFalseMeasured.numberRows(), multiplierRows);
    QCOMPARE(booleanFalseMeasured.numberColumns(), multiplierColumns);

    QCOMPARE(integerMeasured.numberRows(), multiplierRows);
    QCOMPARE(integerMeasured.numberColumns(), multiplierColumns);

    QCOMPARE(realMeasured.numberRows(), multiplierRows);
    QCOMPARE(realMeasured.numberColumns(), multiplierColumns);

    QCOMPARE(complexMeasured.numberRows(), multiplierRows);
    QCOMPARE(complexMeasured.numberColumns(), multiplierColumns);

    Model::Real maximumRelativeError = 0;
    for (unsigned row=1 ; row<=multiplierRows ; ++row) {
        for (unsigned column=1 ; column<=multiplierColumns ; ++column) {
            QCOMPARE(booleanTrueMeasured(row, column), multiplier(row, column));
            QCOMPARE(booleanFalseMeasured(row, column), Model::Complex(0, 0));

            Model::Complex integerExpected   = multiplier(row, column) * integerScalar;
            Model::Complex integerDifference = integerMeasured(row, column) - integerExpected;
            Model::Real    relativeError     = std::sqrt(
                  (integerDifference * integerDifference.conj()).real()
                / (integerExpected * integerExpected.conj()).real()
            );
            if (relativeError > maximumError) {
                QCOMPARE(relativeError <= maximumError, true);
            }

            if (relativeError > maximumRelativeError) {
                maximumRelativeError = relativeError;
            }

            Model::Complex realExpected = multiplier(row, column) * realScalar;
            Model::Complex realDifference = realMeasured(row, column) - realExpected;
            relativeError = std::sqrt(
                  (realDifference * realDifference.conj()).real()
                / (realExpected * realExpected.conj()).real()
            );
            if (relativeError > maximumError) {
                QCOMPARE(relativeError <= maximumError, true);
            }

            if (relativeError > maximumRelativeError) {
                maximumRelativeError = relativeError;
            }

            Model::Complex complexExpected = multiplier(row, column) * complexScalar;
            Model::Complex complexDifference = complexMeasured(row, column) - complexExpected;
            relativeError = std::sqrt(
                  (complexDifference * complexDifference.conj()).real()
                / (complexExpected * complexExpected.conj()).real()
            );
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


void TestMatrixComplex::testDivideAssignmentOperators() {
    // Divide operators are functionally identical to the multiply operators so we only do limited testing.

    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E10, +1E10);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);

    unsigned dividendRows    = dimensionDistribution(rng);
    unsigned dividendColumns = dimensionDistribution(rng);

    Model::MatrixComplex dividend(dividendRows, dividendColumns);
    for (unsigned row=1 ; row<=dividendRows ; ++row) {
        for (unsigned column=1 ; column<dividendColumns ; ++column) {
            Model::Complex c(coefficientDistribution(rng), coefficientDistribution(rng));
            dividend.update(row, column, c);
        }
    }

    Model::Integer       integerScalar   = dimensionDistribution(rng);
    Model::MatrixComplex integerMeasured = dividend;
    integerMeasured /= integerScalar;

    Model::Real          realScalar   = coefficientDistribution(rng);
    Model::MatrixComplex realMeasured = dividend;
    realMeasured /= realScalar;

    Model::Complex       complexScalar(coefficientDistribution(rng), coefficientDistribution(rng));
    Model::MatrixComplex complexMeasured = dividend;
    complexMeasured /= complexScalar;

    QCOMPARE(integerMeasured.numberRows(), dividendRows);
    QCOMPARE(integerMeasured.numberColumns(), dividendColumns);

    QCOMPARE(realMeasured.numberRows(), dividendRows);
    QCOMPARE(realMeasured.numberColumns(), dividendColumns);

    QCOMPARE(complexMeasured.numberRows(), dividendRows);
    QCOMPARE(complexMeasured.numberColumns(), dividendColumns);

    Model::Real maximumError = 0;
    for (unsigned row=1 ; row<=dividendRows ; ++row) {
        for (unsigned column=1 ; column<=dividendColumns ; ++column) {
            Model::Complex integerExpected      = dividend(row, column) / integerScalar;
            Model::Complex integerDifference    = integerMeasured(row, column) - integerExpected;
            Model::Real    integerRelativeError = std::sqrt(
                  (integerDifference * integerDifference.conj()).real()
                / (integerExpected * integerExpected.conj()).real()
            );
            if (integerRelativeError >= 1.0E-11) {
                QCOMPARE(integerRelativeError < 1.0E-15, true);
            }

            Model::Complex realExpected = dividend(row, column) / realScalar;
            Model::Complex realDifference    = realMeasured(row, column) - realExpected;
            Model::Real    realRelativeError = std::sqrt(
                  (realDifference * realDifference.conj()).real()
                / (realExpected * realExpected.conj()).real()
            );
            if (realRelativeError >= 1.0E-11) {
                QCOMPARE(realRelativeError < 1.0E-15, true);
            }

            Model::Complex complexExpected = dividend(row, column) / complexScalar;
            Model::Complex complexDifference    = complexMeasured(row, column) - complexExpected;
            Model::Real    complexRelativeError = std::sqrt(
                  (complexDifference * complexDifference.conj()).real()
                / (complexExpected * complexExpected.conj()).real()
            );
            if (complexRelativeError >= 1.0E-11) {
                QCOMPARE(complexRelativeError < 1.0E-15, true);
            }

            maximumError = std::max(
                std::max(maximumError, integerRelativeError),
                std::max(realRelativeError, complexRelativeError)
            );
        }
    }

    qDebug() << "maximum error = " << maximumError;
}


void TestMatrixComplex::testAdditionOperators() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E10, +1E10);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned numberRows      = dimensionDistribution(rng);
        unsigned numberColumns   = dimensionDistribution(rng);

        Model::MatrixComplex expected1(numberRows, numberColumns);
        Model::MatrixComplex augend1(numberRows, numberColumns);
        Model::MatrixComplex addend1(numberRows, numberColumns);

        Model::MatrixComplex expected2(numberRows, numberColumns);
        Model::MatrixComplex augend2(numberRows, numberColumns);
        Model::MatrixComplex addend2(numberRows, numberColumns);

        Model::MatrixComplex expected3(numberColumns, numberRows);
        Model::MatrixComplex augend3(numberRows, numberColumns);
        Model::MatrixComplex addend3(numberRows, numberColumns);

        for (unsigned long columnIndex=1 ; columnIndex<=numberColumns ; ++columnIndex) {
            for (unsigned long rowIndex=1 ; rowIndex<=numberRows ; ++rowIndex) {
                Model::Complex a1 = Model::Complex(coefficientDistribution(rng), coefficientDistribution(rng));
                Model::Complex a2 = Model::Complex(coefficientDistribution(rng), coefficientDistribution(rng));

                augend1.update(rowIndex, columnIndex, a1);
                addend1.update(rowIndex, columnIndex, a2);
                expected1.update(rowIndex, columnIndex, a1 + a2);

                augend2.update(rowIndex, columnIndex, a1);
                addend2.update(rowIndex, columnIndex, a2);
                expected2.update(rowIndex, columnIndex, 2 * a1 + 3 * a2.conj());

                augend3.update(rowIndex, columnIndex, a1);
                addend3.update(rowIndex, columnIndex, a2);
                expected3.update(columnIndex, rowIndex, 2 * a1 + 3 * a2.conj());
            }
        }

        Model::MatrixComplex measured = augend1 + addend1;
        QCOMPARE(measured, expected1);

        measured = 2 * augend2 + 3 * addend2.conj();
        QCOMPARE(measured, expected2);

        measured = 2 * augend3.transpose() + 3 * addend3.adjoint();
        QCOMPARE(measured, expected3);
    }

    Model::MatrixComplex m1(10, 11);
    Model::MatrixComplex m2(12, 13);
    Model::MatrixComplex s;

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


void TestMatrixComplex::testSubtractionOperators() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E10, +1E10);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned numberRows      = dimensionDistribution(rng);
        unsigned numberColumns   = dimensionDistribution(rng);

        Model::MatrixComplex expected1(numberRows, numberColumns);
        Model::MatrixComplex augend1(numberRows, numberColumns);
        Model::MatrixComplex addend1(numberRows, numberColumns);

        Model::MatrixComplex expected2(numberRows, numberColumns);
        Model::MatrixComplex augend2(numberRows, numberColumns);
        Model::MatrixComplex addend2(numberRows, numberColumns);

        Model::MatrixComplex expected3(numberColumns, numberRows);
        Model::MatrixComplex augend3(numberRows, numberColumns);
        Model::MatrixComplex addend3(numberRows, numberColumns);

        for (unsigned long columnIndex=1 ; columnIndex<=numberColumns ; ++columnIndex) {
            for (unsigned long rowIndex=1 ; rowIndex<=numberRows ; ++rowIndex) {
                Model::Complex a1 = Model::Complex(coefficientDistribution(rng), coefficientDistribution(rng));
                Model::Complex a2 = Model::Complex(coefficientDistribution(rng), coefficientDistribution(rng));

                augend1.update(rowIndex, columnIndex, a1);
                addend1.update(rowIndex, columnIndex, a2);
                expected1.update(rowIndex, columnIndex, a1 - a2);

                augend2.update(rowIndex, columnIndex, a1);
                addend2.update(rowIndex, columnIndex, a2);
                expected2.update(rowIndex, columnIndex, 2 * a1 - 3 * a2.conj());

                augend3.update(rowIndex, columnIndex, a1);
                addend3.update(rowIndex, columnIndex, a2);
                expected3.update(columnIndex, rowIndex, 2 * a1 - 3 * a2.conj());
            }
        }

        Model::MatrixComplex measured = augend1 - addend1;
        QCOMPARE(measured, expected1);

        measured = 2 * augend2 - 3 * addend2.conj();
        QCOMPARE(measured, expected2);

        measured = 2 * augend3.transpose() - 3 * addend3.adjoint();
        QCOMPARE(measured, expected3);
    }

    Model::MatrixComplex m1(10, 11);
    Model::MatrixComplex m2(12, 13);
    Model::MatrixComplex s;

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


void TestMatrixComplex::testMatrixMatrixMultiplicationOperator() {
    std::mt19937 rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E10, +1E10);

    // We test three scenarios, the larger the matrix, the fewer times we must iterate to keep the test time
    // reasonable.
    unsigned mode4Iterations = std::max(1U, TestMatrixComplex::numberIterations / 1000);
    unsigned mode3Iterations = std::max(1U, TestMatrixComplex::numberIterations / 200);
    unsigned mode2Iterations = std::max(1U, TestMatrixComplex::numberIterations / 100);
    unsigned mode1Iterations = std::max(1U, TestMatrixComplex::numberIterations /  10);
    unsigned mode0Iterations = (
          TestMatrixComplex::numberIterations
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
                maximumError     = 1.0E-11;

                break;
            }

            case 1: {
                maximumDimension = 200;
                numberIterations = mode1Iterations;
                maximumError     = 4.0E-11;

                break;
            }

            case 2: {
                maximumDimension  = 500;
                numberIterations  = mode2Iterations;
                maximumError      = 1.0E-11;

                break;
            }


            case 3: {
                maximumDimension  = 1000;
                numberIterations  = mode2Iterations;
                maximumError      = 1.0E-11;

                break;
            }

            case 4: {
                maximumDimension = 10000;
                numberIterations = mode4Iterations;
                maximumError      = 1.0E-11;

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

            Model::MatrixComplex multiplier(multiplierRows, multiplierColumns);
            for (unsigned row=1 ; row<=multiplierRows ; ++row) {
                for (unsigned column=1 ; column<multiplierColumns ; ++column) {
                    Model::Complex c(coefficientDistribution(rng), coefficientDistribution(rng));
                    multiplier.update(row, column, c);
                }
            }

            Model::MatrixComplex multiplicand(multiplicandRows, multiplicandColumns);
            for (unsigned row=1 ; row<=multiplicandRows ; ++row) {
                for (unsigned column=1 ; column<multiplicandColumns ; ++column) {
                    Model::Complex c(coefficientDistribution(rng), coefficientDistribution(rng));
                    multiplicand.update(row, column, c);
                }
            }

            Model::MatrixComplex measuredMatrix = multiplier * multiplicand;

            QCOMPARE(measuredMatrix.numberRows(), multiplierRows);
            QCOMPARE(measuredMatrix.numberColumns(), multiplicandColumns);

            for (unsigned row=1 ; row<=multiplierRows ; ++row) {
                for (unsigned column=1 ; column<=multiplicandColumns ; ++column) {
                    Model::Complex expected(0);
                    for (unsigned multiplierColumn=1 ; multiplierColumn<=multiplierColumns ; ++multiplierColumn) {
                        expected += multiplier(row, multiplierColumn) * multiplicand(multiplierColumn, column);
                    }

                    Model::Complex measured = measuredMatrix(row, column);
                    Model::Complex error    = expected - measured;

                    Model::Real relativeError = sqrt((error * error.conj()).real() / (expected * expected).real());

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

    Model::MatrixComplex m1(10, 11);
    Model::MatrixComplex m2(12, 13);
    Model::MatrixComplex p;

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


void TestMatrixComplex::testMatrixScalarMultiplicationOperators() {
    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E10, +1E10);
    std::uniform_int_distribution<>             dimensionDistribution(1, 100);
    Model::Real                                 maximumAllowedComplexError = 1.0E-15;

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned multiplierRows    = dimensionDistribution(rng);
        unsigned multiplierColumns = dimensionDistribution(rng);

        Model::MatrixComplex multiplier(multiplierRows, multiplierColumns);
        for (unsigned row=1 ; row<=multiplierRows ; ++row) {
            for (unsigned column=1 ; column<multiplierColumns ; ++column) {
                Model::Complex c(coefficientDistribution(rng), coefficientDistribution(rng));
                multiplier.update(row, column, c);
            }
        }

        Model::Boolean booleanScalar = (dimensionDistribution(rng) % 2) != 0;
        Model::Integer integerScalar = dimensionDistribution(rng);
        Model::Real    realScalar    = coefficientDistribution(rng);
        Model::Complex complexScalar(coefficientDistribution(rng), coefficientDistribution(rng));

        bool tm = (dimensionDistribution(rng) % 2) != 0;

        Model::MatrixComplex booleanMeasured  = tm ? multiplier * booleanScalar : booleanScalar * multiplier;
        Model::MatrixComplex integerMeasured  = tm ? multiplier * integerScalar : integerScalar * multiplier;
        Model::MatrixComplex realMeasured     = tm ? multiplier * realScalar    : realScalar * multiplier;
        Model::MatrixComplex complexMeasured  = tm ? multiplier * complexScalar : complexScalar * multiplier;

        QCOMPARE(booleanMeasured.numberRows(), multiplierRows);
        QCOMPARE(booleanMeasured.numberColumns(), multiplierColumns);

        QCOMPARE(integerMeasured.numberRows(), multiplierRows);
        QCOMPARE(integerMeasured.numberColumns(), multiplierColumns);

        QCOMPARE(realMeasured.numberRows(), multiplierRows);
        QCOMPARE(realMeasured.numberColumns(), multiplierColumns);

        QCOMPARE(complexMeasured.numberRows(), multiplierRows);
        QCOMPARE(complexMeasured.numberColumns(), multiplierColumns);

        for (unsigned row=1 ; row<=multiplierRows ; ++row) {
            for (unsigned column=1 ; column<=multiplierColumns ; ++column) {
                QCOMPARE(booleanMeasured(row, column), booleanScalar ? multiplier(row, column) : Model::Complex(0, 0));

                Model::Complex integerExpected = multiplier(row, column) * integerScalar;
                QCOMPARE(integerMeasured(row, column), integerExpected);

                Model::Complex realExpected = multiplier(row, column) * realScalar;
                QCOMPARE(realMeasured(row, column), realExpected);

                Model::Complex complexExpected = multiplier(row, column) * complexScalar;
                Model::Complex complexActual   = complexMeasured(row, column);
                Model::Complex complexError    = complexExpected - complexActual;
                Model::Real    relativeError   = std::sqrt(
                    (complexError * complexError.conj()).real()
                    / (complexExpected * complexExpected.conj()).real()
                );
                if (relativeError > maximumAllowedComplexError) {
                    QCOMPARE(relativeError <= maximumAllowedComplexError, true);
                }
            }
        }
    }
}


void TestMatrixComplex::testDivisionOperators() {
    // Divide operators are functionally identical to the multiply operators so we only do limited testing.

    std::mt19937                                rng(0x12345678);
    std::uniform_real_distribution<Model::Real> coefficientDistribution(-1E10, +1E10);
    std::uniform_int_distribution<>             dimensionDistribution(1, 1000);

    unsigned dividendRows    = dimensionDistribution(rng);
    unsigned dividendColumns = dimensionDistribution(rng);

    Model::MatrixComplex dividend(dividendRows, dividendColumns);
    for (unsigned row=1 ; row<=dividendRows ; ++row) {
        for (unsigned column=1 ; column<dividendColumns ; ++column) {
            Model::Complex c(coefficientDistribution(rng), coefficientDistribution(rng));
            dividend.update(row, column, c);
        }
    }

    Model::Integer       integerScalar   = dimensionDistribution(rng);
    Model::MatrixComplex integerMeasured = dividend / integerScalar;

    Model::Real          realScalar   = coefficientDistribution(rng);
    Model::MatrixComplex realMeasured = dividend / realScalar;

    Model::Complex       complexScalar(coefficientDistribution(rng), coefficientDistribution(rng));
    Model::MatrixComplex complexMeasured = dividend / complexScalar;

    QCOMPARE(integerMeasured.numberRows(), dividendRows);
    QCOMPARE(integerMeasured.numberColumns(), dividendColumns);

    QCOMPARE(realMeasured.numberRows(), dividendRows);
    QCOMPARE(realMeasured.numberColumns(), dividendColumns);

    QCOMPARE(complexMeasured.numberRows(), dividendRows);
    QCOMPARE(complexMeasured.numberColumns(), dividendColumns);

    Model::Real maximumError = 0;
    for (unsigned row=1 ; row<=dividendRows ; ++row) {
        for (unsigned column=1 ; column<=dividendColumns ; ++column) {
            Model::Complex integerExpected      = dividend(row, column) / integerScalar;
            Model::Complex integerDifference    = integerMeasured(row, column) - integerExpected;
            Model::Real    integerRelativeError = std::sqrt(
                  (integerDifference * integerDifference.conj()).real()
                / (integerExpected * integerExpected.conj()).real()
            );
            if (integerRelativeError >= 1.0E-11) {
                QCOMPARE(integerRelativeError < 1.0E-15, true);
            }

            Model::Complex realExpected      = dividend(row, column) / realScalar;
            Model::Complex realDifference    = realMeasured(row, column) - realExpected;
            Model::Real    realRelativeError = std::sqrt(
                  (realDifference * realDifference.conj()).real()
                / (realExpected * realExpected.conj()).real()
            );
            if (realRelativeError >= 1.0E-11) {
                QCOMPARE(realRelativeError < 1.0E-15, true);
            }

            Model::Complex complexExpected = dividend(row, column) / complexScalar;
            Model::Complex complexDifference    = complexMeasured(row, column) - complexExpected;
            Model::Real    complexRelativeError = std::sqrt(
                  (complexDifference * complexDifference.conj()).real()
                / (complexExpected * complexExpected.conj()).real()
            );
            if (complexRelativeError >= 1.0E-11) {
                QCOMPARE(complexRelativeError < 1.0E-15, true);
            }

            maximumError = std::max(
                std::max(maximumError, integerRelativeError),
                std::max(realRelativeError, complexRelativeError)
            );
        }
    }

    qDebug() << "maximum error = " << maximumError;
}


void TestMatrixComplex::testUnaryPlusMinusOperators() {
    Model::MatrixComplex e = Model::MatrixComplex::build(
        3, 3,
        Model::Complex(+1, -1), Model::Complex(-2, +1), Model::Complex(-3, -1),
        Model::Complex(-1, +2), Model::Complex(+2, -2), Model::Complex(+3, +2),
        Model::Complex(+1, -3), Model::Complex(+2, -3), Model::Complex(-3, -3)
    );

    Model::MatrixComplex p = +e;
    Model::MatrixComplex m = -e;

    for (unsigned row=1 ; row<=3 ; ++row) {
        for (unsigned column=1 ; column<=3 ; ++column) {
            QCOMPARE(p(row, column), e(row, column));
            QCOMPARE(m(row, column), -e(row, column));
        }
    }
}


void TestMatrixComplex::testComparisonOperators() {
    Model::MatrixComplex m1 = Model::MatrixComplex::build(
        3, 3,
        Model::Complex(+1, -1), Model::Complex(-2, +1), Model::Complex(-3, -1),
        Model::Complex(-1, +2), Model::Complex(+2, -2), Model::Complex(+3, +2),
        Model::Complex(+1, -3), Model::Complex(+2, -3), Model::Complex(-3, -3)
    );

    Model::MatrixComplex m2 = Model::MatrixComplex::build(
        2, 3,
        Model::Complex(+1, -1), Model::Complex(-2, +1),
        Model::Complex(-1, +2), Model::Complex(+2, -2),
        Model::Complex(+1, -3), Model::Complex(+2, -3)
    );

    Model::MatrixComplex m3 = Model::MatrixComplex::build(
        3, 2,
        Model::Complex(+1, -1), Model::Complex(-2, +1), Model::Complex(-3, -1),
        Model::Complex(-1, +2), Model::Complex(+2, -2), Model::Complex(+3, +2)
    );

    Model::MatrixComplex m4 = Model::MatrixComplex::build( // same as m1
        3, 3,
        Model::Complex(+1, -1), Model::Complex(-2, +1), Model::Complex(-3, -1),
        Model::Complex(-1, +2), Model::Complex(+2, -2), Model::Complex(+3, +2),
        Model::Complex(+1, -3), Model::Complex(+2, -3), Model::Complex(-3, -3)
    );

    Model::MatrixComplex m5 = Model::MatrixComplex::build( // one value different from m1
        3, 3,
        Model::Complex(+1, -1), Model::Complex(-2, +1), Model::Complex(-3, -1),
        Model::Complex(-1, +2), Model::Complex(-2, -2), Model::Complex(+3, +2),
        Model::Complex(+1, -3), Model::Complex(+2, -3), Model::Complex(-3, -3)
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


void TestMatrixComplex::testSubscriptingOperators() {
    // Tested by other methods.
}


void TestMatrixComplex::testMultiplicationFunctions() {
    // Tested by TestMatrixComplex::testMatrixScalarMultiplicationOperators
}


void TestMatrixComplex::testRealImagFunctions() {
    Model::MatrixComplex m1 = Model::MatrixComplex::build(
        3, 3,
        Model::Complex(+1.1, -1.1), Model::Complex(-2.1, +1.2), Model::Complex(-3.1, -1.3),
        Model::Complex(-1.2, +2.1), Model::Complex(+2.2, -2.2), Model::Complex(+3.2, +2.3),
        Model::Complex(+1.3, -3.1), Model::Complex(+2.3, -3.2), Model::Complex(-3.3, -3.3)
    );

    Model::MatrixReal r1 = m1.real();
    Model::MatrixReal i1 = m1.imag();

    QCOMPARE(
        r1,
        Model::MatrixReal::build(
            3, 3,
            Model::Real(+1.1), Model::Real(-2.1), Model::Real(-3.1),
            Model::Real(-1.2), Model::Real(+2.2), Model::Real(+3.2),
            Model::Real(+1.3), Model::Real(+2.3), Model::Real(-3.3)
        )
    );

    QCOMPARE(
        i1,
        Model::MatrixReal::build(
            3, 3,
            Model::Real(-1.1), Model::Real(+1.2), Model::Real(-1.3),
            Model::Real(+2.1), Model::Real(-2.2), Model::Real(+2.3),
            Model::Real(-3.1), Model::Real(-3.2), Model::Real(-3.3)
        )
    );

    Model::MatrixComplex m2 = 2.0 * m1.transpose();

    Model::MatrixReal r2 = m2.real();
    Model::MatrixReal i2 = m2.imag();

    QCOMPARE(
        r2,
        Model::MatrixReal::build(
            3, 3,
            Model::Real(+2.2), Model::Real(-4.2), Model::Real(-6.2),
            Model::Real(-2.4), Model::Real(+4.4), Model::Real(+6.4),
            Model::Real(+2.6), Model::Real(+4.6), Model::Real(-6.6)
        ).transpose()
    );

    QCOMPARE(
        i2,
        Model::MatrixReal::build(
            3, 3,
            Model::Real(-2.2), Model::Real(+2.4), Model::Real(-2.6),
            Model::Real(+4.2), Model::Real(-4.4), Model::Real(+4.6),
            Model::Real(-6.2), Model::Real(-6.4), Model::Real(-6.6)
        ).transpose()
    );

    Model::MatrixComplex m3 = 2.0 * m1.adjoint();

    Model::MatrixReal r3 = m3.real();
    Model::MatrixReal i3 = m3.imag();

    QCOMPARE(
        r3,
        Model::MatrixReal::build(
            3, 3,
            Model::Real(+2.2), Model::Real(-4.2), Model::Real(-6.2),
            Model::Real(-2.4), Model::Real(+4.4), Model::Real(+6.4),
            Model::Real(+2.6), Model::Real(+4.6), Model::Real(-6.6)
        ).transpose()
    );

    QCOMPARE(
        i3,
        Model::MatrixReal::build(
            3, 3,
            Model::Real(+2.2), Model::Real(-2.4), Model::Real(+2.6),
            Model::Real(-4.2), Model::Real(+4.4), Model::Real(-4.6),
            Model::Real(+6.2), Model::Real(+6.4), Model::Real(+6.6)
        ).transpose()
    );
}


void TestMatrixComplex::testIterator() {
    M::MatrixComplex m = M::MatrixComplex::build(
        3, 3,
        Model::Complex(+1, -1), Model::Complex(-2, +1), Model::Complex(-3, -1),
        Model::Complex(-1, +2), Model::Complex(+2, -2), Model::Complex(+3, +2),
        Model::Complex(+1, -3), Model::Complex(+2, -3), Model::Complex(-3, -3)
    );

    M::MatrixComplex::Iterator it = m.begin();
    QCOMPARE(it != m.end(), true);
    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);

    QCOMPARE(it.value(), Model::Complex(+1, -1));
    QCOMPARE(it.constReference(), Model::Complex(+1, -1));
    QCOMPARE(*(it.constPointer()), Model::Complex(+1, -1));
    QCOMPARE(*it, Model::Complex(+1, -1));

    ++it;
    QCOMPARE(it != m.end(), true);
    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(*it, Model::Complex(-1, +2));

    M::MatrixComplex::Iterator it2 = it++;
    QCOMPARE(*it2, Model::Complex(-1, +2));

    QCOMPARE(it != m.end(), true);
    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(*it, Model::Complex(+1, -3));

    it.advance(2);

    QCOMPARE(it != m.end(), true);
    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(*it, Model::Complex(+2, -2));

    it.advance(2);

    QCOMPARE(it != m.end(), true);
    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(*it, Model::Complex(-3, -1));

    it.advance(2);

    QCOMPARE(it != m.end(), true);
    QCOMPARE(it.isValid(), true);
    QCOMPARE(it.isInvalid(), false);
    QCOMPARE(*it, Model::Complex(-3, -3));

    ++it;

    QCOMPARE(it == m.end(), true);
    QCOMPARE(it.isValid(), false);
    QCOMPARE(it.isInvalid(), true);
}


void TestMatrixComplex::dumpMatrix(const Model::MatrixComplex& matrix) {
    unsigned long numberRows = matrix.numberRows();
    unsigned long numberColumns = matrix.numberColumns();

    for (unsigned long row=1 ; row<=numberRows ; ++row) {
        QString t;
        for (unsigned long column=1 ; column<=numberColumns ; ++column) {
            if (!t.isEmpty()) {
                t +="  |  ";
            }

            Model::Complex v = matrix(row, column);
            QString r;
            if (v.real() != 0) {
                r = QString("%1").arg(v.real(), 8, 'g').trimmed();
            }

            QString i;
            if (v.imag() < 0) {
                i = QString("%1").arg(v.imag(), 9, 'g').trimmed() + QString("i");
            } else if (v.imag() > 0) {
                i = QString("+") + QString("%1").arg(v.imag(), 8, 'g').trimmed() + QString("i");
            }

            QString n;
            if (r.isEmpty() && i.isEmpty()) {
                n = QString("0");
            } else {
                n = r + i;

            }

            t += QString("%1").arg(n, 20);
        }

        qDebug() << t;
    }
}
