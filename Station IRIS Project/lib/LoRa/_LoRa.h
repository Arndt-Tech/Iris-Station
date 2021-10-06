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
#define SF 12
#define BW 250E3
#define CR 4
#define PL 6
#define SW 0x16
/*
  Config:
    Band -> 433MHz
    Spread Factor -> 12
    Bandwitch -> 250KHz
    Coding Rate -> 4
    Payload Lenght -> 10 bytes
    Preable Lenght -> 6
    Sync Word -> 0x16
  |----------------------------------|
  Data:
    Equivalent bitrate -> 366 bytes/s
    Time on air -> 561 ms
*/
// Pinout

#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DI00 26

// Struct's
typedef struct _package
{
  uint32_t localAddr;
  uint32_t destAddr;
  float temperature;
  uint8_t humidity;
  uint8_t packetLenght;
} loraPackage;

typedef struct _lora
{
  // Package
  loraPackage packet;
  // Status
  uint8_t received;
  int8_t signal;
  uint8_t valveStatus;
} networkLora;

// Funções
void setupLoRa(networkLora *gtw);
void runningLoRa(networkLora *gtw);
void send_LoRa_Message(networkLora *gtw);
String receive_LoRa_Message(networkLora *gtw);
uint32_t asm_addr(uint8_t *addr);

#endif