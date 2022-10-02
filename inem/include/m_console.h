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
* This header defines the \ref M::Console class.
***********************************************************************************************************************/

/* .. sphinx-project inem */

#ifndef M_CONSOLE_H
#define M_CONSOLE_H

#include "m_common.h"
#include "model_common.h"
#include "model_set.h"
#include "model_tuple.h"
#include "model_variant.h"
#include "m_api_types.h"
#include "m_intrinsic_types.h"

namespace M {
    class M_PUBLIC_API Set;
    class M_PUBLIC_API Tuple;
    class M_PUBLIC_API Variant;

    /**
     * Class you can use as a console interface.  The model can post messages into the console which can then be
     * intercepted by the application and piped to a user interface.
     */
    class M_PUBLIC_API Console {
        public:
            /**
             * Enumeration of message types.
             */
            enum class MessageType {
                /**
                 * Indicates an invalid message type.
                 */
                INVALID,

                /**
                 * Message provides status on model operation.  Generally intended to report when threads start, stop
                 * or pause.
                 */
                INFORMATION,

                /**
                 * Message provides data.
                 */
                DATA,

                /**
                 * Message provides debugging information.
                 */
                DEBUG,

                /**
                 * Message indicates a warning about a condition during build.
                 */
                BUILD_WARNING,

                /**
                 * Message indicates an error during build.
                 */
                BUILD_ERROR,

                /**
                 * Message indicates a warning condition during execution.
                 */
                RUNTIME_WARNING,

                /**
                 * Message indicates an error condition during execution.
                 */
                RUNTIME_ERROR
            };

            /**
             * Value used to indicate to indicate no thread.
             */
            static constexpr unsigned noThreadId = static_cast<unsigned>(-1);

            /**
             * Pure virtual class you must overload to receive reporting information from the console.
             */
            class M_PUBLIC_API Callback {
                public:
                    /**
                     * Value used to indicate no thread ID is tied to a message.
                     */
                    static constexpr unsigned noThreadId = Console::noThreadId;

                    virtual ~Callback() = default;

                    /**
                     * Method that is called when the model is started.  You can overload this method to perform any
                     * needed per-run initialization.
                     *
                     * \param[in] numberThreads The number of threads being instantiated.
                     */
                    virtual void initialize(unsigned numberThreads) = 0;

                    /**
                     * Method that is called when the console is about to post a new message.  This call should block
                     * until a new message can be posted.
                     *
                     * \param[in] threadId    The thread ID.
                     *
                     * \param[in] messageType The message type.
                     *
                     * \return Returns true if the message should be reported.  Returns false if the message should be
                     *         ignored.
                     */
                    virtual bool startMessage(unsigned threadId, MessageType messageType) = 0;

                    /**
                     * Method that is called when the console has finished posting a message.
                     *
                     * \param[in] threadId The thread ID.
                     */
                    virtual void endMessage(unsigned threadId) = 0;

                    /**
                     * Message that is called to receive a text string.  This call should block until the entire
                     * contents can be accepted.
                     *
                     * \param[in] threadId The thread ID.
                     *
                     * \param[in] newText  A nul terminated text string to be reported.
                     */
                    virtual void append(unsigned threadId, const char* newText) = 0;

                    /**
                     * Method that is called to receive a boolean value.   This call should block until the entire
                     * contents can be accepted.
                     *
                     * \param[in] threadId The thread ID.
                     *
                     * \param[in] value    The value to be reported.
                     */
                    virtual void append(unsigned threadId, M::Boolean value) = 0;

                    /**
                     * Method that is called to receive an signed integer value.   This call should block until the
                     * entire contents can be accepted.
                     *
                     * \param[in] threadId The thread ID.
                     *
                     * \param[in] value    The value to be reported.
                     */
                    virtual void append(unsigned threadId, int value) = 0;

