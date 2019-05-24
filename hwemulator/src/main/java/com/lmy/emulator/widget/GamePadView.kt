package com.lmy.emulator.widget

import android.content.Context
import android.graphics.*
import android.util.AttributeSet
import android.util.Log
import android.view.MotionEvent
import android.view.View
import com.lmy.emulator.entity.HwGamePadEvent
import com.lmy.emulator.entity.HwRegion


open class GamePadView : View {
    private val buttonMap = HashMap<Int, HwRegion>()
    private val keyActionMap = HashMap<Int, Int>()//<eventId, key>
    private val paint = Paint(Paint.ANTI_ALIAS_FLAG)
    private var listener: ((HwGamePadEvent) -> Unit)? = null
    private var lastKeyMap = HashMap<Int, Int>()

    constructor(context: Context) : super(context) {
        applyStyle()
    }

    constructor(context: Context, attrs: AttributeSet) : super(context, attrs) {
        applyStyle()
    }

    constructor(context: Context, attrs: AttributeSet, defStyleAttr: Int)
            : super(context, attrs, defStyleAttr) {
        applyStyle()
    }

    constructor(context: Context, attrs: AttributeSet, defStyleAttr: Int, defStyleRes: Int
    ) : super(context, attrs, defStyleAttr, defStyleRes) {
        applyStyle()
    }

    private fun applyStyle() {
        paint.style = Paint.Style.STROKE
        paint.color = Color.WHITE
        paint.strokeWidth = 6f
    }

    fun addButton(key: Int, region: HwRegion) {
        buttonMap[key] = region
    }

    fun addButton(key: Int, rect: Rect) {
        buttonMap[key] = HwRegion.createRect(rect)
    }

    private fun print(action: Int) {
        when (action) {
            MotionEvent.ACTION_DOWN -> Log.i("HWEMULATOR", "ACTION_DOWN")
            MotionEvent.ACTION_UP -> Log.i("HWEMULATOR", "ACTION_UP")
            MotionEvent.ACTION_POINTER_DOWN -> Log.i("HWEMULATOR", "ACTION_POINTER_DOWN")
            MotionEvent.ACTION_POINTER_UP -> Log.i("HWEMULATOR", "ACTION_POINTER_UP")
        }
    }

    private fun findKey(x: Float, y: Float): Int {
        buttonMap.map {
            if (it.value.contains(PointF(x, y))) {
                return it.key
            }
        }
        return HwGamePadEvent.KEY_NONE
    }

    private fun postEvent() {
        buttonMap.map {
            if (keyActionMap.containsValue(it.key)) {
                listener?.invoke(HwGamePadEvent.create(it.key, HwGamePadEvent.ACTION_DOWN))
            } else {
                listener?.invoke(HwGamePadEvent.create(it.key, HwGamePadEvent.ACTION_UP))
            }
        }
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
//        when (event.action) {
//            MotionEvent.ACTION_DOWN -> Log.i("HWEMULATOR", "ACTION_DOWN")
//            MotionEvent.ACTION_UP -> Log.i("HWEMULATOR", "ACTION_UP")
//            MotionEvent.ACTION_POINTER_DOWN -> Log.i("HWEMULATOR", "ACTION_POINTER_DOWN")
//            MotionEvent.ACTION_POINTER_UP -> Log.i("HWEMULATOR", "ACTION_POINTER_UP")
//        }
//        when (event.actionMasked) {
//            MotionEvent.ACTION_DOWN -> Log.i("HWEMULATOR", "ACTION_DOWN m")
//            MotionEvent.ACTION_UP -> Log.i("HWEMULATOR", "ACTION_UP m")
//            MotionEvent.ACTION_POINTER_DOWN -> Log.i("HWEMULATOR", "ACTION_POINTER_DOWN m")
//            MotionEvent.ACTION_POINTER_UP -> Log.i("HWEMULATOR", "ACTION_POINTER_UP m")
//        }
        for (i in 0 until event.pointerCount) {
            val id = event.getPointerId(i)
            val x = event.getX(i)
            val y = event.getY(i)
            when (event.actionMasked) {
                MotionEvent.ACTION_DOWN -> {
                    Log.i("HWEMULATOR", "ACTION_DOWN m, $id")
                    keyActionMap[id] = findKey(x, y)
                    postEvent()
                }
                MotionEvent.ACTION_UP -> {
                    Log.i("HWEMULATOR", "ACTION_UP m, $id")
                    keyActionMap.remove(id)
                    postEvent()
                }
                MotionEvent.ACTION_MOVE -> {
                    keyActionMap[id] = findKey(x, y)
                    Log.i("HWEMULATOR", "ACTION_MOVE m, $id")
                    postEvent()
                }
                MotionEvent.ACTION_POINTER_DOWN -> {
                    Log.i("HWEMULATOR", "ACTION_POINTER_DOWN m, $id")
                    keyActionMap[id] = findKey(x, y)
                    postEvent()
                }
                MotionEvent.ACTION_POINTER_UP -> {
                    Log.i("HWEMULATOR", "ACTION_POINTER_UP m, $id")
                    keyActionMap.remove(id)
                    postEvent()
                }
            }
        }
        return true
//        if (MotionEvent.ACTION_DOWN == event.actionMasked ||
//                MotionEvent.ACTION_MOVE == event.actionMasked ||
//                MotionEvent.ACTION_UP == event.actionMasked ||
//                MotionEvent.ACTION_POINTER_DOWN == event.actionMasked ||
//                MotionEvent.ACTION_POINTER_UP == event.actionMasked) {
//            for (i in 0 until event.pointerCount) {
//                val id = event.getPointerId(i)
//                val x = event.getX(i).toInt()
//                val y = event.getY(i).toInt()
//                buttonMap.map {
//                    if (it.value.contains(PointF(x.toFloat(), y.toFloat()))) {
//                        if (MotionEvent.ACTION_DOWN == event.actionMasked ||
//                                MotionEvent.ACTION_MOVE == event.actionMasked ||
//                                MotionEvent.ACTION_UP == event.actionMasked ||
//                                MotionEvent.ACTION_POINTER_DOWN == event.actionMasked ||
//                                MotionEvent.ACTION_POINTER_UP == event.actionMasked) {
//                            if (lastKeyMap.containsKey(it.key) && lastKeyMap[id] != it.key) {
//                                listener?.invoke(HwGamePadEvent.create(lastKeyMap[id]!!, HwGamePadEvent.ACTION_UP))
//                            }
//                            if (MotionEvent.ACTION_UP == event.actionMasked) {
//                                listener?.invoke(HwGamePadEvent.create(it.key, HwGamePadEvent.ACTION_UP))
//                            } else {
//                                listener?.invoke(HwGamePadEvent.create(it.key, HwGamePadEvent.ACTION_DOWN))
//                            }
//                            lastKeyMap[id] = it.key
//                        }
//                        if (MotionEvent.ACTION_UP == event.action) {
//                            lastKeyMap.map { item ->
//                                listener?.invoke(HwGamePadEvent.create(item.key, HwGamePadEvent.ACTION_UP))
//                            }
//                            lastKeyMap.clear()
//                        }
//                    }
//                }
//            }
//            return true
//        }
//        return super.onTouchEvent(event)
    }


    override fun onDraw(canvas: Canvas?) {
        super.onDraw(canvas)
        buttonMap.map {
            it.value.draw(canvas, paint)
        }
    }

    fun dpToPx(dp: Float): Int {
        return Math.round(context.resources.displayMetrics.density * dp)
    }

    fun setOnPadEventListener(listener: (event: HwGamePadEvent) -> Unit) {
        this.listener = listener
    }
}