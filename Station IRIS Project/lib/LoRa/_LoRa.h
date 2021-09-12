#ifndef _LORA_H
#define _LORA_H

//Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>
#include <SPI.h>
#include <LoRa.h>

/*----------------------------------------------------------------------------------------------*/
// LoRa Config.
#define INTERVAL 2000 // 2000 + 205ms para compensar o delay de processamento do Gateway
#define BAND 433E6    // Frequencia 433MHz
// Pinout
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DI00 26

// Struct's
typedef struct _lora
{
  uint32_t localAddr;
  uint32_t destAddr;
  int packSize;
  String incomingPack;
} lora_com;

// Funções
void setupLoRa(lora_com *gtw);
void runningLoRa(lora_com *gtw);
void send_LoRa_Message(String dados, lora_com *gtw);
void receive_LoRa_Message(lora_com *gtw);

#endif