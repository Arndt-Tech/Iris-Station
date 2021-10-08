#ifndef GPIO_H
#define GPIO_H

// Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>
#include <DHT.h>
#include <DHT_U.h>
#include "pinout.h"
#include "errors.h"
#include "_EEPROM.h"
#include "specFunctions.h"
#include "_LoRa.h"

// Definições
#define readTime 2000
#define typeDHT DHT11
#define resetTmrOF 3

// Funções
err readDHT(networkLora *gtw);
void valve(uint8_t state);
void resetClear();

#endif