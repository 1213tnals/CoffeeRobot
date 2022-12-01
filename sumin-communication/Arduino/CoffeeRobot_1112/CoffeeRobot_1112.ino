#define IN1 11
#define IN2 12
#define IN3 9
#define IN4 10     //for line tracking

#define IR_R 2
#define IR_L 4  //Ir sensor right & left
   

#define TRIG_F 3
#define ECHO_F 4
#define TRIG_R 5
#define ECHO_R 6
#define speed1 7
#define speed2 8   //motor control values

int cmd;
int ir_R;
int ir_L;    // ir sensor read

int Kp = 100;
int Kd = 0;
int Ki = 0;
//gain values

long e,e_dot,int_e;
float pretime,dt;


long duration1;
long length1;
long duration2;
long length2;

void setup() {

  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(IR_R, INPUT);
  pinMode(IR_L, INPUT);

  pinMode(TRIG_F,OUTPUT);
  pinMode(ECHO_F,INPUT);
  pinMode(TRIG_R,OUTPUT);
  pinMode(ECHO_R,INPUT);
}

void loop() {
  if (Serial.available()) {      
    cmd = Serial.read();
    if (cmd == 'S') {
      Serial.println("Arduino: Stop Motor");
      digitalWrite(LED_BUILTIN, HIGH);
      delay(5000);
      digitalWrite(LED_BUILTIN, LOW);
      delay(5000);
    }// hand sign -->motor stop
    
    else {

      ir_R = digitalRead(IR_R);
      ir_L = digitalRead(IR_L);

      if (ir_R == 1 && ir_L == 0) {         //rotate_R
        analogWrite(IN2, 0);
        analogWrite(IN3, 0);
      }
      else if (ir_R == 0 && ir_L == 1) {    //rotate_L
        analogWrite(IN2, 0);
        analogWrite(IN3, 0);
      }
      else {                                //straight
        analogWrite(IN4, 255);
        analogWrite(IN3, 0);
      }

      Serial.print(ir_R);
      Serial.print(' ');
      Serial.println(ir_L);
    }
  }// line tracking driving code

  
  for(int i=0;i<5;i++){
      digitalWrite(TRIG_F, LOW);
      delayMicroseconds(2);
      digitalWrite(TRIG_F, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIG_F, LOW);
    
      duration1 = pulseIn (ECHO_F, HIGH);

      length1 = length1 + duration1;
    }// US sensor reading

  length1 = length1 / 5;
  length1 = length1 * 17/1000;
    if(length1 < 5){
      avoid_algorithmn();
    }

    Serial.println(length); 

    delay(100);
    length = 0;// if sensor value <1000, start avoid algorithm
    
}


// avoid algorithm
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
}// length2 can't convet to cm


//linetracking + sonic sensor done
