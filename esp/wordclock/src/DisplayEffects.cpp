#include "DisplayEffects.h"

DisplayEffects::DisplayEffects(ClockDisplayHAL *hal) : hal(hal) {}

// Color wheel - input 0-255, outputs rainbow colors
uint32_t DisplayEffects::wheel(uint8_t pos)
{
    pos = 255 - pos;
    if (pos < 85)
    {
        return hal->pixels.Color(255 - pos * 3, 0, pos * 3);
    }
    if (pos < 170)
    {
        pos -= 85;
        return hal->pixels.Color(0, pos * 3, 255 - pos * 3);
    }
    pos -= 170;
    return hal->pixels.Color(pos * 3, 255 - pos * 3, 0);
}

uint32_t DisplayEffects::dimColor(uint32_t color, uint8_t brightness)
{
    uint8_t r = ((color >> 16) & 0xFF) * brightness / 255;
    uint8_t g = ((color >> 8) & 0xFF) * brightness / 255;
    uint8_t b = (color & 0xFF) * brightness / 255;
    return hal->pixels.Color(r, g, b);
}

uint32_t DisplayEffects::randomColor()
{
    return wheel(random(256));
}

void DisplayEffects::setAllPixels(uint32_t color)
{
    for (uint8_t y = 0; y < ClockDisplayHAL::HEIGHT; y++)
    {
        for (uint8_t x = 0; x < ClockDisplayHAL::WIDTH; x++)
        {
            hal->setPixel(x, y, color);
        }
    }
}

