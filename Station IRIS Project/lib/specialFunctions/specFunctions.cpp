//
#include "specFunctions.h"

/**
 * @brief Reset device.
 */
void spc::SpecialFunctions::resetModule() { esp_restart(); } // Reinicia dispositivo

/**
 * @brief Address assembly.
 * 
 * @param addr Address. 
 */
uint32_t spc::SpecialFunctions::asm_addr(uint8_t *addr)
{
  uint32_t newAddr = 0;
  newAddr |= addr[0];
  newAddr |= addr[1] << 8;
  newAddr |= addr[2] << 16;
  newAddr |= addr[3] << 24;
  return newAddr;
}