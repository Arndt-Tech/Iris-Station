#ifndef _OLED_H
#define _OLED_H

// Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>
#include <Wire.h>
#include <SSD1306.h>
#include "fonts.h"
#include "images.h"
#include "icons.h"

// Arquivos com funções para printar
#include "ChipID.h"
#include "_LoRa.h"

// OLED pinout
#define OLED_SCL 15
#define OLED_SDA 4
#define OLED_RST 16

// Funções
void setupOLED();
void dataBar(int temperature, String icon, float lora_sig);
void BLE_OK();

#endif