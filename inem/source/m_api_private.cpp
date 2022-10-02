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
* This file implements the \ref M::Api::Private class.
***********************************************************************************************************************/

#include <cassert>
#include <vector>
#include <future>
#include <atomic>
#include <cstring>
#include <cstdint>
#include <exception>
#include <cfenv>

#include "m_api_types.h"
#include "model_api_types.h"
#include "model_identifier_data.h"
#include "model_status.h"
#include "model_rng.h"
#include "m_per_thread.h"
#include "m_model_base.h"
#include "model_api.h"
#include "model_exceptions.h"
#include "m_console.h"
#include "m_api.h"
#include "m_api_private.h"

namespace M {
    Api::Private::Private(Model::Api* api, ModelBase* model) {
        assert(model != nullptr);

        currentApi            = api;
        currentModel          = model;
        currentState          = Model::State::STOPPED;
        currentStatusInstance = nullptr;
        numberActiveThreads   = 0;
    }


    Api::Private::~Private() {
        delete currentModel;
    }


    void Api::Private::registerConsoleCallback(M::Console::Callback* newConsole) {
        M::Console::registerCallback(newConsole);
    }


    void Api::Private::releaseConsoleCallback() {
        M::Console::releaseCallback();
    }


    bool Api::Private::start(Model::Rng::RngType rngType, const Model::Rng::RngSeed& rngSeed, Model::Status* status) {
        bool success;

        if (currentState != Model::State::STOPPED && currentState != Model::State::ABORTED) {
            success = false;
        } else {
            currentAbortReason          = Model::AbortReason::NONE;
            currentAbortOperationHandle = Model::invalidOperationHandle;
            currentState                = Model::State::ACTIVE;
            currentStatusInstance       = status;

            if (status != nullptr) {
                status->started(currentApi);
            }

            threads.clear();
            unsigned numberParallelThreads = currentModel->numberThreads();
            numberActiveThreads = numberParallelThreads;

            Model::Rng::RngSeed workingSeeds;
            std::memcpy(workingSeeds, rngSeed, sizeof(Model::Rng::RngSeed));

            for (unsigned threadId=0 ; threadId<numberParallelThreads ; ++threadId) {
                PerThread* perThreadInstance = new PerThread(
                    threadId,
                    rngType,
                    workingSeeds,
                    M::Console::callback(),
                    status
                );
                generateSeeds(workingSeeds);

                threads.push_back(std::async(std::launch::async, execute, this, perThreadInstance, threadId));
            }

            success = true;
        }

        return success;
    }


    Model::State Api::Private::state() const {
        return currentState;
    }


    bool Api::Private::abort() {
        bool success;

        Model::State reportedState = currentState;
        if (reportedState != Model::State::STOPPED  &&
            reportedState != Model::State::ABORTED  &&
            reportedState != Model::State::ABORTING    ) {
            currentAbortReason = Model::AbortReason::USER_REQUEST;
            currentState       = Model::State::ABORTING;
            currentModel->forceAbort();

            for (std::vector<std::future<void>>::iterator it=threads.begin(),end=threads.end() ; it!=end ; ++it) {
                it->wait();
            }

            success = true;
        } else {
            success = false;
        }

        return success;
    }


    void Api::Private::waitComplete() {
        for (std::vector<std::future<void>>::iterator it=threads.begin(),end=threads.end() ; it!=end ; ++it) {
            it->wait();
        }
    }


    bool Api::Private::run(Model::Rng::RngType rngType, const Model::Rng::RngSeed& rngSeed, Model::Status* status) {
        bool success = start(rngType, rngSeed, status);
        if (success) {
            waitComplete();
        }

        return success;
    }


    bool Api::Private::pause() {
        bool success;

        Model::State reportedState = currentState;
        if (reportedState != Model::State::ABORTED && reportedState != Model::State::ABORTING) {
            success = currentModel->pause();
        } else {
            success = false;
        }

        return success;
    }


    bool Api::Private::singleStep() {
        return currentModel->singleStep();
    }


    bool Api::Private::setRunToLocation(OperationHandle operationHandle) {
        return currentModel->setRunToLocation(operationHandle);
    }


    OperationHandle Api::Private::runToLocation() const {
        return currentModel->runToLocation();
    }


    bool Api::Private::setBreakAtOperation(OperationHandle operationHandle, bool nowSet) {
        return currentModel->setBreakAtOperation(operationHandle, nowSet);
    }


    OperationHandle Api::Private::operationBreakpoints(OperationHandle* list) const {
        return currentModel->operationBreakpoints(list);
    }


    bool Api::Private::resume() {
        bool success;

        Model::State reportedState = currentState;
        if (reportedState == Model::State::PAUSED_ON_USER_REQUEST    ||
            reportedState == Model::State::PAUSED_AT_OPERATION       ||
            reportedState == Model::State::PAUSED_ON_VARIABLE_UPDATE    ) {
            success = currentModel->resume();
        } else {
            success = false;
        }

        return success;
    }


    unsigned Api::Private::numberThreads() const {
        return currentModel->numberThreads();
    }


    OperationHandle Api::Private::numberOperationHandles() const {
        return currentModel->numberOperationHandles();
    }


    Model::IdentifierDatabase Api::Private::identifierDatabase() const {
        return currentModel->identifierDatabase();
    }


