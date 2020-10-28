#include <Arduino.h>
#include "Music.cpp"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char *ssid = "AP-CTL";
const char *password = "";

ESP8266WebServer server(80);
IPAddress ip();
IPAddress gateway();
IPAddress subnet(255, 255, 0, 0);

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
  WiFi.hostname("musicbox");

  if (!WiFi.config(ip, gateway, subnet))
  {
    Serial.println(F("IP-Config is faulty."));
  }

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

    server.on("/play/random", []() {
      server.send(200, "text/html", F("<a href=\"/\">back</a>"));
      Music::playRandom();
    });

  for (int i = 0; i < Music::COUNT; i++)
  {

    const char *const title = Music::getTitle(i);
    char uri[30];
    strcpy(uri, playURL);
    strcat(uri, title);
    Serial.println(uri);
    server.on(uri, [i]() {
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
  Serial.println(F("HTTP server started"));
}

void loop()
{
  if (WiFi.isConnected())
  {
    server.handleClient();
  }
}
