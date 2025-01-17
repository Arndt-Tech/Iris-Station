#pragma once

// Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>
#include <DHT.h>
#include <DHT_U.h>
#include "pinout.h"
#include "errors.h"
#include "debug.h"
#include "Log.h"
#include "specFunctions.h"
#include "_LoRa.h"

// Definições
#define readTime 10000
#define typeDHT DHT11
#define resetTmrOF 4

/**
 * @brief Peripherals.
 * 
 */
namespace per
{
  /**
   * @brief GPIO Class.
   * 
   */
  class GPIO
  {
  private:
    static double m_temperature;
    static double m_humidity;
    static uint8_t m_valve_status;
    static uint8_t m_dht_status;
    static uint8_t m_temperature_unit;

  public:
    struct snsr
    {
      err::Error::err_::Failure readDHT(com::Lora &st);
      double getTemperature();
      double getHumidity();
      uint8_t status();
      void setUnit(bool unit);
    } weather;

    struct vlv
    {
      void setValve(uint8_t status);
      static uint8_t status();
    } valve;

    struct oth
    {
      void checkReset();
    } other;

  public:
    void begin();
  };
}
