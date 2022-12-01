#include <OneWire.h>
#include <DallasTemperature.h>

#define PELT 2
#define ONE_WIRE_BUS A0
#define des_temp 19

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float temp = 0;
bool peltier_on = 0;

void setup() {
  Serial.begin(9600);
  sensors.begin();
  pinMode(PELT,OUTPUT);
}

void loop() {
  Serial.print("TEMP: ");
  sensors.requestTemperatures();
  temp = sensors.getTempCByIndex(0);
  Serial.println(temp);

  if(temp >= 20){
    peltier_on = 1;
  }
  else{
    peltier_on = 0;
  }

  while(peltier_on == 1)
  {
    Serial.print("TEMP: ");
    sensors.requestTemperatures();
    temp = sensors.getTempCByIndex(0);
    Serial.println(temp);
    digitalWrite(PELT,HIGH);
    
    if(temp<=des_temp){
      peltier_on = 0;      
    }
  }
  digitalWrite(PELT,LOW);
}
