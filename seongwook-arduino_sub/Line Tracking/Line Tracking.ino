#define IN1 5
#define IN2 6
#define IN3 9
#define IN4 10

#define IR_R 2
#define IR_L 4
#define IR_F 3

#define US_F 

bool flag = 0;

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
  int c = digitalRead(IR_F);

  if (a == 0 && b == 0 && c == 1){
    analogWrite(IN2,255);
    analogWrite(IN3,255);  //앞으로 가는 코드
  }
  else if (a == 0 && b == 1 && c == 0){
    analogWrite(IN2,255);
    analogWrite(IN3,0);   //왼쪽 적외선 센서 닿았을 때
    flag = 0;
  }
  else if (a == 1 && b == 0 && c == 0){
    analogWrite(IN2,0);
    analogWrite(IN3,255);   //오른쪽 적외선 센서 닿았을 때
    flag = 1;
  }
  else{
    if (flag == 0){
      analogWrite(IN2,255);
      analogWrite(IN3,0); // 오른쪽으로 가는 코드
    }
    else if (flag == 1){
      analogWrite(IN2,0);
      analogWrite(IN3,255); // 왼쪽으로 가는 코드
    }
  }

  Serial.print(a);
  Serial.print(' ');
  Serial.println(b);
  Serial.print(' ');
  Serial.println(c);

}
