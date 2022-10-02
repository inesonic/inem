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
* This header defines functions used to manipulate sets.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_SET_FUNCTIONS_H
#define M_SET_FUNCTIONS_H

#include "m_common.h"
#include "m_intrinsics.h"
#include "m_intrinsic_types.h"
#include "m_type_conversion.h"
#include "m_set.h"
#include "m_tuple.h"
#include "m_variant.h"

namespace M {
    class MatrixBoolean;
    class MatrixInteger;
    class MatrixReal;
    class MatrixComplex;

    /**
     * Class used as a common base class for sets for intrinsic types.  You can instantiate trivial instances of this
     * type as a placeholder to support function/method overloads based on the common types.
     */
    class M_PUBLIC_API IntrinsicTypeSet {
        public:
            IntrinsicTypeSet() = default;

            ~IntrinsicTypeSet() = default;
    };

    /**
     * Class used to represent the set of boolean values.  You can instantiate trivial instances of this type as a
     * placeholder to support function/method overloads based on the common types.
     */
    class M_PUBLIC_API BooleanSet:public IntrinsicTypeSet {
        public:
            /**
             * The global instance.
             */
            static const BooleanSet instance;

            BooleanSet() = default;

            ~BooleanSet() = default;
    };

    /**
     * Class used to represent the set of integer values.  You can instantiate trivial instances of this type as a
     * placeholder to support function/method overloads based on the common types.
     */
    class M_PUBLIC_API IntegerSet:public IntrinsicTypeSet {
        public:
            /**
             * The global instance.
             */
            static const IntegerSet instance;

            IntegerSet() = default;

            ~IntegerSet() = default;
    };

    /**
     * Class used to represent the set of real values.  You can instantiate trivial instances of this type as a
     * placeholder to support function/method overloads based on the common types.
     */
    class M_PUBLIC_API RealSet:public IntrinsicTypeSet {
        public:
            /**
             * The global instance.
             */
            static const RealSet instance;

            RealSet() = default;

            ~RealSet() = default;
    };

    /**
     * Class used to represent the set of complex values.  You can instantiate trivial instances of this type as a
     * placeholder to support function/method overloads based on the common types.
     */
    class M_PUBLIC_API ComplexSet:public IntrinsicTypeSet {
        public:
            /**
             * The global instance.
             */
            static const ComplexSet instance;

            ComplexSet() = default;

            ~ComplexSet() = default;
    };

    /**
     * Function that calculates the cardinality of a set.
     *
     * \param[in] set The set to calculate the cardinarlity of.
     *
     * \return Returns the set cardinality.
     */
    M_PUBLIC_API inline Integer abs(const Set& set) {
        return set.size();
    }

