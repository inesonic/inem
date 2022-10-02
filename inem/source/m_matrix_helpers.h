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
* This header defines a collection of helper functions used by different matrices.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_MATRIX_HELPERS_H
#define M_MATRIX_HELPERS_H

#include "m_common.h"
#include "m_api_types.h"
#include "m_type_conversion.h"
#include "m_intrinsics.h"
#include "m_intrinsic_types.h"

namespace M {
    /**
     * Method you can use to obtain a submatrix of this matrix, by iterable values.  This method will throw an
     * exception if the specified row iterable or column is invalid.
     *
     * \param[in] sourceMatrix The matrix we're extracting from.
     *
     * \param[in] rowIterable  The iterable to use to create the submatrix.
     *
     * \param[in] column       The column to use to create the submatrix.
     *
     * \return Returns the desired submatrix.
     */
    template<typename SM, typename RI> SM atICHelper(
            const SM& sourceMatrix,
            const RI& rowIterable,
            Integer   column
        ) {
        Integer destinationNumberRows = rowIterable.size();
        SM      result(destinationNumberRows, 1);
        Integer rowIndex = 1;
        for (  typename RI::ConstIterator it  = rowIterable.constBegin(),
                                          end = rowIterable.constEnd()
             ; it!=end
             ; ++it
            ) {
            Integer sourceRowIndex = it->toInteger();
            result.update(rowIndex, Integer(1), sourceMatrix.at(sourceRowIndex, column));
            ++rowIndex;
        }

        return result;
    }

    /**
     * Method you can use to obtain a submatrix of this matrix, by iterable values.  This method will throw an
     * exception if the specified row or column iterable is invalid.
     *
     * \param[in] sourceMatrix   The matrix we're extracting from.
     *
     * \param[in] row            The row to use to create the submatrix.
     *
     * \param[in] columnIterable The iterable to use to create the submatrix.
     *
     * \return Returns the desired submatrix.
     */
    template<typename SM, typename CI> SM atCIHelper(
            const SM& sourceMatrix,
            Integer   row,
            const CI& columnIterable
        ) {
        Integer destinationNumberColumns = columnIterable.size();
        SM      result(1, destinationNumberColumns);
        Integer columnIndex = 1;
        for (  typename CI::ConstIterator it  = columnIterable.constBegin(),
                                          end = columnIterable.constEnd()
             ; it!=end
             ; ++it
            ) {
            Integer sourceColumnIndex = it->toInteger();
            result.update(Integer(1), columnIndex, sourceMatrix.at(row, sourceColumnIndex));
            ++columnIndex;
        }

        return result;
    }

    /**
     * Method you can use to obtain a submatrix of this matrix, by iterable values.  This method will throw an
     * exception if the specified row iterable or column iterable is invalid.
     *
     * \param[in] sourceMatrix   The matrix we're extracting from.
     *
     * \param[in] rowIterable    The row to use to create the submatrix.
     *
     * \param[in] columnIterable The iterable to use to create the submatrix.
     *
     * \return Returns the desired submatrix.
     */
    template<typename SM, typename RI, typename CI> SM atIIHelper(
            const SM& sourceMatrix,
            const RI& rowIterable,
            const CI& columnIterable
        ) {
        Integer destinationNumberRows    = rowIterable.size();
        Integer destinationNumberColumns = columnIterable.size();

        SM result(destinationNumberRows, destinationNumberColumns);

        Integer columnIndex = 1;
        for (  typename CI::ConstIterator columnIterator    = columnIterable.constBegin(),
                                          columnEndIterator = columnIterable.constEnd()
             ; columnIterator != columnEndIterator
             ; ++columnIterator
            ) {
            Integer sourceColumnIndex   = columnIterator->toInteger();
            Integer rowIndex = 1;

            for (  typename RI::ConstIterator rowIterator    = rowIterable.constBegin(),
                                              rowEndIterator = rowIterable.constEnd()
                 ; rowIterator != rowEndIterator
                 ; ++rowIterator
                ) {
                Integer sourceRowIndex = rowIterator->toInteger();
                result.update(rowIndex, columnIndex, sourceMatrix.at(sourceRowIndex, sourceColumnIndex));

                ++rowIndex;
            }

            ++columnIndex;
        }

        return result;
    }

