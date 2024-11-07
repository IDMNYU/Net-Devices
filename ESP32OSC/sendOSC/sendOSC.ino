#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

char ssid[] = "sebadoh";   // your network SSID (name)
char pass[] = "bakesale";  // your network password

// A udp instance to let us send and receive packets over udp
WiFiUDP udp;
OSCErrorCode error;
const unsigned int localPort = 13001;  // local port to listen for udp packets
unsigned int ledState = LOW;
const unsigned int outPort = 13002;       // port we send packets to
const IPAddress outIp(192, 168, 1, 255);  // IP address of your computer
// If you don't knwo the address of your computer, or want to communicate with all machines on a subnet,
// use xxx.xxx.xxx.255 which is the broadcast address
// const IPAddress outIp(192, 168, 0, 255);
int pbuttval;  // variable to hold the value of a button

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
    ;
  };
  // configure the input pin
  pinMode(2, INPUT_PULLUP);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting udp");
  udp.begin(localPort);
  Serial.print("Local port: ");

  Serial.println(localPort);
}

void loop() {

  // send stuff
  int buttval = digitalRead(2);
  int potVal = analogRead(A7);

  if (buttval != pbuttval) {          // if the button state has changed and is now LOW
    OSCMessage msg("/trigger");       // add the route
    msg.add(buttval);                 // add the value
    udp.beginPacket(outIp, outPort);  // prepare the packet
    msg.send(udp);                    // send it out
    udp.endPacket();                  // end the packet
  }

  OSCMessage msg("/freq");          // add the route
  msg.add(potVal);                  // add the value
  udp.beginPacket(outIp, outPort);  // prepare the packet
  msg.send(udp);                    // send it out
  udp.endPacket();                  // end the packet
  msg.empty();                      // clear your messages


  pbuttval = buttval;  // updated previous button state for next loop
}