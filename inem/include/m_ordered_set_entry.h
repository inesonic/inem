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
* This header defines the \ref M::OrderedSetEntry class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_ORDERED_SET_ENTRY_H
#define M_ORDERED_SET_ENTRY_H

#include "m_intrinsics.h"
#include "m_unordered_set_base.h"
#include "m_unordered_set.h"

namespace M {
    /**
     * Class used internally by the \ref M::OrderedSet set to track values.  You will rarely need to use this class
     * directly.
     *
     * \param[in] T The type to be stored in the set.  T must be constructable, copy constructable, copy assignable,
     *              comparable against equality and less-than.  T must also be destructable.  The type must also have a
     *              supporting hash template function that calculates a hash on an instance of T and a seed value.
     */
    template<typename T> class OrderedSetEntry:public UnorderedSetEntry<T> {
        public:
            OrderedSetEntry() {
                currentParentAndRedFlag = UnorderedSetBase::invalidIndex;
                currentLeftChild        = UnorderedSetBase::invalidIndex;
                currentRightChild       = UnorderedSetBase::invalidIndex;
            }

            /**
             * Constructor
             *
             * \param[in] distance The distance to assign to this entry.
             *
             * \param[in] value    The value to assign to this entry.
             */
            OrderedSetEntry(UnorderedSetBase::Index distance, const T& value):UnorderedSetEntry<T>(distance, value) {
                currentParentAndRedFlag = UnorderedSetBase::invalidIndex;
                currentLeftChild        = UnorderedSetBase::invalidIndex;
                currentRightChild       = UnorderedSetBase::invalidIndex;
            }

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            OrderedSetEntry(const OrderedSetEntry<T>& other):UnorderedSetEntry<T>(other) {
                currentParentAndRedFlag = other.currentParentAndRedFlag;
                currentLeftChild        = other.currentLeftChild;
                currentRightChild       = other.currentRightChild;
            }

            ~OrderedSetEntry() {}

            /**
             * Method you can use to determine if this node is black.  Note that nodes will be red, by default.
             *
             * \return Returns true if the node is black.  Returns false if the node is red.
             */
            bool isBlack() const {
                return (currentParentAndRedFlag & redFlagMask) == 0;
            }

            /**
             * Method you can use to determine if this node is red.  Note that nodes will be red, by default.
             *
             * \return Returns true if the node is red.  Returns false if the node is black.
             */
            bool isRed() const {
                return (currentParentAndRedFlag & redFlagMask) != 0;
            }

            /**
             * Method you can use to set this node to black or red.
             *
             * \param[in] nowBlack If true, the node will be marked as black.  If false, the node will be
             *                     marked as red.
             */
            void setBlack(bool nowBlack = true) {
                if (nowBlack) {
                    currentParentAndRedFlag &= ~redFlagMask;
                } else {
                    currentParentAndRedFlag |= redFlagMask;
                }
            }

            /**
             * Method you can use to set this node to red or black.
             *
             * \param[in] nowRed If true, the node will be marked as red.  If false, the node will be marked as
             *                   black.
             */
            void setRed(bool nowRed = true) {
                setBlack(!nowRed);
            }

            /**
             * Method you can use to obtain the parent node index.
             *
             * \return Returns the index of the parent node.
             */
            UnorderedSetBase::Index parentIndex() const {
                UnorderedSetBase::Index result = currentParentAndRedFlag & parentMask;

                if (result == (UnorderedSetBase::invalidIndex & parentMask)) {
                    result = UnorderedSetBase::invalidIndex;
                }

                return result;
            }

            /**
             * Method you can use to set the parent node index.
             *
             * \param[in] newParentIndex The new index of the parent node.
             */
            void setParentIndex(UnorderedSetBase::Index newParentIndex) {
                currentParentAndRedFlag = (currentParentAndRedFlag & redFlagMask) | (newParentIndex & parentMask);
            }

            /**
             * Method you can use to obtain the left child node index.
             *
             * \return Returns the index of the left child node.
             */
            UnorderedSetBase::Index leftChildIndex() const {
                return currentLeftChild;
            }

            /**
             * Method you can use to set the left child node index.
             *
             * \param[in] newLeftChildIndex The new index of the left child.
             */
            void setLeftChildIndex(UnorderedSetBase::Index newLeftChildIndex) {
                currentLeftChild = newLeftChildIndex;
            }

            /**
             * Method you can use to obtain the right child node index.
             *
             * \return Returns the index of the right child node.
             */
            UnorderedSetBase::Index rightChildIndex() const {
                return currentRightChild;
            }

            /**
             * Method you can use to set the right child node index.
             *
             * \param[in] newRightChildIndex The new index of the right child.
             */
            void setRightChildIndex(UnorderedSetBase::Index newRightChildIndex) {
                currentRightChild = newRightChildIndex;
            }

            /**
             * Assignment operator
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this instance.
             */
            OrderedSetEntry<T>& operator=(const OrderedSetEntry<T>& other) {
                UnorderedSetEntry<T>::operator=(other);

                currentParentAndRedFlag = other.currentParentAndRedFlag;
                currentLeftChild        = other.currentLeftChild;
                currentRightChild       = other.currentRightChild;

                return *this;
            }

        private:
            /**
             * Mask used to identifier the red-black flag.
             */
            static constexpr UnorderedSetBase::Index redFlagMask = 1UL << (8 * sizeof(UnorderedSetBase::Index) - 1);

            /**
             * Mask used to identify the the parent index.
             */
            static constexpr UnorderedSetBase::Index parentMask = redFlagMask - 1;

            /**
             * The parent node index.  The MSB is overloaded to provide the red-black flag for the entry.  An
             * MSB of 1 indicates a red node.
             */
            UnorderedSetBase::Index currentParentAndRedFlag;

            /**
             * The left child node index.
             */
            UnorderedSetBase::Index currentLeftChild;

            /**
             * The current right child node index.
             */
            UnorderedSetBase::Index currentRightChild;
    };
};

#endif
