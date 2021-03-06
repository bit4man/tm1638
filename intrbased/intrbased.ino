/*
   Use the tm1638
*/

#include "TM1638.h"
#include <timer.h>

TM1638 board(5, 6, 7);
Timer<3> timer;

volatile byte led = 1;
volatile int cnt = 0;
volatile bool doCount=true;
volatile bool doLed=true;

volatile int st = HIGH;
bool blink(void *) {
  digitalWrite(LED_BUILTIN, st);
  st = (st == HIGH) ? LOW : HIGH;
  return true;
}

bool timerUpdate(void *) {
  static volatile bool inUpdate = false;
  if (inUpdate) return true;
  inUpdate = true;
  board.updateDisplay();
  inUpdate = false;
  return true;
}

int state=0;

bool nextLed(void *) {
  switch (state) {
    case 0:
        board.setLed(led, false);
        if (++led > 8) {
          led=8;
          state++;
        }
        board.setLed(led, true);
        break;
    case 1:
        board.setLed(led, false);
        if (--led == 0) {
          led = 1;
          state++;
        }
        board.setLed(led, true);
        break;
    case 2:
        board.setLed(led, true);
        if (++led > 8) {
          state++;
        }
        break;
    case 3:
        board.setLed(led, false);
        if (--led == 1) {
          state = 0;
        }
        break;
    default: state=0;     
  }
  if (state > 3) state = 0;
  return doLed;
}

bool count(void *) {
  if (cnt > 9999) cnt = 0;
  board.displayNum(0, cnt);
  board.displayHex(1, cnt++);
//  if (random(20) < 5) board.displayNum(1, random(999));
  return doCount;
}

void startLed() {
  doLed=true;
  board.blankLed();
  state=0;
  timer.every(877, nextLed);
}

void startCount() {
  doCount=true; 
  timer.every(255, count);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  // board.setLed(2, true);
  timer.every(250, blink);
  startLed();
  startCount();
}


void loop() {
  board.updateDisplay();
  if (board.isKeyPressed(1)) {
    doCount=false;
  }
  if (board.isKeyPressed(2)) startCount();
  if (board.isKeyPressed(3)) { 
    doLed=false;
  }
  if (board.isKeyPressed(4)) startLed();
  timer.tick();
  delayMicroseconds(5);
}
