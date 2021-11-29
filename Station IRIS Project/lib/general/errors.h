#pragma once
/*
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
*/

#include <Arduino.h>
#include <FreeRTOS.h>
#include "pinout.h"
#include "debug.h"

//
#define buzzChannel(a) (uint8_t)(a)
/**
 * @brief Error classes.
 * 
 */
namespace err
{
  /**
   * @brief Error class.
   * 
   */
  class Error
  {
  public:
    /**
    * @brief  Failure enumerations.
    */
    struct err_
    {
      enum Failure
      {
        // Inicialization OK.
        INIT_OK = +1001,

        // No mistakes.
        NO_ERR = -1100,

        // Empty EEPROM
        ERR_EMPTY_EEPROM = +1211,

        // Unidentified DHT sensor.
        ERR_DHT_ISNAN = +2002,

        // Invalid location
        ERR_INVALID_GPS_LOCATION = +3013,

        // No data received from GPS
        WAR_UNKNOWN_GPS_FUNCTIONING = -3100,

        // Error initializing LoRa.
        ERR_INITIALIZING_LORA = +4012,

        // No Lora Packages
        WAR_NO_LORA_PACKAGES = -4100,

        // Unknown package.
        WAR_UNKNOWN_PACKAGE = -4200,

        // Inconsistent package.
        ERR_INCONSISTENT_LORA_PACKAGE = +4320,
      };
    } errors;

  private:
    static void long_bip(uint8_t amount);
    static void short_bip(uint8_t amount);

  public:
    void begin();
    static err::Error::err_::Failure setError(err::Error::err_::Failure error);
  };
}
