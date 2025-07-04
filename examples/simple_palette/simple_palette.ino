/*
 Simple Color Palette Example for NeoPixelConnect Library

 This is a simplified version that demonstrates:
 - Basic predefined colors
 - Rainbow effect using HSV
 - Simple palette cycling

 Hardware: Arduino Nano RP2040 Connect with WS2812 LED strip
*/

#include <NeoPixelConnect.h>

#define RGB_PIN 13   // RGB LED pin
#define LED_COUNT 16 // Number of LEDs in the strip

// Create NeoPixel instance
NeoPixelConnect strip(RGB_PIN, LED_COUNT);

// Simple color palette (R, G, B values)
uint8_t colorPalette[][3] = {
    {255, 0, 0},   // Red
    {255, 127, 0}, // Orange
    {255, 255, 0}, // Yellow
    {0, 255, 0},   // Green
    {0, 255, 255}, // Cyan
    {0, 0, 255},   // Blue
    {127, 0, 255}, // Violet
    {255, 0, 255}  // Magenta
};

int paletteSize = sizeof(colorPalette) / sizeof(colorPalette[0]);

void setup()
{
  Serial.begin(115200);
  delay(2000);
  Serial.println("Simple Color Palette Example");

  // Set medium brightness
  strip.setBrightness(100);
}

void loop()
{
  // Show each color in the palette
  showPaletteColors();
  delay(3000);

  // Show rainbow effect
  showRainbow();
  delay(3000);

  // Show color chase
  showColorChase();
  delay(3000);
}

void showPaletteColors()
{
  Serial.println("Showing palette colors...");

  // Display each color across multiple LEDs
  for (int colorIndex = 0; colorIndex < paletteSize; colorIndex++)
  {
    // Calculate how many LEDs per color
    int ledsPerColor = LED_COUNT / paletteSize;
    int startLed = colorIndex * ledsPerColor;
    int endLed = startLed + ledsPerColor;

    // Handle remainder LEDs
    if (colorIndex == paletteSize - 1)
    {
      endLed = LED_COUNT;
    }

    // Set LEDs to current color
    for (int i = startLed; i < endLed; i++)
    {
      strip.neoPixelSetValue(i,
                             colorPalette[colorIndex][0], // Red
                             colorPalette[colorIndex][1], // Green
                             colorPalette[colorIndex][2], // Blue
                             false);
    }
  }

  strip.neoPixelShow();
}

void showRainbow()
{
  Serial.println("Showing rainbow effect...");

  for (int cycle = 0; cycle < 50; cycle++)
  { // 50 animation cycles
    for (int i = 0; i < LED_COUNT; i++)
    {
      // Calculate hue for this pixel (0-65535 range)
      uint16_t hue = (cycle * 1000 + i * 65535 / LED_COUNT) % 65536;

      // Convert HSV to RGB using the library function
      uint32_t color = strip.ColorHSV(hue);

      // Extract RGB components from 32-bit color
      uint8_t r = (color >> 8) & 0xFF;
      uint8_t g = (color >> 16) & 0xFF;
      uint8_t b = color & 0xFF;

      strip.neoPixelSetValue(i, r, g, b, false);
    }

    strip.neoPixelShow();
    delay(100); // Speed of rainbow movement
  }
}

void showColorChase()
{
  Serial.println("Showing color chase...");

  for (int round = 0; round < 3; round++)
  { // 3 complete rounds
    for (int pos = 0; pos < LED_COUNT; pos++)
    {
      // Clear all LEDs
      strip.neoPixelClear(false);

      // Light up current position with a color from palette
      int colorIndex = pos % paletteSize;
      strip.neoPixelSetValue(pos,
                             colorPalette[colorIndex][0],
                             colorPalette[colorIndex][1],
                             colorPalette[colorIndex][2],
                             false);

      // Optional: light up trailing LED with dimmed color
      if (pos > 0)
      {
        int prevColorIndex = (pos - 1) % paletteSize;
        strip.neoPixelSetValue(pos - 1,
                               colorPalette[prevColorIndex][0] / 4,
                               colorPalette[prevColorIndex][1] / 4,
                               colorPalette[prevColorIndex][2] / 4,
                               false);
      }

      strip.neoPixelShow();
      delay(150); // Chase speed
    }
  }
}
