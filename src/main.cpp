#include<init.h>


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  init_GPIO();
  checkWifiStatus();
  init_mqtt();
  init_ticker();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}