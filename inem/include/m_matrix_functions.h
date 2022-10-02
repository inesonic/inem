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
* This header defines a small collection of matrix functions.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_MATRIX_FUNCTIONS_H
#define M_MATRIX_FUNCTIONS_H

#include "m_intrinsics.h"
#include "m_intrinsic_types.h"
#include "m_type_conversion.h"
#include "m_variant.h"
#include "m_basic_functions.h"
#include "m_matrix_boolean.h"
#include "m_matrix_integer.h"
#include "m_matrix_real.h"
#include "m_matrix_complex.h"

namespace M {
    /**
     * Function that determines the number of rows in a matrix.
     *
     * \param[in] m The matrix to be queried.
     *
     * \return Returns the number of matrix rows.
     */
    M_PUBLIC_API inline Integer numberRows(const Model::Matrix& m) {
        return m.numberRows();
    }

    /**
     * Function that determines the number of rows in a matrix.
     *
     * \param[in] m The matrix to be queried.
     *
     * \return Returns the number of matrix rows.
     */
    M_PUBLIC_API Integer numberRows(const Variant& m);

    /**
     * Function that determines the number of rows in a matrix.
     *
     * \param[in] m The matrix to be queried.
     *
     * \return Returns the number of matrix columns.
     */
    M_PUBLIC_API inline Integer numberColumns(const Model::Matrix& m) {
        return m.numberColumns();
    }

    /**
     * Function that determines the number of rows in a matrix.
     *
     * \param[in] m The matrix to be queried.
     *
     * \return Returns the number of matrix rows.
     */
    M_PUBLIC_API Integer numberColumns(const Variant& m);

    /**
     * Function that determines the number of rows in a matrix.
     *
     * \param[in] m The matrix to be queried.
     *
     * \return Returns the number of matrix columns.
     */
    M_PUBLIC_API inline Integer size(const Model::Matrix& m) {
        return m.numberCoefficients();
    }

    /**
     * Function that provides a boolean identity matrix.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \return Returns a boolean matrix of the requested size.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION MatrixBoolean identityBoolean(
            T1 numberRows,
            T2 numberColumns
        ) {
        Integer nr;
        Integer nc;
        if (toInteger(nr, numberRows) || toInteger(nc, numberColumns)) {
            return MatrixBoolean::identity(nr, nc);
        } else {
            return MatrixBoolean(0, 0);
        }
    }

    /**
     * Function that provides a boolean identity matrix.
     *
     * \param[in] numberSide The number of matrix rows and columns.
     *
     * \return Returns a boolean matrix of the requested size.
     */
    template<typename T1> M_PUBLIC_TEMPLATE_FUNCTION MatrixBoolean identityBoolean(T1 numberSide) {
        Integer ns;
        if (toInteger(ns, numberSide)) {
            return MatrixBoolean::identity(ns);
        } else {
            return MatrixBoolean(0, 0);
        }
    }

    /**
     * Function that provides a boolean zero matrix.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \return Returns a boolean matrix of the requested size.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION MatrixBoolean zeroBoolean(
            T1 numberRows,
            T2 numberColumns
        ) {
        Integer nr;
        Integer nc;
        if (toInteger(nr, numberRows) || toInteger(nc, numberColumns)) {
            return MatrixBoolean::zero(nr, nc);
        } else {
            return MatrixBoolean(0, 0);
        }
    }

    /**
     * Function that provides a boolean zero matrix.
     *
     * \param[in] numberSide The number of matrix rows and columns.
     *
     * \return Returns a boolean matrix of the requested size.
     */
    template<typename T1> M_PUBLIC_TEMPLATE_FUNCTION MatrixBoolean zeroBoolean(T1 numberSide) {
        Integer ns;
        if (toInteger(ns, numberSide)) {
            return MatrixBoolean::zero(ns);
        } else {
            return MatrixBoolean(0, 0);
        }
    }

    /**
     * Function that provides a boolean one's matrix.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \return Returns a boolean matrix of the requested size.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION MatrixBoolean oneBoolean(
            T1 numberRows,
            T2 numberColumns
        ) {
        Integer nr;
        Integer nc;
        if (toInteger(nr, numberRows) || toInteger(nc, numberColumns)) {
            return MatrixBoolean::ones(nr, nc);
        } else {
            return MatrixBoolean(0, 0);
        }
    }

    /**
     * Function that provides a boolean one's matrix.
     *
     * \param[in] numberSide The number of matrix rows and columns.
     *
     * \return Returns a boolean matrix of the requested size.
     */
    template<typename T1> M_PUBLIC_TEMPLATE_FUNCTION MatrixBoolean oneBoolean(T1 numberSide) {
        Integer ns;
        if (toInteger(ns, numberSide)) {
            return MatrixBoolean::ones(ns);
        } else {
            return MatrixBoolean(0, 0);
        }
    }

    /**
     * Function that provides a integer identity matrix.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \return Returns a integer matrix of the requested size.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION MatrixInteger identityInteger(
            T1 numberRows,
            T2 numberColumns
        ) {
        Integer nr;
        Integer nc;
        if (toInteger(nr, numberRows) || toInteger(nc, numberColumns)) {
            return MatrixInteger::identity(nr, nc);
        } else {
            return MatrixInteger(0, 0);
        }
    }

    /**
     * Function that provides a integer identity matrix.
     *
     * \param[in] numberSide The number of matrix rows and columns.
     *
     * \return Returns a integer matrix of the requested size.
     */
    template<typename T1> M_PUBLIC_TEMPLATE_FUNCTION MatrixInteger identityInteger(T1 numberSide) {
        Integer ns;
        if (toInteger(ns, numberSide)) {
            return MatrixInteger::identity(ns);
        } else {
            return MatrixInteger(0, 0);
        }
    }

    /**
     * Function that provides a integer zero matrix.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \return Returns a integer matrix of the requested size.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION MatrixInteger zeroInteger(
            T1 numberRows,
            T2 numberColumns
        ) {
        Integer nr;
        Integer nc;
        if (toInteger(nr, numberRows) || toInteger(nc, numberColumns)) {
            return MatrixInteger::zero(nr, nc);
        } else {
            return MatrixInteger(0, 0);
        }
    }

    /**
     * Function that provides a integer zero matrix.
     *
     * \param[in] numberSide The number of matrix rows and columns.
     *
     * \return Returns a integer matrix of the requested size.
     */
    template<typename T1> M_PUBLIC_TEMPLATE_FUNCTION MatrixInteger zeroInteger(T1 numberSide) {
        Integer ns;
        if (toInteger(ns, numberSide)) {
            return MatrixInteger::zero(ns);
        } else {
            return MatrixInteger(0, 0);
        }
    }

    /**
     * Function that provides a integer one's matrix.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \return Returns a integer matrix of the requested size.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION MatrixInteger oneInteger(
            T1 numberRows,
            T2 numberColumns
        ) {
        Integer nr;
        Integer nc;
        if (toInteger(nr, numberRows) || toInteger(nc, numberColumns)) {
            return MatrixInteger::ones(nr, nc);
        } else {
            return MatrixInteger(0, 0);
        }
    }

    /**
     * Function that provides a integer one's matrix.
     *
     * \param[in] numberSide The number of matrix rows and columns.
     *
     * \return Returns a integer matrix of the requested size.
     */
    template<typename T1> M_PUBLIC_TEMPLATE_FUNCTION MatrixInteger oneInteger(T1 numberSide) {
        Integer ns;
        if (toInteger(ns, numberSide)) {
            return MatrixInteger::ones(ns);
        } else {
            return MatrixInteger(0, 0);
        }
    }

    /**
     * Function that provides a real identity matrix.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \return Returns a real matrix of the requested size.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION MatrixReal identityReal(
            T1 numberRows,
            T2 numberColumns
        ) {
        Integer nr;
        Integer nc;
        if (toInteger(nr, numberRows) || toInteger(nc, numberColumns)) {
            return MatrixReal::identity(nr, nc);
        } else {
            return MatrixReal(0, 0);
        }
    }

    /**
     * Function that provides a real identity matrix.
     *
     * \param[in] numberSide    The number of matrix rows and columns.
     *
     * \return Returns a real matrix of the requested size.
     */
    template<typename T1> M_PUBLIC_TEMPLATE_FUNCTION MatrixReal identityReal(T1 numberSide) {
        Integer ns;
        if (toInteger(ns, numberSide)) {
            return MatrixReal::identity(ns);
        } else {
            return MatrixReal(0, 0);
        }
    }

    /**
     * Function that provides a real zero matrix.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \return Returns a real matrix of the requested size.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION MatrixReal zeroReal(
            T1 numberRows,
            T2 numberColumns
        ) {
        Integer nr;
        Integer nc;
        if (toInteger(nr, numberRows) || toInteger(nc, numberColumns)) {
            return MatrixReal::zero(nr, nc);
        } else {
            return MatrixReal(0, 0);
        }
    }

    /**
     * Function that provides a real zero matrix.
     *
     * \param[in] numberSide The number of matrix rows and columns.
     *
     * \return Returns a real matrix of the requested size.
     */
    template<typename T1> M_PUBLIC_TEMPLATE_FUNCTION MatrixReal zeroReal(T1 numberSide) {
        Integer ns;
        if (toInteger(ns, numberSide)) {
            return MatrixReal::zero(ns);
        } else {
            return MatrixReal(0, 0);
        }
    }

    /**
     * Function that provides a real one's matrix.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \return Returns a real matrix of the requested size.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION MatrixReal oneReal(
            T1 numberRows,
            T2 numberColumns
        ) {
        Integer nr;
        Integer nc;
        if (toInteger(nr, numberRows) || toInteger(nc, numberColumns)) {
            return MatrixReal::ones(nr, nc);
        } else {
            return MatrixReal(0, 0);
        }
    }

    /**
     * Function that provides a real one's matrix.
     *
     * \param[in] numberSide The number of matrix rows and columns.
     *
     * \return Returns a real matrix of the requested size.
     */
    template<typename T1> M_PUBLIC_TEMPLATE_FUNCTION MatrixReal oneReal(T1 numberSide) {
        Integer ns;
        if (toInteger(ns, numberSide)) {
            return MatrixReal::ones(ns);
        } else {
            return MatrixReal(0, 0);
        }
    }

    /**
     * Function that provides a complex identity matrix.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \return Returns a complex matrix of the requested size.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION MatrixComplex identityComplex(
            T1 numberRows,
            T2 numberColumns
        ) {
        Integer nr;
        Integer nc;
        if (toInteger(nr, numberRows) || toInteger(nc, numberColumns)) {
            return MatrixComplex::identity(nr, nc);
        } else {
            return MatrixComplex(0, 0);
        }
    }

    /**
     * Function that provides a complex identity matrix.
     *
     * \param[in] numberSide The number of matrix rows and columns.
     *
     * \return Returns a complex matrix of the requested size.
     */
    template<typename T1> M_PUBLIC_TEMPLATE_FUNCTION MatrixComplex identityComplex(T1 numberSide) {
        Integer ns;
        if (toInteger(ns, numberSide)) {
            return MatrixComplex::identity(ns);
        } else {
            return MatrixComplex(0, 0);
        }
    }

    /**
     * Function that provides a complex zero matrix.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \return Returns a complex matrix of the requested size.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION MatrixComplex zeroComplex(
            T1 numberRows,
            T2 numberColumns
        ) {
        Integer nr;
        Integer nc;
        if (toInteger(nr, numberRows) || toInteger(nc, numberColumns)) {
            return MatrixComplex::zero(nr, nc);
        } else {
            return MatrixComplex(0, 0);
        }
    }

    /**
     * Function that provides a complex zero matrix.
     *
     * \param[in] numberSide The number of matrix rows and columns.
     *
     * \return Returns a complex matrix of the requested size.
     */
    template<typename T1> M_PUBLIC_TEMPLATE_FUNCTION MatrixComplex zeroComplex(T1 numberSide) {
        Integer ns;
        if (toInteger(ns, numberSide)) {
            return MatrixComplex::zero(ns);
        } else {
            return MatrixComplex(0, 0);
        }
    }

    /**
     * Function that provides a complex one's matrix.
     *
     * \param[in] numberRows    The number of matrix rows.
     *
     * \param[in] numberColumns The number of matrix columns.
     *
     * \return Returns a complex matrix of the requested size.
     */
    template<typename T1, typename T2> M_PUBLIC_TEMPLATE_FUNCTION MatrixComplex oneComplex(
            T1 numberRows,
            T2 numberColumns
        ) {
        Integer nr;
        Integer nc;
        if (toInteger(nr, numberRows) || toInteger(nc, numberColumns)) {
            return MatrixComplex::ones(nr, nc);
        } else {
            return MatrixComplex(0, 0);
        }
    }

    /**
     * Function that provides a complex one's matrix.
     *
     * \param[in] numberSide The number of matrix rows and columns.
     *
     * \return Returns a complex matrix of the requested size.
     */
    template<typename T1> M_PUBLIC_TEMPLATE_FUNCTION MatrixComplex oneComplex(T1 numberSide) {
        Integer ns;
        if (toInteger(ns, numberSide)) {
            return MatrixComplex::ones(ns);
        } else {
            return MatrixComplex(0, 0);
        }
    }

    /**
     * Function that obtain the diagonal entries of a matrix.
     *
     * \param[in] matrix The matrix to extract the diagonal entries from.
     *
     * \return Returns a column matrix holding the diagonal entries.
     */
    M_PUBLIC_API inline MatrixBoolean diagonalEntries(const MatrixBoolean& matrix) {
        return matrix.diagonalEntries();
    }

    /**
     * Function that obtain the diagonal entries of a matrix.
     *
     * \param[in] matrix The matrix to extract the diagonal entries from.
     *
     * \return Returns a column matrix holding the diagonal entries.
     */
    M_PUBLIC_API inline MatrixInteger diagonalEntries(const MatrixInteger& matrix) {
        return matrix.diagonalEntries();
    }

    /**
     * Function that obtain the diagonal entries of a matrix.
     *
     * \param[in] matrix The matrix to extract the diagonal entries from.
     *
     * \return Returns a column matrix holding the diagonal entries.
     */
    M_PUBLIC_API inline MatrixReal diagonalEntries(const MatrixReal& matrix) {
        return matrix.diagonalEntries();
    }

    /**
     * Function that obtain the diagonal entries of a matrix.
     *
     * \param[in] matrix The matrix to extract the diagonal entries from.
     *
     * \return Returns a column matrix holding the diagonal entries.
     */
    M_PUBLIC_API inline MatrixComplex diagonalEntries(const MatrixComplex& matrix) {
        return matrix.diagonalEntries();
    }

    /**
     * Function that obtain the diagonal entries of a matrix.
     *
     * \param[in] matrix The matrix to extract the diagonal entries from.
     *
     * \return Returns a column matrix holding the diagonal entries.
     */
    M_PUBLIC_API Variant diagonalEntries(const Variant& matrix);

    /**
     * Function that creates a diagonal matrix from a row/column matrix.
     *
     * \param[in] matrix The matrix holding the diagonal entry values.
     *
     * \return Returns a diagonal matrix.
     */
    M_PUBLIC_API inline MatrixBoolean diagonal(const MatrixBoolean& matrix) {
        return matrix.diagonal();
    }

    /**
     * Function that creates a diagonal matrix from a row/column matrix.
     *
     * \param[in] matrix The matrix holding the diagonal entry values.
     *
     * \return Returns a diagonal matrix.
     */
    M_PUBLIC_API inline MatrixInteger diagonal(const MatrixInteger& matrix) {
        return matrix.diagonal();
    }

    /**
     * Function that creates a diagonal matrix from a row/column matrix.
     *
     * \param[in] matrix The matrix holding the diagonal entry values.
     *
     * \return Returns a diagonal matrix.
     */
    M_PUBLIC_API inline MatrixReal diagonal(const MatrixReal& matrix) {
        return matrix.diagonal();
    }

    /**
     * Function that creates a diagonal matrix from a row/column matrix.
     *
     * \param[in] matrix The matrix holding the diagonal entry values.
     *
     * \return Returns a diagonal matrix.
     */
    M_PUBLIC_API inline MatrixComplex diagonal(const MatrixComplex& matrix) {
        return matrix.diagonal();
    }

    /**
     * Function that creates a diagonal matrix from a row/column matrix.
     *
     * \param[in] matrix The matrix holding the diagonal entry values.
     *
     * \return Returns a diagonal matrix.
     */
    M_PUBLIC_API Variant diagonal(const Variant& matrix);

