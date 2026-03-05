/*********
  ESP32 Combined: DHT11 + Soil Moisture + Relay Pump Control
  - DHT11      : Pin 4
  - Soil       : Pin 34
  - Relay      : Pin 18 (NO - Normally Open)
*********/
#define ENABLE_USER_AUTH
#define ENABLE_DATABASE

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <FirebaseClient.h>
#include "DHT.h"
#include <time.h>

// ================= WIFI =================
#define WIFI_SSID       "POCO"
#define WIFI_PASSWORD   "12345678"

// ================= FIREBASE =================
#define Web_API_KEY     "AIzaSyC4CEMA9XeWvv92l1nwl2IP9hCn99ffJrk"
#define DATABASE_URL    "https://project-wireless-3e13c-default-rtdb.asia-southeast1.firebasedatabase.app"
#define USER_EMAIL      "esp32@test.com"
#define USER_PASS       "12345678"

// ================= SENSORS =================
#define DHTPIN           4
#define DHTTYPE          DHT11
#define SOIL_PIN         34

// ================= RELAY =================
// ตรวจสอบโมดูล Relay: ถ้า LOW แล้วไฟติด ให้สลับ ON เป็น LOW และ OFF เป็น HIGH
#define RELAY_PIN       18
#define RELAY_ON        HIGH   // ปิดวงจร = เปิดปั๊ม
#define RELAY_OFF       LOW    // เปิดวงจร = หยุดปั๊ม

// ================= TIME =================
const char* ntpServer          = "pool.ntp.org";
const long  gmtOffset_sec      = 7 * 3600;
const int   daylightOffset_sec = 0;

// ================= OBJECTS =================
DHT dht(DHTPIN, DHTTYPE);

void processData(AsyncResult &aResult);
UserAuth user_auth(Web_API_KEY, USER_EMAIL, USER_PASS);
FirebaseApp app;
WiFiClientSecure ssl_client;
using AsyncClient = AsyncClientClass;
AsyncClient aClient(ssl_client);
RealtimeDatabase Database;

// ================= TIMING =================
unsigned long lastSendTime       = 0;
const unsigned long sendInterval = 10000;

unsigned long lastPollTime       = 0;
const unsigned long pollInterval = 2000;

// ================= PUMP STATE =================
bool pumpRunning              = false;
unsigned long pumpStartTime   = 0;
const unsigned long pumpDuration = 1000; // 1 วินาที
String lastWateredTime        = "";

// ================= SCHEDULE STATE =================
String schedMode  = "";
String schedTime1 = "";
String schedTime2 = "";

// =====================================================
// SAVE WATERING LOG
// =====================================================
void saveWaterLog(String type) {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("⚠️ Time not ready");
    return;
  }

  char dateStr[11], timeStr[6];
  strftime(dateStr, 11, "%Y-%m-%d", &timeinfo);
  strftime(timeStr,  6, "%H:%M",    &timeinfo);

  String path = "/watering_logs/" + String(time(NULL));
  String json = "{\"date\":\"" + String(dateStr) +
                "\",\"time\":\"" + String(timeStr) +
                "\",\"type\":\"" + type +
                "\",\"timestamp\":" + String((int)time(NULL)) + "}";

  Database.set<object_t>(aClient, path, object_t(json), processData, "Log_Save");
  Serial.println("💾 Log saved: " + type);
}

// =====================================================
// PUMP CONTROL
// =====================================================
void startPump(String type) {
  if (pumpRunning) return;
  
  Serial.println("🌊 WATERING START (" + type + ")");
  digitalWrite(RELAY_PIN, RELAY_ON); 
  pumpRunning   = true;
  pumpStartTime = millis();

  // บันทึก Log และอัปเดตสถานะใน Firebase
  saveWaterLog(type);
  if (app.ready()) {
    Database.set<bool>(aClient, "/test/pump_status", true, processData, "Pump_Status_ON");
    // รีเซ็ตค่าปุ่มกดใน Firebase ทันทีเพื่อป้องกัน Loop ซ้อน
    if (type == "manual") {
        Database.set<int>(aClient, "/test/water", 0, processData, "Reset_Water_Trigger");
    }
  }
}

void stopPump() {
  digitalWrite(RELAY_PIN, RELAY_OFF);
  pumpRunning = false;
  Serial.println("✅ WATERING DONE (1 sec)");

  if (app.ready()) {
    Database.set<bool>(aClient, "/test/pump_status", false, processData, "Pump_Status_OFF");
  }
}

