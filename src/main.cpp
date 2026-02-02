#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <string.h>
#include <WiFi.h>
#include "Screen.h"
#include "MenuScreen.h"
#include "StartScreen.h"
#include "UIConstants.h"
#include "ScreenManager.h"


// SH1106 I2C constructor for 1.3" 128x64 OLED
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

const char VERSION[7] = "v0.5.0";

int optionIndex = 0;
int optionsTopIndex = 0;
const char* options[] = {" Network Scanner ", " Device Scanner ", " Packet Sniffer ", " Channel Analyzer ", " RSSI Meter ", " Wardriving Logger ", " Fake AP ", " Deauth Tester ", " Battery Monitor "};
int lenOptions = sizeof(options) / sizeof(options[0]);

bool comingFromInfo = false;
bool scanStarted = false;
bool hasScanned = false;
int foundNetworks = 0;
String ssidList[50];
int rssiList[50];
int channelList[50];
int encList[50];
int currentNetworkIndex = 0;
int currentNetworkTopIndex = 0;
const int currentNetworkVisibleLines = 4;


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

String encToString(int enc) {
    switch (enc) {
        case WIFI_AUTH_OPEN: return "Open";
        case WIFI_AUTH_WEP: return "WEP";
        case WIFI_AUTH_WPA_PSK: return "WPA";
        case WIFI_AUTH_WPA2_PSK: return "WPA2";
        case WIFI_AUTH_WPA_WPA2_PSK: return "WPA/WPA2";
        case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2-Ent";
        case WIFI_AUTH_WPA3_PSK: return "WPA3";
        case WIFI_AUTH_WPA2_WPA3_PSK: return "WPA2/WPA3";
        case WIFI_AUTH_WAPI_PSK: return "WAPI";
        default: return "Unknown";
    }
}


void NetworkInfoDisplay(ButtonEvent evt) {
    u8g2.setCursor(0, 12);
    u8g2.print("Network Info:");

    u8g2.setCursor(0, 24);
    u8g2.print("SSID: ");
    u8g2.print(ssidList[currentNetworkIndex]);

    u8g2.setCursor(0, 36);
    u8g2.print("RSSI: ");
    u8g2.print(rssiList[currentNetworkIndex]);

    u8g2.setCursor(0, 48);
    u8g2.print("Channels: ");
    u8g2.print(channelList[currentNetworkIndex]);

    u8g2.setCursor(0, 60);
    u8g2.print("Encryption: ");
    u8g2.print(encToString(encList[currentNetworkIndex]));  // Later we can translate this code into WPA/WPA2/WEP etc.

    if (evt == BTN_BACK) {
        comingFromInfo = true;
        requestScreen(&MenuScreen);   // or another Screen*
    }
}


