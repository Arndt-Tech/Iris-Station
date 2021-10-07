// Inclusões
#include "generalFunctions.h"

// Instâncias externas
extern DHT dht;

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
  // Inicialização das tasks
  setupTasks();
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

void setupBuzzer()
{
  pinMode(buzzerPin, OUTPUT);
  ledcAttachPin(buzzerPin, buzzChannel(0));
  ledcSetup(buzzChannel(0), 1000, 10);
}

void error(int8_t long_buzz, int8_t short_buzz)
{
  for (uint8_t i = 0; i < long_buzz; i++)
    long_bip();
  for (uint8_t i = 0; i < short_buzz; i++)
    short_bip();
}

void long_bip()
{
  ledcWrite(buzzChannel(0), 1000);
  delay(700);
  ledcWrite(buzzChannel(0), 0);
  delay(350);
}

void short_bip()
{
  ledcWrite(buzzChannel(0), 1000);
  delay(200);
  ledcWrite(buzzChannel(0), 0);
  delay(200);
}