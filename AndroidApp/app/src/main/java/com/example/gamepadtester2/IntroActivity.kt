package com.example.gamepadtester2

import android.Manifest
import android.bluetooth.BluetoothAdapter
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.content.pm.PackageManager
import android.hardware.input.InputManager
import android.os.Build
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.provider.Settings
import android.view.InputDevice
import android.view.View
import android.widget.ArrayAdapter
import android.widget.Button
import android.widget.LinearLayout
import android.widget.TextView
import android.widget.Toast
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import com.google.android.material.switchmaterial.SwitchMaterial
import android.graphics.Color

class IntroActivity : AppCompatActivity(), InputManager.InputDeviceListener {

    private var tvStatus: TextView? = null
    private var btnStart: TextView? = null
    private var btnOpenBluetooth: Button? = null
    private var btnHistory: View? = null
    private var switchBluetooth: SwitchMaterial? = null
    private var bluetoothStatusContainer: LinearLayout? = null

    private var inputManager: InputManager? = null
    private var bluetoothAdapter: BluetoothAdapter? = null

    companion object {
        private const val GLOW_NONE = 0
        private const val GLOW_SEARCHING = 1
        private const val GLOW_CONNECTED = 2
    }

    private val bluetoothStateReceiver = object : BroadcastReceiver() {
        override fun onReceive(context: Context?, intent: Intent?) {
            if (BluetoothAdapter.ACTION_STATE_CHANGED == intent?.action) {
                if (tvStatus != null) {
                    updateBluetoothSwitchState()
                    checkGamepadConnection()
                }
            }
        }
    }

    private val requestBluetoothEnable = registerForActivityResult(ActivityResultContracts.StartActivityForResult()) {
        if (tvStatus != null) {
            updateBluetoothSwitchState()
            checkGamepadConnection()
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_intro)

        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter()

        tvStatus = findViewById(R.id.tvConnectionStatus)
        btnStart = findViewById(R.id.btnStart)
        btnOpenBluetooth = findViewById(R.id.btnOpenBluetooth)
        btnHistory = findViewById(R.id.btnHistoryIntro)
        switchBluetooth = findViewById(R.id.switchBluetooth)
        bluetoothStatusContainer = findViewById(R.id.bluetoothStatusContainer)

        inputManager = getSystemService(Context.INPUT_SERVICE) as InputManager

        if (Build.VERSION.SDK_INT <= Build.VERSION_CODES.R) {
            if (ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
                ActivityCompat.requestPermissions(this, arrayOf(Manifest.permission.ACCESS_FINE_LOCATION), 1)
            }
        }

        updateBluetoothSwitchState()

        switchBluetooth?.setOnClickListener {
            handleBluetoothSwitch()
        }

        btnOpenBluetooth?.setOnClickListener { showBluetoothPopup() }

        btnStart?.setOnClickListener {
            val intent = Intent(this, MainActivity::class.java)
            startActivity(intent)
        }

        btnHistory?.setOnClickListener {
            val intent = Intent(this, HistoryActivity::class.java)
            intent.putExtra("HISTORY_DATA", HashMap<String, Int>())
            startActivity(intent)
        }
    }

    private fun handleBluetoothSwitch() {
        if (bluetoothAdapter == null) return

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            if (ActivityCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
                ActivityCompat.requestPermissions(this, arrayOf(Manifest.permission.BLUETOOTH_CONNECT), 2)
                switchBluetooth?.isChecked = !(switchBluetooth?.isChecked ?: false)
                return
            }
        }

        val isTurningOn = switchBluetooth?.isChecked == true

