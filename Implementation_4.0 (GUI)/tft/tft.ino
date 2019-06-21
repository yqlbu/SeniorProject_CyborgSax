#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>
#include <Bounce.h>

#define E1_outputA 2
#define E1_outputB 3
#define E1_SW 8

#define E2_outputA 5
#define E2_outputB 7
#define E2_SW 4

//#define PIN_SWITCH_1  16
//#define PIN_SWITCH_2  25
//#define PIN_SWITCH_3  26

#define PIN_SWITCH_1  35
#define PIN_SWITCH_2  36
#define PIN_SWITCH_3  37

#define TFT_DC 9              
#define TFT_CS 10             
#define TFT_RST 8             
#define TFT_MISO 12           
#define TFT_MOSI 11           
#define TFT_CLK 13          

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

#define LINECOLOR1 0xEBD5 // purple

// ************* defining modes *************** //
// mode #1 Sound (S)
// mode #2 Lighting (L)
// mode #3 Playback (P)
// mode #4 General (G)

// set the parameters for switch
int mode;
char mode_char;
int f1,f2;
int buttonPress_1 = 0;
int buttonPress_2 = 0;
int buttonPress_3 = 0;

String button_state, last_button_state;

int radius=25;

// defining functions here

int Color_scheme, Gain, Lighting_mode, Master_volume, FX, Dry_wet;


// set the following parameters for overall coordinates of the GUI layout
int global_x = 40; // set default, identical to the F1 rectangle x=40
int global_y = 55;  // set default, identical to the F1 rectangle y=40


// ************ encoder part *************** //

//  encoder A
int counter_A=1;
int buttonPushCounter_1 = 1;   // E1 counter for the number of button presses
int A_buttonState = 0;         // current state of the button
int A_lastButtonState = 0;     // previous state of the button
int A_aState;
int A_aLastState;  

//  encoder B
int counter_B=0;
int buttonPushCounter_2 = 1;   // E2 counter for the number of button presses
int B_buttonState = 0;         // current state of the button
int B_lastButtonState = 0;     // previous state of the button
int B_aState;
int B_aLastState;  

Bounce bouncer_1 = Bounce (E1_outputA , 2); //Initiate a Bounce object with a 2 milisecond debounce time
Bounce bouncer_2 = Bounce (E2_outputA , 2); //Initiate a Bounce object with a 2 milisecond debounce time

void setup(void)
{
    Serial.println("ILI9341 Test!"); 
    tft.begin();
    tft.setRotation(3); //adjust the tft screen for displaying in the right way
    tft.fillScreen(ILI9341_BLACK);

// ***** main function ***** //    
    test(ILI9341_GREEN); // initialize the GUI pattern

    pinMode (E1_outputA,INPUT_PULLUP);
    pinMode (E1_outputB,INPUT_PULLUP);
    // initialize the button pin as a input:
    pinMode (E1_SW,INPUT_PULLUP);

    pinMode (E2_outputA,INPUT_PULLUP);
    pinMode (E2_outputB,INPUT_PULLUP);
    // initialize the button pin as a input:
    pinMode (E2_SW,INPUT_PULLUP);

    // Set switch pins to input mode.
    pinMode(PIN_SWITCH_1, INPUT); 
    pinMode(PIN_SWITCH_2, INPUT_PULLUP); 
    pinMode(PIN_SWITCH_3, INPUT_PULLUP); 

    // Set the mode / Change the mode
    attachInterrupt(digitalPinToInterrupt(PIN_SWITCH_1),mode_1,CHANGE); 
    attachInterrupt(digitalPinToInterrupt(PIN_SWITCH_3),mode_2,CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_SWITCH_2),mode_3,CHANGE);
    
    //default mode
    mode = 1; 

    Serial.begin(9600);
}

void loop()
{
    encoder_1();
    encoder_2();
//    set_switch();
}


// *****************---------- Main ------------**********************

unsigned long test(uint16_t color) {
  unsigned long start;

//  tft.fillScreen(ILI9341_BLACK);
//  tft.fillRect(120, 70, 50, 70, ILI9341_RED);
  start = micros();

// ------- drawing the title text -------------

  int title_x = 20;
  int title_y = 10;

  tft.setCursor(title_x, title_y); //coordination
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
  tft.println("Welcome to the CyborgSax!");

// ------- drawing a circle to indicate current mode -------------

    int c_x = global_x + 45; // default x = 70 = 25 + 45
    int c_y = global_y + 83; // default y = 118 = 35 + 83 
    int r = 25;

    circle(c_x,c_y,r,ILI9341_RED);
//    mode_char='S';
    tft.setCursor(c_x-6, c_y-9); //coordination
    tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
    tft.println('S');

// ---------- drawing the volume bars --------------- 

    volume_bar();

// ----------  creating rectangles for modes display ---------------

// defining the parameters for drawing the rectangles
int rec_1_x = global_x; // x-coordinate default x=25
int rec_1_y = global_y; // y-coordinate default y=35
int rec_1_w = 90; // width
int rec_1_h = 30; // hight

int rec_2_x = global_x; // x-coordinate default x=25
int rec_2_y = global_y+135; // y-coordinate default y=170
int rec_2_w = 90; // width
int rec_2_h = 30; // hight

// mode_1
    rectangle(rec_1_x,rec_1_y,rec_1_w,rec_1_h,ILI9341_WHITE);
    tft.setCursor(rec_1_x+35, rec_1_y+8); //coordination --> y = 43
    tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
    tft.println("F1");
    
// mode_2
    rectangle(rec_2_x,rec_2_y,rec_2_w,rec_2_h,ILI9341_WHITE);
    tft.setCursor(rec_2_x+35, rec_2_y+8); //coordination
    tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
    tft.println("F2");

// ---------- drawing the lines to connect the chosen mode to mode options (WHITE)------------

    connected_lines();

// ------- drawing the encoder (WHITE) -------------

    Encoder_default();


// ------- drawing the target line (RED)-------------

//    chosen_mode();
    
}

// the function to draw rectangles
unsigned long rectangle(int x,int y,int w,int h,uint16_t color)
{
  unsigned long start = micros();
  start = micros();
  
  for (int i=0; i<3; i++)
  {
    tft.drawRect(x,y,w,h,color)  ;
    x++;
    y++;
    w=w-2;
    h=h-2;
  }
  return micros() - start;
}

// the function to draw a square
unsigned long square(int x, int y, int w, int h, uint16_t color)
{
    unsigned long start = micros();
    start = micros();
    
    for (int i=0; i<4; i++)
    {
        tft.drawRect(x,y,w,h,color);
        x++;
        y++;
        w=w-2;
        h=h-2;
    }
    
    return micros() - start;
}

unsigned long circle(int x, int y, int r, uint16_t color)
{
// center Pixel's horizontal position
// center Pixel's vertical position
// Circle Radius
// Line color

    unsigned long start = micros();
    start = micros();

    tft.fillCircle(x,y,r,color);
    
    return micros() - start;
}

// the function to draw lines
unsigned long lines(int x1, int y1, int x2, int y2, uint16_t color)
{
    unsigned long start = micros();
    start = micros();
    
// argument_1: start Pixel's horizontal position
// argument_2: start Pixel's vertical position
// the first two arguments is the initial coordination of the line
// argument_3: end Pixel's horizontal position
// argument_4: end Pixel's horizontal position

    for (int i=0; i<30;i++){
      tft.drawLine(x1,y1,x2,y2,color);
    }

    return micros() - start;
}

