package com.lmy.emulator

class HwEmulator {
    external fun run()

    companion object {
        init {
            System.loadLibrary("hwvcom")
            System.loadLibrary("hwemulator_nes")
            System.loadLibrary("hwemulator_native")
        }
    }
}