#include "errors.h"

/**
 * @brief Initialize system errors.
 * 
 */
void err::Error::begin()
{
  pinMode(buzzerPin, OUTPUT);
  ledcAttachPin(buzzerPin, buzzChannel(0));
  ledcSetup(buzzChannel(0), 2000, 10);
}

/**
 * @brief Set error with beep.
 * 
 * @param error Error to be checked.
 * @return Parameterized error.
 */
err::Error::err_::Failure err::Error::setError(err::Error::err_::Failure error)
{
  if (error > 0)
  {
    err::Error::long_bip((error % 100) / 10);
    err::Error::short_bip(((error % 100) % 10));
  }
#if _DEBUG_MODE_
  switch (error)
  {
  case err::Error::err_::Failure::INIT_OK:
    Serial.println("Startup ok.");
    break;

  case err::Error::err_::Failure::NO_ERR:
    break;

  case err::Error::err_::Failure::ERR_EMPTY_EEPROM:
    Serial.println("Empty EEPROM.");
    break;

  case err::Error::err_::Failure::ERR_DHT_ISNAN:
    Serial.println("DHT ERROR.");
    break;

  case err::Error::err_::Failure::ERR_INVALID_GPS_LOCATION:
    Serial.println("Invalid location.");
    break;

  case err::Error::err_::Failure::WAR_UNKNOWN_GPS_FUNCTIONING:
    Serial.println("Unknown gps functioning.");
    break;

  case err::Error::err_::Failure::ERR_INITIALIZING_LORA:
    Serial.println("Error initializing LoRa.");
    break;

  case err::Error::err_::Failure::WAR_NO_LORA_PACKAGES:
    break;

  case err::Error::err_::Failure::WAR_UNKNOWN_PACKAGE:
    Serial.println("Unknown package.");
    break;

  case err::Error::err_::Failure::ERR_INCONSISTENT_LORA_PACKAGE:
    Serial.println("Inconsistent package.");
    break;

  default:
    Serial.println("Unknown error.");
    break;
  }
#elif !_DEBUG_MODE_
#endif
  return error;
}

/**
 * @brief Long beep.
 * 
 * @param amount Number of beeps.
 */
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

/**
 * @brief Short beep.
 * 
 * @param amount Number of beeps.
 */
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