void connected_lines()
{
  // defining the parameters for drawing lines
  int l1_x1 = global_x + 45; //default x = 70 = 25 + 45
  int l1_y1 = global_y + 57; // default y = 92 = 35 + 57
  int l1_x2 = global_x + 45; //default x = 70 = 25 + 45
  int l1_y2 = global_y + 30; //default y = 65 = 35 + 30

  int l2_x1 = global_x + 45;
  int l2_y1 = global_y + 109; // default y = 144 = 35 + 109
  int l2_x2 = global_x + 45; 
  int l2_y2 = global_y + 134; // default y = 169 = 35 + 134
    
// vertical lines
    // connected to mode 1
    lines(l1_x1,l1_y1,l1_x2,l1_y2,ILI9341_WHITE);
    lines(l1_x1+1,l1_y1,l1_x2+1,l1_y2,ILI9341_WHITE);
    lines(l1_x1-1,l1_y1,l1_x2-1,l1_y2,ILI9341_WHITE);

    // connected to mode 2
    lines(l2_x1,l2_y1,l1_x2,l2_y2,ILI9341_WHITE);
    lines(l2_x1+1,l2_y1,l1_x2+1,l2_y2,ILI9341_WHITE);
    lines(l2_x1-1,l2_y1,l1_x2-1,l2_y2,ILI9341_WHITE);

}

void volume_bar()
{
// argument_1: start Pixel's horizontal position
// argument_2: start Pixel's vertical position
// the first two arguments is the initial coordination of the rectangle
// argument_3: Rectangle width
// argument_4: Rectangle Height
// #s goes down -> increase (coordination)

    int b1_x=global_x+135, b1_y=global_y+150, w=50, h=10; // default x = 175 = 40 + 135 ; y = 190 = 40 + 150
    int b2_x=global_x +205; // default x = 245 = 40 + 205
    
// ********** bar #1 *********** //

// contour of the bar
    tft.drawRect(b1_x-3,b1_y-64,w+6,h+68, LINECOLOR1);
    
// bottom bar 20%
    tft.drawRect(b1_x,b1_y,w, h, LINECOLOR1);
    tft.drawRect(b1_x+1,b1_y+1,w-2, h-2, LINECOLOR1);
    tft.drawRect(b1_x+2,b1_y+2,w-4, h-4, LINECOLOR1);
    tft.drawRect(b1_x+3,b1_y+3,w-6, h-6, LINECOLOR1);
    tft.drawRect(b1_x+4,b1_y+4,w-8, h-8, LINECOLOR1);
// bottom bar 40%
    tft.drawRect(b1_x,b1_y-15,w, h, LINECOLOR1);
    tft.drawRect(b1_x+1,b1_y-14,w-2, h-2, LINECOLOR1);
    tft.drawRect(b1_x+2,b1_y-13,w-4, h-4, LINECOLOR1);
    tft.drawRect(b1_x+3,b1_y-12,w-6, h-6, LINECOLOR1);
    tft.drawRect(b1_x+4,b1_y-11,w-8, h-8, LINECOLOR1);

    int text1_x = b1_x, text_y = b1_y-84;
// putting text to indicate the % of master volume
    tft.setCursor(text1_x, text_y); //coordination
    tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
    tft.print(counter_A);
    tft.print("%");


// ********** bar #2 *********** //

// contour of the bar
    tft.drawRect(b2_x-3,b1_y-64,w+6,h+68, LINECOLOR1);
    
// bottom bar 20%
    tft.drawRect(b2_x,b1_y,w, h, LINECOLOR1);
    tft.drawRect(b2_x+1,b1_y+1,w-2, h-2, LINECOLOR1);
    tft.drawRect(b2_x+2,b1_y+2,w-4, h-4, LINECOLOR1);
    tft.drawRect(b2_x+3,b1_y+3,w-6, h-6, LINECOLOR1);
    tft.drawRect(b2_x+4,b1_y+4,w-8, h-8, LINECOLOR1);
// bottom bar 40%
    tft.drawRect(b2_x,b1_y-15,w, h, LINECOLOR1);
    tft.drawRect(b2_x+1,b1_y-14,w-2, h-2, LINECOLOR1);
    tft.drawRect(b2_x+2,b1_y-13,w-4, h-4, LINECOLOR1);
    tft.drawRect(b2_x+3,b1_y-12,w-6, h-6, LINECOLOR1);
    tft.drawRect(b2_x+4,b1_y-11,w-8, h-8, LINECOLOR1);
// bottom bar 60%
    tft.drawRect(b2_x,b1_y-30,w, h, LINECOLOR1);
    tft.drawRect(b2_x+1,b1_y-29,w-2, h-2, LINECOLOR1);
    tft.drawRect(b2_x+2,b1_y-28,w-4, h-4, LINECOLOR1);
    tft.drawRect(b2_x+3,b1_y-27,w-6, h-6, LINECOLOR1);
    tft.drawRect(b2_x+4,b1_y-26,w-8, h-8, LINECOLOR1);

    int text2_x = b2_x;
// putting text to indicate the % of master volume
    tft.setCursor(text2_x, text_y); //coordination
    tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
    tft.print(counter_B);
    tft.print("%");
    
    
}

void Encoder_default()
{
  
// ********** Encoder #1 *********** //

    int c1_x = global_x + 157; // default x = 197 = 40 + 157
    int c_y = global_y + 18; // default y = 58 = 40 + 18
    int r = 20;

    circle(c1_x,c_y,r,ILI9341_WHITE);
    
    tft.setCursor(c1_x-8, c_y-7); //coordination
    tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
    tft.println("E1");

// ********** Encoder #2 *********** //

    int c2_x = global_x + 230; // default x = 270 = 40 + 230

    circle(c2_x,c_y,r,ILI9341_WHITE);
    
    tft.setCursor(c2_x-9, c_y-7); //coordination
    tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
    tft.println("E2");

}

// *************** chose the current function ****************** //

