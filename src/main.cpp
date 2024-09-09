#include <Arduino.h>
// OLed
#pragma region Library Includes
// Include Wire Library for I2C
#include <Wire.h>

// Define Display attach to ESP32
#define DISPLAY_OLED
// #define DISPLAY_LCD
// #define DISPLAY_NONE

// // Include SPIFFS and Json Libraries
// #include <FS.h>
// #include <SPIFFS.h>
// #include <ArduinoJson.h>

#include <ClimateMonitorData.h>

#define JSON_CONFIG_FILE "/climate_config.json"

// Include OLED libraries
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>
#ifdef DISPLAY_OLED
#include <ClimateMonitorOLED.h>
#endif // #ifdef DISPLAY_OLED

// Include Temp and Humid Libraries
#include <DHT.h>

// Include WiFiManager Libraries, https://github.com/tzapu/WiFiManager
#include <WiFiManager.h>
#include <DNSServer.h>
#include <WebServer.h>
#pragma endregion

#define SERIAL_DEBUG_OUTPUT true

// select which pin will trigger the configuration portal when set to LOW
#define WFM_CONFIG_TRIGGER_PIN 15

// select which pins will trigger display mode
#define DISPLAY_MODE_SECTION_0_TRIGGER_PIN 8
#define DISPLAY_MODE_SECTION_1_TRIGGER_PIN 9

// select which pin will trigger display mode
#define MIN_MAX_RESET_TRIGGER_PIN 10

// WMF setup default values
// #define WFM_SET_DEBUG_OUTPUT
#define WFM_TIMEOUT 120 // seconds to run for
#define WFM_AP_NAME "HT_SETUP_AP"
#define WFM_AP_USE_PASSPHRASE false
#define WFM_AP_PASSPHRASE "Pa$$w0rd"

// Include display test mode
#ifdef INCLUDE_DISPLAY_TEST
#define DISPLAY_TEST_MODE_TRIGGER 7
#endif

// Reset pin for OLED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the setPins function of the Wire library.
// #define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
// #define I2C_SDA 12
// #define I2C_SCL 13

#ifdef DISPLAY_OLED
ClimateMonitorOLED climateDisplay(I2C_SDA, I2C_SCL);
#endif // #ifdef DISPLAY_OLED

bool forceDisplayUpdate = true;

// Define Temp & Humid object
#define DHT_DATA_PIN 14
DHT dht;

bool shouldSaveConfig = false;

ClimateMonitorData climatMonitorData(JSON_CONFIG_FILE);

WiFiManager wmf;

// WFM Parameters, setup
WiFiManagerParameter locationTextBox("id_location", "Location", "Location", 20);

// Run variables
uint no_of_updates = 0;

void displayTempHumidity();
void checkWFMTrigger();
void checkDisplayTrigger();
void saveConfigFile();

#ifdef INCLUDE_DISPLAY_TEST
void checkDisplayTestTrigger();
#endif // #ifdef INCLUDE_DISPLAY_TEST

void setup()
{
  // Start Wire linrary for I2C
  Wire.begin();

  // Initialize OLED with I3´2C addr 0x3c
  climateDisplay.begin(0x3C);

  // Initialize Temp & Humid Sensor
  dht.setup(DHT_DATA_PIN);

  // WIFI Setup
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP

  // Setup trigger pins
  pinMode(WFM_CONFIG_TRIGGER_PIN, INPUT_PULLUP);             // WFM Trigger
  pinMode(DISPLAY_MODE_SECTION_0_TRIGGER_PIN, INPUT_PULLUP); // Display mode trigger
  pinMode(MIN_MAX_RESET_TRIGGER_PIN, INPUT_PULLUP);          // Reset min/max values trigger

#ifdef INCLUDE_DISPLAY_TEST
  pinMode(DISPLAY_TEST_MODE_TRIGGER, INPUT_PULLUP); // Show test display trigger
#endif                                              // #ifdef INCLUDE_DISPLAY_TEST

#ifndef WFM_SET_DEBUG_OUTPUT
  wmf.setDebugOutput(false);
#endif

#if WFM_AP_USE_PASSPHRASE
  bool res = wmf.autoConnect(WFM_AP_NAME, WFM_AP_PASSPHRASE);
#else
  bool res = wmf.autoConnect(WFM_AP_NAME);
#endif

#if SERIAL_DEBUG_OUTPUT
  if (!res)
  {
    Serial.println("Failed to connect");
    // ESP.restart();
  }
  else
  {
    // if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
  }
#endif
}

