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
* This file implements the \ref M::PerThread::XorShiRo256Base class.
***********************************************************************************************************************/

#include <cstdint>
#include <cstring>

#include "model_exceptions.h"
#include "m_console.h"
#include "m_intrinsics.h"
#include "m_per_thread_rng_base.h"
#include "m_per_thread_xorshiro256_base.h"

namespace M {
    PerThread::XorShiRo256Base::XorShiRo256Base() {
        RngSeed seed;
        trng(reinterpret_cast<std::uint32_t*>(seed), sizeof(RngSeed) / sizeof(std::uint32_t));
        setRngSeed(seed, 0);
    }


    PerThread::XorShiRo256Base::XorShiRo256Base(const PerThread::RngSeed& rngSeed, unsigned extra) {
        setRngSeed(rngSeed, extra);
    }


    PerThread::XorShiRo256Base::~XorShiRo256Base() {}


    void PerThread::XorShiRo256Base::setRngSeed(const PerThread::RngSeed& rngSeed, unsigned extra) {
        std::memcpy(state, rngSeed, sizeof(RngSeed));

        std::uint64_t lcgValue = lcg(extra);

        state[0] += lcgValue;
        lcgValue = lcg(lcgValue);

        state[1] += lcgValue;
        lcgValue = lcg(lcgValue);

        state[2] += lcgValue;
        lcgValue = lcg(lcgValue);

        state[3] += lcgValue;
        lcgValue = lcg(lcgValue);

        std::memcpy(initialSeedValue, rngSeed, sizeof(RngSeed));
        initialExtraValue     = extra;
        needNewDecomposeValue = true;
    }


    const PerThread::RngSeed& PerThread::XorShiRo256Base::rngSeed() const {
        return initialSeedValue;
    }


    PerThread::RngType PerThread::XorShiRo256Base::rngType() const {
        return RngType::XORSHIRO256_PLUS;
    }


    unsigned long PerThread::XorShiRo256Base::random32() {
        unsigned long result;

        if (needNewDecomposeValue) {
            currentDecomposedValue = random64();
            result = static_cast<std::uint32_t>(currentDecomposedValue);
        } else {
            result = static_cast<std::uint32_t>(currentDecomposedValue >> 32ULL);
        }

        return result;
    }
}
