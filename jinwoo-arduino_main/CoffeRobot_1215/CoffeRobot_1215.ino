////바퀴 모터 PWM
#define DIR1 2
#define PWM1 3
#define DIR2 4
#define PWM2 5  //Driving motors

/////컵 모터
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

//////초음파 장애물 회피
#define TRIG_F 24
#define ECHO_F 26
#define TRIG_L 25
#define ECHO_L 27
#define TRIG_R 28
#define ECHO_R 29

//////IR 라인트래킹
#define IR_R 40  //I3
#define IR_F 38  //I2
#define IR_L 42  //I1   Ir sensor right & left

/////워터펌프 PWM
#define M1_pinNum_1  46    //coffee
#define M1_pinNum_2  47
#define M1_SpeedPin  48

#define M2_pinNum_1  49     //water
#define M2_pinNum_2  50
#define M2_SpeedPin  51

///// 초음파 컵 인식
#define TRIG_cup 52
#define ECHO_cup 53

/*
  int order_data = 0;
  bool water_pump_flag = 0;
  int cup_sensor_flag = 0;
*/
char cmd = 'Z';

unsigned long start_time = 0;
unsigned long end_time = 0;

///모드 변수
int hand_flag = 0; //user detecting flag
int drivingflag = 0; //now not using. why???
int avoid_start_flag = 0;

///라인트래킹 변수
int ir_R;
int ir_F;
int ir_L;  // ir sensor read

///pid 주행변수
float Kp = 1.2;
float Kd = 0;
float Ki = 0;

float standard_speed = 70;
float e, e_dot, int_e, pree = 0;
float pretime, dt = 0;

///컵 인식 변수
float cup_dist = 15;
int cup_sensor_flag = 0;


///함수 원형
void stop();

float f_getDistanceCM();
float r_getDistanceCM();
float l_getDistanceCM();
void forward(float speed_by_distance);
void back(float speed_by_distance);
void left();
void right();
void half_shot();
void one_shot();
void two_shot();
void cup_dispenser();
float cup_CM();




