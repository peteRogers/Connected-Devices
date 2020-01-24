#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_GFX.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>

#define PIN 21
#define mw 8 //amount of pixels width
#define mh 8 //amount of pixels height
#define NUMMATRIX (mw*mh)
int x    = mw;
int pass = 0;
CRGB matrixleds[NUMMATRIX];
FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(matrixleds, mw, mh, mw/8, 1, NEO_MATRIX_TOP + NEO_MATRIX_RIGHT + NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE );
const char* ssid = "212B";
const char* password =  "password";
String cString = "";


long del = -60000;


void setup() {
 
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  FastLED.addLeds<NEOPIXEL,PIN>(matrixleds, NUMMATRIX); 
  matrix->begin();
  matrix->setTextWrap(false);
  matrix->setBrightness(255);
}
 
void loop() {
  if(millis() > del + 60000){
    del = millis();
    if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
       HTTPClient http;
       //http.begin(endpoint+key); //Specify the URL
       http.begin("http://data.police.uk/api/stops-street?lat=51.475620&lng=-0.036660&date=2019-08");
       int httpCode = http.GET();  //Make the request
     
        if (httpCode > 0) { //Check for the returning code
            String payload = http.getString();
            Serial.println(payload);
             
            DynamicJsonDocument doc(61440);
            deserializeJson(doc, payload);
            
            Serial.println(doc.size());
            
          JsonArray array = doc.as<JsonArray>();
          for(JsonVariant v : array) {
            String e = v["self_defined_ethnicity"];
            String s = v["legislation"];
            Serial.println(e);
            Serial.println(s);
            cString = e;
            cString +="...";
            cString +=s;
              }
          }
     
        else {
         // Serial.println("Error on HTTP request");
        }
     
        http.end(); //Free the resources
  }
 
  }
  matrix->fillScreen(0);
  matrix->setCursor(x, 0);
  matrix->print(cString);
   int len = (cString.length()*7)*-1;
  if(--x < len) {
    x = matrix->width();
    
  }
  matrix->show();
  delay(50);
 
}
