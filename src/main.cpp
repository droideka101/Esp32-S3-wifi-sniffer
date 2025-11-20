#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <string.h>

// SH1106 I2C constructor for 1.3" 128x64 OLED
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

const char VERSION[7] = "v0.3.3";

const int smallLineSpacing = 13;
int optionIndex = 0;
int topIndex = 0;
const int visibleLines = 5;
const char* options[] = {" Network Scanner ", " Device Scanner ", " Packet Sniffer ", " Channel Analyzer ", " RSSI Meter ", " Wardriving Logger ", " Fake AP ", " Deauth Tester ", " Battery Monitor "};
int lenOptions = sizeof(options) / sizeof(options[0]);

enum ButtonEvent { BTN_NONE, BTN_UP, BTN_DOWN, BTN_SELECT, BTN_BACK };
enum ScreenState { START_SCREEN, MENU_SCREEN, NETWORK_SCANNER_SCREEN, DEVICE_SCANNER_SCREEN, PACKET_SNIFFER_SCREEN, CHANNEL_ANALYZER_SCREEN, RSSI_METER_SCREEN, WARDRIVING_LOGGER_SCREEN, FAKE_AP_SCREEN, DEAUTH_TESTER_SCREEN, BATTERY_MONITOR_SCREEN };
ScreenState currentScreen = START_SCREEN;

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


void NetworkScannerDisplay(ButtonEvent evt) {
    u8g2.setCursor(0, 12);
    u8g2.print("network scanner selected");

    if (evt == BTN_BACK) {
        currentScreen = MENU_SCREEN;
    }
}


void DeviceScannerDisplay(ButtonEvent evt) {
    u8g2.setCursor(0, 12);
    u8g2.print("device scanner selected");

    if (evt == BTN_BACK) {
        currentScreen = MENU_SCREEN;
    }
}


void PacketSnifferDisplay(ButtonEvent evt) {
    u8g2.setCursor(0, 12);
    u8g2.print("packet sniffer selected");

    if (evt == BTN_BACK) {
        currentScreen = MENU_SCREEN;
    }
}


void ChannelAnalyzerDisplay(ButtonEvent evt) {
    u8g2.setCursor(0, 12);
    u8g2.print("channel analyzer selected");

    if (evt == BTN_BACK) {
        currentScreen = MENU_SCREEN;
    }
}


void RssiMeterDisplay(ButtonEvent evt) {
    u8g2.setCursor(0, 12);
    u8g2.print("RSSI meter selected");

    if (evt == BTN_BACK) {
        currentScreen = MENU_SCREEN;
    }
}


void WardrivingLoggerDisplay(ButtonEvent evt) {
    u8g2.setCursor(0, 12);
    u8g2.print("wardriving logger selected");

    if (evt == BTN_BACK) {
        currentScreen = MENU_SCREEN;
    }
}


void FakeAPDisplay(ButtonEvent evt) {
    u8g2.setCursor(0, 12);
    u8g2.print("fake AP selected");

    if (evt == BTN_BACK) {
        currentScreen = MENU_SCREEN;
    }
}


void DeauthTesterDisplay(ButtonEvent evt) {
    u8g2.setCursor(0, 12);
    u8g2.print("Deauth tester selected");

    if (evt == BTN_BACK) {
        currentScreen = MENU_SCREEN;
    }
}


void BatteryMonitorDisplay(ButtonEvent evt) {
    u8g2.setCursor(0, 12);
    u8g2.print("Battery monitor selected");

    if (evt == BTN_BACK) {
        currentScreen = MENU_SCREEN;
    }
}


