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
* This header defines the \ref M::PerThread::XorShiRo256Base class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_PER_THREAD_XORSHIRO256_BASE_H
#define M_PER_THREAD_XORSHIRO256_BASE_H

#include "model_rng.h"
#include "m_intrinsics.h"
#include "m_per_thread.h"
#include "m_per_thread_rng_base.h"

namespace M {
    /**
     * The \ref M::PerThread RNG base class for the XOR-SHIFT-ROTATE 256 + and ** algorithms.
     */
    class PerThread::XorShiRo256Base:public PerThread::RngBase {
        public:
            XorShiRo256Base();

            /**
             * Constructor
             *
             * \param[in] rngSeed The desired seed for the RNG.
             *
             * \param[in] extra   A extra value applied in addition to the provided seed.  The value is intended to
             *                    provide additional scrambling based on a thread ID or similar construct.
             */
            XorShiRo256Base(const RngSeed& rngSeed, unsigned extra);

            ~XorShiRo256Base() override;

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

        protected:
            /**
             * The current generator state.  Direct access to derived classes for speed.
             */
            std::uint64_t state[4];

        private:
            /**
             * Method that provides a trivial linear congruent generator in 64-bits.  Algorithm from :
             *
             *     https://nuclear.llnl.gov/CNP/rng/rngman/node4.html
             *
             * \param[in] current The current linear congruent generator value.
             *
             * \return Returns the next linear congruent generator value.
             */
            static inline std::uint64_t lcg(std::uint64_t current) {
                return 2862933444777941757 * current + 3037000493;
            }

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
