#define IN1 5
#define IN2 6
#define IN3 9
#define IN4 10

#define IR_R 2
#define IR_L 4

#define US_F 

void setup() {

  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);

  pinMode(IR_R,INPUT);
  pinMode(IR_L,INPUT);

  Serial.begin(9600);

}

void loop() {

  int a = digitalRead(IR_R);
  int b = digitalRead(IR_L);

  if (a == 1 && b == 0){
    analogWrite(IN2,0);
    analogWrite(IN3,0);
  }
  else if (a == 0 && b == 1){
    analogWrite(IN2,0);
    analogWrite(IN3,0);
  }
  else{
    analogWrite(IN4,255);
    analogWrite(IN3,0);
  }

  Serial.print(a);
  Serial.print(' ');
  Serial.println(b);

}
