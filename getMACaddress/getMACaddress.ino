#include <WiFi.h>

void setup() {

  Serial.begin(115200);
  while (!Serial) {};

  String mac = WiFi.macAddress();

  // Print the MAC address to the Serial Monitor
  Serial.print("MAC Address: ");
  Serial.println(mac);
}

void loop() {
}
