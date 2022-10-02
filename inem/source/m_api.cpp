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
* This file implements the \ref M::Api class.
***********************************************************************************************************************/

#include <cassert>
#include <ctime>

#include "m_api_types.h"
#include "model_api_types.h"
#include "model_identifier_data.h"
#include "model_status.h"
#include "model_rng.h"
#include "m_console.h"
#include "m_model_base.h"
#include "m_matrix_private_base.h"
#include "m_api_private.h"
#include "model_api.h"
#include "m_api.h"

namespace M {
    void Api::setMatrixApi(const MatApi* matrixApi) {
        MatrixPrivateBase::setMatrixApi(matrixApi);
    }


    Api::Api(ModelBase* model):impl(new Private(this, model)) {}


    Api::~Api() {
        delete impl;
    }


    void Api::registerConsoleCallback(M::Console::Callback* newConsole) {
        impl->registerConsoleCallback(newConsole);
    }


    void Api::releaseConsoleCallback() {
        impl->releaseConsoleCallback();
    }


    bool Api::start(Model::Rng::RngType rngType,const Model::Rng::RngSeed& rngSeed, Model::Status* status) {
        return impl->start(rngType, rngSeed, status);
    }


    Model::State Api::state() const {
        return impl->state();
    }


    bool Api::abort() {
        return impl->abort();
    }


    void Api::waitComplete() {
        impl->waitComplete();
    }


    bool Api::run(Model::Rng::RngType rngType, const Model::Rng::RngSeed& rngSeed, Model::Status* status) {
        return impl->run(rngType, rngSeed, status);
    }


    bool Api::pause() {
        return impl->pause();
    }


    bool Api::singleStep() {
        return impl->singleStep();
    }


    bool Api::setRunToLocation(OperationHandle operationHandle) {
        return impl->setRunToLocation(operationHandle);
    }


    OperationHandle Api::runToLocation() const {
        return impl->runToLocation();
    }


    bool Api::setBreakAtOperation(OperationHandle operationHandle, bool nowSet) {
        return impl->setBreakAtOperation(operationHandle, nowSet);
    }


    OperationHandle Api::operationBreakpoints(OperationHandle* list) const {
        return impl->operationBreakpoints(list);
    }


    bool Api::resume() {
        return impl->resume();
    }


    unsigned Api::numberThreads() const {
        return impl->numberThreads();
    }


    OperationHandle Api::numberOperationHandles() const {
        return impl->numberOperationHandles();
    }


    Model::IdentifierDatabase Api::identifierDatabase() const {
        return impl->identifierDatabase();
    }


    Model::Rng* Api::createRng(Model::Rng::RngType rngType, const Model::Rng::RngSeed& rngSeed) const {
        return impl->createRng(rngType, rngSeed);
    }


    Model::Rng* Api::createRng() const {
        unsigned long long  t    = static_cast<unsigned long long>(std::time(nullptr));
        Model::Rng::RngSeed seed        = { t, t + 1, t + 2, t + 3 };

        return impl->createRng(Model::Rng::RngType::MT19937, seed);
    }


    void Api::deleteRng(Model::Rng* rng) const {
        delete rng;
    }


    void Api::modelPausedOnUserRequest(OperationHandle operationHandle) {
        impl->modelPausedOnUserRequest(operationHandle);
    }


    void Api::modelPausedAtOperation(OperationHandle operationHandle) {
        impl->modelPausedAtOperation(operationHandle);
    }


    void Api::modelPausedOnVariableUpdate(OperationHandle operationHandle, IdentifierHandle identifierHandle) {
        impl->modelPausedOnVariableUpdate(operationHandle, identifierHandle);
    }


    void Api::threadPaused(unsigned threadId) {
        impl->threadPaused(threadId);
    }


    void Api::modelResumed() {
        impl->modelResumed();
    }


    void Api::threadResumed(unsigned threadId) {
        impl->threadResumed(threadId);
    }
}
