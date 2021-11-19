#include "Station.hpp"

err::Error stt::Station::m_err;
com::BLE stt::Station::m_ble;
com::Lora stt::Station::m_lora;
per::GPS stt::Station::m_gps;
per::GPIO stt::Station::m_gpio;

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
  checkSystem(m_lora);
  // Inicialização da comunicação LoRa
  m_lora.begin();
  // Configuração geral completa
  err::Error::setError(fle::Failure::INIT_OK);
}

fle::Failure stt::Station::checkSystem(com::Lora &st)
{
  // Verifica EEPROM
  if (!cfg::Log::check(loChID_addr_min) || !cfg::Log::check(chID_addr_min))
  {
#if _DEBUG_MODE_
    Serial.println("EEPROM Vazia");
#elif _DEBUG_MODE_
#endif
    err::Error::setError(fle::Failure::WAR_EMPTY_EEPROM);
    m_ble.begin();
    m_ble.sendID(st);

    // Aloca novos dados na EEPROM
    cfg::Log::write(st.packet.getDestAddr(), chID_addr_min);
    cfg::Log::write(st.packet.getLocalAddr(), loChID_addr_min);
    vTaskDelay(1000);
    spc::SpecialFunctions::resetModule();
  }
#if _DEBUG_MODE_
  Serial.println("EEPROM Com dados");
#elif _DEBUG_MODE_
#endif
  st.packet.setDestAddr(cfg::Log::read(chID_addr_min));
  st.packet.setLocalAddr(cfg::Log::read(loChID_addr_min));
  return fle::Failure::NO_ERR;
}

err::Error &stt::Station::mng::Error() { return m_err; }

com::BLE &stt::Station::mng::BLE() { return m_ble; }

com::Lora &stt::Station::mng::LoRa() { return m_lora; }

per::GPS &stt::Station::mng::GPS() { return m_gps; }

per::GPIO &stt::Station::mng::GPIO() { return m_gpio; }