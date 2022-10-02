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
* This file implements tests of the \ref Model::Api, \ref M::ModelBase, and \ref Model::Status classes
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QMutex>
#include <QDateTime>
#include <QList>
#include <QSet>

#include <thread>
#include <chrono>

#include <util_hash_functions.h>

#include <model_api.h>
#include <model_status.h>
#include <model_rng.h>
#include <m_identifier_database.h>
#include <m_per_thread.h>
#include <m_model_base.h>
#include <m_api.h>

#include "test_model_api.h"

/***********************************************************************************************************************
 * StatusEvent:
 */

class StatusEvent {
    public:
        /**
         * Enumeration of status event codes.
         */
        enum class EventType {
            INVALID,
            STARTED,
            FINISHED,
            ABORTED,
            PAUSED_ON_USER_REQUEST,
            PAUSED_AT_OPERATION,
            PAUSED_ON_VARIABLE_UPDATE,
            RESUMED,
            THREAD_STARTED,
            THREAD_FINISHED,
            THREAD_PAUSED,
            THREAD_ABORTED
        };

        StatusEvent();

        StatusEvent(EventType eventType, Model::Api* modelApi);

        StatusEvent(EventType eventType, Model::Api* modelApi, unsigned threadId);

        StatusEvent(
            EventType              eventType,
            Model::Api*            modelApi,
            Model::AbortReason     abortReason,
            Model::OperationHandle operationHandle
        );

        StatusEvent(const StatusEvent& other);

        ~StatusEvent();

        EventType eventType() const;

        Model::Api* modelApi() const;

        unsigned threadId() const;

        void setAbortReason(Model::AbortReason newAbortReason);

        Model::AbortReason abortReason() const;

        void setOperationHandle(Model::OperationHandle newOperationHandle);

        Model::OperationHandle operationHandle() const;

        void setIdentifierHandle(Model::IdentifierHandle newIdentifierHandle);

        Model::IdentifierHandle identifierHandle() const;

        StatusEvent& operator=(const StatusEvent& other);

        bool operator==(const StatusEvent& other) const;

        bool operator!=(const StatusEvent& other) const;

    private:
        EventType               currentEventType;
        Model::Api*             currentModelApi;
        unsigned                currentThreadId;
        Model::AbortReason      currentAbortReason;
        Model::OperationHandle  currentOperationHandle;
        Model::IdentifierHandle currentIdentifierHandle;
};

StatusEvent::StatusEvent() {
    currentEventType        = EventType::INVALID;
    currentModelApi         = nullptr;
    currentThreadId         = 0;
    currentAbortReason      = Model::AbortReason::NONE;
    currentOperationHandle  = Model::invalidOperationHandle;
    currentIdentifierHandle = Model::invalidIdentifierHandle;
}


StatusEvent::StatusEvent(StatusEvent::EventType eventType, Model::Api* modelApi) {
    currentEventType        = eventType;
    currentModelApi         = modelApi;
    currentThreadId         = 0;
    currentAbortReason      = Model::AbortReason::NONE;
    currentOperationHandle  = Model::invalidOperationHandle;
    currentIdentifierHandle = Model::invalidIdentifierHandle;
}


StatusEvent::StatusEvent(StatusEvent::EventType eventType, Model::Api* modelApi, unsigned threadId) {
    currentEventType        = eventType;
    currentModelApi         = modelApi;
    currentThreadId         = threadId;
    currentAbortReason      = Model::AbortReason::NONE;
    currentOperationHandle  = Model::invalidOperationHandle;
    currentIdentifierHandle = Model::invalidIdentifierHandle;
}


StatusEvent::StatusEvent(
        EventType              eventType,
        Model::Api*            modelApi,
        Model::AbortReason     abortReason,
        Model::OperationHandle operationHandle
    ) {
    currentEventType        = eventType;
    currentModelApi         = modelApi;
    currentThreadId         = 0;
    currentAbortReason      = abortReason;
    currentOperationHandle  = operationHandle;
    currentIdentifierHandle = Model::invalidIdentifierHandle;
}


