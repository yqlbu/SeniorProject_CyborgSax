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

#define ILI9341_ORANGE 0xFD20 /* 255, 165, 0 */
#define ILI9341_CYAN 0x07FF /* 0, 255, 255 */
#define ILI9341_PINK 0xF81F

#define E1_outputA 37//29
#define E1_outputB 38//30
#define E1_SW 39//25

#define E2_outputA 29
#define E2_outputB 30 //7
#define E2_SW 25

#define TFT_DC 20              
#define TFT_CS 21             
#define TFT_RST 255             
#define TFT_MISO 12           
#define TFT_MOSI 7           
#define TFT_SCK 14
  
#define PIN_SWITCH_1  31
#define PIN_SWITCH_2  28
#define PIN_SWITCH_3  27



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

#define CHORUS_DELAY_LENGTH (16*AUDIO_BLOCK_SAMPLES)
short delayline[CHORUS_DELAY_LENGTH];
int n_chorus = 3;


const int numled = 128;
const int pin = 26;

int buttonPress_1 = 0;
int buttonPress_2 = 0;
int buttonPress_3 = 0;

String button_state, last_button_state;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST, TFT_MISO);

// set the following parameters for overall coordinates of the GUI layout
int global_x = 40; // set default, identical to the F1 rectangle x=40
int global_y = 55;  // set default, identical to the F1 rectangle y=40
int mode;
int f1=0;
int f2=0;

// defining functions here
int Color_scheme, Gain, Lighting_mode, Master_volume, FX, Dry_wet;

// defining parameters here
int FX_val = 0;
int Color_scheme_val = 0;
int Gain_val = 1;
int Lighting_mode_val = 0;
int Master_volume_val = 1;
int Dry_wet_val = 0;

//  encoder A
float counter_A=1;
int buttonPushCounter_1 = 1;   // E1 counter for the number of button presses
int A_buttonState = 0;         // current state of the button
int A_lastButtonState = 0;     // previous state of the button
int A_aState;
int A_aLastState;  

float pre_counter_A,pre_counter_B;

//  encoder B
float counter_B=1;
int buttonPushCounter_2 = 1;   // E2 counter for the number of button presses
int B_buttonState = 0;         // current state of the button
int B_lastButtonState = 0;     // previous state of the button
int B_aState;
int B_aLastState;  

Bounce bouncer_1 = Bounce (E1_outputA , .01); //Initiate a Bounce object with a 2 milisecond debounce time
Bounce bouncer_2 = Bounce (E2_outputA , .01); //Initiate a Bounce object with a 2 milisecond debounce time

bool fft = false;

int Color_1, Color_2, Color_3;

byte drawingMemory[numled*3];         //  3 bytes per LED
DMAMEM byte displayMemory[numled*12]; // 12 bytes per LED

WS2812Serial leds(numled, displayMemory, drawingMemory, pin, WS2812_GRB);

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            Mic;           //xy=107,162
AudioPlaySdWav           playSdWav1;     //xy=137,210
AudioMixer4              mixer1;         //xy=314,178
AudioAmplifier           amp1;           //xy=457,315
AudioEffectChorus        chorus1;        //xy=484,137
AudioEffectReverb        reverb1;        //xy=489,78
AudioEffectWaveshaper    waveshape1;     //xy=495,172
AudioMixer4              mixer2;         //xy=629,185
AudioAnalyzeFFT256       fft256_1;       //xy=632,314
AudioAmplifier           amp2;           //xy=759,185
AudioFilterStateVariable filter1;        //xy=907.9999847412109,191.28570461273193
AudioOutputI2S           i2s1;           //xy=1069.999984741211,204.99999618530273
AudioConnection          patchCord1(Mic, 0, mixer1, 0);
AudioConnection          patchCord2(Mic, 1, mixer1, 1);
AudioConnection          patchCord3(playSdWav1, 0, mixer1, 2);
AudioConnection          patchCord4(playSdWav1, 1, mixer1, 3);
AudioConnection          patchCord5(mixer1, amp1);
AudioConnection          patchCord6(mixer1, reverb1);
AudioConnection          patchCord7(mixer1, 0, mixer2, 3);
AudioConnection          patchCord8(mixer1, chorus1);
AudioConnection          patchCord9(mixer1, waveshape1);
AudioConnection          patchCord10(amp1, fft256_1);
AudioConnection          patchCord11(chorus1, 0, mixer2, 1);
AudioConnection          patchCord12(reverb1, 0, mixer2, 0);
AudioConnection          patchCord13(waveshape1, 0, mixer2, 2);
AudioConnection          patchCord14(mixer2, amp2);
AudioConnection          patchCord15(amp2, 0, filter1, 0);
AudioConnection          patchCord16(filter1, 0, i2s1, 0);
AudioConnection          patchCord17(filter1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=399,357
// GUItool: end automatically generated code



// Use these with the Teensy 3.5 & 3.6 SD card
#define SDCARD_CS_PIN    BUILTIN_SDCARD
#define SDCARD_MOSI_PIN  11  // not actually used
#define SDCARD_SCK_PIN   13  // not actually used

void setup() {
  // put your setup code here, to run once:
    Serial.println("ILI9341 Test!"); 
    tft.begin();
    tft.setRotation(3); //adjust the tft screen for displaying in the right way
    tft.fillScreen(ILI9341_BLACK);

    pinMode (E1_outputA,INPUT_PULLUP);
    pinMode (E1_outputB,INPUT_PULLUP);
    // initialize the button pin as a input:
    pinMode (E1_SW,INPUT_PULLUP);
    
    pinMode (E2_outputA,INPUT_PULLUP);
    pinMode (E2_outputB,INPUT_PULLUP);
    // initialize the button pin as a input:
    pinMode (E2_SW,INPUT_PULLUP);
    
    pinMode(PIN_SWITCH_1, INPUT_PULLUP); 
    pinMode(PIN_SWITCH_2, INPUT_PULLUP); 
    pinMode(PIN_SWITCH_3, INPUT_PULLUP); 

    attachInterrupt(digitalPinToInterrupt(PIN_SWITCH_1),mode_1,CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_SWITCH_2),mode_3,CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_SWITCH_3),mode_2,CHANGE);
    
    welcoming();
    
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
    filter1.frequency(10000);
    filter1.resonance(.7);
    filter1.octaveControl(2);
    mixer1.gain(0, 0);
    mixer1.gain(1, 0);
    mixer1.gain(2, 0.4);
    mixer1.gain(3, 0.4);
   
    delay(1000);
    playSdWav1.play("BSB.WAV");
    
    Serial.begin(9600);

}

float n, m, w, a, b, c, d, e, f, g, h, i, j, k, l, o, p, q, r, s, t, u, v, x, y, mm, nn, ww, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az, ba, bb, bc, bd, be, bf;

int count = 0;
int iteration = 1;
int fft_count;

int cs1, cs2, cs3, cs4, cs5, cs6, cs7, cs8, cs9, cs10, cs11, cs12, cs13, cs14, cs15, cs16;
int param1, param2;

int analogPin = 15;

