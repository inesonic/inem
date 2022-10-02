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
* This file implements the \ref Model::Tuple::Private class.
***********************************************************************************************************************/

#include <cmath>
#include <cstring>
#include <limits>
#include <cstdint>


#include "model_intrinsic_types.h"
#include "model_exceptions.h"
#include "model_range_iterator.h"
#include "model_range.h"
#include "model_variant.h"
#include "model_variant_private.h"
#include "m_variant.h"
#include "m_range.h"
#include "m_list.h"

#include "m_reference_counter.h"
#include "model_tuple.h"
#include "model_tuple_private.h"

namespace Model {
    Tuple::Private::Private() {}


    Tuple::Private::Private(const char* str) {
        unsigned      bytesRemainingThisEncode = 0;
        std::uint32_t unicode                  = 0;
        const char*   c                        = str;

        while (*c != 0) {
            std::uint8_t chr = static_cast<std::uint8_t>(*c);
            if (bytesRemainingThisEncode == 0) {
                if ((chr & 0x80) == 0) {
                    append(M::Variant(static_cast<Integer>(chr)));
                } else {
                    if ((chr & 0xC0) == 0x80) {
                        throw MalformedString(str, static_cast<unsigned>(c - str));
                    } else {
                        if ((chr & 0xE0) == 0xC0) {
                            unicode                  = chr & 0x1F;
                            bytesRemainingThisEncode = 1;
                        } else if ((chr & 0xF0) == 0xE0) {
                            unicode                  = chr & 0x0F;
                            bytesRemainingThisEncode = 2;
                        } else if ((chr & 0xF8) == 0xF0) {
                            unicode                  = chr & 0x07;
                            bytesRemainingThisEncode = 3;
                        } else {
                            throw MalformedString(str, static_cast<unsigned>(c - str));
                        }
                    }
                }
            } else {
                if ((chr & 0xC0) != 0x80) {
                    throw MalformedString(str, static_cast<unsigned>(c - str));
                } else {
                    unicode = (unicode << 6) | (chr & 0x3F);
                    --bytesRemainingThisEncode;
                    if (bytesRemainingThisEncode == 0) {
                        append(M::Variant(static_cast<Integer>(unicode)));
                    }
                }
            }

            ++c;
        }

        if (bytesRemainingThisEncode != 0) {
            throw MalformedString(str, static_cast<unsigned>(c - str));
        }
    }


    Tuple::Private::Private(const Private& other):M::List<M::Variant>(other) {}


    Tuple::Private::Private(const M::List<M::Variant>& other):M::List<M::Variant>(other) {}


    Tuple::Private::Private(Tuple::Private&& other):M::List<M::Variant>(other) {}


    Tuple::Private::~Private() {}


    void Tuple::Private::append(const Range& range) {
        for (Range::ConstIterator it=range.constBegin(),end=range.constEnd() ; it!=end ; ++it) {
            M::List<M::Variant>::append(M::Variant(*(it->impl)));
        }
    }


    void Tuple::Private::prepend(const Range& range) {
        for (Range::ConstIterator it=range.constBegin(),end=range.constEnd() ; it!=end ; ++it) {
            M::List<M::Variant>::prepend(M::Variant(*(it->impl)));
        }
    }


    void Tuple::Private::append(const M::Range& range) {
        for (M::Range::ConstIterator it=range.constBegin(),end=range.constEnd() ; it!=end ; ++it) {
            M::List<M::Variant>::append(*it);
        }
    }


    void Tuple::Private::prepend(const M::Range& range) {
        for (M::Range::ConstIterator it=range.constBegin(),end=range.constEnd() ; it!=end ; ++it) {
            M::List<M::Variant>::prepend(*it);
        }
    }
}
