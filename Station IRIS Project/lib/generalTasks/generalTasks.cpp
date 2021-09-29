// Inclusões
#include "generalTasks.h"

// Instâncias externas
extern DHT dht;

// Funções
void configBegin()
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

void resetClear()
{
  uint16_t counter = 0;
  while (digitalRead(pin_resetEEPROM))
  {
    delay(1000);
    counter++;
    if (counter == resetTmrOF)
    {
      clear_EEPROM(0, EEPROM_SIZE);
      resetModule();
    }
  }
}

void valve(uint8_t state)
{
  uint8_t running = 0;
  static unsigned long tValve = 0;
  if (state)
  {
    if ((millis() - tValve) < 5000 && !running)
    {
      digitalWrite(valvePin1, 0);
      digitalWrite(valvePin2, 1);
      running = 1;
    }
    if ((millis() - tValve) > 5000)
    {
      running = 0;
      tValve = millis();
    }
    }
  else if (!state)
  {
    if ((millis() - tValve) < 5000 && !running)
    {
      digitalWrite(valvePin1, 1);
      digitalWrite(valvePin2, 0);
      running = 1;
    }
    if ((millis() - tValve) > 5000)
    {
      running = 0;
      tValve = millis();
    }
  }
}