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
* This file implements various hash functions for various types.
***********************************************************************************************************************/

#include <cassert>

#include "m_intrinsics.h"
#include "m_api_types.h"
#include "model_intrinsic_types.h"
#include "model_complex.h"
#include "model_set.h"
#include "model_tuple.h"
#include "model_matrix_boolean.h"
#include "model_matrix_integer.h"
#include "model_matrix_real.h"
#include "model_matrix_complex.h"
#include "m_set_iterator.h"
#include "m_set.h"
#include "m_tuple.h"
#include "m_matrix_boolean.h"
#include "m_matrix_integer.h"
#include "m_matrix_real.h"
#include "m_matrix_complex.h"
#include "m_variant.h"
#include "model_variant.h"
#include "m_unordered_set.h"
#include "m_unordered_set_base.h"

namespace M {
    UI64 hashFunction(const int& value, UI32 seed) {
        UI64 result = UnorderedSetBase::fnv1a32(seed);

        if (sizeof(int) == 2) {
            result = UnorderedSetBase::fnv1a16(static_cast<UI16>(value), result);
        } else if (sizeof(int) == 4) {
            result = UnorderedSetBase::fnv1a32(static_cast<UI32>(value), result);
        } else if (sizeof(int) == 8) {
            result = UnorderedSetBase::fnv1a64(static_cast<UI64>(value), result);
        }

        return result;
    }


    UI64 hashFunction(const unsigned& value, UI32 seed) {
        return hashFunction(static_cast<int>(value), seed);
    }


    UI64 hashFunction(const Model::Integer& value, UI32 seed) {
        UI64 result = UnorderedSetBase::fnv1a32(seed);
        result = UnorderedSetBase::fnv1a64(static_cast<UI64>(value), result);

        return result;
    }


    UI64 hashFunction(const Model::Boolean& value, UI32 seed) {
        UI64 result = UnorderedSetBase::fnv1a32(seed);
        result = UnorderedSetBase::fnv1a8(static_cast<UI8>(value), result);

        return result;
    }


    UI64 hashFunction(const Model::Real& value, UI32 seed) {
        UI64 result = UnorderedSetBase::fnv1a32(seed);

        union {
            Model::Real r;
            struct {
                UI64 l;
                UI16 h;
            } i;
        } u;

        u.i.l = u.i.h = 0;
        u.r   = value;

        result = UnorderedSetBase::fnv1a64(u.i.l, result);
        result = UnorderedSetBase::fnv1a64(u.i.h, result);

        return result;
    }


    UI64 hashFunction(const Model::Complex& value, UI32 seed) {
        UI64 result = UnorderedSetBase::fnv1a32(seed);

        union {
            Model::Real r;
            struct {
                UI64 l;
                UI16 h;
            } i;
        } u;

        u.i.l = u.i.h = 0;
        u.r   = value.real();

        result = UnorderedSetBase::fnv1a64(u.i.l, result);
        result = UnorderedSetBase::fnv1a64(u.i.h, result);

        u.r = value.imag();

        result = UnorderedSetBase::fnv1a64(u.i.l, result);
        result = UnorderedSetBase::fnv1a64(u.i.h, result);

        return result;
    }


    UI64 hashFunction(const Model::Set& value, UI32 seed) {
        UI64 result = UnorderedSetBase::fnv1a32(seed);

        for (  Model::Set::ConstIterator elementIterator = value.constBegin(), elementEndIterator = value.constEnd()
             ; elementIterator != elementEndIterator
             ; ++elementIterator
            ) {
            const Model::Variant& element = elementIterator.constReference();

            ++seed;
            result ^= hashFunction(element, seed);
        }

        return result;
    }


    UI64 hashFunction(const Set& value, UI32 seed) {
        UI64 result = UnorderedSetBase::fnv1a32(seed);

        for (  SetIterator elementIterator = value.constBegin(), elementEndIterator = value.constEnd()
             ; elementIterator != elementEndIterator
             ; ++elementIterator
            ) {
            const Variant& element = elementIterator.constReference();

            ++seed;
            result ^= hashFunction(element, seed);
        }

        return result;
    }


