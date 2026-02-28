#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

// --- USER SETTINGS ---
const char* ssid = "kku-wifi";         // University WiFi SSID
const char* password_wifi = "";          // Usually empty for KKU-Google
const char* kku_user = "6733802674";          // Your Student ID (No dashes)
const char* kku_pass = "natchapol167349";         // Your Password

// Timing for re-checking connection (every 5 minutes)
unsigned long lastCheck = 0;
const unsigned long checkInterval = 300000; 

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Initial Connection
  connectToWiFi();
  loginKKU();
}

void loop() {
  // Check connection status periodically
  if (millis() - lastCheck > checkInterval) {
    if (WiFi.status() != WL_CONNECTED) {
      connectToWiFi();
    }
    
    // Simple ping test to see if we actually have internet
    if (!checkInternet()) {
      Serial.println("Internet lost. Re-logging in...");
      loginKKU();
    }
    lastCheck = millis();
  }
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password_wifi);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi Connection Failed.");
  }
}

bool checkInternet() {
  HTTPClient http;
  WiFiClientSecure client;
  client.setInsecure(); // Skip SSL check for speed
  
  // Try to reach a reliable site (Google)
  http.begin(client, "https://www.google.com");
  int httpCode = http.GET();
  http.end();
  
  return (httpCode == 200);
}

void loginKKU() {
  if (WiFi.status() != WL_CONNECTED) return;

  WiFiClientSecure client;
  client.setInsecure(); // Required for KKU's HTTPS login page
  HTTPClient http;

  Serial.println("Step 1: Fetching NAC from login.kku.ac.th...");
  
  // 1. Get the NAC ID
  http.begin(client, "https://login.kku.ac.th/");
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  
  int httpCode = http.GET();
  if (httpCode > 0) {
    String payload = http.getString();
    
    // Find "nac" in the HTML
    int index = payload.indexOf("nac");
    if (index != -1) {
      String nac = "";
      for (int i = index; i < index + 5; i++) {
        if (isAlphaNumeric(payload[i])) {
          nac += payload[i];
        } else break;
      }
      Serial.println("Found NAC ID: " + nac);

      // 2. Perform the POST Login
      String loginUrl = "https://" + nac + ".kku.ac.th/login";
      String postData = "username=" + String(kku_user) + "&password=" + String(kku_pass) + "&dst=&popup=true";

      Serial.println("Step 2: Sending POST to " + loginUrl);
      http.begin(client, loginUrl);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      
      int postCode = http.POST(postData);
      
      if (postCode > 0) {
        Serial.printf("Response Code: %d\n", postCode);
        if (postCode == 200 || postCode == 302) {
          Serial.println(">>> LOGIN SUCCESSFUL! <<<");
        } else {
          Serial.println("Login failed. Check username/password.");
        }
      } else {
        Serial.printf("POST failed, error: %s\n", http.errorToString(postCode).c_str());
      }
    } else {
      Serial.println("Could not find 'nac' ID. You might already be logged in.");
    }
  } else {
    Serial.printf("Initial GET failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
}