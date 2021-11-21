#include "GPS.h"

/**
 * @brief Initialize GPS.
 * 
 */
void per::GPS::begin() { m_softwareSerial.begin(9600, SWSERIAL_8N1, RX_GPS, TX_GPS, false, 256); }

/**
 * @brief Reads location and saves in LoRa shipping package.
 * 
 * @param st LoRa address of the station.
 * @return Operation success. 
 */
err::Error::err_::Failure per::GPS::getLocalization(com::Lora &st)
{
  while (m_softwareSerial.available() > 0)
    if (m_data.encode(m_softwareSerial.read()))
    {
      if (m_data.location.isValid())
      {
        m_lat = m_data.location.lat();
        m_lon = m_data.location.lng();
        st.packet.transmit.set.latitude(m_lat);
        st.packet.transmit.set.longitude(m_lon);
        m_status = 1;
      }
      else
        return err::Error::err_::Failure::ERR_INVALID_GPS_LOCATION;
    }
  if (xTaskGetTickCount() > 5000 && m_data.charsProcessed() < 10)
  {
    m_status = 0;
    return err::Error::err_::Failure::WAR_UNKNOWN_GPS_FUNCTIONING;
  }
  return err::Error::err_::Failure::NO_ERR;
}

/**
 * @brief Get gps status.
 * 
 * @return Gps status. 
 */
uint8_t per::GPS::getStatus() { return m_status; }

/**
 * @brief Get latitude.
 * 
 * @return Latitude.
 */
double per::GPS::getLatitude() { return m_lat; }

/**
 * @brief Get longitude.
 * 
 * @return Longitude. 
 */
double per::GPS::getLongitude() { return m_lon; }
