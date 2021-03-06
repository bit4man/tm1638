const int strobePin = 5;
const int clockPin = 6;
const int dataPin = 7;


void setup()
{
  pinMode(strobePin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  // set up our pins as OUTPUTs
  digitalWrite(strobePin, LOW); //set the strobe low so it'll accept instruction
  shiftOut(dataPin, clockPin, LSBFIRST, 0x8F);  // send the instruction to activate the board and set brightness to max
  digitalWrite(strobePin, HIGH); //we will always set the strobe high after the completion of each instruction and data set
  
  // ***sequential addressing example
  digitalWrite(strobePin, LOW); //strobe goes low to load an instruction
  shiftOut(dataPin, clockPin, LSBFIRST, 0x40); // set up sequential addressing mode
  digitalWrite(strobePin, HIGH); // end of instruction, strobe must go high
  digitalWrite(strobePin, LOW); // strobe goes low to receive the data
  shiftOut(dataPin, clockPin, LSBFIRST, 0xC0);  // sets up the first address
  shiftOut(dataPin, clockPin, LSBFIRST, 0x77);  // sets up a 7 segment "A"
  shiftOut(dataPin, clockPin, LSBFIRST, 0x00);  // turns off discrete LED
  shiftOut(dataPin, clockPin, LSBFIRST, 0x77);  // sets up a 7 segment "A"
  shiftOut(dataPin, clockPin, LSBFIRST, 0x01);  // turns on discrete LED

  digitalWrite(strobePin, HIGH); // end of data, strobe goes high

}

void loop() {
  // put your main code here, to run repeatedly:

}
