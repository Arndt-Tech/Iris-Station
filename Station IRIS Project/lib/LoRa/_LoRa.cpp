// Inclusões
#include "_LoRa.h"

// Variáveis externas
int erros, pacotes, enviados;

void setupLoRa(lora_com *gtw)
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
void runningLoRa(lora_com *gtw)
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
void send_LoRa_Message(String dados, lora_com *gtw)
{
  LoRa.beginPacket();
  LoRa.write(gtw->destAddr);
  LoRa.write(gtw->localAddr);
  LoRa.write(dados.length());
  LoRa.print(dados);
  LoRa.endPacket();
  gtw->packSize = dados.length();
  enviados++;
}

// -|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void receive_LoRa_Message(lora_com *gtw)
{
  if (gtw->packSize == 0)
    return;
  int to_who_addr = 0;
  char sender_addr = 0;
  char incomingLength = 0;
  gtw->incomingPack = "";
  pacotes++;
  // Bytes de protocolo
  to_who_addr = LoRa.read();
  sender_addr = LoRa.read();
  incomingLength = LoRa.read();
  // Concatenação dos dados na string "incomingPack"
  while (LoRa.available())
    gtw->incomingPack += (char)LoRa.read();
  if (incomingLength != gtw->incomingPack.length())
  {
    erros++; 
    return;
  }
  if (to_who_addr != gtw->localAddr || sender_addr != gtw->destAddr)
    return; // Pacote ignorado
  
  
  /*
  Serial.println("Recebido de: 0x" + String(sender, HEX));
  Serial.println("Enviado para: 0x" + String(requisitor, HEX));
  Serial.println("Tamanho do pacote: " + String(incomingLength));
  Serial.println("Pacote recebido: " + incoming);
  Serial.println("dBm: " + String(LoRa.packetRssi()));
  Serial.println("Ruido: " + String(LoRa.packetSnr()));
  Serial.println();
  */
}