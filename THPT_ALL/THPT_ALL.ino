#include <Wire.h> 
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
/* Địa chỉ của DS1307 */
const byte DS1307 = 0x68;
/* Số byte dữ liệu sẽ đọc từ DS1307 */
const byte NumberOfFields = 7;
 
/* khai báo các biến thời gian */
int second, minute, hour, day, wday, month, year;
int voHoc[][2] = {
  // Sang
//  {6, 45},
  {7, 0},
  {7, 50},
  {8, 50},
  {9, 40},
  {10, 30},
  // Chieu
//  {12, 45},
  {13, 0},
  {13, 50},
  {14, 40},
  {15, 40},
  {16, 30}
};
int raTiet[][2] = {
  // Sang
  {7, 45},
  {8, 35},  // Ra choi 1
  {9, 35},
  {10, 25},
  {11, 15}, // Ra ve 4
  //Chieu
  {13, 45},
  {14, 35},
  {15, 25}, // Ra choi 7
  {16, 25},
  {17, 15} // Ra ve 9
};
int minuteTime[10][2];
int minuteNow;
boolean pReset = true;
boolean turnOn, turnOff = true; // turnOn to display just once, turnoff to play mp3 SHDB
boolean endSchool = true; // play mp3 Tan truong and Ra choi
boolean mp3Tiet = true;
int classTime;
void CheckTime();
void LCDTime();
void setup()
{
  Wire.begin();
  /* cài đặt thời gian cho module */
//  setTime(9, 35, 45, 4, 21, 10, 18); // 12:30:45 CN 08-02-2015
  Serial.begin(9600);
  lcd.begin(20, 4);
  mp3_set_serial (Serial);  //set Serial for DFPlayer-mini mp3 module 
  mp3_set_volume (30);
  // convert to minute times
  for (int index = 0; index < 10; index++){
    minuteTime[index][0] = voHoc[index][0] * 60 + voHoc[index][1];
    minuteTime[index][1] = raTiet[index][0] * 60 + raTiet[index][1];
  }
}
 
void loop()
{
  readDS1307();
  LCDTime();
  CheckTime();
  delay(1000);
}
void LCDTime(){
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Hello everyone!");

  lcd.setCursor(0,1);
  lcd.print(hour);
  lcd.print(":");
  lcd.print(minute);
  lcd.print(":");
  lcd.print(second);
  lcd.print(" ");
  lcd.print(day);
  lcd.print("/");
  lcd.print(month);
  lcd.print("/");
  lcd.print(year);  
}
void CheckTime(){
    // solved play just once at the right time :))
  minuteNow = hour*60 + minute;  
  lcd.setCursor(0,2);
  turnOn = true;
    if ((minuteNow >= minuteTime[1][1] && minuteNow < minuteTime[2][0]) 
    || (minuteNow >= minuteTime[7][1] && minuteNow < minuteTime[8][0]))
  {
      lcd.print("Gio ra choi! ");
      if ( endSchool && (minuteNow == minuteTime[1][1] || minuteNow == minuteTime[7][1]))
      {
          mp3_play(3);
          endSchool = false;  
      }
  }
    else if ( (minuteNow >= minuteTime[4][1] && minuteNow < 765) // 765' = 12h45
        || (minuteNow >= minuteTime[9][1] || minuteNow < 405 )) // 405' = 6h45
  {
      lcd.print("TAN HOC! ");
      if (endSchool && ( minuteNow == minuteTime[4][1] || minuteNow == minuteTime[9][1]))
            {
                mp3_play(2);
                endSchool = false;
            }

  }
    else for (int i = 0; i < 10; i++)
  { 
      endSchool = true; 
      if (minuteNow >= minuteTime[i][0] && minuteNow < minuteTime[i][1])
      {
        if (turnOn)
        {
          lcd.print(" Tiet: ");
          lcd.print( i + 1);
          turnOn = false; // for dislay just once

          turnOff = true; // mp3 SHDB
          if (mp3Tiet && minuteNow == minuteTime[i][0]) // default both is true
          {
              mp3_play(4);
          }
          mp3Tiet = false;
        }

      }
      else if ( (minuteNow >=  405 && minuteNow < 420) || (minuteNow >=  765 && minuteNow < 780) )
      {
        if (turnOn)
        {
            lcd.print("Sinh hoat dau buoi");
            turnOn = false;
            if (turnOff && (minuteNow ==  405 || minuteNow ==  765))
            {
                mp3_play(4);
                turnOff = false;              
            } 
        }
      }
      else {
        if (turnOn && i == 9) // don't fix case: when user turn off power ( or press reset)
        {                     // Maybe into "GIAI LAO" at firt time and MP3 don't play
            lcd.print(" GIAI LAO! ");
            turnOn = false;
            if (!mp3Tiet)
            {
                mp3_play(1);
            }
            mp3Tiet = true;            
        }
      }
  }
}
void readDS1307()
{
        Wire.beginTransmission(DS1307);
        Wire.write((byte)0x00);
        Wire.endTransmission();
        Wire.requestFrom(DS1307, NumberOfFields);
        
        second = bcd2dec(Wire.read() & 0x7f);
        minute = bcd2dec(Wire.read() );
        hour   = bcd2dec(Wire.read() & 0x3f); // chế độ 24h.
        wday   = bcd2dec(Wire.read() );
        day    = bcd2dec(Wire.read() );
        month  = bcd2dec(Wire.read() );
        year   = bcd2dec(Wire.read() );
        year += 2000;    
}
/* Chuyển từ format BCD (Binary-Coded Decimal) sang Decimal */
int bcd2dec(byte num)
{
        return ((num/16 * 10) + (num % 16));
}
/* Chuyển từ Decimal sang BCD */
int dec2bcd(byte num)
{
        return ((num/10 * 16) + (num % 10));
}
 
void digitalClockDisplay(){
    // digital clock display of the time
    Serial.print(hour);
    printDigits(minute);
    printDigits(second);
    Serial.print(" ");
    Serial.print(day);
    Serial.print(" ");
    Serial.print(month);
    Serial.print(" ");
    Serial.print(year); 
    Serial.println(); 
}
 
void printDigits(int digits){
    // các thành phần thời gian được ngăn chách bằng dấu :
    Serial.print(":");
        
    if(digits < 10)
        Serial.print('0');
    Serial.print(digits);
}
 
/* cài đặt thời gian cho DS1307 */
void setTime(byte hr, byte min, byte sec, byte wd, byte d, byte mth, byte yr)
{
        Wire.beginTransmission(DS1307);
        Wire.write(byte(0x00)); // đặt lại pointer
        Wire.write(dec2bcd(sec));
        Wire.write(dec2bcd(min));
        Wire.write(dec2bcd(hr));
        Wire.write(dec2bcd(wd)); // day of week: Sunday = 1, Saturday = 7
        Wire.write(dec2bcd(d)); 
        Wire.write(dec2bcd(mth));
        Wire.write(dec2bcd(yr));
        Wire.endTransmission();
}
