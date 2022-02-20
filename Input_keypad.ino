//SETS STATUS OF SENSOR BUTTONS
      
     byte Linemax_menu()
     {
       switch (Active_menu())
       {
         case 1: return Lines_menu1;
         case 2: return Lines_menu2;
         case 3: return Lines_menu3;
         case 4: return Lines_menu4;
         case 5: return Lines_menu5;
         case 6: return Lines_menu6;
         case 7: return Lines_menu6;
         return 8;
       }
     }
        
  //  Update position from keypad input
      void Input_keypad()
      {
       delay(SensitivityKeypad);
       KeyValue = analogRead(0); // Read the value from the keypad shield
       if (KeyValue < 800) 
       {
        if (CurSelect == 1){Pos = Input_value_keypad();}
        else {Pos = Input_navigate_keypad();}
        Display_Input_keypad_serial();
        Display_menu();
       }
       if (Active_menu() > 1) {Display_menu();}
      }
        
  //  Get input from keypad 

      #define RIGHT  0
      #define UP     1
      #define DOWN   2
      #define LEFT   3
      #define SELECT 4
      #define NONE   5 
   
      int Input_read_keypad()
      {
        // For HW-555        
        if (KeyValue > 800) return NONE;   //For HW-555: > 800, for DF Robot: > 1000
        if (KeyValue < 60)   return RIGHT;  //For HW-555: < 60, for DF Robot: < 10. 
        if (KeyValue < 200)  return UP;     //For HW-555: < 200, for DF Robot: < 130.
        if (KeyValue < 400)  return DOWN;   //For HW-555: < 400, for DF Robot: < 280.
        if (KeyValue < 600)  return LEFT;   //For HW-555: < 600, for DF Robot: < 450.
        if (KeyValue < 800)  return SELECT; //For HW-555: < 800, for DF Robot: < 690.  
        return NONE;// when all others fail, return this...

        //For DF Robot
        /*
        if (KeyValue > 1000) return NONE;
        if (KeyValue < 10)   return RIGHT;  
        if (KeyValue < 130)  return UP; 
        if (KeyValue < 280)  return DOWN; 
        if (KeyValue < 450)  return LEFT; 
        if (KeyValue < 690)  return SELECT;   
        return NONE;// when all others fail, return this...
        */
      }  

  //  Navigate - update position and cursor - returns updated Pos variable
      int Input_navigate_keypad()
      {
       KeyStatus = Input_read_keypad();
       byte LineMax = Linemax_menu();
       switch (KeyStatus)            
       {
        case RIGHT:
         Cur = 2;
         if (Pos > (MenuMax*10)) {return 11;}
         return (Pos-Pos%10+11);
         break;
               
        case LEFT:
         Cur = 2;
         if (Pos < 20) {return (MenuMax*10+1);}
         return (Pos-Pos%10-9);
         break;
   
        case UP:
         if (Pos < 20) {return 11;}
         if (Pos%10 == 1) {Cur = 2; CurDel = 1; return Pos;}
         if (Pos%10 == 2 && Cur == 0) {Cur = 1; return --Pos;}
         if (Cur == 1) {Cur = 0; CurDel = 1; return Pos;}
         if (Cur == 0) {return --Pos;}
         break;
        
        case DOWN:
          if (Pos < 20) {return 11;}
          if (Pos%10 == (LineMax-1)) {Cur = 1; CurDel = 0; return Pos;}
          if (Cur == 1) {return ++Pos;}
          if (Pos%10 == 1) {Cur = 1; CurDel = 2; return Pos;}
          if (Cur == 2) {Cur = 1; return Pos;}
          if (Cur == 0) {Cur = 1;CurDel = 0;return Pos;}
          return 11;
          break;
        
        case SELECT:
           if (Pos < 20) {return 11;}                            //ACTIONS MENU
           if (Pos%10 == 1)                                      //ACTIONS first and last menu items
           {if (Cur == 2) {return Pos;}}                            //First menu item always a header with no action
           if (Pos%10 == (LineMax-1)) {return 11;}                  //Last menu item always exits to start screen
           if (Cur == 1) {++Pos;}                              
           if (Active_menu() >3 || Active_menu()== 2)            //ACTIONS MENU 2 and 4+
           {
            switch(CurSelect)
            {
             case 0: CurSelect = 1;break;
             case 1: CurSelect = 0;break;
            } 
           }
           if (Active_menu() == 3) return Pos;                 //ACTION MENU 3
           if (Cur == 1) {--Pos;} 
        default: return Pos;
       }
      }

 //  Adjust values
     int Input_value_keypad()
     {
      KeyStatus = Input_read_keypad();
      Setup_menu_reset();
      ++PosLast;
      switch (KeyStatus)
      {
       case RIGHT: break;
       case LEFT: break;
       case UP: Adjust_value_keypad(1.0f); break;
       case DOWN: Adjust_value_keypad(-1.0f);break;
       case SELECT:
       {
        CurSelect = 0;
        if (ValueChanged == 1)
        {
         Eeprom_status = 2;
         Setup_eeprom();
         Line = Cur;
         Display_save(150);
         Initiate_event();
         ValueChanged = 0;
        }
         break;
       }
      }
      return Pos;
     }

     void Adjust_value_keypad(int x)
     {
      ValueChanged = 1;
      if (Cur == 1) {++Pos;}
      switch (Pos)
      {       
       //ACTIONS MENU       
       case 22:  TempSet = Adjust_mode(TempSet, x); break;
       case 23:  HumSet = Adjust_mode(HumSet, x); break;
       case 24:  FanSet = Adjust_mode(FanSet, x); break;
       case 25:  LightSet = Adjust_mode(LightSet, x); break;         
       
       //TEMPERATURE MENU       
       case 42:  TempSet = Adjust_mode(TempSet, x); break;
       case 43:  TempGoal = (float)TempGoal + (float)x/2.00f; break;
       case 44:  TempHighLimit = (float)TempHighLimit + (float)x/20.00f; break;
       case 45:  TempLowLimit = (float)TempLowLimit + (float)x/20.00f; break;                    
       case 46:  
                 TempDuration_minute = minute(Adjust_time_ms(TempDuration_minute, TempDuration_second, x));
                 TempDuration_second = second(Adjust_time_ms(TempDuration_minute, TempDuration_second, x));
                 break;
       case 47:  
                 TempInterval_minute = minute(Adjust_time_ms(TempInterval_minute, TempInterval_second, x));
                 TempInterval_second = second(Adjust_time_ms(TempInterval_minute, TempInterval_second, x));
                 break;
                 
       //HUMIDITY MENU
       case 52:  HumSet = Adjust_mode(HumSet, x); break;
       case 53:  HumHighLimit = HumHighLimit + (float)x; break;
       case 54:  HumLowLimit = HumLowLimit + (float)x; break;                    

       //FAN MENU
       case 62:  FanSet = Adjust_mode(FanSet, x); break;
       case 63:  
                 x *= 10;
                 FanDuration_minute = minute(Adjust_time_ms(FanDuration_minute, FanDuration_second, x));
                 FanDuration_second = second(Adjust_time_ms(FanDuration_minute, FanDuration_second, x));
                 break;
       case 64:  
                 x *= 10;
                 FanInterval_minute = minute(Adjust_time_ms(FanInterval_minute, FanInterval_second, x));
                 FanInterval_second = second(Adjust_time_ms(FanInterval_minute, FanInterval_second, x));
                 break;

       
        //LIGHT MENU
        case 72:  LightSet = Adjust_mode(LightSet, x); break;
        case 73:  
                  LightDuration_hour = hour(Adjust_time_hm(LightDuration_hour, LightDuration_minute, x));
                  LightDuration_minute = minute(Adjust_time_hm(LightDuration_hour, LightDuration_minute, x));
                  break;
        case 74:  
                  LightInterval_hour = hour(Adjust_time_hm(LightInterval_hour, LightInterval_minute, x));
                  LightInterval_minute = minute(Adjust_time_hm(LightInterval_hour, LightInterval_minute, x));
                  break;
      }
      if (Cur == 1) {--Pos;}
     }

