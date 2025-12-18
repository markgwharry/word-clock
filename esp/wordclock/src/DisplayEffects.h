#ifndef DISPLAY_EFFECTS_H
#define DISPLAY_EFFECTS_H

#include <Arduino.h>
#include "ClockDisplayHAL.h"

enum class EffectType {
    RAINBOW_WAVE,
    SPARKLE,
    MATRIX_RAIN,
    RIPPLE,
    COLOR_WIPE,
    PULSE,
    CONFETTI,
    FIREWORK,
    RANDOM
};

class DisplayEffects
{
public:
    DisplayEffects(ClockDisplayHAL *hal);

    // Play a specific effect for a duration
    void playEffect(EffectType effect, unsigned long durationMs);

    // Play a random effect
    void playRandomEffect(unsigned long durationMs);

    // Individual effects (can be called directly)
    void rainbowWave(unsigned long durationMs);
    void sparkle(unsigned long durationMs, uint32_t baseColor = 0xFFFFFF);
    void matrixRain(unsigned long durationMs);
    void ripple(unsigned long durationMs);
    void colorWipe(unsigned long durationMs, uint32_t color = 0);
    void pulse(unsigned long durationMs, uint32_t color = 0);
    void confetti(unsigned long durationMs);
    void firework(unsigned long durationMs);

private:
    ClockDisplayHAL *hal;

    // Helper functions
    uint32_t wheel(uint8_t pos);  // Color wheel for rainbow effects
    uint32_t dimColor(uint32_t color, uint8_t brightness);
    uint32_t randomColor();
    void setAllPixels(uint32_t color);
    float distance(float x1, float y1, float x2, float y2);
};

#endif
