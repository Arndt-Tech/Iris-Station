#ifndef GPIO_H
#define GPIO_H

// Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>
#include <DHT.h>
#include <DHT_U.h>
#include "_EEPROM.h"
#include "specFunctions.h"
#include "_LoRa.h"

// Mapeamento de hardware
#define DHTpin 2
#define valvePin1 12
#define valvePin2 13
#define pin_resetEEPROM 17

// Definições
#define readTime 2000
#define typeDHT DHT11
#define resetTmrOF 3

// Funções
uint8_t readDHT(networkLora *gtw);
void valve(uint8_t state);
void resetClear();

#endif