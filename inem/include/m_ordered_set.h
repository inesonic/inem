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
* This header defines the \ref M::OrderedSet class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_ORDERED_SET_H
#define M_ORDERED_SET_H

#include "m_intrinsics.h"
#include "m_list.h"
#include "m_unordered_set_base.h"
#include "m_unordered_set.h"
#include "m_ordered_set_compare_function.h"
#include "m_ordered_set_entry.h"
#include "m_ordered_set_iterator.h"
namespace M {
    /**
     * An ordered set template class.  This class extends the \ref UnorderedSet set to maintain a red-black tree of the
     * elements in parallel with the hash.  The premise of the approach is that the \ref UnorderedSet class is
     * relatively memory efficient, provides time/space efficient memory management, provides for very fast lookup into
     * the hash, and supports the core set of functions expected from a hash.
     *
     * This class then uses a red-black tree to provide ordering of elements with an iterator.  Look-ups become
     * constant time and elements are traversed in ascending order.
     *
     * \param[in] T The type to be stored in the set.  T must be constructable, copy constructable, copy assignable,
     *              comparable against equality and less-than.  T must also be destructable.  The type must also have a
     *              supporting hash template function that calculates a hash on an instance of T and a seed value.
     *
     * \param[in] E The hash table entry instance.
     *
     * \param[in] C The entry comparison functor.
     */
    template<
        typename T,
        typename E = OrderedSetEntry<T>,
        int C(const T&, const T&) = OrderedSetCompareFunction<T>
    > class OrderedSet:public UnorderedSet<T, E> {
        friend class OrderedSetIterator<T, E, C>;

        public:
            /**
             * The index class.
             */
            typedef UnorderedSetBase::Index Index;

            /**
             * Value indicating an invalid index.  Note that this value must match the value defined in
             * \ref UnorderedSetBase.
             */
            static constexpr Index invalidIndex = static_cast<Index>(-1);

            /**
             * The set constant iterator class.
             */
            typedef OrderedSetIterator<T, E, C> Iterator;

            OrderedSet() {
                currentRootIndex = invalidIndex;
            }

            /**
             * Constructor
             *
             * \param[in] newSeed The new seed to apply to the hash function.
             */
            OrderedSet(UI32 newSeed):UnorderedSet<T, E>(newSeed) {
                currentRootIndex = invalidIndex;
            }

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            OrderedSet(const OrderedSet<T, E, C>& other):UnorderedSet<T, E>(other) {
                currentRootIndex = other.currentRootIndex;
            }

            /**
             * Copy constructor (move semantics)
             *
             * \param[in] other The instance to be copied.
             */
            OrderedSet(const OrderedSet<T, E, C>&& other):UnorderedSet<T, E>(other) {
                currentRootIndex = other.currentRootIndex;
            }

            /**
             * Constructor
             *
             * \param[in] other The instance to be copied.
             */
            OrderedSet(const UnorderedSet<T, E>& other):UnorderedSet<T, E>(other) {
                completedReservation(nullptr, UnorderedSet<T, E>::storage());
            }

            /**
             * Constructor (move semantics)
             *
             * \param[in] other The instance to be copied.
             */
            OrderedSet(UnorderedSet<T, E>&& other):UnorderedSet<T, E>(other) {
                completedReservation(nullptr, UnorderedSet<T, E>::storage());
            }

            ~OrderedSet() {}

            /**
             * Method you can use to set an iterator point into this set.
             *
             * \param[in] value The value to locate.
             *
             * \return Returns an iterate to the desired entry.  An iterate to the end will be returned if the
             *         requested value does not exist.
             */
            OrderedSetIterator<T, E, C> find(const T& value) const {
                UnorderedSetBase::Index h = UnorderedSet<T,E>::locateInHash(value);
                return h != invalidIndex ? OrderedSetIterator<T, E, C>(this, h, false) : end();
            }

            /**
             * Method you can use to obtain an iterator to the first entry in the set.
             *
             * \return Returns an iterator to the first entry in the set.
             */
            Iterator begin() const {
                return Iterator(this, currentRootIndex, true);
            }

            /**
             * Method you can use to obtain an iterator just past the last entry in the set.
             *
             * \return Returns an iterator just past the last entry in the set.
             */
            Iterator end() const {
                return Iterator(this, invalidIndex, false);
            }

            /**
             * Method you can use to obtain a list of every iterator currently tied to this set.
             *
             * \return Returns a list of iterators tied to this set.
             */
            const UnorderedSet<Iterator*> iterators() const {
                return currentIterators;
            }

            /**
             * Method you can use to determine the minimum and maximum R-B tree depths.  This method is provided for
             * debugging purposes.
             *
             * \param[in,out] minimumDepth Value that is populated with the minimum tree depth.
             *
             * \param[in,out] maximumDepth Value that is populated with the maximum tree depth.
             */
            void calculateTreeDepths(unsigned& minimumDepth, unsigned& maximumDepth) {
                calculateTreeDepthsFromNode(currentRootIndex, minimumDepth, maximumDepth);
            }

            /**
             * Copy assignment operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            OrderedSet<T, E>& operator=(const OrderedSet<T, E>& other) {
                UnorderedSet<T, E>::operator=(other);
                currentRootIndex = other.currentRootIndex;

                return *this;
            }

            /**
             * Move assignment operator.
             *
             * \param[in] other The instance to be assigned to this instance.
             *
             * \return Returns a reference to this instance.
             */
            OrderedSet<T, E>& operator=(const OrderedSet<T, E>&& other) {
                UnorderedSet<T>::operator=(other);
                currentRootIndex = other.currentRootIndex;

                return *this;
            }

        protected:
            /**
             * Trivial class that holds a pair of iterator pointer/value pairs.
             */
            class IteratorState {
                public:
                    IteratorState() {
                        currentIterator = nullptr;
                        currentValue    = T();
                    }

                    /**
                     * Constructor
                     *
                     * \param[in] iterator The iterator to track the state of.
                     *
                     * \param[in] value    The current value of the iterator
                     */
                    IteratorState(Iterator* iterator, const T& value):currentIterator(iterator), currentValue(value) {}

                    /**
                     * Copy constructor
                     *
                     * \param[in] other The instance to be copied.
                     */
                    IteratorState(
                            const IteratorState& other
                        ):currentIterator(
                            other.currentIterator
                        ),currentValue(
                            other.currentValue
                        ) {}

                    ~IteratorState() {}

                    /**
                     * Method that provides a pointer to the iterator.
                     *
                     * \return Returns a pointer to the requested iterator.
                     */
                    Iterator* iterator() const {
                        return currentIterator;
                    }

                    /**
                     * Method that provides the value the iterator was pointing to.
                     *
                     * \return Returns the value the iterator was pointing to.
                     */
                    const T& value() const {
                        return currentValue;
                    }

                    /**
                     * Assignment operator.
                     *
                     * \param[in] other The instance to assign to this instance.
                     *
                     * \return Returns a reference to this instance.
                     */
                    IteratorState& operator=(const IteratorState& other) {
                        currentIterator = other.currentIterator;
                        currentValue    = other.currentValue;

                        return *this;
                    }

                private:
                    /**
                     * The current iterator pointer.
                     */
                    Iterator* currentIterator;

                    /**
                     * The value pointed to by the iterator.
                     */
                    T currentValue;
            };

            /**
             * Class that manages side-band data used to track reservations.
             */
            class IteratorStateData:public UnorderedSet<T,E>::ReservationData {
                public:
                    /**
                     * List of iterator states used to update iterators when the set resizes.
                     */
                    List<IteratorState> iteratorState;
            };

            /**
             * Method that is called just after a new entry is inserted.  You can use this method to update any
             * side-band data.
             *
             * \param[in] hashTable The hash table that was just updated.
             *
             * \param[in] index     The index into the hash table where the newly inserted entry resides.
             *
             * \param[in] value     The value tied to this entry.
             */
            void newEntryInserted(List<E>& hashTable, Index index, const T& value) override {
                insertNode(hashTable, index, value);
                rebalanceAfterInsert(hashTable, index);
            }

            /**
             * Method that is called just before an entry is removed.  You can use this method to update any
             * side band data.
             *
             * \param[in] hashTable The hash table that was just updated.
             *
             * \param[in] index     The index into the hash table where the newly inserted entry resides.
             *
             * \param[in] value     The value tied to this entry.
             */
            void aboutToRemoveEntry(List<E>& hashTable, Index index, const T& value) override {
                (void) value;

                E& entry = hashTable[index];
                removeEntryFromTree(hashTable, entry, index);
                clearEntry(entry);
            }

            /**
             * Method that is called just before performing a reservation operation.  You can use this to clear any
             * status information prior to the reservatin process.
             *
             * \param[in] hashTable The current hash table.
             *
             * \return Returns an optional instance of \ref UnorderedSet::ReservationData.
             */
            typename UnorderedSet<T,E>::ReservationData* aboutToReserve(List<E>& hashTable) override {
                (void) hashTable;

                IteratorStateData* iteratorStateData = new IteratorStateData;
                iteratorStateData->iteratorState = captureIteratorState();

                return iteratorStateData;
            }

            /**
             * Method that is called after a reservation.  You can use this method to update any side band data
             * after a newly constructed hash table is found.
             *
             * \param[in] reservationData The reservation data provided by the call to
             *                            \ref UnorderedSet::aboutToReserve.
             *
             * \param[in] hashTable       The newly built hash table.
             */
            void completedReservation(
                    typename UnorderedSet<T,E>::ReservationData* reservationData,
                    List<E>&                                     hashTable
                ) override {
                Index hashTableSize = static_cast<Index>(hashTable.size());
                currentRootIndex = invalidIndex;

                for (unsigned index=0 ; index<hashTableSize ; ++index) {
                    E& entry = hashTable[index];
                    if (entry.isOccupied()) {
                        const T& value = entry.value();
                        newEntryInserted(hashTable, index, value);
                    }
                }

                if (reservationData != nullptr) {
                    IteratorStateData*   iteratorStateData = static_cast<IteratorStateData*>(reservationData);
                    List<IteratorState>& iteratorState     = iteratorStateData->iteratorState;
                    restoreIteratorState(iteratorState);
                    delete reservationData;
                }
            }

            /**
             * Method that builds a list of iterator/state values.
             *
             * \return Returns a list of iterator value pairs.
             */
            List<IteratorState> captureIteratorState() {
                List<IteratorState> result;

                for (  typename UnorderedSet<Iterator*>::Iterator it  = currentIterators.begin(),
                                                                  end = currentIterators.end()
                     ; it != end
                     ; ++it
                    ) {
                    Iterator* iterator = *it;
                    if (iterator->currentIndex != invalidIndex) {
                        result.append(IteratorState(iterator, *(*iterator)));
                    }
                }

                return result;
            }

            /**
             * Method that updates iterators to point to the correct value.
             *
             * \param[in] iteratorStateList Listing holding \ref OrderedSet::IteratorState instances holding
             *                              the iterator state.
             */
            void restoreIteratorState(List<IteratorState>& iteratorStateList) {
                for (  typename List<IteratorState>::Iterator it  = iteratorStateList.begin(),
                                                              end = iteratorStateList.end()
                     ; it != end
                     ; ++it
                    ) {
                    Iterator* iterator = it->iterator();
                    if (iterator->currentSet == this) {
                        const T&  value    = it->value();
                        Index     index    = UnorderedSet<T,E>::locateInHash(value);

                        iterator->currentIndex = index;
                    }
                }
            }

        private:
            /**
             * Method that swaps out one child for another under the current child's parent.
             *
             * \param[in] hashTable         The hash table that was just updated.
             *
             * \param[in] currentChildIndex The index of the current child.
             *
             * \param[in] newChildIndex     The new child to replace the current child.
             */
            void swapChild(List<E>& hashTable, Index currentChildIndex, Index newChildIndex) {
                E&    currentChild = hashTable[currentChildIndex];
                Index parentIndex  = currentChild.parentIndex();
                if (parentIndex != invalidIndex) {
                    E& parent = hashTable[parentIndex];

                    if (parent.leftChildIndex() == currentChildIndex) {
                        parent.setLeftChildIndex(newChildIndex);
                    } else {
                        parent.setRightChildIndex(newChildIndex);
                    }

                    if (newChildIndex != invalidIndex) {
                        E& newChild = hashTable[newChildIndex];
                        newChild.setParentIndex(parentIndex);
                    }
                } else {
                    _assert(newChildIndex != invalidIndex);
                    E& newChild = hashTable[newChildIndex];
                    newChild.setParentIndex(invalidIndex);

                    currentRootIndex = newChildIndex;
                }
            }

            /**
             * Method that rotates a node left.
             *
             * \param[in] hashTable The hash table that was just updated.
             *
             * \param[in] rootIndex The index of current root entry.  We will pivot on the right side child under this
             *                      entry.
             *
             * \return Returns the index of the pivot entry (now where the root was).
             */
            Index rotateLeft(List<E>& hashTable, Index rootIndex) {
                //     g               g       |
                //     |               |       |
                //     r               d       |
                //    / \             / \      |
                //   /   \   ----->  /   \     |
                //  a     d         r     c    |
                //       / \       / \         |
                //      b   c     a   b        |

                E& r = hashTable[rootIndex];

                Index di = r.rightChildIndex();
                E&    d  = hashTable[di];

                Index bi = d.leftChildIndex();

                swapChild(hashTable, rootIndex, di);

                d.setLeftChildIndex(rootIndex);
                r.setParentIndex(di);

                r.setRightChildIndex(bi);
                if (bi != invalidIndex) {
                    E& b = hashTable[bi];
                    b.setParentIndex(rootIndex);
                }

                return di;
            }

            /**
             * Method that rotates a node right.
             *
             * \param[in] hashTable The hash table that was just updated.
             *
             * \param[in] rootIndex The index of current root entry.  We will pivot on the right side child under this
             *                      entry.
             *
             * \return Returns the index of the pivot entry (now where the root was).
             */
            Index rotateRight(List<E>& hashTable, Index rootIndex) {
                //      g               g        |
                //      |               |        |
                //      r               d        |
                //     / \             / \       |
                //    /   \  ------>  /   \      |
                //   d     c         a     r     |
                //  / \                   / \    |
                // a   b                 b   c   |

                E& r = hashTable[rootIndex];

                Index di = r.leftChildIndex();
                E&    d  = hashTable[di];

                Index bi = d.rightChildIndex();

                swapChild(hashTable,rootIndex, di);

                d.setRightChildIndex(rootIndex);
                r.setParentIndex(di);

                r.setLeftChildIndex(bi);
                if (bi != invalidIndex) {
                    E& b = hashTable[bi];
                    b.setParentIndex(rootIndex);
                }

                return di;
            }

            /**
             * Method that traverses the tree until an empty location has been identified.
             *
             * \param[in] hashTable The hash table that was just updated.
             *
             * \param[in] index     The index into the hash table where the newly inserted entry resides.
             *
             * \param[in] value     The value tied to this entry.
             */
            void insertNode(List<E>& hashTable, Index index, const T& value) {
                E& entry = hashTable[index];

                entry.setLeftChildIndex(invalidIndex);
                entry.setRightChildIndex(invalidIndex);
                entry.setRed();

                if (currentRootIndex == invalidIndex) {
                    currentRootIndex = index;
                    entry.setParentIndex(invalidIndex);
                } else {
                    Index parentIndex = currentRootIndex;

                    do {
                        E&       parent      = hashTable[parentIndex];
                        const T& parentValue = parent.value();
                        Index    newIndex;

                        if (C(value, parentValue) < 0) {
                            newIndex = parent.leftChildIndex();
                            if (newIndex == invalidIndex) {
                                parent.setLeftChildIndex(index);
                                entry.setParentIndex(parentIndex);
                            }
                        } else {
                            newIndex = parent.rightChildIndex();
                            if (newIndex == invalidIndex) {
                                parent.setRightChildIndex(index);
                                entry.setParentIndex(parentIndex);
                            }
                        }

                        parentIndex = newIndex;
                    } while (parentIndex != invalidIndex);
                }
            }

            /**
             * Method that rebalances the tree after insertion if both the parent of the inserted node and the uncle of
             * the inserted node are red.
             *
             * \param[in] hashTable        The hash table containing all the inserted entries.
             *
             * \param[in] parent           The parent's entry in the hash table.
             *
             * \param[in] grandparent      The grandparent's entry in the hash table.
             *
             * \param[in] grandparentIndex The index to the grandparent in the hash table.
             *
             * \param[in] uncle            The uncle to be adjusted.
             */
            void rebalanceIfRedParentAndUncle(
                    List<E>& hashTable,
                    E&       parent,
                    E&       grandparent,
                    Index    grandparentIndex,
                    E&       uncle
                ) {
                parent.setBlack();
                uncle.setBlack();
                grandparent.setRed();

                rebalanceAfterInsert(hashTable, grandparentIndex);
            }

            /**
             * Method that rebalances the tree after insertion if the parent of the inserted node is red and the uncle
             * of the inserted node is black.
             *
             * \param[in] hashTable        The hash table containing all the inserted entries.
             *
             * \param[in] entry            The inserted entry in the hash table.
             *
             * \param[in] entryIndex       The index of the entry in the hash table.
             *
             * \param[in] parent           The parent's entry in the hash table.
             *
             * \param[in] parentIndex      The index to the parent in the hash table.
             *
             * \param[in] grandparent      The grandparent's entry in the hash table.
             *
             * \param[in] grandparentIndex The index to the grandparent in the hash table.
             *
             * \param[in] uncleIsLeftChild Holds true if the uncle is the grandparent's left child.  Holds false if the
             *                             uncle is the grandparent's right child.
             */
            void rebalanceIfRedParentBlackUncle(
                    List<E>& hashTable,
                    E&       entry,
                    Index    entryIndex,
                    E&       parent,
                    Index    parentIndex,
                    E&       grandparent,
                    Index    grandparentIndex,
                    bool     uncleIsLeftChild
                ) {
                Index parentLeftChildIndex = parent.leftChildIndex();

                if (uncleIsLeftChild) {
                    if (entryIndex == parentLeftChildIndex) {
                        // RL case: parent is right child, entry is left child
                        //          - Note that this is a mirror of the LR case below.
                        //          - Rotate right so entry is new parent, parent is new right child of entry.
                        //          - We now match the RR case below although relationhips are a little different.

                        rotateRight(hashTable, parentIndex);
                        rotateLeft(hashTable, grandparentIndex);
                        entry.setBlack();
                        grandparent.setRed();
                    } else {
                        // RR case: parent is right child, entry is right child
                        //          - Note that this is a mirror of the LL case below.
                        //          - Rotate left so parent is the new grandparent
                        //          - Set parent to black, old grandparent to red (so entry and old grandparent are
                        //            red).

                        rotateLeft(hashTable, grandparentIndex);
                        parent.setBlack();
                        grandparent.setRed();
                    }
                } else /* if (!uncleIsLeftChild) */ {
                    if (entryIndex == parentLeftChildIndex) {
                        // LL case: parent is left child, entry is left child
                        //          - Rotate right so parent is new grandparent
                        //          - Set parent to black, old grandparent to red (so entry and old grandparent are
                        //            red).

                        rotateRight(hashTable, grandparentIndex);
                        parent.setBlack();
                        grandparent.setRed();
                    } else {
                        // LR case: parent is left child, entry is right child
                        //          - Rotate left so entry is new parent, parent is new left child of entry.
                        //          - We now match the LL case above although relationships are a little different.

                        rotateLeft(hashTable, parentIndex);
                        rotateRight(hashTable, grandparentIndex);
                        entry.setBlack();
                        grandparent.setRed();
                    }
                }
            }

            /**
             * Method that rebalances the tree after an insertion operation.
             *
             * \param[in] hashTable The hash table that was just updated.
             *
             * \param[in] index     The index into the hash table where the newly inserted entry resides.
             */
            void rebalanceAfterInsert(List<E>& hashTable, Index index) {
                E&    entry       = hashTable[index];
                if (index == currentRootIndex) {
                    // If the newly inserted entry is the root, set it to black.
                    entry.setBlack();
                } else {
                    entry.setRed();

                    Index parentIndex = entry.parentIndex();
                    E&    parent      = hashTable[parentIndex];

                    if (parent.isRed()) {
                        // The new entry and the parent are both red.
                        Index grandparentIndex = parent.parentIndex();
                        _assert(grandparentIndex != invalidIndex); // Parent is red so it could not be the root.

                        E&    grandparent               = hashTable[grandparentIndex];
                        Index grandparentLeftChildIndex = grandparent.leftChildIndex();
                        Index uncleIndex;
                        bool  uncleIsLeftChild;
                        if (grandparentLeftChildIndex == parentIndex) {
                            uncleIndex       = grandparent.rightChildIndex();
                            uncleIsLeftChild = false;
                        } else {
                            uncleIndex       = grandparentLeftChildIndex;
                            uncleIsLeftChild = true;
                        }

                        if (uncleIndex != invalidIndex) {
                            E& uncle = hashTable[uncleIndex];
                            if (uncle.isRed()) {
                                rebalanceIfRedParentAndUncle(
                                    hashTable,
                                    parent,
                                    grandparent,
                                    grandparentIndex,
                                    uncle
                                );
                            } else {
                                rebalanceIfRedParentBlackUncle(
                                    hashTable,
                                    entry,
                                    index,
                                    parent,
                                    parentIndex,
                                    grandparent,
                                    grandparentIndex,
                                    uncleIsLeftChild
                                );
                            }
                        } else {
                            rebalanceIfRedParentBlackUncle(
                                hashTable,
                                entry,
                                index,
                                parent,
                                parentIndex,
                                grandparent,
                                grandparentIndex,
                                uncleIsLeftChild
                            );
                        }
                    }
                }
            }

            /**
             * Method that is called to remove an entry.
             *
             * \param[in] hashTable The hash table that was just updated.
             *
             * \param[in] entry     The entry we need to remove.
             *
             * \param[in] index     The index into the hash table where the newly inserted entry resides.
             */
            void removeEntryFromTree(List<E>& hashTable, E& entry, Index index) {
                if (entry.leftChildIndex() != invalidIndex) {
                    if (entry.rightChildIndex() != invalidIndex) {
                        removeEntryWithTwoChildrenFromTree(hashTable, entry, index);
                    } else {
                        removeEntryWithOneChildFromTree(hashTable, entry, index, true);
                    }
                } else {
                    if (entry.rightChildIndex() != invalidIndex) {
                        removeEntryWithOneChildFromTree(hashTable, entry, index, false);
                    } else {
                        removeEntryWithNoChildFromTree(hashTable, entry, index);
                    }
                }
            }


            /**
             * Method that locates the in-order successor of a given node.
             *
             * \param[in] hashTable The hash table to search.
             *
             * \param[in] entry     The entry we want to find the in-order successor for.
             *
             * \return Returns the index of the in-order successor for a specified node.  Returns an invalid index
             *         if there is no in-order successor.
             */
            Index inOrderSuccessor(const List<E>& hashTable, const E& entry) {
                Index result          = invalidIndex;
                Index nextSearchIndex = entry.rightChildIndex();

                while (nextSearchIndex != invalidIndex) {
                    result          = nextSearchIndex;
                    nextSearchIndex = hashTable[result].leftChildIndex();
                }

                return result;
            }

            /**
             * Method that replaces a tree entry with a new tree entry, maintaining the tree's structure.  Note that
             * the value is not copied so the ordering must be guaranteed outside of this call.  This method will not
             * update the red-black flag.
             *
             * \param[in,out] hashTable The hash table that was just updated.
             *
             * \param[in]     oldEntry  The entry we are replacing.
             *
             * \param[in]     oldIndex  The index into the hash table of the old entry.
             *
             * \param[in,out] newEntry  The replacement entry.
             *
             * \param[in]     newIndex  The index into the hash table of the new entry.
             */
            void replaceNode(List<E>& hashTable, E& oldEntry, Index oldIndex, E& newEntry, Index newIndex) {
                Index parentIndex     = oldEntry.parentIndex();
                Index leftChildIndex  = oldEntry.leftChildIndex();
                Index rightChildIndex = oldEntry.rightChildIndex();

                newEntry.setParentIndex(parentIndex);
                if (parentIndex != invalidIndex) {
                    E& parent = hashTable[parentIndex];
                    if (parent.leftChildIndex() == oldIndex) {
                        parent.setLeftChildIndex(newIndex);
                    } else {
                        parent.setRightChildIndex(newIndex);
                    }
                } else {
                    currentRootIndex = newIndex;
                }

                newEntry.setLeftChildIndex(leftChildIndex);
                if (leftChildIndex != invalidIndex) {
                    E& leftChild = hashTable[leftChildIndex];
                    leftChild.setParentIndex(newIndex);
                }

                newEntry.setRightChildIndex(rightChildIndex);
                if (rightChildIndex != invalidIndex) {
                    E& rightChild = hashTable[rightChildIndex];
                    rightChild.setParentIndex(newIndex);
                }
            }

            /**
             * Method that clears node values.  This method exists primarily for debug purposes.
             *
             * \param[in,out] entry The entry to clear.
             */
            void clearEntry(E& entry) {
                entry.setParentIndex(invalidIndex);
                entry.setRed();
                entry.setLeftChildIndex(invalidIndex);
                entry.setRightChildIndex(invalidIndex);
            }

            /**
             * Method that removes an element with two children from the tree, then rebalances the tree.
             *
             * \param[in,out] hashTable The hash table that was just updated.
             *
             * \param[in,out] entry     The entry we are removing.
             *
             * \param[in]     index     The index into the hash table where the newly inserted entry resides.
             */
            void removeEntryWithTwoChildrenFromTree(List<E>& hashTable, E& entry, Index index) {
                // For this scenario, we rely on the fact that copying values in the tree does not modify the tree so
                // long as order is maintained.  We therefore locate the in-order successor and recursively remove
                // that entry.  Note that the in-order successor is guaranteed to have either zero or one child so
                // we will never recurse back to this method.
                //
                // Once the in-order successor is located and removed, we replace this entry with the in-order
                // successor in the tree and then update the color of the replacement entry to match the entry we're
                // removing.
                //
                // Note that we can't just assign the value of in-order successor to the removed node as doing so would
                // corrupt the underlying hash table.

                Index removedChildIndex = inOrderSuccessor(hashTable, entry);
                E&    removedChild      = hashTable[removedChildIndex];

                removeEntryFromTree(hashTable, removedChild, removedChildIndex);

                replaceNode(hashTable, entry, index, removedChild, removedChildIndex);
                removedChild.setRed(entry.isRed());
            }

            /**
             * Method that removes an element with a single child from the tree, then rebalances the tree.
             *
             * \param[in,out] hashTable    The hash table that was just updated.
             *
             * \param[in,out] entry        The entry we are removing.
             *
             * \param[in]     index        The index into the hash table where the newly inserted entry resides.
             *
             * \param[in]     hasLeftChild If true, the populated child is the left child.  If false, the populated
             *                             child is the right child.
             */
            void removeEntryWithOneChildFromTree(
                    List<E>& hashTable,
                    E&       entry,
                    Index    index,
                    bool     hasLeftChild
                ) {
                // In this scenario, we replace this entry with its child, then rebalance the tree.

                Index parentIndex = entry.parentIndex();

                Index childIndex  = hasLeftChild ? entry.leftChildIndex() : entry.rightChildIndex();
                E&    child       = hashTable[childIndex];
                child.setParentIndex(parentIndex);

                bool  removedLeftChild;
                if (parentIndex != invalidIndex) {
                    E&    parent         = hashTable[parentIndex];
                    Index leftChildIndex = parent.leftChildIndex();
                    if (leftChildIndex == index) {
                        parent.setLeftChildIndex(childIndex);
                        removedLeftChild = true;
                    } else {
                        parent.setRightChildIndex(childIndex);
                        removedLeftChild = false;
                    }
                } else {
                    currentRootIndex = childIndex;
                    removedLeftChild = false;
                }

                if (entry.isRed() || child.isRed() || parentIndex == invalidIndex) {
                    // At least one of the entries was red set setting this entry black maintains our black depth.
                    child.setBlack();
                } else {
                    fixDoubleBlack(hashTable, parentIndex, removedLeftChild);
                }
            }

            /**
             * Method that removes an element with no children from the tree, then rebalances the tree.
             *
             * \param[in,out] hashTable   The hash table that was just updated.
             *
             * \param[in,out] entry       The entry we are removing.
             *
             * \param[in]     index       The index into the hash table where the newly inserted entry resides.
             */
            void removeEntryWithNoChildFromTree(List<E>& hashTable, E& entry, Index index) {
                // In this scenario, we remove the leaf then rebalance the tree.

                Index parentIndex = entry.parentIndex();
                bool  removedLeftChild;
                if (parentIndex != invalidIndex) {
                    E& parent = hashTable[parentIndex];
                    if (parent.leftChildIndex() == index) {
                        parent.setLeftChildIndex(invalidIndex);
                        removedLeftChild = true;
                    } else {
                        parent.setRightChildIndex(invalidIndex);
                        removedLeftChild = false;
                    }
                } else {
                    currentRootIndex = invalidIndex;
                    removedLeftChild = false;
                }

                if (entry.isRed() || parentIndex == invalidIndex) {
                    // leaf node is guaranteed black so black depth is maintained when we removed this entyr.
                } else {
                    fixDoubleBlack(hashTable, parentIndex, removedLeftChild);
                }
            }

            /**
             * Method that is called to fix a double black where the sibling is on the right and one or both of the
             * siblings children are red.
             *
             * \param[in,out] hashTable       The hash table containing the updates.
             *
             * \param[in]     parentIndex     The hash table index of the parent.
             *
             * \param[in]     rightChildIndex The hash table index of the right child.
             */
            void fixDoubleBlackRRCase(List<E>& hashTable, Index parentIndex, Index rightChildIndex) {
                // - Mark right sibling black, don't touch color of left sibling.
                // - Rotate left
                //
                //    p                  s     |
                //   / \                / \    |
                //  x   s      ---->   p   r   |
                //     / \            / \      |
                //    l   R          x   l     |

                E& rightSiblingChild = hashTable[rightChildIndex];
                rightSiblingChild.setBlack();

                rotateLeft(hashTable, parentIndex);
            }

            /**
             * Method that is called to fix a double black where the sibling is on the left and one or both of the
             * siblings children are red.
             *
             * \param[in,out] hashTable      The hash table containing the updates.
             *
             * \param[in]     parentIndex    The hash table index of the parent.
             *
             * \param[in]     leftChildIndex The hash table index of the left child.
             */
            void fixDoubleBlackLLCase(List<E>& hashTable, Index parentIndex, Index leftChildIndex) {
                // - Mark left sibling black, don't touch color of right sibling.
                // - Rotate right
                //
                //      p                  s        |
                //     / \                / \       |
                //    s   x      ---->   l   p      |
                //   / \                    / \     |
                //  L   r                  r   x    |

                E& leftSiblingChild = hashTable[leftChildIndex];
                leftSiblingChild.setBlack();

                rotateRight(hashTable, parentIndex);
            }

            /**
             * Method that is called to fix a double black where the sibling is on the right and the left child of the
             * sibling is red.
             *
             * \param[in,out] hashTable      The hash table containing the updates.
             *
             * \param[in]     parentIndex    The hash table index of the parent.
             *
             * \param[in]     siblingIndex   The has table index of the sibling.
             *
             * \param[in]     leftChildIndex The hash table index of the left child.
             */
            void fixDoubleBlackRLCase(
                    List<E>& hashTable,
                    Index    parentIndex,
                    Index    siblingIndex,
                    Index    leftChildIndex
                ) {
                // - Mark the left child black.
                // - Rotate right so that the left child of the sibling becomes the sibling and the sibling becomes the
                //   right child.
                // - Rotate left so that the left child replaces the parent, the parent becomes the left child and the
                //   sibling becomes the right child.
                //
                //      p                  p                   l          |
                //     / \                / \                 / \         |
                //    x   S      ---->   x   l      ---->    /   \        |
                //       / \                / \             p     S       |
                //      /   \              /   \           / \   / \      |
                //     L     r            a     S         x   a b   r     |
                //    / \   / \                / \                 / \    |
                //   a   b c   d              b   r               c   d   |
                //                               / \                      |
                //                              c   d                     |

                E& leftSiblingChild = hashTable[leftChildIndex];
                leftSiblingChild.setBlack();

                rotateRight(hashTable, siblingIndex);
                rotateLeft(hashTable, parentIndex);
            }

            /**
             * Method that is called to fix a double black where the sibling is on the left and the right child of the
             * sibling is red.
             *
             * \param[in,out] hashTable       The hash table containing the updates.
             *
             * \param[in]     parentIndex     The hash table index of the parent.
             *
             * \param[in]     siblingIndex    The has table index of the sibling.
             *
             * \param[in]     rightChildIndex The hash table index of the right child.
             */
            void fixDoubleBlackLRCase(
                    List<E>& hashTable,
                    Index    parentIndex,
                    Index    siblingIndex,
                    Index    rightChildIndex
                ) {
                // - Mark the right child black.
                // - Rotate left so that the right child of the sibling becomes the sibling and the sibling becomes the
                //   left child.
                // - Rotate right so that the right child replaces the parent, the parent becomes the right child and
                //   the sibling becomes the left child.
                //
                //         p                  p                   r        |
                //        / \                / \                 / \       |
                //       S   x      ---->   r   x      ---->    /   \      |
                //      / \                / \                 S     p     |
                //     /   \              /   \               / \   / \    |
                //    l     R            S     d             l   c d   x   |
                //   / \   / \          / \                 / \            |
                //  a   b c   d        l   c               a   b           |
                //                    / \                                  |
                //                   a   b                                 |

                E& rightSiblingChild = hashTable[rightChildIndex];
                rightSiblingChild.setBlack();

                rotateLeft(hashTable, siblingIndex);
                rotateRight(hashTable, parentIndex);
            }

            /**
             * Method that is called to fix a double black condition at the parent node.
             *
             * \param[in,out] hashTable   The hash table containing the updates.
             *
             * \param[in]     parentIndex The hash table index of the parent.
             *
             * \param[in]     parent      The parent node.
             */
            void fixDoubleBlackAtParent(List<E>& hashTable, Index parentIndex, E& parent) {
                Index grandparentIndex = parent.parentIndex();
                if (grandparentIndex != invalidIndex) {
                    E& grandparent = hashTable[grandparentIndex];
                    bool parentIsLeftChild = (grandparent.leftChildIndex() == parentIndex);
                    fixDoubleBlack(hashTable, grandparentIndex, parentIsLeftChild);
                }
            }

            /**
             * Method that is called to fix a double black where the sibling and both children are black.
             *
             * \param[in,out] hashTable   The hash table containing the updates.
             *
             * \param[in]     parentIndex The hash table index of the parent.
             *
             * \param[in]     parent      The parent node.
             *
             * \param[in]     sibling     The sibling node.
             */
            void fixDoubleBlackSiblingAndChildrenBlack(
                    List<E>& hashTable,
                    Index    parentIndex,
                    E&       parent,
                    E&       sibling
                ) {
                // - Mark the sibling red which maintains the black depth from the parent downward
                // - Parent is now double black so fix it's state.

                sibling.setRed();
                fixDoubleBlackAtParent(hashTable, parentIndex, parent);
            }

            /**
             * Method that is called to rebalance the tree when both the removed entry and the replacement entry, which
             * is possibly a leaf node, is black.
             *
             * In this scenario, we have replaced two entries with a single black entry so our black depth through the
             * tree is no longer maintained.
             *
             * \param[in,out] hashTable        The hash table containing the updates.
             *
             * \param[in]     parentIndex      The hash table index of the parent.
             *
             * \param[in]     removedLeftChild If true, we removed the left child.  If false, we removed the right
             *                child.
             */
            void fixDoubleBlack(List<E>& hashTable, Index parentIndex, bool removedLeftChild) {
                E&    parent       = hashTable[parentIndex];
                Index siblingIndex = removedLeftChild ? parent.rightChildIndex() : parent.leftChildIndex();
                if (siblingIndex != invalidIndex) {
                    E&   sibling        = hashTable[siblingIndex];
                    bool siblingIsBlack = sibling.isBlack();

                    if (siblingIsBlack) {
                        Index siblingLeftChildIndex   = sibling.leftChildIndex();
                        bool  siblingLeftChildIsBlack = (
                               siblingLeftChildIndex == invalidIndex
                            || hashTable[siblingLeftChildIndex].isBlack()
                        );

                        Index siblingRightChildIndex   = sibling.rightChildIndex();
                        bool  siblingRightChildIsBlack = (
                               siblingRightChildIndex == invalidIndex
                            || hashTable[siblingRightChildIndex].isBlack()
                        );

                        if (siblingLeftChildIsBlack) {
                            if (siblingRightChildIsBlack) {
                                // Sibling and both sibling's children are black
                                fixDoubleBlackSiblingAndChildrenBlack(hashTable, parentIndex, parent, sibling);
                            } else {
                                if (removedLeftChild) {
                                    // RR case: Sibling is black and at least one child is red:
                                    //   Sibling is right child, and sibling's right child is red.

                                    fixDoubleBlackRRCase(hashTable, parentIndex, siblingRightChildIndex);
                                } else {
                                    // LR case: Sibling is black and at least one child is red:
                                    //   Sibling is left child and sibling's right child is red.

                                    fixDoubleBlackLRCase(hashTable, parentIndex, siblingIndex, siblingRightChildIndex);
                                }
                            }
                        } else /* if (!siblingLeftChildIsBlack) */ {
                            if (siblingRightChildIsBlack) {
                                if (removedLeftChild) {
                                    // RL case: Sibling is black and at least one child is red
                                    //   Sibling is right child, siblings left child is red.

                                    fixDoubleBlackRLCase(hashTable, parentIndex, siblingIndex, siblingLeftChildIndex);
                                } else {
                                    // LL case: Sibling is black and at least one child is red.
                                    //   Sibling is left child, sibling on left child is red.

                                    fixDoubleBlackLLCase(hashTable, parentIndex, siblingLeftChildIndex);
                                }
                            } else {
                                if (removedLeftChild) {
                                    // RR case: Sibling is black, both children are red
                                    //   Sibling is right child

                                    fixDoubleBlackRRCase(hashTable, parentIndex, siblingRightChildIndex);
                                } else {
                                    // LL case: Sibling is black, both children are red
                                    //   Sibling is left child

                                    fixDoubleBlackLLCase(hashTable, parentIndex, siblingLeftChildIndex);
                                }
                            }
                        }
                    } else /* if (!siblingIsBlack) */ {
                        if (removedLeftChild) {
                            // Sibling is red (and both children are assumed black).  Sibling is on the right.
                            //   - Mark sibling black, mark parent red.
                            //   - Rotate left so the sibling is now the parent and the parent is the left child of
                            //     the sibling.
                            //   - This converts the tree to one of the other cases so recurse through this method
                            //     at the old parent node.
                            //
                            //        p               s     |
                            //       / \             / \    |
                            //      x   S   ---->   P   b   |
                            //         / \         / \      |
                            //        a   b       x   a     |

                            rotateLeft(hashTable, parentIndex);
                            sibling.setBlack();
                            parent.setRed();

                            fixDoubleBlack(hashTable, parentIndex, removedLeftChild);
                        } else {
                            // Sibling is red (and both children are assumed black).  Sibling is on the left.
                            //   - Mark sibling black, mark parent red.
                            //   - Rotate right so the sibling is now the parent and the parent is the right child of
                            //     the sibling.
                            //   - This converts the tree to one of the other cases so recurse through this method
                            //     at the old parent node.
                            //
                            //        p               s       |
                            //       / \             / \      |
                            //      S   x   ---->   a   P     |
                            //     / \                 / \    |
                            //    a   b               b   x   |

                            rotateRight(hashTable, parentIndex);
                            sibling.setBlack();
                            parent.setRed();

                            fixDoubleBlack(hashTable, parentIndex, removedLeftChild);
                        }
                    }
                } else {
                    // Sibling is leaf node (no children).
                    // - We can't fix the double black condition here so we push the condition up to the parent and
                    //   fix the condition there.

                    fixDoubleBlackAtParent(hashTable, parentIndex, parent);
                }
            }

            /**
             * Method that calculates the depth of tree from a given node downward.
             *
             * \param[in]     entryIndex   The index of the node to calculate the depth from, including this node.
             *
             * \param[in,out] minimumDepth Value that is populated with the minimum tree depth.
             *
             * \param[in,out] maximumDepth Value that is populated with the maximum tree depth.
             */
            void calculateTreeDepthsFromNode(Index entryIndex, unsigned& minimumDepth, unsigned& maximumDepth) {
                if (entryIndex != invalidIndex) {
                    const E& entry           = M::UnorderedSet<T, E>::storage().at(entryIndex);
                    Index    leftChildIndex  = entry.leftChildIndex();
                    Index    rightChildIndex = entry.rightChildIndex();

                    unsigned leftChildMinimumDepth  = 0;
                    unsigned leftChildMaximumDepth  = 0;
                    unsigned rightChildMinimumDepth = 0;
                    unsigned rightChildMaximumDepth = 0;

                    calculateTreeDepthsFromNode(leftChildIndex,  leftChildMinimumDepth,  leftChildMaximumDepth);
                    calculateTreeDepthsFromNode(rightChildIndex, rightChildMinimumDepth, rightChildMaximumDepth);

                    if (leftChildMinimumDepth < rightChildMinimumDepth) {
                        minimumDepth = leftChildMinimumDepth + 1;
                    } else {
                        minimumDepth = rightChildMinimumDepth + 1;
                    }

                    if (leftChildMaximumDepth > rightChildMaximumDepth) {
                        maximumDepth = leftChildMaximumDepth + 1;
                    } else {
                        maximumDepth = rightChildMaximumDepth + 1;
                    }
                } else {
                    minimumDepth = 0;
                    maximumDepth = 0;
                }
            }

            /**
             * Index to the root node.
             */
            Index currentRootIndex;

            /**
             * List of iterators that are tied to this set.  We use this to inform the iterator that we've rebuilt our
             * hash table.
             */
            mutable UnorderedSet<Iterator*> currentIterators;
    };
};

#endif
