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
* This header defines the \ref Model::Rng class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_RNG_H
#define MODEL_RNG_H

#include "model_common.h"
#include "model_intrinsic_types.h"

namespace Model {
    /**
     * Base class you can use to access an random number generator (RNG) used by the model
     */
    class MODEL_PUBLIC_API Rng {
        public:
            /**
             * Enumeration of supported RNG types.
             */
            enum class RngType {
                /**
                 * Indicates the model is to use the MT19937 Mercenne Twister algorithm.
                 */
                MT19937,

                /**
                 * Indicates the model is to use the MT216091 SIMD Mercenne Twister algorithm.
                 */
                MT216091,

                /**
                 * Indicates the model is to use the XOR/shift/rotate+ 256 algorithm.
                 */
                XORSHIRO256_PLUS,

                /**
                 * Indicates the model is to use the XOR/shift/rotate** 256 algorithm.
                 */
                XORSHIRO256_STARS,

                /**
                 * Indicates the model is to use the operating system true random number generator (TRNG).
                 */
                TRNG,

                /**
                 * Value used to determine the number of supported random number generators.
                 */
                NUMBER_RNGS
            };

            /**
             * Value indicating the RNG seed length.
             */
            static constexpr unsigned rngSeedLength = 4;

            /**
             * Type used to represent seeds passed to the RNG.
             */
            typedef unsigned long long RngSeed[rngSeedLength];

            virtual ~Rng() = default;

            /**
             * Method you can use to configure the random number generation engine.
             *
             * \param[in] rngType The desired RNG type.
             *
             * \param[in] rngSeed The desired RNG initial seed.  Note that the RNG seed will be interpreted differently
             *                    depending on the selected RNG type.  The RNG seed will be ignored for the true random
             *                    number generator.
             */
            virtual void configure(RngType rngType, const RngSeed& rngSeed) = 0;

            /**
             * Method you can use to set the RNG type.  Calling this method will select a random seed value, if needed.
             *
             * \param[in] rngType The desired RNG type.
             */
            virtual void setRngType(RngType rngType) = 0;

            /**
             * Method you can use to set the RNG seed.
             *
             * \param[in] rngSeed The RNG seed to apply.
             */
            virtual void setRngSeed(const RngSeed& rngSeed) = 0;

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
             * Method you can use to obtain a 32-bit true random value using the operating system cryptograph TRNG.
             *
             * \return Returns a 32-bit true random value.
             */
            virtual unsigned long trng() = 0;

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
             * Method you can use to obtain an unsigned random integer value.
             *
             * \return Returns an unsigned random integer value covering the entire range of possible values.
             */
            virtual Integer randomInteger() = 0;

            /**
             * Method you can use to obtain a random floating point value between 0 and 1, inclusive.
             *
             * \return Returns an floating point random value between 0 and 1, inclusive.
             */
            virtual Real randomInclusive() = 0;

            /**
             * Method you can use to obtain a random floating point value between 0 and 1, excluding 1.0.
             *
             * \return Returns an floating point random value between 0 and 1, excluding 1.0.
             */
            virtual Real randomInclusiveExclusive() = 0;

            /**
             * Method you can use to obtain a random floating point value between 0 and 1, excluding 1.0.
             *
             * \return Returns an floating point random value between 0 and 1, excluding 1.0.
             */
            virtual Real randomExclusiveInclusive() = 0;

            /**
             * Method you can use to obtain a random floating point value between 0 and 1, excluding 0.0 and 1.0.
             *
             * \return Returns an floating point random value between 0 and 1, excluding 1.0.
             */
            virtual Real randomExclusive() = 0;

            /**
             * Method you can use to obtain values from a normal distribution.
             *
             * \return Returns values in a normal distribution.
             */
            virtual Real randomNormal() = 0;

            /**
             * Method you can use to obtain values from a Weibull distribution.
             *
             * \param[in] scale The scale term.
             *
             * \param[in] shape The shape term.
             *
             * \param[in] delay The delay term.
             *
             * \return Returns a random deviate from a Weibull distribution.
             */
            virtual Real randomWeibull(Real scale, Real shape, Real delay) = 0;

            /**
             * Method you can use to obtain values from an exponential distribution.
             *
             * \param[in] rate The rate term.
             *
             * \return Returns values from an exponential distribution.
             */
            virtual Real randomExponential(Real rate) = 0;

            /**
             * Method you can use to obtain values from a gamma distribution.
             *
             * \param[in] k The shape parameter.
             *
             * \param[in] s The scale parameter.
             *
             * \return Returns a value from a gamma distribution.
             */
            virtual Real randomGamma(Real k, Real s) = 0;

            /**
             * Method you can use to obtain random values from a Rayleigh distribution.
             *
             * \param[in] scale The scale parameter for the distribution.
             *
             * \return Returns random deviates in a Rayleigh distribution.
             */
            virtual Real randomRayleigh(Real scale) = 0;

            /**
             * Method you can use to obtain random values from a Chi-sqared distribution.
             *
             * \param[in] k The distribution degrees of freedom.
             *
             * \return Returns a random value from the requested Chi-squared distribution.
             */
            virtual Real randomChiSquared(Integer k) = 0;

            /**
             * Method you can use to obtain random values from a Poisson distribution.
             *
             * \param[in] rate The distribution rate parameter.
             *
             * \return Returns a count of the number of events based on the rate.
             */
            virtual Integer randomPoisson(Real rate) = 0;

            /**
             * Method you can use to obtain random values from a binomial distribution.
             *
             * \param[in] n The number of trials.
             *
             * \param[in] p The success probability for each trial.
             *
             * \return Returns the number of successes.
             */
            virtual Integer randomBinomial(Integer n, Real p) = 0;

            /**
             * Method you can use to obtain random values from a log normal distribution with zero mean and unit sigma.
             *
             * \return Returns values from a log-normal distribution.
             */
            virtual Real randomLogNormal() = 0;

            /**
             * Method you can use to obtain random values from a log normal distribution.
             *
             * \param[in] mean  The mean value.
             *
             * \param[in] sigma The standard deviation.
             *
             * \return Returns values from a log-normal distribution.
             */
            virtual Real randomLogNormal(Real mean, Real sigma) = 0;

            /**
             * Method you can use to obtain random values in a geometric distribution.
             *
             * \param[in] p The success probability.
             *
             * \return Returns the number of trials.
             */
            virtual Integer randomGeometric(Real p) = 0;

            /**
             * Method you can use to obtain random values in a Cauchy-Lorentz distribution
             *
             * \param[in] location The location parameter.
             *
             * \param[in] scale    The scale parameter.
             *
             * \return Returns a random deviate in the Cauchy-Lorentz distribution.
             */
            virtual Real randomCauchyLorentz(Real location, Real scale) = 0;
    };

}

#endif
