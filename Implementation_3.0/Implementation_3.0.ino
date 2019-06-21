
#include <WS2812Serial.h>

const int numled = 128;
const int pin = 26;

int Color_1, Color_2, Color_3;

// Usable pins:
//   Teensy LC:   1, 4, 5, 24
//   Teensy 3.2:  1, 5, 8, 10, 31   (overclock to 120 MHz for pin 8)
//   Teensy 3.5:  1, 5, 8, 10, 26, 32, 33, 48
//   Teensy 3.6:  1, 5, 8, 10, 26, 32, 33

//using pin 26 for matrix

byte drawingMemory[numled*3];         //  3 bytes per LED
DMAMEM byte displayMemory[numled*12]; // 12 bytes per LED

WS2812Serial leds(numled, displayMemory, drawingMemory, pin, WS2812_GRB);

//#define  RED  0x160000 //Red
//#define  GREEN  0x001600 //Green
//#define  BLUE  0x000016 //Blue
//#define  YELLOW 0x101400 //Yellow
//#define  Pink    0x120009 //Pink
//#define  ORANGE 0x100400 //Orange
//#define  WHITE  0x101010 //White
//#define  Purple  0x7600DC //Purple
#define  OFF    0x000000 //Off

#define RED    0xFF0000
#define GREEN  0x00FF00
#define BLUE   0x0000FF
#define YELLOW 0xFFFF00
#define PINK   0xFF1088
#define ORANGE 0xE05800
#define WHITE  0xFFFFFF



//#include <Audio.h>
//#include <Wire.h>
//#include <SPI.h>
//#include <SD.h>
//#include <SerialFlash.h>
//
//// GUItool: begin automatically generated code
//AudioInputI2S            Mic;           //xy=112,170
//AudioMixer4              mixer1;         //xy=314,178
//AudioAmplifier           amp1;           //xy=448,176
//AudioOutputI2S           i2s1;           //xy=586,155
//AudioAnalyzeFFT1024      fft1024_1;      //xy=588,230
//AudioConnection          patchCord1(Mic, 0, mixer1, 0);
//AudioConnection          patchCord2(Mic, 1, mixer1, 1);
//AudioConnection          patchCord3(mixer1, amp1);
//AudioConnection          patchCord4(amp1, 0, i2s1, 0);
//AudioConnection          patchCord5(amp1, 0, i2s1, 1);
//AudioConnection          patchCord6(amp1, fft1024_1);
//AudioControlSGTL5000     sgtl5000_1;     //xy=395,364
//// GUItool: end automatically generated code

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            Mic;           //xy=107,162
AudioMixer4              mixer1;         //xy=314,178
AudioOutputI2S           i2s1;           //xy=472,172
AudioAnalyzeFFT1024      fft1024_1;      //xy=482,249
AudioConnection          patchCord1(Mic, 0, mixer1, 0);
AudioConnection          patchCord2(Mic, 1, mixer1, 1);
AudioConnection          patchCord3(mixer1, 0, i2s1, 0);
AudioConnection          patchCord4(mixer1, fft1024_1);
AudioControlSGTL5000     sgtl5000_1;     //xy=399,357
// GUItool: end automatically generated code





void setup() {
  AudioMemory(12);
  Serial.begin(9600);
  leds.begin();
  sgtl5000_1.enable();
  sgtl5000_1.volume(.7);
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  //sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);
  //sgtl5000_1.micGain(20);
  //biquad1.setHighpass(3, 200, .71);
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  mixer1.gain(0,30);
  mixer1.gain(1,-30);
  mixer1.gain(2,1);
  mixer1.gain(3,1);
  sgtl5000_1.adcHighPassFilterDisable();

  delay(1000);
 // playSdWav1.play("KICK.WAV");
 
   
}


float n, m, w, a, b, c, d, e, f, g, h, i, j, k, l, o, p, q, r, s, t, u, v, x, y, mm, nn, ww, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az, ba, bb, bc, bd, be, bf;

int count = 0;
int iteration = 1;