void chosen_func()
{
    // defining the parameters for drawing lines

    int rec_1_x = global_x; // x-coordinate default x = 25
    int rec_1_y = global_y; // y-coordinate default y = 35
    int rec_1_w = 90; // width
    int rec_1_h = 30; // hight
    
    int rec_2_x = global_x; // x-coordinate default x=25
    int rec_2_y = global_y+135; // y-coordinate default y=170
    int rec_2_w = 90; // width
    int rec_2_h = 30; // hight

    int l1_x1 = global_x + 45; // x-coordinate default x = 70 = 25 + 45
    int l1_y1 = global_y + 57; // y-coordinate default y = 93 = 35 + 57
    int l1_x2 = global_x + 45; // x-coordinate default x = 70 = 25 + 45
    int l1_y2 = global_y + 30; // y-coordinate default y = 65 = 35 + 30

    int l2_y1 = global_y + 109; // default y = 144 = 35 + 109
    int l2_y2 = global_y + 134; // default y = 169 = 35 + 134
    int l2_x1 = global_x + 45;
    int l2_x2 = global_x + 45; 

// rectangles and text
    if (buttonPushCounter_1 == 1 && buttonPushCounter_2 == 0) // E1 ON && E2 OFF
    {
      // mode_1 ON
        rectangle(rec_1_x,rec_1_y,rec_1_w,rec_1_h,ILI9341_RED);
        tft.setCursor(rec_1_x+35, rec_1_y+8); //coordination --> y = 43
        tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
        tft.println("F1");
        f1=1;
        f2=0;

      // connected to mode 1
        lines(l1_x1,l1_y1,l1_x2,l1_y2,ILI9341_RED);
        lines(l1_x1+1,l1_y1,l1_x2+1,l1_y2,ILI9341_RED);
        lines(l1_x1-1,l1_y1,l1_x2-1,l1_y2,ILI9341_RED);

        // connected to mode 2
        lines(l2_x1,l2_y1,l2_x2,l2_y2,ILI9341_WHITE);
        lines(l2_x1+1,l2_y1,l2_x2+1,l2_y2,ILI9341_WHITE);
        lines(l2_x1-1,l2_y1,l2_x2-1,l2_y2,ILI9341_WHITE);

      // mode_2 OFF
        rectangle(rec_2_x,rec_2_y,rec_2_w,rec_2_h,ILI9341_WHITE);
        tft.setCursor(rec_2_x+35, rec_2_y+8); //coordination
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
        tft.println("F2");
        
    }
    
    else if (buttonPushCounter_1 == 0 && buttonPushCounter_2 == 1) // E1 OFF && E2 ON
    {
      // mode_2 ON
        rectangle(rec_2_x,rec_2_y,rec_2_w,rec_2_h,ILI9341_RED);
        tft.setCursor(rec_2_x+35, rec_2_y+8); //coordination
        tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
        tft.println("F2");

      // connected to mode 2
        lines(l2_x1,l2_y1,l2_x2,l2_y2,ILI9341_RED);
        lines(l2_x1+1,l2_y1,l2_x2+1,l2_y2,ILI9341_RED);
        lines(l2_x1-1,l2_y1,l2_x2-1,l2_y2,ILI9341_RED);

        // connected to mode 1
        lines(l1_x1,l1_y1,l1_x2,l1_y2,ILI9341_WHITE);
        lines(l1_x1+1,l1_y1,l1_x2+1,l1_y2,ILI9341_WHITE);
        lines(l1_x1-1,l1_y1,l1_x2-1,l1_y2,ILI9341_WHITE);

      // mode_1 OFF
        rectangle(rec_1_x,rec_1_y,rec_1_w,rec_1_h,ILI9341_WHITE);
        tft.setCursor(rec_1_x+35, rec_1_y+8); //coordination --> y = 43
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
        tft.println("F1");
    }

    else 
    {
        // mode_2 ON
        rectangle(rec_2_x,rec_2_y,rec_2_w,rec_2_h,ILI9341_WHITE);
        tft.setCursor(rec_2_x+35, rec_2_y+8); //coordination
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
        tft.println("F2");

        // connected to mode 2
        lines(l2_x1,l2_y1,l2_x2,l2_y2,ILI9341_WHITE);
        lines(l2_x1+1,l2_y1,l2_x2+1,l2_y2,ILI9341_WHITE);
        lines(l2_x1-1,l2_y1,l2_x2-1,l2_y2,ILI9341_WHITE);

        // connected to mode 1
        lines(l1_x1,l1_y1,l1_x2,l1_y2,ILI9341_WHITE);
        lines(l1_x1+1,l1_y1,l1_x2+1,l1_y2,ILI9341_WHITE);
        lines(l1_x1-1,l1_y1,l1_x2-1,l1_y2,ILI9341_WHITE);

        // mode_1 OFF
        rectangle(rec_1_x,rec_1_y,rec_1_w,rec_1_h,ILI9341_WHITE);
        tft.setCursor(rec_1_x+35, rec_1_y+8); //coordination --> y = 43
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
        tft.println("F1");
    }


}

void set_encoder_1()
{
    int c1_x = global_x + 157; // default x = 197 = 40 + 157
    int c_y = global_y + 18; // default y = 58 = 40 + 18
    int r = 20;
    int c2_x = global_x + 230; // default x = 270 = 40 + 230
    
    if (buttonPushCounter_1 == 1) // ON
    {
  
      circle(c1_x,c_y,r,ILI9341_RED);
      
      tft.setCursor(c1_x-8, c_y-7); //coordination
      tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
      tft.println("E1");
    }

    else if (buttonPushCounter_1 == 0) // OFF
    {
  
      circle(c1_x,c_y,r,ILI9341_WHITE);
      
      tft.setCursor(c1_x-8, c_y-7); //coordination
      tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
      tft.println("E1");
    }

}

void set_encoder_2()
{
    int c1_x = global_x + 157; // default x = 197 = 40 + 157
    int c_y = global_y + 18; // default y = 58 = 40 + 18
    int r = 20;
    int c2_x = global_x + 230; // default x = 270 = 40 + 230
    
    if (buttonPushCounter_2 == 1) // ON
    {
  
      circle(c2_x,c_y,r,ILI9341_RED);
      
      tft.setCursor(c2_x-9, c_y-7); //coordination
      tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
      tft.println("E2");
    }

    else if (buttonPushCounter_2 == 0) // OFF
    {
  
      circle(c2_x,c_y,r,ILI9341_WHITE);
      
      tft.setCursor(c2_x-9, c_y-7); //coordination
      tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
      tft.println("E2");
    }

}

