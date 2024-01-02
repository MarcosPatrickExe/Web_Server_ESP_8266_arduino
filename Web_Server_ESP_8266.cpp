// importando bibliotecas da library
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "Aqui Telecom Saraiva";
const char* password = "91230777";

String waitingEllipsis = "...";

ESP8266WebServer server(80); // creating a 'server' object


const int led = BUILTIN_LED;


void handleRoot() {
  digitalWrite(led, 1);

  String textoHTML;

  textoHTML = "Ola!! Aqui &eacute; o <b>ESP8266</b> falando! ";
  textoHTML += "Porta A0: ";
  textoHTML += analogRead(A0);
   
  textoHTML += "<Br />";
  
  if(server.args() > 0){
    textoHTML += "Arguments received: <Br />";
    
    for (uint8_t i=0; i<server.args(); i++){
       textoHTML += server.argName(i) +": ";
       textoHTML += server.arg(i) +" <Br />";
    }
  }else{
      textoHTML += "empty arguments...";    
  }
  
  server.send(200, "text/html", textoHTML);
  
  delay(1000); // inserido
  digitalWrite(led, 0);
}


void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  
  server.send(404, "text/plain", message);

  delay(1000); // inserido
  digitalWrite(led, 0);
}

void setup(void){
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
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

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started \n\nWaiting for requests");
}



void loop(){
  server.handleClient(); // listenning the client requests

  waitingEllipsis += ".";

  if(waitingEllipsis.length() > 10 ){
      waitingEllipsis = ".";
      Serial.println("");
  }
  
  Serial.print(waitingEllipsis);
  delay(1000);
}
