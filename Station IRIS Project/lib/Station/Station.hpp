#pragma once

//
#include <Arduino.h>
#include <FreeRTOS.h>
#include "pinout.h"
#include "errors.h"
#include "debug.h"
#include "ChipID.h"
#include "Log.h"
#include "Bluetooth.h"
#include "_LoRa.h"
#include "Sensors.h"
#include "GPS.h"

//
namespace stt
{
  class Station
  {
  private:
    /*
    Implicit
    - cfg::ChipID m_chip; 
    - cfg::Log m_log;
    - spc::SpecialFunctions m_special;
    */
    static err::Error m_err;
    static com::BLE m_ble;
    static com::Lora m_lora;
    static per::GPS m_gps;
    static per::GPIO m_gpio;

  private:
    fle::Failure checkSystem();

  public:
    struct mng
    {
      err::Error &Error();
      com::BLE &BLE();
      com::Lora &LoRa();
      per::GPS &GPS();
      per::GPIO &GPIO();
    } manage;

  public:
    void begin();
  };
}