#include <Stepper.h>

const int stepsPerRevolution = 36;  

Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11); //사용한 PIN번호

void setup() {
  // set the speed at 60 rpm:
  myStepper.setSpeed(200);
  // initialize the serial port:
  Serial.begin(9600);
  Serial.println("clockwise");
  myStepper.step(stepsPerRevolution);
  delay(500);

  // step one revolution in the other direction:
  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution);
  delay(500);
}

void loop() {
}
