//GETTING TEMPERATURE FROM SHT20

void Input_sensors()
{
      sht20.initSHT20();
      Temp = sht20.readTemperature();   //Get temperature from SHT20
      Display_temp_serial();
      Hum = sht20.readHumidity();       //Get humidity from SHT20
      Display_hum_serial();     
}
