#include "esp_camera.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "board_config.h"

// ─────────────────────────────────────────────
// USER SETTINGS
// ─────────────────────────────────────────────
const char* ssid          = "POCO";
const char* password_wifi = "12345678";

// --- ImgBB ---
const char* imgbb_api_key = "2e104c5351ad0b50694738c3f61fef7e";

// --- Supabase ---
const char* supabase_url   = "https://jojvabjckmruvmrhrrzd.supabase.co";
const char* supabase_key   = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImpvanZhYmpja21ydXZtcmhycnpkIiwicm9sZSI6ImFub24iLCJpYXQiOjE3NzI0NDM1MjQsImV4cCI6MjA4ODAxOTUyNH0.8XHYvfy3qfzgcf95gmeCTxLEcaQZcmzMI_4qMiRD87M";
const char* supabase_table = "image";

// ─────────────────────────────────────────────
// Timing
// ─────────────────────────────────────────────
unsigned long lastCheck       = 0;
unsigned long lastCapture     = 0;
unsigned long lastControlPoll = 0;

const unsigned long checkInterval = 300000;  // 5 min
const unsigned long controlPollMs = 10000;   // 10 sec

// captureInterval อ่านจาก Supabase camera_control.interval_min — default 60 min
unsigned long captureInterval = 3600000UL;

// ─────────────────────────────────────────────
// Forward declarations
// ─────────────────────────────────────────────
void startCameraServer();
void setupLedFlash();
bool checkInternet();
String captureAndUploadImgBB();
bool insertSupabase(const String& imageUrl);
void pollCameraControl();
bool resetCaptureNow();

// ─────────────────────────────────────────────
// Internet check
// ─────────────────────────────────────────────
bool checkInternet() {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;
  http.begin(client, "https://www.google.com");
  int code = http.GET();
  http.end();
  return (code == 200);
}

// ─────────────────────────────────────────────
// Base64 encoder
// ─────────────────────────────────────────────
static const char b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

String base64Encode(const uint8_t* data, size_t len) {
  String out;
  out.reserve(((len + 2) / 3) * 4);
  for (size_t i = 0; i < len; i += 3) {
    uint8_t b0 = data[i];
    uint8_t b1 = (i + 1 < len) ? data[i + 1] : 0;
    uint8_t b2 = (i + 2 < len) ? data[i + 2] : 0;
    out += b64chars[(b0 >> 2) & 0x3F];
    out += b64chars[((b0 & 0x03) << 4) | ((b1 >> 4) & 0x0F)];
    out += (i + 1 < len) ? b64chars[((b1 & 0x0F) << 2) | ((b2 >> 6) & 0x03)] : '=';
    out += (i + 2 < len) ? b64chars[b2 & 0x3F] : '=';
  }
  return out;
}

// ─────────────────────────────────────────────
// Capture → ImgBB → return URL
// ─────────────────────────────────────────────
String captureAndUploadImgBB() {
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) { Serial.println("Camera capture failed"); return ""; }
  Serial.printf("Captured %u bytes\n", fb->len);

  String b64 = base64Encode(fb->buf, fb->len);
  esp_camera_fb_return(fb);

  b64.replace("+", "%2B");
  b64.replace("/", "%2F");
  b64.replace("=", "%3D");

  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;
  http.begin(client, "https://api.imgbb.com/1/upload");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.setTimeout(20000);

  Serial.printf("Free heap: %u\n", ESP.getFreeHeap());
  Serial.println("Uploading to ImgBB...");
  int code = http.POST("key=" + String(imgbb_api_key) + "&image=" + b64);
  String resp = http.getString();
  http.end();
  Serial.printf("ImgBB HTTP %d\n", code);

  if (code != 200) { Serial.println("ImgBB failed: " + resp); return ""; }

  int urlIdx = resp.indexOf("\"url\":");
  if (urlIdx == -1) { Serial.println("URL not found"); return ""; }
  int start = resp.indexOf('"', urlIdx + 6) + 1;
  int end   = resp.indexOf('"', start);
  String imageUrl = resp.substring(start, end);
  imageUrl.replace("\\/", "/");
  Serial.println("ImgBB URL: " + imageUrl);
  return imageUrl;
}

// ─────────────────────────────────────────────
// Insert into Supabase image table
// ─────────────────────────────────────────────
bool insertSupabase(const String& imageUrl) {
  if (imageUrl.isEmpty()) return false;

  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;

  http.begin(client, String(supabase_url) + "/rest/v1/" + supabase_table);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("apikey", supabase_key);
  http.addHeader("Authorization", "Bearer " + String(supabase_key));
  http.addHeader("Prefer", "return=minimal");

  int code = http.POST("{\"image_url\":\"" + imageUrl + "\"}");
  String resp = http.getString();
  http.end();
  Serial.printf("Supabase HTTP %d: %s\n", code, resp.c_str());
  return (code == 201);
}

