#include <WiFi.h>
#include <WebSocketsServer.h>
 
// Constants
const char* ssid = "VM981001-2G";
const char* password = "susieAmy";
 
// Globals
WebSocketsServer webSocket = WebSocketsServer(80);

void setup() {
  // Start Serial port
  Serial.begin(115200);
  // Connect to access point
  Serial.println("Connecting");
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
  }
  // Print our IP address
  Serial.println("Connected!");
  Serial.print("My IP address: ");
  Serial.println(WiFi.localIP());
  
  // Start WebSocket server and assign callback
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
 
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
