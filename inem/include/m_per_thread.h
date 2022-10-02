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
* This header defines the \ref M::PerThread class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_PER_THREAD_H
#define M_PER_THREAD_H

#include "m_common.h"
#include "m_api_types.h"
#include "m_intrinsic_types.h"
#include "model_api.h"
#include "m_console.h"
#include "model_rng.h"

namespace Model {
    class Status;
}

namespace M {
    /**
     * Class that maintains information that is needed independently by each thread.
     */
    class M_PUBLIC_API PerThread:public Model::Rng {
        public:
            /**
             * The temporary buffer size, in bytes.
             */
            static constexpr unsigned temporaryBufferSizeInBytes = 2048;

            /**
             * Constructor
             *
             * \param[in] threadId        The zero based thread ID of the thread.
             *
             * \param[in] rngType         The type of RNG to be used.
             *
             * \param[in] rngSeed         The seed used to initialize the per-thread RNG.
             *
             * \param[in] consoleCallback The callback to use for the console.  This parameter exists to address how
             *                            Windows manages or doesn't manage shared memory between threads.
             *
             * \param[in] modelStatus     The model status instance.
             */
            PerThread(
                unsigned              threadId,
                RngType               rngType,
                const RngSeed&        rngSeed,
                M::Console::Callback* consoleCallback,
                Model::Status*        modelStatus = nullptr
            );

            ~PerThread() override;

            /**
             * Method you can use to obtain the thread ID.
             *
             * \return Returns the zero based ID of this thread.
             */
            unsigned threadId() const;

            /**
             * Method you can use to configure the random number generation engine.
             *
             * \param[in] rngType The desired RNG type.
             *
             * \param[in] rngSeed The desired RNG initial seed.  Note that the RNG seed will be interpreted differently
             *                    depending on the selected RNG type.  The RNG seed will be ignored for the true random
             *                    number generator.
             */
            void configure(RngType rngType, const RngSeed& rngSeed) final;

            /**
             * Method you can use to set the RNG type.  Calling this method will select a random seed value, if needed.
             *
             * \param[in] rngType The desired RNG type.
             */
            void setRngType(RngType rngType) final;

            /**
             * Method you can use to set the RNG seed.
             *
             * \param[in] rngSeed The RNG seed to apply.
             */
            void setRngSeed(const RngSeed& rngSeed) final;

            /**
             * Method you can use to determine the currently selected RNG type.
             *
             * \return Returns the currently selected RNG type.
             */
            RngType rngType() const final;

            /**
             * Method you can use to determine the last used RNG seed.
             *
             * \return Returns a constant reference to the last seed applied to the selected RNG.
             */
            const RngSeed& rngSeed() const final;

            /**
             * Method you can use to obtain a 32-bit true random value using the operating system cryptograph TRNG.
             *
             * \return Returns a 32-bit true random value.
             */
            unsigned long trng() final;

            /**
             * Method you can use to obtain an unsigned 32-bit random value.
             *
             * \return Returns an unsigned 32-bit random value with uniform distribution.
             */
            unsigned long random32() final;

            /**
             * Method you can use to obtain an unsigned 64-bit random value.
             *
             * \return Returns an unsigned 32-bit random value with uniform distribution.
             */
            unsigned long long random64() final;

            /**
             * Method you can use to obtain an unsigned random integer value.
             *
             * \return Returns an unsigned random integer value covering the entire range of possible values.
             */
            Integer randomInteger() final;

            /**
             * Method you can use to obtain a random floating point value between 0 and 1, inclusive.
             *
             * \return Returns an floating point random value between 0 and 1, inclusive.
             */
            Real randomInclusive() final;

            /**
             * Method you can use to obtain a random floating point value between 0 and 1, excluding 1.0.
             *
             * \return Returns an floating point random value between 0 and 1, excluding 1.0.
             */
            Real randomInclusiveExclusive() final;

            /**
             * Method you can use to obtain a random floating point value between 0 and 1, excluding 1.0.
             *
             * \return Returns an floating point random value between 0 and 1, excluding 1.0.
             */
            Real randomExclusiveInclusive() final;

            /**
             * Method you can use to obtain a random floating point value between 0 and 1, excluding 0.0 and 1.0.
             *
             * \return Returns an floating point random value between 0 and 1, excluding 1.0.
             */
            Real randomExclusive() final;

            /**
             * Method you can use to obtain values from a normal distribution.
             *
             * \return Returns values in a normal distribution.
             */
            Real randomNormal() final;

            /**
             * Method you can use to obtain values from a normal distribution.
             *
             * \param[in] mean  The mean value for the distribution.
             *
             * \param[in] sigma The distribution sigma.
             *
             * \return Returns values in a normal distribution.
             */
            inline Real randomNormal(Real mean, Real sigma) {
                return randomNormal() * sigma + mean;
            }

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
            Real randomWeibull(Real scale, Real shape, Real delay) final;

            /**
             * Method you can use to obtain values from an exponential distribution.
             *
             * \param[in] rate The rate term.
             *
             * \return Returns values from an exponential distribution.
             */
            Real randomExponential(Real rate) final;