    /**
     * Function that combines two matrices in left-to-right order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixBoolean matrixCombineLeftToRight(const MatrixBoolean& m1, const MatrixBoolean& m2) {
        return m1.combineLeftToRight(m2);
    }

    /**
     * Function that combines two matrices in left-to-right order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixInteger matrixCombineLeftToRight(const MatrixBoolean& m1, const MatrixInteger& m2) {
        return MatrixInteger(m1).combineLeftToRight(m2);
    }

    /**
     * Function that combines two matrices in left-to-right order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixReal matrixCombineLeftToRight(const MatrixBoolean& m1, const MatrixReal& m2) {
        return MatrixReal(m1).combineLeftToRight(m2);
    }

    /**
     * Function that combines two matrices in left-to-right order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixComplex matrixCombineLeftToRight(const MatrixBoolean& m1, const MatrixComplex& m2) {
        return MatrixComplex(m1).combineLeftToRight(m2);
    }

    /**
     * Function that combines two matrices in left-to-right order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixInteger matrixCombineLeftToRight(const MatrixInteger& m1, const MatrixBoolean& m2) {
        return m1.combineLeftToRight(m2);
    }

    /**
     * Function that combines two matrices in left-to-right order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixInteger matrixCombineLeftToRight(const MatrixInteger& m1, const MatrixInteger& m2) {
        return m1.combineLeftToRight(m2);
    }

    /**
     * Function that combines two matrices in left-to-right order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixReal matrixCombineLeftToRight(const MatrixInteger& m1, const MatrixReal& m2) {
        return MatrixReal(m1).combineLeftToRight(m2);
    }

    /**
     * Function that combines two matrices in left-to-right order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixComplex matrixCombineLeftToRight(const MatrixInteger& m1, const MatrixComplex& m2) {
        return MatrixComplex(m1).combineLeftToRight(m2);
    }

    /**
     * Function that combines two matrices in left-to-right order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixReal matrixCombineLeftToRight(const MatrixReal& m1, const MatrixBoolean& m2) {
        return m1.combineLeftToRight(m2);
    }

    /**
     * Function that combines two matrices in left-to-right order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixReal matrixCombineLeftToRight(const MatrixReal& m1, const MatrixInteger& m2) {
        return m1.combineLeftToRight(m2);
    }

    /**
     * Function that combines two matrices in left-to-right order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixReal matrixCombineLeftToRight(const MatrixReal& m1, const MatrixReal& m2) {
        return m1.combineLeftToRight(m2);
    }

    /**
     * Function that combines two matrices in left-to-right order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixComplex matrixCombineLeftToRight(const MatrixReal& m1, const MatrixComplex& m2) {
        return MatrixComplex(m1).combineLeftToRight(m2);
    }

    /**
     * Function that combines two matrices in left-to-right order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixComplex matrixCombineLeftToRight(const MatrixComplex& m1, const MatrixBoolean& m2) {
        return m1.combineLeftToRight(m2);
    }

    /**
     * Function that combines two matrices in left-to-right order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixComplex matrixCombineLeftToRight(const MatrixComplex& m1, const MatrixInteger& m2) {
        return m1.combineLeftToRight(m2);
    }

    /**
     * Function that combines two matrices in left-to-right order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixComplex matrixCombineLeftToRight(const MatrixComplex& m1, const MatrixReal& m2) {
        return m1.combineLeftToRight(m2);
    }

    /**
     * Function that combines two matrices in left-to-right order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixComplex matrixCombineLeftToRight(const MatrixComplex& m1, const MatrixComplex& m2) {
        return m1.combineLeftToRight(m2);
    }

    /**
     * Function that combines two matrices in left-to-right order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API Variant matrixCombineLeftToRight(const Variant& m1, const MatrixBoolean& m2);

    /**
     * Function that combines two matrices in left-to-right order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API Variant matrixCombineLeftToRight(const Variant& m1, const MatrixInteger& m2);

    /**
     * Function that combines two matrices in left-to-right order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API Variant matrixCombineLeftToRight(const Variant& m1, const MatrixReal& m2);

    /**
     * Function that combines two matrices in left-to-right order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API MatrixComplex matrixCombineLeftToRight(const Variant& m1, const MatrixComplex& m2);

    /**
     * Function that combines two matrices in left-to-right order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API Variant matrixCombineLeftToRight(const MatrixBoolean& m1, const Variant& m2);

    /**
     * Function that combines two matrices in left-to-right order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API Variant matrixCombineLeftToRight(const MatrixInteger& m1, const Variant& m2);

    /**
     * Function that combines two matrices in left-to-right order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API Variant matrixCombineLeftToRight(const MatrixReal& m1, const Variant& m2);

    /**
     * Function that combines two matrices in left-to-right order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API MatrixComplex matrixCombineLeftToRight(const MatrixComplex& m1, const Variant& m2);

    /**
     * Function that combines two matrices in left-to-right order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API Variant matrixCombineLeftToRight(const Variant& m1, const Variant& m2);

    /**
     * Function that combines two matrices in top-to-bottom order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixBoolean matrixCombineTopToBottom(const MatrixBoolean& m1, const MatrixBoolean& m2) {
        return m1.combineTopToBottom(m2);
    }

    /**
     * Function that combines two matrices in top-to-bottom order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixInteger matrixCombineTopToBottom(const MatrixBoolean& m1, const MatrixInteger& m2) {
        return MatrixInteger(m1).combineTopToBottom(m2);
    }

    /**
     * Function that combines two matrices in top-to-bottom order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixReal matrixCombineTopToBottom(const MatrixBoolean& m1, const MatrixReal& m2) {
        return MatrixReal(m1).combineTopToBottom(m2);
    }

    /**
     * Function that combines two matrices in top-to-bottom order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixComplex matrixCombineTopToBottom(const MatrixBoolean& m1, const MatrixComplex& m2) {
        return MatrixComplex(m1).combineTopToBottom(m2);
    }

    /**
     * Function that combines two matrices in top-to-bottom order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixInteger matrixCombineTopToBottom(const MatrixInteger& m1, const MatrixBoolean& m2) {
        return m1.combineTopToBottom(m2);
    }

    /**
     * Function that combines two matrices in top-to-bottom order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixInteger matrixCombineTopToBottom(const MatrixInteger& m1, const MatrixInteger& m2) {
        return m1.combineTopToBottom(m2);
    }

    /**
     * Function that combines two matrices in top-to-bottom order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixReal matrixCombineTopToBottom(const MatrixInteger& m1, const MatrixReal& m2) {
        return MatrixReal(m1).combineTopToBottom(m2);
    }

    /**
     * Function that combines two matrices in top-to-bottom order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixComplex matrixCombineTopToBottom(const MatrixInteger& m1, const MatrixComplex& m2) {
        return MatrixComplex(m1).combineTopToBottom(m2);
    }

    /**
     * Function that combines two matrices in top-to-bottom order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixReal matrixCombineTopToBottom(const MatrixReal& m1, const MatrixBoolean& m2) {
        return m1.combineTopToBottom(m2);
    }

    /**
     * Function that combines two matrices in top-to-bottom order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixReal matrixCombineTopToBottom(const MatrixReal& m1, const MatrixInteger& m2) {
        return m1.combineTopToBottom(m2);
    }

    /**
     * Function that combines two matrices in top-to-bottom order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixReal matrixCombineTopToBottom(const MatrixReal& m1, const MatrixReal& m2) {
        return m1.combineTopToBottom(m2);
    }

    /**
     * Function that combines two matrices in top-to-bottom order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixComplex matrixCombineTopToBottom(const MatrixReal& m1, const MatrixComplex& m2) {
        return MatrixComplex(m1).combineTopToBottom(m2);
    }

    /**
     * Function that combines two matrices in top-to-bottom order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixComplex matrixCombineTopToBottom(const MatrixComplex& m1, const MatrixBoolean& m2) {
        return m1.combineTopToBottom(m2);
    }

    /**
     * Function that combines two matrices in top-to-bottom order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixComplex matrixCombineTopToBottom(const MatrixComplex& m1, const MatrixInteger& m2) {
        return m1.combineTopToBottom(m2);
    }

    /**
     * Function that combines two matrices in top-to-bottom order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixComplex matrixCombineTopToBottom(const MatrixComplex& m1, const MatrixReal& m2) {
        return m1.combineTopToBottom(m2);
    }

    /**
     * Function that combines two matrices in top-to-bottom order.
     *
     * \param[in] m1 The left matrix to combine.
     *
     * \param[in] m2 The right matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API inline MatrixComplex matrixCombineTopToBottom(const MatrixComplex& m1, const MatrixComplex& m2) {
        return m1.combineTopToBottom(m2);
    }

    /**
     * Function that combines two matrices in top-to-bottom order.
     *
     * \param[in] m1 The top matrix to combine.
     *
     * \param[in] m2 The bottom matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API Variant matrixCombineTopToBottom(const Variant& m1, const MatrixBoolean& m2);

    /**
     * Function that combines two matrices in top-to-bottom order.
     *
     * \param[in] m1 The top matrix to combine.
     *
     * \param[in] m2 The bottom matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API Variant matrixCombineTopToBottom(const Variant& m1, const MatrixInteger& m2);

    /**
     * Function that combines two matrices in top-to-bottom order.
     *
     * \param[in] m1 The top matrix to combine.
     *
     * \param[in] m2 The bottom matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API Variant matrixCombineTopToBottom(const Variant& m1, const MatrixReal& m2);

    /**
     * Function that combines two matrices in top-to-bottom order.
     *
     * \param[in] m1 The top matrix to combine.
     *
     * \param[in] m2 The bottom matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API MatrixComplex matrixCombineTopToBottom(const Variant& m1, const MatrixComplex& m2);

    /**
     * Function that combines two matrices in top-to-bottom order.
     *
     * \param[in] m1 The top matrix to combine.
     *
     * \param[in] m2 The bottom matrix to combine.
     */
    M_PUBLIC_API Variant matrixCombineTopToBottom(const MatrixBoolean& m1, const Variant& m2);

    /**
     * Function that combines two matrices in top-to-bottom order.
     *
     * \param[in] m1 The top matrix to combine.
     *
     * \param[in] m2 The bottom matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API Variant matrixCombineTopToBottom(const MatrixInteger& m1, const Variant& m2);

    /**
     * Function that combines two matrices in top-to-bottom order.
     *
     * \param[in] m1 The top matrix to combine.
     *
     * \param[in] m2 The bottom matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API Variant matrixCombineTopToBottom(const MatrixReal& m1, const Variant& m2);

    /**
     * Function that combines two matrices in top-to-bottom order.
     *
     * \param[in] m1 The top matrix to combine.
     *
     * \param[in] m2 The bottom matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API MatrixComplex matrixCombineTopToBottom(const MatrixComplex& m1, const Variant& m2);

    /**
     * Function that combines two matrices in top-to-bottom order.
     *
     * \param[in] m1 The top matrix to combine.
     *
     * \param[in] m2 The bottom matrix to combine.
     *
     * \return Returns the combined matrix.
     */
    M_PUBLIC_API Variant matrixCombineTopToBottom(const Variant& m1, const Variant& m2);

    /**
     * Function that calculates the determinant of a matrix.
     *
     * \param[in] matrix The matrix to calculate the determinant of.
     *
     * \return Returns the determinant of the matrix.
     */
    M_PUBLIC_API inline Integer abs(const MatrixBoolean& matrix) {
        return Integer(MatrixReal(matrix).determinant() + 0.5);
    }

    /**
     * Function that calculates the determinant of a matrix.
     *
     * \param[in] matrix The matrix to calculate the determinant of.
     *
     * \return Returns the determinant of the matrix.
     */
    M_PUBLIC_API inline Integer abs(const MatrixInteger& matrix) {
        return Integer(MatrixReal(matrix).determinant() + 0.5);
    }

    /**
     * Function that calculates the determinant of a matrix.
     *
     * \param[in] matrix The matrix to calculate the determinant of.
     *
     * \return Returns the determinant of the matrix.
     */
    M_PUBLIC_API inline Real abs(const MatrixReal& matrix) {
        return matrix.determinant();
    }

    /**
     * Function that calculates the determinant of a matrix.
     *
     * \param[in] matrix The matrix to calculate the determinant of.
     *
     * \return Returns the determinant of the matrix.
     */
    M_PUBLIC_API inline Complex abs(const MatrixComplex& matrix) {
        return matrix.determinant();
    }

    /**
     * Function that calculates the complex conjugate of the matrix.
     *
     * \param[in] matrix The matrix to calculate the complex conjugate of.
     *
     * \return Returns the complex conjugate of the supplied matrix.
     */
    M_PUBLIC_API inline const MatrixBoolean& conj(const MatrixBoolean& matrix) {
        return matrix;
    }

    /**
     * Function that calculates the complex conjugate of the matrix.
     *
     * \param[in] matrix The matrix to calculate the complex conjugate of.
     *
     * \return Returns the complex conjugate of the supplied matrix.
     */
    M_PUBLIC_API inline const MatrixInteger& conj(const MatrixInteger& matrix) {
        return matrix;
    }

    /**
     * Function that calculates the complex conjugate of the matrix.
     *
     * \param[in] matrix The matrix to calculate the complex conjugate of.
     *
     * \return Returns the complex conjugate of the supplied matrix.
     */
    M_PUBLIC_API inline const MatrixReal& conj(const MatrixReal& matrix) {
        return matrix;
    }

    /**
     * Function that calculates the complex conjugate of the matrix.
     *
     * \param[in] matrix The matrix to calculate the complex conjugate of.
     *
     * \return Returns the complex conjugate of the supplied matrix.
     */
    M_PUBLIC_API inline MatrixComplex conj(const MatrixComplex& matrix) {
        return matrix.conj();
    }

    /**
     * Function that calculates the Hadamard product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Hadamard product of the two matrices.
     */
    M_PUBLIC_API inline MatrixBoolean hadamard(const MatrixBoolean& m1, const MatrixBoolean& m2) {
        return MatrixBoolean(MatrixInteger(m1).hadamard(MatrixInteger(m2)));
    }

    /**
     * Function that calculates the Hadamard product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Hadamard product of the two matrices.
     */
    M_PUBLIC_API inline MatrixInteger hadamard(const MatrixBoolean& m1, const MatrixInteger& m2) {
        return MatrixInteger(m1).hadamard(m2);
    }

    /**
     * Function that calculates the Hadamard product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Hadamard product of the two matrices.
     */
    M_PUBLIC_API inline MatrixReal hadamard(const MatrixBoolean& m1, const MatrixReal& m2) {
        return MatrixReal(m1).hadamard(m2);
    }

    /**
     * Function that calculates the Hadamard product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Hadamard product of the two matrices.
     */
    M_PUBLIC_API inline MatrixComplex hadamard(const MatrixBoolean& m1, const MatrixComplex& m2) {
        return MatrixComplex(m1).hadamard(m2);
    }

    /**
     * Function that calculates the Hadamard product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Hadamard product of the two matrices.
     */
    M_PUBLIC_API inline MatrixInteger hadamard(const MatrixInteger& m1, const MatrixBoolean& m2) {
        return m1.hadamard(MatrixInteger(m2));
    }

    /**
     * Function that calculates the Hadamard product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Hadamard product of the two matrices.
     */
    M_PUBLIC_API inline MatrixInteger hadamard(const MatrixInteger& m1, const MatrixInteger& m2) {
        return m1.hadamard(m2);
    }

    /**
     * Function that calculates the Hadamard product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Hadamard product of the two matrices.
     */
    M_PUBLIC_API inline MatrixReal hadamard(const MatrixInteger& m1, const MatrixReal& m2) {
        return MatrixReal(m1).hadamard(m2);
    }

    /**
     * Function that calculates the Hadamard product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Hadamard product of the two matrices.
     */
    M_PUBLIC_API inline MatrixComplex hadamard(const MatrixInteger& m1, const MatrixComplex& m2) {
        return MatrixComplex(m1).hadamard(m2);
    }

    /**
     * Function that calculates the Hadamard product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Hadamard product of the two matrices.
     */
    M_PUBLIC_API inline MatrixReal hadamard(const MatrixReal& m1, const MatrixBoolean& m2) {
        return m1.hadamard(MatrixReal(m2));
    }

    /**
     * Function that calculates the Hadamard product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Hadamard product of the two matrices.
     */
    M_PUBLIC_API inline MatrixReal hadamard(const MatrixReal& m1, const MatrixInteger& m2) {
        return m1.hadamard(MatrixReal(m2));
    }

    /**
     * Function that calculates the Hadamard product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Hadamard product of the two matrices.
     */
    M_PUBLIC_API inline MatrixReal hadamard(const MatrixReal& m1, const MatrixReal& m2) {
        return m1.hadamard(m2);
    }

    /**
     * Function that calculates the Hadamard product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Hadamard product of the two matrices.
     */
    M_PUBLIC_API inline MatrixComplex hadamard(const MatrixReal& m1, const MatrixComplex& m2) {
        return MatrixComplex(m1).hadamard(m2);
    }

    /**
     * Function that calculates the Hadamard product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Hadamard product of the two matrices.
     */
    M_PUBLIC_API inline MatrixComplex hadamard(const MatrixComplex& m1, const MatrixBoolean& m2) {
        return m1.hadamard(MatrixComplex(m2));
    }

    /**
     * Function that calculates the Hadamard product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Hadamard product of the two matrices.
     */
    M_PUBLIC_API inline MatrixComplex hadamard(const MatrixComplex& m1, const MatrixInteger& m2) {
        return m1.hadamard(MatrixComplex(m2));
    }

    /**
     * Function that calculates the Hadamard product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Hadamard product of the two matrices.
     */
    M_PUBLIC_API inline MatrixComplex hadamard(const MatrixComplex& m1, const MatrixReal& m2) {
        return m1.hadamard(MatrixComplex(m2));
    }

    /**
     * Function that calculates the Hadamard product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Hadamard product of the two matrices.
     */
    M_PUBLIC_API inline MatrixComplex hadamard(const MatrixComplex& m1, const MatrixComplex& m2) {
        return m1.hadamard(m2);
    }

    /**
     * Function that calculates the Hadamard product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Hadamard product of the two matrices.
     */
    M_PUBLIC_API Variant hadamard(const Variant& m1, const MatrixBoolean& m2);

    /**
     * Function that calculates the Hadamard product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Hadamard product of the two matrices.
     */
    M_PUBLIC_API Variant hadamard(const Variant& m1, const MatrixInteger& m2);

    /**
     * Function that calculates the Hadamard product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Hadamard product of the two matrices.
     */
    M_PUBLIC_API Variant hadamard(const Variant& m1, const MatrixReal& m2);

    /**
     * Function that calculates the Hadamard product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Hadamard product of the two matrices.
     */
    M_PUBLIC_API MatrixComplex hadamard(const Variant& m1, const MatrixComplex& m2);

    /**
     * Function that calculates the Hadamard product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Hadamard product of the two matrices.
     */
    M_PUBLIC_API Variant hadamard(const MatrixBoolean& m1, const Variant& m2);

    /**
     * Function that calculates the Hadamard product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Hadamard product of the two matrices.
     */
    M_PUBLIC_API Variant hadamard(const MatrixInteger& m1, const Variant& m2);

    /**
     * Function that calculates the Hadamard product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Hadamard product of the two matrices.
     */
    M_PUBLIC_API Variant hadamard(const MatrixReal& m1, const Variant& m2);

    /**
     * Function that calculates the Hadamard product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Hadamard product of the two matrices.
     */
    M_PUBLIC_API MatrixComplex hadamard(const MatrixComplex& m1, const Variant& m2);

    /**
     * Function that calculates the Hadamard product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Hadamard product of the two matrices.
     */
    M_PUBLIC_API Variant hadamard(const Variant& m1, const Variant& m2);

    /**
     * Function that calculates the Kronecker product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Kronecker product of the two matrices.
     */
    M_PUBLIC_API inline MatrixBoolean kronecker(const MatrixBoolean& m1, const MatrixBoolean& m2) {
        return MatrixBoolean(MatrixInteger(m1).kronecker(MatrixInteger(m2)));
    }

    /**
     * Function that calculates the Kronecker product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Kronecker product of the two matrices.
     */
    M_PUBLIC_API inline MatrixInteger kronecker(const MatrixBoolean& m1, const MatrixInteger& m2) {
        return MatrixInteger(m1).kronecker(m2);
    }

    /**
     * Function that calculates the Kronecker product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Kronecker product of the two matrices.
     */
    M_PUBLIC_API inline MatrixReal kronecker(const MatrixBoolean& m1, const MatrixReal& m2) {
        return MatrixReal(m1).kronecker(m2);
    }

    /**
     * Function that calculates the Kronecker product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Kronecker product of the two matrices.
     */
    M_PUBLIC_API inline MatrixComplex kronecker(const MatrixBoolean& m1, const MatrixComplex& m2) {
        return MatrixComplex(m1).kronecker(m2);
    }

    /**
     * Function that calculates the Kronecker product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Kronecker product of the two matrices.
     */
    M_PUBLIC_API inline MatrixInteger kronecker(const MatrixInteger& m1, const MatrixBoolean& m2) {
        return m1.kronecker(MatrixInteger(m2));
    }

    /**
     * Function that calculates the Kronecker product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Kronecker product of the two matrices.
     */
    M_PUBLIC_API inline MatrixInteger kronecker(const MatrixInteger& m1, const MatrixInteger& m2) {
        return m1.kronecker(m2);
    }

    /**
     * Function that calculates the Kronecker product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Kronecker product of the two matrices.
     */
    M_PUBLIC_API inline MatrixReal kronecker(const MatrixInteger& m1, const MatrixReal& m2) {
        return MatrixReal(m1).kronecker(m2);
    }

    /**
     * Function that calculates the Kronecker product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Kronecker product of the two matrices.
     */
    M_PUBLIC_API inline MatrixComplex kronecker(const MatrixInteger& m1, const MatrixComplex& m2) {
        return MatrixComplex(m1).kronecker(m2);
    }

