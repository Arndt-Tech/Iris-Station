#ifndef _GPS_H_
#define _GPS_H_

// Inclusões
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include "_LoRa.h"

// Definições
#define BAUD_RATE 4800

// Struct's
typedef struct gps
{
  SoftwareSerial softSerial;
  TinyGPSPlus data;
}GPS;

// Funções
void setupSoftwareSerial(GPS *gps);
void locationRead(GPS *gps, networkLora *gtw);

#endif