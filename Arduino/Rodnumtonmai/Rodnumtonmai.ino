#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <time.h>

// ================= WIFI =================
#define WIFI_SSID "กูเอง"
#define WIFI_PASSWORD "12345678"

// ================= FIREBASE =================
#define API_KEY "AIzaSyC4CEMA9XeWvv92l1nwl2IP9hCn99ffJrk"
#define DATABASE_URL "https://project-wireless-3e13c-default-rtdb.asia-southeast1.firebasedatabase.app/"

// ================= RELAY =================
#define RELAY_PIN 18   // Active LOW

// ================= TIME =================
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 7 * 3600;
const int daylightOffset_sec = 0;

// ================= FIREBASE OBJECT =================
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// ================= PUMP STATE =================
bool pumpRunning = false;
unsigned long pumpStartTime = 0;
const unsigned long pumpDuration = 5000;
String lastWateredTime = "";

// =====================================================
// WIFI AUTO RECONNECT (ไม่บล็อก)
// =====================================================
void connectWiFi() {

  if (WiFi.status() == WL_CONNECTED) return;

  Serial.println("Reconnecting WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long startAttempt = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 8000) {
    delay(300);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED)
    Serial.println("\nWiFi Connected");
  else
    Serial.println("\nWiFi Failed");
}

// =====================================================
// SAVE LOG (มี retry)
// =====================================================
void saveWaterLog(String type) {

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Time not ready");
    return;
  }

  char dateStr[11];
  char timeStr[6];

  strftime(dateStr, 11, "%Y-%m-%d", &timeinfo);
  strftime(timeStr, 6, "%H:%M", &timeinfo);

  FirebaseJson json;
  json.set("timestamp", time(NULL));
  json.set("date", String(dateStr));
  json.set("time", String(timeStr));
  json.set("type", type);

  for (int i = 0; i < 3; i++) {
    if (Firebase.RTDB.pushJSON(&fbdo, "watering_logs", &json)) {
      Serial.println("💾 Log saved");
      return;
    }
    delay(500);
  }

  Serial.println("❌ Log failed");
}

// =====================================================
// START PUMP (กันเน็ตหลุด)
// =====================================================
void startPump(String type) {

  if (pumpRunning) return;

  Serial.println("🌊 WATERING START");

  // 🔥 บันทึกก่อนเปิดปั๊ม
  saveWaterLog(type);

  digitalWrite(RELAY_PIN, LOW);
  pumpRunning = true;
  pumpStartTime = millis();
}

// =====================================================
// STOP PUMP
// =====================================================
void stopPump() {

  digitalWrite(RELAY_PIN, HIGH);
  pumpRunning = false;

  Serial.println("✅ WATERING DONE");
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

  if (!Firebase.RTDB.getString(&fbdo, "test/schedule/mode")) return;

  String mode = fbdo.stringData();

  if (mode == "1") {

    Firebase.RTDB.getString(&fbdo, "test/schedule/time1");
    String t1 = fbdo.stringData();

    if (nowTime == t1 && lastWateredTime != nowTime) {
      startPump("auto");
      lastWateredTime = nowTime;
    }

  } else if (mode == "2") {

    Firebase.RTDB.getString(&fbdo, "test/schedule/time1");
    String t1 = fbdo.stringData();

    Firebase.RTDB.getString(&fbdo, "test/schedule/time2");
    String t2 = fbdo.stringData();

    if ((nowTime == t1 || nowTime == t2) && lastWateredTime != nowTime) {
      startPump("auto");
      lastWateredTime = nowTime;
    }
  }
}

// =====================================================
// SETUP
// =====================================================
void setup() {

  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

  connectWiFi();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase Auth OK");
  }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.println("System Ready 🚀");
}

// =====================================================
// LOOP
// =====================================================
void loop() {

  connectWiFi();

  // ===== Manual Start =====
  if (Firebase.RTDB.getInt(&fbdo, "test/water")) {
    if (fbdo.intData() == 1) {
      startPump("manual");
      Firebase.RTDB.setInt(&fbdo, "test/water", 0);
    }
  }

  // ===== Manual Stop =====
  if (Firebase.RTDB.getInt(&fbdo, "test/stop")) {
    if (fbdo.intData() == 1 && pumpRunning) {
      stopPump();
      Firebase.RTDB.setInt(&fbdo, "test/stop", 0);
    }
  }

  // ===== Auto Stop by Timer =====
  if (pumpRunning && millis() - pumpStartTime >= pumpDuration) {
    stopPump();
  }

  // ===== Auto Schedule =====
  checkAutoSchedule();

  delay(1000);
}