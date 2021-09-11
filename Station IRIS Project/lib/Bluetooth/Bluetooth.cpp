#include "Bluetooth.h"

// VARIÁVEIS
// String's
String packBT = "";    // Buffer virtual, usado no callback
String oldPackBT = ""; // Auxiliares no callback
String auxPackBT = ""; // Auxiliares no callback

// ID's
String GATEWAY_ID = "",
       STATION_ID = "";

// Boolean's
bool connectedBT = 0;        // Estado de conexão bluetooth, usado no callback
bool oldDeviceConnected = 0; // Estado antigo de conexão bluetooth, usado no refreshConnectionBT()
bool fnConnect = 1;          // função auxiliar para controle de escrita dos dados bluetooth na EEPROM

// Outros dados
BLEServer *serverBT = NULL;                  // Aloca server BT
BLECharacteristic *characteristic_TX = NULL; // Aloca característica BT_TX
BLECharacteristic *characteristic_RX = NULL; // Aloca característica BT_RX
uint32_t BT_rxData;                          // Aloca função de recebimento no callback

// CALLBACK'S
// Estado de conexão bluetooth
class CallbackServer : public BLEServerCallbacks
{
  void onConnect(BLEServer *serverBT) { connectedBT = 1; }    // Dispositivo conectado
  void onDisconnect(BLEServer *serverBT) { connectedBT = 0; } // Dispositivo desconectado
};

// Recebimento de dados bluetooth
class CallbackRX : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *characteristic_TX)
  {
    packBT = "";
    auxPackBT = "";
    std::string BT_rxData = characteristic_RX->getValue();
    if (BT_rxData.length() > 0)
    {
      for (int i = 0; i < BT_rxData.length(); i++)
        auxPackBT += (char)(BT_rxData[i]);
      if (auxPackBT == oldPackBT)
        return;
      else if (auxPackBT != oldPackBT)
      {
        packBT = auxPackBT;
        oldPackBT = auxPackBT;
      }
    }
  }
};

//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
// FUNÇÕES

String randomize_ID_BT()
{
  randomSeed(analogRead(pinAnalog));
  return String("IRIStation - " + String(random(9999), DEC));
}

//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void setupBluetooth()
{
  pinMode(pinAnalog, INPUT);

  BLEDevice::init(randomize_ID_BT().c_str());

  // Cria server
  serverBT = BLEDevice::createServer();

  // Seta callback, para idenficar se o dispositivo estÃ¡ conectado
  serverBT->setCallbacks(new CallbackServer());

  // Cria um serviço
  BLEService *serviceBT = serverBT->createService(SERVICE_UUID);

  // Cria uma caracteristica de serviço para TX
  characteristic_TX = serviceBT->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_READ); // Cria caracteristica TX

  // Cria uma caracteristica de serviço para RX
  characteristic_RX = serviceBT->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);

  // Seta callback de recebimento
  characteristic_RX->setCallbacks(new CallbackRX());

  // Adiciona um descritor RX
  characteristic_RX->addDescriptor(new BLE2902());

  // Inicia serviço
  serviceBT->start();

  // Cria a propagação do dispositivo no serviço "SERVICE_UUID"
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);

  // Inicia propagação do dispositivo
  BLEDevice::startAdvertising();
}

//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void waitingBT() // Aguarda bluetooth conectar
{
  Serial.println("Esperando conexão bluetooth...");
  while (!connectedBT)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Bluetooth conectado!");
}

//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void waitingSYNC() // Aguarda sincronização da comunicação
{
  String sync = "";
  Serial.println("Aguardando sincronização...");
  while (sync != "@")
  {
    refreshConnectionBT();
    sync = getData();
  }
  Serial.println("Bluetooth sincronizado!");
}

//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void waitingREQUEST() // Aguarda requisição do clientAPP
{
  do
  {
    refreshConnectionBT();
    if (connectedBT)
      Serial.println("Esperando requisição... ");
    delay(250);
  } while (!getRequestBT());
  Serial.println("Requisição aceita: " + String(getRequestBT()));
  delay(1000);
}

//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void sendREQUEST() // Envia requisição para clientAPP
{
  Serial.println("Enviando request...");
  packBT = ""; // Limpeza do buffer
  writeBT(passwordClientAppBT);
  Serial.println("Request enviado!");
}

//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
String writeBT(String dados)
{
  unsigned long timeBT = 0;
  if (connectedBT)
  {
    if ((millis() - timeBT) > 4)
    {
      characteristic_TX->setValue(dados.c_str());
      characteristic_TX->notify();
      timeBT = millis();
    }
    return dados;
  }
}

//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
String getData() // Função de leitura conjunta com callback
{
  packBT = "";
  while (1)
  {
    refreshConnectionBT();
    if (connectedBT)
    {
      if (packBT.length() != 0 && (packBT.length() == (characteristic_RX->getValue()).length()))
        return (packBT);
    }
  }
}

//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
bool getRequestBT()
{
  while (1)
  {
    refreshConnectionBT();
    if (connectedBT)
    {
      if (packBT.length() != 0 && (packBT.length() == (characteristic_RX->getValue()).length()))
      {
        if (packBT == requestClientAppBT)
        {
          writeBT("#");
          return 1;
        }
        else
        {
          ESP.restart();
          packBT = "";
          return 0;
        }
      }
    }
  }
}

//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void refreshConnectionBT()
{
  // Atualiza conexão -> desconectado
  if (!connectedBT && oldDeviceConnected)
  {
    Serial.println("Bluetooth desconectado!");
    delay(500);
    packBT = ""; // Limpa o buffer
    serverBT->startAdvertising();
    oldDeviceConnected = connectedBT;
  }

  // Atualiza conexão -> conectado
  if (connectedBT && !oldDeviceConnected)
  {
    Serial.println("Bluetooth conectado!");
    oldDeviceConnected = connectedBT;
  }
}

void bluetoothConfig()
{
  // Configurações do bluetooth

  // Espera conexão com clientAPP
  waitingBT();

  // Espera sincronização do clientAPP
  waitingSYNC();

  // Envia request para o clientAPP
  sendREQUEST();

  // Espera para receber o request do clientAPP
  waitingREQUEST();

  //  Fim das configurações bluetooth
}

void getID()
{
  writeBT("|");
  GATEWAY_ID = getData();
  STATION_ID = writeBT(String(getChipID()));
  Serial.println("Chip ID: " + STATION_ID);
  delay(1000);
  writeBT("&");
}