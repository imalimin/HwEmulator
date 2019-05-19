package com.lmy.emulator.entity

import android.graphics.*


class HwRegion : Path {
    private val location = PointF(0f, 0f)
    private val region = Region()

    constructor() : super()

    constructor(src: Path) : super(src)

    fun contains(pointF: PointF): Boolean {
        val bounds = RectF()
        computeBounds(bounds, true)
        region.setPath(this, Region(bounds.left.toInt(), bounds.top.toInt(),
                bounds.right.toInt(), bounds.bottom.toInt()))
        return region.contains(pointF.x.toInt(), pointF.y.toInt())
    }

    fun draw(canvas: Canvas?, paint: Paint) {
        canvas?.drawPath(this, paint)
    }

    fun setLocation(x: Float, y: Float) {
        location.set(x, y)
        offset(x, y)
    }

    companion object {
        fun createRect(left: Float, top: Float, right: Float, bottom: Float): HwRegion {
            val rect = RectF(left, top, right, bottom)
            val region = HwRegion()
            region.addRect(RectF(0f, 0f, rect.width(), rect.height()), Path.Direction.CCW)
            region.setLocation(rect.left, rect.top)
            return region
        }

        fun createRect(rect: Rect): HwRegion {
            val region = HwRegion()
            region.addRect(RectF(0f, 0f, rect.width().toFloat(), rect.height().toFloat()), Path.Direction.CCW)
            region.setLocation(rect.left.toFloat(), rect.top.toFloat())
            return region
        }
    }
}