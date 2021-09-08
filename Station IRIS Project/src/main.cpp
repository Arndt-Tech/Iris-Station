#include <Arduino.h>
#include <FreeRTOS.h>
#include "configBegin.h"
#include "OLED.h"
#include "Bluetooth.h"
#include "_LoRa.h"
#include "multiCore.h"
#include "specFunctions.h"

void setup()
{
  configBegin();

  setupOLED();
  waitingStart();

  setupBluetooth();
  bluetoothConfig();

  BLE_OK();

  setupLoRa();

  xTaskCreatePinnedToCore(taskLoRa, "taskLora", 10000, NULL, 3, NULL, 0);
}

void loop()
{
}