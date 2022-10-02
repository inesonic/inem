/*-*-c++-*-************************************************************************************************************
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
* This file rngements the \ref M::PerThread class.
***********************************************************************************************************************/

#include <cassert>
#include <cmath>
#include <limits>
#include <complex>

#include "m_console.h"
#include "m_api_types.h"
#include "model_exceptions.h"
#include "model_status.h"
#include "m_basic_functions.h"
#include "m_per_thread_rng_base.h"
#include "m_per_thread_mt19937.h"
#include "m_per_thread_mt216091.h"
#include "m_per_thread_xorshiro256_plus.h"
#include "m_per_thread_xorshiro256_stars.h"
#include "m_per_thread_trng.h"
#include "m_per_thread.h"

namespace M {
    static const Real oneThird = 1.0 / 3.0;
    static const Real sqrt2Pi  = std::sqrt(2.0 * 4.0 * std::atan(1.0));


    static const Real rescaledBinomialHistogramTerms[10] = {
        0.08106146679532726,
        0.04134069595540929,
        0.02767792568499834,
        0.02079067210376509,
        0.01664469118982119,
        0.01387612882307075,
        0.01189670994589177,
        0.01041126526197209,
        0.009255462182712733,
        0.008330563433362871
    };

    static constexpr Real rbht1 = 1.0/12.0;
    static constexpr Real rbht2 = 1.0/360.0;
    static constexpr Real rbht3 = 1.0/1260.0;

    static Real rescaledBinomialHistogram(Integer k) {
        Real result;

        // For k <= 9 we use precomputed values for the Sterling approximation of the rescaled binomial.  For larger
        // values, we use the first four terms.  Note that, following Hormann's paper, we've turned the approximation
        // on it's head.  We further refine things by turning the divisions into multiplications as they're faster.

        if (k <= 9) {
            result = rescaledBinomialHistogramTerms[k];
        } else {
            Real rkp1 = 1.0 / (k + 1.0);
            Real rkp1s = rkp1 * rkp1;
            result    = (rbht1 - (rbht2 - rbht3 * rkp1s) * rkp1s) * rkp1;
        }

        return result;
    }

    PerThread::PerThread(
            unsigned                  threadId,
            PerThread::RngType        rngType,
            const PerThread::RngSeed& rngSeed,
            M::Console::Callback*     consoleCallback,
            Model::Status*            modelStatus
        ) {
        currentThreadId        = threadId;
        currentOperationHandle = Model::invalidOperationHandle;
        currentConsoleCallback = consoleCallback;
        currentStatusInstance  = modelStatus;

        gaussianHasSpareValue  = false;
        gammaLastShape         = std::numeric_limits<Real>::lowest();
        poissonLastRate        = std::numeric_limits<Real>::lowest();
        binomialLastN          = -1;
        binomialLastP          = -1.0;

        currentTemporaryBuffer = nullptr;

        rng = createRng(rngType, rngSeed, threadId);
    }


    PerThread::~PerThread() {
        if (rng != nullptr) {
            delete rng;
        }

        if (currentTemporaryBuffer != nullptr) {
            delete reinterpret_cast<std::uint8_t*>(currentTemporaryBuffer);
        }
    }


    void PerThread::configure(RngType rngType, const RngSeed& rngSeed) {
        if (rng != nullptr) {
            delete rng;
        }

        rng = createRng(rngType, rngSeed, currentThreadId);
    }


    void PerThread::setRngType(RngType rngType) {
        if (rng != nullptr) {
            delete rng;
        }

        rng = createRng(rngType);
    }


    void PerThread::setRngSeed(const RngSeed& rngSeed) {
        rng->setRngSeed(rngSeed, currentThreadId);
    }


    PerThread::RngType PerThread::rngType() const {
        return rng->rngType();
    }


    const PerThread::RngSeed& PerThread::rngSeed() const {
        return rng->rngSeed();
    }


    unsigned long PerThread::trng() {
        return rng->trng();
    }


    unsigned long PerThread::random32() {
        return rng->random32();
    }


    unsigned long long PerThread::random64() {
        return rng->random64();
    }


    Integer PerThread::randomInteger() {
        return static_cast<Integer>(rng->random64());
    }


    Real PerThread::randomInclusive() {
        UI64 v = rng->random64();
        return static_cast<long double>(1.0 * v) / static_cast<long double>(1.0 * static_cast<UI64>(-1));
    }


