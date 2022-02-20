//TURNING HEATER ON AND OFF

    void Motor_on_temp()
    {
      Display_Motor_on_temp_serial();
      digitalWrite(Relay_1,LOW);
      TempStatus = 1;
     }

    void Motor_off_temp()
    {
      Display_Motor_off_temp_serial();
      digitalWrite(Relay_1,HIGH);
      TempStatus = 0;
    }

//TURNING HUMIDIFIER ON AND OFF

    void Motor_on_hum()
    {
      Display_Motor_on_hum_serial();
      digitalWrite(Relay_2,LOW);
      HumStatus = 1;
     }

    void Motor_off_hum()
    {
      Display_Motor_off_hum_serial();
      digitalWrite(Relay_2,HIGH);
      HumStatus = 0;
    }

//TURNING FAN ON AND OFF

    void Motor_on_fan()
    {
      Display_Motor_on_fan_serial();
      digitalWrite(Relay_3,LOW);
      FanStatus = 1;
     }

    void Motor_off_fan()
    {
      Display_Motor_off_fan_serial();
      digitalWrite(Relay_3,HIGH);
      FanStatus = 0;
    }

//TURNING LIGHT ON AND OFF

    void Motor_on_light()
    {
      Display_Motor_on_light_serial();
      digitalWrite(Relay_4,LOW);
      LightStatus = 1;
     }

    void Motor_off_light()
    {
      Display_Motor_off_light_serial();
      digitalWrite(Relay_4,HIGH);
      LightStatus = 0;
    }
