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

// ID's de serviço e características BT
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID_TX "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define CHARACTERISTIC_UUID_RX "808ccec4-d862-11eb-b8bc-0242ac130003"

// Senhas para protocolo de segurança
#define requestClientAppBT "tSdcxhoLVYj7oMT500&5"  // Recebe do app
#define passwordClientAppBT "KQ.?2qf01X|,JZRq8!}," // Envia para o app

// Funções
// Setup
void setupBluetooth();    // Inicializa bluetooth
void bluetoothConfig();   // Configura bluetooth
String randomize_ID_BT(); // Monta nome do bluetooth

// Get Data
String getData(); // Recebe dados com callback
String readBT();  // Recebe dados sem callback

// Send Data
void writeBT(String dados); // envia dados via bluetooth

// Connect
void waitingBT();           // Aguarda bluetooth conectar
void waitingSYNC();         // Aguarda sincronizaÃ§Ã£o da comunicaÃ§Ã£o
void refreshConnectionBT(); // Atualiza estado de conexÃ£o bluetooth com callback

// Request
void waitingREQUEST(); // Aguarda requisiÃ§Ã£o do clientAPP
bool getRequestBT();   // Recebe requisiÃ§Ã£o com callback
void sendREQUEST();    // Envia requisiÃ§Ã£o para clientAPP

// Bluetooth-Wifi Procedures 
void waitingSSID_WIFI();     // Espera clientAPP enviar SSID
void waitingPASSWORD_WIFI(); // Espera clientAPP enviar PASSWORD

#endif