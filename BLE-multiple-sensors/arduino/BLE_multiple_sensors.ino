#include <ArduinoBLE.h>
// set up sensor pins
const int potXPin = A0;
const int potYPin = A1; 
const int buttonPin = 2; 

BLEService sensorService("da5286e1-84d4-4c40-842a-96014da7914c"); // create service

// create characteristics and allow remote device to read and write
BLEIntCharacteristic xCharacteristic("b499b98a-3fc2-44ef-a403-32b899e502bd", BLERead | BLENotify);
BLEIntCharacteristic yCharacteristic("e6b4f277-a5b3-4a5d-8b6a-a3757e0fbb13", BLERead | BLENotify);
BLEBoolCharacteristic buttonCharacteristic("718ea9eb-7d9a-42dd-b8a2-a02599268977", BLERead | BLENotify);

void setup() {
  Serial.begin(9600);
  // while (!Serial);

  pinMode(buttonPin, INPUT); // use button pin as an input

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  // set the local name peripheral advertises
  BLE.setLocalName("Arduino Joystick");

  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(sensorService);

  // add the characteristics to the service
  sensorService.addCharacteristic(xCharacteristic);
  sensorService.addCharacteristic(yCharacteristic);
  sensorService.addCharacteristic(buttonCharacteristic);

  // add the service
  BLE.addService(sensorService);

  //set init value
  xCharacteristic.writeValue(0);
  yCharacteristic.writeValue(0);
  buttonCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // poll for BLE events
  BLE.poll();
  // read the values of the pots and button
  int xValue = analogRead(potXPin);
  int yValue = analogRead(potYPin);
  bool buttonValue = digitalRead(buttonPin);
  // send the values
  // the writeValue is only sending a byte so we divide the analog sensor val to scale
  xCharacteristic.writeValue(xValue/8);
  yCharacteristic.writeValue(yValue/8);
  buttonCharacteristic.writeValue(buttonValue);
}