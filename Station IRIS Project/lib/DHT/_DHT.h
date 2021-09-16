#ifndef _DHT_H
#define _DHT_H

// Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>
#include <DHT.h>
#include <DHT_U.h>

// Definições
#define DHTpin 12
#define typeDHT DHT11

// Struct's 
typedef struct temp_sensor
{
  float temperature;
  float humidity;
} Sensor;

// Funções
bool readDHT(Sensor *temp);

#endif