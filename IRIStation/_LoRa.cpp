#include "_LoRa.h"
#include "_OLED.h"
// -|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void setupLoRa()
{
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI00);
  if (!LoRa.begin(BAND))
  {
    Serial.println("Erro ao inicializar LoRa!");
    erroSetupLoRa();
    while (1);
  }
  LoRa.enableCrc();
}

// -|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void runningLoRa()
{
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
  LoRa.write(comLine);
  LoRa.write(localAddress);
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
    incoming += (char) LoRa.read();
  if (incomingLength != incoming.length())
  {
    erros++;
    return;
  }
  if (requisitor != localAddress && requisitor != comLine)
  {
    // Serial.println ("Pacote ignorado");
    return;
  }
  /*
  Serial.println("Recebido de: 0x" + String(sender, HEX));
  Serial.println("Enviado para: 0x" + String(requisitor, HEX));
  Serial.println("Tamanho do pacote: " + String(incomingLength));
  Serial.println("Pacote recebido: " + incoming);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Ruído: " + String(LoRa.packetSnr()));
  Serial.println();
  */
}
