/////////////////////////////////////
// about water pump pin
#define M1_pinNum_1  2
#define M1_pinNum_2  3
#define M1_SpeedPin  4

#define M2_pinNum_1  5
#define M2_pinNum_2  6
#define M2_SpeedPin  7

/////////////////////////////////////////
//about cup US sensor pin
#define TRIG_cup 8
#define ECHO_cup 9
/////////////////////////////////////
#define PELT 10
#define ONE_WIRE_BUS A0
#define des_temp 19 //핀아님
///////////////////////////////////////
#define UNO_TX 51
#define UNO_RX 50

// serial communication with MEGA
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Stepper.h>


OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


int order_data = 3;
bool water_pump_flag = 0;
bool cup_sensor_flag = 0;



SoftwareSerial A_Serial(UNO_TX,UNO_RX);
/////////////////////////////////////
 // about peltier


float temp = 0;
bool peltier_on = 0;
/////////////////////////////////////////////
//about cup dispenser
const int stepsPerRevolution = 36;  

Stepper myStepper(stepsPerRevolution, 22, 23, 24, 25); //사용한 PIN번호 수정해야함!!!!!!!!!!!!!!!!!!!!!!!!!!
/////////////////////////////////////////////

void setup() {
  //////////////////////////////////////////
  //about cup US sensor pin
  pinMode(TRIG_cup, OUTPUT);
  pinMode(ECHO_cup, INPUT); 
  //////////////////////////////////////////
  // about water pump pin
  pinMode(M1_pinNum_1, OUTPUT);
  pinMode(M1_pinNum_2, OUTPUT);
  pinMode(M1_SpeedPin, OUTPUT);

  pinMode(M2_pinNum_1, OUTPUT);
  pinMode(M2_pinNum_2, OUTPUT);
  pinMode(M2_SpeedPin, OUTPUT); 
  //////////////////////////////////////////
  // about peltier
  sensors.begin();
  pinMode(PELT,OUTPUT); 
  //////////////////////////////////////////



}

void loop() {
  /////////////////////////////////////////////
  //cup dist reading 인식 안될 때 최대 거리를 15cm정도로 설정
  float cup_dist =15;
  cup_dist=cup_CM();

  ////////////////////////////////////////////////
  //cup이 떨어지면 flag=1로

  if(cup_dist <= 8){
    cup_sensor_flag =1;
  }

  //cup이 떨어지고 난 뒤 cup이 멀어지면 flag=2로
  if(cup_sensor_flag ==1 && cup_dist > 8){
    cup_sensor_flag =2;
  }

  ////////////////////////////////////////////////
  //about peltier
  sensors.requestTemperatures();
  temp = sensors.getTempCByIndex(0);

  if(temp >= 20){
    peltier_on = 1;
  }
  else{
    peltier_on = 0;
  }

  while(peltier_on == 1)
  {
    sensors.requestTemperatures();
    temp = sensors.getTempCByIndex(0);
    
    digitalWrite(PELT,HIGH);
    
    if(temp<=des_temp){
      peltier_on = 0;      
    }
  }
  digitalWrite(PELT,LOW); 
  ///////////////////////////////////////////////////////////////
  // cmd receive -> S,O,A,B,C->pump control
  if (Serial.available()){
    order_data = A_Serial.read();
  }//serial available close

  if(order_data == 0 ){
    cup_dispenser();
    if( cup_sensor_flag ==1){
    
    half_shot();
    order_data=0;
    water_pump_flag = 0;
    }//다시 초기화
  }//order data =0 일 때 워터펌프 작동시키고, 작동완료 되면 신호 보내주기
  //half_shot(); 안에 써있음

    else if(order_data == 1 ){
      cup_dispenser();
      if( cup_sensor_flag ==1){
    one_shot();
    order_data=0;
    water_pump_flag = 0;
    } //다시 초기화
    }//order data =1 일 때 워터펌프 작동시키고, 작동완료 되면 신호 보내주기
  //one_shot(); 안에 써있음
    

  else if(order_data == 2){
    cup_dispenser();
    if( cup_sensor_flag ==1){
    two_shot();
    
    order_data=0;
    water_pump_flag = 0;
    }  //다시 초기화
  }//order data =2 일 때 워터펌프 작동시키고, 작동완료 되면 신호 보내주기
  //two_shot(); 안에 써있음

  if(cup_sensor_flag == 2){
    A_Serial.println(cup_sensor_flag);

    cup_sensor_flag=0;
  }

}//loop close



//////////////////////////////////////////////////////////
//water pump functions
void half_shot()
{
    digitalWrite(M1_pinNum_1, LOW);
    digitalWrite(M1_pinNum_2, HIGH);
    analogWrite(M1_SpeedPin, 200);
    delay(100);//1번 모터의 작동 시간 조절부
    digitalWrite(M2_pinNum_1, LOW);
    digitalWrite(M2_pinNum_2, HIGH);
    analogWrite(M2_SpeedPin, 200);
    delay(100);//2번 모터의 작동 시간 조절부

    water_pump_flag = 1;
    A_Serial.println(water_pump_flag);
}


void one_shot()
{
    digitalWrite(M1_pinNum_1, LOW);
    digitalWrite(M1_pinNum_2, HIGH);
    analogWrite(M1_SpeedPin, 200);
    delay(100);//1번 모터의 작동 시간 조절부
    digitalWrite(M2_pinNum_1, LOW);
    digitalWrite(M2_pinNum_2, HIGH);
    analogWrite(M2_SpeedPin, 200);
    delay(100);//2번 모터의 작동 시간 조절부

    water_pump_flag = 1;
    A_Serial.println(water_pump_flag);
}


void two_shot()
{
    digitalWrite(M1_pinNum_1, LOW);
    digitalWrite(M1_pinNum_2, HIGH);
    analogWrite(M1_SpeedPin, 200);
    delay(100);//1번 모터의 작동 시간 조절부
    digitalWrite(M2_pinNum_1, LOW);
    digitalWrite(M2_pinNum_2, HIGH);
    analogWrite(M2_SpeedPin, 200);
    delay(100);//2번 모터의 작동 시간 조절부

    water_pump_flag = 1;
    A_Serial.println(water_pump_flag); 
}

//////////////////////////////////////////////////////////
//cup sensor reading
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
//////////////////////////////////////////////////////////
void cup_dispenser(){
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

