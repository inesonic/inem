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
* This file implements the \ref Model::Set class.
***********************************************************************************************************************/

#include <cmath>
#include <limits>

#include "model_intrinsic_types.h"
#include "model_exceptions.h"
#include "model_variant.h"
#include "model_variant_private.h"
#include "model_range.h"
#include "model_set_iterator_private.h"
#include "model_set_iterator.h"
#include "model_set.h"
#include "model_set_private.h"

namespace Model {
    Set::Set():impl(new Private) {}


    Set::Set(const Variant& other) {
        bool ok;
        Set  v = other.toSet(&ok);

        if (ok) {
            v.impl->addReference();
            impl = v.impl;
        } else {
            throw Model::InvalidRuntimeConversion(other.valueType(), ValueType::SET);
        }
    }


    Set::Set(const Set& other) {
        other.impl->addReference();
        impl = other.impl;
    }


    Set::Set(Set&& other) {
        impl = other.impl;
        other.impl = nullptr;
    }


    Set::~Set() {
        if (impl != nullptr && impl->removeReference()) {
            delete impl;
        }
    }


    bool Set::isEmpty() const {
        return impl->isEmpty();
    }


    Model::Integer Set::size() const {
        return static_cast<Model::Integer>(impl->size());
    }


    void Set::clear() {
        impl->lock();
        if (impl->referenceCount() == 1) {
            impl->clear();
            impl->unlock();
        } else {
            Private* oldImplementation = impl;
            impl = new Private;

            impl->restoreIteratorState(oldImplementation->captureIteratorState(), this);

            oldImplementation->unlock();
            oldImplementation->removeReference();
        }
    }


    bool Set::insert(const Variant& value) {
        bool result;

        impl->lock();
        if (impl->referenceCount() == 1) {
            result = impl->insert(*value.impl);
            impl->unlock();
        } else {
            Private* oldImplementation = impl;
            impl = new Private(*impl);

            impl->restoreIteratorState(oldImplementation->captureIteratorState(), this);

            oldImplementation->removeReference();
            oldImplementation->unlock();

            result = impl->insert(*value.impl);
        }

        return result;
    }


    bool Set::insert(const Range& range) {
        bool allValuesUnique = true;

        impl->lock();
        if (impl->referenceCount() == 1) {
            for (Range::ConstIterator it=range.constBegin(),end=range.constEnd() ; it != end ; ++it) {
                allValuesUnique = impl->insert(*it.constReference().impl) && allValuesUnique;
            }

            impl->unlock();
        } else {
            Private* oldImplementation = impl;
            impl = new Private(*impl);

            impl->restoreIteratorState(oldImplementation->captureIteratorState(), this);

            oldImplementation->removeReference();
            oldImplementation->unlock();

            for (Range::ConstIterator it=range.constBegin(),end=range.constEnd() ; it != end ; ++it) {
                allValuesUnique = impl->insert(*it.constReference().impl) && allValuesUnique;
            }
        }

        return allValuesUnique;
    }


    bool Set::remove(const Variant& value) {
        bool result;

        impl->lock();
        if (impl->referenceCount() == 1) {
            result = impl->remove(*value.impl);
            impl->unlock();
        } else {
            Private* oldImplementation = impl;
            impl = new Private(*impl);

            impl->restoreIteratorState(oldImplementation->captureIteratorState(), this);

            oldImplementation->removeReference();
            oldImplementation->unlock();

            result = impl->remove(*value.impl);
        }

        return result;
    }


//    bool Set::remove(const Range<T>& range) {}


    bool Set::contains(const Variant& value) const {
        bool result;

        impl->lock();
        result = impl->contains(*value.impl);
        impl->unlock();

        return result;
    }


    void Set::uniteWith(const Set& other) {
        impl->lock();
        if (impl->referenceCount() == 1) {
            impl->uniteWith(*other.impl);
            impl->unlock();
        } else {
            Private* oldImplementation = impl;
            impl = new Private(*impl);

            impl->restoreIteratorState(oldImplementation->captureIteratorState(), this);

            oldImplementation->removeReference();
            oldImplementation->unlock();

            impl->uniteWith(*other.impl);
        }
    }


