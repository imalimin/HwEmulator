package com.lmy.sample.emulator.ui

import android.content.ContentResolver
import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.os.Environment
import android.provider.MediaStore
import android.support.v7.app.AppCompatActivity
import android.text.TextUtils
import android.util.Log
import android.view.SurfaceHolder
import android.widget.Toast
import com.lmy.emulator.HwEmulator
import com.lmy.sample.emulator.R
import kotlinx.android.synthetic.main.activity_main.*
import java.io.File

class GameActivity : AppCompatActivity() {
    private val mEmulator = HwEmulator()
    private var thread: Thread? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        val i = intent
        var uri = i.data
        if (uri == null)
            uri = i.getParcelableExtra(Intent.EXTRA_STREAM)
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
        if (TextUtils.isEmpty(path) || !File(path).exists()) {
            Toast.makeText(this, "没有找到该文件", Toast.LENGTH_SHORT).show()
            finish()
            return
        }
        surfaceView.keepScreenOn = true
        surfaceView.holder.addCallback(object : SurfaceHolder.Callback {
            override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
                Log.i("HWEMULATOR", "surfaceChanged " + width + "x" + height)
                mEmulator.attachWindow(holder.surface)
            }

            override fun surfaceDestroyed(holder: SurfaceHolder?) {
                thread?.interrupt()
                mEmulator.stop()
            }

            override fun surfaceCreated(holder: SurfaceHolder) {
                surfaceView.post {
                    Log.i("HWEMULATOR", "surfaceCreated " + surfaceView.width + "x" + surfaceView.height)
                    mEmulator.prepare(path!!)
                    thread = object : Thread() {
                        override fun run() {
                            mEmulator.start()
                        }
                    }.apply { start() }
                }
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
            val cursor = contentResolver.query(uri, arrayOf(MediaStore.MediaColumns.DATA), null, null, null)
            if (null != cursor) {
                cursor.moveToFirst()
                do {
                    val index = cursor.getColumnIndex(MediaStore.MediaColumns.DATA)
                    if (index > -1) {
                        data = cursor.getString(index)
                        return data
                    }
                }while (cursor.moveToNext())
                cursor.close()
            }
        }
        return data
    }
}