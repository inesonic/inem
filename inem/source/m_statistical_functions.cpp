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
* This file implements a collection of statistical math functions.
***********************************************************************************************************************/

#include <cassert>
#include <complex>
#include <cmath>
#include <limits>
#include <vector>
#include <cassert>
#include <cstdint>

#include <vector>
#include <map>
#include <algorithm>

#include "m_compiler_abstraction.h"

DISABLE_WARNING_UNUSED_PARAMETER // Suppress warning related to poor coding in the Boost bessel function.
#include <boost/math/special_functions.hpp>
ENABLE_WARNING_UNUSED_PARAMETER

#include "model_exceptions.h"
#include "m_exceptions.h"
#include "m_api_types.h"
#include "m_intrinsic_types.h"
#include "m_set.h"
#include "m_set_iterator.h"
#include "m_implicit_ordering.h"
#include "m_tuple.h"
#include "m_variant.h"
#include "m_per_thread.h"
#include "m_matrix_integer.h"
#include "m_special_values.h"
#include "m_basic_functions.h"
#include "m_statistical_functions.h"

namespace M {
    static const Real oneOverSqrtTwoPi = 1.0 / std::sqrt(2.0 * (4.0 * atan(1.0)));
    static const Real sqrt2            = std::sqrt(2.0);

    void internalLocalTriggerNaNError() {
        internalTriggerNaNError();
    }


    void internalLocalTriggerInfinityError() {
        internalTriggerInfinityError();
    }


    void internalLocalTriggerInvalidParameterValueError() {
        internalTriggerInvalidParameterValueError();
    }


    Integer randomInteger64(PerThread& pt) {
        return pt.random64();
    }


    MatrixInteger randomInteger64(PerThread& pt, Integer numberRows, Integer numberColumns) {
        return MatrixInteger::randomInteger64(pt, numberRows, numberColumns);
    }


    Integer randomInteger32(PerThread& pt) {
        return pt.random32();
    }


    MatrixInteger randomInteger32(PerThread& pt, Integer numberRows, Integer numberColumns) {
        return MatrixInteger::randomInteger32(pt, numberRows, numberColumns);
    }


    Integer trng32(PerThread& pt) {
        return pt.trng();
    }


    Integer trng64(PerThread& pt) {
        std::uint64_t v = (static_cast<std::uint64_t>(pt.trng()) << 32) | pt.trng();
        return static_cast<Integer>(v);
    }


    Real trngU(PerThread& pt) {
        std::uint64_t v = static_cast<std::uint64_t>(trng64(pt));
        Real          r = static_cast<Real>(v) / static_cast<Real>(static_cast<std::uint64_t>(-1));

        return r;
    }


    Real normalPdf(Real x, Real mean, Real sigma) {
        Real t = (x - mean) / sigma;
        return (oneOverSqrtTwoPi / sigma) * std::exp(-0.5 * t * t);
    }


    Real normalCdf(Real x, Real mean, Real sigma) {
        return 0.5 * (1 + M::erf((x - mean) / (sigma * sqrt2)));
    }


    Real normalQuantile(Real p, Real mean, Real sigma) {
        Real result;

        if (p < 0.0 || p > 1.0) {
            internalTriggerInvalidParameterValueError();
            result = NaN;
        } else if (p == 1.0) {
            result = +infinity;
        } else if (p == -1.0) {
            result = -infinity;
        } else {
            result = mean + sigma * sqrt2 * M::erfInv(2.0 * p - 1.0);
        }

        return result;
    }


    Real gammaPdf(Real x, Real k, Real s) {
        Real result;

        if (k <= 0 || s <= 0) {
            internalTriggerInvalidParameterValueError();
            result = NaN;
        } else {
            result = (1.0 / (gamma(k) * std::pow(s, k))) * std::pow(x, k - 1.0) * std::exp(-(x / s));
        }

        return result;
    }


    Real gammaCdf(Real x, Real k, Real s) {
        Real result;

        if (k <= 0 || s <= 0) {
            internalTriggerInvalidParameterValueError();
            result = NaN;
        } else {
            result = normalizedLowerGamma(k, x / s);
        }

        return result;
    }


    Real gammaQuantile(Real p, Real k, Real s) {
        Real result;

        if (p < 0 || p > 1.0 || k <= 0 || s <= 0) {
            internalTriggerInvalidParameterValueError();
            result = NaN;
        } else {
            Real gk = gamma(k);
            result = inverseLowerGamma(k, gk * p) * s;
        }

        return result;
    }


    Real weibullPdf(Real x, Real shape, Real scale, Real delay) {
        Real result;

        if (shape <= 0 || scale <= 0) {
            internalTriggerInvalidParameterValueError();
            result = NaN;
        } else {
            if (x <= delay) {
                result = 0;
            } else {
                Real t = (x - delay) / scale;
                result = (shape / scale) * std::pow(t, shape - 1.0) * std::exp(-std::pow(t, shape));
            }
        }

        return result;
    }


    Real weibullCdf(Real x, Real shape, Real scale, Real delay) {
        Real result;

        if (shape <= 0 || scale <= 0) {
            internalTriggerInvalidParameterValueError();
            result = NaN;
        } else {
            if (x <= delay) {
                result = 0;
            } else {
                Real t = (x - delay) / scale;
                result = 1.0 - std::exp(-std::pow(t, shape));
            }
        }

        return result;
    }


    Real weibullQuantile(Real p, Real shape, Real scale, Real delay) {
        Real result;

        if (p < 0.0 || p >= 1.0 || shape <= 0.0 || scale <= 0.0) {
            internalTriggerInvalidParameterValueError();
            result = NaN;
        } else {
            result = scale * std::pow(-std::log(1.0 - p), 1.0 / shape) + delay;
        }

        return result;
    }


    Real exponentialPdf(Real x, Real rate) {
        Real result;

        if (rate <= 0.0) {
            internalTriggerInvalidParameterValueError();
            result = NaN;
        } else {
            result = rate * std::exp(-rate * x);
        }

        return result;
    }


    Real exponentialCdf(Real x, Real rate) {
        Real result;

        if (rate <= 0.0 || x < 0) {
            internalTriggerInvalidParameterValueError();
            result = NaN;
        } else {
            result = 1.0 - std::exp(-rate * x);
        }

        return result;
    }


    Real exponentialQuantile(Real p, Real rate) {
        Real result;

        if (rate <= 0.0 || p < 0.0 || p > 1.0) {
            internalTriggerInvalidParameterValueError();
            result = NaN;
        } else {
            result = -std::log(1.0 - p) / rate;
        }

        return result;
    }


    Real rayleighPdf(Real x, Real scale) {
        Real result;

        if (scale <= 0.0) {
            internalTriggerInvalidParameterValueError();
            result = NaN;
        } else {
            if (x > 0) {
                Real scaleSquared = scale * scale;
                result = (x / scaleSquared) * std::exp(-(x * x / (2.0 * scaleSquared)));
            } else {
                result = 0.0;
            }
        }

        return result;
    }


    Real rayleighCdf(Real x, Real scale) {
        Real result;

        if (scale <= 0.0) {
            internalTriggerInvalidParameterValueError();
            result = NaN;
        } else {
            if (x >= 0.0) {
                result = 1.0 - std::exp(-x * x / (2.0 * scale * scale));
            } else {
                result = NaN;
            }
        }

        return result;
    }


    Real rayleighQuantile(Real p, Real scale) {
        Real result;

        if (scale <= 0.0 || p < 0.0 || p > 1.0) {
            internalTriggerInvalidParameterValueError();
            result = NaN;
        } else {
            result = scale * std::sqrt(-2.0 * std::log(1.0 - p));
        }

        return result;
    }


    Real chiSquaredPdf(Real x, Integer k) {
        Real result;

        if (k <= 0 || x < 0.0 || (k == 1 && x == 0)) {
            internalTriggerInvalidParameterValueError();
            result = NaN;
        } else {
            Real halfK = static_cast<Real>(k) / 2.0;
            result = (1.0 / (std::pow(2.0, halfK) * gamma(halfK))) * std::pow(x, halfK - 1.0) * std::exp(-x / 2.0);
        }

        return result;
    }


