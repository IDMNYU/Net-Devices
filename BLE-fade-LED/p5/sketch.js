const serviceUuid = "42a543da-b786-4882-8ed6-c0f8c39e8e1b";
let myCharacteristic;
let slide;
let myBLE;
let b;

function setup() {
  myBLE = new p5ble();

  // Create a 'Connect' button
  b = createButton('Connect')
  b.mousePressed(connectToBle);
  b.position(10, 10);

  // Create a text input
  slider = createSlider(0, 255, 0);
  slider.position(10, 50);

}

function connectToBle() {
  // Connect to a device by passing the service UUID
  myBLE.connect(serviceUuid, gotCharacteristics);
}

function gotCharacteristics(error, characteristics) {
  if (error) console.log('error: ', error);
  console.log('characteristics: ', characteristics);
  // Set the first characteristic as myCharacteristic
  myCharacteristic = characteristics[0];
}

function writeToBle() {
  const inputValue = slider.value();
  // Write the value of the input to the myCharacteristic
  myBLE.write(myCharacteristic, inputValue);
}

function mouseDragged() {
  writeToBle();
}