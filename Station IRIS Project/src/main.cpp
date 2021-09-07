#include <Arduino.h>
#include <FreeRTOS.h>

#include "configBegin.h"
#include "OLED.h"
#include "Bluetooth.h"
#include "_LoRa.h"

void setup()
{
  configBegin();

  setupOLED();
  waitingStart();

  setupBluetooth();
  bluetoothConfig();

  setupLoRa();
}

void loop()
{
}