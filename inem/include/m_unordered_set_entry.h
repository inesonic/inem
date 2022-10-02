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
* This header defines the \ref M::UnorderedSetEntry class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_UNORDERED_SET_ENTRY_H
#define M_UNORDERED_SET_ENTRY_H

#include "m_intrinsics.h"
#include "m_unordered_set_base.h"

namespace M {
    /**
     * Class used internally by the \ref M::UnorderedSet set to track values.  You will rarely need to use this class
     * directly.
     */
    template<typename T> class UnorderedSetEntry {
        public:
            UnorderedSetEntry() {
                currentEntryData = 0;
                new(currentValue) T();
            }

            /**
             * Constructor
             *
             * \param[in] distance The distance to assign to this entry.
             *
             * \param[in] value    The value to assign to this entry.
             */
            UnorderedSetEntry(UnorderedSetBase::Index distance, const T& value) {
                currentEntryData = occupiedMask | distance;
                new(currentValue) T(value);
            }

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            UnorderedSetEntry(const UnorderedSetEntry<T>& other) {
                currentEntryData = other.currentEntryData;
                new(currentValue) T(*reinterpret_cast<const T*>(other.currentValue));
            }

            ~UnorderedSetEntry() {
                if (isOccupied()) {
                    reinterpret_cast<T*>(&currentValue)->~T();
                }
            }

            /**
             * Method you can use to determine if this entry is occupied.
             *
             * \return Returns true if the entry is occupied.  Returns false if the entry is unoccupied.
             */
            bool isOccupied() const {
                return (currentEntryData & occupiedMask) != 0;
            }

            /**
             * Method you can use to determine if this entry is unoccupied.
             *
             * \return Returns true if the entry is not occupied.  Returns false if the entry is occupied.
             */
            bool isUnoccupied() const {
                return (currentEntryData & occupiedMask) == 0;
            }

            /**
             * Method you can use to determine the maximum search distance for this hash table entry.
             *
             * \return Returns the maximum search distance for this hash value.
             */
            UnorderedSetBase::Index distance() const {
                return currentEntryData & distanceMask;
            }

            /**
             * Method you can use to obtain the value assigned to this entry
             *
             * \return Returns a constant reference to the value assigned at this entry.
             */
            const T& value() const {
                return *reinterpret_cast<const T*>(currentValue);
            }

            /**
             * Method you can use to assign a value to this entry.  Calls plaement new to allocate the storage.  The
             * location will be marked as occupied.
             *
             * \param[in] newValue The value to assign to this location.
             */
            void assignValue(const T& newValue) {
                if (isOccupied()) {
                    reinterpret_cast<T*>(&currentValue)->~T();
                }

                (void) new(&currentValue) T(newValue);
                currentEntryData |= occupiedMask;
            }

            /**
             * Method you can use to release the value from this location.  Calls the object destructor on the value.
             */
            void releaseValue() {
                currentEntryData &= ~occupiedMask;
                reinterpret_cast<T*>(&currentValue)->~T();
            }

            /**
             * Method you can use to set the maximum search distance for this entry.
             *
             * \param[in] newDistance The new search distance.
             */
            void setDistance(UnorderedSetBase::Index newDistance) {
                currentEntryData = (currentEntryData & occupiedMask) | newDistance;
            }

            /**
             * Assignment operator
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this instance.
             */
            UnorderedSetEntry<T>& operator=(const UnorderedSetEntry<T>& other) {
                *reinterpret_cast<T*>(currentValue) = *reinterpret_cast<const T*>(other.currentValue);
                currentEntryData = other.currentEntryData;

                return *this;
            }

        private:
            /**
             * Mask used to identifier the occupied flag in the entry data.
             */
            static constexpr UnorderedSetBase::Index occupiedMask = 1UL << (8 * sizeof(UnorderedSetBase::Index) - 1);

            /**
             * Mask used to identify the distance in the entry data.
             */
            static constexpr UnorderedSetBase::Index distanceMask = occupiedMask - 1;

            /**
             * Type used to represent storage within this entry.
             */
            typedef UI8 AllocationType[sizeof(T)];

            /**
             * The current value at this location.
             */
            AllocationType currentValue;

            /**
             * The current distance.  The MSB will be used as a flag indicating if the location is occupied.
             */
            UnorderedSetBase::Index currentEntryData;
    };
}

#endif
