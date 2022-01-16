// Arduino Pulse Induction Metal Detector
// Original code by George Overton
// Modifications to support LCD and menuing by James Hughes

//=================================
//Includes
//=================================
#include "config.h"
#include <Adafruit_RGBLCDShield.h>
#include <Adafruit_MCP23017.h>

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
  Serial.println("Arduino PI");

  setIO();
  setInterruptDetails();
  debugln("setup completed");

#ifdef LCD
  LCDInit();
  LCDTitle();
  createCustomChar();
  checkBattery();
#endif
}

//=================================
//Loop
//=================================
void loop() 
{
  DelayPot();
  LCDBar();
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

void DelayPot( void)
{
  if (readDelayPot == true)
  {
    delayVal = analogRead(delayPin);                   // Read the delay pot
    mainDelay = defMainDelay + delayVal * clockCycle;  // Offset main sample delay
    calcTimerValues();                                 // Calculate new timer values
    readDelayPot = false;                              // Set read delay pot flag to false
  }
}