void loop()
{
  checkWFMTrigger();

#ifdef INCLUDE_DISPLAY_TEST
  checkDisplayTestTrigger();
#endif // #ifdef INCLUDE_DISPLAY_TEST

  displayTempHumidity();
}

void displayTempHumidity()
{
  // Delay to allow sensor to stabalize
  delay(dht.getMinimumSamplingPeriod());

  // Read Humidity and Temperature
  float currentHumidity = dht.getHumidity();
  float currentTemperature = dht.getTemperature();

  if (isnan(currentHumidity) || isnan(currentTemperature))
    return;

  bool update = climatMonitorData.updateTemperatureAndHumidity(currentTemperature, currentHumidity);

  if (!forceDisplayUpdate && !update)
    return;

  forceDisplayUpdate = false;
#ifndef DISPLAY_NONE // Don't call climatDisplay.update, if there is no display
  climateDisplay.update(WiFi.SSID(), WiFi.localIP().toString(), &climatMonitorData);
#endif // #ifndef DISPLAY_NONE
}

// Check if WFM trigger is pressed
void checkWFMTrigger()
{
  if (digitalRead(WFM_CONFIG_TRIGGER_PIN) == LOW)
  {
    // reset settings - for testing
    // wm.resetSettings();
    climateDisplay.invertDisplay(true);

    wmf.addParameter(&locationTextBox);

    // set configportal timeout
    wmf.setConfigPortalTimeout(WFM_TIMEOUT);

#if WFM_AP_USE_PASSPHRASE
    bool res = wmf.startConfigPortal(WFM_AP_NAME, WFM_AP_PASSPHRASE);
#else
    bool res = wmf.startConfigPortal(WFM_AP_NAME);
#endif

    if (!res)
    {
      delay(3000);
      // reset and try again, or maybe put it to deep sleep
      ESP.restart();
      delay(5000);
    }
    forceDisplayUpdate = climatMonitorData.updateLocation(locationTextBox.getValue());
    climateDisplay.invertDisplay(false);

    // if you get here you have connected to the WiFi
  }
}

void checkDisplayTrigger()
{
  if (digitalRead(DISPLAY_MODE_SECTION_0_TRIGGER_PIN) == LOW)
  {
    climateDisplay.shiftContent(BOTTOM);

    forceDisplayUpdate = true;
  }
}

#ifdef INCLUDE_DISPLAY_TEST
void checkDisplayTestTrigger()
{
  if (digitalRead(DISPLAY_TEST_MODE_TRIGGER) == LOW)
  {
    climateDisplay.testDisplay();
    delay(10000);
  }
  forceDisplayUpdate = true;
}
#endif // #ifdef INCLUDE_DISPLAY_TEST

// void saveConfigFile()
// {
//   JsonDocument jsonDoc;
//   jsonDoc["location"] = htData.location;
//   jsonDoc["humidity"] = htData.lastHumidity;
//   jsonDoc["min_humidity"] = htData.minHumidity;
//   jsonDoc["max_humidity"] = htData.maxHumidity;
//   jsonDoc["temperature"] = htData.lastTemperature;
//   jsonDoc["min_temperature"] = htData.minTemperature;
//   jsonDoc["max_temperature"] = htData.maxTemperature;
//
//   // Open config file
//   File configFile = SPIFFS.open(JSON_CONFIG_FILE, "w");
//   if (!configFile)
//   {
// #if SERIAL_DEBUG_OUTPUT
//     Serial.println("Failed to op config file for writing");
// #endif
//     return;
//   }
// }
