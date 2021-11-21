// Inclusões
#include "_LoRa.h"

uint8_t com::Lora::m_received = 0;
aux::loraPackage com::Lora::packet;
LoRaClass com::Lora::m_settings;
aux::loraPackage::trns aux::loraPackage::send;
aux::loraPackage::rcpt aux::loraPackage::rec;

/**
 * @brief Initialize LoRa communication.
 * 
 */
void com::Lora::begin()
{
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI00);
  if (!LoRa.begin(BAND))
  {
    err::Error::setError(err::Error::err_::Failure::ERR_INITIALIZING_LORA);
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
  m_received = 0;
}

/**
 * @brief LoRa duplex communication operation.
 * 
 */
void com::Lora::opr::duplex()
{
  if (m_received)
    com::Lora::opr::sendPackage();
  err::Error::setError(com::Lora::opr::readPackage());
}

/**
 * @brief Send LoRa packages.
 * 
 */
void com::Lora::opr::sendPackage()
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
  LoRa.write(packet.transmit.get.size());
  // Fim do pacote
  LoRa.endPacket();
  // Flag de requisição zerada
  m_received = 0;
}

/**
 * @brief Reads LoRa packages.
 * 
 * @return Operation success.
 */
err::Error::err_::Failure com::Lora::opr::readPackage()
{
  uint8_t packSize = LoRa.parsePacket();
  if (packSize == 0)
    return err::Error::err_::Failure::WAR_NO_LORA_PACKAGES;
  uint8_t to_who_addr[4] = {0};
  uint8_t sender_addr[4] = {0};
  uint16_t incomingLength = 0;
  uint8_t __valveStatus = 0;
  for (register uint8_t i = 0; i < 4; i++)
    to_who_addr[i] = LoRa.read();
  for (register uint8_t i = 0; i < 4; i++)
    sender_addr[i] = LoRa.read();
  if (spc::SpecialFunctions::asm_addr(to_who_addr) != packet.transmit.get.localAddr() || spc::SpecialFunctions::asm_addr(sender_addr) != packet.transmit.get.senderAddr())
#if _DEBUG_MODE_
  {
    Serial.println("ERRO LORA - Pacote ignorado");
    Serial.println("Endereco destino: " + String(spc::SpecialFunctions::asm_addr(to_who_addr)));
    Serial.println("Endereco remetente: " + String(spc::SpecialFunctions::asm_addr(sender_addr)));
    return err::Error::err_::Failure::WAR_UNKNOWN_PACKAGE;
  }
#elif !_DEBUG_MODE_
    return err::Error::err_::Failure::WAR_UNKNOWN_PACKAGE;
#endif
  __valveStatus = LoRa.read();
  incomingLength = LoRa.read();
  if (incomingLength != packSize)
#if _DEBUG_MODE_
  {
    Serial.println("ERRO LORA - Pacote inconsistente");
    Serial.println("Tamanho informado do pacote: " + String(incomingLength));
    Serial.println("Tamanho esperado do pacote: " + String(packSize));
    return err::Error::err_::Failure::ERR_INCONSISTENT_LORA_PACKAGE;
  }
#elif !_DEBUG_MODE_
    return err::Error::err_::Failure::ERR_INCONSISTENT_LORA_PACKAGE;
#endif
  packet.rec.inThePackage.m_receiverAddr = spc::SpecialFunctions::asm_addr(to_who_addr);
  packet.rec.inThePackage.m_senderAddr = spc::SpecialFunctions::asm_addr(to_who_addr);
  packet.rec.inThePackage.m_signal = LoRa.packetRssi();
  packet.rec.inThePackage.m_valveStatus = __valveStatus;
  packet.rec.inThePackage.m_size = incomingLength;
  m_received = 1;
  return err::Error::err_::Failure::NO_ERR;
}

void com::Lora::packID()
{
  // Endereço destino
  LoRa.write(packet.transmit.get.senderAddr());
  LoRa.write(packet.transmit.get.senderAddr() >> 8 & 0xFF);
  LoRa.write(packet.transmit.get.senderAddr() >> 16 & 0xFF);
  LoRa.write(packet.transmit.get.senderAddr() >> 24 & 0xFF);
  // Endereço local
  LoRa.write(packet.transmit.get.localAddr());
  LoRa.write(packet.transmit.get.localAddr() >> 8 & 0xFF);
  LoRa.write(packet.transmit.get.localAddr() >> 16 & 0xFF);
  LoRa.write(packet.transmit.get.localAddr() >> 24 & 0xFF);
}

void com::Lora::packSensors()
{
  // Umidade
  LoRa.write(packet.transmit.get.humidity());
  // Temperatura
  LoRa.write(packet.transmit.get.humidity());
  LoRa.write(packet.transmit.get.humidity() >> 8 & 0xFF);
}

