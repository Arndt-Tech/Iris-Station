// Inclusões
#include "_LoRa.h"

void setupLoRa(networkLora *gtw)
{
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI00);
  if (!LoRa.begin(BAND))
  {
    Serial.println("Erro ao inicializar LoRa!");
    while (1)
      ;
  }
  //LoRa.setSpreadingFactor(SF);
  //LoRa.setSignalBandwidth(BW);
  //LoRa.setCodingRate4(CR);
  //LoRa.setPreambleLength(PL);
  //LoRa.setSyncWord(SW);
  LoRa.enableCrc();
}

void runningLoRa(networkLora *gtw, generalData *pin, Sensor *data)
{
  if (gtw->received)
    send_LoRa_Message(gtw, data);
  receive_LoRa_Message(gtw, pin);
  vTaskDelay(1);
}

void send_LoRa_Message(networkLora *gtw, Sensor *data)
{
  uint16_t aux_temp = data->temperature * 10;
  uint8_t packLen = sizeof(gtw->destAddr) +
                    sizeof(gtw->localAddr) +
                    sizeof(data->humidity) +
                    sizeof(data->temperature)/2 +
                    sizeof(packLen);
  LoRa.beginPacket();
  // Endereço destino
  LoRa.write(gtw->destAddr);
  LoRa.write(gtw->destAddr >> 8 & 0xFF);
  LoRa.write(gtw->destAddr >> 16 & 0xFF);
  LoRa.write(gtw->destAddr >> 24 & 0xFF);
  // Endereço local
  LoRa.write(gtw->localAddr);
  LoRa.write(gtw->localAddr >> 8 & 0xFF);
  LoRa.write(gtw->localAddr >> 16 & 0xFF);
  LoRa.write(gtw->localAddr >> 24 & 0xFF);
  // Umidade
  LoRa.write(data->humidity);
  // Temperatura
  LoRa.write(aux_temp);
  LoRa.write(aux_temp >> 8 & 0xFF);
  // Tamanho do pacote
  LoRa.write(packLen);
  // Fim do pacote
  LoRa.endPacket();
  gtw->received = 0;
}

String receive_LoRa_Message(networkLora *gtw, generalData *pin)
{
  uint8_t packSize = LoRa.parsePacket();
  if (packSize == 0)
    return "";
  uint8_t to_who_addr[4] = {0};
  uint8_t sender_addr[4] = {0};
  uint16_t incomingLength = 0;
  uint8_t aux_valveStatus = 0;
  for (register int i = 0; i < 4; i++)
    to_who_addr[i] = LoRa.read();
  for (register int i = 0; i < 4; i++)
    sender_addr[i] = LoRa.read();
  if (asm_addr(to_who_addr) != gtw->localAddr || asm_addr(sender_addr) != gtw->destAddr)
    return "";
  aux_valveStatus = LoRa.read();
  incomingLength = LoRa.read();
  if (incomingLength != packSize)
    return ""; // Pacote inconsistente
  pin->valveStatus = aux_valveStatus;
  gtw->received = 1;
  gtw->signal = LoRa.packetRssi();
  Serial.println("Endereço pra quem: " + String(asm_addr(to_who_addr)));
  Serial.println("Endereço remetente: " + String(asm_addr(sender_addr)));
  Serial.println("Comando: " + String(pin->valveStatus));
  Serial.println("Tamanho recebido: " + String(incomingLength));
  Serial.println("Tamanho identificado: " + String(packSize));
  Serial.println("");
  return "";
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