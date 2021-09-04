//Arduino PI detector
//Transcribed by James Hughes

//Pin Assignments
byte txPin = 8;
byte mainSamplePin = 9;
byte efeSamplePin = 10;
byte audioPin = 11;
byte boostPin = 12;
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




void setup()
{
  pinMode(txPin, OUTPUT);
  pinMode(mainSamplePin, OUTPUT);
  pinMode(efeSamplePin, OUTPUT);
  pinMode(boostPin, INPUT_PULLUP);
  calcTimerValues();
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK0 = 0;
  TCNT1 = txOnCount;
  TCCR1B |= (1 << CS10);
  TIMSK1 |= (1 << TOIE1);
  interrupts();
  analogWrite(audioPin, 127);
}

void loop()
{
  if (readDelayPot == true)
  {
    delayVal = analogRead(delayPin);
    mainDelay = defMainDelay + delayVal * clockCycle;
    calcTimerValues();
    readDelayPot = false;
  }
}

//================================
//Subroutines
//================================
void calcTimerValues()
{
  if (digitalRead(boostPin) == HIGH)
  {
    txOn = normalPower;
  }
  else
  {
    txOn = boostPower;
  }
  temp1 = (txOn - txOnOffset) / clockCycle;
  txOnCount = maxCount - int(temp1);
  temp2 = (mainDelay - mainDelayOffset) / clockCycle;
  mainDelayCount = maxCount - int(temp2);
  temp3 = (mainSample - mainSampleOffset) / clockCycle;
  mainSampleCount = maxCount + int(temp3);
  temp4 = (efeDelay - efeDelayOffset) / clockCycle;
  temp4 -= temp3 + temp2;
  efeDelayCount = maxCount - int(temp4);
  temp5 = (efeSample - efeSampleOffset) / clockCycle;
  efeSampleCount = maxCount - int(temp5);
  temp6 = (txPeriod - txPeriodOffset) / clockCycle;
  temp6 -= temp1 + temp2 + temp3 + temp4 + temp5;
  txPeriodCount = maxCount - int(temp6);
}

//================================
//ISR
//================================
switch (intState)
{
case 0:
  TCNT1 = txOnCount;
  digitalWrite(txPin, mosfetOn);
  intState = 1;
  break;
}
