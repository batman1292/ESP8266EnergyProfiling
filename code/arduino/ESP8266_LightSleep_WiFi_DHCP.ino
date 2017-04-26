// check connect AP
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
extern "C" {
#include "gpio.h"
}

extern "C" {
#include "user_interface.h"
}
// define wifi ssid and password
const char* SSID = "ESL_Lab1";
const char* PASS = "wifi@esl";

// const char* SSID = "khanchuea";
// const char *SSID =	"XMEN";
// const char* PASS = "";

#define wakeUpPin 0
#define CPUFREQ 160
#define power 20
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

void	fpm_wakup_cb_func1(void)
{
  incrementState();
  // WiFi.forceSleepWake();
  // wifi_fpm_do_wakeup();
  //   wifi_fpm_close();
  //    wifi_station_connect();
  WiFi.setOutputPower(power);
  // incrementState();
  WiFi.begin(SSID, PASS);
  // incrementState();
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
    return;
//	connect	to	AP
}


void setup() {
  system_update_cpu_freq(CPUFREQ);

  //  Serial.begin(921600);
   delay(10);
   pinMode(startProcessPin, OUTPUT);
   pinMode(mainProcessPin, OUTPUT);
  //  digital
   // We start by connecting to a WiFi network
 //   incrementState();
 //   wifi_set_opmode(NULL_MODE);
 // //	set	WiFi	mode	to	null	mode.
 // 	wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);
 // //	light	sleep
 // 	wifi_fpm_open();
 // //	enable	force	sleep
 // 	wifi_fpm_set_wakeup_cb(fpm_wakup_cb_func1);	//	Set	wakeup	callback
 // 	wifi_fpm_do_sleep(500*1000);
}

void loop() {
  // startLoop();
  incrementState();
  // WiFi.forceSleepWake();
  // wifi_fpm_do_wakeup();
  //   wifi_fpm_close();
  //    wifi_station_connect();
  WiFi.setOutputPower(power);
  // incrementState();
  WiFi.begin(SSID, PASS);
  // incrementState();
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
    return;
//	connect	to	AP
  // {
  //   incrementState();
  delay(500);
  // }else{
  //   incrementState();
  //   // Serial.print("[loop] IP address: ");
  //   // Serial.println(WiFi.localIP());
  //   delay(1000);
  // }
  incrementState();
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  // wifi_station_disconnect();
	// wifi_set_opmode(NULL_MODE);
//	set	WiFi	mode	to	null	mode.
	wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);
//	light	sleep
	wifi_fpm_open();
//	enable	force	sleep
	// wifi_fpm_set_wakeup_cb(fpm_wakup_cb_func1);	//	Set	wakeup	callback
	// wifi_fpm_do_sleep(500*1000);

  // incrementState();
  // wifi_fpm_set_sleep_type(MODEM_SLEEP_T);
  // wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);
  // WiFi.setSleepMode(WIFI_LIGHT_SLEEP);
  // wifi_fpm_open();
  gpio_pin_wakeup_enable(GPIO_ID_PIN(0), GPIO_PIN_INTR_LOLEVEL);
  wifi_fpm_do_sleep(0xFFFFFFF);
  // incrementState();
  // delay(2000);
  // WiFi.forceSleepWake();
}
