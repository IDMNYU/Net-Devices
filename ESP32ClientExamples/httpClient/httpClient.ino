#include <WiFi.h>
#include <HTTPClient.h>

char ssid[] = "NetworkName";        // your network SSID (name)
char pass[] = "Password";  // your network password

HTTPClient client;

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
}

void loop() {

  Serial.println("HTTP begin...");
  // configure server and url
  client.begin("http://example.com/index.html");  //HTTP

  Serial.println("HTTP GET request going out");
  // start connection and send HTTP header
  int httpCode = client.GET();

  // httpCode will be negative on error
  if (httpCode > 0) {
    // HTTP header has been sent and server response header has been handled
    Serial.print("HTTP GET code: ");
    Serial.println(httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      String payload = client.getString(); // read the file content
      Serial.println(payload); // print it out
    }
  } else {
    Serial.print("HTTP GET failed, error: ");
    Serial.println(client.errorToString(httpCode).c_str());
  }

  client.end();

  delay(5000);
}
