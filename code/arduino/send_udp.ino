#include <ESP8266WiFi.h>
#include <Ticker.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include <WiFiUDP.h>
#include "user_interface.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif
MPU6050 accelgyro(0x68);
#define OUTPUT_READABLE_ACCELGYRO
//int test_time = 13;
WiFiUDP UDP;
const char* ssidAP = "wifi_esp";
const char* ssid = "ESL_Lab1";
const char* password = "wifi@esl";
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t ax1, ay1, az1;
int16_t gx1, gy1, gz1;
int16_t ax2, ay2, az2;
int16_t gx2, gy2, gz2;
int16_t ax3, ay3, az3;
int16_t gx3, gy3, gz3;
int i = 0;
int k = 0;
int class_imu = 0;
int buton_state = 0;
static const uint8_t pin   = 2;
//static const uint8_t test_readonly   = 2;
static const uint8_t ad0_3   = 14;
static const uint8_t ad0_1   = 12;
static const uint8_t ad0_2   = 13;
Ticker flipper;
byte message[12];//38
void timer (void) {
  //Serial.println("start");
  digitalWrite(ad0_1, LOW);
    digitalWrite(ad0_2, HIGH);
  //  digitalWrite(ad0_3, HIGH);
  accelgyro.getMotion6(&ax1, &ay1, &az1, &gx1, &gy1, &gz1);
    digitalWrite(ad0_1, HIGH);
    digitalWrite(ad0_2, LOW);
  //  digitalWrite(ad0_3, HIGH);
    accelgyro.getMotion6(&ax2, &ay2, &az2, &gx2, &gy2, &gz2);
  //  digitalWrite(ad0_1, HIGH);
  //  digitalWrite(ad0_2, HIGH);
  //  digitalWrite(ad0_3, LOW);
  //  accelgyro.getMotion6(&ax3, &ay3, &az3, &gx3, &gy3, &gz3);
  k = k + 1;
  UDP.beginPacket("192.168.1.178", 8081);
  //UDP.beginPacket("192.168.4.2", 8081);
//  message[0] = (i >> 8);
//  message[1] = i;
  message[0] = (ax1 >> 8);
  message[1] = ax1;
  message[2] = (ay1 >> 8);
  message[3] = ay1;
  message[4] = (az1 >> 8);
  message[5] = az1;
  message[6] = (ax2 >> 8);
  message[7] = ax2;
  message[8] = (ay2 >> 8);
  message[9] = ay2;
  message[10] = (az2 >> 8);
  message[11] = az2;
//  message[8] = (gx1 >> 8);
//  message[9] = gx1;
//  message[10] = (gy1 >> 8);
//  message[11] = gy1;
//  message[12] = (gz1 >> 8);
//  message[13] = gz1;
//  message[14] = class_imu;
  //  message[14] = (ax2 >> 8);
  //  message[15] = ax2;
  //  message[16] = (ay2 >> 8);
  //  message[17] = ay2;
  //  message[18] = (az2 >> 8);
  //  message[19] = az2;
  //  message[20] = (gx2 >> 8);
  //  message[21] = gx2;
  //  message[22] = (gy2 >> 8);
  //  message[23] = gy2;
  //  message[24] = (gz2 >> 8);
  //  message[25] = gz2;
  //  message[26] = (ax3 >> 8);
  //  message[27] = ax3;
  //  message[28] = (ay3 >> 8);
  //  message[29] = ay3;
  //  message[30] = (az3 >> 8);
  //  message[31] = az3;
  //  message[32] = (gx3 >> 8);
  //  message[33] = gx3;
  //  message[34] = (gy3 >> 8);
  //  message[35] = gy3;
  //  message[36] = (gz3 >> 8);
  //  message[37] = gz3;
  UDP.write(message, sizeof(message));
  UDP.endPacket();
  i = (i + 1) % 60000;
}

void setup() {
  Serial.begin(115200);
  pinMode(ad0_1, OUTPUT);
  pinMode(ad0_2, OUTPUT);
  pinMode(ad0_3, OUTPUT);
  pinMode(pin, INPUT);
  digitalWrite(ad0_1, LOW);
    digitalWrite(ad0_2, HIGH);
  //  digitalWrite(ad0_3, HIGH);
  delay(100);
  Wire.begin();
  //WiFi_AP();
  WiFi_Config();
  accelgyro.initialize();
  digitalWrite(ad0_1, HIGH);
    digitalWrite(ad0_2, LOW);
  //  digitalWrite(ad0_3, HIGH);
  accelgyro.initialize();
  //  digitalWrite(ad0_1, HIGH);
  //  digitalWrite(ad0_2, HIGH);
  //  digitalWrite(ad0_3, LOW);
  //  accelgyro.initialize();
  //  flipper.attach(0.02, timer);//0.02

}

void loop() {
  if (button(pin) == 1) {
    flipper.detach();
    UDP.beginPacket("192.168.1.178", 8081);
    UDP.write("end");
    UDP.endPacket();
////    UDP.beginPacket("192.168.1.178", 8081);
////    UDP.write("start");
////    UDP.endPacket();
//    flipper.attach(0.02, timer);//0.02
  }
//  if (k == 100) {
//    flipper.detach();
//    UDP.beginPacket("192.168.1.178", 8081);
//    UDP.write("end");
////    UDP.write(class_imu);
//    UDP.endPacket();
//    k = 0;
//  }
}

void WiFi_AP() {
  WiFi.disconnect();
  WiFi.softAP(ssidAP);

  Serial.print("WiFi AP : ");
  Serial.print(ssidAP);
  Serial.println();
  Serial.println("WiFi AP Success");
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
}

void WiFi_Config() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
}

int button(int pin) {
  if (buton_state == 0) {
    if (digitalRead(pin) == LOW) {
      Serial.println("push");
      delay(50);//100
      if (digitalRead(pin) == LOW) {
        flipper.attach(0.02, timer);
        buton_state = 1;
      }
    }
  }
  if (buton_state == 1 and digitalRead(pin) == HIGH) {
    buton_state = 0;
    return 1;
  }
  return 0;
}
