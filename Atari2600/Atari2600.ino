//Allison Berman 2014
//Atari 2600 pinout: http://old.pinouts.ru/Inputs/JoystickAtari2600_pinout.shtml

//Using analog in pins because shield+OLED make digital io hard to reach



#define UP      A0
#define DOWN    A1
#define LEFT    A2
#define RIGHT   A3
#define BUTTON1 A4
//#define BUTTON2 A5 //only for two-button controllers
char buttonUp = 1;
char buttonUpPrev = 1;

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
  switchRead();
  
}

void switchRead(){
  //if switch closed, do this
 
   buttonUp = digitalRead(UP);
   
   if( buttonUp != buttonUpPrev){
      if(buttonUp == LOW){
          Serial.println("Up");
         }
      else if(buttonUp == HIGH){
          Serial.println("Off");
         }
     } 
     
   buttonUpPrev = buttonUp;
}
