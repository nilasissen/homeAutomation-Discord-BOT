#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WebSocketsServer.h>
#ifndef STASSID
#define STASSID "abcd"
#define STAPSK  "abcd"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);
//WebSocketsServer webSocket(81); 
WebSocketsServer webSocket = WebSocketsServer(81);
//const int led = 13;
const int relay = 16;
const int relay2 = 5;
void handleRoot() {
//  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp8266!");
//  digitalWrite(led, 0);
}

void startWebSocket() { // Start a WebSocket server
  webSocket.begin();                          // start the websocket server
  webSocket.onEvent(webSocketEvent);          // if there's an incomming websocket message, go to function 'webSocketEvent'
  Serial.println("WebSocket server started.");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  if (type == WStype_TEXT){
      delay(500);
    if(payload[0]== 'F'){
      digitalWrite(relay2, LOW);
      webSocket.sendTXT(num, "turn-on");
    }
    else if(payload[0]=='L'){
      digitalWrite(relay, LOW);
      webSocket.sendTXT(num, "turn-on");
    }
     else if(payload[0]=='1'){
      digitalWrite(relay, HIGH);
      webSocket.sendTXT(num, "turn-off");
    }
     else if(payload[0]=='2'){
      digitalWrite(relay2, HIGH);
      webSocket.sendTXT(num, "turn-off");
    }
    else if(payload[0]=='B'){
      digitalWrite(relay, LOW);
      digitalWrite(relay2, LOW);
      webSocket.sendTXT(num, "turn-on");
    }
    else if(payload[0]=='3'){
      digitalWrite(relay, HIGH);
      digitalWrite(relay2, HIGH);
      webSocket.sendTXT(num, "turn-off");
    }
   for(int i = 0; i < length; i++) Serial.print((char) payload[i]);
   Serial.println();

  }
  if(type == WStype_CONNECTED){
              IPAddress ip = webSocket.remoteIP(num);
              Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\r\n", num,     ip[0], ip[1], ip[2], ip[3], payload);  
              webSocket.sendTXT(num, "connected");  
            }
}

void handleNotFound() {
//  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
//  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(LED_BUILTIN, OUTPUT);
//  pinMode(led, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay2, LOW);
//  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.on("/gif", []() {
    static const uint8_t gif[] PROGMEM = {
      0x47, 0x49, 0x46, 0x38, 0x37, 0x61, 0x10, 0x00, 0x10, 0x00, 0x80, 0x01,
      0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x2c, 0x00, 0x00, 0x00, 0x00,
      0x10, 0x00, 0x10, 0x00, 0x00, 0x02, 0x19, 0x8c, 0x8f, 0xa9, 0xcb, 0x9d,
      0x00, 0x5f, 0x74, 0xb4, 0x56, 0xb0, 0xb0, 0xd2, 0xf2, 0x35, 0x1e, 0x4c,
      0x0c, 0x24, 0x5a, 0xe6, 0x89, 0xa6, 0x4d, 0x01, 0x00, 0x3b
    };
    char gif_colored[sizeof(gif)];
    memcpy_P(gif_colored, gif, sizeof(gif));
    // Set the background to a random set of colors
    gif_colored[16] = millis() % 256;
    gif_colored[17] = millis() % 256;
    gif_colored[18] = millis() % 256;
    server.send(200, "image/gif", gif_colored, sizeof(gif_colored));
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  startWebSocket();
}

void loop(void) {
//  digitalWrite(relay, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  //digitalWrite(relay, HIGH);
//  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000); 
  if(WiFi.status() == WL_CONNECTED) {
    webSocket.loop();
  } else {
    webSocket.disconnect();
  }
  server.handleClient();
  MDNS.update();
}
