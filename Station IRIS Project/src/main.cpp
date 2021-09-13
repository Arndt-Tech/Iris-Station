#include <Arduino.h>
#include <FreeRTOS.h>
#include "configBegin.h"
#include "OLED.h"
#include "Bluetooth.h"
#include "_LoRa.h"
#include "multiCore.h"
#include "specFunctions.h"
#include "ChipID.h"
#include "_EEPROM.h"

lora_com gateway;

void setup()
{
  configBegin();
  setupOLED();

  clear_EEPROM(0, EEPROM_SIZE);

  // Verifica EEPROM
  if (!verify_EEPROM(loChID_addr_min) || !verify_EEPROM(chID_addr_min))
  {
    // Configura bluetooth e recebe novos dados
    setupBluetooth();
    getID(&gateway);

    // Aloca novos dados na EEPROM
    write_EEPROM(String(gateway.destAddr), chID_addr_min);
    write_EEPROM(String(gateway.localAddr), loChID_addr_min);
  }
  else
  {
    // Lê dados da EEPROM
    gateway.destAddr = atol(read_EEPROM(chID_addr_min).c_str());
    gateway.localAddr = atol(read_EEPROM(loChID_addr_min).c_str());
  }

  setupLoRa(&gateway);

  xTaskCreatePinnedToCore(taskLoRa, "taskLora", 10000, NULL, 3, NULL, 0);
}

void loop()
{
  dataBar(24, "3", LoRa.packetRssi());
  runnigSystem(&gateway);
  delay(100);
}