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
* This header defines the \ref Model::Status class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_STATUS_H
#define MODEL_STATUS_H

#include "model_common.h"
#include "model_api_types.h"
#include "model_api.h"

namespace Model {
    /**
     * Base class you can overload to receive status information about your model.  You can overload the methods in
     * this class to receive notification of specific events.
     *
     * Do note that a number of methods may be triggered from a thread other than the thread used to start the model.
     */
    class MODEL_PUBLIC_API Status {
        public:
            virtual ~Status() = default;

            /**
             * Method that is called when the model has started execution.
             *
             * \param[in] modelApi A pointer to the model API class.
             */
            virtual void started(Api* modelApi);

            /**
             * Method that is called when the model has completed execution.  Note that this method may be called from
             * a thread other than the thread that started the model and *must* be thread safe.
             *
             * \param[in] modelApi A pointer to the model API class.
             */
            virtual void finished(Api* modelApi);

            /**
             * Method that is called when the model has been aborted.  Note that this method may be called from a
             * thread other than the thread that started the model and *must* be thread safe.
             *
             * \param[in] modelApi        A pointer to the model API class.
             *
             * \param[in] abortReason     The reason the model was aborted.
             *
             * \param[in] operationHandle The handle to the operation where the abort occurred.
             */
            virtual void aborted(Api* modelApi, AbortReason abortReason, OperationHandle operationHandle);

            /**
             * Method that is called when a thread has paused due to a call to \ref Api::pause.
             *
             * \param[in] modelApi        A pointer to the model API class.
             *
             * \param[in] operationHandle The operation handle where the model paused.
             */
            virtual void pausedOnUserRequest(Api* modelApi, OperationHandle operationHandle);

            /**
             * Method that is called when a thread has paused due to an instruction breakpoint.  Note that this method
             * may be called from a thread other than the thread that started the model and *must* be thread safe.
             *
             * \param[in] modelApi        A pointer to the model API class.
             *
             * \param[in] operationHandle The operation handle where the model paused.
             */
            virtual void pausedAtOperation(Api* modelApi, OperationHandle operationHandle);

            /**
             * Method that is called when a thread has paused due to a data breakpoint.  Note that this method may be
             * called from a thread other than the thread that started the model and *must* be thread safe.
             *
             * \param[in] modelApi         A pointer to the model API class.
             *
             * \param[in] operationHandle  The operation handle where the model paused.
             *
             * \param[in] identifierHandle The identifier handle that triggered the pause condition.
             */
            virtual void pausedOnVariableUpdate(
                Api*             modelApi,
                OperationHandle  operationHandle,
                IdentifierHandle identifierHandle
            );

            /**
             * Method that is called when the model resumes operation.
             *
             * \param[in] modelApi A pointer to the model API class.
             */
            virtual void resumed(Api* modelApi);

            /**
             * Method that is called when a thread is started.  You can use this method to track when threads in the
             * model actually launch.  Note that this method is called from each thread and *must* be thread safe.
             *
             * \param[in] modelApi A pointer to the model API class.
             *
             * \param[in] threadId The zero based ID of the started thread.
             */
            virtual void threadStarted(Api* modelApi, unsigned threadId);

            /**
             * Method that is called when a thread finishes.  You can use this method to determine when a portion of
             * the model indicates it has completed.
             *
             * Note that this method is called from each thread and *must* be thread safe.
             *
             * \param[in] modelApi A pointer to the model API class.
             *
             * \param[in] threadId The zero based ID of the thread that is now stopping.
             */
            virtual void threadFinished(Api* modelApi, unsigned threadId);

            /**
             * Method that is called when a thread has paused.
             *
             * Note that this method is called from each thread and *must* be thread safe.
             *
             * \param[in] modelApi A pointer to the model API class.
             *
             * \param[in] threadId The zero based ID of the thread that is now stopping.
             */
            virtual void threadPaused(Api* modelApi, unsigned threadId);

            /**
             * Method that is called by the model implementation to indicate that a thread has resumed.
             *
             * \param[in] modelApi A pointer to the model API class.
             *
             * \param[in] threadId The zero based ID of the thread that is now stopping.
             */
            virtual void threadResumed(Api* modelApi, unsigned threadId);

            /**
             * Method that is called when a thread is aborted.  You can use this method to determine when a portion of
             * the model indicates it has been aborted.
             *
             * Note that this method is called from each thread and *must* be thread safe.
             *
             * \param[in] modelApi A pointer to the model API class.
             *
             * \param[in] threadId The zero based ID of the thread that is now stopping.
             */
            virtual void threadAborted(Api* modelApi, unsigned threadId);

            /**
             * Method that is called by a model to report immediate output.  You can use this to stream output from
             * the model to a given device and channel.  This method is called from the same thread(s) that the model
             * is running in and will block the thread.
             *
             * Note that this method is called from each thread and *must* be thread safe.  The default implementation
             * simply returns.
             *
             * \param[in] device The device to receive the data.
             *
             * \param[in] value  The reported value.
             */
            virtual void sendToDevice(Device device, const Model::Variant& value);
    };

}

#endif
