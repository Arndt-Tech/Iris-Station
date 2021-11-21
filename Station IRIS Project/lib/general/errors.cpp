#include "errors.h"
/*
* 
* Areas 1-System.   2-GPIO.   3-GPS.   4-LoRa.
* 
* Order From right to left 
* 
* First The first represents the amount of short beeps.
* 
* Second The second represents the amount of long beeps.
* 
* Third The third represents the sub-error area.
* 
* Fourth The fourth represents the error area. 
* 
* Signal The signal indicates if the error will beep ('+' = beep) ('-' = No beep). 
* 
*/
void err::Error::begin()
{
  pinMode(buzzerPin, OUTPUT);
  ledcAttachPin(buzzerPin, buzzChannel(0));
  ledcSetup(buzzChannel(0), 2000, 10);
}

err::Error::err_::Failure err::Error::setError(err::Error::err_::Failure error)
{
  if (error > 0)
  {
    err::Error::long_bip((error % 100) / 10);
    err::Error::short_bip(((error % 100) % 10));
  }
  return error;
}

void err::Error::long_bip(uint8_t amount)
{
  for (uint8_t i = 0; i < amount; i++)
  {
    ledcWrite(buzzChannel(0), 256);
    vTaskDelay(700);
    ledcWrite(buzzChannel(0), 0);
    vTaskDelay(350);
  }
}

void err::Error::short_bip(uint8_t amount)
{
  for (uint8_t i = 0; i < amount; i++)
  {
    ledcWrite(buzzChannel(0), 256);
    vTaskDelay(200);
    ledcWrite(buzzChannel(0), 0);
    vTaskDelay(200);
  }
}
