#include "SerialHelper.h"

void initSerial()
{
#if USE_SERIAL
    Serial.begin(9600);
    delay(1000);  // Wait for USB-CDC to initialize on ESP32-C3
#endif
}