void bar_chosen_1()
{
  
    int b1_x=global_x+135, b1_y=global_y+150, w=50, h=10; // default x = 175 = 40 + 135 ; y = 190 = 40 + 150

    // ***************** for mode "Color_scheme; Lighting_mode; FX" *****************
    
    if (Color_scheme == 1 || Lighting_mode == 1 || FX == 1){
      if (counter_A == 0)
      {
          // bottom bar 20%
          tft.drawRect(b1_x,b1_y,w, h, LINECOLOR1);
          tft.drawRect(b1_x+1,b1_y+1,w-2, h-2, LINECOLOR1);
          tft.drawRect(b1_x+2,b1_y+2,w-4, h-4, LINECOLOR1);
          tft.drawRect(b1_x+3,b1_y+3,w-6, h-6, LINECOLOR1);
          tft.drawRect(b1_x+4,b1_y+4,w-8, h-8, LINECOLOR1);
          // bottom bar 40%
          tft.drawRect(b1_x,b1_y-15,w, h, ILI9341_BLACK);
          tft.drawRect(b1_x+1,b1_y-14,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b1_x+2,b1_y-13,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b1_x+3,b1_y-12,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b1_x+4,b1_y-11,w-8, h-8, ILI9341_BLACK);
          // bottom bar 60%
          tft.drawRect(b1_x,b1_y-30,w, h, ILI9341_BLACK);
          tft.drawRect(b1_x+1,b1_y-29,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b1_x+2,b1_y-28,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b1_x+3,b1_y-27,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b1_x+4,b1_y-26,w-8, h-8, ILI9341_BLACK);
          // bottom bar 80%
          tft.drawRect(b1_x,b1_y-45,w, h, ILI9341_BLACK);
          tft.drawRect(b1_x+1,b1_y-44,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b1_x+2,b1_y-43,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b1_x+3,b1_y-42,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b1_x+4,b1_y-41,w-8, h-8, ILI9341_BLACK);
          // bottom bar 100% --> the very top bar
          tft.drawRect(b1_x,b1_y-60,w, h, ILI9341_BLACK);
          tft.drawRect(b1_x+1,b1_y-59,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b1_x+2,b1_y-58,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b1_x+3,b1_y-57,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b1_x+4,b1_y-56,w-8, h-8, ILI9341_BLACK);
      }
      else if (counter_A == 1)
      {
          // bottom bar 20%
          tft.drawRect(b1_x,b1_y,w, h, LINECOLOR1);
          tft.drawRect(b1_x+1,b1_y+1,w-2, h-2, LINECOLOR1);
          tft.drawRect(b1_x+2,b1_y+2,w-4, h-4, LINECOLOR1);
          tft.drawRect(b1_x+3,b1_y+3,w-6, h-6, LINECOLOR1);
          tft.drawRect(b1_x+4,b1_y+4,w-8, h-8, LINECOLOR1);
          // bottom bar 40%
          tft.drawRect(b1_x,b1_y-15,w, h, LINECOLOR1);
          tft.drawRect(b1_x+1,b1_y-14,w-2, h-2, LINECOLOR1);
          tft.drawRect(b1_x+2,b1_y-13,w-4, h-4, LINECOLOR1);
          tft.drawRect(b1_x+3,b1_y-12,w-6, h-6, LINECOLOR1);
          tft.drawRect(b1_x+4,b1_y-11,w-8, h-8, LINECOLOR1);
          // bottom bar 60%
          tft.drawRect(b1_x,b1_y-30,w, h, ILI9341_BLACK);
          tft.drawRect(b1_x+1,b1_y-29,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b1_x+2,b1_y-28,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b1_x+3,b1_y-27,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b1_x+4,b1_y-26,w-8, h-8, ILI9341_BLACK);
          // bottom bar 80%
          tft.drawRect(b1_x,b1_y-45,w, h, ILI9341_BLACK);
          tft.drawRect(b1_x+1,b1_y-44,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b1_x+2,b1_y-43,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b1_x+3,b1_y-42,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b1_x+4,b1_y-41,w-8, h-8, ILI9341_BLACK);
          // bottom bar 100% --> the very top bar
          tft.drawRect(b1_x,b1_y-60,w, h, ILI9341_BLACK);
          tft.drawRect(b1_x+1,b1_y-59,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b1_x+2,b1_y-58,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b1_x+3,b1_y-57,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b1_x+4,b1_y-56,w-8, h-8, ILI9341_BLACK);
      }
      else if (counter_A == 2)
      {
          // bottom bar 20%
          tft.drawRect(b1_x,b1_y,w, h, LINECOLOR1);
          tft.drawRect(b1_x+1,b1_y+1,w-2, h-2, LINECOLOR1);
          tft.drawRect(b1_x+2,b1_y+2,w-4, h-4, LINECOLOR1);
          tft.drawRect(b1_x+3,b1_y+3,w-6, h-6, LINECOLOR1);
          tft.drawRect(b1_x+4,b1_y+4,w-8, h-8, LINECOLOR1);
          // bottom bar 40%
          tft.drawRect(b1_x,b1_y-15,w, h, LINECOLOR1);
          tft.drawRect(b1_x+1,b1_y-14,w-2, h-2, LINECOLOR1);
          tft.drawRect(b1_x+2,b1_y-13,w-4, h-4, LINECOLOR1);
          tft.drawRect(b1_x+3,b1_y-12,w-6, h-6, LINECOLOR1);
          tft.drawRect(b1_x+4,b1_y-11,w-8, h-8, LINECOLOR1);
          // bottom bar 60%
          tft.drawRect(b1_x,b1_y-30,w, h, LINECOLOR1);
          tft.drawRect(b1_x+1,b1_y-29,w-2, h-2, LINECOLOR1);
          tft.drawRect(b1_x+2,b1_y-28,w-4, h-4, LINECOLOR1);
          tft.drawRect(b1_x+3,b1_y-27,w-6, h-6, LINECOLOR1);
          tft.drawRect(b1_x+4,b1_y-26,w-8, h-8, LINECOLOR1);
          // bottom bar 80%
          tft.drawRect(b1_x,b1_y-45,w, h, ILI9341_BLACK);
          tft.drawRect(b1_x+1,b1_y-44,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b1_x+2,b1_y-43,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b1_x+3,b1_y-42,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b1_x+4,b1_y-41,w-8, h-8, ILI9341_BLACK);
          // bottom bar 100% --> the very top bar
          tft.drawRect(b1_x,b1_y-60,w, h, ILI9341_BLACK);
          tft.drawRect(b1_x+1,b1_y-59,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b1_x+2,b1_y-58,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b1_x+3,b1_y-57,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b1_x+4,b1_y-56,w-8, h-8, ILI9341_BLACK);
      }
      else if (counter_A == 3)
      {
          // bottom bar 20%
          tft.drawRect(b1_x,b1_y,w, h, LINECOLOR1);
          tft.drawRect(b1_x+1,b1_y+1,w-2, h-2, LINECOLOR1);
          tft.drawRect(b1_x+2,b1_y+2,w-4, h-4, LINECOLOR1);
          tft.drawRect(b1_x+3,b1_y+3,w-6, h-6, LINECOLOR1);
          tft.drawRect(b1_x+4,b1_y+4,w-8, h-8, LINECOLOR1);
          // bottom bar 40%
          tft.drawRect(b1_x,b1_y-15,w, h, LINECOLOR1);
          tft.drawRect(b1_x+1,b1_y-14,w-2, h-2, LINECOLOR1);
          tft.drawRect(b1_x+2,b1_y-13,w-4, h-4, LINECOLOR1);
          tft.drawRect(b1_x+3,b1_y-12,w-6, h-6, LINECOLOR1);
          tft.drawRect(b1_x+4,b1_y-11,w-8, h-8, LINECOLOR1);
          // bottom bar 60%
          tft.drawRect(b1_x,b1_y-30,w, h, LINECOLOR1);
          tft.drawRect(b1_x+1,b1_y-29,w-2, h-2, LINECOLOR1);
          tft.drawRect(b1_x+2,b1_y-28,w-4, h-4, LINECOLOR1);
          tft.drawRect(b1_x+3,b1_y-27,w-6, h-6, LINECOLOR1);
          tft.drawRect(b1_x+4,b1_y-26,w-8, h-8, LINECOLOR1);
          // bottom bar 80%
          tft.drawRect(b1_x,b1_y-45,w, h, LINECOLOR1);
          tft.drawRect(b1_x+1,b1_y-44,w-2, h-2, LINECOLOR1);
          tft.drawRect(b1_x+2,b1_y-43,w-4, h-4, LINECOLOR1);
          tft.drawRect(b1_x+3,b1_y-42,w-6, h-6, LINECOLOR1);
          tft.drawRect(b1_x+4,b1_y-41,w-8, h-8, LINECOLOR1);
          // bottom bar 100% --> the very top bar
          tft.drawRect(b1_x,b1_y-60,w, h, ILI9341_BLACK);
          tft.drawRect(b1_x+1,b1_y-59,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b1_x+2,b1_y-58,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b1_x+3,b1_y-57,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b1_x+4,b1_y-56,w-8, h-8, ILI9341_BLACK);
      }
      else if (counter_A == 4)
      {
          // bottom bar 20%
          tft.drawRect(b1_x,b1_y,w, h, LINECOLOR1);
          tft.drawRect(b1_x+1,b1_y+1,w-2, h-2, LINECOLOR1);
          tft.drawRect(b1_x+2,b1_y+2,w-4, h-4, LINECOLOR1);
          tft.drawRect(b1_x+3,b1_y+3,w-6, h-6, LINECOLOR1);
          tft.drawRect(b1_x+4,b1_y+4,w-8, h-8, LINECOLOR1);
          // bottom bar 40%
          tft.drawRect(b1_x,b1_y-15,w, h, LINECOLOR1);
          tft.drawRect(b1_x+1,b1_y-14,w-2, h-2, LINECOLOR1);
          tft.drawRect(b1_x+2,b1_y-13,w-4, h-4, LINECOLOR1);
          tft.drawRect(b1_x+3,b1_y-12,w-6, h-6, LINECOLOR1);
          tft.drawRect(b1_x+4,b1_y-11,w-8, h-8, LINECOLOR1);
          // bottom bar 60%
          tft.drawRect(b1_x,b1_y-30,w, h, LINECOLOR1);
          tft.drawRect(b1_x+1,b1_y-29,w-2, h-2, LINECOLOR1);
          tft.drawRect(b1_x+2,b1_y-28,w-4, h-4, LINECOLOR1);
          tft.drawRect(b1_x+3,b1_y-27,w-6, h-6, LINECOLOR1);
          tft.drawRect(b1_x+4,b1_y-26,w-8, h-8, LINECOLOR1);
          // bottom bar 80%
          tft.drawRect(b1_x,b1_y-45,w, h, LINECOLOR1);
          tft.drawRect(b1_x+1,b1_y-44,w-2, h-2, LINECOLOR1);
          tft.drawRect(b1_x+2,b1_y-43,w-4, h-4, LINECOLOR1);
          tft.drawRect(b1_x+3,b1_y-42,w-6, h-6, LINECOLOR1);
          tft.drawRect(b1_x+4,b1_y-41,w-8, h-8, LINECOLOR1);
          // bottom bar 100% --> the very top bar
          tft.drawRect(b1_x,b1_y-60,w, h, LINECOLOR1);
          tft.drawRect(b1_x+1,b1_y-59,w-2, h-2, LINECOLOR1);
          tft.drawRect(b1_x+2,b1_y-58,w-4, h-4, LINECOLOR1);
          tft.drawRect(b1_x+3,b1_y-57,w-6, h-6, LINECOLOR1);
          tft.drawRect(b1_x+4,b1_y-56,w-8, h-8, LINECOLOR1);
      }
    }
}

