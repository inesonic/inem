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
* This file implements the \ref M::PerThread::MT216091 class.
***********************************************************************************************************************/

#include <cstring>
#include <algorithm>

#include <mat_api.h>

#include "model_exceptions.h"
#include "m_console.h"
#include "m_intrinsics.h"
#include "m_per_thread_rng_base.h"
#include "m_per_thread_mt216091.h"

#include <sfmt_params_216091.h> /* Taken directly from the SFMT source without modificaiton*/

namespace M {
    PerThread::MT216091::MT216091() {
        allocateStateVector();

        RngSeed seed;
        trng(reinterpret_cast<std::uint32_t*>(seed), sizeof(RngSeed) / sizeof(std::uint32_t));

        MT216091::setRngSeed(seed, 0);
    }


    PerThread::MT216091::MT216091(const PerThread::RngSeed& rngSeed, unsigned extra) {
        allocateStateVector();
        MT216091::setRngSeed(rngSeed, extra);
    }


    PerThread::MT216091::~MT216091() {
        releaseMemory(stateVectorUnaligned);
    }


    void PerThread::MT216091::setRngSeed(const PerThread::RngSeed& rngSeed, unsigned extra) {
        static constexpr unsigned newSeedSize = (
              (sizeof(rngSeed) + sizeof(unsigned) + sizeof(std::uint64_t) - 1)
            / sizeof(std::uint64_t)
        );

        std::memcpy(initialSeedValue, rngSeed, sizeof(rngSeed));

        std::uint64_t newSeed[newSeedSize];
        std::memset(newSeed, 0, sizeof(newSeed));
        std::memcpy(newSeed, rngSeed, sizeof(rngSeed));
        *reinterpret_cast<unsigned*>(reinterpret_cast<std::uint8_t*>(newSeed) + sizeof(rngSeed)) = extra;

        matrixApi()->mt216091Seed(stateVector, newSeed, newSeedSize);
        stateVectorIndex = stateVectorLength;
    }


    const PerThread::RngSeed& PerThread::MT216091::rngSeed() const {
        return initialSeedValue;
    }


    PerThread::RngType PerThread::MT216091::rngType() const {
        return RngType::MT216091;
    }


    unsigned long PerThread::MT216091::random32() {
        unsigned long result;

        if (needNewDecomposeValue) {
            currentDecomposedValue = random64();
            result = static_cast<std::uint32_t>(currentDecomposedValue);
        } else {
            result = static_cast<std::uint32_t>(currentDecomposedValue >> 32ULL);
        }

        return result;
    }


    unsigned long long PerThread::MT216091::random64() {
        if (stateVectorIndex >= stateVectorLength) {
            matrixApi()->mt216091Update(stateVector);
            stateVectorIndex = 0;
        }

        unsigned long long result = stateVector[stateVectorIndex];
        ++stateVectorIndex;

        return result;
    }


    void PerThread::MT216091::fillArray(void* array, unsigned long numberValues) {
        std::uint64_t* p                = reinterpret_cast<std::uint64_t*>(array);
        unsigned long  remainingThisSet = stateVectorLength - stateVectorIndex;

        if (remainingThisSet) {
            unsigned long numberToCopy = std::min(numberValues, remainingThisSet);
            memcpy(p, stateVector + stateVectorIndex, numberToCopy * sizeof(std::uint64_t));
            stateVectorIndex += numberToCopy;
            p += numberToCopy;
        }

        while (numberValues >= stateVectorLength) {
            matrixApi()->mt216091Update(stateVector);
            memcpy(p, stateVector, stateVectorLength * sizeof(std::uint64_t));
            numberValues -= stateVectorLength;
            p += stateVectorLength;
        }

        if (numberValues > 0) {
            matrixApi()->mt216091Update(stateVector);
            memcpy(p, stateVector, numberValues * sizeof(std::uint8_t));
            stateVectorIndex = numberValues;
        }
    }


    void PerThread::MT216091::allocateStateVector() {
        stateVectorLength = matrixApi()->mt216091ValueArraySize();
        stateVector       = reinterpret_cast<std::uint64_t*>(
            allocateMemory(
                stateVectorUnaligned,
                stateVectorLength * sizeof(std::uint64_t),
                0
            )
        );
    }
}
