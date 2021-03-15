//SETUP PINS
   void Setup_pins()
   {
    //Set up input and output pins
      pinMode(Relay_1, OUTPUT);
      pinMode(Relay_2, OUTPUT); 
      pinMode(Relay_3, OUTPUT); 
      
      pinMode(DisplayLight_OUT, OUTPUT);
      
    //Set status of pins
      digitalWrite(Relay_1, HIGH);
      digitalWrite(Relay_2, HIGH);
      digitalWrite(Relay_3, HIGH);
      
      Display_Setup_pins_serial();
}
