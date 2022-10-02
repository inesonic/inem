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
* This file implements the \ref M::ModelBase class.
***********************************************************************************************************************/

#include <cassert>

#include "model_identifier_data.h"
#include "model_api.h"
#include "model_status.h"
#include "m_api.h"
#include "m_per_thread.h"
#include "m_console.h"
#include "m_model_base_private.h"
#include "m_model_base.h"

namespace M {
    ModelBase::ModelBase() {
        impl                = new Private(this);
        currentPendingEvent = false;
    }


    ModelBase::~ModelBase() {
        delete impl;
    }


    void ModelBase::setApi(Api* newApi) {
        impl->setApi(newApi);
    }


    void ModelBase::execute(PerThread& perThreadInstance, unsigned threadId) {
        switch (threadId) {
            case  0: { t1(perThreadInstance);   break; }
            case  1: { t2(perThreadInstance);   break; }
            case  2: { t3(perThreadInstance);   break; }
            case  3: { t4(perThreadInstance);   break; }
            case  4: { t5(perThreadInstance);   break; }
            case  5: { t6(perThreadInstance);   break; }
            case  6: { t7(perThreadInstance);   break; }
            case  7: { t8(perThreadInstance);   break; }
            case  8: { t9(perThreadInstance);   break; }
            case  9: { t10(perThreadInstance);  break; }
            case 10: { t11(perThreadInstance);  break; }
            case 11: { t12(perThreadInstance);  break; }
            case 12: { t13(perThreadInstance);  break; }
            case 13: { t14(perThreadInstance);  break; }
            case 14: { t15(perThreadInstance);  break; }
            case 15: { t16(perThreadInstance);  break; }
            case 16: { t17(perThreadInstance);  break; }
            case 17: { t18(perThreadInstance);  break; }
            case 18: { t19(perThreadInstance);  break; }
            case 19: { t20(perThreadInstance);  break; }
            case 20: { t21(perThreadInstance);  break; }
            case 21: { t22(perThreadInstance);  break; }
            case 22: { t23(perThreadInstance);  break; }
            case 23: { t24(perThreadInstance);  break; }
            case 24: { t25(perThreadInstance);  break; }
            case 25: { t26(perThreadInstance);  break; }
            case 26: { t27(perThreadInstance);  break; }
            case 27: { t28(perThreadInstance);  break; }
            case 28: { t29(perThreadInstance);  break; }
            case 29: { t30(perThreadInstance);  break; }
            case 30: { t31(perThreadInstance);  break; }
            case 31: { t32(perThreadInstance);  break; }
            default: { assert(false);            break; }
        }
    }


    void ModelBase::forceAbort() {
        impl->forceAbort();
    }


    void ModelBase::clearAbort() {
        impl->clearAbort();
    }


    bool ModelBase::pause() {
        return impl->pause();
    }


    bool ModelBase::singleStep() {
        return impl->singleStep();
    }


    bool ModelBase::setRunToLocation(OperationHandle operationHandle) {
        return impl->setRunToLocation(operationHandle);
    }


    OperationHandle ModelBase::runToLocation() const {
        return impl->runToLocation();
    }


    bool ModelBase::setBreakAtOperation(OperationHandle operationHandle, bool nowSet) {
        return impl->setBreakAtOperation(operationHandle, nowSet);
    }


    OperationHandle ModelBase::operationBreakpoints(OperationHandle* list) const {
        return impl->operationBreakpoints(list);
    }


    bool ModelBase::resume() {
        return impl->resume();
    }


    void ModelBase::t2(PerThread&) {}


    void ModelBase::t3(PerThread&) {}


    void ModelBase::t4(PerThread&) {}


    void ModelBase::t5(PerThread&) {}


    void ModelBase::t6(PerThread&) {}


    void ModelBase::t7(PerThread&) {}


    void ModelBase::t8(PerThread&) {}


    void ModelBase::t9(PerThread&) {}


    void ModelBase::t10(PerThread&) {}


    void ModelBase::t11(PerThread&) {}


    void ModelBase::t12(PerThread&) {}


    void ModelBase::t13(PerThread&) {}


    void ModelBase::t14(PerThread&) {}


    void ModelBase::t15(PerThread&) {}


    void ModelBase::t16(PerThread&) {}


    void ModelBase::t17(PerThread&) {}


    void ModelBase::t18(PerThread&) {}


    void ModelBase::t19(PerThread&) {}


    void ModelBase::t20(PerThread&) {}


    void ModelBase::t21(PerThread&) {}


    void ModelBase::t22(PerThread&) {}


    void ModelBase::t23(PerThread&) {}


    void ModelBase::t24(PerThread&) {}


    void ModelBase::t25(PerThread&) {}


    void ModelBase::t26(PerThread&) {}


    void ModelBase::t27(PerThread&) {}


    void ModelBase::t28(PerThread&) {}


    void ModelBase::t29(PerThread&) {}


    void ModelBase::t30(PerThread&) {}


    void ModelBase::t31(PerThread&) {}


    void ModelBase::t32(PerThread&) {}


    void ModelBase::handleCheck(PerThread& perThread, OperationHandle operationHandle) const {
        impl->handleCheck(perThread, operationHandle);
    }


    void ModelBase::handleCheck(
            PerThread&       perThread,
            OperationHandle  operationHandle,
            IdentifierHandle identifierHandle
        ) const {
        impl->handleCheck(perThread, operationHandle, identifierHandle);
    }
}
