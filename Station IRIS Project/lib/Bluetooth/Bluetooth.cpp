#include "Bluetooth.h"

#define pinAnalog 26

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
  bool connectedBT        = 0;  // Estado de conexÃ£o bluetooth, usado no callback
  bool oldDeviceConnected = 0;  // Estado antigo de conexÃ£o bluetooth, usado no refreshConnectionBT()
  bool fnConnect          = 1;  // FunÃ§Ã£o auxiliar para controle de escrita dos dados bluetooth na EEPROM
  
  // Outros dados
  BLEServer* serverBT                  = NULL;  // Aloca server BT
  BLECharacteristic* characteristic_TX = NULL;  // Aloca caracterÃ­stica BT_TX
  BLECharacteristic* characteristic_RX = NULL;  // Aloca caracterÃ­stica BT_RX
  uint32_t BT_rxData;                           // Aloca funÃ§Ã£o de recebimento no callback


  // CALLBACK'S
  // Estado de conexão bluetooth
  class CallbackServer: public BLEServerCallbacks 
  {
    void onConnect(BLEServer* serverBT) {connectedBT = 1;}  // Dispositivo conectado
    void onDisconnect(BLEServer* serverBT) {connectedBT = 0;}   // Dispositivo desconectado
  };
  
  // Recebimento de dados bluetooth
  class CallbackRX: public BLECharacteristicCallbacks 
  {
    void onWrite(BLECharacteristic *characteristic_TX) 
    {
      packBT = "";
      auxPackBT = "";
      std::string BT_rxData = characteristic_RX->getValue();
      if (BT_rxData.length() > 0) 
      { 
        for (int i = 0; i < BT_rxData.length(); i++)auxPackBT += (char)(BT_rxData[i]);
        if (auxPackBT == oldPackBT)return;
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
  BLEDevice::init(randomize_ID_BT().c_str());
  
  // Cria server
  serverBT = BLEDevice::createServer();
  
  // Seta callback, para idenficar se o dispositivo estÃ¡ conectado
  serverBT->setCallbacks(new CallbackServer());
  
  // Cria um serviÃ§o
  BLEService* serviceBT = serverBT->createService(SERVICE_UUID);

  // Cria uma caracteristica de serviÃ§o para TX
  characteristic_TX = serviceBT->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_READ);   // Cria caracteristica TX

  // Cria uma caracteristica de serviÃ§o para RX
  characteristic_RX = serviceBT->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
  
  // Seta callback de recebimento
  characteristic_RX->setCallbacks(new CallbackRX());

  // Adiciona um descritor RX
  characteristic_RX->addDescriptor(new BLE2902());

  // Inicia serviÃ§o
  serviceBT->start();

  // Cria a propagaÃ§Ã£o do dispositivo no serviÃ§o "SERVICE_UUID" 
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  
  // Inicia propagaÃ§Ã£o do dispositivo
  BLEDevice::startAdvertising();
}



//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void waitingBT()   // Aguarda bluetooth conectar
{
  Serial.println ("Esperando conexÃ£o bluetooth...");
  while (!connectedBT)
  {
    Serial.print (".");
    delay (500);
  }
  Serial.println("Bluetooth conectado!"); 
}



//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void waitingSYNC()  // Aguarda sincronizaÃ§Ã£o da comunicaÃ§Ã£o
{
  String sync = "";
  Serial.println ("Aguardando sincronizaÃ§Ã£o...");
  while (sync != "@")
  {
    refreshConnectionBT();
    sync = getData();
  }
  Serial.println ("Bluetooth sincronizado!");
}



//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void waitingREQUEST()  // Aguarda requisiÃ§Ã£o do clientAPP
{
  do{
     refreshConnectionBT();
     if (connectedBT)Serial.println ("Esperando requisiÃ§Ã£o...");
     delay (250);
  }while (!getRequestBT());
  Serial.println ("RequisiÃ§Ã£o aceita: " + String(getRequestBT())); 
  delay (1000);
}


//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void sendREQUEST()  // Envia requisiÃ§Ã£o para clientAPP
{
  Serial.println ("Enviando request...");
  packBT = "";    // Limpeza do buffer
  writeBT (passwordClientAppBT);
  Serial.println ("Request enviado!");
}


//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
void writeBT(String dados)
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
  }
}



//-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
String getData()        // FunÃ§Ã£o de leitura conjunta com callback
{ 
  packBT = "";     
  while (1)
  { 
    refreshConnectionBT(); 
    if (connectedBT) 
    {
      if (packBT.length() != 0 && (packBT.length() == (characteristic_RX->getValue()).length())) return (packBT);
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
          writeBT ("#");
          return 1;
        }
        else if (packBT != requestClientAppBT) 
        {
          //resetModule();
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
  // Atualiza conexÃ£o -> desconectado
  if (!connectedBT && oldDeviceConnected) 
  {
    Serial.println ("Bluetooth desconectado!");
    delay(500);
    packBT = "";                         // Limpa o buffer
    serverBT->startAdvertising(); 
    oldDeviceConnected = connectedBT;
  }
    
  // Atualiza conexÃ£o -> conectado
  if (connectedBT && !oldDeviceConnected) 
  {
    Serial.println ("Bluetooth conectado!");
    oldDeviceConnected = connectedBT;
  }
}



void bluetoothConfig()
{
  // ConfiguraÃ§Ãµes do bluetooth
  
  // Espera conexÃ£o com clientAPP
  waitingBT();            

  // Espera sincronizaÃ§Ã£o do clientAPP
  waitingSYNC();          

  // Envia request para o clientAPP
  sendREQUEST();          

  // Espera para receber o request do clientAPP
  waitingREQUEST(); 
  
  //  Fim das configuraÃ§Ãµes bluetooth
}