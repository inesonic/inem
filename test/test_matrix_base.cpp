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
* This file implements a common base class for matrix test harnesses.
***********************************************************************************************************************/

#include <QObject>
#include <QList>

#include <limits>
#include <complex>
#include <cmath>
#include <random>
#include <algorithm>

#include "test_matrix_base.h"

QList<unsigned long long> TestMatrixBase::currentPrimes;
unsigned long long        TestMatrixBase::primeIndex;

TestMatrixBase::TestMatrixBase() {}


TestMatrixBase::~TestMatrixBase() {}


const QList<unsigned long long>& TestMatrixBase::calculatePrimes(unsigned long long numberPrimes) {
    unsigned long long currentNumberPrimes = static_cast<unsigned long long>(currentPrimes.size());
    if (numberPrimes > currentNumberPrimes) {
        if (currentNumberPrimes < 2) {
            currentPrimes.append(2);
            currentPrimes.append(3);

            primeIndex = 6;
        }

        while (currentNumberPrimes < numberPrimes) {
            if (isPrime(primeIndex - 1, currentPrimes)) {
                currentPrimes.append(primeIndex - 1);
                ++currentNumberPrimes;
            }

            if (isPrime(primeIndex + 1, currentPrimes)) {
                currentPrimes.append(primeIndex + 1);
                ++currentNumberPrimes;
            }

            primeIndex += 6;
        }
    }

    return currentPrimes;
}


bool TestMatrixBase::isPrime(unsigned long long p, const QList<unsigned long long>& knownPrimes) {
    unsigned long long maximumFactor = static_cast<unsigned long long>(std::sqrt(p) + 1);

    QList<unsigned long long>::const_iterator it  = knownPrimes.constBegin();
    QList<unsigned long long>::const_iterator end = knownPrimes.constEnd();

    unsigned long long remainder = 1;
    while (remainder != 0 && it != end && *it <= maximumFactor) {
        unsigned long long factor   = *it;
        unsigned long long quotient = p / factor;
        remainder = p - factor * quotient;
        ++it;
    }

    return remainder != 0;
}