    /**
     * Function that calculates the union of two sets.
     *
     * \param[in] s1 The first set.
     *
     * \param[in] s2 The second set.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API inline Set unionOf(const Set& s1, const Set& s2) {
        return s1.unitedWith(s2);
    }

    /**
     * Function that calculates the union of two sets.
     *
     * \param[in] s1 The first set.
     *
     * \param[in] s2 The second set.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API Set unionOf(const Variant& s1, const Set& s2);

    /**
     * Function that calculates the union of two sets.
     *
     * \param[in] s1 The first set.
     *
     * \param[in] s2 The second set.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API Set unionOf(const Set& s1, const Variant& s2);

    /**
     * Function that calculates the union of two sets.
     *
     * \param[in] s1 The first set.
     *
     * \param[in] s2 The second set.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API Set unionOf(const Variant& s1, const Variant& s2);

    /**
     * Function that calculates the N-ary union of multiple sets stored within a set.
     *
     * \param[in] container The containing set.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API Set unionOf(const Set& container);

    /**
     * Function that calculates the N-ary union of multiple sets stored within a tuple.
     *
     * \param[in] container The containing tuple.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API Set unionOf(const Tuple& container);

    /**
     * Function that calculates the N-ary union of multiple sets stored within a set.
     *
     * \param[in] container The containing set.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API Set unionOf(const Variant& container);

    /**
     * Function that calculates the intersection of two sets.
     *
     * \param[in] s1 The first set.
     *
     * \param[in] s2 The second set.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API inline Set intersectionOf(const Set& s1, const Set& s2) {
        return s1.intersectedWith(s2);
    }

    /**
     * Function that calculates the intersection of two sets.
     *
     * \param[in] s1 The first set.
     *
     * \param[in] s2 The second set.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API Set intersectionOf(const Variant& s1, const Set& s2);

    /**
     * Function that calculates the intersection of two sets.
     *
     * \param[in] s1 The first set.
     *
     * \param[in] s2 The second set.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API Set intersectionOf(const Set& s1, const Variant& s2);

    /**
     * Function that calculates the intersection of two sets.
     *
     * \param[in] s1 The first set.
     *
     * \param[in] s2 The second set.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API Set intersectionOf(const Variant& s1, const Variant& s2);

    /**
     * Function that calculates the disjoint union of two sets.
     *
     * \param[in] s1 The first set.
     *
     * \param[in] s2 The second set.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API Set disjointUnionOf(const Set& s1, const Set& s2);

    /**
     * Function that calculates the disjoint union of two sets.
     *
     * \param[in] s1 The first set.
     *
     * \param[in] s2 The second set.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API Set disjointUnionOf(const Variant& s1, const Set& s2);

    /**
     * Function that calculates the disjoint union of two sets.
     *
     * \param[in] s1 The first set.
     *
     * \param[in] s2 The second set.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API Set disjointUnionOf(const Set& s1, const Variant& s2);

    /**
     * Function that calculates the disjoint union of two sets.
     *
     * \param[in] s1 The first set.
     *
     * \param[in] s2 The second set.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API Set disjointUnionOf(const Variant& s1, const Variant& s2);

    /**
     * Function that calculates the N-ary disjoint union of multiple sets stored within a sets.
     *
     * \param[in] container The containing set.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API Set disjointUnionOf(const Set& container);

    /**
     * Function that calculates the N-ary disjoint union of multiple sets stored within a tuple.
     *
     * \param[in] container The containing tuple.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API Set disjointUnionOf(const Tuple& container);

    /**
     * Function that calculates the N-ary disjoint union of multiple sets stored within a tuple.
     *
     * \param[in] container The containing tuple.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API Set disjointUnionOf(const Variant& container);

    /**
     * Function that calculates the cartesian product of two sets.
     *
     * \param[in] s1 The first set.
     *
     * \param[in] s2 The second set.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API inline Set cartesianProductOf(const Set& s1, const Set& s2) {
        return s1.cartesianProduct(s2);
    }

    /**
     * Function that calculates the cartesian product of two sets.
     *
     * \param[in] s1 The first set.
     *
     * \param[in] s2 The second set.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API Set cartesianProductOf(const Variant& s1, const Set& s2);

    /**
     * Function that calculates the cartesian product of two sets.
     *
     * \param[in] s1 The first set.
     *
     * \param[in] s2 The second set.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API Set cartesianProductOf(const Set& s1, const Variant& s2);

    /**
     * Function that calculates the cartesian product of two sets.
     *
     * \param[in] s1 The first set.
     *
     * \param[in] s2 The second set.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API Set cartesianProductOf(const Variant& s1, const Variant& s2);

    /**
     * Function that calculates the relative complement of two sets.
     *
     * \param[in] s1 The first set.
     *
     * \param[in] s2 The second set.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API inline Set relativeComplementOf(const Set& s1, const Set& s2) {
        return s1.difference(s2);
    }

    /**
     * Function that calculates the relative complement of two sets.
     *
     * \param[in] s1 The first set.
     *
     * \param[in] s2 The second set.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API Set relativeComplementOf(const Variant& s1, const Set& s2);

    /**
     * Function that calculates the relative complement of two sets.
     *
     * \param[in] s1 The first set.
     *
     * \param[in] s2 The second set.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API Set relativeComplementOf(const Set& s1, const Variant& s2);

    /**
     * Function that calculates the relative complement of two sets.
     *
     * \param[in] s1 The first set.
     *
     * \param[in] s2 The second set.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API Set relativeComplementOf(const Variant& s1, const Variant& s2);

    /**
     * Function that calcualtes the symmetric difference of two sets.
     *
     * \param[in] s1 The first set.
     *
     * \param[in] s2 The second set.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API Set symmetricDifferenceOf(const Set& s1, const Set& s2);

    /**
     * Function that calcualtes the symmetric difference of two sets.
     *
     * \param[in] s1 The first set.
     *
     * \param[in] s2 The second set.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API Set symmetricDifferenceOf(const Variant& s1, const Set& s2);

    /**
     * Function that calcualtes the symmetric difference of two sets.
     *
     * \param[in] s1 The first set.
     *
     * \param[in] s2 The second set.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API Set symmetricDifferenceOf(const Set& s1, const Variant& s2);

    /**
     * Function that calcualtes the symmetric difference of two sets.
     *
     * \param[in] s1 The first set.
     *
     * \param[in] s2 The second set.
     *
     * \return Returns the resulting set.
     */
    M_PUBLIC_API Set symmetricDifferenceOf(const Variant& s1, const Variant& s2);

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    template<typename T> M_PUBLIC_TEMPLATE_FUNCTION bool isElementOfSet(const T& element, const Set& set) {
        return set.contains(Variant(element));
    }

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    M_PUBLIC_API inline bool isElementOfSet(Boolean element, const BooleanSet& set) {
        (void) element;
        (void) set;
        return true;
    }

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    M_PUBLIC_API inline bool isElementOfSet(Integer element, const BooleanSet& set) {
        (void) element;
        (void) set;
        return false;
    }

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    M_PUBLIC_API inline bool isElementOfSet(Real element, const BooleanSet& set) {
        (void) element;
        (void) set;
        return false;
    }

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    M_PUBLIC_API inline bool isElementOfSet(Complex element, const BooleanSet& set) {
        (void) element;
        (void) set;
        return false;
    }

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    M_PUBLIC_API inline bool isElementOfSet(const Set& element, const BooleanSet& set) {
        (void) element;
        (void) set;
        return false;
    }

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    M_PUBLIC_API inline bool isElementOfSet(const Tuple& element, const BooleanSet& set) {
        (void) element;
        (void) set;
        return false;
    }

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    M_PUBLIC_API inline bool isElementOfSet(const Variant& element, const BooleanSet& set) {
        (void) set;
        return element.valueType() == ValueType::BOOLEAN;
    }

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    M_PUBLIC_API inline bool isElementOfSet(Boolean element, const IntegerSet& set) {
        (void) element;
        (void) set;
        return true;
    }

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    M_PUBLIC_API inline bool isElementOfSet(Integer element, const IntegerSet& set) {
        (void) element;
        (void) set;
        return true;
    }

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    M_PUBLIC_API bool isElementOfSet(Real element, const IntegerSet& set);

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    M_PUBLIC_API bool isElementOfSet(const Complex &element, const IntegerSet& set);

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    M_PUBLIC_API inline bool isElementOfSet(const Set& element, const IntegerSet& set) {
        (void) element;
        (void) set;
        return false;
    }

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    M_PUBLIC_API inline bool isElementOfSet(const Tuple& element, const IntegerSet& set) {
        (void) element;
        (void) set;
        return false;
    }

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    M_PUBLIC_API inline bool isElementOfSet(const Variant& element, const IntegerSet& set) {
        (void) set;
        return element.canTranslateTo(ValueType::INTEGER);
    }

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    M_PUBLIC_API inline bool isElementOfSet(Boolean element, const RealSet& set) {
        (void) element;
        (void) set;
        return true;
    }

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    M_PUBLIC_API inline bool isElementOfSet(Integer element, const RealSet& set) {
        (void) element;
        (void) set;
        return true;
    }

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    M_PUBLIC_API inline bool isElementOfSet(Real element, const RealSet& set) {
        (void) element;
        (void) set;
        return true;
    }

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    M_PUBLIC_API inline bool isElementOfSet(Complex element, const RealSet& set) {
        (void) set;
        return element.imag() == Real(0);
    }

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    M_PUBLIC_API inline bool isElementOfSet(const Set& element, const RealSet& set) {
        (void) element;
        (void) set;
        return false;
    }

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    M_PUBLIC_API inline bool isElementOfSet(const Tuple& element, const RealSet& set) {
        (void) element;
        (void) set;
        return false;
    }

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    M_PUBLIC_API inline bool isElementOfSet(const Variant& element, const RealSet& set) {
        (void) set;
        return element.canTranslateTo(ValueType::REAL);
    }

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    M_PUBLIC_API inline bool isElementOfSet(Boolean element, const ComplexSet& set) {
        (void) element;
        (void) set;
        return true;
    }

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    M_PUBLIC_API inline bool isElementOfSet(Integer element, const ComplexSet& set) {
        (void) element;
        (void) set;
        return true;
    }

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    M_PUBLIC_API inline bool isElementOfSet(Real element, const ComplexSet& set) {
        (void) element;
        (void) set;
        return true;
    }

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    M_PUBLIC_API inline bool isElementOfSet(Complex element, const ComplexSet& set) {
        (void) element;
        (void) set;
        return true;
    }

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    M_PUBLIC_API inline bool isElementOfSet(const Set& element, const ComplexSet& set) {
        (void) element;
        (void) set;
        return false;
    }

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    M_PUBLIC_API inline bool isElementOfSet(const Tuple& element, const ComplexSet& set) {
        (void) element;
        (void) set;
        return false;
    }

