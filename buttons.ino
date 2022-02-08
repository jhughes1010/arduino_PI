//=================================
//Routines to read i2c buttons
//=================================
byte readButtons(void)
{
  static bool pressed = false;
  static byte priorDebounceCounter = 0;
  byte buttonValue = 0;

  if (!pressed)
  {
    buttonValue = lcd.readButtons();
    if (buttonValue)
    {
      pressed = true;
      priorDebounceCounter = debounceCounter;
    }
  }
  else
  {
    if (abs(debounceCounter - priorDebounceCounter) >= 3)
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
//buttonRead()
//=================================
void buttonRead(void)
{
  if (intState == 5)
  {
    if (rightPressed())
    {
      cycleTxPulse();
      LCDPrintCoilWidth();
    }
    if(upPressed())
    {
      cycleSampleWidth();
      LCDPrintSampleWidth();
    }
  }
}

//=================================
//cycleTxPulse()
//=================================
void cycleTxPulse (void)
{
  coilPulseIndex ++;
  coilPulseIndex = coilPulseIndex % coilPulseMaxChoice;
  debugln(coilPulseIndex);
}

//=================================
//cycleSampleWidth()
//=================================
void cycleSampleWidth (void)
{
  targetSamplePulseIndex ++;
  targetSamplePulseIndex = targetSamplePulseIndex % targetSampleMaxChoice;
  debugln(targetSamplePulseIndex);
}
