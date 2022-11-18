#define TRIG1 9 //TRIG 핀 설정 (초음파 보내는 핀)
#define ECHO1 8 //ECHO 핀 설정 (초음파 받는 핀)
#define TRIG2 7 //TRIG 핀 설정 (초음파 보내는 핀)
#define ECHO2 6 //ECHO 핀 설정 (초음파 받는 핀)

void setup() {
  Serial.begin(9600); //시리얼모니터로 센서값을 확인하기위해서 시리얼 통신을 정의                    
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);  
}

void loop(){

  unsigned long duration1, duration2;

  //초음파 센서 1
  digitalWrite(TRIG1, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG1, LOW);
  duration1 = pulseIn (ECHO1, HIGH); //물체에 반사되어돌아온 초음파의 시간을 변수에 저장
  
  //초음파 센서 2
  digitalWrite(TRIG2, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG2, LOW);
  duration2 = pulseIn (ECHO2, HIGH); //물체에 반사되어돌아온 초음파의 시간을 변수에 저장
  
  //34000*초음파가 물체로 부터 반사되어 돌아오는시간 /1000000 / 2(왕복값이아니라 편도값이기때문에 나누기2를 해줍니다.)
  //초음파센서의 거리값이 위 계산값과 동일하게 Cm로 환산되는 계산공식

  float distance1 = duration1 * 17 / 1000; 
  float distance2 = duration2 * 17 / 1000; 
  //시리얼모니터로 초음파 거리값을 확인 하는 코드
 

  Serial.print("\n Sensor1 (Distance): ");
  Serial.print(distance1); //측정된 물체로부터 거리값(cm값)
  Serial.println(" Cm");
  Serial.print(" Sensor2 (Distance): ");
  Serial.print(distance2); //측정된 물체로부터 거리값(cm값)
  Serial.println(" Cm");
  delay(500); //측정값보여주는 시간 간격

  if (distance1 <5) // 거리가 5cm 이하일때 작동 정지.
  exit(0);

  if (distance2 <5) // 거리가 5cm 이하일때 작동 정지.
  exit(0);
  
}
