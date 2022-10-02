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
* This header defines the \ref Model::Range::Private class as well as several associated classes.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_RANGE_PRIVATE_H
#define MODEL_RANGE_PRIVATE_H

#include "model_intrinsic_types.h"
#include "m_variant.h"
#include "model_range.h"

namespace M {
    class Variant;
}

namespace Model {
    /**
     * Class that is used as a private implementation of the \ref Model::Range class.
     */
    class RangePrivateBase {
        public:
            RangePrivateBase();

            virtual ~RangePrivateBase();

            /**
             * Clone method.
             *
             * \return Returns an appropriately constructed copy of this instance.
             */
            virtual RangePrivateBase* clone() const;

            /**
             * Method you can use to determine if the range is empty.
             *
             * \return Returns true if the range is empty.  Returns false if the range is not empty.
             */
            virtual bool isEmpty() const;

            /**
             * Method you can use to determine the size of the range, in elements.
             *
             * \return Returns the current size of the range, in elements.
             */
            virtual Model::Integer size() const;

            /**
             * Method you can use to obtain the first value in the range.
             *
             * \return Returns the first value in the range.
             */
            virtual const M::Variant& first() const;

            /**
             * Method you can use to obtain the second value in the range.
             *
             * \return Returns the second value in the range.
             */
            virtual const M::Variant& second() const;

            /**
             * Method you can use to obtain the last value in the range.
             *
             * \return Returns the last value in the range.
             */
            virtual const M::Variant& last() const;

            /**
             * Method you can use to determine if a value is contained in the range.
             *
             * \param[in] value The value to check for.
             *
             * \return Returns true if the value exists in the range.  Returns false if the value does not exist in the
             *         range.  Note that only values that will be returned by iterating over the range will be
             *         considered as in the range.
             */
            virtual bool contains(const M::Variant& value) const;

            /**
             * Method that calculates the value of the range for a given index value.
             *
             * \param[in] index The zero based index of interest.
             *
             * \return Returns the calculated value at the specified index.
             */
            virtual M::Variant valueAtIndex(unsigned long long index) const;

        private:
            /**
             * Dummy variant used to return values by reference.
             */
            static const M::Variant dummyVariant;
    };

    /**
     * Class that is used as the reported private implementation of the \ref Model::Range class.
     */
    class Range::Private:public RangePrivateBase {
        public:
            Private();

            virtual ~Private();
    };

