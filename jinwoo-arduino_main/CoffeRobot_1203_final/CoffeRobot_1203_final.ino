#include <SoftwareSerial.h>
///////////////
#define DIR1 2
#define PWM1 3
#define DIR2 4
#define PWM2 5  //Driving motors

#define IR_R 40  //I3
#define IR_F 38  //I2
#define IR_L 42  //I1   Ir sensor right & left

#define TRIG_F 24
#define ECHO_F 26
#define TRIG_L 25
#define ECHO_L 27
#define TRIG_R 28
#define ECHO_R 29

#define MEGA_TX 50
#define MEGA_RX 51

//SoftwareSerial A_Serial(MEGA_TX,MEGA_RX);

int order_data = 0;
bool water_pump_flag = 0;
int cup_sensor_flag = 0;

int drivingflag = 0; //now not using. why???
int avoid_start_flag = 0;

char cmd = 'Z';
int ir_R;
int ir_F;
int ir_L;  // ir sensor read

float Kp = 2.35;
float Kd = 0;
float Ki = 0;
//gain values

float e, e_dot, int_e, pree = 0;
float pretime, dt = 0;



int flag = 0; //user detecting flag

void stop();
void left();
float f_getDistanceCM();
float r_getDistanceCM();
float l_getDistanceCM();




