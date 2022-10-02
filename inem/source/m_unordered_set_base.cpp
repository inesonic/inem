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
* This file implements the \ref M::UnorderedSetBase class.
***********************************************************************************************************************/

#include <cmath>

#include "m_list.h"
#include "m_unordered_set_base.h"

namespace M {
    static inline bool divides(UnorderedSetBase::Index dividend, UnorderedSetBase::Index divisor) {
        return (dividend % divisor) == 0;
    }


    static bool isPrime(UnorderedSetBase::Index value) {
        bool prime;

        if (value == 2 || value == 3) {
            prime = true;
        } else if (value < 5) {
            prime = false;
        } else {
            UnorderedSetBase::Index endingIndex = static_cast<UnorderedSetBase::Index>(sqrt(value)) + 1;
            UnorderedSetBase::Index i           = 6;

            prime = true;
            while (prime && i <= endingIndex) {
                prime = !divides(value, i - 1) && !divides(value, i + 1);
                i += 6;
            }
        }

        return prime;
    }


    const UnorderedSetBase::Index UnorderedSetBase::invalidIndex = static_cast<UnorderedSetBase::Index>(-1);

    UnorderedSetBase::UnorderedSetBase() {}


    UnorderedSetBase::~UnorderedSetBase() {}


    UnorderedSetBase::Index UnorderedSetBase::calculateNextPrime(UnorderedSetBase::Index value) {
        Index nextPrime;
        if (value < 5) {
            if (value < 2) {
                nextPrime = 2;
            } else if (value < 3) {
                nextPrime = 3;
            } else {
                nextPrime = 5;
            }
        } else {
            nextPrime = 0;

            Index i  = 6UL * ((value + 2UL) / 6UL);
            Index v1 = i - 1;
            if (v1 > value && isPrime(v1)) {
                nextPrime = v1;
            } else {
                Index v2 = i + 1;
                if (v2 > value && isPrime(v2)) {
                    nextPrime = v2;
                } else {
                    do {
                        i += 6;
                        v1 = i - 1;
                        if (isPrime(v1)) {
                            nextPrime = v1;
                        } else {
                            v2 = i + 1;
                            if (isPrime(v2)) {
                                nextPrime = v2;
                            }
                        }
                    } while (nextPrime == 0);
                }
            }
        }

        return nextPrime;
    }


    UI64 UnorderedSetBase::fnv1a8(UI8 value, UI64 hash) {
        return (hash ^ value) * fnv1Prime;
    }


    UI64 UnorderedSetBase::fnv1a16(UI16 value, UI64 hash) {
        unsigned char* bytes = reinterpret_cast<unsigned char*>(&value);

        hash = (hash ^ bytes[0]) * fnv1Prime;
        hash = (hash ^ bytes[1]) * fnv1Prime;

        return hash;
    }


    UI64 UnorderedSetBase::fnv1a32(UI32 value, UI64 hash) {
        unsigned char* bytes = reinterpret_cast<unsigned char*>(&value);

        hash = (hash ^ bytes[0]) * fnv1Prime;
        hash = (hash ^ bytes[1]) * fnv1Prime;
        hash = (hash ^ bytes[2]) * fnv1Prime;
        hash = (hash ^ bytes[3]) * fnv1Prime;

        return hash;
    }


    UI64 UnorderedSetBase::fnv1a64(UI64 value, UI64 hash) {
        unsigned char* bytes = reinterpret_cast<unsigned char*>(&value);

        hash = (hash ^ bytes[0]) * fnv1Prime;
        hash = (hash ^ bytes[1]) * fnv1Prime;
        hash = (hash ^ bytes[2]) * fnv1Prime;
        hash = (hash ^ bytes[3]) * fnv1Prime;
        hash = (hash ^ bytes[4]) * fnv1Prime;
        hash = (hash ^ bytes[5]) * fnv1Prime;
        hash = (hash ^ bytes[6]) * fnv1Prime;
        hash = (hash ^ bytes[7]) * fnv1Prime;

        return hash;
    }


    UnorderedSetBase::Index UnorderedSetBase::calculateRecommendedTableSize(UnorderedSetBase::Index reservation) {
        return calculateNextPrime(static_cast<UnorderedSetBase::Index>(reservationMargin * reservation));
    }
}
