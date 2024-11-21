#include <ArduinoMqttClient.h>
#include <WiFi.h>

char ssid[] = "SECRET_SSID";  // your network SSID (name)
char pass[] = "SECRET_PASS";  // your network password 

WiFiClient client;
MqttClient mqttClient(client);
int val;

const char broker[] = "XXXXXX.cloud.shiftr.io";
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
  mqttClient.setId("RP2040 reader");

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

  Serial.print("Subscribing to topic: ");
  Serial.println(topic);
  Serial.println();

  // subscribe to a topic
  mqttClient.subscribe(topic);

  // topics can be unsubscribed using:
  // mqttClient.unsubscribe(topic);

  Serial.print("Waiting for messages on topic: ");
  Serial.println(topic);
  Serial.println();
}

void loop() {
  int messageSize = mqttClient.parseMessage();
  if (messageSize) {
    // we received a message, print out the topic and contents
    Serial.print("Received a message with topic '");
    Serial.print(mqttClient.messageTopic());
    Serial.print("', length ");
    Serial.print(messageSize);
    Serial.println(" bytes:");

    // use the Svtream interface to print the contents
    String dataRec =""; 
    while (mqttClient.available()) {
      dataRec += (char)mqttClient.read();
    }
    Serial.println(dataRec);
    val = dataRec.toInt();
    analogWrite(LED_BUILTIN, val/16);
    Serial.println(val);
  }
}
