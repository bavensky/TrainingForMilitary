#include <DHT22.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <SD.h>
#include "RTClib.h"

/********DS1307**********/
RTC_DS1307 rtc;  //PIN SDA = A4 ,SCL = A5
/******DHT22 Sensor*****/
#define DHT22_PIN 7
DHT22 myDHT22(DHT22_PIN);
/********SDCARD**********/
File myFile;
const int chipSelect = 4;
/******Output*********/
int bluePin = 3;  //Blue 
int greenPin = 9; //Green
int redPin = 10;  //Red
int Buzzer = 8;  //Sound 
int i1 = 0;  int i2 = 0;  int i3 = 0;  int i4 = 0;  int i5 = 0; int i6 =0;
/*********LCD16x*********************/
LiquidCrystal lcd(5, 6, A3, A2, A1, A0);

byte time = 1000;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  if (! rtc.isrunning()) 
  {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }
  startsdcard();
  setColor(0, 0, 0);
  pinMode(bluePin, OUTPUT); pinMode(greenPin, OUTPUT); pinMode(redPin, OUTPUT); pinMode(Buzzer, OUTPUT); 
  pinMode(4, OUTPUT); 
  lcd.begin(16, 4);lcd.print("  CMTC Temp & Humi ");delay(2000);  
  int temp = myDHT22.getTemperatureC();  //อุณหภูมิ
  int humi = myDHT22.getHumidity();      //ความชื้น
  lcd.setCursor(2, 1);lcd.print("T:");lcd.print(temp);lcd.print(" C ");
  lcd.setCursor(9, 1);lcd.print("H:");lcd.print(humi);lcd.print(" %RH ");
  timer();
}

void loop()
{
  Check();
  /******************************Show***************************************/
 /* int temp = myDHT22.getTemperatureC();  //อุณหภูมิ
  int humi = myDHT22.getHumidity();      //ความชื้น
  lcd.setCursor(2, 1);lcd.print("T:");lcd.print(temp);lcd.print(" C ");
  lcd.setCursor(9, 1);lcd.print("H:");lcd.print(humi);lcd.print(" %RH ");
  timer();
  //Serial.print(temp);Serial.print("    ");Serial.println(humi);
  
  
  /***************CheckHumi***************************************************/
  int temp = myDHT22.getTemperatureC();  //อุณหภูมิ
  int humi = myDHT22.getHumidity();      //ความชื้น
  //สีขาว เขียว เหลือง แดง น้ำเงิน
  if(humi <= 55 && humi <= 60 )
  //setColor(red, green, blue)
  {colorwhite();displaypink();i1++;  
  while(i1 == 1){savewhite();buzzer();i1 = 2;} 
  if(i1 >= 2 ){i1 = 2;i2=0; i3=0; i4=0; i5=0;}}

  if(humi >= 61 && humi <= 65 )
  //setColor(red, green, blue)
  {colorgreen();displaygreen();i2++; 
  while(i2 == 1){savegreen();buzzer();i2 = 2;}    
  if(i2 >= 2 ){i1 = 0;i2=2; i3=0; i4=0; i5=0;}}  
  
  if(humi >= 66 && humi <= 70 )
  //setColor(red, green, blue)
  {colororange();displayorange();i3++;
  while(i3 == 1){saveorange();buzzer();i3 = 2;}    
  if(i3 >= 2 ){i1 = 0;i2=0; i3=2; i4=0; i5=0;}}  
  
  if(humi >= 71 && humi <= 75 )
  //setColor(red, green, blue)
  {colorred();displayred();i4++;
  while(i4 == 1){savered();buzzer();i4 = 2;}    
  if(i4 >= 2 ){i1 = 0;i2=0; i3=0; i4=2; i5=0;}}       
  
  if(humi >= 76 )
  //setColor(red, green, blue)
  {colorblue();displayblue();i5++;
  while(i5 == 1){saveblue();buzzer();i5 = 2;}    
  if(i5 >= 2 ){i1 = 0;i2=0; i3=0; i4=0; i5=2;}} 
  /*
  if(humi == 0)
  {lcd.clear();lcd.print("Sensor Error");i6++;
  while(i6 == 1){saveblue();buzzer();i5 = 2;}    
  if(i6 >= 2 ){i1 = 0;i2=0; i3=0; i4=0; i5=0; i6=2;}} 
 */
  delay(time);
}

