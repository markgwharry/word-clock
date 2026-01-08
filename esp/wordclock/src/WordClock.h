#ifndef WORDCLOCK_H
#define WORDCLOCK_H

#include <Arduino.h>
#include "ClockDisplayHAL.h"
#include "NetworkManager.h"
#include "GifPlayer.h"
#include "DisplayEffects.h"

class WordClock
{
public:
    WordClock(ClockDisplayHAL *clockDisplayHAL, WiFiTimeManager *networkManager, GifPlayer *gifPlayer, DisplayEffects *displayEffects);
    void setup();
    void displayTime();

private:
    int lastHour;
    String allLastHighlightedWords;
    ClockDisplayHAL *clockDisplayHAL;
    WiFiTimeManager *networkManager;
    GifPlayer *gifPlayer;
    DisplayEffects *displayEffects;

    void downloadAndPlayRandomGIF();
    void highlightWord(const String &word, uint32_t color = 0xFFFFFF);
    String getMinutesWord(int minute);
    uint32_t getRandomColor();
};

#endif
