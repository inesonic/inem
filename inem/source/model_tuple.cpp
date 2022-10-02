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

#include "model_intrinsic_types.h"
#include "model_variant.h"
#include "model_variant_private.h"
#include "model_exceptions.h"
#include "model_range.h"
#include "model_tuple_iterator_base.h"
#include "model_tuple_iterator_base_private.h"
#include "model_tuple_const_iterator.h"
#include "model_tuple_iterator.h"
#include "model_tuple.h"
#include "model_tuple_private.h"

namespace Model {
    Tuple::Tuple():impl(new Private) {}


    Tuple::Tuple(const Variant& other) {
        bool  ok;
        Tuple v = other.toTuple(&ok);

        if (ok) {
            v.impl->addReference();
            impl = v.impl;
        } else {
            throw Model::InvalidRuntimeConversion(other.valueType(), ValueType::TUPLE);
        }
    }


    Tuple::Tuple(const char* str):impl(new Private(str)) {}


    Tuple::Tuple(const Tuple& other) {
        other.impl->addReference();
        impl = other.impl;
    }


    Tuple::Tuple(Tuple&& other) {
        impl = other.impl;
        other.impl = nullptr;
    }


    Tuple::~Tuple() {
        if (impl != nullptr && impl->removeReference()) {
            delete impl;
        }
    }


    Tuple Tuple::build() {
        return Tuple();
    }


    Variant Tuple::value(Tuple::Index index) const {
        return   index > 0 && static_cast<unsigned long long>(index) <= impl->size()
               ? Variant(new Variant::Private(impl->at(index - 1)))
               : Variant();
    }


    bool Tuple::setValue(Tuple::Index index, const Variant& value) {
        bool success = false;

        if (index > 0) {
            update(index, value);
            success = true;
        }

        return success;
    }


    bool Tuple::isEmpty() const {
        return impl->isEmpty();
    }


    Tuple::Index Tuple::size() const {
        return static_cast<Model::Integer>(impl->size());
    }


    void Tuple::clear() {
        impl->lock();
        if (impl->referenceCount() == 1) {
            impl->clear();
            impl->unlock();
        } else {
            Private* oldImplementation = impl;
            impl = new Private;

            oldImplementation->unlock();
            oldImplementation->removeReference();
        }
    }


