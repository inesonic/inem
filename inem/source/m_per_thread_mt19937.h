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
* This header defines the \ref M::PerThread::MT19937 class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_PER_THREAD_MT19937_H
#define M_PER_THREAD_MT19937_H

#include <cstdint>

#include "model_rng.h"
#include "m_intrinsics.h"
#include "m_per_thread.h"
#include "m_per_thread_rng_base.h"

namespace M {
    /**
     * The \ref M::PerThread RNG base class for the MT19937 PRNG.  This version is modified slightly from the original
     * to improve performance.
     *
     * Copyright on the original implementation follows:
     *
     *   Copyright (C) 2004, Makoto Matsumoto and Takuji Nishimura,
     *   All rights reserved.
     *
     *   Redistribution and use in source and binary forms, with or without
     *   modification, are permitted provided that the following conditions
     *   are met:
     *
     *     1. Redistributions of source code must retain the above copyright
     *        notice, this list of conditions and the following disclaimer.
     *
     *     2. Redistributions in binary form must reproduce the above copyright
     *        notice, this list of conditions and the following disclaimer in the
     *        documentation and/or other materials provided with the distribution.
     *
     *     3. The names of its contributors may not be used to endorse or promote
     *        products derived from this software without specific prior written
     *        permission.
     *
     *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
     *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
     *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
     *   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
     *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
     *   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
     *   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
     *   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
     *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
     *   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
     *   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
     *
     *   References:
     *     T. Nishimura, ``Tables of 64-bit Mersenne Twisters''
     *     ACM Transactions on Modeling and
     *     Computer Simulation 10. (2000) 348--357.
     *   M. Matsumoto and T. Nishimura,
     *     ``Mersenne Twister: a 623-dimensionally equidistributed
     *       uniform pseudorandom number generator''
     *     ACM Transactions on Modeling and
     *     Computer Simulation 8. (Jan. 1998) 3--30.
     */
    class PerThread::MT19937:public PerThread::RngBase {
        public:
            MT19937();

            /**
             * Constructor
             *
             * \param[in] rngSeed The desired seed for the RNG.
             *
             * \param[in] extra   A extra value applied in addition to the provided seed.  The value is intended to
             *                    provide additional scrambling based on a thread ID or similar construct.
             */
            MT19937(const RngSeed& rngSeed, unsigned extra);

            ~MT19937() override;

            /**
             * Method you can use to set the RNG seed.
             *
             * \param[in] rngSeed The RNG seed to apply.
             *
             * \param[in] extra   A extra value applied in addition to the provided seed.  The value is intended to
             *                    provide additional scrambling based on a thread ID or similar construct.
             */
            void setRngSeed(const RngSeed& rngSeed, unsigned extra) override;

            /**
             * Method you can use to determine the currently selected RNG type.
             *
             * \return Returns the currently selected RNG type.
             */
            RngType rngType() const override;

            /**
             * Method you can use to determine the last used RNG seed.
             *
             * \return Returns a constant reference to the last seed applied to the selected RNG.
             */
            const RngSeed& rngSeed() const override;

            /**
             * Method you can use to obtain an unsigned 32-bit random value.
             *
             * \return Returns an unsigned 32-bit random value with uniform distribution.
             */
            unsigned long random32() override;

            /**
             * Method you can use to obtain an unsigned 64-bit random value.
             *
             * \return Returns an unsigned 32-bit random value with uniform distribution.
             */
            unsigned long long random64() override;

            /**
             * Method you can use to rapidly populate an arbitrary length array with 64-bit random integer values.
             * The algorithm used depends on the underlying RNG.  Note that the RNG may throw away values to force
             * memory alignment or to further optimize the fill operation.
             *
             * \param[in] array        Pointer to the starting location to be populated.
             *
             * \param[in] numberValues The length of the array to be populated.
             */
            void fillArray(void* array, unsigned long numberValues) override;

        private:
            /**
             * The alternation matrix.
             */
            static const std::uint64_t mag01[2];

            /**
             * Length of the MT state vector.
             */
            static constexpr unsigned stateVectorLength = 312; /* NN */

            /**
             * Last entry in the state vector.
             */
            static constexpr unsigned lastStateVectorEntry = stateVectorLength - 1;

            /**
             * The split point used to represent the start of the bottom half of the state vector.
             */
            static constexpr unsigned splitPoint = stateVectorLength/2; /* MM */

            /**
             * The length of each split.
             */
            static constexpr unsigned splitLength = stateVectorLength - splitPoint;

            /**
             * Constant used for the alternation matrix.
             */
            static constexpr std::uint64_t matrixA = 0xB5026F5AA96619E9ULL;

            /**
             * Value used to mask in the upper 33 bits of a 64-bit value.
             */
            static constexpr std::uint64_t upperBits = 0xFFFFFFFF80000000ULL;

            /**
             * Value used to mask in the lower 31 bits of a 64-bit value.
             */
            static constexpr std::uint64_t lowerBits = 0x7FFFFFFFULL;         /* Least significant 31 bits */

            static constexpr std::uint64_t initializerValue1 = 19650218ULL;
            static constexpr std::uint64_t initializerValue2 = 6364136223846793005ULL;
            static constexpr std::uint64_t initializerValue3 = 3935559000370003845ULL;
            static constexpr std::uint64_t initializerValue4 = 2862933555777941757ULL;

            /**
             * Method used internally to update the state vector.
             */
            void updateStateVector();

            /**
             * State vector array.
             */
            std::uint64_t stateVector[stateVectorLength]; /* mt */

            /**
             * Index into the state vector.
             */
            unsigned stateVectorIndex; /* mti */

            /**
             * Initial seed value.
             */
            RngSeed initialSeedValue;

            /**
             * Initial "extra" value.
             */
            unsigned initialExtraValue;

            /**
             * Value used for 32-bit values.
             */
            std::uint64_t currentDecomposedValue;

            /**
             * Flag indicating if we need to fetch a new value to decompose.
             */
            bool needNewDecomposeValue;
    };
};

#endif