    /**
     * Function that calculates the Kronecker product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Kronecker product of the two matrices.
     */
    M_PUBLIC_API inline MatrixReal kronecker(const MatrixReal& m1, const MatrixBoolean& m2) {
        return m1.kronecker(MatrixReal(m2));
    }

    /**
     * Function that calculates the Kronecker product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Kronecker product of the two matrices.
     */
    M_PUBLIC_API inline MatrixReal kronecker(const MatrixReal& m1, const MatrixInteger& m2) {
        return m1.kronecker(MatrixReal(m2));
    }

    /**
     * Function that calculates the Kronecker product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Kronecker product of the two matrices.
     */
    M_PUBLIC_API inline MatrixReal kronecker(const MatrixReal& m1, const MatrixReal& m2) {
        return m1.kronecker(m2);
    }

    /**
     * Function that calculates the Kronecker product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Kronecker product of the two matrices.
     */
    M_PUBLIC_API inline MatrixComplex kronecker(const MatrixReal& m1, const MatrixComplex& m2) {
        return MatrixComplex(m1).kronecker(m2);
    }

    /**
     * Function that calculates the Kronecker product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Kronecker product of the two matrices.
     */
    M_PUBLIC_API inline MatrixComplex kronecker(const MatrixComplex& m1, const MatrixBoolean& m2) {
        return m1.kronecker(MatrixComplex(m2));
    }

    /**
     * Function that calculates the Kronecker product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Kronecker product of the two matrices.
     */
    M_PUBLIC_API inline MatrixComplex kronecker(const MatrixComplex& m1, const MatrixInteger& m2) {
        return m1.kronecker(MatrixComplex(m2));
    }

    /**
     * Function that calculates the Kronecker product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Kronecker product of the two matrices.
     */
    M_PUBLIC_API inline MatrixComplex kronecker(const MatrixComplex& m1, const MatrixReal& m2) {
        return m1.kronecker(MatrixComplex(m2));
    }

    /**
     * Function that calculates the Kronecker product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Kronecker product of the two matrices.
     */
    M_PUBLIC_API inline MatrixComplex kronecker(const MatrixComplex& m1, const MatrixComplex& m2) {
        return m1.kronecker(m2);
    }

    /**
     * Function that calculates the Kronecker product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Kronecker product of the two matrices.
     */
    M_PUBLIC_API Variant kronecker(const Variant& m1, const MatrixBoolean& m2);

    /**
     * Function that calculates the Kronecker product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Kronecker product of the two matrices.
     */
    M_PUBLIC_API Variant kronecker(const Variant& m1, const MatrixInteger& m2);

    /**
     * Function that calculates the Kronecker product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Kronecker product of the two matrices.
     */
    M_PUBLIC_API Variant kronecker(const Variant& m1, const MatrixReal& m2);

    /**
     * Function that calculates the Kronecker product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Kronecker product of the two matrices.
     */
    M_PUBLIC_API MatrixComplex kronecker(const Variant& m1, const MatrixComplex& m2);

    /**
     * Function that calculates the Kronecker product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Kronecker product of the two matrices.
     */
    M_PUBLIC_API Variant kronecker(const MatrixBoolean& m1, const Variant& m2);

    /**
     * Function that calculates the Kronecker product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Kronecker product of the two matrices.
     */
    M_PUBLIC_API Variant kronecker(const MatrixInteger& m1, const Variant& m2);

    /**
     * Function that calculates the Kronecker product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Kronecker product of the two matrices.
     */
    M_PUBLIC_API Variant kronecker(const MatrixReal& m1, const Variant& m2);

    /**
     * Function that calculates the Kronecker product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Kronecker product of the two matrices.
     */
    M_PUBLIC_API MatrixComplex kronecker(const MatrixComplex& m1, const Variant& m2);

    /**
     * Function that calculates the Kronecker product of two matrices.
     *
     * \param[in] m1 The first matrix.
     *
     * \param[in] m2 The second matrix.
     *
     * \return Returns the Kronecker product of the two matrices.
     */
    M_PUBLIC_API Variant kronecker(const Variant& m1, const Variant& m2);

    /**
     * Function that calculates the transpose of a matrix.
     *
     * \param[in] matrix The matrix to calculate the transpose of.
     *
     * \return Returns the transpose of the matrix.
     */
    M_PUBLIC_API inline MatrixBoolean transpose(const MatrixBoolean& matrix) {
        return matrix.transpose();
    }

    /**
     * Function that calculates the transpose of a matrix.
     *
     * \param[in] matrix The matrix to calculate the transpose of.
     *
     * \return Returns the transpose of the matrix.
     */
    M_PUBLIC_API inline MatrixInteger transpose(const MatrixInteger& matrix) {
        return matrix.transpose();
    }

    /**
     * Function that calculates the transpose of a matrix.
     *
     * \param[in] matrix The matrix to calculate the transpose of.
     *
     * \return Returns the transpose of the matrix.
     */
    M_PUBLIC_API inline MatrixReal transpose(const MatrixReal& matrix) {
        return matrix.transpose();
    }

    /**
     * Function that calculates the transpose of a matrix.
     *
     * \param[in] matrix The matrix to calculate the transpose of.
     *
     * \return Returns the transpose of the matrix.
     */
    M_PUBLIC_API inline MatrixComplex transpose(const MatrixComplex& matrix) {
        return matrix.transpose();
    }

    /**
     * Function that calculates the transpose of a matrix.
     *
     * \param[in] matrix The matrix to calculate the transpose of.
     *
     * \return Returns the transpose of the matrix.
     */
    M_PUBLIC_API Variant transpose(const Variant& matrix);

    /**
     * Function that calculates the hermitian of a matrix.
     *
     * \param[in] matrix The matrix to calculate the hermitian of.
     *
     * \return Returns the hermitian of the matrix.
     */
    M_PUBLIC_API inline MatrixBoolean hermitian(const MatrixBoolean& matrix) {
        return matrix.adjoint();
    }

    /**
     * Function that calculates the hermitian of a matrix.
     *
     * \param[in] matrix The matrix to calculate the hermitian of.
     *
     * \return Returns the hermitian of the matrix.
     */
    M_PUBLIC_API inline MatrixInteger hermitian(const MatrixInteger& matrix) {
        return matrix.adjoint();
    }

    /**
     * Function that calculates the hermitian of a matrix.
     *
     * \param[in] matrix The matrix to calculate the hermitian of.
     *
     * \return Returns the hermitian of the matrix.
     */
    M_PUBLIC_API inline MatrixReal hermitian(const MatrixReal& matrix) {
        return matrix.adjoint();
    }

    /**
     * Function that calculates the hermitian of a matrix.
     *
     * \param[in] matrix The matrix to calculate the hermitian of.
     *
     * \return Returns the hermitian of the matrix.
     */
    M_PUBLIC_API inline MatrixComplex hermitian(const MatrixComplex& matrix) {
        return matrix.adjoint();
    }

    /**
     * Function that calculates the hermitian of a matrix.
     *
     * \param[in] matrix The matrix to calculate the hermitian of.
     *
     * \return Returns the hermitian of the matrix.
     */
    M_PUBLIC_API Variant hermitian(const Variant& matrix) ;

    /**
     * Function that calculates the matrix inverse.
     *
     * \param[in] matrix The matrix to calculate the inverse of.
     *
     * \return Returns the inverse matrix.
     */
    M_PUBLIC_API inline MatrixReal inverse(const MatrixBoolean& matrix) {
        return MatrixReal(matrix).inverse();
    }

    /**
     * Function that calculates the matrix inverse.
     *
     * \param[in] matrix The matrix to calculate the inverse of.
     *
     * \return Returns the inverse matrix.
     */
    M_PUBLIC_API inline MatrixReal inverse(const MatrixInteger& matrix) {
        return MatrixReal(matrix).inverse();
    }

    /**
     * Function that calculates the matrix inverse.
     *
     * \param[in] matrix The matrix to calculate the inverse of.
     *
     * \return Returns the inverse matrix.
     */
    M_PUBLIC_API inline MatrixReal inverse(const MatrixReal& matrix) {
        return matrix.inverse();
    }

    /**
     * Function that calculates the matrix inverse.
     *
     * \param[in] matrix The matrix to calculate the inverse of.
     *
     * \return Returns the inverse matrix.
     */
    M_PUBLIC_API inline MatrixComplex inverse(const MatrixComplex& matrix) {
        return matrix.inverse();
    }

    /**
     * Function that calculates the matrix inverse.
     *
     * \param[in] matrix The matrix to calculate the inverse of.
     *
     * \return Returns the inverse matrix.
     */
    M_PUBLIC_API Variant inverse(const Variant& matrix);

    /**
     * Method you can use to reverse the columns in this matrix.
     *
     * \param[in] matrix The matrix to be reversed.
     *
     * \return Returns a column-reversed version of this matrix.
     */
    M_PUBLIC_API inline MatrixBoolean columnReverse(const MatrixBoolean& matrix) {
        return matrix.columnReverse();
    }

    /**
     * Method you can use to reverse the columns in this matrix.
     *
     * \param[in] matrix The matrix to be reversed.
     *
     * \return Returns a column-reversed version of this matrix.
     */
    M_PUBLIC_API inline MatrixInteger columnReverse(const MatrixInteger& matrix) {
        return matrix.columnReverse();
    }

    /**
     * Method you can use to reverse the columns in this matrix.
     *
     * \param[in] matrix The matrix to be reversed.
     *
     * \return Returns a column-reversed version of this matrix.
     */
    M_PUBLIC_API inline MatrixReal columnReverse(const MatrixReal& matrix) {
        return matrix.columnReverse();
    }

    /**
     * Method you can use to reverse the columns in this matrix.
     *
     * \param[in] matrix The matrix to be reversed.
     *
     * \return Returns a column-reversed version of this matrix.
     */
    M_PUBLIC_API inline MatrixComplex columnReverse(const MatrixComplex& matrix) {
        return matrix.columnReverse();
    }

    /**
     * Method you can use to reverse the columns in this matrix.
     *
     * \param[in] matrix The matrix to be reversed.
     *
     * \return Returns a column-reversed version of this matrix.
     */
    M_PUBLIC_API Variant columnReverse(const Variant& matrix);

    /**
     * Method you can use to reverse the rows in this matrix.
     *
     * \param[in] matrix The matrix to be reversed.
     *
     * \return Returns a row-reversed version of this matrix.
     */
    M_PUBLIC_API inline MatrixBoolean rowReverse(const MatrixBoolean& matrix) {
        return matrix.rowReverse();
    }

    /**
     * Method you can use to reverse the rows in this matrix.
     *
     * \param[in] matrix The matrix to be reversed.
     *
     * \return Returns a row-reversed version of this matrix.
     */
    M_PUBLIC_API inline MatrixInteger rowReverse(const MatrixInteger& matrix) {
        return matrix.rowReverse();
    }

    /**
     * Method you can use to reverse the rows in this matrix.
     *
     * \param[in] matrix The matrix to be reversed.
     *
     * \return Returns a row-reversed version of this matrix.
     */
    M_PUBLIC_API inline MatrixReal rowReverse(const MatrixReal& matrix) {
        return matrix.rowReverse();
    }

    /**
     * Method you can use to reverse the rows in this matrix.
     *
     * \param[in] matrix The matrix to be reversed.
     *
     * \return Returns a row-reversed version of this matrix.
     */
    M_PUBLIC_API inline MatrixComplex rowReverse(const MatrixComplex& matrix) {
        return matrix.rowReverse();
    }

    /**
     * Method you can use to reverse the rows in this matrix.
     *
     * \param[in] matrix The matrix to be reversed.
     *
     * \return Returns a row-reversed version of this matrix.
     */
    M_PUBLIC_API Variant rowReverse(const Variant& matrix);

    /**
     * Method you can use to determine if this matrix is square.  That is, whether the matrix has the same
     * number of rows as columns.
     *
     * \param[in] matrix The matrix to be tested.
     *
     * \return Returns true if this matrix is square.  Returns false if this matrix is not square.
     */
    M_PUBLIC_API inline bool isSquare(const MatrixBoolean& matrix) {
        return matrix.isSquare();
    }

    /**
     * Method you can use to determine if this matrix is square.  That is, whether the matrix has the same
     * number of rows as columns.
     *
     * \param[in] matrix The matrix to be tested.
     *
     * \return Returns true if this matrix is square.  Returns false if this matrix is not square.
     */
    M_PUBLIC_API inline bool isSquare(const MatrixInteger& matrix) {
        return matrix.isSquare();
    }

    /**
     * Method you can use to determine if this matrix is square.  That is, whether the matrix has the same
     * number of rows as columns.
     *
     * \param[in] matrix The matrix to be tested.
     *
     * \return Returns true if this matrix is square.  Returns false if this matrix is not square.
     */
    M_PUBLIC_API inline bool isSquare(const MatrixReal& matrix) {
        return matrix.isSquare();
    }

    /**
     * Method you can use to determine if this matrix is square.  That is, whether the matrix has the same
     * number of rows as columns.
     *
     * \param[in] matrix The matrix to be tested.
     *
     * \return Returns true if this matrix is square.  Returns false if this matrix is not square.
     */
    M_PUBLIC_API inline bool isSquare(const MatrixComplex& matrix) {
        return matrix.isSquare();
    }

    /**
     * Method you can use to determine if this matrix is square.  That is, whether the matrix has the same
     * number of rows as columns.
     *
     * \param[in] matrix The matrix to be tested.
     *
     * \return Returns true if this matrix is square.  Returns false if this matrix is not square.
     */
    M_PUBLIC_API bool isSquare(const Variant& matrix);

    /**
     * Method you can use to determine if this matrix is symmetric.  That is, whether the matrix is equal to
     * it's own transpose.
     *
     * \param[in] matrix            The matrix to be tested.
     *
     * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
     *                              the magnitude of the difference in values, divided by average of the two
     *                              values exceeds this threshold.
     *
     * \return Returns true if this matrix is symmetric.  Returns false if this matrix is not symmetric.
     */
    M_PUBLIC_API inline bool isSymmetric(
            const MatrixBoolean& matrix,
            Real                 relativeTolerance = MatrixReal::defaultRelativeTolerance
        ) {
        return MatrixReal(matrix).isSymmetric(relativeTolerance);
    }

    /**
     * Method you can use to determine if this matrix is symmetric.  That is, whether the matrix is equal to
     * it's own transpose.
     *
     * \param[in] matrix            The matrix to be tested.
     *
     * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
     *                              the magnitude of the difference in values, divided by average of the two
     *                              values exceeds this threshold.
     *
     * \return Returns true if this matrix is symmetric.  Returns false if this matrix is not symmetric.
     */
    M_PUBLIC_API inline bool isSymmetric(
            const MatrixInteger& matrix,
            Real                 relativeTolerance = MatrixReal::defaultRelativeTolerance
        ) {
        return MatrixReal(matrix).isSymmetric(relativeTolerance);
    }

    /**
     * Method you can use to determine if this matrix is symmetric.  That is, whether the matrix is equal to
     * it's own transpose.
     *
     * \param[in] matrix            The matrix to be tested.
     *
     * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
     *                              the magnitude of the difference in values, divided by average of the two
     *                              values exceeds this threshold.
     *
     * \return Returns true if this matrix is symmetric.  Returns false if this matrix is not symmetric.
     */
    M_PUBLIC_API inline bool isSymmetric(
            const MatrixReal& matrix,
            Real              relativeTolerance = MatrixReal::defaultRelativeTolerance
        ) {
        return matrix.isSymmetric(relativeTolerance);
    }

    /**
     * Method you can use to determine if this matrix is symmetric.  That is, whether the matrix is equal to
     * it's own transpose.
     *
     * \param[in] matrix            The matrix to be tested.
     *
     * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
     *                              the magnitude of the difference in values, divided by average of the two
     *                              values exceeds this threshold.
     *
     * \return Returns true if this matrix is symmetric.  Returns false if this matrix is not symmetric.
     */
    M_PUBLIC_API inline bool isSymmetric(
            const MatrixComplex& matrix,
            Real                 relativeTolerance = MatrixComplex::defaultRelativeTolerance
        ) {
        return matrix.isSymmetric(relativeTolerance);
    }

    /**
     * Method you can use to determine if this matrix is symmetric.  That is, whether the matrix is equal to
     * it's own transpose.
     *
     * \param[in] matrix            The matrix to be tested.
     *
     * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
     *                              the magnitude of the difference in values, divided by average of the two
     *                              values exceeds this threshold.
     *
     * \return Returns true if this matrix is symmetric.  Returns false if this matrix is not symmetric.
     */
    M_PUBLIC_API bool isSymmetric(
        const Variant& matrix,
        Real           relativeTolerance = MatrixComplex::defaultRelativeTolerance
    );

    /**
     * Method you can use to determine if this matrix is Hermitian.  That is, whether the matrix is equal to
     * it's own conjugate transpose.
     *
     * \param[in] matrix            The matrix to be tested.
     *
     * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
     *                              the magnitude of the difference in values, divided by average of the two
     *                              values exceeds this threshold.
     *
     * \return Returns true if this matrix is Hermitian.  Returns false if this matrix is not Hermitian.
     */
    M_PUBLIC_API inline bool isHermitian(
            const MatrixBoolean& matrix,
            Real                 relativeTolerance = MatrixReal::defaultRelativeTolerance
        ) {
        return MatrixReal(matrix).isHermitian(relativeTolerance);
    }

    /**
     * Method you can use to determine if this matrix is Hermitian.  That is, whether the matrix is equal to
     * it's own conjugate transpose.
     *
     * \param[in] matrix            The matrix to be tested.
     *
     * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
     *                              the magnitude of the difference in values, divided by average of the two
     *                              values exceeds this threshold.
     *
     * \return Returns true if this matrix is Hermitian.  Returns false if this matrix is not Hermitian.
     */
    M_PUBLIC_API inline bool isHermitian(
            const MatrixInteger& matrix,
            Real                 relativeTolerance = MatrixReal::defaultRelativeTolerance
        ) {
        return MatrixReal(matrix).isHermitian(relativeTolerance);
    }

    /**
     * Method you can use to determine if this matrix is Hermitian.  That is, whether the matrix is equal to
     * it's own conjugate transpose.
     *
     * \param[in] matrix            The matrix to be tested.
     *
     * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
     *                              the magnitude of the difference in values, divided by average of the two
     *                              values exceeds this threshold.
     *
     * \return Returns true if this matrix is Hermitian.  Returns false if this matrix is not Hermitian.
     */
    M_PUBLIC_API inline bool isHermitian(
            const MatrixReal& matrix,
            Real              relativeTolerance = MatrixReal::defaultRelativeTolerance
        ) {
        return matrix.isHermitian(relativeTolerance);
    }

    /**
     * Method you can use to determine if this matrix is Hermitian.  That is, whether the matrix is equal to
     * it's own conjugate transpose.
     *
     * \param[in] matrix            The matrix to be tested.
     *
     * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
     *                              the magnitude of the difference in values, divided by average of the two
     *                              values exceeds this threshold.
     *
     * \return Returns true if this matrix is Hermitian.  Returns false if this matrix is not Hermitian.
     */
    M_PUBLIC_API inline bool isHermitian(
            const MatrixComplex& matrix,
            Real                 relativeTolerance = MatrixComplex::defaultRelativeTolerance
        ) {
        return matrix.isHermitian(relativeTolerance);
    }

