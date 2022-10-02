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
* This header defines a number of compiler intrinsic types.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_INTRINSICS_H
#define M_INTRINSICS_H

#if (defined(INEM_USE_OS_ESCAPE))

    #include "m_os_escape.h"

#endif

namespace M {
    #if (defined(INEM_USE_OS_ESCAPE))

        /**
         * Platform agnostic guaranteed signed 8 bit integer type.
         */
        typedef std::int8_t   SI8;

        /**
         * Platform agnostic guaranteed unsigned 8 bit integer type.
         */
        typedef std::uint8_t  UI8;

        /**
         * Platform agnostic guaranteed signed 16 bit integer type.
         */
        typedef std::int16_t  SI16;

        /**
         * Platform agnostic guaranteed unsigned 16 bit integer type.
         */
        typedef std::uint16_t UI16;

        /**
         * Platform agnostic guaranteed signed 32 bit integer type.
         */
        typedef std::int32_t  SI32;

        /**
         * Platform agnostic guaranteed unsigned 32 bit integer type.
         */
        typedef std::uint32_t UI32;

        /**
         * Platform agnostic guaranteed signed 64 bit integer type.
         */
        typedef std::int64_t  SI64;

        /**
         * Platform agnostic guaranteed unsigned 64 bit integer type.
         */
        typedef std::uint64_t UI64;

        /**
         * Platform agnostic assert.
         *
         * \param[in] condition The condition to be tested.
         */
        inline void _assert(bool condition) {
            (void) condition;
            assert(condition);
        }

    #else

        /**
         * Platform agnostic guaranteed signed 8 bit integer type.
         */
        typedef __INT8_TYPE__   SI8;

        /**
         * Platform agnostic guaranteed unsigned 8 bit integer type.
         */
        typedef __UINT8_TYPE__  UI8;

        /**
         * Platform agnostic guaranteed signed 16 bit integer type.
         */
        typedef __INT16_TYPE__  SI16;

        /**
         * Platform agnostic guaranteed unsigned 16 bit integer type.
         */
        typedef __UINT16_TYPE__ UI16;

        /**
         * Platform agnostic guaranteed signed 32 bit integer type.
         */
        typedef __INT32_TYPE__  SI32;

        /**
         * Platform agnostic guaranteed unsigned 32 bit integer type.
         */
        typedef __UINT32_TYPE__ UI32;

        /**
         * Platform agnostic guaranteed signed 64 bit integer type.
         */
        typedef __INT64_TYPE__  SI64;

        /**
         * Platform agnostic guaranteed unsigned 64 bit integer type.
         */
        typedef __UINT64_TYPE__ UI64;

        /**
         * Platform agnostic assert.
         *
         * \param[in] condition The condition to be tested.
         */
        inline void _assert(bool condition) {
            (void) condition;
        }

    #endif
};

#if (defined(INEM_USE_OS_ESCAPE))

    #if (defined(_MSC_VER))

        /**
         * Define you can use to force a function to always be compiled inline.
         */
        #define FORCE_INLINE __forceinline

        /**
         * Define you can use to force a function never be compiled inline.
         */
        #define NEVER_INLINE __declspec(noinline)

        /**
         * Define you can use to indicate that a function will never return.
         */
        #define NO_RETURN    __declspec(noreturn)

    #elif (defined(__linux__))

        /**
         * Define you can use to force a function to always be compiled inline.
         */
        #define FORCE_INLINE __attribute__((always_inline, unused)) inline

        /**
         * Define you can use to force a function never be compiled inline.
         */
        #define NEVER_INLINE __attribute__((noinline))

        /**
         * Define you can use to indicate that a function will never return.
         */
        #define NO_RETURN    __attribute__((noreturn))

    #else

        /**
         * Define you can use to force a function to always be compiled inline.
         */
        #define FORCE_INLINE __attribute__((always_inline))

        /**
         * Define you can use to force a function never be compiled inline.
         */
        #define NEVER_INLINE __attribute__((noinline))

        /**
         * Define you can use to indicate that a function will never return.
         */
        #define NO_RETURN    __attribute__((noreturn))

    #endif

#else

    /**
     * Placement new operator.
     *
     * \param[in] size The size of the newly allocated construct.
     *
     * \param[in] p    The pointer to be allocated.
     *
     * \return Returns a pointer to the newly allocated type.
     */
    [[clang::warn_unused_result]] inline void* operator new(__SIZE_TYPE__ size, void* p) noexcept   {
        (void) size;
        return p;
    }

    /**
     * Array placement new operator.
     *
     * \param[in] size The size of the newly allocated construct.
     *
     * \param[in] p    The pointer to be allocated.
     *
     * \return Returns a pointer to the newly allocated type.
     */
    [[clang::warn_unused_result]] inline void* operator new[](__SIZE_TYPE__ size, void* p) noexcept {
        (void) size;
        return p;
    }

    /**
     * Define you can use to force a function to always be compiled inline.
     */
    #define FORCE_INLINE __attribute__((always_inline))

    /**
     * Define you can use to force a function never be compiled inline.
     */
    #define NEVER_INLINE __attribute__((noinline))

    /**
     * Define you can use to indicate that a function will never return.
     */
    #define NO_RETURN    __attribute__((noreturn))

#endif

#endif