// =====================================================
// FETCH SCHEDULE
// =====================================================
void fetchSchedule() {
  Database.get(aClient, "/test/schedule/mode", [](AsyncResult &res) {
    if (res.available()) {
      schedMode = res.c_str();
      schedMode.replace("\"", "");
    }
  }, false, "Get_Mode");

  Database.get(aClient, "/test/schedule/time1", [](AsyncResult &res) {
    if (res.available()) {
      schedTime1 = res.c_str();
      schedTime1.replace("\"", "");
    }
  }, false, "Get_Time1");

  Database.get(aClient, "/test/schedule/time2", [](AsyncResult &res) {
    if (res.available()) {
      schedTime2 = res.c_str();
      schedTime2.replace("\"", "");
    }
  }, false, "Get_Time2");
}

// =====================================================
// AUTO SCHEDULE
// =====================================================
void checkAutoSchedule() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return;

  char currentTime[6];
  strftime(currentTime, 6, "%H:%M", &timeinfo);
  String nowTime = String(currentTime);

  if (schedMode == "1") {
    if (nowTime == schedTime1 && lastWateredTime != nowTime) {
      startPump("auto");
      lastWateredTime = nowTime;
    }
  } else if (schedMode == "2") {
    if ((nowTime == schedTime1 || nowTime == schedTime2) && lastWateredTime != nowTime) {
      startPump("auto");
      lastWateredTime = nowTime;
    }
  }
}

// =====================================================
// SEND SENSOR DATA
// =====================================================
void sendSensorData() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  int soil_raw         = analogRead(SOIL_PIN);
  int moisture_percent = map(soil_raw, 4095, 0, 0, 100);
  moisture_percent     = constrain(moisture_percent, 0, 100);

  Serial.println("--- Sensor Data ---");
  if (isnan(h) || isnan(t)) {
    Serial.println("❌ DHT11 Error");
  } else {
    Serial.printf("Temp: %.2f C | Hum: %.2f %%\n", t, h);
    Database.set<float>(aClient, "/test/temp", t, processData, "Send_Temp");
    Database.set<float>(aClient, "/test/hum",  h, processData, "Send_Hum");
  }
  
  Serial.printf("Soil Moisture: %d %%\n", moisture_percent);
  Database.set<int>(aClient, "/test/soil_moisture", moisture_percent, processData, "Send_Soil");
}

// =====================================================
// SETUP
// =====================================================
void setup() {
  Serial.begin(115200);

  pinMode(SOIL_PIN,  INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_OFF); // ปิดปั๊มทันทีเมื่อเริ่ม

  dht.begin();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\n✅ Wi-Fi Connected");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  ssl_client.setInsecure();
  initializeApp(aClient, app, getAuth(user_auth), processData, "authTask");
  app.getApp<RealtimeDatabase>(Database);
  Database.url(DATABASE_URL);

  Serial.println("🚀 System Ready!");
}

// =====================================================
// LOOP
// =====================================================
void loop() {
  app.loop();
  unsigned long now = millis();

  // [จุดแก้ไขสำคัญ] 1. ตรวจสอบการหยุดปั๊ม (ทำงานตลอดเวลา ไม่ต้องรอ Firebase Ready)
  if (pumpRunning) {
    if (now - pumpStartTime >= pumpDuration) {
      stopPump();
    }
  }

  if (app.ready()) {
    // 2. ส่งข้อมูลเซ็นเซอร์และดึงตารางเวลา
    if (now - lastSendTime >= sendInterval) {
      lastSendTime = now;
      sendSensorData();
      fetchSchedule();
    }

    // 3. ตรวจสอบปุ่มกดจาก Web App
    if (now - lastPollTime >= pollInterval) {
      lastPollTime = now;

      // เช็คปุ่มรดน้ำ
      Database.get(aClient, "/test/water", [](AsyncResult &res) {
        if (res.available()) {
          String payload = res.c_str();
          payload.replace("\"", ""); // ลบเครื่องหมาย " ออกถ้ามี
          if (payload == "1" && !pumpRunning) {
            startPump("manual");
          }
        }
      }, false, "Get_Water");

      // เช็คปุ่มหยุดฉุกเฉิน
      Database.get(aClient, "/test/stop", [](AsyncResult &res) {
        if (res.available()) {
          String payload = res.c_str();
          payload.replace("\"", "");
          if (payload == "1" && pumpRunning) {
            stopPump();
            Database.set<int>(aClient, "/test/stop", 0, processData, "Reset_Stop");
          }
        }
      }, false, "Get_Stop");
    }

    // 4. ตรวจสอบตารางเวลารดน้ำอัตโนมัติ
    checkAutoSchedule();
  }
  
  delay(10); // ปรับให้ Loop ทำงานเร็วขึ้นเพื่อความแม่นยำของเวลา
}

void processData(AsyncResult &aResult) {
  if (!aResult.isResult()) return;
  if (aResult.isError()) {
    if (aResult.error().code() != -118) {
      Serial.printf("❌ Error: %s\n", aResult.error().message().c_str());
    }
  }
}