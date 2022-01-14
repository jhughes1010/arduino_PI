//=================================
//checkBattery()
//=================================
void checkBattery(void)
{
  float vbat;
  vbat = readBattery();
  if (vbat < 9)
  {
    LCDLowBat();
    while (1)
    {
    }
  }
}

//=================================
//readBattery()
//=================================
int readBattery( void)
{
  int adc;
  float voltage;
  adc = analogRead(batteryPin);
  voltage = adc / 68.2;
  return voltage;
}
