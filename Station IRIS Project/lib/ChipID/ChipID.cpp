// Inclusões
#include "ChipID.h"

// Funções
uint32_t getChipID()
{
  uint32_t chipID = 0;
  for (int i = 0; i < 17; i = i + 8)
    chipID |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  return chipID;
}