    /**
     * Method you can use to obtain a submatrix of this matrix, by iterable values.  This method will throw an
     * exception if the specified row iterable or column iterable is invalid.
     *
     * \param[in] sourceMatrix The matrix we're extracting from.
     *
     * \param[in] iterable     The iterable used to create the submatrix.
     *
     * \return Returns the desired submatrix.
     */
    template<typename SM, typename I> SM atIHelper(const SM& sourceMatrix, const I& iterable) {
        SM result(iterable.size(), 1);

        Integer rowIndex = 1;
        for (typename I::ConstIterator it=iterable.constBegin(),end=iterable.constEnd() ; it!=end ; ++it) {
            Integer sourceIndex = it->toInteger();
            result.update(rowIndex, Integer(1), sourceMatrix.at(sourceIndex));
            ++rowIndex;
        }

        return result;
    }

    /**
     * Method you can use to obtain a submatrix of this matrix, by iterable values.  This method will throw an
     * exception if the specified row iterable or column is invalid.
     *
     * \param[in] sourceMatrix The matrix we're extracting from.
     *
     * \param[in] rowMatrix    The iterable to use to create the submatrix.
     *
     * \param[in] column       The matrix to use to create the submatrix.
     *
     * \return Returns the desired submatrix.
     */
    template<typename SM, typename RM> SM atMCHelper(
            const SM& sourceMatrix,
            const RM& rowMatrix,
            Integer   column
        ) {
        Integer destinationNumberRows = rowMatrix.numberCoefficients();
        SM      result(destinationNumberRows, 1);
        for (Integer rowIndex=1 ; rowIndex<=destinationNumberRows ; ++rowIndex) {
            Integer sourceRowIndex = inlineToInteger(rowMatrix.at(rowIndex));
            result.update(rowIndex, Integer(1), sourceMatrix.at(sourceRowIndex, column));
        }

        return result;
    }

    /**
     * Method you can use to obtain a submatrix of this matrix, by iterable values.  This method will throw an
     * exception if the specified row or column iterable is invalid.
     *
     * \param[in] sourceMatrix The matrix we're extracting from.
     *
     * \param[in] row          The row to use to create the submatrix.
     *
     * \param[in] columnMatrix The matrix to use to create the submatrix.
     *
     * \return Returns the desired submatrix.
     */
    template<typename SM, typename CM> SM atCMHelper(
            const SM& sourceMatrix,
            Integer   row,
            const CM& columnMatrix
        ) {
        Integer destinationNumberColumns = columnMatrix.numberCoefficients();
        SM      result(1, destinationNumberColumns);
        for (Integer columnIndex=1 ; columnIndex<=destinationNumberColumns ; ++columnIndex) {
            Integer sourceColumnIndex = inlineToInteger(columnMatrix.at(columnIndex));
            result.update(Integer(1), columnIndex, sourceMatrix.at(row, sourceColumnIndex));
        }

        return result;
    }

    /**
     * Method you can use to obtain a submatrix of this matrix, by iterable values.  This method will throw an
     * exception if the specified row iterable or column iterable is invalid.
     *
     * \param[in] sourceMatrix The matrix we're extracting from.
     *
     * \param[in] rowIterable  The row to use to create the submatrix.
     *
     * \param[in] columnMatrix The iterable to use to create the submatrix.
     *
     * \return Returns the desired submatrix.
     */
    template<typename SM, typename RI, typename CM> SM atIMHelper(
            const SM& sourceMatrix,
            const RI& rowIterable,
            const CM& columnMatrix
        ) {
        Integer destinationNumberRows    = rowIterable.size();
        Integer destinationNumberColumns = columnMatrix.numberCoefficients();

        SM result(destinationNumberRows, destinationNumberColumns);

        for (Integer columnIndex=1 ; columnIndex<=destinationNumberColumns ; ++columnIndex) {
            Integer sourceColumnIndex = inlineToInteger(columnMatrix.at(columnIndex));
            Integer rowIndex          = 1;

            for (  typename RI::ConstIterator rowIterator    = rowIterable.constBegin(),
                                              rowEndIterator = rowIterable.constEnd()
                 ; rowIterator != rowEndIterator
                 ; ++rowIterator
                ) {
                Integer sourceRowIndex = rowIterator->toInteger();
                result.update(rowIndex, columnIndex, sourceMatrix.at(sourceRowIndex, sourceColumnIndex));

                ++rowIndex;
            }
        }

        return result;
    }