    UI64 hashFunction(const Model::Tuple& value, UI32 seed) {
        UI64 result = UnorderedSetBase::fnv1a32(seed);

        for (  Model::Tuple::ConstIterator elementIterator = value.constBegin(), elementEndIterator = value.constEnd()
             ; elementIterator != elementEndIterator
             ; ++elementIterator
            ) {
            const Model::Variant& element = elementIterator.constReference();

            ++seed;
            result ^= hashFunction(element, seed);
        }

        return result;
    }


    UI64 hashFunction(const Tuple& value, UI32 seed) {
        UI64 result = UnorderedSetBase::fnv1a32(seed);

        for (  Tuple::ConstIterator elementIterator = value.constBegin(), elementEndIterator = value.constEnd()
             ; elementIterator != elementEndIterator
             ; ++elementIterator
            ) {
            const Variant& element = elementIterator.constReference();

            ++seed;
            result ^= hashFunction(element, seed);
        }

        return result;
    }


    UI64 hashFunction(const Model::MatrixBoolean& value, UI32 seed) {
        UI64 result = UnorderedSetBase::fnv1a32(seed);

        unsigned long numberRows    = static_cast<unsigned long>(value.numberRows());
        unsigned long numberColumns = static_cast<unsigned long>(value.numberColumns());

        result = UnorderedSetBase::fnv1a32(static_cast<std::uint32_t>(numberRows), result);
        result = UnorderedSetBase::fnv1a32(static_cast<std::uint32_t>(numberColumns), result);

        for (unsigned long c=1 ; c<=numberColumns ; ++c) {
            for (unsigned long r=1 ; r<=numberRows ; ++r) {
                std::uint8_t v = value(r,c) ? 1 : 0;
                result = UnorderedSetBase::fnv1a8(v, result);
            }
        }

        return result;
    }


    UI64 hashFunction(const M::MatrixBoolean& value, UI32 seed) {
        UI64 result = UnorderedSetBase::fnv1a32(seed);

        Integer numberRows    = static_cast<unsigned long>(value.numberRows());
        Integer numberColumns = static_cast<unsigned long>(value.numberColumns());

        result = UnorderedSetBase::fnv1a32(static_cast<std::uint32_t>(numberRows), result);
        result = UnorderedSetBase::fnv1a32(static_cast<std::uint32_t>(numberColumns), result);

        for (Integer c=1 ; c<=numberColumns ; ++c) {
            for (Integer r=1 ; r<=numberRows ; ++r) {
                std::uint8_t v = value.at(static_cast<Integer>(r), static_cast<Integer>(c)) ? 1 : 0;
                result = UnorderedSetBase::fnv1a8(v, result);
            }
        }

        return result;
    }


    UI64 hashFunction(const Model::MatrixInteger& value, UI32 seed) {
        UI64 result = UnorderedSetBase::fnv1a32(seed);

        unsigned long numberRows    = static_cast<unsigned long>(value.numberRows());
        unsigned long numberColumns = static_cast<unsigned long>(value.numberColumns());

        result = UnorderedSetBase::fnv1a32(static_cast<std::uint32_t>(numberRows), result);
        result = UnorderedSetBase::fnv1a32(static_cast<std::uint32_t>(numberColumns), result);

        for (unsigned long c=1 ; c<=numberColumns ; ++c) {
            for (unsigned long r=1 ; r<=numberRows ; ++r) {
                std::uint64_t v = static_cast<std::uint64_t>(value(r,c));
                result = UnorderedSetBase::fnv1a64(v, result);
            }
        }

        return result;
    }


