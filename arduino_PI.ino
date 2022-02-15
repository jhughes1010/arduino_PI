// Arduino Pulse Induction Metal Detector
// Original code by George Overton
// Modifications to support LCD and menuing by James Hughes

//01-18-2022    1. VBAT monitor and low battery warning on power up. Detector will not operate
//              2. LCD support started
//              3. Button for scrolling through coil pulse width

//=================================
//Includes
//=================================
#include "config.h"
#include <Adafruit_RGBLCDShield.h>
#include <Adafruit_MCP23X17.h>

//=================================
//Global Instances
//=================================
#ifdef LCD
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
#endif

//=================================
//setup()
//=================================
void setup() {
  Serial.begin(115200);
  Serial.println(NAME);

  setIO();
  setInterruptDetails();
  debugln("setup completed");

#ifdef LCD
  LCDInit();
  LCDTitle();
  createCustomChar();
  //checkBattery();
#endif
}

//=================================
//Loop
//=================================
void loop()
{
  if (readDelayPot == true)
  {
    readDelayPot == false;
    DelayPotRead();
    //buttonRead();
    //LCDBar();
  }
}


void setIO(void)
{
  pinMode(txPin, OUTPUT);           // Set TX pin to output mode
  pinMode(mainSamplePin, OUTPUT);   // Set main sample pin to output mode
  pinMode(efeSamplePin, OUTPUT);    // Set EFE sample pin to output mode
  pinMode(boostPin, INPUT_PULLUP);  // Set Boost switch pin to input mode with pullup resistor
  pinMode(audioPin, OUTPUT);

  //LED and A1 defined
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A1, INPUT);
  //Pulse LED for 1000ms to indicate boot
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
}

void DelayPotRead( void)
{
  if (readDelayPot == true)
  {
    //debug(".");
    delayVal = analogRead(delayPin);                   // Read the delay pot
    //delayVal=0;
    mainDelay = defMainDelay + delayVal * clockCycle;  // Offset main sample delay
    calcTimerValues();                                 // Calculate new timer values
    //readDelayPot = false;                              // Set read delay pot flag to false
  }
}
