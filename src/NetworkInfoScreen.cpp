//
// Created by ben on 04/02/2026.
//

#include "NetworkInfoScreen.h"

#include <Arduino.h>
#include <U8g2lib.h>
#include "ScreenManager.h"
#include "UIConstants.h"
#include "StartScreen.h"
#include "NetworkScannerScreen.h"


// these still live globally for now
extern U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2;

static String ssid;
static int rssi;
static int channel;
static int encryption;

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


void NetworkInfoEnter() {
    ssid = getSelectedSSID();
    rssi = getSelectedRSSI();
    channel = getSelectedChannel();
    encryption = getSelectedEncryption();

}

void NetworkInfoExit() {}

void NetworkInfoEvent(ButtonEvent evt) {
    if (evt == BTN_DOWN) {
    }

    if (evt == BTN_UP) {
    }

    if (evt == BTN_SELECT) {
    }

    if (evt == BTN_BACK) {
        requestScreen(&NetworkScannerScreen);
    }
}

void NetworkInfoDraw() {
    u8g2.setCursor(0, 12);
    u8g2.print("Network Info");

    u8g2.setCursor(0, 24);
    u8g2.print("SSID: ");
    u8g2.print(ssid);

    u8g2.setCursor(0, 36);
    u8g2.print("RSSI: ");
    u8g2.print(rssi);

    u8g2.setCursor(0, 48);
    u8g2.print("Channel: ");
    u8g2.print(channel);

    u8g2.setCursor(0, 60);
    u8g2.print("Encryption: ");
    u8g2.print(encToString(encryption));
}


Screen NetworkInfoScreen = {
    NetworkInfoEnter,
    NetworkInfoEvent,
    NetworkInfoDraw,
    NetworkInfoExit
};
