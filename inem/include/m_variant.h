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
* This header defines the \ref M::Variant class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_VARIANT_H
#define M_VARIANT_H

#include "m_common.h"
#include "m_api_types.h"
#include "m_intrinsic_types.h"
#include "m_exceptions.h"
#include "m_set.h"
#include "m_tuple.h"
#include "model_matrix_boolean.h"
#include "model_matrix_integer.h"
#include "model_matrix_real.h"
#include "model_matrix_complex.h"
#include "m_matrix_boolean.h"
#include "m_matrix_integer.h"
#include "m_matrix_real.h"
#include "m_matrix_complex.h"
#include "model_variant.h"

namespace Model {
    class Set;
    class Tuple;
    class MatrixBoolean;
    class MatrixInteger;
    class MatrixReal;
    class MatrixComplex;
}

namespace M {
    class MatrixBoolean;
    class MatrixInteger;
    class MatrixReal;
    class MatrixComplex;

    /**
     * Class you can use to store values of different types.
     */
    class M_PUBLIC_API Variant {
        public:
            Variant();

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(M::Boolean value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(M::Integer value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(M::Real value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(M::Complex value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(const Model::Set& value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(const M::Set& value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(const Model::Tuple& value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(const M::Tuple& value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(const Model::MatrixBoolean& value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(Model::MatrixBoolean&& value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(const M::MatrixBoolean& value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(const Model::MatrixInteger& value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(Model::MatrixInteger&& value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(const M::MatrixInteger& value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(const Model::MatrixReal& value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(Model::MatrixReal&& value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(const M::MatrixReal& value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(const Model::MatrixComplex& value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(Model::MatrixComplex&& value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(const M::MatrixComplex& value);

            /**
             * Constructor
             *
             * \param[in] value The value to be stored.
             */
            Variant(const Model::Variant& value);

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
             * Method you can use to translate the value to a boolean value.
             *
             * \param[in,out] ok Pointer to a boolean you can use to determine if the translation was successful.
             *
             * \return Returns the value translated to a boolean.
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
            const Set& toSet(bool* ok = nullptr) const;

            /**
             * Method you can use to translate the value to a tuple.
             *
             * \param[in,out] ok Pointer to a boolean you can use to determine if the translation was successful.
             *
             * \return Returns the value translated to a tuple.
             */
            const Tuple& toTuple(bool* ok = nullptr) const;

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
             * \param[in,out] ok Pointer to a integer you can use to determine if the translation was successful.
             *
             * \return Returns the value translated to a matrix of integer values.
             */
            MatrixInteger toMatrixInteger(bool* ok = nullptr) const;

            /**
             * Method you can use to translate the value to a matrix of real values.
             *
             * \param[in,out] ok Pointer to a real you can use to determine if the translation was successful.
             *
             * \return Returns the value translated to a matrix of real values.
             */
            MatrixReal toMatrixReal(bool* ok = nullptr) const;

            /**
             * Method you can use to translate the value to a matrix of complex values.
             *
             * \param[in,out] ok Pointer to a complex you can use to determine if the translation was successful.
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
             * Method you can use to determien the best up-cast to use for a pair of variants.
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
             * Method you can use to determien the best up-cast to use for a pair of variants.
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
             * Method you can use to determien the best up-cast to use for a pair of variants.
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
             * Template method you can use to determine the best upcast across a range of variants.
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
            > M_PUBLIC_TEMPLATE_METHOD static ValueType bestUpcast(
                    const T1&     p1,
                    const T2&     p2,
                    Additional... additional
                ) {
                return bestUpcast(p1, bestUpcast(p2, additional...));
            }

            /**
             * Method you can use to update a coefficient at a specified row and column.  This method will throw an
             * exception if the contained type is not a matrix and will throw an exception if the specified row or
             * column is invalid.  If needed, the underlying matrix will be resized to contain the coefficient.
             *
             * \param[in] row      The one based row number.
             *
             * \param[in] column   The one based column number.
             *
             * \param[in] newValue The new value at the specified location.
             */
            void update(Model::Matrix::Index row, Model::Matrix::Index column, Variant newValue);

            /**
             * Method you can use to update a coefficient at a specified index.  This method will throw an exception
             * if the contained type is not a matrix or tuple and will throw an exception if the index is invalid.  If
             * needed, the underlying matrix or tuple will be resized to contain the new value.
             *
             * \param[in] index    The one based index into the matrix.
             *
             * \param[in] newValue The new value at the specified location.
             */
            void update(Model::Matrix::Index index, Variant newValue);

            /**
             * Method you can use to obtain a value, by position.  This method will throw an exception if the specified
             * row or column is invalid.
             *
             * \param[in] row    The one based row number.
             *
             * \param[in] column The one based column number.
             *
             * \return Returns the value at the specified location.
             */
            template<typename R, typename C> M_PUBLIC_TEMPLATE_METHOD Variant at(
                    const R& row,
                    const C& column
                ) const {
                switch (currentValueType) {
                    case ValueType::NONE:
                    case ValueType::BOOLEAN:
                    case ValueType::INTEGER:
                    case ValueType::REAL:
                    case ValueType::COMPLEX:
                    case ValueType::SET:
                    case ValueType::TUPLE: {
                        internalTriggerInvalidParameterValueError();
                        return Variant();
                    }

                    case ValueType::MATRIX_BOOLEAN: {
                        return toMatrixBoolean().at(row, column);
                    }

                    case ValueType::MATRIX_INTEGER: {
                        return toMatrixInteger().at(row, column);
                    }

                    case ValueType::MATRIX_REAL: {
                        return toMatrixReal().at(row, column);
                    }

                    case ValueType::MATRIX_COMPLEX: {
                        return toMatrixComplex().at(row, column);
                    }

                    default: {
                        internalTriggerInvalidParameterValueError();
                        return Variant();
                    }
                }
            }

            /**
             * Method you can use to obtain a value, by index.  This method will throw an exception if the specified
             * index is invalid.
             *
             * \param[in] index The one based index to the method.  On matrices, the index will go in row/column order.
             *
             * \return Returns the value at the specified location.
             */
            template<typename I> M_PUBLIC_TEMPLATE_METHOD Variant at(const I& index) const {
                switch (currentValueType) {
                    case ValueType::NONE:
                    case ValueType::BOOLEAN:
                    case ValueType::INTEGER:
                    case ValueType::REAL:
                    case ValueType::COMPLEX:
                    case ValueType::SET: {
                        internalTriggerInvalidParameterValueError();
                        return Variant();
                    }

                    case ValueType::TUPLE: {
                        return toTuple().at(index);
                    }

                    case ValueType::MATRIX_BOOLEAN: {
                        return toMatrixBoolean().at(index);
                    }

                    case ValueType::MATRIX_INTEGER: {
                        return toMatrixInteger().at(index);
                    }

                    case ValueType::MATRIX_REAL: {
                        return toMatrixReal().at(index);
                    }

                    case ValueType::MATRIX_COMPLEX: {
                        return toMatrixComplex().at(index);
                    }

                    default: {
                        internalTriggerInvalidParameterValueError();
                        return Variant();
                    }
                }
            }

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
             * \return Returns true if the values are not.  Returns false if the values are equal.  Note that the
             *         comparisons are fuzzy.  The underlying data-types do not need to be the same.  This method
             *         always returns false if the types can-not be compared.
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
             * \param[in] value The value to be stored.
             *
             * \return Returns a reference to this instance.
             */
            Variant& operator=(Model::Boolean value);

            /**
             * Assignment operator
             *
             * \param[in] value The value to be stored.
             *
             * \return Returns a reference to this instance.
             */
            Variant& operator=(Model::Integer value);

            /**
             * Assignment operator
             *
             * \param[in] value The value to be stored.
             *
             * \return Returns a reference to this instance.
             */
            Variant& operator=(Model::Real value);

            /**
             * Assignment operator
             *
             * \param[in] value The value to be stored.
             *
             * \return Returns a reference to this instance.
             */
            Variant& operator=(Model::Complex value);

            /**
             * Assignment operator
             *
             * \param[in] value The value to be stored.
             *
             * \return Returns a reference to this instance.
             */
            Variant& operator=(const Set& value);

            /**
             * Assignment operator
             *
             * \param[in] value The value to be stored.
             *
             * \return Returns a reference to this instance.
             */
            Variant& operator=(const Tuple& value);

            /**
             * Assignment operator
             *
             * \param[in] value The value to be stored.
             *
             * \return Returns a reference to this instance.
             */
            Variant& operator=(const MatrixBoolean& value);

            /**
             * Assignment operator
             *
             * \param[in] value The value to be stored.
             *
             * \return Returns a reference to this instance.
             */
            Variant& operator=(const MatrixInteger& value);

            /**
             * Assignment operator
             *
             * \param[in] value The value to be stored.
             *
             * \return Returns a reference to this instance.
             */
            Variant& operator=(const MatrixReal& value);

            /**
             * Assignment operator
             *
             * \param[in] value The value to be stored.
             *
             * \return Returns a reference to this instance.
             */
            Variant& operator=(const MatrixComplex& value);

            /**
             * Assignment operator
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            Variant& operator=(const Variant& other);

        private:
            /**
             * A dummy set used as an error case return value.
             */
            static const Set dummySet;

            /**
             * A dummy tuple used as an error case return value.
             */
            static const Tuple dummyTuple;

            /**
             * Method that releases any heap space used by the underlying data-type.
             */
            void releaseMemory();

            /**
             * Method that copies the contents of another variant.
             *
             * \param[in] other The variant to be copied.
             */
            void copy(const Variant& other);

            /**
             * Union of supported variant types.  Placed as the first element so that we can cast to specific types, if
             * needed.
             */
            union M_PUBLIC_API Data {
                Data();

                ~Data();

                /**
                 * Boolean representation
                 */
                Model::Boolean boolean;

                /**
                 * Integer representation
                 */
                Model::Integer integer;

                /**
                 * Real representation
                 */
                Model::Real real;

                /**
                 * Complex representation
                 */
                Model::Complex complex;

                /**
                 * Set representation.
                 */
                M::Set set;

                /**
                 * Tuple representation.
                 */
                M::Tuple tuple;

                /**
                 * Representation as a matrix of boolean values.
                 */
                Model::MatrixBoolean matrixBoolean;

                /**
                 * Representation as a matrix of integer values.
                 */
                Model::MatrixInteger matrixInteger;

                /**
                 * Representation as a matrix of real values.
                 */
                Model::MatrixReal matrixReal;

                /**
                 * Representation as a matrix of complex values.
                 */
                Model::MatrixComplex matrixComplex;
            } data;

            /**
             * The current underlying type.
             */
            ValueType currentValueType;
    };
}

#endif
