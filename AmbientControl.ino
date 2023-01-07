/******************************************************************
 Created - 7.1.2023
 
 Project    : AMBIENT CONTROL PROGRAM - AmbientControl
 
 Libraries  : Wire.h //Included in Arduino IDE folder hardware/libraries/Wire
              TimeLib.h
              LiquidCrystal.h Included in Arduino IDE folder
              DFRobot_SHT20.h https://github.com/DFRobot/DFRobot_SHT20
              EEPROM.h Included in Arduino IDE folder
              avr/wdt.h Included in Arduino IDE folder
 
 Author     : Simon Nyrup Madsen
 
 DESCRIPTION:
 This program is designes to control relays on the basis of temperature and humidity inputs, timers and manual inputs from a keypad. Its features includes:
 * Timekeeping and setting timers
 * Measurering temperature and humidty
 * Control of 4 relays on the basis of temperature and humidity measure and timers.
 * Keypad and display for manual control and for changeing settings.
 In this version, the program is designed to manage a mushroom incobation chamber or grow room with automatic heating by thermostat and air exchange/humidifying from timer.
 
 VERSION NOTES;
 OBS! In this version, the fan on auto mode is set to turn on, whenever the humidifier is on.
 
 USER INPUTS (SETTINGS IN CODE):
 * Default timer parameters and delays - force save to eeprom on installation, see notice in code (Setup-routine)
 * Serial debug - set to true or false.
 * Treshholds for keypad shield buttons (Input_keypad tab).
    
 SUGGESTED IMPROVEMENTS:
 * Automatical saving to eeprom on first setup.
 * Option to restore settings to preset values from keypad.
 
 HARDWARE:  
 *  Arduino Uno
        Power: 7-12V (recommended), approx. 100 mA consumption
        I/O pins max. 20mA for longer periods, 40mA for shorter
        Power output from 5V pins max. 400 mA
 *  Relay-module - 4 Realys in this version
        Power: 5V, ~75-90 mA each when active - must be powered from seperate supply
        Pins: D2, D3, A2, A3, 5V (VCC-JD jumper off - VCC from arduino on pin side - seperate GND and power supply to VCC-JD from external power supply
 *  LCD Keypad Shield, DF Robot or HW-555 - NB! Change values in function "Input_keypad" according to the type of keypad shield
        Power: 5V, ~80mA with full backlight
        Pins: LCD D4-10, Keypad A0
        Library: LiquidCrystal.h
 *  Temperature and humidity sensor SHT20 
        Power: 3.3V, < 1 mA
        Pins: Connect to SCL and SDA on Arduino and supplied from 3.3 V outlet and GND on Arduino
        Library: DFRobot_SHT20.h
 *  Power supply:
        LM2596 DC-DC step down 4-30V to supply vin pin with 12V and/or seperately relays with 5v, 2,5A
        WX-DC12003 to convert 220V to 5V+/-0,15V, 700mA to supply relays (NB! Not stable enough to supply arduino 5v pin)

******************************************************************/

//INCLUSION OF LIBRARIES - libraries compatible with Arduino IDE 1.8.13

  //  Time features
      #include <TimeLib.h> 
            
  //  Temperature and humidity sensor
      #include <Wire.h>
      #include <DFRobot_SHT20.h>
            
  //  LCD display
      #include  <LiquidCrystal.h> 
      
  //  EEPROM to save settings to non-volatile memory on the board
      #include <EEPROM.h>
  
  //  Watchdog to reset
      #include <avr/wdt.h>

//DISTRIBUTION OF PINS

  //  Keypad
      #define KeyPad_IN A0
  
  //  Relay
      #define Relay_1 2 //Heater
      #define Relay_2 3 //Humidifier
      #define Relay_3 A2 //Fan
      #define Relay_4 A3 //Light
                   
  //  LCD 1602
      LiquidCrystal LCD( 8, 9, 4, 5, 6, 7); //LCD pins set by syntax LiquidCrystal [Name](RS,E,D4,D5,D6,D7)
      #define DisplayLight_OUT 10 //Light intensity on display - used in Setup_LCD()

  //  Temperature and humidity sensor SHT20 uses I2C
      //SCL - pin A4 occupied
      //SDA - pin A5 occupied

//DISTRIBUTION OF EEPROM ADRESSES

      #define Eeprom_status_address         0
      
      #define TempSet_address               1

      #define TempDuration_minute_address   2
      #define TempDuration_second_address   3
      #define TempInterval_minute_address   4
      #define TempInterval_second_address   5
        
      #define HumSet_address                6
      #define HumHighLimit_address          7
      #define HumLowLimit_address           8
      
      #define FanSet_address                9
      #define FanDuration_minute_address   10
      #define FanDuration_second_address   11
      #define FanInterval_minute_address   12
      #define FanInterval_second_address   13

      #define LightSet_address             14
      #define LightDuration_hour_address   15
      #define LightDuration_minute_address 16
      #define LightInterval_hour_address   17
      #define LightInterval_minute_address 18

      #define TempGoal_address             20
      #define TempHighLimit_address        30
      #define TempLowLimit_address         40
      
