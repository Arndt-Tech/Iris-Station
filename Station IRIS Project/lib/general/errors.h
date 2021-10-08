#ifndef _ERRORS_H
#define _ERRORS_H

// Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>
#include "pinout.h"

// Enumerações
typedef enum
{
  // Geral
  INIT_OK = 1,
  NOT_ERROR = 2,
  // Sensores
  ERR_DHT_ISNAN = 10,
  // LoRa
  ERR_INITIALIZING_LORA = 20,
  WAR_NO_LORA_PACKAGES = 21,
  WAR_PACKET_OUT_OF_PROTOCOL = 22,
  WAR_INCONSISTENT_LORA_PACKAGE = 23,
  // EEPROM
  WAR_EMPTY_EEPROM = 30,
} err;

// Definições
#define buzzChannel(a) (uint8_t)(a)

// Funções
void setupBuzzer();
err error(err error);
void long_bip();
void short_bip();

#endif