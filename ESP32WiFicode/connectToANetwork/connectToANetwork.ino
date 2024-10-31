#include <WiFi.h>

const char ssid[] = "sebadoh";
const char password[] = "bakesale";

void setup() {
  Serial.begin(115200);
  while (!Serial) { ; }
  pinMode(LED_BUILTIN, OUTPUT);  // set the LED pin mode


  // start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA); // Configure ESP32 in STA Mode
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println();
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); // local IP give sthe IP of the microcontroller

}
void loop(){

}