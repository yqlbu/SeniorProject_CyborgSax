
#include <WS2812Serial.h>

const int numled = 64;
const int pin = 26;

// Usable pins:
//   Teensy LC:   1, 4, 5, 24
//   Teensy 3.2:  1, 5, 8, 10, 31   (overclock to 120 MHz for pin 8)
//   Teensy 3.5:  1, 5, 8, 10, 26, 32, 33, 48
//   Teensy 3.6:  1, 5, 8, 10, 26, 32, 33

//using pin 26 for matrix

byte drawingMemory[numled*3];         //  3 bytes per LED
DMAMEM byte displayMemory[numled*12]; // 12 bytes per LED

WS2812Serial leds(numled, displayMemory, drawingMemory, pin, WS2812_GRB);

#define RED    0x160000
#define GREEN  0x001600
#define BLUE   0x000016
#define YELLOW 0x101400
#define PINK   0x120009
#define ORANGE 0x100400
#define WHITE  0x101010

//#define RED    0xFF0000
//#define GREEN  0x00FF00
//#define BLUE   0x0000FF
//#define YELLOW 0xFFFF00
//#define PINK   0xFF1088
//#define ORANGE 0xE05800
//#define WHITE  0xFFFFFF

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlayMemory          playMem1;       //xy=164,216
AudioSynthWaveform       waveform1;      //xy=166,276
AudioPlaySdWav           playSdWav1;     //xy=171,158
AudioMixer4              mixer1;         //xy=388,209
AudioOutputI2S           i2s1;           //xy=588,197
AudioAnalyzeFFT1024      fft1024_1;      //xy=588,276
AudioConnection          patchCord1(playMem1, 0, mixer1, 2);
AudioConnection          patchCord2(waveform1, 0, mixer1, 3);
AudioConnection          patchCord3(playSdWav1, 0, mixer1, 0);
AudioConnection          patchCord4(playSdWav1, 1, mixer1, 1);
AudioConnection          patchCord5(mixer1, 0, i2s1, 0);
AudioConnection          patchCord6(mixer1, 0, i2s1, 1);
AudioConnection          patchCord7(mixer1, fft1024_1);
AudioControlSGTL5000     sgtl5000_1;     //xy=462,361
// GUItool: end automatically generated code

// Use these with the Teensy Audio Shield
//#define SDCARD_CS_PIN    10
//#define SDCARD_MOSI_PIN  7
//#define SDCARD_SCK_PIN   14

// Use these with the Teensy 3.5 & 3.6 SD card
#define SDCARD_CS_PIN    BUILTIN_SDCARD
#define SDCARD_MOSI_PIN  11  // not actually used
#define SDCARD_SCK_PIN   13  // not actually used

// Use these for the SD+Wiz820 or other adaptors
//#define SDCARD_CS_PIN    4
//#define SDCARD_MOSI_PIN  11
//#define SDCARD_SCK_PIN   13

void setup() {
  Serial.begin(9600);
  leds.begin();
  AudioMemory(10);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.7);
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  mixer1.gain(0, 0.7);
  mixer1.gain(1, 0.7);
  mixer1.gain(2, 0.0);
  mixer1.gain(3, 0.0);
  delay(1000);
  playSdWav1.play("jc15.WAV");
  
}



float n;
float m;
float w;
float a; 
float b;
float c;
float d;
float e;
float f;
float g;
float h;
float i;
float j;
float k;
float l;
float o;
float p;
float q;
float r;
float s;
float t;
float u;
float v;
float x;
float y;
float mm;
float nn;
float ww;


void loop() {

  int microsec = 200000 / leds.numPixels(); //.1s

//   print Fourier Transform data to the Arduino Serial Monitor
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
      
      printNumber(ww);
      printNumber(c);
      printNumber(f);
      printNumber(i);
      printNumber(m);
      printNumber(q);
      printNumber(u);
      printNumber(y);

      Serial.println();
    }
    
    Bar1(microsec,ww);
    Bar2(microsec,c);
    Bar3(microsec,f);
    Bar4(microsec,i);
    Bar5(microsec,m);
    Bar6(microsec,q);
    Bar7(microsec,u);
    Bar8(microsec,y);

  

}

