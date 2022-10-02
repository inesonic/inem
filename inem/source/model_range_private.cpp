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
* This file implements the \ref Model::Range::Private class as well as several associated classes.
***********************************************************************************************************************/

#include <cmath>
#include <limits>

#include "model_intrinsic_types.h"
#include "m_variant.h"
#include "model_range.h"
#include "model_range_private.h"

/***********************************************************************************************************************
 * Model::RangePrivateBase:
 */

namespace Model {
    const M::Variant RangePrivateBase::dummyVariant;

    RangePrivateBase::RangePrivateBase() {}


    RangePrivateBase::~RangePrivateBase() {}


    RangePrivateBase* RangePrivateBase::clone() const {
        return new RangePrivateBase;
    }


    bool RangePrivateBase::isEmpty() const {
        return true;
    }


    Model::Integer RangePrivateBase::size() const {
        return 0;
    }


    const M::Variant& RangePrivateBase::first() const {
        return dummyVariant;
    }


    const M::Variant& RangePrivateBase::second() const {
        return dummyVariant;
    }


    const M::Variant& RangePrivateBase::last() const {
        return dummyVariant;
    }


    bool RangePrivateBase::contains(const M::Variant& /* value */) const {
        return false;
    }


    M::Variant RangePrivateBase::valueAtIndex(unsigned long long /* index */) const {
        return M::Variant();
    }
}

/***********************************************************************************************************************
 * Model::RangePrivateBase:
 */

namespace Model {
    Range::Private::Private() {}


    Range::Private::~Private() {}
}