float DisplayEffects::distance(float x1, float y1, float x2, float y2)
{
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void DisplayEffects::playEffect(EffectType effect, unsigned long durationMs)
{
    switch (effect)
    {
    case EffectType::RAINBOW_WAVE:
        rainbowWave(durationMs);
        break;
    case EffectType::SPARKLE:
        sparkle(durationMs);
        break;
    case EffectType::MATRIX_RAIN:
        matrixRain(durationMs);
        break;
    case EffectType::RIPPLE:
        ripple(durationMs);
        break;
    case EffectType::COLOR_WIPE:
        colorWipe(durationMs);
        break;
    case EffectType::PULSE:
        pulse(durationMs);
        break;
    case EffectType::CONFETTI:
        confetti(durationMs);
        break;
    case EffectType::FIREWORK:
        firework(durationMs);
        break;
    case EffectType::RANDOM:
        playRandomEffect(durationMs);
        break;
    }
}

void DisplayEffects::playRandomEffect(unsigned long durationMs)
{
    // Pick a random effect (excluding RANDOM itself)
    EffectType effects[] = {
        EffectType::RAINBOW_WAVE,
        EffectType::SPARKLE,
        EffectType::MATRIX_RAIN,
        EffectType::RIPPLE,
        EffectType::COLOR_WIPE,
        EffectType::PULSE,
        EffectType::CONFETTI,
        EffectType::FIREWORK};
    int numEffects = sizeof(effects) / sizeof(effects[0]);
    EffectType chosen = effects[random(numEffects)];
    playEffect(chosen, durationMs);
}

// Rainbow wave sweeping across the display
void DisplayEffects::rainbowWave(unsigned long durationMs)
{
    unsigned long startTime = millis();
    uint8_t offset = 0;

    while (millis() - startTime < durationMs)
    {
        for (uint8_t y = 0; y < ClockDisplayHAL::HEIGHT; y++)
        {
            for (uint8_t x = 0; x < ClockDisplayHAL::WIDTH; x++)
            {
                // Create diagonal rainbow wave
                uint8_t colorIndex = (x * 10 + y * 20 + offset) & 0xFF;
                hal->setPixel(x, y, wheel(colorIndex));
            }
        }
        hal->show();
        offset += 5;
        delay(30);
    }
}

// Sparkle/twinkle effect - random pixels flash
void DisplayEffects::sparkle(unsigned long durationMs, uint32_t baseColor)
{
    unsigned long startTime = millis();

    while (millis() - startTime < durationMs)
    {
        hal->clearPixels(false);

        // Light up random pixels
        int numSparkles = random(5, 15);
        for (int i = 0; i < numSparkles; i++)
        {
            uint8_t x = random(ClockDisplayHAL::WIDTH);
            uint8_t y = random(ClockDisplayHAL::HEIGHT);
            uint8_t brightness = random(100, 255);
            hal->setPixel(x, y, dimColor(baseColor, brightness));
        }
        hal->show();
        delay(50);
    }
}

// Matrix rain effect - falling green columns
void DisplayEffects::matrixRain(unsigned long durationMs)
{
    unsigned long startTime = millis();

    // Track drop positions for each column
    int8_t drops[ClockDisplayHAL::WIDTH];
    uint8_t speeds[ClockDisplayHAL::WIDTH];

    // Initialize drops
    for (uint8_t x = 0; x < ClockDisplayHAL::WIDTH; x++)
    {
        drops[x] = random(-5, ClockDisplayHAL::HEIGHT);
        speeds[x] = random(1, 4);
    }

    uint8_t frameCount = 0;

    while (millis() - startTime < durationMs)
    {
        hal->clearPixels(false);

        for (uint8_t x = 0; x < ClockDisplayHAL::WIDTH; x++)
        {
            // Draw the trail
            for (int8_t trail = 0; trail < 5; trail++)
            {
                int8_t y = drops[x] - trail;
                if (y >= 0 && y < ClockDisplayHAL::HEIGHT)
                {
                    uint8_t brightness = 255 - (trail * 50);
                    uint32_t color = hal->pixels.Color(0, brightness, 0);
                    hal->setPixel(x, y, color);
                }
            }

            // Move drop down based on speed
            if (frameCount % speeds[x] == 0)
            {
                drops[x]++;
                if (drops[x] > ClockDisplayHAL::HEIGHT + 5)
                {
                    drops[x] = random(-5, 0);
                    speeds[x] = random(1, 4);
                }
            }
        }

        hal->show();
        frameCount++;
        delay(40);
    }
}

// Ripple effect - expanding circles from center
void DisplayEffects::ripple(unsigned long durationMs)
{
    unsigned long startTime = millis();
    float centerX = ClockDisplayHAL::WIDTH / 2.0;
    float centerY = ClockDisplayHAL::HEIGHT / 2.0;
    float maxDist = distance(0, 0, centerX, centerY);
    float radius = 0;
    uint8_t colorOffset = 0;

    while (millis() - startTime < durationMs)
    {
        hal->clearPixels(false);

        for (uint8_t y = 0; y < ClockDisplayHAL::HEIGHT; y++)
        {
            for (uint8_t x = 0; x < ClockDisplayHAL::WIDTH; x++)
            {
                float dist = distance(x, y, centerX, centerY);

                // Create multiple ripple rings
                float ripplePhase = fmod(dist - radius, 4.0);
                if (ripplePhase < 0)
                    ripplePhase += 4.0;

                if (ripplePhase < 1.5)
                {
                    uint8_t brightness = 255 * (1.0 - ripplePhase / 1.5);
                    uint32_t color = dimColor(wheel(colorOffset + (uint8_t)(dist * 20)), brightness);
                    hal->setPixel(x, y, color);
                }
            }
        }

        hal->show();
        radius += 0.3;
        if (radius > maxDist + 4)
        {
            radius = 0;
            colorOffset += 30;
        }
        delay(30);
    }
}

// Color wipe - fill display with color, then clear
void DisplayEffects::colorWipe(unsigned long durationMs, uint32_t color)
{
    unsigned long startTime = millis();
    unsigned long elapsed;

    while ((elapsed = millis() - startTime) < durationMs)
    {
        // Pick new color for each cycle if not specified
        uint32_t wipeColor = (color == 0) ? randomColor() : color;

        // Wipe on (diagonal)
        for (int diag = 0; diag < ClockDisplayHAL::WIDTH + ClockDisplayHAL::HEIGHT; diag++)
        {
            for (uint8_t x = 0; x < ClockDisplayHAL::WIDTH; x++)
            {
                uint8_t y = diag - x;
                if (y < ClockDisplayHAL::HEIGHT)
                {
                    hal->setPixel(x, y, wipeColor);
                }
            }
            hal->show();
            delay(20);

            if (millis() - startTime >= durationMs)
                return;
        }

        delay(200);

        // Wipe off (opposite diagonal)
        for (int diag = 0; diag < ClockDisplayHAL::WIDTH + ClockDisplayHAL::HEIGHT; diag++)
        {
            for (uint8_t x = 0; x < ClockDisplayHAL::WIDTH; x++)
            {
                uint8_t y = diag - x;
                if (y < ClockDisplayHAL::HEIGHT)
                {
                    hal->setPixel(x, y, 0);
                }
            }
            hal->show();
            delay(20);

            if (millis() - startTime >= durationMs)
                return;
        }

        delay(100);
    }
}

// Pulse/breathe effect - all LEDs fade in and out
void DisplayEffects::pulse(unsigned long durationMs, uint32_t color)
{
    unsigned long startTime = millis();
    uint32_t pulseColor = (color == 0) ? randomColor() : color;
    float phase = 0;

    while (millis() - startTime < durationMs)
    {
        // Sine wave for smooth breathing
        uint8_t brightness = (uint8_t)(127.5 * (1.0 + sin(phase)));
        setAllPixels(dimColor(pulseColor, brightness));
        hal->show();

        phase += 0.08;
        if (phase > TWO_PI)
        {
            phase -= TWO_PI;
            pulseColor = randomColor(); // Change color each cycle
        }
        delay(20);
    }
}

// Confetti - random colored pixels appearing and fading
void DisplayEffects::confetti(unsigned long durationMs)
{
    unsigned long startTime = millis();

    // Pixel brightness array for fading
    uint8_t brightness[ClockDisplayHAL::WIDTH][ClockDisplayHAL::HEIGHT] = {0};
    uint32_t colors[ClockDisplayHAL::WIDTH][ClockDisplayHAL::HEIGHT] = {0};

    while (millis() - startTime < durationMs)
    {
        // Add new confetti
        for (int i = 0; i < 2; i++)
        {
            uint8_t x = random(ClockDisplayHAL::WIDTH);
            uint8_t y = random(ClockDisplayHAL::HEIGHT);
            brightness[x][y] = 255;
            colors[x][y] = randomColor();
        }

        // Update display and fade
        for (uint8_t y = 0; y < ClockDisplayHAL::HEIGHT; y++)
        {
            for (uint8_t x = 0; x < ClockDisplayHAL::WIDTH; x++)
            {
                if (brightness[x][y] > 0)
                {
                    hal->setPixel(x, y, dimColor(colors[x][y], brightness[x][y]));
                    brightness[x][y] = (brightness[x][y] > 15) ? brightness[x][y] - 15 : 0;
                }
                else
                {
                    hal->setPixel(x, y, 0);
                }
            }
        }

        hal->show();
        delay(30);
    }
}

// Firework effect - bursts from random points
void DisplayEffects::firework(unsigned long durationMs)
{
    unsigned long startTime = millis();

    while (millis() - startTime < durationMs)
    {
        // Launch point
        uint8_t burstX = random(2, ClockDisplayHAL::WIDTH - 2);
        uint8_t burstY = random(2, ClockDisplayHAL::HEIGHT - 2);
        uint32_t burstColor = randomColor();

        // Particle positions and velocities
        const int numParticles = 12;
        float px[numParticles], py[numParticles];
        float vx[numParticles], vy[numParticles];

        // Initialize particles radiating outward
        for (int i = 0; i < numParticles; i++)
        {
            float angle = (TWO_PI / numParticles) * i;
            px[i] = burstX;
            py[i] = burstY;
            vx[i] = cos(angle) * 0.5;
            vy[i] = sin(angle) * 0.5;
        }

        // Animate burst
        for (int frame = 0; frame < 20; frame++)
        {
            hal->clearPixels(false);

            uint8_t brightness = 255 - (frame * 12);

            for (int i = 0; i < numParticles; i++)
            {
                px[i] += vx[i];
                py[i] += vy[i];

                int x = (int)px[i];
                int y = (int)py[i];

                if (x >= 0 && x < ClockDisplayHAL::WIDTH &&
                    y >= 0 && y < ClockDisplayHAL::HEIGHT)
                {
                    hal->setPixel(x, y, dimColor(burstColor, brightness));
                }
            }

            hal->show();
            delay(40);

            if (millis() - startTime >= durationMs)
                return;
        }

        delay(100);
    }
}
