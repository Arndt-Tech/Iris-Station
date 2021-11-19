//
#include "Sensors.h"

DHT dht(DHTpin, typeDHT);

//
void per::GPIO::begin()
{
  pinMode(pin_resetEEPROM, INPUT);
  pinMode(valvePin1, OUTPUT);
  pinMode(valvePin2, OUTPUT);
  m_valve_status = false;
  dht.begin();
}

fle::Failure per::GPIO::getDHT(com::Lora &st)
{
  float aux_temp = st.packet.getTemperature();
  uint8_t aux_humidity = st.packet.getHumidity();
  static unsigned long tempoLeituraDHT = 0;
  if ((xTaskGetTickCount() - tempoLeituraDHT) > readTime)
  {
    st.packet.setTemperature(dht.readTemperature());
    st.packet.setHumidity((uint8_t)dht.readHumidity());
    tempoLeituraDHT = xTaskGetTickCount();
  }
  if (isnan(aux_humidity) || isnan(aux_temp))
#if _DEBUG_MODE_
  {
    Serial.println("Erro ao ler DHT" + String(typeDHT));
    Serial.println("Temperatura: " + String(st.packet.getTemperature()));
    Serial.println("Umidade: " + String(st.packet.getHumidity()));
    return fle::Failure::ERR_DHT_ISNAN;
  }
#elif !_DEBUG_MODE_
  {
    st.packet.setTemperature(aux_temp);
    st.packet.setHumidity(aux_humidity);
    return fle::Failure::ERR_DHT_ISNAN;
  }
#endif
  st.packet.setTemperature(aux_temp);
  st.packet.setHumidity(aux_humidity);
#if _DEBUG_MODE_
  static unsigned long tDebug = 0;
  if ((xTaskGetTickCount() - tDebug) > readTime)
  {
    Serial.println("Temperatura: " + String(st.packet.getTemperature()));
    Serial.println("Umidade: " + String(st.packet.getHumidity()));
    tDebug = xTaskGetTickCount();
  }
#elif !_DEBUG_MODE_
#endif
  return fle::Failure::NO_ERR;
}

void per::GPIO::setValve(bool status = 0)
{
  m_valve_status = status;
  if (m_valve_status)
  {
    digitalWrite(valvePin1, 1);
    digitalWrite(valvePin2, 0);
  }
  else if (!m_valve_status)
  {
    digitalWrite(valvePin1, 0);
    digitalWrite(valvePin2, 1);
  }
}

void per::GPIO::checkReset()
{
  uint16_t counter = 0;
  while (digitalRead(pin_resetEEPROM))
  {
#if _DEBUG_MODE_
    Serial.println("Reset pressionado por " + String(counter) + " segundos");
#elif !_DEBUG_MODE_
#endif
    vTaskDelay(1000);
    counter++;
    if (counter < resetTmrOF)
    {
      cfg::Log::clear(0, EEPROM_SIZE);
      spc::SpecialFunctions::resetModule();
    }
  }
}

bool per::GPIO::getValveStatus() { return m_valve_status; }