    /**
     * Function that determines if a value is an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is contained in the set.  Returns false if the element is *not* contained
     *         in the set.
     */
    M_PUBLIC_API inline bool isElementOfSet(const Variant& element, const ComplexSet& set) {
        (void) set;
        return element.canTranslateTo(ValueType::COMPLEX);
    }

    // TODO: Add isElementOfSet for matrix and array types.

    /**
     * Function that determines if a value is not an element of a set.
     *
     * \param[in] element The element to check.
     *
     * \param[in] set     The set to check against.
     *
     * \return Returns true if the element is not contained in the set.  Returns false if the element is contained in
     *         the set.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION bool isNotElementOfSet(T1 element, T2 set) {
        return !isElementOfSet(element, set);
    }

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isSubsetOf(const BooleanSet& s1, const BooleanSet& s2) {
        (void) s1;
        (void) s2;
        return true;
    }

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isSubsetOf(const IntegerSet& s1, const BooleanSet& s2) {
        (void) s1;
        (void) s2;
        return false;
    }

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isSubsetOf(const RealSet& s1, const BooleanSet& s2) {
        (void) s1;
        (void) s2;
        return false;
    }

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isSubsetOf(const ComplexSet& s1, const BooleanSet& s2) {
        (void) s1;
        (void) s2;
        return false;
    }

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API bool isSubsetOf(const Set& s1, const BooleanSet& s2);

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isSubsetOf(const BooleanSet& s1, const IntegerSet& s2) {
        (void) s1;
        (void) s2;
        return false;
    }

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isSubsetOf(const IntegerSet& s1, const IntegerSet& s2) {
        (void) s1;
        (void) s2;
        return true;
    }

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isSubsetOf(const RealSet& s1, const IntegerSet& s2) {
        (void) s1;
        (void) s2;
        return false;
    }

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isSubsetOf(const ComplexSet& s1, const IntegerSet& s2) {
        (void) s1;
        (void) s2;
        return false;
    }

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API bool isSubsetOf(const Set& s1, const IntegerSet& s2);

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isSubsetOf(const BooleanSet& s1, const RealSet& s2) {
        (void) s1;
        (void) s2;
        return false;
    }

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isSubsetOf(const IntegerSet& s1, const RealSet& s2) {
        (void) s1;
        (void) s2;
        return true;
    }

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isSubsetOf(const RealSet& s1, const RealSet& s2) {
        (void) s1;
        (void) s2;
        return true;
    }

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isSubsetOf(const ComplexSet& s1, const RealSet& s2) {
        (void) s1;
        (void) s2;
        return false;
    }

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API bool isSubsetOf(const Set& s1, const RealSet& s2);

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isSubsetOf(const BooleanSet& s1, const ComplexSet& s2) {
        (void) s1;
        (void) s2;
        return false;
    }

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isSubsetOf(const IntegerSet& s1, const ComplexSet& s2) {
        (void) s1;
        (void) s2;
        return true;
    }

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isSubsetOf(const RealSet& s1, const ComplexSet& s2) {
        (void) s1;
        (void) s2;
        return true;
    }

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isSubsetOf(const ComplexSet& s1, const ComplexSet& s2) {
        (void) s1;
        (void) s2;
        return true;
    }

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API bool isSubsetOf(const Set& s1, const ComplexSet& s2);

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API bool isSubsetOf(const BooleanSet& s1, const Set& s2);

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isSubsetOf(const IntegerSet& s1, const Set& s2) {
        (void) s1;
        (void) s2;
        return false;
    }

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isSubsetOf(const RealSet& s1, const Set& s2) {
        (void) s1;
        (void) s2;
        return false;
    }

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isSubsetOf(const ComplexSet& s1, const Set& s2) {
        (void) s1;
        (void) s2;
        return false;
    }

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API bool isSubsetOf(const Set& s1, const Set& s2);

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isSubsetOf(const Variant& s1, const Set& s2) {
        return isSubsetOf(s1.toSet(), s2);
    }

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isSubsetOf(const Variant& s1, const BooleanSet& s2) {
        return isSubsetOf(s1.toSet(), s2);
    }

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isSubsetOf(const Variant& s1, const IntegerSet& s2) {
        return isSubsetOf(s1.toSet(), s2);
    }

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isSubsetOf(const Variant& s1, const RealSet& s2) {
        return isSubsetOf(s1.toSet(), s2);
    }

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isSubsetOf(const Variant& s1, const ComplexSet& s2) {
        return isSubsetOf(s1.toSet(), s2);
    }

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isSubsetOf(const Set& s1, const Variant& s2) {
        return isSubsetOf(s1, s2.toSet());
    }

    /**
     * Function that determines if a set is a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isSubsetOf(const Variant& s1, const Variant& s2) {
        return isSubsetOf(s1.toSet(), s2.toSet());
    }

    /**
     * Function that determines if a set is a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isProperSubsetOf(const BooleanSet& s1, const BooleanSet& s2) {
        (void) s1;
        (void) s2;
        return false;
    }

    /**
     * Function that determines if a set is a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isProperSubsetOf(const IntegerSet& s1, const BooleanSet& s2) {
        (void) s1;
        (void) s2;
        return false;
    }

    /**
     * Function that determines if a set is a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isProperSubsetOf(const RealSet& s1, const BooleanSet& s2) {
        (void) s1;
        (void) s2;
        return false;
    }

    /**
     * Function that determines if a set is a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isProperSubsetOf(const ComplexSet& s1, const BooleanSet& s2) {
        (void) s1;
        (void) s2;
        return false;
    }

    /**
     * Function that determines if a set is a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API bool isProperSubsetOf(const Set& s1, const BooleanSet& s2);

    /**
     * Function that determines if a set is a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isProperSubsetOf(const BooleanSet& s1, const IntegerSet& s2) {
        (void) s1;
        (void) s2;
        return true;
    }

    /**
     * Function that determines if a set is a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isProperSubsetOf(const IntegerSet& s1, const IntegerSet& s2) {
        (void) s1;
        (void) s2;
        return false;
    }

    /**
     * Function that determines if a set is a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isProperSubsetOf(const RealSet& s1, const IntegerSet& s2) {
        (void) s1;
        (void) s2;
        return false;
    }

    /**
     * Function that determines if a set is a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isProperSubsetOf(const ComplexSet& s1, const IntegerSet& s2) {
        (void) s1;
        (void) s2;
        return false;
    }

    /**
     * Function that determines if a set is a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isProperSubsetOf(const Set& s1, const IntegerSet& s2) {
        return isSubsetOf(s1, s2);
    }

    /**
     * Function that determines if a set is a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isProperSubsetOf(const BooleanSet& s1, const RealSet& s2) {
        (void) s1;
        (void) s2;
        return true;
    }

    /**
     * Function that determines if a set is a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isProperSubsetOf(const IntegerSet& s1, const RealSet& s2) {
        (void) s1;
        (void) s2;
        return true;
    }

    /**
     * Function that determines if a set is a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isProperSubsetOf(const RealSet& s1, const RealSet& s2) {
        (void) s1;
        (void) s2;
        return false;
    }

    /**
     * Function that determines if a set is a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isProperSubsetOf(const ComplexSet& s1, const RealSet& s2) {
        (void) s1;
        (void) s2;
        return false;
    }

    /**
     * Function that determines if a set is a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isProperSubsetOf(const Set& s1, const RealSet& s2) {
        return isSubsetOf(s1, s2);
    }

    /**
     * Function that determines if a set is a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isProperSubsetOf(const BooleanSet& s1, const ComplexSet& s2) {
        (void) s1;
        (void) s2;
        return true;
    }

    /**
     * Function that determines if a set is a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isProperSubsetOf(const IntegerSet& s1, const ComplexSet& s2) {
        (void) s1;
        (void) s2;
        return true;
    }

    /**
     * Function that determines if a set is a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isProperSubsetOf(const RealSet& s1, const ComplexSet& s2) {
        (void) s1;
        (void) s2;
        return true;
    }

    /**
     * Function that determines if a set is a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isProperSubsetOf(const ComplexSet& s1, const ComplexSet& s2) {
        (void) s1;
        (void) s2;
        return false;
    }

    /**
     * Function that determines if a set is a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isProperSubsetOf(const Set& s1, const ComplexSet& s2) {
        return isSubsetOf(s1, s2);
    }

    /**
     * Function that determines if a set is a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API bool isProperSubsetOf(const BooleanSet& s1, const Set& s2);

    /**
     * Function that determines if a set is a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isProperSubsetOf(const IntegerSet& s1, const Set& s2) {
        (void) s1;
        (void) s2;
        return false;
    }

    /**
     * Function that determines if a set is a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isProperSubsetOf(const RealSet& s1, const Set& s2) {
        (void) s1;
        (void) s2;
        return false;
    }

    /**
     * Function that determines if a set is a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isProperSubsetOf(const ComplexSet& s1, const Set& s2) {
        (void) s1;
        (void) s2;
        return false;
    }

    /**
     * Function that determines if a set is a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API bool isProperSubsetOf(const Set& s1, const Set& s2);

    /**
     * Function that determines if a set is a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    M_PUBLIC_API inline bool isProperSubsetOf(const Variant& s1, const Variant& s2) {
        return isProperSubsetOf(s1.toSet(), s2.toSet());
    }

    /**
     * Function that determines if a set is a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    template<typename T> M_PUBLIC_TEMPLATE_FUNCTION bool isProperSubsetOf(const Variant& s1, const T& s2) {
        return isProperSubsetOf(s1.toSet(), s2);
    }

    /**
     * Function that determines if a set is a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (superset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    template<typename T> M_PUBLIC_TEMPLATE_FUNCTION bool isProperSubsetOf(const T& s1, const Variant& s2) {
        return isProperSubsetOf(s1, s2.toSet());
    }

    /**
     * Function that determines if a set is a superset of another set.
     *
     * \param[in] s1 The set to be tested (proposed superset).
     *
     * \param[in] s2 The set to be tested against (subset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION bool isSupersetOf(const T1& s1, const T2& s2) {
        return isSubsetOf(s2, s1);
    }

    /**
     * Function that determines if a set is a proper superset of another set.
     *
     * \param[in] s1 The set to be tested (proposed superset).
     *
     * \param[in] s2 The set to be tested against (subset).
     *
     * \return Returns true if the provided subset is a proper subset of the superset.  Returns false otherwise.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION bool isProperSupersetOf(const T1& s1, const T2& s2) {
        return isProperSubsetOf(s2, s1);
    }

    /**
     * Function that determines if a set not a subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (subset).
     *
     * \return Returns true if the provided subset is not a subset of the superset.  Returns false otherwise.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION bool isNotSubsetOf(const T1& s1, const T2& s2) {
        return !isSubsetOf(s1, s2);
    }

    /**
     * Function that determines if a set not a proper subset of another set.
     *
     * \param[in] s1 The set to be tested (proposed subset).
     *
     * \param[in] s2 The set to be tested against (subset).
     *
     * \return Returns true if the provided subset is not a proper subset of the superset.  Returns false otherwise.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION bool isNotProperSubsetOf(
            const T1& s1,
            const T2& s2
        ) {
        return !isProperSubsetOf(s1, s2);
    }

    /**
     * Function that determines if a set not a superset of another set.
     *
     * \param[in] s1 The set to be tested (proposed superset).
     *
     * \param[in] s2 The set to be tested against (subset).
     *
     * \return Returns true if the provided subset is not a superset of the subset.  Returns false otherwise.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION bool isNotSupersetOf(const T1& s1, const T2& s2) {
        return !isSubsetOf(s2, s1);
    }

    /**
     * Function that determines if a set not a proper superset of another set.
     *
     * \param[in] s1 The set to be tested (proposed superset).
     *
     * \param[in] s2 The set to be tested against (subset).
     *
     * \return Returns true if the provided subset is not a proper superset of the subset.  Returns false otherwise.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION bool isNotProperSupersetOf(
            const T1& s1,
            const T2& s2
        ) {
        return !isProperSubsetOf(s2, s1);
    }

    /**
     * Function that calculates the size of a set.
     *
     * \param[in] set The set to calculate the size of.
     *
     * \return Returns the set cardinality.
     */
    M_PUBLIC_API inline Integer size(const Set& set) {
        return set.size();
    }

