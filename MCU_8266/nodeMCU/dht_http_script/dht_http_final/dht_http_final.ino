#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "./src/ESP8266HTTPClient.h"
#include <WiFiClient.h>
#include <dht11.h>
#include "Timer.h" //https://github.com/SandyWalsh/Timer/commit/1c79c8089ffa4febb03abb78609b74bd85391424
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




#define PIN_DHT11 2
// Host needs to be replaced by "wonderstone.vicp.io" when deployed in productive env
//#define HOST "192.168.0.5" //cannot use 127.0.0.1 or localhost
#define HOST "wonderstone.vicp.io" 
#define PORT 80
#define USR "www"
#define PSW "www"

void setup() {
  Serial.begin(9600);
  Serial.println();
  pinMode(PIN_DHT11, OUTPUT);  

  DHT01.dht =  DHT11x;
  // this is the ID part and should be changed everytime! 
  DHT01.ID = "Y03";
  DHT01.PIN = PIN_DHT11;
 
  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("TP-LINK_488E", "61493360");
  
  Serial.println("Connecting Wifi...");
  while(true){
    if(WiFiMulti.run() == WL_CONNECTED){
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      break;
    }else{
      Serial.println("WiFi not connected!");
      delay(2000);
    }
  }

  configTime(timezone * 3600, 0, "pool.ntp.org", "time.nist.gov","0.cn.pool.ntp.org");
  Serial.println("\nWaiting for time");
  //Serial.println(Get_Time());
  while (time(nullptr)<= 100000){
    Serial.print(".");
//    Serial.println(time(nullptr));
    delay(1500); }
  t.every(2500, Proc_Humidity,(void*)&DHT01);
}

void loop()
{
  t.update();
  //Serial.println(Get_Time());
  //Serial.println(time(nullptr));
}

// https://stackoverflow.com/questions/11626786/what-does-void-mean-and-how-to-use-it
// A pointer to void is a "generic" pointer type. A void * can be converted to any other 
// pointer type without an explicit cast. You cannot dereference a void * or do pointer 
// arithmetic with it; you must convert it to a pointer to a complete data type first.
void Proc_Humidity(void* context)
{
  struct tm baseline = { 0 };//初始化结构体  
  baseline.tm_year = 2018 - 1900;//年份设置为2016年  
  baseline.tm_mon = 10;//月份设置为11月  
  baseline.tm_mday = 12;//记得tm_mday要设置  否则0就是前一天  
  // read sensor
  // https://www.arduino.cc/reference/en/language/structure/pointer-access-operators/reference/
  // the pointer theDHT point to a  struct DHTs
  struct DHTs * theDHT = (struct DHTs *)context;
  // You use . when you're dealing with variables. You use -> when you are dealing with pointers.
  // https://stackoverflow.com/questions/5998599/difference-between-and-in-a-struct
  int chk = theDHT->dht.read(theDHT->PIN);
  float res_humi = theDHT->dht.humidity;
  float res_temp = theDHT->dht.temperature;
  double dt = difftime(time(nullptr), mktime(&baseline));

  

  if (isnan(res_humi) || isnan(res_temp)) {
    // not a number case or time error
    Serial.println("not a num error: ");
    Serial.print(res_humi);
    Serial.print(res_temp);
//    Serial.println(dt);
  }else{
    // a number and time ok
    String humi = String(round(res_humi));
    String temp = String(round(res_temp));
    String timenow = Get_Time();
    // http SET proc
    String URI = give_URI("/admin/set",theDHT->ID+"H",humi);
    http_set(HOST, PORT, URI, USR,PSW);
    URI = give_URI("/admin/set",theDHT->ID+"T",temp);
    http_set(HOST, PORT, URI, USR,PSW);
    URI = give_URI("/admin/set",theDHT->ID+"Time",timenow);
    http_set(HOST, PORT, URI, USR,PSW);
    URI = give_URI("/admin/set","time",timenow);
    http_set(HOST, PORT, URI, USR,PSW);
  }
 
}
//
String Get_Time()
{ time_t now = time(nullptr);
  struct tm * timeinfo;
  timeinfo = localtime(&now);  
//  String res = String(timeinfo->tm_year+1900)+"-"+String(timeinfo->tm_mon+1)+"-"+
//                String(timeinfo->tm_mday)+"%20"+String(timeinfo->tm_hour)+":"+
//                String(timeinfo->tm_min)+":"+String(timeinfo->tm_sec);
  String res = String(timeinfo->tm_year+1900)+"-"+process_time(timeinfo->tm_mon+1)+"-"+
                process_time(timeinfo->tm_mday)+"%20"+process_time(timeinfo->tm_hour)+":"+
                process_time(timeinfo->tm_min)+":"+process_time(timeinfo->tm_sec);
//  Serial.println(tmpres);
  
  return res; }
// solve from 2019-1-1 to 2019-01-01 problem
String process_time(int num){
  if(num <10){
    return "0"+String(num);
  }else{
    return String(num);
  }
}

String give_URI(String base,String key,String value)
{
  return base +"/" + key + "/"+value;
}
//
void http_set(String host, int port, String URI, String usr,String psw)
{
  if ((WiFiMulti.run() == WL_CONNECTED)) {
  WiFiClient client;
  HTTPClient http;
  Serial.print("[HTTP] begin...\n");
  // configure traged server and url

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
