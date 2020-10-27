#include <Arduino.h>
#include "Music.cpp"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char *ssid = "AP-CTL";
const char *password = "";

ESP8266WebServer server(80);

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Serial.begin");
  randomSeed(analogRead(0));
  Music::playCoin();
  WiFi.hostname("Name");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  Music::playCoin();

  
  const char playURL[] = "/play/";
  for (int i = 0; i < Music::COUNT; i++)
  {
    
    const char *const title = Music::getTitle(i);
    char uri[30];
    strcpy(uri, playURL);
    strcat(uri, title);
    Serial.println(uri);
    server.on(uri, [i]() {
      server.send(200, "text/plain", "this works as well");
      Music::play(i);
    });
  }

  server.on("/", []() {
    server.send(200, "text/plain", "this works as well");
    Music::playRandom();
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();

  if (WiFi.isConnected())
  {
    server.handleClient();
    //Music::play("1-UP");
  }
  /*
  delay(10000);*/
}
