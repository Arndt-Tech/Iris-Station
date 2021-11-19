// Inclusões
#include "_LoRa.h"

void com::Lora::begin()
{
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI00);
  if (!LoRa.begin(BAND))
  {
    err::Error::setError(fle::Failure::ERR_INITIALIZING_LORA);
    while (1)
    {
    };
  }
  LoRa.setSpreadingFactor(SF);
  //LoRa.setSignalBandwidth(BW);
  //LoRa.setCodingRate4(CR);
  //LoRa.setPreambleLength(PL);
  //LoRa.setSyncWord(SW);
  LoRa.enableCrc();
  m_status = 1;
  m_signal = -164;
  m_received = 0;
  m_valveStatus = 0;
}

void com::Lora::run()
{
  if (m_status)
  {
    if (m_received)
      sendPackage();
    err::Error::setError(getPackage());
  }
}

void com::Lora::sendPackage()
{
  // Inicio do pacote
  LoRa.beginPacket();
  // Envia ID's
  packID();
  // Envia dados de Sensores
  packSensors();
  // Envia dados de GPS (Se corresponder com tipo de pacote)
  packGPS();
  // Envia tamanho do pacote
  LoRa.write(packet.getPacketLenght());
  // Fim do pacote
  LoRa.endPacket();
  // Flag de requisição zerada
  m_received = 0;
}

fle::Failure com::Lora::getPackage()
{
  uint8_t packSize = LoRa.parsePacket();
  if (packSize == 0)
    return fle::Failure::WAR_NO_LORA_PACKAGES;
  uint8_t to_who_addr[4] = {0};
  uint8_t sender_addr[4] = {0};
  uint16_t incomingLength = 0;
  uint8_t __valveStatus = 0;
  for (register uint8_t i = 0; i < 4; i++)
    to_who_addr[i] = LoRa.read();
  for (register uint8_t i = 0; i < 4; i++)
    sender_addr[i] = LoRa.read();
  if (spc::SpecialFunctions::asm_addr(to_who_addr) != packet.getLocalAddr() || spc::SpecialFunctions::asm_addr(sender_addr) != packet.getDestAddr())
#if _DEBUG_MODE_
  {
    Serial.println("Pacote ignorado");
    Serial.println("Endereco destino: " + String(spc::SpecialFunctions::asm_addr(to_who_addr)));
    Serial.println("Endereco remetente: " + String(spc::SpecialFunctions::asm_addr(sender_addr)));
    return fle::Failure::WAR_UNKNOWN_PACKAGE;
  }
#elif !_DEBUG_MODE_
    return fle::Failure::WAR_UNKNOWN_PACKAGE;
#endif
  __valveStatus = LoRa.read();
  incomingLength = LoRa.read();
  if (incomingLength != packSize)
#if _DEBUG_MODE_
  {
    Serial.println("Pacote inconsistente");
    Serial.println("Tamanho informado do pacote: " + String(incomingLength));
    Serial.println("Tamanho esperado do pacote: " + String(packSize));
    return fle::Failure::ERR_INCONSISTENT_LORA_PACKAGE;
  }
#elif !_DEBUG_MODE_
    return fle::Failure::ERR_INCONSISTENT_LORA_PACKAGE;
#endif
  m_received = 1;
  m_signal = LoRa.packetRssi();
  m_valveStatus = __valveStatus;
#if _DEBUG_MODE_
  Serial.println();
  Serial.println("Endereco destino: " + String(spc::SpecialFunctions::asm_addr(to_who_addr)));
  Serial.println("Endereco remetente: " + String(spc::SpecialFunctions::asm_addr(sender_addr)));
  Serial.println("Valvula: " + String(__valveStatus));
  Serial.println("Tamanho informado do pacote: " + String(incomingLength));
  Serial.println("Tamanho esperado do pacote: " + String(packSize));
  Serial.println("Sinal: " + String(m_signal));
  Serial.println();
#elif !_DEBUG_MODE_
#endif
  return fle::Failure::NO_ERR;
}

void com::Lora::packID() const
{
  // Endereço destino
  LoRa.write(packet.getDestAddr());
  LoRa.write(packet.getDestAddr() >> 8 & 0xFF);
  LoRa.write(packet.getDestAddr() >> 16 & 0xFF);
  LoRa.write(packet.getDestAddr() >> 24 & 0xFF);
  // Endereço local
  LoRa.write(packet.getLocalAddr());
  LoRa.write(packet.getLocalAddr() >> 8 & 0xFF);
  LoRa.write(packet.getLocalAddr() >> 16 & 0xFF);
  LoRa.write(packet.getLocalAddr() >> 24 & 0xFF);
}

void com::Lora::packSensors() const
{
  // Umidade
  LoRa.write(packet.getHumidity());
  // Temperatura
  LoRa.write(packet.getHumidity());
  LoRa.write(packet.getHumidity() >> 8 & 0xFF);
}

void com::Lora::packGPS() const
{
  // Latitude
  LoRa.write(packet.getLatitude());
  LoRa.write(packet.getLatitude() >> 8 & 0xFF);
  LoRa.write(packet.getLatitude() >> 16 & 0xFF);
  LoRa.write(packet.getLatitude() >> 24 & 0xFF);
  // Longitude
  LoRa.write(packet.getLongitude());
  LoRa.write(packet.getLongitude() >> 8 & 0xFF);
  LoRa.write(packet.getLongitude() >> 16 & 0xFF);
  LoRa.write(packet.getLongitude() >> 24 & 0xFF);
}

uint8_t com::Lora::getValveStatus() const { return m_valveStatus; }

void com::Lora::startLoRa() { m_status = 1; }

void com::Lora::stopLoRa() { m_status = 0; }

aux::loraPackage::loraPackage()
{
  inThePackage.m_destAddr = 0;
  inThePackage.m_localAddr = 0;
  inThePackage.m_temperature = 0;
  inThePackage.m_humidity = 0;
  inThePackage.m_latitude = 0;
  inThePackage.m_longitude = 0;
  inThePackage.m_packageLen = 0;
}

uint32_t aux::loraPackage::getLocalAddr() const { return inThePackage.m_localAddr; }

uint32_t aux::loraPackage::getDestAddr() const { return inThePackage.m_destAddr; }

int16_t aux::loraPackage::getTemperature() const { return inThePackage.m_temperature; }

uint8_t aux::loraPackage::getHumidity() const { return inThePackage.m_humidity; }

uint8_t aux::loraPackage::getPacketLenght() const { return sizeof(inThePackage); }

int32_t aux::loraPackage::getLatitude() const { return inThePackage.m_latitude; }

int32_t aux::loraPackage::getLongitude() const { return inThePackage.m_longitude; }

void aux::loraPackage::setLocalAddr(uint32_t value) { inThePackage.m_localAddr = value; }

void aux::loraPackage::setDestAddr(uint32_t value) { inThePackage.m_destAddr = value; }

void aux::loraPackage::setTemperature(float value) { inThePackage.m_temperature = value * 1E1; }

void aux::loraPackage::setHumidity(uint8_t value) { inThePackage.m_humidity = value; }

void aux::loraPackage::setLatitude(double value) { inThePackage.m_latitude = value * -1E6; }

void aux::loraPackage::setLongitude(double value) { inThePackage.m_longitude = value * -1E6; }
