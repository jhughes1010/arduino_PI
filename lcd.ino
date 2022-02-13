//Functions for LCD usage
#ifdef LCD
//=================================
//Init LCD
//=================================
void LCDInit(void)
{
  lcd.begin(16, 2);
  lcd.setBacklight(0x01);
}

//=================================
//Title at 0,0
//=================================
void LCDTitle(void)
{
  lcd.setCursor(0, 0);
  lcd.print(NAME);
  lcd.setCursor(0, 1);
  lcd.print(VERSION);
  delay(2000);
}

//=================================
//LCDBoost()
//=================================
void LCDBoost( void )
{
  lcd.setCursor(0, 0);
  if (boost)
    lcd.print("Boost ");
  else
    lcd.print("Normal");
}

//=================================
//LCDBar()
//=================================
void LCDBar ( void)
{
  byte bar[16] = {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32};
  int pixels;
  int pos;
  int columns;
  int pixPortion;
  int signalValue;

  if (debounceCounter % 4 == 0 && intState == 5)
  {
    debounceCounter++;
    intTimerOff();
    //readDelayPot = false;
    signalValue = analogRead(signalPin);
    //signalValue = analogRead(delayPin);
    //signalValue = 480;

    //convert 10 bit value to pixel 0-79
    pixels = (float)signalValue / 1023 * 80;
    columns = (int)(pixels / 5);
    pixPortion = (int)pixels % 5;

    debugln(columns);
    debugln(pixPortion);
    //fill columns
    for (pos = 0; pos < columns; pos++)
    {
      bar[pos] = 4;
    }
    bar[columns] = pixPortion;

    //Write full row
    lcd.setCursor(0, 1);
    for (pos = 0; pos < 16; pos++)
    {
      lcd.write(bar[pos]);
    }
    intTimerOn();
  }
}

//=================================
//LCDLowBat()
//=================================
void LCDLowBat(void)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Low Battery");
}

//=================================
//LCDPrintVbat()
//=================================
void LCDPrintVbat(float voltage)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("VBAT: ");
  lcd.print(voltage, 2);
}

//=================================
//LCDPrintCoilWidth()
//=================================
void LCDPrintCoilWidth(void)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TX: ");
  lcd.print(coilPulseWidthArray[coilPulseIndex] * 1e6);
}

//=================================
//LCDPrintSampleWidth()
//=================================
void LCDPrintSampleWidth(void)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sample: ");
  lcd.print(targetSampleWidthArray[targetSamplePulseIndex] * 1e6);
}
#endif
