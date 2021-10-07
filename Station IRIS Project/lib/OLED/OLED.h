#ifndef __OLED_H
#define __OLED_H

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
#include "GPIO.h"

// Funções
void setupOLED();
void dataBar(networkLora *gtw, String icon, bool commit, bool clear);
void runnigSystem(networkLora *gtw, bool commit, bool clear);

#endif