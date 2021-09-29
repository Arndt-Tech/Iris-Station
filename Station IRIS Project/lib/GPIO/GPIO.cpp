// Inclusões
#include "GPIO.h"

// Instâncias
DHT dht(DHTpin, typeDHT);

// Funções
bool readDHT(Sensor *temp)
{
  float aux_temp = temp->temperature, aux_humidity = temp->humidity;
  static unsigned long tempoLeituraDHT = 0;
  if ((millis() - tempoLeituraDHT) > 2000)
  {
    temp->temperature = dht.readTemperature();
    temp->humidity = dht.readHumidity();
    tempoLeituraDHT = millis();
  }
  if (isnan(temp->humidity) || isnan(temp->temperature))
  {
    temp->temperature = aux_temp;
    temp->humidity = aux_humidity;
    return 0;
  }
  return 1;
}