StatusEvent::StatusEvent(const StatusEvent& other) {
    currentEventType        = other.currentEventType;
    currentModelApi         = other.currentModelApi;
    currentThreadId         = other.currentThreadId;
    currentAbortReason      = other.currentAbortReason;
    currentOperationHandle  = other.currentOperationHandle;
    currentIdentifierHandle = other.currentIdentifierHandle;
}


StatusEvent::~StatusEvent() {}


StatusEvent::EventType StatusEvent::eventType() const {
    return currentEventType;
}


Model::Api* StatusEvent::modelApi() const {
    return currentModelApi;
}


unsigned StatusEvent::threadId() const {
    return currentThreadId;
}


void StatusEvent::setAbortReason(Model::AbortReason newAbortReason) {
    currentAbortReason = newAbortReason;
}


Model::AbortReason StatusEvent::abortReason() const {
    return currentAbortReason;
}


void StatusEvent::setOperationHandle(Model::OperationHandle newOperationHandle) {
    currentOperationHandle = newOperationHandle;
}


Model::OperationHandle StatusEvent::operationHandle() const {
    return currentOperationHandle;
}


void StatusEvent::setIdentifierHandle(Model::IdentifierHandle newIdentifierHandle) {
    currentIdentifierHandle = newIdentifierHandle;
}


Model::IdentifierHandle StatusEvent::identifierHandle() const {
    return currentIdentifierHandle;
}


StatusEvent& StatusEvent::operator=(const StatusEvent& other) {
    currentEventType        = other.currentEventType;
    currentModelApi         = other.currentModelApi;
    currentThreadId         = other.currentThreadId;
    currentAbortReason      = other.currentAbortReason;
    currentOperationHandle  = other.currentOperationHandle;
    currentIdentifierHandle = other.currentIdentifierHandle;

    return *this;
}


bool StatusEvent::operator==(const StatusEvent& other) const {
    return (
           currentEventType        == other.currentEventType
        && currentModelApi         == other.currentModelApi
        && currentThreadId         == other.currentThreadId
        && currentAbortReason      == other.currentAbortReason
        && currentOperationHandle  == other.currentOperationHandle
        && currentIdentifierHandle == other.currentIdentifierHandle
    );
}


bool StatusEvent::operator!=(const StatusEvent& other) const {
    return (
           currentEventType        != other.currentEventType
        || currentModelApi         != other.currentModelApi
        || currentThreadId         != other.currentThreadId
        || currentAbortReason      != other.currentAbortReason
        || currentOperationHandle  != other.currentOperationHandle
        || currentIdentifierHandle != other.currentIdentifierHandle
    );
}


Util::HashResult qHash(const StatusEvent& event, Util::HashSeed seed = 0) {
    return (
          qHash(static_cast<unsigned>(event.eventType()), seed)
        ^ qHash(event.modelApi(), seed)
        ^ qHash(event.threadId(), seed)
        ^ qHash(static_cast<unsigned>(event.abortReason()), seed)
        ^ qHash(event.operationHandle(), seed)
        ^ qHash(event.identifierHandle(), seed)
    );
}

/***********************************************************************************************************************
 * Status:
 */

class Status:public Model::Status {
    public:
        Status();

        ~Status() final;

        void started(Model::Api* modelApi) final;

        void finished(Model::Api* modelApi) final;

        void aborted(
            Model::Api*            modelApi,
            Model::AbortReason     abortReason,
            Model::OperationHandle operationHandle
        ) final;

        void pausedOnUserRequest(Model::Api* modelApi, Model::OperationHandle operationHandle) final;

        void pausedAtOperation(Model::Api* modelApi, Model::OperationHandle operationHandle) final;

        void pausedOnVariableUpdate(
            Model::Api*             modelApi,
            Model::OperationHandle  operationHandle,
            Model::IdentifierHandle identifierHandle
        ) final;

