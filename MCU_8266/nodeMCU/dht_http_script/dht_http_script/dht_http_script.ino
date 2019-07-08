#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "./src/ESP8266HTTPClient.h"
#include <WiFiClient.h>
#include <dht11.h>
#include "Timer.h"
#include <time.h>


ESP8266WiFiMulti WiFiMulti;

Timer t; 
dht11 DHT11x;

int timezone = 8;
int dst = 0;

struct DHTs
{ 
  dht11 dht;
  int PIN;
  String ID;
  String T;
  String H;
  String Time;
}DHT01;
struct Relay
{ 
  int PIN;
  String State;
}Relay01;

#define PIN_DHT11 2
#define PIN_Relay01 0
#define HOST "http://wonderstone.vicp.io/"
#define PORT 80
#define USR "www"
#define PSW "www"


void setup() {

  Serial.begin(9600);
  Serial.println();
  Serial.println();
  Serial.println();

  pinMode(PIN_DHT11, OUTPUT);  
  pinMode(PIN_Relay01, OUTPUT);  

  DHT01.dht =  DHT11x;
  DHT01.ID = "Y02";
  DHT01.PIN = PIN_DHT11;

  Relay01.PIN = PIN_Relay01;
  Relay01.State = "OFF";

  

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("D-Link_DUDU", "1112100108050929");
  WiFiMulti.addAP("360WiFi", "1112100108050929");
  
  configTime(timezone * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for time");
  while (!time(nullptr)) 
  { Serial.print(".");
    delay(1000); }
  t.every(2000, Proc_Humidity,(void*)&DHT01);
}

void loop()
{
  t.update();
}

//get data
void Proc_Relay(void* context)
{
  struct Relay * theRelay = (struct Relay *)context;
  if (theRelay->State =="OFF"){digitalWrite(theRelay->PIN, LOW);}
  if (theRelay->State =="ON"){digitalWrite(theRelay->PIN, HIGH);}
}


void Proc_Humidity(void* context)
{
  // read sensor
  struct DHTs * theDHT = (struct DHTs *)context;
  int chk = theDHT->dht.read(theDHT->PIN);
  float res_humi = theDHT->dht.humidity;
  String humi = String(theDHT->dht.humidity);
  String temp = String(theDHT->dht.temperature);
  String timenow = Get_Time();
  // http SET proc


  String URI = give_URI("/admin/set","I01H",humi);
  http_set(HOST, PORT, URI, USR,PSW);
  URI = give_URI("/admin/set","I01T",temp);
  http_set(HOST, PORT, URI, USR,PSW);
  URI = give_URI("/admin/set","I01Time",timenow);
  http_set(HOST, PORT, URI, USR,PSW);
  URI = give_URI("/admin/set","time",timenow);
  http_set(HOST, PORT, URI, USR,PSW);
  
  // action on humi
  if (res_humi>50)
  { Serial.println("humi>50");
    Relay01.State = "ON"; }
  if (res_humi<=50)
  { Serial.println("humi<=50");
    Relay01.State = "OFF"; }
  Proc_Relay((void*)&Relay01);
  Serial.println(" .  ");
}
//
String Get_Time()
{ time_t now = time(nullptr);
  struct tm * timeinfo;
  timeinfo = localtime(&now);  
  
  String res = String(timeinfo->tm_year+1900)+"-"+String(timeinfo->tm_mon+1)+"-"+
                String(timeinfo->tm_mday)+"%20"+String(timeinfo->tm_hour)+":"+
                String(timeinfo->tm_min)+":"+String(timeinfo->tm_sec);
  return res; }

String give_URI(String base,String key,String value)
{
  return base +"/" + key + "/"+value;
}

 void http_set(String host, int port, String URI, String usr,String psw)
 {
   if ((WiFiMulti.run() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");
    // configure traged server and url

    //http.begin(client, "http://wonderstone.vicp.io/admin");
    http.begin(client, host, port, URI);
    http.setAuthorization(usr.c_str(), psw.c_str());

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
 }