    Real PerThread::randomInclusiveExclusive() {
        Real v;

        do {
            v = (
                  static_cast<long double>(1.0 * rng->random64())
                / static_cast<long double>(1.0 * static_cast<UI64>(-1))
            );
        } while (v == 1.0);

        return v;
    }


    Real PerThread::randomExclusiveInclusive() {
        Real v;

        do {
            v = (
                  static_cast<long double>(1.0 * rng->random64())
                / static_cast<long double>(1.0 * static_cast<UI64>(-1))
            );
        } while (v == 0.0);

        return v;
    }


    Real PerThread::randomExclusive() {
        Real v;

        do {
            v = (
                  static_cast<long double>(1.0 * rng->random64())
                / static_cast<long double>(1.0 * static_cast<UI64>(-1))
            );
        } while (v == 1.0 || v == 1.0);

        return v;
    }


    Real PerThread::randomNormal() {
        // This method uses the Marsaglia Polar Method to calculate two terms simultaneously.  Terms are used in
        // alternating fashion.
        //
        // Longer term, we should consider Ziggurat's algorithm.

        Real result;
        if (gaussianHasSpareValue) {
            gaussianHasSpareValue = false;
            result = gaussianSpare;
        } else {
            Real w;
            Real x1;
            Real x2;

            do {
                x1 = 2.0 * randomInclusive() - 1.0;
                x2 = 2.0 * randomInclusive() - 1.0;

                w = x1 * x1 + x2 * x2;
            } while (w == 0.0 || w >= 1.0);

            w = std::sqrt(-2.0 * std::log(w) / w);

            gaussianSpare = x2 * w;
            result        = x1 * w;
        }

        return result;
    }


    Real PerThread::randomGamma(Real shape, Real scale) {
        if (shape <= 0 || scale <= 0) {
            throw Model::InvalidNumericValue();
        }

        return   shape >= 1.0
               ? scale * randomGammaHelper(shape)
               : scale * randomGammaHelper(shape + 1.0) * std::pow(randomExclusive(), 1.0 / shape);
    }


    Real PerThread::randomWeibull(Real scale, Real shape, Real delay) {
        if (scale <= 0 || shape <= 0) {
            throw Model::InvalidNumericValue();
        }

        return scale * std::pow(-std::log(randomExclusiveInclusive()), 1 / shape) + delay;
    }


    Real PerThread::randomExponential(Real rate) {
        if (rate <= 0) {
            throw Model::InvalidNumericValue();
        }

        return -std::log(randomExclusive()) / rate;
    }


    Real PerThread::randomRayleigh(Real scale) {
        // Calculated using transformation method.  See
        //
        //   http://en.wikipedia.org/wiki/Rayleigh_distribution

        if (scale <= 0) {
            throw Model::InvalidNumericValue();
        }

        return scale * std::sqrt(-2.0 * std::log(randomExclusive()));
    }


    Real PerThread::randomChiSquared(Integer k) {
        // We rely on the fact that the chi-squared distribution is a special case of the gamma distribution with
        // shape = k / 2 and scale = 2.0

        if (k <= 0) {
            throw Model::InvalidNumericValue();
        }

        return randomGamma(k / 2.0, 2.0);
    }


