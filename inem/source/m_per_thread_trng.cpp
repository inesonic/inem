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
* This file implements the \ref M::PerThread::TRNG class.
***********************************************************************************************************************/

#include "model_exceptions.h"
#include "m_console.h"
#include "m_intrinsics.h"
#include "m_per_thread_rng_base.h"
#include "m_per_thread_trng.h"

namespace M {
    const PerThread::TRNG::RngSeed PerThread::TRNG::defaultRngSeed = { 0, 0, 0, 0};

    PerThread::TRNG::TRNG() {}


    PerThread::TRNG::~TRNG() {}


    void PerThread::TRNG::setRngSeed(const PerThread::RngSeed& /* rngSeed */, unsigned /* extra */) {}


    const PerThread::RngSeed& PerThread::TRNG::rngSeed() const {
        return defaultRngSeed;
    }


    PerThread::RngType PerThread::TRNG::rngType() const {
        return RngType::TRNG;
    }


    unsigned long PerThread::TRNG::random32() {
        return trng();
    }


    unsigned long long PerThread::TRNG::random64() {
        return static_cast<unsigned long long>(trng()) << 32 | trng();
    }


    void PerThread::TRNG::fillArray(void* array, unsigned long numberValues) {
        std::uint32_t* p = reinterpret_cast<std::uint32_t*>(array);
        trng(p, numberValues);
    }
}
