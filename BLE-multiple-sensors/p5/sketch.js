// multiple sensors with a nano esp32
// that has 2 pots and a button, like a joystick
// arduino code at https://gist.github.com/shfitz/9d14e612b1d21ae1c90d9e22ee0bb728
// service - must match Arduino service
const serviceUuid = "da5286e1-84d4-4c40-842a-96014da7914c";
// characteristics - must match Arduino characteristicss
const characteristicsUUID = {
  button:"718ea9eb-7d9a-42dd-b8a2-a02599268977",
  xAxis:"b499b98a-3fc2-44ef-a403-32b899e502bd",
  yAxis:"e6b4f277-a5b3-4a5d-8b6a-a3757e0fbb13"
}
let buttonCharacteristic;
let xCharacteristic;
let yCharacteristic;
let buttonValue = 0;
let xVal = 0;
let yVal = 0;
let myBLE;

function setup() {
  // Create a p5ble class
  myBLE = new p5ble();

  createCanvas(windowWidth, windowHeight);
  background(255);

  // connect button
  const connectButton = createButton("Connect");
  connectButton.position(10, 10);
  connectButton.mousePressed(connectBLE);

}

function connectBLE() {
  // Connect to a device by passing the service UUID
  myBLE.connect(serviceUuid, gotCharacteristics);
}

// A function that will be called once we have gotten characteristics
function gotCharacteristics(error, characteristics) {
  // if there's an error
  if (error) console.log('error: ', error);
  // iterate through the array of characteristics
  for(let i = 0; i < characteristics.length;i++){
    // if it's the button characteristic
    if(characteristics[i].uuid == characteristicsUUID.button){
      buttonCharacteristic = characteristics[i];
      // start the notification and identify the callback to handle button data
      myBLE.startNotifications(buttonCharacteristic, handleButton);
    }else if(characteristics[i].uuid == characteristicsUUID.xAxis){
      // if it's the pot for the xaxis
      xCharacteristic = characteristics[i];
      myBLE.startNotifications(xCharacteristic,handleXSensor);
    }else if(characteristics[i].uuid == characteristicsUUID.yAxis){
      // if it's the pot for the yaxis
      yCharacteristic = characteristics[i];
      myBLE.startNotifications(yCharacteristic, handleYSensor);
    }else{
      console.log("i got nothing");
    }
  } 
}

// function for once a button update happens
function handleButton(data) {
  console.log('button: ', data);
  buttonValue = Number(data);
}

// function for handling x pot data
function handleXSensor(data) {
  console.log('x: ', data);
  xVal = int(data);
}

// function for handling y pot data
function handleYSensor(data) {
  console.log('y: ', data);
  yVal = int(data);
}

function draw() {
  noStroke();
  fill(0);

  // erase bg if button pressd
  if(buttonValue>0){
    background(255);
  }
  // map values 
  let x = map(xVal, 0, 255, 0, height);
  let y = map(yVal, 0, 255, 0, width);
  
  ellipse(x, y, 10);
}