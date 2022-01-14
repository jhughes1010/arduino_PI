//=================================
//Routines to read i2c buttons
//=================================
byte readButtons(void)
{
  static bool pressed = false;
  static int debounce = 0;
  byte buttonValue = 0;

  if(!pressed)
  {
    buttonValue = lcd.readButtons();
    if(buttonValue)
    {
      pressed = true;
      debounce=0;
    }
  }
  else
  {
    debounce++;
    if(debounce >=2^14)
    pressed = false;
  }
  return buttonValue;
}
//=================================
//Routines to read i2c buttons
//=================================
int selectPressed()
{
  uint8_t buttons = readButtons() & BUTTON_SELECT;
  if (buttons)
  {
    Serial.print("Select button value: ");
    Serial.println(buttons);
    delay(2000);
  }
  return buttons;
}

//=================================
//upPressed()
//=================================
int upPressed()
{
  uint8_t buttons = readButtons()& BUTTON_UP;
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
  uint8_t buttons = readButtons() & BUTTON_DOWN;
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
  uint8_t button = readButtons() & BUTTON_LEFT;
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
  uint8_t button = readButtons() & BUTTON_RIGHT;
  if (button)
  {
    button = 1;
  }
  return button;
}

//=================================
//nonIntDelay()
//=================================
void nonIntDelay(int delayTime)
{
  int loop1, loop2;
  for (loop1 = 0; loop1 < 255; loop1++)
  {
    for (loop2 = 0; loop2 < maxCount; loop2++)
    {
    }
  }
}