        void resumed(Model::Api* modelApi) final;

        void threadStarted(Model::Api* modelApi, unsigned threadId) final;

        void threadFinished(Model::Api* modelApi, unsigned threadId) final;

        void threadPaused(Model::Api* modelApi, unsigned threadId) final;

        void threadAborted(Model::Api* modelApi, unsigned threadId) final;

        QList<StatusEvent> events() const;

        void clearEvents();

    private:
        mutable QMutex     mutex;
        QList<StatusEvent> currentEvents;
};


Status::Status() {}


Status::~Status() {}


void Status::started(Model::Api* modelApi) {
    QMutexLocker locker(&mutex);
    currentEvents.append(StatusEvent(StatusEvent::EventType::STARTED, modelApi));
}


void Status::finished(Model::Api* modelApi) {
    QMutexLocker locker(&mutex);
    currentEvents.append(StatusEvent(StatusEvent::EventType::FINISHED, modelApi));
}


void Status::aborted(
        Model::Api*            modelApi,
        Model::AbortReason     abortReason,
        Model::OperationHandle operationHandle
    ) {
    QMutexLocker locker(&mutex);
    currentEvents.append(StatusEvent(StatusEvent::EventType::ABORTED, modelApi, abortReason, operationHandle));
}


void Status::pausedOnUserRequest(Model::Api* modelApi, Model::OperationHandle operationHandle) {
    QMutexLocker locker(&mutex);
    StatusEvent newStatusEvent(StatusEvent::EventType::PAUSED_ON_USER_REQUEST, modelApi);
    newStatusEvent.setOperationHandle(operationHandle);
    currentEvents.append(newStatusEvent);
}


void Status::pausedAtOperation(Model::Api* modelApi, Model::OperationHandle operationHandle) {
    QMutexLocker locker(&mutex);
    StatusEvent newStatusEvent(StatusEvent::EventType::PAUSED_AT_OPERATION, modelApi);
    newStatusEvent.setOperationHandle(operationHandle);
    currentEvents.append(newStatusEvent);
}


void Status::pausedOnVariableUpdate(
        Model::Api*             modelApi,
        Model::OperationHandle  operationHandle,
        Model::IdentifierHandle identifierHandle
    ) {
    QMutexLocker locker(&mutex);
    StatusEvent newStatusEvent(StatusEvent::EventType::PAUSED_ON_VARIABLE_UPDATE, modelApi);
    newStatusEvent.setOperationHandle(operationHandle);
    newStatusEvent.setIdentifierHandle(identifierHandle);
    currentEvents.append(newStatusEvent);
}


void Status::resumed(Model::Api* modelApi) {
    QMutexLocker locker(&mutex);
    currentEvents.append(StatusEvent(StatusEvent::EventType::RESUMED, modelApi));
}


void Status::threadStarted(Model::Api* modelApi, unsigned threadId) {
    QMutexLocker locker(&mutex);
    currentEvents.append(StatusEvent(StatusEvent::EventType::THREAD_STARTED, modelApi, threadId));
}


void Status::threadFinished(Model::Api* modelApi, unsigned threadId) {
    QMutexLocker locker(&mutex);
    currentEvents.append(StatusEvent(StatusEvent::EventType::THREAD_FINISHED, modelApi, threadId));
}


void Status::threadPaused(Model::Api* modelApi, unsigned threadId) {
    QMutexLocker locker(&mutex);
    currentEvents.append(StatusEvent(StatusEvent::EventType::THREAD_PAUSED, modelApi, threadId));
}


void Status::threadAborted(Model::Api* modelApi, unsigned threadId) {
    QMutexLocker locker(&mutex);
    currentEvents.append(StatusEvent(StatusEvent::EventType::THREAD_ABORTED, modelApi, threadId));
}


QList<StatusEvent> Status::events() const {
    mutex.lock();
    QList<StatusEvent> result = currentEvents;
    mutex.unlock();

    return result;
}


