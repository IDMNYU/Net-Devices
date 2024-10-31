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
    int r = analogRead(A0)/16;
    int g = analogRead(A1)/16;
    int b = analogRead(A2)/16;

    // send the HTTP response body
    const char htmlString[] = R"(<!DOCTYPE html><html lang="en"><head><title="hi there"></title> <meta http-equiv="refresh" content="5"></head><body style="background-color:rgb()";
    const char punky[]=R"(<svg width="797" height="600" xmlns="http://www.w3.org/2000/svg">
  <ellipse ry="179" rx="209.5" id="svg_1" cy="349" cx="410" stroke="#000" fill="#FABA6F"/>
  <ellipse ry="34.5" rx="21.5" id="svg_3" cy="287.5" cx="511" stroke="#000" fill="#000000"/>
  <polyline stroke-linecap="round" id="svg_9" points="1102.5,341 1102.5,341 " stroke="#000" fill="none"/>
  <line id="svg_11" y2="438" x2="366.5" y1="342" x1="295.5" stroke-width="28" stroke="#000" fill="none"/>
  <line id="svg_12" y2="444" x2="355.5" y1="362" x1="411.5" stroke-width="28" stroke="#000" fill="none"/>
  <line id="svg_13" y2="349" x2="408.5" y1="422" x1="449.5" stroke-width="28" stroke="#000" fill="none"/>
  <line id="svg_14" y2="432" x2="439.5" y1="362" x1="489.5" stroke-width="28" stroke="#000" fill="none"/>
  <line id="svg_15" y2="350" x2="485.5" y1="416" x1="518.5" stroke-width="28" stroke="#000" fill="none"/>
  <line id="svg_16" y2="403" x2="516.5" y1="342" x1="554.5" stroke-width="28" stroke="#000" fill="none"/>
  <ellipse ry="34.5" rx="21.5" id="svg_17" cy="284.5" cx="372" stroke="#000" fill="#000000"/>
  <path stroke-width="63" d="m323.5,136c0,0 0,0 1,0c2,0 4,0 5,0c1,0 2,1 3,1c1,0 2.29291,0.29289 3,1c0.70709,0.70711 0.69345,1.4588 2,2c0.92389,0.38269 2.07611,0.61731 3,1c1.30655,0.5412 2.02676,1.77025 3,2c2.17624,0.51375 3.07611,1.61731 4,2c2.61313,1.0824 3,2 4,3c0,0 1.07611,0.61731 2,1c1.30655,0.5412 1.07611,1.61731 2,2c1.30655,0.5412 2.1731,0.85274 3,2c1.30746,1.814 2.186,2.69255 4,4c1.14728,0.8269 3,3 4,4c1,1 2,2 4,4c1,1 2,3 3,4c1,1 2.23462,2.15224 3,4c0.5412,1.30656 2,2 3,3c1,1 0.61731,1.07613 1,2c0.5412,1.30656 1,1 2,2c1,1 1.1731,1.85274 2,3c1.30746,1.814 0.69254,3.186 2,5c0.8269,1.14726 2.4588,2.69344 3,4c0.38269,0.92387 0.61731,3.07613 1,4c0.5412,1.30656 1.4588,1.69344 2,3c0.38269,0.92387 0.61731,1.07613 1,2c0.5412,1.30656 0.61731,2.07613 1,3c0.5412,1.30656 1,1 1,2c0,0 0,0 0,1c0,0 0,0 0,1c0,0 0,0 0,0c0,0 0,-1 0,-1c0,0 0,-1 0,-1c0,0 0.29291,-0.29289 1,-1c2.82843,-2.82843 5,-1 7,-1c2,0 4.02582,-0.67964 6,-1c3.12143,-0.50655 5.87857,-1.49345 9,-2c2.96127,-0.48055 5,-1 8,-2c3,-1 4.87857,-1.49345 8,-2c1.97418,-0.32036 5.0535,-0.5405 7,-1c2.17624,-0.51375 4,-1 6,-1c1,0 2,0 3,0c1,0 1,0 2,0c0,0 0,0 0,0c1,0 1,0 1,0c0,0 0,0 0,1c0,0 0,0 0,0c0,-1 -0.48627,-1.82375 -1,-4c-0.4595,-1.9465 -1.31073,-8.08025 -2,-11c-1.0275,-4.35251 -2.1731,-5.85274 -3,-7c-1.30746,-1.814 -1.4588,-2.69344 -2,-4c-0.38269,-0.92387 -1.1731,-2.85274 -2,-4c-1.30746,-1.814 -3,-4 -5,-6c-2,-2 -4.1731,-4.85274 -5,-6c-1.30746,-1.814 -3.48627,-2.82375 -4,-5c-0.22977,-0.97325 -0.29291,-1.29289 -1,-2c-0.70709,-0.70711 -1.07611,-0.61731 -2,-1c-1.30655,-0.5412 -1.59399,-2.513 -4,-4c-2.69,-1.66251 -5.90634,-3.10341 -7,-4c-2.78833,-2.28587 -4.88153,-4.19028 -8,-6c-1.93399,-1.12234 -5.04291,-3.71022 -6,-4c-3.45084,-1.04483 -5.21167,-3.71412 -8,-6c-2.18735,-1.79319 -6.13202,-2.75532 -10,-5c-3.11847,-1.80972 -5,-3 -8,-4c-3,-1 -4.69345,-2.4588 -6,-3c-1.84775,-0.76537 -2,-1 -3,-1c-1,0 -1,0 -1,0c-1,0 -3.82376,-0.48626 -6,-1c-2.91974,-0.68926 -7,-1 -8,-1c-3,0 -6,-1 -7,-1c-1,0 -1,-1 -1,-1c0,0 0,0 0,0c0,0 0,0 -1,0c0,0 0,0 -1,0c0,0 -0.29291,0.29289 -1,1c-0.70709,0.70711 -0.85272,2.1731 -2,3c-1.814,1.30745 -3.31021,1.71899 -6,5c-2.28589,2.78832 -3.69254,4.186 -5,6c-0.8269,1.14727 -2,2 -3,3c0,0 -1,1 -1,1c0,1 -1,1 -1,1c0,1 0,1 0,1c0,0 0,1 0,1c0,0 0,0 0,1c0,0 0,0 0,0c1,1 1,1 1,1c0,0 0,1 0,1c0,0 0,0 1,0c0,0 0,1 0,1l0,0l0,0l0,-1" id="svg_18" stroke="#000" fill="none"/>
</svg>)";
    client.println(htmlString);
    client.print(r);
    client.print(",");
    client.print(g);
    client.print(",");
    client.print(b);
    client.print(");\"><h1>Happy Halloween!</h1>");
    
    client.print(punky);
    
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
