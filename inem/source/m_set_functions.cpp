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
* This file implements a collection of functions used to manipulate sets.
***********************************************************************************************************************/

#include <cassert>
#include <complex>
#include <cmath>
#include <limits>
#include <vector>
#include <cstdio>
#include <cstring>
#include <cinttypes>

#include "model_exceptions.h"
#include "m_intrinsic_types.h"
#include "m_exceptions.h"
#include "m_type_conversion.h"
#include "m_special_values.h"
#include "m_set_iterator.h"
#include "m_set.h"
#include "m_tuple.h"
#include "m_matrix_boolean.h"
#include "m_matrix_integer.h"
#include "m_matrix_real.h"
#include "m_matrix_complex.h"
#include "m_set_functions.h"

/***********************************************************************************************************************
 * M::BooleanSet
 */

namespace M {
    const BooleanSet BooleanSet::instance;
}

/***********************************************************************************************************************
 * M::IntegerSet
 */

namespace M {
    const IntegerSet IntegerSet::instance;
}

/***********************************************************************************************************************
 * M::RealSet
 */

namespace M {
    const RealSet RealSet::instance;
}

/***********************************************************************************************************************
 * M::ComplexSet
 */

namespace M {
    const ComplexSet ComplexSet::instance;
}

/***********************************************************************************************************************
 * Functions
 */

namespace M {
    Set unionOf(const Variant& s1, const Set& s2) {
        return s1.toSet().unitedWith(s2);
    }


    Set unionOf(const Set& s1, const Variant& s2) {
        return s1.unitedWith(s2.toSet());
    }


    Set unionOf(const Variant& s1, const Variant& s2) {
        return s1.toSet().unitedWith(s2.toSet());
    }


    Set unionOf(const Set& container) {
        Set result;

        for (Set::ConstIterator it=container.constBegin(),end=container.constEnd() ; it!=end ; ++it) {
            const Variant& variant   = it.constReference();
            ValueType      valueType = variant.valueType();
            if (valueType != ValueType::SET) {
                throw Model::InvalidContainerContents(valueType);
            } else {
                bool       ok;
                const Set& incomingSet = variant.toSet(&ok);
                if (!ok) {
                    throw Model::InvalidRuntimeConversion(valueType, ValueType::SET);
                } else {
                    result.uniteWith(incomingSet);
                }
            }
        }

        return result;
    }


    Set unionOf(const Tuple& container) {
        Set result;

        for (Tuple::ConstIterator it=container.constBegin(),end=container.constEnd() ; it!=end ; ++it) {
            const Variant& variant   = it.constReference();
            ValueType      valueType = variant.valueType();
            if (valueType != ValueType::SET) {
                throw Model::InvalidContainerContents(valueType);
            } else {
                bool       ok;
                const Set& incomingSet = variant.toSet(&ok);
                if (!ok) {
                    throw Model::InvalidRuntimeConversion(valueType, ValueType::SET);
                } else {
                    result.uniteWith(incomingSet);
                }
            }
        }

        return result;
    }


    Set unionOf(const Variant& container) {
        ValueType valueType = container.valueType();
        if (valueType == ValueType::SET) {
            return unionOf(container.toSet());
        } else if (valueType == ValueType::TUPLE) {
            return unionOf(container.toTuple());
        } else {
            M::internalTriggerTypeConversionError(valueType, ValueType::SET);
            return Set();
        }
    }


    Set intersectionOf(const Variant& s1, const Set& s2) {
        return s1.toSet().intersectedWith(s2);
    }


    Set intersectionOf(const Set& s1, const Variant& s2) {
        return s1.intersectedWith(s2.toSet());
    }


    Set intersectionOf(const Variant& s1, const Variant& s2) {
        return s1.toSet().intersectedWith(s2.toSet());
    }


