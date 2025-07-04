/*
 Color Palette Example for NeoPixelConnect Library

 This example demonstrates various color palettes and effects:
 1. Basic RGB colors
 2. Rainbow spectrum using HSV
 3. Predefined color palettes (warm, cool, nature, etc.)
 4. Smooth color transitions
 5. Brightness control

 Hardware: Arduino Nano RP2040 Connect with WS2812 LED strip
 Author: Generated example for NeoPixelConnect library
*/

#include <NeoPixelConnect.h>

#define RGB_PIN 13   // RGB LED pin
#define LED_COUNT 16 // Number of LEDs in the strip

// Create NeoPixel instance
NeoPixelConnect strip(RGB_PIN, LED_COUNT);

// Color palette structures
struct Color
{
    uint8_t r, g, b;
};

// Predefined color palettes
const Color basicColors[] = {
    {255, 0, 0},    // Red
    {0, 255, 0},    // Green
    {0, 0, 255},    // Blue
    {255, 255, 0},  // Yellow
    {255, 0, 255},  // Magenta
    {0, 255, 255},  // Cyan
    {255, 255, 255} // White
};

const Color warmColors[] = {
    {255, 100, 0},   // Orange
    {255, 50, 0},    // Red-Orange
    {255, 200, 50},  // Warm Yellow
    {255, 150, 100}, // Peach
    {200, 100, 50},  // Brown
    {255, 180, 120}, // Light Orange
    {180, 50, 0}     // Dark Orange
};

const Color coolColors[] = {
    {0, 100, 255},   // Sky Blue
    {50, 200, 255},  // Light Blue
    {0, 255, 200},   // Turquoise
    {100, 150, 255}, // Lavender
    {0, 150, 100},   // Teal
    {150, 200, 255}, // Pale Blue
    {50, 100, 200}   // Deep Blue
};

const Color natureColors[] = {
    {34, 139, 34},   // Forest Green
    {107, 142, 35},  // Olive
    {160, 82, 45},   // Saddle Brown
    {210, 180, 140}, // Tan
    {127, 255, 0},   // Chartreuse
    {0, 100, 0},     // Dark Green
    {255, 215, 0}    // Gold
};

const Color pastelColors[] = {
    {255, 182, 193}, // Light Pink
    {173, 216, 230}, // Light Blue
    {144, 238, 144}, // Light Green
    {255, 218, 185}, // Peach Puff
    {221, 160, 221}, // Plum
    {255, 255, 224}, // Light Yellow
    {230, 230, 250}  // Lavender
};

// Current palette and settings
const Color *currentPalette;
int paletteSize;
int currentMode = 0;
unsigned long lastModeChange = 0;
const unsigned long MODE_DURATION = 5000; // 5 seconds per mode

void setup()
{
    Serial.begin(115200);
    delay(2000);
    Serial.println("NeoPixel Color Palette Example");
    Serial.println("Cycling through different color palettes...");

    // Initialize with medium brightness
    strip.setBrightness(128);

    // Start with basic colors
    currentPalette = basicColors;
    paletteSize = sizeof(basicColors) / sizeof(basicColors[0]);
}

void loop()
{
    // Check if it's time to change modes
    if (millis() - lastModeChange > MODE_DURATION)
    {
        changeMode();
        lastModeChange = millis();
    }

    // Execute current mode
    switch (currentMode)
    {
    case 0:
        displayStaticPalette(basicColors, sizeof(basicColors) / sizeof(basicColors[0]), "Basic Colors");
        break;
    case 1:
        displayStaticPalette(warmColors, sizeof(warmColors) / sizeof(warmColors[0]), "Warm Colors");
        break;
    case 2:
        displayStaticPalette(coolColors, sizeof(coolColors) / sizeof(coolColors[0]), "Cool Colors");
        break;
    case 3:
        displayStaticPalette(natureColors, sizeof(natureColors) / sizeof(natureColors[0]), "Nature Colors");
        break;
    case 4:
        displayStaticPalette(pastelColors, sizeof(pastelColors) / sizeof(pastelColors[0]), "Pastel Colors");
        break;
    case 5:
        rainbowSpectrum("Rainbow Spectrum (HSV)");
        break;
    case 6:
        smoothTransition("Smooth Color Transitions");
        break;
    case 7:
        breathingEffect("Breathing Effect");
        break;
    case 8:
        paletteChase("Palette Chase");
        break;
    }

    delay(50); // Small delay for smooth animations
}

void changeMode()
{
    currentMode = (currentMode + 1) % 9; // Cycle through 9 modes
}

