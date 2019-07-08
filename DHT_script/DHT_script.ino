#include <dht11.h>
#include "Timer.h"

Timer t; 
dht11 DHT11;
dht11 DHT11_01;
dht11 DHT11_02;
dht11 DHT11_03;

#define PIN_DHT11 2
#define PIN_FanRelay 3
#define PIN_HumidifierRelay 4

#define PIN_DHT11_01 5
#define PIN_FanRelay_01 6
#define PIN_HumidifierRelay_01 7

#define PIN_DHT11_02 8
#define PIN_FanRelay_02 9
#define PIN_HumidifierRelay_02 10

#define PIN_DHT11_03 11
#define PIN_FanRelay_03 12
#define PIN_HumidifierRelay_03 13

 
void setup()
{
  Serial.begin(9600);
  pinMode(PIN_DHT11, OUTPUT);
  pinMode(PIN_FanRelay, OUTPUT);
  pinMode(PIN_HumidifierRelay, OUTPUT);
  
  pinMode(PIN_DHT11_01, OUTPUT);
  pinMode(PIN_FanRelay_01, OUTPUT);
  pinMode(PIN_HumidifierRelay_01, OUTPUT);

  pinMode(PIN_DHT11_02, OUTPUT);
  pinMode(PIN_FanRelay_02, OUTPUT);
  pinMode(PIN_HumidifierRelay_02, OUTPUT);

  pinMode(PIN_DHT11_03, OUTPUT);
  pinMode(PIN_FanRelay_03, OUTPUT);
  pinMode(PIN_HumidifierRelay_03, OUTPUT);

  
  t.every(500, Proc_Humidity,(void*)0L);
  t.every(500, Proc_Humidity_01,(void*)0L);
  t.every(500, Proc_Humidity_02,(void*)0L);
  t.every(500, Proc_Humidity_03,(void*)0L);


  //t.every(1000, Proc_Temperature);

  int tickEvent = t.every(10L*60L*1000L, Proc_Air, (void*)30000L);
}
 
void loop()
{
  t.update();
  delay(1000);
}



void Proc_Humidity(void* context)
{
  //读取传感器数据
  int chk = DHT11.read(PIN_DHT11);
  float humi = DHT11.humidity;
  float temp = DHT11.temperature;
  Send_data("I0H","I0T",humi,temp);

  Serial.print("DHT11 Humidity (%): ");
  Serial.println((float)DHT11.humidity,2);
  Serial.print("DHT11 Temperature (oC): ");
  Serial.println((float)DHT11.temperature,2);

  // body
    Serial.print("Proc_Humidity() is running !!!");
    Serial.flush();
    
    if(humi > 90)
    {
      Serial.println("Humidity is too high !!!");
      Serial.flush();
      digitalWrite(PIN_FanRelay,LOW);       // turn the Fan on 
      digitalWrite(PIN_HumidifierRelay, HIGH);  // turn the Humidifier off
    }
    if (humi >= 80 && humi <= 90)
    {
      Serial.println("Humidity is ok !!!");
      Serial.flush();
      digitalWrite(PIN_FanRelay,HIGH);          // turn the Fan off 
      if (humi >= 85)
      {
        Serial.println("Humidifier is off !!!");
        Serial.flush();
        digitalWrite(PIN_HumidifierRelay, HIGH);  // turn the Humidifier off
      }
    }
    if(humi< 80)
    {
      Serial.println("Humidity is too low !!!");
      Serial.flush();
      digitalWrite(PIN_FanRelay,HIGH);          // turn the Fan off 
      digitalWrite(PIN_HumidifierRelay, LOW);   // turn the Humidifier on
    }
}


void Proc_Humidity_01(void* context)
{
  int chk_01 = DHT11_01.read(PIN_DHT11_01);
  float humi_01 = DHT11_01.humidity;
  float temp_01 = DHT11_01.temperature;
  Send_data("I1H","I1T",humi_01,temp_01);
  Serial.print("DHT11_01 Humidity_01 (%): ");
  Serial.println((float)DHT11_01.humidity,2);
  Serial.print("DHT11_01 Temperature_01 (oC): ");
  Serial.println((float)DHT11_01.temperature,2);
  //body
    Serial.print("Proc_Humidity_01() is running !!!");
    Serial.flush();
    
    if(humi_01 > 90)
    {
      Serial.println("Humidity_01 is too high !!!");
      Serial.flush();
      digitalWrite(PIN_FanRelay_01,LOW);       // turn the Fan on 
      digitalWrite(PIN_HumidifierRelay_01, HIGH);  // turn the Humidifier off
    }
    if (humi_01 >= 80 && humi_01 <= 90)
    {
      Serial.println("Humidity_01 is ok !!!");
      Serial.flush();
      digitalWrite(PIN_FanRelay_01,HIGH);          // turn the Fan off 
  
      //digitalWrite(PIN_FanRelay_01,LOW);          // turn the Fan off 
      if (humi_01 >= 85)
      {
        Serial.println("Humidifier_01 is off !!!");
        Serial.flush();
        digitalWrite(PIN_HumidifierRelay_01, HIGH);  // turn the Humidifier off
      }
    }
    if(humi_01< 80)
    {
      Serial.println("Humidity_01 is too low !!!");
      Serial.flush();
      digitalWrite(PIN_FanRelay_01,HIGH);          // turn the Fan off 
      digitalWrite(PIN_HumidifierRelay_01, LOW);   // turn the Humidifier on
    }
}







