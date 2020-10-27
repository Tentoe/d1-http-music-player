#include <Arduino.h>
#include "ArduinoJson.h"
#include <string>
#include "Music.cpp"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
// Network SSID
const char *ssid = "AP-CTL-GAST";
const char *password = "";
const String user = "kkurz";
const String token = "";

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
}

int lastBuild = 0;

int jenkinsAPI(const String URL)
{
  std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);

  client->setInsecure();

  HTTPClient https;

  if (https.begin(*client, URL))
  {
    int httpCode = https.GET();

    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
    {
      const size_t capacity = JSON_OBJECT_SIZE(2) + 70;
      DynamicJsonDocument doc(capacity);

      DeserializationError error = deserializeJson(doc, https.getString());
      https.end();
      if (error)
      {
        Serial.println("deserializeJson() failed: ");
        Serial.println(error.c_str());
      }
      //https://arduinojson.org/v6/assistant/

      return (int)doc["number"];
    }
  }
  else
  {
    Serial.printf("[HTTPS] Unable to connect\n");
  }
  return -1;
}

void loop()
{
  if (WiFi.isConnected())
  {

    const int completed = jenkinsAPI("https://" + user + ":" + token + "@jenkins.ctl.de/job/LV/lastCompletedBuild/api/json?tree=number");
    const int stable = jenkinsAPI("https://" + user + ":" + token + "@jenkins.ctl.de/job/LV/lastStableBuild/api/json?tree=number");
    Serial.print(completed);
    Serial.print(", ");
    Serial.println(stable);
    if ((completed != stable || completed < 0)) // lastBuild != completed && 
      Music::playRandom();

    lastBuild = completed;
  }

  delay(60000);
}