void Status::clearEvents() {
    currentEvents.clear();
}

/***********************************************************************************************************************
 * Model:
 */

class TestModel:public M::ModelBase {
    public:
        TestModel(unsigned numberThreads);

        ~TestModel() override;

        static void resetAllocationCounter();

        static unsigned numberAllocatedModels();

        void reset();

        unsigned numberThreadsExecuted() const;

        void setDelayEnabled(bool nowEnabled);

        bool delayEnabled() const;

        bool didThreadExecute(unsigned threadId) const;

        unsigned numberThreads() const final;

        M::OperationHandle numberOperationHandles() const final;

        M::IdentifierDatabase identifierDatabase() final;

        void t1(M::PerThread& _pt) final;

        void t2(M::PerThread& _pt) final;

        void t3(M::PerThread& _pt) final;

        void t4(M::PerThread& _pt) final;

        void t5(M::PerThread& _pt) final;

        void t6(M::PerThread& _pt) final;

        void t7(M::PerThread& _pt) final;

        void t8(M::PerThread& _pt) final;

        void t9(M::PerThread& _pt) final;

        void t10(M::PerThread& _pt) final;

        void t11(M::PerThread& _pt) final;

        void t12(M::PerThread& _pt) final;

        void t13(M::PerThread& _pt) final;

        void t14(M::PerThread& _pt) final;

        void t15(M::PerThread& _pt) final;

        void t16(M::PerThread& _pt) final;

        void t17(M::PerThread& _pt) final;

        void t18(M::PerThread& _pt) final;

        void t19(M::PerThread& _pt) final;

        void t20(M::PerThread& _pt) final;

        void t21(M::PerThread& _pt) final;

        void t22(M::PerThread& _pt) final;

        void t23(M::PerThread& _pt) final;

        void t24(M::PerThread& _pt) final;

        void t25(M::PerThread& _pt) final;

        void t26(M::PerThread& _pt) final;

        void t27(M::PerThread& _pt) final;

        void t28(M::PerThread& _pt) final;

        void t29(M::PerThread& _pt) final;

        void t30(M::PerThread& _pt) final;

        void t31(M::PerThread& _pt) final;

        void t32(M::PerThread& _pt) final;

    private:
        void bumpCountAndDelay(unsigned threadId, M::PerThread& pt);

        static unsigned             currentNumberAllocatedModels;

        volatile unsigned long long threadMask;
        unsigned                    reportedNumberThreads;
        volatile unsigned           numberExecutedThreads;
        bool                        delayIsEnabled;
        QMutex                      accessMutex;
};


unsigned TestModel::currentNumberAllocatedModels;

TestModel::TestModel(unsigned numberThreads) {
    ++currentNumberAllocatedModels;

    reportedNumberThreads = numberThreads;
    delayIsEnabled        = false;
    numberExecutedThreads = 0;
    threadMask            = 0;
}


TestModel::~TestModel() {
    Q_ASSERT(currentNumberAllocatedModels > 0);
    --currentNumberAllocatedModels;
}


void TestModel::resetAllocationCounter() {
    currentNumberAllocatedModels = 0;
}


unsigned TestModel::numberAllocatedModels() {
    return currentNumberAllocatedModels;
}


void TestModel::reset() {
    numberExecutedThreads = 0;
    threadMask            = 0;
}


unsigned TestModel::numberThreadsExecuted() const {
    return numberExecutedThreads;
}


void TestModel::setDelayEnabled(bool nowEnabled) {
    delayIsEnabled = nowEnabled;
}


bool TestModel::delayEnabled() const {
    return delayIsEnabled;
}


bool TestModel::didThreadExecute(unsigned threadId) const {
    return threadMask & (1ULL << threadId);
}


unsigned TestModel::numberThreads() const {
    return reportedNumberThreads;
}


M::OperationHandle TestModel::numberOperationHandles() const {
    return 4;
}


