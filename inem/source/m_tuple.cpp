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
* This file implements the \ref Model::Tuple class.
***********************************************************************************************************************/

#include <cmath>
#include <limits>
#include <algorithm>

#include "model_intrinsic_types.h"
#include "model_exceptions.h"
#include "m_exceptions.h"
#include "m_range.h"
#include "m_set.h"
#include "m_set_iterator.h"
#include "m_matrix_integer.h"
#include "m_matrix_real.h"
#include "m_matrix_complex.h"
#include "m_variant.h"
#include "model_tuple.h"
#include "model_tuple_private.h"
#include "m_tuple.h"

/***********************************************************************************************************************
 * M::Tuple::IteratorBase
 */

namespace M {
    Tuple::IteratorBase::~IteratorBase() {}


    Variant Tuple::IteratorBase::value() const {
        return currentTuple->at(Integer(currentIndex + 1));
    }


    const Variant& Tuple::IteratorBase::constReference() const {
        return currentTuple->at(Integer(currentIndex + 1));
    }


    const Variant* Tuple::IteratorBase::constPointer() const {
        return &currentTuple->at(Integer(currentIndex + 1));
    }


    void Tuple::IteratorBase::advance() {
        if (static_cast<unsigned long long>(currentTuple->size()) > currentIndex) {
            ++currentIndex;
        }
    }


    void Tuple::IteratorBase::advance(signed long long distance) {
        unsigned long long currentSize = static_cast<unsigned long long>(currentTuple->size());
        if (distance >= 0) {
            if (static_cast<unsigned long long>(currentIndex + distance) < currentSize) {
                currentIndex += static_cast<signed long>(distance);
            } else {
                currentIndex = static_cast<unsigned long>(currentSize);
            }
        } else {
            if (currentIndex >= static_cast<unsigned long long>(-distance)) {
                currentIndex += static_cast<signed long>(distance);
            } else {
                currentIndex = static_cast<unsigned long>(currentSize);
            }
        }
    }


    void Tuple::IteratorBase::backup() {
        if (currentIndex > 0) {
            --currentIndex;
        } else {
            currentIndex = static_cast<unsigned long>(currentTuple->size());
        }
    }


    void Tuple::IteratorBase::backup(signed long long distance) {
        advance(-distance);
    }


    bool Tuple::IteratorBase::isValid() const {
        return currentTuple != nullptr && currentIndex < static_cast<unsigned long long>(currentTuple->size());
    }


    bool Tuple::IteratorBase::isInvalid() const {
        return !isValid();
    }


    signed long long Tuple::IteratorBase::operator-(const Tuple::IteratorBase& other) const {
        return currentIndex - other.currentIndex;
    }


    bool Tuple::IteratorBase::operator==(const Tuple::IteratorBase& other) const {
        return currentIndex == other.currentIndex;
    }


    bool Tuple::IteratorBase::operator!=(const Tuple::IteratorBase& other) const {
        return currentIndex != other.currentIndex;
    }


    Tuple::IteratorBase& Tuple::IteratorBase::operator=(const Tuple::IteratorBase& other) {
        currentTuple = other.currentTuple;
        currentIndex = other.currentIndex;

        return *this;
    }


    Tuple::IteratorBase::IteratorBase():currentTuple(nullptr),currentIndex(0) {}


    Tuple::IteratorBase::IteratorBase(
            const Tuple*  newTuple,
            unsigned long newIndex
        ):currentTuple(
            newTuple
        ),currentIndex(
            newIndex
        ) {}


    Tuple::IteratorBase::IteratorBase(
            const Tuple::IteratorBase& other
        ):currentTuple(
            other.currentTuple
        ),currentIndex(
            other.currentIndex
        ) {}
}

/***********************************************************************************************************************
 * M::Tuple::ConstIterator
 */

namespace M {
    Tuple::ConstIterator::ConstIterator() {}


    Tuple::ConstIterator::ConstIterator(const Tuple::ConstIterator& other):Tuple::IteratorBase(other) {}


    Tuple::ConstIterator::~ConstIterator() {}


    const Variant& Tuple::ConstIterator::operator*() const {
        return currentTuple->at(Integer(currentIndex + 1));
    }


    const Variant* Tuple::ConstIterator::operator->() const {
        return &currentTuple->at(Integer(currentIndex + 1));
    }


    Tuple::ConstIterator& Tuple::ConstIterator::operator++() {
        advance();
        return *this;
    }


    Tuple::ConstIterator Tuple::ConstIterator::operator++(int) {
        Tuple::ConstIterator result = *this;
        advance();

        return result;
    }


    Tuple::ConstIterator& Tuple::ConstIterator::operator--() {
        backup();
        return *this;
    }


    Tuple::ConstIterator Tuple::ConstIterator::operator--(int) {
        Tuple::ConstIterator result = *this;
        backup();

        return result;
    }


