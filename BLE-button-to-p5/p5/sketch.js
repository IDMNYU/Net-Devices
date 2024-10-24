// The serviceUuid must match the serviceUuid of the device you would like to connect
const serviceUuid = "dc52d0c5-efb0-4d41-a779-98f0422da984";
let myCharacteristic;
let latestData = 'nothing yet';
let myBLE;

function setup() {
  createCanvas(400, 400);
  // Create a p5ble class
  myBLE = new p5ble();

  textSize(20);
  textAlign(CENTER, CENTER);

  // Create a 'Connect' button
  const connectButton = createButton('Connect')
  connectButton.mousePressed(connectToBle);
}

function connectToBle() {
  // Connect to a device by passing the service UUID
  myBLE.connect(serviceUuid, gotCharacteristics);
}

// A function that will be called once got characteristics
function gotCharacteristics(error, characteristics) {
  if (error) console.log('error: ', error);
  console.log('characteristics: ', characteristics);
  myCharacteristic = characteristics[0];
  // Read the value of the first characteristic
  myBLE.read(myCharacteristic, gotValue);
}

// A function that will be called once got values
function gotValue(error, value) {
  if (error) console.log('error: ', error);
  console.log('value: ', value);
  latestData = value;
  // After getting a value, call p5ble.read() again to get the value again
  myBLE.read(myCharacteristic, gotValue);

}

function draw() {
  background(255, 255, 255);
  fill(0, 0, 0);
  textAlign(LEFT);

  text('received a ' + latestData, 10, 10); // print the data to the sketch

  // in this example, we are reciving a 0 and a 1
  // if the button is not pressed we get a 0
  if (latestData === 0) {
    ellipse(width / 2, height / 2, 100, 100);
  } else { // if it is pressed, we get a 1
    rectMode(CENTER);
    rect(width / 2, height / 2, 100, 100);
  }
}