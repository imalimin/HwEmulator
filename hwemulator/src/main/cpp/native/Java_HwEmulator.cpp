/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include <jni.h>
#include "HwNESEmulator.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_lmy_emulator_HwEmulator_run
        (JNIEnv *env, jobject thiz) {
    HwNESEmulator *emulator = new HwNESEmulator();
    emulator->prepare("/sdcard/yx.nes");
}

#ifdef __cplusplus
}
#endif