void Check()
{
   DHT22_ERROR_t errorCode;
   errorCode = myDHT22.readData();
      char buf[128];
      sprintf(buf, "Temperature %hi.%01hi C, Humidity %i.%01i %% RH",
                   myDHT22.getTemperatureCInt()/10, abs(myDHT22.getTemperatureCInt()%10),
                   myDHT22.getHumidityInt()/10, myDHT22.getHumidityInt()%10);
   //Serial.println(buf);
}
void timer()
{
  DateTime now = rtc.now();
  int hour1 = now.hour();int minute1 = now.minute();int second1 = now.second();
  lcd.setCursor(7, 2);lcd.print("Time: ");lcd.print(now.hour(), DEC);lcd.print(':');lcd.print(now.minute(), DEC);lcd.print(':');lcd.print(now.second(), DEC);
  /*lcd.print(' ');
  lcd.print(now.day(), DEC);
  lcd.print('/');
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.year(), DEC);*/
  DateTime future (now.unixtime() + 7 * 86400L + 30);
}
void setColor(int red, int green, int blue)
{
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}

void colorwhite()
{setColor (255,   0, 128);}
void colorgreen()
{setColor (  0, 255,   0);}
void colororange()
{setColor (255, 128,   0);}
void colorred()
{setColor (255,   0,   0);}
void colorblue()
{setColor (  0,   0, 240);}

