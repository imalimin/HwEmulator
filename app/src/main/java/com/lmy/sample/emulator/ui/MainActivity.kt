package com.lmy.sample.emulator.ui

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.view.SurfaceHolder
import com.lmy.emulator.HwEmulator
import com.lmy.sample.emulator.R
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {
    private val mEmulator = HwEmulator()
    private var thread: Thread? = null
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        surfaceView.keepScreenOn = true
        surfaceView.holder.addCallback(object : SurfaceHolder.Callback {
            override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
            }

            override fun surfaceDestroyed(holder: SurfaceHolder?) {
                thread?.interrupt()
                mEmulator.stop()
            }

            override fun surfaceCreated(holder: SurfaceHolder) {
                mEmulator.prepare(holder.surface)
                thread = object : Thread() {
                    override fun run() {
                        mEmulator.start()
                    }
                }.apply { start() }
            }
        })
    }

    override fun onDestroy() {
        super.onDestroy()
    }
}
