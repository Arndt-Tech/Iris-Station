#include "Bluetooth.h"

BLECharacteristic *com::BLE::m_characteristic_RX = NULL;
String com::BLE::m_data;
uint8_t com::BLE::m_repeatDataFilter;
uint8_t com::BLE::m_connected;

//
// Estado de conexão bluetooth
class CallbackServer : public BLEServerCallbacks
{
  void onConnect(BLEServer *serverBT) { com::BLE::setConnectionStatus(1); }    // Dispositivo conectado
  void onDisconnect(BLEServer *serverBT) { com::BLE::setConnectionStatus(0); } // Dispositivo desconectado
};

// Recebimento de dados bluetooth
class CallbackRX : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *characteristic_TX) { com::BLE::callback(); }
};

void com::BLE::callback()
{
  static String oldData;
  String auxData = "";
  m_data = "";
  std::string BT_rxData = m_characteristic_RX->getValue();
  if (BT_rxData.length() > 0)
  {
    for (register uint8_t i = 0; i < BT_rxData.length(); i++)
      auxData += (char)(BT_rxData[i]);
    if (m_repeatDataFilter)
    {
      if (auxData == oldData)
        return;
      else if (auxData != oldData)
      {
        m_data = auxData;
        oldData = auxData;
      }
    }
    else
      m_data = auxData;
  }
}

void com::BLE::begin()
{
  BLEDevice::init("StationIRIS");

  // Cria server
  m_serverBT = BLEDevice::createServer();

  // Seta callback, para idenficar se o dispositivo está conectado
  m_serverBT->setCallbacks(new CallbackServer());

  // Cria um serviço
  BLEService *m_serviceBT = m_serverBT->createService(SERVICE_UUID);

  // Cria uma caracteristica de serviço para TX
  m_characteristic_TX = m_serviceBT->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_READ);

  // Cria uma caracteristica de serviço para RX
  m_characteristic_RX = m_serviceBT->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);

  // Seta callback de recebimento
  m_characteristic_RX->setCallbacks(new CallbackRX());

  // Adiciona um descritor RX
  m_characteristic_RX->addDescriptor(new BLE2902());

  // Inicia serviço
  m_serviceBT->start();

  // Cria a propagação do dispositivo no serviço "SERVICE_UUID"
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);

  // Inicia propagação do dispositivo
  BLEDevice::startAdvertising();

  m_repeatDataFilter = 1; // Desativa filtro contra repetição de dados

  config();
}

void com::BLE::waiting() // Aguarda bluetooth conectar
{
#if _DEBUG_MODE_
  Serial.println("Esperando conexão bluetooth...");
#elif !_DEBUG_MODE_
#endif
  while (!m_connected)
  {
    Serial.print(".");
    vTaskDelay(500);
  }
#if _DEBUG_MODE_
  Serial.println("Bluetooth conectado!");
#elif !_DEBUG_MODE_
#endif
}

void com::BLE::waitingSYNC() // Aguarda sincronização da comunicação
{
  m_repeatDataFilter = 0; // Desativa filtro contra repetição de dados
#if _DEBUG_MODE_
  Serial.println("Aguardando sincronização ");
#elif !_DEBUG_MODE_
#endif
  while (getData() != SYNC_FLAG)
  {
    Serial.print(".");
    vTaskDelay(500);
  }
#if _DEBUG_MODE_
  Serial.println("Bluetooth sincronizado!");
#elif !_DEBUG_MODE_
#endif
  m_repeatDataFilter = 1; // Ativa filtro contra repetição de dados
}

void com::BLE::waitingRequest() // Aguarda requisição do clientAPP
{
#if _DEBUG_MODE_
  Serial.println("Esperando requisição ");
#elif !_DEBUG_MODE_
#endif
  while (!getRequest())
    ;
  {
    Serial.print(".");
    vTaskDelay(500);
  }
#if _DEBUG_MODE_
  Serial.println("\nRequisição aceita: " + String(getRequest()));
#elif !_DEBUG_MODE_
#endif
  vTaskDelay(10);
}

void com::BLE::sendRequest() // Envia requisição para clientAPP
{
#if _DEBUG_MODE_
  Serial.println("Enviando request...");
#elif !_DEBUG_MODE_
#endif
  m_data = ""; // Limpeza do buffer
  write(passwordClientAppBT);
#if _DEBUG_MODE_
  Serial.println("Request enviado!");
#elif !_DEBUG_MODE_
#endif
}

String com::BLE::write(String dados)
{
  unsigned long timeBT = 0;
  if (m_connected)
  {
    if ((xTaskGetTickCount() - timeBT) > 4)
    {
      m_characteristic_TX->setValue(dados.c_str());
      m_characteristic_TX->notify();
      timeBT = xTaskGetTickCount();
    }
    return dados;
  }
  else
    return "";
}

String com::BLE::getData() // Função de leitura conjunta com callback
{
  m_data = "";
  while (1)
  {
    refresh();
    if (m_connected)
      if (m_data.length() != 0 && (m_data.length() == (m_characteristic_RX->getValue()).length()))
      {
        Serial.println(m_data);
        return (m_data);
      }
  }
}

bool com::BLE::getRequest()
{
  while (1)
  {
    refresh();
    if (m_connected)
    {
      if (m_data.length() != 0 && (m_data.length() == (m_characteristic_RX->getValue()).length()))
      {
        if (m_data == requestClientAppBT)
        {
          write("#");
          return 1;
        }
        else
          spc::SpecialFunctions::resetModule();
      }
    }
  }
}

void com::BLE::refresh()
{
  static uint8_t oldDeviceConnected = 1;
  if (!m_connected && oldDeviceConnected) // Atualiza conexão -> desconectado
  {
#if _DEBUG_MODE_
    Serial.println("Bluetooth desconectado!");
#elif !_DEBUG_MODE_
#endif
    vTaskDelay(500);
    m_data = ""; // Limpa o buffer
    m_serverBT->startAdvertising();
    oldDeviceConnected = m_connected;
  }
  if (m_connected && !oldDeviceConnected) // Atualiza conexão -> conectado
  {
#if _DEBUG_MODE_
    Serial.println("Bluetooth reconectado!");
#elif !_DEBUG_MODE_
#endif
    oldDeviceConnected = m_connected;
  }
}

void com::BLE::config()
{
  // Configurações do bluetooth

  // Espera conexão com clientAPP
  waiting();

  // Espera sincronização do clientAPP
  waitingSYNC();

  // Envia request para o clientAPP
  sendRequest();

  // Espera para receber o request do clientAPP
  waitingRequest();

  //  Fim das configurações bluetooth
}

void com::BLE::bleDisable()
{
  esp_bluedroid_disable();
  esp_bluedroid_deinit();
  esp_bt_controller_disable();
  esp_bt_controller_deinit();
  esp_bt_mem_release(ESP_BT_MODE_BTDM);
}

void com::BLE::sendID(com::Lora &st)
{
  write(APP_SENDS_DESTADDR);
  st.packet.transmit.set.senderAddr(atol(getData().c_str()));
  st.packet.transmit.set.localAddr(atol(write(String(cfg::ChipID::get())).c_str()));
  vTaskDelay(1000);
  write(SUCCESSFULLY_CONNECTED);
}

void com::BLE::setConnectionStatus(uint8_t status) { m_connected = status; }

uint8_t com::BLE::getConnectionStatus() { return m_connected; }