//SETUP TIMERS FROM VALUES SAVED IN EEPROM

  //  Function to set update EEPROM - values of Eeprom_status: 1 = EEPROM is updated, don't save anything, 0 = Save preset values / nothing has been saved in EEPROM by this program, 2 = Update EPROM from time values entered via keypad (values are only written to eeprom, if they have changed)
      void Setup_eeprom()
      {
        if (Eeprom_status == 0)  
        {
          for (int i = 0 ; i < EEPROM.length() ; i++) {EEPROM.write(i, 0);}  //Clear EEPROM
          
          EEPROM.put(Eeprom_status_address,1);
             
          EEPROM.put(TempSet_address, TempSet_preset);
          eeprom_write_float((float*) TempGoal_address, TempGoal_preset);
          eeprom_write_float((float*) TempHighLimit_address, TempHighLimit_preset);
          eeprom_write_float((float*) TempLowLimit_address, TempLowLimit_preset);
          EEPROM.put(TempDuration_minute_address, TempDuration_minute_preset);
          EEPROM.put(TempDuration_second_address, TempDuration_second_preset);
          EEPROM.put(TempInterval_minute_address, TempInterval_minute_preset);
          EEPROM.put(TempInterval_second_address, TempInterval_second_preset);
                    
          EEPROM.put(HumSet_address, HumSet_preset);
          EEPROM.put(HumHighLimit_address, HumHighLimit_preset);
          EEPROM.put(HumLowLimit_address, HumLowLimit_preset);
          
          EEPROM.put(FanSet_address, FanSet_preset);
          EEPROM.put(FanDuration_minute_address, FanDuration_minute_preset);
          EEPROM.put(FanDuration_second_address, FanDuration_second_preset);
          EEPROM.put(FanInterval_minute_address, FanInterval_minute_preset);
          EEPROM.put(FanInterval_second_address, FanInterval_second_preset);
        }
        
        if (Eeprom_status == 2)  
        { 
          EEPROM.put(TempSet_address, TempSet);
          eeprom_write_float((float*) TempGoal_address, TempGoal);
          eeprom_write_float((float*) TempHighLimit_address, TempHighLimit);
          eeprom_write_float((float*) TempLowLimit_address, TempLowLimit);
          EEPROM.put(TempDuration_minute_address, TempDuration_minute);
          EEPROM.put(TempDuration_second_address, TempDuration_second);
          EEPROM.put(TempInterval_minute_address, TempInterval_minute);
          EEPROM.put(TempInterval_second_address, TempInterval_second);
                    
          EEPROM.put(HumSet_address, HumSet);
          EEPROM.put(HumHighLimit_address, HumHighLimit);
          EEPROM.put(HumLowLimit_address, HumLowLimit);
          
          EEPROM.put(FanSet_address, FanSet);
          EEPROM.put(FanDuration_minute_address, FanDuration_minute);
          EEPROM.put(FanDuration_second_address, FanDuration_second);
          EEPROM.put(FanInterval_minute_address, FanInterval_minute);
          EEPROM.put(FanInterval_second_address, FanInterval_second);
        }
      }
