// Inclusões
#include "Log.h"

void cfg::Log::begin() { EEPROM.begin(EEPROM_SIZE); }

/**
 * @brief Check if the address is clean.
 * 
 * @param addr Address to be verified.
 * @return Address status.
 */
bool cfg::Log::check(uint8_t addr) { return EEPROM.read(addr) != 255; }

/**
 * @brief Clear certain log partition.
 * 
 * @param addr0 Initial Address.
 * @param addr1 Final Address.
 * @return true - Operation success.
 */
bool cfg::Log::clear(uint8_t addr0, uint8_t addr1)
{
  for (register uint8_t i = addr0; i <= addr1; i++)
    EEPROM.writeULong(i, 255);
  return EEPROM.commit();
}

/**
 * @brief Write ULong data in log.
 * 
 * @param data Data to be written.
 * @param addr Data address.
 * @return true - Operation success. 
 */
bool cfg::Log::write(uint32_t data, uint8_t addr)
{
  EEPROM.writeULong(addr, data);
  return EEPROM.commit();
}

/**
 * @brief Read ULong data in log. 
 * 
 * @param addr Data address.
 * @return ULong data in the address.
 */
uint32_t cfg::Log::read(uint8_t addr) { return EEPROM.readULong(addr); }
