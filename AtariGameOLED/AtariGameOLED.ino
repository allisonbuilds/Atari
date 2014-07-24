/*************************************************** 
  This is a example sketch demonstrating bitmap drawing
  for the 1.5" & 1.27" 16-bit Color OLEDs with SSD1331 driver chip

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/1431
  ------> http://www.adafruit.com/products/1673

  If you're using a 1.27" OLED, change SSD1351HEIGHT in Adafruit_SSD1351.h
	to 96 instead of 128

  These displays use SPI to communicate, 4 or 5 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution

  The Adafruit GFX Graphics core library is also required
  https://github.com/adafruit/Adafruit-GFX-Library
  Be sure to install it!
 ****************************************************/
 /*
 This game is written by Allison Berman 07/16/14
 Designed for a 5-button control system such as the Atari joystick
 */

// You can use any 5 pins 
#define sclk 2
#define mosi 3
#define dc   4
#define cs   5
#define rst  6

// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>

//copied from AtariButton since need to be global var
#define UP      A0
#define DOWN    A1
#define LEFT    A2
#define RIGHT   A3
#define BUTTON1 A4
//#define BUTTON2 A5 //only for two-button controllers

//extern tells us it is in another sketch
//need to declare the functions here even though they are defined elsewhere
extern void initButton(void);
extern void buttonInput(void);
extern char switchRead(uint8_t B, char pos);

extern int keyInput(void);

/*not sure if this should be here or not - am i wasting space by 
duplicating an array that will update in both places?
*/
//char lastButtonState[] = {1,1,1,1,1,1};

// Option 1: use any pins but a little slower
//Adafruit_SSD1351 tft = Adafruit_SSD1351(cs, dc, mosi, sclk, rst);  

// Option 2: must use the hardware SPI pins 
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be 
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
Adafruit_SSD1351 tft = Adafruit_SSD1351(cs, dc, rst);

//why pi? why not
float p = 3.1415926;

//movement varibles are global
int xmove = 0;
int ymove = 0;

void setup(void) {
  Serial.begin(115200);
  Serial.print("hello!");
  tft.begin();

  Serial.println("init");
  tft.fillRect(0, 0, 128, 128, BLACK);
  
  uint16_t time = millis();
  //use this for debounce reference
  time = millis() - time;
  Serial.println(time, DEC);
  delay(500);
  
  //this used to be the setup() when AtariButton was standalone
  initButton();
  
  tft.fillScreen(BLACK);
  //center the text in the screen
  tft.setCursor(tft.width()/15, tft.height()/2);
  tft.setTextColor(BLUE);
  tft.setTextSize(1);
  tft.println("The little game that could");
  
  delay(1000);
}

void loop() {

 
  tft.fillScreen(BLACK);
  
  gameSprites();

}



void testdrawtext(char *text, uint16_t color) {
//  tft.setCursor(0,0);
  tft.setTextColor(color);
  tft.print(text);
}



void testdrawrects(uint16_t color) {
 tft.fillScreen(BLACK);
 for (uint16_t x=0; x < tft.height()-1; x+=6) {
   tft.drawRect((tft.width()-1)/2 -x/2, (tft.height()-1)/2 -x/2 , x, x, color);
 }
}

void testfillrects(uint16_t color1, uint16_t color2) {
 tft.fillScreen(BLACK);
 for (uint16_t x=tft.height()-1; x > 6; x-=6) {
   tft.fillRect((tft.width()-1)/2 -x/2, (tft.height()-1)/2 -x/2 , x, x, color1);
   tft.drawRect((tft.width()-1)/2 -x/2, (tft.height()-1)/2 -x/2 , x, x, color2);
 }
}

void testfillcircles(uint8_t radius, uint16_t color) {
  for (uint8_t x=radius; x < tft.width()-1; x+=radius*2) {
    for (uint8_t y=radius; y < tft.height()-1; y+=radius*2) {
      tft.fillCircle(x, y, radius, color);
    }
  }  
}

void testdrawcircles(uint8_t radius, uint16_t color) {
  for (uint8_t x=0; x < tft.width()-1+radius; x+=radius*2) {
    for (uint8_t y=0; y < tft.height()-1+radius; y+=radius*2) {
      tft.drawCircle(x, y, radius, color);
    }
  }  
}

void testtriangles() {
  tft.fillScreen(BLACK);
  int color = 0xF800;
  int t;
  int w = tft.width()/2;
  int x = tft.height();
  int y = 0;
  int z = tft.width();
  for(t = 0 ; t <= 15; t+=1) {
    tft.drawTriangle(w, y, y, x, z, x, color);
    x-=4;
    y+=4;
    z-=4;
    color+=100;
  }
}

void testroundrects() {
  tft.fillScreen(BLACK);
  int color = 100;
  
  int x = 0;
  int y = 0;
  int w = tft.width();
  int h = tft.height();
  for(int i = 0 ; i <= 24; i++) {
    tft.drawRoundRect(x, y, w, h, 5, color);
    x+=2;
    y+=3;
    w-=4;
    h-=6;
    color+=1100;
    Serial.println(i);
  }
}



/*
************************************************************
Game Code Begin
************************************************************
*/

void gameSprites(){
  int xmove = 0;
  int ymove = 0;
  int currentW = 10;
  int currentH = 10;
  int currentX = 50 + xmove;
  int currentY = 50 + ymove;
  
  int moveRight = 0;
  int moveLeft = 0;
  xmove = xmove + moveRight + moveLeft;
  
  int moveUp = 0;
  int moveDown = 0;
  ymove = ymove + moveUp + moveDown;
  
  
//  Serial.println("moveRight");

  //"sound effects"
  tft.setCursor(tft.width()/15, tft.height()/15);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.println("Pew pew pew!");
  
  //move the white dot
  for(int i = 0; i < 20; i++){
    moveUp = switchRead(UP, 1);
    moveDown = -1 * switchRead(DOWN, 2);
    moveRight = switchRead(RIGHT, 3);
    moveLeft = -1 * switchRead(LEFT, 4);
    
    Serial.write('s');
    int movetest = keyInput();
   
    Serial.print(xmove);
    Serial.print(",");
    Serial.println(ymove);
   
    currentX+=xmove;
    currentY+=ymove;
//    tft.fillScreen(BLACK);

  tft.setTextColor(BLUE);
  tft.setCursor(80,100);
  tft.println(movetest);
  tft.setCursor(90,100);
  tft.println(moveDown);
  
//just checking that these update correctly
      xmove++;
//    moveRight++;
//    moveDown++;
      
//to do: set triangle x,y with button input from AtariButton
    tft.fillTriangle(42 + xmove, 20, 42 + xmove, 26, 50 + xmove, 23, RED);
    //fill in behind triangle as it moves
//     tft.fillRect(38 + xmove, 20, 4, 8, BLACK);
     
    tft.fillRect(currentX, currentY,currentW, currentH, WHITE);
    tft.fillRect(currentX, currentY,currentW, currentH, BLACK); //faster than drawing over entire screen
    delay(50);
  }
  delay(500);
}
