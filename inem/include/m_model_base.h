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
* This header defines the \ref M::ModelBase class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_MODEL_BASE_H
#define M_MODEL_BASE_H

#include "m_common.h"
#include "m_api_types.h"
#include "model_identifier_data.h"
#include "model_status.h"
#include "m_api.h"
#include "m_identifier_database.h"
#include "m_per_thread.h"

namespace Model {
    class Status;
}

namespace M {
    /**
     * Pure virtual base class that is used to define the compiler generated customer's model implementation.
     *
     * The class definea 32 distinct virtual functions that can be overloaded for up-to 32 distinct threads.  You need
     * only overload as many functions as you need for the number of threads.
     */
    class M_PUBLIC_API ModelBase {
        public:
            /**
             * Value indicating the maximum number of threads that can be supported.
             */
            static constexpr unsigned _maximumNumberThreads = 32;

            ModelBase();

            virtual ~ModelBase();

            /**
             * Method that is called to set the Api for the model.
             *
             * \param[in] api The API tied to the model.
             */
            void setApi(Api* api);

            /**
             * Method that returns the number of operations.
             *
             * \return Returns the number of required threads.
             */
            virtual unsigned numberThreads() const = 0;

            /**
             * Method that returns the total number of pausable operations in the model.
             *
             * \return Returns the number of pausable operations.  Operation handles will be numbered zero to one less
             *         than this value.
             */
            virtual OperationHandle numberOperationHandles() const = 0;

            /**
             * Method that builds a new identifier database.
             *
             * \return Returns a newly constructed identifier database.
             */
            virtual M::IdentifierDatabase identifierDatabase() = 0;

            /**
             * Method you can call to invoke the correct thread function.
             *
             * \param[in] perThreadInstance Reference to the \ref M::PerThread instance to tie to this thread.
             *
             * \param[in] threadId          The thread ID of the thread to be executed.
             */
            void execute(PerThread& perThreadInstance, unsigned threadId);

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

        protected:
            /**
             * Method that is called to start thread 1.  You are expected to *always* overload this method.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t1(PerThread& _pt) = 0;

            /**
             * Method that is called to start thread 2.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t2(PerThread& _pt);

            /**
             * Method that is called to start thread 3.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t3(PerThread& _pt);

            /**
             * Method that is called to start thread 4.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t4(PerThread& _pt);

            /**
             * Method that is called to start thread 5.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t5(PerThread& _pt);

            /**
             * Method that is called to start thread 6.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t6(PerThread& _pt);

            /**
             * Method that is called to start thread 7.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t7(PerThread& _pt);

            /**
             * Method that is called to start thread 8.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t8(PerThread& _pt);

            /**
             * Method that is called to start thread 9.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t9(PerThread& _pt);

            /**
             * Method that is called to start thread 10.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t10(PerThread& _pt);

            /**
             * Method that is called to start thread 11.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t11(PerThread& _pt);

            /**
             * Method that is called to start thread 12.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t12(PerThread& _pt);

            /**
             * Method that is called to start thread 13.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t13(PerThread& _pt);

            /**
             * Method that is called to start thread 14.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t14(PerThread& _pt);

            /**
             * Method that is called to start thread 15.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t15(PerThread& _pt);

            /**
             * Method that is called to start thread 16.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t16(PerThread& _pt);

            /**
             * Method that is called to start thread 17.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t17(PerThread& _pt);

            /**
             * Method that is called to start thread 18.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t18(PerThread& _pt);

            /**
             * Method that is called to start thread 19.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t19(PerThread& _pt);

            /**
             * Method that is called to start thread 20.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t20(PerThread& _pt);

            /**
             * Method that is called to start thread 21.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t21(PerThread& _pt);

            /**
             * Method that is called to start thread 22.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t22(PerThread& _pt);

            /**
             * Method that is called to start thread 23.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t23(PerThread& _pt);

            /**
             * Method that is called to start thread 24.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t24(PerThread& _pt);

            /**
             * Method that is called to start thread 25.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t25(PerThread& _pt);

            /**
             * Method that is called to start thread 26.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t26(PerThread& _pt);

            /**
             * Method that is called to start thread 27.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t27(PerThread& _pt);

            /**
             * Method that is called to start thread 28.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t28(PerThread& _pt);

            /**
             * Method that is called to start thread 29.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t29(PerThread& _pt);

            /**
             * Method that is called to start thread 30.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t30(PerThread& _pt);

            /**
             * Method that is called to start thread 31.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t31(PerThread& _pt);

            /**
             * Method that is called to start thread 32.
             *
             * \param[in] _pt Class instance holding per-thread constructs needed independently by each thread.
             */
            virtual void t32(PerThread& _pt);

        protected:
            /**
             * Method that checks for pending pause and abort requests.  This inline method is designed to be fully
             * reentrant and thread safe.
             *
             * \param[in] perThread       The per-thread class instance tied to the thread.
             *
             * \param[in] operationHandle The operation handle to be checked.
             */
            inline void c(PerThread& perThread, OperationHandle operationHandle) const {
                perThread.setOperationHandle(operationHandle);
                if (currentPendingEvent) {
                    handleCheck(perThread, operationHandle);
                }
            }

            /**
             * Method that checks for pending pause and abort requests.  This inline method is designed to be fully
             * reentrant and thread safe.
             *
             * \param[in] perThread        The per-thread class instance tied to the thread.
             *
             * \param[in] operationHandle  The operation handle to be checked.
             *
             * \param[in] identifierHandle The instance handle to be checked.
             *
             * \return Returns true if the thread should abort.  Returns false if the thread should not abort.
             */
            inline void c(
                    PerThread&       perThread,
                    OperationHandle  operationHandle,
                    IdentifierHandle identifierHandle
                ) const {
                perThread.setOperationHandle(operationHandle);
                if (currentPendingEvent) {
                    handleCheck(perThread, operationHandle, identifierHandle);
                }
            }

        private:
            /**
             * Method that handles pending events.
             *
             * \param[in] perThread       The per-thread class instance tied to the thread.
             *
             * \param[in] operationHandle The operation handle to be checked.
             */
            void handleCheck(PerThread& perThread, OperationHandle operationHandle) const;

            /**
             * Method that handles pending events.
             *
             * \param[in] perThread        The per-thread class instance tied to the thread.
             *
             * \param[in] operationHandle  The operation handle to be checked.
             *
             * \param[in] identifierHandle The instance handle to be checked.
             */
            void handleCheck(
                PerThread&       perThread,
                OperationHandle  operationHandle,
                IdentifierHandle identifierHandle
            ) const;

            /**
             * Flag indicating that there is at least one pending event.  Placed in the interface rather than the
             * implementation for speed purposes.
             */
            bool currentPendingEvent;

            class Private;
            Private* impl;
    };
};

#endif
