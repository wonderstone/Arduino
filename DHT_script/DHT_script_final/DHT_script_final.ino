#include <dht11.h>
#include "Timer.h"

Timer t; 
dht11 DHT11;

struct SwitchDuration
{ 
  long duration;
  int pin;
}SD00,SD01,SD02,SD03;

struct HumiProc
{ 
  int pin_DHT;
  int pin_Fan;
  int pin_Humi;
}HP00,HP01,HP02,HP03;

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

  
  SD00.pin = PIN_FanRelay;  SD00.duration = 15000L;
  HP00.pin_DHT=PIN_DHT11;  HP00.pin_Fan=PIN_FanRelay;  HP00.pin_Humi=PIN_HumidifierRelay;

  SD01.pin = PIN_FanRelay_01;  SD01.duration = 15000L;
  HP01.pin_DHT=PIN_DHT11_01;  HP01.pin_Fan=PIN_FanRelay_01;  HP01.pin_Humi=PIN_HumidifierRelay_01;

  SD02.pin = PIN_FanRelay_02;  SD02.duration = 15000L;
  HP02.pin_DHT=PIN_DHT11_02;  HP02.pin_Fan=PIN_FanRelay_02;  HP02.pin_Humi=PIN_HumidifierRelay_02;

  SD03.pin = PIN_FanRelay_03;  SD03.duration = 15000L;
  HP03.pin_DHT=PIN_DHT11_03;  HP03.pin_Fan=PIN_FanRelay_03;  HP03.pin_Humi=PIN_HumidifierRelay_03;
  
  
  t.every(1000, Proc_Humidity,(void*)&HP00);
  t.every(600000, RelaySwitch, (void*)&SD00);
  t.every(1000, Proc_Humidity_R,(void*)&HP01);
  t.every(600000, RelaySwitch_R, (void*)&SD01);
  t.every(1000, Proc_Humidity_R,(void*)&HP02);
  t.every(600000, RelaySwitch_R, (void*)&SD02);
  t.every(1000, Proc_Humidity,(void*)&HP03);
  t.every(600000, RelaySwitch, (void*)&SD03);
}

 
void loop()
{
  t.update();
  delay(1000);
}


void Proc_Humidity(void* context)
{
  //读取传感器数据
  Serial.println("Proc_Humidity is on ");
  struct HumiProc * theHP = (struct HumiProc *)context;
  int chk = DHT11.read(theHP->pin_DHT);
  float humi = DHT11.humidity;
  float temp = DHT11.temperature;
  Send_data("I0H","I0T",humi,temp);

  Serial.print("DHT11 Humidity (%): ");
  Serial.println(humi,2);
  Serial.print("DHT11 Temperature (oC): ");
  Serial.println(temp,2);

  // body

    
    if(humi > 90)
    {
      Serial.println("Humidity is too high");
      Serial.flush();
      RelayOn(theHP->pin_Fan);
      RelayOff(theHP->pin_Humi);
    }
    if (humi >= 80 && humi <= 90)
    {
      Serial.println("Humidity is ok");
      Serial.flush();
      RelayOff(theHP->pin_Fan);
      
      if (humi >= 85)
      {
        Serial.println("Humidifier is off");
        Serial.flush();
        RelayOff(theHP->pin_Humi);  // turn the Humidifier off
      }
    }
    if(humi< 80)
    {
      Serial.println("Humidity is too low");
      Serial.flush();
      RelayOff(theHP->pin_Fan);        // turn the Fan off 
      RelayOn(theHP->pin_Humi);  // turn the Humidifier on
    }
}




void Proc_Humidity_R(void* context)
{
  //读取传感器数据
  Serial.println("Proc_Humidity is on ");
  struct HumiProc * theHP = (struct HumiProc *)context;
  int chk = DHT11.read(theHP->pin_DHT);
  float humi = DHT11.humidity;
  float temp = DHT11.temperature;
  Send_data("I0H","I0T",humi,temp);

  Serial.print("DHT11 Humidity (%): ");
  Serial.println(humi,2);
  Serial.print("DHT11 Temperature (oC): ");
  Serial.println(temp,2);

  // body

    
    if(humi > 90)
    {
      Serial.println("Humidity is too high");
      Serial.flush();
      RelayOn_R(theHP->pin_Fan);
      RelayOff_R(theHP->pin_Humi);
    }
    if (humi >= 80 && humi <= 90)
    {
      Serial.println("Humidity is ok");
      Serial.flush();
      RelayOff_R(theHP->pin_Fan);
      
      if (humi >= 85)
      {
        Serial.println("Humidifier is off");
        Serial.flush();
        RelayOff_R(theHP->pin_Humi);  // turn the Humidifier off
      }
    }
    if(humi< 80)
    {
      Serial.println("Humidity is too low");
      Serial.flush();
      RelayOff_R(theHP->pin_Fan);        // turn the Fan off 
      RelayOn_R(theHP->pin_Humi);  // turn the Humidifier on
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




void RelayOn(int pin)
{
  Serial.print("Pin state is on for ");
  Serial.println(pin);
  digitalWrite(pin, LOW);   // turn the Fun on
}
void RelayOff(int pin)
{
  Serial.print("Pin state is off for ");
  Serial.println(pin);
  digitalWrite(pin, HIGH);   // turn the Fun on
}

void RelaySwitch(void* context)
{
  Serial.println("RelaySwitch proc is on: ");
  struct SwitchDuration * theSD = (struct SwitchDuration *)context;
  RelayOn(theSD->pin);
  delay(theSD->duration);
  RelayOff(theSD->pin);
}



void RelayOn_R(int pin)
{
  Serial.print("Pin state is on for ");
  Serial.println(pin);
  digitalWrite(pin, HIGH);   // turn the Fun on
}
void RelayOff_R(int pin)
{
  Serial.print("Pin state is off for ");
  Serial.println(pin);
  digitalWrite(pin, LOW);   // turn the Fun on
}

void RelaySwitch_R(void* context)
{
  Serial.println("RelaySwitch proc is on: ");
  struct SwitchDuration * theSD = (struct SwitchDuration *)context;
  RelayOn_R(theSD->pin);
  delay(theSD->duration);
  RelayOff_R(theSD->pin);
}

