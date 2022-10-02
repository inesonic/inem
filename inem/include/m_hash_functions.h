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
* This header defines hash functions for various types.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_HASH_FUNCTIONS_H
#define MODEL_HASH_FUNCTIONS_H

#include "m_common.h"
#include "m_intrinsics.h"
#include "model_intrinsic_types.h"

namespace Model {
    class Complex;
    class Set;
    class Tuple;
    class Variant;
    class MatrixBoolean;
    class MatrixInteger;
    class MatrixReal;
    class MatrixComplex;
}

namespace M {
    class Set;
    class Tuple;
    class Variant;
    class MatrixBoolean;
    class MatrixInteger;
    class MatrixReal;
    class MatrixComplex;

    /**
     * Hash function for integer types.
     *
     * \param[in] value The value to calculate our hash for.
     *
     * \param[in] seed  A seed to apply to the hash.
     *
     * \return Returns a hash of the provided value.
     */
    M_PUBLIC_API UI64 hashFunction(const int& value, UI32 seed);

    /**
     * Hash function for unsigned integers.
     *
     * \param[in] value The value to calculate our hash for.
     *
     * \param[in] seed  A seed to apply to the hash.
     *
     * \return Returns a hash of the provided value.
     */
    M_PUBLIC_API UI64 hashFunction(const unsigned& value, UI32 seed);

    /**
     * Hash function for the \ref Model::Variant type.
     *
     * \param[in] value The value to calculate our hash for.
     *
     * \param[in] seed  A seed to apply to the hash.
     *
     * \return Returns a hash of the provided value.
     */
    M_PUBLIC_API UI64 hashFunction(const Model::Variant& value, UI32 seed);

    /**
     * Hash function for the Model::Integer type.
     *
     * \param[in] value The value to calculate our hash for.
     *
     * \param[in] seed  A seed to apply to the hash.
     *
     * \return Returns a hash of the provided value.
     */
    M_PUBLIC_API UI64 hashFunction(const Model::Integer& value, UI32 seed);

    /**
     * Hash function for the Model::Real type.
     *
     * \param[in] value The value to calculate our hash for.
     *
     * \param[in] seed  A seed to apply to the hash.
     *
     * \return Returns a hash of the provided value.
     */
    M_PUBLIC_API UI64 hashFunction(const Model::Real& value, UI32 seed);

    /**
     * Hash function for the \ref Model::Complex type.
     *
     * \param[in] value The value to calculate our hash for.
     *
     * \param[in] seed  A seed to apply to the hash.
     *
     * \return Returns a hash of the provided value.
     */
    M_PUBLIC_API UI64 hashFunction(const Model::Complex& value, UI32 seed);

    /**
     * Hash function for the \ref Model::Set type.
     *
     * \param[in] value The value to calculate our hash for.
     *
     * \param[in] seed  A seed to apply to the hash.
     *
     * \return Returns a hash of the provided value.
     */
    M_PUBLIC_API UI64 hashFunction(const Model::Set& value, UI32 seed);

    /**
     * Hash function for the \ref M::Set type.
     *
     * \param[in] value The value to calculate our hash for.
     *
     * \param[in] seed  A seed to apply to the hash.
     *
     * \return Returns a hash of the provided value.
     */
    M_PUBLIC_API UI64 hashFunction(const M::Set& value, UI32 seed);

    /**
     * Hash function for the \ref Model::Tuple type.
     *
     * \param[in] value The value to calculate our hash for.
     *
     * \param[in] seed  A seed to apply to the hash.
     *
     * \return Returns a hash of the provided value.
     */
    M_PUBLIC_API UI64 hashFunction(const Model::Tuple& value, UI32 seed);

    /**
     * Hash function for the \ref M::Tuple type.
     *
     * \param[in] value The value to calculate our hash for.
     *
     * \param[in] seed  A seed to apply to the hash.
     *
     * \return Returns a hash of the provided value.
     */
    M_PUBLIC_API UI64 hashFunction(const M::Tuple& value, UI32 seed);

    /**
     * Hash function for the \ref M::MatrixBoolean type.
     *
     * \param[in] value The value to calculate our hash for.
     *
     * \param[in] seed  A seed to apply to the hash.
     *
     * \return Returns a hash of the provided value.
     */
    M_PUBLIC_API UI64 hashFunction(const Model::MatrixBoolean& value, UI32 seed);

    /**
     * Hash function for the \ref M::MatrixBoolean type.
     *
     * \param[in] value The value to calculate our hash for.
     *
     * \param[in] seed  A seed to apply to the hash.
     *
     * \return Returns a hash of the provided value.
     */
    M_PUBLIC_API UI64 hashFunction(const M::MatrixBoolean& value, UI32 seed);

    /**
     * Hash function for the \ref M::MatrixInteger type.
     *
     * \param[in] value The value to calculate our hash for.
     *
     * \param[in] seed  A seed to apply to the hash.
     *
     * \return Returns a hash of the provided value.
     */
    M_PUBLIC_API UI64 hashFunction(const Model::MatrixInteger& value, UI32 seed);

    /**
     * Hash function for the \ref M::MatrixInteger type.
     *
     * \param[in] value The value to calculate our hash for.
     *
     * \param[in] seed  A seed to apply to the hash.
     *
     * \return Returns a hash of the provided value.
     */
    M_PUBLIC_API UI64 hashFunction(const M::MatrixInteger& value, UI32 seed);

    /**
     * Hash function for the \ref M::MatrixReal type.
     *
     * \param[in] value The value to calculate our hash for.
     *
     * \param[in] seed  A seed to apply to the hash.
     *
     * \return Returns a hash of the provided value.
     */
    M_PUBLIC_API UI64 hashFunction(const Model::MatrixReal& value, UI32 seed);

    /**
     * Hash function for the \ref M::MatrixReal type.
     *
     * \param[in] value The value to calculate our hash for.
     *
     * \param[in] seed  A seed to apply to the hash.
     *
     * \return Returns a hash of the provided value.
     */
    M_PUBLIC_API UI64 hashFunction(const M::MatrixReal& value, UI32 seed);

    /**
     * Hash function for the \ref M::MatrixComplex type.
     *
     * \param[in] value The value to calculate our hash for.
     *
     * \param[in] seed  A seed to apply to the hash.
     *
     * \return Returns a hash of the provided value.
     */
    M_PUBLIC_API UI64 hashFunction(const Model::MatrixComplex& value, UI32 seed);

    /**
     * Hash function for the \ref M::MatrixComplex type.
     *
     * \param[in] value The value to calculate our hash for.
     *
     * \param[in] seed  A seed to apply to the hash.
     *
     * \return Returns a hash of the provided value.
     */
    M_PUBLIC_API UI64 hashFunction(const M::MatrixComplex& value, UI32 seed);

    /**
     * Hash function for the \ref M::Variant type.
     *
     * \param[in] value The value to calculate our hash for.
     *
     * \param[in] seed  A seed to apply to the hash.
     *
     * \return Returns a hash of the provided value.
     */
    M_PUBLIC_API UI64 hashFunction(const M::Variant& value, UI32 seed);

    /**
     * Hash function for the Model::Boolean type.
     *
     * \param[in] value The value to calculate our hash for.
     *
     * \param[in] seed  A seed to apply to the hash.
     *
     * \return Returns a hash of the provided value.
     */
    M_PUBLIC_API UI64 hashFunction(const Model::Boolean& value, UI32 seed);
}

#endif
