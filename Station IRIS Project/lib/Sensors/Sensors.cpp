//
#include "Sensors.h"

DHT dht(DHTpin, typeDHT);

double per::GPIO::m_temperature = 0;
double per::GPIO::m_humidity = 0;
uint8_t per::GPIO::m_valve_status = false;
uint8_t per::GPIO::m_dht_status = false;
uint8_t per::GPIO::m_temperature_unit = false;

void per::GPIO::begin()
{
  pinMode(pin_resetEEPROM, INPUT);
  pinMode(valvePin1, OUTPUT);
  pinMode(valvePin2, OUTPUT);
  dht.begin();
}

err::Error::err_::Failure per::GPIO::snsr::readDHT(com::Lora &st)
{
  float aux_temp = st.packet.transmit.get.temperature();
  uint8_t aux_humidity = st.packet.transmit.get.humidity();
  static unsigned long tempoLeituraDHT = 0;
  if ((xTaskGetTickCount() - tempoLeituraDHT) > readTime)
  {
    m_temperature = dht.readTemperature(m_temperature_unit);
    m_humidity = dht.readHumidity();
    tempoLeituraDHT = xTaskGetTickCount();
  }
  if (isnan(aux_humidity) || isnan(aux_temp))
  {
    m_temperature = aux_temp;
    m_humidity = aux_humidity;
    m_dht_status = 0;
    return err::Error::err_::Failure::ERR_DHT_ISNAN;
  }
  m_dht_status = 1;
  st.packet.transmit.set.temperature(m_temperature);
  st.packet.transmit.set.humidity((uint8_t)m_humidity);
  return err::Error::err_::Failure::NO_ERR;
}

double per::GPIO::snsr::getTemperature() { return m_temperature; }

double per::GPIO::snsr::getHumidity() { return m_humidity; }

uint8_t per::GPIO::snsr::status() { return m_dht_status; }

void per::GPIO::snsr::setUnit(bool unit) { m_temperature_unit = unit; }

void per::GPIO::vlv::setValve(uint8_t status = 0)
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

uint8_t per::GPIO::vlv::status() { return (digitalRead(valvePin1) && !digitalRead(valvePin2)) == true ? true : false; }

void per::GPIO::oth::checkReset()
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
