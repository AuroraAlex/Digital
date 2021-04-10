#ifndef init_h
#define init_h

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <Ticker.h>
#include <ESP_EEPROM.h>
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <PubSubClient.h>
#include <Arduino_JSON.h>


char uuid[40];
String uuid_str;
String uuid_status_str;
WiFiClient espClient;
PubSubClient client("192.168.1.131", 1883, espClient);
WiFiManager wifiManager;
Ticker ticker1; //声明Ticker对象

unsigned int SW_status = 0;

void configModeCallback (WiFiManager *myWiFiManager);
void init_GPIO();
void callback(char* topic, byte* payload, unsigned int length);
void init_mqtt();
void writeUUID();
void readUUID();
void init_ticker();
void callback1();
void checkWifiStatus();
void reconnect();

#endif