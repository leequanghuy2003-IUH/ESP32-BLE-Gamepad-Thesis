package com.example.gamepadtester2

import android.graphics.Color
import android.graphics.Typeface
import android.os.Bundle
import android.view.Gravity
import android.view.View
import android.widget.GridLayout
import android.widget.LinearLayout
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import org.json.JSONObject

class HistoryActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_history)
        findViewById<View>(R.id.btnBack).setOnClickListener {
            finish()
        }

        val containerLayout = findViewById<LinearLayout>(R.id.historyContainer)
        if (containerLayout == null) return

        val currentSessionData = intent.getSerializableExtra("HISTORY_DATA") as? HashMap<String, Int>

        if (currentSessionData != null && currentSessionData.isNotEmpty()) {
            val currentHeader = TextView(this).apply {
                text = "PHIÊN VỪA THAO TÁC (Chưa lưu)"
                textSize = 18f
                setTextColor(Color.YELLOW)
                setBackgroundColor(Color.parseColor("#D32F2F"))
                setPadding(24, 16, 24, 16)
                setTypeface(null, Typeface.BOLD)
                layoutParams = LinearLayout.LayoutParams(
                    LinearLayout.LayoutParams.MATCH_PARENT,
                    LinearLayout.LayoutParams.WRAP_CONTENT
                ).apply { setMargins(0, 0, 0, 0) }
            }
            containerLayout.addView(currentHeader)

            val grid = GridLayout(this).apply {
                columnCount = 2
                alignmentMode = GridLayout.ALIGN_MARGINS
                layoutParams = LinearLayout.LayoutParams(
                    LinearLayout.LayoutParams.MATCH_PARENT,
                    LinearLayout.LayoutParams.WRAP_CONTENT
                )
            }
            for ((name, count) in currentSessionData) {
                grid.addView(createHistoryItem(name, count))
            }
            containerLayout.addView(grid)
        }

        val historyList = HistoryManager.getHistory(this)

        if (historyList.isEmpty() && (currentSessionData == null || currentSessionData.isEmpty())) {
            val emptyView = TextView(this).apply {
                text = "Chưa có lịch sử nào"
                textSize = 18f
                gravity = Gravity.CENTER
                setPadding(32, 64, 32, 32)
            }
            containerLayout.addView(emptyView)
            return
        }

        for (sessionStr in historyList) {
            try {
                val session = JSONObject(sessionStr)
                val timestamp = session.getLong("timestamp")
                val dateStr = HistoryManager.formatDate(timestamp)
                val buttonsData = session.getJSONObject("data")

                val dateHeader = TextView(this).apply {
                    text = "Lịch sử: $dateStr"
                    textSize = 18f
                    setTextColor(Color.WHITE)
                    setBackgroundColor(Color.parseColor("#1976D2"))
                    setPadding(24, 16, 24, 16)
                    setTypeface(null, Typeface.BOLD)
                    layoutParams = LinearLayout.LayoutParams(
                        LinearLayout.LayoutParams.MATCH_PARENT,
                        LinearLayout.LayoutParams.WRAP_CONTENT
                    ).apply { setMargins(0, 32, 0, 0) }
                }
                containerLayout.addView(dateHeader)

                val grid = GridLayout(this).apply {
                    columnCount = 2
                    alignmentMode = GridLayout.ALIGN_MARGINS
                    layoutParams = LinearLayout.LayoutParams(
                        LinearLayout.LayoutParams.MATCH_PARENT,
                        LinearLayout.LayoutParams.WRAP_CONTENT
                    )
                }

                val keys = buttonsData.keys()
                while (keys.hasNext()) {
                    val btnName = keys.next()
                    val count = buttonsData.getInt(btnName)
                    grid.addView(createHistoryItem(btnName, count))
                }
                containerLayout.addView(grid)
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }
    }

    private fun createHistoryItem(name: String, count: Int): View {
        val container = LinearLayout(this).apply {
            orientation = LinearLayout.HORIZONTAL
            layoutParams = GridLayout.LayoutParams().apply {
                width = 0
                height = GridLayout.LayoutParams.WRAP_CONTENT
                columnSpec = GridLayout.spec(GridLayout.UNDEFINED, 1f)
                setMargins(8, 8, 8, 8)
            }
            setPadding(16, 16, 16, 16)
            setBackgroundColor(Color.WHITE)
            elevation = 4f
            gravity = Gravity.CENTER_VERTICAL
        }

        val iconView = View(this).apply {
            layoutParams = LinearLayout.LayoutParams(50, 50)
            when {
                name.contains("A") || name.contains("B") || name.contains("X") || name.contains("Y")
                        || name.contains("MODE") -> setBackgroundResource(R.drawable.bg_button_circle_outline)
                name.contains("L") || name.contains("R") -> setBackgroundResource(R.drawable.bg_button_rect)
                name.contains("UP") || name.contains("DOWN") -> setBackgroundResource(R.drawable.dpad_vertical_outline)
                name.contains("LEFT") || name.contains("RIGHT") -> setBackgroundResource(R.drawable.dpad_horizontal_outline)
                else -> setBackgroundResource(R.drawable.bg_button_pill)
            }
            if (name == "DOWN" || name == "RIGHT") rotation = 180f
        }

        val textView = TextView(this).apply {
            text = "$name: $count"
            textSize = 16f
            setTextColor(Color.parseColor("#1565C0"))
            setPadding(24, 0, 0, 0)
        }

        container.addView(iconView)
        container.addView(textView)
        return container
    }
}