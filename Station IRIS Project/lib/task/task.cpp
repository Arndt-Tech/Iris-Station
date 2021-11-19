// 
#include "task.h"

//
extern stt::Station iris;

// 
void setupTasks()
{
  xTaskCreatePinnedToCore(taskReadData, "taskReadData", STACK(2048), NULL, PRIORITY(5), NULL, CORE(1));
  vTaskDelay(2000);
  xTaskCreatePinnedToCore(taskGPS, "taskGPS", STACK(4096), NULL, PRIORITY(2), NULL, CORE(1));
  xTaskCreatePinnedToCore(taskLoRa, "taskLora", STACK(8192), NULL, PRIORITY(5), NULL, CORE(0));
}

void setupResetTask(){}

// 
void taskLoRa(void *pvParameters)
{
  iris.manage.LoRa().startLoRa();
  while (1)
  {
    iris.manage.LoRa().run();
    iris.manage.GPIO().setValve(iris.manage.LoRa().getValveStatus());
    vTaskDelay(1);
  }
}

void taskReadData(void *pvParameters)
{
  while (1)
  {
    iris.manage.Error().setError(iris.manage.GPIO().getDHT(iris.manage.LoRa()));
    vTaskDelay(1);
  }
}

void taskReset(void *pvParameters)
{
  while (1)
  {
    iris.manage.GPIO().checkReset();
    vTaskDelay(1);
  }
}

void taskGPS(void *pvParameters)
{
  while(1)
  {
    iris.manage.GPS().getLocalization(iris.manage.LoRa());
    vTaskDelay(1);
  }
}