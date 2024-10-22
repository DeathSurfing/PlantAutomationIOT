#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "config.h"

// Replace with your WiFi credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// IFTTT Webhook URL
String iftttKey = "your_ifttt_webhook_key"; 
String eventName = "soil_moisture_alert";

// Soil moisture sensor pin
int sensorPin = A0; 

// Threshold for dry soil
int threshold = 300; 

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  // Connecting to WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void loop() {
  int sensorValue = analogRead(sensorPin); 
  Serial.println(sensorValue);

  // If the soil is dry (value is below the threshold)
  if(sensorValue < threshold) {
    triggerIFTTT(sensorValue);
  }

  delay(60000); // Wait for a minute before checking again
}

void triggerIFTTT(int value) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://maker.ifttt.com/trigger/" + eventName + "/with/key/" + iftttKey + "?value1=" + String(value);
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode > 0) {
      Serial.println("IFTTT triggered successfully");
    } else {
      Serial.println("Error triggering IFTTT");
    }
    http.end();
  }
}
