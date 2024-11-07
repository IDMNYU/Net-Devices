#include <WiFi.h>
#include <WiFiUdp.h>

// WiFi network name and password:
const char ssid[] = "sebadoh";
const char pass[] = "bakesale";

//IP address to send UDP data to:
// either use the ip address of the server or
// a network broadcast address
const char remoteip[] = "192.168.1.77";
const int remoteport = 13001;

//The udp library class
WiFiUDP udp;

void setup() {
  // Initilize hardware serial:
  Serial.begin(115200);
  while (!Serial) {
    ;;
  }
  //Connect to the WiFi network
  WiFi.mode(WIFI_STA); /* Configure ESP32 in STA Mode */
  Serial.print("Attempting to connect to SSID: ");
  WiFi.begin(ssid, pass);
  // attempt to connect to WiFi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println(ssid);
  // you're connected now, so print out the status:
  printWifiStatus();
  udp.begin(10000);
}

void loop() {
  
    //Send a packet
    udp.beginPacket(remoteip, remoteport);
    udp.print("Seconds since boot: ");
    udp.print(millis() / 1000);
    udp.endPacket();
    delay(500);  
}

void printWifiStatus() {
  // print your board's IP address:
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // print the received signal strength:
  Serial.print("signal strength (RSSI):");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}
