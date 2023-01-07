//CHECK TO SEE, IF EVENTS SHOULD BE INITIATIED - operation of realys occours in Motor-functions
  
  void Initiate_event()
  {
    Initiate_temp_event();
    Initiate_hum_event();
    Initiate_fan_event();
    Initiate_light_event();
  }
  
  void Initiate_temp_event()
  {
   Display_Initiate_temp_event_serial();
   Display_temp_status_serial();
   switch(TempSet)
   {
    case 1: Motor_on_temp();break;
    case 2: Motor_off_temp();break;
    case 0: 
    {
    
     if((float)Temp >= (float)TempGoal)
     {
      TempArea = 4;
      Motor_off_temp();
      break;
     }
     
     if ((float)Temp < (float)TempGoal + (float)TempLowLimit)
     {
      Motor_on_temp();
      TempArea = 1;
      break;
     }
     
     if((float)Temp > (float)TempGoal + (float)TempLowLimit && (float)Temp < (float)TempGoal + (float)TempHighLimit)
     {
      TempArea = 2;
      if(TempOff_t < now())
      {
       Motor_off_temp();
       TempOn_t = now()+ TempInterval_minute*60UL + TempInterval_second;
       TempOff_t = TempOn_t + TempDuration_minute*60UL + TempDuration_second;
      }
      if(TempOn_t < now())
      {
       Motor_on_temp();
      }
      break;
     }

     if((float)Temp > (float)TempGoal + (float)TempHighLimit && (float)Temp < (float)TempGoal)
     {
      TempArea = 3;
      if(TempOff_t < now())
      {
       Motor_off_temp();
       TempOn_t = now()+ TempInterval_minute*60UL + TempInterval_second;
       TempOff_t = TempOn_t + TempDuration_minute*60UL/TempAdjust + TempDuration_second/TempAdjust;
      }
      if(TempOn_t < now())
      {
       Motor_on_temp();
      }
      break;
     }
     
    }
   }
  }
    
  void Initiate_hum_event()                         
  {
    Display_Initiate_hum_event_serial();
    Display_hum_status_serial();
    switch(HumSet)
    {
     case 1: Motor_on_hum(); break;
     case 2: Motor_off_hum(); break;
     case 0: 
         {
          if (Hum >= HumHighLimit) {Motor_off_hum();}
          if (Hum <= HumLowLimit){Motor_on_hum();}
          break;
         } 
     default: break;
     }
    }
  
  void Initiate_fan_event()
  {
    Display_Initiate_fan_event_serial();
    Display_fan_status_serial();
    switch(FanSet)
    {
     case 1: Motor_on_fan(); break;
     case 2: Motor_off_fan(); break;
     case 0: 
         { 
          if (HumStatus == 1) {Motor_on_fan();break;}
          switch(FanStatus)
          {
           case 0:
           {
            if (FanOn_t < now())
            {
              FanOff_t=Time_t(hour(), minute()+FanDuration_minute, second()+FanDuration_second);
              FanOn_t=Time_t(hour(FanOff_t), minute(FanOff_t)+FanInterval_minute, second(FanOff_t)+FanInterval_second);
              Motor_on_fan();
              break;
            }
           }
           case 1:
           {
            if (FanOff_t < now()) {Motor_off_fan();break;}
            else {break;}
           }
          }
         }
    }
   } 

   void Initiate_light_event()
  {
    Display_Initiate_light_event_serial();
    Display_light_status_serial();
    switch(LightSet)
    {
     case 1: Motor_on_light(); break;
     case 2: Motor_off_light(); break;
     case 0: 
         { 
          switch(LightStatus)
          {
           case 0:
           {
            if (LightOn_t < now())
            {
              LightOff_t=Time_t(hour()+LightDuration_hour, minute()+LightDuration_minute, second());
              LightOn_t=Time_t(hour(LightOff_t)+LightInterval_hour, minute(LightOff_t)+LightInterval_minute, second(LightOff_t));
              Motor_on_light();
              break;
            }
           }
           case 1:
           {
            if (LightOff_t < now()) {Motor_off_light();break;}
            else {break;}
           }
          }
         }
    }
   } 