void buzzer()
{ 
  int fivems = 1500;  int twoms = 500;
  digitalWrite(Buzzer, HIGH);delay(fivems);digitalWrite(Buzzer, LOW);delay(twoms);
  digitalWrite(Buzzer, HIGH);delay(fivems);digitalWrite(Buzzer, LOW);delay(twoms);
  digitalWrite(Buzzer, HIGH);delay(fivems);digitalWrite(Buzzer, LOW);
}
/***************SaveSDCARD***************************************************/
void startsdcard()
{
  /**************CheckSDCARD***********************************/ 
  if (!SD.begin(chipSelect)) 
  {
    return;
  }
  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {
    myFile.println("Data Temp & %RH for Militaty Training");
    myFile.print("Data / Time");
    myFile.print("                                 ");myFile.print("Temperaturemy");myFile.print("              ");myFile.print("Humidity");myFile.print("                  ");myFile.println("Status");  
    // close the file:
    myFile.close();}
}
void savewhite()
{
  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) 
  {
    DateTime now = rtc.now();
    int hour1 = now.hour();int minute1 = now.minute();int second1 = now.second();int day1 = now.day();int month1 = now.month();int year1 = now.year();
    myFile.print(day1);myFile.print('/');myFile.print(month1);myFile.print('/');myFile.print(year1);myFile.print(" & ");
    myFile.print(hour1);myFile.print(':');myFile.print(minute1);myFile.print(':');myFile.print(second1);myFile.print(" minute");myFile.print("    ");
    DateTime future (now.unixtime() + 7 * 86400L + 30);
    
    int temp = myDHT22.getTemperatureC();int humi = myDHT22.getHumidity();
    myFile.print("Temperaturemy : ");myFile.print(temp);myFile.print("C "); myFile.print("  ");
    myFile.print("Humidity : ");myFile.print(humi);myFile.print("%RH ");myFile.print("  ");
    myFile.println("Status : Pink  "); 
    myFile.close();
  }
} 
void savegreen()
{
  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) 
  {
    DateTime now = rtc.now();
    int hour1 = now.hour();int minute1 = now.minute();int second1 = now.second();int day1 = now.day();int month1 = now.month();int year1 = now.year();
    myFile.print(day1);myFile.print('/');myFile.print(month1);myFile.print('/');myFile.print(year1);myFile.print(" & ");
    myFile.print(hour1);myFile.print(':');myFile.print(minute1);myFile.print(':');myFile.print(second1);myFile.print(" minute");myFile.print("    ");
    DateTime future (now.unixtime() + 7 * 86400L + 30);
    
    int temp = myDHT22.getTemperatureC();int humi = myDHT22.getHumidity();
    myFile.print("Temperaturemy : ");myFile.print(temp);myFile.print("C "); myFile.print("  ");
    myFile.print("Humidity : ");myFile.print(humi);myFile.print("%RH ");myFile.print("  ");
    myFile.println("Status : GREEN  ");
    myFile.close();
  }
} 
void saveorange()
{
  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) 
  {
    DateTime now = rtc.now();
    int hour1 = now.hour();int minute1 = now.minute();int second1 = now.second();int day1 = now.day();int month1 = now.month();int year1 = now.year();
    myFile.print(day1);myFile.print('/');myFile.print(month1);myFile.print('/');myFile.print(year1);myFile.print(" & ");
    myFile.print(hour1);myFile.print(':');myFile.print(minute1);myFile.print(':');myFile.print(second1);myFile.print(" minute");myFile.print("    ");
    DateTime future (now.unixtime() + 7 * 86400L + 30);
    
    int temp = myDHT22.getTemperatureC();int humi = myDHT22.getHumidity();
    myFile.print("Temperaturemy : ");myFile.print(temp);myFile.print("C "); myFile.print("  ");
    myFile.print("Humidity : ");myFile.print(humi);myFile.print("%RH ");myFile.print("  ");
    myFile.println("Status : ORANGE  ");
    myFile.close();
  }
} 
void savered()
{
  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) 
  {
    DateTime now = rtc.now();
    int hour1 = now.hour();int minute1 = now.minute();int second1 = now.second();int day1 = now.day();int month1 = now.month();int year1 = now.year();
    myFile.print(day1);myFile.print('/');myFile.print(month1);myFile.print('/');myFile.print(year1);myFile.print(" & ");
    myFile.print(hour1);myFile.print(':');myFile.print(minute1);myFile.print(':');myFile.print(second1);myFile.print(" minute");myFile.print("    ");;
    DateTime future (now.unixtime() + 7 * 86400L + 30);
    
    int temp = myDHT22.getTemperatureC();int humi = myDHT22.getHumidity();
    myFile.print("Temperaturemy : ");myFile.print(temp);myFile.print("C "); myFile.print("  ");
    myFile.print("Humidity : ");myFile.print(humi);myFile.print("%RH ");myFile.print("  ");
    myFile.println("Status : RED  ");
    myFile.close();
    
  }
}
void saveblue()
{
  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) 
  {
    DateTime now = rtc.now();
    int hour1 = now.hour();int minute1 = now.minute();int second1 = now.second();int day1 = now.day();int month1 = now.month();int year1 = now.year();
    myFile.print(day1);myFile.print('/');myFile.print(month1);myFile.print('/');myFile.print(year1);myFile.print(" & ");
    myFile.print(hour1);myFile.print(':');myFile.print(minute1);myFile.print(':');myFile.print(second1);myFile.print(" minute");myFile.print("    ");
    DateTime future (now.unixtime() + 7 * 86400L + 30);
    
    int temp = myDHT22.getTemperatureC();int humi = myDHT22.getHumidity();
    myFile.print("Temperaturemy : ");myFile.print(temp);myFile.print("C "); myFile.print("  ");
    myFile.print("Humidity : ");myFile.print(humi);myFile.print("%RH ");myFile.print("  ");
    myFile.println("Status : BLUE  ");
    myFile.close();
  }
} 

void displaypink()
{
  lcd.setCursor(0, 0);lcd.print("  CMTC Temp & Humi ");
  int temp = myDHT22.getTemperatureC();int humi = myDHT22.getHumidity();      
  lcd.setCursor(2, 1);lcd.print(" ");lcd.print("T:");lcd.print(" ");lcd.print(temp);lcd.print(" C ");
  lcd.setCursor(9, 1);lcd.print("H:");lcd.print(humi);lcd.print(" %RH ");
  DateTime now = rtc.now();
  int hour1 = now.hour();int minute1 = now.minute();int second1 = now.second();
  lcd.setCursor(7, 2);lcd.print("Time: ");lcd.print(now.hour(), DEC);lcd.print(":");lcd.print(now.minute(), DEC);lcd.print(":");lcd.print(now.second(), DEC);lcd.print("  ");
  DateTime future (now.unixtime() + 7 * 86400L + 30);
  lcd.setCursor(7, 3);lcd.print("Status : Pink     ");
}

