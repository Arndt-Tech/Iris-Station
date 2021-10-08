// Inclusões
#include "systemFunctions.h"

// Instâncias externas
DHT dht(DHTpin, typeDHT);

// Funções
void configBegin(networkLora *gtw)
{
  // Inicialização dos pinos
  pinMode(pin_resetEEPROM, INPUT);
  pinMode(DHTpin, INPUT);
  pinMode(valvePin1, OUTPUT);
  pinMode(valvePin2, OUTPUT);
  // Inicialização da válvula
  valve(false);
  // Inicialização da Serial
  Serial.begin(115200);
  // Inicialização da EEPROM
  EEPROM.begin(EEPROM_SIZE);
  // Inicialização dos sensores
  dht.begin();
  // Inicializa Buzzer
  setupBuzzer();
  // Inicialização display OLED
  setupOLED();
  // Inicialização dos dados do sistema
  setupDataSystem(gtw);
  // Inicialização da rede LoRa
  setupLoRa(gtw);
  // Configuração geral completa
  error(INIT_OK);
  // Inicialização das tasks
  setupTasks();
}

void setupDataSystem(networkLora *gtw)
{
  // Verifica EEPROM
  if (!verify_EEPROM(loChID_addr_min) || !verify_EEPROM(chID_addr_min))
  {
    // Configura bluetooth e recebe novos dados
    error(WAR_EMPTY_EEPROM);
    setupBluetooth();
    getID(gtw);

    // Aloca novos dados na EEPROM
    write_EEPROM(String(gtw->packet.destAddr), chID_addr_min);
    write_EEPROM(String(gtw->packet.localAddr), loChID_addr_min);
  }
  else
  {
    // Lê dados da EEPROM
    gtw->packet.destAddr = atol(read_EEPROM(chID_addr_min).c_str());
    gtw->packet.localAddr = atol(read_EEPROM(loChID_addr_min).c_str());
  }
}