    /**
     * Function that calculates the size of a tuple.
     *
     * \param[in] tuple The tuple to calculate the size of.
     *
     * \return Returns the size of the tuple, in entries.
     */
    M_PUBLIC_API inline Integer size(const Tuple& tuple) {
        return tuple.size();
    }

    /**
     * Function that calculates the alphabet of a set.
     *
     * \param[in] set The set to calculate the alphabet of.
     *
     * \return Returns a set representing the alphabet of the supplied set.
     */
    M_PUBLIC_API inline Set alphabet(const Set& set) {
        return set;
    }

    /**
     * Function that calculates the alphabet of a tuple.
     *
     * \param[in] tuple The tuple to calculate the alphabet of.
     *
     * \return Returns a set representing the alphabet of the supplied tuple.
     */
    M_PUBLIC_API Set alphabet(const Tuple& tuple);

    /**
     * Function that calculates the alphabet of a boolean matrix.
     *
     * \param[in] matrix The matrix to calculate the alphabet of.
     *
     * \return Returns a set representing the alphabet of the supplied matrix.
     */
    M_PUBLIC_API Set alphabet(const MatrixBoolean& matrix);

    /**
     * Function that calculates the alphabet of an integer matrix.
     *
     * \param[in] matrix The matrix to calculate the alphabet of.
     *
     * \return Returns a set representing the alphabet of the supplied matrix.
     */
    M_PUBLIC_API Set alphabet(const MatrixInteger& matrix);

