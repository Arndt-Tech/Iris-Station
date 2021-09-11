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

  display.clear();
  display.drawRect(0, 0, 126, 62);
  display.drawXbm(48, 7, logo_widht, logo_height, logo);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(Dialog_plain_12);
  display.drawString(64, 41, "IRIS");
  display.display();
  delay (5000);
}

void dataBar(int temperature, String icon, float lora_sig)
{
  display.clear();

  // Temperatura
  if (isnan(temperature))
  {
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(Dialog_plain_12);
    display.drawString(0, 4, "N/A");
  }
  else
  {
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(Dialog_plain_12);
    display.drawString(0, 4, String(temperature) + "°C");
  }
  
  // Ícone de tempo
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.setFont(Meteocons_Regular_18);
  display.drawString(128, 0, icon);


  // Força sinal LoRa
  if (lora_sig >= -35)
    display.drawIco16x16(88, 0, lora_str_signal, false);
  else if (lora_sig < -35 && lora_sig >= -80)
    display.drawIco16x16(88, 0, lora_mid_signal, false);
  else if (lora_sig < -80 && lora_sig >= -130)
    display.drawIco16x16(88, 0, lora_low_signal, false);
  else if (lora_sig < -130 || lora_sig == 0)
    display.drawIco16x16(88, 0, lora_not_signal, false);

  display.drawHorizontalLine(0, 21, 128);
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