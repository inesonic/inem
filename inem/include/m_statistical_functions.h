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
* This header defines statistical math functions.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_STATISTICAL_FUNCTIONS_H
#define M_STATISTICAL_FUNCTIONS_H

#include "m_common.h"
#include "m_per_thread.h"
#include "model_matrix.h"
#include "m_basic_functions.h"
#include "m_matrix_boolean.h"
#include "m_matrix_integer.h"
#include "m_matrix_real.h"
#include "m_matrix_complex.h"
#include "m_list.h"
#include "m_special_values.h"
#include "m_exceptions.h"
#include "m_type_conversion.h"
#include "m_intrinsic_types.h"

namespace M {
    class PerThread;
    class Set;
    class Tuple;
    class Variant;

    /**
     * Function that is used locally to enforce a NaN error.  Exists to get around issues in Windows.
     */
    M_PUBLIC_API void internalLocalTriggerNaNError();

    /**
     * Function that is used locally to enforce a NaN error.  Exists to get around issues in Windows.
     */
    M_PUBLIC_API void internalLocalTriggerInfinityError();

    /**
     * Function that triggers an invalid parameter error.  This function exists to work around limitations in Windows.
     */
    M_PUBLIC_API void internalLocalTriggerInvalidParameterValueError();

    /**
     * Function that returns a random 64-bit integer.
     *
     * \param[in] pt The per-thread instance used to generate the random deviate.
     *
     * \return Returns a random 64-bit integer value.
     */
    M_PUBLIC_API Integer randomInteger64(PerThread& pt);

    /**
     * Function that returns a random 64-bit positive integer.
     *
     * \param[in] pt    The per-thread instance used to generate the random deviate.
     *
     * \param[in] dummy The dummy value.
     */
    template<typename T> M_PUBLIC_TEMPLATE_FUNCTION Integer randomInteger64(PerThread& pt, T dummy) {
        (void) dummy;
        return randomInteger64(pt);
    }

    /**
     * Function that returns a matrix of random integers values.
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \return Returns a matrix of random values.
     */
    M_PUBLIC_API MatrixInteger randomInteger64(PerThread& pt, Integer numberRows, Integer numberColumns);

    /**
     * Template function that returns a matrix of random integers values.
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \return Returns a matrix of random values.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION MatrixInteger randomInteger64(
            PerThread& pt,
            T1         numberRows,
            T2         numberColumns
        ) {
        Integer nr;
        Integer nc;
        if (toInteger(nr, numberRows) && toInteger(nc, numberColumns)) {
            return MatrixInteger::randomInteger64(pt, nr, nc);
        } else {
            return MatrixInteger();
        }
    }

    /**
     * Function that returns a random 32-bit positive integer.
     *
     * \param[in] pt The per-thread instance used to generate the random deviate.
     *
     * \return Returns a random 64-bit integer value.
     */
    M_PUBLIC_API Integer randomInteger32(PerThread& pt);

    /**
     * Function that returns a random 32-bit positive integer.
     *
     * \param[in] pt    The per-thread instance used to generate the random deviate.
     *
     * \param[in] dummy The dummy value.
     */
    template<typename T> M_PUBLIC_TEMPLATE_FUNCTION Integer randomInteger32(PerThread& pt, T dummy) {
        (void) dummy;
        return randomInteger32(pt);
    }

    /**
     * Function that returns a matrix of random integers values.
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \return Returns a matrix of random values.
     */
    M_PUBLIC_API MatrixInteger randomInteger32(PerThread& pt, Integer numberRows, Integer numberColumns);

    /**
     * Template function that returns a matrix of random integers values.
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \return Returns a matrix of random values.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION MatrixInteger randomInteger32(
            PerThread& pt,
            T1         numberRows,
            T2         numberColumns
        ) {
        Integer nr;
        Integer nc;
        if (toInteger(nr, numberRows) && toInteger(nc, numberColumns)) {
            return MatrixInteger::randomInteger32(pt, nr, nc);
        } else {
            return MatrixInteger();
        }
    }

    /**
     * Function that returns a true random number.
     *
     * \param[in] pt The per-thread instance used to generate the random deviate.
     *
     * \return Returns a random 32-bit integer.
     */
    M_PUBLIC_API Integer trng32(PerThread& pt);

    /**
     * Function that returns a true random number.
     *
     * \param[in] pt The per-thread instance used to generate the random deviate.
     *
     * \return Returns a random 64-bit integer.
     */
    M_PUBLIC_API Integer trng64(PerThread& pt);

    /**
     * Function that returns a true random number between 0 and 1, inclusive.
     *
     * \param[in] pt The per-thread instance used to generate the random deviate.
     *
     * \return Returns a random number in a uniform distribution.
     */
    M_PUBLIC_API M::Real trngU(PerThread& pt);

    /**
     * Template function that returns a true random number.
     *
     * \param[in] pt The per-thread instance used to generate the random deviate.
     *
     * \param[in] t  An arbitrary parameter.
     *
     * \return Returns a random 32-bit number.
     */
    template<typename T> M_PUBLIC_TEMPLATE_FUNCTION Integer trng32(PerThread& pt, T t) {
        (void) t;
        return trng32(pt);
    }

    /**
     * Template function that returns a true random number.
     *
     * \param[in] pt The per-thread instance used to generate the random deviate.
     *
     * \param[in] t  An arbitrary parameter.
     *
     * \return Returns a random 64-bit number.
     */
    template<typename T> M_PUBLIC_TEMPLATE_FUNCTION Integer trng64(PerThread& pt, T t) {
        (void) t;
        return trng64(pt);
    }

    /**
     * Template function that returns a true random number.
     *
     * \param[in] pt The per-thread instance used to generate the random deviate.
     *
     * \param[in] t  An arbitrary parameter.
     *
     * \return Returns a random value in a uniform distribution.
     */
    template<typename T> M_PUBLIC_TEMPLATE_FUNCTION Real trngU(PerThread& pt, T t) {
        (void) t;
        return trngU(pt);
    }

    /**
     * Function that returns a uniform random deviate over the range [0, 1].
     *
     * \param[in] pt The per-thread instance used to generate the random deviate.
     *
     * \return Returns the random value.
     */
    M_PUBLIC_API inline Real uniformDeviateInclusive(PerThread& pt) {
        return pt.randomInclusive();
    }

    /**
     * Function that returns a matrix of a uniform random deviate over the range [0, 1].
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \return Returns a matrix of random values.
     */
    M_PUBLIC_API inline MatrixReal uniformDeviateInclusive(PerThread& pt, Integer numberRows, Integer numberColumns) {
        return MatrixReal::randomInclusive(pt, numberRows, numberColumns);
    }

    /**
     * Function that returns a matrix of a uniform random deviate over the range [0, 1].
     *
     * \param[in] pt The per-thread instance used to generate the random deviate.
     *
     * \param[in] t  Dummy parameter.
     *
     * \return Returns a a single random value.
     */
    template<typename T> M_PUBLIC_TEMPLATE_FUNCTION Real uniformDeviateInclusive(PerThread& pt, T t) {
        (void) t;
        return pt.randomInclusive();
    }

    /**
     * Function that returns a matrix of a uniform random deviate over the range [0, 1].
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \return Returns a matrix of random values.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION MatrixReal uniformDeviateInclusive(
            PerThread& pt,
            T1         numberRows,
            T2         numberColumns
        ) {
        Integer nr;
        Integer nc;
        if (toInteger(nr, numberRows) && toInteger(nc, numberColumns)) {
            return MatrixReal::randomInclusive(pt, nr, nc);
        } else {
            return MatrixReal();
        }
    }

    /**
     * Function that returns a uniform random deviate over the range (0, 1).
     *
     * \param[in] pt The per-thread instance used to generate the random deviate.
     *
     * \return Returns the random value.
     */
    M_PUBLIC_API inline Real uniformDeviateExclusive(PerThread& pt) {
        return pt.randomExclusive();
    }

    /**
     * Function that returns a matrix of a uniform random deviate over the range (0, 1).
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \return Returns a matrix of random values.
     */
    M_PUBLIC_API inline MatrixReal uniformDeviateExclusive(PerThread& pt, Integer numberRows, Integer numberColumns) {
        return MatrixReal::randomExclusive(pt, numberRows, numberColumns);
    }

    /**
     * Function that returns a matrix of a uniform random deviate over the range (0, 1).
     *
     * \param[in] pt The per-thread instance used to generate the random deviate.
     *
     * \param[in] t  Dummy parameter.
     *
     * \return Returns a matrix of random values.
     */
    template<typename T> M_PUBLIC_TEMPLATE_FUNCTION Real uniformDeviateExclusive(PerThread& pt, T t) {
        (void) t;
        return pt.randomExclusive();
    }

    /**
     * Function that returns a matrix of a uniform random deviate over the range (0, 1).
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \return Returns a matrix of random values.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION MatrixReal uniformDeviateExclusive(
            PerThread& pt,
            T1         numberRows,
            T2         numberColumns
        ) {
        Integer nr;
        Integer nc;
        if (toInteger(nr, numberRows) && toInteger(nc, numberColumns)) {
            return MatrixReal::randomExclusive(pt, nr, nc);
        } else {
            return MatrixReal();
        }
    }

    /**
     * Function that calculates a value in a normal distribution with mean of 0 and sigma of 1.0.
     *
     * \param[in] pt The per-thread instance used to generate the random deviate.
     *
     * \return Returns the random value.
     */
    M_PUBLIC_API inline Real normalDeviate(PerThread& pt) {
        return pt.randomNormal();
    }

    /**
     * Function that calculates a value in a normal distribution with mean of 0 and sigma of 1.0.
     *
     * \param[in] pt    The per-thread instance used to generate the random deviate.
     *
     * \param[in] mean  The mean value
     *
     * \param[in] sigma The standard deviation.
     *
     * \return Returns the random value.
     */
    M_PUBLIC_API inline Real normalDeviate(PerThread& pt, Real mean, Real sigma = 1.0) {
        return pt.randomNormal(mean, sigma);
    }

    /**
     * Function that calculates a value in a normal distribution with mean of 0 and sigma of 1.0.
     *
     * \param[in] pt    The per-thread instance used to generate the random deviate.
     *
     * \param[in] mean  The mean value
     *
     * \param[in] sigma The standard deviation.
     *
     * \return Returns the random value.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION Real normalDeviate(
            PerThread& pt,
            T1         mean,
            T2         sigma = T2(1.0)
        ) {
        Real m;
        Real s;
        if (toReal(m, mean) && toReal(s, sigma)) {
            return pt.randomNormal(m, s);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates a value in a normal distribution with mean of 0 and sigma of 1.0.
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \param[in] mean          The mean value
     *
     * \param[in] sigma         The standard deviation.
     *
     * \return Returns the random value.
     */
    M_PUBLIC_API inline MatrixReal normalDeviate(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns,
            Real       mean,
            Real       sigma = 1.0
        ) {
        return MatrixReal::randomNormal(pt, numberRows, numberColumns, mean, sigma);
    }

    /**
     * Function that calculates a value in a normal distribution with mean of 0 and sigma of 1.0.
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \param[in] mean          The mean value
     *
     * \param[in] sigma         The standard deviation.
     *
     * \return Returns the random value.
     */
    template<typename T1, typename T2, typename T3, typename T4> M_PUBLIC_TEMPLATE_FUNCTION MatrixReal normalDeviate(
            PerThread& pt,
            T1         numberRows,
            T2         numberColumns,
            T3         mean,
            T4         sigma = T4(1.0)
        ) {
        Integer nr;
        Integer nc;
        Real    m;
        Real    s;
        if (toInteger(nr, numberRows) && toInteger(nc, numberColumns) && toReal(m, mean) && toReal(s, sigma)) {
            return MatrixReal::randomNormal(pt, nr, nc, m, s);
        } else {
            return MatrixReal();
        }
    }

    /**
     * Function that calculates the normal PDF.
     *
     * \param[in] x     The value to calculate the PDF at.
     *
     * \param[in] mean  The desired mean value.
     *
     * \param[in] sigma The standard deviation.
     *
     * \return Returns the PDF of the normal distribution.
     */
    M_PUBLIC_API Real normalPdf(Real x, Real mean = 0.0, Real sigma = 1.0);

