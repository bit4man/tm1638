/*
 * Use the tm1638
 */

const int strobePin = 5;
const int clockPin = 6;
const int dataPin = 7;

byte intensity=0x03;

enum mode_t { WRITE_MODE, READ_MODE } unitMode;

const byte digits[] = {
  0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E
};

const byte values[] = {
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
  , 0x39  // C
  , 0x5E  // D
  , 0x79  // E 
  , 0x71  // F
  , 0x00  // BLANK
};

const byte leds[] = {
    0x01
  , 0x03
  , 0x05
  , 0x07
  , 0x09
  , 0x0B
  , 0x0D
  , 0x0F };          

bool keyPushed[8];

void setWriteMode() {
  pinMode(dataPin, OUTPUT);
  unitMode = WRITE_MODE;
}

void setReadMode() {
  pinMode(dataPin, INPUT_PULLUP);
  unitMode = READ_MODE;
}

void initUnit(byte brightness);  // Forward
void blank();

void setup() {
  Serial.begin(115200);
  setWriteMode();
  pinMode(strobePin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  initUnit(intensity);
  for (int i=0;i<8;i++) keyPushed[i]=false;
  blank();
  delay(1000);
}

void sendByte(byte b) {
  shiftOut(dataPin, clockPin, LSBFIRST, b);
}

void sendCommand(byte cmd) {
  if (unitMode == READ_MODE) setWriteMode();

  digitalWrite(strobePin, LOW); //set the strobe low so it'll accept instruction
  sendByte(cmd);
  digitalWrite(strobePin, HIGH); //set the strobe low so it'll accept instruction
}


void initUnit(byte brightness) {
  byte cmd = 0x88 | (brightness & 0x07);
  sendCommand(cmd);
}

void offUnit() {
  sendCommand(0x80);
}

void singleAddressMode() {
  sendCommand(0x44);
}

void startAddressMode(byte adr) {
  sendCommand(0x40);
}

void readButtons() {
  byte status[4];
  digitalWrite(strobePin, LOW); //set the strobe low so it'll accept instruction
  sendByte(0x42);
  setReadMode();
  for (int i=0;i<4;i++) {
     status[i] = shiftIn(dataPin, clockPin, LSBFIRST);
  }
  setWriteMode();
  digitalWrite(strobePin, HIGH); //set the strobe low so it'll accept instruction  
  for (int i=0;i<4;i++) {
    keyPushed[i+4] = false;
    if ((status[i] & 0x10) > 0) keyPushed[i+4]=true;
    keyPushed[i] = false;
    if ((status[i] & 0x01) > 0) keyPushed[i]=true;
  }
}

void sendAdrValue(byte adr, byte val) {
  digitalWrite(strobePin, LOW); //set the strobe low so it'll accept instruction
  sendByte(0xC0 | (adr & 0x0f)); // Last 4 bits is address
  sendByte(val);
  digitalWrite(strobePin, HIGH); //set the strobe low so it'll accept instruction  
}

void blank() {
  for (int i=0;i<8;i++) {
    sendAdrValue(0xC0+(digits[i]&0x0f),0x00); //blank
  }
}

void setLed(byte led) {
  digitalWrite(strobePin, LOW); //set the strobe low so it'll accept instruction
  sendByte(0xC0 | (leds[led] & 0x0f)); // Last 4 bits is address
  sendByte(0xff);
  digitalWrite(strobePin, HIGH); //set the strobe low so it'll accept instruction    
}

void resetLed(byte led) {
  digitalWrite(strobePin, LOW); //set the strobe low so it'll accept instruction
  sendByte(0xC0 | (leds[led] & 0x0f)); // Last 4 bits is address
  sendByte(0x00);
  digitalWrite(strobePin, HIGH); //set the strobe low so it'll accept instruction    
}

void displayNum(byte display, int value) {
  byte start=0; 
  if (display==1) start=4;
  if (value>9999) value=9999;
  singleAddressMode();
  bool isZero = true;
  for (int index=1000; index>0; index/=10) {
    int d = (value/index);
    if (d>0) isZero=false; 
    if (isZero && (d==0)) {
       sendAdrValue(digits[start++],0x00); // Blank 
    } else {
       sendAdrValue(digits[start++],values[d]);
    }
    value-=d*index;
  }
}


void loop() {
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
