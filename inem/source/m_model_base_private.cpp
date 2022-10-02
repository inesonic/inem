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
* This file implements the \ref M::ModelBase::Private class.
***********************************************************************************************************************/

#include <cstdint>
#include <cassert>
#include <thread>
#include <chrono>

#include "model_status.h"
#include "model_exceptions.h"
#include "m_api.h"
#include "m_unordered_set.h"
#include "m_per_thread.h"
#include "m_model_base.h"
#include "m_model_base_private.h"

namespace M {
    const unsigned ModelBase::Private::entriesPerGroup    =  64U;
    const unsigned ModelBase::Private::pauseSpinlockDelay = 100U;

    ModelBase::Private::Private(ModelBase* interface) {
        iface                        = interface;
        api                          = nullptr;
        numberOperationHandles       = 0;
        currentForcedAbort           = false;
        currentPauseRequested        = false;
        currentSingleStepRequested   = false;
        currentPausingThread         = static_cast<unsigned>(-1);
        currentRunToLocation         = Model::invalidOperationHandle;
        currentOperationBreakpoints  = nullptr;
    }


    ModelBase::Private::~Private() {
        if (currentOperationBreakpoints != nullptr) {
            delete currentOperationBreakpoints;
        }
    }


    void ModelBase::Private::setApi(Api* newApi) {
        api = newApi;
    }


    void ModelBase::Private::forceAbort() {
        if (!currentForcedAbort) {
            currentForcedAbort         = true;
            currentPauseRequested      = false;
            currentSingleStepRequested = false;
            currentPausingThread       = static_cast<unsigned>(-1);

            setPendingEventFlag();
        }
    }


    void ModelBase::Private::clearAbort() {
        if (currentForcedAbort) {
            currentForcedAbort = false;
            restorePendingEventFlag();
        }
    }


    bool ModelBase::Private::pause() {
        bool result;

        bool pauseAlreadyRequested = currentPauseRequested.exchange(true);
        if (!pauseAlreadyRequested && currentPausingThread == static_cast<unsigned>(-1)) {
            setPendingEventFlag();
            result = true;
        } else {
            result = false;
        }

        return result;
    }


    bool ModelBase::Private::singleStep() {
        // TODO: Code below is a hack, suggest using condition variables or similar for a cleaner solution.
        bool success = (currentPausingThread != static_cast<unsigned>(-1));
        if (success) {
            currentSingleStepRequested = true;
            currentPausingThread       = static_cast<unsigned>(-1);
        }

        return success;
    }


    bool ModelBase::Private::setRunToLocation(OperationHandle operationHandle) {
        bool result;

        if (numberOperationHandles == 0) {
            numberOperationHandles = iface->numberOperationHandles();
        }

        if (operationHandle < numberOperationHandles) {
            currentRunToLocation = operationHandle;

            if (currentRunToLocation != Model::invalidOperationHandle) {
                setPendingEventFlag();
            } else {
                restorePendingEventFlag();
            }

            result = true;
        } else {
            result = false;
        }

        return result;
    }


    OperationHandle ModelBase::Private::runToLocation() const {
        return currentRunToLocation;
    }


    bool ModelBase::Private::setBreakAtOperation(OperationHandle operationHandle, bool nowSet) {
        bool success = false;

        if (currentOperationBreakpoints == nullptr) {
            numberOperationHandles = iface->numberOperationHandles();

            unsigned long numberChunks = static_cast<unsigned long>(
                (numberOperationHandles + (entriesPerGroup - 1)) / entriesPerGroup
            );
            currentOperationBreakpoints = new BreakpointGroup[numberChunks]();
        }

        unsigned long   entryIndex = static_cast<unsigned long>(operationHandle / entriesPerGroup);
        unsigned        bitOffset  = static_cast<unsigned>(operationHandle % entriesPerGroup);
        BreakpointGroup mask       = BreakpointGroup(1) << bitOffset;
        BreakpointGroup entry      = currentOperationBreakpoints[entryIndex];

        if (nowSet && (entry & mask) == 0) {
            currentOperationBreakpoints[entryIndex] = entry | mask;
            setPendingEventFlag();

            success = true;
        } else if (!nowSet && (entry && mask) != 0) {
            restorePendingEventFlag();
            currentOperationBreakpoints[entryIndex] = entry & ~mask;

            success = true;
        }

        return success;
    }


    OperationHandle ModelBase::Private::operationBreakpoints(OperationHandle* list) const {
        unsigned long numberEntries = 0;

        if (currentOperationBreakpoints != nullptr) {
            unsigned long numberBitFields = static_cast<unsigned long>(
                (numberOperationHandles + (entriesPerGroup - 1)) / entriesPerGroup
            );
            unsigned long handle = 0;

            for (unsigned i=0 ; i<numberBitFields ; ++i) {
                BreakpointGroup bitField = currentOperationBreakpoints[i];
                BreakpointGroup mask     = 1;

                while (mask) {
                    if (bitField & mask) {
                        list[numberEntries] = handle;
                        ++numberEntries;
                    }

                    mask <<= BreakpointGroup(1);
                    ++handle;
                }
            }
        }

        return numberEntries;
    }


