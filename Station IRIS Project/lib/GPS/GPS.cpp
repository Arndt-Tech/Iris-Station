#include "GPS.h"

void per::GPS::begin() { softwareSerial.begin(9600, SWSERIAL_8N1, RX_GPS, TX_GPS, false, 256); }

fle::Failure per::GPS::getLocalization(com::Lora &st)
{
  while (softwareSerial.available() > 0)
    if (data.encode(softwareSerial.read()))
    {
      if (data.location.isValid())
      {
        st.packet.setLatitude(data.location.lat());
        st.packet.setLongitude(data.location.lng());
      }
      else
        return fle::Failure::WAR_INVALID_GPS_LOCATION;
    }
  if (xTaskGetTickCount() > 5000 && data.charsProcessed() < 10)
#if _DEBUG_MODE_
  {
    Serial.println("Erro GPS, dados desconhecidos");
    return fle::Failure::ERR_UNKNOWN_GPS_FUNCTIONING;
  }
#elif !_DEBUG_MODE_
    return fle::Failure::ERR_UNKNOWN_GPS_FUNCTIONING;
#endif

  return fle::Failure::NO_ERR;
}
