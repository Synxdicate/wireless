#include "WiFi.h"

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("กำลังสแกน WiFi...");

  int n = WiFi.scanNetworks(); 
  if (n == 0) {
    Serial.println("ไม่พบเครือข่าย");
  } else {
    Serial.printf("พบทั้งหมด %d เครือข่าย:\n", n);
    for (int i = 0; i < n; ++i) {
      Serial.printf("%d: %s (RSSI: %d) %s\n", 
        i + 1,
        WiFi.SSID(i).c_str(), 
        WiFi.RSSI(i),
        (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "เปิด" : "มีรหัส"
      );
    }
  }
}

void loop() {
}