    /**
     * Method you can use to determine if this matrix is Hermitian.  That is, whether the matrix is equal to
     * it's own conjugate transpose.
     *
     * \param[in] matrix            The matrix to be tested.
     *
     * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
     *                              the magnitude of the difference in values, divided by average of the two
     *                              values exceeds this threshold.
     *
     * \return Returns true if this matrix is Hermitian.  Returns false if this matrix is not Hermitian.
     */
    M_PUBLIC_API bool isHermitian(
        const Variant& matrix,
        Real           relativeTolerance = MatrixComplex::defaultRelativeTolerance
    );

    /**
     * Method you can use to determine if this matrix is skew symmetric.  That is, whether the matrix is equal
     * to the negative of its own transpose.
     *
     * \param[in] matrix            The matrix to be tested.
     *
     * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
     *                              the magnitude of the difference in values, divided by average of the two
     *                              values exceeds this threshold.
     *
     * \return Returns true if this matrix is skew symmetric.  Returns false if this matrix is not skew
     *         symmetric.
     */
    M_PUBLIC_API inline bool isSkewSymmetric(
            const MatrixBoolean& matrix,
            Real                 relativeTolerance = MatrixReal::defaultRelativeTolerance
        ) {
        return MatrixReal(matrix).isSkewSymmetric(relativeTolerance);
    }

    /**
     * Method you can use to determine if this matrix is skew symmetric.  That is, whether the matrix is equal
     * to the negative of its own transpose.
     *
     * \param[in] matrix            The matrix to be tested.
     *
     * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
     *                              the magnitude of the difference in values, divided by average of the two
     *                              values exceeds this threshold.
     *
     * \return Returns true if this matrix is skew symmetric.  Returns false if this matrix is not skew
     *         symmetric.
     */
    M_PUBLIC_API inline bool isSkewSymmetric(
            const MatrixInteger& matrix,
            Real                 relativeTolerance = MatrixReal::defaultRelativeTolerance
        ) {
        return MatrixReal(matrix).isSkewSymmetric(relativeTolerance);
    }

    /**
     * Method you can use to determine if this matrix is skew symmetric.  That is, whether the matrix is equal
     * to the negative of its own transpose.
     *
     * \param[in] matrix            The matrix to be tested.
     *
     * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
     *                              the magnitude of the difference in values, divided by average of the two
     *                              values exceeds this threshold.
     *
     * \return Returns true if this matrix is skew symmetric.  Returns false if this matrix is not skew
     *         symmetric.
     */
    M_PUBLIC_API inline bool isSkewSymmetric(
            const MatrixReal& matrix,
            Real              relativeTolerance = MatrixReal::defaultRelativeTolerance
        ) {
        return matrix.isSkewSymmetric(relativeTolerance);
    }

    /**
     * Method you can use to determine if this matrix is skew symmetric.  That is, whether the matrix is equal
     * to the negative of its own transpose.
     *
     * \param[in] matrix            The matrix to be tested.
     *
     * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
     *                              the magnitude of the difference in values, divided by average of the two
     *                              values exceeds this threshold.
     *
     * \return Returns true if this matrix is skew symmetric.  Returns false if this matrix is not skew
     *         symmetric.
     */
    M_PUBLIC_API inline bool isSkewSymmetric(
            const MatrixComplex& matrix,
            Real                 relativeTolerance = MatrixComplex::defaultRelativeTolerance
        ) {
        return matrix.isSkewSymmetric(relativeTolerance);
    }

    /**
     * Method you can use to determine if this matrix is skew symmetric.  That is, whether the matrix is equal
     * to the negative of its own transpose.
     *
     * \param[in] matrix            The matrix to be tested.
     *
     * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
     *                              the magnitude of the difference in values, divided by average of the two
     *                              values exceeds this threshold.
     *
     * \return Returns true if this matrix is skew symmetric.  Returns false if this matrix is not skew
     *         symmetric.
     */
    M_PUBLIC_API bool isSkewSymmetric(
        const Variant& matrix,
        Real           relativeTolerance = MatrixComplex::defaultRelativeTolerance
    );

    /**
     * Method you can use to determine if this matrix is skew Hermitian.  That is, whether the matrix is equal
     * to the negative of it's own conjugate transpose.
     *
     * \param[in] matrix            The matrix to be tested.
     *
     * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
     *                              the magnitude of the difference in values, divided by average of the two
     *                              values exceeds this threshold.
     *
     * \return Returns true if this matrix is shew Hermitian.  Returns false if this matrix is not skew
     *         Hermitian.
     */
    M_PUBLIC_API inline bool isSkewHermitian(
            const MatrixBoolean& matrix,
            Real                 relativeTolerance = MatrixReal::defaultRelativeTolerance
        ) {
        return MatrixReal(matrix).isSkewHermitian(relativeTolerance);
    }

    /**
     * Method you can use to determine if this matrix is skew Hermitian.  That is, whether the matrix is equal
     * to the negative of it's own conjugate transpose.
     *
     * \param[in] matrix            The matrix to be tested.
     *
     * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
     *                              the magnitude of the difference in values, divided by average of the two
     *                              values exceeds this threshold.
     *
     * \return Returns true if this matrix is shew Hermitian.  Returns false if this matrix is not skew
     *         Hermitian.
     */
    M_PUBLIC_API inline bool isSkewHermitian(
            const MatrixInteger& matrix,
            Real                 relativeTolerance = MatrixReal::defaultRelativeTolerance
        ) {
        return MatrixReal(matrix).isSkewHermitian(relativeTolerance);
    }

    /**
     * Method you can use to determine if this matrix is skew Hermitian.  That is, whether the matrix is equal
     * to the negative of it's own conjugate transpose.
     *
     * \param[in] matrix            The matrix to be tested.
     *
     * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
     *                              the magnitude of the difference in values, divided by average of the two
     *                              values exceeds this threshold.
     *
     * \return Returns true if this matrix is shew Hermitian.  Returns false if this matrix is not skew
     *         Hermitian.
     */
    M_PUBLIC_API inline bool isSkewHermitian(
            const MatrixReal& matrix,
            Real              relativeTolerance = MatrixReal::defaultRelativeTolerance
        ) {
        return matrix.isSkewHermitian(relativeTolerance);
    }

    /**
     * Method you can use to determine if this matrix is skew Hermitian.  That is, whether the matrix is equal
     * to the negative of it's own conjugate transpose.
     *
     * \param[in] matrix            The matrix to be tested.
     *
     * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
     *                              the magnitude of the difference in values, divided by average of the two
     *                              values exceeds this threshold.
     *
     * \return Returns true if this matrix is shew Hermitian.  Returns false if this matrix is not skew
     *         Hermitian.
     */
    M_PUBLIC_API inline bool isSkewHermitian(
            const MatrixComplex& matrix,
            Real                 relativeTolerance = MatrixComplex::defaultRelativeTolerance
        ) {
        return matrix.isSkewHermitian(relativeTolerance);
    }

    /**
     * Method you can use to determine if this matrix is skew Hermitian.  That is, whether the matrix is equal
     * to the negative of it's own conjugate transpose.
     *
     * \param[in] matrix            The matrix to be tested.
     *
     * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
     *                              the magnitude of the difference in values, divided by average of the two
     *                              values exceeds this threshold.
     *
     * \return Returns true if this matrix is shew Hermitian.  Returns false if this matrix is not skew
     *         Hermitian.
     */
    M_PUBLIC_API bool isSkewHermitian(
        const Variant& matrix,
        Real           relativeTolerance = MatrixComplex::defaultRelativeTolerance
    );

    /**
     * Method you can use to determine if this matrix is normal.  That is, the product of the matrix and its
     * conjugate transpose is equal to the product of the conjugate transpsoe of the matrix and the matrix.
     *
     * \param[in] matrix            The matrix to be tested.
     *
     * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
     *                              the magnitude of the difference in values, divided by average of the two
     *                              values exceeds this threshold.
     *
     * \return Returns true if the matrix is normal.  Returns false if the matrix is not normal.
     */
    M_PUBLIC_API inline bool isNormal(
            const MatrixBoolean& matrix,
            Real                 relativeTolerance = MatrixReal::defaultRelativeTolerance
        ) {
        return MatrixReal(matrix).isNormal(relativeTolerance);
    }

    /**
     * Method you can use to determine if this matrix is normal.  That is, the product of the matrix and its
     * conjugate transpose is equal to the product of the conjugate transpsoe of the matrix and the matrix.
     *
     * \param[in] matrix            The matrix to be tested.
     *
     * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
     *                              the magnitude of the difference in values, divided by average of the two
     *                              values exceeds this threshold.
     *
     * \return Returns true if the matrix is normal.  Returns false if the matrix is not normal.
     */
    M_PUBLIC_API inline bool isNormal(
            const MatrixInteger& matrix,
            Real                 relativeTolerance = MatrixReal::defaultRelativeTolerance
        ) {
        return MatrixReal(matrix).isNormal(relativeTolerance);
    }

    /**
     * Method you can use to determine if this matrix is normal.  That is, the product of the matrix and its
     * conjugate transpose is equal to the product of the conjugate transpsoe of the matrix and the matrix.
     *
     * \param[in] matrix            The matrix to be tested.
     *
     * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
     *                              the magnitude of the difference in values, divided by average of the two
     *                              values exceeds this threshold.
     *
     * \return Returns true if the matrix is normal.  Returns false if the matrix is not normal.
     */
    M_PUBLIC_API inline bool isNormal(
            const MatrixReal& matrix,
            Real              relativeTolerance = MatrixReal::defaultRelativeTolerance
        ) {
        return matrix.isNormal(relativeTolerance);
    }

    /**
     * Method you can use to determine if this matrix is normal.  That is, the product of the matrix and its
     * conjugate transpose is equal to the product of the conjugate transpsoe of the matrix and the matrix.
     *
     * \param[in] matrix            The matrix to be tested.
     *
     * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
     *                              the magnitude of the difference in values, divided by average of the two
     *                              values exceeds this threshold.
     *
     * \return Returns true if the matrix is normal.  Returns false if the matrix is not normal.
     */
    M_PUBLIC_API inline bool isNormal(
            const MatrixComplex& matrix,
            Real                 relativeTolerance = MatrixComplex::defaultRelativeTolerance
        ) {
        return matrix.isNormal(relativeTolerance);
    }

    /**
     * Method you can use to determine if this matrix is normal.  That is, the product of the matrix and its
     * conjugate transpose is equal to the product of the conjugate transpsoe of the matrix and the matrix.
     *
     * \param[in] matrix            The matrix to be tested.
     *
     * \param[in] relativeTolerance The relative tolerance to allow for comparison.  The test will fail if
     *                              the magnitude of the difference in values, divided by average of the two
     *                              values exceeds this threshold.
     *
     * \return Returns true if the matrix is normal.  Returns false if the matrix is not normal.
     */
    M_PUBLIC_API bool isNormal(
        const Variant& matrix,
        Real           relativeTolerance = MatrixComplex::defaultRelativeTolerance
    );

    /**
     * Method you can use to determine the rank of the matrix.  This method operates using SVD decomposition of
     * the matrix.
     *
     * \param[in] matrix  The matrix to be tested.
     *
     * \param[in] epsilon An epsilon value you can use as a threshold for a zero value.  The absolute value of
     *                    each diagonal value will be compared against this value.  Values equal to or less
     *                    than this value will be treated as zero.
     *
     * \return Returns the estimated matrix rank.
     */
    M_PUBLIC_API inline Integer rank(const MatrixBoolean& matrix, Real epsilon) {
        return MatrixReal(matrix).rank(epsilon);
    }

    /**
     * Method you can use to determine the rank of the matrix.  This method operates using SVD decomposition of
     * the matrix.
     *
     * \param[in] matrix  The matrix to be tested.
     *
     * \param[in] epsilon An epsilon value you can use as a threshold for a zero value.  The absolute value of
     *                    each diagonal value will be compared against this value.  Values equal to or less
     *                    than this value will be treated as zero.
     *
     * \return Returns the estimated matrix rank.
     */
    M_PUBLIC_API inline Integer rank(const MatrixInteger& matrix, Real epsilon) {
        return MatrixReal(matrix).rank(epsilon);
    }

    /**
     * Method you can use to determine the rank of the matrix.  This method operates using SVD decomposition of
     * the matrix.
     *
     * \param[in] matrix  The matrix to be tested.
     *
     * \param[in] epsilon An epsilon value you can use as a threshold for a zero value.  The absolute value of
     *                    each diagonal value will be compared against this value.  Values equal to or less
     *                    than this value will be treated as zero.
     *
     * \return Returns the estimated matrix rank.
     */
    M_PUBLIC_API inline Integer rank(const MatrixReal& matrix, Real epsilon) {
        return matrix.rank(epsilon);
    }

    /**
     * Method you can use to determine the rank of the matrix.  This method operates using SVD decomposition of
     * the matrix.
     *
     * \param[in] matrix  The matrix to be tested.
     *
     * \param[in] epsilon An epsilon value you can use as a threshold for a zero value.  The absolute value of
     *                    each diagonal value will be compared against this value.  Values equal to or less
     *                    than this value will be treated as zero.
     *
     * \return Returns the estimated matrix rank.
     */
    M_PUBLIC_API inline Integer rank(const MatrixComplex& matrix, Real epsilon) {
        return matrix.rank(epsilon);
    }

    /**
     * Method you can use to determine the rank of the matrix.  This method operates using SVD decomposition of
     * the matrix.
     *
     * \param[in] matrix  The matrix to be tested.
     *
     * \param[in] epsilon An epsilon value you can use as a threshold for a zero value.  The absolute value of
     *                    each diagonal value will be compared against this value.  Values equal to or less
     *                    than this value will be treated as zero.
     *
     * \return Returns the estimated matrix rank.
     */
    M_PUBLIC_API Integer rank(const Variant& matrix, Real epsilon);

    /**
     * Method that returns the determinant of this matrix.
     *
     * \param[in] matrix The matrix to calculate the determant of.
     *
     * \return Returns the complex conjugate of this value.
     */
    M_PUBLIC_API inline Real determinant(const MatrixBoolean& matrix) {
        return MatrixReal(matrix).determinant();
    }

    /**
     * Method that returns the determinant of this matrix.
     *
     * \param[in] matrix The matrix to calculate the determant of.
     *
     * \return Returns the complex conjugate of this value.
     */
    M_PUBLIC_API inline Real determinant(const MatrixInteger& matrix) {
        return MatrixReal(matrix).determinant();
    }

    /**
     * Method that returns the determinant of this matrix.
     *
     * \param[in] matrix The matrix to calculate the determant of.
     *
     * \return Returns the complex conjugate of this value.
     */
    M_PUBLIC_API inline Real determinant(const MatrixReal& matrix) {
        return matrix.determinant();
    }

    /**
     * Method that returns the determinant of this matrix.
     *
     * \param[in] matrix The matrix to calculate the determant of.
     *
     * \return Returns the complex conjugate of this value.
     */
    M_PUBLIC_API inline Complex determinant(const MatrixComplex& matrix) {
        return matrix.determinant();
    }

    /**
     * Method that returns the determinant of this matrix.
     *
     * \param[in] matrix The matrix to calculate the determant of.
     *
     * \return Returns the complex conjugate of this value.
     */
    M_PUBLIC_API Complex determinant(const Variant& matrix);

    /**
     * Method that perform LU decomposition with partial pivoting on this matrix.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the pivot matrix, the lower triangular matrix, the upper
     *         triangular matrix, and a flag indicating whether the matrix was not singular.
     */
    M_PUBLIC_API inline Tuple plu(const MatrixBoolean& matrix) {
        return MatrixReal(matrix).plu();
    }

    /**
     * Method that perform LU decomposition with partial pivoting on this matrix.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the pivot matrix, the lower triangular matrix, the upper
     *         triangular matrix, and a flag indicating whether the matrix was not singular.
     */
    M_PUBLIC_API inline Tuple plu(const MatrixInteger& matrix) {
        return MatrixReal(matrix).plu();
    }

    /**
     * Method that perform LU decomposition with partial pivoting on this matrix.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the pivot matrix, the lower triangular matrix, the upper
     *         triangular matrix, and a flag indicating whether the matrix was not singular.
     */
    M_PUBLIC_API inline Tuple plu(const MatrixReal& matrix) {
        return matrix.plu();
    }

    /**
     * Method that perform LU decomposition with partial pivoting on this matrix.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the pivot matrix, the lower triangular matrix, the upper
     *         triangular matrix, and a flag indicating whether the matrix was not singular.
     */
    M_PUBLIC_API inline Tuple plu(const MatrixComplex& matrix) {
        return matrix.plu();
    }

    /**
     * Method that perform LU decomposition with partial pivoting on this matrix.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the pivot matrix, the lower triangular matrix, the upper
     *         triangular matrix, and a flag indicating whether the matrix was not singular.
     */
    M_PUBLIC_API Tuple plu(const Variant& matrix);

    /**
     * Method that perform singular value decomposition on this matrix.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the unitary matrix (U), the singular values matrix, (sigma), the
     *         conjugate transpose of the left and right singular vectors (V) of the matrix, and a boolean
     *         flag indicating success (or failure).
     */
    M_PUBLIC_API inline Tuple svd(const MatrixBoolean& matrix) {
        return MatrixReal(matrix).svd();
    }

    /**
     * Method that perform singular value decomposition on this matrix.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the unitary matrix (U), the singular values matrix, (sigma), the
     *         conjugate transpose of the left and right singular vectors (V) of the matrix, and a boolean
     *         flag indicating success (or failure).
     */
    M_PUBLIC_API inline Tuple svd(const MatrixInteger& matrix) {
        return MatrixReal(matrix).svd();
    }

    /**
     * Method that perform singular value decomposition on this matrix.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the unitary matrix (U), the singular values matrix, (sigma), the
     *         conjugate transpose of the left and right singular vectors (V) of the matrix, and a boolean
     *         flag indicating success (or failure).
     */
    M_PUBLIC_API inline Tuple svd(const MatrixReal& matrix) {
        return matrix.svd();
    }

    /**
     * Method that perform singular value decomposition on this matrix.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the unitary matrix (U), the singular values matrix, (sigma), the
     *         conjugate transpose of the left and right singular vectors (V) of the matrix, and a boolean
     *         flag indicating success (or failure).
     */
    M_PUBLIC_API inline Tuple svd(const MatrixComplex& matrix) {
        return matrix.svd();
    }

    /**
     * Method that perform singular value decomposition on this matrix.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the unitary matrix (U), the singular values matrix, (sigma), the
     *         conjugate transpose of the left and right singular vectors (V) of the matrix, and a boolean
     *         flag indicating success (or failure).
     */
    M_PUBLIC_API Tuple svd(const Variant& matrix);

    /**
     * Method that performs QR decomposition of this matrix.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the Q unitary matrix, the R upper trianglular matrix representing,
     *         and a flag indicating success.
     */
    M_PUBLIC_API inline Tuple qr(const MatrixBoolean& matrix) {
        return MatrixReal(matrix).qr();
    }

