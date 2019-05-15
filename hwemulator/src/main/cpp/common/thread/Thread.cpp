/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "../include/Thread.h"
#include <assert.h>
#include "../include/log.h"

static void *run(void *arg) {
    Thread *thread = static_cast<Thread *>(arg);
    LOGI("Thread(%ld) start", pthread_self());
    thread->runnable();
    LOGI("Thread(%ld:%s) stop", pthread_self(), thread->name.c_str());
    return nullptr;
}

Thread::Thread(string name, function<void()> runnable) {
    this->inter = false;
    this->name = name;
    this->runnable = runnable;
    pthread_mutex_init(&mutex, nullptr);
}

Thread::~Thread() {
    LOGI("~Thread");
    this->runnable = nullptr;
    stop();
}

void Thread::start() {
    this->inter = false;
    createThread();
}

void Thread::createThread() {
    pthread_attr_init(&attr);
    //将线程的属性称为detached，则线程退出时会自己清理资源
//    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    int ret = pthread_create(&thread, &attr, run, (void *) this);
    if (0 != ret) {
        pthread_attr_destroy(&attr);
        LOGE("Thread create failed: %d", ret);
        assert(false);
    }
}

void Thread::stop() {
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&mutex);
}

bool Thread::isRunning() {
    return !interrupted();
}

void Thread::interrupt() {
    lock();
    if (inter) {
        return;
    }
    inter = true;
    unLock();
    if (pthread_join(thread, 0)) {
        LOGE("Thread(%ld) join failed", thread);
    }
}

bool Thread::interrupted() {
    lock();
    bool ret = inter;
    unLock();
    return ret;
}

void Thread::lock() {
    pthread_mutex_lock(&mutex);
}

void Thread::unLock() {
    pthread_mutex_unlock(&mutex);
}
