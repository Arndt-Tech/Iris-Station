// Inclusões
#include "systemFunctions.h"

// Instâncias externas
DHT dht(DHTpin, typeDHT);

// Funções
void configBegin(networkBluetooth *ble, networkLora *gtw, GPS *gps)
{
  // Inicialização dos pinos
  pinMode(pin_resetEEPROM, INPUT);
  pinMode(DHTpin, INPUT);
  pinMode(valvePin1, OUTPUT);
  pinMode(valvePin2, OUTPUT);
  // Inicializa taskReset
  xTaskCreatePinnedToCore(taskReset, "taskReset", STACK(2048), NULL, PRIORITY(4), NULL, CORE(1));
  // Inicialização da válvula
  valve(false);
  // Inicialização da Serial
  Serial.begin(115200);
  // Inicialização da SoftwareSerial
  setupSoftwareSerial(gps);
  // Inicialização da EEPROM
  EEPROM.begin(EEPROM_SIZE);
  // Inicialização dos sensores
  dht.begin();
  // Inicializa Buzzer
  setupBuzzer();
  // Inicialização display OLED
  setupOLED();
  // Inicialização dos dados do sistema
  setupDataSystem(ble, gtw);
  // Inicialização da rede LoRa
  setupLoRa(gtw);
  // Configuração geral completa
  error(INIT_OK);
  // Inicialização das tasks
  setupTasks();
}

void setupDataSystem(networkBluetooth *ble, networkLora *gtw)
{
  // Verifica EEPROM
  if (!verifyEEPROM(loChID_addr_min) || !verifyEEPROM(chID_addr_min))
  {
    // Configura bluetooth e recebe novos dados
    error(WAR_EMPTY_EEPROM);
    setupBluetooth(ble);
    getID(ble, gtw);

    // Aloca novos dados na EEPROM
    writeEEPROM(gtw->packet.destAddr, chID_addr_min);
    writeEEPROM(gtw->packet.localAddr, loChID_addr_min);
    vTaskDelay(1000);
    resetModule();
  }
  else
  {
    // Lê dados da EEPROM
    gtw->packet.destAddr = readEEPROM(chID_addr_min);
    gtw->packet.localAddr = readEEPROM(loChID_addr_min);
  }
}

