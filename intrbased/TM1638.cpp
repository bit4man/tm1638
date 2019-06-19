/*
   Use the tm1638
*/

#include "TM1638.h"
#include "Arduino.h"

void TM1638::setWriteMode() {
  pinMode(dataPin, OUTPUT);
  unitMode = WRITE_MODE;
}

void TM1638::setReadMode() {
  pinMode(dataPin, INPUT_PULLUP);
  unitMode = READ_MODE;
}

void TM1638::sendByte(byte b) {
  shiftOut(dataPin, clockPin, LSBFIRST, b);
}

void TM1638::sendCommand(byte cmd) {
  if (unitMode == READ_MODE) setWriteMode();

  digitalWrite(strobePin, LOW); //set the strobe low so it'll accept instruction
  sendByte(cmd);
  digitalWrite(strobePin, HIGH); //set the strobe low so it'll accept instruction
}


void TM1638::initUnit(byte brightness) {
  byte cmd = 0x88 | (brightness & 0x07);
  sendCommand(cmd);
  Serial.println("initUnit");
}

void TM1638::offUnit() {
  sendCommand(0x80);
}

void TM1638::singleAddressMode() {
  sendCommand(0x44);
}

void TM1638::startAddressMode() {
  sendCommand(0x40);
}


void TM1638::updateDisplay() {
  static volatile bool inUpdate=false;
  if (inUpdate) return;
  inUpdate=true;
  if (dirty) {
    dirty = false;
    startAddressMode();
    digitalWrite(strobePin, LOW); //set the strobe low so it'll accept instruction
    sendByte(0x00); // Start address
    for (int i = 0; i < 16; i++) {
      sendByte(ledOut[i]);
    }
    digitalWrite(strobePin, HIGH); //set the strobe low so it'll accept instruction
  }
  readButtons();
  inUpdate=false;
}

bool TM1638::timerUpdate(void *) {
  static volatile bool inUpdate = false;
  if (inUpdate) return true;
  inUpdate=true;
  updateDisplay();
  inUpdate=false;
  return true;
}

void TM1638::readButtons() {
  byte status[4];
  digitalWrite(strobePin, LOW); //set the strobe low so it'll accept instruction
  sendByte(0x42);
  setReadMode();
  for (int i = 0; i < 4; i++) {
    status[i] = shiftIn(dataPin, clockPin, LSBFIRST);
  }
  digitalWrite(strobePin, HIGH); //set the strobe low so it'll accept instruction
  setWriteMode();
  for (int i = 0; i < 4; i++) {
    keyStatus[i + 4] = false;
    if ((status[i] & 0x10) > 0) keyStatus[i + 4] = true;
    keyStatus[i] = false;
    if ((status[i] & 0x01) > 0) keyStatus[i] = true;
  }
}

void TM1638::sendAdrValue(byte adr, byte val) {
  digitalWrite(strobePin, LOW); //set the strobe low so it'll accept instruction
  sendByte(0xC0 | (adr & 0x0f)); // Last 4 bits is address
  sendByte(val);
  digitalWrite(strobePin, HIGH); //set the strobe low so it'll accept instruction
}

void TM1638::blank() {
  for (int i = 0; i < 16; i++) {
    ledOut[i] = 0x00; //blank/off
  }
  dirty = true;
}

void TM1638::init(int _strobePin, int _clockPin, int _dataPin) {
  strobePin = _strobePin;
  clockPin = _clockPin;
  dataPin = _dataPin;
  dirty = true;
  intensity = 0x01;

  setWriteMode();
  pinMode(strobePin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  // Setup device
  initUnit(intensity);
  for (int i=0; i<8; i++) keyStatus[i] = false;

  blank();
  updateDisplay();
  // TODO - Setup timer interrupt to update unit
}

TM1638::TM1638(int _strobePin, int _clockPin, int _dataPin) {
  init(_strobePin, _clockPin, _dataPin);
}

void TM1638::displayNum(byte display, int value) {
  byte start = 0;
  if (display == 1) start = 4;
  if (value > 9999) value = 9999;
  bool isZero = true;
  for (int index = 1000; index > 0; index /= 10) {
    int d = (value / index);
    if (d > 0) isZero = false;
    ledOut[digits[start++]] = (isZero && (d==0))?0x00:values[d];
    value -= d * index;
  }
  dirty=true;
}

void TM1638::displayHex(byte display, int value) {
  byte start = 0;
  if (display == 1) start = 4;
  value &= 0xffff;
  int mask=0xf000;
  bool isZero = true;
  for (int index=4; index > 0; index--) {
    int d = ((value & mask) >> ((index-1)*4)) & 0x0f;
    if (d > 0) isZero = false;
    ledOut[digits[start++]] = (isZero && (d==0))?0x00:values[d];
    mask >>= 4;
  }
  dirty=true;
}

void TM1638::setLed(byte led, bool state) {
  if (led <= 8) {
    ledOut[leds[led-1]] = (state==true);
    dirty = true;  
  }
}

void TM1638::blankLed() {
  for (int i=0; i<8; i++) ledOut[leds[i]] = 0;
}

bool TM1638::isKeyPressed(int key) {
  return keyStatus[key-1];
}