void Bar1(int wait, float n){

   if (n >= 0.00 and n <= 0.014) {
      leds.clear();
      delayMicroseconds(wait);
  }
  
  else if (n >= 0.015 and n <= 0.035) {
      leds.setPixel(0,GREEN);
      leds.show();
      delayMicroseconds(wait);
  }
  else if (n >= 0.036 and n <= 0.06) {
      leds.setPixel(0,GREEN);
      leds.setPixel(15,GREEN);
      leds.show();
      delayMicroseconds(wait);
  }
   else if (n >= 0.061 and n <= 0.08) {
      leds.setPixel(0,GREEN);
      leds.setPixel(15,GREEN);
      leds.setPixel(16,GREEN);
      leds.show();
      delayMicroseconds(wait);
  }
    else if (n >= 0.081 and n <= 0.1) {
      leds.setPixel(0,GREEN);
      leds.setPixel(15,GREEN);
      leds.setPixel(16,GREEN);
      leds.setPixel(31,GREEN);
      leds.show();
      delayMicroseconds(wait);
  } 
  else if (n >= 0.101 and n <= 0.12) {
      leds.setPixel(0,GREEN);
      leds.setPixel(15,GREEN);
      leds.setPixel(16,GREEN);
      leds.setPixel(31,GREEN);
      leds.setPixel(32,YELLOW);
      leds.show();
      delayMicroseconds(wait);
  } 
  else if (n >= 0.121 and n <= 0.13) {
      leds.setPixel(0,GREEN);
      leds.setPixel(15,GREEN);
      leds.setPixel(16,GREEN);
      leds.setPixel(31,GREEN);
      leds.setPixel(32,YELLOW);
      leds.setPixel(47,YELLOW);
      leds.show();
      delayMicroseconds(wait);
  } 

  else if (n >= 0.131 and n <= 0.14) {
      leds.setPixel(0,GREEN);
      leds.setPixel(15,GREEN);
      leds.setPixel(16,GREEN);
      leds.setPixel(31,GREEN);
      leds.setPixel(32,YELLOW);
      leds.setPixel(47,YELLOW);
      leds.setPixel(48,YELLOW);
      leds.show();
      delayMicroseconds(wait);
  } 
  else if (n >= 0.141) {
      leds.setPixel(0,GREEN);
      leds.setPixel(15,GREEN);
      leds.setPixel(16,GREEN);
      leds.setPixel(31,GREEN);
      leds.setPixel(32,YELLOW);
      leds.setPixel(47,YELLOW);
      leds.setPixel(48,YELLOW);
      leds.setPixel(63,RED);
      leds.show();
      delayMicroseconds(wait);
  } 
}

void Bar2(int wait, float n){

   if (n >= 0.00 and n <= 0.014) {
      leds.clear();
      delayMicroseconds(wait);
  }
  
  else if (n >= 0.015 and n <= 0.035) {
      leds.setPixel(1,GREEN);
      leds.show();
      delayMicroseconds(wait);
  }
  else if (n >= 0.036 and n <= 0.06) {
      leds.setPixel(1,GREEN);
      leds.setPixel(14,GREEN);
      leds.show();
      delayMicroseconds(wait);
  }
   else if (n >= 0.061 and n <= 0.08) {
      leds.setPixel(1,GREEN);
      leds.setPixel(14,GREEN);
      leds.setPixel(17,GREEN);
      leds.show();
      delayMicroseconds(wait);
  }
    else if (n >= 0.081 and n <= 0.1) {
      leds.setPixel(1,GREEN);
      leds.setPixel(14,GREEN);
      leds.setPixel(17,GREEN);
      leds.setPixel(30,GREEN);
      leds.show();
      delayMicroseconds(wait);
  } 
  else if (n >= 0.101 and n <= 0.13) {
      leds.setPixel(1,GREEN);
      leds.setPixel(14,GREEN);
      leds.setPixel(17,GREEN);
      leds.setPixel(30,GREEN);
      leds.setPixel(33,YELLOW);
      leds.show();
      delayMicroseconds(wait);
  } 
  else if (n >= 0.131 and n <= 0.15) {
      leds.setPixel(1,GREEN);
      leds.setPixel(14,GREEN);
      leds.setPixel(17,GREEN);
      leds.setPixel(30,GREEN);
      leds.setPixel(33,YELLOW);
      leds.setPixel(46,YELLOW);
      leds.show();
      delayMicroseconds(wait);
  } 

  else if (n >= 0.151 and n <= 0.17) {
      leds.setPixel(1,GREEN);
      leds.setPixel(14,GREEN);
      leds.setPixel(17,GREEN);
      leds.setPixel(30,GREEN);
      leds.setPixel(33,YELLOW);
      leds.setPixel(46,YELLOW);
      leds.setPixel(49,YELLOW);
      leds.show();
      delayMicroseconds(wait);
  } 
  else if (n >= 0.5) {
      leds.setPixel(1,GREEN);
      leds.setPixel(14,GREEN);
      leds.setPixel(17,GREEN);
      leds.setPixel(30,GREEN);
      leds.setPixel(33,YELLOW);
      leds.setPixel(46,YELLOW);
      leds.setPixel(49,YELLOW);
      leds.setPixel(62,RED);
      leds.show();
      delayMicroseconds(wait);
  } 
}