    /**
     * Method that performs QR decomposition of this matrix.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the Q unitary matrix, the R upper trianglular matrix representing,
     *         and a flag indicating success.
     */
    M_PUBLIC_API inline Tuple qr(const MatrixInteger& matrix) {
        return MatrixReal(matrix).qr();
    }

    /**
     * Method that performs QR decomposition of this matrix.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the Q unitary matrix, the R upper trianglular matrix representing,
     *         and a flag indicating success.
     */
    M_PUBLIC_API inline Tuple qr(const MatrixReal& matrix) {
        return matrix.qr();
    }

    /**
     * Method that performs QR decomposition of this matrix.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the Q unitary matrix, the R upper trianglular matrix representing,
     *         and a flag indicating success.
     */
    M_PUBLIC_API inline Tuple qr(const MatrixComplex& matrix) {
        return matrix.qr();
    }

    /**
     * Method that performs QR decomposition of this matrix.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the Q unitary matrix, the R upper trianglular matrix representing,
     *         and a flag indicating success.
     */
    M_PUBLIC_API Tuple qr(const Variant& matrix);

    /**
     * Method that performs LQ decomposition of this matrix.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the L lower triangular matrix, the Q unitary matrix, and a flag
     *         indicating success.
     */
    M_PUBLIC_API inline Tuple lq(const MatrixBoolean& matrix) {
        return MatrixReal(matrix).lq();
    }

    /**
     * Method that performs LQ decomposition of this matrix.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the L lower triangular matrix, the Q unitary matrix, and a flag
     *         indicating success.
     */
    M_PUBLIC_API inline Tuple lq(const MatrixInteger& matrix) {
        return MatrixReal(matrix).lq();
    }

    /**
     * Method that performs LQ decomposition of this matrix.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the L lower triangular matrix, the Q unitary matrix, and a flag
     *         indicating success.
     */
    M_PUBLIC_API inline Tuple lq(const MatrixReal& matrix) {
        return matrix.lq();
    }

    /**
     * Method that performs LQ decomposition of this matrix.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the L lower triangular matrix, the Q unitary matrix, and a flag
     *         indicating success.
     */
    M_PUBLIC_API inline Tuple lq(const MatrixComplex& matrix) {
        return matrix.lq();
    }

    /**
     * Method that performs LQ decomposition of this matrix.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the L lower triangular matrix, the Q unitary matrix, and a flag
     *         indicating success.
     */
    M_PUBLIC_API Tuple lq(const Variant& matrix);

    /**
     * Method that performs Cholesky decomposition of this matrix.  The matrix must be Hermitian.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns the lower triangular matrix representing the Cholesky decomposition of this matrix.
     *         An empty matrix will be returned on failure.
     */
    M_PUBLIC_API inline MatrixReal cholesky(const MatrixBoolean& matrix) {
        return MatrixReal(matrix).cholesky();
    }

    /**
     * Method that performs Cholesky decomposition of this matrix.  The matrix must be Hermitian.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns the lower triangular matrix representing the Cholesky decomposition of this matrix.
     *         An empty matrix will be returned on failure.
     */
    M_PUBLIC_API inline MatrixReal cholesky(const MatrixInteger& matrix) {
        return MatrixReal(matrix).cholesky();
    }

    /**
     * Method that performs Cholesky decomposition of this matrix.  The matrix must be Hermitian.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns the lower triangular matrix representing the Cholesky decomposition of this matrix.
     *         An empty matrix will be returned on failure.
     */
    M_PUBLIC_API inline MatrixReal cholesky(const MatrixReal& matrix) {
        return matrix.cholesky();
    }

    /**
     * Method that performs Cholesky decomposition of this matrix.  The matrix must be Hermitian.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns the lower triangular matrix representing the Cholesky decomposition of this matrix.
     *         An empty matrix will be returned on failure.
     */
    M_PUBLIC_API inline MatrixComplex cholesky(const MatrixComplex& matrix) {
        return matrix.cholesky();
    }

    /**
     * Method that performs Cholesky decomposition of this matrix.  The matrix must be Hermitian.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns the lower triangular matrix representing the Cholesky decomposition of this matrix.
     *         An empty matrix will be returned on failure.
     */
    M_PUBLIC_API Variant cholesky(const Variant& matrix);

    /**
     * Method that performs Cholesky decomposition of this matrix, creating an upper triangular matrix.  The
     * matrix must be Hermitian.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns the upper triangular matrix representing the Cholesky decomposition of this matrix.
     *         An empty matrix will be returned on failure.
     */
    M_PUBLIC_API inline MatrixReal upperCholesky(const MatrixBoolean& matrix) {
        return MatrixReal(matrix).upperCholesky();
    }

    /**
     * Method that performs Cholesky decomposition of this matrix, creating an upper triangular matrix.  The
     * matrix must be Hermitian.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns the upper triangular matrix representing the Cholesky decomposition of this matrix.
     *         An empty matrix will be returned on failure.
     */
    M_PUBLIC_API inline MatrixReal upperCholesky(const MatrixInteger& matrix) {
        return MatrixReal(matrix).upperCholesky();
    }

    /**
     * Method that performs Cholesky decomposition of this matrix, creating an upper triangular matrix.  The
     * matrix must be Hermitian.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns the upper triangular matrix representing the Cholesky decomposition of this matrix.
     *         An empty matrix will be returned on failure.
     */
    M_PUBLIC_API inline MatrixReal upperCholesky(const MatrixReal& matrix) {
        return matrix.upperCholesky();
    }

    /**
     * Method that performs Cholesky decomposition of this matrix, creating an upper triangular matrix.  The
     * matrix must be Hermitian.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns the upper triangular matrix representing the Cholesky decomposition of this matrix.
     *         An empty matrix will be returned on failure.
     */
    M_PUBLIC_API inline MatrixComplex upperCholesky(const MatrixComplex& matrix) {
        return matrix.upperCholesky();
    }

    /**
     * Method that performs Cholesky decomposition of this matrix, creating an upper triangular matrix.  The
     * matrix must be Hermitian.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns the upper triangular matrix representing the Cholesky decomposition of this matrix.
     *         An empty matrix will be returned on failure.
     */
    M_PUBLIC_API Variant upperCholesky(const Variant& matrix);

    /**
     * Method that reduces this matrix to upper Hessenberg form.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the Q matrix and the upper Hessenberg matrix.
     */
    M_PUBLIC_API inline Tuple hessenberg(const MatrixBoolean& matrix) {
        return MatrixReal(matrix).hessenberg();
    }

    /**
     * Method that reduces this matrix to upper Hessenberg form.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the Q matrix and the upper Hessenberg matrix.
     */
    M_PUBLIC_API inline Tuple hessenberg(const MatrixInteger& matrix) {
        return MatrixReal(matrix).hessenberg();
    }

    /**
     * Method that reduces this matrix to upper Hessenberg form.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the Q matrix and the upper Hessenberg matrix.
     */
    M_PUBLIC_API inline Tuple hessenberg(const MatrixReal& matrix) {
        return matrix.hessenberg();
    }

    /**
     * Method that reduces this matrix to upper Hessenberg form.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the Q matrix and the upper Hessenberg matrix.
     */
    M_PUBLIC_API inline Tuple hessenberg(const MatrixComplex& matrix) {
        return matrix.hessenberg();
    }

    /**
     * Method that reduces this matrix to upper Hessenberg form.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the Q matrix and the upper Hessenberg matrix.
     */
    M_PUBLIC_API Tuple hessenberg(const Variant& matrix);

    /**
     * Method that performs a Schur decomposition of this matrix.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the Q unitary matrix, the U upper triangular Schur matrix that
     *         represents the Schur decomposition of this matrix, a column matrix containing just the
     *         eignenvalues of the matrix, and a flag indicating true on success, false on error.
     */
    M_PUBLIC_API inline Tuple schur(const MatrixBoolean& matrix) {
        return MatrixComplex(matrix).schur();
    }

    /**
     * Method that performs a Schur decomposition of this matrix.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the Q unitary matrix, the U upper triangular Schur matrix that
     *         represents the Schur decomposition of this matrix, a column matrix containing just the
     *         eignenvalues of the matrix, and a flag indicating true on success, false on error.
     */
    M_PUBLIC_API inline Tuple schur(const MatrixInteger& matrix) {
        return MatrixComplex(matrix).schur();
    }

    /**
     * Method that performs a Schur decomposition of this matrix.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the Q unitary matrix, the U upper triangular Schur matrix that
     *         represents the Schur decomposition of this matrix, a column matrix containing just the
     *         eignenvalues of the matrix, and a flag indicating true on success, false on error.
     */
    M_PUBLIC_API inline Tuple schur(const MatrixReal& matrix) {
        return MatrixComplex(matrix).schur();
    }

    /**
     * Method that performs a Schur decomposition of this matrix.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the Q unitary matrix, the U upper triangular Schur matrix that
     *         represents the Schur decomposition of this matrix, a column matrix containing just the
     *         eignenvalues of the matrix, and a flag indicating true on success, false on error.
     */
    M_PUBLIC_API inline Tuple schur(const MatrixComplex& matrix) {
        return matrix.schur();
    }

    /**
     * Method that performs a Schur decomposition of this matrix.
     *
     * \param[in] matrix The matrix to calculate the decomposition of.
     *
     * \return Returns a tuple containing the Q unitary matrix, the U upper triangular Schur matrix that
     *         represents the Schur decomposition of this matrix, a column matrix containing just the
     *         eignenvalues of the matrix, and a flag indicating true on success, false on error.
     */
    M_PUBLIC_API Tuple schur(const Variant& matrix);

    /**
     * Method that calculates a complete set of right eigenvectors and eigenvalues for this matrix.  The method
     * also calculates and returns the Schur decomposition of this matrix.
     *
     * \param[in] matrix The matrix to calculate the eigenvalues of.
     *
     * \return Returns a tuple containing a row matrix holding the eigenvalues, the Q unitary matrix, the U
     *         upper triangular Schur matrix that represents the Schur decomposition of this matrix, and zero
     *         or more column matrices representing the eigenvectors corresponding to the supplied eigenvalues.
     *
     *         If an error occurs, an empty matrix of eigenvalues will be returned.
     */
    M_PUBLIC_API inline Tuple eigenvectors(const MatrixBoolean& matrix) {
        return MatrixComplex(matrix).eigenvectors();
    }

    /**
     * Method that calculates a complete set of right eigenvectors and eigenvalues for this matrix.  The method
     * also calculates and returns the Schur decomposition of this matrix.
     *
     * \param[in] matrix The matrix to calculate the eigenvalues of.
     *
     * \return Returns a tuple containing a row matrix holding the eigenvalues, the Q unitary matrix, the U
     *         upper triangular Schur matrix that represents the Schur decomposition of this matrix, and zero
     *         or more column matrices representing the eigenvectors corresponding to the supplied eigenvalues.
     *
     *         If an error occurs, an empty matrix of eigenvalues will be returned.
     */
    M_PUBLIC_API inline Tuple eigenvectors(const MatrixInteger& matrix) {
        return MatrixComplex(matrix).eigenvectors();
    }

    /**
     * Method that calculates a complete set of right eigenvectors and eigenvalues for this matrix.  The method
     * also calculates and returns the Schur decomposition of this matrix.
     *
     * \param[in] matrix The matrix to calculate the eigenvalues of.
     *
     * \return Returns a tuple containing a row matrix holding the eigenvalues, the Q unitary matrix, the U
     *         upper triangular Schur matrix that represents the Schur decomposition of this matrix, and zero
     *         or more column matrices representing the eigenvectors corresponding to the supplied eigenvalues.
     *
     *         If an error occurs, an empty matrix of eigenvalues will be returned.
     */
    M_PUBLIC_API inline Tuple eigenvectors(const MatrixReal& matrix) {
        return MatrixComplex(matrix).eigenvectors();
    }

    /**
     * Method that calculates a complete set of right eigenvectors and eigenvalues for this matrix.  The method
     * also calculates and returns the Schur decomposition of this matrix.
     *
     * \param[in] matrix The matrix to calculate the eigenvalues of.
     *
     * \return Returns a tuple containing a row matrix holding the eigenvalues, the Q unitary matrix, the U
     *         upper triangular Schur matrix that represents the Schur decomposition of this matrix, and zero
     *         or more column matrices representing the eigenvectors corresponding to the supplied eigenvalues.
     *
     *         If an error occurs, an empty matrix of eigenvalues will be returned.
     */
    M_PUBLIC_API inline Tuple eigenvectors(const MatrixComplex& matrix) {
        return matrix.eigenvectors();
    }

    /**
     * Method that calculates a complete set of right eigenvectors and eigenvalues for this matrix.  The method
     * also calculates and returns the Schur decomposition of this matrix.
     *
     * \param[in] matrix The matrix to calculate the eigenvalues of.
     *
     * \return Returns a tuple containing a row matrix holding the eigenvalues, the Q unitary matrix, the U
     *         upper triangular Schur matrix that represents the Schur decomposition of this matrix, and zero
     *         or more column matrices representing the eigenvectors corresponding to the supplied eigenvalues.
     *
     *         If an error occurs, an empty matrix of eigenvalues will be returned.
     */
    M_PUBLIC_API Tuple eigenvectors(const Variant& matrix);

    /**
     * Method that calculates the forward discrete Fourier transform (DFT) of this matrix.  A matrix with a
     * single dimension will be processed as a one-dimensional DFT.  A matrix with multiple rows and columns
     * will be processed as a two dimensional DFT.
     *
     * \param[in] matrix The matrix to calculate the DFT of.
     *
     * \return Returns a matrix representing the complex discrete Fourier transform of the matrix.
     */
    M_PUBLIC_API inline MatrixComplex dft(const MatrixBoolean& matrix) {
        return MatrixComplex(matrix).dft();
    }

    /**
     * Method that calculates the forward discrete Fourier transform (DFT) of this matrix.  A matrix with a
     * single dimension will be processed as a one-dimensional DFT.  A matrix with multiple rows and columns
     * will be processed as a two dimensional DFT.
     *
     * \param[in] matrix The matrix to calculate the DFT of.
     *
     * \return Returns a matrix representing the complex discrete Fourier transform of the matrix.
     */
    M_PUBLIC_API inline MatrixComplex dft(const MatrixInteger& matrix) {
        return MatrixComplex(matrix).dft();
    }

    /**
     * Method that calculates the forward discrete Fourier transform (DFT) of this matrix.  A matrix with a
     * single dimension will be processed as a one-dimensional DFT.  A matrix with multiple rows and columns
     * will be processed as a two dimensional DFT.
     *
     * \param[in] matrix The matrix to calculate the DFT of.
     *
     * \return Returns a matrix representing the complex discrete Fourier transform of the matrix.
     */
    M_PUBLIC_API inline MatrixComplex dft(const MatrixReal& matrix) {
        return MatrixComplex(matrix).dft();
    }

    /**
     * Method that calculates the forward discrete Fourier transform (DFT) of this matrix.  A matrix with a
     * single dimension will be processed as a one-dimensional DFT.  A matrix with multiple rows and columns
     * will be processed as a two dimensional DFT.
     *
     * \param[in] matrix The matrix to calculate the DFT of.
     *
     * \return Returns a matrix representing the complex discrete Fourier transform of the matrix.
     */
    M_PUBLIC_API inline MatrixComplex dft(const MatrixComplex& matrix) {
        return matrix.dft();
    }

    /**
     * Method that calculates the forward discrete Fourier transform (DFT) of this matrix.  A matrix with a
     * single dimension will be processed as a one-dimensional DFT.  A matrix with multiple rows and columns
     * will be processed as a two dimensional DFT.
     *
     * \param[in] matrix The matrix to calculate the DFT of.
     *
     * \return Returns a matrix representing the complex discrete Fourier transform of the matrix.
     */
    M_PUBLIC_API inline MatrixComplex dft(const Variant& matrix) {
        return matrix.toMatrixComplex().dft();
    }

    /**
     * Method that calculates the backwards or inverse discrete Fourier transform (DFT) of this matrix.  A
     * matrix with a single dimension will be processed as a one dimensional backward DFT.  A matrix with
     * multiple rows and columns will be processed as a two dimensional backward DFT.
     *
     * \param[in] matrix The matrix to calculate the inverse DFT of.
     *
     * \return Returns a matrix representing the complex inverse discrete Fourier transform of the matrix.
     */
    M_PUBLIC_API inline MatrixComplex idft(const MatrixBoolean& matrix) {
        return MatrixComplex(matrix).idft();
    }

    /**
     * Method that calculates the backwards or inverse discrete Fourier transform (DFT) of this matrix.  A
     * matrix with a single dimension will be processed as a one dimensional backward DFT.  A matrix with
     * multiple rows and columns will be processed as a two dimensional backward DFT.
     *
     * \param[in] matrix The matrix to calculate the inverse DFT of.
     *
     * \return Returns a matrix representing the complex inverse discrete Fourier transform of the matrix.
     */
    M_PUBLIC_API inline MatrixComplex idft(const MatrixInteger& matrix) {
        return MatrixComplex(matrix).idft();
    }

    /**
     * Method that calculates the backwards or inverse discrete Fourier transform (DFT) of this matrix.  A
     * matrix with a single dimension will be processed as a one dimensional backward DFT.  A matrix with
     * multiple rows and columns will be processed as a two dimensional backward DFT.
     *
     * \param[in] matrix The matrix to calculate the inverse DFT of.
     *
     * \return Returns a matrix representing the complex inverse discrete Fourier transform of the matrix.
     */
    M_PUBLIC_API inline MatrixComplex idft(const MatrixReal& matrix) {
        return MatrixComplex(matrix).idft();
    }

    /**
     * Method that calculates the backwards or inverse discrete Fourier transform (DFT) of this matrix.  A
     * matrix with a single dimension will be processed as a one dimensional backward DFT.  A matrix with
     * multiple rows and columns will be processed as a two dimensional backward DFT.
     *
     * \param[in] matrix The matrix to calculate the inverse DFT of.
     *
     * \return Returns a matrix representing the complex inverse discrete Fourier transform of the matrix.
     */
    M_PUBLIC_API inline MatrixComplex idft(const MatrixComplex& matrix) {
        return matrix.idft();
    }

    /**
     * Method that calculates the backwards or inverse discrete Fourier transform (DFT) of this matrix.  A
     * matrix with a single dimension will be processed as a one dimensional backward DFT.  A matrix with
     * multiple rows and columns will be processed as a two dimensional backward DFT.
     *
     * \param[in] matrix The matrix to calculate the inverse DFT of.
     *
     * \return Returns a matrix representing the complex inverse discrete Fourier transform of the matrix.
     */
    M_PUBLIC_API inline MatrixComplex idft(const Variant& matrix) {
        return matrix.toMatrixComplex().idft();
    }

    /**
     * Method that calculates the forward discrete cosine transform, type 2 (DCT) of this matrix.  A matrix with a
     * single dimension will be processed as a one-dimensional DCT.  A matrix with multiple rows and columns
     * will be processed as a two dimensional DCT.
     *
     * \param[in] matrix The matrix to calculate the DCT of.
     *
     * \return Returns a matrix representing the complex discrete cosine transform, type 2 of the matrix.
     */
    M_PUBLIC_API inline MatrixReal dct(const MatrixBoolean& matrix) {
        return MatrixReal(matrix).dct();
    }

