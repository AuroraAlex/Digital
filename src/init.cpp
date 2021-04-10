#include<init.h>


/**
 * 功能描述：使用wifimanager库配网并且写入eeprom uuid
 */
void init_wifi(){
  Serial.println("connected...yeey :)");
  wifiManager.setDebugOutput(false);
  WiFiManagerParameter custom_uuid("uuid", "配置uuid号", uuid, 40);
    // 设置进入AP模式的回调
  wifiManager.setAPCallback(configModeCallback);
  // 设置点击保存的回调
  wifiManager.addParameter(&custom_uuid);
  wifiManager.autoConnect("HOMEIOT");
  strcpy(uuid, custom_uuid.getValue());
  Serial.println(uuid);
  uuid_str = uuid;
  writeUUID();
  uuid_status_str = "$PERSISTENCE/"+uuid_str;
  Serial.println(uuid_status_str);
}

/**
 * 功能描述：配置进入AP模式通知回调
 */
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
 
}

/**
 * 功能描述：初始化GPIO
 */
void init_GPIO(){
  //板载按键
   pinMode(5,INPUT_PULLUP);
   pinMode(4,OUTPUT);
   digitalWrite(4,0);
   SW_status = 0;
}

/**
 * 功能描述：初始化MQTT,并且从eeprom读取uuid
 */
void init_mqtt(){
  client.setCallback(callback);
  readUUID();
  for (int i = 0 ; i < 10; i++) {
    client.connect(uuid, "mqtt", "mqtt");
    Serial.print("=>");
    delay(1000);
    if (client.connected()) {
      //连接成功
      Serial.println("\n|Connect to mqtt success!");
      break;
    }
  }

}

/**
 * 功能描述：MQTT 回调函数
 */

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String msg;
  for (int i = 0; i < length; i++) {
    // Serial.print((char)payload[i]);
    msg+=(char)payload[i];
  }
  Serial.println(msg);
  //判断json格式是否正确
  JSONVar dataJson = JSON.parse(msg);
  if (JSON.typeof(dataJson) == "undefined") {
    Serial.println("Parsing input failed!");
    return;
  } else{
      

    }

    

}
/**
 * 功能描述：掉线重连
 */

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(uuid,"mqtt", "mqtt")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // client.publish("outTopic", "hello world");
      // ... and resubscribe
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/**
 * 功能描述：检测是否初始化配置wifi 如GPIO5位低电平则重新配网
 */

void checkWifiStatus(){
  int i = digitalRead(2);
  Serial.println(i);
  if(digitalRead(5)==0){
    Serial.println(digitalRead(5));
    delay(10);
    if(digitalRead(5)==0){
      wifiManager.resetSettings();
      init_wifi();
    }
  }
}

/**
 * 功能描述：从eeprom读取uuid
 */

void readUUID(){
  EEPROM.begin(sizeof(uuid));
  EEPROM.get(0, uuid);
  uuid_str = uuid;
  uuid_status_str = "$PERSISTENCE/"+uuid_str;
  Serial.println(uuid_str);
  Serial.println(uuid_status_str);
}
/**
 * 功能描述：eeprom写入uuid
 */
void writeUUID(){
  EEPROM.begin(sizeof(uuid));
  EEPROM.put(0, uuid);
  EEPROM.commitReset();
}

/**
 * 功能描述：Ticker 回调函数
 */
void callback1() //回调函数1
{
  Serial.println("  回调函数1已启用");
  client.publish(uuid_status_str.c_str(),"hello world");
}
/**
 * 功能描述：初始化ticker
 */
void init_ticker(){
  ticker1.attach(60, callback1); 
}
