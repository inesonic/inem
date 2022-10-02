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
* This header defines the \ref Model::Api class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef MODEL_API_H
#define MODEL_API_H

#include "model_api_types.h"
#include "model_variant.h"
#include "model_identifier_database.h"
#include "model_rng.h"

extern "C" typedef struct _MatApi MatApi;

namespace Model {
    class Api;
    class Status;

    /**
     * The symbol name of the model allocation function.  After dynamically loading your model, you should resolve a
     * symbol with this name which you can use as a function pointer to the model allocator function.
     */
    static constexpr char allocatorFunctionName[] = "allocator";

    /**
     * The symbol name of the model deallocation function.  You should resolve a symbol with this name to use as a
     * deallocation function.  Call this function to release storage used by your model.
     */
    static constexpr char deallocatorFunctionName[] = "deallocator";

    /**
     * Type you can use to cast the model allocator function address to use as a function pointer.
     *
     * \param[in] matrixApi The matrix API library to be used.
     *
     * \return On success, returns a pointer to your model.  A null pointer is returned on error.
     */
    typedef Api* (*AllocatorFunction)(const MatApi* matrixApi);

    /**
     * Type you can use to cast the model deallocation function address to use as a function pointer.
     *
     * \param[in] model Pointer to the model to be deallocated.
     */
    typedef void (*DeallocatorFunction)(Api* model);

    /**
     * Pure-virtual class you can use to access your model.  Your model will be presented as a pointer to a class of
     * this type.
     */
    class MODEL_PUBLIC_API Api {
        public:
            virtual ~Api() = default;

            /**
             * Method you can use to start the model or algorithm running in the background.
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
            virtual bool start(
                Model::Rng::RngType        rngType,
                const Model::Rng::RngSeed& rngSeed,
                Status*                    status = nullptr
            ) = 0;

            /**
             * Method you can use to check status of the model or algorithm.
             *
             * \return Returns the last reported model operating status.
             */
            virtual Model::State state() const = 0;

            /**
             * Method you can use to forcibly stop the model or algorithm.  The method will wait until the model has
             * actually stopped execution.
             *
             * \return Returns true on success, returns false if the model is not active.
             */
            virtual bool abort() = 0;

            /**
             * Method you can use to block until the model or algorithm completes execution.
             */
            virtual void waitComplete() = 0;

            /**
             * Method that can be called to run the model or algorithm, blocking until the model or algorithm finishes
             * execution.
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
            virtual bool run(
                Model::Rng::RngType        rngType,
                const Model::Rng::RngSeed& rngSeed,
                Status*                    status = nullptr
            ) = 0;

            /**
             * Method you can use to pause the model immediately.
             *
             * \return Returns true on success, returns false if the model is not active.
             */
            virtual bool pause() = 0;

            /**
             * Method you can use to single step the model.
             *
             * \return Returns true on success, returns false if the model is currently active.
             */
            virtual bool singleStep() = 0;

            /**
             * Method you can use to specify a specific location to run-to before stopping.
             *
             * \param[in] operationHandle The operation handle of the location to run-to.  Setting the value to
             *                            Model::invalidOperationHandle will clear the run-to location.
             *
             * \return Returns true on success, returns false if the run-to location is invalid.
             */
            virtual bool setRunToLocation(OperationHandle operationHandle) = 0;

            /**
             * Method you can use to obtain the current run-to location.
             *
             * \return Returns the current run-to location.  A value of Model::invalidOperationHandle is returned
             *         if there is no run-to location set.
             */
            virtual OperationHandle runToLocation() const = 0;

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
            virtual bool setBreakAtOperation(OperationHandle operationHandle, bool nowSet) = 0;

            /**
             * Method you can use to obtain a list of pending operation breakpoints.
             *
             * \param[in]  list The list to be populated.  The list must be pre-allocated with sufficient space to
             *                  store one entry per operation handle.
             *
             * \return Returns the actual length of the list, in entries.
             */
            virtual OperationHandle operationBreakpoints(OperationHandle* list) const = 0;

            /**
             * Method you can use to resume the model after being paused.
             *
             * \return Returns true on success, returns false if the model is not paused.
             */
            virtual bool resume() = 0;

            /**
             * Method that returns the number of threads required by the model.
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
             * Method that returns a database containing information about identifiers within the model.
             *
             * \return Returns a new identifier database instance.
             */
            virtual IdentifierDatabase identifierDatabase() const = 0;

            /**
             * Method you can use to obtain an RNG tied to this model instance.
             *
             * \param[in] rngType The type of RNG to be used by the model.
             *
             * \param[in] rngSeed A value used to seed the PRNG, ignored if a TRNG is used.
             *
             * \return Returns a pointer to a newly created and configured \ref Model::Rng instance.
             */
            virtual Rng* createRng(Rng::RngType rngType, const Rng::RngSeed& rngSeed) const = 0;

            /**
             * Method you can use to obtain an RNG tied to this model instance.  This version uses the MT19937-64 PRNG
             * algorithm and Gcurrent system time as a seed
             *
             * \return Returns a pointer to a newly created and configured \ref Model::Rng instance.
             */
            virtual Rng* createRng() const = 0;

            /**
             * Method you can use to delete a previously created RNG instance.
             *
             * \param[in] rng The random number generator instance to be deleted.
             */
            virtual void deleteRng(Rng* rng) const = 0;
    };
}

#endif
