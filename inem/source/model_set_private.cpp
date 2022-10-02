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
* This file implements the \ref Model::Set::Private class.
***********************************************************************************************************************/

#include <cmath>
#include <limits>

#include "model_intrinsic_types.h"
#include "m_variant.h"
#include "m_list.h"
#include "m_tuple.h"
#include "m_unordered_set.h"
#include "m_ordered_set.h"
#include "m_variant_ordered_set.h"
#include "m_reference_counter.h"
#include "m_set_iterator.h"
#include "model_set.h"
#include "model_set_private.h"

namespace Model {
    Set::Private::Private() {}


    Set::Private::Private(const Private& other):M::VariantOrderedSet(other) {}


    Set::Private::Private(const M::OrderedSet<M::Variant>& other):M::VariantOrderedSet(other) {}


    Set::Private::Private(Private&& other):M::VariantOrderedSet(other) {}


    Set::Private::Private(M::OrderedSet<M::Variant>&& other):M::VariantOrderedSet(other) {}


    Set::Private::~Private() {}


    Set::Private* Set::Private::cartesianProduct(const Private& other) const {
        Private* result = new Private();

        for (const M::Variant& a : *this) {
            for (const M::Variant& b : other) {
                M::Tuple tuple;
                tuple.append(a);
                tuple.append(b);

                result->insert(tuple);
            }
        }

        return result;
    }


    Set::Private::IteratorStateData* Set::Private::captureIteratorState() {
        IteratorStateData* result = new IteratorStateData;
        result->iteratorState = M::VariantOrderedSet::captureIteratorState();

        return result;
    }


    void Set::Private::restoreIteratorState(
            Set::Private::IteratorStateData* iteratorStateData,
            Model::Set*                      setInstance
        ) {
        M::List<IteratorState>  updateState;
        M::List<IteratorState>& iteratorState   = iteratorStateData->iteratorState;
        unsigned long           numberIterators = iteratorState.size();
        for (unsigned long index=0 ; index<numberIterators ; ++index) {
            Iterator*       iterator    = iteratorState.at(index).iterator();
            M::SetIterator* setIterator = static_cast<M::SetIterator*>(iterator);
            if (setIterator->setApi() == setInstance) {
                updateState.append(iteratorState.at(index));
            }
        }

        M::VariantOrderedSet::restoreIteratorState(updateState);
        delete iteratorStateData;
    }


    void Set::Private::reassignIterators(Private* old) {
        const UnorderedSet<Iterator*> iterators = old->iterators();
        for (  M::UnorderedSet<Iterator*>::Iterator it = iterators.begin(),end = iterators.end()
             ; it != end
             ; ++it
            ) {
            Iterator* iterator = *it;
            if (iterator->isValid()) {
                const M::Variant& value    = iterator->constReference();
                (*iterator) = find(value);
            } else {
                (*iterator) = Private::end();
            }
        }
    }
}