void Bar3(int wait, float n){

   if (n >= 0.00 and n <= 0.014) {
      leds.clear();
      delayMicroseconds(wait);
  }
  
  else if (n >= 0.015 and n <= 0.04) {
      leds.setPixel(2,GREEN);
      leds.show();
      delayMicroseconds(wait);
  }
  else if (n >= 0.041 and n <= 0.08) {
      leds.setPixel(2,GREEN);
      leds.setPixel(13,GREEN);
      leds.show();
      delayMicroseconds(wait);
  }
   else if (n >= 0.081 and n <= 0.11) {
      leds.setPixel(2,GREEN);
      leds.setPixel(13,GREEN);
      leds.setPixel(18,GREEN);
      leds.show();
      delayMicroseconds(wait);
  }
    else if (n >= 0.111 and n <= 0.14) {
      leds.setPixel(2,GREEN);
      leds.setPixel(13,GREEN);
      leds.setPixel(18,GREEN);
      leds.setPixel(29,GREEN);
      leds.show();
      delayMicroseconds(wait);
  } 
  else if (n >= 0.141 and n <= 0.17) {
      leds.setPixel(2,GREEN);
      leds.setPixel(13,GREEN);
      leds.setPixel(18,GREEN);
      leds.setPixel(29,GREEN);
      leds.setPixel(34,YELLOW);
      leds.show();
      delayMicroseconds(wait);
  } 
  else if (n >= 0.171 and n <= 0.25) {
      leds.setPixel(2,GREEN);
      leds.setPixel(13,GREEN);
      leds.setPixel(18,GREEN);
      leds.setPixel(29,GREEN);
      leds.setPixel(34,YELLOW);
      leds.setPixel(45,YELLOW);
      leds.show();
      delayMicroseconds(wait);
  } 

  else if (n >= 0.251 and n <= 0.5) {
      leds.setPixel(2,GREEN);
      leds.setPixel(13,GREEN);
      leds.setPixel(18,GREEN);
      leds.setPixel(29,GREEN);
      leds.setPixel(34,YELLOW);
      leds.setPixel(45,YELLOW);
      leds.setPixel(50,YELLOW);
      leds.show();
      delayMicroseconds(wait);
  } 
  else if (n >= 0.5) {
      leds.setPixel(2,GREEN);
      leds.setPixel(15,GREEN);
      leds.setPixel(18,GREEN);
      leds.setPixel(29,GREEN);
      leds.setPixel(34,YELLOW);
      leds.setPixel(45,YELLOW);
      leds.setPixel(50,YELLOW);
      leds.setPixel(61,RED);
      leds.show();
      delayMicroseconds(wait);
  } 
}

