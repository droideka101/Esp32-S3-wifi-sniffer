//
// Created by ben on 03/02/2026.
//
#include <Arduino.h>
#include "NetworkScannerScreen.h"
#include <U8g2lib.h>
#include <WiFi.h>

#include "ScreenManager.h"
#include "MenuScreen.h"
#include "UIConstants.h"
#include "NetworkInfoScreen.h"


// these still live globally for now
extern U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2;

static bool scanStarted = false;
static bool hasScanned = false;
static int foundNetworks = 0;

static String ssidList[50];
static int rssiList[50];
static int channelList[50];
static int encList[50];

static int currentNetworkIndex = 0;
static int currentNetworkTopIndex = 0;
static const int currentNetworkVisibleLines = 4;

static bool resetRequested = false;


int getSelectedNetworkIndex() {
    return currentNetworkIndex;
}
String getSelectedSSID() {
    return ssidList[currentNetworkIndex];
}
int getSelectedRSSI() {
    return rssiList[currentNetworkIndex];
}
int getSelectedChannel() {
    return channelList[currentNetworkIndex];
}
int getSelectedEncryption() {
    return encList[currentNetworkIndex];
}


void requestNetworkScanReset() {
    resetRequested = true;
}


void NetworkScannerEnter() {
    if (resetRequested) {
        scanStarted = false;
        hasScanned = false;
        foundNetworks = 0;
        currentNetworkIndex = 0;
        currentNetworkTopIndex = 0;
        resetRequested = false;
    }
}

void NetworkScannerExit() {

}

void NetworkScannerEvent(ButtonEvent evt) {
    if (!hasScanned) return;

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
        requestScreen(&NetworkInfoScreen);
    }

    if (evt == BTN_BACK) {
        requestScreen(&MenuScreen);
    }

    if (currentNetworkIndex < currentNetworkTopIndex) {
        currentNetworkTopIndex = currentNetworkIndex;
    }
    if (currentNetworkIndex >= currentNetworkTopIndex + currentNetworkVisibleLines) {
        currentNetworkTopIndex = currentNetworkIndex - currentNetworkVisibleLines + 1;
    }

}

void NetworkScannerDraw() {
    if (!scanStarted) {
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

        for (int i = 0; i < foundNetworks; i++) {
            ssidList[i] = WiFi.SSID(i);
            rssiList[i] = WiFi.RSSI(i);
            channelList[i] = WiFi.channel(i);
            encList[i] = WiFi.encryptionType(i);
        }

        hasScanned = true;

    }

    if (foundNetworks == 0) {
        u8g2.drawStr(0, 24, "No Networks Found");
    } else {
        u8g2.setCursor(0, 12);
        u8g2.print(" Networks Found: ");
        u8g2.print(foundNetworks);
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

Screen NetworkScannerScreen = {
    NetworkScannerEnter,
    NetworkScannerEvent,
    NetworkScannerDraw,
    NetworkScannerExit
};