    Set disjointUnionOf(const Set& s1, const Set& s2) {
        Set result;

        for (Set::ConstIterator setIterator = s1.constBegin(), setEndIterator = s1.constEnd()
             ; setIterator != setEndIterator
             ; ++setIterator
            ) {
            M::Tuple tuple;
            tuple.append(setIterator.constReference());
            tuple.append(M::Integer(1));

            result.insert(tuple);
        }

        for (Set::ConstIterator setIterator = s2.constBegin(), setEndIterator = s2.constEnd()
             ; setIterator != setEndIterator
             ; ++setIterator
            ) {
            M::Tuple tuple;
            tuple.append(setIterator.constReference());
            tuple.append(M::Integer(2));

            result.insert(tuple);
        }

        return result;
    }


    Set disjointUnionOf(const Variant& s1, const Set& s2) {
        return disjointUnionOf(s1.toSet(), s2);
    }


    Set disjointUnionOf(const Set& s1, const Variant& s2) {
        return disjointUnionOf(s1, s2.toSet());
    }


    Set disjointUnionOf(const Variant& s1, const Variant& s2) {
        return disjointUnionOf(s1.toSet(), s2.toSet());
    }


    Set disjointUnionOf(const Set& container) {
        Set result;

        Integer index = 1;
        for (  Set::ConstIterator containerIterator    = container.constBegin(),
                                  containerEndIterator = container.constEnd()
             ; containerIterator != containerEndIterator
             ; ++containerIterator
            ) {
            const Variant& variant   = containerIterator.constReference();
            ValueType      valueType = variant.valueType();
            if (valueType != ValueType::SET) {
                throw Model::InvalidContainerContents(valueType);
            } else {
                bool       ok;
                const Set& incomingSet = variant.toSet(&ok);
                if (!ok) {
                    throw Model::InvalidRuntimeConversion(valueType, ValueType::SET);
                } else {
                    for (  Set::ConstIterator setIterator    = incomingSet.constBegin(),
                                              setEndIterator = incomingSet.constEnd()
                         ; setIterator != setEndIterator
                         ; ++setIterator
                        ) {
                        Tuple entryTuple;

                        entryTuple.append(setIterator.constReference());
                        entryTuple.append(index);

                        Variant entryVariant(entryTuple);
                        result.insert(entryVariant);
                    }

                    ++index;
                }
            }
        }

        return result;
    }


    Set disjointUnionOf(const Tuple& container) {
        Set result;

        Integer index = 1;
        for (  Tuple::ConstIterator containerIterator    = container.constBegin(),
                                    containerEndIterator = container.constEnd()
             ; containerIterator != containerEndIterator
             ; ++containerIterator
            ) {
            const Variant& variant   = containerIterator.constReference();
            ValueType      valueType = variant.valueType();
            if (valueType != ValueType::SET) {
                throw Model::InvalidContainerContents(valueType);
            } else {
                bool       ok;
                const Set& incomingSet = variant.toSet(&ok);
                if (!ok) {
                    throw Model::InvalidRuntimeConversion(valueType, ValueType::SET);
                } else {
                    for (  Set::ConstIterator setIterator    = incomingSet.constBegin(),
                                              setEndIterator = incomingSet.constEnd()
                         ; setIterator != setEndIterator
                         ; ++setIterator
                        ) {
                        Tuple entryTuple;

                        entryTuple.append(setIterator.constReference());
                        entryTuple.append(index);

                        Variant entryVariant(entryTuple);
                        result.insert(entryVariant);
                    }

                    ++index;
                }
            }
        }

        return result;
    }


    Set disjointUnionOf(const Variant& container) {
        ValueType valueType = container.valueType();
        if (valueType == ValueType::SET) {
            return disjointUnionOf(container.toSet());
        } else if (valueType == ValueType::TUPLE) {
            return disjointUnionOf(container.toTuple());
        } else {
            M::internalTriggerTypeConversionError(valueType, ValueType::SET);
            return Set();
        }
    }


    Set cartesianProductOf(const Variant& s1, const Set& s2) {
        return cartesianProductOf(s1.toSet(), s2);
    }


