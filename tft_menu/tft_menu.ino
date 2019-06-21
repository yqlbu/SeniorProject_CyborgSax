#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>
#include <Bounce.h>

#define ILI9341_ORANGE 0xFD20 /* 255, 165, 0 */
#define ILI9341_CYAN 0x07FF /* 0, 255, 255 */
#define ILI9341_PINK 0xF81F

#define E1_outputA 2
#define E1_outputB 3
#define E1_SW 8

#define E2_outputA 5
#define E2_outputB 7
#define E2_SW 4
//
//#define TFT_DC 9              
//#define TFT_CS 10             
//#define TFT_RST 8             
//#define TFT_MISO 12           
//#define TFT_MOSI 11           
//#define TFT_CLK 13

#define TFT_DC 20              
#define TFT_CS 21             
#define TFT_RST 255             
#define TFT_MISO 12           
#define TFT_MOSI 7           
#define TFT_SCK 14 
  
#define PIN_SWITCH_1  35
#define PIN_SWITCH_2  36
#define PIN_SWITCH_3  37

int buttonPress_1 = 0;
int buttonPress_2 = 0;
int buttonPress_3 = 0;

String button_state, last_button_state;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_RST, TFT_MISO);

// set the following parameters for overall coordinates of the GUI layout
int global_x = 40; // set default, identical to the F1 rectangle x=40
int global_y = 55;  // set default, identical to the F1 rectangle y=40
int mode;
int f1,f2;

// defining functions here
int Color_scheme, Gain, Lighting_mode, Master_volume, FX, Dry_wet;

// defining parameters here
int FX_val, Color_scheme_val, Gain_val, Lighting_mode_val, Master_volume_val, Dry_wet_val;

//  encoder A
int counter_A=0;
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

int pre_counter_A, pre_counter_B;

Bounce bouncer_1 = Bounce (E1_outputA , 2); //Initiate a Bounce object with a 2 milisecond debounce time
Bounce bouncer_2 = Bounce (E2_outputA , 2); //Initiate a Bounce object with a 2 milisecond debounce time

void setup() {
    // put your setup code here, to run once:
//    while (!Serial) {
//      ; // wait for serial port to connect. Needed for native USB port only
//    }
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
    
    Serial.begin(9600);
}

void loop() {
    // put your main code here, to run repeatedly:
    encoder_1();
    encoder_2();
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
      }
      else{
        //overwrite the text
        tft.fillRect(c_f_x_t, c_f_y_t - 17, 180, 20, ILI9341_BLACK);
        tft.setCursor(c_f_x_t, c_f_y_t - 15); //coordination
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
        tft.println("FX Function"); // default text
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
      }
      else{
        //overwrite the text
        tft.fillRect(f2_text_x, f2_text_y - 2, 180, 20, ILI9341_BLACK);
        tft.setCursor(f2_text_x, f2_text_y); //coordination
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
        tft.println("Dry Wet"); // default text
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
      }
      else{
        tft.fillRect(c_f_x_t, c_f_y_t - 17, 180, 20, ILI9341_BLACK);
        tft.setCursor(c_f_x_t, c_f_y_t - 15); //coordination
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
        tft.println("Color Scheme"); // default text
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
      }
      else{
        tft.fillRect(f2_text_x, f2_text_y - 2, 180, 20, ILI9341_BLACK);
        tft.setCursor(f2_text_x, f2_text_y); //coordination
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
        tft.println("Gain"); // default text
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
      }
      else{
        tft.fillRect(c_f_x_t, c_f_y_t - 17, 180, 20, ILI9341_BLACK);
        tft.setCursor(c_f_x_t, c_f_y_t - 15); //coordination
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
        tft.println("Lighting Mode"); // default text
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
      }
      else{
        tft.fillRect(f2_text_x, f2_text_y - 2, 180, 20, ILI9341_BLACK);
        tft.setCursor(f2_text_x, f2_text_y); //coordination
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
        tft.println("Master Volume"); // default text
      }
    }
}


// --------------------------- switching mode ---------------------------------- //

void mode_1()
{   
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
            tft.print("F"); // default text
            tft.print(FX_val); // default text

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
    
}

void mode_3()
{
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
          tft.println("L1"); // default text

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
    last_button_state = button_state;
}

void mode_2()
{  
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
        tft.print("F");
        tft.print(FX_val);
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
            delay(300);
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
         
         if (digitalRead(E2_outputB) != B_aState) { 
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
            delay(300);
        }
        
        // Delay a little bit to avoid bouncing
        delay(50);
      }
      // save the current state as the last state, for next time through the loop
      B_lastButtonState = B_buttonState;
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
        Color_scheme_val = Color_scheme_val + (counter_A - pre_counter_A);
        Color_scheme_val = constrain(Color_scheme_val,0,4); 
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
        Gain_val = counter_B;
        Gain_val = Gain_val + (counter_B - pre_counter_B);
        Gain_val = constrain(Gain_val,0,10); // 1-10
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
        Lighting_mode_val = Lighting_mode_val + (counter_A - pre_counter_A);
        Lighting_mode_val = constrain(Lighting_mode_val,1,2); // 1-2
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
        Master_volume_val = Master_volume_val + (counter_B - pre_counter_B);
        Master_volume_val = constrain(Master_volume_val,1,100); // 1-100
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
        FX_val = FX_val + (counter_A - pre_counter_A);
        FX_val = constrain(FX_val,0,3); // 0-3
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
        Dry_wet_val = Dry_wet_val + (counter_B - pre_counter_B);
        Dry_wet_val = constrain(Dry_wet_val,1,100); // 1-100
        Serial.println("Dry_wet is ON!");
    }  
}
