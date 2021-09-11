// Inclusões
#include "_LoRa.h"

// Variáveis
char localAddr = 0xBB; // Endereço local
char destAddr  = 0xFF; // Endereço da destino
int requisitor;
char sender;
char incomingLength;
String incoming = "";
int erros, pacotes, enviados;

void setupLoRa()
{
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI00);
  if (!LoRa.begin(BAND))
  {
    Serial.println("Erro ao inicializar LoRa!");
    while (1)
      ;
  }
  LoRa.enableCrc();
}

// -|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void runningLoRa()
{
  static unsigned long tLoRaSend = 0;
  if ((millis() - tLoRaSend) > INTERVAL)
  {
    send_LoRa_Message("");
    tLoRaSend = millis();
  }
  receive_LoRa_Message(LoRa.parsePacket());
}

// -|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void send_LoRa_Message(String dados)
{
  LoRa.beginPacket();
  LoRa.write(destAddr);
  LoRa.write(localAddr);
  LoRa.write(dados.length());
  LoRa.print(dados);
  LoRa.endPacket();
  enviados++;
}

// -|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void receive_LoRa_Message(int packetSize)
{
  if (packetSize == 0)
    return;
  pacotes++;
  // Bytes de protocolo
  requisitor = LoRa.read();
  sender = LoRa.read();
  incomingLength = LoRa.read();
  incoming = "";
  // Concatenação dos dados na string "incoming"
  while (LoRa.available())
    incoming += (char)LoRa.read();
  if (incomingLength != incoming.length())
  {
    erros++;
    return;
  }
  if (requisitor != localAddr && requisitor != destAddr)
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