void Bar4(int wait, float n){

   if (n >= 0.00 and n <= 0.014) {
      leds.clear();
      delayMicroseconds(wait);
  }
  
  else if (n >= 0.015 and n <= 0.035) {
      leds.setPixel(3,GREEN);
      leds.show();
      delayMicroseconds(wait);
  }
  else if (n >= 0.036 and n <= 0.06) {
      leds.setPixel(3,GREEN);
      leds.setPixel(12,GREEN);
      leds.show();
      delayMicroseconds(wait);
  }
   else if (n >= 0.061 and n <= 0.08) {
      leds.setPixel(3,GREEN);
      leds.setPixel(12,GREEN);
      leds.setPixel(19,GREEN);
      leds.show();
      delayMicroseconds(wait);
  }
    else if (n >= 0.081 and n <= 0.12) {
      leds.setPixel(3,GREEN);
      leds.setPixel(12,GREEN);
      leds.setPixel(19,GREEN);
      leds.setPixel(28,GREEN);
      leds.show();
      delayMicroseconds(wait);
  } 
  else if (n >= 0.121 and n <= 0.150) {
      leds.setPixel(3,GREEN);
      leds.setPixel(12,GREEN);
      leds.setPixel(19,GREEN);
      leds.setPixel(28,GREEN);
      leds.setPixel(35,YELLOW);
      leds.show();
      delayMicroseconds(wait);
  } 
  else if (n >= 0.151 and n <= 0.17) {
      leds.setPixel(3,GREEN);
      leds.setPixel(12,GREEN);
      leds.setPixel(19,GREEN);
      leds.setPixel(28,GREEN);
      leds.setPixel(35,YELLOW);
      leds.setPixel(44,YELLOW);
      leds.show();
      delayMicroseconds(wait);
  } 

  else if (n >= 0.171 and n <= 0.19) {
      leds.setPixel(3,GREEN);
      leds.setPixel(12,GREEN);
      leds.setPixel(19,GREEN);
      leds.setPixel(28,GREEN);
      leds.setPixel(35,YELLOW);
      leds.setPixel(44,YELLOW);
      leds.setPixel(51,YELLOW);
      leds.show();
      delayMicroseconds(wait);
  } 
  else if (n >= 0.19) {
      leds.setPixel(3,GREEN);
      leds.setPixel(12,GREEN);
      leds.setPixel(19,GREEN);
      leds.setPixel(28,GREEN);
      leds.setPixel(35,YELLOW);
      leds.setPixel(44,YELLOW);
      leds.setPixel(51,YELLOW);
      leds.setPixel(60,RED);
      leds.show();
      delayMicroseconds(wait);
  } 
}

void Bar5(int wait, float n){

   if (n >= 0.00 and n <= 0.014) {
      leds.clear();
      delayMicroseconds(wait);
  }
  
  else if (n >= 0.015 and n <= 0.035) {
      leds.setPixel(4,GREEN);
      leds.show();
      delayMicroseconds(wait);
  }
  else if (n >= 0.036 and n <= 0.06) {
      leds.setPixel(4,GREEN);
      leds.setPixel(11,GREEN);
      leds.show();
      delayMicroseconds(wait);
  }
   else if (n >= 0.061 and n <= 0.08) {
      leds.setPixel(4,GREEN);
      leds.setPixel(11,GREEN);
      leds.setPixel(20,GREEN);
      leds.show();
      delayMicroseconds(wait);
  }
    else if (n >= 0.081 and n <= 0.1) {
      leds.setPixel(4,GREEN);
      leds.setPixel(11,GREEN);
      leds.setPixel(20,GREEN);
      leds.setPixel(27,GREEN);
      leds.show();
      delayMicroseconds(wait);
  } 
  else if (n >= 0.101 and n <= 0.12) {
      leds.setPixel(4,GREEN);
      leds.setPixel(11,GREEN);
      leds.setPixel(20,GREEN);
      leds.setPixel(27,GREEN);
      leds.setPixel(36,YELLOW);
      leds.show();
      delayMicroseconds(wait);
  } 
  else if (n >= 0.121 and n <= 0.13) {
      leds.setPixel(4,GREEN);
      leds.setPixel(11,GREEN);
      leds.setPixel(20,GREEN);
      leds.setPixel(27,GREEN);
      leds.setPixel(36,YELLOW);
      leds.setPixel(43,YELLOW);
      leds.show();
      delayMicroseconds(wait);
  } 

  else if (n >= 0.131 and n <= 0.14) {
      leds.setPixel(4,GREEN);
      leds.setPixel(11,GREEN);
      leds.setPixel(20,GREEN);
      leds.setPixel(27,GREEN);
      leds.setPixel(36,YELLOW);
      leds.setPixel(43,YELLOW);
      leds.setPixel(52,YELLOW);
      leds.show();
      delayMicroseconds(wait);
  } 
  else if (n >= 0.141) {
      leds.setPixel(4,GREEN);
      leds.setPixel(11,GREEN);
      leds.setPixel(20,GREEN);
      leds.setPixel(27,GREEN);
      leds.setPixel(36,YELLOW);
      leds.setPixel(43,YELLOW);
      leds.setPixel(52,YELLOW);
      leds.setPixel(59,RED);
      leds.show();
      delayMicroseconds(wait);
  } 
}

