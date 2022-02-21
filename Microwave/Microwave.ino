#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myservo;

int bootButton = 2;
int cancelButton = 3;
int firstButton = 4;
int secondButton = 5;
int thirdButton = 6;
int saftySwitch = 7;

int firstCountTime = 5;
int secondCountTime = 15;
int thirdCountTime = 30;

int stackCount = 0;

boolean saftyState = false;
boolean operateState = false;

void setup() {
  lcd.init();
  lcd.backlight();
  myservo.attach(9);

  pinMode(cancelButton, INPUT_PULLUP);
  pinMode(firstButton, INPUT_PULLUP);
  pinMode(secondButton, INPUT_PULLUP);
  pinMode(thirdButton, INPUT_PULLUP);
  pinMode(bootButton, INPUT_PULLUP);
  pinMode(saftySwitch, INPUT_PULLUP);
}

void loop() {
  input();
  operate();
  lcdShow();
  delay(500);
}

//--------------------------------------------------

void input() {
  cancel();
  first();
  second();
  third();
  boot();
  safty();
}

void operate() {
  if (saftyState == true && operateState == true) {
    stackCount--;
    turnBase();
    delay(500);
    if (stackCount == 0)
      operateState = false;
  } else {
    stopBase();
  }
}

void lcdShow() {
  lcd.setCursor(0, 0);
  lcd.print("Dual Microwave");

  lcd.setCursor(0, 1);
  lcd.print("Timer : ");

  int stackCountMinute = stackCount / 60;
  int stackCountSecond = stackCount % 60;

  if (stackCountMinute < 10) {
    lcd.print("0");
    lcd.print(stackCountMinute);
    lcd.print("M ");
  } else {
    lcd.print(stackCountMinute);
  }

  lcd.print(" ");

  if (stackCountSecond < 10) {
    lcd.print("0");
    lcd.print(stackCountSecond);
    lcd.print("S ");
  } else {
    lcd.print(stackCountSecond);
  }
}

//--------------------------------------------------

void cancel() {
  if (!digitalRead(cancelButton)) {
    operateState = false;
    stackCount = 0;
  }
}

void first() {
  if (!digitalRead(firstButton))
    stackCount += firstCountTime;
}

void second() {
  if (!digitalRead(secondButton))
    stackCount += secondCountTime;
}

void third() {
  if (!digitalRead(thirdButton))
    stackCount += thirdCountTime;
}

void boot() {
  if (!digitalRead(bootButton))
    operateState = !operateState;
}

void safty() {
  saftyState = digitalRead(saftySwitch);
}

//--------------------------------------------------

void turnBase() {
  myservo.write(85);
}

void stopBase() {
  myservo.write(90);
}
