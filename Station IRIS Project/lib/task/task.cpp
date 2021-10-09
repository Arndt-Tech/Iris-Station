// Inclusões
#include "task.h"

// Inicialização das tasks
void setupTasks()
{
  xTaskCreatePinnedToCore(taskReset, "taskReset", STACK(2048), NULL, PRIORITY(4), NULL, CORE(1));
  xTaskCreatePinnedToCore(taskReadData, "taskReadData", STACK(1024), NULL, PRIORITY(5), NULL, CORE(1));
  vTaskDelay(2000);
  xTaskCreatePinnedToCore(taskLoRa, "taskLora", STACK(8192), NULL, PRIORITY(5), NULL, CORE(0));
  xTaskCreatePinnedToCore(taskOled, "taskOled", STACK(4096), NULL, PRIORITY(3), NULL, CORE(1));
}

// Tasks
void taskLoRa(void *pvParameters)
{
  while (1)
    runningLoRa(&gateway);
}

void taskReadData(void *pvParameters)
{
  while (1)
  {
    error(readDHT(&gateway));
    valve(gateway.valveStatus);
    vTaskDelay(1);
  }
}

void taskOled(void *pvParameters)
{
  while (1)
  {
    dataBar(&gateway, "3", false, true);
    runnigSystem(&gateway, true, false);
    vTaskDelay(1);
  }
}

void taskReset(void *pvParameters)
{
  while (1)
  {
    resetClear();
    vTaskDelay(1);
  }
}

/*
void verify_LoRa_Timeout(networkFirebase *fb)
{
  static unsigned long tPend = 0;
  if ((xTaskGetTickCount() - tPend) >= loraTmt)
  {
    for (uint8_t i = 0; i < fb->TOTAL_STATIONS; i++)
    {
      if (fb->STATION_ID[i][RETURN] == "1")
        fb->STATION_ID[i][ISCONNECTED] = "false";
      else if (fb->STATION_ID[i][RETURN] == "0")
        fb->STATION_ID[i][ISCONNECTED] = "true";
    }
    tPend = xTaskGetTickCount();
  }
}
*/