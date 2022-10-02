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
* This file implements the \ref M::PerThread::MT19937 class.
***********************************************************************************************************************/

#include <cstdint>
#include <cstring>
#include <algorithm>

#include "model_exceptions.h"
#include "m_console.h"
#include "m_intrinsics.h"
#include "m_per_thread_rng_base.h"
#include "m_per_thread_mt19937.h"

namespace M {
    const std::uint64_t PerThread::MT19937::mag01[2] = { 0, PerThread::MT19937::matrixA };

    PerThread::MT19937::MT19937() {
        RngSeed seed;
        trng(reinterpret_cast<std::uint32_t*>(seed), sizeof(RngSeed) / sizeof(std::uint32_t));
        setRngSeed(seed, 0);
    }


    PerThread::MT19937::MT19937(const PerThread::RngSeed& rngSeed, unsigned extra) {
        setRngSeed(rngSeed, extra);
    }


    PerThread::MT19937::~MT19937() {}


    void PerThread::MT19937::setRngSeed(const PerThread::RngSeed& rngSeed, unsigned extra) {
        std::uint64_t seed[rngSeedLength + 1];
        std::memcpy(seed, rngSeed, sizeof(RngSeed));
        seed[rngSeedLength] = extra;

        std::uint64_t v = initializerValue1;
        stateVector[0] = v;

        for (unsigned i=1 ; i<stateVectorLength ; ++i) {
            v = (initializerValue2 * (v ^ (v >> 62)) + i);
            stateVector[i] = v;
        }

        unsigned i=1;
        unsigned j=0;

        for (unsigned k=stateVectorLength ; k ; --k) {
            std::uint64_t vim1 = stateVector[i - 1];
            stateVector[i] = (stateVector[i] ^ ((vim1 ^ (vim1 >> 62)) * initializerValue3)) + seed[j] + j;

            ++i;
            if (i >= stateVectorLength) {
                stateVector[0] = stateVector[lastStateVectorEntry];
                i = 1;
            }

            ++j;
            if (j > rngSeedLength) {
                j = 0;
            }
        }

        for (unsigned k=(stateVectorLength-1) ; k ; --k) {
            std::uint64_t vim1 = stateVector[i - 1];
            stateVector[i] = (stateVector[i] ^ ((vim1 ^ (vim1 >> 62)) * initializerValue4)) - i;
            ++i;
            if (i >= stateVectorLength) {
                stateVector[0] = stateVector[lastStateVectorEntry];
                i = 1;
            }
        }

        stateVector[0] = 0x8000000000000000ULL;
        stateVectorIndex = stateVectorLength;

        std::memcpy(initialSeedValue, rngSeed, sizeof(RngSeed));
        initialExtraValue = extra;

        needNewDecomposeValue = true;
    }


    const PerThread::RngSeed& PerThread::MT19937::rngSeed() const {
        return initialSeedValue;
    }


    PerThread::RngType PerThread::MT19937::rngType() const {
        return RngType::MT19937;
    }


    unsigned long PerThread::MT19937::random32() {
        unsigned long result;

        if (needNewDecomposeValue) {
            currentDecomposedValue = random64();
            result = static_cast<std::uint32_t>(currentDecomposedValue);
        } else {
            result = static_cast<std::uint32_t>(currentDecomposedValue >> 32ULL);
        }

        return result;
    }


    unsigned long long PerThread::MT19937::random64() {
        if (stateVectorIndex >= stateVectorLength) {
            updateStateVector();
            stateVectorIndex = 0;
        }

        std::uint64_t result = stateVector[stateVectorIndex];
        ++stateVectorIndex;

        result ^= (result >> 29) & 0x5555555555555555ULL;
        result ^= (result << 17) & 0x71D67FFFEDA60000ULL;
        result ^= (result << 37) & 0xFFF7EEE000000000ULL;
        result ^= (result >> 43);

        return result;
    }


    void PerThread::MT19937::fillArray(void* array, unsigned long numberValues) {
        std::uint64_t* p = reinterpret_cast<std::uint64_t*>(array);

        unsigned long remaining = numberValues;

        if (stateVectorIndex >= stateVectorLength) {
            updateStateVector();
            stateVectorIndex = 0;
        }

        unsigned long remainingFirstSet = stateVectorLength - stateVectorIndex;
        unsigned long numberToFill      = std::min(numberValues, remainingFirstSet);
        if (numberToFill > 0) {
            std::memcpy(p, stateVector + stateVectorIndex, numberToFill * sizeof(std::uint64_t));
            p                += numberToFill;
            remaining        -= numberToFill;
            stateVectorIndex += numberToFill;

            if (stateVectorIndex >= stateVectorLength) {
                updateStateVector();
                stateVectorIndex = 0;
            }
        }

        while (remaining >= stateVectorLength) {
            std::memcpy(p, stateVector, stateVectorLength * sizeof(std::uint64_t));
            p         += stateVectorLength;
            remaining -= stateVectorLength;

            updateStateVector();
        }

        if (remaining > 0) {
            std::memcpy(p, stateVector, remaining * sizeof(std::uint64_t));
            stateVectorIndex += remaining;
        }

        p = reinterpret_cast<std::uint64_t*>(array);
        for (unsigned long long i=0 ; i<numberValues ; ++i) {
            std::uint64_t result = p[i];
            result ^= (result >> 29) & 0x5555555555555555ULL;
            result ^= (result << 17) & 0x71D67FFFEDA60000ULL;
            result ^= (result << 37) & 0xFFF7EEE000000000ULL;
            result ^= (result >> 43);

            p[i] = result;
        }
    }


    void PerThread::MT19937::updateStateVector() {
        std::uint64_t x;
        unsigned      i = 0;
        do {
            x = (stateVector[i] & upperBits) | (stateVector[i+1] & lowerBits);
            stateVector[i] = stateVector[i + splitPoint] ^ (x >> 1) ^ mag01[static_cast<unsigned>(x & 1)];

            ++i;
        } while (i < splitLength);

        do {
            x = (stateVector[i] & upperBits) | (stateVector[i+1] & lowerBits);
            stateVector[i] = stateVector[i - splitLength] ^ (x>>1) ^ mag01[static_cast<unsigned>(x & 1)];

            ++i;
        } while (i < lastStateVectorEntry);

        x = (stateVector[lastStateVectorEntry] & upperBits) | (stateVector[0] & lowerBits);
        stateVector[lastStateVectorEntry] = stateVector[splitPoint-1] ^ (x>>1) ^ mag01[static_cast<unsigned>(x & 1)];
    }
}