// ─────────────────────────────────────────────
// Poll Supabase camera_control (ทุก 10 วิ)
//
// สร้างตารางใน Supabase SQL Editor (ทำครั้งเดียว):
//   create table camera_control (
//     id           int8 primary key default 1,
//     capture_now  bool default false,
//     interval_min int4 default 60
//   );
//   insert into camera_control (id) values (1);
// ─────────────────────────────────────────────
void pollCameraControl() {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;

  http.begin(client, String(supabase_url) + "/rest/v1/camera_control?id=eq.1&select=capture_now,interval_min");
  http.addHeader("apikey", supabase_key);
  http.addHeader("Authorization", "Bearer " + String(supabase_key));
  http.setTimeout(8000);

  int code = http.GET();
  if (code != 200) { Serial.printf("pollCameraControl HTTP %d\n", code); http.end(); return; }

  String resp = http.getString();
  http.end();

  // Parse interval_min
  int intIdx = resp.indexOf("\"interval_min\":");
  if (intIdx != -1) {
    int s = intIdx + 15;
    while (s < (int)resp.length() && !isDigit(resp[s])) s++;
    int e = s;
    while (e < (int)resp.length() && isDigit(resp[e])) e++;
    int mins = resp.substring(s, e).toInt();
    if (mins > 0) {
      unsigned long newInterval = (unsigned long)mins * 60000UL;
      if (newInterval != captureInterval) {
        captureInterval = newInterval;
        Serial.printf("captureInterval updated: %d min\n", mins);
      }
    }
  }

  // Parse capture_now
  int cnIdx = resp.indexOf("\"capture_now\":");
  if (cnIdx != -1) {
    int vs = cnIdx + 14;
    while (vs < (int)resp.length() && resp[vs] == ' ') vs++;
    if (resp.substring(vs, vs + 4) == "true") {
      Serial.println(">>> capture_now triggered from webapp!");
      String imgUrl = captureAndUploadImgBB();
      insertSupabase(imgUrl);
      lastCapture = millis();
      resetCaptureNow();
    }
  }
}

// ─────────────────────────────────────────────
// Reset capture_now = false
// ─────────────────────────────────────────────
bool resetCaptureNow() {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;

  http.begin(client, String(supabase_url) + "/rest/v1/camera_control?id=eq.1");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("apikey", supabase_key);
  http.addHeader("Authorization", "Bearer " + String(supabase_key));
  http.addHeader("Prefer", "return=minimal");
  http.setTimeout(8000);

  int code = http.sendRequest("PATCH", "{\"capture_now\":false}");
  http.end();
  Serial.printf("resetCaptureNow HTTP %d\n", code);
  return (code == 204 || code == 200);
}

// ─────────────────────────────────────────────
// Setup
// ─────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer   = LEDC_TIMER_0;
  config.pin_d0       = Y2_GPIO_NUM;
  config.pin_d1       = Y3_GPIO_NUM;
  config.pin_d2       = Y4_GPIO_NUM;
  config.pin_d3       = Y5_GPIO_NUM;
  config.pin_d4       = Y6_GPIO_NUM;
  config.pin_d5       = Y7_GPIO_NUM;
  config.pin_d6       = Y8_GPIO_NUM;
  config.pin_d7       = Y9_GPIO_NUM;
  config.pin_xclk     = XCLK_GPIO_NUM;
  config.pin_pclk     = PCLK_GPIO_NUM;
  config.pin_vsync    = VSYNC_GPIO_NUM;
  config.pin_href     = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn     = PWDN_GPIO_NUM;
  config.pin_reset    = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size   = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;
  config.grab_mode    = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location  = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count     = 1;

  if (config.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      config.jpeg_quality = 10;
      config.fb_count     = 2;
      config.grab_mode    = CAMERA_GRAB_LATEST;
    } else {
      config.frame_size  = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed: 0x%x\n", err);
    return;
  }

  sensor_t* s = esp_camera_sensor_get();
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);
    s->set_brightness(s, 1);
    s->set_saturation(s, -2);
  }
  if (config.pixel_format == PIXFORMAT_JPEG)
    s->set_framesize(s, FRAMESIZE_QVGA);

#if defined(LED_GPIO_NUM)
  setupLedFlash();
#endif

  WiFi.begin(ssid, password_wifi);
  WiFi.setSleep(false);
  Serial.print("WiFi connecting");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500); Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected: " + WiFi.localIP().toString());
    pollCameraControl();           // โหลด interval_min จาก Supabase
    String url = captureAndUploadImgBB();
    insertSupabase(url);
    lastCapture     = millis();
    lastControlPoll = millis();
  }

  startCameraServer();
  Serial.print("Camera Ready! http://");
  Serial.println(WiFi.localIP());
}

// ─────────────────────────────────────────────
// Loop
// ─────────────────────────────────────────────
void loop() {
  unsigned long now = millis();

  // 1. WiFi/internet check (ทุก 5 นาที)
  if (now - lastCheck > checkInterval) {
    if (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, password_wifi);
    } else if (!checkInternet()) {
      Serial.println("Internet lost. Retrying...");
    }
    lastCheck = now;
  }

  // 2. Poll camera_control (ทุก 10 วิ) — รับ capture_now + interval_min
  if (now - lastControlPoll > controlPollMs) {
    if (WiFi.status() == WL_CONNECTED) pollCameraControl();
    lastControlPoll = now;
  }

  // 3. Auto capture ตาม interval
  if (now - lastCapture > captureInterval) {
    if (WiFi.status() == WL_CONNECTED) {
      String url = captureAndUploadImgBB();
      insertSupabase(url);
    }
    lastCapture = now;
  }
}
