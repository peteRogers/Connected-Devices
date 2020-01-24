

// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include <Adafruit_GFX.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>


#define PIN 21
#define mw 8 //amount of pixels width
#define mh 8 //amount of pixels height
#define NUMMATRIX (mw*mh)
CRGB matrixleds[NUMMATRIX];
int cx = 0;
int cy = 0;
FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(matrixleds, mw, mh, mw/8, 1, NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT + NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE );

// Replace with your network credentials
const char* ssid = "212B";
const char* password = "password";


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);



 
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  FastLED.addLeds<NEOPIXEL,PIN>(matrixleds, NUMMATRIX); 
  matrix->begin();
  matrix->setBrightness(255);
  
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
    request->send(SPIFFS, "/index.html", String(), false);
  });

  
 
  
  
 

  server.on("/", HTTP_POST, [](AsyncWebServerRequest *request){
    int paramsNr = request->params();
    Serial.println(paramsNr);
 
    for(int i=0;i<paramsNr;i++){
 
        AsyncWebParameter* p = request->getParam(i);
        Serial.print("Param name: ");
        Serial.println(p->name());
        if(p->name() == "y"){
          Serial.println("y");
          cy = p->value().toInt();
        }
        if(p->name() == "x"){
          Serial.println("x");
          cx = p->value().toInt();
        }
           if(p->name() == "color"){
            if(p->value()=="red"){
               matrix->drawPixel(cx, cy, matrix->Color(255,0,0));
               matrix->show();//this sends data to the board
            }
            if(p->value()=="green"){
               matrix->drawPixel(cx, cy, matrix->Color(0,255,0));
               matrix->show();//this sends data to the board
            }
            if(p->value()=="blue"){
               matrix->drawPixel(cx, cy, matrix->Color(0,0,255));
               matrix->show();//this sends data to the board
            }
            if(p->value()=="black"){
               matrix->drawPixel(cx, cy, matrix->Color(0,0,0));
               matrix->show();//this sends data to the board
            }
        }
        
        Serial.print("Param value: ");
        Serial.println(p->value());
        Serial.println("------");
    }
 
    request->send(200);
  });


  // Start server
  server.begin();
}
 
void loop(){
 
  //delay(100);
}
