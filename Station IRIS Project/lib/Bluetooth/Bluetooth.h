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
    BLEServer *m_serverBT = NULL;                  // Aloca server BT
    BLECharacteristic *m_characteristic_TX = NULL; // Aloca característica BT_TX
    static BLECharacteristic *m_characteristic_RX; // Aloca característica BT_RX
    uint32_t m_BT_rxData;                          // Recebimento callback
    static uint8_t m_repeatDataFilter;
    static String m_data;
    esp_bt_controller_status_t m_status;
    static uint8_t m_connected; // Estado de conexão

  private:
    // Config
    void config();
    // Connect
    void waiting();     // Aguarda bluetooth conectar
    void waitingSYNC(); // Aguarda sincronização da comunicação
    // Request
    void waitingRequest(); // Aguarda requisição do clientAPP
    bool getRequest();     // Recebe requisição com callback
    void sendRequest();    // Envia requisição para clientAPP

  public:
    void begin();
    // Communication
    String getData();
    String write(String dados);
    // Refresh
    void refresh();
    // Disable
    void bleDisable();
    // Procedures
    void sendID(com::Lora &st);
    // Callback
    static void callback();
    // Connection
    static void setConnectionStatus(uint8_t status);
    static uint8_t getConnectionStatus();
  };
}
