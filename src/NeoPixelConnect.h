/*
 Copyright (c) 2020-2024 Alan Yorinks All rights reserved.

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE
 Version 3 as published by the Free Software Foundation; either
 or (at your option) any later version.
 This library is distributed in the hope that it will be useful,f
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 General Public License for more details.

 You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE
 along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef NEOPIXEL_CONNECT_NEOPIXELCONNECT_H
#define NEOPIXEL_CONNECT_NEOPIXELCONNECT_H

#include <Arduino.h>
#include "stdio.h"
#include <stdlib.h>

#include "hardware/pio.h"
#include "hardware/dma.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"

#include "ws2812.pio.h"

#ifndef MAXIMUM_NUM_NEOPIXELS
#define MAXIMUM_NUM_NEOPIXELS 1024
#endif

// Pixel buffer array offsets
#define RED 0
#define GREEN 1
#define BLUE 2

class NeoPixelConnect
{
public:
    /// @brief Constructor
    /// @param pinNumber: GPIO pin that controls the NeoPixel string.
    /// @param numberOfPixels: Number of pixels in the string
    /// @param pio: pio selected - default = pio0. pio1 may be specified
    /// @param sm: state machine selected. Default = 0
    NeoPixelConnect(byte pinNumber, uint16_t numberOfPixels);

    /// @brief Constructor
    /// @param pinNumber: GPIO pin that controls the NeoPixel string.
    /// @param numberOfPixels: Number of pixels in the string
    /// This constructor sets pio=pio0 and sm to 0
    NeoPixelConnect(byte pinNumber, uint16_t numberOfPixels, PIO pio, uint sm);

    /// @brief Destructor
    virtual ~NeoPixelConnect() {};

    ///@brief Initialize the class instance after calling constructor
    /// @param pinNumber: GPIO pin that controls the NeoPixel string.
    /// @param numberOfPixels: Number of pixels in the string
    void neoPixelInit(byte pinNumber, uint16_t numberOfPixels);

    /// @brief Set a NeoPixel to a given color. By setting autoShow to true, change is
    /// displayed immediately.
    /// @param pixelNumber: set a color for a specific neopixel in the string
    /// @param r: red value (0-255)
    /// @param g: green value(0-255)
    /// @param b: blue value (0-255)
    /// @param autoShow: If true, show the change immediately.
    void neoPixelSetValue(uint16_t pixel_number, uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, bool autoShow = false);

    /// @brief Set all the pixels to "off".
    /// @param autoShow: If true, show the change immediately
    // set all pixels to 0
    void neoPixelClear(bool autoShow = true);

    /// @brief Fill all the pixels with same value
    /// @param r: red value (0-255)
    /// @param g: green value(0-255)
    /// @param b: blue value (0-255)
    /// @param autoShow: If true, show the change immediately.
    void neoPixelFill(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, bool autoShow = true);

    /// @brief Display all the pixels in the buffer
    void neoPixelShow(void);

    /// @brief set a pixel's value to reflect pixel_grb
    /// @param pixel_grb: rgb represented as a 32 bit value
    void putPixel(uint32_t pixel_grb); //{

    /// @brief recalculate the clock to match a new CPU clock
    void recalculateClock();

    /// @brief Get the current offset for
    /// @return The current offset value
    uint16_t getOffset();

    /// @brief Set the offset for the rainbow chase.
    /// @param offset: The new offset value.
    void setOffset(uint16_t offset);

    /// @brief Convert a hue value to a 32-bit RGB value.
    /// @param hue: The hue value (0-65535).
    /// @return The 32-bit RGB value.
    uint32_t ColorHSV(uint16_t hue);

    /// @brief Set the brightness of the NeoPixels.
    /// @param brightness: The brightness value (0-255).
    void setBrightness(uint8_t brightness);

    /// @brief Blend two colors based on a ratio.
    /// @param color1: The first color (32-bit RGB).
    /// @param color2: The second color (32-bit RGB).
    /// @param ratio: The blending ratio (0.0 to 1.0).
    uint32_t blendColors(uint32_t color1, uint32_t color2, float ratio);

private:
    // pio - 0 or 1
    PIO pixelPio;

    // brightness value
    uint8_t currentBrightness = 255;

    // calculated program offset in memory
    uint pixelOffset;

    // pio state machine to use
    uint pixelSm;

    // number of pixels in the strip
    uint16_t actual_number_of_pixels;

    // a buffer that holds the color for each pixel
    uint8_t pixelBuffer[MAXIMUM_NUM_NEOPIXELS][3];

    // a buffer that holds the original color for each pixel
    uint8_t originalBuffer[MAXIMUM_NUM_NEOPIXELS][3];

    // create a 32 bit value combining the 3 colors
    uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);
};

#endif // NEOPIXEL_CONNECT_NEOPIXELCONNECT_H
