package com.example.gamepadtester2

import android.content.Context
import android.content.SharedPreferences
import org.json.JSONArray
import org.json.JSONObject
import java.text.SimpleDateFormat
import java.util.Date
import java.util.Locale

object HistoryManager {
    private const val PREF_NAME = "GamepadHistory"
    private const val KEY_SESSIONS = "sessions"

    fun saveSession(context: Context, buttonCounts: Map<String, Int>) {
        if (buttonCounts.isEmpty()) return

        val prefs = context.getSharedPreferences(PREF_NAME, Context.MODE_PRIVATE)
        val currentHistory = getHistory(context).toMutableList()

        val sessionJson = JSONObject()
        sessionJson.put("timestamp", System.currentTimeMillis())

        val buttonsJson = JSONObject()
        for ((key, value) in buttonCounts) {
            buttonsJson.put(key, value)
        }
        sessionJson.put("data", buttonsJson)

        currentHistory.add(0, sessionJson.toString())

        val saveList = if (currentHistory.size > 10) currentHistory.subList(0, 10) else currentHistory

        val jsonArray = JSONArray(saveList)
        prefs.edit().putString(KEY_SESSIONS, jsonArray.toString()).apply()
    }

    fun getHistory(context: Context): List<String> {
        val prefs = context.getSharedPreferences(PREF_NAME, Context.MODE_PRIVATE)
        val jsonString = prefs.getString(KEY_SESSIONS, "[]")
        val list = mutableListOf<String>()
        try {
            val jsonArray = JSONArray(jsonString)
            for (i in 0 until jsonArray.length()) {
                list.add(jsonArray.getString(i))
            }
        } catch (e: Exception) {
            e.printStackTrace()
        }
        return list
    }

    fun formatDate(timestamp: Long): String {
        val sdf = SimpleDateFormat("dd/MM/yyyy HH:mm:ss", Locale.getDefault())
        return sdf.format(Date(timestamp))
    }
}