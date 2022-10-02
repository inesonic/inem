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
* This file implements the \ref M::PerThread::XorShiRo256Stars class.
***********************************************************************************************************************/

#include <cstdint>
#include <cstring>

#include "model_exceptions.h"
#include "m_console.h"
#include "m_intrinsics.h"
#include "m_per_thread_rng_base.h"
#include "m_per_thread_xorshiro256_stars.h"

namespace M {
    PerThread::XorShiRo256Stars::XorShiRo256Stars():PerThread::XorShiRo256Base() {}


    PerThread::XorShiRo256Stars::XorShiRo256Stars(
            const PerThread::RngSeed& rngSeed,
            unsigned                  extra
        ):PerThread::XorShiRo256Base(
            rngSeed,
            extra
        ) {}


    PerThread::XorShiRo256Stars::~XorShiRo256Stars() {}


    unsigned long long PerThread::XorShiRo256Stars::random64() {
        std::uint64_t s0 = state[0];
        std::uint64_t s1 = state[1];
        std::uint64_t s2 = state[2];
        std::uint64_t s3 = state[3];

        std::uint64_t t      = s1 << 17;
        std::uint64_t x      = s1 * 5;
        std::uint64_t result = ((x << 7) | (x >> 57)) * 9;

        s2 ^= s0;
        s3 ^= s1;
        s1 ^= s2;
        s0 ^= s3;

        s2 ^= t;
        s3 = (s3 << 45) | (s3 >> 19);

        state[0] = s0;
        state[1] = s1;
        state[2] = s2;
        state[3] = s3;

        return result;
    }


    void PerThread::XorShiRo256Stars::fillArray(void* array, unsigned long numberValues) {
        std::uint64_t* p  = reinterpret_cast<std::uint64_t*>(array);

        std::uint64_t  s0 = state[0];
        std::uint64_t  s1 = state[1];
        std::uint64_t  s2 = state[2];
        std::uint64_t  s3 = state[3];

        for (unsigned long i=0 ; i<numberValues ; ++i) {
            std::uint64_t x = s1 * 5;
            p[i] = ((x << 7) | (x >> 57)) * 9;

            std::uint64_t t = s1 << 17;
            s2 ^= s0;
            s3 ^= s1;
            s1 ^= s2;
            s0 ^= s3;

            s2 ^= t;
            s3 = (s3 << 45) | (s3 >> 19);
        }

        state[0] = s0;
        state[1] = s1;
        state[2] = s2;
        state[3] = s3;
    }
}
