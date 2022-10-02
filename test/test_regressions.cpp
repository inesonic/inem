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
* This file implements tests for possible code regressions that don't belong in other locations.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QObject>
#include <QList>
#include <QtTest/QtTest>

#include <iostream> // Debug
#include <limits>
#include <complex>
#include <cmath>
#include <random>

#include <boost/math/special_functions/gamma.hpp>
#include <boost/math/special_functions/erf.hpp>

#include <m_intrinsic_types.h>
#include <m_assignment_functions.h>
#include <m_per_thread.h>
#include <m_variant.h>
#include <m_set.h>
#include <m_set_iterator.h>
#include <m_tuple.h>
#include <m_matrix_integer.h>
#include <m_matrix_real.h>
#include <m_basic_functions.h>
#include <m_assignment_functions.h>
#include <m_statistical_functions.h>
#include <m_set_functions.h>

#include "test_regressions.h"

TestRegressions::TestRegressions() {}


TestRegressions::~TestRegressions() {}


void TestRegressions::testBug142() {
    M::Variant event;
    M::Boolean busy;
    M::Real rho;
    M::Integer numberArrivals;
    M::Real current;
    M::Set events;
    M::MatrixInteger lengthMatrix;
    M::Integer arrival;
    M::Integer index;
    M::Integer currentQueueDepth;
    M::MatrixReal eventTime;

    M::PerThread::RngSeed rngSeed = { 1, 2,3, 4 };
    M::PerThread pt(0, M::PerThread::RngType::MT19937, rngSeed, nullptr, nullptr);
    pt.threadLocalSetup();

    M::assign(numberArrivals,M::Integer(1000));
    M::assign(arrival,M::Integer(0));
    M::assign(current,M::Real(0.0000000000000000L));
    M::assign(currentQueueDepth,M::Integer(0));
    M::assign(busy,false);
    M::assign(event,M::Tuple::build());
    M::assign(events,M::Set::build((M::Tuple::build((M::Integer(0)),(true)))));
    M::assign(rho,M::Real(0.9000000000000000L));
    M::assign(index,M::Integer(0));

    unsigned i=1;
    std::cerr << "-- Start iterator (" << i << ")" << std::endl;
    for (const auto& LRI35 : events) {
        std::cerr << "   End iterator" << std::endl;
        event = LRI35;
        [&](){
            M::Variant _l;
            M::assign(current,(event).at(M::Integer(1)));
            M::assign(_l,(event).at(M::Integer(2)));
            if (M::inlineToBoolean(_l)) {
                [&](){
                    M::assign(currentQueueDepth,(currentQueueDepth+M::Integer(1)));
                    M::assign(arrival,(arrival+M::Integer(1)));
                    if (M::inlineToBoolean((arrival<numberArrivals))) {
                        [&](){
                            M::Real _m;
                            M::assign(_m,(current+M::exponentialDeviate(pt,(rho))));
                            M::assign(events,M::unionOf((events),(M::Set::build((M::Tuple::build((_m),(true)))))));
                        }();
                    }
                }();
            } else {
                [&](){
                    M::assign(currentQueueDepth,(currentQueueDepth-M::Integer(1)));
                    M::assign(busy,false);
                }();
            }

            if (M::inlineToBoolean(((!busy)&&(currentQueueDepth>M::Integer(0))))) {
                [&](){
                    M::Real _n;
                    M::assign(_n,(current+M::exponentialDeviate(pt,(M::Real(1.0000000000000000L)))));
                    M::assign(events,M::unionOf((events),(M::Set::build((M::Tuple::build((_n),(false)))))));
                    M::assign(busy,true);
                }();
            }

            M::assign(index,(index+M::Integer(1)));
            (eventTime).update((index),(current));
            (lengthMatrix).update((index),(currentQueueDepth));
        }();

        ++i;
        std::cerr << "-- Start iterator (" << i << ")" << std::endl;
    }
    std::cerr << "   End iterator" << std::endl;
}
