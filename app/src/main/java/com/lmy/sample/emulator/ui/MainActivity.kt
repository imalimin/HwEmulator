package com.lmy.sample.emulator.ui

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import com.lmy.emulator.HwEmulator
import com.lmy.sample.emulator.R

class MainActivity : AppCompatActivity() {
    private val mEmulator = HwEmulator()
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        mEmulator.prepare()
        object : Thread() {
            override fun run() {
                mEmulator.start()
            }
        }.start()
    }

    override fun onDestroy() {
        super.onDestroy()
        mEmulator.stop()
    }
}
