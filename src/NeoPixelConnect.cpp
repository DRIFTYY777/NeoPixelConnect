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
#include "NeoPixelConnect.h"

/// @brief Constructor - pio will be set to pio0 and sm to 0
/// @param pinNumber: GPIO pin that controls the NeoPixel string.
/// @param numberOfPixels: Number of pixels in the string
NeoPixelConnect::NeoPixelConnect(byte pinNumber, uint16_t numberOfPixels)
{
    this->pixelSm = 0;
    this->pixelPio = pio0;
    this->neoPixelInit(pinNumber, numberOfPixels);
}

/// @brief Constructor
/// @param pinNumber: GPIO pin that controls the NeoPixel string.
/// @param numberOfPixels: Number of pixels in the string
/// @param pio: pio selected - default = pio0. pio1 may be specified
/// @param sm: state machine selected. Default = 0
NeoPixelConnect::NeoPixelConnect(byte pinNumber, uint16_t numberOfPixels, PIO pio, uint sm)
{
    this->pixelSm = sm;
    this->pixelPio = pio;
    this->neoPixelInit(pinNumber, numberOfPixels);
}

/// @brief Continuation of Constructor
/// @param pinNumber: GPIO pin that controls the NeoPixel string.
/// @param numberOfPixels: Number of pixels in the string
void NeoPixelConnect::neoPixelInit(byte pinNumber, uint16_t numberOfPixels)
{
    uint offset = pio_add_program(this->pixelPio, &ws2812_program);
    ws2812_program_init(this->pixelPio, this->pixelSm, offset, pinNumber, 800000, false);

    // save the number of pixels in use
    this->actual_number_of_pixels = numberOfPixels;

    // set the pixels to the fill color
    for (uint16_t i = 0; i < this->actual_number_of_pixels; i++)
    {
        this->pixelBuffer[i][RED] = 0 - (0 * (255 - currentBrightness)) / 255;
        this->pixelBuffer[i][GREEN] = 0 - (0 * (255 - currentBrightness)) / 255;
        this->pixelBuffer[i][BLUE] = 0 - (0 * (255 - currentBrightness)) / 255;
    }

    // show the pixels
    this->neoPixelShow();
    delay(1);
}

/// @brief Set a NeoPixel to a given color. By setting autoShow to true, change is
/// displayed immediately.
/// @param pixelNumber: set a color for a specific neopixel in the string
/// @param r: red value (0-255)
/// @param g: green value(0-255)
/// @param b: blue value (0-255)
/// @param autoShow: If true, show the change immediately.
void NeoPixelConnect::neoPixelSetValue(uint16_t pixelNumber, uint8_t r, uint8_t g, uint8_t b, bool autoShow)
{
    // Store original values
    originalBuffer[pixelNumber][RED] = r;
    originalBuffer[pixelNumber][GREEN] = g;
    originalBuffer[pixelNumber][BLUE] = b;

    // Apply current brightness
    pixelBuffer[pixelNumber][RED] = (r * currentBrightness) / 255;
    pixelBuffer[pixelNumber][GREEN] = (g * currentBrightness) / 255;
    pixelBuffer[pixelNumber][BLUE] = (b * currentBrightness) / 255;

    if (autoShow)
    {
        neoPixelShow();
    }
}

/// @brief Set all the pixels to "off".
/// @param autoShow: If true, show the change immediately
void NeoPixelConnect::neoPixelClear(bool autoShow)
{
    // set all the neopixels in the buffer to all zeroes
    for (uint16_t i = 0; i < this->actual_number_of_pixels; i++)
    {
        this->pixelBuffer[i][RED] = 0 - (0 * (255 - currentBrightness)) / 255;
        this->pixelBuffer[i][GREEN] = 0 - (0 * (255 - currentBrightness)) / 255;
        this->pixelBuffer[i][BLUE] = 0 - (0 * (255 - currentBrightness)) / 255;
    }
    if (autoShow)
    {
        this->neoPixelShow();
    }
}

