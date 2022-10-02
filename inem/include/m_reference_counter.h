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
* This header defines the \ref M::ReferenceCounter class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_REFERENCE_COUNTER_H
#define M_REFERENCE_COUNTER_H

#include "m_common.h"

namespace M {
    /**
     * Base class you can used for reference counted types.  The reference count will be initialized to 1.
     */
    class M_PUBLIC_API ReferenceCounter {
        public:
            ReferenceCounter();

            ~ReferenceCounter();

            /**
             * Method you can call to lock the reference to make operations atomic.
             */
            inline void lock() {}

            /**
             * Method you can call to unlock the reference after performing atomic operations.
             */
            inline void unlock() {}

            /**
             * Method you can call to increase the reference count on this class.  Note that this method may be called
             * while the inheriting object is locked. The locked state will not be modified by this method.
             */
            void addReference() const;

            /**
             * Method you can call to determine the current reference count.  Note that this method may be called
             * while the inheriting object is locked.  The locked state will not be modified by this method.
             *
             * \return Returns the current reference count.
             */
            unsigned referenceCount() const;

            /**
             * Method you can call to decrease the reference count.
             *
             * \return Returns true if the reference count has reached zero. Returns false if the reference count has
             *         not reached zero.
             */
            bool removeReference() const;

        private:
            /**
             * The internal reference count.
             */
            mutable unsigned currentReferenceCount;
    };
};

#endif