    void Tuple::append(const Variant& newValue) {
        impl->lock();
        if (impl->referenceCount() == 1) {
            impl->append(*newValue.impl);
            impl->unlock();
        } else {
            Private* oldImplementation = impl;
            impl = new Private(*impl);

            oldImplementation->removeReference();
            oldImplementation->unlock();

            impl->append(*newValue.impl);
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
            impl->prepend(*newValue.impl);
            impl->unlock();
        } else {
            Private* oldImplementation = impl;
            impl = new Private(*impl);

            oldImplementation->removeReference();
            oldImplementation->unlock();

            impl->prepend(*newValue.impl);
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


    void Tuple::push_back(const Range& newValue) {
        append(newValue);
    }


    void Tuple::push_front(const Range& newValue) {
        prepend(newValue);
    }


    Variant Tuple::first() const {
        return Variant(new Variant::Private(impl->first()));
    }


    Variant Tuple::last() const {
        return Variant(new Variant::Private(impl->last()));
    }


    Variant Tuple::takeFirst() {
        Variant::Private* result;

        impl->lock();
        if (impl->referenceCount() == 1) {
            result = new Variant::Private(impl->takeFirst());
            impl->unlock();
        } else {
            Private* oldImplementation = impl;
            impl = new Private(*impl);

            oldImplementation->removeReference();
            oldImplementation->unlock();

            result = new Variant::Private(impl->takeFirst());
        }

        return Variant(result);
    }


    Variant Tuple::takeLast() {
        Variant::Private* result;

        impl->lock();
        if (impl->referenceCount() == 1) {
            result = new Variant::Private(impl->takeLast());
            impl->unlock();
        } else {
            Private* oldImplementation = impl;
            impl = new Private(*impl);

            oldImplementation->removeReference();
            oldImplementation->unlock();

            result = new Variant::Private(impl->takeLast());
        }

        return Variant(result);
    }


    bool Tuple::popFirst() {
        bool result;

        if (impl->isEmpty()) {
            result = false;
        } else {
            impl->lock();
            if (impl->referenceCount() == 1) {
                (void) impl->takeFirst();
                impl->unlock();
            } else {
                Private* oldImplementation = impl;
                impl = new Private(*impl);

                oldImplementation->removeReference();
                oldImplementation->unlock();

                (void) impl->takeFirst();
            }

            result = true;
        }

        return result;
    }


    bool Tuple::popLast() {
        bool result;

        if (impl->isEmpty()) {
            result = false;
        } else {
            impl->lock();
            if (impl->referenceCount() == 1) {
                (void) impl->takeLast();
                impl->unlock();
            } else {
                Private* oldImplementation = impl;
                impl = new Private(*impl);

                oldImplementation->removeReference();
                oldImplementation->unlock();

                (void) impl->takeLast();
            }

            result = true;
        }

        return result;
    }


    void Tuple::pop_front() {
        popFirst();
    }


    void Tuple::pop_back() {
        popLast();
    }


    Variant Tuple::at(Tuple::Index index) const {
        Variant result;

        if (index > 0 && static_cast<unsigned long long>(index) <= impl->size()) {
            result = Variant(new Variant::Private(impl->at(index - 1)));
        } else {
            throw InvalidIndex(index, impl->size());
        }

        return result;
    }


    void Tuple::update(Index index, const Variant& newValue) {
        if (index > 0) {
            unsigned long long i = static_cast<unsigned long long>(index - 1);

            impl->lock();
            if (impl->referenceCount() == 1) {
                if (i < impl->size()) {
                    (*impl)[i] = *newValue.impl;
                } else {
                    while (i > impl->size()) {
                        impl->append(M::Variant());
                    }

                    impl->append(*newValue.impl);
                }

                impl->unlock();
            } else {
                Private* oldImplementation = impl;
                impl = new Private(*impl);

                oldImplementation->unlock();

                if (oldImplementation->removeReference()) {
                    delete oldImplementation;
                }

                if (i < impl->size()) {
                    (*impl)[i] = *newValue.impl;
                } else {
                    while (i > impl->size()) {
                        impl->append(M::Variant());
                    }

                    impl->append(*newValue.impl);
                }
            }
        } else {
            throw InvalidIndex(index, impl->size());
        }
    }


    Tuple::ConstIterator Tuple::constBegin() const {
        return TupleConstIterator(new TupleIteratorBase::Private(this));
    }


    Tuple::ConstIterator Tuple::constEnd() const {
        return TupleConstIterator(new TupleIteratorBase::Private(this, impl->size()));
    }


    Tuple::const_iterator Tuple::cbegin() const {
        return constBegin();
    }


    Tuple::const_iterator Tuple::cend() const {
        return constEnd();
    }


    Tuple::Iterator Tuple::begin() {
        return TupleIterator(new TupleIteratorBase::Private(this));
    }


    Tuple::Iterator Tuple::end() {
        return TupleIterator(new TupleIteratorBase::Private(this, impl->size()));
    }


    char* Tuple::toString() const {
        unsigned stringAllocation = 1;
        bool     isValid          = true;
        unsigned stringLength     = static_cast<unsigned>(impl->size());
        unsigned i                = 0;

        while (isValid && i < stringLength) {
            Integer v = impl->at(i).toInteger(&isValid);

            if (isValid && v > 0) {
                unsigned long c = static_cast<unsigned long>(v);

                if        (c <= 0x00007FUL) {
                    stringAllocation += 1;
                } else if (c <= 0x0007FFUL) {
                    stringAllocation += 2;
                } else if (c <= 0x00FFFFUL) {
                    stringAllocation += 3;
                } else if (c <= 0x10FFFFUL) {
                    stringAllocation += 4;
                } else {
                    isValid = false;
                }
            } else {
                isValid = false;
            }

            ++i;
        }

        char* result;
        if (isValid) {
            result = new char[stringAllocation];
            std::uint8_t* r = reinterpret_cast<std::uint8_t*>(result);
            for (unsigned i=0 ; i<stringLength ; ++i) {
                Integer v = impl->at(i).toInteger(&isValid);
                assert(isValid && v <= 0x10FFFFL);

                unsigned long c = static_cast<unsigned long>(v);

                if        (c <= 0x00007FUL) {
                    r[0] = static_cast<char>(c);
                    r += 1;
                } else if (c <= 0x0007FFUL) {
                    r[0] = (static_cast<std::uint8_t>(c >> 6) & 0x1F) | 0xC0;
                    r[1] = (static_cast<std::uint8_t>(c     ) & 0x3F) | 0x80;

                    r += 2;
                } else if (c <= 0x00FFFFUL) {
                    r[0] = (static_cast<std::uint8_t>(c >> 12) & 0x0F) | 0xE0;
                    r[1] = (static_cast<std::uint8_t>(c >>  6) & 0x3F) | 0x80;
                    r[2] = (static_cast<std::uint8_t>(c      ) & 0x3F) | 0x80;

                    r += 3;
                } else if (c <= 0x10FFFFUL) {
                    r[0] = (static_cast<std::uint8_t>(c >> 18) & 0x07) | 0xF0;
                    r[1] = (static_cast<std::uint8_t>(c >> 12) & 0x3F) | 0x80;
                    r[2] = (static_cast<std::uint8_t>(c >>  6) & 0x3F) | 0x80;
                    r[3] = (static_cast<std::uint8_t>(c      ) & 0x3F) | 0x80;

                    r += 4;
                }
            }

            *r = '\0';
        } else {
            result = nullptr;
        }

        return result;
    }


    Tuple::operator Boolean() const {
        return !impl->isEmpty();
    }


    Variant Tuple::operator()(Tuple::Index index) const {
        return at(index);
    }


    Tuple Tuple::operator*(const Tuple& other) const {
        unsigned long otherSize = static_cast<unsigned long>(other.impl->size());

        if (otherSize == 0) {
            return *this;
        } else {
            Private* result = new Private(*impl);
            for (unsigned long i=0 ; i<otherSize ; ++i) {
                result->append(other.impl->at(i));
            }

            return Tuple(result);
        }
    }


    Tuple& Tuple::operator*=(const Tuple& other) {
        if (!other.impl->isEmpty()) {
            impl->lock();

            if (impl->isEmpty()) {
                if (impl->removeReference()) {
                    delete impl;
                }

                other.impl->addReference();
                impl = other.impl;

                impl->unlock();
            } else {
                if (impl->referenceCount() == 1) {
                    unsigned long otherSize = static_cast<unsigned long>(other.impl->size());
                    for (unsigned long i=0 ; i<otherSize ; ++i) {
                        impl->append(other.impl->at(i));
                    }

                    impl->unlock();
                } else {
                    Private* oldImpl = impl;
                    impl = new Private(*impl);
                    oldImpl->removeReference();
                    oldImpl->unlock();

                    unsigned long otherSize = static_cast<unsigned long>(other.impl->size());
                    for (unsigned long i=0 ; i<otherSize ; ++i) {
                        impl->append(other.impl->at(i));
                    }
                }
            }
        }

        return *this;
    }


    Tuple Tuple::operator/(const Tuple& other) const {
        Private* result = nullptr;

        unsigned long thisNumberTerms  = static_cast<unsigned long>(impl->size());
        unsigned long otherNumberTerms = static_cast<unsigned long>(other.impl->size());

        if (otherNumberTerms > thisNumberTerms || otherNumberTerms == 0 || thisNumberTerms == 0) {
            impl->addReference();
            result = impl;
        } else {
            unsigned long matchLocation      = static_cast<unsigned long>(-1);
            unsigned long lastSearchLocation = thisNumberTerms - otherNumberTerms;
            unsigned long searchIndex        = 0;
            unsigned long compareIndex       = 0;

            do {
                if (impl->at(searchIndex + compareIndex) == other.impl->at(compareIndex)) {
                    ++compareIndex;
                    if (compareIndex >= otherNumberTerms) {
                        matchLocation = searchIndex;
                    }
                } else {
                    compareIndex = 0;
                    ++searchIndex;
                }
            } while (searchIndex <= lastSearchLocation && matchLocation == static_cast<unsigned long>(-1));

            if (matchLocation != static_cast<unsigned long>(-1)) {
                result = new Private;
                unsigned long i=0;
                while (i < matchLocation) {
                    result->append(impl->at(i));
                    ++i;
                }

                i += otherNumberTerms;

                while (i< thisNumberTerms) {
                    result->append(impl->at(i));
                    ++i;
                }
            } else {
                impl->addReference();
                result = impl;
            }
        }

        return Tuple(result);
    }


    Tuple& Tuple::operator/=(const Tuple& other) {
        Tuple result = *this / other;
        return operator=(result);
    }


    Tuple& Tuple::operator=(const Tuple& other) {
        Private* oldImplementation = impl;

        other.impl->addReference();
        impl = other.impl;

        if (oldImplementation->removeReference()) {
            delete oldImplementation;
        }

        return *this;
    }


    Tuple& Tuple::operator=(Tuple&& other) {
        impl = other.impl;
        other.impl = nullptr;

        return *this;
    }


    Tuple& Tuple::operator=(const Variant& other) {
        bool  ok;
        Tuple v = other.toTuple(&ok);

        if (ok) {
            operator=(v);
        } else {
            throw InvalidRuntimeConversion(other.valueType(), ValueType::TUPLE);
        }

        return *this;
    }


    bool Tuple::operator==(const Tuple& other) const {
        return *impl == *other.impl;
    }


    bool Tuple::operator!=(const Tuple& other) const {
        return *impl != *other.impl;
    }
}
