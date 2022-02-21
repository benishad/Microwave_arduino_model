#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);                       //LCD 주소 값은 0x27이며 가로 16칸,세로 2칸 임
Servo myservo;                                            //서보모터 이름을 myservo 로 만듬

int bootButton = 2;                                       //각 소자별 아두이노 핀 번호
int cancelButton = 3;                                     // 
int firstButton = 4;                                      //
int secondButton = 5;                                     //
int thirdButton = 6;                                      //
int saftySwitch = 7;                                      //
int speaker = 8;                                          //
int modeButton = 11;

int fan0 = A0;

int firstCountTime = 5;                                   //firstCountTime을 5로 설정
int secondCountTime = 15;                                 //secondCountTime을 15로 설정
int thirdCountTime = 30;                                  //thirdCountTime을 30으로 설정

int stackCount = 0;                                       //stackCount를 0으로 이는 나중에 전자레인지 시간 표시에 사용됨

boolean saftyState = false;
boolean operateState = false;
boolean turnState = false;
boolean modeState = false;

void setup() {
  lcd.init();                                             //LCD 초기화
  lcd.backlight();                                        //LCD 배경밝기 설정
  myservo.attach(9);                                      //서보모터를 9번에 할당

  pinMode(cancelButton, INPUT_PULLUP);
  pinMode(firstButton, INPUT_PULLUP);
  pinMode(secondButton, INPUT_PULLUP);
  pinMode(thirdButton, INPUT_PULLUP);
  pinMode(bootButton, INPUT_PULLUP);
  pinMode(saftySwitch, INPUT_PULLUP);
  pinMode(modeButton, INPUT_PULLUP);
  pinMode(fan0, OUTPUT);
}

void loop() {
  input();          //입력 코드
  operate();        //작동 코드
  fanmode();
  lcdmode();
  delay(150);       //0.15초
}

//----------------------------------------------------------------------------------------

void input() {                          //입력 코드
  cancel();                                         //취소 코드
  first();                                          //5초 코드
  second();                                         //15초 코드
  third();                                          //30초 코드
  boot();                                           //시작 코드
  safty();                                          //문열림 코드
  modeset();
}

//------------------------------------------------------------------------------------------
                                                                                     //작동 코드
void operate() {
  if (((saftyState == true && operateState == true) && stackCount > 0) && modeState == false){              //리미트 스위치가 닫혀있고, operateState 신호가 오면 
    stackCount--;                                                                    //쌓여있던 시간을 1씩 감소하면서
    turnBase();                                                                      //서보모터 작동
    delay(850);                                                                      //0.85초씩 작동 loop에 0.15초를 더해서 1초씩 감소하게됨
    if (stackCount == 0)
      operateState = false;                                                          //시간이 0이되면 operateState를 0으로
  } else {
    stopBase();                                                                      //시간이 0이되면 작동중지
  }
}

//-------------------------------------------------------------------------------------------
                                                                        //LCD에 표현하는 코드
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
    lcd.print("M");
  } else {
    lcd.print(stackCountMinute);
  }

  lcd.print(" ");

  if (stackCountSecond < 10) {
    lcd.print("0");
    lcd.print(stackCountSecond);
    lcd.print("S");
  } else {
    lcd.print(stackCountSecond);
  }
}

//---------------------------------------------------------------------

void cancel() {                              //취소 코드
  if (!digitalRead(cancelButton)) {               //취소 버튼을 누르면 operateState를 0으로 만들고 stackCount를 0으로 만듬
    operateState = false;
    stackCount = 0;
  }
}

void first() {                                            //5초 코드
  if (!digitalRead(firstButton))                          //4번 버튼을 누르면 stackCount에 5를 더함
    stackCount += firstCountTime;
}

void second() {                                           //15초 코드
  if (!digitalRead(secondButton))                         //5번 버튼을 누르면 stackCount에 15를 더함
    stackCount += secondCountTime;
}

void third() {                                            //30초 코드
  if (!digitalRead(thirdButton))                          //6번 버튼을 누르면 stackCount에 30를 더함
    stackCount += thirdCountTime;
}
                                                      //시작 코드
void boot() {                                         //전자레인지를 작동하는 코드
  if (!digitalRead(bootButton))                       //시작 버튼을 누르면 operateState를 0으로 / 누를 때마다 값이 변함 0~1
    operateState = !operateState;
}
                                                      //문열림 코드
void safty() {                                        //리미트 스위치에 상태를 표현하는 코드
  if (!digitalRead(saftySwitch)) {                    //리미트 스위치를 LOW상태로 표현
    saftyState = false;                               //LOW이면 리미트스위치 상태를 0으로 전환
  }
  else{                                               //HIGH면 상태를 1로 전환
    saftyState = true;
  }
}

//---------------------------------------------------------------------------------------------------------------------

void modeset() {
  if (!digitalRead(modeButton))                         
    modeState = !modeState;
}

void fanmode() {
  if (((saftyState == true && operateState == true) && stackCount > 0) && modeState == true) {              //리미트 스위치가 닫혀있고, modeState 신호가 오면 
    stackCount--;                                                                    //쌓여있던 시간을 1씩 감소하면서
    fan();                                                                      //서보모터 작동
    delay(850);                                                                      //0.85초씩 작동 loop에 0.15초를 더해서 1초씩 감소하게됨
    if (stackCount == 0)
      operateState = false;                                                          //시간이 0이되면 operateState를 0으로
  } else {
    digitalWrite(fan0, LOW);                                                                      //시간이 0이되면 작동중지
  }
}

void lcdfanmode() {
  lcd.setCursor(0, 0);
  lcd.print("   Fan Mode!   ");

  lcd.setCursor(0, 1);
  lcd.print("Timer : ");

  int stackCountMinute = stackCount / 60;
  int stackCountSecond = stackCount % 60;

  if (stackCountMinute < 10) {
    lcd.print("0");
    lcd.print(stackCountMinute);
    lcd.print("M");
  } else {
    lcd.print(stackCountMinute);
  }

  lcd.print(" ");

  if (stackCountSecond < 10) {
    lcd.print("0");
    lcd.print(stackCountSecond);
    lcd.print("S");
  } else {
    lcd.print(stackCountSecond);
  }
}

//---------------------------------------------------------------------------------------------------------------------

void lcdmode() {
  if (modeState == false) {              //리미트 스위치가 닫혀있고, modeState 신호가 오면 
    lcdShow();                                                          //시간이 0이되면 operateState를 0으로
  } else if (modeState == true){
    lcdfanmode();                                                                      //시간이 0이되면 작동중지
  }
}

//----------------------------------------------------------------------------------------------------------------------

void turnBase() {
  myservo.write(83);                                  //서보모터 작동 83에 속도 (0에 가까울수록 빠르고 90에 가까울수록 느림)
  turnState = true;                                   //turnState을 1로
}

void stopBase() {
  myservo.write(90);                                  //서보모터 정지 (0에 가까울수록 빠르고 90에 가까울수록 느림)
  turnState = false;                                  //turnState을 0으로
}

void fan() {
  digitalWrite(fan0, HIGH);
}

void fanstop() {
  digitalWrite(fan0, LOW);
}

//------------------------------------------------------------------------------------------------------------------------

void buzzer() {
  tone(speaker,500,100);
  delay(100);
  tone(speaker,500,100);
  delay(100);
  tone(speaker,500,100);
  delay(100);
  tone(speaker,500,100);
  delay(1000);
  tone(speaker,500,100);
  delay(100);
  tone(speaker,500,100);
  delay(100);
  tone(speaker,500,100);
  delay(100);
  tone(speaker,500,100);
}
