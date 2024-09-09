#ifndef CLIMAT_DATA_H
#define CLIMAT_DATA_H

#define DEFAULT_LOCATION_VALUE "My Location"

#define PROP_LOCATION "location"

#define PROP_LAST_TEMPERATURE "lastTemperature"
#define PROP_MIN_TEMPERATURE "minTemperature"
#define PROP_MAX_TEMPERATURE "maxTemperature"

#define PROP_LAST_HUMIDITY "lastHumidity"
#define PROP_MIN_HUMIDITY "minHumidity"
#define PROP_MAX_HUMIDITY "maxHumidity"

#define PROP_DISPLAY_CONTENT_ARRAY "displayContent"

#define DEFAULT_LAST_TEMPERATURE_VALUE -100
#define DEFAULT_MIN_TEMPERATURE_VALUE 100
#define DEFAULT_MAX_TEMPERATURE_VALUE -100

#define DEFAULT_LAST_HUMIDITY_VALUE -100
#define DEFAULT_MIN_HUMIDITY_VALUE 100
#define DEFAULT_MAX_HUMIDITY_VALUE 0

#define TEMPERATURE_CALIBRATION_C -3

#define NO_OF_SECTIONS 2
#define NO_OF_CONTENT_TYPES 5

enum ContentPosition
{
    TOP,
    BOTTOM
};

enum DisplayContent
{
    CLIMATE_LIVE_INFO,
    CLIMATE_MIN_MAX_INFO,
    CLIMATE_WIFI_INFO,
    /// @brief Show Live Teperature and Min/Max
    CLIMATE_TEMPERATURE_INFO,
    /// @brief Show Live Humidity and Min/MAx
    CLIMATE_HUMIDITY_INFO
};


struct ClimateData
{
    // Properties
    const char *location = DEFAULT_LOCATION_VALUE;

    // Teperature Data
    float lastTemperature = DEFAULT_LAST_TEMPERATURE_VALUE;
    float minTemperature = DEFAULT_MIN_TEMPERATURE_VALUE;
    float maxTemperature = DEFAULT_MAX_TEMPERATURE_VALUE;

    // Humidity Data
    float lastHumidity = DEFAULT_LAST_HUMIDITY_VALUE;
    float minHumidity = DEFAULT_MIN_HUMIDITY_VALUE;
    float maxHumidity = DEFAULT_MAX_HUMIDITY_VALUE;

    // Display content settings
    enum DisplayContent displayContent[NO_OF_SECTIONS] = {CLIMATE_TEMPERATURE_INFO, CLIMATE_HUMIDITY_INFO};;
};
#endif // #ifndef CLIMAT_DATA_H