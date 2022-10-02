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
* This header defines the \ref M::UnorderedSetBase class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_UNORDERED_SET_BASE_H
#define M_UNORDERED_SET_BASE_H

#include "m_common.h"
#include "m_intrinsics.h"
#include "m_list.h"

namespace M {
    /**
     * Base class for unordered sets.  Provides a small collection of necessary helper methods used by sets independent
     * of the applied type.
     */
    class M_PUBLIC_API UnorderedSetBase {
        public:
            /**
             * Type used to represent index values within an unordered set.
             */
            typedef UI32 Index;

            /**
             * Value used to represent an invalid index.
             */
            static const Index invalidIndex;

            /**
             * Initial seed for the FNV-1a hash.
             */
            static constexpr UI64 fnv1aOffsetBasis = 14695981039346656037ULL;

            UnorderedSetBase();

            ~UnorderedSetBase();

            /**
             * Method that calculates the next prime above a given value.
             *
             * \param[in] value The starting value to calculate the next prime from.
             */
            static Index calculateNextPrime(Index value);

            /**
             * Method that calculates an FNV-1 hash on an 8-bit value.
             *
             * \param[in] value The value to apply the CRC to.
             *
             * \param[in] hash  The initial generator seed value.
             *
             * \return Returns the resulting calculated value.
             */
            static UI64 fnv1a8(UI8 value, UI64 hash = fnv1aOffsetBasis);

            /**
             * Method that calculates an FNV-1 hash on a 16-bit value.
             *
             * \param[in] value The value to apply the CRC to.
             *
             * \param[in] hash  The initial generator seed value.
             *
             * \return Returns the resulting calculated value.
             */
            static UI64 fnv1a16(UI16 value, UI64 hash = fnv1aOffsetBasis);

            /**
             * Method that calculates an FNV-1 hash on a 32-bit value.
             *
             * \param[in] value The value to apply the CRC to.
             *
             * \param[in] hash  The initial generator seed value.
             *
             * \return Returns the resulting calculated value.
             */
            static UI64 fnv1a32(UI32 value, UI64 hash = fnv1aOffsetBasis);

            /**
             * Method that calculates an FNV-1 hash on a 64-bit value.
             *
             * \param[in] value The value to apply the CRC to.
             *
             * \param[in] hash  The initial generator seed value.
             *
             * \return Returns the resulting calculated value.
             */
            static UI64 fnv1a64(UI64 value, UI64 hash = fnv1aOffsetBasis);

        protected:
            /**
             * Method you can call to calculate a recommended hash table size based on a specified reservation.
             *
             * \param[in] reservation The desired reservation for the table.
             *
             * \return Returns a recommended size, in entries.
             */
            static Index calculateRecommendedTableSize(Index reservation);

        private:
            /**
             * Factor used to calculate the hash table size based on a reservation.  The underlying hash table will be
             * set to the next prime value greater than this factor times the reservation.
             */
            static constexpr float reservationMargin = 1.5F;

            /**
             * The FNV-1 prime
             */
            static constexpr UI64 fnv1Prime = 1099611628211ULL;
    };
};

#endif
