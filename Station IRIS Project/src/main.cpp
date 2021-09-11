#include <Arduino.h>
#include <FreeRTOS.h>
#include "configBegin.h"
#include "OLED.h"
#include "Bluetooth.h"
#include "_LoRa.h"
#include "multiCore.h"
#include "specFunctions.h"
#include "ChipID.h"

void setup()
{
  configBegin();
  
  setupOLED();
  Serial.println(getChipID());
  dataBar(-3, "7", LoRa.packetRssi());

  setupBluetooth();
  bluetoothConfig();

  setupLoRa();

  xTaskCreatePinnedToCore(taskLoRa, "taskLora", 10000, NULL, 3, NULL, 0);
}

void loop()
{
  
}