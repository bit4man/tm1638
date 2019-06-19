/*
   Use the tm1638
*/

#ifndef TM1638_h
#define TM1638_h

#include "Arduino.h"
#include <timer.h>

class TM1638
{
  public:
    TM1638(int _strobePin, int _clockPin, int _dataPin);

    void displayNum(byte display, int value);
    void displayHex(byte display, int value);
    void setLed(byte led, bool state);
    void updateDisplay();
    bool timerUpdate(void *);
    bool isKeyPressed(int key);

    void blank();
    void blankLed();
    
  private:
    void init(int _strobePin, int _clockPin, int _dataPin);

    enum mode_t { WRITE_MODE, READ_MODE } unitMode;

    int strobePin, clockPin, dataPin;

    byte intensity;
    volatile byte ledOut[16]; // All 16 addressable LED/7Segment units
    bool dirty;

    const byte values[17] = {
        0x3F  // 0
      , 0x06  // 1
      , 0x5B  // 2
      , 0x4f  // 3
      , 0x66  // 4
      , 0x6D  // 5
      , 0x7D  // 6
      , 0x07  // 7
      , 0x7f  // 8
      , 0x6f  // 9
      , 0x77  // A
      , 0x7C  // B
      , 0x58  // C
      , 0x5E  // D
      , 0x79  // E
      , 0x71  // F
      , 0x00  // BLANK
    };

    const byte digits[8] = {
      0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E
    };

    const byte leds[8] = {
        0x01
      , 0x03
      , 0x05
      , 0x07
      , 0x09
      , 0x0B
      , 0x0D
      , 0x0F
    };

    volatile bool keyStatus[8];

    void setWriteMode();

    void setReadMode();

    void sendByte(byte b);

    void sendCommand(byte cmd);

    void initUnit(byte brightness);

    void offUnit();

    void singleAddressMode();

    void startAddressMode();

    void readButtons();

    void sendAdrValue(byte adr, byte val);

};

#endif
