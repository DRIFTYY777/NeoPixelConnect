/*
 Smooth Chasing Rainbow Example for NeoPixelConnect Library

 This example demonstrates various smooth rainbow chasing effects:
 1. Classic rainbow chase - rainbow moves along the strip
 2. Rainbow comet - bright head with fading tail
 3. Dual direction chase - rainbows moving in opposite directions
 4. Breathing rainbow - intensity pulses while chasing
 5. Wave rainbow - sine wave modulated rainbow
 6. Spiral rainbow - for circular LED arrangements

 The effects automatically cycle every 10 seconds.

 Hardware: Arduino Nano RP2040 Connect with WS2812 LED strip
 Author: Generated example for NeoPixelConnect library
*/

#include <NeoPixelConnect.h>

#define RGB_PIN 13   // RGB LED pin
#define LED_COUNT 16 // Number of LEDs in the strip

// Create NeoPixel instance
NeoPixelConnect strip(RGB_PIN, LED_COUNT);

// Animation variables
unsigned long lastUpdate = 0;
unsigned long lastModeChange = 0;
uint16_t rainbowOffset = 0;
int currentMode = 0;
float wavePhase = 0;
float breathPhase = 0;

// Timing constants
const unsigned long UPDATE_INTERVAL = 50;  // 50ms = 20 FPS
const unsigned long MODE_DURATION = 10000; // 10 seconds per mode
const uint16_t HUE_STEP = 256;             // How fast the rainbow moves
const int RAINBOW_WIDTH = LED_COUNT;       // Width of rainbow across strip

void setup()
{
    Serial.begin(115200);
    delay(2000);
    Serial.println("Smooth Chasing Rainbow Example");
    Serial.println("Cycling through different rainbow effects...");

    // Set brightness for optimal visibility
    strip.setBrightness(200);

    // Initialize timing
    lastUpdate = millis();
    lastModeChange = millis();
}

void loop()
{
    unsigned long currentTime = millis();

    // Check if it's time to change modes
    if (currentTime - lastModeChange > MODE_DURATION)
    {
        currentMode = (currentMode + 1) % 6; // Cycle through 6 effects
        lastModeChange = currentTime;
        printCurrentMode();
    }

    // Update animation at regular intervals
    if (currentTime - lastUpdate > UPDATE_INTERVAL)
    {
        switch (currentMode)
        {
        case 0:
            classicRainbowChase();
            break;
        case 1:
            rainbowComet();
            break;
        case 2:
            dualDirectionChase();
            break;
        case 3:
            breathingRainbow();
            break;
        case 4:
            waveRainbow();
            break;
        case 5:
            spiralRainbow();
            break;
        }

        strip.neoPixelShow();
        lastUpdate = currentTime;

        // Increment animation counters
        rainbowOffset += HUE_STEP;
        wavePhase += 0.15;
        breathPhase += 0.08;
    }
}

void printCurrentMode()
{
    const char *modeNames[] = {
        "Classic Rainbow Chase",
        "Rainbow Comet",
        "Dual Direction Chase",
        "Breathing Rainbow",
        "Wave Rainbow",
        "Spiral Rainbow"};
    Serial.print("Mode ");
    Serial.print(currentMode + 1);
    Serial.print(": ");
    Serial.println(modeNames[currentMode]);
}

// Effect 1: Classic smooth rainbow chase
void classicRainbowChase()
{
    for (int i = 0; i < LED_COUNT; i++)
    {
        // Calculate hue for this pixel position
        uint16_t pixelHue = rainbowOffset + (i * 65535 / RAINBOW_WIDTH);

        // Convert HSV to RGB
        uint32_t color = strip.ColorHSV(pixelHue);
        uint8_t r = (color >> 8) & 0xFF;
        uint8_t g = (color >> 16) & 0xFF;
        uint8_t b = color & 0xFF;

        strip.neoPixelSetValue(i, r, g, b, false);
    }
}

