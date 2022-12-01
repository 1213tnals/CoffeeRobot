char cmd;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    cmd = Serial.read();

    if(cmd=='S'){
      Serial.println("Arduino: Stop Motor");
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
    }
  }
}