    bool ModelBase::Private::resume() {
        // TODO: Code below is a hack, suggest using condition variables or similar for a cleaner solution.
        bool success = (currentPausingThread != static_cast<unsigned>(-1));
        if (success) {
            currentPausingThread = static_cast<unsigned>(-1);
            restorePendingEventFlag();
        }

        return success;
    }


    void ModelBase::Private::handleCheck(PerThread& perThread, OperationHandle operationHandle) {
        if (currentForcedAbort) {
            throw Model::UserAbortRequested();
        } else if (currentPauseRequested.exchange(false)) {
            currentPausingThread = perThread.threadId();

            api->modelPausedOnUserRequest(operationHandle);
            api->threadPaused(perThread.threadId());

            waitForResume(perThread);
        } else if (currentRunToLocation == operationHandle) {
            currentRunToLocation = Model::invalidOperationHandle;
            currentPausingThread = perThread.threadId();

            api->modelPausedOnUserRequest(operationHandle);
            api->threadPaused(perThread.threadId());

            waitForResume(perThread);
        } else if (currentPausingThread != static_cast<unsigned>(-1)) {
            api->threadPaused(perThread.threadId());
            waitForResume(perThread);
        } else {
            checkOperationHandle(perThread, operationHandle);
        }
    }


    void ModelBase::Private::handleCheck(
            PerThread&       perThread,
            OperationHandle  operationHandle,
            IdentifierHandle /* identifierHandle */
        ) {
        if (currentForcedAbort) {
            throw Model::UserAbortRequested();
        } else if (currentPauseRequested.exchange(false)) {
            currentPausingThread = perThread.threadId();

            api->modelPausedOnUserRequest(operationHandle);
            api->threadPaused(perThread.threadId());

            waitForResume(perThread);
        } else if (currentRunToLocation == operationHandle) {
            currentRunToLocation = Model::invalidOperationHandle;
            currentPausingThread = perThread.threadId();

            api->modelPausedOnUserRequest(operationHandle);
            api->threadPaused(perThread.threadId());

            waitForResume(perThread);
        } else if (currentPausingThread != static_cast<unsigned>(-1)) {
            api->threadPaused(perThread.threadId());
            waitForResume(perThread);
        } else {
            checkOperationHandle(perThread, operationHandle);
        }
    }


    void ModelBase::Private::setPendingEventFlag() {
        iface->currentPendingEvent = true;
    }


    void ModelBase::Private::restorePendingEventFlag() {
        bool hasPendingEvent = (
               currentPauseRequested
            || currentForcedAbort
            || (currentPausingThread != static_cast<unsigned>(-1))
            || (currentRunToLocation != Model::invalidOperationHandle)
        );

        if (!hasPendingEvent && currentOperationBreakpoints != nullptr) {
            unsigned long numberOperationBitFields = static_cast<unsigned long>(
                (numberOperationHandles + (entriesPerGroup - 1)) / entriesPerGroup
            );
            unsigned index = 0;
            while (!hasPendingEvent && index<numberOperationBitFields) {
                hasPendingEvent = (currentOperationBreakpoints[index] != 0);
                ++index;
            }
        }

        iface->currentPendingEvent = hasPendingEvent;
    }


    bool ModelBase::Private::checkOperationHandle(PerThread& perThread, OperationHandle operationHandle) {
        bool paused = false;

        if (currentOperationBreakpoints != nullptr) {
            unsigned long entryIndex = static_cast<unsigned long>(operationHandle / entriesPerGroup);
            unsigned      bitOffset  = operationHandle % entriesPerGroup;
            BreakpointGroup mask       = BreakpointGroup(1) << bitOffset;
            BreakpointGroup entry      = currentOperationBreakpoints[entryIndex];

            if ((entry & mask) != 0) {
                api->modelPausedAtOperation(operationHandle);
                currentPausingThread = perThread.threadId();
                waitForResume(perThread);
                paused = true;
            }
        }

        return paused;
    }


    void ModelBase::Private::waitForResume(PerThread& perThread) {
        // TODO: I've spent a little time looking into cleaner solutions that would not inject a race condition
        //       somehwhere; however, every other solution is proving to be problematic or surprisingly complex.
        //
        //       For this reason, for now, I've decided to keep things simple for now.  spin-lock with a delay to avoid
        //       hammering the processors.  Longer term we can pursue cleaner solutions (or pursue non-invasive
        //       debugging).

        unsigned pausingThread = currentPausingThread;
        while (currentPausingThread != static_cast<unsigned>(-1)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(pauseSpinlockDelay));
        }

        if (!currentForcedAbort) {
            if (perThread.threadId() == pausingThread) {
                api->modelResumed();
            }

            api->threadResumed(perThread.threadId());
        }

        currentPauseRequested |= currentSingleStepRequested.exchange(false);
    }
}