int cs1, cs2, cs3, cs4, cs5, cs6, cs7, cs8, cs9, cs10, cs11, cs12, cs13, cs14, cs15, cs16;


void loop() {
  count++;

  int color_choose = 0;

  if (color_choose == 0){
    Color_1 = GREEN;
    Color_2 = YELLOW;
    Color_3 = RED;
  }
  else if (color_choose == 1){
    Color_1 = BLUE;
    Color_2 = ORANGE;
    Color_3 = RED;
  }
  else if (color_choose == 2){
    Color_1 = PINK;
    Color_2 = YELLOW;
    Color_3 = GREEN;
  }
  else if (color_choose == 3){
    Color_1 = ORANGE;
    Color_2 = BLUE;
    Color_3 = PINK;
  }
  else if (color_choose == 4){
    Color_1 = BLUE;
    Color_2 = ORANGE;
    Color_3 = WHITE;
  }

  int microsec = 100000 / leds.numPixels();
  if (fft1024_1.available()) {
      
      nn = fft1024_1.read(1);
      mm= fft1024_1.read(2);
      ww = nn+mm;
      a = fft1024_1.read(3);
      b = fft1024_1.read(4);
      c = a + b;
      d = fft1024_1.read(5);
      e = fft1024_1.read(6);
      f = e + d;
      g = fft1024_1.read(7);
      h = fft1024_1.read(8);
      i = g + h;
      j = fft1024_1.read(9);
      k = fft1024_1.read(10);
      l = fft1024_1.read(11);
      m = j + k + l;
      n = fft1024_1.read(12);
      o = fft1024_1.read(13);
      p = fft1024_1.read(14);
      q = n + o + p;
      r = fft1024_1.read(15);
      s = fft1024_1.read(16);
      t = fft1024_1.read(17);
      u = r + s + t;
      v = fft1024_1.read(18);
      w = fft1024_1.read(19);
      x = fft1024_1.read(20);
      y = v + w + x;
      z = fft1024_1.read(21);
      aa = fft1024_1.read(22);
      ab = fft1024_1.read(23);
      ac = z + aa + ab;
      ad = fft1024_1.read(24);
      ae = fft1024_1.read(25);
      af = fft1024_1.read(26);
      ag = ad + ae + af;
      ah = fft1024_1.read(27);
      ai = fft1024_1.read(28);
      aj = fft1024_1.read(29);
      ak = ah + ai + aj;     
      al = fft1024_1.read(30);
      am = fft1024_1.read(31);
      an = fft1024_1.read(32);
      ao = al + am + an;
      ap = fft1024_1.read(33);
      aq = fft1024_1.read(34);
      ar = fft1024_1.read(35);
      as = ap + aq + ar;
      at = fft1024_1.read(36);
      au = fft1024_1.read(37);
      av = fft1024_1.read(38);
      aw = at + au + av;
      ax = fft1024_1.read(39);
      ay = fft1024_1.read(40);
      az = fft1024_1.read(41);
      ba = ax + ay + az;
      bb = fft1024_1.read(42);
      bc = fft1024_1.read(43);
      bd = fft1024_1.read(44);
      be = fft1024_1.read(45);
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

void Bar1(int wait, float n, int i){
  Serial.println(n);
  if (i == 1000){
    if (n >= 0.00 and n <= 0.035) {
        leds.setPixel(0,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs1=1;
    }
    else if (n >= 0.036 and n <= 0.06) {
        leds.setPixel(0,Color_1);
        leds.setPixel(15,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs1=2;
    }
     else if (n >= 0.061 and n <= 0.08) {
        leds.setPixel(0,Color_1);
        leds.setPixel(15,Color_1);
        leds.setPixel(16,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs1=3;
    }
      else if (n >= 0.081 and n <= 0.1) {
        leds.setPixel(0,Color_1);
        leds.setPixel(15,Color_1);
        leds.setPixel(16,Color_1);
        leds.setPixel(31,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs1=4;
    } 
    else if (n >= 0.101 and n <= 0.12) {
        leds.setPixel(0,Color_1);
        leds.setPixel(15,Color_1);
        leds.setPixel(16,Color_1);
        leds.setPixel(31,Color_1);
        leds.setPixel(32,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs1=5;
    } 
    else if (n >= 0.121 and n <= 0.13) {
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
  
    else if (n >= 0.131 and n <= 0.14) {
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
    else if (n >= 0.141) {
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
    if (cs1 == 0 and n >= .014){
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
    if (cs1 == 0 and n <= .0139){
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
    else if (cs1 == 2 and n <= .0356){
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
    else if (cs1 == 3 and n >= .08){
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
    else if (cs1 == 3 and n <= .06){
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
    else if (cs1 == 4 and n >= .1){
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
    else if (cs1 == 4 and n <= .08){
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
    else if (cs1 == 5 and n >= .12){
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
    if (cs1 == 5 and n <= .1){
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
    else if (cs1 == 6 and n >= .13){
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
    else if (cs1 == 6 and n <= .12){
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
    else if (cs1 == 7 and n >= .14){
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
    else if (cs1 == 7 and n <= .13){
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
    if (cs1 == 8 and n >= .141){
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
    else if (cs1 == 8 and n <= .14){
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

  if (i ==1000){
    if (n >= 0.016 and n <= 0.03) {
        leds.setPixel(5,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs6=1;
    }
    else if (n >= 0.031 and n <= 0.04) {
        leds.setPixel(5,Color_1);
        leds.setPixel(10,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs6=2;
    }
     else if (n >= 0.041 and n <= 0.050) {
        leds.setPixel(5,Color_1);
        leds.setPixel(10,Color_1);
        leds.setPixel(21,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs6=3;
    }
      else if (n >= 0.051 and n <= 0.06) {
        leds.setPixel(5,Color_1);
        leds.setPixel(10,Color_1);
        leds.setPixel(21,Color_1);
        leds.setPixel(28,Color_1);
        leds.show();
        delayMicroseconds(wait);
        cs6=4;
    } 
    else if (n >= 0.061 and n <= 0.1) {
        leds.setPixel(5,Color_1);
        leds.setPixel(10,Color_1);
        leds.setPixel(21,Color_1);
        leds.setPixel(26,Color_1);
        leds.setPixel(37,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs6=5;
    } 
    else if (n >= 0.101 and n <= 0.15) {
        leds.setPixel(5,Color_1);
        leds.setPixel(10,Color_1);
        leds.setPixel(21,Color_1);
        leds.setPixel(26,Color_1);
        leds.setPixel(37,Color_2);
        leds.setPixel(42,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs6=6;
    } 
  
    else if (n >= 0.151 and n <= 0.201) {
        leds.setPixel(0,Color_1);
        leds.setPixel(15,Color_1);
        leds.setPixel(16,Color_1);
        leds.setPixel(26,Color_1);
        leds.setPixel(37,Color_2);
        leds.setPixel(42,Color_2);
        leds.setPixel(53,Color_2);
        leds.show();
        delayMicroseconds(wait);
        cs6=7;
    } 
    else if (n >= 0.201) {
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
  else if ( i>1000){
    if (cs6 == 0 and n >= .016){
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
    if (cs6 == 0 and n < .016){
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
    if (cs6 == 1 and n >= .03){
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
    else if (cs6 == 1 and n <= .016){
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
    else if (cs6 == 2 and n >= .04){
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
    else if (cs6 == 2 and n <= .03){
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
    else if (cs6 == 3 and n >= .05){
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
    else if (cs6 == 3 and n <= .04){
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
    else if (cs6 == 4 and n >= .06){
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
        cs5=6;
    }
    else if (cs6 == 4 and n <= .05){
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
    else if (cs6 == 5 and n >= .1){
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
        cs6=6;      
    }
    if (cs6 == 5 and n <= .06){
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
    else if (cs6 == 6 and n >= .15){
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
    else if (cs6 == 6 and n <= .1){
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
    else if (cs6 == 7 and n >= .2){
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
    else if (cs6 == 7 and n <= .15){
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
        cs6=6;
    }
    if (cs6 == 8 and n >= .2){
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
    else if (cs6 == 8 and n < .2){
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
        cs7=6;
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
