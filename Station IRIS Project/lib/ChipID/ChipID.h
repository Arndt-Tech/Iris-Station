#pragma once

// 
#include <Arduino.h>
#include <FreeRTOS.h>
#include "pinout.h"
#include "errors.h"
#include "debug.h"

/**
 * @brief Configuration
 * 
 */
namespace cfg
{
  class ChipID
  {
    public:
      static uint32_t get();
  };
}