    Tuple::ConstIterator& Tuple::ConstIterator::operator+=(signed long long distance) {
        advance(distance);
        return *this;
    }


    Tuple::ConstIterator& Tuple::ConstIterator::operator-=(signed long long distance) {
        backup(distance);
        return *this;
    }


    Tuple::ConstIterator Tuple::ConstIterator::operator+(signed long long distance) const {
        Tuple::ConstIterator result = *this;
        result.advance(distance);

        return result;
    }


    Tuple::ConstIterator Tuple::ConstIterator::operator-(signed long long distance) const {
        Tuple::ConstIterator result = *this;
        result.backup(distance);

        return result;
    }


    Tuple::ConstIterator& Tuple::ConstIterator::operator=(const Tuple::ConstIterator& other) {
        IteratorBase::operator=(other);
        return *this;
    }


    Tuple::ConstIterator::ConstIterator(
            const Tuple*  newTuple,
            unsigned long newIndex
        ):IteratorBase(
            newTuple,
            newIndex
        ) {}
}

/***********************************************************************************************************************
 * M::Tuple::Iterator
 */

namespace M {
    Tuple::Iterator::Iterator() {}


    Tuple::Iterator::Iterator(const Tuple::Iterator& other):Tuple::IteratorBase(other) {}


    Tuple::Iterator::~Iterator() {}


    Variant& Tuple::Iterator::reference() const {
        return (*const_cast<class Tuple*>(currentTuple))[currentIndex + 1];
    }


    Variant* Tuple::Iterator::pointer() const {
        return &(*const_cast<class Tuple*>(currentTuple))[currentIndex + 1];
    }


    Variant& Tuple::Iterator::operator*() const {
        return (*const_cast<class Tuple*>(currentTuple))[currentIndex + 1];
    }


    Variant* Tuple::Iterator::operator->() const {
        return &(*const_cast<class Tuple*>(currentTuple))[currentIndex + 1];
    }


    Tuple::Iterator& Tuple::Iterator::operator++() {
        advance();
        return *this;
    }


    Tuple::Iterator Tuple::Iterator::operator++(int) {
        Tuple::Iterator result = *this;
        advance();

        return result;
    }


    Tuple::Iterator& Tuple::Iterator::operator--() {
        backup();
        return *this;
    }


    Tuple::Iterator Tuple::Iterator::operator--(int) {
        Tuple::Iterator result = *this;
        backup();

        return result;
    }


    Tuple::Iterator& Tuple::Iterator::operator+=(signed long long distance) {
        advance(distance);
        return *this;
    }


    Tuple::Iterator& Tuple::Iterator::operator-=(signed long long distance) {
        backup(distance);
        return *this;
    }


    Tuple::Iterator Tuple::Iterator::operator+(signed long long distance) const {
        Tuple::Iterator result = *this;
        result.advance(distance);

        return result;
    }


    Tuple::Iterator Tuple::Iterator::operator-(signed long long distance) const {
        Tuple::Iterator result = *this;
        result.backup(distance);

        return result;
    }


    Tuple::Iterator& Tuple::Iterator::operator=(const Tuple::Iterator& other) {
        IteratorBase::operator=(other);
        return *this;
    }


    Tuple::Iterator::Iterator(
            Tuple*        newTuple,
            unsigned long newIndex
        ):IteratorBase(
            newTuple,
            newIndex
        ) {}
}

/***********************************************************************************************************************
 * M::Tuple
 */

namespace M {
    template<typename I> static Tuple atIHelper(const Tuple& sourceTuple, const I& iterable) {
        Tuple result;

        for (typename I::ConstIterator it=iterable.constBegin(),end=iterable.constEnd() ; it!=end ; ++it) {
            Integer sourceIndex = it->toInteger();
            result.append(sourceTuple.at(sourceIndex));
        }

        return result;
    }


    template<typename M> static Tuple atMHelper(const Tuple& sourceTuple, const M& matrix) {
        Tuple   result;

        Integer destinationNumberValues = matrix.numberCoefficients();
        for (Integer index=1 ; index<=destinationNumberValues ; ++index) {
            Integer sourceIndex = inlineToInteger(matrix.at(index));
            result.append(sourceTuple.at(sourceIndex));
        }

        return result;
    }


    const Variant Tuple::dummyVariant;

    Tuple::Tuple() {}


    Tuple::Tuple(const Variant& other) {
        bool  ok;
        Tuple v = other.toTuple(&ok);

        if (ok) {
            Model::Tuple::operator=(v);
        } else {
            internalTriggerTypeConversionError(other.valueType(), ValueType::TUPLE);
        }
    }


    Tuple::Tuple(const char* str):Model::Tuple(str) {}


    Tuple::Tuple(const Model::Tuple& other):Model::Tuple(other) {}