    /**
     * Private implementation of the \ref Model::Range class.
     *
     * \param[in] T The type tracked by this implementation.
     */
    template<typename T> class RangePrivate:public RangePrivateBase {
        public:
            RangePrivate() {
                currentFirstValue  = T();
                currentSecondValue = T();
                currentLastValue   = T();
                currentIncrement   = T();
                numberSteps        = 0;
            }

            /**
             * Constructor
             *
             * \param[in] first The first value over the range.
             *
             * \param[in] last  The last value over the range.
             */
            RangePrivate(const T& first, const T& last) {
                currentFirstValue = first;
                currentLastValue  = last;

                if (currentLastValue > currentFirstValue) {
                    currentIncrement = T(1);
                    currentSecondValue = currentFirstValue + currentIncrement;
                    if (currentSecondValue > currentLastValue) {
                        currentIncrement   = currentLastValue - currentFirstValue;
                        currentSecondValue = currentLastValue;
                    }
                    numberSteps = static_cast<unsigned long long>(
                        1ULL + (currentLastValue - currentFirstValue) / currentIncrement
                    );
                } else if (currentLastValue == currentFirstValue) {
                    currentIncrement   = 0;
                    currentSecondValue = currentFirstValue;
                    numberSteps        = 1;
                } else {
                    currentIncrement = T(-1);
                    currentSecondValue = currentFirstValue + currentIncrement;
                    if (currentSecondValue < currentLastValue) {
                        currentIncrement   = currentLastValue - currentFirstValue;
                        currentSecondValue = currentLastValue;
                    }
                }

                currentFirstValueVariant  = first;
                currentSecondValueVariant = currentSecondValue;
                currentLastValueVariant   = last;
            }

            /**
             * Constructor
             *
             * \param[in] first  The first value over the range.
             *
             * \param[in] second The second value over the range.
             *
             * \param[in] last   The last value over the range.
             */
            RangePrivate(T first, T second, T last) {
                currentFirstValue  = first;
                currentSecondValue = second;
                currentLastValue   = last;

                if (currentSecondValue != currentFirstValue) {
                    currentIncrement   = currentSecondValue - currentFirstValue;
                    numberSteps        = static_cast<unsigned long long>(
                        1ULL + (currentLastValue - currentFirstValue) / currentIncrement
                    );
                } else {
                    currentIncrement = 0;
                    numberSteps      = 1;
                }

                currentFirstValueVariant  = first;
                currentSecondValueVariant = second;
                currentLastValueVariant   = last;
            }

            ~RangePrivate() override {}

            /**
             * Clone method.
             *
             * \return Returns an appropriately constructed copy of this instance.
             */
            RangePrivateBase* clone() const override {
                RangePrivate<T>* result = new RangePrivate<T>;

                result->currentFirstValue         = currentFirstValue;
                result->currentSecondValue        = currentSecondValue;
                result->currentLastValue          = currentLastValue;
                result->currentIncrement          = currentIncrement;
                result->numberSteps               = numberSteps;
                result->currentFirstValueVariant  = currentFirstValueVariant;
                result->currentSecondValueVariant = currentSecondValueVariant;
                result->currentLastValueVariant   = currentLastValueVariant;

                return result;
            }

            /**
             * Method you can use to determine if the range is empty.
             *
             * \return Returns true if the range is empty.  Returns false if the range is not empty.
             */
            bool isEmpty() const override {
                return false;
            }

            /**
             * Method you can use to determine the size of the range, in elements.
             *
             * \return Returns the current size of the range, in elements.
             */
            Model::Integer size() const override {
                return static_cast<Model::Integer>(numberSteps);
            }

            /**
             * Method you can use to obtain the first value in the range.
             *
             * \return Returns the first value in the range.
             */
            const M::Variant& first() const override {
                return currentFirstValueVariant;
            }

            /**
             * Method you can use to obtain the second value in the range.
             *
             * \return Returns the second value in the range.
             */
            const M::Variant& second() const override {
                return currentSecondValueVariant;
            }

            /**
             * Method you can use to obtain the last value in the range.
             *
             * \return Returns the last value in the range.
             */
            const M::Variant& last() const override {
                return currentLastValueVariant;
            }

            /**
             * Method you can use to determine if a value is contained in the range.
             *
             * \param[in] value The value to check for.
             *
             * \return Returns true if the value exists in the range.  Returns false if the value does not exist in the
             *         range.  Note that only values that will be returned by iterating over the range will be
             *         considered as in the range.
             */
            bool contains(const M::Variant& value) const override  {
                bool inRange;

                T v;;
                toT(v, value);

                if ((currentFirstValue <= currentLastValue &&
                     v >= currentFirstValue                &&
                     v <= currentLastValue                    ) ||
                    (currentFirstValue > currentLastValue &&
                     v >= currentFirstValue               &&
                     v <= currentLastValue                    )    ) {
                    long long i = static_cast<long long>((v - currentFirstValue) / currentIncrement);
                    inRange = (v == (currentIncrement * i) + currentFirstValue);
                } else {
                    inRange = false;
                }

                return inRange;
            }

            /**
             * Method that calculates the value of the range for a given index value.
             *
             * \param[in] index The zero based index of interest.
             *
             * \return Returns the calculated value at the specified index.
             */
            M::Variant valueAtIndex(unsigned long long index) const override {
                return   index < numberSteps
                       ? M::Variant(static_cast<T>(currentFirstValue + index * currentIncrement))
                       : M::Variant();
            }

        private:
            /**
             * Function to convert a variant to an integer value.
             *
             * \param[out] o The result.
             *
             * \param[in]  v The variant value.
             */
            static inline void toT(M::Integer& o, const M::Variant& v) {
                o = v.toInteger();
            }

            /**
             * Function to convert a variant to an integer value.
             *
             * \param[out] o The result.
             *
             * \param[in]  v The variant value.
             */
            static void toT(M::Real& o, const M::Variant& v) {
                o = v.toReal();
            }

            /**
             * The current first value over the range.
             */
            T currentFirstValue;

            /**
             * The current second value over the range.
             */
            T currentSecondValue;

            /**
             * The current last value over the range.
             */
            T currentLastValue;

            /**
             * The current first value over the range.
             */
            M::Variant currentFirstValueVariant;

            /**
             * The current second value over the range.
             */
            M::Variant currentSecondValueVariant;

            /**
             * The current last value over the range.
             */
            M::Variant currentLastValueVariant;

            /**
             * The current increment.
             */
            T currentIncrement;

            /**
             * The number of steps.
             */
            unsigned long long numberSteps;
    };
}

#endif
