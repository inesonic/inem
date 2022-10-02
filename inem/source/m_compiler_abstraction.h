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
* This header defines defines and macros needed for compiler abstraction.  The contents of this file are *not* intended
* to be part of the internal or customer facing API.  Use \file m_intrinsics.h or \file model_api_types.h for API
* visible content.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_COMPILER_ABSTRACTION_H
#define M_COMPILER_ABSTRACTION_H

#include <cstdint>

#if (defined(INEM_RTTI_ENABLED))

    /**
     * Template casting function.  Maps to dynamic_cast if RTTI is enabled, otherwise maps to static_cast.
     */
    #define POLYMORPHIC_CAST dynamic_cast

#else

    /**
     * Template casting function.  Maps to dynamic_cast if RTTI is enabled, otherwise maps to static_cast.
     */
    #define POLYMORPHIC_CAST static_cast

#endif

#if (defined(_MSC_VER))

    /**
     * Macro you can use to force alignment of variables.
     *
     * \param[in] _x The alignment constraint, in bytes
     */
    #define FORCE_ALIGNMENT(_x) __declspec(align(_x))

    /**
     * Macro you can use to force alignment of variables.
     *
     * \param[in] _x The alignment constraint, in bytes
     */
    #define DISABLE_WARNING_UNUSED_PARAMETER __pragma(warning(disable:4100))

    /**
     * Macro you can use to force alignment of variables.
     *
     * \param[in] _x The alignment constraint, in bytes
     */
    #define ENABLE_WARNING_UNUSED_PARAMETER __pragma(warning(default:4100))

#elif (defined(__linux__))

    /**
     * Macro you can use to force alignment of variables.
     *
     * \param[in] _x The alignment constraint, in bytes
     */
    #define FORCE_ALIGNMENT(_x) __attribute__((aligned(_x)))

    /**
     * Macro you can use to force alignment of variables.
     *
     * \param[in] _x The alignment constraint, in bytes
     */
    #define DISABLE_WARNING_UNUSED_PARAMETER _Pragma("GCC diagnostic ignored \"-Wunused-parameter\"")

    /**
     * Macro you can use to force alignment of variables.
     *
     * \param[in] _x The alignment constraint, in bytes
     */
    #define ENABLE_WARNING_UNUSED_PARAMETER _Pragma("GCC diagnostic warning \"-Wunused-parameter\"")

#elif (defined(__APPLE__))

    /**
     * Macro you can use to force alignment of variables.
     *
     * \param[in] _x The alignment constraint, in bytes
     */
    #define FORCE_ALIGNMENT(_x) __attribute__((aligned(_x)))

    /**
     * Macro you can use to force alignment of variables.
     *
     * \param[in] _x The alignment constraint, in bytes
     */
    #define DISABLE_WARNING_UNUSED_PARAMETER _Pragma("clang diagnostic ignored \"-Wunused-parameter\"")

    /**
     * Macro you can use to force alignment of variables.
     *
     * \param[in] _x The alignment constraint, in bytes
     */
    #define ENABLE_WARNING_UNUSED_PARAMETER _Pragma("clang diagnostic warning \"-Wunused-parameter\"")

#else

    #error Unknown platform

#endif

#endif