void bar_chosen_2()
{
  
    int b1_x=global_x+135, b1_y=global_y+150, w=50, h=10; // default x = 175 = 40 + 135 ; y = 190 = 40 + 150
    int b2_x=global_x +205; // default x = 245 = 40 + 205
    
    // ***************** for mode "Dry_wet; Master_volume" *****************
    
    if (Dry_wet == 1 || Master_volume == 1){
      if (counter_B > 0 && counter_B <= 20)
      {
          // bottom bar 20%
          tft.drawRect(b2_x,b1_y,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y+1,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y+2,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y+3,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y+4,w-8, h-8, LINECOLOR1);
          // bottom bar 40%
          tft.drawRect(b2_x,b1_y-15,w, h, ILI9341_BLACK);
          tft.drawRect(b2_x+1,b1_y-14,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b2_x+2,b1_y-13,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b2_x+3,b1_y-12,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b2_x+4,b1_y-11,w-8, h-8, ILI9341_BLACK);
          // bottom bar 60%
          tft.drawRect(b2_x,b1_y-30,w, h, ILI9341_BLACK);
          tft.drawRect(b2_x+1,b1_y-29,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b2_x+2,b1_y-28,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b2_x+3,b1_y-27,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b2_x+4,b1_y-26,w-8, h-8, ILI9341_BLACK);
          // bottom bar 80%
          tft.drawRect(b2_x,b1_y-45,w, h, ILI9341_BLACK);
          tft.drawRect(b2_x+1,b1_y-44,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b2_x+2,b1_y-43,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b2_x+3,b1_y-42,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b2_x+4,b1_y-41,w-8, h-8, ILI9341_BLACK);
          // bottom bar 100% --> the very top bar
          tft.drawRect(b2_x,b1_y-60,w, h, ILI9341_BLACK);
          tft.drawRect(b2_x+1,b1_y-59,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b2_x+2,b1_y-58,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b2_x+3,b1_y-57,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b2_x+4,b1_y-56,w-8, h-8, ILI9341_BLACK);
      }
      else if (counter_B > 20 && counter_B <= 40)
      {
          // bottom bar 20%
          tft.drawRect(b2_x,b1_y,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y+1,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y+2,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y+3,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y+4,w-8, h-8, LINECOLOR1);
          // bottom bar 40%
          tft.drawRect(b2_x,b1_y-15,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y-14,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y-13,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y-12,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y-11,w-8, h-8, LINECOLOR1);
          // bottom bar 60%
          tft.drawRect(b2_x,b1_y-30,w, h, ILI9341_BLACK);
          tft.drawRect(b2_x+1,b1_y-29,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b2_x+2,b1_y-28,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b2_x+3,b1_y-27,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b2_x+4,b1_y-26,w-8, h-8, ILI9341_BLACK);
          // bottom bar 80%
          tft.drawRect(b2_x,b1_y-45,w, h, ILI9341_BLACK);
          tft.drawRect(b2_x+1,b1_y-44,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b2_x+2,b1_y-43,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b2_x+3,b1_y-42,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b2_x+4,b1_y-41,w-8, h-8, ILI9341_BLACK);
          // bottom bar 100% --> the very top bar
          tft.drawRect(b2_x,b1_y-60,w, h, ILI9341_BLACK);
          tft.drawRect(b2_x+1,b1_y-59,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b2_x+2,b1_y-58,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b2_x+3,b1_y-57,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b2_x+4,b1_y-56,w-8, h-8, ILI9341_BLACK);
      }
      else if (counter_B > 40 && counter_B <= 60)
      {
          // bottom bar 20%
          tft.drawRect(b2_x,b1_y,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y+1,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y+2,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y+3,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y+4,w-8, h-8, LINECOLOR1);
          // bottom bar 40%
          tft.drawRect(b2_x,b1_y-15,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y-14,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y-13,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y-12,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y-11,w-8, h-8, LINECOLOR1);
          // bottom bar 60%
          tft.drawRect(b2_x,b1_y-30,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y-29,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y-28,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y-27,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y-26,w-8, h-8, LINECOLOR1);
          // bottom bar 80%
          tft.drawRect(b2_x,b1_y-45,w, h, ILI9341_BLACK);
          tft.drawRect(b2_x+1,b1_y-44,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b2_x+2,b1_y-43,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b2_x+3,b1_y-42,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b2_x+4,b1_y-41,w-8, h-8, ILI9341_BLACK);
          // bottom bar 100% --> the very top bar
          tft.drawRect(b2_x,b1_y-60,w, h, ILI9341_BLACK);
          tft.drawRect(b2_x+1,b1_y-59,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b2_x+2,b1_y-58,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b2_x+3,b1_y-57,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b2_x+4,b1_y-56,w-8, h-8, ILI9341_BLACK);
      }
      else if (counter_B > 60 && counter_B <= 80)
      {
          // bottom bar 20%
          tft.drawRect(b2_x,b1_y,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y+1,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y+2,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y+3,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y+4,w-8, h-8, LINECOLOR1);
          // bottom bar 40%
          tft.drawRect(b2_x,b1_y-15,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y-14,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y-13,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y-12,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y-11,w-8, h-8, LINECOLOR1);
          // bottom bar 60%
          tft.drawRect(b2_x,b1_y-30,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y-29,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y-28,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y-27,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y-26,w-8, h-8, LINECOLOR1);
          // bottom bar 80%
          tft.drawRect(b2_x,b1_y-45,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y-44,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y-43,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y-42,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y-41,w-8, h-8, LINECOLOR1);
          // bottom bar 100% --> the very top bar
          tft.drawRect(b2_x,b1_y-60,w, h, ILI9341_BLACK);
          tft.drawRect(b2_x+1,b1_y-59,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b2_x+2,b1_y-58,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b2_x+3,b1_y-57,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b2_x+4,b1_y-56,w-8, h-8, ILI9341_BLACK);
      }
      else if (counter_B > 80 && counter_B < 100)
      {
          // bottom bar 20%
          tft.drawRect(b2_x,b1_y,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y+1,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y+2,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y+3,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y+4,w-8, h-8, LINECOLOR1);
          // bottom bar 40%
          tft.drawRect(b2_x,b1_y-15,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y-14,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y-13,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y-12,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y-11,w-8, h-8, LINECOLOR1);
          // bottom bar 60%
          tft.drawRect(b2_x,b1_y-30,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y-29,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y-28,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y-27,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y-26,w-8, h-8, LINECOLOR1);
          // bottom bar 80%
          tft.drawRect(b2_x,b1_y-45,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y-44,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y-43,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y-42,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y-41,w-8, h-8, LINECOLOR1);
          // bottom bar 100% --> the very top bar
          tft.drawRect(b2_x,b1_y-60,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y-59,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y-58,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y-57,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y-56,w-8, h-8, LINECOLOR1);
      }
    }

// ***************** for mode "Gain" *****************

    else if (Gain == 1 && buttonPushCounter_1 == 0){
      if (counter_B > 0 && counter_B <= 2)
      {
          // bottom bar 20%
          tft.drawRect(b2_x,b1_y,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y+1,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y+2,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y+3,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y+4,w-8, h-8, LINECOLOR1);
          // bottom bar 40%
          tft.drawRect(b2_x,b1_y-15,w, h, ILI9341_BLACK);
          tft.drawRect(b2_x+1,b1_y-14,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b2_x+2,b1_y-13,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b2_x+3,b1_y-12,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b2_x+4,b1_y-11,w-8, h-8, ILI9341_BLACK);
          // bottom bar 60%
          tft.drawRect(b2_x,b1_y-30,w, h, ILI9341_BLACK);
          tft.drawRect(b2_x+1,b1_y-29,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b2_x+2,b1_y-28,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b2_x+3,b1_y-27,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b2_x+4,b1_y-26,w-8, h-8, ILI9341_BLACK);
          // bottom bar 80%
          tft.drawRect(b2_x,b1_y-45,w, h, ILI9341_BLACK);
          tft.drawRect(b2_x+1,b1_y-44,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b2_x+2,b1_y-43,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b2_x+3,b1_y-42,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b2_x+4,b1_y-41,w-8, h-8, ILI9341_BLACK);
          // bottom bar 100% --> the very top bar
          tft.drawRect(b2_x,b1_y-60,w, h, ILI9341_BLACK);
          tft.drawRect(b2_x+1,b1_y-59,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b2_x+2,b1_y-58,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b2_x+3,b1_y-57,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b2_x+4,b1_y-56,w-8, h-8, ILI9341_BLACK);
      }
      else if (counter_B > 2 && counter_B <= 4)
      {
          // bottom bar 20%
          tft.drawRect(b2_x,b1_y,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y+1,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y+2,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y+3,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y+4,w-8, h-8, LINECOLOR1);
          // bottom bar 40%
          tft.drawRect(b2_x,b1_y-15,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y-14,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y-13,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y-12,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y-11,w-8, h-8, LINECOLOR1);
          // bottom bar 60%
          tft.drawRect(b2_x,b1_y-30,w, h, ILI9341_BLACK);
          tft.drawRect(b2_x+1,b1_y-29,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b2_x+2,b1_y-28,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b2_x+3,b1_y-27,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b2_x+4,b1_y-26,w-8, h-8, ILI9341_BLACK);
          // bottom bar 80%
          tft.drawRect(b2_x,b1_y-45,w, h, ILI9341_BLACK);
          tft.drawRect(b2_x+1,b1_y-44,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b2_x+2,b1_y-43,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b2_x+3,b1_y-42,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b2_x+4,b1_y-41,w-8, h-8, ILI9341_BLACK);
          // bottom bar 100% --> the very top bar
          tft.drawRect(b2_x,b1_y-60,w, h, ILI9341_BLACK);
          tft.drawRect(b2_x+1,b1_y-59,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b2_x+2,b1_y-58,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b2_x+3,b1_y-57,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b2_x+4,b1_y-56,w-8, h-8, ILI9341_BLACK);
      }
      else if (counter_B > 4 && counter_B <= 6)
      {
          // bottom bar 20%
          tft.drawRect(b2_x,b1_y,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y+1,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y+2,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y+3,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y+4,w-8, h-8, LINECOLOR1);
          // bottom bar 40%
          tft.drawRect(b2_x,b1_y-15,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y-14,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y-13,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y-12,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y-11,w-8, h-8, LINECOLOR1);
          // bottom bar 60%
          tft.drawRect(b2_x,b1_y-30,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y-29,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y-28,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y-27,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y-26,w-8, h-8, LINECOLOR1);
          // bottom bar 80%
          tft.drawRect(b2_x,b1_y-45,w, h, ILI9341_BLACK);
          tft.drawRect(b2_x+1,b1_y-44,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b2_x+2,b1_y-43,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b2_x+3,b1_y-42,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b2_x+4,b1_y-41,w-8, h-8, ILI9341_BLACK);
          // bottom bar 100% --> the very top bar
          tft.drawRect(b2_x,b1_y-60,w, h, ILI9341_BLACK);
          tft.drawRect(b2_x+1,b1_y-59,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b2_x+2,b1_y-58,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b2_x+3,b1_y-57,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b2_x+4,b1_y-56,w-8, h-8, ILI9341_BLACK);
      }
      else if (counter_B > 6 && counter_B <= 8)
      {
          // bottom bar 20%
          tft.drawRect(b2_x,b1_y,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y+1,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y+2,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y+3,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y+4,w-8, h-8, LINECOLOR1);
          // bottom bar 40%
          tft.drawRect(b2_x,b1_y-15,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y-14,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y-13,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y-12,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y-11,w-8, h-8, LINECOLOR1);
          // bottom bar 60%
          tft.drawRect(b2_x,b1_y-30,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y-29,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y-28,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y-27,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y-26,w-8, h-8, LINECOLOR1);
          // bottom bar 80%
          tft.drawRect(b2_x,b1_y-45,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y-44,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y-43,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y-42,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y-41,w-8, h-8, LINECOLOR1);
          // bottom bar 100% --> the very top bar
          tft.drawRect(b2_x,b1_y-60,w, h, ILI9341_BLACK);
          tft.drawRect(b2_x+1,b1_y-59,w-2, h-2, ILI9341_BLACK);
          tft.drawRect(b2_x+2,b1_y-58,w-4, h-4, ILI9341_BLACK);
          tft.drawRect(b2_x+3,b1_y-57,w-6, h-6, ILI9341_BLACK);
          tft.drawRect(b2_x+4,b1_y-56,w-8, h-8, ILI9341_BLACK);
      }
      else if (counter_B > 8 && counter_B < 10)
      {
          // bottom bar 20%
          tft.drawRect(b2_x,b1_y,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y+1,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y+2,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y+3,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y+4,w-8, h-8, LINECOLOR1);
          // bottom bar 40%
          tft.drawRect(b2_x,b1_y-15,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y-14,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y-13,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y-12,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y-11,w-8, h-8, LINECOLOR1);
          // bottom bar 60%
          tft.drawRect(b2_x,b1_y-30,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y-29,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y-28,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y-27,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y-26,w-8, h-8, LINECOLOR1);
          // bottom bar 80%
          tft.drawRect(b2_x,b1_y-45,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y-44,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y-43,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y-42,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y-41,w-8, h-8, LINECOLOR1);
          // bottom bar 100% --> the very top bar
          tft.drawRect(b2_x,b1_y-60,w, h, LINECOLOR1);
          tft.drawRect(b2_x+1,b1_y-59,w-2, h-2, LINECOLOR1);
          tft.drawRect(b2_x+2,b1_y-58,w-4, h-4, LINECOLOR1);
          tft.drawRect(b2_x+3,b1_y-57,w-6, h-6, LINECOLOR1);
          tft.drawRect(b2_x+4,b1_y-56,w-8, h-8, LINECOLOR1);
      }
    }
    
}



