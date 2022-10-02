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
* This header defines the \ref M::PerThread::XorShiRo256Stars class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_PER_THREAD_XORSHIRO256_STARS_H
#define M_PER_THREAD_XORSHIRO256_STARS_H

#include <cstdint>

#include "model_rng.h"
#include "m_intrinsics.h"
#include "m_per_thread.h"
#include "m_per_thread_xorshiro256_base.h"

namespace M {
    /**
     * The \ref M::PerThread RNG base class for the XOR-SHIFT-ROTATE 256** PRNG.
     *
     * The core algorithm from:
     *     Marsaglia, George (July 2003). "Xorshift RNGs". Journal of Statistical Software. 8 (14).
     *     doi:10.18637/jss.v008.i14.
     *
     * Implementation based on pseudo code at:
     *
     *     https://en.wikipedia.org/wiki/Xorshift
     */
    class PerThread::XorShiRo256Stars:public PerThread::XorShiRo256Base {
        public:
            XorShiRo256Stars();

            /**
             * Constructor
             *
             * \param[in] rngSeed The desired seed for the RNG.
             *
             * \param[in] extra   A extra value applied in addition to the provided seed.  The value is intended to
             *                    provide additional scrambling based on a thread ID or similar construct.
             */
            XorShiRo256Stars(const RngSeed& rngSeed, unsigned extra);

            ~XorShiRo256Stars() override;

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
    };
};

#endif
