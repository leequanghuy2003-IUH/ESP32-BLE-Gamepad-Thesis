package com.example.gamepadtester2.ble

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.util.AttributeSet
import android.view.View
import kotlin.math.min
import kotlin.math.sqrt

class JoystickView @JvmOverloads constructor(
    context: Context, attrs: AttributeSet? = null
) : View(context, attrs) {

    private val paintBgFill = Paint().apply {
        color = Color.parseColor("#E3F2FD")
        style = Paint.Style.FILL
        isAntiAlias = true
    }
    private val paintBgBorder = Paint().apply {
        color = Color.parseColor("#1565C0")
        style = Paint.Style.STROKE
        strokeWidth = 5f
        isAntiAlias = true
    }
    private val paintThumb = Paint().apply {
        color = Color.parseColor("#1565C0")
        style = Paint.Style.FILL
        isAntiAlias = true
        setShadowLayer(10f, 0f, 5f, Color.GRAY)
    }

    private var centerX = 0f
    private var centerY = 0f
    private var bgRadius = 0f
    private var thumbRadius = 0f
    private var drawX = 0f
    private var drawY = 0f
    private var lastDrawTime = 0L
    private val FRAME_DELAY = 33L // 33ms ~ 30 FPS

    fun updateJoystick(rawX: Int, rawY: Int) {
        if (bgRadius == 0f) return
        val currentTime = System.currentTimeMillis()
        if (currentTime - lastDrawTime < FRAME_DELAY) {
            return
        }
        lastDrawTime = currentTime

        val centerVal = 16383.5f
        val maxDelta = bgRadius - thumbRadius

        var normX = (rawX - centerVal) / centerVal
        var normY = (rawY - centerVal) / centerVal

        val distance = sqrt(normX * normX + normY * normY)

        if (distance > 1f) {
            normX /= distance
            normY /= distance
        }

        drawX = centerX + normX * maxDelta
        drawY = centerY + normY * maxDelta

        invalidate()
    }

    override fun onSizeChanged(w: Int, h: Int, oldw: Int, oldh: Int) {
        super.onSizeChanged(w, h, oldw, oldh)
        centerX = w / 2f
        centerY = h / 2f
        bgRadius = min(w, h) / 2f * 0.9f
        thumbRadius = bgRadius * 0.35f
        drawX = centerX
        drawY = centerY
    }

    override fun onDraw(canvas: Canvas) {
        super.onDraw(canvas)

        canvas.drawCircle(centerX, centerY, bgRadius, paintBgFill)
        canvas.drawCircle(centerX, centerY, bgRadius, paintBgBorder)
        canvas.drawCircle(drawX, drawY, thumbRadius, paintThumb)
    }
}