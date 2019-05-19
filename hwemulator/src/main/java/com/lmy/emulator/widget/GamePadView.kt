package com.lmy.emulator.widget

import android.content.Context
import android.graphics.*
import android.util.AttributeSet
import android.view.MotionEvent
import android.view.View
import com.lmy.emulator.entity.HwGamePadEvent
import com.lmy.emulator.entity.HwRegion


open class GamePadView : View {
    private val buttonMap = HashMap<Int, HwRegion>()
    private val paint = Paint(Paint.ANTI_ALIAS_FLAG)
    private var listener: ((HwGamePadEvent) -> Unit)? = null
    private var lastKey = HwGamePadEvent.KEY_NONE
    private var lastMaskedKey = HwGamePadEvent.KEY_NONE

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

    override fun onTouchEvent(event: MotionEvent): Boolean {
        if (MotionEvent.ACTION_DOWN == event.actionMasked ||
                MotionEvent.ACTION_MOVE == event.actionMasked ||
                MotionEvent.ACTION_UP == event.actionMasked ||
                MotionEvent.ACTION_POINTER_DOWN == event.actionMasked ||
                MotionEvent.ACTION_POINTER_UP == event.actionMasked) {
            for (i in 0 until event.pointerCount) {
                val id = event.getPointerId(i)
                val x = event.getX(id).toInt()
                val y = event.getY(id).toInt()
                buttonMap.map {
                    if (it.value.contains(PointF(x.toFloat(), y.toFloat()))) {
                        if (MotionEvent.ACTION_DOWN == event.actionMasked ||
                                MotionEvent.ACTION_MOVE == event.actionMasked ||
                                MotionEvent.ACTION_UP == event.actionMasked) {
                            if (lastKey != it.key) {
                                listener?.invoke(HwGamePadEvent.create(lastKey, HwGamePadEvent.ACTION_UP))
                            }
                            if (MotionEvent.ACTION_UP == event.actionMasked) {
                                listener?.invoke(HwGamePadEvent.create(it.key, HwGamePadEvent.ACTION_UP))
                            } else {
                                listener?.invoke(HwGamePadEvent.create(it.key, HwGamePadEvent.ACTION_DOWN))
                            }
                            lastKey = it.key
                        }
                        return true
                    }
                }
            }
//            buttonMap.map {
//                if (it.value.contains(event.x.toInt(), event.y.toInt())) {
//                    if (MotionEvent.ACTION_DOWN == event.actionMasked ||
//                            MotionEvent.ACTION_MOVE == event.actionMasked ||
//                            MotionEvent.ACTION_UP == event.actionMasked) {
//                        if (lastKey != it.key) {
//                            listener?.invoke(HwGamePadEvent.create(lastKey, HwGamePadEvent.ACTION_UP))
//                        }
//                        if (MotionEvent.ACTION_UP == event.action) {
//                            listener?.invoke(HwGamePadEvent.create(it.key, HwGamePadEvent.ACTION_UP))
//                        } else {
//                            listener?.invoke(HwGamePadEvent.create(it.key, HwGamePadEvent.ACTION_DOWN))
//                        }
//                        lastKey = it.key
//                    }
//                    if (MotionEvent.ACTION_POINTER_DOWN == event.actionMasked ||
//                            MotionEvent.ACTION_POINTER_UP == event.actionMasked) {
//                        if (lastMaskedKey != it.key) {
//                            listener?.invoke(HwGamePadEvent.create(lastMaskedKey, HwGamePadEvent.ACTION_UP))
//                        }
//                        if (MotionEvent.ACTION_POINTER_UP == event.action) {
//                            listener?.invoke(HwGamePadEvent.create(it.key, HwGamePadEvent.ACTION_UP))
//                        } else {
//                            listener?.invoke(HwGamePadEvent.create(it.key, HwGamePadEvent.ACTION_DOWN))
//                        }
//                        lastMaskedKey = it.key
//
//                    }
//                    return true
//                }
//            }
        }
        return super.onTouchEvent(event)
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