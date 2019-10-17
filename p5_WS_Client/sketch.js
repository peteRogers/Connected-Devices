// where the serial server is (your local machine):
var host = '10.100.13.154';
var socket; // the websocket
var sensorValue = 0; // the sensor value
var button;

function setup() {
    createCanvas(400, 400);
    // connect to server:
    socket = new WebSocket('ws://' + host);
    // socket connection listener:

    // socket message listener:
    socket.onmessage = readMessage;
    button = new Clickable();
    button.locate(20, 20);
    button.color = "#53b54a"
    button.onPress = function(){
        socket.send("on");
        this.color = "#b7f0b1"
  }
     button.onRelease = function(){
        socket.send("off");
         this.color = "#53b54a"
  }

}

function draw() {
 button.draw();
}



function readMessage(event) {
  var msg = event.data; // read data from the onmessage event
  sensorValue = Number(msg) / 4;
  println(sensorValue); // print it
}


