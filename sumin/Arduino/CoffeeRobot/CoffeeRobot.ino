#define IN1 5
#define IN2 6
#define IN3 9
#define IN4 10     //for line tracking

#define IR_R 2
#define IR_L 4

#define US_F

#include "test.h"

void setup() {

  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(IR_R, INPUT);
  pinMode(IR_L, INPUT);
}

void loop() {
  if (Serial.available()) {      
    cmd = Serial.read();
    if (cmd == 'S') {
      Serial.println("Arduino: Stop Motor");
      digitalWrite(LED_BUILTIN, HIGH);
      delay(5000);
      digitalWrite(LED_BUILTIN, LOW);
      delay(5000);
    }

    else {

      ir_R = digitalRead(IR_R);
      ir_L = digitalRead(IR_L);

      if (ir_R == 1 && ir_L == 0) {         //rotate_R
        analogWrite(IN2, 0);
        analogWrite(IN3, 0);
      }
      else if (ir_R == 0 && ir_L == 1) {    //rotate_L
        analogWrite(IN2, 0);
        analogWrite(IN3, 0);
      }
      else {                                //straight
        analogWrite(IN4, 255);
        analogWrite(IN3, 0);
      }

      Serial.print(ir_R);
      Serial.print(' ');
      Serial.println(ir_L);
    }
  }
}
