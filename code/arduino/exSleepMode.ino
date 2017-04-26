#include <ESP8266WiFi.h>
extern "C" {
  #include "user_interface.h"
}

const char* ssid = "XMEN";
const char* password = "";

void setup() {
  Serial.begin(115200);
  Serial.println();
}

void loop() {

  initWifi();

  Serial.println("Light sleep:");
  wifi_set_sleep_type(LIGHT_SLEEP_T);
  doDelays();

  Serial.println("None sleep:");
  wifi_set_sleep_type(NONE_SLEEP_T);
  doDelays();

  WiFi.disconnect();
  Serial.print("WiFi disconnected, IP address: "); Serial.println(WiFi.localIP());
  Serial.println("Light sleep:");
  wifi_set_sleep_type(LIGHT_SLEEP_T);
  doDelays();

}

void doDelays() {
  Serial.println("Yield for 2 sec");
  long endMs = millis() + 2000;
  while (millis() < endMs) {
     yield();
  }

  Serial.println("Delay for 2 sec");
  delay(2000);
}

void initWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while ((WiFi.status() != WL_CONNECTED)) {
     delay(500);
     Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected, IP address: "); Serial.println(WiFi.localIP());
}
