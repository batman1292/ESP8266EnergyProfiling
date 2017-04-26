#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>

#define SLEEP_PIN 13
#define PIR_PIN 15
#define mainProcessPin 2

extern "C" {
#include "gpio.h"
}
extern "C" {
#include "user_interface.h"
}

// WiFiClient wclient;
// PubSubClient mqttClient(wclient);

void incrementState(){
  digitalWrite(mainProcessPin, LOW);
  delayMicroseconds(10);
  digitalWrite(mainProcessPin, HIGH);
}

void setup() {
  Serial.begin(115200);
  gpio_init();
  // put your setup code here, to run once:
  // Serial.print("ESP chip ID: ");
  // Serial.println(ESP.getChipId(), DEC);
  // WiFi.mode(WIFI_STA);
  // // WiFi.config(getIpAddr("192.168.1.10"), getIpAddr("192.168.1.1"), getIpAddr("255.255.255.0"));
  // WiFi.begin("XMEN", "");
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println("");
  // if (WiFi.status() == WL_CONNECTED) {
  //   Serial.println("WiFi connected");
  //   // mqttClient.setServer("192.168.1.2", 1883);
  //   // mqttClient.connect("Test");
  //   Serial.println("MQTT connected");
  //   // mqttClient.publish("RHTest","here");
  // } else {
  //   Serial.println("WiFi not connected");
  // }
  // pinMode(SLEEP_PIN, INPUT_PULLUP);
  pinMode(PIR_PIN, INPUT_PULLUP);
  // pinMode(0, INPUT_PULLUP);
}

void loop() {
  incrementState();
  WiFi.mode(WIFI_STA);
  // WiFi.config(getIpAddr("192.168.1.10"), getIpAddr("192.168.1.1"), getIpAddr("255.255.255.0"));
  WiFi.begin("", "");
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
    return;
  Serial.println("WiFi connected");
  incrementState();
  delay(1000);
  incrementState();

  gpio_pin_wakeup_enable(GPIO_ID_PIN(PIR_PIN), GPIO_PIN_INTR_LOLEVEL);
  delay(100);
  wifi_station_disconnect();
  wifi_set_opmode(NULL_MODE);
  wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);
  wifi_fpm_open();
  // wifi_fpm_set_wakeup_cb(fpm_wakup_cb_func­)；
  wifi_fpm_do_sleep(3000000);
  Serial.print("S");
  //system_deep_sleep(30000000);
  delay(100);
}
