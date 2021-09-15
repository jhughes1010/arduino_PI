//=================================
//Routines to read i2c buttons
//=================================

//=================================
//Routines to read i2c buttons
//=================================
int selectPressed()
{
  uint8_t buttons = lcd.readButtons() & BUTTON_SELECT;
  if (buttons)
  {
    Serial.print("Select button value: ");
    Serial.println(buttons);
    delay(750);
  }
  return buttons;
}

//=================================
//upPressed()
//=================================
int upPressed()
{
  uint8_t buttons = lcd.readButtons() & BUTTON_UP;
  if (buttons)
  {
    //Serial.print("Up button value: ");
    //Serial.println(buttons);
    //delay(750);
    buttons = 1;
  }
  return buttons;
}

//=================================
//downPressed()
//=================================
int downPressed()
{
  uint8_t buttons = lcd.readButtons() & BUTTON_DOWN;
  if (buttons)
  {
    //Serial.print("Down button value: ");
    //Serial.println(buttons);
    //delay(750);
    buttons = 1;
  }
  return buttons;
}

//=================================
//leftPressed()
//=================================
int leftPressed()
{
  uint8_t button = lcd.readButtons() & BUTTON_LEFT;
  if (button)
  {
    button = 1;
  }
  return button;
}

//=================================
//rightPressed()
//=================================
int rightPressed()
{
  uint8_t button = lcd.readButtons() & BUTTON_RIGHT;
  if (button)
  {
    button = 1;
  }
  return button;
}
