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
* This header defines the \ref M::SetPrivate class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_SET_PRIVATE_H
#define M_SET_PRIVATE_H

#include "model_intrinsic_types.h"
#include "m_const_forward_iterator.h"
#include "m_range.h"
#include "m_reference_counter.h"
#include "m_unordered_set.h"
#include "m_ordered_set.h"
#include "m_set_api.h"

namespace M {
    /**
     * Default private implementation of the set template class.  By default, sets are unordered.
     */
    template<typename T> class SetPrivate:public ReferenceCounter,public UnorderedSet<T> {
        public:
            SetPrivate() {}

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            SetPrivate(const SetPrivate<T>& other):OrderedSet<T>(other) {}

            /**
             * Copy constructor (move semantics)
             *
             * \param[in] other The instance to be moved to this instance.
             */
            SetPrivate(SetPrivate&& other):OrderedSet<T>(other) {}

            /**
             * Copy constructor (move semantics)
             *
             * \param[in] other The instance to be moved to this instance.
             */
            SetPrivate(OrderedSet<T>&& other):OrderedSet<T>(other) {}

            virtual ~SetPrivate() {}
    };

    /**
     * Private implementation of the set template class for signed integers.
     */
    template<> class SetPrivate<signed int>:public ReferenceCounter,public OrderedSet<signed int> {
        public:
            SetPrivate() {}

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            SetPrivate(const SetPrivate<signed int>& other):OrderedSet<signed int>(other) {}

            /**
             * Copy constructor (move semantics)
             *
             * \param[in] other The instance to be moved to this instance.
             */
            SetPrivate(SetPrivate&& other):OrderedSet<signed int>(other) {}

            /**
             * Copy constructor (move semantics)
             *
             * \param[in] other The instance to be moved to this instance.
             */
            SetPrivate(OrderedSet<signed int>&& other):OrderedSet<signed int>(other) {}

            virtual ~SetPrivate() {}
    };

    /**
     * Private implementation of the set template class for unsigned integers.
     */
    template<> class SetPrivate<unsigned int>:public ReferenceCounter,public OrderedSet<unsigned int> {
        public:
            SetPrivate() {}

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            SetPrivate(const SetPrivate<unsigned int>& other):OrderedSet<unsigned int>(other) {}

            /**
             * Copy constructor (move semantics)
             *
             * \param[in] other The instance to be moved to this instance.
             */
            SetPrivate(SetPrivate&& other):OrderedSet<unsigned int>(other) {}

            /**
             * Copy constructor (move semantics)
             *
             * \param[in] other The instance to be moved to this instance.
             */
            SetPrivate(OrderedSet<unsigned int>&& other):OrderedSet<unsigned int>(other) {}

            virtual ~SetPrivate() {}
    };

    /**
     * Private implementation of the set template class for signed long integers.
     */
    template<> class SetPrivate<signed long>:public ReferenceCounter,public OrderedSet<signed long> {
        public:
            SetPrivate() {}

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            SetPrivate(const SetPrivate<signed long>& other):OrderedSet<signed long>(other) {}

            /**
             * Copy constructor (move semantics)
             *
             * \param[in] other The instance to be moved to this instance.
             */
            SetPrivate(SetPrivate&& other):OrderedSet<signed long>(other) {}

            /**
             * Copy constructor (move semantics)
             *
             * \param[in] other The instance to be moved to this instance.
             */
            SetPrivate(OrderedSet<signed long>&& other):OrderedSet<signed long>(other) {}

            virtual ~SetPrivate() {}
    };

    /**
     * Private implementation of the set template class for unsigned long integers.
     */
    template<> class SetPrivate<unsigned long>:public ReferenceCounter,public OrderedSet<unsigned long> {
        public:
            SetPrivate() {}

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            SetPrivate(const SetPrivate<unsigned long>& other):OrderedSet<unsigned long>(other) {}

            /**
             * Copy constructor (move semantics)
             *
             * \param[in] other The instance to be moved to this instance.
             */
            SetPrivate(SetPrivate&& other):OrderedSet<unsigned long>(other) {}

            /**
             * Copy constructor (move semantics)
             *
             * \param[in] other The instance to be moved to this instance.
             */
            SetPrivate(OrderedSet<unsigned long>&& other):OrderedSet<unsigned long>(other) {}

            virtual ~SetPrivate() {}
    };

