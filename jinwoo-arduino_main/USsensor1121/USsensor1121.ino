#define DIR1 2
#define PWM1 3
#define DIR2 4
#define PWM2 5     //Driving motors


#define TRIG_F 24
#define ECHO_F 26
#define TRIG_R 25
#define ECHO_R 27
#define TRIG_L 28
#define ECHO_L 29



void setup() {
  pinMode(DIR1, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(DIR2, OUTPUT);
  pinMode(PWM2, OUTPUT);

  pinMode(TRIG_F,OUTPUT);
  pinMode(ECHO_F,INPUT);
  pinMode(TRIG_R,OUTPUT);
  pinMode(ECHO_R,INPUT);
  pinMode(TRIG_L,OUTPUT);
  pinMode(ECHO_L,INPUT);
}

void loop() {
  int cm = f_getStableDistanceCM();
  long leftDistance = 0;
  long rightDistance = 0;
  if(Serial.available()){
    Serial.println(cm);
  }
  
 
  //25cm이내에 벽이 있다면 아래의 코드를 실행합니다.
  if (cm < 25) {
    //0.5초간 정지합니다.
    stop();
    delay(50);
    leftDistance = l_getStableDistanceCM();
    rightDistance = r_getStableDistanceCM();

 
    //  0.3초간 후진합니다.
    back();
    delay(3000);
    /*
     왼쪽이 오른쪽보다 크다면 0.5초간 좌회전하며, 
     아니라면(작다면)  0.5초간 우회전합니다.
     */
    if (leftDistance > rightDistance) left();
    else right();
    
    delay(800);
    //   25cm이내에 벽이 없다면 전진합니다.
  } 
  else {
    forward();
  }

}



/**
   전방 거리측정(CM)
   초음파센서를 이용해서 벽까지의 거리(mm)를 구합니다.
*/
float f_getDistanceCM() {
  digitalWrite(ECHO_F, LOW);
  digitalWrite(TRIG_F, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_F, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_F, LOW);

  //    거리값을 저장합니다.
  float f_distance = pulseIn(ECHO_F, HIGH)  / 29.0 / 2.0;

  return f_distance;
}
/**
   전방 신뢰할 수 있는 거리측정(CM)
   초음파센서를 여러번 나누어 신뢰할 수 있는 거리값을 구합니다.
   주의: 초음파센서 등 많은 측정 센서들의 경우, 중간에 값의 오차가 발생하는 경우로 인해 오동작을 유발하므로, 여러번 측정 후 평균값을 구하는 방식으로 개선하여 사용합니다.
   만약 다음과 같이 해도 허공에서 물체를 감지하는 등의 문제가 발생한다면 10이 아닌 다른 수로 변경하시어 테스트하셔도 됩니다.
*/
float f_getStableDistanceCM() {
  int fCmSum = 0;

  for (int i = 0; i < 10; i++) {
    fCmSum += f_getDistanceCM();
  }


  return fCmSum / 10;

  
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
  float r_distance = pulseIn(ECHO_R, HIGH)  / 29.0 / 2.0;

  return r_distance;
}
/**
   우측 신뢰할 수 있는 거리측정(CM)
   초음파센서를 여러번 나누어 신뢰할 수 있는 거리값을 구합니다.
   주의: 초음파센서 등 많은 측정 센서들의 경우, 중간에 값의 오차가 발생하는 경우로 인해 오동작을 유발하므로, 여러번 측정 후 평균값을 구하는 방식으로 개선하여 사용합니다.
   만약 다음과 같이 해도 허공에서 물체를 감지하는 등의 문제가 발생한다면 10이 아닌 다른 수로 변경하시어 테스트하셔도 됩니다.
*/
float r_getStableDistanceCM() {
  int rCmSum = 0;

  for (int i = 0; i < 10; i++) {
    rCmSum += r_getDistanceCM();
  }

  return rCmSum / 10;
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
  float l_distance = pulseIn(ECHO_L, HIGH)  / 29.0 / 2.0;

  return l_distance;
}
/**
   좌측 신뢰할 수 있는 거리측정(CM)
   초음파센서를 여러번 나누어 신뢰할 수 있는 거리값을 구합니다.
   주의: 초음파센서 등 많은 측정 센서들의 경우, 중간에 값의 오차가 발생하는 경우로 인해 오동작을 유발하므로, 여러번 측정 후 평균값을 구하는 방식으로 개선하여 사용합니다.
   만약 다음과 같이 해도 허공에서 물체를 감지하는 등의 문제가 발생한다면 10이 아닌 다른 수로 변경하시어 테스트하셔도 됩니다.
*/
float l_getStableDistanceCM() {
  int lCmSum = 0;

  for (int i = 0; i < 10; i++) {
    lCmSum += l_getDistanceCM();
  }

  return lCmSum / 10;
}








/**
   RC카 전진
   왼쪽,오른쪽 모터를 정회전하여 전진합니다.
*/
void forward() {

  //왼쪽모터 정회전
  digitalWrite(DIR1, HIGH);
  analogWrite(PWM1, 50);

  //오른쪽모터 정회전
  digitalWrite(DIR2,HIGH);
  analogWrite(PWM2, 50);
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
  digitalWrite(DIR2,LOW);
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
  digitalWrite(DIR2, HIGH);
  analogWrite(PWM2, 80);//숫자조절 필요
}

/**
   RC카 우회전
   오른쪽 모터를 정지, 왼쪽 모터를 정회전하여 우회전합니다.
*/

void right() {
 //왼쪽모터 PID만큼 빠르게?
  digitalWrite(DIR1, HIGH);
  analogWrite(PWM1, 80);//숫자조절 필요

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
  analogWrite(DIR1, 0);
  analogWrite(PWM1, 0);
  //오른쪽모터 정지
  analogWrite(DIR2, 0);
  analogWrite(PWM2, 0);
}
