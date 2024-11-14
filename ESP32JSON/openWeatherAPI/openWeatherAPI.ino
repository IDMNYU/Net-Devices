#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

char ssid[] = "Network";        // your network SSID (name)
char pass[] = "Password";  // your network password

String apiKey = "YOUR_API_KEY_HERE";
String url = "https://api.openweathermap.org/data/2.5/weather?lat=40.6501&lon=-73.9496&units=imperial&appid=";

HTTPClient client;

unsigned long previousMillis = 0;  // will store last time client req was updated
const long interval = 360000;       // interval at which to make a req (milliseconds)

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


  url += apiKey;
  // start the client with the URL we're going to call
  client.begin(url);  
  // make the request
  makeRequest();
}

void loop() {
  // only call it every minute
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the time you made the req
    previousMillis = currentMillis;
    // make the request!
    makeRequest();
  }
}

void makeRequest() {
  Serial.println("client GET...");
  // start connection and send HTTP header
  int httpCode = client.GET();

  // httpCode will be negative on error
  if (httpCode > 0) {
    // HTTP header has been sent and the server response header has been handled
    Serial.print("HTTP GET... code:");
    Serial.println(httpCode);

    // if the file is found at the server
    if (httpCode == HTTP_CODE_OK) {
      // parse the data
      String payload = client.getString();
      parseJSON(payload);
    }
  } else { // if we have a problem, report it
    Serial.print("HTTP GET... failed, error: ");
    Serial.println(client.errorToString(httpCode).c_str());
  }
  // close the connection
  client.end();
}

// fxn that parses the data
void parseJSON(String response) {
  // create a new JSON datatype with the info from the server 
  JSONVar JSONresp = JSON.parse(response);
  // print it out!
  Serial.println(JSONresp);

  // print the keys
  Serial.print("keys: ");
  Serial.println(JSONresp.keys());


  Serial.println(JSONresp["weather"][0]["description"]);

}
