package com.lmy.sample.emulator.ui

import android.content.ContentResolver
import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.os.Environment
import android.provider.MediaStore
import android.support.v7.app.AppCompatActivity
import android.text.TextUtils
import android.view.MotionEvent
import android.view.SurfaceHolder
import android.view.View
import android.widget.Toast
import com.lmy.emulator.HwEmulator
import com.lmy.sample.emulator.R
import kotlinx.android.synthetic.main.activity_main.*
import java.io.File

class MainActivity : AppCompatActivity() {
    private val mEmulator = HwEmulator()
    private var thread: Thread? = null
    private val onTouchListener = View.OnTouchListener { v, event ->
        if (MotionEvent.ACTION_DOWN != event.action &&
                MotionEvent.ACTION_UP != event.action) {
            return@OnTouchListener false
        }
        val action = when (event.action) {
            MotionEvent.ACTION_DOWN -> 0x101
            MotionEvent.ACTION_UP -> 0x102
            else -> 0x000
        }
        when (v.id) {
            R.id.selectBtn -> mEmulator.postEvent(0x005, action)
            R.id.startBtn -> mEmulator.postEvent(0x006, action)
            R.id.aBtn -> mEmulator.postEvent(0x007, action)
            R.id.bBtn -> mEmulator.postEvent(0x008, action)
            R.id.upBtn -> mEmulator.postEvent(0x003, action)
            R.id.downBtn -> mEmulator.postEvent(0x004, action)
            R.id.leftBtn -> mEmulator.postEvent(0x002, action)
            R.id.rightBtn -> mEmulator.postEvent(0x001, action)
        }
        true
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        var uri = intent.data
        if (uri == null)
            uri = intent.getParcelableExtra(Intent.EXTRA_STREAM)
        if (uri == null) {
            val testFile = File(Environment.getExternalStorageDirectory(), "tank.nes")
            if (!testFile.exists()) {
                Toast.makeText(this, "没有找到该文件", Toast.LENGTH_SHORT).show()
                finish()
                return
            }
            uri = Uri.fromFile(testFile)
        }
        val path = getRealFilePath(uri)
        if (TextUtils.isEmpty(path)) {
            Toast.makeText(this, "没有找到该文件", Toast.LENGTH_SHORT).show()
            finish()
            return
        }
        selectBtn.setOnTouchListener(onTouchListener)
        startBtn.setOnTouchListener(onTouchListener)
        aBtn.setOnTouchListener(onTouchListener)
        bBtn.setOnTouchListener(onTouchListener)
//        upBtn.setOnTouchListener(onTouchListener)
//        downBtn.setOnTouchListener(onTouchListener)
//        leftBtn.setOnTouchListener(onTouchListener)
//        rightBtn.setOnTouchListener(onTouchListener)
        surfaceView.keepScreenOn = true
        surfaceView.holder.addCallback(object : SurfaceHolder.Callback {
            override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
            }

            override fun surfaceDestroyed(holder: SurfaceHolder?) {
                thread?.interrupt()
                mEmulator.stop()
            }

            override fun surfaceCreated(holder: SurfaceHolder) {
                mEmulator.prepare(path!!, holder.surface)
                thread = object : Thread() {
                    override fun run() {
                        mEmulator.start()
                    }
                }.apply { start() }
            }
        })
        gamePadView.setOnPadEventListener {
            mEmulator.postEvent(it.getKey(), it.getAction())
        }
    }

    override fun onDestroy() {
        super.onDestroy()
    }

    private fun getRealFilePath(uri: Uri?): String? {
        if (null == uri) return null
        val scheme = uri.scheme
        var data: String? = null
        if (scheme == null)
            data = uri.path
        else if (ContentResolver.SCHEME_FILE == scheme) {
            data = uri.path
        } else if (ContentResolver.SCHEME_CONTENT == scheme) {
            val cursor = contentResolver.query(uri, arrayOf(MediaStore.Images.ImageColumns.DATA), null, null, null)
            if (null != cursor) {
                if (cursor.moveToFirst()) {
                    val index = cursor.getColumnIndex(MediaStore.Images.ImageColumns.DATA)
                    if (index > -1) {
                        data = cursor.getString(index)
                    }
                }
                cursor.close()
            }
        }
        return data
    }
}