void loop() {
  int lightLevel = analogRead(analogPin);
  // Convert the raw data value (0 - 1023) to voltage (0.0V - 5.0V):
  float voltage = lightLevel * (10 / 1024.0);
  // write the voltage value to the serial monitor:
  
  // put your main code here, to run repeatedly:
    encoder_1();
    encoder_2(); 
    if (f1 == 0 && f2 ==0)
      fft = true;
    else if (f1 == 1 || f2 ==1) 
      fft = false;       
    if (fft == true)
      runFFT();
    lightingOptions(Color_scheme_val, Gain_val);
    soundOptions(FX_val, Dry_wet_val);
    generalOptions(Lighting_mode_val, Master_volume_val);
    //amp2.gain(voltage/Master_volume_val);


   //Serial.println(analogRead(A1));
}
void soundOptions(int param1, int param2) {
  if (param1 == 0){
    mixer2.gain(0, 0);
    mixer2.gain(1, 0);
    mixer2.gain(2, 0);
    mixer2.gain(3, 1);
  }
  else if (param1 == 1){

    mixer2.gain(0, 2*(.01*param2));
    mixer2.gain(1, 0);
    mixer2.gain(2, 0);
    mixer2.gain(3, 1-.01*param2);
    reverb1.reverbTime(1);
  }
  else if (param1 == 2){
    mixer2.gain(0, 0);
    mixer2.gain(1, .1*param2);
    mixer2.gain(2, 0);
    mixer2.gain(3, 1-.1*param2);
    //chorus1.begin(delayline, CHORUS_DELAY_LENGTH, n_chorus);
  }
  else if (param1 == 3){
    mixer2.gain(0, 0);
    mixer2.gain(1, 0);
    mixer2.gain(2, .1*param2);
    mixer2.gain(3, 1-.1*param2);
    //waveshape1.begin();
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

  amp1.gain(param2*5);
}

void generalOptions(int param1, int param2) {
//  if (param1 == 0){
//    mixer1.gain(0, 1);
//    mixer1.gain(1, 1);
//  }
//  else{
//    mixer1.gain(0, 1);
//    mixer1.gain(1, -1);
//  }
  if (param1 == 1){
    fft = false;
    leds.clear();
  }
  else 
    fft = true;
  amp2.gain(param2*2);
}

void offMode(int param1) {
 
}

void welcoming(){
    int title_x = tft.height()/3;
    int title_y = tft.width()/4;
    
    tft.setCursor(title_x, title_y); //coordination
    tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
    tft.println("Welcome to the");
    tft.setCursor(title_x-35, title_y+50); //coordination
    tft.setTextColor(ILI9341_RED);  tft.setTextSize(4);
    tft.println("CyborgSax!");
    delay(4000);
    tft.fillScreen(ILI9341_BLACK);

    default_page(ILI9341_WHITE);
}

unsigned long default_page(uint16_t color) {
  
    unsigned long start;
    start = micros();

    // initialize the coordinates
    int title_x = 110;
    int title_y = 15;
    
    // mode
    int c_x = global_x + 130; // default x = 40 + 120 = 180
    int c_y = global_y + 160; // default y = 55 + 150 = 205
    // text inside the mode circle
    int c_t_x = c_x - 4; // default x = 60 - 4 = 56
    int c_t_y = c_y - 7; // default y = 205 - 7 = 198
    int r_m = 16;
    
    // function
    int c_f_x = global_x + 12; // default x = 40 + 120 = 180
    int c_f_y = global_y + 30; // default y = 55 + 150 = 205
    int c_f_x_t = c_f_x + 60;
    int c_f_y_t = c_f_y - 6;
    int r_f = 21;

    int f1_text_x = global_x + 72;
    int f1_text_y = global_y + 9;
    int f2_text_x = f1_text_x;
    int f2_text_y = f1_text_y + 75;

    int f1_paramater_x = f1_text_x;
    int f1_paramater_y = global_y + 35;
    int f2_paramater_x = f2_text_x;
    int f2_paramater_y = f1_paramater_y + 75;
    
    // drawing the modes
    circle(c_x,c_y,r_m,ILI9341_RED);
    circle(c_x + 40,c_y,r_m,ILI9341_WHITE);
    circle(c_x + 80,c_y,r_m,ILI9341_WHITE);
    circle(c_x + 120,c_y,r_m,ILI9341_WHITE);

    // title text
    tft.setCursor(title_x, title_y); //coordination
    tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
    tft.println("User Menu");

    // text on the mode circle
    tft.setCursor(global_x - 10, c_t_y+5); //coordination
    tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
    tft.println("Mode: ");
    
    tft.setCursor(c_t_x, c_t_y); //coordination
    tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
    tft.println('S'); // mode_1
    tft.setCursor(c_t_x + 40, c_t_y); //coordination
    tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
    tft.println('L'); // mode_2
    tft.setCursor(c_t_x + 80, c_t_y); //coordination
    tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
    tft.println('P'); // mode_3
    tft.setCursor(c_t_x + 120, c_t_y); //coordination
    tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
    tft.println('G'); // mode_4

    // text on the functions
    // F1
    circle(c_f_x,c_f_y,r_f+5,ILI9341_WHITE);
    circle(c_f_x,c_f_y,r_f,ILI9341_WHITE);
    tft.setCursor(c_f_x-9, c_f_y_t); //coordination
    tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
    tft.println("F1"); 
    
    // F2
    circle(c_f_x,c_f_y+75,r_f+5,ILI9341_WHITE);
    circle(c_f_x,c_f_y+75,r_f,ILI9341_WHITE);
    tft.setCursor(c_f_x-10, c_f_y+69); //coordination
    tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
    tft.println("F2"); 

    tft.setCursor(c_f_x_t, c_f_y_t - 15); //coordination
    tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
    tft.println("FX Function"); // default text

    tft.setCursor(f1_paramater_x, f1_paramater_y); //coordination
    tft.setTextColor(ILI9341_ORANGE);  tft.setTextSize(2);
    tft.print("F"); // default text
    tft.print(FX_val); // default text

    tft.setCursor(f2_text_x, f2_text_y); //coordination
    tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
    tft.println("Dry Wet"); // default text

    tft.setCursor(f2_paramater_x, f2_paramater_y); //coordination
    tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
    tft.print(Dry_wet_val); // default text
    tft.print("%"); // default text
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

void chosen_func()
{ 
    // function
    int c_f_x = global_x + 12; // default x = 40 + 120 = 180
    int c_f_y = global_y + 30; // default y = 55 + 150 = 205
    int c_f_x_t = c_f_x + 60;
    int c_f_y_t = c_f_y - 6;
    int r_f = 21;
        
    int f1_text_x = global_x + 72;
    int f1_text_y = global_y + 9;
    int f1_paramater_x = f1_text_x;
    int f1_paramater_y = global_y + 35;
    
    int f2_text_x = f1_text_x;
    int f2_text_y = f1_text_y + 75;
    int f2_paramater_x = f2_text_x;
    int f2_paramater_y = f1_paramater_y + 75;
    
    unsigned long start = micros();
    start = micros();
    
    // **************** mode_1 **************** //
    
    if (mode == 1 && f1 == 1 && f2 == 0){
      if (buttonPushCounter_1 == 1){
        //overwrite the function circle
        circle(c_f_x,c_f_y,r_f+5,ILI9341_WHITE);
        circle(c_f_x,c_f_y,r_f,ILI9341_RED);
        tft.setCursor(c_f_x-9, c_f_y_t); //coordination
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
        tft.println("F1"); 
        circle(c_f_x,c_f_y+75,r_f+5,ILI9341_WHITE);
        circle(c_f_x,c_f_y+75,r_f,ILI9341_WHITE);
        tft.setCursor(c_f_x-10, c_f_y+69); //coordination
        tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
        tft.println("F2"); 
        //overwrite the text
        tft.fillRect(c_f_x_t, c_f_y_t - 17, 180, 20, ILI9341_BLACK);
        tft.setCursor(c_f_x_t, c_f_y_t - 15); //coordination
        tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
        tft.println("FX Function"); // default text
        f1=1;
        f2=0;
      }
      else{
        //overwrite the function circle
        circle(c_f_x,c_f_y,r_f+5,ILI9341_WHITE);
        circle(c_f_x,c_f_y,r_f,ILI9341_WHITE);
        tft.setCursor(c_f_x-9, c_f_y_t); //coordination
        tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
        tft.println("F1"); 
        circle(c_f_x,c_f_y+75,r_f+5,ILI9341_WHITE);
        circle(c_f_x,c_f_y+75,r_f,ILI9341_WHITE);
        tft.setCursor(c_f_x-10, c_f_y+69); //coordination
        tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
        tft.println("F2"); 
        //overwrite the text
        tft.fillRect(c_f_x_t, c_f_y_t - 17, 180, 20, ILI9341_BLACK);
        tft.setCursor(c_f_x_t, c_f_y_t - 15); //coordination
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
        tft.println("FX Function"); // default text
        f1=0;
        f2=0;
      }
    }
    else if (mode == 1 && f1 == 0 && f2 == 1){
      if (buttonPushCounter_2 == 1){
        //overwrite the function circle
        circle(c_f_x,c_f_y,r_f+5,ILI9341_WHITE);
        circle(c_f_x,c_f_y,r_f,ILI9341_WHITE);
        tft.setCursor(c_f_x-9, c_f_y_t); //coordination
        tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
        tft.println("F1"); 
        circle(c_f_x,c_f_y+75,r_f+5,ILI9341_WHITE);
        circle(c_f_x,c_f_y+75,r_f,ILI9341_RED);
        tft.setCursor(c_f_x-10, c_f_y+69); //coordination
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
        tft.println("F2"); 
        //overwrite the text
        tft.fillRect(f2_text_x, f2_text_y - 2, 180, 20, ILI9341_BLACK);
        tft.setCursor(f2_text_x, f2_text_y); //coordination
        tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
        tft.println("Dry Wet"); // default text
        f1=0;
        f2=1;
      }
      else{
        //overwrite the function circle
        circle(c_f_x,c_f_y,r_f+5,ILI9341_WHITE);
        circle(c_f_x,c_f_y,r_f,ILI9341_WHITE);
        tft.setCursor(c_f_x-9, c_f_y_t); //coordination
        tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
        tft.println("F1"); 
        circle(c_f_x,c_f_y+75,r_f+5,ILI9341_WHITE);
        circle(c_f_x,c_f_y+75,r_f,ILI9341_WHITE);
        tft.setCursor(c_f_x-10, c_f_y+69); //coordination
        tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
        tft.println("F2"); 
        //overwrite the text
        tft.fillRect(f2_text_x, f2_text_y - 2, 180, 20, ILI9341_BLACK);
        tft.setCursor(f2_text_x, f2_text_y); //coordination
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
        tft.println("Dry Wet"); // default text
        f1=0;
        f2=0;
      }
      
    }
    
    // **************** mode_2 **************** //
    else if (mode == 2 && f1 == 1 && f2 == 0){
      if (buttonPushCounter_1 == 1){
        //overwrite the function circle
        circle(c_f_x,c_f_y,r_f+5,ILI9341_WHITE);
        circle(c_f_x,c_f_y,r_f,ILI9341_RED);
        tft.setCursor(c_f_x-9, c_f_y_t); //coordination
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
        tft.println("F1"); 
        circle(c_f_x,c_f_y+75,r_f+5,ILI9341_WHITE);
        circle(c_f_x,c_f_y+75,r_f,ILI9341_WHITE);
        tft.setCursor(c_f_x-10, c_f_y+69); //coordination
        tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
        tft.println("F2"); 
        //overwrite the text
        tft.fillRect(c_f_x_t, c_f_y_t - 17, 180, 20, ILI9341_BLACK);
        tft.setCursor(c_f_x_t, c_f_y_t - 15); //coordination
        tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
        tft.println("Color Scheme"); // default text
        f1=1;
        f2=0;
      }
      else{
        //overwrite the function circle
        circle(c_f_x,c_f_y,r_f+5,ILI9341_WHITE);
        circle(c_f_x,c_f_y,r_f,ILI9341_WHITE);
        tft.setCursor(c_f_x-9, c_f_y_t); //coordination
        tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
        tft.println("F1"); 
        circle(c_f_x,c_f_y+75,r_f+5,ILI9341_WHITE);
        circle(c_f_x,c_f_y+75,r_f,ILI9341_WHITE);
        tft.setCursor(c_f_x-10, c_f_y+69); //coordination
        tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
        tft.println("F2"); 
        //overwrite the text
        tft.fillRect(c_f_x_t, c_f_y_t - 17, 180, 20, ILI9341_BLACK);
        tft.setCursor(c_f_x_t, c_f_y_t - 15); //coordination
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
        tft.println("Color Scheme"); // default text
        f1=0;
        f2=0;
      }
    }
    else if (mode == 2 && f1 == 0 && f2 == 1){
      if (buttonPushCounter_2 == 1){
        //overwrite the function circle
        circle(c_f_x,c_f_y,r_f+5,ILI9341_WHITE);
        circle(c_f_x,c_f_y,r_f,ILI9341_WHITE);
        tft.setCursor(c_f_x-9, c_f_y_t); //coordination
        tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
        tft.println("F1"); 
        circle(c_f_x,c_f_y+75,r_f+5,ILI9341_WHITE);
        circle(c_f_x,c_f_y+75,r_f,ILI9341_RED);
        tft.setCursor(c_f_x-10, c_f_y+69); //coordination
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
        tft.println("F2"); 
        //overwrite the text
        tft.fillRect(f2_text_x, f2_text_y - 2, 180, 20, ILI9341_BLACK);
        tft.setCursor(f2_text_x, f2_text_y); //coordination
        tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
        tft.println("Gain"); // default text
        f1=0;
        f2=1;
      }
      else{
        //overwrite the function circle
        circle(c_f_x,c_f_y,r_f+5,ILI9341_WHITE);
        circle(c_f_x,c_f_y,r_f,ILI9341_WHITE);
        tft.setCursor(c_f_x-9, c_f_y_t); //coordination
        tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
        tft.println("F1"); 
        circle(c_f_x,c_f_y+75,r_f+5,ILI9341_WHITE);
        circle(c_f_x,c_f_y+75,r_f,ILI9341_WHITE);
        tft.setCursor(c_f_x-10, c_f_y+69); //coordination
        tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
        tft.println("F2"); 
        //overwrite the text
        tft.fillRect(f2_text_x, f2_text_y - 2, 180, 20, ILI9341_BLACK);
        tft.setCursor(f2_text_x, f2_text_y); //coordination
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
        tft.println("Gain"); // default text
        f1=0;
        f2=0;
      }
    }
    
    // **************** mode_4 **************** //
    else if (mode == 4 && f1 == 1 && f2 == 0){
      if (buttonPushCounter_1 == 1){
        //overwrite the function circle
        circle(c_f_x,c_f_y,r_f+5,ILI9341_WHITE);
        circle(c_f_x,c_f_y,r_f,ILI9341_RED);
        tft.setCursor(c_f_x-9, c_f_y_t); //coordination
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
        tft.println("F1"); 
        circle(c_f_x,c_f_y+75,r_f+5,ILI9341_WHITE);
        circle(c_f_x,c_f_y+75,r_f,ILI9341_WHITE);
        tft.setCursor(c_f_x-10, c_f_y+69); //coordination
        tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
        tft.println("F2"); 
        //overwrite the text
        tft.fillRect(c_f_x_t, c_f_y_t - 17, 180, 20, ILI9341_BLACK);
        tft.setCursor(c_f_x_t, c_f_y_t - 15); //coordination
        tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
        tft.println("Lighting Mode"); // default text
        f1=1;
        f2=0;
      }
      else{
        //overwrite the function circle
        circle(c_f_x,c_f_y,r_f+5,ILI9341_WHITE);
        circle(c_f_x,c_f_y,r_f,ILI9341_WHITE);
        tft.setCursor(c_f_x-9, c_f_y_t); //coordination
        tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
        tft.println("F1"); 
        circle(c_f_x,c_f_y+75,r_f+5,ILI9341_WHITE);
        circle(c_f_x,c_f_y+75,r_f,ILI9341_WHITE);
        tft.setCursor(c_f_x-10, c_f_y+69); //coordination
        tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
        tft.println("F2"); 
        //overwrite the text
        tft.fillRect(c_f_x_t, c_f_y_t - 17, 180, 20, ILI9341_BLACK);
        tft.setCursor(c_f_x_t, c_f_y_t - 15); //coordination
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
        tft.println("Lighting Mode"); // default text
        f1=0;
        f2=0;
      }
    }
    else if (mode == 4 && f1 == 0 && f2 == 1){
      if (buttonPushCounter_2 == 1){
        //overwrite the function circle
        circle(c_f_x,c_f_y,r_f+5,ILI9341_WHITE);
        circle(c_f_x,c_f_y,r_f,ILI9341_WHITE);
        tft.setCursor(c_f_x-9, c_f_y_t); //coordination
        tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
        tft.println("F1"); 
        circle(c_f_x,c_f_y+75,r_f+5,ILI9341_WHITE);
        circle(c_f_x,c_f_y+75,r_f,ILI9341_RED);
        tft.setCursor(c_f_x-10, c_f_y+69); //coordination
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
        tft.println("F2"); 
        //overwrite the text
        tft.fillRect(f2_text_x, f2_text_y - 2, 180, 20, ILI9341_BLACK);
        tft.setCursor(f2_text_x, f2_text_y); //coordination
        tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
        tft.println("Master Volume"); // default text
        f1=0;
        f2=1;
      }
      else{
        //overwrite the function circle
        circle(c_f_x,c_f_y,r_f+5,ILI9341_WHITE);
        circle(c_f_x,c_f_y,r_f,ILI9341_WHITE);
        tft.setCursor(c_f_x-9, c_f_y_t); //coordination
        tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
        tft.println("F1"); 
        circle(c_f_x,c_f_y+75,r_f+5,ILI9341_WHITE);
        circle(c_f_x,c_f_y+75,r_f,ILI9341_WHITE);
        tft.setCursor(c_f_x-10, c_f_y+69); //coordination
        tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
        tft.println("F2"); 
        //overwrite the text
        tft.fillRect(f2_text_x, f2_text_y - 2, 180, 20, ILI9341_BLACK);
        tft.setCursor(f2_text_x, f2_text_y); //coordination
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
        tft.println("Master Volume"); // default text
        f1=0;
        f2=0;
      }
    }
}


// --------------------------- switching mode ---------------------------------- //

void mode_1()
{   
    sgtl5000_1.muteLineout();
    sgtl5000_1.muteHeadphone();
    // mode
    int c_x = global_x + 130; // default x = 40 + 120 = 180
    int c_y = global_y + 160; // default y = 55 + 150 = 205
    // text inside the mode circle
    int c_t_x = c_x - 4; // default x = 60 - 4 = 56
    int c_t_y = c_y - 7; // default y = 205 - 7 = 198
    int r_m = 16;

    // function
    int c_f_x = global_x + 12; // default x = 40 + 120 = 180
    int c_f_y = global_y + 30; // default y = 55 + 150 = 205
    int c_f_x_t = c_f_x + 60;
    int c_f_y_t = c_f_y - 6;
    int r_f = 21;

    int f1_text_x = global_x + 72;
    int f1_text_y = global_y + 9;
    int f1_paramater_x = f1_text_x;
    int f1_paramater_y = global_y + 35;
    
    int f2_text_x = f1_text_x;
    int f2_text_y = f1_text_y + 75;
    int f2_paramater_x = f2_text_x;
    int f2_paramater_y = f1_paramater_y + 75;

//    int b1_x=global_x + 12, b1_y=global_y + 30, w=50, h=10; // default x = 175 = 40 + 135 ; y = 190 = 40 + 150
//    int text1_x = b1_x + 60, text_y = b1_y + 5;
//    
//    int overwrite_x = global_x + 130; // default x = 170 = 40 + 130
//    int overwrite_y = global_y + 65;// default y = 105 = 40 + 65

    buttonPress_1 = digitalRead(PIN_SWITCH_1);
    buttonPress_2 = digitalRead(PIN_SWITCH_2);
    buttonPress_3 = digitalRead(PIN_SWITCH_3);
    
    button_state = String(String(buttonPress_1) + String(buttonPress_2) + String(buttonPress_3));

    if (button_state != last_button_state){
        // 1(011)
        if(digitalRead(PIN_SWITCH_1) == LOW && digitalRead(PIN_SWITCH_2) == HIGH && digitalRead(PIN_SWITCH_3) == HIGH){
            // overwrite the mode circle and text
            circle(c_x,c_y,r_m,ILI9341_RED);
            circle(c_x + 40,c_y,r_m,ILI9341_WHITE);
            circle(c_x + 80,c_y,r_m,ILI9341_WHITE);
            circle(c_x + 120,c_y,r_m,ILI9341_WHITE);
            tft.setCursor(c_t_x, c_t_y); //coordination
            tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
            tft.println('S'); // mode_1
            tft.setCursor(c_t_x + 40, c_t_y); //coordination
            tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
            tft.println('L'); // mode_2
            tft.setCursor(c_t_x + 80, c_t_y); //coordination
            tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
            tft.println('P'); // mode_3
            tft.setCursor(c_t_x + 120, c_t_y); //coordination
            tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
            tft.println('G'); // mode_4
            
            button_state = String("011");
            mode=1;
            f1=0;
            f2=0;      

            // F1
            circle(c_f_x,c_f_y,r_f+5,ILI9341_WHITE);
            circle(c_f_x,c_f_y,r_f,ILI9341_WHITE);
            tft.setCursor(c_f_x-9, c_f_y_t); //coordination
            tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
            tft.println("F1"); 
            // F2
            circle(c_f_x,c_f_y+75,r_f+5,ILI9341_WHITE);
            circle(c_f_x,c_f_y+75,r_f,ILI9341_WHITE);
            tft.setCursor(c_f_x-10, c_f_y+69); //coordination
            tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
            tft.println("F2"); 

            // Parameter_text
            
            tft.fillRect(f1_text_x, f1_text_y - 2, 180, 20, ILI9341_BLACK);
            tft.setCursor(f1_text_x, f1_text_y); //coordination
            tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
            tft.println("FX Function"); // default text
            tft.fillRect(f1_paramater_x, f1_paramater_y - 2, 180, 20, ILI9341_BLACK);
            tft.setCursor(f1_paramater_x, f1_paramater_y); //coordination
            tft.setTextColor(ILI9341_ORANGE);  tft.setTextSize(2);
            if (FX_val == 0)
              tft.print("Regular");
            else if (FX_val == 1)
              tft.print("Reverb");
            else if (FX_val == 2)
              tft.print("Chorus");
              
//            tft.setCursor(f1_paramater_x, f1_paramater_y); //coordination
//            tft.setTextColor(ILI9341_ORANGE);  tft.setTextSize(2);
//            tft.print("F"); // default text
//            tft.print(FX_val); // default text

            tft.fillRect(f2_text_x, f2_text_y - 2, 180, 20, ILI9341_BLACK);
            tft.setCursor(f2_text_x, f2_text_y); //coordination
            tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
            tft.println("Dry Wet"); // default text
            tft.fillRect(f2_paramater_x, f2_paramater_y - 2, 180, 20, ILI9341_BLACK);
            tft.setCursor(f2_paramater_x, f2_paramater_y); //coordination
            tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
            tft.print(Dry_wet_val); // default text
            tft.print("%"); // default text
            
            Serial.println("Mode 1!");
            delay(20);  
        }
        else{
            // overwrite the mode circle and text
            circle(c_x,c_y,r_m,ILI9341_WHITE);
            circle(c_x + 40,c_y,r_m,ILI9341_WHITE);
            circle(c_x + 80,c_y,r_m,ILI9341_WHITE);
            circle(c_x + 120,c_y,r_m,ILI9341_RED);
            tft.setCursor(c_t_x, c_t_y); //coordination
            tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
            tft.println('S'); // mode_1
            tft.setCursor(c_t_x + 40, c_t_y); //coordination
            tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
            tft.println('L'); // mode_2
            tft.setCursor(c_t_x + 80, c_t_y); //coordination
            tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
            tft.println('P'); // mode_3
            tft.setCursor(c_t_x + 120, c_t_y); //coordination
            tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
            tft.println('G'); // mode_4
            
            button_state = String("111");
            mode=4;
            f1=0;
            f2=0;

            // F1
            circle(c_f_x,c_f_y,r_f+5,ILI9341_WHITE);
            circle(c_f_x,c_f_y,r_f,ILI9341_WHITE);
            tft.setCursor(c_f_x-9, c_f_y_t); //coordination
            tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
            tft.println("F1"); 
            // F2
            circle(c_f_x,c_f_y+75,r_f+5,ILI9341_WHITE);
            circle(c_f_x,c_f_y+75,r_f,ILI9341_WHITE);
            tft.setCursor(c_f_x-10, c_f_y+69); //coordination
            tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
            tft.println("F2"); 

            // Parameter_text
            tft.fillRect(f1_text_x, f1_text_y - 2, 180, 20, ILI9341_BLACK);
            tft.setCursor(f1_text_x, f1_text_y); //coordination
            tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
            tft.println("Lighting Mode"); // default text
            tft.fillRect(f1_paramater_x, f1_paramater_y - 2, 180, 20, ILI9341_BLACK);
            tft.setCursor(f1_paramater_x, f1_paramater_y); //coordination
            tft.setTextColor(ILI9341_PINK);  tft.setTextSize(2);
            tft.print("L"); // default text
            tft.println(Lighting_mode_val); // default text

            tft.fillRect(f2_text_x, f2_text_y - 2, 180, 20, ILI9341_BLACK);
            tft.setCursor(f2_text_x, f2_text_y); //coordination
            tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
            tft.println("Master Volume"); // default text
            tft.fillRect(f2_paramater_x, f2_paramater_y - 2, 180, 20, ILI9341_BLACK);
            tft.setCursor(f2_paramater_x, f2_paramater_y); //coordination
            tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
            tft.print(Master_volume_val); // default text
            tft.print("%"); // default text
            
            Serial.println("Mode 4!");
            delay(20); 
        }
        delay(20); 
    }
    last_button_state = button_state;
        sgtl5000_1.unmuteLineout();
        sgtl5000_1.unmuteHeadphone();
    
}

void mode_3()
{
    sgtl5000_1.muteLineout();
    sgtl5000_1.muteHeadphone();
    // mode
    int c_x = global_x + 130; // default x = 40 + 120 = 180
    int c_y = global_y + 160; // default y = 55 + 150 = 205
    // text inside the mode circle
    int c_t_x = c_x - 4; // default x = 60 - 4 = 56
    int c_t_y = c_y - 7; // default y = 205 - 7 = 198
    int r_m = 16;

    // function
    int c_f_x = global_x + 12; // default x = 40 + 120 = 180
    int c_f_y = global_y + 30; // default y = 55 + 150 = 205
    int c_f_x_t = c_f_x + 60;
    int c_f_y_t = c_f_y - 6;
    int r_f = 21;

    int f1_text_x = global_x + 72;
    int f1_text_y = global_y + 9;
    int f1_paramater_x = f1_text_x;
    int f1_paramater_y = global_y + 35;

    int f2_text_x = f1_text_x;
    int f2_text_y = f1_text_y + 75;
    int f2_paramater_x = f2_text_x;
    int f2_paramater_y = f1_paramater_y + 75;

    buttonPress_1 = digitalRead(PIN_SWITCH_1);
    buttonPress_2 = digitalRead(PIN_SWITCH_2);
    buttonPress_3 = digitalRead(PIN_SWITCH_3);
    
    button_state = String(String(buttonPress_1) + String(buttonPress_2) + String(buttonPress_3));\
    
    if (button_state != last_button_state){
      // 3(101)
      if(digitalRead(PIN_SWITCH_1) == HIGH && digitalRead(PIN_SWITCH_2) == LOW && digitalRead(PIN_SWITCH_3) == HIGH){
          // overwrite the mode circle and text
          circle(c_x,c_y,r_m,ILI9341_WHITE);
          circle(c_x + 40,c_y,r_m,ILI9341_WHITE);
          circle(c_x + 80,c_y,r_m,ILI9341_RED);
          circle(c_x + 120,c_y,r_m,ILI9341_WHITE);
          tft.setCursor(c_t_x, c_t_y); //coordination
          tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
          tft.println('S'); // mode_1
          tft.setCursor(c_t_x + 40, c_t_y); //coordination
          tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
          tft.println('L'); // mode_2
          tft.setCursor(c_t_x + 80, c_t_y); //coordination
          tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
          tft.println('P'); // mode_3
          tft.setCursor(c_t_x + 120, c_t_y); //coordination
          tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
          tft.println('G'); // mode_4

          button_state = String("101");
          mode=3;
          f1=0;
          f2=0;

          // F1
          circle(c_f_x,c_f_y,r_f+5,ILI9341_WHITE);
          circle(c_f_x,c_f_y,r_f,ILI9341_WHITE);
          tft.setCursor(c_f_x-9, c_f_y_t); //coordination
          tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
          tft.println("F1"); 
          // F2
          circle(c_f_x,c_f_y+75,r_f+5,ILI9341_WHITE);
          circle(c_f_x,c_f_y+75,r_f,ILI9341_WHITE);
          tft.setCursor(c_f_x-10, c_f_y+69); //coordination
          tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
          tft.println("F2"); 

          // Parameter_text --> same as mode 4 for now
          tft.fillRect(f1_text_x, f1_text_y - 2, 180, 20, ILI9341_BLACK);
          tft.setCursor(f1_text_x, f1_text_y); //coordination
          tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
          tft.println("Lighting Mode"); // default text
          tft.fillRect(f1_paramater_x, f1_paramater_y - 2, 180, 20, ILI9341_BLACK);
          tft.setCursor(f1_paramater_x, f1_paramater_y); //coordination
          tft.setTextColor(ILI9341_PINK);  tft.setTextSize(2);
          tft.println("L"); // default text

          tft.fillRect(f2_text_x, f2_text_y - 2, 180, 20, ILI9341_BLACK);
          tft.setCursor(f2_text_x, f2_text_y); //coordination
          tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
          tft.println("Master Volume"); // default text
          tft.fillRect(f2_paramater_x, f2_paramater_y - 2, 180, 20, ILI9341_BLACK);
          tft.setCursor(f2_paramater_x, f2_paramater_y); //coordination
          tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
          tft.print(0); // default text
          tft.print("%"); // default text

          Serial.println("Mode 3!");
          delay(20);  
      }
      else{
          // overwrite the mode circle and text
          circle(c_x,c_y,r_m,ILI9341_WHITE);
          circle(c_x + 40,c_y,r_m,ILI9341_WHITE);
          circle(c_x + 80,c_y,r_m,ILI9341_WHITE);
          circle(c_x + 120,c_y,r_m,ILI9341_RED);
          tft.setCursor(c_t_x, c_t_y); //coordination
          tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
          tft.println('S'); // mode_1
          tft.setCursor(c_t_x + 40, c_t_y); //coordination
          tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
          tft.println('L'); // mode_2
          tft.setCursor(c_t_x + 80, c_t_y); //coordination
          tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
          tft.println('P'); // mode_3
          tft.setCursor(c_t_x + 120, c_t_y); //coordination
          tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
          tft.println('G'); // mode_4
            
          button_state = String("111");
          mode=4;
          f1=0;
          f2=0;

          // F1
          circle(c_f_x,c_f_y,r_f+5,ILI9341_WHITE);
          circle(c_f_x,c_f_y,r_f,ILI9341_WHITE);
          tft.setCursor(c_f_x-9, c_f_y_t); //coordination
          tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
          tft.println("F1"); 
          // F2
          circle(c_f_x,c_f_y+75,r_f+5,ILI9341_WHITE);
          circle(c_f_x,c_f_y+75,r_f,ILI9341_WHITE);
          tft.setCursor(c_f_x-10, c_f_y+69); //coordination
          tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
          tft.println("F2"); 

          // Parameter_text
          tft.fillRect(f1_text_x, f1_text_y - 2, 180, 20, ILI9341_BLACK);
          tft.setCursor(f1_text_x, f1_text_y); //coordination
          tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
          tft.println("Lighting Mode"); // default text
          tft.fillRect(f1_paramater_x, f1_paramater_y - 2, 180, 20, ILI9341_BLACK);
          tft.setCursor(f1_paramater_x, f1_paramater_y); //coordination
          tft.setTextColor(ILI9341_PINK);  tft.setTextSize(2);
          tft.print("L"); // default text
          tft.println(Lighting_mode_val); // default text

          tft.fillRect(f2_text_x, f2_text_y - 2, 180, 20, ILI9341_BLACK);
          tft.setCursor(f2_text_x, f2_text_y); //coordination
          tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
          tft.println("Master Volume"); // default text
          tft.fillRect(f2_paramater_x, f2_paramater_y - 2, 180, 20, ILI9341_BLACK);
          tft.setCursor(f2_paramater_x, f2_paramater_y); //coordination
          tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
          tft.print(Master_volume_val); // default text
          tft.print("%"); // default text
          
          Serial.println("Mode 4!");
          delay(20);   
      }
      delay(20); 
    }
    sgtl5000_1.unmuteLineout();
    sgtl5000_1.unmuteHeadphone();
    last_button_state = button_state;
}

void mode_2()
{  
    sgtl5000_1.muteLineout();
    sgtl5000_1.muteHeadphone();
    // mode
    int c_x = global_x + 130; // default x = 40 + 120 = 180
    int c_y = global_y + 160; // default y = 55 + 150 = 205
    // text inside the mode circle
    int c_t_x = c_x - 4; // default x = 60 - 4 = 56
    int c_t_y = c_y - 7; // default y = 205 - 7 = 198
    int r_m = 16;

    // function
    int c_f_x = global_x + 12; // default x = 40 + 120 = 180
    int c_f_y = global_y + 30; // default y = 55 + 150 = 205
    int c_f_x_t = c_f_x + 60;
    int c_f_y_t = c_f_y - 6;
    int r_f = 21;

    int f1_text_x = global_x + 72;
    int f1_text_y = global_y + 9;
    int f1_paramater_x = f1_text_x;
    int f1_paramater_y = global_y + 35;

    int f2_text_x = f1_text_x;
    int f2_text_y = f1_text_y + 75;
    int f2_paramater_x = f2_text_x;
    int f2_paramater_y = f1_paramater_y + 75;
    
    buttonPress_1 = digitalRead(PIN_SWITCH_1);
    buttonPress_2 = digitalRead(PIN_SWITCH_2);
    buttonPress_3 = digitalRead(PIN_SWITCH_3);
    
    button_state = String(String(buttonPress_1) + String(buttonPress_2) + String(buttonPress_3));
    
    if (button_state != last_button_state){
      // 011 -> 2(110) [Unique Case]
      if(digitalRead(PIN_SWITCH_3) == LOW){
          // overwrite the mode circle and text
          circle(c_x,c_y,r_m,ILI9341_WHITE);
          circle(c_x + 40,c_y,r_m,ILI9341_RED);
          circle(c_x + 80,c_y,r_m,ILI9341_WHITE);
          circle(c_x + 120,c_y,r_m,ILI9341_WHITE);
          tft.setCursor(c_t_x, c_t_y); //coordination
          tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
          tft.println('S'); // mode_1
          tft.setCursor(c_t_x + 40, c_t_y); //coordination
          tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
          tft.println('L'); // mode_2
          tft.setCursor(c_t_x + 80, c_t_y); //coordination
          tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
          tft.println('P'); // mode_3
          tft.setCursor(c_t_x + 120, c_t_y); //coordination
          tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
          tft.println('G'); // mode_4
          
          button_state = String("110");
          mode=2;
          f1=0;
          f2=0;

          // F1
          circle(c_f_x,c_f_y,r_f+5,ILI9341_WHITE);
          circle(c_f_x,c_f_y,r_f,ILI9341_WHITE);
          tft.setCursor(c_f_x-9, c_f_y_t); //coordination
          tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
          tft.println("F1"); 
          // F2
          circle(c_f_x,c_f_y+75,r_f+5,ILI9341_WHITE);
          circle(c_f_x,c_f_y+75,r_f,ILI9341_WHITE);
          tft.setCursor(c_f_x-10, c_f_y+69); //coordination
          tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
          tft.println("F2"); 

          // Parameter_text
          tft.fillRect(f1_text_x, f1_text_y - 2, 180, 20, ILI9341_BLACK);
          tft.setCursor(f1_text_x, f1_text_y); //coordination
          tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
          tft.println("Color Scheme"); // default text
          tft.fillRect(f1_paramater_x, f1_paramater_y - 2, 180, 20, ILI9341_BLACK);
          tft.setCursor(f1_paramater_x, f1_paramater_y); //coordination
          tft.setTextColor(ILI9341_CYAN);  tft.setTextSize(2);
          tft.print("C"); // default text
          tft.print(Color_scheme_val); // default text

          tft.fillRect(f2_text_x, f2_text_y - 2, 180, 20, ILI9341_BLACK);
          tft.setCursor(f2_text_x, f2_text_y); //coordination
          tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
          tft.println("Gain"); // default text
          tft.fillRect(f2_paramater_x, f2_paramater_y - 2, 180, 20, ILI9341_BLACK);
          tft.setCursor(f2_paramater_x, f2_paramater_y); //coordination
          tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
          tft.print(Gain_val); // default text
          tft.print("%"); // default text
          
          Serial.println("Mode 2!");
          delay(20);   
      }
      else{
          // overwrite the mode circle and text
          circle(c_x,c_y,r_m,ILI9341_WHITE);
          circle(c_x + 40,c_y,r_m,ILI9341_WHITE);
          circle(c_x + 80,c_y,r_m,ILI9341_WHITE);
          circle(c_x + 120,c_y,r_m,ILI9341_RED);
          tft.setCursor(c_t_x, c_t_y); //coordination
          tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
          tft.println('S'); // mode_1
          tft.setCursor(c_t_x + 40, c_t_y); //coordination
          tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
          tft.println('L'); // mode_2
          tft.setCursor(c_t_x + 80, c_t_y); //coordination
          tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
          tft.println('P'); // mode_3
          tft.setCursor(c_t_x + 120, c_t_y); //coordination
          tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
          tft.println('G'); // mode_4
                  
          button_state = String("111");
          mode=4;
          f1=0;
          f2=0;
          
          // F1
          circle(c_f_x,c_f_y,r_f+5,ILI9341_WHITE);
          circle(c_f_x,c_f_y,r_f,ILI9341_WHITE);
          tft.setCursor(c_f_x-9, c_f_y_t); //coordination
          tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
          tft.println("F1"); 
          // F2
          circle(c_f_x,c_f_y+75,r_f+5,ILI9341_WHITE);
          circle(c_f_x,c_f_y+75,r_f,ILI9341_WHITE);
          tft.setCursor(c_f_x-10, c_f_y+69); //coordination
          tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
          tft.println("F2"); 

          // Parameter_text
          tft.fillRect(f1_text_x, f1_text_y - 2, 180, 20, ILI9341_BLACK);
          tft.setCursor(f1_text_x, f1_text_y); //coordination
          tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
          tft.println("Lighting Mode"); // default text
          tft.fillRect(f1_paramater_x, f1_paramater_y - 2, 180, 20, ILI9341_BLACK);
          tft.setCursor(f1_paramater_x, f1_paramater_y); //coordination
          tft.setTextColor(ILI9341_PINK);  tft.setTextSize(2);
          tft.print("L"); // default text
          tft.println(Lighting_mode_val); // default text

          tft.fillRect(f2_text_x, f2_text_y - 2, 180, 20, ILI9341_BLACK);
          tft.setCursor(f2_text_x, f2_text_y); //coordination
          tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
          tft.println("Master Volume"); // default text
          tft.fillRect(f2_paramater_x, f2_paramater_y - 2, 180, 20, ILI9341_BLACK);
          tft.setCursor(f2_paramater_x, f2_paramater_y); //coordination
          tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
          tft.print(Master_volume_val); // default text
          tft.print("%"); // default text
            
          Serial.println("Mode 4!");
          delay(20); 
      }
      delay(20); 
    }
    last_button_state = button_state;
    sgtl5000_1.unmuteLineout();
    sgtl5000_1.unmuteHeadphone();
    
}

// --------------------------- overwriting parameters ---------------------------------- //

void parameters()
{  
      int f1_text_x = global_x + 72;
      int f1_text_y = global_y + 9;
      int f1_paramater_x = f1_text_x;
      int f1_paramater_y = global_y + 35;

      int f2_text_x = f1_text_x;
      int f2_text_y = f1_text_y + 75;
      int f2_paramater_x = f2_text_x;
      int f2_paramater_y = f1_paramater_y + 75;

      if (FX == 1){
        tft.fillRect(f1_paramater_x, f1_paramater_y - 2, 180, 20, ILI9341_BLACK);
        tft.setCursor(f1_paramater_x, f1_paramater_y); //coordination
        tft.setTextColor(ILI9341_ORANGE);  tft.setTextSize(2);
        if (FX_val == 0)
          tft.print("Regular");
        else if (FX_val == 1)
          tft.print("Reverb");
        else if (FX_val == 2)
          tft.print("Chorus");
      }
      else if (Color_scheme == 1){
        tft.fillRect(f1_paramater_x, f1_paramater_y - 2, 180, 20, ILI9341_BLACK);
        tft.setCursor(f1_paramater_x, f1_paramater_y); //coordination
        tft.setTextColor(ILI9341_CYAN);  tft.setTextSize(2);
        tft.print("C");
        tft.print(Color_scheme_val);
      }
      else if (Lighting_mode == 1){
        tft.fillRect(f1_paramater_x, f1_paramater_y - 2, 180, 20, ILI9341_BLACK);
        tft.setCursor(f1_paramater_x, f1_paramater_y); //coordination
        tft.setTextColor(ILI9341_PINK);  tft.setTextSize(2);
        tft.print("L");
        tft.print(Lighting_mode_val);
      }
      else if (Dry_wet == 1){
        tft.fillRect(f2_paramater_x, f2_paramater_y - 2, 180, 20, ILI9341_BLACK);
        tft.setCursor(f2_paramater_x, f2_paramater_y); //coordination
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
        tft.print(Dry_wet_val);
        tft.print("%");
      }
      else if (Gain == 1){
        tft.fillRect(f2_paramater_x, f2_paramater_y - 2, 180, 20, ILI9341_BLACK);
        tft.setCursor(f2_paramater_x, f2_paramater_y); //coordination
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
        tft.print(Gain_val);
        tft.print("%");
      }
      else if (Master_volume == 1){
        tft.fillRect(f2_paramater_x, f2_paramater_y - 2, 180, 20, ILI9341_BLACK);
        tft.setCursor(f2_paramater_x, f2_paramater_y); //coordination
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
        tft.print(Master_volume_val);
        tft.print("%");
      }
}


// --------------------------- setting parameters (Encoder) ---------------------------------- //

void encoder_1()
{
      // set the initial parameters for the parameters bar
      int b1_x=global_x + 12, b1_y=global_y + 30, w=50, h=10; // default x = 175 = 40 + 135 ; y = 190 = 40 + 150
      int text1_x = b1_x + 60, text_y = b1_y + 5;
      
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

      counter_A = constrain(counter_A,1,3); // 0-4
     
      if (bouncer_1.risingEdge() && buttonPushCounter_1 == 1){
       // If the previous and the current state of the outputA are different, that means a Pulse has occured
       if (A_aState != A_aLastState){     
         // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
         // rewrite the text in the background color

         if (digitalRead(E1_outputB) != A_aState) { 
           counter_A ++;
         } 
         else {
           counter_A --;
         }
         
         Serial.print("Position: ");
         Serial.println(counter_A);

         // overwrite the encoder value as text on top of the bar
         function_selection();
         parameters();            
       }
       pre_counter_A = counter_A;
       
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
            Serial.print("Status: ");
            Serial.println(buttonPushCounter_1);
      
            Serial.print("Current mode: ");
            Serial.println(mode);
            Serial.print("F1: ");
            Serial.println(f1);
            Serial.print("F2: ");
            Serial.println(f2);
      
            // check the current mode and current function
            chosen_func();
            function_selection();
            delay(50);
        }
       
        
        // Delay a little bit to avoid bouncing
        delay(50);
      }
      // save the current state as the last state, for next time through the loop
      A_lastButtonState = A_buttonState;
}

void encoder_2()
{         
  
     // Update the debouncer
     bouncer_2.update();
     
     B_aState = digitalRead(E2_outputA); // Reads the "current" state of the outputA
     B_buttonState = digitalRead(E2_SW); // read the state of the button

// *********** differentiating different functions *********** //

     if (Color_scheme == 1){
        counter_B = constrain(counter_B,1,3); // 0-4
     }
     else if (Gain == 1){
        counter_B = constrain(counter_B,1,99); // 0-10
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
          if (mode == 1){
           if (digitalRead(E2_outputB) != B_aState) { 
             counter_B= counter_B+10;
           } 
           else {
             counter_B= counter_B - 10;
           }
         }
         else if (mode == 2){
           if (digitalRead(E2_outputB) != B_aState) { 
             counter_B= counter_B + .25;
           } 
           else {
             counter_B= counter_B - .25;
           }
         }
         else if (digitalRead(E2_outputB) != B_aState) { 
           counter_B ++;
         } 
         else {
           counter_B --;
         }
         
         Serial.print("Position: ");
         Serial.println(counter_B);

         // overwrite the encoder value as text on top of the bar
         function_selection();
         parameters();
              
       }
       pre_counter_B = counter_B;
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
            chosen_func();
            function_selection();
            delay(50);
        }
        
        // Delay a little bit to avoid bouncing
        delay(50);
      }
      // save the current state as the last state, for next time through the loop
      B_lastButtonState = B_buttonState;
}


