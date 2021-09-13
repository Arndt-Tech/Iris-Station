#include "OLED.h"

// Instâncias
SSD1306 display(0x3c, OLED_SDA, OLED_SCL);

// Funções
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
  delay(3000);
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

  // dBM
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(Dialog_plain_12);
  display.drawString(0, 64, String(lora_sig));

  // Força sinal LoRa
  if (lora_sig >= -35)
    display.drawIco16x16(88, 0, lora_str_signal, false);
  else if (lora_sig < -30 && lora_sig >= -60)
    display.drawIco16x16(88, 0, lora_mid_signal, false);
  else if (lora_sig < -60 && lora_sig >= -120)
    display.drawIco16x16(88, 0, lora_low_signal, false);
  else if (lora_sig < -120 || lora_sig == -164)
    display.drawIco16x16(88, 0, lora_not_signal, false);

  display.drawHorizontalLine(0, 21, 128);
  //display.display();
}

void runnigSystem(lora_com *gtw)
{
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(Dialog_plain_12);
  display.drawString(0, 25, "Gateway: " + String(gtw->destAddr));
  display.drawString(0, 45, "Local: " + String(gtw->localAddr));
  display.display();
}