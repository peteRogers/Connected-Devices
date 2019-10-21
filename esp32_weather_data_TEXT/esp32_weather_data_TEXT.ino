#include <ArduinoJson.h>

#include <WiFi.h>
#include <HTTPClient.h>
 #include <Adafruit_GFX.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>

#define PIN 21

long del = 0;

#define mw 8 //amount of pixels width
#define mh 8 //amount of pixels height
#define NUMMATRIX (mw*mh)

int x    = mw;
int pass = 0;


CRGB matrixleds[NUMMATRIX];

FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(matrixleds, mw, mh, mw/8, 1, 
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
    NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE );

const char* ssid = "VM981001-2G";
const char* password =  "susieAmy";
 String cString = "";
const String endpoint = "http://api.openweathermap.org/data/2.5/weather?id=6947041&APPID=";
const String key = "ebf1fe041a7eabcc11f8e1bc1641d10d";
 
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
  //matrix->setTextColor(colors[0]);
}
 
void loop() {
 

  if( millis() > del + 30000){
    Serial.println("foof");
    del = millis();
      if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
 
    HTTPClient http;
 
    http.begin(endpoint + key); //Specify the URL
  // http.begin("http://api.erg.kcl.ac.uk/AirQuality/Daily/MonitoringIndex/Latest/SiteCode=LW1/Json");
    int httpCode = http.GET();  //Make the request
 
    if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
       // Serial.println(httpCode);
        Serial.println(payload);
       

        DynamicJsonDocument doc(1024);
        
        deserializeJson(doc, payload);
//        
//        JsonObject obj = doc.as<JsonObject>();
//        JsonVariant object = obj.getMember("DailyAirQualityIndex");
//        JsonVariant la = object.getMember("LocalAuthority");
//        JsonVariant site = la.getMember("Site");
//        JsonVariant spec = site.getMember("Species");
       // int d = doc["DailyAirQualityIndex"]["LocalAuthority"]["Site"]["Species"]["@AirQualityIndex"];
       String d = doc["rain"]["1h"];
  
    //delay(5);
        
        //int d = object[String("@AirQualityIndex")];
       // Serial.println(d);
        cString = d;

        //////
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
  if(--x < -36) {
    x = matrix->width();
    
  }
  matrix->show();
  delay(50);
 
}
