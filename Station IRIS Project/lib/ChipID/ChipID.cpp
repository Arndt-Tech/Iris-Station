// 
#include "ChipID.h"

/**
 * @brief Get the chip ID.
 * 
 * @return chip ID.
 */
uint32_t cfg::ChipID::get()
{
  uint32_t chipID = 0;
  for (register uint8_t i = 0; i < 17; i = i + 8)
    chipID |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  return chipID;
}