void startScreen(ButtonEvent evt) {
    // --- SMALL WIFI ICON (2× size) ---
    u8g2.setFont(u8g2_font_open_iconic_www_2x_t);
    u8g2.drawGlyph(54, 24, 0x0048);

    // --- TITLE ---
    u8g2.setFont(u8g2_font_6x12_mr);
    const char* title = "WiFi Scraper";
    int tw = u8g2.getStrWidth(title);
    u8g2.setCursor((128 - tw) / 2, 40);
    u8g2.print(title);

    // --- START BUTTON FRAME ---
    const char* startText = " Start ";
    int sw = u8g2.getStrWidth(startText);

    int bx = (128 - (sw + 18)) / 2;
    int by = 46;

    u8g2.drawRBox(bx, by, sw + 18, 16, 3);

    // --- SMALL PLAY ICON (correct glyph + alignment) ---
    u8g2.setFont(u8g2_font_open_iconic_play_2x_t);

    // centered vertically inside 16px button
    int iconY = by + 16;
    int iconX = bx + 1;
    u8g2.setDrawColor(0);
    u8g2.drawGlyph(iconX, iconY, 0x45);
    u8g2.setDrawColor(1);

    // --- BUTTON TEXT ---
    u8g2.setFont(u8g2_font_6x12_mr);
    u8g2.setCursor(bx + 16, by + 12);
    u8g2.print(startText);

    // --- HANDLE SELECT ---
    if (evt == BTN_SELECT) {
        currentScreen = MENU_SCREEN;
    }
}


void MenuDisplay(ButtonEvent evt) {
    if (evt == BTN_DOWN) {
        if (optionIndex < lenOptions - 1) {
            optionIndex++;
        } else {
            optionIndex = 0;
            topIndex = 0;
        }
    }
    if (evt == BTN_UP) {
        if (optionIndex > 0) {
            optionIndex--;
        } else {
            optionIndex = lenOptions - 1;
            topIndex = (lenOptions > visibleLines) ? (lenOptions - visibleLines) : 0;
        }
    }
    if (evt == BTN_SELECT) {
        switch (optionIndex) {
            case 0: currentScreen = NETWORK_SCANNER_SCREEN; break;
            case 1: currentScreen = DEVICE_SCANNER_SCREEN; break;
            case 2: currentScreen = PACKET_SNIFFER_SCREEN; break;
            case 3: currentScreen = CHANNEL_ANALYZER_SCREEN; break;
            case 4: currentScreen = RSSI_METER_SCREEN; break;
            case 5: currentScreen = WARDRIVING_LOGGER_SCREEN; break;
            case 6: currentScreen = FAKE_AP_SCREEN; break;
            case 7: currentScreen = DEAUTH_TESTER_SCREEN; break;
            case 8: currentScreen = BATTERY_MONITOR_SCREEN; break;
        }
    }
    if (evt == BTN_BACK) {
        currentScreen = START_SCREEN;
    }

    if (optionIndex < topIndex) {
        topIndex = optionIndex;
    }
    if (optionIndex >= topIndex + visibleLines) {
        topIndex = optionIndex - visibleLines + 1;
    }

    int linesToShow = (lenOptions < visibleLines) ? lenOptions : visibleLines;

    for (int i = 0; i < linesToShow; i++) {
        int optIdx = topIndex + i;
        int y = (1 + i) * smallLineSpacing - 3;
        if (optIdx == optionIndex) {
            u8g2.setDrawColor(0);
            u8g2.setCursor(0, y);
            u8g2.print(options[optIdx]);
            u8g2.setDrawColor(1);
        }
        else {
            u8g2.setDrawColor(1);
            u8g2.setCursor(0, y);
            u8g2.print(options[optIdx]);
        }
    }
}


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
}


void loop() {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x12_mr);

    ButtonEvent evt = getButtonEvent();

    switch (currentScreen) {
        case START_SCREEN:
            startScreen(evt);
            break;
        case MENU_SCREEN:
            MenuDisplay(evt);
            break;
        case NETWORK_SCANNER_SCREEN:
            NetworkScannerDisplay(evt);
            break;
        case DEVICE_SCANNER_SCREEN:
            DeviceScannerDisplay(evt);
            break;
        case PACKET_SNIFFER_SCREEN:
            PacketSnifferDisplay(evt);
            break;
        case CHANNEL_ANALYZER_SCREEN:
            ChannelAnalyzerDisplay(evt);
            break;
        case RSSI_METER_SCREEN:
            RssiMeterDisplay(evt);
            break;
        case WARDRIVING_LOGGER_SCREEN:
            WardrivingLoggerDisplay(evt);
            break;
        case FAKE_AP_SCREEN:
            FakeAPDisplay(evt);
            break;
        case DEAUTH_TESTER_SCREEN:
            DeauthTesterDisplay(evt);
            break;
        case BATTERY_MONITOR_SCREEN:
            BatteryMonitorDisplay(evt);
            break;
    }

    u8g2.sendBuffer();
}