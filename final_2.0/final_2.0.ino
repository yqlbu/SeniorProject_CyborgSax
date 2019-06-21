#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>
#include <Bounce.h>
#include <WS2812Serial.h>

#define  RED_1  0x160000 //Red
#define  GREEN_1  0x001600 //Green
#define  BLUE_1  0x000016 //Blue
#define  YELLOW_1 0x101400 //Yellow
#define  PINK_1    0x120009 //Pink
#define  ORANGE_1 0x100400 //Orange
#define  WHITE_1  0x101010 //White

// Brighter Colors
#define OFF    0x000000 
#define RED    0xFF0000
#define GREEN  0x00FF00
#define BLUE   0x0000FF
#define YELLOW 0xFFFF00
#define PINK   0xFF1088
#define ORANGE 0xE05800
#define WHITE  0xFFFFFF

#define LINECOLOR1 0xEBD5 // purple

//#define SW_1 37
//#define outputA_1 29
//#define outputB_1 30
//#define SW_2 37 
//#define outputA_2 29
//#define outputB_2 30

#define E1_outputA 2
#define E1_outputB 3
#define E1_SW 8

#define E2_outputA 5
#define E2_outputB 7
#define E2_SW 4

#define PIN_SWITCH_1  35
#define PIN_SWITCH_2  36
#define PIN_SWITCH_3  37

#define TFT_DC 20              
#define TFT_CS 21             
#define TFT_RST 255             
#define TFT_MISO 12           
#define TFT_MOSI 7           
#define TFT_SCK 14 

//#define TFT_DC 9              
//#define TFT_CS 10             
//#define TFT_RST 255             
//#define TFT_MISO 12           
//#define TFT_MOSI 11           
//#define TFT_SCK 13

#define FLANGE_DELAY_LENGTH (12*AUDIO_BLOCK_SAMPLES)
short delayline[FLANGE_DELAY_LENGTH];
int s_idx = 3*FLANGE_DELAY_LENGTH/4;
//int s_depth = FLANGE_DELAY_LENGTH/8;
double s_freq = .0625;


const int numled = 128;
const int pin = 26;

int f1,f2;
int buttonPress_1 = 0;
int buttonPress_2 = 0;
int buttonPress_3 = 0;

// defining functions here

int Color_scheme, Gain, Lighting_mode, Master_volume, FX, Dry_wet;


// set the following parameters for overall coordinates of the GUI layout
int global_x = 40; // set default, identical to the F1 rectangle x=40
int global_y = 55;  // set default, identical to the F1 rectangle y=40


// ************ encoder part *************** //

//  encoder A
int counter_A = 0;
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

int mode;  
bool fft = true;

int Color_1, Color_2, Color_3;

byte drawingMemory[numled*3];         //  3 bytes per LED
DMAMEM byte displayMemory[numled*12]; // 12 bytes per LED

WS2812Serial leds(numled, displayMemory, drawingMemory, pin, WS2812_GRB);

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST, TFT_MISO);