void setup() {
  //A_Serial.begin(9600);
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(DIR1, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(DIR2, OUTPUT);
  pinMode(PWM2, OUTPUT);

  pinMode(IR_R, INPUT);
  pinMode(IR_F, INPUT);
  pinMode(IR_L, INPUT);

  pinMode(TRIG_F, OUTPUT);
  pinMode(ECHO_F, INPUT);
  pinMode(TRIG_R, OUTPUT);
  pinMode(ECHO_R, INPUT);
  pinMode(TRIG_L, OUTPUT);
  pinMode(ECHO_L, INPUT);
}

void loop() {
  //Serial.println("100");
  if (Serial.available()) {
    cmd = Serial.read();
    /*
      if (cmd == 'S') {
      //Serial.println("Stop Motor");
      //digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      stop();
      flag == 1;
      Serial.println("100");
      }

      if (cmd == 'O') {
      //Serial.println("User Finded");
      //digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      Serial.println("99");
      }

      if (cmd == 'A') {
      //Serial.println("You order Half shot");
      //digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      order_data = 0;
      A_Serial.println(order_data);
      Serial.println("5");

      if(A_Serial.available())
      {
        while(water_pump_flag != 1)
        {
          water_pump_flag = A_Serial.read();
        }
        water_pump_flag = 0;
      }
      //delay(5000);  //assume: after 5sec, user has gone
      Serial.println("9");

      if(A_Serial.available())
      {
        while(cup_sensor_flag != 2)
        {
          cup_sensor_flag = A_Serial.read();
        }
        cup_sensor_flag = 0;
      }

      cmd = 'Z';

      flag = 0;  //워터펌프 제어코드 추가, 컵 인식플래그 만들고 인식플래그 변화완료되면 다시 주행시작하도록 해야 함.
      }

      if (cmd == 'B') {
      //Serial.println("You order One shot");
      //digitalWrite(LED_BUILTIN, HIGH);
      order_data = 1;
      A_Serial.println(order_data);
      delay(100);
      Serial.println("5");

      if(A_Serial.available())
      {
        while(water_pump_flag != 1)
        {
          water_pump_flag = A_Serial.read();
        }
        water_pump_flag = 0;
      }
      //delay(5000);  //assume: after 5sec, user has gone
      Serial.println("9");

      if(A_Serial.available())
      {
        while(cup_sensor_flag != 2)
        {
          cup_sensor_flag = A_Serial.read();
        }
        cup_sensor_flag = 0;
      }

      cmd = 'Z';
      flag = 0;  //워터펌프 제어코드 추가, 컵 인식플래그 만들고 인식플래그 변화완료되면 다시 주행시작하도록 해야 함.
      }

      if (cmd == 'C') {
      //Serial.println("You order Two shot");
      //digitalWrite(LED_BUILTIN, HIGH);
      delay(100);  //assume: after 5sec, making finish
      order_data = 2;
      //A_Serial.println(order_data);
      Serial.println("5");

      if(A_Serial.available())
      {
        while(water_pump_flag != 1)
        {
          water_pump_flag = A_Serial.read();
        }
        water_pump_flag = 0;
      }
      //delay(5000);  //assume: after 5sec, user has gone
      Serial.println("9");

      if(A_Serial.available())
      {
        while(cup_sensor_flag != 2)
        {
          cup_sensor_flag = A_Serial.read();
        }
        cup_sensor_flag = 0;
      }

      cmd = 'Z';
      flag = 0;  //워터펌프 제어코드 추가, 컵 인식플래그 만들고 인식플래그 변화완료되면 다시 주행시작하도록 해야 함.
      }
    */
  }  // hand sign -->motor stop
  //s-> stop 약자
  //o-> order
  //A,B,C->반샷 한샷 투샷
  //5-> 워터펌프 작동 완료 시그널/9-> 사람이 컵까지 가져갔을 때 시그널

  //초음파 센서 읽는부분 추가
  float cm = 60;
  cm = f_getDistanceCM();
  //Serial.print("\n Sensor1 (Distance): ");

  //Serial.print(cm);
  float speed_by_distance = map(cm, 0, 60, 40, 90);

  float leftDistance = 60;
  float rightDistance = 60;


  if (cm < 40) {
    //Serial.println("zzzzzzz");
    //0.5초간 정지합니다.
    stop();
    delay(1000);

    leftDistance = l_getDistanceCM();
    rightDistance = r_getDistanceCM();
    //  0.3초간 후진합니다.
    back(speed_by_distance);
    Serial.print("frontDistance: ");
    Serial.println(cm);
    Serial.print("leftDistance: ");
    Serial.println(leftDistance);
    Serial.print("rightDistance: ");
    Serial.println(rightDistance);

    avoid_start_flag = 1;


    stop();
    delay(1000);
    /*
      왼쪽이 오른쪽보다 크다면 0.5초간 좌회전하며,
      아니라면(작다면)  0.5초간 우회전합니다.
    */


    if (avoid_start_flag == 1) {
      if (leftDistance > rightDistance) {
        while (rightDistance < 40) {
          left();
          //Serial.print("rightDistance:");
          //Serial.println(rightDistance);
          rightDistance = r_getDistanceCM();
          if (ir_R == 1 || ir_F == 1 || ir_L == 1) { //ir 센서 검출되면 0으로 보냄
            avoid_start_flag = 0;
          }
        }
      }

      else if (leftDistance < rightDistance) {
        while (leftDistance < 40) {
          right();
          //Serial.print("leftDistance:");
          //Serial.println(leftDistance);
          leftDistance = l_getDistanceCM();
          if (ir_R == 1 || ir_F == 1 || ir_L == 1) { //ir 센서 검출되면 0으로 보냄
            avoid_start_flag = 0;
          }
        }
        //Serial.println("bbbbbbb");
      }

      else { //좌측과 우측의 거리가 동일하게 나온부분
        //forward(speed_by_distance); <-원래는 직진시키려했는데, 앞을 계속 막고있으니 직진을 못하고 있었을 것
        //따라서 일단 좌우측 거리가 동일하게 나오면 왼쪽으로 피하게끔 설정해봄.
        left();
        //Serial.print("rightDistance:");
        //Serial.println(rightDistance);
        rightDistance = r_getDistanceCM();
        if (ir_R == 1 || ir_F == 1 || ir_L == 1) { //ir 센서 검출되면 0으로 보냄
          avoid_start_flag = 0;
        }
      }



      if (ir_R == 1 || ir_F == 1 || ir_L == 1) { //ir 센서 검출되면 0으로 보냄
        avoid_start_flag = 0;
      }
      Serial.print("AAAAA");
    }
  }

  else if (cm >= 40 && avoid_start_flag == 0) { // 요부분에 ir 주행 추가
    //Serial.print(cmd);
    if (flag == 0 && cmd != 'S') {
      ir_R = digitalRead(IR_R);
      ir_F = digitalRead(IR_F);
      ir_L = digitalRead(IR_L);
      Serial.print("ir_r:");
      Serial.println(ir_R);
      Serial.print("ir_L:");
      Serial.println(ir_L);
      Serial.print("ir_F:");
      Serial.println(ir_F);
      if (ir_R == 1 && ir_F == 0 && ir_L == 0) {       //rotate_R
        digitalWrite(DIR1, HIGH);
        analogWrite(PWM1, 85);
        digitalWrite(DIR2, HIGH);
        analogWrite(PWM2, 17);
        //drivingflag = 1;
        //Serial.println(ir_R);
      }
      else if (ir_R == 0 && ir_F == 0 && ir_L == 1) {  //rotate_L
        digitalWrite(DIR1, HIGH);
        analogWrite(PWM1, 17);
        digitalWrite(DIR2, HIGH);
        analogWrite(PWM2, 85);
        //drivingflag = 0;
        //Serial.println(ir_L);
      }
      else if (ir_R == 0 && ir_F == 1 && ir_L == 0) {     //stright black=1 white=0
        digitalWrite(DIR1, HIGH);
        analogWrite(PWM1, 70);
        digitalWrite(DIR2, HIGH);
        analogWrite(PWM2, 70);
        //Serial.println("eeeeeeee");
      }

      //Serial.println("hhhhhhh");
    }
    Serial.print("BBBBB");
  }
  Serial.print("CCCCCC");
}




/**
   전방 거리측정(CM)
   초음파센서를 이용해서 벽까지의 거리(mm)를 구합니다.
*/
float f_getDistanceCM() {
  digitalWrite(TRIG_F, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_F, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_F, LOW);

  //    거리값을 저장합니다.
  unsigned long f_distance1 = pulseIn(ECHO_F, HIGH);
  float f_distance = f_distance1 * 17 / 1000;
  delay(100);

  if (f_distance > 60) {
    f_distance = 60;
  }

  return f_distance;
}

/**
   우측 거리측정(CM)
   초음파센서를 이용해서 벽까지의 거리(mm)를 구합니다.
*/
float r_getDistanceCM() {
  digitalWrite(ECHO_R, LOW);
  digitalWrite(TRIG_R, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_R, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_R, LOW);

  //    거리값을 저장합니다.
  unsigned long r_distance1 = pulseIn(ECHO_R, HIGH);
  float r_distance = r_distance1 * 17 / 1000;

  if (r_distance > 60) {
    r_distance = 60;
  }


  return r_distance;
}

/**
   좌측 거리측정(CM)
   초음파센서를 이용해서 벽까지의 거리(mm)를 구합니다.
*/
float l_getDistanceCM() {
  digitalWrite(ECHO_L, LOW);
  digitalWrite(TRIG_L, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_L, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_L, LOW);

  //    거리값을 저장합니다.
  unsigned long l_distance1 = pulseIn(ECHO_L, HIGH);
  float l_distance = l_distance1 * 17 / 1000;


  if (l_distance > 60) {
    l_distance = 60;
  }

  return l_distance;
}

/**
   RC카 전진
   왼쪽,오른쪽 모터를 정회전하여 전진합니다.
*/
void forward(float speed_by_distance) {

  //왼쪽모터 정회전
  digitalWrite(DIR1, HIGH);
  analogWrite(PWM1, speed_by_distance);

  //오른쪽모터 정회전
  digitalWrite(DIR2, HIGH);
  analogWrite(PWM2, speed_by_distance);
}

/**
   RC카 후진
   왼쪽,오른쪽 모터를 역회전하여 후진합니다.
*/

void back(float speed_by_distance) {
  //왼쪽모터 역회전
  digitalWrite(DIR1, LOW);
  analogWrite(PWM1, speed_by_distance);

  //오른쪽모터 역회전
  digitalWrite(DIR2, LOW);
  analogWrite(PWM2, speed_by_distance);
}
/**
   RC카 좌회전
   오른쪽 모터를 정회전, 왼쪽모터를 정지하여 좌회전합니다.
*/
void left() {
  //왼쪽모터 기본속도
  digitalWrite(DIR1, HIGH);
  analogWrite(PWM1, 17);

  //오른쪽모터 PID만큼 빠르게?
  dt = (micros() - pretime) / 1000000;
  pretime = micros();


  e = 50 - r_getDistanceCM();
  int_e += e * dt;
  e_dot = (e - pree) / dt;
  pree = e;

  digitalWrite(DIR2, HIGH);
  analogWrite(PWM2, Kp * e + Kd * e_dot + Ki * int_e);  //숫자조절 필요

}

/**
   RC카 우회전
   오른쪽 모터를 정지, 왼쪽 모터를 정회전하여 우회전합니다.
*/

void right() {
  //왼쪽모터 PID만큼 빠르게?

  dt = (micros() - pretime) / 1000000;
  pretime = micros();

  e = 50 - l_getDistanceCM();
  int_e += e * dt;
  e_dot = (e - pree) / dt;
  pree = e;
  digitalWrite(DIR1, HIGH);
  analogWrite(PWM1, Kp * e + Kd * e_dot + Ki * int_e);  //숫자조절 필요

  //오른쪽모터 기본속도
  digitalWrite(DIR2, HIGH);
  analogWrite(PWM2, 17);

}

/**
   RC카 정지
   오른쪽,왼쪽모터를 모두 정지합니다.
*/
void stop() {
  //왼쪽모터 정지
  digitalWrite(DIR1, HIGH);
  analogWrite(PWM1, 0);
  //오른쪽모터 정지
  digitalWrite(DIR2, HIGH);
  analogWrite(PWM2, 0);
}
