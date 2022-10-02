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
* This file implements a collection of basic math functions used by many or most models.
***********************************************************************************************************************/

#include <cassert>
#include <complex>
#include <cmath>
#include <limits>
#include <vector>
#include <cassert>
#include <cstdint>
#include <chrono>

#include <boost/math/special_functions/lambert_w.hpp>
#include <boost/math/special_functions/zeta.hpp>

#include "model_exceptions.h"
#include "m_per_thread.h"
#include "m_api_types.h"
#include "m_intrinsic_types.h"
#include "m_variant.h"
#include "m_special_values.h"
#include "m_basic_functions.h"

namespace M {
    const Real defaultLambertWEpsilon = 4.0 * std::numeric_limits<Real>::epsilon();

    static const Integer minimumInteger = std::numeric_limits<Integer>::min();
    static const Integer maximumInteger = std::numeric_limits<Integer>::max();

    // Coefficients shamelessly lifted from https://en.wikipedia.org/wiki/Lanczos_approximation,
    static constexpr unsigned numberLanczosCoefficients = 8;
    static constexpr Real     lanczosA                  = 0.99999999999980993;
    static const     Real     lanczosB                  = std::sqrt(2.0 * pi);
    static const     Real     lanczosCoefficients[numberLanczosCoefficients] = {
          676.5203681218851,
        -1259.1392167224028,
          771.32342877765313,
         -176.61502916214059,
           12.507343278686905,
           -0.13857109526572012,
            9.9843695780195716e-6,
            1.5056327351493116e-7
    };

    static constexpr unsigned numberSterlingCoefficients = 6;
    static const Real         sterlingCoefficients[numberSterlingCoefficients] = {
        8.33333333333333287074040641e-02,
        8.33333333333333287074040641e-02,
        1.63888888888888889505679458e-01,
        4.83333333333333281522925517e-01,
        1.90357142857142891401167617e+00,
        9.38690476190476097428927460e+00 //,
//        5.56271825396825434495440277e+01,
//        3.85061111111111131322104484e+02,
//        3.04937028619528609851840883e+03,
//        2.71906628787878835282754153e+04,
//        2.69592445899933460168540478e+05,
//        2.94214534606227139011025429e+06,
//        3.50467952851800993084907532e+07,
//        4.52484628001388967037200928e+08,
//        6.29402423290470790863037109e+09,
//        9.38382340807317047119140625e+10,
//        1.49281524544799609375000000e+12,
//        2.52401794450722695312500000e+13,
//        4.51981687567429812500000000e+14,
//        8.54550564657985900000000000e+15,
    };

    static const Real         log0p5                            = std::log(static_cast<Real>(0.5));
    static constexpr unsigned maximumNumberLowerGammaIterations = 100;
    static constexpr unsigned maximumNumberUpperGammaIterations = 1000;

    static const Real resiprocalSqrtPi = 1.0 / std::sqrt(pi);
    static const Real erfConstant      = 2 * resiprocalSqrtPi;

    static std::vector<Real> generateFactorials();
    static std::vector<Real> generateLogFactorials();

    static const std::vector<Real> factorialTable(generateFactorials());
    static const std::vector<Real> logFactorialTable(generateLogFactorials());

    static constexpr Real     lambertWPadeNumerator0 = 0.0;
    static constexpr Real     lambertWPadeNumerator1 = 1.0;
    static constexpr Real     lambertWPadeNumerator2 = 2.682352941176471717;
    static constexpr Real     lambertWPadeNumerator3 = 1.326470588235293846;

    static constexpr Real     lambertWPadeDenominator0 = 1.0;
    static constexpr Real     lambertWPadeDenominator1 = 3.682352941176471717;
    static constexpr Real     lambertWPadeDenominator2 = 3.508823529411765563;
    static constexpr Real     lambertWPadeDenominator3 = 0.651960784313724284;

    static constexpr Real     lambertWSeries0 = -1.0;
    static constexpr Real     lambertWSeries1 =  1.0;
    static constexpr Real     lambertWSeries2 = -1.0/3.0;
    static constexpr Real     lambertWSeries3 =  11.0/72.0;
    static constexpr unsigned maximumLambertWHalleyIterations = 500;

    typedef boost::math::policies::policy<
        boost::math::policies::domain_error<boost::math::policies::errno_on_error>,
        boost::math::policies::pole_error<boost::math::policies::errno_on_error>,
        boost::math::policies::overflow_error<boost::math::policies::errno_on_error>,
        boost::math::policies::evaluation_error<boost::math::policies::errno_on_error>
    > LambertWBoostPolicy;

    static const Real oneOverE = 1.0 / e;
    static const Real maximumAllowedInverseLowerGammaError = 10.0 * std::numeric_limits<Real>::epsilon();

    typedef std::complex<double> ZC;

    // For most values, we converge quite quickly.  I've found a few cases where we need up to 94 iterations to
    // converge.
    static constexpr unsigned maximumZetaIterations = 100;

    static inline const std::complex<Complex::T>& toStdComplex(const Complex& value) {
        return reinterpret_cast<const std::complex<Complex::T>&>(value);
    }

    static inline const Complex& toComplex(const std::complex<Complex::T>& value) {
        return reinterpret_cast<const Complex&>(value);
    }


    static inline Complex exp(const Complex& x) {
        return toComplex(std::exp(toStdComplex(x)));
    }


    static std::vector<Real> generateFactorials() {
        std::vector<Real> factorials;

        std::vector<Real>::size_type i          = 1;
        Real                         lastResult = Real(1);
        do {
            factorials.push_back(lastResult);

            lastResult *= static_cast<Real>(i);
            ++i;
        } while (!std::isinf(lastResult));

        factorials.shrink_to_fit();
        return factorials;
    }


    static std::vector<Real> generateLogFactorials() {
        std::vector<Real> logFactorials;

        std::vector<Real>::size_type i          = 1;
        Real                         lastResult = Real(1);
        do {
            logFactorials.push_back(std::log(lastResult));

            lastResult *= static_cast<Real>(i);
            ++i;
        } while (!std::isinf(lastResult));

        logFactorials.shrink_to_fit();
        return logFactorials;
    }


    static inline Complex resiprocal(const Complex& value) {
        Complex::T a = value.real();
        Complex::T b = value.imag();
        Complex::T d = (a * a) + (b * b);

        return Complex(a/d, -b/d);
    }


    static Real lanczosApproximation(Real value) {
        // Lanczos approximation is not valid for real values less than 0.5.
        assert(value >= 0.5);

        Real x = (
              lanczosA
            + lanczosCoefficients[0] / (value + 0)
            + lanczosCoefficients[1] / (value + 1)
            + lanczosCoefficients[2] / (value + 2)
            + lanczosCoefficients[3] / (value + 3)
            + lanczosCoefficients[4] / (value + 4)
            + lanczosCoefficients[5] / (value + 5)
            + lanczosCoefficients[6] / (value + 6)
            + lanczosCoefficients[7] / (value + 7)
        );

        Real t      = value + numberLanczosCoefficients - 1.5;
        Real result = lanczosB * std::pow(t, value - 0.5) * std::exp(-t) * x;

        if (std::isnan(result) && value > static_cast<Real>(factorialTable.size() - 1)) {
            result = +infinity;
        }

        return result;
    }


    static Complex lanczosApproximation(const Complex& value) {
        // Lanczos approximation is not valid for real values less than 0.5.
        assert(value.real() >= 0.5);

        Complex x = (
              lanczosA
            + lanczosCoefficients[0] / (value + 0)
            + lanczosCoefficients[1] / (value + 1)
            + lanczosCoefficients[2] / (value + 2)
            + lanczosCoefficients[3] / (value + 3)
            + lanczosCoefficients[4] / (value + 4)
            + lanczosCoefficients[5] / (value + 5)
            + lanczosCoefficients[6] / (value + 6)
            + lanczosCoefficients[7] / (value + 7)
        );

        Complex t = value + numberLanczosCoefficients - 1.5;

        std::complex<Complex::T> st(t.real(), t.imag());
        Complex result = lanczosB * toComplex(std::pow(st, toStdComplex(value - 0.5))) * toComplex(std::exp(-st)) * x;

        if (std::isnan(result.real()) && value.real() > static_cast<Real>(factorialTable.size() - 1)) {
            result = Complex(+infinity, result.imag());
        }

        return result;
    }


    static Real sterlingsApproximation(Real value) {
        // 5 terms appears to provide best worst-case error as well as the lowest number of terms to asymtotically
        // reach the minimum average error.

        Real vn1 = value + 1;
        Real vn2 = vn1 * (value + 2);
        Real vn3 = vn2 * (value + 3);
        Real vn4 = vn3 * (value + 4);
        Real vn5 = vn4 * (value + 5);

        Real result = (
              value * std::log(value)
            - value
            + 0.5 * std::log(2 * pi / value)
            + sterlingCoefficients[0] / vn1
            + sterlingCoefficients[1] / vn2
            + sterlingCoefficients[2] / vn3
            + sterlingCoefficients[3] / vn4
            + sterlingCoefficients[4] / vn5
        );

        return result;
    }


    static Complex sterlingsApproximation(const Complex& value) {
        // For complex values, 6 terms appears to provide best worst-case error as well as the lowest number of terms
        // to asymptotically reach the minimum average error.

        Complex vn1 = value + 1;
        Complex vn2 = vn1 * (value + 2);
        Complex vn3 = vn2 * (value + 3);
        Complex vn4 = vn3 * (value + 4);
        Complex vn5 = vn4 * (value + 5);
        Complex vn6 = vn5 * (value + 6);

        Complex result = (
              value * toComplex(std::log(toStdComplex(value)))
            - value
            + 0.5 * toComplex(std::log(toStdComplex(2 * pi / value)))
            + sterlingCoefficients[0] / vn1
            + sterlingCoefficients[1] / vn2
            + sterlingCoefficients[2] / vn3
            + sterlingCoefficients[3] / vn4
            + sterlingCoefficients[4] / vn5
            + sterlingCoefficients[5] / vn6
        );

        return result;
    }


    Real unsignedSterlingNumber1Engine(Real n, Real k) {
        Real result;

        if (n <= 0) {
            if (k <= 0) {
                result = 1.0;
            } else {
                result = 0.0;
            }
        } else if (k <= 0) {
            result = 0;
        } else {
            result = (n - 1) * unsignedSterlingNumber1Engine(n - 1, k) + unsignedSterlingNumber1Engine(n - 1, k - 1);
        }

        return result;
    }


    Real alphaz(Real x) {
        // ACM Transactions On Mathematical Software, Volume 5, Number 4, December 1979 (eq 4.14, p. 476) notes
        // that the implementation of the upper gamma function will converge quickly for cases where s <= alpha*(x)
        // where alpha*(x) is defined as:
        //
        //               /
        //              | x + 0.25         0.25 <= x < inf
        //  alpha*(x) = |
        //              | ln(0.5) / ln(x)  otherwise
        //               \                 .
        //
        // So this should be the limiting condition for the choice of this method to calculate Gu(s,x).

        Real result;

        if (x >= 0.25) {
            result = x + 0.25;
        } else {
            result = log0p5 / std::log(x);
        }

        return result;
    }


    Real lowerRegularizedGammaEngine1(Real s, Real x, Real gammaS) {
        // See https://en.wikipedia.org/wiki/incomplete_gamma_function.
        //
        // This method uses the power series expansion of the lower incomplete gamma function:
        //                              inf       x ^ k
        //    P(s,x) = x ^ s * e^(-x) * Sum ------------------
        //                              k=0  gamma(s + k + 1)
        //
        // As an optimization, this function also relies on the relation gamma(z+1) = z * gamma(z) so the multiplier
        // and denominator gamma term need only be calculated once for the entire function.

        Real     result;
        Real     sum      = 0;
        Real     lastSum  = 0;
        Real     xk       = 1;
        unsigned k        = 0;
        Real     spk      = s;          // This represents s + k with k=0 at the beginning.
        Real     gsk1     = s * gammaS; // This represents gamma(s + k + 1) for k = 0;

        do {
            lastSum = sum;

            sum += xk / gsk1;

            xk   *= x;
            spk  += 1;
            gsk1  = spk * gsk1;

            ++k;
        } while (sum != lastSum && !std::isnan(sum) && k < maximumNumberLowerGammaIterations);

        result = std::pow(x, s) * std::exp(-x) * sum;
        return result;
    }


    Real lowerRegularizedGammaEngine2(Real s, Real x, Real lnGammaS) {
        // See https://en.wikipedia.org/wiki/incomplete_gamma_function.
        //
        // This method uses the power series expansion of the lower incomplete gamma function:
        //                              inf       x ^ k
        //    P(s,x) = x ^ s * e^(-x) * Sum ------------------
        //                              k=0  gamma(s + k + 1)
        //
        // This version is designed to work better for large values of s.  Rather than calculating gamma(s + k + 1)
        // directly, this method uses ln gamma.
        //
        // As an optimization, this function also relies on the relation gamma(z+1) = z * gamma(z) so the multiplier
        // and denominator gamma term need only be calculated once for the entire function.
        //
        // For small values of s, we calculate the value of the gamma function directly.  For larger values of s, we
        // fall back on lnGamma.

        Real     result;
        Real     sum      = 0;
        Real     lastSum  = 0;
        unsigned k        = 0;
        Real     lnS      = std::log(s);
        Real     lnX      = std::log(x);
        Real     lnXk     = 0;
        Real     spk      = s;              // This represents s + k with k=0 at the beginning.
        Real     lnGsk1   = lnS + lnGammaS; // This represents lnGamma(s + k + 1) for k = 0;

        do {
            lastSum = sum;

            sum += std::exp(lnXk - lnGsk1);

            lnXk  += lnX;
            spk   += 1;
            lnGsk1 = std::log(spk) + lnGsk1;

            ++k;
        } while (sum != lastSum && !std::isnan(sum) && k < maximumNumberLowerGammaIterations);

        result = std::pow(x, s) * std::exp(-x) * sum;
        return result;
    }


