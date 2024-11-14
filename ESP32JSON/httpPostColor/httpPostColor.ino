#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

char ssid[] = "Network";        // your network SSID (name)
char pass[] = "Password";  // your network password

String serverName = "http://colormind.io/api/";

HTTPClient client;

unsigned long previousMillis = 0;  // will store last time client req was updated
const long interval = 360000;      // interval at which to make a req (milliseconds)

void setup() {
  Serial.begin(115200);
  while (!Serial) { ; };

  // Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // log on to network
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

  // make the request
  makeRequest();
}

void makeRequest() {
  // Your Domain name with URL path or IP address with path
  client.begin(serverName);
  // Specify content-type header
  client.addHeader("Content-Type", "application/json");
  // Data to send with HTTP POST
  String httpRequestData = "{\"model\":\"default\"}";
  // Send HTTP POST request
  int httpCode = client.POST(httpRequestData);

  Serial.print("HTTP Response code: ");
  Serial.println(httpCode);
  if (httpCode == HTTP_CODE_OK) {
    // parse the data
    String payload = client.getString();
    parseJSON(payload);
  } else {  // if we have a problem, report it
    Serial.print("HTTP POST... failed, error: ");
    Serial.println(http.errorToString(httpCode).c_str());
  }
  // Free resources
  client.end();
}

void loop() {
}

void parseJSON(String response) {
  // create a new JSON datatype with the info from the server
  JSONVar JSONresp = JSON.parse(response);
  // print it out!
  Serial.println(JSONresp);
}