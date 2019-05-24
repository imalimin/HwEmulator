package com.lmy.emulator

import android.view.Surface

class HwEmulator {
    private var handler: Long = 0

    init {
        handler = create()
    }

    fun prepare(rom: String) {
        if (handler > 0) {
            prepare(handler, rom)
        }
    }

    fun attachWindow(surface: Surface) {
        if (handler > 0) {
            attachWindow(handler, surface)
        }
    }

    fun start() {
        if (handler > 0) {
            start(handler)
        }
    }

    fun stop() {
        if (handler > 0) {
            stop(handler)
        }
    }

    fun postEvent(event: Int, action: Int) {
        if (handler > 0) {
            postEvent(handler, event, action)
        }
    }

    private external fun create(): Long
    private external fun prepare(handler: Long, rom: String): Int
    private external fun attachWindow(handler: Long, surface: Surface)
    private external fun start(handler: Long): Int
    private external fun stop(handler: Long): Int
    private external fun postEvent(handler: Long, event: Int, action: Int)

    companion object {
        init {
            System.loadLibrary("hwvcom")
            System.loadLibrary("hwemulatorbase")
            System.loadLibrary("hwemulator_nes")
            System.loadLibrary("hwemulator_native")
        }
    }
}