// Arduino Pulse Induction Metal Detector
// Original code by George Overton
// Modifications to support LCD and menuing by James Hughes

//=================================
//Includes
//=================================
#include "config.h"
#ifdef LCD
#include <Adafruit_RGBLCDShield.h>
#include <Adafruit_MCP23017.h>
#endif


//=================================
//Defines
//=================================
#define MS * 1E-3
#define US * 1E-6
#define NS * 1E-9

//=================================
//Global Instances
//=================================
#ifdef LCD
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
#endif
bool boost = false;

//=================================
// Pin assignments
//=================================
byte txPin = 8;          // Assign TX output
byte mainSamplePin = 9;  // Assign main sample pulse
byte efeSamplePin = 10;  // Assign EFE sample pulse
byte audioPin = 11;      // Assign audio chopper
byte boostPin = 12;      // Assign boost switch
byte delayPin = A0;      // Assign delay pot
byte batteryPin = A1;    // Assign battery monitor
byte signalPin = A3;     // Assign signal monitor

//=================================
// Program constants
//=================================
const float normalPower = 50 US;       // Normal TX-on time (50us)
const float boostPower = 100 US;       // Boost TX-on time (100us)
const float clockCycle = 62.5 NS;      // Time for one clock cycle (1/16MHz)
const unsigned long maxCount = 65535;  // Value of 2^16 - 1
const byte readDelayLimit = 100;       // Wait 100 TX periods (100ms) before reading delay pot
const byte mosfetOn = HIGH;            // Mosfet turns on when transmitter input high
const byte mosfetOff = LOW;            // Mosfet turns off when transmitter input low
const byte syncDemodOn = LOW;          // Sample gate turns on when input high
const byte syncDemodOff = HIGH;        // Sample gate turns off when input low

//=================================
// Detector timings
//=================================
float txOn = normalPower;        // TX-on time using normal power mode
float defMainDelay = 10 US;      // Default main sample delay (20us)
float mainDelay = defMainDelay;  // Main sample pulse delay
float mainSample = 50 US;        // Main sample pulse width (50us)
float efeDelay = 240 US;         // EFE sample pulse delay (240us)
float efeSample = mainSample;    // EFE sample pulse width (same as main sample)
float txPeriod = 1 MS;           // TX period (1ms)

//=================================
// Timing offsets
//=================================
float txOnOffset = 3 US;         // TX-on pulse offset (3us)
float mainDelayOffset = 4.2 US;  // Main delay pulse offset (4.2us)
float mainSampleOffset = 3 US;   // Main sample pulse offset (3us)
float efeDelayOffset = 12 US;    // EFE delay pulse offset (12us)
float efeSampleOffset = 4 US;    // EFE sample pulse offset (4us)
float txPeriodOffset = 30 US;    // TX period offset (30us)

//=================================
// Program variables
//=================================
float temp1, temp2, temp3, temp4, temp5, temp6;  // Intermediate calculation variables
word txOnCount;                                  // TX pulse
word mainDelayCount;                             // Main sample delay
word mainSampleCount;                            // Main sample pulse
word efeDelayCount;                              // EFE sample delay
word efeSampleCount;                             // EFE sample pulse
word txPeriodCount;                              // TX period
word delayVal = 0;                               // Delay pot value
bool readDelayPot = false;                       // Delay pot read (true or false)
byte audioLevel = LOW;                           // Value for audio chopper signal
byte intState = 0;                               // Interrupt state machine
byte readDelayCounter = 0;                       // Read delay pot counter

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
  if (bgValue >= 1023 || bgValue < 0)
  {
    dir = -dir;
  }
  bgValue += dir * 24;
}