    Model::Rng* Api::Private::createRng(Model::Rng::RngType rngType, const Model::Rng::RngSeed& rngSeed) const {
        return new PerThread(0, rngType, rngSeed, M::Console::callback());
    }


    void Api::Private::modelPausedOnUserRequest(OperationHandle operationHandle) {
        currentState = Model::State::PAUSED_ON_USER_REQUEST;

        if (currentStatusInstance != nullptr) {
            currentStatusInstance->pausedOnUserRequest(currentApi, operationHandle);
        }
    }


    void Api::Private::modelPausedAtOperation(OperationHandle operationHandle) {
        currentState = Model::State::PAUSED_AT_OPERATION;

        if (currentStatusInstance != nullptr) {
            currentStatusInstance->pausedAtOperation(currentApi, operationHandle);
        }
    }


    void Api::Private::modelPausedOnVariableUpdate(
            OperationHandle  operationHandle,
            IdentifierHandle identifierHandle
        ) {
        currentState = Model::State::PAUSED_ON_VARIABLE_UPDATE;

        if (currentStatusInstance != nullptr) {
            currentStatusInstance->pausedOnVariableUpdate(currentApi, operationHandle, identifierHandle);
        }
    }


    void Api::Private::threadPaused(unsigned threadId) {
        if (currentStatusInstance != nullptr) {
            currentStatusInstance->threadPaused(currentApi, threadId);
        }
    }


    void Api::Private::modelResumed() {
        currentState = Model::State::ACTIVE;

        if (currentStatusInstance != nullptr) {
            currentStatusInstance->resumed(currentApi);
        }
    }


    void Api::Private::threadResumed(unsigned threadId) {
        if (currentStatusInstance != nullptr) {
            currentStatusInstance->threadResumed(currentApi, threadId);
        }
    }


    void Api::Private::execute(Api::Private* impl, PerThread* perThreadInstance, unsigned threadId) {
        Model::Api*    currentApi     = impl->currentApi;
        ModelBase*     model          = impl->currentModel;
        Model::Status* statusInstance = impl->currentStatusInstance;

        Console::report(threadId, Console::MessageType::INFORMATION, "Starting thread ", threadId, ".");

        fesetround(FE_TONEAREST);

        if (statusInstance != nullptr) {
            statusInstance->threadStarted(currentApi, threadId);
        }

        try {
            model->execute(*perThreadInstance, threadId);
        } catch (Model::UserAbortRequested&) {
            impl->currentAbortOperationHandle = perThreadInstance->operationHandle();
        } catch (Model::InesonicException& inesonicException) {
            impl->currentState                = Model::State::ABORTING;
            impl->currentAbortReason          = inesonicException.exceptionType();
            impl->currentAbortOperationHandle = perThreadInstance->operationHandle();
            model->forceAbort();
        } catch (std::bad_alloc&) {
            impl->currentState                = Model::State::ABORTING;
            impl->currentAbortReason          = Model::AbortReason::MEMORY_ALLOCATION_ERROR;
            impl->currentAbortOperationHandle = perThreadInstance->operationHandle();
            model->forceAbort();
        } catch (std::exception&) {
            impl->currentState                = Model::State::ABORTING;
            impl->currentAbortReason          = Model::AbortReason::SYSTEM;
            impl->currentAbortOperationHandle = perThreadInstance->operationHandle();
            model->forceAbort();
        }

        delete perThreadInstance;

        Model::State        currentState        = impl->currentState;
        std::atomic_int8_t& numberActiveThreads = impl->numberActiveThreads;

        std::int8_t pastNumberActiveThreads = numberActiveThreads.fetch_sub(1);

        if (pastNumberActiveThreads == 1) {
            if (currentState == Model::State::ABORTING) {
                impl->currentState = Model::State::ABORTED;
            } else {
                impl->currentState = Model::State::STOPPED;
            }
        } else if (pastNumberActiveThreads < 1) {
            ++numberActiveThreads;
        }

        if (statusInstance != nullptr) {
            if (currentState == Model::State::ABORTING) {
                statusInstance->threadAborted(currentApi, threadId);
                Console::report(threadId, Console::MessageType::INFORMATION, "Thread ", threadId, " aborted.");
            } else {
                statusInstance->threadFinished(currentApi, threadId);
                Console::report(threadId, Console::MessageType::INFORMATION, "Thread ", threadId, " finished.");
            }

            if (pastNumberActiveThreads == 1) {
                if (currentState == Model::State::ABORTING) {
                    model->clearAbort();
                    statusInstance->aborted(currentApi, impl->currentAbortReason, impl->currentAbortOperationHandle);
                } else {
                    statusInstance->finished(currentApi);
                }
            }
        }
    }


    void Api::Private::generateSeeds(Model::Rng::RngSeed& seeds) {
        std::uint32_t* knSeeds     = reinterpret_cast<std::uint32_t*>(seeds);
        unsigned       numberSeeds = (sizeof(Model::Rng::RngSeed) + 3) / 4;

        for (unsigned i=0 ; i<numberSeeds ; ++i) {
            std::uint32_t& knSeed = knSeeds[i];

            // NOTE: Knuth-Lewis PRNG.  Constants below from Numerical Recipes In C++, Second Edition, p. 289
            knSeed = 1664525UL * knSeed + 1013904223L;
        }
    }
}
