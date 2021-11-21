#pragma once

//
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include "pinout.h"
#include "errors.h"
#include "debug.h"
#include "_LoRa.h"

#define BAUD_RATE 9600

/**
 * @brief Peripherals.
 * 
 */
namespace per
{
  /**
   * @brief GPS Class.
   * 
   */
  class GPS 
  {
  private:
    SoftwareSerial m_softwareSerial;
    TinyGPSPlus m_data;
    uint8_t m_status;
    double m_lat;
    double m_lon;

  public:
    void begin();
    err::Error::err_::Failure getLocalization(com::Lora &st);
    uint8_t getStatus();
    double getLatitude();
    double getLongitude();

  
  };
}