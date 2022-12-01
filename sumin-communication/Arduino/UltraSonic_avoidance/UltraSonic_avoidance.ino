#define TRIG_F 2
#define ECHO_F 3
#define TRIG_R 4
#define ECHO_R 5
#define IR_R 10
#define speed1 6
#define speed2 7

//1000

int Kp = 100;
int Kd = 0;
int Ki = 0;

long e,e_dot,int_e;
float pretime,dt;

long duration1;
long length1;
long duration2;
long length2;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_F,OUTPUT);
  pinMode(ECHO_F,INPUT);
  pinMode(TRIG_R,OUTPUT);
  pinMode(ECHO_R,INPUT);
}

void loop() {

  for(int i=0;i<5;i++){
    digitalWrite(TRIG_F, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_F, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_F, LOW);
    
    duration1 = pulseIn (ECHO_F, HIGH);

    length1 = length1 + duration1;
  }

  length1 = length1 / 5;

  if(length1 < 1000){
    avoid_algorithmn();
  }

  Serial.println(length); 

  delay(100);
  length = 0;
}



void avoid_algorithmn()
{
  while(length2 < 1000){

    for(int i=0;i<5;i++){
      digitalWrite(TRIG_R, LOW);
      delayMicroseconds(2);
      digitalWrite(TRIG_R, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIG_R, LOW);
      
      duration2 = pulseIn (ECHO_R, HIGH);
      
      length2 = length2 + duration1;
    }
    length2 = length2 / 5;

    //90도 왼쪽으로 도는 알고리즘 추가

  }

  while(1 == digitalRead(IR_R)){

    for(int i=0;i<5;i++){
      digitalWrite(TRIG_R, LOW);
      delayMicroseconds(2);
      digitalWrite(TRIG_R, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIG_R, LOW);
      
      duration2 = pulseIn (ECHO_R, HIGH);
      
      length2 = length2 + duration1;
    }
    length2 = length2 / 5;

    dt = (micros() - pretime)/1000000;
    pretime = micros();

    e = 1000 - length2;
    int_e += e*dt;
    e_dot = (e-pree)/dt;
    pree = e;

    digitalWrite(speed1,Kp*e+Kd*e_dot+Ki*int_e);
    digitalWrite(speed2,200);
  }
}