    Integer PerThread::randomPoisson(Real rate) {
        Integer result;

        if (rate <= 0) {
            throw Model::InvalidNumericValue();
        }

        // For small values we use Knuth's method.

        if (rate <= 12) {
            Real    l = std::exp(-rate);
            Integer k = -1;
            Real    p = 1.0;

            do {
                ++k;
                p *= randomInclusive();
            } while (p > l);

            result = k;
        } else {
            // For large values we use rejection method.
            //
            // I note that Numerical Methods in C++ uses a fitting function based on the Cauchy-Lorentz distribution.
            // The advantage of using this fitting function is that we can calculate the quantile by transformation
            // method and the fit roughly matches the Poisson distribution.
            //
            // I note that the normal distribution provides a much closer fit so the likelihood of rejection is much
            // less.  This is, of course, traded off against the fact that we must use rejection method to calculate
            // values in the normal distribution as well as the fact that at least some of the exponential terms will
            // fall out when evaluating over the log of the Poisson PDF (log is needed due to the factorial).
            //
            // The probability of rejection in Marsaglia's algorithm, used above is around 21% which suggests an
            // inferior fit.  However, acceptance provides two values so our effective rejection rate is, in fact,
            // closer to 10% and the fit is, in fact, much closer than the Lorentz-Cauchy distribution used by
            // Numerical Methods so the overall rejection rate should be lower.
            //
            // The normal distribution PDF will sometimes cross a little under the Poisson distribution PMF so we
            // address this by adding a small fixed offset the normal PDF.  The offset is estimated at mean - 0.5 sigma
            // as that appears to be a good fit based on experimentation.  We re-adjusted the offset if we find a case
            // where the offset is insufficient.
            //
            // For this reason, our algorithm uses a normal distribution.
            //
            // We use the following factors for our normal distribution:
            //
            //     mean   = rate (the 0.5 provides a continuity correction for the PMF)
            //     sigma  = sqrt(rate)
            //     offset = 1.01 (normal_pdf(mean - 0.5 sigma, sigma) - poisson_pmf(mean - 0.5 sigma))
            //
            // We accelerate our computations by caching values whenever possible.

            PoissonTerms pt;
            if (poissonLastRate != rate) {
                poissonLastRate = rate;

                pt.normalSigma       = std::sqrt(rate);
                pt.normalMean        = rate;
                pt.logRate           = std::log(rate);
                pt.normalScaleFactor = 1.0 / (pt.normalSigma * sqrt2Pi);

                Real offsetRate = pt.normalMean - 0.5 * pt.normalSigma;
                Real poissonPmf = std::exp(offsetRate * pt.logRate - rate - lnGamma(offsetRate + 1.0));
                Real normalExp  = (offsetRate - pt.normalMean) / pt.normalSigma;
                Real normalPdf  = pt.normalScaleFactor * std::exp(-0.5 * normalExp * normalExp);

                // We add a tiny scale factor to our offset to make certain we have no issues due to rounding.
                pt.normalOffset = 1.02 * (poissonPmf - normalPdf);

                poissonTerms = pt;
            } else {
                pt = poissonTerms; // Local cache
            }

            Real normalPdf;
            Real poissonPmf;
            Real k;
            do {
                Real guess;
                do {
                    guess = randomNormal(pt.normalMean, pt.normalSigma);
                } while (guess < 0);
                k = std::floor(guess + 0.5); // +0.5 is a continuity correction

                Real normalExp = (k - pt.normalMean) / pt.normalSigma;

                normalPdf  = pt.normalScaleFactor * std::exp(-0.5 * normalExp * normalExp) + pt.normalOffset;
                poissonPmf = std::exp(k * pt.logRate - rate - lnFactorial(k));

                if (normalPdf < poissonPmf) {
                    pt.normalOffset           += 1.02 * (poissonPmf - normalPdf);
                    poissonTerms.normalOffset  = pt.normalOffset;

                    poissonPmf = 1.0; // Force retry
                }
            } while (poissonPmf < normalPdf * randomInclusive());

            result = static_cast<Integer>(k);
        }

        return result;
    }


    Integer PerThread::randomBinomial(Integer n, Real p) {
        // For small values of n less than 15, or in cases where the BTRD algorithm won't reliabily converge, this
        // method uses a brute force approach based simply on the definition of the binomial distribution.  While
        // this may seem counter intuitive, the brute force algorithm, is, on average, very fast.
        //
        // For larger values of n, this method uses th BTRD algorithm by Wolfgang Hormann published in "The Generation
        // of Binomial Random Variates", Preprint 1, April 1992.

        if (n <= 0 || p < 0 || p > 1.0) {
            throw Model::InvalidNumericValue();
        }

        Integer k;

        if (n < 15) {
            k = 0;

            std::uint32_t threshold = static_cast<std::uint32_t>(0.5 + p * static_cast<std::uint32_t>(-1));
            for (Integer i=0 ; i<n ; ++i) {
                k += static_cast<int>(rng->random32() < threshold);
            }
        } else if (n * p < 10) {
            if (p > 0.5) {
                k = n - randomBinomialByInvertedDistribution(n , 1.0 - p);
            } else {
                k = randomBinomialByInvertedDistribution(n, p);
            }
        } else {
            if (p > 0.5) {
                k = n - randomBinomialByBtrd(n, 1.0 - p);
            } else {
                k = randomBinomialByBtrd(n, p);
            }
        }

        return k;
    }


    Real PerThread::randomLogNormal() {
        return std::exp(randomNormal());
    }


    Real PerThread::randomLogNormal(Real mean, Real sigma) {
        return std::exp(randomNormal(mean, sigma));
    }


