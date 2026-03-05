#include "esp_camera.h"
#include <WiFi.h>
#include <HTTPClient.h>         // เพิ่มบรรทัดนี้
#include <WiFiClientSecure.h>   // เพิ่มบรรทัดนี้
// ===========================
// Select camera model in board_config.h
// ===========================
#include "board_config.h"

// ===========================
// Enter your WiFi credentials
// ===========================
// --- USER SETTINGS ---
const char* ssid = "kku-wifi";         // University WiFi SSID
const char* password_wifi = "";          // Usually empty for KKU-Google
const char* kku_user = "6733802674";          // Your Student ID (No dashes)
const char* kku_pass = "natchapol167349";         // Your Password

unsigned long lastCheck = 0;
const unsigned long checkInterval = 300000; // 5 นาที

void startCameraServer();
void setupLedFlash();

bool checkInternet() {
  HTTPClient http;
  WiFiClientSecure client;
  client.setInsecure(); 
  http.begin(client, "https://www.google.com");
  int httpCode = http.GET();
  http.end();
  return (httpCode == 200);
}

void loginKKU() {
  if (WiFi.status() != WL_CONNECTED) return;
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;
  Serial.println("Step 1: Fetching NAC...");
  http.begin(client, "https://login.kku.ac.th/");
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  
  int httpCode = http.GET();
  if (httpCode > 0) {
    String payload = http.getString();
    int index = payload.indexOf("nac");
    if (index != -1) {
      String nac = "";
      for (int i = index; i < index + 5; i++) {
        if (isAlphaNumeric(payload[i])) nac += payload[i];
        else break;
      }
      String loginUrl = "https://" + nac + ".kku.ac.th/login";
      String postData = "username=" + String(kku_user) + "&password=" + String(kku_pass) + "&dst=&popup=true";
      http.begin(client, loginUrl);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      int postCode = http.POST(postData);
      if (postCode == 200 || postCode == 302) Serial.println(">>> LOGIN SUCCESSFUL! <<<");
    }
  }
  http.end();
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;  // for streaming
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if (config.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    // Best option for face detection/recognition
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t *s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);        // flip it back
    s->set_brightness(s, 1);   // up the brightness just a bit
    s->set_saturation(s, -2);  // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  if (config.pixel_format == PIXFORMAT_JPEG) {
    s->set_framesize(s, FRAMESIZE_QVGA);
  }

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

#if defined(CAMERA_MODEL_ESP32S3_EYE)
  s->set_vflip(s, 1);
#endif

// Setup LED FLash if LED pin is defined in camera_pins.h
#if defined(LED_GPIO_NUM)
  setupLedFlash();
#endif
WiFi.begin(ssid, password_wifi); // เปลี่ยนเป็น password_wifi ตามตัวแปรใหม่
  WiFi.setSleep(false);

  Serial.print("WiFi connecting");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected");
    loginKKU(); // <--- เรียก Login ทันทีที่เชื่อม WiFi ติด
  }

  startCameraServer();
  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");
}

void loop() {
  // ตรวจสอบสถานะเน็ตทุกๆ 5 นาที (checkInterval)
  if (millis() - lastCheck > checkInterval) {
    if (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, password_wifi);
    } else {
      // ถ้า WiFi ติด แต่เน็ตเข้าไม่ได้ (ติดหน้า Login) ให้ Login ใหม่
      if (!checkInternet()) {
        Serial.println("Internet lost. Re-logging in...");
        loginKKU();
      }
    }
    lastCheck = millis();
  }

}
