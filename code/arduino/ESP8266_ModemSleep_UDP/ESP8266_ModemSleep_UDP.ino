// check connect AP
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

// const char* SSID = "khanchuea";
// // const char *SSID =	"XMEN";
// const char* PASS = "1qaz2wsx";

// define network variable for setting static IP
IPAddress ip(192, 168, 1, 129);
IPAddress gateway(192, 168, 1, 255);
IPAddress subnet(255, 255, 255, 0);

#define CPUFREQ 80
#define power 20
#define mainProcessPin 2       // D6 pin
#define BUFSIZE 64

// UDP variables
WiFiUDP Udp;
char dummyBuf [BUFSIZE];

void connectAP(const char* ssid, const char *passphrase) {
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

void incrementState() {
  digitalWrite(mainProcessPin, LOW);
  delayMicroseconds(10);
  digitalWrite(mainProcessPin, HIGH);
}

void setup() {
  system_update_cpu_freq(CPUFREQ);
  pinMode(mainProcessPin, OUTPUT);
  //   WiFi.persistent(false);
  for (int i = 0; i < BUFSIZE; i++) {
    dummyBuf[i] = 'x';
  }
}

void loop() {
  incrementState();
  // delay(500);
  WiFi.forceSleepWake();
  WiFi.setOutputPower(power);
  WiFi.begin(SSID, PASS);
  // delay(500);
  // incrementState();
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    //    WiFi.config(ip, gateway, subnet);/
    incrementState();
    delay(500);
    // return;
  } else {
//    WiFi.config(ip, gateway, subnet);  // set static
    incrementState();
    for (int i = 0; i < 100; i++) {
      Udp.beginPacket("192.168.1.182", 8081);
      Udp.write(dummyBuf, BUFSIZE);
      Udp.flush();
      Udp.endPacket();
      delay(10);
    }
  }
  incrementState();
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  incrementState();
  delay(10000);
}