            /**
             * Method you can use to obtain values from a gamma distribution.
             *
             * \param[in] k The shape parameter.
             *
             * \param[in] s The scale parameter.
             *
             * \return Returns a value from a gamma distribution.
             */
            Real randomGamma(Real k, Real s) final;

            /**
             * Method you can use to obtain random values from a Rayleigh distribution.
             *
             * \param[in] scale The scale parameter for the distribution.
             *
             * \return Returns random deviates in a Rayleigh distribution.
             */
            Real randomRayleigh(Real scale) final;

            /**
             * Method you can use to obtain random values from a Chi-sqared distribution.
             *
             * \param[in] k The distribution degrees of freedom.
             *
             * \return Returns a random value from the requested Chi-squared distribution.
             */
            Real randomChiSquared(Integer k) final;

            /**
             * Method you can use to obtain random values from a Poisson distribution.
             *
             * \param[in] rate The distribution rate parameter.
             *
             * \return Returns a count of the number of events based on the rate.
             */
            Integer randomPoisson(Real rate) final;

            /**
             * Method you can use to obtain random values from a binomial distribution.
             *
             * \param[in] n The number of trials.
             *
             * \param[in] p The success probability for each trial.
             *
             * \return Returns the number of successes.
             */
            Integer randomBinomial(Integer n, Real p) final;

            /**
             * Method you can use to obtain random values from a log normal distribution with zero mean and unit sigma.
             *
             * \return Returns values from a log-normal distribution.
             */
            Real randomLogNormal() final;

            /**
             * Method you can use to obtain random values from a log normal distribution.
             *
             * \param[in] mean  The mean value.
             *
             * \param[in] sigma The standard deviation.
             *
             * \return Returns values from a log-normal distribution.
             */
            Real randomLogNormal(Real mean, Real sigma) final;

            /**
             * Method you can use to obtain random values in a geometric distribution.
             *
             * \param[in] p The success probability.
             *
             * \return Returns the number of trials.
             */
            Integer randomGeometric(Real p) final;

            /**
             * Method you can use to obtain random values in a Cauchy-Lorentz distribution
             *
             * \param[in] location The location parameter.
             *
             * \param[in] scale    The scale parameter.
             *
             * \return Returns a random deviate in the Cauchy-Lorentz distribution.
             */
            Real randomCauchyLorentz(Real location, Real scale) final;

            /**
             * Method you can use to obtain a temporary buffer for calculations.
             *
             * \return Returns a temporary buffer you can use in this thread for calculations.
             */
            void* temporaryBuffer();

            /**
             * Method you can use to set the last reported operation handle.  Function is made in-line for speed.
             *
             * \param[in] newOperationHandle The new operation handle.
             */
            inline void setOperationHandle(OperationHandle newOperationHandle) {
                currentOperationHandle = newOperationHandle;
            }

            /**
             * Method you can use to obtain the last reported operation handle.
             *
             * \return Returns the last reported operation handle.
             */
            OperationHandle operationHandle() const;

            /**
             * Method you can call to register thread local parameters such as the console callback.  This call
             * primarily exists to deal with differences in memory management between the compilers on different
             * platforms.
             */
            void threadLocalSetup();

            /**
             * Method you can use to rapidly populate an arbitrary length array with 64-bit random integer values.
             * The algorithm used depends on the underlying RNG.  Note that the RNG may throw away values to force
             * memory alignment or to further optimize the fill operation.
             *
             * \param[in] array        Pointer to the starting location to be populated.  To support SIMD operations,
             *                         the array is expected to be 128-bit aligned and padded up to contain full
             *                         128-bit values.
             *
             * \param[in] numberValues The length of the array to be populated, in 64-bit values.  The 64-bit entry
             *                         just past the last entry in the array will be guaranteed to be forced to 0 if
             *                         the number of values is odd.
             */
            void fillArray(void* array, unsigned long numberValues);

            /**
             * Method that is called by a model to report immediate output.  You can use this to stream output from
             * the model to a given device and channel.  This method is called from the same thread(s) that the model
             * is running in and will block the thread.
             *
             * Note that this method is called from each thread and *must* be thread safe.  The default implementation
             * simply returns.
             *
             * \param[in] device The device to receive the data.
             *
             * \param[in] value  The reported value.
             */
            virtual void sendToDevice(M::Device device, const Model::Variant& value);

        private:
            /**
             * Structure holding the Poisson distribution cachaed parameters.
             */
            struct PoissonTerms {
                /**
                 * Natural log of the rate term.
                 */
                Real logRate;

                /**
                 * The normal mean value.
                 */
                Real normalMean;

                /**
                 * Log sigma to apply to the normal distribution to generate the comparison value.
                 */
                Real normalSigma;

                /**
                 * Scale factor used to rapidly generate the normal distribution.
                 */
                Real normalScaleFactor;

                /**
                 * A small offset to be applied to the normal distribution.  Forced the distribution to always remain
                 * above the Poisson distribution during the rejection algorithm.
                 */
                Real normalOffset;
            };

