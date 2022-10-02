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
* This header defines the \ref Model::Variant class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_VARIANT_H
#define MODEL_VARIANT_H

#include "model_common.h"
#include "model_intrinsic_types.h"
#include "model_api_types.h"
#include "model_complex.h"
#include "model_set.h"

namespace M {
    class Variant;
}

namespace Model {
    class Range;
    class RangeIterator;
    class Tuple;
    class TupleIteratorBase;
    class Set;
    class SetIterator;
    class MatrixBoolean;
    class MatrixInteger;
    class MatrixReal;
    class MatrixComplex;
    class IdentifierData;

    /**
     * Class you can use to store values of different types.
     */
    class MODEL_PUBLIC_API Variant {
        friend class Range;
        friend class RangeIterator;
        friend class Tuple;
        friend class TupleIteratorBase;
        friend class Set;
        friend class SetIterator;
        friend class IdentifierData;
        friend class M::Variant;

        public:
            Variant();

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(Boolean value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(Integer value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(Real value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(Complex value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(const Set& value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(const Tuple& value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(const MatrixBoolean& value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(MatrixBoolean&& value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(const MatrixInteger& value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(MatrixInteger&& value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(const MatrixReal& value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(MatrixReal&& value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(const MatrixComplex& value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(MatrixComplex&& value);

            /**
             * Constructor
             *
             * \param[in] pointer   Pointer to a location containing a value of a specified type.
             *
             * \param[in] valueType The type of the value at the location.
             */
            Variant(const void* pointer, ValueType valueType);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            Variant(const Variant& other);

            /**
             * Move constructor
             *
             * \param[in] other The instance to be moved to this instance.
             */
            Variant(Variant&& other);

            ~Variant();

            /**
             * Method you can use to determine the current underlying data type.
             *
             * \return Returns the current underlying data type.
             */
            ValueType valueType() const;

            /**
             * Method you can use to determine if this variant can translate to a given type.  Note that translation
             * is only allowed if there will be no loss in precision so Real(5) can be translated to Integer(5) but
             * Real(5.5) can not be translated to an integer.
             *
             * \param[in] desiredType The desired data type.
             */
            bool canTranslateTo(ValueType desiredType) const;

            /**
             * Method that determines the best type to use for comparison of variant types.
             *
             * \param[in] type1 The value type of the first variant.
             *
             * \param[in] type2 The value type of the second variant.
             *
             * \return Returns the recommended shared translation type.  A value of ValueType::NONE is returned if the
             *         types are not compatible.
             */
            static ValueType bestUpcast(ValueType type1, ValueType type2);

            /**
             * Method you can use to determine the best up-cast to use for a pair of variants.
             *
             * \param[in] type1 The value type of the first variant.
             *
             * \param[in] v2    The second value to be checked.
             *
             * \return Returns the recommended value type to use for comparisons and math operations between the two
             *         variant types.
             */
            static ValueType bestUpcast(ValueType type1, const Variant& v2);

            /**
             * Method you can use to determine the best up-cast to use for a pair of variants.
             *
             * \param[in] v1    The first value to be checked.
             *
             * \param[in] type2 The value type of the second variant.
             *
             * \return Returns the recommended value type to use for comparisons and math operations between the two
             *         variant types.
             */
            static ValueType bestUpcast(const Variant& v1, ValueType type2);

            /**
             * Method you can use to determine the best up-cast to use for a pair of variants.
             *
             * \param[in] v1 The first value to be checked.
             *
             * \param[in] v2 The second value to be checked.
             *
             * \return Returns the recommended value type to use for comparisons and math operations between the two
             *         variant types.
             */
            static ValueType bestUpcast(const Variant& v1, const Variant& v2);

            /**
             * Method you can use to determine the best upcase across a range of variants.
             *
             * \param[in] p1         The first parameter.
             *
             * \param[in] p2         The second parameter.
             *
             * \param[in] additional Additional parameters.
             *
             * \return Returns the recommended value type to use for comparisons and math operations between the two
             *         variant types.
             */
            template<
                typename T1,
                typename T2,
                typename... Additional
            > MODEL_PUBLIC_TEMPLATE_METHOD static ValueType bestUpcast(
                    const T1&     p1,
                    const T2&     p2,
                    Additional... additional
                ) {
                return bestUpcast(p1, bestUpcast(p2, additional...));
            }

            /**
             * Method you can use to translate the value to an boolean value.
             *
             * \param[in,out] ok Pointer to a boolean you can use to determine if the translation was successful.
             *
             * \return Returns the value translated to an boolean.
             */
            Boolean toBoolean(bool* ok = nullptr) const;

            /**
             * Method you can use to translate the value to an integer value.
             *
             * \param[in,out] ok Pointer to a boolean you can use to determine if the translation was successful.
             *
             * \return Returns the value translated to an integer.
             */
            Integer toInteger(bool* ok = nullptr) const;

            /**
             * Method you can use to translate the value to a real value.
             *
             * \param[in,out] ok Pointer to a boolean you can use to determine if the translation was successful.
             *
             * \return Returns the value translated to a real value.
             */
            Real toReal(bool* ok = nullptr) const;

            /**
             * Method you can use to translate the value to a complex value.
             *
             * \param[in,out] ok Pointer to a boolean you can use to determine if the translation was successful.
             *
             * \return Returns the value translated to a complex value.
             */
            Complex toComplex(bool* ok = nullptr) const;

            /**
             * Method you can use to translate the value to a set.
             *
             * \param[in,out] ok Pointer to a boolean you can use to determine if the translation was successful.
             *
             * \return Returns the value translated to a set.
             */
            Set toSet(bool* ok = nullptr) const;

            /**
             * Method you can use to translate the value to a tuple.
             *
             * \param[in,out] ok Pointer to a boolean you can use to determine if the translation was successful.
             *
             * \return Returns the value translated to a tuple.
             */
            Tuple toTuple(bool* ok = nullptr) const;

            /**
             * Method you can use to translate the value to a matrix of boolean values.
             *
             * \param[in,out] ok Pointer to a boolean you can use to determine if the translation was successful.
             *
             * \return Returns the value translated to a matrix of boolean values.
             */
            MatrixBoolean toMatrixBoolean(bool* ok = nullptr) const;

            /**
             * Method you can use to translate the value to a matrix of integer values.
             *
             * \param[in,out] ok Pointer to a boolean you can use to determine if the translation was successful.
             *
             * \return Returns the value translated to a matrix of integer values.
             */
            MatrixInteger toMatrixInteger(bool* ok = nullptr) const;

            /**
             * Method you can use to translate the value to a matrix of real values.
             *
             * \param[in,out] ok Pointer to a boolean you can use to determine if the translation was successful.
             *
             * \return Returns the value translated to a matrix of real values.
             */
            MatrixReal toMatrixReal(bool* ok = nullptr) const;

            /**
             * Method you can use to translate the value to a matrix of complex values.
             *
             * \param[in,out] ok Pointer to a boolean you can use to determine if the translation was successful.
             *
             * \return Returns the value translated to a matrix of complex values.
             */
            MatrixComplex toMatrixComplex(bool* ok = nullptr) const;

            /**
             * Method you can use to convert the variant to a specific type, programmatically.
             *
             * \param[in,out] pointer   Pointer to a location to be populated with a specified value.
             *
             * \param[in]     valueType The value type of the value at the requested location.
             *
             * \return Returns true on success, returns false on error.
             */
            bool convert(void* pointer, ValueType valueType) const;

            /**
             * Cast to Boolean
             *
             * \throws Model::InvalidRuntimeConversion
             *
             * \return Returns an boolean representation of the variant.
             */
            operator Boolean() const;

            /**
             * Cast to Integer
             *
             * \throws Model::InvalidRuntimeConversion
             *
             * \return Returns an integer representation of the variant.
             */
            operator Integer() const;

            /**
             * Cast to Real
             *
             * \throws Model::InvalidRuntimeConversion
             *
             * \return Returns an integer representation of the variant.
             */
            operator Real() const;

            /**
             * Comparison operator.
             *
             * \param[in] other The value to compare against.
             *
             * \return Returns true if the values are equal.  Returns false if the values are not equal.  Note that the
             *         comparisons are fuzzy.  The underlying data-types do not need to be the same.  This method
             *         always returns false if the types can-not be compared.
             */
            bool operator==(const Variant& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The value to compare against.
             *
             * \return Returns true if the values are equal.  Returns false if the values are not equal.
             */
            bool operator!=(const Variant& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The value to compare against.
             *
             * \return Returns true if this value should precede the other value.  Returns false if this value should
             *         follow the other value.  This method triggers an exception or returns false if the two values
             *         can-not be compared.
             */
            bool operator<(const Variant& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The value to compare against.
             *
             * \return Returns true if this value should follow the other value.  Returns false if this value should
             *         precede the other value.  This method triggers an exception or returns false if the two values
             *         can-not be compared.
             */
            bool operator>(const Variant& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The value to compare against.
             *
             * \return Returns true if this value should precede the other value or is equal to the other value.
             *         Returns false if this value should follow the other value.  This method triggers an exception
             *         or returns false if the two values are not equal and can-not be compared.
             */
            bool operator<=(const Variant& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The value to compare against.
             *
             * \return Returns true if this value should precede the other value.  Returns false if this value should
             *         follow the other value.  This method triggers an exception or returns false if the two values
             *         are not equal and can-not be compared.
             */
            bool operator>=(const Variant& other) const;

            /**
             * Assignment operator
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            Variant& operator=(const Variant& other);

            /**
             * Assignment operator (move semantics).
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            Variant& operator=(Variant&& other);

        private:
            class Private;

            /**
             * Constructor
             *
             * \param[in] newImplementation The new underlying implementation instance.
             */
            inline Variant(Private* newImplementation):impl(newImplementation) {}

            Private* impl;
    };
};

#endif
