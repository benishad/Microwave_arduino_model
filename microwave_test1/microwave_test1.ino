#include <Servo.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16,2);
Servo servomotor;

int button1 = 2;  //cancel
int button2 = 3;  //5sec
int button3 = 4;  //15sec

int countvalue = 0;
int ButtonCount = 0;
int state = 0;

void setup(){
  lcd.init();                                                   //lcd초기화
  lcd.backlight();                                              //백라이트
  lcd.print("Start~");
  servomotor.attach(9);                                         //서보모터 9번핀

  pinMode(button1, INPUT);  //cancel
  pinMode(button2, INPUT);  //5sec
  pinMode(button3, INPUT);  //15sec
}

void loop(){
  countvalue = digitalRead(button2);
  if(countvalue == HIGH){
    if(state == 0){
      delay(10);
      state = 1;
    }
  }
  if(countvalue == LOW){
    if(state ==1){
      ButtonCount += 1;
      delay(10);
      state = 0;
    }
  }
  if{
    
  }
  if(ButtonCount = 1){
    lcd.setCursor(0,1);
    lcd.print("push");
    lcd.setCursor(5,1);
    lcd.print(ButtonCount);
    delay(10);
  }
}
