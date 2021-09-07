#include "OLED.h"

// Instâncias
SSD1306 display(0x3c, OLED_SDA, OLED_SCL);

// Desenvolvimento de funções
void setupOLED()
{
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, 0);
  digitalWrite(OLED_RST, 1);
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
}

void waitingStart()
{
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 12, "Waiting");
  display.drawString(64, 32, "Connection...");
  display.display();
}

void BLE_OK()
{
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 12, "Successfully");
  display.drawString(64, 32, "Connected!");
  display.display();
}