    /**
     * Private implementation of the set template class for signed long long integers.
     */
    template<> class SetPrivate<signed long long>:public ReferenceCounter,public OrderedSet<signed long long> {
        public:
            SetPrivate() {}

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            SetPrivate(const SetPrivate<signed long long>& other):OrderedSet<signed long long>(other) {}

            /**
             * Copy constructor (move semantics)
             *
             * \param[in] other The instance to be moved to this instance.
             */
            SetPrivate(SetPrivate&& other):OrderedSet<signed long long>(other) {}

            /**
             * Copy constructor (move semantics)
             *
             * \param[in] other The instance to be moved to this instance.
             */
            SetPrivate(OrderedSet<signed long long>&& other):OrderedSet<signed long long>(other) {}

            virtual ~SetPrivate() {}
    };

    /**
     * Private implementation of the set template class for unsigned long long integers.
     */
    template<> class SetPrivate<unsigned long long>:public ReferenceCounter,public OrderedSet<unsigned long long> {
        public:
            SetPrivate() {}

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            SetPrivate(const SetPrivate<unsigned long long>& other):OrderedSet<unsigned long long>(other) {}

            /**
             * Copy constructor (move semantics)
             *
             * \param[in] other The instance to be moved to this instance.
             */
            SetPrivate(SetPrivate&& other):OrderedSet<unsigned long long>(other) {}

            /**
             * Copy constructor (move semantics)
             *
             * \param[in] other The instance to be moved to this instance.
             */
            SetPrivate(OrderedSet<unsigned long long>&& other):OrderedSet<unsigned long long>(other) {}

            virtual ~SetPrivate() {}
    };

    /**
     * Private implementation of the set template class for single precision floating point.
     */
    template<> class SetPrivate<float>:public ReferenceCounter,public OrderedSet<float> {
        public:
            SetPrivate() {}

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            SetPrivate(const SetPrivate<float>& other):OrderedSet<float>(other) {}

            /**
             * Copy constructor (move semantics)
             *
             * \param[in] other The instance to be moved to this instance.
             */
            SetPrivate(SetPrivate&& other):OrderedSet<float>(other) {}

            /**
             * Copy constructor (move semantics)
             *
             * \param[in] other The instance to be moved to this instance.
             */
            SetPrivate(OrderedSet<float>&& other):OrderedSet<float>(other) {}

            virtual ~SetPrivate() {}
    };

    /**
     * Private implementation of the set template class for double precision floating point.
     */
    template<> class SetPrivate<double>:public ReferenceCounter,public OrderedSet<double> {
        public:
            SetPrivate() {}

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            SetPrivate(const SetPrivate<double>& other):OrderedSet<double>(other) {}

            /**
             * Copy constructor (move semantics)
             *
             * \param[in] other The instance to be moved to this instance.
             */
            SetPrivate(SetPrivate&& other):OrderedSet<double>(other) {}

            /**
             * Copy constructor (move semantics)
             *
             * \param[in] other The instance to be moved to this instance.
             */
            SetPrivate(OrderedSet<double>&& other):OrderedSet<double>(other) {}

            virtual ~SetPrivate() {}
    };

    /**
     * Private implementation of the set template class for quad precision floating point.
     */
    template<> class SetPrivate<long double>:public ReferenceCounter,public OrderedSet<long double> {
        public:
            SetPrivate() {}

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            SetPrivate(const SetPrivate<long double>& other):OrderedSet<long double>(other) {}

            /**
             * Copy constructor (move semantics)
             *
             * \param[in] other The instance to be moved to this instance.
             */
            SetPrivate(SetPrivate&& other):OrderedSet<long double>(other) {}

            /**
             * Copy constructor (move semantics)
             *
             * \param[in] other The instance to be moved to this instance.
             */
            SetPrivate(OrderedSet<long double>&& other):OrderedSet<long double>(other) {}

            virtual ~SetPrivate() {}
    };
};

#endif