//  Utility function: Calculate time_t for specific time (minute and second)
    time_t Adjust_time_ms(byte Minute, byte Second, int AddedValue)
    {
     time_t AdjustedMinuteSecond_t;
     TimeElements tm;
     tm.Hour = 1;
     tm.Minute = Minute;
     tm.Second = Second;
     tm.Day = 1;
     tm.Month = 1;
     tm.Year = 2020-1970;
     AdjustedMinuteSecond_t = makeTime(tm) + AddedValue;
     return AdjustedMinuteSecond_t;
    }

//  Utility function: Calculate time_t for specific time (hour and minute)
    time_t Adjust_time_hm(byte Hour, byte Minute, int AddedValue)
    {
     time_t AdjustedHourMinute_t;
     TimeElements tm;
     tm.Hour = Hour;
     tm.Minute = Minute;
     tm.Second = 1;
     tm.Day = 1;
     tm.Month = 1;
     tm.Year = 2020-1970;
     AdjustedHourMinute_t = makeTime(tm) + AddedValue*600;
     return AdjustedHourMinute_t;
    }

//  Utility function to adjust mode
    byte Adjust_mode(byte ModeLast, float AddedValue)
    {
     int ModeNew;
     ModeNew = ModeLast + AddedValue;
     if (ModeNew > 2) {ModeNew = 0;}
     if (ModeNew < 0) {ModeNew = 2;}
     return ModeNew;
    }