    /**
     * Function that calculates the alphabet of a real matrix.
     *
     * \param[in] matrix The matrix to calculate the alphabet of.
     *
     * \return Returns a set representing the alphabet of the supplied matrix.
     */
    M_PUBLIC_API Set alphabet(const MatrixReal& matrix);

    /**
     * Function that calculates the alphabet of a complex matrix.
     *
     * \param[in] matrix The matrix to calculate the alphabet of.
     *
     * \return Returns a set representing the alphabet of the supplied matrix.
     */
    M_PUBLIC_API Set alphabet(const MatrixComplex& matrix);

    /**
     * Function that converts a scalar value to a string representation.
     *
     * \param[in] scalar The scalar to be converted.
     *
     * \param[in] format An optional tuple containing the format string.  The format string can also contain optional
     *                   text.  The function will throw an exception if the format string could not be decoded.  An
     *                   empty tuple will cause a reasonable format to be used.
     *
     * \return Returns a tuple containing a string representation of the scalar value.
     */
    M_PUBLIC_API Tuple toTupleString(Boolean scalar, const Tuple& format = Tuple());

    /**
     * Function that converts a scalar value to a string representation.
     *
     * \param[in] scalar The scalar to be converted.
     *
     * \param[in] format An optional tuple containing the format string.  The format string can also contain optional
     *                   text.  The function will throw an exception if the format string could not be decoded.  An
     *                   empty tuple will cause a reasonable format to be used.
     *
     * \return Returns a tuple containing a string representation of the scalar value.
     */
    M_PUBLIC_API Tuple toTupleString(Integer scalar, const Tuple& format = Tuple());

