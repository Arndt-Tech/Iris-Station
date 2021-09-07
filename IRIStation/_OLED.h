#ifndef _OLED_H
#define _OLED_H

#include <SSD1306.h>

// SSD1306 pinout
  #define OLED_SCL 15
  #define OLED_SDA 4
  #define OLED_RST 16

SSD1306 display(0x3c, OLED_SDA, OLED_SCL);

void setupOLED();
void waitingStart();

#endif