void displayStaticPalette(const Color palette[], int size, const char *name)
{
    static bool printed = false;
    if (!printed)
    {
        Serial.println(name);
        printed = true;
    }

    // Map palette colors across the LED strip
    for (int i = 0; i < LED_COUNT; i++)
    {
        int paletteIndex = (i * size) / LED_COUNT;
        strip.neoPixelSetValue(i,
                               palette[paletteIndex].r,
                               palette[paletteIndex].g,
                               palette[paletteIndex].b,
                               false);
    }
    strip.neoPixelShow();

    // Reset printed flag when mode changes
    if (millis() - lastModeChange < 100)
    {
        printed = false;
    }
}

void rainbowSpectrum(const char *name)
{
    static bool printed = false;
    static unsigned long lastUpdate = 0;
    static uint16_t hueOffset = 0;

    if (!printed)
    {
        Serial.println(name);
        printed = true;
    }

    if (millis() - lastUpdate > 50)
    { // Update every 50ms
        for (int i = 0; i < LED_COUNT; i++)
        {
            // Calculate hue for this pixel
            uint16_t hue = (hueOffset + (i * 65535 / LED_COUNT)) % 65536;
            uint32_t color = strip.ColorHSV(hue);

            // Extract RGB from the 32-bit color
            uint8_t r = (color >> 8) & 0xFF;
            uint8_t g = (color >> 16) & 0xFF;
            uint8_t b = color & 0xFF;

            strip.neoPixelSetValue(i, r, g, b, false);
        }
        strip.neoPixelShow();

        hueOffset += 500; // Rotate the rainbow
        lastUpdate = millis();
    }

    // Reset printed flag when mode changes
    if (millis() - lastModeChange < 100)
    {
        printed = false;
    }
}

void smoothTransition(const char *name)
{
    static bool printed = false;
    static unsigned long lastUpdate = 0;
    static float phase = 0;

    if (!printed)
    {
        Serial.println(name);
        printed = true;
    }

    if (millis() - lastUpdate > 30)
    { // Update every 30ms
        for (int i = 0; i < LED_COUNT; i++)
        {
            // Create smooth color transitions using sine waves
            float r = (sin(phase + i * 0.3) + 1) * 127;
            float g = (sin(phase + i * 0.3 + 2.1) + 1) * 127;
            float b = (sin(phase + i * 0.3 + 4.2) + 1) * 127;

            strip.neoPixelSetValue(i, (uint8_t)r, (uint8_t)g, (uint8_t)b, false);
        }
        strip.neoPixelShow();

        phase += 0.1;
        lastUpdate = millis();
    }

    // Reset printed flag when mode changes
    if (millis() - lastModeChange < 100)
    {
        printed = false;
    }
}

void breathingEffect(const char *name)
{
    static bool printed = false;
    static unsigned long lastUpdate = 0;
    static float breathPhase = 0;
    static int colorIndex = 0;

    if (!printed)
    {
        Serial.println(name);
        printed = true;
    }

    if (millis() - lastUpdate > 50)
    { // Update every 50ms
        // Calculate breathing intensity
        float intensity = (sin(breathPhase) + 1) / 2; // 0 to 1

        // Use a color from the warm palette
        Color currentColor = warmColors[colorIndex % (sizeof(warmColors) / sizeof(warmColors[0]))];

        uint8_t r = currentColor.r * intensity;
        uint8_t g = currentColor.g * intensity;
        uint8_t b = currentColor.b * intensity;

        // Fill all LEDs with the breathing color
        strip.neoPixelFill(r, g, b, true);

        breathPhase += 0.05;

        // Change color every few breath cycles
        if (breathPhase > 12.566)
        { // 2π * 2 (two complete cycles)
            breathPhase = 0;
            colorIndex++;
        }

        lastUpdate = millis();
    }

    // Reset printed flag when mode changes
    if (millis() - lastModeChange < 100)
    {
        printed = false;
    }
}

void paletteChase(const char *name)
{
    static bool printed = false;
    static unsigned long lastUpdate = 0;
    static int chasePosition = 0;

    if (!printed)
    {
        Serial.println(name);
        printed = true;
    }

    if (millis() - lastUpdate > 200)
    { // Update every 200ms
        // Clear all pixels
        strip.neoPixelClear(false);

        // Light up 3 consecutive LEDs with different colors from nature palette
        for (int i = 0; i < 3; i++)
        {
            int pos = (chasePosition + i) % LED_COUNT;
            int colorIdx = i % (sizeof(natureColors) / sizeof(natureColors[0]));
            strip.neoPixelSetValue(pos,
                                   natureColors[colorIdx].r,
                                   natureColors[colorIdx].g,
                                   natureColors[colorIdx].b,
                                   false);
        }

        strip.neoPixelShow();
        chasePosition = (chasePosition + 1) % LED_COUNT;
        lastUpdate = millis();
    }

    // Reset printed flag when mode changes
    if (millis() - lastModeChange < 100)
    {
        printed = false;
    }
}