void com::Lora::packGPS()
{
  // Latitude
  LoRa.write(packet.transmit.get.latitude());
  LoRa.write(packet.transmit.get.latitude() >> 8 & 0xFF);
  LoRa.write(packet.transmit.get.latitude() >> 16 & 0xFF);
  LoRa.write(packet.transmit.get.latitude() >> 24 & 0xFF);
  // Longitude
  LoRa.write(packet.transmit.get.longitude());
  LoRa.write(packet.transmit.get.longitude() >> 8 & 0xFF);
  LoRa.write(packet.transmit.get.longitude() >> 16 & 0xFF);
  LoRa.write(packet.transmit.get.longitude() >> 24 & 0xFF);
}

/**
 * @brief Advanced Settings.
 */
LoRaClass &com::Lora::advancedSettings() { return m_settings; }

/**
 * @brief Get request status from gateway.
 * 
 * @return Request status.
 */
uint8_t com::Lora::checkRequest() { return m_received; }

aux::loraPackage::loraPackage()
{
  send.inThePackage.m_senderAddr = 0;
  send.inThePackage.m_localAddr = 0;
  send.inThePackage.m_temperature = 0;
  send.inThePackage.m_humidity = 0;
  send.inThePackage.m_latitude = 0;
  send.inThePackage.m_longitude = 0;
  send.inThePackage.m_size = 0;
  rec.inThePackage.m_receiverAddr = 0;
  rec.inThePackage.m_senderAddr = 0;
  rec.inThePackage.m_valveStatus = 0;
  rec.inThePackage.m_signal = -164;
  rec.inThePackage.m_size = 0;
}

/**
 * @brief Get local address.
 */
uint32_t aux::loraPackage::trnsf::gt::localAddr() const { return send.inThePackage.m_localAddr; }

/**
 * @brief Get sender address.
 */
uint32_t aux::loraPackage::trnsf::gt::senderAddr() const { return send.inThePackage.m_senderAddr; }

/**
 * @brief Get temperature.
 */
int16_t aux::loraPackage::trnsf::gt::temperature() const { return send.inThePackage.m_temperature; }

/**
 * @brief Get humidity.
 */
uint8_t aux::loraPackage::trnsf::gt::humidity() const { return send.inThePackage.m_humidity; }

/**
 * @brief Get the last package size.
 */
uint8_t aux::loraPackage::trnsf::gt::size() const { return sizeof(send.inThePackage); }

/**
 * @brief Get latitude.
 */
int32_t aux::loraPackage::trnsf::gt::latitude() const { return send.inThePackage.m_latitude; }

/**
 * @brief Get longitude.
 */
int32_t aux::loraPackage::trnsf::gt::longitude() const { return send.inThePackage.m_longitude; }

/**
 * @brief Set local address.
 */
void aux::loraPackage::trnsf::st::localAddr(uint32_t value) { send.inThePackage.m_localAddr = value; }

/**
 * @brief Set sender address.
 */
void aux::loraPackage::trnsf::st::senderAddr(uint32_t value) { send.inThePackage.m_senderAddr = value; }

/**
 * @brief Set temperature.
 */
void aux::loraPackage::trnsf::st::temperature(float value) { send.inThePackage.m_temperature = value * 1E1; }

/**
 * @brief Set humidity.
 */
void aux::loraPackage::trnsf::st::humidity(uint8_t value) { send.inThePackage.m_humidity = value; }

/**
 * @brief Set latitude.
 */
void aux::loraPackage::trnsf::st::latitude(double value) { send.inThePackage.m_latitude = value * -1E6; }

/**
 * @brief Set longitude.
 */
void aux::loraPackage::trnsf::st::longitude(double value) { send.inThePackage.m_longitude = value * -1E6; }

/**
 * @brief Get the last address from the receiver. 
 */
uint32_t aux::loraPackage::rcptf::gt::receiverAddr() const { return rec.inThePackage.m_receiverAddr; }

/**
 * @brief Get the last sender's address.
 */
uint32_t aux::loraPackage::rcptf::gt::senderAddr() const { return rec.inThePackage.m_senderAddr; }

/**
 * @brief Get the last status of the valve.
 */
uint8_t aux::loraPackage::rcptf::gt::valveStatus() const { return rec.inThePackage.m_valveStatus; }

/**
 * @brief Get the sign from the last package.
 */
int16_t aux::loraPackage::rcptf::gt::signal() const { return rec.inThePackage.m_signal; }

/**
 * @brief Get the latest package size.
 */
uint8_t aux::loraPackage::rcptf::gt::size() const { return rec.inThePackage.m_size; }
