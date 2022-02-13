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
  TIMSK2 = 0;
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

  float temp1, temp2, temp3, temp4, temp5, temp6, temp7;  // Intermediate calculation variables
  if (digitalRead(boostPin) == HIGH) {                   // Get boost switch position
    txOn = coilPulseWidthArray[coilPulseIndex];
  } else {
    txOn = boostPower;                                   // Set TX-on to 100us if LOW
  }
  temp1 = (txOn - txOnOffset) / clockCycle;
  txOnCount = maxCount - int(temp1);                     // TX-on count for Timer1

  temp2 = (mainDelay - mainDelayOffset) / clockCycle;
  mainDelayCount = maxCount - int(temp2);                // Main sample delay count for Timer1

  mainSample = targetSampleWidthArray[targetSamplePulseIndex];
  efeSample = mainSample;
  temp3 = (mainSample - mainSampleOffset) / clockCycle;
  mainSampleCount = maxCount - int(temp3);               // Main sample pulse count for Timer1

  temp4 = (efeDelay - efeDelayOffset) / clockCycle;
  temp4 -= temp3 + temp2;
  efeDelayCount = maxCount - int(temp4);                 // EFE sample delay count for Timer1

  temp5 = (efeSample - efeSampleOffset) / clockCycle;
  efeSampleCount = maxCount - int(temp5);                // EFE sample pulse count for Timer 1

  temp7 = txPeriodBuffer / clockCycle;                   // TX buffer for no I2C traffic
  txPeriodBufferCount = maxCount - int(temp7);

  temp6 = (txPeriod - txPeriodOffset) / clockCycle;
  temp6 -= temp1 + temp2 + temp3 + temp4 + temp5 + temp7;
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
      PORTB |= (1 << 0);                           // TX coil on
      break;

    case 1:
      TCNT1 = mainDelayCount;                      // Load Timer1 with main sample delay count
      //TCNT1 = 0xfffd;
      PORTB &= ~(1 << 0);                          // TX coil off
      break;

    case 2:
      PORTB &= ~(1 << 1);                          // Sample pulse enable
      TCNT1 = mainSampleCount;                     // Load Timer1 with main sample pulse count
      break;

    case 3:
      TCNT1 = efeDelayCount;                       // Load Timer1 with EFE sample delay count
      PORTB |= (1 << 1);                           // Sample pulse disable
      break;

    case 4:
      TCNT1 = efeSampleCount;                      // Load Timer1 with EFE sample pulse count
      PORTB &= ~(1 << 2);                          // EFE pulse enable
      break;

    case 5:
      TCNT1 = txPeriodBufferCount;                       // Load Timer1 with TX period count
      PORTB |= (1 << 2);                           // EFE pulse disable

      //toggle audio pin state
      digitalWrite(audioPin, !digitalRead(audioPin));

      //update counter to allow for periodic reading of delay pot
      if (readDelayPot == false)                   // Check if read delay pot flag is false
      {
        readDelayCounter++;                        // Increment read delay counter
        readDelayCounter = readDelayCounter % readDelayLimit;
        if (!readDelayCounter)                      // Check if read delay counter is zero
        {
          readDelayPot = true;                     // Enable read of delay pot
          debounceCounter++;
          debounceCounter = debounceCounter % 10;  //10 counts per second
        }
      }
      break;

    case 6:
      TCNT1 = txPeriodCount;
      break;

    default:
      TCNT1 = 0xfffd;
      intState = -1;
      break;
  }

  //Increment to next state machine value but always ensure intState is between 0 - 6
  intState ++;
  //intState = intState % 7;
}
