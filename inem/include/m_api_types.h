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
* This header defines various types required by the model internal API.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_API_TYPES_H
#define M_API_TYPES_H

#include "model_api_types.h"

namespace M {
    /**
     * Type used to represent an operation handle.
     */
    typedef Model::OperationHandle OperationHandle;

    /**
     * Type used to represent an identifier handle.
     */
    typedef Model::IdentifierHandle IdentifierHandle;

    /**
     * Enumeration of supported base types.
     */
    typedef Model::ValueType ValueType;

    /**
     * Enumeration of supported I/O devices.
     */
    typedef Model::Device Device;
}

#endif