    /**
     * Method you can use to obtain a submatrix of this matrix, by iterable values.  This method will throw an
     * exception if the specified row iterable or column iterable is invalid.
     *
     * \param[in] sourceMatrix   The matrix we're extracting from.
     *
     * \param[in] rowMatrix      The row to use to create the submatrix.
     *
     * \param[in] columnIterable The iterable to use to create the submatrix.
     *
     * \return Returns the desired submatrix.
     */
    template<typename SM, typename RM, typename CI> SM atMIHelper(
            const SM& sourceMatrix,
            const RM& rowMatrix,
            const CI& columnIterable
        ) {
        Integer destinationNumberRows    = rowMatrix.numberCoefficients();
        Integer destinationNumberColumns = columnIterable.size();

        SM result(destinationNumberRows, destinationNumberColumns);

        Integer columnIndex = 1;
        for (  typename CI::ConstIterator columnIterator    = columnIterable.constBegin(),
                                          columnEndIterator = columnIterable.constEnd()
             ; columnIterator != columnEndIterator
             ; ++columnIterator
            ) {
            Integer sourceColumnIndex   = columnIterator->toInteger();
            for (typename RM::Index rowIndex=1 ; rowIndex<=destinationNumberRows ; ++rowIndex) {
                Integer sourceRowIndex = inlineToInteger(rowMatrix.at(rowIndex));
                result.update(rowIndex, columnIndex, sourceMatrix.at(sourceRowIndex, sourceColumnIndex));
            }

            ++columnIndex;
        }

        return result;
    }

    /**
     * Method you can use to obtain a submatrix of this matrix, by iterable values.  This method will throw an
     * exception if the specified row iterable or column iterable is invalid.
     *
     * \param[in] sourceMatrix The matrix we're extracting from.
     *
     * \param[in] rowMatrix    The row matrix to use to create the submatrix.
     *
     * \param[in] columnMatrix The column matrix to use to create the submatrix.
     *
     * \return Returns the desired submatrix.
     */
    template<typename SM, typename RM, typename CM> SM atMMHelper(
            const SM& sourceMatrix,
            const RM& rowMatrix,
            const CM& columnMatrix
        ) {
        Integer destinationNumberRows    = rowMatrix.numberCoefficients();
        Integer destinationNumberColumns = columnMatrix.numberCoefficients();

        SM result(destinationNumberRows, destinationNumberColumns);

        for (Integer columnIndex=1 ; columnIndex<=destinationNumberColumns ; ++columnIndex) {
            Integer sourceColumnIndex = inlineToInteger(columnMatrix.at(columnIndex));
            for (Integer rowIndex=1 ; rowIndex<=destinationNumberRows ; ++rowIndex) {
                Integer sourceRowIndex = inlineToInteger(rowMatrix.at(rowIndex));
                result.update(rowIndex, columnIndex, sourceMatrix.at(sourceRowIndex, sourceColumnIndex));
            }
        }

        return result;
    }

    /**
     * Method you can use to obtain a submatrix of this matrix, by iterable values.  This method will throw an
     * exception if the specified row or column iterable is invalid.
     *
     * \param[in] sourceMatrix The matrix we're extracting from.
     *
     * \param[in] matrix       The matrix to use to create the submatrix.
     *
     * \return Returns the desired submatrix.
     */
    template<typename SM, typename M> SM atMHelper(const SM& sourceMatrix, const M& matrix) {
        Integer destinationNumberCoefficients = matrix.numberCoefficients();
        SM      result(destinationNumberCoefficients, 1);
        for (Integer index=1 ; index<=destinationNumberCoefficients ; ++index) {
            Integer sourceIndex = inlineToInteger(matrix.at(index));
            result.update(index, sourceMatrix.at(sourceIndex));
        }

        return result;
    }
}

#endif
