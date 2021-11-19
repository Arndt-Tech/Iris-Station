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
    SoftwareSerial softwareSerial;
    TinyGPSPlus data;

  public:
    void begin();
    fle::Failure getLocalization(com::Lora &st);
  };
}