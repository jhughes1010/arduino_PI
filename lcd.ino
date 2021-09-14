//Functions for LCD usage
#ifdef LCD
//=================================
//Init LCD
//=================================
void LCDInit(void)
{
  lcd.init();
  lcd.backlight();
}

//=================================
//Title at 0,0
//=================================
void LCDTitle(void)
{
  lcd.setCursor(0, 0);
  lcd.print(NAME);
}
#endif