        if (isTurningOn) {
            try {
                if (bluetoothAdapter?.enable() == false) {
                    val enableBtIntent = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
                    requestBluetoothEnable.launch(enableBtIntent)
                }
            } catch (e: SecurityException) {
                val enableBtIntent = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
                requestBluetoothEnable.launch(enableBtIntent)
            }
        } else {
            try {
                if (bluetoothAdapter?.disable() == false) {
                    Toast.makeText(this, "Hệ thống chặn tắt. Vui lòng tắt trong Cài đặt.", Toast.LENGTH_SHORT).show()
                    startActivity(Intent(Settings.ACTION_BLUETOOTH_SETTINGS))
                    switchBluetooth?.isChecked = true
                }
            } catch (e: SecurityException) {
                Toast.makeText(this, "Cần quyền Admin. Vui lòng tắt trong Cài đặt.", Toast.LENGTH_SHORT).show()
                startActivity(Intent(Settings.ACTION_BLUETOOTH_SETTINGS))
                switchBluetooth?.isChecked = true
            }
        }
    }

    override fun onResume() {
        super.onResume()
        inputManager?.registerInputDeviceListener(this, null)
        try {
            registerReceiver(bluetoothStateReceiver, IntentFilter(BluetoothAdapter.ACTION_STATE_CHANGED))
        } catch (e: Exception) { }

        updateBluetoothSwitchState()
        checkGamepadConnection()
    }

    override fun onPause() {
        super.onPause()
        inputManager?.unregisterInputDeviceListener(this)
        try { unregisterReceiver(bluetoothStateReceiver) } catch (e: Exception) {}
    }

    private fun updateBluetoothSwitchState() {
        if (switchBluetooth == null) return

        val isOn = bluetoothAdapter?.isEnabled == true
        if (switchBluetooth?.isChecked != isOn) {
            switchBluetooth?.setOnClickListener(null)
            switchBluetooth?.isChecked = isOn
            switchBluetooth?.setOnClickListener { handleBluetoothSwitch() }
        }

        switchBluetooth?.isEnabled = bluetoothAdapter != null
        switchBluetooth?.text = if (isOn) "Bluetooth: BẬT" else "Bluetooth: TẮT"
        switchBluetooth?.setTextColor(if (isOn) Color.parseColor("#1565C0") else Color.GRAY)
    }

    private fun showBluetoothPopup() {
        if (bluetoothAdapter == null) return

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S && ActivityCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, arrayOf(Manifest.permission.BLUETOOTH_CONNECT, Manifest.permission.BLUETOOTH_SCAN), 1)
            return
        }

        if (bluetoothAdapter?.isEnabled == false) {
            try {
                if (bluetoothAdapter?.enable() == false) {
                    val enableBtIntent = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
                    requestBluetoothEnable.launch(enableBtIntent)
                } else {
                    Toast.makeText(this, "Đang bật Bluetooth...", Toast.LENGTH_SHORT).show()
                }
            } catch (e: SecurityException) {
                val enableBtIntent = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
                requestBluetoothEnable.launch(enableBtIntent)
            }
            return
        }

        val devices = bluetoothAdapter?.bondedDevices?.toList() ?: emptyList()
        val deviceNames = ArrayList<String>()
        for (device in devices) deviceNames.add("🔗 ${device.name} (${device.address})")
        deviceNames.add("🔍 Mở Cài đặt gốc để quét mới...")

        AlertDialog.Builder(this)
            .setTitle("Chọn thiết bị Bluetooth")
            .setAdapter(ArrayAdapter(this, android.R.layout.simple_list_item_1, deviceNames)) { _, which ->
                if (which == deviceNames.lastIndex) startActivity(Intent(Settings.ACTION_BLUETOOTH_SETTINGS))
            }
            .setNegativeButton("Đóng", null)
            .show()
    }

    private fun checkGamepadConnection() {
        if (tvStatus == null) return

        val deviceIds = InputDevice.getDeviceIds()
        var connectedDeviceId = -1

        for (deviceId in deviceIds) {
            val device = InputDevice.getDevice(deviceId) ?: continue
            val sources = device.sources
            if (((sources and InputDevice.SOURCE_GAMEPAD) == InputDevice.SOURCE_GAMEPAD) ||
                ((sources and InputDevice.SOURCE_JOYSTICK) == InputDevice.SOURCE_JOYSTICK)) {
                connectedDeviceId = deviceId
                break
            }
        }

        if (connectedDeviceId != -1) {
            updateUI(true, "Đã kết nối với tay cầm", Color.parseColor("#4CAF50"), GLOW_CONNECTED)
        }
        else {
            if (bluetoothAdapter?.isEnabled == true) {
                updateUI(false, "Đang tìm kiếm thiết bị tay cầm...", Color.parseColor("#FF9800"), GLOW_SEARCHING)
            }
            else {
                updateUI(false, "Vui lòng bật Bluetooth để kết nối", Color.RED, GLOW_NONE)
            }
        }
    }

    private fun updateUI(isReady: Boolean, statusText: String, color: Int, glowType: Int) {
        if (isFinishing || tvStatus == null) return

        tvStatus?.text = statusText
        tvStatus?.setTextColor(color)

        btnStart?.isEnabled = isReady
        btnStart?.isClickable = isReady

        val backgroundDrawable = btnStart?.background?.mutate()
        backgroundDrawable?.setTint(if (isReady) Color.parseColor("#1565C0") else Color.parseColor("#BDBDBD"))
        btnStart?.background = backgroundDrawable

        when (glowType) {
            GLOW_SEARCHING -> bluetoothStatusContainer?.setBackgroundResource(R.drawable.bg_card_glow)
            GLOW_CONNECTED -> bluetoothStatusContainer?.setBackgroundResource(R.drawable.bg_card_connected_glow)
            else -> bluetoothStatusContainer?.setBackgroundResource(R.drawable.bg_card_normal)
        }
    }

    override fun onInputDeviceAdded(deviceId: Int) {
        Handler(Looper.getMainLooper()).postDelayed({
            checkGamepadConnection()
        }, 500)
    }

    override fun onInputDeviceRemoved(deviceId: Int) = checkGamepadConnection()
    override fun onInputDeviceChanged(deviceId: Int) = checkGamepadConnection()
}