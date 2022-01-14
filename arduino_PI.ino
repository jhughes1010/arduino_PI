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


  //set timer for coil timing waveforms
  calcTimerValues();                // Calculate all timer values
  noInterrupts();                   // Disable interrupts
  TCCR1A = 0;                       // Initialize Timer1 registers
  TCCR1B = 0;
  TIMSK0 = 0;                       // Clear Timer0 mask register to eliminate jitter
  TCNT1 = txOnCount;                // Load Timer1 with TX-on count
  TCCR1B |= (1 << CS10);            // No prescaling for Timer1
  TIMSK1 |= (1 << TOIE1);           // Enable Timer1 overflow interrupt
  interrupts();                     // Enable interrupts

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
void loop() {
  static int bgValue = 0;
  static int dir = 1;
  if (readDelayPot == true)
  {
    delayVal = analogRead(delayPin);                   // Read the delay pot
    mainDelay = defMainDelay + delayVal * clockCycle;  // Offset main sample delay
    calcTimerValues();                                 // Calculate new timer values
    readDelayPot = false;                              // Set read delay pot flag to false
  }

  //See about buttons
  uint8_t selectB = selectPressed();
  if (selectB)
  {
    lcd.clear();
    boost = !boost;
    LCDBoost();
    debugln(millis());
  }
  LCDBar ( bgValue);
}
