/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */
package com.lmy.sample.emulator

import android.app.Application
import com.tencent.bugly.crashreport.CrashReport

/**
 * Created by lmyooyo@gmail.com on 2018/5/29.
 */
class MyApplication : Application() {
    override fun onCreate() {
        super.onCreate()
        CrashReport.initCrashReport(applicationContext, "43ddfdd702", true)
    }
}