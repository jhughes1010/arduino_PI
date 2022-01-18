//=================================
//setInterruptDetails()
//=================================
void setInterruptDetails(void)
{
  calcTimerValues();                // Calculate all timer values
  noInterrupts();                   // Disable interrupts
  TCCR1A = 0;                       // Initialize Timer1 registers
  TCCR1B = 0;
  TIMSK0 = 0;                       // Clear Timer0 mask register to eliminate jitter
  TCNT1 = txOnCount;                // Load Timer1 with TX-on count
  TCCR1B |= (1 << CS10);            // No prescaling for Timer1
  TIMSK1 |= (1 << TOIE1);           // Enable Timer1 overflow interrupt
  interrupts();                     // Enable interrupts
}

//=================================
//calcTimerValues() calculates all
//6 portions of sampling cycle
//=================================
void calcTimerValues()
{
  
  float temp1, temp2, temp3, temp4, temp5, temp6;  // Intermediate calculation variables
  if (digitalRead(boostPin) == HIGH) {                   // Get boost switch position
    txOn = normalPower;                                  // Set TX-on to 50us if HIGH
  } else {
    txOn = boostPower;                                   // Set TX-on to 100us if LOW
  }
  temp1 = (txOn - txOnOffset) / clockCycle;
  txOnCount = maxCount - int(temp1);                     // TX-on count for Timer1
  temp2 = (mainDelay - mainDelayOffset) / clockCycle;
  mainDelayCount = maxCount - int(temp2);                // Main sample delay count for Timer1
  temp3 = (mainSample - mainSampleOffset) / clockCycle;
  mainSampleCount = maxCount - int(temp3);               // Main sample pulse count for Timer1
  temp4 = (efeDelay - efeDelayOffset) / clockCycle;
  temp4 -= temp3 + temp2;
  efeDelayCount = maxCount - int(temp4);                 // EFE sample delay count for Timer1
  temp5 = (efeSample - efeSampleOffset) / clockCycle;
  efeSampleCount = maxCount - int(temp5);                // EFE sample pulse count for Timer 1
  temp6 = (txPeriod - txPeriodOffset) / clockCycle;
  temp6 -= temp1 + temp2 + temp3 + temp4 + temp5;
  txPeriodCount = maxCount - int(temp6);                 // TX period count for Timer1
}

//=================================
//ISR for timer events
//=================================
ISR(TIMER1_OVF_vect)
{
  switch (intState) {
    case 0:
      TCNT1 = txOnCount;                           // Load Timer1 with TX-ON count
      digitalWrite(txPin, mosfetOn);               // Turn on Mosfet
      digitalWrite(audioPin, audioLevel);
      if (audioLevel == HIGH)
      {
        audioLevel = LOW;
      }
      else
      {
        audioLevel = HIGH;
      }
      intState = 1;
      break;

    case 1:
      TCNT1 = mainDelayCount;                      // Load Timer1 with main sample delay count
      digitalWrite(txPin, mosfetOff);              // Turn off Mosfet
      intState = 2;
      break;

    case 2:
      TCNT1 = mainSampleCount;                     // Load Timer1 with main sample pulse count
      digitalWrite(mainSamplePin, syncDemodOn);    // Turn on main sample gate
      intState = 3;
      break;

    case 3:
      TCNT1 = efeDelayCount;                       // Load Timer1 with EFE sample delay count
      digitalWrite(mainSamplePin, syncDemodOff);   // Turn off main sample gate
      if (readDelayPot == false) {                 // Check if read delay pot flag is false
        readDelayCounter++;                        // Increment read delay counter
        if (readDelayCounter >= readDelayLimit) {  // Check if read delay counter has reached limit
          readDelayPot = true;                     // Enable read of delay pot
          readDelayCounter = 0;                    // Clear read delay counter
        }
      }
      intState = 4;
      break;

    case 4:
      TCNT1 = efeSampleCount;                      // Load Timer1 with EFE sample pulse count
      digitalWrite(efeSamplePin, syncDemodOn);     // Turn on EFE sample gate
      intState = 5;
      break;

    case 5:
      TCNT1 = txPeriodCount;                       // Load Timer1 with TX period count
      digitalWrite(efeSamplePin, syncDemodOff);    // Turn off EFE sample gate
      intState = 0;
      break;

    default:
      intState = 0;
      break;
  }
}
