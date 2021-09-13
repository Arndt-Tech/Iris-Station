// Arquivos de funções
#include "_LoRa.h"
#include "_OLED.h"
#include "_configBegin.h"
#include "_multiCore.h"

// Mapeamento de hardware
int pinAnalog = 26;

unsigned long tLoRaSend;
float temp = 23.15;
int umi = 45;
String pacoteDados = ("t" + String(temp) + "u" + String(umi));
 

void setup() 
{
  configBegin();
  
  setupOLED();

  waitingStart();

  setupBluetooth();

  bluetoothConfig();

  
  
  setupLoRa();
  
  xTaskCreatePinnedToCore(taskLoRa, "taskLora", 10000, NULL, 3, NULL, 0);
}


void loop() 
{
  interfaceDebugLora();
  pacoteDados = ("t" + String(temp) + "u" + String(umi));
  umi++;
  if (umi > 99)umi = 0;
  Serial.println(pacoteDados);
  vTaskDelay (1000);
}