                    /**
                     * Method that is called to receive an unsigned integer value.   This call should block until the
                     * entire contents can be accepted.
                     *
                     * \param[in] threadId The thread ID.
                     *
                     * \param[in] value    The value to be reported.
                     */
                    virtual void append(unsigned threadId, unsigned value) = 0;

                    /**
                     * Method that is called to receive an unsigned integer value.   This call should block until the
                     * entire contents can be accepted.
                     *
                     * \param[in] threadId The thread ID.
                     *
                     * \param[in] value    The value to be reported.
                     */
                    virtual void append(unsigned threadId, unsigned long long value) = 0;

                    /**
                     * Method that is called to receive an integer value.   This call should block until the entire
                     * contents can be accepted.
                     *
                     * \param[in] threadId The thread ID.
                     *
                     * \param[in] value    The value to be reported.
                     */
                    virtual void append(unsigned threadId, M::Integer value) = 0;

                    /**
                     * Method that is called to receive a real value.   This call should block until the entire
                     * contents can be accepted.
                     *
                     * \param[in] threadId The thread ID.
                     *
                     * \param[in] value    The value to be reported.
                     */
                    virtual void append(unsigned threadId, M::Real value) = 0;

                    /**
                     * Method that is called to receive a complex value.   This call should block until the entire
                     * contents can be accepted.
                     *
                     * \param[in] threadId The thread ID.
                     *
                     * \param[in] value    The value to be reported.
                     */
                    virtual void append(unsigned threadId, M::Complex value) = 0;

                    /**
                     * Method that is called to receive a set value.   This call should block until the entire contents
                     * can be accepted.
                     *
                     * \param[in] threadId The thread ID.
                     *
                     * \param[in] value    The value to be reported.
                     */
                    virtual void append(unsigned threadId, const Model::Set& value) = 0;

                    /**
                     * Method that is called to receive a set value.   This call should block until the entire contents
                     * can be accepted.
                     *
                     * \param[in] threadId The thread ID.
                     *
                     * \param[in] value    The value to be reported.
                     */
                    virtual void append(unsigned threadId, const M::Set& value) = 0;

                    /**
                     * Method that is called to receive a tuple value.   This call should block until the entire
                     * contents can be accepted.
                     *
                     * \param[in] threadId The thread ID.
                     *
                     * \param[in] value    The value to be reported.
                     */
                    virtual void append(unsigned threadId, const Model::Tuple& value) = 0;

                    /**
                     * Method that is called to receive a tuple value.   This call should block until the entire
                     * contents can be accepted.
                     *
                     * \param[in] threadId The thread ID.
                     *
                     * \param[in] value    The value to be reported.
                     */
                    virtual void append(unsigned threadId, const M::Tuple& value) = 0;

                    /**
                     * Method that is called to receive a variant value.   This call should block until the entire
                     * contents can be accepted.
                     *
                     * \param[in] threadId The thread ID.
                     *
                     * \param[in] value    The value to be reported.
                     */
                    virtual void append(unsigned threadId, const Model::Variant& value) = 0;

                    /**
                     * Method that is called to receive a variant value.   This call should block until the entire
                     * contents can be accepted.
                     *
                     * \param[in] threadId The thread ID.
                     *
                     * \param[in] value    The value to be reported.
                     */
                    virtual void append(unsigned threadId, const M::Variant& value) = 0;

                    /**
                     * Method that is called to set the base to use for integer values.
                     *
                     * \param[in] threadId The thread ID.
                     *
                     * \param[in] newBase The new base to be used.
                     */
                    virtual void setBase(unsigned threadId, unsigned newBase) = 0;

                    /**
                     * Method that is called to set the width to use for inserted values.
                     *
                     * \param[in] threadId The thread ID.
                     *
                     * \param[in] newWidth The new width to be used.
                     */
                    virtual void setWidth(unsigned threadId, unsigned newWidth) = 0;