M::IdentifierDatabase TestModel::identifierDatabase() {
    return Model::IdentifierDatabase();
}


void TestModel::t1(M::PerThread& _pt) {
    bumpCountAndDelay(0, _pt);
}


void TestModel::t2(M::PerThread& _pt) {
    bumpCountAndDelay(1, _pt);
}


void TestModel::t3(M::PerThread& _pt) {
    bumpCountAndDelay(2, _pt);
}


void TestModel::t4(M::PerThread& _pt) {
    bumpCountAndDelay(3, _pt);
}


void TestModel::t5(M::PerThread& _pt) {
    bumpCountAndDelay(4, _pt);
}


void TestModel::t6(M::PerThread& _pt) {
    bumpCountAndDelay(5, _pt);
}


void TestModel::t7(M::PerThread& _pt) {
    bumpCountAndDelay(6, _pt);
}


void TestModel::t8(M::PerThread& _pt) {
    bumpCountAndDelay(7, _pt);
}


void TestModel::t9(M::PerThread& _pt) {
    bumpCountAndDelay(8, _pt);
}


void TestModel::t10(M::PerThread& _pt) {
    bumpCountAndDelay(9, _pt);
}


void TestModel::t11(M::PerThread& _pt) {
    bumpCountAndDelay(10, _pt);
}


void TestModel::t12(M::PerThread& _pt) {
    bumpCountAndDelay(11, _pt);
}


void TestModel::t13(M::PerThread& _pt) {
    bumpCountAndDelay(12, _pt);
}


void TestModel::t14(M::PerThread& _pt) {
    bumpCountAndDelay(13, _pt);
}


void TestModel::t15(M::PerThread& _pt) {
    bumpCountAndDelay(14, _pt);
}


void TestModel::t16(M::PerThread& _pt) {
    bumpCountAndDelay(15, _pt);
}


void TestModel::t17(M::PerThread& _pt) {
    bumpCountAndDelay(16, _pt);
}


void TestModel::t18(M::PerThread& _pt) {
    bumpCountAndDelay(17, _pt);
}


void TestModel::t19(M::PerThread& _pt) {
    bumpCountAndDelay(18, _pt);
}


void TestModel::t20(M::PerThread& _pt) {
    bumpCountAndDelay(19, _pt);
}


void TestModel::t21(M::PerThread& _pt) {
    bumpCountAndDelay(20, _pt);
}


void TestModel::t22(M::PerThread& _pt) {
    bumpCountAndDelay(21, _pt);
}


void TestModel::t23(M::PerThread& _pt) {
    bumpCountAndDelay(22, _pt);
}


void TestModel::t24(M::PerThread& _pt) {
    bumpCountAndDelay(23, _pt);
}


void TestModel::t25(M::PerThread& _pt) {
    bumpCountAndDelay(24, _pt);
}


void TestModel::t26(M::PerThread& _pt) {
    bumpCountAndDelay(25, _pt);
}


void TestModel::t27(M::PerThread& _pt) {
    bumpCountAndDelay(26, _pt);
}


void TestModel::t28(M::PerThread& _pt) {
    bumpCountAndDelay(27, _pt);
}


void TestModel::t29(M::PerThread& _pt) {
    bumpCountAndDelay(28, _pt);
}


void TestModel::t30(M::PerThread& _pt) {
    bumpCountAndDelay(29, _pt);
}


void TestModel::t31(M::PerThread& _pt) {
    bumpCountAndDelay(30, _pt);
}


void TestModel::t32(M::PerThread& _pt) {
    bumpCountAndDelay(31, _pt);
}


void TestModel::bumpCountAndDelay(unsigned threadId, M::PerThread& _pt) {
    accessMutex.lock();
    ++numberExecutedThreads;
    threadMask |= (1ULL << threadId);
    accessMutex.unlock();

    if (delayIsEnabled) {
        unsigned long numberIterationsRemaining = 10 * threadId + 10;
        while (numberIterationsRemaining > 0) {
            c(_pt, 3);
            c(_pt, 0);
            c(_pt, 1);
            c(_pt, 2);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            --numberIterationsRemaining;
        }
    }
}