//DECLARATION OF GLOBAL VARIABLES AND SETTING SOME VALUES (SETTINGS)
    
  //  Activate debugging messages in serial screen - set to true to debug (Warning: if all is activated, dynamic memory will overload)

      const bool SerialDebugSetup = true;
      const bool SerialDebugDisplay = false;
      const bool SerialDebugControls = false;
      const bool SerialDebugStatus = true;   //Status of relays
      const bool SerialTempLog = true;
      const bool SerialHumLog =true;
      
  //  Constants and preset values for timer-variables (values used, when nothing has yet been saved in EEPROM, or if code overwriting EEPROM status variable is activated in setup-routine)

      byte TempArea = 1; //Values: 1 = Below low limit, 2 = Between hign and low limit; 3= Between high limit and goal, 4 = Above goal
      
      #define TempSet_preset              0 // Values: 1 = Heater always on, 0 = Heater switched on automatically, 2 = Heater disabled
      #define TempGoal_preset            23.50f
      #define TempHighLimit_preset       -2.50f
      #define TempLowLimit_preset        -5.00f
      
      #define TempDuration_minute_preset  0 // These values define the scheme used in temperature area 2 and 3 - in temperature area 3 duration is dvided by TempAdjust in Initiate_event
      #define TempDuration_second_preset 14
      
      #define TempInterval_minute_preset  0
      #define TempInterval_second_preset 30

      byte TempAdjust =                   2; //For area 3, duration is divided by this number
      
      #define HumSet_preset               0 // Values: 1 = Humidifier always on, 0 = Humidifier switched on automatically, 2 = Humidifier disabled
      #define HumHighLimit_preset        95
      #define HumLowLimit_preset         85
            
      #define FanSet_preset               0 // Values: 1 = Fan always on, 0 = Fan switched on automatically, 2 = Fan disabled
      #define FanDuration_minute_preset   1
      #define FanDuration_second_preset   0
      #define FanInterval_minute_preset   1
      #define FanInterval_second_preset   0

      #define LightSet_preset             0 // Values: 1 = Fan always on, 0 = Fan switched on automatically, 2 = Fan disabled
      #define LightDuration_hour_preset  12
      #define LightDuration_minute_preset 0
      #define LightInterval_hour_preset  12
      #define LightInterval_minute_preset 0

  //  Intensity of backgrund light on reset - set a number between 0 and 255
      const int DisplayLightIntensity = 30;

  //  KeyPad and display fuctions
      int SensitivityKeypad = 500; //  Set sensitivity of buttons on keypad in miliseconds - don't exceed 500 not to trigger Watchdog reset
      int MenuReset = 45; // Seconds before menu exits, so loop can continue. If value is too high, system will reset.

  //  Menu structure
      #define MenuMax     7  // Number of menues
      
      #define Lines_menu1 2  // Number of lines in each menu
      #define Lines_menu2 6
      #define Lines_menu3 6
      #define Lines_menu4 8
      #define Lines_menu5 5
      #define Lines_menu6 5
      #define Lines_menu7 5