    Set cartesianProductOf(const Set& s1, const Variant& s2) {
        return cartesianProductOf(s1, s2.toSet());
    }


    Set cartesianProductOf(const Variant& s1, const Variant& s2) {
        return cartesianProductOf(s1.toSet(), s2.toSet());
    }


    Set relativeComplementOf(const Variant& s1, const Set& s2) {
        return relativeComplementOf(s1.toSet(), s2);
    }


    Set relativeComplementOf(const Set& s1, const Variant& s2) {
        return relativeComplementOf(s1, s2.toSet());
    }


    Set relativeComplementOf(const Variant& s1, const Variant& s2) {
        return relativeComplementOf(s1.toSet(), s2.toSet());
    }


    Set symmetricDifferenceOf(const Set& s1, const Set& s2) {
        Set u = s1.unitedWith(s2);
        Set i = s1.intersectedWith(s2);

        return u.difference(i);
    }


    Set symmetricDifferenceOf(const Variant& s1, const Set& s2) {
        return symmetricDifferenceOf(s1.toSet(), s2);
    }


    Set symmetricDifferenceOf(const Set& s1, const Variant& s2) {
        return symmetricDifferenceOf(s1, s2.toSet());
    }


    Set symmetricDifferenceOf(const Variant& s1, const Variant& s2) {
        return symmetricDifferenceOf(s1.toSet(), s2.toSet());
    }


    bool isElementOfSet(Real element, const IntegerSet& /* set */) {
        Real wholePart;
        return (modf(element, &wholePart) == Real(0));
    }


    bool isElementOfSet(const Complex& element, const IntegerSet& /* set */) {
        Real wholePart;
        return (element.imag() == Real(0) && modf(element.real(), &wholePart) == Real(0));
    }


    static bool isSubsetOfWorker(const Set& s1, const Set& s2) {
        bool               isSubset = true;
        Set::ConstIterator it       = s1.constBegin();
        Set::ConstIterator end      = s1.constEnd();

        while (isSubset && it != end) {
            const Variant& variant = it.constReference();
            if (!s2.contains(variant)) {
                isSubset = false;
            } else {
                ++it;
            }
        }

        return isSubset;
    }


    bool isSubsetOf(const Set& s1, const BooleanSet& /* s2 */) {
        bool result = true;

        if (!s1.isEmpty()) {
            if (s1.size() > 2) {
                result = false;
            } else {
                for (Set::ConstIterator it=s1.constBegin(),end=s1.constEnd() ; it!=end && result ; ++it) {
                    result = (it.constReference().valueType() == ValueType::BOOLEAN);
                }
            }
        }

        return result;
    }


    bool isSubsetOf(const Set& s1, const IntegerSet& /* s2 */) {
        bool result = true;

        for (Set::ConstIterator it=s1.constBegin(),end=s1.constEnd() ; it!=end && result ; ++it) {
            const Variant& variant = it.constReference();
            result = (variant.valueType() != ValueType::BOOLEAN && variant.canTranslateTo(ValueType::INTEGER));
        }

        return result;
    }


    bool isSubsetOf(const Set& s1, const RealSet& /* s2 */) {
        bool result = true;

        for (Set::ConstIterator it=s1.constBegin(),end=s1.constEnd() ; it!=end && result ; ++it) {
            const Variant& variant = it.constReference();
            result = (variant.valueType() != ValueType::BOOLEAN && variant.canTranslateTo(ValueType::REAL));
        }

        return result;
    }


    bool isSubsetOf(const Set& s1, const ComplexSet& /* s2 */) {
        bool result = true;

        for (Set::ConstIterator it=s1.constBegin(),end=s1.constEnd() ; it!=end && result ; ++it) {
            const Variant& variant = it.constReference();
            result = (variant.valueType() != ValueType::BOOLEAN && variant.canTranslateTo(ValueType::COMPLEX));
        }

        return result;

    }


    bool isSubsetOf(const BooleanSet& /* s1 */, const Set& s2) {
        bool result;

        if (s2.size() >= 2) {
            result = s2.contains(Variant(true)) && s2.contains(Variant(false));
        } else {
            result = false;
        }

        return result;
    }