                    /**
                     * Method that is called to set the pad character to use for inserted values.
                     *
                     * \param[in] threadId The thread ID.
                     *
                     * \param[in] newPad   The new pad character.
                     */
                    virtual void setPad(unsigned threadId, char newPad) = 0;

                    /**
                     * Method that is called to indicate values should be rendered in upper-case.
                     *
                     * \param[in] threadId The thread ID.
                     */
                    virtual void setUpperCase(unsigned threadId) = 0;

                    /**
                     * Method that is called to indicate values should be rendered in lower-case.
                     *
                     * \param[in] threadId The thread ID.
                     */
                    virtual void setLowerCase(unsigned threadId) = 0;

                    /**
                     * Method that is called to indicate case rendering should be reset.
                     *
                     * \param[in] threadId The thread ID.
                     */
                    virtual void resetCase(unsigned threadId) = 0;
            };

            /**
             * Modifier you can use to set the base for integer values.
             */
            class M_PUBLIC_API Base {
                public:
                    /**
                     * Constructor.
                     *
                     * \param[in] newBase The new base to display.
                     */
                    Base(unsigned newBase = 10);

                    ~Base();

                    /**
                     * Method that reports the desired base.
                     *
                     * \return Returns the desired base.
                     */
                    unsigned base() const;

                private:
                    /**
                     * The current base.
                     */
                    unsigned currentBase;
            };

            /**
             * Modifier you can use to set the desired field width.
             */
            class M_PUBLIC_API Width {
                public:
                    /**
                     * Constructor.
                     *
                     * \param[in] newWidth The new field width.
                     */
                    Width(unsigned newWidth = 0);

                    ~Width();

                    /**
                     * Method that reports the desired width.
                     *
                     * \return Returns the desired width.
                     */
                    unsigned width() const;

                private:
                    /**
                     * The current field width.
                     */
                    unsigned currentWidth;
            };

            /**
             * Modifier you can use to set the desired pad character.
             */
            class M_PUBLIC_API Pad {
                public:
                    /**
                     * Constructor.
                     *
                     * \param[in] newPadCharacter The desired pad character.
                     */
                    Pad(char newPadCharacter = ' ');

                    ~Pad();

                    /**
                     * Method that reports the desired pad character.
                     *
                     * \return Returns the desired pad character.
                     */
                    char pad() const;

                private:
                    /**
                     * The current pad character.
                     */
                    char currentPad;
            };

            /**
             * Modifier you can use to indicate upper case.
             */
            class M_PUBLIC_API UpperCase {};

            /**
             * Modifier you can use to indicate lower case.
             */
            class M_PUBLIC_API LowerCase {};

            /**
             * Modifier you can use to reset case handling.
             */
            class M_PUBLIC_API ResetCase {};

            /**
             * Method you should call to register a callback instance with the console.
             *
             * \param[in] newCallbackInstance The callback instance to receive notification from the console.
             */
            static void registerCallback(Callback* newCallbackInstance);

            /**
             * Method you should call to release the callback instance from the console.
             */
            static void releaseCallback();

            /**
             * Method you can use to obtain a pointer to the current callback instance.
             *
             * \return Returns a pointer to the currently registered callback.
             */
            static Callback* callback();

            /**
             * Static method you should call before starting a new model.  The method informs the console callback that
             * we are starting new execution.
             *
             * \param[in] numberThreads The expected number of threads.
             */
            static void initialize(unsigned numberThreads);

            /**
             * Static method you can use to dump a message to the console.
             *
             * \param[in] threadId       The thread ID.
             *
             * \param[in] messageType    The message type.
             *
             * \param[in] parameters     Variadic arguments representing the payloads to be displayed.
             */
            template<typename... ParameterTypes> M_PUBLIC_TEMPLATE_METHOD static void report(
                    unsigned          threadId,
                    MessageType       messageType,
                    ParameterTypes... parameters
                ) {
                if (startMessage(threadId, messageType)) {
                    send(threadId, parameters...);
                    endMessage(threadId);
                }
            }