    Tuple::Tuple(Model::Tuple&& other):Model::Tuple(other) {}


    Tuple::Tuple(const M::Tuple& other):Model::Tuple(other) {}


    Tuple::Tuple(M::Tuple&& other):Model::Tuple(other) {}


    Tuple::~Tuple() {}


    void Tuple::append(const Variant& newValue) {
        impl->lock();
        if (impl->referenceCount() == 1) {
            impl->append(newValue);
            impl->unlock();
        } else {
            Private* oldImplementation = impl;
            impl = new Private(*impl);

            oldImplementation->removeReference();
            oldImplementation->unlock();

            impl->append(newValue);
        }
    }


    void Tuple::append(const Range& range) {
        impl->lock();
        if (impl->referenceCount() == 1) {
            impl->append(range);
            impl->unlock();
        } else {
            Private* oldImplementation = impl;
            impl = new Private(*impl);

            oldImplementation->removeReference();
            oldImplementation->unlock();

            impl->append(range);
        }
    }


    void Tuple::prepend(const Variant& newValue) {
        impl->lock();
        if (impl->referenceCount() == 1) {
            impl->prepend(newValue);
            impl->unlock();
        } else {
            Private* oldImplementation = impl;
            impl = new Private(*impl);

            oldImplementation->removeReference();
            oldImplementation->unlock();

            impl->prepend(newValue);
        }
    }


    void Tuple::prepend(const Range& range) {
        impl->lock();
        if (impl->referenceCount() == 1) {
            impl->prepend(range);
            impl->unlock();
        } else {
            Private* oldImplementation = impl;
            impl = new Private(*impl);

            oldImplementation->removeReference();
            oldImplementation->unlock();

            impl->prepend(range);
        }
    }


    void Tuple::push_back(const Variant& newValue) {
        append(newValue);
    }


    void Tuple::push_front(const Variant& newValue) {
        prepend(newValue);
    }


    void Tuple::push_back(const Range& range) {
        append(range);
    }


    void Tuple::push_front(const Range& range) {
        prepend(range);
    }


    Variant Tuple::first() const {
        return impl->first();
    }


    Variant Tuple::last() const {
        return impl->last();
    }


    Variant Tuple::takeFirst() {
        Variant result;

        impl->lock();
        if (impl->referenceCount() == 1) {
            result = impl->takeFirst();
            impl->unlock();
        } else {
            Private* oldImplementation = impl;
            impl = new Private(*impl);

            oldImplementation->removeReference();
            oldImplementation->unlock();

            result = impl->takeFirst();
        }

        return result;
    }


    Variant Tuple::takeLast() {
        Variant result;

        impl->lock();
        if (impl->referenceCount() == 1) {
            result = impl->takeLast();
            impl->unlock();
        } else {
            Private* oldImplementation = impl;
            impl = new Private(*impl);

            oldImplementation->removeReference();
            oldImplementation->unlock();

            result = impl->takeLast();
        }

        return result;
    }


    const Variant& Tuple::at(Integer index) const {
        if (index > 0 && static_cast<unsigned long long>(index) <= impl->size()) {
            return impl->at(index - 1);
        } else {
            internalTriggerInvalidParameterValueError();
            return dummyVariant;
        }
    }


    const Variant& Tuple::at(Real index) const {
        Integer i;
        if (toInteger(i, index)) {
            return at(i);
        } else {
            internalTriggerInvalidParameterValueError();
            return dummyVariant;
        }
    }


    const Variant& Tuple::at(const Complex& index) const {
        Integer i;
        if (toInteger(i, index)) {
            return at(i);
        } else {
            internalTriggerInvalidParameterValueError();
            return dummyVariant;
        }
    }


    Tuple Tuple::at(const Range& index) const {
        return atIHelper(*this, index);
    }


    Tuple Tuple::at(const Set& index) const {
        return atIHelper(*this, index);
    }


    Tuple Tuple::at(const Tuple& index) const {
        return atIHelper(*this, index);
    }


    Tuple Tuple::at(const MatrixInteger& index) const {
        return atMHelper(*this, index);
    }


    Tuple Tuple::at(const MatrixReal& index) const {
        return atMHelper(*this, index);
    }


    Tuple Tuple::at(const MatrixComplex& index) const {
        return atMHelper(*this, index);
    }


