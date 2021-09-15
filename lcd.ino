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


#endif