    Set Set::unitedWith(const Set& other) const {
        impl->lock();
        Private* newImplementation = new Private(impl->unitedWith(*other.impl));
        impl->unlock();

        return Set(newImplementation);
    }


    void Set::intersectWith(const Set& other) {
        impl->lock();
        if (impl->referenceCount() == 1) {
            impl->intersectWith(*other.impl);
            impl->unlock();
        } else {
            Private* oldImplementation = impl;
            impl = new Private(*impl);

            impl->restoreIteratorState(oldImplementation->captureIteratorState(), this);

            oldImplementation->removeReference();
            oldImplementation->unlock();

            impl->intersectWith(*other.impl);
        }
    }


    Set Set::intersectedWith(const Set& other) const {
        impl->lock();
        Private* newImplementation = new Private(impl->intersectedWith(*other.impl));
        impl->unlock();

        return Set(newImplementation);
    }


    void Set::subtract(const Set& other) {
        impl->lock();
        if (impl->referenceCount() == 1) {
            impl->subtract(*other.impl);
            impl->unlock();
        } else {
            Private* oldImplementation = impl;
            impl = new Private(*impl);

            impl->restoreIteratorState(oldImplementation->captureIteratorState(), this);

            oldImplementation->removeReference();
            oldImplementation->unlock();

            impl->intersectWith(*other.impl);
        }
    }


    Set Set::difference(const Set& other) const {
        impl->lock();
        Private* newImplementation = new Private(impl->difference(*other.impl));
        impl->unlock();

        return Set(newImplementation);
    }


    Set Set::cartesianProduct(const Set& other) const {
        impl->lock();
        Private* newImplementation = impl->cartesianProduct(*other.impl);
        impl->unlock();

        Set result(newImplementation);
        return result;
    }


    Set::ConstIterator Set::find(const Variant& value) const {
        M::SetIterator it = impl->find(*(value.impl));
        return Set::ConstIterator(new Set::ConstIterator::Private(it, this));
    }


    Set::Iterator Set::find(const Variant& value) {
        M::SetIterator it = impl->find(*(value.impl));
        return Set::Iterator(new Set::Iterator::Private(it, this));
    }


    Set::ConstIterator Set::constBegin() const {
        return Set::ConstIterator(new Set::ConstIterator::Private(impl->begin(), this));
    }


    Set::ConstIterator Set::constEnd() const {
        return Set::ConstIterator(new Set::ConstIterator::Private(impl->end(), this));
    }


    Set::const_iterator Set::cbegin() const {
        return constBegin();
    }


    Set::const_iterator Set::cend() const {
        return constEnd();
    }


    Set::Iterator Set::begin() {
        return constBegin();
    }


    Set::Iterator Set::end() {
        return constEnd();
    }


    Set::operator Boolean() const {
        return !impl->isEmpty();
    }


    Set& Set::operator=(const Set& other) {
        if (this != &other) {
            Private* oldImplementation = impl;

            other.impl->addReference();
            impl = other.impl;
            impl->reassignIterators(oldImplementation);

            if (oldImplementation->removeReference()) {
                delete oldImplementation;
            }
        }

        return *this;
    }


    Set& Set::operator=(Set&& other) {
        if (this != &other) {
            Private* oldImplementation = impl;

            impl = other.impl;
            impl->reassignIterators(oldImplementation);
            other.impl = nullptr;

            if (oldImplementation->removeReference()) {
                delete oldImplementation;
            }
        }

        return *this;
    }


    Set& Set::operator=(const Variant& other) {
        bool ok;
        Set  v = other.toSet(&ok);

        if (ok) {
            operator=(v);
        } else {
            throw InvalidRuntimeConversion(other.valueType(), ValueType::SET);
        }

        return *this;
    }


    bool Set::operator==(const Set& other) const {
        bool result;

        if (impl == other.impl) {
            result = true;
        } else {
            impl->lock();
            other.impl->lock();

            result = (*impl == *other.impl);

            other.impl->unlock();
            impl->unlock();
        }

        return result;
    }


    bool Set::operator!=(const Set& other) const {
        return !operator==(other);
    }
}
