#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <CircularBuffer.hpp>
#include <WiFi.h>

#include <SPI.h>
#include <U8g2lib.h>

// --------------------------------------------------------------
// |     Settings                                               |
// --------------------------------------------------------------

const char *WIFI_SSID = "SSID";
const char *WIFI_PASSWORD = "PASSWORD";

const char *NTP_SERVER = "pool.ntp.org";
const char *NTP_TIMEZONE = "CET-1CEST,M3.5.0,M10.5.0/3"; // Europe/Berlin

const char *ROOT_CA =
    "-----BEGIN CERTIFICATE-----\n"
    "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\n"
    "-----END CERTIFICATE-----\n";

#define MS_PER_TICK 500               // how often is the display redrawn
#define MS_PER_UPDATE 1000 * 60 * 3   // how often is rest api called
#define TRAIN_BUFFER_SIZE 12          // how many trains get stored internally

const char *VBB_REST_ENDPOINT = 
    "https://v6.vbb.transport.rest/stops/"
    "900056102"                       // station Nollendorfplatz
    "/departures"
    "?results=12"
    "&suburban=false"
    "&subway=true"
    "&tram=false"
    "&bus=false"
    "&ferry=false"
    "&express=false"
    "&regional=false";

// --------------------------------------------------------------
// |     Helper                                                 |
// --------------------------------------------------------------


tm parseDateTime(String dateTimeString)
{
  tm datetime;

  datetime.tm_year = dateTimeString.substring(0, 4).toInt() - 1900; // number of years since 1900
  datetime.tm_mon = dateTimeString.substring(5, 7).toInt() - 1;     // number of months since january
  datetime.tm_mday = dateTimeString.substring(8, 10).toInt();
  datetime.tm_hour = dateTimeString.substring(11, 13).toInt();
  datetime.tm_min = dateTimeString.substring(14, 16).toInt();
  datetime.tm_sec = dateTimeString.substring(17, 19).toInt();
  datetime.tm_isdst = -1; // daylight savings, -1 uses timezone

  return datetime;
}

struct TrainInfo
{
  String line;
  String destination;
  time_t when;
};

// --------------------------------------------------------------
// |     Setup                                                  |
// --------------------------------------------------------------

HTTPClient client;
CircularBuffer<TrainInfo, TRAIN_BUFFER_SIZE> trains;
U8G2_SSD1309_128X64_NONAME0_F_4W_SW_SPI OLED(U8G2_R0, /* clock=*/18, /* data=*/23, /* cs=*/5, /* dc=*/17, /* reset=*/16);

bool updateData()
{
  client.begin(VBB_REST_ENDPOINT, ROOT_CA);

  int httpCode = client.GET();

  JsonDocument filter;

  filter["departures"][0]["when"] = true;
  filter["departures"][0]["plannedWhen"] = true;
  filter["departures"][0]["delay"] = true;
  filter["departures"][0]["direction"] = true;
  filter["departures"][0]["line"]["name"] = true;

  trains.clear();

  if (httpCode != HTTP_CODE_OK)
  {
    client.end();
    return false;
  }

  JsonDocument doc;
  deserializeJson(doc, client.getStream(), DeserializationOption::Filter(filter));

  for (JsonObject trainJson : doc["departures"].as<JsonArray>())
  {
    String line = trainJson["line"]["name"];
    String destination = trainJson["direction"];
    struct tm datetime = parseDateTime(trainJson["when"]);
    time_t when = mktime(&datetime);

    trains.push({line, destination, when});
  }

  client.end();
  return true;
}

void setupWiFi()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to: ");
  Serial.print(WIFI_SSID);
  Serial.print(" ");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println("Connected!");

  configTime(0, 0, NTP_SERVER);
  setenv("TZ", NTP_TIMEZONE, 1);
  tzset();
}

void setup()
{
  Serial.begin(9600);
  setupWiFi();
  OLED.begin();

  updateData();
}

// --------------------------------------------------------------
// |     Loop                                                   |
// --------------------------------------------------------------

bool blink;

void updateDisplay()
{
  struct tm timeInfoNow;
  getLocalTime(&timeInfoNow);
  time_t timeNow = mktime(&timeInfoNow);

  OLED.clearBuffer();

  // remove trains, which have already passed
  while (trains.size() != 0 && trains[0].when < timeNow)
    trains.shift();

  
  for (int i = 0; i < min(4, (int)trains.size()); i++)
  {
    TrainInfo train = trains[i];

    // minutes till train arrives
    time_t min = train.when < timeNow ? 0 : (train.when - timeNow) / 60;

    if (min == 0 && blink)
      continue;

    OLED.setFont(u8g2_font_12x6LED_tf);
    OLED.drawUTF8(2, (i + 1) * 16 - 2, train.line.c_str());

    OLED.setFont(u8g2_font_boutique_bitmap_7x7_tf);
    OLED.drawUTF8(20, (i + 1) * 16 - 8 - 1, train.destination.c_str());

    if (min != 0)
      OLED.drawUTF8(128 - 24, (i + 1) * 16 - 1, (String(min) + " min").c_str());
  }

  OLED.sendBuffer();
  blink = !blink;
}

unsigned long lastDataTime = millis();

void loop()
{
  if (millis() - lastDataTime > MS_PER_UPDATE)
  {
    lastDataTime = millis();

    updateData();
    Serial.println("Data Updated");
  }

  unsigned long before = millis();

  updateDisplay();

  unsigned long elapsed = millis() - before;

  if (elapsed < MS_PER_TICK)
    delay(MS_PER_TICK);
}