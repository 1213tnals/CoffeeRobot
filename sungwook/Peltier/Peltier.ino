#include <DHT.h>

#define PELT 2

void setup() {
  pinMode(PELT,OUTPUT);
  dht.begin();
}

void loop() {
  float temp = dht.readTemparature();

  if(temp >= 8){
    digitalWrite(PELT,HIGH);
  }
  else{
    digitalWrite(PELT,LOW);
  }

}