/***********************************************************************************************************************
 * TestModelApi:
 */

TestModelApi::TestModelApi() {}


TestModelApi::~TestModelApi() {}


void TestModelApi::testInvocation() {
    Status              status;
    Model::Rng::RngSeed rngSeed;

    TestModel::resetAllocationCounter();

    for (unsigned numberThreads=1 ; numberThreads<=M::ModelBase::_maximumNumberThreads ; ++numberThreads) {
        TestModel* model = new TestModel(numberThreads);
        M::Api api(model);
        model->setApi(&api);

        api.run(Model::Rng::RngType::MT19937, rngSeed, &status);
        QCOMPARE(model->numberThreadsExecuted(), numberThreads);

        for (unsigned i=0 ; i<numberThreads ; ++i) {
            QCOMPARE(model->didThreadExecute(i), true);
        }

        for (unsigned i=numberThreads ; i<M::ModelBase::_maximumNumberThreads ; ++i) {
            QCOMPARE(model->didThreadExecute(i), false);
        }
    }

    QCOMPARE(TestModel::numberAllocatedModels(), 0u);
}


void TestModelApi::testWait() {
    Status              status;
    Model::Rng::RngSeed rngSeed;

    TestModel* model = new TestModel(8); // Delay should be at least 800mSec.
    M::Api api(model);
    model->setApi(&api);

    model->setDelayEnabled(true);

    api.start(Model::Rng::RngType::MT216091, rngSeed, &status);

    unsigned long long startMSec = QDateTime::currentMSecsSinceEpoch();
    api.waitComplete();
    unsigned long long endMSec  = QDateTime::currentMSecsSinceEpoch();

    QCOMPARE(model->numberThreadsExecuted(), 8U);

    long long elapsedTimeMSec = endMSec - startMSec;
    QVERIFY(elapsedTimeMSec >= 800);
}


void TestModelApi::testStatus() {
    Status              status;
    Model::Rng::RngSeed rngSeed;

    TestModel* model = new TestModel(8); // Delay should be at least 800mSec.
    M::Api api(model);
    model->setApi(&api);

    model->setDelayEnabled(true);

    api.start(Model::Rng::RngType::MT216091, rngSeed, &status);
    QCOMPARE(api.state(), Model::State::ACTIVE);

    api.waitComplete();
    QCOMPARE(api.state(), Model::State::STOPPED);

    QList<StatusEvent> eventsInOrder = status.events();
    status.clearEvents();

    QSet<StatusEvent> events = QSet<StatusEvent>(eventsInOrder.begin(), eventsInOrder.end());
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::STARTED, &api)));

    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_STARTED, &api, 0)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_STARTED, &api, 1)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_STARTED, &api, 2)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_STARTED, &api, 3)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_STARTED, &api, 4)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_STARTED, &api, 5)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_STARTED, &api, 6)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_STARTED, &api, 7)));

    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_FINISHED, &api, 0)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_FINISHED, &api, 1)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_FINISHED, &api, 2)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_FINISHED, &api, 3)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_FINISHED, &api, 4)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_FINISHED, &api, 5)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_FINISHED, &api, 6)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_FINISHED, &api, 7)));

    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::FINISHED, &api)));



    api.start(Model::Rng::RngType::MT216091, rngSeed, &status);

    QCOMPARE(api.state(), Model::State::ACTIVE);
    api.abort();
    QCOMPARE(api.state(), Model::State::ABORTED);

    eventsInOrder = status.events();
    status.clearEvents();

    events = QSet<StatusEvent>(eventsInOrder.begin(), eventsInOrder.end());
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::STARTED, &api)));

    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_STARTED, &api, 0)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_STARTED, &api, 1)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_STARTED, &api, 2)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_STARTED, &api, 3)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_STARTED, &api, 4)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_STARTED, &api, 5)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_STARTED, &api, 6)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_STARTED, &api, 7)));

    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_ABORTED, &api, 0)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_ABORTED, &api, 1)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_ABORTED, &api, 2)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_ABORTED, &api, 3)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_ABORTED, &api, 4)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_ABORTED, &api, 5)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_ABORTED, &api, 6)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_ABORTED, &api, 7)));

    QVERIFY(
        events.contains(StatusEvent(StatusEvent::EventType::ABORTED, &api, Model::AbortReason::USER_REQUEST, 3))
    );
}


