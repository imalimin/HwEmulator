/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef HARDWAREVIDEOCODEC_RECYCLERBLOCKQUEUE_H
#define HARDWAREVIDEOCODEC_RECYCLERBLOCKQUEUE_H

#include <functional>
#include "HwRecycler.h"
#include "BlockQueue.h"

template<class T>
class RecyclerBlockQueue : public HwRecycler<T> {
public:
    RecyclerBlockQueue(int initCount, function<T *()> initor) : HwRecycler<T>() {
        queue = new BlockQueue<T>();
        recycler = new BlockQueue<T>();
        for (int i = 0; i < initCount; ++i) {
            recycle(initor());
        }
    }

    virtual ~RecyclerBlockQueue() {
    }

    void clear() {
        if (recycler) {
            recycler->clear();
            delete recycler;
            recycler = nullptr;
        }
        if (queue) {
            queue->clear();
            delete queue;
            queue = nullptr;
        }
    }

    /**
     * 消费一个数据
     */
    T *take() {
        return queue->take();
    }

    /**
     * 提供一个数据
     */
    void offer(T *e) {
        queue->offer(e);
    }

    /*
     * 取出一个缓存
     */
    T *takeCache() {
        if (recycler) {
            return recycler->take();
        }
        return nullptr;
    }

    /**
     * 回收一个数据
     */
    void recycle(T *e) {
        recycler->offer(e);
    }

    /**
     * 回收所有数据
     */
    void recycleAll() {
        if (0 == size()) return;
        while (size() > 0) {
            T *e = take();
            if (e) {
                recycle(e);
            }
        }
    }

    int size() {
        return queue->size();
    }

    int getCacheSize() {
        return recycler->size();
    }

    void notify() override {
        Object::notify();
        queue->notify();
        recycler->notify();
    }

private:
    BlockQueue<T> *queue = nullptr;
    BlockQueue<T> *recycler = nullptr;
};

#endif //HARDWAREVIDEOCODEC_RECYCLERBLOCKQUEUE_H