    /**
     * Function that converts a scalar value to a string representation.
     *
     * \param[in] scalar The scalar to be converted.
     *
     * \param[in] format An optional tuple containing the format string.  The format string can also contain optional
     *                   text.  The function will throw an exception if the format string could not be decoded.  An
     *                   empty tuple will cause a reasonable format to be used.
     *
     * \return Returns a tuple containing a string representation of the scalar value.
     */
    M_PUBLIC_API Tuple toTupleString(Real scalar, const Tuple& format = Tuple());

    /**
     * Function that converts a scalar value to a string representation.
     *
     * \param[in] scalar The scalar to be converted.
     *
     * \param[in] format An optional tuple containing the format string.  The format string can also contain optional
     *                   text.  The function will throw an exception if the format string could not be decoded.  An
     *                   empty tuple will cause a reasonable format to be used.
     *
     * \return Returns a tuple containing a string representation of the scalar value.
     */
    M_PUBLIC_API Tuple toTupleString(const Complex& scalar, const Tuple& format = Tuple());

    /**
     * Function that converts a scalar value to a string representation.
     *
     * \param[in] scalar The scalar to be converted.
     *
     * \param[in] format An optional tuple containing the format string.  The format string can also contain optional
     *                   text.  The function will throw an exception if the format string could not be decoded.  An
     *                   empty tuple will cause a reasonable format to be used.
     *
     * \return Returns a tuple containing a string representation of the scalar value.
     */
    M_PUBLIC_API Tuple toTupleString(const Variant& scalar, const Tuple& format = Tuple());

    /**
     * Function that converts a scalar value to a string representation.
     *
     * \param[in] scalar The scalar to be converted.
     *
     * \param[in] format An optional tuple containing the format string.  The format string can also contain optional
     *                   text.  The function will throw an exception if the format string could not be decoded.  An
     *                   empty tuple will cause a reasonable format to be used.
     *
     * \return Returns a tuple containing a string representation of the scalar value.
     */
    M_PUBLIC_API inline Tuple toTupleString(Boolean scalar, const Variant& format) {
        return toTupleString(scalar, format.toTuple());
    }

