/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include <jni.h>
#include "HwGamePadEvent.h"

#ifdef __cplusplus
extern "C" {
#endif

static HwGamePadEvent *getHandler(jlong handler) {
    return reinterpret_cast<HwGamePadEvent *>(handler);
}

JNIEXPORT jlong JNICALL Java_com_lmy_emulator_entity_HwGamePadEvent_create
        (JNIEnv *env, jobject thiz, jint key, jint action) {
    return reinterpret_cast<jlong>(HwGamePadEvent::create(key, action));
}

JNIEXPORT jint JNICALL Java_com_lmy_emulator_entity_HwGamePadEvent_getKey
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        return getHandler(handler)->getKey();
    }
    return HwGamePadEvent::KEY_NONE;
}

JNIEXPORT jint JNICALL Java_com_lmy_emulator_entity_HwGamePadEvent_getAction
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        return getHandler(handler)->getAction();
    }
    return HwGamePadEvent::ACTION_NONE;
}

#ifdef __cplusplus
}
#endif