    Real upperIncompleteGammaEngine(Real s, Real z) {
        // This method is based on the continued fraction form of the upper incomplete gamma function:
        //
        //                z ^ s * e ^ (-z)
        //     Gu(s,z) = ---------------------------------------------------------------------
        //                             s - 1
        //                1 + z - s + --------------------------------------------------------
        //                                         2(s - 2)
        //                            3 + z - s + --------------------------------------------
        //                                                      3(s - 3)
        //                                         5 + z - s + -------------------------------
        //                                                                   4(s - 4)
        //                                                      7 + z - s + ------------------
        //                                                                   9 + z - s + ....
        //
        // (see https://en.wikipedia.org/wiki/incomplete_gamma_function).
        //
        // ACM Transactions On Mathematical Software, Volume 5, Number 4, December 1979 (eq 4.14, p. 475) notes that we
        // can rewrite the above continued fraction as:
        //
        //                                       1
        //     (z + 1 - s) z^(-s) e^x Gu(s,z) = ------------------------
        //                                            a1
        //                                       1 + -------------------
        //                                                 a2
        //                                            1 + --------------
        //                                                      a3
        //                                                 1 + ---------
        //                                                      1 + ...
        //
        //                           k (s - k)
        // where: ak = -----------------------------------    (k = 1, 2, 3, ...)
        //              (z + 2k - 1 - s) (z + 2k + 1 - s)
        //
        // We can rewrite the above continued fraction as the infinite series:
        //
        //      1                         inf
        //     ------------------------ = Sum tk
        //           a1                   k=0
        //      1 + -------------------
        //               a2
        //           1 + --------------
        //                     a3
        //                1 + ---------
        //                     1 + ...
        //
        // where: t0 = 1    p0 = 0
        //
        //        tk = p1 p2 p3 ... pk
        //
        //              -ak (1 + p(k-1))
        //        pk = ----------------------
        //              1 + ak [1 + p(k-1)]
        //
        // The ACM article also points out that, if we define ok = 1 = pk, then we can define our series using the
        // much simpler relation:
        //
        //                         1
        // o0 = 1      ok = ---------------     (k = 1, 2, 3, ...)
        //                   1 + ak o(k-1)
        //
        // The paper also notes that the term for ak can be simplified further.  We can define the terms, modified
        // from the ACM paper to avoid confusion between upper/lower case and greek/roman letters:
        //
        //   uk = -k (z - k)     qk = (z + 2k - 1 - s)(z + 2k +1 - s)     vk = 4(z + 2k + 1 - s)     wk = 2k - s + 1
        //
        // With the above relations, we can calculate our tk terms by:
        //
        //   u0 = 0      q0 = (z - 1 - s)(z + 1 - s)   v0 = 4(z + 1 - s)     w0 = -s + 1
        //
        //   p0 = 0      t0 = 1
        //
        // And, for k = 1, 2, 3, ...:
        //
        //   uk  = u(k-1) + w(k-1)
        //   qk  = q(k-1) + v(k-1)
        //   vk  = v(k-1) + 8
        //   wk  = w(k-1) + 2
        //   tau = uk (1 + p(k-1))
        //   pk  = tau / (qk - tau)
        //   tk  = pk t(k-1)
        //

        Real u     = 0;
        Real zms   = z - s;
        Real zmsp1 = zms + 1;
        Real q     = (zms - 1) * zmsp1;
        Real v     = 4.0 * zmsp1;
        Real w     = 1 - s;
        Real p     = 0;
        Real t     = 1;

        Real     sum     = 1;
        Real     lastSum = 0;
        unsigned k       = 1;
        do {
            Real uk  = u + w;
            Real qk  = q + v;
            Real tau = uk * (1.0 + p);

            p = tau / (qk - tau);
            t = p * t;

            lastSum = sum;
            sum += t;

            u = uk;
            q = qk;
            v = v + 8.0;
            w = w + 2.0;

            ++k;
        } while (lastSum != sum && k < maximumNumberUpperGammaIterations);

        Real factor = zmsp1 * std::pow(z, -s) * std::exp(z);
        Real result = sum / factor;

        return result;
    }


    Complex lowerRegularizedGammaEngine1(const Complex& s, const Complex& x, const Complex& gammaS) {
        // See the write-up on the real version of this function for notes.

        Complex  result;
        Complex  sum      = 0;
        Complex  lastSum  = 0;
        Complex  xk       = 1;
        unsigned k        = 0;
        Complex  spk      = s;
        Complex  gsk1     = s * gammaS;

        do {
            lastSum = sum;

            sum += xk / gsk1;

            xk   *= x;
            spk  += 1;
            gsk1  = spk * gsk1;

            ++k;
        } while (sum != lastSum && !std::isnan(sum.real()) && k < maximumNumberLowerGammaIterations);

        result = pow(x, s) * exp(-x) * sum;
        return result;
    }


    Complex lowerRegularizedGammaEngine2(const Complex& s, const Complex& x, const Complex& lnGammaS) {
        // See the write-up on the real version of this function for notes.

        Complex  result;
        Complex  sum      = 0;
        Complex  lastSum  = 0;
        unsigned k        = 0;
        Complex  lnS      = ln(s);
        Complex  lnX      = ln(x);
        Complex  lnXk     = 0;
        Complex  spk      = s;
        Complex  lnGsk1   = lnS + lnGammaS;

        do {
            lastSum = sum;

            sum += exp(lnXk - lnGsk1);

            lnXk  += lnX;
            spk   += 1;
            lnGsk1 = ln(spk) + lnGsk1;

            ++k;
        } while (sum != lastSum && !std::isnan(sum.real()) && k < maximumNumberLowerGammaIterations);

        result = pow(x, s) * exp(-x) * sum;
        return result;
    }


    Complex upperIncompleteGammaEngine(const Complex& s, const Complex& z) {
        // See the write-up on the real version of this function for notes.

        Complex  u     = 0;
        Complex  zms   = z - s;
        Complex  zmsp1 = zms + 1;
        Complex  q     = (zms - 1) * zmsp1;
        Complex  v     = 4.0 * zmsp1;
        Complex  w     = 1 - s;
        Complex  p     = 0;
        Complex  t     = 1;

        Complex  sum     = 1;
        Complex  lastSum = 0;
        unsigned k       = 1;
        do {
            Complex uk  = u + w;
            Complex qk  = q + v;
            Complex tau = uk * (1.0 + p);

            p = tau / (qk - tau);
            t = p * t;

            lastSum = sum;
            sum += t;

            u = uk;
            q = qk;
            v = v + 8.0;
            w = w + 2.0;

            ++k;
        } while (lastSum != sum && k < maximumNumberUpperGammaIterations);

        Complex factor = zmsp1 * pow(z, -s) * exp(z);
        Complex result = sum / factor;

        return result;
    }


    static_assert(
        sizeof(ZC) * maximumZetaIterations <= PerThread::temporaryBufferSizeInBytes,
        "M::PerThread temporary buffer too small"
    );

    static ZC reimannZetaByAlternatingSeries(PerThread& pt, const ZC& s) {
        ZC result;

        if (s.imag() == 0) {
            // If s is real, we use the Boost implementation since they include a lot of nice additional shortcuts
            // to make computation faster.

            result = ZC(boost::math::zeta<ZC::value_type>(s.real()));
        } else {
            // We have a truly complex value -- life sucks, can't use Boost's goodness.
            //
            // For this, we use convergence of alternating series method (see
            // http://numbers.computation.free.fr/Constants/Miscellaneous/zetaevaluations.pdf).

            static constexpr unsigned requiredConverged = 2;   // The number of required converged iterations.
            static constexpr unsigned lastIteration     = maximumZetaIterations - 1;

            ZC* a = reinterpret_cast<ZC*>(pt.temporaryBuffer());

            a[0] = ZC(0.5) / (ZC(1.0) - std::pow(ZC(2.0), (ZC(1.0) - s)));
            result = a[0];

            ZC       lastResult;
            unsigned remainingConverged = requiredConverged;
            unsigned n                  = 0;

            do {
                lastResult = result;
                ++n;

                ZC nc(n);
                for (unsigned k=0 ; k<n ; ++k) {
                    a[k] *= ZC(0.5) * nc / (nc - ZC(k));
                    result += a[k];
                }

                a[n] = -a[n-1] * std::pow(nc / (nc + ZC(1.0)), s) / nc;
                result    += a[n];

                if (std::abs(result - lastResult) / std::abs(result) <= std::numeric_limits<double>::epsilon()) {
                    --remainingConverged;
                } else {
                    remainingConverged = requiredConverged;
                }
            } while (n < lastIteration && remainingConverged > 0);
        }

        return result;
    }


    Real erfEngine(Real x) {
        // This method relies on the series expansion documented in "Numerical Methods That Work", Copyright 1990
        // by Forman S. Action, p. 16
        //
        // This implementation will work for values of 0 <= x <= 4 only.  For values above
        // 4, used erfcEngine.

        Real x2              = x * x;
        Real x2t2            = 2.0 * x2;
        Real numerator       = 1;
        Real denominator     = 1;
        Real denominatorTerm = 1;
        Real sum             = 1;
        Real lastSum         = 0;

        do {
            denominatorTerm += 2;

            numerator   *= x2t2;
            denominator *= denominatorTerm;

            lastSum = sum;
            sum += numerator / denominator;
        } while (sum != lastSum);

        Real result = erfConstant * std::exp(-x2) * x * sum;
        return result;
    }


    Real erfcEngine(Real x) {
        // This method relies on the continued fraction form of the erfc function defined by:
        //
        //                exp(-x^2)                   1
        //     erfc(x) = ----------- -----------------------------------
        //                sqrt(pi)                   1/2
        //                            x + ------------------------------
        //                                                2/2
        //                                 x + -------------------------
        //                                                  3/2
        //                                      x + --------------------
        //                                                     4/2
        //                                           x + ---------------
        //                                                       5/2
        //                                                x + ----------
        //                                                     x + ....
        //
        // For real values, the number of iterations to get below 1.0E-15 relative error appears to approximately
        // follow:
        //                        100
        //     Niterations = 2 + -----
        //                         x

        unsigned numberIterations = static_cast<unsigned>(2 + std::ceil(100 / x));
        Real     numerator        = numberIterations / 2.0;
        Real     f                = 0;

        while (numerator >= 0.5) {
            f = numerator / ( x + f);
            numerator -= 0.5;
        }

        Real result = resiprocalSqrtPi * std::exp(-x * x) / (x + f);
        return result;
    }


    Complex erfEngine(const Complex& x) {
        // This method relies on the series expansion documented in "Numerical Methods That Work", Copyright 1990
        // by Forman S. Action, p. 16
        //
        // This implementation will work for values of 0 <= x <= 4 only.  For values above
        // 4, used erfcEngine.

        Complex x2              = x * x;
        Complex x2t2            = 2.0 * x2;
        Real    denominator     = 1;
        Real    denominatorTerm = 1;
        Complex numerator(1);
        Complex sum(1);
        Complex lastSum(0);

        do {
            denominatorTerm += 2;

            numerator   *= x2t2;
            denominator *= denominatorTerm;

            lastSum = sum;
            sum += numerator / denominator;
        } while (sum != lastSum);

        Complex result = erfConstant * exp(-x2) * x * sum;
        return result;
    }


    Complex erfcEngine(const Complex& x) {
        // This method relies on the continued fraction form of the erfc function defined by:
        //
        //                exp(-x^2)                   1
        //     erfc(x) = ----------- -----------------------------------
        //                sqrt(pi)                   1/2
        //                            x + ------------------------------
        //                                                2/2
        //                                 x + -------------------------
        //                                                  3/2
        //                                      x + --------------------
        //                                                     4/2
        //                                           x + ---------------
        //                                                       5/2
        //                                                x + ----------
        //                                                     x + ....
        //
        // For complex values, number of iterations appears to follow a more complex relationship than with real
        // numbers.  Experimentation shows that comparing against the magnitude is the best simple relationship althoug
        // the approach may mean more iterations are needed for a given value.
        //
        //                        138
        //     Niterations = 4 + -----
        //                        |x|

        Real     magnitudeX       = M::abs(x);
        unsigned numberIterations = static_cast<unsigned>(4 + std::ceil(140 / magnitudeX));
        Real     numerator        = numberIterations / 2.0;
        Complex  f(0);

        while (numerator >= 0.5) {
            f = numerator / ( x + f);
            numerator -= 0.5;
        }

        Complex result = resiprocalSqrtPi * exp(-x * x) / (x + f);
        return result;
    }


    Complex lambertWPadeGuess(const Complex& x) {
        // See Corless, Gonnet, Hare, Jeffrey, Knuth, p. 355.
        //   Also see Paul Smith's write-up on the Pade approximant algorithm.

        Complex x2 = x * x;
        Complex x3 = x2 * x;

        Complex numerator = (
              lambertWPadeNumerator0
            + lambertWPadeNumerator1 * x
            + lambertWPadeNumerator2 * x2
            + lambertWPadeNumerator3 * x3
        );

        Complex denominator = (
            + lambertWPadeDenominator0
            + lambertWPadeDenominator1 * x
            + lambertWPadeDenominator2 * x2
            + lambertWPadeDenominator3 * x3
        );

        Complex result = numerator / denominator;
        return result;
    }


    Complex lambertWSeriesExpansionGuess(Integer k, const Complex& x) {
        // See Corless, Gonnet, Hare, Jeffrey, Knuth, eq. 4.20, p. 350 for details.

        Complex kTerm(0.0, 2.0 * pi * k);
        Complex lnx    = ln(x);
        Complex result = lnx + kTerm - ln(lnx + kTerm);

        return result;
    }


    Complex lambertWBranchPointGuess(const Complex& x) {
        // See Corless, Gonnet, Hare, Jeffrey, Knuth, eq. 4.22, p. 349, 350 for details.

        Complex p  = sqrt(2.0 * e * x + 1);
        Complex p2 = p * p;
        Complex p3 = p2 * p;

        Complex result = (
              lambertWSeries0
            + lambertWSeries1 * p
            + lambertWSeries2 * p2
            + lambertWSeries3 * p3
        );

        return result;
    }


    Complex lambertWHalley(const Complex& initialGuess, const Complex& z, Real epsilon) {
        // See Corless, Gonnet, Hare, Jeffrey, Knuth, p. 354, 355.
        //
        // Note that we use Householder's Method with third derivatives of xe^x rather then Halley's method as Halley's
        // method appears to repeatedly over compensate and fail to converge for some values.  By comparison,
        // Householder converges rapidly.
        //
        // specifically:
        //                            1   f''(w)
        //                       1 + --- -------- h
        //                            2   f'(w)
        //    wn = w + h -------------------------------------
        //                      f''(w)       1   f'''(w)
        //                 1 + -------- h + --- --------- h^2
        //                      f'(w)        6    f'(w)
        //
        // where:
        //            f(w)
        //     h = - -------
        //            f'(w)

        Complex  lastW;
        Real     error;
        unsigned iteration = 0;
        Complex  w         = initialGuess;

        do {
            Complex ew      = exp(w);
            Complex wew     = w * ew;
            Complex f       = wew - z;     // f = w ^ e^w - z
            Complex df      = ew + wew;    // first derivative of f:  f' = (w + 1) e^w
            Complex ddf     = ew + df;     // second derivative of f: f" = (w + 2) e^w
            Complex dddf    = ew + ddf;    // third derivative of f:  f''' = (w + 3) e^w
            Complex hn      = - (f / df);
            Complex ddfodf  = ddf / df;    // ratio of the second over the first derivative
            Complex dddfodf = dddf / df;   // ratio of the third over the first derivative

            Complex numerator   = 1 + (0.5 * ddfodf * hn);
            Complex denominator = 1 + (ddfodf + (dddfodf * hn / 6.0)) * hn;

            lastW = w;
            w += hn * numerator / denominator;

            error = lastW == 0 ? abs(w - lastW) : abs(w - lastW) / abs(lastW);
            ++iteration;
        } while (error > epsilon && iteration < maximumLambertWHalleyIterations);

        if (iteration >= maximumLambertWHalleyIterations && error > epsilon) {
            w = Complex(NaN, NaN);
        }

        return w;
    }


