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
#include "specFunctions.h"
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

// Pino de leitura analógica
#define pinAnalog 36

// Funções
// Setup
void setupBluetooth();    // Inicializa bluetooth
void bluetoothConfig();   // Configura bluetooth
String randomize_ID_BT(); // Monta nome do bluetooth

// Get Data
String getData(); // Recebe dados com callback

// Send Data
String writeBT(String dados); // envia dados via bluetooth

// Connect
void waitingBT();           // Aguarda bluetooth conectar
void waitingSYNC();         // Aguarda sincronização da comunicação
void refreshConnectionBT(); // Atualiza estado de conexão bluetooth com callback

// Request
void waitingREQUEST(); // Aguarda requisição do clientAPP
bool getRequestBT();   // Recebe requisição com callback
void sendREQUEST();    // Envia requisição para clientAPP

// Procedures
void getID(networkLora *gtw);

#endif