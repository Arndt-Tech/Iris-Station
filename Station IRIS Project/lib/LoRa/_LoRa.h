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
  int signal;
  int noise;
} networkLora;

// Funções
void setupLoRa(networkLora *gtw);
void runningLoRa(networkLora *gtw);
void send_LoRa_Message(String dados, networkLora *gtw);
String receive_LoRa_Message(networkLora *gtw);
uint32_t asm_addr(uint8_t *addr);

#endif