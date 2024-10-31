#include <WiFi.h>

const char ssid[] = "sebadoh";        // change your network SSID (name)
const char pass[] = "bakesale";  // change your network password (use for WPA, or use as key for WEP)

WiFiServer server(80);

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while(!Serial){;;}
  WiFi.mode(WIFI_STA); /* Configure ESP32 in STA Mode */
  Serial.print("Attempting to connect to SSID: ");
  WiFi.begin(ssid, pass);
  // attempt to connect to WiFi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println(ssid);
  server.begin();
  // you're connected now, so print out the status:
  printWifiStatus();
}

void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    // read the HTTP request header line by line
    while (client.connected()) {
      if (client.available()) {
        String header = client.readStringUntil('\n');  // read the header line of HTTP request

        if (header.equals("\r"))  // the end of HTTP request
          break;
        Serial.println(header);  // print HTTP request to Serial Monitor
      }
    }

    // send the HTTP response
    // send the HTTP response header
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");  // the connection will be closed after completion of the response
    client.println();                     // the separator between HTTP header and body
    // send the HTTP response body
    const char htmlString[] = R"(<!DOCTYPE html><html lang="en"><head><title="hi there"></title> <meta http-equiv="refresh" content="5"></head><body>The sensor attached to A0 is reading)";
    client.println(htmlString);
    int val = getSensor();
    client.print(val);
    client.println("</body></html>");

    client.flush();

    // give the web browser time to receive the data
    delay(10);

    // close the connection:
    client.stop();
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

int getSensor() {
  int sensorVal = analogRead(A0);
  return sensorVal; 
}
