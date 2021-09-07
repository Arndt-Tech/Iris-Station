#ifndef _LORA_H
#define _LORA_H

//Inclusões
#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

// LoRa defines
#define INTERVAL 2205 // 2000 + 205ms para compensar o delay de processamento do Gateway
#define BAND 433E6    // Frequencia 433MHz

// LoRa pinout
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DI00 26

// Funções
void setupLoRa();
void runningLoRa();
void send_LoRa_Message(String dados);
void receive_LoRa_Message(int packetSize);

#endif