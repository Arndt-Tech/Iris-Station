// Inclusões
#include "multiCore.h"



// Funções
void taskLoRa(void *pvParameters) // Nucleo 0, falta adicionar semaforo binario.
{
  while (1)
  {
    static unsigned long tLoRaSend = 0;
    if ((xTaskGetTickCount() - tLoRaSend) > INTERVAL)
    {
      send_LoRa_Message("", &gateway);
      tLoRaSend = xTaskGetTickCount();
    }
    gateway.packSize = LoRa.parsePacket();
    receive_LoRa_Message(&gateway);
    vTaskDelay(1);
  }
}