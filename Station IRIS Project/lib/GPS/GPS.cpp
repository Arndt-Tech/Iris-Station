#include "GPS.h"

void setupSoftwareSerial(GPS *gps)
{
  gps->softSerial.begin(9600, SWSERIAL_8N1, RX_GPS, TX_GPS, false, 256);
}

void locationRead(GPS *gps, networkLora *gtw)
{
  while (gps->softSerial.available() > 0)
    if (gps->data.encode(gps->softSerial.read()))
    {
      if (gps->data.location.isValid())
      {
        gtw->packetAux.latitute = gps->data.location.lat();
        gtw->packetAux.longitude = gps->data.location.lng();
        Serial.println("Latitude: " + String(gps->data.location.lat(), 6));
        Serial.println("Longitude: " + String(gps->data.location.lng(), 6));
      }
      else Serial.println("Localizacao invalida!");
    }
  if (xTaskGetTickCount() > 5000 && gps->data.charsProcessed() < 10)
    Serial.println("Erro no GPS, verificar conexao");
}