// ------------------------- select the function ------------------------------- //
     
void function_selection(){
  
// ************** Color_scheme ************** //
    if (mode == 2 && f1 == 1){
        Color_scheme = 1;
        Gain = 0;
        Lighting_mode = 0;
        Master_volume = 0;
        FX = 0;
        Dry_wet = 0;
        Color_scheme_val = Color_scheme_val + (counter_A - pre_counter_A);
        Color_scheme_val = constrain(Color_scheme_val,0,4); 
        Serial.println("Color_scheme is ON!"); 
    }  
// ************** Gain ************** //
    else if (mode == 2 && f2 == 1){
        Color_scheme = 0;
        Gain = 1;
        Lighting_mode = 0;
        Master_volume = 0;
        FX = 0;
        Dry_wet = 0; 
        Gain_val = counter_B;
        Gain_val = Gain_val + (counter_B - pre_counter_B);
        Gain_val = constrain(Gain_val,0,10); // 1-10
        Serial.println("Gain is ON!");
    } 
// ************** Lighting_mode ************** //
    else if (mode == 4 && f1 == 1){
        Color_scheme = 0;
        Gain = 0;
        Lighting_mode = 1;
        Master_volume = 0;
        FX = 0;
        Dry_wet = 0; 
        Lighting_mode_val = Lighting_mode_val + (counter_A - pre_counter_A);
        Lighting_mode_val = constrain(Lighting_mode_val,1,2); // 1-2
        Serial.println("Lighting_mode is ON!");
    }   
// ************** Master_volume ************** //
    else if (mode == 4 && f2 == 1){
        Color_scheme = 0;
        Gain = 0;
        Lighting_mode = 0;
        Master_volume = 1;
        FX = 0;
        Dry_wet = 0; 
        Master_volume_val = Master_volume_val + (counter_B - pre_counter_B);
        Master_volume_val = constrain(Master_volume_val,0,10); // 1-100
        Serial.println("Master_volume is ON!");
    }  
// ************** FX ************** //
    else if (mode == 1 && f1 == 1){
        Color_scheme = 0;
        Gain = 0;
        Lighting_mode = 0;
        Master_volume = 0;
        FX = 1;
        Dry_wet = 0; 
        FX_val = FX_val + (counter_A - pre_counter_A);
        FX_val = constrain(FX_val,0,3); // 0-3
        Serial.println("FX is ON!");
    } 
// ************** Dry_wet ************** //
    else if (mode == 1 && f2 == 1){
        Color_scheme = 0;
        Gain = 0;
        Lighting_mode = 0;
        Master_volume = 0;
        FX = 0;
        Dry_wet = 1; 
        Dry_wet_val = Dry_wet_val + (counter_B - pre_counter_B);
        Dry_wet_val = constrain(Dry_wet_val,0,100); // 1-100
        Serial.println("Dry_wet is ON!");
    }  
}

