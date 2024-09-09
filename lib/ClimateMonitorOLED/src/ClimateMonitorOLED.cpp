#include "ClimateMonitorOLED.h"
/**
 * ClimatMonitorOLED.h
 *
 * ClimatMonitorOLED Library implementation for controlling and displaying Climat Monitor data on OLED display.
 *
 * @author Creator: Jens Hjuler Lindhardt
 * @author Lindhardt Freelance
 * @version 1.0.0
 * @license MIT
 */

void ClimateMonitorOLED::drawBitmap(u_int16_t x, u_int16_t y, u_int16_t width, u_int16_t height, const u_int8_t bitmap[])
{
    display.drawBitmap(x, y, bitmap, width, height, 1);
}

ClimateMonitorOLED::ClimateMonitorOLED()
{
    ClimateMonitorOLED(I2C_SDA, I2C_SCL);
}

ClimateMonitorOLED::ClimateMonitorOLED(int sdaPin, int sclPin)
{
    Wire.setPins(sdaPin, sclPin);
    display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
}

void ClimateMonitorOLED::begin()
{
    begin(SCREEN_ADDRESS);
}

void ClimateMonitorOLED::begin(uint8_t screenAdress)
{
    display.begin(SSD1306_SWITCHCAPVCC, screenAdress);
}

void ClimateMonitorOLED::setup(uint8_t dialogButton0, uint8_t dialogButton1, uint8_t dialogButton2, uint8_t dialogButton3, uint8_t dialogButton4)
{
}

void ClimateMonitorOLED::update(String SSID, String localIP, ClimateMonitorData *climatMonitorData)
{
    no_of_updates++;
#pragma region Prepare display
    // Clear display
    display.clearDisplay();
    // Set the color - laways use white despiye actual display color
    display.setTextColor(WHITE);
#pragma endregion

    int currentPos = 0;
    bool drawSectionDevider = true;

    for (int i = 0; i < NO_OF_SECTIONS; i++)
    {
        switch (content[i])
        {
        case CLIMATE_LIVE_INFO:
            display.setTextSize(1);
            // Display headline
            display.setCursor(0, currentPos);
            display.printf("%-15s %5d", climatMonitorData->getLocation(), no_of_updates);
            currentPos += FONT_SIZE_1_LINE_INCREMENT;
            // Display Humidity
            display.setCursor(0, currentPos);
            display.printf("Humidity:      %3.1f %%", climatMonitorData->getHumidity());
            currentPos += FONT_SIZE_1_LINE_INCREMENT;
            // Display Teperature
            display.setCursor(0, currentPos);
            display.printf("Temperature:   %3.1f C", climatMonitorData->getTemperature());
            currentPos += FONT_SIZE_1_LINE_INCREMENT;
            break;
        case CLIMATE_MIN_MAX_INFO:
            display.setTextSize(1);
            // Display min/max data
            display.setCursor(0, currentPos);
            display.print("-------- Min -  Max -");
            currentPos += FONT_SIZE_1_LINE_INCREMENT;
            // Display Humidity
            display.setCursor(0, currentPos);
            display.printf("Humid:  %3.1f - %3.1f %%", climatMonitorData->getMinHumidity(), climatMonitorData->getMaxHumidity());
            currentPos += FONT_SIZE_1_LINE_INCREMENT;
            // Display Teperature
            display.setCursor(0, currentPos);
            display.printf("Temp:   %3.1f - %3.1f C", climatMonitorData->getMinTemperature(), climatMonitorData->getMaxTemperature());
            currentPos += FONT_SIZE_1_LINE_INCREMENT;
            break;
        case CLIMATE_WIFI_INFO:
            display.setTextSize(1);
            // Display network data0
            display.setCursor(0, currentPos);
            display.print("------ Network ------");
            currentPos += FONT_SIZE_1_LINE_INCREMENT;
            // Display SSID
            display.setCursor(0, currentPos);
            display.printf("SSID:  %14s", SSID);
            currentPos += FONT_SIZE_1_LINE_INCREMENT;
            // Display IP-addres
            display.setCursor(0, currentPos);
            display.printf("IP:    %14s", localIP);
            currentPos += FONT_SIZE_1_LINE_INCREMENT;
            break;
        case CLIMATE_TEMPERATURE_INFO:
            drawSectionDevider = true;
            drawBitmap(4, currentPos, TEMPERATURE_BMP_WIDTH, TEMPERATURE_BMP_HEIGHT, temperatureBitmap);

            display.setTextSize(2);
            display.setCursor(TEMPERATURE_BMP_WIDTH + 16, currentPos);
            display.printf(" %3.1fC", climatMonitorData->getTemperature());
            display.setTextSize(1);
            display.setCursor(TEMPERATURE_BMP_WIDTH + 16, currentPos + FONT_SIZE_2_LINE_INCREMENT);
            display.printf(" %3.1f - %3.1fC", climatMonitorData->getMinTemperature(), climatMonitorData->getMaxTemperature());
            currentPos += 32;
            break;
        case CLIMATE_HUMIDITY_INFO:
            drawSectionDevider = true;
            currentPos += 2;
            drawBitmap(0, currentPos, HUMIDITY_BMP_WIDTH, HUMIDITY_BMP_HEIGHT, humidityBitmap);

            display.setTextSize(2);
            display.setCursor(HUMIDITY_BMP_WIDTH + 8, currentPos);
            display.printf(" %3.1f%%", climatMonitorData->getHumidity());
            display.setTextSize(1);
            display.setCursor(HUMIDITY_BMP_WIDTH + 8, currentPos + FONT_SIZE_2_LINE_INCREMENT);
            display.printf(" %3.1f - %3.1f%%", climatMonitorData->getMinHumidity(), climatMonitorData->getMaxHumidity());
            currentPos += 32;
            break;
        }

        if (drawSectionDevider && i < NO_OF_SECTIONS - 1) // Only draw line between sections
        {
            display.drawLine(10, currentPos, 118, currentPos, WHITE);
            currentPos += 2;
        }
    }

    // Update display
    display.display();
}

void ClimateMonitorOLED::dialogBox(const char *heading, const char *message[], const char *btnText[5])
{
    display.drawRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, WHITE);
    display.drawLine(0, 11, SCREEN_WIDTH - 1, 11, WHITE);
}

void ClimateMonitorOLED::invertDisplay(bool invert)
{
    display.invertDisplay(invert);
}

#ifdef INCLUDE_DISPLAY_TEST
void ClimateMonitorOLED::testDisplay()
{
    // Clear display
    display.clearDisplay();
    // Set the color - laways use white despiye actual display color
    display.setTextColor(WHITE);

    int currentYPos = 0;

    for (int i = 1; i < 4; i++)
    {
        display.setCursor(0, currentYPos);
        display.setTextSize(i);
        for (int j = 0; j < 20 / i; j++)
        {
            display.print(j % 10);
        }
        currentYPos += i * 10;
    }

    display.display();
}
#endif // #ifdef INCLUDE_DISPLAY_TEST

void ClimateMonitorOLED::shiftContent(ContentPosition position)
{
    int inx = (content[position] + 1) % NO_OF_CONTENT_TYPES;
    content[position] = static_cast<DisplayContent>(inx);
}
