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
* This header defines the \ref M::PerThread::RngBase class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_PER_THREAD_RNG_BASE_H
#define M_PER_THREAD_RNG_BASE_H

#include <cstdint>

#include "model_rng.h"
#include "m_intrinsics.h"
#include "m_matrix_private_base.h"
#include "m_per_thread.h"

namespace M {
    /**
     * The \ref M::PerThread RNG base class.
     */
    class PerThread::RngBase:public MatrixPrivateBase {
        public:
            /**
             * The RNG type codes.
             */
            typedef Model::Rng::RngType RngType;

            /**
             * THe RNG seed array.@brief RngBase
             */
            typedef Model::Rng::RngSeed RngSeed;

            RngBase();

            virtual ~RngBase();

            /**
             * Method that returns an OS specific true random value.
             *
             * \return Returns a true random value.
             */
            std::uint32_t trng();

            /**
             * Method that populates an array with true random values.
             *
             * \param[in] array       The array to receive the random values.
             *
             * \param[in] numberTerms The number of desired RNG terms.
             */
            void trng(std::uint32_t* array, unsigned numberTerms);

            /**
             * Method you can use to set the RNG seed.
             *
             * \param[in] rngSeed The RNG seed to apply.
             *
             * \param[in] extra   A extra value applied in addition to the provided seed.  The value is intended to
             *                    provide additional scrambling based on a thread ID or similar construct.
             */
            virtual void setRngSeed(const RngSeed& rngSeed, unsigned extra) = 0;

            /**
             * Method you can use to determine the currently selected RNG type.
             *
             * \return Returns the currently selected RNG type.
             */
            virtual RngType rngType() const = 0;

            /**
             * Method you can use to determine the last used RNG seed.
             *
             * \return Returns a constant reference to the last seed applied to the selected RNG.
             */
            virtual const RngSeed& rngSeed() const = 0;

            /**
             * Method you can use to obtain an unsigned 32-bit random value.
             *
             * \return Returns an unsigned 32-bit random value with uniform distribution.
             */
            virtual unsigned long random32() = 0;

            /**
             * Method you can use to obtain an unsigned 64-bit random value.
             *
             * \return Returns an unsigned 32-bit random value with uniform distribution.
             */
            virtual unsigned long long random64() = 0;

            /**
             * Method you can use to rapidly populate an arbitrary length array with 64-bit random integer values.
             * The algorithm used depends on the underlying RNG.  Note that the RNG may throw away values to force
             * memory alignment or to further optimize the fill operation.
             *
             * \param[in] array        Pointer to the starting location to be populated.
             *
             * \param[in] numberValues The length of the array to be populated.
             */
            virtual void fillArray(void* array, unsigned long numberValues) = 0;
    };
};

#endif
