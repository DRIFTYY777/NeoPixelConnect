/*
 * Example sketch for the NeoPixelConnect library.
 * This sketch demonstrates how to control a NeoPixel strip using the library.
 * It cycles through red, green, and blue colors with a delay in between.
 */

#include <NeoPixelConnect.h>

#define RGB_PIN 13   // RGB LED pin
#define LED_COUNT 16 // Number of LEDs in the strip

// Create an instance of the NeoPixelConnect class
NeoPixelConnect strip(RGB_PIN, LED_COUNT, pio0, 0);

void setup()
{
    // Initialize the NeoPixel strip
    strip.neoPixelInit(RGB_PIN, LED_COUNT);
    strip.setBrightness(255); // Set brightness to maximum
}

void loop()
{
    // Set all pixels to red
    strip.neoPixelFill(255, 0, 0);
    strip.neoPixelShow();
    delay(500); // Wait for 0.5 seconds

    // Set all pixels to green
    strip.neoPixelFill(0, 255, 0);
    strip.neoPixelShow();
    delay(500); // Wait for 0.5 seconds

    // Set all pixels to blue
    strip.neoPixelFill(0, 0, 255);
    strip.neoPixelShow();
    delay(500); // Wait for 0.5 seconds

    // Clear all pixels (Black)
    strip.neoPixelClear();
    strip.neoPixelShow();
    delay(500); // Wait for 0.5 seconds
}