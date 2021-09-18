#include "generalTasks.h"

networkLora gateway;
Sensor sensor;

void setup()
{
  configBegin();
  setupOLED();
  setupDataSystem(&gateway);
  setupLoRa(&gateway);
  setupMultiCore(0);
}

void loop()
{
  gateway.signal = LoRa.packetRssi();
  readDHT(&sensor);
  dataBar(&sensor, "3", &gateway, false, true);
  runnigSystem(&gateway, true, false);
  resetClear();

  Serial.println(LoRa.packetRssi());

  delay(100);
}