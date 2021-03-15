//FUNCTIONS TO DISPLAY CONTENT ON SERIAL SCREEN
       
  //  Temperature log function
      void Display_temp_serial(){if (SerialTempLog)
      {
       Serial.println();Serial.print((float)Temp); Serial.print(" *C"); 
       Serial.print(" TempStatus= "); Serial.print(TempStatus);
       Serial.print(" TempArea= "); Serial.print(TempArea);
      }}

  //  Humidity log function
      void Display_hum_serial(){if (SerialHumLog)
      {
       Serial.println();Serial.print((float)Hum); Serial.print(" %RH");
      }}
      
  //  Setup debug functions
      void Display_Setup_i2c_serial()
      {if (SerialDebugSetup){Serial.println();Serial.print("Setup_i2c()");}}
          
      void Display_Setup_pins_serial()
      {if (SerialDebugSetup){Serial.println();Serial.print("Setup_pins()");}}

      void Display_Setup_LCD_serial()
      {if (SerialDebugSetup){Serial.println();Serial.print("Setup_LCD()");}}

      void Display_Setup_time_sync_serial()
      {if (SerialDebugSetup){Serial.println();Serial.print("Setup_time_sync()");}}

      void Display_Setup_reset_serial()
      {if (SerialDebugSetup){Serial.println();Serial.print("Setup_reset()");}}
        
      void Display_Setup_sun_serial()
      {if (SerialDebugSetup){Serial.println();Serial.print("Setup_sun()");}}

  //  Display debug functions
      void Display_Display_update_serial()
      {if (SerialDebugDisplay){Serial.println();Serial.print("Display_update()");}}
           
      void Display_Display_time_serial()
      {if (SerialDebugDisplay){Serial.println();Serial.print("Display_time()");}}

      void Display_Input_keypad_serial()
      {if (SerialDebugDisplay){Serial.println();Serial.print("Input_keypad() - KeyStatus = ");Serial.print(KeyStatus);Serial.print(" Pos = ");Serial.print(Pos);Serial.print(" Cur = ");Serial.print(Cur);Serial.print(" CurSelect = ");Serial.print(CurSelect);}}

      void Display_Display_menu11_serial()
      {if (SerialDebugDisplay){Serial.println();Serial.print("Display_menu11() - Menu = 1, Pos = ");Serial.print(Pos);}}

      void Display_Display_menu12_serial()
      {if (SerialDebugDisplay){Serial.println();Serial.print("Display_notice_menu12() - Menu = 1, Pos = ");Serial.print(Pos);}}
      
      void Display_Display_menu_serial()
      {if (SerialDebugDisplay){Serial.println();Serial.print("Display_menu() - Menu = ");Serial.print(Active_menu());Serial.print(" - Pos = ");Serial.print(Pos);}}

  //  Control debug funtions
      void Display_Reset_watchdog_timer_serial()
      {if (SerialDebugControls){Serial.println();Serial.print("Reset_watchdog_timer()");}}

      void Display_Initiate_temp_event_serial()
      {if (SerialDebugControls){Serial.println();Serial.print("Initiate_temp_event()");}}
 
      void Display_Initiate_hum_event_serial()
      {if (SerialDebugControls){Serial.println();Serial.print("Initiate_hum_event()");}}
        
      void Display_Initiate_fan_event_serial()
      {if (SerialDebugControls){Serial.println();Serial.print("Initiate_fan_event()");}}

      void Display_Motor_on_hum_serial()
      {if (SerialDebugControls){Serial.println();Serial.print("Motor_on_hum()");}}
      
      void Display_Motor_off_hum_serial()
      {if (SerialDebugControls){Serial.println();Serial.print("Motor_off_hum()");}}

      void Display_Motor_on_fan_serial()
      {if (SerialDebugControls){Serial.println();Serial.print("Motor_on_fan()");}}
      
      void Display_Motor_off_fan_serial()
      {if (SerialDebugControls){Serial.println();Serial.print("Motor_off_fan()");}}

      void Display_Motor_on_temp_serial()
      {if (SerialDebugControls){Serial.println();Serial.print("Motor_on_temp()");}}
      
      void Display_Motor_off_temp_serial()
      {if (SerialDebugControls){Serial.println();Serial.print("Motor_off_temp()");}}
 
  //  Timers debug functions
  
      //Time and date
      void Display_time_serial()
      {if (SerialDebugStatus)
      {
      Serial.println();
      Serial.print("Now: ");
      Serial.print(day());
      Serial.print(".");
      switch(month())
      {
       case 1: Serial.print(" jan.");break;
       case 2: Serial.print(" febr.");break;
        case 3: Serial.print(" marts");break;
        case 4: Serial.print(" april");break;
        case 5: Serial.print(" maj");break;
        case 6: Serial.print(" juni");break;
        case 7: Serial.print(" juli");break;
        case 8: Serial.print(" aug.");break;
        case 9: Serial.print(" sept.");break;
        case 10: Serial.print(" okt.");break;
        case 11: Serial.print(" nov.");break;
        case 12: Serial.print(" dec.");break;
        default: Serial.print(" Fejl!");break;
       }
       Serial.print(" ");
       Serial.print(year());
       Serial.print(" ");
       SerialprintDigits(hour());
       Serial.print(":");
       SerialprintDigits(minute());
       if (DST == 1)
       {
        Serial.print(" (DST)");
       }
       if (DST == 0)
       {
       Serial.print(" (not DST)");
       }
       Serial.println();
       }
      }
            
  //  Status debug functions
   
      //  Status of light
      void Display_temp_status_serial() {if (SerialDebugStatus)
      {
        Serial.print ("Heater status: ");
        Serial.println (TempStatus);
      }}     

      //  Status of door
      void Display_hum_status_serial() {if (SerialDebugStatus)
      {
        Serial.print ("Humidifier status: ");
        Serial.println (HumStatus);
      }}
      
      //  Status of nest
      void Display_fan_status_serial() {if (SerialDebugStatus)
      {
        Serial.print ("Fan status: ");
        Serial.println (FanStatus);
      }}
      
  //  Utility function to add "0" if number is smaller than 10
      void SerialprintDigits(int digits)
      {
       if(digits < 10)
       Serial.print('0');
       Serial.print(digits);
      }
   
