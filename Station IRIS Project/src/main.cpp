#include "generalFunctions.h"

networkLora gateway;
Sensor sensor;
generalData hardware;

void setup()
{
  configBegin(&gateway);
  setupTasks();
}

void loop() {}