void NetworkScannerDisplay(ButtonEvent evt) {
    if (!scanStarted && !comingFromInfo) {
        u8g2.drawStr(0, 12, "network scanner");
        u8g2.drawStr(0, 24, "scanning...");
        scanStarted = true;
        return;
    }
    u8g2.drawStr(0, 12, "network scanner");

    if (!hasScanned) {
        WiFi.mode(WIFI_STA);
        WiFi.disconnect();
        delay(100);

        foundNetworks = WiFi.scanNetworks();
        if (foundNetworks > 50) foundNetworks = 50;

        Serial.print(foundNetworks);
        Serial.println(" networks found");

        for (int i = 0; i < foundNetworks; i++) {
            ssidList[i] = WiFi.SSID(i);
            rssiList[i] = WiFi.RSSI(i);
            channelList[i] = WiFi.channel(i);
            encList[i] = WiFi.encryptionType(i);
            Serial.print("SSID: ");
            Serial.print(ssidList[i]);
            Serial.print(", RSSI: ");
            Serial.print(rssiList[i]);
            Serial.print(", Channel: ");
            Serial.print(channelList[i]);
            Serial.print(", Encryption: ");
            Serial.println(encList[i]);
        }




        hasScanned = true;
    }

    if (foundNetworks == 0) {
        u8g2.drawStr(0, 24, "No Networks Found");
        Serial.println("No networks found");
    } else {


        u8g2.setCursor(0, 12);
        u8g2.print("  Networks Found: ");
        u8g2.print(foundNetworks);

        if (evt == BTN_DOWN) {
            if (currentNetworkIndex < foundNetworks - 1) {
                currentNetworkIndex++;
            } else {
                currentNetworkIndex = 0;
                currentNetworkTopIndex = 0;
            }
        }
        if (evt == BTN_UP) {
            if (currentNetworkIndex > 0) {
                currentNetworkIndex--;
            } else {
                currentNetworkIndex = foundNetworks - 1;
                currentNetworkTopIndex = (foundNetworks > currentNetworkVisibleLines) ? (foundNetworks - currentNetworkVisibleLines) : 0;
            }
        }
        if (evt == BTN_SELECT) {
            requestScreen(&MenuScreen);   // or another Screen*
            return;
        }

        if (currentNetworkIndex < currentNetworkTopIndex) {
            currentNetworkTopIndex = currentNetworkIndex;
        }
        if (currentNetworkIndex >= currentNetworkTopIndex + currentNetworkVisibleLines) {
            currentNetworkTopIndex = currentNetworkIndex - currentNetworkVisibleLines + 1;
        }

        int linesToShow = (foundNetworks < currentNetworkVisibleLines) ? foundNetworks : currentNetworkVisibleLines;

        for (int i = 0; i < linesToShow; i++) {
            int netIdx = currentNetworkTopIndex + i;
            int y = (2 + i) * SMALL_LINE_SPACING - 3;
            if (netIdx == currentNetworkIndex) {
                u8g2.setDrawColor(0);
                u8g2.setCursor(0, y);
                u8g2.print(ssidList[netIdx]);
                u8g2.setDrawColor(1);
            }
            else {
                u8g2.setDrawColor(1);
                u8g2.setCursor(0, y);
                u8g2.print(ssidList[netIdx]);
            }

        }

    }

    if (evt == BTN_BACK) {
        comingFromInfo = false;
        scanStarted = false;
        hasScanned = false;
        requestScreen(&MenuScreen);
    }
}


void DeviceScannerDisplay(ButtonEvent evt) {
    u8g2.setCursor(0, 12);
    u8g2.print("device scanner selected");

    if (evt == BTN_BACK) {
        requestScreen(&MenuScreen);
    }
}


void PacketSnifferDisplay(ButtonEvent evt) {
    u8g2.setCursor(0, 12);
    u8g2.print("packet sniffer selected");

    if (evt == BTN_BACK) {
        requestScreen(&MenuScreen);
    }
}


void ChannelAnalyzerDisplay(ButtonEvent evt) {
    u8g2.setCursor(0, 12);
    u8g2.print("channel analyzer selected");

    if (evt == BTN_BACK) {
        requestScreen(&MenuScreen);
    }
}


void RssiMeterDisplay(ButtonEvent evt) {
    u8g2.setCursor(0, 12);
    u8g2.print("RSSI meter selected");

    if (evt == BTN_BACK) {
        requestScreen(&MenuScreen);
    }
}


void WardrivingLoggerDisplay(ButtonEvent evt) {
    u8g2.setCursor(0, 12);
    u8g2.print("wardriving logger selected");

    if (evt == BTN_BACK) {
        requestScreen(&MenuScreen);
    }
}


void FakeAPDisplay(ButtonEvent evt) {
    u8g2.setCursor(0, 12);
    u8g2.print("fake AP selected");

    if (evt == BTN_BACK) {
        requestScreen(&MenuScreen);
    }
}


void DeauthTesterDisplay(ButtonEvent evt) {
    u8g2.setCursor(0, 12);
    u8g2.print("Deauth tester selected");

    if (evt == BTN_BACK) {
        requestScreen(&MenuScreen);
    }
}


void BatteryMonitorDisplay(ButtonEvent evt) {
    u8g2.setCursor(0, 12);
    u8g2.print("Battery monitor selected");

    if (evt == BTN_BACK) {
        requestScreen(&MenuScreen);
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