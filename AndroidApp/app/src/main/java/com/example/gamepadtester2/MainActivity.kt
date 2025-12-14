package com.example.gamepadtester2

import android.content.Intent
import android.graphics.Color
import android.graphics.Typeface
import android.graphics.drawable.GradientDrawable
import android.os.Bundle
import android.util.SparseIntArray
import android.view.Gravity
import android.view.InputDevice
import android.view.KeyEvent
import android.view.MotionEvent
import android.view.View
import android.widget.LinearLayout
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import com.example.gamepadtester2.databinding.ActivityMainBinding
import kotlin.math.abs

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding
    private val buttonCounts = SparseIntArray()
    private var lastDpadState = -1
    private val MAX_NOTIFICATIONS = 5

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        window.decorView.systemUiVisibility = (View.SYSTEM_UI_FLAG_FULLSCREEN
                or View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                or View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.btnExit.setOnClickListener { saveAndExit() }

        binding.btnHistory.setOnClickListener {
            val currentSessionData = HashMap<String, Int>()
            for (i in 0 until buttonCounts.size()) {
                val viewId = buttonCounts.keyAt(i)
                val count = buttonCounts.valueAt(i)
                currentSessionData[getButtonName(viewId)] = count
            }
            val intent = Intent(this, HistoryActivity::class.java)
            intent.putExtra("HISTORY_DATA", currentSessionData)
            startActivity(intent)
        }
    }

    private fun saveAndExit() {
        val sessionData = HashMap<String, Int>()
        for (i in 0 until buttonCounts.size()) {
            val viewId = buttonCounts.keyAt(i)
            val count = buttonCounts.valueAt(i)
            sessionData[getButtonName(viewId)] = count
        }
        HistoryManager.saveSession(this, sessionData)
        finish()
    }

    override fun onBackPressed() {
        saveAndExit()
        super.onBackPressed()
    }

    private fun getButtonName(viewId: Int): String {
        return when(viewId) {
            R.id.btnCross -> "A"
            R.id.btnCircle -> "B"
            R.id.btnSquare -> "X"
            R.id.btnTriangle -> "Y"
            R.id.btnL1 -> "L1"
            R.id.btnR1 -> "R1"
            R.id.btnL2 -> "L2"
            R.id.btnR2 -> "R2"
            R.id.btnL3 -> "L3"
            R.id.btnR3 -> "R3"
            R.id.btnShare -> "SELECT"
            R.id.btnOptions -> "START"
            R.id.btnPS -> "MODE"
            R.id.btnDpadUp -> "UP"
            R.id.btnDpadDown -> "DOWN"
            R.id.btnDpadLeft -> "LEFT"
            R.id.btnDpadRight -> "RIGHT"
            else -> "Unknown"
        }
    }

    private fun incrementAndShow(viewId: Int) {
        val newCount = buttonCounts.get(viewId, 0) + 1
        buttonCounts.put(viewId, newCount)
        val name = getButtonName(viewId)
        val text = "$name đã ấn $newCount lần"
        showFloatingNotification(text)
    }

    private fun showFloatingNotification(text: String) {
        val container = binding.notificationContainer
        while (container.childCount >= MAX_NOTIFICATIONS) {
            container.removeViewAt(0)
        }

        val tv = TextView(this).apply {
            this.text = text
            textSize = 20f
            setTextColor(Color.parseColor("#D50000"))
            setTypeface(null, Typeface.BOLD)
            gravity = Gravity.CENTER

            val bgDrawable = GradientDrawable().apply {
                shape = GradientDrawable.RECTANGLE
                setColor(Color.WHITE)
                cornerRadius = 20f
            }
            background = bgDrawable

            setPadding(16, 8, 16, 8)
            layoutParams = LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.WRAP_CONTENT,
                LinearLayout.LayoutParams.WRAP_CONTENT
            ).apply {
                setMargins(0, 8, 0, 0)
                gravity = Gravity.CENTER_HORIZONTAL
            }
            alpha = 0f
            translationY = 50f
            elevation = 10f
        }

        container.addView(tv)
        tv.animate().alpha(1f).translationY(0f).setDuration(200).start()
        tv.postDelayed({
            if (tv.parent != null) {
                tv.animate().alpha(0f).translationY(-50f).setDuration(300).withEndAction { container.removeView(tv) }.start()
            }
        }, 1000)
    }

    private fun setButtonHighlight(viewId: Int, highlight: Boolean) {
        val v = findViewById<android.widget.TextView>(viewId) ?: return

        if (highlight) {
            v.alpha = 1.0f
            v.setTextColor(Color.WHITE)
            when (viewId) {
                R.id.btnTriangle, R.id.btnCross, R.id.btnSquare, R.id.btnCircle,
                R.id.btnPS -> v.setBackgroundResource(R.drawable.bg_button_circle_solid_blue)
                R.id.btnShare, R.id.btnOptions -> v.setBackgroundResource(R.drawable.bg_button_pill_solid_blue)
                R.id.btnDpadUp, R.id.btnDpadDown -> v.setBackgroundResource(R.drawable.dpad_vertical_solid)
                R.id.btnDpadLeft, R.id.btnDpadRight -> v.setBackgroundResource(R.drawable.dpad_horizontal_solid)
                else -> v.setBackgroundResource(R.drawable.bg_button_solid_blue)
            }

            if (viewId != R.id.btnDpadUp && viewId != R.id.btnDpadDown &&
                viewId != R.id.btnDpadLeft && viewId != R.id.btnDpadRight) {
                incrementAndShow(viewId)
            }
        } else {
            v.alpha = 1.0f
            if (viewId != R.id.btnPS) v.setTextColor(Color.parseColor("#1565C0"))
            else v.setTextColor(Color.WHITE)

            when (viewId) {
                R.id.btnPS -> {
                    v.setTextColor(Color.parseColor("#1565C0"))
                    v.setBackgroundResource(R.drawable.bg_button_circle_surface)
                }
                R.id.btnTriangle, R.id.btnCross, R.id.btnSquare, R.id.btnCircle ->
                    v.setBackgroundResource(R.drawable.bg_button_circle_surface)
                R.id.btnShare, R.id.btnOptions ->
                    v.setBackgroundResource(R.drawable.bg_button_pill_surface)
                R.id.btnDpadUp, R.id.btnDpadDown -> v.setBackgroundResource(R.drawable.dpad_vertical_outline)
                R.id.btnDpadLeft, R.id.btnDpadRight -> v.setBackgroundResource(R.drawable.dpad_horizontal_outline)
                else -> v.setBackgroundResource(R.drawable.bg_button_surface)
            }
        }
    }

    override fun onKeyDown(keyCode: Int, event: KeyEvent?): Boolean {
        if (event?.repeatCount != 0) return super.onKeyDown(keyCode, event)
        val id = mapKeyToId(keyCode)
        if (id != null) {
            setButtonHighlight(id, true)
            return true
        }
        return super.onKeyDown(keyCode, event)
    }

    override fun onKeyUp(keyCode: Int, event: KeyEvent?): Boolean {
        val id = mapKeyToId(keyCode)
        if (id != null) {
            setButtonHighlight(id, false)
            return true
        }
        return super.onKeyUp(keyCode, event)
    }

    private fun mapKeyToId(keyCode: Int): Int? {
        return when (keyCode) {
            KeyEvent.KEYCODE_BUTTON_A -> R.id.btnCross
            KeyEvent.KEYCODE_BUTTON_B -> R.id.btnCircle
            KeyEvent.KEYCODE_BUTTON_X -> R.id.btnSquare
            KeyEvent.KEYCODE_BUTTON_Y -> R.id.btnTriangle
            KeyEvent.KEYCODE_BUTTON_L1 -> R.id.btnL1
            KeyEvent.KEYCODE_BUTTON_R1 -> R.id.btnR1
            KeyEvent.KEYCODE_BUTTON_L2 -> R.id.btnL2
            KeyEvent.KEYCODE_BUTTON_R2 -> R.id.btnR2
            KeyEvent.KEYCODE_BUTTON_SELECT -> R.id.btnShare
            KeyEvent.KEYCODE_BUTTON_START  -> R.id.btnOptions
            KeyEvent.KEYCODE_BUTTON_MODE   -> R.id.btnPS
            KeyEvent.KEYCODE_BUTTON_THUMBL -> R.id.btnL3
            KeyEvent.KEYCODE_BUTTON_THUMBR -> R.id.btnR3
            else -> null
        }
    }

    override fun onGenericMotionEvent(event: MotionEvent): Boolean {
        if ((event.source and InputDevice.SOURCE_CLASS_JOYSTICK) != 0
            && event.action == MotionEvent.ACTION_MOVE) {

            val lx = event.getAxisValue(MotionEvent.AXIS_X)
            val ly = event.getAxisValue(MotionEvent.AXIS_Y)
            val rx = event.getAxisValue(MotionEvent.AXIS_Z)
            val ry = event.getAxisValue(MotionEvent.AXIS_RZ)

            val center = 16383f
            val cleanLx = if (abs(lx) < 0.1) 0f else lx
            val cleanLy = if (abs(ly) < 0.1) 0f else ly
            val cleanRx = if (abs(rx) < 0.1) 0f else rx
            val cleanRy = if (abs(ry) < 0.1) 0f else ry

            binding.leftJoystick.updateJoystick(((cleanLx + 1f) * center).toInt(), ((cleanLy + 1f) * center).toInt())
            binding.rightJoystick.updateJoystick(((cleanRx + 1f) * center).toInt(), ((cleanRy + 1f) * center).toInt())

            val hatX = event.getAxisValue(MotionEvent.AXIS_HAT_X)
            val hatY = event.getAxisValue(MotionEvent.AXIS_HAT_Y)

            var currentDpadState = 0
            if (hatX < -0.5f) currentDpadState = 1
            else if (hatX > 0.5f) currentDpadState = 2
            else if (hatY < -0.5f) currentDpadState = 3
            else if (hatY > 0.5f) currentDpadState = 4

            if (currentDpadState != lastDpadState) {
                setButtonHighlight(R.id.btnDpadLeft, false)
                setButtonHighlight(R.id.btnDpadRight, false)
                setButtonHighlight(R.id.btnDpadUp, false)
                setButtonHighlight(R.id.btnDpadDown, false)

                if (currentDpadState == 1) { setButtonHighlight(R.id.btnDpadLeft, true); incrementAndShow(R.id.btnDpadLeft) }
                else if (currentDpadState == 2) { setButtonHighlight(R.id.btnDpadRight, true); incrementAndShow(R.id.btnDpadRight) }
                else if (currentDpadState == 3) { setButtonHighlight(R.id.btnDpadUp, true); incrementAndShow(R.id.btnDpadUp) }
                else if (currentDpadState == 4) { setButtonHighlight(R.id.btnDpadDown, true); incrementAndShow(R.id.btnDpadDown) }

                lastDpadState = currentDpadState
            }
            return true
        }
        return super.onGenericMotionEvent(event)
    }
}