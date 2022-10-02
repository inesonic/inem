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
* This header defines the \ref M::Api class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_API_H
#define M_API_H

#include "m_common.h"
#include "m_api_types.h"
#include "model_identifier_database.h"
#include "model_status.h"
#include "model_rng.h"
#include "m_console.h"
#include "model_api.h"

extern "C" typedef struct _MatApi MatApi;

namespace M {
    class ModelBase;
    class Console;

    /**
     * Class that provides a functioning API for the user's model.
     */
    class M_PUBLIC_API Api:public Model::Api {
        public:
            /**
             * Method you can use to specify the matrix library to be used.  You should call this before any other
             * method or class.
             *
             * Note that this settings is global.
             *
             * \param[in] matrixApi The matrix API library to be used.
             */
            static void setMatrixApi(const MatApi* matrixApi);

            /**
             * Constructor
             *
             * \param[in] model The model this API should control.  This instance will take ownership of the model
             *                  and destroy it at end of scope.
             */
            Api(ModelBase* model);

            ~Api() override;

            /**
             * Method you can call to register a console callback.  This method exists to deal with Windows specific
             * memory issues associated with dynamic loading.
             *
             * \param[in] newConsoleCallback The new console to be registered.
             */
            void registerConsoleCallback(M::Console::Callback* newConsoleCallback);

            /**
             * Method you can call to release a previously registered console.
             *
             * \param[in] newConsole The new console to be registered.
             */
            void releaseConsoleCallback();

            /**
             * Method you can use to start the model running in the background.
             *
             * \param[in] rngType The type of RNG to be used by the model.
             *
             * \param[in] rngSeed A value used to seed the PRNG, ignored if a TRNG is used.
             *
             * \param[in] status  An instance of \ref Model::Status you can optionally use to receive notification of
             *                    events.
             *
             * \return Returns true if the model was started.  Returns false if the model is currently running.
             */
            bool start(
                Model::Rng::RngType        rngType,
                const Model::Rng::RngSeed& rngSeed,
                Model::Status*             status = nullptr
            ) final;

            /**
             * Method you can use to check status of the model.
             *
             * \return Returns the last reported model operating status.
             */
            Model::State state() const final;

            /**
             * Method you can use to forcibly stop the model.  The method will wait until the model has actually
             * stopped execution.
             *
             * \return Returns true on success, returns false if the model is not active.
             */
            bool abort() final;

            /**
             * Method you can use to block until the model completes execution.
             */
            void waitComplete() final;

            /**
             * Method that can be called to run the model, blocking until the model finishes execution.
             *
             * \param[in] rngType The type of RNG to be used by the model.
             *
             * \param[in] rngSeed A value used to seed the PRNG, ignored if a TRNG is used.
             *
             * \param[in] status  An instance of \ref Model::Status you can optionally use to receive notification of
             *                    events.
             *
             * \return Returns true if the model was started.  Returns false if the model is currently running.
             */
            bool run(
                Model::Rng::RngType        rngType,
                const Model::Rng::RngSeed& rngSeed,
                Model::Status*             status = nullptr
            ) final;

            /**
             * Method you can use to pause the model immediately.
             *
             * \return Returns true on success, returns false if the model is not active.
             */
            bool pause() final;

            /**
             * Method you can use to single step the model.
             *
             * \return Returns true on success, returns false if the model is currently active.
             */
            bool singleStep() final;

            /**
             * Method you can use to specify a specific location to run-to before stopping.
             *
             * \param[in] operationHandle The operation handle of the location to run-to.  Setting the value to
             *                            Model::invalidOperationHandle will clear the run-to location.
             *
             * \return Returns true on success, returns false if the run-to location is invalid.
             */
            bool setRunToLocation(OperationHandle operationHandle) final;

            /**
             * Method you can use to obtain the current run-to location.
             *
             * \return Returns the current run-to location.  A value of Model::invalidOperationHandle is returned if
             *         there is no run-to location set.
             */
            OperationHandle runToLocation() const final;

