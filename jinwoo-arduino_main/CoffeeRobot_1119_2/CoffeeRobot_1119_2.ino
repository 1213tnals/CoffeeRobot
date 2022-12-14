#define DIR1 2
#define PWM1 3
#define DIR2 4
#define PWM2 5     //Driving motors

#define IR_R 32
#define IR_L 33 //Ir sensor right & left
   

#define TRIG_F 24
#define ECHO_F 26
#define TRIG_R 25
#define ECHO_R 27


int M1_pinNum_1 = 28;
int M1_pinNum_2 = 29;
int M1_SpeedPin = 6;

int M2_pinNum_1 = 30;
int M2_pinNum_2 = 31;
int M2_SpeedPin = 7;


char cmd;
int ir_R;
int ir_L;    // ir sensor read

int Kp = 100;
int Kd = 0;
int Ki = 0;
//gain values

long e,e_dot,int_e,pree;
float pretime,dt;


long duration1;
long length1 = 0;
long duration2;
long length2;

int flag =0;

void one_shot();
void two_shot();
void Half_shot();

void setup() {

  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  
  pinMode(DIR1, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(DIR2, OUTPUT);
  pinMode(PWM2, OUTPUT);

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
          
     if(cmd=='S'){
      Serial.println("Stop Motor");
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      flag++;
    }

    if(cmd=='O'){
      //Serial.println("User Finded");
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
    }
      
    if(cmd=='A'){
      //Serial.println("You order Half shot");
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      Serial.println("5");
      delay(5000);     //assume: after 5sec, user has gone
      Serial.println("9");
      flag=0;
    }
      
    if(cmd=='B'){
      //Serial.println("You order One shot");
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      Serial.println("5");
      delay(5000);     //assume: after 5sec, user has gone
      Serial.println("9");
      flag=0;
    }
      
    if(cmd=='C'){
      //Serial.println("You order Two shot");
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);     //assume: after 5sec, making finish
      Serial.println("5");
      delay(5000);     //assume: after 5sec, user has gone
      Serial.println("9");
      flag=0;
    }
  }// hand sign -->motor stop
    //s-> stop ??????
    //o-> order
    //A,B,C->?????? ?????? ??????
    //5-> ???????????? ?????? ?????? ?????????/9-> ????????? ????????? ???????????? ??? ?????????

    
   
  if(flag==0 && cmd!='S') {

    ir_R = digitalRead(IR_R);  
    ir_L = digitalRead(IR_L);  
    //Serial.println(ir_R);
    //Serial.println(ir_L);    

    if (ir_R == 1 && ir_L == 0) {         //rotate_R
      digitalWrite(DIR1, HIGH);
      analogWrite(PWM1, 30);
      digitalWrite(DIR2,LOW);
      analogWrite(PWM2, 30);
    }
    else if (ir_R == 0 && ir_L == 1) {    //rotate_L
      digitalWrite(DIR1, LOW);
      analogWrite(PWM1, 30);
      digitalWrite(DIR2, HIGH);
      analogWrite(PWM2, 30);
    }
    else {                                //straight
      digitalWrite(DIR1, HIGH);
      analogWrite(PWM1, 30);
      digitalWrite(DIR2, HIGH);
      analogWrite(PWM2, 30);
    }

    //Serial.print(ir_R);
    //Serial.print(' ');
    //Serial.println(ir_L);  }
  // line tracking driving code

  
  for(int i=0;i<5;i++)
  {
      digitalWrite(TRIG_F, LOW);
      delayMicroseconds(2);
      digitalWrite(TRIG_F, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIG_F, LOW);
    
      duration1 = pulseIn(ECHO_F, HIGH);
      Serial.println(duration1);
      length1 = length1 + duration1;
      
  }// US sensor reading




  if(cmd=='A')
  {    
    Half_shot();
    cmd='Z';
  }
   else if(cmd=='B')
  {
    one_shot();
    cmd='Z';
  }
  else if(cmd=='C')
  {
    two_shot();
    cmd='Z';
  }
  else
  {
    digitalWrite(M1_pinNum_1, LOW);
    digitalWrite(M1_pinNum_2, LOW);
    analogWrite(M1_SpeedPin, 0);
    digitalWrite(M2_pinNum_1, LOW);
    digitalWrite(M2_pinNum_2, LOW);
    analogWrite(M2_SpeedPin, 0);
    delay(100);
  }



  length1 = length1 / 5;
  length1 = length1 * 17/1000;
  if(length1 < 5 && flag==0 && cmd!='S' ){
    avoid_algorithmn();
  }

  //Serial.println(length1); 

  delay(100);// if sensor value <1000, start avoid algorithm 
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
    Serial.print(length2);

    //90??? ???????????? ?????? ???????????? ??????
     digitalWrite(DIR1, HIGH);
     analogWrite(PWM1, 30);
     digitalWrite(DIR2,LOW);
     analogWrite(PWM2, 30);

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

    digitalWrite(PWM1,Kp*e+Kd*e_dot+Ki*int_e);
    digitalWrite(PWM2,200);
  }
}// length2 can't convet to cm



//linetracking + sonic sensor done



void Half_shot()
{
    digitalWrite(M1_pinNum_1, LOW);
    digitalWrite(M1_pinNum_2, HIGH);
    analogWrite(M1_SpeedPin, 200);
    delay(100);//1??? ????????? ?????? ?????? ?????????
    digitalWrite(M2_pinNum_1, LOW);
    digitalWrite(M2_pinNum_2, HIGH);
    analogWrite(M2_SpeedPin, 200);
    delay(100);//2??? ????????? ?????? ?????? ?????????  
}


void one_shot()
{
    digitalWrite(M1_pinNum_1, LOW);
    digitalWrite(M1_pinNum_2, HIGH);
    analogWrite(M1_SpeedPin, 200);
    delay(100);//1??? ????????? ?????? ?????? ?????????
    digitalWrite(M2_pinNum_1, LOW);
    digitalWrite(M2_pinNum_2, HIGH);
    analogWrite(M2_SpeedPin, 200);
    delay(100);//2??? ????????? ?????? ?????? ?????????
}


void two_shot()
{
    digitalWrite(M1_pinNum_1, LOW);
    digitalWrite(M1_pinNum_2, HIGH);
    analogWrite(M1_SpeedPin, 200);
    delay(100);//1??? ????????? ?????? ?????? ?????????
    digitalWrite(M2_pinNum_1, LOW);
    digitalWrite(M2_pinNum_2, HIGH);
    analogWrite(M2_SpeedPin, 200);
    delay(100);//2??? ????????? ?????? ?????? ?????????
} 
