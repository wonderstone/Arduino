#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
//#include <ESP8266HTTPClient.h>
//the include line below is very important
#include "./src/ESP8266HTTPClient.h"
#include <WiFiClient.h>
#include <time.h>

#define INPUT_SIZE 75


int timezone = 8;
int dst = 0;

ESP8266WiFiMulti WiFiMulti;

void setup() {


  Serial.begin(9600);
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("D-Link_DUDU", "1112100108050929");
  WiFiMulti.addAP("HiWiFi", "821112wtq");
  while (WiFiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(1000);
    Serial.print('.');
  }
  configTime(timezone * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for time");
  while (!time(nullptr)) 
  { Serial.print(".");
    delay(1000); }
  
}




void loop() {
  double co2_1,co2_2,co2_3,temp_1,temp_2,temp_3,humi_1,humi_2,humi_3;
  char input[INPUT_SIZE+1]; 
  byte size = Serial.readBytes(input, INPUT_SIZE);
  // Add the final 0 to end the C string
  input[INPUT_SIZE+1] = 0;

  char* token = strtok(input, " ");
  Serial.println(token);
  if (strcmp(token, "FF") == 0){
    token = strtok(NULL, " ");
    if (strcmp(token, "EE") == 0){
      //Serial.println("Cool man ");
      token = strtok(NULL, " ");
  
      co2_1= co2_cal(token);
      Serial.println(co2_1);
      co2_2= co2_cal(token);
      Serial.println(co2_2);
      co2_3= co2_cal(token);
      Serial.println(co2_3);
  
      temp_1= temp_cal(token);
      Serial.println(temp_1);
      temp_2= temp_cal(token);
      Serial.println(temp_2);
      temp_3= temp_cal(token);
      Serial.println(temp_3);
  
      humi_1= humi_cal(token);
      Serial.println(humi_1);
      humi_2= humi_cal(token);
      Serial.println(humi_2);
      humi_3= humi_cal(token);
      Serial.println(humi_3);
    }
    
    

  }else{
    Serial.println("Fuck off ");
  }

  
  String timenow = Get_Time();
  // sth bought
//  String info;
//  while (Serial.available()>0)
//  {
//    info = Serial.read();
//    delay(1);
//  }  
  Serial.println(timenow);
  // wait for WiFi connection
  String tmp ;
  tmp = url("I1H",String(humi_1));
  httpFunc(tmp);
  tmp = url("I1T",String(temp_1));
  httpFunc(tmp);
  tmp = url("I1CO2",String(co2_1));
  httpFunc(tmp);
  tmp = url("time",timenow);
//  tmp = url("time",info);
  httpFunc(tmp);
  delay(5000);
}


void httpFunc(String tmp)
{
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");
    // configure traged server and url
    http.begin(client, tmp);
    http.setAuthorization("www", "www");



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


String url(String key,String value)
{
  String addr = "http://wonderstone.vicp.io/admin/set/";
  String tmp = addr + key + '/'+ value;
  return tmp;
}

String Get_Time()
{ time_t now = time(nullptr);
  struct tm * timeinfo;
  timeinfo = localtime(&now);  
  
  String res = String(timeinfo->tm_year+1900)+"-"+String(timeinfo->tm_mon+1)+"-"+
                String(timeinfo->tm_mday)+"%20"+String(timeinfo->tm_hour)+":"+
                String(timeinfo->tm_min)+":"+String(timeinfo->tm_sec);
  return res; }


  
double co2_cal(char* &token)
{
      long H = strtol(token, NULL, 16);
      token = strtok(NULL, " ");
      long L = strtol(token, NULL, 16);
      long res = H*256+L;
      token = strtok(NULL, " ");
      return res;
}

double temp_cal(char* &token)
{
      float res;
      if (token[0]=='1'){
        token[0] = 0;
        long H = strtol(token, NULL, 16);
        token = strtok(NULL, " ");
        long L = strtol(token, NULL, 16);
        res = -(float)(H*256+L)/10.0;
      }else{
        long H = strtol(token, NULL, 16);
        token = strtok(NULL, " ");
        long L = strtol(token, NULL, 16);
        res = (float)(H*256+L)/10.0;
      }
      token = strtok(NULL, " ");
      return res; 
}

double humi_cal(char* &token)
{
      long H = strtol(token, NULL, 16);
      token = strtok(NULL, " ");
      long L = strtol(token, NULL, 16);
      float res = (float)(H*256+L)/10.0;
      token = strtok(NULL, " ");
      return res;
}
