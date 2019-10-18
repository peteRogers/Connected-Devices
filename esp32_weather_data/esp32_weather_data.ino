#include <ArduinoJson.h>

#include <WiFi.h>
#include <HTTPClient.h>
 
const char* ssid = "labLAN";
const char* password =  "password";
 
const String endpoint = "http://api.openweathermap.org/data/2.5/weather?q=Blackheath&APPID=";
const String key = "ebf1fe041a7eabcc11f8e1bc1641d10d";
 
void setup() {
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
 
}
 
void loop() {
 
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
 
    HTTPClient http;
 
   // http.begin(endpoint + key); //Specify the URL
   http.begin("http://api.erg.kcl.ac.uk/AirQuality/Daily/MonitoringIndex/Latest/SiteCode=LW1/Json");
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
        int d = doc["DailyAirQualityIndex"]["LocalAuthority"]["Site"]["Species"]["@AirQualityIndex"];
        
        //int d = object[String("@AirQualityIndex")];
        Serial.println(d);

        //////
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
    http.end(); //Free the resources
  }
 
  delay(30000);
 
}
