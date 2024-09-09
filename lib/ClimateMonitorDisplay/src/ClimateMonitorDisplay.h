/**
 * ClimateMonitorDISPLAY.h
 *
 * ClimateMonitorDISPLAY Library header for controlling and displaying Climat Monitor data on OLED display.
 *
 * @author Creator: Jens Hjuler Lindhardt
 * @author Lindhardt Freelance
 * @version 1.0.0
 * @license MIT
 */
#ifndef CLIMAT_MONITOR_DISPLAY_H
#define CLIMAT_MONITOR_DISPLAY_H

// Include Wire Library for I2C
#include <Wire.h>

// Include OLED libraries
#include <Adafruit_GFX.h>

// Climat data library
#include <ClimateMonitorData.h>

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the setPins function of the Wire library.
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define I2C_SDA 12
#define I2C_SCL 13

#endif // #ifndef CLIMAT_MONITOR_DISPLAY_H