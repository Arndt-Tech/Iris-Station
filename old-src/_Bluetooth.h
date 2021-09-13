#ifndef _BLUETOOTH_H  
#define _BLUETOOTH_H

// Inclusões de bibliotecas
  #include <BLEDevice.h>
  #include <BLEServer.h>
  #include <BLEUtils.h>
  #include <BLE2902.h>
  #include "esp_err.h"
  #include "esp_gap_bt_api.h"
  #include "esp_bt_main.h"
  #include "_specialFunction.h"

// ID's de serviço e características BT
  #define SERVICE_UUID           "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
  #define CHARACTERISTIC_UUID_TX "beb5483e-36e1-4688-b7f5-ea07361b26a8"
  #define CHARACTERISTIC_UUID_RX "808ccec4-d862-11eb-b8bc-0242ac130003"

// Senhas para protocolo de segurança
  #define requestClientAppBT "tSdcxhoLVYj7oMT500&5"       // Recebe do app
  #define passwordClientAppBT "KQ.?2qf01X|,JZRq8!},"      // Envia para o app



//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|                             
// FUNÇÕES
  // SETUP BT
  void setupBluetooth();       // Inicializa bluetooth
  void bluetoothConfig();      // Configura bluetooth
  String randomize_ID_BT();    // Monta nome do bluetooth

  // GET DATA BT
  String getData();            // Recebe dados com callback
  String readBT();             // Recebe dados sem callback

  // ENVIO DE DADOS BT
  void writeBT(String dados);  // envia dados via bluetooth

  // CONEXÃO BT
  void waitingBT();            // Aguarda bluetooth conectar
  void waitingSYNC();          // Aguarda sincronização da comunicação
  void refreshConnectionBT();  // Atualiza estado de conexão bluetooth com callback

  // REQUEST BT
  void waitingREQUEST();       // Aguarda requisição do clientAPP
  bool getRequestBT();         // Recebe requisição com callback
  void sendREQUEST();          // Envia requisição para clientAPP

  // BT WIFI_DATA
  void waitingSSID_WIFI();     // Espera clientAPP enviar SSID
  void waitingPASSWORD_WIFI(); // Espera clientAPP enviar PASSWORD


// TESTES DE FUÇÕES
void reInitBT();
 
  

//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
// VARIÁVEIS  
  // String's
  String packBT    = ""; // Buffer virtual, usado no callback
  String oldPackBT = ""; // Auxiliares no callback
  String auxPackBT = ""; // Auxiliares no callback
  
  // ID's
  String USER_ID_STR    = "",
         STATION_ID_STR = "",
         SSID_STR       = "",
         PASSWORD_STR   = "";

  // Boolean's
  bool connectedBT        = 0;  // Estado de conexão bluetooth, usado no callback
  bool oldDeviceConnected = 0;  // Estado antigo de conexão bluetooth, usado no refreshConnectionBT()
  bool fnConnect          = 1;  // Função auxiliar para controle de escrita dos dados bluetooth na EEPROM
  
  // Outros dados
  BLEServer* serverBT                  = NULL;  // Aloca server BT
  BLECharacteristic* characteristic_TX = NULL;  // Aloca característica BT_TX
  BLECharacteristic* characteristic_RX = NULL;  // Aloca característica BT_RX
  uint32_t BT_rxData;                           // Aloca função de recebimento no callback

#endif
