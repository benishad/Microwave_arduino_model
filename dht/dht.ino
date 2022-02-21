
#include <LiquidCrystal_I2C.h>                                   //LCD 라이브러리
#include <DHT.h>                                                 //DHT 라이브러리 호출 ---- 온습도 센서
#define DHTPIN 12                                                // 온습도 센서가 12번에 연결 
#define DHTTYPE DHT11                                            // DHT11 온습도 센서 사용

DHT dht(DHTPIN, DHTTYPE);                                        //DHT 설정(12,DHT11)

LiquidCrystal_I2C lcd(0x27, 16,2);

void setup(){
  Serial.begin(9600);                                           //통신속도 9600으로 통신 시작
  lcd.init();                                                   //lcd초기화
  lcd.backlight();                                              //lcd 바탕을 밝게하기
  lcd.print("Start~");
  dht.begin();
}
 
void loop() {
  delay(1000);                                                  //전원 공급시 1초후 LCD 작동
  LCDdis();
}

void LCDdis(){
  float h = dht.readHumidity();                                   //습도값을 h에 저장
  float t = dht.readTemperature();                                //온도값을 t에 저장
  float f = dht.readTemperature(true);                            // 화씨 온도를 측정합니다.
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);
  Serial.print("Humidity: ");                                     //문자열 출력
  Serial.print(h);                                                //습도값 출력
  Serial.print("% ");
  Serial.print("Temperature: ");
  Serial.print(hic);                                              //온도값 출력
  Serial.println("C");
  lcd.setCursor(0,0);
  lcd.print("Humidity");
  lcd.setCursor(9,0);
  lcd.print(h);
  lcd.setCursor(15,0);
  lcd.print("%");
  lcd.setCursor(0,1);
  lcd.print("Temp");
  lcd.setCursor(5,1);
  lcd.print(hic);
  lcd.setCursor(9,1);
  lcd.print(" C");
  delay(500);
}
/*
여기는 lcd의 주소를 알아보는 코드
#include <Wire.h>

void setup()
{
  Wire.begin();

  Serial.begin(9600);
  while (!Serial);           
  Serial.println("\nI2C Scanner");
}

void loop()
{
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknow error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }

  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
  delay(5000);
}
*/
