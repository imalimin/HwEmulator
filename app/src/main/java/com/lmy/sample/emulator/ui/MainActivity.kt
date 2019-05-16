package com.lmy.sample.emulator.ui

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import com.lmy.emulator.HwEmulator
import com.lmy.sample.emulator.R

class MainActivity : AppCompatActivity() {
    private val mEmulator = HwEmulator()
    private var thread: Thread? = null
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        mEmulator.prepare()
        thread = object : Thread() {
            override fun run() {
                mEmulator.start()
            }
        }
        thread?.start()
    }

    override fun onDestroy() {
        super.onDestroy()
        thread?.interrupt()
        mEmulator.stop()
    }
}
