int fan0 = A0;
int fan1 = A1;
int button1 = 2;
int button2 = 3;

void setup() {
  pinMode(fan0, OUTPUT);
  pinMode(fan1, OUTPUT);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

}

void loop() {
  if (!digitalRead(button1)){
    digitalWrite(fan0, HIGH);
    digitalWrite(fan1, LOW);
  }
  if(!digitalRead(button2)){
    digitalWrite(fan0, LOW);
    digitalWrite(fan1, LOW);
  }

}
