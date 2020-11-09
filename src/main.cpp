#include "config.h"
#include <Arduino.h>
#include "Music.cpp"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


const char *ssid = CONFIG_SSID;
const char *password = CONFIG_PASSWORD;

ESP8266WebServer server(80);
IPAddress ip(CONFIG_IP);
IPAddress gateway(CONFIG_GATEWAY);
IPAddress subnet(CONFIG_SUBNET);

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
#ifdef DEBUG
  Serial.begin(9600);
  Serial.println("Serial.begin");
#endif
  randomSeed(analogRead(0));
  Music::playCoin();
  WiFi.hostname("musicbox");

#ifdef DEBUG
  if (!WiFi.config(ip, gateway, subnet))
  {
    Serial.println(F("IP-Config is faulty."));
  }
#endif
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
#ifdef DEBUG
    Serial.print(".");
#endif
  }
#ifdef DEBUG
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
#endif
  Music::playCoin();

  const String playURL = "/play/";

    server.on(playURL + "random", []() {
      server.send(200, "text/html", F("<a href=\"/\">back</a>"));
      Music::playRandom();
    });

  for (int i = 0; i < Music::COUNT; i++)
  {
    server.on(playURL + Music::getTitle(i), [i]() {
      server.send(200, "text/html", F("<a href=\"/\">back</a>"));
      Music::play(i);
    });
  }

  server.on("/", [playURL]() {
    String message = "<html><body>";

    for (int i = 0; i < Music::COUNT; i++)
    {
      message += "<a href=\"";
      message += playURL;
      message += Music::getTitle(i);
      message += "\">";
      message += Music::getTitle(i);
      message += "</a><br>";
    }
    message += "</body></html>";
    server.send(200, "text/html", message);
  });

  server.onNotFound(handleNotFound);

  server.begin();
#ifdef DEBUG
  Serial.println(F("HTTP server started"));
#endif
}

void loop()
{
  if (WiFi.isConnected())
  {
    server.handleClient();
  }
}
