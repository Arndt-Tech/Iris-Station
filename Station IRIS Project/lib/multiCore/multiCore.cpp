// Inclusões
#include "multiCore.h"
// Nucleo 0, falta adicionar semaforo binario.

// Funções
void taskLoRa(void *pvParameters)
{
  while (1)
  {
    static unsigned long tLoRaSend = 0;
    if ((xTaskGetTickCount() - tLoRaSend) > INTERVAL)
    {
      send_LoRa_Message(&gateway, &sensor);
      tLoRaSend = xTaskGetTickCount();
    }
    gateway.packSize = LoRa.parsePacket();
    receive_LoRa_Message(&gateway, &hardware);
    vTaskDelay(1);
  }
}

void setupMultiCore(BaseType_t core) { xTaskCreatePinnedToCore(taskLoRa, "taskLora", 10000, NULL, 3, NULL, core); }
