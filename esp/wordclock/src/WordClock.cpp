#include "WordClock.h"
#include "SerialHelper.h"

const uint32_t COLORS[] = {
    0xFF0000,
    0x00FF00,
    0x0000FF,
    0xFFFF00,
    0xFF00FF,
    0x00FFFF,
    0xFFFFFF,
    0xA52A2A};

const char *GIF_URLS[] = {
    "https://raw.githubusercontent.com/markgwharry/word-clock/main/esp/wordclock/gifs/heart.gif",
    "https://raw.githubusercontent.com/markgwharry/word-clock/main/esp/wordclock/gifs/smiley.gif",
    "https://raw.githubusercontent.com/markgwharry/word-clock/main/esp/wordclock/gifs/star.gif",
    "https://raw.githubusercontent.com/markgwharry/word-clock/main/esp/wordclock/gifs/rainbow.gif",
    "https://raw.githubusercontent.com/markgwharry/word-clock/main/esp/wordclock/gifs/fireworks.gif",
    "https://raw.githubusercontent.com/markgwharry/word-clock/main/esp/wordclock/gifs/sun.gif"};
const int NUM_GIFS = 6;

WordClock::WordClock(ClockDisplayHAL *clockDisplayHAL, WiFiTimeManager *networkManager, GifPlayer *gifPlayer, DisplayEffects *displayEffects)
    : clockDisplayHAL(clockDisplayHAL), networkManager(networkManager), gifPlayer(gifPlayer), displayEffects(displayEffects), lastHour(-1), allLastHighlightedWords("") {}

void WordClock::setup()
{
    downloadAndPlayRandomGIF();
}

void WordClock::downloadAndPlayRandomGIF()
{
    int gifIndex = random(0, NUM_GIFS);
    const char *gifUrl = GIF_URLS[gifIndex];

    SERIAL_PRINT("Downloading GIF: ");
    SERIAL_PRINTLN(gifUrl);

    if (networkManager->downloadGIF(gifUrl))
    {
        uint8_t *gifBuffer = networkManager->getGifBuffer();
        size_t gifSize = networkManager->getGifBufferSize();
        if (gifSize > 0 && gifBuffer != nullptr)
        {
            if (gifPlayer->loadGIF(gifBuffer, gifSize))
            {
                SERIAL_PRINTLN("GIF downloaded and loaded successfully.");
                gifPlayer->playGIF(4000);
                clockDisplayHAL->clearPixels(false);
            }
        }
    }
    else
    {
        SERIAL_PRINTLN("Failed to download GIF, using built-in effect instead.");
        if (displayEffects != nullptr)
        {
            displayEffects->playRandomEffect(4000);
            clockDisplayHAL->clearPixels(false);
        }
    }
}

void WordClock::highlightWord(const String &word, uint32_t color)
{
    clockDisplayHAL->displayWord(word, color);
}

String WordClock::getMinutesWord(int minute)
{
    if (minute < 5)
        return "OCLOCK";
    else if (minute < 10)
        return "FIVE";
    else if (minute < 15)
        return "TEN";
    else if (minute < 20)
        return "FIFTEEN";
    else if (minute < 25)
        return "TWENTY";
    else if (minute < 30)
        return "TWENTYFIVE";
    else if (minute < 35)
        return "THIRTY";
    else if (minute < 40)
        return "TWENTYFIVE";
    else if (minute < 45)
        return "TWENTY";
    else if (minute < 50)
        return "FIFTEEN";
    else if (minute < 55)
        return "TEN";
    else
        return "FIVE";
}

uint32_t WordClock::getRandomColor()
{
    int index = random(0, sizeof(COLORS) / sizeof(COLORS[0]));
    return COLORS[index];
}

void WordClock::displayTime()
{
    struct tm currentTime = networkManager->getLocalTimeStruct();
    int hour = currentTime.tm_hour % 12;
    if (hour == 0)
        hour = 12;
    int minute = currentTime.tm_min;

    clockDisplayHAL->clearPixels(false);

    // Play random GIF on the hour
    if (hour != lastHour && minute == 0)
    {
        lastHour = hour;
        downloadAndPlayRandomGIF();
    }

    highlightWord("IT", getRandomColor());
    highlightWord("IS", getRandomColor());
    String allHighlightedWords = "ITIS";

    if (minute < 5)
    {
        highlightWord("OCLOCK", getRandomColor());
        allHighlightedWords += "OCLOCK";
    }
    else if (minute < 35)
    {
        highlightWord("PAST", getRandomColor());
        highlightWord("MINUTES", getRandomColor());
        allHighlightedWords += "PASTMINUTES";
    }
    else
    {
        highlightWord("TO", getRandomColor());
        highlightWord("MINUTES", getRandomColor());
        allHighlightedWords += "TOMINUTES";
        hour = (hour + 1) % 12;
        if (hour == 0)
            hour = 12;
    }

    String hourWord = "HOUR_" + String(hour);
    highlightWord(getMinutesWord(minute), getRandomColor());
    allHighlightedWords += getMinutesWord(minute);
    highlightWord(hourWord, getRandomColor());
    allHighlightedWords += hourWord;

    if (allLastHighlightedWords != allHighlightedWords)
    {
        clockDisplayHAL->show();
        allLastHighlightedWords = allHighlightedWords;
    }
}
