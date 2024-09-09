/**
 * ClimatMonitorData.h
 *
 * ClimatMonitorData Library for holding, loading and saving Climat Data.
 * Updating temperatur, humidity and location text.
 *
 * @author Creator: Jens Hjuler Lindhardt
 * @author Lindhardt Freelance
 * @version 1.0.0
 * @license MIT
 */
#ifndef CLIMAT_MONITOR_DATA_H
#define CLIMAT_MONITOR_DATA_H

#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

#ifndef CLIMAT_DATA_H
#include <ClimateData.h>
#endif

/// @brief Default config filename
#define DEFAULT_CONFIG_FILE_NAME "/climate_config_file.json"

/// @brief Default delay in milliseconds before save when shouldSaveConfig true
#define SHOULD_SAVE_DELAY_MILLISEC 60000 // 1 60 seconds

class ClimateMonitorData
{

private:
    ClimateData climateMonitorData;
    const char *configFileName;
    bool shouldSaveConfig = false; // If saved data has changed, set to true.
    unsigned long timeStampDataChnaged = 0;

    const char *setStatus(const char *status, const char *statusText);

    /// @brief Set shouldSaveConfig
    /// @return shouldSaveConfig
    bool dataChanged();
    
public:
    /// @brief Default constructor, configFileName = DEFALT_CONFIG_FILE_NAME
    ClimateMonitorData();
    ClimateMonitorData(const char *fileName);

    /// @brief setup and initialize data
    /// @param fileName config filename
    void setup(const char *fileName);

    /// @brief
    /// @return
    const char *getLocation();

    /// @brief Update location name.
    /// @param location new location name
    /// @param saveData save data after update? default = true
    /// @return Location updated
    bool updateLocation(const char *location, bool saveData = true);

    /// @brief
    /// @return
    // HumidityTemperatureData getHumidityTemperatureData();

    /// @brief Updates temperature and humidity values
    /// @param temperature Current temperature
    /// @param humidity Current humidity
    /// @return true if temperature or humidity was opdated, false if no values was updated
    bool updateTemperatureAndHumidity(float temperature, float humidity);

    /// @brief update all temperature data: last, min and max
    /// @param temperature current temeprature
    /// @return true if any temperature data was changed else false (no changes)
    bool updateTemperature(float temperature);

    /// @brief update all humidity data: last, min and max
    /// @param humidity current humidity
    /// @return true if any humidity data was changed else false (no changes)
    bool updateHumidity(float humidity);

    /// @brief
    /// @return
    float getTemperature();

    /// @brief
    /// @return
    float getMinTemperature();

    /// @brief
    /// @return
    float getMaxTemperature();

    /// @brief
    /// @return
    float getHumidity();

    /// @brief
    /// @return
    float getMinHumidity();

    /// @brief
    /// @return
    float getMaxHumidity();

    /// @brief Rest min and max values for temperature and humidity
    /// @param temperature value for min and max temperature, -999 sets values to default
    /// @param humidity value for min and max humidity, -999 sets values to default
    /// @return
    bool resetMinMaxValues(float temperature = -999, float humidity = -999);

    /// @brief Load data from storage
    /// @return true if data loaded
    bool loadData(const char *status = nullptr);

    /// @brief save data  to storage
    /// @param force force saving, even if shouldSaveConfig is false, default: false
    /// @return true if data saved, else false
    bool saveData(bool force = false);
};
#endif // ifndef CLIMAT_MONITOR_DATA_H