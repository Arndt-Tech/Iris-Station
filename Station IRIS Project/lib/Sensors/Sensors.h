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
#define readTime 2000
#define typeDHT DHT11
#define resetTmrOF 5

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
    bool m_valve_status;

  public:
    void begin();
    fle::Failure getDHT(com::Lora &st);
    void setValve(bool status);
    void checkReset();
    bool getValveStatus();
  };
}