    /**
     * Method that calculates the forward discrete cosine transform, type 2 (DCT) of this matrix.  A matrix with a
     * single dimension will be processed as a one-dimensional DCT.  A matrix with multiple rows and columns
     * will be processed as a two dimensional DCT.
     *
     * \param[in] matrix The matrix to calculate the DCT of.
     *
     * \return Returns a matrix representing the complex discrete cosine transform, type 2 of the matrix.
     */
    M_PUBLIC_API inline MatrixReal dct(const MatrixInteger& matrix) {
        return MatrixReal(matrix).dct();
    }

    /**
     * Method that calculates the forward discrete cosine transform, type 2 (DCT) of this matrix.  A matrix with a
     * single dimension will be processed as a one-dimensional DCT.  A matrix with multiple rows and columns
     * will be processed as a two dimensional DCT.
     *
     * \param[in] matrix The matrix to calculate the DCT of.
     *
     * \return Returns a matrix representing the complex discrete cosine transform, type 2 of the matrix.
     */
    M_PUBLIC_API inline MatrixReal dct(const MatrixReal& matrix) {
        return MatrixReal(matrix).dct();
    }

    /**
     * Method that calculates the forward discrete cosine transform, type 2 (DCT) of this matrix.  A matrix with a
     * single dimension will be processed as a one-dimensional DCT.  A matrix with multiple rows and columns
     * will be processed as a two dimensional DCT.
     *
     * \param[in] matrix The matrix to calculate the DCT of.
     *
     * \return Returns a matrix representing the complex discrete cosine transform, type 2 of the matrix.
     */
    M_PUBLIC_API inline MatrixReal dct(const Variant& matrix) {
        return matrix.toMatrixReal().dct();
    }

    /**
     * Method that calculates the forward discrete cosine transform, type 3 (DCT) of this matrix.  A matrix with a
     * single dimension will be processed as a one-dimensional DCT.  A matrix with multiple rows and columns
     * will be processed as a two dimensional DCT.
     *
     * \param[in] matrix The matrix to calculate the DCT of.
     *
     * \return Returns a matrix representing the complex discrete cosine transform, type 3 of the matrix.
     */
    M_PUBLIC_API inline MatrixReal idct(const MatrixBoolean& matrix) {
        return MatrixReal(matrix).idct();
    }

    /**
     * Method that calculates the forward discrete cosine transform, type 3 (DCT) of this matrix.  A matrix with a
     * single dimension will be processed as a one-dimensional DCT.  A matrix with multiple rows and columns
     * will be processed as a two dimensional DCT.
     *
     * \param[in] matrix The matrix to calculate the DCT of.
     *
     * \return Returns a matrix representing the complex discrete cosine transform, type 3 of the matrix.
     */
    M_PUBLIC_API inline MatrixReal idct(const MatrixInteger& matrix) {
        return MatrixReal(matrix).idct();
    }

    /**
     * Method that calculates the forward discrete cosine transform, type 3 (DCT) of this matrix.  A matrix with a
     * single dimension will be processed as a one-dimensional DCT.  A matrix with multiple rows and columns
     * will be processed as a two dimensional DCT.
     *
     * \param[in] matrix The matrix to calculate the DCT of.
     *
     * \return Returns a matrix representing the complex discrete cosine transform, type 3 of the matrix.
     */
    M_PUBLIC_API inline MatrixReal idct(const MatrixReal& matrix) {
        return MatrixReal(matrix).idct();
    }

    /**
     * Method that calculates the forward discrete cosine transform, type 3 (DCT) of this matrix.  A matrix with a
     * single dimension will be processed as a one-dimensional DCT.  A matrix with multiple rows and columns
     * will be processed as a two dimensional DCT.
     *
     * \param[in] matrix The matrix to calculate the DCT of.
     *
     * \return Returns a matrix representing the complex discrete cosine transform, type 3 of the matrix.
     */
    M_PUBLIC_API inline MatrixReal idct(const Variant& matrix) {
        return matrix.toMatrixReal().idct();
    }

    /**
     * Method that calculates the Hilbert transform of a row or column matrix.
     *
     * \param[in] matrix The matrix to calculate the Hilbert transform of.
     *
     * \return Returns a complex matrix holding the original matrix in the real portion and the phase
     *         shifted/transformed matrix in the imaginary portion.
     */
    M_PUBLIC_API inline MatrixComplex hilbertTransform(const MatrixBoolean& matrix) {
        return MatrixReal(matrix).hilbertTransform();
    }

    /**
     * Method that calculates the Hilbert transform of a row or column matrix.
     *
     * \param[in] matrix The matrix to calculate the Hilbert transform of.
     *
     * \return Returns a complex matrix holding the original matrix in the real portion and the phase
     *         shifted/transformed matrix in the imaginary portion.
     */
    M_PUBLIC_API inline MatrixComplex hilbertTransform(const MatrixInteger& matrix) {
        return MatrixReal(matrix).hilbertTransform();
    }

    /**
     * Method that calculates the Hilbert transform of a row or column matrix.
     *
     * \param[in] matrix The matrix to calculate the Hilbert transform of.
     *
     * \return Returns a complex matrix holding the original matrix in the real portion and the phase
     *         shifted/transformed matrix in the imaginary portion.
     */
    M_PUBLIC_API inline MatrixComplex hilbertTransform(const MatrixReal& matrix) {
        return matrix.hilbertTransform();
    }

    /**
     * Method that calculates the Hilbert transform of a row or column matrix.
     *
     * \param[in] matrix The matrix to calculate the Hilbert transform of.
     *
     * \return Returns a complex matrix holding the original matrix in the real portion and the phase
     *         shifted/transformed matrix in the imaginary portion.
     */
    M_PUBLIC_API inline MatrixComplex hilbertTransform(const Variant& matrix) {
        return matrix.toMatrixReal().hilbertTransform();
    }

    /**
     * Method that calculates the condition number of this matrix.
     *
     * \param[in] matrix The matrix to calculate the condition number of.
     *
     * \return Returns the condition number of this matrix.
     */
    M_PUBLIC_API inline Real conditionNumber(const MatrixBoolean& matrix) {
        return MatrixReal(matrix).conditionNumber();
    }

    /**
     * Method that calculates the condition number of this matrix.
     *
     * \param[in] matrix The matrix to calculate the condition number of.
     *
     * \return Returns the condition number of this matrix.
     */
    M_PUBLIC_API inline Real conditionNumber(const MatrixInteger& matrix) {
        return MatrixReal(matrix).conditionNumber();
    }

    /**
     * Method that calculates the condition number of this matrix.
     *
     * \param[in] matrix The matrix to calculate the condition number of.
     *
     * \return Returns the condition number of this matrix.
     */
    M_PUBLIC_API inline Real conditionNumber(const MatrixReal& matrix) {
        return matrix.conditionNumber();
    }

    /**
     * Method that calculates the condition number of this matrix.
     *
     * \param[in] matrix The matrix to calculate the condition number of.
     *
     * \return Returns the condition number of this matrix.
     */
    M_PUBLIC_API inline Real conditionNumber(const MatrixComplex& matrix) {
        return matrix.conditionNumber();
    }

    /**
     * Method that calculates the condition number of this matrix.
     *
     * \param[in] matrix The matrix to calculate the condition number of.
     *
     * \return Returns the condition number of this matrix.
     */
    M_PUBLIC_API Real conditionNumber(const Variant& matrix);

    /**
     * Method that calculates the entry-wise p-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \param[in] p      The power term to apply to the norm operation.
     *
     * \return Returns the p-norm of this matrix.
     */
    M_PUBLIC_API inline Real pNorm(const MatrixBoolean& matrix, Integer p) {
        return MatrixReal(matrix).pNorm(p);
    }

    /**
     * Method that calculates the entry-wise p-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \param[in] p      The power term to apply to the norm operation.
     *
     * \return Returns the p-norm of this matrix.
     */
    M_PUBLIC_API inline Real pNorm(const MatrixInteger& matrix, Integer p) {
        return MatrixReal(matrix).pNorm(p);
    }

    /**
     * Method that calculates the entry-wise p-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \param[in] p      The power term to apply to the norm operation.
     *
     * \return Returns the p-norm of this matrix.
     */
    M_PUBLIC_API inline Real pNorm(const MatrixReal& matrix, Integer p) {
        return matrix.pNorm(p);
    }

    /**
     * Method that calculates the entry-wise p-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \param[in] p      The power term to apply to the norm operation.
     *
     * \return Returns the p-norm of this matrix.
     */
    M_PUBLIC_API inline Real pNorm(const MatrixComplex& matrix, Integer p) {
        return matrix.pNorm(p);
    }

    /**
     * Method that calculates the entry-wise p-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \param[in] p      The power term to apply to the norm operation.
     *
     * \return Returns the p-norm of this matrix.
     */
    M_PUBLIC_API inline Real pNorm(const MatrixBoolean& matrix, Boolean p) {
        return MatrixReal(matrix).pNorm(Integer(p ? 1 : 0));
    }

    /**
     * Method that calculates the entry-wise p-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \param[in] p      The power term to apply to the norm operation.
     *
     * \return Returns the p-norm of this matrix.
     */
    M_PUBLIC_API inline Real pNorm(const MatrixInteger& matrix, Boolean p) {
        return MatrixReal(matrix).pNorm(Integer(p ? 1 : 0));
    }

    /**
     * Method that calculates the entry-wise p-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \param[in] p      The power term to apply to the norm operation.
     *
     * \return Returns the p-norm of this matrix.
     */
    M_PUBLIC_API inline Real pNorm(const MatrixReal& matrix, Boolean p) {
        return matrix.pNorm(Integer(p ? 1 : 0));
    }

    /**
     * Method that calculates the entry-wise p-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \param[in] p      The power term to apply to the norm operation.
     *
     * \return Returns the p-norm of this matrix.
     */
    M_PUBLIC_API inline Real pNorm(const MatrixComplex& matrix, Boolean p) {
        return matrix.pNorm(Integer(p ? 1 : 0));
    }

    /**
     * Method that calculates the entry-wise p-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \param[in] p      The power term to apply to the norm operation.
     *
     * \return Returns the p-norm of this matrix.
     */
    M_PUBLIC_API Real pNorm(const MatrixBoolean& matrix, Real p);

    /**
     * Method that calculates the entry-wise p-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \param[in] p      The power term to apply to the norm operation.
     *
     * \return Returns the p-norm of this matrix.
     */
    M_PUBLIC_API Real pNorm(const MatrixInteger& matrix, Real p);

    /**
     * Method that calculates the entry-wise p-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \param[in] p      The power term to apply to the norm operation.
     *
     * \return Returns the p-norm of this matrix.
     */
    M_PUBLIC_API Real pNorm(const MatrixReal& matrix, Real p);

    /**
     * Method that calculates the entry-wise p-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \param[in] p      The power term to apply to the norm operation.
     *
     * \return Returns the p-norm of this matrix.
     */
    M_PUBLIC_API Real pNorm(const MatrixComplex& matrix, Real p);

    /**
     * Method that calculates the entry-wise p-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \param[in] p      The power term to apply to the norm operation.
     *
     * \return Returns the p-norm of this matrix.
     */
    M_PUBLIC_API Real pNorm(const MatrixBoolean& matrix, const Complex& p);

    /**
     * Method that calculates the entry-wise p-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \param[in] p      The power term to apply to the norm operation.
     *
     * \return Returns the p-norm of this matrix.
     */
    M_PUBLIC_API Real pNorm(const MatrixInteger& matrix, const Complex& p);

    /**
     * Method that calculates the entry-wise p-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \param[in] p      The power term to apply to the norm operation.
     *
     * \return Returns the p-norm of this matrix.
     */
    M_PUBLIC_API Real pNorm(const MatrixReal& matrix, const Complex& p);

    /**
     * Method that calculates the entry-wise p-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \param[in] p      The power term to apply to the norm operation.
     *
     * \return Returns the p-norm of this matrix.
     */
    M_PUBLIC_API Real pNorm(const MatrixComplex& matrix, const Complex& p);

    /**
     * Method that calculates the entry-wise p-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \param[in] p      The power term to apply to the norm operation.
     *
     * \return Returns the p-norm of this matrix.
     */
    M_PUBLIC_API Real pNorm(const Variant& matrix, Boolean p);

    /**
     * Method that calculates the entry-wise p-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \param[in] p      The power term to apply to the norm operation.
     *
     * \return Returns the p-norm of this matrix.
     */
    M_PUBLIC_API Real pNorm(const Variant& matrix, Integer p);

    /**
     * Method that calculates the entry-wise p-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \param[in] p      The power term to apply to the norm operation.
     *
     * \return Returns the p-norm of this matrix.
     */
    M_PUBLIC_API Real pNorm(const Variant& matrix, Real p);

    /**
     * Method that calculates the entry-wise p-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \param[in] p      The power term to apply to the norm operation.
     *
     * \return Returns the p-norm of this matrix.
     */
    M_PUBLIC_API Real pNorm(const Variant& matrix, const Complex& p);

    /**
     * Method that calculates the entry-wise p-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \param[in] p      The power term to apply to the norm operation.
     *
     * \return Returns the p-norm of this matrix.
     */
    M_PUBLIC_API Real pNorm(const MatrixBoolean& matrix, const Variant& p);

    /**
     * Method that calculates the entry-wise p-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \param[in] p      The power term to apply to the norm operation.
     *
     * \return Returns the p-norm of this matrix.
     */
    M_PUBLIC_API Real pNorm(const MatrixInteger& matrix, const Variant& p);

    /**
     * Method that calculates the entry-wise p-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \param[in] p      The power term to apply to the norm operation.
     *
     * \return Returns the p-norm of this matrix.
     */
    M_PUBLIC_API Real pNorm(const MatrixInteger& matrix, const Variant& p);

    /**
     * Method that calculates the entry-wise p-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \param[in] p      The power term to apply to the norm operation.
     *
     * \return Returns the p-norm of this matrix.
     */
    M_PUBLIC_API Real pNorm(const MatrixComplex& matrix, const Variant& p);

    /**
     * Method that calculates the entry-wise p-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \param[in] p      The power term to apply to the norm operation.
     *
     * \return Returns the p-norm of this matrix.
     */
    M_PUBLIC_API Real pNorm(const Variant& matrix, const Variant& p);

    /**
     * Method that calculates the entry-wise Euclidean norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \return Returns the Euclidean norm of this matrix.
     */
    M_PUBLIC_API inline Real euclideanNorm(const MatrixBoolean& matrix) {
        return MatrixReal(matrix).euclideanNorm();
    }

    /**
     * Method that calculates the entry-wise Euclidean norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \return Returns the Euclidean norm of this matrix.
     */
    M_PUBLIC_API inline Real euclideanNorm(const MatrixInteger& matrix) {
        return MatrixReal(matrix).euclideanNorm();
    }

    /**
     * Method that calculates the entry-wise Euclidean norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \return Returns the Euclidean norm of this matrix.
     */
    M_PUBLIC_API inline Real euclideanNorm(const MatrixReal& matrix) {
        return matrix.euclideanNorm();
    }

    /**
     * Method that calculates the entry-wise Euclidean norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \return Returns the Euclidean norm of this matrix.
     */
    M_PUBLIC_API inline Real euclideanNorm(const MatrixComplex& matrix) {
        return matrix.euclideanNorm();
    }

    /**
     * Method that calculates the entry-wise Euclidean norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the p-norm of.
     *
     * \return Returns the Euclidean norm of this matrix.
     */
    M_PUBLIC_API Real euclideanNorm(const Variant& matrix);

    /**
     * Method that calculates the 1-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the one-norm of.
     *
     * \return Returns the 1-normnorm of this matrix.
     */
    M_PUBLIC_API inline Real oneNorm(const MatrixBoolean& matrix) {
        return MatrixReal(matrix).oneNorm();
    }

    /**
     * Method that calculates the 1-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the one-norm of.
     *
     * \return Returns the 1-normnorm of this matrix.
     */
    M_PUBLIC_API inline Real oneNorm(const MatrixInteger& matrix) {
        return MatrixReal(matrix).oneNorm();
    }

    /**
     * Method that calculates the 1-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the one-norm of.
     *
     * \return Returns the 1-normnorm of this matrix.
     */
    M_PUBLIC_API inline Real oneNorm(const MatrixReal& matrix) {
        return matrix.oneNorm();
    }

    /**
     * Method that calculates the 1-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the one-norm of.
     *
     * \return Returns the 1-normnorm of this matrix.
     */
    M_PUBLIC_API inline Real oneNorm(const MatrixComplex& matrix) {
        return matrix.oneNorm();
    }

    /**
     * Method that calculates the 1-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the one-norm of.
     *
     * \return Returns the 1-normnorm of this matrix.
     */
    M_PUBLIC_API Real oneNorm(const Variant& matrix);

    /**
     * Method that calculates the infinity-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the infinity-norm of.
     *
     * \return Returns the infinity-norm of this matrix.
     */
    M_PUBLIC_API inline Real infinityNorm(const MatrixBoolean& matrix) {
        return MatrixReal(matrix).infinityNorm();
    }

    /**
     * Method that calculates the infinity-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the infinity-norm of.
     *
     * \return Returns the infinity-norm of this matrix.
     */
    M_PUBLIC_API inline Real infinityNorm(const MatrixInteger& matrix) {
        return MatrixReal(matrix).infinityNorm();
    }

    /**
     * Method that calculates the infinity-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the infinity-norm of.
     *
     * \return Returns the infinity-norm of this matrix.
     */
    M_PUBLIC_API inline Real infinityNorm(const MatrixReal& matrix) {
        return matrix.infinityNorm();
    }

    /**
     * Method that calculates the infinity-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the infinity-norm of.
     *
     * \return Returns the infinity-norm of this matrix.
     */
    M_PUBLIC_API inline Real infinityNorm(const MatrixComplex& matrix) {
        return matrix.infinityNorm();
    }

    /**
     * Method that calculates the infinity-norm of this matrix.
     *
     * \param[in] matrix The matrix to calculate the infinity-norm of.
     *
     * \return Returns the infinity-norm of this matrix.
     */
    M_PUBLIC_API Real infinityNorm(const Variant& matrix);

    /**
     * Method that calculates a set of row and column factors you can use to equilibrate a matrix.
     *
     * \param[in] matrix The matrix to equilibrate.
     *
     * \return Returns a tuple containing two diagnonal matrices plus a flag indicating success.  The first
     *         matrix represents the row scale factors to apply to this matrix.  The second matrix represents
     *         the column scale factors to apply to this matrix.  For the tuple \f[ \left ( R, C \right ) \f],
     *         you can fully equilibrate the matrix "M" using the relation \f[ R \times M \times C \f].
     */
    M_PUBLIC_API inline Tuple equilibrate(const MatrixBoolean& matrix) {
        return MatrixReal(matrix).equilibrate();
    }

    /**
     * Method that calculates a set of row and column factors you can use to equilibrate a matrix.
     *
     * \param[in] matrix The matrix to equilibrate.
     *
     * \return Returns a tuple containing two diagnonal matrices plus a flag indicating success.  The first
     *         matrix represents the row scale factors to apply to this matrix.  The second matrix represents
     *         the column scale factors to apply to this matrix.  For the tuple \f[ \left ( R, C \right ) \f],
     *         you can fully equilibrate the matrix "M" using the relation \f[ R \times M \times C \f].
     */
    M_PUBLIC_API inline Tuple equilibrate(const MatrixInteger& matrix) {
        return MatrixReal(matrix).equilibrate();
    }

