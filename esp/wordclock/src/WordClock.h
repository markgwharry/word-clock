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

    // Allow setting the hourly effect type (default is RANDOM)
    void setHourlyEffect(EffectType effect);
    EffectType getHourlyEffect() const;

private:
    int lastHour;
    String allLastHighlightedWords;
    ClockDisplayHAL *clockDisplayHAL;
    WiFiTimeManager *networkManager;
    GifPlayer *gifPlayer;
    DisplayEffects *displayEffects;
    bool gifDownloaded;
    EffectType hourlyEffect;

    void downloadGIF();
    void playHourlyAnimation();
    void highlightWord(const String &word, uint32_t color = 0xFFFFFF);
    String getMinutesWord(int minute);
    uint32_t getRandomColor();
};

#endif
