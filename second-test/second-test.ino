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
  
  // ***single addressing example
  digitalWrite(strobePin, LOW); //set the strobe low so it'll accept instruction
  shiftOut(dataPin, clockPin, LSBFIRST, 0x44); //set to single address mode
  digitalWrite(strobePin, HIGH); //the mode is now set, we must set the strobe back to high
  digitalWrite(strobePin, LOW); //set the strobe low so it'll accept data
  shiftOut(dataPin, clockPin, LSBFIRST, 0xC6);  // set the address to 6 which is 7 segment display #4
  shiftOut(dataPin, clockPin, LSBFIRST, 0x4F);  // sends a figure "A" displayed on 7 segment to address 6
  digitalWrite(strobePin, HIGH); //last data sent, set the strobe line HIGH
}

void loop() {
  // put your main code here, to run repeatedly:

}
