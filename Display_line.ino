//PLACES CURSOR AND PRINTS LINES ON LCD - update of the menu is done in Display_menu

/* SPECIAL CHARACTERS (See function Setup_LC)
 LCD.print(char(1)); // writes "æ"
 LCD.print(char(2)); // writes "ø"
 LCD.write(3); // writes "å"
 LCD.write(4); // writes "Æ"
 LCD.write(5); // writes "Ø"
 LCD.write(6); // writes "Å"
 LCD.write(7); // writes Up pointer
 LCD.print(char(0)); // writes Down pointer
*/

  //  Menu lines
      void Display_item(int x)
      {
        switch (x)
        {
          //Menu 1 - START SCREEN
          case 11: Display_humtemp();break;
          // Item12 is used for printing a message according to a function passed as a parameter to Dispaly_menu()
          
          //Menu 2 - CONTROLS MENU
          case 21: LCD.setCursor(0,Line);LCD.print("HANDLINGER      ");break;
          case 22: LCD.setCursor(0,Line);LCD.print(" Varme:     ");Display_mode_status(TempSet);break;
          case 23: LCD.setCursor(0,Line);LCD.print(" Fugter:    ");Display_mode_status(HumSet);break;
          case 24: LCD.setCursor(0,Line);LCD.print(" Bl");LCD.print(char(1));LCD.print("ser:    ");Display_mode_status(FanSet);break;
          case 25: LCD.setCursor(0,Line);LCD.print(" EXIT MENU      ");break;

          //Menu 3 lines - STATUS MENU
          case 31: LCD.setCursor(0,Line);LCD.print("STATUS          ");break;
          case 32: LCD.setCursor(0,Line);LCD.print(" Varme ");Display_motor_status(TempStatus);LCD.setCursor(14,Line);LCD.print("  "); break;
          case 33: LCD.setCursor(0,Line);LCD.print(" Fugter ");Display_motor_status(HumStatus);LCD.setCursor(15,Line);LCD.print(" ");break;
          case 34: LCD.setCursor(0,Line);LCD.print(" Bl");LCD.print(char(1));LCD.print("ser ");LCD.setCursor(15,Line);LCD.print(" "); break;
          case 35: LCD.setCursor(0,Line);LCD.print(" EXIT MENU      ");break;

          //Menu 4 lines - TEMPERATURE MENU
          case 41: LCD.setCursor(0,Line);LCD.print("INDSTIL VARME   ");break;
          case 42: LCD.setCursor(0,Line);LCD.print(" Mode:      ");Display_mode_status(TempSet);break;
          case 43: LCD.setCursor(0,Line);LCD.print(" Auto:    ");LCD.print(TempGoal);LCD.setCursor(14,Line);LCD.print(char(223));LCD.print("C");break;
          case 44: LCD.setCursor(0,Line);LCD.print(" Dif. ");LCD.write(7);LCD.print(": ");LCD.print((float)TempHighLimit);LCD.setCursor(14,Line);LCD.print(char(223));LCD.print("C");break;
          case 45: LCD.setCursor(0,Line);LCD.print(" Dif. ");LCD.print(char(0));LCD.print(": ");LCD.print((float)TempLowLimit);LCD.setCursor(14,Line);LCD.print(char(223));LCD.print("C");break;
          case 46: LCD.setCursor(0,Line);LCD.print(" Varighed: ");LCDprintDigits(TempDuration_minute);LCD.print(":"); LCDprintDigits(TempDuration_second); break;
          case 47: LCD.setCursor(0,Line);LCD.print(" Interval: ");LCDprintDigits(TempInterval_minute);LCD.print(":"); LCDprintDigits(TempInterval_second); break;
          case 48: LCD.setCursor(0,Line);LCD.print(" EXIT MENU      ");break;
          
          //Menu 5 lines - HUMIDITY MENU
          case 51: LCD.setCursor(0,Line);LCD.print("INDSTIL FUGT    ");break;
          case 52: LCD.setCursor(0,Line);LCD.print(" Mode:      ");Display_mode_status(HumSet);break;
          case 53: LCD.setCursor(0,Line);LCD.print(" Max hum.:   ");LCD.print(HumHighLimit);LCD.setCursor(15,Line);LCD.print("%");break;
          case 54: LCD.setCursor(0,Line);LCD.print(" Min hum.:   ");LCD.print(HumLowLimit);LCD.setCursor(15,Line);LCD.print("%");break;
          case 55: LCD.setCursor(0,Line);LCD.print(" EXIT MENU      ");break;

          //Menu 6 lines - FAN MENU
          case 61: LCD.setCursor(0,Line);LCD.print("INDSTIL BL");LCD.write(4);LCD.print("SER  ");break;
          case 62: LCD.setCursor(0,Line);LCD.print(" Mode:      ");Display_mode_status(FanSet);break;
          case 63: LCD.setCursor(0,Line);LCD.print(" Varighed: ");LCDprintDigits(FanDuration_minute);LCD.print(":"); LCDprintDigits(FanDuration_second); break;
          case 64: LCD.setCursor(0,Line);LCD.print(" Interval: ");LCDprintDigits(FanInterval_minute);LCD.print(":"); LCDprintDigits(FanInterval_second); break;
          case 65: LCD.setCursor(0,Line);LCD.print(" EXIT MENU      ");break;
        }
      }
 
  //  Generic texts (use in different menues)
      
      // Display "*" or ">" in col 0
      void Display_cur()
      {
       LCD.setCursor(0,Line);
       if (CurSelect == 1) {LCD.print (">");}
       else {LCD.print ("*");}
      }

      // Empty old cursor " " in col 0
      void Display_curdel()
      {
       LCD.setCursor(0,Line);
       LCD.print (" ");
      }
      
      //Empty line
      void Display_empty()
      {
        LCD.setCursor(0,Line);
        LCD.print("                ");
      }
      
      //Date and time
      void Display_humtemp()
      { 
        LCD.setCursor(0,Line);
        LCDprintDigits(Hum);
        if (Hum>99) {LCD.setCursor(3,Line);}
        else {LCD.setCursor(2,Line);}
        LCD.print("%RH     ");
        LCD.print(Temp);
        LCD.setCursor(14,Line);
        LCD.print(char(223));
        LCD.print("C");
      }
      
      //Error message
      void Display_ERROR()
      {
       LCD.print("Fejl!");  
      }
  
      //Update message
      void Display_update(int Delay)
      {
        LCD.setCursor(0,Line);
        LCD.print("Opdaterer");
        delay(Delay);
        LCD.print(".");
        delay(Delay);
        LCD.print(".");
        delay(Delay);
        LCD.print(".");
        delay(Delay);
        LCD.print(".");
        delay(Delay);
        Reset_watchdog_timer();
        Display_Display_update_serial();
        Display_empty();
       }        

      //Save message
      void Display_save(int Delay)
      {
        LCD.setCursor(1,Line);
        LCD.print("               ");
        LCD.setCursor(1,Line);
        LCD.print("Gemmer");
        delay(Delay);
        LCD.print(".");
        delay(Delay);
        LCD.print(".");
        delay(Delay);
        LCD.print(".");
        delay(Delay);
        LCD.print(".");
        delay(Delay);
        Reset_watchdog_timer();
        Display_empty();
       }   
   
   // UTILITY FIÙNCTIONS - prints something at a position defined in another function
   
      //  Mode status - values: 1 = Timer, 0 = Automatic, 2 = Disabled - manual mode
      void Display_mode_status(byte x)
      {
        switch (x)
        { 
          case 0: LCD.print("AUTO"); break;
          case 1: LCD.print(" TIL"); break;          
          case 2: LCD.print(" FRA");break;
        }
      }
      
      //  Light status - values LightStatus: 0 = No light nedded; 1 = Morning light has not been turned on yet, 2 = Morning light is turned on, 3 = Morning light has been turned off, 4 = Evening light is turned on);   
      void Display_motor_status(int x)
      {
        switch (x)
        { //Space as last character to fit cursor in menues
          case 0: LCD.print("slukket"); break;
          case 1: LCD.print("t"); LCD.print(char(1)); LCD.print("ndt  "); break;          
        }
       }
      
      //Display "..."
      void Display_processing()
      {
       delay(SensitivityKeypad);
       LCD.print(".");
       Reset_watchdog_timer();
       delay(SensitivityKeypad);
       Input_keypad();
       LCD.print(".");
       Reset_watchdog_timer();
       delay(SensitivityKeypad);
       Input_keypad();
       LCD.print(".");
       delay(SensitivityKeypad);
       Input_keypad();
       Reset_watchdog_timer();
      }

      //Utility function to add "0" if number is smaller than 10
      void LCDprintDigits(int digits)
      {
       if(digits < 10)
       LCD.print('0');
       LCD.print(digits);
      }
  