            /**
             * Static method you can use to dump a message to the console.  This version will report no thread.
             *
             * \param[in] messageType    The message type.
             *
             * \param[in] parameters     Variadic arguments representing the payloads to be displayed.
             */
            template<typename... ParameterTypes> M_PUBLIC_TEMPLATE_METHOD static void report(
                    MessageType       messageType,
                    ParameterTypes... parameters
                ) {
                report(noThreadId, messageType, parameters...);
            }

        private:
            /**
             * Method that is called to report multiple messages to the callback.
             *
             * \param[in] threadId   The thread ID.
             *
             * \param[in] parameter  The parameter to be reported.
             */
            template<typename Parameter> M_PUBLIC_TEMPLATE_METHOD static void send(
                    unsigned  threadId,
                    Parameter parameter
                ) {
                append(threadId, parameter);
            }

            /**
             * Method that is called to report multiple messages to the callback.
             *
             * \param[in] threadId   The thread ID.
             *
             * \param[in] p1         The parameter to be reported.
             *
             * \param[in] additional The additional parameters.
             */
            template<typename P1, typename... Additional> M_PUBLIC_TEMPLATE_METHOD static void send(
                    unsigned      threadId,
                    P1            p1,
                    Additional... additional
                ) {
                append(threadId, p1);
                send(threadId, additional...);
            }

            /**
             * Method that is called when the console is about to post a new message.  This call should block
             * until a new message can be posted.  This method simply forwards the message to the console callback
             * instance.
             *
             * \param[in] threadId    The thread ID.
             *
             * \param[in] messageType The message type.
             *
             * \return Returns true if the message should be reported.  Returns false if the message should be
             *         ignored.
             */
            static bool startMessage(unsigned threadId, MessageType messageType);

            /**
             * Method that is called when the console has finished posting a message.  This method simply forwards the
             * message to the console callback instance.
             *
             * \param[in] threadId The thread ID.
             */
            static void endMessage(unsigned threadId);

            /**
             * Message that is called to receive a text string.  This call should block until the entire
             * contents can be accepted.  This method simply forwards the message to the console callback instance.
             *
             * \param[in] threadId The thread ID.
             *
             * \param[in] newText  A nul terminated text string to be reported.
             */
            static void append(unsigned threadId, const char* newText);

            /**
             * Method that is called to receive an unsigned integer value.   This call should block until the entire
             * contents can be accepted.  This method simply forwards the message to the console callback instance.
             *
             * \param[in] threadId The thread ID.
             *
             * \param[in] value    The value to be reported.
             */
            static void append(unsigned threadId, unsigned value);

            /**
             * Method that is called to receive an signed integer value.   This call should block until the entire
             * contents can be accepted.  This method simply forwards the message to the console callback instance.
             *
             * \param[in] threadId The thread ID.
             *
             * \param[in] value    The value to be reported.
             */
            static void append(unsigned threadId, int value);

            /**
             * Method that is called to receive an unsigned integer value.   This call should block until the
             * entire contents can be accepted.
             *
             * \param[in] threadId The thread ID.
             *
             * \param[in] value    The value to be reported.
             */
            static void append(unsigned threadId, unsigned long long value);

            /**
             * Method that is called to receive a boolean value.   This call should block until the entire
             * contents can be accepted.  This method simply forwards the message to the console callback instance.
             *
             * \param[in] threadId The thread ID.
             *
             * \param[in] value    The value to be reported.
             */
            static void append(unsigned threadId, M::Boolean value);

            /**
             * Method that is called to receive an integer value.   This call should block until the entire
             * contents can be accepted.  This method simply forwards the message to the console callback instance.
             *
             * \param[in] threadId The thread ID.
             *
             * \param[in] value    The value to be reported.
             */
            static void append(unsigned threadId, M::Integer value);