void runFFT(){
  int microsec = 100000 / leds.numPixels();
  count++;
  if (fft256_1.available()) {
      
      nn = fft256_1.read(1);
      a = fft256_1.read(2);
      b = fft256_1.read(3);
      d = fft256_1.read(4);
      e = fft256_1.read(5);

      g = fft256_1.read(6);
      h = fft256_1.read(7);

      j = fft256_1.read(8);
      k = fft256_1.read(9);
      l = fft256_1.read(10);
      n = fft256_1.read(11);
      o = fft256_1.read(12);
      p = fft256_1.read(13);

      r = fft256_1.read(14);
      s = fft256_1.read(15);
      t = fft256_1.read(16);

           
     printNumber(nn);
      printNumber(a);
      printNumber(b);
      printNumber(d);
      printNumber(e);
      printNumber(g);
      printNumber(h);
      printNumber(j);

     Serial.println();
    }

    if (count == 1000){
      count = 0;
      
      Bar1(microsec,nn,iteration);
      Bar2(microsec,a,iteration);
      Bar3(microsec,b,iteration);
      Bar4(microsec,d,iteration);
      Bar5(microsec,e,iteration);
      Bar6(microsec,g,iteration);
      Bar7(microsec,h,iteration);
      Bar8(microsec,j,iteration); 
      Bar9(microsec,k,iteration);
      Bar10(microsec,l,iteration);
      Bar11(microsec,n,iteration);
      Bar12(microsec,o,iteration);
      Bar13(microsec,p,iteration);
      Bar14(microsec,r,iteration);
      Bar15(microsec,s,iteration);
      Bar16(microsec,t,iteration);

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
//  
//  if (n >= 0.004) {
//    Serial.print(n, 3);
//    Serial.print(" ");
//  } else {
//    Serial.print("   -  "); // don't print "0.00"
//  }

}
