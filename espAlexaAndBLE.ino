
#include "BluetoothSerial.h"
#include <Arduino.h>
#include <string.h>
#include <WiFi.h>
#include <fauxmoESP.h>

fauxmoESP fauxmo;


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


BluetoothSerial SerialBT;
const int LED_BUILTIN = 26;


void setup() {

    Serial.begin(115200);
  
    pinMode(0, INPUT_PULLUP);

    // initialize EEPROM with predefined size

 
    Serial.println(ESP.getSdkVersion());
     String deviceName = "light";

    Serial.println(deviceName);
    SerialBT.begin(deviceName);
    SerialBT.setTimeout(100);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH); 
  

      if (connectWifi()) {
    // Setup fauxmo
     Serial.println("Adding LED device");
     fauxmo.setPort(80);  
     fauxmo.enable(true);
     fauxmo.addDevice("Light"); 
     fauxmo.onSetState([](unsigned char device_id, const char * device_name, 
                    bool state, unsigned char value) {
    Serial.println(state);
    handleLight(state);
    // Here we handle the command received
});
  
}
}


boolean connectWifi() {
  // Let us connect to WiFi
  WiFi.begin("akku", "nosecurity");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi Connected....IP Address:");
  return true;
}


void handleLight(bool code) {
  Serial.println(code);
  
   if(code == 1){
    Serial.println("Came in on");
      digitalWrite(LED_BUILTIN, HIGH);
    }else{
      Serial.println("Came in off");
      digitalWrite(LED_BUILTIN, LOW);   
    }
  }
void loop() {

    String code = SerialBT.readString();
    code.trim();
   Serial.println("came in loop");
    if(code == "on"){
      digitalWrite(LED_BUILTIN, HIGH);
    }else if(code == "off"){
      digitalWrite(LED_BUILTIN, LOW);   
    }
     fauxmo.handle();
    yield();
}
