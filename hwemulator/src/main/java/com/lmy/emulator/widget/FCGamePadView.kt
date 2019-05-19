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
            setLocation(dpToPx(30f).toFloat(), measuredHeight - dpToPx(130f).toFloat())
        })
        //Top
        addButton(HwGamePadEvent.KEY_UP, HwRegion().apply {
            moveTo(0f, 0f)
            lineTo(dpToPx(50f).toFloat(), dpToPx(50f).toFloat())
            lineTo(dpToPx(100f).toFloat(), dpToPx(0f).toFloat())
            close()
            setLocation(dpToPx(30f).toFloat(), measuredHeight - dpToPx(130f).toFloat())
        })
        //Right
        addButton(HwGamePadEvent.KEY_RIGHT, HwRegion().apply {
            moveTo(dpToPx(100f).toFloat(), 0f)
            lineTo(dpToPx(100f).toFloat(), dpToPx(100f).toFloat())
            lineTo(dpToPx(50f).toFloat(), dpToPx(50f).toFloat())
            close()
            setLocation(dpToPx(30f).toFloat(), measuredHeight - dpToPx(130f).toFloat())
        })
        //Bottom
        addButton(HwGamePadEvent.KEY_DOWN, HwRegion().apply {
            moveTo(0f, dpToPx(100f).toFloat())
            lineTo(dpToPx(100f).toFloat(), dpToPx(100f).toFloat())
            lineTo(dpToPx(50f).toFloat(), dpToPx(50f).toFloat())
            close()
            setLocation(dpToPx(30f).toFloat(), measuredHeight - dpToPx(130f).toFloat())
        })
        //A
        addButton(HwGamePadEvent.KEY_A, HwRegion.createRect(0f, 0f,
                dpToPx(80f).toFloat(), dpToPx(50f).toFloat()).apply {
            setLocation(measuredWidth - dpToPx(80f).toFloat(),
                    measuredHeight - dpToPx(100f).toFloat())
        })
        //B
        addButton(HwGamePadEvent.KEY_B, HwRegion.createRect(0f, 0f,
                dpToPx(80f).toFloat(), dpToPx(50f).toFloat()).apply {
            setLocation(measuredWidth - dpToPx(80f).toFloat(),
                    measuredHeight - dpToPx(50f).toFloat())
        })
        //Select
        addButton(HwGamePadEvent.KEY_SELECT, HwRegion.createRect(0f, 0f,
                dpToPx(80f).toFloat(), dpToPx(50f).toFloat()).apply {
            setLocation((measuredWidth - dpToPx(80f).toFloat()) / 2,
                    measuredHeight - dpToPx(100f).toFloat())
        })
        //Start
        addButton(HwGamePadEvent.KEY_START, HwRegion.createRect(0f, 0f,
                dpToPx(80f).toFloat(), dpToPx(50f).toFloat()).apply {
            setLocation((measuredWidth - dpToPx(80f).toFloat()) / 2,
                    measuredHeight - dpToPx(50f).toFloat())
        })
    }

    override fun onDraw(canvas: Canvas?) {
        if (!initButton) {
            initButton = true
        }
        super.onDraw(canvas)
    }
}