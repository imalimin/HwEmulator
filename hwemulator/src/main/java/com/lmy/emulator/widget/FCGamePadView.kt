package com.lmy.emulator.widget

import android.content.Context
import android.graphics.Canvas
import android.graphics.Rect
import android.util.AttributeSet
import com.lmy.emulator.entity.HwGamePadEvent

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
        addButton(HwGamePadEvent.KEY_LEFT, Rect(0, measuredHeight - dpToPx(80f + 60f),
                dpToPx(50f), measuredHeight - dpToPx(60f)))
        //Top
        addButton(HwGamePadEvent.KEY_UP, Rect(dpToPx(30f), measuredHeight - dpToPx(80f + 60f + 50f),
                dpToPx(30 + 80f), measuredHeight - dpToPx(80f + 60f)))
        //Right
        addButton(HwGamePadEvent.KEY_RIGHT, Rect(dpToPx(90f), measuredHeight - dpToPx(80f + 60f),
                dpToPx(90f + 50f), measuredHeight - dpToPx(60f)))
        //Bottom
        addButton(HwGamePadEvent.KEY_DOWN, Rect(dpToPx(30f), measuredHeight - dpToPx(60f),
                dpToPx(30 + 80f), measuredHeight - dpToPx(10f)))
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