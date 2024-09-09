/**
 * ClimateMonitorOLED.h
 *
 * ClimateMonitorOLED Library header for controlling and displaying Climat Monitor data on OLED display.
 *
 * @author Creator: Jens Hjuler Lindhardt
 * @author Lindhardt Freelance
 * @version 1.0.0
 * @license MIT
 */
#ifndef CLIMATE_MONITOR_OLED_H
#define CLIMATE_MONITOR_OLED_H

// Include Wire Library for I2C
#include <Wire.h>

// Include OLED libraries
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Climat Monitor Bitmaps
#include <ClimateMonitorBitmaps.h>

// Climat data library
#include <ClimateMonitorData.h>

// Reset pin for OLED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the setPins function of the Wire library.
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define I2C_SDA 12
#define I2C_SCL 13

// bool i2c_ok = Wire.setPins(I2C_SDA, I2C_SCL);
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define TOP_START 0
#define BOTTOM_START 30

#define FONT_SIZE_1_LINE_INCREMENT 10
#define FONT_SIZE_2_LINE_INCREMENT 20
#define FONT_SIZE_3_LINE_INCREMENT 30
#define FONT_SIZE_4_LINE_INCREMENT 40

#define INCLUDE_DISPLAY_TEST

#define DIALOG_BUTTON_0_TRIGGER_PIN 1
#define DIALOG_BUTTON_1_TRIGGER_PIN 2
#define DIALOG_BUTTON_2_TRIGGER_PIN 3
#define DIALOG_BUTTON_3_TRIGGER_PIN 4
#define DIALOG_BUTTON_4_TRIGGER_PIN 5

enum DialogButton
{
    BUTTON_0,
    BUTTON_1,
    BUTTON_2,
    BUTTON_3,
    BUTTON_4
};

class ClimateMonitorOLED
{

private:
    Adafruit_SSD1306 display;

    enum DisplayContent content[NO_OF_SECTIONS] = {CLIMATE_TEMPERATURE_INFO, CLIMATE_HUMIDITY_INFO};
    int no_of_updates = 0;

    const uint8_t dialogButtonTriggerPins[5] =
        {
            DIALOG_BUTTON_0_TRIGGER_PIN,
            DIALOG_BUTTON_1_TRIGGER_PIN,
            DIALOG_BUTTON_2_TRIGGER_PIN,
            DIALOG_BUTTON_3_TRIGGER_PIN,
            DIALOG_BUTTON_4_TRIGGER_PIN};

    /// @brief
    /// @param x
    /// @param y
    /// @param width
    /// @param height
    /// @param bmp
    void drawBitmap(u_int16_t x, u_int16_t y, u_int16_t width, u_int16_t height, const u_int8_t bitmap[]);

public:
    /// @brief
    ClimateMonitorOLED();

    /// @brief
    /// @param sdaPin
    /// @param sclPin
    ClimateMonitorOLED(int sdaPin, int sclPin);

    /// @brief
    void begin();

    /// @brief
    /// @param screenAdress
    void begin(uint8_t screenAdress);

    /// @brief
    /// @param dialogButton0
    /// @param dialogButton1
    /// @param dialogButton2
    /// @param dialogButton3
    /// @param dialogButton4
    void setup(uint8_t dialogButton0 = DIALOG_BUTTON_0_TRIGGER_PIN, uint8_t dialogButton1 = DIALOG_BUTTON_1_TRIGGER_PIN,
               uint8_t dialogButton2 = DIALOG_BUTTON_2_TRIGGER_PIN, uint8_t dialogButton3 = DIALOG_BUTTON_3_TRIGGER_PIN,
               uint8_t dialogButton4 = DIALOG_BUTTON_4_TRIGGER_PIN);

    /// @brief
    /// @param SSID
    /// @param localIP
    /// @param climatData
    void update(String SSID, String localIP, ClimateMonitorData *climatData);

    /// @brief Show message in display, messageBox( "Heanding", {"Line 1","Line 2","Line 3","Line 4","Line 5"})
    /// @param heading Heading max 21 chars.
    /// @param message Message max 5 lines, 21 chars.
    void dialogBox(const char *heading, const char *message[],
                   const char *btnText[5]);

    /// @brief
    /// @param invert
    void invertDisplay(bool invert);

    /// @brief
    /// @param
    void shiftContent(enum ContentPosition position);

#ifdef INCLUDE_DISPLAY_TEST
    /// @brief Show Text size 1-3
    void testDisplay();
#endif // #ifdef INCLUDE_DISPLAY_TEST
};
#endif // #ifndef CLIMATE_MONITOR_OLED_H