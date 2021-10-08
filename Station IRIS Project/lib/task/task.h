#ifndef _MULTICORE_H
#define _MULTICORE_H

// Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>
#include "pinout.h"
#include "errors.h"
#include "OLED.h"
#include "_LoRa.h"
#include "Peripherals.h"

// Definições
#define CORE(a) (BaseType_t)(a)
#define STACK(a) (uint32_t)(a)
#define PRIORITY(a) (UBaseType_t)(a)

// Struct's externas
extern networkLora gateway;

// Funções
void setupTasks();

// Tasks
void taskLoRa(void *pvParameters);
void taskReadData(void *pvParameters);
void taskOled(void *pvParameters);
void taskReset(void *pvParameters);

#endif