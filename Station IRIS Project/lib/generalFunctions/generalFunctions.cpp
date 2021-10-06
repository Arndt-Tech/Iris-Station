// Inclusões
#include "generalFunctions.h"

// Instâncias externas
extern DHT dht;

// Funções
void configBegin(networkLora *gtw)
{
  pinMode(pin_resetEEPROM, INPUT);
  pinMode(DHTpin, INPUT);
  pinMode(valvePin1, OUTPUT);
  pinMode(valvePin2, OUTPUT);
  digitalWrite(valvePin1, LOW);
  digitalWrite(valvePin2, LOW);
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);
  dht.begin();
  setupOLED();
  setupDataSystem(gtw);
}

void setupDataSystem(networkLora *gtw)
{
  // Verifica EEPROM
  if (!verify_EEPROM(loChID_addr_min) || !verify_EEPROM(chID_addr_min))
  {
    // Configura bluetooth e recebe novos dados
    setupBluetooth();
    getID(gtw);

    // Aloca novos dados na EEPROM
    write_EEPROM(String(gtw->destAddr), chID_addr_min);
    write_EEPROM(String(gtw->localAddr), loChID_addr_min);
  }
  else
  {
    // Lê dados da EEPROM
    gtw->destAddr = atol(read_EEPROM(chID_addr_min).c_str());
    gtw->localAddr = atol(read_EEPROM(loChID_addr_min).c_str());
  }
}