            /**
             * Structure holding the cached binomial distribution cached parameters.  For details, see
             *
             * Wolfgang Hormann, "The Generation of Binomial Random Variates", Preprint 1, April 1992.
             */
            struct BinomialTerms {
                Integer m;
                Real    r;
                Real    nr;
                Real    npq;
                Real    twoNpq;
                Real    b;
                Real    a;
                Real    c;
                Real    alpha;
                Real    vr;
                Real    urvr;
                Real    twoUrVr;
                Integer nm;
                Real    h;
            };

            /**
             * Private RNG base class instance.
             */
            class RngBase;

            /**
             * Private MT19937 RNG instance.
             */
            class MT19937;

            /**
             * Private MT216091 RNG instance.
             */
            class MT216091;

            /**
             * Private base class for the XORSHIRO256+ and XORSHIRO256** RNGs.
             */
            class XorShiRo256Base;

            /**
             * Private XORSHIRO256+ RNG instance.
             */
            class XorShiRo256Plus;

            /**
             * Private XORSHIRO256** RNG instance.
             */
            class XorShiRo256Stars;

            /**
             * Private TRNG instance.
             */
            class TRNG;

            /**
             * Method you can call to create a new RNG instance.
             *
             * \param[in] rngType The desired type of RNG.
             *
             * \return Returns a pointer to a newly created RNG.
             */
            static RngBase* createRng(RngType rngType);

            /**
             * Method you can call to create a new RNG instance with a specified seed.
             *
             * \param[in] rngType The desired type of RNG.
             *
             * \param[in] rngSeed The desired RNG seed.
             *
             * \param[in] extra   An extra value applied in conjunction with the seed.  This value will generally be
             *                    the thread ID.
             *
             * \return Returns a pointer to a newly created RNG.
             */
            static RngBase* createRng(RngType rngType, const RngSeed& rngSeed, unsigned extra);

            /**
             * Helper method used to generate random deviates in a random distribution.  Assumes a scale term of 1.0.
             *
             * \param[in] shape The shape term.  Value must be greater than 1.0.
             *
             * \return Returns a random deviate in a gamma distribution.
             */
            Real randomGammaHelper(Real shape);

            /**
             * Helper method that performs the decomposition portion of the binomial random deviate generator.
             *
             * \param[out] k  The calculated random deviate.
             *
             * \param[in]  n  The number of trials.
             *
             * \param[in]  v  The first random deviate for the comparison function.
             *
             * \param[in]  bt Structure holding the current binomial terms.
             *
             * \return Returns true if the rejection algorithm succeeded, returns false if a retry must be performed.
             */
            bool randomBinomialDecomposition(Integer& k, Integer n, Real v, const BinomialTerms& bt);

            /**
             * Helper method that calculates random deviates in a binomial distribution over a limited range of
             * probabilities using Wolfgang Hormann's BTRD algorithm.
             *
             * \param[in] n  The number of trials.
             *
             * \param[in] p  The probability.  Value must be less than 0.5.
             *
             * \return Returns the calculated number of successes.
             */
            Integer randomBinomialByBtrd(Integer n, Real p);

            /**
             * Helper method that calculates random deviates in a binomial distribution using an inverted binomial
             * distribution as a basis.
             *
             * \param[in] n The number of trials.
             *
             * \param[in] p The probability.  Value must be less than 0.5.
             */
            Integer randomBinomialByInvertedDistribution(Integer n, Real p);

            /**
             * The current thread ID.
             */
            unsigned currentThreadId;

            /**
             * The current operation handle.
             */
            OperationHandle currentOperationHandle;

            /**
             * Private RNG base class.
             */
            RngBase* rng;

            /**
             * The console callback.
             */
            M::Console::Callback* currentConsoleCallback;

            /**
             * The model status instance.
             */
            Model::Status* currentStatusInstance;

            /**
             * Flag used during generation of gaussian random deviates to reduce the number of extracted random values.
             */
            bool gaussianHasSpareValue;

            /**
             * The last shape term used for Gamma random deviates.  We store the values to reduce some of the
             * computation between passes.
             */
            Real gammaLastShape;

            /**
             * The last "c" term.
             */
            Real gammaLastC;

            /**
             * The spare value for the gaussian value.
             */
            Real gaussianSpare;

            /**
             * The last rate provided to the Poisson distribution.  Used to cache calculated parameters.
             */
            Real poissonLastRate;

            /**
             * Structure holding the Poisson distribution cachaed parameters.
             */
            PoissonTerms poissonTerms;

            /**
             * The last number tries values provided to the binomial distribution.
             */
            Integer binomialLastN;

            /**
             * The last binomial distribution probability value.
             */
            Real binomialLastP;

            /**
             * Structure holding the Binomial distribution cached parameters.
             */
            BinomialTerms binomialTerms;

            /**
             * The currently allocated temporary buffer.  A null pointer indicates that no buffer has been allocated
             * yet.
             */
            void* currentTemporaryBuffer;
    };
};

#endif
