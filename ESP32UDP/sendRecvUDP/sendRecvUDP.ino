#include <WiFi.h>
#include <WiFiUdp.h>

// WiFi network name and password:
const char ssid[] = "sebadoh";
const char pass[] = "bakesale";

//IP address to send UDP data to:
// either use the ip address of the server or
// a network broadcast address
const char remoteip[] = "192.168.1.255";  // x.x.x.255 is the broadcast addr
const int remoteport = 13001;

char incomingPacket[255];
char reply[] = "got it";

//The udp library class
WiFiUDP udp;

void setup() {
  // Initilize hardware serial:
  Serial.begin(115200);
  while (!Serial) {
    ;
    ;
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
  int packetSize = udp.parsePacket();
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(udp.remotePort());

    // read the packet into packetBufffer
    int len = udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = 0;
    }
    Serial.println("Contents:");
    Serial.println(incomingPacket);
    //Send a packet
    udp.beginPacket(remoteip, remoteport);
    udp.print("Seconds since boot: ");
    udp.print(millis() / 1000);
    udp.endPacket();
  }
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
