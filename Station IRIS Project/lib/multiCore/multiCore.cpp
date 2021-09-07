// Inclusões
#include "multiCore.h"

// Funções
void taskLoRa(void *pvParameters) // Nucleo 0, falta adicionar semaforo binario.
{
  while (1)
  {
    unsigned long tLoRaSend = 0;
    if ((xTaskGetTickCount() - tLoRaSend) > INTERVAL)
    {
      send_LoRa_Message("");
      tLoRaSend = xTaskGetTickCount();
    }
    receive_LoRa_Message(LoRa.parsePacket());
    vTaskDelay(1);
  }
}