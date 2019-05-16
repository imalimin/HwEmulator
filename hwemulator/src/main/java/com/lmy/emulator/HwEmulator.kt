package com.lmy.emulator

class HwEmulator {
    private var handler: Long = 0

    init {
        handler = create()
    }

    fun prepare() {
        if (handler > 0) {
            prepare(handler)
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

    private external fun create(): Long
    private external fun prepare(handler: Long): Int
    private external fun start(handler: Long): Int
    private external fun stop(handler: Long): Int

    companion object {
        init {
            System.loadLibrary("hwvcom")
            System.loadLibrary("hwemulator_nes")
            System.loadLibrary("hwemulator_native")
        }
    }
}