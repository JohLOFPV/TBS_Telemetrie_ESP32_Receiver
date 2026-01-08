#include <Arduino.h>
#include <WiFi.h>
#include <AsyncUDP.h>
#include <MAVLink.h>            // Include MAVLink by Oleg Kalachev 2.0.25

const char* ssid = "";          //enter your WIFI SSID (default: tbs_crossfire_XXXXXXXXXX)
const char* password = "";      //enter your WIFI password
unsigned int localPort = 8888;  //enter your Telemetry Port (default: 8888)

AsyncUDP udp;

float pi = 3.1415962;

void handleMavlinkMessage(mavlink_message_t* msg);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");

  // Start Asynchronous Listening
  if(udp.listen(localPort)) {
    Serial.printf("Async UDP listening on port %d\n", localPort);
    
    // Callback: This runs automatically whenever a packet is received
    udp.onPacket([](AsyncUDPPacket packet) {
      mavlink_message_t msg;
      mavlink_status_t status;
      
      // Parse the bytes from the async packet
      for (size_t i = 0; i < packet.length(); i++) {
        if (mavlink_parse_char(MAVLINK_COMM_0, packet.data()[i], &msg, &status)) {
          handleMavlinkMessage(&msg);
        }
      }
    });
  }
}

void loop() {
  // YOUR MAIN CODE GOES HERE
  // This will NOT be blocked by incoming UDP packets
  static unsigned long lastTick = 0;
  if (millis() - lastTick > 1000) {
   // Serial.println("Main loop is running freely...");
    lastTick = millis();
  }
}

void handleMavlinkMessage(mavlink_message_t* msg) {
  switch (msg->msgid) {
    case MAVLINK_MSG_ID_HEARTBEAT: { // ID 0
      mavlink_heartbeat_t hb;
      mavlink_msg_heartbeat_decode(msg, &hb);
      Serial.printf("[HB] Mode: %d | Status: %d\n", hb.base_mode, hb.system_status);
      break;
    }
    case MAVLINK_MSG_ID_SYS_STATUS: { // ID 1
      mavlink_sys_status_t sys;
      mavlink_msg_sys_status_decode(msg, &sys);
      Serial.printf("[SYS] Bat: %dmV | Load: %d%%\n", sys.voltage_battery, sys.load/10);
      break;
    }
    case MAVLINK_MSG_ID_GPS_RAW_INT: { // ID 24
      mavlink_gps_raw_int_t gps;
      mavlink_msg_gps_raw_int_decode(msg, &gps);
      Serial.printf("[GPS] Lat: %.7f | Lon: %.7f | Alt: %d\n", gps.lat/10000000.0, gps.lon/10000000.0, gps.alt);
      break;
    }
    case MAVLINK_MSG_ID_ATTITUDE: { // ID 30
      mavlink_attitude_t att;
      mavlink_msg_attitude_decode(msg, &att);
      Serial.printf("[ATT] Roll: %.2f | Pitch: %.2f | Yaw: %.2f\n", (att.roll * 180 / pi), (att.pitch * 180 / pi), (att.yaw * 180 / pi));
      break;
    }
    case MAVLINK_MSG_ID_RADIO_STATUS: { // ID 109
      mavlink_radio_status_t radio;
      mavlink_msg_radio_status_decode(msg, &radio);
      Serial.printf("[RADIO] RSSI: %d | Noise: %d\n", radio.rssi, radio.noise);
      break;
    }
    case MAVLINK_MSG_ID_BATTERY_STATUS: { // ID 147
      mavlink_battery_status_t bat;
      mavlink_msg_battery_status_decode(msg, &bat);
      Serial.printf("[BAT] Rem: %d%% | Curr: %.2fA\n", bat.battery_remaining, bat.current_battery/100.0);
      break;
    }
    default:
      // Uncomment to see all received message IDs
      Serial.printf("Received message with ID %d\n", msg->msgid);
      break;
  }
}
