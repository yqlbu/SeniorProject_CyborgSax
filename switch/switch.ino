#define PIN_SWITCH_1  35
#define PIN_SWITCH_2  36
#define PIN_SWITCH_3  37

int value = 0; 

void setup()
{
     // Set switch pins to input mode.
     pinMode(PIN_SWITCH_1, INPUT_PULLUP); 
     pinMode(PIN_SWITCH_2, INPUT_PULLUP); 
     pinMode(PIN_SWITCH_3, INPUT_PULLUP); 
     

     Serial.begin (9600);
 }

 void loop()
 {

     Serial.print (digitalRead(PIN_SWITCH_1));
     Serial.print (digitalRead(PIN_SWITCH_2));
     Serial.print (digitalRead(PIN_SWITCH_3));
     Serial.println();
 }