    Complex lambertWCorless(Integer k, const Complex& z, Real epsilon) {
        // Calculates branch 0 of the Lambert W function for real values.
        //
        // z will never be NaN of infinity when we enter.  k will never be 0 or -1 (we handle that separately)

        Complex result;

        Real zReal = z.real();
        Real zImag = z.imag();
        if (zReal == 0 && zImag == 0) {
            result = Complex(-infinity);
        } else {
            // As per Corless, determine our initial guess value.

            Complex guess;
            if (k == 0) {
                if (abs(z + oneOverE) < 0.3) {
                    guess = lambertWBranchPointGuess(z);
                } else {
                    Real absZImag = std::abs(zImag);
                    if (zReal > -1.0 && zReal < 1.5 && absZImag < 1.0 && (-2.5 * absZImag - 0.2) < zReal) {
                        // The above conditions were shamelessly lifted from the SciPy library source and were
                        // determined by their developers.  I see no reason not to honor their insight.

                        guess = lambertWPadeGuess(z);
                    } else {
                        guess = lambertWSeriesExpansionGuess(k, z);
                    }
                }
            } else if (k == -1) {
                if (abs(z) <= oneOverE && zImag == 0 && zReal < 0) {
                    guess = std::log(-zReal);
                } else {
                    guess = lambertWSeriesExpansionGuess(k, z);
                }
            } else {
                guess = lambertWSeriesExpansionGuess(k, z);
            }

            result = lambertWHalley(guess, z, epsilon);
        }

        return result;
    }


    static Real inverseLowerGammaBinarySearch(Real s, Real y, Real lowerZ, Real upperZ) {
        Real z;
        do {
            z = (upperZ + lowerZ) / 2.0;
            Real midY = lowerGamma(s, z);

            if (midY > y) {
                upperZ = z;
            } else {
                lowerZ = z;
            }
        } while ((upperZ - lowerZ) >= maximumAllowedInverseLowerGammaError);

        return z;
    }


    void internalReportInvalidRange() {
        throw Model::InvalidRangeParameter();
    }


    Integer systemTime() {
        return std::chrono::high_resolution_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
    }


    Boolean isNaN(Real value) {
        return std::isnan(value);
    }


    Boolean isNaN(const Complex& value) {
        return std::isnan(value.real()) || std::isnan(value.imag());
    }