    Real chiSquaredCdf(Real x, Integer k) {
        Real result;

        if (k <= 0 || x < 0.0 || (k == 1 && x == 0)) {
            internalTriggerInvalidParameterValueError();
            result = NaN;
        } else {
            Real halfK = static_cast<Real>(k) / 2.0;
            result = normalizedLowerGamma(halfK, x / 2.0);
        }

        return result;
    }


    Real chiSquaredQuantile(Real p, Integer k) {
        Real result;

        if (k <= 0 || p < 0.0 || p > 1.0) {
            internalTriggerInvalidParameterValueError();
            result = NaN;
        } else {
            Real halfK = static_cast<Real>(k) / 2.0;
            result = 2.0 * inverseLowerGamma(halfK, p * gamma(halfK));
        }

        return result;
    }


    Real poissonPmf(Integer k, Real rate) {
        Real result;

        if (rate > 0 && k >= 0) {
            result = std::exp(k * std::log(rate) - rate - lnFactorial(k));
        } else {
            internalTriggerInvalidParameterValueError();
            result = NaN;
        }

        return result;
    }


    Real poissonCdf(Integer k, Real rate) {
        Real result;

        if (rate > 0 && k >= 0) {
            result = normalizedUpperGamma(static_cast<M::Real>(k + 1), rate);
        } else {
            internalTriggerInvalidParameterValueError();
            result = NaN;
        }

        return result;
    }


    Real binomialPmf(Integer k, Integer n, Real p) {
        Real result;

        if (n >= 0 && p >= 0.0 && p <= 1.0 && k >= 0) {
            if (p == 0 || p == 1.0 || k > n) {
                result = 0.0;
            } else {
                result = std::exp(lnBinomial(n, k) + k * std::log(p) + (n - k) * std::log(1.0 - p));
            }
        } else {
            internalTriggerInvalidParameterValueError();
            result = NaN;
        }

        return result;
    }


    Real binomialCdf(Integer k, Integer n, Real p) {
        Real result;

        if (n >= 0 && p >= 0.0 && p <= 1.0 && k >= 0) {
            if (p == 0 || p == 1.0 || k > n) {
                result = 0.0;
            } else {
                result = boost::math::ibeta(static_cast<Real>(n - k), static_cast<Real>(1 + k), 1.0 - p);
            }
        } else {
            internalTriggerInvalidParameterValueError();
            result = NaN;
        }

        return result;
    }


    Real logNormalPdf(Real x, Real mean, Real sigma) {
        Real result;

        if (sigma > 0) {
            Real t = (std::log(x) - mean) / sigma;
            result = (oneOverSqrtTwoPi / (x * sigma)) * std::exp(-0.5 * t * t);
        } else {
            internalTriggerInvalidParameterValueError();
            result = NaN;
        }

        return result;
    }


    Real logNormalCdf(Real x, Real mean, Real sigma) {
        Real result;

        if (sigma > 0) {
            result = 0.5 * M::erfc(-(std::log(x) - mean) / (sigma * sqrt2));
        } else {
            internalTriggerInvalidParameterValueError();
            result = NaN;
        }

        return result;
    }


    Real logNormalQuantile(Real p, Real mean, Real sigma) {
        Real result;

        if (sigma > 0) {
            if (p == 0) {
                result = -infinity;
            } else if (p == 1.0) {
                result = +infinity;
            } else {
                result = std::exp(mean + sqrt2 * sigma * M::erfInv(2.0 * p - 1.0));
            }
        } else {
            internalTriggerInvalidParameterValueError();
            result = NaN;
        }

        return result;
    }


    Real geometricPmf(Integer k, Real p) {
        Real result;

        if (p > 0.0 && p < 1.0 && k > 0) {
            result = std::pow(1.0 - p, k - 1) * p;
        } else {
            internalTriggerInvalidParameterValueError();
            result = NaN;
        }

        return result;
    }


    Real geometricCdf(Integer k, Real p) {
        Real result;

        if (p > 0.0 && p < 1.0 && k > 0) {
            result = 1.0 - std::pow(1.0 - p, k);
        } else {
            internalTriggerInvalidParameterValueError();
            result = NaN;
        }

        return result;
    }


    Real cauchyLorentzPdf(Real x, Real location, Real scale) {
        Real result;

        if (scale > 0.0) {
            Real t = (x - location) / scale;
            result = 1.0 / (pi * scale * (1.0 + t * t));
        } else {
            internalTriggerInvalidParameterValueError();
            result = NaN;
        }

        return result;
    }


    Real cauchyLorentzCdf(Real x, Real location, Real scale) {
        Real result;

        if (scale > 0.0) {
            result = (1.0 / pi) * std::atan((x - location) / scale) + 0.5;
        } else {
            internalTriggerInvalidParameterValueError();
            result = NaN;
        }

        return result;
    }


    Real cauchyLorentzQuantile(Real p, Real location, Real scale) {
        Real result;

        if (scale > 0.0) {
            result = location + scale * std::tan(pi * (p - 0.5));
        } else {
            internalTriggerInvalidParameterValueError();
            result = NaN;
        }

        return result;
    }


    Integer count(const Set& value) {
        Integer result = 0;
        for (Set::ConstIterator it=value.constBegin(),end=value.constEnd() ; it!=end ; ++it) {
            result += count(*it);
        }

        return result;
    }


    Integer count(const Tuple& value) {
        Integer result = 0;
        for (Tuple::ConstIterator it=value.constBegin(),end=value.constEnd() ; it!=end ; ++it) {
            result += count(*it);
        }

        return result;
    }


    Integer count(const Variant& value) {
        Integer result;

        ValueType valueType = value.valueType();
        switch (valueType) {
            case ValueType::NONE:           { result = 0;                                break; }
            case ValueType::BOOLEAN:        { result = 1;                                break; }
            case ValueType::INTEGER:        { result = 1;                                break; }
            case ValueType::REAL:           { result = 1;                                break; }
            case ValueType::COMPLEX:        { result = 1;                                break; }
            case ValueType::SET:            { result = count(value.toSet());             break; }
            case ValueType::TUPLE:          { result = count(value.toTuple());           break; }
            case ValueType::MATRIX_BOOLEAN: { result = count(value.toMatrixBoolean());   break; }
            case ValueType::MATRIX_INTEGER: { result = count(value.toMatrixInteger());   break; }
            case ValueType::MATRIX_REAL:    { result = count(value.toMatrixReal());      break; }
            case ValueType::MATRIX_COMPLEX: { result = count(value.toMatrixComplex());   break; }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                result = 0;
                break;
            }

            default: {
                assert(false);
                result = 0;
                break;
            }
        }

