#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <string.h>
#include "Screen.h"
#include "MenuScreen.h"
#include "StartScreen.h"
#include "UIConstants.h"
#include "ScreenManager.h"


// SH1106 I2C constructor for 1.3" 128x64 OLED
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

const char VERSION[7] = "v0.5.0";

Screen* currentScreen = nullptr;
Screen* nextScreen = nullptr;

void requestScreen(Screen* s) {
    nextScreen = s;
}


ButtonEvent getButtonEvent() {
    static uint32_t lastPress = 0;
    const uint32_t debounceMs = 150;
    uint32_t now = millis();
    if (now - lastPress < debounceMs) return BTN_NONE;

    if (digitalRead(8) == LOW) {
        lastPress = now;
        return BTN_DOWN;
    }
    if (digitalRead(3) == LOW) {
        lastPress = now;
        return BTN_UP;
    }
    if (digitalRead(46) == LOW) {
        lastPress = now;
        return BTN_SELECT;
    }
    if (digitalRead(9) == LOW) {
        lastPress = now;
        return BTN_BACK;
    }
    return BTN_NONE;
}


/*
===============================================================================
 Future Features / Screens To Be Implemented
===============================================================================

[ ] Device Scanner
    - Scan local network for connected devices
    - Display IP, MAC, vendor (if possible)
    - Select device for more details

[ ] Packet Sniffer
    - Promiscuous mode capture
    - Packet counters by type
    - Optional live scrolling view

[ ] Channel Analyzer
    - Scan channel utilisation
    - Graph or list AP count per channel
    - Highlight best channels

[ ] RSSI Meter
    - Live RSSI tracking for selected network
    - Min / max / average signal strength
    - Optional graph view

[ ] Wardriving Logger
    - Log SSID, RSSI, channel, encryption
    - Store to flash / SD (future)
    - Timestamped entries

[ ] Fake AP
    - Create configurable beacon frames
    - SSID, channel, encryption options
    - Start / stop control

[ ] Deauth Tester
    - Select target AP
    - Controlled deauth bursts
    - Safety limits and warnings

[ ] Battery Monitor
    - Read battery voltage
    - Estimate percentage
    - Low-power warnings

-------------------------------------------------------------------------------
 Architecture Notes:
 - Each feature should be implemented as its own Screen (.h/.cpp)
 - No feature logic belongs in main.cpp
 - Navigation handled exclusively via ScreenManager
===============================================================================
*/



void setup() {
    // Start serial for debugging
    Serial.begin(115200);
    // Start the custom I2C pins
    Wire.begin(48, 47);  // SDA=48, SCL=47
    // Initialize the display
    u8g2.begin();

    pinMode(8, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    pinMode(46, INPUT_PULLUP);
    pinMode(9, INPUT_PULLUP);

    // Draw initial message
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x12_mr);
    u8g2.drawStr(0, 18, "Wifi Scraper");
    u8g2.drawStr(0, 32, "Initializing...");
    u8g2.drawStr(80, 60, VERSION);
    u8g2.sendBuffer();
    delay(1500);
    requestScreen(&StartScreen);
}


void loop() {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x12_mr);

    if (nextScreen && nextScreen != currentScreen) {
        if (currentScreen && currentScreen->onExit)
            currentScreen->onExit();

        currentScreen = nextScreen;

        if (currentScreen && currentScreen->onEnter)
            currentScreen->onEnter();

        nextScreen = nullptr;
    }


    ButtonEvent evt = getButtonEvent();

    if (currentScreen) {
        if (currentScreen->onEvent)
            currentScreen->onEvent(evt);

        if (currentScreen->onDraw)
            currentScreen->onDraw();
    }


    u8g2.sendBuffer();
}