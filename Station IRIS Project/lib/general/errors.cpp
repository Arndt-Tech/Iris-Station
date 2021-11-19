#include "errors.h"

void err::Error::begin()
{
  pinMode(buzzerPin, OUTPUT);
  ledcAttachPin(buzzerPin, buzzChannel(0));
  ledcSetup(buzzChannel(0), 2000, 10);
}

fle::Failure err::Error::setError(fle::Failure error)
{
  uint8_t long_buzz = 0, short_buzz = 0;
  switch (error)
  {
  // 0 Longos 0 Curtos --> Nenhum erro
  case fle::Failure::NO_ERR:
    return fle::Failure::NO_ERR;

  // 0 Longos 1 Curtos --> Inicialização realizada com sucesso
  case fle::Failure::INIT_OK:
    long_buzz = 0;
    short_buzz = 1;
    break;

  // 0 Longos 2 Curtos --> Erro no sensor de temperatura DHT
  case fle::Failure::ERR_DHT_ISNAN:
    long_buzz = 0;
    short_buzz = 2;
    break;

  // 0 Longos 3 Curtos --> Pacote LoRa inconsistente
  case fle::Failure::ERR_INCONSISTENT_LORA_PACKAGE:
    long_buzz = 0;
    short_buzz = 3;
    break;

  // 1 Longos 1 Curtos --> Sem dados na EEPROM
  case fle::Failure::WAR_EMPTY_EEPROM:
    long_buzz = 1;
    short_buzz = 1;
    break;

  // 1 Longos 2 Curtos --> Erro ao inicializar rede LoRa
  case fle::Failure::ERR_INITIALIZING_LORA:
    long_buzz = 1;
    short_buzz = 2;
    break;
  }
  for (uint8_t i = 0; i < long_buzz; i++)
    long_bip();
  for (uint8_t i = 0; i < short_buzz; i++)
    short_bip();
  return error;
}

void err::Error::long_bip()
{
  ledcWrite(buzzChannel(0), 256);
  vTaskDelay(700);
  ledcWrite(buzzChannel(0), 0);
  vTaskDelay(350);
}

void err::Error::short_bip()
{
  ledcWrite(buzzChannel(0), 256);
  vTaskDelay(200);
  ledcWrite(buzzChannel(0), 0);
  vTaskDelay(200);
}
