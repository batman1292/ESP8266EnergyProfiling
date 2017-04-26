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

RF24 radio(4, 16);   


const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };  // Radio pipe addresses for the 2 nodes to communicate.

byte data[32];   

unsigned long counter, rxTimer; 
unsigned long startTime, stopTime;  

#define mainProcessPin 2       // D6 pin

void incrementState(){
  digitalWrite(mainProcessPin, LOW);
  delayMicroseconds(10);
  digitalWrite(mainProcessPin, HIGH);
}

void setup() {
  // put your setup code here, to run once:
//  WiFi.mode(WIFI_OFF);
//  WiFi.forceSleepBegin();

  // set cpu frequency
  // system_update_cpu_freq(CPUFREQ);
   Serial.begin(115200);
  pinMode(mainProcessPin, OUTPUT);

  radio.begin();                           // Setup and configure rf radio
  radio.setChannel(1);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_1MBPS);
  radio.setAutoAck(1);                     // Ensure autoACK is enabled
  radio.setRetries(2,15);                  // Optionally, increase the delay between retries & # of retries
  
  radio.setCRCLength(RF24_CRC_8);          // Use 8-bit CRC for performance
radio.openWritingPipe(pipes[1]);
      radio.openReadingPipe(1,pipes[0]);
      radio.stopListening();
   radio.printDetails(); 
//  radio.startListening();
//  radio.startListening();                 // Start listening

  randomSeed(analogRead(0));              //Seed for random number generation
  
  for(int i=0; i<32; i++){
     data[i] = random(255);               //Load the buffer with random data
  }
  radio.powerUp();      
}

void loop() {
  incrementState();
  delay(2000);
  incrementState();
//  radio.powerUp();  
//   delay(2000);
    
//    Serial.println(F("Initiating Basic Data Transfer"));
    
    
    unsigned long cycles = 50; //Change this to a higher or lower number. 
    
    startTime = millis();
    unsigned long pauseTime = millis();
            
    for(int i=0; i<cycles; i++){        //Loop through a number of cycles
      data[0] = i;                      //Change the first byte of the payload for identification
      if(!radio.writeFast(&data,32)){   //Write to the FIFO buffers        
        counter++;                      //Keep count of failed payloads
      }
      
      //This is only required when NO ACK ( enableAutoAck(0) ) payloads are used
//      if(millis() - pauseTime > 3){
//        pauseTime = millis();
//        radio.txStandBy();          // Need to drop out of TX mode every 4ms if sending a steady stream of multicast data
//        //delayMicroseconds(130);     // This gives the PLL time to sync back up   
//      }
      delay(20);
    }
    
   stopTime = millis();   
                                         //This should be called to wait for completion and put the radio in standby mode after transmission, returns 0 if data still in FIFO (timed out), 1 if success
   if(!radio.txStandBy()){ counter+=3; } //Standby, block only until FIFO empty or auto-retry timeout. Flush TX FIFO if failed
   radio.txStandBy(1000);              //Standby, using extended timeout period of 1 second
   
   float numBytes = cycles*32;
   float rate = numBytes / (stopTime - startTime);
    
   Serial.print("Transfer complete at "); Serial.print(rate); Serial.println(" KB/s");
   Serial.print("Use time "); Serial.print(stopTime - startTime); Serial.println(" ms");
   Serial.print(counter); Serial.print(" of "); Serial.print(cycles); Serial.println(" Packets Failed to Send");
   counter = 0;   
    
}
