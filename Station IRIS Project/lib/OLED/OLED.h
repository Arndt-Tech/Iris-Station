#ifndef _OLED_H
#define _OLED_H

// Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>
#include <Wire.h>
#include <SSD1306.h>

// OLED pinout
#define OLED_SCL 15
#define OLED_SDA 4
#define OLED_RST 16

// Funções
void setupOLED();
void waitingStart();
void BLE_OK();

#endif