// ***************** debouncing encoder_1 ***************** //

void encoder_1()
{
     // set the initial parameters for the parameters bar
     int b1_x=global_x+135, b1_y=global_y+150, w=50, h=10; // default x = 175 = 40 + 135 ; y = 190 = 40 + 150
     int text1_x = b1_x, text_y = b1_y-84;

     int overwrite_x = global_x + 130; // default x = 170 = 40 + 130
     int overwrite_y = global_y + 65;// default y = 105 = 40 + 65

     // Update the debouncer
     bouncer_1.update();
     
     A_aState = digitalRead(E1_outputA); // Reads the "current" state of the outputA
     A_buttonState = digitalRead(E1_SW); // read the state of the button

     // *********** differentiating different functions *********** //

     if (Color_scheme == 1){
        counter_A = constrain(counter_A,1,3); // 0-4
     }
     else if (Gain == 1){
        counter_A = constrain(counter_A,1,9); // 0-10
     }
     else if (Lighting_mode == 1){
        counter_A = constrain(counter_A,1,9); // 0-10
     }
     else if (Master_volume == 1){
        counter_A = constrain(counter_A,1,99); // 1-99
     }     
     else if (FX == 1){
        counter_A = constrain(counter_A,1,2); // 0-3
     } 
     else if (Dry_wet == 1){
        counter_A = constrain(counter_A,1,99); // 1-99
     }   
     
     if (bouncer_1.risingEdge() && buttonPushCounter_1 == 1){
       // If the previous and the current state of the outputA are different, that means a Pulse has occured
       if (A_aState != A_aLastState){     
         // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
         // rewrite the text in the background color
         tft.fillRect(overwrite_x, overwrite_y, 40, 20, ILI9341_BLACK);

         if (digitalRead(E1_outputB) != A_aState) { 
           counter_A ++;
         } 
         else {
           counter_A --;
         }

         // make response to the parameter bar
         bar_chosen_1();
         
         Serial.print("Position: ");
         Serial.println(counter_A);

// ***          volume bar part        ***    //

         // overwrite the encoder value as text on top of the bar
         tft.setCursor(text1_x, text_y); //coordination
         tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
         tft.print(counter_A);
         tft.print("%");
              
       }
     }
     A_aLastState = A_aState; // Updates the previous state of the outputA with the current state
     
// *****           button push          ***** //

     if (A_buttonState != A_lastButtonState) {
        // if the state has changed, increment the counter
        if (A_buttonState == HIGH) 
        {
            // if the current state is HIGH then the button went from off to on:
            buttonPushCounter_1 = (buttonPushCounter_1 + 1) % 2;
            f1 = 1;
            f2 = 0;
            Serial.println("Encoder #1 pushed!");
            Serial.print("mode: ");
            Serial.println(buttonPushCounter_1);

            function_selection();
            Serial.print("Current mode: ");
            Serial.println(mode);
            Serial.print("F1: ");
            Serial.println(f1);
            Serial.print("F2: ");
            Serial.println(f2);

            // check the current mode and current function
            set_encoder_1();
            chosen_func();
            delay(300);
        }
        
        // Delay a little bit to avoid bouncing
        delay(50);
      }
      // save the current state as the last state, for next time through the loop
      A_lastButtonState = A_buttonState;
}


