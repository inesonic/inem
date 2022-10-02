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
* This header defines the \ref M::UnorderedSet class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_UNORDERED_SET_H
#define M_UNORDERED_SET_H

#include "m_intrinsics.h"
#include "m_list.h"
#include "m_unordered_set_base.h"
#include "m_hash_functions.h"
#include "m_unordered_set_entry.h"
#include "m_unordered_set_iterator.h"

namespace M {
    /**
     * An unordered set template class.  This class provides support for basic set operations on a pool of elements.
     *
     * The class manages the set a a large circular buffer of prime length.  The hash is used to identify the starting
     * location in the buffer which is then traversed is memory order to either locate the hash entry or to locate an
     * empty location to receive the hash entry.  Each entry keeps a maximum distance that is used to determine the
     * maximum distance that must be searched for entries tied to a given hash value.  The approach provides several
     * advantages of a bucket based hash:
     *
     *     * Searchs are memory sequential which works well with cache lines designed to storage sequential regions
     *       of memory.
     *
     *     * The hash table size increases naturally with the number of entries allowing relatively consistent
     *       performance independent of hash table size.  The approach also means that only a single table is needed
     *       to both store entries and store the base addresses for each hash bucket.
     *
     * \param[in] T The type to be stored in the set.  T must be constructable, copy constructable, copy assignable,
     *              comparable against equality, and destructable.  The type must also have a supporting hash template
     *              function that calculates a hash on an instance of T and a seed value.
     *
     * \param[in] E The hash table entry instance.
     */
    template<typename T, typename E = UnorderedSetEntry<T>> class UnorderedSet:public UnorderedSetBase {
        friend class UnorderedSetIterator<T, E>;

        public:
            /**
             * The set constant iterator class.
             */
            typedef UnorderedSetIterator<T, E> Iterator;

            /**
             * Type representing the type managed by this list.
             */
            typedef T ValueType;

            /**
             * Type provided for STL compatibility.
             */
            typedef Iterator iterator;

            /**
             * Type provided for STL compatibility.
             */
            typedef ValueType value_type;

            /**
             * Value representing the default reservation for the hash table.  The actual hash table will be larger
             * than this value.
             */
            static constexpr UnorderedSetBase::Index defaultReservation = 16;

            /**
             * Factor used to calculate the hash table size based on a reservation.  The underlying hash table will be
             * set to the next prime value greater than this factor times the reservation.
             */
            static constexpr float reservationMargin = 1.5F;

            /**
             * The default hash seed.
             */
            static constexpr UI32 defaultSeed = 0;

            /**
             * Constructor
             *
             * \param[in] newSeed The new seed to apply to the hash function.
             */
            UnorderedSet(
                    UI32 newSeed = defaultSeed
                ):hashTable(
                    calculateNextPrime(static_cast<UnorderedSetBase::Index>(reservationMargin * defaultReservation))
                ) {
                currentNumberEntries = 0;
                currentNextThreshold = defaultReservation;
                currentHashSeed      = newSeed;
            }

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            UnorderedSet(const UnorderedSet<T, E>& other):hashTable(other.hashTable) {
                currentNumberEntries = other.currentNumberEntries;
                currentNextThreshold = other.currentNextThreshold;
                currentHashSeed      = other.currentHashSeed;
            }

            /**
             * Copy constructor (move semantics)
             *
             * \param[in] other The instance to be copied.
             */
            UnorderedSet(const UnorderedSet<T, E>&& other):hashTable(other.hashTable) {
                currentNumberEntries = other.currentNumberEntries;
                currentNextThreshold = other.currentNextThreshold;
                currentHashSeed      = other.currentHashSeed;
            }

            ~UnorderedSet() {}

            /**
             * Method you can use to determine if the list is empty.
             *
             * \return Returns true if the list is empty.  Returns false if the list is not empty.
             */
            bool isEmpty() const {
                return currentNumberEntries == 0;
            }

            /**
             * Method you can use to determine the current size of the set, in elements.
             *
             * \return Returns the current size, in elements.
             */
            UnorderedSetBase::Index size() const {
                return currentNumberEntries;
            }

            /**
             * Method that clears the list, destroying all elements.
             */
            void clear() {
                hashTable.clear();
                hashTable.resize(
                    calculateNextPrime(static_cast<UnorderedSetBase::Index>(reservationMargin * defaultReservation))
                );

                currentNumberEntries = 0;
                currentNextThreshold = defaultReservation;
            }

            /**
             * Method you can use to pre-allocate a specified amount of space for the list.
             *
             * \param[in] newReservedSpace The new space to reserve, in elements.  Note that sets can not be shrunk
             *                             using this method.
             *
             * \return Returns true on success, returns false if the reserved space is insufficient for the number of
             *         entries.
             */
            bool reserve(UnorderedSetBase::Index newReservedSpace) {
                bool result;

                if (currentNumberEntries <= newReservedSpace) {
                    ReservationData* reservationData = aboutToReserve(hashTable);

                    UnorderedSetBase::Index newBaseSize = static_cast<UnorderedSetBase::Index>(
                        reservationMargin * newReservedSpace
                    );
                    UnorderedSetBase::Index newTableSize = calculateNextPrime(newBaseSize);
                    List<E> newHashTable(newTableSize);

                    UnorderedSetBase::Index tableSize = static_cast<UnorderedSetBase::Index>(hashTable.size());
                    for (UnorderedSetBase::Index i=0 ; i<tableSize ; ++i) {
                        if (hashTable.at(i).isOccupied()) {
                            const T& value = hashTable.at(i).value();
                            insert(newHashTable, newTableSize, value, currentHashSeed);
                        }
                    }

                    hashTable.swap(newHashTable);
                    currentNextThreshold = newReservedSpace;

                    completedReservation(reservationData, hashTable);

                    result = true;
                } else {
                    result = false;
                }

                return result;
            }

            /**
             * Method you can use to insert an element into the set.
             *
             * \param[in] value The value to be inserted.
             *
             * \return Returns true if the value has been inserted into the set.  Returns false if the value already
             *         exists in the set and was not inserted.
             */
            bool insert(const T& value) {
                if (currentNumberEntries >= currentNextThreshold) {
                    reserve(static_cast<UnorderedSetBase::Index>(2.0 * currentNextThreshold));
                }

                UnorderedSetBase::Index tableSize   = static_cast<UnorderedSetBase::Index>(hashTable.size());
                bool  wasInserted = insertIfUnique(hashTable, tableSize, value, currentHashSeed);

                if (wasInserted) {
                    ++currentNumberEntries;
                }

                return wasInserted;
            }

            /**
             * Method you can use to remove an element from the set.
             *
             * \param[in] value The value to be removed.
             *
             * \return Returns true if the value was removed.  Returns false if the value did not exist in the set.
             */
            bool remove(const T& value) {
                bool  success   = false;
                UnorderedSetBase::Index tableSize = static_cast<UnorderedSetBase::Index>(hashTable.size());
                UnorderedSetBase::Index
                    h = static_cast<UnorderedSetBase::Index>(hashFunction(value, currentHashSeed)) % tableSize;

                E&                      baseEntry       = hashTable[h];
                UnorderedSetBase::Index maximumDistance = baseEntry.distance();
                UnorderedSetBase::Index newDistance     = 0;
                bool                    wasLast         = false;

                UnorderedSetBase::Index index = h;
                for (  UnorderedSetBase::Index currentDistance=0
                     ; currentDistance<=maximumDistance
                     ; ++currentDistance
                    ) {
                    E& entry = hashTable[index];
                    if (entry.isOccupied()) {
                        const T& entryValue = entry.value();
                        if (entryValue == value) {
                            aboutToRemoveEntry(hashTable, index, value);

                            wasLast = (currentDistance == maximumDistance);
                            entry.releaseValue();
                            success = true;
                        } else {
                            UnorderedSetBase::Index eh = static_cast<UnorderedSetBase::Index>(
                                hashFunction(entryValue, currentHashSeed)
                            ) % tableSize;

                            if (eh == h) {
                                newDistance = currentDistance;
                            }
                        }
                    }

                    ++index;
                    if (index >= tableSize) {
                        index = 0;
                    }
                }

                if (wasLast && newDistance != maximumDistance) {
                    baseEntry.setDistance(newDistance);
                }

                if (success) {
                    --currentNumberEntries;
                }

                return success;
            }

            /**
             * Method you can use to determine if a value is contained in the set.
             *
             * \param[in] value The value to check for.
             *
             * \return Returns true if the value exists in the set.  Returns false if the value does not exist in the
             *         set.
             */
            bool contains(const T& value) const {
                UnorderedSetBase::Index h = locateInHash(value);
                return h != invalidIndex;
            }

            /**
             * Method you can use to set an iterator point into this set.
             *
             * \param[in] value The value to locate.
             *
             * \return Returns an iterate to the desired entry.  An iterate to the end will be returned if the
             *         requested value does not exist.
             */
            UnorderedSetIterator<T, E> find(const T& value) const {
                UnorderedSetBase::Index h = locateInHash(value);
                return h != invalidIndex ? UnorderedSetIterator<T, E>(this, h) : end();
            }

            /**
             * Method you can use to obtain a list holding all the values in the set.
             *
             * \return Returns a list holding every value in the set.
             */
            List<T> toList() const {
                List<T> result;

                UnorderedSetBase::Index otherTableSize = static_cast<UnorderedSetBase::Index>(hashTable.size());
                for (UnorderedSetBase::Index i=0 ; i<otherTableSize ; ++i) {
                    const E& entry = hashTable.at(i);
                    if (entry.isOccupied()) {
                        const T& value = entry.value();
                        result.append(value);
                    }
                }

                return result;
            }

            /**
             * Method that unites this set with another set.
             *
             * \param[in] other The set to unite with this set.
             */
            void uniteWith(const UnorderedSet<T, E>& other) {
                List<T> otherEntries = other.toList();
                for (const T& value : otherEntries) {
                    insert(value);
                }
            }

            /**
             * Method that unites this set with another set, returning the union.
             *
             * \param[in] other The set to unite with this set.
             *
             * \return Returns the union of the two sets.
             */
            UnorderedSet<T, E> unitedWith(const UnorderedSet<T, E>& other) const {
                if (size() > other.size()) {
                    UnorderedSet<T, E> result = *this;
                    result.uniteWith(other);

                    return result;
                } else {
                    UnorderedSet<T, E> result = other;
                    result.uniteWith(*this);

                    return result;
                }
            }

            /**
             * Method that intersects this set with another set.
             *
             * \param[in] other The set to intersect with this set.
             */
            void intersectWith(const UnorderedSet<T, E>& other) {
                UnorderedSetBase::Index tableSize = static_cast<UnorderedSetBase::Index>(hashTable.size());
                for (UnorderedSetBase::Index i=0 ; i<tableSize ; ++i) {
                    const E& entry = hashTable.at(i);
                    if (entry.isOccupied()) {
                        const T& value = entry.value();
                        if (!other.contains(value)) {
                            remove(value);
                        }
                    }
                }
            }

            /**
             * Method that calculates the intersection of this set with another set.
             *
             * \param[in] other The set to unite with this set.
             *
             * \return Returns the union of the two sets.
             */
            UnorderedSet<T, E> intersectedWith(const UnorderedSet<T, E>& other) const {
                UnorderedSet<T, E> result;

                if (hashTable.size() < other.hashTable.size()) {
                    UnorderedSetBase::Index tableSize = static_cast<UnorderedSetBase::Index>(hashTable.size());
                    for (UnorderedSetBase::Index i=0 ; i<tableSize ; ++i) {
                        const E& entry = hashTable.at(i);
                        if (entry.isOccupied()) {
                            const T& value = entry.value();
                            if (other.contains(value)) {
                                result.insert(value);
                            }
                        }
                    }
                } else {
                    UnorderedSetBase::Index tableSize = static_cast<UnorderedSetBase::Index>(other.hashTable.size());
                    for (UnorderedSetBase::Index i=0 ; i<tableSize ; ++i) {
                        const E& entry = other.hashTable.at(i);
                        if (entry.isOccupied()) {
                            const T& value = entry.value();
                            if (contains(value)) {
                                result.insert(value);
                            }
                        }
                    }
                }

                return result;
            }

            /**
             * Method that subtracts another set from this set.
             *
             * \param[in] other The set to intersect with this set.
             */
            void subtract(const UnorderedSet<T, E>& other) {
                List<T> otherEntries = other.toList();
                for (const T& value : otherEntries) {
                    remove(value);
                }
            }

            /**
             * Method that calculates the difference of this set and other set.  The returned set will contain the
             * elements in this set that are not found in the other set.
             *
             * \param[in] other The set to subtract from this set.
             */
            UnorderedSet<T, E> difference(const UnorderedSet<T, E>& other) const {
                UnorderedSet<T, E> result;

                UnorderedSetBase::Index tableSize = static_cast<UnorderedSetBase::Index>(hashTable.size());
                for (UnorderedSetBase::Index i=0 ; i<tableSize ; ++i) {
                    const E& entry = hashTable.at(i);
                    if (entry.isOccupied()) {
                        const T& value = entry.value();
                        if (!other.contains(value)) {
                            result.insert(value);
                        }
                    }
                }

                return result;
            }

            /**
             * Method you can use to obtain an iterator to the first entry in the set.
             *
             * \return Returns an iterator to the first entry in the set.
             */
            Iterator begin() const {
                UnorderedSetBase::Index tableSize = static_cast<UnorderedSetBase::Index>(hashTable.size());
                UnorderedSetBase::Index index     = 0;
                while (index < tableSize && hashTable.at(index).isUnoccupied()) {
                    ++index;
                }

                return Iterator(this, index);
            }

            /**
             * Method you can use to obtain an iterator just past the last entry in the set.
             *
             * \return Returns an iterator just past the last entry in the set.
             */
            Iterator end() const {
                return Iterator(this, static_cast<UnorderedSetBase::Index>(hashTable.size()));
            }

            /**
             * Method you can use to set the hash seed.  This method exists primarily for testing purposes.  You should
             * set the seed before adding any entries to the hash.
             *
             * \param[in] newSeed The new hash seed value.
             */
            void setSeed(UI32 newSeed) {
                currentHashSeed = newSeed;
            }

            /**
             * Method that returns the current hash seed.   This method exists primarily for testing purposes in order
             * to identify values that will trigger collisions.
             *
             * \return Returns the current hash seed value.
             */
            UI32 seed() const {
                return currentHashSeed;
            }

            /**
             * Method that returns the current table size, in entries.  This method exists primarily for testing
             * purposes.
             *
             * \return Returns the current hash table size, in entries.
             */
            UnorderedSetBase::Index tableSize() const {
                return static_cast<UnorderedSetBase::Index>(hashTable.size());
            }

            /**
             * Subtraction operator
             *
             * \param[in] other The instance to subtract from this instance.
             *
             * \return Returns a set containing the difference of the two sets.
             */
            UnorderedSet<T, E> operator-(const UnorderedSet<T, E>& other) {
                return difference(other);
            }

            /**
             * Comparison operator
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if the instances are equivalent.  Returns false if the instances are not
             *         equivalent.
             */
            bool operator==(const UnorderedSet<T, E>& other) const {
                bool isEqual = true;

                if (currentNumberEntries == other.currentNumberEntries) {
                    UnorderedSetBase::Index tableSize = static_cast<UnorderedSetBase::Index>(hashTable.size());
                    UnorderedSetBase::Index index     = 0;
                    while (isEqual && index<tableSize) {
                        const E& entry = hashTable.at(index);
                        isEqual = entry.isUnoccupied() || other.contains(entry.value());
                        ++index;
                    }
                } else {
                    isEqual = false;
                }

                return isEqual;
            }

            /**
             * Comparison operator
             *
             * \param[in] other The instance to compare against this instance.
             *
             * \return Returns true if the instances are not equivalent.  Returns false if the instances are
             *         equivalent.
             */
            bool operator!=(const UnorderedSet<T, E>& other) const {
                return !operator==(other);
            }

            /**
             * Copy assignment operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            UnorderedSet<T, E>& operator=(const UnorderedSet<T, E>& other) {
                hashTable            = other.hashTable;
                currentNumberEntries = other.currentNumberEntries;
                currentNextThreshold = other.currentNextThreshold;
                currentHashSeed      = other.currentHashSeed;

                return *this;
            }

            /**
             * Move assignment operator.
             *
             * \param[in] other The instance to be assigned to this instance.
             *
             * \return Returns a reference to this instance.
             */
            UnorderedSet<T, E>& operator=(const UnorderedSet<T, E>&& other) {
                hashTable.swap(other.hashTable);

                currentNumberEntries = other.currentNumberEntries;
                currentNextThreshold = other.currentNextThreshold;
                currentHashSeed      = other.currentHashSeed;

                return *this;
            }

        protected:
            /**
             * Class you can inherit to pass values around in derived classes during a reserve operation.
             */
            class ReservationData {};

            /**
             * Method that is called just after a new entry is inserted.  You can use this method to update any
             * side-band data.  This method is *not* called during reservation operations.
             *
             * The default implementation does nothing.
             *
             * \param[in] hashTable The hash table that was just updated.
             *
             * \param[in] index     The index into the hash table where the newly inserted entry resides.
             *
             * \param[in] value     The value tied to this entry.
             */
            virtual void newEntryInserted(List<E>& hashTable, UnorderedSetBase::Index index, const T& value) {
                (void) hashTable;
                (void) index;
                (void) value;
            }

            /**
             * Method that is called just before an entry is removed.  You can use this method to update any
             * side band data.
             *
             * The default implementation does nothing.
             *
             * \param[in] hashTable The hash table that was just updated.
             *
             * \param[in] index     The index into the hash table where the newly inserted entry resides.
             *
             * \param[in] value     The value tied to this entry.
             */
            virtual void aboutToRemoveEntry(List<E>& hashTable, UnorderedSetBase::Index index, const T& value) {
                (void) hashTable;
                (void) index;
                (void) value;
            }

            /**
             * Method that is called just before performing a reservation operation.  You can use this to clear any
             * status information prior to the reservatin process.
             *
             * The default implementation does nothing.
             *
             * \param[in] hashTable The current hash table.
             *
             * \return Returns an optional instance of \ref UnorderedSet::ReservationData.  The default
             *         implementation returns a null pointer.
             */
            virtual ReservationData* aboutToReserve(List<E>& hashTable) {
                (void) hashTable;
                return nullptr;
            }

            /**
             * Method that is called after a reservation.  You can use this method to update any side band data
             * after a newly constructed hash table is found.
             *
             * \param[in] reservationData The reservation data provided by the call to
             *                            \ref M::UnorderedSet<T,E>::aboutToReserve.  The default implementation will
             *                            delete this pointer if it's not null.
             *
             * \param[in] hashTable The newly built hash table.
             */
            virtual void completedReservation(ReservationData* reservationData, List<E>& hashTable) {
                (void) hashTable;
                delete reservationData;
            }

            /**
             * Method you can use to access the underlying hash table.
             *
             * \param[in] index The zero based index into the underlying hash table.
             *
             * \return Returns the requested entry.
             */
            inline List<E>& storage() {
                return hashTable;
            }

            /**
             * Method you can use to access the underlying hash table.
             *
             * \param[in] index The zero based index into the underlying hash table.
             *
             * \return Returns the requested entry.
             */
            inline const List<E>& storage() const {
                return hashTable;
            }

            /**
             * Method that returns the hash table index of the entry containing a specified value.
             *
             * \param[in] value The value to locate.
             *
             * \return Returns the current index into the hash table.  An invalid index is returned if the entry does
             *         not exist in the table.
             */
            UnorderedSetBase::Index locateInHash(const T& value) const {
                UnorderedSetBase::Index tableSize = static_cast<UnorderedSetBase::Index>(hashTable.size());
                UnorderedSetBase::Index
                    h = static_cast<UnorderedSetBase::Index>(hashFunction(value, currentHashSeed)) % tableSize;

                UnorderedSetBase::Index maximumDistance = hashTable[h].distance();
                UnorderedSetBase::Index currentDistance = 0;

                while (currentDistance <= maximumDistance                             &&
                       (hashTable[h].isUnoccupied() || hashTable[h].value() != value)    ) {
                    ++h;
                    if (h >= tableSize) {
                        h = 0;
                    }

                    ++currentDistance;
                }

                return currentDistance <= maximumDistance ? h : invalidIndex;
            }

        private:
            /**
             * Method that inserts an entry into a hash table.
             *
             * \param[in,out] hashTable The hash table to receive the value.
             *
             * \param[in]     tableSize The size to use for the hash table.
             *
             * \param[in]     value     The value to be inserted.
             *
             * \param[in]     hashSeed  A seed to be applied to the hash function.
             */
            void insert(List<E>& hashTable, UnorderedSetBase::Index tableSize, const T& value, UI32 hashSeed) {
                UnorderedSetBase::Index
                    h = static_cast<UnorderedSetBase::Index>(hashFunction(value, hashSeed)) % tableSize;

                E& baseEntry = hashTable[h];
                if (baseEntry.isUnoccupied()) {
                    baseEntry.assignValue(value);
                } else {
                    UnorderedSetBase::Index currentBaseDistance = baseEntry.distance();
                    UnorderedSetBase::Index distance            = 0;

                    do {
                        ++h;
                        if (h >= tableSize) {
                            h = 0;
                        }
                        ++distance;
                    } while (hashTable[h].isOccupied());

                    E& finalEntry = hashTable[h];
                    finalEntry.assignValue(value);

                    if (distance > currentBaseDistance) {
                        baseEntry.setDistance(distance);
                    }
                }
            }

            /**
             * Method that inserts an entry into a hash table only if it is unique.
             *
             * \param[in,out] hashTable The hash table to receive the value.
             *
             * \param[in]     tableSize The size to use for the hash table.
             *
             * \param[in]     value     The value to be inserted.
             *
             * \param[in]     hashSeed  A seed to be applied to the hash function.
             *
             * \return Returns true if the entry is unique.  Returns false if the entry is not unique.
             */
            bool insertIfUnique(List<E>& hashTable, UnorderedSetBase::Index tableSize, const T& value, UI32 hashSeed) {
                bool isUnique;

                UnorderedSetBase::Index
                    h = static_cast<UnorderedSetBase::Index>(hashFunction(value, hashSeed)) % tableSize;

                E& baseEntry = hashTable[h];

                if (baseEntry.isUnoccupied() && baseEntry.distance() == 0) {
                    baseEntry.assignValue(value);
                    newEntryInserted(hashTable, h, value);
                    isUnique = true;
                } else {
                    UnorderedSetBase::Index currentBaseDistance = baseEntry.distance();
                    UnorderedSetBase::Index distance            = 0;

                    while (hashTable[h].isOccupied() && hashTable[h].value() != value) {
                        ++h;
                        if (h >= tableSize) {
                            h = 0;
                        }
                        ++distance;
                    }

                    if (hashTable[h].isOccupied()) {
                        isUnique = false;
                    } else if (distance < currentBaseDistance) {
                        UnorderedSetBase::Index c = h;
                        UnorderedSetBase::Index remainingDistance = currentBaseDistance - distance;

                        isUnique = true;
                        while (isUnique && remainingDistance > 0) {
                            ++c;
                            if (c >= tableSize) {
                                c = 0;
                            }

                            isUnique = hashTable[c].isUnoccupied() || hashTable[c].value() != value;
                            --remainingDistance;
                        }
                    } else {
                        isUnique = true;
                    }

                    if (isUnique) {
                        E& finalEntry = hashTable[h];
                        finalEntry.assignValue(value);

                        if (distance > currentBaseDistance) {
                            baseEntry.setDistance(distance);
                        }

                        newEntryInserted(hashTable, h, value);
                    }
                }

                return isUnique;
            }

            /**
             * Vector holding the hash table.
             */
            List<E> hashTable;

            /**
             * The current number of entries in the hash table.
             */
            UnorderedSetBase::Index currentNumberEntries;

            /**
             * Next threshold where we should expand our hash table.
             */
            UnorderedSetBase::Index currentNextThreshold;

            /**
             * Seed used to randomize our hashing algorithm.  The seed is set by the constructor and remains
             * constant through the life of the hash.  The seed exists to prevent attacks based on an understanding of
             * the hashing algorithm.
             */
            UnorderedSetBase::Index currentHashSeed;
    };
};

#endif