void Proc_Humidity_02(void* context)
{
  int chk_02 = DHT11_02.read(PIN_DHT11_02);
  float humi_02 = DHT11_02.humidity;
  float temp_02 = DHT11_02.temperature;
  Send_data("I2H","I2T",humi_02,temp_02);
  Serial.print("DHT11_02 Humidity_02 (%): ");
  Serial.println((float)DHT11_02.humidity,2);
  Serial.print("DHT11_02 Temperature_02 (oC): ");
  Serial.println((float)DHT11_02.temperature,2);
  //body
    Serial.print("Proc_Humidity_02() is running !!!");
    Serial.flush();
    
    if(humi_02 > 90)
    {
      Serial.println("Humidity_02 is too high !!!");
      Serial.flush();
      digitalWrite(PIN_FanRelay_02,LOW);       // turn the Fan on 
      digitalWrite(PIN_HumidifierRelay_02, HIGH);  // turn the Humidifier off
    }
    if (humi_02 >= 80 && humi_02 <= 90)
    {
      Serial.println("Humidity_02 is ok !!!");
      Serial.flush();
      digitalWrite(PIN_FanRelay_02,HIGH);          // turn the Fan off 
  
      //digitalWrite(PIN_FanRelay_01,LOW);          // turn the Fan off 
      if (humi_02 >= 85)
      {
        Serial.println("Humidifier_02 is off !!!");
        Serial.flush();
        digitalWrite(PIN_HumidifierRelay_02, HIGH);  // turn the Humidifier off
      }
    }
    if(humi_02< 80)
    {
      Serial.println("Humidity_02 is too low !!!");
      Serial.flush();
      digitalWrite(PIN_FanRelay_02,HIGH);          // turn the Fan off 
      digitalWrite(PIN_HumidifierRelay_02, LOW);   // turn the Humidifier on
    }
}






void Proc_Humidity_03(void* context)
{
  int chk_03 = DHT11_03.read(PIN_DHT11_03);
  float humi_03 = DHT11_03.humidity;
  float temp_03 = DHT11_03.temperature;
  Send_data("I3H","I3T",humi_03,temp_03);
  Serial.print("DHT11_03 Humidity_03 (%): ");
  Serial.println((float)DHT11_03.humidity,2);
  Serial.print("DHT11_03 Temperature_03 (oC): ");
  Serial.println((float)DHT11_03.temperature,2);
  //body
    Serial.print("Proc_Humidity_03() is running !!!");
    Serial.flush();
    
    if(humi_03 > 90)
    {
      Serial.println("Humidity_03 is too high !!!");
      Serial.flush();
      digitalWrite(PIN_FanRelay_03,LOW);       // turn the Fan on 
      digitalWrite(PIN_HumidifierRelay_03, HIGH);  // turn the Humidifier off
    }
    if (humi_03 >= 80 && humi_03 <= 90)
    {
      Serial.println("Humidity_03 is ok !!!");
      Serial.flush();
      digitalWrite(PIN_FanRelay_03,HIGH);          // turn the Fan off 
  
      //digitalWrite(PIN_FanRelay_01,LOW);          // turn the Fan off 
      if (humi_03 >= 89)
      {
        Serial.println("Humidifier_03 is off !!!");
        Serial.flush();
        digitalWrite(PIN_HumidifierRelay_03, HIGH);  // turn the Humidifier off
      }
    }
    if(humi_03< 75)
    {
      Serial.println("Humidity_03 is too low !!!");
      Serial.flush();
      digitalWrite(PIN_FanRelay_03,HIGH);          // turn the Fan off 
      digitalWrite(PIN_HumidifierRelay_03, LOW);   // turn the Humidifier on
    }
}



void Send_data(String INH,String INT,float humi,float temp)
{
  Serial.print("{\"");
  Serial.print(INH);
  Serial.print("\":");
  Serial.print(humi);
  Serial.print(",\"");
  Serial.print(INT);
  Serial.print("\":");
  Serial.print(temp);
  Serial.println("}");
  Serial.flush();
}





//void Proc_Temperature()
//{
//  float temp = DHT11.temperature;
//  if(temp >28)
//  {
//    digitalWrite(PIN_FanRelay, HIGH);   // turn the Fan on 
//    digitalWrite(PIN_HeaterRelay,LOW);  // turn the Humidifier off
//  }
//  if(temp< 20)
//  {
//    digitalWrite(PIN_FanRelay, LOW);      // turn the Fan off 
//    digitalWrite(PIN_HeaterRelay,HIGH);   // turn the Humidifier on
//  }
//}


void Proc_Air(void* context)
{
    int duration = (long)context;
    Serial.print("Proc_Air() is running !!!");
    digitalWrite(PIN_FanRelay, LOW);   // turn the Fun on
    delay(duration);                   // wait for 5 Secs
    digitalWrite(PIN_FanRelay, HIGH);    // turn the Fun off 
        
    digitalWrite(PIN_FanRelay_01, LOW);   // turn the Fun on
    delay(duration);                   // wait for 5 Secs
    digitalWrite(PIN_FanRelay_01, HIGH);    // turn the Fun off 
    
    digitalWrite(PIN_FanRelay_02, LOW);   // turn the Fun on
    delay(duration);                   // wait for 5 Secs
    digitalWrite(PIN_FanRelay_02, HIGH);    // turn the Fun off 
    
    digitalWrite(PIN_FanRelay_03, LOW);   // turn the Fun on
    delay(duration);                   // wait for 5 Secs
    digitalWrite(PIN_FanRelay_03, HIGH);    // turn the Fun off 
}