    /**
     * Method that calculates a set of row and column factors you can use to equilibrate a matrix.
     *
     * \param[in] matrix The matrix to equilibrate.
     *
     * \return Returns a tuple containing two diagnonal matrices plus a flag indicating success.  The first
     *         matrix represents the row scale factors to apply to this matrix.  The second matrix represents
     *         the column scale factors to apply to this matrix.  For the tuple \f[ \left ( R, C \right ) \f],
     *         you can fully equilibrate the matrix "M" using the relation \f[ R \times M \times C \f].
     */
    M_PUBLIC_API inline Tuple equilibrate(const MatrixReal& matrix) {
        return matrix.equilibrate();
    }

    /**
     * Method that calculates a set of row and column factors you can use to equilibrate a matrix.
     *
     * \param[in] matrix The matrix to equilibrate.
     *
     * \return Returns a tuple containing two diagnonal matrices plus a flag indicating success.  The first
     *         matrix represents the row scale factors to apply to this matrix.  The second matrix represents
     *         the column scale factors to apply to this matrix.  For the tuple \f[ \left ( R, C \right ) \f],
     *         you can fully equilibrate the matrix "M" using the relation \f[ R \times M \times C \f].
     */
    M_PUBLIC_API inline Tuple equilibrate(const MatrixComplex& matrix) {
        return matrix.equilibrate();
    }

    /**
     * Method that calculates a set of row and column factors you can use to equilibrate a matrix.
     *
     * \param[in] matrix The matrix to equilibrate.
     *
     * \return Returns a tuple containing two diagnonal matrices plus a flag indicating success.  The first
     *         matrix represents the row scale factors to apply to this matrix.  The second matrix represents
     *         the column scale factors to apply to this matrix.  For the tuple \f[ \left ( R, C \right ) \f],
     *         you can fully equilibrate the matrix "M" using the relation \f[ R \times M \times C \f].
     */
    M_PUBLIC_API inline Tuple equilibrate(const Variant& matrix);

    /**
     * Method that solves the system of linear equations specified by this matrix.  For a system of linear
     * equations defined by:
     *
     * \f[
     *     A \times X = Y
     * \f]
     *
     * Where 'A' is represented by this matrix and Y is a row vector defining the results, this method will
     * return approximate values for 'X'.
     *
     * \param[in] a The A matrix to use to solve for x.
     *
     * \param[in] y The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'X'.  An empty matrix will be returned if A is singular.
     */
    M_PUBLIC_API inline MatrixReal solve(const MatrixBoolean& a, const MatrixBoolean& y) {
        return MatrixReal(a).solve(MatrixReal(y));
    }

    /**
     * Method that solves the system of linear equations specified by this matrix.  For a system of linear
     * equations defined by:
     *
     * \f[
     *     A \times X = Y
     * \f]
     *
     * Where 'A' is represented by this matrix and Y is a row vector defining the results, this method will
     * return approximate values for 'X'.
     *
     * \param[in] a The A matrix to use to solve for x.
     *
     * \param[in] y The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'X'.  An empty matrix will be returned if A is singular.
     */
    M_PUBLIC_API inline MatrixReal solve(const MatrixBoolean& a, const MatrixInteger& y) {
        return MatrixReal(a).solve(MatrixReal(y));
    }

    /**
     * Method that solves the system of linear equations specified by this matrix.  For a system of linear
     * equations defined by:
     *
     * \f[
     *     A \times X = Y
     * \f]
     *
     * Where 'A' is represented by this matrix and Y is a row vector defining the results, this method will
     * return approximate values for 'X'.
     *
     * \param[in] a The A matrix to use to solve for x.
     *
     * \param[in] y The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'X'.  An empty matrix will be returned if A is singular.
     */
    M_PUBLIC_API inline MatrixReal solve(const MatrixBoolean& a, const MatrixReal& y) {
        return MatrixReal(a).solve(y);
    }

    /**
     * Method that solves the system of linear equations specified by this matrix.  For a system of linear
     * equations defined by:
     *
     * \f[
     *     A \times X = Y
     * \f]
     *
     * Where 'A' is represented by this matrix and Y is a row vector defining the results, this method will
     * return approximate values for 'X'.
     *
     * \param[in] a The A matrix to use to solve for x.
     *
     * \param[in] y The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'X'.  An empty matrix will be returned if A is singular.
     */
    M_PUBLIC_API inline MatrixComplex solve(const MatrixBoolean& a, const MatrixComplex& y) {
        return MatrixComplex(a).solve(y);
    }

    /**
     * Method that solves the system of linear equations specified by this matrix.  For a system of linear
     * equations defined by:
     *
     * \f[
     *     A \times X = Y
     * \f]
     *
     * Where 'A' is represented by this matrix and Y is a row vector defining the results, this method will
     * return approximate values for 'X'.
     *
     * \param[in] a The A matrix to use to solve for x.
     *
     * \param[in] y The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'X'.  An empty matrix will be returned if A is singular.
     */
    M_PUBLIC_API inline MatrixReal solve(const MatrixInteger& a, const MatrixBoolean& y) {
        return MatrixReal(a).solve(MatrixReal(y));
    }

    /**
     * Method that solves the system of linear equations specified by this matrix.  For a system of linear
     * equations defined by:
     *
     * \f[
     *     A \times X = Y
     * \f]
     *
     * Where 'A' is represented by this matrix and Y is a row vector defining the results, this method will
     * return approximate values for 'X'.
     *
     * \param[in] a The A matrix to use to solve for x.
     *
     * \param[in] y The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'X'.  An empty matrix will be returned if A is singular.
     */
    M_PUBLIC_API inline MatrixReal solve(const MatrixInteger& a, const MatrixInteger& y) {
        return MatrixReal(a).solve(MatrixReal(y));
    }

    /**
     * Method that solves the system of linear equations specified by this matrix.  For a system of linear
     * equations defined by:
     *
     * \f[
     *     A \times X = Y
     * \f]
     *
     * Where 'A' is represented by this matrix and Y is a row vector defining the results, this method will
     * return approximate values for 'X'.
     *
     * \param[in] a The A matrix to use to solve for x.
     *
     * \param[in] y The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'X'.  An empty matrix will be returned if A is singular.
     */
    M_PUBLIC_API inline MatrixReal solve(const MatrixInteger& a, const MatrixReal& y) {
        return MatrixReal(a).solve(y);
    }

    /**
     * Method that solves the system of linear equations specified by this matrix.  For a system of linear
     * equations defined by:
     *
     * \f[
     *     A \times X = Y
     * \f]
     *
     * Where 'A' is represented by this matrix and Y is a row vector defining the results, this method will
     * return approximate values for 'X'.
     *
     * \param[in] a The A matrix to use to solve for x.
     *
     * \param[in] y The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'X'.  An empty matrix will be returned if A is singular.
     */
    M_PUBLIC_API inline MatrixComplex solve(const MatrixInteger& a, const MatrixComplex& y) {
        return MatrixComplex(a).solve(MatrixComplex(y));
    }

    /**
     * Method that solves the system of linear equations specified by this matrix.  For a system of linear
     * equations defined by:
     *
     * \f[
     *     A \times X = Y
     * \f]
     *
     * Where 'A' is represented by this matrix and Y is a row vector defining the results, this method will
     * return approximate values for 'X'.
     *
     * \param[in] a The A matrix to use to solve for x.
     *
     * \param[in] y The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'X'.  An empty matrix will be returned if A is singular.
     */
    M_PUBLIC_API inline MatrixReal solve(const MatrixReal& a, const MatrixBoolean& y) {
        return a.solve(MatrixReal(y));
    }

    /**
     * Method that solves the system of linear equations specified by this matrix.  For a system of linear
     * equations defined by:
     *
     * \f[
     *     A \times X = Y
     * \f]
     *
     * Where 'A' is represented by this matrix and Y is a row vector defining the results, this method will
     * return approximate values for 'X'.
     *
     * \param[in] a The A matrix to use to solve for x.
     *
     * \param[in] y The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'X'.  An empty matrix will be returned if A is singular.
     */
    M_PUBLIC_API inline MatrixReal solve(const MatrixReal& a, const MatrixInteger& y) {
        return a.solve(MatrixReal(y));
    }

    /**
     * Method that solves the system of linear equations specified by this matrix.  For a system of linear
     * equations defined by:
     *
     * \f[
     *     A \times X = Y
     * \f]
     *
     * Where 'A' is represented by this matrix and Y is a row vector defining the results, this method will
     * return approximate values for 'X'.
     *
     * \param[in] a The A matrix to use to solve for x.
     *
     * \param[in] y The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'X'.  An empty matrix will be returned if A is singular.
     */
    M_PUBLIC_API inline MatrixReal solve(const MatrixReal& a, const MatrixReal& y) {
        return a.solve(y);
    }

    /**
     * Method that solves the system of linear equations specified by this matrix.  For a system of linear
     * equations defined by:
     *
     * \f[
     *     A \times X = Y
     * \f]
     *
     * Where 'A' is represented by this matrix and Y is a row vector defining the results, this method will
     * return approximate values for 'X'.
     *
     * \param[in] a The A matrix to use to solve for x.
     *
     * \param[in] y The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'X'.  An empty matrix will be returned if A is singular.
     */
    M_PUBLIC_API inline MatrixComplex solve(const MatrixReal& a, const MatrixComplex& y) {
        return MatrixComplex(a).solve(MatrixComplex(y));
    }

    /**
     * Method that solves the system of linear equations specified by this matrix.  For a system of linear
     * equations defined by:
     *
     * \f[
     *     A \times X = Y
     * \f]
     *
     * Where 'A' is represented by this matrix and Y is a row vector defining the results, this method will
     * return approximate values for 'X'.
     *
     * \param[in] a The A matrix to use to solve for x.
     *
     * \param[in] y The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'X'.  An empty matrix will be returned if A is singular.
     */
    M_PUBLIC_API inline MatrixComplex solve(const MatrixComplex& a, const MatrixBoolean& y) {
        return a.solve(MatrixComplex(y));
    }

    /**
     * Method that solves the system of linear equations specified by this matrix.  For a system of linear
     * equations defined by:
     *
     * \f[
     *     A \times X = Y
     * \f]
     *
     * Where 'A' is represented by this matrix and Y is a row vector defining the results, this method will
     * return approximate values for 'X'.
     *
     * \param[in] a The A matrix to use to solve for x.
     *
     * \param[in] y The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'X'.  An empty matrix will be returned if A is singular.
     */
    M_PUBLIC_API inline MatrixComplex solve(const MatrixComplex& a, const MatrixInteger& y) {
        return a.solve(MatrixComplex(y));
    }

    /**
     * Method that solves the system of linear equations specified by this matrix.  For a system of linear
     * equations defined by:
     *
     * \f[
     *     A \times X = Y
     * \f]
     *
     * Where 'A' is represented by this matrix and Y is a row vector defining the results, this method will
     * return approximate values for 'X'.
     *
     * \param[in] a The A matrix to use to solve for x.
     *
     * \param[in] y The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'X'.  An empty matrix will be returned if A is singular.
     */
    M_PUBLIC_API inline MatrixComplex solve(const MatrixComplex& a, const MatrixReal& y) {
        return a.solve(MatrixComplex(y));
    }

    /**
     * Method that solves the system of linear equations specified by this matrix.  For a system of linear
     * equations defined by:
     *
     * \f[
     *     A \times X = Y
     * \f]
     *
     * Where 'A' is represented by this matrix and Y is a row vector defining the results, this method will
     * return approximate values for 'X'.
     *
     * \param[in] a The A matrix to use to solve for x.
     *
     * \param[in] y The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'X'.  An empty matrix will be returned if A is singular.
     */
    M_PUBLIC_API inline MatrixComplex solve(const MatrixComplex& a, const MatrixComplex& y) {
        return a.solve(y);
    }

    /**
     * Method that solves the system of linear equations specified by this matrix.  For a system of linear
     * equations defined by:
     *
     * \f[
     *     A \times X = Y
     * \f]
     *
     * Where 'A' is represented by this matrix and Y is a row vector defining the results, this method will
     * return approximate values for 'X'.
     *
     * \param[in] a The A matrix to use to solve for x.
     *
     * \param[in] y The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'X'.  An empty matrix will be returned if A is singular.
     */
    M_PUBLIC_API Variant solve(const Variant& a, const MatrixBoolean& y);

    /**
     * Method that solves the system of linear equations specified by this matrix.  For a system of linear
     * equations defined by:
     *
     * \f[
     *     A \times X = Y
     * \f]
     *
     * Where 'A' is represented by this matrix and Y is a row vector defining the results, this method will
     * return approximate values for 'X'.
     *
     * \param[in] a The A matrix to use to solve for x.
     *
     * \param[in] y The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'X'.  An empty matrix will be returned if A is singular.
     */
    M_PUBLIC_API Variant solve(const Variant& a, const MatrixInteger& y);

    /**
     * Method that solves the system of linear equations specified by this matrix.  For a system of linear
     * equations defined by:
     *
     * \f[
     *     A \times X = Y
     * \f]
     *
     * Where 'A' is represented by this matrix and Y is a row vector defining the results, this method will
     * return approximate values for 'X'.
     *
     * \param[in] a The A matrix to use to solve for x.
     *
     * \param[in] y The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'X'.  An empty matrix will be returned if A is singular.
     */
    M_PUBLIC_API Variant solve(const Variant& a, const MatrixReal& y);

    /**
     * Method that solves the system of linear equations specified by this matrix.  For a system of linear
     * equations defined by:
     *
     * \f[
     *     A \times X = Y
     * \f]
     *
     * Where 'A' is represented by this matrix and Y is a row vector defining the results, this method will
     * return approximate values for 'X'.
     *
     * \param[in] a The A matrix to use to solve for x.
     *
     * \param[in] y The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'X'.  An empty matrix will be returned if A is singular.
     */
    M_PUBLIC_API MatrixComplex solve(const Variant& a, const MatrixComplex& y);

    /**
     * Method that solves the system of linear equations specified by this matrix.  For a system of linear
     * equations defined by:
     *
     * \f[
     *     A \times X = Y
     * \f]
     *
     * Where 'A' is represented by this matrix and Y is a row vector defining the results, this method will
     * return approximate values for 'X'.
     *
     * \param[in] a The A matrix to use to solve for x.
     *
     * \param[in] y The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'X'.  An empty matrix will be returned if A is singular.
     */
    M_PUBLIC_API Variant solve(const MatrixBoolean& a, const Variant& y);

    /**
     * Method that solves the system of linear equations specified by this matrix.  For a system of linear
     * equations defined by:
     *
     * \f[
     *     A \times X = Y
     * \f]
     *
     * Where 'A' is represented by this matrix and Y is a row vector defining the results, this method will
     * return approximate values for 'X'.
     *
     * \param[in] a The A matrix to use to solve for x.
     *
     * \param[in] y The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'X'.  An empty matrix will be returned if A is singular.
     */
    M_PUBLIC_API Variant solve(const MatrixInteger& a, const Variant& y);

    /**
     * Method that solves the system of linear equations specified by this matrix.  For a system of linear
     * equations defined by:
     *
     * \f[
     *     A \times X = Y
     * \f]
     *
     * Where 'A' is represented by this matrix and Y is a row vector defining the results, this method will
     * return approximate values for 'X'.
     *
     * \param[in] a The A matrix to use to solve for x.
     *
     * \param[in] y The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'X'.  An empty matrix will be returned if A is singular.
     */
    M_PUBLIC_API Variant solve(const MatrixReal& a, const Variant& y);

    /**
     * Method that solves the system of linear equations specified by this matrix.  For a system of linear
     * equations defined by:
     *
     * \f[
     *     A \times X = Y
     * \f]
     *
     * Where 'A' is represented by this matrix and Y is a row vector defining the results, this method will
     * return approximate values for 'X'.
     *
     * \param[in] a The A matrix to use to solve for x.
     *
     * \param[in] y The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'X'.  An empty matrix will be returned if A is singular.
     */
    M_PUBLIC_API MatrixComplex solve(const MatrixComplex& a, const Variant& y);

    /**
     * Method that solves the system of linear equations specified by this matrix.  For a system of linear
     * equations defined by:
     *
     * \f[
     *     A \times X = Y
     * \f]
     *
     * Where 'A' is represented by this matrix and Y is a row vector defining the results, this method will
     * return approximate values for 'X'.
     *
     * \param[in] a The A matrix to use to solve for x.
     *
     * \param[in] y The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'X'.  An empty matrix will be returned if A is singular.
     */
    M_PUBLIC_API Variant solve(const Variant& a, const Variant& y);

    /**
     * Method that solves and over/under determined system using QR or LQ factorization.  When provided an
     * under-determined matrix, this method will determine the minimum Euclidean-norm solution.
     *
     * For over-determined systems, this method will find the solution that exactly satisfies \f[ Ax = b \f].  For
     * under-determined system, this method will find the solution that identifies the value of \f[ x \f] that
     * minimizes \f[ \Vert\Vert A x - b \Vert\Vert _ 2 \f].
     *
     * \param[in] a The A matrix from above.
     *
     * \param[in] b The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'x'.  An empty matrix will be returned if this matrix is not
     *         full rank.
     */
    M_PUBLIC_API inline MatrixReal leastSquares(const MatrixBoolean& a, const MatrixBoolean& b) {
        return MatrixReal(a).leastSquares(MatrixReal(b));
    }

    /**
     * Method that solves and over/under determined system using QR or LQ factorization.  When provided an
     * under-determined matrix, this method will determine the minimum Euclidean-norm solution.
     *
     * For over-determined systems, this method will find the solution that exactly satisfies \f[ Ax = b \f].  For
     * under-determined system, this method will find the solution that identifies the value of \f[ x \f] that
     * minimizes \f[ \Vert\Vert A x - b \Vert\Vert _ 2 \f].
     *
     * \param[in] a The A matrix from above.
     *
     * \param[in] b The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'x'.  An empty matrix will be returned if this matrix is not
     *         full rank.
     */
    M_PUBLIC_API inline MatrixReal leastSquares(const MatrixBoolean& a, const MatrixInteger& b) {
        return MatrixReal(a).leastSquares(MatrixReal(b));
    }

    /**
     * Method that solves and over/under determined system using QR or LQ factorization.  When provided an
     * under-determined matrix, this method will determine the minimum Euclidean-norm solution.
     *
     * For over-determined systems, this method will find the solution that exactly satisfies \f[ Ax = b \f].  For
     * under-determined system, this method will find the solution that identifies the value of \f[ x \f] that
     * minimizes \f[ \Vert\Vert A x - b \Vert\Vert _ 2 \f].
     *
     * \param[in] a The A matrix from above.
     *
     * \param[in] b The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'x'.  An empty matrix will be returned if this matrix is not
     *         full rank.
     */
    M_PUBLIC_API inline MatrixReal leastSquares(const MatrixBoolean& a, const MatrixReal& b) {
        return MatrixReal(a).leastSquares(b);
    }

