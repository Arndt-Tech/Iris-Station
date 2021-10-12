#include "systemFunctions.h"

networkLora gateway;
GPS gps;

void setup() { configBegin(&gateway, &gps); }
void loop() 
{ 
  locationRead(&gps, &gateway);
  Serial.println("HEAP: " + String(xPortGetFreeHeapSize()));
  vTaskDelay(1000); 
}