# Arduino IDE Setup Guide for Word Clock (ESP32-C3)

## Your Configuration (Already Set Up!)
✅ WiFi SSID: `Mark & Kiwi IoT`
✅ WiFi Password: `49Fmn5p5`
✅ Timezone: UK (GMT/UTC with BST daylight saving)
✅ LED Pin: GPIO 8 (common for ESP32-C3)

---

## Step-by-Step Upload Instructions

### 1. Install ESP32 Board Support in Arduino IDE

1. Open **Arduino IDE**
2. Go to **File → Preferences**
3. In "Additional Board Manager URLs" field, add:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Click **OK**
5. Go to **Tools → Board → Boards Manager**
6. Search for "**esp32**"
7. Install "**esp32 by Espressif Systems**" (version 2.0.0 or higher recommended)

### 2. Install Required Libraries

Go to **Sketch → Include Library → Manage Libraries** and install:

1. **Adafruit NeoPixel** by Adafruit
   - Search: "Adafruit NeoPixel"
   - Click Install
   
2. **AnimatedGIF** by Larry Bank (bitbank2)
   - Search: "AnimatedGIF"
   - Click Install (the one by bitbank2/Larry Bank)

### 3. Open Your Sketch

1. In Arduino IDE, go to **File → Open**
2. Navigate to: `/Users/mark/Library/Mobile Documents/com~apple~CloudDocs/Documents/CursorProjects/word-clock/esp/wordclock/src/`
3. Open the `wordclock.ino` file

Arduino IDE will automatically load all the other `.cpp` and `.h` files in the same folder.

### 4. Configure Board Settings for ESP32-C3

1. **Tools → Board → ESP32 Arduino**
2. Select: **ESP32C3 Dev Module**
   
3. Configure these settings:
   - **Port:** Select your ESP32-C3's COM port (e.g., COM3 on Windows, /dev/cu.usbserial-* on Mac)
   - **Upload Speed:** 921600 (or 115200 if you have upload issues)
   - **CPU Frequency:** 160MHz
   - **Flash Mode:** QIO
   - **Flash Size:** 4MB (or match your board)
   - **Partition Scheme:** Default 4MB with spiffs
   - **Core Debug Level:** None (or Info for debugging)
   - **Erase All Flash Before Sketch Upload:** Disabled

### 5. Adjust LED Pin if Needed

The code is set to use GPIO 8 for the LED strip. If your wiring uses a different pin:

1. Open `config.h` in Arduino IDE
2. Change the line:
   ```cpp
   #define LED_PIN 8
   ```
   to your pin number (common ESP32-C3 pins: 2, 8, 9, 10)

### 6. Upload to ESP32-C3

1. Connect your ESP32-C3 to your computer via USB
2. Press and hold the **BOOT** button on your ESP32-C3 board
3. Click the **Upload** button (→) in Arduino IDE
4. Keep holding BOOT until you see "Connecting..." in the output window
5. Release BOOT once upload starts

**Note:** Some ESP32-C3 boards auto-reset and don't need the BOOT button hold.

### 7. Monitor Serial Output

1. Open **Tools → Serial Monitor**
2. Set baud rate to **9600**
3. You should see:
   - WiFi connecting dots (`.....`)
   - "WiFi connected"
   - "Downloading GIF..."
   - "GIF downloaded and loaded successfully."

---

## Troubleshooting

### Upload Failed / Port Not Found
- Make sure your USB cable supports data (not just charging)
- Install CH340/CP2102 USB drivers if needed for your ESP32-C3
- Try different USB ports
- Hold BOOT button during upload

### WiFi Connection Issues
- Double-check WiFi credentials in `config.h`
- Make sure your WiFi is 2.4GHz (ESP32 doesn't support 5GHz)
- Check signal strength

### LEDs Not Working
- Check LED_PIN matches your wiring
- Verify LED strip is WS2812B/NeoPixel compatible
- Check power supply (132 LEDs need good 5V supply)
- Verify data wire connection

### Serial Monitor Shows Garbage
- Set baud rate to 9600 in Serial Monitor

### Compilation Errors
- Make sure both libraries are installed (Adafruit NeoPixel and AnimatedGIF)
- Update ESP32 board support to latest version

---

## What This Code Does

1. **Connects to WiFi** using your credentials
2. **Syncs time** from NTP server (pool.ntp.org) for accurate UK time
3. **Downloads a GIF** (heart animation) that plays every hour
4. **Displays the time** in word format with colorful LEDs
5. **Updates every second** with smooth color changes

Enjoy your Word Clock! 🕐✨

