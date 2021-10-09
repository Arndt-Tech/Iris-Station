// Inclusões
#include "_EEPROM.h"

// Funções
bool verifyEEPROM(uint8_t addr_min)
{
  if (EEPROM.read(addr_min) != 255)
    return 1;
  else
    return 0;
}

bool clearEEPROM(uint8_t addr0, uint8_t addr1)
{
  for (register uint8_t i = addr0; i <= addr1; i++)
    EEPROM.writeULong(i, 255);
  if (EEPROM.commit())
    return 1;
  else
    return 0;
  return 1;
}

bool writeEEPROM(uint32_t data, uint8_t addr)
{
  EEPROM.writeULong(addr, data);
  if (EEPROM.commit())
    return 1;
  else
    return 0;
}

uint32_t readEEPROM(uint8_t addr) { return EEPROM.readULong(addr); }
