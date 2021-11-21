#include "main.hpp"

stt::Station iris;

void setup()
{
#if _DEBUG_MODE_
  Serial.begin(115200);
  delay(1000);
  Serial.write('\n');
  Serial.write('\n');
  Serial.println("******************************");
  Serial.println("**********DEBUG MODE**********");
  Serial.println("******************************");
  Serial.write('\n');
#elif !_DEBUG_MODE_
#endif
  xTaskCreatePinnedToCore(taskReset, "taskReset", STACK(2048), NULL, PRIORITY(4), NULL, CORE(1));
  iris.begin();
  setupTasks();
}
void loop()
{

#if _DEBUG_MODE_
  Serial.write('\n');
  Serial.write('\n');
  Serial.write('\n');
  Serial.println("*********************************************************");
  Serial.println("******************** IRIS DATA DEBUG ********************");
  Serial.println("*********************************************************");
  Serial.write('\n');
  Serial.write('\n');
  Serial.write('\n');
  Serial.println("------------------------------------------------------------");
  Serial.println("IRIS -> Bluetooth");
  Serial.write('\n');
  Serial.print("Connection Status: "); Serial.println(iris.manage.BLE().getConnectionStatus() == true ? "Enable." : "Disable.");
  Serial.write('\n');
  Serial.println("------------------------------------------------------------");
  Serial.println("IRIS -> GPIO");
  Serial.write('\n');
  Serial.print("Valve Status: "); Serial.println(iris.manage.GPIO().valve.status() == true ? "Active." : "Disable.");
  Serial.println("Sensor temperature: " + String(iris.manage.GPIO().weather.getTemperature()));
  Serial.println("Sensor humidity: " + String(iris.manage.GPIO().weather.getHumidity()));
  Serial.write('\n');
  Serial.println("------------------------------------------------------------");
  Serial.println("IRIS -> GPS");
  Serial.write('\n');
  Serial.print("GPS Status: "); Serial.println(iris.manage.GPS().getStatus() == true ? "Active." : "Inactive.");
  Serial.println("Latitude: " + String(iris.manage.GPS().getLatitude(), 6));
  Serial.println("Longitude: " + String(iris.manage.GPS().getLongitude(), 6));
  Serial.write('\n');
  Serial.println("------------------------------------------------------------");
  Serial.println("IRIS -> LoRa -> Transmission package");
  Serial.write('\n');
  Serial.println("Receiver: " + String(iris.manage.LoRa().packet.transmit.get.senderAddr()));
  Serial.println("Sender: " + String(iris.manage.LoRa().packet.transmit.get.localAddr()));
  Serial.println("Temperature to send: " + String(iris.manage.LoRa().packet.transmit.get.temperature()));
  Serial.println("Humidity to send: " + String(iris.manage.LoRa().packet.transmit.get.humidity()));
  Serial.println("Latitude to send: " + String(iris.manage.LoRa().packet.transmit.get.latitude()));
  Serial.println("Longitude to send: " + String(iris.manage.LoRa().packet.transmit.get.longitude()));
  Serial.println("Package size: " + String(iris.manage.LoRa().packet.transmit.get.size()) + " bytes.");
  Serial.write('\n');
  Serial.println("------------------------------------------------------------");
  Serial.println("IRIS -> LoRa -> Reception package");
  Serial.write('\n');
  Serial.print("Request: "); Serial.println(iris.manage.LoRa().checkRequest() == true ? "Yes." : "No.");
  Serial.println("Last receiver: " + String(iris.manage.LoRa().packet.receive.get.receiverAddr()));
  Serial.println("Last sender: " + String(iris.manage.LoRa().packet.receive.get.senderAddr()));
  Serial.print("Last valve status: "); Serial.println(iris.manage.LoRa().packet.receive.get.valveStatus() == true ? "Activate." : "Disable.");
  Serial.println("Last package size: " + String(iris.manage.LoRa().packet.receive.get.size()) + " bytes.");
  Serial.println("Last signal: " + String(iris.manage.LoRa().packet.receive.get.signal()) + " dBm.");
  Serial.write('\n');
  Serial.println("------------------------------------------------------------");
  Serial.write('\n');
  Serial.write('\n');
  Serial.write('\n');
  Serial.write('\n');
  Serial.write('\n');
  delay(5000);
#elif !_DEBUG_MODE_
#endif
}