#include "_multiCore.h"



void taskLoRa (void *pvParameters) // Nucleo 0, falta adicionar semaforo binario.
{
  while(1)
  {
    if ((xTaskGetTickCount() - tLoRaSend) > INTERVAL)
    {
      send_LoRa_Message(pacoteDados);
      tLoRaSend = xTaskGetTickCount(); 
    }
    receive_LoRa_Message (LoRa.parsePacket());
    vTaskDelay(1);
  }
}
