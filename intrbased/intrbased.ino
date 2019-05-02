/*
 * Use the tm1638
 */

#include "TM1638.h"

TM1638 board;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN,OUTPUT);
  board.setLed(2, true);  
}

bool st = false;
void loop() {
  // board.updateDisplay();
  delay(400);
  digitalWrite(LED_BUILTIN,st);
  st=!st;
}

/*
 * void loop() {
  static byte val=0, led=0;
  static bool count = false;
  singleAddressMode();
  if (count) {
    displayNum(0, val++);
    if (random(50) > 25) {
       displayNum(1, random(0,9999));
    }
  } else
    displayNum((int)random(0,2), random(0,9999));
  if (led > 0x07) resetLed(led&0x07); else setLed(led);
  led = ++led & 0x0f;
  readButtons();
  for (int i=0;i<8;i++) {
    if (keyPushed[i]) {
      Serial.print("Key ");
      Serial.print(i);
      Serial.println(" pushed.");
    }
  }
  if (keyPushed[0]) {
     if (count) {
       count=false; 
     } else {
       count=true;
       val=0;
     }
  }
  delay(400);
}
*/
 
