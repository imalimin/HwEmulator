package com.lmy.emulator.entity

class HwGamePadEvent private constructor(key: Int, action: Int) {
    private var handler: Long = 0

    init {
        handler = create(key, action)
    }

    fun getKey(): Int {
        if (handler > 0) {
            return getKey(handler)
        }
        return KEY_NONE
    }

    fun getAction(): Int {
        if (handler > 0) {
            return getAction(handler)
        }
        return ACTION_NONE
    }

    private external fun create(key: Int, action: Int): Long
    private external fun getKey(handle: Long): Int
    private external fun getAction(handle: Long): Int

    companion object {
        fun create(key: Int, action: Int): HwGamePadEvent {
            return HwGamePadEvent(key, action)
        }

        const val KEY_NONE = 0x000
        const val KEY_RIGHT = 0x001
        const val KEY_LEFT = 0x002
        const val KEY_UP = 0x003
        const val KEY_DOWN = 0x004
        const val KEY_SELECT = 0x005
        const val KEY_START = 0x006
        const val KEY_A = 0x007
        const val KEY_B = 0x008
        const val KEY_ESC = 0x009
        const val ACTION_NONE = 0x000
        const val ACTION_DOWN = 0x101
        const val ACTION_UP = 0x102
    }
}