            /**
             * Method you can use to set a breakpoint at a specific execution point.
             *
             * \param[in] operationHandle The operation handle we wish to pause at.  Note that the model will pause
             *                            just before performing the operation.
             *
             * \param[in] nowSet          If true, the breakpoint is now set.  If false, the breakpoint will be
             *                            cleared.
             *
             * \return Returns true on success, returns false if the model is not active.
             */
            bool setBreakAtOperation(OperationHandle operationHandle, bool nowSet) final;

            /**
             * Method you can use to obtain a list of pending operation breakpoints.
             *
             * \param[in]  list The list to be populated.  The list must be pre-allocated with sufficient space to
             *                  store one entry per operation handle.
             *
             * \return Returns the actual length of the list, in entries.
             */
            OperationHandle operationBreakpoints(OperationHandle* list) const final;

            /**
             * Method you can use to resume the model after being paused.
             *
             * \return Returns true on success, returns false if the model is not paused.
             */
            bool resume() final;

            /**
             * Method that returns the number of threads required by the model.
             *
             * \return Returns the number of required threads.
             */
            virtual unsigned numberThreads() const final;

            /**
             * Method that returns the total number of pausable operations in the model.
             *
             * \return Returns the number of pausable operations.  Operation handles will be numbered zero to one less
             *         than this value.
             */
            OperationHandle numberOperationHandles() const final;

            /**
             * Method that returns a database containing information about identifiers within the model.
             *
             * \return Returns a new identifier database instance.
             */
            Model::IdentifierDatabase identifierDatabase() const final;

            /**
             * Method you can use to obtain an RNG tied to this model instance.
             *
             * \param[in] rngType The type of RNG to be used by the model.
             *
             * \param[in] rngSeed A value used to seed the PRNG, ignored if a TRNG is used.
             *
             * \return Returns a pointer to a newly created and configured \ref Model::Rng instance.
             */
            Model::Rng* createRng(Model::Rng::RngType rngType, const Model::Rng::RngSeed& rngSeed) const final;

            /**
             * Method you can use to obtain an RNG tied to this model instance.  This version uses the current system
             * time as a seed
             *
             * \return Returns a pointer to a newly created and configured \ref Model::Rng instance.
             */
            virtual Model::Rng* createRng() const final;

            /**
             * Method you can use to delete a previously created RNG instance.
             *
             * \param[in] rng The random number generator instance to be deleted.
             */
            void deleteRng(Model::Rng* rng) const final;

            /**
             * Method that is called when a thread has paused due to a call to \ref Model::Api::pause.
             *
             * \param[in] operationHandle The operation handle where the model paused.
             */
            void modelPausedOnUserRequest(OperationHandle operationHandle);

            /**
             * Method that is called when a thread has paused due to an instruction breakpoint.  Note that this method
             * may be called from a thread other than the thread that started the model and *must* be thread safe.
             *
             * \param[in] operationHandle The operation handle where the model paused.
             */
            void modelPausedAtOperation(OperationHandle operationHandle);

            /**
             * Method that is called when a thread has paused due to a data breakpoint.  Note that this method may be
             * called from a thread other than the thread that started the model and *must* be thread safe.
             *
             * \param[in] operationHandle  The operation handle where the model paused.
             *
             * \param[in] identifierHandle The identifier handle that triggered the pause condition.
             */
            void modelPausedOnVariableUpdate(OperationHandle operationHandle, IdentifierHandle identifierHandle);

            /**
             * Method that is called by the model implementation to indicate that a thread has paused.
             *
             * \param[in] threadId The zero based ID of the thread that is now stopping.
             */
            void threadPaused(unsigned threadId);

            /**
             * Method that is called by the model implementation to indicate that the model has resumed.
             */
            void modelResumed();

            /**
             * Method that is called by the model implementation to indicate that a thread has resumed.
             *
             * \param[in] threadId The zero based ID of the thread that is now stopping.
             */
            void threadResumed(unsigned threadId);

         private:
            class Private;
            Private* impl;
    };
}

#endif
