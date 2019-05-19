package com.lmy.emulator.widget

import android.content.Context
import android.graphics.Canvas
import android.graphics.Rect
import android.util.AttributeSet
import com.lmy.emulator.entity.HwGamePadEvent
import com.lmy.emulator.entity.HwRegion

class FCGamePadView : GamePadView {
    private var initButton = false

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
    }

    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec)
        //Left
        addButton(HwGamePadEvent.KEY_LEFT, HwRegion().apply {
            moveTo(0f, 0f)
            lineTo(dpToPx(50f).toFloat(), dpToPx(50f).toFloat())
            lineTo(0f, dpToPx(100f).toFloat())
            close()
        })
        //Top
        addButton(HwGamePadEvent.KEY_UP, HwRegion().apply {
            moveTo(0f, 0f)
            lineTo(dpToPx(50f).toFloat(), dpToPx(100f).toFloat())
            lineTo(50f, dpToPx(50f).toFloat())
            close()
        })
        //Right
        addButton(HwGamePadEvent.KEY_RIGHT, HwRegion().apply {
            moveTo(dpToPx(100f).toFloat(), 0f)
            lineTo(dpToPx(100f).toFloat(), dpToPx(100f).toFloat())
            lineTo(50f, dpToPx(50f).toFloat())
            close()
        })
        //Bottom
        addButton(HwGamePadEvent.KEY_DOWN, HwRegion().apply {
            moveTo(0f, dpToPx(100f).toFloat())
            lineTo(dpToPx(100f).toFloat(), dpToPx(100f).toFloat())
            lineTo(50f, dpToPx(50f).toFloat())
            close()
        })
        //A
        addButton(HwGamePadEvent.KEY_A, Rect(measuredWidth - dpToPx(80f), measuredHeight - dpToPx(60f + 50f),
                measuredWidth, measuredHeight - dpToPx(60f)))
        //B
        addButton(HwGamePadEvent.KEY_B, Rect(measuredWidth - dpToPx(80f), measuredHeight - dpToPx(60f),
                measuredWidth, measuredHeight - dpToPx(10f)))
    }

    override fun onDraw(canvas: Canvas?) {
        if (!initButton) {
            initButton = true
        }
        super.onDraw(canvas)
    }
}