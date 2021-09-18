// Inclusões
#include "_DHT.h"

// Instâncias
DHT dht(DHTpin, typeDHT);

// Funções
bool readDHT(Sensor *temp)
{
  static unsigned long tempoLeituraDHT = 0;
  if ((millis() - tempoLeituraDHT) > 2000)
  {
    temp->temperature = dht.readTemperature();
    temp->humidity = dht.readHumidity();
    tempoLeituraDHT = millis();
  }
  if (isnan(temp->humidity) || isnan(temp->temperature))
    return 0;
  return 1;
}