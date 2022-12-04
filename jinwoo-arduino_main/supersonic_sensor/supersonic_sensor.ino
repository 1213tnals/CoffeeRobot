#define TRIG_L 25 //TRIG 핀 설정 (초음파 보내는 핀)
#define ECHO_L 27 //ECHO 핀 설정 (초음파 받는 핀)
#define TRIG_R 28 //TRIG 핀 설정 (초음파 보내는 핀)
#define ECHO_R 29 //ECHO 핀 설정 (초음파 받는 핀)
#define TRIG_F 24
#define ECHO_F 26

void setup() {
  Serial.begin(9600); //시리얼모니터로 센서값을 확인하기위해서 시리얼 통신을 정의                    
  pinMode(TRIG_L, OUTPUT);
  pinMode(ECHO_L, INPUT);
  pinMode(TRIG_R, OUTPUT);
  pinMode(ECHO_R, INPUT);
  pinMode(TRIG_F, OUTPUT);
  pinMode(ECHO_F, INPUT);
}

void loop(){

  unsigned long duration1, duration2, duration3;

  //초음파 센서 1
  digitalWrite(TRIG_L, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_L, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_L, LOW);
  duration1 = pulseIn (ECHO_L, HIGH); //물체에 반사되어돌아온 초음파의 시간을 변수에 저장
  
  //초음파 센서 2
  digitalWrite(TRIG_R, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_R, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_R, LOW);
  duration2 = pulseIn (ECHO_R, HIGH); //물체에 반사되어돌아온 초음파의 시간을 변수에 저장

  //초음파 센서 3
  digitalWrite(TRIG_F, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_F, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_F, LOW);
  duration3 = pulseIn (ECHO_F, HIGH); //물체에 반사되어돌아온 초음파의 시간을 변수에 저장
  
  //34000*초음파가 물체로 부터 반사되어 돌아오는시간 /1000000 / 2(왕복값이아니라 편도값이기때문에 나누기2를 해줍니다.)
  //초음파센서의 거리값이 위 계산값과 동일하게 Cm로 환산되는 계산공식

  float distance1 = duration1 * 17 / 1000; 
  float distance2 = duration2 * 17 / 1000;
  float distance3 = duration3 * 17 / 1000; 
  //시리얼모니터로 초음파 거리값을 확인 하는 코드
 

  Serial.print("\n Sensor1 (Distance): ");
  Serial.print(distance1); //측정된 물체로부터 거리값(cm값)
  Serial.println(" Cm");
  Serial.print(" Sensor2 (Distance): ");
  Serial.print(distance2); //측정된 물체로부터 거리값(cm값)
  Serial.println(" Cm");
  Serial.print(" Sensor3 (Distance): ");
  Serial.print(distance3); //측정된 물체로부터 거리값(cm값)
  Serial.println(" Cm");
  delay(500); //측정값보여주는 시간 간격


  
}