    Integer PerThread::randomGeometric(Real p) {
        // Note that this method models the CDF: 1 - (1-p)^k.  (number of trials, not number of failures).

        if (p < 0.0 || p > 1.0) {
            throw Model::InvalidNumericValue();
        }

        Real u = randomExclusiveInclusive();
        return static_cast<Integer>(std::log(u) / std::log(1.0 - p)) + 1;
    }


    Real PerThread::randomCauchyLorentz(Real location, Real scale) {

        if (scale <= 0) {
            throw Model::InvalidNumericValue();
        }

        Real u = randomExclusive();
        return location + scale * std::tan(pi * (u - 0.5));
    }


    unsigned PerThread::threadId() const {
        return currentThreadId;
    }


    void* PerThread::temporaryBuffer() {
        if (currentTemporaryBuffer == nullptr) {
            currentTemporaryBuffer = new std::uint8_t[temporaryBufferSizeInBytes];
        }

        return currentTemporaryBuffer;
    }


    Model::OperationHandle PerThread::operationHandle() const {
        return currentOperationHandle;
    }


    void PerThread::threadLocalSetup() {
        #if (defined(_MSC_VER))

            M::Console::registerCallback(currentConsoleCallback);

        #endif
    }


    void PerThread::fillArray(void* array, unsigned long numberValues) {
        rng->fillArray(array, numberValues);
    }


    void PerThread::sendToDevice(M::Device device, const Model::Variant& value) {
        if (currentStatusInstance != nullptr) {
            currentStatusInstance->sendToDevice(device, value);
        }
    }


    PerThread::RngBase* PerThread::createRng(RngType rngType) {
        RngBase* rng;

        switch (rngType) {
            case RngType::MT19937: {
                rng = new MT19937;
                break;
            }

            case RngType::MT216091: {
                rng = new MT216091;
                break;
            }

            case RngType::XORSHIRO256_PLUS: {
                rng = new XorShiRo256Plus;
                break;
            }

            case RngType::XORSHIRO256_STARS: {
                rng = new XorShiRo256Stars;
                break;
            }

            case RngType::TRNG: {
                rng = new TRNG;
                break;
            }

            default: {
                rng = nullptr;
                assert(false);
            }
        }

        return rng;
    }


    PerThread::RngBase* PerThread::createRng(RngType rngType, const RngSeed& rngSeed, unsigned extra) {
        RngBase* rng;

        switch (rngType) {
            case RngType::MT19937: {
                rng = new MT19937(rngSeed, extra);
                break;
            }

            case RngType::MT216091: {
                rng = new MT216091(rngSeed, extra);
                break;
            }

            case RngType::XORSHIRO256_PLUS: {
                rng = new XorShiRo256Plus(rngSeed, extra);
                break;
            }

            case RngType::XORSHIRO256_STARS: {
                rng = new XorShiRo256Stars(rngSeed, extra);
                break;
            }

            case RngType::TRNG: {
                rng = new TRNG;
                break;
            }

            default: {
                rng = nullptr;
                assert(false);
            }
        }

        return rng;
    }


    Real PerThread::randomGammaHelper(Real shape) {
        // Implementation uses Marsaglia and Tsang's algorithm for generating random deviates.
        //
        // G. Marsaglia and W. Tsang, "A simple method for generating gamma variables".
        // ACM Transactions on Mathematical Software, 26(3):363-372, 2000
        //
        // See: http://www.hongliangjie.com/2012/12/19/how-to-generate-gamma-random-variables/

        Real d = shape - oneThird;

        Real c;
        if (shape != gammaLastShape) {
            c = oneThird / std::sqrt(d);
            gammaLastC = c;
        } else {
            c = gammaLastC;
        }

        Real mrc = -1.0 / c;
        Real lu  = std::log(randomExclusive());

        Real z;
        Real v;
        do {
            z = randomNormal();
            Real onecz = 1.0 + c * z;
            v = onecz * onecz * onecz;
        } while (z <= mrc || lu >= (0.5 * z * z + d - d * v + d * std::log(v)));

        return d * v;    }


