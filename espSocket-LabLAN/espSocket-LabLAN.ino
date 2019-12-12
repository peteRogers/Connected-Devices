#include <WiFi.h>
#include <WebSocketsServer.h>

/*
// Constants
const char* ssid = "largePrinter";
const char* password = "password";
 
// Globals
WebSocketsServer webSocket = WebSocketsServer(80);

*/

const char* ssid     = "ESP32-Access-Point";
const char* password = "password";
WebSocketsServer webSocket = WebSocketsServer(80);

void setup() {
  // Start Serial port
  Serial.begin(115200);
  Serial1.begin(115200);
  // Connect to access point
  Serial.println("Connecting");
  /**
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
  }

  **/
    WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Print our IP address
  Serial.println("Connected!");
  Serial.print("My IP address: ");
  Serial.println(WiFi.localIP());
  
  // Start WebSocket server and assign callback
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
  //Serial1.println("#0AA100");
  //Serial1.println("#0AD15");
  Serial1.println("#0SD5400");
  Serial1.println("#0EM0");
}
 
// Called when receiving any WebSocket message
void onWebSocketEvent(uint8_t num,
                      WStype_t type,
                      uint8_t * payload,
                      size_t length) {
 
  // Figure out the type of WebSocket event
  switch(type) {
 
    // Client has disconnected
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
 
    // New client has connected
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connection from ", num);
        Serial.println(ip.toString());
        webSocket.sendTXT(num, ".");
      }
      break;
 
    // Where we receive data from the socket
    case WStype_TEXT:
      //webSocket.sendTXT(num, ".");
      Serial.printf("[%u] Text: %s\n", num, payload);
      Serial1.printf("#0D%s", payload);
      Serial1.println();
      
      
      break;
 
    // For everything else: do nothing - JUST IGNORE
    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    default:
      break;
  }
}
 

 
void loop() {
 
  // Look for and handle WebSocket data
  webSocket.loop();
 
  
}
