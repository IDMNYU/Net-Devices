//************************************************************
// this is a simple example that uses the painlessmesh library
// based on the startHere example
//
// 1. blinks led once for every node on the mesh
// 2. blink cycle repeats every BLINK_PERIOD
// 3. sends a message to every node on the mesh at a random time between 1 and 5 seconds
// 4. prints anything it receives to Serial.print
//
//
//************************************************************
#include <painlessMesh.h>

#define LED LED_BUILTIN //use the builtin LED

#define   BLINK_PERIOD    3000 // milliseconds until cycle repeat
#define   BLINK_DURATION  100  // milliseconds LED is on for

#define   MESH_SSID       "blur"
#define   MESH_PASSWORD   "parklife"
#define   MESH_PORT       5555

painlessMesh  mesh; //mesh instance
Scheduler     userScheduler; // controls tasks outside of loop()

SimpleList<uint32_t> nodes; // who is connected??

// set up a task to send a message to the network
// TASK_SECOND is a scheduler constant
void sendMessage(); 
Task taskSendMessage(TASK_SECOND * 1, TASK_FOREVER, &sendMessage); // start with a one second interval

// Task to blink the LED
Task blinkNoNodes;
bool onFlag = false; // is the LED on or off? 

void setup() {
  Serial.begin(115200);

  pinMode(LED, OUTPUT);

  mesh.setDebugMsgTypes(ERROR | DEBUG);  // set before init() so that you can see error messages

  mesh.init(MESH_SSID, MESH_PASSWORD, &userScheduler, MESH_PORT); // start mesh

  // callbacks
  mesh.onReceive(&receivedCallback); // what to do when we recv msg
  mesh.onNewConnection(&newConnectionCallback); // what to do when a new node joins
  mesh.onChangedConnections(&changedConnectionCallback); // notifies when topology changes
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback); // triggered when local time is adjusted to mesh time

  // how to send
  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();

  // use scheduler to blink LEDs nased on # of nodes
  blinkNoNodes.set(BLINK_PERIOD, (mesh.getNodeList().size() + 1) * 2, []() {
      // If on, switch off, else switch on
      if (onFlag)
        onFlag = false;
      else
        onFlag = true;
      blinkNoNodes.delay(BLINK_DURATION);

      if (blinkNoNodes.isLastIteration()) {
        // Finished blinking. Reset task for next run 
        // blink number of nodes (including this node) times
        blinkNoNodes.setIterations((mesh.getNodeList().size() + 1) * 2);
        // Calculate delay based on current mesh time and BLINK_PERIOD
        // This results in blinks between nodes being synced
        blinkNoNodes.enableDelayed(BLINK_PERIOD - 
            (mesh.getNodeTime() % (BLINK_PERIOD*1000))/1000);
      }
  });
  // add the blink task
  userScheduler.addTask(blinkNoNodes);
  // start the task
  blinkNoNodes.enable();

  randomSeed(analogRead(A0));
}

void loop() {
  mesh.update(); // required to run mesh
  digitalWrite(LED, !onFlag); // change the LED as necessary
}

void sendMessage() {
  // message to send to the whole network
  String msg = "Hello from node ";
  msg += mesh.getNodeId();
  msg += " I'm an Arduino!";
  mesh.sendBroadcast(msg); // sends broadcast message
  
  Serial.println("Sending message..");
  // randomly send a message to the network
  taskSendMessage.setInterval(random(TASK_SECOND * 1, TASK_SECOND * 5));  // between 1 and 5 seconds
}

void receivedCallback(uint32_t from, String & msg) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
  // Reset blink task
  onFlag = false;
  blinkNoNodes.setIterations((mesh.getNodeList().size() + 1) * 2);
  blinkNoNodes.enableDelayed(BLINK_PERIOD - (mesh.getNodeTime() % (BLINK_PERIOD*1000))/1000);
 
  Serial.printf("New Connection, nodeId = %u\n", nodeId);
  Serial.printf("New Connection, %s\n", mesh.subConnectionJson(true).c_str());
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
  // Reset blink task
  onFlag = false;
  blinkNoNodes.setIterations((mesh.getNodeList().size() + 1) * 2);
  blinkNoNodes.enableDelayed(BLINK_PERIOD - (mesh.getNodeTime() % (BLINK_PERIOD*1000))/1000);
 
  nodes = mesh.getNodeList();

  Serial.printf("Num nodes: %d\n", nodes.size());
  Serial.printf("Connection list:");

  SimpleList<uint32_t>::iterator node = nodes.begin();
  while (node != nodes.end()) {
    Serial.printf(" %u", *node);
    node++;
  }
  Serial.println();
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}