/// @brief Fill all the pixels with same value
/// @param r: red value (0-255)
/// @param g: green value(0-255)
/// @param b: blue value (0-255)
/// @param autoShow: If true, show the change immediately.
void NeoPixelConnect::neoPixelFill(uint8_t r, uint8_t g, uint8_t b, bool autoShow)
{
    // fill all the neopixels in the buffer with the
    // specified rgb values.
    for (uint16_t i = 0; i < this->actual_number_of_pixels; i++)
    {
        this->pixelBuffer[i][RED] = r - (r * (255 - currentBrightness)) / 255;
        this->pixelBuffer[i][GREEN] = g - (g * (255 - currentBrightness)) / 255;
        this->pixelBuffer[i][BLUE] = b - (b * (255 - currentBrightness)) / 255;
    }
    if (autoShow)
    {
        this->neoPixelShow();
    }
}

/// @brief Display all the pixels in the buffer
void NeoPixelConnect::neoPixelShow(void)
{
    // show the neopixels in the buffer
    for (uint16_t i = 0; i < this->actual_number_of_pixels; i++)
    {
        this->putPixel(
            urgb_u32(
                pixelBuffer[i][RED],
                pixelBuffer[i][GREEN],
                pixelBuffer[i][BLUE]));
    }
}

/// @brief convert rgb values into a single uint32_t
/// @param r: red value (0-255)
/// @param g: green value(0-255)
/// @param b: blue value (0-255)
uint32_t NeoPixelConnect::urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) |
           ((uint32_t)(g) << 16) |
           (uint32_t)(b);
}

/// @brief set a pixel's value to reflect pixel_grb
/// @param pixel_grb: rgb represented as a 32 bit value
void NeoPixelConnect::putPixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(this->pixelPio, this->pixelSm,
                        pixel_grb << 8u);
}

/// @brief recalculate the clock to match a new CPU clock
void NeoPixelConnect::recalculateClock()
{
    int cycles_per_bit = ws2812_T1 + ws2812_T2 + ws2812_T3;
    float div = clock_get_hz(clk_sys) / (800000.0f * cycles_per_bit);
    pio_sm_set_clkdiv(this->pixelPio, this->pixelSm, div);
}

uint16_t NeoPixelConnect::getOffset()
{
    // return the current offset for the rainbow chase
    return this->pixelOffset;
}

uint32_t NeoPixelConnect::ColorHSV(uint16_t hue)
{
    uint8_t r, g, b;
    if (hue < 0x5555)
    {
        r = 0xFF;
        g = (hue * 2) >> 8;
        b = 0;
    }
    else if (hue < 0xAAAA)
    {
        r = 0xFF - ((hue - 0x5555) *
                    2) >>
            8;
        g = 0xFF;
        b = (hue - 0x5555) * 2 >> 8;
    }
    else if (hue < 0xFFFF)
    {
        r = 0;
        g = 0xFF - ((hue - 0xAAAA) * 2) >> 8;
        b = 0xFF;
    }
    else
    {
        r = (hue - 0xFFFF) * 2 >> 8;
        g = 0;
        b = 0xFF;
    }
    return urgb_u32(r, g, b);
}

void NeoPixelConnect::setOffset(uint16_t offset)
{
    // set the offset for the rainbow chase
    this->pixelOffset = offset;
}

void NeoPixelConnect::setBrightness(uint8_t brightness)
{
    currentBrightness = brightness;

    // Update all pixels with new brightness
    for (uint16_t i = 0; i < actual_number_of_pixels; i++)
    {
        pixelBuffer[i][RED] = (originalBuffer[i][RED] * currentBrightness) >> 8;
        pixelBuffer[i][GREEN] = (originalBuffer[i][GREEN] * currentBrightness) >> 8;
        pixelBuffer[i][BLUE] = (originalBuffer[i][BLUE] * currentBrightness) >> 8;
    }
    neoPixelShow();
}

uint32_t NeoPixelConnect::blendColors(uint32_t color1, uint32_t color2, float ratio)
{
    uint8_t r1 = (color1 >> 8) & 0xFF;
    uint8_t g1 = (color1 >> 16) & 0xFF;
    uint8_t b1 = color1 & 0xFF;

    uint8_t r2 = (color2 >> 8) & 0xFF;
    uint8_t g2 = (color2 >> 16) & 0xFF;
    uint8_t b2 = color2 & 0xFF;

    uint8_t r = r1 + (r2 - r1) * ratio;
    uint8_t g = g1 + (g2 - g1) * ratio;
    uint8_t b = b1 + (b2 - b1) * ratio;

    return urgb_u32(r, g, b);
}