    bool isSubsetOf(const Set& s1, const Set& s2) {
        bool result;

        if (s1.size() <= s2.size()) {
            result = isSubsetOfWorker(s1, s2);
        } else {
            result = false;
        }

        return result;
    }

    bool isProperSubsetOf(const Set& s1, const BooleanSet& /* s2 */) {
        bool result = true;

        if (!s1.isEmpty()) {
            if (s1.size() >= 2) {
                result = false;
            } else {
                for (Set::ConstIterator it=s1.constBegin(),end=s1.constEnd() ; it!=end && result ; ++it) {
                    result = (it.constReference().valueType() == ValueType::BOOLEAN);
                }
            }
        }

        return result;
    }


    bool isProperSubsetOf(const BooleanSet& /* s1 */, const Set& s2) {
        bool result;

        if (s2.size() > 2) {
            result = s2.contains(Variant(true)) && s2.contains(Variant(false));
        } else {
            result = false;
        }

        return result;
    }


    bool isProperSubsetOf(const Set& s1, const Set& s2) {
        bool result;

        if (s1.size() < s2.size()) {
            result = isSubsetOfWorker(s1, s2);
        } else {
            result = false;
        }

        return result;
    }


    Set alphabet(const Tuple& tuple) {
        Set result;

        Integer numberTerms = tuple.size();
        for (Integer i=1 ; i<=numberTerms ; ++i) {
            result.insert(tuple.at(i));
        }

        return result;
    }


    Set alphabet(const MatrixBoolean& matrix) {
        Set result;

        Integer numberRows    = static_cast<unsigned long>(matrix.numberRows());
        Integer numberColumns = static_cast<unsigned long>(matrix.numberColumns());

        Integer row = 1;
        while (row <= numberRows && result.size() < 2) {
            Integer column = 1;
            while (column <= numberColumns && result.size() < 2) {
                result.insert(matrix.at(row, column));
                ++column;
            }
            ++row;
        }

        return result;
    }


    Set alphabet(const MatrixInteger& matrix) {
        Set result;

        Integer numberRows    = static_cast<unsigned long>(matrix.numberRows());
        Integer numberColumns = static_cast<unsigned long>(matrix.numberColumns());

        for (Integer row=1 ; row<=numberRows ; ++row) {
            for (Integer column=1 ; column<=numberColumns ; ++column) {
                result.insert(matrix.at(row, column));
            }
        }

        return result;
    }


    Set alphabet(const MatrixReal& matrix) {
        Set result;

        Integer numberRows    = static_cast<unsigned long>(matrix.numberRows());
        Integer numberColumns = static_cast<unsigned long>(matrix.numberColumns());

        for (Integer row=1 ; row<=numberRows ; ++row) {
            for (Integer column=1 ; column<=numberColumns ; ++column) {
                result.insert(matrix.at(row, column));
            }
        }

        return result;
    }


    Set alphabet(const MatrixComplex& matrix) {
        Set result;

        Integer numberRows    = static_cast<unsigned long>(matrix.numberRows());
        Integer numberColumns = static_cast<unsigned long>(matrix.numberColumns());

        for (Integer row=1 ; row<=numberRows ; ++row) {
            for (Integer column=1 ; column<=numberColumns ; ++column) {
                result.insert(matrix.at(row, column));
            }
        }

        return result;
    }


    Tuple toTupleString(Boolean scalar, const Tuple& format) {
        Tuple result;

        if (format.isEmpty()) {
            result = scalar ? Tuple("true") : Tuple("false");
        } else {
            char* formatString = format.toString();

            char resultString[64];
            int requiredLength = std::snprintf(resultString, 64, formatString, scalar);

            if (requiredLength < 0) {
                throw Model::MalformedString(formatString, 0);
            } else if (requiredLength < 64) {
                result = Tuple(resultString);
            } else {
                char* longResultString = new char[requiredLength + 1];
                std::snprintf(longResultString, requiredLength + 1, formatString, scalar);
                result = Tuple(longResultString);
                delete[] longResultString;
            }
        }

        return result;
    }