void displaygreen()
{
  lcd.setCursor(0, 0);lcd.print("  CMTC Temp & Humi ");
  int temp = myDHT22.getTemperatureC();int humi = myDHT22.getHumidity();      
  lcd.setCursor(2, 1);lcd.print(" ");lcd.print("T:");lcd.print(" ");lcd.print(temp);lcd.print(" C ");
  lcd.setCursor(9, 1);lcd.print("H:");lcd.print(humi);lcd.print(" %RH ");
  DateTime now = rtc.now();
  int hour1 = now.hour();int minute1 = now.minute();int second1 = now.second();
  lcd.setCursor(7, 2);lcd.print("Time: ");lcd.print(now.hour(), DEC);lcd.print(':');lcd.print(now.minute(), DEC);lcd.print(':');lcd.print(now.second(), DEC);lcd.print("  ");
  DateTime future (now.unixtime() + 7 * 86400L + 30);
  lcd.setCursor(7, 3);lcd.print("Status : Green     "); 
}

void displayorange()
{
  lcd.setCursor(0, 0);lcd.print("  CMTC Temp & Humi ");
  int temp = myDHT22.getTemperatureC();int humi = myDHT22.getHumidity();      
  lcd.setCursor(2, 1);lcd.print(" ");lcd.print("T:");lcd.print(" ");lcd.print(temp);lcd.print(" C ");
  lcd.setCursor(9, 1);lcd.print("H:");lcd.print(humi);lcd.print(" %RH ");
  DateTime now = rtc.now();
  int hour1 = now.hour();int minute1 = now.minute();int second1 = now.second();
  lcd.setCursor(7, 2);lcd.print("Time: ");lcd.print(now.hour(), DEC);lcd.print(':');lcd.print(now.minute(), DEC);lcd.print(':');lcd.print(now.second(), DEC);lcd.print("  ");
  DateTime future (now.unixtime() + 7 * 86400L + 30);
  lcd.setCursor(7, 3);lcd.print("Status : ORANGE     ");
}

void displayred()
{
  lcd.setCursor(0, 0);lcd.print("  CMTC Temp & Humi ");
  int temp = myDHT22.getTemperatureC();int humi = myDHT22.getHumidity();      
  lcd.setCursor(2, 1);lcd.print(" ");lcd.print("T:");lcd.print(" ");lcd.print(temp);lcd.print(" C ");
  lcd.setCursor(9, 1);lcd.print("H:");lcd.print(humi);lcd.print(" %RH ");
  DateTime now = rtc.now();
  int hour1 = now.hour();int minute1 = now.minute();int second1 = now.second();
  lcd.setCursor(7, 2);lcd.print("Time: ");lcd.print(now.hour(), DEC);lcd.print(':');lcd.print(now.minute(), DEC);lcd.print(':');lcd.print(now.second(), DEC);lcd.print("  ");
  DateTime future (now.unixtime() + 7 * 86400L + 30);
  lcd.setCursor(7, 3);lcd.print("Status : RED     ");
}

void displayblue()
{
  lcd.setCursor(0, 0);lcd.print("  CMTC Temp & Humi ");
  int temp = myDHT22.getTemperatureC();int humi = myDHT22.getHumidity();      
  lcd.setCursor(2, 1);lcd.print(" ");lcd.print("T:");lcd.print(" ");lcd.print(temp);lcd.print(" C ");
  lcd.setCursor(9, 1);lcd.print("H:");lcd.print(humi);lcd.print(" %RH ");
  DateTime now = rtc.now();
  int hour1 = now.hour();int minute1 = now.minute();int second1 = now.second();
  lcd.setCursor(7, 2);lcd.print("Time: ");lcd.print(now.hour(), DEC);lcd.print(':');lcd.print(now.minute(), DEC);lcd.print(':');lcd.print(now.second(), DEC);lcd.print("  ");
  DateTime future (now.unixtime() + 7 * 86400L + 30);
  lcd.setCursor(7, 3);lcd.print("Status : BLUE     ");
}
