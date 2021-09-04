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
float txOnOffset = 3e-6;
float mainDelayOffset = 4.2e-6;
float mainSampleOffset = 3e-6;
float efeDelayOffset = 12e-6;
float efeSampleOffset = 4e-6;
float txPeriodOffset = 30e-6;



//Program variables
float temp1, temp2, temp3, temp4, temp5, temp6;
word txOnCount;
word mainDelayCount;
word mainSampleCount;
word efeDelayCount;
word efeSampleCount;
word txPeriodCount;
word delayVal = 0;
bool readDelayPot = false;
byte initState = 0;
byte readDelayCounter = 0;




void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
