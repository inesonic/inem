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
* This header defines the \ref M::PerThread::MT216091 class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_PER_THREAD_MT216091_H
#define M_PER_THREAD_MT216091_H

#include <cstdint>
#include <emmintrin.h>

#include <sfmt_params_216091.h> /* Taken directly from the SFMT source without modificaiton*/

#include "model_rng.h"
#include "m_intrinsics.h"
#include "m_per_thread.h"
#include "m_per_thread_rng_base.h"

namespace M {
    /**
     * The \ref M::PerThread RNG base class for the MT216091 PRNG.
     */
    class PerThread::MT216091:public PerThread::RngBase {
        public:
            MT216091();

            /**
             * Constructor
             *
             * \param[in] rngSeed The desired seed for the RNG.
             *
             * \param[in] extra   A extra value applied in addition to the provided seed.  The value is intended to
             *                    provide additional scrambling based on a thread ID or similar construct.
             */
            MT216091(const RngSeed& rngSeed, unsigned extra);

            ~MT216091() override;

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
             * Function that allocates the internal state vector.
             */
            void allocateStateVector();

            /**
             * The state array unaligned pointer.
             */
            void* stateVectorUnaligned;

            /**
             * The state array.
             */
            std::uint64_t* stateVector;

            /**
             * The length of the state vector, in 64-bit values.
             */
            unsigned stateVectorLength;

            /**
             * The current index into the state vector.
             */
            unsigned stateVectorIndex;

            /**
             * Initial seed value.
             */
            RngSeed initialSeedValue;

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