void setup() {
  //A_Serial.begin(9600);
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(DIR1, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(DIR2, OUTPUT);
  pinMode(PWM2, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(IR_R, INPUT);
  pinMode(IR_F, INPUT);
  pinMode(IR_L, INPUT);

  pinMode(TRIG_F, OUTPUT);
  pinMode(ECHO_F, INPUT);
  pinMode(TRIG_R, OUTPUT);
  pinMode(ECHO_R, INPUT);
  pinMode(TRIG_L, OUTPUT);
  pinMode(ECHO_L, INPUT);

  pinMode(M1_pinNum_1, OUTPUT);
  pinMode(M1_pinNum_2, OUTPUT);
  pinMode(M1_SpeedPin, OUTPUT);

  pinMode(M2_pinNum_1, OUTPUT);
  pinMode(M2_pinNum_2, OUTPUT);
  pinMode(M2_SpeedPin, OUTPUT);
}

void loop() {
  //Serial.println("100");
  if (Serial.available()) {
    cmd = Serial.read();

    if (cmd == 'S') {
      //Serial.println("Stop Motor");
      //digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      stop();
      hand_flag = 1;
      //Serial.println("100");
    }
    //Serial.println("12");
    delay(500);

    while (hand_flag == 1) {
      //Serial.println("34");
      start_time = millis();
      end_time = millis();

      while ((end_time - start_time) <= 10000) {
        cmd = Serial.read();
        end_time = millis();
        //Serial.println();

        if (cmd == 'O') {
          //Serial.println("User Finded");
          //digitalWrite(LED_BUILTIN, HIGH);
          delay(100);
          //Serial.println("99");
          hand_flag = 2;
          break;
        }
        else {
          hand_flag = 0;
          //Serial.println("78");
        }
      }
    }
    //Serial.println("56");

    while (hand_flag == 2) {
      cmd = Serial.read();

      if (cmd == 'A') {
        //Serial.println("You order Half shot");
        //digitalWrite(LED_BUILTIN, HIGH);
        delay(100);

        //Serial.println("11");
        cup_dispenser();
        //Serial.println("22");
        delay(2000);
        cup_dist = cup_CM();
        //Serial.println(cup_dist);

        while (cup_dist > 12) {
          cup_dist = cup_CM();
          //Serial.println("33");
        }
        //Serial.println("44");
        cup_sensor_flag = 1;
        Serial.println("5");
        half_shot();


        //cup이 떨어지고 난 뒤 cup이 멀어지면 flag=2로
        while (cup_sensor_flag == 1 && cup_dist <= 12) {
          cup_dist = cup_CM();
          Serial.println(cup_dist);
          delay(1000);
        }
        //Serial.println("66");
        cup_sensor_flag = 0;
        Serial.println("9");
        cmd = 'Z';
        delay(5000);
        hand_flag = 0;  //워터펌프 제어코드 추가, 컵 인식플래그 만들고 인식플래그 변화완료되면 다시 주행시작하도록 해야 함.
      }

      if (cmd == 'B') {
        //Serial.println("You order Half shot");
        //digitalWrite(LED_BUILTIN, HIGH);
        delay(100);

        //Serial.println("11");
        cup_dispenser();
        //Serial.println("22");
        delay(2000);
        cup_dist = cup_CM();
        //Serial.println(cup_dist);

        while (cup_dist > 12) {
          cup_dist = cup_CM();
          //Serial.println("33");
        }
        //Serial.println("44");
        cup_sensor_flag = 1;
        Serial.println("5");
        one_shot();


        //cup이 떨어지고 난 뒤 cup이 멀어지면 flag=2로
        while (cup_sensor_flag == 1 && cup_dist <= 12) {
          cup_dist = cup_CM();
          delay(1000);
          //Serial.println("55");
        }
        //Serial.println("66");
        cup_sensor_flag = 0;
        Serial.println("9");
        cmd = 'Z';
        delay(5000);
        hand_flag = 0;  //워터펌프 제어코드 추가, 컵 인식플래그 만들고 인식플래그 변화완료되면 다시 주행시작하도록 해야 함.
      }

      if (cmd == 'C') {
        //Serial.println("You order Half shot");
        //digitalWrite(LED_BUILTIN, HIGH);
        delay(100);

        //Serial.println("11");
        cup_dispenser();
        //Serial.println("22");
        delay(2000);
        cup_dist = cup_CM();
        //Serial.println(cup_dist);

        while (cup_dist > 12) {
          cup_dist = cup_CM();
          //Serial.println("33");
        }
        //Serial.println("44");
        cup_sensor_flag = 1;
        Serial.println("5");
        two_shot();


        //cup이 떨어지고 난 뒤 cup이 멀어지면 flag=2로
        while (cup_sensor_flag == 1 && cup_dist <= 12) {
          cup_dist = cup_CM();
          delay(1000);
          //Serial.println("55");
        }
        //Serial.println("66");
        cup_sensor_flag = 0;
        Serial.println("9");
        cmd = 'Z';
        delay(5000);
        hand_flag = 0;  //워터펌프 제어코드 추가, 컵 인식플래그 만들고 인식플래그 변화완료되면 다시 주행시작하도록 해야 함.
      }
    }
  }
  // hand sign -->motor stop
  //s-> stop 약자
  //o-> order
  //A,B,C->반샷 한샷 투샷
  //5-> 워터펌프 작동 완료 시그널/9-> 사람이 컵까지 가져갔을 때 시그널

  ////////////////////////////////////////////////////////////////////////////////////////////////////////아래부터 주행부

  //초음파 센서 읽는부분 추가
  float cm = 60;
  cm = f_getDistanceCM();

  
  //Serial.print("\n Sensor1 (Distance): ");

  //Serial.print(cm);
  float speed_by_distance = map(cm, 0, 60, 40, 90);

  float leftDistance = 60;
  float rightDistance = 60;

  if(cm < 10) {
    stop();
    delay(500);
  }

  else if (cm < 30) {
    //Serial.println("zzzzzzz");
    //0.5초간 정지합니다.
    stop();
    delay(1000);

    leftDistance = l_getDistanceCM();
    rightDistance = r_getDistanceCM();
    //  0.3초간 후진합니다.
    back(speed_by_distance);
    //Serial.print("frontDistance: ");
    //Serial.println(cm);
    //Serial.print("leftDistance: ");
    //Serial.println(leftDistance);
    //Serial.print("rightDistance: ");
    //Serial.println(rightDistance);

    avoid_start_flag = 1;

    stop();
    delay(1000);
    /*
      왼쪽이 오른쪽보다 크다면 0.5초간 좌회전하며,
      아니라면(작다면)  0.5초간 우회전합니다.
    */


    if (avoid_start_flag == 1) {
      if (leftDistance > rightDistance) {
        digitalWrite(DIR1, HIGH);
        analogWrite(PWM1, 20);
        digitalWrite(DIR2, HIGH);
        analogWrite(PWM2, 75);
        delay(1100);
        stop();
        delay(1000);
        ir_R = 0;
        ir_F = 0;
        ir_L = 0;


        while (ir_R == 0 && ir_F == 0 && ir_L == 0) {
          cm = f_getDistanceCM();
          if (cm <= 25) {
            digitalWrite(DIR1, HIGH);
            analogWrite(PWM1, 20);
            digitalWrite(DIR2, HIGH);
            analogWrite(PWM2, 75);
            delay(1100);
            stop();
            delay(1000);
          }
          right();
        }

        avoid_start_flag = 0;

        //Serial.print("rightDistance:");
        //Serial.println(rightDistance);
        //rightDistance = r_getDistanceCM();
        //if (ir_R == 1 || ir_F == 1 || ir_L == 1) { //ir 센서 검출되면 0으로 보냄
        //  avoid_start_flag = 0;
        //}
      }

      else if (leftDistance < rightDistance) {
        digitalWrite(DIR1, HIGH);
        analogWrite(PWM1, 75);
        digitalWrite(DIR2, HIGH);
        analogWrite(PWM2, 20);
        delay(1100);
        stop();
        delay(1000);
        ir_R = 0;
        ir_F = 0;
        ir_L = 0;

        while (ir_R == 0 && ir_F == 0 && ir_L == 0) {
          cm = f_getDistanceCM();
          if (cm <= 25) {
            digitalWrite(DIR1, HIGH);
            analogWrite(PWM1, 75);
            digitalWrite(DIR2, HIGH);
            analogWrite(PWM2, 20);
            delay(1100);
            stop();
            delay(1000);
          }
          left();
          //Serial.println("여기 맞아요");
        }
        //Serial.println("여기 아닌데아닌데? 아닌데?");
        avoid_start_flag = 0;


        //Serial.print("leftDistance:");
        //Serial.println(leftDistance);
        leftDistance = l_getDistanceCM();
        if (ir_R == 0 && ir_F == 0 && ir_L == 0) { //ir 센서 검출되면 0으로 보냄
          avoid_start_flag = 0;
        }
        //Serial.println("bbbbbbb");
      }

      else { //좌측과 우측의 거리가 동일하게 나온부분
        //forward(speed_by_distance); <-원래는 직진시키려했는데, 앞을 계속 막고있으니 직진을 못하고 있었을 것
        //따라서 일단 좌우측 거리가 동일하게 나오면 왼쪽으로 피하게끔 설정해봄.
        digitalWrite(DIR1, HIGH);
        analogWrite(PWM1, 20);
        digitalWrite(DIR2, HIGH);
        analogWrite(PWM2, 75);
        delay(1100);
        stop();
        delay(1000);
        ir_R = 0;
        ir_F = 0;
        ir_L = 0;


        while (ir_R == 0 && ir_F == 0 && ir_L == 0) {
          cm = f_getDistanceCM();
          if (cm <= 25) {
            digitalWrite(DIR1, HIGH);
            analogWrite(PWM1, 20);
            digitalWrite(DIR2, HIGH);
            analogWrite(PWM2, 75);
            delay(1100);
            stop();
            delay(1000);
          }
          right();
        }

        avoid_start_flag = 0;

        //Serial.print("rightDistance:");
        //Serial.println(rightDistance);
        rightDistance = r_getDistanceCM();
        //if (ir_R == 1 || ir_F == 1 || ir_L == 1) { //ir 센서 검출되면 0으로 보냄
        //  avoid_start_flag = 0;
        //}
      }



      //if (ir_R == 1 || ir_F == 1 || ir_L == 1) { //ir 센서 검출되면 0으로 보냄
      //  avoid_start_flag = 0;
      //}
      //Serial.print("AAAAA");
    }
  }

  else if (cm >= 40 && avoid_start_flag == 0) { // 요부분에 ir 주행 추가
    //Serial.print(cmd);
    if (hand_flag == 0 && cmd != 'S') {
      ir_R = digitalRead(IR_R);
      ir_F = digitalRead(IR_F);
      ir_L = digitalRead(IR_L);
      //Serial.print("ir_r:");
      //Serial.println(ir_R);
      //Serial.print("ir_L:");
      //Serial.println(ir_L);
      //Serial.print("ir_F:");
      //Serial.println(ir_F);
      if (ir_R == 1 || (ir_F == 0 && ir_L == 0)) {       //rotate_R
        digitalWrite(DIR1, HIGH);
        analogWrite(PWM1, 70);
        digitalWrite(DIR2, HIGH);
        analogWrite(PWM2, 30);
        //drivingflag = 1;
        //Serial.println(ir_R);
      }
      else if ((ir_R == 0 && ir_F == 0) || ir_L == 1) {  //rotate_L
        digitalWrite(DIR1, HIGH);
        analogWrite(PWM1, 30);
        digitalWrite(DIR2, HIGH);
        analogWrite(PWM2, 70);
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
//      else
//      {
//        digitalWrite(DIR1, HIGH);
//        analogWrite(PWM1, 75);
//        digitalWrite(DIR2, HIGH);
//        analogWrite(PWM2, 20);
//      }
      //Serial.println("hhhhhhh");
    }
    //Serial.print("BBBBB");
  }
  //Serial.print("CCCCCC");
}





/////////////////////////////////////////////////////////////////////////////////////////////////////////////아래부터 함수 선언
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
  ir_R = digitalRead(IR_R);
  ir_F = digitalRead(IR_F);
  ir_L = digitalRead(IR_L);
  //왼쪽모터 기본속도

  //오른쪽모터 PID만큼 빠르게?
  dt = (micros() - pretime) / 1000000;
  pretime = micros();


  e = 30 - l_getDistanceCM();
  int_e += e * dt;
  e_dot = (e - pree) / dt;
  pree = e;

  digitalWrite(DIR1, HIGH);
  analogWrite(PWM1, standard_speed + (Kp * e + Kd * e_dot + Ki * int_e));
  digitalWrite(DIR2, HIGH);
  analogWrite(PWM2, standard_speed );  //숫자조절 필요

  //Serial.println("*left: ");
  //Serial.print("leftwheel: ");
  //Serial.println(standard_speed + (Kp * e + Kd * e_dot + Ki * int_e));
  //Serial.print("rightwheel: ");
  //Serial.println(standard_speed );
  //Serial.print("Kp: ");
  //Serial.println(Kp);
}

/**
   RC카 우회전
   오른쪽 모터를 정지, 왼쪽 모터를 정회전하여 우회전합니다.
*/

void right() {
  //왼쪽모터 PID만큼 빠르게?
  ir_R = digitalRead(IR_R);
  ir_F = digitalRead(IR_F);
  ir_L = digitalRead(IR_L);

  dt = (micros() - pretime) / 1000000;
  pretime = micros();

  e = 30 - r_getDistanceCM();
  int_e += e * dt;
  e_dot = (e - pree) / dt;
  pree = e;

  digitalWrite(DIR1, HIGH);
  analogWrite(PWM1, standard_speed );  //숫자조절 필요
  digitalWrite(DIR2, HIGH);
  analogWrite(PWM2, standard_speed + (Kp * e + Kd * e_dot + Ki * int_e));

  //오른쪽모터 기본속도
  //Serial.println("*right: ");
  //Serial.print("leftwheel: ");
  //Serial.println(standard_speed + (Kp * e + Kd * e_dot + Ki * int_e));
  //Serial.print("rightwheel: ");
  //Serial.println(standard_speed);
  //Serial.println(".");
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


//////////////////////////////////////////////////////////
//water pump functions
void half_shot()
{
  digitalWrite(M2_pinNum_1, LOW);
  digitalWrite(M2_pinNum_2, HIGH);
  analogWrite(M2_SpeedPin, 130);
  delay(2000);//2번 모터의 작동 시간 조절부
  digitalWrite(M1_pinNum_1, LOW);
  digitalWrite(M1_pinNum_2, HIGH);
  analogWrite(M1_SpeedPin, 130);
  delay(700);//1번 모터의 작동 시간 조절부
  digitalWrite(M1_pinNum_1, LOW);
  digitalWrite(M1_pinNum_2, LOW);
  analogWrite(M1_SpeedPin, 0);
  //delay(100);//1번 모터의 작동 시간 조절부
  digitalWrite(M2_pinNum_1, LOW);
  digitalWrite(M2_pinNum_2, LOW);
  analogWrite(M2_SpeedPin, 0);
  delay(100);//2번 모터의 작동 시간 조절부
}


void one_shot()
{
  digitalWrite(M1_pinNum_1, LOW);
  digitalWrite(M1_pinNum_2, HIGH);
  analogWrite(M1_SpeedPin, 130);
  digitalWrite(M2_pinNum_1, LOW);
  digitalWrite(M2_pinNum_2, HIGH);
  analogWrite(M2_SpeedPin, 130);
  delay(1300);//2번 모터의 작동 시간 조절부
  digitalWrite(M1_pinNum_1, LOW);
  digitalWrite(M1_pinNum_2, LOW);
  analogWrite(M1_SpeedPin, 0);
  //delay(100);//1번 모터의 작동 시간 조절부
  digitalWrite(M2_pinNum_1, LOW);
  digitalWrite(M2_pinNum_2, LOW);
  analogWrite(M2_SpeedPin, 0);
  delay(100);//2번 모터의 작동 시간 조절부
}


void two_shot()
{
  digitalWrite(M1_pinNum_1, LOW);
  digitalWrite(M1_pinNum_2, HIGH);
  analogWrite(M1_SpeedPin, 130);
  delay(2000);//1번 모터의 작동 시간 조절부
  digitalWrite(M2_pinNum_1, LOW);
  digitalWrite(M2_pinNum_2, HIGH);
  analogWrite(M2_SpeedPin, 130);
  delay(700);//2번 모터의 작동 시간 조절부
  digitalWrite(M1_pinNum_1, LOW);
  digitalWrite(M1_pinNum_2, LOW);
  analogWrite(M1_SpeedPin, 0);
  //delay(100);//1번 모터의 작동 시간 조절부
  digitalWrite(M2_pinNum_1, LOW);
  digitalWrite(M2_pinNum_2, LOW);
  analogWrite(M2_SpeedPin, 0);
  delay(100);//2번 모터의 작동 시간 조절부
}

void cup_dispenser() {
  for (int i = 0; i < 50; i++) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(5);

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(5);

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(5);

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(5);
  }
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  delay(700);
  
  for (int i = 0; i < 50; i++) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(5);

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(5);

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(5);

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(5);
  }
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

float cup_CM() {
  digitalWrite(TRIG_cup, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_cup, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_cup, LOW);

  //    거리값을 저장합니다.
  unsigned long cup_distance1 = pulseIn(ECHO_cup, HIGH);
  float cup_distance = cup_distance1 * 17 / 1000;
  delay(100);

  if (cup_distance > 15) {
    cup_distance = 15;
  }

  return cup_distance;
}
