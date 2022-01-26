//Configuration and compile settings
#define NAME "Searcher"
#define VERSION "v1.1 - 2022"
#define LCD
#define CALIBRATE_OFFSETS

#define DEBUG 1

#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif

//=================================
//Defines
//=================================
#define MS * 1E-3
#define US * 1E-6
#define NS * 1E-9

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
const float normalPower = 45 US;                    // Normal TX-on time
const float boostPower = 100 US;                    // Boost TX-on time
const float clockCycle = 62.5 NS;                   // Time for one clock cycle (1/16MHz)
const unsigned long maxCount = 65535;               // Value of 2^16 - 1
const byte readDelayLimit = 100;                    // Wait 100 TX periods (100ms) before reading delay pot
const byte mosfetOn = HIGH;                         // Mosfet turns on when transmitter input high
const byte mosfetOff = LOW;                         // Mosfet turns off when transmitter input low
const byte syncDemodOn = LOW;                       // Sample gate turns on when input high
const byte syncDemodOff = HIGH;                     // Sample gate turns off when input low
const unsigned long debounceDelayCountMax = 65535;  //Software debounce delay for switch read

//=================================
// Detector timings
//=================================
float txOn = normalPower;        // TX-on time using normal power mode
float defMainDelay = 10 US;      // Default main sample delay
float mainDelay = defMainDelay;  // Main sample pulse delay
float mainSample = 50 US;        // Main sample pulse width
float efeDelay = 240 US;         // EFE sample pulse delay
float efeSample = mainSample;    // EFE sample pulse width (same as main sample)
float txPeriod = 1 MS;           // TX period

//=================================
// Detector timings - menu choices
//=================================
float coilPulseWidthArray[4] = {20 US, 30 US, 40 US, 60 US};
float targetSampleWidthArray[3] = {15 US, 30 US, 45 US};
int coilSamplePulseIndex = 0;
int targetPulseIndex = 0;
int coilPulseMaxChoice = 4;
int targetSampleMaxChoice = 3;

#ifndef CALIBRATE_OFFSETS
//=================================
// Timing offsets
//=================================
float txOnOffset = 3 US;         // TX-on pulse offset
float mainDelayOffset = 4.2 US;  // Main delay pulse offset
float mainSampleOffset = 3 US;   // Main sample pulse offset
float efeDelayOffset = 12 US;    // EFE delay pulse offset
float efeSampleOffset = 4 US;    // EFE sample pulse offset
float txPeriodOffset = 30 US;    // TX period offset


#else
//=================================
// Timing offsets
//=================================
float txOnOffset = 3 US;         // TX-on pulse offset
float mainDelayOffset = 4.2 US;  // Main delay pulse offset
float mainSampleOffset = 3 US;   // Main sample pulse offset
float efeDelayOffset = 12 US;    // EFE delay pulse offset
float efeSampleOffset = 4 US;    // EFE sample pulse offset
float txPeriodOffset = 30 US;    // TX period offset
#endif

//=================================
// Program variables
//=================================
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

bool boost = false;
