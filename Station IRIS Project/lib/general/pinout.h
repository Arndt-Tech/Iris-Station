#ifndef _PINOUT_H
#define _PINOUT_H

/*|----------| Station Hardware Mapping |----------|*/

//   Buzzer pinout
#define buzzerPin 23 

//   DHT pinout
#define DHTpin 2
#define valvePin1 12
#define valvePin2 13

//   EEPROM pinout
#define pin_resetEEPROM 17

//   OLED pinout
#define OLED_SCL 15
#define OLED_SDA 4
#define OLED_RST 16

// GPS pinout
#define RX_GPS (22)
#define TX_GPS (25)

//   LoRa pinout
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DI00 26

#endif
