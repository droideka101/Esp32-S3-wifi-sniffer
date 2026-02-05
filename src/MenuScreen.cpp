//
// Created by ben on 01/02/2026.
//
#include <Arduino.h>
#include "MenuScreen.h"
#include <U8g2lib.h>
#include "ScreenManager.h"
#include "UIConstants.h"

#include "StartScreen.h"
#include "NetworkScannerScreen.h"
#include "DeviceScannerScreen.h"
#include "PacketSnifferScreen.h"
#include "ChannelAnalyzerScreen.h"
#include "RSSIMeterScreen.h"
#include "WarDrivingLoggerScreen.h"
#include "FakeAPScreen.h"
#include "DeauthTesterScreen.h"
#include "BatteryMonitorScreen.h"



// these still live globally for now
extern U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2;
// MenuScreen.cpp
static const char* options[] = {
    " Network Scanner ",
    " Device Scanner ",
    " Packet Sniffer ",
    " Channel Analyzer ",
    " RSSI Meter ",
    " Wardriving Logger ",
    " Fake AP ",
    " Deauth Tester ",
    " Battery Monitor "
};

static const int lenOptions = sizeof(options) / sizeof(options[0]);
static int optionIndex = 0;
static int optionsTopIndex = 0;

void menuEnter() {}

void menuExit() {}

void menuEvent(ButtonEvent evt) {
    if (evt == BTN_DOWN) {
        if (optionIndex < lenOptions - 1) optionIndex++;
        else optionIndex = 0;
    }

    if (evt == BTN_UP) {
        if (optionIndex > 0) optionIndex--;
        else optionIndex = lenOptions - 1;
    }

    if (evt == BTN_SELECT) {
        switch (optionIndex) {
            case 0:
                requestNetworkScanReset();
                requestScreen(&NetworkScannerScreen);
                break;
            case 1:
                requestScreen(&DeviceScannerScreen);
                break;
            case 2:
                requestScreen(&PacketSnifferScreen);
                break;
            case 3:
                requestScreen(&ChannelAnalyzerScreen);
                break;
            case 4:
                requestScreen(&RSSIMeterScreen);
                break;
            case 5:
                requestScreen(&WarDrivingLoggerScreen);
                break;
            case 6:
                requestScreen(&FakeAPScreen);
                break;
            case 7:
                requestScreen(&DeauthTesterScreen);
                break;
            case 8:
                requestScreen(&BatteryMonitorScreen);
                break;
            default:
                requestScreen(&MenuScreen);
        }
    }

    if (evt == BTN_BACK) {
        requestScreen(&StartScreen);
    }

    if (optionIndex < optionsTopIndex) {
        optionsTopIndex = optionIndex;
    }

    if (optionIndex >= optionsTopIndex + MENU_VISIBLE_LINES) {
        optionsTopIndex = optionIndex - MENU_VISIBLE_LINES + 1;
    }

}

void menuDraw() {
    for (int i = 0; i < MENU_VISIBLE_LINES; i++) {
        int idx = optionsTopIndex + i;
        if (idx >= lenOptions) break;

        int y = (1 + i) * SMALL_LINE_SPACING - 3;
        if (idx == optionIndex) {
            u8g2.setDrawColor(0);
            u8g2.setCursor(0, y);
            u8g2.print(options[idx]);
            u8g2.setDrawColor(1);
        } else {
            u8g2.setCursor(0, y);
            u8g2.print(options[idx]);
        }
    }
}

Screen MenuScreen = {
    menuEnter,
    menuEvent,
    menuDraw,
    menuExit
};
