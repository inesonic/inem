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
* This file implements the \ref M::Console ckass.
***********************************************************************************************************************/

#include "m_api_types.h"
#include "m_intrinsic_types.h"
#include "m_set.h"
#include "m_tuple.h"
#include "m_console.h"

/***********************************************************************************************************************
 * M::Console::Base
 */

namespace M {
    Console::Base::Base(unsigned newBase):currentBase(newBase) {}


    Console::Base::~Base() {}


    unsigned Console::Base::base() const {
        return currentBase;
    }
}

/***********************************************************************************************************************
 * M::Console::Width
 */

namespace M {
    Console::Width::Width(unsigned newWidth):currentWidth(newWidth) {}


    Console::Width::~Width() {}


    unsigned Console::Width::width() const {
        return currentWidth;
    }
}

/***********************************************************************************************************************
 * M::Console::Pad
 */

namespace M {
    Console::Pad::Pad(char newPad):currentPad(newPad) {}


    Console::Pad::~Pad() {}


    char Console::Pad::pad() const {
        return currentPad;
    }
}

/***********************************************************************************************************************
 * M::Console
 */

namespace M {
    Console::Callback* Console::currentCallback = nullptr;

    void Console::registerCallback(Console::Callback* newCallbackInstance) {
        currentCallback = newCallbackInstance;
    }


    void Console::releaseCallback() {
        currentCallback = nullptr;
    }


    Console::Callback* Console::callback() {
        return currentCallback;
    }


    void Console::initialize(unsigned numberThreads) {
        if (currentCallback != nullptr) {
            currentCallback->initialize(numberThreads);
        }
    }


    bool Console::startMessage(unsigned threadId, MessageType messageType) {
        bool result;

        if (currentCallback != nullptr) {
            result = currentCallback->startMessage(threadId, messageType);
        } else {
            result = false;
        }

        return result;
    }


    void Console::endMessage(unsigned threadId) {
        if (currentCallback != nullptr) {
            currentCallback->endMessage(threadId);
        }
    }


    void Console::append(unsigned threadId, const char* newText) {
        if (currentCallback != nullptr) {
            currentCallback->append(threadId, newText);
        }
    }


    void Console::append(unsigned threadId, unsigned value) {
        if (currentCallback != nullptr) {
            currentCallback->append(threadId, value);
        }
    }


    void Console::append(unsigned threadId, int value) {
        if (currentCallback != nullptr) {
            currentCallback->append(threadId, value);
        }
    }


    void Console::append(unsigned threadId, unsigned long long value) {
        if (currentCallback != nullptr) {
            currentCallback->append(threadId, value);
        }
    }


    void Console::append(unsigned threadId, M::Boolean value) {
        if (currentCallback != nullptr) {
            currentCallback->append(threadId, value);
        }
    }


    void Console::append(unsigned threadId, M::Integer value) {
        if (currentCallback != nullptr) {
            currentCallback->append(threadId, value);
        }
    }


    void Console::append(unsigned threadId, M::Real value) {
        if (currentCallback != nullptr) {
            currentCallback->append(threadId, value);
        }
    }


    void Console::append(unsigned threadId, M::Complex value) {
        if (currentCallback != nullptr) {
            currentCallback->append(threadId, value);
        }
    }


    void Console::append(unsigned threadId, const Model::Set& value) {
        if (currentCallback != nullptr) {
            currentCallback->append(threadId, value);
        }
    }


    void Console::append(unsigned threadId, const M::Set& value) {
        if (currentCallback != nullptr) {
            currentCallback->append(threadId, value);
        }
    }


    void Console::append(unsigned threadId, const Model::Tuple& value) {
        if (currentCallback != nullptr) {
            currentCallback->append(threadId, value);
        }
    }


    void Console::append(unsigned threadId, const M::Tuple& value) {
        if (currentCallback != nullptr) {
            currentCallback->append(threadId, value);
        }
    }


    void Console::append(unsigned threadId, const Model::Variant& value) {
        if (currentCallback != nullptr) {
            currentCallback->append(threadId, value);
        }
    }


    void Console::append(unsigned threadId, const M::Variant& value) {
        if (currentCallback != nullptr) {
            currentCallback->append(threadId, value);
        }
    }


    void Console::append(unsigned threadId, const Console::Base& value) {
        if (currentCallback != nullptr) {
            currentCallback->setBase(threadId, value.base());
        }
    }


    void Console::append(unsigned threadId, const Console::Width& value) {
        if (currentCallback != nullptr) {
            currentCallback->setWidth(threadId, value.width());
        }
    }


    void Console::append(unsigned threadId, const Console::Pad& value) {
        if (currentCallback != nullptr) {
            currentCallback->setPad(threadId, value.pad());
        }
    }


    void Console::append(unsigned threadId, const Console::UpperCase& /* value */) {
        if (currentCallback != nullptr) {
            currentCallback->setUpperCase(threadId);
        }
    }


    void Console::append(unsigned threadId, const Console::LowerCase& /* value */) {
        if (currentCallback != nullptr) {
            currentCallback->setLowerCase(threadId);
        }
    }


    void Console::append(unsigned threadId, const Console::ResetCase& /* value */) {
        if (currentCallback != nullptr) {
            currentCallback->resetCase(threadId);
        }
    }
}
