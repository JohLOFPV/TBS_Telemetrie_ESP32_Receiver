# ESP32 TBS Crossfire/Tracer MAVLink Telemetry Decoder


This repository contains a **non-blocking MAVLink telemetry decoder** for the ESP32. It is specifically designed to bridge flight controllers (Betaflight, INAV, ArduPilot, PX4) by intercepting the UDP telemetry stream from the integrated WiFi of a **Team BlackSheep (TBS) Crossfire or Tracer TX module**.

## 🚀 Features
- **Asynchronous UDP:** Uses `AsyncUDP` to handle incoming packets in the background. Your `loop()` stays 100% free for other tasks (OLEDs, LEDs, etc.).
- **Real-Time Decoding:** Supports the most critical MAVLink messages used in FPV and drone systems.
- **Auto-Parsing:** Features a robust state-machine parser to handle fragmented or high-speed data streams.
- **TBS Optimized:** Pre-configured for the default TBS WiFi telemetry port (`8888`).

## 📡 Decoded Message IDs
The script currently supports and prints the following data:
- **ID 0 (HEARTBEAT):** Flight mode and system status.
- **ID 1 (SYS_STATUS):** Battery voltage (mV) and CPU load.
- **ID 24 (GPS_RAW_INT):** Latitude, Longitude, and Altitude.
- **ID 30 (ATTITUDE):** Roll, Pitch, and Yaw (converted from Radians to Degrees).
- **ID 109 (RADIO_STATUS):** RSSI and Noise levels for link monitoring.
- **ID 147 (BATTERY_STATUS):** Remaining %, Current draw (A), and advanced battery info.

## 🛠️ Requirements
- **Hardware:** [ESP32 Development Board](www.espressif.com)
- **Library:** [MAVLink v2 by Oleg Kalachev](https://github.com/okalachev/mavlink-arduino?utm_source=platformio&utm_medium=piohome) (Available in the Arduino Library Manager).
- **Core:** ESP32 Arduino Core (AsyncUDP is included by default).

## 📥 Quick Start
1. **Install Library:** Open Arduino IDE -> Library Manager -> Search for "MAVLink" (v2.0.25 or newer) and install it.
2. **WiFi Setup:** Update the `ssid` , `password` and `localPort` in the script to match your TBS TX WiFi settings.
3. **Flash:** Upload the code to your ESP32.
4. **Monitor:** Open Serial Monitor at **115200 baud**.

```cpp
// Change these in the script:
const char* ssid = "tbs_crossfire_XXXXXXXXXX"; 
const char* password = "";      // Default is often empty
unsigned int localPort = 8888;  // Default TBS Port
```