//DECLARATION OF SOME ADDITIONAL GLOBAL VARIABLES USED BY THE FUNCTIONS - don't cahnge!
  
  //  Calculated in Initiate_event()   
      time_t TempOn_t = now();
      time_t TempOff_t = now();
      
      time_t FanOn_t = now();
      time_t FanOff_t = now();

      time_t LightOn_t = now();
      time_t LightOff_t = now();
      
  //  Calcultated in Input_keypad();
      time_t Adjusted_time;

  //  Define global variables and read values from EEPROM
      byte Eeprom_status = EEPROM.read(Eeprom_status_address);
             
      byte TempSet = EEPROM.read(TempSet_address);
      float TempGoal = eeprom_read_float((float*) TempGoal_address);
      float TempHighLimit = eeprom_read_float((float*) TempHighLimit_address);
      float TempLowLimit = eeprom_read_float((float*) TempLowLimit_address);
      byte TempDuration_minute = EEPROM.read(TempDuration_minute_address);
      byte TempDuration_second = EEPROM.read(TempDuration_second_address);
      byte TempInterval_minute = EEPROM.read(TempInterval_minute_address);
      byte TempInterval_second = EEPROM.read(TempInterval_second_address);
      
      byte HumSet = EEPROM.read(HumSet_address);
      byte HumHighLimit = EEPROM.read(HumHighLimit_address);
      byte HumLowLimit = EEPROM.read(HumLowLimit_address);
      
      byte FanSet = EEPROM.read(FanSet_address);
      byte FanDuration_minute = EEPROM.read(FanDuration_minute_address);
      byte FanDuration_second = EEPROM.read(FanDuration_second_address);
      byte FanInterval_minute = EEPROM.read(FanInterval_minute_address);
      byte FanInterval_second = EEPROM.read(FanInterval_second_address);

      byte LightSet = EEPROM.read(LightSet_address);
      byte LightDuration_hour = EEPROM.read(LightDuration_hour_address);
      byte LightDuration_minute = EEPROM.read(LightDuration_minute_address);
      byte LightInterval_hour = EEPROM.read(LightInterval_hour_address);
      byte LightInterval_minute = EEPROM.read(LightInterval_minute_address);
     
  //  Used in Input_keypad and Display_menu functions
      int KeyStatus = 0;
      int KeyValue = 0;     
      byte Pos = 11;
      byte Cur = 2;         //Values: 2 = cursor off, 0 = Cursor line 0, 1 = cur line 1
      byte CurSelect = 0;   //Values: 1 = Line selected, 0 = Line not selected
      byte PosLast = 0;
      byte CurDel = 2;      //Values: 0 = Cursor deleted in line 0, 1 = Curor deleted in line 1 (next time Display_menu() is called)
      byte Line = 0;
      byte ValueChanged = 0;

  //  Variables used in Check_status fucntions
      byte TempStatus = 0;  //Values: 0 = Off; 1 = On;
      byte HumStatus = 0;   //Values: 0 = Off; 1 = On;
      byte FanStatus = 0;   //Values: 0 = Off; 1 = On;
      byte LightStatus = 0; //Values: 0 = Off; 1 = On;
      
  //  For temperature and humidity sensor
      float Temp = 0;
      float Hum = 0;
      DFRobot_SHT20    sht20;
        
  //  From Setup_reset functions
      time_t ResetTime_t;
      time_t MenuReset_t;

//SETUP ROUTINE - runs only once    
void setup()
{ 
  //  Define PINs as input/output
      Setup_pins();       
        
  //  Initiate serial screen
      Serial.begin(9600);

  //  Initiate LCD, set background light intensity and define special characters
      Setup_LCD();
           
  //  Get values from EEPROM
      //Eeprom_status = 0; // Force EEPROM to reset to preset values above  - Values of adress 0: 1 = eeprom is up to date, 2 = Update EPROM with new time-values set by Keypad, 0 = Reset to preset values above - NB: RESTART to have them loaded into RAM, then outcomment this line and upload again
      Setup_eeprom();
    
  //  Setup watchdog timer
      Setup_reset();
   
  //  Display update message to indicate, that reset has been done
      Display_notice_menu(Display_update, 500); //Don´t set so high that watchdog is not activated (after 8 secs. without reset)
 }

//PROGRAM - runs in loop - functions defined in other tabs
void loop()
{
  //  Check to se, if it is time to reset Arduino - if not, watch dog timer is reset - this also prevents Arduino to fuck up, sinse it resets every 8 seconds if this function is not called
      Reset_watchdog_timer();
      
  //  Update relays
      Initiate_event();
      
  //  Get temperature and humidity from SHT20 
      Input_sensors();
      
  //  Check keypad, timers, display status and initate events
      Input_keypad();

  //  Update menu
      Display_menu();
}

//GENERAL UTILITY FUNCTIONS

  //  Calculate time_t for TODAYS occurrance from input hour, minute and second
  time_t Time_t(byte Hour, byte Minute, byte Second)
  {
   time_t Todays_t;
   TimeElements tm;
   tm.Hour = Hour;
   tm.Minute = Minute;
   tm.Second = Second;
   tm.Day = day();
   tm.Month = month();
   tm.Year = year()-1970;
   Todays_t = makeTime(tm);
   return Todays_t;
  }
  
  //  Calculate time_t for NEXT occurrance from input hour and minute
  time_t Time_next_t(byte Hour, byte Minute)
  {
   time_t Next_t;
   TimeElements tm;
   tm.Hour = Hour;
   tm.Minute = Minute;
   tm.Second = 0;
   if ((hour() >= Hour) && (minute() >= Minute))
   {
     tm.Day = day() + 1;
   }
   else
   {
     tm.Day = day();
   }
   tm.Month = month();
   tm.Year = year()-1970;
   Next_t = makeTime(tm);
   return Next_t;
  }
 
  //  Calculate time_t for specific DATE
  time_t Time_date_t(byte Day, byte Month)
  {
   time_t Date_t;
   TimeElements tm;
   tm.Hour = 23;
   tm.Minute = 59;
   tm.Second = 59;
   tm.Day = Day;
   tm.Month = Month;
   tm.Year = year()-1970;
   Date_t = makeTime(tm);
   return Date_t;
  }