// GUItool: begin automatically generated code
AudioInputI2S            Mic;           //xy=107,162
AudioPlaySdWav           playSdWav1;     //xy=120,209
AudioMixer4              mixer1;         //xy=314,178
AudioAmplifier           amp1;           //xy=457,315
AudioEffectFade          fade1;          //xy=525,149
AudioEffectFlange        flange1;        //xy=527,252
AudioEffectReverb        reverb1;        //xy=535,106
AudioAnalyzeFFT256       fft256_1;       //xy=632,314
AudioAmplifier           amp2;           //xy=705,180
AudioOutputI2S           i2s1;           //xy=885,182
AudioConnection          patchCord1(Mic, 0, mixer1, 0);
AudioConnection          patchCord2(Mic, 1, mixer1, 1);
AudioConnection          patchCord3(playSdWav1, 0, mixer1, 2);
AudioConnection          patchCord4(playSdWav1, 1, mixer1, 3);
AudioConnection          patchCord5(mixer1, amp2);
AudioConnection          patchCord6(mixer1, amp1);
AudioConnection          patchCord7(mixer1, reverb1);
AudioConnection          patchCord9(mixer1, flange1);
AudioConnection          patchCord10(mixer1, fade1);
AudioConnection          patchCord11(amp1, fft256_1);
AudioConnection          patchCord12(amp2, 0, i2s1, 0);
AudioConnection          patchCord13(amp2, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=399,357
// GUItool: end automatically generated code



// Use these with the Teensy 3.5 & 3.6 SD card
#define SDCARD_CS_PIN    BUILTIN_SDCARD
#define SDCARD_MOSI_PIN  11  // not actually used
#define SDCARD_SCK_PIN   13  // not actually used

void setup() {
  // put your setup code here, to run once:
  
    Serial.begin(9600);
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


   leds.begin();
  AudioMemory(10);
  sgtl5000_1.enable();
  sgtl5000_1.volume(.7);
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000_1.micGain(20);
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  sgtl5000_1.adcHighPassFilterDisable();
  mixer1.gain(0, 10);
  mixer1.gain(1, 10);
  mixer1.gain(2, 0);
  mixer1.gain(3, 0);
  delay(1000);
  playSdWav1.play("AFRICA.WAV");
 
    // Reads the initial state of the outputA
//    A_aLastState = digitalRead(E1_outputA); 
    mode = 2; 
}

float n, m, w, a, b, c, d, e, f, g, h, i, j, k, l, o, p, q, r, s, t, u, v, x, y, mm, nn, ww, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az, ba, bb, bc, bd, be, bf;

int count = 0;
int iteration = 1;

int cs1, cs2, cs3, cs4, cs5, cs6, cs7, cs8, cs9, cs10, cs11, cs12, cs13, cs14, cs15, cs16;
int param1, param2;

void loop() {
    encoder_1();
    encoder_2();
    if (fft == true)
      runFFT();
    lightingOptions(counter_A, counter_B);
}

void soundOptions(int param1, int param2) {
  if (param1 == 0){
    AudioConnection regConnection(mixer1, amp2);
  }
  else if (param1 == 1){
    AudioConnection reverbConnection(reverb1, amp2);
    reverb1.reverbTime(param2);
  }
  else if (param1 == 2){
    AudioConnection flangeConnection(flange1, amp2);
    flange1.begin(delayline,FLANGE_DELAY_LENGTH,s_idx,FLANGE_DELAY_LENGTH/param2,s_freq);
  }
  
}

void lightingOptions(int param1, int param2) {
  if (param1 == 0){
    Color_1 = GREEN;
    Color_2 = YELLOW;
    Color_3 = RED;
  }
  else if (param1 == 1){
    Color_1 = BLUE;
    Color_2 = ORANGE;
    Color_3 = GREEN;
  }
  else if (param1 == 2){
    Color_1 = PINK;
    Color_2 = WHITE;
    Color_3 = ORANGE;
  }
  else if (param1 == 3){
    Color_1 = ORANGE;
    Color_2 = BLUE;
    Color_3 = PINK;
  }
  else if (param1 == 4){
    Color_1 = RED;
    Color_2 = ORANGE;
    Color_3 = WHITE;
  }

  amp1.gain(param2);
}

void generalOptions(int param1, int param2) {
  if (param1 == 1)
    fft = false;
  else 
    fft = true;
    
  amp2.gain(param2);
}

void offMode() {
  
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
    tft.println('L');

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
        counter_A = constrain(counter_A,1,4); // 0-4
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


// *****           4-position switch          ***** //

//void set_switch()
//{
//    
//    int c_x = global_x + 45; // default x = 70 = 25 + 45
//    int c_y = global_y + 83; // default y = 118 = 35 + 83 
//    int r = 25;
//    
//    char mode_char;
//
//    int previous_mode=0;
//    
////       circle(c_x,c_y,r,ILI9341_RED);
////    tft.setCursor(c_x-6, c_y-9); //coordination
////    tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
////    tft.println(mode_char);
//    
//    buttonPress_1 = digitalRead(PIN_SWITCH_1);
//    buttonPress_2 = digitalRead(PIN_SWITCH_2);
//    buttonPress_3 = digitalRead(PIN_SWITCH_3);
//
////    if (previous_mode != mode){
////
////      circle(c_x,c_y,r,ILI9341_RED);
//      
//      // #2 -> #1
//      if (buttonPress_1 == HIGH && buttonPress_2 == HIGH && buttonPress_3 == LOW){
//        delay(1000);
//        buttonPress_1 = digitalRead(PIN_SWITCH_1);
//        buttonPress_2 = digitalRead(PIN_SWITCH_2);
//        buttonPress_3 = digitalRead(PIN_SWITCH_3);
//        // (1,1,1)
//        if (buttonPress_1 == LOW && buttonPress_2 == HIGH && buttonPress_3 == HIGH){
//          Serial.println("BANG!_1");
//          mode=1;
//          mode_char='S';
//          tft.setCursor(c_x-6, c_y-9); //coordination
//          tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
//          tft.println(mode_char);
//        }
//        
//        // #2 -> #3
//        else if (buttonPress_1 == HIGH && buttonPress_2 == LOW && buttonPress_3 == HIGH){
//          Serial.println("BANG!_3");
//          mode=3;
//          mode_char='P';
//          tft.setCursor(c_x-6, c_y-9); //coordination
//          tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
//          tft.println(mode_char);
//        }
//      }
//      
//      // #1 -> #2
//      else if (buttonPress_1 == LOW && buttonPress_2 == HIGH && buttonPress_3 == HIGH){
//        delay(1000);
//        buttonPress_1 = digitalRead(PIN_SWITCH_1);
//        buttonPress_2 = digitalRead(PIN_SWITCH_2);
//        buttonPress_3 = digitalRead(PIN_SWITCH_3);
//        // (1,1,0)
//        if (buttonPress_1 == HIGH && buttonPress_2 == HIGH && buttonPress_3 == LOW){
//          Serial.println("BANG!_2");
//          mode=2;
//  //        mode_char='L';
//  //        tft.setCursor(c_x-6, c_y-9); //coordination
//  //        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
//  //        tft.println(mode_char);
//        }
//      }
//      
//      // #3 -> #2
//      else if (buttonPress_1 == HIGH && buttonPress_2 == LOW && buttonPress_3 == HIGH){
//        delay(1000);
//        buttonPress_1 = digitalRead(PIN_SWITCH_1);
//        buttonPress_2 = digitalRead(PIN_SWITCH_2);
//        buttonPress_3 = digitalRead(PIN_SWITCH_3);
//        // (1,1,0)
//        if (buttonPress_1 == HIGH && buttonPress_2 == HIGH && buttonPress_3 == LOW){
//          Serial.println("BANG!_2");
//          mode=2;
//  //        mode_char='L';
//  //        tft.setCursor(c_x-6, c_y-9); //coordination
//  //        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
//  //        tft.println(mode_char);
//        }
//        // #3 -> #4
//        else if (buttonPress_1 == HIGH && buttonPress_2 == HIGH && buttonPress_3 == HIGH){
//          Serial.println("BANG!_4");
//          mode=4;
//  //        mode_char='G';
//  //        tft.setCursor(c_x-6, c_y-9); //coordination
//  //        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
//  //        tft.println(mode_char);
//        }
//      }
//      
//      // #4 -> #3
//      else if (buttonPress_1 == HIGH && buttonPress_2 == HIGH && buttonPress_3 == HIGH){
//        delay(1000);
//        buttonPress_1 = digitalRead(PIN_SWITCH_1);
//        buttonPress_2 = digitalRead(PIN_SWITCH_2);
//        buttonPress_3 = digitalRead(PIN_SWITCH_3);
//        // (1,1,0)
//        if (buttonPress_1 == HIGH && buttonPress_2 == LOW && buttonPress_3 == HIGH){
//          Serial.println("BANG!_3");
//          mode=3;
//  //        mode_char='P';
//  //        tft.setCursor(c_x-6, c_y-9); //coordination
//  //        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
//  //        tft.println(mode_char);
//        }
//      }
////    }
////    previous_mode=mode;
//
//}
     
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

void runFFT(){
  int microsec = 100000 / leds.numPixels();
  count++;
  if (fft256_1.available()) {
      
      nn = fft256_1.read(1);
      mm= fft256_1.read(2);
      ww = nn+mm;
      a = fft256_1.read(3);
      b = fft256_1.read(4);
      c = a + b;
      d = fft256_1.read(5);
      e = fft256_1.read(6);
      f = e + d;
      g = fft256_1.read(7);
      h = fft256_1.read(8);
      i = g + h;
      j = fft256_1.read(9);
      k = fft256_1.read(10);
      l = fft256_1.read(11);
      m = j + k + l;
      n = fft256_1.read(12);
      o = fft256_1.read(13);
      p = fft256_1.read(14);
      q = n + o + p;
      r = fft256_1.read(15);
      s = fft256_1.read(16);
      t = fft256_1.read(17);
      u = r + s + t;
      v = fft256_1.read(18);
      w = fft256_1.read(19);
      x = fft256_1.read(20);
      y = v + w + x;
      z = fft256_1.read(21);
      aa = fft256_1.read(22);
      ab = fft256_1.read(23);
      ac = z + aa + ab;
      ad = fft256_1.read(24);
      ae = fft256_1.read(25);
      af = fft256_1.read(26);
      ag = ad + ae + af;
      ah = fft256_1.read(27);
      ai = fft256_1.read(28);
      aj = fft256_1.read(29);
      ak = ah + ai + aj;     
      al = fft256_1.read(30);
      am = fft256_1.read(31);
      an = fft256_1.read(32);
      ao = al + am + an;
      ap = fft256_1.read(33);
      aq = fft256_1.read(34);
      ar = fft256_1.read(35);
      as = ap + aq + ar;
      at = fft256_1.read(36);
      au = fft256_1.read(37);
      av = fft256_1.read(38);
      aw = at + au + av;
      ax = fft256_1.read(39);
      ay = fft256_1.read(40);
      az = fft256_1.read(41);
      ba = ax + ay + az;
      bb = fft256_1.read(42);
      bc = fft256_1.read(43);
      bd = fft256_1.read(44);
      be = fft256_1.read(45);
      bf = bb + bc + bd + be;
           
     printNumber(bf);
      printNumber(c);
      printNumber(f);
      printNumber(i);
      printNumber(m);
      printNumber(q);
      printNumber(u);
      printNumber(y);

     Serial.println();
    }

    if (count == 1000){
      count = 0;
      
      Bar1(microsec,ww,iteration);
      Bar2(microsec,c,iteration);
      Bar3(microsec,f,iteration);
      Bar4(microsec,i,iteration);
      Bar5(microsec,m,iteration);
      Bar6(microsec,q,iteration);
      Bar7(microsec,u,iteration);
      Bar8(microsec,y,iteration); 
      Bar9(microsec,ac,iteration);
      Bar10(microsec,ag,iteration);
      Bar11(microsec,ak,iteration);
      Bar12(microsec,ao,iteration);
      Bar13(microsec,as,iteration);
      Bar14(microsec,aw,iteration);
      Bar15(microsec,ba,iteration);
      Bar16(microsec,bf,iteration);

    }

    iteration++;
}

void Bar1(int wait, float n, int i) {
  if (i == 1000){
    if (n >= 0.00 and n <= 0.07) {
        leds.setPixel(0,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs1=1;
    }
    else if (n >= 0.07 and n <= 0.12) {
        leds.setPixel(0,Color_1);
        leds.setPixel(15,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs1=2;
    }
     else if (n >= 0.121 and n <= 0.16) {
        leds.setPixel(0,Color_1);
        leds.setPixel(15,Color_1);
        leds.setPixel(16,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs1=3;
    }
      else if (n >= 0.161 and n <= 0.2) {
        leds.setPixel(0,Color_1);
        leds.setPixel(15,Color_1);
        leds.setPixel(16,Color_1);
        leds.setPixel(31,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs1=4;
    } 
    else if (n >= 0.201 and n <= 0.24) {
        leds.setPixel(0,Color_1);
        leds.setPixel(15,Color_1);
        leds.setPixel(16,Color_1);
        leds.setPixel(31,Color_1);
        leds.setPixel(32,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs1=5;
    } 
    else if (n >= 0.241 and n <= 0.26) {
        leds.setPixel(0,Color_1);
        leds.setPixel(15,Color_1);
        leds.setPixel(16,Color_1);
        leds.setPixel(31,Color_1);
        leds.setPixel(32,Color_2);
        leds.setPixel(47,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs1=6;
    } 
  
    else if (n >= 0.261 and n <= 0.28) {
        leds.setPixel(0,Color_1);
        leds.setPixel(15,Color_1);
        leds.setPixel(16,Color_1);
        leds.setPixel(31,Color_1);
        leds.setPixel(32,Color_2);
        leds.setPixel(47,Color_2);
        leds.setPixel(48,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs1=7;
    } 
    else if (n >= 0.281) {
        leds.setPixel(0,Color_1);
        leds.setPixel(15,Color_1);
        leds.setPixel(16,Color_1);
        leds.setPixel(31,Color_1);
        leds.setPixel(32,Color_2);
        leds.setPixel(47,Color_2);
        leds.setPixel(48,Color_2);
        leds.setPixel(63,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs1=8;
    }
  } 
  else if (i > 1000){
    if (cs1 == 0 and n >= .028){
        leds.setPixel(0,Color_1);
        leds.setPixel(15,OFF);
        leds.setPixel(16,OFF);
        leds.setPixel(31,OFF);
        leds.setPixel(32,OFF);
        leds.setPixel(47,OFF);
        leds.setPixel(48,OFF);
        leds.setPixel(63,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs1=1;     
    }
    if (cs1 == 0 and n <= .0279){
        leds.setPixel(0,OFF);
        leds.setPixel(15,OFF);
        leds.setPixel(16,OFF);
        leds.setPixel(31,OFF);
        leds.setPixel(32,OFF);
        leds.setPixel(47,OFF);
        leds.setPixel(48,OFF);
        leds.setPixel(63,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs1=0;     
    }
    if (cs1 == 1 and n >= .0356){
        leds.setPixel(0,Color_1);
        leds.setPixel(15,Color_1);
        leds.setPixel(16,OFF);
        leds.setPixel(31,OFF);
        leds.setPixel(32,OFF);
        leds.setPixel(47,OFF);
        leds.setPixel(48,OFF);
        leds.setPixel(63,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs1=2;
    }
    else if (cs1 == 1 and n <= .014){
        leds.setPixel(0,OFF);
        leds.setPixel(15,OFF);
        leds.setPixel(16,OFF);
        leds.setPixel(31,OFF);
        leds.setPixel(32,OFF);
        leds.setPixel(47,OFF);
        leds.setPixel(48,OFF);
        leds.setPixel(63,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs1=0;
    }
    else if (cs1 == 2 and n >= .06){
        leds.setPixel(0,Color_1);
        leds.setPixel(15,Color_1);
        leds.setPixel(16,Color_1);
        leds.setPixel(31,OFF);
        leds.setPixel(32,OFF);
        leds.setPixel(47,OFF);
        leds.setPixel(48,OFF);
        leds.setPixel(63,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs1=3;     
    }
    else if (cs1 == 2 and n <= .07){
        leds.setPixel(0,Color_1);
        leds.setPixel(15,OFF);
        leds.setPixel(16,OFF);
        leds.setPixel(31,OFF);
        leds.setPixel(32,OFF);
        leds.setPixel(47,OFF);
        leds.setPixel(48,OFF);
        leds.setPixel(63,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs1=1;
    }
    else if (cs1 == 3 and n >= .16){
        leds.setPixel(0,Color_1);
        leds.setPixel(15,Color_1);
        leds.setPixel(16,Color_1);
        leds.setPixel(31,Color_1);
        leds.setPixel(32,OFF);
        leds.setPixel(47,OFF);
        leds.setPixel(48,OFF);
        leds.setPixel(63,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs1=4;
    }
    else if (cs1 == 3 and n <= .12){
        leds.setPixel(0,Color_1);
        leds.setPixel(15,Color_1);
        leds.setPixel(16,OFF);
        leds.setPixel(31,OFF);
        leds.setPixel(32,OFF);
        leds.setPixel(47,OFF);
        leds.setPixel(48,OFF);
        leds.setPixel(63,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs1=2;
    }
    else if (cs1 == 4 and n >= .2){
        leds.setPixel(0,Color_1);
        leds.setPixel(15,Color_1);
        leds.setPixel(16,Color_1);
        leds.setPixel(31,Color_1);
        leds.setPixel(32,Color_2);
        leds.setPixel(47,OFF);
        leds.setPixel(48,OFF);
        leds.setPixel(63,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs1=5;
    }
    else if (cs1 == 4 and n <= .16){
        leds.setPixel(0,Color_1);
        leds.setPixel(15,Color_1);
        leds.setPixel(16,Color_1);
        leds.setPixel(31,OFF);
        leds.setPixel(32,OFF);
        leds.setPixel(47,OFF);
        leds.setPixel(48,OFF);
        leds.setPixel(63,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs1=3;
    }
    else if (cs1 == 5 and n >= .24){
        leds.setPixel(0,Color_1);
        leds.setPixel(15,Color_1);
        leds.setPixel(16,Color_1);
        leds.setPixel(31,Color_1);
        leds.setPixel(32,Color_2);
        leds.setPixel(47,Color_2);
        leds.setPixel(48,OFF);
        leds.setPixel(63,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs1=6;      
    }
    if (cs1 == 5 and n <= .2){
        leds.setPixel(0,Color_1);
        leds.setPixel(15,Color_1);
        leds.setPixel(16,Color_1);
        leds.setPixel(31,Color_1);
        leds.setPixel(32,OFF);
        leds.setPixel(47,OFF);
        leds.setPixel(48,OFF);
        leds.setPixel(63,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs1=4;    
    }
    else if (cs1 == 6 and n >= .26){
        leds.setPixel(0,Color_1);
        leds.setPixel(15,Color_1);
        leds.setPixel(16,Color_1);
        leds.setPixel(31,Color_1);
        leds.setPixel(32,Color_2);
        leds.setPixel(47,Color_2);
        leds.setPixel(48,Color_2);
        leds.setPixel(63,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs1=7;      
    }
    else if (cs1 == 6 and n <= .24){
        leds.setPixel(0,Color_1);
        leds.setPixel(15,Color_1);
        leds.setPixel(16,Color_1);
        leds.setPixel(31,Color_1);
        leds.setPixel(32,Color_2);
        leds.setPixel(47,OFF);
        leds.setPixel(48,OFF);
        leds.setPixel(63,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs1=5;     
    }
    else if (cs1 == 7 and n >= .28){
        leds.setPixel(0,Color_1);
        leds.setPixel(15,Color_1);
        leds.setPixel(16,Color_1);
        leds.setPixel(31,Color_1);
        leds.setPixel(32,Color_2);
        leds.setPixel(47,Color_2);
        leds.setPixel(48,Color_2);
        leds.setPixel(63,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs1=8;      
    }
    else if (cs1 == 7 and n <= .26){
        leds.setPixel(0,Color_1);
        leds.setPixel(15,Color_1);
        leds.setPixel(16,Color_1);
        leds.setPixel(31,Color_1);
        leds.setPixel(32,Color_2);
        leds.setPixel(47,Color_2);
        leds.setPixel(48,OFF);
        leds.setPixel(63,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs1=6;
    }
    if (cs1 == 8 and n >= .281){
        leds.setPixel(0,Color_1);
        leds.setPixel(15,Color_1);
        leds.setPixel(16,Color_1);
        leds.setPixel(31,Color_1);
        leds.setPixel(32,Color_2);
        leds.setPixel(47,Color_2);
        leds.setPixel(48,Color_2);
        leds.setPixel(63,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs1=8;     
    }
    else if (cs1 == 8 and n <= .28){
        leds.setPixel(0,Color_1);
        leds.setPixel(15,Color_1);
        leds.setPixel(16,Color_1);
        leds.setPixel(31,Color_1);
        leds.setPixel(32,Color_2);
        leds.setPixel(47,Color_2);
        leds.setPixel(48,Color_2);
        leds.setPixel(63,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs1=7;       
    }
    
  }
}

void Bar2(int wait, float n, int i){
  if (i == 1000){
    if (n >= 0.015 and n <= 0.035) {
        leds.setPixel(1,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs2=1;
    }
    else if (n >= 0.036 and n <= 0.06) {
        leds.setPixel(1,Color_1);
        leds.setPixel(14,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs2=2;
    }
     else if (n >= 0.061 and n <= 0.08) {
        leds.setPixel(1,Color_1);
        leds.setPixel(14,Color_1);
        leds.setPixel(17,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs2=3;
    }
      else if (n >= 0.081 and n <= 0.1) {
        leds.setPixel(1,Color_1);
        leds.setPixel(14,Color_1);
        leds.setPixel(17,Color_1);
        leds.setPixel(30,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs2=4;
    } 
    else if (n >= 0.101 and n <= 0.13) {
        leds.setPixel(1,Color_1);
        leds.setPixel(14,Color_1);
        leds.setPixel(17,Color_1);
        leds.setPixel(30,Color_1);
        leds.setPixel(33,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs2=5;
    } 
    else if (n >= 0.131 and n <= 0.15) {
        leds.setPixel(1,Color_1);
        leds.setPixel(14,Color_1);
        leds.setPixel(17,Color_1);
        leds.setPixel(30,Color_1);
        leds.setPixel(33,Color_2);
        leds.setPixel(46,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs2=6;
    } 
  
    else if (n >= 0.151 and n <= 0.17) {
        leds.setPixel(1,Color_1);
        leds.setPixel(14,Color_1);
        leds.setPixel(17,Color_1);
        leds.setPixel(30,Color_1);
        leds.setPixel(33,Color_2);
        leds.setPixel(46,Color_2);
        leds.setPixel(49,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs2=7;
    } 
    else if (n >= 0.5) {
        leds.setPixel(1,Color_1);
        leds.setPixel(14,Color_1);
        leds.setPixel(17,Color_1);
        leds.setPixel(30,Color_1);
        leds.setPixel(33,Color_2);
        leds.setPixel(46,Color_2);
        leds.setPixel(49,Color_2);
        leds.setPixel(62,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs2=8;
    } 
  }
  else if (i > 1000){
    if (cs2 == 0 and n >= .015){
        leds.setPixel(1,Color_1);
        leds.setPixel(14,OFF);
        leds.setPixel(17,OFF);
        leds.setPixel(30,OFF);
        leds.setPixel(33,OFF);
        leds.setPixel(46,OFF);
        leds.setPixel(49,OFF);
        leds.setPixel(62,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs2=1;     
    }
    if (cs2 == 0 and n < .015){
        leds.setPixel(1,OFF);
        leds.setPixel(14,OFF);
        leds.setPixel(17,OFF);
        leds.setPixel(30,OFF);
        leds.setPixel(33,OFF);
        leds.setPixel(46,OFF);
        leds.setPixel(49,OFF);
        leds.setPixel(62,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs2=0;     
    }
    if (cs2 == 1 and n >= .035){
        leds.setPixel(1,Color_1);
        leds.setPixel(14,Color_1);
        leds.setPixel(17,OFF);
        leds.setPixel(30,OFF);
        leds.setPixel(33,OFF);
        leds.setPixel(46,OFF);
        leds.setPixel(49,OFF);
        leds.setPixel(62,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs2=2;
    }
    else if (cs2 == 1 and n <= .015){
        leds.setPixel(1,OFF);
        leds.setPixel(14,OFF);
        leds.setPixel(17,OFF);
        leds.setPixel(30,OFF);
        leds.setPixel(33,OFF);
        leds.setPixel(46,OFF);
        leds.setPixel(49,OFF);
        leds.setPixel(62,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs2=0;
    }
    else if (cs2 == 2 and n >= .06){
        leds.setPixel(1,Color_1);
        leds.setPixel(14,Color_1);
        leds.setPixel(17,Color_1);
        leds.setPixel(30,OFF);
        leds.setPixel(33,OFF);
        leds.setPixel(46,OFF);
        leds.setPixel(49,OFF);
        leds.setPixel(62,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs2=3;     
    }
    else if (cs1 == 2 and n <= .035){
        leds.setPixel(1,Color_1);
        leds.setPixel(14,OFF);
        leds.setPixel(17,OFF);
        leds.setPixel(30,OFF);
        leds.setPixel(33,OFF);
        leds.setPixel(46,OFF);
        leds.setPixel(49,OFF);
        leds.setPixel(62,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs2=1;
    }
    else if (cs2 == 3 and n >= .08){
        leds.setPixel(1,Color_1);
        leds.setPixel(14,Color_1);
        leds.setPixel(17,Color_1);
        leds.setPixel(30,Color_1);
        leds.setPixel(33,OFF);
        leds.setPixel(46,OFF);
        leds.setPixel(49,OFF);
        leds.setPixel(62,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs2=4;
    }
    else if (cs2 == 3 and n <= .06){
        leds.setPixel(1,Color_1);
        leds.setPixel(14,Color_1);
        leds.setPixel(17,OFF);
        leds.setPixel(30,OFF);
        leds.setPixel(33,OFF);
        leds.setPixel(46,OFF);
        leds.setPixel(49,OFF);
        leds.setPixel(62,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs2=2;
    }
    else if (cs2 == 4 and n >= .1){
        leds.setPixel(1,Color_1);
        leds.setPixel(14,Color_1);
        leds.setPixel(17,Color_1);
        leds.setPixel(30,Color_1);
        leds.setPixel(33,Color_2);
        leds.setPixel(46,OFF);
        leds.setPixel(49,OFF);
        leds.setPixel(62,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs2=5;
    }
    else if (cs2 == 4 and n <= .08){
        leds.setPixel(1,Color_1);
        leds.setPixel(14,Color_1);
        leds.setPixel(17,Color_1);
        leds.setPixel(30,OFF);
        leds.setPixel(33,OFF);
        leds.setPixel(46,OFF);
        leds.setPixel(49,OFF);
        leds.setPixel(62,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs2=3;
    }
    else if (cs2 == 5 and n >= .13){
        leds.setPixel(1,Color_1);
        leds.setPixel(14,Color_1);
        leds.setPixel(17,Color_1);
        leds.setPixel(30,Color_1);
        leds.setPixel(33,Color_2);
        leds.setPixel(46,Color_2);
        leds.setPixel(49,OFF);
        leds.setPixel(62,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs2=6;      
    }
    if (cs2 == 5 and n <= .1){
        leds.setPixel(1,Color_1);
        leds.setPixel(14,Color_1);
        leds.setPixel(17,Color_1);
        leds.setPixel(30,Color_1);
        leds.setPixel(33,OFF);
        leds.setPixel(46,OFF);
        leds.setPixel(49,OFF);
        leds.setPixel(62,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs2=4;    
    }
    else if (cs2 == 6 and n >= .15){
        leds.setPixel(1,Color_1);
        leds.setPixel(14,Color_1);
        leds.setPixel(17,Color_1);
        leds.setPixel(30,Color_1);
        leds.setPixel(33,Color_2);
        leds.setPixel(46,Color_2);
        leds.setPixel(49,Color_2);
        leds.setPixel(62,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs2=7;      
    }
    else if (cs2 == 6 and n <= .13){
        leds.setPixel(1,Color_1);
        leds.setPixel(14,Color_1);
        leds.setPixel(17,Color_1);
        leds.setPixel(30,Color_1);
        leds.setPixel(33,Color_2);
        leds.setPixel(46,OFF);
        leds.setPixel(49,OFF);
        leds.setPixel(62,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs2=5;     
    }
    else if (cs2 == 7 and n >= .17){
        leds.setPixel(1,Color_1);
        leds.setPixel(14,Color_1);
        leds.setPixel(17,Color_1);
        leds.setPixel(30,Color_1);
        leds.setPixel(33,Color_2);
        leds.setPixel(46,Color_2);
        leds.setPixel(49,Color_2);
        leds.setPixel(62,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs2=8;      
    }
    else if (cs2 == 7 and n <= .15){
        leds.setPixel(1,Color_1);
        leds.setPixel(14,Color_1);
        leds.setPixel(17,Color_1);
        leds.setPixel(30,Color_1);
        leds.setPixel(33,Color_2);
        leds.setPixel(46,Color_2);
        leds.setPixel(49,OFF);
        leds.setPixel(62,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs2=6;
    }
    if (cs2 == 8 and n >= .5){
        leds.setPixel(1,Color_1);
        leds.setPixel(14,Color_1);
        leds.setPixel(17,Color_1);
        leds.setPixel(30,Color_1);
        leds.setPixel(33,Color_2);
        leds.setPixel(46,Color_2);
        leds.setPixel(49,Color_2);
        leds.setPixel(62,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs2=8;     
    }
    else if (cs2 == 8 and n <= .17){
        leds.setPixel(1,Color_1);
        leds.setPixel(14,Color_1);
        leds.setPixel(17,Color_1);
        leds.setPixel(30,Color_1);
        leds.setPixel(33,Color_2);
        leds.setPixel(46,Color_2);
        leds.setPixel(49,Color_2);
        leds.setPixel(62,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs2=7;       
    }
  }
}

void Bar3(int wait, float n, int i){
  if (i == 1000){
    if (n >= 0.015 and n <= 0.04) {
        leds.setPixel(2,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs3=1;
    }
    else if (n >= 0.041 and n <= 0.08) {
        leds.setPixel(2,Color_1);
        leds.setPixel(13,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs3=2;
    }
     else if (n >= 0.081 and n <= 0.11) {
        leds.setPixel(2,Color_1);
        leds.setPixel(13,Color_1);
        leds.setPixel(18,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs3=3;
    }
      else if (n >= 0.111 and n <= 0.14) {
        leds.setPixel(2,Color_1);
        leds.setPixel(13,Color_1);
        leds.setPixel(18,Color_1);
        leds.setPixel(29,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs3=4;
    } 
    else if (n >= 0.141 and n <= 0.17) {
        leds.setPixel(2,Color_1);
        leds.setPixel(13,Color_1);
        leds.setPixel(18,Color_1);
        leds.setPixel(29,Color_1);
        leds.setPixel(34,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs3=5;
    } 
    else if (n >= 0.171 and n <= 0.25) {
        leds.setPixel(2,Color_1);
        leds.setPixel(13,Color_1);
        leds.setPixel(18,Color_1);
        leds.setPixel(29,Color_1);
        leds.setPixel(34,Color_2);
        leds.setPixel(45,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs3=6;
    } 
  
    else if (n >= 0.251 and n <= 0.5) {
        leds.setPixel(2,Color_1);
        leds.setPixel(13,Color_1);
        leds.setPixel(18,Color_1);
        leds.setPixel(29,Color_1);
        leds.setPixel(34,Color_2);
        leds.setPixel(45,Color_2);
        leds.setPixel(50,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs3=7;
    } 
    else if (n >= 0.5) {
        leds.setPixel(2,Color_1);
        leds.setPixel(13,Color_1);
        leds.setPixel(18,Color_1);
        leds.setPixel(29,Color_1);
        leds.setPixel(34,Color_2);
        leds.setPixel(45,Color_2);
        leds.setPixel(50,Color_2);
        leds.setPixel(61,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs3=8;
    } 
  }
  else if (i > 1000){
    if (cs3 == 0 and n >= .015){
        leds.setPixel(2,Color_1);
        leds.setPixel(13,OFF);
        leds.setPixel(18,OFF);
        leds.setPixel(29,OFF);
        leds.setPixel(34,OFF);
        leds.setPixel(45,OFF);
        leds.setPixel(50,OFF);
        leds.setPixel(61,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs3=1;     
    }
    if (cs3 == 0 and n < .015){
        leds.setPixel(2,OFF);
        leds.setPixel(13,OFF);
        leds.setPixel(18,OFF);
        leds.setPixel(29,OFF);
        leds.setPixel(34,OFF);
        leds.setPixel(45,OFF);
        leds.setPixel(50,OFF);
        leds.setPixel(61,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs3=0;     
    }
    if (cs3 == 1 and n >= .04){
        leds.setPixel(2,Color_1);
        leds.setPixel(13,Color_1);
        leds.setPixel(18,OFF);
        leds.setPixel(29,OFF);
        leds.setPixel(34,OFF);
        leds.setPixel(45,OFF);
        leds.setPixel(50,OFF);
        leds.setPixel(61,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs3=2;
    }
    else if (cs2 == 1 and n <= .015){
        leds.setPixel(2,OFF);
        leds.setPixel(13,OFF);
        leds.setPixel(18,OFF);
        leds.setPixel(29,OFF);
        leds.setPixel(34,OFF);
        leds.setPixel(45,OFF);
        leds.setPixel(50,OFF);
        leds.setPixel(61,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs3=0;
    }
    else if (cs3 == 2 and n >= .08){
        leds.setPixel(2,Color_1);
        leds.setPixel(13,Color_1);
        leds.setPixel(18,Color_1);
        leds.setPixel(29,OFF);
        leds.setPixel(34,OFF);
        leds.setPixel(45,OFF);
        leds.setPixel(50,OFF);
        leds.setPixel(61,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs3=3;     
    }
    else if (cs3 == 2 and n <= .04){
        leds.setPixel(2,Color_1);
        leds.setPixel(13,OFF);
        leds.setPixel(18,OFF);
        leds.setPixel(29,OFF);
        leds.setPixel(34,OFF);
        leds.setPixel(45,OFF);
        leds.setPixel(50,OFF);
        leds.setPixel(61,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs3=1;
    }
    else if (cs3 == 3 and n >= .11){
        leds.setPixel(2,Color_1);
        leds.setPixel(13,Color_1);
        leds.setPixel(18,Color_1);
        leds.setPixel(29,Color_1);
        leds.setPixel(34,OFF);
        leds.setPixel(45,OFF);
        leds.setPixel(50,OFF);
        leds.setPixel(61,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs3=4;
    }
    else if (cs3 == 3 and n <= .08){
        leds.setPixel(2,Color_1);
        leds.setPixel(13,Color_1);
        leds.setPixel(18,OFF);
        leds.setPixel(29,OFF);
        leds.setPixel(34,OFF);
        leds.setPixel(45,OFF);
        leds.setPixel(50,OFF);
        leds.setPixel(61,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs3=2;
    }
    else if (cs3 == 4 and n >= .14){
        leds.setPixel(2,Color_1);
        leds.setPixel(13,Color_1);
        leds.setPixel(18,Color_1);
        leds.setPixel(29,Color_1);
        leds.setPixel(34,Color_2);
        leds.setPixel(45,OFF);
        leds.setPixel(50,OFF);
        leds.setPixel(61,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs3=5;
    }
    else if (cs3 == 4 and n <= .11){
        leds.setPixel(2,Color_1);
        leds.setPixel(13,Color_1);
        leds.setPixel(18,Color_1);
        leds.setPixel(29,OFF);
        leds.setPixel(34,OFF);
        leds.setPixel(45,OFF);
        leds.setPixel(50,OFF);
        leds.setPixel(61,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs3=3;
    }
    else if (cs3 == 5 and n >= .17){
        leds.setPixel(2,Color_1);
        leds.setPixel(13,Color_1);
        leds.setPixel(18,Color_1);
        leds.setPixel(29,Color_1);
        leds.setPixel(34,Color_2);
        leds.setPixel(45,Color_2);
        leds.setPixel(50,OFF);
        leds.setPixel(61,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs3=6;      
    }
    if (cs3 == 5 and n <= .14){
        leds.setPixel(2,Color_1);
        leds.setPixel(13,Color_1);
        leds.setPixel(18,Color_1);
        leds.setPixel(29,Color_1);
        leds.setPixel(34,OFF);
        leds.setPixel(45,OFF);
        leds.setPixel(50,OFF);
        leds.setPixel(61,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs3=4;    
    }
    else if (cs3 == 6 and n >= .25){
        leds.setPixel(2,Color_1);
        leds.setPixel(13,Color_1);
        leds.setPixel(18,Color_1);
        leds.setPixel(29,Color_1);
        leds.setPixel(34,Color_2);
        leds.setPixel(45,Color_2);
        leds.setPixel(50,Color_2);
        leds.setPixel(61,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs3=7;      
    }
    else if (cs3 == 6 and n <= .17){
        leds.setPixel(2,Color_1);
        leds.setPixel(13,Color_1);
        leds.setPixel(18,Color_1);
        leds.setPixel(29,Color_1);
        leds.setPixel(34,Color_2);
        leds.setPixel(45,OFF);
        leds.setPixel(50,OFF);
        leds.setPixel(61,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs3=5;     
    }
    else if (cs3 == 7 and n >= .5){
        leds.setPixel(2,Color_1);
        leds.setPixel(13,Color_1);
        leds.setPixel(18,Color_1);
        leds.setPixel(29,Color_1);
        leds.setPixel(34,Color_2);
        leds.setPixel(45,Color_2);
        leds.setPixel(50,Color_2);
        leds.setPixel(61,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs3=8;      
    }
    else if (cs3 == 7 and n <= .25){
        leds.setPixel(2,Color_1);
        leds.setPixel(13,Color_1);
        leds.setPixel(18,Color_1);
        leds.setPixel(29,Color_1);
        leds.setPixel(34,Color_2);
        leds.setPixel(45,Color_2);
        leds.setPixel(50,OFF);
        leds.setPixel(61,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs3=6;
    }
    if (cs3 == 8 and n >= .5){
        leds.setPixel(2,Color_1);
        leds.setPixel(13,Color_1);
        leds.setPixel(18,Color_1);
        leds.setPixel(29,Color_1);
        leds.setPixel(34,Color_2);
        leds.setPixel(45,Color_2);
        leds.setPixel(50,Color_2);
        leds.setPixel(61,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs3=8;     
    }
    else if (cs3 == 8 and n <= .5){
        leds.setPixel(2,Color_1);
        leds.setPixel(13,Color_1);
        leds.setPixel(18,Color_1);
        leds.setPixel(29,Color_1);
        leds.setPixel(34,Color_2);
        leds.setPixel(45,Color_2);
        leds.setPixel(50,Color_2);
        leds.setPixel(61,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs3=7;       
    }
  }
}

void Bar4(int wait, float n, int i){
 if (i == 1000){ 
   if (n >= 0.015 and n <= 0.035) {
        leds.setPixel(3,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs4 = 1;
    }
    else if (n >= 0.036 and n <= 0.06) {
        leds.setPixel(3,Color_1);
        leds.setPixel(12,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs4=2;
    }
     else if (n >= 0.061 and n <= 0.08) {
        leds.setPixel(3,Color_1);
        leds.setPixel(12,Color_1);
        leds.setPixel(19,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs4=3;
    }
      else if (n >= 0.081 and n <= 0.12) {
        leds.setPixel(3,Color_1);
        leds.setPixel(12,Color_1);
        leds.setPixel(19,Color_1);
        leds.setPixel(28,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs4=4;
    } 
    else if (n >= 0.121 and n <= 0.150) {
        leds.setPixel(3,Color_1);
        leds.setPixel(12,Color_1);
        leds.setPixel(19,Color_1);
        leds.setPixel(28,Color_1);
        leds.setPixel(35,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs4=5;
    } 
    else if (n >= 0.151 and n <= 0.17) {
        leds.setPixel(3,Color_1);
        leds.setPixel(12,Color_1);
        leds.setPixel(19,Color_1);
        leds.setPixel(28,Color_1);
        leds.setPixel(35,Color_2);
        leds.setPixel(44,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs4=6;
    } 
  
    else if (n >= 0.171 and n <= 0.19) {
        leds.setPixel(3,Color_1);
        leds.setPixel(12,Color_1);
        leds.setPixel(19,Color_1);
        leds.setPixel(28,Color_1);
        leds.setPixel(35,Color_2);
        leds.setPixel(44,Color_2);
        leds.setPixel(51,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs4=7;
    } 
    else if (n >= 0.19) {
        leds.setPixel(3,Color_1);
        leds.setPixel(12,Color_1);
        leds.setPixel(19,Color_1);
        leds.setPixel(28,Color_1);
        leds.setPixel(35,Color_2);
        leds.setPixel(44,Color_2);
        leds.setPixel(51,Color_2);
        leds.setPixel(60,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs4=8;
    } 
 }
 else if (i > 1000){
      if (cs4 == 0 and n >= .015){
        leds.setPixel(3,Color_1);
        leds.setPixel(12,OFF);
        leds.setPixel(19,OFF);
        leds.setPixel(28,OFF);
        leds.setPixel(35,OFF);
        leds.setPixel(44,OFF);
        leds.setPixel(51,OFF);
        leds.setPixel(60,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs4=1;     
    }
    if (cs4 == 0 and n < .015){
        leds.setPixel(3,OFF);
        leds.setPixel(12,OFF);
        leds.setPixel(19,OFF);
        leds.setPixel(28,OFF);
        leds.setPixel(35,OFF);
        leds.setPixel(44,OFF);
        leds.setPixel(51,OFF);
        leds.setPixel(60,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs4=0;     
    }
    if (cs4 == 1 and n >= .035){
        leds.setPixel(3,Color_1);
        leds.setPixel(12,Color_1);
        leds.setPixel(19,OFF);
        leds.setPixel(28,OFF);
        leds.setPixel(35,OFF);
        leds.setPixel(44,OFF);
        leds.setPixel(51,OFF);
        leds.setPixel(60,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs4=2;
    }
    else if (cs4 == 1 and n <= .015){
        leds.setPixel(3,OFF);
        leds.setPixel(12,OFF);
        leds.setPixel(19,OFF);
        leds.setPixel(28,OFF);
        leds.setPixel(35,OFF);
        leds.setPixel(44,OFF);
        leds.setPixel(51,OFF);
        leds.setPixel(60,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs4=0;
    }
    else if (cs4 == 2 and n >= .06){
        leds.setPixel(3,Color_1);
        leds.setPixel(12,Color_1);
        leds.setPixel(19,Color_1);
        leds.setPixel(28,OFF);
        leds.setPixel(35,OFF);
        leds.setPixel(44,OFF);
        leds.setPixel(51,OFF);
        leds.setPixel(60,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs4=3;     
    }
    else if (cs4 == 2 and n <= .035){
        leds.setPixel(3,Color_1);
        leds.setPixel(12,OFF);
        leds.setPixel(19,OFF);
        leds.setPixel(28,OFF);
        leds.setPixel(35,OFF);
        leds.setPixel(44,OFF);
        leds.setPixel(51,OFF);
        leds.setPixel(60,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs4=1;
    }
    else if (cs4 == 3 and n >= .08){
        leds.setPixel(3,Color_1);
        leds.setPixel(12,Color_1);
        leds.setPixel(19,Color_1);
        leds.setPixel(28,Color_1);
        leds.setPixel(35,OFF);
        leds.setPixel(44,OFF);
        leds.setPixel(51,OFF);
        leds.setPixel(60,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs4=4;
    }
    else if (cs4 == 3 and n <= .06){
        leds.setPixel(3,Color_1);
        leds.setPixel(12,Color_1);
        leds.setPixel(19,OFF);
        leds.setPixel(28,OFF);
        leds.setPixel(35,OFF);
        leds.setPixel(44,OFF);
        leds.setPixel(51,OFF);
        leds.setPixel(60,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs4=2;
    }
    else if (cs4 == 4 and n >= .12){
        leds.setPixel(3,Color_1);
        leds.setPixel(12,Color_1);
        leds.setPixel(19,Color_1);
        leds.setPixel(28,Color_1);
        leds.setPixel(35,Color_2);
        leds.setPixel(44,OFF);
        leds.setPixel(51,OFF);
        leds.setPixel(60,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs4=5;
    }
    else if (cs4 == 4 and n <= .08){
        leds.setPixel(3,Color_1);
        leds.setPixel(12,Color_1);
        leds.setPixel(19,Color_1);
        leds.setPixel(28,OFF);
        leds.setPixel(35,OFF);
        leds.setPixel(44,OFF);
        leds.setPixel(51,OFF);
        leds.setPixel(60,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs4=3;
    }
    else if (cs4 == 5 and n >= .15){
        leds.setPixel(3,Color_1);
        leds.setPixel(12,Color_1);
        leds.setPixel(19,Color_1);
        leds.setPixel(28,Color_1);
        leds.setPixel(35,Color_2);
        leds.setPixel(44,Color_2);
        leds.setPixel(51,OFF);
        leds.setPixel(60,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs4=6;      
    }
    if (cs4 == 5 and n <= .12){
        leds.setPixel(3,Color_1);
        leds.setPixel(12,Color_1);
        leds.setPixel(19,Color_1);
        leds.setPixel(28,Color_1);
        leds.setPixel(35,OFF);
        leds.setPixel(44,OFF);
        leds.setPixel(51,OFF);
        leds.setPixel(60,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs4=4;    
    }
    else if (cs4 == 6 and n >= .17){
        leds.setPixel(3,Color_1);
        leds.setPixel(12,Color_1);
        leds.setPixel(19,Color_1);
        leds.setPixel(28,Color_1);
        leds.setPixel(35,Color_2);
        leds.setPixel(44,Color_2);
        leds.setPixel(51,Color_2);
        leds.setPixel(60,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs4=7;      
    }
    else if (cs4 == 6 and n <= .15){
        leds.setPixel(3,Color_1);
        leds.setPixel(12,Color_1);
        leds.setPixel(19,Color_1);
        leds.setPixel(28,Color_1);
        leds.setPixel(35,Color_2);
        leds.setPixel(44,OFF);
        leds.setPixel(51,OFF);
        leds.setPixel(60,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs4=5;     
    }
    else if (cs4 == 7 and n >= .19){
        leds.setPixel(3,Color_1);
        leds.setPixel(12,Color_1);
        leds.setPixel(19,Color_1);
        leds.setPixel(28,Color_1);
        leds.setPixel(35,Color_2);
        leds.setPixel(44,Color_2);
        leds.setPixel(51,Color_2);
        leds.setPixel(60,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs4=8;      
    }
    else if (cs4 == 7 and n <= .17){
        leds.setPixel(3,Color_1);
        leds.setPixel(12,Color_1);
        leds.setPixel(19,Color_1);
        leds.setPixel(28,Color_1);
        leds.setPixel(35,Color_2);
        leds.setPixel(44,Color_2);
        leds.setPixel(51,OFF);
        leds.setPixel(60,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs4=6;
    }
    if (cs4 == 8 and n >= .19){
        leds.setPixel(3,Color_1);
        leds.setPixel(12,Color_1);
        leds.setPixel(19,Color_1);
        leds.setPixel(28,Color_1);
        leds.setPixel(35,Color_2);
        leds.setPixel(44,Color_2);
        leds.setPixel(51,Color_2);
        leds.setPixel(60,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs4=8;     
    }
    else if (cs4 == 8 and n < .19){
        leds.setPixel(3,Color_1);
        leds.setPixel(12,Color_1);
        leds.setPixel(19,Color_1);
        leds.setPixel(28,Color_1);
        leds.setPixel(35,Color_2);
        leds.setPixel(44,Color_2);
        leds.setPixel(51,Color_2);
        leds.setPixel(60,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs4=7;       
    }
 }
 
}

void Bar5(int wait, float n, int i){

  if (i == 1000) {  
    if (n >= 0.015 and n <= 0.035) {
        leds.setPixel(4,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs5=1;
    }
    else if (n >= 0.036 and n <= 0.06) {
        leds.setPixel(4,Color_1);
        leds.setPixel(11,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs5=2;
    }
     else if (n >= 0.061 and n <= 0.08) {
        leds.setPixel(4,Color_1);
        leds.setPixel(11,Color_1);
        leds.setPixel(20,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs5=3;
    }
      else if (n >= 0.081 and n <= 0.1) {
        leds.setPixel(4,Color_1);
        leds.setPixel(11,Color_1);
        leds.setPixel(20,Color_1);
        leds.setPixel(27,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs5=4;
    } 
    else if (n >= 0.101 and n <= 0.12) {
        leds.setPixel(4,Color_1);
        leds.setPixel(11,Color_1);
        leds.setPixel(20,Color_1);
        leds.setPixel(27,Color_1);
        leds.setPixel(36,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs5=5;
    } 
    else if (n >= 0.121 and n <= 0.13) {
        leds.setPixel(4,Color_1);
        leds.setPixel(11,Color_1);
        leds.setPixel(20,Color_1);
        leds.setPixel(27,Color_1);
        leds.setPixel(36,Color_2);
        leds.setPixel(43,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs5=6;
    } 
  
    else if (n >= 0.131 and n <= 0.14) {
        leds.setPixel(4,Color_1);
        leds.setPixel(11,Color_1);
        leds.setPixel(20,Color_1);
        leds.setPixel(27,Color_1);
        leds.setPixel(36,Color_2);
        leds.setPixel(43,Color_2);
        leds.setPixel(52,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs5=7;
    } 
    else if (n >= 0.141) {
        leds.setPixel(4,Color_1);
        leds.setPixel(11,Color_1);
        leds.setPixel(20,Color_1);
        leds.setPixel(27,Color_1);
        leds.setPixel(36,Color_2);
        leds.setPixel(43,Color_2);
        leds.setPixel(52,Color_2);
        leds.setPixel(59,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs5=8;
    } 
  }
  else if (i > 1000){
    if (cs5 == 0 and n >= .015){
        leds.setPixel(4,Color_1);
        leds.setPixel(11,OFF);
        leds.setPixel(20,OFF);
        leds.setPixel(27,OFF);
        leds.setPixel(36,OFF);
        leds.setPixel(43,OFF);
        leds.setPixel(52,OFF);
        leds.setPixel(59,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs5=1;     
    }
    if (cs5 == 0 and n < .015){
        leds.setPixel(4,OFF);
        leds.setPixel(11,OFF);
        leds.setPixel(20,OFF);
        leds.setPixel(27,OFF);
        leds.setPixel(36,OFF);
        leds.setPixel(43,OFF);
        leds.setPixel(52,OFF);
        leds.setPixel(59,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs5=0;     
    }
    if (cs5 == 1 and n >= .035){
        leds.setPixel(4,Color_1);
        leds.setPixel(11,Color_1);
        leds.setPixel(20,OFF);
        leds.setPixel(27,OFF);
        leds.setPixel(36,OFF);
        leds.setPixel(43,OFF);
        leds.setPixel(52,OFF);
        leds.setPixel(59,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs5=2;
    }
    else if (cs5 == 1 and n <= .015){
        leds.setPixel(4,OFF);
        leds.setPixel(11,OFF);
        leds.setPixel(20,OFF);
        leds.setPixel(27,OFF);
        leds.setPixel(36,OFF);
        leds.setPixel(43,OFF);
        leds.setPixel(52,OFF);
        leds.setPixel(59,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs5=0;
    }
    else if (cs5 == 2 and n >= .06){
        leds.setPixel(4,Color_1);
        leds.setPixel(11,Color_1);
        leds.setPixel(20,Color_1);
        leds.setPixel(27,OFF);
        leds.setPixel(36,OFF);
        leds.setPixel(43,OFF);
        leds.setPixel(52,OFF);
        leds.setPixel(59,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs5=3;     
    }
    else if (cs5 == 2 and n <= .035){
        leds.setPixel(4,Color_1);
        leds.setPixel(11,OFF);
        leds.setPixel(20,OFF);
        leds.setPixel(27,OFF);
        leds.setPixel(36,OFF);
        leds.setPixel(43,OFF);
        leds.setPixel(52,OFF);
        leds.setPixel(59,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs5=1;
    }
    else if (cs5 == 3 and n >= .08){
        leds.setPixel(4,Color_1);
        leds.setPixel(11,Color_1);
        leds.setPixel(20,Color_1);
        leds.setPixel(27,Color_1);
        leds.setPixel(36,OFF);
        leds.setPixel(43,OFF);
        leds.setPixel(52,OFF);
        leds.setPixel(59,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs5=4;
    }
    else if (cs5 == 3 and n <= .06){
        leds.setPixel(4,Color_1);
        leds.setPixel(11,Color_1);
        leds.setPixel(20,OFF);
        leds.setPixel(27,OFF);
        leds.setPixel(36,OFF);
        leds.setPixel(43,OFF);
        leds.setPixel(52,OFF);
        leds.setPixel(59,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs5=2;
    }
    else if (cs5 == 4 and n >= .1){
        leds.setPixel(4,Color_1);
        leds.setPixel(11,Color_1);
        leds.setPixel(20,Color_1);
        leds.setPixel(27,Color_1);
        leds.setPixel(36,Color_2);
        leds.setPixel(43,OFF);
        leds.setPixel(52,OFF);
        leds.setPixel(59,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs5=5;
    }
    else if (cs5 == 4 and n <= .08){
        leds.setPixel(4,Color_1);
        leds.setPixel(11,Color_1);
        leds.setPixel(20,Color_1);
        leds.setPixel(27,OFF);
        leds.setPixel(36,OFF);
        leds.setPixel(43,OFF);
        leds.setPixel(52,OFF);
        leds.setPixel(59,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs5=3;
    }
    else if (cs5 == 5 and n >= .12){
        leds.setPixel(4,Color_1);
        leds.setPixel(11,Color_1);
        leds.setPixel(20,Color_1);
        leds.setPixel(27,Color_1);
        leds.setPixel(36,Color_2);
        leds.setPixel(43,Color_2);
        leds.setPixel(52,OFF);
        leds.setPixel(59,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs5=6;      
    }
    if (cs5 == 5 and n <= .1){
        leds.setPixel(4,Color_1);
        leds.setPixel(11,Color_1);
        leds.setPixel(20,Color_1);
        leds.setPixel(27,Color_1);
        leds.setPixel(36,OFF);
        leds.setPixel(43,OFF);
        leds.setPixel(52,OFF);
        leds.setPixel(59,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs5=4;    
    }
    else if (cs5 == 6 and n >= .13){
        leds.setPixel(4,Color_1);
        leds.setPixel(11,Color_1);
        leds.setPixel(20,Color_1);
        leds.setPixel(27,Color_1);
        leds.setPixel(36,Color_2);
        leds.setPixel(43,Color_2);
        leds.setPixel(52,Color_2);
        leds.setPixel(59,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs5=7;      
    }
    else if (cs5 == 6 and n <= .12){
        leds.setPixel(4,Color_1);
        leds.setPixel(11,Color_1);
        leds.setPixel(20,Color_1);
        leds.setPixel(27,Color_1);
        leds.setPixel(36,Color_2);
        leds.setPixel(43,OFF);
        leds.setPixel(52,OFF);
        leds.setPixel(59,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs5=5;     
    }
    else if (cs5 == 7 and n >= .14){
        leds.setPixel(4,Color_1);
        leds.setPixel(11,Color_1);
        leds.setPixel(20,Color_1);
        leds.setPixel(27,Color_1);
        leds.setPixel(36,Color_2);
        leds.setPixel(43,Color_2);
        leds.setPixel(52,Color_2);
        leds.setPixel(59,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs5=8;      
    }
    else if (cs5 == 7 and n <= .13){
        leds.setPixel(4,Color_1);
        leds.setPixel(11,Color_1);
        leds.setPixel(20,Color_1);
        leds.setPixel(27,Color_1);
        leds.setPixel(36,Color_2);
        leds.setPixel(43,Color_2);
        leds.setPixel(52,OFF);
        leds.setPixel(59,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs5=6;
    }
    if (cs5 == 8 and n >= .14){
        leds.setPixel(4,Color_1);
        leds.setPixel(11,Color_1);
        leds.setPixel(20,Color_1);
        leds.setPixel(27,Color_1);
        leds.setPixel(36,Color_2);
        leds.setPixel(43,Color_2);
        leds.setPixel(52,Color_2);
        leds.setPixel(59,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs5=8;     
    }
    else if (cs5 == 8 and n < .14){
        leds.setPixel(4,Color_1);
        leds.setPixel(11,Color_1);
        leds.setPixel(20,Color_1);
        leds.setPixel(27,Color_1);
        leds.setPixel(36,Color_2);
        leds.setPixel(43,Color_2);
        leds.setPixel(52,Color_2);
        leds.setPixel(59,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs5=7;       
    }
  }
}

void Bar6(int wait, float n, int i){
  if (i==1){
    if (n >= 0.015 and n <= 0.035) {
        leds.setPixel(5,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs6=1;
    }
    else if (n >= 0.036 and n <= 0.06) {
        leds.setPixel(5,Color_1);
        leds.setPixel(10,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs6=2;
    }
     else if (n >= 0.061 and n <= 0.09) {
        leds.setPixel(5,Color_1);
        leds.setPixel(10,Color_1);
        leds.setPixel(21,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs6=3;
    }
      else if (n >= 0.091 and n <= 0.12) {
        leds.setPixel(5,Color_1);
        leds.setPixel(10,Color_1);
        leds.setPixel(21,Color_1);
        leds.setPixel(26,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs6=4;
    } 
    else if (n >= 0.121 and n <= 0.15) {
        leds.setPixel(5,Color_1);
        leds.setPixel(10,Color_1);
        leds.setPixel(21,Color_1);
        leds.setPixel(26,Color_1);
        leds.setPixel(37,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs6=5;
    } 
    else if (n >= 0.151 and n <= 0.18) {
        leds.setPixel(5,Color_1);
        leds.setPixel(10,Color_1);
        leds.setPixel(21,Color_1);
        leds.setPixel(26,Color_1);
        leds.setPixel(37,Color_2);
        leds.setPixel(42,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs6=5;
    } 
  
    else if (n >= 0.181 and n <= 0.210) {
        leds.setPixel(5,Color_1);
        leds.setPixel(10,Color_1);
        leds.setPixel(21,Color_1);
        leds.setPixel(26,Color_1);
        leds.setPixel(37,Color_2);
        leds.setPixel(42,Color_2);
        leds.setPixel(53,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs6=7;
    } 
    else if (n >= 0.211) {
        leds.setPixel(5,Color_1);
        leds.setPixel(10,Color_1);
        leds.setPixel(21,Color_1);
        leds.setPixel(26,Color_1);
        leds.setPixel(37,Color_2);
        leds.setPixel(42,Color_2);
        leds.setPixel(53,Color_2);
        leds.setPixel(58,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs6=8;
    } 
  }
  else if (i > 1000){
    if (cs6 == 0 and n >= .015){
        leds.setPixel(5,Color_1);
        leds.setPixel(10,OFF);
        leds.setPixel(21,OFF);
        leds.setPixel(26,OFF);
        leds.setPixel(37,OFF);
        leds.setPixel(42,OFF);
        leds.setPixel(53,OFF);
        leds.setPixel(58,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs6=1;     
    }
    if (cs6 == 0 and n < .015){
        leds.setPixel(5,OFF);
        leds.setPixel(10,OFF);
        leds.setPixel(21,OFF);
        leds.setPixel(26,OFF);
        leds.setPixel(37,OFF);
        leds.setPixel(42,OFF);
        leds.setPixel(53,OFF);
        leds.setPixel(58,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs6=0;     
    }
    if (cs6 == 1 and n >= .035){
        leds.setPixel(5,Color_1);
        leds.setPixel(10,Color_1);
        leds.setPixel(21,OFF);
        leds.setPixel(26,OFF);
        leds.setPixel(37,OFF);
        leds.setPixel(42,OFF);
        leds.setPixel(53,OFF);
        leds.setPixel(58,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs6=2;
    }
    else if (cs6 == 1 and n <= .015){
        leds.setPixel(5,OFF);
        leds.setPixel(10,OFF);
        leds.setPixel(21,OFF);
        leds.setPixel(26,OFF);
        leds.setPixel(37,OFF);
        leds.setPixel(42,OFF);
        leds.setPixel(53,OFF);
        leds.setPixel(58,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs6=0;
    }
    else if (cs6 == 2 and n >= .06){
        leds.setPixel(5,Color_1);
        leds.setPixel(10,Color_1);
        leds.setPixel(21,Color_1);
        leds.setPixel(26,OFF);
        leds.setPixel(37,OFF);
        leds.setPixel(42,OFF);
        leds.setPixel(53,OFF);
        leds.setPixel(58,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs6=3;     
    }
    else if (cs6 == 2 and n <= .035){
        leds.setPixel(5,Color_1);
        leds.setPixel(10,OFF);
        leds.setPixel(21,OFF);
        leds.setPixel(26,OFF);
        leds.setPixel(37,OFF);
        leds.setPixel(42,OFF);
        leds.setPixel(53,OFF);
        leds.setPixel(58,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs6=1;
    }
    else if (cs6 == 3 and n >= .09){
        leds.setPixel(5,Color_1);
        leds.setPixel(10,Color_1);
        leds.setPixel(21,Color_1);
        leds.setPixel(26,Color_1);
        leds.setPixel(37,OFF);
        leds.setPixel(42,OFF);
        leds.setPixel(53,OFF);
        leds.setPixel(58,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs6=4;
    }
    else if (cs6 == 3 and n <= .06){
        leds.setPixel(5,Color_1);
        leds.setPixel(10,Color_1);
        leds.setPixel(21,OFF);
        leds.setPixel(26,OFF);
        leds.setPixel(37,OFF);
        leds.setPixel(42,OFF);
        leds.setPixel(53,OFF);
        leds.setPixel(58,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs6=2;
    }
    else if (cs6 == 4 and n >= .12){
        leds.setPixel(5,Color_1);
        leds.setPixel(10,Color_1);
        leds.setPixel(21,Color_1);
        leds.setPixel(26,Color_1);
        leds.setPixel(37,Color_2);
        leds.setPixel(42,OFF);
        leds.setPixel(53,OFF);
        leds.setPixel(58,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs6=5;
    }
    else if (cs6 == 4 and n <= .09){
        leds.setPixel(5,Color_1);
        leds.setPixel(10,Color_1);
        leds.setPixel(21,Color_1);
        leds.setPixel(26,OFF);
        leds.setPixel(37,OFF);
        leds.setPixel(42,OFF);
        leds.setPixel(53,OFF);
        leds.setPixel(58,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs6=3;
    }
    else if (cs6 == 5 and n >= .15){
        leds.setPixel(5,Color_1);
        leds.setPixel(10,Color_1);
        leds.setPixel(21,Color_1);
        leds.setPixel(26,Color_1);
        leds.setPixel(37,Color_2);
        leds.setPixel(42,Color_2);
        leds.setPixel(53,OFF);
        leds.setPixel(58,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs6=5;      
    }
    if (cs6 == 5 and n <= .12){
        leds.setPixel(5,Color_1);
        leds.setPixel(10,Color_1);
        leds.setPixel(21,Color_1);
        leds.setPixel(26,Color_1);
        leds.setPixel(37,OFF);
        leds.setPixel(42,OFF);
        leds.setPixel(53,OFF);
        leds.setPixel(58,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs6=4;    
    }
    else if (cs6 == 5 and n >= .18){
        leds.setPixel(5,Color_1);
        leds.setPixel(10,Color_1);
        leds.setPixel(21,Color_1);
        leds.setPixel(26,Color_1);
        leds.setPixel(37,Color_2);
        leds.setPixel(42,Color_2);
        leds.setPixel(53,Color_2);
        leds.setPixel(58,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs6=7;      
    }
    else if (cs6 == 5 and n <= .15){
        leds.setPixel(5,Color_1);
        leds.setPixel(10,Color_1);
        leds.setPixel(21,Color_1);
        leds.setPixel(26,Color_1);
        leds.setPixel(37,Color_2);
        leds.setPixel(42,OFF);
        leds.setPixel(53,OFF);
        leds.setPixel(58,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs6=5;     
    }
    else if (cs6 == 7 and n >= .21){
        leds.setPixel(5,Color_1);
        leds.setPixel(10,Color_1);
        leds.setPixel(21,Color_1);
        leds.setPixel(26,Color_1);
        leds.setPixel(37,Color_2);
        leds.setPixel(42,Color_2);
        leds.setPixel(53,Color_2);
        leds.setPixel(58,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs6=8;      
    }
    else if (cs6 == 7 and n <= .18){
        leds.setPixel(5,Color_1);
        leds.setPixel(10,Color_1);
        leds.setPixel(21,Color_1);
        leds.setPixel(26,Color_1);
        leds.setPixel(37,Color_2);
        leds.setPixel(42,Color_2);
        leds.setPixel(53,OFF);
        leds.setPixel(58,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs6=5;
    }
    if (cs6 == 8 and n >= .21){
        leds.setPixel(5,Color_1);
        leds.setPixel(10,Color_1);
        leds.setPixel(21,Color_1);
        leds.setPixel(26,Color_1);
        leds.setPixel(37,Color_2);
        leds.setPixel(42,Color_2);
        leds.setPixel(53,Color_2);
        leds.setPixel(58,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs6=8;     
    }
    else if (cs6 == 8 and n < .21){
        leds.setPixel(5,Color_1);
        leds.setPixel(10,Color_1);
        leds.setPixel(21,Color_1);
        leds.setPixel(26,Color_1);
        leds.setPixel(37,Color_2);
        leds.setPixel(42,Color_2);
        leds.setPixel(53,Color_2);
        leds.setPixel(58,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs6=7;       
    }
  }
}

void Bar7(int wait, float n, int i){
  if (i==1){
    if (n >= 0.015 and n <= 0.035) {
        leds.setPixel(6,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs7=1;
    }
    else if (n >= 0.036 and n <= 0.06) {
        leds.setPixel(6,Color_1);
        leds.setPixel(9,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs7=2;
    }
     else if (n >= 0.061 and n <= 0.09) {
        leds.setPixel(6,Color_1);
        leds.setPixel(9,Color_1);
        leds.setPixel(22,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs7=3;
    }
      else if (n >= 0.091 and n <= 0.12) {
        leds.setPixel(6,Color_1);
        leds.setPixel(9,Color_1);
        leds.setPixel(22,Color_1);
        leds.setPixel(25,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs7=4;
    } 
    else if (n >= 0.121 and n <= 0.15) {
        leds.setPixel(6,Color_1);
        leds.setPixel(9,Color_1);
        leds.setPixel(22,Color_1);
        leds.setPixel(25,Color_1);
        leds.setPixel(38,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs7=5;
    } 
    else if (n >= 0.151 and n <= 0.18) {
        leds.setPixel(6,Color_1);
        leds.setPixel(9,Color_1);
        leds.setPixel(22,Color_1);
        leds.setPixel(25,Color_1);
        leds.setPixel(38,Color_2);
        leds.setPixel(41,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs7=6;
    } 
  
    else if (n >= 0.181 and n <= 0.210) {
        leds.setPixel(6,Color_1);
        leds.setPixel(9,Color_1);
        leds.setPixel(22,Color_1);
        leds.setPixel(25,Color_1);
        leds.setPixel(38,Color_2);
        leds.setPixel(41,Color_2);
        leds.setPixel(54,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs7=7;
    } 
    else if (n >= 0.211) {
        leds.setPixel(6,Color_1);
        leds.setPixel(9,Color_1);
        leds.setPixel(22,Color_1);
        leds.setPixel(25,Color_1);
        leds.setPixel(38,Color_2);
        leds.setPixel(41,Color_2);
        leds.setPixel(54,Color_2);
        leds.setPixel(57,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs7=8;
    } 
  }
  else if (i > 1000){
    if (cs7 == 0 and n >= .015){
        leds.setPixel(6,Color_1);
        leds.setPixel(9,OFF);
        leds.setPixel(22,OFF);
        leds.setPixel(25,OFF);
        leds.setPixel(38,OFF);
        leds.setPixel(41,OFF);
        leds.setPixel(54,OFF);
        leds.setPixel(57,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs7=1;     
    }
    if (cs7 == 0 and n < .015){
        leds.setPixel(6,OFF);
        leds.setPixel(9,OFF);
        leds.setPixel(22,OFF);
        leds.setPixel(25,OFF);
        leds.setPixel(38,OFF);
        leds.setPixel(41,OFF);
        leds.setPixel(54,OFF);
        leds.setPixel(57,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs7=0;     
    }
    if (cs7 == 1 and n >= .035){
        leds.setPixel(6,Color_1);
        leds.setPixel(9,Color_1);
        leds.setPixel(22,OFF);
        leds.setPixel(25,OFF);
        leds.setPixel(38,OFF);
        leds.setPixel(41,OFF);
        leds.setPixel(54,OFF);
        leds.setPixel(57,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs7=2;
    }
    else if (cs7 == 1 and n <= .015){
        leds.setPixel(6,OFF);
        leds.setPixel(9,OFF);
        leds.setPixel(22,OFF);
        leds.setPixel(25,OFF);
        leds.setPixel(38,OFF);
        leds.setPixel(41,OFF);
        leds.setPixel(54,OFF);
        leds.setPixel(57,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs7=0;
    }
    else if (cs7 == 2 and n >= .06){
        leds.setPixel(6,Color_1);
        leds.setPixel(9,Color_1);
        leds.setPixel(22,Color_1);
        leds.setPixel(25,OFF);
        leds.setPixel(38,OFF);
        leds.setPixel(41,OFF);
        leds.setPixel(54,OFF);
        leds.setPixel(57,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs7=3;     
    }
    else if (cs7 == 2 and n <= .035){
        leds.setPixel(6,Color_1);
        leds.setPixel(9,OFF);
        leds.setPixel(22,OFF);
        leds.setPixel(25,OFF);
        leds.setPixel(38,OFF);
        leds.setPixel(41,OFF);
        leds.setPixel(54,OFF);
        leds.setPixel(57,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs7=1;
    }
    else if (cs7 == 3 and n >= .09){
        leds.setPixel(6,Color_1);
        leds.setPixel(9,Color_1);
        leds.setPixel(22,Color_1);
        leds.setPixel(25,Color_1);
        leds.setPixel(38,OFF);
        leds.setPixel(41,OFF);
        leds.setPixel(54,OFF);
        leds.setPixel(57,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs7=4;
    }
    else if (cs7 == 3 and n <= .06){
        leds.setPixel(6,Color_1);
        leds.setPixel(9,Color_1);
        leds.setPixel(22,OFF);
        leds.setPixel(25,OFF);
        leds.setPixel(38,OFF);
        leds.setPixel(41,OFF);
        leds.setPixel(54,OFF);
        leds.setPixel(57,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs7=2;
    }
    else if (cs7 == 4 and n >= .12){
        leds.setPixel(6,Color_1);
        leds.setPixel(9,Color_1);
        leds.setPixel(22,Color_1);
        leds.setPixel(25,Color_1);
        leds.setPixel(38,Color_2);
        leds.setPixel(41,OFF);
        leds.setPixel(54,OFF);
        leds.setPixel(57,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs7=5;
    }
    else if (cs7 == 4 and n <= .09){
        leds.setPixel(6,Color_1);
        leds.setPixel(9,Color_1);
        leds.setPixel(22,Color_1);
        leds.setPixel(25,OFF);
        leds.setPixel(38,OFF);
        leds.setPixel(41,OFF);
        leds.setPixel(54,OFF);
        leds.setPixel(57,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs7=3;
    }
    else if (cs7 == 5 and n >= .15){
        leds.setPixel(6,Color_1);
        leds.setPixel(9,Color_1);
        leds.setPixel(22,Color_1);
        leds.setPixel(25,Color_1);
        leds.setPixel(38,Color_2);
        leds.setPixel(41,Color_2);
        leds.setPixel(54,OFF);
        leds.setPixel(57,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs7=6;      
    }
    if (cs7 == 5 and n <= .12){
        leds.setPixel(6,Color_1);
        leds.setPixel(9,Color_1);
        leds.setPixel(22,Color_1);
        leds.setPixel(25,Color_1);
        leds.setPixel(38,OFF);
        leds.setPixel(41,OFF);
        leds.setPixel(54,OFF);
        leds.setPixel(57,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs7=4;    
    }
    else if (cs7 == 6 and n >= .18){
        leds.setPixel(6,Color_1);
        leds.setPixel(9,Color_1);
        leds.setPixel(22,Color_1);
        leds.setPixel(25,Color_1);
        leds.setPixel(38,Color_2);
        leds.setPixel(41,Color_2);
        leds.setPixel(54,Color_2);
        leds.setPixel(57,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs7=7;      
    }
    else if (cs7 == 6 and n <= .15){
        leds.setPixel(6,Color_1);
        leds.setPixel(9,Color_1);
        leds.setPixel(22,Color_1);
        leds.setPixel(25,Color_1);
        leds.setPixel(38,Color_2);
        leds.setPixel(41,OFF);
        leds.setPixel(54,OFF);
        leds.setPixel(57,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs7=5;     
    }
    else if (cs7 == 7 and n >= .21){
        leds.setPixel(6,Color_1);
        leds.setPixel(9,Color_1);
        leds.setPixel(22,Color_1);
        leds.setPixel(25,Color_1);
        leds.setPixel(38,Color_2);
        leds.setPixel(41,Color_2);
        leds.setPixel(54,Color_2);
        leds.setPixel(57,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs7=8;      
    }
    else if (cs7 == 7 and n <= .18){
        leds.setPixel(6,Color_1);
        leds.setPixel(9,Color_1);
        leds.setPixel(22,Color_1);
        leds.setPixel(25,Color_1);
        leds.setPixel(38,Color_2);
        leds.setPixel(41,Color_2);
        leds.setPixel(54,OFF);
        leds.setPixel(57,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs7=6;
    }
    if (cs7 == 8 and n >= .21){
        leds.setPixel(6,Color_1);
        leds.setPixel(9,Color_1);
        leds.setPixel(22,Color_1);
        leds.setPixel(25,Color_1);
        leds.setPixel(38,Color_2);
        leds.setPixel(41,Color_2);
        leds.setPixel(54,Color_2);
        leds.setPixel(57,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs7=8;     
    }
    else if (cs7 == 8 and n < .21){
        leds.setPixel(6,Color_1);
        leds.setPixel(9,Color_1);
        leds.setPixel(22,Color_1);
        leds.setPixel(25,Color_1);
        leds.setPixel(38,Color_2);
        leds.setPixel(41,Color_2);
        leds.setPixel(54,Color_2);
        leds.setPixel(57,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs7=7;       
    }
  }
}

void Bar8(int wait, float n, int i){

  if (i == 1000){
    if (n >= 0.015 and n <= 0.035) {
        leds.setPixel(7,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs8=1;
    }
    else if (n >= 0.036 and n <= 0.06) {
        leds.setPixel(7,Color_1);
        leds.setPixel(8,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs8=2;
    }
     else if (n >= 0.061 and n <= 0.08) {
        leds.setPixel(7,Color_1);
        leds.setPixel(8,Color_1);
        leds.setPixel(23,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs8=3;
    }
      else if (n >= 0.081 and n <= 0.11) {
        leds.setPixel(7,Color_1);
        leds.setPixel(8,Color_1);
        leds.setPixel(23,Color_1);
        leds.setPixel(24,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs8=4;
    } 
    else if (n >= 0.111 and n <= 0.14) {
        leds.setPixel(7,Color_1);
        leds.setPixel(8,Color_1);
        leds.setPixel(23,Color_1);
        leds.setPixel(24,Color_1);
        leds.setPixel(39,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs8=5;
    } 
    else if (n >= 0.141 and n <= 0.17) {
        leds.setPixel(7,Color_1);
        leds.setPixel(8,Color_1);
        leds.setPixel(23,Color_1);
        leds.setPixel(24,Color_1);
        leds.setPixel(39,Color_2);
        leds.setPixel(40,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs8=6;
    } 
  
    else if (n >= 0.171 and n <= 0.210) {
        leds.setPixel(7,Color_1);
        leds.setPixel(8,Color_1);
        leds.setPixel(23,Color_1);
        leds.setPixel(24,Color_1);
        leds.setPixel(39,Color_2);
        leds.setPixel(40,Color_2);
        leds.setPixel(55,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs8=7;
    } 
    else if (n >= 0.211) {
        leds.setPixel(7,Color_1);
        leds.setPixel(8,Color_1);
        leds.setPixel(23,Color_1);
        leds.setPixel(24,Color_1);
        leds.setPixel(39,Color_2);
        leds.setPixel(40,Color_2);
        leds.setPixel(55,Color_2);
        leds.setPixel(56,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs8=8;
    } 
  }
  else if (i > 1000){
    if (cs8 == 0 and n >= .015){
        leds.setPixel(7,Color_1);
        leds.setPixel(8,OFF);
        leds.setPixel(23,OFF);
        leds.setPixel(24,OFF);
        leds.setPixel(39,OFF);
        leds.setPixel(40,OFF);
        leds.setPixel(55,OFF);
        leds.setPixel(56,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs8=1;     
    }
    if (cs8 == 0 and n < .015){
        leds.setPixel(7,OFF);
        leds.setPixel(8,OFF);
        leds.setPixel(23,OFF);
        leds.setPixel(24,OFF);
        leds.setPixel(39,OFF);
        leds.setPixel(40,OFF);
        leds.setPixel(55,OFF);
        leds.setPixel(56,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs8=0;     
    }
    if (cs8 == 1 and n >= .035){
        leds.setPixel(7,Color_1);
        leds.setPixel(8,Color_1);
        leds.setPixel(23,OFF);
        leds.setPixel(24,OFF);
        leds.setPixel(39,OFF);
        leds.setPixel(40,OFF);
        leds.setPixel(55,OFF);
        leds.setPixel(56,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs8=2;
    }
    else if (cs8 == 1 and n <= .015){
        leds.setPixel(7,OFF);
        leds.setPixel(8,OFF);
        leds.setPixel(23,OFF);
        leds.setPixel(24,OFF);
        leds.setPixel(39,OFF);
        leds.setPixel(40,OFF);
        leds.setPixel(55,OFF);
        leds.setPixel(56,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs8=0;
    }
    else if (cs8 == 2 and n >= .06){
        leds.setPixel(7,Color_1);
        leds.setPixel(8,Color_1);
        leds.setPixel(23,Color_1);
        leds.setPixel(24,OFF);
        leds.setPixel(39,OFF);
        leds.setPixel(40,OFF);
        leds.setPixel(55,OFF);
        leds.setPixel(56,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs8=3;     
    }
    else if (cs8 == 2 and n <= .035){
        leds.setPixel(7,Color_1);
        leds.setPixel(8,OFF);
        leds.setPixel(23,OFF);
        leds.setPixel(24,OFF);
        leds.setPixel(39,OFF);
        leds.setPixel(40,OFF);
        leds.setPixel(55,OFF);
        leds.setPixel(56,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs8=1;
    }
    else if (cs8 == 3 and n >= .09){
        leds.setPixel(7,Color_1);
        leds.setPixel(8,Color_1);
        leds.setPixel(23,Color_1);
        leds.setPixel(24,Color_1);
        leds.setPixel(39,OFF);
        leds.setPixel(40,OFF);
        leds.setPixel(55,OFF);
        leds.setPixel(56,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs8=4;
    }
    else if (cs8 == 3 and n <= .06){
        leds.setPixel(7,Color_1);
        leds.setPixel(8,Color_1);
        leds.setPixel(23,OFF);
        leds.setPixel(24,OFF);
        leds.setPixel(39,OFF);
        leds.setPixel(40,OFF);
        leds.setPixel(55,OFF);
        leds.setPixel(56,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs8=2;
    }
    else if (cs8 == 4 and n >= .12){
        leds.setPixel(7,Color_1);
        leds.setPixel(8,Color_1);
        leds.setPixel(23,Color_1);
        leds.setPixel(24,Color_1);
        leds.setPixel(39,Color_2);
        leds.setPixel(40,OFF);
        leds.setPixel(55,OFF);
        leds.setPixel(56,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs8=6;
    }
    else if (cs8 == 4 and n <= .09){
        leds.setPixel(7,Color_1);
        leds.setPixel(8,Color_1);
        leds.setPixel(23,Color_1);
        leds.setPixel(24,OFF);
        leds.setPixel(39,OFF);
        leds.setPixel(40,OFF);
        leds.setPixel(55,OFF);
        leds.setPixel(56,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs8=3;
    }
    else if (cs8 == 5 and n >= .15){
        leds.setPixel(7,Color_1);
        leds.setPixel(8,Color_1);
        leds.setPixel(23,Color_1);
        leds.setPixel(24,Color_1);
        leds.setPixel(39,Color_2);
        leds.setPixel(40,Color_2);
        leds.setPixel(55,OFF);
        leds.setPixel(56,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs8=6;      
    }
    if (cs8 == 5 and n <= .12){
        leds.setPixel(7,Color_1);
        leds.setPixel(8,Color_1);
        leds.setPixel(23,Color_1);
        leds.setPixel(24,Color_1);
        leds.setPixel(39,OFF);
        leds.setPixel(40,OFF);
        leds.setPixel(55,OFF);
        leds.setPixel(56,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs8=4;    
    }
    else if (cs8 == 6 and n >= .18){
        leds.setPixel(7,Color_1);
        leds.setPixel(8,Color_1);
        leds.setPixel(23,Color_1);
        leds.setPixel(24,Color_1);
        leds.setPixel(39,Color_2);
        leds.setPixel(40,Color_2);
        leds.setPixel(55,Color_2);
        leds.setPixel(56,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs8=7;      
    }
    else if (cs8 == 6 and n <= .15){
        leds.setPixel(7,Color_1);
        leds.setPixel(8,Color_1);
        leds.setPixel(23,Color_1);
        leds.setPixel(24,Color_1);
        leds.setPixel(39,Color_2);
        leds.setPixel(40,OFF);
        leds.setPixel(55,OFF);
        leds.setPixel(56,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs8=5;     
    }
    else if (cs8 == 7 and n >= .21){
        leds.setPixel(7,Color_1);
        leds.setPixel(8,Color_1);
        leds.setPixel(23,Color_1);
        leds.setPixel(24,Color_1);
        leds.setPixel(39,Color_2);
        leds.setPixel(40,Color_2);
        leds.setPixel(55,Color_2);
        leds.setPixel(56,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs8=8;      
    }
    else if (cs8 == 7 and n <= .18){
        leds.setPixel(7,Color_1);
        leds.setPixel(8,Color_1);
        leds.setPixel(23,Color_1);
        leds.setPixel(24,Color_1);
        leds.setPixel(39,Color_2);
        leds.setPixel(40,Color_2);
        leds.setPixel(55,OFF);
        leds.setPixel(56,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs8=6;
    }
    if (cs8 == 8 and n >= .21){
        leds.setPixel(7,Color_1);
        leds.setPixel(8,Color_1);
        leds.setPixel(23,Color_1);
        leds.setPixel(24,Color_1);
        leds.setPixel(39,Color_2);
        leds.setPixel(40,Color_2);
        leds.setPixel(55,Color_2);
        leds.setPixel(56,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs8=8;     
    }
    else if (cs8 == 8 and n < .21){
        leds.setPixel(7,Color_1);
        leds.setPixel(8,Color_1);
        leds.setPixel(23,Color_1);
        leds.setPixel(24,Color_1);
        leds.setPixel(39,Color_2);
        leds.setPixel(40,Color_2);
        leds.setPixel(55,Color_2);
        leds.setPixel(56,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs8=7;       
    }    
  }
}

void Bar9(int wait, float n, int i){

  if (i == 1000){
    if (n >= 0.015 and n <= 0.035) {
        leds.setPixel(64,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs9=1;
    }
    else if (n >= 0.036 and n <= 0.06) {
        leds.setPixel(64,Color_1);
        leds.setPixel(79,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs9=2;
    }
     else if (n >= 0.061 and n <= 0.08) {
        leds.setPixel(64,Color_1);
        leds.setPixel(79,Color_1);
        leds.setPixel(80,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs9=3;
    }
      else if (n >= 0.081 and n <= 0.11) {
        leds.setPixel(64,Color_1);
        leds.setPixel(79,Color_1);
        leds.setPixel(80,Color_1);
        leds.setPixel(95,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs9=4;
    } 
    else if (n >= 0.111 and n <= 0.14) {
        leds.setPixel(64,Color_1);
        leds.setPixel(79,Color_1);
        leds.setPixel(80,Color_1);
        leds.setPixel(95,Color_1);
        leds.setPixel(96,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs9=5;
    } 
    else if (n >= 0.141 and n <= 0.17) {
        leds.setPixel(64,Color_1);
        leds.setPixel(79,Color_1);
        leds.setPixel(80,Color_1);
        leds.setPixel(95,Color_1);
        leds.setPixel(96,Color_2);
        leds.setPixel(111,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs9=6;
    } 
  
    else if (n >= 0.171 and n <= 0.210) {
        leds.setPixel(64,Color_1);
        leds.setPixel(79,Color_1);
        leds.setPixel(80,Color_1);
        leds.setPixel(95,Color_1);
        leds.setPixel(96,Color_2);
        leds.setPixel(111,Color_2);
        leds.setPixel(112,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs9=7;
    } 
    else if (n >= 0.211) {
        leds.setPixel(64,Color_1);
        leds.setPixel(79,Color_1);
        leds.setPixel(80,Color_1);
        leds.setPixel(95,Color_1);
        leds.setPixel(96,Color_2);
        leds.setPixel(111,Color_2);
        leds.setPixel(112,Color_2);
        leds.setPixel(127,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs9=8;
    } 
  }
  else if (i > 1000){
    if (cs9 == 0 and n >= .015){
        leds.setPixel(64,Color_1);
        leds.setPixel(79,OFF);
        leds.setPixel(80,OFF);
        leds.setPixel(95,OFF);
        leds.setPixel(96,OFF);
        leds.setPixel(111,OFF);
        leds.setPixel(112,OFF);
        leds.setPixel(127,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs9=1;     
    }
    if (cs9 == 0 and n < .015){
        leds.setPixel(64,OFF);
        leds.setPixel(79,OFF);
        leds.setPixel(80,OFF);
        leds.setPixel(95,OFF);
        leds.setPixel(96,OFF);
        leds.setPixel(111,OFF);
        leds.setPixel(112,OFF);
        leds.setPixel(127,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs9=0;     
    }
    if (cs9 == 1 and n >= .035){
        leds.setPixel(64,Color_1);
        leds.setPixel(79,Color_1);
        leds.setPixel(80,OFF);
        leds.setPixel(95,OFF);
        leds.setPixel(96,OFF);
        leds.setPixel(111,OFF);
        leds.setPixel(112,OFF);
        leds.setPixel(127,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs9=2;
    }
    else if (cs9 == 1 and n <= .015){
        leds.setPixel(64,OFF);
        leds.setPixel(79,OFF);
        leds.setPixel(80,OFF);
        leds.setPixel(95,OFF);
        leds.setPixel(96,OFF);
        leds.setPixel(111,OFF);
        leds.setPixel(112,OFF);
        leds.setPixel(127,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs9=0;
    }
    else if (cs9 == 2 and n >= .06){
        leds.setPixel(64,Color_1);
        leds.setPixel(79,Color_1);
        leds.setPixel(80,Color_1);
        leds.setPixel(95,OFF);
        leds.setPixel(96,OFF);
        leds.setPixel(111,OFF);
        leds.setPixel(112,OFF);
        leds.setPixel(127,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs9=3;     
    }
    else if (cs9 == 2 and n <= .035){
        leds.setPixel(64,Color_1);
        leds.setPixel(79,OFF);
        leds.setPixel(80,OFF);
        leds.setPixel(95,OFF);
        leds.setPixel(96,OFF);
        leds.setPixel(111,OFF);
        leds.setPixel(112,OFF);
        leds.setPixel(127,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs9=1;
    }
    else if (cs9 == 3 and n >= .09){
        leds.setPixel(64,Color_1);
        leds.setPixel(79,Color_1);
        leds.setPixel(80,Color_1);
        leds.setPixel(95,Color_1);
        leds.setPixel(96,OFF);
        leds.setPixel(111,OFF);
        leds.setPixel(112,OFF);
        leds.setPixel(127,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs9=4;
    }
    else if (cs9 == 3 and n <= .06){
        leds.setPixel(64,Color_1);
        leds.setPixel(79,Color_1);
        leds.setPixel(80,OFF);
        leds.setPixel(95,OFF);
        leds.setPixel(96,OFF);
        leds.setPixel(111,OFF);
        leds.setPixel(112,OFF);
        leds.setPixel(127,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs9=2;
    }
    else if (cs9 == 4 and n >= .12){
        leds.setPixel(64,Color_1);
        leds.setPixel(79,Color_1);
        leds.setPixel(80,Color_1);
        leds.setPixel(95,Color_1);
        leds.setPixel(96,Color_2);
        leds.setPixel(111,OFF);
        leds.setPixel(112,OFF);
        leds.setPixel(127,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs9=6;
    }
    else if (cs9 == 4 and n <= .09){
        leds.setPixel(64,Color_1);
        leds.setPixel(79,Color_1);
        leds.setPixel(80,Color_1);
        leds.setPixel(95,OFF);
        leds.setPixel(96,OFF);
        leds.setPixel(111,OFF);
        leds.setPixel(112,OFF);
        leds.setPixel(127,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs9=3;
    }
    else if (cs9 == 5 and n >= .15){
        leds.setPixel(64,Color_1);
        leds.setPixel(79,Color_1);
        leds.setPixel(80,Color_1);
        leds.setPixel(95,Color_1);
        leds.setPixel(96,Color_2);
        leds.setPixel(111,Color_2);
        leds.setPixel(112,OFF);
        leds.setPixel(127,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs9=6;      
    }
    if (cs9 == 5 and n <= .12){
        leds.setPixel(64,Color_1);
        leds.setPixel(79,Color_1);
        leds.setPixel(80,Color_1);
        leds.setPixel(95,Color_1);
        leds.setPixel(96,OFF);
        leds.setPixel(111,OFF);
        leds.setPixel(112,OFF);
        leds.setPixel(127,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs9=4;    
    }
    else if (cs9 == 6 and n >= .18){
        leds.setPixel(64,Color_1);
        leds.setPixel(79,Color_1);
        leds.setPixel(80,Color_1);
        leds.setPixel(95,Color_1);
        leds.setPixel(96,Color_2);
        leds.setPixel(111,Color_2);
        leds.setPixel(112,Color_2);
        leds.setPixel(127,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs9=7;      
    }
    else if (cs9 == 6 and n <= .15){
        leds.setPixel(64,Color_1);
        leds.setPixel(79,Color_1);
        leds.setPixel(80,Color_1);
        leds.setPixel(95,Color_1);
        leds.setPixel(96,Color_2);
        leds.setPixel(111,OFF);
        leds.setPixel(112,OFF);
        leds.setPixel(127,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs9=5;     
    }
    else if (cs9 == 7 and n >= .21){
        leds.setPixel(64,Color_1);
        leds.setPixel(79,Color_1);
        leds.setPixel(80,Color_1);
        leds.setPixel(95,Color_1);
        leds.setPixel(96,Color_2);
        leds.setPixel(111,Color_2);
        leds.setPixel(112,Color_2);
        leds.setPixel(127,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs9=8;      
    }
    else if (cs9 == 7 and n <= .18){
        leds.setPixel(64,Color_1);
        leds.setPixel(79,Color_1);
        leds.setPixel(80,Color_1);
        leds.setPixel(95,Color_1);
        leds.setPixel(96,Color_2);
        leds.setPixel(111,Color_2);
        leds.setPixel(112,OFF);
        leds.setPixel(127,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs9=6;
    }
    if (cs9 == 8 and n >= .21){
        leds.setPixel(64,Color_1);
        leds.setPixel(79,Color_1);
        leds.setPixel(80,Color_1);
        leds.setPixel(95,Color_1);
        leds.setPixel(96,Color_2);
        leds.setPixel(111,Color_2);
        leds.setPixel(112,Color_2);
        leds.setPixel(127,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs9=8;     
    }
    else if (cs9 == 8 and n < .21){
        leds.setPixel(64,Color_1);
        leds.setPixel(79,Color_1);
        leds.setPixel(80,Color_1);
        leds.setPixel(95,Color_1);
        leds.setPixel(96,Color_2);
        leds.setPixel(111,Color_2);
        leds.setPixel(112,Color_2);
        leds.setPixel(127,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs9=7;       
    }    
  }
}

void Bar10(int wait, float n, int i){

  if (i == 1000){
    if (n >= 0.015 and n <= 0.035) {
        leds.setPixel(65,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs10=1;
    }
    else if (n >= 0.036 and n <= 0.06) {
        leds.setPixel(65,Color_1);
        leds.setPixel(78,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs10=2;
    }
     else if (n >= 0.061 and n <= 0.08) {
        leds.setPixel(65,Color_1);
        leds.setPixel(78,Color_1);
        leds.setPixel(81,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs10=3;
    }
      else if (n >= 0.081 and n <= 0.11) {
        leds.setPixel(65,Color_1);
        leds.setPixel(78,Color_1);
        leds.setPixel(81,Color_1);
        leds.setPixel(94,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs10=4;
    } 
    else if (n >= 0.111 and n <= 0.14) {
        leds.setPixel(65,Color_1);
        leds.setPixel(78,Color_1);
        leds.setPixel(81,Color_1);
        leds.setPixel(94,Color_1);
        leds.setPixel(97,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs10=5;
    } 
    else if (n >= 0.141 and n <= 0.17) {
        leds.setPixel(65,Color_1);
        leds.setPixel(78,Color_1);
        leds.setPixel(81,Color_1);
        leds.setPixel(94,Color_1);
        leds.setPixel(97,Color_2);
        leds.setPixel(110,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs10=6;
    } 
  
    else if (n >= 0.171 and n <= 0.210) {
        leds.setPixel(65,Color_1);
        leds.setPixel(78,Color_1);
        leds.setPixel(81,Color_1);
        leds.setPixel(94,Color_1);
        leds.setPixel(97,Color_2);
        leds.setPixel(110,Color_2);
        leds.setPixel(113,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs10=7;
    } 
    else if (n >= 0.211) {
        leds.setPixel(65,Color_1);
        leds.setPixel(78,Color_1);
        leds.setPixel(81,Color_1);
        leds.setPixel(94,Color_1);
        leds.setPixel(97,Color_2);
        leds.setPixel(110,Color_2);
        leds.setPixel(113,Color_2);
        leds.setPixel(126,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs10=8;
    } 
  }
  else if (i > 1000){
    if (cs10 == 0 and n >= .015){
        leds.setPixel(65,Color_1);
        leds.setPixel(78,OFF);
        leds.setPixel(81,OFF);
        leds.setPixel(94,OFF);
        leds.setPixel(97,OFF);
        leds.setPixel(110,OFF);
        leds.setPixel(113,OFF);
        leds.setPixel(126,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs10=1;     
    }
    if (cs10 == 0 and n < .015){
        leds.setPixel(65,OFF);
        leds.setPixel(78,OFF);
        leds.setPixel(81,OFF);
        leds.setPixel(94,OFF);
        leds.setPixel(97,OFF);
        leds.setPixel(110,OFF);
        leds.setPixel(113,OFF);
        leds.setPixel(126,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs10=0;     
    }
    if (cs10 == 1 and n >= .035){
        leds.setPixel(65,Color_1);
        leds.setPixel(78,Color_1);
        leds.setPixel(81,OFF);
        leds.setPixel(94,OFF);
        leds.setPixel(97,OFF);
        leds.setPixel(110,OFF);
        leds.setPixel(113,OFF);
        leds.setPixel(126,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs10=2;
    }
    else if (cs10 == 1 and n <= .015){
        leds.setPixel(65,OFF);
        leds.setPixel(78,OFF);
        leds.setPixel(81,OFF);
        leds.setPixel(94,OFF);
        leds.setPixel(97,OFF);
        leds.setPixel(110,OFF);
        leds.setPixel(113,OFF);
        leds.setPixel(126,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs10=0;
    }
    else if (cs10 == 2 and n >= .06){
        leds.setPixel(65,Color_1);
        leds.setPixel(78,Color_1);
        leds.setPixel(81,Color_1);
        leds.setPixel(94,OFF);
        leds.setPixel(97,OFF);
        leds.setPixel(110,OFF);
        leds.setPixel(113,OFF);
        leds.setPixel(126,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs10=3;     
    }
    else if (cs10 == 2 and n <= .035){
        leds.setPixel(65,Color_1);
        leds.setPixel(78,OFF);
        leds.setPixel(81,OFF);
        leds.setPixel(94,OFF);
        leds.setPixel(97,OFF);
        leds.setPixel(110,OFF);
        leds.setPixel(113,OFF);
        leds.setPixel(126,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs10=1;
    }
    else if (cs10 == 3 and n >= .09){
        leds.setPixel(65,Color_1);
        leds.setPixel(78,Color_1);
        leds.setPixel(81,Color_1);
        leds.setPixel(94,Color_1);
        leds.setPixel(97,OFF);
        leds.setPixel(110,OFF);
        leds.setPixel(113,OFF);
        leds.setPixel(126,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs10=4;
    }
    else if (cs10 == 3 and n <= .06){
        leds.setPixel(65,Color_1);
        leds.setPixel(78,Color_1);
        leds.setPixel(81,OFF);
        leds.setPixel(94,OFF);
        leds.setPixel(97,OFF);
        leds.setPixel(110,OFF);
        leds.setPixel(113,OFF);
        leds.setPixel(126,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs10=2;
    }
    else if (cs10 == 4 and n >= .12){
        leds.setPixel(65,Color_1);
        leds.setPixel(78,Color_1);
        leds.setPixel(81,Color_1);
        leds.setPixel(94,Color_1);
        leds.setPixel(97,Color_2);
        leds.setPixel(110,OFF);
        leds.setPixel(113,OFF);
        leds.setPixel(126,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs10=6;
    }
    else if (cs10 == 4 and n <= .09){
        leds.setPixel(65,Color_1);
        leds.setPixel(78,Color_1);
        leds.setPixel(81,Color_1);
        leds.setPixel(94,OFF);
        leds.setPixel(97,OFF);
        leds.setPixel(110,OFF);
        leds.setPixel(113,OFF);
        leds.setPixel(126,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs10=3;
    }
    else if (cs10 == 5 and n >= .15){
        leds.setPixel(65,Color_1);
        leds.setPixel(78,Color_1);
        leds.setPixel(81,Color_1);
        leds.setPixel(94,Color_1);
        leds.setPixel(97,Color_2);
        leds.setPixel(110,Color_2);
        leds.setPixel(113,OFF);
        leds.setPixel(126,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs10=6;      
    }
    if (cs10 == 5 and n <= .12){
        leds.setPixel(65,Color_1);
        leds.setPixel(78,Color_1);
        leds.setPixel(81,Color_1);
        leds.setPixel(94,Color_1);
        leds.setPixel(97,OFF);
        leds.setPixel(110,OFF);
        leds.setPixel(113,OFF);
        leds.setPixel(126,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs10=4;    
    }
    else if (cs10 == 6 and n >= .18){
        leds.setPixel(65,Color_1);
        leds.setPixel(78,Color_1);
        leds.setPixel(81,Color_1);
        leds.setPixel(94,Color_1);
        leds.setPixel(97,Color_2);
        leds.setPixel(110,Color_2);
        leds.setPixel(113,Color_2);
        leds.setPixel(126,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs10=7;      
    }
    else if (cs10 == 6 and n <= .15){
        leds.setPixel(65,Color_1);
        leds.setPixel(78,Color_1);
        leds.setPixel(81,Color_1);
        leds.setPixel(94,Color_1);
        leds.setPixel(97,Color_2);
        leds.setPixel(110,OFF);
        leds.setPixel(113,OFF);
        leds.setPixel(126,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs10=5;     
    }
    else if (cs10 == 7 and n >= .21){
        leds.setPixel(65,Color_1);
        leds.setPixel(78,Color_1);
        leds.setPixel(81,Color_1);
        leds.setPixel(94,Color_1);
        leds.setPixel(97,Color_2);
        leds.setPixel(110,Color_2);
        leds.setPixel(113,Color_2);
        leds.setPixel(126,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs10=8;      
    }
    else if (cs10 == 7 and n <= .18){
        leds.setPixel(65,Color_1);
        leds.setPixel(78,Color_1);
        leds.setPixel(81,Color_1);
        leds.setPixel(94,Color_1);
        leds.setPixel(97,Color_2);
        leds.setPixel(110,Color_2);
        leds.setPixel(113,OFF);
        leds.setPixel(126,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs10=6;
    }
    if (cs10 == 8 and n >= .21){
        leds.setPixel(65,Color_1);
        leds.setPixel(78,Color_1);
        leds.setPixel(81,Color_1);
        leds.setPixel(94,Color_1);
        leds.setPixel(97,Color_2);
        leds.setPixel(110,Color_2);
        leds.setPixel(113,Color_2);
        leds.setPixel(126,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs10=8;     
    }
    else if (cs10 == 8 and n < .21){
        leds.setPixel(65,Color_1);
        leds.setPixel(78,Color_1);
        leds.setPixel(81,Color_1);
        leds.setPixel(94,Color_1);
        leds.setPixel(97,Color_2);
        leds.setPixel(110,Color_2);
        leds.setPixel(113,Color_2);
        leds.setPixel(126,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs10=7;       
    }    
  }
}

void Bar11(int wait, float n, int i){

  if (i == 1000){
    if (n >= 0.015 and n <= 0.035) {
        leds.setPixel(66,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs11=1;
    }
    else if (n >= 0.036 and n <= 0.06) {
        leds.setPixel(66,Color_1);
        leds.setPixel(77,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs11=2;
    }
     else if (n >= 0.061 and n <= 0.08) {
        leds.setPixel(66,Color_1);
        leds.setPixel(77,Color_1);
        leds.setPixel(82,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs11=3;
    }
      else if (n >= 0.081 and n <= 0.11) {
        leds.setPixel(66,Color_1);
        leds.setPixel(77,Color_1);
        leds.setPixel(82,Color_1);
        leds.setPixel(93,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs11=4;
    } 
    else if (n >= 0.111 and n <= 0.14) {
        leds.setPixel(66,Color_1);
        leds.setPixel(77,Color_1);
        leds.setPixel(82,Color_1);
        leds.setPixel(93,Color_1);
        leds.setPixel(98,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs11=5;
    } 
    else if (n >= 0.141 and n <= 0.17) {
        leds.setPixel(66,Color_1);
        leds.setPixel(77,Color_1);
        leds.setPixel(82,Color_1);
        leds.setPixel(93,Color_1);
        leds.setPixel(98,Color_2);
        leds.setPixel(109,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs11=6;
    } 
  
    else if (n >= 0.171 and n <= 0.210) {
        leds.setPixel(66,Color_1);
        leds.setPixel(77,Color_1);
        leds.setPixel(82,Color_1);
        leds.setPixel(93,Color_1);
        leds.setPixel(98,Color_2);
        leds.setPixel(109,Color_2);
        leds.setPixel(114,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs11=7;
    } 
    else if (n >= 0.211) {
        leds.setPixel(66,Color_1);
        leds.setPixel(77,Color_1);
        leds.setPixel(82,Color_1);
        leds.setPixel(93,Color_1);
        leds.setPixel(98,Color_2);
        leds.setPixel(109,Color_2);
        leds.setPixel(114,Color_2);
        leds.setPixel(125,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs11=8;
    } 
  }
  else if (i > 1000){
    if (cs11 == 0 and n >= .015){
        leds.setPixel(66,Color_1);
        leds.setPixel(77,OFF);
        leds.setPixel(82,OFF);
        leds.setPixel(93,OFF);
        leds.setPixel(98,OFF);
        leds.setPixel(109,OFF);
        leds.setPixel(114,OFF);
        leds.setPixel(125,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs11=1;     
    }
    if (cs11 == 0 and n < .015){
        leds.setPixel(66,OFF);
        leds.setPixel(77,OFF);
        leds.setPixel(82,OFF);
        leds.setPixel(93,OFF);
        leds.setPixel(98,OFF);
        leds.setPixel(109,OFF);
        leds.setPixel(114,OFF);
        leds.setPixel(125,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs7=0;     
    }
    if (cs11 == 1 and n >= .035){
        leds.setPixel(66,Color_1);
        leds.setPixel(77,Color_1);
        leds.setPixel(82,OFF);
        leds.setPixel(93,OFF);
        leds.setPixel(98,OFF);
        leds.setPixel(109,OFF);
        leds.setPixel(114,OFF);
        leds.setPixel(125,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs11=2;
    }
    else if (cs11 == 1 and n <= .015){
        leds.setPixel(66,OFF);
        leds.setPixel(77,OFF);
        leds.setPixel(82,OFF);
        leds.setPixel(93,OFF);
        leds.setPixel(98,OFF);
        leds.setPixel(109,OFF);
        leds.setPixel(114,OFF);
        leds.setPixel(125,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs11=0;
    }
    else if (cs11 == 2 and n >= .06){
        leds.setPixel(66,Color_1);
        leds.setPixel(77,Color_1);
        leds.setPixel(82,Color_1);
        leds.setPixel(93,OFF);
        leds.setPixel(98,OFF);
        leds.setPixel(109,OFF);
        leds.setPixel(114,OFF);
        leds.setPixel(125,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs11=3;     
    }
    else if (cs11 == 2 and n <= .035){
        leds.setPixel(66,Color_1);
        leds.setPixel(77,OFF);
        leds.setPixel(82,OFF);
        leds.setPixel(93,OFF);
        leds.setPixel(98,OFF);
        leds.setPixel(109,OFF);
        leds.setPixel(114,OFF);
        leds.setPixel(125,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs11=1;
    }
    else if (cs11 == 3 and n >= .09){
        leds.setPixel(66,Color_1);
        leds.setPixel(77,Color_1);
        leds.setPixel(82,Color_1);
        leds.setPixel(93,Color_1);
        leds.setPixel(98,OFF);
        leds.setPixel(109,OFF);
        leds.setPixel(114,OFF);
        leds.setPixel(125,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs11=4;
    }
    else if (cs11 == 3 and n <= .06){
        leds.setPixel(66,Color_1);
        leds.setPixel(77,Color_1);
        leds.setPixel(82,OFF);
        leds.setPixel(93,OFF);
        leds.setPixel(98,OFF);
        leds.setPixel(109,OFF);
        leds.setPixel(114,OFF);
        leds.setPixel(125,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs11=2;
    }
    else if (cs11 == 4 and n >= .12){
        leds.setPixel(66,Color_1);
        leds.setPixel(77,Color_1);
        leds.setPixel(82,Color_1);
        leds.setPixel(93,Color_1);
        leds.setPixel(98,Color_2);
        leds.setPixel(109,OFF);
        leds.setPixel(114,OFF);
        leds.setPixel(125,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs11=6;
    }
    else if (cs11 == 4 and n <= .09){
        leds.setPixel(66,Color_1);
        leds.setPixel(77,Color_1);
        leds.setPixel(82,Color_1);
        leds.setPixel(93,OFF);
        leds.setPixel(98,OFF);
        leds.setPixel(109,OFF);
        leds.setPixel(114,OFF);
        leds.setPixel(125,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs11=3;
    }
    else if (cs11 == 5 and n >= .15){
        leds.setPixel(66,Color_1);
        leds.setPixel(77,Color_1);
        leds.setPixel(82,Color_1);
        leds.setPixel(93,Color_1);
        leds.setPixel(98,Color_2);
        leds.setPixel(109,Color_2);
        leds.setPixel(114,OFF);
        leds.setPixel(125,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs11=6;      
    }
    if (cs11 == 5 and n <= .12){
        leds.setPixel(66,Color_1);
        leds.setPixel(77,Color_1);
        leds.setPixel(82,Color_1);
        leds.setPixel(93,Color_1);
        leds.setPixel(98,OFF);
        leds.setPixel(109,OFF);
        leds.setPixel(114,OFF);
        leds.setPixel(125,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs11=4;    
    }
    else if (cs11 == 6 and n >= .18){
        leds.setPixel(66,Color_1);
        leds.setPixel(77,Color_1);
        leds.setPixel(82,Color_1);
        leds.setPixel(93,Color_1);
        leds.setPixel(98,Color_2);
        leds.setPixel(109,Color_2);
        leds.setPixel(114,Color_2);
        leds.setPixel(125,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs11=7;      
    }
    else if (cs11 == 6 and n <= .15){
        leds.setPixel(66,Color_1);
        leds.setPixel(77,Color_1);
        leds.setPixel(82,Color_1);
        leds.setPixel(93,Color_1);
        leds.setPixel(98,Color_2);
        leds.setPixel(109,OFF);
        leds.setPixel(114,OFF);
        leds.setPixel(125,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs11=5;     
    }
    else if (cs11 == 7 and n >= .21){
        leds.setPixel(66,Color_1);
        leds.setPixel(77,Color_1);
        leds.setPixel(82,Color_1);
        leds.setPixel(93,Color_1);
        leds.setPixel(98,Color_2);
        leds.setPixel(109,Color_2);
        leds.setPixel(114,Color_2);
        leds.setPixel(125,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs11=8;      
    }
    else if (cs11 == 7 and n <= .18){
        leds.setPixel(66,Color_1);
        leds.setPixel(77,Color_1);
        leds.setPixel(82,Color_1);
        leds.setPixel(93,Color_1);
        leds.setPixel(98,Color_2);
        leds.setPixel(109,Color_2);
        leds.setPixel(114,OFF);
        leds.setPixel(125,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs11=6;
    }
    if (cs11 == 8 and n >= .21){
        leds.setPixel(66,Color_1);
        leds.setPixel(77,Color_1);
        leds.setPixel(82,Color_1);
        leds.setPixel(93,Color_1);
        leds.setPixel(98,Color_2);
        leds.setPixel(109,Color_2);
        leds.setPixel(114,Color_2);
        leds.setPixel(125,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs11=8;     
    }
    else if (cs11 == 8 and n < .21){
        leds.setPixel(66,Color_1);
        leds.setPixel(77,Color_1);
        leds.setPixel(82,Color_1);
        leds.setPixel(93,Color_1);
        leds.setPixel(98,Color_2);
        leds.setPixel(109,Color_2);
        leds.setPixel(114,Color_2);
        leds.setPixel(125,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs11=7;       
    }    
  }
}

void Bar12(int wait, float n, int i){

  if (i == 1000){
    if (n >= 0.015 and n <= 0.035) {
        leds.setPixel(67,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs12=1;
    }
    else if (n >= 0.036 and n <= 0.06) {
        leds.setPixel(67,Color_1);
        leds.setPixel(76,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs12=2;
    }
     else if (n >= 0.061 and n <= 0.08) {
        leds.setPixel(67,Color_1);
        leds.setPixel(76,Color_1);
        leds.setPixel(83,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs12=3;
    }
      else if (n >= 0.081 and n <= 0.11) {
        leds.setPixel(67,Color_1);
        leds.setPixel(76,Color_1);
        leds.setPixel(83,Color_1);
        leds.setPixel(92,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs12=4;
    } 
    else if (n >= 0.111 and n <= 0.14) {
        leds.setPixel(67,Color_1);
        leds.setPixel(76,Color_1);
        leds.setPixel(83,Color_1);
        leds.setPixel(92,Color_1);
        leds.setPixel(99,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs12=5;
    } 
    else if (n >= 0.141 and n <= 0.17) {
        leds.setPixel(67,Color_1);
        leds.setPixel(76,Color_1);
        leds.setPixel(83,Color_1);
        leds.setPixel(92,Color_1);
        leds.setPixel(99,Color_2);
        leds.setPixel(108,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs12=6;
    } 
  
    else if (n >= 0.171 and n <= 0.210) {
        leds.setPixel(67,Color_1);
        leds.setPixel(76,Color_1);
        leds.setPixel(83,Color_1);
        leds.setPixel(92,Color_1);
        leds.setPixel(99,Color_2);
        leds.setPixel(108,Color_2);
        leds.setPixel(115,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs12=7;
    } 
    else if (n >= 0.211) {
        leds.setPixel(67,Color_1);
        leds.setPixel(76,Color_1);
        leds.setPixel(83,Color_1);
        leds.setPixel(92,Color_1);
        leds.setPixel(99,Color_2);
        leds.setPixel(108,Color_2);
        leds.setPixel(115,Color_2);
        leds.setPixel(124,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs12=8;
    } 
  }
  else if (i > 1000){
    if (cs12 == 0 and n >= .015){
        leds.setPixel(67,Color_1);
        leds.setPixel(76,OFF);
        leds.setPixel(83,OFF);
        leds.setPixel(92,OFF);
        leds.setPixel(99,OFF);
        leds.setPixel(108,OFF);
        leds.setPixel(115,OFF);
        leds.setPixel(124,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs12=1;     
    }
    if (cs12 == 0 and n < .015){
        leds.setPixel(67,OFF);
        leds.setPixel(76,OFF);
        leds.setPixel(83,OFF);
        leds.setPixel(92,OFF);
        leds.setPixel(99,OFF);
        leds.setPixel(108,OFF);
        leds.setPixel(115,OFF);
        leds.setPixel(124,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs7=0;     
    }
    if (cs12 == 1 and n >= .035){
        leds.setPixel(67,Color_1);
        leds.setPixel(76,Color_1);
        leds.setPixel(83,OFF);
        leds.setPixel(92,OFF);
        leds.setPixel(99,OFF);
        leds.setPixel(108,OFF);
        leds.setPixel(115,OFF);
        leds.setPixel(124,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs12=2;
    }
    else if (cs12 == 1 and n <= .015){
        leds.setPixel(67,OFF);
        leds.setPixel(76,OFF);
        leds.setPixel(83,OFF);
        leds.setPixel(92,OFF);
        leds.setPixel(99,OFF);
        leds.setPixel(108,OFF);
        leds.setPixel(115,OFF);
        leds.setPixel(124,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs12=0;
    }
    else if (cs12 == 2 and n >= .06){
        leds.setPixel(67,Color_1);
        leds.setPixel(76,Color_1);
        leds.setPixel(83,Color_1);
        leds.setPixel(92,OFF);
        leds.setPixel(99,OFF);
        leds.setPixel(108,OFF);
        leds.setPixel(115,OFF);
        leds.setPixel(124,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs12=3;     
    }
    else if (cs12 == 2 and n <= .035){
        leds.setPixel(67,Color_1);
        leds.setPixel(76,OFF);
        leds.setPixel(83,OFF);
        leds.setPixel(92,OFF);
        leds.setPixel(99,OFF);
        leds.setPixel(108,OFF);
        leds.setPixel(115,OFF);
        leds.setPixel(124,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs12=1;
    }
    else if (cs12 == 3 and n >= .09){
        leds.setPixel(67,Color_1);
        leds.setPixel(76,Color_1);
        leds.setPixel(83,Color_1);
        leds.setPixel(92,Color_1);
        leds.setPixel(99,OFF);
        leds.setPixel(108,OFF);
        leds.setPixel(115,OFF);
        leds.setPixel(124,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs12=4;
    }
    else if (cs12 == 3 and n <= .06){
        leds.setPixel(67,Color_1);
        leds.setPixel(76,Color_1);
        leds.setPixel(83,OFF);
        leds.setPixel(92,OFF);
        leds.setPixel(99,OFF);
        leds.setPixel(108,OFF);
        leds.setPixel(115,OFF);
        leds.setPixel(124,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs12=2;
    }
    else if (cs12 == 4 and n >= .12){
        leds.setPixel(67,Color_1);
        leds.setPixel(76,Color_1);
        leds.setPixel(83,Color_1);
        leds.setPixel(92,Color_1);
        leds.setPixel(99,Color_2);
        leds.setPixel(108,OFF);
        leds.setPixel(115,OFF);
        leds.setPixel(124,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs12=6;
    }
    else if (cs12 == 4 and n <= .09){
        leds.setPixel(67,Color_1);
        leds.setPixel(76,Color_1);
        leds.setPixel(83,Color_1);
        leds.setPixel(92,OFF);
        leds.setPixel(99,OFF);
        leds.setPixel(108,OFF);
        leds.setPixel(115,OFF);
        leds.setPixel(124,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs12=3;
    }
    else if (cs12 == 5 and n >= .15){
        leds.setPixel(67,Color_1);
        leds.setPixel(76,Color_1);
        leds.setPixel(83,Color_1);
        leds.setPixel(92,Color_1);
        leds.setPixel(99,Color_2);
        leds.setPixel(108,Color_2);
        leds.setPixel(115,OFF);
        leds.setPixel(124,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs12=6;      
    }
    if (cs12 == 5 and n <= .12){
        leds.setPixel(67,Color_1);
        leds.setPixel(76,Color_1);
        leds.setPixel(83,Color_1);
        leds.setPixel(92,Color_1);
        leds.setPixel(99,OFF);
        leds.setPixel(108,OFF);
        leds.setPixel(115,OFF);
        leds.setPixel(124,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs12=4;    
    }
    else if (cs12 == 6 and n >= .18){
        leds.setPixel(67,Color_1);
        leds.setPixel(76,Color_1);
        leds.setPixel(83,Color_1);
        leds.setPixel(92,Color_1);
        leds.setPixel(99,Color_2);
        leds.setPixel(108,Color_2);
        leds.setPixel(115,Color_2);
        leds.setPixel(124,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs12=7;      
    }
    else if (cs12 == 6 and n <= .15){
        leds.setPixel(67,Color_1);
        leds.setPixel(76,Color_1);
        leds.setPixel(83,Color_1);
        leds.setPixel(92,Color_1);
        leds.setPixel(99,Color_2);
        leds.setPixel(108,OFF);
        leds.setPixel(115,OFF);
        leds.setPixel(124,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs12=5;     
    }
    else if (cs12 == 7 and n >= .21){
        leds.setPixel(67,Color_1);
        leds.setPixel(76,Color_1);
        leds.setPixel(83,Color_1);
        leds.setPixel(92,Color_1);
        leds.setPixel(99,Color_2);
        leds.setPixel(108,Color_2);
        leds.setPixel(115,Color_2);
        leds.setPixel(124,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs12=8;      
    }
    else if (cs12 == 7 and n <= .18){
        leds.setPixel(67,Color_1);
        leds.setPixel(76,Color_1);
        leds.setPixel(83,Color_1);
        leds.setPixel(92,Color_1);
        leds.setPixel(99,Color_2);
        leds.setPixel(108,Color_2);
        leds.setPixel(115,OFF);
        leds.setPixel(124,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs12=6;
    }
    if (cs12 == 8 and n >= .21){
        leds.setPixel(67,Color_1);
        leds.setPixel(76,Color_1);
        leds.setPixel(83,Color_1);
        leds.setPixel(92,Color_1);
        leds.setPixel(99,Color_2);
        leds.setPixel(108,Color_2);
        leds.setPixel(115,Color_2);
        leds.setPixel(124,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs12=8;     
    }
    else if (cs12 == 8 and n < .21){
        leds.setPixel(67,Color_1);
        leds.setPixel(76,Color_1);
        leds.setPixel(83,Color_1);
        leds.setPixel(92,Color_1);
        leds.setPixel(99,Color_2);
        leds.setPixel(108,Color_2);
        leds.setPixel(115,Color_2);
        leds.setPixel(124,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs12=7;       
    }    
  }
}

void Bar13(int wait, float n, int i){

  if (i == 1000){
    if (n >= 0.015 and n <= 0.035) {
        leds.setPixel(68,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs13=1;
    }
    else if (n >= 0.036 and n <= 0.06) {
        leds.setPixel(68,Color_1);
        leds.setPixel(75,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs13=2;
    }
     else if (n >= 0.061 and n <= 0.08) {
        leds.setPixel(68,Color_1);
        leds.setPixel(75,Color_1);
        leds.setPixel(84,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs13=3;
    }
      else if (n >= 0.081 and n <= 0.11) {
        leds.setPixel(68,Color_1);
        leds.setPixel(75,Color_1);
        leds.setPixel(84,Color_1);
        leds.setPixel(91,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs13=4;
    } 
    else if (n >= 0.111 and n <= 0.14) {
        leds.setPixel(68,Color_1);
        leds.setPixel(75,Color_1);
        leds.setPixel(84,Color_1);
        leds.setPixel(91,Color_1);
        leds.setPixel(100,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs13=5;
    } 
    else if (n >= 0.141 and n <= 0.17) {
        leds.setPixel(68,Color_1);
        leds.setPixel(75,Color_1);
        leds.setPixel(84,Color_1);
        leds.setPixel(91,Color_1);
        leds.setPixel(100,Color_2);
        leds.setPixel(107,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs13=6;
    } 
  
    else if (n >= 0.171 and n <= 0.210) {
        leds.setPixel(68,Color_1);
        leds.setPixel(75,Color_1);
        leds.setPixel(84,Color_1);
        leds.setPixel(91,Color_1);
        leds.setPixel(100,Color_2);
        leds.setPixel(107,Color_2);
        leds.setPixel(116,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs13=7;
    } 
    else if (n >= 0.211) {
        leds.setPixel(68,Color_1);
        leds.setPixel(75,Color_1);
        leds.setPixel(84,Color_1);
        leds.setPixel(91,Color_1);
        leds.setPixel(100,Color_2);
        leds.setPixel(107,Color_2);
        leds.setPixel(116,Color_2);
        leds.setPixel(123,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs13=8;
    } 
  }
  else if (i > 1000){
    if (cs13 == 0 and n >= .015){
        leds.setPixel(68,Color_1);
        leds.setPixel(75,OFF);
        leds.setPixel(84,OFF);
        leds.setPixel(91,OFF);
        leds.setPixel(100,OFF);
        leds.setPixel(107,OFF);
        leds.setPixel(116,OFF);
        leds.setPixel(123,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs13=1;     
    }
    if (cs13 == 0 and n < .015){
        leds.setPixel(68,OFF);
        leds.setPixel(75,OFF);
        leds.setPixel(84,OFF);
        leds.setPixel(91,OFF);
        leds.setPixel(100,OFF);
        leds.setPixel(107,OFF);
        leds.setPixel(116,OFF);
        leds.setPixel(123,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs7=0;     
    }
    if (cs13 == 1 and n >= .035){
        leds.setPixel(68,Color_1);
        leds.setPixel(75,Color_1);
        leds.setPixel(84,OFF);
        leds.setPixel(91,OFF);
        leds.setPixel(100,OFF);
        leds.setPixel(107,OFF);
        leds.setPixel(116,OFF);
        leds.setPixel(123,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs13=2;
    }
    else if (cs13 == 1 and n <= .015){
        leds.setPixel(68,OFF);
        leds.setPixel(75,OFF);
        leds.setPixel(84,OFF);
        leds.setPixel(91,OFF);
        leds.setPixel(100,OFF);
        leds.setPixel(107,OFF);
        leds.setPixel(116,OFF);
        leds.setPixel(123,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs13=0;
    }
    else if (cs13 == 2 and n >= .06){
        leds.setPixel(68,Color_1);
        leds.setPixel(75,Color_1);
        leds.setPixel(84,Color_1);
        leds.setPixel(91,OFF);
        leds.setPixel(100,OFF);
        leds.setPixel(107,OFF);
        leds.setPixel(116,OFF);
        leds.setPixel(123,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs13=3;     
    }
    else if (cs13 == 2 and n <= .035){
        leds.setPixel(68,Color_1);
        leds.setPixel(75,OFF);
        leds.setPixel(84,OFF);
        leds.setPixel(91,OFF);
        leds.setPixel(100,OFF);
        leds.setPixel(107,OFF);
        leds.setPixel(116,OFF);
        leds.setPixel(123,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs13=1;
    }
    else if (cs13 == 3 and n >= .09){
        leds.setPixel(68,Color_1);
        leds.setPixel(75,Color_1);
        leds.setPixel(84,Color_1);
        leds.setPixel(91,Color_1);
        leds.setPixel(100,OFF);
        leds.setPixel(107,OFF);
        leds.setPixel(116,OFF);
        leds.setPixel(123,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs13=4;
    }
    else if (cs13 == 3 and n <= .06){
        leds.setPixel(68,Color_1);
        leds.setPixel(75,Color_1);
        leds.setPixel(84,OFF);
        leds.setPixel(91,OFF);
        leds.setPixel(100,OFF);
        leds.setPixel(107,OFF);
        leds.setPixel(116,OFF);
        leds.setPixel(123,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs13=2;
    }
    else if (cs13 == 4 and n >= .12){
        leds.setPixel(68,Color_1);
        leds.setPixel(75,Color_1);
        leds.setPixel(84,Color_1);
        leds.setPixel(91,Color_1);
        leds.setPixel(100,Color_2);
        leds.setPixel(107,OFF);
        leds.setPixel(116,OFF);
        leds.setPixel(123,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs13=6;
    }
    else if (cs13 == 4 and n <= .09){
        leds.setPixel(68,Color_1);
        leds.setPixel(75,Color_1);
        leds.setPixel(84,Color_1);
        leds.setPixel(91,OFF);
        leds.setPixel(100,OFF);
        leds.setPixel(107,OFF);
        leds.setPixel(116,OFF);
        leds.setPixel(123,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs13=3;
    }
    else if (cs13 == 5 and n >= .15){
        leds.setPixel(68,Color_1);
        leds.setPixel(75,Color_1);
        leds.setPixel(84,Color_1);
        leds.setPixel(91,Color_1);
        leds.setPixel(100,Color_2);
        leds.setPixel(107,Color_2);
        leds.setPixel(116,OFF);
        leds.setPixel(123,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs13=6;      
    }
    if (cs13 == 5 and n <= .12){
        leds.setPixel(68,Color_1);
        leds.setPixel(75,Color_1);
        leds.setPixel(84,Color_1);
        leds.setPixel(91,Color_1);
        leds.setPixel(100,OFF);
        leds.setPixel(107,OFF);
        leds.setPixel(116,OFF);
        leds.setPixel(123,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs13=4;    
    }
    else if (cs13 == 6 and n >= .18){
        leds.setPixel(68,Color_1);
        leds.setPixel(75,Color_1);
        leds.setPixel(84,Color_1);
        leds.setPixel(91,Color_1);
        leds.setPixel(100,Color_2);
        leds.setPixel(107,Color_2);
        leds.setPixel(116,Color_2);
        leds.setPixel(123,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs13=7;      
    }
    else if (cs13 == 6 and n <= .15){
        leds.setPixel(68,Color_1);
        leds.setPixel(75,Color_1);
        leds.setPixel(84,Color_1);
        leds.setPixel(91,Color_1);
        leds.setPixel(100,Color_2);
        leds.setPixel(107,OFF);
        leds.setPixel(116,OFF);
        leds.setPixel(123,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs13=5;     
    }
    else if (cs13 == 7 and n >= .21){
        leds.setPixel(68,Color_1);
        leds.setPixel(75,Color_1);
        leds.setPixel(84,Color_1);
        leds.setPixel(91,Color_1);
        leds.setPixel(100,Color_2);
        leds.setPixel(107,Color_2);
        leds.setPixel(116,Color_2);
        leds.setPixel(123,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs13=8;      
    }
    else if (cs13 == 7 and n <= .18){
        leds.setPixel(68,Color_1);
        leds.setPixel(75,Color_1);
        leds.setPixel(84,Color_1);
        leds.setPixel(91,Color_1);
        leds.setPixel(100,Color_2);
        leds.setPixel(107,Color_2);
        leds.setPixel(116,OFF);
        leds.setPixel(123,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs13=6;
    }
    if (cs13 == 8 and n >= .21){
        leds.setPixel(68,Color_1);
        leds.setPixel(75,Color_1);
        leds.setPixel(84,Color_1);
        leds.setPixel(91,Color_1);
        leds.setPixel(100,Color_2);
        leds.setPixel(107,Color_2);
        leds.setPixel(116,Color_2);
        leds.setPixel(123,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs13=8;     
    }
    else if (cs13 == 8 and n < .21){
        leds.setPixel(68,Color_1);
        leds.setPixel(75,Color_1);
        leds.setPixel(84,Color_1);
        leds.setPixel(91,Color_1);
        leds.setPixel(100,Color_2);
        leds.setPixel(107,Color_2);
        leds.setPixel(116,Color_2);
        leds.setPixel(123,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs13=7;       
    }    
  }
}

void Bar14(int wait, float n, int i){

  if (i == 1000){
    if (n >= 0.015 and n <= 0.035) {
        leds.setPixel(69,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs14=1;
    }
    else if (n >= 0.036 and n <= 0.06) {
        leds.setPixel(69,Color_1);
        leds.setPixel(74,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs14=2;
    }
     else if (n >= 0.061 and n <= 0.08) {
        leds.setPixel(69,Color_1);
        leds.setPixel(74,Color_1);
        leds.setPixel(85,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs14=3;
    }
      else if (n >= 0.081 and n <= 0.11) {
        leds.setPixel(69,Color_1);
        leds.setPixel(74,Color_1);
        leds.setPixel(85,Color_1);
        leds.setPixel(90,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs14=4;
    } 
    else if (n >= 0.111 and n <= 0.14) {
        leds.setPixel(69,Color_1);
        leds.setPixel(74,Color_1);
        leds.setPixel(85,Color_1);
        leds.setPixel(90,Color_1);
        leds.setPixel(101,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs14=5;
    } 
    else if (n >= 0.141 and n <= 0.17) {
        leds.setPixel(69,Color_1);
        leds.setPixel(74,Color_1);
        leds.setPixel(85,Color_1);
        leds.setPixel(90,Color_1);
        leds.setPixel(101,Color_2);
        leds.setPixel(106,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs14=6;
    } 
  
    else if (n >= 0.171 and n <= 0.210) {
        leds.setPixel(69,Color_1);
        leds.setPixel(74,Color_1);
        leds.setPixel(85,Color_1);
        leds.setPixel(90,Color_1);
        leds.setPixel(101,Color_2);
        leds.setPixel(106,Color_2);
        leds.setPixel(117,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs14=7;
    } 
    else if (n >= 0.211) {
        leds.setPixel(69,Color_1);
        leds.setPixel(74,Color_1);
        leds.setPixel(85,Color_1);
        leds.setPixel(90,Color_1);
        leds.setPixel(101,Color_2);
        leds.setPixel(106,Color_2);
        leds.setPixel(117,Color_2);
        leds.setPixel(122,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs14=8;
    } 
  }
  else if (i > 1000){
    if (cs14 == 0 and n >= .015){
        leds.setPixel(69,Color_1);
        leds.setPixel(74,OFF);
        leds.setPixel(85,OFF);
        leds.setPixel(90,OFF);
        leds.setPixel(101,OFF);
        leds.setPixel(106,OFF);
        leds.setPixel(117,OFF);
        leds.setPixel(122,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs14=1;     
    }
    if (cs14 == 0 and n < .015){
        leds.setPixel(69,OFF);
        leds.setPixel(74,OFF);
        leds.setPixel(85,OFF);
        leds.setPixel(90,OFF);
        leds.setPixel(101,OFF);
        leds.setPixel(106,OFF);
        leds.setPixel(117,OFF);
        leds.setPixel(122,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs7=0;     
    }
    if (cs14 == 1 and n >= .035){
        leds.setPixel(69,Color_1);
        leds.setPixel(74,Color_1);
        leds.setPixel(85,OFF);
        leds.setPixel(90,OFF);
        leds.setPixel(101,OFF);
        leds.setPixel(106,OFF);
        leds.setPixel(117,OFF);
        leds.setPixel(122,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs14=2;
    }
    else if (cs14 == 1 and n <= .015){
        leds.setPixel(69,OFF);
        leds.setPixel(74,OFF);
        leds.setPixel(85,OFF);
        leds.setPixel(90,OFF);
        leds.setPixel(101,OFF);
        leds.setPixel(106,OFF);
        leds.setPixel(117,OFF);
        leds.setPixel(122,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs14=0;
    }
    else if (cs14 == 2 and n >= .06){
        leds.setPixel(69,Color_1);
        leds.setPixel(74,Color_1);
        leds.setPixel(85,Color_1);
        leds.setPixel(90,OFF);
        leds.setPixel(101,OFF);
        leds.setPixel(106,OFF);
        leds.setPixel(117,OFF);
        leds.setPixel(122,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs14=3;     
    }
    else if (cs14 == 2 and n <= .035){
        leds.setPixel(69,Color_1);
        leds.setPixel(74,OFF);
        leds.setPixel(85,OFF);
        leds.setPixel(90,OFF);
        leds.setPixel(101,OFF);
        leds.setPixel(106,OFF);
        leds.setPixel(117,OFF);
        leds.setPixel(122,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs14=1;
    }
    else if (cs14 == 3 and n >= .09){
        leds.setPixel(69,Color_1);
        leds.setPixel(74,Color_1);
        leds.setPixel(85,Color_1);
        leds.setPixel(90,Color_1);
        leds.setPixel(101,OFF);
        leds.setPixel(106,OFF);
        leds.setPixel(117,OFF);
        leds.setPixel(122,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs14=4;
    }
    else if (cs14 == 3 and n <= .06){
        leds.setPixel(69,Color_1);
        leds.setPixel(74,Color_1);
        leds.setPixel(85,OFF);
        leds.setPixel(90,OFF);
        leds.setPixel(101,OFF);
        leds.setPixel(106,OFF);
        leds.setPixel(117,OFF);
        leds.setPixel(122,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs14=2;
    }
    else if (cs14 == 4 and n >= .12){
        leds.setPixel(69,Color_1);
        leds.setPixel(74,Color_1);
        leds.setPixel(85,Color_1);
        leds.setPixel(90,Color_1);
        leds.setPixel(101,Color_2);
        leds.setPixel(106,OFF);
        leds.setPixel(117,OFF);
        leds.setPixel(122,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs14=6;
    }
    else if (cs14 == 4 and n <= .09){
        leds.setPixel(69,Color_1);
        leds.setPixel(74,Color_1);
        leds.setPixel(85,Color_1);
        leds.setPixel(90,OFF);
        leds.setPixel(101,OFF);
        leds.setPixel(106,OFF);
        leds.setPixel(117,OFF);
        leds.setPixel(122,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs14=3;
    }
    else if (cs14 == 5 and n >= .15){
        leds.setPixel(69,Color_1);
        leds.setPixel(74,Color_1);
        leds.setPixel(85,Color_1);
        leds.setPixel(90,Color_1);
        leds.setPixel(101,Color_2);
        leds.setPixel(106,Color_2);
        leds.setPixel(117,OFF);
        leds.setPixel(122,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs14=6;      
    }
    if (cs14 == 5 and n <= .12){
        leds.setPixel(69,Color_1);
        leds.setPixel(74,Color_1);
        leds.setPixel(85,Color_1);
        leds.setPixel(90,Color_1);
        leds.setPixel(101,OFF);
        leds.setPixel(106,OFF);
        leds.setPixel(117,OFF);
        leds.setPixel(122,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs14=4;    
    }
    else if (cs14 == 6 and n >= .18){
        leds.setPixel(69,Color_1);
        leds.setPixel(74,Color_1);
        leds.setPixel(85,Color_1);
        leds.setPixel(90,Color_1);
        leds.setPixel(101,Color_2);
        leds.setPixel(106,Color_2);
        leds.setPixel(117,Color_2);
        leds.setPixel(122,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs14=7;      
    }
    else if (cs14 == 6 and n <= .15){
        leds.setPixel(69,Color_1);
        leds.setPixel(74,Color_1);
        leds.setPixel(85,Color_1);
        leds.setPixel(90,Color_1);
        leds.setPixel(101,Color_2);
        leds.setPixel(106,OFF);
        leds.setPixel(117,OFF);
        leds.setPixel(122,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs14=5;     
    }
    else if (cs14 == 7 and n >= .21){
        leds.setPixel(69,Color_1);
        leds.setPixel(74,Color_1);
        leds.setPixel(85,Color_1);
        leds.setPixel(90,Color_1);
        leds.setPixel(101,Color_2);
        leds.setPixel(106,Color_2);
        leds.setPixel(117,Color_2);
        leds.setPixel(122,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs14=8;      
    }
    else if (cs14 == 7 and n <= .18){
        leds.setPixel(69,Color_1);
        leds.setPixel(74,Color_1);
        leds.setPixel(85,Color_1);
        leds.setPixel(90,Color_1);
        leds.setPixel(101,Color_2);
        leds.setPixel(106,Color_2);
        leds.setPixel(117,OFF);
        leds.setPixel(122,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs14=6;
    }
    if (cs14 == 8 and n >= .21){
        leds.setPixel(69,Color_1);
        leds.setPixel(74,Color_1);
        leds.setPixel(85,Color_1);
        leds.setPixel(90,Color_1);
        leds.setPixel(101,Color_2);
        leds.setPixel(106,Color_2);
        leds.setPixel(117,Color_2);
        leds.setPixel(122,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs14=8;     
    }
    else if (cs14 == 8 and n < .21){
        leds.setPixel(69,Color_1);
        leds.setPixel(74,Color_1);
        leds.setPixel(85,Color_1);
        leds.setPixel(90,Color_1);
        leds.setPixel(101,Color_2);
        leds.setPixel(106,Color_2);
        leds.setPixel(117,Color_2);
        leds.setPixel(122,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs14=7;       
    }    
  }
}

void Bar15(int wait, float n, int i){

  if (i == 1000){
    if (n >= 0.015 and n <= 0.035) {
        leds.setPixel(70,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs15=1;
    }
    else if (n >= 0.036 and n <= 0.06) {
        leds.setPixel(70,Color_1);
        leds.setPixel(73,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs15=2;
    }
     else if (n >= 0.061 and n <= 0.08) {
        leds.setPixel(70,Color_1);
        leds.setPixel(73,Color_1);
        leds.setPixel(86,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs15=3;
    }
      else if (n >= 0.081 and n <= 0.11) {
        leds.setPixel(70,Color_1);
        leds.setPixel(73,Color_1);
        leds.setPixel(86,Color_1);
        leds.setPixel(89,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs15=4;
    } 
    else if (n >= 0.111 and n <= 0.14) {
        leds.setPixel(70,Color_1);
        leds.setPixel(73,Color_1);
        leds.setPixel(86,Color_1);
        leds.setPixel(89,Color_1);
        leds.setPixel(102,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs15=5;
    } 
    else if (n >= 0.141 and n <= 0.17) {
        leds.setPixel(70,Color_1);
        leds.setPixel(73,Color_1);
        leds.setPixel(86,Color_1);
        leds.setPixel(89,Color_1);
        leds.setPixel(102,Color_2);
        leds.setPixel(105,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs15=6;
    } 
  
    else if (n >= 0.171 and n <= 0.210) {
        leds.setPixel(70,Color_1);
        leds.setPixel(73,Color_1);
        leds.setPixel(86,Color_1);
        leds.setPixel(89,Color_1);
        leds.setPixel(102,Color_2);
        leds.setPixel(105,Color_2);
        leds.setPixel(118,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs15=7;
    } 
    else if (n >= 0.211) {
        leds.setPixel(70,Color_1);
        leds.setPixel(73,Color_1);
        leds.setPixel(86,Color_1);
        leds.setPixel(89,Color_1);
        leds.setPixel(102,Color_2);
        leds.setPixel(105,Color_2);
        leds.setPixel(118,Color_2);
        leds.setPixel(121,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs15=8;
    } 
  }
  else if (i > 1000){
    if (cs15 == 0 and n >= .015){
        leds.setPixel(70,Color_1);
        leds.setPixel(73,OFF);
        leds.setPixel(86,OFF);
        leds.setPixel(89,OFF);
        leds.setPixel(102,OFF);
        leds.setPixel(105,OFF);
        leds.setPixel(118,OFF);
        leds.setPixel(121,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs15=1;     
    }
    if (cs15 == 0 and n < .015){
        leds.setPixel(70,OFF);
        leds.setPixel(73,OFF);
        leds.setPixel(86,OFF);
        leds.setPixel(89,OFF);
        leds.setPixel(102,OFF);
        leds.setPixel(105,OFF);
        leds.setPixel(118,OFF);
        leds.setPixel(121,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs7=0;     
    }
    if (cs15 == 1 and n >= .035){
        leds.setPixel(70,Color_1);
        leds.setPixel(73,Color_1);
        leds.setPixel(86,OFF);
        leds.setPixel(89,OFF);
        leds.setPixel(102,OFF);
        leds.setPixel(105,OFF);
        leds.setPixel(118,OFF);
        leds.setPixel(121,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs15=2;
    }
    else if (cs15 == 1 and n <= .015){
        leds.setPixel(70,OFF);
        leds.setPixel(73,OFF);
        leds.setPixel(86,OFF);
        leds.setPixel(89,OFF);
        leds.setPixel(102,OFF);
        leds.setPixel(105,OFF);
        leds.setPixel(118,OFF);
        leds.setPixel(121,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs15=0;
    }
    else if (cs15 == 2 and n >= .06){
        leds.setPixel(70,Color_1);
        leds.setPixel(73,Color_1);
        leds.setPixel(86,Color_1);
        leds.setPixel(89,OFF);
        leds.setPixel(102,OFF);
        leds.setPixel(105,OFF);
        leds.setPixel(118,OFF);
        leds.setPixel(121,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs15=3;     
    }
    else if (cs15 == 2 and n <= .035){
        leds.setPixel(70,Color_1);
        leds.setPixel(73,OFF);
        leds.setPixel(86,OFF);
        leds.setPixel(89,OFF);
        leds.setPixel(102,OFF);
        leds.setPixel(105,OFF);
        leds.setPixel(118,OFF);
        leds.setPixel(121,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs15=1;
    }
    else if (cs15 == 3 and n >= .09){
        leds.setPixel(70,Color_1);
        leds.setPixel(73,Color_1);
        leds.setPixel(86,Color_1);
        leds.setPixel(89,Color_1);
        leds.setPixel(102,OFF);
        leds.setPixel(105,OFF);
        leds.setPixel(118,OFF);
        leds.setPixel(121,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs15=4;
    }
    else if (cs15 == 3 and n <= .06){
        leds.setPixel(70,Color_1);
        leds.setPixel(73,Color_1);
        leds.setPixel(86,OFF);
        leds.setPixel(89,OFF);
        leds.setPixel(102,OFF);
        leds.setPixel(105,OFF);
        leds.setPixel(118,OFF);
        leds.setPixel(121,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs15=2;
    }
    else if (cs15 == 4 and n >= .12){
        leds.setPixel(70,Color_1);
        leds.setPixel(73,Color_1);
        leds.setPixel(86,Color_1);
        leds.setPixel(89,Color_1);
        leds.setPixel(102,Color_2);
        leds.setPixel(105,OFF);
        leds.setPixel(118,OFF);
        leds.setPixel(121,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs15=6;
    }
    else if (cs15 == 4 and n <= .09){
        leds.setPixel(70,Color_1);
        leds.setPixel(73,Color_1);
        leds.setPixel(86,Color_1);
        leds.setPixel(89,OFF);
        leds.setPixel(102,OFF);
        leds.setPixel(105,OFF);
        leds.setPixel(118,OFF);
        leds.setPixel(121,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs15=3;
    }
    else if (cs15 == 5 and n >= .15){
        leds.setPixel(70,Color_1);
        leds.setPixel(73,Color_1);
        leds.setPixel(86,Color_1);
        leds.setPixel(89,Color_1);
        leds.setPixel(102,Color_2);
        leds.setPixel(105,Color_2);
        leds.setPixel(118,OFF);
        leds.setPixel(121,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs15=6;      
    }
    if (cs15 == 5 and n <= .12){
        leds.setPixel(70,Color_1);
        leds.setPixel(73,Color_1);
        leds.setPixel(86,Color_1);
        leds.setPixel(89,Color_1);
        leds.setPixel(102,OFF);
        leds.setPixel(105,OFF);
        leds.setPixel(118,OFF);
        leds.setPixel(121,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs15=4;    
    }
    else if (cs15 == 6 and n >= .18){
        leds.setPixel(70,Color_1);
        leds.setPixel(73,Color_1);
        leds.setPixel(86,Color_1);
        leds.setPixel(89,Color_1);
        leds.setPixel(102,Color_2);
        leds.setPixel(105,Color_2);
        leds.setPixel(118,Color_2);
        leds.setPixel(121,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs15=7;      
    }
    else if (cs15 == 6 and n <= .15){
        leds.setPixel(70,Color_1);
        leds.setPixel(73,Color_1);
        leds.setPixel(86,Color_1);
        leds.setPixel(89,Color_1);
        leds.setPixel(102,Color_2);
        leds.setPixel(105,OFF);
        leds.setPixel(118,OFF);
        leds.setPixel(121,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs15=5;     
    }
    else if (cs15 == 7 and n >= .21){
        leds.setPixel(70,Color_1);
        leds.setPixel(73,Color_1);
        leds.setPixel(86,Color_1);
        leds.setPixel(89,Color_1);
        leds.setPixel(102,Color_2);
        leds.setPixel(105,Color_2);
        leds.setPixel(118,Color_2);
        leds.setPixel(121,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs15=8;      
    }
    else if (cs15 == 7 and n <= .18){
        leds.setPixel(70,Color_1);
        leds.setPixel(73,Color_1);
        leds.setPixel(86,Color_1);
        leds.setPixel(89,Color_1);
        leds.setPixel(102,Color_2);
        leds.setPixel(105,Color_2);
        leds.setPixel(118,OFF);
        leds.setPixel(121,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs15=6;
    }
    if (cs15 == 8 and n >= .21){
        leds.setPixel(70,Color_1);
        leds.setPixel(73,Color_1);
        leds.setPixel(86,Color_1);
        leds.setPixel(89,Color_1);
        leds.setPixel(102,Color_2);
        leds.setPixel(105,Color_2);
        leds.setPixel(118,Color_2);
        leds.setPixel(121,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs15=8;     
    }
    else if (cs15 == 8 and n < .21){
        leds.setPixel(70,Color_1);
        leds.setPixel(73,Color_1);
        leds.setPixel(86,Color_1);
        leds.setPixel(89,Color_1);
        leds.setPixel(102,Color_2);
        leds.setPixel(105,Color_2);
        leds.setPixel(118,Color_2);
        leds.setPixel(121,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs15=7;       
    }    
  }
}

void Bar16(int wait, float n, int i){

  if (i == 1000){
    if (n >= 0.015 and n <= 0.035) {
        leds.setPixel(71,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs16=1;
    }
    else if (n >= 0.036 and n <= 0.06) {
        leds.setPixel(71,Color_1);
        leds.setPixel(72,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs16=2;
    }
     else if (n >= 0.061 and n <= 0.08) {
        leds.setPixel(71,Color_1);
        leds.setPixel(72,Color_1);
        leds.setPixel(87,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs16=3;
    }
      else if (n >= 0.081 and n <= 0.11) {
        leds.setPixel(71,Color_1);
        leds.setPixel(72,Color_1);
        leds.setPixel(87,Color_1);
        leds.setPixel(88,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs16=4;
    } 
    else if (n >= 0.111 and n <= 0.14) {
        leds.setPixel(71,Color_1);
        leds.setPixel(72,Color_1);
        leds.setPixel(87,Color_1);
        leds.setPixel(88,Color_1);
        leds.setPixel(103,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs16=5;
    } 
    else if (n >= 0.141 and n <= 0.17) {
        leds.setPixel(71,Color_1);
        leds.setPixel(72,Color_1);
        leds.setPixel(87,Color_1);
        leds.setPixel(88,Color_1);
        leds.setPixel(103,Color_2);
        leds.setPixel(104,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs16=6;
    } 
  
    else if (n >= 0.171 and n <= 0.210) {
        leds.setPixel(71,Color_1);
        leds.setPixel(72,Color_1);
        leds.setPixel(87,Color_1);
        leds.setPixel(88,Color_1);
        leds.setPixel(103,Color_2);
        leds.setPixel(104,Color_2);
        leds.setPixel(119,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs16=7;
    } 
    else if (n >= 0.211) {
        leds.setPixel(71,Color_1);
        leds.setPixel(72,Color_1);
        leds.setPixel(87,Color_1);
        leds.setPixel(88,Color_1);
        leds.setPixel(103,Color_2);
        leds.setPixel(104,Color_2);
        leds.setPixel(119,Color_2);
        leds.setPixel(120,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs16=8;
    } 
  }
  else if (i > 1000){
    if (cs16 == 0 and n >= .015){
        leds.setPixel(71,Color_1);
        leds.setPixel(72,OFF);
        leds.setPixel(87,OFF);
        leds.setPixel(88,OFF);
        leds.setPixel(103,OFF);
        leds.setPixel(104,OFF);
        leds.setPixel(119,OFF);
        leds.setPixel(120,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs16=1;     
    }
    if (cs16 == 0 and n < .015){
        leds.setPixel(71,OFF);
        leds.setPixel(72,OFF);
        leds.setPixel(87,OFF);
        leds.setPixel(88,OFF);
        leds.setPixel(103,OFF);
        leds.setPixel(104,OFF);
        leds.setPixel(119,OFF);
        leds.setPixel(120,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs7=0;     
    }
    if (cs16 == 1 and n >= .035){
        leds.setPixel(71,Color_1);
        leds.setPixel(72,Color_1);
        leds.setPixel(87,OFF);
        leds.setPixel(88,OFF);
        leds.setPixel(103,OFF);
        leds.setPixel(104,OFF);
        leds.setPixel(119,OFF);
        leds.setPixel(120,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs16=2;
    }
    else if (cs16 == 1 and n <= .015){
        leds.setPixel(71,OFF);
        leds.setPixel(72,OFF);
        leds.setPixel(87,OFF);
        leds.setPixel(88,OFF);
        leds.setPixel(103,OFF);
        leds.setPixel(104,OFF);
        leds.setPixel(119,OFF);
        leds.setPixel(120,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs16=0;
    }
    else if (cs16 == 2 and n >= .06){
        leds.setPixel(71,Color_1);
        leds.setPixel(72,Color_1);
        leds.setPixel(87,Color_1);
        leds.setPixel(88,OFF);
        leds.setPixel(103,OFF);
        leds.setPixel(104,OFF);
        leds.setPixel(119,OFF);
        leds.setPixel(120,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs16=3;     
    }
    else if (cs16 == 2 and n <= .035){
        leds.setPixel(71,Color_1);
        leds.setPixel(72,OFF);
        leds.setPixel(87,OFF);
        leds.setPixel(88,OFF);
        leds.setPixel(103,OFF);
        leds.setPixel(104,OFF);
        leds.setPixel(119,OFF);
        leds.setPixel(120,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs16=1;
    }
    else if (cs16 == 3 and n >= .09){
        leds.setPixel(71,Color_1);
        leds.setPixel(72,Color_1);
        leds.setPixel(87,Color_1);
        leds.setPixel(88,Color_1);
        leds.setPixel(103,OFF);
        leds.setPixel(104,OFF);
        leds.setPixel(119,OFF);
        leds.setPixel(120,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs16=4;
    }
    else if (cs16 == 3 and n <= .06){
        leds.setPixel(71,Color_1);
        leds.setPixel(72,Color_1);
        leds.setPixel(87,OFF);
        leds.setPixel(88,OFF);
        leds.setPixel(103,OFF);
        leds.setPixel(104,OFF);
        leds.setPixel(119,OFF);
        leds.setPixel(120,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs16=2;
    }
    else if (cs16 == 4 and n >= .12){
        leds.setPixel(71,Color_1);
        leds.setPixel(72,Color_1);
        leds.setPixel(87,Color_1);
        leds.setPixel(88,Color_1);
        leds.setPixel(103,Color_2);
        leds.setPixel(104,OFF);
        leds.setPixel(119,OFF);
        leds.setPixel(120,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs16=6;
    }
    else if (cs16 == 4 and n <= .09){
        leds.setPixel(71,Color_1);
        leds.setPixel(72,Color_1);
        leds.setPixel(87,Color_1);
        leds.setPixel(88,OFF);
        leds.setPixel(103,OFF);
        leds.setPixel(104,OFF);
        leds.setPixel(119,OFF);
        leds.setPixel(120,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs16=3;
    }
    else if (cs16 == 5 and n >= .15){
        leds.setPixel(71,Color_1);
        leds.setPixel(72,Color_1);
        leds.setPixel(87,Color_1);
        leds.setPixel(88,Color_1);
        leds.setPixel(103,Color_2);
        leds.setPixel(104,Color_2);
        leds.setPixel(119,OFF);
        leds.setPixel(120,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs16=6;      
    }
    if (cs16 == 5 and n <= .12){
        leds.setPixel(71,Color_1);
        leds.setPixel(72,Color_1);
        leds.setPixel(87,Color_1);
        leds.setPixel(88,Color_1);
        leds.setPixel(103,OFF);
        leds.setPixel(104,OFF);
        leds.setPixel(119,OFF);
        leds.setPixel(120,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs16=4;    
    }
    else if (cs16 == 6 and n >= .18){
        leds.setPixel(71,Color_1);
        leds.setPixel(72,Color_1);
        leds.setPixel(87,Color_1);
        leds.setPixel(88,Color_1);
        leds.setPixel(103,Color_2);
        leds.setPixel(104,Color_2);
        leds.setPixel(119,Color_2);
        leds.setPixel(120,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs16=7;      
    }
    else if (cs16 == 6 and n <= .15){
        leds.setPixel(71,Color_1);
        leds.setPixel(72,Color_1);
        leds.setPixel(87,Color_1);
        leds.setPixel(88,Color_1);
        leds.setPixel(103,Color_2);
        leds.setPixel(104,OFF);
        leds.setPixel(119,OFF);
        leds.setPixel(120,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs16=5;     
    }
    else if (cs16 == 7 and n >= .21){
        leds.setPixel(71,Color_1);
        leds.setPixel(72,Color_1);
        leds.setPixel(87,Color_1);
        leds.setPixel(88,Color_1);
        leds.setPixel(103,Color_2);
        leds.setPixel(104,Color_2);
        leds.setPixel(119,Color_2);
        leds.setPixel(120,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs16=8;      
    }
    else if (cs16 == 7 and n <= .18){
        leds.setPixel(71,Color_1);
        leds.setPixel(72,Color_1);
        leds.setPixel(87,Color_1);
        leds.setPixel(88,Color_1);
        leds.setPixel(103,Color_2);
        leds.setPixel(104,Color_2);
        leds.setPixel(119,OFF);
        leds.setPixel(120,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs16=6;
    }
    if (cs16 == 8 and n >= .21){
        leds.setPixel(71,Color_1);
        leds.setPixel(72,Color_1);
        leds.setPixel(87,Color_1);
        leds.setPixel(88,Color_1);
        leds.setPixel(103,Color_2);
        leds.setPixel(104,Color_2);
        leds.setPixel(119,Color_2);
        leds.setPixel(120,Color_3);
        leds.show();
        delayMicroseconds(wait);
        cs16=8;     
    }
    else if (cs16 == 8 and n < .21){
        leds.setPixel(71,Color_1);
        leds.setPixel(72,Color_1);
        leds.setPixel(87,Color_1);
        leds.setPixel(88,Color_1);
        leds.setPixel(103,Color_2);
        leds.setPixel(104,Color_2);
        leds.setPixel(119,Color_2);
        leds.setPixel(120,OFF);
        leds.show();
        delayMicroseconds(wait);
        cs16=7;       
    }    
  }
}

void printNumber(float n) {
  
  if (n >= 0.004) {
    Serial.print(n, 3);
    Serial.print(" ");
  } else {
    Serial.print("   -  "); // don't print "0.00"
  }

}
