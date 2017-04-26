
#include <ESP8266WiFi.h>
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

// define network variable for setting static IP
IPAddress ip(192,168,1,129);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

#define startProcessPin 14      // D5
#define mainProcessPin 12       // D6 pin

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

// 2 case of testing
// 1 use default ESP8266WiFi.h that call wifi_station_dhcpc_start() before disable it
// 2 make own function don't call wifi_station_dhcpc_start()
// case 1 use 238ms and case 2 use 78.8
void loop() {
  startLoop();
  incrementState();
  // case 2
  connectAP(SSID, PASS);
  // case 1
  WiFi.begin(SSID,PASS);
  // incrementState();
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
    return;
  incrementState();
  WiFi.config(ip, gateway, subnet);
  // incrementState();
  // Serial.print("[loop] IP address: ");
  // Serial.println(WiFi.localIP());
  // incrementState();
  delay(1000);
  incrementState();
  WiFi.disconnect();
  incrementState();
  // Serial.print("Disconnect Wifi Status : ");
  // Serial.println(WiFi.status());
  delay(3000);
}
