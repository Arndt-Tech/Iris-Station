#ifndef GPIO_H
#define GPIO_H

// Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>
#include <DHT.h>
#include <DHT_U.h>

// Definições
#define DHTpin 2
#define valvePin1 12
#define valvePin2 13
#define typeDHT DHT11

// Struct's
typedef struct
{
  float temperature;
  uint8_t humidity;
} Sensor;

typedef struct
{
  uint8_t valveStatus;
} generalData;

// Funções
bool readDHT(Sensor *temp);


#endif