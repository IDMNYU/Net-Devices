
// include the library
#include <ArduinoBLE.h>

BLEService ledService("42a543da-b786-4882-8ed6-c0f8c39e8e1b"); // BLE LED Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEIntCharacteristic ledCharacteristic("4f5c532d-0ce1-4537-990b-6716876a6db1", BLERead | BLEWrite);

const int ledPin = 13; // pin to use for the builtin LED

void setup() {
  // for debugging if necessary
  Serial.begin(9600);
  // while(!Serial){;}

  // set LED pin to output
  pinMode(ledPin, OUTPUT);

  // start BLE
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  // set name and adverstise the service
  BLE.setLocalName("LED Fader");
  BLE.setAdvertisedService(ledService);

  // add the characteristic to the service
  ledService.addCharacteristic(ledCharacteristic);

  // add service
  BLE.addService(ledService);

  // set the initial value for the characeristic:
  ledCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");
}

void loop() {
  // listen for BLE central to connect
  BLEDevice central = BLE.central();

  // if a central is connected 
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is connected 
    while (central.connected()) {
      // if central wrote to the characteristic,
      // use the value to control the LED:
      if (ledCharacteristic.written()) {
        int brightness = ledCharacteristic.value();
        analogWrite(ledPin, brightness);
      }
    }

    // when central disconnects, print it out:
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}