            /**
             * Method that is called to receive a real value.   This call should block until the entire
             * contents can be accepted.  This method simply forwards the message to the console callback instance.
             *
             * \param[in] threadId The thread ID.
             *
             * \param[in] value    The value to be reported.
             */
            static void append(unsigned threadId, M::Real value);

            /**
             * Method that is called to receive a complex value.   This call should block until the entire
             * contents can be accepted.  This method simply forwards the message to the console callback instance.
             *
             * \param[in] threadId The thread ID.
             *
             * \param[in] value    The value to be reported.
             */
            static void append(unsigned threadId, M::Complex value);

            /**
             * Method that is called to receive a set value.   This call should block until the entire contents
             * can be accepted.  This method simply forwards the message to the console callback instance.
             *
             * \param[in] threadId The thread ID.
             *
             * \param[in] value    The value to be reported.
             */
            static void append(unsigned threadId, const Model::Set& value);

            /**
             * Method that is called to receive a set value.   This call should block until the entire contents
             * can be accepted.  This method simply forwards the message to the console callback instance.
             *
             * \param[in] threadId The thread ID.
             *
             * \param[in] value    The value to be reported.
             */
            static void append(unsigned threadId, const M::Set& value);

            /**
             * Method that is called to receive a tuple value.   This call should block until the entire
             * contents can be accepted.  This method simply forwards the message to the console callback instance.
             *
             * \param[in] threadId The thread ID.
             *
             * \param[in] value    The value to be reported.
             */
            static void append(unsigned threadId, const Model::Tuple& value);

            /**
             * Method that is called to receive a tuple value.   This call should block until the entire
             * contents can be accepted.  This method simply forwards the message to the console callback instance.
             *
             * \param[in] threadId The thread ID.
             *
             * \param[in] value    The value to be reported.
             */
            static void append(unsigned threadId, const M::Tuple& value);

            /**
             * Method that is called to receive a variant value.   This call should block until the entire
             * contents can be accepted.  This method simply forwards the message to the console callback instance.
             *
             * \param[in] threadId The thread ID.
             *
             * \param[in] value    The value to be reported.
             */
            static void append(unsigned threadId, const Model::Variant& value);

            /**
             * Method that is called to receive a variant value.   This call should block until the entire
             * contents can be accepted.  This method simply forwards the message to the console callback instance.
             *
             * \param[in] threadId The thread ID.
             *
             * \param[in] value    The value to be reported.
             */
            static void append(unsigned threadId, const M::Variant& value);

            /**
             * Method that is called to set the desired base.
             *
             * \param[in] threadId The thread ID.
             *
             * \param[in] value    The value to be reported.
             */
            static void append(unsigned threadId, const Base& value);

            /**
             * Method that is called to set the desired width.
             *
             * \param[in] threadId The thread ID.
             *
             * \param[in] value    The value to be reported.
             */
            static void append(unsigned threadId, const Width& value);

            /**
             * Method that is called to set the desired padding.
             *
             * \param[in] threadId The thread ID.
             *
             * \param[in] value    The value to be reported.
             */
            static void append(unsigned threadId, const Pad& value);

            /**
             * Method that is called to set upper case rendering of values.
             *
             * \param[in] threadId The thread ID.
             *
             * \param[in] value    The value to be reported.
             */
            static void append(unsigned threadId, const UpperCase& value);

            /**
             * Method that is called to set lower case rendering of values.
             *
             * \param[in] threadId The thread ID.
             *
             * \param[in] value    The value to be reported.
             */
            static void append(unsigned threadId, const LowerCase& value);

            /**
             * Method that is called to reset case rendering of values.
             *
             * \param[in] threadId The thread ID.
             *
             * \param[in] value    The value to be reported.
             */
            static void append(unsigned threadId, const ResetCase& value);

            /**
             * The active console callback instance.
             */
            static Callback* currentCallback;
    };
}

#endif
