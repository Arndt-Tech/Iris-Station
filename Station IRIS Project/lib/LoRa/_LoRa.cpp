// Inclusões
#include "_LoRa.h"

void setupLoRa(networkLora *gtw)
{
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI00);
  if (!LoRa.begin(BAND))
  {
    error(ERR_INITIALIZING_LORA);
    while(1);
  }
  //LoRa.setSpreadingFactor(SF);
  //LoRa.setSignalBandwidth(BW);
  //LoRa.setCodingRate4(CR);
  //LoRa.setPreambleLength(PL);
  //LoRa.setSyncWord(SW);
  LoRa.enableCrc();
}

void runningLoRa(networkLora *gtw)
{
  if (gtw->received)
    send_LoRa_Message(gtw);
  receive_LoRa_Message(gtw);
  vTaskDelay(1);
}

void send_LoRa_Message(networkLora *gtw)
{
  uint16_t aux_temp = gtw->packet.temperature * 10;
  gtw->packet.packetLenght = (sizeof(gtw->packet) - (size_t)4);
  LoRa.beginPacket();
  // Endereço destino
  LoRa.write(gtw->packet.destAddr);
  LoRa.write(gtw->packet.destAddr >> 8 & 0xFF);
  LoRa.write(gtw->packet.destAddr >> 16 & 0xFF);
  LoRa.write(gtw->packet.destAddr >> 24 & 0xFF);
  // Endereço local
  LoRa.write(gtw->packet.localAddr);
  LoRa.write(gtw->packet.localAddr >> 8 & 0xFF);
  LoRa.write(gtw->packet.localAddr >> 16 & 0xFF);
  LoRa.write(gtw->packet.localAddr >> 24 & 0xFF);
  // Umidade
  LoRa.write(gtw->packet.humidity);
  // Temperatura
  LoRa.write(aux_temp);
  LoRa.write(aux_temp >> 8 & 0xFF);
  // Tamanho do pacote
  LoRa.write(gtw->packet.packetLenght);
  // Fim do pacote
  LoRa.endPacket();
  gtw->received = 0;
}

err receive_LoRa_Message(networkLora *gtw)
{
  uint8_t packSize = LoRa.parsePacket();
  if (packSize == 0)
    return WAR_NO_LORA_PACKAGES;
  uint8_t to_who_addr[4] = {0};
  uint8_t sender_addr[4] = {0};
  uint16_t incomingLength = 0;
  uint8_t __valveStatus = 0;
  for (register int i = 0; i < 4; i++)
    to_who_addr[i] = LoRa.read();
  for (register int i = 0; i < 4; i++)
    sender_addr[i] = LoRa.read();
  if (asm_addr(to_who_addr) != gtw->packet.localAddr || asm_addr(sender_addr) != gtw->packet.destAddr)
    return WAR_PACKET_OUT_OF_PROTOCOL;
  __valveStatus = LoRa.read();
  incomingLength = LoRa.read();
  if (incomingLength != packSize)
    return WAR_INCONSISTENT_LORA_PACKAGE; // Pacote inconsistente
  gtw->valveStatus = __valveStatus;
  gtw->received = 1;
  gtw->signal = LoRa.packetRssi();
  Serial.println("Endereço pra quem: " + String(asm_addr(to_who_addr)));
  Serial.println("Endereço remetente: " + String(asm_addr(sender_addr)));
  Serial.println("Comando: " + String(gtw->valveStatus));
  Serial.println("Tamanho recebido: " + String(incomingLength));
  Serial.println("Tamanho identificado: " + String(packSize));
  Serial.println("");
  return NOT_ERROR;
}

uint32_t asm_addr(uint8_t *addr)
{
  uint32_t newAddr = 0;
  newAddr |= addr[0];
  newAddr |= addr[1] << 8;
  newAddr |= addr[2] << 16;
  newAddr |= addr[3] << 24;
  return newAddr;
}