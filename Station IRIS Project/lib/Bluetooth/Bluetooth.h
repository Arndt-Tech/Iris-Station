#pragma once

// Inclusões de bibliotecas
#include <Arduino.h>
#include <FreeRTOS.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "esp_err.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_main.h"
#include "pinout.h"
#include "errors.h"
#include "debug.h"
#include "ChipID.h"
#include "_LoRa.h"

// Geral
#define BT_NAME "IRIStation"

// ID's
#define SERVICE_UUID "32454db8-1322-11ec-82a8-0242ac130003"           // ID serviço
#define CHARACTERISTIC_UUID_TX "beb5483e-36e1-4688-b7f5-ea07361b26a8" // ID de característica TX
#define CHARACTERISTIC_UUID_RX "808ccec4-d862-11eb-b8bc-0242ac130003" // ID de característica RX

// Senhas para protocolo de segurança
#define requestClientAppBT "hHo}8j<Sf(:4mTQW8W#l"  // Recebe do app
#define passwordClientAppBT "&i((3,iH0,+-z[i]PpAh" // Envia para o app

// Flag's
#define SYNC_FLAG "@"
#define APP_SENDS_USERID "/"
#define APP_SENDS_SSID "~"
#define APP_SENDS_PASSWORD "^"
#define CONNECTION_ERROR "!"
#define SUCCESSFULLY_CONNECTED "&"
#define APP_SENDS_DESTADDR "|"

/**
 * @brief Communication
 * 
 */
namespace com
{
  class BLE
  {
  private:
    BLEServer *m_serverBT = NULL;                  
    BLECharacteristic *m_characteristic_TX = NULL; 
    static BLECharacteristic *m_characteristic_RX; 
    uint32_t m_BT_rxData;                          
    static uint8_t m_repeatDataFilter;
    static String m_data;
    esp_bt_controller_status_t m_status;
    static uint8_t m_connected; 

  protected:
    static void callback();
    static void setConnectionStatus(uint8_t status);

  private:
    void config(com::Lora &st);
    void waiting();     
    void waitingSYNC();
    void waitingRequest(); 
    bool getRequest();     
    void sendRequest();    
    void sendID(com::Lora &st);

  public:
    void begin(com::Lora &st);
    String read();
    String write(String data);
    void refresh();
    void disable();
    static uint8_t getConnectionStatus();
  };
}