void Bar6(int wait, float n){

   if (n >= 0.00 and n <= 0.014) {
      leds.clear();
      delayMicroseconds(wait);
  }
  
  else if (n >= 0.016 and n <= 0.03) {
      leds.setPixel(5,GREEN);
      leds.show();
      delayMicroseconds(wait);
  }
  else if (n >= 0.031 and n <= 0.04) {
      leds.setPixel(5,GREEN);
      leds.setPixel(10,GREEN);
      leds.show();
      delayMicroseconds(wait);
  }
   else if (n >= 0.041 and n <= 0.050) {
      leds.setPixel(5,GREEN);
      leds.setPixel(10,GREEN);
      leds.setPixel(21,GREEN);
      leds.show();
      delayMicroseconds(wait);
  }
    else if (n >= 0.051 and n <= 0.06) {
      leds.setPixel(5,GREEN);
      leds.setPixel(10,GREEN);
      leds.setPixel(21,GREEN);
      leds.setPixel(28,GREEN);
      leds.show();
      delayMicroseconds(wait);
  } 
  else if (n >= 0.061 and n <= 0.1) {
      leds.setPixel(5,GREEN);
      leds.setPixel(10,GREEN);
      leds.setPixel(21,GREEN);
      leds.setPixel(26,GREEN);
      leds.setPixel(37,YELLOW);
      leds.show();
      delayMicroseconds(wait);
  } 
  else if (n >= 0.11 and n <= 0.15) {
      leds.setPixel(5,GREEN);
      leds.setPixel(10,GREEN);
      leds.setPixel(21,GREEN);
      leds.setPixel(26,GREEN);
      leds.setPixel(37,YELLOW);
      leds.setPixel(42,YELLOW);
      leds.show();
      delayMicroseconds(wait);
  } 

  else if (n >= 0.151 and n <= 0.201) {
      leds.setPixel(0,GREEN);
      leds.setPixel(15,GREEN);
      leds.setPixel(16,GREEN);
      leds.setPixel(26,GREEN);
      leds.setPixel(37,YELLOW);
      leds.setPixel(42,YELLOW);
      leds.setPixel(53,YELLOW);
      leds.show();
      delayMicroseconds(wait);
  } 
  else if (n >= 0.201) {
      leds.setPixel(5,GREEN);
      leds.setPixel(10,GREEN);
      leds.setPixel(21,GREEN);
      leds.setPixel(26,GREEN);
      leds.setPixel(37,YELLOW);
      leds.setPixel(42,YELLOW);
      leds.setPixel(53,YELLOW);
      leds.setPixel(58,RED);
      leds.show();
      delayMicroseconds(wait);
  } 
}