    /**
     * Function that converts a scalar value to a string representation.
     *
     * \param[in] scalar The scalar to be converted.
     *
     * \param[in] format An optional tuple containing the format string.  The format string can also contain optional
     *                   text.  The function will throw an exception if the format string could not be decoded.  An
     *                   empty tuple will cause a reasonable format to be used.
     *
     * \return Returns a tuple containing a string representation of the scalar value.
     */
    M_PUBLIC_API inline Tuple toTupleString(Integer scalar, const Variant& format) {
        return toTupleString(scalar, format.toTuple());
    }

    /**
     * Function that converts a scalar value to a string representation.
     *
     * \param[in] scalar The scalar to be converted.
     *
     * \param[in] format An optional tuple containing the format string.  The format string can also contain optional
     *                   text.  The function will throw an exception if the format string could not be decoded.  An
     *                   empty tuple will cause a reasonable format to be used.
     *
     * \return Returns a tuple containing a string representation of the scalar value.
     */
    M_PUBLIC_API inline Tuple toTupleString(Real scalar, const Variant& format) {
        return toTupleString(scalar, format.toTuple());
    }

    /**
     * Function that converts a scalar value to a string representation.
     *
     * \param[in] scalar The scalar to be converted.
     *
     * \param[in] format An optional tuple containing the format string.  The format string can also contain optional
     *                   text.  The function will throw an exception if the format string could not be decoded.  An
     *                   empty tuple will cause a reasonable format to be used.
     *
     * \return Returns a tuple containing a string representation of the scalar value.
     */
    M_PUBLIC_API inline Tuple toTupleString(const Complex& scalar, const Variant& format) {
        return toTupleString(scalar, format.toTuple());
    }

    /**
     * Function that converts a scalar value to a string representation.
     *
     * \param[in] scalar The scalar to be converted.
     *
     * \param[in] format An optional tuple containing the format string.  The format string can also contain optional
     *                   text.  The function will throw an exception if the format string could not be decoded.  An
     *                   empty tuple will cause a reasonable format to be used.
     *
     * \return Returns a tuple containing a string representation of the scalar value.
     */
    M_PUBLIC_API inline Tuple toTupleString(const Variant& scalar, const Variant& format) {
        return toTupleString(scalar, format.toTuple());
    }

    /**
     * Function that finds the location of a sub-tuple within a tuple.
     *
     * \param[in] tuple      The tuple to be searched.
     *
     * \param[in] subTuple   The sub-tuple to be found.
     *
     * \param[in] startingAt The index of the first location to search.  This function will throw an exception if the
     *                       provided value is zero or negative.
     *
     * \return Returns the index where the sub-tuple resides.  A value of 0 is returned if the sub-tuple does not
     *         reside within the tuple.
     */
    M_PUBLIC_API Integer find(const Tuple& tuple, const Tuple& subTuple, Integer startingAt = 1);

    /**
     * Function that finds the location of a sub-tuple within a tuple.
     *
     * \param[in] tuple      The tuple to be searched.
     *
     * \param[in] subTuple   The sub-tuple to be found.
     *
     * \param[in] startingAt The index of the first location to search.  This function will throw an exception if the
     *                       provided value is zero or negative.
     *
     * \return Returns the index where the sub-tuple resides.  A value of 0 is returned if the sub-tuple does not
     *         reside within the tuple.
     */
    M_PUBLIC_API inline Integer find(const Variant& tuple, const Tuple& subTuple, Integer startingAt = 1) {
        return find(tuple.toTuple(), subTuple, startingAt);
    }

    /**
     * Function that finds the location of a sub-tuple within a tuple.
     *
     * \param[in] tuple      The tuple to be searched.
     *
     * \param[in] subTuple   The sub-tuple to be found.
     *
     * \param[in] startingAt The index of the first location to search.  This function will throw an exception if the
     *                       provided value is zero or negative.
     *
     * \return Returns the index where the sub-tuple resides.  A value of 0 is returned if the sub-tuple does not
     *         reside within the tuple.
     */
    M_PUBLIC_API inline Integer find(const Variant& tuple, const Variant& subTuple, Integer startingAt = 1) {
        return find(tuple.toTuple(), subTuple.toTuple(), startingAt);
    }

    /**
     * Function that finds the location of a sub-tuple within a tuple.
     *
     * \param[in] tuple      The tuple to be searched.
     *
     * \param[in] subTuple   The sub-tuple to be found.
     *
     * \param[in] startingAt The index of the first location to search.  This function will throw an exception if the
     *                       provided value is zero or negative.
     *
     * \return Returns the index where the sub-tuple resides.  A value of 0 is returned if the sub-tuple does not
     *         reside within the tuple.
     */
    M_PUBLIC_API inline Integer find(const Tuple& tuple, const Variant& subTuple, Integer startingAt = 1) {
        return find(tuple, subTuple.toTuple(), startingAt);
    }

