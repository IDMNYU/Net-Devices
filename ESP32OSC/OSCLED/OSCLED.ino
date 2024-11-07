// physical pixel OSC
#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

char ssid[] = "sebadoh";  // your network SSID (name)
char pass[] = "bakesale";    // your network password

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;
OSCErrorCode error;
const unsigned int localPort = 13001;  // local port to listen for UDP packets 

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  while(!Serial){;;}
  // Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // logon to network
  WiFi.begin(ssid, pass);

  // wait to get on
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  // print put local IP address and verify port
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(localPort);
}

// handler to deal with received data
// argument is the OSC received 
void led(OSCMessage &msg) {
  int brightness = msg.getInt(0); // read the value from the OSC message
  analogWrite(LED_BUILTIN, onOff);
  Serial.print("/led: ");
  Serial.println(brightness);
}

void loop() {
  // receive stuff
  // instance of the library listens in the background
  OSCMessage msg;
  // variable to hold packet size when received
  int size = udp.parsePacket();

  // if there's soemthing in the buffer
  if (size > 0) {  // read it
    while (size--) { // this iterates over all vlaues in the udp packet
      msg.fill(udp.read());
    }
    if (!msg.hasError()) {   // if our message is error free, send it to the led function declared above
      msg.dispatch("/led", led); // call the led() fxn and pass the value of the /led path
    } else {
      error = msg.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
}