    Boolean isNaN(const Variant& value) {
        ValueType valueType = value.valueType();
        switch (valueType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::SET:
            case ValueType::TUPLE:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(valueType, ValueType::REAL);
                return false;
            }

            case ValueType::BOOLEAN: {
                return false;
            }

            case ValueType::INTEGER: {
                return false;
            }

            case ValueType::REAL: {
                return std::isnan(value.toReal());
            }

            case ValueType::COMPLEX: {
                Complex v = value.toComplex();
                return std::isnan(v.real()) || std::isnan(v.imag());
            }

            default: {
                assert(false);
                return false;
            }
        }

    }


    Integer pow(Integer base, Integer exponent) {
        Integer result;

        if (exponent == 2) {
            result = base * base;
        } else if (exponent == 3) {
            result = base * base * base;
        } else {
            result = static_cast<Integer>(std::pow(base, exponent));

            // On at least MacOS, when an overflow occurs, we end up with a maximal negative value even when the
            // exponent is even or the base is positive.  We address that issue here.
            if (result == minimumInteger && (base >= 0 || (exponent & 1) == 0)) {
                result = maximumInteger;
            }
        }

        return result;
    }


    Real pow(Real base, Integer exponent) {
        Real result;

        if (exponent == 2) {
            result = base * base;
        } else if (exponent == 3) {
            result = base * base * base;
        } else {
            // On at least MacOS, the std::pow function does not handle NaN cleanly.  We address that here.
            if (std::isnan(base)) {
                result = NaN;
            } else {
                if (exponent == 0 && (base == Real(0) || std::isinf(base))) {
                    result = NaN;
                } else {
                    result = std::pow(base, exponent);
                }
            }
        }

        return result;
    }


    Complex pow(Integer base, Real exponent) {
        return pow(Real(base), exponent);
    }


    Complex pow(Real base, Real exponent) {
        Complex result;

        if (exponent == Real(0.5)) {
            result = sqrt(base);
        } else if (exponent == Real(2)) {
            result = base * base;
        } else if (exponent == Real(3)) {
            result = base * base * base;
        } else {
            // The standard math libraries don't handle infinities and NaN correctly so we address those here.

            if (std::isnan(base) || std::isnan(exponent)) {
                result = Complex(NaN, 0);
            } else if (exponent == Real(0) && (base == Real(0) || std::isinf(base))) {
                result = Complex(NaN, 0);
            } else if (std::isinf(exponent)) {
                if (base == Real(0)) {
                    result = Complex(NaN, 0);
                } else {
                    if (exponent < 0) {
                        base = Real(1) / base;
                    }

                    if (base > Real(1)) {
                        result = Complex(+infinity, 0);
                    } else if (base == Real(1)) {
                        result = Complex(1, 0);
                    } else if (base == Real(0)) {
                        result = Complex(NaN, 0);
                    } else if (base > Real(-1) /* && base < Real(1) && base != Real(0) */) {
                        result = Complex(0, 0);
                    } else /* if (base <= Real(-1)) */ {
                        result = Complex(NaN, 0);
                    }
                }
            } else {
                std::complex<M::Complex::T> complexBase(base, 0);
                std::complex<M::Complex::T> stdResult(std::pow(complexBase, exponent));

                result = toComplex(stdResult);
            }
        }

        return result;
    }


    Complex pow(const Complex& base, Integer exponent) {
        return pow(base, Complex(static_cast<M::Complex::T>(exponent)));
    }


    Complex pow(Integer base, const Complex& exponent) {
        return pow(Complex(static_cast<M::Complex::T>(base)), exponent);
    }


    Complex pow(const Complex& base, Real exponent) {
        return pow(base, Complex(exponent));
    }


    Complex pow(Real base, const Complex& exponent) {
        return pow(Complex(base), exponent);
    }


    Complex pow(const Complex& base, const Complex& exponent) {
        Complex result;

        if (exponent == Complex(0.5, 0)) {
            result = sqrt(base);
        } else if (exponent == Complex(2, 0)) {
            result = base * base;
        } else if (exponent == Complex(3, 0)) {
            result = base * base * base;
        } else {
            // The standard math libraries don't handle infinities and NaN correctly on all platforms so we address
            // those here.  Also note that some the behaviours were discovered with long double types and may not
            // be present with traditional double types but the tests are left in so that we can migrate between types
            // more easily.

            if (std::isnan(base.real())     ||
                std::isnan(base.imag())     ||
                std::isnan(exponent.real()) ||
                std::isnan(exponent.imag())    ) {
                // If base and exponent are real and the exponent is a whole, non-zero number, we return NaN + 0i
                // If the exponent is strictly zero, we return 1 + 0i
                // otherwise we return NaN + NaN i

                if (base.imag() == 0 && exponent.imag() == 0) {
                    if (std::isnan(exponent.real())) {
                        result = Complex(NaN, 0);
                    } else {
                        // base is real, exponent is real.
                        Real ew;
                        Real ef = std::modf(exponent.real(), &ew);
                        if (ef == 0.0L) {
                            // Base is real, exponent is real whole number.
                            result = Complex(NaN, 0);
                        } else {
                            result = Complex(NaN, NaN);
                        }
                    }
                } else {
                    result = Complex(NaN, NaN);
                }
            } else if (base == Complex(0, 0)) {
                if (exponent == Complex(0, 0)) {
                    // Special case of zero to the zero power.  Traditionally treated as NaN.
                    result = Complex(NaN, 0);
                } else if (std::isinf(exponent.real())) {
                    if (exponent.imag() == Real(0)) {
                        result = Complex(NaN, 0);
                    } else {
                        result = Complex(NaN, NaN);
                    }
                } else if (std::isinf(exponent.imag())) {
                    result = Complex(NaN, NaN);
                } else {
                    if (exponent.real() > 0) {
                        result = Complex(0, 0);
                    } else {
                        M::Real realMultiplier      = std::cos(exponent.imag());
                        M::Real imaginaryMultiplier = std::sin(exponent.imag());

                        M::Real realComponent      =   realMultiplier == 0
                                                     ? M::Real(0)
                                                     :   realMultiplier > 0
                                                       ? +infinity
                                                       : -infinity;
                        M::Real imaginaryComponent =   imaginaryMultiplier == 0
                                                     ? M::Real(0)
                                                     :   imaginaryMultiplier > 0
                                                       ? +infinity
                                                       : -infinity;

                        result = Complex(realComponent, imaginaryComponent);
                    }
                }
            } else if (std::isinf(base.real())     ||
                       std::isinf(base.imag())     ||
                       std::isinf(exponent.real()) ||
                       std::isinf(exponent.imag())    ) {

                // Base and/or exponent contains infinities.  Life gets really-really complex really fast.
                //
                // At one point this code attempted to go through *all* the possible cases; however, the number of
                // cases I found that required distinct handling was immense, especially when both base terms are non
                // zero.  For reasons that became obvious to me after several days of effort, I also noted that none of
                // the existing math libraries I could find even attempted to get this correct.
                //
                // In light of the unlikliness of getting this correct and in light of having no reference I can use
                // to validate the work, I have decided to defer this effort, only handling the simpler cases.

                if (exponent == Complex(0, 0)) {
                    if (std::isinf(base.real())) {
                        // Special case of infinity to the zero power.  Treat as NaN.
                        if (base.imag() == Real(0)) {
                            result = Complex(NaN, 0);
                        } else {
                            result = Complex(NaN, NaN);
                        }
                    } else if (std::isinf(base.imag())) {
                        result = Complex(NaN, NaN);
                    } else {
                        // Exponent is zero and base is finite so we don't care about the base.
                        result = Complex(1, 0);
                    }
                } else if (exponent == Complex(1, 0)) {
                    // Exponent is 1, result is our base.
                    result = base;
                } else if (std::isinf(exponent.imag())) {
                    // Exponent has infinite imaginary component.  Result can reside anywhere on a circle around the
                    // origin with distance based on base and real part of the exponent.  Result is clearly NaN.

                    result = Complex(NaN, NaN);
                } else {
                    // Exponent is real or has finite complex term.

                    M::Real realMultiplier      = std::cos(exponent.imag());
                    M::Real imaginaryMultiplier = std::sin(exponent.imag());

                    if (base.imag() == Real(0)) {
                        // Base is real

                        M::Real baseRaisedToRealExponent;
                        if (std::isinf(exponent.real())) {
                            // Exponent is infinite

                            if (exponent.real() > Real(0)) {
                                if (base.real() > Real(1)) {
                                    baseRaisedToRealExponent = +infinity;
                                } else if (base.real() == Real(1)) {
                                    baseRaisedToRealExponent = Real(1);
                                } else if (base.real() > Real(-1) /* && base.real() < Real(1) */) {
                                    baseRaisedToRealExponent = Real(0);
                                } else /* if (base.real() < Real(0)) */ {
                                    baseRaisedToRealExponent = NaN;
                                }
                            } else /* if (exponent.real() < 0) */ {
                                if (base.real() > Real(1)) {
                                    baseRaisedToRealExponent = std::isinf(base.real()) ? NaN : Real(0);
                                } else if (base.real() == Real(1)) {
                                    baseRaisedToRealExponent = Real(1);
                                } else if (base.real() >= Real(0) /* && base.real() < Real(1) */) {
                                    baseRaisedToRealExponent = +infinity;
                                } else if (base.real() >= Real(-1) /* && base.real() < Real(0) */) {
                                    baseRaisedToRealExponent = NaN;
                                } else /* if (base.real() < Real(-1)) */ {
                                    baseRaisedToRealExponent = std::isinf(base.real()) ? NaN : Real(0);
                                }
                            }
                        } else {
                            baseRaisedToRealExponent = std::pow(base.real(), exponent.real());
                        }

                        if (std::isnan(baseRaisedToRealExponent)) {
                            result = Complex(
                                realMultiplier != Real(0) ? NaN : Real(0),
                                imaginaryMultiplier != Real(0) ? NaN : Real(0)
                            );
                        } else {
                            result = Complex(
                                realMultiplier != 0 ? baseRaisedToRealExponent * realMultiplier : 0,
                                imaginaryMultiplier != 0 ? baseRaisedToRealExponent * imaginaryMultiplier : 0
                            );
                        }
                    } else if (base.real() == Real(0)) {
                        // Base is strictly imaginary (no real term)

                        if (std::isinf(exponent.real())) {
                            // Exponent is infinite

                            if (exponent.real() > Real(0)) {
                                if (base.imag() <= Real(-1) || base.imag() >= Real(1)) {
                                    // Can't determine sign of infinities.
                                    result = Complex(NaN, NaN);
                                } else /* if (base.imag() > Real(-1) && base.imag() < Real(1)) */ {
                                    result = Complex(0, 0);
                                }
                            } else /* if (exponent.real() < 0) */ {
                                if (base.imag() < Real(-1) || base.imag() > Real(1)) {
                                    if (std::isinf(base.imag())) {
                                        result = Complex(NaN, NaN);
                                    } else {
                                        result = Complex(0, 0);
                                    }
                                } else /* if (base.imag() >= Real(-1) || base.imag() <= Real(1)) */ {
                                    // Can't determine sign of infinities.
                                    result = Complex(NaN, NaN);
                                }
                            }
                        } else {
                            // Exponent is finite.  Real portion of base is 0, imaginary portion of the base must be
                            // infinite.

                            assert(std::isinf(base.imag()));

                            if (exponent.real() < 0) {
                                result = Complex(0, 0);
                            } else {
                                Real ew;
                                Real ef = std::modf(exponent.real(), &ew);
                                if (ef == 0.0L) {
                                    Complex multiplier(realMultiplier, imaginaryMultiplier);
                                    Real    baseSign = base.imag() > 0 ? Real(+1) : Real(-1);

                                    // Exponent is a whole number.  Result will ping-pong between the four quadrants.
                                    // exponent % 4  |             |   base > 0   |   base < 0
                                    // ------------  | ----------- | ------------ | ------------
                                    //            0  | i ^ 0 =  1  | +infinity, 0 | +infinity, 0
                                    //            1  | i ^ 1 =  i  | 0, +infinity | 0, -infinity
                                    //            2  | i ^ 2 = -1  | -infinity, 0 | +infinity, 0
                                    //            3  | i ^ 3 = -1i | 0, -infinity | 0, +infinity

                                    // Note we are relying on the fact that ew is whole and that the division is a strict
                                    // power of 2 to make the code below work reliably.
                                    Real remainder = std::abs((ew / 4.0) - std::trunc(ew / 4.0));
                                    if (remainder == Real(0)) {
                                        // exponent % 4 == 0
                                        result = +infinity * multiplier;
                                    } else if (remainder == Real(0.25)) {
                                        // exponent % 4 == 1
                                        result = Complex(0, baseSign * infinity) * multiplier;
                                    } else if (remainder == Real(0.5)) {
                                        // exponent % 4 == 2
                                        result = -baseSign * infinity * multiplier;
                                    } else {
                                        // exponent % 4 == 3
                                        result = Complex(0, -baseSign * infinity) * multiplier;
                                    }
                                } else {
                                    // Exponent is fractional.  Result will smoo into the real portion of the result.
                                    // While we *might* be able to determine the sign of our infinities, doing so
                                    // appears to be extremely complex.

                                    result = Complex(NaN, NaN);
                                }
                            }
                        }
                    } else {
                        // Base is complex.  Most cases end up as NaN.  For now we defer these.

                        if (std::isinf(base.real())) {
                            if (std::isinf(base.imag())) {
                                // Both base terms are infinite.
                                // TODO: Resolve these cases.
                                result = Complex(NaN, NaN);
                            } else {
                                // infinite real term, finite imaginary term.
                                // TODO: Resolve these cases.
                                result = Complex(NaN, NaN);
                            }
                        } else {
                            if (std::isinf(base.imag())) {
                                // finite real term, infinite imaginary term.
                                // TODO: Resolve these cases.
                                result = Complex(NaN, NaN);
                            } else {
                                // finite real and imaginary terms, infinite exponent.
                                // TODO: Resolve these cases.
                                result = Complex(NaN, NaN);
                            }
                        }
                    }
                }
            } else {
                std::complex<M::Complex::T> stdBase(base.real(), base.imag());
                std::complex<M::Complex::T> stdExponent(exponent.real(), exponent.imag());

                std::complex<Complex::T> stdResult = std::pow(stdBase, stdExponent);
                result = toComplex(stdResult);
            }
        }

        return result;
    }


    Complex pow(const Variant& base, Integer exponent) {
        ValueType baseType = base.valueType();

        switch (baseType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(baseType, ValueType::COMPLEX);
                return M::Complex(std::numeric_limits<Real>::quiet_NaN(), std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return pow(base.toInteger(), exponent);
            }

            case ValueType::REAL: {
                return pow(base.toReal(), exponent);
            }

            case M::ValueType::COMPLEX: {
                return pow(base.toComplex(), exponent);
            }

            default: {
                assert(false);
                return Variant();
            }
        }
    }


    Complex pow(const Variant& base, Real exponent) {
        ValueType baseType = base.valueType();

        switch (baseType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(baseType, ValueType::COMPLEX);
                return M::Complex(std::numeric_limits<Real>::quiet_NaN(), std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return pow(base.toInteger(), exponent);
            }

            case ValueType::REAL: {
                return pow(base.toReal(), exponent);
            }

            case M::ValueType::COMPLEX: {
                return pow(base.toComplex(), exponent);
            }

            default: {
                assert(false);
                return Variant();
            }
        }
    }


    Complex pow(const Variant& base, const Complex& exponent) {
        ValueType baseType = base.valueType();

        switch (baseType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(baseType, ValueType::COMPLEX);
                return M::Complex(std::numeric_limits<Real>::quiet_NaN(), std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return pow(base.toInteger(), exponent);
            }

            case ValueType::REAL: {
                return pow(base.toReal(), exponent);
            }

            case M::ValueType::COMPLEX: {
                return pow(base.toComplex(), exponent);
            }

            default: {
                assert(false);
                return Variant();
            }
        }
    }


    Complex pow(Integer base, const Variant& exponent) {
        ValueType exponentType = exponent.valueType();

        switch (exponentType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(exponentType, ValueType::COMPLEX);
                return M::Complex(std::numeric_limits<Real>::quiet_NaN(), std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return pow(base, exponent.toInteger());
            }

            case ValueType::REAL: {
                return pow(base, exponent.toReal());
            }

            case M::ValueType::COMPLEX: {
                return pow(base, exponent.toComplex());
            }

            default: {
                assert(false);
                return Variant();
            }
        }
    }


    Complex pow(Real base, const Variant& exponent) {
        ValueType exponentType = exponent.valueType();

        switch (exponentType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(exponentType, ValueType::COMPLEX);
                return M::Complex(std::numeric_limits<Real>::quiet_NaN(), std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return pow(base, exponent.toInteger());
            }

            case ValueType::REAL: {
                return pow(base, exponent.toReal());
            }

            case M::ValueType::COMPLEX: {
                return pow(base, exponent.toComplex());
            }

            default: {
                assert(false);
                return Variant();
            }
        }
    }


    Complex pow(const Complex& base, const Variant& exponent) {
        ValueType exponentType = exponent.valueType();

        switch (exponentType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(exponentType, ValueType::COMPLEX);
                return M::Complex(std::numeric_limits<Real>::quiet_NaN(), std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return pow(base, exponent.toInteger());
            }

            case ValueType::REAL: {
                return pow(base, exponent.toReal());
            }

            case M::ValueType::COMPLEX: {
                return pow(base, exponent.toComplex());
            }

            default: {
                assert(false);
                return Variant();
            }
        }
    }


    Complex pow(const Variant& base, const Variant& exponent) {
        ValueType baseType = base.valueType();

        switch (baseType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(baseType, ValueType::COMPLEX);
                return M::Complex(std::numeric_limits<Real>::quiet_NaN(), std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return pow(base.toInteger(), exponent);
            }

            case ValueType::REAL: {
                return pow(base.toReal(), exponent);
            }

            case M::ValueType::COMPLEX: {
                return pow(base.toComplex(), exponent);
            }

            default: {
                assert(false);
                return Variant();
            }
        }
    }


    Complex sqrt(Integer value) {
        std::complex<M::Complex::T> stdValue(static_cast<M::Complex::T>(value), 0);
        std::complex<M::Complex::T> result = std::sqrt(stdValue);
        return toComplex(result);
    }


    Complex sqrt(Real value) {
        Complex result;

        if (std::isinf(value)) {
            if (value > 0) {
                result = Complex(+infinity, 0);
            } else {
                result = Complex(0, +infinity);
            }
        } else if (std::isnan(value)) {
            result = Complex(NaN, 0);
        } else {
            result = toComplex(std::sqrt(std::complex<M::Complex::T>(value, 0)));
        }

        return result;
    }


    Complex sqrt(const Complex& value) {
        Complex result;

        if (std::isnan(value.real())) {
            if (value.imag() == 0) {
                result = Complex(NaN, 0);
            } else {
                result = Complex(NaN, NaN);
            }
        } else if (std::isnan(value.imag()) /* && !std::isnan(value.real())) */) {
            result = Complex(NaN, NaN);
        } else if (std::isinf(value.real())) {
            if (std::isinf(value.imag())) {
                result = Complex(NaN, NaN);
            } else {
                if (value.real() > 0) {
                    result = Complex(+infinity, 0);
                } else {
                    result = Complex(0, +infinity);
                }
            }
        } else if (std::isinf(value.imag())) {
            if (value.imag() > 0) {
                result = Complex(+infinity, +infinity);
            } else {
                result = Complex(+infinity, -infinity);
            }
        } else {
            std::complex<M::Complex::T> stdValue(toStdComplex(value));
            result = toComplex(std::sqrt(stdValue));
        }

        return result;
    }


    Complex sqrt(const Variant& value) {
        ValueType valueType = value.valueType();

        switch (valueType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(valueType, ValueType::COMPLEX);
                return M::Complex(std::numeric_limits<Real>::quiet_NaN(), std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return sqrt(value.toInteger());
            }

            case ValueType::REAL: {
                return sqrt(value.toReal());
            }

            case M::ValueType::COMPLEX: {
                return sqrt(value.toComplex());
            }

            default: {
                assert(false);
                return Variant();
            }
        }
    }


    Complex nroot(Integer value, Integer root) {
        return pow(value, 1.0 / Real(root));
    }


    Complex nroot(Integer value, Real root) {
        return pow(value, 1.0 / root);
    }

    Complex nroot(Integer value, const Complex& root) {
        return pow(value, resiprocal(root));
    }


    Complex nroot(Real value, Integer root) {
        return pow(value, 1.0 / Real(root));
    }


    Complex nroot(Real value, Real root) {
        return pow(value, 1.0 / root);
    }


    Complex nroot(Real value, const Complex& root) {
        return pow(value, resiprocal(root));
    }


    Complex nroot(const Complex& value, Integer root) {
        return pow(value, 1.0 / Real(root));
    }


    Complex nroot(const Complex& value, Real root) {
        return pow(value, 1.0 / root);
    }


    Complex nroot(const Complex& value, const Complex& root) {
        return pow(value, resiprocal(root));
    }


    Complex nroot(const Variant& value, Integer root) {
        ValueType valueType = value.valueType();

        switch (valueType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(valueType, ValueType::COMPLEX);
                return M::Complex(std::numeric_limits<Real>::quiet_NaN(), std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return nroot(value.toInteger(), root);
            }

            case ValueType::REAL: {
                return nroot(value.toReal(), root);
            }

            case M::ValueType::COMPLEX: {
                return nroot(value.toComplex(), root);
            }

            default: {
                assert(false);
                return Variant();
            }
        }
    }


    Complex nroot(const Variant& value, Real root) {
        ValueType valueType = value.valueType();

        switch (valueType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(valueType, ValueType::COMPLEX);
                return M::Complex(std::numeric_limits<Real>::quiet_NaN(), std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return nroot(value.toInteger(), root);
            }

            case ValueType::REAL: {
                return nroot(value.toReal(), root);
            }

            case M::ValueType::COMPLEX: {
                return nroot(value.toComplex(), root);
            }

            default: {
                assert(false);
                return Variant();
            }
        }
    }


    Complex nroot(const Variant& value, const Complex& root) {
        ValueType valueType = value.valueType();

        switch (valueType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(valueType, ValueType::COMPLEX);
                return M::Complex(std::numeric_limits<Real>::quiet_NaN(), std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return nroot(value.toInteger(), root);
            }

            case ValueType::REAL: {
                return nroot(value.toReal(), root);
            }

            case M::ValueType::COMPLEX: {
                return nroot(value.toComplex(), root);
            }

            default: {
                assert(false);
                return Variant();
            }
        }
    }


    Complex nroot(Integer value, const Variant& root) {
        ValueType rootType = root.valueType();

        switch (rootType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(rootType, ValueType::COMPLEX);
                return M::Complex(std::numeric_limits<Real>::quiet_NaN(), std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return nroot(value, root.toInteger());
            }

            case ValueType::REAL: {
                return nroot(value, root.toReal());
            }

            case M::ValueType::COMPLEX: {
                return nroot(value, root.toComplex());
            }

            default: {
                assert(false);
                return Variant();
            }
        }
    }


    Complex nroot(Real value, const Variant& root) {
        ValueType rootType = root.valueType();

        switch (rootType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(rootType, ValueType::COMPLEX);
                return M::Complex(std::numeric_limits<Real>::quiet_NaN(), std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return nroot(value, root.toInteger());
            }

            case ValueType::REAL: {
                return nroot(value, root.toReal());
            }

            case M::ValueType::COMPLEX: {
                return nroot(value, root.toComplex());
            }

            default: {
                assert(false);
                return Variant();
            }
        }
    }


    Complex nroot(const Complex& value, const Variant& root) {
        ValueType rootType = root.valueType();

        switch (rootType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(rootType, ValueType::COMPLEX);
                return M::Complex(std::numeric_limits<Real>::quiet_NaN(), std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return nroot(value, root.toInteger());
            }

            case ValueType::REAL: {
                return nroot(value, root.toReal());
            }

            case M::ValueType::COMPLEX: {
                return nroot(value, root.toComplex());
            }

            default: {
                assert(false);
                return Variant();
            }
        }
    }


    Complex nroot(const Variant& value, const Variant& root) {
        ValueType valueType = value.valueType();

        switch (valueType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(valueType, ValueType::COMPLEX);
                return M::Complex(std::numeric_limits<Real>::quiet_NaN(), std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return nroot(value.toInteger(), root);
            }

            case ValueType::REAL: {
                return nroot(value.toReal(), root);
            }

            case M::ValueType::COMPLEX: {
                return nroot(value.toComplex(), root);
            }

            default: {
                assert(false);
                return Variant();
            }
        }
    }


    Real abs(const Complex& value) {
        Real result;

        Real r = value.real();
        Real i = value.imag();

        if (i == Real(0)) {
            result = abs(r);
        } else {
            if (r == Real(0)) {
                result = abs(i);
            } else {
                result = std::sqrt(r*r + i*i);
            }
        }

        return result;
    }


    Real floor(Real value) {
        return std::floor(value);
    }


    Real floor(const Complex& value) {
        return std::floor(inlineToReal(value));
    }


    Real floor(const Variant& value) {
        return std::floor(value.toReal());
    }


    Real ceil(Real value) {
        return std::ceil(value);
    }


    Real ceil(const Complex& value) {
        return std::ceil(inlineToReal(value));
    }


    Real ceil(const Variant& value) {
        return std::ceil(value.toReal());
    }


    Real nint(Real value) {
        Real result;

        if (std::isinf(value) || std::isnan(value)) {
            result = value;
        } else {
            Real fractional = std::modf(value, &result);

            if        (fractional > Real(0.5)                           ||
                       (fractional == Real(0.5)                    &&
                        std::trunc(result / 2.0) != (result / 2.0)    )    ) {
                result += Real(1);
            } else if (fractional < Real(-0.5)                          ||
                       (fractional == Real(-0.5)                   &&
                        std::trunc(result / 2.0) != (result / 2.0)    )    ) {
                result -= Real(1);
            }
        }

        return result;
    }


    Real nint(const Complex& value) {
        return nint(inlineToReal(value));
    }


    Real nint(const Variant& value) {
        return nint(value.toReal());
    }


    Real factorial(Integer value) {
        Real result;

        if (value < 0) {
            result = NaN;
        } else {
            std::vector<Real>::size_type index = static_cast<std::vector<Real>::size_type>(value);
            result = index < factorialTable.size() ? factorialTable.at(index) : +infinity;
        }

        return result;
    }


    Real factorial(Real value) {
        Real result;

        Real dummyValue;
        if (std::isnan(value) || value < 0 || modf(value, &dummyValue) != 0) {
            result = NaN;
        } else if (value > std::numeric_limits<std::vector<Real>::size_type>::max()) {
            result = +infinity;
        } else {
            std::vector<Real>::size_type index = static_cast<std::vector<Real>::size_type>(value);
            result = index < factorialTable.size() ? factorialTable.at(index) : +infinity;
        }

        return result;
    }


    Real factorial(const Complex& value) {
        return factorial(inlineToReal(value));
    }


    Real factorial(const Variant& value) {
        return factorial(value.toReal());
    }


    Real gamma(Integer value) {
        Real result;

        if (value > 0) {
            Integer i = value - 1;
            result = static_cast<unsigned long>(i) < factorialTable.size() ? factorialTable.at(i) : +infinity;
        } else {
            result = gamma(static_cast<Real>(value));
        }

        return result;
    }


    Real gamma(Real value) {
        Real result;

        if (value < 0.5) {
            // Lanczos approximation is not valid for values less than 0.5 so we use the reflection formula below
            // to keep the values accurate over the appropriate range.

            result = pi / (std::sin(pi * value) * lanczosApproximation(1.0 - value));
        } else {
            Real wholePart;
            if (value > 0 && std::modf(value, &wholePart) == 0) {
                result = factorial(value - 1.0);
            } else {
                result = lanczosApproximation(value);
            }
        }

        return result;
    }


    Complex gamma(const Complex& value) {
        Complex result;

        Real realPart = value.real();
        if (realPart < 0.5) {
            // Lanczos approximation is not valid for real values less than 0.5 so we use the reflection formula below
            // to keep the values accurate over the appropriate range.

            result = pi / (toComplex(std::sin(toStdComplex(pi * value))) * lanczosApproximation(1.0 - value));
        } else {
            Real wholePart;
            if (realPart > 0 && value.imag() == 0 && std::modf(realPart, &wholePart) == 0) {
                result = Complex(factorial(realPart - 1));
            } else {
                result = lanczosApproximation(value);
            }
        }

        if (value.imag() == 0 && result.imag() != 0) {
            result = M::Complex(result.real());
        }

        return result;
    }


    Complex gamma(const Variant& value) {
        ValueType valueType = value.valueType();

        switch (valueType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(valueType, ValueType::COMPLEX);
                return M::Complex(std::numeric_limits<Real>::quiet_NaN(), std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return gamma(value.toInteger());
            }

            case ValueType::REAL: {
                return gamma(value.toReal());
            }

            case M::ValueType::COMPLEX: {
                return gamma(value.toComplex());
            }

            default: {
                assert(false);
                return Variant();
            }
        }
    }


    Complex ln(Integer value) {
        return toComplex(std::log(std::complex<Complex::T>(value)));
    }


    Complex ln(Real value) {
        return toComplex(std::log(std::complex<Complex::T>(value)));
    }


    Complex ln(const Complex& value) {
        return toComplex(std::log(toStdComplex(value)));
    }


    Complex ln(const Variant& value) {
        ValueType valueType = value.valueType();

        switch (valueType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(valueType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return ln(value.toInteger());
            }

            case ValueType::REAL: {
                return ln(value.toReal());
            }

            case M::ValueType::COMPLEX: {
                return ln(value.toComplex());
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex log(Integer base, Integer value) {
        Complex result;

        if (value > 0 && base > 0) {
            if (base == 2) {
                result = std::log2(static_cast<Real>(value));
            } else if (base == 10) {
                result = std::log10(static_cast<Real>(value));
            } else {
                result = std::log(static_cast<Real>(value)) / std::log(static_cast<Real>(base));
            }
        } else {
            result = ln(value) / std::log(static_cast<Real>(base));
        }

        return result;
    }


    Complex log(Integer base, Real value) {
        Complex result;

        if (value > 0 && base > 0) {
            if (base == 2) {
                result = std::log2(value);
            } else if (base == 10) {
                result = std::log10(value);
            } else {
                result = std::log(value) / std::log(static_cast<Real>(base));
            }
        } else {
            result = ln(value) / ln(base);
        }

        return result;
    }


    Complex log(Integer base, const Complex& value) {
        return ln(value) / ln(base);
    }


    Complex log(Real base, Integer value) {
        Complex result;

        if (value > 0 && base > 0) {
            if (base == 2.0) {
                result = std::log2(value);
            } else if (base == e) {
                result = std::log(value);
            } else if (base == 10.0) {
                result = std::log10(value);
            } else {
                result = std::log(value) / std::log(base);
            }
        } else {
            result = ln(value) / ln(base);
        }

        return result;
    }


    Complex log(Real base, Real value) {
        Complex result;

        if (value > 0 && base > 0) {
            if (base == 2.0) {
                result = std::log2(value);
            } else if (base == e) {
                result = std::log(value);
            } else if (base == 10.0) {
                result = std::log10(value);
            } else {
                result = std::log(value) / std::log(base);
            }
        } else {
            result = ln(value) / ln(base);
        }

        return result;
    }


    Complex log(Real base, const Complex& value) {
        return ln(value) / ln(base);
    }


    Complex log(const Variant& base, Integer value) {
        ValueType baseType = base.valueType();

        switch (baseType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(baseType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return log(base.toInteger(), value);
            }

            case ValueType::REAL: {
                return log(base.toReal(), value);
            }

            case M::ValueType::COMPLEX: {
                return log(base.toComplex(), value);
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex log(const Variant& base, Real value) {
        ValueType baseType = base.valueType();

        switch (baseType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(baseType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return log(base.toInteger(), value);
            }

            case ValueType::REAL: {
                return log(base.toReal(), value);
            }

            case M::ValueType::COMPLEX: {
                return log(base.toComplex(), value);
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex log(const Variant& base, const Complex& value) {
        ValueType baseType = base.valueType();

        switch (baseType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(baseType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return log(base.toInteger(), value);
            }

            case ValueType::REAL: {
                return log(base.toReal(), value);
            }

            case M::ValueType::COMPLEX: {
                return log(base.toComplex(), value);
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex log(Integer base, const Variant& value) {
        ValueType valueType = value.valueType();

        switch (valueType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(valueType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return log(base, value.toInteger());
            }

            case ValueType::REAL: {
                return log(base, value.toReal());
            }

            case M::ValueType::COMPLEX: {
                return log(base, value.toComplex());
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex log(Real base, const Variant& value) {
        ValueType valueType = value.valueType();

        switch (valueType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(valueType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return log(base, value.toInteger());
            }

            case ValueType::REAL: {
                return log(base, value.toReal());
            }

            case M::ValueType::COMPLEX: {
                return log(base, value.toComplex());
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex log(const Complex& base, const Variant& value) {
        ValueType valueType = value.valueType();

        switch (valueType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(valueType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return log(base, value.toInteger());
            }

            case ValueType::REAL: {
                return log(base, value.toReal());
            }

            case M::ValueType::COMPLEX: {
                return log(base, value.toComplex());
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex log(const Variant& base, const Variant& value) {
        ValueType baseType = base.valueType();

        switch (baseType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(baseType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return log(base.toInteger(), value);
            }

            case ValueType::REAL: {
                return log(base.toReal(), value);
            }

            case M::ValueType::COMPLEX: {
                return log(base.toComplex(), value);
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Real lnFactorial(Integer value) {
        Real result;

        if (value < 0) {
            result = NaN;
        } else {
            if (static_cast<unsigned long>(value) < logFactorialTable.size()) {
                result = logFactorialTable.at(value);
            } else {
                // Note: first 6 terms of Sterling's approximation.
                //
                // The last term only provides a very small reduction in error compared to sum of logs approach so its
                // inclusion is questionable.

                Real x  = static_cast<Real>(value);
                Real x2 = x * x;
                Real x3 = x2 * x;
                Real x5 = x2 * x3;
                result = (
                      x * std::log(x)
                    - x
                    + 0.5 * std::log(2 * pi * x)
                    + 1.0 / (12.0 * x)
                    - 1.0 / (360.0 * x3)
                    + 1.0 / (1260.0 * x5)
                );
            }
        }

        return result;
    }


    Real lnFactorial(Real value) {
        Real result;

        Real dummyValue;
        if (std::isnan(value) || value < 0 || modf(value, &dummyValue) != 0) {
            result = NaN;
        } else {
            result = lnFactorial(static_cast<Integer>(value));
        }

        return result;
    }


    Real lnFactorial(const Complex& value) {
        Real result;

        Real dummyValue;
        if (std::isnan(value.real())                  ||
            std::isnan(value.imag())                  ||
            value.real() < 0                          ||
            value.imag() != 0                         ||
            std::modf(value.real(), &dummyValue) != 0    ) {
            result = NaN;
        } else {
            result = lnFactorial(static_cast<Integer>(value.real()));
        }

        return result;
    }


    Real lnGamma(Integer value) {
        Real result;

        if (value > 0) {
            result = lnFactorial(value - 1);
        } else {
            result = lnGamma(static_cast<Real>(value));
        }

        return result;
    }


    Real lnGamma(Real value) {
        Real result;

        double dummy;
        if (value >= 1 && std::modf(value, &dummy) == 0) {
            result = lnFactorial(value - 1);
        } else if (value < 0.5) {
            Real reflected = lanczosApproximation(1.0 - value);
            if (std::isinf(reflected)) {
                result = std::log(pi) - std::log(std::sin(pi * value)) - sterlingsApproximation(1.0 - value);
            } else {
                result = std::log(pi / (std::sin(pi * value) * reflected));
            }
        } else {
            if (value <= 142.0) {
                Real g = lanczosApproximation(value);
                if (std::isinf(g)) {
                    result = sterlingsApproximation(value);
                } else {
                    result = std::log(g);
                }
            } else {
                result = sterlingsApproximation(value);
            }
        }

        return result;
    }


    Complex lnGamma(const Complex& value) {
        Complex result;

        double dummy;
        if (value.real() >= 1 && value.imag() == 0 && std::modf(value.real(), &dummy) == 0) {
            result = lnFactorial(value - 1);
        } else if (value.real() < 0.5) {
            Complex reflected = lanczosApproximation(1.0 - value);
            if (std::isinf(reflected.real()) || std::isinf(reflected.imag()) || std::isnan(reflected.imag())) {
                result = (
                      std::log(pi)
                    - toComplex(std::log(std::sin(toStdComplex(pi * value))))
                    - sterlingsApproximation(1.0 - value)
                );
            } else {
                result = toComplex(std::log(pi / (std::sin(pi * toStdComplex(value)) * toStdComplex(reflected))));
            }
        } else {
            if (value.real() <= 142.0) {
                Complex g = lanczosApproximation(value);
                if (std::isinf(g.real()) || std::isinf(g.imag())) {
                    result = sterlingsApproximation(value);
                } else {
                    result = toComplex(std::log(toStdComplex(g)));
                }
            } else {
                result = sterlingsApproximation(value);
            }
        }

        return result;
    }


    Complex lnGamma(const Variant& value) {
        ValueType valueType = value.valueType();

        switch (valueType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(valueType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return lnGamma(value.toInteger());
            }

            case ValueType::REAL: {
                return lnGamma(value.toReal());
            }

            case M::ValueType::COMPLEX: {
                return lnGamma(value.toComplex());
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Real binomial(Integer n, Integer k) {
        Real result;

        if (n < 0 || k < 0 || k > n) {
            result = NaN;
        } else {
            Integer firstLogFactorial = static_cast<Integer>(factorialTable.size());
            if (n < firstLogFactorial && k < firstLogFactorial) {
                result = (factorialTable.at(n) / factorialTable.at(k)) / factorialTable.at(n - k);
            } else {
                Real lnBinomial = lnFactorial(n) - lnFactorial(k) - lnFactorial(n - k);
                result = std::exp(lnBinomial);
            }
        }

        return result;
    }


    Real binomial(Integer n, Real k) {
        Real result;

        if (n < 0 || k < 0 || k > n) {
            result = NaN;
        } else {
            Real wholePart;
            if (std::modf(k, &wholePart) != 0) {
                result = NaN;
            } else {
                Integer firstLogFactorial = static_cast<Integer>(factorialTable.size());
                if (n < firstLogFactorial && k < firstLogFactorial) {
                    Integer ki = static_cast<Integer>(k);
                    result = (factorialTable.at(n) / factorialTable.at(ki)) / factorialTable.at(n - ki);
                } else {
                    Real lnBinomial = lnFactorial(n) - lnFactorial(k) - lnFactorial(n - k);
                    result = std::exp(lnBinomial);
                }
            }
        }

        return result;
    }


    Real binomial(Real n, Integer k) {
        Real result;

        if (n < 0 || k < 0 || k > n) {
            result = NaN;
        } else {
            Real wholePart;
            if (std::modf(n, &wholePart) != 0) {
                result = NaN;
            } else {
                Integer firstLogFactorial = static_cast<Integer>(factorialTable.size());
                if (n < firstLogFactorial && k < firstLogFactorial) {
                    Integer ni = static_cast<Integer>(n);
                    result = (factorialTable.at(ni) / factorialTable.at(k)) / factorialTable.at(ni - k);
                } else {
                    Real lnBinomial = lnFactorial(n) - lnFactorial(k) - lnFactorial(n - k);
                    result = std::exp(lnBinomial);
                }
            }
        }

        return result;
    }


    Real binomial(Real n, Real k) {
        Real result;

        if (n < 0 || k < 0 || k > n) {
            result = NaN;
        } else {
            Real wholePart;
            if (std::modf(n, &wholePart) != 0 || std::modf(k, &wholePart) != 0) {
                result = NaN;
            } else {
                Integer firstLogFactorial = static_cast<Integer>(factorialTable.size());
                if (n < firstLogFactorial && k < firstLogFactorial) {
                    Integer ni = static_cast<Integer>(n);
                    Integer ki = static_cast<Integer>(k);
                    result = (factorialTable.at(ni) / factorialTable.at(ki)) / factorialTable.at(ni - ki);
                } else {
                    Real lnBinomial = lnFactorial(n) - lnFactorial(k) - lnFactorial(n - k);
                    result = std::exp(lnBinomial);
                }
            }
        }

        return result;
    }


    Real binomial(Integer n, const Complex& k) {
        Real result;

        if (k.imag() != 0) {
            result = NaN;
        } else {
            result = binomial(n, k.real());
        }

        return result;
    }


    Real binomial(Real n, const Complex& k) {
        Real result;

        if (k.imag() != 0) {
            result = NaN;
        } else {
            result = binomial(n, k.real());
        }

        return result;
    }


    Real binomial(const Complex& n, Integer k) {
        Real result;

        if (n.imag() != 0) {
            result = NaN;
        } else {
            result = binomial(n.real(), k);
        }

        return result;
    }


    Real binomial(const Complex& n, Real k) {
        Real result;

        if (n.imag() != 0) {
            result = NaN;
        } else {
            result = binomial(n.real(), k);
        }

        return result;
    }


    Real binomial(const Complex& n, const Complex& k) {
        Real result;

        if (n.imag() != 0 || k.imag() != 0) {
            result = NaN;
        } else {
            result = binomial(n.real(), k.real());
        }

        return result;
    }


    Real binomial(const Variant& n, Integer k) {
        ValueType nType = n.valueType();

        switch (nType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(nType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return binomial(n.toInteger(), k);
            }

            case ValueType::REAL: {
                return binomial(n.toReal(), k);
            }

            case M::ValueType::COMPLEX: {
                return binomial(n.toComplex(), k);
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Real binomial(const Variant& n, Real k) {
        ValueType nType = n.valueType();

        switch (nType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(nType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return binomial(n.toInteger(), k);
            }

            case ValueType::REAL: {
                return binomial(n.toReal(), k);
            }

            case M::ValueType::COMPLEX: {
                return binomial(n.toComplex(), k);
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Real binomial(const Variant& n, const Complex& k) {
        ValueType nType = n.valueType();

        switch (nType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(nType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return binomial(n.toInteger(), k);
            }

            case ValueType::REAL: {
                return binomial(n.toReal(), k);
            }

            case M::ValueType::COMPLEX: {
                return binomial(n.toComplex(), k);
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Real binomial(Integer n, const Variant& k) {
        ValueType kType = k.valueType();

        switch (kType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(kType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return binomial(n, k.toInteger());
            }

            case ValueType::REAL: {
                return binomial(n, k.toReal());
            }

            case M::ValueType::COMPLEX: {
                return binomial(n, k.toComplex());
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Real binomial(Real n, const Variant& k) {
        ValueType kType = k.valueType();

        switch (kType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(kType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return binomial(n, k.toInteger());
            }

            case ValueType::REAL: {
                return binomial(n, k.toReal());
            }

            case M::ValueType::COMPLEX: {
                return binomial(n, k.toComplex());
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Real binomial(const Complex& n, const Variant& k) {
        ValueType kType = k.valueType();

        switch (kType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(kType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return binomial(n, k.toInteger());
            }

            case ValueType::REAL: {
                return binomial(n, k.toReal());
            }

            case M::ValueType::COMPLEX: {
                return binomial(n, k.toComplex());
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Real binomial(const Variant& n, const Variant& k) {
        ValueType nType = n.valueType();

        switch (nType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(nType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return binomial(n.toInteger(), k);
            }

            case ValueType::REAL: {
                return binomial(n.toReal(), k);
            }

            case M::ValueType::COMPLEX: {
                return binomial(n.toComplex(), k);
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Integer unsignedSterlingNumber1(Integer n, Integer k) {
        Integer result;

        if (n <= 0) {
            if (k <= 0) {
                result = 1;
            } else {
                result = 0;
            }
        } else if (k == 0) {
            result = 0;
        } else {
            result = (n - 1) * unsignedSterlingNumber1(n - 1, k) + unsignedSterlingNumber1(n - 1, k - 1);
        }

        return result;
    }

    Real unsignedSterlingNumber1(Integer n, Real k) {
        Real result;

        Real wholeValue;
        if (std::modf(k, &wholeValue) == 0) {
            result = unsignedSterlingNumber1Engine(static_cast<Real>(n), k);
        } else{
            result = NaN;
        }

        return result;
    }


    Real unsignedSterlingNumber1(Integer n, const Complex& k) {
        return (k.imag() == 0 ? unsignedSterlingNumber1(n, k.real()) : NaN);
    }


    Real unsignedSterlingNumber1(Real n, Integer k) {
        Real result;

        Real wholeValue;
        if (std::modf(n, &wholeValue) == 0) {
            result = unsignedSterlingNumber1Engine(n, static_cast<Real>(k));
        } else{
            result = NaN;
        }

        return result;
    }


    Real unsignedSterlingNumber1(Real n, Real k) {
        Real result;

        Real wholeValue;
        if (std::modf(n, &wholeValue) == 0 && std::modf(k, &wholeValue) == 0) {
            result = unsignedSterlingNumber1Engine(n, k);
        } else{
            result = NaN;
        }

        return result;
    }


    Real unsignedSterlingNumber1(Real n, const Complex& k) {
        return (k.imag() == 0 ? unsignedSterlingNumber1(n, k.real()) : NaN);
    }


    Real unsignedSterlingNumber1(const Complex& n, Integer k) {
        return (n.imag() == 0 ? unsignedSterlingNumber1(n.real(), k) : NaN);
    }


    Real unsignedSterlingNumber1(const Complex& n, Real k) {
        return (n.imag() == 0 ? unsignedSterlingNumber1(n.real(), k) : NaN);
    }


    Real unsignedSterlingNumber1(const Complex& n, const Complex& k) {
        return (n.imag() == 0 ? unsignedSterlingNumber1(n.real(), k) : NaN);
    }


    Real sterlingNumber2(Integer n, Integer k) {
        Real result;

        if (n >= 0 && k >= 0) {
            Real sum = 0;
            for (unsigned i=0 ; i<=k ; ++i) {
                Real term = (
                      ((i & 1) == 0 ? +1 : -1)
                    * binomial(k, static_cast<Integer>(i))
                    * std::pow(k - i, n)
                );

                sum += term;
            }

            result = std::round(std::exp(std::log(sum) - lnFactorial(k)));
        } else {
            result = 0;
        }

        return result;
    }


    Real sterlingNumber2(Integer n, Real k) {
        Real result;

        Real wholeValue;
        if (std::modf(k, &wholeValue) == 0) {
            result = sterlingNumber2(n, static_cast<Integer>(k));
        } else {
            result = NaN;
        }

        return result;
    }


    Real sterlingNumber2(Integer n, const Complex& k) {
        Real result;

        if (k.imag() == 0) {
            result = sterlingNumber2(n, k.real());
        } else {
            result = NaN;
        }

        return result;
    }


    Real sterlingNumber2(Real n, Integer k) {
        Real result;

        Real wholeValue;
        if (std::modf(n, &wholeValue) == 0) {
            result = sterlingNumber2(static_cast<Integer>(n), k);
        } else {
            result = NaN;
        }

        return result;
    }


    Real sterlingNumber2(Real n, Real k) {
        Real result;

        Real wholeValue;
        if (std::modf(n, &wholeValue) == 0 && std::modf(k, &wholeValue) == 0) {
            result = sterlingNumber2(static_cast<Integer>(n), static_cast<Integer>(k));
        } else {
            result = NaN;
        }

        return result;
    }


    Real sterlingNumber2(Real n, const Complex& k) {
        Real result;

        Real wholeValue;
        if (std::modf(n, &wholeValue) == 0 && k.imag() == 0) {
            result = sterlingNumber2(static_cast<Integer>(n), k.real());
        } else {
            result = NaN;
        }

        return result;
    }


    Real sterlingNumber2(const Complex& n, Integer k) {
        Real result;

        if (n.imag() == 0) {
            result = sterlingNumber2(n.real(), k);
        } else {
            result = NaN;
        }

        return result;
    }


    Real sterlingNumber2(const Complex& n, Real k) {
        Real result;

        if (n.imag() == 0) {
            result = sterlingNumber2(n.real(), k);
        } else {
            result = NaN;
        }

        return result;
    }


    Real sterlingNumber2(const Complex& n, const Complex& k) {
        Real result;

        if (n.imag() == 0) {
            result = sterlingNumber2(n.real(), k);
        } else {
            result = NaN;
        }

        return result;
    }


    Real lowerGamma(Integer s, Integer x) {
        return lowerGamma(static_cast<Real>(s), static_cast<Real>(x));
    }


    Real lowerGamma(Integer s, Real x) {
        return lowerGamma(static_cast<Real>(s), x);
    }


    Complex lowerGamma(Integer s, const Complex& x) {
        return lowerGamma(Complex(s), x);
    }


    Real lowerGamma(Real s, Integer x) {
        return lowerGamma(s, static_cast<Real>(x));
    }


    Real lowerGamma(Real s, Real x) {
        Real result;

        if (s > 0 && x >= 0) {
            if (s >= alphaz(x)) {
                // Based on some experimentation, I've found that the simpler lowerRegularizedGammaEngine1 method
                // is more accurate for s < 73.  Above about 65, the lowerRegularizedGammaEngine2 has similar accuracy
                // or better accuracy.  For values less than s < 65, version 2 is slightly less accurate.  The
                // threshold does vary somewhat based on the value of x but generally improves the behaviors rather
                // than worstens them.
                //
                // Based on the empirical measurements, we've set a threshold at 70 which should select the best fit
                // method with little additional overhead.

                if (s < 70 || x == 0) {
                    Real gammaS = gamma(s);
                    Real p = lowerRegularizedGammaEngine1(s, x, gammaS);

                    result = p * gammaS;
                } else {
                    Real lnGammaS = lnGamma(s);
                    Real p        = lowerRegularizedGammaEngine2(s, x, lnGammaS);

                    result = std::exp(std::log(p) + lnGammaS);
                }
            } else {
                result = gamma(s) - upperIncompleteGammaEngine(s, x);
            }
        } else {
            result = NaN;
        }

        return result;
    }


    Complex lowerGamma(Real s, const Complex& x) {
        return lowerGamma(Complex(s), x);
    }


    Complex lowerGamma(const Complex& s, Integer x) {
        return lowerGamma(s, Complex(x));
    }


    Complex lowerGamma(const Complex& s, Real x) {
        return lowerGamma(s, Complex(x));
    }


    Complex lowerGamma(const Complex& s, const Complex& x) {
        // See the write-up on the real version of this function for notes.

        Complex result;

        if (s.real() > 0 && x.real() >= 0) {
            if (s.real() >= alphaz(x.real())) {
                if (s.real() < 70 || x.real() == 0) {
                    Complex gammaS = gamma(s);
                    Complex p = lowerRegularizedGammaEngine1(s, x, gammaS);

                    result = p * gammaS;
                } else {
                    Complex lnGammaS = lnGamma(s);
                    Complex p        = lowerRegularizedGammaEngine2(s, x, lnGammaS);

                    result = exp(ln(p) + lnGammaS);
                }
            } else {
                result = gamma(s) - upperIncompleteGammaEngine(s, x);
            }
        } else {
            result = NaN;
        }

        return result;
    }


    Complex lowerGamma(const Variant& s, Integer z) {
        ValueType sType = s.valueType();

        switch (sType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(sType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return lowerGamma(s.toInteger(), z);
            }

            case ValueType::REAL: {
                return lowerGamma(s.toReal(), z);
            }

            case M::ValueType::COMPLEX: {
                return lowerGamma(s.toComplex(), z);
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex lowerGamma(const Variant& s, Real z) {
        ValueType sType = s.valueType();

        switch (sType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(sType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return lowerGamma(s.toInteger(), z);
            }

            case ValueType::REAL: {
                return lowerGamma(s.toReal(), z);
            }

            case M::ValueType::COMPLEX: {
                return lowerGamma(s.toComplex(), z);
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex lowerGamma(const Variant& s, const Complex& z) {
        ValueType sType = s.valueType();

        switch (sType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(sType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return lowerGamma(s.toInteger(), z);
            }

            case ValueType::REAL: {
                return lowerGamma(s.toReal(), z);
            }

            case M::ValueType::COMPLEX: {
                return lowerGamma(s.toComplex(), z);
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex lowerGamma(Integer s, const Variant& z) {
        ValueType zType = z.valueType();

        switch (zType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(zType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return lowerGamma(s, z.toInteger());
            }

            case ValueType::REAL: {
                return lowerGamma(s, z.toReal());
            }

            case M::ValueType::COMPLEX: {
                return lowerGamma(s, z.toComplex());
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex lowerGamma(Real s, const Variant& z) {
        ValueType zType = z.valueType();

        switch (zType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(zType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return lowerGamma(s, z.toInteger());
            }

            case ValueType::REAL: {
                return lowerGamma(s, z.toReal());
            }

            case M::ValueType::COMPLEX: {
                return lowerGamma(s, z.toComplex());
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex lowerGamma(const Complex& s, const Variant& z) {
        ValueType zType = z.valueType();

        switch (zType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(zType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return lowerGamma(s, z.toInteger());
            }

            case ValueType::REAL: {
                return lowerGamma(s, z.toReal());
            }

            case M::ValueType::COMPLEX: {
                return lowerGamma(s, z.toComplex());
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex lowerGamma(const Variant& s, const Variant& z) {
        ValueType sType = s.valueType();

        switch (sType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(sType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return lowerGamma(s.toInteger(), z);
            }

            case ValueType::REAL: {
                return lowerGamma(s.toReal(), z);
            }

            case M::ValueType::COMPLEX: {
                return lowerGamma(s.toComplex(), z);
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Real upperGamma(Integer s, Integer x) {
        return upperGamma(static_cast<Real>(s), static_cast<Real>(x));
    }


    Real upperGamma(Integer s, Real x) {
        return upperGamma(static_cast<Real>(s), x);
    }


    Complex upperGamma(Integer s, const Complex& x) {
        return upperGamma(Complex(s), x);
    }


    Real upperGamma(Real s, Integer x) {
        return upperGamma(s, static_cast<Real>(x));
    }


    Real upperGamma(Real s, Real x) {
        // See the write-up on the real version of the lower gamma function for notes.

        Real result;

        if (s > 0 && x >= 0) {
            if (s >= alphaz(x)) {
                if (s < 70 || x == 0) {
                    Real gammaS = gamma(s);
                    Real p = lowerRegularizedGammaEngine1(s, x, gammaS);

                    result = (1.0 - p) * gammaS;
                } else {
                    Real lnGammaS = lnGamma(s);
                    Real p        = lowerRegularizedGammaEngine2(s, x, lnGammaS);

                    result = std::exp(std::log(1.0 - p) + lnGammaS);
                }
            } else {
                result = upperIncompleteGammaEngine(s, x);
            }
        } else {
            result = NaN;
        }

        return result;
    }


    Complex upperGamma(Real s, const Complex& x) {
        return upperGamma(Complex(s), x);
    }


    Complex upperGamma(const Complex& s, Integer x) {
        return upperGamma(s, Complex(x));
    }


    Complex upperGamma(const Complex& s, Real x) {
        return upperGamma(s, Complex(x));
    }


    Complex upperGamma(const Complex& s, const Complex& x) {
        // See the write-up on the real version of the lower gamma function for notes.

        Complex result;

        if (s.real() > 0 && x.real() >= 0) {
            if (s.real() >= alphaz(x.real())) {
                if (s.real() < 70 || x.real() == 0) {
                    Complex gammaS = gamma(s);
                    Complex p = lowerRegularizedGammaEngine1(s, x, gammaS);

                    result = (1.0 - p) * gammaS;
                } else {
                    Complex lnGammaS = lnGamma(s);
                    Complex p        = lowerRegularizedGammaEngine2(s, x, lnGammaS);

                    result = exp(ln(1.0 - p) + lnGammaS);
                }
            } else {
                result = upperIncompleteGammaEngine(s, x);
            }
        } else {
            result = NaN;
        }

        return result;
    }


    Complex upperGamma(const Variant& s, Integer z) {
        ValueType sType = s.valueType();

        switch (sType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(sType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return upperGamma(s.toInteger(), z);
            }

            case ValueType::REAL: {
                return upperGamma(s.toReal(), z);
            }

            case M::ValueType::COMPLEX: {
                return upperGamma(s.toComplex(), z);
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex upperGamma(const Variant& s, Real z) {
        ValueType sType = s.valueType();

        switch (sType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(sType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return upperGamma(s.toInteger(), z);
            }

            case ValueType::REAL: {
                return upperGamma(s.toReal(), z);
            }

            case M::ValueType::COMPLEX: {
                return upperGamma(s.toComplex(), z);
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex upperGamma(const Variant& s, const Complex& z) {
        ValueType sType = s.valueType();

        switch (sType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(sType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return upperGamma(s.toInteger(), z);
            }

            case ValueType::REAL: {
                return upperGamma(s.toReal(), z);
            }

            case M::ValueType::COMPLEX: {
                return upperGamma(s.toComplex(), z);
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex upperGamma(Integer s, const Variant& z) {
        ValueType zType = z.valueType();

        switch (zType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(zType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return upperGamma(s, z.toInteger());
            }

            case ValueType::REAL: {
                return upperGamma(s, z.toReal());
            }

            case M::ValueType::COMPLEX: {
                return upperGamma(s, z.toComplex());
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex upperGamma(Real s, const Variant& z) {
        ValueType zType = z.valueType();

        switch (zType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(zType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return upperGamma(s, z.toInteger());
            }

            case ValueType::REAL: {
                return upperGamma(s, z.toReal());
            }

            case M::ValueType::COMPLEX: {
                return upperGamma(s, z.toComplex());
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex upperGamma(const Complex& s, const Variant& z) {
        ValueType zType = z.valueType();

        switch (zType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(zType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return upperGamma(s, z.toInteger());
            }

            case ValueType::REAL: {
                return upperGamma(s, z.toReal());
            }

            case M::ValueType::COMPLEX: {
                return upperGamma(s, z.toComplex());
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex upperGamma(const Variant& s, const Variant& z) {
        ValueType sType = s.valueType();

        switch (sType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(sType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return upperGamma(s.toInteger(), z);
            }

            case ValueType::REAL: {
                return upperGamma(s.toReal(), z);
            }

            case M::ValueType::COMPLEX: {
                return upperGamma(s.toComplex(), z);
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Real gamma(Integer s, Integer z) {
        return upperGamma(s, z);
    }


    Real gamma(Integer s, Real z) {
        return upperGamma(s, z);
    }


    Complex gamma(Integer s, const Complex& z) {
        return upperGamma(s, z);
    }


    Real gamma(Real s, Integer z) {
        return upperGamma(s, z);
    }


    Real gamma(Real s, Real z) {
        return upperGamma(s, z);
    }


    Complex gamma(Real s, const Complex& z) {
        return upperGamma(s, z);
    }


    Complex gamma(const Complex& s, Integer z) {
        return upperGamma(s, z);
    }


    Complex gamma(const Complex& s, Real z) {
        return upperGamma(s, z);
    }


    Complex gamma(const Complex& s, const Complex& z) {
        return upperGamma(s, z);
    }


    Real normalizedLowerGamma(Integer s, Integer x) {
        return normalizedLowerGamma(static_cast<Real>(s), static_cast<Real>(x));
    }


    Real normalizedLowerGamma(Integer s, Real x) {
        return normalizedLowerGamma(static_cast<Real>(s), x);
    }


    Complex normalizedLowerGamma(Integer s, const Complex& x) {
        return normalizedLowerGamma(Complex(s), x);
    }


    Real normalizedLowerGamma(Real s, Integer x) {
        return normalizedLowerGamma(s, static_cast<Real>(x));
    }


    Real normalizedLowerGamma(Real s, Real x) {
        // See the write-up on the real version of the lower gamma function for notes.

        Real result;

        if (s > 0 && x >= 0) {
            if (s >= alphaz(x)) {
                if (s < 70 || x == 0) {
                    Real gammaS = gamma(s);
                    result = lowerRegularizedGammaEngine1(s, x, gammaS);
                } else {
                    Real lnGammaS = lnGamma(s);
                    result = lowerRegularizedGammaEngine2(s, x, lnGammaS);
                }
            } else {
                Real gammaS = gamma(s);
                result = 1 - (upperIncompleteGammaEngine(s, x) / gammaS);
            }
        } else {
            result = NaN;
        }

        return result;
    }


    Complex normalizedLowerGamma(Real s, const Complex& x) {
        return normalizedLowerGamma(Complex(s), x);
    }


    Complex normalizedLowerGamma(const Complex& s, Integer x) {
        return normalizedLowerGamma(s, Complex(x));
    }


    Complex normalizedLowerGamma(const Complex& s, Real x) {
        return normalizedLowerGamma(s, Complex(x));
    }


    Complex normalizedLowerGamma(const Complex& s, const Complex& x) {
        // See the write-up on the real version of the lower gamma function for notes.

        Complex result;

        if (s.real() > 0 && x.real() >= 0) {
            if (s.real() >= alphaz(x.real())) {
                if (s.real() < 70 || x.real() == 0) {
                    Complex gammaS = gamma(s);
                    result = lowerRegularizedGammaEngine1(s, x, gammaS);
                } else {
                    Complex lnGammaS = lnGamma(s);
                    result = lowerRegularizedGammaEngine2(s, x, lnGammaS);
                }
            } else {
                Complex gammaS = gamma(s);
                result = 1 - (upperIncompleteGammaEngine(s, x) / gammaS);
            }
        } else {
            result = NaN;
        }

        return result;
    }


    Complex normalizedLowerGamma(const Variant& s, Integer z) {
        ValueType sType = s.valueType();

        switch (sType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(sType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return normalizedLowerGamma(s.toInteger(), z);
            }

            case ValueType::REAL: {
                return normalizedLowerGamma(s.toReal(), z);
            }

            case M::ValueType::COMPLEX: {
                return normalizedLowerGamma(s.toComplex(), z);
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex normalizedLowerGamma(const Variant& s, Real z) {
        ValueType sType = s.valueType();

        switch (sType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(sType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return normalizedLowerGamma(s.toInteger(), z);
            }

            case ValueType::REAL: {
                return normalizedLowerGamma(s.toReal(), z);
            }

            case M::ValueType::COMPLEX: {
                return normalizedLowerGamma(s.toComplex(), z);
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex normalizedLowerGamma(const Variant& s, const Complex& z) {
        ValueType sType = s.valueType();

        switch (sType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(sType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return normalizedLowerGamma(s.toInteger(), z);
            }

            case ValueType::REAL: {
                return normalizedLowerGamma(s.toReal(), z);
            }

            case M::ValueType::COMPLEX: {
                return normalizedLowerGamma(s.toComplex(), z);
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex normalizedLowerGamma(Integer s, const Variant& z) {
        ValueType zType = z.valueType();

        switch (zType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(zType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return normalizedLowerGamma(s, z.toInteger());
            }

            case ValueType::REAL: {
                return normalizedLowerGamma(s, z.toReal());
            }

            case M::ValueType::COMPLEX: {
                return normalizedLowerGamma(s, z.toComplex());
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex normalizedLowerGamma(Real s, const Variant& z) {
        ValueType zType = z.valueType();

        switch (zType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(zType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return normalizedLowerGamma(s, z.toInteger());
            }

            case ValueType::REAL: {
                return normalizedLowerGamma(s, z.toReal());
            }

            case M::ValueType::COMPLEX: {
                return normalizedLowerGamma(s, z.toComplex());
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex normalizedLowerGamma(const Complex& s, const Variant& z) {
        ValueType zType = z.valueType();

        switch (zType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(zType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return normalizedLowerGamma(s, z.toInteger());
            }

            case ValueType::REAL: {
                return normalizedLowerGamma(s, z.toReal());
            }

            case M::ValueType::COMPLEX: {
                return normalizedLowerGamma(s, z.toComplex());
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex normalizedLowerGamma(const Variant& s, const Variant& z) {
        ValueType sType = s.valueType();

        switch (sType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(sType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return normalizedLowerGamma(s.toInteger(), z);
            }

            case ValueType::REAL: {
                return normalizedLowerGamma(s.toReal(), z);
            }

            case M::ValueType::COMPLEX: {
                return normalizedLowerGamma(s.toComplex(), z);
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Real normalizedUpperGamma(Integer s, Integer x) {
        return normalizedUpperGamma(static_cast<Real>(s), static_cast<Real>(x));
    }


    Real normalizedUpperGamma(Integer s, Real x) {
        return normalizedUpperGamma(static_cast<Real>(s), x);
    }


    Complex normalizedUpperGamma(Integer s, const Complex& x) {
        return normalizedUpperGamma(Complex(s), x);
    }


    Real normalizedUpperGamma(Real s, Integer x) {
        return normalizedUpperGamma(s, static_cast<Real>(x));
    }


    Real normalizedUpperGamma(Real s, Real x) {
        // See the write-up on the real version of the lower gamma function for notes.

        Real result;

        if (s > 0 && x >= 0) {
            if (s >= alphaz(x)) {
                if (s < 70 || x == 0) {
                    Real gammaS = gamma(s);
                    result = 1 - lowerRegularizedGammaEngine1(s, x, gammaS);
                } else {
                    Real lnGammaS = lnGamma(s);
                    result = 1 - lowerRegularizedGammaEngine2(s, x, lnGammaS);
                }
            } else {
                Real gammaS = gamma(s);
                result = upperIncompleteGammaEngine(s, x) / gammaS;
            }
        } else {
            result = NaN;
        }

        return result;
    }


    Complex normalizedUpperGamma(Real s, const Complex& x) {
        return normalizedUpperGamma(Complex(s), x);
    }


    Complex normalizedUpperGamma(const Complex& s, Integer x) {
        return normalizedUpperGamma(s, Complex(x));
    }


    Complex normalizedUpperGamma(const Complex& s, Real x) {
        return normalizedUpperGamma(s, Complex(x));
    }


    Complex normalizedUpperGamma(const Complex& s, const Complex& x) {
        // See the write-up on the real version of the lower gamma function for notes.

        Complex result;

        if (s.real() > 0 && x.real() >= 0) {
            if (s.real() >= alphaz(x.real())) {
                if (s.real() < 70 || x.real() == 0) {
                    Complex gammaS = gamma(s);
                    result = 1 - lowerRegularizedGammaEngine1(s, x, gammaS);
                } else {
                    Complex lnGammaS = lnGamma(s);
                    result = 1 - lowerRegularizedGammaEngine2(s, x, lnGammaS);
                }
            } else {
                Complex gammaS = gamma(s);
                result = upperIncompleteGammaEngine(s, x) / gammaS;
            }
        } else {
            result = NaN;
        }

        return result;
    }


    Complex normalizedUpperGamma(const Variant& s, Integer z) {
        ValueType sType = s.valueType();

        switch (sType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(sType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return normalizedUpperGamma(s.toInteger(), z);
            }

            case ValueType::REAL: {
                return normalizedUpperGamma(s.toReal(), z);
            }

            case M::ValueType::COMPLEX: {
                return normalizedUpperGamma(s.toComplex(), z);
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex normalizedUpperGamma(const Variant& s, Real z) {
        ValueType sType = s.valueType();

        switch (sType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(sType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return normalizedUpperGamma(s.toInteger(), z);
            }

            case ValueType::REAL: {
                return normalizedUpperGamma(s.toReal(), z);
            }

            case M::ValueType::COMPLEX: {
                return normalizedUpperGamma(s.toComplex(), z);
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex normalizedUpperGamma(const Variant& s, const Complex& z) {
        ValueType sType = s.valueType();

        switch (sType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(sType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return normalizedUpperGamma(s.toInteger(), z);
            }

            case ValueType::REAL: {
                return normalizedUpperGamma(s.toReal(), z);
            }

            case M::ValueType::COMPLEX: {
                return normalizedUpperGamma(s.toComplex(), z);
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex normalizedUpperGamma(Integer s, const Variant& z) {
        ValueType zType = z.valueType();

        switch (zType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(zType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return normalizedUpperGamma(s, z.toInteger());
            }

            case ValueType::REAL: {
                return normalizedUpperGamma(s, z.toReal());
            }

            case M::ValueType::COMPLEX: {
                return normalizedUpperGamma(s, z.toComplex());
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex normalizedUpperGamma(Real s, const Variant& z) {
        ValueType zType = z.valueType();

        switch (zType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(zType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return normalizedUpperGamma(s, z.toInteger());
            }

            case ValueType::REAL: {
                return normalizedUpperGamma(s, z.toReal());
            }

            case M::ValueType::COMPLEX: {
                return normalizedUpperGamma(s, z.toComplex());
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex normalizedUpperGamma(const Complex& s, const Variant& z) {
        ValueType zType = z.valueType();

        switch (zType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(zType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return normalizedUpperGamma(s, z.toInteger());
            }

            case ValueType::REAL: {
                return normalizedUpperGamma(s, z.toReal());
            }

            case M::ValueType::COMPLEX: {
                return normalizedUpperGamma(s, z.toComplex());
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex normalizedUpperGamma(const Variant& s, const Variant& z) {
        ValueType sType = s.valueType();

        switch (sType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(sType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return normalizedUpperGamma(s.toInteger(), z);
            }

            case ValueType::REAL: {
                return normalizedUpperGamma(s.toReal(), z);
            }

            case M::ValueType::COMPLEX: {
                return normalizedUpperGamma(s.toComplex(), z);
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Real inverseLowerGamma(Real s, Real y) {
        // We can find the inverse of the lower gamma function using Newton-Raphson or Halley's method noting that
        // the first partial derivative with respect to z is:
        //
        //       d
        //     ----- lg = x^(s-1) e^(-z)
        //      d z
        //
        // and the second partial derivative with respect to z is:
        //
        //      d^2
        //     ----- lg = (s-1) z^(s-2) e^(-z) - z^(s-1) e^(-z)
        //       dz
        //
        // We note that the second derivative is, in fact, easy to calculate, especially since the second term is
        // simply the first derivative.  Since lower gamma is fairly expensive to calculate, we'll use Halley's
        // method.  We should note that we must guard against the possibility that z temporarily reaches a negative
        // value which will cause the incomplete gamma function to blow up.
        //
        // To accelerate convergence, we can note that:
        //
        //     lg(s,z) = \int_{0}^{z} t^{s-1} e^{-t} dt
        //
        // Or, for s = 1, we can simplify this to:
        //
        //     lg(1,z) = \int_{0}^{z} e^{-t} dt = 1 - e^{-z}
        //
        // Which we can use to guess an initial value for y < 1.
        //
        //     z = -ln(1 - y)
        //
        // For y >= 1, we start with an initial guess of 1.0.
        //
        // In some cases, the algorithm will refuse to converge for small values of s (less than 0.6) due to either
        // a very large or a very small first derivative pushing the guess value of z negative.

        Real z;

        Real          sm1      = s - 1.0;
        Real          sm2      = s - 2.0;
        std::uint32_t fuzzSeed = 0;

        if (y < 1) {
            z = -std::log(1.0 - y);
        } else {
            z = 1.0;
        }

        Real nextGuessValue = 0.5 * z;
        Real lgm            = lowerGamma(s, z);
        Real error          = lgm - y;

        Real magnitudeError = std::numeric_limits<Real>::max();
        Real lastMagnitudeError;

        do {
            Real emz  = std::exp(-z);
            Real dlg  = std::pow(z, sm1) * emz;
            Real d2lg = sm1 * std::pow(z, sm2) * emz - dlg;

            z -= 2.0 * error * dlg / (2.0 * dlg * dlg - error * d2lg);

            if (std::isnan(z)) {
                // If our first and second derivates are both zero, we can end up with NaN.  This means two scenarios:
                //
                //     1. We're very-very close to zero so we can't resolve or slopes.  If this occurs, we find a
                //        value above our value and then do a binary search.
                //
                //     2. We're way out on the far right edge of the curve where the curve is extremely flat.  Result
                //        is likely infinite.

                if (y < M::epsilon) {
                    Real upperZ = 0.01;
                    while (lowerGamma(s, upperZ) < y) {
                        upperZ += 0.01;
                    }

                    z = inverseLowerGammaBinarySearch(s, y, 0, upperZ);
                } else {
                    z = M::infinity;
                }

                magnitudeError = 0;
            } else if (z <= 0.0) {
                // There are two scenarios where z can go negative:
                //
                //     1. The slope is so steep that we pass zero when we're looking for a value close to zero.  In
                //        this scenario, we switch over to a binary search algorithm since we can easily guess an
                //        upper bounds for z.
                //
                //     2. The slope is very shallow and we're very far out so we shoot past the Y axis.  In this
                //        scenario we start over with a different, smaller, guess value.
                //
                // The slope is related to s so we use the value of s to guess which scenario we have.

                if (s < 0.99) {
                    // Switch to binary search.

                    z = inverseLowerGammaBinarySearch(s, y, 0, -std::log(1.0 - y));
                    magnitudeError = 0;
                } else {
                    z = nextGuessValue;
                    nextGuessValue *= 0.5;

                    magnitudeError = std::numeric_limits<Real>::max();
                }
            } else {
                lgm    = lowerGamma(s, z);
                error  = lgm - y;

                lastMagnitudeError = magnitudeError;
                magnitudeError     = std::abs(error);

                if (magnitudeError >= lastMagnitudeError && magnitudeError > maximumAllowedInverseLowerGammaError) {
                    // If we are not converging then move our guess value left or right by a small random amount.
                    z += 0.2 * z * (static_cast<Real>(fuzzSeed) / static_cast<Real>(0xFFFFFFFFUL) - 0.5);
                    fuzzSeed = 1664525 * fuzzSeed + 1013904223; // Knuth-Lewis PRNG used for fuzzing.
                }
            }
        } while (!std::isnan(z) && magnitudeError > maximumAllowedInverseLowerGammaError);

        return z;
    }


    Real beta(Integer x, Integer y) {
        return beta(static_cast<Real>(x), static_cast<Real>(y));
    }


    Real beta(Integer x, Real y) {
        return beta(static_cast<Real>(x), y);
    }


    Complex beta(Integer x, const Complex& y) {
        return beta(Complex(x), y);
    }


    Real beta(Real x, Integer y) {
        return beta(x, static_cast<Real>(y));
    }


    Real beta(Real x, Real y) {
        Real result;

        if (x > 0 && y > 0) {
            Real xpy = x + y;
            if (x < 140 && y < 140 && xpy < 140) {
                result = gamma(x) * gamma(y) / gamma(xpy);
            } else {
                result = std::exp(lnGamma(x) + lnGamma(y) - lnGamma(xpy));
            }
        } else {
            result = NaN;
        }

        return result;
    }


    Complex beta(Real x, const Complex& y) {
        return beta(Complex(x), y);
    }


    Complex beta(const Complex& x, Integer y) {
        return beta(x, Complex(y));
    }


    Complex beta(const Complex& x, Real y) {
        return beta(x, Complex(y));
    }


    Complex beta(const Complex& x, const Complex& y) {
        Complex result;

        if (x.real() > 0 && y.real() > 0) {
            if (x.imag() == 0 && y.imag() == 0) {
                result = beta(x.real(), y.real());
            } else {
                result = exp(lnGamma(x) + lnGamma(y) - lnGamma(x + y));
            }
        } else {
            result = Complex(NaN, NaN);
        }

        return result;
    }


    Complex beta(const Variant& s, Integer z) {
        ValueType sType = s.valueType();

        switch (sType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(sType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return beta(s.toInteger(), z);
            }

            case ValueType::REAL: {
                return beta(s.toReal(), z);
            }

            case M::ValueType::COMPLEX: {
                return beta(s.toComplex(), z);
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex beta(const Variant& s, Real z) {
        ValueType sType = s.valueType();

        switch (sType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(sType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return beta(s.toInteger(), z);
            }

            case ValueType::REAL: {
                return beta(s.toReal(), z);
            }

            case M::ValueType::COMPLEX: {
                return beta(s.toComplex(), z);
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex beta(const Variant& s, const Complex& z) {
        ValueType sType = s.valueType();

        switch (sType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(sType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return beta(s.toInteger(), z);
            }

            case ValueType::REAL: {
                return beta(s.toReal(), z);
            }

            case M::ValueType::COMPLEX: {
                return beta(s.toComplex(), z);
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex beta(Integer s, const Variant& z) {
        ValueType zType = z.valueType();

        switch (zType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(zType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return beta(s, z.toInteger());
            }

            case ValueType::REAL: {
                return beta(s, z.toReal());
            }

            case M::ValueType::COMPLEX: {
                return beta(s, z.toComplex());
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex beta(Real s, const Variant& z) {
        ValueType zType = z.valueType();

        switch (zType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(zType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return beta(s, z.toInteger());
            }

            case ValueType::REAL: {
                return beta(s, z.toReal());
            }

            case M::ValueType::COMPLEX: {
                return beta(s, z.toComplex());
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex beta(const Complex& s, const Variant& z) {
        ValueType zType = z.valueType();

        switch (zType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(zType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return beta(s, z.toInteger());
            }

            case ValueType::REAL: {
                return beta(s, z.toReal());
            }

            case M::ValueType::COMPLEX: {
                return beta(s, z.toComplex());
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Complex beta(const Variant& s, const Variant& z) {
        ValueType sType = s.valueType();

        switch (sType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(sType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return beta(s.toInteger(), z);
            }

            case ValueType::REAL: {
                return beta(s.toReal(), z);
            }

            case M::ValueType::COMPLEX: {
                return beta(s.toComplex(), z);
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Real reimannZeta(PerThread& /* pt */, Integer s) {
        // Note that boost::math::zeta uses the Euler-Maclaurin formula to calculate the terms of the Reimann Zeta
        // function.

        return boost::math::zeta<Real>(Real(s));
    }


    Real reimannZeta(PerThread& /* pt */, Real s) {
        // Note that boost::math::zeta uses the Euler-Maclaurin formula to calculate the terms of the Reimann Zeta
        // function.

        return boost::math::zeta<Real>(s);
    }


    static_assert(
        sizeof(ZC) * maximumZetaIterations <= PerThread::temporaryBufferSizeInBytes,
        "M::PerThread temporary buffer too small"
    );

    Complex reimannZeta(PerThread& pt, const Complex& s) {
        Complex    result;
        Complex::T sr = s.real();

        if (sr == 1.0 || sr == 0.0) {
            internalTriggerCanNotConvergeError();
            result = Complex(M::NaN, M::NaN);
        } else if (sr > 1.0) {
            result = toComplex(reimannZetaByAlternatingSeries(pt, ZC(s.real(), s.imag())));
        } else /* if (sr < 1.0 && sr != 0.0) */ {
            // I note that alternating series method gives reasonable results without the use of the reflection formula
            // over much of this range; however, the method becomes increasingly innaccurate for increasingly negative
            // values.  For this reason, we apply the reflection formula to keep our results positive.

            ZC ss   = toStdComplex(s);
            ZC sm1  = ss - ZC(1.0);
            ZC oms  = ZC(1.0) - ss;
            ZC g1ms = toStdComplex(gamma(1.0 - s));
            ZC z1ms = reimannZetaByAlternatingSeries(pt, oms);

            result = toComplex(std::pow(2.0, ss) * std::pow(M::pi, sm1) * std::sin(0.5 * M::pi * ss) * g1ms * z1ms);
        }

        return result;
    }


    Complex reimannZeta(PerThread& pt, const Variant& s) {
        ValueType sType = s.valueType();

        switch (sType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case M::ValueType::TUPLE:
            case M::ValueType::SET:
            case M::ValueType::MATRIX_BOOLEAN:
            case M::ValueType::MATRIX_INTEGER:
            case M::ValueType::MATRIX_REAL:
            case M::ValueType::MATRIX_COMPLEX: {
                M::internalTriggerTypeConversionError(sType, ValueType::COMPLEX);
                return std::numeric_limits<Real>::quiet_NaN();
            }

            case ValueType::BOOLEAN:
            case ValueType::INTEGER: {
                return Complex(reimannZeta(pt, s.toInteger()));
            }

            case ValueType::REAL: {
                return Complex(reimannZeta(pt, s.toReal()));
            }

            case M::ValueType::COMPLEX: {
                return reimannZeta(pt, s.toComplex());
            }

            default: {
                assert(false);
                return 0;
            }
        }
    }


    Real erf(Integer x) {
        return erf(static_cast<Real>(x));
    }


    Real erf(Real x) {
        // For values of x greater than 0.5, the continued fraction expansion approach is more performant; however,
        // we note that erf(0.5) is roughly 0.5 which means we'll likely lose at least one LSB of precision.  We
        // therefore switch over to the continued fraction approach later and accept some loss in performance.

        Real result;

        if (x < -2.0) {
            result = erfcEngine(-x) - 1.0;
        } else if (x < 0) {
            result = -erfEngine(-x);
        } else if (x < +2.0) {
            result = erfEngine(x);
        } else {
            result = 1.0 - erfcEngine(x);
        }

        return result;
    }


    Complex erf(const Complex& x) {
        // For values of x greater than 0.5, the continued fraction expansion approach is more performant; however,
        // we note that erf(0.5) is roughly 0.5 which means we'll likely lose at least one LSB of precision.  We
        // therefore switch over to the continued fraction approach later and accept some loss in performance.

        Complex z;

        Complex y(std::abs(x.real()), std::abs(x.imag()));
        Real m = x.real() * x.real() + x.imag() * x.imag();
        if (m < 2.0) {
            z = erfEngine(y);
        } else {
            z = 1.0 - erfcEngine(y);
        }

        Real r = x.real() < 0 ? -z.real() : z.real();
        Real i = x.imag() < 0 ? -z.imag() : z.imag();

        return Complex(r, i);
    }


    Real erfc(Integer x) {
        return erfc(static_cast<Real>(x));
    }


    Real erfc(Real x) {
        Real result;

        if (x < -2.0) {
            result = 2.0 - erfcEngine(-x);
        } else if (x < 0) {
            result = 1.0 + erfEngine(-x);
        } else if (x < +2.0) {
            result = resiprocalSqrtPi * upperGamma(0.5, x * x);
        } else {
            result = erfcEngine(x);
        }

        return result;
    }


    Complex erfc(const Complex& x) {
        Complex result;

        if (x.imag() == 0) {
            result = Complex(erfc(x.real()));
        } else {
            result = 1.0 - erf(x);
        }

        return result;
    }


    Real erfInv(Real x) {
        // Find the inverse erf function value using Newton-Raphson.

        Real result = 0;
        if (x > -1.0 && x < 1.0) {
            Real     lastResult       = 0;
            unsigned terminationCount = 10;
            Real     change;

            do {
                lastResult = result;

                Real m = erfConstant * std::exp(-result * result);
                result -= (erf(result) - x) / m;

                change = std::abs(result - lastResult);
                if (change <= 2000.0 * epsilon) {
                    --terminationCount;
                }
            } while (change != 0 && terminationCount > 0);
        } else {
            result = NaN;
        }

        return result;
    }


    Complex lambertW(Integer k, Integer z, Real epsilon) {
        return lambertW(k, static_cast<Real>(z), epsilon);
    }


    Complex lambertW(Integer k, Real z, Real epsilon) {
        return lambertW(k, Complex(z), epsilon);
    }


    Complex lambertW(Integer k, const Complex& z, Real epsilon) {
        Complex result;

        if (z.imag() == 0) {
            if (std::isnan(z.real())) {
                result = Complex(NaN, NaN);
            } else if (std::isinf(z.real())) {
                result = Complex(z.real(), 0.0);
            } else {
                if (z.real() >= -oneOverE) {
                    if (k == 0) {
                        result = boost::math::lambert_w0(static_cast<long double>(z.real()), LambertWBoostPolicy());

                        if (std::isnan(result.real())) {
                            result = lambertWCorless(k, z, epsilon);
                        }
                    } else if (k == -1) {
                        result = boost::math::lambert_wm1(static_cast<long double>(z.real()), LambertWBoostPolicy());

                        if (std::isnan(result.real())) {
                            result = lambertWCorless(k, z, epsilon);
                        }
                    } else {
                        result = lambertWCorless(k, z, epsilon);
                    }
                } else {
                    result = lambertWCorless(k, z, epsilon);
                }
            }
        } else {
            if (std::isnan(z.real()) || std::isnan(z.imag())) {
                result = Complex(NaN, NaN);
            } else if (std::isinf(z.real())) {
                if (std::isinf(z.imag())) {
                    result = Complex(NaN, NaN);
                } else {
                    if (z.real() > 0) {
                        result = Complex(0.0, z.real() + 2.0 * pi * k);
                    } else {
                        result = Complex(0.0, 2.0 * pi * k - z.real());
                    }
                }
            } else {
                result = lambertWCorless(k, z, epsilon);
            }
        }

        return result;
    }


    Complex lambertW(Real k, Integer z, Real epsilon) {
        Complex result;

        Real wholePart;
        if (std::modf(k, &wholePart) == 0) {
            result = lambertW(static_cast<Integer>(k), z, epsilon);
        } else {
            result = Complex(NaN, NaN);
        }

        return result;
    }


    Complex lambertW(Real k, Real z, Real epsilon) {
        Complex result;

        Real wholePart;
        if (std::modf(k, &wholePart) == 0) {
            result = lambertW(static_cast<Integer>(k), z, epsilon);
        } else {
            result = Complex(NaN, NaN);
        }

        return result;
    }


    Complex lambertW(Real k, const Complex& z, Real epsilon) {
        Complex result;

        Real wholePart;
        if (std::modf(k, &wholePart) == 0) {
            result = lambertW(static_cast<Integer>(k), z, epsilon);
        } else {
            result = Complex(NaN, NaN);
        }

        return result;
    }


    Complex lambertW(const Complex& k, Integer z, Real epsilon) {
        Complex result;

        Real wholePart;
        if (k.imag() == 0 && std::modf(k.real(), &wholePart) == 0) {
            result = lambertW(static_cast<Integer>(k.real()), z, epsilon);
        } else {
            result = Complex(NaN, NaN);
        }

        return result;
    }


    Complex lambertW(const Complex& k, Real z, Real epsilon) {
        Complex result;

        Real wholePart;
        if (k.imag() == 0 && std::modf(k.real(), &wholePart) == 0) {
            result = lambertW(static_cast<Integer>(k.real()), z, epsilon);
        } else {
            result = Complex(NaN, NaN);
        }

        return result;
    }


    Complex lambertW(const Complex& k, const Complex& z, Real epsilon) {
        Complex result;

        Real wholePart;
        if (k.imag() == 0 && std::modf(k.real(), &wholePart) == 0) {
            result = lambertW(static_cast<Integer>(k.real()), z, epsilon);
        } else {
            result = Complex(NaN, NaN);
        }

        return result;
    }
}