    Tuple toTupleString(Integer scalar, const Tuple& format) {
        Tuple result;

        if (format.isEmpty()) {
            char resultString[64];
            std::sprintf(resultString, "%lld", scalar);
            result = Tuple(resultString);
        } else {
            char* formatString = format.toString();

            char resultString[64];
            int requiredLength = std::snprintf(resultString, 64, formatString, scalar);

            if (requiredLength < 0) {
                throw Model::MalformedString(formatString, 0);
            } else if (requiredLength < 64) {
                result = Tuple(resultString);
            } else {
                char* longResultString = new char[requiredLength + 1];
                std::snprintf(longResultString, requiredLength + 1, formatString, scalar);
                result = Tuple(longResultString);
                delete[] longResultString;
            }
        }

        return result;
    }


    Tuple toTupleString(Real scalar, const Tuple& format) {
        Tuple result;

        if (format.isEmpty()) {
            char resultString[64];
            std::sprintf(resultString, "%g", scalar);

            unsigned i = static_cast<unsigned>(std::strlen(resultString));

            if (i > 3 && resultString[i - 2] == '9' && resultString[i - 3] == '9') {
                char resultString2[64];
                std::sprintf(resultString2, "%g", scalar + std::copysign(M::epsilon, scalar));

                unsigned i2 = static_cast<unsigned>(std::strlen(resultString2));
                if (i2 > 3 && resultString2[i -2] == '0' && resultString[i - 3] == '0') {
                    strcpy(resultString, resultString2);
                    i = i2;
                }
            }

            if (i > 3 && resultString[i - 1] != '0' && resultString[i - 2] == '0' && resultString[i - 3] == '0') {
                --i;
            }

            unsigned lastToCheck = resultString[0] == '-' ? 1 : 0;
            do {
                --i;
            } while (i > lastToCheck && resultString[i] == '0');
            resultString[i + 1] = '\0';

            result = Tuple(resultString);
        } else {
            char* formatString = format.toString();

            char resultString[64];
            int requiredLength = std::snprintf(resultString, 64, formatString, scalar);

            if (requiredLength < 0) {
                throw Model::MalformedString(formatString, 0);
            } else if (requiredLength < 64) {
                result = Tuple(resultString);
            } else {
                char* longResultString = new char[requiredLength + 1];
                std::snprintf(longResultString, requiredLength + 1, formatString, scalar);
                result = Tuple(longResultString);
                delete[] longResultString;
            }
        }

        return result;
    }


    Tuple toTupleString(const Complex& scalar, const Tuple& format) {
        Real v;
        return toReal(v, scalar) ? toTupleString(v, format) : Tuple();
    }


    Tuple toTupleString(const Variant& scalar, const Tuple& format) {
        ValueType valueType = scalar.valueType();
        switch (valueType) {
            case ValueType::NONE:
            case ValueType::VARIANT:
            case ValueType::TUPLE:
            case ValueType::SET:
            case ValueType::MATRIX_BOOLEAN:
            case ValueType::MATRIX_INTEGER:
            case ValueType::MATRIX_REAL:
            case ValueType::MATRIX_COMPLEX: {
                internalTriggerTypeConversionError(valueType, ValueType::REAL);
                return Tuple();
            }

            case ValueType::BOOLEAN: {
                return toTupleString(scalar.toBoolean(), format);
            }

            case ValueType::INTEGER: {
                return toTupleString(scalar.toInteger(), format);
            }

            case ValueType::REAL: {
                return toTupleString(scalar.toReal(), format);
            }

            case ValueType::COMPLEX: {
                return toTupleString(scalar.toComplex(), format);
            }

            default: {
                assert(false);
                return Tuple();
            }
        }
    }


