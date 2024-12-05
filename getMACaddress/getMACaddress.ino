#include <WiFi.h>

char ssid[] = "yourNetwork";  // bogus name just to start the library

byte mac[6];  // the MAC address of your board

void setup() {
  Serial.begin(9600);
  while(!Serial){};
  WiFi.begin(ssid);

  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);
}

void loop() {}
