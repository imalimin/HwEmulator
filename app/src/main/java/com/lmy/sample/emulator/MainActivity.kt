package com.lmy.sample.emulator

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import com.lmy.emulator.HwEmulator

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        HwEmulator().run()
    }
}