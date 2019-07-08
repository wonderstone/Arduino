#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
//#include <ESP8266HTTPClient.h>
//the include line below is very important
#include "./src/ESP8266HTTPClient.h"
#include <WiFiClient.h>

ESP8266WiFiMulti WiFiMulti;

void setup() {

  Serial.begin(9600);
  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("D-Link_DUDU", "1112100108050929");

}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");
    // configure traged server and url

    //http.begin(client, "http://wonderstone.vicp.io/admin");
    http.begin(client, "192.168.0.5", 80, "/admin/set/I0932HTI/20181");
    //http.begin(client, "jigsaw.w3.org", 80, "/HTTP/connection.html");
    http.setAuthorization("www", "www");
    /*
      // or
      http.begin(client, "http://jigsaw.w3.org/HTTP/Basic/");
      http.setAuthorization("guest", "guest");
      // or
      http.begin(client, "http://jigsaw.w3.org/HTTP/Basic/");
      http.setAuthorization("Z3Vlc3Q6Z3Vlc3Q=");
    */


    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(10000);
}
