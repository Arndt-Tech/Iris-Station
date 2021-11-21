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
namespace com{ class Lora; }
namespace aux{ class loraPackage; }

namespace aux
{
  class loraPackage
  {
    friend class com::Lora;

  private:
    struct trns
    {
      struct pckg
      {
        uint32_t m_localAddr;
        uint32_t m_senderAddr;
        int16_t m_temperature;
        uint8_t m_humidity;
        uint8_t m_size;
        int32_t m_latitude;
        int32_t m_longitude;
      } inThePackage;
    } static send;

  private:
    struct rcpt
    {
      struct pckg
      {
        uint32_t m_receiverAddr;
        uint32_t m_senderAddr;
        uint8_t m_valveStatus;
        uint8_t m_size;
        int16_t m_signal;
      } inThePackage;
    } static rec;

  public:
    loraPackage();
    struct trnsf
    {
      struct gt
      {
        uint32_t localAddr() const;
        uint32_t senderAddr() const;
        int16_t temperature() const;
        uint8_t humidity() const;
        uint8_t size() const;
        int32_t latitude() const;
        int32_t longitude() const;
      } get;
      struct st
      {
        void localAddr(uint32_t value);
        void senderAddr(uint32_t value);
        void temperature(float value);
        void humidity(uint8_t value);
        void latitude(double value);
        void longitude(double value);
      } set;
    } transmit;

    struct rcptf
    {
      struct gt
      {
        uint32_t receiverAddr() const;
        uint32_t senderAddr() const;
        uint8_t valveStatus() const;
        uint8_t size() const;
        int16_t signal() const;
      } get;
    } receive;
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
    static uint8_t m_received;
    static LoRaClass m_settings;

  private:
    static void packID();
    static void packSensors();
    static void packGPS();

  public:
    static aux::loraPackage packet;
    struct opr
    {
      static err::Error::err_::Failure readPackage();
      static void sendPackage();
      static void duplex();
    } operation;

  public:
    void begin();
    uint8_t checkRequest();
    LoRaClass &advancedSettings();
  };
}
