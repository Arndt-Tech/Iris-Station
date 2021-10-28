#include "systemFunctions.h"

networkBluetooth BLE;
networkLora gateway;
GPS gps;

void setup() { configBegin(&BLE, &gateway, &gps); }
void loop() {}