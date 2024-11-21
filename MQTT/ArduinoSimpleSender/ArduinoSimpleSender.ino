#include <ArduinoMqttClient.h>
#include <WiFi.h>

char ssid[] = "SECRET_SSID";  // your network SSID (name)
char pass[] = "SECRET_PASS";  // your network password

WiFiClient client;
MqttClient mqttClient(client);

const char broker[] = "XXXX.cloud.shiftr.io";
int port = 1883;
const char topic[] = "topic";

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to WiFi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);  // Configure ESP32 in STA Mode
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("You're connected to the network");
  Serial.println();

  // You can provide a unique client ID, if not set the library uses Arduino-millis()
  // Each client must have a unique client ID
  mqttClient.setId("ESP32 sender");

  // You can provide a username and password for authentication
  mqttClient.setUsernamePassword("XXXX", "password");

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1)
      ;
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}

void loop() {
  // call poll() regularly to allow the library to send MQTT keepalive messages which
  // avoid the broker from disconnecting 
  mqttClient.poll();

  int val = analogRead(A0);
  Serial.print("Sending message to topic: ");
  Serial.println(topic);
  Serial.println(val);

  // send message
  mqttClient.beginMessage(topic);
  mqttClient.print(val);
  mqttClient.endMessage();

  delay(33); // no need to update more than 30x a second ;)
}
