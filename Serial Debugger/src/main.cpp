#include <Arduino.h>
#include <FreeRTOS.h>
#include <HardwareSerial.h>

#define S1RX 25
#define S1TX 26

HardwareSerial sr(2);

void setup() 
{
  Serial.begin(115200);
  sr.begin(115200, SERIAL_8N1, S1RX, S1TX);
}

void loop() 
{
  if (sr.available())
    Serial.write(sr.read());
}