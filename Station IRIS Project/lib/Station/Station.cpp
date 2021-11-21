#include "Station.hpp"

err::Error stt::Station::m_err;
com::BLE stt::Station::m_ble;
com::Lora stt::Station::m_lora;
per::GPS stt::Station::m_gps;
per::GPIO stt::Station::m_gpio;

/**
 * @brief Boot station.
 * 
 */
void stt::Station::begin()
{
  // Inicialização do Log
  cfg::Log::begin();
  // Inicialização do sistema de controle das GPIO's
  m_gpio.begin();
  // Inicialização do sistema de erros
  m_err.begin();
  // Inicialização do sistema GPS
  m_gps.begin();
  // Verificação dos dados do sistema
  checkSystem();
  // Inicialização da comunicação LoRa
  m_lora.begin();
  // Configuração geral completa
  err::Error::setError(err::Error::err_::Failure::INIT_OK);
}

/**
 * @brief Check system data integrity.
 * 
 * @return Returns error if data recovery fails. 
 */
err::Error::err_::Failure stt::Station::checkSystem()
{
  // Verifica EEPROM
  if (!cfg::Log::check(loChID_addr_min) || !cfg::Log::check(chID_addr_min))
  {
#if _DEBUG_MODE_
    Serial.println("EEPROM Vazia");
#elif _DEBUG_MODE_
#endif
    err::Error::setError(err::Error::err_::Failure::ERR_EMPTY_EEPROM);
    m_ble.begin(m_lora);

    // Aloca novos dados na EEPROM
    cfg::Log::write(m_lora.packet.transmit.get.senderAddr(), chID_addr_min);
    cfg::Log::write(m_lora.packet.transmit.get.localAddr(), loChID_addr_min);
    vTaskDelay(1000);
    spc::SpecialFunctions::resetModule();
  }
#if _DEBUG_MODE_
  Serial.println("EEPROM Com dados");
#elif _DEBUG_MODE_
#endif

  m_lora.packet.transmit.set.senderAddr(cfg::Log::read(chID_addr_min));
  m_lora.packet.transmit.set.localAddr(cfg::Log::read(loChID_addr_min));
  return err::Error::err_::Failure::NO_ERR;
}

/**
 * @brief Error module. 
 */
err::Error &stt::Station::mng::Error() { return m_err; }

/**
 * @brief Bluetooth module. 
 */
com::BLE &stt::Station::mng::BLE() { return m_ble; }

/**
 * @brief LoRa module. 
 */
com::Lora &stt::Station::mng::LoRa() { return m_lora; }

/**
 * @brief GPS module. 
 */
per::GPS &stt::Station::mng::GPS() { return m_gps; }

/**
 * @brief GPIO module. 
 */
per::GPIO &stt::Station::mng::GPIO() { return m_gpio; }