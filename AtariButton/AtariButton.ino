//Allison Berman 2014
//Atari 2600 pinout: http://old.pinouts.ru/Inputs/JoystickAtari2600_pinout.shtml

//Using analog in pins because shield+OLED make digital io hard to reach



#define UP      A0
#define DOWN    A1
#define LEFT    A2
#define RIGHT   A3
#define BUTTON1 A4
//#define BUTTON2 A5 //only for two-button controllers

//In switchRead(), [pos] refers to the position of the buttonstate in this array
char lastButtonState[] = {1,1,1,1,1,1};

void setup() {
 Serial.begin(115200);
/*controller works as set of switches connected to ground
so set pullup resistors on inputs so that power goes in 
and you can read the power cut off as a button press*/

digitalWrite(UP, HIGH);
digitalWrite(DOWN, HIGH);
digitalWrite(LEFT, HIGH);
digitalWrite(RIGHT, HIGH);
digitalWrite(BUTTON1, HIGH);
//digitalWrite(A5, HIGH); //only for two-button controllers

}
void loop() {
  // put your main code here, to run repeatedly:
  switchRead(UP, 0);
  switchRead(DOWN, 1);
  switchRead(LEFT, 2);
  switchRead(RIGHT, 3);
  switchRead(BUTTON1, 4);
}

void switchRead(uint8_t B, char pos){
  
  char buttonState = digitalRead(B);
  
   //if switch closed, do this
   
   if( buttonState != lastButtonState[pos]){
      if(buttonState == LOW){
        //DEC is the Decimal number representation of A0 - A5 
        //we can't use simple numbers for the button inputs 
        //because the pins have to be referred to as strings not ints
        //some say A0 - A5 are actually 14 - 19 but that code did not work
          Serial.print(B, DEC);
          Serial.println(" On");
         }
      else if(buttonState == HIGH){
          Serial.println("Off");
         }
     } 
     
   lastButtonState[pos] = buttonState;
}
