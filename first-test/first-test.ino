
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
  shiftOut(dataPin, clockPin, LSBFIRST, 0x80);  // send the instruction to activate the board and set brightness to max
  digitalWrite(strobePin, HIGH); //we will always set the strobe high after the completion of each instruction and data set
}

void loop() {
  // put your main code here, to run repeatedly:

}
