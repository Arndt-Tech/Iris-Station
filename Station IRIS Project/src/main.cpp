#include "generalFunctions.h"

networkLora gateway;

void setup()
{
  configBegin(&gateway);
  setupTasks();
}

void loop() {}