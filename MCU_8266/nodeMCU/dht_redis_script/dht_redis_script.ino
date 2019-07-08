#include <dht11.h>
#include "Timer.h"
#include <Redis.h>
#include <time.h>

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
#define WIFI_SSID       "D-Link_DUDU"
#define WIFI_PASSWORD   "1112100108050929"

#define REDIS_ADDR      "192.168.0.5"
#define REDIS_PORT      6379
#define REDIS_PASSWORD  ""

Redis redis(REDIS_ADDR, REDIS_PORT);

void setup()
{
  Serial.begin(9600);
  pinMode(PIN_DHT11, OUTPUT);  
  pinMode(PIN_Relay01, OUTPUT);  

  DHT01.dht =  DHT11x;
  DHT01.ID = "I01";
  DHT01.PIN = PIN_DHT11;

  Relay01.PIN = PIN_Relay01;
  Relay01.State = "OFF";
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to the WiFi");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(250);
    Serial.print(".");
  }
    Serial.println();
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    if (redis.begin(REDIS_PASSWORD)) 
    { Serial.println("Connected to the Redis server!"); } 
    else 
    { Serial.println("Failed to connect to the Redis server!");
      return; }
  configTime(timezone * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for time");
  while (!time(nullptr)) 
  { Serial.print(".");
    delay(1000); }
  t.every(1000, Proc_Humidity,(void*)&DHT01);
//  redis.close();
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
  // redis SET proc
  if (redis.set((theDHT->ID+"H").c_str(), humi.c_str()))
    { Serial.println(theDHT->ID+"H"+" ok!"); }
    else
    { Serial.println("err!"); }
  if (redis.set((theDHT->ID+"T").c_str(), temp.c_str()))
    { Serial.println(theDHT->ID+"T" + " ok!"); }
    else
    { Serial.println("err!"); }
  if (redis.set((theDHT->ID+"Time").c_str(), timenow.c_str()))
    { Serial.println(theDHT->ID+"Time"+" ok!"); }
    else
    { Serial.println("err!"); }  
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
                String(timeinfo->tm_mday)+" "+String(timeinfo->tm_hour)+":"+
                String(timeinfo->tm_min)+":"+String(timeinfo->tm_sec);
  return res; }

//set redis
