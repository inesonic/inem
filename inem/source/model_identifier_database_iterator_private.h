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
* This header defines the \ref Model::IdentifierDatabaseIterator::Private class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_IDENTIFIER_DATABASE_ITERATOR_PRIVATE_H
#define MODEL_IDENTIFIER_DATABASE_ITERATOR_PRIVATE_H

#include "model_api_types.h"
#include "model_identifier_data.h"
#include "model_identifier_database_private.h"
#include "model_identifier_database_iterator.h"

namespace Model {
    class IdentifierDatabase;
    class IdentifierData;

    /**
     * Private implementation of the \ref Model::IdentifierDatabaseIterator class.
     */
    class IdentifierDatabaseIterator::Private:public IdentifierDatabase::Private::Iterator {
        public:
            Private();

            /**
             * Constructor
             *
             * \param[in] container The constainer this iterator is traversing.
             *
             * \param[in] iterator  The database iterator to assign to this class.
             */
            Private(
                const IdentifierDatabase::Private::Container* container,
                const IdentifierDatabase::Private::Iterator& iterator
            );

            /**
             * Copy constructor
             *
             * \param other The instance to be copied.
             */
            Private(const Private& other);

            ~Private();

            /**
             * Method you can use to obtain the container associated with this iterator.
             *
             * \return Returns a reference to the container this iterator is operating on.
             */
            const IdentifierDatabase::Private::Container* container() const;

            /**
             * Assignment operator
             *
             * \param[in] iterator The iterator to assign to this iterator.
             *
             * \return Returns a reference to this instance.
             */
            Private& operator=(const Private& iterator);

        private:
            /**
             * The container this iterator is operating over.
             */
            const IdentifierDatabase::Private::Container* currentContainer;
    };
};

#endif
