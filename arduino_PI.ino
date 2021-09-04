//Arduino PI detector
//Transcribed by James Hughes

//Pin Assignments
byte txPin = 8;
byte mainSamplePin = 9;
byte audioPin = 10;
byte boostPin = 11;
byte delayPin = A0;


//Program Constants
const float normalPower = 50e-6;
const float boostPower = 100e-6;
const float clockCycle = 62.5e-9;
const unsigned long maxCount = 65535;
const byte readDelayLimit = 100;
const byte mosfetOn = HIGH;
const byte mosfetOff = LOW;
const byte syncDemodOn = LOW;
const byte syncDemodOff = HIGH;


//Detector timings
float txOn = normalPower;
float defMainDelay = normalPower;
float mainDelay = defMainDelay;
float mainSample = 50e-6;
float efeDelay = 240e-6;
float efeSample = mainSample;
float txPeriod = 15e-3;


//Timing offsets


//Program variables




void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
