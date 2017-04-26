#include <ESP8266WiFi.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

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

// define testing parameter
#define CPUFREQ 80

#define mainProcessPin 2       // D6 pin

RF24 radio(4, 16);
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };

byte counter = 1;


void incrementState(){
  digitalWrite(mainProcessPin, LOW);
  delayMicroseconds(10);
  digitalWrite(mainProcessPin, HIGH);
}

void setup() {
  // disable wifi and set to modem sleep mode
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();

  // set cpu frequency
  // system_update_cpu_freq(CPUFREQ);
  pinMode(mainProcessPin, OUTPUT);

  // Serial.begin(115200);
  radio.begin();
  radio.setAutoAck(1);                    // Ensure autoACK is enabled
  radio.enableAckPayload();               // Allow optional ack payloads
  radio.setRetries(5, 15);                 // Smallest time between retries, max no. of retries
  radio.setPayloadSize(16);                // Here we are sending 1-byte payloads to test the call-response speed
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  // radio.printDetails();
}

void loop(){
  incrementState();
  // delay(100);
  radio.powerUp();
  radio.stopListening();                                  // First, stop listening so we can talk.
  // for (int i = 0; i< 100; i++){
  // printf("Now sending %d as payload. ",counter);
  byte gotByte;
  unsigned long time = micros();                          // Take the time, and send it.  This will block until complete
                                                          //Called when STANDBY-I mode is engaged (User is finished sending)
  if (!radio.write( &counter, 1 )){
    // Serial.println(F("failed."));
  }else{
    // incrementState();
    if(!radio.available()){
      // Serial.println(F("Blank Payload Received."));
    }else{
      while(radio.available() ){
        unsigned long tim = micros();
        radio.read( &gotByte, 1 );
        // printf("Got response %d, round-trip delay: %lu microseconds\n\r",gotByte,tim-time);
        counter++;
      }
      // delay(10);
      // incrementState();
    }
  // }
}
    delay(100);

  // Try again later
  radio.powerDown();
  incrementState();
  delay(1000);
}
