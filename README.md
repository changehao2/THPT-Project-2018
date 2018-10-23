# THPT-Project-2018 
BETTER PRODUCT DEVELOPMENT

 void CheckTime(){
    // solved play just once at the right time 
  minuteNow = hour*60 + minute;  
  lcd.setCursor(0,2);
  turnOn = true;
    if ((minuteNow >= minuteTime[1][1] && minuteNow <= minuteTime[2][0]) 
    || (minuteNow >= minuteTime[7][1] && minuteNow <= minuteTime[8][0]))
  {
      lcd.print("Gio ra choi! ");
      if ( endSchool && (minuteNow == minuteTime[1][1] || minuteNow == minuteTime[7][1]))
      {
          mp3_play(3);
          endSchool = false;  
      }
  }
    else if (minuteNow >= minuteTime[4][1] || minuteNow >= minuteTime[9][1])
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
      if (minuteNow >= minuteTime[i][0] && minuteNow <= minuteTime[i][1])
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
      else if ( (minuteNow >=  405 && minuteNow <= 420) || (minuteNow >=  765 && minuteNow <= 780) )
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
