// static esp ip
// state check use pluse @ 20us
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

// define network variable for setting static IP
IPAddress ip(192,168,1,129);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

// set testing variable
#define CPUFREQ 80
#define power 0
#define mainProcessPin 2       // D6 pin
#define BUFSIZE 64
int sleepTime = 10000;            // setting time to sleep(ms)

// UDP variables
WiFiUDP Udp;
char dummyBuf [BUFSIZE];

void incrementState(){
  digitalWrite(mainProcessPin, LOW);
  delayMicroseconds(10);
  digitalWrite(mainProcessPin, HIGH);
}

void generateDummyData(){
  for(int i = 0; i < BUFSIZE; i++){
    dummyBuf[i] = 'x';
  }
}

void setup(){
  // Serial.begin(921600);
  system_update_cpu_freq(CPUFREQ);
  pinMode(mainProcessPin, OUTPUT);
  generateDummyData();
  // start process
  incrementState();
  // connect AP process
  WiFi.mode(WIFI_STA);
  WiFi.setOutputPower(power);
  WiFi.begin(SSID, PASS);
  if (WiFi.waitForConnectResult() != WL_CONNECTED){
    incrementState();
    delay(500);
    // return;
  }else{
    // Serial.println("conneted wifi");
    // WiFi.config(ip, gateway, subnet);  //uncomment for static ip
    incrementState();
    for(int i = 0; i<100; i++){
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
  delay(1);
  ESP.deepSleep(sleepTime*1000ul, WAKE_RF_DEFAULT);
}

void loop(){

}
