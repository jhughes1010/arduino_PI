//=================================
//voltageCalFactor
//=================================
// measured voltage at battery/measured
// voltage at analog pin A1
#define voltageCalFactor 12.3/4.3

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
  voltage = ((float)adc / 1023) * voltageCalFactor;
  return voltage;
}