// ***************** debouncing encoder_2 ***************** //

void encoder_2()
{         
     int b1_x=global_x+135, b1_y=global_y+150, w=50, h=10; // default x = 175 = 40 + 135 ; y = 190 = 40 + 150
     int b2_x=global_x +205; // default x = 245 = 40 + 205 
     int text_y = b1_y-84;
     int text2_x = b2_x;

     int overwrite_x = global_x + 200; // default x = 240 = 40 + 200
     int overwrite_y = global_y + 65;// default y = 105 = 40 + 65
     
     // Update the debouncer
     bouncer_2.update();
     
     B_aState = digitalRead(E2_outputA); // Reads the "current" state of the outputA
     B_buttonState = digitalRead(E2_SW); // read the state of the button

// *********** differentiating different functions *********** //

     if (Color_scheme == 1){
        counter_B = constrain(counter_B,1,3); // 0-4
     }
     else if (Gain == 1){
        counter_B = constrain(counter_B,1,9); // 0-10
     }
     else if (Lighting_mode == 1){
        counter_B = constrain(counter_B,1,9); // 0-10
     }
     else if (Master_volume == 1){
        counter_B = constrain(counter_B,1,99); // 1-99
     }     
     else if (FX == 1){
        counter_B = constrain(counter_B,1,2); // 0-3
     } 
     else if (Dry_wet == 1){
        counter_B = constrain(counter_B,1,99); // 1-99
     }             

     if (bouncer_2.risingEdge() && buttonPushCounter_2 == 1){
       // If the previous and the current state of the outputA are different, that means a Pulse has occured
       if (B_aState != B_aLastState){     
         // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
         // rewrite the text in the background color
         tft.fillRect(overwrite_x, overwrite_y, 40, 20, ILI9341_BLACK);

         if (digitalRead(E2_outputB) != B_aState) { 
           counter_B ++;
         } 
         else {
           counter_B --;
         }

         // make response to the parameter bar
         bar_chosen_2();
         
         Serial.print("Position: ");
         Serial.println(counter_B);

// ***          volume bar part        ***    //

         // overwrite the encoder value as text on top of the bar
         tft.setCursor(text2_x, text_y); //coordination
         tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
         tft.print(counter_B);
         tft.print("%");
              
       }
     }
     B_aLastState = B_aState; // Updates the previous state of the outputA with the current state
     
         
// *****           button push          ***** //

     if (B_buttonState != B_lastButtonState) {
        // if the state has changed, increment the counter
        if (B_buttonState == HIGH) 
        {
            // if the current state is HIGH then the button went from off to on:
            buttonPushCounter_2 = (buttonPushCounter_2 + 1) % 2;
            f2 = 1;
            f1 = 0;
            Serial.println("Encoder #2 pushed!");
            Serial.print("mode: ");
            Serial.println(buttonPushCounter_2);
            
            function_selection();
            Serial.print("Current mode: ");
            Serial.println(mode);
            Serial.print("F1: ");
            Serial.println(f1);
            Serial.print("F2: ");
            Serial.println(f2);

            // check the current mode and current function
            set_encoder_2();
            chosen_func();
            delay(300);
        }
        
        // Delay a little bit to avoid bouncing
        delay(50);
      }
      // save the current state as the last state, for next time through the loop
      B_lastButtonState = B_buttonState;
}


