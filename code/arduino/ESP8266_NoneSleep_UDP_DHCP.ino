#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

extern "C" {
#include "c_types.h"
#include "ets_sys.h"
#include "os_type.h"
#include "osapi.h"
#include "mem.h"
#include "user_interface.h"
#include "smartconfig.h"
#include "lwip/opt.h"
#include "lwip/err.h"
#include "lwip/dns.h"
}
// define wifi ssid and password
const char* SSID = "ESL_Lab1";
const char* PASS = "wifi@esl";

// const char* SSID = "khanchuea"
// const char *SSID =	"DrayTek";
// const char* PASS = "1qaz2wsx";

#define CPUFREQ 160
#define startProcessPin 14      // D5
#define mainProcessPin 2       // D6 pin

WiFiUDP Udp;

void connectAP(const char* ssid, const char *passphrase){
  struct station_config conf;
  strcpy(reinterpret_cast<char*>(conf.ssid), ssid);

  if (passphrase) {
      strcpy(reinterpret_cast<char*>(conf.password), passphrase);
  } else {
      *conf.password = 0;
  }

  conf.bssid_set = 0;

  ETS_UART_INTR_DISABLE();
  wifi_station_set_config(&conf);
  wifi_station_connect();
  ETS_UART_INTR_ENABLE();
}

void startLoop(){
  digitalWrite(startProcessPin, LOW);
  delayMicroseconds(10);
  digitalWrite(startProcessPin, HIGH);
}

void incrementState(){
  digitalWrite(mainProcessPin, LOW);
  delayMicroseconds(10);
  digitalWrite(mainProcessPin, HIGH);
}

void setup() {
   system_update_cpu_freq(CPUFREQ);
   Serial.begin(921600);
   delay(10);
   pinMode(startProcessPin, OUTPUT);
   pinMode(mainProcessPin, OUTPUT);
  //  digital
   // We start by connecting to a WiFi network
   Serial.println();
   Serial.println();
   Serial.print("Connecting to ");
   Serial.println(SSID);
}

void loop() {
  startLoop();
  incrementState();
  WiFi.begin(SSID, PASS);
  incrementState();
  if (WiFi.waitForConnectResult() != WL_CONNECTED){
    incrementState();
    delay(500);
    // return;
  }else{
    Serial.println("conneted wifi");
    incrementState();
    delay(100);
    Udp.beginPacket("192.168.1.182", 8081);
    Udp.write("1234");
    Udp.flush();
    Udp.endPacket();
  }
  incrementState();
  delay(1000);
  incrementState();
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  incrementState();
  delay(3000);
}