    bool PerThread::randomBinomialDecomposition(Integer& k, Integer n, Real v, const BinomialTerms& bt) {
        bool success = false;

        /* Step 2 */
        Real u;
        if (v >= bt.vr) {
            u = randomExclusive() - 0.5;
        } else {
            u = v / bt.vr - 0.93;
            u = (u >= 0.0 ? 0.5 : -0.5) - u;
            v = randomExclusive() * bt.vr;
        }

        /* Step 3.0 */
        Real us = 0.5 - std::abs(u);
        k = static_cast<Integer>(bt.c + u * (bt.b + 2.0 * bt.a / us));

        if (k >= 0 && k <= n) {
            v = v * bt.alpha / (bt.b + bt.a / (us * us));

            Integer km = std::abs(k - bt.m);
            if (km <= 15) {
                /* Step 3.1 */

                Real f = 1.0;
                if (bt.m < k) {
                    for (Integer i=bt.m ; i<k ; ++i) { // We can optimize this using std::pow
                        f *= (bt.nr / i) - bt.r;
                    }
                } else if (bt.m > k) {
                    for (Integer i=k ; i<bt.m ; ++i) { // We can optimize this using std::pow
                        v *= (bt.nr / i) - bt.r;
                    }
                }

                if (v <= f) {
                    success = true;
                }
            } else {
                /* Step 3.2 */

                v = std::log(v);
                Real rho = (static_cast<Real>(km) / bt.npq) * ((1.0/6.0 + km * (0.625 + km / 3.0))/bt.npq + 0.5);
                Real t   = -static_cast<Real>(km * km) / bt.twoNpq;

                if (v < (t - rho)) {
                    success = true;
                } else if (v <= (t + rho)) {
                    /* Step 3.3 - Values are pre-calculated to improve performance */
                    /* Step 3.4 */

                    Integer nk = n - k + 1;
                    Real    sq = (
                          bt.h
                        + (n + 1) * std::log(static_cast<Real>(bt.nm) / static_cast<Real>(nk))
                        + (k + 0.5) * std::log((nk * bt.r) / (k + 1.0))
                        - rescaledBinomialHistogram(k)
                        - rescaledBinomialHistogram(n - k)
                    );

                    if (v <= sq) {
                        success = true;
                    }
                }
            }
        }

        return success;
    }


    Integer PerThread::randomBinomialByBtrd(Integer n, Real p) {
        Integer k = -1;

        /* Step 0 */
        BinomialTerms bt;
        if (n != binomialLastN || p != binomialLastP) {
            Real q = 1.0 - p;

            bt.m       = static_cast<Integer>((n + 1) * p);
            bt.r       = p / q;
            bt.nr      = (n + 1.0) * bt.r;
            bt.npq     = n * p * q;
            bt.twoNpq  = 2.0 * bt.npq;

            Real sqrtnpq = std::sqrt(bt.npq);

            bt.b       = 1.15 + 2.53 * sqrtnpq;
            bt.a       = -0.0873 + 0.0248 * bt.b + 0.01 * p;
            bt.c       = n * p + 0.5;
            bt.alpha   = (2.83 + 5.1 / bt.b) * sqrtnpq;
            bt.vr      = 0.92 - 4.2 / bt.b;
            bt.urvr    = 0.86 * bt.vr;
            bt.twoUrVr = 2.0 * bt.urvr;
            bt.nm      = n - bt.m + 1;                                   /* Step 3.3 */
            bt.h       = (                                               /* Step 3.3 */
                  (bt.m + 0.5) * std::log((bt.m + 1.0) / (bt.r * bt.nm)) /* Step 3.3 */
                + rescaledBinomialHistogram(bt.m)                        /* Step 3.3 */
                + rescaledBinomialHistogram(n - bt.m)                    /* Step 3.3 */
            );                                                           /* Step 3.3 */

            binomialLastN = n;
            binomialLastP = p;
            binomialTerms = bt;
        } else {
            bt = binomialTerms;
        }

        /* Step 1 */
        Real v;
        do {
            v = randomInclusive();
        } while (!(v <= bt.urvr) && !randomBinomialDecomposition(k, n, v, bt));

        if (v <= bt.urvr) {
            Real u = (v / bt.vr) - 0.43;
            k = static_cast<Integer>(bt.c + u * (bt.b + 2.0 * bt.a / (0.5 - std::abs(u))));
        } else {
            // k term is calculated in the decomposition path function
        }

        return k;
    }


    Integer PerThread::randomBinomialByInvertedDistribution(Integer n, Real p) {
        // This is based on the code used in the Boost libraries v. 1.70.
        Real q     = 1.0 - p;
        Real s     = p / q;
        Real a     = (n + 1) * s;
        Real r     = std::pow(q, static_cast<Real>(n));
        Real lastR = r;
        Real u      = randomInclusive();

        Integer k = 0;
        while (u > r && (r >= lastR || r >= epsilon)) {
            ++k;
            u -= r;

            lastR = r;
            r *= (a / static_cast<Real>(k)) - s;
        }

        return k;
    }
}
