// Inclusões
#include "_LoRa.h"

// Variáveis externas
int erros, pacotes, enviados;

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
  gtw->packSize = 0;
}

// -|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void runningLoRa(networkLora *gtw, generalData *pin, Sensor *data)
{
  static unsigned long tLoRaSend = 0;
  if ((millis() - tLoRaSend) > INTERVAL)
  {
    send_LoRa_Message(gtw, data);
    tLoRaSend = millis();
  }
  gtw->packSize = LoRa.parsePacket();
  receive_LoRa_Message(gtw, pin);
}

// -|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void send_LoRa_Message(networkLora *gtw, Sensor *data)
{
  float aux_temp = data->temperature * 10;
  uint8_t packLen = sizeof(gtw->destAddr) +
                    sizeof(gtw->localAddr) +
                    sizeof(data->humidity) +
                    sizeof(data->temperature) +
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
  LoRa.write((int8_t)aux_temp / 10);
  LoRa.write((uint8_t)aux_temp % 10);
  // Tamanho do pacote
  LoRa.write(packLen);
  // Fim do pacote
  LoRa.endPacket();
  gtw->packSize = packLen;
  enviados++;
}

// -|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
String receive_LoRa_Message(networkLora *gtw, generalData *pin)
{
  if (gtw->packSize == 0)
    return "";
  uint8_t to_who_addr[4] = {0};
  uint8_t sender_addr[4] = {0};
  uint16_t incomingLength = 0;
  uint8_t aux_valveStatus = 0;
  pacotes++;
  for (register int i = 0; i < 4; i++)
    to_who_addr[i] = LoRa.read();
  for (register int i = 0; i < 4; i++)
    sender_addr[i] = LoRa.read();
  if (asm_addr(to_who_addr) != gtw->localAddr || asm_addr(sender_addr) != gtw->destAddr)
  {
    Serial.println("Pacote ignorado\n");
    return "";
  }
  aux_valveStatus = LoRa.read();
  incomingLength = LoRa.read();
  if (incomingLength != gtw->packSize)
  {
    Serial.println("Pacote inconsistente");
    return ""; // Pacote inconsistente
  }
  pin->valveStatus = aux_valveStatus;
  gtw->received = 1;
  Serial.println("Endereço pra quem: " + String(asm_addr(to_who_addr)));
  Serial.println("Endereço remetente: " + String(asm_addr(sender_addr)));
  Serial.println("Comando: " + String(pin->valveStatus));
  Serial.println("Tamanho recebido: " + String(incomingLength));
  Serial.println("Tamanho identificado: " + String(gtw->packSize));
  Serial.println("");
  return "";
}

/*
  Serial.println("Recebido de: 0x" + String(sender, HEX));
  Serial.println("Enviado para: 0x" + String(requisitor, HEX));
  Serial.println("Tamanho do pacote: " + String(incomingLength));
  Serial.println("Pacote recebido: " + incoming);
  Serial.println("dBm: " + String(LoRa.packetRssi()));
  Serial.println("Ruido: " + String(LoRa.packetSnr()));
  Serial.println();
  */

uint32_t asm_addr(uint8_t *addr)
{
  uint32_t newAddr = 0;
  newAddr |= addr[0];
  newAddr |= addr[1] << 8;
  newAddr |= addr[2] << 16;
  newAddr |= addr[3] << 24;
  return newAddr;
}