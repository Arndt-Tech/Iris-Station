#ifndef _MULTICORE_H
#define _MULTICORE_H

// Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>
#include "_LoRa.h"

void taskLoRa (void *pvParameters);

#endif