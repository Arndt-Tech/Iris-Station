#include "main.hpp"

stt::Station iris;

void setup()
{
#if _DEBUG_MODE_
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\n******************************\n");
  Serial.println("**********DEBUG MODE**********");
  Serial.println("******************************\n");
#elif !_DEBUG_MODE_
#endif
  xTaskCreatePinnedToCore(taskReset, "taskReset", STACK(2048), NULL, PRIORITY(4), NULL, CORE(1));
  iris.begin();
  setupTasks();
}
void loop() {}