void TestModelApi::testAbort() {
    Status              status;
    Model::Rng::RngSeed rngSeed;

    TestModel* model = new TestModel(8); // Delay should be at least 800mSec.
    M::Api api(model);
    model->setApi(&api);

    model->setDelayEnabled(true);

    api.start(Model::Rng::RngType::MT216091, rngSeed, &status);

    unsigned long long startMSec = QDateTime::currentMSecsSinceEpoch();
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Give a little time for threads to get going.
    api.abort();
    unsigned long long endMSec = QDateTime::currentMSecsSinceEpoch();

    QCOMPARE(api.state(), Model::State::ABORTED);
    QVERIFY(model->numberThreadsExecuted() >= 1); // Fail if nothing at least started.

    long long elapsedTimeMSec = endMSec - startMSec;
    QVERIFY(elapsedTimeMSec >= 100); // Fail if the delay in this routine was not accounted for.
    QVERIFY(elapsedTimeMSec <= 500); // Fail if we're well above our expected delay.
}


void TestModelApi::testPause() {
    Status              status;
    Model::Rng::RngSeed rngSeed;

    TestModel* model = new TestModel(8); // Delay should be at least 800mSec.
    M::Api* api = new M::Api(model);
    model->setApi(api);

    model->setDelayEnabled(true);

    api->start(Model::Rng::RngType::MT216091, rngSeed, &status);
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Give a little time for threads to get going.

    bool success = api->pause();
    QVERIFY(success);

    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Give a little time for threads to pause.
    QVERIFY(api->state() == Model::State::PAUSED_ON_USER_REQUEST);

    QList<StatusEvent> eventsInOrder = status.events();
    status.clearEvents();

    QSet<StatusEvent> events = QSet<StatusEvent>(eventsInOrder.begin(), eventsInOrder.end());
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::STARTED, api)));

    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_STARTED, api, 0)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_STARTED, api, 1)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_STARTED, api, 2)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_STARTED, api, 3)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_STARTED, api, 4)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_STARTED, api, 5)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_STARTED, api, 6)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_STARTED, api, 7)));

    StatusEvent pauseEvent(StatusEvent::EventType::PAUSED_ON_USER_REQUEST, api);
    pauseEvent.setOperationHandle(3);
    QVERIFY(events.contains(pauseEvent));

    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_PAUSED, api, 0)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_PAUSED, api, 1)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_PAUSED, api, 2)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_PAUSED, api, 3)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_PAUSED, api, 4)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_PAUSED, api, 5)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_PAUSED, api, 6)));
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::THREAD_PAUSED, api, 7)));

    success = api->resume();
    QVERIFY(success);

    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Give a little time for threads to get going.
    eventsInOrder = status.events();
    status.clearEvents();

    events = QSet<StatusEvent>(eventsInOrder.begin(), eventsInOrder.end());
    QVERIFY(events.contains(StatusEvent(StatusEvent::EventType::RESUMED, api)));

    api->waitComplete();

    delete api;
}


