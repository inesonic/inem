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
* This file implements the \ref M::Set class.
***********************************************************************************************************************/

#include <cmath>
#include <limits>

#include "model_intrinsic_types.h"
#include "model_exceptions.h"
#include "model_exceptions.h"
#include "model_variant.h"
#include "model_set.h"
#include "model_set_private.h"
#include "m_ordered_set.h"
#include "m_variant.h"
#include "m_variant_ordered_set.h"
#include "m_set_iterator.h"
#include "m_range.h"
#include "m_set.h"

/***********************************************************************************************************************
 * M::Set
 */

namespace M {
    Set::Set() {}


    Set::Set(const Variant& other) {
        bool ok;
        Set  v = other.toSet(&ok);

        if (ok) {
            Model::Set::operator=(v);
        } else {
            throw Model::InvalidRuntimeConversion(other.valueType(), M::ValueType::SET);
        }
    }


    Set::Set(const Model::Set& other):Model::Set(other) {}


    Set::Set(Model::Set&& other):Model::Set(other) {}


    Set::Set(const Set& other):Model::Set(other) {}


    Set::Set(Set&& other):Model::Set(other) {}


    Set::~Set() {}


    bool Set::insert(const Variant& value) {
        bool result;

        impl->lock();
        if (impl->referenceCount() == 1) {
            result = impl->insert(value);
            impl->unlock();
        } else {
            Private* oldImplementation = impl;
            impl = new Private(*impl);

            oldImplementation->removeReference();
            oldImplementation->unlock();

            result = impl->insert(value);
        }

        return result;
    }


    bool Set::insert(const Range& range) {
        bool allValuesUnique = true;

        impl->lock();
        if (impl->referenceCount() == 1) {
            for (Range::ConstIterator it=range.constBegin(),end=range.constEnd() ; it != end ; ++it) {
                allValuesUnique = impl->insert(it.constReference()) && allValuesUnique;
            }

            impl->unlock();
        } else {
            Private* oldImplementation = impl;
            impl = new Private(*impl);

            impl->restoreIteratorState(oldImplementation->captureIteratorState(), this);

            oldImplementation->removeReference();
            oldImplementation->unlock();

            for (Range::ConstIterator it=range.constBegin(),end=range.constEnd() ; it != end ; ++it) {
                allValuesUnique = impl->insert(it.constReference()) && allValuesUnique;
            }
        }

        return allValuesUnique;
    }


    bool Set::remove(const Variant& value) {
        bool result;

        impl->lock();
        if (impl->referenceCount() == 1) {
            result = impl->remove(value);
            impl->unlock();
        } else {
            Private* oldImplementation = impl;
            impl = new Private(*impl);

            oldImplementation->removeReference();
            oldImplementation->unlock();

            result = impl->remove(value);
        }

        return result;
    }


    bool Set::contains(const Variant& value) const {
        bool result;

        impl->lock();
        result = impl->contains(value);
        impl->unlock();

        return result;
    }


    Set::ConstIterator Set::find(const Variant& value) const {
        M::VariantOrderedSet::Iterator it = impl->find(value);
        return Set::ConstIterator(it, this);
    }


    Set::Iterator Set::find(const Variant& value) {
        M::VariantOrderedSet::Iterator it = impl->find(value);
        return Set::Iterator(it, this);
    }


    Set::ConstIterator Set::constBegin() const {
        return Iterator(impl->begin(), this);
    }


    Set::ConstIterator Set::constEnd() const {
        return Iterator(impl->end(), this);
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


    int Set::relativeOrder(const Set &other) const {
        int result = 0;

        if (impl != other.impl) {
            if (impl->size() < other.impl->size()) {
                result = -1;
            } else if (impl->size() > other.impl->size()) {
                result = +1;
            } else {
                VariantOrderedSet::Iterator thisIterator  = impl->begin();
                VariantOrderedSet::Iterator otherIterator = other.impl->begin();

                while (thisIterator.isValid() && otherIterator.isValid() && result == 0) {
                    const Variant& thisValue  = thisIterator.constReference();
                    const Variant& otherValue = otherIterator.constReference();

                    if (thisValue < otherValue) {
                        result = -1;
                    } else if (thisValue > otherValue) {
                        result = +1;
                    }

                    ++thisIterator;
                    ++otherIterator;
                }
            }
        }

        return result;
    }


    bool Set::operator==(const Set& other) const {
        return impl == other.impl || *impl == *other.impl;
    }


    Set& Set::operator=(const Set& other) {
        Model::Set::operator=(other);
        return *this;
    }


    Set& Set::operator=(Set&& other) {
        Model::Set::operator=(other);
        return *this;
    }


    Set& Set::operator=(const Variant& other) {
        bool ok;
        Set  v = other.toSet(&ok);

        if (ok) {
            Model::Set::operator=(v);
        } else {
            throw Model::InvalidRuntimeConversion(other.valueType(), M::ValueType::SET);
        }

        return *this;
    }
}
