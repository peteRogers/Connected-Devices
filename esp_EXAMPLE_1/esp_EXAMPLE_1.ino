// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include <ESP32Servo.h>


// Network credentials
const char* ssid = "labLAN";
const char* password = "password";
//other variables
const int ledPin = 13;
String ledState = "";
String inputMessage = "";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


 
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  
  
  Serial.println("started");
  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Route to set GPIO to HIGH
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, HIGH);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to set GPIO to LOW
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, LOW);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  
  
  
  //route to get message from input
  server.on("/message", HTTP_GET, [] (AsyncWebServerRequest *request) {
      inputMessage = request->getParam("input1")->value();
      Serial.println(inputMessage);
  });
  // Start server
  server.begin();
}
 
void loop(){
  
}


//Handling Templates
String processor(const String& var){
  if(var == "STATE"){
    if(digitalRead(ledPin)){
      ledState = "ON";
    }
    else{
      ledState = "OFF";
    }
    
    return ledState;
  }
  if(var == "MESSAGE") {
    return inputMessage;

  }
  return String();
}