    Variant Tuple::at(const Variant& index) const {
        ValueType indexType = index.valueType();
        switch (indexType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::BOOLEAN:
            case ValueType::MATRIX_BOOLEAN: {
                internalTriggerInvalidParameterValueError();
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }

            case ValueType::INTEGER:
            case ValueType::REAL:
            case ValueType::COMPLEX: {
                return at(index.toInteger());
            }

            case ValueType::SET: {
                return Variant(at(index.toSet()));
            }

            case ValueType::TUPLE: {
                return Variant(at(index.toTuple()));
            }

            case ValueType::MATRIX_INTEGER: {
                return Variant(at(index.toMatrixInteger()));
            }

            case ValueType::MATRIX_REAL: {
                return Variant(at(index.toMatrixReal()));
            }

            case ValueType::MATRIX_COMPLEX: {
                return Variant(at(index.toMatrixComplex()));
            }

            default: {
                assert(false);
                return Variant(std::numeric_limits<Real>::quiet_NaN());
            }
        }
    }


    void Tuple::update(unsigned long index, const Variant& newValue) {
        impl->lock();

        unsigned long currentSize = impl->size();
        if (index == 0) {
            throw Model::InvalidIndex(index, currentSize);
        } else {
            if (impl->referenceCount() == 1) {
                if (index <= impl->size()) {
                    (*impl)[index - 1] = newValue;
                } else {
                    while ((index - 1) > impl->size()) {
                        impl->append(M::Variant(M::Integer(0)));
                    }

                    impl->append(newValue);
                }

                impl->unlock();
            } else {
                Private* oldImplementation = impl;
                impl = new Private(*impl);

                oldImplementation->removeReference();
                oldImplementation->unlock();

                if (index <= impl->size()) {
                    (*impl)[index - 1] = newValue;
                } else {
                    while ((index - 1) > impl->size()) {
                        impl->append(M::Variant(M::Integer(0)));
                    }

                    impl->append(newValue);
                }
            }
        }
    }


    Tuple::ConstIterator Tuple::constBegin() const {
        return ConstIterator(this, 0);
    }


    Tuple::ConstIterator Tuple::constEnd() const {
        return ConstIterator(this, static_cast<unsigned long>(size()));
    }


    Tuple::const_iterator Tuple::cbegin() const {
        return constBegin();
    }


    Tuple::const_iterator Tuple::cend() const {
        return constEnd();
    }


    Tuple::Iterator Tuple::begin() {
        return Iterator(this, 0);
    }


    Tuple::Iterator Tuple::end() {
        return Iterator(this, static_cast<unsigned long>(size()));
    }


    int Tuple::relativeOrder(const Tuple& other) const {
        int result = 0;

        if (impl != other.impl) {
            unsigned long thisSize    = static_cast<unsigned long>(impl->size());
            unsigned long otherSize   = static_cast<unsigned long>(other.impl->size());
            unsigned long minimumSize = std::min(thisSize, otherSize);
            unsigned long index       = 0;

            while (result == 0 && index < minimumSize) {
                const M::Variant& thisValue  = impl->at(index);
                const M::Variant& otherValue = other.impl->at(index);

                if (thisValue < otherValue) {
                    result = -1;
                } else if (thisValue > otherValue) {
                    result = +1;
                }

                ++index;
            }

            if (result == 0) {
                result = static_cast<long>(thisSize) - static_cast<long>(otherSize);
            }
        }

        return result;
    }


    const Variant& Tuple::operator[](unsigned long index) const {
        unsigned long currentSize = impl->size();

        if (index == 0) {
            throw Model::InvalidIndex(index, currentSize);
        }

        return index <= currentSize ? impl->at(index) : dummyVariant;
    }


    Variant& Tuple::operator[](unsigned long index) {
        Variant* result;

        impl->lock();
        if (impl->referenceCount() > 1) {
            Private* oldImplementation = impl;
            impl = new Private(*impl);

            oldImplementation->removeReference();
        }

        while (index > impl->size()) {
            impl->append(M::Variant());
        }

        result = &(*impl)[index - 1];
        impl->unlock();

        return *result;
    }


    Tuple Tuple::operator*(const Tuple& other) const {
        return Tuple(Model::Tuple::operator*(other));
    }


    Tuple& Tuple::operator*=(const Tuple& other) {
        Model::Tuple::operator*=(other);
        return *this;
    }


    Tuple Tuple::operator/(const Tuple& other) const {
        return Tuple(Model::Tuple::operator/(other));
    }


    Tuple& Tuple::operator/=(const Tuple& other) {
        Model::Tuple::operator/=(other);
        return *this;
    }


    bool Tuple::operator==(const Tuple& other) const {
        return impl == other.impl || *impl == *other.impl;
    }


    Tuple& Tuple::operator=(const Tuple& other) {
        Model::Tuple::operator=(other);
        return *this;
    }


    Tuple& Tuple::operator=(Tuple&& other) {
        Model::Tuple::operator=(other);
        return *this;
    }


    Tuple& Tuple::operator=(const Variant& other) {
        bool  ok;
        Tuple v = other.toTuple(&ok);

        if (ok) {
            operator=(v);
        } else {
            throw Model::InvalidRuntimeConversion(other.valueType(), ValueType::TUPLE);
        }

        return *this;
    }
}
