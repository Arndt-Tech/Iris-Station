#pragma once

#include <Arduino.h>
#include <FreeRTOS.h>
#include <SPI.h>
#include <LoRa.h>
#include "pinout.h"
#include "errors.h"
#include "debug.h"
#include "specFunctions.h"

#define INTERVAL 2000
// LoRa shipping interval
#define BAND 433E6
// 433MHz Radio working frequency
#define SF 12
//
#define BW 250E3
//
#define CR 4
//
#define PL 6
//
#define SW 0x16
//
/*
  Config:
    Band -> 433MHz
    Spread Factor -> 12
    Bandwitch -> 250KHz
    Coding Rate -> 4
    Payload Lenght -> 10 bytes
    Preable Lenght -> 6
    Sync Word -> 0x16
  |----------------------------------|
  Data:
    Equivalent bitrate -> 366 bytes/s
    Time on air -> 561 ms
*/
//
#define SIZE_CORRECTION(a) (size_t)(a)

/**
 * @brief Auxiliary classes.
 * 
 */
namespace aux
{
  class loraPackage
  {
  private:
    struct pckg
    {
      uint32_t m_localAddr;
      uint32_t m_destAddr;
      int16_t m_temperature;
      uint8_t m_humidity;
      uint8_t m_packageLen;
      int32_t m_latitude;
      int32_t m_longitude;
    } inThePackage;

  public:
    loraPackage();
    uint32_t getLocalAddr() const;
    uint32_t getDestAddr() const;
    int16_t getTemperature() const;
    uint8_t getHumidity() const;
    uint8_t getPacketLenght() const;
    int32_t getLatitude() const;
    int32_t getLongitude() const;
    void setLocalAddr(uint32_t value);
    void setDestAddr(uint32_t value);
    void setTemperature(float value);
    void setHumidity(uint8_t value);
    void setLatitude(double value);
    void setLongitude(double value);
  };
}

/**
 * @brief Communication classes.
 * 
 */
namespace com
{
  /**
   * @brief Communication class via Lora at the IRIS station.
   * 
   */
  class Lora
  {
  private:
    uint8_t m_received;
    int16_t m_signal;
    uint8_t m_valveStatus;
    uint8_t m_status;

  public:
    aux::loraPackage packet;

  private:
    fle::Failure getPackage();
    void sendPackage();
    void packID() const;
    void packSensors() const;
    void packGPS() const;

  public:
    void begin();
    uint8_t getValveStatus() const;
    void startLoRa();
    void stopLoRa();
    void run();
  };
}
