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
* This header defines the \ref M::ModelBase::Private class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_MODEL_BASE_PRIVATE_H
#define M_MODEL_BASE_PRIVATE_H

#include <cstdint>
#include <atomic>

#include "m_api.h"
#include "m_unordered_set.h"
#include "m_model_base.h"

namespace Model {
    class Status;
}

namespace M {
    /**
     * Private implementation of the M::ModelBase class.
     */
    class ModelBase::Private {
        public:
            /**
             * Constructor.
             *
             * \param[in] interface Pointer to the public interface.
             */
            Private(ModelBase* interface);

            ~Private();

            /**
             * Method that is called to set the Api for the model.
             *
             * \param[in] api The API tied to the model.
             */
            void setApi(Api* api);

            /**
             * Method that is called to trigger the model to abort.
             */
            void forceAbort();

            /**
             * Method that is called to clear an abort condition.
             */
            void clearAbort();

            /**
             * Method you can call to request the model to pause at the next opportunity.
             *
             * \return Returns true on success, returns false on error.
             */
            bool pause();

            /**
             * Method you can use to single step the model.
             *
             * \return Returns true on success, returns false if the model is currently active.
             */
            bool singleStep();

            /**
             * Method you can use to specify a specific location to run-to before stopping.
             *
             * \param[in] operationHandle The operation handle of the location to run-to.  Setting the value to
             *                            Model::invalidOperationHandle will clear the run-to location.
             *
             * \return Returns true on success, returns false if the run-to location is invalid.
             */
            bool setRunToLocation(OperationHandle operationHandle);

            /**
             * Method you can use to obtain the current run-to location.
             *
             * \return Returns the current run-to location.  A value of Model::invalidOperationHandle is returned if
             *         there is no run-to location set.
             */
            OperationHandle runToLocation() const;

            /**
             * Method you can use to pause the model at a specific execution point.
             *
             * \param[in] operationHandle The operation handle we wish to pause at.  Note that the model will pause
             *                            just before performing the operation.
             *
             * \param[in] nowSet          If true, we will pause at the operation.  If false, any pending break
             *                            condition will be cleared.
             *
             * \return Returns true on success, returns false if the model is not active.
             */
            bool setBreakAtOperation(OperationHandle operationHandle, bool nowSet);

            /**
             * Method you can use to obtain a list of pending operation breakpoints.
             *
             * \param[in]  list The list to be populated.  The list must be pre-allocated with sufficient space to
             *                  store one entry per operation handle.
             *
             * \return Returns the actual length of the list, in entries.
             */
            OperationHandle operationBreakpoints(OperationHandle* list) const;

            /**
             * Method you can call to request the model resume execution.
             *
             * \return Returns true on success, returns false on error.
             */
            bool resume();

            /**
             * Method that handles pending events.
             *
             * \param[in] perThread       The per-thread class instance tied to the thread.
             *
             * \param[in] operationHandle The operation handle to be checked.
             */
            void handleCheck(PerThread& perThread, OperationHandle operationHandle);

            /**
             * Method that handles pending events.
             *
             * \param[in] perThread        The per-thread class instance tied to the thread.
             *
             * \param[in] operationHandle  The operation handle to be checked.
             *
             * \param[in] identifierHandle The identifier handle to be checked.
             */
            void handleCheck(
                PerThread&       perThread,
                OperationHandle  operationHandle,
                IdentifierHandle identifierHandle
            );

        private:
            /**
             * Method that conditionally pauses on an operation handle.
             *
             * \param[in] perThread       The per-thread class instance tied to the thread.
             *
             * \param[in] operationHandle The operation handle to be checked.
             *
             * \return Returns true if we paused.  Returns false if there was no pause condition.
             */
            bool checkOperationHandle(PerThread& perThread, OperationHandle operationHandle);

            /**
             * Method that is called to update the pending flag on the inclusion of a new event.
             */
            void setPendingEventFlag();

            /**
             * Method you can call to update the pending flag when a pending event has been cleared.
             */
            void restorePendingEventFlag();

            /**
             * Method that is called to block until \ref M::ModelBase::Private::resume is called.
             */
            void waitForResume(PerThread& perThread);

            /**
             * Type used for breakpoint flags.
             */
            typedef std::uint64_t BreakpointGroup;

            /**
             * Value used to indicate the number of breakpoints tracked per group.
             */
            static const unsigned entriesPerGroup;

            /**
             * Value indicating the desired delay between pause spinlock checks.
             */
            static const unsigned pauseSpinlockDelay;

            /**
             * The interface class tied to this object.
             */
            ModelBase* iface;

            /**
             * The API tied to this object.
             */
            Api* api;

            /**
             * The number of operation handles.  Used to perform bounds checking.
             */
            OperationHandle numberOperationHandles;

            /**
             * Flag used to indicate a forced abort.
             */
            volatile bool currentForcedAbort;

            /**
             * Flag used to indicate a pause request.
             */
            std::atomic_uint8_t currentPauseRequested;

            /**
             * Flag used to indicate single step requested.
             */
            std::atomic_uint8_t currentSingleStepRequested;

            /**
             * The ID of the thread that triggered the pause event.
             */
            volatile unsigned currentPausingThread;

            /**
             * Value holding the current run-to location.
             */
            OperationHandle currentRunToLocation;

            /**
             * Array of operation handle flags.
             */
            volatile BreakpointGroup* currentOperationBreakpoints;
    };
};

#endif
