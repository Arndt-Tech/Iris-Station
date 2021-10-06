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

// OLED pinout
#define OLED_SCL 15
#define OLED_SDA 4
#define OLED_RST 16

// Funções
void setupOLED();
void dataBar(Sensor *data, String icon, networkLora *lora, bool commit, bool clear);
void runnigSystem(networkLora *gtw, bool commit, bool clear);

#endif