    /**
     * Function that finds the location of a sub-tuple within a tuple.
     *
     * \param[in] tuple      The tuple to be searched.
     *
     * \param[in] subTuple   The sub-tuple to be found.
     *
     * \param[in] startingAt The index of the first location to search.  This function will throw an exception if the
     *                       provided value is zero or negative.
     *
     * \return Returns the index where the sub-tuple resides.  A value of 0 is returned if the sub-tuple does not
     *         reside within the tuple.
     */
    template<typename T, typename ST, typename SA> M_PUBLIC_TEMPLATE_FUNCTION Integer find(
            const T&  tuple,
            const ST& subTuple,
            SA        startingAt
        ) {
        Integer result;
        Integer sa;
        if (toInteger(&sa, startingAt)) {
            result = find(tuple, subTuple, sa);
        } else {
            result = 0;
        }

        return result;
    }

    /**
     * Function that splits a tuple into sub-tuples.  Also works for strings.
     *
     * \param[in] tuple       The tuple to be split into sub-tuples.
     *
     * \param[in] splitTerms  The tuple to locate for each split.
     *
     * \param[in] keepSplit   If true, split elements will be maintained as the last element on each line.  If false,
     *                        the split elements will be removed.
     *
     * \param[in] removeEmpty If true, empty sub-tuples will be removed.  If false, empty sub-tuples will be kept.
     *
     * \return Returns a tuple of tuples.
     */
    M_PUBLIC_API Tuple split(
        const Tuple& tuple,
        const Tuple& splitTerms,
        Boolean      keepSplit = false,
        Boolean      removeEmpty = false
    );

    /**
     * Function that splits a tuple into sub-tuples.  Also works for strings.  This version will split if any of the
     * split terms are found.
     *
     * \param[in] tuple       The tuple to be split into sub-tuples.
     *
     * \param[in] splitTerms  The tuple to locate for each split.
     *
     * \param[in] keepSplit   If true, split elements will be maintained as the last element on each line.  If false,
     *                        the split elements will be removed.
     *
     * \param[in] removeEmpty If true, empty sub-tuples will be removed.  If false, empty sub-tuples will be kept.
     *
     * \return Returns a tuple of tuples.
     */
    M_PUBLIC_API Tuple split(
        const Tuple& tuple,
        const Set&   splitTerms,
        Boolean      keepSplit = false,
        Boolean      removeEmpty = false
    );

    /**
     * Function that splits a tuple into sub-tuples.  Also works for strings.
     *
     * \param[in] tuple       The tuple to be split into sub-tuples.
     *
     * \param[in] splitTerms  The tuple to locate for each split.
     *
     * \param[in] keepSplit   If true, split elements will be maintained as the last element on each line.  If false,
     *                        the split elements will be removed.
     *
     * \param[in] removeEmpty If true, empty sub-tuples will be removed.  If false, empty sub-tuples will be kept.
     *
     * \return Returns a tuple of tuples.
     */
    M_PUBLIC_API inline Tuple split(
            const Variant& tuple,
            const Tuple&   splitTerms,
            Boolean        keepSplit = false,
            Boolean        removeEmpty = false
        ) {
        return split(tuple.toTuple(), splitTerms, keepSplit, removeEmpty);
    }

    /**
     * Function that splits a tuple into sub-tuples.  Also works for strings.
     *
     * \param[in] tuple       The tuple to be split into sub-tuples.
     *
     * \param[in] splitTerms  The tuple to locate for each split.
     *
     * \param[in] keepSplit   If true, split elements will be maintained as the last element on each line.  If false,
     *                        the split elements will be removed.
     *
     * \param[in] removeEmpty If true, empty sub-tuples will be removed.  If false, empty sub-tuples will be kept.
     *
     * \return Returns a tuple of tuples.
     */
    M_PUBLIC_API Tuple split(
        const Tuple&   tuple,
        const Variant& splitTerms,
        Boolean        keepSplit = false,
        Boolean        removeEmpty = false
    );

    /**
     * Function that splits a tuple into sub-tuples.  Also works for strings.
     *
     * \param[in] tuple       The tuple to be split into sub-tuples.
     *
     * \param[in] splitTerms  The tuple to locate for each split.
     *
     * \param[in] keepSplit   If true, split elements will be maintained as the last element on each line.  If false,
     *                        the split elements will be removed.
     *
     * \param[in] removeEmpty If true, empty sub-tuples will be removed.  If false, empty sub-tuples will be kept.
     *
     * \return Returns a tuple of tuples.
     */
    M_PUBLIC_API Tuple split(
        const Variant& tuple,
        const Variant& splitTerms,
        Boolean        keepSplit = false,
        Boolean        removeEmpty = false
    );

    /**
     * Function that splits a tuple into sub-tuples.  Also works for strings.  This version will split if any of the
     * split terms are found.
     *
     * \param[in] tuple       The tuple to be split into sub-tuples.
     *
     * \param[in] splitTerms  The tuple to locate for each split.
     *
     * \param[in] keepSplit   If true, split elements will be maintained as the last element on each line.  If false,
     *                        the split elements will be removed.
     *
     * \param[in] removeEmpty If true, empty sub-tuples will be removed.  If false, empty sub-tuples will be kept.
     *
     * \return Returns a tuple of tuples.
     */
    M_PUBLIC_API Tuple split(
        const Variant& tuple,
        const Set&     splitTerms,
        Boolean        keepSplit = false,
        Boolean        removeEmpty = false
    );
}

#endif