    /**
     * Method that solves and over/under determined system using QR or LQ factorization.  When provided an
     * under-determined matrix, this method will determine the minimum Euclidean-norm solution.
     *
     * For over-determined systems, this method will find the solution that exactly satisfies \f[ Ax = b \f].  For
     * under-determined system, this method will find the solution that identifies the value of \f[ x \f] that
     * minimizes \f[ \Vert\Vert A x - b \Vert\Vert _ 2 \f].
     *
     * \param[in] a The A matrix from above.
     *
     * \param[in] b The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'x'.  An empty matrix will be returned if this matrix is not
     *         full rank.
     */
    M_PUBLIC_API inline MatrixComplex leastSquares(const MatrixBoolean& a, const MatrixComplex& b) {
        return MatrixComplex(a).leastSquares(b);
    }

    /**
     * Method that solves and over/under determined system using QR or LQ factorization.  When provided an
     * under-determined matrix, this method will determine the minimum Euclidean-norm solution.
     *
     * For over-determined systems, this method will find the solution that exactly satisfies \f[ Ax = b \f].  For
     * under-determined system, this method will find the solution that identifies the value of \f[ x \f] that
     * minimizes \f[ \Vert\Vert A x - b \Vert\Vert _ 2 \f].
     *
     * \param[in] a The A matrix from above.
     *
     * \param[in] b The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'x'.  An empty matrix will be returned if this matrix is not
     *         full rank.
     */
    M_PUBLIC_API inline MatrixReal leastSquares(const MatrixInteger& a, const MatrixBoolean& b) {
        return MatrixReal(a).leastSquares(MatrixReal(b));
    }

    /**
     * Method that solves and over/under determined system using QR or LQ factorization.  When provided an
     * under-determined matrix, this method will determine the minimum Euclidean-norm solution.
     *
     * For over-determined systems, this method will find the solution that exactly satisfies \f[ Ax = b \f].  For
     * under-determined system, this method will find the solution that identifies the value of \f[ x \f] that
     * minimizes \f[ \Vert\Vert A x - b \Vert\Vert _ 2 \f].
     *
     * \param[in] a The A matrix from above.
     *
     * \param[in] b The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'x'.  An empty matrix will be returned if this matrix is not
     *         full rank.
     */
    M_PUBLIC_API inline MatrixReal leastSquares(const MatrixInteger& a, const MatrixInteger& b) {
        return MatrixReal(a).leastSquares(MatrixReal(b));
    }

    /**
     * Method that solves and over/under determined system using QR or LQ factorization.  When provided an
     * under-determined matrix, this method will determine the minimum Euclidean-norm solution.
     *
     * For over-determined systems, this method will find the solution that exactly satisfies \f[ Ax = b \f].  For
     * under-determined system, this method will find the solution that identifies the value of \f[ x \f] that
     * minimizes \f[ \Vert\Vert A x - b \Vert\Vert _ 2 \f].
     *
     * \param[in] a The A matrix from above.
     *
     * \param[in] b The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'x'.  An empty matrix will be returned if this matrix is not
     *         full rank.
     */
    M_PUBLIC_API inline MatrixReal leastSquares(const MatrixInteger& a, const MatrixReal& b) {
        return MatrixReal(a).leastSquares(b);
    }

    /**
     * Method that solves and over/under determined system using QR or LQ factorization.  When provided an
     * under-determined matrix, this method will determine the minimum Euclidean-norm solution.
     *
     * For over-determined systems, this method will find the solution that exactly satisfies \f[ Ax = b \f].  For
     * under-determined system, this method will find the solution that identifies the value of \f[ x \f] that
     * minimizes \f[ \Vert\Vert A x - b \Vert\Vert _ 2 \f].
     *
     * \param[in] a The A matrix from above.
     *
     * \param[in] b The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'x'.  An empty matrix will be returned if this matrix is not
     *         full rank.
     */
    M_PUBLIC_API inline MatrixComplex leastSquares(const MatrixInteger& a, const MatrixComplex& b) {
        return MatrixComplex(a).leastSquares(b);
    }

    /**
     * Method that solves and over/under determined system using QR or LQ factorization.  When provided an
     * under-determined matrix, this method will determine the minimum Euclidean-norm solution.
     *
     * For over-determined systems, this method will find the solution that exactly satisfies \f[ Ax = b \f].  For
     * under-determined system, this method will find the solution that identifies the value of \f[ x \f] that
     * minimizes \f[ \Vert\Vert A x - b \Vert\Vert _ 2 \f].
     *
     * \param[in] a The A matrix from above.
     *
     * \param[in] b The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'x'.  An empty matrix will be returned if this matrix is not
     *         full rank.
     */
    M_PUBLIC_API inline MatrixReal leastSquares(const MatrixReal& a, const MatrixBoolean& b) {
        return a.leastSquares(MatrixReal(b));
    }

    /**
     * Method that solves and over/under determined system using QR or LQ factorization.  When provided an
     * under-determined matrix, this method will determine the minimum Euclidean-norm solution.
     *
     * For over-determined systems, this method will find the solution that exactly satisfies \f[ Ax = b \f].  For
     * under-determined system, this method will find the solution that identifies the value of \f[ x \f] that
     * minimizes \f[ \Vert\Vert A x - b \Vert\Vert _ 2 \f].
     *
     * \param[in] a The A matrix from above.
     *
     * \param[in] b The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'x'.  An empty matrix will be returned if this matrix is not
     *         full rank.
     */
    M_PUBLIC_API inline MatrixReal leastSquares(const MatrixReal& a, const MatrixInteger& b) {
        return a.leastSquares(MatrixReal(b));
    }

    /**
     * Method that solves and over/under determined system using QR or LQ factorization.  When provided an
     * under-determined matrix, this method will determine the minimum Euclidean-norm solution.
     *
     * For over-determined systems, this method will find the solution that exactly satisfies \f[ Ax = b \f].  For
     * under-determined system, this method will find the solution that identifies the value of \f[ x \f] that
     * minimizes \f[ \Vert\Vert A x - b \Vert\Vert _ 2 \f].
     *
     * \param[in] a The A matrix from above.
     *
     * \param[in] b The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'x'.  An empty matrix will be returned if this matrix is not
     *         full rank.
     */
    M_PUBLIC_API inline MatrixReal leastSquares(const MatrixReal& a, const MatrixReal& b) {
        return a.leastSquares(b);
    }

    /**
     * Method that solves and over/under determined system using QR or LQ factorization.  When provided an
     * under-determined matrix, this method will determine the minimum Euclidean-norm solution.
     *
     * For over-determined systems, this method will find the solution that exactly satisfies \f[ Ax = b \f].  For
     * under-determined system, this method will find the solution that identifies the value of \f[ x \f] that
     * minimizes \f[ \Vert\Vert A x - b \Vert\Vert _ 2 \f].
     *
     * \param[in] a The A matrix from above.
     *
     * \param[in] b The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'x'.  An empty matrix will be returned if this matrix is not
     *         full rank.
     */
    M_PUBLIC_API inline MatrixComplex leastSquares(const MatrixReal& a, const MatrixComplex& b) {
        return MatrixComplex(a).leastSquares(b);
    }

    /**
     * Method that solves and over/under determined system using QR or LQ factorization.  When provided an
     * under-determined matrix, this method will determine the minimum Euclidean-norm solution.
     *
     * For over-determined systems, this method will find the solution that exactly satisfies \f[ Ax = b \f].  For
     * under-determined system, this method will find the solution that identifies the value of \f[ x \f] that
     * minimizes \f[ \Vert\Vert A x - b \Vert\Vert _ 2 \f].
     *
     * \param[in] a The A matrix from above.
     *
     * \param[in] b The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'x'.  An empty matrix will be returned if this matrix is not
     *         full rank.
     */
    M_PUBLIC_API inline MatrixComplex leastSquares(const MatrixComplex& a, const MatrixBoolean& b) {
        return a.leastSquares(MatrixComplex(b));
    }

    /**
     * Method that solves and over/under determined system using QR or LQ factorization.  When provided an
     * under-determined matrix, this method will determine the minimum Euclidean-norm solution.
     *
     * For over-determined systems, this method will find the solution that exactly satisfies \f[ Ax = b \f].  For
     * under-determined system, this method will find the solution that identifies the value of \f[ x \f] that
     * minimizes \f[ \Vert\Vert A x - b \Vert\Vert _ 2 \f].
     *
     * \param[in] a The A matrix from above.
     *
     * \param[in] b The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'x'.  An empty matrix will be returned if this matrix is not
     *         full rank.
     */
    M_PUBLIC_API inline MatrixComplex leastSquares(const MatrixComplex& a, const MatrixInteger& b) {
        return a.leastSquares(MatrixComplex(b));
    }

    /**
     * Method that solves and over/under determined system using QR or LQ factorization.  When provided an
     * under-determined matrix, this method will determine the minimum Euclidean-norm solution.
     *
     * For over-determined systems, this method will find the solution that exactly satisfies \f[ Ax = b \f].  For
     * under-determined system, this method will find the solution that identifies the value of \f[ x \f] that
     * minimizes \f[ \Vert\Vert A x - b \Vert\Vert _ 2 \f].
     *
     * \param[in] a The A matrix from above.
     *
     * \param[in] b The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'x'.  An empty matrix will be returned if this matrix is not
     *         full rank.
     */
    M_PUBLIC_API inline MatrixComplex leastSquares(const MatrixComplex& a, const MatrixReal& b) {
        return a.leastSquares(MatrixComplex(b));
    }

    /**
     * Method that solves and over/under determined system using QR or LQ factorization.  When provided an
     * under-determined matrix, this method will determine the minimum Euclidean-norm solution.
     *
     * For over-determined systems, this method will find the solution that exactly satisfies \f[ Ax = b \f].  For
     * under-determined system, this method will find the solution that identifies the value of \f[ x \f] that
     * minimizes \f[ \Vert\Vert A x - b \Vert\Vert _ 2 \f].
     *
     * \param[in] a The A matrix from above.
     *
     * \param[in] b The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'x'.  An empty matrix will be returned if this matrix is not
     *         full rank.
     */
    M_PUBLIC_API inline MatrixComplex leastSquares(const MatrixComplex& a, const MatrixComplex& b) {
        return a.leastSquares(b);
    }

    /**
     * Method that solves and over/under determined system using QR or LQ factorization.  When provided an
     * under-determined matrix, this method will determine the minimum Euclidean-norm solution.
     *
     * For over-determined systems, this method will find the solution that exactly satisfies \f[ Ax = b \f].  For
     * under-determined system, this method will find the solution that identifies the value of \f[ x \f] that
     * minimizes \f[ \Vert\Vert A x - b \Vert\Vert _ 2 \f].
     *
     * \param[in] a The A matrix from above.
     *
     * \param[in] b The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'x'.  An empty matrix will be returned if this matrix is not
     *         full rank.
     */
    M_PUBLIC_API Variant leastSquares(const Variant& a, const MatrixBoolean& b);

    /**
     * Method that solves and over/under determined system using QR or LQ factorization.  When provided an
     * under-determined matrix, this method will determine the minimum Euclidean-norm solution.
     *
     * For over-determined systems, this method will find the solution that exactly satisfies \f[ Ax = b \f].  For
     * under-determined system, this method will find the solution that identifies the value of \f[ x \f] that
     * minimizes \f[ \Vert\Vert A x - b \Vert\Vert _ 2 \f].
     *
     * \param[in] a The A matrix from above.
     *
     * \param[in] b The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'x'.  An empty matrix will be returned if this matrix is not
     *         full rank.
     */
    M_PUBLIC_API Variant leastSquares(const Variant& a, const MatrixInteger& b);

    /**
     * Method that solves and over/under determined system using QR or LQ factorization.  When provided an
     * under-determined matrix, this method will determine the minimum Euclidean-norm solution.
     *
     * For over-determined systems, this method will find the solution that exactly satisfies \f[ Ax = b \f].  For
     * under-determined system, this method will find the solution that identifies the value of \f[ x \f] that
     * minimizes \f[ \Vert\Vert A x - b \Vert\Vert _ 2 \f].
     *
     * \param[in] a The A matrix from above.
     *
     * \param[in] b The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'x'.  An empty matrix will be returned if this matrix is not
     *         full rank.
     */
    M_PUBLIC_API Variant leastSquares(const Variant& a, const MatrixReal& b);
    /**
     * Method that solves and over/under determined system using QR or LQ factorization.  When provided an
     * under-determined matrix, this method will determine the minimum Euclidean-norm solution.
     *
     * For over-determined systems, this method will find the solution that exactly satisfies \f[ Ax = b \f].  For
     * under-determined system, this method will find the solution that identifies the value of \f[ x \f] that
     * minimizes \f[ \Vert\Vert A x - b \Vert\Vert _ 2 \f].
     *
     * \param[in] a The A matrix from above.
     *
     * \param[in] b The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'x'.  An empty matrix will be returned if this matrix is not
     *         full rank.
     */
    M_PUBLIC_API MatrixComplex leastSquares(const Variant& a, const MatrixComplex& b);

    /**
     * Method that solves and over/under determined system using QR or LQ factorization.  When provided an
     * under-determined matrix, this method will determine the minimum Euclidean-norm solution.
     *
     * For over-determined systems, this method will find the solution that exactly satisfies \f[ Ax = b \f].  For
     * under-determined system, this method will find the solution that identifies the value of \f[ x \f] that
     * minimizes \f[ \Vert\Vert A x - b \Vert\Vert _ 2 \f].
     *
     * \param[in] a The A matrix from above.
     *
     * \param[in] b The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'x'.  An empty matrix will be returned if this matrix is not
     *         full rank.
     */
    M_PUBLIC_API Variant leastSquares(const MatrixBoolean& a, const Variant& b);

    /**
     * Method that solves and over/under determined system using QR or LQ factorization.  When provided an
     * under-determined matrix, this method will determine the minimum Euclidean-norm solution.
     *
     * For over-determined systems, this method will find the solution that exactly satisfies \f[ Ax = b \f].  For
     * under-determined system, this method will find the solution that identifies the value of \f[ x \f] that
     * minimizes \f[ \Vert\Vert A x - b \Vert\Vert _ 2 \f].
     *
     * \param[in] a The A matrix from above.
     *
     * \param[in] b The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'x'.  An empty matrix will be returned if this matrix is not
     *         full rank.
     */
    M_PUBLIC_API Variant leastSquares(const MatrixInteger& a, const Variant& b);

    /**
     * Method that solves and over/under determined system using QR or LQ factorization.  When provided an
     * under-determined matrix, this method will determine the minimum Euclidean-norm solution.
     *
     * For over-determined systems, this method will find the solution that exactly satisfies \f[ Ax = b \f].  For
     * under-determined system, this method will find the solution that identifies the value of \f[ x \f] that
     * minimizes \f[ \Vert\Vert A x - b \Vert\Vert _ 2 \f].
     *
     * \param[in] a The A matrix from above.
     *
     * \param[in] b The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'x'.  An empty matrix will be returned if this matrix is not
     *         full rank.
     */
    M_PUBLIC_API Variant leastSquares(const MatrixReal& a, const Variant& b);

    /**
     * Method that solves and over/under determined system using QR or LQ factorization.  When provided an
     * under-determined matrix, this method will determine the minimum Euclidean-norm solution.
     *
     * For over-determined systems, this method will find the solution that exactly satisfies \f[ Ax = b \f].  For
     * under-determined system, this method will find the solution that identifies the value of \f[ x \f] that
     * minimizes \f[ \Vert\Vert A x - b \Vert\Vert _ 2 \f].
     *
     * \param[in] a The A matrix from above.
     *
     * \param[in] b The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'x'.  An empty matrix will be returned if this matrix is not
     *         full rank.
     */
    M_PUBLIC_API MatrixComplex leastSquares(const MatrixComplex& a, const Variant& b);

    /**
     * Method that solves and over/under determined system using QR or LQ factorization.  When provided an
     * under-determined matrix, this method will determine the minimum Euclidean-norm solution.
     *
     * For over-determined systems, this method will find the solution that exactly satisfies \f[ Ax = b \f].  For
     * under-determined system, this method will find the solution that identifies the value of \f[ x \f] that
     * minimizes \f[ \Vert\Vert A x - b \Vert\Vert _ 2 \f].
     *
     * \param[in] a The A matrix from above.
     *
     * \param[in] b The row matrix holding the results of the product \f[ A \times X \f].
     *
     * \return Returns the resulting values of 'x'.  An empty matrix will be returned if this matrix is not
     *         full rank.
     */
    M_PUBLIC_API Variant leastSquares(const Variant& a, const Variant& b);

    /**
     * Method that returns the real portion of this matrix.
     *
     * \param[in] matrix The matrix to obtain the real value from..
     *
     * \return Returns a real matrix holding the real values of each coefficient.
     */
    M_PUBLIC_API inline MatrixBoolean real(const MatrixBoolean& matrix) {
        return matrix;
    }

    /**
     * Method that returns the real portion of this matrix.
     *
     * \param[in] matrix The matrix to obtain the real value from..
     *
     * \return Returns a real matrix holding the real values of each coefficient.
     */
    M_PUBLIC_API inline MatrixInteger real(const MatrixInteger& matrix) {
        return matrix;
    }

    /**
     * Method that returns the real portion of this matrix.
     *
     * \param[in] matrix The matrix to obtain the real value from..
     *
     * \return Returns a real matrix holding the real values of each coefficient.
     */
    M_PUBLIC_API inline MatrixReal real(const MatrixReal& matrix) {
        return matrix;
    }

    /**
     * Method that returns the real portion of this matrix.
     *
     * \param[in] matrix The matrix to obtain the real value from..
     *
     * \return Returns a real matrix holding the real values of each coefficient.
     */
    M_PUBLIC_API inline MatrixReal real(const MatrixComplex& matrix) {
        return matrix.real();
    }

    /**
     * Method that returns the imaginary portion of this matrix.
     *
     * \param[in] matrix The matrix to obtain the imaginary value from..
     *
     * \return Returns a real matrix holding the imaginary values of each coefficient.
     */
    M_PUBLIC_API inline MatrixBoolean imag(const MatrixBoolean& matrix) {
        return MatrixBoolean::zero(matrix.numberRows(), matrix.numberColumns());
    }

    /**
     * Method that returns the imaginary portion of this matrix.
     *
     * \param[in] matrix The matrix to obtain the imaginary value from..
     *
     * \return Returns a real matrix holding the imaginary values of each coefficient.
     */
    M_PUBLIC_API inline MatrixInteger imag(const MatrixInteger& matrix) {
        return MatrixInteger::zero(matrix.numberRows(), matrix.numberColumns());
    }

    /**
     * Method that returns the imaginary portion of this matrix.
     *
     * \param[in] matrix The matrix to obtain the imaginary value from..
     *
     * \return Returns a real matrix holding the imaginary values of each coefficient.
     */
    M_PUBLIC_API inline MatrixReal imag(const MatrixReal& matrix) {
        return MatrixReal::zero(matrix.numberRows(), matrix.numberColumns());
    }

    /**
     * Method that returns the imaginary portion of this matrix.
     *
     * \param[in] matrix The matrix to obtain the imaginary value from..
     *
     * \return Returns a real matrix holding the imaginary values of each coefficient.
     */
    M_PUBLIC_API inline MatrixReal imag(const MatrixComplex& matrix) {
        return matrix.imag();
    }
}


#endif
