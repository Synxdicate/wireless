/*********
  ESP32 DHT11 (Pin 4) & Soil Moisture (Pin 34) to Firebase
*********/
#define ENABLE_USER_AUTH
#define ENABLE_DATABASE

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <FirebaseClient.h>
#include "DHT.h"

// --- ตั้งค่า Network และ Firebase ---
#define WIFI_SSID "POCO"
#define WIFI_PASSWORD "12345678"
#define Web_API_KEY "AIzaSyC4CEMA9XeWvv92l1nwl2IP9hCn99ffJrk"
#define DATABASE_URL "https://project-wireless-3e13c-default-rtdb.asia-southeast1.firebasedatabase.app"
#define USER_EMAIL "esp32@test.com"
#define USER_PASS "12345678"

// --- ตั้งค่า DHT11 ---
#define DHTPIN 4       // ขา Data DHT11 ต่อ Pin 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// --- ตั้งค่า Soil Moisture Sensor ---
const int soil_pin = 32; // ใช้ขา D34 (ADC1) ตามที่คุณระบุ

// --- ตัวแปร Firebase ---
void processData(AsyncResult &aResult);
UserAuth user_auth(Web_API_KEY, USER_EMAIL, USER_PASS);
FirebaseApp app;
WiFiClientSecure ssl_client;
using AsyncClient = AsyncClientClass;
AsyncClient aClient(ssl_client);
RealtimeDatabase Database;

// --- ตัวแปรควบคุมเวลา ---
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 10000; // ส่งข้อมูลทุก 10 วินาที

void setup() {
  Serial.begin(115200);
  dht.begin(); 

  // กำหนดโหมดขา Pin 34 เป็น Input
  pinMode(soil_pin, INPUT);

  // เชื่อมต่อ Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\n✅ Wi-Fi Connected!");

  ssl_client.setInsecure();

  // เริ่มต้น Firebase
  initializeApp(aClient, app, getAuth(user_auth), processData, "🔐 authTask");
  app.getApp<RealtimeDatabase>(Database);
  Database.url(DATABASE_URL);
}

void loop() {
  app.loop(); // รักษาการเชื่อมต่อ Firebase

  if (app.ready()) {
    unsigned long currentTime = millis();
    if (currentTime - lastSendTime >= sendInterval) {
      lastSendTime = currentTime;

      // 1. อ่านค่า DHT11 (อุณหภูมิและความชื้นอากาศ)
      float h = dht.readHumidity();
      float t = dht.readTemperature();

      // 2. อ่านค่า Soil Moisture (ความชื้นในดิน)
      int sensor_raw = analogRead(soil_pin); 
      // สูตรคำนวณ %: 4095 คือแห้ง (0%), 0 คือเปียก (100%)
      int moisture_percent = map(sensor_raw, 4095, 0, 0, 100);
      
      // ป้องกันค่ากระโดดเกิน 0-100%
      if(moisture_percent > 100) moisture_percent = 100;
      if(moisture_percent < 0) moisture_percent = 0;

      // แสดงค่าใน Serial Monitor เพื่อตรวจสอบ
      Serial.println("--- Sensor Data ---");
      if (isnan(h) || isnan(t)) {
        Serial.println("❌ DHT11 Error: Check wiring at Pin 4");
      } else {
        Serial.printf("Temp: %.2f C | Hum: %.2f %%\n", t, h);
      }
      Serial.printf("Soil Raw: %d | Moisture: %d %%\n", sensor_raw, moisture_percent);
      Serial.println("-------------------");

      // --- ส่งข้อมูลขึ้น Firebase ---
      if (!isnan(h) && !isnan(t)) {
        Database.set<float>(aClient, "/test/temp", t, processData, "RTDB_Send_Temp");
        Database.set<float>(aClient, "/test/humidity", h, processData, "RTDB_Send_Hum");
      }
      // ส่งค่าความชื้นในดิน (แม้ DHT จะเสีย แต่ Soil ก็ยังส่งได้)
      Database.set<int>(aClient, "/test/soil_moisture", moisture_percent, processData, "RTDB_Send_Soil");
    }
  }
}

// ฟังก์ชันดักจับ Error และสถานะจาก Firebase
void processData(AsyncResult &aResult) {
  if (!aResult.isResult()) return;
  if (aResult.isError()) {
    Firebase.printf("❌ Error: %s, code: %d\n", aResult.error().message().c_str(), aResult.error().code());
  }
  if (aResult.available()) {
    Firebase.printf("✅ Success: %s, payload: %s\n", aResult.uid().c_str(), aResult.c_str());
  }
}