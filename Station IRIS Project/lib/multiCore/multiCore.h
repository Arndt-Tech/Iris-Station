#ifndef _MULTICORE_H
#define _MULTICORE_H

// Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>
#include "_LoRa.h"

// Struct's externas
extern lora_com gateway;

// Funções
void taskLoRa (void *pvParameters);

#endif