#include "Bluetooth.h"

// Callback's
// Estado de conexão bluetooth
class CallbackServer : public BLEServerCallbacks
{
  void onConnect(BLEServer *serverBT) { BLE.connected = 1; }    // Dispositivo conectado
  void onDisconnect(BLEServer *serverBT) { BLE.connected = 0; } // Dispositivo desconectado
};

// Recebimento de dados bluetooth
class CallbackRX : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *characteristic_TX) { callbackBLE(&BLE); }
};

// Funções
void setupBluetooth(networkBluetooth *ble)
{
  BLEDevice::init("StationIRIS");

  // Cria server
  ble->serverBT = BLEDevice::createServer();

  // Seta callback, para idenficar se o dispositivo está conectado
  ble->serverBT->setCallbacks(new CallbackServer());

  // Cria um serviço
  BLEService *serviceBT = ble->serverBT->createService(SERVICE_UUID);

  // Cria uma caracteristica de serviço para TX
  ble->characteristic_TX = serviceBT->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_READ);

  // Cria uma caracteristica de serviço para RX
  ble->characteristic_RX = serviceBT->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);

  // Seta callback de recebimento
  ble->characteristic_RX->setCallbacks(new CallbackRX());

  // Adiciona um descritor RX
  ble->characteristic_RX->addDescriptor(new BLE2902());

  // Inicia serviço
  serviceBT->start();

  // Cria a propagação do dispositivo no serviço "SERVICE_UUID"
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);

  // Inicia propagação do dispositivo
  BLEDevice::startAdvertising();

  ble->repeatDataFilter = 1; // Desativa filtro contra repetição de dados

  bluetoothConfig(ble);
}

void callbackBLE(networkBluetooth *ble)
{
  static String oldData;
  String auxData = "";
  ble->data = "";
  std::string BT_rxData = ble->characteristic_RX->getValue();
  if (BT_rxData.length() > 0)
  {
    for (register uint8_t i = 0; i < BT_rxData.length(); i++)
      auxData += (char)(BT_rxData[i]);
    if (ble->repeatDataFilter)
    {
      if (auxData == oldData)
        return;
      else if (auxData != oldData)
      {
        ble->data = auxData;
        oldData = auxData;
      }
    }
    else
      ble->data = auxData;
  }
}

void waitingBT(networkBluetooth *ble) // Aguarda bluetooth conectar
{
  Serial.println("Esperando conexão bluetooth...");
  while (!ble->connected)
  {
    Serial.print(".");
    vTaskDelay(500);
  }
  Serial.println("Bluetooth conectado!");
}

void waitingSYNC(networkBluetooth *ble) // Aguarda sincronização da comunicação
{
  ble->repeatDataFilter = 0; // Desativa filtro contra repetição de dados
  Serial.println("Aguardando sincronização ");
  while (getData(ble) != SYNC_FLAG)
  {
    Serial.print(".");
    vTaskDelay(500);
  }
  Serial.println("Bluetooth sincronizado!");
  ble->repeatDataFilter = 1; // Ativa filtro contra repetição de dados
}

void waitingREQUEST(networkBluetooth *ble) // Aguarda requisição do clientAPP
{
  Serial.println("Esperando requisição ");
  while (!getRequestBT(ble))
    ;
  {
    Serial.print(".");
    vTaskDelay(500);
  }
  Serial.println("\nRequisição aceita: " + String(getRequestBT(ble)));
  vTaskDelay(10);
}

void sendREQUEST(networkBluetooth *ble) // Envia requisição para clientAPP
{
  Serial.println("Enviando request...");
  ble->data = ""; // Limpeza do buffer
  writeBT(ble, passwordClientAppBT);
  Serial.println("Request enviado!");
}

String writeBT(networkBluetooth *ble, String dados)
{
  unsigned long timeBT = 0;
  if (ble->connected)
  {
    if ((xTaskGetTickCount() - timeBT) > 4)
    {
      ble->characteristic_TX->setValue(dados.c_str());
      ble->characteristic_TX->notify();
      timeBT = xTaskGetTickCount();
    }
    return dados;
  }
  else
    return "";
}

String getData(networkBluetooth *ble) // Função de leitura conjunta com callback
{
  ble->data = "";
  while (1)
  {
    refreshConnectionBT(ble);
    if (ble->connected)
      if (ble->data.length() != 0 && (ble->data.length() == (ble->characteristic_RX->getValue()).length()))
      {
        Serial.println(ble->data);
        return (ble->data);
      }
  }
}

bool getRequestBT(networkBluetooth *ble)
{
  while (1)
  {
    refreshConnectionBT(ble);
    if (ble->connected)
    {
      if (ble->data.length() != 0 && (ble->data.length() == (ble->characteristic_RX->getValue()).length()))
      {
        if (ble->data == requestClientAppBT)
        {
          writeBT(ble, "#");
          return 1;
        }
        else
          resetModule();
      }
    }
  }
}

void refreshConnectionBT(networkBluetooth *ble)
{
  static uint8_t oldDeviceConnected = 1;
  if (!ble->connected && oldDeviceConnected) // Atualiza conexão -> desconectado
  {
    Serial.println("Bluetooth desconectado!");
    vTaskDelay(500);
    ble->data = ""; // Limpa o buffer
    ble->serverBT->startAdvertising();
    oldDeviceConnected = ble->connected;
  }
  if (ble->connected && !oldDeviceConnected) // Atualiza conexão -> conectado
  {
    Serial.println("Bluetooth reconectado!");
    oldDeviceConnected = ble->connected;
  }
}

void bluetoothConfig(networkBluetooth *ble)
{
  // Configurações do bluetooth

  // Espera conexão com clientAPP
  waitingBT(ble);

  // Espera sincronização do clientAPP
  waitingSYNC(ble);

  // Envia request para o clientAPP
  sendREQUEST(ble);

  // Espera para receber o request do clientAPP
  waitingREQUEST(ble);

  //  Fim das configurações bluetooth
}

void bleDisable()
{
  esp_bluedroid_disable();
  esp_bluedroid_deinit();
  esp_bt_controller_disable();
  esp_bt_controller_deinit();
  esp_bt_mem_release(ESP_BT_MODE_BTDM);
}

void getID(networkBluetooth *ble , networkLora *gtw)
{
  writeBT(ble, APP_SENDS_DESTADDR);
  gtw->packet.destAddr = atol(getData(ble).c_str());
  gtw->packet.localAddr = atol(writeBT(ble, String(getChipID())).c_str());
  Serial.println("Chip ID: " + String(gtw->packet.localAddr));
  vTaskDelay(1000);
  writeBT(ble, SUCCESSFULLY_CONNECTED);
}