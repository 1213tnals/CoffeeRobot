#include <Stepper.h>

const int stepsPerRevolution = 36;
Stepper myStepper(stepsPerRevolution, 30, 31, 32, 33); //사용한 PIN번호 수정해야함!!!!!!!!!!!!!!!!!!!!!!!!!!

void setup() {
  // put your setup code here, to run once:
  myStepper.setSpeed(60); // set the speed at 60 rpm:
}

void loop() {
  // put your main code here, to run repeatedly:
  myStepper.step(stepsPerRevolution);
  delay(500);
  myStepper.step(-stepsPerRevolution);
  delay(500);
}