    Integer find(const Tuple& tuple, const Tuple& subTuple, Integer startingAt) {
        Integer result = 0;

        if (startingAt > 0) {
            Integer tupleSize = tuple.size();
            Integer subSize   = subTuple.size();

            if (subSize > tupleSize || subSize == 0 || tupleSize == 0) {
                result = 0;
            } else {
                Integer lastSearchLocation = tupleSize - subSize + 1;
                Integer searchIndex        = startingAt;
                Integer compareIndex       = 1;

                do {
                    if (tuple.at(searchIndex + compareIndex - 1) == subTuple.at(compareIndex)) {
                        ++compareIndex;
                        if (compareIndex > subSize) {
                            result = searchIndex;
                        }
                    } else {
                        compareIndex = 1;
                        ++searchIndex;
                    }
                } while (searchIndex <= lastSearchLocation && result == 0);
            }
        } else {
            throw Model::InvalidIndex(startingAt, tuple.size());
        }

        return result;
    }


    Tuple split(const Tuple& tuple, const Tuple& splitTerms, Boolean keepSplit, Boolean removeEmpty) {
        Tuple   result;
        Integer nextSplit;
        Integer currentIndex = 1;
        Integer tupleLength  = tuple.size();

        do {
            nextSplit = find(tuple, splitTerms, currentIndex);
            Integer lastToInclude;
            Integer nextStart;
            if (nextSplit > 0) {
                nextStart     = nextSplit + splitTerms.size();
                lastToInclude = keepSplit ? nextStart - 1 : nextSplit - 1;
            } else {
                lastToInclude = tupleLength;
                nextStart     = tupleLength + 1;
            }

            if (!removeEmpty || currentIndex <= lastToInclude) {
                Tuple   subTuple;
                while (currentIndex <= lastToInclude) {
                    subTuple.append(tuple.at(currentIndex));
                    ++currentIndex;
                }

                result.append(subTuple);
            }

            currentIndex = nextStart;
        } while (currentIndex <= tupleLength);

        return result;
    }


    Tuple split(const Tuple& tuple, const Set& splitTerms, Boolean keepSplit, Boolean removeEmpty) {
        Tuple   result;
        Tuple   subTuple;

        Integer tupleLength = tuple.size();
        for (Integer currentIndex=1 ; currentIndex<=tupleLength ; ++currentIndex) {
            const M::Variant& v = tuple.at(currentIndex);

            if (splitTerms.contains(v)) {
                if (keepSplit) {
                    subTuple.append(v);
                }

                if (!removeEmpty || !subTuple.isEmpty()) {
                    result.append(subTuple);
                }

                subTuple.clear();
            } else {
                subTuple.append(v);
            }
        }

        if (!subTuple.isEmpty()) {
            result.append(subTuple);
        }

        return result;
    }


    M_PUBLIC_API Tuple split(const Tuple& tuple, const Variant& splitTerms, Boolean keepSplit, Boolean removeEmpty) {
        ValueType splitValueType = splitTerms.valueType();
        if (splitValueType == ValueType::TUPLE) {
            return split(tuple, splitTerms.toTuple(), keepSplit, removeEmpty);
        } else if (splitValueType == ValueType::SET) {
            return split(tuple, splitTerms.toSet(), keepSplit, removeEmpty);
        } else {
            internalTriggerTypeConversionError(splitValueType, ValueType::TUPLE);
            return Tuple();
        }
    }


    M_PUBLIC_API Tuple split(const Variant& tuple, const Variant& splitTerms, Boolean keepSplit, Boolean removeEmpty) {
        ValueType splitValueType = splitTerms.valueType();
        if (splitValueType == ValueType::TUPLE) {
            return split(tuple.toTuple(), splitTerms.toTuple(), keepSplit, removeEmpty);
        } else if (splitValueType == ValueType::SET) {
            return split(tuple.toTuple(), splitTerms.toSet(), keepSplit, removeEmpty);
        } else {
            internalTriggerTypeConversionError(splitValueType, ValueType::TUPLE);
            return Tuple();
        }
    }
}
