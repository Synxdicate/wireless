#include "esp_camera.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "board_config.h"

// --- USER SETTINGS ---
const char* ssid          = "POCO";
const char* password_wifi = "12345678";
//const char* kku_user      = "6733802674";
//const char* kku_pass      = "natchapol167349";

// --- ImgBB ---
const char* imgbb_api_key = "2e104c5351ad0b50694738c3f61fef7e";   // <-- paste your ImgBB key

// --- Supabase ---
const char* supabase_url    = "https://jojvabjckmruvmrhrrzd.supabase.co";  // <-- your project URL
const char* supabase_key    = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImpvanZhYmpja21ydXZtcmhycnpkIiwicm9sZSI6ImFub24iLCJpYXQiOjE3NzI0NDM1MjQsImV4cCI6MjA4ODAxOTUyNH0.8XHYvfy3qfzgcf95gmeCTxLEcaQZcmzMI_4qMiRD87M";            // <-- your anon key
const char* supabase_table  = "image";                            // <-- your table name

unsigned long lastCheck     = 0;
unsigned long lastCapture   = 0;
const unsigned long checkInterval   = 300000;  // 5 min
const unsigned long captureInterval = 60000;   // capture every 1 min (adjust as needed)

// ─────────────────────────────────────────────
// Forward declarations
// ─────────────────────────────────────────────
void startCameraServer();
void setupLedFlash();
bool checkInternet();
//void loginKKU();
String captureAndUploadImgBB();
bool insertSupabase(const String& imageUrl);

// ─────────────────────────────────────────────
// KKU Login helpers
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

// void loginKKU() {
//   if (WiFi.status() != WL_CONNECTED) return;
//   WiFiClientSecure client;
//   client.setInsecure();
//   HTTPClient http;
//   Serial.println("Step 1: Fetching NAC...");
//   http.begin(client, "https://login.kku.ac.th/");
//   http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
//   int httpCode = http.GET();
//   if (httpCode > 0) {
//     String payload = http.getString();
//     int index = payload.indexOf("nac");
//     if (index != -1) {
//       String nac = "";
//       for (int i = index; i < index + 5; i++) {
//         if (isAlphaNumeric(payload[i])) nac += payload[i];
//         else break;
//       }
//       String loginUrl = "https://" + nac + ".kku.ac.th/login";
//       String postData = "username=" + String(kku_user) + "&password=" + String(kku_pass) + "&dst=&popup=true";
//       http.begin(client, loginUrl);
//       http.addHeader("Content-Type", "application/x-www-form-urlencoded");
//       int postCode = http.POST(postData);
//       if (postCode == 200 || postCode == 302) Serial.println(">>> KKU LOGIN SUCCESSFUL! <<<");
//     }
//   }
//   http.end();
// }

// ─────────────────────────────────────────────
// Capture photo → base64 → upload to ImgBB
// Returns: image URL string, or "" on failure
// ─────────────────────────────────────────────
// Lightweight base64 encoder
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

String captureAndUploadImgBB() {
  // 1. Capture frame
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return "";
  }
  Serial.printf("Captured %u bytes\n", fb->len);

  // 2. Base64-encode
  String b64 = base64Encode(fb->buf, fb->len);
  esp_camera_fb_return(fb);

  // 3. Upload to ImgBB
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;
  http.begin(client, "https://api.imgbb.com/1/upload");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  // URL-encode the base64 string (replace + with %2B)
  // ImgBB accepts raw base64 in the 'image' field

  // After generating b64, replace + and /
  b64.replace("+", "%2B");
  b64.replace("/", "%2F");
  b64.replace("=", "%3D");

  String postBody = "key=" + String(imgbb_api_key) + "&image=" + b64;

  http.setTimeout(20000);
  Serial.printf("Free heap: %u\n", ESP.getFreeHeap());
  Serial.printf("b64 length: %u\n", b64.length());

  Serial.println("Uploading to ImgBB...");
  int code = http.POST(postBody);
  String resp = http.getString();
  http.end();
  Serial.printf("ImgBB HTTP %d\n", code);

  if (code != 200) {
    Serial.println("ImgBB upload failed: " + resp);
    return "";
  }

  // 4. Parse URL from JSON (simple string search, no JSON library needed)
  // ImgBB returns: "url":"https:\/\/i.ibb.co\/..."
  int urlIdx = resp.indexOf("\"url\":");
  if (urlIdx == -1) {
    Serial.println("URL not found in ImgBB response");
    return "";
  }
  int start = resp.indexOf('"', urlIdx + 6) + 1;
  int end   = resp.indexOf('"', start);
  String imageUrl = resp.substring(start, end);
  imageUrl.replace("\\/", "/");  // unescape
  Serial.println("ImgBB URL: " + imageUrl);
  return imageUrl;
}

// ─────────────────────────────────────────────
// Insert row into Supabase table
// Table columns expected: id (auto), image_url (text), created_at (default now())
// ─────────────────────────────────────────────
bool insertSupabase(const String& imageUrl) {
  if (imageUrl.isEmpty()) return false;

  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;

  String url = String(supabase_url) + "/rest/v1/" + supabase_table;
  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("apikey", supabase_key);
  http.addHeader("Authorization", "Bearer " + String(supabase_key));
  http.addHeader("Prefer", "return=minimal");

  String body = "{\"image_url\":\"" + imageUrl + "\"}";
  Serial.println("Inserting into Supabase: " + body);
  int code = http.POST(body);
  String resp = http.getString();
  http.end();

  Serial.printf("Supabase HTTP %d: %s\n", code, resp.c_str());
  return (code == 201);
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
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
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
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected: " + WiFi.localIP().toString());
    //loginKKU();
    // First capture immediately on boot
    String url = captureAndUploadImgBB();
    insertSupabase(url);
    lastCapture = millis();
  }

  startCameraServer();
}

// ─────────────────────────────────────────────
// Loop
// ─────────────────────────────────────────────
void loop() {
  unsigned long now = millis();

  // Periodic internet/login check
  if (now - lastCheck > checkInterval) {
    if (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, password_wifi);
    } else if (!checkInternet()) {
      Serial.println("Internet lost. Re-logging in...");
      //loginKKU();
    }
    lastCheck = now;
  }

  // Periodic photo capture
  if (now - lastCapture > captureInterval) {
    if (WiFi.status() == WL_CONNECTED) {
      String url = captureAndUploadImgBB();
      insertSupabase(url);
    }
    lastCapture = now;
  }
}