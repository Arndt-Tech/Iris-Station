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
    while (1);
  }
  gtw->packSize = 0;
  gtw->incomingPack = "";
  LoRa.enableCrc();
}

// -|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void runningLoRa(networkLora *gtw)
{
  static unsigned long tLoRaSend = 0;
  if ((millis() - tLoRaSend) > INTERVAL)
  {
    send_LoRa_Message("", gtw);
    tLoRaSend = millis();
  }
  gtw->packSize = LoRa.parsePacket();
  receive_LoRa_Message(gtw);
}

// -|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void send_LoRa_Message(String pack, networkLora *gtw)
{
  LoRa.beginPacket();

  LoRa.write(gtw->destAddr);
  LoRa.write(gtw->destAddr >> 8 & 0xFF);
  LoRa.write(gtw->destAddr >> 16 & 0xFF);
  LoRa.write(gtw->destAddr >> 24 & 0xFF);

  LoRa.write(gtw->localAddr);
  LoRa.write(gtw->localAddr >> 8 & 0xFF);
  LoRa.write(gtw->localAddr >> 16 & 0xFF);
  LoRa.write(gtw->localAddr >> 24 & 0xFF);

  LoRa.write(pack.length());
  LoRa.print(pack);

  LoRa.endPacket();

  gtw->packSize = pack.length();
  enviados++;
}

// -|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
String receive_LoRa_Message(networkLora *gtw)
{
  if (gtw->packSize == 0)
    return "";
  uint8_t to_who_addr[4] = {0};
  uint8_t sender_addr[4] = {0};
  uint16_t incomingLength = 0;
  gtw->incomingPack = "";
  pacotes++;
  for (register int i = 0; i < 4; i++)
    to_who_addr[i] = LoRa.read();
  for (register int i = 0; i < 4; i++)
    sender_addr[i] = LoRa.read();
  incomingLength = LoRa.read();
  while (LoRa.available())
    gtw->incomingPack += (char)LoRa.read();
  if (incomingLength != gtw->incomingPack.length())
  {
    erros++;
    return ""; // Pacote incosistente
  }
  if (asm_addr(to_who_addr) != gtw->localAddr || asm_addr(sender_addr) != gtw->destAddr)
    return ""; // Pacote ignorado
  return gtw->incomingPack;
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
  uint32_t newAddr;
  newAddr |= addr[0];
  newAddr |= addr[1] << 8;
  newAddr |= addr[2] << 16;
  newAddr |= addr[3] << 24;
  return newAddr;
}