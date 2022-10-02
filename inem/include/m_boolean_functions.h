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
* This header defines a small collection of boolean functions.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_BOOLEAN_FUNCTIONS_H
#define M_BOOLEAN_FUNCTIONS_H

#include "m_common.h"
#include "m_intrinsics.h"
#include "m_intrinsic_types.h"
#include "m_variant.h"
#include "m_tuple.h"

namespace M {
    /**
     * Function that performs a binary exclusive OR operation.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the exclusive OR of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalExclusiveOr(Boolean p1, Boolean p2) {
        return p1 != p2;
    }

    /**
     * Function that performs a binary exclusive OR operation.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the exclusive OR of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalExclusiveOr(Integer p1, Boolean p2) {
        return (p1 != 0) != p2;
    }

    /**
     * Function that performs a binary exclusive OR operation.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the exclusive OR of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalExclusiveOr(Real p1, Boolean p2) {
        return (p1 != 0) != p2;
    }

    /**
     * Function that performs a binary exclusive OR operation.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the exclusive OR of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalExclusiveOr(const Complex& p1, Boolean p2) {
        return (p1 != 0) != p2;
    }

    /**
     * Function that performs a binary exclusive OR operation.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the exclusive OR of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalExclusiveOr(const Variant& p1, Boolean p2) {
        return p1.toBoolean() != p2;
    }

    /**
     * Function that performs a binary exclusive OR operation.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the exclusive OR of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalExclusiveOr(Boolean p1, Integer p2) {
        return p1 != (p2 != 0);
    }

    /**
     * Function that performs a binary exclusive OR operation.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the exclusive OR of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalExclusiveOr(Integer p1, Integer p2) {
        return (p1 != 0) != (p2 != 0);
    }

    /**
     * Function that performs a binary exclusive OR operation.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the exclusive OR of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalExclusiveOr(Real p1, Integer p2) {
        return (p1 != 0) != (p2 != 0);
    }

    /**
     * Function that performs a binary exclusive OR operation.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the exclusive OR of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalExclusiveOr(const Complex& p1, Integer p2) {
        return (p1 != 0) != (p2 != 0);
    }

    /**
     * Function that performs a binary exclusive OR operation.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the exclusive OR of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalExclusiveOr(const Variant& p1, Integer p2) {
        return p1.toBoolean() != (p2 != 0);
    }

    /**
     * Function that performs a binary exclusive OR operation.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the exclusive OR of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalExclusiveOr(Boolean p1, Real p2) {
        return p1 != (p2 != 0);
    }

    /**
     * Function that performs a binary exclusive OR operation.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the exclusive OR of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalExclusiveOr(Integer p1, Real p2) {
        return (p1 != 0) != (p2 != 0);
    }

    /**
     * Function that performs a binary exclusive OR operation.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the exclusive OR of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalExclusiveOr(Real p1, Real p2) {
        return (p1 != 0) != (p2 != 0);
    }

    /**
     * Function that performs a binary exclusive OR operation.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the exclusive OR of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalExclusiveOr(const Complex& p1, Real p2) {
        return (p1 != 0) != (p2 != 0);
    }

    /**
     * Function that performs a binary exclusive OR operation.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the exclusive OR of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalExclusiveOr(const Variant& p1, Real p2) {
        return p1.toBoolean() != (p2 != 0);
    }

    /**
     * Function that performs a binary exclusive OR operation.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the exclusive OR of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalExclusiveOr(Boolean p1, const Complex& p2) {
        return p1 != (p2 != 0);
    }

    /**
     * Function that performs a binary exclusive OR operation.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the exclusive OR of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalExclusiveOr(Integer p1, const Complex& p2) {
        return (p1 != 0) != (p2 != 0);
    }

    /**
     * Function that performs a binary exclusive OR operation.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the exclusive OR of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalExclusiveOr(Real p1, const Complex& p2) {
        return (p1 != 0) != (p2 != 0);
    }

    /**
     * Function that performs a binary exclusive OR operation.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the exclusive OR of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalExclusiveOr(const Complex& p1, const Complex& p2) {
        return (p1 != 0) != (p2 != 0);
    }

    /**
     * Function that performs a binary exclusive OR operation.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the exclusive OR of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalExclusiveOr(const Variant& p1, const Complex& p2) {
        return p1.toBoolean() != (p2 != 0);
    }

    /**
     * Function that performs a binary exclusive OR operation.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the exclusive OR of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalExclusiveOr(Boolean p1, const Variant& p2) {
        return p1 != p2.toBoolean();
    }

    /**
     * Function that performs a binary exclusive OR operation.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the exclusive OR of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalExclusiveOr(Integer p1, const Variant& p2) {
        return (p1 != 0) != p2.toBoolean();
    }

    /**
     * Function that performs a binary exclusive OR operation.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the exclusive OR of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalExclusiveOr(Real p1, const Variant& p2) {
        return (p1 != 0) != p2.toBoolean();
    }

    /**
     * Function that performs a binary exclusive OR operation.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the exclusive OR of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalExclusiveOr(const Complex& p1, const Variant& p2) {
        return (p1 != 0) != p2.toBoolean();
    }

    /**
     * Function that performs a binary exclusive OR operation.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the exclusive OR of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalExclusiveOr(const Variant& p1, const Variant& p2) {
        return p1.toBoolean() != p2.toBoolean();
    }

    /**
     * Function that provides a logical biconditional operator.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the biconditional of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalBiconditional(Boolean p1, Boolean p2) {
        return p1 == p2;
    }

    /**
     * Function that provides a logical biconditional operator.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the biconditional of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalBiconditional(Integer p1, Boolean p2) {
        return (p1 != 0) == p2;
    }

    /**
     * Function that provides a logical biconditional operator.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the biconditional of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalBiconditional(Real p1, Boolean p2) {
        return (p1 != 0) == p2;
    }

    /**
     * Function that provides a logical biconditional operator.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the biconditional of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalBiconditional(const Complex& p1, Boolean p2) {
        return (p1 != 0) == p2;
    }

    /**
     * Function that provides a logical biconditional operator.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the biconditional of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalBiconditional(const Variant& p1, Boolean p2) {
        return p1.toBoolean() == p2;
    }

    /**
     * Function that provides a logical biconditional operator.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the biconditional of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalBiconditional(Boolean p1, Integer p2) {
        return p1 == (p2 != 0);
    }

    /**
     * Function that provides a logical biconditional operator.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the biconditional of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalBiconditional(Integer p1, Integer p2) {
        return (p1 != 0) == (p2 != 0);
    }

    /**
     * Function that provides a logical biconditional operator.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the biconditional of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalBiconditional(Real p1, Integer p2) {
        return (p1 != 0) == (p2 != 0);
    }

    /**
     * Function that provides a logical biconditional operator.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the biconditional of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalBiconditional(const Complex& p1, Integer p2) {
        return (p1 != 0) == (p2 != 0);
    }

    /**
     * Function that provides a logical biconditional operator.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the biconditional of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalBiconditional(const Variant& p1, Integer p2) {
        return p1.toBoolean() == (p2 != 0);
    }

    /**
     * Function that provides a logical biconditional operator.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the biconditional of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalBiconditional(Boolean p1, Real p2) {
        return p1 == (p2 != 0);
    }

    /**
     * Function that provides a logical biconditional operator.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the biconditional of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalBiconditional(Integer p1, Real p2) {
        return (p1 != 0) == (p2 != 0);
    }

    /**
     * Function that provides a logical biconditional operator.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the biconditional of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalBiconditional(Real p1, Real p2) {
        return (p1 != 0) == (p2 != 0);
    }

    /**
     * Function that provides a logical biconditional operator.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the biconditional of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalBiconditional(const Complex& p1, Real p2) {
        return (p1 != 0) == (p2 != 0);
    }

    /**
     * Function that provides a logical biconditional operator.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the biconditional of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalBiconditional(const Variant& p1, Real p2) {
        return p1.toBoolean() == (p2 != 0);
    }

    /**
     * Function that provides a logical biconditional operator.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the biconditional of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalBiconditional(Boolean p1, const Complex& p2) {
        return p1 == (p2 != 0);
    }

    /**
     * Function that provides a logical biconditional operator.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the biconditional of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalBiconditional(Integer p1, const Complex& p2) {
        return (p1 != 0) == (p2 != 0);
    }

    /**
     * Function that provides a logical biconditional operator.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the biconditional of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalBiconditional(Real p1, const Complex& p2) {
        return (p1 != 0) == (p2 != 0);
    }

    /**
     * Function that provides a logical biconditional operator.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the biconditional of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalBiconditional(const Complex& p1, const Complex& p2) {
        return (p1 != 0) == (p2 != 0);
    }

    /**
     * Function that provides a logical biconditional operator.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the biconditional of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalBiconditional(const Variant& p1, const Complex& p2) {
        return p1.toBoolean() == (p2 != 0);
    }

    /**
     * Function that provides a logical biconditional operator.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the biconditional of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalBiconditional(Boolean p1, const Variant& p2) {
        return p1 == p2.toBoolean();
    }

    /**
     * Function that provides a logical biconditional operator.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the biconditional of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalBiconditional(Integer p1, const Variant& p2) {
        return (p1 != 0) == p2.toBoolean();
    }

    /**
     * Function that provides a logical biconditional operator.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the biconditional of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalBiconditional(Real p1, const Variant& p2) {
        return (p1 != 0) == p2.toBoolean();
    }

    /**
     * Function that provides a logical biconditional operator.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the biconditional of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalBiconditional(const Complex& p1, const Variant& p2) {
        return (p1 != 0) == p2.toBoolean();
    }

    /**
     * Function that provides a logical biconditional operator.
     *
     * \param[in] p1 The first parameter.
     *
     * \param[in] p2 The second parameter.
     *
     * \return Returns the biconditional of p1 and p2.
     */
    M_PUBLIC_API inline Boolean logicalBiconditional(const Variant& p1, const Variant& p2) {
        return p1.toBoolean() == p2.toBoolean();
    }
}

#endif
