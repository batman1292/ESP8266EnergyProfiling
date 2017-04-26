
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
#include <PubSubClient.h>
// define wifi ssid and password
const char* SSID = "ESL_Lab1";
const char* PASS = "wifi@esl";

// define network variable for setting static IP
IPAddress ip(192,168,1,129);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

// Update these with values suitable for your network.
IPAddress server(192, 168, 1, 9);

void callback(const MQTT::Publish& pub) {
  // handle message arrived
}

WiFiClient wclient;
PubSubClient client(wclient, server);

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
   connectAP(SSID, PASS);
   if (WiFi.waitForConnectResult() != WL_CONNECTED)
     return;
   WiFi.config(ip, gateway, subnet);
   Serial.print("[loop] IP address: ");
   Serial.println(WiFi.localIP());
   client.set_callback(callback);
   if (!client.connected()) {
      if (client.connect("arduinoClient")) {
	      client.set_callback(callback);
      }
    }

    if (client.connected())
      client.loop();
}

void loop() {
  if (client.connected()){
    client.publish("outTopic","hello world");
  }
  delay(1000);
}