void Bar7(int wait, float n){

   if (n >= 0.00 and n <= 0.014) {
      leds.clear();
      delayMicroseconds(wait);
  }
  
  else if (n >= 0.015 and n <= 0.035) {
      leds.setPixel(6,GREEN);
      leds.show();
      delayMicroseconds(wait);
  }
  else if (n >= 0.036 and n <= 0.06) {
      leds.setPixel(6,GREEN);
      leds.setPixel(9,GREEN);
      leds.show();
      delayMicroseconds(wait);
  }
   else if (n >= 0.061 and n <= 0.09) {
      leds.setPixel(6,GREEN);
      leds.setPixel(9,GREEN);
      leds.setPixel(22,GREEN);
      leds.show();
      delayMicroseconds(wait);
  }
    else if (n >= 0.091 and n <= 0.12) {
      leds.setPixel(6,GREEN);
      leds.setPixel(9,GREEN);
      leds.setPixel(22,GREEN);
      leds.setPixel(25,GREEN);
      leds.show();
      delayMicroseconds(wait);
  } 
  else if (n >= 0.121 and n <= 0.15) {
      leds.setPixel(6,GREEN);
      leds.setPixel(9,GREEN);
      leds.setPixel(22,GREEN);
      leds.setPixel(25,GREEN);
      leds.setPixel(38,YELLOW);
      leds.show();
      delayMicroseconds(wait);
  } 
  else if (n >= 0.151 and n <= 0.18) {
      leds.setPixel(6,GREEN);
      leds.setPixel(9,GREEN);
      leds.setPixel(22,GREEN);
      leds.setPixel(25,GREEN);
      leds.setPixel(38,YELLOW);
      leds.setPixel(41,YELLOW);
      leds.show();
      delayMicroseconds(wait);
  } 

  else if (n >= 0.181 and n <= 0.210) {
      leds.setPixel(6,GREEN);
      leds.setPixel(9,GREEN);
      leds.setPixel(22,GREEN);
      leds.setPixel(25,GREEN);
      leds.setPixel(38,YELLOW);
      leds.setPixel(41,YELLOW);
      leds.setPixel(54,YELLOW);
      leds.show();
      delayMicroseconds(wait);
  } 
  else if (n >= 0.211) {
      leds.setPixel(6,GREEN);
      leds.setPixel(9,GREEN);
      leds.setPixel(22,GREEN);
      leds.setPixel(25,GREEN);
      leds.setPixel(38,YELLOW);
      leds.setPixel(41,YELLOW);
      leds.setPixel(54,YELLOW);
      leds.setPixel(57,RED);
      leds.show();
      delayMicroseconds(wait);
  } 
}

void Bar8(int wait, float n){

   if (n >= 0.00 and n <= 0.014) {
      leds.clear();
      delayMicroseconds(wait);
  }
  
  else if (n >= 0.015 and n <= 0.035) {
      leds.setPixel(7,GREEN);
      leds.show();
      delayMicroseconds(wait);
  }
  else if (n >= 0.036 and n <= 0.06) {
      leds.setPixel(7,GREEN);
      leds.setPixel(8,GREEN);
      leds.show();
      delayMicroseconds(wait);
  }
   else if (n >= 0.061 and n <= 0.08) {
      leds.setPixel(7,GREEN);
      leds.setPixel(8,GREEN);
      leds.setPixel(23,GREEN);
      leds.show();
      delayMicroseconds(wait);
  }
    else if (n >= 0.081 and n <= 0.11) {
      leds.setPixel(7,GREEN);
      leds.setPixel(8,GREEN);
      leds.setPixel(23,GREEN);
      leds.setPixel(24,GREEN);
      leds.show();
      delayMicroseconds(wait);
  } 
  else if (n >= 0.111 and n <= 0.14) {
      leds.setPixel(7,GREEN);
      leds.setPixel(8,GREEN);
      leds.setPixel(23,GREEN);
      leds.setPixel(24,GREEN);
      leds.setPixel(39,YELLOW);
      leds.show();
      delayMicroseconds(wait);
  } 
  else if (n >= 0.141 and n <= 0.17) {
      leds.setPixel(7,GREEN);
      leds.setPixel(8,GREEN);
      leds.setPixel(23,GREEN);
      leds.setPixel(24,GREEN);
      leds.setPixel(39,YELLOW);
      leds.setPixel(40,YELLOW);
      leds.show();
      delayMicroseconds(wait);
  } 

  else if (n >= 0.171 and n <= 0.210) {
      leds.setPixel(7,GREEN);
      leds.setPixel(8,GREEN);
      leds.setPixel(23,GREEN);
      leds.setPixel(24,GREEN);
      leds.setPixel(39,YELLOW);
      leds.setPixel(40,YELLOW);
      leds.setPixel(55,YELLOW);
      leds.show();
      delayMicroseconds(wait);
  } 
  else if (n >= 0.211) {
      leds.setPixel(7,GREEN);
      leds.setPixel(8,GREEN);
      leds.setPixel(23,GREEN);
      leds.setPixel(24,GREEN);
      leds.setPixel(39,YELLOW);
      leds.setPixel(40,YELLOW);
      leds.setPixel(55,YELLOW);
      leds.setPixel(56,RED);
      leds.show();
      delayMicroseconds(wait);
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


  //1: 1,2
  //2: 3,4
  //3: 5,6
  //4: 7,8
  //5: 9, 10 , 11
  //6: 12, 13, 14
  //7: 15, 16, 17
  //8: 18, 19, 20
