int M1_pinNum_1 = 2;
int M1_pinNum_2 = 3;
int M1_SpeedPin = 10;

int M2_pinNum_1 = 4;
int M2_pinNum_2 = 5;
int M2_SpeedPin = 11;


char App_order;

void one_shot();
void two_shot();
void Half_shot();


void setup() {
  // put your setup code here, to run once:
  pinMode(M1_pinNum_1, OUTPUT);
  pinMode(M1_pinNum_2, OUTPUT);
  pinMode(M1_SpeedPin, OUTPUT);  
  
  pinMode(M2_pinNum_1, OUTPUT);
  pinMode(M2_pinNum_2, OUTPUT);
  pinMode(M2_SpeedPin, OUTPUT);  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(strcmp(cmd=='A')
  
  {    
    Half_shot();
    App_order="sleep";

  }
   else if(strcmp(cmd=='B')
   {
    one_shot();
    App_order="sleep";

  }
   else if(cmd=='C')
   {
    two_shot();
    App_order="sleep";
   }
  else
    digitalWrite(M1_pinNum_1, LOW);
    digitalWrite(M1_pinNum_2, LOW);
    analogWrite(M1_SpeedPin, 0);
    digitalWrite(M2_pinNum_1, LOW);
    digitalWrite(M2_pinNum_2, LOW);
    analogWrite(M2_SpeedPin, 0);
    delay(100);

}


void Half_shot()
{
    digitalWrite(M1_pinNum_1, LOW);
    digitalWrite(M1_pinNum_2, HIGH);
    analogWrite(M1_SpeedPin, 200);
    delay(100);//1번 모터의 작동 시간 조절부
    digitalWrite(M2_pinNum_1, LOW);
    digitalWrite(M2_pinNum_2, HIGH);
    analogWrite(M2_SpeedPin, 200);
    delay(100);//2번 모터의 작동 시간 조절부

  
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

  
}
