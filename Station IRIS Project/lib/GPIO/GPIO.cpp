// Inclusões
#include "GPIO.h"

// Instâncias
DHT dht(DHTpin, typeDHT);

// Funções
bool readDHT(Sensor *temp)
{
  float aux_temp = temp->temperature, aux_humidity = temp->humidity;
  static unsigned long tempoLeituraDHT = 0;
  if ((xTaskGetTickCount() - tempoLeituraDHT) > readTime)
  {
    temp->temperature = dht.readTemperature();
    temp->humidity = dht.readHumidity();
    tempoLeituraDHT = xTaskGetTickCount();
  }
  if (isnan(temp->humidity) || isnan(temp->temperature))
  {
    temp->temperature = aux_temp;
    temp->humidity = aux_humidity;
    return 0;
  }
  return 1;
}

void valve(uint8_t state)
{
  if (state)
  {
    digitalWrite(valvePin1, 1);
    digitalWrite(valvePin2, 0);
  }
  else if (!state)
  {
    digitalWrite(valvePin1, 0);
    digitalWrite(valvePin2, 1);
  }
}

void resetClear()
{
  uint16_t counter = 0;
  while (digitalRead(pin_resetEEPROM))
  {
    vTaskDelay(1000);
    counter++;
    if (counter == resetTmrOF)
    {
      clear_EEPROM(0, EEPROM_SIZE);
      resetModule();
    }
  }
}
