#ifndef _LORA_H
#define _LORA_H

#include <SPI.h>              
#include <LoRa.h>

// LoRa
  #define INTERVAL 2000 // 2000 + 205ms para compensar o delay de processamento do Gateway
  #define BAND 433E6    // Frequencia 433MHz

// LoRa pinout
  #define SCK   5
  #define MISO 19
  #define MOSI 27
  #define SS   18
  #define RST  14
  #define DI00 26 


// Protótipo de funções
  void setupLoRa();
  void runningLoRa();
  void send_LoRa_Message(String dados);
  void receive_LoRa_Message(int packetSize);

// Variáveis
  byte localAddress = 0xBB;     // Endereço deste gateway
  byte comLine = 0xFF;      // Endereço da estação IRIS
  int requisitor;
  byte sender;
  byte incomingLength;
  String incoming = "";
  int erros, pacotes, enviados;


#endif
