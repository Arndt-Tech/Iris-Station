// Inclusões
#include "GPIO.h"

// Instâncias
DHT dht(DHTpin, typeDHT);

// Funções
uint8_t readDHT(networkLora *gtw)
{
  float aux_temp = gtw->packet.temperature;
  uint8_t aux_humidity = gtw->packet.humidity;
  static unsigned long tempoLeituraDHT = 0;
  if ((xTaskGetTickCount() - tempoLeituraDHT) > readTime)
  {
    gtw->packet.temperature = dht.readTemperature();
    gtw->packet.humidity = (uint8_t)dht.readHumidity();
    tempoLeituraDHT = xTaskGetTickCount();
  }
  if (isnan(gtw->packet.humidity) || isnan(gtw->packet.temperature))
  {
    gtw->packet.temperature = aux_temp;
    gtw->packet.humidity = aux_humidity;
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
