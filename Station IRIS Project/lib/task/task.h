#pragma once

// 
#include <Arduino.h>
#include <FreeRTOS.h>
#include "Station.hpp"
#include "debug.h"

// 
#define CORE(a) (BaseType_t)(a)
#define STACK(a) (uint32_t)(a)
#define PRIORITY(a) (UBaseType_t)(a)


// 
void setupTasks();

// 
void taskLoRa(void *pvParameters);
void taskReadData(void *pvParameters);
void taskReset(void *pvParameters);
void taskGPS(void *pvParameters);