    UI64 hashFunction(const M::MatrixInteger& value, UI32 seed) {
        UI64 result = UnorderedSetBase::fnv1a32(seed);

        Integer numberRows    = static_cast<unsigned long>(value.numberRows());
        Integer numberColumns = static_cast<unsigned long>(value.numberColumns());

        result = UnorderedSetBase::fnv1a32(static_cast<std::uint32_t>(numberRows), result);
        result = UnorderedSetBase::fnv1a32(static_cast<std::uint32_t>(numberColumns), result);

        for (Integer c=1 ; c<=numberColumns ; ++c) {
            for (Integer r=1 ; r<=numberRows ; ++r) {
                std::uint64_t v = static_cast<std::uint64_t>(value(r,c));
                result = UnorderedSetBase::fnv1a64(v, result);
            }
        }

        return result;
    }


    UI64 hashFunction(const Model::MatrixReal& value, UI32 seed) {
        UI64 result = UnorderedSetBase::fnv1a32(seed);

        unsigned long numberRows    = static_cast<unsigned long>(value.numberRows());
        unsigned long numberColumns = static_cast<unsigned long>(value.numberColumns());

        result = UnorderedSetBase::fnv1a32(static_cast<std::uint32_t>(numberRows), result);
        result = UnorderedSetBase::fnv1a32(static_cast<std::uint32_t>(numberColumns), result);

        for (unsigned long c=1 ; c<=numberColumns ; ++c) {
            for (unsigned long r=1 ; r<=numberRows ; ++r) {
                union {
                    std::uint64_t i;
                    Real          f;
                } u;

                u.f = value(r,c);
                result = UnorderedSetBase::fnv1a64(u.i, result);
            }
        }

        return result;
    }


    UI64 hashFunction(const M::MatrixReal& value, UI32 seed) {
        UI64 result = UnorderedSetBase::fnv1a32(seed);

        Integer numberRows    = static_cast<unsigned long>(value.numberRows());
        Integer numberColumns = static_cast<unsigned long>(value.numberColumns());

        result = UnorderedSetBase::fnv1a32(static_cast<std::uint32_t>(numberRows), result);
        result = UnorderedSetBase::fnv1a32(static_cast<std::uint32_t>(numberColumns), result);

        for (Integer c=1 ; c<=numberColumns ; ++c) {
            for (Integer r=1 ; r<=numberRows ; ++r) {
                union {
                    std::uint64_t i;
                    Real          f;
                } u;

                u.f = value(r,c);
                result = UnorderedSetBase::fnv1a64(u.i, result);
            }
        }

        return result;
    }


    UI64 hashFunction(const Model::MatrixComplex& value, UI32 seed) {
        UI64 result = UnorderedSetBase::fnv1a32(seed);

        unsigned long numberRows    = static_cast<unsigned long>(value.numberRows());
        unsigned long numberColumns = static_cast<unsigned long>(value.numberColumns());

        result = UnorderedSetBase::fnv1a32(static_cast<std::uint32_t>(numberRows), result);
        result = UnorderedSetBase::fnv1a32(static_cast<std::uint32_t>(numberColumns), result);

        for (unsigned long c=1 ; c<=numberColumns ; ++c) {
            for (unsigned long r=1 ; r<=numberRows ; ++r) {
                union {
                    std::uint64_t i;
                    Real          f;
                } u;

                const Model::Complex& v = value(r,c);
                u.f = v.real();
                result = UnorderedSetBase::fnv1a64(u.i, result);

                u.f = v.imag();
                result = UnorderedSetBase::fnv1a64(u.i, result);
            }
        }

        return result;
    }


    UI64 hashFunction(const M::MatrixComplex& value, UI32 seed) {
        UI64 result = UnorderedSetBase::fnv1a32(seed);

        Integer numberRows    = static_cast<unsigned long>(value.numberRows());
        Integer numberColumns = static_cast<unsigned long>(value.numberColumns());

        result = UnorderedSetBase::fnv1a32(static_cast<std::uint32_t>(numberRows), result);
        result = UnorderedSetBase::fnv1a32(static_cast<std::uint32_t>(numberColumns), result);

        for (Integer c=1 ; c<=numberColumns ; ++c) {
            for (Integer r=1 ; r<=numberRows ; ++r) {
                union {
                    std::uint64_t i;
                    Real          f;
                } u;

                const Model::Complex& v = value(r,c);
                u.f = v.real();
                result = UnorderedSetBase::fnv1a64(u.i, result);

                u.f = v.imag();
                result = UnorderedSetBase::fnv1a64(u.i, result);
            }
        }

        return result;
    }


