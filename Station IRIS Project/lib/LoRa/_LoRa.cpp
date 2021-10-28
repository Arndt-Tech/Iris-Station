// Inclusões
#include "_LoRa.h"

void setupLoRa(networkLora *gtw)
{
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI00);
  if (!LoRa.begin(BAND))
  {
    error(ERR_INITIALIZING_LORA);
    while (1)
      ;
  }
  LoRa.setSpreadingFactor(SF);
  //LoRa.setSignalBandwidth(BW);
  //LoRa.setCodingRate4(CR);
  //LoRa.setPreambleLength(PL);
  //LoRa.setSyncWord(SW);
  LoRa.enableCrc();
  gtw->signal = -164;
  gtw->received = 0;
  gtw->valveStatus = 0;
}

void runningLoRa(networkLora *gtw)
{
  if (gtw->received)
    send_LoRa_Message(gtw);
  error(receive_LoRa_Message(gtw));
}

void send_LoRa_Message(networkLora *gtw)
{
  // Ajuste de dados
  gtw->packet.packetLenght = sizeof(gtw->packet);
  // Inicio do pacote
  LoRa.beginPacket();
  // Envia ID's
  packID(gtw);
  // Envia dados de Sensores
  packSensors(gtw);
  // Envia dados de GPS (Se corresponder com tipo de pacote)
  packGPS(gtw);
  // Envia tamanho do pacote
  LoRa.write(gtw->packet.packetLenght);
  // Fim do pacote
  LoRa.endPacket();
  // Flag de requisição zerada
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
  for (register uint8_t i = 0; i < 4; i++)
    to_who_addr[i] = LoRa.read();
  for (register uint8_t i = 0; i < 4; i++)
    sender_addr[i] = LoRa.read();
  if (asm_addr(to_who_addr) != gtw->packet.localAddr || asm_addr(sender_addr) != gtw->packet.destAddr)
    return WAR_PACKET_OUT_OF_PROTOCOL;
  __valveStatus = LoRa.read();
  incomingLength = LoRa.read();
  if (incomingLength != packSize)
    return ERR_INCONSISTENT_LORA_PACKAGE; // Pacote inconsistente
  gtw->received = 1;
  gtw->signal = LoRa.packetRssi();
  valve(gtw->valveStatus = __valveStatus);
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

void packID(networkLora *gtw)
{
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
}

void packSensors(networkLora *gtw)
{
  // Ajuste de dados
  gtw->packet.temperature = gtw->packetAux.temperature * 10;
  // Umidade
  LoRa.write(gtw->packet.humidity);
  // Temperatura
  LoRa.write(gtw->packet.temperature);
  LoRa.write(gtw->packet.temperature >> 8 & 0xFF);
}

void packGPS(networkLora *gtw)
{
  // Ajuste de dados
  gtw->packet.latitude = gtw->packetAux.latitute * -1000000;
  gtw->packet.longitude = gtw->packetAux.longitude * -1000000;
  // Latitude
  LoRa.write(gtw->packet.latitude);
  LoRa.write(gtw->packet.latitude >> 8 & 0xFF);
  LoRa.write(gtw->packet.latitude >> 16 & 0xFF);
  LoRa.write(gtw->packet.latitude >> 24 & 0xFF);
  // Longitude
  LoRa.write(gtw->packet.longitude);
  LoRa.write(gtw->packet.longitude >> 8 & 0xFF);
  LoRa.write(gtw->packet.longitude >> 16 & 0xFF);
  LoRa.write(gtw->packet.longitude >> 24 & 0xFF);
}