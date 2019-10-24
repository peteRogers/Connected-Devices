var video;
var poseNet;
var poses = [];

// you may need to update the host
////////////////////////
var host = '192.168.0.14';
var socket;
////////////////////////

function setup() {
  // this is where the image is drawn from the camera
  createCanvas(640, 480);
  // get the camera feed
  video = createCapture(VIDEO);
  // set the size of the video
  video.size(width, height);

  // Create a new poseNet method with a single detection
  poseNet = ml5.poseNet(video, modelReady);
  // This sets up an event that fills the global variable "poses"
  // with an array every time new poses are detected
  poseNet.on('pose', function(results) {
    poses = results;
  });
  // Hide the video element, and just show the canvas
  video.hide();

  // setup the sockets
  socket = new WebSocket('ws://' + host);
  //handle an incoming message    
  socket.onmessage = readMessage;

  socket.onclose = reopen;

  // this limits too much date being sent - depends on conncection
  frameRate(10);
}

// show that the model has loaded on the screen
function modelReady() {
  select('#status').html('Model Loaded');
}

// list all the body parts you can record into the console (right click > inspect) as json
function mousePressed(){
  console.log(poses)
}

function draw() {

  // draw the image from the camera to the canvas
  image(video, 0, 0, width, height);

  // For one pose only (use a for loop for multiple poses!) so one person at a time
  if (poses.length > 0) {
    // get the first pose
    var pose = poses[0].pose;
    // get the nose from the pose    
    var nose = pose['nose'];
    // draw a circle where the nose is
    ellipse(nose.x, nose.y, 20, 20);
    // secnd the position of the nose to the server
      try {
   socket.send(parseInt(nose.x));
}
catch(error) {
  console.log("error");
}

  }
}

// this prints messages from the server to the console (right click > inspect)
function readMessage(event) {
  var msg = event.data; // read data from the onmessage event
  console.log(msg); // print it
}

function reopen(event){
    socket = new WebSocket('ws://' + host);
    socket.onmessage = readMessage;
    socket.onclose = reopen;
    
}