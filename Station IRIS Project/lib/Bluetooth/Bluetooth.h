#ifndef _BLUETOOTH_H
#define _BLUETOOTH_H

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
#include "task.h"
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

// Struct's
typedef struct ble
{
  BLEServer *serverBT = NULL;                  // Aloca server BT
  BLECharacteristic *characteristic_TX = NULL; // Aloca característica BT_TX
  BLECharacteristic *characteristic_RX = NULL; // Aloca característica BT_RX
  uint32_t BT_rxData;                          // Recebimento callback
  uint8_t connected;                           // Estado de conexão
  uint8_t repeatDataFilter;
  String data;
  esp_bt_controller_status_t status;
} networkBluetooth;

// Externos
extern networkBluetooth BLE;

// Funções
void setupBluetooth(networkBluetooth *ble);
void callbackBLE(networkBluetooth *ble);

// Setup
void bluetoothConfig(networkBluetooth *ble); // Configura bluetooth

// Duplex
String getData(networkBluetooth *ble);               // Recebe dados com callback
String writeBT(networkBluetooth *ble, String dados); // envia dados via bluetooth

// Connect
void waitingBT(networkBluetooth *ble);           // Aguarda bluetooth conectar
void waitingSYNC(networkBluetooth *ble);         // Aguarda sincronização da comunicação
void refreshConnectionBT(networkBluetooth *ble); // Atualiza estado de conexão bluetooth com callback

// Request
void waitingREQUEST(networkBluetooth *ble); // Aguarda requisição do clientAPP
bool getRequestBT(networkBluetooth *ble);   // Recebe requisição com callback
void sendREQUEST(networkBluetooth *ble);    // Envia requisição para clientAPP

// Disable
void bleDisable();

// Procedures
void getID(networkBluetooth *ble, networkLora *gtw);

#endif