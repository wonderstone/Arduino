#include <dht11.h>
#include "Timer.h"


Timer t; 
dht11 DHT11;

#define PIN_DHT11 2


 
void setup()
{
  Serial.begin(9600);
  Serial.flush(); 
  pinMode(PIN_DHT11, OUTPUT);

  t.every(5000, Proc_Humidity,(void*)0L);
}
 
void loop()
{
  t.update();
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
  Serial.flush();
  
}