    /**
     * Function that calculates the normal PDF.
     *
     * \param[in] x     The value to calculate the PDF at.
     *
     * \param[in] mean  The desired mean value.
     *
     * \param[in] sigma The standard deviation.
     *
     * \return Returns the PDF of the normal distribution.
     */
    template<typename T1, typename T2 = Real, typename T3 = Real> M_PUBLIC_TEMPLATE_FUNCTION Real normalPdf(
            T1 x,
            T2 mean = T2(0.0),
            T3 sigma = T3(1.0)
        ) {
        Real tx;
        Real tm;
        Real ts;
        if (toReal(tx, x) && toReal(tm, mean) && toReal(ts, sigma)) {
            return normalPdf(tx, tm, ts);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates the normal CDF.
     *
     * \param[in] x     The value to calculate the CDF at.
     *
     * \param[in] mean  The desired mean value.
     *
     * \param[in] sigma The standard deviation.
     *
     * \return Returns the CDF of the normal distribution.
     */
    M_PUBLIC_API Real normalCdf(Real x, Real mean = 0.0, Real sigma = 1.0);

    /**
     * Function that calculates the normal CDF.
     *
     * \param[in] x     The value to calculate the CDF at.
     *
     * \param[in] mean  The desired mean value.
     *
     * \param[in] sigma The standard deviation.
     *
     * \return Returns the CDF of the normal distribution.
     */
    template<typename T1, typename T2 = Real, typename T3 = Real> M_PUBLIC_TEMPLATE_FUNCTION Real normalCdf(
            T1 x,
            T2 mean = T2(0.0),
            T3 sigma = T3(1.0)
        ) {
        Real tx;
        Real tm;
        Real ts;
        if (toReal(tx, x) && toReal(tm, mean) && toReal(ts, sigma)) {
            return normalCdf(tx, tm, ts);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates the normal quantile.
     *
     * \param[in] p     The value to calculate the quantile at.
     *
     * \param[in] mean  The desired mean value.
     *
     * \param[in] sigma The standard deviation.
     *
     * \return Returns the quantile of the normal distribution.
     */
    M_PUBLIC_API Real normalQuantile(Real p, Real mean = 0.0, Real sigma = 1.0);

    /**
     * Function that calculates the normal quantile.
     *
     * \param[in] p     The value to calculate the quantile at.
     *
     * \param[in] mean  The desired mean value.
     *
     * \param[in] sigma The standard deviation.
     *
     * \return Returns the quantile of the normal distribution.
     */
    template<typename T1, typename T2 = Real, typename T3 = Real> M_PUBLIC_TEMPLATE_FUNCTION Real normalQuantile(
            T1 p,
            T2 mean = T2(0.0),
            T3 sigma = T3(1.0)
        ) {
        Real tp;
        Real tm;
        Real ts;
        if (toReal(tp, p) && toReal(tm, mean) && toReal(ts, sigma)) {
            return normalQuantile(tp, tm, ts);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates a value in a gamma distribution.
     *
     * \param[in] pt The per-thread instance used to generate the random deviate.
     *
     * \param[in] k  The shape term.
     *
     * \param[in] s  The scale term.
     *
     * \return Returns the random value.
     */
    M_PUBLIC_API inline Real gammaDeviate(PerThread& pt, Real k, Real s = 1.0) {
        return pt.randomGamma(k, s);
    }

    /**
     * Function that calculates a value in a gamma distribution.
     *
     * \param[in] pt The per-thread instance used to generate the random deviate.
     *
     * \param[in] k  The shape term.
     *
     * \param[in] s  The scale term.
     *
     * \return Returns the random value.
     */
    template<typename T1, typename T2 = Real> M_PUBLIC_TEMPLATE_FUNCTION Real gammaDeviate(
            PerThread& pt,
            T1         k,
            T2         s = T2(1.0)
        ) {
        Real tk;
        Real ts;
        if (toReal(tk, k) && toReal(ts, s)) {
            return pt.randomGamma(tk, ts);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates a value in a gamma distribution.
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \param[in] k             The shape term.
     *
     * \param[in] s             The scale term.
     *
     * \return Returns the random value.
     */
    M_PUBLIC_API inline MatrixReal gammaDeviate(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns,
            Real       k,
            Real       s = 1.0
        ) {
        return MatrixReal::randomGamma(pt, numberRows, numberColumns, k, s);
    }

    /**
     * Function that calculates a value in a gamma distribution.
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \param[in] k             The shape term.
     *
     * \param[in] s             The scale term.
     *
     * \return Returns the random value.
     */
    template<typename T1, typename T2, typename T3, typename T4 = Real>
        M_PUBLIC_TEMPLATE_FUNCTION MatrixReal gammaDeviate(
            PerThread& pt,
            T1         numberRows,
            T2         numberColumns,
            T3         k,
            T4         s = T4(1.0)
        ) {
        Integer nr;
        Integer nc;
        Real    tk;
        Real    ts;
        if (toInteger(nr, numberRows) && toInteger(nc, numberColumns) && toReal(tk, k) && toReal(ts, s)) {
            return MatrixReal::randomGamma(pt, nr, nc, tk, ts);
        } else {
            return MatrixReal();
        }
    }

    /**
     * Function that calculates the PDF in a gamma distribution.
     *
     * \param[in] x The value to calculate the PDF at.
     *
     * \param[in] k The desired mean value.
     *
     * \param[in] s The scale term.
     *
     * \return Returns the PDF of the gamma distribution.
     */
    M_PUBLIC_API Real gammaPdf(Real x, Real k, Real s = 1.0);

    /**
     * Function that calculates the PDF in a gamma distribution.
     *
     * \param[in] x The value to calculate the PDF at.
     *
     * \param[in] k The desired mean value.
     *
     * \param[in] s The scale term.
     *
     * \return Returns the PDF of the gamma distribution.
     */
    template<typename T1, typename T2, typename T3 = Real> M_PUBLIC_TEMPLATE_FUNCTION Real gammaPdf(
            T1 x,
            T2 k,
            T3 s = T3(1.0)
        ) {
        Real tx;
        Real tk;
        Real ts;
        if (toReal(tx, x) && toReal(tk, k) && toReal(ts, s)) {
            return gammaPdf(tx, tk, ts);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates the CDF in the gamma distribution.
     *
     * \param[in] x The value to calculate the CDF at.
     *
     * \param[in] k The shape term.
     *
     * \param[in] s The scale term.
     *
     * \return Returns the CDF of the gamma distribution.
     */
    M_PUBLIC_API Real gammaCdf(Real x, Real k, Real s = 1.0);

    /**
     * Function that calculates the CDF in the gamma distribution.
     *
     * \param[in] x The value to calculate the CDF at.
     *
     * \param[in] k The shape term.
     *
     * \param[in] s The scale term.
     *
     * \return Returns the CDF of the gamma distribution.
     */
    template<typename T1, typename T2, typename T3 = Real> M_PUBLIC_TEMPLATE_FUNCTION Real gammaCdf(
            T1 x,
            T2 k,
            T3 s = T3(1.0)
        ) {
        Real tx;
        Real tk;
        Real ts;
        if (toReal(tx, x) && toReal(tk, k) && toReal(ts, s)) {
            return gammaCdf(tx, tk, ts);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates the quantile of the gamma distribution.
     *
     * \param[in] p The value to calculate the quantile at.
     *
     * \param[in] k The shape term.
     *
     * \param[in] s The scale term.
     *
     * \return Returns the quantile of the gamma distribution.
     */
    M_PUBLIC_API Real gammaQuantile(Real p, Real k, Real s = 1.0);

    /**
     * Function that calculates the quantile of the gamma distribution.
     *
     * \param[in] p The value to calculate the quantile at.
     *
     * \param[in] k The shape term.
     *
     * \param[in] s The scale term.
     *
     * \return Returns the quantile of the gamma distribution.
     */
    template<typename T1, typename T2, typename T3 = Real> M_PUBLIC_TEMPLATE_FUNCTION Real gammaQuantile(
            T1 p,
            T2 k,
            T3 s = T3(1.0)
        ) {
        Real tp;
        Real tk;
        Real ts;
        if (toReal(tp, p) && toReal(tk, k) && toReal(ts, s)) {
            return gammaQuantile(tp, tk, ts);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates a value in a Weibull distribution.
     *
     * \param[in] pt    The per-thread instance used to generate the random deviate.
     *
     * \param[in] shape The shape term.
     *
     * \param[in] scale The scale term.
     *
     * \param[in] delay The delay term.
     *
     * \return Returns the random value.
     */
    M_PUBLIC_API inline Real weibullDeviate(PerThread& pt, Real shape, Real scale = 1.0, Real delay = 0.0) {
        return pt.randomWeibull(scale, shape, delay);
    }

    /**
     * Function that calculates a value in a Weibull distribution.
     *
     * \param[in] pt    The per-thread instance used to generate the random deviate.
     *
     * \param[in] shape The shape term.
     *
     * \param[in] scale The scale term.
     *
     * \param[in] delay The delay term.
     *
     * \return Returns the random value.
     */
    template<typename T1, typename T2 = Real, typename T3 = Real> M_PUBLIC_TEMPLATE_FUNCTION Real weibullDeviate(
            PerThread& pt,
            T1         shape,
            T2         scale = T2(1.0),
            T3         delay = T3(0.0)
        ) {
        Real lShape;
        Real lScale;
        Real lDelay;
        if (toReal(lShape, shape) && toReal(lScale, scale) && toReal(lDelay, delay)) {
            return pt.randomWeibull(lScale, lShape, lDelay);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates a value in a Weibull distribution.
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \param[in] shape         The shape term.
     *
     * \param[in] scale         The scale term.
     *
     * \param[in] delay         The delay term.
     *
     * \return Returns the random value.
     */
    M_PUBLIC_API inline MatrixReal weibullDeviate(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns,
            Real       shape,
            Real       scale,
            Real       delay = 0.0
        ) {
        return MatrixReal::randomWeibull(pt, numberRows, numberColumns, scale, shape, delay);
    }

    /**
     * Function that calculates a value in a Weibull distribution.
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \param[in] shape         The shape term.
     *
     * \param[in] scale         The scale term.
     *
     * \param[in] delay         The delay term.
     *
     * \return Returns the random value.
     */
    template<typename T1, typename T2, typename T3, typename T4, typename T5 = Real>
        M_PUBLIC_TEMPLATE_FUNCTION MatrixReal weibullDeviate(
            PerThread& pt,
            T1         numberRows,
            T2         numberColumns,
            T3         shape,
            T4         scale,
            T5         delay = T5(0.0)
        ) {
        Integer lNumberRows;
        Integer lNumberColumns;
        Real    lShape;
        Real    lScale;
        Real    lDelay;
        if (toInteger(lNumberRows, numberRows)       &&
            toInteger(lNumberColumns, numberColumns) &&
            toReal(lShape, shape)                    &&
            toReal(lScale, scale)                    &&
            toReal(lDelay, delay)                       ) {
            return MatrixReal::randomWeibull(pt, lNumberRows, lNumberColumns, lScale, lShape, lDelay);
        } else {
            return MatrixReal();
        }
    }

    /**
     * Function that calculates the PDF in a Weibull distribution.
     *
     * \param[in] x     The value to calculate the PDF at.
     *
     * \param[in] shape The shape term.
     *
     * \param[in] scale The scale term.
     *
     * \param[in] delay The delay term.
     *
     * \return Returns the PDF of the Weibull distribution.
     */
    M_PUBLIC_API Real weibullPdf(Real x, Real shape, Real scale = 1.0, Real delay = 0.0);

    /**
     * Function that calculates the PDF in a Weibull distribution.
     *
     * \param[in] x     The value to calculate the PDF at.
     *
     * \param[in] shape The shape term.
     *
     * \param[in] scale The scale term.
     *
     * \param[in] delay The delay term.
     *
     * \return Returns the PDF of the Weibull distribution.
     */
    template<typename T1, typename T2, typename T3 = Real, typename T4 = Real>
        M_PUBLIC_TEMPLATE_FUNCTION Real weibullPdf(
            T1 x,
            T2 shape,
            T3 scale = T3(1.0),
            T4 delay = T4(0.0)
        ) {
        Real lX;
        Real lShape;
        Real lScale;
        Real lDelay;
        if (toReal(lX, x) && toReal(lShape, shape) && toReal(lScale, scale) && toReal(lDelay, delay)) {
            return weibullPdf(lX, lShape, lScale, lDelay);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates the CDF in the Weibull distribution.
     *
     * \param[in] x     The value to calculate the CDF at.
     *
     * \param[in] shape The shape term.
     *
     * \param[in] scale The scale term.
     *
     * \param[in] delay The delay term.
     *
     * \return Returns the CDF of the Weibull distribution.
     */
    M_PUBLIC_API Real weibullCdf(Real x, Real shape, Real scale = 1.0, Real delay = 0.0);

    /**
     * Function that calculates the CDF in a Weibull distribution.
     *
     * \param[in] x     The value to calculate the PDF at.
     *
     * \param[in] shape The shape term.
     *
     * \param[in] scale The scale term.
     *
     * \param[in] delay The delay term.
     *
     * \return Returns the CDF of the Weibull distribution.
     */
    template<typename T1, typename T2, typename T3 = Real, typename T4 = Real>
        M_PUBLIC_TEMPLATE_FUNCTION Real weibullCdf(
            T1 x,
            T2 shape,
            T3 scale = T3(1.0),
            T4 delay = T4(0.0)
        ) {
        Real lX;
        Real lShape;
        Real lScale;
        Real lDelay;
        if (toReal(lX, x) && toReal(lShape, shape) && toReal(lScale, scale) && toReal(lDelay, delay)) {
            return weibullCdf(lX, lShape, lScale, lDelay);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates the quantile of the Weibull distribution.
     *
     * \param[in] p     The value to calculate the quantile at.
     *
     * \param[in] shape The shape term.
     *
     * \param[in] scale The scale term.
     *
     * \param[in] delay The delay term.
     *
     * \return Returns the quantile of the Weibull distribution.
     */
    M_PUBLIC_API Real weibullQuantile(Real p, Real shape, Real scale = 1.0, Real delay = 0.0);

    /**
     * Function that calculates the quantile of a Weibull distribution.
     *
     * \param[in] p     The value to calculate the quantile at.
     *
     * \param[in] shape The shape term.
     *
     * \param[in] scale The scale term.
     *
     * \param[in] delay The delay term.
     *
     * \return Returns the quantile of the Weibull distribution.
     */
    template<typename T1, typename T2, typename T3 = Real, typename T4 = Real>
        M_PUBLIC_TEMPLATE_FUNCTION Real weibullQuantile(
            T1 p,
            T2 shape,
            T3 scale = T3(1.0),
            T4 delay = T4(0.0)
        ) {
        Real lP;
        Real lShape;
        Real lScale;
        Real lDelay;
        if (toReal(lP, p) && toReal(lShape, shape) && toReal(lScale, scale) && toReal(lDelay, delay)) {
            return weibullQuantile(lP, lShape, lScale, lDelay);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates a value in a exponential distribution.
     *
     * \param[in] pt   The per-thread instance used to generate the random deviate.
     *
     * \param[in] rate The rate value.
     *
     * \return Returns the random value.
     */
    M_PUBLIC_API inline Real exponentialDeviate(PerThread& pt, Real rate) {
        return pt.randomExponential(rate);
    }

    /**
     * Function that calculates a value in a exponential distribution.
     *
     * \param[in] pt   The per-thread instance used to generate the random deviate.
     *
     * \param[in] rate The rate value.
     *
     * \return Returns the random value.
     */
    template<typename T1> M_PUBLIC_TEMPLATE_FUNCTION Real exponentialDeviate(PerThread& pt, T1 rate) {
        Real lRate;
        if (toReal(lRate, rate)) {
            return pt.randomExponential(lRate);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates a value in a exponential distribution.
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \param[in] rate          The rate value.
     *
     * \return Returns the random value.
     */
    M_PUBLIC_API inline MatrixReal exponentialDeviate(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns,
            Real       rate
        ) {
        return MatrixReal::randomExponential(pt, numberRows, numberColumns, rate);
    }

    /**
     * Function that calculates a value in a exponential distribution.
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \param[in] rate          The rate value.
     *
     * \return Returns the random value.
     */
    template<typename T1, typename T2, typename T3> M_PUBLIC_TEMPLATE_FUNCTION MatrixReal exponentialDeviate(
            PerThread& pt,
            T1         numberRows,
            T2         numberColumns,
            T3         rate
        ) {
        Integer lNumberRows;
        Integer lNumberColumns;
        Real    lRate;
        if (toInteger(lNumberRows, numberRows) && toInteger(lNumberColumns, numberColumns) && toReal(lRate, rate)) {
            return MatrixReal::randomExponential(pt, lNumberRows, lNumberColumns, lRate);
        } else {
            return MatrixReal();
        }
    }

    /**
     * Function that calculates the PDF in a exponential distribution.
     *
     * \param[in] x    The value to calculate the PDF at.
     *
     * \param[in] rate The rate value.
     *
     * \return Returns the PDF of the exponential distribution.
     */
    M_PUBLIC_API Real exponentialPdf(Real x, Real rate);

    /**
     * Function that calculates the PDF in a exponential distribution.
     *
     * \param[in] x    The value to calculate the PDF at.
     *
     * \param[in] rate The rate value.
     *
     * \return Returns the PDF of the exponential distribution.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION Real exponentialPdf(T1 x, T2 rate) {
        Real lX;
        Real lRate;
        if (toReal(lX, x) && toReal(lRate, rate)) {
            return exponentialPdf(lX, lRate);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates the CDF in the exponential distribution.
     *
     * \param[in] x    The value to calculate the CDF at.
     *
     * \param[in] rate The rate value.
     *
     * \return Returns the CDF of the exponential distribution.
     */
    M_PUBLIC_API Real exponentialCdf(Real x, Real rate);

    /**
     * Function that calculates the CDF in a exponential distribution.
     *
     * \param[in] x    The value to calculate the PDF at.
     *
     * \param[in] rate The rate value.
     *
     * \return Returns the CDF of the exponential distribution.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION Real exponentialCdf(T1 x, T2 rate) {
        Real lX;
        Real lRate;
        if (toReal(lX, x) && toReal(lRate, rate)) {
            return exponentialCdf(lX, lRate);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates the quantile of the exponential distribution.
     *
     * \param[in] p    The value to calculate the quantile at.
     *
     * \param[in] rate The rate value.
     *
     * \return Returns the quantile of the exponential distribution.
     */
    M_PUBLIC_API Real exponentialQuantile(Real p, Real rate);

    /**
     * Function that calculates the quantile of a exponential distribution.
     *
     * \param[in] p    The value to calculate the quantile at.
     *
     * \param[in] rate The shape term.
     *
     * \return Returns the quantile of the exponential distribution.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION Real exponentialQuantile(T1 p, T2 rate) {
        Real lP;
        Real lRate;
        if (toReal(lP, p) && toReal(lRate, rate)) {
            return exponentialQuantile(lP, lRate);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates a value in a rayleigh distribution.
     *
     * \param[in] pt    The per-thread instance used to generate the random deviate.
     *
     * \param[in] scale The scale value.
     *
     * \return Returns the random value.
     */
    M_PUBLIC_API inline Real rayleighDeviate(PerThread& pt, Real scale) {
        return pt.randomRayleigh(scale);
    }

    /**
     * Function that calculates a value in a rayleigh distribution.
     *
     * \param[in] pt    The per-thread instance used to generate the random deviate.
     *
     * \param[in] scale The scale value.
     *
     * \return Returns the random value.
     */
    template<typename T1> M_PUBLIC_TEMPLATE_FUNCTION Real rayleighDeviate(PerThread& pt, T1 scale) {
        Real lScale;
        if (toReal(lScale, scale)) {
            return pt.randomRayleigh(lScale);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates a value in a rayleigh distribution.
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \param[in] scale         The scale value.
     *
     * \return Returns the random value.
     */
    M_PUBLIC_API inline MatrixReal rayleighDeviate(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns,
            Real       scale
        ) {
        return MatrixReal::randomRayleigh(pt, numberRows, numberColumns, scale);
    }

    /**
     * Function that calculates a value in a rayleigh distribution.
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \param[in] scale         The scale value.
     *
     * \return Returns the random value.
     */
    template<typename T1, typename T2, typename T3> M_PUBLIC_TEMPLATE_FUNCTION MatrixReal rayleighDeviate(
            PerThread& pt,
            T1         numberRows,
            T2         numberColumns,
            T3         scale
        ) {
        Integer lNumberRows;
        Integer lNumberColumns;
        Real    lScale;
        if (toInteger(lNumberRows, numberRows) && toInteger(lNumberColumns, numberColumns) && toReal(lScale, scale)) {
            return MatrixReal::randomRayleigh(pt, lNumberRows, lNumberColumns, lScale);
        } else {
            return MatrixReal();
        }
    }

    /**
     * Function that calculates the PDF in a rayleigh distribution.
     *
     * \param[in] x     The value to calculate the PDF at.
     *
     * \param[in] scale The scale value.
     *
     * \return Returns the PDF of the rayleigh distribution.
     */
    M_PUBLIC_API Real rayleighPdf(Real x, Real scale);

    /**
     * Function that calculates the PDF in a rayleigh distribution.
     *
     * \param[in] x     The value to calculate the PDF at.
     *
     * \param[in] scale The scale value.
     *
     * \return Returns the PDF of the rayleigh distribution.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION Real rayleighPdf(T1 x, T2 scale) {
        Real lX;
        Real lScale;
        if (toReal(lX, x) && toReal(lScale, scale)) {
            return rayleighPdf(lX, lScale);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates the CDF in the rayleigh distribution.
     *
     * \param[in] x     The value to calculate the CDF at.
     *
     * \param[in] scale The scale value.
     *
     * \return Returns the CDF of the rayleigh distribution.
     */
    M_PUBLIC_API Real rayleighCdf(Real x, Real scale);

    /**
     * Function that calculates the CDF in a rayleigh distribution.
     *
     * \param[in] x     The value to calculate the PDF at.
     *
     * \param[in] scale The scale value.
     *
     * \return Returns the CDF of the rayleigh distribution.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION Real rayleighCdf(T1 x, T2 scale) {
        Real lX;
        Real lScale;
        if (toReal(lX, x) && toReal(lScale, scale)) {
            return rayleighCdf(lX, lScale);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates the quantile of the rayleigh distribution.
     *
     * \param[in] p     The value to calculate the quantile at.
     *
     * \param[in] scale The scale value.
     *
     * \return Returns the quantile of the rayleigh distribution.
     */
    M_PUBLIC_API Real rayleighQuantile(Real p, Real scale);

    /**
     * Function that calculates the quantile of a rayleigh distribution.
     *
     * \param[in] p     The value to calculate the quantile at.
     *
     * \param[in] scale The shape term.
     *
     * \return Returns the quantile of the rayleigh distribution.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION Real rayleighQuantile(T1 p, T2 scale) {
        Real lP;
        Real lScale;
        if (toReal(lP, p) && toReal(lScale, scale)) {
            return rayleighQuantile(lP, lScale);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates a value in a chi-squared distribution.
     *
     * \param[in] pt The per-thread instance used to generate the random deviate.
     *
     * \param[in] k  The degrees of freedom.
     *
     * \return Returns the random value.
     */
    M_PUBLIC_API inline Real chiSquaredDeviate(PerThread& pt, Integer k) {
        return pt.randomChiSquared(k);
    }

    /**
     * Function that calculates a value in a chi-squared distribution.
     *
     * \param[in] pt The per-thread instance used to generate the random deviate.
     *
     * \param[in] k  The degrees of freedom.
     *
     * \return Returns the random value.
     */
    template<typename T1> M_PUBLIC_TEMPLATE_FUNCTION Real chiSquaredDeviate(PerThread& pt, T1 k) {
        Integer lk;
        if (toInteger(lk, k)) {
            return pt.randomChiSquared(lk);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates a value in a chi-squared distribution.
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \param[in] k             The degrees of freedom.
     *
     * \return Returns the random value.
     */
    M_PUBLIC_API inline MatrixReal chiSquaredDeviate(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns,
            Integer    k
        ) {
        return MatrixReal::randomChiSquared(pt, numberRows, numberColumns, k);
    }

    /**
     * Function that calculates a value in a chi-squared distribution.
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \param[in] k             The degrees of freedom.
     *
     * \return Returns the random value.
     */
    template<typename T1, typename T2, typename T3> M_PUBLIC_TEMPLATE_FUNCTION MatrixReal chiSquaredDeviate(
            PerThread& pt,
            T1         numberRows,
            T2         numberColumns,
            T3         k
        ) {
        Integer lNumberRows;
        Integer lNumberColumns;
        Integer lk;
        if (toInteger(lNumberRows, numberRows) && toInteger(lNumberColumns, numberColumns) && toInteger(lk, k)) {
            return MatrixReal::randomChiSquared(pt, lNumberRows, lNumberColumns, lk);
        } else {
            return MatrixReal();
        }
    }

    /**
     * Function that calculates the PDF in a chi-squared distribution.
     *
     * \param[in] x The value to calculate the PDF at.
     *
     * \param[in] k The degrees of freedom.
     *
     * \return Returns the PDF of the chi-squared distribution.
     */
    M_PUBLIC_API Real chiSquaredPdf(Real x, Integer k);

    /**
     * Function that calculates the PDF in a chi-squared distribution.
     *
     * \param[in] x The value to calculate the PDF at.
     *
     * \param[in] k The degrees of freedom.
     *
     * \return Returns the PDF of the chi-squared distribution.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION Real chiSquaredPdf(T1 x, T2 k) {
        Real    lX;
        Integer lk;
        if (toReal(lX, x) && toInteger(lk, k)) {
            return chiSquaredPdf(lX, lk);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates the CDF in the chi-squared distribution.
     *
     * \param[in] x The value to calculate the CDF at.
     *
     * \param[in] k The degrees of freedom.
     *
     * \return Returns the CDF of the chi-squared distribution.
     */
    M_PUBLIC_API Real chiSquaredCdf(Real x, Integer k);

    /**
     * Function that calculates the CDF in a chi-squared distribution.
     *
     * \param[in] x The value to calculate the PDF at.
     *
     * \param[in] k The degrees of freedom.
     *
     * \return Returns the CDF of the chi-squared distribution.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION Real chiSquaredCdf(T1 x, T2 k) {
        Real    lX;
        Integer lk;
        if (toReal(lX, x) && toInteger(lk, k)) {
            return chiSquaredCdf(lX, lk);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates the quantile of the chi-squared distribution.
     *
     * \param[in] p The value to calculate the quantile at.
     *
     * \param[in] k The degrees of freedom.
     *
     * \return Returns the quantile of the chi-squared distribution.
     */
    M_PUBLIC_API Real chiSquaredQuantile(Real p, Integer k);

    /**
     * Function that calculates the quantile of a chi-squared distribution.
     *
     * \param[in] p The value to calculate the quantile at.
     *
     * \param[in] k The shape term.
     *
     * \return Returns the quantile of the chi-squared distribution.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION Real chiSquaredQuantile(T1 p, T2 k) {
        Real    lP;
        Integer lk;
        if (toReal(lP, p) && toInteger(lk, k)) {
            return chiSquaredQuantile(lP, lk);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates a value in a Poisson distribution.
     *
     * \param[in] pt   The per-thread instance used to generate the random deviate.
     *
     * \param[in] rate The event rate.
     *
     * \return Returns the random value.
     */
    M_PUBLIC_API inline Integer poissonDeviate(PerThread& pt, Real rate) {
        return pt.randomPoisson(rate);
    }

    /**
     * Function that calculates a value in a Poisson distribution.
     *
     * \param[in] pt   The per-thread instance used to generate the random deviate.
     *
     * \param[in] rate The event rate.
     *
     * \return Returns the random value.
     */
    template<typename T1> M_PUBLIC_TEMPLATE_FUNCTION Integer poissonDeviate(PerThread& pt, T1 rate) {
        Real lRate;
        if (toReal(lRate, rate)) {
            return pt.randomPoisson(lRate);
        } else {
            return -1;
        }
    }

    /**
     * Function that calculates a value in a Poisson distribution.
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \param[in] rate          The event rate.
     *
     * \return Returns the random value.
     */
    M_PUBLIC_API inline MatrixInteger poissonDeviate(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns,
            Real       rate
        ) {
        return MatrixInteger::randomPoisson(pt, numberRows, numberColumns, rate);
    }

    /**
     * Function that calculates a value in a Poisson distribution.
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \param[in] rate          The event rate.
     *
     * \return Returns the random value.
     */
    template<typename T1, typename T2, typename T3> M_PUBLIC_TEMPLATE_FUNCTION MatrixInteger poissonDeviate(
            PerThread& pt,
            T1         numberRows,
            T2         numberColumns,
            T3         rate
        ) {
        Integer lNumberRows;
        Integer lNumberColumns;
        Real    lRate;
        if (toInteger(lNumberRows, numberRows) && toInteger(lNumberColumns, numberColumns) && toReal(lRate, rate)) {
            return MatrixInteger::randomPoisson(pt, lNumberRows, lNumberColumns, lRate);
        } else {
            return MatrixInteger();
        }
    }

    /**
     * Function that calculates the PMF in a Poisson distribution.
     *
     * \param[in] k    The value to calculate the PMF at.
     *
     * \param[in] rate The event rate.
     *
     * \return Returns the PMF of the Poisson distribution.
     */
    M_PUBLIC_API Real poissonPmf(Integer k, Real rate);

    /**
     * Function that calculates the PMF in a Poisson distribution.
     *
     * \param[in] k    The value to calculate the PMF at.
     *
     * \param[in] rate The event rate.
     *
     * \return Returns the PMF of the Poisson distribution.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION Real poissonPmf(T1 k, T2 rate) {
        Integer lK;
        Real    lRate;
        if (toInteger(lK, k) && toReal(lRate, rate)) {
            return poissonPmf(lK, lRate);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates the CDF in the Poisson distribution.
     *
     * \param[in] k    The value to calculate the CDF at.
     *
     * \param[in] rate The event rate.
     *
     * \return Returns the CDF of the Poisson distribution.
     */
    M_PUBLIC_API Real poissonCdf(Integer k, Real rate);

    /**
     * Function that calculates the CDF in a Poisson distribution.
     *
     * \param[in] k    The value to calculate the CDF at.
     *
     * \param[in] rate The event rate.
     *
     * \return Returns the CDF of the Poisson distribution.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION Real poissonCdf(T1 k, T2 rate) {
        Integer lK;
        Real    lRate;
        if (toInteger(lK, k) && toReal(lRate, rate)) {
            return poissonCdf(lK, lRate);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates a value in a binomial distribution.
     *
     * \param[in] pt   The per-thread instance used to generate the random deviate.
     *
     * \param[in] n    The number of trials.
     *
     * \param[in] p    The probability of success for a given trial.
     *
     * \return Returns the random value.
     */
    M_PUBLIC_API inline Integer binomialDeviate(PerThread& pt, Integer n, Real p) {
        return pt.randomBinomial(n, p);
    }

    /**
     * Function that calculates a value in a binomial distribution.
     *
     * \param[in] pt   The per-thread instance used to generate the random deviate.
     *
     * \param[in] n    The number of trials.
     *
     * \param[in] p    The probability of success for a given trial.
     *
     * \return Returns the random value.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION Integer binomialDeviate(PerThread& pt, T1 n, T2 p) {
        Integer lN;
        Real    lP;
        if (toInteger(lN, n) && toReal(lP, p)) {
            return pt.randomBinomial(lN, lP);
        } else {
            return -1;
        }
    }

    /**
     * Function that calculates a value in a binomial distribution.
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \param[in] n             The number of trials.
     *
     * \param[in] p             The probability of success for a given trial.
     *
     * \return Returns the random value.
     */
    M_PUBLIC_API inline MatrixInteger binomialDeviate(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns,
            Integer    n,
            Real       p
        ) {
        return MatrixInteger::randomBinomial(pt, numberRows, numberColumns, n, p);
    }

    /**
     * Function that calculates a value in a binomial distribution.
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \param[in] n             The number of trials.
     *
     * \param[in] p             The probability of success for a given trial.
     *
     * \return Returns the random value.
     */
    template<typename T1, typename T2, typename T3, typename T4>
        M_PUBLIC_TEMPLATE_FUNCTION MatrixInteger binomialDeviate(
            PerThread& pt,
            T1         numberRows,
            T2         numberColumns,
            T3         n,
            T4         p
        ) {
        Integer lNumberRows;
        Integer lNumberColumns;
        Integer lN;
        Real    lP;
        if (toInteger(lNumberRows, numberRows)       &&
            toInteger(lNumberColumns, numberColumns) &&
            toInteger(lN, n)                         &&
            toReal(lP, p)                               ) {
            return MatrixInteger::randomBinomial(pt, lNumberRows, lNumberColumns, lN, lP);
        } else {
            return MatrixInteger();
        }
    }

    /**
     * Function that calculates the PMF in a binomial distribution.
     *
     * \param[in] k The number of successes.
     *
     * \param[in] n The number of trials.
     *
     * \param[in] p The probability of success for a given trial.
     *
     * \return Returns the PMF of the binomial distribution.
     */
    M_PUBLIC_API Real binomialPmf(Integer k, Integer n, Real p);

    /**
     * Function that calculates the PMF in a binomial distribution.
     *
     * \param[in] k The number of successes.
     *
     * \param[in] n The number of trials.
     *
     * \param[in] p The probability of success for a given trial.
     *
     * \return Returns the PMF of the binomial distribution.
     */
    template<typename T1, typename T2, typename T3> M_PUBLIC_TEMPLATE_FUNCTION Real binomialPmf(T1 k, T2 n, T3 p) {
        Integer lK;
        Integer lN;
        Real    lP;
        if (toInteger(lK, k) && toInteger(lN, n) && toReal(lP, p)) {
            return binomialPmf(lK, lN, lP);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates the CDF in the binomial distribution.
     *
     * \param[in] k The number of successes.
     *
     * \param[in] n The number of trials.
     *
     * \param[in] p The probability of success for a given trial.
     *
     * \return Returns the CDF of the binomial distribution.
     */
    M_PUBLIC_API Real binomialCdf(Integer k, Integer n, Real p);

    /**
     * Function that calculates the CDF in a binomial distribution.
     *
     * \param[in] k The number of successes.
     *
     * \param[in] n The number of trials.
     *
     * \param[in] p The probability of success for a given trial.
     *
     * \return Returns the CDF of the binomial distribution.
     */
    template<typename T1, typename T2, typename T3> M_PUBLIC_TEMPLATE_FUNCTION Real binomialCdf(T1 k, T2 n, T3 p) {
        Integer lK;
        Integer lN;
        Real    lP;
        if (toInteger(lK, k) && toInteger(lN, n) && toReal(lP, p)) {
            return binomialCdf(lK, lN, lP);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates a value in a log normal distribution with mean of 0 and sigma of 1.0.
     *
     * \param[in] pt The per-thread instance used to generate the random deviate.
     *
     * \return Returns the random value.
     */
    M_PUBLIC_API inline Real logNormalDeviate(PerThread& pt) {
        return pt.randomLogNormal();
    }

    /**
     * Function that calculates a value in a log normal distribution with mean of 0 and sigma of 1.0.
     *
     * \param[in] pt    The per-thread instance used to generate the random deviate.
     *
     * \param[in] mean  The mean value
     *
     * \param[in] sigma The standard deviation.
     *
     * \return Returns the random value.
     */
    M_PUBLIC_API inline Real logNormalDeviate(PerThread& pt, Real mean, Real sigma) {
        return pt.randomLogNormal(mean, sigma);
    }

    /**
     * Function that calculates a value in a log normal distribution with mean of 0 and sigma of 1.0.
     *
     * \param[in] pt    The per-thread instance used to generate the random deviate.
     *
     * \param[in] mean  The mean value
     *
     * \param[in] sigma The standard deviation.
     *
     * \return Returns the random value.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION Real logNormalDeviate(
            PerThread& pt,
            T1         mean,
            T2         sigma
        ) {
        Real m;
        Real s;
        if (toReal(m, mean) && toReal(s, sigma)) {
            return pt.randomLogNormal(m, s);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates a value in a log normal distribution with mean of 0 and sigma of 1.0.
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \param[in] mean          The mean value
     *
     * \param[in] sigma         The standard deviation.
     *
     * \return Returns the random value.
     */
    M_PUBLIC_API inline MatrixReal logNormalDeviate(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns,
            Real       mean,
            Real       sigma
        ) {
        return MatrixReal::randomLogNormal(pt, numberRows, numberColumns, mean, sigma);
    }

    /**
     * Function that calculates a value in a log normal distribution with mean of 0 and sigma of 1.0.
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \param[in] mean          The mean value
     *
     * \param[in] sigma         The standard deviation.
     *
     * \return Returns the random value.
     */
    template<typename T1, typename T2, typename T3, typename T4>
        M_PUBLIC_TEMPLATE_FUNCTION MatrixReal logNormalDeviate(
            PerThread& pt,
            T1         numberRows,
            T2         numberColumns,
            T3         mean,
            T4         sigma
        ) {
        Integer nr;
        Integer nc;
        Real    m;
        Real    s;
        if (toInteger(nr, numberRows) && toInteger(nc, numberColumns) && toReal(m, mean) && toReal(s, sigma)) {
            return MatrixReal::randomLogNormal(pt, nr, nc, m, s);
        } else {
            return MatrixReal();
        }
    }

    /**
     * Function that calculates the log normal PDF.
     *
     * \param[in] x     The value to calculate the PDF at.
     *
     * \param[in] mean  The desired mean value.
     *
     * \param[in] sigma The standard deviation.
     *
     * \return Returns the PDF of the log normal distribution.
     */
    M_PUBLIC_API Real logNormalPdf(Real x, Real mean = 0.0, Real sigma = 1.0);

    /**
     * Function that calculates the log normal PDF.
     *
     * \param[in] x     The value to calculate the PDF at.
     *
     * \param[in] mean  The desired mean value.
     *
     * \param[in] sigma The standard deviation.
     *
     * \return Returns the PDF of the log normal distribution.
     */
    template<typename T1, typename T2 = Real, typename T3 = Real> M_PUBLIC_TEMPLATE_FUNCTION Real logNormalPdf(
            T1 x,
            T2 mean = T2(0.0),
            T3 sigma = T3(1.0)
        ) {
        Real tx;
        Real tm;
        Real ts;
        if (toReal(tx, x) && toReal(tm, mean) && toReal(ts, sigma)) {
            return logNormalPdf(tx, tm, ts);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates the log normal CDF.
     *
     * \param[in] x     The value to calculate the CDF at.
     *
     * \param[in] mean  The desired mean value.
     *
     * \param[in] sigma The standard deviation.
     *
     * \return Returns the CDF of the log normal distribution.
     */
    M_PUBLIC_API Real logNormalCdf(Real x, Real mean = 0.0, Real sigma = 1.0);

    /**
     * Function that calculates the log normal CDF.
     *
     * \param[in] x     The value to calculate the CDF at.
     *
     * \param[in] mean  The desired mean value.
     *
     * \param[in] sigma The standard deviation.
     *
     * \return Returns the CDF of the log normal distribution.
     */
    template<typename T1, typename T2 = Real, typename T3 = Real> M_PUBLIC_TEMPLATE_FUNCTION Real logNormalCdf(
            T1 x,
            T2 mean = T2(0.0),
            T3 sigma = T3(1.0)
        ) {
        Real tx;
        Real tm;
        Real ts;
        if (toReal(tx, x) && toReal(tm, mean) && toReal(ts, sigma)) {
            return logNormalCdf(tx, tm, ts);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates the log normal quantile.
     *
     * \param[in] p     The value to calculate the quantile at.
     *
     * \param[in] mean  The desired mean value.
     *
     * \param[in] sigma The standard deviation.
     *
     * \return Returns the quantile of the log normal distribution.
     */
    M_PUBLIC_API Real logNormalQuantile(Real p, Real mean = 0.0, Real sigma = 1.0);

    /**
     * Function that calculates the log normal quantile.
     *
     * \param[in] p     The value to calculate the quantile at.
     *
     * \param[in] mean  The desired mean value.
     *
     * \param[in] sigma The standard deviation.
     *
     * \return Returns the quantile of the log normal distribution.
     */
    template<typename T1, typename T2 = Real, typename T3 = Real> M_PUBLIC_TEMPLATE_FUNCTION Real logNormalQuantile(
            T1 p,
            T2 mean = T2(0.0),
            T3 sigma = T3(1.0)
        ) {
        Real tp;
        Real tm;
        Real ts;
        if (toReal(tp, p) && toReal(tm, mean) && toReal(ts, sigma)) {
            return normalQuantile(tp, tm, ts);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates a value in a geometric distribution.
     *
     * \param[in] pt The per-thread instance used to generate the random deviate.
     *
     * \param[in] p  The success probability.
     *
     * \return Returns the random value.
     */
    M_PUBLIC_API inline Integer geometricDeviate(PerThread& pt, Real p) {
        return pt.randomGeometric(p);
    }

    /**
     * Function that calculates a value in a geometric distribution.
     *
     * \param[in] pt The per-thread instance used to generate the random deviate.
     *
     * \param[in] p  The success probability.
     *
     * \return Returns the random value.
     */
    template<typename T1> M_PUBLIC_TEMPLATE_FUNCTION Integer geometricDeviate(PerThread& pt, T1 p) {
        Real lRate;
        if (toReal(lRate, p)) {
            return pt.randomGeometric(lRate);
        } else {
            return -1;
        }
    }

    /**
     * Function that calculates a value in a geometric distribution.
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \param[in] p             The success probability.
     *
     * \return Returns the random value.
     */
    M_PUBLIC_API inline MatrixInteger geometricDeviate(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns,
            Real       p
        ) {
        return MatrixInteger::randomGeometric(pt, numberRows, numberColumns, p);
    }

    /**
     * Function that calculates a value in a geometric distribution.
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \param[in] p             The success probability.
     *
     * \return Returns the random value.
     */
    template<typename T1, typename T2, typename T3> M_PUBLIC_TEMPLATE_FUNCTION MatrixInteger geometricDeviate(
            PerThread& pt,
            T1         numberRows,
            T2         numberColumns,
            T3         p
        ) {
        Integer lNumberRows;
        Integer lNumberColumns;
        Real    lRate;
        if (toInteger(lNumberRows, numberRows) && toInteger(lNumberColumns, numberColumns) && toReal(lRate, p)) {
            return MatrixInteger::randomGeometric(pt, lNumberRows, lNumberColumns, lRate);
        } else {
            return MatrixInteger();
        }
    }

    /**
     * Function that calculates the PMF in a geometric distribution.
     *
     * \param[in] k The number of trials to calculate the PMF at.
     *
     * \param[in] p The success probability.
     *
     * \return Returns the PMF of the geometric distribution.
     */
    M_PUBLIC_API Real geometricPmf(Integer k, Real p);

    /**
     * Function that calculates the PMF in a geometric distribution.
     *
     * \param[in] k The number of trials to calculate the PMF at.
     *
     * \param[in] p The success probability.
     *
     * \return Returns the PMF of the geometric distribution.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION Real geometricPmf(T1 k, T2 p) {
        Integer lK;
        Real    lRate;
        if (toInteger(lK, k) && toReal(lRate, p)) {
            return geometricPmf(lK, lRate);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates the CDF in the geometric distribution.
     *
     * \param[in] k The number of trials to calculate the CDF at.
     *
     * \param[in] p The success probability.
     *
     * \return Returns the CDF of the geometric distribution.
     */
    M_PUBLIC_API Real geometricCdf(Integer k, Real p);

    /**
     * Function that calculates the CDF in a geometric distribution.
     *
     * \param[in] k The number of trials to calculate the CDF at.
     *
     * \param[in] p The success probability.
     *
     * \return Returns the CDF of the geometric distribution.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION Real geometricCdf(T1 k, T2 p) {
        Integer lK;
        Real    lRate;
        if (toInteger(lK, k) && toReal(lRate, p)) {
            return geometricCdf(lK, lRate);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates a value in a Cauchy-Lorentz distribution.
     *
     * \param[in] pt       The per-thread instance used to generate the random deviate.
     *
     * \param[in] location The location parameter.
     *
     * \param[in] scale    The scale parameter.
     *
     * \return Returns the random value.
     */
    M_PUBLIC_API inline Real cauchyLorentzDeviate(PerThread& pt, Real location, Real scale) {
        return pt.randomCauchyLorentz(location, scale);
    }

    /**
     * Function that calculates a value in a Cauchy-Lorentz distribution.
     *
     * \param[in] pt       The per-thread instance used to generate the random deviate.
     *
     * \param[in] location The location parameter.
     *
     * \param[in] scale    The scale parameter.
     *
     * \return Returns the random value.
     */
    template<typename T1, typename T2>
        M_PUBLIC_TEMPLATE_FUNCTION Real cauchyLorentzDeviate(PerThread& pt, T1 location, T2 scale) {
        Real lLocation;
        Real lScale;
        if (toReal(lLocation, location) && toReal(lScale, scale)) {
            return pt.randomCauchyLorentz(lLocation, lScale);
        } else {
            return -1;
        }
    }

    /**
     * Function that calculates a value in a Cauchy-Lorentz distribution.
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \param[in] location      The location parameter.
     *
     * \param[in] scale         The scale parameter.
     *
     * \return Returns the random value.
     */
    M_PUBLIC_API inline MatrixReal cauchyLorentzDeviate(
            PerThread& pt,
            Integer    numberRows,
            Integer    numberColumns,
            Real       location,
            Real       scale
        ) {
        return MatrixReal::randomCauchyLorentz(pt, numberRows, numberColumns, location, scale);
    }

    /**
     * Function that calculates a value in a Cauchy-Lorentz distribution.
     *
     * \param[in] pt            The per-thread instance used to generate the random deviate.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \param[in] location      The location parameter.
     *
     * \param[in] scale         The scale parameter.
     *
     * \return Returns the random value.
     */
    template<typename T1, typename T2, typename T3, typename T4>
        M_PUBLIC_TEMPLATE_FUNCTION MatrixReal cauchyLorentzDeviate(
            PerThread& pt,
            T1         numberRows,
            T2         numberColumns,
            T3         location,
            T4         scale
        ) {
        Integer lNumberRows;
        Integer lNumberColumns;
        Real    lLocation;
        Real    lScale;
        if (toInteger(lNumberRows, numberRows)       &&
            toInteger(lNumberColumns, numberColumns) &&
            toReal(lLocation, location)              &&
            toReal(lScale, scale)                       ) {
            return MatrixReal::randomCauchyLorentz(pt, lNumberRows, lNumberColumns, lLocation, lScale);
        } else {
            return MatrixReal();
        }
    }

    /**
     * Function that calculates the PDF in a Cauchy-Lorentz distribution.
     *
     * \param[in] x        The value to calculate the PDF at.
     *
     * \param[in] location The location parameter.
     *
     * \param[in] scale    The scale parameter.
     *
     * \return Returns the PDF of the Cauchy-Lorentz distribution.
     */
    M_PUBLIC_API Real cauchyLorentzPdf(Real x, Real location, Real scale);

    /**
     * Function that calculates the PDF in a Cauchy-Lorentz distribution.
     *
     * \param[in] x        The value to calculate the PDF at.
     *
     * \param[in] location The location parameter.
     *
     * \param[in] scale    The scale parameter.
     *
     * \return Returns the PDF of the Cauchy-Lorentz distribution.
     */
    template<typename T1, typename T2, typename T3>
        M_PUBLIC_TEMPLATE_FUNCTION Real cauchyLorentzPdf(T1 x, T2 location, T3 scale) {
        Real lX;
        Real lLocation;
        Real lScale;
        if (toReal(lX, x) && toReal(lLocation, location) && toReal(lScale, scale)) {
            return cauchyLorentzPdf(lX, lLocation, lScale);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates the CDF in the Cauchy-Lorentz distribution.
     *
     * \param[in] x        The value to calculate the PDF at.
     *
     * \param[in] location The location parameter.
     *
     * \param[in] scale    The scale parameter.
     *
     * \return Returns the CDF of the Cauchy-Lorentz distribution.
     */
    M_PUBLIC_API Real cauchyLorentzCdf(Real x, Real location, Real scale);

    /**
     * Function that calculates the CDF in a Cauchy-Lorentz distribution.
     *
     * \param[in] x        The value to calculate the PDF at.
     *
     * \param[in] location The location parameter.
     *
     * \param[in] scale    The scale parameter.
     *
     * \return Returns the CDF of the Cauchy-Lorentz distribution.
     */
    template<typename T1, typename T2, typename T3>
        M_PUBLIC_TEMPLATE_FUNCTION Real cauchyLorentzCdf(T1 x, T2 location, T3 scale) {
        Real lX;
        Real lLocation;
        Real lScale;
        if (toReal(lX, x) && toReal(lLocation, location) && toReal(lScale, scale)) {
            return cauchyLorentzCdf(lX, lLocation, lScale);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates the Cauchy-Lorentz quantile.
     *
     * \param[in] p        The value to calculate the quantile at.
     *
     * \param[in] location The location parameter.
     *
     * \param[in] scale    The scale parameter.
     *
     * \return Returns the quantile of the Cauchy-Lorentz distribution.
     */
    M_PUBLIC_API Real cauchyLorentzQuantile(Real p, Real location, Real scale);

    /**
     * Function that calculates the Cauchy-Lorentz quantile.
     *
     * \param[in] p        The value to calculate the quantile at.
     *
     * \param[in] location The location parameter.
     *
     * \param[in] scale    The scale parameter.
     *
     * \return Returns the quantile of the Cauchy-Lorentz distribution.
     */
    template<typename T1, typename T2, typename T3> M_PUBLIC_TEMPLATE_FUNCTION Real cauchyLorentzQuantile(
            T1 p,
            T2 location,
            T3 scale
        ) {
        Real lP;
        Real lLocation;
        Real lScale;
        if (toReal(lP, p) && toReal(lLocation, location) && toReal(lScale, scale)) {
            return cauchyLorentzQuantile(lP, lLocation, lScale);
        } else {
            return NaN;
        }
    }

    /**
     * Function that calculates the count of scalar values.
     *
     * \return Returns the count of scalar values.
     */
    M_PUBLIC_API inline Integer count() {
        return 0;
    }

    /**
     * Function that calculates the count of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the count of scalar values.
     */
    M_PUBLIC_API inline Integer count(Boolean value) {
        (void) value;
        return 1;
    }

    /**
     * Function that calculates the count of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the count of scalar values.
     */
    M_PUBLIC_API inline Integer count(Integer value) {
        (void) value;
        return 1;
    }

    /**
     * Function that calculates the count of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the count of scalar values.
     */
    M_PUBLIC_API inline Integer count(Real value) {
        (void) value;
        return 1;
    }

    /**
     * Function that calculates the count of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the count of scalar values.
     */
    M_PUBLIC_API inline Integer count(const Complex& value) {
        (void) value;
        return 1;
    }

    /**
     * Function that calculates the count of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the count of scalar values.
     */
    M_PUBLIC_API Integer count(const Set& value);

    /**
     * Function that calculates the count of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the count of scalar values.
     */
    M_PUBLIC_API Integer count(const Tuple& value);

    /**
     * Function that calculates the count of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the count of scalar values.
     */
    M_PUBLIC_API inline Integer count(const MatrixBoolean& value) {
        return value.numberCoefficients();
    }

    /**
     * Function that calculates the count of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the count of scalar values.
     */
    M_PUBLIC_API inline Integer count(const MatrixInteger& value) {
        return value.numberCoefficients();
    }

    /**
     * Function that calculates the count of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the count of scalar values.
     */
    M_PUBLIC_API inline Integer count(const MatrixReal& value) {
        return value.numberCoefficients();
    }

    /**
     * Function that calculates the count of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the count of scalar values.
     */
    M_PUBLIC_API inline Integer count(const MatrixComplex& value) {
        return value.numberCoefficients();
    }

    /**
     * Function that calculates the count of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the count of scalar values.
     */
    M_PUBLIC_API Integer count(const Variant& value);


    /**
     * Template function that calculates the count of scalar values.
     *
     * \param[in] first      The first parameter.
     *
     * \param[in] additional The additional parameters.
     *
     * \return Returns the count of scalar values.
     */
    template<typename First, typename... Additional> M_PUBLIC_TEMPLATE_FUNCTION Integer count(
            const First&         first,
            const Additional&... additional
        ) {
        return count(first) + count(additional...);
    }

    /**
     * Function that calculates the sum of scalar values.
     *
     * \return Returns the sum of scalar values.
     */
    M_PUBLIC_API Complex sum();

    /**
     * Function that calculates the sum of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of scalar values.
     */
    M_PUBLIC_API Complex sum(Boolean value);

    /**
     * Function that calculates the sum of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of scalar values.
     */
    M_PUBLIC_API Complex sum(Integer value);

    /**
     * Function that calculates the sum of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of scalar values.
     */
    M_PUBLIC_API Complex sum(Real value);

    /**
     * Function that calculates the sum of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of scalar values.
     */
    M_PUBLIC_API inline Complex sum(const Complex& value) {
        return value;
    }

    /**
     * Function that calculates the sum of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of scalar values.
     */
    M_PUBLIC_API Complex sum(const Set& value);

    /**
     * Function that calculates the sum of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of scalar values.
     */
    M_PUBLIC_API Complex sum(const Tuple& value);

    /**
     * Function that calculates the sum of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of scalar values.
     */
    M_PUBLIC_API Complex sum(const MatrixBoolean& value);

    /**
     * Function that calculates the sum of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of scalar values.
     */
    M_PUBLIC_API Complex sum(const MatrixInteger& value);

    /**
     * Function that calculates the sum of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of scalar values.
     */
    M_PUBLIC_API Complex sum(const MatrixReal& value);

    /**
     * Function that calculates the sum of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of scalar values.
     */
    M_PUBLIC_API Complex sum(const MatrixComplex& value);

    /**
     * Function that calculates the sum of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of scalar values.
     */
    M_PUBLIC_API Complex sum(const Variant& value);

    /**
     * Template function that calculates the sum of scalar values.
     *
     * \param[in] first      The first parameter.
     *
     * \param[in] additional The additional parameters.
     *
     * \return Returns the sum of scalar values.
     */
    template<typename First, typename... Additional> M_PUBLIC_TEMPLATE_FUNCTION Complex sum(
            const First&         first,
            const Additional&... additional
        ) {
        return sum(first) + sum(additional...);
    }

    /**
     * Template function that calculates the average of scalar values.
     *
     * \param[in] values The first parameter.
     *
     * \return Returns the sum of scalar values.
     */
    template<typename... Values> M_PUBLIC_TEMPLATE_FUNCTION Complex avg(const Values&... values) {
        Integer numberTerms = count(values...);
        Complex sumValues   = sum(values...);

        return sumValues / static_cast<Real>(numberTerms);
    }

    /**
     * Function that calculates the minimum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the minimum value of scalar values.
     */
    M_PUBLIC_API inline bool internalHasValue(Integer value) {
        (void) value;
        return true;
    }

    /**
     * Function that calculates the minimum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the minimum value of scalar values.
     */
    M_PUBLIC_API inline bool internalHasValue(Real value) {
        (void) value;
        return true;
    }

    /**
     * Function that calculates the minimum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the minimum value of scalar values.
     */
    M_PUBLIC_API inline bool internalHasValue(const Complex& value) {
        (void) value;
        return true;
    };

    /**
     * Function that calculates the minimum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the minimum value of scalar values.
     */
    M_PUBLIC_API bool internalHasValue(const Set& value);

    /**
     * Function that calculates the minimum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the minimum value of scalar values.
     */
    M_PUBLIC_API bool internalHasValue(const Tuple& value);

    /**
     * Function that calculates the minimum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the minimum value of scalar values.
     */
    M_PUBLIC_API inline bool internalHasValue(const MatrixBoolean& value) {
        return value.numberCoefficients() != 0;
    }

    /**
     * Function that calculates the minimum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the minimum value of scalar values.
     */
    M_PUBLIC_API inline bool internalHasValue(const MatrixInteger& value) {
        return value.numberCoefficients() != 0;
    }

    /**
     * Function that calculates the minimum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the minimum value of scalar values.
     */
    M_PUBLIC_API inline bool internalHasValue(const MatrixReal& value) {
        return value.numberCoefficients() != 0;
    }

    /**
     * Function that calculates the minimum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the minimum value of scalar values.
     */
    M_PUBLIC_API inline bool internalHasValue(const MatrixComplex& value) {
        return value.numberCoefficients() != 0;
    }

    /**
     * Function that calculates the minimum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the minimum value of scalar values.
     */
    M_PUBLIC_API bool internalHasValue(const Variant& value);

    /**
     * Function that calculates the minimum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the minimum value of scalar values.
     */
    M_PUBLIC_API inline bool internalHasValue(Boolean value) {
        (void) value;
        return true;
    }

    /**
     * Template function that calculates the minimum value of scalar values.
     *
     * \param[in] first      The first value.
     *
     * \param[in] additional The additional values.
     *
     * \return Returns the minimum value of scalar values.
     */
    template<typename First, typename... Additional> M_PUBLIC_TEMPLATE_FUNCTION bool internalHasValue(
            const First&         first,
            const Additional&... additional
        ) {
        return internalHasValue(first) || internalHasValue(additional...);
    }

    /**
     * Function that calculates the minimum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the minimum value of scalar values.
     */
    M_PUBLIC_API inline Real internalMinHelper(Boolean value) {
        return static_cast<Real>(value);
    }

    /**
     * Function that calculates the minimum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the minimum value of scalar values.
     */
    M_PUBLIC_API inline Real internalMinHelper(Integer value) {
        return static_cast<Real>(value);
    }

    /**
     * Function that calculates the minimum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the minimum value of scalar values.
     */
    M_PUBLIC_API inline Real internalMinHelper(Real value) {
        return value;
    }

    /**
     * Function that calculates the minimum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the minimum value of scalar values.
     */
    M_PUBLIC_API Real internalMinHelper(const Complex& value);

    /**
     * Function that calculates the minimum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the minimum value of scalar values.
     */
    M_PUBLIC_API Real internalMinHelper(const Set& value);

    /**
     * Function that calculates the minimum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the minimum value of scalar values.
     */
    M_PUBLIC_API Real internalMinHelper(const Tuple& value);

    /**
     * Function that calculates the minimum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the minimum value of scalar values.
     */
    M_PUBLIC_API Real internalMinHelper(const MatrixBoolean& value);

    /**
     * Function that calculates the minimum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the minimum value of scalar values.
     */
    M_PUBLIC_API Real internalMinHelper(const MatrixInteger& value);

    /**
     * Function that calculates the minimum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the minimum value of scalar values.
     */
    M_PUBLIC_API Real internalMinHelper(const MatrixReal& value);

    /**
     * Function that calculates the minimum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the minimum value of scalar values.
     */
    M_PUBLIC_API Real internalMinHelper(const MatrixComplex& value);

    /**
     * Function that calculates the minimum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the minimum value of scalar values.
     */
    M_PUBLIC_API Real internalMinHelper(const Variant& value);

    /**
     * Template function that calculates the minimum value of scalar values.
     *
     * \param[in] first      The first value.
     *
     * \param[in] additional The additional values.
     *
     * \return Returns the minimum value of scalar values.
     */
    template<typename First, typename... Additional> M_PUBLIC_TEMPLATE_FUNCTION Real internalMinHelper(
            const First&         first,
            const Additional&... additional
        ) {
        Real firstMinimum      = internalMinHelper(first);
        Real additionalMinimum = internalMinHelper(additional...);

        Real result;
        if (isNaN(firstMinimum) || isNaN(additionalMinimum)) {
            result = NaN;
        } else {
            result = firstMinimum < additionalMinimum ? firstMinimum : additionalMinimum;
        }

        return result;
    }

    /**
     * Template function that calculates the minimum value of scalar values.
     *
     * \param[in] values The values.
     *
     * \return Returns the minimum value of scalar values.
     */
    template<typename... ValueTypes> M_PUBLIC_TEMPLATE_FUNCTION Real min(const ValueTypes&... values) {
        Real result;

        if (internalHasValue(values...)) {
            result = internalMinHelper(values...);
        } else {
            internalLocalTriggerNaNError();
            result = NaN;
        }

        return result;
    }

    /**
     * Function that calculates the maximum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the maximum value of scalar values.
     */
    M_PUBLIC_API inline Real internalMaxHelper(Boolean value) {
        return static_cast<Real>(value);
    }

    /**
     * Function that calculates the maximum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the maximum value of scalar values.
     */
    M_PUBLIC_API inline Real internalMaxHelper(Integer value) {
        return static_cast<Real>(value);
    }

    /**
     * Function that calculates the maximum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the maximum value of scalar values.
     */
    M_PUBLIC_API inline Real internalMaxHelper(Real value) {
        return value;
    }

    /**
     * Function that calculates the maximum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the maximum value of scalar values.
     */
    M_PUBLIC_API Real internalMaxHelper(const Complex& value);

    /**
     * Function that calculates the maximum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the maximum value of scalar values.
     */
    M_PUBLIC_API Real internalMaxHelper(const Set& value);

    /**
     * Function that calculates the maximum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the maximum value of scalar values.
     */
    M_PUBLIC_API Real internalMaxHelper(const Tuple& value);

    /**
     * Function that calculates the maximum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the maximum value of scalar values.
     */
    M_PUBLIC_API Real internalMaxHelper(const MatrixBoolean& value);

    /**
     * Function that calculates the maximum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the maximum value of scalar values.
     */
    M_PUBLIC_API Real internalMaxHelper(const MatrixInteger& value);

    /**
     * Function that calculates the maximum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the maximum value of scalar values.
     */
    M_PUBLIC_API Real internalMaxHelper(const MatrixReal& value);

    /**
     * Function that calculates the maximum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the maximum value of scalar values.
     */
    M_PUBLIC_API Real internalMaxHelper(const MatrixComplex& value);

    /**
     * Function that calculates the maximum value of scalar values.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the maximum value of scalar values.
     */
    M_PUBLIC_API Real internalMaxHelper(const Variant& value);

    /**
     * Template function that calculates the maximum value of scalar values.
     *
     * \param[in] first      The first value.
     *
     * \param[in] additional The additional values.
     *
     * \return Returns the maximum value of scalar values.
     */
    template<typename First, typename... Additional> M_PUBLIC_TEMPLATE_FUNCTION Real internalMaxHelper(
            const First&         first,
            const Additional&... additional
        ) {
        Real firstMaximum      = internalMaxHelper(first);
        Real additionalMaximum = internalMaxHelper(additional...);

        Real result;
        if (isNaN(firstMaximum) || isNaN(additionalMaximum)) {
            result = NaN;
        } else {
            result = firstMaximum > additionalMaximum ? firstMaximum : additionalMaximum;
        }

        return result;
    }

    /**
     * Template function that calculates the maximum value of scalar values.
     *
     * \param[in] values The values.
     *
     * \return Returns the maximum value of scalar values.
     */
    template<typename... ValueTypes> M_PUBLIC_TEMPLATE_FUNCTION Real max(const ValueTypes&... values) {
        Real result;

        if (internalHasValue(values...)) {
            result = internalMaxHelper(values...);
        } else {
            internalLocalTriggerNaNError();
            result = NaN;
        }

        return result;
    }

    /**
     * Function that calculates the sum of the squares of difference of the scalar values and the mean.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the squares of scalar values.
     */
    M_PUBLIC_API Real internalStdDevHelper(Real mean, Boolean value);

    /**
     * Function that calculates the sum of the squares of scalar values.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the squares of scalar values.
     */
    M_PUBLIC_API Real internalStdDevHelper(Real mean, Integer value);

    /**
     * Function that calculates the sum of the squares of scalar values.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the squares of scalar values.
     */
    M_PUBLIC_API Real internalStdDevHelper(Real mean, Real value);

    /**
     * Function that calculates the sum of the squares of scalar values.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the squares of scalar values.
     */
    M_PUBLIC_API Real internalStdDevHelper(Real mean, const Complex& value);

    /**
     * Function that calculates the sum of the squares of scalar values.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the squares of scalar values.
     */
    M_PUBLIC_API Real internalStdDevHelper(Real mean, const Set& value);

    /**
     * Function that calculates the sum of the squares of scalar values.
     *
     * \param[in] mean  The current mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the squares of scalar values.
     */
    M_PUBLIC_API Real internalStdDevHelper(Real mean, const Tuple& value);

    /**
     * Function that calculates the sum of the squares of scalar values.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the squares of scalar values.
     */
    M_PUBLIC_API Real internalStdDevHelper(Real mean, const MatrixBoolean& value);

    /**
     * Function that calculates the sum of the squares of scalar values.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the squares of scalar values.
     */
    M_PUBLIC_API Real internalStdDevHelper(Real mean, const MatrixInteger& value);

    /**
     * Function that calculates the sum of the squares of scalar values.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the squares of scalar values.
     */
    M_PUBLIC_API Real internalStdDevHelper(Real mean, const MatrixReal& value);

    /**
     * Function that calculates the sum of the squares of scalar values.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the squares of scalar values.
     */
    M_PUBLIC_API Real internalStdDevHelper(Real mean, const MatrixComplex& value);

    /**
     * Function that calculates the sum of the squares of scalar values.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the squares of scalar values.
     */
    M_PUBLIC_API Real internalStdDevHelper(Real mean, const Variant& value);

    /**
     * Template function that calculates the sum of the squares of scalar values.
     *
     * \param[in] mean       The mean value.
     *
     * \param[in] first      The first value.
     *
     * \param[in] additional The additional values.
     *
     * \return Returns the minimum value of scalar values.
     */
    template<typename First, typename... Additional> M_PUBLIC_TEMPLATE_FUNCTION Real internalStdDevHelper(
            Real                 mean,
            const First&         first,
            const Additional&... additional
        ) {
        return internalStdDevHelper(mean, first) + internalStdDevHelper(mean, additional...);
    }

    /**
     * Template function that calculates the variance of scalar values.
     *
     * \param[in] values The values to be calculated.
     *
     * \return Returns the variance of the requested values.
     */
    template<typename... ValueTypes> M_PUBLIC_TEMPLATE_FUNCTION Real variance(const ValueTypes&... values) {
        Real    result;

        Integer numberValues = count(values...);
        if (numberValues == 0) {
            internalLocalTriggerNaNError();
            result = NaN;
        } else {
            Real mean       = sum(values...).real() / static_cast<Real>(numberValues);
            Real sumSquares = internalStdDevHelper(mean, values...);

            result = sumSquares / static_cast<Real>(numberValues);
        }

        return result;
    }

    /**
     * Template function that calculates the standard deviation of scalar values.
     *
     * \param[in] values The values to be calculated.
     *
     * \return Returns the standard deviation of the requested values.
     */
    template<typename... ValueTypes> M_PUBLIC_TEMPLATE_FUNCTION Real stdDev(const ValueTypes&... values) {
        return M::sqrt(variance(values...)).real();
    }

    /**
     * Template function that calculates the sample variance of scalar values.
     *
     * \param[in] values The values to be calculated.
     *
     * \return Returns the standard deviation of the requested values.
     */
    template<typename... ValueTypes> M_PUBLIC_TEMPLATE_FUNCTION Real sampleStdDev(const ValueTypes&... values) {
        Real    result;

        Integer numberValues = count(values...);
        if (numberValues == 0) {
            internalLocalTriggerNaNError();
            result = NaN;
        } else {
            Real mean       = sum(values...).real() / static_cast<Real>(numberValues);
            Real sumSquares = internalStdDevHelper(mean, values...);

            result = M::sqrt(sumSquares / static_cast<Real>(numberValues - 1)).real();
        }

        return result;
    }

    /**
     * Function that adds a real value to a list.
     *
     * \param[in] list  The list to receive the new value.
     *
     * \param[in] value The value to be added.
     *
     * \return Returns true on success, returns false if the value could not be converted to a real value.
     */
    M_PUBLIC_API inline bool internalBuildRealList(List<Real>& list, Boolean value) {
        list.append(value ? 1.0 : 0.0);
        return true;
    }

    /**
     * Function that adds a real value to a list.
     *
     * \param[in] list  The list to receive the new value.
     *
     * \param[in] value The value to be added.
     *
     * \return Returns true on success, returns false if the value could not be converted to a real value.
     */
    M_PUBLIC_API inline bool internalBuildRealList(List<Real>& list, Integer value) {
        list.append(static_cast<Real>(value));
        return true;
    }

    /**
     * Function that adds a real value to a list.
     *
     * \param[in] list  The list to receive the new value.
     *
     * \param[in] value The value to be added.
     *
     * \return Returns true on success, returns false if the value could not be converted to a real value.
     */
    M_PUBLIC_API inline bool internalBuildRealList(List<Real>& list, Real value) {
        list.append(value);
        return true;
    }

    /**
     * Function that adds a real value to a list.
     *
     * \param[in] list  The list to receive the new value.
     *
     * \param[in] value The value to be added.
     *
     * \return Returns true on success, returns false if the value could not be converted to a real value.
     */
    M_PUBLIC_API bool internalBuildRealList(List<Real>& list, const Complex& value);

    /**
     * Function that adds a real value to a list.
     *
     * \param[in] list  The list to receive the new value.
     *
     * \param[in] value The value to be added.
     *
     * \return Returns true on success, returns false if the value could not be converted to a real value.
     */
    M_PUBLIC_API bool internalBuildRealList(List<Real>& list, const Set& value);

    /**
     * Function that adds a real value to a list.
     *
     * \param[in] list  The list to receive the new value.
     *
     * \param[in] value The value to be added.
     *
     * \return Returns true on success, returns false if the value could not be converted to a real value.
     */
    M_PUBLIC_API bool internalBuildRealList(List<Real>& list, const Tuple& value);

    /**
     * Function that adds a real value to a list.
     *
     * \param[in] list  The list to receive the new value.
     *
     * \param[in] value The value to be added.
     *
     * \return Returns true on success, returns false if the value could not be converted to a real value.
     */
    M_PUBLIC_API bool internalBuildRealList(List<Real>& list, const MatrixBoolean& value);

    /**
     * Function that adds a real value to a list.
     *
     * \param[in] list  The list to receive the new value.
     *
     * \param[in] value The value to be added.
     *
     * \return Returns true on success, returns false if the value could not be converted to a real value.
     */
    M_PUBLIC_API bool internalBuildRealList(List<Real>& list, const MatrixInteger& value);

    /**
     * Function that adds a real value to a list.
     *
     * \param[in] list  The list to receive the new value.
     *
     * \param[in] value The value to be added.
     *
     * \return Returns true on success, returns false if the value could not be converted to a real value.
     */
    M_PUBLIC_API bool internalBuildRealList(List<Real>& list, const MatrixReal& value);

    /**
     * Function that adds a real value to a list.
     *
     * \param[in] list  The list to receive the new value.
     *
     * \param[in] value The value to be added.
     *
     * \return Returns true on success, returns false if the value could not be converted to a real value.
     */
    M_PUBLIC_API bool internalBuildRealList(List<Real>& list, const MatrixComplex& value);

    /**
     * Function that adds a real value to a list.
     *
     * \param[in] list  The list to receive the new value.
     *
     * \param[in] value The value to be added.
     *
     * \return Returns true on success, returns false if the value could not be converted to a real value.
     */
    M_PUBLIC_API bool internalBuildRealList(List<Real>& list, const Variant& value);

    /**
     * Template function that adds a real value to a list.
     *
     * \param[in] list       The list to receive the new value.
     *
     * \param[in] first      The first value.
     *
     * \param[in] additional The additional values.
     *
     * \return Returns true on success, returns false if the value could not be converted to a real value.
     */
    template<typename First, typename... Additional> M_PUBLIC_TEMPLATE_FUNCTION bool internalBuildRealList(
            List<Real>&          list,
            const First&         first,
            const Additional&... additional
        ) {
        return internalBuildRealList(list, first) && internalBuildRealList(list, additional...);
    }

    /**
     * Method that calculates the mediam from a list.
     *
     * \param[in] list The list to use to calculate the media value from.
     *
     * \return Returns the median of the list.
     */
    M_PUBLIC_API Real internalCalculateMedian(const List<Real>& list);

    /**
     * Template function that calculates the median of scalar values.
     *
     * \param[in] values The values to be calculated.
     *
     * \return Returns the median of the requested values.
     */
    template<typename... ValueTypes> M_PUBLIC_TEMPLATE_FUNCTION Real median(const ValueTypes&... values) {
        Real result;

        List<Real> list;
        if (internalBuildRealList(list, values...)) {
            result = internalCalculateMedian(list);
        } else {
            internalLocalTriggerNaNError();
            result = NaN;
        }

        return result;
    }

    /**
     * Function that adds a real value to a list.
     *
     * \param[in] list  The list to receive the new value.
     *
     * \param[in] value The value to be added.
     */
    M_PUBLIC_API inline void internalBuildComplexList(List<Complex>& list, Boolean value) {
        list.append(value ? 1.0 : 0.0);
    }

    /**
     * Function that adds a real value to a list.
     *
     * \param[in] list  The list to receive the new value.
     *
     * \param[in] value The value to be added.
     */
    M_PUBLIC_API inline void internalBuildComplexList(List<Complex>& list, Integer value) {
        list.append(value);
    }

    /**
     * Function that adds a real value to a list.
     *
     * \param[in] list  The list to receive the new value.
     *
     * \param[in] value The value to be added.
     */
    M_PUBLIC_API inline void internalBuildComplexList(List<Complex>& list, Real value) {
        list.append(value);
    }

    /**
     * Function that adds a real value to a list.
     *
     * \param[in] list  The list to receive the new value.
     *
     * \param[in] value The value to be added.
     */
    M_PUBLIC_API inline void internalBuildComplexList(List<Complex>& list, const Complex& value) {
        list.append(value);
    }

    /**
     * Function that adds a real value to a list.
     *
     * \param[in] list  The list to receive the new value.
     *
     * \param[in] value The value to be added.
     */
    M_PUBLIC_API void internalBuildComplexList(List<Complex>& list, const Set& value);

    /**
     * Function that adds a real value to a list.
     *
     * \param[in] list  The list to receive the new value.
     *
     * \param[in] value The value to be added.
     */
    M_PUBLIC_API void internalBuildComplexList(List<Complex>& list, const Tuple& value);

    /**
     * Function that adds a real value to a list.
     *
     * \param[in] list  The list to receive the new value.
     *
     * \param[in] value The value to be added.
     */
    M_PUBLIC_API void internalBuildComplexList(List<Complex>& list, const MatrixBoolean& value);

    /**
     * Function that adds a real value to a list.
     *
     * \param[in] list  The list to receive the new value.
     *
     * \param[in] value The value to be added.
     */
    M_PUBLIC_API void internalBuildComplexList(List<Complex>& list, const MatrixInteger& value);

    /**
     * Function that adds a real value to a list.
     *
     * \param[in] list  The list to receive the new value.
     *
     * \param[in] value The value to be added.
     */
    M_PUBLIC_API void internalBuildComplexList(List<Complex>& list, const MatrixReal& value);

    /**
     * Function that adds a real value to a list.
     *
     * \param[in] list  The list to receive the new value.
     *
     * \param[in] value The value to be added.
     */
    M_PUBLIC_API void internalBuildComplexList(List<Complex>& list, const MatrixComplex& value);

    /**
     * Function that adds a real value to a list.
     *
     * \param[in] list  The list to receive the new value.
     *
     * \param[in] value The value to be added.
     */
    M_PUBLIC_API void internalBuildComplexList(List<Complex>& list, const Variant& value);

    /**
     * Template function that adds a real value to a list.
     *
     * \param[in] list       The list to receive the new value.
     *
     * \param[in] first      The first value.
     *
     * \param[in] additional The additional values.
     */
    template<typename First, typename... Additional> M_PUBLIC_TEMPLATE_FUNCTION void internalBuildComplexList(
            List<Complex>&       list,
            const First&         first,
            const Additional&... additional
        ) {
        internalBuildComplexList(list, first);
        internalBuildComplexList(list, additional...);
    }

    /**
     * Method that calculates the mode from a list.
     *
     * \param[in] list The list to use to calculate the mode value from.
     *
     * \return Returns the median of the list.
     */
    M_PUBLIC_API Complex internalCalculateMode(const List<Complex>& list);

    /**
     * Template function that calculates the median of scalar values.
     *
     * \param[in] values The values to be calculated.
     *
     * \return Returns the mode of the requested values.
     */
    template<typename... ValueTypes> M_PUBLIC_TEMPLATE_FUNCTION Complex mode(const ValueTypes&... values) {
        List<Complex> list;

        internalBuildComplexList(list, values...);
        Complex result = internalCalculateMode(list);

        return result;
    }

    /**
     * Function that calculates the sum of the cubes of the difference between the values and the mean.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the cubes of the difference between the values and the mean.
     */
    M_PUBLIC_API Real internalSkewHelper(Real mean, Boolean value);

    /**
     * Function that calculates the sum of the cubes of the difference between the values and the mean.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the cubes of the difference between the values and the mean.
     */
    M_PUBLIC_API Real internalSkewHelper(Real mean, Integer value);

    /**
     * Function that calculates the sum of the cubes of the difference between the values and the mean.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the cubes of the difference between the values and the mean.
     */
    M_PUBLIC_API Real internalSkewHelper(Real mean, Real value);

    /**
     * Function that calculates the sum of the cubes of the difference between the values and the mean.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the cubes of the difference between the values and the mean.
     */
    M_PUBLIC_API Real internalSkewHelper(Real mean, const Complex& value);

    /**
     * Function that calculates the sum of the cubes of the difference between the values and the mean.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the cubes of the difference between the values and the mean.
     */
    M_PUBLIC_API Real internalSkewHelper(Real mean, const Set& value);

    /**
     * Function that calculates the sum of the cubes of the difference between the values and the mean.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the cubes of the difference between the values and the mean.
     */
    M_PUBLIC_API Real internalSkewHelper(Real mean, const Tuple& value);

    /**
     * Function that calculates the sum of the cubes of the difference between the values and the mean.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the cubes of the difference between the values and the mean.
     */
    M_PUBLIC_API Real internalSkewHelper(Real mean, const MatrixBoolean& value);

    /**
     * Function that calculates the sum of the cubes of the difference between the values and the mean.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the cubes of the difference between the values and the mean.
     */
    M_PUBLIC_API Real internalSkewHelper(Real mean, const MatrixInteger& value);

    /**
     * Function that calculates the sum of the cubes of the difference between the values and the mean.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the cubes of the difference between the values and the mean.
     */
    M_PUBLIC_API Real internalSkewHelper(Real mean, const MatrixReal& value);

    /**
     * Function that calculates the sum of the cubes of the difference between the values and the mean.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the cubes of the difference between the values and the mean.
     */
    M_PUBLIC_API Real internalSkewHelper(Real mean, const MatrixComplex& value);

    /**
     * Function that calculates the sum of the cubes of the difference between the values and the mean.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the cubes of the difference between the values and the mean.
     */
    M_PUBLIC_API Real internalSkewHelper(Real mean, const Variant& value);

    /**
     * Template function that calculates the sum of the cubes of the difference between the values and the mean.
     *
     * \param[in] mean       The mean value.
     *
     * \param[in] first      The first value.
     *
     * \param[in] additional The additional values.
     *
     * \return Returns the minimum value of scalar values.
     */
    template<typename First, typename... Additional> M_PUBLIC_TEMPLATE_FUNCTION Real internalSkewHelper(
            Real                 mean,
            const First&         first,
            const Additional&... additional
        ) {
        return internalSkewHelper(mean, first) + internalSkewHelper(mean, additional...);
    }

    /**
     * Method that calculates values for the skew function.
     *
     * \param[in] numberTerms The number of terms.
     *
     * \param[in] sumValues   The sum of the values.
     *
     * \param[in] sumSquares  The sum of the squares of the values.
     *
     * \param[in] sumCubes    The sum of the cubes of the difference from the average.
     *
     * \return Returns the calculated skew.
     */
    M_PUBLIC_API Real internalSampleSkewCalculator(
        Real numberTerms,
        Real sumValues,
        Real sumSquares,
        Real sumCubes
    );

    /**
     * Template function that calculates the skew of scalar values.
     *
     * \param[in] values The values to be calculated.
     *
     * \return Returns the sample skew of the requested values.
     */
    template<typename... ValueTypes> M_PUBLIC_TEMPLATE_FUNCTION Real sampleSkew(const ValueTypes&... values) {
        Real result;

        Integer numberTerms = count(values...);
        if (numberTerms <= 0) {
            internalLocalTriggerNaNError();
            result = NaN;
        } else if (numberTerms == 1) {
            result = 0;
        } else {
            Real sumValues   = sum(values...).real();
            Real mean        = sumValues / static_cast<Real>(numberTerms);
            Real sumSquares  = internalStdDevHelper(mean, values...);
            Real sumCubes    = internalSkewHelper(mean, values...);

            result = internalSampleSkewCalculator(numberTerms, sumValues, sumSquares, sumCubes);
        }

        return result;
    }

    /**
     * Function that calculates the sum of the difference between the values and the mean to the fourth power.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the difference between the values and the mean to the fourth power.
     */
    M_PUBLIC_API Real internalKurtosisHelper(Real mean, Boolean value);

    /**
     * Function that calculates the sum of the difference between the values and the mean to the fourth power.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the difference between the values and the mean to the fourth power.
     */
    M_PUBLIC_API Real internalKurtosisHelper(Real mean, Integer value);

    /**
     * Function that calculates the sum of the difference between the values and the mean to the fourth power.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the difference between the values and the mean to the fourth power.
     */
    M_PUBLIC_API Real internalKurtosisHelper(Real mean, Real value);

    /**
     * Function that calculates the sum of the difference between the values and the mean to the fourth power.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the difference between the values and the mean to the fourth power.
     */
    M_PUBLIC_API Real internalKurtosisHelper(Real mean, const Complex& value);

    /**
     * Function that calculates the sum of the difference between the values and the mean to the fourth power.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the difference between the values and the mean to the fourth power.
     */
    M_PUBLIC_API Real internalKurtosisHelper(Real mean, const Set& value);

    /**
     * Function that calculates the sum of the difference between the values and the mean to the fourth power.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the difference between the values and the mean to the fourth power.
     */
    M_PUBLIC_API Real internalKurtosisHelper(Real mean, const Tuple& value);

    /**
     * Function that calculates the sum of the difference between the values and the mean to the fourth power.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the difference between the values and the mean to the fourth power.
     */
    M_PUBLIC_API Real internalKurtosisHelper(Real mean, const MatrixBoolean& value);

    /**
     * Function that calculates the sum of the difference between the values and the mean to the fourth power.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the difference between the values and the mean to the fourth power.
     */
    M_PUBLIC_API Real internalKurtosisHelper(Real mean, const MatrixInteger& value);

    /**
     * Function that calculates the sum of the difference between the values and the mean to the fourth power.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the difference between the values and the mean to the fourth power.
     */
    M_PUBLIC_API Real internalKurtosisHelper(Real mean, const MatrixReal& value);

    /**
     * Function that calculates the sum of the difference between the values and the mean to the fourth power.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the difference between the values and the mean to the fourth power.
     */
    M_PUBLIC_API Real internalKurtosisHelper(Real mean, const MatrixComplex& value);

    /**
     * Function that calculates the sum of the difference between the values and the mean to the fourth power.
     *
     * \param[in] mean  The mean value.
     *
     * \param[in] value The value to query.
     *
     * \return Returns the sum of the difference between the values and the mean to the fourth power.
     */
    M_PUBLIC_API Real internalKurtosisHelper(Real mean, const Variant& value);

    /**
     * Template function that calculates the sum of the difference between the values and the mean to the fourth power.
     *
     * \param[in] mean       The mean value.
     *
     * \param[in] first      The first value.
     *
     * \param[in] additional The additional values.
     *
     * \return Returns the minimum value of scalar values.
     */
    template<typename First, typename... Additional> M_PUBLIC_TEMPLATE_FUNCTION Real internalKurtosisHelper(
            Real                 mean,
            const First&         first,
            const Additional&... additional
        ) {
        return internalKurtosisHelper(mean, first) + internalKurtosisHelper(mean, additional...);
    }

    /**
     * Method that calculates values for the excess kurtosis function.
     *
     * \param[in] numberTerms The number of terms.
     *
     * \param[in] sumValues   The sum of the values.
     *
     * \param[in] sumSquares  The sum of the squares of the values.
     *
     * \param[in] sumQuads    The sum of the difference from the average to the fourth power.
     *
     * \return Returns the calculated skew.
     */
    M_PUBLIC_API Real internalExcessKurtosisCalculator(
        Real numberTerms,
        Real sumValues,
        Real sumSquares,
        Real sumQuads
    );

    /**
     * Template function that calculates the excess kurtosis of scalar values.
     *
     * \param[in] values The values to be calculated.
     *
     * \return Returns the excess kurtosis of the requested values.
     */
    template<typename... ValueTypes> M_PUBLIC_TEMPLATE_FUNCTION Real excessKurtosis(const ValueTypes&... values) {
        Real result;

        Integer numberTerms = count(values...);
        if (numberTerms <= 0) {
            internalLocalTriggerNaNError();
            result = NaN;
        } else {
            Real sumValues   = sum(values...).real();
            Real mean        = sumValues / static_cast<Real>(numberTerms);
            Real sumSquares  = internalStdDevHelper(mean, values...);
            Real sumQuads    = internalKurtosisHelper(mean, values...);

            result = internalExcessKurtosisCalculator(numberTerms, sumValues, sumSquares, sumQuads);
        }

        return result;
    }

    /**
     * Function that adds values to a histogram list.
     *
     * \param[in,out] counts       The list of bucket counts.
     *
     * \param[in,out] below        The running count of values below the lower bound.
     *
     * \param[in,out] above        The running count of values above the upper bound.
     *
     * \param[in,out] lowerBound   The histogram upper bound.
     *
     * \param[in,out] upperBound   The histogram lower bound.
     *
     * \param[in,out] bucketWidths The width of the buckets.
     *
     * \param[in]    value         The value to be added.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API bool internalHistogramBuilder(
        List<unsigned long>& counts,
        unsigned long&       below,
        unsigned long&       above,
        Real                 lowerBound,
        Real                 upperBound,
        Real                 bucketWidths,
        Boolean              value
    );

    /**
     * Function that adds values to a histogram list.
     *
     * \param[in,out] counts       The list of bucket counts.
     *
     * \param[in,out] below        The running count of values below the lower bound.
     *
     * \param[in,out] above        The running count of values above the upper bound.
     *
     * \param[in,out] lowerBound   The histogram upper bound.
     *
     * \param[in,out] upperBound   The histogram lower bound.
     *
     * \param[in,out] bucketWidths The width of the buckets.
     *
     * \param[in]    value         The value to be added.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API bool internalHistogramBuilder(
        List<unsigned long>& counts,
        unsigned long&       below,
        unsigned long&       above,
        Real                 lowerBound,
        Real                 upperBound,
        Real                 bucketWidths,
        Integer              value
    );

    /**
     * Function that adds values to a histogram list.
     *
     * \param[in,out] counts       The list of bucket counts.
     *
     * \param[in,out] below        The running count of values below the lower bound.
     *
     * \param[in,out] above        The running count of values above the upper bound.
     *
     * \param[in,out] lowerBound   The histogram upper bound.
     *
     * \param[in,out] upperBound   The histogram lower bound.
     *
     * \param[in,out] bucketWidths The width of the buckets.
     *
     * \param[in]    value         The value to be added.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API bool internalHistogramBuilder(
        List<unsigned long>& counts,
        unsigned long&       below,
        unsigned long&       above,
        Real                 lowerBound,
        Real                 upperBound,
        Real                 bucketWidths,
        Real                 value
    );

    /**
     * Function that adds values to a histogram list.
     *
     * \param[in,out] counts       The list of bucket counts.
     *
     * \param[in,out] below        The running count of values below the lower bound.
     *
     * \param[in,out] above        The running count of values above the upper bound.
     *
     * \param[in,out] lowerBound   The histogram upper bound.
     *
     * \param[in,out] upperBound   The histogram lower bound.
     *
     * \param[in,out] bucketWidths The width of the buckets.
     *
     * \param[in]    value         The value to be added.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API bool internalHistogramBuilder(
        List<unsigned long>& counts,
        unsigned long&       below,
        unsigned long&       above,
        Real                 lowerBound,
        Real                 upperBound,
        Real                 bucketWidths,
        const Complex&       value
    );

    /**
     * Function that adds values to a histogram list.
     *
     * \param[in,out] counts       The list of bucket counts.
     *
     * \param[in,out] below        The running count of values below the lower bound.
     *
     * \param[in,out] above        The running count of values above the upper bound.
     *
     * \param[in,out] lowerBound   The histogram upper bound.
     *
     * \param[in,out] upperBound   The histogram lower bound.
     *
     * \param[in,out] bucketWidths The width of the buckets.
     *
     * \param[in]    value         The value to be added.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API bool internalHistogramBuilder(
        List<unsigned long>& counts,
        unsigned long&       below,
        unsigned long&       above,
        Real                 lowerBound,
        Real                 upperBound,
        Real                 bucketWidths,
        const Set&           value
    );

    /**
     * Function that adds values to a histogram list.
     *
     * \param[in,out] counts       The list of bucket counts.
     *
     * \param[in,out] below        The running count of values below the lower bound.
     *
     * \param[in,out] above        The running count of values above the upper bound.
     *
     * \param[in,out] lowerBound   The histogram upper bound.
     *
     * \param[in,out] upperBound   The histogram lower bound.
     *
     * \param[in,out] bucketWidths The width of the buckets.
     *
     * \param[in]    value         The value to be added.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API bool internalHistogramBuilder(
        List<unsigned long>& counts,
        unsigned long&       below,
        unsigned long&       above,
        Real                 lowerBound,
        Real                 upperBound,
        Real                 bucketWidths,
        const Tuple&         value
    );

    /**
     * Function that adds values to a histogram list.
     *
     * \param[in,out] counts       The list of bucket counts.
     *
     * \param[in,out] below        The running count of values below the lower bound.
     *
     * \param[in,out] above        The running count of values above the upper bound.
     *
     * \param[in,out] lowerBound   The histogram upper bound.
     *
     * \param[in,out] upperBound   The histogram lower bound.
     *
     * \param[in,out] bucketWidths The width of the buckets.
     *
     * \param[in]    value         The value to be added.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API bool internalHistogramBuilder(
        List<unsigned long>& counts,
        unsigned long&       below,
        unsigned long&       above,
        Real                 lowerBound,
        Real                 upperBound,
        Real                 bucketWidths,
        const MatrixBoolean& value
    );

    /**
     * Function that adds values to a histogram list.
     *
     * \param[in,out] counts       The list of bucket counts.
     *
     * \param[in,out] below        The running count of values below the lower bound.
     *
     * \param[in,out] above        The running count of values above the upper bound.
     *
     * \param[in,out] lowerBound   The histogram upper bound.
     *
     * \param[in,out] upperBound   The histogram lower bound.
     *
     * \param[in,out] bucketWidths The width of the buckets.
     *
     * \param[in]    value         The value to be added.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API bool internalHistogramBuilder(
        List<unsigned long>& counts,
        unsigned long&       below,
        unsigned long&       above,
        Real                 lowerBound,
        Real                 upperBound,
        Real                 bucketWidths,
        const MatrixInteger& value
    );

    /**
     * Function that adds values to a histogram list.
     *
     * \param[in,out] counts       The list of bucket counts.
     *
     * \param[in,out] below        The running count of values below the lower bound.
     *
     * \param[in,out] above        The running count of values above the upper bound.
     *
     * \param[in,out] lowerBound   The histogram upper bound.
     *
     * \param[in,out] upperBound   The histogram lower bound.
     *
     * \param[in,out] bucketWidths The width of the buckets.
     *
     * \param[in]    value         The value to be added.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API bool internalHistogramBuilder(
        List<unsigned long>& counts,
        unsigned long&       below,
        unsigned long&       above,
        Real                 lowerBound,
        Real                 upperBound,
        Real                 bucketWidths,
        const MatrixReal&    value
    );

    /**
     * Function that adds values to a histogram list.
     *
     * \param[in,out] counts       The list of bucket counts.
     *
     * \param[in,out] below        The running count of values below the lower bound.
     *
     * \param[in,out] above        The running count of values above the upper bound.
     *
     * \param[in,out] lowerBound   The histogram upper bound.
     *
     * \param[in,out] upperBound   The histogram lower bound.
     *
     * \param[in,out] bucketWidths The width of the buckets.
     *
     * \param[in]    value         The value to be added.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API bool internalHistogramBuilder(
        List<unsigned long>& counts,
        unsigned long&       below,
        unsigned long&       above,
        Real                 lowerBound,
        Real                 upperBound,
        Real                 bucketWidths,
        const MatrixComplex& value
    );

    /**
     * Function that adds values to a histogram list.
     *
     * \param[in,out] counts       The list of bucket counts.
     *
     * \param[in,out] below        The running count of values below the lower bound.
     *
     * \param[in,out] above        The running count of values above the upper bound.
     *
     * \param[in,out] lowerBound   The histogram upper bound.
     *
     * \param[in,out] upperBound   The histogram lower bound.
     *
     * \param[in,out] bucketWidths The width of the buckets.
     *
     * \param[in]    value         The value to be added.
     *
     * \return Returns true on success, returns false on error.
     */
    M_PUBLIC_API bool internalHistogramBuilder(
        List<unsigned long>& counts,
        unsigned long&       below,
        unsigned long&       above,
        Real                 lowerBound,
        Real                 upperBound,
        Real                 bucketWidths,
        const Variant&       value
    );

    /**
     * Template function that adds values to a histogram list.
     *
     * \param[in,out] counts       The list of bucket counts.
     *
     * \param[in,out] below        The running count of values below the lower bound.
     *
     * \param[in,out] above        The running count of values above the upper bound.
     *
     * \param[in,out] lowerBound   The histogram upper bound.
     *
     * \param[in,out] upperBound   The histogram lower bound.
     *
     * \param[in,out] bucketWidths The width of the buckets.
     *
     * \param[in]    first         The first value.
     *
     * \param[in]    additional    The additional values.
     *
     * \return Returns true on success, return false on error.
     */
    template<typename First, typename... Additional> M_PUBLIC_TEMPLATE_FUNCTION bool internalHistogramBuilder(
            List<unsigned long>& counts,
            unsigned long&       below,
            unsigned long&       above,
            Real                 lowerBound,
            Real                 upperBound,
            Real                 bucketWidths,
            const First&         first,
            const Additional&... additional
        ) {
        return (
               internalHistogramBuilder(
                   counts,
                   below,
                   above,
                   lowerBound,
                   upperBound,
                   bucketWidths,
                   first
               )
            && internalHistogramBuilder(
                   counts,
                   below,
                   above,
                   lowerBound,
                   upperBound,
                   bucketWidths,
                   additional...
               )
        );
    }

    /**
     * Function used internally to build the initial count buckets.
     *
     * \param[in] numberBuckets The number of buckets.
     *
     * \return Returns an initialized count bucket list.
     */
    M_PUBLIC_API List<unsigned long> internalBuildCountBuckets(Integer numberBuckets);

    /**
     * Function that generates the histogram tuple based on the intermediate results.
     *
     * \param[in,out] counts       The list of bucket counts.
     *
     * \param[in,out] below        The running count of values below the lower bound.
     *
     * \param[in,out] above        The running count of values above the upper bound.
     *
     * \param[in,out] lowerBound   The histogram upper bound.
     *
     * \param[in,out] upperBound   The histogram lower bound.
     *
     * \param[in,out] bucketWidths The width of the buckets.
     *
     * \return Returns a tuple containing:
     *             * An integer column matrix holding the counts for each bucket.
     *             * A real column matrix holding an estimated probability density function.
     *             * A real column matrix holding the bucket center positions.
     *             * The number of values that fell below the lower bound.
     *             * The number of values that fell above the upper bound.
     */
    M_PUBLIC_API Tuple internalProcessHistogramCounts(
        const List<unsigned long>& counts,
        unsigned long              below,
        unsigned long              above,
        Real                       lowerBound,
        Real                       upperBound,
        Real                       bucketWidths
    );

    /**
     * Template function that creates a histogram.
     *
     * \param[in] lowerBound    The histogram upper bound.
     *
     * \param[in] upperBound    The histogram lower bound.
     *
     * \param[in] numberBuckets The number of buckets.
     *
     * \param[in] values        The values to include in the histogram.
     *
     * \return Returns a tuple containing:
     *             * An integer column matrix holding the counts for each bucket.
     *             * A real column matrix holding an estimated probability density function.
     *             * A real column matrix holding the bucket center positions.
     *             * The number of values that fell below the lower bound.
     *             * The number of values that fell above the upper bound.
     *             * The lower bound
     *             * The upper bound
     *             * The bucket widths
     */
    template<typename T1, typename T2, typename T3, typename... ValueTypes>
        M_PUBLIC_TEMPLATE_FUNCTION Tuple histogram(
            T1                   lowerBound,
            T2                   upperBound,
            T3                   numberBuckets,
            const ValueTypes&... values
        ) {
        Tuple result;

        Real    lb;
        Real    ub;
        Integer nb;

        if (toReal(lb, lowerBound) && toReal(ub, upperBound) && toInteger(nb, numberBuckets)) {
            if (lb < ub && nb > 0) {
                List<unsigned long> counts       = internalBuildCountBuckets(nb);
                unsigned long       below        = 0;
                unsigned long       above        = 0;
                Real                bucketWidths = (ub - lb) / static_cast<Real>(nb);

                if (internalHistogramBuilder(counts, below, above, lb, ub, bucketWidths, values...)) {
                    result = internalProcessHistogramCounts(counts, below, above, lb, ub, bucketWidths);
                } else {
                    internalLocalTriggerInvalidParameterValueError();
                }
            } else {
                internalLocalTriggerInvalidParameterValueError();
            }
        }

        return result;
    }

    /**
     * Function that sorts values in a set.
     *
     * \param[in] set The set to sort.
     *
     * \return Returns a tuple containing a sorted set.
     */
    M_PUBLIC_API Tuple sort(const Set& set);

    /**
     * Function that sorts values in a tuple.
     *
     * \param[in] tuple The tuple to sort.
     *
     * \return Returns a tuple containing the sorted values.
     */
    M_PUBLIC_API Tuple sort(const Tuple& tuple);

    /**
     * Function that sorts values in a boolean matrix.
     *
     * \param[in] matrix The matrix to be sorted.
     *
     * \return Returns a sorted matrix.  The matrix dimensions will match the input values.
     */
    M_PUBLIC_API MatrixBoolean sort(const MatrixBoolean& matrix);

    /**
     * Function that sorts values in an integer matrix.
     *
     * \param[in] matrix The matrix to be sorted.
     *
     * \return Returns a sorted matrix.  The matrix dimensions will match the input values.
     */
    M_PUBLIC_API MatrixInteger sort(const MatrixInteger& matrix);

    /**
     * Function that sorts values in a real matrix.
     *
     * \param[in] matrix The matrix to be sorted.
     *
     * \return Returns a sorted matrix.  The matrix dimensions will match the input values.
     */
    M_PUBLIC_API MatrixReal sort(const MatrixReal& matrix);

    /**
     * Function that sorts values in a complex matrix.
     *
     * \param[in] matrix The matrix to be sorted.  Values must be real.
     *
     * \return Returns a sorted matrix.  The matrix dimensions will match the input values.
     */
    M_PUBLIC_API MatrixComplex sort(const MatrixComplex& matrix);

    /**
     * Function that sorts values in a set in reverse order.
     *
     * \param[in] set The set to sort.
     *
     * \return Returns a tuple containing a sorted set.
     */
    M_PUBLIC_API Tuple sortDescending(const Set& set);

    /**
     * Function that sorts values in a tuple in reverse order.
     *
     * \param[in] tuple The tuple to sort.
     *
     * \return Returns a tuple containing the sorted values.
     */
    M_PUBLIC_API Tuple sortDescending(const Tuple& tuple);

    /**
     * Function that sorts values in a boolean matrix in descending order.
     *
     * \param[in] matrix The matrix to be sorted.
     *
     * \return Returns a sorted matrix.  The matrix dimensions will match the input values.
     */
    M_PUBLIC_API MatrixBoolean sortDescending(const MatrixBoolean& matrix);

    /**
     * Function that sorts values in an integer matrix in descending order.
     *
     * \param[in] matrix The matrix to be sorted.
     *
     * \return Returns a sorted matrix.  The matrix dimensions will match the input values.
     */
    M_PUBLIC_API MatrixInteger sortDescending(const MatrixInteger& matrix);

    /**
     * Function that sorts values in a real matrix in descending order.
     *
     * \param[in] matrix The matrix to be sorted.
     *
     * \return Returns a sorted matrix.  The matrix dimensions will match the input values.
     */
    M_PUBLIC_API MatrixReal sortDescending(const MatrixReal& matrix);

    /**
     * Function that sorts values in a complex matrix in descending order.
     *
     * \param[in] matrix The matrix to be sorted.
     *
     * \return Returns a sorted matrix.  The matrix dimensions will match the input values.
     */
    M_PUBLIC_API MatrixComplex sortDescending(const MatrixComplex& matrix);
}

#endif
