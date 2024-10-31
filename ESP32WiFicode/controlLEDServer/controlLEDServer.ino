#include <WiFi.h>

const char ssid[] = "sebadoh";   // your network SSID (name)
const char pass[] = "bakesale";  // your network password

WiFiServer server(80);

void setup() {
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  Serial.begin(115200);  // initialize serial communication
  // while (!Serial) {
  //   ;;
  // }
  WiFi.mode(WIFI_STA); /* Configure ESP32 in STA Mode */
  Serial.print("Attempting to connect to Network named: ");
  Serial.println(ssid);  // print the network name (SSID);
  // Connect to network.
  WiFi.begin(ssid, pass);
  // wait until connected
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  server.begin();     // start the web server on port 80
  printWifiStatus();  // you're connected now, so print out the status
}


void loop() {
  WiFiClient client = server.available();  // listen for incoming clients

  if (client) {                    // if you get a client,
    Serial.println("new client");  // print a message out the serial port
    String currentLine = "";       // make a String to hold incoming data from the client
    while (client.connected()) {   // loop while the client's connected
      if (client.available()) {    // if there's bytes to read from the client,
        char c = client.read();    // read a byte, then
        Serial.write(c);           // print it out the serial monitor
        if (c == '\n') {           // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("<style>");
            client.print(".container {margin: 0 auto; text-align: center; margin-top: 100px;}");
            client.print("button {color: white; width: 100px; height: 100px;");
            client.print("border-radius: 50%; margin: 20px; border: none; font-size: 20px; outline: none; transition: all 0.2s;}");
            client.print(".red{background-color: rgb(196, 39, 39);}");
            client.print(".green{background-color: rgb(39, 121, 39);}");
            client.print(".blue {background-color: rgb(5, 87, 180);}");
            client.print(".off{background-color: grey;}");
            client.print("button:hover{cursor: pointer; opacity: 0.7;}");
            client.print("</style>");
            client.print("<div class='container'>");
            client.print("<button class='red' type='submit' onmousedown='location.href=\"/RH\"'>ON</button>");
            client.print("<button class='off' type='submit' onmousedown='location.href=\"/RL\"'>OFF</button><br>");
            client.print("<button class='green' type='submit' onmousedown='location.href=\"/GH\"'>ON</button>");
            client.print("<button class='off' type='submit' onmousedown='location.href=\"/GL\"'>OFF</button><br>");
            client.print("<button class='blue' type='submit' onmousedown='location.href=\"/BH\"'>ON</button>");
            client.print("<button class='off' type='submit' onmousedown='location.href=\"/BL\"'>OFF</button>");
            client.print("</div>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {  // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was /X
        if (currentLine.endsWith("GET /RH")) {
          digitalWrite(LEDR, LOW);
        }
        if (currentLine.endsWith("GET /RL")) {
          digitalWrite(LEDR, HIGH);
        }
        if (currentLine.endsWith("GET /GH")) {
          digitalWrite(LEDG, LOW);
        }
        if (currentLine.endsWith("GET /GL")) {
          digitalWrite(LEDG, HIGH);
        }
        if (currentLine.endsWith("GET /BH")) {
          digitalWrite(LEDB, LOW);
        }
        if (currentLine.endsWith("GET /BL")) {
          digitalWrite(LEDB, HIGH);
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}