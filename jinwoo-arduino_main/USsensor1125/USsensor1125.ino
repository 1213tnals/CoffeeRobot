#define DIR1 2
#define PWM1 3
#define DIR2 4
#define PWM2 5  //Driving motors


#define TRIG_F 24
#define ECHO_F 26
#define TRIG_R 25
#define ECHO_R 27
#define TRIG_L 28
#define ECHO_L 29

float Kp = 0.01;
float Kd = 0;
float Ki = 0;
//gain values

long e, e_dot, int_e, pree = 0;
float pretime, dt = 0;

void setup() {
  Serial.begin(9600);
  pinMode(DIR1, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(DIR2, OUTPUT);
  pinMode(PWM2, OUTPUT);

  pinMode(TRIG_F, OUTPUT);
  pinMode(ECHO_F, INPUT);
  pinMode(TRIG_R, OUTPUT);
  pinMode(ECHO_R, INPUT);
  pinMode(TRIG_L, OUTPUT);
  pinMode(ECHO_L, INPUT);
}

void loop() {
  float cm = f_getDistanceCM();


  float speed_by_distance = map(cm, 0, 60, 50, 80);

  long leftDistance = 0;
  long rightDistance = 0;



  //25cm이내에 벽이 있다면 아래의 코드를 실행합니다.
  if (cm < 40) {
    //0.5초간 정지합니다.
    stop();
    delay(500);
    leftDistance = l_getDistanceCM();
    rightDistance = r_getDistanceCM();


    //  0.3초간 후진합니다.
    back();
    delay(500);


    /*
     왼쪽이 오른쪽보다 크다면 0.5초간 좌회전하며, 
     아니라면(작다면)  0.5초간 우회전합니다.
     */
    if (leftDistance > rightDistance) {
      left();
    }

    else {
      right();
    }

    delay(3000);  //딜레이가 아니라 다시 선을 만날때까지 회전하도록 바꿔야 함
    //   25cm이내에 벽이 없다면 전진합니다.
  }
  else {
  forward(speed_by_distance);
  delay(3000);
  }
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
  Serial.print("\n Sensor1 (Distance): ");

  Serial.print(f_distance);

  return f_distance;
}
/**
   전방 신뢰할 수 있는 거리측정(CM)
   초음파센서를 여러번 나누어 신뢰할 수 있는 거리값을 구합니다.
   주의: 초음파센서 등 많은 측정 센서들의 경우, 중간에 값의 오차가 발생하는 경우로 인해 오동작을 유발하므로, 여러번 측정 후 평균값을 구하는 방식으로 개선하여 사용합니다.
   만약 다음과 같이 해도 허공에서 물체를 감지하는 등의 문제가 발생한다면 10이 아닌 다른 수로 변경하시어 테스트하셔도 됩니다.
*/
// float f_getStableDistanceCM() {
//   float fCmSum = 0;

//   for (int i = 0; i < 10; i++) {
//     fCmSum += f_getDistanceCM();

//   }
//   fCmSum = fCmSum / 10;
//   Serial.print("\n Sensor1 (Distance): ");

//  Serial.print(fCmSum);


//   return fCmSum;


// }

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
  float r_distance = pulseIn(ECHO_R, HIGH);

  r_distance = r_distance * 17 / 1000;

  if (r_distance > 60) {
    r_distance = 60;
  }


  return r_distance;
}
/**
   우측 신뢰할 수 있는 거리측정(CM)
   초음파센서를 여러번 나누어 신뢰할 수 있는 거리값을 구합니다.
   주의: 초음파센서 등 많은 측정 센서들의 경우, 중간에 값의 오차가 발생하는 경우로 인해 오동작을 유발하므로, 여러번 측정 후 평균값을 구하는 방식으로 개선하여 사용합니다.
   만약 다음과 같이 해도 허공에서 물체를 감지하는 등의 문제가 발생한다면 10이 아닌 다른 수로 변경하시어 테스트하셔도 됩니다.
*/
// float r_getStableDistanceCM() {
//   int rCmSum = 0;

//   for (int i = 0; i < 10; i++) {
//     rCmSum += r_getDistanceCM();
//   }

//   return rCmSum / 10;
// }




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
  float l_distance = pulseIn(ECHO_L, HIGH);
  l_distance = l_distance * 17 / 1000;


  if (l_distance > 60) {
    l_distance = 60;
  }

  return l_distance;
}
/**
   좌측 신뢰할 수 있는 거리측정(CM)
   초음파센서를 여러번 나누어 신뢰할 수 있는 거리값을 구합니다.
   주의: 초음파센서 등 많은 측정 센서들의 경우, 중간에 값의 오차가 발생하는 경우로 인해 오동작을 유발하므로, 여러번 측정 후 평균값을 구하는 방식으로 개선하여 사용합니다.
   만약 다음과 같이 해도 허공에서 물체를 감지하는 등의 문제가 발생한다면 10이 아닌 다른 수로 변경하시어 테스트하셔도 됩니다.
*/
// float l_getStableDistanceCM() {
//   int lCmSum = 0;

//   for (int i = 0; i < 10; i++) {
//     lCmSum += l_getDistanceCM();
//   }

//   return lCmSum / 10;
// }








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

void back() {
  //왼쪽모터 역회전
  digitalWrite(DIR1, LOW);
  analogWrite(PWM1, 50);

  //오른쪽모터 역회전
  digitalWrite(DIR2, LOW);
  analogWrite(PWM2, 50);
}

/**
   RC카 좌회전
   오른쪽 모터를 정회전, 왼쪽모터를 정지하여 좌회전합니다.
*/
void left() {
  //왼쪽모터 기본속도
  digitalWrite(DIR1, HIGH);
  analogWrite(PWM1, 50);

  //오른쪽모터 PID만큼 빠르게?
  dt = (micros() - pretime) / 1000000;
  pretime = micros();


  e = 3 - l_getDistanceCM();
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

  e = 25 - r_getDistanceCM();
  int_e += e * dt;
  e_dot = (e - pree) / dt;
  pree = e;
  digitalWrite(DIR1, HIGH);
  analogWrite(PWM1, Kp * e + Kd * e_dot + Ki * int_e);  //숫자조절 필요

  //오른쪽모터 기본속도
  digitalWrite(DIR2, HIGH);
  analogWrite(PWM2, 50);
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
