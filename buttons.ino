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
      //debugln(buttonValue);
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
int selectPressed(uint8_t button)
{
  if (button & BUTTON_SELECT)
  {
    return 1;
  }
  return 0;
}

//=================================
//upPressed()
//=================================
int upPressed(uint8_t button)
{
  if (button & BUTTON_UP)
  {
    debugln("Up");
    return 1;
  }
  return 0;
}

//=================================
//downPressed()
//=================================
int downPressed(uint8_t button)
{
  if (button & BUTTON_DOWN)
  {
    debugln("Down");
    return 1;
  }
  return 0;
}

//=================================
//leftPressed()
//=================================
int leftPressed(uint8_t button)
{
  if (button  & BUTTON_LEFT)
  {
    debugln("Left");
    return 1;
  }
  return 0;
}

//=================================
//rightPressed()
//=================================
int rightPressed(uint8_t button)
{
  if (button & BUTTON_RIGHT)
  {
    debugln("Right");
    return 1;
  }
  return 0;
}

//=================================
//buttonRead()
//=================================
void buttonRead(void)
{
  //static bool readFlag = false;
  uint8_t button;
  
  if (readDelayPot == true)
  {
    //debug("/");
    readDelayPot = false;
    //if (intState == 5)
    {
      //readFlag = true;
      //read button register
      button = readButtons();
      //process button push
      if (rightPressed(button))
      {
        cycleTxPulse();
        LCDPrintCoilWidth();
      }
      if (upPressed(button))
      {
        cycleSampleWidth();
        LCDPrintSampleWidth();
      }
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
  //debugln(coilPulseIndex);
}

//=================================
//cycleSampleWidth()
//=================================
void cycleSampleWidth (void)
{
  targetSamplePulseIndex ++;
  targetSamplePulseIndex = targetSamplePulseIndex % targetSampleMaxChoice;
  //debugln(targetSamplePulseIndex);
}
