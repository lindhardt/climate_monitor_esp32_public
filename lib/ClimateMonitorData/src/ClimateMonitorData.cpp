/**
 * ClimateMonitorData.cpp
 *
 * ClimateMonitorData Library implementation for holding, loading and saving Climat Data.
 * Updating temperatur, humidity and location text.
 *
 * @author Creator: Jens Hjuler Lindhardt
 * @author Lindhardt Freelance
 * @version 1.0.0
 * @license MIT
 */

#include "ClimateMonitorData.h"

bool ClimateMonitorData::dataChanged()
{
    if (!timeStampDataChnaged)
        timeStampDataChnaged = millis();

    shouldSaveConfig = true;
    return shouldSaveConfig;
}

ClimateMonitorData::ClimateMonitorData()
{
    ClimateMonitorData(DEFALT_CONFIG_FILE_NAME);
}

ClimateMonitorData::ClimateMonitorData(const char *fileName)
{
    configFileName = fileName;

    if (loadData())
        return;
}

void ClimateMonitorData::setup(const char *fileName)
{
    configFileName = fileName;
}

const char *ClimateMonitorData::getLocation()
{
    return climateMonitorData.location;
}

bool ClimateMonitorData::updateLocation(const char *location, bool saveData)
{
    if (strlen(location) > 21)
    {
        // If string length > 21 then truncate string to 21 chars
        char *location_tmp;
        strlcpy(location_tmp, location, 21);
        location = location_tmp;
    }
    climateMonitorData.location = location;

    // ClimatData::saveData(true);
    return true;
}

bool ClimateMonitorData::updateTemperatureAndHumidity(float temperature, float humidity)
{
    return updateTemperature(temperature) | updateHumidity(humidity);
}

bool ClimateMonitorData::updateTemperature(float temperature)
{
    float calibTemperature = temperature + TEMPERATURE_CALIBRATION_C;

    if (climateMonitorData.lastTemperature == calibTemperature)
        return false;

    climateMonitorData.lastTemperature = calibTemperature;
    if (climateMonitorData.minTemperature > calibTemperature)
        climateMonitorData.minTemperature = calibTemperature;
    if (climateMonitorData.maxTemperature < calibTemperature)
        climateMonitorData.maxTemperature = calibTemperature;

    dataChanged();
    return true;
}

bool ClimateMonitorData::updateHumidity(float humidity)
{
    if (climateMonitorData.lastTemperature == humidity)
        return false;

    climateMonitorData.lastHumidity = humidity;
    if (climateMonitorData.minHumidity > humidity)
        climateMonitorData.minHumidity = humidity;
    if (climateMonitorData.maxHumidity < humidity)
        climateMonitorData.maxHumidity = humidity;

    dataChanged();
    return true;
}

float ClimateMonitorData::getTemperature()
{
    return climateMonitorData.lastTemperature;
}

float ClimateMonitorData::getMinTemperature()
{
    return climateMonitorData.minTemperature;
}

float ClimateMonitorData::getMaxTemperature()
{
    return climateMonitorData.maxTemperature;
}

float ClimateMonitorData::getHumidity()
{
    return climateMonitorData.lastHumidity;
}

float ClimateMonitorData::getMinHumidity()
{
    return climateMonitorData.minHumidity;
}

float ClimateMonitorData::getMaxHumidity()
{
    return climateMonitorData.maxHumidity;
}

bool ClimateMonitorData::resetMinMaxValues(float temperature, float humidity)
{
    return false;
}

bool ClimateMonitorData::loadData(const char *status)
{
    return false;
}

bool ClimateMonitorData::saveData(bool force)
{
    if (!shouldSaveConfig && !force)
        return false;

    if ((timeStampDataChnaged + SHOULD_SAVE_DELAY_MILLISEC) > millis())
        return false;

    JsonDocument jsonDoc;
    jsonDoc[PROP_LOCATION] = climateMonitorData.location;
    jsonDoc[PROP_LAST_HUMIDITY] = climateMonitorData.lastHumidity;
    jsonDoc[PROP_MIN_HUMIDITY] = climateMonitorData.minHumidity;
    jsonDoc[PROP_MAX_HUMIDITY] = climateMonitorData.maxHumidity;
    jsonDoc[PROP_LAST_TEMPERATURE] = climateMonitorData.lastTemperature;
    jsonDoc[PROP_MIN_TEMPERATURE] = climateMonitorData.minTemperature;
    jsonDoc[PROP_MAX_TEMPERATURE] = climateMonitorData.maxTemperature;

    JsonArray data = jsonDoc["data"].to<JsonArray>();
    for (int i = 0; i < NO_OF_SECTIONS; i++)
        data.add(climateMonitorData.displayContent[i]);

    // Open config file
    File configFile = SPIFFS.open(configFileName, "w");
    if (!configFile)
        return false;

    // Save to FS

    // Reset Should Save Config
    shouldSaveConfig = false;
    timeStampDataChnaged = 0;

    return true;
}
