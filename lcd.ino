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


void LCDBoost( void )
{
  lcd.setCursor(0, 0);
  if (boost)
    lcd.print("Boost ");
  else
    lcd.print("Normal");
  delay(1500);
}

void LCDBar ( int adcValue)
{
  int full;
  //convert 10 bit value to pixel 0-79
  float pixels = ((float)adcValue / 1023 * 80);
  int columns = (int)(pixels / 5);
  int pixPortion = (int)pixels % 5;
  //Serial.println(pixels);
  //Serial.println(columns);
  //Serial.println(pixPortion);
  //Serial.println("---");
  lcd.setCursor(0, 1);
  for (full = 0; full < columns; full++)
  {
    lcd.write(4);
  }
  lcd.write(byte(pixPortion));
  
}


#endif
