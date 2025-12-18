#ifndef GIF_PLAYER_H
#define GIF_PLAYER_H

#include <Arduino.h>
#include <AnimatedGIF.h>
#include "ClockDisplayHAL.h"

class GifPlayer
{
public:
    GifPlayer(ClockDisplayHAL *clockDisplayHAL);
    bool loadGIF(uint8_t *gifBuffer, size_t gifSize);
    void playGIF(unsigned long durationMs);

private:
    ClockDisplayHAL *clockDisplayHAL;
    AnimatedGIF gif;
    static void GIFDraw(GIFDRAW *pDraw);

    // Store buffer reference for replay capability
    uint8_t *storedBuffer;
    size_t storedSize;
    bool gifLoaded;

    bool reopenGIF();

    static GifPlayer *instance;
};

#endif
