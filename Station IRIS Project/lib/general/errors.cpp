#include "errors.h"

// Funções
void setupBuzzer()
{
  pinMode(buzzerPin, OUTPUT);
  ledcAttachPin(buzzerPin, buzzChannel(0));
  ledcSetup(buzzChannel(0), 1000, 10);
}

err error(err error)
{
  uint8_t long_buzz = 0, short_buzz = 1;
  switch (error)
  {
  // 0 Longos 1 Curtos --> Inicialização realizada com sucesso
  case INIT_OK:
    long_buzz = 0;
    short_buzz = 1;
    break;

  // 0 Longos 0 Curtos --> Nenhum erro
  case NOT_ERROR:
    long_buzz = 0;
    short_buzz = 0;
    break;

  // 0 Longos 4 Curtos --> Erro no sensor de temperatura DHT
  case ERR_DHT_ISNAN:
    long_buzz = 0;
    short_buzz = 4;
    break;

  // 1 Longos 2 Curtos --> Erro ao inicializar rede LoRa
  case ERR_INITIALIZING_LORA:
    long_buzz = 1;
    short_buzz = 2;
    break;

  // 0 Longos 2 Curtos --> Pacote LoRa fora do protocolo esperado
  case WAR_PACKET_OUT_OF_PROTOCOL:
    long_buzz = 0;
    short_buzz = 2;
    break;

  // 0 Longos 3 Curtos --> Pacote LoRa inconsistente
  case WAR_INCONSISTENT_LORA_PACKAGE:
    long_buzz = 0;
    short_buzz = 3;
    break;

    // 1 Longos 1 Curtos --> Sem dados na EEPROM
  case WAR_EMPTY_EEPROM:
    long_buzz = 1;
    short_buzz = 1;
    break;
  }
  for (uint8_t i = 0; i < long_buzz; i++)
    long_bip();
  for (uint8_t i = 0; i < short_buzz; i++)
    short_bip();
  return error;
}

void long_bip()
{
  ledcWrite(buzzChannel(0), 1000);
  vTaskDelay(700);
  ledcWrite(buzzChannel(0), 0);
  vTaskDelay(350);
}

void short_bip()
{
  ledcWrite(buzzChannel(0), 1000);
  vTaskDelay(200);
  ledcWrite(buzzChannel(0), 0);
  vTaskDelay(200);
}