// Effect 2: Rainbow comet with fading tail
void rainbowComet()
{
    // Clear all pixels first
    strip.neoPixelClear(false);

    // Calculate comet head position
    int cometHead = (rainbowOffset / 2000) % LED_COUNT;

    // Draw comet with fading tail
    for (int i = 0; i < 8; i++)
    { // Tail length of 8 pixels
        int pos = (cometHead - i + LED_COUNT) % LED_COUNT;

        // Calculate brightness (brightest at head, fading for tail)
        float brightness = 1.0 - (i * 0.15);
        if (brightness < 0)
            brightness = 0;

        // Calculate hue for this position
        uint16_t pixelHue = rainbowOffset + (pos * 65535 / LED_COUNT);
        uint32_t color = strip.ColorHSV(pixelHue);

        uint8_t r = ((color >> 8) & 0xFF) * brightness;
        uint8_t g = ((color >> 16) & 0xFF) * brightness;
        uint8_t b = (color & 0xFF) * brightness;

        strip.neoPixelSetValue(pos, r, g, b, false);
    }
}

// Effect 3: Two rainbows chasing in opposite directions
void dualDirectionChase()
{
    for (int i = 0; i < LED_COUNT; i++)
    {
        // Calculate hue for this pixel position
        uint16_t pixelHue1 = rainbowOffset + (i * 65535 / RAINBOW_WIDTH);
        uint16_t pixelHue2 = rainbowOffset + ((LED_COUNT - i) * 65535 / RAINBOW_WIDTH);

        // Convert HSV to RGB for both directions
        uint32_t color1 = strip.ColorHSV(pixelHue1);
        uint32_t color2 = strip.ColorHSV(pixelHue2);

        // Blend colors from both directions
        uint8_t r = (((color1 >> 8) & 0xFF) + ((color2 >> 8) & 0xFF)) / 2;
        uint8_t g = (((color1 >> 16) & 0xFF) + ((color2 >> 16) & 0xFF)) / 2;
        uint8_t b = ((color1 & 0xFF) + (color2 & 0xFF)) / 2;

        strip.neoPixelSetValue(i, r, g, b, false);
    }
}

// Effect 4: Rainbow with breathing intensity
void breathingRainbow()
{
    // Calculate breathing intensity (0.3 to 1.0)
    float breathIntensity = 0.3 + 0.7 * (sin(breathPhase) + 1) / 2;

    for (int i = 0; i < LED_COUNT; i++)
    {
        uint16_t pixelHue = rainbowOffset + (i * 65535 / LED_COUNT);
        uint32_t color = strip.ColorHSV(pixelHue);

        uint8_t r = ((color >> 8) & 0xFF) * breathIntensity;
        uint8_t g = ((color >> 16) & 0xFF) * breathIntensity;
        uint8_t b = (color & 0xFF) * breathIntensity;

        strip.neoPixelSetValue(i, r, g, b, false);
    }
}

// Effect 5: Rainbow with sine wave modulation
void waveRainbow()
{
    for (int i = 0; i < LED_COUNT; i++)
    {
        // Calculate wave intensity for this pixel
        float waveIntensity = (sin(wavePhase + i * 0.5) + 1) / 2; // 0 to 1
        waveIntensity = 0.2 + 0.8 * waveIntensity;                // Scale to 0.2-1.0

        // Calculate base hue
        uint16_t pixelHue = rainbowOffset + (i * 65535 / LED_COUNT);
        uint32_t color = strip.ColorHSV(pixelHue);

        uint8_t r = ((color >> 8) & 0xFF) * waveIntensity;
        uint8_t g = ((color >> 16) & 0xFF) * waveIntensity;
        uint8_t b = (color & 0xFF) * waveIntensity;

        strip.neoPixelSetValue(i, r, g, b, false);
    }
}

// Effect 6: Spiral rainbow (works well with circular arrangements)
void spiralRainbow()
{
    for (int i = 0; i < LED_COUNT; i++)
    {
        // Create a spiral effect by using both position and time
        uint16_t spiralHue = rainbowOffset * 2 + (i * 65535 / LED_COUNT) * 3;

        // Add some sparkle with secondary modulation
        float sparkle = (sin(wavePhase * 2 + i * 0.8) + 1) / 2;
        sparkle = 0.6 + 0.4 * sparkle; // Scale to 0.6-1.0

        uint32_t color = strip.ColorHSV(spiralHue);

        uint8_t r = ((color >> 8) & 0xFF) * sparkle;
        uint8_t g = ((color >> 16) & 0xFF) * sparkle;
        uint8_t b = (color & 0xFF) * sparkle;

        strip.neoPixelSetValue(i, r, g, b, false);
    }
}