// *****----------------           4-position switch          ---------------- ***** //

void mode_1()
{   
    buttonPress_1 = digitalRead(PIN_SWITCH_1);
    buttonPress_2 = digitalRead(PIN_SWITCH_2);
    buttonPress_3 = digitalRead(PIN_SWITCH_3);  
    button_state = String(String(buttonPress_1) + String(buttonPress_2) + String(buttonPress_3));
    
    if (button_state != last_button_state){
      // 1(011)
      if(digitalRead(PIN_SWITCH_1) == LOW && digitalRead(PIN_SWITCH_2) == HIGH && digitalRead(PIN_SWITCH_3) == HIGH){
          // overwrite the previous mode
          circle(global_x + 45,global_y + 83, radius,ILI9341_RED);
          mode=1;
          mode_char='S';
          button_state = String("011");
          tft.setCursor(global_x + 39, global_y + 74); //coordination
          tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
          tft.println(mode_char);
          delay(20); 
      }
      else{
          circle(global_x + 45,global_y + 83, radius,ILI9341_RED);
          mode_char='G';
          button_state = String("111");
          tft.setCursor(global_x + 39, global_y + 74); //coordination
          tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
          tft.println(mode_char);
          delay(20); 
      }
      delay(20);
    }
    last_button_state = button_state;
}

void mode_3()
{
    buttonPress_1 = digitalRead(PIN_SWITCH_1);
    buttonPress_2 = digitalRead(PIN_SWITCH_2);
    buttonPress_3 = digitalRead(PIN_SWITCH_3);  
    button_state = String(String(buttonPress_1) + String(buttonPress_2) + String(buttonPress_3));

    if (button_state != last_button_state){
      // 3(101)
      if(digitalRead(PIN_SWITCH_1) == HIGH && digitalRead(PIN_SWITCH_2) == LOW && digitalRead(PIN_SWITCH_3) == HIGH){
          // overwrite the previous mode
          circle(global_x + 45,global_y + 83, radius,ILI9341_RED);
          mode=3;
          mode_char='P';
          button_state = String("101");
          tft.setCursor(global_x + 39, global_y + 74); //coordination
          tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
          tft.println(mode_char);
          delay(20); 
      }
      else{
          circle(global_x + 45,global_y + 83, radius,ILI9341_RED);
          mode=4;
          mode_char='G';
          button_state = String("111");
          tft.setCursor(global_x + 39, global_y + 74); //coordination
          tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
          tft.println(mode_char);
          delay(20); 
      }
      delay(20);
    }
    last_button_state = button_state;
    
}

void mode_2()
{  
    buttonPress_1 = digitalRead(PIN_SWITCH_1);
    buttonPress_2 = digitalRead(PIN_SWITCH_2);
    buttonPress_3 = digitalRead(PIN_SWITCH_3);  
    button_state = String(String(buttonPress_1) + String(buttonPress_2) + String(buttonPress_3));

    if (button_state != last_button_state){  
      // 011 -> 2(110) [Unique Case]
      if(digitalRead(PIN_SWITCH_3) == LOW){
          // overwrite the previous mode
          circle(global_x + 45,global_y + 83, radius,ILI9341_RED);
          mode=2;
          mode_char='L';
          button_state = String("110");
          tft.setCursor(global_x + 39, global_y + 74); //coordination
          tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
          tft.println(mode_char);
          delay(20); 
      }
      else{
          circle(global_x + 45,global_y + 83, radius,ILI9341_RED);
          mode=4;
          mode_char='G';
          button_state = String("111");
          tft.setCursor(global_x + 39, global_y + 74); //coordination
          tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
          tft.println(mode_char);
          delay(20); 
      }
      delay(20);
    }
    last_button_state = button_state;
}


// ------------------------- select the function ------------------------------- //
     
void function_selection(){
  
// **************** Color_scheme **************** //
    if (mode == 2 && f1 == 1){
        Color_scheme = 1;
        Gain = 0;
        Lighting_mode = 0;
        Master_volume = 0;
        FX = 0;
        Dry_wet = 0;
        Serial.println("Color_scheme is ON!"); 
    }  
// **************** Gain **************** //
    else if (mode == 2 && f2 == 1){
        Color_scheme = 0;
        Gain = 1;
        Lighting_mode = 0;
        Master_volume = 0;
        FX = 0;
        Dry_wet = 0; 
        Serial.println("Gain is ON!");
    } 
// **************** Lighting_mode **************** //
    else if (mode == 4 && f1 == 1){
        Color_scheme = 0;
        Gain = 0;
        Lighting_mode = 1;
        Master_volume = 0;
        FX = 0;
        Dry_wet = 0; 
        Serial.println("Lighting_mode is ON!");
    }   
// **************** Master_volume **************** //
    else if (mode == 4 && f2 == 1){
        Color_scheme = 0;
        Gain = 0;
        Lighting_mode = 0;
        Master_volume = 1;
        FX = 0;
        Dry_wet = 0; 
        Serial.println("Master_volume is ON!");
    }  
// **************** FX **************** //
    else if (mode == 1 && f1 == 1){
        Color_scheme = 0;
        Gain = 0;
        Lighting_mode = 0;
        Master_volume = 0;
        FX = 1;
        Dry_wet = 0; 
        Serial.println("FX is ON!");
    } 
// **************** Dry_wet **************** //
    else if (mode == 1 && f2 == 1){
        Color_scheme = 0;
        Gain = 0;
        Lighting_mode = 0;
        Master_volume = 0;
        FX = 0;
        Dry_wet = 1; 
        Serial.println("Dry_wet is ON!");
    }  
                       
}
