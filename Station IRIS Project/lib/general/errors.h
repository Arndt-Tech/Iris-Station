#pragma once

//
#include <Arduino.h>
#include <FreeRTOS.h>
#include "pinout.h"
#include "debug.h"

//
#define buzzChannel(a) (uint8_t)(a)

/**
 * @brief Failure enumeration.
 * 
 */
namespace fle
{
  /**
   * @brief Fault enumeration class.
   * 
   */
  enum class Failure
  {
    // Inicialization OK.
    INIT_OK = 1,

    // No mistakes.
    NO_ERR = 2,

    // Unidentified DHT sensor.
    ERR_DHT_ISNAN = 10,

    // Invalid location
    WAR_INVALID_GPS_LOCATION = 11,

    // No data received from GPS
    ERR_UNKNOWN_GPS_FUNCTIONING = 12,

    // Error initializing LoRa.
    ERR_INITIALIZING_LORA = 20,

    // Unidentified DHT sensor.
    WAR_NO_LORA_PACKAGES = 21,

    // Unknown package.
    WAR_UNKNOWN_PACKAGE = 22,

    // Inconsistent package.
    ERR_INCONSISTENT_LORA_PACKAGE = 23,

    // Empty EEPROM
    WAR_EMPTY_EEPROM = 30,
  };
}

/**
 * @brief Errors
 * 
 */
namespace err
{
  class Error
  {
  private:
    static void long_bip();
    static void short_bip();

  public:
    void begin();
    static fle::Failure setError(fle::Failure error);
  };
}
