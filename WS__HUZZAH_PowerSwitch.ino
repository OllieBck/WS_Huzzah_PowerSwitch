/*
   WebSocketClient.ino

    Created on: 24.05.2015

  https://github.com/godstale/ESP8266_Arduino_IDE_Example/blob/master/example/WebSocketClient/WebSocketClient.ino

*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h>
#include <Hash.h>
#include "config.h"

//char* alphabet[26] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};
//int abcPlace = 0;

ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;

int incomingByte;

//#define USE_SERIAL Serial1

void setup() {
  Serial.begin(115200);
  pinMode(4, OUTPUT);
  //Serial.setDebugOutput(true);
  Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFiMulti.addAP(ssid, pass);

  //WiFi.disconnect();
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }

  webSocket.begin("192.168.43.1", 8787);
  //webSocket.setAuthorization("user", "Password"); // HTTP Basic Authorization
  webSocket.onEvent(webSocketEvent);

}

void loop() {
  webSocket.loop();
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t lenght) {

  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED:
      {
        Serial.print("[WSc] Connected to url: %s\n");
        Serial.println((char*)payload);

        // send message to server when Connected
        webSocket.sendTXT("Retaliator Connected");
      }
      break;
    case WStype_TEXT:
      Serial.print("received: ");
      Serial.println((char*)payload);
      if (strcmp((char*)payload, "e") == 0) {
        digitalWrite(4, HIGH);
      }
      if (strcmp((char*)payload, "t") == 0) {
        digitalWrite(4, LOW);
      }
      break;
    case WStype_BIN:
      Serial.printf("[WSc] get binary lenght: %u\n", lenght);
      //hexdump(payload, lenght);

      // send data to server
      // webSocket.sendBIN(payload, lenght);
      break;
  }

}
