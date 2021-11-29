#include "Bluetooth.h"

BLECharacteristic *com::BLE::m_characteristic_RX = NULL;
String com::BLE::m_data;
uint8_t com::BLE::m_repeatDataFilter;
uint8_t com::BLE::m_connected;

//
/**
 * @brief Bluetooth connection status callback.
 * 
 */
class CallbackServer : public BLEServerCallbacks, com::BLE
{
  void onConnect(BLEServer *serverBT) { com::BLE::setConnectionStatus(1); }    // Dispositivo conectado
  void onDisconnect(BLEServer *serverBT) { com::BLE::setConnectionStatus(0); } // Dispositivo desconectado
};

/**
 * @brief Bluetooth data callback.
 * 
 */
class CallbackRX : public BLECharacteristicCallbacks, com::BLE
{
  void onWrite(BLECharacteristic *characteristic_TX) { com::BLE::callback(); }
};
/**
 * @brief Callback.
 * 
 */
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
/**
 * @brief Starts bluetooth communication with the station's configuration protocol and saves the data in LoRa packages.
 * 
 * @param st Reference for LoRa communication from the station.
 */
void com::BLE::begin(com::Lora &st)
{
  BLEDevice::init("StationIRIS");

  // Create server
  m_serverBT = BLEDevice::createServer();

  // Create connection state callback.
  m_serverBT->setCallbacks(new CallbackServer());

  // Create service.
  BLEService *m_serviceBT = m_serverBT->createService(SERVICE_UUID);

  // Create TX characteristic.
  m_characteristic_TX = m_serviceBT->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_READ);

  // Create RX characteristic.
  m_characteristic_RX = m_serviceBT->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);

  // Create downstream callback.
  m_characteristic_RX->setCallbacks(new CallbackRX());

  // Add descriptor.
  m_characteristic_RX->addDescriptor(new BLE2902());

  // initialize service.
  m_serviceBT->start();

  // Set propagation.
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);

  // Start propagation.
  BLEDevice::startAdvertising();

  // Enables filter against data repetition.
  m_repeatDataFilter = 1;

  config(st);
}

/**
 * @brief Waiting for connection.
 * 
 */
void com::BLE::waiting()
{
#if _DEBUG_MODE_
  Serial.println("Waiting for bluetooth connection...");
#elif !_DEBUG_MODE_
#endif
  while (!m_connected)
  {
    Serial.print(".");
    vTaskDelay(500);
  }
#if _DEBUG_MODE_
  Serial.println("Bluetooth connected!");
#elif !_DEBUG_MODE_
#endif
}

/**
 * @brief Wait sync.
 * 
 */
void com::BLE::waitingSYNC()
{
  // Disable data filter
  m_repeatDataFilter = 0;
#if _DEBUG_MODE_
  Serial.println("Waiting for synchronization.");
#elif !_DEBUG_MODE_
#endif
  while (read() != SYNC_FLAG)
  {
    Serial.print(".");
    vTaskDelay(500);
  }
#if _DEBUG_MODE_
  Serial.println("Synchronized Bluetooth!");
#elif !_DEBUG_MODE_
#endif
  // Enable data filter
  m_repeatDataFilter = 1;
}

/**
 * @brief Waiting for app request.
 * 
 */
void com::BLE::waitingRequest()
{
#if _DEBUG_MODE_
  Serial.println("Waiting for request.");
#elif !_DEBUG_MODE_
#endif
  while (!getRequest())
    ;
  {
    Serial.print(".");
    vTaskDelay(500);
  }
#if _DEBUG_MODE_
  Serial.println("\nRequest accepted: " + String(getRequest()));
#elif !_DEBUG_MODE_
#endif
  vTaskDelay(10);
}

/**
 * @brief Send app request.
 * 
 */
void com::BLE::sendRequest()
{
#if _DEBUG_MODE_
  Serial.println("Sending request...");
#elif !_DEBUG_MODE_
#endif
  m_data = "";
  write(passwordClientAppBT);
#if _DEBUG_MODE_
  Serial.println("Request sent!");
#elif !_DEBUG_MODE_
#endif
}

/**
 * @brief Sends data via bluetooth.
 * 
 * @param data Data to write.
 * @return Written data.
 */
String com::BLE::write(String data)
{
  TickType_t timeBT = 0;
  if (m_connected)
  {
    if ((spc::SpecialFunctions::ctrlTickCount(xTaskGetTickCount(), timeBT)) > 4)
    {
      m_characteristic_TX->setValue(data.c_str());
      m_characteristic_TX->notify();
      timeBT = xTaskGetTickCount();
    }
    return data;
  }
  else
    return "";
}

/**
 * @brief Reads bluetooth data.
 * 
 * @return Bluetooth data. 
 */
String com::BLE::read()
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

/**
 * @brief Check request received.
 * 
 * @return Request is true or reset device. 
 */
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

/**
 * @brief Update connection state.
 * 
 */
void com::BLE::refresh()
{
  static uint8_t oldDeviceConnected = 1;
  if (!m_connected && oldDeviceConnected)
  {
#if _DEBUG_MODE_
    Serial.println("Bluetooth disconnected!");
#elif !_DEBUG_MODE_
#endif
    vTaskDelay(500);
    m_data = ""; // Limpa o buffer
    m_serverBT->startAdvertising();
    oldDeviceConnected = m_connected;
  }
  if (m_connected && !oldDeviceConnected)
  {
#if _DEBUG_MODE_
    Serial.println("Bluetooth reconnected!");
#elif !_DEBUG_MODE_
#endif
    oldDeviceConnected = m_connected;
  }
}

/**
 * @brief IRIS protocol configuration.
 * 
 * @param st Reference for LoRa communication from the station.
 */
void com::BLE::config(com::Lora &st)
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
  sendID(st);
}

/**
 * @brief Disable bluetooth.
 * 
 */
void com::BLE::disable()
{
  esp_bluedroid_disable();
  esp_bluedroid_deinit();
  esp_bt_controller_disable();
  esp_bt_controller_deinit();
  esp_bt_mem_release(ESP_BT_MODE_BTDM);
}

/**
 * @brief Configure ID's for LoRa communication.
 * 
 * @param st Reference for LoRa communication from the station.
 */
void com::BLE::sendID(com::Lora &st)
{
  write(APP_SENDS_DESTADDR);
  st.packet.transmit.set.senderAddr(atol(read().c_str()));
  st.packet.transmit.set.localAddr(atol(write(String(cfg::ChipID::get())).c_str()));
  vTaskDelay(1000);
  write(SUCCESSFULLY_CONNECTED);
}

/**
 * @brief Set connection status.
 * 
 * @param status Connection status.
 */
void com::BLE::setConnectionStatus(uint8_t status) { m_connected = status; }

/**
 * @brief Get connection status.
 * 
 * @return Connection status. 
 */
uint8_t com::BLE::getConnectionStatus() { return m_connected; }

/**
 * @brief Get controller status.
 * 
 * @return Controller status. 
 */
esp_bt_controller_status_t com::BLE::getStatus() { return m_status; }