    UI64 hashFunction(const Model::Variant& value, UI32 seed) {
        UI64         result = 0;
        ValueType valueType = value.valueType();

        switch (valueType) {
            case ValueType::NONE: {
                // Hash is fine as stands.
                break;
            }

            case ValueType::BOOLEAN: {
                Model::Integer i = value.toBoolean();
                result = hashFunction(i, seed);

                break;
            }

            case ValueType::INTEGER: {
                Model::Integer i = value.toInteger();
                result = hashFunction(i, seed);

                break;
            }

            case ValueType::REAL: {
                Model::Real r = value.toReal();
                result = hashFunction(r, seed);

                break;
            }

            case ValueType::COMPLEX: {
                Model::Complex c = value.toComplex();
                result = hashFunction(c, seed);

                break;
            }

            case ValueType::SET: {
                Set s = value.toSet();
                result = hashFunction(s, seed);

                break;
            }

            case ValueType::MATRIX_BOOLEAN: {
                Model::MatrixBoolean m = value.toMatrixBoolean();
                result = hashFunction(m, seed);

                break;
            }

            case ValueType::MATRIX_INTEGER: {
                Model::MatrixInteger m = value.toMatrixInteger();
                result = hashFunction(m, seed);

                break;
            }

            case ValueType::MATRIX_REAL: {
                Model::MatrixReal m = value.toMatrixReal();
                result = hashFunction(m, seed);

                break;
            }

            case ValueType::MATRIX_COMPLEX: {
                Model::MatrixComplex m = value.toMatrixComplex();
                result = hashFunction(m, seed);

                break;
            }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        return UnorderedSetBase::fnv1a8(static_cast<UI8>(valueType), result);
    }


    UI64 hashFunction(const Variant& value, UI32 seed) {
        UI64         result    = 0;
        ValueType valueType = value.valueType();

        switch (valueType) {
            case ValueType::NONE: {
                // Hash is fine as stands.
                break;
            }

            case ValueType::BOOLEAN: {
                Model::Boolean b = value.toBoolean();
                result = hashFunction(b, seed);

                break;
            }

            case ValueType::INTEGER: {
                Model::Integer i = value.toInteger();
                result = hashFunction(i, seed);

                break;
            }

            case ValueType::REAL: {
                Model::Real r = value.toReal();
                result = hashFunction(r, seed);

                break;
            }

            case ValueType::COMPLEX: {
                Model::Complex c = value.toComplex();
                result = hashFunction(c, seed);

                break;
            }

            case ValueType::SET: {
                Set s = value.toSet();
                result = hashFunction(s, seed);

                break;
            }

            case ValueType::TUPLE: {
                Tuple s = value.toTuple();
                result = hashFunction(s, seed);

                break;
            }

            case ValueType::MATRIX_BOOLEAN: {
                MatrixBoolean m = value.toMatrixBoolean();
                result = hashFunction(m, seed);

                break;
            }

            case ValueType::MATRIX_INTEGER: {
                MatrixInteger m = value.toMatrixInteger();
                result = hashFunction(m, seed);

                break;
            }

            case ValueType::MATRIX_REAL: {
                MatrixReal m = value.toMatrixReal();
                result = hashFunction(m, seed);

                break;
            }

            case ValueType::MATRIX_COMPLEX: {
                MatrixComplex m = value.toMatrixComplex();
                result = hashFunction(m, seed);

                break;
            }

            case ValueType::NUMBER_TYPES: {
                assert(false);
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        return UnorderedSetBase::fnv1a8(static_cast<UI8>(valueType), result);
    }
}
