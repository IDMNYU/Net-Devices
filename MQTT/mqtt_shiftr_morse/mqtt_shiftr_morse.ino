// for use with https://editor.p5js.org/shfitz/sketches/2QRxiBUEk

#include <WiFi.h>
#include <ArduinoMqttClient.h>
int ledPin = 13;

//For letters
char* letters[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",    // A-I
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",  // J-R
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."          // S-Z
};

//For Numbers
char* numbers[] = {
  "-----", ".----", "..---", "...--", "....-", ".....",
  "-....", "--...", "---..", "----."
};

int dotDelay = 200;

const char ssid[] = "sebadoh";
const char pass[] = "bakesale";

WiFiClient client;
MqttClient mqttClient(client);

unsigned long lastMillis = 0;

const char broker[] = "lampy.cloud.shiftr.io";
int port = 1883;
const char topic[] = "morse";

void setup() {
  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);
  while (!Serial) { ; }

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
  mqttClient.setId("reader");

  // You can provide a username and password for authentication
  mqttClient.setUsernamePassword("lampy", "6WPWBDct3jb7qiLy");

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

void messageReceived(String cha) {

  for (int x = 0; x <= cha.length(); x++) {
    char ch = cha[x];
    if (ch >= 'a' && ch <= 'z') {
      flashSequence(letters[ch - 'a']);
    } else if (ch >= 'A' && ch <= 'Z') {
      flashSequence(letters[ch - 'A']);
    } else if (ch >= '0' && ch <= '9') {
      flashSequence(numbers[ch - '0']);
    } else if (ch == ' ') {
      delay(dotDelay * 4);
    }
    Serial.print(ch);
  }
  Serial.println();
}

void flashSequence(char* sequence) {
  int i = 0;
  while (sequence[i] != NULL) {
    flashDotOrDash(sequence[i]);
    i++;
  }
  delay(dotDelay * 3);
}

void flashDotOrDash(char dotOrDash) {
  digitalWrite(ledPin, HIGH);
  if (dotOrDash == '.') {
    delay(dotDelay);
    Serial.print(".");
  } else  // must be a -
  {
    delay(dotDelay * 3);
    Serial.print("-");
  }
  digitalWrite(ledPin, LOW);
  delay(dotDelay);
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

    // print the contents
    String dataRec = "";
    while (mqttClient.available()) {
      dataRec += (char)mqttClient.read();
    }
    Serial.println(dataRec);

    messageReceived(dataRec);
  }
}