        return result;
    }


    Complex sum() {
        return Complex(0);
    }


    Complex sum(Boolean value) {
        return Complex(value ? 1 : 0);
    }


    Complex sum(Integer value) {
        return Complex(value);
    }


    Complex sum(Real value) {
        return Complex(value);
    }


    Complex sum(const Set& value) {
        Complex result(0);

        for (Set::ConstIterator it=value.constBegin(),end=value.constEnd() ; it!=end ; ++it) {
            result += sum(*it);
        }

        return result;
    }


    Complex sum(const Tuple& value) {
        Complex result(0);

        for (Tuple::ConstIterator it=value.constBegin(),end=value.constEnd() ; it!=end ; ++it) {
            result += sum(*it);
        }

        return result;
    }


    Complex sum(const MatrixBoolean& value) {
        Integer result = 0;

        Integer numberRows    = static_cast<unsigned long>(value.numberRows());
        Integer numberColumns = static_cast<unsigned long>(value.numberColumns());

        for (Integer c=1 ; c<=numberColumns ; ++c) {
            for (Integer r=1 ; r<=numberRows ; ++r) {
                result += value(r,c) ? 1 : 0;
            }
        }

        return Complex(result);
    }


    Complex sum(const MatrixInteger& value) {
        Real result = 0;

        Integer numberRows    = static_cast<unsigned long>(value.numberRows());
        Integer numberColumns = static_cast<unsigned long>(value.numberColumns());

        for (Integer c=1 ; c<=numberColumns ; ++c) {
            for (Integer r=1 ; r<=numberRows ; ++r) {
                result += static_cast<Real>(value(r,c));
            }
        }

        return Complex(result);
    }


    Complex sum(const MatrixReal& value) {
        Real result = 0;

        Integer numberRows    = static_cast<unsigned long>(value.numberRows());
        Integer numberColumns = static_cast<unsigned long>(value.numberColumns());

        for (Integer c=1 ; c<=numberColumns ; ++c) {
            for (Integer r=1 ; r<=numberRows ; ++r) {
                result += value(r,c);
            }
        }

        return Complex(result);
    }


    Complex sum(const MatrixComplex& value) {
        Complex result(0);

        Integer numberRows    = static_cast<unsigned long>(value.numberRows());
        Integer numberColumns = static_cast<unsigned long>(value.numberColumns());

        for (Integer c=1 ; c<=numberColumns ; ++c) {
            for (Integer r=1 ; r<=numberRows ; ++r) {
                result += value(r,c);
            }
        }

        return result;
    }


    Complex sum(const Variant& value) {
        Complex result;

        ValueType valueType = value.valueType();
        switch (valueType) {
            case ValueType::NONE:           { result = 0;                              break; }
            case ValueType::BOOLEAN:        { result = sum(value.toBoolean());         break; }
            case ValueType::INTEGER:        { result = sum(value.toInteger());         break; }
            case ValueType::REAL:           { result = sum(value.toReal());            break; }
            case ValueType::COMPLEX:        { result = sum(value.toComplex());         break; }
            case ValueType::SET:            { result = sum(value.toSet());             break; }
            case ValueType::TUPLE:          { result = sum(value.toTuple());           break; }
            case ValueType::MATRIX_BOOLEAN: { result = sum(value.toMatrixBoolean());   break; }
            case ValueType::MATRIX_INTEGER: { result = sum(value.toMatrixInteger());   break; }
            case ValueType::MATRIX_REAL:    { result = sum(value.toMatrixReal());      break; }
            case ValueType::MATRIX_COMPLEX: { result = sum(value.toMatrixComplex());   break; }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                result = 0;
                break;
            }

            default: {
                assert(false);
                result = 0;
                break;
            }
        }

        return result;
    }


    bool internalHasValue(const Set& value) {
        bool result = false;

        if (!value.isEmpty()) {
            Set::ConstIterator it     = value.constBegin();
            Set::ConstIterator end    = value.constEnd();

            // NOTE: assignment in loop.
            while (it != end && (result = internalHasValue(*it)) == false) {
                ++it;
            }
        } else {
            result = false;
        }

        return result;
    }


    bool internalHasValue(const Tuple& value) {
        bool result = false;

        if (!value.isEmpty()) {
            Tuple::ConstIterator it     = value.constBegin();
            Tuple::ConstIterator end    = value.constEnd();

            // NOTE: assignment in loop.
            while (it != end && (result = internalHasValue(*it)) == false) {
                ++it;
            }
        } else {
            result = false;
        }

        return result;
    }


    bool internalHasValue(const Variant& value) {
        bool result;

        ValueType valueType = value.valueType();
        switch (valueType) {
            case ValueType::NONE:           { result = false;                                       break; }
            case ValueType::BOOLEAN:        { result = internalHasValue(value.toBoolean());         break; }
            case ValueType::INTEGER:        { result = internalHasValue(value.toInteger());         break; }
            case ValueType::REAL:           { result = internalHasValue(value.toReal());            break; }
            case ValueType::COMPLEX:        { result = internalHasValue(value.toComplex());         break; }
            case ValueType::SET:            { result = internalHasValue(value.toSet());             break; }
            case ValueType::TUPLE:          { result = internalHasValue(value.toTuple());           break; }
            case ValueType::MATRIX_BOOLEAN: { result = internalHasValue(value.toMatrixBoolean());   break; }
            case ValueType::MATRIX_INTEGER: { result = internalHasValue(value.toMatrixInteger());   break; }
            case ValueType::MATRIX_REAL:    { result = internalHasValue(value.toMatrixReal());      break; }
            case ValueType::MATRIX_COMPLEX: { result = internalHasValue(value.toMatrixComplex());   break; }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                result = false;
                break;
            }

            default: {
                assert(false);
                result = false;
                break;
            }
        }

        return result;
    }


    Real internalMinHelper(const Complex& value) {
        Real result;
        if (!toReal(result, value)) {
            result = NaN;
        }

        return result;
    }


    Real internalMinHelper(const Set& value) {
        Real result = +infinity;

        for (Set::ConstIterator it=value.constBegin(),end=value.constEnd() ; it!=end ; ++it) {
            Real valueMinimum = internalMinHelper(*it);
            if (valueMinimum < result) {
                result = valueMinimum;
            }
        }

        return result;
    }


    Real internalMinHelper(const Tuple& value) {
        Real result = +infinity;

        for (Tuple::ConstIterator it=value.constBegin(),end=value.constEnd() ; it!=end ; ++it) {
            Real valueMinimum = internalMinHelper(*it);
            if (valueMinimum < result) {
                result = valueMinimum;
            }
        }

        return result;
    }


    Real internalMinHelper(const MatrixBoolean& value) {
        Real result = +infinity;

        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                Real v = value(ri, ci) ? 1.0 : 0.0;
                if (v < result) {
                    result = v;
                }
            }
        }

        return result;
    }


    Real internalMinHelper(const MatrixInteger& value) {
        Real result = +infinity;

        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                Real v = static_cast<Real>(value(ri, ci));
                if (v < result) {
                    result = v;
                }
            }
        }

        return result;
    }


    Real internalMinHelper(const MatrixReal& value) {
        Real result = +infinity;

        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                Real v = value(ri, ci);
                if (v < result) {
                    result = v;
                }
            }
        }

        return result;
    }


    Real internalMinHelper(const MatrixComplex& value) {
        Real result = +infinity;

        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        Integer ri = 1;
        while (!std::isnan(result) && ri <= numberRows) {
            Integer ci = 1;
            while (!std::isnan(result) && ci <= numberColumns) {
                Complex v = value(ri, ci);
                if (v.imag() != 0.0) {
                    internalTriggerInvalidParameterValueError();
                    v = NaN;
                } else if (v.real() < result) {
                    result = v.real();
                }

                ++ci;
            }

            ++ri;
        }

        return result;
    }


    Real internalMinHelper(const Variant& value) {
        Real result;

        ValueType valueType = value.valueType();
        switch (valueType) {
            case ValueType::NONE:           { result = +infinity;                                    break; }
            case ValueType::BOOLEAN:        { result = internalMinHelper(value.toBoolean());         break; }
            case ValueType::INTEGER:        { result = internalMinHelper(value.toInteger());         break; }
            case ValueType::REAL:           { result = internalMinHelper(value.toReal());            break; }
            case ValueType::COMPLEX:        { result = internalMinHelper(value.toComplex());         break; }
            case ValueType::SET:            { result = internalMinHelper(value.toSet());             break; }
            case ValueType::TUPLE:          { result = internalMinHelper(value.toTuple());           break; }
            case ValueType::MATRIX_BOOLEAN: { result = internalMinHelper(value.toMatrixBoolean());   break; }
            case ValueType::MATRIX_INTEGER: { result = internalMinHelper(value.toMatrixInteger());   break; }
            case ValueType::MATRIX_REAL:    { result = internalMinHelper(value.toMatrixReal());      break; }
            case ValueType::MATRIX_COMPLEX: { result = internalMinHelper(value.toMatrixComplex());   break; }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                result = +infinity;
                break;
            }

            default: {
                assert(false);
                result = +infinity;
                break;
            }
        }

        return result;
    }


    Real internalMaxHelper(const Complex& value) {
        Real result;
        if (!toReal(result, value)) {
            result = NaN;
        }

        return result;
    }


    Real internalMaxHelper(const Set& value) {
        Real result = -infinity;

        for (Set::ConstIterator it=value.constBegin(),end=value.constEnd() ; it!=end ; ++it) {
            Real valueMaximum = internalMaxHelper(*it);
            if (valueMaximum > result) {
                result = valueMaximum;
            }
        }

        return result;
    }


    Real internalMaxHelper(const Tuple& value) {
        Real result = -infinity;

        for (Tuple::ConstIterator it=value.constBegin(),end=value.constEnd() ; it!=end ; ++it) {
            Real valueMaximum = internalMaxHelper(*it);
            if (valueMaximum > result) {
                result = valueMaximum;
            }
        }

        return result;
    }


    Real internalMaxHelper(const MatrixBoolean& value) {
        Real result = -infinity;

        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                Real v = value(ri, ci) ? 1.0 : 0.0;
                if (v > result) {
                    result = v;
                }
            }
        }

        return result;
    }


    Real internalMaxHelper(const MatrixInteger& value) {
        Real result = -infinity;

        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                Real v = static_cast<Real>(value(ri, ci));
                if (v > result) {
                    result = v;
                }
            }
        }

        return result;
    }


    Real internalMaxHelper(const MatrixReal& value) {
        Real result = -infinity;

        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                Real v = value(ri, ci);
                if (v > result) {
                    result = v;
                }
            }
        }

        return result;
    }


    Real internalMaxHelper(const MatrixComplex& value) {
        Real result = -infinity;

        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        Integer ri = 1;
        while (!std::isnan(result) && ri <= numberRows) {
            Integer ci = 1;
            while (!std::isnan(result) && ci <= numberColumns) {
                Complex v = value(ri, ci);
                if (v.imag() != 0.0) {
                    internalTriggerInvalidParameterValueError();
                    v = NaN;
                } else if (v.real() > result) {
                    result = v.real();
                }

                ++ci;
            }

            ++ri;
        }

        return result;
    }


    Real internalMaxHelper(const Variant& value) {
        Real result;

        ValueType valueType = value.valueType();
        switch (valueType) {
            case ValueType::NONE:           { result = +infinity;                                    break; }
            case ValueType::BOOLEAN:        { result = internalMaxHelper(value.toBoolean());         break; }
            case ValueType::INTEGER:        { result = internalMaxHelper(value.toInteger());         break; }
            case ValueType::REAL:           { result = internalMaxHelper(value.toReal());            break; }
            case ValueType::COMPLEX:        { result = internalMaxHelper(value.toComplex());         break; }
            case ValueType::SET:            { result = internalMaxHelper(value.toSet());             break; }
            case ValueType::TUPLE:          { result = internalMaxHelper(value.toTuple());           break; }
            case ValueType::MATRIX_BOOLEAN: { result = internalMaxHelper(value.toMatrixBoolean());   break; }
            case ValueType::MATRIX_INTEGER: { result = internalMaxHelper(value.toMatrixInteger());   break; }
            case ValueType::MATRIX_REAL:    { result = internalMaxHelper(value.toMatrixReal());      break; }
            case ValueType::MATRIX_COMPLEX: { result = internalMaxHelper(value.toMatrixComplex());   break; }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                result = +infinity;
                break;
            }

            default: {
                assert(false);
                result = +infinity;
                break;
            }
        }

        return result;
    }


    Real internalStdDevHelper(Real mean, Boolean value) {
        Real v = (value ? 1.0 : 0.0) - mean;
        return v * v;
    }


    Real internalStdDevHelper(Real mean, Integer value) {
        Real v = static_cast<Real>(value) - mean;
        return v * v;
    }


    Real internalStdDevHelper(Real mean, Real value) {
        Real v = value - mean;
        return v * v;
    }


    Real internalStdDevHelper(Real mean, const Complex& value) {
        Real result;
        Real v;
        if (!toReal(v, value)) {
            result = NaN;
        } else {
            v -= mean;
            result = v * v;
        }

        return result;
    }


    Real internalStdDevHelper(Real mean, const Set& value) {
        Real result = 0.0;

        for (Set::ConstIterator it=value.constBegin(),end=value.constEnd() ; it!=end ; ++it) {
            Real v = internalStdDevHelper(mean, *it);
            result += v;
        }

        return result;
    }


    Real internalStdDevHelper(Real mean, const Tuple& value) {
        Real result = 0.0;

        for (Tuple::ConstIterator it=value.constBegin(),end=value.constEnd() ; it!=end ; ++it) {
            Real v = internalStdDevHelper(mean, *it);
            result += v;
        }

        return result;
    }


    Real internalStdDevHelper(Real mean, const MatrixBoolean& value) {
        Real result = 0;

        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                Real v = (value(ri, ci) ? 1.0 : 0.0) - mean;
                result += v * v;
            }
        }

        return result;
    }


    Real internalStdDevHelper(Real mean, const MatrixInteger& value) {
        Real result = 0;

        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                Real v = static_cast<Real>(value(ri, ci)) - mean;
                result += v * v;
            }
        }

        return result;
    }


    Real internalStdDevHelper(Real mean, const MatrixReal& value) {
        Real result = 0;

        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                Real v = value(ri, ci) - mean;
                result += v * v;
            }
        }

        return result;
    }


    Real internalStdDevHelper(Real mean, const MatrixComplex& value) {
        Real result = 0;

        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        Integer ri = 1;
        while (!std::isnan(result) && ri <= numberRows) {
            Integer ci = 1;
            while (!std::isnan(result) && ci <= numberColumns) {
                Complex v = value(ri, ci);
                if (v.imag() != 0.0) {
                    internalTriggerInvalidParameterValueError();
                    v = NaN;
                } else {
                    Real vr = v.real() - mean;
                    result = vr * vr;
                }

                ++ci;
            }

            ++ri;
        }

        return result;
    }


    Real internalStdDevHelper(Real mean, const Variant& value) {
        Real result;

        ValueType valueType = value.valueType();
        switch (valueType) {
            case ValueType::NONE:           { result = 0;                                                     break; }
            case ValueType::BOOLEAN:        { result = internalStdDevHelper(mean, value.toBoolean());         break; }
            case ValueType::INTEGER:        { result = internalStdDevHelper(mean, value.toInteger());         break; }
            case ValueType::REAL:           { result = internalStdDevHelper(mean, value.toReal());            break; }
            case ValueType::COMPLEX:        { result = internalStdDevHelper(mean, value.toComplex());         break; }
            case ValueType::SET:            { result = internalStdDevHelper(mean, value.toSet());             break; }
            case ValueType::TUPLE:          { result = internalStdDevHelper(mean, value.toTuple());           break; }
            case ValueType::MATRIX_BOOLEAN: { result = internalStdDevHelper(mean, value.toMatrixBoolean());   break; }
            case ValueType::MATRIX_INTEGER: { result = internalStdDevHelper(mean, value.toMatrixInteger());   break; }
            case ValueType::MATRIX_REAL:    { result = internalStdDevHelper(mean, value.toMatrixReal());      break; }
            case ValueType::MATRIX_COMPLEX: { result = internalStdDevHelper(mean, value.toMatrixComplex());   break; }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                result = 0;
                break;
            }

            default: {
                assert(false);
                result = 0;
                break;
            }
        }

        return result;
    }


    bool internalBuildRealList(List<Real>& list, const Complex& value) {
        bool result;

        Real v;
        if (toReal(v, value)) {
            list.append(v);
            result = true;
        } else {
            internalTriggerNaNError();
            result = false;
        }

        return result;
    }


    bool internalBuildRealList(List<Real>& list, const Set& value) {
        bool result = true;

        Set::ConstIterator it  = value.constBegin();
        Set::ConstIterator end = value.constEnd();
        while (it != end && result) {
            result = internalBuildRealList(list, *it);
            ++it;
        }

        return result;
    }


    bool internalBuildRealList(List<Real>& list, const Tuple& value) {
        bool result = true;

        Tuple::ConstIterator it  = value.constBegin();
        Tuple::ConstIterator end = value.constEnd();
        while (it != end && result) {
            result = internalBuildRealList(list, *it);
            ++it;
        }

        return result;
    }


    bool internalBuildRealList(List<Real>& list, const MatrixBoolean& value) {
        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                Real v = value(ri, ci) ? 1.0 : 0.0;
                list.append(v);
            }
        }

        return true;
    }


    bool internalBuildRealList(List<Real>& list, const MatrixInteger& value) {
        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                Real v = static_cast<Real>(value(ri, ci));
                list.append(v);
            }
        }

        return true;
    }


    bool internalBuildRealList(List<Real>& list, const MatrixReal& value) {
        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                Real v = value(ri, ci);
                list.append(v);
            }
        }

        return true;
    }


    bool internalBuildRealList(List<Real>& list, const MatrixComplex& value) {
        bool result = true;
        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        Integer ri = 1;
        while (result && ri <= numberRows) {
            Integer ci = 1;
            while (result && ci <= numberColumns) {
                Complex v = value(ri, ci);

                if (v.imag() != 0.0) {
                    internalTriggerInvalidParameterValueError();
                    result = false;
                } else {
                    list.append(v.real());
                    result = true;
                }

                ++ci;
            }

            ++ri;
        }

        return result;
    }


    bool internalBuildRealList(List<Real>& list, const Variant& value) {
        bool result;

        ValueType valueType = value.valueType();
        switch (valueType) {
            case ValueType::NONE:           { result = false;                                                 break; }
            case ValueType::BOOLEAN:        { result = internalBuildRealList(list, value.toBoolean());        break; }
            case ValueType::INTEGER:        { result = internalBuildRealList(list, value.toInteger());        break; }
            case ValueType::REAL:           { result = internalBuildRealList(list, value.toReal());           break; }
            case ValueType::COMPLEX:        { result = internalBuildRealList(list, value.toComplex());        break; }
            case ValueType::SET:            { result = internalBuildRealList(list, value.toSet());            break; }
            case ValueType::TUPLE:          { result = internalBuildRealList(list, value.toTuple());          break; }
            case ValueType::MATRIX_BOOLEAN: { result = internalBuildRealList(list, value.toMatrixBoolean());  break; }
            case ValueType::MATRIX_INTEGER: { result = internalBuildRealList(list, value.toMatrixInteger());  break; }
            case ValueType::MATRIX_REAL:    { result = internalBuildRealList(list, value.toMatrixReal());     break; }
            case ValueType::MATRIX_COMPLEX: { result = internalBuildRealList(list, value.toMatrixComplex());  break; }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                result = 0;
                break;
            }

            default: {
                assert(false);
                result = 0;
                break;
            }
        }

        return result;
    }


    Real internalCalculateMedian(const List<Real>& list) {
        Real result;

        unsigned long numberEntries = static_cast<unsigned long>(list.size());
        if (numberEntries > 0) {
            std::vector<Real> l(numberEntries);
            for (unsigned i=0 ; i<numberEntries ; ++i) {
                l[i] = list.at(i);
            }

            std::sort(l.begin(), l.end());

            unsigned index = (numberEntries / 2);
            result = (numberEntries & 1) == 0 ? (l[index] + l[index - 1]) / 2.0 : l.at(index);
        } else {
            internalTriggerNaNError();
            result = NaN;
        }

        return result;
    }


    void internalBuildComplexList(List<Complex>& list, const Set& value) {
        for (Set::ConstIterator it=value.constBegin(),end=value.constEnd() ; it!=end ; ++it) {
            internalBuildComplexList(list, *it);
        }
    }


    void internalBuildComplexList(List<Complex>& list, const Tuple& value) {
        for (Tuple::ConstIterator it=value.constBegin(),end=value.constEnd() ; it!=end ; ++it) {
            internalBuildComplexList(list, *it);
        }
    }


    void internalBuildComplexList(List<Complex>& list, const MatrixBoolean& value) {
        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                Real v = value(ri, ci) ? 1.0 : 0.0;
                list.append(v);
            }
        }
    }


    void internalBuildComplexList(List<Complex>& list, const MatrixInteger& value) {
        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                Real v = static_cast<Real>(value(ri, ci));
                list.append(v);
            }
        }
    }


    void internalBuildComplexList(List<Complex>& list, const MatrixReal& value) {
        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                Real v = value(ri, ci);
                list.append(v);
            }
        }
    }


    void internalBuildComplexList(List<Complex>& list, const MatrixComplex& value) {
        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                Complex v = value(ri, ci);
                list.append(v);
            }
        }
    }


    void internalBuildComplexList(List<Complex>& list, const Variant& value) {
        ValueType valueType = value.valueType();
        switch (valueType) {
            case ValueType::NONE:           {                                                           break; }
            case ValueType::BOOLEAN:        { internalBuildComplexList(list, value.toBoolean());        break; }
            case ValueType::INTEGER:        { internalBuildComplexList(list, value.toInteger());        break; }
            case ValueType::REAL:           { internalBuildComplexList(list, value.toReal());           break; }
            case ValueType::COMPLEX:        { internalBuildComplexList(list, value.toComplex());        break; }
            case ValueType::SET:            { internalBuildComplexList(list, value.toSet());            break; }
            case ValueType::TUPLE:          { internalBuildComplexList(list, value.toTuple());          break; }
            case ValueType::MATRIX_BOOLEAN: { internalBuildComplexList(list, value.toMatrixBoolean());  break; }
            case ValueType::MATRIX_INTEGER: { internalBuildComplexList(list, value.toMatrixInteger());  break; }
            case ValueType::MATRIX_REAL:    { internalBuildComplexList(list, value.toMatrixReal());     break; }
            case ValueType::MATRIX_COMPLEX: { internalBuildComplexList(list, value.toMatrixComplex());  break; }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                break;
            }

            default: {
                assert(false);
                break;
            }
        }
    }


    class InternalComplexCompareFunction {
        public:
            bool operator()(const Complex& a, const Complex& b) const {
                return a.real() < b.real() || (a.real() == b.real() && a.imag() < b.imag());
            }
    };


    Complex internalCalculateMode(const List<Complex>& list) {
        Complex result;

        unsigned long numberEntries = static_cast<unsigned long>(list.size());
        if (numberEntries > 0) {
            std::map<Complex, unsigned long, InternalComplexCompareFunction> values;

            for (unsigned i=0 ; i<numberEntries ; ++i) {
                Complex v = list.at(i);
                std::map<Complex, unsigned long, InternalComplexCompareFunction>::iterator it = values.find(v);

                if (it == values.end()) {
                    values.emplace(v, 1U);
                } else {
                    ++(it->second);
                }
            }

            unsigned bestCount = 0;
            for (  std::map<Complex, unsigned long, InternalComplexCompareFunction>::const_iterator
                       it  = values.cbegin(),
                       end = values.cend()
                 ; it != end
                 ; ++it
                ) {
                if (it->second > bestCount) {
                    bestCount = it->second;
                    result    = it->first;
                }
            }
        } else {
            internalTriggerNaNError();
            result = NaN;
        }

        return result;
    }


    Real internalSkewHelper(Real mean, Boolean value) {
        Real v = (value ? 1.0 : 0.0) - mean;
        return v * v * v;
    }


    Real internalSkewHelper(Real mean, Integer value) {
        Real v = value - mean;
        return v * v * v;
    }


    Real internalSkewHelper(Real mean, Real value) {
        Real v = value - mean;
        return v * v * v;
    }


    Real internalSkewHelper(Real mean, const Complex& value) {
        Real result;

        Real v;
        if (toReal(v, value)) {
            v -= mean;
            result = v * v * v;
        } else {
            result = NaN;
        }

        return result;
    }


    Real internalSkewHelper(Real mean, const Set& value) {
        Real result = 0;

        for (Set::ConstIterator it=value.constBegin(),end=value.constEnd() ; it!=end ; ++it) {
            result += internalSkewHelper(mean, *it);
        }

        return result;
    }


    Real internalSkewHelper(Real mean, const Tuple& value) {
        Real result = 0;

        for (Tuple::ConstIterator it=value.constBegin(),end=value.constEnd() ; it!=end ; ++it) {
            result += internalSkewHelper(mean, *it);
        }

        return result;
    }


    Real internalSkewHelper(Real mean, const MatrixBoolean& value) {
        Real result = 0;

        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                Real v = (value(ri, ci) ? 1.0 : 0.0) - mean;
                result += v * v * v;
            }
        }

        return result;
    }


    Real internalSkewHelper(Real mean, const MatrixInteger& value) {
        Real result = 0;

        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                Real v = value(ri, ci) - mean;
                result += v * v * v;
            }
        }

        return result;
    }


    Real internalSkewHelper(Real mean, const MatrixReal& value) {
        Real result = 0;

        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                Real v = value(ri, ci) - mean;
                result += v * v * v;
            }
        }

        return result;
    }


    Real internalSkewHelper(Real mean, const MatrixComplex& value) {
        Real result = 0;

        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        Integer ri = 1;
        while (!std::isnan(result) && ri <= numberRows) {
            Integer ci = 1;
            while (!std::isnan(result) && ci <= numberColumns) {
                Real v;
                if (toReal(v, value(ri, ci))) {
                    v -= mean;
                    result += v * v * v;
                } else {
                    internalTriggerNaNError();
                    result = NaN;
                }

                ++ci;
            }

            ++ri;
        }

        return result;
    }


    Real internalSkewHelper(Real mean, const Variant& value) {
        Real result;

        ValueType valueType = value.valueType();
        switch (valueType) {
            case ValueType::NONE:           { result = 0;                                                  break; }
            case ValueType::BOOLEAN:        { result = internalSkewHelper(mean, value.toBoolean());        break; }
            case ValueType::INTEGER:        { result = internalSkewHelper(mean, value.toInteger());        break; }
            case ValueType::REAL:           { result = internalSkewHelper(mean, value.toReal());           break; }
            case ValueType::COMPLEX:        { result = internalSkewHelper(mean, value.toComplex());        break; }
            case ValueType::SET:            { result = internalSkewHelper(mean, value.toSet());            break; }
            case ValueType::TUPLE:          { result = internalSkewHelper(mean, value.toTuple());          break; }
            case ValueType::MATRIX_BOOLEAN: { result = internalSkewHelper(mean, value.toMatrixBoolean());  break; }
            case ValueType::MATRIX_INTEGER: { result = internalSkewHelper(mean, value.toMatrixInteger());  break; }
            case ValueType::MATRIX_REAL:    { result = internalSkewHelper(mean, value.toMatrixReal());     break; }
            case ValueType::MATRIX_COMPLEX: { result = internalSkewHelper(mean, value.toMatrixComplex());  break; }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                result = 0;
                break;
            }

            default: {
                assert(false);
                result = 0;
                break;
            }
        }

        return result;
    }


    Real internalSampleSkewCalculator(Real numberTerms, Real /* sumValues */, Real sumSquares, Real sumCubes) {
        Real besselCount = static_cast<Real>(numberTerms - 1);
        Real denom       = std::pow(sumSquares / besselCount, 1.5);
        Real num         = sumCubes / static_cast<Real>(numberTerms);

        return num / denom;
    }


    Real internalKurtosisHelper(Real mean, Boolean value) {
        Real v = (value ? 1.0 : 0.0) - mean;
        Real v2 = v * v;
        return v2 * v2;
    }


    Real internalKurtosisHelper(Real mean, Integer value) {
        Real v = value - mean;
        Real v2 = v * v;
        return v2 * v2;
    }


    Real internalKurtosisHelper(Real mean, Real value) {
        Real v = value - mean;
        Real v2 = v * v;
        return v2 * v2;
    }


    Real internalKurtosisHelper(Real mean, const Complex& value) {
        Real result;

        Real v;
        if (toReal(v, value)) {
            v -= mean;
            Real v2 = v * v;
            result = v2 * v2;
        } else {
            result = NaN;
        }

        return result;
    }


    Real internalKurtosisHelper(Real mean, const Set& value) {
        Real result = 0;

        for (Set::ConstIterator it=value.constBegin(),end=value.constEnd() ; it!=end ; ++it) {
            result += internalKurtosisHelper(mean, *it);
        }

        return result;
    }


    Real internalKurtosisHelper(Real mean, const Tuple& value) {
        Real result = 0;

        for (Tuple::ConstIterator it=value.constBegin(),end=value.constEnd() ; it!=end ; ++it) {
            result += internalKurtosisHelper(mean, *it);
        }

        return result;
    }


    Real internalKurtosisHelper(Real mean, const MatrixBoolean& value) {
        Real result = 0;

        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                Real v = (value(ri, ci) ? 1.0 : 0.0) - mean;
                Real v2 = v * v;
                result += v2 * v2;
            }
        }

        return result;
    }


    Real internalKurtosisHelper(Real mean, const MatrixInteger& value) {
        Real result = 0;

        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                Real v = value(ri, ci) - mean;
                Real v2 = v * v;
                result += v2 * v2;
            }
        }

        return result;
    }


    Real internalKurtosisHelper(Real mean, const MatrixReal& value) {
        Real result = 0;

        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                Real v = value(ri, ci) - mean;
                Real v2 = v * v;
                result += v2 * v2;
            }
        }

        return result;
    }


    Real internalKurtosisHelper(Real mean, const MatrixComplex& value) {
        Real result = 0;

        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        Integer ri = 1;
        while (!std::isnan(result) && ri <= numberRows) {
            Integer ci = 1;
            while (!std::isnan(result) && ci <= numberColumns) {
                Real v;
                if (toReal(v, value(ri, ci))) {
                    v -= mean;
                    Real v2 = v * v;
                    result += v2 * v2;
                } else {
                    internalTriggerNaNError();
                    result = NaN;
                }

                ++ci;
            }

            ++ri;
        }

        return result;
    }


    Real internalKurtosisHelper(Real mean, const Variant& value) {
        Real result;

        ValueType valueType = value.valueType();
        switch (valueType) {
            case ValueType::NONE:           { result = 0;                                                      break; }
            case ValueType::BOOLEAN:        { result = internalKurtosisHelper(mean, value.toBoolean());        break; }
            case ValueType::INTEGER:        { result = internalKurtosisHelper(mean, value.toInteger());        break; }
            case ValueType::REAL:           { result = internalKurtosisHelper(mean, value.toReal());           break; }
            case ValueType::COMPLEX:        { result = internalKurtosisHelper(mean, value.toComplex());        break; }
            case ValueType::SET:            { result = internalKurtosisHelper(mean, value.toSet());            break; }
            case ValueType::TUPLE:          { result = internalKurtosisHelper(mean, value.toTuple());          break; }
            case ValueType::MATRIX_BOOLEAN: { result = internalKurtosisHelper(mean, value.toMatrixBoolean());  break; }
            case ValueType::MATRIX_INTEGER: { result = internalKurtosisHelper(mean, value.toMatrixInteger());  break; }
            case ValueType::MATRIX_REAL:    { result = internalKurtosisHelper(mean, value.toMatrixReal());     break; }
            case ValueType::MATRIX_COMPLEX: { result = internalKurtosisHelper(mean, value.toMatrixComplex());  break; }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                result = 0;
                break;
            }

            default: {
                assert(false);
                result = 0;
                break;
            }
        }

        return result;
    }


    Real internalExcessKurtosisCalculator(Real numberTerms, Real /* sumValues */, Real sumSquares, Real sumQuads) {
        Real variance = sumSquares / static_cast<Real>(numberTerms);
        Real denom    = variance * variance;
        Real num      = sumQuads / static_cast<Real>(numberTerms);

        return (num / denom) - 3.0;
    }


    bool internalHistogramBuilder(
            List<unsigned long>& counts,
            unsigned long&       below,
            unsigned long&       above,
            Real                 lowerBound,
            Real                 upperBound,
            Real                 bucketWidths,
            Boolean              value
        ) {
        internalHistogramBuilder(
            counts,
            below,
            above,
            lowerBound,
            upperBound,
            bucketWidths,
            static_cast<Real>(value)
        );

        return true;
    }


    bool internalHistogramBuilder(
            List<unsigned long>& counts,
            unsigned long&       below,
            unsigned long&       above,
            Real                 lowerBound,
            Real                 upperBound,
            Real                 bucketWidths,
            Integer              value
        ) {
        internalHistogramBuilder(
            counts,
            below,
            above,
            lowerBound,
            upperBound,
            bucketWidths,
            static_cast<Real>(value)
        );

        return true;
    }


    bool internalHistogramBuilder(
            List<unsigned long>& counts,
            unsigned long&       below,
            unsigned long&       above,
            Real                 lowerBound,
            Real                 upperBound,
            Real                 bucketWidths,
            Real                 value
        ) {
        if (value < lowerBound) {
            ++below;
        } else if (value > upperBound) {
            ++above;
        } else {
            unsigned long bucket = static_cast<unsigned long>((value - lowerBound) / bucketWidths);
            if (bucket >= counts.size()) {
                bucket = counts.size() - 1;
            }

            ++counts[bucket];
        }

        return true;
    }


    bool internalHistogramBuilder(
            List<unsigned long>& counts,
            unsigned long&       below,
            unsigned long&       above,
            Real                 lowerBound,
            Real                 upperBound,
            Real                 bucketWidths,
            const Complex&       value
        ) {
        bool result;

        Real v;
        if (toReal(v, value)) {
            internalHistogramBuilder(
                counts,
                below,
                above,
                lowerBound,
                upperBound,
                bucketWidths,
                v
            );
            result = true;
        } else {
            result = false;
        }

        return result;
    }


    bool internalHistogramBuilder(
            List<unsigned long>& counts,
            unsigned long&       below,
            unsigned long&       above,
            Real                 lowerBound,
            Real                 upperBound,
            Real                 bucketWidths,
            const Set&           value
        ) {
        bool result = true;

        Set::ConstIterator it  = value.constBegin();
        Set::ConstIterator end = value.constEnd();
        while (it != end && result) {
            result = internalHistogramBuilder(counts, below, above, lowerBound, upperBound, bucketWidths, *it);
            ++it;
        }

        return result;
    }


    bool internalHistogramBuilder(
            List<unsigned long>& counts,
            unsigned long&       below,
            unsigned long&       above,
            Real                 lowerBound,
            Real                 upperBound,
            Real                 bucketWidths,
            const Tuple&         value
        ) {
        bool result = true;

        Tuple::ConstIterator it  = value.constBegin();
        Tuple::ConstIterator end = value.constEnd();
        while (it != end && result) {
            result = internalHistogramBuilder(counts, below, above, lowerBound, upperBound, bucketWidths, *it);
            ++it;
        }

        return result;
    }


    bool internalHistogramBuilder(
            List<unsigned long>& counts,
            unsigned long&       below,
            unsigned long&       above,
            Real                 lowerBound,
            Real                 upperBound,
            Real                 bucketWidths,
            const MatrixBoolean& value
        ) {
        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                internalHistogramBuilder(counts, below, above, lowerBound, upperBound, bucketWidths, value(ri, ci));
            }
        }

        return true;
    }


    bool internalHistogramBuilder(
            List<unsigned long>& counts,
            unsigned long&       below,
            unsigned long&       above,
            Real                 lowerBound,
            Real                 upperBound,
            Real                 bucketWidths,
            const MatrixInteger& value
        ) {
        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                internalHistogramBuilder(counts, below, above, lowerBound, upperBound, bucketWidths, value(ri, ci));
            }
        }

        return true;
    }


    bool internalHistogramBuilder(
            List<unsigned long>& counts,
            unsigned long&       below,
            unsigned long&       above,
            Real                 lowerBound,
            Real                 upperBound,
            Real                 bucketWidths,
            const MatrixReal&    value
        ) {
        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                internalHistogramBuilder(counts, below, above, lowerBound, upperBound, bucketWidths, value(ri, ci));
            }
        }

        return true;
    }


    bool internalHistogramBuilder(
            List<unsigned long>& counts,
            unsigned long&       below,
            unsigned long&       above,
            Real                 lowerBound,
            Real                 upperBound,
            Real                 bucketWidths,
            const MatrixComplex& value
        ) {
        bool result = true;

        Integer numberRows    = value.numberRows();
        Integer numberColumns = value.numberColumns();

        Integer ri = 1;
        while (ri <= numberRows && result) {
            Integer ci = 1;
            while (ci <= numberColumns && result) {
                result = internalHistogramBuilder(
                    counts,
                    below,
                    above,
                    lowerBound,
                    upperBound,
                    bucketWidths,
                    value(ri, ci)
                );

                ++ci;
            }

            ++ri;
        }

        return result;

    }


    bool internalHistogramBuilder(
            List<unsigned long>& counts,
            unsigned long&       below,
            unsigned long&       above,
            Real                 lowerBound,
            Real                 upperBound,
            Real                 bucketWidths,
            const Variant&       value
        ) {
        Real result;

        ValueType valueType = value.valueType();
        switch (valueType) {
            case ValueType::NONE: {
                result = false;
                break;
            }

            case ValueType::BOOLEAN: {
                result = internalHistogramBuilder(
                    counts,
                    below,
                    above,
                    lowerBound,
                    upperBound,
                    bucketWidths,
                    value.toBoolean()
                );

                break;
            }

            case ValueType::INTEGER: {
                result = internalHistogramBuilder(
                    counts,
                    below,
                    above,
                    lowerBound,
                    upperBound,
                    bucketWidths,
                    value.toInteger()
                );

                break;
            }

            case ValueType::REAL: {
                result = internalHistogramBuilder(
                    counts,
                    below,
                    above,
                    lowerBound,
                    upperBound,
                    bucketWidths,
                    value.toReal()
                );

                break;
            }

            case ValueType::COMPLEX: {
                result = internalHistogramBuilder(
                    counts,
                    below,
                    above,
                    lowerBound,
                    upperBound,
                    bucketWidths,
                    value.toComplex()
                );

                break;
            }

            case ValueType::SET: {
                result = internalHistogramBuilder(
                    counts,
                    below,
                    above,
                    lowerBound,
                    upperBound,
                    bucketWidths,
                    value.toSet()
                );

                break;
            }

            case ValueType::TUPLE: {
                result = internalHistogramBuilder(
                    counts,
                    below,
                    above,
                    lowerBound,
                    upperBound,
                    bucketWidths,
                    value.toTuple()
                );

                break;
            }

            case ValueType::MATRIX_BOOLEAN: {
                result = internalHistogramBuilder(
                    counts,
                    below,
                    above,
                    lowerBound,
                    upperBound,
                    bucketWidths,
                    value.toMatrixBoolean()
                );

                break;
            }

            case ValueType::MATRIX_INTEGER: {
                result = internalHistogramBuilder(
                    counts,
                    below,
                    above,
                    lowerBound,
                    upperBound,
                    bucketWidths,
                    value.toMatrixInteger()
                );

                break;
            }

            case ValueType::MATRIX_REAL: {
                result = internalHistogramBuilder(
                    counts,
                    below,
                    above,
                    lowerBound,
                    upperBound,
                    bucketWidths,
                    value.toMatrixReal()
                );

                break;
            }

            case ValueType::MATRIX_COMPLEX: {
                result = internalHistogramBuilder(
                    counts,
                    below,
                    above,
                    lowerBound,
                    upperBound,
                    bucketWidths,
                    value.toMatrixComplex()
                );

                break;
            }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                result = 0;
                break;
            }

            default: {
                assert(false);
                result = 0;
                break;
            }
        }

        return result;
    }


    List<unsigned long> internalBuildCountBuckets(Integer numberBuckets) {
        return List<unsigned long>(numberBuckets);
    }


    Tuple internalProcessHistogramCounts(
            const List<unsigned long>& counts,
            unsigned long              below,
            unsigned long              above,
            Real                       lowerBound,
            Real                       upperBound,
            Real                       bucketWidths
        ) {
        unsigned long numberBuckets = counts.size();
        unsigned long totalCounts   = 0;
        Real          bucketOffset  = (bucketWidths / 2.0) + lowerBound;

        MatrixInteger bucketCounts(counts.size(), 1);
        MatrixReal    pdf(counts.size(), 1);
        MatrixReal    bucketCenters(counts.size(), 1);

        for (unsigned long i=0 ; i<numberBuckets ; ++i) {
            unsigned long count = counts.at(i);

            bucketCounts.update(
                static_cast<MatrixInteger::Index>(i + 1),
                static_cast<MatrixInteger::Index>(1),
                static_cast<Integer>(count)
            );
            totalCounts += count;
        }

        for (unsigned long i=0 ; i<numberBuckets ; ++i) {
            unsigned long count = counts.at(i);

            pdf.update(
                static_cast<MatrixReal::Index>(i + 1),
                static_cast<MatrixReal::Index>(1),
                static_cast<Real>(count) / static_cast<Real>(totalCounts)
            );

            bucketCenters.update(
                static_cast<MatrixReal::Index>(i + 1),
                static_cast<MatrixReal::Index>(1),
                static_cast<MatrixReal::Scalar>(bucketOffset + bucketWidths * i)
            );
        }

        Tuple result;
        result.append(bucketCounts);
        result.append(pdf);
        result.append(bucketCenters);
        result.append(Integer(below));
        result.append(Integer(above));
        result.append(lowerBound);
        result.append(upperBound);
        result.append(bucketWidths);

        return result;
    }


    Tuple sort(const Set& set) {
        Tuple result;

        for (Set::ConstIterator it=set.constBegin(),end=set.constEnd() ; it!=end ; ++it) {
            result.append(*it);
        }

        return result;
    }


    Tuple sort(const Tuple& tuple) {
        class SortOrdering {
            public:
                bool operator()(const Variant& a, const Variant& b) const {
                    return implicitOrdering(a, b) < 0;
                }
        };

        unsigned long numberElements = tuple.size();
        std::vector<M::Variant> values(numberElements);

        for (unsigned long i=0 ; i<numberElements ; ++i) {
            values[i] = tuple.at(M::Integer(i + 1));
        }

        std::sort(values.begin(), values.end(), SortOrdering());

        Tuple result;
        for (unsigned long i=0 ; i<numberElements ; ++i) {
            result.append(values[i]);
        }

        return result;
    }


    MatrixBoolean sort(const MatrixBoolean& matrix) {
        Integer            numberRows    = matrix.numberRows();
        Integer            numberColumns = matrix.numberColumns();
        Integer            numberEntries = numberRows * numberColumns;

        unsigned long long numberOnes    = 0;

        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                if (matrix.at(ri, ci)) {
                    ++numberOnes;
                }
            }
        }

        unsigned long long numberZeros = numberEntries - numberOnes;
        MatrixBoolean      result(numberEntries, 1);

        for (Integer index=numberZeros + 1 ; index<=numberEntries ; ++index) {
            result.update(index, M::Integer(1), true);
        }

        return result;
    }


    MatrixInteger sort(const MatrixInteger& matrix) {
        Integer numberRows    = matrix.numberRows();
        Integer numberColumns = matrix.numberColumns();
        Integer numberEntries = numberRows * numberColumns;

        unsigned long        i = 0;
        std::vector<Integer> values(numberEntries);
        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                values[i] = matrix.at(ri, ci);
                ++i;
            }
        }

        std::sort(values.begin(), values.end());

        MatrixInteger result(numberEntries, 1);
        for (Integer index=0 ; index<numberEntries ; ++index) {
            result.update(index + 1, M::Integer(1), values.at(index));
        }

        return result;
    }


    MatrixReal sort(const MatrixReal& matrix) {
        Integer numberRows    = matrix.numberRows();
        Integer numberColumns = matrix.numberColumns();
        Integer numberEntries = numberRows * numberColumns;

        unsigned long     i = 0;
        std::vector<Real> values(numberEntries);
        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                values[i] = matrix.at(ri, ci);
                ++i;
            }
        }

        std::sort(values.begin(), values.end());

        MatrixReal result(numberEntries, 1);
        for (Integer index=0 ; index<numberEntries ; ++index) {
            result.update(index + 1, M::Integer(1), values.at(index));
        }

        return result;
    }


    MatrixComplex sort(const MatrixComplex& matrix) {
        class RealSortOrdering {
            public:
                bool operator()(const MatrixComplex::Scalar& a, const MatrixComplex::Scalar& b) const {
                    return a.real() < b.real();
                }
        };

        class ComplexSortOrdering {
            public:
                bool operator()(const MatrixComplex::Scalar& a, const MatrixComplex::Scalar& b) const {
                    return M::abs(a) < M::abs(b);
                }
        };

        Integer numberRows    = matrix.numberRows();
        Integer numberColumns = matrix.numberColumns();
        Integer numberEntries = numberRows * numberColumns;

        unsigned long i                = 0;
        bool          hasComplexValues = false;
        std::vector<MatrixComplex::Scalar> values(numberEntries);
        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                MatrixComplex::Scalar v = matrix.at(ri, ci);
                values[i] = v;

                if (v.imag() != 0) {
                    hasComplexValues = true;
                }

                ++i;
            }
        }

        if (hasComplexValues) {
            std::sort(values.begin(), values.end(), ComplexSortOrdering());
        } else {
            std::sort(values.begin(), values.end(), RealSortOrdering());
        }

        MatrixComplex result(numberEntries, 1);
        for (Integer index=0 ; index<numberEntries ; ++index) {
            result.update(index + 1, M::Integer(1), values.at(index));
        }

        return result;
    }


    Tuple sortDescending(const Set& set) {
        Tuple result;

        unsigned long numberEntries = set.size();
        for (unsigned long i=0 ; i<numberEntries ; ++i) {
            result.append(M::Variant());
        }

        unsigned long i = numberEntries;
        for (Set::ConstIterator it=set.constBegin(),end=set.constEnd() ; it!=end ; ++it) {
            result.update(i, *it);
            --i;
        }

        return result;
    }


    Tuple sortDescending(const Tuple& tuple) {
        class SortOrdering {
            public:
                bool operator()(const Variant& a, const Variant& b) const {
                    return implicitOrdering(a, b) > 0;
                }
        };

        unsigned long numberElements = tuple.size();
        std::vector<M::Variant> values(numberElements);

        for (unsigned long i=0 ; i<numberElements ; ++i) {
            values[i] = tuple.at(M::Integer(i + 1));
        }

        std::sort(values.begin(), values.end(), SortOrdering());

        Tuple result;
        for (unsigned long i=0 ; i<numberElements ; ++i) {
            result.append(values[i]);
        }

        return result;
    }


    MatrixBoolean sortDescending(const MatrixBoolean& matrix) {
        Integer            numberRows    = matrix.numberRows();
        Integer            numberColumns = matrix.numberColumns();
        Integer            numberEntries = numberRows * numberColumns;
        unsigned long long numberOnes    = 0;

        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                if (matrix.at(ri, ci)) {
                    ++numberOnes;
                }
            }
        }

        MatrixBoolean result(numberEntries, 1);

        for (Integer i=1 ; static_cast<unsigned long long>(i)<=numberOnes ; ++i) {
            result.update(i, M::Integer(1), true);
        }

        return result;
    }


    MatrixInteger sortDescending(const MatrixInteger& matrix) {
        Integer numberRows    = matrix.numberRows();
        Integer numberColumns = matrix.numberColumns();
        Integer numberEntries = numberRows * numberColumns;

        unsigned long        i = 0;
        std::vector<Integer> values(numberEntries);
        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                values[i] = matrix.at(ri, ci);
                ++i;
            }
        }

        std::sort(values.begin(), values.end(), std::greater<Integer>());

        MatrixInteger result(numberEntries, 1);
        for (Integer index=0 ; index<numberEntries ; ++index) {
            result.update(index + 1, M::Integer(1), values.at(index));
        }

        return result;
    }


    MatrixReal sortDescending(const MatrixReal& matrix) {
        Integer numberRows    = matrix.numberRows();
        Integer numberColumns = matrix.numberColumns();
        Integer numberEntries = numberRows * numberColumns;

        unsigned long     i = 0;
        std::vector<Real> values(numberEntries);
        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                values[i] = matrix.at(ri, ci);
                ++i;
            }
        }

        std::sort(values.begin(), values.end(), std::greater<Real>());

        MatrixReal result(numberEntries, 1);
        for (Integer index=0 ; index<numberEntries ; ++index) {
            result.update(index + 1, M::Integer(1), values.at(index));
        }

        return result;
    }


    MatrixComplex sortDescending(const MatrixComplex& matrix) {
        class RealSortOrdering {
            public:
                bool operator()(const MatrixComplex::Scalar& a, const MatrixComplex::Scalar& b) const {
                    return a.real() > b.real();
                }
        };

        class ComplexSortOrdering {
            public:
                bool operator()(const MatrixComplex::Scalar& a, const MatrixComplex::Scalar& b) const {
                    return M::abs(a) > M::abs(b);
                }
        };

        Integer numberRows    = matrix.numberRows();
        Integer numberColumns = matrix.numberColumns();
        Integer numberEntries = numberRows * numberColumns;

        unsigned long i                = 0;
        bool          hasComplexValues = false;
        std::vector<MatrixComplex::Scalar> values(numberEntries);
        for (Integer ri=1 ; ri<=numberRows ; ++ri) {
            for (Integer ci=1 ; ci<=numberColumns ; ++ci) {
                MatrixComplex::Scalar v = matrix.at(ri, ci);
                values[i] = v;

                if (v.imag() != 0) {
                    hasComplexValues = true;
                }

                ++i;
            }
        }

        if (hasComplexValues) {
            std::sort(values.begin(), values.end(), ComplexSortOrdering());
        } else {
            std::sort(values.begin(), values.end(), RealSortOrdering());
        }

        MatrixComplex result(numberEntries, 1);
        for (Integer index=0 ; index<numberEntries ; ++index) {
            result.update(index + 1, M::Integer(1), values.at(index));
        }

        return result;
    }
}
