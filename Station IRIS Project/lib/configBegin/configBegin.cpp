#include "configBegin.h"

void configBegin()
{
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);
}