void TestModelApi::testAbortDuringPause() {
    Status              status;
    Model::Rng::RngSeed rngSeed;

    TestModel* model = new TestModel(8); // Delay should be at least 800mSec.
    M::Api* api = new M::Api(model);
    model->setApi(api);

    model->setDelayEnabled(true);

    api->start(Model::Rng::RngType::MT216091, rngSeed, &status);
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Give a little time for threads to get going.

    bool success = api->pause();
    QVERIFY(success);

    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Give a little time for threads to pause.
    QCOMPARE(api->state(), Model::State::PAUSED_ON_USER_REQUEST);

    unsigned long long startMSec = QDateTime::currentMSecsSinceEpoch();
    api->abort();
    unsigned long long endMSec = QDateTime::currentMSecsSinceEpoch();

    QCOMPARE(api->state(), Model::State::ABORTED);

    long long elapsedTimeMSec = endMSec - startMSec;
    QVERIFY(elapsedTimeMSec <= 200);

    delete api;
}


void TestModelApi::testRunTo() {
    Status              status;
    Model::Rng::RngSeed rngSeed;

    TestModel* model = new TestModel(8); // Delay should be at least 800mSec.
    M::Api* api = new M::Api(model);
    model->setApi(api);

    model->setDelayEnabled(true);

    bool success = api->setRunToLocation(3);
    QCOMPARE(success, true);

    QCOMPARE(api->runToLocation(), 3U);

    api->start(Model::Rng::RngType::MT216091, rngSeed, &status);
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Give a little time for threads to get going.
    QVERIFY(api->state() == Model::State::PAUSED_ON_USER_REQUEST);

    success = api->resume();
    QVERIFY(success);

    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Give a little time for threads to get going.
    Model::State currentState = api->state();
    QCOMPARE(currentState, Model::State::ACTIVE);

    api->waitComplete();

    delete api;
}


void TestModelApi::testSingleStep() {
    Status              status;
    Model::Rng::RngSeed rngSeed;

    TestModel* model = new TestModel(8); // Delay should be at least 800mSec.
    M::Api* api = new M::Api(model);
    model->setApi(api);

    model->setDelayEnabled(true);

    bool success = api->setRunToLocation(3);
    QCOMPARE(success, true);

    QCOMPARE(api->runToLocation(), 3U);

    api->start(Model::Rng::RngType::MT216091, rngSeed, &status);
    std::this_thread::sleep_for(std::chrono::milliseconds(20)); // Give a little time for threads to get going.
    QVERIFY(api->state() == Model::State::PAUSED_ON_USER_REQUEST);

    QList<StatusEvent> statusEvents = status.events();
    QSet<StatusEvent> events(statusEvents.begin(), statusEvents.end());

    StatusEvent p0(StatusEvent::EventType::PAUSED_ON_USER_REQUEST, api);
    p0.setOperationHandle(3);
    QVERIFY(events.contains(p0));

    status.clearEvents();
    success = api->singleStep();
    QVERIFY(success);

    std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Give a little time for threads to get going.
    QVERIFY(api->state() == Model::State::PAUSED_ON_USER_REQUEST);

    statusEvents = status.events();
    events = QSet<StatusEvent>(statusEvents.begin(), statusEvents.end());

    StatusEvent p1(StatusEvent::EventType::PAUSED_ON_USER_REQUEST, api);
    p1.setOperationHandle(0);
    QVERIFY(events.contains(p1));

    status.clearEvents();
    success = api->singleStep();
    QVERIFY(success);

    std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Give a little time for threads to get going.
    QVERIFY(api->state() == Model::State::PAUSED_ON_USER_REQUEST);

    statusEvents = status.events();
    events = QSet<StatusEvent>(statusEvents.begin(), statusEvents.end());

    StatusEvent p2(StatusEvent::EventType::PAUSED_ON_USER_REQUEST, api);
    p2.setOperationHandle(1);
    QVERIFY(events.contains(p2));

    status.clearEvents();
    success = api->resume();
    QVERIFY(success);

    std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Give a little time for threads to get going.
    Model::State currentState = api->state();
    QCOMPARE(currentState, Model::State::ACTIVE);

    api->waitComplete();

    delete api;
}
