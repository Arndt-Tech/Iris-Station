#include "generalTasks.h"

networkLora gateway;
Sensor sensor;
generalData hardware;

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

  valve(hardware.valveStatus);

  delay(50);
}